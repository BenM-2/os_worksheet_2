/**
 * @file pic.h
 * @author UNKNOWN
 * @brief Holds the whole I/O interface
 */

#ifndef INCLUDE_PIC_H
#define INCLUDE_PIC_H

#include "type.h"

/* I/O port*/
#define PIC_1 0x20 /*IO baseaddress for masterPIC */
#define PIC_2 0xA0 /*IO baseaddress for slavePIC */

#define PIC_1_COMMAND PIC_1
#define PIC_1_DATA (PIC_1 + 1)

#define PIC_2_COMMAND PIC_2
#define PIC_2_DATA (PIC_2 + 1)

#define PIC_1_OFFSET 0x20
#define PIC_2_OFFSET 0x28
#define PIC_2_END PIC_2_OFFSET +7

#define PIC_1_COMMAND_PORT 0x20
#define PIC_2_COMMAND_PORT 0xA0

#define PIC_ACKNOWLEDGE 0x20

#define PIC_ICW1_ICW4 0x01      /*ICW4(not) needed */
#define PIC_ICW1_SINGLE 0x02    /*Single(cascade) mode */
#define PIC_ICW1_INTERVAL4 0x04 /*Calladdress interval4 (8) */
#define PIC_ICW1_LEVEL 0x08     /*Leveltriggered (edge)mode */
#define PIC_ICW1_INIT 0x10      /*Initialization- required! */

#define PIC_ICW4_8086 0x01       /*8086/88(MCS-80/85) mode */
#define PIC_ICW4_AUTO 0x02       /*Auto(normal) EOI */
#define PIC_ICW4_BUF_SLAVE 0x08  /*Bufferedmode/slave */
#define PIC_ICW4_BUF_MASTER 0x0C /*Buffered mode/master*/
#define PIC_ICW4_SFNM 0x10       /*Special fully nested (not)*/

void pic_remap(s32int offset1, s32int offset2);
void pic_acknowledge(u32int interrupt);
#endif /* INCLUDE_PIC_H */
