#include "canopen_sdo_sender.h"
#include "system_config.h"
#include "macros.h"
#include "base_proto_def.h"
#include "canopenport.h"
#include "debug_print_interface.h"				






void SDOQueueSender::Init(CO_Data* d, node_id_t receiver, sdo_transfer_callback_t sdo_callback, overflow_callback_t overflow_callback)
{
	_dictionary = d;
	_sdo_callback = sdo_callback;
	_overflow_callback = overflow_callback;
	_deque_size = 0;
	_receiver = receiver;
	_send_state = SEND_REQUEST;
}

bool SDOQueueSender::Transmit(U16 index, U8 sub_index, UI size, const void * data)
{
	if(_deque_size < deque_max_size)
	{
		_deque_size++;
		Item item;
		item.index = index;
		item.sub_index = sub_index;
		item.size = size;
		item.data = new U8[size];
		U8* p  = (U8*)data;
		ff(i, size)
		{
			item.data[i] = p[i];
		}
		_send_container.push_back(item);
		return true;
	}
	else if(_overflow_callback)
	{
		_overflow_callback();
	}
	return false;
}


bool SDOQueueSender::start_transmit_errors(const Item & item)
{
	if (CanOpen::ConnectionIsGood(_receiver) == false)
	{
		return false;
	}
	
	return 0 == writeNetworkDict( _dictionary, _receiver, item.index, item.sub_index, item.size, 0, item.data, 0 );;
}



U8 SDOQueueSender::get_result()
{
	return getWriteResultNetworkDict( _dictionary, _receiver, (UNS32*)&_abortCode);
}

void SDOQueueSender::Handler()
{
	switch(_send_state)
	{
		case SEND_REQUEST:
			if(_deque_size != 0)
			{
				if(start_transmit_errors(_send_container.front()))
				{
					_send_state = WAIT_SDO_COMPLETE;
				}
			}
			break;
				
		case WAIT_SDO_COMPLETE:
			{
				U8 res = get_result();
				if(res != SDO_DOWNLOAD_IN_PROGRESS)
				{
					if ( res == SDO_FINISHED)
					{
						Item & front = _send_container.front();
						if (_sdo_callback)
						{
							_sdo_callback(front, true);
						}	
						delete front.data;
						_send_container.pop_front();
						_deque_size--;
						_send_state = SEND_REQUEST;
					}
					else
					{
						if (_sdo_callback)
						{
							Item & front = _send_container.front();
							_sdo_callback(front, false);
						}
						_send_state = SEND_REQUEST;//продолжаем пока не получится
					}
				}
			}
			break;
	}
}

