//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtTipoRede.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEdtTipoRede::TFormEdtTipoRede(TComponent* Owner, VTApl *apl, VTTipoRede *tiporede)
                             :TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl           = apl;
   this->TipoRedeOrig = tiporede;
   //cria clone do tiporede
   this->tiporede = TipoRedeOrig->Clone();
   Inicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtTipoRede::~TFormEdtTipoRede(void)
   {
   //destrói objetos criados
   if (tiporede) {delete tiporede; tiporede = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTipoRede::ActionCancelaExecute(TObject *Sender)
   {
   //exclui tipochave novo
   if (tiporede->Status[sttNOVO]) tiporede->Status[sttEXCLUIDO] = true;
   //fecha o form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTipoRede::ActionConfirmaExecute(TObject *Sender)
   {
	//lê todos os dados
	LeDados();
   //atualiza status indicando que o tiporede foi alterado
   if(! tiporede->Status[sttNOVO])
      {
      tiporede->Status[sttALTERADO] = true;
      }
   //atualiza tiporede original
   TipoRedeOrig->CopiaAtributosDe(*tiporede);
   //fecha o form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTipoRede::ActionCorExecute(TObject *Sender)
   {
   Application->ModalPopupMode = pmAuto;
   //executa ColorDialog
   if(ColorDialog->Execute())
      {//altera para a cor selecionada
      PanelColor->Color = ColorDialog->Color;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTipoRede::Inicia(void)
   {
   //lê código do tiporede
   EditTipoRede->Text = tiporede->Codigo;
   //lê descrição
   EditRedeDescr->Text = tiporede->Descricao;
   //lê cor da rede
   PanelColor->Color = tiporede->Color;
   }
 
//---------------------------------------------------------------------------
void __fastcall TFormEdtTipoRede::LeDados(void)
   {
   //lê código do tiporede
   tiporede->Codigo = EditTipoRede->Text;
   //lê descrição
    tiporede->Descricao = EditRedeDescr->Text;
   //lê cor da rede
   tiporede->Color = PanelColor->Color;
   }

//---------------------------------------------------------------------------
