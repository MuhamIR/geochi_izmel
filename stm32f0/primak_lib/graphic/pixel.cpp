#include "pixel.h"
#include "macros.h"
#include "debug_print_interface.h"

CPixelArea operator+(const CPixelArea & obj1, const CPixel & obj2)
{
	return CPixelArea(obj1.start() + obj2, obj1.end() + obj2); 
}

bool operator==(const CPixel & obj1, const CPixel & obj2)
{
	return ((obj1.x == obj2.x) && (obj1.y == obj2.y));
}

bool operator!=(const CPixel & obj1, const CPixel & obj2)
{
	return ((obj1.x != obj2.x) || (obj1.y != obj2.y));
}

bool operator>=(const CPixel & obj1, const CPixel & obj2)
{
	return ((obj1.x >= obj2.x) && (obj1.y >= obj2.y));
}

bool operator<=(const CPixel & obj1, const CPixel & obj2)
{
	return ((obj1.x <= obj2.x) && (obj1.y <= obj2.y));
}

CPixel operator-(const CPixel & obj1, const CPixel & obj2)
{
	return 	CPixel(obj1.x - obj2.x, obj1.y - obj2.y);
}
CPixel operator+(const CPixel & obj1, const CPixel & obj2)
{
	return 	CPixel(obj1.x + obj2.x, obj1.y + obj2.y);
}

CPixel operator*(const CPixel & obj1, int obj2)
{
	return 	CPixel(obj1.x * obj2, obj1.y * obj2);
}
CPixel operator/(const CPixel & obj1, int obj2)
{
	return 	CPixel(obj1.x / obj2, obj1.y / obj2);
}

void CPixelArea::init(const CPixel & a_start, const CPixel & a_end) 
{
	_start.x = MIN(a_start.x, a_end.x); 
	_start.y = MIN(a_start.y, a_end.y); 
	_end.x = MAX(a_start.x, a_end.x); 
	_end.y = MAX(a_start.y, a_end.y); 
}

CPixel CPixelArea::getAnchor(alignment align) const
{
	CPixel res;
	switch(align & 3)
	{
		case _A_LEFT:    
			res.x = _start.x;
			break;
		case _A_RIGHT:    
			res.x = _start.x + get_h_size();
			break;
		case _A_HCENTER:  
			res.x = _start.x + get_h_size()/2;				
			break;
		default:
			PRINT_PLACE_AND_STOP();
	}
	
	switch((align) & (3ul<<2))
	{
		case _A_TOP:    
			res.y = _start.y;
			break;
		case _A_BOTTOM:    
			res.y = _start.y + get_v_size();
			break;
		case _A_VCENTER:  
			res.y = _start.y + get_v_size()/2;				
			break;
		default:
			PRINT_PLACE_AND_STOP();
	}
	return res;
}
