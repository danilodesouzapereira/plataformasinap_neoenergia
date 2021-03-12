//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEllipsoid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TEllipsoid::TEllipsoid(void)
	{
	//nada a fazer
	}

//---------------------------------------------------------------------------
TEllipsoid::TEllipsoid(int Id, char* name, double radius, double ecc)
	{
	id                  = Id;
	ellipsoidName       = name;
	EquatorialRadius    = radius;
	eccentricitySquared = ecc;
	}

//---------------------------------------------------------------------------
TEllipsoid::~TEllipsoid(void)
	{
	//nada a fazer
	}

//---------------------------------------------------------------------------
//eof
