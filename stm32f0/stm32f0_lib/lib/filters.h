#pragma once
#include "System.h"

class CExponentialSmooth
{
	public:
		CExponentialSmooth(){_smooth_ampl = 1.; _data = 0;};
		void init( float ampl , float init_data = 0){_smooth_ampl = ampl; _data = init_data;};
		CExponentialSmooth( float ampl , float init_data = 0){init(ampl, init_data);};
		void reset(float data) {_data = data;};
		float new_val(float data);
		void set_smooth_ampl(float ampl){_smooth_ampl = ampl;};
		float get_smooth(){return _data;};
	protected:
		float _data;
		float _smooth_ampl;
};


class CGroupExponentialSmooth
{
	public:
		CGroupExponentialSmooth( float ampl , UI num_of, float init_data = 0);
		void reset(UI num, float data) {_data[num] = data;};
		float new_val(UI num, float data);
		float get_smooth(UI num){return _data[num];};
	protected:
		float * _data;
		float _smooth_ampl;
		UI _num;
};

class CRingSmooth
{
	public:
		CRingSmooth( UI size = 4);
		void reset( U16 val);
		bool resize( UI size);
		void set_new_val(U16 data);
		U16 get_smooth(void);
	protected:
		U16 * _data;
		UI _size;
		UI _ptr;
};


#define NO_DIAGNOSTIC 0
#define KZ 1
#define OBRIV 2
#define NORMAL_STATE 3


// class CStatisticOutputDiagnostic
// {
// 	public:
// 		CStatisticOutputDiagnostic();
// 		void init(void);
// 		U16 add_val(U16 val);
// 		inline UI get_state(){return  _state;};
// 		inline UI get_prev_state(){return  _previous_state;};
// 	protected:
// 		
// 		U16 _in_1;
// 		U16 _in_2;
// 		U16 _in_3;
// 		U16 _state;
// 		U16 _counter;
// 		U16 _previous_state;
// };

