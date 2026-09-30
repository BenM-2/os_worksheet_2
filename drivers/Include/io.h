/**
 * @file io.h
 * @author Ben Marples
 * @brief Holds the whole I/O interface
 */

#ifndef IO_H
#define IO_H

//------------------------------------------------------------------------------
// Definitions

/** outb:
* Sends the given data to the given I/O port. Defined in io.s
*
* @param port The I/O port to send the data to
* @param data The data to send to the I/O port 25
*/
void outb(unsigned short port, unsigned char data);


/** inb:
; move the address of the I/O port to the dx register
; read a byte from the I/O port and store it in the al register
; return the read byte
* Read a byte from an I/O port.
*
* @param port The address of the I/O port
* @return
The read byte
*/
unsigned char inb(unsigned short port);
#endif /* IO_H */
