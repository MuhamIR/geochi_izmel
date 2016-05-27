#ifndef _CAN_PROTO_TABLE_H_
#define _CAN_PROTO_TABLE_H_

#include "can_interface.h"
#include "base_proto_def.h"

//�� �������� ���������� define OWN_PROTOCOL_ID
#include "System_config.h"


#ifdef USE_CONN_CHECK_WITH_PROTO_TABLE
#include "conn_check.h"
#endif


namespace ProtoTable{

#ifdef USE_CONN_CHECK_WITH_PROTO_TABLE
using ProtoConnCheck::update_counters;
using ProtoConnCheck::is_good_conn;	
#endif	
	
	
//������������� ����� ID CAN ��������� � ���������
struct proto_id
{
	UI offset:14;
	UI type:5;
	UI dst:5;
	UI src:5;
};

//������� ��� ��� ���������� ID
union ID_t
{	
	ID_t(U32 id){raw = id;}
	proto_id parsed;
	U32 raw;
};

struct CAN_proto_mes
{
	ID_t id;
	union
	{
		U32 D32[2];
		U16 D16[4];
		U8 	D8[8];
	};
};

struct table_info
{
	table_info()//��� ����,����� ����� ���� ������������ � map
	{
		data = NULL;
		size = 0;
	}
	table_info(void * a_data, UI a_size);
	void put(const CAN_proto_mes * mes);
private:
	void * data;
	UI size;
};

//������� ��� �������� ����� �� ������ � ���� �������
inline UI get_can_key( protocol_id_t dst, protocol_id_t src, proto_types type)
{
	return ((UI)src << 24) | ((UI)dst << 19) | ((UI)type << 14);
}

//�������� ������� �������������� ���������, �������������� proto_can_handler
typedef void ( * special_handler_t) (CAN_proto_mes & mes, C_CAN_CHANNELL * can_ch);
void special_handler(CAN_proto_mes & mes, C_CAN_CHANNELL * can_ch);

//�������� ��� ������� ��� ��������� �������� ���������
void proto_can_handler( C_CAN_CHANNELL * can_ch, special_handler_t special_handler = NULL);

//������� ����������� ����� ������� ��� ��������������� ���������� ������� �� CAN
void add_table( UI key, table_info & table);
template< typename T >
void add_table( protocol_id_t dst, protocol_id_t src, proto_types type, T * data )
{
	table_info table(data, sizeof(T));
	add_table( get_can_key( dst, src, type), table);
}

//������� �������� � �������������� ������������������ offset, ������ ������������ ������ ������ ���� ������ 8
void send_table(C_CAN_CHANNELL & can_ch, protocol_id_t dst, proto_types type, const void * data, UI size_in_bytes , protocol_id_t src = OWN_PROTOCOL_ID );//size_in_bytes ������ �������� �� 8 ��� �������
template< typename T >
void send_table( C_CAN_CHANNELL & can_ch, protocol_id_t dst, proto_types type, const T * data, protocol_id_t src = OWN_PROTOCOL_ID)//sizeof(T) ������ �������� �� 8 ��� �������
{
	send_table( can_ch,  dst,  type, data, sizeof(T), src );
}


}//namespace ProtoTable

#endif

