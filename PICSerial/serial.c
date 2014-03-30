/*
** Serial
** UART2 RS232 asynchronous communication demonstration
*/
#include <config.h>

// circ buf for uart reception
#include "circ_buf.h"
#include "uart.h"

#include "LCD.h"

struct CONSOLE {
    unsigned char (*read)(void);
    void (*write)(const unsigned char *, unsigned int);
    int (*available)(void);
};

struct HAL {
    struct CONSOLE *console;
};

enum Directive {
    TX_ERR = 0,
    SEND_AGAIN = (1<<1),
    ACK = (1<<2),
    GET_ORIENTATION = (1<<3),
    NUDGE_ROLL_LEFT = (1<<4),
    NUDGE_ROLL_RIGHT = (1<<5),
    NUDGE_UP = (1<<6),
    NUDGE_DOWN = (1<<7),
    NUDGE_YAW_CCW = (1<<8),
    NUDGE_YAW_CW = (1<<9),
    NUDGE_PITCH_DOWN = (1<<10),
    NUDGE_PITCH_UP = (1<<11)
};

/**
 * the ATMega2560 is little endian. Woot.
 * Network is big endian. Wooter.
 */
unsigned long PIC_htonl(unsigned long host)
{
    return ((host&0x000000FF)<<24) |
           ((host&0x0000FF00)<<8)  |
           ((host&0x00FF0000)>>8)  |
           ((host&0xFF000000)>>24);
}
#define htonl PIC_htonl

unsigned int PIC_htons(unsigned int host)
{
    return ((host&0x00FF)<<8) |
           ((host&0xFF00)>>8);
}
#define htons PIC_htons

unsigned long PIC_ntohl(unsigned long net)
{
    return ((net&0x000000FF)<<24) |
           ((net&0x0000FF00)<<8)  |
           ((net&0x00FF0000)>>8)  |
           ((net&0xFF000000)>>24);

}
#define ntohl PIC_ntohl

unsigned int PIC_ntohs(unsigned int net)
{
    return ((net&0x00FF)<<8) |
           ((net&0xFF00)>>8);
}
#define ntohs PIC_ntohs

void pack_uint32(unsigned long val, unsigned char *buf)
{
    unsigned long net = htonl(val);
    buf[0] = (unsigned char)((net&0xFF000000) >> 24);
    buf[1] = (unsigned char)((net&0x00FF0000) >> 16);
    buf[2] = (unsigned char)((net&0x0000FF00) >>  8);
    buf[3] = (unsigned char)((net&0x000000FF));
}

void unpack_uint32(unsigned char *buf, unsigned long *val)
{
    unsigned long net = 0;
    net |= (unsigned long)(buf[0]) << 24;
    net |= (unsigned long)(buf[1]) << 16;
    net |= (unsigned long)(buf[2]) <<  8;
    net |= (unsigned long)(buf[3]);
    *(val) = ntohl(net);
}

struct Command {
    unsigned long m_directives;
};
#define CMDMSG 2
#define CMD_PACKED_SIZE 5
#define ORT_PACKED_SIZE 13
#define include(cmd, dir) (cmd).m_directives |= (unsigned long)(dir)
#define exclude(cmd, dir) (cmd).m_directives &= ~(unsigned long)(dir)

#define BEGIN_UART_MESSAGE 128

void pack_cmd(struct Command* cmd, unsigned char *buf, int len)
{
    if(len < CMD_PACKED_SIZE) {
        return;
    }
    buf[0] = CMDMSG;
    pack_uint32(cmd->m_directives, buf);
}

void unpack_cmd(struct Command* cmd, unsigned char *buf, int len)
{
    if(len < CMD_PACKED_SIZE) {
        cmd->m_directives = TX_ERR;
        return;
    }
    if(buf[0] != CMDMSG) {
        cmd->m_directives = TX_ERR;
        return;
    }
    unpack_uint32(&(buf[1]), &(cmd->m_directives));
}

// global HAL
static struct HAL hal;

struct Command check_for_command(void)
{
    // state if we have not received a full message yet
    static int beganMessage = 0;
    static int haveSize = 0;
    static unsigned long messageSize = 0;

    struct Command cmd;
    cmd.m_directives = TX_ERR;

    int bytesAvail = hal.console->available();

    // if we have not begun reading in a message, check for one
    if(!beganMessage && bytesAvail > 0) {
        unsigned char byte = hal.console->read();
        if(byte == BEGIN_UART_MESSAGE) {
            beganMessage = 1;
        }
    }

    if(beganMessage && !haveSize && bytesAvail >= 4) {
        unsigned char lenBuf[4];
        int i = 0;
        for(i = 0; i < 4; ++i) {
            lenBuf[i] = hal.console->read();
        }
        unpack_uint32(lenBuf, &messageSize);
        if(messageSize == CMD_PACKED_SIZE) {
            haveSize = 1;
        }
        else {
            haveSize = 0;
            beganMessage = 0;
        }
    }

    if(beganMessage && haveSize && bytesAvail >= messageSize) {
        unsigned char cmdBuf[CMD_PACKED_SIZE];
        unsigned int i = 0;
        for(i = 0; i < messageSize; ++i) {
            cmdBuf[i] = hal.console->read();
        }
        unpack_cmd(&cmd, cmdBuf, CMD_PACKED_SIZE);
        beganMessage = 0;
        haveSize = 0;
    }
    return cmd;
}

int main(void)
{
    InitLCD();
    InitU2();

    // set up the fake hal
    struct CONSOLE console;
    console.read = getU2;
    console.write = writeU2;
    console.available = availableU2;

    hal.console = &console;

    char myStr[] = "MyString\r\n";
    hal.console->write(myStr, sizeof(myStr));
    putsLCD("Hello World!");

    // emulate an ArduPilot, I guess. Dude
    int thisDraw = 0;
    int lastDraw = 0;
    while(1) {
        char hi = 'a';
        hal.console->write(&hi, 1);
        /*
        struct Command cmd;
        unsigned char sendBuf[1+4+CMD_PACKED_SIZE];
        sendBuf[0] = BEGIN_UART_MESSAGE;
        pack_uint32(CMD_PACKED_SIZE, &(sendBuf[1]));
        pack_cmd(&cmd, &(sendBuf[5]), CMD_PACKED_SIZE);
        hal.console->write(sendBuf, 10);
        */
        /*
        // If I receive an ACK, send one back
        struct Command cmd = check_for_command();
        if(cmd.m_directives == ACK) {
            lastDraw = thisDraw;
            thisDraw = 1;
            putsLCD("Got ACK!");
            unsigned char sendBuf[1+4+CMD_PACKED_SIZE];
            sendBuf[0] = BEGIN_UART_MESSAGE;
            pack_uint32(CMD_PACKED_SIZE, &(sendBuf[1]));
            pack_cmd(&cmd, &(sendBuf[5]), CMD_PACKED_SIZE);
            hal.console->write(sendBuf, 10);
        }
        else {
            lastDraw = thisDraw;
            thisDraw = 2;
        }

        if(thisDraw != lastDraw) {
            ClrLCD();
            if(thisDraw == 1) {
                putsLCD("Got ACK!");
            }
            else if(thisDraw == 2) {
                putsLCD("Got Not ACK!");
            }
        }
     */
    }
    
}// main
