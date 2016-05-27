#include "errors_collector.h"
#include "system_config.h"
#include <map>


static ErrColl::send_event_error_t error_state_changed_callback;

void ErrColl::Init(send_event_error_t callback)
{
	error_state_changed_callback = callback;
}

namespace
{
	struct ErrorInfo
	{
		ErrorInfo()
		{
			code.raw = 0;
		}
		void setState(bool isActive)
		{
			if(code.state != isActive)
			{		
				code.state = isActive;
				error_state_changed_callback(code);
			}
		}
		bool isActive()const{return code.state;}
			
		bool haveTimeout;
		UI timeoutCounter;
		ErrColl::transmit_object code;
	};

	struct key_t
	{
		friend bool operator <(const key_t &, const key_t &);
		U32 spn;
		systems_t address;
	};
	bool operator <( const key_t & k1, const key_t & k2)
	{
		return ((k1.address < k2.address) || ((k1.address == k2.address) && (k1.spn < k2.spn)) );
	}
	static std::map<key_t, ErrorInfo> error_map;
}

void ErrColl::AddError( systems_t address, U32 spn, fmi_t fmi, UI timeout )
{
	key_t key;
	key.address = address;
	key.spn = spn;
	
	ErrorInfo & error_info = error_map[key];
	if(( error_info.code.state ) && ( error_info.code.fmi != fmi ))
		error_info.setState(false);
	
	if( timeout )
	{
		error_info.haveTimeout = true;
		error_info.timeoutCounter = timeout;
	}
	else
	{
		error_info.haveTimeout = false;
	}
	
	error_info.code.fmi = fmi;
	error_info.code.spn = spn;
	error_info.code.address = address;
	error_info.setState(true);
}

void ErrColl::EraseError( systems_t address, U32 spn )
{
	key_t key;
	key.address = address;
	key.spn = spn;
	ErrorInfo & error_info = error_map[key];
	error_info.haveTimeout = true;
	error_info.timeoutCounter = 0;
}

void ErrColl::EraseError( systems_t address, U32 spn, fmi_t fmi )
{
	key_t key;
	key.address = address;
	key.spn = spn;
	ErrorInfo & error_info = error_map[key];
	if( error_info.code.fmi == fmi )
	{
		error_info.haveTimeout = true;
		error_info.timeoutCounter = 0;
	}
}

bool ErrColl::IsErrorActive( systems_t address, U32 spn )
{
	key_t key;
	key.address = address;
	key.spn = spn;
	ErrorInfo & error_info = error_map[key];
	return error_info.code.state;
}

bool ErrColl::IsErrorActive( systems_t address, U32 spn, fmi_t fmi )
{
	key_t key;
	key.address = address;
	key.spn = spn;
	ErrorInfo & error_info = error_map[key];
	return ( error_info.code.state && ( error_info.code.fmi == fmi ) );
}

bool ErrColl::GetErrorFmi( systems_t address, U32 spn, fmi_t* fmi )
{
	key_t key;
	key.address = address;
	key.spn = spn;
	ErrorInfo & error_info = error_map[key];
	if( error_info.code.state )
	{
		*fmi = error_info.code.Fmi();
		return true;
	}
	return false;
}

void ErrColl::SetErrorState( systems_t address, U32 spn, fmi_t fmi, bool state )
{
	if( state )
		AddError( address, spn, fmi );
	else
		EraseError( address, spn, fmi );
}

void ErrColl::Handler_100ms()
{
	for(std::map<key_t, ErrorInfo>::iterator it( error_map.begin() ); it != error_map.end(); ++it)
	{
		ErrorInfo & error_info = it->second;
		if(error_info.isActive() && error_info.haveTimeout)
		{
			if(error_info.timeoutCounter)
			{
				error_info.timeoutCounter--;
			}
			else
			{
				error_info.setState(false);
			}
		}
	}
}

void ErrColl::GetErrorList( std::vector<U32> & error_list)
{
	error_list.clear();
	error_list.reserve(32);
	for(std::map<key_t, ErrorInfo>::iterator it( error_map.begin() ); it != error_map.end(); ++it)
	{
		ErrorInfo & error_info = it->second;
		if(error_info.isActive())
		{
			error_list.push_back(error_info.code.raw);
		}
	}
}




