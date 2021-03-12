//---------------------------------------------------------------------------
#ifndef TLigaSerieH
#define TLigaSerieH

//-----------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\TLigacao.h"

//---------------------------------------------------------------------------
class TLigaSerie : public TLigacao
   {
   public:
                 __fastcall TLigaSerie(void);
                 __fastcall ~TLigaSerie(void);
                     void __fastcall LisBarra(TList *lisEQP);
					VTLigacao* __fastcall Clone(void) {return(NULL);};
               void       __fastcall DefineObjBarraSerie(VTBarra *pbar);
					int        __fastcall Tipo(void);// {return(-1);};
               AnsiString __fastcall TipoAsString(void){return("");};
               bool       __fastcall AddEqpto(VTEqpto* eqpto);
               TList*     __fastcall LisEqpto(void);

   private:
		TList* lisEqpto;
		TList* lisBarra;
		int i_na;
	public:
		double Comprimento_km;
	};

//---------------------------------------------------------------------------
#endif
//eof




