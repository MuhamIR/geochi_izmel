#include "filters.h"
#include "macros.h"
#include "memory_operations.h"

float CExponentialSmooth::new_val(float data)
{
	_data = _data * (1 - _smooth_ampl) + data * _smooth_ampl;
	return _data;
}


CGroupExponentialSmooth::CGroupExponentialSmooth( float ampl , UI num_of, float init_data)
{
	_num = num_of;
	_smooth_ampl = ampl;
	_data = new float[num_of]; 
	for (int i = 0; i < num_of; i++) 
		_data[i] = init_data; 
}

float CGroupExponentialSmooth::new_val(UI num, float data)
{
	_data[num] = _data[num] * (1 - _smooth_ampl) + data * _smooth_ampl;
	return _data[num];
}


// class CRingSmooth
// {
// 	public:
// 		
// 	protected:
// // 		U16 * _data;
// // 		UI _num;
// // 		UI _ptr;
// };

CRingSmooth::CRingSmooth( UI size)
{
	restrict_range<UI>( size,1, 128 );
	_size = size;
	_data = new U16[_size];
	memory_set<U16>(_data, 0 , _size);
	_ptr = 0;
}


bool CRingSmooth::resize( UI size)
{
	restrict_range<UI>( size,1, 128 );
	if (_size != size)
	{
		delete _data;	
		_size = size;
		_data = new U16[_size];	
		_ptr = 0;
		return true;
	}
	return false;
}

void CRingSmooth::reset(  U16 val)
{
	memory_set<U16>(_data, 0 , _size);
}

void CRingSmooth::set_new_val(U16 data)
{
	if(++_ptr >= _size)
	{
		_ptr = 0;
	}
	_data[_ptr] = data;
}
U16 CRingSmooth::get_smooth(void)
{
	UI sum = 0;
	for(UI i = 0;i<_size; i++)
	{
		sum+=_data[i];
	}
	return (sum + _size/2)/_size;
}



// #ifndef UNUSUAL_CURRENT_FEEDBACK
// #define KZ_PROPORTION	0.7
// #define OBRIV_PROPORTION	0.7
// #define NORMAL_PROPORTION	0.7
// #endif




// void CStatisticOutputDiagnostic::init(void)
// {
// 	_previous_state = NO_DIAGNOSTIC;
// 	_state = NORMAL_STATE;
// 	_in_1 = 0;
// 	_in_2 = 0;
// 	_in_3 = 0;
// 	_counter = 0;
// }

// CStatisticOutputDiagnostic::CStatisticOutputDiagnostic()
// {
// 	init();
// }


// U16 CStatisticOutputDiagnostic::add_val(U16 val)
// {
// 	if (val <= ADC_LIMIT1)
// 	   _in_1++;
// 	   
// 	if (( val > ADC_LIMIT1) && (val < ADC_LIMIT2) )
// 	   _in_2++;	

// 	if (val >= ADC_LIMIT2) 
// 	   _in_3++;
// 	   
// 	if (_counter++ == STAT_SIZE)
// 	{
// 		_previous_state = _state;
// 		if ( _in_1 > KZ_PROPORTION * STAT_SIZE)
// 			_state = KZ;
// 		else if (_in_2 > OBRIV_PROPORTION * STAT_SIZE)
// 			_state = OBRIV;
// 		else if (_in_3 > NORMAL_PROPORTION * STAT_SIZE)
// 			_state = NORMAL_STATE;

// 		_in_1 = 0;
// 		_in_2 = 0;
// 		_in_3 = 0;
// 		_counter = 0;
// 	}	
// 	return _state;
// }


