//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormImportaBarras.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Edita\VTEdita.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormImportaBarras::TFormImportaBarras(TComponent* Owner, VTApl *apl_owner)
   : TForm(Owner)
   {
   //salva ponteiro
   this->apl = apl_owner;
   //cria lista de barras
   lisBARRAS = new TList();
   //cria stringLists
   linhas = new TStringList();
   campos = new TStringList();
   //inicia
   Inicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormImportaBarras::~TFormImportaBarras(void)
   {
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destroi lista sem destrui objetos
   if(lisBARRAS) {delete lisBARRAS; lisBARRAS = NULL;}
   if(linhas)    {delete linhas;    linhas = NULL;}
   if(campos)    {delete campos;    campos = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaBarras::ActionCancelaExecute(TObject *Sender)
   {
   //fecha o form
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaBarras::ActionSelArqExecute(TObject *Sender)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTEdita   *edita   = (VTEdita*)apl->GetObject(__classid(VTEdita));

   //verifica se algum valor válido foi inserido no EditVnom
   if(EditVnom->Text.IsEmpty())
      {
      Aviso("Defina um valor de tensão nominal");
      return;
      }
   if(! StrToDouble(EditVnom->Text, vnom_kv))
      {
      Aviso("Tensão nominal inválida");
      return;
      }
   if((vnom_kv < 0) || IsDoubleZero(vnom_kv))
      {
      Aviso("Tensão nominal inválida");
      return;
      }
   //esconde este form
   Hide();
   //seleciona arquivo com coordenadas
   if(OpenDialog->Execute())
      {//executa
      ImportaBarras(OpenDialog->FileName);
      //insere todas as barras importadas
      edita->InsereLisEqpto(lisBARRAS);
      grafico->AreaRedeDefine();
      }
   //reapresenta form
   Show();
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TFormImportaBarras::CriaBarra(TStringList *campos)
   {
   //variáveis locais
   double      utm_x, utm_y;
   VTBarra     *barra;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   try{//converte campos nas coordenadas
   utm_x = StrToDouble(campos->Strings[0]) * 100;  //m => cm
   utm_y = StrToDouble(campos->Strings[1]) * 100;  //m => cm
   if(IsDoubleZero(utm_x) || IsDoubleZero(utm_y)) return(NULL);
   //cria obj barra
   barra = DLL_NewObjBarra(patamares->NumPatamar());
   barra->Id     = FalsoId();
   barra->Codigo = "B_" + IntToStr(lisBARRAS->Count + 1);
   barra->vnom   = vnom_kv;
   barra->utm.x  = int(utm_x);
   barra->utm.y  = int(utm_y);
   barra->esq.x1 = barra->utm.x;
   barra->esq.y1 = barra->utm.y;
   barra->esq.x2 = barra->utm.x;
   barra->esq.y2 = barra->utm.y;
   barra->Status[sttNOVO]   = true;
   barra->Show[eqp_VISIBLE] = true;
   }catch(Exception &e)
      {
      if(barra) delete barra;
      return(NULL);
      }
   return(barra);
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaBarras::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaBarras::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaBarras::FormShow(TObject *Sender)
   {
   //foco no edit
   EditVnom->SetFocus();
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaBarras::ImportaBarras(AnsiString arq_txt)
   {
   //variáveis locais
   VTBarra     *barra;

   //limpa lista de barras
   lisBARRAS->Clear();
   try{//proteção: verifica se o arquivo existe
      if(! FileExists(arq_txt)) return;
      //lê arquivo
      linhas->Clear();
      linhas->LoadFromFile(arq_txt);
      for(int nl = 0; nl < linhas->Count; nl++)
         {//verifica se é linha de comentário
         if(linhas->Strings[nl].Pos("//") > 0) continue;
         //lê colunas
         campos->Clear();
         if(ExtraiStrings(linhas->Strings[nl], ";", campos) < 2) continue;
         //cria barra e insere na lista
         barra = CriaBarra(campos);
         if(barra != NULL) lisBARRAS->Add(barra);
         }
   }catch(Exception &e)
      {
      Aviso("Erro ao importar barras");
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaBarras::Inicia(void)
   {
   //variáveis locais
   VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //define pasta default importa
   OpenDialog->InitialDir = path->DirImporta();
   //inicia vnom
   vnom_kv = 0.;
   }

//---------------------------------------------------------------------------
