//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#include <algorithm>  //min(), max
#pragma hdrstop
#include "TFormEdtTrfMono.h"
#include "TFormEdtFoto.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Fases.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Geral\VTGeral.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;

//---------------------------------------------------------------------------
__fastcall TFormEdtTrfMono::TFormEdtTrfMono(TComponent* Owner, VTApl *apl, VTTrafo *trafo, bool criar_copia)
                       : TForm(Owner)
   {
   //salva opção de criar cópia
   this->criar_copia = criar_copia;
   //salva ponteiro p/ objetos
   this->apl    = apl;
   geral        = (VTGeral*)apl->GetObject(__classid(VTGeral));
   //cria lista
   lisTRF = new TList();
   lisTMP = new TList();
   //salva Trafo em lisTRF
   lisTRF->Add(trafo);
   //inicia dados
   aviso_enabled = false;
   //cria um clone do Trafo que será uado durante edição
   clone = trafo->Clone();
   //inicia CLBoxAtributo
//   CLBoxAtributoInicia();
   //desabilita TabSheetAtributo
//   TabSheetAtributo->TabVisible = false
   //FKM 2015.09.03 /* "Combobox" do LTC estava bloqueado, não sei o motivo
   gboxTap->Enabled = true;
	//FKM 2015.09.03                                                         */
	 //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtTrfMono::TFormEdtTrfMono(TComponent* Owner, VTApl *apl, TList *lisEXT, bool criar_copia)
                           : TForm(Owner)
   {
   //variáveis locais
   VTTrafo *trafo;
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //salva opção de criar cópia
   this->criar_copia = criar_copia;
   //salva ponteiro p/ objeto
   this->apl = apl;
   geral     = (VTGeral*)apl->GetObject(__classid(VTGeral));
   //proteção
   if (lisEXT->Count == 0)
      {
      Close();
      return;
      }
   //cria lista
   lisTRF = new TList();
   lisTMP = new TList();
   //salva Trafos de lisEXT em lisTRF
   CopiaTList(lisEXT, lisTRF);
   //cria um clone de um Trafo que será uado durante edição
   trafo = (VTTrafo*)lisEXT->First();
   clone = trafo->Clone();
   //inicia dados
   aviso_enabled = false;
   //inicia CLBoxAtributo
//   CLBoxAtributoInicia();
   //habilita TabSheetAtributo
//   TabSheetAtributo->TabVisible = (lisTRF->Count > 1);
   //posiciona o Form
//   FormIniciaPosicao();
   //FKM 2015.09.03 /* "Combobox" do LTC estava bloqueado, não sei o motivo
   gboxTap->Enabled = true;
	//FKM 2015.09.03                                                         */
	FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtTrfMono::~TFormEdtTrfMono(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //apaga eventual moldura
//   Moldura();
   //destrói clone do Trafo usado na edição
   if (clone)  {delete clone; clone = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisTMP)  {delete lisTMP; lisTMP = NULL;}
   if (lisTRF)  {delete lisTRF; lisTRF = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::ActionConfirmaExecute(TObject *Sender)
   { //fkm 2015.09.02 tirei a dupla checagem do ledadostrafo
   //habilita mensagens de aviso/erro
   aviso_enabled = true;
   //valida dados do Trafo
   if (! LeDadosTrafo())     {aviso_enabled = false; return;}
   //if (! LeDadosTrafo())     {aviso_enabled = false; return;}
   if (! ValidaDadosTrafo()) return;
   /*
   //verifica se é editor de planejamento
   if (FormEdtPlan)
      {//insere Eqpto no planejamento
      if (! FormEdtPlan->ConfirmaInsereEqpto(trafo)) return;
      }
   */
   //atualiza dados do Trafo original a partir do clone
   AtualizaDados();
   //fecha o form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::ActionRetiraExecute(TObject *Sender)
   {
   /*
   //verifica se é editor de planejamento
   if (FormEdtPlan)
      {//retira Eqpto no planejamento
      FormEdtPlan->CancelaInsereEqpto(trafo);
      }
   */
   //fecha o form indicando exclusão do Eqpto
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::ActionSelFotoExecute(TObject *Sender)
   {
   //variáveis locais
   TFormEdtFoto *form;

   try{//cria Form e exibe como janela modal
      form = new TFormEdtFoto(this, apl, clone);
      form->ShowModal();
      delete form;
      }catch(Exception &e)
         {
         }

   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::AtualizaDados(void)
   {
   //variáveis locais
   VTTrafo *trafo;

   //atualiza todos os Trafos em lisTRF: somente os campos habilitados
   for (int n = 0; n < lisTRF->Count; n++)
      {
      trafo = (VTTrafo*)lisTRF->Items[n];
      //verifica se deve criar uma cópia
      if (criar_copia)
         {//cria um clone do Trafo e associa com trafo original
         trafo->Obj = trafo->Clone();
         //redefine trafo para apontar para a cópia
         trafo = (VTTrafo*)(trafo->Obj);
         }
      //atualiza atributos (do trafo original ou de sua cópia)
      trafo->CopiaAtributosDe(*clone);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::CBoxBarPriChange(TObject *Sender)
   {
   //variáveis locais
   AnsiString vnom_pri, tap_pri;

   //proteção: verifica se houve alteração da Barra
   if (CBoxBarPri->ItemIndex != CBoxBarSec->ItemIndex) return;
   //altera Vnom do secundário
   CBoxBarSec->ItemIndex = 1 - CBoxBarPri->ItemIndex;
   //troca Vnom primário/secundário
   vnom_pri          = EditVnomPri->Text;
   EditVnomPri->Text = EditVnomSec->Text;
   EditVnomSec->Text = vnom_pri;
   //troca Tap primário/secundário
   tap_pri          = EditTapPri->Text;
   EditTapPri->Text = EditTapSec->Text;
   EditTapSec->Text = tap_pri;
   //atualiza Vnom dos Enrolamentos
   //DefineVnomTrafo();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::CBoxBarSecChange(TObject *Sender)
   {
   //variáveis locais
   AnsiString vnom_pri, tap_pri;

   //proteção: verifica se houve alteração da Barra
   if (CBoxBarSec->ItemIndex != CBoxBarPri->ItemIndex) return;
   //altera Vnom do secundário
   CBoxBarPri->ItemIndex = 1 - CBoxBarSec->ItemIndex;
   //troca Vnom primário/secundário
   vnom_pri          = EditVnomPri->Text;
   EditVnomPri->Text = EditVnomSec->Text;
   EditVnomSec->Text = vnom_pri;
   //troca Tap primário/secundário
   tap_pri          = EditTapPri->Text;
   EditTapPri->Text = EditTapSec->Text;
   EditTapSec->Text = tap_pri;
   //atualiza Vnom dos Enrolamentos
   //DefineVnomTrafo();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::CBoxFasePriChange(TObject *Sender)
   {
   //redefine Vnom/Vtap do trafo
   DefineVnomVtap();
   ExibeVnomVtap();
   //configura campos condicionais
   ExibeCamposCondicionais();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::CBoxFaseSecChange(TObject *Sender)
   {
   //redefine Vnom/Vtap do trafo
   DefineVnomVtap();
   ExibeVnomVtap();
   //configura campos condicionais
   ExibeCamposCondicionais();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::CBoxLtcTipoChange(TObject *Sender)
   {
   //redefine Vnom/Vtap do trafo
   DefineVnomVtap();
   //configura cmapos condicionais
   ExibeCamposCondicionais();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::CBoxModeloChange(TObject *Sender)
   {
   //reinicia CBoxFaseSec
   IniciaCBoxFaseSec();
   //redefine Vnom/Vtap do trafo
   DefineVnomVtap();
   //configura cmapos condicionais
   ExibeCamposCondicionais();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::DefineValoresDefault(void)
   {
   //variáveis locais
   VTTrafo *ult_trf;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //verifica se o Trafo não está definido
   if ((clone->snom == 0.)||(clone->pri.vnom == 0.)||(clone->sec.vnom == 0.))
      {//inicia dados gerais c/ valores default
      clone->auto_trafo  = false;
      clone->snom        = 0.050;  //50 kVA
      clone->perda_ferro = 0.1;
      clone->z0.r        = 0.;
      clone->z0.x        = 0.05;
      clone->z1.r        = 0.;
      clone->z1.x        = 0.05;
      //inicia dados do primário
      clone->pri.ligacao   = lgMONOFASICO;
      clone->pri.fase      = faseAB;
      //clone->pri.vnom      = vat;
      clone->pri.zterra.r  = 0;
      clone->pri.zterra.x  = 0;
      //inicia dados do secundário
      clone->sec.ligacao   = lgMONOFASICO;
      clone->sec.fase      = faseAB;
      //clone->sec.vnom      = vmt;
      clone->sec.zterra.r  = 0;
      clone->sec.zterra.x  = 0;
      //define Vnom/Vtap p/ primário e secundário
      DefineVnomVtap();
      //inicia dados de LTC
		clone->ltc.tipo             = ltcFIXO;
		clone->ltc.ajuste_auto.v_pu = 1.0;
		//seleciona Trafo de referência p/ copiar alguns dados
      lisTMP->Clear();
      redes->LisLigacao(lisTMP, eqptoTRAFO);
      //determina Trafo monofásico
      for (int n = lisTMP->Count-1; n >= 0; n--)
         {
         ult_trf = (VTTrafo*)lisTMP->Items[n];
         if (! ult_trf->Monofasico) continue;
         //inicia dados gerais
         clone->snom   = ult_trf->snom;
         clone->z0     = ult_trf->z0;
         clone->z1     = ult_trf->z1;
         //inicia dados do primário
         clone->pri.zterra  = ult_trf->pri.zterra;
         //inicia dados do secundário
         clone->sec.zterra  = ult_trf->sec.zterra;
         break;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::DefineVnomVtap(void)
   {
   //variáveis locais
   double  v_pri     = clone->pbarra1->vnom;
   double  v_sec     = clone->pbarra2->vnom;
   TFase   *fase_pri = (TFase*)CBoxFasePri->Items->Objects[CBoxFasePri->ItemIndex];
   TFase   *fase_sec = (TFase*)CBoxFaseSec->Items->Objects[CBoxFaseSec->ItemIndex];
   VTFases *fases    = (VTFases*)apl->GetObject(__classid(VTFases));

   //verifica se trafo elevador
   if (v_pri > v_sec)
      {//determina Vnom do primário
      if (fases->NumeroDeFases(fase_pri->tag) == 1) clone->pri.vnom = v_pri / sqrt(3.);
      else                                          clone->pri.vnom = v_pri;
      //determina Vnom do secundário
      //if (fases->NumeroDeFases(fase_sec->tag) == 1) clone->sec.vnom = v_sec / 2.;
      //else                                          clone->sec.vnom = v_sec;
      clone->sec.vnom = v_sec;
      }
   else
      {//determina Vnom do primário
      //if (fases->NumeroDeFases(fase_pri->tag) == 1) clone->pri.vnom = v_pri / 2.;
      //else                                          clone->pri.vnom = v_pri;
      clone->pri.vnom = v_pri;
      //determina Vnom do secundário
      if (fases->NumeroDeFases(fase_sec->tag) == 1) clone->sec.vnom = v_sec /  sqrt(3.);
      else                                          clone->sec.vnom = v_sec;
      }
   clone->pri.tap       = clone->pri.vnom;
   clone->sec.tap       = clone->sec.vnom;
   clone->pri.defasagem = 0.;
   clone->sec.defasagem = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::DeterminaFasesDisponiveis(void)
   {
   /*
   //variáveis locais
   int       fases_lig;
   VTBarra   *barra;
   VTLigacao *ligacao;
   VTFases   *fases = (VTFases*)apl->GetObject(__classid(VTFases));
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //inicia lisBAR c/ as Barras do Trecho
   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {//assume que nenhuma fase está disponível na Barra em análise
      fases_disp[ind_bar] = faseINV;
      if ((barra = clone->Barra(ind_bar)) == NULL) continue;
      //obtém lista de Ligacoes que conectam a Barra
      lisLIG->Clear();
      redes->LisLigacao(lisLIG, barra);
      //loop p/ todas Ligacoes
      for (int nl = 0; nl < lisLIG->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLIG->Items[nl];
         //verifica se a Ligacao é o próprio Trecho (CUIDADO: clone é clone do Trecho original)
         if ((ligacao->Tipo() == eqptoTRAFO)&&(ligacao->Id == clone->Id)) continue;
         //verifica se as fases da Ligacao são válidas
         if ((fases_lig = ligacao->Fases(barra)) == faseINV) continue;
         //verifica se a Barra ainda não tem fase definida
         if (fases_disp[ind_bar] == faseINV) fases_disp[ind_bar] = fases_lig;
         //verifica se as fases da Ligacao são iguais às fases disponíveis na Barra
         if (fases_lig == fases_disp[ind_bar]) continue;
         //define fases disponíveis: uniao das fases disponíveis p/ a Barra c/ as fases da Ligacao
         fases_disp[ind_bar] = fases->Uniao(fases_disp[ind_bar], fases_lig);
         }
      }
   //proteção final: redefine Barras que ficaram c/ fases inválidas
   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {
      if (fases_disp[ind_bar] == faseINV) fases_disp[ind_bar] = faseABCNT;
      }
*/
   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++) fases_disp[ind_bar] = faseABCNT;
   }

//---------------------------------------------------------------------------
double __fastcall TFormEdtTrfMono::DeterminaVnomFFouFN(int fase, double vnom_bar, double fat_mult)
   {
   //variáveis locais
   double vnom;
   int    num_fase;
   VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

   //determina número de fases
   num_fase = 0;
   if (fases->Fase1ContemFase2(fase, faseA)) num_fase++;
   if (fases->Fase1ContemFase2(fase, faseB)) num_fase++;
   if (fases->Fase1ContemFase2(fase, faseC)) num_fase++;
   if (num_fase == 1)
      {//tensão de fase/neutro
      vnom = vnom = vnom_bar * fat_mult;
      }
   else
      {//tensão fase/fase
      vnom = vnom_bar;
      }
   return(vnom);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::ExibeCamposCondicionais(void)
   {
   //variáveis locais
   TFase   *fase;
   VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

   //aterramento do primário
   if (CBoxFasePri->ItemIndex < 0)
      {
      GBoxZatPri->Visible = false;
      }
   else
      {
      fase = (TFase*)CBoxFasePri->Items->Objects[CBoxFasePri->ItemIndex];
      GBoxZatPri->Visible  = (fases->Fase1ContemFase2(fase->tag, faseT));
      }
   //aterramento do secundário
   if (CBoxFaseSec->ItemIndex < 0)
      {
      GBoxZatSec->Visible = false;
      }
   else
      {
      fase = (TFase*)CBoxFaseSec->Items->Objects[CBoxFaseSec->ItemIndex];
      GBoxZatSec->Visible  = (fases->Fase1ContemFase2(fase->tag, faseT));
      }
   //exibe campos condicionais em relação a LTC
   switch (CBoxLtcTipo->ItemIndex)
      {
      case 1: //ajuste automático no tap do primário
         //desabilita edição de Vtap do primário
         //LabelTapPri->Visible = false;
         //EditTapPri->Visible  = false;
         //habilita edição de Vtap do secundário
         LabelTapSec->Visible = true;
         EditTapSec->Visible  = true;
         //habilita seleção da barra de referência e da edição de tensão controlada
         GBoxLtcBar->Visible  = true;
         GBoxLtcVpu->Visible  = true;
         break;
      case 2:  //ajuste automático no tap do secundário
         //habilita edição de Vtap do primário
         LabelTapPri->Visible = true;
         EditTapPri->Visible  = true;
         //desabilita edição de Vtap do secundário
         //LabelTapSec->Visible = false;
         //EditTapSec->Visible  = false;
         //habilita seleção da barra de referência e da edição de tensão controlada
         GBoxLtcBar->Visible  = true;
         GBoxLtcVpu->Visible  = true;
         break;
      default:  //não utiliza LTC
         //habilita edição de Vtap do primário
         LabelTapPri->Visible = true;
         EditTapPri->Visible  = true;
         //habilita edição de Vtap do secundário
         LabelTapSec->Visible = true;
         EditTapSec->Visible  = true;
         //desabilita seleção da barra de referência e da edição de tensão controlada
         GBoxLtcBar->Visible  = false;
         GBoxLtcVpu->Visible  = false;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::ExibeDadosTrafo(void)
   {
   //variáveis locais
   VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	double tap1, tap2;

   //exibe dados gerais do Trafo
   CheckAutoTrafo->Checked = clone->auto_trafo;
   EditCodigo->Text    = clone->Codigo;
   EditSnom->Text      = DoubleToStr("%4.3f", clone->snom * 1000);  //MVA =>kVA
   EditPerdaFe->Text   = DoubleToStr("%4.3f", clone->perda_ferro);
   //exibe fases do primário/secundário
   CBoxFasePri->ItemIndex = CBoxFasePri->Items->IndexOfObject(fases->TagToFase[clone->pri.fase]);
   CBoxFaseSec->ItemIndex = CBoxFaseSec->Items->IndexOfObject(fases->TagToFase[clone->sec.fase]);
   //exibe dados de impedância em pu
   EditR->Text     = DoubleToStr("%6.5f", clone->z1.r);
   EditX->Text     = DoubleToStr("%6.5f", clone->z1.x);
   //exibe Vnom/Vtap dos Enrolamentos
   ExibeVnomVtap();
   //exibe dados do primário
   EditRatPri->Text = DoubleToStr("%2.1f", clone->pri.zterra.r);
   EditXatPri->Text = DoubleToStr("%2.1f", clone->pri.zterra.x);
   //exibe dados do secundário
   EditRatSec->Text = DoubleToStr("%2.1f", clone->sec.zterra.r);
   EditXatSec->Text = DoubleToStr("%2.1f", clone->sec.zterra.x);
   //seleciona opção de LTC
	if      (clone->ltc.tipo == ltcPRIMARIO)   CBoxLtcTipo->ItemIndex = 1;
	else if (clone->ltc.tipo == ltcSECUNDARIO) CBoxLtcTipo->ItemIndex = 2;
	else                                       CBoxLtcTipo->ItemIndex = 0;
   //exibe tensão controlada
	EditLtcVpu->Text = DoubleToStr("%5.4f", clone->ltc.ajuste_auto.v_pu);
	//exibe taps
	tap1 = ((clone->pri.tap)
		- StrToDouble(EditVnomPri->Text))/(StrToDouble(EditVnomPri->Text)*0.05);
	tap2 = ((clone->sec.tap)
		- StrToDouble(EditVnomSec->Text))/(StrToDouble(EditVnomSec->Text)*0.05);
	LabelNumTap1->Caption = DoubleToStr("%1.0f",tap1,'.') + ")";
	LabelNumTap2->Caption = DoubleToStr("%1.0f",tap2,'.') + ")";
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::DeterminaVnomTrafo(void)
   {
   //variáveis locais
   double  vnom;
   double  fat_mult;
   VTBarra *pbarra;
   TFase   *fase;

   //exibe Vnom do Primário
   pbarra   = (VTBarra*)(CBoxBarPri->Items->Objects[CBoxBarPri->ItemIndex]);
   fase     = (TFase*)CBoxFasePri->Items->Objects[CBoxFasePri->ItemIndex];
   fat_mult = 1./sqrt(3.);
   vnom     = DeterminaVnomFFouFN(fase->tag, pbarra->vnom, fat_mult);
   EditVnomPri->Text = DoubleToStr("%4.3f", vnom);
   //exibe Vnom do Secundário
   pbarra   = (VTBarra*)(CBoxBarSec->Items->Objects[CBoxBarSec->ItemIndex]);
   fase     = (TFase*)CBoxFaseSec->Items->Objects[CBoxFaseSec->ItemIndex];
   fat_mult = 0.5;
   vnom     = DeterminaVnomFFouFN(fase->tag, pbarra->vnom, fat_mult);
   EditVnomSec->Text = DoubleToStr("%4.3f", vnom);
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::ExibeVnomVtap(void)
   {
   EditVnomPri->Text = DoubleToStr("%4.3f", clone->pri.vnom);
   EditTapPri->Text = DoubleToStr("%4.3f", clone->pri.tap);
   EditVnomSec->Text = DoubleToStr("%4.3f", clone->sec.vnom);
   EditTapSec->Text = DoubleToStr("%4.3f", clone->sec.tap);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
		if (geral->Infoset->LoadFromFile(this)) return;
		}
	//posiciona o Form sem alinhamento
  /*	Left = 0;
	Top  = 0;*/
	}

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::FormShow(TObject *Sender)
   {
   //determina fases admissíveis p/ o Trecho em função das Ligacoes vizinhas
   DeterminaFasesDisponiveis();
   //inicia CBoxBarra c/ as barras do Trafo
   IniciaCBoxBarraPriSec();
   IniciaCBoxBarraControlada();
   //inicia CBox c/ as Redes
   IniciaCBoxRede();
   //inicia CBox
   IniciaCBoxModelo();
   IniciaCBoxFasePri();
   IniciaCBoxFaseSec();
   //se necessário, define valores default p/ Trecho
   DefineValoresDefault();
   //inicia Captions,Labels etc da tela
   IniciaParteFixaTela();
   //exibe dados do Trafo
   ExibeDadosTrafo();
   ExibeCamposCondicionais();
   //seleciona TableSheet
   pgCtrl->ActivePage = TabSheetTrafo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::IniciaCBoxBarraPriSec(void)
   {
   //variáveis locais
   VTBarra     *pbar[2];
   AnsiString  nomeBarra;

   //monta vetor c/ Barras ordenadas: primário, secundário, terciário
   pbar[0] = clone->pbarra1;
   pbar[1] = clone->pbarra2;
   //reinicia CBoxVnomPri, CBoxVnomSec
   CBoxBarPri->Clear();
   CBoxBarSec->Clear();
   for (int n = 0; n < 2; n++)
      {
      //FKM 2015.09.03   /* Não exibe Codigo vazio
      if(pbar[n]->Codigo.IsEmpty()){nomeBarra = "(Id = " + IntToStr(pbar[n]->Id) +")";}
      else{ nomeBarra = pbar[n]->Codigo;}
      CBoxBarPri->Items->AddObject(nomeBarra, pbar[n]);
      CBoxBarSec->Items->AddObject(nomeBarra, pbar[n]);
      //FKM 2015.09.03   ************************** */
      //CBoxBarPri->Items->AddObject(pbar[n]->Codigo, pbar[n]);
      //CBoxBarSec->Items->AddObject(pbar[n]->Codigo, pbar[n]);
      }
   //seleciona Barra/Vnom do primário, secundário e terciário
   CBoxBarPri->ItemIndex = 0;
   CBoxBarSec->ItemIndex = 1;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::IniciaCBoxBarraControlada(void)
   {
   //variáveis locais
   VTBarra *pbarra;
   VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   AnsiString  nomeBarra;
   //TList   *lisBAR = redes->LisBarra();

   //reinicia CBoxBarRef
   CBoxLtcBar->Clear();
//   //loop p/ todas Barras
//   for (int n = 0; n < lisBAR->Count; n++)
//      {
//      pbarra = (VTBarra*)lisBAR->Items[n];
//      CBoxLtcBar->Items->AddObject(pbarra->Codigo, pbarra);
//      }
   //FKM - 2015.09.03 /*Listar somente as duas barras do trafo
   if(clone->pbarra1->Codigo.IsEmpty()){ nomeBarra = "(Id = " + IntToStr(clone->pbarra1->Id) +")";}
   else{nomeBarra = clone->pbarra1->Codigo;}
   CBoxLtcBar->Items->AddObject(nomeBarra, clone->pbarra1);
   if(clone->pbarra2->Codigo.IsEmpty()){ nomeBarra = "(Id = " + IntToStr(clone->pbarra2->Id) +")";}
   else{nomeBarra = clone->pbarra2->Codigo;}
   CBoxLtcBar->Items->AddObject(nomeBarra, clone->pbarra2);
   //FKM - 2015.09.03 **************************************** */
   //habilita ordem alfabética
   CBoxLtcBar->Sorted = true;
   //verifica se há Barras no CBox
   if (CBoxLtcBar->Items->Count > 0)
      {//pré-seleciona Barra controlada
		if (clone->ltc.ajuste_auto.bar_ref)
			{
			CBoxLtcBar->ItemIndex = CBoxLtcBar->Items->IndexOfObject(clone->ltc.ajuste_auto.bar_ref);
			}
      //pré-seleciona primeira Barra
      if (CBoxLtcBar->ItemIndex < 0) CBoxLtcBar->ItemIndex = 0;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::IniciaCBoxFasePri(void)
   {
   //variáveis locais
   int     ind_bar;
   TFase   *fase;
   VTFases *fases   = (VTFases*)apl->GetObject(__classid(VTFases));
   TList   *lisFASE = fases->LisFasesTrafoMonoPrimario();

   //determina ínidce da Barra do primário
   ind_bar = clone->IndiceBarra(clone->BarraPrimario());
   //reinicia CBoxFasePri
   CBoxFasePri->Clear();
   for (int n = 0; n < lisFASE->Count; n++)
      {
      fase = (TFase*)lisFASE->Items[n];
      //verifica se as fases disponíveis na Barra contém a fase
      if (fases->Fase1ContemFase2(fases_disp[ind_bar], fase->tag))
         {
         CBoxFasePri->Items->AddObject(fase->codigo, fase);
         }
      }
   //pré-seleciona primeiro elemento
   if (CBoxFasePri->Items->Count > 0) CBoxFasePri->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::IniciaCBoxFaseSec(void)
   {
   //variáveis locais
   bool    mono_5terminais;
   int     ind_bar;
   TFase   *fase_ant = NULL;
   TFase   *fase;
   TList   *lisFASE;
   VTFases *fases   = (VTFases*)apl->GetObject(__classid(VTFases));

   //determina ínidce da Barra do primário
   ind_bar = clone->IndiceBarra(clone->BarraSecundario());
   //obtém lista de Fases do secundário com ou sem neutro
   mono_5terminais = (CBoxModelo->ItemIndex == 1);
   lisFASE    = fases->LisFasesTrafoMonoSecundario(mono_5terminais);
   //salva Fase eventualmente selecionada
   if (CBoxFaseSec->ItemIndex >= 0)
      {
      fase_ant = (TFase*)CBoxFaseSec->Items->Objects[CBoxFaseSec->ItemIndex];
      }
   //reinicia CBoxFaseSec
   CBoxFaseSec->Clear();
   for (int n = 0; n < lisFASE->Count; n++)
      {
      fase = (TFase*)lisFASE->Items[n];
      //verifica se as fases disponíveis na Barra contém a fase
      if (fases->Fase1ContemFase2(fases_disp[ind_bar], fase->tag))
         {
         CBoxFaseSec->Items->AddObject(fase->codigo, fase);
         }
      }
   //torna a selecionar a Fase selecionada anteriormente
   if (fase_ant) CBoxFaseSec->ItemIndex = CBoxFaseSec->Items->IndexOfObject(fase_ant);
   //se não há uma Fase selecionada, pré-seleciona primeiro elemento
   if ((CBoxFaseSec->ItemIndex < 0)&&(CBoxFaseSec->Items->Count > 0)) CBoxFaseSec->ItemIndex = 0;
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::IniciaCBoxModelo(void)
   {
   //variáveis locais
   VTFases *fases   = (VTFases*)apl->GetObject(__classid(VTFases));

   //verifica se o secundário possui neutro
   CBoxModelo->ItemIndex = (fases->Fase1ContemFase2(clone->sec.fase, faseN)) ? 1 : 0;
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::IniciaCBoxModelo(void)
   {
   //variáveis locais
   int     num_fase;
   VTFases *fases   = (VTFases*)apl->GetObject(__classid(VTFases));

   //determina número de fases do secundário
   num_fase = 0;
   if (fases->Fase1ContemFase2(clone->sec.fase, faseA)) num_fase++;
   if (fases->Fase1ContemFase2(clone->sec.fase, faseB)) num_fase++;
   if (fases->Fase1ContemFase2(clone->sec.fase, faseC)) num_fase++;
   if (fases->Fase1ContemFase2(clone->sec.fase, faseN)) num_fase++;
   if (num_fase <= 2) CBoxModelo->ItemIndex = 0;
   else               CBoxModelo->ItemIndex = 1;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::IniciaCBoxRede(void)
   {
   //variáveis locais
   int      index;
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
   //pré-seleciona Rede que conecta Barras do Trafo
   index = 0;
   if ((rede = SelecionaRede(lisREDE)) != NULL) index = CBoxRede->Items->IndexOfObject(rede);
   CBoxRede->ItemIndex = index;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::IniciaParteFixaTela(void)
   {
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrfMono::LeDadosTrafo(void)
   { //fkm 2015.09.02
   //variáveis locais
   TFase   *fase_pri, *fase_sec;
   VTBarra *pbar;

   //proteção: verifica seleção dos ComboBox
   if (CBoxRede->ItemIndex < 0)
      {
      Aviso("Defina a Rede do Transformador");
      return(false);
      }
   if ((CBoxFasePri->ItemIndex < 0)||(CBoxFaseSec->ItemIndex < 0))
      {
      Aviso("Defina as fases do primário e do secundário");
      return(false);
      }
   try{//determina Rede do Trafo
      clone->rede        = (VTRede*)CBoxRede->Items->Objects[CBoxRede->ItemIndex];
      //dados gerais
      clone->auto_trafo  = CheckAutoTrafo->Checked;
      clone->Codigo      = EditCodigo->Text;
      clone->snom        = StrToDouble(EditSnom->Text) /1000.;  //kVA => MVA
      clone->perda_ferro = StrToDouble(EditPerdaFe->Text); //%
      //fkm 2015.09.02 inseri a verificação de valor nulo na impedancia
      if(IsDoubleZero(StrToDouble(EditR->Text)) && IsDoubleZero(StrToDouble(EditX->Text)))
      {
         Aviso("Defina o valor não nulo para o Zcurto.");
         return(false);
      }
      clone->z1.r        = StrToDouble(EditR->Text);      //pu
      clone->z1.x        = StrToDouble(EditX->Text);      //pu
      clone->z0          = clone->z1;
      //fases do primário e do secundário
      fase_pri            = (TFase*)CBoxFasePri->Items->Objects[CBoxFasePri->ItemIndex];
      fase_sec            = (TFase*)CBoxFaseSec->Items->Objects[CBoxFaseSec->ItemIndex];
      clone->pri.fase    = fase_pri->tag;
      clone->sec.fase    = fase_sec->tag;
      //opção de uso de LTC
		if      (CBoxLtcTipo->ItemIndex == 1) clone->ltc.tipo = ltcPRIMARIO;
		else if (CBoxLtcTipo->ItemIndex == 2) clone->ltc.tipo = ltcSECUNDARIO;
		else                                  clone->ltc.tipo = ltcFIXO;
      //define valores default p/ dados cuja edição pode estar bloqueada
      clone->pri.zterra.r  = 0.;
      clone->pri.zterra.x  = 0.;
      clone->sec.zterra.r  = 0.;
      clone->sec.zterra.x  = 0.;
      clone->sec.defasagem = 0.;
      //dados do primário
      clone->pri.vnom = StrToDouble(EditVnomPri->Text);
      clone->pri.tap  = StrToDouble(EditTapPri->Text);
		if (clone->ltc.tipo == ltcPRIMARIO)
         {//primário com LTC:  tap não editável ==> garante valor válido
         if (clone->pri.tap == 0.) clone->pri.tap = clone->pri.vnom;
         }
      //define tipo da Ligacao do primário
      clone->pri.ligacao = lgMONOFASICO;
      //lê Zterra do primário
      if (GBoxZatPri->Visible)
         {//lê Zterra do primário somente se a edição estiver habilitada
         clone->pri.zterra.r = StrToDouble(EditRatPri->Text);    //ohm
         clone->pri.zterra.x = StrToDouble(EditXatPri->Text);    //ohm
         }
      //dados do secundário
      clone->sec.vnom = StrToDouble(EditVnomSec->Text);
      clone->sec.tap  = StrToDouble(EditTapSec->Text);
		if (clone->ltc.tipo == ltcSECUNDARIO)
         {//secundário com LTC:  tap não editável ==> garante valor válido
         if (clone->sec.tap == 0.) clone->sec.tap = clone->sec.vnom;
         }
      //define tipo da Ligacao do secundário
      clone->sec.ligacao = lgMONOFASICO;
      //lê Zterra do secundário
      if (GBoxZatSec->Visible)
         {
         clone->sec.zterra.r = StrToDouble(EditRatSec->Text);    //ohm
         clone->sec.zterra.x = StrToDouble(EditXatSec->Text);    //ohm
         }
      //define defasagem do secundário
      clone->sec.defasagem = 0.;
      //lê tensões controladas somente se LTC selecionado
		clone->ltc.ajuste_auto.bar_ref = NULL;
		if (clone->ltc.tipo != ltcFIXO)
         {//determina Barra controlada
			clone->ltc.ajuste_auto.bar_ref = (VTBarra*)CBoxLtcBar->Items->Objects[CBoxLtcBar->ItemIndex];
         //lê tensão da barra controlada
			clone->ltc.ajuste_auto.v_pu = StrToDouble(EditLtcVpu->Text);
         }
      }catch(Exception &e)
         {
         Erro("Dados Inválidos no Trafo");
         return false;
         }

   //redefine Barras do trafo
   pbar = (VTBarra*)CBoxBarPri->Items->Objects[CBoxBarPri->ItemIndex];
   clone->RedefineBarraDoPrimario(pbar);
   return true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::OpcaoRetira(bool enabled)
   {
   ActionRetira->Visible = enabled;
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TFormEdtTrfMono::SelecionaRede(TList *lisREDE)
   {
   //variáveis locais
   VTRede  *rede;

   //verifica se o Trafo já possui uma Rede definida
   if (clone->rede) return(clone->rede);
   //procura uma Rede que conecta as Barras do Trafo
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if ((rede->ExisteBarra(clone->pbarra1))||
          (rede->ExisteBarra(clone->pbarra2))) return(rede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------//---------------------------------------------------------------------------//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::UpDownPriClick(TObject *Sender,
      TUDBtnType Button)
	{
	//var local
	double tap_pri, novo_tap;

	//inicializa botão
	tap_pri = StrToDouble(EditVnomPri->Text);
	UpDownPri->Min = -5;
	UpDownPri->Max = 5;
	//troca Tap primário para mais ou menos 5% do valor nominal
	novo_tap = (UpDownPri->Position)*0.05*tap_pri;
	EditTapPri->Text = DoubleToStr("%4.3f",novo_tap+tap_pri,'.');
	LabelNumTap1->Caption = IntToStr(UpDownPri->Position) + ")";
	}

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrfMono::UpDownSecClick(TObject *Sender,
      TUDBtnType Button)
	{
	//var local
	double tap_sec, novo_tap;

	//inicializa botão
	tap_sec = StrToDouble(EditVnomSec->Text);
	UpDownSec->Min = -5;
	UpDownSec->Max = 5;
	//troca Tap primário para mais ou menos 5% do valor nominal
	novo_tap = (UpDownSec->Position)*0.05*tap_sec;
	EditTapSec->Text = DoubleToStr("%4.3f",novo_tap+tap_sec,'.');
	LabelNumTap2->Caption = IntToStr(UpDownSec->Position) + ")";
	}

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrfMono::ValidaDadosTrafo(void)
   {
   //valida Snom
   if (clone->snom == 0.)
      {
      Aviso("Defina potência nominal do transformador");
      return(false);
      }
   //valida Vnom do primário e secundário
   if ((clone->pri.vnom == 0.)||(clone->sec.vnom == 0.))
      {
      Aviso("Defina tensões nominais do transformador");
      return(false);
      }
   //valida Tap
   if (! ValidaTap()) return(false);
   //valida perda no ferro
	if (clone->perda_ferro < 0.)
      {
      Aviso("Perda no ferro com valor negativo");
      return(false);
      }
   //valida impedâncias
   if ((clone->z0.r == 0.)&&(clone->z0.x == 0.)&&
       (clone->z1.r == 0.)&&(clone->z1.x == 0.))
      {
      Aviso("Defina impedâncias do transformador");
      return(false);
      }
   //valida LTC
	if (clone->ltc.tipo != ltcFIXO)
      {/*
      //não permite que a Barra controlada seja a mesma do tap c/ ajuste automático
      if ((clone->ltc.pri_sec == ltcPRIMARIO)&&(clone->ltc.pbarra == clone->pbarra1))
         {
         Erro("A barra controlada não pode ser a mesma do tap ajustável");
         return(false);
         }
      else if ((clone->ltc.pri_sec == ltcSECUNDARIO)&&(clone->ltc.pbarra == clone->pbarra2))
         {
         Erro("A barra controlada não pode ser a mesma do tap ajustável");
         return(false);
         }
      */
      //valida tensão da Barra controlada
		if ((clone->ltc.ajuste_auto.v_pu < 0.7)||(clone->ltc.ajuste_auto.v_pu > 1.3))
         {
         Aviso("LTC: tensão controlada inválida (0.7 <= V(pu) <= 1.3)");
         return(false);
         }
      }
   //verifica código nulo
   if (clone->Codigo.IsEmpty())
      {
      //if (Confirma("Código não preenchido.", "Deseja definir o código?") != IDNO) return(false);
      //FKM - 2015.10.05 poe o focus onde o usuário precisa editar   \\
      if (Confirma("Código não preenchido.", "Deseja definir o código?") != IDNO)
         {
         pgCtrl->ActivePage = TabSheetTrafo;
         EditCodigo->SetFocus();
         return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrfMono::ValidaTap(void)
   {
   //variáveis locais
   double dv_pu;

   //valida Tap do primário
	if (clone->ltc.tipo != ltcPRIMARIO)
      {
      if (IsDoubleZero(clone->pri.tap))
         {
         Aviso("Defina tensão de tap do primário");
         return(false);
         }
      //determina variação de tap em pu do primário
      dv_pu = fabs(clone->pri.vnom - clone->pri.tap) / clone->pri.vnom;
      if (dv_pu > 0.15)
         {
         if (Confirma("Primário: " +
                      DoubleToStr("Vnom = %4.3f kV, "  , clone->pri.vnom) +
                      DoubleToStr("Vtap = %4.3f kV, "  , clone->pri.tap)  +
                      DoubleToStr("variação = %2.1f \%", 100. * dv_pu),
                      "Estes valores estão corretos ?") != IDYES) return(false);
         }
      }
   //valida Tap do secundário
	if (clone->ltc.tipo != ltcSECUNDARIO)
      {
      if (IsDoubleZero(clone->sec.tap))
         {
         Aviso("Defina tensão de tap do secundário");
         return(false);
         }
      //determina variação de tap em pu do primário
      dv_pu = fabs(clone->sec.vnom - clone->sec.tap) / clone->sec.vnom;
      if (dv_pu > 0.15)
         {
         if (Confirma("Secundário: " +
                      DoubleToStr("Vnom = %4.3f kV, "  , clone->sec.vnom) +
                      DoubleToStr("Vtap = %4.3f kV, "  , clone->sec.tap)  +
                      DoubleToStr("variação = %2.1f \%", 100. * dv_pu),
                      "Estes valores estão corretos ?") != IDYES) return(false);
         }
      }
   return(true);
   }





//---------------------------------------------------------------------------
//eof




