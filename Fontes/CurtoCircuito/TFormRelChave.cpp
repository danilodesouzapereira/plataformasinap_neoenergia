//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Report.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Rede\VTChave.h>
#include <Fontes\Rede\VTLigacao.h>
#include <Fontes\RedeCC\ResCurto.h>
#include <Fontes\RedeCC\VTLigCC.h>
#include <Fontes\RedeCC\VTRedeCC.h>
#include <Fontes\Report\VTReport.h>
#include "TFormRelChave.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormRelChave::TFormRelChave(TComponent* Owner, VTApl *apl_owner)
   : TForm(Owner)
   {
   //salva ponteiro para objetos
   this->apl = apl_owner;
   //define ordenação do ListView pelo código da Chave
   ListViewSort.ColumnIndex    = 0;
   ListViewSort.OrdemCrescente = true;
   }

//---------------------------------------------------------------------------
__fastcall TFormRelChave::~TFormRelChave(void)
   {
   //nada  fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelChave::ActionCloseExecute(TObject *Sender)
   {
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelChave::ActionExportaExecute(TObject *Sender)
   {
   //variáveis locais
   VTReport* report;

   try{//seleciona arquivo para gravar
      if (! SelecionaArquivo()) return;
      //cria objeto VTReport
      if ((report = DLL_NewObjReport(apl)) != NULL)
         {//exporta LView em arquivo txt
         report->ArquivoTexto(SaveDialog->FileName, LView, "Capacidade Disruptiva das Chaves");
         DLL_DeleteObjReport(report);
         }
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
int __fastcall TFormRelChave::CompareNum(AnsiString S1, AnsiString S2)
   {
   //variaveis locais
   double   D1=0.0, D2=0.0;

   try{
      D1 = StrToDouble(S1);
      D2 = StrToDouble(S2);
      if(D1 > D2)          return(1);
      else if (D1 < D2)    return(-1);
      else                 return(0);
      }
   catch(Exception &e){
      //retorna 0
      return(0);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelChave::DefineCaminhoEletrico(TList *lisLIG)
   {
   //variáveis locais
   int       ind_bar   = 0;
   int       vet_def[] = {defeito3F, defeito2F, defeitoFT, defeitoFTZ, defeito2FT};
   double    imax_amp;
   TListItem *Item;
   VTChave   *chave;
   VTLigacao *ligacao;
   VTLigCC   *ligCC;
   VTRedeCC  *redeCC = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));

   //reinicia LView
   LView->Clear();
   //identifica as Chaves em lisLIG
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      if (ligacao->Tipo() == eqptoCHAVE)
         {//
         chave = (VTChave*)ligacao;
         //determina LigCC da Ligacao
         ligCC = (VTLigCC*)ligacao->Obj;
         //loop para todos os defeitos
         for (int ind_def = 0; ind_def < sizeof(vet_def)/sizeof(int); ind_def++)
            {//determina maior corrente do tipo de defeito
            imax_amp = ligCC->Ifas_amp_max[vet_def[ind_def]][ind_bar];
            //insere novo ListItem
            Item          = LView->Items->Add();
            Item->Data    = chave;
            Item->Caption = chave->Codigo;
            //capacidade disruptiva
            if (IsDoubleZero(chave->CapDisruptiva_amp))
               {Item->SubItems->Add("indefinida");}
            else
               {Item->SubItems->Add(DoubleToStr("%2.1f", chave->CapDisruptiva_amp));}
            //tipo de defeito
            switch(vet_def[ind_def])
               {
               case defeito3F:  Item->SubItems->Add("3F");         break;
               case defeito2F:  Item->SubItems->Add("2F");         break;
               case defeitoFT:  Item->SubItems->Add("FT");         break;
               case defeitoFTZ: Item->SubItems->Add("FTz");        break;
               case defeito2FT: Item->SubItems->Add("2FT");        break;
               default:         Item->SubItems->Add("indefinido"); break;
               }
            //corrente máxima
            Item->SubItems->Add(DoubleToStr("%2.1f", imax_amp));
            //diagnóstico
            if (IsDoubleZero(chave->CapDisruptiva_amp))
               {Item->SubItems->Add("---");}
            else if (imax_amp < chave->CapDisruptiva_amp)
               {Item->SubItems->Add("normal");}
            else
               {Item->SubItems->Add("violada");}
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelChave::FormClose(TObject *Sender, TCloseAction &Action)   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelChave::LViewColumnClick(TObject *Sender, TListColumn *Column)
   {
   //verifica se selecionou a mesma coluna
   if (ListViewSort.ColumnIndex == Column->Index)
      {//inverte ordem
      ListViewSort.OrdemCrescente  = ! ListViewSort.OrdemCrescente;
      }
   else
      {//redefine índice da coluna usada p/ ordenação
      ListViewSort.ColumnIndex = Column->Index;
      }
   //força reordenação
   LView->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelChave::LViewCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
          int Data, int &Compare)
   {
   //variáveis locais
   int  index = ListViewSort.ColumnIndex-1;

   switch(ListViewSort.ColumnIndex)
      {
      case 0:  //Caption - código da chave - compara strings
         Compare = CompareText(Item1->Caption, Item2->Caption);
         break;
      case 1:  //capacidade disruptiva: compara números
         Compare = CompareNum(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
         break;
      case 2:  //tipo de defeito: compara strings
         Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
         break;
      case 3:  //corrente de defeito: compara números
         Compare = CompareNum(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
         break;
      case 4:  //daignóstico: compara strings
         Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
         break;
      default: //compara strings
         Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
         break;
      }
   //verifica se ordem crescente/decrescente
   if (! ListViewSort.OrdemCrescente) Compare = - Compare;
   }

//---------------------------------------------------------------------------
bool __fastcall TFormRelChave::SelecionaArquivo(void)
   {
   //variáveis locais
   VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //verifica se o diretório SaveDialog já foi definido
   if (SaveDialog->InitialDir.IsEmpty()) SaveDialog->InitialDir = path->DirExporta();
   return(SaveDialog->Execute());
   }

//---------------------------------------------------------------------------
//eof


