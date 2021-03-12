// --------------------------------------------------------------------------------------------- bof
#ifndef TLogManagerH
#define TLogManagerH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>
#include <stdio.h>
#include <iostream>
#include <fstream>

// ------------------------------------------------------------------------------------ declarations

// Class declaration
class VTApl;
class TLogThread;

// --------------------------------------------------------------------------------------- namespace
using namespace std;
// using namespace sinapsis::sinap::neoenergia;

// ------------------------------------------------------------------------------------------- class
class TLogManager : public TObject
{
public:
	__fastcall TLogManager(VTApl *apl);
	__fastcall ~TLogManager(void);
	TLogThread* __fastcall NewLogThread(AnsiString nome_classe, AnsiString dir = "");
	TLogThread* __fastcall LogThread(AnsiString nome_classe);

private: // métodos

		public : // objetos externos

	public : // dados locais

		private : // dados externos
		VTApl *apl;
	// dados internos
	TList *lisLogs;
};

#endif

// --------------------------------------------------------------------------------------------- eof
