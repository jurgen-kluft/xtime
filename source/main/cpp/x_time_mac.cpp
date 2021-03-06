#include "xbase/x_target.h"
#ifdef TARGET_MAC

#include <time.h>

#include "xbase/x_debug.h"

#include "xtime/x_time.h"
#include "xtime/x_timespan.h"
#include "xtime/x_datetime.h"

#include "xtime/private/x_time_source.h"
#include "xtime/private/x_datetime_source.h"

namespace xcore
{
	class xdatetime_source_mac : public datetime_source_t
	{
	public:
		virtual u64			getSystemTimeUtc()
		{
			time_t curTime;
			time( &curTime );

			tm		gmTime = (*gmtime( &curTime ));

			datetime_t dt(gmTime.tm_year + 1900, gmTime.tm_mon + 1, gmTime.tm_mday, gmTime.tm_hour, gmTime.tm_min, gmTime.tm_sec);
			return (u64)dt.ticks();
		}

		virtual u64			getSystemTimeLocal()
		{
			time_t	curTime;
			time(&curTime);

			tm		localTime = (*localtime(&curTime));

			datetime_t dt(1900 + localTime.tm_year, localTime.tm_mon + 1, localTime.tm_mday, localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
			return (u64)dt.ticks();
		}

		virtual s64			getSystemTimeZone()
		{
			// NOT implemented yet
			return 0;
		}

		virtual u64			getSystemTimeAsFileTime()
		{
			u64 systemTime = getSystemTimeLocal();
			return getFileTimeFromSystemTime(systemTime);
		}

		virtual u64			getSystemTimeFromFileTime(u64 inTime)
		{
			u32 second, minute, hour, year, month, day;

			second	= inTime % 60;
			inTime	/= 60;
			minute	= inTime % 60;
			inTime	/= 60;
			hour	= inTime % 24;
			inTime	/= 24;

			u32	uYear	= 1970;
			u32	uMonth	= 0;
			u32	uDay	= 0;

			while (xTRUE)
			{
				bool	boLeap			= false;
				u32		uNumDaysInYear	= 365;

				if((uYear % 4) == 0)
				{
					uNumDaysInYear	= 366;
					boLeap			= true;
				}

				if(inTime >= uNumDaysInYear)
				{
					inTime	-= uNumDaysInYear;
					uYear++;
				}
				else
				{
					for(u32 uI = 0; uI < 12; uI++)
					{
						u32	uNumDaysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

						if(uI == 1)
						{
							if(boLeap)
							{
								uNumDaysPerMonth[uI] = 29;
							}
							else
							{
								uNumDaysPerMonth[uI] = 28;
							}
						}

						if(inTime >= uNumDaysPerMonth[uI])
						{
							inTime -= uNumDaysPerMonth[uI];
							uMonth++;
						}
						else
						{
							uDay	= inTime;
							break;
						}
					}

					break;
				}
			}

			day		= uDay + 1;
			month	= uMonth;
			year	= uYear;

			datetime_t dt = datetime_t(year, month, day, hour, minute, second);
			return dt.ticks();
		}

		virtual u64			getFileTimeFromSystemTime(u64 inSystemTime)
		{
			return inSystemTime;
		}
	};

	/**
	 * Time source for Mac OS
	 */
	class xtime_source_mac : public time_source_t
	{
		f64				mFreqPerSec;
		tick_t			mBaseTimeTick;
		tick_t			mLastTicks;

	public:
		void			init()
		{
			mFreqPerSec  = CLOCKS_PER_SEC;
			mBaseTimeTick = clock();
		}

		/**
         * ------------------------------------------------------------------------------
		 *   Author:
		 *       Virtuos
		 *   Summary:
		 *       Get elapsed time from timer initialized in second.
		 *   Arguments:
		 *       void
		 *   Returns:
		 *       Ticks that have elapsed from x_TimeInit called
		 *   Description:
		 *       use xcritical_section to make PerformanceCounter owned by only one thread
		 *       at a time.
		 *   See Also:
		 *       xcritical_section
		 * ------------------------------------------------------------------------------
		 */
		virtual tick_t	getTimeInTicks()
		{
			ASSERT(mBaseTimeTick);
			s64 ticks = clock();
			ticks -= mBaseTimeTick;
			return ticks;
		}

		virtual s64		getTicksPerSecond()
		{
			return mFreqPerSec;
		}
	};
};

namespace xtime
{
	void x_Init(void)
	{
		static xcore::xtime_source_mac sTimeSource;
		sTimeSource.init();
		xcore::x_SetTimeSource(&sTimeSource);

		static xcore::xdatetime_source_mac sDateTimeSource;
		xcore::x_SetDateTimeSource(&sDateTimeSource);
	}

	void x_Exit(void)
	{
		xcore::x_SetTimeSource(NULL);
		xcore::x_SetDateTimeSource(NULL);
	}
}

#endif /// TARGET_MAC
