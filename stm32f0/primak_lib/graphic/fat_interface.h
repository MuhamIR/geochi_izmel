#ifndef _FATFS_INTERFACE_H_
#define _FATFS_INTERFACE_H_

#include "ff.h"
#include "TypeDef.h"

namespace Fat
{
	
FRESULT open (FIL*, const char*, BYTE);			/* Open or create a file */
FRESULT read (FIL*, void*, UINT, UINT*);			/* Read data from a file */
FRESULT close (FIL*);	
	
class RFile
{
public:
	enum status_e
	{
		IS_NOT_CREATED =		0,
		ALLOC_FAT_FILE =		(1UL << 0),
		IS_OPEN =				(1UL << 1),
		DATA_IS_ALOCCATED =		(1UL << 2),
		DATA_IS_READ =			(1UL << 3)
	};
	RFile();
	RFile(const char * path);
	~RFile();
	bool open(const char * path );
	bool close();
	inline void * get_data(void){return data;}
	inline UI get_size(void)const{return size;}
protected:
	U32 status;
	void * data;
	UI size;
	FIL * file;
};


}

#endif

