// ---------------------------------------------------------------------------
#ifndef TImportaArquivoUgridH
#define TImportaArquivoUgridH

// ---------------------------------------------------------------------------------------- includes
#include <stdexcept>
#include <list>
#include <Classes.hpp>
#include <System.Classes.hpp>
#include <System.Zip.hpp>

// Plataforma include
#include <Fontes\ImportaBDGD\Ugrid\VTImportador.h>
#include <Fontes\ImportaRedeGis\VTImportaRedeGis.h>

// ---------------------------------------------------------------------------
class BDGDDAO;
class EImportaUgrid;
class VTLog;
class VTProgresso;
class VTCommons;
class TRedeBT;

// Namespace using
using namespace std;

// ---------------------------------------------------------------------------
class TImportaArquivoUgrid : public VTImportador
{
protected:
	TZipFile *zip;
	UnicodeString arquivoConfiguracaoPath;
	UnicodeString diretorioTemporario;
//	UnicodeString gridPath;

	strOPImporta *opcao;
	VTPreMonta *pre_monta;
	TStringList *slisNomes;
	VTApl *apl;
	VTProgresso *progresso;
	BDGDDAO *dao;
	AnsiString nomeSe, nomeCircuito, nomeSec;

	// identificação dos relógios
	// Configuração
#define FILENAME_COMMONS	"commons.cm"
#define RelogioCfgTipoChave "CFG_TipoChave"
	// SED
#define RelogioEtdChave     "SED_Chave"
#define RelogioEtdTrafo     "SED_Trafo"
#define RelogioEtdTrafo3E   "SED_Trafo3E"
	// SDMT
#define RelogioPriCapacitor  "SDMT_Capacitor"
#define RelogioPriCarga      "SDMT_Consumidor"
#define RelogioPriChave      "SDMT_Chave"
#define RelogioPriRede       "SDMT_Rede"
#define RelogioPriRegulador  "SDMT_Regulador"
#define RelogioPriTrecho     "SDMT_Trecho"
	// SDBT
#define RelogioSecCarga      "SDBT_Consumidor"
#define RelogioSecIP         "SDBT_IP"
#define RelogioSecRede       "SDBT_Rede"
#define RelogioSecTrafo      "SDBT_Trafo"
#define RelogioSecTrecho     "SDBT_Trecho"

	// versão
#define VERSAO_UGRID 1.0

public:
	/**
	 *Construtor
	 *@param eGridPath O caminho completo até o arquivo .egrid.
	 *@throws runtime_error Lança uma exceção caso nao seja possivel abrir o arquivo.
	 */
	__fastcall TImportaArquivoUgrid(VTApl *apl, UnicodeString uGridPath);
	//throw(runtime_error);
	/**
	 *Destrutor
	 */
	__fastcall ~TImportaArquivoUgrid(void);
	void __fastcall Close(void);
    VTCommons* __fastcall CriaCommons(UnicodeString filepathXGRID);
	void __fastcall CriaDAO(AnsiString nomeArq);
	void __fastcall DestroiDAO(void);
	void __fastcall IniciaZip(UnicodeString gridPath);
	bool __fastcall ReadCommons(void);
	void __fastcall Unzip(TList *lisTMP);
	bool __fastcall ExisteSubZip(AnsiString sigla_sub);
	TStringList* __fastcall GetSubestacaoList(void);
	bool __fastcall InsereArranjo(void);
	bool __fastcall LeRedes(TSubestacao *subestacao, strOPImporta *opcao);

	// métodos de leitura de tabelas
	bool __fastcall ReadCfg_TipoChave(void);
	// etd
	bool __fastcall ReadEtd_TabBarramento(int etd_id);
	bool __fastcall ReadEtd_TabSe(int etd_id);
	bool __fastcall ReadEtd_TabTrafoSe(int etd_id);
//	// rede primária
	bool __fastcall ReadPri_TabCapacitor(int pri_id);
	bool __fastcall ReadPri_TabChave(int pri_id);
	bool __fastcall ReadPri_TabRede(TRedeMT *primario);
	virtual bool __fastcall ReadPri_TabCargaMT(int pri_id);
	bool __fastcall ReadPri_TabGerador(int pri_id);
	bool __fastcall ReadPri_TabRegulador(int pri_id);
	bool __fastcall ReadPri_TabTrecho(int pri_id);
//	// rede secundária
	bool __fastcall ReadSec_TabRede(TRedeMT *primario);
	bool __fastcall ReadSec_TabTrafoMTBT(TRedeMT *primario);
	bool __fastcall ReadSec_TabCapacitor(int sec_id);
	bool __fastcall ReadSec_TabChave(int sec_id);
	virtual bool __fastcall ReadSec_TabConsBT(int sec_id);
	bool __fastcall ReadSec_TabConsIP(int sec_id);
	bool __fastcall ReadSec_TabGeradorBT(int sec_id);
	bool __fastcall ReadSec_TabRamalLig(int sec_id);
	bool __fastcall ReadSec_TabTrechoBT(int sec_id);
	//

protected: // métodos
    bool __fastcall ReadCommons(VTCommons *commons);
	bool __fastcall LeUmaRedeEtd_TodasRedesPriSec(TSubestacao* subestacao) throw(EImportaUgrid);
	bool __fastcall LeUmaRedePri_TodasRedesSec(TRedeMT *primario) throw(EImportaUgrid);
	bool __fastcall LeUmaRedeSec_Completa(int sec_id);
	bool __fastcall LeUmaRedeSec_SomenteCarga(int sec_id);
	bool __fastcall LeUmaRedeSec_TrafoCarga(int sec_id);
	int __fastcall FieldAsInteger(AnsiString fieldname);
	bool __fastcall VersaoCompativel(AnsiString &versao);

	void __fastcall DefinirDiretorioTemporario(void);
	UnicodeString GetPath(UnicodeString siglaSubestacao);
	void __fastcall Nomes(TStringList *slisNomes);

};

#endif
// ---------------------------------------------------------------------------
// eof
