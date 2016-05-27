#ifndef __PIXEL_H__
#define __PIXEL_H__
#include "typedef.h"

/* Text alignment flags, horizontal */
#define _A_LEFT        (0<<0)
#define _A_RIGHT	     (1<<0)
#define _A_CENTER	     (2<<0)
#define _A_HCENTER	   _A_CENTER  /* easier to remember :-)  */

/* Text alignment flags, vertical */
#define _A_VERTICAL   (3<<2)
#define _A_TOP	      (0<<2)
#define _A_BOTTOM	    (1<<2)
#define _A_BASELINE   (2<<2)
#define _A_VCENTER    (3<<2)

enum alignment{
	left_center = _A_LEFT|_A_VCENTER, left_top = _A_LEFT|_A_TOP, left_bottom = _A_LEFT|_A_BOTTOM, 
	right_center = _A_RIGHT|_A_VCENTER, right_top = _A_RIGHT|_A_TOP, right_bottom = _A_RIGHT|_A_BOTTOM, 
	center_center = _A_HCENTER|_A_VCENTER, center_top = _A_HCENTER|_A_TOP, center_bottom = _A_HCENTER|_A_BOTTOM,
	center = _A_HCENTER|_A_VCENTER
};


struct CPixel
{
	CPixel():
	x ( 0),
	y ( 0){	}
	CPixel(int xx, int yy):
	x(xx), y(yy){}
	CPixel(const CPixel & elem):x(elem.x), y(elem.y){};		
	int x;
	int y;		
};

typedef CPixel CSize;

bool operator==( const CPixel & obj1, const CPixel & obj2);
bool operator!=(const CPixel & obj1, const CPixel & obj2);
bool operator>=(const CPixel & obj1, const CPixel & obj2);
bool operator<=(const CPixel & obj1, const CPixel & obj2);
CPixel operator*(const CPixel & obj1, int obj2);
CPixel operator/(const CPixel & obj1, int obj2);
CPixel operator-(const CPixel & obj1, const CPixel & obj2);
CPixel operator+(const CPixel & obj1, const CPixel & obj2);

struct CPixelArea												   
{
	CPixelArea(){init( CPixel(0,0),  CPixel(0,0));};
	CPixelArea(const CPixel & a_start, const CPixel & a_end) {init(a_start, a_end);};
	void init(const CPixel & a_start, const CPixel & a_end);
	CPixel getAnchor(alignment align) const;
		
	bool is_inside(const CPixel & pixel) const
	{
		return ((_start <= pixel) && (_end >= pixel));
	};
	
	bool is_inside(const CPixelArea & area) const
	{
		return ((_start <= area._start) && (_end >= area._end) && (_end >= area._start) && (_start <= area._end));
	};
	inline int get_v_size(void) const
	{
		return (_end.y -_start.y + 1);
	}
	inline int get_h_size(void) const
	{
		return (_end.x -_start.x + 1);
	}
	const CPixel & start()const{return _start;}
	const CPixel & end()const{return _end;}
private:
	CPixel _start;
	CPixel _end;
};

CPixelArea operator+(const CPixelArea & obj1, const CPixel & obj2);


#endif

