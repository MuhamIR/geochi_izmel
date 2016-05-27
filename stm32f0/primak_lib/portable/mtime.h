#ifndef _MY_TIME_H_
#define _MY_TIME_H_
#include "TypeDef.h"
#include <ctime>
#include <string>

namespace time
{
//функции для вывода стандартной структуры std::tm в виде "ДД.ММ.ГГ ММ:СС"
char * time_to_string(char * buf, const std::tm& time);

//получение времени комрилиции проекта
void get_compilation_time(std::tm * time);//передает по указателю время компиляции в виде структуры
std::time_t get_compilation_time(void);//время компиляции в секундах
	
//функции конвертации стандартной структуры для представления времени в UNIX-время(количество секунд после какого-то года)
//typedef unsigned int time_t;     /* date/time in unix secs past 1-Jan-70
void tm_from_raw_time(std::tm * time, std::time_t timer_val);//время из time_t(U32) в структуру tm
inline std::time_t make_time(std::tm * a_tm){return std::mktime(a_tm);}

}

#endif

