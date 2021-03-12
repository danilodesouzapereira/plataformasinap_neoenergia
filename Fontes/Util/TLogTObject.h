//---------------------------------------------------------------------------
#ifndef TLogTObjectH
#define TLogTObjectH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
//#include <WinBase.h>
#include <stdio.h>
class VTRede;
//---------------------------------------------------------------------------
class TLogTObject
	{
   public :
				  __fastcall  TLogTObject(AnsiString fileDir = "");
				  __fastcall ~TLogTObject(void);
		void      __fastcall LogObject(TObject *obj, AnsiString msg);

	private: //dados locais
		void      __fastcall AddTexto(TObject *obj, AnsiString msg);
		void      __fastcall AddTextoRede(VTRede *rede, AnsiString msg);
		void      __fastcall AddTextoLista(TList *list, AnsiString msg);
		FILE *myfile;

	};

//---------------------------------------------------------------------------
#endif
//eof
