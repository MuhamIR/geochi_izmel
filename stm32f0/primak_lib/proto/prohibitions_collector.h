#ifndef _PROHIBITION_COLLECTOR_H_
#define _PROHIBITION_COLLECTOR_H_

#pragma anon_unions
#include "base_proto_def.h"

namespace PrColl
{
	union transmit_object
	{
		__packed
		struct{
			U32 address :7;
			U32 state		:1;
			U32 message :20;
			U32 prior   :4;
		};
		U32 raw;
	};
	typedef void (*send_event_prohibition_t)( const transmit_object & data);
	
	void Init(send_event_prohibition_t callback);
	void AddProhibition( systems_t address, U32 message, U8 prior, UI timeout );
	void EraseProhibition( systems_t address, U32 message );
	void EraseProhibition( systems_t address );
	
	void Handler_100ms();
}

#endif
