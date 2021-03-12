//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtTipoChave.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTTipoChave.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEdtTipoChave::TFormEdtTipoChave(TComponent* Owner, VTApl *apl, VTTipoChave *tipochave)
                             :TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl           = apl;
   this->TipoChaveOrig = tipochave;
   //cria clone do tipochave
   this->tipochave = TipoChaveOrig->Clone();
   Inicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtTipoChave::~TFormEdtTipoChave(void)
   {
   //destr�i objetos criados
   if (tipochave) {delete tipochave; tipochave = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTipoChave::ActionCancelaExecute(TObject *Sender)
   {
   //exclui tipochave novo
   if (tipochave->Status[sttNOVO]) tipochave->Status[sttEXCLUIDO] = true;
   //fecha o form
   ModalResult = mrCancel;
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormEdtTipoChave::ActionConfirmaExecute(TObject *Sender)
   {
	//l� todos os dados
	LeDados();
   //atualiza status indicando que o tipochave foi alterado
   if(! tipochave->Status[sttNOVO])
      {
      tipochave->Status[sttALTERADO] = true;
      }
   //atualiza tipochave original
   TipoChaveOrig->CopiaAtributosDe(*tipochave);
   //fecha o form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTipoChave::ActionCorExecute(TObject *Sender)
   {
   Application->ModalPopupMode = pmAuto;
   //executa ColorDialog
   if(ColorDialog->Execute())
      {//altera para a cor selecionada
      PanelColor->Color = ColorDialog->Color;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTipoChave::Inicia(void)
   {
   //l� c�digo do tipochave
   EditTipoChv->Text = tipochave->Codigo;
   //l� modo de opera��o
   CBoxOperacao->Items->Add("Manual");
   CBoxOperacao->Items->Add("Telecomandada");
   CBoxOperacao->Items->Add("Autom�tica");
   CBoxOperacao->Items->Add("Indefinida");
   switch(tipochave->Operacao)
         {
         case chaveOP_MANUAL:          {CBoxOperacao->ItemIndex = 0;   break;}
         case chaveOP_TELECOMANDADA:   {CBoxOperacao->ItemIndex = 1;   break;}
         case chaveOP_AUTOMATICA:      {CBoxOperacao->ItemIndex = 2;   break;}
         default:                      CBoxOperacao->ItemIndex = 3;
         }
   //l� cor da chave
   PanelColor->Color = tipochave->Color;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTipoChave::LeDados(void)
   {
   //l� c�digo do tipochave
   tipochave->Codigo = EditTipoChv->Text;
   //l� modo de opera��o
   switch(CBoxOperacao->ItemIndex)
         {
         case 0:  {tipochave->Operacao = chaveOP_MANUAL;          break;}
         case 1:  {tipochave->Operacao = chaveOP_TELECOMANDADA;   break;}
         case 2:  {tipochave->Operacao = chaveOP_AUTOMATICA;      break;}
         default: tipochave->Operacao = chaveOP_INDEFINIDA;    
         }
   //l� cor da chave
   tipochave->Color = PanelColor->Color;
   }

//---------------------------------------------------------------------------


