//---------------------------------------------------------------------------
#ifndef VTEstiloLinkH
#define VTEstiloLinkH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TEstilo.h"
//---------------------------------------------------------------------------
class VTEstiloLink : public TEstilo
   {
   public:  //property
	  __property int        BordaTipo      		= {read=bordaTipo,        	write=bordaTipo};
	  __property int        BordaEspessura 		= {read=bordaEspessura,   	write=bordaEspessura};
	  __property TColor     Bordacor	   		= {read=bordaCor,   		write=bordaCor};
	  
   public:
				   __fastcall  VTEstiloLink(void){};
   virtual         __fastcall ~VTEstiloLink(void){};
//   virtual bool    __fastcall  IsEqual(VTEstilo &ref) = 0;

   protected:  //dados acessados via property
		int 	bordaTipo;
		int 	bordaEspessura;
		TColor 	bordaCor;

   };

#endif
//---------------------------------------------------------------------------
//eof

