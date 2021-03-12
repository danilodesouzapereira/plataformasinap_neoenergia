//---------------------------------------------------------------------------
#ifndef TParseListaH
#define TParseListaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTParse.h"

//---------------------------------------------------------------------------
class VTExtrator;

//---------------------------------------------------------------------------
class TParseLista : public VTParse
	{
   public:
           __fastcall  TParseLista(VTApl *apl);
			  __fastcall ~TParseLista(void);
      bool __fastcall  Executa(AnsiString arq_txt);
      bool __fastcall  Executa(AnsiString arq_lista, TStrings *lines);

   private: //dados externos
      VTApl    *apl;
   };


#endif

//---------------------------------------------------------------------------
//eof