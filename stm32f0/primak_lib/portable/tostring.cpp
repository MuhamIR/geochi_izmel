#include "tostring.h"
#include "macros.h"
#include "debug_print_interface.h"
#include <vector>

//почему snprintf зависало - не буду его использовать

void libIntToStr ( U32 data, UI length, char* str, char no_pos_symbol = '0' )
{
	UI i = 0;
	while ( i < length )
	{
		U8 digit = data % 10; 
		digit += '0';
		data /= 10;
		if ( digit == '0' )
		{
			if ( ( i == 0 ) ||  ( data != 0 ) ) 
			{
				str[ length - i - 1 ] = digit;
			}
			else 
			{
				str[ length - i - 1 ] = no_pos_symbol;
			}
		}
		else
		{
			str[ length - i - 1 ] = digit;
		}
		++i;
	};
}

void libIntToHexStr ( U32 data, UI length, char* str, char no_pos_symbol = '0', bool big_letters = true )
{
	UI i = 0;
	while ( i < length )
	{
		U8 digit = data & 0xF; 
		if (digit <= 9)
		{
			digit += '0';
		}
		else
		{
			digit = digit - 10 + (big_letters ? 'A' : 'a');
		}
		data = data>>4;
		if ( digit == '0' )
		{
			if ( ( i == 0 ) ||  ( data != 0 ) ) 
			{
				str[ length - i - 1 ] = digit;
			}
			else 
			{
				str[ length - i - 1 ] = no_pos_symbol;
			}
		}
		else
		{
			str[ length - i - 1 ] = digit;
		}
		++i;
	};
}


static UI count_num_of_pos_in_decimal ( UI data )
{
	UI i = 0;
	do
	{
		++i;
		data = data/10;
	}
	while ( data );
	return i;
}

// std::string ToString::uint_to_fstring(UI val, UI num_of_pos, char no_pos_symbol)
// {
// 	if(num_of_pos > 15)
// 	{
// 		num_of_pos = 15;
// 	}
// 	char buf[16];
// 	libIntToStr ( val, num_of_pos, buf, no_pos_symbol );
// 	buf[num_of_pos] = 0;
// 	return buf;
// }

// std::string ToString::int_to_fstring(int val, UI num_of_pos)
// {
// 	std::string res;
// 	if (val < 0)
// 	{
// 		val = -val;
// 		res += '-';
// 	}
// 	else
// 	{
// 		res += ' ';
// 	}
// 	res += uint_to_fstring(val, num_of_pos, '0');
// 	return res;
// }

// std::string ToString::int_to_string(int val)
// {
// 	char buf[16];
// 	std::string res = "";
// 	if (val < 0)
// 	{
// 		val = -val;
// 		res += '-';
// 	}
// 	UI len = count_num_of_pos_in_decimal(val);
// 	libIntToStr ( val, len, buf );
// 	buf[len] = 0;
// 	res += buf;
// 	return res;
// }



// std::string ToString::rational_to_string(int val, UI dot_pos)
// {
// 	if (dot_pos == 0)
// 	{
// 		return int_to_string(val);
// 	}
// 	const int dividers[] = {1, 10, 100, 1000, 10000, 100000};
// 	restrict_max<UI>(dot_pos,ELEMENTS(dividers));
// 	std::string res;
// 	if (val < 0)
// 	{
// 		val = -val;
// 		res += '-';
// 	}
// 	res += int_to_string(val/dividers[dot_pos]);
// 	
// 	val = val%dividers[dot_pos];
// 	res += '.' + uint_to_fstring(val, dot_pos, '0');
// 	return res;
// }

// std::string ToString::u_rational_to_fstring(UI val, UI dot_pos, UI all_num_pos)
// {
// 	ASSERT( all_num_pos > dot_pos);
// 	if (dot_pos == 0)
// 	{
// 		return uint_to_fstring(val, all_num_pos, '0');
// 	}
// 	const int dividers[] = {1, 10, 100, 1000, 10000, 100000};
// 	restrict_max<UI>(dot_pos,ELEMENTS(dividers));
// 	std::string res = uint_to_fstring(val/dividers[dot_pos], all_num_pos - dot_pos, '0' );
// 	
// 	val = val%dividers[dot_pos];
// 	res += '.' + uint_to_fstring(val, dot_pos, '0');
// 	return res;
// }


// std::string ToString::uint_to_hex_fstring(UI val, UI num_of_pos, char no_pos_symbol, bool big_letters)
// {
// 	if(num_of_pos > 15)
// 	{
// 		num_of_pos = 15;
// 	}
// 	char buf[16];
// 	libIntToHexStr ( val, num_of_pos, buf, no_pos_symbol, big_letters );
// 	buf[num_of_pos] = 0;
// 	return buf;
// }



//---------------------------------------------------------------------


char * ToString::uint_to_fstring(char * buf, UI val, UI num_of_pos, char no_pos_symbol)
{
	if(num_of_pos > 15)
	{
		num_of_pos = 15;
	}
	libIntToStr ( val, num_of_pos, buf, no_pos_symbol );
	buf[num_of_pos] = 0;
	return &buf[num_of_pos];
}

char *  ToString::int_to_fstring(char * buf, int val, UI num_of_pos)
{
	if (val < 0)
	{
		val = -val;
		buf[0] = '-';
	}
	else
	{
		buf[0] = ' ';
	}
	return uint_to_fstring(&buf[1], val, num_of_pos, '0');
}

char *  ToString::int_to_string(char * buf, int val)
{
	if (val < 0)
	{
		val = -val;
		*buf++ = '-';
	}
	UI len = count_num_of_pos_in_decimal(val);
	libIntToStr ( val, len, buf );
	buf[len] = 0;
	return &buf[len];
}



char *  ToString::rational_to_string(char * buf, int val, UI dot_pos)
{
	if (dot_pos == 0)
	{
		return int_to_string(buf,val);
	}
	const int dividers[] = {1, 10, 100, 1000, 10000, 100000};
	restrict_max<UI>(dot_pos,ELEMENTS(dividers));
	if (val < 0)
	{
		val = -val;
		*buf++ = '-';
	}
	buf = int_to_string(buf,val/dividers[dot_pos]);
	
	val = val%dividers[dot_pos];
	*buf++ = '.';
	return uint_to_fstring(buf,val, dot_pos, '0');
}

char *  ToString::u_rational_to_fstring(char * buf, UI val, UI dot_pos, UI all_num_pos)
{
	ASSERT( all_num_pos >= dot_pos);
	if (dot_pos == 0)
	{
		return uint_to_fstring(buf, val, all_num_pos, '0');
	}
	const int dividers[] = {1, 10, 100, 1000, 10000, 100000};
	restrict_max<UI>(dot_pos,ELEMENTS(dividers));
	buf = uint_to_fstring(buf, val/dividers[dot_pos], all_num_pos - dot_pos, '0' );
	*buf++ = '.';
	val = val%dividers[dot_pos];
	return uint_to_fstring(buf, val, dot_pos, '0');
}

char * ToString::u_rational_to_string(char * buf, UI val, UI dot_pos)
{
	if (dot_pos == 0)
	{
		return int_to_string(buf, val);
	}
	const int dividers[] = {1, 10, 100, 1000, 10000, 100000};
	restrict_max<UI>(dot_pos,ELEMENTS(dividers));
	buf = int_to_string(buf, val/dividers[dot_pos] );
	*buf++ = '.';
	val = val%dividers[dot_pos];	
	return uint_to_fstring(buf, val, dot_pos, '0');
}

char *  ToString::rational_to_fstring(char * buf, int val, UI dot_pos, UI all_num_pos)
{
	if( val < 0 )
	{
		*buf++ = '-';
		val = -val;
	}
	else
		*buf++ = ' ';
	return u_rational_to_fstring( buf, val, dot_pos, all_num_pos );
}

char *  ToString::uint_to_hex_fstring(char * buf, UI val, UI num_of_pos, char no_pos_symbol, bool big_letters)
{
	if(num_of_pos > 15)
	{
		num_of_pos = 15;
	}
	libIntToHexStr ( val, num_of_pos, buf, no_pos_symbol, big_letters );
	buf[num_of_pos] = 0;
	return &buf[num_of_pos];
}
	
char * ToString::add(char * buf, const char * s)
{
	while(*s)
	{
		*buf++ = *s++;
	}
	*buf = 0;
	return buf;
}

char * ToString::join2(char * buf, const char * s1, const char * s2)
{
	char *p = buf;
	p = add(p, s1);
	p = add(p, s2);
	return p;
}
char * ToString::join3(char * buf, const char * s1, const char * s2, const char * s3)
{
	char *p = buf;
	p = add(p, s1);
	p = add(p, s2);
	p = add(p, s3);
	return p;
}

char * ToString::find_end(char * s)
{
	while(*s++)
	;
	return s - 1;
}

int FromString::string_to_int(const char * buf)
{
	int temp(0);
	bool minus(false);
	while( *buf != 0 )
	{
		temp *= 10;
		if(*buf == '-')
			minus = true;
		else
			temp += *buf - '0';
		++buf;
	}
	return minus ? -temp: temp;
}
