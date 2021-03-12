//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TGrupo.h"
#include "VTFasor.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTGrupo* __fastcall NewObjGrupo(void)
   {
   try{//cria objeto TGrupo
      return(new TGrupo());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
//---------------------------------------------------------------------------
__fastcall TGrupo::TGrupo(void)
   {
   try{//cria lista de objetos TFasor
      lisFASOR = new TList();
      //inicia dados
      Visible = false;
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TGrupo::~TGrupo(void)
   {
   //destrói lista e seus objetos
   if (lisFASOR) {LimpaTList(lisFASOR); delete lisFASOR; lisFASOR = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TGrupo::ExisteFasorEnabled(void)
   {
   //variáveis locais
   VTFasor *fasor;

   for (int n = 0; n < lisFASOR->Count; n++)
      {
      fasor = (VTFasor*)lisFASOR->Items[n];
      if (fasor->enabled) return(true);
      }
   return(false);
   }
   
//---------------------------------------------------------------------------
bool __fastcall TGrupo::InsereFasor(VTFasor *fasor)
   {
   //proteção
   if (fasor == NULL) return(false);
   //insere fasor na lista
   if (lisFASOR->IndexOf(fasor) < 0)
      {
      lisFASOR->Add(fasor);
      }
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TGrupo::LisFasor(void)
   {
   return(lisFASOR);
   }

//---------------------------------------------------------------------------
void __fastcall TGrupo::LisFasor(TList *lisEXT)
   {
   CopiaTList(lisFASOR, lisEXT);
   }

//---------------------------------------------------------------------------
//eof
