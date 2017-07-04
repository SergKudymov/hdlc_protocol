/* 
 * File:   crc.h
 * Author: serg
 *
 * Created on March 19, 2017, 5:02 PM
 */

#ifndef CRC_H
#define CRC_H

#include <cstdint>


#define CRC16


#if defined(CRC8)

typedef uint8_t  crc_t;

#define INIT                    0xff
#define POLYNOMIAL		0x7
#define WIDTH                   8
#define XOROUT                  0x0000


#elif defined(CRC16)

typedef uint16_t  crc_t;

#define INIT                    0x1D0F
#define POLYNOMIAL		0x1021
#define WIDTH                   16
#define XOROUT                  0x0


#elif defined(CRC32)

typedef uint32_t  crc_t;

#define INIT                    0xffffffff
#define POLYNOMIAL		0x4c11db7
#define WIDTH                   32
#define XOROUT                  0x0000


/*
 * #elif defined(YOUR_CRC)
 * 
 * #define TABLE_LENGHT         256
 * #define POLYNOMIAL           YOUR POLY
 * define WIDTH_POLY            YOUR POLY_WIDTH
 * 
 * //if your poly width 16 bit is BIT_FOR_CHEKING -> 0x8000 (1000 0000 0000 0000)
 * #define BIT_FOR_CHEKING 	0x8xxxxx
 * 
 
 */


#endif



extern crc_t crc_normal(uint8_t*, uint32_t);



#endif /* CRC_H */



