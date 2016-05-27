#ifndef _USEFULL_H_
#define _USEFULL_H_
#include "TypeDef.h"

U32 U32_big_endian( void * ptr);
void U32_big_endian_to_bytes(U32 val, void * ptr);
U32 U32_little_endian( void * ptr);
void U32_little_endian_to_bytes(U32 val, void * ptr);
__pure int get_bit_num (UI number);//выдает смещение первого бита, но если установленных битов больше одного выдает -2
__pure int true_get_bit_num (UI number);//выдает смещение первого установленного бита
__pure UI get_num_of_bit(UI number);
U32 U32_change_endian(U32 val);

#endif
