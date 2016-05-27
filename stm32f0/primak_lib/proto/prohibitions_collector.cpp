#include "prohibitions_collector.h"
#include <map>



static PrColl::send_event_prohibition_t prohibition_state_changed_callback;

void PrColl::Init(send_event_prohibition_t callback)
{
	prohibition_state_changed_callback = callback;
}

namespace
{
	struct ProhibitionInfo
	{
		ProhibitionInfo()
		{
			code.raw = 0;
		}
		void setState(bool isActive)
		{
			if(code.state != isActive)
			{		
				code.state = isActive;
				prohibition_state_changed_callback(code);
			}
		}
		UI timeoutCounter;
		PrColl::transmit_object code;
	};

	struct key_t
	{
		friend bool operator <(const key_t &, const key_t &);
		U32 message;
		systems_t address;
	};
	bool operator <( const key_t & k1, const key_t & k2)
	{
		return ((k1.address < k2.address) || ((k1.address == k2.address) && (k1.message < k2.message)) );
	}
	static std::map<key_t, ProhibitionInfo> prohibition_map;
}

void PrColl::AddProhibition( systems_t address, U32 message, U8 prior, UI timeout )
{
	key_t key;
	key.address = address;
	key.message = message;
	
	ProhibitionInfo & prohibition_info = prohibition_map[key];
	if(( prohibition_info.code.state ) && ( prohibition_info.code.prior != prior ))
		prohibition_info.setState(false);
	
	prohibition_info.timeoutCounter = timeout;
	prohibition_info.code.address = address;
	prohibition_info.code.message = message;
	prohibition_info.code.prior = prior;
	prohibition_info.setState(true);
}

void PrColl::EraseProhibition( systems_t address, U32 message )
{
	key_t key;
	key.address = address;
	key.message = message;
	ProhibitionInfo & prohibition_info = prohibition_map[key];
	prohibition_info.timeoutCounter = 1;
}

void PrColl::EraseProhibition( systems_t address )
{
	for( std::map<key_t, ProhibitionInfo>::iterator it( prohibition_map.begin() ); it != prohibition_map.end(); ++it )
		if( it->first.address == address )
			it->second.timeoutCounter = 1;
}

void PrColl::Handler_100ms()
{
	for( std::map<key_t, ProhibitionInfo>::iterator it( prohibition_map.begin() ); it != prohibition_map.end(); ++it )
	{
		ProhibitionInfo & prohibition_info = it->second;
		if( prohibition_info.code.state )
		{
			if( prohibition_info.timeoutCounter )
			{
				prohibition_info.timeoutCounter--;
			}
			else
			{
				prohibition_info.setState(false);
			}
		}
	}
}

