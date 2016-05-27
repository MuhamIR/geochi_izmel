#ifndef _CAN_PROTO_TIME_H_
#define _CAN_PROTO_TIME_H_


#include "mtime.h"
#include "System_config.h"
#include "can_proto_table.h"

inline void proto_send_version(C_CAN_CHANNELL * chan)
{
	static std::time_t comp_time = 0;
	if (!comp_time)//если не расчитывалось ранее
	{
		comp_time = time::get_compilation_time();
	}
	ProtoTable::ID_t id(0);
	id.parsed.src = OWN_PROTOCOL_ID;
	id.parsed.dst = BROADCAST;
	id.parsed.type = VERSION_TYPE;
	id.parsed.offset = 1;
	CAN_message temp = CAN_message::generator(id.raw);
	temp.D32[0] = comp_time;
	temp.D32[1] = (SOFTWARE_VERSION << 24) | (SOFTWARE_SUBVERSION << 16) | (HASH_CODE << 0);
	chan->transmit(&temp);
}


#endif

