//---------------------------------------------------------------------------
#ifndef TMemoriaH
#define TMemoriaH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include <WinBase.h>

//---------------------------------------------------------------------------
class TMemoria
	{
   public :
				  __fastcall  TMemoria(void);
				  __fastcall ~TMemoria(void);
		double  __fastcall  EmUso_perc(void);

	private: //dados locais
		MEMORYSTATUSEX statex;
	};

//---------------------------------------------------------------------------
#endif
//eof
