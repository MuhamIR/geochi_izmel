#ifndef __MACROS_H_
#define __MACROS_H_

#include "TypeDef.h"

//макросы для улучшения читабельности при работе с большими числами, например количеством памяти или скоростями
//например 250 K = 250000
#define M		*1000000
#define MHz		*1000000
#define K		*1000
#define KHz		*1000
#define Kb		*1024
#define Mb		*1024*1024

//ELEMENTS(x) - ПРИМЕНЯТЬ С ОСТОРОЖНОСТЬЮ!! Макрос для получения количества элементов в СТАТИЧЕСКИ выделенном массиве, для ДИНАМИЧЕСКИХ массивов(по сути указателей) компилируется, 
//но дает неправильный результат
#define ELEMENTS(x) (sizeof(x) / sizeof((x)[0]))

//следующие макросы обрамляют часть кода, которая должна исполняться олин раз в prescaler проходов функцией этого участка
#define STATIC_PRESCALER_START(prescaler)		{static UI counter = 0; if (++counter >= prescaler) { counter = 0;
#define STATIC_PRESCALER_END	}}

//STATIC_PRESCALER_START_INSTANT_FIRST_TIME - это версия STATIC_PRESCALER_START, которая выполняет впервые действие заполняют сразу, а не по накоплению счетчика
#define STATIC_PRESCALER_START_INSTANT_FIRST_TIME(prescaler)		{static UI counter = (prescaler) - 1; if (++counter == (prescaler)) { counter = 0;

#define ff(counter_name, limit)      for( UI counter_name = 0; counter_name < (limit); counter_name++)



#if !defined(ABS)
#define ABS(val)	( ((val) > 0)? (val) : (-(val)))
#endif
#if !defined(MAX)
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
#if !defined(MIN)
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

//N_BIT_MASK - создает число, в котором первые n бит - единицы (n <= 32). Например N_BIT_MASK(5) == 0x1Ful
#define N_BIT_MASK(n)			((1ul << (n)) - 1)

//Макросы для работы с битами и битовыми операторами
//не рекомендую в новых разработках, так как считаю, что писать напрямую более очевидно(при базовом накопленном опыте в битовых операциях C++)
#define re_SetBits(X,Y) (X |= Y)
#define re_ClrBits(X,Y) (X &= ~Y)
#define re_TglBits(X,Y) (X ^= Y)
#define re_ChkBits(X,Y) (X & Y)
#define re_IsBitsSet(X,Y) ((X & Y) != 0)
#define re_IsBitsClr(X,Y) ((X & Y) == 0)

//Макросы для более удобной записи битов
#define BIT(num) ((U64)1 << (num))
#define BIT0 (1ul << 0)
#define BIT1 (1ul << 1)
#define BIT2 (1ul << 2)
#define BIT3 (1ul << 3)
#define BIT4 (1ul << 4)
#define BIT5 (1ul << 5)
#define BIT6 (1ul << 6)
#define BIT7 (1ul << 7)
#define BIT8 (1ul << 8)
#define BIT9 (1ul << 9)
#define BIT10 (1ul << 10)
#define BIT11 (1ul << 11)
#define BIT12 (1ul << 12)
#define BIT13 (1ul << 13)
#define BIT14 (1ul << 14)
#define BIT15 (1ul << 15)
#define BIT16 (1ul << 16)
#define BIT17 (1ul << 17)
#define BIT18 (1ul << 18)
#define BIT19 (1ul << 19)
#define BIT20 (1ul << 20)
#define BIT21 (1ul << 21)
#define BIT22 (1ul << 22)
#define BIT23 (1ul << 23)
#define BIT24 (1ul << 24)
#define BIT25 (1ul << 25)
#define BIT26 (1ul << 26)
#define BIT27 (1ul << 27)
#define BIT28 (1ul << 28)
#define BIT29 (1ul << 29)
#define BIT30 (1ul << 30)
#define BIT31 (1ul << 31)


//LongToBin и __BIN - не используются напрямую
#define LongToBin(n) (((n >> 21) & 0x80) | \
                      ((n >> 18) & 0x40) | \
                      ((n >> 15) & 0x20) | \
                      ((n >> 12) & 0x10) | \
                      ((n >>  9) & 0x08) | \
                      ((n >>  6) & 0x04) | \
                      ((n >>  3) & 0x02) | \
                      ((n      ) & 0x01))

#define __BIN(n) LongToBin(0x##n##l)


//следующие макросы определяют псевдоформат бинарного представления, например BIN8(01010101) == 0x55, 
//а BIN32(00000001, 00000010, 00000011, 00000100) == 0x01020304UL
#define BIN8(n)                       __BIN(n)
#define BIN(n)                        __BIN(n)
#define BIN16(b1,b2)        ((        __BIN(b1)  <<  8UL) + \
                                      __BIN(b2))
#define BIN32(b1,b2,b3,b4) ((((U32)__BIN(b1)) << 24UL) + \
                            (((U32)__BIN(b2)) << 16UL) + \
                            (((U32)__BIN(b3)) <<  8UL) + \
                              (U32)__BIN(b4))

//эта часть компилируется, только в C++ файлах, таким образом этот файл можно подключать и в си-шных файлах
#ifdef __cplusplus

/*
prescale - альтернатива паре макросов STATIC_PRESCALER_SATRT и STATIC_PRESCALER_END
ПРИМЕР:
static UI prescale_counter = 0;
if(prescale(prescale_counter, 5))
{
	//код будет выполняться каждый пятый раз прохода этой функции
}
else//необязательно
{
	//остальные разы будет выполняться этот код
}
*/
template< class T >
inline bool prescale( T& val, T max_val )
{
	if (++val >= max_val) 
	{
		val = 0;
		return true;
	}
	return false;
}

//ограничение числа сверху
template< class T >
inline void restrict_max( T& val, T max_val )
{
  if (val > max_val) 
	  val = max_val;
}

//ограничение числа снизу
template< typename T >
inline void restrict_min( T& val, T min_val )
{
  if (val < min_val) 
	  val = min_val;
}

//ограничение числа и снизу и сверху
template< typename T >
inline void restrict_range( T& val, T min_val,  T max_val )
{
  if (val < min_val) 
	  val = min_val;
  if (val > max_val) 
	  val = max_val;
}

//проверка числа на принадлежность диапазону [min_val, max_val]
template< typename T >
inline bool in_range( T val, T min_val,  T max_val )
{
	return ((val >= min_val) && (val <= max_val) );
}

//проверка числа на эквивалентность одному из двух
template< typename T >
inline bool equal_1_or_2( T val, T match_val1,  T match_val2 )
{
  return ((val == match_val1) || (val == match_val2));
}

//проверка числа на эквивалентность одному из трех
template< typename T >
inline bool equal_1_or_2_or_3( T val, T match_val1,  T match_val2, T match_val3 )
{
  return ((val == match_val1) || (val == match_val2) || (val == match_val3));
}

//Вычисление минимума 2 чисел
template< typename T >
inline T min2( T val1,  T val2 )
{
  return (val1 < val2)? val1 : val2;
}

//Вычисление максимума 2 чисел
template< typename T >
inline T max2( T val1,  T val2 )
{
  return (val1 > val2)? val1 : val2;
}

//вычисление максимума 3 чисел
template< typename T >
inline T max3( T val1,  T val2, T val3 )
{
  return max2(val1,max2(val2,val3));
}

//установка или очистка битов bit_mask в *val в зависимости от результата логического выражения check_val
template< typename T >
inline void set_bitmask_if( T * val, T bit_mask, bool check_val )
{
  if (check_val)
  {
	  *val |= bit_mask;
  }
  else
  {
	  *val &= ~bit_mask;
  }
}

//краткое название для set_bitmask_if
#define sbi	set_bitmask_if

#endif


//кандидаты на выбытие из библиотеки 
//НЕ ИСПОЛЬЗОВАТЬ В НОВЫХ РАЗРАБОТКАХ


#define SIM2(c1,c2) (c1 << 8)|c2

#define RW_U16_LEAST_BYTE(valU16)		(*((U8*)&valU16 + 0))
#define RW_U16_MOST_BYTE(valU16)		(*((U8*)&valU16 + 1))

#define RO_U16_LEAST_BYTE(valU16) 	((valU16 >> 0) & 0xFF)
#define RO_U16_MOST_BYTE(valU16) 	((valU16 >> 8) & 0xFF)


#endif
