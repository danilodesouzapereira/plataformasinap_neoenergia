//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <algorithm>
#include "TFormEdtCap.h"
#include "TFormEditaCurva.h"
#include "TFormExibeCurva.h"
#include "..\Apl\VTApl.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Legenda\VTLegenda.h"
#include "..\Legenda\VTVetval.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Legenda.h"
#include "..\..\DLL_Inc\Patamar.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;

//---------------------------------------------------------------------------
__fastcall TFormEdtCap::TFormEdtCap(TComponent* Owner, VTApl *apl, VTCapacitor *pcap)
   : TForm(Owner)
   {
   //salva ponteiro p/ objetos
   this->apl  = apl;
   this->pcap = pcap;
   //cria ojetos
   patamares  = DLL_NewObjPatamares(apl);
   legenda    = DLL_NewObjLegenda(apl);
   lisVETVAL  = new TList();
   lisEQP     = new TList();
   //se necess�rio, define valores default p/ Capacitor
   DefValoresDefaultCapacitor();
   //define Curva t�pica e Curva pr�pria
   IniciaCurvas();
   //inicia CBoxCurva c/ as curvas t�picas de Capacitor
   IniciaCBoxCurva();
   //exibe dados do Capacitor
   ExibeDadosCapacitor();
   //seleciona TableSheet
	pgCtrl->ActivePage = tsCapacitor;
   //cria TFormEditaCurva p/ edi��o da Curva
   FormEditaCurva = new TFormEditaCurva(this, apl, TabSheetEdicao);
   FormEditaCurva->InsereColuna(indQ, "Q(MVAr)");
   FormEditaCurva->DefineCurva(curva.propria);
   FormEditaCurva->Show();
   //cria TFormExibeCurva p/ exibi��o da Curva
   FormExibeCurva = new TFormExibeCurva(this, apl, TabSheetGrafico);
   FormExibeCurva->Show();
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtCap::~TFormEdtCap(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
	//destr�i curva pr�pria n�o utilizada
	if ((curva.propria != NULL)&&(curva.propria != pcap->curva)) {delete curva.propria; curva.propria = NULL;}
   //destr�i Forms
   if (FormEditaCurva) {delete FormEditaCurva; FormEditaCurva = NULL;}
   if (FormExibeCurva) {delete FormExibeCurva; FormExibeCurva = NULL;}
   //destr�i objetos criados
   if (legenda)   {delete legenda;   legenda   = NULL;}
   if (patamares) {delete patamares; patamares = NULL;}
   //destr�i lista sem destruir seus objetos
   if (lisEQP)  {delete lisEQP; lisEQP = NULL;}
   //destr�i listas e seus obejtos
   if (lisVETVAL) {LimpaTList(lisVETVAL); delete lisVETVAL; lisVETVAL = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCap::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCap::ActionConfirmaExecute(TObject *Sender)
   {
   //l� e valida os dados
   if (! LeDadosCapacitor()) return;
   /*
   //verifica se � editor de planejamento
   if (FormEdtPlan)
      {//insere Eqpto no planejamento
      if (! FormEdtPlan->ConfirmaInsereEqpto(pcap)) return;
      }
   */
   //fecha o form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCap::ActionRetiraExecute(TObject *Sender)
   {
   /*
   //verifica se � editor de planejamento
   if (FormEdtPlan)
      {//retira Eqpto no planejamento
      FormEdtPlan->CancelaInsereEqpto(pcap);
      }
   */
   //fecha o form indicando exclus�o do Eqpto
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCap::CBoxCurvaChange(TObject *Sender)
   {
   //prote��o
   if (CBoxCurva->ItemIndex < 0) return;
   //salva Curva t�pica selecionada
   curva.tipica = (VTCurva*)CBoxCurva->Items->Objects[CBoxCurva->ItemIndex];
   //redefine Curva do Capacitor
   pcap->curva = curva.tipica;
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TFormEdtCap::CriaCurva(void)
   {
   //vari�veis locais
   strHM   hm_ini, hm_fim;
   double  valor[nvCURVA_PQ];
   VTCurva *curva;

   //cria Curva
   curva                = DLL_NewObjCurva();
   curva->Tipica        = false;
   curva->Escala        = patEDITAVEL;
   curva->Unidade       = unidMVA;
   curva->Tipo          = curvaPQ;
   curva->NumeroValores = nvCURVA_PQ;
   //curva->Reconfigura(patEDITAVEL);
   //inicia Curva c/ um �nico patamar
   hm_ini.hora   =  0;
   hm_ini.minuto =  0;
//   hm_fim.hora   = 23;
//   hm_fim.minuto = 59;
   //FKM 2015.09.25 (o curva agora trabalha com: [a,b[ )
   hm_fim.hora   = 24;
   hm_fim.minuto = 00;
   //FKM 2015.09.25 (o curva agora trabalha com: [a,b[ )
   valor[indP]   =  0;
   valor[indQ]   = pcap->Q_mvar;
   curva->SetPonto(hm_ini, hm_fim, valor, curva->NumeroValores);
   return(curva);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCap::DefValoresDefaultCapacitor(void)
   {
   //vari�veis locais
   VTCapacitor *ult_cap;
   VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //verifica se o Capacitor n�o est� definido
   if ((pcap->vnom == 0.)||(pcap->q == 0.))
      {//define Vnom
      pcap->vnom = pcap->pbarra->vnom;
      //define Q
      pcap->q    = 0.3; //MVAr
      //define tipo de liga��o
      pcap->ligacao = lgTRIANG;
      //seleciona Capacitor de refer�ncia p/ copiar dados
      lisEQP->Clear();
      redes->LisEqbar(lisEQP, eqptoCAPACITOR);
      if (lisEQP->Count == 0) return;
      if ((ult_cap = (VTCapacitor*)lisEQP->Last()) == NULL)  return;
      //copia dados
      //pcap->vnom    = ult_cap->vnom;
      pcap->q       = ult_cap->q;
      pcap->ligacao = ult_cap->ligacao;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCap::ExibeDadosCapacitor(void)
   {
   //exibe c�digo
   EditCodigo->Text = pcap->Codigo;
   //exibe vnom
   EditVnom->Text = DoubleToStr("%4.3f", pcap->vnom);
   //exibe pot.reativa
   EditQcap->Text = DoubleToStr("%4.3f", pcap->q);
   //seleciona tipo de liga��o
   switch(pcap->ligacao)
      {
      case lgEST_AT:  cboxLigacao->ItemIndex = 0; break;
      case lgTRIANG:  cboxLigacao->ItemIndex = 1; break;
      case lgEST_ISO: cboxLigacao->ItemIndex = 2; break;
      default       : cboxLigacao->ItemIndex = 0; break;
      }
   //pr�-seleciona Curva t�pica
   if (CBoxCurva->Items->Count > 0)
      {
      CBoxCurva->ItemIndex = CBoxCurva->Items->IndexOfObject(curva.tipica);
      if (CBoxCurva->ItemIndex < 0) CBoxCurva->ItemIndex = 0;
      }
   //pr�-seleciona tipo de curva: t�pica ou pr�pria
   if ((pcap->curva != NULL)&&(pcap->curva->Tipica))
      {
      RadioGroupTipoCurva->ItemIndex = 0;
      }
   else
      {
      RadioGroupTipoCurva->ItemIndex = 1;
      }
   //simula sele��o de tipo de curva
   //RadioGroupTipoCurvaClick(NULL);
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TFormEdtCap::ExisteCurvaTipica(void)
   {
   //vari�veis locais
   VTCurva  *curva;
   VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   //classe e energia definidos: detemina Curva t�pica
   curva = curvas->ExisteCurvaTipicaCapacitor();
   if (curva == NULL) Aviso("N�o existe curva t�pica p/ o capacitor");
   return(curva);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCap::FormClose(TObject *Sender,TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCap::FormCloseQuery(TObject *Sender, bool &CanClose)
	{
	//verifica se Capacitor ficou c/ Curva pr�pria
	if (pcap->curva == curva.propria)
		{//cancela ponteiro p/ Curva pr�pria
		curva.propria = NULL;
		}
	CanClose = true;
	}

//---------------------------------------------------------------------------
void __fastcall TFormEdtCap::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      geral->Infoset->LoadFromFile(this);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCap::FormShow(TObject *Sender)
   {
   //posiciona o Cursor em EditCodigo
   EditCodigo->SetFocus();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCap::IniciaCBoxCurva(void)
   {
   //vari�veis locais
   VTCurva  *curva;
   VTCurvas *curvas   = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
   TList    *lisCURVA = curvas->ListaCurvaTipicaCapacitor();

   //reinicia CBoxCurva
   CBoxCurva->Clear();
   for (int n = 0; n < lisCURVA->Count; n++)
      {
      curva = (VTCurva*)lisCURVA->Items[n];
      CBoxCurva->Items->AddObject(curva->Codigo, curva);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCap::IniciaCurvas(void)
   {
   //verifica se a Capacitor ainda n�o tem uma Curva associada
   if (pcap->curva == NULL)
      {//cria Curva pr�pria p/ Capacitor
      curva.propria = CriaCurva();
      //pr�-selciona Curva t�pica
      curva.tipica = ExisteCurvaTipica();
		}
	//verifica se a Curva do Capacitor � t�pica
	else if (pcap->curva->Tipica)
		{//cria Curva pr�pria p/ Capacitor
		curva.propria = CriaCurva();
      //salva Curva t�pica
      curva.tipica = pcap->curva;
		}
	else
		{//salva ponteiro p/ Curva pr�pria
		curva.propria = pcap->curva;
      //pr�-selciona Curva t�pica
      curva.tipica = ExisteCurvaTipica();
		}
   //prote��o: inicia Curva do Capacitor
   if (pcap->curva == NULL) pcap->DefineCurva(curva.tipica);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtCap::LeDadosCapacitor(void)
   {
   //vari�veis locais
   double q_cap;

   //verifica se foi selecionado o tipo de liga��o
   if ( cboxLigacao->ItemIndex < 0)
      {
      Aviso("Defina o tipo de liga��o do capacitor");
      return(false);
      }
   //verifica se foi selecionada uma Curva
	if (pcap->curva == NULL)
      {
      Aviso("Selecione a curva t�pica do capacitor");
      return(false);
      }
   try{//l� atributos do capacitor
      pcap->Codigo  = EditCodigo->Text;
      pcap->vnom    = StrToDouble(EditVnom->Text);
      //seleciona tipo de liga��o
      switch(cboxLigacao->ItemIndex)
         {
         case 0:  pcap->ligacao = lgEST_AT;  break;
         case 1:  pcap->ligacao = lgTRIANG;  break;
         case 2:  pcap->ligacao = lgEST_ISO; break;
         default: pcap->ligacao = lgTRIANG;  break;
         }
      //verifica se est� selecionada Curva t�pica ou pr�pria
      if (pcap->curva->Tipica)
         {//determina valor nomnal
         pcap->q = StrToDouble(EditQcap->Text);
         }
      else
         {//l� dados da Curva pr�pria
         if (! LePontosCurvaPropria()) return(false);
         //define valor nominal igual ao valor m�ximo da curva
         pcap->q = ValorMaximoCurvaPropria();
         }
      }catch(Exception &e)
         {
         Erro("Dados Inv�lidos no capacitor");
         return(false);
         }
   //valida Vnom
   if (IsDoubleZero(pcap->vnom))
      {
      Aviso("Defina tens�o nominal do capacitor");
      return(false);
      }
   //valida Pot.reativa
   if (IsDoubleZero(pcap->q))
      {
      Aviso("Defina pot�ncia do capacitor");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtCap::LePontosCurvaPropria(void)
   {
   //prote��o: verifica se Curva t�pica
   if (pcap->curva->Tipica) return(true);
	//l� pontos da Curva
	if (! FormEditaCurva->LePontosCurva()) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCap::OpcaoRetira(bool enabled)
   {
   butRetira->Visible = enabled;
	}

//---------------------------------------------------------------------------
void __fastcall TFormEdtCap::pgCtrlChange(TObject *Sender)
   {
	//verifica se est� selecionado visualiza��o da Curva
   if (pgCtrl->ActivePage == TabSheetGrafico)
      {//atualiza FormExibeCurva
		FormExibeCurva->DefineCurva(pcap, pcap->curva);
		FormExibeCurva->HabilitaCurva(cap_Q);
		}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCap::pgCtrlChanging(TObject *Sender, bool &AllowChange)
   {
   //verifica se o Capacitor est� associado a uma Curva
   if (pcap->curva == NULL)
      {
      Aviso("Selecione a curva do capacitor");
      AllowChange = false;
      return;
      }
   //verifica se deve ler dados editados
   if (pgCtrl->ActivePage != TabSheetGrafico)
      {//l� os dados editados
		AllowChange = LeDadosCapacitor();
      }
   }
//---------------------------------------------------------------------------
double __fastcall TFormEdtCap::ValorMaximoCurvaPropria(void)
   {
   //vari�veis locais
   strHM      hm_ini, hm_fim;
   double     q_max = 0;
   double     valor[nvCURVA_PQVT];

   //preenche dados dos pontos da Curva
   for(int np = 0; np < patamares->NumPatamar(); np++)
      {//obt�m ponto da Curva p/ o Patamar
      if (! curva.propria->GetPonto(np, hm_ini, hm_fim, valor, curva.propria->NumeroValores)) break;
      //determina valor m�ximo
      if (fabs(valor[indQ]) > q_max) q_max = valor[indQ];
      }
   return(q_max);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCap::RadioGroupTipoCurvaClick(TObject *Sender)
   {
   //verifica se est� selecionada op��o de Curva t�pica
   if (RadioGroupTipoCurva->ItemIndex == 0)
		{//redefine Curva tipica
		pcap->curva = curva.tipica;
      //exibe PanelTipica
      GBoxTipica->Visible = true;
      //desabilita edi��o da Curva
      TabSheetEdicao->TabVisible = false;
      }
   else
      {//redefine Curva pr�pria
      pcap->curva = curva.propria;
      //esconde PanelTipica
		GBoxTipica->Visible = false;
      //habilita edi��o da Curva
      TabSheetEdicao->TabVisible = true;
      }
   }

//---------------------------------------------------------------------------
//eof

