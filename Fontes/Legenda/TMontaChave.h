//---------------------------------------------------------------------------
#ifndef TMontaChaveH
#define TMontaChaveH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TMontaLigacao.h"

//---------------------------------------------------------------------------
class VTChave;

//---------------------------------------------------------------------------
class TMontaChave : public TMontaLigacao
   {
   public:
                 __fastcall TMontaChave(VTApl *apl);
                 __fastcall ~TMontaChave(void);
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
                
   private:
      void __fastcall ValorCorrenteNominal(void);
      void __fastcall ValorEstado(void);
      void __fastcall ValorTipoDaChave(void);

   private: //objetos externos
      VTChave *chave;
   };

//---------------------------------------------------------------------------
#endif
//eof
