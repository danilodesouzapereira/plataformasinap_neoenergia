//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCurtoTabela.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Diretorio\VTPath.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrecho.h"
#include "..\RedeCC\ResCurto.h"
#include "..\RedeCC\VTBarCC.h"
#include "..\RedeCC\VTLigCC.h"
#include "..\RedeCC\VTRedeCC.h"
#include "..\Report\VTReport.h"
#include "..\..\DLL_Inc\Report.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
/* FKM 2015.08.12
* Fiz uma altera��o na rela��o do CBoxDefeito com o indice do defeito, pois ao
* excluir a exibi��o dos resultados do 1FT (franco), a rela��o que era direta se
* perdeu.
* tambem alterei a exibicao do nome do curto 1FTz para 1FT , assim todos os
* os curtos tem o valor da impedancia de defeito considerado (nenhum apresentava
* o z no nome).
* Se o usu�rio quer desconsiderar uma impedancia de defeito, basta usar = 0.
	FKM 2015.08.12 */

//---------------------------------------------------------------------------
__fastcall TFormCurtoTabela::TFormCurtoTabela(TComponent* Owner, TWinControl *parent, VTApl *apl)
   : TForm(Owner)
	{
	//salva ponteiro p/ objetos
	this->apl = apl;
	redeCC    = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
	//salva dimens�o m�nima p/ este form
	width_min = Width;
	//insere Form em seu Parent
	if (parent)
		{
		Parent = parent;
		Align  = alClient;
		}
   //inicia CBoxDefeito
   IniciaCBoxDefeito();
	}

//---------------------------------------------------------------------------
__fastcall TFormCurtoTabela::~TFormCurtoTabela(void)
	{
	//nada a fazer
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::ButRelatorioClick(TObject *Sender)
   {
   ExportaRelatorio();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::CBoxChange(TObject *Sender)
	{
	IniciaLView();
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::EqptoDefeito(VTEqpto *eqpto)
	{
   //limpa todos LView
   LViewDefIcc->Clear();
   LViewDefVcc->Clear();
   LViewSupIcc->Clear();
   LViewBarVcc->Clear();
   LViewLigIcc->Clear();
   LViewLigScc->Clear();
   //seleciona TabSheet
   PageControl->ActivePage = TabSheetPtoDefeito;
   //inicia apenas LView do ponto de defeito
   IniciaLView();
   //IniciaLViewDefIcc();
   //IniciaLViewDefVcc();
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::EqptoSelecionado(VTEqpto *eqpto)
	{
	//vari�veis locais
	TListItem *Item;
	VTBarCC   *barCC;
	VTLigCC   *ligCC;

	//verifica tipo de Eqpto
	if (eqpto->TipoBarra())
		{//seleciona ListItem assoaciado � Barra
		if ((Item = ExisteEqpto(LViewSupIcc, eqpto)) != NULL)
			{
			LViewSupIcc->Selected = Item;
			}
		else if ((Item = ExisteEqpto(LViewBarVcc, eqpto)) != NULL)
			{
			LViewBarVcc->Selected = Item;
			}
		}
	else	if (eqpto->TipoLigacao())
		{
		if ((Item = ExisteEqpto(LViewLigIcc, eqpto)) != NULL)
			{
			LViewLigIcc->Selected = Item;
			}
		}
	}

//---------------------------------------------------------------------------
TListItem* __fastcall TFormCurtoTabela::ExisteEqpto(TListView *LView, VTEqpto *eqpto)
	{
	//vari�veis locais
	TListItem *Item;

	//procura ListItem associado ao Eqpto
	for (int n = 0; n < LView->Items->Count; n++)
		{
		if (LView->Items->Item[n]->Data == eqpto) return(LView->Items->Item[n]);
		}
	return(NULL);
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::ExportaRelatorio(void)
   {
   //vari�veis locais
   AnsiString arq_ext;
   VTPath     *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //define diret�rio
   if (SaveDialog->InitialDir.IsEmpty()) SaveDialog->InitialDir = path->DirExporta();
   //define nome do arquivo
   SaveDialog->FileName = "RelCurtoCircuito";
   //simula sele��o de tipo de arquivo p/ definir extens�o default
   SaveDialog->FilterIndex = 1;
   SaveDialogTypeChange(NULL);
   //seleciona arquivo
   if (SaveDialog->Execute())
      {//determina tipo do arquivo pelo FilterIndex
      if (SaveDialog->FilterIndex == 1) ExportaRelatorioTexto(SaveDialog->FileName);
      else                              ExportaRelatorioExcel(SaveDialog->FileName);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::ExportaRelatorioExcel(AnsiString arq)
   {
   //vari�veis locais
   VTReport *report;

   try{//cria objeto VTReport
      report = DLL_NewObjReport(apl);
      //tens�es e correntes no ponto de defeito
      report->PlanilhaExcel(arq, "TensaoPtoDefeito",   LViewDefVcc);
      report->PlanilhaExcel(arq, "CorrentePtoDefeito", LViewDefIcc);
      //contribui��es, tens�es e correntes
      report->PlanilhaExcel(arq, "Contribuicao", LViewSupIcc);
      report->PlanilhaExcel(arq, "Tensoes",      LViewBarVcc);
      report->PlanilhaExcel(arq, "Correntes",    LViewLigIcc);
      //destr�i objeto
      delete report;
      //gera aviso para o usu�rio
      Aviso("O relat�rio foi exportado corretamente");
      }catch(Exception &e)
         {
         Erro("N�o foi poss�vel gravar o arquivo " + ExtractFileName(arq));
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::ExportaRelatorioTexto(AnsiString arq)
   {
   //vari�veis locais
   bool       append = true;
   AnsiString titulo;
   VTReport   *report;

   try{//cria objeto VTReport
      report = DLL_NewObjReport(apl);
      //tens�es no ponto de defeito
      titulo = CBoxDefeito->Text + ": Tensoes Ponto de Defeito";
      report->ArquivoTexto(arq, LViewDefVcc, titulo, !append);
      //correntes no ponto de defeito
      titulo = CBoxDefeito->Text + ": Correntes Ponto de Defeito";
      report->ArquivoTexto(arq, LViewDefIcc, titulo, append);
      //contribui��es
      titulo = CBoxDefeito->Text + ": Contribuicoes";
      report->ArquivoTexto(arq, LViewSupIcc, titulo, append);
      //tens�es
      titulo = CBoxDefeito->Text + ": Tensoes";
      report->ArquivoTexto(arq, LViewBarVcc, titulo, append);
      //correntes
      titulo = CBoxDefeito->Text + ": Correntes";
      report->ArquivoTexto(arq, LViewLigIcc, titulo, append);
      //destr�i objeto
      delete report;
      //gera aviso para o usu�rio
      Aviso("O relat�rio foi exportado corretamente");
      }catch(Exception &e)
         {
         Erro("N�o foi poss�vel gravar o arquivo " + ExtractFileName(arq));
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::FormClose(TObject *Sender, TCloseAction &Action)
   {
	//esconde este Form
	Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::FormShow(TObject *Sender)
   {
	//seleciona TabSheet
   PageControl->ActivePage = TabSheetPtoDefeito;
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::IniciaCBoxDefeito(void)
   {
   //limpa CBoxDefeito
   CBoxDefeito->Clear();
   //reinicia CBoxDefeito
	for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
      {
		//FKM - 2015.08.12 esconder o 1FT franco
		if(ind_def != defeitoFT) CBoxDefeito->Items->Add(NomeDefeito(ind_def));
      }
   //pr�-seleciona primeiro defeito
   CBoxDefeito->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::IniciaLView(void)
	{
	//inicia todos LView
	IniciaLViewDefIcc();
	IniciaLViewDefVcc();
	IniciaLViewSupIcc();
	IniciaLViewBarVcc();
	IniciaLViewLigIcc();
	IniciaLViewLigScc();
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::IniciaLViewBarVcc(void)
   {
   //vari�veis locais
	int        ind_def;
   VTBarra    *barra_fic;
   VTBarCC    *barCC;
	TList      *lisBAR_CC;
	TListItem  *Item;
   TListView  *LView = LViewBarVcc;

   //bloqueia tratamento do evento OnChange
   LView->OnChange = NULL;
   //reinicia LView
	LView->Items->Clear();
	LView->Items->BeginUpdate();
	//FKM 2015.08.12
	//com a exclusao do 1FT , � ness�rio incrementar o ind_def
	//pois n�o corresponde mais com o indice do combo
	//defeito3F=0, defeito2F=1, defeitoFT=, defeitoFTZ=2, defeito2FT=3
	//determina �ndice do defeito selecionado
	if ((ind_def = CBoxDefeito->ItemIndex) < 0) return;
	else if(ind_def >= 2 ) ind_def = CBoxDefeito->ItemIndex + 1;
	//FKM 2015.08.12
   //se defeito no Trecho, determina Barra fict�cia
   barra_fic = NULL;
   if (redeCC->TrechoDefeito() != NULL) barra_fic = redeCC->BarraDefeito();
	//loop p/ todas BarCC
	lisBAR_CC = redeCC->LisBarraCC();
	for (int nl = 0; nl < lisBAR_CC->Count; nl++)
		{
		barCC = (VTBarCC*)lisBAR_CC->Items[nl];
		//n�o exibe dados da Barra fict�cia usada p/ defeito no Trecho
      if (barCC->Barra == barra_fic) continue;
      //insere ListItem no ListView
      Item = LView->Items->Add();
      //associa barCC ao ListItem
		Item->Data = barCC->Barra;
      //insere tipo da Ligacao
      Item->Caption = barCC->Barra->TipoAsString();
      //insere identifica��o da Barra
      Item->SubItems->Add(barCC->Barra->Codigo);
      //verifica a unidade: pu ou kV
      if (CBoxUnidTensao->ItemIndex == 0)
			{//insere tens�es de fase em pu
			for (int ind_fas = 0; ind_fas < NUM_MAX_FAS; ind_fas++)
				{Item->SubItems->Add(ComplexPolarToStr("%4.3f", barCC->Vfas_pu[ind_def][ind_fas]));}
			//insere tens�es de sequencia em pu
			for (int ind_seq = 0; ind_seq < NUM_MAX_FAS; ind_seq++)
				{Item->SubItems->Add(ComplexPolarToStr("%4.3f", barCC->Vseq_pu[ind_def][ind_seq]));}
			}
		else
			{//insere tens�es de fase em kV
         for (int ind_fas = 0; ind_fas < NUM_MAX_FAS; ind_fas++)
            {Item->SubItems->Add(ComplexPolarToStr("%4.3f", barCC->Vfas_kv[ind_def][ind_fas]));}
         //insere tens�es de sequencia
         for (int ind_seq = 0; ind_seq < NUM_MAX_FAS; ind_seq++)
            {Item->SubItems->Add(ComplexPolarToStr("%4.3f", barCC->Vseq_kv[ind_def][ind_seq]));}
         }
	  }
	LView->Items->EndUpdate();
   //restaura tratamento do evento OnChange
   LView->OnChange = LViewChange;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::IniciaLViewDefIcc(void)
   {
   //vari�veis locais
   int        ind_def;
   VTBarCC    *barCC;
   VTEqpto    *eqpto;
   TListItem  *Item;
   TListView  *LView =  LViewDefIcc;
   double     fat_conv;

   //determina fator de conver�o
   fat_conv = (CBoxUnidCorrente->ItemIndex == 1) ? 1000. : 1.;
   //reinicia LView
   LView->Items->Clear();
   LView->Items->BeginUpdate();
	//FKM 2015.08.12
	//com a exclusao do 1FT , � ness�rio incrementar o ind_def
	//pois n�o corresponde mais com o indice do combo
	//defeito3F=0, defeito2F=1, defeitoFT=, defeitoFTZ=2, defeito2FT=3
	//determina �ndice do defeito selecionado
	if ((ind_def = CBoxDefeito->ItemIndex) < 0) return;
	else if(ind_def >= 2 ) ind_def = CBoxDefeito->ItemIndex + 1;
	//FKM 2015.08.12
   //determina Eqpto de defeito
   eqpto = redeCC->EqptoDefeito();
   //determina BarCC usado apenas p/ armazenar correntes de defeito
//   if ((barCC = redeCC->CorrenteDefeito()) == NULL) return;
   if ((barCC = redeCC->BarCCDefeito()) == NULL) return;
   //insere ListItem no ListView
   Item = LView->Items->Add();
   //N�O associa Barra de defeito ao ListItem
	Item->Data = NULL;
   //insere tipo do eqpto do defeito (barra ou trecho)
	Item->Caption = eqpto->TipoAsString();
   //insere identifica��o do eqpto do defeito
   Item->SubItems->Add(eqpto->Codigo);
   //insere correntes de fase em kA
   for (int ind_fas = 0; ind_fas < NUM_MAX_FAS; ind_fas++)
      {
      Item->SubItems->Add(ComplexPolarToStr("%4.3f", barCC->Ifas_amp[ind_def][ind_fas] / fat_conv));
      }
   //insere correntes de sequencia em kA
   for (int ind_seq = 0; ind_seq < NUM_MAX_FAS; ind_seq++)
      {
      Item->SubItems->Add(ComplexPolarToStr("%4.3f", barCC->Iseq_amp[ind_def][ind_seq] / fat_conv));
	  }
   LView->Items->EndUpdate();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::IniciaLViewDefVcc(void)
   {
	//vari�veis locais
	int        ind_def;
	VTBarCC    *barCC;
	VTEqpto    *eqpto;
	TListItem  *Item;
	TListView  *LView =  LViewDefVcc;

   //bloqueia tratamento do evento OnChange
   LView->OnChange = NULL;
	//reinicia LView
	LView->Items->Clear();
	LView->Items->BeginUpdate();
	//FKM 2015.08.12
	//com a exclusao do 1FT , � ness�rio incrementar o ind_def
	//pois n�o corresponde mais com o indice do combo
	//defeito3F=0, defeito2F=1, defeitoFT=, defeitoFTZ=2, defeito2FT=3
	//determina �ndice do defeito selecionado
	if ((ind_def = CBoxDefeito->ItemIndex) < 0) return;
	else if(ind_def >= 2 ) ind_def = CBoxDefeito->ItemIndex + 1;
	//FKM 2015.08.12
   //determina Eqpto de defeito
   eqpto = redeCC->EqptoDefeito();
   //determina BarCC de defeito
   if ((barCC = redeCC->BarCCDefeito()) == NULL) return;
   //insere ListItem no ListView
   Item = LView->Items->Add();
   //N�O associa Barra de defeito ao ListItem
   Item->Data = NULL;
   //insere tipo do eqpto do defeito (barra ou trecho)
   Item->Caption = eqpto->TipoAsString();
   //insere identifica��o do eqpto do defeito
   Item->SubItems->Add(eqpto->Codigo);
   //insere ten�es de fase
   for (int ind_fas = 0; ind_fas < NUM_MAX_FAS; ind_fas++)
      {//verifica a unidade
      if (CBoxUnidTensao->ItemIndex == 0)
         {Item->SubItems->Add(ComplexPolarToStr("%4.3f", barCC->Vfas_pu[ind_def][ind_fas]));}
      else
         {Item->SubItems->Add(ComplexPolarToStr("%4.3f", barCC->Vfas_kv[ind_def][ind_fas]));}
      }
   //insere correntes de sequencia
   for (int ind_seq = 0; ind_seq < NUM_MAX_FAS; ind_seq++)
      {//verifica a unidade
      if (CBoxUnidTensao->ItemIndex == 0)
         {Item->SubItems->Add(ComplexPolarToStr("%4.3f", barCC->Vseq_pu[ind_def][ind_seq]));}
      else
         {Item->SubItems->Add(ComplexPolarToStr("%4.3f", barCC->Vseq_kv[ind_def][ind_seq]));}
	  }
   LView->Items->EndUpdate();
   //restaura tratamento do evento OnChange
   LView->OnChange = LViewChange;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::IniciaLViewLigIcc(void)
   {
	//vari�veis locais
	int        ind_def;
	VTLigCC    *ligCC;
	VTTrafo    *trafo;
	TList      *lisLIG_CC;
	TListView  *LView =  LViewLigIcc;

   //bloqueia tratamento do evento OnChange
   LView->OnChange = NULL;
	//reinicia LView
	LView->Items->Clear();
	LView->Items->BeginUpdate();
	//FKM 2015.08.12
	//com a exclusao do 1FT , � ness�rio incrementar o ind_def
	//pois n�o corresponde mais com o indice do combo
	//defeito3F=0, defeito2F=1, defeitoFT=, defeitoFTZ=2, defeito2FT=3
	//determina �ndice do defeito selecionado
	if ((ind_def = CBoxDefeito->ItemIndex) < 0) return;
	else if(ind_def >= 2 ) ind_def = CBoxDefeito->ItemIndex + 1;
	//FKM 2015.08.12
	//loop p/ todas LigCC
   lisLIG_CC = redeCC->LisLigacaoCC();
   for (int nl = 0; nl < lisLIG_CC->Count; nl++)
      {
      ligCC = (VTLigCC*)lisLIG_CC->Items[nl];
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         IniciaLViewLigIcc(ligCC, ind_def, ind_bar);
         }
	  }
   LView->Items->EndUpdate();
   //restaura tratamento do evento OnChange
   LView->OnChange = LViewChange;
   }
//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::IniciaLViewLigScc(void)
   {
	//vari�veis locais
	int        ind_def;
	VTLigCC    *ligCC;
	VTTrafo    *trafo;
	TList      *lisLIG_CC;
	TListView  *LView =  LViewLigScc;

   //bloqueia tratamento do evento OnChange
   LView->OnChange = NULL;
	//reinicia LView
	LView->Items->Clear();
	LView->Items->BeginUpdate();
	//FKM 2015.08.12
	//com a exclusao do 1FT , � ness�rio incrementar o ind_def
	//pois n�o corresponde mais com o indice do combo
	//defeito3F=0, defeito2F=1, defeitoFT=, defeitoFTZ=2, defeito2FT=3
	//determina �ndice do defeito selecionado
	if ((ind_def = CBoxDefeito->ItemIndex) < 0) return;
	else if(ind_def >= 2 ) ind_def = CBoxDefeito->ItemIndex + 1;
	//FKM 2015.08.12
	//loop p/ todas LigCC
   lisLIG_CC = redeCC->LisLigacaoCC();
   for (int nl = 0; nl < lisLIG_CC->Count; nl++)
      {
      ligCC = (VTLigCC*)lisLIG_CC->Items[nl];
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
		 IniciaLViewLigScc(ligCC, ind_def, ind_bar);
         }
	  }
   LView->Items->EndUpdate();
   //restaura tratamento do evento OnChange
   LView->OnChange = LViewChange;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::IniciaLViewLigScc(VTLigCC *ligCC, int ind_def, int ind_bar)
	{
	//vari�veis locais
	AnsiString txt;
	VTBarra    *bar_ref;
	TListItem  *Item;
	TListView  *LView =  LViewLigScc;
	double     fat_conv;
	complex<double> s;
	double p, q, sMod, sArg;

   //determina Barra
   if ((bar_ref = ligCC->Ligacao->Barra(ind_bar)) == NULL) return;
	//determina fator de conver�o
	fat_conv = (CBoxUnidPotencia->ItemIndex == 0) ? 1000 : 1;
	//insere ListItem no ListView
	Item = LView->Items->Add();
	//associa LigCC ao ListItem
	Item->Data = ligCC->Ligacao;
	//insere tipo da Ligacao
	Item->Caption = ligCC->Ligacao->TipoAsString();
	//insere identifica��o da Ligacao
	Item->SubItems->Add(ligCC->Ligacao->Codigo);
   //insere c�digo da Barra de refer�ncia
   if (bar_ref->Codigo.IsEmpty()) txt.sprintf("bar.%d",     ind_bar+1);
   else                           txt.sprintf("bar.%d: %s", ind_bar+1, bar_ref->Codigo.c_str());
   Item->SubItems->Add(txt);

   //guarda a potencia aparente
   s = ligCC->S_mva[ind_def][ind_bar];
   //adiciona o valor de P
   p = s.real();
   Item->SubItems->Add(DoubleToStr("%4.3f",p*fat_conv));
   //adiciona o valor de Q
   q = s.imag();
   Item->SubItems->Add(DoubleToStr("%4.3f",q*fat_conv));
   //adiciona o valor de S mod
   sMod = Abs(s);
   //Item->SubItems->Add(DoubleToStr("%4.3f",sMod*fat_conv));
   //adiciona o valor de S arg
   sArg = ArgGrau(s);
   //Item->SubItems->Add(DoubleToStr("%2.1f",sArg));
   Item->SubItems->Add(ComplexPolarToStr("%4.3f", ligCC->S_mva[ind_def][ind_bar]*fat_conv));
   }
//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::IniciaLViewLigIcc(VTLigCC *ligCC, int ind_def, int ind_bar)
	{
	//vari�veis locais
   AnsiString txt;
   VTBarra    *bar_ref;
	TListItem  *Item;
	TListView  *LView =  LViewLigIcc;
	double     fat_conv;

   //determina Barra
   if ((bar_ref = ligCC->Ligacao->Barra(ind_bar)) == NULL) return;
	//determina fator de conver�o
	fat_conv = (CBoxUnidCorrente->ItemIndex == 1) ? 1000. : 1.;
	//insere ListItem no ListView
	Item = LView->Items->Add();
	//associa LigCC ao ListItem
	Item->Data = ligCC->Ligacao;
	//insere tipo da Ligacao
	Item->Caption = ligCC->Ligacao->TipoAsString();
	//insere identifica��o da Ligacao
	Item->SubItems->Add(ligCC->Ligacao->Codigo);
   //insere c�digo da Barra de refer�ncia
   if (bar_ref->Codigo.IsEmpty()) txt.sprintf("bar.%d",     ind_bar+1);
   else                           txt.sprintf("bar.%d: %s", ind_bar+1, bar_ref->Codigo.c_str());
   Item->SubItems->Add(txt);
	//insere correntes de fase em kA
   for (int ind_fas = 0; ind_fas < NUM_MAX_FAS; ind_fas++)
      {
		Item->SubItems->Add(ComplexPolarToStr("%4.3f", ligCC->Ifas_amp[ind_def][ind_bar][ind_fas] / fat_conv));
      }
   //insere correntes de sequencia em kA
   for (int ind_seq = 0; ind_seq < NUM_MAX_FAS; ind_seq++)
      {
      Item->SubItems->Add(ComplexPolarToStr("%4.3f", ligCC->Iseq_amp[ind_def][ind_bar][ind_seq] / fat_conv));
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::IniciaLViewSupIcc(void)
   {
	//vari�veis locais
	int        ind_def;
	double     fat_conv;
	VTBarCC    *barCC;
	TList      *lisBAR_CC;
	TListItem  *Item;
	TListView  *LView = LViewSupIcc;

   //bloqueia tratamento do evento OnChange
   LView->OnChange = NULL;
	//reinicia LView
	LView->Items->Clear();
	//FKM 2015.08.12
		//com a exclusao do 1FT , � ness�rio incrementar o ind_def
		//pois n�o corresponde mais com o indice do combo
		//defeito3F=0, defeito2F=1, defeitoFT=, defeitoFTZ=2, defeito2FT=3
	//determina �ndice do defeito selecionado
	if ((ind_def = CBoxDefeito->ItemIndex) < 0) return;
	else if(ind_def >= 2 ) ind_def = CBoxDefeito->ItemIndex + 1;
	//FKM 2015.08.12
	//determina fator de conver�o
	fat_conv = (CBoxUnidCorrente->ItemIndex == 1) ? 1000. : 1.;
	//loop p/ todas BarCC
	lisBAR_CC = redeCC->LisBarraCC();
	for (int n = 0; n < lisBAR_CC->Count; n++)
		{
		barCC = (VTBarCC*)lisBAR_CC->Items[n];
		//verifica se a Barra associada a BarCC possui Gerador/Supimento
		if (! barCC->Barra->ExisteGeracao()) continue;
		//insere ListItem no ListView
		Item = LView->Items->Add();
		//associa barCC ao ListItem
		Item->Data = barCC->Barra;
		//insere tipo: barra
		Item->Caption = barCC->Barra->TipoAsString();
		//insere identifica��o da Barra
		Item->SubItems->Add(barCC->Barra->Codigo);
		//correntes de fase
		for (int ind_fas = 0; ind_fas < NUM_MAX_FAS; ind_fas++)
			{Item->SubItems->Add(ComplexPolarToStr("%4.3f", barCC->Ifas_amp[ind_def][ind_fas] / fat_conv));}
		//correntes de sequencia
		for (int ind_seq = 0; ind_seq < NUM_MAX_FAS; ind_seq++)
			{Item->SubItems->Add(ComplexPolarToStr("%4.3f", barCC->Iseq_amp[ind_def][ind_seq] / fat_conv));}
		}
   //restaura tratamento do evento OnChange
   LView->OnChange = LViewChange;
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::LViewChange(TObject *Sender, TListItem *Item, TItemChange Change)
	{
	//vari�veis locais
	VTEqpto *eqpto;
	TListView *LView = (TListView*)Sender;

	//verifica tipo de altera��o
	if (Change != ctState ) return;
	//verifica se ListItem indicado � o selecionado
	if (LView->Selected != Item) return;
	//atualiza Moldura com o Eqpto do  ListItem selecionado
	//determina Eqpto associado ao
	Moldura((VTEqpto*)Item->Data);
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::Moldura(VTEqpto *eqpto)
   {
   //vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	//atualiza Moldura
	grafico->Moldura(eqpto);
	}

//-----------------------------------------------------------------------------
AnsiString __fastcall TFormCurtoTabela::NomeDefeito(int ind_def)
   {
	//vari�veis locais
	//original
	//char *tipo_def[] = {"3F(abc)", "2F(bc)", "1FT(at)", "1FTz(at)", "2FT(bct)"};
	//FKM - 2015.08.12 - Mudan�a do nome exibido do curto 1FTz para 1FT
	char *tipo_def[] = {"3F(abc)", "2F(bc)", "1FT(at)", "1FT(at)", "2FT(bct)"};

	return(tipo_def[ind_def]);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::PageControlChange(TObject *Sender)
	{
	//vari�veis locais
	VTEqpto *eqpto = NULL;

	//determina ListView associado ao TabSheet ativo
	if (PageControl->ActivePage == TabSheetPtoDefeito)
		{//Eqpto de defeito: n�o mostra moldura
		eqpto = NULL;
		}
	else if (PageControl->ActivePage == TabSheetContribuicao)
		{//verifica se o LView est� preenchido
      if (LViewSupIcc->Items->Count == 0) IniciaLViewSupIcc();
		//determina Eqpto associado ao TListItem selecionado
		if (LViewSupIcc->Selected != NULL) eqpto = (VTEqpto*)LViewSupIcc->Selected->Data;
		}
	else if (PageControl->ActivePage == TabSheetTensao)
		{//verifica se o LView est� preenchido
      if (LViewBarVcc->Items->Count == 0) IniciaLViewBarVcc();
      //determina Eqpto associado ao TListItem selecionado
		if (LViewBarVcc->Selected != NULL) eqpto = (VTEqpto*)LViewBarVcc->Selected->Data;
		}
	else if (PageControl->ActivePage == TabSheetCorrente)
		{//verifica se o LView est� preenchido
      if (LViewLigIcc->Items->Count == 0) IniciaLViewLigIcc();
		//determina Eqpto associado ao TListItem selecionado
		if (LViewLigIcc->Selected != NULL) eqpto = (VTEqpto*)LViewLigIcc->Selected->Data;
		}
	else if (PageControl->ActivePage == TabSheetPotencias)
		{//verifica se o LView est� preenchido
	  if (LViewLigScc->Items->Count == 0) IniciaLViewLigScc();
		//determina Eqpto associado ao TListItem selecionado
		if (LViewLigScc->Selected != NULL) eqpto = (VTEqpto*)LViewLigScc->Selected->Data;
		}
	//atualiza Moldura
	Moldura(eqpto);
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::LviewSelecionaZoom(TObject *Sender)
	{
	//vari�veis locais
	VTEqpto   *eqpto;
	TListView *LView   = (TListView*)Sender;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	//verifica se h� um Eqpto selecionado
	if (LView->Selected == NULL) return;
	if ((eqpto = (VTEqpto*)LView->Selected->Data) == NULL) return;
	//seleciona �rea de zoom p/ destacar o Eqpto
	grafico->SelecionaZoom(eqpto);
	}

//---------------------------------------------------------------------------
void __fastcall TFormCurtoTabela::SaveDialogTypeChange(TObject *Sender)
   {
   //define extens�o default em fun��o do tipo de arquivo
   if (SaveDialog->FilterIndex == 1) SaveDialog->DefaultExt  = ".txt";
   else                              SaveDialog->DefaultExt  = ".xls";
   }

//---------------------------------------------------------------------------
int __fastcall TFormCurtoTabela::WidthMinimo(void)
	{
	return(width_min);
	}

//---------------------------------------------------------------------------
//eof

