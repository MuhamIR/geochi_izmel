#include <math.h>
#include "framebuffer.h"
#include "debug_print_interface.h"
#include "macros.h"
#include "memory_operations.h"
#include "usefull.h"

void GrLib::draw_V_line(UI x, color_t color)
{
	ASSERT(x < LCD_x_size);
	ff(y, LCD_y_size)
	{
		set_pixel(x, y, color);
	}
}
void GrLib::draw_H_line(UI y, color_t color)
{
	ASSERT(y < LCD_y_size);
	ff(x, LCD_x_size)
	{
		set_pixel(x, y, color);
	}
}

void GrLib::draw_line(const CPixel & start, const CPixel & end, color_t color)
{
	ASSERT(framebuffer_area.is_inside(start));
	ASSERT(framebuffer_area.is_inside(end));
	if (ABS(end.x - start.x) > ABS(end.y - start.y))
	{
		int x_start = (start.x < end.x)? start.x : end.x;
		int x_end = (start.x < end.x)? end.x : start.x;
		for (int x = x_start; x <= x_end; x++)
		{
			set_pixel(x, start.y + ((end.y - start.y) * (x - start.x))/(end.x - start.x), color);
		}
	}
	else
	{
		int y_start = (start.y < end.y)? start.y : end.y;
		int y_end = (start.y < end.y)? end.y : start.y;
		for (int y = y_start; y <= y_end; y++)
		{
			set_pixel(start.x + ((end.x - start.x) * (y - start.y))/(end.y - start.y), y, color);
		}
	}
}

#if (LCD_INVERT_ROWS == 0) && (LCD_INVERT_COLUMNS == 0)	

#define ADD_ROW(dst)		dst += GrLib::LCD_x_size;
#define ADD_PIXEL(dst)		dst++;
#define SET_AND_ADD_PIXEL_FROM_POINTER(dst, src)		*t_dst++ = *src++;
#define SET_AND_ADD(dst, color)		*dst++ = (color);

#endif


#if (LCD_INVERT_ROWS == 0) && LCD_INVERT_COLUMNS


#define ADD_ROW(dst)		dst += GrLib::LCD_x_size;
#define ADD_PIXEL(dst)		dst--;
#define SET_AND_ADD_PIXEL_FROM_POINTER(dst, src)		*t_dst-- = *src++;
#define SET_AND_ADD(dst, color)		*dst-- = (color);

#endif


#if LCD_INVERT_ROWS && (LCD_INVERT_COLUMNS == 0)	


#define ADD_ROW(dst)		dst -= GrLib::LCD_x_size;
#define ADD_PIXEL(dst)		dst++;
#define SET_AND_ADD_PIXEL_FROM_POINTER(dst, src)		*t_dst++ = *src++;
#define SET_AND_ADD(dst, color)		*dst++ = (color);
#endif

#if LCD_INVERT_ROWS && LCD_INVERT_COLUMNS


#define ADD_ROW(dst)		dst -= GrLib::LCD_x_size;
#define ADD_PIXEL(dst)		dst--;
#define SET_AND_ADD_PIXEL_FROM_POINTER(dst, src)		*t_dst-- = *src++;
#define SET_AND_ADD(dst, color)		*dst-- = (color);
#endif

void GrLib::draw_area(const CPixelArea & area, color_t color)
{
	ASSERT(framebuffer_area.is_inside(area));
	for (int y = area.start().y; y <= area.end().y; y++)
	{
		color_t * dst = framebuffer + CONVERT_Y(y) * LCD_x_size + CONVERT_X(area.start().x);
		UI size = area.get_h_size();
		while(size--)
		{
			SET_AND_ADD(dst, color)
		}
	}
}

void GrLib::draw_from_source(const CPixelArea & area, const color_t * source )
{
	color_t * dst = GrLib::framebuffer + CONVERT_X(area.start().x) + CONVERT_Y(area.start().y)*GrLib::LCD_x_size;
	for  ( UI yy = 0 ; yy <  area.get_v_size(); yy++)
	{	
		color_t * t_dst = dst;
		for  ( UI xx = 0 ; xx <  area.get_h_size(); xx++)
		{
			SET_AND_ADD_PIXEL_FROM_POINTER(t_dst, source)
		}
		ADD_ROW(dst)
	}
}

void GrLib::draw_from_source_with_alpha_color(const CPixelArea & area, const color_t * source , color_t alpha)
{
	color_t * dst = GrLib::framebuffer + CONVERT_X(area.start().x) + CONVERT_Y(area.start().y)*GrLib::LCD_x_size;
	for  ( UI yy = 0 ; yy <  area.get_v_size(); yy++)
	{	
		color_t * t_dst = dst;
		for  ( UI xx = 0 ; xx <  area.get_h_size(); xx++)
		{
			if (alpha != *source)
			{
				SET_AND_ADD_PIXEL_FROM_POINTER(t_dst, source)
			}
			else
			{
				ADD_PIXEL(t_dst)
				source++;
			}
		}
		ADD_ROW(dst)
	}
}


void GrLib::draw_from_source_with_alpha_channel_U8(const CPixelArea & area, const color_t * source , const U8 *alpha)
{	
	const UI red_offset = true_get_bit_num(eRed);
	const UI green_offset = true_get_bit_num(eGreen);
	const UI blue_offset = true_get_bit_num(eBlue);
	const UI red_mask = (1UL << get_num_of_bit(eRed))-1;
	const UI green_mask = (1UL << get_num_of_bit(eGreen))-1;
	const UI blue_mask = (1UL << get_num_of_bit(eBlue))-1;
	
	U16 * dst = GrLib::framebuffer + CONVERT_X(area.start().x) + CONVERT_Y(area.start().y)*GrLib::LCD_x_size;
	for  ( UI yy = 0 ; yy <  area.get_v_size(); yy++)
	{	
		volatile U16 * t_dst = dst;
		for  ( UI xx = 0 ; xx <  area.get_h_size(); xx++)
		{			
			UI red = (((*t_dst >> red_offset) & red_mask) * (255-*alpha) + ((*source >> red_offset)& red_mask)*(*alpha) + 127)/255;
			UI green = (((*t_dst >> green_offset)& green_mask)* (255-*alpha) + ((*source >> green_offset)& green_mask)*(*alpha) + 127)/255;
			UI blue = (((*t_dst >> blue_offset)& blue_mask)* (255-*alpha) + ((*source >> blue_offset)& blue_mask)*(*alpha) + 127)/255;
			
			*t_dst = (red << red_offset) | (green << green_offset) | (blue << blue_offset);
			ADD_PIXEL(t_dst)
			source++;
			alpha++;
		}
		ADD_ROW(dst)
	}
}

void GrLib::FillCircle(CPixel center_point, int radius, color_t color)
{
	float r_sqr =  radius * radius;	
	ff(i, 2 * radius - 2)
	{
		GrLib::set_pixel(center_point.x - radius + 1 + i, center_point.y, color);
	}
	for (int y = 1; y < radius; y++)
	{
		float temp = r_sqr - y*y;
		ULONG delta;
		temp = sqrt(temp);
		delta = temp;
		ff(x, 2 * delta)
		{
			GrLib::set_pixel(center_point.x - delta + x, center_point.y + y, color);
			GrLib::set_pixel(center_point.x - delta + x, center_point.y - y, color);
		}
	}	
}
