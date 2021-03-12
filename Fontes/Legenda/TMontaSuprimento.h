//---------------------------------------------------------------------------
#ifndef TMontaSuprimentoH
#define TMontaSuprimentoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TAue.h"
#include "TMontaEqbar.h"

//---------------------------------------------------------------------------
class VTSuprimento;

//---------------------------------------------------------------------------
class TMontaSuprimento : public TMontaEqbar
   {
   public:
                 __fastcall TMontaSuprimento(VTApl *apl);
                 __fastcall ~TMontaSuprimento(void);
      VTGrupo*   __fastcall IniciaGrupo(VTEqpto *eqpto);
      bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR);
      bool       __fastcall LisValores(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisValoresCurva(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisValoresFluxo(VTEqpto *eqpto, TList *lisDADO, int ano, int num_pat);
      bool       __fastcall LisVetValEqpto(VTEqpto *eqpto, TList *lisEXT, int ind_atrib);
      bool       __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano);
      AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);
                
   private:
      void __fastcall ValorCapacidade(void);
      void __fastcall ValorCargtoGrafico(void);
      void __fastcall ValorCarregamento(void);
      void __fastcall ValorCorrentePorfase(void);
      void __fastcall ValorCorrente_max(void);
      void __fastcall ValorCorrente_min(void);
      void __fastcall ValorCurvaPQ_PV_VF(void);
      void __fastcall ValorFatPotenciaTotal(void);
      void __fastcall ValorFatPotenciaPorFase(void);
      void __fastcall ValorPotAparenteTotal(void);
      void __fastcall ValorPotAparentePorFase(void);
      void __fastcall ValorPotAparente_max(void);
      void __fastcall ValorPotAparente_min(void);
      void __fastcall ValorPotAtivaTotal(void);
      void __fastcall ValorPotAtivaPorFase(void);
      void __fastcall ValorPotAtiva_max(void);
      void __fastcall ValorPotAtiva_min(void);
      void __fastcall ValorPotReativaTotal(void);
      void __fastcall ValorPotReativaPorFase(void);
      void __fastcall ValorPotReativa_max(void);
      void __fastcall ValorPotReativa_min(void);
      void __fastcall ValorTipoSuprimento(void);
      void __fastcall ValorTipoSuprimentoNoFluxo(void);
      void __fastcall ValorZeqv(void);
      void __fastcall VetValorEqptoPQVF(int ind_atrib);
      void __fastcall VetValorFluxoCurvaPQ_PV_VF(int ind_atrib);
      void __fastcall VetValorFluxoCargto(int ind_atrib);
      void __fastcall VetValorFluxoFP(int ind_atrib);
      void __fastcall VetValorFluxoFPf(int ind_atrib);
      void __fastcall VetValorFluxoIf(int ind_atrib);
      void __fastcall VetValorFluxoIf_max(int ind_atrib);
      void __fastcall VetValorFluxoIf_min(int ind_atrib);
      void __fastcall VetValorFluxoP(int ind_atrib);
      void __fastcall VetValorFluxoPf(int ind_atrib);
      void __fastcall VetValorFluxoPf_max(int ind_atrib);
      void __fastcall VetValorFluxoPf_min(int ind_atrib);
      void __fastcall VetValorFluxoQ(int ind_atrib);
      void __fastcall VetValorFluxoQf(int ind_atrib);
      void __fastcall VetValorFluxoQf_max(int ind_atrib);
      void __fastcall VetValorFluxoQf_min(int ind_atrib);
      void __fastcall VetValorFluxoS(int ind_atrib);
      void __fastcall VetValorFluxoSf(int ind_atrib);
      void __fastcall VetValorFluxoSf_max(int ind_atrib);
      void __fastcall VetValorFluxoSf_min(int ind_atrib);

   private: //objetos externos
      VTApl        *apl;
      VTSuprimento *suprimento;
   };

//---------------------------------------------------------------------------
#endif
//eof
