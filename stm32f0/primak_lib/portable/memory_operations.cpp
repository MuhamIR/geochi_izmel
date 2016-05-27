#include "memory_operations.h"

void memset_32(void * mem, U32 val, UI size)
{
	U32 * ptr = (U32 *) mem;
	while(size--)
		*ptr++ = val;
}

void memcpy_32(void * dest, const void * src, UI size)
{
	U32 * write_ptr = (U32 *) dest;
	U32 * read_ptr = (U32 *) src;
	while(size--)
		*write_ptr++ = *read_ptr++;
}

int memcmp_32(void * data1, void * data2, UI size)  
{
	U32 * ptr1 = (U32 *) data1;
	U32 * ptr2 = (U32 *) data2;
	UI counter = size;
	while(counter--)
		if(*ptr1++ != *ptr2++)
			return (size - counter);
	return 0;
		
}

void memset_16(void * mem, U16 val, UI size)
{
	U16 * ptr = (U16 *) mem;
	while(size--)
		*ptr++ = val;
}

void memcpy_16(void *__restrict dest, const void *__restrict src, UI size)
{
	U16 * write_ptr = (U16 *) dest;
	U16 * read_ptr = (U16 *) src;
	while(size--)
		*write_ptr++ = *read_ptr++;
}

int memcmp_16(void * data1, void * data2, UI size)  
{
	U16 * ptr1 = (U16 *) data1;
	U16 * ptr2 = (U16 *) data2;
	UI counter = size;
	while(counter--)
		if(*ptr1++ != *ptr2++)
			return (size - counter);
	return 0;		
}

void memset_8(void * mem, U8 val, UI size)
{
	U8 * ptr = (U8 *) mem;
	while(size--)
		*ptr++ = val;
}

void memcpy_8(void * dest, const void * src, UI size)
{
	U8 * write_ptr = (U8 *) dest;
	U8 * read_ptr = (U8 *) src;
	while(size--)
		*write_ptr++ = *read_ptr++;
}

int memcmp_8(void * data1, void * data2, UI size)  
{
	U8 * ptr1 = (U8 *) data1;
	U8 * ptr2 = (U8 *) data2;
	UI counter = size;
	while(counter--)
		if(*ptr1++ != *ptr2++)
			return (size - counter);
	return 0;		
}



