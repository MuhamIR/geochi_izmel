#ifndef __TO_STRING_H__
#define __TO_STRING_H__

#include "TypeDef.h"

namespace ToString
{
	//����� ������ ��������
// 	std::string int_to_string(int val);
// 	std::string rational_to_string(int val, UI dot_pos);
// 	std::string u_rational_to_fstring(UI val, UI dot_pos, UI all_num_pos);//� all_num_pos ����� �� �����������

// 	std::string uint_to_fstring(UI val, UI num_of_pos, char no_pos_symbol = ' ');
// 	std::string int_to_fstring(int val, UI num_of_pos);//num_of_pos �� �������� � ���� '-'
// 	std::string uint_to_hex_fstring(UI val, UI num_of_pos = 8, char no_pos_symbol = '0', bool big_letters = true);//num_of_pos - ����������� ���-�� �������� ��� �����������(�������������� 0)
	
	//������� ������� ����� ������ ������� ������ �����
	//���������� ��������� �� ��������� char ��������� ������('\0') ��� ����������������� �������������
	char * int_to_string(char * buf, int val);
	char * rational_to_string(char * buf, int val, UI dot_pos);
	char * u_rational_to_fstring(char * buf, UI val, UI dot_pos, UI all_num_pos);//� all_num_pos ����� �� �����������
	char * u_rational_to_string(char * buf, UI val, UI dot_pos);
	char * rational_to_fstring(char * buf, int val, UI dot_pos, UI all_num_pos);//� all_num_pos ����� � ����� �� �����������

	char * uint_to_fstring(char * buf, UI val, UI num_of_pos, char no_pos_symbol = ' ');
	char * int_to_fstring(char * buf, int val, UI num_of_pos);//num_of_pos �� �������� � ���� '-'
	char * uint_to_hex_fstring(char * buf, UI val, UI num_of_pos = 8, char no_pos_symbol = '0', bool big_letters = true);
	char * add(char * buf, const char * s);
	char * join2(char * buf, const char * s1, const char * s2);
	char * join3(char * buf, const char * s1, const char * s2, const char * s3);
	
	char * find_end(char * s);
}

namespace FromString
{
	int string_to_int(const char * buf);//�������� � ��� ����������� �������. ������ ����� ���������� � �����.
}


#endif

