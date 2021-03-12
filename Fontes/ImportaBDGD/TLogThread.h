// --------------------------------------------------------------------------------------------- bof
#ifndef TLogThreadH
#define TLogThreadH

#define ltstr_ERRO "Erro"
#define ltstr_WARN "Warning"

// ---------------------------------------------------------------------------------------- includes
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <Classes.hpp>

// ------------------------------------------------------------------------------------ declarations

// Class declaration
class VTApl;

// --------------------------------------------------------------------------------------- namespace
using namespace std;

// ------------------------------------------------------------------------------------------- class
class TLogThread : public TObject
	// class TLogThread : public TThread
{
public:
	__fastcall TLogThread(VTApl *apl, AnsiString nome_classe, AnsiString dir = "");
	__fastcall ~TLogThread(void);
	void __fastcall AddLog(AnsiString string, const char *nome_funcao = __FUNC__, bool isErro = false,
		int ordinal = 0);
	// property
	__property AnsiString Codigo = {read = PM_GetCodigo, write = PM_SetCodigo};

protected: // métodos acessados via property
	AnsiString __fastcall PM_GetCodigo(void);
	void __fastcall PM_SetCodigo(AnsiString codigo);

	// dados acessados via property
	AnsiString codigo;

private : // dados externos
	VTApl *apl;

private: // dados internos
	// FILE *pFile;

	// ofstream myfile;
	FILE *myfile;
	AnsiString name;
	AnsiString nome_arq;

};

#endif

// --------------------------------------------------------------------------------------------- eof