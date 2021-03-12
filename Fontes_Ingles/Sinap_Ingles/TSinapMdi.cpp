//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TSinapMdi.h"
#include "..\Apl\VTApl.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTSinapMdi* __fastcall NewObjSinapMdi(VTApl *apl_owner)
   {
   try{
		return (new TSinapMdi(apl_owner));
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TSinapMdi::TSinapMdi(VTApl *apl_owner)
	{
	//salva pontero par objetp
	this->apl = apl_owner;
	}

//---------------------------------------------------------------------------
__fastcall TSinapMdi::~TSinapMdi(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
TAction* __fastcall TSinapMdi::PM_GetActionNewChild(void)
	{
	return(PD.ActionNewChild);
	}

//---------------------------------------------------------------------------
TMainMenu* __fastcall TSinapMdi::PM_GetMainMenu(void)
	{
	return(PD.MainMenu);
	}

//---------------------------------------------------------------------------
TPopupMenu* __fastcall TSinapMdi::PM_GetPopupMenuImporta(void)
	{
	return(PD.PopupMenuImporta);
	}

//---------------------------------------------------------------------------
void __fastcall TSinapMdi::PM_SetActionNewChild(TAction *Action)
	{
	PD.ActionNewChild = Action;
	}

//---------------------------------------------------------------------------
void __fastcall TSinapMdi::PM_SetMainMenu(TMainMenu *MainMenu)
	{
	PD.MainMenu = MainMenu;
	}

//---------------------------------------------------------------------------
void __fastcall TSinapMdi::PM_SetPopupMenuImporta(TPopupMenu *PopupMenu)
	{
	PD.PopupMenuImporta = PopupMenu;
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TSinapMdi::UsuarioEmpresa(void)
   {
   //variáveis locais
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca  *licenca  = licencas->ExisteLicenca();

   return((licenca != NULL) ? licenca->EmpresaStr : AnsiString("Indefinida"));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TSinapMdi::UsuarioNome(void)
   {
   //variáveis locais
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTLicenca  *licenca  = licencas->ExisteLicenca();

   return((licenca != NULL) ? licenca->Login : AnsiString("Indefinido"));
   }

//---------------------------------------------------------------------------
bool __fastcall TSinapMdi::UsuarioPrivilegiado(void)
   {
   //variáveis locais
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca  *licenca  = licencas->ExisteLicenca();

   if (licenca == NULL) return(false);
   return(licenca->Tipo == tipoMASTER);
   }

//---------------------------------------------------------------------------
int __fastcall TSinapMdi::UsuarioTipo(void)
   {
   //variáveis locais
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca  *licenca  = licencas->ExisteLicenca();

   if(licenca == NULL) return(tipoINDEFINIDA);
   return(licenca->Tipo);
   }

//---------------------------------------------------------------------------
//eof
