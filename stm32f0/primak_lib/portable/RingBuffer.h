#ifndef _RING_BUFFER_H_
#define  _RING_BUFFER_H_
#include "TypeDef.h"

template<class DATA_T=unsigned char>
class RingBuffer
{
public:
	typedef UI INDEX_T;
private:
	DATA_T * _data;
	UI _size;	
	volatile bool _full_flag;
	volatile INDEX_T _readCount; 
	volatile INDEX_T _writeCount;
public:
	RingBuffer(UI size){init(size);};
	void init(UI size)
	{
		_data = new DATA_T[size]; 
		if (_data == NULL) 
			while(1); 
		_size = size; 
		Clear();
	};
	inline bool Write(const DATA_T& value)
	{
		if(IsFull())
		{
			Clear();
		}
		_data[_writeCount] = value;
		if ((_writeCount + 1) == _size)
			_writeCount = 0;
		else 
			_writeCount++;
		if (_writeCount == _readCount)
			_full_flag=1;
		return true;
	}
	inline bool Read(DATA_T &value)
	{
		if(IsEmpty())
			return false;
		value = _data[_readCount];
		if ((_readCount + 1) == _size)
			_readCount = 0;
		else 
			_readCount++;
		_full_flag=0;
		return true;
	}
		
	inline bool get(DATA_T *value)
	{
		if(IsEmpty())
			return false;
		*value = _data[_readCount];
		return true;
	}
		
	inline bool pop(DATA_T &value)
	{
		if(IsEmpty())
			return false;
		if ((_readCount + 1) == _size)
			_readCount = 0;
		else 
			_readCount++;
		_full_flag=0;
		return true;
	}
		
	inline bool push(const DATA_T& value)
	{
		return Write(value);
	}
   
	inline bool IsEmpty()const
	{
		return (_writeCount == _readCount) && !_full_flag;
	}
   
	inline bool IsFull()const
	{
		return _full_flag;
	}

	INDEX_T Count()const
	{
		if ( _writeCount >= _readCount)
			return (_writeCount - _readCount);
		else
			return (_size + _writeCount - _readCount);
	}

	inline void Clear()
	{
		_readCount=0;
		_writeCount=0;
		_full_flag=0;
	}

	inline unsigned Size() {return _size;}
};

#endif
