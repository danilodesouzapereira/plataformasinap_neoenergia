//---------------------------------------------------------------------------
#ifndef TMontaYrefH
#define TMontaYrefH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TMontaEqbar.h"

//---------------------------------------------------------------------------
class VTYref;

//---------------------------------------------------------------------------
class TMontaYref : public TMontaEqbar
   {
   public:
                 __fastcall TMontaYref(VTApl *apl);
                 __fastcall ~TMontaYref(void);
      VTGrupo*   __fastcall IniciaGrupo(VTEqpto *eqpto);
      bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      //bool       __fastcall LisValoresCurva(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      bool       __fastcall LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      bool       __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano);
      AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);

   private:
		void __fastcall ValorAdmitancia_PU100(void);
		void __fastcall ValorAdmitancia_SIEMEN(void);
      void __fastcall ValorPotAparente(void);

   private: //objetos externos
      VTApl  *apl;
      VTYref *yref;
   };

//---------------------------------------------------------------------------
#endif
//eof
