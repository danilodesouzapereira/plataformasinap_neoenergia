//---------------------------------------------------------------------------
#ifndef VTEstiloRetanguloH
#define VTEstiloRetanguloH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEstilo.h"
//---------------------------------------------------------------------------
class VTEstiloRetangulo : public TEstilo
{
   public:  //property
	  __property int        BordaTipo      		= {read=bordaTipo,        	write=bordaTipo};
	  __property int        BordaEspessura 		= {read=bordaEspessura,   	write=bordaEspessura};
	  __property TColor     Bordacor	   		= {read=bordaCor,   		write=bordaCor};
	  __property TColor     FundoCor 	     	= {read=fundoCor,   		write=fundoCor};
	  __property bool       FundoTransparente 	= {read=fundoTransparente,  write=fundoTransparente};

   public:
					   __fastcall  VTEstiloRetangulo(void){};
	   virtual         __fastcall ~VTEstiloRetangulo(void){};
//	   virtual bool    __fastcall  IsEqual(VTEstilo &ref) = 0;

   protected:  //dados acessados via property
	int 	bordaTipo;
	int 	bordaEspessura;
	TColor 	bordaCor;
	TColor 	fundoCor;
	bool 	fundoTransparente;
};

#endif
//---------------------------------------------------------------------------
//eof

