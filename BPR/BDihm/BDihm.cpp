#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Fontes\BDihm\TFormAbre.h>
#include <Fontes\BDihm\TFormEstudoRnt.h>
#include <Fontes\BDihm\VTAbre.h>
#include <Fontes\BDihm\VTInsere.h>
#include <Fontes\BDihm\VTSalva.h>
#include <Fontes\Planejamento\VTEstudo.h>
#include <DLL_Inc\BDihm.h>
#pragma argsused

#pragma comment(lib, "BD.a")
#pragma comment(lib, "Busca.a")
#pragma comment(lib, "Editor.a")
#pragma comment(lib, "Funcao.a")
#pragma comment(lib, "Grafico.a")
#pragma comment(lib, "Progresso.a")
#pragma comment(lib, "Rede.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
   {
   return 1;
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormAbre(TComponent *Owner, VTApl *apl_owner)
   {
   try{
      return(new TFormAbre(Owner, apl_owner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormAbreEstudoRBT(TComponent *Owner, VTApl *apl_owner)
   {
   try{
      return(new TFormEstudoRnt(Owner, apl_owner, estudoRECLAMACAO_BT));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
EXPORT TForm* __fastcall DLL_NewFormAbreEstudoRMT(TComponent *Owner, VTApl *apl_owner)
   {
   try{
      return(new TFormEstudoRnt(Owner, apl_owner, estudoRECLAMACAO_MT));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
EXPORT VTAbre* __fastcall DLL_NewObjAbre(TComponent *Owner, VTApl *apl_owner)
   {
   try{
      return(NewObjAbre(Owner, apl_owner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//-----------------------------------------------------------------------------
EXPORT VTInsere*  __fastcall DLL_NewObjInsere(TComponent *Owner, VTApl *apl_owner, TAction *ActionInsRede)
   {
   try{
      return(NewObjInsere(apl_owner, ActionInsRede));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
EXPORT VTSalva* __fastcall DLL_NewObjSalva(TComponent *Owner, VTApl *apl_owner)
   {
   try{
      return(NewObjSalva(Owner, apl_owner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
//eof

