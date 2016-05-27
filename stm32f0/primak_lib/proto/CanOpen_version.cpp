#include "revision_proj.h"
#include "revision_lib.h"
#include "proto_def.h"

void fill_version( U64 & version )
{
	version_t & ver( *(version_t *)&version );
	
	std::tm data = NSSubWCRevProject::TimeNow;
	ver.time = std::mktime( &data );
	ver.project = NSSubWCRevProject::Revision;
	ver.library = NSSubWCRevLib::Revision;
#ifdef FOR_DEBUG
	ver.project |= 0x8000;
#endif
}

