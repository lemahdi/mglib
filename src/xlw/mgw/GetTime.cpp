//============================================================================
//============================================================================
//
// Financial Applications of Excel Add-in Development in C/C++ (2nd edition)
// 
// Author: Steve Dalton
// 
// Published by John Wiley & Sons Ltd, The Atrium, Southern Gate, Chichester,
// West Sussex, PO19 8SQ, UK.
// 
// All intellectual property rights and copyright in the code listed in this
// module are reserved.  You may reproduce and use this code only as permitted
// in the Rules of Use that you agreed to on installation.  To use the material
// in this source module, or material derived from it, in any other way you
// must first obtain written permission.  Requests for permission can be sent
// by email to permissions@eigensys.com.
// 
// No warranty, explicit or implied, is made by either the author or publisher
// as to the quality, fitness for a particular purpose, accuracy or
// appropriateness of material in this module.  The code and methods contained
// are intended soley for example and clarification.  You should not rely on
// any part of this code without having completely satisfied yourself that it
// is correct and appropriate for your needs.
//
//============================================================================
//============================================================================
// This source file contains code relating to getting the system time and
// system clock.
//============================================================================
//============================================================================
#include <time.h>
#pragma warning (disable : 4996)

#define SECS_PER_DAY		(60.0 * 60.0 * 24.0)

#if _MSC_VER > 1200 // Later than Visual Studio 6.0
#define EXPORT comment(linker, "/EXPORT:"__FUNCTION__"="__FUNCDNAME__)
#else // else need to use DEF file or __declspec(dllexport)
#define EXPORT
#endif

//==========================================================================
// Returns the time of day rounded down to the nearest second as number of
// seconds since the start of day.
//==========================================================================
long current_system_time(void)
{
	time_t time_t_T;
	struct tm tm_T;

	time(&time_t_T);
	tm_T = *localtime(&time_t_T);

	return tm_T.tm_sec + 60 * (tm_T.tm_min + 60 * tm_T.tm_hour);
}

//==========================================================================
// Returns the time of day rounded down to the nearest second as a fraction
// of 1 day, i.e. compatible with Excel time formatting.
//
// Wraps the function long current_system_time(void) providing a trigger
// for Excel calling and using the standard calling convention for Win32 DLLs.
//==========================================================================
double __stdcall get_system_time_C(long trigger)
{
#pragma EXPORT

	return current_system_time() / SECS_PER_DAY;
}

//==========================================================================
// Returns the time of day to the nearest 100th of a second as a fraction
// of 1 day, i.e. compatible with Excel time formatting.
// 
// Takes one argument which is ignored but provides a means of triggering a
// call to this function by Excel.
//
// On first call sets up the initial time using standard library routines
// prototyped in <time.h> which provide system time rounded down to the
// nearset second.  Improves and maintains time to the nearest 100th of a
// second using the C run-time clock() function that provides a meassure
// to a supposed accuracy of CLOCKS_PER_SEC (1000) but is only reliable when
// rounded to the nearest 100th of a second.
//==========================================================================

 // round to the nearest 100th second// CLOCKS_PER_SEC #defined in <time.h>
#define CLOCKS_PER_100TH_SEC (CLOCKS_PER_SEC / 100)

double __stdcall get_time_C(short trigger)
{
#pragma EXPORT

	static bool first_call = true;
	static long initial_100ths;
	static double initial_time;

	if(first_call)
	{
		long T, T_last = current_system_time();

		first_call = false; // do this part only once

// Wait till the second changes, so no fractional second
		while((T = current_system_time()) == T_last);

// Round to the nearest 100th second
		initial_100ths = (clock() + 5) / CLOCKS_PER_100TH_SEC;
		return initial_time = (T / SECS_PER_DAY);
	}
	return initial_time + ((clock() + 5) / CLOCKS_PER_100TH_SEC - initial_100ths) / (SECS_PER_DAY * 100.0);
}

//==========================================================================
// Wraps the C run-time function long clock(void) providing a trigger
// for Excel calling and using the standard calling convention for Win32 DLLs.
//==========================================================================
long __stdcall get_clock(long trigger)
{
#pragma EXPORT

	return clock();
}