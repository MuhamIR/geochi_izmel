#include <string.h>
#include "macros.h" 
#include "image_file.h"
#include "debug_print_interface.h"
#include "framebuffer.h"
#include "fat_interface.h" 
#include "usefull.h" 




__packed struct tagBITMAPFILEHEADER 
{ 
	WORD    bfType; 
	DWORD   bfSize; 
	WORD    bfReserved1; 
	WORD    bfReserved2; 
	DWORD   bfOffBits; 
};

__packed struct tagBITMAPCOREHEADER
{
	DWORD  bcSize; 
	LONG   bcWidth; 
	LONG   bcHeight; 
	WORD   bcPlanes; 
	WORD   bcBitCount;
};

__packed struct tagBITMAPINFOHEADER
{
	DWORD  biCompression;
	DWORD  biSizeImage;
	LONG   biXPelsPerMeter;
	LONG   biYPelsPerMeter;
	DWORD  biClrUsed;
	DWORD  biClrImportant;
};

__packed struct tagBITMAPV4HEADER
{
	DWORD  bV4RedMask;
	DWORD  bV4GreenMask;
	DWORD  bV4BlueMask;
	DWORD  bV4AlphaMask;
};

static U8 getMaskOffset( U32 mask )
{
	for (U8 i = 0; i < 32; i++)
		if (mask & (1 << i)) return i;
	return 32;
};

static U8 getMaskSize( U32 mask )
{
	mask >>= getMaskOffset(mask);
	for (U8 i = 0; i < 32; i++)
		if (!(mask & (1 << i))) return i;
	return 32;
}

struct parsed_bmp
{
	UI x_size;
	UI y_size;
	U16 * _buf;
};

template<typename T>
static void fill(char **p, T** data)
{
	*data = (T*)*p;
	*p += sizeof(T);
}

bool parse_bmp( const char * fileName, parsed_bmp & bmp )
{
	bmp.x_size = 0;
	bmp.y_size = 0;
	bmp._buf = NULL;

	Fat::RFile file(fileName);
	if (file.get_size() == 0)
	{
		PUTS("\n\r Image "); PUTS( fileName ); PUTS(": failed to open file"); FLUSH();
		return false;
	}
		

	tagBITMAPFILEHEADER * bfh = NULL;
	tagBITMAPCOREHEADER * bch = NULL;
	tagBITMAPINFOHEADER * bih = NULL;
	tagBITMAPV4HEADER *   b4h = NULL;
	char *p = (char *)file.get_data();
	fill(&p, &bfh);
	fill(&p, &bch);
			
	if ( bch->bcSize > 12 )
	{
		fill(&p, &bih);
		if ( bch->bcSize > 40 )
		{
			fill(&p, &b4h);
			int tempL = bfh->bfOffBits - sizeof( tagBITMAPFILEHEADER ) - sizeof( tagBITMAPCOREHEADER ) - sizeof( tagBITMAPINFOHEADER ) - sizeof( tagBITMAPV4HEADER );
			p += tempL;
		}
	}
			
	if ( bch->bcBitCount == 16 )
	{
		UI row_size_in_bytes = bch->bcWidth * 2 + (bch->bcWidth * 2) % 4;
		bmp.x_size = bch->bcWidth;
		bmp.y_size = bch->bcHeight;
		bmp._buf = new U16[bmp.x_size * bmp.y_size];
		U16 * dst = bmp._buf;
		
		UI bV4RedMask;
		UI bV4GreenMask;
		UI bV4BlueMask;
		if ( b4h )
		{
			bV4RedMask = b4h->bV4RedMask;
			bV4GreenMask = b4h->bV4GreenMask;
			bV4BlueMask = b4h->bV4BlueMask;
		}
		else
		{
			bV4RedMask = 0x7C00;
			bV4GreenMask = 0x03E0;
			bV4BlueMask = 0x001F;
		}


		UI red_offset = getMaskOffset(bV4RedMask);
		UI green_offset = getMaskOffset(bV4GreenMask);
		UI blue_offset = getMaskOffset(bV4BlueMask);
		UI green_bits = getMaskSize(bV4GreenMask);
		ff(  i, bch->bcHeight )
		{	
			U16 * src = (U16*) (p + (bch->bcHeight - 1 - i) * row_size_in_bytes);
			ff(  j, bch->bcWidth )
			{			
				U16 color = *src++;				
				U16 red = ((color & bV4RedMask) >> red_offset);
				red = red << 0;
				U16 blue = ((color & bV4BlueMask) >> blue_offset);
				blue = blue << 11;
				U16 green = ((color & bV4GreenMask) >> green_offset);
				green = green << ((green_bits == 6) ? 5 : 6);
				*dst++ = red | green | blue;
			}
		}
	}
	else
	{
		PUTS("\n\r Image "); PUTS( fileName ); PUTS(": wrong encoding format \n\r"); PUTS("Permitted 16bit, 24bit, 32bit color"); FLUSH();
	}
	return false;
};



void BufferedImage::init(const char *path)
{
	const char * ptr = path;
	while((*ptr != '.') && *ptr)
	{
		ptr++;
	}
	if (*ptr == '.')
	{
		if (strcmp(ptr, ".bmp") == 0)
		{
			parsed_bmp bmp;
			parse_bmp( path, bmp );
			x_size = bmp.x_size;
			y_size = bmp.y_size;
			_buf = bmp._buf;
		}
		else
		{
			PUTS("You can use only .bmp:");PUTS(path);PUTS("\r\n");
			
			_buf = NULL;
		}
	}
}

BufferedImage::BufferedImage(const char *path)
{
	init(path);
}

bool BufferedImage::draw(CPixel point, alignment align)
{
	if (_buf)
	{
		CPixelArea t = get_area(CPixel(point.x, point.y), align);
		ASSERT(GrLib::framebuffer_area.is_inside(t));
		
		
		GrLib::draw_from_source(t, _buf );
	}
	return true;
}

bool BufferedImage::draw_w_alpha(CPixel point, alignment align, color_t alpha)
{
	if (_buf)
	{
		CPixelArea t = get_area(CPixel(point.x, point.y), align);
		ASSERT(GrLib::framebuffer_area.is_inside(t));
		
		GrLib::draw_from_source_with_alpha_color(t, _buf , alpha);
	}
	return true;
}

bool BufferedImage::draw_in_area(CPixel point, const CPixelArea & only_in_area, alignment align)
{
	if (_buf)
	{
		CPixelArea t = get_area(CPixel(point.x, point.y), align);
		ASSERT(GrLib::framebuffer_area.is_inside(t));
		ASSERT(align == left_top);//пока в этой функции подерживается только это выравнивание
		
		ff(y, y_size)
		{	
			if(in_range<int>(y + point.y, only_in_area.start().y, only_in_area.end().y))
			{
				ff(x, x_size)
				{
					if(in_range<int>(x + point.x, only_in_area.start().x, only_in_area.end().x))
					{
						GrLib::set_pixel(point + CPixel(x, y), _buf[y * x_size + x]);
					}
				}
			}			
		}	
		
	}
	return true;
}


BufferedImage::~BufferedImage()
{
	delete _buf;
}

struct parsed_alpha_bmp
{
	UI x_size;
	UI y_size;
	U16 * _buf;
	U8 * _alpha;
};


bool parse_bmp( const char * fileName, parsed_alpha_bmp & bmp )
{
	bmp.x_size = 0;
	bmp.y_size = 0;
	bmp._buf = NULL;
	bmp._alpha = NULL;
	Fat::RFile file(fileName);
	if (file.get_size() == 0)
	{
		PUTS("\n\r Image "); PUTS( fileName ); PUTS(": failed to open file"); FLUSH();
		return false;
	}
		

	tagBITMAPFILEHEADER * bfh = NULL;
	tagBITMAPCOREHEADER * bch = NULL;
	tagBITMAPINFOHEADER * bih = NULL;
	tagBITMAPV4HEADER *   b4h = NULL;
	char *p = (char *)file.get_data();
	fill(&p, &bfh);
	fill(&p, &bch);
			
	if ( bch->bcSize > 12 )
	{
		fill(&p, &bih);
		if ( bch->bcSize > 40 )
		{
			fill(&p, &b4h);
			int tempL = bfh->bfOffBits - sizeof( tagBITMAPFILEHEADER ) - sizeof( tagBITMAPCOREHEADER ) - sizeof( tagBITMAPINFOHEADER ) - sizeof( tagBITMAPV4HEADER );
			p += tempL;
		}
	}
			
	if ( bch->bcBitCount == 32 )
	{
		UI row_size_in_bytes = bch->bcWidth * 4;
		bmp.x_size = bch->bcWidth;
		bmp.y_size = bch->bcHeight;
		bmp._buf = new U16[bmp.x_size * bmp.y_size];
		U16 * buf = bmp._buf;
		bmp._alpha = new U8[bmp.x_size * bmp.y_size];
		U8 * alpha = bmp._alpha;

		ff(  i, bch->bcHeight )
		{	
			U8 * src = (U8*) (p + (bmp.y_size - 1 - i) * row_size_in_bytes);
			ff(  j, bmp.x_size )
			{						
					
				U16 blue = ((*src++) >> 3);
				blue = blue << 11;				
				U16 green = ((*src++) >> 2);
				green = green << 5;
				U16 red = ((*src++) >> 3);
				red = red << 0;		
				*buf++ = red | green | blue;
				*alpha++ = *src++;
			}
		}
	}
	else
	{
		PUTS("\n\r Image "); PUTS( fileName ); PUTS(": wrong encoding format \n\r"); PUTS("Permitted 16bit, 24bit, 32bit color"); FLUSH();
	}
	return false;
};



void BufImageWAlpha::init(const char *path)
{
	const char * ptr = path;
	while((*ptr != '.') && *ptr)
	{
		ptr++;
	}
	if (*ptr == '.')
	{
		if (strcmp(ptr, ".bmp") == 0)
		{
			parsed_alpha_bmp bmp;
			parse_bmp( path, bmp );
			x_size = bmp.x_size;
			y_size = bmp.y_size;
			_buf = bmp._buf;
			_alpha = bmp._alpha;
		}
		else
		{
			PUTS("You can use only .bmp:");PUTS(path);PUTS("\r\n");
			
			_buf = NULL;
		}
	}
}

BufImageWAlpha::BufImageWAlpha(const char *path)
{
	init(path);
}

bool BufImageWAlpha::draw(CPixel point, alignment align)
{
	if (_buf)
	{
		CPixelArea t = get_area(CPixel(point.x, point.y), align);
		ASSERT(GrLib::framebuffer_area.is_inside(t));

		GrLib::draw_from_source_with_alpha_channel_U8(t, _buf, _alpha);
	}
	return true;
}




BufImageWAlpha::~BufImageWAlpha()
{
	delete _buf;
	delete _alpha;
}

//-------------------------------------------------------------

CPixelArea ImageObj::get_area(void)
{
	return _image->get_area(_base_point, _align);
}


ImageObj::ImageObj(CPixel base, IImage * image, alignment align):
CGraphicObjWithRectArea(base),
_image(image),
_destroy_flag(false),
_align(align)
{
}

ImageObj::ImageObj(CPixel base, const char *path, alignment align):
CGraphicObjWithRectArea(base),
_image(new BufferedImage(path)),
_destroy_flag(true),
_align(align)
{
}


ImageObj::~ImageObj()
{
	if(_destroy_flag)
	{
		delete _image;
	}
}

void ImageObj::redraw( bool force_full_redraw)
{
	if(force_full_redraw)
	{
		CGraphicObjWithRectArea::clear();
		_image->draw(_base_point, _align);
		set_clear_area(get_area());
		after_redraw();
	}
}

	





