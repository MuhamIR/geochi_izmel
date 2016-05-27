#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_
#include "CAN_interface.h"
#include "base_proto_def.h"
#include "System_config.h"


void send_table( C_CAN_CHANNELL * chan, proto_types type, protocol_id_t dest, const void * array, int size, protocol_id_t source = OWN_PROTOCOL_ID );


#define ID_SIZE	5
#define SOURCE_BIT_SIZE				ID_SIZE
#define SOURCE_OFFSET  				24
#define DEST_BIT_SIZE	    		ID_SIZE
#define DEST_OFFSET  				19
#define PACKET_NUM_BIT_SIZE	    	14
#define PACKET_NUM_OFFSET  			0
#define ELEM_TYPE_BIT_SIZE	    	5
#define ELEM_TYPE_OFFSET  			14

#define PACKET_NUM_BIT_SIZE_OLD	    	6
#define PACKET_NUM_OFFSET_OLD  			0
#define NUM_OF_ELEM_BIT_SIZE_OLD	    8
#define NUM_OF_ELEM_OFFSET_OLD  		6

#define SET_BITS( val, bit_num, bit_offset)	(( (ULONG)val & ( (1 << bit_num) - 1)) << bit_offset)
#define SET_HEADER(source,destination,packet_offset, type) (SET_BITS ( source, SOURCE_BIT_SIZE, SOURCE_OFFSET) | SET_BITS ( destination, DEST_BIT_SIZE, DEST_OFFSET) | SET_BITS ( packet_offset, PACKET_NUM_BIT_SIZE, PACKET_NUM_OFFSET)| SET_BITS ( type, ELEM_TYPE_BIT_SIZE, ELEM_TYPE_OFFSET))
#define SET_HEADER_OLD(source,destination,packet_offset,num_of_el, type) (SET_BITS ( source, SOURCE_BIT_SIZE, SOURCE_OFFSET) | SET_BITS ( destination, DEST_BIT_SIZE, DEST_OFFSET) | SET_BITS ( packet_offset, PACKET_NUM_BIT_SIZE_OLD, PACKET_NUM_OFFSET_OLD)| SET_BITS ( num_of_el, NUM_OF_ELEM_BIT_SIZE_OLD, NUM_OF_ELEM_OFFSET_OLD)| SET_BITS ( type, ELEM_TYPE_BIT_SIZE, ELEM_TYPE_OFFSET))


#endif
