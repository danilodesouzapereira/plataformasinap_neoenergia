//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TSpy.h"
#include "..\Forms\TFormEqpto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função para criar objeto VTSpy
//---------------------------------------------------------------------------
VTSpy* __fastcall NewObjSpy(VTApl *AplOwner)
   {
   try{
      return(new TSpy(AplOwner));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TSpy::TSpy(VTApl *AplOwner)
   {
   //salva ponteiro p/ objeto externo
   apl = AplOwner;
   }

//---------------------------------------------------------------------------
__fastcall TSpy::~TSpy(void)
   {
   //não destrói TFormEqpto
   //if (FormEqpto) {delete FormEqpto; FormEqpto=NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TSpy::ExibeFormEqpto(TComponent *Owner, TWinControl *parent)
   {
   //verifica se já existe TFormEqpto
   if (FormEqpto == NULL)
      {//cria TFormEqpto
      FormEqpto = new TFormEqpto(Owner, parent, apl);
      }
   FormEqpto->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TSpy::DefineEqpto(VTEqpto *eqpto)
   {
   if (FormEqpto != NULL) FormEqpto->DefineEqpto(eqpto);
   }

//---------------------------------------------------------------------------
//eof
