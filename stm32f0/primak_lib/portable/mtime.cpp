#include "mtime.h"

#include <ctime>
#include <cctype>
#include <cstring>
#include <stdlib.h>
#include "tostring.h"

using namespace std;

namespace
{
const char * copy_simbols(const char * src, char * buf)
{
	while (!isalnum(*src))//пропуск пробелов и прочего
		src++;
	while (isalnum(*src))//копирование
		*buf++ = *src++;
	*buf = 0;//вставка терминального нуля
	return src;
}

int get_month(const char * name)
{
	 if(!strcmp("Jan", name))		return 0;
	else if(!strcmp("Feb", name))	return 1;
	else if(!strcmp("Mar", name))	return 2;
	else if(!strcmp("Apr", name))	return 3;
	else if(!strcmp("May", name))	return 4;
	else if(!strcmp("Jun", name))	return 5;
	else if(!strcmp("Jul", name))	return 6;
	else if(!strcmp("Aug", name))	return 7;
	else if(!strcmp("Sep", name))	return 8;
	else if(!strcmp("Oct", name))	return 9;
	else if(!strcmp("Nov", name))	return 10;
	else if(!strcmp("Dec", name))	return 11;
	else return -1;
}

}

time_t time::get_compilation_time(void)
{
	tm temp;
	get_compilation_time(&temp);
	return mktime (&temp);
}



char * time::time_to_string(char * buf, const std::tm& time)
{
	using namespace ToString;

	buf = uint_to_fstring(buf, time.tm_mday, 2, '0');
	*buf++ = '.';
	buf = uint_to_fstring(buf, time.tm_mon + 1 , 2, '0');
	*buf++ = '.';
	buf = uint_to_fstring(buf, time.tm_year - 100 , 2, '0');
	*buf++ = ' ';
	buf = uint_to_fstring(buf, time.tm_hour , 2, '0');
	*buf++ = ':';
	buf = uint_to_fstring(buf, time.tm_min , 2, '0');
	return buf;
}

void time::get_compilation_time(tm * time)
{
	char buf[32];
	
	const char * ptr1 = __DATE__;
	ptr1 = copy_simbols(ptr1, buf);
	time->tm_mon = get_month(buf);

		
	ptr1 = copy_simbols(ptr1, buf);
	time->tm_mday = atoi(buf);
	
	ptr1 = copy_simbols(ptr1, buf);
	time->tm_year = atoi(buf) - 1900;

	ptr1 = __TIME__;
	ptr1 = copy_simbols(ptr1, buf);
	time->tm_hour = atoi(buf);
	
	ptr1 = copy_simbols(ptr1, buf);
	time->tm_min = atoi(buf);
	
	ptr1 = copy_simbols(ptr1, buf);
	time->tm_sec = atoi(buf);
	
}


void time::tm_from_raw_time(tm * time, time_t timer_val)
{
	char buf[32];
	const char * ptr1 = ctime (&timer_val);
	
	ptr1 = copy_simbols(ptr1, buf);//пропустили день недели
	
	ptr1 = copy_simbols(ptr1, buf);//получили месяц
	time->tm_mon = get_month(buf);
		
	ptr1 = copy_simbols(ptr1, buf);
	time->tm_mday = atoi(buf);
	
	ptr1 = copy_simbols(ptr1, buf);
	time->tm_hour = atoi(buf);
	
	ptr1 = copy_simbols(ptr1, buf);
	time->tm_min = atoi(buf);
	
	ptr1 = copy_simbols(ptr1, buf);
	time->tm_sec = atoi(buf);
	
	ptr1 = copy_simbols(ptr1, buf);
	time->tm_year = atoi(buf) - 1900;
	
}
