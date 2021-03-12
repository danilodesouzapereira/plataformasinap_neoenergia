//---------------------------------------------------------------------------
#ifndef TMontaTrafoH
#define TMontaTrafoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TMontaLigacao.h"

//---------------------------------------------------------------------------
class VTTrafo;

//---------------------------------------------------------------------------
class TMontaTrafo : public TMontaLigacao
   {
   public:
                 __fastcall TMontaTrafo(VTApl *apl);
                 __fastcall ~TMontaTrafo(void);
      VTGrupo*   __fastcall IniciaGrupo(VTEqpto *eqpto);
      bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);
                
   private:     
      void __fastcall ValorImpAterramento(int pri_sec);
      void __fastcall ValorImpSequencia(void);
      void __fastcall ValorPerdaFerro(void);
      void __fastcall ValorPotNominal(void);
      void __fastcall ValorTensaoNominal(int pri_sec);
      void __fastcall ValorTipoLigacao(int pri_sec);

   private: //objetos externos
      VTTrafo *trafo;

   private: //dados locais
      enum enumPRI_SEC {PRIMARIO=1, SECUNDARIO=2};
   };

//---------------------------------------------------------------------------
#endif
//eof
