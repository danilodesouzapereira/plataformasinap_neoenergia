//---------------------------------------------------------------------------
#ifndef TMontaReguladorH
#define TMontaReguladorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TMontaLigacao.h"

//---------------------------------------------------------------------------
class VTRegulador;

//---------------------------------------------------------------------------
class TMontaRegulador : public TMontaLigacao
   {
   public:
                 __fastcall TMontaRegulador(VTApl *apl);
                 __fastcall ~TMontaRegulador(void);
      VTGrupo*   __fastcall IniciaGrupo(VTEqpto *eqpto);
      bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);
                
   private:
	  void __fastcall ValorAutCfgTap(TList *lisEXTdados, int num_pat);
	  void __fastcall ValorConfiguracao(int num_pat);
      void __fastcall ValorImpAterramento(int pri_sec);
      void __fastcall ValorImpSequencia(void);
      void __fastcall ValorPotNominal(void);
	  void __fastcall ValorTipoLigacao(void);

   private: //objetos externos
      VTRegulador *regulador;

   private: //dados locais
   };

//---------------------------------------------------------------------------
#endif
//eof
