#ifndef __X_TIME_DATETIME_SOURCE_H__
#define __X_TIME_DATETIME_SOURCE_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
//  INCLUDES
//==============================================================================
#include "xbase\x_types.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	class xdatetime_source
	{
	public:
		// The platform specific part
		virtual u64			getSystemTime() = 0;
		virtual u64			getSystemTimeAsFileTime() = 0;
		virtual u64			getSystemTimeFromFileTime(u64 inFileSystemTime) = 0;
		virtual u64			getFileTimeFromSystemTime(u64 inSystemTime) = 0;
	};

	extern void				x_SetDateTimeSource(xdatetime_source*);
	
	//==============================================================================
	// END xCore namespace
	//==============================================================================
};


//==============================================================================
// END __X_TIME_DATETIME_SOURCE_H__
//==============================================================================
#endif