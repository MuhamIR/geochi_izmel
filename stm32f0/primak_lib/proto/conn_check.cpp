#include "conn_check.h"
#include "System_config.h"

#ifndef CONN_REFRESH_VALUE  
#define CONN_REFRESH_VALUE 60
#endif


//static UI counters[PROTO_MAX_ID + 1];

void ProtoConnCheck::update_counters(void)
{
// 	for( UI i = 0; i <= PROTO_MAX_ID; i++)
// 	{
// 		if (counters[i])
// 			counters[i]--;
// 	}
}

bool ProtoConnCheck::is_good_conn(protocol_id_t proto_id)
{
// 	if (((proto_id <= PROTO_MAX_ID) && (counters[proto_id])) || (OWN_PROTOCOL_ID == proto_id))
// 		return true;
// 	else
// 		return false;
	return true;
}

void ProtoConnCheck::reset_counter(protocol_id_t proto_id)
{
// 	if (proto_id <= PROTO_MAX_ID)
// 	{
// 		counters[proto_id] = CONN_REFRESH_VALUE;
// 	}
}


