#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\Cartografia\TFormGoogle.h>
#include <Fontes\Cartografia\VTCartografia.h>
#include <Fontes\Cartografia\VTScg.h>
#include <DLL_Inc\Cartografia.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Grafico.a")
#pragma comment(lib, "Radial.a")
#pragma comment(lib, "Rede.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//-----------------------------------------------------------------------------
EXPORT void __fastcall DLL_CloseFormGoogle(TComponent *Owner)
   {
   //nada a fazer
   }

//-----------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormGoogle(TComponent *Owner, VTApl *apl, TWinControl *parent)
   {
   //variáveis locais
   TForm *Form;

   try{//cria FormGoogle
      Form = new TFormGoogle(Owner, apl, parent);
      Form->Show();
      }catch(Exception &e)
         {
         Form = NULL;
         }
   return(Form);
   }

//-----------------------------------------------------------------------------
EXPORT VTCartografia* __fastcall DLL_NewObjCartografia(VTApl *apl, TWinControl *parent, TAction *ActionMapaDisp)
   {
   return(NewObjCartografia(apl, parent, ActionMapaDisp));
   }

//---------------------------------------------------------------------------
EXPORT VTScg*  __fastcall DLL_NewObjScg(VTApl *apl, int empresa_id, AnsiString empresa_nome)
   {
   return(NewObjScg(apl, empresa_id, empresa_nome));
   }

//---------------------------------------------------------------------------
//eof

