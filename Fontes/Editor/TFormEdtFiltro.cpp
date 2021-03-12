//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtFiltro.h"
//#include "TFormEdtPlan.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTFiltro.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEdtFiltro::TFormEdtFiltro(TComponent* Owner, VTApl *apl, VTFiltro* filtro)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl    = apl;
   this->filtro = filtro;
   //cria lista
   lisEQP = new TList();
   /*
   //verifica se deve habilitar edição de dados de planejamento
   if ((filtro->planeja.ano > 0)&&
       ((filtro->planeja.ano >= rede->planeja.ano)||(rede->planeja.ano == MAX_ANO)))
      {//cria Form p/ edição de dados do Planejamento
      FormEdtPlan = new TFormEdtPlan(this, tsPlaneja, apl, filtro);
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
__fastcall TFormEdtFiltro::~TFormEdtFiltro(void)
   {//variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destrói objetos criados
   //if (FormEdtPlan) {delete FormEdtPlan; FormEdtPlan = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP)  {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFiltro::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFiltro::ActionConfirmaExecute(TObject *Sender)
   {
   //lê dados do Eqpto
   if (! LeDadosFiltro()) return;
   /*
   //verifica se é editor de planejamento
   if (FormEdtPlan)
      {//insere Eqpto no planejamento
      if (! FormEdtPlan->ConfirmaInsereEqpto(filtro)) return;
      }
   */
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFiltro::ActionRetiraExecute(TObject *Sender)
   {
   /*
   //verifica se é editor de planejamento
   if (FormEdtPlan)
      {//retira Eqpto no planejamento
      FormEdtPlan->CancelaInsereEqpto(filtro);
      }
   */
   //fecha o form indicando exclusão do Eqpto
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFiltro::DefValoresDefaultFiltro(void)
   {
   //variáveis locais
   VTFiltro *ult_filtro;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //verifica se o Filtro não está definido
   if ((filtro->z0.r == 0.)&&(filtro->z0.x == 0.))
      {//assume valores default
      filtro->vnom    = filtro->pbarra->vnom;
      filtro->snom    = 100.;
      filtro->z0.r    = 0.;
      filtro->z0.x    = 0.05;
      //seleciona Filtro de referência p/ copiar os dados
      lisEQP->Clear();
      redes->LisEqbar(lisEQP, eqptoFILTRO);
      if (lisEQP->Count == 0) return;
      if ((ult_filtro = (VTFiltro*)lisEQP->Last()) == NULL)  return;
      //copia dados do Filtro de referência
      filtro->vnom    = ult_filtro->vnom;
      filtro->snom    = ult_filtro->snom;
      filtro->z0      = ult_filtro->z0;
      }
   }

//---------------------------------------------------------------------------
void _fastcall TFormEdtFiltro::ExibeDadosFiltro(void)
   {
   //exibe dados do Filtro
   EditCodigo->Text = filtro->Codigo;
   edtSnom->Text   = DoubleToStr("%4.3f", filtro->snom);
   edtVnom->Text   = DoubleToStr("%4.3f", filtro->vnom);
   edtR0->Text     = DoubleToStr("%4.3f", filtro->z0.r);
   edtX0->Text     = DoubleToStr("%4.3f", filtro->z0.x);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFiltro::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      geral->Infoset->LoadFromFile(this);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFiltro::FormShow(TObject *Sender)
   {
   //se necessário, define valores default p/ Filtro
   DefValoresDefaultFiltro();
   //inicia Captions,Labels etc da tela
   IniciaParteFixaTela();
   //exibe dados do Filtro e da Chave
   ExibeDadosFiltro();
   //seleciona TableSheet
   pgCtrl->ActivePage = tsFiltro;
   //posiciona o Cursor em EditCodigo
   EditCodigo->SetFocus();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFiltro::IniciaParteFixaTela(void)
   {
   /*
   //atualiza Label c/ Inom da Chvae
   LabelChvInom->Caption = "Inom(" + aue->UEextCorrente()    + ")";
   LabelSnom->Caption    = "Snom(" + aue->UEextPotAparente() + ")";
   LabelVnom->Caption    = "Vnom(" + aue->UEextTensao()      + ")";
   */
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtFiltro::LeDadosFiltro(void)
   {
   //proteção: verifica se o Filtro está incluído
   if (filtro == NULL) return(true);
   try{//lê dados d0 Filtro
      filtro->Codigo  = EditCodigo->Text;
      filtro->vnom    = StrToDouble(edtVnom->Text);
      filtro->snom    = StrToDouble(edtSnom->Text);
      filtro->z0.r    = StrToDouble(edtR0->Text);
      filtro->z0.x    = StrToDouble(edtX0->Text);
      }catch(Exception &e)
         {
         Erro("Dados Inválidos do Filtro");
         return(false);
         }
   if (filtro->vnom == 0.)
      {
      Erro("Tensão nominal nula");
      return(false);
      }
   if (filtro->snom == 0.)
      {
      Erro("Potência nominal nula");
      return(false);
      }
   if ((filtro->z0.r == 0.)&&(filtro->z0.x == 0.))
      {
      Erro("Impedância de seq.0 nula");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtFiltro::OpcaoRetira(bool enabled)
   {
   butRetira->Visible = enabled;
   }

//---------------------------------------------------------------------------
//eof


