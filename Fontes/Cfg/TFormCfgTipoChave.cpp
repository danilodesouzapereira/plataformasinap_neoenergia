//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCfgTipoChave.h"
#include "TFormEdtTipoChave.h"
#include "TAplCfg.h"
//#include "..\BD\VTBD.h"
//#include "..\BD\Rede\VTBDGeral.h"
//#include "..\BD\VTSNPDBConn.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTTipos.h"
#include "..\..\DLL_Inc\BD.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCfgTipoChave::TFormCfgTipoChave(TComponent* Owner, VTApl *apl_owner)
   : TForm(Owner)
   {
	//cria objeto AplCfg
	apl  = new TAplCfg(this, apl_owner);
	//salva ponteiro com tipos //DVK 2015.10.01
	tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
   //l� TipoChave da base de dados
   apl->LeBaseTipoChave();
   //inicia LView
   LViewChvInicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormCfgTipoChave::~TFormCfgTipoChave(void)
   {
   //destr�i objetos
	if (apl) {delete apl; apl = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTipoChave::ActionCancelaExecute(TObject *Sender)
   {
   //vari�veis locais
	VTTipoChave *tipochave;
	//DVK 2015.10.01 comentado
//	VTTipos     *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList       *lisTIPO = tipos->LisTipoChave();

   //elimina todos TipoChave criados
   for (int n = lisTIPO->Count-1; n >= 0; n--)
      {
      tipochave = (VTTipoChave*)lisTIPO->Items[n];
      if (tipochave->Status[sttNOVO])
         {//destr�i objeto
         lisTIPO->Delete(n);
         delete tipochave;
         }
      else
         {//atualiza Status do TipoChave
         tipochave->Status[sttALTERADO] = false;
         tipochave->Status[sttNOVO]     = false;
         tipochave->Status[sttEXCLUIDO] = false;
         }
      }
   //fecha o Form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTipoChave::ActionConfirmaExecute(TObject *Sender)
   {
	//vari�veis locais
	VTTipoChave *tipochave;
	//DVK 2015.10.01 comentado
//	VTTipos     *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList       *lisTIPO = tipos->LisTipoChave();
   /*
   //atualiza os TipoChave que foram alterados
   for (int row = 1; row < SGrid->RowCount; row++)
      {
      tipochave  = (VTTipoChave*)SGrid->Objects[0][row];
      if (tipochave->Codigo.AnsiCompareIC(SGrid->Cells[0][row]) != 0)
         {
         tipochave->Codigo = SGrid->Cells[0][row];
         tipochave->Status[sttALTERADO] = true;
         }
      } */
   //elimina todos TipoChave criados e exclu�dos
   for (int n = lisTIPO->Count-1; n >= 0; n--)
      {
      tipochave = (VTTipoChave*)lisTIPO->Items[n];
      if ((tipochave->Status[sttNOVO])&&(tipochave->Status[sttEXCLUIDO]))
         {//destr�i objeto
         lisTIPO->Delete(n);
         delete tipochave;
         }
      }
   // Salva na base de dados
	SalvaBaseDados();

   //fecha o Form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTipoChave::ActionInsereExecute(TObject *Sender)
   {
   //vari�veis locais
	VTTipoChave       *tipochave;
	//DVK 2015.10.01 comentado
//	VTTipos           *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));
   TFormEdtTipoChave *form;

   //determina tipochave selecionado
//   if (LViewChv->Selected == NULL) return;
//   if ((tipochave = (VTTipoChave*)LViewChv->Selected->Data) == NULL) return;
   //esconde este form
   Visible = false;
   try{//cria um novo tipo de chave e insere em Tipos
      tipochave                  = DLL_NewObjTipoChave();
//      tipochave->Extern_id       = NULL;
      tipochave->Tipo            = NextValorTipo();
      tipochave->Codigo          = NextCodigo();
      tipochave->Operacao        = chaveOP_INDEFINIDA;
      tipochave->Color           = clBlue;
      tipochave->Status[sttNOVO] = true;
      tipos->InsereTipoChave(tipochave);
      //cria TFormEdtTipoChave como janela modal
      form = new TFormEdtTipoChave(this, apl, tipochave);
      if (form->ShowModal() == mrOk)
         {//atualiza LViewChv
         LViewChvInicia();
         }
      delete form;
      }catch (Exception &e)
         {
         }
   //reapresenta este form
   Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTipoChave::ActionRetiraExecute(TObject *Sender)
   {/*
   //vari�veis locais
   int         row = SGrid->Row;
   VTTipoChave *tipochave;

   //prote��o
   if ((row <= 0)||(row >= SGrid->RowCount)) return;
   //determina TipoChave associado � linha
   tipochave  = (VTTipoChave*)SGrid->Objects[0][row];
   //verifica se � um tTipoChave reservado
   if (tipochave->Tipo <= chaveINDEFINIDA)
      {//TipoChave n�o pode ser exclu�do
      Aviso("Este tipo de chave n�o pode ser exclu�do");
      }
   else
      {//marca que o TipoChave est� exclu�do
      tipochave->Status[sttALTERADO] = true;
      tipochave->Status[sttEXCLUIDO] = true;
      //reinicia SGrid
      SGridInicia();
      }
      */
   //vari�veis locais
   VTTipoChave *tipochave;

   //determina tipochave selecionado
   if (LViewChv->Selected == NULL) return;
   if ((tipochave = (VTTipoChave*)LViewChv->Selected->Data) == NULL) return;
   //verifica se � um TipoChave reservado
   if (tipochave->Tipo <= chaveINDEFINIDA)
      {//TipoChave n�o pode ser exclu�do
      Aviso("Este tipo de chave n�o pode ser exclu�do");
      return;
      }
   //pede confirma��o do usu�rio
   if (Confirma("Tipo chave " + AnsiQuotedStr(tipochave->Codigo, '\''),
         "Confirma exclus�o ?") != IDYES) return;
   //marca que o TipoChave est� exclu�do
   tipochave->Status[sttALTERADO] = true;
   tipochave->Status[sttEXCLUIDO] = true;
   //reinicia LViewChv
   LViewChvInicia();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgTipoChave::ExisteTipoChave(AnsiString codigo)
   {
   //vari�veis locais
	VTTipoChave *tipochave;
	//DVK 2015.10.01 comentado
//   VTTipos     *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList       *lisTIPO = tipos->LisTipoChave();

   //procura TipoChave na lista de Tipos
   for (int n = 0; n < lisTIPO->Count; n++)
      {
      tipochave = (VTTipoChave*)lisTIPO->Items[n];
      if (codigo.AnsiCompareIC(tipochave->Codigo) == 0) return(true);
      }
      /*
   //procura TipoChave no LViewChv (o usu�rio pode ter alterado o c�digo)
   for (int row = 1; row < LViewChv->RowCount; row++)
      {
      //tipochave  = (VTTipoChave*)LViewChv->Objects[0][row];
      if (codigo.AnsiCompareIC(LViewChv->Cells[0][row]) == 0) return(true);
      }  */
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTipoChave::FormResize(TObject *Sender)
   {
   //redefine tamanho da coluna
//   LViewChv->DefaultColWidth = ClientWidth - 24;
   LViewChv->Width = ClientWidth - 290;//24;
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormCfgTipoChave::NextCodigo(void)
   {
   //vari�veis locais
   int        count = 1;
   AnsiString codigo;

   do {codigo = "Novo tipo chave " + IntToStr(count++);
      }while(ExisteTipoChave(codigo));
   return(codigo);
   }

//---------------------------------------------------------------------------
int __fastcall TFormCfgTipoChave::NextValorTipo(void)
   {
   //vari�veis locais
   int         next_tipo = chaveINDEFINIDA;
	VTTipoChave *tipochave;
	//DVK 2015.10.01 comentado
//	VTTipos     *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList       *lisTIPO = tipos->LisTipoChave();

   for (int n = 0; n < lisTIPO->Count; n++)
      {
      tipochave = (VTTipoChave*)lisTIPO->Items[n];
      if (tipochave->Tipo > next_tipo) next_tipo = tipochave->Tipo;
      }
  return(next_tipo+1);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTipoChave::SalvaBaseDados(void)
   {
   apl->SalvaBaseTipoChave();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTipoChave::LViewChvCustomDrawSubItem(
      TCustomListView *Sender, TListItem *Item, int SubItem,
      TCustomDrawState State, bool &DefaultDraw)
   {
   //vari�veis locais
   VTTipoChave *tipochave = (VTTipoChave*)Item->Data;

   //prote��o
   if (tipochave == NULL) return;
   //preenche coluna Cor
   if (SubItem == (Item->SubItems->Count + 1))
      {
      LViewChv->Canvas->Brush->Color = tipochave->Color;
      LViewChv->Canvas->Brush->Style = bsSolid;
      }
   else
      {
      LViewChv->Canvas->Brush->Color = clWindow;
      LViewChv->Canvas->Brush->Style = bsSolid;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTipoChave::LViewChvDblClick(TObject *Sender)
   {
   //variaveis locais
   VTTipoChave *tipochave;
   TFormEdtTipoChave *form;

   //determina tipochave selecionado
   if (LViewChv->Selected == NULL) return;
   if ((tipochave = (VTTipoChave*)LViewChv->Selected->Data) == NULL) return;
   //esconde este form
   Visible = false;
   //cria TFormEdtTipoChave como janela modal
   form = new TFormEdtTipoChave(this, apl, tipochave);
   if (form->ShowModal() == mrOk)
      {//atualiza LViewChv
      LViewChvInicia();
      }
   delete form;
   //reapresenta este form
   Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTipoChave::LViewChvInicia(void)
   {
   //vari�veis locais
   AnsiString  operacao;
	VTTipoChave *tipochave;
	//DVK 2015.10.01 comentado
//	VTTipos     *tipos   = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList       *lisTIPO = tipos->LisTipoChave();
   TListItem   *item;

   //reinicia linhas do LViewChv
	LViewChv->Clear();
   //insere TipoChave
   for (int n = 0; n < lisTIPO->Count; n++)
      {
      tipochave = (VTTipoChave*)lisTIPO->Items[n];
      //verifica se � um TipoChave exclu�do
      if (tipochave->Status[sttEXCLUIDO]) continue;
      item          = LViewChv->Items->Add();
      item->Data    = tipochave;
      item->Caption = tipochave->Codigo;
      switch(tipochave->Operacao)
         {
         case chaveOP_MANUAL:        {operacao = "Manual";          break;}
         case chaveOP_TELECOMANDADA: {operacao = "Telecomandada";   break;}
         case chaveOP_AUTOMATICA:    {operacao = "Autom�tica";      break;}
         default:                    operacao = "Indefinida";
         }
      item->SubItems->Add(operacao);
      }
   }

//---------------------------------------------------------------------------
//eof



