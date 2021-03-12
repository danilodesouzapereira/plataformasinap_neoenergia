//---------------------------------------------------------------------------
#ifndef TAjustaH
#define TAjustaH
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include <Fontes\Constante\Const.h>
#include <Fontes\Constante\Fases.h>
#include "VTAjusta.h"

//---------------------------------------------------------------------------
class VTApl;
class VTBloco;
class VTChave;
class VTCarga;
class VTCanal;
class VTCurva;
class VTElo;
class VTFlow;
class VTMedicao;
class VTMedidor;
class VTMedicoes;
class VTNode;
class VTObra;
class VTPatamar;
class VTPrimario;
class VTMedicao;
class VTAjusteFator;
class VTGerenciaGrupos;
class VTGrupos;
//---------------------------------------------------------------------------
class TAjusta : public VTAjusta
   {
   public:
           __fastcall  TAjusta(VTApl *apl);
           __fastcall ~TAjusta(void);
      bool __fastcall  CalculaFluxo(bool prog_enabled);
      bool __fastcall  Executa(bool med_neg_enabled=false);
	  bool __fastcall  Finaliza(void);
	  bool __fastcall  IniciaDados(TComponent *Owner);
	  void __fastcall  RemoveMedicoesFicticias(void);
      void __fastcall  RestauraCargasOriginais(void);
      void __fastcall  RestauraCargasRedesNaoAjustadas(void);

   protected:  //métodos a serem redefinidos pelas classes derivadas
	  virtual void   __fastcall CalculaDemandasGruposAjuste(VTGrupos *grupos, int ind_pat) = 0;
      virtual void   __fastcall CalculaFatoresGruposAjuste(VTGrupos *grupos, bool pre_fluxo) = 0;
      virtual double __fastcall ExecutaAjuste(TList *lisNode, bool pos_fluxo) = 0;

   protected: //métodos
      double     __fastcall CalculaDemandaTotal(TList *lisCAR, VTPatamar *patamar);
	  int        __fastcall CalculaPatamarDemandaMax(TList *lisCAR);
      bool       __fastcall CriaCurvaPropria(VTCarga* carga);
	  void       __fastcall DefineDiagnosticoTempo(TList *lisNODE);
      AnsiString __fastcall DiagMedicao(TList *lisMEDICAO);
      bool       __fastcall EqptoJusante(VTNode *node, TList *lisCargaJusante, int tipo);
      VTMedicao* __fastcall ExisteMedicao(VTElo *elo);
	  bool       __fastcall ExisteMedicaoHabilitada(TList *lisNODE);
	  VTRede*    __fastcall IdentificaRedeMedidor(VTMedidor *medidor);
      void       __fastcall IniciaLisEqbar(VTBloco *bloco, int tipo_eqbar, TList *lisEQB);
	  TList*     __fastcall LisRedeVal(void);
	  bool       __fastcall PM_GetPat(int ind_pat);
	  void       __fastcall PM_SetPat(int ind_pat, bool enabled);
      void       __fastcall RecalculaEnergiaCargas(void);
      bool       __fastcall RedefineCargaComCurvaPropria(TList *lisCAR);
      bool       __fastcall RedefineCargaComDemandaPorFase(VTCarga *carga);
	  void       __fastcall RedefineLisCargaMed(VTMedicao *medicao);
      void       __fastcall RestauraCargasRedesSemFluxo(void);
      void       __fastcall SeparaCargaNaoAjustavel(VTMedicao *medicao);
      bool       __fastcall ValidaDiagFluxo(VTMedicao *medicao);

   private:
	  //void 		 __fastcall AjustaMedicaoFatores(VTMedicao *medicao);
	  void 		 __fastcall AjustaMedicaoFatores(VTAjusteFator *fator);
      bool       __fastcall AjustePosFluxo(void);
	  bool       __fastcall AjustePreFluxo(void);
	  //void		 __fastcall AtualizaValorErroFicticio(VTMedicao *medicaoFic);
      bool       __fastcall AssociaBlocoMed(VTBloco *bloco, VTMedicao *medicao);
//      bool       __fastcall ConverteCurva(VTMedicao *medicao);
	  bool       __fastcall CalcularFluxo(AnsiString msg_usuario);
	  VTMedidor* __fastcall CriaMedidorAPartirFluxo(VTChave *chave);
	  void		 __fastcall CriaMedidoresAPartirFluxo(TList *lisMedidores);
	  void		 __fastcall CriaMedicoesFicticias(void);
      bool       __fastcall ContinuaMesmoComPrimariosNaoAjustaveis(TList *lisREDE_VAL);
      bool       __fastcall FundeBlocosSemMedicao(void);
      int        __fastcall IndBarraMontante(VTBloco *bloco_jus,VTBloco *bloco_mon);
	  bool       __fastcall IniciaBlocos(void);
	  void       __fastcall IniciaPat(void);
      bool       __fastcall LicencaCopel(void);
	  void       __fastcall ListaNodesACalcular(TList *lisACalcular, TList *lisCalculado, TList *lisNode);
      void       __fastcall LogaDiagnostico(void);
	  void       __fastcall LogaMedidores(void);
	  void 		 __fastcall LogaMedidor(VTMedidor *medidor);
	  //void		 __fastcall MultiplicaValoresPQ(double valores[]);
      AnsiString __fastcall MaiorErro(VTCanal *canal);
	  void		 __fastcall MultiplicaValoresPQ(double valores[], VTAjusteFator *fator);
	  bool       __fastcall NodePaiComFilhosCalculados(VTNode *node, TList *lisCalculado);                                                                                                      bool          __fastcall PreparaDados(VTMedicoes *medicoes, bool med_neg_enabled);
      void       __fastcall RetiraNodeRaiz(TList *lisNode);
      bool       __fastcall SalvaCopiaDasCargasOriginais(void);
      void       __fastcall TrataIP(TList *lisCARGA_AJUS, TList *lisCARGA_FIXA);
      bool       __fastcall ValidaBlocos(void);
      bool       __fastcall ValidaMedicao(VTElo *elo_pai, VTElo *elo);
      void       __fastcall ZeraCarga(VTCarga *carga);

   protected: //objetos externos
      VTApl  *apl;
      VTObra *obra_original;
	  TList  *lisCURVA, *lisMEDICAO , *lisMEDICAO_FIC;
	  TList  *lisFATORES;

   protected: //dados locais
      TList           *lisEQP, *lisREDE_VAL;
      TList           *lisCARGA_ESP; //cargas que não serão ajustadas pois estão em redes que não rodam fluxo
      VTObra          *obra;
      double          fatcorP[MAX_FASE], fatcorQ[MAX_FASE];
      double          q_cap_mvar[MAX_FASE];       //somatória das pot.reativa dos capacitores
      complex<double> s_car_ajus_mva[MAX_FASE];   //somatória da demanda das cargas ajustáveis
      complex<double> s_car_fixa_mva[MAX_FASE];   //somatória da demanda das cargas fixas
      complex<double> s_ent_flow_mva[MAX_FASE];   //somatória da demanda no início dos primários (calculada pelo fluxo)
	  complex<double> s_ger_mva[MAX_FASE];        //somatória do fornecimento de geração (calculada pelo fluxo)
	  bool            pat[pat01HORA]; //patamares habilitados para ajuste
	  VTGerenciaGrupos *gerGrupos; //objeto que gerencia os grupos de ajuste

   };

//---------------------------------------------------------------------------
#endif
//eof

 