#ifndef _MY_TIME_H_
#define _MY_TIME_H_
#include "TypeDef.h"
#include <ctime>
#include <string>

namespace time
{
//������� ��� ������ ����������� ��������� std::tm � ���� "��.��.�� ��:��"
char * time_to_string(char * buf, const std::tm& time);

//��������� ������� ���������� �������
void get_compilation_time(std::tm * time);//�������� �� ��������� ����� ���������� � ���� ���������
std::time_t get_compilation_time(void);//����� ���������� � ��������
	
//������� ����������� ����������� ��������� ��� ������������� ������� � UNIX-�����(���������� ������ ����� ������-�� ����)
//typedef unsigned int time_t;     /* date/time in unix secs past 1-Jan-70
void tm_from_raw_time(std::tm * time, std::time_t timer_val);//����� �� time_t(U32) � ��������� tm
inline std::time_t make_time(std::tm * a_tm){return std::mktime(a_tm);}

}

#endif

