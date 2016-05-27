#ifndef _ERROR_GENERATOR_H_
#define _ERROR_GENERATOR_H_

#include "CAN_interface.h"
#include "proto_def.h"
#include <map>
#include <vector>

class CErrorGenerator
{
	public:
		CErrorGenerator( systems_t system, C_CAN_CHANNELL * chan ){ _system = system; _chan = chan; }
		void process_500ms( void );
		bool isError( const UI spn, const fmi_t fmi ) const;
		bool isError( const UI spn ) const;
		std::map< U32, fmi_t > errors;
	private:
		systems_t _system;
		C_CAN_CHANNELL * _chan;
		std::vector< U32 > _vecErrors;
		void _addVecError( U8 address, UI spn, fmi_t fmi );
};

extern CErrorGenerator * errorGenerator;

#endif
