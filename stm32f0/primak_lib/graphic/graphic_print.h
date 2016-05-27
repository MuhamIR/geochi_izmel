#ifndef _GRAPHIC_PRINT_H_
#define _GRAPHIC_PRINT_H_

#include "graphic_manager.h"
#include <string>



class CGraphicString : public CGraphicObjWithRectArea
{
public:
	CGraphicString(CPixel base, alignment align = center,  const char  * capture = ""):
	CGraphicObjWithRectArea(base), _current_s(capture), _drawed_s(""),_align(align){}
		
	void set(const std::string& str){_current_s = str;}
	void operator= (const std::string& str){_current_s = str;}
	void operator= (const char* s){_current_s = s;}
	operator std::string(){ return _current_s; }
	void change_place(CPixel base, alignment align){move_to(base); _align = align; must_be_redrawed();}
protected:
	std::string _current_s;
	std::string _drawed_s;
	alignment _align;
};

#endif




