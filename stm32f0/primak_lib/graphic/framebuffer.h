#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include "TypeDef.h"
#include "pixel.h"
#include "LCD_config.h"


namespace GrLib
{
	
	enum colors 
	{	
		eBlack = RGB(0, 0, 0),
		eWhite = RGB(0xFF, 0xFF, 0xFF),
		
		eRed = RGB(0xFF, 0, 0),
		eBlue = RGB(0, 0, 0xFF),
		eGreen = RGB(0, 0xFF, 0),
		
		eDarkRed = RGB(0x80, 0, 0),
		eDarkBlue = RGB(0, 0, 0x80),
		eDarkGreen = RGB(0, 0x80, 0),
		
		eLightRed = RGB(0xFF, 0x80, 0x80),
		eLightBlue = RGB(0x80, 0x80, 0xFF),
		eLightGreen = RGB(0x80, 0xFF, 0x80),
		
		eCyan = RGB(0, 0xFF, 0xFF),
		eMagenta = RGB(0xFF, 0, 0xFF),
		eYellow = RGB(0xFF, 0xFF, 0),
		
		eLightGray = RGB(0xD3, 0xD3, 0xD3),
		eGray = RGB(0x80, 0x80, 0x80),
		eDarkGray = RGB(0x40, 0x40, 0x40),
		
		eOrange = RGB(0xff, 0xa5, 0x0),
	};
	
	color_t * const framebuffer = (color_t *)LCD_LINEAR_BUFFER_BASE;
	const UI LCD_x_size = LCD_H_SIZE;
	const UI LCD_y_size = LCD_V_SIZE;
	
	const CPixel Center(GrLib::LCD_x_size/2, GrLib::LCD_y_size/2);
	const CPixelArea framebuffer_area( CPixel(0,0), CPixel(LCD_x_size - 1, LCD_y_size - 1));
	
#if (LCD_INVERT_ROWS == 0) && (LCD_INVERT_COLUMNS == 0)	
#define CONVERT_Y(y) (y)
#define CONVERT_X(x) (x)
#endif	

#if (LCD_INVERT_ROWS == 0) && (LCD_INVERT_COLUMNS)	
#define CONVERT_Y(y) (y)
#define CONVERT_X(x) (GrLib::LCD_x_size - 1 - (x))
#endif
	
#if LCD_INVERT_ROWS && (LCD_INVERT_COLUMNS == 0)	
#define CONVERT_Y(y) (GrLib::LCD_y_size - 1 - (y))
#define CONVERT_X(x) (x)
#endif	
	
#if LCD_INVERT_ROWS && LCD_INVERT_COLUMNS
#define CONVERT_Y(y) (GrLib::LCD_y_size - 1 - (y))
#define CONVERT_X(x) (GrLib::LCD_x_size - 1 - (x))
#endif	

	inline void set_pixel(UI x, UI y, color_t color)
	{
		*(framebuffer + CONVERT_Y(y) * LCD_x_size + CONVERT_X(x)) = color;
	}
	
	inline void set_pixel(const CPixel & pixel, color_t color)
	{
		set_pixel(pixel.x, pixel.y, color);
	}
	
	inline color_t get_color(UI x, UI y)
	{
		return *(framebuffer + CONVERT_Y(y) * LCD_x_size + CONVERT_X(x));
	}
	
	inline color_t get_color(const CPixel & pixel)
	{
		return get_color(pixel.x, pixel.y);
	}
	
	void draw_line(const CPixel & start, const CPixel & end, color_t color);
	void draw_V_line(UI x, color_t color);
	void draw_H_line(UI y, color_t color);
	void draw_area(const CPixelArea & area, color_t color);
	void draw_from_source(const CPixelArea & area, const color_t * source );
	void draw_from_source_with_alpha_color(const CPixelArea & area, const color_t * source , color_t alpha);
	void draw_from_source_with_alpha_channel_U8(const CPixelArea & area, const color_t * source , const U8 *alpha);
	
	void FillCircle(CPixel center_point, int radius, color_t color);
}


#endif
