#ifndef _CONN_CHECK_H__
#define _CONN_CHECK_H__

#include "base_proto_def.h"

//для проверки связи
namespace ProtoConnCheck
{
	void update_counters(void);
	bool is_good_conn(protocol_id_t proto_id);
	void reset_counter(protocol_id_t proto_id);
}


#endif


