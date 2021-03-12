//---------------------------------------------------------------------------
#ifndef TServidorH
#define TServidorH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTServidor.h"

//---------------------------------------------------------------------------
class VTBDlicenca;
class VTLicenca;
class VTLoglic;

//---------------------------------------------------------------------------
class TServidor : public VTServidor
   {
   public :
           __fastcall TServidor(void);
           __fastcall ~TServidor(void);
	  bool __fastcall AlocaLicenca(VTLicenca *licenca, AnsiString &diagnostico);
      bool __fastcall Conecta(AnsiString database_name);
      bool __fastcall ConsultaPendrive(VTPendrive *pendrive);
      void __fastcall Desconecta(void);
      bool __fastcall LiberaLicenca(VTLicenca *licenca);
      bool __fastcall RenovaLicenca(VTLicenca *licenca);

   protected: //métodos
      void __fastcall BloqueiaLicencaNaBaseDado(VTLicenca *licenca);
      bool __fastcall CriaLicencaDemo(VTLicenca *licenca);
      void __fastcall DefineLicencaInvalida(VTLicenca *licenca, int status);
		int  __fastcall ValidaAlocaLicenca(VTLicenca *licenca, VTLicenca *lic_micro);
		int  __fastcall ValidaLiberaLicenca(VTLicenca *licenca, VTLicenca *lic_micro);
		int  __fastcall ValidaRenovaLicenca(VTLicenca *licenca, VTLicenca *lic_micro);

	protected: //dados locais
      VTBDlicenca *bdlicenca;
      VTLoglic    *loglic;
      VTLicenca   *lic_micro;
      VTLicenca   *lic_bdado;
   };

//---------------------------------------------------------------------------
#endif
//eof
