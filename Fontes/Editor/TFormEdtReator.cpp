//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include "TFormEdtReator.h"
//#include "TFormEdtPlan.h"
#include "..\Apl\VTApl.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Legenda\VTLegenda.h"
#include "..\Legenda\VTVetval.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Legenda.h"
#include "..\..\DLL_Inc\Patamar.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;

//---------------------------------------------------------------------------
__fastcall TFormEdtReator::TFormEdtReator(TComponent* Owner, VTApl *apl, VTReator* reator)
   : TForm(Owner)
   {

   //salva ponteiros p/ objetos
   this->apl    = apl;
   this->reator = reator;
   //cria lista
   //cria ojetos
   patamares  = DLL_NewObjPatamares(apl);
   legenda    = DLL_NewObjLegenda(apl);
   lisVETVAL  = new TList();
   lisEQP     = new TList();
   /*
   //verifica se deve habilitar edição de dados de planejamento
   if ((reator->planeja.ano > 0)&&
       ((reator->planeja.ano >= rede->planeja.ano)||(rede->planeja.ano == MAX_ANO)))
      {//cria Form p/ edição de dados do Planejamento
      FormEdtPlan = new TFormEdtPlan(this, tsPlaneja, apl, reator);
      //habilita TableSheet de planejamento
      tsPlaneja->TabVisible = true;
      }
   else
      {//desabilita TableSheet de planejamento
      tsPlaneja->TabVisible = false;
      }
   */
   //desabilita TableSheet de planejamento
   tsPlaneja->TabVisible = false;
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtReator::~TFormEdtReator(void)
   {//variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destrói objetos criados
   //if (FormEdtPlan) {delete FormEdtPlan; FormEdtPlan = NULL;}
   //destrói objetos criados
   if (legenda)   {delete legenda;   legenda   = NULL;}
   if (patamares) {delete patamares; patamares = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP)  {delete lisEQP; lisEQP = NULL;}
   //destrói listas e seus obejtos
   if (lisVETVAL) {LimpaTList(lisVETVAL); delete lisVETVAL; lisVETVAL = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtReator::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtReator::ActionConfirmaExecute(TObject *Sender)
   {
   //lê dados do Eqpto
   if (! LeDadosReator()) return;
   /*
   //verifica se é editor de planejamento
   if (FormEdtPlan)
      {//insere Eqpto no planejamento
      if (! FormEdtPlan->ConfirmaInsereEqpto(reator)) return;
      }
   */
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtReator::ActionRetiraExecute(TObject *Sender)
   {
   /*
   //verifica se é editor de planejamento
   if (FormEdtPlan)
      {//retira Eqpto no planejamento
      FormEdtPlan->CancelaInsereEqpto(reator);
      }
   */
   //fecha o form indicando exclusão do Eqpto
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtReator::DefValoresDefaultReator(void)
   {
   //variáveis locais
   VTReator *ult_reator;
   VTRedes  *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //verifica se o Reator não está definido
   if ((reator->q == 0.)&&(reator->vnom == 0.))
      {//assume valores default
      reator->vnom    = reator->pbarra->vnom;
      reator->q       = 20.;
      //define tipo de ligação
      reator->ligacao = lgTRIANG;
      //seleciona Reator de referência p/ copiar os dados
      lisEQP->Clear();
      redes->LisEqbar(lisEQP, eqptoREATOR);
      if (lisEQP->Count == 0) return;
      if ((ult_reator = (VTReator*)lisEQP->Last()) == NULL) return;
      //copia dados do Reator de referência
      //reator->vnom    = reator->vnom;
      reator->q       = reator->q;
      reator->ligacao = ult_reator->ligacao;
      reator->curva   = ult_reator->curva;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtReator::ExibeCurva(void)
   {
   //variáveis locais
   int         ind_cor;
   double      val, val_max, val_min, increment;
   AnsiString  titulo;
   TColor      cor = clBlue;
   VTVetval    *vetval;

   //proteção: verifica se há uma Curva selecionada
   if (CBoxCurva->ItemIndex < 0) return;
   //redefine Curva do Reator
   reator->curva  = (VTCurva*)CBoxCurva->Items->Objects[CBoxCurva->ItemIndex];
   //obtém lista de valores de Pot.Reativa definida pela Curva do Capacitor
   legenda->LisVetValFluxo(reator, lisVETVAL, reat_Q);
   if (lisVETVAL->Count == 0) return;
   //insere Serie no TChart p/ o Vetval
   vetval = (VTVetval*)lisVETVAL->First();
   //reinicia série
   Series1->Clear();
   chart->LeftAxis->Minimum   = 0.;
   chart->LeftAxis->Maximum   = 0.;
   //preenche a Série c/ os valores em todos patamares
   for (int np = 0; np < patamares->NumPatamar(); np++)
      {//determina valor no patamar
      val = vetval->Valor[np];
      //insere ponto na Serie
      //Series1->AddXY(np+1, val, IntToStr(np+1), cor);
      Series1->AddXY(np,   val, IntToStr(np+1), cor);
      Series1->AddXY(np+1, val, IntToStr(np+1), cor);
      //determina valores máximo e mínimo
      if (np == 0) val_max = val_min = val;
      val_max = max(val_max, val);
      val_min = min(val_min, val);
      }
   //inclui uma margem adicional nos valores máximo e mínimo
   val_max += 0.1 * val_max;
   val_min -= 0.1 * val_min;
   if (val_min < 0.) val_min = 0;
   //define incremento
   increment = (val_max - val_min) / 10.;
   if      (increment > 1.)   increment = 1.;
   else if (increment > 0.1)  increment = 0.1;
   else if (increment > 0.01) increment = 0.01;
   else                       increment = 0.001;
   //redefine valores mínimo e máximo do eixo y
   chart->LeftAxis->Visible   = true;
//   chart->LeftAxis->Automatic = false;
//   chart->LeftAxis->Maximum   = val_max;
//   chart->LeftAxis->Minimum   = val_min;
//   chart->LeftAxis->Increment = increment;
   chart->LeftAxis->Automatic = true;  //FKM
   chart->LeftAxis->Increment = 0.;    //FKM
   //reinicia título
   chart->Title->Text->Clear();
   chart->Title->Text->Add(vetval->DescUnid());
   }

//---------------------------------------------------------------------------
void _fastcall TFormEdtReator::ExibeDadosReator(void)
   {
   //exibe dados do Reator
   EditCodigo->Text        = reator->Codigo;
   edtSnom->Text          = DoubleToStr("%4.3f", reator->q);
   edtVnom->Text          = DoubleToStr("%4.3f", reator->vnom);
   cboxLigacao->ItemIndex = reator->ligacao;
   //verifica se o Reator possui uma Curva típica associada
   if (reator->curva != NULL)
      {
      CBoxCurva->ItemIndex = CBoxCurva->Items->IndexOfObject(reator->curva);
      if ((CBoxCurva->ItemIndex < 0)&&(CBoxCurva->Items->Count > 0)) CBoxCurva->ItemIndex = 0;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtReator::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      geral->Infoset->LoadFromFile(this);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtReator::FormShow(TObject *Sender)
   {
   //se necessário, define valores default p/ Reator
   DefValoresDefaultReator();
   //inicia Captions,Labels etc da tela
   IniciaParteFixaTela();
   //inicia CBoxCurva c/ as curvas típicas de Reator
   IniciaCBoxCurva();
   //exibe dados do Reator e da Chave
   ExibeDadosReator();
   //seleciona TableSheet
   pgCtrl->ActivePage = tsReator;
   //posiciona o Cursor em EditCodigo
   EditCodigo->SetFocus();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtReator::IniciaCBoxCurva(void)
   {
   //variáveis locais
   VTCurva  *curva;
   VTCurvas *curvas   = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
   TList    *lisCURVA = curvas->ListaCurvaTipicaReator();

   //reinicia CBoxCurva
   CBoxCurva->Clear();
   for (int n = 0; n < lisCURVA->Count; n++)
      {
      curva = (VTCurva*)lisCURVA->Items[n];
      CBoxCurva->Items->AddObject(curva->Codigo, curva);
      }
   //pré-seleciona primeira Curva
   if (CBoxCurva->Items->Count > 0) CBoxCurva->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtReator::IniciaParteFixaTela(void)
   {
   /*
   //atualiza Label c/ Inom da Chvae
   LabelChvInom->Caption = "Inom(" + aue->UEextCorrente()    + ")";
   LabelSnom->Caption    = "Snom(" + aue->UEextPotAparente() + ")";
   LabelVnom->Caption    = "Vnom(" + aue->UEextTensao()      + ")";
   */
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtReator::LeDadosReator(void)
   {
   //proteção: verifica se o Reator está incluído
   if (reator == NULL) return(true);
   try{//lê dados do Reator
      reator->Codigo  = EditCodigo->Text;
      reator->q       = StrToDouble(edtSnom->Text);
      reator->vnom    = StrToDouble(edtVnom->Text);
      reator->ligacao = cboxLigacao->ItemIndex;
      reator->curva   = (VTCurva*)CBoxCurva->Items->Objects[CBoxCurva->ItemIndex];
      }catch(Exception &e)
         {
         Erro("Dados Inválidos do Reator");
         return(false);
         }
   if (reator->vnom == 0.)
      {
      Erro("Tensão nominal nula");
      return(false);
      }
   if (reator->q == 0.)
      {
      Erro("Potência reativa nula");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtReator::OpcaoRetira(bool enabled)
   {
   butRetira->Visible = enabled;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtReator::pgCtrlChanging(TObject *Sender, bool &AllowChange)
   {
   //default: permite mudança
   AllowChange = true;
   //verifica se está selecionado visualização da Curva
   if (pgCtrl->ActivePage == tsReator)
      {//só permite mudança se os dados do Reator forem válidos
       AllowChange = LeDadosReator();
       }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtReator::pgCtrlChange(TObject *Sender)
   {
   //verifica se está selecionado visualização da Curva
   if (pgCtrl->ActivePage != TabSheetCurva) return;
   //atualiza TChart
   ExibeCurva();
   }

//---------------------------------------------------------------------------
//eof

