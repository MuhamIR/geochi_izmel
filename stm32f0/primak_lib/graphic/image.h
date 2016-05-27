#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <map>

#include "macros.h" 
#include "graphic_manager.h" 

class IImage
{
public:
	virtual ~IImage(){}
	virtual bool draw(CPixel point, alignment align) = 0;
	CPixelArea get_area(CPixel point, alignment align);
	virtual UI XSize(void) = 0;
	virtual UI YSize(void) = 0;
};


class BaseImageCollection : public CGraphicObjWithRectArea
{
public:	
	BaseImageCollection(CPixel base = CPixel(0,0), alignment align = left_top);
	void delete_elements(void);	
	void change_place(CPixel base, alignment align){_base_point = base; _align = align;must_be_redrawed();}
	CSize get_max_size(void);
	virtual void redraw( bool force_full_redraw);
protected:
	void add(int key, IImage* image);
	void set_state(int key);
private:
	typedef std::map<int, IImage*> _map_t;
	_map_t _map;
	int _key;
	int _last_drawed;
	bool _key_is_set;
	alignment _align;
};


template <typename T>
class ImageCollection : public BaseImageCollection
{
public:
	ImageCollection(CPixel base = CPixel(0,0), alignment align = center):BaseImageCollection(base, align)	{	}
	void add(T key, IImage* image){	BaseImageCollection::add( key,  image);	}
	void set_state(T key)	{	BaseImageCollection::set_state(key);	}
private:
};


#endif

