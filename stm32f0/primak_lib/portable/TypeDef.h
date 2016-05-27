#ifndef TYPEDEF_H
#define TYPEDEF_H

#include <inttypes.h>

//����� �� ������� ����������, � ������� �������� � ���� ������ ������� �������� ��������� � ����� 0
//������� ��� ��������� ��������� � ����� ��������� ������������ NULL
#ifndef NULL
#define NULL  0
#endif

typedef enum {FALSE = 0, TRUE = 1} Bool;
typedef enum {DISABLE = 0, ENABLE = 1} FunctionalState;

typedef unsigned long address_t;
typedef volatile unsigned long VULONG;

//��������� 4 ������� �� ������������� � ���������� � ����� �������� - � ����� � ����������������� � ��������� �������� short, int, long � 
//�������������� ����������� (������) ����� ������ ������������ ����� �������������, �� ��� ���� ������� �������� �����
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
typedef unsigned int UINT;

//��������� ����������� ����� ������������� � ����������
typedef uint8_t BYTE;
typedef uint8_t		U8;
typedef int8_t		S8;
typedef int8_t		I8;

typedef uint16_t	U16;
typedef int16_t		S16;
typedef int16_t		I16;

typedef uint32_t	U32;
typedef int32_t		S32;
typedef int32_t		I32;

typedef uint64_t	U64;
typedef __int64		S64;
typedef __int64		I64;


#define MAX_U8	0xFF
#define MAX_U16	0xFFFF
#define MAX_U32	0xFFFFFFFF
#define MAX_I32	0x7FFFFFFF

//���� ����������� �� �����, ���������� ��������� UI
typedef unsigned int	UI;

enum OnOff_t {
	OFF = 0,
	ON = 1
};

enum extOnOff_t
{
	EXT_UNDEF = 0,
	EXT_OFF 	= 1,
	EXT_ON 		= 2,
	EXT_ERROR = 3
};

enum extCmd_t
{
	EXT_CMD_NONE = 0,
	EXT_CMD_OFF = 1,
	EXT_CMD_ON = 2,
	EXT_CMD_STOP = 3
};

typedef const char * c_str_t;

#define USHORT_MAX  0xFFFF
#define SSHORT_MAX  0x7fff


#ifdef __cplusplus
//������ � ����������� ������������� ������������� ����������� ������������� ���� (�� �������� enum)
//������ ����� ���������� ��� ���������� (������� �������� �� ����� �� ������� (��������������� ��� ������� ������� �������� �����������)
//���������� ���� �� ���������� �� ����� ����������� �� �������, � ������� �� ����������� ������� � ��������������� warning
//���� ����������� ������������, ��� � ����������� ����������
template< typename Tind, typename Tdata, UI num >
class tpmas_t
{
	Tdata mas[num];
	public:
		Tdata & operator[]( Tind i ){ return mas[i]; }
		Tdata * begin( void ){ return mas; }
		Tdata * end( void ){ return &mas[num]; }
	public:
		typedef  Tdata * iterator;
		static inline UI size(void){return num;}
};

//������� ��� ����������������� ���������� ������ ���� (�� �������� enum)
template< typename Tenum > inline void incEnum( Tenum * i ){	++(*(int*)i); }

//���������� ������� std::vrector, �� ��� �++ �����.
//����� ��������� ��������� ����, ������� � �����������.
template< typename T >
class smartmas_t
{
	T * data;
	public:
		smartmas_t(){ data = 0; _wasCreated = false; _size = 0; }
		smartmas_t( UI size ){ _wasCreated = false; create(size); }
		~smartmas_t(){ clear(); }
		void create( UI size )
		{
			clear();
			if( size )
			{
				_size = size;
				data = new T[_size];
				_wasCreated = true;
			}
		}
		void clear( void )
		{
			if( _wasCreated )
			{
				delete[] data;
				_size = 0;
				_wasCreated = false;
			}
		}
		T & operator[]( UI i ){ return data[i]; }
		T * begin( void ){ return data; }
		T * end( void ){ return &data[_size]; }
		inline UI size(void) const {return _size;}
		typedef T * iterator;
	private:
		bool _wasCreated;
		UI _size;
};


#endif

#endif
