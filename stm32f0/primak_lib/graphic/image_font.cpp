#include <vector>
#include "image_font.h"
#include "tostring.h"
#include "debug_print_interface.h"
#include "memory_operations.h"
#include "fat_interface.h" 

static bool find(char c, const char * s)
{
	while(*s)
	{
		if(*s == c)
			return true;
		s++;
	}
	return false;
}
	
ImageFont::ImageFont( UI x_size, UI y_size, const char * directory, const char * char_set):
_x_size(x_size),_y_size(y_size)
{
	FIL * file = new FIL;
	char buf[32];
	ff(i, char_num)
	{
		_simbols[i] = NULL;
		if(char_set != NULL)
		{
			if (find(i, char_set) == false)
				continue;
		}
		char * p = buf;
		p = ToString::add(p, directory);
		p = ToString::uint_to_hex_fstring(p, i, 2);
		p = ToString::add(p, ".bmp");
		
		if (Fat::open (file, buf, FA_READ) == FR_OK)
		{
			Fat::close (file);
			_simbols[i] = new BufferedImage(buf);
			ASSERT(_simbols[i] != NULL);
			ASSERT(_simbols[i]->XSize() == _x_size);
			ASSERT(_simbols[i]->YSize() == _y_size);			
		}
	}
	
	// если есть только большие буквы, то вместо маленьких тоже будут большие
	for( UI i = 'a'; i <= 'z'; ++i)
	{
		if (_simbols[i] == NULL)
		{
			_simbols[i] = _simbols[i-0x20];
		}
	}
	for( UI i = 'а'; i <= 'я'; ++i)
	{
		if (_simbols[i] == NULL)
		{
			_simbols[i] = _simbols[i-0x20];
		}
	}
	
	delete file;
}

ImageFont::~ImageFont()
{
	ff(i, char_num)
	{
		//убираем дублирование больших и маленьких букв
		if (in_range<int>(i, 'a','z') || in_range<int>(i, 'а','я'))
		{
			if (_simbols[i] && (_simbols[i] == _simbols[i-0x20]))
				_simbols[i] = NULL;
		}
	}
	ff(i, char_num)
	{
		if (_simbols[i])
			delete _simbols[i];
	}
}



void ImageFont::print(CPixel base, const char  * capture, alignment align, CPixelArea * area_to_clear, bool with_alpha)const
{
	
	CPixel left_top_point;
	int x_size = std::strlen(capture) * _x_size;
	
	switch(align & 3)
	{
		case _A_LEFT:    
			left_top_point.x = base.x;
			break;
		case _A_RIGHT:    
			left_top_point.x = base.x - x_size;
			break;
		case _A_HCENTER:  
			left_top_point.x = base.x - x_size/2;				
			break;
		default:
			PRINT_PLACE_AND_STOP();
	}
	
	switch((align) & (3ul<<2))
	{
		case _A_TOP:    
			left_top_point.y = base.y;
			break;
		case _A_BOTTOM:    
			left_top_point.y = base.y - _y_size;
			break;
		case _A_VCENTER:  
			left_top_point.y = base.y - _y_size/2;				
			break;
		default:
			PRINT_PLACE_AND_STOP();
	}
	
	ASSERT(GrLib::framebuffer_area.is_inside(left_top_point));
	ASSERT(GrLib::framebuffer_area.is_inside(left_top_point + CPixel(x_size, _y_size)));
	
	if(area_to_clear != NULL)
	{
		area_to_clear->init( left_top_point, left_top_point + CPixel(x_size, _y_size));
	}
	
	while(*capture)
	{
		if (_simbols[*capture] != NULL)
		{
			if (with_alpha)
			{
				_simbols[*capture]->draw_w_alpha(left_top_point, left_top, 0);
			}
			else
				{
				_simbols[*capture]->draw(left_top_point, left_top);
			}
		}
		else
		{
		}
		left_top_point.x += _x_size;
		++capture;
	}
}

//---------------------------------------------------------------

FontPool::FontInfo FontPool::_pool[NumOfFonts];

const ImageFont* FontPool::load( UI x_size, UI y_size, const char * directory, bool is_temporary, const char * char_set)
{
	//проверяем, не был ли еще загружен
	ff(i, NumOfFonts)
	{
		if(_pool[i].font && (_pool[i].directory == directory))
		{
			ASSERT(_pool[i].font->XSize() == x_size);
			ASSERT(_pool[i].font->YSize() == y_size);
			if ( is_temporary == false)
			{
				_pool[i].is_temporary = false;			
			}
			return _pool[i].font;
		}
	}
	ff(i, NumOfFonts)
	{
		if(_pool[i].font == NULL)
		{
			_pool[i].font = new ImageFont( x_size, y_size, directory, char_set);
			_pool[i].directory = directory;
			_pool[i].is_temporary = is_temporary;
			return _pool[i].font;
		}
	}
	return NULL;
}

void FontPool::removeTemporaryFonts()
{
	ff(i, NumOfFonts)
	{
		if((_pool[i].font != NULL) && _pool[i].is_temporary)
		{
			delete _pool[i].font;
			_pool[i].font = 0;
		}
	}
}

//---------------------------------------------------------------


ImageString::ImageString(CPixel base, alignment align, const ImageFont * font, const char  * capture ):
CGraphicString(base, align, capture), _font(font)
{
	if (_font == NULL)
	{
		_font = FontPool::defaultFont();
		ASSERT(_font != NULL);
	}
}



void ImageString::redraw( bool force_full_redraw)
{
	if(force_full_redraw || _current_s.compare(_drawed_s))
	{
		CGraphicObjWithRectArea::clear();
		CPixelArea to_clear;
		_font->print(_base_point, _current_s.c_str(), _align, &to_clear);
		set_clear_area(to_clear.start(),to_clear.end());
		_drawed_s = _current_s;
		after_redraw();
	}
}

//-----------------------------------------------------------------

std::vector<std::string> ImageFont::size_split(const char * s, UI size)const
{
	UI char_size = XSize();
	//Сначала разделяем строку на слова
	std::vector<std::string> word_vector;
	std::string word = "";
	while(*s)
	{
		if(*s == ' ')
		{
			if (word != "")
			{
				word_vector.push_back(word);
				word = "";
			}
		}
		else if(*s == '\n')
		{
			if (word != "")
			{
				word_vector.push_back(word);				
			}
			word = "\n";
		}
		else
		{
			word += *s;
		}
		s++;
	}
	if (word != "")
	{
		word_vector.push_back(word);
	}
	
	
	//Затем из слов собираем последовательности длиной в пикселях меньше или равно указанной
	std::vector<std::string> sequnces;
	std::string sequence = "";
	UI sequence_len = 0;
	ff(i, word_vector.size())
	{
		UI word_len = word_vector[i].length() * char_size;
		if (word_len > size)
		{//обрезание слишком длинных слов
			std::string temp;
			ff(char_counter, word_vector[i].length())
			{
				temp += word_vector[i][char_counter];
				if (temp.length() * char_size > size)
				{
					//последний характер был лишним
					word_vector[i] = word_vector[i].substr(0, char_counter-1);
					word_vector[i] += '*';
					word_len = word_vector[i].length() * char_size;
					break;
				}
			}
		}
		
		if (word_vector[i][0] == '\n')
		{
			if (sequence_len != 0)
			{
				sequnces.push_back(sequence);
				sequence = "";
				sequence_len = 0;
			}
			word_vector[i] = word_vector[i].c_str()+1;
		}
		
		if (sequence_len != 0)
		{
			if ( (sequence_len + word_len + char_size) <= size)
			{
				sequence_len += char_size + word_len;
				sequence += " " + word_vector[i];
			}
			else
			{
				sequnces.push_back(sequence);
				sequence = "";
				sequence_len = 0;
			}
		}
		
		if (sequence_len == 0)
		{
			sequence_len += word_len;
			sequence += word_vector[i];
		}
 	}
	if (sequence != "")
	{
		sequnces.push_back(sequence);
	}
	if (sequnces.size() == 0)
	{
		sequnces.push_back("");
	}
	return sequnces;
}



ImageText::ImageText(CPixelArea area , const ImageFont * font, alignment align, bool with_alpha)
:CGraphicString(area.start(), align),
_end(area.end()),
_font(font),
_align(align),
_with_alpha(with_alpha)
{
	if (_font == NULL)
	{
		_font = FontPool::defaultFont();
		ASSERT(_font != NULL);
	}
	set_clear_area(area);
}



void ImageText::redraw( bool force_full_redraw)
{
	if(force_full_redraw || _current_s.compare(_drawed_s))
	{
		if(_with_alpha == false)
		{
			clear();
		}
		_drawed_s = _current_s;
		CPixelArea area(_base_point, _end);
		std::vector<std::string> rows = _font->size_split(_current_s.c_str(), area.get_h_size());
		UI max_rows = area.get_v_size() / _font->YSize();
		UI row_number = MIN(rows.size(), max_rows);
		
		if (_align == center)
		{
			CPixel base = area.getAnchor(center_top);
			base.y += (max_rows - row_number) * _font->YSize()/2 + (area.get_v_size() - max_rows*_font->YSize())/2;
			ff(i, row_number)
			{
				_font->print(base, rows[i].c_str(), center_top, NULL, _with_alpha);			
				base.y += _font->YSize();
			}
		}
		else if (_align == left_top)
		{
			CPixel base = area.getAnchor(left_top);
			ff(i, row_number)
			{
				_font->print(base, rows[i].c_str(), left_top, NULL, _with_alpha);			
				base.y += _font->YSize();
			}
		}
		after_redraw();
	}
}
