#ifndef _CAN_VERSION_H_
#define _CAN_VERSION_H_

#include "base_proto_def.h"
#include "CAN_interface.h"
#include "system_config.h"

void send_version( C_CAN_CHANNELL * chan, protocol_id_t dest = BROADCAST, protocol_id_t source = OWN_PROTOCOL_ID );

#endif
