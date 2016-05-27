#ifndef _ERRORS_COLLECTOR_H_
#define _ERRORS_COLLECTOR_H_

#pragma anon_unions
#include "base_proto_def.h"
#include <vector>

namespace ErrColl
{
	union transmit_object
	{
		__packed struct 
		{
			U32 fmi 		:5;
			U32 spn 		:19;
			U32 address :7;
			U32 state		:1;
		};
		U32 raw;
		systems_t Address()const {return (systems_t)address;}
		fmi_t Fmi()const {return (fmi_t)fmi;}
	};
	typedef void (*send_event_error_t)( const transmit_object & data);
	void Init(send_event_error_t callback);
	
	void AddError( systems_t address, U32 spn, fmi_t fmi, UI timeout = 0 );
	inline void AddErrorWTimeout( systems_t address, U32 spn, fmi_t fmi, UI timeout = 5){ AddError( address, spn, fmi, timeout ); }
	void EraseError( systems_t address, U32 spn );
	void EraseError( systems_t address, U32 spn, fmi_t fmi );
	bool IsErrorActive( systems_t address, U32 spn );
	bool IsErrorActive( systems_t address, U32 spn, fmi_t fmi );
	bool GetErrorFmi( systems_t address, U32 spn, fmi_t* fmi );
	void SetErrorState( systems_t address, U32 spn, fmi_t fmi, bool state );
	
	void Handler_100ms();
	void GetErrorList( std::vector<U32> & error_list);
};

#endif
