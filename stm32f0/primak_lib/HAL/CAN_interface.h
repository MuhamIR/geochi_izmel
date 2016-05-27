#ifndef _CAN_INTERFACE_H_
#define _CAN_INTERFACE_H_
#include "macros.h"
#include "TypeDef.h"


namespace CAN_DEF
{
const U32 ID_29BIT	= 0x80000000UL;
const U32 RTR_BIT	= 0x40000000UL;
const U32 DLC0 = 0x00000000UL;
const U32 DLC2 = 0x00020000UL;
const U32 DLC3 = 0x00030000UL;
const U32 DLC4 = 0x00040000UL;
const U32 DLC8 = 0x00080000UL;

//Стандартное сообщение - 29 бит, 8 байт данных
const U32 FF_DLC8 = ID_29BIT | DLC8;
}//namespace CAN_DEF


struct CAN_message
{
	U32   TFI_RFS;    //Конфигурация отправляемого/читаемого сообщения задается в TFI_RFS      
	U32   ID;
	union
	{
		U32   DataField[2];
		U32 D32[2];
		U16 D16[4];
		U8  D8[8];
	};
	
	UI GetDataLen(void)const{UI len = (TFI_RFS >> 16) & 0xF; return (len <= 8) ? len : 8;}
	
	__pure inline static CAN_message generator(U32 id, UI data_len = 8, bool extended_frame = true)
	{
		CAN_message mes;
		mes.ID = id;
		mes.TFI_RFS = 0;
		if(extended_frame)
		{
			mes.TFI_RFS |= CAN_DEF::ID_29BIT;
		}
		if(data_len < 8)
		{
			mes.TFI_RFS |= data_len << 16;
		}
		else
		{
			mes.TFI_RFS |= 8 << 16;
		}
		return mes;
	}
};



class C_CAN_CHANNELL
{
	public:
		virtual bool transmit (const CAN_message *  pTransmitBuf) = 0;	
		virtual bool receive (CAN_message *  pTransmitBuf) = 0;
		virtual bool is_empty(void) = 0;
	
		inline bool transmit (const CAN_message &  pTransmitBuf){return transmit(&pTransmitBuf);}
		bool receive_data_ready(void){return !is_empty();}
};

typedef void (*CANCallback_t)( CAN_message * mes );

#endif

