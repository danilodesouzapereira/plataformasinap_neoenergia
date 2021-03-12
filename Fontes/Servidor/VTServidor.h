//---------------------------------------------------------------------------
#ifndef VTServidorH
#define VTServidorH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTLicenca;
class VTPendrive;

//---------------------------------------------------------------------------
class VTServidor : public TObject
   {
   public:
                   __fastcall VTServidor(void) {};
      virtual      __fastcall ~VTServidor(void) {};
      virtual bool __fastcall AlocaLicenca(VTLicenca *licenca, AnsiString &diagnostico) = 0;
      virtual bool __fastcall Conecta(AnsiString database_name) = 0;
      virtual bool __fastcall ConsultaPendrive(VTPendrive *pendrive) = 0;
      virtual void __fastcall Desconecta(void) = 0;
      virtual bool __fastcall LiberaLicenca(VTLicenca *licenca) = 0;
      virtual bool __fastcall RenovaLicenca(VTLicenca *licenca) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTServidor* __fastcall NewObjServidor(void);
VTServidor* __fastcall NewObjServidorLocal(void);
VTServidor* __fastcall NewObjServidorRemoto(void);

#endif
//---------------------------------------------------------------------------
//eof

