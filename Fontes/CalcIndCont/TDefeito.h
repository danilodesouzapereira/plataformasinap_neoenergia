// ---------------------------------------------------------------------------
#ifndef TDefeitoH
#define TDefeitoH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTDefeito.h"

// ---------------------------------------------------------------------------
class TDefeito : public VTDefeito
{
public:
	__fastcall TDefeito(VTBloco *blocoDef, int numPat);
	__fastcall ~TDefeito(void);
	void __fastcall Inicia(void);
	void __fastcall ReiniciaTempos(void);
	double __fastcall ContribTransf(void); // Contribuicao em % (clihSemTransf/ClihComTRansf)

private: // dados externos
	void __fastcall CriaLisEstratSoc(void);
	void __fastcall IniciaLisEstratSoc(void);
	void __fastcall TemposDefault(void);

protected: // métodos acessados via property

	VTEqpto* __fastcall PM_GetEqpto(void)
	{
		return (eqpto);
	};

	void __fastcall PM_SetEqpto(VTEqpto *eqpto)
	{
		this->eqpto = eqpto;
	};

	VTRede* __fastcall PM_GetRede(void)
	{
		return (rede);
	};

	void __fastcall PM_SetRede(VTRede *rede)
	{
		this->rede = rede;
	};

	VTBloco* __fastcall PM_GetBloco(void)
	{
		return (bloco);
	};
	void __fastcall PM_SetBloco(VTBloco *bloco);

	VTChave* __fastcall PM_GetChaveProt(void)
	{
		return (chvProt);
	};

	void __fastcall PM_SetChaveProt(VTChave *chvProt)
	{
		this->chvProt = chvProt;
	};

	VTChave* __fastcall PM_GetChaveReliga(void)
	{
		return (chvReliga);
	};

	void __fastcall PM_SetChaveReliga(VTChave *chvReliga)
	{
		this->chvReliga = chvReliga;
	};

	VTChave* __fastcall PM_GetChaveIsolaMont(void)
	{
		return (chvIsolaMont);
	};

	void __fastcall PM_SetChaveIsolaMont(VTChave *chvIsolaMont)
	{
		this->chvIsolaMont = chvIsolaMont;
	};

	VTChave* __fastcall PM_GetChaveIsolaAutoMont(void)
	{
		return (chvIsolaAutoMont);
	};

	void __fastcall PM_SetChaveIsolaAutoMont(VTChave *chvIsolaAutoMont)
	{
		this->chvIsolaAutoMont = chvIsolaAutoMont;
	};

	TList* __fastcall PM_GetLisProt(void)
	{
		return (lisProt);
	};

	TList* __fastcall PM_GetLisChvMon(void)
	{
		return (lisChvMon);
	};

	TList* __fastcall PM_GetLisChvJus(void)
	{
		return (lisChvJus);
	};

	TList* __fastcall PM_GetLisBlocosMan(void)
	{
		return (lisBlocosMan);
	};

	TList* __fastcall PM_GetLisChavesBlocosMan(void)
	{
		return (lisChavesBlocosMan);
	};

	TList* __fastcall PM_GetLisBlocosManVis(void)
	{
		return (lisBlocoManVis);
	};

	TList* __fastcall PM_GetLisBlocosVis(void)
	{
		return (lisBlocosVis);
	};

	TList* __fastcall PM_GetLisChvAlivioVis(void)
	{
		return (lisChvAlivioVis);
	};

	TList* __fastcall PM_GetLisChvBlocoVis(void)
	{
		return (lisChvBlocoVis);
	};
	TList* __fastcall PM_GetLisChvVisDef(void); // {return(lisChvVisDef);};

	TList* __fastcall PM_GetLisVisRedeDef(void)
	{
		return (lisVisRedeDef);
	};

	TList* __fastcall PM_GetLisBlocoRadJusDef(void)
	{
		return (lisBlocoRadJusDef);
	};

	TList* __fastcall PM_GetLisBlocosJusProt(void)
	{
		return (lisBlocosJusProt);
	};

	TList* __fastcall PM_GetLisBlocosJusDef(void)
	{
		return (lisBlocosJusDef);
	};

	TList* __fastcall PM_GetLisBlocosProtDef(void)
	{
		return (lisBlocosProtDef);
	};

	TList* __fastcall PM_GetLisBlocosJusAuto(void)
	{
		return (lisBlocosJusAuto);
	};

	TList* __fastcall PM_GetLisBlocosProtAuto(void)
	{
		return (lisBlocosProtAuto);
	};

	TList* __fastcall PM_GetLisBlocosAutoDef(void)
	{
		return (lisBlocosAutoDef);
	};

	TList* __fastcall PM_GetLisBlocosAgrega(void)
	{
		return (lisBlocosAgreg);
	};

	TList* __fastcall PM_GetLisLigaVis(void)
	{
		return (lisLigaVis);
	};

	TList* __fastcall PM_GetLisLigaVisDef(void)
	{
		return (lisLigaVisDef);
	};

	TDateTime __fastcall PM_GetHSist(void)
	{
		return (hsist);
	};

	void __fastcall PM_SetHSist(TDateTime hsist)
	{
		this->hsist = hsist;
	};

	TDateTime __fastcall PM_GetHpreDef(void)
	{
		return (hpreDef);
	};

	void __fastcall PM_SetHpreDef(TDateTime hpreDef)
	{
		this->hpreDef = hpreDef;
	};

	TDateTime __fastcall PM_GetHfimDef(void)
	{
		return (hfimDef);
	};

	void __fastcall PM_SetHfimDef(TDateTime hfimDef)
	{
		this->hfimDef = hfimDef;
	};

	double __fastcall PM_GetDuraRep(void)
	{
		return (durarep);
	};

	void __fastcall PM_SetDuraRep(double durarep)
	{
		this->durarep = durarep;
	};

	double __fastcall PM_GetDuraPreFalta(void)
	{
		return (duraPreFalta);
	};

	void __fastcall PM_SetDuraPreFalta(double duraPreFalta)
	{
		this->duraPreFalta = duraPreFalta;
	};

	int __fastcall PM_GetIPat(void)
	{
		return (iPat);
	};

	void __fastcall PM_SetIPat(int iPat)
	{
		this->iPat = iPat;
	};

	double __fastcall PM_GetFC(void)
	{
		return (fc);
	};

	void __fastcall PM_SetFC(double fc)
	{
		this->fc = fc;
	};

	int __fastcall PM_GetIniPat(void)
	{
		return (iniPat);
	};

	void __fastcall PM_SetIniPat(int iniPat)
	{
		this->iniPat = iniPat;
	};

	int __fastcall PM_GetFimPat(void)
	{
		return (fimPat);
	};

	void __fastcall PM_SetFimPat(int fimPat)
	{
		this->fimPat = fimPat;
	};
	VTEstratSoc* __fastcall PM_GetEstratSoc(int np);

	double __fastcall PM_GetNFalhas(void)
	{
		return (nfalhas);
	};

	void __fastcall PM_SetNFalhas(double nfalhas)
	{
		this->nfalhas = nfalhas;
	};

	double __fastcall PM_GetNPisca(void)
	{
		return (npisca);
	};

	void __fastcall PM_SetNPisca(double npisca)
	{
		this->npisca = npisca;
	};

	double __fastcall PM_GetTReparo(void)
	{
		return (treparo);
	};

	void __fastcall PM_SetTReparo(double treparo)
	{
		this->treparo = treparo;
	};

	double __fastcall PM_GetTReparoTotal(void)
	{
		return (treparoTotal);
	};

	void __fastcall PM_SetTReparoTotal(double treparoTotal)
	{
		this->treparoTotal = treparoTotal;
	};

	double __fastcall PM_GetTaxaFalha(void)
	{
		return (taxaFalha);
	};

	void __fastcall PM_SetTaxaFalha(double taxaFalha)
	{
		this->taxaFalha = taxaFalha;
	};

	double __fastcall PM_GetComprimentoBloco(void)
	{
		return (comprimentoBloco);
	};

	void __fastcall PM_SetComprimentoBloco(double comprimentoBloco)
	{
		this->comprimentoBloco = comprimentoBloco;
	};

	double __fastcall PM_GetDCons(void)
	{
		return (dcons);
	};

	void __fastcall PM_SetDCons(double dcons)
	{
		this->dcons = dcons;
	};

	double __fastcall PM_GetTCons(void)
	{
		return (tcons);
	};

	void __fastcall PM_SetTCons(double tcons)
	{
		this->tcons = tcons;
	};

	double __fastcall PM_GetNmanAuto(void)
	{
		return (nmanAuto);
	}; // hkedp int->double

	void __fastcall PM_SetNmanAuto(double nmanAuto)
	{
		this->nmanAuto = nmanAuto;
	}; // hkedp int->double

	double __fastcall PM_GetNmanMan(void)
	{
		return (nmanMan);
	}; // hkedp int->double

	void __fastcall PM_SetNmanMan(double nmanMan)
	{
		this->nmanMan = nmanMan;
	}; // hkedp int->double

	bool __fastcall PM_GetFlagCortaVis(void)
	{
		return (flagCortaVis);
	};

	void __fastcall PM_SetFlagCortaVis(bool flagCortaVis)
	{
		this->flagCortaVis = flagCortaVis;
	};

	double __fastcall PM_GetFatTMA(void)
	{
		return (fatTMA);
	}; // hk201703: adaptações para planTurmas

	void __fastcall PM_SetFatTMA(double fatTMA)
	{
		this->fatTMA = fatTMA;
	}; // hk201703: adaptações para planTurmas
     //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
	  bool         __fastcall PM_GetFlagTPre(void);
	  double       __fastcall PM_GetPtpre(void) {return(ptpre);};
	  void         __fastcall PM_SetPtpre(double ptpre) {this->ptpre = ptpre;};
	  double       __fastcall PM_GetPtdlo(void) {return(ptdlo);};
	  void         __fastcall PM_SetPtdlo(double ptdlo) {this->ptdlo = ptdlo;};
	  double       __fastcall PM_GetPtexe(void) {return(ptexe);};
	  void         __fastcall PM_SetPtexe(double ptexe) {this->ptexe = ptexe;};
     //hk201710IF: Informações de identificadores de falta, IF
	  strIF*       __fastcall PM_GetInfIF(void) {return(infIF);};
	  void         __fastcall PM_SetInfIF(strIF *infIF) {this->infIF = infIF;};
	  double       __fastcall PM_GetFatIFDlo(void) {return(fatIFDlo);};
	  void         __fastcall PM_SetFatIFDlo(double fatIFDlo) {this->fatIFDlo = fatIFDlo;};
	  double       __fastcall PM_GetFatIFPre(void) {return(fatIFPre);};
	  void         __fastcall PM_SetFatIFPre(double fatIFPre) {this->fatIFPre = fatIFPre;};
	// TList*       __fastcall PM_GetLisJusVisSoc(void) {return(lisJusVisSoc);};
	// TList*       __fastcall PM_GetLisBlocosSoc(int np) {return(lisBlocosSoc);};
	// TList*       __fastcall PM_GetLisVisRedeSoc(int np) {return(lisVisRedeSoc);};
	// double       __fastcall PM_GetCargaTransf(int np) {return(cargaTransf);};
	// void         __fastcall PM_SetCargaTransf(int np, double cargaTransf) {this->cargaTransf = cargaTransf;};
	// double       __fastcall PM_GetCapacTransf(int np) {return(capacTransf);};
	// void         __fastcall PM_SetCapacTransf(int np, double capacTransf) {this->capacTransf = capacTransf;};

private: // dados locais

	// Localizacao do defeito
	VTEqpto *eqpto; // Eqpto em falha apontado pelo usuário
	VTRede *rede;
	VTBloco *bloco; // bloco em falha
	TList *lisBlocosAgreg; // lista de blocos agregados por chave não implementada

	VTChave *chvProt; // Chave de protecao que atuou
	VTChave *chvReliga;
	// 1a Chave religadora a montante da chave que atuou, caso chvProt não seja religadora
	VTChave *chvIsolaMont; // chave de isolamento a montante do defeito
	VTChave *chvIsolaAutoMont; // chave de isolamento Automática a montante do defeito

	TList *lisProt; // lista de chaves de proteção a montante do defeito
	TList *lisChvMon; // lista de chaves a montante do bloco de defeito
	TList *lisChvJus; // lista de chaves a jusante do bloco de defeito
	TList *lisBlocosMan; // lista de blocos manobráveis na rede do defeito
	TList *lisChavesBlocosMan;
	// lista de chaves internas a lisBlocosMan, com possibilidade de manobra

	TList *lisBlocoManVis; // lista de blocos manobráveis nas redes de vis
	TList *lisBlocosVis; // lista de blocos troncais em funcao do defeito das redes VIS
	TList *lisChvAlivioVis;
	TList *lisChvBlocoVis;

	TList *lisChvVisDef; // lista de chaves vis do defeito
	TList *lisVisRedeDef; // lista de objtos visRede do defeito
	TList *lisBlocoRadJusDef; // lista de blocoRad manobráveis a jusante do bloco de defeito

	TList *lisBlocosJusProt; // lista de blocos a justante da chave de protecao
	TList *lisBlocosJusAuto;
	// lista de blocos a justante da chave de automática de isolamento a montante
	TList *lisBlocosJusDef; // lista de blocos a justante do bloco de defeito

	TList *lisBlocosProtDef; // lista de blocos entre chave de protecao e bloco de defeito
	TList *lisBlocosProtAuto;
	// lista de blocos entre a chave de protecao e chave automática de isolamento a montante

	TList *lisBlocosAutoDef;
	// lista de blocos entre a chave automática de isolamento a montante bloco de defeito
	TList *lisLigaVis;
	TList *lisLigaVisDef;

	// Estrategias de socorro
	TList *lisEstratSoc; // lista de estratégias de socorro por patamar de demanda: TEstratSoc[npat]
	// TList     *lisBlocosSoc;        //lista de blocos a justante socorridos
	// TList     *lisVisRedeSoc;       //lista de visRede atuadas
	// TList     *lisJusVisSoc;        //lista de chaves a jusante do defeito associadas ao defeito

	TDateTime hsist; // Instante iniciado sistema
	TDateTime hpreDef; // Instante préfalta
	TDateTime hfimDef; // Término do reparo
	double durarep; // duração a partir de hsist para o reparo
	double duraPreFalta; // duração entre o início do defeito e hora atual
	int iniPat, fimPat; // Periodo do início do processamento até a previsão de reparo do defeito
	int iPat; // patamar de referência para gerar estratégia de socorro
	double fc; // fator de carga para os blocos desligados entre iniPat e fimPat
	double cargaTransf, capacTransf;
	// Carga transferida a jusante e capacidade total de transferencia em Mw
	double nfalhas; // número de falhas média por ano no bloco do defeito
	double npisca; // número de piscadas por religamento média por ano no bloco do defeito
	// Indicadores do efeito dos desligamentos do defeito para duracao do desligto > 3min
	double dcons; // desliga x cons
	double tcons; // tdesligax cons
	// Tratamento do defeito
	double nmanAuto; // #manobras automáticas //hkedp int->double
	double nmanMan; // #manobras manuais     //hkedp int->double
	double treparo; // duracao do reparo em minutos
	double treparoTotal;
	// duracao do reparo total: reparo + localização+ isola a montante em minutos
	double comprimentoBloco; // comprimento do bloco
	double taxaFalha; // taxa de falhas média do bloco
       double    ptpre, ptdlo, ptexe;//hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)

	int numPat; // número de patamares para guardar estratégias
	bool flagCortaVis; // flag de self healing para corte de carga em circuitos de socorro
	// hk201703: adaptações para planTurmas
	double fatTMA;

       //hk201710IF: informações preparadas dos IF
       strIF     *infIF;
       double    fatIFDlo, fatIFPre; //Fatores de redução de tdes e tpre devido ao IF

public:

};

// ---------------------------------------------------------------------------
#endif
