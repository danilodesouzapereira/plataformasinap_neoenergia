//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TLoglic.h"
#include "..\BDlicenca\VTBDlicenca.h"
#include "..\Licenca\VTLicenca.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
VTLoglic* __fastcall NewObjLoglic(VTBDlicenca *bdlicenca)
   {
   return(new TLoglic(bdlicenca));
   }

//---------------------------------------------------------------------------
__fastcall TLoglic::TLoglic(VTBDlicenca *bdlicenca)
   {
   //salva ponteiro p/ objeto
   this->bdlicenca = bdlicenca;
   }

//---------------------------------------------------------------------------
__fastcall TLoglic::~TLoglic(void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TLoglic::ErroAloca_LicAlocada(VTLicenca *licenca)
   {
   bdlicenca->InsereLog(licenca->ID,
                    operALOCACAO,
                    respRECUSADA,
                    erroLICENCA_ALOCADA,
                    Now(),
                    "CPU_ID=" + licenca->CPU_ID);
   }

//---------------------------------------------------------------------------
void __fastcall TLoglic::ErroAloca_LicBloqueada(VTLicenca *licenca)
   {
   bdlicenca->InsereLog(licenca->ID,
                    operALOCACAO,
                    respRECUSADA,
                    erroLICENCA_BLOQUEADA,
                    Now(),
                    "CPU_ID=" + licenca->CPU_ID);
   }

//---------------------------------------------------------------------------
void __fastcall TLoglic::ErroAloca_LicInexistente(VTLicenca *licenca)
   {
   //não grava log por enquanto
   }

//---------------------------------------------------------------------------
void __fastcall TLoglic::ErroAlocaDemo_LoginRepetido(VTLicenca *licenca)
   {
   bdlicenca->InsereLog(licenca->ID,
                    operALOCACAO,
                    respRECUSADA,
                    erroLOGIN_PASSWORD_REPETIDO,
                    Now(),
                    "CPU_ID=" + licenca->CPU_ID);
   }

//---------------------------------------------------------------------------
void __fastcall TLoglic::ErroLibera_LicAlocada(VTLicenca *licenca)
   {
   bdlicenca->InsereLog(licenca->ID,
                    operLIBERACAO,
                    respRECUSADA,
                    erroLICENCA_ALOCADA,
                    Now(),
                    "CPU_ID=" + licenca->CPU_ID);
   }

//---------------------------------------------------------------------------
void __fastcall TLoglic::ErroLibera_LicBloqueada(VTLicenca *licenca)
   {
   bdlicenca->InsereLog(licenca->ID,
                    operLIBERACAO,
                    respRECUSADA,
                    erroLICENCA_BLOQUEADA,
                    Now(),
                    "CPU_ID=" + licenca->CPU_ID);
   }

//---------------------------------------------------------------------------
void __fastcall TLoglic::ErroLibera_LicInexistente(VTLicenca *licenca)
   {
   //não grava log por enquanto
   }

//---------------------------------------------------------------------------
void __fastcall TLoglic::ErroLibera_LicLivre(VTLicenca *licenca)
   {
   bdlicenca->InsereLog(licenca->ID,
                    operLIBERACAO,
                    respRECUSADA,
                    Now(),
                    erroLICENCA_LIVRE,
                    "");
   }

//---------------------------------------------------------------------------
void __fastcall TLoglic::ErroRenova_LicAlocada(VTLicenca *licenca)
   {
   bdlicenca->InsereLog(licenca->ID,
                    operRENOVACAO,
                    respRECUSADA,
                    erroLICENCA_ALOCADA,
                    Now(),
                    "CPU_ID=" + licenca->CPU_ID);
   }

//---------------------------------------------------------------------------
void __fastcall TLoglic::ErroRenova_LicBloqueada(VTLicenca *licenca)
   {
   bdlicenca->InsereLog(licenca->ID,
                    operRENOVACAO,
                    respRECUSADA,
                    erroLICENCA_BLOQUEADA,
                    Now(),
                    "CPU_ID=" + licenca->CPU_ID);
   }

//---------------------------------------------------------------------------
void __fastcall TLoglic::ErroRenova_LicInexistente(VTLicenca *licenca)
   {
   //não grava log por enquanto
   }

//---------------------------------------------------------------------------
void __fastcall TLoglic::ErroRenova_LicLivre(VTLicenca *licenca)
   {
   bdlicenca->InsereLog(licenca->ID,
                    operRENOVACAO,
                    respRECUSADA,
                    erroLICENCA_LIVRE,
                    Now(),
                    "");
   }

//---------------------------------------------------------------------------
void __fastcall TLoglic::LicencaBloqueada(VTLicenca *licenca)
   {
   bdlicenca->InsereLog(licenca->ID,
                    operBLOQUEIO,
                    respATENDIDA,
                    erroNENHUM,
                    Now(),
                    "");
   }

//---------------------------------------------------------------------------
void __fastcall TLoglic::SucessoAloca(VTLicenca *licenca)
   {
   bdlicenca->InsereLog(licenca->ID,
                    operALOCACAO,
                    respATENDIDA,
                    erroNENHUM,
                    Now(),
                    "");
   }

//---------------------------------------------------------------------------
void __fastcall TLoglic::SucessoLibera(VTLicenca *licenca)
   {
   bdlicenca->InsereLog(licenca->ID,
                    operLIBERACAO,
                    respATENDIDA,
                    erroNENHUM,
                    Now(),
                    "");
   }

//---------------------------------------------------------------------------
void __fastcall TLoglic::SucessoRenova(VTLicenca *licenca)
   {
   bdlicenca->InsereLog(licenca->ID,
                    operRENOVACAO,
                    respATENDIDA,
                    erroNENHUM,
                    Now(),
                    "");
   }

//-----------------------------------------------------------------------------
//eof



