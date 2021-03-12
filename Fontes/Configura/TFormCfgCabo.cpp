//---------------------------------------------------------------------------
#include <vcl.h>
#include "TFormParCabo.h"
#include "TFormCfgCabo.h"
#include "..\SubTrans\TCabo.h"
#include "..\DLL_Stub\Basico.h"
#include "..\BDado\VTBDado.h"
#include "..\Sisplan\DirArq.h"
#include "..\Sisplan\Funcao.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TformCfgCabo::TformCfgCabo(TComponent* Owner, AnsiString dirBase, AnsiString dirRede)
	: TForm(Owner)
	{

   //variáveis locais
   AnsiString dirGeral;

   //cria objeto BDado
   bdado = DLL_NewObjBDado();
   //bdado = NewObjBDado();
   bdado->DefDiretorio(dirBase, dirRede);

   lisCabos = new TList();
   CopiaTList(bdado->LeCabo(), lisCabos);

	}
//-------------------------------------------------------------------------
__fastcall TformCfgCabo::~TformCfgCabo(void)
   {

   //destrói objeto criado
   if (bdado) { delete bdado; bdado = NULL; }
   if (lisCabos){delete lisCabos; lisCabos = NULL;}

   }

//---------------------------------------------------------------------------
void __fastcall TformCfgCabo::ActionAdicionaExecute(TObject *Sender)
   {

   TCabo* newCabo;
   newCabo= new TCabo;

   TformParCabo* formParCabo = new TformParCabo(this, newCabo);

   if (formParCabo->ShowModal() == mrOk)
      {
      newCabo->id=-1;

      if (newCabo) lisCabos->Add(newCabo);

      loadCabos();
      }


   delete formParCabo;
   formParCabo=NULL;

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgCabo::ActionCancelaExecute(TObject *Sender)
   {

   ModalResult=mrCancel;

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgCabo::ActionConfirmaExecute(TObject *Sender)
   {

   if(bdado->ReinitTabCabo(lisCabos))
      {
      bdado->AtualizaCaboID();
      ModalResult = mrOk;
      }

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgCabo::ActionDesfazExecute(TObject *Sender)
   {

   lisCabos=bdado->LeCabo();
   loadCabos();

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgCabo::ActionEditaExecute(TObject *Sender)
   {

  if (lvwCabos->SelCount != 1) return;

  TformParCabo* formParCabo = new TformParCabo(this, ((TCabo*)lvwCabos->Selected->Data));

  formParCabo->ShowModal();

  delete formParCabo;
  formParCabo=NULL;

  loadCabos();

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgCabo::ActionRemoveExecute(TObject *Sender)
   {

   //remove cabo da lista
   if (lvwCabos->SelCount != 1) return;
   lisCabos->Remove((TCabo*)lvwCabos->Selected->Data);
   loadCabos();

   }
//-------------------------------------------------------------------------
/*
void __fastcall TformCfgCabo::dbgCaboDrawColumnCell(TObject *Sender,
      const TRect &Rect, int DataCol, TColumn *Column,
      TGridDrawState State)
   {

   //variáveis locais
   TDBGrid     *dbgCabos = (TDBGrid*)Sender;
   TCanvas     *pCanvas  = dbgCabos->Canvas;
   TColor      color_ori;
   TBrushStyle style_ori;

   if (Column->FieldName != "COLOR")
      {//apresenta valor da célula
      dbgCabos->DefaultDrawColumnCell(Rect, DataCol, Column, State);
      return;
      }
   //apresenta a cor do Cabo
   //salva cor/style original
   color_ori = pCanvas->Brush->Color;
   style_ori = pCanvas->Brush->Style;
   pCanvas->Brush->Color =(TColor)Column->Field->AsInteger;
   pCanvas->Brush->Style = bsSolid;
   pCanvas->FillRect(Rect);  //desenha o retângulo da cor

   //restaura cor/style original
   pCanvas->Brush->Color = color_ori;
   pCanvas->Brush->Style = style_ori;

   }
*/
//---------------------------------------------------------------------------
void __fastcall TformCfgCabo::FormActivate(TObject *Sender)
   {

   loadCabos();

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgCabo::loadCabos(void)
   {

   TCabo   *tmpCabo;

   lvwCabos->Clear();

   for(int numReg=0; numReg < lisCabos->Count; numReg++)
      {
      tmpCabo=(TCabo *)lisCabos->Items[numReg];
      tmpCabo->nome;
      lvwCabos->Items->Add();
      lvwCabos->Items->Item[numReg]->Data=tmpCabo;
      lvwCabos->Items->Item[numReg]->Caption = tmpCabo->codigo;
      lvwCabos->Items->Item[numReg]->SubItems->Add(tmpCabo->iadm);
      lvwCabos->Items->Item[numReg]->SubItems->Add(tmpCabo->r0);
      lvwCabos->Items->Item[numReg]->SubItems->Add(tmpCabo->x0);
      lvwCabos->Items->Item[numReg]->SubItems->Add(tmpCabo->c0);
      lvwCabos->Items->Item[numReg]->SubItems->Add(tmpCabo->r1);
      lvwCabos->Items->Item[numReg]->SubItems->Add(tmpCabo->c1);
      }
   }
//---------------------------------------------------------------------------
void __fastcall TformCfgCabo::lvwCabosAdvancedCustomDrawSubItem(
      TCustomListView *Sender, TListItem *Item, int SubItem,
      TCustomDrawState State, TCustomDrawStage Stage, bool &DefaultDraw)
   {

   if (SubItem==7)
      lvwCabos->Canvas->Brush->Color= ((TCabo *)Item->Data)->color;
   else
      lvwCabos->Canvas->Brush->Color= clWhite;

   }
//---------------------------------------------------------------------------

