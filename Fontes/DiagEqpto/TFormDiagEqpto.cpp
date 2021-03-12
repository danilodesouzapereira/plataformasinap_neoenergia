// ---------------------------------------------------------------------------
#include <vcl.h>
#include <cstdio>
using namespace std;
#pragma hdrstop
#include "..\Apl\VTApl.h"
#include "..\Edita\VTEdita.h"
#include "..\Rede\VTArea.h"
// #include "..\Rede\VTBarra.h"
// #include "..\Rede\VTRede.h"
// #include "..\Rede\VTRedes.h"
#include "..\Diretorio\VTPath.h"
#include "..\Grafico\VTGrafico.h"
#include "VTDiagEqpto.h"
#include "TFormDiagEqpto.h"
#include "TFormDiagTrecho.h"
#include "TDiagTopo.h"
#include "TDiagArranjo.h"
#include "TDiagBarra.h"
#include "TDiagBIso.h"
#include "TDiagCabo.h"
#include "TDiagCapacitor.h"
#include "TDiagCarga.h"
#include "TDiagChave.h"
#include "TDiagRede.h"
#include "TDiagReg.h"
#include "TDiagTrecho.h"
#include "TDiagTrafo.h"
#include "..\Catalogo\TFormCatalogoCap.h"
#include "..\Catalogo\TFormCatalogoTrf.h"
// #include "..\Zona\VTZonas.h"
#include "..\..\Progresso\VTProgresso.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Progresso.h"
// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Cronometro.h"
#include "..\Cronometro\VTCronometro.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
__fastcall TFormDiagEqpto::TFormDiagEqpto(TComponent* Owner, VTApl *apl, TWinControl *parent)
	: TForm(Owner)
{
	this->apl = apl;
	// insere Form em seu parent
	if (parent)
		Parent = parent;
	// cria progresso proprio
	progresso = DLL_NewObjProgresso(this, NULL);
	diagTopo = new TDiagTopo(apl, progresso);
	diagTopo->Inicia();
	// define ordenação do ListView pelo tipo da Rede
	ListViewSort.ColumnIndex = 1;
	ListViewSort.OrdemCrescente = true;
	// inicializa o modoGeral
	dgGeral.barra        = true;
	dgGeral.trecho       = true;
	dgGeral.chave        = true;
	dgGeral.capacitor    = true;
	dgGeral.trafo        = true;
    dgGeral.reg          = true;
	dgGeral.carga        = true;
	dgGeral.arranjo      = true;
	dgGeral.cabo         = true;
	dgGeral.rede         = true;
	dgGeral.blocoIsolado = true;
	// inicia no modoGeral
	modoForm = modoGERAL;
	// modoForm = modoCHAVE;
	IniciaCheckListGeral();
	ActionExibeGeral->Execute();
	// cria uma lista auxiliar vazia
	emptyList = new TList;
}

// ---------------------------------------------------------------------------
__fastcall TFormDiagEqpto::~TFormDiagEqpto(void)
{
	if (diagTopo)
	{
		delete diagTopo;
		diagTopo = NULL;
	}
	if (emptyList)
	{
		delete emptyList;
		emptyList = NULL;
	}
	if (progresso)
	{
		delete progresso;
		progresso = NULL;
	}
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
void __fastcall TFormDiagEqpto::ActionConfiguraExecute(TObject *Sender)
{
    VTDiagEqpto * diagEqpto;
    switch (modoForm)
    {
    case modoGERAL:
    case modoARRANJO:
    case modoBARRA:
    case modoBLOCOISOLADO:
    case modoREDE:
    case modoREG:
        // nada a fazer
        break;
    case modoCABO:
        diagEqpto = diagTopo->DGCabo;
        diagEqpto->ShowFormCfg();
        break;
    case modoCAPACITOR:
        diagEqpto = diagTopo->DGCapacitor;
        diagEqpto->ShowFormCfg();
        break;
    case modoCARGA:
        diagEqpto = diagTopo->DGCarga;
        diagEqpto->ShowFormCfg();
        break;
    case modoCHAVE:
        diagEqpto = diagTopo->DGChave;
        diagEqpto->ShowFormCfg();
        break;
    case modoTRAFO:
        diagEqpto = diagTopo->DGTrafo;
        diagEqpto->ShowFormCfg();
        break;
    case modoTRECHO:
        diagEqpto = diagTopo->DGTrecho;
        diagEqpto->ShowFormCfg();
        break;
    default:
        // nada a fazer
        ;
    }
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
void __fastcall TFormDiagEqpto::ActionExecutaDiagExecute(TObject *Sender)
{
    // variaveis locais
    AnsiString aviso = "";
    bool problema = false;

    // prepara a rede caso nao tenha sido preparada
    if (!diagTopo->RedePreparada)
    {
        diagTopo->PreExecuta();
    }
    switch (modoForm)
    {
    case modoGERAL:
        // exibe progresso
        if (progresso)
            progresso->Start(progTEXTO);
        if (progresso)
            progresso->Add("Executando...");
        // salva as opcoes marcadas do checklist
        LeCheckList();
        if (dgGeral.arranjo)
        { // exibe progresso
            if (progresso)
                progresso->Add("Verificando arranjos");
            if (!(diagTopo->DGArranjo)->Executa())
            {
                aviso = aviso + "Erro na verificação de Arranjos.\n";
                problema = true;
            }
        }
        if (dgGeral.blocoIsolado)
        { // exibe progresso
            if (progresso)
                progresso->Add("Verificando blocos isolados");
            if (!(diagTopo->DGBIso)->Executa())
            {
                aviso = aviso + "Erro na verificação de Blocos Isolados.\n";
                problema = true;
            }
        }
        if (dgGeral.barra)
        { // exibe progresso
            if (progresso)
                progresso->Add("Verificando barras");
            if (!(diagTopo->DGBarra)->Executa())
            {
                aviso = aviso + "Erro na verificação de Barras.\n";
                problema = true;
            }
        }
        if (dgGeral.cabo)
        { // exibe progresso
            if (progresso)
                progresso->Add("Verificando cabos");
            if (!(diagTopo->DGCabo)->Executa())
            {
                aviso = aviso + "Erro na verificação de Cabos.\n";
                problema = true;
            }
        }
        if (dgGeral.capacitor)
        { // exibe progresso
            if (progresso)
                progresso->Add("Verificando capacitores");
            if (!(diagTopo->DGCapacitor)->Executa())
            {
                aviso = aviso + "Erro na verificação de Capacitores.\n";
                problema = true;
            }
        }
        if (dgGeral.carga)
        { // exibe progresso
            if (progresso)
                progresso->Add("Verificando cargas");
            if (!(diagTopo->DGCarga)->Executa())
            {
                aviso = aviso + "Erro na verificação de Cargas.\n";
                problema = true;
            }
        }
        if (dgGeral.chave)
        { // exibe progresso
            if (progresso)
                progresso->Add("Verificando chaves");
            if (!(diagTopo->DGChave)->Executa())
            {
                aviso = aviso + "Erro na verificação de Chaves.\n";
                problema = true;
            }
        }
        if (dgGeral.rede)
        { // exibe progresso
            if (progresso)
                progresso->Add("Verificando " "rede" " ");
            if (!(diagTopo->DGRede)->Executa())
            {
                aviso = aviso + "Erro na verificação de Redes.\n";
                problema = true;
            }
        }
        if (dgGeral.reg)
        { // exibe progresso
            if (progresso)
                progresso->Add("Verificando " "regulador" " ");
            if (!(diagTopo->DGReg)->Executa())
            {
                aviso = aviso + "Erro na verificação de Reguladores.\n";
                problema = true;
            }
        }
        if (dgGeral.trecho)
        { // exibe progresso
            if (progresso)
                progresso->Add("Verificando trechos");
            if (!(diagTopo->DGTrecho)->Executa())
            {
                aviso = aviso + "Erro na verificação de Trechos.\n";
                problema = true;
            }
        }
        if (dgGeral.trafo)
        { // exibe progresso
            if (progresso)
                progresso->Add("Verificando trafos");
            if (!(diagTopo->DGTrafo)->Executa())
            {
                aviso = aviso + "Erro na verificação de Trafos.\n";
                problema = true;
            }
        }
        PreencheLogGeral();
        // esconde progresso
        if (progresso)
        {
            progresso->Stop();
        }
        break;

    case modoARRANJO:
        if (!ExecutaDiagEqpto(diagTopo->DGArranjo))
        {
            aviso = aviso + "Erro na verificação de Arranjos.\n";
            problema = true;
        }
        break;

    case modoBARRA:
        if (!ExecutaDiagEqpto(diagTopo->DGBarra))
        {
            aviso = aviso + "Erro na verificação de Barra.\n";
            problema = true;
        }
        break;

    case modoBLOCOISOLADO:
        if (!ExecutaDiagEqpto(diagTopo->DGBIso))
        {
            aviso = aviso + "Erro na verificação de Blocos Isolados.\n";
            problema = true;
        }
        break;

    case modoCABO:
        if (!ExecutaDiagEqpto(diagTopo->DGCabo))
        {
            aviso = aviso + "Erro na verificação de Cabos.\n";
            problema = true;
        }
        break;

    case modoCAPACITOR:
        if (!ExecutaDiagEqpto(diagTopo->DGCapacitor))
        {
            aviso = aviso + "Erro na verificação de Capacitores.\n";
            problema = true;
        }
        break;

    case modoCARGA:
        if (!ExecutaDiagEqpto(diagTopo->DGCarga))
        {
            aviso = aviso + "Erro na verificação de Cargas.\n";
            problema = true;
        }
        break;

    case modoCHAVE:
        if (!ExecutaDiagEqpto(diagTopo->DGChave))
        {
            aviso = aviso + "Erro na verificação de Chaves.\n";
            problema = true;
        }
        break;

    case modoREDE:
        if (!ExecutaDiagEqpto(diagTopo->DGRede))
        {
            aviso = aviso + "Erro na verificação de Redes.\n";
            problema = true;
        }
        break;

    case modoREG:
        if (!ExecutaDiagEqpto(diagTopo->DGReg))
        {
            aviso = aviso + "Erro na verificação de Reguladores.\n";
            problema = true;
        }
        break;

    case modoTRECHO:
        if (!ExecutaDiagEqpto(diagTopo->DGTrecho))
        {
            aviso = aviso + "Erro na verificação de Trechos.\n";
            problema = true;
        }
        break;

    case modoTRAFO:
        if (!ExecutaDiagEqpto(diagTopo->DGTrafo))
        {
            aviso = aviso + "Erro na verificação de Trafo.\n";
            problema = true;
        }
        break;

    default:

        ;
    }
    // aviso caso não haja ocorrências
    if (!problema)
    {
        if (lvLog->Items->Count == 0)
        {
            Aviso("Não houve ocorrências.");
        }
    }
    // //deleta progresso novo
    // if (progresso && progressoNovo)
    // {
    // delete progresso;
    // progresso = NULL;
    // }
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::ActionExibeArranjoExecute(TObject *Sender)
{
	// salva as opcoes marcadas do checklist
	LeCheckList();
	// muda o modo
	modoForm = modoARRANJO;
	AlteraStatusToolButton(tbutArranjo);
	AlteraStatusConfigButton(modoForm);
	AtualizaForm(diagTopo->DGArranjo);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::ActionExibeBarraExecute(TObject *Sender)
{
	// salva as opcoes marcadas do checklist
	LeCheckList();
	// muda o modo
	modoForm = modoBARRA;
	AlteraStatusToolButton(tbutBarra);
	AlteraStatusConfigButton(modoForm);
	AtualizaForm(diagTopo->DGBarra);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::ActionExibeBIsoExecute(TObject *Sender)
{
	// salva as opcoes marcadas do checklist
	LeCheckList();
	// muda o modo
	modoForm = modoBLOCOISOLADO;
	AlteraStatusToolButton(tbutBIso);
	AlteraStatusConfigButton(modoForm);
	AtualizaForm(diagTopo->DGBIso);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::ActionExibeCaboExecute(TObject *Sender)
{
	// salva as opcoes marcadas do checklist
	LeCheckList();
	// muda o modo
	modoForm = modoCABO;
	AlteraStatusToolButton(tbutCabo);
	AlteraStatusConfigButton(modoForm);
	AtualizaForm(diagTopo->DGCabo);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::ActionExibeCapacitorExecute(TObject *Sender)
{
	// salva as opcoes marcadas do checklist
	LeCheckList();
	// muda o modo
	modoForm = modoCAPACITOR;
	AlteraStatusToolButton(tbutCapacitor);
	AlteraStatusConfigButton(modoForm);
	AtualizaForm(diagTopo->DGCapacitor);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::ActionExibeCargaExecute(TObject *Sender)
{
	// salva as opcoes marcadas do checklist
	LeCheckList();
	// muda o modo
	modoForm = modoCARGA;
	AlteraStatusToolButton(tbutCarga);
	AlteraStatusConfigButton(modoForm);
	AtualizaForm(diagTopo->DGCarga);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::ActionExibeChaveExecute(TObject *Sender)
{
	// salva as opcoes marcadas do checklist
	LeCheckList();
	// muda o modo
	modoForm = modoCHAVE;
	AlteraStatusToolButton(tbutChave);
	AlteraStatusConfigButton(modoForm);
	AtualizaForm(diagTopo->DGChave);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::ActionExibeTrechoExecute(TObject *Sender)
{
	// salva as opcoes marcadas do checklist
	LeCheckList();
	// muda o modo
	modoForm = modoTRECHO;
	AlteraStatusToolButton(tbutTrecho);
	AlteraStatusConfigButton(modoForm);
	AtualizaForm(diagTopo->DGTrecho);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::ActionExibeTrafoExecute(TObject *Sender)
{
	// salva as opcoes marcadas do checklist
	LeCheckList();
	// muda o modo
	modoForm = modoTRAFO;
	AlteraStatusToolButton(tbutTrafo);
	AlteraStatusConfigButton(modoForm);
	AtualizaForm(diagTopo->DGTrafo);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::ActionExibeRedeExecute(TObject *Sender)
{
	// salva as opcoes marcadas do checklist
	LeCheckList();
	// muda o modo
	modoForm = modoREDE;
	AlteraStatusToolButton(tbutRede);
	AlteraStatusConfigButton(modoForm);
	AtualizaForm(diagTopo->DGRede);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
void __fastcall TFormDiagEqpto::ActionExibeRegExecute(TObject *Sender)
{
    // salva as opcoes marcadas do checklist
    LeCheckList();
    // muda o modo
    modoForm = modoREG;
    AlteraStatusToolButton(tbutReg);
    AlteraStatusConfigButton(modoForm);
    AtualizaForm(diagTopo->DGReg);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::ActionExibeGeralExecute(TObject *Sender)
{
	// salva as opcoes marcadas do checklist
	LeCheckList();
	// muda o modo
	modoForm = modoGERAL;
	AlteraStatusToolButton(tbutGeral);
	AlteraStatusConfigButton(modoForm);
	IniciaCheckListGeral();
	// esconde o informações adicionais
	gbInfo->Visible = false;
	// atualiza o listview
	PreencheLogGeral();
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
void __fastcall TFormDiagEqpto::ActionExportaRelExecute(TObject *Sender)
{
    // variáveis locais
    TStringList *log = NULL;

    // seleciona o log a ser exportado
    switch (modoForm)
    {
    case modoGERAL:
        /* TODO : pensar em um log do geral e como exportá-lo */
        break;

    case modoARRANJO:
        log = (diagTopo->DGArranjo)->Log;
        break;

    case modoBARRA:
        log = (diagTopo->DGBarra)->Log;
        break;

    case modoBLOCOISOLADO:
        log = (diagTopo->DGBIso)->Log;
        break;

    case modoCABO:
        log = (diagTopo->DGCabo)->Log;
        break;

    case modoCAPACITOR:
        log = (diagTopo->DGCapacitor)->Log;
        break;

    case modoCARGA:
        log = (diagTopo->DGCarga)->Log;
        break;

    case modoCHAVE:
        log = (diagTopo->DGChave)->Log;
        break;

    case modoREDE:
        log = (diagTopo->DGRede)->Log;
        break;

    case modoREG:
        log = (diagTopo->DGReg)->Log;
        break;

    case modoTRAFO:
        log = (diagTopo->DGTrafo)->Log;
        break;

    case modoTRECHO:
        log = (diagTopo->DGTrecho)->Log;
        break;
    default: ;
    }
    if (log)
    {
        ExportaRel(log);
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::ActionHelpExecute(TObject *Sender)
{ // variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Diagnóstico_de_Equipamentos");
	if (formHelp)
		formHelp->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::ActionZoomExecute(TObject *Sender)
{ // variáveis locais
	VTEqpto *eqpto;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TListItem *item;
	// TList       *listLigacoes;
	DIAG_ISOLADO *blocoIsolado;

	try
	{
		// toma uma ação baseada no modo do form
		switch (modoForm)
		{
		case modoGERAL:
			// nada a fazer
			break;

		case modoBLOCOISOLADO:
			item = lvLog->Selected;
			if(item == NULL)
		{
			break;
		}
		else
		{
			blocoIsolado = (DIAG_ISOLADO*)item->Data;
			// protecao
			if (blocoIsolado->lisBarras->Count <= 0)
				break;
			// dá zoom na 1a barra da lista de barras do bloco isolado
			eqpto = (VTEqpto*)blocoIsolado->lisBarras->First();
			grafico->SelecionaZoom(eqpto);
			break;
		}

		default: // caso não seja rede ou iso
			// determina Eqpto ,
			item = lvLog->Selected;
            if(item == NULL)
			{
				break;
			}
			else
			{
				eqpto = (VTEqpto*) item->Data;
				// seleciona área de zoom p/ destacar o Eqpto
				grafico->SelecionaZoom(eqpto);
				break;
			}
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::AlteraStatusToolButton(TToolButton *toolButton)
{
	for (int nb = 0; nb < ToolBarSelecao->ButtonCount; nb++)
	{
		ToolBarSelecao->Buttons[nb]->Down = false;
	}
	toolButton->Down = true;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
void __fastcall TFormDiagEqpto::AlteraStatusConfigButton(int modoFORM)
{
    // esconde o panelCor
    PanelCor->Visible = false;
    // habilita o botão config só para os eqptos que possuem form de configuração
    switch (modoFORM)
    {
    case modoTRECHO:
    case modoCHAVE:
    case modoCABO:
    case modoCAPACITOR:
    case modoTRAFO:
    case modoCARGA:
        tbConfigLimites->Enabled = true;
        break;
    case modoBLOCOISOLADO:
        tbConfigLimites->Enabled = true;
        PanelCor->Visible = true;
        break;
    default:
        tbConfigLimites->Enabled = false;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::AtualizaForm(VTDiagEqpto *diagEqpto)
{ // variaveis locais
	// VTCronometro *timer = DLL_NewObjCronometro(apl);
	// VTGrafico   *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	// VTProgresso  *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

	// insere os items do checklist
	diagEqpto->IniciaCheckList(CheckListBox1);
	// caso o diagnostico para este eqpto tenha sido executado...
	if (diagEqpto->Executado)
	{
		// timer->Start("mostra");
		// mostra o progresso
		if (progresso)
			progresso->Start(1);
		// esconde o listView para melhorar
		lvLog->Visible = false;
		// preenche o listView com o log desse eqpto
		diagEqpto->PreencheListView(lvLog);
		// exibe o groupbox informações adicionais
		gbInfo->Visible = diagEqpto->InfoExtra;
		// caso haja ocorrências habilita o exporta rel
		if (lvLog->Items->Count > 0)
			butExportRel->Enabled = true;
		else
		{
			butExportRel->Enabled = false;
		}
		// timer->Start("mostra");
		lvLog->Visible = true;
		// timer->Stop("mostra");
		// timer->Show();
		// mostra o progresso
		if (progresso)
			progresso->Stop();
	}
	else
	{
		lvLog->Items->BeginUpdate();
		lvLog->Clear();
		lvLog->Columns->Clear();
		lvLog->Items->EndUpdate();
		gbInfo->Visible = false;
		butExportRel->Enabled = false;
	}
	// remove molduras/destaques
	AtualizaGrafico();
	// if(timer){delete timer; timer = NULL;}
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::AtualizaGrafico(void)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// retira a moldura dos eqptos
	grafico->MolduraSolida();
	grafico->Moldura();
	// retira o destaque
	grafico->DestacaEqpto(emptyList, clBlack);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::AutoSizeTListViewColumns(TListView * ListView)
{ // variaveis locais
	// const int teste = ListView->Columns->Count;
	int wid[20];
	AnsiString aviso;

	ListView->Items->BeginUpdate();
	for (int index = 0; index < ListView->Columns->Count; ++index)
	{
		ListView->Columns->Items[index]->Width = ColumnTextWidth;
		ListView->Columns->Items[index]->Width = ColumnHeaderWidth;
		wid[index] = ListView->Columns->Items[index]->Width;
		aviso = aviso + IntToStr(wid[index]) + " ";
	}
	ListView->Items->EndUpdate();
	// Aviso(aviso);
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::butSelAllClick(TObject *Sender)
{ // marca todos os items do CLBox
	for (int n = 0; n < CheckListBox1->Items->Count; n++)
	{
		CheckListBox1->Checked[n] = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::butSelNoneClick(TObject *Sender)
{ // marca todos os items do CLBox
	for (int n = 0; n < CheckListBox1->Items->Count; n++)
	{
		CheckListBox1->Checked[n] = false;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormDiagEqpto::ExecutaDiagEqpto(VTDiagEqpto *diagEqpto)
{ // variaveis locais
	// VTProgresso    *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
	bool executou = true;
	bool progressoNovo = false;

	// exibe progresso
	if (progresso)
		progresso->Start(progDEFAULT);
	// verifica quais opcoes o usuário habilitou
	diagEqpto->LeCheckList(CheckListBox1);
	// executa a verificação para esse eqpto
	if (diagEqpto->Executa())
	{ // preenche o listView com o log
		diagEqpto->PreencheListView(lvLog);
		// exibe o groupbox de infos adicionais
		gbInfo->Visible = diagEqpto->InfoExtra;
		// atualiza o grafico
		AtualizaGrafico();
		// atualiza o form
		AtualizaForm(diagEqpto);
	}
	else
	{
		executou = false;
	}
	// exibe progresso
	if (progresso)
		progresso->Stop();
	// if(progressoNovo) delete progresso;
	return executou;
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::ExportaRel(TStringList *log)
{ // variáveis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	FILE *FSave;
	AnsiString linha;

	// verifica se o diretório SaveDialog já foi definido
	if (SaveDialog->InitialDir.IsEmpty())
	{
		SaveDialog->InitialDir = path->DirExporta();
	}
	// executa o save dialog
	if (SaveDialog->Execute())
	{ // começa o processo de escrita
		FSave = fopen(AnsiString(SaveDialog->FileName).c_str(), "w");
		// proteção
		if (FSave == NULL)
		{
			Aviso("O arquivo não pode ser salvo.");
			return;
		}
		// escreve as linhas do log no arquivo
		for (int nl = 0; nl < log->Count; nl++)
		{
			linha = log->Strings[nl] + "\n";
			fprintf(FSave, "%s", linha.c_str());
		}
		fclose(FSave);
		Aviso("O relatório foi exportado corretamente.");
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::FormClose(TObject *Sender, TCloseAction &Action)
{
	try
	{
		AtualizaGrafico();
		Action = caFree;
	}
	catch (Exception &e)
	{
		// nada a fazer
	}
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
void __fastcall TFormDiagEqpto::IniciaCheckListGeral(void)
{
    CheckListBox1->Clear();
    CheckListBox1->Items->Add("Barras");
    CheckListBox1->Checked[modoBARRA] = dgGeral.barra;
    CheckListBox1->Items->Add("Trechos");
    CheckListBox1->Checked[modoTRECHO] = dgGeral.trecho;
    CheckListBox1->Items->Add("Chaves");
    CheckListBox1->Checked[modoCHAVE] = dgGeral.chave;
    CheckListBox1->Items->Add("Capacitores");
    CheckListBox1->Checked[modoCAPACITOR] = dgGeral.capacitor;
    CheckListBox1->Items->Add("Transformadores");
    CheckListBox1->Checked[modoTRAFO] = dgGeral.trafo;
    CheckListBox1->Items->Add("Reguladores");
    CheckListBox1->Checked[modoREG] = dgGeral.reg;
    CheckListBox1->Items->Add("Cargas");
    CheckListBox1->Checked[modoCARGA] = dgGeral.carga;
    CheckListBox1->Items->Add("Arranjos");
    CheckListBox1->Checked[modoARRANJO] = dgGeral.arranjo;
    CheckListBox1->Items->Add("Cabos");
    CheckListBox1->Checked[modoCABO] = dgGeral.cabo;
    CheckListBox1->Items->Add("Redes");
    CheckListBox1->Checked[modoREDE] = dgGeral.rede;
    CheckListBox1->Items->Add("Blocos Isolados");
    CheckListBox1->Checked[modoBLOCOISOLADO] = dgGeral.blocoIsolado;
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
void __fastcall TFormDiagEqpto::LeCheckList(void)
{
    // variaveis locais
    VTDiagEqpto *diagEqpto;

    switch (modoForm)
    {
    case modoGERAL:
        LeCheckListGeral();
        break;

    case modoARRANJO:
        diagEqpto = diagTopo->DGArranjo;
        // Le o checklist e salva as ateracoes no objeto
        diagEqpto->LeCheckList(CheckListBox1);
        break;

    case modoBARRA:
        diagEqpto = diagTopo->DGBarra;
        // Le o checklist e salva as ateracoes no objeto
        diagEqpto->LeCheckList(CheckListBox1);
        break;

    case modoBLOCOISOLADO:
        diagEqpto = diagTopo->DGBIso;
        // Le o checklist e salva as ateracoes no objeto
        diagEqpto->LeCheckList(CheckListBox1);
        break;

    case modoCABO:
        diagEqpto = diagTopo->DGCabo;
        // Le o checklist e salva as ateracoes no objeto
        diagEqpto->LeCheckList(CheckListBox1);
        break;

    case modoCAPACITOR:
        diagEqpto = diagTopo->DGCapacitor;
        // Le o checklist e salva as ateracoes no objeto
        diagEqpto->LeCheckList(CheckListBox1);
        break;

    case modoCARGA:
        diagEqpto = diagTopo->DGCarga;
        // Le o checklist e salva as ateracoes no objeto
        diagEqpto->LeCheckList(CheckListBox1);
        break;

    case modoCHAVE:
        diagEqpto = diagTopo->DGChave;
        // Le o checklist e salva as ateracoes no objeto
        diagEqpto->LeCheckList(CheckListBox1);
        break;

    case modoREDE:
        diagEqpto = diagTopo->DGRede;
        // Le o checklist e salva as ateracoes no objeto
        diagEqpto->LeCheckList(CheckListBox1);
        break;

    case modoREG:
        diagEqpto = diagTopo->DGReg;
        // Le o checklist e salva as ateracoes no objeto
        diagEqpto->LeCheckList(CheckListBox1);
        break;

    case modoTRAFO:
        diagEqpto = diagTopo->DGTrafo;
        // Le o checklist e salva as ateracoes no objeto
        diagEqpto->LeCheckList(CheckListBox1);
        break;

    case modoTRECHO:
        diagEqpto = diagTopo->DGTrecho;
        // Le o checklist e salva as ateracoes no objeto
        diagEqpto->LeCheckList(CheckListBox1);
        break;

    default: ;
    }
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
void __fastcall TFormDiagEqpto::LeCheckListGeral(void)
{
    dgGeral.barra        = CheckListBox1->Checked[modoBARRA];
    dgGeral.trecho       = CheckListBox1->Checked[modoTRECHO];
    dgGeral.chave        = CheckListBox1->Checked[modoCHAVE];
    dgGeral.capacitor    = CheckListBox1->Checked[modoCAPACITOR];
    dgGeral.trafo        = CheckListBox1->Checked[modoTRAFO];
    dgGeral.reg          = CheckListBox1->Checked[modoREG];
    dgGeral.carga        = CheckListBox1->Checked[modoCARGA];
    dgGeral.arranjo      = CheckListBox1->Checked[modoARRANJO];
    dgGeral.cabo         = CheckListBox1->Checked[modoCABO];
    dgGeral.rede         = CheckListBox1->Checked[modoREDE];
    dgGeral.blocoIsolado = CheckListBox1->Checked[modoBLOCOISOLADO];
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::lvLogClick(TObject *Sender)
{ // variáveis locais
	VTEqpto *eqpto;
	// VTBarra        *barra, *barraClone;
	DIAG_ISOLADO *blocoIsolado;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TListItem *item;

	try
	{ // determina Eqpto ,
		item = lvLog->Selected;
		// toma uma ação baseada no modo do form
		switch (modoForm)
		{
		case modoGERAL:
			// nada a fazer
			break;

		case modoBLOCOISOLADO:
		if(item == NULL)
		{
			break;
		}
		else
		{
			blocoIsolado = (DIAG_ISOLADO*)item->Data;
			// caso sejam poucas barras cria moldura
			if (blocoIsolado->lisBarras->Count < 4)
			{
				grafico->MolduraSolida(blocoIsolado->lisBarras);
			}
			else
			{
				grafico->MolduraSolida();
			}
			// destaca as barras e ligacoes
			grafico->DestacaEqpto(blocoIsolado->lisEqpto, PanelSelCor->Color, 3);
			break;
		}
		default: // caso não seja rede ou iso
			if(item == NULL)
			{
			break;
			}
			else
			{
			eqpto = (VTEqpto*) item->Data;
			// seleciona área de zoom p/ destacar o Eqpto
			grafico->Moldura(eqpto);
			}
		}
	}
	catch (Exception &e)
	{
		// nada a fazer
	}
}

// ---------------------------------------------------------------------------
// void __fastcall TFormDiagEqpto::lvLogColumnClick(TObject *Sender,
// TListColumn *Column)
// {  //verifica se selecionou a mesma coluna
// if (ListViewSort.ColumnIndex == Column->Index)
// {//inverte ordem
// ListViewSort.OrdemCrescente  = ! ListViewSort.OrdemCrescente;
// }
// else
// {//redefine índice da coluna usada p/ ordenação
// ListViewSort.ColumnIndex = Column->Index;
// }
// //força reordenação
// lvLog->AlphaSort();
// }

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::lvLogColumnClick(TObject *Sender, TListColumn *Column)
{
	// prepara o lvlog pra update
	SendMessage(lvLog->Handle, WM_SETREDRAW, false, 0);
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
	lvLog->AlphaSort();
	// redesenha o richedit
	SendMessage(lvLog->Handle, WM_SETREDRAW, true, 0);
	lvLog->Refresh();
	Application->ProcessMessages();
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::lvLogCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
	int Data, int &Compare)
{ // variáveis locais
	int index = ListViewSort.ColumnIndex - 1;
	// double    valor1, valor2;

	// proteção
	if ((index < 0) || (index >= lvLog->Columns->Count))
		index = 0;
	switch (ListViewSort.ColumnIndex)
	{
	case 0: // coluna 0
		Compare = CompareText(Item1->Caption, Item2->Caption);
		break;
	default: // demais colunas
		Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
		break;
	}
	// verifica se ordem crescente/decrescente
	if (!ListViewSort.OrdemCrescente)
		Compare = -Compare;
}

// ---------------------------------------------------------------------------
// void __fastcall TFormDiagEqpto::lvLogCompare(TObject *Sender, TListItem *Item1,
// TListItem *Item2,int Data, int &Compare)
// {  //variáveis locais
// int       index = ListViewSort.ColumnIndex-1;
// double    valor1, valor2;
//
// //proteção
// if ((index < 0)||(index >= lvLog->Columns->Count)) index = 0;
// switch(ListViewSort.ColumnIndex)
// {
// case 0:  //coluna 0
// if ((StrToDouble(Item1->Caption,valor1)) && (StrToDouble(Item2->Caption,valor2)))
// {
// if(IsDoubleZero(valor1 - valor2)){Compare = 0;}
// else if(valor1 < valor2){Compare = -1;}
// else if(valor1 > valor2){Compare = 1;}
// }
// else
// {
// Compare = CompareText(Item1->Caption, Item2->Caption);
// }
// break;
// default:  //demais colunas
// if ((StrToDouble(Item1->SubItems->Strings[index],valor1)) &&
// (StrToDouble(Item2->SubItems->Strings[index],valor2)))
// {
// if(IsDoubleZero(valor1 - valor2)){Compare = 0;}
// else if(valor1 < valor2){Compare = -1;}
// else if(valor1 > valor2){Compare = 1;}
// }
// else
// {
// Compare = CompareText(Item1->SubItems->Strings[index],
// Item2->SubItems->Strings[index]);
// }
// break;
// }
// //verifica se ordem crescente/decrescente
// if (! ListViewSort.OrdemCrescente) Compare = - Compare;
// }

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::lvLogDblClick(TObject *Sender)
{
	ActionZoom->Execute();
}

// ---------------------------------------------------------------------------
void __fastcall TFormDiagEqpto::PanelSelCorClick(TObject *Sender)
{ // variaveis locais
	// VTGrafico      *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	ColorDialog->Color = PanelSelCor->Color;
	if (ColorDialog->Execute())
	{
		PanelSelCor->Color = ColorDialog->Color;
		// grafico->Redraw();
		lvLogClick(this);
	}
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18567881
void __fastcall TFormDiagEqpto::PreencheLogGeral(void)
{ /* TODO : passar pra dentro do TDiagEqpto */
	// variaveis locais
	VTDiagEqpto *diagEqpto;
	TListColumn *listColumn;
	TListItem *listItem;
	TListView *listView;

	// inicializa o listView
	listView = lvLog;
	listView->Clear();
	listView->Columns->Clear();
	// adiciona as colunas
	listColumn = listView->Columns->Add();
	listColumn->Caption = "Equipamento";
	listColumn->Width = 90;
	listColumn = listView->Columns->Add();
	listColumn->Caption = "Itens verificados";
	listColumn->Width = 90;
	listColumn = listView->Columns->Add();
	listColumn->Caption = "Ocorrências";
	listColumn->Width = 90;
	// adiciona as linha de barra
	diagEqpto = diagTopo->DGBarra;
	if (diagEqpto->Executado && dgGeral.barra)
	{
		listItem = listView->Items->Add();
		listItem->Caption = diagEqpto->TipoAsString();
		listItem->SubItems->Add(diagEqpto->Verificados);
		listItem->SubItems->Add(diagEqpto->Ocorrências);
	}
	// adiciona as linha de trecho
	diagEqpto = diagTopo->DGTrecho;
	if (diagEqpto->Executado && dgGeral.trecho)
	{
		listItem = listView->Items->Add();
		listItem->Caption = diagEqpto->TipoAsString();
		listItem->SubItems->Add(diagEqpto->Verificados);
		listItem->SubItems->Add(diagEqpto->Ocorrências);
	}
	// adiciona as linha de chave
	diagEqpto = diagTopo->DGChave;
	if (diagEqpto->Executado && dgGeral.chave)
	{
		listItem = listView->Items->Add();
		listItem->Caption = diagEqpto->TipoAsString();
		listItem->SubItems->Add(diagEqpto->Verificados);
		listItem->SubItems->Add(diagEqpto->Ocorrências);
	}
	// adiciona as linha de capacitor
	diagEqpto = diagTopo->DGCapacitor;
	if (diagEqpto->Executado && dgGeral.capacitor)
	{
		listItem = listView->Items->Add();
		listItem->Caption = diagEqpto->TipoAsString();
		listItem->SubItems->Add(diagEqpto->Verificados);
		listItem->SubItems->Add(diagEqpto->Ocorrências);
	}
	// adiciona as linha de trafo
	diagEqpto = diagTopo->DGTrafo;
	if (diagEqpto->Executado && dgGeral.trafo)
	{
		listItem = listView->Items->Add();
		listItem->Caption = diagEqpto->TipoAsString();
		listItem->SubItems->Add(diagEqpto->Verificados);
		listItem->SubItems->Add(diagEqpto->Ocorrências);
	}
	// adiciona as linha de regulador
	diagEqpto = diagTopo->DGReg;
	if (diagEqpto->Executado && dgGeral.reg)
	{
		listItem = listView->Items->Add();
		listItem->Caption = diagEqpto->TipoAsString();
		listItem->SubItems->Add(diagEqpto->Verificados);
		listItem->SubItems->Add(diagEqpto->Ocorrências);
	}
	// adiciona as linha de carga
	diagEqpto = diagTopo->DGCarga;
	if (diagEqpto->Executado && dgGeral.carga)
	{
		listItem = listView->Items->Add();
		listItem->Caption = diagEqpto->TipoAsString();
		listItem->SubItems->Add(diagEqpto->Verificados);
		listItem->SubItems->Add(diagEqpto->Ocorrências);
	}
	// adiciona as linha de arranjo
	diagEqpto = diagTopo->DGArranjo;
	if (diagEqpto->Executado && dgGeral.arranjo)
	{
		listItem = listView->Items->Add();
		listItem->Caption = diagEqpto->TipoAsString();
		listItem->SubItems->Add(diagEqpto->Verificados);
		listItem->SubItems->Add(diagEqpto->Ocorrências);
	}
	// adiciona as linha de cabo
	diagEqpto = diagTopo->DGCabo;
	if (diagEqpto->Executado && dgGeral.cabo)
	{
		listItem = listView->Items->Add();
		listItem->Caption = diagEqpto->TipoAsString();
		listItem->SubItems->Add(diagEqpto->Verificados);
		listItem->SubItems->Add(diagEqpto->Ocorrências);
	}
	// adiciona as linha de "Rede"
	diagEqpto = diagTopo->DGRede;
	if (diagEqpto->Executado && dgGeral.rede)
	{
		listItem = listView->Items->Add();
		listItem->Caption = diagEqpto->TipoAsString();
		listItem->SubItems->Add(diagEqpto->Verificados);
		listItem->SubItems->Add(diagEqpto->Ocorrências);
	}
	// adiciona as linha de blocos isolados
	diagEqpto = diagTopo->DGBIso;
	if (diagEqpto->Executado && dgGeral.blocoIsolado)
	{
		listItem = listView->Items->Add();
		listItem->Caption = diagEqpto->TipoAsString();
		listItem->SubItems->Add(diagEqpto->Verificados);
		listItem->SubItems->Add(diagEqpto->Ocorrências);
	}
}

//---------------------------------------------------------------------------

