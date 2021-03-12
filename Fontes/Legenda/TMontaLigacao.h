//---------------------------------------------------------------------------
#ifndef TMontaLigacaoH
#define TMontaLigacaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TMontaEqpto.h"

//---------------------------------------------------------------------------
class VTLigacao;

//---------------------------------------------------------------------------
class TMontaLigacao : public TMontaEqpto
   {
   public:
                         __fastcall TMontaLigacao(VTApl *apl);
      virtual            __fastcall ~TMontaLigacao(void);
      virtual VTGrupo*   __fastcall IniciaGrupo(VTEqpto *eqpto);
      //virtual bool       __fastcall LisAtributos(VTEqpto *eqpto, TList *lisATR);
      virtual bool       __fastcall LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR);
      virtual bool       __fastcall LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR);
      //virtual bool       __fastcall LisValoresCurva(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      virtual bool       __fastcall LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      virtual bool       __fastcall LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat);
      virtual bool       __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano);
      //virtual AnsiString __fastcall ValorAtributo(VTEqpto *eqpto, int ind_atrib);

   protected:          
      bool __fastcall FluxoIndefinido(int ind_bar);
      bool __fastcall LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ind_bar, int ano);
      void __fastcall ValorCarregamento(void);
      void __fastcall ValorCodigoBarras(void);
      void __fastcall ValorCodigoRede(void);
      void __fastcall ValorCorrentePorfase(int ind_bar);
      void __fastcall ValorCorrente_max(int ind_bar);
      void __fastcall ValorCorrente_min(int ind_bar);
      void __fastcall ValorFatorPotenciaPorFase(int ind_bar);
      void __fastcall ValorFatorPotenciaTotal(int ind_bar);
      void __fastcall ValorIndiceBal(int ind_bar);
      void __fastcall ValorPerda(int ind_bar);
      void __fastcall ValorPotAparenteTotal(int ind_bar);
      void __fastcall ValorPotAparentePorFase(int ind_bar);
      void __fastcall ValorPotAparente_max(int ind_bar);
      void __fastcall ValorPotAparente_min(int ind_bar);
      void __fastcall ValorPotAtivaTotal(int ind_bar);
      void __fastcall ValorPotAtivaPorFase(int ind_bar);
      void __fastcall ValorPotAtiva_max(int ind_bar);
      void __fastcall ValorPotAtiva_min(int ind_bar);
      void __fastcall ValorPotReativaTotal(int ind_bar);
      void __fastcall ValorPotReativaPorFase(int ind_bar);
      void __fastcall ValorPotReativa_max(int ind_bar);
      void __fastcall ValorPotReativa_min(int ind_bar);
      void __fastcall VetValorCargto(int ind_atrib);
      void __fastcall VetValorFP(int ind_atrib, int ind_bar);
      void __fastcall VetValorFPf(int ind_atrib, int ind_bar);
      void __fastcall VetValorIf(int ind_atrib, int ind_bar);
      void __fastcall VetValorIf_max(int ind_atrib, int ind_bar);
      void __fastcall VetValorIf_min(int ind_atrib, int ind_bar);
      void __fastcall VetValorIndiceBal(int ind_atrib, int ind_bar);
      void __fastcall VetValorPerda(int ind_atrib);
      void __fastcall VetValorP(int ind_atrib, int ind_bar);
      void __fastcall VetValorPf(int ind_atrib, int ind_bar);
      void __fastcall VetValorPf_max(int ind_atrib, int ind_bar);
      void __fastcall VetValorPf_min(int ind_atrib, int ind_bar);
      void __fastcall VetValorQ(int ind_atrib, int ind_bar);
      void __fastcall VetValorQf(int ind_atrib, int ind_bar);
      void __fastcall VetValorQf_max(int ind_atrib, int ind_bar);
      void __fastcall VetValorQf_min(int ind_atrib, int ind_bar);
      void __fastcall VetValorS(int ind_atrib, int ind_bar);
      void __fastcall VetValorSf(int ind_atrib, int ind_bar);
      void __fastcall VetValorSf_max(int ind_atrib, int ind_bar);
      void __fastcall VetValorSf_min(int ind_atrib, int ind_bar);

   protected: //objetos externos
      VTApl     *apl;
      VTLigacao *ligacao;
   };

//---------------------------------------------------------------------------
#endif
//eof
