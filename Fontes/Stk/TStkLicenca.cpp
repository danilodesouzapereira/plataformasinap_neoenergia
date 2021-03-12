//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TStkLicenca.h"
#include "..\Apl\VTApl.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTStkLicenca* __fastcall NewObjStkLicenca(VTApl *apl_owner)
   {
   try{//cria objeto
      return(new TStkLicenca(apl_owner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TStkLicenca::TStkLicenca(VTApl *apl_owner)
	{
   //variáveis locais
   VTLicencas *licencas  = (VTLicencas*)apl_owner->GetObject(__classid(VTLicencas));

   //salva ponteiro para objeto
   this->apl = apl_owner;
   licenca   = licencas->ExisteLicenca();
	}

//---------------------------------------------------------------------------
__fastcall TStkLicenca::~TStkLicenca(void)
   {
	//nada a fazer
	}

//---------------------------------------------------------------------------
int __fastcall TStkLicenca::EmpresaId(void)
   {
   return(licenca->Empresa);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TStkLicenca::EmpresaNome(void)
   {
   return(licenca->EmpresaStr);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TStkLicenca::UsuarioNome(void)
   {
   return(licenca->Login);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TStkLicenca::UsuarioSenha(void)
   {
   return(licenca->Password);
   }

//-----------------------------------------------------------------------------
//eof



