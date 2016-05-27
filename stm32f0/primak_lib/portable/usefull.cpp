#include "usefull.h"

U32 U32_big_endian( void * ptr)
{
	BYTE *l_ptr = (BYTE *)ptr;
	return ( (UI)l_ptr[0]<< 24) + ((UI)l_ptr[1]<< 16) + ((UI)l_ptr[2]<< 8) + ((UI)l_ptr[3]);
}

void U32_big_endian_to_bytes(U32 val, void * ptr)
{
	BYTE *l_ptr = (BYTE *)ptr;
	l_ptr[0] = val >> 24;
	l_ptr[1] = val >> 16;
	l_ptr[2] = val >> 8 ;
	l_ptr[3] = val;
}

U32 U32_little_endian( void * ptr)
{
	BYTE *l_ptr = (BYTE *)ptr;
	return ( (UI)l_ptr[3]<< 24) + ((UI)l_ptr[2]<< 16) + ((UI)l_ptr[1]<< 8) + ((UI)l_ptr[0]);
}

void U32_little_endian_to_bytes(U32 val, void * ptr)
{
	BYTE *l_ptr = (BYTE *)ptr;
	l_ptr[3] = val >> 24;
	l_ptr[2] = val >> 16;
	l_ptr[1] = val >> 8 ;
	l_ptr[0] = val;
}

U32 U32_change_endian(U32 val)
{
	U8 temp[4] = {0,0,0,0};
	U32_big_endian_to_bytes(val, temp);
	return U32_little_endian( temp);
}

__pure int get_bit_num (UI number)
{
	int res = 0;
	if (number == 0)
		return -1;
	while ((number & 1) == 0)
	{
		number >>= 1;
		res++;
	}	
	if (number & ~1)
		return -2;
	return res;	
}

__pure int true_get_bit_num (UI number)
{
	int res = 0;
	if (number == 0)
		return -1;
	while ((number & 1) == 0)
	{
		number >>= 1;
		res++;
	}	
	return res;	
}

__pure UI get_num_of_bit (UI number)
{
	int res = 0;
	if (number == 0)
		return 0;
	while (number)
	{
		if (number & 1)
			res++;
		number >>= 1;		
	}	
	return res;	
}

