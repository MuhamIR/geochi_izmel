#include "screen.h"
#include "debug_print_interface.h"


CScreen * CScreen::_current_screen;
tpmas_t<eScreen, CScreen *, eNumOfScreens > CScreen::_array;
eScreen CScreen::_current_id = eMain1;

CScreen::CScreen(eScreen key)
{
	ASSERT (key < _array.size());
	ASSERT(_array[key] == NULL);
	_array[key] = this;
	_id = key;
}

CScreen::~CScreen()
{
	_array[_id] = NULL;
}

void CScreen::change_screen_to_next(void)
{
	eScreen id = _current_id;		
	while(1)
	{
		id = (eScreen)(id + 1 );
		if (id == eNumOfScreens)
		{
			id = eMain1;
		}
		
		if (_array[id] != NULL)
		{
			enter(id);
			return;
		}
	}
}

void CScreen::enter(void)
{
	CScreen::enter(this);
}

void CScreen::enter(eScreen   key)
{
	CScreen::enter(_array[key]);
}

void CScreen::enter(CScreen * screen)
{
	if (screen != _current_screen)
	{
		CGraphicManager::clear();
		if (_current_screen != NULL)
		{
			_current_screen->close();
		}
		_current_screen = screen;
#ifdef USE_KEYPAD
		Keypad::set_keypad(_current_screen);
#endif		
		if (_current_screen == NULL)
			return;
		CGraphicManager::add(_current_screen);
		_current_screen->open();
		_current_screen->update_50ms();		
	}
}
