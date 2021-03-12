// ---------------------------------------------------------------------------
#ifndef TImportaBDGDH
#define TImportaBDGDH

// arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\ImportaRedeGis\VTImportaRedeGis.h>
#include <Fontes\ImportaBDGD\Ugrid\VTImportador.h>

// ---------------------------------------------------------------------------
class VTConversor;
class VTLog;
class VTCommons;
class VTParse;
class TSubestacao;
class VTImportador;

// ---------------------------------------------------------------------------
class TImportaBDGD : public VTImportaRedeGIS
{
public: // Property
	__property VTImportador *Importador = {read=importador_arq, write=importador_arq};

public: // Metodos
	__fastcall TImportaBDGD(TComponent *Owner, VTApl *apl_owner);
	__fastcall ~TImportaBDGD(void);

	virtual bool __fastcall ArquivoEgrid(void);
	bool __fastcall ImportaLisArqPrimario(TStrings *files, strOPImporta *opcao);
	bool __fastcall ImportaLisArqSubestacao(TStrings *files, strOPImporta *opcao);
	void __fastcall SelecionaArqTxt(TOpenDialog *OpenDialog, TStrings *files);
	void __fastcall SelecionaLisArqTxt(TOpenDialog *OpenDialog, TStrings *files);
	bool __fastcall ShowModalFormImportaRedeGIS(void);
	bool __fastcall ShowModalFormImportaRedeGIS_SDBT(void);
	bool __fastcall ShowModalFormImportaRedeGIS_SDMT(void);


protected: // métodos
	VTImportador *importador_arq;
//	void __fastcall PM_SetImportador(VTImportador *importadorArquivo);
//	VTImportador* __fastcall PM_GetImportador(void);

//private : // objetos externos
	TComponent *Owner;
	VTApl *apl_owner;

	// dados locais
	TList *lisSubestacao, *lisAllSub;
	VTConversor *conversor;
	VTCommons *commons;
};

#endif

// -----------------------------------------------------------------------------
// eof
