//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TClasses.h"
#include "VTClasse.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTClasses* __fastcall NewObjClasses(void)
   {
   try{
      return(new TClasses());
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TClasses::TClasses()
   {
   //cria lista
   lisCLASSE = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TClasses::~TClasses(void)
   {
   //destrói lista e seus objetos
   if (lisCLASSE) {LimpaTList(lisCLASSE); delete lisCLASSE; lisCLASSE = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TClasses::Clear(void)
   {
   //destrói objetos
   LimpaTList(lisCLASSE);
   }

//---------------------------------------------------------------------------
VTClasse* __fastcall TClasses::ExisteClasse(AnsiString codigo)
   {
   //variáveis locais
   VTClasse *classe;

   for (int n = 0; n < lisCLASSE->Count; n++)
      {
      classe = (VTClasse*)lisCLASSE->Items[n];
      if (classe->Codigo.AnsiCompareIC(codigo) == 0) return(classe);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTClasse* __fastcall TClasses::ExisteClasse(int classe_id)
   {
   //variáveis locais
   VTClasse *classe;

   for (int n = 0; n < lisCLASSE->Count; n++)
      {
      classe = (VTClasse*)lisCLASSE->Items[n];
      if (classe->Id == classe_id) return(classe);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTClasse* __fastcall TClasses::ExisteClasseTag(int tag)
   {
   //variáveis locais
   VTClasse *classe;

   for (int n = 0; n < lisCLASSE->Count; n++)
      {
      classe = (VTClasse*)lisCLASSE->Items[n];
      if (classe->Tag == tag) return(classe);
      }
   return(NULL);
   }
//---------------------------------------------------------------------------
VTClasse* __fastcall TClasses::First(void)
   {
   if (lisCLASSE->Count == 0) return(NULL);
   return((VTClasse*)lisCLASSE->First());
   }

//---------------------------------------------------------------------------
void __fastcall TClasses::InsereClasse(VTClasse *classe)
   {
   //verifica se a Classe já existe na lista
   if (ExisteClasseTag(classe->Tag))
      {//destrói objeto
      delete classe;
      return;
      }
   //verifica se a Classe já existe na lista
   if (lisCLASSE->IndexOf(classe) < 0) lisCLASSE->Add(classe);
   }

//---------------------------------------------------------------------------
TList* __fastcall TClasses::LisClasse(void)
   {
   return(lisCLASSE);
   }

//---------------------------------------------------------------------------
//eof



