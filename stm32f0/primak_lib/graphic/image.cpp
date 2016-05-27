#include "image.h"
#include "debug_print_interface.h"


CPixelArea IImage::get_area(CPixel point, alignment align)
{
	CPixel left_top;
	switch(align & 3)
	{
		case _A_LEFT:    
			left_top.x = point.x;
			break;
		case _A_RIGHT:    
			left_top.x = point.x - XSize();
			break;
		case _A_HCENTER:  
			left_top.x = point.x - XSize()/2;				
			break;
		default:
			PRINT_PLACE_AND_STOP();
	}
	
	switch((align) & (3ul<<2))
	{
		case _A_TOP:    
			left_top.y = point.y;
			break;
		case _A_BOTTOM:    
			left_top.y = point.y - YSize();
			break;
		case _A_VCENTER:  
			left_top.y = point.y - YSize()/2;				
			break;
		default:
			PRINT_PLACE_AND_STOP();
	}
	
	ASSERT(GrLib::framebuffer_area.is_inside(left_top));
	ASSERT(GrLib::framebuffer_area.is_inside(left_top + CPixel(XSize()-1, YSize()-1)));
	
	return (YSize() && XSize())? CPixelArea(left_top, left_top + CPixel(XSize()-1, YSize()-1)):CPixelArea(left_top, left_top);
}


BaseImageCollection::BaseImageCollection(CPixel base, alignment align):CGraphicObjWithRectArea(base),_key_is_set(false),_align(align)
{
}


void BaseImageCollection::delete_elements(void)
{
	for (_map_t::iterator it = _map.begin(); it != _map.end(); ++it)
	{
		delete it->second;
	}
	_map.clear();
}

CSize BaseImageCollection::get_max_size(void)
{
	CPixel maxe_size_global(0,0);
	for (_map_t::iterator it = _map.begin(); it != _map.end(); ++it)
	{
		maxe_size_global.x = max2<int>(maxe_size_global.x, it->second->XSize());
		maxe_size_global.y = max2<int>(maxe_size_global.y, it->second->YSize());
	}
	return maxe_size_global;
}
void BaseImageCollection::redraw( bool force_full_redraw)
{
	if ((force_full_redraw || (_last_drawed != _key)) && _key_is_set)
	{
		_last_drawed = _key;
		CGraphicObjWithRectArea::clear();
		_map_t::iterator it = _map.find(_key);
		if (it != _map.end())
		{
			it->second->draw(_base_point, _align);
			int x_size = it->second->XSize();
			int y_size = it->second->YSize();
			set_clear_area(it->second->get_area(_base_point, _align));
			after_redraw();
		}
	}
}

void BaseImageCollection::add(int key, IImage* image)
{
	_map[key] = image;
}
void BaseImageCollection::set_state(int key)
{
	_key = key;
	_key_is_set = true;
}
