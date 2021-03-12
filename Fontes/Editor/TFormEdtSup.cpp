//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include <complex>
#include "TformEdtSup.h"
#include "TFormEditaCurva.h"
#include "TFormExibeCurva.h"
#include "TImportaCurva.h"
//#include "TFormEdtCurvaSup.h"
//#include "TFormEdtPlan.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Diretorio\VTPath.h" //DVK 2014.08.01
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTSuprimento.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;

//---------------------------------------------------------------------------
__fastcall TFormEdtSup::TFormEdtSup(TComponent* Owner, VTApl *apl, VTSuprimento *psup, bool op_retira)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl  = apl;
   this->psup = psup;
   //configura op��o de retirar
   ActionRetira->Visible = op_retira;
   //cria objetos
   lisTMP = new TList();
   //assume defini��oda pot�ncia de curto fase/terra
   RadioGroupPcc_ft->ItemIndex = 0;
   //inicia dados default p/ Eqpto novo
   IniciaDadosSuprimentoNovo();
	//cria TFormEditaCurva p/ edi��o da Curva do Suprimento
	FormEditaCurva = new TFormEditaCurva(this, apl, TabSheetCurva);
	FormEditaCurva->InsereColuna(indP, "P(MW)");
	FormEditaCurva->InsereColuna(indQ, "Q(MVAr)");
	FormEditaCurva->InsereColuna(indV, "V(pu)");
	FormEditaCurva->InsereColuna(indT, "Fase(graus)");
	FormEditaCurva->DefineCurva(psup->curva);
	FormEditaCurva->Show();
	//cria TFormExibeCurva p/ exibi��o da Curva
   FormExibeCurva = new TFormExibeCurva(this, apl, TabSheetGrafico);
   FormExibeCurva->Show();
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtSup::~TFormEdtSup(void)
   {//vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destr�i objetos criados
   //if (FormEdtCurva) {delete FormEdtCurva; FormEdtCurva = NULL;}
   //if (FormEdtPlan) {delete FormEdtPlan; FormEdtPlan = NULL;}
   //destr�i lista sem destruir seus objetos
   if (lisTMP) {delete lisTMP; lisTMP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSup::ActionCancelaExecute(TObject *Sender)
	{
	//valida os dados
	ModalResult = mrCancel;
	}

//---------------------------------------------------------------------------
void __fastcall TFormEdtSup::ActionConfirmaExecute(TObject *Sender)
	{
	//l� dados
	if (! LeDadosSup())   return;
	if (! LeDadosCurva()) return;
	//l� e valida dados de PotCC ou Zeqv
	if (rgImpedancia->ItemIndex == 0)
		{//l� e valida dados de Zeqv
		if (! LeDadosZeqv_CalculaPotCC(true)) return;
		if (! ValidaZeqv(true))   return;
		if (! ValidaZeqv(false))
			{
			Aviso("Imped�ncias equivalentes com valores incorretos");
			return;
			}
		}
	else
		{//l� e valida dados de PotCC
		if (! LeDadosPotCC_CalculaZeqv(true)) return;;
		if (! ValidaPotCC(true)) return;
		if (! ValidaPotCC(false))
			{
			Aviso("Pot�ncias de curto com valores incorretos");
			return;
			}
		}
	ModalResult = mrOk;
	}

//---------------------------------------------------------------------------
void __fastcall TFormEdtSup::ActionImportaExecute(TObject *Sender)
   {//DVK 2014.08.01
   //variaveis locais
   AnsiString     filename;
   VTPath         *path = (VTPath*)apl->GetObject(__classid(VTPath));
   TImportaCurva  *importa = new TImportaCurva(apl, psup->curva);

   //inicia opendialog
   OpenDialog->InitialDir = path->DirImporta();
   OpenDialog->Filter = "Arquivos texto (*.txt)|*.txt|Todos arquivos (*.*)|*.*";
   if (OpenDialog->Execute())
      {
      if (OpenDialog->Files->Count != 1) return;
      filename = OpenDialog->Files->Strings[0];
      if(importa->Executa(filename))
         {//importa curva a partir do arquivo
         FormEditaCurva->DefineCurva(psup->curva);
         PageControl->ActivePage = TabSheetCurva;
         }
      }
   if(importa) delete importa;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSup::ActionRetiraExecute(TObject *Sender)
	{
	ModalResult = mrNo;
	}

//---------------------------------------------------------------------------
void __fastcall TFormEdtSup::cboxTipoSupChange(TObject *Sender)
   {
   //reinicia Captions,Labels etc da tela
   IniciaParteFixaTela();
   //atualiza dados de VPQ conforme op��o selecionada
   ExibeDadosVPQ();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtSup::ConvertePotCurtoToZeqv(void)
   {
   //vari�veis locais
   complex<double> scc_3f_pu, scc_ft_pu, z0_pu, z1_pu;

   try{//inicia valores de pot.curto complexa, em pu
      scc_3f_pu = complex<double> (psup->pcc_3f.p, psup->pcc_3f.q) / Sbase_mva;
      scc_ft_pu = complex<double> (psup->pcc_ft.p, psup->pcc_ft.q) / Sbase_mva;
		//calcula Zeqv
      z0_pu     = (3. / conj(scc_ft_pu)) - (2. / conj(scc_3f_pu));
      z1_pu     = (1. / conj(scc_3f_pu));
      //atualiza imp.equivalente do Suprimento
      psup->zeq0.r = z0_pu.real();
      psup->zeq0.x = z0_pu.imag();
      psup->zeq1.r = z1_pu.real();
      psup->zeq1.x = z1_pu.imag();

      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtSup::ConverteZeqvToPotCurto(void)
   {
   //vari�veis locais
   complex<double> scc_3f_mva, scc_ft_mva, z0_pu, z1_pu;

   try{//determina valores complexos de Zeq e Zeq1 em pu
      z0_pu      = complex<double>(psup->zeq0.r,psup->zeq0.x);
      z1_pu      = complex<double>(psup->zeq1.r,psup->zeq1.x);
      //calcula PotCC, em MVA
      scc_3f_mva = (1. / conj(z1_pu)) * Sbase_mva;
      scc_ft_mva = (3. / ((2. * conj(z1_pu)) + conj(z0_pu))) * Sbase_mva;
      //atualiza pot.curto do Suprimento
      psup->pcc_3f.p = RoundTo(scc_3f_mva.real(), -3);
      psup->pcc_3f.q = RoundTo(scc_3f_mva.imag(), -3);
      psup->pcc_ft.p = RoundTo(scc_ft_mva.real(), -3);
      psup->pcc_ft.q = RoundTo(scc_ft_mva.imag(), -3);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

// ---------------------------------------------------------------------------
VTCurva* __fastcall TFormEdtSup::CriaCurva(void)
{
    // vari�veis locais
    double valor[nvCURVA_PQVT];
    strHM hm_ini, hm_fim;
    VTCurva *curva;

    // cria Curva
    curva = DLL_NewObjCurva();
    curva->Tipica = false;
    curva->Escala = patEDITAVEL;
    curva->Tipo = curvaPQVT;
    curva->Unidade = unidPU;
    curva->NumeroValores = nvCURVA_PQVT;
    curva->Reconfigura(patEDITAVEL);
    // valor default alterado para 1pu se estiver zerado
    // valor[2] = 1;
    curva->GetPonto(0, hm_ini, hm_fim, valor, nvCURVA_PQVT);
    if (IsDoubleZero(valor[2]))
        valor[2] = 1.;
    curva->SetPonto(hm_ini, hm_fim, valor, nvCURVA_PQVT);
    return (curva);
}

//---------------------------------------------------------------------------
void __fastcall TFormEdtSup::ExibeDadosPotCC(void)
   {
   //vari�veis locais
   double pcc_ft, qcc_ft;

   //exibe valores de pot.curto trif�sico
   sgCurto->Cells[1][1] = DoubleToStr("%6.5f", psup->pcc_3f.p);
   sgCurto->Cells[2][1] = DoubleToStr("%6.5f", psup->pcc_3f.q);
   //verifica o tipo de defini��o de pot.curto fase/terra selecionado
   if (RadioGroupPcc_ft->ItemIndex == 0)
      {//pot.trif�sica
      pcc_ft = psup->pcc_ft.p;
      qcc_ft = psup->pcc_ft.q;
      }
   else
      {//pot.monof�sica
      pcc_ft = psup->pcc_ft.p / 3.;
      qcc_ft = psup->pcc_ft.q / 3.;
      }
   //exibe valores de pot.curto fase/terra
   sgCurto->Cells[1][2] = DoubleToStr("%6.5f", pcc_ft);
   sgCurto->Cells[2][2] = DoubleToStr("%6.5f", qcc_ft);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSup::ExibeDadosSup(void)
   {
   //inicia c�digo
   EditCodigo->Text = psup->Codigo;
   //inicia Smax do Suprimento
   edtSmax->Text = DoubleToStr("%4.3f", psup->smax);
	edtVnom->Text = DoubleToStr("%4.3f", psup->vnom);
   //inicia tipo de suprimento: PV
   switch(psup->tiposup)
      {
      case supPV : cboxTipoSup->ItemIndex = 0; break;
      case supPQ : cboxTipoSup->ItemIndex = 1; break;
      case supVF :
      default:     cboxTipoSup->ItemIndex = 2; break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSup::ExibeDadosVPQ(void)
   {
   FormEditaCurva->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSup::ExibeDadosZeq(void)
   {
   //inicia valores de z0
   sgZeq->Cells[1][1] = DoubleToStr("%10.9f",psup->zeq0.r);
   sgZeq->Cells[2][1] = DoubleToStr("%10.9f",psup->zeq0.x);
   //inicia valores z1
   sgZeq->Cells[1][2] = DoubleToStr("%10.9f",psup->zeq1.r);
   sgZeq->Cells[2][2] = DoubleToStr("%10.9f",psup->zeq1.x);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSup::FormClose(TObject *Sender, TCloseAction &Action)
   {
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSup::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      geral->Infoset->LoadFromFile(this);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSup::FormShow(TObject *Sender)
   {
   //seleciona TableSheet
   PageControl->ActivePage = TabSheetSuprimento;
   //inicia CBoxTipoSup (OBS: precisa ser iniciado antes de IniciaParteFixaTela())
   cboxTipoSup->ItemIndex = psup->tiposup - supPV;
   //inicia Captions,Labels etc da tela
   IniciaParteFixaTela();
   //inicia dados do Suprimento
   ExibeDadosSup();
   ExibeDadosVPQ();
   ExibeDadosZeq();
   ExibeDadosPotCC();
   //seleciona entrada de PotCC (e n�o de Zeqv)
   rgImpedancia->OnClick   = NULL;
   rgImpedancia->ItemIndex = 1;
   gboxZeq->Visible        = false;
   gboxCurto->Visible      = true;
   rgImpedancia->OnClick = rgImpedanciaClick;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSup::IniciaDadosSuprimentoNovo(void)
   {
   //vari�veis locais
//   strHM   hm_ini, hm_fim;
//   double  valor[nvCURVA_PQVT];

   //verifica se o Suprimento n�o est� definido
   if (psup->smax == 0.) psup->smax = 100.;
   if (psup->vnom == 0.) psup->vnom = psup->pbarra->vnom;
   //verifica se Pot.Curto est� definida
   if ((psup->pcc_3f.p == 0.)&&(psup->pcc_3f.q == 0.)&&
       (psup->pcc_ft.p == 0.)&&(psup->pcc_ft.q == 0.))
      {//verifica se Imp.Eqv est� definida
      if ((psup->zeq0.r   == 0.)&&(psup->zeq0.x == 0.)&&
          (psup->zeq1.r   == 0.)&&(psup->zeq1.x == 0.))
         {//define Imp.Eqv
         psup->zeq1.x = psup->zeq0.x = 0.05;
         }
      //calcula Pot.Curto
      ConverteZeqvToPotCurto();
      }
   else
      {///calcula Imp.Eqv
      ConvertePotCurtoToZeqv();
      }
   //verifica se o Suprimento possui uma Curva
   if (psup->curva == NULL)
      {//cria Curva e associa c/ o Suprimento
      psup->DefineCurva(CriaCurva());
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSup::IniciaParteFixaTela(void)
   {
   //inicia SGridCurto: t�tulos das colunas
   sgCurto->Cells[0][0] = "Pot.curto";
   sgCurto->Cells[1][0] = "P(MW)";
   //inicia SGridCurto: t�tulos das linhas
   sgCurto->Cells[2][0] = "Q(MVAr)";
   sgCurto->Cells[0][1] = "Trif�sico";
   sgCurto->Cells[0][2] = "Fase/terra";
   //inicia SGridZeq: t�tulos das colunas
   sgZeq->Cells[0][0] = "Imped�ncia";
   sgZeq->Cells[1][0] = "r(pu)";
   sgZeq->Cells[2][0] = "x(pu)";
   //inicia SGridZeq: t�tulos das linhas
   sgZeq->Cells[0][1] = "seq.0";
   sgZeq->Cells[0][2] = "seq.1";
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtSup::LeDadosCurva(void)
   {
   //determina tipo de suprimento
   switch(cboxTipoSup->ItemIndex)
      {
      case 0: psup->tiposup = supPV; break;
      case 1: psup->tiposup = supPQ; break;
      case 2: psup->tiposup = supVF; break;
      default:
         Erro("Defina o tipo de suprimento");
         return(false);
      }
   //l� os dados da Curva
   return(FormEditaCurva->LePontosCurva());
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtSup::LeDadosPotCC_CalculaZeqv(bool show_erro)
   {
   //vari�veis locais
   double          pcc_3f, qcc_3f, pcc_ft, qcc_ft;
   complex<double> scc_3f_pu, scc_ft_pu, z0_pu, z1_pu;

   try{//l� dados de curto trif�sico
      pcc_3f = StrToDouble(sgCurto->Cells[1][1]);
      qcc_3f = StrToDouble(sgCurto->Cells[2][1]);
      //dados de curto fase/terra
      pcc_ft = StrToDouble(sgCurto->Cells[1][2]);
      qcc_ft = StrToDouble(sgCurto->Cells[2][2]);
      //prote��o contra valores nulos
		if (IsDoubleZero(pcc_3f) && IsDoubleZero(qcc_3f))
			{//exibe mensagem de erro
			if (show_erro) Erro("Pot�ncia de curto trif�sico com valor nulo");
			return(false);
			}
		if (IsDoubleZero(pcc_ft)&&IsDoubleZero(qcc_ft))
         {//exibe mensagem de erro
			if (show_erro) Erro("Pot�ncia de curto fase/terra com valor nulo");
			return(false);
         }
      //verifica op��o de defini��o da pot.curto fase terra
      if (RadioGroupPcc_ft->ItemIndex == 1)
         {//converte para pot.fase/terra = Raiz(3.)*Vff*Icc
         pcc_ft *= 3.;
         qcc_ft *= 3.;
         }
      //atualiza pot.curto do Suprimento
      psup->pcc_3f.p = pcc_3f;
      psup->pcc_3f.q = qcc_3f;
      psup->pcc_ft.p = pcc_ft;
      psup->pcc_ft.q = qcc_ft;
      //calcula Zeq
      ConvertePotCurtoToZeqv();
      }catch(Exception &e)
         {//exibe mensagem de erro
			if (show_erro) Erro("Pot�ncias de curto: dados inv�lidos");
			return(false);
         }
   //atualiza tela
	ExibeDadosZeq();
	return(true);
   }


//---------------------------------------------------------------------------
bool __fastcall TFormEdtSup::LeDadosSup(void)
   {
   //vari�veis locais
   double v_pu;

   try{//atributos do suprimento
      psup->Codigo  = EditCodigo->Text.Trim();
      psup->smax    = StrToDouble(edtSmax->Text);
      psup->vnom    = StrToDouble(edtVnom->Text);
      }catch(Exception &e)
         {
         Erro("Valor Inv�lido: dados do suprimento");
         return(false);
         }
   //valida dados
/*
   if (psup->Codigo.IsEmpty())
      {
      Aviso("Defina um c�digo para o suprimento.");
      return(false);
      }
*/
   if (! IsDoubleZero(psup->pbarra->vnom))
      {
      v_pu = psup->vnom / psup->pbarra->vnom;
      if ((v_pu < 0.95)||(v_pu > 1.05))
         {
         Erro("Tens�o nominal incompat�vel c/ a tens�o da barra");
         return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtSup::LeDadosZeqv_CalculaPotCC(bool show_erro)
   {
   //vari�veis locais
   double          r0_pu, x0_pu, r1_pu, x1_pu;

   try{//l� valores de z0 em pu
      r0_pu = StrToDouble(sgZeq->Cells[1][1]);
      x0_pu = StrToDouble(sgZeq->Cells[2][1]);
      //l� valores z1
      r1_pu = StrToDouble(sgZeq->Cells[1][2]);
      x1_pu = StrToDouble(sgZeq->Cells[2][2]);
      //prote��o: Zseq1 n�o pode ser nula
      if (IsDoubleZero(r1_pu) && IsDoubleZero(x1_pu))
         {//exibe mensagem de erro
         if (show_erro) Erro("Valor Inv�lido: imped�ncia de seq.1");
			return(false);
         }
      //atualiza imp.equivalente do Suprimento
      psup->zeq0.r = r0_pu;
      psup->zeq0.x = x0_pu;
      psup->zeq1.r = r1_pu;
      psup->zeq1.x = x1_pu;
      //calcula PotCC
      ConverteZeqvToPotCurto();
      }catch(Exception &e)
         {//exibe mensagem de erro
			if (show_erro) Erro("Imped�ncias de seq��ncia com valor inv�lido");
			return(false);
         }
   //atualiza tela
	ExibeDadosPotCC();
	return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSup::PageControlChange(TObject *Sender)
	{
	//verifica se est� selecionado visualiza��o da Curva
   if (PageControl->ActivePage == TabSheetGrafico)
      {//atualiza FormExibeCurva
		FormExibeCurva->DefineCurva(psup, psup->curva);
		FormExibeCurva->HabilitaCurva(sup_PQVF);
		//FormExibeCurva->HabilitaCurva(sup_P);
		//FormExibeCurva->HabilitaCurva(sup_Q);
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormEdtSup::PageControlChanging(TObject *Sender, bool &AllowChange)
	{
   //verifica se deve ler dados editados
   if (PageControl->ActivePage == TabSheetSuprimento)
      {//l� os dados editados
		AllowChange = LeDadosSup();
		}
   else if (PageControl->ActivePage == TabSheetCurva)
      {//l� os pontos editados da curva
      AllowChange = LeDadosCurva();
		}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSup::RadioGroupPcc_ftClick(TObject *Sender)
   {
   //vari�veis locais
   double pcc_3f, qcc_3f;
   double pcc_ft, qcc_ft;

   try{//l� valores da tela: pot.curto trif�sico
      pcc_3f = StrToDouble(sgCurto->Cells[1][1]);
      qcc_3f = StrToDouble(sgCurto->Cells[2][1]);
      }catch(Exception &e)
         {
         pcc_3f = qcc_3f = 0;
         }
   try{//l� valores da tela: pot.curto fase/terra
      pcc_ft = StrToDouble(sgCurto->Cells[1][2]);
      qcc_ft = StrToDouble(sgCurto->Cells[2][2]);
      }catch(Exception &e)
         {
         pcc_ft = qcc_ft = 0;
         }
   //verifica o tipo de defini��o selecionado
   if (RadioGroupPcc_ft->ItemIndex == 0)
      {//converte para pot.trif�sica
      pcc_ft *= 3.;
      qcc_ft *= 3.;
      }
   else
      {//converte para pot.monof�sica
      //pcc_ft /= 3.;
      //qcc_ft /= 3.;
      }
   //atualiza pot.curto do Suprimento
   psup->pcc_3f.p = pcc_3f;
   psup->pcc_3f.q = qcc_3f;
   psup->pcc_ft.p = pcc_ft;
   psup->pcc_ft.q = qcc_ft;
   //reapresenta os valores
   ExibeDadosPotCC();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSup::rgImpedanciaClick(TObject *Sender)
   {
   //vari�veis locais
   bool show_erro = false;

   //verifica op��o selecionada
   if (rgImpedancia->ItemIndex == 0)
      {//exibe Zeq
      gboxZeq->Visible   = true;
      gboxCurto->Visible = false;
      //l� POtCC e converte p/ Zeqv
      LeDadosPotCC_CalculaZeqv(show_erro);
      }
   else
      {//exibe pot�ncia de curto
      gboxZeq->Visible   = false;
      gboxCurto->Visible = true;
      //l� Zeqv e converte p/ POtCC
      LeDadosZeqv_CalculaPotCC(show_erro);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtSup::ValidaPotCC(bool show_erro)
	{
	//vari�veis locais
	complex<double> pcc_ft(psup->pcc_ft.p, psup->pcc_ft.q);
	complex<double> pcc_3f(psup->pcc_3f.p, psup->pcc_3f.q);

	//verifica valores nulos
	if ((IsDoubleZero(psup->pcc_3f.p) && IsDoubleZero(psup->pcc_3f.q)) ||
		 (IsDoubleZero(psup->pcc_ft.p) && IsDoubleZero(psup->pcc_ft.q)))
		{
		if (show_erro) Erro("Defina pot�ncia de curto");
		return(false);
		}
	//verifica se pcc_3f > pcc_ft
	if (Abs(pcc_3f) < Abs(pcc_ft))
		{
		if (show_erro)
         {
         return(Confirma("Pot�ncias de curto: Scc_trif�sica < Scc_fase/terra",
                         "Os valores indicados est�o corretos ?") == IDYES);
         }
		return(true);
		}
	return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtSup::ValidaZeqv(bool show_erro)
   {
   //vari�veis locais
   complex<double> z_seq0, z_seq1;
	if (psup->zeq0.r < 0)
		{
		if (show_erro) Erro("Resist�ncia equivalente de seq.0 com valor negativo");
		return(false);
		}
	if (psup->zeq0.x < 0)
		{
		if (show_erro) Erro("Reat�ncia equivalente de seq.0 com valor negativo");
		return(false);
		}
	if (psup->zeq1.r < 0)
		{
		if (show_erro) Erro("Resist�ncia equivalentede seq.1 com valor negativo");
		return(false);
		}
	if (psup->zeq1.x < 0)
		{
		if (show_erro) Erro("Reat�ncia equivalente de seq.1 com valor negativo");
		return(false);
		}
	if (IsDoubleZero(psup->zeq0.r) && IsDoubleZero(psup->zeq0.x))
		{
		if (show_erro) Erro("Imped�ncia equivalente de seq.0 nula");
		return(false);
		}
	if (IsDoubleZero(psup->zeq1.r) && IsDoubleZero(psup->zeq1.x))
		{
		if (show_erro) Erro("Imped�ncia equivalente de seq.1 nula");
		return(false);
		}
	//verifica se Zseq0 > Zseq1
   z_seq0 = complex<double>(psup->zeq0.r, psup->zeq0.x);
   z_seq1 = complex<double>(psup->zeq1.r, psup->zeq1.x);
	if (Abs(z_seq0) < Abs(z_seq1))
		{
		if (show_erro)
         {
         return(Confirma("Imped�ncias equivalentes: Zseq_0 < Zseq_1",
                         "Os valores indicados est�o corretos ?") == IDYES);
         }
		return(true);
		}
	return(true);
   }

//---------------------------------------------------------------------------
//eof

