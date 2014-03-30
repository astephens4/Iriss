/* 
 * File:   uart.h
 * Author: alex
 *
 * Created on March 29, 2014, 1:13 PM
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <circ_buf.h>

#define ALL_GOOD 0
#define BUF_EMPTY 1
#define BUF_FULL 2

// timing and baud rate calculations
#define BRATE    34         // 115200 baud (BREGH=1)
#define U_ENABLE 0x8008     // enable the UART peripheral, 8N1
#define U_TX     0x04C0     // enable transmission, rx interrupts every 4 bytes

/**
 * Initialize UART2 module
 */
void InitU2(void);

/**
 * Get the least recently received byte from the buffer
 * @return Least recently received byte
 */
unsigned char getU2(void);

/**
 * Check to see how many bytes are available to read in
 * @return Number of available bytes
 */
int availableU2(void);

/**
 * Write a byte out of the UART2 port
 * @param c Byte to send out
 * @return The byte that was sent
 */
int putU2(unsigned char c);

/**
 * Write the contents of a buffer out UART2
 * @param buf Buffer to send
 * @param bufLen Number of bytes to send in the buffer
 */
void writeU2(const unsigned char *buf, unsigned int bufLen);


#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

