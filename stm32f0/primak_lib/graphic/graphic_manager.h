#ifndef _GRAPHIC_MANAGER_H_
#define _GRAPHIC_MANAGER_H_


#include <list>
#include "TypeDef.h"
#include "framebuffer.h"

class CGraphicManager;
class CGraphicComposition;

class CGraphicObj
{
friend class CGraphicManager;
friend class CGraphicComposition;
public:
	CGraphicObj();
	virtual ~CGraphicObj(){}					
	void hide(void);//скрыть объект
	void show(void);//показать объект
	void set_visibility(bool state);//установить видимость объекта, true - объект отображается	
	void set_blinking(bool flag, UI limit = 4);//установка мигание с задаваемой частотой(flag == true значит мигать, при этом у объекта должна быть корректно определена функция clear())
protected:	
	virtual void redraw( bool force_full_redraw)  = 0;
	virtual void clear( void ) = 0;
	void after_redraw( void) {_drawed_flag = true;};
	void after_clear( void ) {_drawed_flag = false;_flag_to_clear = false;};
	virtual void update_50ms( void ) {};
	void update_blinking( void);
	bool _flag_to_clear;
	bool _drawed_flag;
	void must_be_redrawed(void){_req_to_full_redraw = true;}
private:
	bool _hide_flag;
	bool _blinking_flag;//true, если объект должен мигать
	UI _blinking_counter;//счетчик для задания времени мигания
	UI _blinking_limit;//время между сменой состояния(скрыт/виден)
	bool _blinking_state;//текущее состояние(скрыт или виден)
	bool _req_to_full_redraw;
};

class CGraphicComposition: public std::list<CGraphicObj*>, public CGraphicObj
{
public:
	void delete_elements( void )
	{
		for(std::list<CGraphicObj*>::iterator it = std::list<CGraphicObj*>::begin(); it != std::list<CGraphicObj*>::end();++it)
		{
			delete (*it);
		}
	}		
protected:	
	virtual void redraw( bool force_full_redraw)
	{
		for(std::list<CGraphicObj*>::iterator it = std::list<CGraphicObj*>::begin(); it != std::list<CGraphicObj*>::end();++it)
		{
			(*it)->redraw( force_full_redraw);
		}
		after_redraw();
	}
	virtual void clear( void )
	{
		for(std::list<CGraphicObj*>::iterator it = std::list<CGraphicObj*>::begin(); it != std::list<CGraphicObj*>::end();++it)
		{
			(*it)->clear();
		}
		after_clear();
	}
};

class CGraphicLocatedObj : public CGraphicObj
{
public:
	CGraphicLocatedObj(CPixel point);

	virtual void move_to(CPixel point)
	{
		if (point != _base_point)
		{
			_base_point = point;
			if ( _drawed_flag)
				_flag_to_clear = true;	
			must_be_redrawed();
		}
	}
	void add_row(int offset)
	{
		move_to(_base_point + CPixel(0, offset));
	}
protected:
	CPixel _base_point;
};

class CGraphicObjWithRectArea : public CGraphicLocatedObj
{
public:
	CGraphicObjWithRectArea(CPixel point);	
	void set_back_color(color_t color){back_color = color;}
	const CPixelArea & get_area()const {return _area;}
protected:
	virtual void clear( void );
	void set_clear_area(CPixel start, CPixel end)
	{
		_area = CPixelArea(start, end);
		_is_defined = true;
	}
	void set_clear_area(const CPixelArea & area)
	{
		_area = area;
		_is_defined = true;
	}	
private:
	CPixelArea _area;
	bool _is_defined;
	color_t back_color;
};



class CGraphicManager
{
public:
	static void add(CGraphicObj * obj);
	static void add(CGraphicObj & obj){add(&obj);}
	static void remove(CGraphicObj * obj);
	static void update_50ms( void);
	static void clear(void);
	static void full_redraw_request(void){_global_redraw = true;_global_clear = true;}
protected:
	CGraphicManager(){}
	static void draw_all(void );
	static void clear_all( void);	
	static void unset_global_redraw(void){_global_redraw = 0;};	
	static bool _global_redraw;	
	static bool _global_clear;	
	typedef std::list<CGraphicObj*> obj_list;
	static obj_list _list;
};

typedef CGraphicManager GrMan;

#endif

