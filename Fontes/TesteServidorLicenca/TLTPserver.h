//---------------------------------------------------------------------------
#ifndef TLTPserverH
#define TLTPserverH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTLTPserver.h"

//---------------------------------------------------------------------------
class VTLicenca;
class VTServidor;

//---------------------------------------------------------------------------
class TLTPserver : public VTLTPserver
   {
   public :
           __fastcall TLTPserver(void);
           __fastcall ~TLTPserver(void);
      void __fastcall Conecta(AnsiString arq_db);
      bool __fastcall TrataPacote(VTPacote *pacote);

   private: //métodos
      void __fastcall AlocaLicenca(void);
      void __fastcall BloqueiaLicencaNaBaseDado(VTLicenca *licenca);
      void __fastcall ComandoInvalido(void);
      bool __fastcall CriaLicencaDemo(VTLicenca *licenca);
      void __fastcall DefineLicencaInvalida(VTLicenca *licenca, int status);
      void __fastcall LiberaLicenca(void);
      void __fastcall MontaPacote(VTPacote *pacote, AnsiString comando, VTLicenca *licenca);
      void __fastcall RenovaLicenca(void);
      int  __fastcall ValidaAlocaLicenca(VTLicenca *licenca);
      int  __fastcall ValidaLiberaLicenca(VTLicenca *licenca);
      int  __fastcall ValidaRenovaLicenca(VTLicenca *licenca);

   public:  //objetos externos
      VTPacote *pacote;

   private: //dados locais
      VTServidor  *servidor;
      VTLicenca   *lic_micro;
      VTLicenca   *lic_bdado;
   };

//---------------------------------------------------------------------------
#endif
//eof
