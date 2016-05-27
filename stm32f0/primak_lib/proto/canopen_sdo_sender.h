#ifndef _CANOPEN_SDO_SENDER_H_
#define _CANOPEN_SDO_SENDER_H_

#include "base_proto_def.h"
#include "data.h"
#include <deque>


class SDOQueueSender
{
public:
	
	struct Item
	{	
			U16 index;
			U8 sub_index;
			UI size;
			U8 * data;
	};
	

	typedef void (* sdo_transfer_callback_t)(const Item & item, bool succesfull);
	typedef void (* overflow_callback_t)();
	void Init(CO_Data* d, node_id_t receiver, sdo_transfer_callback_t sdo_callback = NULL, overflow_callback_t overflow_callback = NULL);
	bool Transmit(U16 index, U8 sub_index, UI size, const void * data);
	void Handler();

private:		
	enum send_states_t { SEND_REQUEST, WAIT_SDO_COMPLETE};
	UI _deque_size;
	CO_Data* _dictionary;
	sdo_transfer_callback_t _sdo_callback;
	overflow_callback_t _overflow_callback;
	node_id_t _receiver;
	U32 _abortCode;
	std::deque<Item> _send_container;
	send_states_t _send_state;

	bool start_transmit_errors(const Item & item);
	U8 get_result();
	static const UI deque_max_size = 500;
};

#endif
