// ---------------------------------------------------------------------------
#ifndef TPreMontaH
#define TPreMontaH

// arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\Constante\Const.h>
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Rede\Estrutura.h>
#include "VTPreMonta.h"

// ---------------------------------------------------------------------------
class TArranjo;
class TBarraBDGD;
class TBarramento;
class TCapacitorBDGD;
class TCargaBDGD;
class TChaveBDGD;
class TGeradorBDGD;
class TRedeBT;
class TRedeMT;
class TReguladorMT;
class TTipoUnidade;
class TTrechoBDGD;
class TSubestacao;
class TSe;
class TPri;
class TSec;
class TTrafoSubestacao;
class VTArranjo;
class VTArranjos;
class VTBarra;
class VTCabo;
class VTCarga;
class VTChave;
class VTClasses;
class VTDominio;

class VTFases;
class VTLog;
class VTMonta;
class VTPatamares;
class VTSuporte;
class VTSuprimento;
class VTTipos;
struct strIMP;
class VTScg;

class TLogThread;
class TLogManager;

// ---------------------------------------------------------------------------
class TPreMonta : public VTPreMonta
{
public:
	__fastcall TPreMonta(VTApl *apl);
	__fastcall ~TPreMonta(void);
	//
	// controle de barras
	void __fastcall InsereBarraStart(void);
	void __fastcall InsereBarraStop(void);
	virtual bool __fastcall InsereCfg_Arranjo(TArranjoBDGD *arranjoBDGD);
	virtual bool __fastcall InsereCfg_TipoChave(TTipoUnidade* tipo_ch);
	// etd
	virtual bool __fastcall InsereEtd_Barra(TBarramento *barramento);
	virtual bool __fastcall InsereEtd_Rede(TSubestacao *subestacao);
	virtual bool __fastcall InsereEtd_Trafo(TTrafoSubestacao *trafo_sub);
	// pri
	virtual bool __fastcall InserePri_Barra(TBarraBDGD *barraBDGD);
	virtual bool __fastcall InserePri_Capacitor(TCapacitorBDGD *cap);
	virtual bool __fastcall InserePri_Chave(TChaveBDGD *chaveBDGD);
	virtual bool __fastcall InserePri_Consumidor(TCargaBDGD *cargaMT);
	virtual bool __fastcall InserePri_Gerador(TGeradorBDGD *geradorMT);
	virtual bool __fastcall InserePri_Rede(TRedeMT *primario);
	virtual bool __fastcall InserePri_Regulador(TReguladorMT *reg);
	virtual bool __fastcall InserePri_Trecho(TTrechoBDGD *trechoBDGD);

	//sec
	// EM CADA PRE-MONTA

  	virtual bool __fastcall Sucesso(void);
	virtual bool __fastcall TrataInicioDeDados(void);
	virtual bool __fastcall TrataFimDeDados(void);

protected: // métodos
//	void __fastcall AtualizaCargaEquivalente(TEt *et, VTCarga *carga);
	VTArranjo* __fastcall ArranjoBTcerto(int fases_tag);
	VTArranjo* __fastcall ArranjoMTcerto(int fases_tag);
//	VTBarra* __fastcall BarraSolta(VTChave *chave);
	int __fastcall ConverteClasseConsumidor(AnsiString sigla_classe);
    int __fastcall ConverteTipoLigacao(AnsiString codigo_lig);
	bool __fastcall CriaArranjoTipicoBT(void);
	bool __fastcall CriaArranjoTipicoMT(void);
	VTCarga* __fastcall CriaConsMTdemanda(strCARGA &str, TCargaBDGD *cargaMT);
	VTCarga* __fastcall CriaConsMTenergia(strCARGA &str);
	VTCarga* __fastcall CriaCargaConsumidorBT(TCargaBDGD *cargaBT, int barra_id);
	VTCarga* __fastcall CriaCargaConsumidorIP(TCargaIP *cargaIP, int barra_id);
	TColor __fastcall DefineCorRedePrimaria(TRedeMT *primario);
	VTArranjo* __fastcall ExisteArranjo(int arranjo_id, int fases_tag, bool ramal = false);
//	VTBarra*   __fastcall ExisteBarra(VTBarra *barra_ref,TList *lisBarrasIndesejaveis, VTRede *rede1= NULL, VTRede *rede2= NULL, double raio_m =1);
	int __fastcall ExisteTipoChaveSinap(int id_chave);
	TSe* __fastcall ExisteSubestacao(int se_id);
	bool __fastcall IniciaDominio(void);
	bool __fastcall InserePri_DisjuntosFicticio(TBarraBDGD *barraIni, TBarraBDGD *barraTrafo);
	VTBarra* __fastcall InsereEtd_Barra(TTrafoSubestacao *trafo_sub, int barra_id);
	bool __fastcall InsereEtd_Suprimento(TTrafoSubestacao *trafo_sub);
	bool __fastcall InsereEtd_Suprimento(VTBarra *barra);
	bool __fastcall InsereEtd_Trafo2E(TTrafoSubestacao *trafo_sub);
	bool __fastcall InsereEtd_Trafo3E(TTrafoSubestacao *trafo_sub);
	bool __fastcall InserePri_Cap_Paralelo(TCapacitorBDGD *cap);
	bool __fastcall InserePri_Cap_Serie(TCapacitorBDGD *cap);
	bool __fastcall InserePri_DisjuntorFicticio(VTBarra *barraIni, VTBarra *barraTrafo);
	bool __fastcall InsereSec_Trafo2E(TTrafoDistribuicao *trafoMTBT);
	bool __fastcall InsereSec_Trafo3E(TTrafoDistribuicao *trafoMTBT);
//	VTBarra* __fastcall InserePri_BarraFicticia(VTBarra *bar_ref);
//	bool __fastcall InserePri_DisjuntorFicticio(void);
//	VTBarra* __fastcall InsereSec_BarraTrafo(TEt *et, int barra_id);
//	void __fastcall InsereSec_CargaEquivalente(TEt *et);
	bool __fastcall IsBarraIsolada(VTBarra *barra);
	void __fastcall RemoveTrechosParalelo(void);
	bool __fastcall TraduzTipoLigacaoTrafo(TLigacaoTrafo *lig_trafo, int tipo_liga_pri, int tipo_liga_sec, int tipo_liga_ter);
	void __fastcall TrataSESemTrafo(void);

	// objetos externos
	VTApl *apl;
	VTArranjos *arranjos;
	VTClasses *classes;
	VTDominio *dominio;
	VTFases *fases;
	VTLog *plog;
	VTScg *scg;
	VTMonta *monta;
	VTPatamares *patamares;
	VTTipos *tipos;

	// defines
#define   ERRO_DE_SISTEMA   AnsiString("Erro na criação do eqpto")
#define   POSBAR  50.
#define   DXBAR   2000
#define   DYBAR   2000
#define   ARRANJO_ID 1000
#define   INIPONTA 18
#define   FIMPONTA 22
#define   IP_FATPOT 0.9

	// dados locais
	TSe *se;
	TSec *sec;
    TPri *pri;
	TList *lisEQP;
	TList *lisSE;
    VTArranjo *arranjoBT;
	VTArranjo *arranjoMT;

	struct
	{ // estrutura p/ armazenar hora inicial/final dos patamres da EDP
		strHM hm_ini;
		strHM hm_fim;
		bool ponta;
	}
	pat_edp[pat01HORA];

public:
	TLogThread *log_thread;
	TLogManager *log_manager;
	AnsiString msglog;
	const char *function;
};

#endif
// -----------------------------------------------------------------------------
// eof
