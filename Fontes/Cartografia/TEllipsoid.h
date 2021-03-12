//---------------------------------------------------------------------------
#ifndef TEllipsoidH
#define TEllipsoidH

//---------------------------------------------------------------------------
#include <Classes.hpp>
//#include <string.h>

//---------------------------------------------------------------------------
class TEllipsoid
	{
	public:
		 TEllipsoid(void);
		 TEllipsoid(int Id, char* name, double radius, double ecc);
		~TEllipsoid(void);

	public:	//dados
		int    id;
		char*  ellipsoidName;
		double EquatorialRadius;
		double eccentricitySquared;
	};

//---------------------------------------------------------------------------
#endif
//eof

