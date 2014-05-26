#ifndef __CZONE_H
#define __CZONE_H

#ifdef WIN32
#include <windows.h>
#endif

#include <math.h>

class CZone2D
{
public:
	float fMinX, fMinY, fMaxX, fMaxY;

	CZone2D ()
	{
		this->fMinX = 0.0f;
		this->fMinY = 0.0f;
		this->fMaxX = 0.0f;
		this->fMaxY = 0.0f;
	};
	
	CZone2D ( float fMinX, float fMinY, float fMaxX, float fMaxY) 
	{ 
		this->fMinX = fMinX;
		this->fMinY = fMinY;
		this->fMaxX = fMaxX;
		this->fMaxY = fMaxY;
	};
};

#endif