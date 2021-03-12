//---------------------------------------------------------------------------
#ifndef TConverteH
#define TConverteH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Constante\Fases.h>

//---------------------------------------------------------------------------
class VTApl;
class VTCanal;
class VTCurva;
class VTLigacao;
class VTMedicao;
class VTMedidor;
class VTPatamar;

//---------------------------------------------------------------------------
class TConverte : public TObject
   {
   public:
		   __fastcall  TConverte(VTApl *apl);
		   __fastcall ~TConverte(void);
	  bool __fastcall  Executa(VTMedicao *medicao);
	  bool __fastcall  CanaisIparaCanalPQtrifasico(VTMedidor *medidor);
	  bool __fastcall  CanaisIparaCanaisPQfase(VTMedidor *medidor);
	  bool __fastcall  CanalP3FparaCanaisPfase(VTMedidor *medidor, VTCanal *canal_p3f);
	  bool __fastcall  CanalQ3FparaCanaisQfase(VTMedidor *medidor, VTCanal *canal_q3f);
	  bool __fastcall  CanaisI(TList *lisI,   VTMedicao *medicao);
	  bool __fastcall  CanaisP(TList *lisPQ,  VTMedicao *medicao);
	  bool __fastcall  CanaisQ(TList *lisPQ,  VTMedicao *medicao);
	  bool __fastcall  CanaisPQ(TList *lisPQ, VTMedicao *medicao);
	  bool __fastcall  CanaisS(TList *lisS,   VTMedicao *medicao);

   private: //métodos
	  bool     __fastcall CalculaCurvaDemandaPorFase(void);
	  bool     __fastcall CalculaDemandaFluxoPorFase(void);
	  bool     __fastcall CalculaDemandaMedicao1Fase(int ind_pat);
	  bool     __fastcall CalculaDemandaMedicaoPorFase(VTPatamar *patamar, TList *lisI);
	  double   __fastcall CalculaEnergiaDiariaFluxo(void);
	  bool     __fastcall CalculaFatorCorrecaoIfase(double fatcor[MAX_FASE],
											 VTPatamar *patamar, TList *lisI);
	  double   __fastcall CalculaFatorCorrecaoP3f(int ind_pat, double p_mw);
	  double   __fastcall CalculaFatorCorrecaoQ3f(int ind_pat, double q_mvar);
	  double   __fastcall CalculaFatorCorrecaoPfase(int ind_fase, int ind_pat, double p_mw);
	  double   __fastcall CalculaFatorCorrecaoQfase(int ind_fase, int ind_pat, double q_mvar);
	  double   __fastcall CalculaFatorCorrecaoS3f(int ind_pat, double s_mva);
	  double   __fastcall CalculaFatorCorrecaoSfase(VTCanal *canal_i, int ind_pat);
	  bool     __fastcall CalculaFatorCorrecaoSfase(double fatcor_i[MAX_FASE], int ind_pat);
	  double   __fastcall CalculaFluxoP(int ind_fase, int ind_pat);
	  double   __fastcall CalculaFluxoQ(int ind_fase, int ind_pat);
	  double   __fastcall CalculaFluxoP(VTCanal *canal_i, int ind_pat);
	  double   __fastcall CalculaFluxoQ(VTCanal *canal_i, int ind_pat);
	  bool     __fastcall CorrenteToP(VTCanal *canal_i, VTCanal *canal_p);
	  bool     __fastcall CorrenteToQ(VTCanal *canal_i, VTCanal *canal_q);
	  bool     __fastcall DeterminaVfn_kv(int ind_pat);
	  bool     __fastcall Energia(void);
	  bool     __fastcall FatorUnico(double fator_unico);
	  bool     __fastcall I1fase(void);
	  bool     __fastcall I3fase(TList *lisI);
	  bool     __fastcall ImaxFase(void);
	  bool     __fastcall PQfase(TList *lisPQ);
	  bool     __fastcall PQtrifasico(TList *lisPQ);
	  bool     __fastcall Pfase(TList *lisPQ);
	  bool     __fastcall Ptrifasico(TList *lisPQ);
	  bool     __fastcall Qfase(TList *lisPQ);
	  bool     __fastcall Qtrifasico(TList *lisPQ);
	  bool     __fastcall Strifasico(TList *lisS);
	  bool     __fastcall ValorCanal(TList *lisPQ, VTPatamar *patamar, double pq_mva[MAX_FASE+MAX_FASE]);
	  //bool     __fastcall ValorMedidoNulo(VTCanal *canal);

   private: //objetos externos
	  VTApl     *apl;
	  VTMedicao *medicao;

   private: //dados locais
	  #define   MAX_PONTO   6
	  #define   FP_092  0.92
	  double    vfn_kv[MAX_FASE];
	  double    s_med_mva[MAX_FASE];
	  TList     *lisEQP;
	  VTLigacao *liga_inicial;
	  VTCurva   *curva_med_original;
   };

//---------------------------------------------------------------------------
#endif
//eof

