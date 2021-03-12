#include <vcl.h>
#include <windows.h>

#pragma hdrstop
#include <PlataformaSinap\Fontes\AutoTag\VTAutoTag.h>
#include <PlataformaSinap\DLL_Inc\AutoTag.h>
#pragma argsused
#pragma comment(lib, "Funcao.a")
//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	return 1;
}
//---------------------------------------------------------------------------
EXPORT VTAutoTag*    __fastcall DLL_NewObjAutoTag(VTApl *apl_owner)
   {
   try{
	  return(NewObjAutoTag(apl_owner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
//--------------------------------------------------------------------------

