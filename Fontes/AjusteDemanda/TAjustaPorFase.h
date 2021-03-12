//---------------------------------------------------------------------------
#ifndef TAjustaPorFaseH
#define TAjustaPorFaseH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TAjusta.h"
//---------------------------------------------------------------------------
class VTBarra;
class VTDemanda;
class VTElo;
class VTFlow;
class VTFornecimento;
class VTLigacao;
class VTMedicao;
class VTPatamar;
class VTPatamares;
class VTProgresso;
class VTRedes;
//---------------------------------------------------------------------------
class TAjustaPorFase : public TAjusta
   {
   public:
           __fastcall  TAjustaPorFase(VTApl *apl);
           __fastcall ~TAjustaPorFase(void);

	private: //métodos
		double __fastcall AjustePos(VTMedicao *medicao, TList *lisCargaJusante, VTElo *elo);
		double __fastcall AjustePre(VTMedicao *medicao, TList *lisCargaJusante, TList *lisCapJusante);
		void   __fastcall CalculaDemandaCargaFase(TList *lisCAR, int ind_pat, double p_mw[MAX_FASE]);
		void   __fastcall CalculaDemandaCapacitorFase(TList *lisCAP, int ind_pat, double q_mvar[MAX_FASE]);
		void   __fastcall CalculaDemandaEntradaFase(int ind_bar_ref, VTElo *elo, int ind_pat, double p_ent_flow_mw[MAX_FASE]);
      void   __fastcall CalculaDemandaGeradorFase(TList *lisGER, int ind_pat, double p_mw[MAX_FASE]);
		void   __fastcall CalculaDemandaMedicaoFase(VTCurva *curva, VTPatamar *patamar, double p_med_mw[MAX_FASE]);
		void   __fastcall CalculaDemandaReativaCargaFase(TList *lisCAR, int ind_pat, double q_mvar[MAX_FASE]);
		void   __fastcall CalculaDemandaReativaEntradaFase(int ind_bar_ref, VTElo *elo, int ind_pat, double q_ent_flow_mvar[MAX_FASE]);
		void   __fastcall CalculaDemandaReativaGeradorFase(TList *lisGER, int ind_pat, double q_mvar[MAX_FASE]);
		void   __fastcall CalculaDemandaReativaMedicaoFase(VTCurva *curva, VTPatamar *patamar, double q_med_mvar[MAX_FASE]);
		void   __fastcall CalculaDemandasGruposAjuste(VTGrupos *grupos, int ind_pat);
		void   __fastcall CalculaFatoresGruposAjuste(VTGrupos *grupos, bool pre_fluxo);
		bool   __fastcall CorrigeDemandaCarga(TList *lisCARGA, int ind_pat, double fat_mult_p[MAX_FASE], double fat_mult_q[MAX_FASE]);
		bool   __fastcall CorrigeDemandaCarga(VTMedicao *medicao, int ind_pat);
		bool   __fastcall DemandaNula(double vetor[MAX_FASE]);
		double __fastcall ExecutaAjuste(TList *lisNode, bool pos_fluxo);
		double __fastcall ExecutaAjustePosFluxo(TList *lisNode);
		double __fastcall ExecutaAjustePreFluxo(TList *lisNode);
		void   __fastcall FatorAjusteP(double fat_P[MAX_FASE], bool pre_fluxo);
		void   __fastcall FatorAjusteQ(double fat_Q[MAX_FASE], bool pre_fluxo);
      bool   __fastcall FatorNegativo(double fat_mult_p[MAX_FASE]);
		double __fastcall MaiorFator(double fator[MAX_FASE]);
		double __fastcall MaiorErro(double fator_max,double fatorP[MAX_FASE], double fatorQ[MAX_FASE]);
	  double __fastcall MenorFator(double fator[MAX_FASE]);
      void   __fastcall RecalculaMaiorErro(VTMedicao *medicao, VTElo *elo);
		void   __fastcall SalvaArquivoEvolucao(void);
      void   __fastcall ZeraValores(void);

	private: //objetos externos
		VTDemanda   	*demanda;
		VTFornecimento *fornecimento;
		VTMedicoes  	*medicoes;
		VTPatamares 	*patamares;

   private: //dados locais
		VTApl  		*apl;
		VTFlow 		*flow;
      TStringList *linhas;
		double 		p_med_mw[MAX_FASE],      q_med_mvar[MAX_FASE];
		double      p_car_ajus_mw[MAX_FASE], q_car_ajus_mvar[MAX_FASE];
		double  		p_car_fixa_mw[MAX_FASE], q_car_fixa_mvar[MAX_FASE];
		double  		p_car_jusante_mw[MAX_FASE], q_car_jusante_mvar[MAX_FASE];
      double  		p_ent_flow_mw[MAX_FASE], q_ent_flow_mvar[MAX_FASE];
		double  		p_ger_mw[MAX_FASE],      q_ger_mvar[MAX_FASE];
		double      q_cap_mvar[MAX_FASE];
	};

//---------------------------------------------------------------------------
#endif
//eof

 