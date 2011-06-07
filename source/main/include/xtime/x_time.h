#ifndef __X_TIME_H__
#define __X_TIME_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

//==============================================================================
//  INCLUDES
//==============================================================================
#include "xbase\x_types.h"
#include "xbase\x_string_std.h"

//==============================================================================
// xCore namespace
//==============================================================================
namespace xcore
{
	//==============================================================================
	// Library initialization
	//==============================================================================
	extern void    x_TimeInit (void);
	extern void    x_TimeExit (void);

	//==============================================================================
	// GLOBAL FUNCTIONS
	//==============================================================================

	//==============================================================================
	// Types
	//==============================================================================
	typedef		s64		xtick;

	extern xtick   x_GetTime           (void);
	extern f64     x_GetTimeSec        (void);
	extern f64     x_TicksToMs         (xtick inTicks);
	extern f64     x_TicksToSec        (xtick inTicks);
	extern s64     x_GetTicksPerMs     (void);
	extern s64     x_GetTicksPerSecond (void);

	//==============================================================================
	// INLINE
	//==============================================================================
	#include "private/x_time_inline.h"


	//==============================================================================
	// END xCore namespace
	//==============================================================================
};


//==============================================================================
// END __X_TIME_H__
//==============================================================================
#endif