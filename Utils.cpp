/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Utils.cpp
	- Author(s): OrMisicL

  =========================================*/

#include "main.h"

// Linux GetTickCount
#ifndef _WIN32

#include "sys/time.h"
timeval start_time;
timeval now_time;

void TickCountLoad()
{
	// Get the starting time
	gettimeofday(&start_time, 0);
}

int GetTickCount()
{
	// Get the time elapsed since the start
	gettimeofday(&now_time,0);
	return (now_time.tv_usec - start_time.tv_usec) / 1000 + 1000 * (now_time.tv_sec - start_time.tv_sec);
}

#endif
