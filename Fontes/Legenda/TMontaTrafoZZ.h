//---------------------------------------------------------------------------
#ifndef TMontaTrafoZZH
#define TMontaTrafoZZH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TAue.h"
#include "TMontaEqbar.h"

//---------------------------------------------------------------------------
class VTTrafoZZ;

//---------------------------------------------------------------------------
class TMontaTrafoZZ : public TMontaEqbar
   {
   public:
                 __fastcall TMontaTrafoZZ(VTApl *apl);
                 __fastcall ~TMontaTrafoZZ(void);
      VTGrupo*   __fastcall IniciaGrupo(VTEqpto *eqpto);
      bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisValores(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisValoresCurva(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisValoresFluxo(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisVetValEqpto(VTEqpto *eqpto, TList *lisEXT, int ind_atrib);
      bool       __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano);
      AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);
                
   private:
      void __fastcall ValorPotenciaNominal(void);
      void __fastcall ValorTensaoNominal(void);
      void __fastcall VetValorFluxoCargto(int ind_atrib);

   private: //objetos externos
      VTApl       *apl;
      VTTrafoZZ   *trafoZZ;
   };

//---------------------------------------------------------------------------
#endif
//eof
