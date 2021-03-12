//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCfgTipoRede.h"
#include "TFormEdtTipoRede.h"
#include "TAplCfg.h"
//#include "..\BD\VTBD.h"
//#include "..\BD\Rede\VTBDGeral.h"
//#include "..\BD\VTSNPDBConn.h"
#include "..\Geral\VTGeral.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTTipos.h"
//#include "..\..\DLL_Inc\BD.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCfgTipoRede::TFormCfgTipoRede(TComponent* Owner, VTApl *apl_owner)
   : TForm(Owner)
   {
	//cria objeto AplCfg
   apl  = new TAplCfg(this, apl_owner);
   //lê TipoRede da base de dados
   apl->LeBaseTipoRede();
   //inicia LView
   LViewRedeInicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormCfgTipoRede::~TFormCfgTipoRede(void)
   {
   //destrói objetos
	if (apl) {delete apl; apl = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTipoRede::ActionCancelaExecute(TObject *Sender)
   {
   //variáveis locais
   VTTipoRede *tiporede;
   VTTipos     *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));
   TList       *lisTIPO = tipos->LisTipoRede();

   //elimina todos TipoRede criados
   for (int n = lisTIPO->Count-1; n >= 0; n--)
      {
      tiporede = (VTTipoRede*)lisTIPO->Items[n];
      if (tiporede->Status[sttNOVO])
         {//destrói objeto
         lisTIPO->Delete(n);
         delete tiporede;
         }
      else
         {//atualiza Status do TipoRede
         tiporede->Status[sttALTERADO] = false;
         tiporede->Status[sttNOVO]     = false;
         tiporede->Status[sttEXCLUIDO] = false;
         }
      }
   //fecha o Form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTipoRede::ActionConfirmaExecute(TObject *Sender)
   {
   //variáveis locais
   VTTipoRede *tiporede;
   VTTipos     *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));
   TList       *lisTIPO = tipos->LisTipoRede();
   /*
   //atualiza os TipoRede que foram alterados
   for (int row = 1; row < LViewRede->RowCount; row++)
      {
      tiporede  = (VTTipoRede*)LViewRede->Objects[0][row];
      if (tiporede->Codigo.AnsiCompareIC(LViewRede->Cells[0][row]) != 0)
         {
         tiporede->Codigo = LViewRede->Cells[0][row];
         tiporede->Status[sttALTERADO] = true;
         }
      }*/
   //elimina todos TipoRede criados e excluídos
   for (int n = lisTIPO->Count-1; n >= 0; n--)
      {
      tiporede = (VTTipoRede*)lisTIPO->Items[n];
      if ((tiporede->Status[sttNOVO])&&(tiporede->Status[sttEXCLUIDO]))
         {//destrói objeto
         lisTIPO->Delete(n);
         delete tiporede;
         }
      }
   //atualiza base de dados
   SalvaBaseDados();

   //fecha o Form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTipoRede::ActionInsereExecute(TObject *Sender)
   {
   //variáveis locais
   VTTipoRede        *tiporede;
   VTTipos           *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));
   TFormEdtTipoRede  *form;

   //esconde este form
   Visible = false;
   try{//cria um novo tipo de chave e insere em Tipos
      tiporede                  = DLL_NewObjTipoRede();
      tiporede->Segmento        = NextValorSegmento();
      tiporede->Codigo          = NextCodigo();
      tiporede->Status[sttNOVO] = true;
      tipos->InsereTipoRede(tiporede);
      //cria TFormEdtTipoChave como janela modal
      form = new TFormEdtTipoRede(this, apl, tiporede);
      if (form->ShowModal() == mrOk)
         {//atualiza LViewRede
         LViewRedeInicia();
         }
      delete form;
      }catch(Exception &e)
         {
         }
   //reapresenta este form
   Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTipoRede::ActionRetiraExecute(TObject *Sender)
   { /*
   //variáveis locais
   int         row = LViewRede->Row;
   VTTipoRede *tiporede;

   //proteção
   if ((row <= 0)||(row >= LViewRede->RowCount)) return;
   //determina TipoRede associado à linha
   tiporede  = (VTTipoRede*)LViewRede->Objects[0][row];
   //verifica se é um tTipoRede reservado
   if (tiporede->Segmento <= redeDEFINIDA_USUARIO)
      {//TipoRede não pode ser excluído
      Aviso("Este tipo de rede não pode ser excluído");
      }
   else
      {//marca que o TipoRede está excluído
      tiporede->Status[sttALTERADO] = true;
      tiporede->Status[sttEXCLUIDO] = true;
      //reinicia LViewRede
      LViewRedeInicia();
      }
      */
   //variáveis locais
   VTTipoRede *tiporede;

   //determina tiporede selecionado
   if (LViewRede->Selected == NULL) return;
   if ((tiporede = (VTTipoRede*)LViewRede->Selected->Data) == NULL) return;
   //verifica se é um tiporede reservado
   if (tiporede->Segmento < redeDEFINIDA_USUARIO)
      {//TipoRede não pode ser excluído
      Aviso("Este tipo de rede não pode ser excluído");
      return;
      }
   //pede confirmação do usuário
   if (Confirma("Tipo rede " + AnsiQuotedStr(tiporede->Codigo, '\''),
         "Confirma exclusão ?") != IDYES) return;
   //marca que o tiporede está excluído
   tiporede->Status[sttALTERADO] = true;
   tiporede->Status[sttEXCLUIDO] = true;
   //reinicia LViewRede
   LViewRedeInicia();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgTipoRede::ExisteTipoRede(AnsiString codigo)
   {
   //variáveis locais
   VTTipoRede  *tiporede;
   VTTipos     *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));
   TList       *lisTIPO = tipos->LisTipoRede();

   //procura TipoRede na lista
   for (int n = 0; n < lisTIPO->Count; n++)
      {
      tiporede = (VTTipoRede*)lisTIPO->Items[n];
      if (codigo.AnsiCompareIC(tiporede->Codigo) == 0) return(true);
      } /*
   //procura TipoRede no LViewRede (o usuário pode ter alterado o código)
   for (int row = 1; row < LViewRede->RowCount; row++)
      {
      //tiporede  = (VTTipoRede*)LViewRede->Objects[0][row];
      if (codigo.AnsiCompareIC(LViewRede->Cells[0][row]) == 0) return(true);
      }  */
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTipoRede::FormResize(TObject *Sender)
   {
   //redefine tamanho da coluna
//   LViewRede->DefaultColWidth = ClientWidth - 24;
   LViewRede->Width = ClientWidth - 290;//24
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormCfgTipoRede::NextCodigo(void)
   {
   //variáveis locais
   int        count = 1;
   AnsiString codigo;

   do {codigo = "Novo tipo rede " + IntToStr(count++);
      }while(ExisteTipoRede(codigo));
   return(codigo);
   }

//---------------------------------------------------------------------------
int __fastcall TFormCfgTipoRede::NextValorSegmento(void)
   {
   //variáveis locais
   int         next_seg = redeDEFINIDA_USUARIO;
   VTTipoRede  *tiporede;
   VTTipos     *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));
   TList       *lisTIPO = tipos->LisTipoRede();

   for (int n = 0; n < lisTIPO->Count; n++)
      {
      tiporede = (VTTipoRede*)lisTIPO->Items[n];
      if (tiporede->Segmento > next_seg) next_seg = tiporede->Segmento;
      }
   return(next_seg+1);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTipoRede::SalvaBaseDados(void)
   {
   apl->SalvaBaseTipoRede();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTipoRede::LViewRedeCustomDrawSubItem(
      TCustomListView *Sender, TListItem *Item, int SubItem,
      TCustomDrawState State, bool &DefaultDraw)
   {
   //variáveis locais
   VTTipoRede *tiporede = (VTTipoRede*)Item->Data;

   //proteção
   if (tiporede == NULL) return;
   //preenche coluna Cor
   if (SubItem == (Item->SubItems->Count + 1))
      {
      LViewRede->Canvas->Brush->Color = tiporede->Color;
      LViewRede->Canvas->Brush->Style = bsSolid;
      }
   else
      {
      LViewRede->Canvas->Brush->Color = clWindow;
      LViewRede->Canvas->Brush->Style = bsSolid;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTipoRede::LViewRedeDblClick(TObject *Sender)
   {
   //variaveis locais
   VTTipoRede *tiporede;
   TFormEdtTipoRede *form;

   //determina tiporede selecionado
   if (LViewRede->Selected == NULL) return;
   if ((tiporede = (VTTipoRede*)LViewRede->Selected->Data) == NULL) return;
   //esconde este form
   Visible = false;
   //cria TFormEdtTipoRede como janela modal
   form = new TFormEdtTipoRede(this, apl, tiporede);
   if (form->ShowModal() == mrOk)
      {//atualiza LViewRede
      LViewRedeInicia();
      }
   delete form;
   //reapresenta este form
   Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTipoRede::LViewRedeInicia(void)
   {
   //variáveis locais
   VTTipoRede *tiporede;
   VTTipos     *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));
   TList       *lisTIPO = tipos->LisTipoRede();
   TListItem   *item;

   //reinicia linhas do LViewRede
   LViewRede->Clear();
   //insere tiporede
   for (int n = 0; n < lisTIPO->Count; n++)
      {
      tiporede = (VTTipoRede*)lisTIPO->Items[n];
      //verifica se é um tiporede excluído
      if (tiporede->Status[sttEXCLUIDO]) continue;
      item          = LViewRede->Items->Add();
      item->Data    = tiporede;
      item->Caption = tiporede->Codigo;
      item->SubItems->Add(tiporede->Descricao);
      }
   }

//---------------------------------------------------------------------------
//eof


