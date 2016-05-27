#ifndef _MEMORY_OPERATIONS_H_
#define _MEMORY_OPERATIONS_H_
#include "TypeDef.h"



template< typename T >
inline void memory_set( T * array, T val, UI size )
{
	while(size--)
	{
		*array++ = val;
	}
}

template< typename T >
inline void memory_clear( T * array, UI size )
{
	memory_set<T>( array, (T)0, size );
}



template< typename T >
inline void memory_copy( T * dest, const T * src, UI size )
{
	while(size--)
	{
		*dest++ = *src++;
	}
}

template< typename T >
inline bool memory_compare(const T * array1, const T * array2, UI size )
{
	while(size--)
	{
		if (*array1++ != *array2++)
		{
			return false;
		}
	}
	return true;
}



//От следующих функций наверное надо отказаться, из-за них только лишний .cpp в проекте
extern void memset_32(void * mem, U32 val, UI size);
extern void memcpy_32(void *__restrict dest, const void *__restrict src, UI size);
extern int memcmp_32(void * data1, void * data2, UI size);

extern void memset_16(void * mem, U16 val, UI size);
extern void memcpy_16(void *__restrict dest, const void *__restrict src, UI size);
extern int memcmp_16(void * data1, void * data2, UI size);

extern void memset_8(void * mem, U8 val, UI size);
extern void memcpy_8(void *__restrict dest, const void *__restrict src, UI size);
extern int memcmp_8(void * data1, void * data2, UI size);


#endif

