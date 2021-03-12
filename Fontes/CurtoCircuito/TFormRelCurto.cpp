//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormRelCurto.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrecho.h"
#include "..\RedeCC\ResCurto.h"
#include "..\RedeCC\VTBarCC.h"
#include "..\RedeCC\VTLigCC.h"
#include "..\RedeCC\VTRedeCC.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormRelCurto::TFormRelCurto(TComponent* Owner, TWinControl *parent, VTApl *apl)
   : TForm(Owner)
   {
	//salva ponteiro p/ objetos
	this->apl = apl;
	redeCC    = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
   //insere Form em seu Parent
   if (parent) {Parent = parent;}
   }

//---------------------------------------------------------------------------
__fastcall TFormRelCurto::~TFormRelCurto(void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelCurto::ActionAtualizaExecute(TObject *Sender)
   {
   //inicia LView c/ dados de tensào e corrente do defeito selecionado
   IniciaLViewDefIcc();
   IniciaLViewDefVcc();
   IniciaLViewSupIcc();
   IniciaLViewBarVcc();
   IniciaLViewLigIcc();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelCurto::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelCurto::FormShow(TObject *Sender)
   {
   //inicia CBoxDefeito
   IniciaCBoxDefeito();
   //seleciona TabSheet
   PageControl->ActivePage = TabSheetPtoDefeito;
   //inicia LView c/ dados de tensão e corrente do defeito selecionado
   ActionAtualizaExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelCurto::IniciaCBoxDefeito(void)
   {
   //limpa CBoxDefeito
   CBoxDefeito->Clear();
   //reinicia CBoxDefeito
   for (int ind_def = 0; ind_def < NUM_MAX_DEF; ind_def++)
      {
      CBoxDefeito->Items->Add(NomeDefeito(ind_def));
      }
   //pré-seleciona primeiro defeito
   CBoxDefeito->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelCurto::IniciaLViewBarVcc(void)
   {
   //variáveis locais
   int        ind_def;
   VTBarra    *barra_fic;
   VTBarCC    *barCC;
	TList      *lisBAR_CC;
	TListItem  *Item;
   TListView  *LView = LViewBarVcc;

   //reinicia LView
   LView->Items->Clear();
   //determina índice do defeito selecionado
   if ((ind_def = CBoxDefeito->ItemIndex) < 0) return;
   //se defeito no Trecho, determina Barra fictícia
   barra_fic = NULL;
   if (redeCC->TrechoDefeito() != NULL) barra_fic = redeCC->BarraDefeito();
	//loop p/ todas BarCC
	lisBAR_CC = redeCC->LisBarraCC();
	for (int nl = 0; nl < lisBAR_CC->Count; nl++)
		{
		barCC = (VTBarCC*)lisBAR_CC->Items[nl];
		//não exibe dados da Barra fictícia usada p/ defeito no Trecho
      if (barCC->Barra == barra_fic) continue;
      //insere ListItem no ListView
      Item = LView->Items->Add();
      //associa barCC ao ListItem
		Item->Data = barCC->Barra;
      //insere tipo da Ligacao
      Item->Caption = barCC->Barra->TipoAsString();
      //insere identificação da Barra
      Item->SubItems->Add(barCC->Barra->Codigo);
      //verifica a unidade: pu ou kV
      if (CBoxUnidTensao->ItemIndex == 0)
			{//insere tensões de fase em pu
			for (int ind_fas = 0; ind_fas < NUM_MAX_FAS; ind_fas++)
				{Item->SubItems->Add(ComplexPolarToStr("%4.3f", barCC->Vfas_pu[ind_def][ind_fas]));}
			//insere tensões de sequencia em pu
			for (int ind_seq = 0; ind_seq < NUM_MAX_FAS; ind_seq++)
				{Item->SubItems->Add(ComplexPolarToStr("%4.3f", barCC->Vseq_pu[ind_def][ind_seq]));}
			}
		else
			{//insere tensões de fase em kV
         for (int ind_fas = 0; ind_fas < NUM_MAX_FAS; ind_fas++)
            {Item->SubItems->Add(ComplexPolarToStr("%4.3f", barCC->Vfas_kv[ind_def][ind_fas]));}
         //insere tensões de sequencia
         for (int ind_seq = 0; ind_seq < NUM_MAX_FAS; ind_seq++)
            {Item->SubItems->Add(ComplexPolarToStr("%4.3f", barCC->Vseq_kv[ind_def][ind_seq]));}
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelCurto::IniciaLViewDefIcc(void)
   {
   //variáveis locais
   int        ind_def;
   VTBarCC    *barCC;
   VTEqpto    *eqpto;
   TListItem  *Item;
   TListView  *LView =  LViewDefIcc;
   double     fat_conv;

   //determina fator de converão
   fat_conv = (CBoxUnidCorrente->ItemIndex == 1) ? 1000. : 1.;
   //reinicia LView
   LView->Items->Clear();
   //determina índice do defeito selecionado
   if ((ind_def = CBoxDefeito->ItemIndex) < 0) return;
   //determina Barra ou Trecho do defeito
   if ((eqpto = redeCC->TrechoDefeito()) == NULL) eqpto = redeCC->BarraDefeito();
   if (eqpto == NULL) return;
   //determina BarCC de defeito
   if ((barCC = redeCC->BarCCDefeito()) == NULL) return;
   //insere ListItem no ListView
   Item = LView->Items->Add();
   //NÃO associa Barra de defeito ao ListItem
	Item->Data = NULL;
   //insere tipo do eqpto do defeito (barra ou trecho)
	Item->Caption = eqpto->TipoAsString();
   //insere identificação do eqpto do defeito
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
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelCurto::IniciaLViewDefVcc(void)
   {
	//variáveis locais
	int        ind_def;
	VTBarCC    *barCC;
	VTEqpto    *eqpto;
	TListItem  *Item;
	TListView  *LView =  LViewDefVcc;

	//reinicia LView
	LView->Items->Clear();
	//determina índice do defeito selecionado
	if ((ind_def = CBoxDefeito->ItemIndex) < 0) return;
	//determina Barra ou Trecho do defeito
   if ((eqpto = redeCC->TrechoDefeito()) == NULL) eqpto = redeCC->BarraDefeito();
   if (eqpto == NULL) return;
   //determina BarCC de defeito
   if ((barCC = redeCC->BarCCDefeito()) == NULL) return;
   //insere ListItem no ListView
   Item = LView->Items->Add();
   //NÃO associa Barra de defeito ao ListItem
   Item->Data = NULL;
   //insere tipo do eqpto do defeito (barra ou trecho)
   Item->Caption = eqpto->TipoAsString();
   //insere identificação do eqpto do defeito
   Item->SubItems->Add(eqpto->Codigo);
   //insere tenões de fase
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
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelCurto::IniciaLViewLigIcc(void)
   {
	//variáveis locais
	int        ind_bar = 0;
	VTLigCC    *ligCC;
	VTTrafo    *trafo;
	TList      *lisLIG_CC;
	TListView  *LView =  LViewLigIcc;

	//reinicia LView
	LView->Items->Clear();
	//loop p/ todas LigCC
   lisLIG_CC = redeCC->LisLigacaoCC();
   for (int nl = 0; nl < lisLIG_CC->Count; nl++)
      {
      ligCC = (VTLigCC*)lisLIG_CC->Items[nl];
      //verifica o tipo da Ligacao
      switch(ligCC->Ligacao->Tipo())
         {
         case eqptoCHAVE:
         case eqptoTRECHO: //apresenta apenas um valor de corrente
            IniciaLViewLigIcc(ligCC, ligCC->Ligacao->Codigo, ind_bar=0);
            break;
         case eqptoTRAFO: //apresenta valor de corrente no primário e no secundário
            trafo = (VTTrafo*)ligCC->Ligacao;
            //exibe dados no primário
				ind_bar = trafo->IndiceBarra(trafo->BarraPrimario());
            IniciaLViewLigIcc(ligCC, ligCC->Ligacao->Codigo + "(pri)", ind_bar);
            //exibe dados no secundário
            ind_bar = trafo->IndiceBarra(trafo->BarraSecundario());
            IniciaLViewLigIcc(ligCC, ligCC->Ligacao->Codigo + "(sec)", ind_bar);
            break;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelCurto::IniciaLViewLigIcc(VTLigCC *ligCC, AnsiString codigo, int ind_bar)
	{
	//variáveis locais
	int        ind_def;
	TListItem  *Item;
	TListView  *LView =  LViewLigIcc;
	double     fat_conv;

	//determina índice do defeito selecionado
	if ((ind_def = CBoxDefeito->ItemIndex) < 0) return;
	//determina fator de converão
	fat_conv = (CBoxUnidCorrente->ItemIndex == 1) ? 1000. : 1.;
	//insere ListItem no ListView
	Item = LView->Items->Add();
	//associa LigCC ao ListItem
	Item->Data = ligCC->Ligacao;
	//insere tipo da Ligacao
	Item->Caption = ligCC->Ligacao->TipoAsString();
	//insere identificação da Ligacao
	Item->SubItems->Add(codigo);
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
void __fastcall TFormRelCurto::IniciaLViewSupIcc(void)
   {
	//variáveis locais
	int        ind_def;
	double     fat_conv;
	VTBarCC    *barCC;
	TList      *lisBAR_CC;
	TListItem  *Item;
	TListView  *LView = LViewSupIcc;

	//reinicia LView
	LView->Items->Clear();
	//determina índice do defeito selecionado
	if ((ind_def = CBoxDefeito->ItemIndex) < 0) return;
	//determina fator de converão
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
		//insere identificação da Barra
		Item->SubItems->Add(barCC->Barra->Codigo);
		//correntes de fase
		for (int ind_fas = 0; ind_fas < NUM_MAX_FAS; ind_fas++)
			{Item->SubItems->Add(ComplexPolarToStr("%4.3f", barCC->Ifas_amp[ind_def][ind_fas] / fat_conv));}
		//insere tensões de sequencia em pu
		for (int ind_seq = 0; ind_seq < NUM_MAX_FAS; ind_seq++)
			{Item->SubItems->Add(ComplexPolarToStr("%4.3f", barCC->Iseq_amp[ind_def][ind_seq] / fat_conv));}
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormRelCurto::LViewChange(TObject *Sender, TListItem *Item, TItemChange Change)
	{
	//variáveis locais
	VTEqpto *eqpto;
	TListView *LView = (TListView*)Sender;

	//verifica tipo de alteração
	if (Change != ctState ) return;
	//verifica se ListItem indicado é o selecionado
	if (LView->Selected != Item) return;
	//atualiza Moldura com o Eqpto do  ListItem selecionado
	//determina Eqpto associado ao
	Moldura((VTEqpto*)Item->Data);
	}

//---------------------------------------------------------------------------
void __fastcall TFormRelCurto::Moldura(VTEqpto *eqpto)
   {
   //variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	//atualiza Moldura
	grafico->Moldura(eqpto);
	}

//-----------------------------------------------------------------------------
AnsiString __fastcall TFormRelCurto::NomeDefeito(int ind_def)
   {
   //variáveis locais
   char *tipo_def[] = {"3F(abc)", "2F(bc)", "1FT(at)", "1FTz(at)", "2FT(bct)"};

	return(tipo_def[ind_def]);
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelCurto::PageControlChange(TObject *Sender)
	{
	//variáveis locais
	VTEqpto *eqpto = NULL;

	//determina ListView associado ao TabSheet ativo
	if (PageControl->ActivePage == TabSheetPtoDefeito)
		{//Eqpto de defeito: não mostra moldura
		eqpto = NULL;
		}
	else if (PageControl->ActivePage == TabSheetContribuicao)
		{//determina Eqpto associado ao TListItem selecionado
		if (LViewSupIcc->Selected != NULL) eqpto = (VTEqpto*)LViewSupIcc->Selected->Data;
		}
	else if (PageControl->ActivePage == TabSheetTensao)
		{//determina Eqpto associado ao TListItem selecionado
		if (LViewBarVcc->Selected != NULL) eqpto = (VTEqpto*)LViewBarVcc->Selected->Data;
		}
	else if (PageControl->ActivePage == TabSheetCorrente)
		{//determina Eqpto associado ao TListItem selecionado
		if (LViewLigIcc->Selected != NULL) eqpto = (VTEqpto*)LViewLigIcc->Selected->Data;
		}
	//atualiza Moldura
	Moldura(eqpto);
	}

//---------------------------------------------------------------------------
//eof


