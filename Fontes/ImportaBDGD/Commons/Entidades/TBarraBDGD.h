// --------------------------------------------------------------------------------------------- bof
#ifndef TBarraBDGDH
#define TBarraBDGDH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// ------------------------------------------------------------------------------------------- class
class TBarraBDGD : public TObject
{
public: //Property
		__property int           Id        = {read=PD.id_barra,     write=PD.id_barra };
		__property AnsiString    Codigo    = {read=PD.codigo,       write=PD.codigo   };
		__property double        Latitude  = {read=PD.latitude,     write=PD.latitude };
		__property double        Longitude = {read=PD.longitude,    write=PD.longitude};
public:
	__fastcall TBarraBDGD(void);
	__fastcall ~TBarraBDGD(void);

	protected:
	// dados
	struct
	{
		int id_barra;
		AnsiString codigo;
		double latitude;
		double longitude;
	}PD;
};

// função global
TBarraBDGD* __fastcall NewObjBarraBDGD(void);

#endif

// --------------------------------------------------------------------------------------------- eof
