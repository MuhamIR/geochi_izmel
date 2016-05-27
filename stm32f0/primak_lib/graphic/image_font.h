#ifndef _IMAGE_FONT_H_
#define _IMAGE_FONT_H_

#include <vector>
#include "image_file.h"
#include "graphic_print.h"

class FontPool;

class ImageFont
{
	friend class FontPool;
public:		
	void print(CPixel base, const char  * capture, alignment align = center, CPixelArea * area_to_clear = NULL, bool with_alpha = false)const;
	UI YSize(void)const{return _y_size;}
	UI XSize(void)const{return _x_size;}
	std::vector<std::string> size_split(const char * s, UI size)const;
private:
	ImageFont( UI x_size, UI y_size, const char * directory, const char * char_set = NULL);
	virtual ~ImageFont();
	static const UI char_num = 256;
	
	const UI _x_size;
	const UI _y_size;
	BufferedImage * _simbols[char_num];	
};

class FontPool
{
public:
	static const ImageFont* load( UI x_size, UI y_size, const char * directory, bool is_temporary = true, const char * char_set = NULL);
	static void removeTemporaryFonts();
	inline static const ImageFont* defaultFont(){ return load( 15, 28, "/f/b28/", false);}
private:
	struct FontInfo
	{
		const ImageFont* font;
		std::string directory;
		bool is_temporary;		
	};
	static const UI NumOfFonts = 8;
	static FontInfo _pool[NumOfFonts];
};

class ImageString : public CGraphicString
{
public:
	ImageString(CPixel base = CPixel(0,0), alignment align = center, const ImageFont * font = NULL, const char  * capture = "" );	
	virtual void redraw( bool force_full_redraw);
	void operator= (const std::string& str){_current_s = str;}
	void operator= (const char* s){_current_s = s;}
	void set_font(const ImageFont * font){_font = font; must_be_redrawed();}
protected:
	const ImageFont * _font;
};


class ImageText : public CGraphicString
{
public:
	ImageText(CPixelArea area , const ImageFont * font = NULL, alignment align = center, bool with_alpha = false);
	virtual void redraw( bool force_full_redraw);
	void set_alignment(alignment align){_align = align;}
private:
	CPixel _end;
	const ImageFont * _font;
	alignment _align;
	bool _with_alpha;
};



#endif

