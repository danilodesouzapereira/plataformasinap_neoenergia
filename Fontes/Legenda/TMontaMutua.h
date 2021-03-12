//---------------------------------------------------------------------------
#ifndef TMontaMutuaH
#define TMontaMutuaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TMontaEqpto.h"

//---------------------------------------------------------------------------
class VTMutua;

//---------------------------------------------------------------------------
class TMontaMutua : public TMontaEqpto
   {
   public:
                 __fastcall TMontaMutua(VTApl *apl);
                 __fastcall ~TMontaMutua(void);
      VTGrupo*   __fastcall IniciaGrupo(VTEqpto *eqpto);
      bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      //bool       __fastcall LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      //bool       __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano);
      AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);

   private:
      void __fastcall ValorCodigoTrechos(void);
      void __fastcall ValorImpedZ0(void);
      void __fastcall ValorImpedZ1(void);

   private: //objetos externos
      VTMutua *mutua;

   private: //dados locais
   };

//---------------------------------------------------------------------------
#endif
//eof
