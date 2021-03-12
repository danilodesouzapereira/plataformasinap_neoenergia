//---------------------------------------------------------------------------
#ifndef TMontaReducaoH
#define TMontaReducaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TAue.h"
#include "TMontaLigacao.h"

//---------------------------------------------------------------------------
class VTReducao;

//---------------------------------------------------------------------------
class TMontaReducao : public TMontaLigacao
   {
   public:
                 __fastcall TMontaReducao(VTApl *apl);
                 __fastcall ~TMontaReducao(void);
      VTGrupo*   __fastcall IniciaGrupo(VTEqpto *eqpto);
      bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisValores(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      //bool       __fastcall LisValoresCurva(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisValoresFluxo(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano);
      AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);
                
   protected:
      bool __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ind_bar, int ano);

   private: //objetos externos
      VTReducao *reducao;
      TList     *lisEXT;

   private: //dados locais
      TAue aue;
      int  ano;
      int  num_pat;
   };

//---------------------------------------------------------------------------
#endif
//eof
