//---------------------------------------------------------------------------
#ifndef TLoglicH
#define TLoglicH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTLoglic.h"

//---------------------------------------------------------------------------
enum tipoOPERACAO {operALOCACAO=1, operRENOVACAO, operLIBERACAO, operBLOQUEIO};
enum tipoRESPOSTA {respATENDIDA=1, respRECUSADA};
enum tipoERRO     {erroPRODUTO_INV=1, erroLOGIN_INV,       erroPASSWORD_INV,
                   erroLICENCA_INV,   erroLICENCA_ALOCADA, erroLICENCA_BLOQUEADA,
                   erroLICENCA_LIVRE, erroNENHUM, erroLOGIN_PASSWORD_REPETIDO};
                   
//---------------------------------------------------------------------------
class VTLoglic;

//---------------------------------------------------------------------------
class TLoglic : public VTLoglic
   {
   public :
           __fastcall TLoglic(VTBDlicenca *bdlicenca);
           __fastcall ~TLoglic(void);
      //
      void __fastcall ErroAloca_LicAlocada(VTLicenca *licenca);
      void __fastcall ErroAloca_LicBloqueada(VTLicenca *licenca);
      void __fastcall ErroAloca_LicInexistente(VTLicenca *licenca);
      void __fastcall ErroAlocaDemo_LoginRepetido(VTLicenca *licenca);
      void __fastcall ErroLibera_LicAlocada(VTLicenca *licenca);
      void __fastcall ErroLibera_LicBloqueada(VTLicenca *licenca);
      void __fastcall ErroLibera_LicInexistente(VTLicenca *licenca);
      void __fastcall ErroLibera_LicLivre(VTLicenca *licenca);
      void __fastcall ErroRenova_LicAlocada(VTLicenca *licenca);
      void __fastcall ErroRenova_LicBloqueada(VTLicenca *licenca);
      void __fastcall ErroRenova_LicInexistente(VTLicenca *licenca);
      void __fastcall ErroRenova_LicLivre(VTLicenca *licenca);
      void __fastcall LicencaBloqueada(VTLicenca *licenca);
      void __fastcall SucessoAloca(VTLicenca *licenca);
      void __fastcall SucessoLibera(VTLicenca *licenca);
      void __fastcall SucessoRenova(VTLicenca *licenca);
      
   private:
      VTBDlicenca *bdlicenca;
   };

//---------------------------------------------------------------------------
#endif
//eof
