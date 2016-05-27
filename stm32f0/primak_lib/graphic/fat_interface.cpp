#include "fat_interface.h"
#include "debug_print_interface.h"


Fat::RFile::RFile()
{
	size = 0;
	data = NULL;
	file = NULL;
	status = IS_NOT_CREATED;
}

Fat::RFile::RFile(const char * path)
{
	size = 0;
	data = NULL;
	file = NULL;
	status = IS_NOT_CREATED;
	if (open( path) == false)
	{
//		dtrap();
	}
}

bool Fat::RFile::open(const char * path)
{
	if (status != IS_NOT_CREATED)
		return false;
	FRESULT rc;
	file = new FIL;
	if (file != NULL)
		status |= ALLOC_FAT_FILE;
	rc = Fat::open(file, path, FA_READ);	
	
	if (rc)
	{
		PUTS("CANNOT OPEN FILE ");
		PUTS(path);
		return false;
	}
	status |= IS_OPEN;
	
	data = new U8[(file->fsize + 256 - 1) & ~(256 - 1)];	

	if ((file->fsize) && (data == NULL))
		return false;
	status |= DATA_IS_ALOCCATED;
	size = 	file->fsize;
	UI br;		
	rc = Fat::read(file, data, file->fsize, &br);
	if (rc)
		return false;
	status |= DATA_IS_READ;
	
	rc = Fat::close(file);
	if (rc)
		return false;
	status &= ~IS_OPEN;
	
	delete file;
	file = NULL;
	status &= ~ALLOC_FAT_FILE;
	return true;
}

bool Fat::RFile::close( )
{
	if (status == IS_NOT_CREATED)
		return true;
	if (status & DATA_IS_READ)
	{
		status &= ~DATA_IS_READ;
	}
	if (status & DATA_IS_ALOCCATED)
	{
		delete static_cast<U8*>(data);
		data = NULL;
		status &= ~DATA_IS_ALOCCATED;
		size = 0;
	}
	if (status & IS_OPEN)
	{
		FRESULT rc = Fat::close(file);
		if (rc)
			return false;
		status &= ~IS_OPEN;
	}
	if (status & ALLOC_FAT_FILE)
	{
		delete file;
		file = NULL;
		status &= ~ALLOC_FAT_FILE;
	}
	if (status == IS_NOT_CREATED)
	{
		return true;
	}
	else
		return false;
}


Fat::RFile::~RFile ()
{
	if (close() == false)
	{
		PRINT_PLACE_AND_STOP();
	}
}
