//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtPolyline.h"
#include "TEdtPolyline.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Apl\VTApl.h"
#include "..\Editor\VTEditor.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Arranjo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEdtPolyline::TFormEdtPolyline(TComponent* Owner, VTApl *apl,
                                              TEdtPolyline *EdtPolyline, TWinControl *parent)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl         = apl;
   this->EdtPolyline = EdtPolyline;
   geral             = (VTGeral*)apl->GetObject(__classid(VTGeral));
   //redefine Parent
   if (parent) {Parent = parent;}
   //cria lista
   //lisEQP = new TList();
   lisBARRA  = new TList();
   lisTRECHO = new TList();
   //inicia componentes
   CBoxArranjoInicia();
   CBoxRedeInicia();
   SGridInicia();
   //pré-define dados p/ os trechos
   EditCodigoTrecho->Text = "";
   EditComprimento->Text  = "";
   EditCodigoBarra->Text  = "";
   EditVnom->Text         = "";
   //salva posição inicial da janela flutuante
   janela.top    = 0;
   janela.left   = 0;
   janela.width  = Width;
   janela.height = Height;
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtPolyline::~TFormEdtPolyline(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destrói lista sem destruir seus objetos
   if (lisBARRA)  {delete lisBARRA;  lisBARRA  = NULL;}
   if (lisTRECHO) {delete lisTRECHO; lisTRECHO = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtPolyline::ActionAlignNoneExecute(TObject *Sender)
   {
   //cancela alinhamento da janela (flutuante)
   Align  = alNone;
   Top    = janela.top;
   Left   = janela.left;
   Width  = janela.width;
   Height = janela.height;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtPolyline::ActionAlteraExecute(TObject *Sender)
   {
   //variáveis locais
   VTTrecho *trecho;

   //proteção
   if (SGrid->RowCount <= 1)
      {
      Aviso("Não foram definidos novos trechos");
      return;
      }
   //lê dados de todos os Trechos
   for (int row = 1; row < SGrid->RowCount; row++)
      {
      try{//determina Trecho
         if ((trecho = (VTTrecho*)SGrid->Objects[0][row]) == NULL) continue;
         //lê código e comprimento
         trecho->Codigo         = SGrid->Cells[1][row];
         trecho->Comprimento_km = (StrToDouble(SGrid->Cells[2][row]) / 1000.);
         }catch(Exception &e)
            {
            Erro("Comprimento inválido do trecho: " + trecho->Codigo);
            return;
            }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtPolyline::ActionFechaExecute(TObject *Sender)
   {
   //fecha este form
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtPolyline::ActionNovaSequenciaExecute(TObject *Sender)
   {
   //inicia uma nova sequencia de Trechos
   EdtPolyline->IniciaNovaSequenciaTrechos();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtPolyline::CBoxArranjoInicia(void)
   {
   //variáveis locais
   VTArranjo  *arranjo;
   VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   TList      *lisARR   = arranjos->LisArranjo();

   //reinicia CBox c/ os Arranjos
   CBoxArranjo->Clear();
   //inclui no CBoxArranjo somente os Arranjos que atendem às restrições de fase das barras
   for (int n = 0; n < lisARR->Count; n++)
      {
      arranjo = (VTArranjo*)lisARR->Items[n];
      //verifica se o Arranjo é obsoleto
      if (arranjo->StatusExcluido()) {continue;}
      //insere Arranjo
      CBoxArranjo->Items->AddObject(arranjo->Codigo, arranjo);
      }
   //define ordem alfabética
   CBoxArranjo->Sorted = true;
   //não pré-seleciona Arranjo do Trecho
   CBoxArranjo->ItemIndex = -1;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtPolyline::CBoxRedeInicia(void)
   {
   //variáveis locais
   VTRede  *rede;
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //reinicia CBox c/ as Redes
   CBoxRede->Clear();
   if (lisREDE->Count == 0) return;
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (! rede->Carregada) continue;
      CBoxRede->Items->AddObject(rede->Codigo, rede);
      }
   //define ordem alfabética
   CBoxRede->Sorted = true;
   //pré-seleciona Rede se houver apenas uma Rede
   CBoxRede->ItemIndex = (CBoxRede->Items->Count == 1) ? 0 : -1;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtPolyline::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //variáveis locais
   VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

   //IMPORTANTE: esconde e desabilita o Form p/ evitar sua destruição por TEdtPolyline
   Hide();
   Enabled = false;
   //destrói EdtPolyline que criou este form
   editor->Disable();
   //esconde o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtPolyline::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this))
         {
         /*
         switch(Align)
            {
            case alLeft:   ActionAlignLeft->Execute();   break;
            case alRight:  ActionAlignRight->Execute();  break;
            case alTop:    ActionAlignTop->Execute();    break;
            case alBottom: ActionAlignBottom->Execute(); break;
            case alNone:                                 break;
            }
         */
         }
      return;
      }
   //posiciona o Form sem alinhamento
   ActionAlignNoneExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtPolyline::FormShow(TObject *Sender)
   {
   //posiciona o Cursor em EditComp
   PageControl->ActivePage = TabSheetConfigura;
   EditComprimento->SetFocus();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtPolyline::InsereBarra(VTBarra *barra)
   {
   //insere Barra em lisBARRA
   lisBARRA->Add(barra);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtPolyline::InsereTrecho(VTTrecho *trecho)
   {
   //insere TRecho em lisTRECHO
   lisTRECHO->Add(trecho);
   SGridInicia();
   }

//---------------------------------------------------------------------------
TList* __fastcall TFormEdtPolyline::LisBarra(void)
   {
   return(lisBARRA);
   }

//---------------------------------------------------------------------------
TList* __fastcall TFormEdtPolyline::LisTrecho(void)
   {
   return(lisTRECHO);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TFormEdtPolyline::PM_GetArranjo(void)
   {
   //variáveis locais
   VTArranjo *arranjo;

   //verifica se há um Arranjo selecionado
   if (CBoxArranjo->ItemIndex < 0)
      {//não há arranjo típico selecionado
      Aviso("Selecione um arranjo para os trechos.");
      return(NULL);
      }
   arranjo = (VTArranjo*)(CBoxArranjo->Items->Objects[CBoxArranjo->ItemIndex]);
   return(arranjo);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormEdtPolyline::PM_GetCodigoBarra(void)
   {
   //variáveis locais
   AnsiString codigo;

   codigo  = EditCodigoBarra->Text.Trim();
   if (! codigo.IsEmpty()) codigo += IntToStr(lisBARRA->Count + 1);

   return(codigo);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormEdtPolyline::PM_GetCodigoTrecho(void)
   {
   //variáveis locais
   AnsiString codigo;

   codigo  = EditCodigoTrecho->Text.Trim();
   if (! codigo.IsEmpty()) codigo += IntToStr(lisTRECHO->Count + 1);

   return(codigo);
   }

//---------------------------------------------------------------------------
double __fastcall TFormEdtPolyline::PM_GetComprimento_km(void)
   {
   //variáveis locais
   double comp_km;

   try{//lê comprimento
      comp_km = StrToDouble(EditComprimento->Text) / 1000.;
      }catch(Exception &e)
         {
         comp_km = 0.;
         }
   return(comp_km);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtPolyline::PM_GetExibirRastro(void)
   {
   return(CheckBoxRastro->Checked);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TFormEdtPolyline::PM_GetRede(void)
   {
   //variáveis locais
   VTRede  *rede;

   //verifica se há um Arranjo selecionado
   if (CBoxRede->ItemIndex < 0)
      {//não há arranjo típico selecionado
      Aviso("Selecione a rede dos trechos.");
      return(NULL);
      }
   rede = (VTRede*)(CBoxRede->Items->Objects[CBoxRede->ItemIndex]);
   return(rede);
   }

//---------------------------------------------------------------------------
double __fastcall TFormEdtPolyline::PM_GetVnom_kv(void)
   {
   //variáveis locais
   double vnom_kv;

   try{//lê comprimento
      vnom_kv = StrToDouble(EditVnom->Text);
      if (IsDoubleZero(vnom_kv))
         {
         Aviso("Defina a tensão nominal das barras");
         }
      }catch(Exception &e)
         {
         Aviso("Tensão nominal das barras com valor inválido");
         vnom_kv = 0.;
         }
   return(vnom_kv);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtPolyline::SGridInicia(void)
   {
   //variáveis locais
   VTTrecho *trecho;

   //verifica se há Trechos em lisTRECHO
   if (lisTRECHO->Count == 0)
      {//esconde SGrid
      SGrid->Visible = false;
      return;
      }
   //reinicia SGrid
   for (int row = 0; row < SGrid->RowCount; row++) SGrid->Rows[row]->Clear();
   //define número de linhas e colunas
   SGrid->RowCount = (lisTRECHO->Count + 1);
   SGrid->ColCount = 3;
   //define títulos das colunas
   SGrid->Cells[0][0] = "Rede";
   SGrid->Cells[1][0] = "Trecho";
   SGrid->Cells[2][0] = "Comp(m)";
   //exibe dados dos Trechos
   for (int n = 0, row=1; n < lisTRECHO->Count; n++, row++)
      {
      trecho = (VTTrecho*)lisTRECHO->Items[n];
      //salva Trecho na primeira célula da linha
      SGrid->Objects[0][row] = trecho;
      //preenche colunas: rede, trecho, comprimento
      SGrid->Cells[0][row] = trecho->rede->Codigo;
      SGrid->Cells[1][row] = trecho->Codigo;
      SGrid->Cells[2][row] = DoubleToStr("%4.1f", trecho->Comprimento_m);
      }
   //exibe SGrid
   SGrid->Visible = true;
   }

//---------------------------------------------------------------------------
//eof


