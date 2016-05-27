#ifndef _PACKET_HANDLER_H_
#define _PACKET_HANDLER_H_
#include "UART_driver.h"


template<class DATA_T=unsigned char, UI SIZE = 32>
struct 	CPacket
{ 
	public:	 
		CPacket(){_size = 0;};
		DATA_T _data[SIZE];	//array to save received simbols
		UI  _size;	 //не размер, а количество записанных элементов
		DATA_T operator[](UI ind){return _data[ind];};
};

#define UART_MAX_PACKET_SIZE 256
typedef CPacket<U8, UART_MAX_PACKET_SIZE> UART_packet;

struct 	C_UART_ReceivePacket : public UART_packet
{ 
	public:	 
	    C_UART_ReceivePacket();
  	protected:
		UI  add(U8 data);
		UI	_flag; //State of packet
		UI  _esc_flag;	// esc simbol was received 
};

typedef void (* pkt_handler_t )(UART_packet * pkt);

class packet_handler 
{ 
	public:
		packet_handler(pkt_handler_t handler){_handler = handler;};
		void handler(void){}; 
	protected:
		pkt_handler_t _handler;
};

class UART_handler : protected packet_handler , protected C_UART_ReceivePacket
{ 
	public:
		UART_handler(C_UART * uart,pkt_handler_t handler):
			packet_handler(handler),C_UART_ReceivePacket(){_uart = uart;};
		void handler(void); 
	protected:
		C_UART * _uart;
};


#endif
