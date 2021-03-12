//---------------------------------------------------------------------------
#ifndef TAjustaTrifasicoH
#define TAjustaTrifasicoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TAjusta.h"
#include "..\Constante\Fases.h"

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTCarga;
class VTCurva;
class VTElo;
class VTFlow;
class VTFornecimento;
class VTLigacao;
class VTDemanda;
class VTPatamar;
class VTPatamares;
class VTProgresso;
class VTPrimario;
class VTRedes;
class VTMedicao;
class VTMedicoes;

//---------------------------------------------------------------------------
class TAjustaTrifasico : public TAjusta
   {
   public:
           __fastcall  TAjustaTrifasico(VTApl *apl_owner);
           __fastcall ~TAjustaTrifasico(void);

	private: //métodos
		double __fastcall AjustePos(VTMedicao *medicao, TList *lisCargaJusante, VTElo *elo);
		double __fastcall AjustePre(VTMedicao *medicao, TList *lisCargaJusante, TList *lisCapJusante);
		double __fastcall CalculaDemandaCapacitor(TList *lisCAP, int ind_pat);
		double __fastcall CalculaDemandaCarga(TList *lisCAR, int ind_pat);
		double __fastcall CalculaDemandaEntrada(int ind_bar_ref, VTElo *elo, int ind_pat);
		double __fastcall CalculaDemandaGerador(TList *lisGER, int ind_pat);
		double __fastcall CalculaDemandaMedicao(VTMedicao *medicao, VTPatamar *patamar);
		double __fastcall CalculaDemandaReativaCarga(TList *lisCAR, int ind_pat);
		double __fastcall CalculaDemandaReativaEntrada(int ind_bar_ref, VTElo *elo, int ind_pat);
		double __fastcall CalculaDemandaReativaGerador(TList *lisGER, int ind_pat);
	  double __fastcall CalculaDemandaReativaMedicao(VTCurva *curva, VTPatamar *patamar);
		void   __fastcall CalculaDemandasGruposAjuste(VTGrupos *grupos, int ind_pat);
        void   __fastcall CalculaFatoresGruposAjuste(VTGrupos *grupos, bool pre_fluxo);
		bool   __fastcall CorrigeDemandaCarga(VTMedicao *medicao, int ind_pat, double fat_mult_p, double fat_mult_q);
		bool   __fastcall CorrigeDemandaCarga(VTMedicao *medicao, int ind_pat);
		double __fastcall ExecutaAjuste(TList *lisNode, bool pos_fluxo);
		double __fastcall ExecutaAjustePosFluxo(TList *lisNode);
		double __fastcall ExecutaAjustePreFluxo(TList *lisNode);
		void   __fastcall FatorAjusteP(double *fat_P, bool pre_fluxo);
		void   __fastcall FatorAjusteQ(double *fat_Q, bool pre_fluxo);
        double __fastcall MaiorErro(double fator_max,double fatorP ,double fatorQ);
//        void   __fastcall NormalizaPesos(VTMedicao *medicao, int ind_pat);
      void   __fastcall RecalculaMaiorErro(void);
      void   __fastcall SalvaArquivoEvolucao(void);
		void   __fastcall ZeraValores(void);

	private: //objetos externos
		VTDemanda  		*demanda;
		VTFornecimento *fornecimento;
		VTMedicoes  	*medicoes;
		VTPatamares 	*patamares;

   private: //dados locais
      VTApl       *apl;
		VTFlow      *flow;
      TStringList *linhas;
		double p_car_ajus_mw, p_car_jusante_mw, p_car_fixa_mw;
      double p_med_mw, p_ger_mw, p_ent_flow_mw;
		double q_car_ajus_mvar, q_car_jusante_mvar, q_car_fixa_mvar;
      double q_med_mvar, q_ger_mvar, q_ent_flow_mvar, q_cap_mvar;
	};

//---------------------------------------------------------------------------
#endif
//eof

 