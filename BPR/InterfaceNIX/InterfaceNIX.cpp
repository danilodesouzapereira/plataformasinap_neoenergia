#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Apl\VTApl.h>
#include <Fontes\InterfaceNIX\TFormNIX.h>
#include <Fontes\InterfaceNIX\TFormNIXELPA.h>
#include <Fontes\InterfaceNIX\VTInterfaceNIX.h>
#include <Fontes\Licenca\VTLicenca.h>
#include <Fontes\Licenca\VTLicencas.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\InterfaceNIX.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Help.a")
#pragma comment(lib, "InfoZcc.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")
//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormNIX(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
{
	VTLicencas *licencas = (VTLicencas*)apl_owner->GetObject(__classid(VTLicencas));
	VTLicenca *licenca = licencas->ExisteLicenca();
	TForm *Form;

	switch (licenca->Empresa)
	{

	case ELETROPAULO:
		//verifica se jé existe um FormNIX aberto
		if ((Form = ExisteForm("TFormNIXELPA", Owner)) == NULL)
		{//cria um novo TFormNIX
			Form = new TFormNIXELPA(Owner, apl_owner, parent);
		}
		break;
	default:
		//verifica se jé existe um FormNIX aberto
		if ((Form = ExisteForm("TFormNIX", Owner)) == NULL)
		{//cria um novo TFormNIX
			Form = new TFormNIX(Owner, apl_owner, parent);
		}

	}

	return(Form);
}

//---------------------------------------------------------------------------
EXPORT VTInterfaceNIX* __fastcall DLL_NewObjInterfaceNIX(VTApl *apl_owner)
   {
   return(NewObjInterfaceNIX(apl_owner));
   }

//---------------------------------------------------------------------------
//eof

