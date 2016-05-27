#ifndef _PROHIBITION_GENERATOR_H_
#define _PROHIBITION_GENERATOR_H_

#include "CAN_interface.h"
#include "proto_def.h"
#include <map>
#include <vector>

struct pt
{
	U8 prior;
	U32 timer;
	pt( U8 inprior, U32 intimer ){ prior = inprior; timer = intimer; }
	pt(){}
};

class CProhibitionGenerator
{
	public:
		CProhibitionGenerator( systems_t system, C_CAN_CHANNELL * chan ){ _system = system; _chan = chan; }
		void process_100ms( void );
		std::map< U32, pt > prohibitions;
	private:
		systems_t _system;
		C_CAN_CHANNELL * _chan;
		std::vector< U32 > _vecProhibitions;
		void _addVecProhibition( systems_t address, U32 message, U8 prior );
};

extern CProhibitionGenerator * prohibitionGenerator;

#endif
