#include "uart.h"
#include <xc.h>

static struct circ_buf g_uartBuf;
static int g_err = ALL_GOOD;

void _ISRFAST _U2RXInterrupt(void)
{
    if(U2STAbits.OERR) {
        g_err = BUF_FULL;
        U2STAbits.OERR = 0;
    }
    else {
        int status = 0;
        append_fast(g_uartBuf, (U2RXREG)&0xFFu, &status);
        if(status < 0) {
            g_err = BUF_FULL;
        }
        append_fast(g_uartBuf, *((&U2RXREG)+1)&0xFFu, &status);
        if(status < 0) {
            g_err = BUF_FULL;
        }
        append_fast(g_uartBuf, *((&U2RXREG)+2)&0xFFu, &status);
        if(status < 0) {
            g_err = BUF_FULL;
        }
        append_fast(g_uartBuf, *((&U2RXREG)+3)&0xFFu, &status);
        if(status < 0) {
            g_err = BUF_FULL;
        }
    }
    IFS1bits.U2RXIF = 0;
    return;
}

void InitU2( void)
{
    g_uartBuf;
    g_uartBuf.size = BUF_LEN;
    g_uartBuf.begin = 0;
    g_uartBuf.end = 0;
    U2BRG   = BRATE;
    U2MODE  = U_ENABLE;
    U2STA   = U_TX;

    // enable interrupts
    IFS1bits.U2RXIF = 0;
    IEC1bits.U2RXIE = 1;

    g_err = ALL_GOOD;
} // initU2


// send a character to the UART2 serial port
int putU2(unsigned char c)
{
    while ( U2STAbits.UTXBF);   // wait while Tx buffer full
    U2TXREG = c;
    g_err = ALL_GOOD;
    return c;
} // putU2


// wait for a new character to arrive to the UART2 serial port
unsigned char getU2( void)
{
    int recved = pop(&g_uartBuf);
    if(recved < 0) {
        g_err = BUF_EMPTY;
        return (unsigned char)255;
    }
    g_err = ALL_GOOD;
    return (unsigned char)recved;
}// getU2

// Check to see how many bytes are available in the buffer
int availableU2(void)
{
    int len = length(&g_uartBuf);
    if(len == 0) {
        g_err = BUF_EMPTY;
    }
    else if(len == g_uartBuf.size) {
        g_err = BUF_FULL;
    }
    return len;
}

void writeU2(const unsigned char *buf, unsigned int bufLen)
{
    int i = 0;
    for(i = 0; i < bufLen; ++i) {
        putU2(bufLen);
    }
    g_err = ALL_GOOD;
    return;
}