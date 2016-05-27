#include "can_version.h"
#include "revision_proj.h"
#include "revision_lib.h"
#include "protocol.h"


void send_version( C_CAN_CHANNELL * chan, protocol_id_t dest, protocol_id_t source )
{
	CAN_message	mes( CAN_message::generator( SET_HEADER( source, dest, 1, VERSION_TYPE ) ) );
	std::tm data = NSSubWCRevProject::TimeNow;
	mes.D32[0] = std::mktime( &data );
	mes.D16[2] = (NSSubWCRevLib::Modified_flag << 1) | NSSubWCRevProject::Modified_flag;
#ifdef FOR_DEBUG
	mes.D16[2] |= 0x8000;
#endif
	mes.D8[6] = (NSSubWCRevProject::Revision - 400) & 0xFF;
	mes.D8[7] = (NSSubWCRevLib::Revision - 400) & 0xFF;
	chan->transmit( &mes );
}

