#include "debug_print_interface.h"

#if DEBUG_PRINTING
class CEmptyDebug : public CDebugPrint 
{
	public:	
		virtual void puts(const char * s){}
		virtual void flush(void){}
};

CEmptyDebug empty_debug;
CDebugPrint * CDebugPrint::_debug = &empty_debug;
#endif


