//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormPerdaComercial.h"
#include "Medicao\TMedicao.h"
#include "Medicao\VTMedicoes.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTRede.h"
#include "..\Report\VTReport.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Report.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;
                    
//---------------------------------------------------------------------------
__fastcall TFormPerdaComercial::TFormPerdaComercial(TComponent* Owner, VTApl *apl_owner, TWinControl *parent)
                         : TForm(Owner)
   {
   //insere Form no seu Parent, se este for definido
   if (parent) Parent = parent;
   //salva ponteiro para objetos
   this->apl = apl_owner;
   //inicia controle da ordena��o dos LView
   LViewSort.column     = LViewMedicao->Column[0];
   LViewSort.ascendente = true;
   }

//---------------------------------------------------------------------------
__fastcall TFormPerdaComercial::~TFormPerdaComercial(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   }

//---------------------------------------------------------------------------
void __fastcall TFormPerdaComercial::ActionFecharExecute(TObject *Sender)
   {
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormPerdaComercial::ActionSalvaRelatorioExecute(TObject *Sender)
   {
   //vari�veis locais
   bool     append = false;
   VTReport *report;
   VTPath   *path  = (VTPath*)apl->GetObject(__classid(VTPath));

   try{//verifica se o arquivo j� existe
      //define diret�rio inicial
      if (SaveDialog->InitialDir.IsEmpty()) SaveDialog->InitialDir = path->DirExporta();
      //exibe OpenTextFileDialog
      if (! SaveDialog->Execute()) return;
      //cria objeto VTReport
      report = DLL_NewObjReport(apl);
      //exporta ListView
      report->ArquivoTexto(SaveDialog->FileName, LViewMedicao, Caption, append);
      //destr�i objeto
      delete report;
      }catch(Exception &e)
         {
         Erro("N�o foi poss�vel gravar o arquivo " + ExtractFileName(SaveDialog->FileName));
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormPerdaComercial::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormPerdaComercial::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form canto superior esquerdo
   Top  = 0;
   Left = 0;;
   }

//---------------------------------------------------------------------------
void __fastcall TFormPerdaComercial::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   //desabilita tratamento deste evento novamente
   OnShow = NULL;
   }

//---------------------------------------------------------------------------
void __fastcall TFormPerdaComercial::Inicia(void)
   {
   //inicia LView
   LViewMedicaoInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormPerdaComercial::LViewMedicaoColumnClick(TObject *Sender, TListColumn *Column)
   {
   //verifica se selecionou a mesma coluna
   if (LViewSort.column == Column)
      {//inverte ordem
      LViewSort.ascendente = ! LViewSort.ascendente;
      }
   else
      {//redefine �ndice da coluna usada p/ ordena��o
      LViewSort.column     = Column;
      LViewSort.ascendente = true;
      }
   //for�a reordena��o
   LViewMedicao->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormPerdaComercial::LViewMedicaoCompare(TObject *Sender, TListItem *Item1,
                                                   TListItem *Item2, int Data, int &Compare)
   {
   //vari�veis locais
   int       index = LViewSort.column->Index - 1;
   double    valor1, valor2;

   //verifica Coluna sendo ordenada
   if (LViewSort.column->Index == 0)
      {//compara Caption
      Compare = CompareText(Item1->Caption,Item2->Caption);
      }
   //todas as demais colunas s�o num�ricas
   else
      {//compara��o de colunas com valor num�rico
      valor1  = StrToDouble(Item1->SubItems->Strings[index]);
      valor2  = StrToDouble(Item2->SubItems->Strings[index]);
      if      (valor1 < valor2) Compare = -1;
      else if (valor1 > valor2) Compare =  1;
      else                      Compare =  0;
      }
   //verifica se ordem ascendente/descendente
   if (! LViewSort.ascendente) Compare = - Compare;
   }

//---------------------------------------------------------------------------
void __fastcall TFormPerdaComercial::LViewMedicaoInicia(void)
   {
   //vari�veis locais
   TListItem  *Item;
   TMedicao   *medicao;
   VTMedicoes *medicoes   = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
   TList      *lisMEDICAO = medicoes->LisMedicao();

   //reinicia LViewMedicao
   LViewMedicao->Clear();
   //atualiza LViewMedicao
   for (int n = 0; n < lisMEDICAO->Count; n++)
      {
      medicao       = (TMedicao*)lisMEDICAO->Items[n];
      Item          = LViewMedicao->Items->Add();
      Item->Data    = medicao;
//      Item->Caption = medicao->rede->Codigo;  //DVK 2016.05.25
      Item->Caption = medicao->cod_rede;
	  Item->SubItems->Add(DoubleToStr("%4.3f", medicao->energia.medida_mwhmes));
	  Item->SubItems->Add(DoubleToStr("%4.3f", medicao->energia.carga_ini_mwhmes));
	  Item->SubItems->Add(DoubleToStr("%4.3f", medicao->PerdaTotal_mwhmes()));
	  Item->SubItems->Add(DoubleToStr("%3.2f", medicao->PerdaTotal_perc()));
	  Item->SubItems->Add(DoubleToStr("%4.3f", medicao->PerdaTecnica_perc()));
	  Item->SubItems->Add(DoubleToStr("%4.3f", medicao->PerdaComercial_perc()));
	  Item->SubItems->Add(DoubleToStr("%4.3f", medicao->PerdaComercialA3a_perc()));
	  Item->SubItems->Add(DoubleToStr("%4.3f", medicao->PerdaComercialA4_perc()));
	  Item->SubItems->Add(DoubleToStr("%4.3f", medicao->PerdaComercialB_perc()));
	  Item->SubItems->Add(DoubleToStr("%4.3f", medicao->PerdaComercialOutros_perc()));
	  }
   //for�a ordena��o por nome da rede
   LViewSort.column     = LViewMedicao->Column[0];
   LViewSort.ascendente = true;
   LViewMedicao->AlphaSort();
   }

//---------------------------------------------------------------------------
//eof



