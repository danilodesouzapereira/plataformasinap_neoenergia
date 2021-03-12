//---------------------------------------------------------------------------
#ifndef TMontaEqbarH
#define TMontaEqbarH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TMontaEqpto.h"

//---------------------------------------------------------------------------
class VTEqbar;

//---------------------------------------------------------------------------
class TMontaEqbar : public TMontaEqpto
   {
   public:
                         __fastcall TMontaEqbar(void);
      virtual            __fastcall ~TMontaEqbar(void);
      //virtual bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR);
      virtual bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      virtual bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR);
      //virtual bool       __fastcall LisValoresCurva(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      virtual bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      virtual bool       __fastcall LisVetValEqpto(VTEqpto *eqpto, TList *lisEXT, int ind_atrib);
      virtual bool       __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano=0);
      //virtual AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);

   private:              
      void __fastcall ValorCodigoBarra(void);
                       
   protected: //objetos externos
      VTEqbar *eqbar;
   };

//---------------------------------------------------------------------------
#endif
//eof
