//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtTrfZZ.h"
//#include "TFormEdtPlan.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTTrafoZZ.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEdtTrfZZ::TFormEdtTrfZZ(TComponent* Owner, VTApl *apl, VTTrafoZZ* ptrfZZ)
   : TForm(Owner)
   {
   //salva ponteiros p/ objetos
   this->apl    = apl;
   this->ptrfZZ = ptrfZZ;
   //cria lista
   lisEQP = new TList();
   /*
   //verifica se deve habilitar edição de dados de planejamento
   if ((ptrfZZ->planeja.ano > 0)&&
       ((ptrfZZ->planeja.ano >= rede->planeja.ano)||(rede->planeja.ano == MAX_ANO)))
      {//cria Form p/ edição de dados do Planejamento
      FormEdtPlan = new TFormEdtPlan(this, tsPlaneja, apl, ptrfZZ);
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
__fastcall TFormEdtTrfZZ::~TFormEdtTrfZZ(void)
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
void __fastcall TFormEdtTrfZZ::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfZZ::ActionConfirmaExecute(TObject *Sender)
   {
   //lê e valida dados do Eqpto
   if (! LeDadosTrafoZZ()) return;
   /*
   //verifica se é editor de planejamento
   if (FormEdtPlan)
      {//insere Eqpto no planejamento
      if (! FormEdtPlan->ConfirmaInsereEqpto(ptrfZZ)) return;
      }
   */
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfZZ::ActionRetiraExecute(TObject *Sender)
   {
   /*
   //verifica se é editor de planejamento
   if (FormEdtPlan)
      {//retira Eqpto no planejamento
      FormEdtPlan->CancelaInsereEqpto(ptrfZZ);
      }
   //fecha o form indicando exclusão do Eqpto
   */
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfZZ::DefValoresDefaultTrafoZZ(void)
   {
   //variáveis locais
   VTTrafoZZ *ult_trfZZ;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //verifica se o TrafoZZ não está definido
   if ((ptrfZZ->snom == 0.)||(ptrfZZ->vnom == 0.)||(ptrfZZ->z0.x == 0.))
      {//assume valores default
      ptrfZZ->vnom = ptrfZZ->pbarra->vnom;
      ptrfZZ->snom = 50;
      ptrfZZ->z0.r = 0.;
      ptrfZZ->z0.x = 0.05;
      //seleciona TrafoZZ de referência p/ copiar os dados
      lisEQP->Clear();
      redes->LisEqbar(lisEQP, eqptoTRAFOZZ);
      if (lisEQP->Count == 0) return;
      if ((ult_trfZZ = (VTTrafoZZ*)lisEQP->Last()) == NULL)  return;
      //copia dados do TrafoZZ de referência
      ptrfZZ->vnom = ult_trfZZ->vnom;
      ptrfZZ->snom = ult_trfZZ->snom;
      ptrfZZ->z0   = ult_trfZZ->z0;
      }
   }

//---------------------------------------------------------------------------
void _fastcall TFormEdtTrfZZ::ExibeDadosTrafoZZ(void)
   {
   //exibe dados do TrafoZZ
   EditCodigo->Text = ptrfZZ->Codigo;
   edtVnom->Text   = DoubleToStr("%4.3f", ptrfZZ->vnom);
   edtSnom->Text   = DoubleToStr("%4.3f", ptrfZZ->snom);
   edtR0->Text     = DoubleToStr("%4.3f", ptrfZZ->z0.r);
   edtX0->Text     = DoubleToStr("%4.3f", ptrfZZ->z0.x);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfZZ::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      geral->Infoset->LoadFromFile(this);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfZZ::FormShow(TObject *Sender)
   {
   //se necessário, define valores default p/ TrafoZZ
   DefValoresDefaultTrafoZZ();
   //inicia Captions,Labels etc da tela
   IniciaParteFixaTela();
   //exibe dados do TrafoZZ
   ExibeDadosTrafoZZ();
   //seleciona TableSheet
   pgCtrl->ActivePage = tsTrafoZZ;
   //posiciona o Cursor em EditCodigo
   EditCodigo->SetFocus();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfZZ::IniciaParteFixaTela(void)
   {
   /*
   //atualiza Label c/ Inom da Chvae
   LabelChvInom->Caption = "Inom(" + aue->UEextCorrente() + ")";
   //atualiza Labels de Vnom s SNom do TrafoZZ
   LabelVnom->Caption = "Vnom(" + aue->UEextTensao() + ")";
   LabelSnom->Caption = "Snom(" + aue->UEextPotAparente() + ")";
   */
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrfZZ::LeDadosTrafoZZ(void)
   {
   //proteção: verifica se o TrafoZZ está incluído
   if (ptrfZZ == NULL) return(true);
   try{//lê dados d0 TrafoZZ
      ptrfZZ->Codigo   = EditCodigo->Text;
      ptrfZZ->vnom     = StrToDouble(edtVnom->Text);
      ptrfZZ->snom     = StrToDouble(edtSnom->Text);
      ptrfZZ->z0.r     = StrToDouble(edtR0->Text);
      ptrfZZ->z0.x     = StrToDouble(edtX0->Text);
      }catch(Exception &e)
         {
         Erro("Dados Inválidos do TrafoZZ");
         return(false);
         }
   //valida os dados
   if (ptrfZZ->vnom == 0.)
      {
      Erro("Tensão nominal nula");
      return(false);
      }
   if (ptrfZZ->snom == 0.)
      {
      Erro("Potência nominal nula");
      return(false);
      }
   if ((ptrfZZ->z0.r == 0.)&&(ptrfZZ->z0.x == 0.))
      {
      Erro("Impedância nula");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfZZ::OpcaoRetira(bool enabled)
   {
   butRetira->Visible = enabled;
   }

//---------------------------------------------------------------------------
//eof

