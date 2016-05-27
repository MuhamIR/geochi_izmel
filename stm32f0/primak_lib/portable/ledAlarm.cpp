#include "ledAlarm.h"
#include <algorithm>

CBlinkAlarm::CBlinkAlarm( U8 maxSize )
{
	_state = LED_ALARM_NORM;
	_maxSize = maxSize;
	_currentInd = 0;
}

//	В последнем такте индикации функция возвращает TRUE, сигнализируя, что следующий такт можно запускать следующий примитив.
bool CBlinkAlarm::_blinkTrivial( U8 timeON, U8 timeOFF )
{
	static U8 timer = 0, _timeON = 0, _timeOFF = 0;
	
	if (( _timeON != timeON ) || ( _timeOFF != timeOFF ))
	{
		_timeON = timeON;
		_timeOFF = timeOFF;
		timer = 0;
	}
	
	if ( timer < timeON )
		this->set_state(1);
	else
		this->set_state(0);
	
	timer++;
	if ( timer >= ( timeON + timeOFF ) )
	{
		timer = 0;
		return true;
	}
	else
		return false;
}

//	В последнем такте индикации функция возвращает TRUE, сигнализируя, что следующий такт можно запускать следующий символ.
bool CBlinkAlarm::_blinkChar( U8 timeON, U8 timeOFF, U8 ch )
{
	static U8 timer = 0;
	if ( _blinkTrivial( timeON, timeOFF ) )
		timer++;
	
	if ( timer >= ch )
	{
		timer = 0;
		return true;
	}
	else
		return false;
}

void CBlinkAlarm::deleteAlarm( U8 firstCode, U8 secondCode )
{
	pair_t temp( firstCode, secondCode );
	iter_t pos = std::find( _buf.begin(), _buf.end(), temp );
	if ( pos != _buf.end() )
	{
		_buf.erase( pos );
		if( _currentInd >= _buf.size() )
			_currentInd = 0;
	}
}

void CBlinkAlarm::addAlarm( U8 firstCode, U8 secondCode )
{
	pair_t temp( firstCode, secondCode );
	iter_t pos = std::find( _buf.begin(), _buf.end(), temp );
	if (( pos == _buf.end() ) && ( _buf.size() < _maxSize ))
	{
		_buf.push_back( temp );
	}
}

void CBlinkAlarm::process( void )
{
	switch ( _state )
	{
		case LED_ALARM_NORM:
			if ( _blinkTrivial( 4, 4 ) )
			{
				if ( !_buf.empty() )
				{
					_currentInd = _buf.size();
					_state = LED_ALARM_SPACING_SECOND;
				}
			}
		break;
		
		case LED_ALARM_FIRST_CODE:
			if ( _blinkChar( 9, 4, _buf[_currentInd].first ) )
				_state = LED_ALARM_SPACING_FIRST;
		break;
			
		case LED_ALARM_SPACING_FIRST:
			if ( _blinkTrivial( 0, 5 ) )
				_state = LED_ALARM_SECOND_CODE;
		break;
		
		case LED_ALARM_SECOND_CODE:
			if ( _blinkChar( 2, 2, _buf[_currentInd].second ) )
				_state = LED_ALARM_SPACING_SECOND;
		break;
		
		case LED_ALARM_SPACING_SECOND:
			if ( _blinkTrivial( 0, 14 ) )
			{
				if ( _buf.empty() )
					_state = LED_ALARM_NORM;
				else
				{
					if ( ++_currentInd >= _buf.size() )
						_currentInd = 0;
					_state = LED_ALARM_FIRST_CODE;
				}
			}
		break;
	}
}
