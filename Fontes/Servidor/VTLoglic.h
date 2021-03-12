//-----------------------------------------------------------------------------
#ifndef VTLoglic_H
#define VTLoglic_H

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTBDlicenca;
class VTLicenca;

//-----------------------------------------------------------------------------
class VTLoglic  : public TObject
   {
   public:
                   __fastcall VTLoglic(void)  {};
      virtual      __fastcall ~VTLoglic(void) {};
      //
      virtual void __fastcall ErroAloca_LicAlocada(VTLicenca *licenca) = 0;
      virtual void __fastcall ErroAloca_LicBloqueada(VTLicenca *licenca) = 0;
      virtual void __fastcall ErroAloca_LicInexistente(VTLicenca *licenca) = 0;
      //
      virtual void __fastcall ErroAlocaDemo_LoginRepetido(VTLicenca *licenca) = 0;
      //
      virtual void __fastcall ErroLibera_LicAlocada(VTLicenca *licenca) = 0;
      virtual void __fastcall ErroLibera_LicBloqueada(VTLicenca *licenca) = 0;
      virtual void __fastcall ErroLibera_LicInexistente(VTLicenca *licenca) = 0;
      virtual void __fastcall ErroLibera_LicLivre(VTLicenca *licenca) = 0;
      //
      virtual void __fastcall ErroRenova_LicAlocada(VTLicenca *licenca) = 0;
      virtual void __fastcall ErroRenova_LicBloqueada(VTLicenca *licenca) = 0;
      virtual void __fastcall ErroRenova_LicInexistente(VTLicenca *licenca) = 0;
      virtual void __fastcall ErroRenova_LicLivre(VTLicenca *licenca) = 0;
      //
      virtual void __fastcall LicencaBloqueada(VTLicenca *licenca) = 0;
      //
      virtual void __fastcall SucessoAloca(VTLicenca *licenca) = 0;
      virtual void __fastcall SucessoLibera(VTLicenca *licenca) = 0;
      virtual void __fastcall SucessoRenova(VTLicenca *licenca) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTLoglic
//-----------------------------------------------------------------------------
VTLoglic* __fastcall NewObjLoglic(VTBDlicenca *bdlicenca);

#endif
//---------------------------------------------------------------------------
//eof
