#include "graphic_manager.h"
#include "macros.h"
#include "debug_print_interface.h"

bool CGraphicManager::_global_redraw = true;	
bool CGraphicManager::_global_clear = true;	
std::list<CGraphicObj*> CGraphicManager::_list;


CGraphicObj::CGraphicObj():
_flag_to_clear(0),
_drawed_flag(0),
_hide_flag(0),
_blinking_flag(0),
_blinking_counter(0),
_blinking_limit(0),
_blinking_state(0),
_req_to_full_redraw(0)
{
}

void CGraphicObj::hide(void)
{
	if(!_hide_flag)
	{
		_hide_flag = true;
		_flag_to_clear = true;
	}
}

void CGraphicObj::set_blinking(bool flag, UI limit)
{
	if (_blinking_flag != flag)
	{
		_blinking_flag = flag;
		if (_blinking_flag)
		{
			_blinking_state = true;
			_blinking_limit = limit;
			_blinking_counter = 0;
		}
		else
		{
			_req_to_full_redraw = true;
		}
	}
}

void CGraphicObj::update_blinking( void)
{
	if (_blinking_flag)
	{
		if (++_blinking_counter >= _blinking_limit)
		{
			_blinking_state = !_blinking_state;
			if (_blinking_state)
			{
				_req_to_full_redraw = true;
			}
			else
				_flag_to_clear = true;
			_blinking_counter = 0;
		}
	}
}

void CGraphicObj::show(void)
{
	if(_hide_flag)
	{
		_hide_flag = false;
		_req_to_full_redraw = true;
	}
}

void CGraphicObj::set_visibility(bool state)
{
	if(state)
		show();
	else
		hide();
}




CGraphicLocatedObj::CGraphicLocatedObj(CPixel point)
:CGraphicObj(),_base_point(point)
{
	
}


CGraphicObjWithRectArea::CGraphicObjWithRectArea(CPixel point):
CGraphicLocatedObj(point), _is_defined(false), back_color(GrLib::eBlack)
{
}

void CGraphicObjWithRectArea::clear( void )
{
	if (_is_defined && _drawed_flag)
	{
		GrLib::draw_area(_area, back_color);
	}
	after_clear();
}



void CGraphicManager::draw_all(void )
{
	if (  _global_redraw)
	{
		_global_redraw = false;
		for (obj_list::iterator it = _list.begin(); it != _list.end(); ++it)
		{
			if ((*it)->_hide_flag == false)
			{
				if (((*it)->_blinking_flag == false) || (((*it)->_blinking_flag) && ((*it)->_blinking_state)) )
				{
					(*it)->redraw(true);
					(*it)->_req_to_full_redraw = false;
				}
			}
		}
	}
	else
	{
		for (obj_list::iterator it = _list.begin(); it != _list.end(); ++it)
		{
			if ((*it)->_hide_flag == false)
			{
				if (((*it)->_blinking_flag == false) || (((*it)->_blinking_flag) && ((*it)->_blinking_state)) )
				{
					if ((*it)->_req_to_full_redraw == true)
					{
						(*it)->redraw(true); 
						(*it)->_req_to_full_redraw = false;
					}
					else
						(*it)->redraw(false); 
				}
			}
		}
	}
}





void CGraphicManager::clear_all( void )
{
	if (_global_clear == false)
	{
		for (obj_list::iterator it = _list.begin(); it != _list.end(); ++it)
		{
			if ( (*it)->_flag_to_clear)
			{
				(*it)->clear(); 
			}
		}
	}
	else
	{
		_global_clear = false;
		extern void clear_framebuffer();
		clear_framebuffer();
		for (obj_list::iterator it = _list.begin(); it != _list.end(); ++it)
		{
			(*it)->_flag_to_clear = false;
		}
	}
	
}


void CGraphicManager::add(CGraphicObj * obj)
{
	obj->_req_to_full_redraw = true;
	_list.push_back(obj);
}

void CGraphicManager::clear(void)
{
	_global_clear = true;
	_list.clear();
}



void CGraphicManager::remove(CGraphicObj * obj)
{
	_global_clear = true;
	_global_redraw = true;
	_list.remove(obj);
};


void CGraphicManager::update_50ms( void)
{
	extern void framebuffer_wait_for_update();
	framebuffer_wait_for_update();
	
	for (obj_list::iterator it = _list.begin(); it != _list.end(); ++it)
	{
		(*it)->update_50ms();
		(*it)->update_blinking(); 
	}
	
	clear_all();
	draw_all();
	
	extern void framebuffer_was_updated();
	framebuffer_was_updated();
}

