//---------------------------------------------------------------------------
#ifndef TMontaReatorH
#define TMontaReatorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TAue.h"
#include "TMontaEqbar.h"

//---------------------------------------------------------------------------
class VTReator;

//---------------------------------------------------------------------------
class TMontaReator : public TMontaEqbar
   {
   public:
                 __fastcall TMontaReator(VTApl *apl);
                 __fastcall ~TMontaReator(void);
      VTGrupo*   __fastcall IniciaGrupo(VTEqpto *eqpto);
      bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisValores(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisValoresCurva(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisValoresFluxo(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano);
      AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);
                
   private:
      void __fastcall ValorTipoLigacao(void);
      void __fastcall ValorPotenciaReativa(void);
      void __fastcall VetValorPotReativa(int ind_atrib);

   private: //objetos externos
      VTApl    *apl;
      VTReator *reator;
   };

//---------------------------------------------------------------------------
#endif
//eof
