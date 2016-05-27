#ifndef _LED_ALARM_
#define _LED_ALARM_

#include "TypeDef.h"
#include <vector>
#include <utility>

class CBlinkAlarm
{
	typedef std::pair< U8, U8 > pair_t;
	typedef std::vector< pair_t > vector_t;
	typedef vector_t::iterator iter_t;
	public:
		CBlinkAlarm( U8 maxSize );
		void process( void );
		void addAlarm( U8 firstCode, U8 secondCode );
		void deleteAlarm( U8 firstCode, U8 secondCode );
	virtual void set_state( bool ) = 0;
	protected:
		enum {
			LED_ALARM_NORM = 0,
			LED_ALARM_FIRST_CODE = 1,
			LED_ALARM_SPACING_FIRST = 2,
			LED_ALARM_SECOND_CODE = 3,
			LED_ALARM_SPACING_SECOND = 4
		} _state;
		U8 _maxSize;
		vector_t _buf;
		UI _currentInd;
		bool _blinkTrivial( U8 timeON, U8 timeOFF );
		bool _blinkChar( U8 timeON, U8 timeOFF, U8 ch );
};

#endif
