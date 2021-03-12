//---------------------------------------------------------------------------
#ifndef TMontaTrafo3EH
#define TMontaTrafo3EH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TMontaLigacao.h"

//---------------------------------------------------------------------------
class VTTrafo3E;

//---------------------------------------------------------------------------
class TMontaTrafo3E : public TMontaLigacao
   {
   public:
                 __fastcall TMontaTrafo3E(VTApl *apl);
                 __fastcall ~TMontaTrafo3E(void);
      VTGrupo*   __fastcall IniciaGrupo(VTEqpto *eqpto);
      bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);

   private:
      void __fastcall ValorImpAterramento(int pst);
      void __fastcall ValorImpSequencia(void);
      void __fastcall ValorPerdaFerro(void);
      void __fastcall ValorPotNominal(int pst);
      void __fastcall ValorTensaoNominal(int pst);
      void __fastcall ValorTipoLigacao(int pst);

   private: //objetos externos
      VTTrafo3E *trafo3E;

   private: //dados locais
      enum enumPRI_SEC {PRIMARIO=1, SECUNDARIO=2, TERCIARIO=3};
   };

//---------------------------------------------------------------------------
#endif
//eof
