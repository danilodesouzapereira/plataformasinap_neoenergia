// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEstimaDemanda.h"
#include "TMontaMedicaoChave.h"
#include "TZip.h"
#include "VTEstimaDemanda.h"
#include "TExportaCarga.h"
#include "..\Apl\VTApl.h"
#include "..\Copia\VTCopia.h"
#include "..\Cronometro\VTCronometro.h"
#include "..\Editor\VTEditor.h"
#include "..\Estimador\VTEstimador.h"
#include "..\Flow\VTFlow.h"
#include "..\Grafico\VTGrafico.h"
#include "..\ImportaMedicao\VTImportaMedicao.h"
#include "..\ImportaMedicao\VTMedicoes.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Radial\VTRadial.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\SisLin\VTSisLin.h"
#include "..\Stk\VTStkCarga.h"
#include "..\Stk\VTStkPatamar.h"
#include "..\Topologia\VTTopologia.h"
#include "..\..\DLL_Inc\Copia.h"
#include "..\..\DLL_Inc\Cronometro.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Equivalente.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\ImportaMedicao.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Radial.h"
#include "..\..\DLL_Inc\Stk.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormEstimaDemanda::TFormEstimaDemanda(TComponent *Owner, VTApl *apl_owner,
	TWinControl *parent) : TFormEdt(Owner, parent)
{
	// vari�veis locais
	VTSinapChild *sinap_child = (VTSinapChild*)apl_owner->GetObject(__classid(VTSinapChild));
	VTRadial *radial;
	VTRedes *redes = (VTRedes*)apl_owner->GetObject(__classid(VTRedes));

	// salva ponteiro dos objetos
	if (parent)
		Parent = parent;
	// cria objeto Apl pr�prio
	apl = NewObjApl(this, apl_owner);
	// cria e inicia objeto Radial
	apl->Add(radial = DLL_NewObjRadial(apl));
	radial->Inicia(redes);
	// cria demais objetos e insere no apl
	apl->Add(DLL_NewObjEditor(apl));
	apl->Add(importa_medicao = DLL_NewObjImportaMedicao(this, apl));
	apl->Add(medicoes = DLL_NewObjMedicoes(apl));
	apl->Add(cronometro = DLL_NewObjCronometro(apl));
	// apl->Add(estima          = NewObjEstimaDemanda(apl_owner));
	files = new TStringList();
	lisEQP = new TList();
	// torna todos os Medidores vis�veis
	ExibeMedidores(true);
	// exibe gr�fico de topologia
	sinap_child->ActionTopo->Execute();
	// inicia CBoxPatamar
	CBoxPatamarInicia();
	// atualiza LView
	LViewInicia();

	// Inicializa��o
	FatMultMedChave = 1;
	EditFatMultMedChave->Text = AnsiString(FatMultMedChave);
	FormEqv = NULL;
	//RadioGroupEstimacao->Visible = LicencaMaster();
	//default Copel: estimador equilibrado
	RadioGroupEstimacao->ItemIndex = LicencaCopel();
	RadioGroupMetodoEE->ItemIndex = 1; // Default: Givens COM ordena��o de linhas
	estima_executado = false;
	//master
	ButTeste->Visible = LicencaMaster();
	CheckBoxHorasAfetadas->Visible = LicencaMaster();
}

// ---------------------------------------------------------------------------
__fastcall TFormEstimaDemanda::~TFormEstimaDemanda(void)
{
	// vari�veis locais
	TForm *form;
	VTEqpto *eqpto = NULL;

	// limpa moldura
	Moldura(eqpto);
	// destroi redes_original
	if (redes_original)
	{
		delete redes_original;
		redes_original = NULL;
	}
	// destr�i Forms
	if (FormEqv)
	{
		delete FormEqv;
		FormEqv = NULL;
	}
	if (FormEqvWise)
	{
		delete FormEqvWise;
		FormEqvWise = NULL;
	}
	// torna todos os Medidores n�o vis�veis
	ExibeMedidores(false);
	// destr�i objetos
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
	if (files)
	{
		delete files;
		files = NULL;
	}
	// destr�i listas sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ActionEdtExecute(TObject *Sender)
{
	// variaveis locais
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

	// editor de altera��es
	// editor->EditaAltera(NULL, NULL, ActionRedeChanged);
	editor->EditaAltera(ActionMouseOff, ActionMouseOn, ActionRedeChanged);
	AtualizaIndicacaoEditorOn(butEdt);
	ButEdtMedicao->Down = (!butEdt->Down);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ActionEdtMedicaoExecute(TObject *Sender)
{
	// variaveis locais
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

	// editor de Medi��es
	// editor->EditaMedicao(NULL, NULL, ActionRedeChanged);
	editor->EditaMedicao(ActionMouseOff, ActionMouseOn, ActionRedeChanged);
	AtualizaIndicacaoEditorOn(ButEdtMedicao);
	butEdt->Down = (!ButEdtMedicao->Down);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14300069
// https://sinapsisenergia.teamwork.com/#tasks/15372453
void __fastcall TFormEstimaDemanda::ActionExecutaExecute(TObject *Sender)
{
	// vari�veis locais
	bool sucesso;
	VTProgresso *progresso;

	// prote��o: tipo de estima��o
	if (RadioGroupEstimacao->ItemIndex < 0)
	{
		Aviso("Selecione o tipo de estima��o");
		return;
	}
	// prote��o: patamar
	if (CBoxPatamar->ItemIndex < 0)
	{
		Aviso("Selecione o patamar para estima��o");
		return;
	}
	// prote��o: tipo de relat�rio
	if (RadioGroupRelatorio->ItemIndex < 0)
	{
		Aviso("Selecione o tipo de relat�rio");
		return;
	}
	// prote��o:verifica o m�todo de Estima��o de Estado
	if (RadioGroupMetodoEE->ItemIndex < 0)
	{
		Aviso("Selecione o m�todo de estima��o de estado");
		return;
	}
	// prote��o: redu��o de rede
	if (RadioGroupReducao->ItemIndex < 0)
	{
		Aviso("Selecione op��o de otimiza��o");
		return;
	}
	// Atualiza fator multiplicativo da medi��o de chaves
	FatMultMedChave = StrToDouble(EditFatMultMedChave->Text);
	// JCG - 2018.12.01 - grava��o de arquivo com demandas das cargas
	// TESTE_GravaArquivoCarga("EstimaDemanda_CargasAntes.csv");
	// verifica se ha redes reticuladas (cluster): nao pode ser por rede primaria
	if ((RadioGroupRede->ItemIndex == 1) && (ExisteReticulado()))
	{
		Aviso("Selecione o tipo de an�lise: Rede �nica");
		return;
	}
	// verifica se ha medi��es suficientes para estimar demanda
	if (!ValidaMedidoresSuprimentos())
	{
		if (Confirma
			("Para uma estima��o confi�vel, � imprescind�vel que todos os pontos de entrada "
			"de energia na rede possuam medi��o. \nNesta rede, h� pontos de suprimento sem medi��o; a estima��o "
			"poder� at� ser calculada, mas a solu��o n�o refletir� corretamente o estado da rede."
			, "Deseja continuar assim mesmo ?") != idYes)
			return;
	}
	// verifica se foi selecionada op��o equilibrado
	if (RadioGroupEstimacao->ItemIndex == 0)
	{
		if (Confirma
			("O estimador equilibrado altera a fase das cargas que n�o forem trif�sicas."
			, "Deseja continuar ?") != idYes)
			return;
	}
	// verifica se o estimador j� foi executado anteriormente
	if (estima_executado)
	{
		if (Confirma("O estimador de demanda j� foi executado anteriormente",
			"Deseja executar novamente a partir da rede original ?") == IDYES)
			RestauraRedesOriginal();
	}
	else
	{// duplica redes original
		DuplicaRedesOriginal();
	}
	// inicia cronometro
	cronometro->Reset();
	cronometro->Start("Estimador");
	// cria progresso
	progresso = DLL_NewObjProgresso(NULL, this);
	progresso->Start(progDEFAULT);
	// prote��o
	switch (RadioGroupRede->ItemIndex)
	{
	case 0:
		sucesso = EstimaDemandaRedeUnica();
		break;
	case 1:
		sucesso = EstimaDemandaPorRede();
		break;
	default:
		Aviso("Selecione op��o estima��o de rede �nica ou por rede");
		progresso->Stop();
		return;
	}
	// fecha progresso
	progresso->Stop();
	delete progresso;
	// finaliza rel�gio
	cronometro->Stop("Estimador");
	if (sucesso)
	{
		Aviso("Estima��o de demanda executada corretamente");
		estima_executado = true;
	}
	else
	{
		Aviso("Erro na execu��o de estima��o de demanda");
		estima_executado = false;
		// substitui redes do apl pelo redes_original
		RestauraRedesOriginal();
	}
	//atualiza LView
	LViewInicia();
	// if (Confirma("Estima��o de demanda executada corretamente",
	// "Deseja visualizar o tempo de execu��o ?") == IDYES)
	// {
	// cronometro->Show();
	// }
	// }
	// else
	// {
	// Aviso("Erro na execu��o de estima��o de demanda");
	// }
	// JCG - 2018.12.01 - grava��o de arquivo com demandas das cargas
	// TESTE_GravaArquivoCarga("EstimaDemanda_CargasDepois.csv");
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ActionFechaExecute(TObject *Sender)
{
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ActionHelpExecute(TObject *Sender)
{
	// variaveis locais
	TForm *FormHelp;

	FormHelp = DLL_NewFormHelp(this, apl, "Estima��o_de_Demanda");
	if (FormHelp)
		FormHelp->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ActionImportaMedicaoExecute(TObject *Sender)
{
	// seleciona arquivo de medi��o
	if (!SelecionaArquivo())
		return;
	// importa arquivo de medi��o
	if (ImportaMedicao())
	{ // atualiza LView
		LViewInicia();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ActionTeste_CriaMedicaoIExecute(TObject *Sender)
{
	// vari�veis locais
	TMontaMedicaoChave *monta;
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// prote��o
	if (RadioGroupEstimacao->ItemIndex < 0)
	{
		Aviso("Selecione o tipo de estima��o");
		return;
	}
	// se necess�rio, executa o flxo
	flow->TipoRelatorio = relNENHUM; // HPS, 2016.08.28
	flow->CalculaPorRede(redes);

	// Atualiza fator multiplicativo da medi��o de chaves
	FatMultMedChave = StrToDouble(EditFatMultMedChave->Text);

	// Monta medi��o de chaves
	monta = new TMontaMedicaoChave(apl);
	monta->DJ_Enabled = true;
	monta->RA_Enabled = true;
	monta->FatMultMedChave = FatMultMedChave;
	// a medi��o de corrente sempre � por fase (desequilibrada)
	monta->Executa(medicaoI, true);
	delete monta;
	// atualiza tela
	LViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ActionTeste_CriaMedicaoI_ChaveRAExecute(TObject *Sender)
{
	// vari�veis locais
	TMontaMedicaoChave *monta;
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// prote��o
	if (RadioGroupEstimacao->ItemIndex < 0)
	{
		Aviso("Selecione o tipo de estima��o");
		return;
	}
	// se necess�rio, executa o flxo
	flow->TipoRelatorio = relNENHUM; // HPS, 2016.08.28
	flow->CalculaPorRede(redes);

	// Atualiza fator multiplicativo da medi��o de chaves
	FatMultMedChave = StrToDouble(EditFatMultMedChave->Text);

	// Monta medi��o de corrente apenas para RA
	monta = new TMontaMedicaoChave(apl);
	monta->DJ_Enabled = false;
	monta->RA_Enabled = true;
	monta->FatMultMedChave = FatMultMedChave;
	// a medi��o de corrente sempre � por fase (desequilibrada)
	monta->Executa(medicaoI, true);
	delete monta;
	// atualiza tela
	LViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ActionTeste_CriaMedicaoPQExecute(TObject *Sender)
{
	// vari�veis locais
	TMontaMedicaoChave *monta;
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// prote��o
	if (RadioGroupEstimacao->ItemIndex < 0)
	{
		Aviso("Selecione o tipo de estima��o");
		return;
	}
	// se necess�rio, executa o fluxo
	flow->TipoRelatorio = relNENHUM; // HPS, 2016.08.28
	flow->CalculaPorRede(redes);

	// Atualiza fator multiplicativo da medi��o de chaves
	FatMultMedChave = StrToDouble(EditFatMultMedChave->Text);

	// Monta medi��o de chaves
	monta = new TMontaMedicaoChave(apl);
	monta->FatMultMedChave = FatMultMedChave;
	monta->Executa(medicaoPQ, (RadioGroupEstimacao->ItemIndex == 1));
	delete monta;
	// atualiza tela
	LViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ActionTeste_CriaMedicaoVExecute(TObject *Sender)
{
	// vari�veis locais
	TMontaMedicaoChave *monta;
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// prote��o
	if (RadioGroupEstimacao->ItemIndex < 0)
	{
		Aviso("Selecione o tipo de estima��o");
		return;
	}
	// se necess�rio, executa o fluxo
	flow->TipoRelatorio = relNENHUM; // HPS, 2016.08.28
	flow->CalculaPorRede(redes);

	// Atualiza fator multiplicativo da medi��o de chaves
	FatMultMedChave = StrToDouble(EditFatMultMedChave->Text);

	// Monta medi��o de chaves
	monta = new TMontaMedicaoChave(apl);
	monta->FatMultMedChave = FatMultMedChave;
	monta->Executa(medicaoV, RadioGroupEstimacao->ItemIndex == 1);
	delete monta;
	// atualiza tela
	LViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ActionTeste_RedeWiseExecute(TObject *Sender)
{
	if (FormEqvWise == NULL)
		FormEqvWise = DLL_NewFormEqvWise(this, apl, Parent);
	FormEqvWise->Show();
	// delete form;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ActionTeste_RedeZipExecute(TObject *Sender)
{
	// vari�veis locais
	TZip *zip;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

	try
	{ // cria objeto
		zip = new TZip(apl);
		if (zip->RetiraChavesSemMedicao())
		{
			zip->FundeTrechos();
		}
		delete zip;
		topologia->Reconfigura();
		grafico->AreaRedeDefine();
	}
	catch (Exception &e)
	{
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ActionTeste_RedutorExecute(TObject *Sender)
{
	if (FormEqv == NULL)
		FormEqv = DLL_NewFormEqv(this, apl, Parent);
	FormEqv->Show();
	// delete form;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ActionTeste_ReticuladoExecute(TObject *Sender)
{
	// vari�veis locais
	// VTGrafico *grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	Aviso("Fun��o est� sendo implementada");

	try
	{ // cria objeto

		// reformata o o ring bus dos mini-reticulados

		// destr�i objeto

	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ActionTeste_RetiraChaveSemMedicaoExecute(TObject *Sender)
{
	// vari�veis locais
	TZip *zip;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// cria objeto
	zip = new TZip(apl);
	zip->RetiraChavesSemMedicao();
	zip->FundeTrechos();
	delete zip;
	Aviso("Todas as chaves sem medi��o foram retiradas");
	grafico->AreaRedeDefine();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ActionTeste_RetiraTodasChaveExecute(TObject *Sender)
{
	// vari�veis locais
	TZip *zip;

	// cria objeto
	zip = new TZip(apl);
	zip->RetiraChavesExcetoDJinicial();
	delete zip;
	Aviso("Todas as chaves foram retiradas (exceto os disjuntores)");
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ActionTeste_RetiraMedicaoExecute(TObject *Sender)
{
	// vari�veis locais
	TMontaMedicaoChave *monta;

	// cria objeto
	monta = new TMontaMedicaoChave(apl);
	monta->RetiraMedicao();
	delete monta;

	// atualiza tela
	LViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ActionTeste_ZeraCargaExecute(TObject *Sender)
{
	// vari�veis locais
	VTCarga *carga;
	VTStkCarga *stk;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// cria objeto VTStkCarga
	stk = DLL_NewObjStkCarga(apl);
	// obt�m lista de Cargas da rede
	lisEQP->Clear();
	redes->LisEqbar(lisEQP, eqptoCARGA);
	// zera demanda de todas as Cargas
	for (int n = 0; n < lisEQP->Count; n++)
	{
		carga = (VTCarga*)lisEQP->Items[n];
		// loop para todos Patamares
		for (int np = 0; np < patamares->NumPatamar(); np++)
		{
			stk->RedefineCargaComDemandaPropriaDesequilibrada(carga);
			stk->AlteraDemanda(carga, np, 0., 0.);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destr�i o Form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ActionRedeChangedExecute(TObject *Sender)
{
	// atualiza LView
	LViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::CBoxPatamarInicia(void)
{
	// vari�veis locais
	bool todos = true;
	VTStkPatamar *stk;

	try
	{ // cria objeto
		if ((stk = DLL_NewObjStkPatamar(apl)) != NULL)
		{ // inicia CBoxPatamar
			stk->IniciaCBox(CBoxPatamar, todos);
			// selecionar primeiro Patamar
			if (CBoxPatamar->Items->Count == 25)
				CBoxPatamar->ItemIndex = 1;
		}
		// destr�i objeto
		delete stk;
	}
	catch (Exception &e)
	{
	}
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15312678
bool __fastcall TFormEstimaDemanda::CopiaRedes1ParaRedes2(VTRedes *redes1, VTRedes *redes2)
{
	// vari�veis locais
	bool sucesso = true;
	VTCopia *copia;

	try
	{ // prote��o
		copia = DLL_NewObjCopia(apl);
		sucesso = copia->CopiaRedes1ParaRedes2(redes1, redes2);
		delete copia;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15312678
void __fastcall TFormEstimaDemanda::DuplicaRedesOriginal(void)
{
	// vari�veis locais
	VTCopia *copia;
	VTRedes *redes = (VTRedes*)apl->Owner->GetObject(__classid(VTRedes));

	try
	{ // copia
		copia = DLL_NewObjCopia(apl);
		redes_original = copia->DuplicaRedes(redes);
		delete copia;
	}
	catch (Exception &e)
	{
		if (copia != NULL)
			delete copia;
		redes_original = NULL;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEstimaDemanda::EstimaDemandaPorRede(void)
{
	// vari�veis locais
	bool sucesso;
	int nmax;
	AnsiString txt;
	TStrings *lines;
	VTEstimaDemanda *estima;
	VTPatamar *patamar = (VTPatamar*)CBoxPatamar->Items->Objects[CBoxPatamar->ItemIndex];
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try
	{ // cria objeto EstimaDemanda
		estima = NewObjEstimaDemanda(apl);
		estima->FatMultMedChave = FatMultMedChave;
		// configura tipo de estima��o
		if (RadioGroupEstimacao->ItemIndex == 0)
			estima->TipoEstimacao = estimacaoEQUILIBRADA;
		else
			estima->TipoEstimacao = estimacaoDESEQUILIBRADA;
		// configura op��o de redu��o
		switch (RadioGroupReducao->ItemIndex)
		{
		case 0:
			estima->TipoReducao = reducaoNENHUMA;
			break;
		case 1:
			estima->TipoReducao = reducaoZIP;
			break;
		case 2:
			estima->TipoReducao = reducaoEQV;
			break;
		default:
			estima->TipoReducao = reducaoZIP;
			break;
		}
		// configura tipo de relat�rio
		switch (RadioGroupRelatorio->ItemIndex)
		{
		case 0:
			estima->TipoRelatorio = relNENHUM;
			break;
		case 1:
			estima->TipoRelatorio = relRESUMIDO;
			break;
		case 2:
			estima->TipoRelatorio = relCOMPLETO;
			break;
		case 3:
			estima->TipoRelatorio = relCOMPLETO + 1;
			break;
		default:
			estima->TipoRelatorio = relNENHUM;
			break;
		}
		// Configura m�todo de estima��o de estado
		switch (RadioGroupMetodoEE->ItemIndex)
		{
		case 0:
			estima->MetodoEE          = meeQR_GIVENS;
			estima->OrdenacaoLinhasQR = qrlSEM_ORDENACAO;
			break;
		case 1:
		default:
			estima->MetodoEE          = meeQR_GIVENS;
			estima->OrdenacaoLinhasQR = qrlMENOR_NUM_ELEM_FD;
			break;
		}
		// Configura uso do VDNM (Very Dishonest Newton Method)
		if (CheckBoxVDNM->Checked)
		{
			nmax = StrToInt(EdtNumMaxMontH->Text);
			if (nmax > 0) // Valor OK
			{
				estima->UtilizaVDNM = true;
				estima->NumMaxMontH = nmax;
			}
			else // Valor inv�lido, desabilita VDNM
			{
				estima->UtilizaVDNM = false;
				estima->NumMaxMontH = 0;
			}
		}
		else
		{
			estima->UtilizaVDNM = false;
			estima->NumMaxMontH = 0;
		}
		// verifica se foram selecionados todos os patamares
		if (patamar == NULL)
		{ // configura op��o de todos patamares
			estima->HoraReferencia = -1;
			estima->HorasAfetadas = 0;
		}
		else
		{ // configura op��o de apenas 1 patamar
			estima->HoraReferencia = patamar->HoraIni;
			estima->HorasAfetadas = (CheckBoxHorasAfetadas->Checked) ? 4 : 0;
		}
		// executa Estimador
		sucesso = estima->ExecutaPorRedePrimaria(redes);
		// verifica se a estima��o n�o foi feita corretamente
		if (!sucesso)
		{ // lista redes n�o calculadas
			txt = "Redes n�o estimadas:\n";
			lines = estima->StrLisRedeNaoEstimada();
			for (int n = 0; n < lines->Count; n++)
				txt += "> " + lines->Strings[n] + "\n";
			Aviso(txt);
		}
		// destr�i objeto
		delete estima;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEstimaDemanda::EstimaDemandaRedeUnica(void)
{
	// vari�veis locais
	bool sucesso;
	int nmax;
	VTEstimaDemanda *estima;
	VTPatamar *patamar = (VTPatamar*)CBoxPatamar->Items->Objects[CBoxPatamar->ItemIndex];
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try
	{ // cria objeto EstimaDemanda
		estima = NewObjEstimaDemanda(apl);
		estima->FatMultMedChave = FatMultMedChave;
		// configura tipo de estima��o
		if (RadioGroupEstimacao->ItemIndex == 0)
			estima->TipoEstimacao = estimacaoEQUILIBRADA;
		else
			estima->TipoEstimacao = estimacaoDESEQUILIBRADA;
		// configura op��o de redu��o
		switch (RadioGroupReducao->ItemIndex)
		{
		case 0:
			estima->TipoReducao = reducaoNENHUMA;
			break;
		case 1:
			estima->TipoReducao = reducaoZIP;
			break;
		case 2:
			estima->TipoReducao = reducaoEQV;
			break;
		default:
			estima->TipoReducao = reducaoZIP;
			break;
		}
		// configura tipo de relat�rio
		switch (RadioGroupRelatorio->ItemIndex)
		{
		case 0:
			estima->TipoRelatorio = relNENHUM;
			break;
		case 1:
			estima->TipoRelatorio = relRESUMIDO;
			break;
		case 2:
			estima->TipoRelatorio = relCOMPLETO;
			break;
		case 3:
			estima->TipoRelatorio = relCOMPLETO + 1;
			break;
		default:
			estima->TipoRelatorio = relNENHUM;
			break;
		}
		// Configura m�todo de estima��o de estado
		switch (RadioGroupMetodoEE->ItemIndex)
		{
		case 0:
			estima->MetodoEE          = meeQR_GIVENS;
			estima->OrdenacaoLinhasQR = qrlSEM_ORDENACAO;
			break;
		case 1:
		default:
			estima->MetodoEE          = meeQR_GIVENS;
			estima->OrdenacaoLinhasQR = qrlMENOR_NUM_ELEM_FD;
			break;
		}
		// Configura VDNM (Very Dishonest Newton Method)
		if (CheckBoxVDNM->Checked)
		{
			nmax = StrToInt(EdtNumMaxMontH->Text);
			if (nmax > 0) // Valor OK
			{
				estima->UtilizaVDNM = true;
				estima->NumMaxMontH = nmax;
			}
			else // Valor inv�lido, desabilita VDNM
			{
				estima->UtilizaVDNM = false;
				estima->NumMaxMontH = 0;
			}
		}
		else
		{
			estima->UtilizaVDNM = false;
			estima->NumMaxMontH = 0;
		}
		// verifica se foram selecionados todos os patamares
		if (patamar == NULL)
		{ // configura op��o de todos patamares
			estima->HoraReferencia = -1;
			estima->HorasAfetadas = 0;
		}
		else
		{ // configura op��o de apenas 1 patamar
			estima->HoraReferencia = patamar->HoraIni;
			estima->HorasAfetadas = (CheckBoxHorasAfetadas->Checked) ? 4 : 0;
		}
		// executa Estimador
		sucesso = estima->ExecutaRedeUnica(redes);
		if (!sucesso)
		{
			Aviso("Erro na execu��o de estima��o de demanda");
		}
		// destr�i objeto
		delete estima;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::ExibeMedidores(bool visible)
{
	// vari�veis locais
	VTMedidor *medidor;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// obt�m lista de Medidores da rede
	lisEQP->Clear();
	redes->LisEqbar(lisEQP, eqptoMEDIDOR);
	// loop p/ todos Medidores
	for (int i = 0; i < lisEQP->Count; i++)
	{
		medidor = (VTMedidor*)lisEQP->Items[i];
		medidor->Show[eqp_VISIBLE] = visible;
	}
	// atualiza o gr�fico
	if (lisEQP->Count > 0)
		grafico->Redraw();
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEstimaDemanda::ExisteMedidor(TList *lisLIG)
{
	// variaveis locais
	VTLigacao *ligacao;
	bool existe = true;

	// verifica se h� alguma liga��o que n�o � eqpto de canal
	for (int n = 0; (n < lisLIG->Count) && (existe); n++)
	{
		ligacao = (VTLigacao*)lisLIG->Items[n];
		if (lisEQP->IndexOf(ligacao) < 0)
			existe = false;
	}
	return existe;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEstimaDemanda::ExisteReticulado(void)
{
	// variaveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisCLUSTER = redes->LisCluster();

	return (lisCLUSTER->Count > 0);
}

/*
 //---------------------------------------------------------------------------
 bool  __fastcall TFormEstimaDemanda::ImportaMedicao(void)
 {
 //vari�veis locais
 bool     leitura = false;
 //VTFlow   *flow   = (VTFlow*)apl->GetObject(__classid(VTFlow));
 //VTRedes  *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));

 //prote��o
 if (files->Count == 0)
 {
 Aviso("Selecione arquivos de medi��o");
 return(false);
 }
 //verifica se existem todos os arquivos selecionados
 for (int n = 0; n < files->Count; n++)
 {
 if (! FileExists(files->Strings[n]))
 {
 Aviso("N�o existe o arquivo de medi��o:\n" + files->Strings[n]);
 return(false);
 }
 }
 try{//se necess�rio, executa c�lculo de fluxo
 //flow->TipoRelatorio = relNENHUM;  // HPS, 2018.08.21
 //if (! flow->CalculaPorRede(redes))
 //   {
 //   Aviso("O c�lculo de fluxo n�o pode ser executado para a rede.\nA importa��o de medi��o n�o pode ser realizada");
 //   return(false);
 // }
 //importa lista de medi��es no arquivo
 leitura = importa_medicao->LeArquivoCurvaPadrao(files);
 //      if(leitura) medicoes->AtualizaMedidores();
 //      if(leitura) leitura = medicoes->CriaMedicoesMedidores();
 }catch(Exception &)
 {
 Aviso("Erro na leitura do arquivo de medi��o");
 return(false);
 }
 return(leitura);
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	// variaveis locais
	int result;

	// verifica se o estimador j� foi executado anteriormente
	if (estima_executado)
	{
		result = Confirma("O m�dulo Estimador de Demanda ser� fechado.",
			"Deseja efetivar a estima��o de demanda ?");
		if (result == IDNO)
		{
			RestauraRedesOriginal();
			CanClose = true;
			return;
		}
		else if (result == IDYES)
		{
			CanClose = true;
			return;
		}
		else
		{
			CanClose = false;
			return;
		}
	}
	CanClose = true;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEstimaDemanda::ImportaMedicao(void)
{
	// vari�veis locais
	bool leitura = false;
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// prote��o
	if (files->Count == 0)
	{
		Aviso("Selecione arquivos de medi��o");
		return (false);
	}
	// verifica se existem todos os arquivos selecionados
	for (int n = 0; n < files->Count; n++)
	{
		if (!FileExists(files->Strings[n]))
		{
			Aviso("N�o existe o arquivo de medi��o:\n" + files->Strings[n]);
			return (false);
		}
	}
	try
	{ // se necess�rio, executa c�lculo de fluxo
		flow->TipoRelatorio = relNENHUM; // HPS, 2018.08.21
		if (!flow->CalculaPorRede(redes))
		{
			Aviso("O c�lculo de fluxo n�o pode ser executado para a rede.\nA importa��o de medi��o n�o pode ser realizada"
				);
			return (false);
		}
		// importa lista de medi��es no arquivo
		leitura = importa_medicao->LeArquivoCurvaPadrao(files);
	}
	catch (Exception &)
	{
		Aviso("Erro na leitura do arquivo de medi��o");
		return (false);
	}
	return (leitura);
}

// ---------------------------------------------------------------------------
int __fastcall TFormEstimaDemanda::LicencaCopel(void)
{
	// variaveis locais
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca *licenca;
	int tipo = 1;//0=equilibrado, 1=desequilibrado

	if (licencas == NULL)
		return tipo;
	licenca = licencas->ExisteLicenca();
	if (licenca == NULL)
		return tipo;
	//verifica empresa
	if (licenca->Empresa == COPEL)
		tipo = 0;

    return tipo;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEstimaDemanda::LicencaMaster(void)
{
	// variaveis locais
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca *licenca;

	if (licencas == NULL)
		return false;
	licenca = licencas->ExisteLicenca();
	if (licenca == NULL)
		return false;
	return (licenca->Tipo == tipoMASTER);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::LViewClick(TObject *Sender)
{
	// vari�veis locais
	TListItem *item;
	VTCanal *canal;

	// determina medidor selecionado
	item = LView->Selected;
	// desenha moldura
	if (item && item->Data)
	{ // moldura no eqpto do medidor (se tiver)
		canal = (VTCanal*)item->Data;
		Moldura(canal->Eqpto);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::LViewInicia(void)
{
	// vari�veis locais
	TList *lisCANAL;
	TListItem *Item;
	VTCanal *canal;
	VTMedidor *medidor;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// reinicia LView
	LView->Clear();
	// obt�m lista de Medidores da rede
	lisEQP->Clear();
	redes->LisEqbar(lisEQP, eqptoMEDIDOR);
	// loop p/ todos Medidores
	for (int i = 0; i < lisEQP->Count; i++)
	{
		medidor = (VTMedidor*)lisEQP->Items[i];
		// loop p/ todos Canais do Medidor
		lisCANAL = medidor->LisCanal();
		for (int j = 0; j < lisCANAL->Count; j++)
		{
			canal = (VTCanal*)lisCANAL->Items[j];
			// insere novo ListItem
			Item = LView->Items->Add();
			Item->Data = canal;
			Item->Caption = medidor->Codigo;
			Item->SubItems->Add(canal->Codigo);
			Item->SubItems->Add(canal->TipoCanalAsString);
			Item->SubItems->Add(canal->Eqpto->TipoAsString());
			Item->SubItems->Add(canal->Eqpto->Codigo);
			Item->SubItems->Add(fases->AsStrABCN[canal->Fases]);
		}
	}

	// Atualiza "Caption" de GBoxMedicao
	GBoxMedicao->Caption = AnsiString("Medidores instalados (total ") +
		AnsiString(LView->Items->Count) + AnsiString(")");
}

// ---------------------------------------------------------------------------
void __fastcall TFormEstimaDemanda::Moldura(VTEqpto *eqpto)
{
	// vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	if (grafico)
		grafico->Moldura(eqpto);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15312678
void __fastcall TFormEstimaDemanda::RestauraRedesOriginal(void)
{
	// vari�veis locais
	VTRedes *redes = (VTRedes*)apl->Owner->GetObject(__classid(VTRedes));
	VTRedes *redes_graf;
	VTGrafico *grafico = (VTGrafico*)apl->Owner->GetObject(__classid(VTGrafico));

	try
	{ // restaura objeto VTRedes
		CopiaRedes1ParaRedes2(redes_original, redes);
		redes_graf = (VTRedes*)grafico->Apl->GetObject(__classid(VTRedes));
		grafico->Apl->Remove(redes_graf);
		grafico->Apl->Add(redes);
		grafico->AreaRedeDefine();
		grafico->Refresh();
	}
	catch (Exception &e)
	{
		// nada
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEstimaDemanda::SelecionaArquivo(void)
{
	// vari�veis locais
	bool sucesso = false;

	// esconde este Form
	Visible = false;
	// DVK 2015.09.24 try/catch para impedir que um arquivo de medi��o aberto
	// tente ser importado
	try
	{ // reinicia StringList de arquivos de medi��o
		files->Clear();

		// Filtro abaixo inserido em 2018.12.13 (HPS)
		OpenTextFileDialog->Filter =
			"Arquivos de importa��o (*.csv ou *.txt)|*.csv;*.txt|Arquivos do Carga Base (*.ccb)|*.ccb|Todos os arquivos (*.*)|*.*";
		OpenTextFileDialog->FilterIndex = 2;

		if (OpenTextFileDialog->Execute())
		{ // l� lista de arquivos de Medi��o selecionados
			files->AddStrings(OpenTextFileDialog->Files);
		}
		sucesso = true;
	}
	catch (Exception &e)
	{ // avisa o problema e reexibe este Form
		Aviso("O arquivo selecionado deve estar aberto. "
			"Feche o arquivo para importar a medi��o.");
		Visible = true;
		return (false);
	}
	// exibe este Form
	Visible = true;

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEstimaDemanda::ValidaMedidoresSuprimentos(void)
{
	// variaveis locais
	VTGerador *gerador;
	VTLigacao *ligacao;
	VTMedidor *medidor;
	VTSuprimento *suprimento;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	bool valido = true;
	TList *lisSUP = new TList();
	TList *lisLIG = new TList();
	TList *lisMED = new TList();

	// obt�m lista de Medidores da rede
	redes->LisEqbar(lisMED, eqptoMEDIDOR);
	// acumula todos eqptos dos canais dos medidores
	lisEQP->Clear();
	for (int nm = 0; nm < lisMED->Count; nm++)
	{
		medidor = (VTMedidor*)lisMED->Items[nm];
		medidor->LisEqptoDosCanais(lisEQP);
	}
	// preenche lista de suprimentos
	redes->LisEqbar(lisSUP, eqptoSUPRIMENTO);
	// verifica liga��es na barra de cada suprimento
	for (int n = 0; (n < lisSUP->Count) && (valido); n++)
	{
		suprimento = (VTSuprimento*)lisSUP->Items[n];
		lisLIG->Clear();
		redes->LisLigacao(lisLIG, suprimento->pbarra);
		// obrigatorio ter medidor associado a todas liga��es da lista
		if (!ExisteMedidor(lisLIG))
			valido = false;
	}
	if (valido)
	{ // preenche lista de geradores
		lisSUP->Clear();
		redes->LisEqbar(lisSUP, eqptoGERADOR);
		// verifica liga��es na barra de cada gerador n�o PQ
		for (int n = 0; (n < lisSUP->Count) && (valido); n++)
		{
			gerador = (VTGerador*)lisSUP->Items[n];
			if (gerador->tiposup == supPQ)
				continue;
			lisLIG->Clear();
			redes->LisLigacao(lisLIG, gerador->pbarra);
			// obrigatorio ter medidor associado a todas liga��es da lista
			if (!ExisteMedidor(lisLIG))
				valido = false;
		}
	}
	// destroi listas usadas
	delete lisSUP;
	delete lisLIG;
	delete lisMED;

	return valido;
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TFormEstimaDemanda::TESTE_GravaArquivoCarga(AnsiString filename)
 {
 //vari�veis locais
 TExportaCarga *exporta;
 VTPatamar     *patamar = (VTPatamar*)CBoxPatamar->Items->Objects[CBoxPatamar->ItemIndex];

 // executa somente em  modo _DEBUG
 // #ifdef _DEBUG

 //cria objeto TExportaCarga
 if ((exporta = new TExportaCarga(apl)) != NULL)
 {
 exporta->Executa(filename, patamar->Index);
 delete exporta;
 }
 return(true);

 // #endif
 }
 */

// ---------------------------------------------------------------------------
// eof
