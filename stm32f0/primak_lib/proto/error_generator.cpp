#include "error_generator.h"
#include "protocol.h"

void CErrorGenerator::process_500ms( void )
{
	_vecErrors.clear();
 	for( std::map< U32, fmi_t >::const_iterator ci( errors.begin() ); ci != errors.end(); ++ci )
		_addVecError( _system, ci->first, ci->second );
 	
	send_table( _chan, ERROR_TYPE, BROADCAST, (U16 *)_vecErrors.begin(), _vecErrors.size()*2, OWN_PROTOCOL_ID );
}

bool CErrorGenerator::isError( UI spn, fmi_t fmi ) const
{
	std::map< U32, fmi_t >::const_iterator ci( errors.find( spn ) );
	if( ci == errors.end() )
		return false;
	else if( ci->second != fmi )
		return false;
	return true;
}

bool CErrorGenerator::isError( UI spn ) const
{
	std::map< U32, fmi_t >::const_iterator ci( errors.find( spn ) );
	if( ci == errors.end() )
		return false;
	else 
		return true;
}

union error_t
{
	__packed
	struct {
		U32 fmi     :5;
		U32 spn     :19;
		U32 address :8;
	};
	U32 raw;
};

void CErrorGenerator::_addVecError( U8 address, UI spn, fmi_t fmi )
{
	error_t er;
	er.fmi = fmi;
	er.spn = spn;
	er.address = address;
	_vecErrors.push_back( er.raw );
}
