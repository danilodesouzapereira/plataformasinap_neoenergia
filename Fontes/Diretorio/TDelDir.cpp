// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <dir.h>
#include <winbase.h>
// #include <stdio.h>
#include <Registry.hpp>
#include "TDelDir.h"
#include "VTDir.h"
#include "VTPath.h"
#include "..\Apl\VTApl.h"
#include "..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TDelDir::TDelDir(VTApl *apl)
{
	// salva ponteiro p/ objetos externos
	this->apl = apl;
	// roda
	Executa();
}

// ---------------------------------------------------------------------------
__fastcall TDelDir::~TDelDir(void)
{
	// nada
}

// ---------------------------------------------------------------------------
bool __fastcall TDelDir::DeleteDiretorioParaLixeira(AnsiString dir)
{ // nao sei pq nao ta indo pra lixeira...
	// variaveis locais
	int len = dir.Length();
	TCHAR* pszFrom = new TCHAR[len + 4]; // 4 to handle wide char
	// _tcscpy(pszFrom, dir.c_str()); //todo:remove warning//;//convet wchar to char*
	UnicodeString wc = UnicodeString(dir);
	wcscpy_s(pszFrom, len + 2, wc.w_str());
	pszFrom[len] = 0;
	pszFrom[len + 1] = 0;

	SHFILEOPSTRUCT fileop;
	fileop.hwnd = NULL; // no status display
	fileop.wFunc = FO_DELETE; // delete operation
	fileop.pFrom = pszFrom; // source file name as double null terminated string
	fileop.pTo = NULL; // no destination needed
	fileop.fFlags = FOF_NOCONFIRMATION | FOF_SILENT; // do not prompt the user

	// envia para lixeira
	fileop.fFlags |= FOF_ALLOWUNDO;

	fileop.fAnyOperationsAborted = FALSE;
	fileop.lpszProgressTitle = NULL;
	fileop.hNameMappings = NULL;

	int ret = SHFileOperation(&fileop);
	// SHFileOperation returns zero if successful; otherwise nonzero
	delete[]pszFrom;
	return (0 == ret);
}

// ---------------------------------------------------------------------------
bool __fastcall TDelDir::Executa(void)
{
	// variaveis locais
	AnsiString dir_tmp, dir_nome;
	TStringList *slDirTmp = new TStringList();
	TDateTimeInfoRec datetime;
	double tempo = 30.;
	TDateTime limite = Now() - tempo;
	VTDir *dir = (VTDir*)apl->GetObject(__classid(VTDir));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// diretorio onde estao todos os diretorios temporarios gerados pelo Sinap
	dir_tmp = path->DirBase + "\\Tmp";
	// lista diretorios
	if (!dir->LisDirectory(dir_tmp, slDirTmp))
	{
		delete slDirTmp;
		return false;
	}

	// verifica data
	for (int n = slDirTmp->Count - 1; n >= 0; n--)
	{
		dir_nome = slDirTmp->Strings[n];
		if (!DirectoryExists(dir_nome))
		{
			continue;
		}
		if (!FileGetDateTimeInfo(dir_nome, datetime))
		{
			continue;
		}
		if (datetime.CreationTime < limite)
		{ // apaga diretorio antigo
//			DeleteDiretorioParaLixeira(dir_nome);
			dir->DeleteDirectory(dir_nome);
		}
	}

	// ======================================================
	// para testes
	// for (int n = slDirTmp->Count - 1; n >= 0; n--)
	// {
	// dir_nome = slDirTmp->Strings[n];
	// if (!DirectoryExists(dir_nome))
	// {
	// continue;
	// }
	// if (dir_nome.AnsiCompareIC(path->DirTmpBatch) == 0)
	// {
	// continue;
	// }
	// // apaga diretorio antigo
	// DeleteDiretorioParaLixeira(dir_nome);
	// }
	// ======================================================

	delete slDirTmp;
	return true;
}

// ---------------------------------------------------------------------------
// eof

