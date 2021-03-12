//---------------------------------------------------------------------------
#ifndef TServidorLocalH
#define TServidorLocalH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTServidor.h"

//---------------------------------------------------------------------------
//class VTPacote;
class VTPendrive;
class VTPenFinder;

//---------------------------------------------------------------------------
class TServidorLocal : public VTServidor
   {
   public :
           __fastcall TServidorLocal(void);
           __fastcall ~TServidorLocal(void);
      bool __fastcall AlocaLicenca(VTLicenca *licenca, AnsiString &diagnostico);
      bool __fastcall Conecta(AnsiString database_name);
      bool __fastcall ConsultaPendrive(VTPendrive *pendrive);
      void __fastcall Desconecta(void);
      bool __fastcall LiberaLicenca(VTLicenca *licenca);
      bool __fastcall RenovaLicenca(VTLicenca *licenca);

   private: //métodos
      VTPendrive* __fastcall ExisteServidorLicenca(void);
      AnsiString  __fastcall PathDatabase(VTPendrive *pendrive);
      bool        __fastcall ValidaPendrive(VTPendrive *pendrive);

   private: //objetos externos

   private: //objetos externos

   private: //dados locais
      VTServidor  *servidor;
      VTPenFinder *penfinder;
   };

//---------------------------------------------------------------------------
#endif
//eof
