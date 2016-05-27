#include "prohibition_generator.h"
#include "protocol.h"

void CProhibitionGenerator::process_100ms( void )
{
	_vecProhibitions.clear();
	for( std::map< U32, pt >::iterator it( prohibitions.begin() ); it != prohibitions.end(); ++it )
	{
		if( it->second.timer > 0 )
		{
			it->second.timer--;
			_addVecProhibition( _system, it->first, it->second.prior );
		}
	}
	
	STATIC_PRESCALER_START(5)	
	{
		send_table( _chan, PROHIBITION_TYPE, MI_M3_PROTOCOL_ID, (U16 *)_vecProhibitions.begin(), _vecProhibitions.size()*2, OWN_PROTOCOL_ID );
	}
	STATIC_PRESCALER_END
}


union prohibition_t
{
	__packed
	struct{
		U32 address :8;
		U32 message :20;
		U32 prior   :4;
	};
	U32 raw;
};

void CProhibitionGenerator::_addVecProhibition( systems_t address, U32 message, U8 prior )
{
	prohibition_t pr;
	pr.address = address;
	pr.message = message;
	pr.prior = prior;
	_vecProhibitions.push_back( pr.raw );
}



