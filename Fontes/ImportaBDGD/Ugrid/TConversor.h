// ---------------------------------------------------------------------------
#ifndef TConversorH
#define TConversorH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <map>
#include <Ugrid\VTImportador.h>
#include "..\Exceptions\EImportaUgrid.h"
#include "VTConversor.h"

// ------------------------------------------------------------------------------------ declarations
// #ifdef _DEBUG
class TLogThread;
class TLogManager;
// #endif

//--------------------------------------------------------------------
class VTLigacao;
class VTBarra;
class VTDominio;

// ---------------------------------------------------------------------------
class TConversor : public VTConversor
{
public:
	__fastcall TConversor(VTApl *apl_owner);
	__fastcall ~TConversor(void);
	bool __fastcall ImportaCommons(AnsiString fileName) throw (EImportaUgrid);
	bool __fastcall ImportaRedes(TList *lisSUBESTACAO, strOPImporta *opcao, int destino);
//
//protected: // métodos
//	void __fastcall PM_SetImportador(VTImportador *importadorArquivo);
//	VTImportador* __fastcall PM_GetImportador(void);

private:
    bool __fastcall CriaDominioGlobal(void);
	bool __fastcall CriaNovoArqRede(AnsiString dirname, AnsiString cod_base = "");
	bool __fastcall CriaNovoDiretorio(AnsiString &dirname);
	bool __fastcall DefineDiretorio(AnsiString &dirname);
	void __fastcall ExibeGrafico(void);
	bool __fastcall ImportaToBaseMultipla(TList *lisSUBESTACAO);
	bool __fastcall ImportaToBaseUnica(TList *lisSUBESTACAO);
	bool __fastcall ImportaToMemoria(TList *lisSUBESTACAO);
	bool __fastcall IniciaObjApl(void);
	bool __fastcall IntegraETDcompleta(void);
	void __fastcall MoveLigacaoDeParaNovaBarra(VTLigacao *lig, VTBarra *de);
	void __fastcall LogErro(AnsiString arq_txt, AnsiString msg_erro);
	void __fastcall LogFecha(void);
	void __fastcall LogGeral(void);
	void __fastcall LogInicia(void);
	void __fastcall LogShow(void);
	void __fastcall LogSucesso(AnsiString arq_txt, TDateTime start, TDateTime stop);
    bool __fastcall RedefineDominio(void);
	void __fastcall ReiniciaRedes(bool inclusive_dominio);
	void __fastcall SalvaRede(void);
	void __fastcall VerificaBarrasFronteira(VTRede *redeSec) ;
	void __fastcall VerificaFases(void);
	void __fastcall VerificaMalhas(void);
	void __fastcall VerificaPerdaFerro(void);

	// objetos externos
	VTApl *apl_owner;
	strOPImporta *opcao;
//	VTImportador *importador;

	// dados locais
	VTApl *apl;
	VTDominio *dominio_global;
	VTLog *log_resumo, *log_geral;
	AnsiString nome_arq_log;
	TList *lisNOVASBARRAS;
	TList *lisEQPTOSALTERADOS;

	// #ifdef _DEBUG
	TLogThread *log_thread;
	TLogManager *log_manager;
	AnsiString msglog;
	const char *function;
	// #endif
};


// ---------------------------------------------------------------------------
#endif
// eof
