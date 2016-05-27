#include "can_proto_table.h"
#include "debug_print_interface.h"
#include <map>

static std::map<UI, ProtoTable::table_info> can_map;

ProtoTable::table_info::table_info(void * a_data, UI a_size):
data(a_data),size(a_size)
{
	ASSERT_WITH_STOP( !(size % 8) );
	size = size/8;
}

void ProtoTable::table_info::put(const CAN_proto_mes * mes)
{
	UI offset = mes->id.parsed.offset - 1;
	if(offset < size)
	{
		U32 * p = (U32 *)data + offset*2;
		p[0] = mes->D32[0];
		p[1] = mes->D32[1];
	}
}

void ProtoTable::add_table( UI key, table_info & table)
{
	can_map[key] = table;
}

void ProtoTable::proto_can_handler( C_CAN_CHANNELL * can_ch, special_handler_t special_handler)
{
	while(can_ch->receive_data_ready())
	{
		CAN_message rec_mes;
		can_ch->receive(&rec_mes);
		CAN_proto_mes * parsed = (CAN_proto_mes *)&rec_mes.ID;
#ifdef USE_CONN_CHECK_WITH_PROTO_TABLE
		ProtoConnCheck::reset_counter((protocol_id_t)parsed->id.parsed.src);
#endif	
		std::map<UI, table_info>::iterator it = can_map.find(get_can_key( (protocol_id_t)parsed->id.parsed.dst, 
															(protocol_id_t)parsed->id.parsed.src, (proto_types)parsed->id.parsed.type));
		if (it != can_map.end())
		{
			it->second.put(parsed);
		}
		else if (special_handler != NULL)
		{
			special_handler(*parsed, can_ch);
		}
	}
}


void ProtoTable::send_table(C_CAN_CHANNELL & can_ch, protocol_id_t dst, proto_types type, const void * data, UI size_in_bytes, protocol_id_t src )
{
	DEBUG_ASSERT( (size_in_bytes%8 == 0) && data);
	ID_t id(0);
	id.parsed.dst = dst;
	id.parsed.src = src;
	id.parsed.type = type;
	id.parsed.offset = 1;
	UI size = size_in_bytes/8;	
	CAN_message	 mes = CAN_message::generator(id.raw);
	U32 * p = (U32*)data;
	while(size--)
	{
		mes.D32[0] = *p++;
		mes.D32[1] = *p++;
		can_ch.transmit( mes);
		++id.parsed.offset;
		mes.ID = id.raw;
	}
}



