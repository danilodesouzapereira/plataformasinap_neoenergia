//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TStkPatamar.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Patamar\VTPatamar.h>
#include <Fontes\Patamar\VTPatamares.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTStkPatamar* __fastcall NewObjStkPatamar(VTApl *apl_owner)
   {
   try{//cria objeto
      return(new TStkPatamar(apl_owner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TStkPatamar::TStkPatamar(VTApl *apl_owner)
	{
   //salva ponteiro para objeto
   this->apl = apl_owner;
	//obtém informação da memória
	}

//---------------------------------------------------------------------------
__fastcall TStkPatamar::~TStkPatamar(void)
   {
	//nada a fazer
	}

//---------------------------------------------------------------------------
bool __fastcall TStkPatamar::IniciaCBox(TComboBox *cbox, bool opcao_todos)
   {
   //variáveis locais
   TList       *lisPAT;
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //proteção
   if (patamares == NULL) return(false);
   //reinicia CBoxPatamar c/ Patamares
   cbox->Clear();
   //verifica se deve incluir a opção "todos"
   if (opcao_todos)
      {
      cbox->Items->AddObject("todos", NULL);
      }
   //loop para todos patamares
   lisPAT = patamares->LisPatamar();
   for (int np = 0; np < lisPAT->Count; np++)
      {
      patamar = (VTPatamar*)lisPAT->Items[np];
      if (patamar->Enabled)
         {
         cbox->Items->AddObject(patamar->Nome, patamar);
         }
      }
   //pré-seleciona primeiro patamar
   if (cbox->Items->Count > 0) cbox->ItemIndex = 0;

   return(true);
   }


//-----------------------------------------------------------------------------
//eof



