//---------------------------------------------------------------------------
#ifndef TServidorWebH
#define TServidorWebH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "TServidor.h"
#include "wsSinap.h"

//---------------------------------------------------------------------------
class TServidorWeb : public TServidor
   {
	public :
           __fastcall TServidorWeb(void);
           __fastcall ~TServidorWeb(void);
		bool __fastcall AlocaLicenca(VTLicenca *licenca, AnsiString &diagnostico);
		bool __fastcall LiberaLicenca(VTLicenca *licenca);
		bool __fastcall RenovaLicenca(VTLicenca *licenca);

   private: //métodos
		int         __fastcall CalculaValidade(VTLicenca *licenca);
		bool        __fastcall ConectaWS(VTLicenca *licenca, AnsiString &diagnostico);
		AnsiString  __fastcall PathDatabase(VTPendrive *pendrive);
		int         __fastcall ValidaAlocaLicencaWeb(Licenca *lic_web, VTLicenca *lic_micro);
		int         __fastcall ValidaLiberaLicencaWeb(Licenca *lic_web, VTLicenca *lic_micro);
	};

//---------------------------------------------------------------------------
#endif
//eof
