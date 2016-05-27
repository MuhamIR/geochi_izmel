#ifndef _IMAGE_FILE_H_
#define _IMAGE_FILE_H_

#include <string> 
#include "image.h"






class BufferedImage : public IImage
{
public:
	BufferedImage(const char *path);
	BufferedImage(const std::string & path){init(path.c_str());}
	virtual ~BufferedImage();
	virtual bool draw(CPixel point, alignment align);
	virtual bool draw_w_alpha(CPixel point, alignment align, color_t alpha);
	virtual bool draw_in_area(CPixel point, const CPixelArea & only_in_area, alignment align = left_top);
	virtual UI XSize(void){return x_size;}
	virtual UI YSize(void){return y_size;}
private:
	void init(const char *path);
	UI x_size;
	UI y_size;
	U16 * _buf;
};

class BufImageWAlpha : public IImage
{
public:
	BufImageWAlpha(const char *path);
	BufImageWAlpha(std::string path){init(path.c_str());}
	virtual ~BufImageWAlpha();
	virtual bool draw(CPixel point, alignment align);
	virtual UI XSize(void){return x_size;}
	virtual UI YSize(void){return y_size;}
private:
	void init(const char *path);
	UI x_size;
	UI y_size;
	U16 * _buf;
	U8 * _alpha;
};


class ImageObj : public CGraphicObjWithRectArea
{
public:

	ImageObj(CPixel base, IImage * image, alignment align = center);
	ImageObj(CPixel base, const char *path, alignment align = center);
	virtual ~ImageObj();
	virtual void redraw( bool force_full_redraw);
	CPixelArea get_area(void);
	UI XSize(void){return _image->XSize();}
	UI YSize(void){return _image->YSize();}
	void change_place(CPixel base, alignment align){_base_point = base; _align = align;must_be_redrawed();}
private:
	IImage * _image;
	bool _destroy_flag;
	alignment _align;
};



#endif


