//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TFormEdtCaps.h"
//#include "TFormEdtPlan.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTCapserie.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEdtCaps::TFormEdtCaps(TComponent* Owner, VTApl *apl, VTCapserie* pcaps)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl    = apl;
   this->pcaps = pcaps;
   //cria objetos
   lisEQP = new TList();
   /*
   //verifica se deve habilitar edição de dados de planejamento
   if ((pcaps->planeja.ano > 0)&&
       ((pcaps->planeja.ano >= rede->planeja.ano)||(rede->planeja.ano == MAX_ANO)))
      {//cria Form p/ edição de dados do Planejamento
      FormEdtPlan = new TFormEdtPlan(this, tsPlaneja, apl, pcaps);
      //habilita TableSheet de planejamento
      tsPlaneja->TabVisible = true;
      }
   else
      {//desabilita TableSheet de planejamento
      tsPlaneja->TabVisible = false;
      }
   */
   //desabilita TableSheet de planejamento
   TabSheetPlaneja->TabVisible = false;
   //inicia CBox c/ as Redes
   IniciaCBoxRede();
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtCaps::~TFormEdtCaps(void)
   {//variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   //destrói objetos criados
   //if (FormEdtPlan) {delete FormEdtPlan; FormEdtPlan = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCaps::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCaps::ActionConfirmaExecute(TObject *Sender)
   {
   if (! LeDadosCapserie()) return;
   /*
   //verifica se é editor de planejamento
   if (FormEdtPlan)
      {//insere Eqpto no planejamento
      if (! FormEdtPlan->ConfirmaInsereEqpto(pcaps)) return;
      }
   //atualiza status do Eqpto: alterado
   */
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCaps::ActionRetiraExecute(TObject *Sender)
   {
   /*
   //verifica se é editor de planejamento
   if (FormEdtPlan)
      {//retira Eqpto no planejamento
      FormEdtPlan->CancelaInsereEqpto(pcaps);
      }
   */
   //fecha o form indicando exclusão do Eqpto
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
void _fastcall TFormEdtCaps::ExibeDadosCapserie(void)
   {
   //exibe dados do Capserie
   EditCodigo->Text = pcaps->Codigo;
//   EditR->Text      = DoubleToStr("%5.4f", pcaps->z.r);
//   EditX->Text      = DoubleToStr("%5.4f", fabs(pcaps->z.x));
   EditR->Text      = DoubleToStr("%6.5f", pcaps->z.r);        //FKM - 01.10.2015
   EditX->Text      = DoubleToStr("%6.5f", fabs(pcaps->z.x));  //FKM - 01.10.2015
   EditSnom->Text   = DoubleToStr("%4.3f", pcaps->snom * 1000.); //MVA => kVA
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCaps::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      geral->Infoset->LoadFromFile(this);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCaps::FormShow(TObject *Sender)
   {
   //inicia dados default p/ Eqpto novo
   DefineValoresDefault();
   //inicia Captions,Labels etc da tela
   IniciaParteFixaTela();
   //seleciona TableSheet
   pgCtrl->ActivePage = tsCapserie;
   //exibe dados da Capserie
   ExibeDadosCapserie();
   //posiciona o Cursor em EditCodigo
   EditCodigo->SetFocus();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCaps::DefineValoresDefault(void)
   {
   //variáveis locais
   VTCapserie *ult_pcaps;
   VTRedes    *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //verifica se o Capserie ainda não foi iniciado
   if ((pcaps->z.r == 0.)&&(pcaps->z.x == 0.))
      {//define valores default
      pcaps->z.r = 0.;
      pcaps->z.x = 0.;
      //verifica se existem outros Capserie na rede
      redes->LisLigacao(lisEQP, eqptoCAPSERIE);
      if (lisEQP->Count == 0) return;
      //seleciona Capserie de referência p/ copiar os dados
      if ((ult_pcaps = (VTCapserie*)lisEQP->Last()) == NULL) return;
      //copia dados da chave de referência
      pcaps->z = ult_pcaps->z;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCaps::IniciaCBoxRede(void)
   {
   //variáveis locais
   int      index;
   VTRede  *rede;
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //reinicia CBox c/ as Redes
   cboxRede->Clear();
   if (lisREDE->Count == 0) return;
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (! rede->Carregada) continue;
      cboxRede->Items->AddObject(rede->Codigo, rede);
      }
   //define ordem alfabética
   cboxRede->Sorted = true;
   //pré-seleciona Rede que conecta as Barras da Chave
   index = 0;
   if ((rede = SelecionaRede(lisREDE)) != NULL) index = cboxRede->Items->IndexOfObject(rede);
   cboxRede->ItemIndex = index;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCaps::IniciaParteFixaTela(void)
   {
   //atualiza Label c/ Inom da Chvae
   //LabelCapsCapacidade->Caption = "Capacidade(" + aue->UEextPotAparente() + ")";
   }

//---------------------------------------------------------------------------
/*
bool __fastcall TFormEdtCaps::LeDadosCapserie(void)
   {
   //proteção: verifica se o Capserie está incluído
   if (pcaps == NULL) return(true);
   try{//lê dados d0 Capserie
      pcaps->Codigo = EditCodigo->Text;
      pcaps->z.r    = 0.;
      pcaps->z.r    = StrToDouble(EditR->Text);
      pcaps->z.x    = -fabs(StrToDouble(EditX->Text));
      pcaps->snom   = StrToDouble(EditSnom->Text) / 1000.;   //kVA => MVA
      }catch(Exception &e)
         {
         Erro("Dados Inválidos do Capserie");
         return(false);
         }
   //verifica validade dos dados
   if (IsDoubleZero(pcaps->snom))
      {
      Aviso("Capacidade com valor nulo");
      return(false);
      }
   if (IsDoubleZero(pcaps->z.x))
      {
      Aviso("Reatância com valor nulo");
      return(false);
      }
   return(true);
   }
 */
//---------------------------------------------------------------------------
//FKM - 2015.09.30
bool __fastcall TFormEdtCaps::LeDadosCapserie(void)
   {
      //verifica validade dos dados
   if (IsDoubleZero(StrToDouble(EditSnom->Text),1e-3))
      {
      Aviso("Capacidade com valor inválido ou nulo");
      return(false);
      }
   if (IsDoubleZero(StrToDouble(EditX->Text)))
      {
      Aviso("Reatância com valor inválido ou nulo");
      return(false);
      }
   //proteção: verifica se o Capserie está incluído
   if (pcaps == NULL) return(true);
   try{//lê dados d0 Capserie
      pcaps->Codigo = EditCodigo->Text;
      pcaps->z.r    = 0.;
      pcaps->z.r    = StrToDouble(EditR->Text);
      pcaps->z.x    = -fabs(StrToDouble(EditX->Text));
	  pcaps->snom   = StrToDouble(EditSnom->Text) / 1000.;   //kVA => MVA
	  //determina Rede do Trecho
	  pcaps->rede = (VTRede*)cboxRede->Items->Objects[cboxRede->ItemIndex];
      }catch(Exception &e)
		 {
         Erro("Dados Inválidos do Capserie");
         return(false);
         }

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCaps::OpcaoRetira(bool enabled)
   {
   ActionRetira->Visible = enabled;
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TFormEdtCaps::SelecionaRede(TList *lisREDE)
   {
   //variáveis locais
   VTRede  *rede;

   //verifica se o pcaps já possui uma Rede definida
   if (pcaps->rede) return(pcaps->rede);
   //procura uma Rede que conecta as Barras do pcaps
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
	  if ((rede->ExisteBarra(pcaps->pbarra1))||
		  (rede->ExisteBarra(pcaps->pbarra2))) return(rede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof




