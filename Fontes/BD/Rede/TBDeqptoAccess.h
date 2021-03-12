// ---------------------------------------------------------------------------
#ifndef TBDeqptoAccessH
#define TBDeqptoAccessH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ADODB.hpp>
#include "VTBDeqpto.h"
#include "BD_inc.h"

// ---------------------------------------------------------------------------
class TBDcurvaAccess;
class TBDfunc;
class VTAlternativa;
class VTBarra;
class VTCurva;
class VTEqbar;
class VTEqpto;
class VTLog;
class VTManobra;
class VTMonta;
class VTObra;
class VTPatamares;
class VTPath;
class VTRedes;
class VTCronometro;
class VTProgresso;
class VTLock;
class VTBasico;
struct strREDE;

// ---------------------------------------------------------------------------
// Estrutura auxiliar para geração dos identificadores sem consultar a base de dados
struct sttIndexEquip
{
	int ndxAgrupamentoId;
	int ndxDominioId;
	int ndxEquipId;
	int ndxEstudoId;
	int ndxAlternativaId;
    int ndxItemId;
	int ndxObraId;
	int ndxAcaoId;
	int ndxCorteId;
	int ndxManobraId;
	int ndxSincronismoId;
};

// ---------------------------------------------------------------------------
class TBDeqptoAccess : public VTBDeqpto
{
public:
	__fastcall TBDeqptoAccess(VTApl *apl_owner, VTSNPDBConn *bd_conn);
	__fastcall ~TBDeqptoAccess(void);
	bool __fastcall CarregaEstudo(VTEstudo *estudo);
	bool __fastcall CarregaMRedeEstudo(VTEstudo *estudo, TList *lisMREDE, TList *lisMREDE_VIZ);
	bool __fastcall CarregaRedeEstudo(VTEstudo *estudo);
	bool __fastcall CriaEstudoNovo(VTEstudo *estudo, TList *lisREDE);
	bool __fastcall LeListaCargaNL(VTEstudo *estudo, TList *lisREDE, TList* lisCargaNL,
		bool bolObra);
	bool __fastcall LeListaCorte(VTEstudo *estudo, TList *lisMREDE, TList *lisCORTE = NULL);
	bool __fastcall LeListaDominio(void);
	bool __fastcall LeListaEstudo(void);
	bool __fastcall LeListaMRede(VTEstudo *estudo, TList *lisCORTE);
	bool __fastcall LeListaRede(VTEstudo *estudo, VTDominio *dominio);
	bool __fastcall LeListaRede(VTEstudo *estudo, VTTipoRede *tiporede);
	bool __fastcall LeListaRedeVizinha(VTEstudo *estudo, VTRede *rede, TList *lisVIZ);
	int __fastcall LeQtdeCargaRede(int rede_id);
	bool __fastcall RemoveDominio(VTEstudo *estudo, VTDominio *dominio);
	bool __fastcall RemoveEstudo(VTEstudo *estudo);
	bool __fastcall RemoveEstudoRede(VTEstudo *estudo, VTRede *rede);
	bool __fastcall RemoveManobra(VTManobra* manobra);
	bool __fastcall RemoveSincronismo(VTLock* lock);
	bool __fastcall SalvaAlternativaEstudo(VTEstudo *estudo);
	bool __fastcall SalvaListaCargaNL(VTEstudo *estudo, TList* lisCargaNL, bool bolObra);
	bool __fastcall SalvaListaDominio(TList *lisDOM);
	bool __fastcall SalvaRedeEstudo(VTEstudo *estudo);
	bool __fastcall SalvaRedeEstudoComo(VTEstudo *estudo);
	bool __fastcall ValidaVersaoBaseParaGravacao(void);
	bool __fastcall ValidaVersaoBaseParaLeitura(void);
	AnsiString  __fastcall VersaoBase(void);

private:
	bool __fastcall CarregaAlternativaEstudo(VTEstudo *estudo);
	bool __fastcall CarregaEqptosRede(VTEstudo *estudo, bool bolUseRedeTmp, bool bolObra);
	bool __fastcall CarregaRedeEstudo(VTEstudo *estudo, TList *lisREDE); // OOPS Manter isso aqui?
	// bool __fastcall CarregaReducaoEstudo(VTEstudo *estudo);
	bool __fastcall CarregaReducaoEstudo(VTEstudo *estudo, TList *lisMREDE = NULL);
	bool __fastcall CopiaSchemaIni(void);
	int __fastcall FNSNP_rdAgrupamentoIdProx(bool bolLocaIndex = true, bool bolBeginNext = false);
	int __fastcall FNSNP_rdAcaoIdProx(VTEstudo *estudo, VTAlternativa* alternativa, VTObra* obra,
		bool bolLocaIndex = true, bool bolBeginNext = false);
	int __fastcall FNSNP_rdAlternativaIdProx(VTEstudo *estudo, bool bolLocaIndex = true,
		bool bolBeginNext = false);
	int __fastcall FNSNP_rdCorteIdProx(bool bolLocaIndex = true, bool bolBeginNext = false);
	int __fastcall FNSNP_rdDominioIdProx(bool bolLocaIndex = true, bool bolBeginNext = false);
	int __fastcall FNSNP_rdEstudoIdProx(bool bolLocaIndex = true, bool bolBeginNext = false);
	int __fastcall FNSNP_rdEquipIdProx(VTEstudo *estudo, bool bolLocaIndex = true,
		bool bolBeginNext = false);
	int __fastcall FNSNP_rdManobraIdProx(bool bolLocaIndex = true, bool bolBeginNext = false);
	int __fastcall FNSNP_rdObraIdProx(VTEstudo *estudo, VTAlternativa* alternativa,
		bool bolLocaIndex = true, bool bolBeginNext = false);
	int __fastcall FNSNP_rdItemIdProx(VTEstudo* estudo, VTAlternativa* alternativa, VTObra *obra,
		bool bolLocaIndex = true, bool bolBeginNext = false);
	int __fastcall FNSNP_rdRedeDominioCountDominio(VTEstudo* estudo, VTDominio* dominio);
	int __fastcall FNSNP_rdRedeDominioCountRede(VTDominio* dominio);
	int __fastcall FNSNP_rdSincronismoIdProx(bool bolLocaIndex = true, bool bolBeginNext = false);
	int __fastcall FNSNP_rdTipoEqbarCount(VTEstudo* estudo, int intRedeId, int intTipoEquipId);
	int __fastcall FNSNP_rdTipoEqligCount(VTEstudo* estudo, int intRedeId, int intTipoEquipId);
	bool __fastcall InsereCampo(AnsiString tabela, AnsiString campo, AnsiString tipo);
	bool __fastcall RenovaStatus(VTEstudo* estudo);
	bool __fastcall RenovaStatus(TList *lisEQP);
	bool __fastcall SalvaReducoes(VTEstudo *estudo);
	bool __fastcall SalvaReducoesEstudoZero(VTEstudo *estudo);
	bool __fastcall SalvaReducoesEstudoNaoZero(VTEstudo *estudo);
	bool __fastcall SPSNP_dlDominio(VTEstudo *estudo, VTDominio* dominio);
	bool __fastcall SPSNP_dlEquipamentoBarra(VTEstudo* estudo, VTEqpto* eqpto);
	bool __fastcall SPSNP_dlEquipTmpEquipCurva(VTEstudo* estudo);
	bool __fastcall SPSNP_dlEstudo(VTEstudo* estudo, bool bolRedeInclusive);
	bool __fastcall SPSNP_dlEstudoCorte(VTEstudo* estudo, TList* lisMRede);
	bool __fastcall SPSNP_dlEstudoCorte(VTEstudo* estudo, int intMRedeId);
	bool __fastcall SPSNP_dlEstudoEquip(VTEstudo* estudo, bool bolRedeInclusive);
	bool __fastcall SPSNP_dlEstudoRede(VTEstudo *estudo, VTRede* rede);
	bool __fastcall SPSNP_dlManobra(VTManobra* manobra);
	bool __fastcall SPSNP_dlManobraChave(VTManobra* manobra);
	bool __fastcall SPSNP_dlMedidor(VTEstudo* estudo, VTEqpto* eqpto);
	bool __fastcall SPSNP_dlRedeDominio(VTEstudo *estudo, VTRede* rede);
	bool __fastcall SPSNP_dlRREquipamentoBarra(VTEstudo* estudo, VTEqpto* eqpto, int intMRedeId,
		int intCorteId);
	bool __fastcall SPSNP_dlRREquipamentoBarra(VTEstudo* estudo, int intTipoEquipId,
		TList* lisMRede);
	bool __fastcall SPSNP_dlSincronismo(VTLock* lock);
	bool __fastcall SPSNP_dlSincronismoChave(VTLock* lock);
	bool __fastcall SPSNP_dlTmpBarraRede(VTEstudo* estudo, TList* lstRedes);
	bool __fastcall SPSNP_dlTmpCurva(void);
	bool __fastcall SPSNP_dlTmpEquip(void);
	bool __fastcall SPSNP_dlTmpEquipCurva(void);
	bool __fastcall SPSNP_dlTmpEquipCorte(void);
	bool __fastcall SPSNP_dlTmpEstudoRedeCorte(void);
	bool __fastcall SPSNP_dlTrafoEnrol(VTEstudo* estudo, VTEqpto* eqpto);
	bool __fastcall SPSNP_rdBarra(VTEstudo* estudo, bool bolUseRedeTmp = false,
		bool bolObra = false);
	bool __fastcall SPSNP_rdBarraCursor(VTEstudo* estudo, TADOQuery* ptrQryDS);
	bool __fastcall SPSNP_rdCanal(VTEstudo* estudo, bool bolUseRedeTmp = false,
		bool bolObra = false);
	bool __fastcall SPSNP_rdCapacitor(VTEstudo* estudo, bool bolUseRedeTmp = false,
		bool bolObra = false);
	bool __fastcall SPSNP_rdCapacitorSerie(VTEstudo* estudo, bool bolUseRedeTmp = false,
		bool bolObra = false);
	bool __fastcall SPSNP_rdCarga(VTEstudo* estudo, bool bolUseRedeTmp = false,
		bool bolObra = false);
	bool __fastcall SPSNP_rdCargaIP(VTEstudo* estudo, bool bolUseRedeTmp = false,
		bool bolObra = false);
	bool __fastcall SPSNP_rdCargaNL(VTEstudo* estudo, int intRedeId, bool bolObra);
	bool __fastcall SPSNP_rdChave(VTEstudo* estudo, bool bolUseRedeTmp = false,
		bool bolObra = false);
	bool __fastcall SPSNP_rdCorteMRede(VTEstudo* estudo, TList *lisMRede, TList *lisCORTE);
	bool __fastcall SPSNP_rdDominio(void);
	bool __fastcall SPSNP_rdTipoEqbarRedes(VTEstudo* estudo, int intTipoEquipId, TList* lstRedes);
	bool __fastcall SPSNP_rdTipoEqbarCargaRedes(VTEstudo* estudo, int intTipoEquipId,
		TList* lstRedes);
	bool __fastcall SPSNP_rdTipoEqbarCargaIPRedes(VTEstudo* estudo, int intTipoEquipId,
		TList* lstRedes);
	bool __fastcall SPSNP_rdTipoEqligRedes(VTEstudo* estudo, int intTipoEquipId, TList* lstRedes);

	bool __fastcall SPSNP_rdFiltro(VTEstudo* estudo, bool bolUseRedeTmp = false,
		bool bolObra = false);
	bool __fastcall SPSNP_rdGerador(VTEstudo* estudo, bool bolUseRedeTmp = false,
		bool bolObra = false);
	bool __fastcall SPSNP_rdManobra(void);
	bool __fastcall SPSNP_rdMedidor(VTEstudo* estudo, bool bolUseRedeTmp = false,
		bool bolObra = false);
	bool __fastcall SPSNP_rdMRedeCorte(VTEstudo* estudo, TList *lisCORTE);
	bool __fastcall SPSNP_rdMutua(VTEstudo* estudo, bool bolUseRedeTmp = false,
		bool bolObra = false);
	bool __fastcall SPSNP_rdReator(VTEstudo* estudo, bool bolUseRedeTmp = false,
		bool bolObra = false);
	bool __fastcall SPSNP_rdRede(VTEstudo* estudo, int intTipoRedeId = IDINDEFINIDO,
		bool bolObra = false);
	bool __fastcall SPSNP_rdRedeCursor(VTEstudo* estudo, TADOQuery* ptrQryDS,
		TList *lisRedeAux = NULL);
	bool __fastcall SPSNP_rdRedeDominio(VTEstudo* estudo, VTDominio *dominio);
	bool __fastcall SPSNP_rdRedeMRede(VTEstudo* estudo, int intMRedeId, TList *lisRedeAux);
	bool __fastcall SPSNP_rdRedeVizinha(VTEstudo* estudo, int intRedeId, TList *lisVIZ);
	bool __fastcall SPSNP_rdRRBarra(VTEstudo* estudo, bool bolSemFiltro);
	bool __fastcall SPSNP_rdRRCarga(VTEstudo* estudo, bool bolSemFiltro);
	bool __fastcall SPSNP_rdRRSuprimento(VTEstudo* estudo, bool bolSemFiltro);
	bool __fastcall SPSNP_rdRRTrafo(VTEstudo* estudo, bool bolSemFiltro);
	bool __fastcall SPSNP_rdRRYRef(VTEstudo* estudo, bool bolSemFiltro);
	bool __fastcall SPSNP_rdSincronismo(void);
	bool __fastcall SPSNP_rdSuprimento(VTEstudo* estudo, bool bolUseRedeTmp = false,
		bool bolObra = false);
	bool __fastcall SPSNP_rdTrafo(VTEstudo* estudo, bool bolUseRedeTmp = false,
		bool bolObra = false);
	bool __fastcall SPSNP_rdTrafo3E(VTEstudo* estudo, bool bolUseRedeTmp = false,
		bool bolObra = false);
	bool __fastcall SPSNP_rdTrafoZZ(VTEstudo* estudo, bool bolUseRedeTmp = false,
		bool bolObra = false);
	bool __fastcall SPSNP_rdTrecho(VTEstudo* estudo, bool bolUseRedeTmp = false,
		bool bolObra = false);
	bool __fastcall SPSNP_wrAgrupamento(VTEstudo *estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrAlternativaObra(VTEstudo *estudo);
	bool __fastcall SPSNP_wrBarra(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrBarraUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrBarraIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrBarraRedeIns(VTEstudo* estudo);
	bool __fastcall SPSNP_wrBateria(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrBateriaIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrBateriaUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrCanal(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrCanalIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrCanalUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrCarga(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrCargaIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrCargaUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrCargaIP(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrCargaIPIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrCargaIPUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrCargaNL(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrCapacitor(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrCapacitorIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrCapacitorUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrCapacitorSerie(VTEstudo* estudo, TList* lisObjRede,
		bool bolObra = false);
	bool __fastcall SPSNP_wrCapacitorSerieIns(VTEstudo* estudo, TList* lisObjRede,
		bool bolObra = false);
	bool __fastcall SPSNP_wrCapacitorSerieUpd(VTEstudo* estudo, TList* lisObjRede,
		bool bolObra = false);
	bool __fastcall SPSNP_wrChave(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrChaveIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrChaveUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrDominio(TList* lisDominio, bool bolObra = false);
	bool __fastcall SPSNP_wrEquipamentoIns(VTEstudo* estudo, TList* lisObjRede, int intTipoId,
		int intRedeId = IDINDEFINIDO, bool bolObra = false);
	bool __fastcall SPSNP_wrEquipamentoUpd(VTEstudo* estudo, VTEqpto* eqpto, int intTipoId,
		int intRedeId = IDINDEFINIDO, bool bolObra = false);
	bool __fastcall SPSNP_wrEquipamentoIns(VTEstudo* estudo, VTEqpto* eqpto, int intTipoId,
		int intRedeId = IDINDEFINIDO, bool bolObra = false);
	bool __fastcall SPSNP_wrEquipamentoBarraUpd(VTEstudo* estudo, VTEqpto* eqpto);
	bool __fastcall SPSNP_wrEquipamentoBarraIns(VTEstudo* estudo, TList* lisObjRede);
	bool __fastcall SPSNP_wrEstudoAlternativa(VTEstudo *estudo);
	bool __fastcall SPSNP_wrEstudoCorteNaoZero(VTEstudo* estudo, TList* lisMRede);
	bool __fastcall SPSNP_wrEstudoCorteZero(VTEstudo* estudo, TList* lisMRede);
	bool __fastcall SPSNP_wrEstudoEqpto(VTEstudo* estudo);
	bool __fastcall SPSNP_wrFiltro(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrFiltroIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrFiltroUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrGerador(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrGeradorIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrGeradorUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrManobra(VTEstudo* estudo);
	bool __fastcall SPSNP_wrManobraChave(VTEstudo* estudo, VTManobra* manobra);
	bool __fastcall SPSNP_wrMedidor(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrMedidorIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrMRedeRREquipTmpEquipCurva(VTEstudo* estudo, int intMRedeId);
    bool __fastcall SPSNP_wrMunicipio(void);
	bool __fastcall SPSNP_wrMutua(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrMutuaIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrMutuaUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrObraAcao(VTEstudo *estudo);
	bool __fastcall SPSNP_wrReator(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrReatorIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrReatorUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrRede(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrRedeUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrRedeIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrRedeDominioIns(VTEstudo *estudo, TList* lisRede);
	bool __fastcall SPSNP_wrRegulador(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrRRCarga(VTEstudo* estudo, TList* lisMRede, bool bolObra = false);
	bool __fastcall SPSNP_wrRRCargaIns(VTEstudo* estudo, TList* lisMRede, bool bolObra = false);
	bool __fastcall SPSNP_wrRRCargaUpd(VTEstudo* estudo, TList* lisMRede, bool bolObra = false);
	bool __fastcall SPSNP_wrRREquipamentoBarra(VTEstudo* estudo, int intTipoEquipId,
		TList* lisMRede);
	bool __fastcall SPSNP_wrRREquipamentoBarra(VTEstudo* estudo, VTEqpto* eqpto, int intMRedeId,
		int intCorteId);
	bool __fastcall SPSNP_wrRRSuprimento(VTEstudo* estudo, TList* lisMRede, bool bolObra = false);
	bool __fastcall SPSNP_wrRRSuprimentoIns(VTEstudo* estudo, TList* lisMRede, bool bolObra =
		false);
	bool __fastcall SPSNP_wrRRSuprimentoUpd(VTEstudo* estudo, TList* lisMRede, bool bolObra =
		false);
	bool __fastcall SPSNP_wrRRTrafo(VTEstudo* estudo, int intRedeId, int intCorteId,
		TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrRRYRef(VTEstudo* estudo, int intRedeId, int intCorteId,
		TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrSincronismo(VTEstudo* estudo);
	bool __fastcall SPSNP_wrSincronismoChave(VTEstudo* estudo, VTLock* sincronismo);
	bool __fastcall SPSNP_wrSuprimento(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrSuprimentoIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra =
		false);
	bool __fastcall SPSNP_wrSuprimentoUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra =
		false);
	bool __fastcall SPSNP_wrTmpEquip(int intEquipId);
	bool __fastcall SPSNP_wrTmpCurva(int intEstudoId);
	bool __fastcall SPSNP_wrTmpEquip(TList* lisObjRede);
	bool __fastcall SPSNP_wrTmpEquipEstudo(int intEstudoId);
	bool __fastcall SPSNP_wrTmpEquipCorte(VTEstudo* estudo, int intTipoEquipId, TList* lisMREDE);
	bool __fastcall SPSNP_wrTmpEstudoMRedeCorte(VTEstudo* estudo, TList* lisMRede);
	bool __fastcall SPSNP_wrTrafo(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrTrafoIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrTrafoUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrTrafoEnrolIns(VTEstudo* estudo, TList* lisObjRede);
	bool __fastcall SPSNP_wrTrafoZZ(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrTrafoZZIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrTrafoZZUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrTrafo3E(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrTrafo3EIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrTrafo3EUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrTrecho(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrTrechoIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
	bool __fastcall SPSNP_wrTrechoUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra = false);
    void __fastcall TrataExternIdRedeLe(strREDE &str_rede);
    void __fastcall TrataExternIdRedeSalva(VTEqpto* eqpto);
	bool __fastcall TrataLixeira(VTEstudo* estudo);
	bool __fastcall SPSNP_wrEstudo(VTEstudo* estudo);
	bool __fastcall SPSNP_wrObraAtribExt(VTEstudo *estudo);
	bool __fastcall SPSNP_wrReguladorIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra);
	bool __fastcall SPSNP_wrReguladorUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra);

    virtual bool __fastcall SPSNP_rdBateria(VTEstudo* estudo, bool bolUseRedeTmp, bool bolObra) = 0;
	virtual bool __fastcall SPSNP_rdEstudo(void) = 0;
	virtual bool __fastcall SPSNP_rdEstudoAlternativa(VTEstudo* estudo) = 0;
	virtual	bool __fastcall SPSNP_rdEstudoObraAtribExt(VTEstudo* estudo) = 0;
	virtual bool __fastcall SPSNP_rdMunicipio(void) = 0;
	virtual bool __fastcall SPSNP_rdRegulador(VTEstudo* estudo, bool bolUseRedeTmp, bool bolObra) = 0;
	// bool __fastcall ValidaVersaoBaseIgualSuperior(AnsiString versao);

protected:
	// objetos externos
	VTApl *apl;
	VTBasico *basico;
	VTMonta *monta;
	VTPath *path;
	VTPatamares *patamares;
	VTRedes *redes;
	VTSNPDBConn *bd_conn;
	TBDfunc *bd_func; // Classe para manipulação de dados
	VTLog *plog; // Ponteiro para objeto de log

	// objetos locais
	VTCronometro *cronometro;
	VTProgresso *progresso;
//jss20191003
	bool reservado;
	TBDcurvaAccess *bd_curva; // Funções para gravações de curvas no Access
	struct sttIndexEquip index; // Indexação local dos identificadores
	TStringList *lstStrData; // Dados a serem salvos em modo batch
};

#endif
// ---------------------------------------------------------------------------
// eof
