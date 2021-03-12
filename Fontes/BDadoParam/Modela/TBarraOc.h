//---------------------------------------------------------------------------
#ifndef TBarraOcH
#define TBarraOcH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBarraOc.h"
//---------------------------------------------------------------------------
//hk201612 class VTOcorr;
//---------------------------------------------------------------------------
class TBarraOc : public VTBarraOc
	{
	public:
							 __fastcall TBarraOc(void);
							 __fastcall ~TBarraOc(void);

	public: //dados locais

	protected:  //métodos acessados via property
		AnsiString      __fastcall PM_GetCodigo(void) {return(codigo);};
		void            __fastcall PM_SetCodigo(AnsiString codigo) {this->codigo = codigo;};
		AnsiString      __fastcall PM_GetCodigoRede(void) {return(codigoRede);};
		void            __fastcall PM_SetCodigoRede(AnsiString codigoRede) {this->codigoRede = codigoRede;};
		VTRede*         __fastcall PM_GetRedePri(void) {return(redePri);};
		void            __fastcall PM_SetRedePri(VTRede *rede) {this->redePri = rede;};
		VTEqpto*        __fastcall PM_GetEqOcorr(void) {return(eqpOcorr);};
		void            __fastcall PM_SetEqOcorr(VTEqpto *eqpto) {eqpOcorr = eqpto;};
		TObject*        __fastcall PM_GetParEqptoExt(void) {return(parEqptoExt);};
		void            __fastcall PM_SetParEqptoExt(TObject *parEqptoExt) {this->parEqptoExt = parEqptoExt;};
		TList*          __fastcall PM_GetLisTrataOcorr(void) {return(lisTrataOcorr);};
		TList*          __fastcall PM_GetLisOcorrF(void) {return(lisOcorr_f);};
		TList*          __fastcall PM_GetLisCircuitos(void) {return(lisCircuitos);};
		//int             __fastcall PM_GetNOcor(void) {return(lisOcorr_f->Count);};
		double          __fastcall PM_GetNOcorRamal(void) {return(nocorRamal);};
		void            __fastcall PM_SetNOcorRamal(double nocorRamal) {this->nocorRamal = nocorRamal;};
		double          __fastcall PM_GetNOcorNRamal(void) {return(nocorNRamal);};
		void            __fastcall PM_SetNOcorNRamal(double nocorNRamal) {this->nocorNRamal = nocorNRamal;};
		double          __fastcall PM_GetNOcorPond(void) {return(nocorPond);};
		void            __fastcall PM_SetNOcorPond(double nocorPond) {this->nocorPond = nocorPond;};
		double          __fastcall PM_GetNDiasObs(void) {return(ndiasObs);};
		void            __fastcall PM_SetNDiasObs(double ndiasObs) {this->ndiasObs = ndiasObs;};
		double          __fastcall PM_GetDuraTotal(void) {return(duraTotal);};
		void            __fastcall PM_SetDuraTotal(double duraTotal) {this->duraTotal = duraTotal;};
		double          __fastcall PM_GetCli(void) {return(cli);};
		void            __fastcall PM_SetCli(double cli) {this->cli = cli;};
		double          __fastcall PM_GetClih(void) {return(clih);};
		void            __fastcall PM_SetClih(double clih) {this->clih = clih;};
		double          __fastcall PM_GetNConsBT(void) {return(nconsBT);};
		void            __fastcall PM_SetNConsBT(double nconsBT) {this->nconsBT = nconsBT;};
		double          __fastcall PM_GetTdeslMedio(void) {return(tdeslMedio);};
		void            __fastcall PM_SetTdeslMedio(double tdeslMedio) {this->tdeslMedio = tdeslMedio;};
		VTEqptoOc*      __fastcall PM_GetEqptoOc(void) {return(eqptoOc);};
		void            __fastcall PM_SetEqptoOc(VTEqptoOc* eqptoOc) {this->eqptoOc = eqptoOc;};
		double          __fastcall PM_GetNFHora(int n) {return(nfhora[n]);};
		void            __fastcall PM_SetNFHora(int n, double nf) {nfhora[n] = nf;};
		double          __fastcall PM_GetNFMes(int n) {return(nfmes[n]);};
		void            __fastcall PM_SetNFMes(int n, double nf) {nfmes[n] = nf;};
		double          __fastcall PM_GetNFRMes(int n) {return(nfrmes[n]);};
		void            __fastcall PM_SetNFRMes(int n, double nf) {nfrmes[n] = nf;};
		double          __fastcall PM_GetCLIHMes(int n) {return(clihmes[n]);};
		void            __fastcall PM_SetCLIHMes(int n, double clih) {clihmes[n] = clih;};
		double          __fastcall PM_GetCLIMes(int n) {return(climes[n]);};
		void            __fastcall PM_SetCLIMes(int n, double cli) {climes[n] = cli;};
		double          __fastcall PM_GetTDMes(int n) {return(tdmes[n]);};
		void            __fastcall PM_SetTDMes(int n, double tdesl) {tdmes[n] = tdesl;};
		double          __fastcall PM_GetDuraTotalMes(int n) {return(durTot[n]);};
		void            __fastcall PM_SetDuraTotalMes(int n, double duraTotMes) {durTot[n] = duraTotMes;};
		double          __fastcall PM_GetX(void) {return(x);}; //hk201702 coordenadas da chave ou da carga
		void            __fastcall PM_SetX(double x) {this->x = x;};
		double          __fastcall PM_GetY(void) {return(y);};
		void            __fastcall PM_SetY(double y) {this->y = y;};
		TList*          __fastcall PM_GetLisParamCarga(void) {return(lisParamCarga);}; //hk201703: adaptacoes planTurmas
		TList*          __fastcall PM_GetLisParamBloco(void) {return(lisParamBloco);}; //hk201703: adaptacoes planTurmas
		double          __fastcall PM_GetTat(void) {return(tat);};           //hk201703: Correção do tempo de atendimento
		void            __fastcall PM_SetTat(double tat) {this->tat = tat;}; //hk201703: Correção do tempo de atendimento

		double          __fastcall PM_GetPtpre(void) {return(ptpre);};//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
		void            __fastcall PM_SetPtpre(double ptpre) {this->ptpre = ptpre;};//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
		double          __fastcall PM_GetPtdlo(void) {return(ptdlo);};//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
		void            __fastcall PM_SetPtdlo(double ptdlo) {this->ptdlo = ptdlo;};//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
		double          __fastcall PM_GetPtexe(void) {return(ptexe);};//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
		void            __fastcall PM_SetPtexe(double ptexe) {this->ptexe = ptexe;};//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)

      //hk201706: multiplas bases módulo de planejamento de turmas
      //: estatísticas por base de atendimento de tma das ocorrências e tma por simulação
		double          __fastcall PM_GetTMABase(void) {return(tmaBase);};
		void            __fastcall PM_SetTMABase(double tmaBase) {this->tmaBase = tmaBase;};
		double          __fastcall PM_GetTMAReal(void) {return(tmaReal);};
		void            __fastcall PM_SetTMAReal(double tmaReal) {this->tmaReal = tmaReal;};

	private:
//		double          __fastcall DeterminaTexe(VTOcorr *ocorr);

	private: //dados externos

	private: //dados locais
		VTEqptoOc *eqptoOc;
		VTEqpto *eqpOcorr;
		VTRede  *redePri;
		TObject *parEqptoExt;    //Parametros para cálculo de continuidade
		//Lista de ocorrencias
		TList   *lisOcorr_f;
		TList   *lisTrataOcorr;
		TList   *lisCircuitos;

		AnsiString codigo;//codigo do equipamento para busca em rede carregada
		AnsiString codigoRede;//codigo do circuito em que o equipamento esta alocado, no caso de periodos longos erros de transferencia
		double  x, y;    //hk
		double  cli, clih;
		double  duraTotal;
		double  tdeslMedio; //tempo de desligamento médio
		double  tat;//tempo de atendimento -> média dos tempos das ocorrencias no equipamento //hk201703: Correção do tempo de atendimento
		double  ndiasObs; //Número de dias de observação (lisMES->Count * 30
		double  nocorRamal; //Numero de ocorrencias em ramal da BT
		double  nocorNRamal;//Número de ocorrencias cheias em trafoBT ou na rede MT
		double  nocorPond;
		double  nconsBT; //número de consumidores do transformador de distribuição, hknova
		bool    flagEncontraRede;

		double  nfhora[24];
		double  nfmes[12], nfrmes[12], tdmes[12];
		double  climes[12], clihmes[12];
		double  durTot[12];  //Duracao [min.] por ano das falhas no equipamento localizado

      double  ptpre;//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
      double  ptdlo;//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)
      double  ptexe;//hk201705: acréscimo de percentuais do tat (prep, desl+loc, exe)

		TList   *lisParamCarga; //hk201703: adaptacoes planTurmas
		TList   *lisParamBloco; //hk201703: adaptacoes planTurmas

      //hk201706: multiplas bases módulo de planejamento de turmas
      double  tmaReal, tmaBase; //estatísticas por base de atendimento de tma das ocorrências e tma por simulação
	};

#endif
//---------------------------------------------------------------------------
