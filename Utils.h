/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Utils.cpp
	- Author(s): OrMisicL

  =========================================*/

#ifndef UTILS_H
#define UTILS_H

// Linux
#ifndef _WIN32

int GetTickCount();
void TickCountLoad();

#endif

float GetDistance3D(CVector *vecPosition, CVector *_vecPosition);

#endif