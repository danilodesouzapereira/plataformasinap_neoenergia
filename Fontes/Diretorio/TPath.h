// ---------------------------------------------------------------------------
#ifndef TPathH
#define TPathH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTPath.h"

// ---------------------------------------------------------------------------
class TPath : public VTPath
{
public:
	__fastcall TPath(void);
	__fastcall ~TPath(void);
	AnsiString __fastcall ArqBitmap(int bmp_id);
	AnsiString __fastcall ArqRedeEmpresa(void);
	AnsiString __fastcall ArqRedeSinap(void);
	AnsiString __fastcall ArqRelCurto(void);
	AnsiString __fastcall ArqRelFlow(void);
	AnsiString __fastcall ArqRelFlowDebug(void);
	AnsiString __fastcall ArqTermoUso(void);
	AnsiString __fastcall ArqTermoUsoDemo(void);
	AnsiString __fastcall ArqTermoUsoDidatico(void);
	AnsiString __fastcall DirBmp(void);
	AnsiString __fastcall DirDat(void);
	AnsiString __fastcall DirExporta(void);
	AnsiString __fastcall DirGeral(void);
	AnsiString __fastcall DirHelp(void);
	AnsiString __fastcall DirHistorico(void);
	AnsiString __fastcall DirIcone(void);
	AnsiString __fastcall DirImporta(void);
	AnsiString __fastcall DirModuloConstrutivo(void);
	AnsiString __fastcall DirRedes(void);

	/**
	*Retorna o path completo até o diretório temporário utilizado pelo SINAPgrid.
	*Caso seja definido um diretório para os processos batch em paralelo, este método retornará o valor
	*obtido pelo método PM_GetDirTmpBatch, caso contrário será <diretório de instalação do SINAPgrid>\tmp
	*/
	AnsiString __fastcall DirTmp(void);

	AnsiString __fastcall DirUsuario(void);
	//AnsiString __fastcall DirTmpBatch(void);

private: // métodos acessados via property
	AnsiString __fastcall PM_GetArqHistorico(void);
	AnsiString __fastcall PM_GetArqRede(void);
	AnsiString __fastcall PM_GetDirBase(void);
	AnsiString __fastcall PM_GetDirBin(void);


	/**
	*Retorna o diretório temporário do SINAPgrid quando este é executado em modo batch.
	*@return O caminho completo até o diretório temporário, ex.: C:\Users\usrsnp\AppData\Local\Temp
	*/
	AnsiString __fastcall PM_GetDirTmpBatch(void);

	// set
	void __fastcall PM_SetArqHistorico(AnsiString arq_historico);
	void __fastcall PM_SetArqRede(AnsiString arq_rede);
	void __fastcall PM_SetDirBase(AnsiString dir_base);
	void __fastcall PM_SetDirBin(AnsiString dir_bin);

	/**
	*Define o diretório temporário do SINAPgrid quando este é executado em modo batch.
	*@param tmpDir O caminho completo até o diretório temporário, ex.: C:\Users\usrsnp\AppData\Local\Temp
	*/
	void __fastcall PM_SetDirTmpBatch(AnsiString tmpDir);

	// dados acessados via property
	struct
	{
		AnsiString historico;
		AnsiString rede;
	}

	arq;

	struct
	{
		AnsiString base;
		AnsiString bin;
		AnsiString tmpBatch;
	}

	dir;
};

#endif
// ---------------------------------------------------------------------------
// eof
