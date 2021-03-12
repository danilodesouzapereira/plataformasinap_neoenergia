//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include <math.h>
#include "TFormEdtTrf3E.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;

//---------------------------------------------------------------------------
__fastcall TFormEdtTrf3E::TFormEdtTrf3E(TComponent* Owner, VTApl *apl, VTTrafo3E *ptrafo3e)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl     = apl;
   this->trafo3e = ptrafo3e;
   //cria lista
   lisTRF = new TList();
   //inicia dados
   sbase_100mva = 100.0;
   /*
   //verifica se deve habilitar edi��o de dados de planejamento
   if ((trafo3e->planeja.ano > 0)&&
       ((trafo3e->planeja.ano >= rede->planeja.ano)||(rede->planeja.ano == MAX_ANO)))
      {//cria Form p/ edi��o de dados do Planejamento
      FormEdtPlan = new TFormEdtPlan(this, tsPlaneja, apl, trafo3e);
      //habilita TableSheet de planejamento
      tsPlaneja->TabVisible = true;
      }
   else
      {//desabilita TableSheet de planejamento
      tsPlaneja->TabVisible = false;
      }
   */
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtTrf3E::~TFormEdtTrf3E(void)
   {//vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destr�i objetos criados
   //if (FormEdtPlan) {delete FormEdtPlan; FormEdtPlan = NULL;}
   //destr�i lista sem destruir seus objetos
   if (lisTRF)  {delete lisTRF; lisTRF = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::ActionConfirmaExecute(TObject *Sender)
   {
   //valida dados do Trafo
   if (! LeDadosTrafo())     return;
   if (! ValidaDadosTrafo()) return;
   /*
   //verifica se � editor de planejamento
   if (FormEdtPlan)
      {//insere Eqpto no planejamento
      if (! FormEdtPlan->ConfirmaInsereEqpto(trafo3e)) return;
      }
   */
   //fecha o form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::ActionRetiraExecute(TObject *Sender)
   {
   /*
   //verifica se � editor de planejamento
   if (FormEdtPlan)
      {//retira Eqpto no planejamento
      FormEdtPlan->CancelaInsereEqpto(trafo);
      }
   */
   //fecha o form indicando exclus�o do Eqpto
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf3E::CalculaZseqTrafo(double sbase_old, double sbase_new)
   {
	//variaveis locais
   double fat_mult;

   //prote��o
   if (IsDoubleZero(sbase_old) || IsDoubleZero(sbase_new)) return(false);
   //define fator de multiplica��o p/ converter z0 e z1 na base selecionada
   fat_mult = sbase_new / sbase_old;
   //recalcula zps0 e zps1
   trafo3e->zps0.r *= fat_mult;
   trafo3e->zps0.x *= fat_mult;
   trafo3e->zps1.r *= fat_mult;
   trafo3e->zps1.x *= fat_mult;
   //recalcula zpt0 e zpt1
   trafo3e->zpt0.r *= fat_mult;
   trafo3e->zpt0.x *= fat_mult;
   trafo3e->zpt1.r *= fat_mult;
   trafo3e->zpt1.x *= fat_mult;
   //recalcula zst0 e zst1
   trafo3e->zst0.r *= fat_mult;
   trafo3e->zst0.x *= fat_mult;
   trafo3e->zst1.r *= fat_mult;
   trafo3e->zst1.x *= fat_mult;

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::cboxPriBarraChange(TObject *Sender)
   {
   //vari�veis locais
   AnsiString tap_pri;

   //determina qual enrolamento deve ser alterado
   if (cboxPriBarra->ItemIndex == cboxSecBarra->ItemIndex)
      {//muda Barra do secund�rio
      cboxSecBarra->ItemIndex = 3 - (cboxPriBarra->ItemIndex + cboxTerBarra->ItemIndex);
      //troca Tap do prim�rio/secund�rio
      tap_pri         = edtPriTap->Text;
      edtPriTap->Text = edtSecTap->Text;
      edtSecTap->Text = tap_pri;
      }
   else if (cboxPriBarra->ItemIndex == cboxTerBarra->ItemIndex)
      {//muda Barra do terci�rio
      cboxTerBarra->ItemIndex = 3 - (cboxPriBarra->ItemIndex + cboxSecBarra->ItemIndex);
      //troca Tap do prim�rio/terci�rio
      tap_pri         = edtPriTap->Text;
      edtPriTap->Text = edtTerTap->Text;
      edtTerTap->Text = tap_pri;
      }
   //atualiza Vnom dos Enrolamentos
   ExibeVnomTrafo();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::cboxSecBarraChange(TObject *Sender)
   {
   //vari�veis locais
   AnsiString tap_sec;

   //determina qual enrolamento deve ser alterado
   if (cboxSecBarra->ItemIndex == cboxPriBarra->ItemIndex)
      {//muda Barra do prim�rio
      cboxPriBarra->ItemIndex = 3 - (cboxSecBarra->ItemIndex + cboxTerBarra->ItemIndex);
      //troca Tap do secund�rio/prim�rio
      tap_sec         = edtSecTap->Text;
      edtSecTap->Text = edtPriTap->Text;
      edtPriTap->Text = tap_sec;
      }
   else if (cboxSecBarra->ItemIndex == cboxTerBarra->ItemIndex)
      {//muda Barra do terci�rio
      cboxTerBarra->ItemIndex = 3 - (cboxPriBarra->ItemIndex + cboxSecBarra->ItemIndex);
      //troca Tap do secund�rio/terci�rio
      tap_sec         = edtSecTap->Text;
      edtSecTap->Text = edtTerTap->Text;
      edtTerTap->Text = tap_sec;
      }
   //atualiza Vnom dos Enrolamentos
   ExibeVnomTrafo();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::cboxTerBarraChange(TObject *Sender)
   {
   //vari�veis locais
   AnsiString tap_ter;

   //determina qual enrolamento deve ser alterado
   if (cboxTerBarra->ItemIndex == cboxPriBarra->ItemIndex)
      {//muda Barra do prim�rio
      cboxPriBarra->ItemIndex = 3 - (cboxSecBarra->ItemIndex + cboxTerBarra->ItemIndex);
      //troca Tap do terci�rio/prim�rio
      tap_ter         = edtTerTap->Text;
      edtTerTap->Text = edtPriTap->Text;
      edtPriTap->Text = tap_ter;
      }
   else if (cboxTerBarra->ItemIndex == cboxSecBarra->ItemIndex)
      {//muda Barra do secund�rio
      cboxSecBarra->ItemIndex = 3 - (cboxPriBarra->ItemIndex + cboxTerBarra->ItemIndex);
      //troca Tap do terci�rio/secund�rio
      tap_ter         = edtTerTap->Text;
      edtTerTap->Text = edtSecTap->Text;
      edtSecTap->Text = tap_ter;
      }
   //atualiza Vnom dos Enrolamentos
   ExibeVnomTrafo();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::cboxPriLigaChange(TObject *Sender)
   {
   //verifica se a liga��o do prim�rio � lgTRIANG
   if (cboxPriLiga->ItemIndex == 1)
      {//prim�rio � lgTRIANG: secund�rio e terci�rio n�o podem ser lgTRIANG
      if ((cboxSecLiga->ItemIndex == 1)&&(cboxTerLiga->ItemIndex == 1))
         {//altera terci�rio p/ lgEST_AT
         cboxTerLiga->ItemIndex = 0;
         }
      }
   //l� ligacao do prim�rio, secund�rio e terci�rio
   trafo3e->pri.ligacao = cboxPriLiga->ItemIndex;
   trafo3e->sec.ligacao = cboxSecLiga->ItemIndex;
   trafo3e->ter.ligacao = cboxTerLiga->ItemIndex;
   //exibe/esconde campos condicionais
   ExibeCamposCondicionais();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::CBoxSbaseChange(TObject *Sender)
   {
   //vari�veis locais
   double sbase_old, sbase_new, snom_mva, snom_pri, snom_sec, snom_ter;

   try{//pergunta ao usu�rio se os valores das imped�ncias devem ser recalculados na nova base
      if (Confirma("O valor de base das imped�ncias foi alterado",
                   "Os valores de Z0 e Z1 devem ser recalculados na nova base selecionada ?") == IDYES)
         {//determina valor de Snom
         snom_pri = StrToDouble(edtPriSnom->Text);
         snom_sec = StrToDouble(edtSecSnom->Text);
         snom_ter = StrToDouble(edtTerSnom->Text);
         snom_mva = max(snom_pri, max(snom_sec, snom_ter));
         //determina valores de base anterior e posterior � altera��o do CBoxSbase
         if (CBoxSbase->ItemIndex == 0)
            {
            sbase_new = snom_mva;
            sbase_old = sbase_100mva;
            }
         else
            {
            sbase_new = sbase_100mva;
            sbase_old = snom_mva;
            }
         //l� valores de Z0 e Z1 em pu
         LeZseqTrafo();
         //calcula valores de Z0 e Z1 na nova base
         CalculaZseqTrafo(sbase_old, sbase_new);
         //exibe os novos valores de Z0 e Z1
         ExibeZseqTrafo();
         }
      }catch(Exception &)
         {
         Aviso("Dados inv�lidos.\nOs valores de Z0 e Z1 n�o puderam ser calculados");
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::cboxSecLigaChange(TObject *Sender)
   {
   //verifica se a liga��o do secund�rio � lgTRIANG
   if (cboxSecLiga->ItemIndex == 1)
      {//secund�rio � lgTRIANG: terci�rio e prim�rio n�o podem ser lgTRIANG
      if ((cboxTerLiga->ItemIndex == 1)&&(cboxPriLiga->ItemIndex == 1))
         {//altera prim�rio p/ lgEST_AT
         cboxPriLiga->ItemIndex = 0;
         }
      }
   //l� ligacao do prim�rio, secund�rio e terci�rio
   trafo3e->pri.ligacao = cboxPriLiga->ItemIndex;
   trafo3e->sec.ligacao = cboxSecLiga->ItemIndex;
   trafo3e->ter.ligacao = cboxTerLiga->ItemIndex;
   //exibe/esconde campos condicionais
   ExibeCamposCondicionais();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::cboxTerLigaChange(TObject *Sender)
   {
   //verifica se a liga��o do terci�rio  � lgTRIANG
   if (cboxTerLiga->ItemIndex == 1)
      {//terci�rio � lgTRIANG: prim�rio e secund�rio n�o podem ser lgTRIANG
      if ((cboxPriLiga->ItemIndex == 1)&&(cboxSecLiga->ItemIndex == 1))
         {//altera prim�rio p/ lgEST_AT
         cboxPriLiga->ItemIndex = 0;
         }
      }
   //l� ligacao do prim�rio, secund�rio e terci�rio
   trafo3e->pri.ligacao = cboxPriLiga->ItemIndex;
   trafo3e->sec.ligacao = cboxSecLiga->ItemIndex;
   trafo3e->ter.ligacao = cboxTerLiga->ItemIndex;
   //exibe/esconde campos condicionais
   ExibeCamposCondicionais();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::DefBarrasPriSecTer(void)
   {
   //vari�veis locais
   VTBarra *pbar_tmp, *pbar[NBAR_LIG];

   //verifica se o Trafo n�o est� definido
   if ((trafo3e->pri.snom > 0.)&&(trafo3e->pri.vnom == trafo3e->pbarra1->vnom)&&
       (trafo3e->sec.snom > 0.)&&(trafo3e->sec.vnom == trafo3e->pbarra2->vnom)&&
       (trafo3e->ter.snom > 0.)&&(trafo3e->ter.vnom == trafo3e->pbarra3->vnom)) return;
   //ordena Barras por ordem decrescente de Vnom
   pbar[0] = trafo3e->pbarra1;
   pbar[1] = trafo3e->pbarra2;
   pbar[2] = trafo3e->pbarra3;
   for (int i = 0; i < NBAR_LIG; i++)
      {
      for (int j = i+1; j < NBAR_LIG; j++)
         {
         if (pbar[j]->vnom > pbar[i]->vnom)
            {//troca bar[j] por bar[i]
            pbar_tmp = pbar[i];
            pbar[i]  = pbar[j];
            pbar[j]  = pbar_tmp;
            }
         }
      }
   //redefine Barras do Trafo3E
   trafo3e->RedefineBarraPriSecTer(pbar);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::DefValoresDefaultTrafo(void)
   {
   //vari�veis locais
   VTTrafo3E *ult_trf;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //verifica se o Trafo n�o est� definido
   if ((trafo3e->pri.snom == 0.)||(trafo3e->pri.vnom == 0.)||
       (trafo3e->sec.snom == 0.)||(trafo3e->sec.vnom == 0.)||
       (trafo3e->ter.snom == 0.)||(trafo3e->ter.vnom == 0.))
      {//define Barras do prim�rio, secund�rio, terci�rio
      DefBarrasPriSecTer();
      //define perda no ferro
      trafo3e->perda_ferro = 0.1;
      //inicia dados prim�rio
      trafo3e->pri.vnom      = trafo3e->pbarra1->vnom;
      trafo3e->pri.tap       = trafo3e->pri.vnom;
      trafo3e->pri.snom      = 60.;
      trafo3e->pri.ligacao   = lgTRIANG;
      trafo3e->pri.defasagem = 0.;
      trafo3e->pri.zterra.r  = 0.;
      trafo3e->pri.zterra.x  = 0.;
      //inicia dados secund�rio
      trafo3e->sec.vnom      = trafo3e->pbarra2->vnom;
      trafo3e->sec.tap       = trafo3e->sec.vnom;
      trafo3e->sec.snom      = 30.;
      trafo3e->sec.ligacao   = lgEST_AT;
      trafo3e->sec.defasagem = -30.;
      trafo3e->sec.zterra.r  = 0.;
      trafo3e->sec.zterra.x  = 0.;
      //inicia dados terci�rio
      trafo3e->ter.vnom      = trafo3e->pbarra3->vnom;
      trafo3e->ter.tap       = trafo3e->ter.vnom;
      trafo3e->ter.snom      = 30.;
      trafo3e->ter.ligacao   = lgEST_AT;
      trafo3e->ter.defasagem = -30.;
      trafo3e->ter.zterra.r  = 0.;
      trafo3e->ter.zterra.x  = 0.;
      //inicia imped�ncia
      trafo3e->zps0.r = trafo3e->zps1.r = 0;
      trafo3e->zpt0.r = trafo3e->zpt1.r = 0;
      trafo3e->zst0.r = trafo3e->zst1.r = 0;
      trafo3e->zps0.x = trafo3e->zps1.x = 0.05;
      trafo3e->zpt0.x = trafo3e->zpt1.x = 0.05;
      trafo3e->zst0.x = trafo3e->zst1.x = 0.05;
      //seleciona Trafo3E de refer�ncia p/ copiar alguns dados
      lisTRF->Clear();
      redes->LisLigacao(lisTRF, eqptoTRAFO3E);
      if (lisTRF->Count == 0) return;
      if ((ult_trf = (VTTrafo3E*)lisTRF->Last()) == NULL) return;
      //inicia dados prim�rio
      trafo3e->pri.snom      = ult_trf->pri.snom;
      trafo3e->pri.ligacao   = ult_trf->pri.ligacao;
      trafo3e->pri.defasagem = ult_trf->pri.defasagem;
      trafo3e->pri.zterra    = ult_trf->pri.zterra;
      //inicia dados secund�rio
      trafo3e->sec.snom      = ult_trf->sec.snom;
      trafo3e->sec.ligacao   = ult_trf->sec.ligacao;
      trafo3e->sec.defasagem = ult_trf->sec.defasagem;
      trafo3e->sec.zterra    = ult_trf->sec.zterra;
      //inicia dados terci�rio
      trafo3e->ter.snom      = ult_trf->ter.snom;
      trafo3e->ter.ligacao   = ult_trf->ter.ligacao;
      trafo3e->ter.defasagem = ult_trf->ter.defasagem;
      trafo3e->ter.zterra    = ult_trf->ter.zterra;
      //inicia imped�ncia
      trafo3e->zps0 = ult_trf->zps0;
      trafo3e->zps1 = ult_trf->zps1;
      trafo3e->zpt0 = ult_trf->zpt0;
      trafo3e->zpt1 = ult_trf->zpt1;
      trafo3e->zst0 = ult_trf->zst0;
      trafo3e->zst1 = ult_trf->zst1;
      }
   }

//---------------------------------------------------------------------------
/*
//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::ExibeCamposCondicionais(void)
   {
   //vari�veis locais
   double defasagem;
   bool   triang, estrela;

   //exibe/esconde campos condicionais em rela��o ao tipo de liga��o
   gboxPriZterra->Visible = (cboxPriLiga->ItemIndex == lgEST_AT);
   gboxSecZterra->Visible = (cboxSecLiga->ItemIndex == lgEST_AT);
   gboxTerZterra->Visible = (cboxTerLiga->ItemIndex == lgEST_AT);
   //exibe/esconde defasagem prim�rio/secund�rio
   triang  = (cboxPriLiga->ItemIndex == lgTRIANG)||(cboxSecLiga->ItemIndex == lgTRIANG);
   estrela = (cboxPriLiga->ItemIndex != lgTRIANG)||(cboxSecLiga->ItemIndex != lgTRIANG);
   GBoxSecDefasagem->Visible = (triang && estrela);
   //exibe/esconde defasagem prim�rio/terci�rio
   triang  = (cboxPriLiga->ItemIndex == lgTRIANG)||(cboxTerLiga->ItemIndex == lgTRIANG);
   estrela = (cboxPriLiga->ItemIndex != lgTRIANG)||(cboxTerLiga->ItemIndex != lgTRIANG);
   GBoxTerDefasagem->Visible = (triang && estrela);
   try{//atualiza sinal das defasagens
      if (GBoxSecDefasagem->Visible)
         {//atualiza sinal da defasagem
         defasagem = fabs(StrToDouble(edtSecDefasagem->Text));
         if (cboxSecLiga->ItemIndex == lgTRIANG) defasagem = -defasagem;
         edtSecDefasagem->Text  = DoubleToStr("%2.1f", defasagem);
         }
      if (GBoxTerDefasagem->Visible)
         {//atualiza sinal da defasagem
         defasagem = fabs(StrToDouble(edtTerDefasagem->Text));
         if (cboxTerLiga->ItemIndex == lgTRIANG) defasagem = -defasagem;
         edtTerDefasagem->Text  = DoubleToStr("%2.1f", defasagem);
         }
      }catch(Exception &e)
         {//nada a fazer
         }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::ExibeCamposCondicionais(void)
   {
   //vari�veis locais
   bool   triang, estrela;

   //exibe/esconde campos condicionais em rela��o ao tipo de liga��o
   gboxPriZterra->Visible = (cboxPriLiga->ItemIndex == lgEST_AT);
   gboxSecZterra->Visible = (cboxSecLiga->ItemIndex == lgEST_AT);
   gboxTerZterra->Visible = (cboxTerLiga->ItemIndex == lgEST_AT);
   //exibe defasagem prim�rio/secund�rio
   ExibeSecDefasagem();
   //exibe defasagem prim�rio/terci�rio
   ExibeTerDefasagem();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::ExibeDadosTrafo(void)
   {
   //exibe dados gerais do Trafo3e
   EditCodigo->Text = trafo3e->Codigo;
   edtPerdaFe->Text       = DoubleToStr("%4.3f", trafo3e->perda_ferro);
   //exibe dados de Zseq
   ExibeZseqTrafo();
   //exibe Vnom dos Enrolamentos
   ExibeVnomTrafo();
   //exibe Tap
   edtPriTap->Text = DoubleToStr("%4.3f", trafo3e->pri.tap);
   edtSecTap->Text = DoubleToStr("%4.3f", trafo3e->sec.tap);
   edtTerTap->Text = DoubleToStr("%4.3f", trafo3e->ter.tap);
   //exibe Snom
   edtPriSnom->Text = DoubleToStr("%4.3f", trafo3e->pri.snom);
   edtSecSnom->Text = DoubleToStr("%4.3f", trafo3e->sec.snom);
   edtTerSnom->Text = DoubleToStr("%4.3f", trafo3e->ter.snom);
   //exibe Liga��o
   cboxPriLiga->ItemIndex = trafo3e->pri.ligacao;
   cboxSecLiga->ItemIndex = trafo3e->sec.ligacao;
   cboxTerLiga->ItemIndex = trafo3e->ter.ligacao;
   //exibe Zterra
   edtPriRterra->Text    = DoubleToStr("%4.3f", trafo3e->pri.zterra.r);
   edtPriXterra->Text    = DoubleToStr("%4.3f", trafo3e->pri.zterra.x);
   edtSecRterra->Text    = DoubleToStr("%4.3f", trafo3e->sec.zterra.r);
   edtSecXterra->Text    = DoubleToStr("%4.3f", trafo3e->sec.zterra.x);
   edtTerRterra->Text    = DoubleToStr("%4.3f", trafo3e->ter.zterra.r);
   edtTerXterra->Text    = DoubleToStr("%4.3f", trafo3e->ter.zterra.x);
   //exibe Defasagems
   //edtSecDefasagem->Text = DoubleToStr("%2.1f", trafo3e->sec.defasagem);
   //edtTerDefasagem->Text = DoubleToStr("%2.1f", trafo3e->ter.defasagem);
   ExibeSecDefasagem();
   ExibeTerDefasagem();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::ExibeSecDefasagem(void)
   {
   //reinicia CBoxSecDefasagem
   CBoxSecDefasagem->Clear();
   //reinicia op��es do CBoxSecDefasagem
   if ((trafo3e->pri.ligacao == lgTRIANG)&&(trafo3e->sec.ligacao != lgTRIANG))
      {
      CBoxSecDefasagem->Items->Add("Dy1");
      CBoxSecDefasagem->Items->Add("Dy11");
      CBoxSecDefasagem->ItemIndex = (trafo3e->sec.defasagem <= 0) ? 0 : 1;
      CBoxSecDefasagem->Enabled   = true;
      GBoxSecDefasagem->Visible   = true;
      }
   else if ((trafo3e->pri.ligacao != lgTRIANG)&&(trafo3e->sec.ligacao == lgTRIANG))
      {
      CBoxSecDefasagem->Items->Add("Yd1");
      CBoxSecDefasagem->Items->Add("Yd11");
      CBoxSecDefasagem->ItemIndex = (trafo3e->sec.defasagem >= 0) ? 1 : 0;
      CBoxSecDefasagem->Enabled   = true;
      GBoxSecDefasagem->Visible   = true;
      }
   else
      {
      CBoxSecDefasagem->Items->Add("n�o se aplica");
      CBoxSecDefasagem->ItemIndex = 0;
      CBoxSecDefasagem->Enabled   = false;
      GBoxSecDefasagem->Visible   = false;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::ExibeTerDefasagem(void)
   {
   //reinicia CBoxTerDefasagem
   CBoxTerDefasagem->Clear();
   //reinicia op��es do CBoxTerDefasagem
   if ((trafo3e->pri.ligacao == lgTRIANG)&&(trafo3e->ter.ligacao != lgTRIANG))
      {
      CBoxTerDefasagem->Items->Add("Dy1");
      CBoxTerDefasagem->Items->Add("Dy11");
      CBoxTerDefasagem->ItemIndex = (trafo3e->ter.defasagem <= 0) ? 0 : 1;
      CBoxTerDefasagem->Enabled   = true;
      GBoxTerDefasagem->Visible   = true;
      }
   else if ((trafo3e->pri.ligacao != lgTRIANG)&&(trafo3e->ter.ligacao == lgTRIANG))
      {
      CBoxTerDefasagem->Items->Add("Yd1");
      CBoxTerDefasagem->Items->Add("Yd11");
      CBoxTerDefasagem->ItemIndex = (trafo3e->ter.defasagem >= 0) ? 1 : 0;
      CBoxTerDefasagem->Enabled   = true;
      GBoxTerDefasagem->Visible   = true;
      }
   else
      {
      CBoxTerDefasagem->Items->Add("n�o se aplica");
      CBoxTerDefasagem->ItemIndex = 0;
      CBoxTerDefasagem->Enabled   = false;
      GBoxTerDefasagem->Visible   = false;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::ExibeVnomTrafo(void)
   {
   //vari�veis locais
   VTBarra *pbarra;

   //exibe Vnom do Prim�rio
   pbarra = (VTBarra*)(cboxPriBarra->Items->Objects[cboxPriBarra->ItemIndex]);
   edtPriVnom->Text = DoubleToStr("%4.3f", pbarra->vnom);
   //exibe Vnom do Secund�rio
   pbarra = (VTBarra*)(cboxSecBarra->Items->Objects[cboxSecBarra->ItemIndex]);
   edtSecVnom->Text = DoubleToStr("%4.3f", pbarra->vnom);
   //exibe Vnom do Terci�rio
   pbarra = (VTBarra*)(cboxTerBarra->Items->Objects[cboxTerBarra->ItemIndex]);
   edtTerVnom->Text = DoubleToStr("%4.3f", pbarra->vnom);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::ExibeZseqTrafo(void)
   {
   //vari�veis locais
   AnsiString tit_col[5] = {"", "r0",  "x0",  "r1",  "x1"};
   AnsiString tit_lin[4] = {"", "Zps", "Zpt", "Zst"};

   //preenche t�tulos do SGrid
   for (int n = 0; n < 5; n++) sgrid->Cells[n][0] = tit_col[n];
   for (int n = 0; n < 4; n++) sgrid->Cells[0][n] = tit_lin[n];
   //exibe imped�ncia prim�rio/secund�rio em pu
   sgrid->Cells[1][1] = DoubleToStr("%6.5f", trafo3e->zps0.r); //RPS0
   sgrid->Cells[2][1] = DoubleToStr("%6.5f", trafo3e->zps0.x); //XPS0
   sgrid->Cells[3][1] = DoubleToStr("%6.5f", trafo3e->zps1.r); //RPS1
   sgrid->Cells[4][1] = DoubleToStr("%6.5f", trafo3e->zps1.x); //XPS1
   //exibe imped�ncia prim�rio/terci�rio em pu
   sgrid->Cells[1][2] = DoubleToStr("%6.5f", trafo3e->zpt0.r); //RPT0
   sgrid->Cells[2][2] = DoubleToStr("%6.5f", trafo3e->zpt0.x); //XPT0
   sgrid->Cells[3][2] = DoubleToStr("%6.5f", trafo3e->zpt1.r); //RPT1
   sgrid->Cells[4][2] = DoubleToStr("%6.5f", trafo3e->zpt1.x); //XPT1
   //exibe imped�ncia secund�rio/terci�rio em pu
   sgrid->Cells[1][3] = DoubleToStr("%6.5f", trafo3e->zst0.r); //RST0
   sgrid->Cells[2][3] = DoubleToStr("%6.5f", trafo3e->zst0.x); //XST0
   sgrid->Cells[3][3] = DoubleToStr("%6.5f", trafo3e->zst1.r); //RST1
   sgrid->Cells[4][3] = DoubleToStr("%6.5f", trafo3e->zst1.x); //XST1
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      geral->Infoset->LoadFromFile(this);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::FormShow(TObject *Sender)
   {
   //se necess�rio, define valores default p/ Trecho
   DefValoresDefaultTrafo();
   //seleciona TableSheet
   pgCtrl->ActivePage = tsTrafo;
   //inicia CBoxBarra c/ as barras do Trafo
   IniciaCBoxBarra();
   //inicia CBox c/ as Redes
   IniciaCBoxRede();
   //inicia Captions,Labels etc da tela
   IniciaParteFixaTela();
   //exibe dados do Trafo3E
   ExibeDadosTrafo();
   //exibe/esconde dados opcionais
   ExibeCamposCondicionais();
   //posiciona o Cursor em EditCodigo
   EditCodigo->SetFocus();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::IniciaCBoxBarra(void)
   {
   //vari�veis locais
   VTBarra     *pb, *pbar[3];
   AnsiString  nomeBarra;

   //monta vetor c/ Barras ordenadas: prim�rio, secund�rio, terci�rio
   pbar[0] = trafo3e->pbarra1;
   pbar[1] = trafo3e->pbarra2;
   pbar[2] = trafo3e->pbarra3;
   //reinicia CBoxVnomPri, CBoxVnomSec e CBoxVnomTer
   cboxPriBarra->Clear();
   cboxSecBarra->Clear();
   cboxTerBarra->Clear();
   for (int n = 0; n < 3; n++)
      {  //FKM 2015.09.03   /* N�o exibe Codigo vazio
         if(pbar[n]->Codigo.IsEmpty()){nomeBarra = "(Id = " + IntToStr(pbar[n]->Id) +")";}
         else{ nomeBarra = pbar[n]->Codigo;}
         cboxPriBarra->Items->AddObject(nomeBarra, pbar[n]);
         cboxSecBarra->Items->AddObject(nomeBarra, pbar[n]);
         cboxTerBarra->Items->AddObject(nomeBarra, pbar[n]);
        //FKM 2015.09.03   ************************** */
//      cboxPriBarra->Items->AddObject(pbar[n]->Codigo, pbar[n]);
//      cboxSecBarra->Items->AddObject(pbar[n]->Codigo, pbar[n]);
//      cboxTerBarra->Items->AddObject(pbar[n]->Codigo, pbar[n]);
      }
   //seleciona Barra/Vnom do prim�rio, secund�rio e terci�rio
   cboxPriBarra->ItemIndex = 0;
   cboxSecBarra->ItemIndex = 1;
   cboxTerBarra->ItemIndex = 2;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::IniciaCBoxRede(void)
   {
   //vari�veis locais
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
   //define ordem alfab�tica
   cboxRede->Sorted = true;
   //pr�-seleciona Rede que conecta Barras do Trafo3E
   index = 0;
   if ((rede = SelecionaRede(lisREDE)) != NULL) index = cboxRede->Items->IndexOfObject(rede);
   cboxRede->ItemIndex = index;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::IniciaParteFixaTela(void)
   {
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf3E::LeDadosTrafo(void)
   {
   //vari�veis locais
   VTBarra *pbar[3];

   //prote��o: verifica se a Rede est� definida
   if (cboxRede->ItemIndex < 0)
      {
      Aviso("Defina a Rede do Transformador");
      return(false);
      }
   try{//determina Rede do Trafo
      trafo3e->rede        = (VTRede*)cboxRede->Items->Objects[cboxRede->ItemIndex];
      //dados gerais
      trafo3e->Codigo      = EditCodigo->Text;
      trafo3e->perda_ferro = StrToDouble(edtPerdaFe->Text); //%
      //define valores default p/ dados cuja edi��o pode estar bloqueada
      trafo3e->pri.zterra.r  = 0.;
      trafo3e->pri.zterra.x  = 0.;
      trafo3e->sec.zterra.r  = 0.;
      trafo3e->sec.zterra.x  = 0.;
      trafo3e->sec.defasagem = 0.;
      trafo3e->ter.zterra.r  = 0.;
      trafo3e->ter.zterra.x  = 0.;
      trafo3e->ter.defasagem = 0.;
      //dados do prim�rio
      trafo3e->pri.vnom    = StrToDouble(edtPriVnom->Text);
      trafo3e->pri.tap     = StrToDouble(edtPriTap->Text);
      trafo3e->pri.snom    = StrToDouble(edtPriSnom->Text);
      //l� tipo da Ligacao do prim�rio
      trafo3e->pri.ligacao = cboxPriLiga->ItemIndex;
      //l� defasagem prim�rio
      if (! LePriDefasagem()) return(false);
      //l� Zterra do prim�rio
      if (gboxPriZterra->Visible)
         {//l� Zterra do prim�rio somente se a edi��o estiver habilitada
         trafo3e->pri.zterra.r = StrToDouble(edtPriRterra->Text);    //ohm
         trafo3e->pri.zterra.x = StrToDouble(edtPriXterra->Text);    //ohm
         }
      //dados do secund�rio
      trafo3e->sec.vnom    = StrToDouble(edtSecVnom->Text);
      trafo3e->sec.tap     = StrToDouble(edtSecTap->Text);
      trafo3e->sec.snom    = StrToDouble(edtSecSnom->Text);
      //l� tipo da Ligacao do secund�rio
      trafo3e->sec.ligacao = cboxSecLiga->ItemIndex;
      //l� defasagem prim�rio/secund�rio
      if (! LeSecDefasagem()) return(false);
      //l� Zterra do secund�rio
      if (gboxSecZterra->Visible)
         {
         trafo3e->sec.zterra.r = StrToDouble(edtSecRterra->Text);    //ohm
         trafo3e->sec.zterra.x = StrToDouble(edtSecXterra->Text);    //ohm
         }
      //dados do terci�rio
      trafo3e->ter.vnom    = StrToDouble(edtTerVnom->Text);
      trafo3e->ter.tap     = StrToDouble(edtTerTap->Text);
      trafo3e->ter.snom    = StrToDouble(edtTerSnom->Text);
      //l� tipo da Ligacao do terci�rio
      trafo3e->ter.ligacao = cboxTerLiga->ItemIndex;
      //l� defasagem prim�rio/terci�rio
      if (! LeTerDefasagem()) return(false);
      //l� Zterra do terci�rio
      if (gboxTerZterra->Visible)
         {
         trafo3e->ter.zterra.r = StrToDouble(edtTerRterra->Text);    //ohm
         trafo3e->ter.zterra.x = StrToDouble(edtTerXterra->Text);    //ohm
         }
      //l� Zseq  em pu (ap�s ler pot�ncias nominais do prim�rio, secund�rio e terci�rio)
      if (! LeZseqTrafo()) return(false);
      //se necess�rio converte valores de Z0 e Z1 p/ Sbase igual a Snom
      if (CBoxSbase->ItemIndex == 1) CalculaZseqTrafo(sbase_100mva, trafo3e->Sbase);
      }catch (Exception &e)
         {
         Erro("Dados Inv�lidos no Trafo3E");
         return false;
         }
   //valida valores
   if ((trafo3e->pri.vnom == 0)||(trafo3e->sec.vnom == 0)||(trafo3e->ter.vnom == 0))
      {
      Erro("Defina Vnom do prim�rio, secund�rio e terci�rio");
      return(false);
      }
   if ((trafo3e->pri.snom == 0)||(trafo3e->sec.snom == 0)||(trafo3e->ter.snom == 0))
      {
      Erro("Defina Snom do prim�rio, secund�rio e terci�rio");
      return(false);
      }
   //redefine Barras do trafo e suas Vnom
   pbar[0] = (VTBarra*)cboxPriBarra->Items->Objects[cboxPriBarra->ItemIndex];
   pbar[1] = (VTBarra*)cboxSecBarra->Items->Objects[cboxSecBarra->ItemIndex];
   pbar[2] = (VTBarra*)cboxTerBarra->Items->Objects[cboxTerBarra->ItemIndex];
   trafo3e->RedefineBarraPriSecTer(pbar);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf3E::LePriDefasagem(void)
   {
   //defasagem do prim�rio sempre � nula
   trafo3e->pri.defasagem = 0;
   return true; //FKM - estava sem return...
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf3E::LeSecDefasagem(void)
   {
   //vari�veis locais
   int defasagem;

   //verifica se op��o de liga��o est� selecionada
   if (CBoxSecDefasagem->ItemIndex < 0)
      {
      Aviso("Selecione a defasagem entre prim�rio/secund�rio");
      return(false);
      }
   //define valor do �ngulo em fun��o da liga��o selecionada
   if ((trafo3e->pri.ligacao == lgTRIANG)&&(trafo3e->sec.ligacao != lgTRIANG))
      {
      if (CBoxSecDefasagem->ItemIndex == 0) defasagem = -30;  //Dy1
      else                                  defasagem =  30;  //Dy11
      }
   else if ((trafo3e->pri.ligacao != lgTRIANG)&&(trafo3e->sec.ligacao == lgTRIANG))
      {
      if (CBoxSecDefasagem->ItemIndex == 0) defasagem = -30;   //Yd1
      else                                  defasagem =  30;   //Yd11
      }
   else
      {
      defasagem = 0;
      }
   //atualiza �ngulo de defasagem
   trafo3e->sec.defasagem = defasagem;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf3E::LeTerDefasagem(void)
   {
   //vari�veis locais
   int defasagem;

   //verifica se op��o de liga��o est� selecionada
   if (CBoxTerDefasagem->ItemIndex < 0)
      {
      Aviso("Selecione a defasagem entre prim�rio/terci�rio");
      return(false);
      }
   //define valor do �ngulo em fun��o da liga��o selecionada
   if ((trafo3e->pri.ligacao == lgTRIANG)&&(trafo3e->ter.ligacao != lgTRIANG))
      {
      if (CBoxTerDefasagem->ItemIndex == 0) defasagem = -30;  //Dy1
      else                                  defasagem =  30;  //Dy11
      }
   else if ((trafo3e->pri.ligacao != lgTRIANG)&&(trafo3e->ter.ligacao == lgTRIANG))
      {
      if (CBoxTerDefasagem->ItemIndex == 0) defasagem = -30;   //Yd1
      else                                  defasagem =  30;   //Yd11
      }
   else
      {
      defasagem = 0;
      }
   //atualiza �ngulo de defasagem
   trafo3e->ter.defasagem = defasagem;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf3E::LeZseqTrafo(void)
   {
   try{
      //fkm 2015.09.02 inseri a verifica��o de valor nulo
      //if(StrToDouble(sgrid->Cells[1][1]) == 0){throw Exception("Valor RPS0 nulo");}
      //if(StrToDouble(sgrid->Cells[2][1]) == 0){throw Exception("Valor XPS0 nulo");}
      //if(StrToDouble(sgrid->Cells[3][1]) == 0){throw Exception("Valor RPS1 nulo");}
      //if(StrToDouble(sgrid->Cells[4][1]) == 0){throw Exception("Valor XPS1 nulo");}
      if((StrToDouble(sgrid->Cells[1][1]) == 0) && (StrToDouble(sgrid->Cells[2][1]) == 0))
         {
         throw Exception("Valor Z.PS0 nulo");
         }
      if((StrToDouble(sgrid->Cells[3][1]) == 0) && (StrToDouble(sgrid->Cells[4][1]) == 0))
         {
         throw Exception("Valor Z.PS1 nulo");
         }
      trafo3e->zps0.r = StrToDouble(sgrid->Cells[1][1]); //RPS0
      trafo3e->zps0.x = StrToDouble(sgrid->Cells[2][1]); //XPS0
      trafo3e->zps1.r = StrToDouble(sgrid->Cells[3][1]); //RPS1
      trafo3e->zps1.x = StrToDouble(sgrid->Cells[4][1]); //XPS1
      //imped�ncia prim�rio/terci�rio em pu
//      if(StrToDouble(sgrid->Cells[1][2]) == 0){throw Exception("Valor RPT0 nulo");}
//      if(StrToDouble(sgrid->Cells[2][2]) == 0){throw Exception("Valor XPT0 nulo");}
//      if(StrToDouble(sgrid->Cells[3][2]) == 0){throw Exception("Valor RPT1 nulo");}
//      if(StrToDouble(sgrid->Cells[4][2]) == 0){throw Exception("Valor XPT1 nulo");}
      if((StrToDouble(sgrid->Cells[1][2]) == 0) && (StrToDouble(sgrid->Cells[2][2]) == 0))
         {
         throw Exception("Valor Z.PT0 nulo");
         }
      if((StrToDouble(sgrid->Cells[3][2]) == 0) && (StrToDouble(sgrid->Cells[4][2]) == 0))
         {
         throw Exception("Valor Z.PT1 nulo");
         }
      trafo3e->zpt0.r = StrToDouble(sgrid->Cells[1][2]); //RPT0
      trafo3e->zpt0.x = StrToDouble(sgrid->Cells[2][2]); //XPT0
      trafo3e->zpt1.r = StrToDouble(sgrid->Cells[3][2]); //RPT1
      trafo3e->zpt1.x = StrToDouble(sgrid->Cells[4][2]); //XPT1
      //imped�ncia secund�rio/terci�rio em pu
//      if(StrToDouble(sgrid->Cells[1][3]) == 0){throw Exception("Valor RST0 nulo");}
//      if(StrToDouble(sgrid->Cells[2][3]) == 0){throw Exception("Valor XST0 nulo");}
//      if(StrToDouble(sgrid->Cells[3][3]) == 0){throw Exception("Valor RST1 nulo");}
//      if(StrToDouble(sgrid->Cells[4][3]) == 0){throw Exception("Valor XST1 nulo");}
      if((StrToDouble(sgrid->Cells[1][3]) == 0) && (StrToDouble(sgrid->Cells[2][3]) == 0))
         {
         throw Exception("Valor Z.ST0 nulo");
         }
      if((StrToDouble(sgrid->Cells[3][3]) == 0) && (StrToDouble(sgrid->Cells[4][3]) == 0))
         {
         throw Exception("Valor Z.ST1 nulo");
         }
      trafo3e->zst0.r = StrToDouble(sgrid->Cells[1][3]); //RST0
      trafo3e->zst0.x = StrToDouble(sgrid->Cells[2][3]); //XST0
      trafo3e->zst1.r = StrToDouble(sgrid->Cells[3][3]); //RST1
      trafo3e->zst1.x = StrToDouble(sgrid->Cells[4][3]); //XST1
      }catch(Exception &e)
         {
         Aviso("Dados inv�lidos de imped�ncias de sequ�ncia");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf3E::OpcaoRetira(bool enabled)
   {
   butRetira->Visible = enabled;
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TFormEdtTrf3E::SelecionaRede(TList *lisREDE)
   {
   //vari�veis locais
   VTRede  *rede;

   //verifica se o Trafo3E j� possui uma Rede definida
   if (trafo3e->rede) return(trafo3e->rede);
   //procura uma Rede que conecta as Barras do Trafo3E
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if ((rede->ExisteBarra(trafo3e->pbarra1))||
          (rede->ExisteBarra(trafo3e->pbarra2))||
          (rede->ExisteBarra(trafo3e->pbarra3))) return(rede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------//---------------------------------------------------------------------------//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf3E::ValidaDadosTrafo(void)
   {
   //valida Snom
   if ((trafo3e->pri.snom == 0.)||(trafo3e->sec.snom == 0.)||(trafo3e->ter.snom == 0.))
      {
      Aviso("Defina pot�ncias nominais do transformador");
      return(false);
      }
   //valida Vnom
   if ((trafo3e->pri.vnom == 0.)||(trafo3e->sec.vnom == 0.)||(trafo3e->ter.vnom == 0.))
      {
      Aviso("Defina tens�es nominais do transformador");
      return(false);
      }
   //valida Tap
   if (! ValidaTap()) return(false);
   //valida perda no ferro
	if (trafo3e->perda_ferro < 0.)
      {
      Aviso("Perda no ferro com valor negativo");
      return(false);
      }
   //valida imped�ncias
   if ((trafo3e->zps0.r == 0.)&&(trafo3e->zps0.x == 0.)&&
       (trafo3e->zps1.r == 0.)&&(trafo3e->zps1.x == 0.))
      {
      Aviso("Defina imped�ncias prim�rio/secund�rio do transformador");
      return(false);
      }
   if ((trafo3e->zpt0.r == 0.)&&(trafo3e->zpt0.x == 0.)&&
       (trafo3e->zpt1.r == 0.)&&(trafo3e->zpt1.x == 0.))
      {
      Aviso("Defina imped�ncias prim�rio/terci�rio do transformador");
      return(false);
      }
   if ((trafo3e->zst0.r == 0.)&&(trafo3e->zst0.x == 0.)&&
       (trafo3e->zst1.r == 0.)&&(trafo3e->zst1.x == 0.))
      {
      Aviso("Defina imped�ncias secund�rio/terci�rio do transformador");
      return(false);
      }
   //valida diferen�a entre vnom e tap
   if (! ValidaTap()) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf3E::ValidaTap(void)
   {
   //vari�veis locais
   double dv_pu;

   //valida Tap do prim�rio
   if (IsDoubleZero(trafo3e->pri.tap))
      {
      Aviso("Defina tens�o de tap do prim�rio");
      return(false);
      }
   //determina varia��o de tap em pu
   dv_pu = fabs(trafo3e->pri.vnom - trafo3e->pri.tap) / trafo3e->pri.vnom;
   if (dv_pu > 0.15)
      {
      if (Confirma("Prim�rio: " +
                   DoubleToStr("Vnom = %4.3f kV, "  , trafo3e->pri.vnom) +
                   DoubleToStr("Vtap = %4.3f kV, "  , trafo3e->pri.tap)  +
                   DoubleToStr("varia��o = %2.1f \%", 100. * dv_pu),
                   "Estes valores est�o corretos ?") != IDYES) return(false);
      }
   //valida Tap do secund�rio
   if (IsDoubleZero(trafo3e->sec.tap))
      {
      Aviso("Defina tens�o de tap do secund�rio");
      return(false);
      }
   //determina varia��o de tap em pu
   dv_pu = fabs(trafo3e->sec.vnom - trafo3e->sec.tap) / trafo3e->sec.vnom;
   if (dv_pu > 0.15)
      {
      if (Confirma("Secund�rio: " +
                   DoubleToStr("Vnom = %4.3f kV, "  , trafo3e->sec.vnom) +
                   DoubleToStr("Vtap = %4.3f kV, "  , trafo3e->sec.tap)  +
                   DoubleToStr("varia��o = %2.1f \%", 100. * dv_pu),
                   "Estes valores est�o corretos ?") != IDYES) return(false);
      }
   //valida Tap do tercd�rio
   if (IsDoubleZero(trafo3e->ter.tap))
      {
      Aviso("Defina tens�o de tap do terc�rio");
      return(false);
      }
   //determina varia��o de tap em pu
   dv_pu = fabs(trafo3e->ter.vnom - trafo3e->ter.tap) / trafo3e->ter.vnom;
   if (dv_pu > 0.15)
      {
      if (Confirma("Secund�rio: " +
                   DoubleToStr("Vnom = %4.3f kV, "  , trafo3e->ter.vnom) +
                   DoubleToStr("Vtap = %4.3f kV, "  , trafo3e->ter.tap)  +
                   DoubleToStr("varia��o = %2.1f \%", 100. * dv_pu),
                   "Estes valores est�o corretos ?") != IDYES) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

