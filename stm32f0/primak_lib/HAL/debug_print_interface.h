#ifndef _DEBUG_PRINTING_INTERFACE_H_
#define _DEBUG_PRINTING_INTERFACE_H_
#include "System_config.h"
#include "TypeDef.h"
#include "tostring.h"
#include <string>

#if DEBUG_PRINTING

class CDebugPrint
{
	public:	
		virtual void puts(const char * s) = 0;
		inline void puts(const std::string & s){puts(s.c_str());}
		virtual void flush(void) = 0;
		static void set_debug(CDebugPrint *debug){ _debug = debug;};
		static CDebugPrint * _debug;				
};

//следующие макросы одинаково (кроме вывода информации) работают при DEBUG_PRINTING == 0 и DEBUG_PRINTING != 0
//
#define PUTS( str)  CDebugPrint::_debug->puts(str)
#define FLUSH()		CDebugPrint::_debug->flush()
#define PRINT_PLACE()		PUTS( "\r\nFile = "__FILE__"\r\n" )
#define PRINT_PLACE_AND_STOP()		{PUTS( "\r\nSTOP in file = "__FILE__"\r\n" ); while(1);}
#define ASSERT_WITH_STOP(expr)	{if (!(expr)) {	PUTS( "\r\nAssert failed and program stop ( " #expr " )\r\n");while(1);}}

#else


#define PUTS( str)  
#define FLUSH()
#define PRINT_PLACE()
#define PRINT_PLACE_AND_STOP()		do{while(1);}while(0)
#define ASSERT_WITH_STOP(expr)	{if (!(expr)) while(1);}
#endif


//DEBUG_ASSERT работает по-разному
//при DEBUG_PRINTING == 0 просто пропуск для оптимизации
//при DEBUG_PRINTING != 0 выполнение программы будет остановлено
#if DEBUG_PRINTING
#define DEBUG_ASSERT(expr)	{if (!(expr)) {	PUTS( "\r\nDebug assert failed and program stop ( " #expr " )\r\n"); while(1);}}
#else
#define DEBUG_ASSERT(expr)	
#endif

#define ASSERT(expr)	DEBUG_ASSERT(expr)


#endif


