#include "protocol.h"
#include "can_proto_table.h"
#include "memory_operations.h"
#include "debug_print_interface.h"
#include <vector>

using namespace CAN_DEF;

void send_table( C_CAN_CHANNELL * chan, proto_types type, protocol_id_t dest, const void * array, int size, protocol_id_t source)
{
	ProtoTable::ID_t id(0);
	id.parsed.src = source;
	id.parsed.dst = dest;
	id.parsed.type = type;
	id.parsed.offset = 1;
	CAN_message	mes( CAN_message::generator(id.raw) );

	U32 * p32 = (U32 *)array;
	while (size >=4)
	{
		mes.D32[0] = *p32++;
		mes.D32[1] = *p32++;
		chan->transmit( &mes );
		++id.parsed.offset;
		mes.ID = id.raw;
		size -= 4;
	}

	if (size)
	{
		U16 * p16 = (U16 *)p32;
		mes.D32[0] = 0;
		mes.D32[1] = 0;
		
		UI ind = 0;
		while (size--)
		{
			mes.D16[ind++] = *p16++;
		}
		chan->transmit( &mes );
	}
}
