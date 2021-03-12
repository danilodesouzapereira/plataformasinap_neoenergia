// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <Math.hpp>
#include "TFormMercado.h"
#include "TFormEdtTaxa.h"
#include "TImportaMercado.h"
#include "..\Apl\VTApl.h"
#include "..\BD\VTBD.h"
#include "..\BD\VTSNPDBConn.h"
#include "..\BD\Rede\VTBDgeral.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
// #include "..\Cronometro\VTCronometro.h"   //added
#include "..\Diretorio\VTPath.h"
#include "..\Forms\TFormEdtTxt.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Mapa\VTRegiao.h"
#include "..\Mapa\VTRegioes.h"
#include "..\Mercado\VTMercado.h"
#include "..\Mercado\VTSegmento.h"
#include "..\Mercado\VTCrescimento.h"
#include "..\Mercado\TFormEdtRegiao.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h" //FKM 2016.06.27
#include "..\Redegraf\VTRedegraf.h" //FKM 2017.04.19
#include "..\Sinap\VTSinapChild.h"
#include "..\..\DLL_Inc\BD.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Radial.h"
#include "..\..\DLL_Inc\Help.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#tasks/14893054
int __fastcall CompareCodigoClasse(void *Item1, void *Item2)
{
	VTCrescimento *item1, *item2;

	item1 = (VTCrescimento*)Item1;
	item2 = (VTCrescimento*)Item2;

	return (item1->Codigo.AnsiCompare(item2->Codigo)); // descending
}

// ---------------------------------------------------------------------------
__fastcall TFormMercado::TFormMercado(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
	: TFormEdt(Owner, parent)
{
	VTRedegraf *redegraf;

	// salva ponteiro p/ objeto
	this->apl = apl_owner;
	mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
	redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
	// cria objeto p/ acessar base de dados
	BaseDadoConecta();
	// cria Series em Chart
	// ChartCriaSeries();
	// cria objetos
	lisCRE = new TList();
	lisREGIAO_pre = new TList();
	lisStrArvore = new TList();
	// salva a lista de regioes para comparação posterior
	RegistraRegioesPreExistentes();
	// //iniica CBoxAnoInicial e CBoxAnoFinal
	// CBoxAnoInicia();
	// //se necessário, cria Segmento default
	// CriaSegmentoDefault();
	// //cria Segmento p/ TipoRede e Rede que ainda não tem Segmento associada
	// CriaSegmentoParaRedes();
	// //cria Segmento p/ Regiao que ainda não tem Segmento associada
	// CriaSegmentoParaRegioes();
	// Cria Arvore de redes
	CriaSegmentoParaRedes();
	arvore = DLL_NewObjArvore(apl);
	// reinicia arvore de redes
	arvore->Clear();
	arvore->Inicia(redes);
	CriaArvoreRedes();
	//
	AtualizaForm();
	// salva o modo grafico atual
	if (redegraf)
	{
		modoGraficoAnterior = redegraf->ModoGrafico;
	}

}

// ---------------------------------------------------------------------------
__fastcall TFormMercado::~TFormMercado(void)
{ // FKM 2015.10.13 Seguindo o padrão do formIniciaPosicao \\
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// FKM 2015.10.13 //
	// desconecta a base de dados
	BaseDadoDesconecta();
	// destrói lista sem destruir seus objetos
	if (lisCRE != NULL)
	{
		delete lisCRE;
		lisCRE = NULL;
	}
	if (lisREGIAO_pre != NULL)
	{
		delete lisREGIAO_pre;
		lisREGIAO_pre = NULL;
	}
	// retorna o modo grafico
	RetornaModoGrafico();
	// destroi form
	if (formEdtRegiao != NULL)
	{
		delete formEdtRegiao;
		formEdtRegiao = NULL;
	}
	if (lisStrArvore)
	{
		LimpaTList(lisStrArvore);
		delete lisStrArvore;
		lisStrArvore = NULL;
	}
	if (arvore)
	{
		arvore->Clear();
		delete arvore;
		arvore = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::ActionAddRegiaoExecute(TObject *Sender)
{
	VTRegioes *regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes));

	if (formEdtRegiao == NULL)
	{
		// formEdtRegiao = new TFormEdtRegiao(apl,this, NULL);
		formEdtRegiao = new TFormEdtRegiao(this, apl, NULL, ActionRegiaoEditado,
			regioes->LisRegiao());
	}
	formEdtRegiao->Show();
	this->Hide();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::ActionCancelaExecute(TObject *Sender)
{
	// verifica se foram feitas alterações
	if (ExisteAlteracao())
	{ // pede confirmação do usuário
		if (Confirma("As alterações efetuadas serão perdidas.", "Deseja descartar as alterações ?")
			!= IDYES)
			return;
		{ // reinicia Mercado a partir da base de dados
			mercado->Reinicia();
			// lê dados da base
			// OOPS: Modificado em 20120611
			/*
			 if      (bd)         bd->Geral->LeMercado();
			 else if (bd_mercado) bd_mercado->Le();
			 */
			// Eu acho que não há necessidade, uma vez que o mercado é lido
			// ao carregar o BD.
			if (bd)
				bd->Geral->LeMercado();
			else
				return;
			// atualiza crescimentos das Cargas
			mercado->IniciaRedes(redes);
		}
	}
	// fecha o form
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::ActionConfirmaExecute(TObject *Sender)
{
	// variáveis locais
	bool existe_alteracao;
	// VTCronometro *timer;
	VTSinapChild *sinapChild;

	// para mudar o uptodate
	sinapChild = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
	// timer = DLL_NewObjCronometro(apl);
	// verifica se foi feita alguma alteração
	// existe_alteracao = ExisteAlteracao();
	existe_alteracao = (ExisteAlteracao() || ExisteAlteracaoEmRegioes());
	// elimina Regioes sem crescimento
	RetiraDaBaseSegmentoSemCrescimento();
	// elimina Regioes c/ crescimento nulo
	// RetiraDaBaseSegmentoComCrescimentoNulo();
	// verifica se foram feitas alterações
	if (existe_alteracao)
	{ // atualiza base de dados
		// OOPS: Modificado em 20120611
		/*
		 if      (bd)         bd->Geral->SalvaMercado();
		 else if (bd_mercado) bd_mercado->Grava();
		 */
		// OOPS: Deveria logar alguma coisa aqui, não??
		if (bd)
			bd->Geral->SalvaMercado();
		else
			return;

		// timer->Reinicia();
		// timer->Start("IniciaRedes");
		// atualiza crescimentos das Cargas
		mercado->IniciaRedes(redes);
		// timer->Stop("IniciaRedes");
		// timer->Show();
		// FKM - avisa o SinapChild da mundaça do mercado
		sinapChild->ActionMercadoChanged->Execute();
	}
	// fecha o form
	Close();

	// delete timer;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::ActionEditaSegmentoExecute(TObject *Sender)
{
	// variáveis locais
	TTreeNode *node_segmento, *node_pai;
	VTSegmento *segmento;
	TFormEdtTaxa *FormEdtTaxa;

	// determina TreeNode c/ Segmento
	node_segmento = TreeView->Selected;
	while (node_segmento->Level > 1)
	{
		node_segmento = node_segmento->Parent;
	}
	// verifica se há uma Segmento selecionada
	if ((segmento = ExisteSegmentoSelecionada(node_segmento)) == NULL)
		return;
	try
	{ // cria FormEdtTaxa p/ edição do Crescimento
		FormEdtTaxa = new TFormEdtTaxa(this, mercado, segmento);
		// exibe FormEdtTaxa como janela modal
		if (FormEdtTaxa->ShowModal() == mrOk)
		{ // Segmento foi alterada
			segmento->Alterada = true;
			segmento->Enabled = true;
			// atualiza TreeView                     /*DVK 2012.10.09 inicio - tirei comentários*/
			node_pai = node_segmento->Parent;
			TreeView->Items->Delete(node_segmento);
			node_segmento = TreeViewInsereSegmento(node_pai, segmento);
			node_pai->AlphaSort(false); /* DVK 2012.10.09 fim */
			// atualiza ListView e TChart
			TreeViewChange(TreeView, node_segmento);
		}
		// destrói FormEdtTaxa
		delete FormEdtTaxa;
	}
	catch (Exception &e)
	{
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::ActionHelpExecute(TObject *Sender)
{ // variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Mercado");
	if (formHelp)
		formHelp->Show();
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15166963
//https://sinapsisenergia.teamwork.com/#/tasks/17789343
void __fastcall TFormMercado::ActionImportaMercadoExecute(TObject *Sender)
{
    // variáveis locais
    TImportaMercado *importa;
    VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
    AnsiString arq_mercado;
    VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
    VTLicenca *licenca = licencas->ExisteLicenca();
    bool sucesso = false;

    // configura diretório inicial do openDialog
    OpenDialog->InitialDir = path->DirImporta();
    if (OpenDialog->Execute())
    {//importa taxas de crescimento por alimentador (Copel)
        arq_mercado = OpenDialog->FileName;
        importa = new TImportaMercado(apl);
        //verifica se é copel
        if((licenca != NULL) && (licenca->Empresa == COPEL))
        {
            sucesso = importa->Executa(arq_mercado);
        }
        else
        {
            sucesso = importa->ExecutaV2(arq_mercado);
        }
        if(sucesso)
        {
            Aviso("Taxas de crescimento importadas com sucesso !");
        }
        delete importa;
        AtualizaForm();
    }

}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::ActionInsereSegmentoExecute(TObject *Sender)
{
	//
}

/* DVK 2012.10.09 inicio */
// ---------------------------------------------------------------------------
void __fastcall TFormMercado::ActionReiniciaTaxasExecute(TObject *Sender)
{
	// variáveis locais
	VTSegmento *segmento;
	TList *lisSEGMENTO = mercado->LisSegmento();

	// pede confirmação do usuário
	if (Confirma("Todas as taxas de crescimento serão excluídas.", "Confirma?") != IDYES)
		return;

	// loop p/ todas Regioes
	for (int nr = lisSEGMENTO->Count - 1; nr >= 0; nr--)
	{
		segmento = (VTSegmento*)lisSEGMENTO->Items[nr];
		// reinicia todas as taxas de crescimento
		segmento->IniciaCrescimentoNulo();
		// não elimina Segmento Default
		if (segmento->Tipo != smDEFAULT)
		{
			segmento->Enabled = false;
		}
		segmento->Alterada = true;
	}
	// reinicia TreeView
	// CriaSegmentoParaRedes();
	TreeViewInicia();
	TreeViewChange(TreeView, node_default);
}

/* DVK 2012.10.09 fim */
// ---------------------------------------------------------------------------
void __fastcall TFormMercado::ActionRegiaoEditadoExecute(TObject *Sender)
{
	// caso tenha sido removida alguma regiao
	RetiraSegmentoRegiaoRemovida();
	// cria Segmento p/ Regiao que ainda não tem Segmento associada
	CriaSegmentoParaRegioes();
	// atualiza o form
	AtualizaForm();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::ActionRetiraSegmentoExecute(TObject *Sender)
{
	// variáveis locais
	AnsiString txt;
	TTreeNode *node, *node_pai;
	VTSegmento *segmento;

	// proteção: verifica se há um TreeNode selecionado
	if ((node = TreeView->Selected) == NULL)
		return;
	// verifica se Node de Level 1 ou superior
	while (node->Level > 1)
	{
		node = node->Parent;
	}
	if (node->Level != 1)
		return;
	// determina Node pai
	node_pai = node->Parent;
	// verifica se há uma Segmento selecionada
	if ((segmento = ExisteSegmentoSelecionada(node)) == NULL)
		return;
	// verifica se é a Segmento default
	if (segmento == mercado->ExisteSegmentoDefault())
	{
		Aviso("Taxas de crescimento default não podem ser excluídas");
		return;
	}
	// pede confirmação do usuário
	txt += AnsiQuotedStr(segmento->Codigo, '\'');
	if (Confirma("Taxas de crescimento p/ " + txt + " serão excluídas.",
		"Confirma exclusão ?") != IDYES)
		return;
	// recinicia crescimento da Segmento e marca Segmento como não Enabled
	segmento->IniciaCrescimentoNulo();
	segmento->Enabled = false;
	// exclui TreeNode
	// node->Delete();
	// esconde TChart
	chart->Visible = false;
	// seleciona Node pai
	TreeViewChange(TreeView, node_pai);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::ActionSelAreaExecute(TObject *Sender)
{
	// habilita seleção de área no gráfico
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::AtualizaForm(void)
{
	// iniica CBoxAnoInicial e CBoxAnoFinal
	CBoxAnoInicia();
	// se necessário, cria Segmento default
	CriaSegmentoDefault();
	// cria Segmento p/ TipoRede e Rede que ainda não tem Segmento associada
	CriaSegmentoParaRedes();
	// cria Segmento p/ Regiao que ainda não tem Segmento associada
	CriaSegmentoParaRegioes();
	// inicia TreeView
	TreeViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::BaseDadoConecta(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// OOPS: Modificado em 20120611
	// verifica base de dados em uso
	if (!geral)
		return;
	if ((bd = DLL_NewObjBD(apl)) != NULL)
	{
		bd->Conecta(geral->BaseDado);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::BaseDadoDesconecta(void)
{
	// fecha conexão com Oracle
	// OOPS_perda: não há necessidade, o destrutor desconecta
	// if (bd) {bd->Desconecta(); delete bd; bd = NULL;}
	if (bd)
	{
		delete bd;
		bd = NULL;
	}
	// fecha conexão com Access
	// if (bd_mercado) {delete bd_mercado; bd_mercado = NULL;}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::CBoxAnoChange(TObject *Sender)
{
	// variáveis locais
	int ano_ini, ano_fim;
	VTSegmento *segmento;
	TList *lisSEGMENTO;

	try
	{ // lê ano inial e final
		ano_ini = StrToInt(CBoxAnoInicial->Text);
		ano_fim = StrToInt(CBoxAnoFinal->Text);
	}
	catch (Exception &e)
	{
		Aviso("Valores inválidos de ano inicial e final");
		return;
	}
	// redefine ano inicial e final do mercado
	mercado->RedefinePeriodo(ano_ini, ano_fim);
	// marca que todas Regioes foram alteradas
	lisSEGMENTO = mercado->LisSegmento();
	for (int n = 0; n < lisSEGMENTO->Count; n++)
	{
		segmento = (VTSegmento*)lisSEGMENTO->Items[n];
		segmento->Alterada = true;
	}
	// reinicia TreeView
	TreeViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::CBoxAnoInicia(void)
{
	// variáveis locais
	int ano_cur = YearOf(Now());
	int ano_ini = ano_cur - 10;
	int ano_fim = ano_cur + 20;

	// proteção caso o Mercado ainda não esteja definido
	if (mercado->AnoInicial == 0)
		mercado->AnoInicial = ano_ini;
	if (mercado->AnoFinal == 0)
		mercado->AnoFinal = ano_fim;
	// define ano inicial e final
	if (ano_ini > mercado->AnoInicial)
		ano_ini = mercado->AnoInicial;
	if (ano_fim < mercado->AnoFinal)
		ano_fim = mercado->AnoFinal;
	// inicia CBoxAnoInicial
	CBoxAnoInicial->Clear();
	for (int ano = ano_ini; ano <= ano_cur; ano++)
	{
		CBoxAnoInicial->Items->Add(IntToStr(ano));
	}
	// inicia CBoxAnoFinal
	CBoxAnoFinal->Clear();
	for (int ano = ano_cur; ano <= ano_fim; ano++)
	{
		CBoxAnoFinal->Items->Add(IntToStr(ano));
	}
	// pré seleciona ano inicial e final
	CBoxAnoInicial->ItemIndex = mercado->AnoInicial - ano_ini;
	CBoxAnoFinal->ItemIndex = mercado->AnoFinal - ano_cur;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::ChartCriaSeries(void)
{
	/*
	 //variáveis locais

	 VTClasses *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
	 TList     *lisCLASSE = classes->LisClasse();

	 for (int n = chart->SeriesCount; n < lisCLASSE->Count; n++)
	 {//cria uma nova Série p/ o Chart

	 }
	 */
}

// ---------------------------------------------------------------------------
TColor __fastcall TFormMercado::ChartCorSerie(int ind)
{
	TColor color[] =
	{clBlue, clRed, clGreen, clYellow, clTeal, clFuchsia, clBlack, clLime, clWebCoral};

	// proteção
	ind = (ind % (sizeof(color) / sizeof(TColor)));
	return (color[ind]);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::ChartInicia(TList *lisCRESCIMENTO, int index)
{
	// variáveis locais
	// double        val_min, val_max, increment = 0;
	double val_min, val_max;
	// double        margem;
	VTCrescimento *crescimento;

	// reinicia TChart
	chart->LeftAxis->Minimum = 0;
	chart->LeftAxis->Maximum = 0;
	// desabilita todas as Series
	for (int n = 0; n < chart->SeriesCount(); n++)
	{
		chart->Series[n]->Active = false;
	}
	// loop p/ todo objeto Crescimento
	for (int n = 0; (n < lisCRESCIMENTO->Count) && (n < chart->SeriesCount()); n++)
	{ // verifica se deve exibir um único Crescimento, indicado por index
		if ((index >= 0) && (n != index))
			continue;
		crescimento = (VTCrescimento*)lisCRESCIMENTO->Items[n];
		// verifica se Crescimento nulo
		// if (crescimento->Nulo()) continue;
		// reinicia TChartSerie c/ taxas do Crescimento
		chart->Series[n]->Active = true; /* DVK 2012.10.10 (index < 0) */
		// ChartIniciaSerie(crescimento, chart->Series[n], ChartCorSerie(n), (n == 0));
		ChartIniciaSerie(crescimento, chart->Series[n], ChartCorSerie(n), (index < 0));
	}
	// completa configuração do TChart
	// inclui uma margem adicional nos valores máximo e mínimo
	// margem = 0.1 * Max(fabs(chart->LeftAxis->Maximum),fabs(chart->LeftAxis->Minimum));
	// //fkm 2015.09.02 alterei como setar o maximo e minimo
	// if(chart->LeftAxis->Maximum != 0 )         {val_max = chart->LeftAxis->Maximum + margem;}
	// else if (chart->LeftAxis->Maximum == 0)   {val_max =0;}
	// if(chart->LeftAxis->Minimum != 0 )         {val_min = chart->LeftAxis->Minimum - margem;}
	// else if (chart->LeftAxis->Minimum == 0)   {val_min =0;}
	// fkm 2015.09.02
	// define incremento
	// increment = (val_max - val_min) / 10.;
	// if      (increment > 1.)   increment = 1.;
	// else if (increment > 0.1)  increment = 0.1;
	// else if (increment > 0.01) increment = 0.01;
	// else                       increment = 0.001;
	// redefine valores mínimo e máximo do eixo y
	chart->LeftAxis->Visible = true;
	chart->LeftAxis->Automatic = false;
	// chart->LeftAxis->Maximum   = val_max;
	// chart->LeftAxis->Minimum   = val_min;
	// chart->LeftAxis->Increment = increment;
	chart->LeftAxis->Increment = 0; // fkm: 2015.09.02 incremento automatico
	chart->LeftAxis->Automatic = true; // fkm: 2015.09.02 maximum/minimum automatico
	chart->LeftAxis->MaximumRound = true; // fkm: 2015.09.02 arredonda automatico
	chart->LeftAxis->MinimumRound = true; // fkm: 2015.09.02 arredonda automatico

	chart->Visible = true;
	chart->Refresh();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::ChartIniciaSerie(VTCrescimento *crescimento, TChartSeries *serie,
	TColor cor, bool first)
{
	// variáveis locais
	double val;
	double val_max = 0., val_min = 0.;

	// reinicia série
	serie->Clear();
	serie->Active = true;
	serie->Title = crescimento->Classe->Codigo;
	serie->SeriesColor = cor;
	// preenche a Série c/ os valores em todos patamares
	for (int ind = 0, ano = crescimento->AnoInicial; ano <= crescimento->AnoFinal; ano++, ind++)
	{ // determina valor no patamar
		val = crescimento->Taxa_perc[ano];
		// insere ponto na Serie
		// serie->AddXY(ind, val, IntToStr(ind+1), cor);
		serie->AddXY(ano, val, IntToStr(ano), cor);
		// inclui ponto adicional p/ representar o fim do patamar(histograma)
		// serie->AddXY(ind+1, val, IntToStr(ind+1), cor);
		serie->AddXY(ano + 1, val, IntToStr(ano + 1), cor);
		// determina valores máximo e mínimo
		if (ind == 0)
		{
			val_max = val_min = val;
		}
		val_max = Max(val_max, val);
		val_min = Min(val_min, val);
	}
	// proteção contra valores nulos
	// if ((val_min == 0.)&&(val_max == 0.)) val_max = 1.;
	// define valores mínimo e máximo do TChart
	if (!first) /* DVK 2012.10.10 "!" */
	{
		chart->LeftAxis->Maximum = val_max;
		chart->LeftAxis->Minimum = val_min;
	}
	else
	{
		chart->LeftAxis->Maximum = Max(chart->LeftAxis->Maximum, val_max);
		chart->LeftAxis->Minimum = Min(chart->LeftAxis->Minimum, val_min);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::CriaArvoreRedes(void)
{
	// variáveis locais
	strArvorePD *arvore_pairs;
	VTRede *rede;
	TList *lisREDE = redes->LisRede();
	VTNode *node;
	VTRede *rede_pai;
	AnsiString sub, cod_sub;
	VTSegmento *segmento;
	TList *lisSEGMENTO = mercado->LisSegmento();

	for (int n = 0; n < lisSEGMENTO->Count; n++)
	{
		segmento = (VTSegmento*)lisSEGMENTO->Items[n];
		if(segmento->Tipo == smREDE)
		{
			// loop p/ todas Redes carregadas
			for (int nr = 0; nr < lisREDE->Count; nr++)
			{
				rede = (VTRede*)lisREDE->Items[nr];
				if (!rede->Carregada)
					continue;
				if(rede->Codigo == segmento->Codigo)
				{
					if (rede->TipoRede->Segmento == redePRI)
					{
						node = arvore->ExisteNode(rede);
						if (node == NULL)
							return;
						// alimentador pode não ter subestação supridora
						sub = "";
						cod_sub = "";
						if (node->Pai != NULL)
						{ // determina subestação supridora
							rede_pai = DeterminaSubestacaoNode(node->Pai);
							if (rede_pai != NULL)
							{
								arvore_pairs = new strArvorePD();
								arvore_pairs->PD.id_segmento = segmento->Id;
								arvore_pairs->PD.cod_segmento = segmento->Codigo;
								arvore_pairs->PD.tipo_rede = rede->TipoRede->Codigo;
								arvore_pairs->PD.id_sub = rede_pai->Extern_id;
								arvore_pairs->PD.cod_sub = rede_pai->Codigo;
								arvore_pairs->PD.primario = true;
								lisStrArvore->Add(arvore_pairs);
							}
						}
					}
					else
					{
						arvore_pairs = new strArvorePD();
						arvore_pairs->PD.id_segmento = segmento->Id;
						arvore_pairs->PD.cod_segmento = segmento->Codigo;
						arvore_pairs->PD.tipo_rede = rede->TipoRede->Codigo;
						arvore_pairs->PD.id_sub = "";
						arvore_pairs->PD.cod_sub = segmento->Codigo;
						arvore_pairs->PD.primario = false;
						lisStrArvore->Add(arvore_pairs);
					}
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::CriaSegmentoDefault(void)
{
	// verifica se existe Segmento default
	if (mercado->ExisteSegmentoDefault() == NULL)
	{ // cria Segmento Default
		mercado->CriaSegmentoDefault();
	}
}

// ---------------------------------------------------------------------------
// /* TODO : tirar do form, talvez colocar no tmercado */
// ---------------------------------------------------------------------------
void __fastcall TFormMercado::CriaSegmentoParaRedes(void)
{
	// variáveis locais
	VTRede *rede;
	TList *lisREDE = redes->LisRede();

	// loop p/ todas Redes carregadas
	for (int nr = 0; nr < lisREDE->Count; nr++)
	{
		rede = (VTRede*)lisREDE->Items[nr];
		if (!rede->Carregada)
			continue;
		// verifica se há Segmento definida p/ a TipoRede
		if (mercado->ExisteSegmento(rede->TipoRede) == NULL)
		{ // cria Segmento p/ a TipoRede
			mercado->CriaSegmentoNova(rede->TipoRede);
		}
		// verifica se há Segmento definida p/ a Rede
		// if (mercado->ExisteSegmento(rede) == NULL)   //FKM 2016.06.27
		if (mercado->ExisteSegmento(rede, true) == NULL)
		{ // cria Segmento p/ a Rede
			mercado->CriaSegmentoNova(rede);
		}
	}
}

// ---------------------------------------------------------------------------
// /* TODO : tirar do form, talvez colocar no tmercado */
// ---------------------------------------------------------------------------
void __fastcall TFormMercado::CriaSegmentoParaRegioes(void)
{ // variáveis locais
	VTRegioes *regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes));
	VTRegiao *regiao;
	TList *lisRegiao = regioes->LisRegiao();

	// loop p/ todas regioes carregadas
	for (int nr = 0; nr < lisRegiao->Count; nr++)
	{
		regiao = (VTRegiao*)lisRegiao->Items[nr];
		// verifica se há Segmento definida p/ a TipoRede
		// if (mercado->ExisteSegmento(regiao) == NULL)
		if (ExisteSegmento(regiao) == NULL)
		{ // cria Segmento p/ a regiao
			// mercado->CriaSegmentoNova(regiao);
			CriaSegmentoNova(regiao);
		}
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16233886
VTRede* __fastcall TFormMercado::DeterminaSubestacaoNode(VTNode *node_pai)
{
	// variáveis locais
	UnicodeString className;
	VTRede *rede;
	TObject *obj;
	TList *lisOBJ;

	//proteção
	if(node_pai->Object == NULL)
        return NULL;
	// verifica se object é rede subestação
	className = node_pai->Object->ClassName();
	if (className.CompareIC("TRede") == 0)
	{
		rede = (VTRede*)node_pai->Object;
		if (rede->TipoRede->Segmento == redeETD)
			return rede;
	}
	// verifica objects do node
	lisOBJ = node_pai->LisObjectNode();
	for (int n = 0; n < lisOBJ->Count; n++)
	{
		obj = (TObject*)lisOBJ->Items[n];
		className = obj->ClassName();
		if (className.CompareIC("TRede") == 0)
		{
			rede = (VTRede*)obj;
			if (rede->TipoRede->Segmento == redeETD)
				return rede;
		}
	}
	return NULL;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormMercado::ExisteAlteracao(void)
{
	// variáveis locais
	VTSegmento *segmento;
	TList *lisSEGMENTO = mercado->LisSegmento();

	// loop p/ todas Regioes
	for (int nr = 0; nr < lisSEGMENTO->Count; nr++)
	{
		segmento = (VTSegmento*)lisSEGMENTO->Items[nr];
		// verifica se Segmento foi alterada
		if (segmento->Alterada)
		{
			return true;
		}
		// verifica se Segmento pre-existente foi excluída pelo usuário
		if ((!segmento->Enabled) && (!segmento->Nova))
		{
			return true;
		}
	}

	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormMercado::ExisteAlteracaoEmRegioes(void)
{
	VTRegioes *regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes));
	TList *lisRegiao = regioes->LisRegiao();

	if (lisREGIAO_pre == NULL)
	{
		return true;
	}
	if (lisRegiao == NULL)
	{
		return true;
	}
	if (lisREGIAO_pre->Count != lisRegiao->Count)
	{
		return true;
	}
	// compara as listas, se algum item for diferente é diferente
	for (int ni = 0; ni < lisREGIAO_pre->Count; ni++)
	{
		if (lisREGIAO_pre->Items[ni] != lisRegiao->Items[ni])
		{
			return true;
		}
	}

	return false;
}

// ---------------------------------------------------------------------------
VTCrescimento* __fastcall TFormMercado::ExisteCrescimentoSelecionado(TTreeNode *node)
{
	// variáveis locais
	AnsiString class_name;
	TObject *object;
	VTCrescimento *crescimento = NULL;

	// proteção: verifica se há um TreeNode selecionado
	if (node == NULL)
		return (NULL);
	// verifica se Node de Level 2 ou superior
	while (node->Level > 2)
	{
		node = node->Parent;
	}
	if (node->Level != 2)
	{
		return (NULL);
	}
	// proteção: verifica se existe um Crescimento associado ao TreeNode
	if ((object = (TObject*)node->Data) == NULL)
	{
		return (NULL);
	}
	class_name = object->ClassName();
	if (class_name.AnsiCompareIC("TCrescimento") == 0)
		crescimento = (VTCrescimento*)object;
	return (crescimento);
}

// ---------------------------------------------------------------------------
VTSegmento* __fastcall TFormMercado::ExisteSegmentoSelecionada(TTreeNode *node)
{
	// variáveis locais
	AnsiString class_name;
	TObject *object;
	VTSegmento *segmento = NULL;

	// proteção: verifica se há um TreeNode selecionado
	if (node == NULL)
		return (NULL);
	// verifica se Node de Level 1 ou superior
	while (node->Level > 1)
	{
		node = node->Parent;
	}
	if (node->Level != 1)
	{
		return (NULL);
	}
	// proteção: verifica se existe uma Segmento associada ao TreeNode
	if ((object = (TObject*)node->Data) == NULL)
	{
		return (NULL);
	}
	class_name = object->ClassName();
	if (class_name.AnsiCompareIC("TSegmento") == 0)
		segmento = (VTSegmento*)object;
	return (segmento);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destrói Regioes novas (não foram salvas na base)
	RetiraSegmentoNova();
	// destrói o Form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	// assume que o Form pode ser fechado
	CanClose = true;
	// verifica se foram feitas alterações
	if (ExisteAlteracao())
	{ // pede confirmação do usuário
		if (Confirma("As alterações efetuadas no mercado serão perdidas.",
			"Deseja descartar as alterações ?") == IDYES)
		{ // o Form será fechado
			CanClose = true;
			// reinicia Mercado a partir da base de dados
			mercado->Reinicia();
			// OOPS: Modificado em 20120611
			/*
			 if      (bd)         bd->Geral->LeMercado();
			 else if (bd_mercado) bd_mercado->Le();
			 */
			// OOPS: deveria logar aqui?
			if (bd)
				bd->Geral->LeMercado();
			else
				return;
			// atualiza crescimentos das Cargas
			mercado->IniciaRedes(redes);
		}
		else
		{ // o Form não pode ser fechado
			CanClose = false;
		}
	}
}

// ---------------------------------------------------------------------------
// FKM 2015.10.13
void __fastcall TFormMercado::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
	// posiciona o Form sem alinhamento
	ActionAlignNoneExecute(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::FormShow(TObject *Sender)
{
	// posiciona o Form
	FormIniciaPosicao(); // FKM 2015.10.13
	// mostra regioes
	RegioesVisible(true); // FKM 2016.04.06
	// inicia TreeView
	// TreeViewInicia();
}

// ---------------------------------------------------------------------------
/*
 //---------------------------------------------------------------------------
 void __fastcall TFormMercado::InsereSegmentoDefinidaPorArea(AnsiString codigo)
 {
 //variáveis locais
 VTSegmento    *segmento;
 TTreeNode   *node;

 //cria uma nova Segmento associada a uma área
 if ((segmento = mercado->CriaSegmentoNova()) == NULL) return;
 segmento->Codigo = codigo;
 segmento->Tipo   = segmentoAREA;
 //insere Segmento no TreeView
 if ((node = TreeViewInsereSegmento(node_area, segmento)) == NULL) return;
 //entra em modo edição
 TreeView->Selected = node;
 ActionEditaSegmentoExecute(NULL);
 }
 */
/*
 //---------------------------------------------------------------------------
 void __fastcall TFormMercado::RetiraDaBaseSegmentoComCrescimentoNulo(void)
 {
 //variáveis locais
 VTSegmento    *segmento;
 TList       *lisSEGMENTO = mercado->LisSegmento();

 //loop p/ todas Regioes
 for (int nr = lisSEGMENTO->Count-1; nr >= 0; nr--)
 {
 segmento = (VTSegmento*)lisSEGMENTO->Items[nr];
 //não elimina Segmento Default
 if (segmento->Tipo == segmentoDEFAULT) continue;
 //não elimina Segmento alterada
 if (segmento->Alterada) continue;
 //não elimina Segmento pré-existente
 if (! segmento->Nova) continue;
 //if (segmento->Id > 0) continue;
 //verifica se a Segmento possui crescimento nulo
 if (segmento->CrescimentoNulo())
 {//retira Segmento da base de dados
 if (segmento->Id > 0)
 {//retira Segmento da base de dados
 // OOPS: Modificado em 20120611
 // OOPS: o else deveria indicar um erro???
 if (bd) bd->Geral->RetiraSegmentoMercado(segmento);
 }
 //retira Segmento
 mercado->RetiraSegmento(segmento);
 }
 }
 }
 */

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::RegistraRegioesPreExistentes(void)
{
	VTRegioes *regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes));
	TList *lisRegioes = regioes->LisRegiao();

	lisREGIAO_pre->Clear();

	CopiaTList(lisRegioes, lisREGIAO_pre);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::RegioesVisible(bool visible)
{ // variaveis locais
	VTRegiao *regiao;
	VTRegioes *regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes));
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
	VTGrafico *graf = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// garante modo geo-referenciado
	if (redegraf->ModoGrafico != modoGEOREFERENCIADO)
	{
		redegraf->ModoGrafico = modoGEOREFERENCIADO;
	}
	if (regioes)
	{
		for (int nr = 0; nr < regioes->LisRegiao()->Count; nr++)
		{
			regiao = (VTRegiao*) regioes->LisRegiao()->Items[nr];
			regiao->Visible = visible;
		}
	}
	if (graf)
		graf->Redraw();
}

// ---------------------------------------------------------------------------
// /* TODO : tirar do form, talvez colocar no tmercado */
// ---------------------------------------------------------------------------
void __fastcall TFormMercado::RetiraDaBaseSegmentoSemCrescimento(void)
{
	// variáveis locais
	VTSegmento *segmento;
	TList *lisSEGMENTO = mercado->LisSegmento();

	// loop p/ todas Regioes
	for (int nr = lisSEGMENTO->Count - 1; nr >= 0; nr--)
	{
		segmento = (VTSegmento*)lisSEGMENTO->Items[nr];
		// não elimina Segmento Default
		if (segmento->Tipo == smDEFAULT)
			continue;
		// não elimina Segmento com crescimento
		if (segmento->Enabled)
			continue;
		// retira Segmento da base de dados
		if (segmento->Id > 0)
		{ // retira Segmento da base de dados
			/*
			 if      (bd)         bd->Geral->RetiraSegmentoMercado(segmento);
			 else if (bd_mercado) bd_mercado->RetiraSegmento(segmento);
			 */
			// OOPS: Modificado em 20120611
			// OOPS: o else deveria indicar um erro???
			if (bd)
				bd->Geral->RetiraSegmentoMercado(segmento);
		}
		// retira Segmento
		mercado->RetiraSegmento(segmento);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::RetiraSegmentoSemRegiao(VTSegmento *segmento)
{
	VTRegioes *regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes));
	TList *lisRegiao = regioes->LisRegiao();

	// não elimina Segmento Default
	if (segmento->Tipo == smDEFAULT)
		return;
	// retira Segmento da base de dados
	if (segmento->Tipo == smREGIAO)
	{
		if (segmento->Regiao != NULL)
		{ // caso a regiao nao esteja no regioes
			if (lisRegiao->IndexOf(segmento->Regiao) < 0)
			{ // caso seja um segmento na base
				if (segmento->Id > 0)
				{
					if (bd)
						bd->Geral->RetiraSegmentoMercado(segmento);
				}
				// retira Segmento
				mercado->RetiraSegmento(segmento);
			}
		}
		else
		{ // caso seja um segmento na base
			if (segmento->Id > 0)
			{
				if (bd)
					bd->Geral->RetiraSegmentoMercado(segmento);
			}
			// retira Segmento
			mercado->RetiraSegmento(segmento);
		}
	}
}

// ---------------------------------------------------------------------------
// /* TODO : tirar do form, talvez colocar no tmercado */
// ---------------------------------------------------------------------------
void __fastcall TFormMercado::RetiraSegmentoNova(void)
{
	// variáveis locais
	VTSegmento *segmento;
	TList *lisSEGMENTO = mercado->LisSegmento();

	// loop p/ todas Regioes
	for (int nr = lisSEGMENTO->Count - 1; nr >= 0; nr--)
	{
		segmento = (VTSegmento*)lisSEGMENTO->Items[nr];
		// não elimina Segmento Default
		if (segmento->Tipo == smDEFAULT)
			continue;
		// elimina Segmento nova apenas do Mercado (não existe na base)
		if (segmento->Nova)
		{
			if (!segmento->Enabled)
			{ // retira Segmento
				mercado->RetiraSegmento(segmento);
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::RetiraSegmentoRegiaoRemovida(void)
{ // variáveis locais
	VTSegmento *segmento;
	TList *lisSEGMENTO = mercado->LisSegmento();
	VTRegiao *regiao;
	VTRegioes *regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes));

	// loop p/ todas Regioes
	for (int nr = lisSEGMENTO->Count - 1; nr >= 0; nr--)
	{
		segmento = (VTSegmento*)lisSEGMENTO->Items[nr];
		if (segmento->Tipo == smREGIAO)
		{
			regiao = segmento->Regiao;
			RetiraSegmentoSemRegiao(segmento);
		}
	}
}

// ---------------------------------------------------------------------------
// /* TODO : tirar do form, talvez colocar no tmercado */
// ---------------------------------------------------------------------------
void __fastcall TFormMercado::RetornaModoGrafico(void)
{ // variaveis locais
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// esconde regioes
	RegioesVisible(false);
	// garante modo geo-referenciado
	if (modoGraficoAnterior != redegraf->ModoGrafico)
	{
		redegraf->ModoGrafico = modoGraficoAnterior;
		grafico->ModoGraficoAlterado();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::TreeViewAdvancedCustomDrawItem(TCustomTreeView *Sender,
	TTreeNode *Node, TCustomDrawState State, TCustomDrawStage Stage, bool &PaintImages,
	bool &DefaultDraw)
{
	// variáveis locais
	int ano;
	double taxa_perc;
	TColor color;
	TRect NodeRect;
	VTCrescimento *crescimento = (VTCrescimento*)Node->Data;

	// proteção
	if (Stage != cdPostPaint)
		return;
	// obtém área de texto do TreeNode
	NodeRect = Node->DisplayRect(true);
	NodeRect.Right = TreeView->ClientRect.Right;
	// salva cor de Brush
	color = TreeView->Canvas->Brush->Color;
	if (Node->Level < 2)
	{ // limpa área de texto do TreeNode
		if (Node->Selected)
		{
			TreeView->Canvas->Brush->Color = clMenuHighlight;
		}
		TreeView->Canvas->FillRect(NodeRect);
		// apresenta texto do TreeNode
		TreeView->Canvas->TextRect(NodeRect, NodeRect.Left, NodeRect.Top + 2, Node->Text);
	}
	// verifica TreNode Level
	else if (Node->Level == 2)
	{ // limpa área de texto do TreeNode
		if (Node->Selected)
		{
			TreeView->Canvas->Brush->Color = clMenuHighlight;
		}
		TreeView->Canvas->FillRect(NodeRect);
		// determina retângulo p/ exibir cor associada à curva de crescimento
		TRect rect_col1(NodeRect.Left + 1, NodeRect.Top + 1,
			NodeRect.Left + (NodeRect.Bottom - NodeRect.Top) - 2, NodeRect.Bottom - 1);
		// desenha retângulo c/ a Cor usada no TChart
		TreeView->Canvas->Brush->Color = ChartCorSerie(Node->Index);
		TreeView->Canvas->FillRect(rect_col1);
		// apresenta texto do TreeNode
		rect_col1.Left = rect_col1.Right + 1;
		rect_col1.Top = NodeRect.Top;
		rect_col1.Right = NodeRect.Right;
		rect_col1.Bottom = NodeRect.Bottom;
		TreeView->Canvas->TextRect(rect_col1, rect_col1.Left, rect_col1.Top + 2, Node->Text);
		// restaura cor de Brush
		TreeView->Canvas->Brush->Color = color;
		// bloqueia alteração automática
		DefaultDraw = false;
	}
	else if (Node->Level == 3)
	{ // limpa área de texto do TreeNode
		if (Node->Selected)
		{
			TreeView->Canvas->Brush->Color = clMenuHighlight;
		}
		TreeView->Canvas->FillRect(NodeRect);
		// determina ano e taxa de crescimento
		ano = crescimento->AnoInicial + Node->Index;
		taxa_perc = crescimento->Taxa_perc[ano];
		// divide a área de texto em duas colunas
		TRect rect_col1(NodeRect);
		TRect rect_col2(NodeRect);
		// rect_col1.Right = (NodeRect.Left + 100);
		rect_col1.Right = TreeViewHeader->Sections->Items[0]->Width;
		rect_col2.Left = rect_col1.Right + 2;
		// preenche TreeNode c/ ano e taxa (salvas na estrutura tv)
		TreeView->Canvas->TextRect(rect_col1, rect_col1.Left, rect_col1.Top + 2, IntToStr(ano));
		TreeView->Canvas->TextRect(rect_col2, rect_col2.Left, rect_col2.Top + 2,
			DoubleToStr("%6.2f", taxa_perc));
	}
	// restaura cor de Brush
	TreeView->Canvas->Brush->Color = color;
	// bloqueia alteração automática
	DefaultDraw = false;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::TreeViewChange(TObject *Sender, TTreeNode *Node)
{
	// variáveis locais
	int index;
	VTCrescimento *crescimento;
	VTSegmento *segmento;

	// proteção: verifica se foi selecionado o TreeNode de uma Região
	if ((segmento = ExisteSegmentoSelecionada(Node)) == NULL)
	{ // esconde PanelAviso e TChart
		PanelAviso->Visible = false;
		chart->Visible = false;
		// desabilita Actions
		ActionSelArea->Enabled = false;
		ActionEditaSegmento->Enabled = false;
		ActionRetiraSegmento->Enabled = false;
		return;
	}
	// existe uma Região selecionada: reabilita Actions
	ActionSelArea->Enabled = (segmento->Tipo == smAREA);
	ActionEditaSegmento->Enabled = true;
	ActionRetiraSegmento->Enabled = true;
	// verifica se a Região possui Crescimento
	// if ((segmento->Nova) && (! segmento->Alterada) && segmento->CrescimentoNulo())
	if (!segmento->Enabled)
	{ // esconde TChart
		chart->Visible = false;
		// exibe PanelAviso
		LabelRegiao->Caption = segmento->Codigo;
		PanelAviso->Visible = true;
		return;
	}
	// inicia lisCRE c/ todos Crescimentos da Segmento
	lisCRE->Clear();
	CopiaTList(segmento->LisCrescimento(), lisCRE);
	// verifica se foi selecionado o TreeNode de um Crescimento específico
	crescimento = ExisteCrescimentoSelecionado(Node);
	// define índice do Crescimento a ser exibido
	index = (crescimento == NULL) ? -1 : lisCRE->IndexOf(crescimento);
	// reinicia e exibe TChart
	ChartInicia(lisCRE, index);
	chart->Visible = true;
	// esconde PanelAviso
	PanelAviso->Visible = false;
}

////---------------------------------------------------------------------------
// void __fastcall TFormMercado::TreeViewInicia(void)
// {
// //variáveis locais
// TTreeNode *node_pai;
// VTSegmento  *segmento;
// TList     *lisSEGMENTO = mercado->LisSegmento();
//
// //reinicia TreeView
// TreeView->Items->Clear();
// //insere TreeNodes p/ Default, Áreas, TipoRede e Redes
// //node_area      = TreeView->Items->Add(NULL, "Por Região");
// node_regiao    = TreeView->Items->Add(NULL, "Por Região");
// //node_regiao    = TreeView->Items->Add(NULL, " "); //FKM 2015.10.13 provisório
// //node_regiao->Enabled = false; //FKM - 2015.10.09 provisório
// node_carregada = TreeView->Items->Add(NULL, "Redes carregadas");
// node_rede      = TreeView->Items->Add(NULL, "Outras redes da base de dados");
// node_tiporede  = TreeView->Items->Add(NULL, "Tipo de rede");
// node_default   = TreeView->Items->Add(NULL, "Default");
// //loop p/ todas Regioes
// for (int n = 0; n < lisSEGMENTO->Count; n++)
// {
// segmento = (VTSegmento*)lisSEGMENTO->Items[n];
// switch(segmento->Tipo)
// {
// case smDEFAULT:
// node_pai = node_default;
// break;
// case smTIPOREDE:
// node_pai = node_tiporede;
// break;
// case smREDE:  //verifica se a Rede está carregada
// if ((redes->ExisteRede(segmento->Codigo)) != NULL) node_pai = node_carregada;
// else                                               node_pai = node_rede;
// break;
// case smREGIAO:   //fkm 2015.07.30
// //node_pai = node_carga;
// node_pai = node_regiao;
// break;
// case smAREA:
// //node_pai = node_area;
// node_pai = NULL;
// break;
// case smCARGA:
// //node_pai = node_carga;
// node_pai = NULL;
// break;
// default:
// node_pai = NULL;
// break;
// }
// //cria TreeNode p/ Segmento
// if (node_pai !=  NULL) TreeViewInsereSegmento(node_pai, segmento);
// }
// //elimina node_rede caso não possua nenhum TreeNode filho
// if (node_rede->Count == 0)
// {//exclui TreeNode
// node_rede->Delete();
// node_rede = NULL;
// }
// //ordena Nodes
// //node_area->AlphaSort();
// if (node_rede)      node_rede-> AlphaSort();
// if (node_tiporede)  node_tiporede->AlphaSort();
// if (node_default)   node_default-> AlphaSort();
// if (node_carregada) node_carregada->AlphaSort();
// if (node_regiao) 	  node_regiao->AlphaSort();  //fkm 2015.07.30
// //expande Nodes de nível 0
// //node_area->Expand(false);
// //node_rede->Expand(false);
// //node_tiporede->Expand(false);
// node_default->Expand(false);
// //seleciona Node default
// TreeView->Selected = node_default;
// }
// ---------------------------------------------------------------------------
void __fastcall TFormMercado::TreeViewInicia(void)
{
	// variáveis locais
	TTreeNode *node_pai;
	VTSegmento *segmento;
	TList *lisSEGMENTO = mercado->LisSegmento();
	bool somenteBT;
	VTRede *rede;

	// verifica se só possui redes bt
	somenteBT = VerificaSomenteSDBT();
	// reinicia TreeView
	TreeView->Items->Clear();
	// insere TreeNodes p/ Default, Áreas, TipoRede e Redes
	// node_area      = TreeView->Items->Add(NULL, "Por Região");
	node_regiao = TreeView->Items->Add(NULL, "Por Região");
	// node_regiao    = TreeView->Items->Add(NULL, " "); //FKM 2015.10.13 provisório
	// node_regiao->Enabled = false; //FKM - 2015.10.09 provisório
	node_carregada = TreeView->Items->Add(NULL, "Redes carregadas");
	node_rede = TreeView->Items->Add(NULL, "Outras redes da base de dados");
	node_tiporede = TreeView->Items->Add(NULL, "Tipo de rede");
	node_default = TreeView->Items->Add(NULL, "Default");
	// loop p/ todas Regioes
	for (int n = 0; n < lisSEGMENTO->Count; n++)
	{
		segmento = (VTSegmento*)lisSEGMENTO->Items[n];
		switch (segmento->Tipo)
		{
		case smDEFAULT:
			node_pai = node_default;
			break;
		case smTIPOREDE:
			node_pai = node_tiporede;
			break;
		case smREDE: // verifica se a Rede está carregada
			if ((rede = redes->ExisteRede(segmento->Codigo)) != NULL)
			{ // verifica se a rede é BT
				if (rede->TipoRede->Segmento == redeSEC)
				{ // sendo bt, só adiciona se somente houver bts
					if (somenteBT)
					{
						node_pai = node_carregada;
					}
					else
					{
						node_pai = NULL;
					}
				}
				else
				{ // sendo outro tipo, adiciona
					node_pai = node_carregada;
				}
			}
			else
			{
				node_pai = node_rede;
			}
			break;
		case smREGIAO: // fkm 2015.07.30
			// node_pai = node_carga;
			node_pai = node_regiao;
			break;
		case smAREA:
			// node_pai = node_area;
			node_pai = NULL;
			break;
		case smCARGA:
			// node_pai = node_carga;
			node_pai = NULL;
			break;
		default:
			node_pai = NULL;
			break;
		}
		// cria TreeNode p/ Segmento
		if (node_pai != NULL)
			TreeViewInsereSegmento(node_pai, segmento);
	}
	// elimina node_rede caso não possua nenhum TreeNode filho
	if (node_rede->Count == 0)
	{ // exclui TreeNode
		node_rede->Delete();
		node_rede = NULL;
	}
	// ordena Nodes
	// node_area->AlphaSort();
	if (node_rede)
		node_rede->AlphaSort();
	if (node_tiporede)
		node_tiporede->AlphaSort();
	if (node_default)
		node_default->AlphaSort();
	if (node_carregada)
		node_carregada->AlphaSort();
	if (node_regiao)
		node_regiao->AlphaSort(); // fkm 2015.07.30
	// expande Nodes de nível 0
	// node_area->Expand(false);
	// node_rede->Expand(false);
	// node_tiporede->Expand(false);
	node_default->Expand(false);
	// seleciona Node default
	TreeView->Selected = node_default;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#tasks/14893054
TTreeNode* __fastcall TFormMercado::TreeViewInsereSegmento(TTreeNode *node_pai,
	VTSegmento *segmento)
{
	// variáveis locais
	int ano_inicial, ano_final;
	TTreeNode *node_segmento, *node_classe;
	VTCrescimento *crescimento;
	TList *lisEXT;
	VTRegiao *regiao;
	strArvorePD *arvore_pairs;
	AnsiString nome_segmento = segmento->Codigo;

	// proteção:
	if ((segmento == NULL) || (node_pai == NULL))
		return (NULL);
	if(segmento->Tipo == smREDE)
	{
		// loop p/ todas Redes carregadas
		for (int nr = 0; nr < lisStrArvore->Count; nr++)
		{
			arvore_pairs = (strArvorePD*)lisStrArvore->Items[nr];
			if (segmento->Id != arvore_pairs->PD.id_segmento)
			continue;
			if(arvore_pairs->PD.primario)
			{
				nome_segmento = "SED: " + arvore_pairs->PD.cod_sub + ", SDMT: " + arvore_pairs->PD.cod_segmento;
				break;
			}
			else
			{
			   nome_segmento = arvore_pairs->PD.tipo_rede + ": " + arvore_pairs->PD.cod_segmento;
            }
		}
	}
	// cria TreeNode p/ a Região
	node_segmento = TreeView->Items->AddChildObject(node_pai, nome_segmento, segmento);
	// verifica se as taxas de crescimento ainda não foram definidas
	if (!segmento->Enabled)
		return (node_segmento);
	// loop p/ todo Crescimento da Segmento
	lisEXT = segmento->LisCrescimento();
	//ordena lisEXT por ordem alfabetica o codigo da classe
	lisEXT->Sort(CompareCodigoClasse);
	for (int n = 0; n < lisEXT->Count; n++)
	{
		crescimento = (VTCrescimento*)lisEXT->Items[n];
		// insere TreeNodes p/ Crescimento
		node_classe = TreeView->Items->AddChildObject(node_segmento, crescimento->Codigo,
			crescimento);
		// não inclui valores p/ Crescimento nulo
		// if (crescimento->Nulo()) continue;
		// determina ano inicial e final p/ as taxas de crescimento
		ano_inicial = crescimento->AnoInicial;
		ano_final = crescimento->AnoFinal;
		// cria TreeNodes c/ ano e taxa de  crescimento
		for (int ano = ano_inicial; ano <= ano_final; ano++)
		{ // insere TreeNode c/ taxa de crescimento do ano
			TreeView->Items->AddChildObject(node_classe, IntToStr(ano), crescimento);
		}
	}
	// ordena alfabeticamente node_pai
	// node_pai->AlphaSort();
	return (node_segmento);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMercado::TreeViewMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	// determina TreeNode na posição X,Y
	TTreeNode *Node = TreeView->GetNodeAt(X, Y);

	// proteção
	if (Node == NULL)
		return;
	// descarta Click no sinal +/- (expand/collapse do TreeNode)
	TRect NodeRect = Node->DisplayRect(true);
	if (X < NodeRect.Left)
		return;
	// verifica se o TreeNode está selecionado
	if (!Node->Selected)
	{ // seleciona o TreeNode
		Node->Selected = true;
	}
}

// ---------------------------------------------------------------------------
// /* TODO : tirar do form, talvez colocar no tmercado */
// ---------------------------------------------------------------------------
bool __fastcall TFormMercado::VerificaSomenteSDBT(void)
{ // variaveis locais
	TList *lisREDESCARREGADAS;
	VTRede *rede;
	bool sucesso = false;
	try
	{
		lisREDESCARREGADAS = new TList;
		redes->LisRedeCarregada(lisREDESCARREGADAS);
		// percorre todas redes carregadas  a procura de redes difenrentes de BT
		for (int nr = 0; nr < lisREDESCARREGADAS->Count; nr++)
		{
			rede = (VTRede*) lisREDESCARREGADAS->Items[nr];
			// se o tipo for diferente de redeSEC
			if (rede->TipoRede->Segmento != redeSEC)
			{
				return (false);
			}
		}
		sucesso = true;
	}
	catch (Exception &e)
	{
		// nada
	}
	// destroi lista
	if (lisREDESCARREGADAS)
	{
		delete lisREDESCARREGADAS;
		lisREDESCARREGADAS = NULL;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
// funcoes que devem ser alteradas no VTMERCADO
// ---------------------------------------------------------------------------
VTSegmento* __fastcall TFormMercado::ExisteSegmento(VTRegiao *regiao)
{
	// variáveis locais
	VTSegmento *segmento;
	TList *lisSEGMENTO = mercado->LisSegmento();
	// proteção
	if (regiao == NULL)
		return (NULL);
	// procura um Segmento que tenha a regiao
	for (int n = 0; n < lisSEGMENTO->Count; n++)
	{
		segmento = (VTSegmento*)lisSEGMENTO->Items[n];
		if ((segmento->Tipo == smREGIAO) && (segmento->Regiao == regiao))
		{
			return (segmento);
		}
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTSegmento* __fastcall TFormMercado::CriaSegmentoNova(VTRegiao *regiao)
{
	// variáveis locais
	VTSegmento *segmento;

	// cria uma Segmento nova (associada a uma Area)
	if ((segmento = mercado->CriaSegmentoNova()) != NULL)
	{ // redefine Tipo e Codigo da Segmento
		segmento->Tipo = smREGIAO;
		segmento->Codigo = regiao->Codigo;
		segmento->Regiao = regiao;
	}
	return (segmento);
}

////---------------------------------------------------------------------------
//void __fastcall TFormMercado::TreeViewMouseMove(TObject *Sender, TShiftState Shift,
//          int X, int Y)
//{
////  	// determina TreeNode na posição X,Y
//	strArvorePD *arvore_pairs;
//	TTreeNode *Node = TreeView->GetNodeAt(X, Y);
//	TObject *object;
//	VTSegmento *segmento = NULL;
//	AnsiString class_name;
//	AnsiString tag = "";
//	VTRede *rede;
//	TList *lisREDE = redes->LisRede();
//
//	// proteção
//	if (Node == NULL)
//		return;
//	// proteção: verifica se existe uma Segmento associada ao TreeNode
//	if ((object = (TObject*)Node->Data) == NULL)
//	{
//		return;
//	}
//	class_name = object->ClassName();
//	if (class_name.AnsiCompareIC("TSegmento") == 0)
//		segmento = (VTSegmento*)object;
//	if(segmento==NULL)
//	return;
//	if(segmento->Tipo == smREDE)
//	{
//		// loop p/ todas Redes carregadas
//		for (int nr = 0; nr < lisStrArvore->Count; nr++)
//		{
//			arvore_pairs = (strArvorePD*)lisStrArvore->Items[nr];
//			if (segmento->Id != arvore_pairs->PD.id_segmento)
//			continue;
//			if(arvore_pairs->PD.subestacao)
//			   {
//				  tag = "SED: " + segmento->Codigo;
//				  break;
//			   }
//			else
//			   {
//				  tag = "SED: " + arvore_pairs->PD.cod_sub + ", SDMT: " + arvore_pairs->PD.cod_segmento;
//				  break;
//			   }
//			//}
//		}
//	//Node->Tag = tag;
//	TreeView->Hint = tag;
//	//TreeViewChange(NULL, Node);
//	}
//}

//---------------------------------------------------------------------------


