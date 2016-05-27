#ifndef _SCREEN_H__
#define _SCREEN_H__

#include "graphic_manager.h"
#include "system_config.h"

#ifdef USE_KEYPAD
#include "keypad.h"
#endif

class CScreen : public CGraphicObj
#ifdef USE_KEYPAD
, public Keypad
#endif
{
public:
	static void enter(eScreen key);
	CScreen(eScreen  key);
	virtual ~CScreen();	
	static void change_screen_to_next(void);
	static bool is_current_screen(CScreen * screen){return _current_screen == screen;}
	static eScreen get_current_screen(void){return _current_id;}
	eScreen get_id(void)const{return _id;};
protected:
	static void enter(CScreen * screen);
	void enter(void);
	virtual void redraw( bool force_full_redraw){after_redraw();}
	virtual void clear( void ){after_clear();}
	virtual void open(void) = 0;
	virtual void close(void) = 0;	
	static CScreen * _current_screen;
	static eScreen _current_id;
	static tpmas_t<eScreen, CScreen *, eNumOfScreens > _array;
	eScreen _id;
};






#endif

