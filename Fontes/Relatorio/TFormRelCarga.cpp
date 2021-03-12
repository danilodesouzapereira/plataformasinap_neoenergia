// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TFormRelCarga.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Complexo\Complexo.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
// #include "..\Rede\VTMRede.h"     //DVK 2014.12.10
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
// #include "..\Rede\VTReducao.h"   //DVK 2014.12.10
#include "..\Rede\VTTipoRede.h"
#include "..\Report\VTReport.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Radial.h"
#include "..\..\DLL_Inc\Report.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormRelCarga::TFormRelCarga(TForm *Owner, VTApl *apl_owner, TWinControl *parent)
	: TForm(Owner)
{
	// salva ponteiro p/ objetos
	apl = apl_owner;
	// redefine parent
	if (parent)
		Parent = parent;
	// cria objetos
	lisAUX = new TList();
	lisEQP = new TList();
	lisREDE = new TList();
	lisREDE_MON = new TList();
	// define ordenação do ListView pelo tipo da Rede
	ListViewSort.ColumnIndex = 1;
	ListViewSort.OrdemCrescente = true;
	// inicia o progresso
	progresso = DLL_NewObjProgresso(this, NULL);
	progresso->Start(progDEFAULT);
	// cria objeto Arvore p/ obter hierarquia da rede
	IniciaArvore();
	// inicia CBoxClasses
	IniciaCBoxClasses();
	// inicia ListView
	// LViewInicia();
	// inicia TreeView e SGrid
	TViewInicia();
	TViewChange(NULL, NULL); // DVK 2013.04.02
	SGridInicia();
	// inicia gráfico Chart
	ChartInicia();
	// inicia ListView  //DVK 2016.02.01 inicia após iniciar TView
	LViewInicia();
	// seleciona TabSheet
	PageControl->ActivePage = TabSheetPlanilha;
	PageControlChange(NULL); // DVK 2015.09.11
	// para o progresso
	progresso->Stop();
}

// ---------------------------------------------------------------------------
__fastcall TFormRelCarga::~TFormRelCarga(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// destrói objetos
	if (arvore)
	{
		delete arvore;
		arvore = NULL;
	}
	// destrói objetos
	if (progresso)
	{
		delete progresso;
		progresso = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lisAUX)
	{
		delete lisAUX;
		lisAUX = NULL;
	}
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	if (lisREDE)
	{
		delete lisREDE;
		lisREDE = NULL;
	}
	if (lisREDE_MON)
	{
		delete lisREDE_MON;
		lisREDE_MON = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::ActionExportaExecute(TObject *Sender)
{
	// variáveis locais
	bool sucesso = false;
	VTReport *report;
	AnsiString arq_ext;
	try
	{ // determina o arquivo
		if (!SelecionaArquivo())
			return;
		// cria objeto VTReport
		report = DLL_NewObjReport(apl);
		// imprime o relatório
		arq_ext = ExtractFileExt(SaveDialog->FileName);
		// DVK 2014.12.03 determina aba ativa
		if (PageControl->ActivePage == TabSheetPlanilha)
		{
			if (arq_ext.AnsiCompareIC(".xls") == 0)
			{
				sucesso = report->PlanilhaExcel(SaveDialog->FileName, "Demanda", LView);
			}
			else
			{
				sucesso = report->ArquivoTexto(SaveDialog->FileName, LView);
			}
		}
		else
		{
			if (arq_ext.AnsiCompareIC(".xls") == 0)
			{
				sucesso = report->PlanilhaExcel(SaveDialog->FileName, "Demanda", SGridDemPropria);
			}
			else
			{
				sucesso = report->ArquivoTexto(SaveDialog->FileName, SGridDemPropria);
			}
		}
		// destrói VTReport
		DLL_DeleteObjReport(report);
		if (sucesso)
			Aviso("A tabela foi exportada corretamente.");
		else
			Aviso("Erro ao exportar tabela.");
	}
	catch (Exception &e)
	{
		Aviso("Erro ao exportar tabela.");
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::ActionHelpExecute(TObject *Sender)
{ // variaveis locais
	TForm *formHelp;
	formHelp = DLL_NewFormHelp(this, apl, "Relatórios_Relatórios_de_Demanda");
	if (formHelp)
		formHelp->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::Atualiza(void)
{
	// cria objeto Arvore p/ obter hierarquia da rede
	IniciaArvore();
	// inicia ListView
	LViewInicia();
	// inicia TreeView e SGrid
	TViewInicia();
	SGridInicia();
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormRelCarga::Carga_Modelo(int modelo)
{
	switch (modelo)
	{
	case 1:
		return ("Icte");
	case 2:
		return ("Scte");
	case 3:
		return ("Zcte");
	}
	return ("Inválida");
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::ChartInicia(void)
{
	// variaveis locais
	AnsiString tipo_demanda, cod_rede;
	double y1, y2, y3, max;
	// if(nodeSel->Level < 1) return;
	// preenche título do chart
	if (CBoxRelatorio->ItemIndex == 0)
		tipo_demanda = "própria";
	else
		tipo_demanda = "agregada";
	cod_rede = TView->Selected->Text;
	chart->Title->Caption = "Demanda " + tipo_demanda + " da rede " + cod_rede;
	// reinicia max e series
	max = 0.01;
	Series1->Clear();
	Series2->Clear();
	Series3->Clear();
	for (int n = 0; n < SGridDemPropria->RowCount - 1; n++)
	{ // preenche gráfico
		y1 = StrToDouble(SGridDemPropria->Cells[1][n + 1]);
		Series1->AddXY(n, y1, "", clRed);
		Series1->AddXY(n + 1, y1, "", clRed);
		y2 = StrToDouble(SGridDemPropria->Cells[2][n + 1]);
		Series2->AddXY(n, y2, "", clBlue);
		Series2->AddXY(n + 1, y2, "", clBlue);
		y3 = StrToDouble(SGridDemPropria->Cells[3][n + 1]);
		Series3->AddXY(n, y3, "", clGreen);
		Series3->AddXY(n + 1, y3, "", clGreen);
		max = Max(y1, max);
		max = Max(y2, max);
		max = Max(y3, max);
	}
	//rotulo eixo X
	chart->BottomAxis->Visible = true;
	chart->BottomAxis->Title->Caption = "Patamar (h)";
	// define limites
	// chart->LeftAxis->Minimum = 0.0;
	// chart->LeftAxis->Maximum = 1.2 * max;
}

//---------------------------------------------------------------------------
void __fastcall TFormRelCarga::CBoxClassesChange(TObject *Sender)
{
	SGridInicia();
	TViewChange(NULL, TView->Selected);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::CBoxRelatorioChange(TObject *Sender)
{
	// reinicia somente SGrid e TView
	SGridInicia();
	TViewChange(NULL, TView->Selected);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::CBoxUnidadeChange(TObject *Sender)
{
	// reinicia somente LView
	LViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destrói o Form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	if (geral)
	{ // lê parâmetros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
	// posiciona o Form canto superior esquerdo
	Top = 0;
	Left = 0; ;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::FormShow(TObject *Sender)
{
	// posiciona o Form
	FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::IniciaArvore(void)
{
	// destrói eventual objeto criado anteriormente
	if (arvore)
	{
		delete arvore;
	}
	// cria novo objeto Arvore
	arvore = DLL_NewObjArvore(apl);
	arvore->Inicia((VTRedes*)apl->GetObject(__classid(VTRedes)));
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::IniciaCBoxClasses(void)
{
	// variaveis locais
	VTClasse *classe;
	VTClasses *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
	TList *lisCLASSE;
	lisCLASSE = classes->LisClasse();
	// CBoxClasses->Items->Add("Todas");
	for (int nc = 0; nc < lisCLASSE->Count; nc++)
	{ // lista todas as classes
		classe = (VTClasse*)lisCLASSE->Items[nc];
		CBoxClasses->Items->Add(classe->Codigo);
	}
}

/*
 //---------------------------------------------------------------------------
 int __fastcall TFormRelCarga::IniciaLisCarga(TList *lisREDE, TList *lisCARGA)
 {
 //variáveis locais
 VTRede    *rede;
 //	VTReducao *reducao;

 //reinicia lisCARGA
 lisCARGA->Clear();
 //determina Cargas de todas Redes
 for (int n = 0; n < lisREDE->Count; n++)
 {
 rede = (VTRede*)lisREDE->Items[n];
 //verifica se Rede carregada   //DVK 2014.12.10
 if (! rede->Carregada) continue;
 //			{
 //			for(int nrd = 0; nrd < rede->MRede->LisReducao()->Count; nrd++)
 //				{
 //				reducao = (VTReducao*)rede->MRede->LisReducao()->Items[nrd];
 //				//obtém lista de Cargas da Reducao  //DVK 2014.12.10
 //				reducao->LisEqbar(lisCARGA, eqptoCARGA);
 //				}
 //			}
 else
 {//obtém lista de Cargas da Rede
 rede->LisEqbar(lisCARGA, eqptoCARGA);
 }
 }
 return(lisCARGA->Count);
 }
 */ // DVK 2016.01.29 filtra cargas em barras de fronteira
// ---------------------------------------------------------------------------
int __fastcall TFormRelCarga::IniciaLisCarga(TList *lisREDE, TList *lisCARGA)
{
	// variáveis locais
	VTCarga *carga;
	VTRede *rede, *rede_mon;
	TList *lisCARGA_MON = new TList();
	// reinicia lisCARGA
	lisCARGA->Clear();
	lisCARGA_MON->Clear();
	// determina Cargas de todas Redes a montante
	for (int n = 0; n < lisREDE_MON->Count; n++)
	{
		rede_mon = (VTRede*)lisREDE_MON->Items[n];
		// verifica se Rede carregada
		if (!rede_mon->Carregada)
			continue;
		// obtém lista de Cargas da Rede
		rede_mon->LisEqbar(lisCARGA_MON, eqptoCARGA);
	}
	// determina Cargas de todas Redes da lista lisREDE
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		// verifica se Rede carregada
		if (!rede->Carregada)
			continue;
		// obtém lista de Cargas da Rede
		rede->LisEqbar(lisCARGA, eqptoCARGA);
		// verifica se a rede não faz parte da lista de redes a montante
		if (lisREDE_MON->IndexOf(rede) >= 0)
			continue;
		// verifica se a lista lisCARGA_MON está vazia
		if (lisCARGA_MON->Count == 0)
			continue;
		// não faz parte: verifica se há cargas em comum nas listas lisCARGA e lisCARGA_MON
		for (int nc = lisCARGA->Count - 1; nc >= 0; nc--)
		{
			carga = (VTCarga*)lisCARGA->Items[nc];
			if (lisCARGA_MON->IndexOf(carga) < 0)
				continue;
			// retira carga de lisCARGA
			lisCARGA->Remove(carga);
		}
	}
	// destroi lista
	delete lisCARGA_MON;
	return (lisCARGA->Count);
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::LViewColumnClick(TObject *Sender, TListColumn *Column)
{
	// verifica se selecionou a mesma coluna
	if (ListViewSort.ColumnIndex == Column->Index)
	{ // inverte ordem
		ListViewSort.OrdemCrescente = !ListViewSort.OrdemCrescente;
	}
	else
	{ // redefine índice da coluna usada p/ ordenação
		ListViewSort.ColumnIndex = Column->Index;
	}
	// força reordenação
	LView->AlphaSort();
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::LViewCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
	int Data, int &Compare)
{
	// variáveis locais
	int index = ListViewSort.ColumnIndex - 1;
	double valor1, valor2;
	// proteção
	if ((index < 0) || (index >= LView->Columns->Count))
		index = 0;
	switch (ListViewSort.ColumnIndex)
	{
	case 0: // tipo da rede
		Compare = CompareText(Item1->Caption, Item2->Caption);
		break;
	case 1: // código da rede
		Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
		break;
	default: // valor de demanda
		valor1 = StrToDouble(Item1->SubItems->Strings[index]);
		valor2 = StrToDouble(Item2->SubItems->Strings[index]);
		if (valor1 < valor2)
			Compare = -1;
		else if (valor1 > valor2)
			Compare = 1;
		else
			Compare = 0;
		break;
	}
	// verifica se ordem crescente/decrescente
	if (!ListViewSort.OrdemCrescente)
		Compare = -Compare;
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormRelCarga::LViewInicia(void)
 {
 //variáveis locais
 double          valor;
 complex<double> s_kva;
 TListItem       *Item;
 VTRede          *rede;
 //	VTReducao 		 *reducao;
 VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
 VTRedes         *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
 TList           *lisREDE   = redes->LisRede();
 int             num_pat    = patamares->NumPatamar();

 //reinicia LView e s_kva
 LView->Clear();
 s_kva = CZero();
 //atualiza Caption das colunas dos patamares
 for (int nc = 2, ind_pat = 0; (nc < LView->Columns->Count)&&(ind_pat < num_pat); nc++, ind_pat++)
 {
 LView->Columns->Items[nc]->Caption = patamares->Patamar[ind_pat]->Nome;
 }
 //proteção
 if (CBoxUnidade->ItemIndex < 0)
 {
 Aviso("Defina unidade dos valores a serem mostrados");
 return;
 }
 //loop p/ todas Redes
 for (int nr = 0; nr < lisREDE->Count; nr++)
 {
 rede = (VTRede*)lisREDE->Items[nr];
 //verifica se Rede carregada
 if (! rede->Carregada) continue;  //DVK 2014.12.10
 //			{
 //			for(int nrd = 0; nrd < rede->MRede->LisReducao()->Count; nrd++)
 //				{
 //				reducao = (VTReducao*)rede->MRede->LisReducao()->Items[nrd];
 //				//obtém lista de Cargas da Reducao //DVK 2014.12.10
 //				lisEQP->Clear();
 //				reducao->LisEqbar(lisEQP, eqptoCARGA);
 //				}
 //			}
 else
 {//obtém lista de Cargas da Rede
 lisEQP->Clear();
 rede->LisEqbar(lisEQP, eqptoCARGA);
 }
 //insere ListItem
 Item = LView->Items->Add();
 Item->Caption = rede->TipoRede->Codigo;
 Item->SubItems->Add(rede->Codigo);
 //calcula somatória de demanda das Cargas em cada patamar
 for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
 {
 SomaDemandaCarga(lisEQP, ind_pat, s_kva);
 switch(CBoxUnidade->ItemIndex)
 {
 case 0:  valor = Abs(s_kva);   break;
 case 1:  valor = s_kva.real(); break;
 case 2:  valor = s_kva.imag(); break;
 default: continue;
 }
 Item->SubItems->Add(DoubleToStr("%5.3f", valor));
 }
 }
 }
 */
// ---------------------------------------------------------------------------
// void __fastcall TFormRelCarga::LViewInicia(void)
// {
// //variáveis locais
// double          valor;
// complex<double> s_kva;
// TListItem       *Item;
// VTNode          *node;
// VTRede          *rede;
// VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
// TList           *lisNODE;
// int             num_pat    = patamares->NumPatamar();
//
// //reinicia LView e s_kva
// LView->Clear();
// s_kva = CZero();
// //atualiza Caption das colunas dos patamares
// for (int nc = 2, ind_pat = 0; (nc < LView->Columns->Count)&&(ind_pat < num_pat); nc++, ind_pat++)
// {
// LView->Columns->Items[nc]->Caption = patamares->Patamar[ind_pat]->Nome;
// }
// //proteção
// if (CBoxUnidade->ItemIndex < 0)
// {
// Aviso("Defina unidade dos valores a serem mostrados");
// return;
// }
// //cria lista
// lisNODE   = new TList();
// //loop p/ todas Redes a partir da árvore de redes
// arvore->LisAllNode(lisNODE);
// for (int n = 0; n < lisNODE->Count; n++)
// {
// node = (VTNode*)lisNODE->Items[n];
// //reinicia lisREDE e lisREDE_MON
// lisREDE->Clear();
// lisREDE_MON->Clear();
// //preenche listas
// node->LisObjectNode(lisREDE);
// if(node->Pai) node->Pai->LisObjectNode(lisREDE_MON);
// //obtém lista de Cargas da Rede
// lisEQP->Clear();
// IniciaLisCarga(lisREDE, lisEQP);
// for(int nr = 0; nr < lisREDE->Count; nr++)
// {
// rede = (VTRede*)lisREDE->Items[nr];
// //verifica se Rede carregada
// if (! rede->Carregada) continue;
// //insere ListItem
// Item = LView->Items->Add();
// Item->Caption = rede->TipoRede->Codigo;
// Item->SubItems->Add(rede->Codigo);
// //calcula somatória de demanda das Cargas em cada patamar
// for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
// {
// SomaDemandaCarga(lisEQP, ind_pat, s_kva);
// switch(CBoxUnidade->ItemIndex)
// {
// case 0:  valor = Abs(s_kva);   break;
// case 1:  valor = s_kva.real(); break;
// case 2:  valor = s_kva.imag(); break;
// default: continue;
// }
// Item->SubItems->Add(DoubleToStr("%5.3f", valor));
// }
// }
// }
// //destroi lista
// delete lisNODE;
// }
// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::LViewInicia(void)
{
	// variáveis locais
	double valor;
	complex<double>s_kva;
	TListItem *Item;
	VTNode *node;
	VTRede *rede;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	TList *lisNODE;
	int num_pat = patamares->NumPatamar();

	// FKM 2016.02.25
	LView->Items->BeginUpdate();
	// reinicia LView e s_kva
	LView->Clear();
	s_kva = CZero();
	// atualiza Caption das colunas dos patamares
	for (int nc = 2, ind_pat = 0; (nc < LView->Columns->Count) && (ind_pat < num_pat);
	nc++, ind_pat++)
	{
		LView->Columns->Items[nc]->Caption = patamares->Patamar[ind_pat]->Nome;
	}
	// proteção
	if (CBoxUnidade->ItemIndex < 0)
	{
		Aviso("Defina unidade dos valores a serem mostrados");
		return;
	}
	// cria lista
	lisNODE = new TList();
	// loop p/ todas Redes a partir da árvore de redes
	arvore->LisAllNode(lisNODE);
	for (int n = 0; n < lisNODE->Count; n++)
	{
		node = (VTNode*)lisNODE->Items[n];
		// reinicia lisREDE e lisREDE_MON
		lisREDE->Clear();
		lisREDE_MON->Clear();
		// preenche listas
		node->LisObjectNode(lisREDE);
		if (node->Pai)
			node->Pai->LisObjectNode(lisREDE_MON);
		// obtém lista de Cargas da Rede
		lisEQP->Clear();
		IniciaLisCarga(lisREDE, lisEQP);
		for (int nr = 0; nr < lisREDE->Count; nr++)
		{
			rede = (VTRede*)lisREDE->Items[nr];
			////adaptação FKM 2016.02.25
			if (node->Codigo.AnsiCompare("Redes Isoladas") == 0)
			{
				// obtém lista de Cargas da Rede
				lisEQP->Clear();
				rede->LisEqbar(lisEQP, eqptoCARGA);
			}
			// \\adaptação FKM 2016.02.25
			// verifica se Rede carregada
			if (!rede->Carregada)
				continue;
			// insere ListItem
			Item = LView->Items->Add();
			Item->Caption = rede->TipoRede->Codigo;
			Item->SubItems->Add(rede->Codigo);
			// calcula somatória de demanda das Cargas em cada patamar
			for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
			{
				SomaDemandaCarga(lisEQP, ind_pat, s_kva);
				switch (CBoxUnidade->ItemIndex)
				{
				case 0:
					valor = Abs(s_kva);
					break;
				case 1:
					valor = s_kva.real();
					break;
				case 2:
					valor = s_kva.imag();
					break;
				default:
					continue;
				}
				Item->SubItems->Add(DoubleToStr("%5.3f", valor));
			}
		}
	}
	// FKM 2016.02.25
	LView->Items->EndUpdate();
	// destroi lista
	delete lisNODE;
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TFormRelCarga::PageControlChange(TObject *Sender)
 {//na aba com o gráfico, esconde painel Selecionar Unidade
 if(PageControl->TabIndex == 1)
 {
 PanelUnidade->Enabled = false;
 CBoxUnidade->Color = clInactiveCaptionText;
 }
 else
 {
 PanelUnidade->Enabled = true;
 CBoxUnidade->Color = clWindow;
 }
 }
 */  // DVK 2015.09.11 alterado
// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::PageControlChange(TObject *Sender)
{
	progresso->Start(progDEFAULT);
	if (PageControl->ActivePage == TabSheetGrafico)
	{ // na aba com o gráfico, bloqueia painel Selecionar Unidade
		PanelUnidade->Enabled = false;
		CBoxUnidade->Color = clInactiveCaptionText;
		// desbloqueia painel Tipo de Relatorio e mostra unidade S
		PanelTipoRel->Enabled = true;
		CBoxRelatorio->Color = clWindow;
		CBoxUnidade->ItemIndex = 0;
		CBoxRelatorioChange(NULL);
		// teste
		// LView->Visible = false;
		// LView->Parent = NULL;
	}
	else
	{ // teste
		// LView->Visible = true;
		// LView->Parent = TabSheetPlanilha;
		// desbloqueia painel Selecionar Unidade
		PanelUnidade->Enabled = true;
		CBoxUnidade->Color = clWindow;
		// bloqueia painel Tipo de Relatorio e mostra demanda agregada
		PanelTipoRel->Enabled = false;
		CBoxRelatorio->Color = clInactiveCaptionText;
		CBoxRelatorio->ItemIndex = 0;
		CBoxUnidadeChange(NULL);
	}
	progresso->Stop();
}

// ---------------------------------------------------------------------------
bool __fastcall TFormRelCarga::SelecionaArquivo(void)
{
	// variáveis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	// verifica se o diretório SaveDialog já foi definido
	if (SaveDialog->InitialDir.IsEmpty())
		SaveDialog->InitialDir = path->DirExporta();
	return (SaveDialog->Execute());
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::SGridInicia(void)
{
	// variáveis locais
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	complex<double>s_kva;
	int num_pat = patamares->NumPatamar();
	// limpa lisEQP e zera valor total
	s_kva = CZero();
	// preenche unidades
	SGridDemPropria->Cells[1][0] = "S[kVA]";
	SGridDemPropria->Cells[2][0] = "P[kW]";
	SGridDemPropria->Cells[3][0] = "Q[kVAr]";
	// define núm. linhas conforme patamares
	SGridDemPropria->RowCount = num_pat + 1;
	for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
	{ // preenche patamares
		SGridDemPropria->Cells[0][ind_pat + 1] = patamares->Patamar[ind_pat]->Periodo;
		// preenche valores
		SomaDemandaCarga(lisEQP, ind_pat, s_kva);
		// preenche linha da SGrid com demanda própria
		SGridDemPropria->Cells[1][ind_pat + 1] = DoubleToStr("%5.2f", Abs(s_kva), ',');
		SGridDemPropria->Cells[2][ind_pat + 1] = DoubleToStr("%5.2f", s_kva.real(), ',');
		SGridDemPropria->Cells[3][ind_pat + 1] = DoubleToStr("%5.2f", s_kva.imag(), ',');
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::SomaDemandaCarga(TList *lisCAR, int ind_pat, complex<double> &s_kva)
{
	// variáveis locais
	complex<double>s_mva_total;
	VTCarga *carga;
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	// zera valor total
	s_mva_total = CZero();
	// loop p/ todas Cargas
	for (int nc = 0; nc < lisCAR->Count; nc++)
	{
		carga = (VTCarga*)lisCAR->Items[nc];
		// filtra carga pela classe selecionada em CBoxClasses
		if (CBoxClasses->ItemIndex != 0)
		{
			if (CBoxClasses->Text.CompareIC(carga->classe->Codigo) != 0)
				continue;
		}
		s_mva_total += demanda->S_mva[carga][ind_pat];
	}
	// atualiza valor de retorno
	s_kva = s_mva_total * 1000.;
}

// ---------------------------------------------------------------------------
// void __fastcall TFormRelCarga::TViewChange(TObject *Sender, TTreeNode *treenode)
// {
// //variáveis locais
// AnsiString class_name;
// VTNode     *node;
// VTRede     *rede;
//
// //reinicia lista de Redes e de Cargas
// lisREDE->Clear();
// lisREDE_MON->Clear();
// lisEQP->Clear();
// //verifica se selecionou um TreeNode
// if ((treenode != NULL)&&(treenode->Data != NULL))
// {
// class_name = ((TObject*)(treenode->Data))->ClassName();
// if (class_name.AnsiCompareIC("TArvore") == 0)
// {//verifica opção de cargas agregadas
// if (CBoxRelatorio->ItemIndex == 1)
// {//obtém lista de todas redes da Arvore
// arvore->LisAllObject(lisREDE);
// }
// }
// else if (class_name.AnsiCompareIC("TNode") == 0)
// {//obtém lista completa de Redes do Node
// node = (VTNode*)treenode->Data;
// node->LisObjectNode(lisREDE);
// node->LisObjectMontante(lisREDE_MON);   //DVK 2016.01.29
// //verifica opção de carga agregada
// if (CBoxRelatorio->ItemIndex == 1)
// {//obtém lista de Redes a jusante
// node->LisObjectJusante(lisREDE);
// }
// }
// else if (class_name.AnsiCompareIC("TRede") == 0)
// {//determina Rede associada ao TreeNode
// rede = (VTRede*)treenode->Data;
// //verifica opção de carga própria ou agregada
// if (CBoxRelatorio->ItemIndex == 0)
// {//insere apenas a Rede lisREDE
// lisREDE->Add(rede);
// }
// else
// {//obtém Node da Rede
// if ((node = arvore->ExisteNode(rede)) != NULL)
// {//obtém lista de Redes do Node
// node->LisObjectNode(lisREDE);
// node->LisObjectMontante(lisREDE_MON);   //DVK 2016.01.29
// //verifica opção de carga agregada
// if (CBoxRelatorio->ItemIndex == 1)
// {//obtém lista de Redes a jusante do Node
// node->LisObjectJusante(lisREDE);
// }
// }
// }
// }
// }
// //determina Cargas das Redes selecionadas
// IniciaLisCarga(lisREDE, lisEQP);
// //atualiza SGrid
// SGridInicia();
// //atualiza chart
// ChartInicia();
// }
// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::TViewChange(TObject *Sender, TTreeNode *treenode)
{
	// variáveis locais
	AnsiString class_name;
	VTNode *node;
	VTNode *node_child;
	VTRede *rede;
	TList *lisNodes, *lisObjNode;
	bool aguarde = false;

	// proteção
	if (treenode == NULL)
		return;
	// se necessário, insere nodes filhos
	TViewInsereNodesChild(treenode);
	// reinicia lista de Redes e de Cargas
	lisREDE->Clear();
	lisREDE_MON->Clear();
	lisEQP->Clear();
	// verifica que objeto tem no TreeNode
	if ((treenode->Data != NULL))
	{
		class_name = ((TObject*)(treenode->Data))->ClassName();
		if (class_name.AnsiCompareIC("TArvore") == 0)
		{ // verifica opção de cargas agregadas
			if (CBoxRelatorio->ItemIndex == 1)
			{ // obtém lista de todas redes da Arvore
				arvore->LisAllObject(lisREDE);
			}
		}
		else if (class_name.AnsiCompareIC("TNode") == 0)
		{ // obtém lista completa de Redes do Node
			node = (VTNode*)treenode->Data;
			node->LisObjectNode(lisREDE);
			node->LisObjectMontante(lisREDE_MON); // DVK 2016.01.29
			// verifica opção de carga agregada
			if (CBoxRelatorio->ItemIndex == 1)
			{ // obtém lista de Redes a jusante
				node->LisObjectJusante(lisREDE);
				// Para evitar que o aguarde seja mostrado quando o cálculo é rápido
                // Coloquei um valor de 100 cargas na rede para ele mostrar o AGUARDE
				if(lisREDE->Count > 100){aguarde = true;}
			}
		}
		else if (class_name.AnsiCompareIC("TList") == 0)
		{ // obtém lista completa de Redes do grupo
			lisObjNode = (TList*)treenode->Data;
			for (int nn = 0; nn < lisObjNode->Count; nn++)
			{ // determina cada rede que compõe o grupo
				rede = (VTRede*)lisObjNode->Items[nn];
				// verifica opção de carga própria ou agregada
				if (CBoxRelatorio->ItemIndex == 0)
				{ // insere apenas a Rede em lisREDE
					lisREDE->Add(rede);
				}
				else
				{ // obtém Node da Rede
					if ((node = arvore->ExisteNode(rede)) != NULL)
					{ // obtém lista de Redes do Node
						node->LisObjectNode(lisREDE);
						node->LisObjectMontante(lisREDE_MON); // DVK 2016.01.29
						// verifica opção de carga agregada
						if (CBoxRelatorio->ItemIndex == 1)
						{ // obtém lista de Redes a jusante do Node
							node->LisObjectJusante(lisREDE);
						}
					}
				}
			}
		}

		else if (class_name.AnsiCompareIC("TRede") == 0)
		{ // determina Rede associada ao TreeNode
			rede = (VTRede*)treenode->Data;
			// verifica opção de carga própria ou agregada
			if (CBoxRelatorio->ItemIndex == 0)
			{ // insere apenas a Rede em lisREDE
				lisREDE->Add(rede);
			}
			else
			{ // obtém Node da Rede
				if ((node = arvore->ExisteNode(rede)) != NULL)
				{ // obtém lista de Redes do Node
					node->LisObjectNode(lisREDE);
					node->LisObjectMontante(lisREDE_MON); // DVK 2016.01.29
					// verifica opção de carga agregada
					if (CBoxRelatorio->ItemIndex == 1)
					{ // obtém lista de Redes a jusante do Node
						node->LisObjectJusante(lisREDE);
					}
				}
			}
		}
	}
	// determina Cargas das Redes selecionadas
	IniciaLisCarga(lisREDE, lisEQP);
	if(aguarde){progresso->Start(progDEFAULT);}
	// atualiza SGrid
	SGridInicia();
	// atualiza chart
	ChartInicia();
	if(aguarde){progresso->Stop();}
}

// ---------------------------------------------------------------------------
// void __fastcall TFormRelCarga::TViewInicia(void)
// {
// //desabilita tratamento do evento OnChange
// TView->OnChange = NULL;
// //inicia TreeView com as redes organizadas por Hierarquia
// arvore->TreeViewInicia(TView);
// //pré-seleciona e expande primeiro TreeNode
// if (TView->Items->Count > 0)
// {
// TView->Selected = TView->Items->Item[0];
// if (TView->Selected) TView->Selected->Expand(false);
// }
// //habilita tratamento do evento OnChange
// TView->OnChange = TViewChange;
// }
// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::TViewInicia(void)
{
	// variaveis locais
	TList *lisNodeRaiz, *lisObjNode;
	TTreeNode *treenode;
	TTreeNode *treenodeChild;
	VTNode *node;
	VTRede *rede;
	AnsiString class_name;

	// desabilita tratamento do evento OnChange
	TView->OnChange = NULL;
	// inicia TreeView com as redes organizadas por Hierarquia
	// arvore->TreeViewInicia(TView);

	lisNodeRaiz = arvore->LisNodeRaiz();
	TView->Items->Clear();
	for (int nn = 0; nn < lisNodeRaiz->Count; nn++)
	{
		node = (VTNode*) lisNodeRaiz->Items[nn];
		treenode = TView->Items->AddChildObject(NULL, node->Codigo, node);
		lisObjNode = node->LisObjectNode();
		// FKM 2016.04.05- adaptação específica pro Redes Isoladas...
		if (node->Codigo.AnsiCompare("Redes Isoladas") == 0)
		{
			for (int nr = 0; nr < lisObjNode->Count; nr++)
			{
				class_name = ((TObject*)lisObjNode->Items[nr])->ClassName();
				if (class_name.AnsiCompare("TRede") == 0)
				{
					rede = (VTRede*)lisObjNode->Items[nr];
					treenodeChild = TView->Items->AddChildObject(treenode, rede->Codigo, rede);
				}
			}
		}
	}
	TView->Selected = treenode;
	// //pré-seleciona e expande primeiro TreeNode
	// if (TView->Items->Count > 0)
	// {
	// TView->Selected = TView->Items->Item[0];
	// if (TView->Selected) TView->Selected->Expand(false);
	// }

	// habilita tratamento do evento OnChange
	TView->OnChange = TViewChange;
}

// ---------------------------------------------------------------------------
void __fastcall TFormRelCarga::TViewInsereNodesChild(TTreeNode *treenode)
{
	// variaveis locais
	AnsiString class_name;
	TList *lisObjNode, *lisNodes;
	VTNode *node, *node_child;
	VTRede *rede;

	if (treenode != NULL)
	{ // verifica se tem TNode ou TList
		class_name = ((TObject*)(treenode->Data))->ClassName();
		if (class_name.AnsiCompareIC("TNode") == 0)
		{
			node = (VTNode*)treenode->Data;
			lisObjNode = NULL;
		}
		else if (class_name.AnsiCompareIC("TList") == 0)
		{
			lisObjNode = (TList*)treenode->Data;
			node = NULL;
		}
		else
		{
			return;
		}
		// verifica se há grupo de redes
		if ((lisObjNode != NULL) && (treenode->Count == 0))
		{
			for (int nn = 0; nn < lisObjNode->Count; nn++)
			{
				class_name = ((TObject*)(lisObjNode->Items[nn]))->ClassName();
				if (class_name.AnsiCompareIC("TRede") == 0)
				{ // insere nodes de cada rede que compõe o grupo
					rede = (VTRede*)lisObjNode->Items[nn];
					TView->Items->AddChildObject(treenode, rede->Codigo, rede);
				}
			}
		}
		// verifica se é só um Node
		else if ((node != NULL) && (treenode->Count == 0))
		{
			node = (VTNode*)treenode->Data;
			// verifica se é um grupo de redes (2 ou +)
			lisObjNode = node->LisObjectNode();
			if (lisObjNode->Count > 1)
			{ // insere treenode filho com redes do grupo
				TView->Items->AddChildObject(treenode, "<Redes do grupo>", lisObjNode);
			}
			lisNodes = node->LisNode();
			for (int nn = 0; nn < lisNodes->Count; nn++)
			{ // insere treenodes filhos com nodes
				node_child = (VTNode*)lisNodes->Items[nn];
				TView->Items->AddChildObject(treenode, node_child->Codigo, node_child);
			}
		}
	}
}

// ---------------------------------------------------------------------------
// eof



