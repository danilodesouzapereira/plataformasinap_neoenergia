// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TformEdtGer.h"
#include "TFormEditaCurva.h"
#include "TFormExibeCurva.h"
#include "TImportaCurva.h" //DVK 2014.08.01
#include "..\Apl\VTApl.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Diretorio\VTPath.h" //DVK 2014.08.01
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormEdtGer::TFormEdtGer(TComponent* Owner, VTApl *apl, VTGerador *pger) : TForm(Owner)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	this->pger = pger;
	// cria objetos
	lisTMP = new TList();
	// inicia dados default p/ Eqpto novo
	IniciaDadosGeradorNovo();
	// cria TFormEditaCurva p/ edição da Curva do Gerador
	FormEditaCurva = new TFormEditaCurva(this, apl, TabSheetCurva);
	FormEditaCurva->InsereColuna(indP, "P(MW)");
	FormEditaCurva->InsereColuna(indQ, "Q(MVAr)");
	FormEditaCurva->InsereColuna(indV, "V(pu)");
	FormEditaCurva->InsereColuna(indT, "Fase(graus)");
	FormEditaCurva->DefineCurva(curva.propria);
	FormEditaCurva->Show();
	// cria TFormExibeCurva p/ exibição da Curva
	FormExibeCurva = new TFormExibeCurva(this, apl, TabSheetGrafico);
	FormExibeCurva->Show();
	// inicia CBoxCurvaTipica
	CBoxCurvaTipicaInicia();
	// inicia RadioGroupTipoCurva
	RadioGroupTipoCurva->ItemIndex = (pger->curva->Tipica) ? 0 : 1;
	// posiciona o Form
	FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
__fastcall TFormEdtGer::~TFormEdtGer(void)
{ // variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// destrói objetos criados
	if (curva.propria)
	{
		delete curva.propria;
		curva.propria = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lisTMP)
	{
		delete lisTMP;
		lisTMP = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::ActionCancelaExecute(TObject *Sender)
{
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::ActionConfirmaExecute(TObject *Sender)
{
	/*
	 //lê e valida dados de planejamento
	 if (formEdtPlan)
	 {
	 if (! formEdtPlan->LeDados()) return;
	 }
	 */
	if (!LeDadosGerador())
		return;
	if (!LeDadosCurva())
		return;
	if (!LeDadosImpedancia())
		return;
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::ActionImportaExecute(TObject *Sender)
{ // DVK 2014.08.01
	// variaveis locais
	AnsiString filename;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	TImportaCurva *importa = new TImportaCurva(apl, pger->curva);

	// inicia opendialog
	OpenDialog->InitialDir = path->DirImporta();
	OpenDialog->Filter = "Arquivos texto (*.txt)|*.txt|Todos arquivos (*.*)|*.*";
	if (OpenDialog->Execute())
	{
		if (OpenDialog->Files->Count != 1)
			return;
		filename = OpenDialog->Files->Strings[0];
		if (importa->Executa(filename))
		{ // importa curva a partir do arquivo
			FormEditaCurva->DefineCurva(pger->curva);
			PageControl->ActivePage = TabSheetCurva;
		}
	}
	if (importa)
		delete importa;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::ActionRetiraExecute(TObject *Sender)
{
	ModalResult = mrNo;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15823745
void __fastcall TFormEdtGer::AtualizaForm(void)
{
	 //mostra o radiogroup dependendo do combox tipo da fonte do Gerador
	switch (CBoxFonte->ItemIndex)
	{
	case 3:  //gerEOLICA
	case 4:  //gerSOLAR
		RadioGroupTipoCurva->Visible = true;
		break;
	default:
		RadioGroupTipoCurva->Visible = false;
		break;
	}
	if (RadioGroupTipoCurva->Visible)
	{//pode ser tipica ou propria
		GBoxCurvaTipica->Visible = (RadioGroupTipoCurva->ItemIndex == 0);
		if (GBoxCurvaTipica->Visible)
		{//tipica: esconde aba Curva
			// inicia CBoxCurvaTipica
			CBoxCurvaTipicaInicia();
			TabSheetCurva->TabVisible = false;
			// determina Curva p/ Gerador
			SelecionaCurva();
		}
		else
		{//propria: mostra aba Curva
			TabSheetCurva->TabVisible = true;
        }
	}
	else
	{//só pode ser própria: mostra aba Curva e esconde opção de curva típica
		TabSheetCurva->TabVisible = true;
		GBoxCurvaTipica->Visible = false;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::CBoxCurvaTipicaChange(TObject *Sender)
{
	SelecionaCurva();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::CBoxCurvaTipicaInicia(void)
{
	// variáveis locais
	AnsiString filtro;
	VTCurva *curva;
	VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
	TList *lisCURVA = curvas->ListaCurvaTipicaGeradorFotovoltaico();

	// reinicia CBoxCurvaTipica
	CBoxCurvaTipica->Clear();
	// define string que será usado para filtrar curvas de geração eólica ou fotovoltaica
	if (pger->FonteEnergia == gerEOLICA)
		filtro = "Eólica";
	else if (pger->FonteEnergia == gerSOLAR)
		filtro = "GerFV";
	else
		return;
	// insere Curvas em CBoxCurvaTipica
	for (int n = 0; n < lisCURVA->Count; n++)
	{
		curva = (VTCurva*)lisCURVA->Items[n];
		if (curva->Codigo.AnsiPos(filtro) > 0)
		{
			CBoxCurvaTipica->Items->AddObject(curva->Codigo, curva);
		}
	}
	// verifica se o Gerador está associado a uma curva típica
	if ((pger->curva != NULL) && (pger->curva->Tipica))
	{ // seleciona Curva do Gerador
		CBoxCurvaTipica->ItemIndex = CBoxCurvaTipica->Items->IndexOfObject(pger->curva);

		// se não houver uma Curva selecionada, seleciona primeira curva
		if ((CBoxCurvaTipica->ItemIndex < 0) && (CBoxCurvaTipica->Items->Count > 0))
		{
			CBoxCurvaTipica->ItemIndex = 0;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::CBoxLigacaoChange(TObject *Sender)
{
	if (cboxLigacao->ItemIndex == 3)
	{
		GBoxFases->Visible = true;
		CBoxFases->Visible = true;
	}
	else
	{
		GBoxFases->Visible = false;
		CBoxFases->Visible = false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::CBoxFonteChange(TObject *Sender)
{
	// atualiza tipo da fonte do Gerador
	switch (CBoxFonte->ItemIndex)
	{
	case 0:
		pger->FonteEnergia = gerDIESEL;
		break;
	case 1:
		pger->FonteEnergia = gerPCH;
		break;
	case 2:
		pger->FonteEnergia = gerVAPOR;
		break;
	case 3:
		pger->FonteEnergia = gerEOLICA;
		break;
	case 4:
		pger->FonteEnergia = gerSOLAR;
		break;
		// case 5:  pger->FonteEnergia = gerTGAS;       break;
		// case 6:  pger->FonteEnergia = gerHIDRAULICA; break;
	default:
		pger->FonteEnergia = gerPCH;
		break;
	}
//	// exibe/esconde opções para configura curva de gerador fotovoltaico
//	if ((pger->FonteEnergia == gerSOLAR) || (pger->FonteEnergia == gerEOLICA))
//	{
//		RadioGroupTipoCurva->Visible = true;
//		GBoxCurvaTipica->Visible = true;
//		// inicia CBoxCurvaTipica
//		CBoxCurvaTipicaInicia();
//		// altera para curva típica e simula alteração de RadioGroupTipoCurva
//		RadioGroupTipoCurvaClick(NULL);
////		if (RadioGroupTipoCurva->ItemIndex != 0)
////			RadioGroupTipoCurva->ItemIndex = 0;
//	}
//	else
//	{
//		RadioGroupTipoCurva->Visible = false;
//		GBoxCurvaTipica->Visible = false;
//		// altera para curva própria  e simula alteração de RadioGroupTipoCurva
//		RadioGroupTipoCurvaClick(NULL);
////		if (RadioGroupTipoCurva->ItemIndex != 1)
////			RadioGroupTipoCurva->ItemIndex = 1;
//	}
    AtualizaForm();
	// reinicia Captions,Labels etc da tela
	IniciaParteFixaTela();
	// atualiza dados de VPQ conforme opção selecionada
	ExibeDadosVPQ();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::CBoxFasesChange(TObject *Sender)
{
	// atualiza tipo da fonte do Gerador
	switch (CBoxFases->ItemIndex)
	{
	case 0:
		pger->Fases = faseANT;
		break;
		// case 1:
		// pger->Fases = faseBNT;
		// break;
		// case 2:
		// pger->Fases = faseCNT;
		// break;
		// case 3:
	case 1:
		pger->Fases = faseABNT;
		break;
		// case 4:
		// pger->Fases = faseBCNT;
		// break;
		// case 5:
		// pger->Fases = faseCANT;
		// break;
	default:
		pger->Fases = faseABCNT;
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::CBoxTipoSupChange(TObject *Sender)
{
    // DVC 25/07/2019
	// A opção de limitar fator de potência aparece se gerador for do tipo PV

	if(cboxTipoSup->ItemIndex == 0)
	{
	   EditFatPotLim->Visible = true;
	   CheckBoxFatPotLim->Visible = true;
	}
	else
	{
	   CheckBoxFatPotLim->Checked = false;
	   CheckBoxFatPotLim->Visible = false;
	   EditFatPotLim->Visible = false;
	}
	AtualizaForm();
	// reinicia Captions,Labels etc da tela
	IniciaParteFixaTela();
	// atualiza dados de VPQ conforme opção selecionada
	ExibeDadosVPQ();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::CheckBoxFatPotLimClick(TObject *Sender)
{
	// habilita/desabilita edição de fat.pot. limite
	EditFatPotLim->Visible = CheckBoxFatPotLim->Checked;
}

// ---------------------------------------------------------------------------
VTCurva* __fastcall TFormEdtGer::CriaCurva(void)
{
	// variáveis locais
	double valor[nvCURVA_PQVT];
	strHM hm_ini, hm_fim;
	VTCurva *curva;

	// cria Curva
	curva = DLL_NewObjCurva();
	curva->Tipica = false;
	curva->Escala = patEDITAVEL; ;
	curva->Tipo = curvaPQVT;
	curva->Unidade = unidMVA;
	curva->NumeroValores = nvCURVA_PQVT;
	curva->Reconfigura(patEDITAVEL);
	// inicia tensão c/ 1 pu
	curva->GetPonto(0, hm_ini, hm_fim, valor, nvCURVA_PQVT);
	valor[2] = 1.0;
	curva->SetPonto(hm_ini, hm_fim, valor, nvCURVA_PQVT);
	return (curva);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::ExibeDadosGerador(void)
{
	// atributos do gerador
	EditCodigo->Text = pger->Codigo;
	edtVnom->Text = DoubleToStr("%4.3f", pger->vnom);
	edtSnom->Text = DoubleToStr("%4.3f", pger->snom);
	// verifica se limita fator de potência
	if (IsDoubleZero(pger->fplim))
	{
		EditFatPotLim->Text = "";
		EditFatPotLim->Visible = false;
		CheckBoxFatPotLim->Checked = false;
	}
	else
	{
		EditFatPotLim->Text = DoubleToStr("%7.6f", pger->fplim);
		EditFatPotLim->Visible = true;
		CheckBoxFatPotLim->Checked = true;
	}
	// inicia tipo de ligação
	cboxLigacao->ItemIndex = pger->ligacao;
	// inicia tipo de suprimento: PV
	switch (pger->tiposup)
	{
	case supPV:
		cboxTipoSup->ItemIndex = 0;
		EditFatPotLim->Visible = true;
		CheckBoxFatPotLim->Visible = true;
		break;
	case supPQ:
		cboxTipoSup->ItemIndex = 1;
		EditFatPotLim->Visible = false;
		CheckBoxFatPotLim->Checked = false;
		CheckBoxFatPotLim->Visible = false;
		break;
	case supVF:
	default:
		cboxTipoSup->ItemIndex = 2;
		EditFatPotLim->Visible = false;
		CheckBoxFatPotLim->Checked = false;
		CheckBoxFatPotLim->Visible = false;
		break;
	}
	// inicia CBoxFonte
	switch (pger->FonteEnergia)
	{
	case gerDIESEL:
		CBoxFonte->ItemIndex = 0;
		break;
	case gerPCH:
		CBoxFonte->ItemIndex = 1;
		break;
	case gerVAPOR:
		CBoxFonte->ItemIndex = 2;
		break;
	case gerEOLICA:
		CBoxFonte->ItemIndex = 3;
		break;
	case gerSOLAR:
		CBoxFonte->ItemIndex = 4;
		break;
		// case gerTGAS:       CBoxFonte->ItemIndex = 5; break;
		// case gerHIDRAULICA: CBoxFonte->ItemIndex = 6; break;
	default:
		CBoxFonte->ItemIndex = 1;
		break;
	}

	// inicia CBoxFase
	switch (pger->Fases)
	{
	case faseANT:
		CBoxFases->ItemIndex = 0;
		break;
		// case faseBNT:
		// CBoxFases->ItemIndex = 1;
		// break;
		// case faseCNT:
		// CBoxFases->ItemIndex = 2;
		// break;
	case faseABNT:
		CBoxFases->ItemIndex = 1; // 3;
		break;
		// case faseBCNT:
		// CBoxFases->ItemIndex = 4;
		// break;
		// case faseCANT:
		// CBoxFases->ItemIndex = 5;
		// break;
	default:
		CBoxFases->ItemIndex = -1;
		break;
	}

	// simula evento de seleção de fontes
	CBoxFonteChange(NULL);
	// inicia CBoxTipo em função da Curva da Carga
	RadioGroupTipoCurva->ItemIndex = (pger->curva->Tipica) ? 0 : 1;
	// simula seleção de Curva típica/própria
	// RadioGroupTipoCurvaClick(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::ExibeDadosImpedancia(void)
{
	// inicia valores de z0 em pu
	sgZeq->Cells[1][1] = DoubleToStr("%7.6f", pger->z0.r);
	sgZeq->Cells[2][1] = DoubleToStr("%7.6f", pger->z0.x);
	// inicia valores z1 em pu
	sgZeq->Cells[1][2] = DoubleToStr("%7.6f", pger->z1.r);
	sgZeq->Cells[2][2] = DoubleToStr("%7.6f", pger->z1.x);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::ExibeDadosVPQ(void)
{
	FormEditaCurva->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	// verifica se Carga ficou c/ Curva própria
	if (pger->curva == curva.propria)
	{ // cancela ponteiro p/ Curva própria
		curva.propria = NULL;
	}
	CanClose = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		geral->Infoset->LoadFromFile(this);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::FormShow(TObject *Sender)
{
	// seleciona TableSheet
	PageControl->ActivePage = TabSheetGerador;
	// inicia CBoxTipoSup (OBS: precisa ser iniciado antes de IniciaParteFixaTela())
	cboxTipoSup->ItemIndex = pger->tiposup - supPV;
	// inicia Captions,Labels etc da tela
	IniciaParteFixaTela();
	// exibe dados do Gerador e da Chave
	ExibeDadosGerador();
	ExibeDadosVPQ();
	ExibeDadosImpedancia();
	// força alteração do tipo de ligação, para mostrar/esconder fases
	CBoxLigacaoChange(NULL);
	// posiciona o Cursor em EditCodigo
	EditCodigo->SetFocus();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::IniciaDadosGeradorNovo(void)
{
	// variáveis locais
	strESQ_EQBAR esq;
	VTBarra *barra;
	VTGerador *ult_gerador;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// verifica se o Gerador não está definido
	if (IsDoubleZero(pger->vnom) && IsDoubleZero(pger->snom))
	{ // verifica se há outros Geradores definidos
		lisTMP->Clear();
		redes->LisEqbar(lisTMP, eqptoGERADOR);
		if (lisTMP->Count > 0)
		{ // determina último Gerador
			ult_gerador = (VTGerador*)lisTMP->Last();
			// salva barra do Gerador e sua posição relativa
			barra = pger->pbarra;
			esq = pger->esq;
			// assume dados do último gerador
			pger->CopiaAtributosDe(*ult_gerador);
			// restaura Barra original do gerador e posição
			pger->pbarra = barra;
			pger->esq = esq;
			// define tensão nominal igual à da Barra
			pger->vnom = pger->pbarra->vnom;
			// redefine código
			pger->Codigo = "";
		}
	}
	// verifica se o Gerador não está definido
	if (pger->vnom == 0.)
		pger->vnom = pger->pbarra->vnom;
	if (pger->snom == 0.)
		pger->snom = 10.;
	if ((pger->z0.r == 0.) && (pger->z0.x == 0.) && (pger->z1.r == 0.) && (pger->z1.x == 0.))
	{
		pger->z0.x = pger->z1.x = 0.05;
	}
	// verifica se o Gerador possui uma Curva
	if (pger->curva == NULL)
	{ // cria Curva e associa c/ o Gerador
		pger->DefineCurva(CriaCurva());
	}
	// verifica se a Curva do Gerador é típica
	if (pger->curva->Tipica)
	{ // cria uma Curva própria para o Gerador e salva seu ponteiro
		curva.propria = CriaCurva();
	}
	else
	{ // salva ponteiro p/ Curva própria
		curva.propria = pger->curva;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::IniciaParteFixaTela(void)
{
	// inicia SGridZeq: títulos das colunas
	sgZeq->Cells[0][0] = "Impedância";
	sgZeq->Cells[1][0] = "r(pu)";
	sgZeq->Cells[2][0] = "x(pu)";
	// inicia SGridZeq: títulos das linhas
	sgZeq->Cells[0][1] = "seq.0";
	sgZeq->Cells[0][2] = "seq.1";
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtGer::LeDadosCurva(void)
{
	// determina tipo de suprimento
	switch (cboxTipoSup->ItemIndex)
	{
	case 0:
		pger->tiposup = supPV;
		break;
	case 1:
		pger->tiposup = supPQ;
		break;
	case 2:
		pger->tiposup = supVF;
		break;
	default:
		Erro("Defina o tipo de suprimento");
		return (false);
	}
	// verifica se está selecionada curva própria
	if (RadioGroupTipoCurva->ItemIndex == 1)
	{ // lê dados da Curva própria
		if (!FormEditaCurva->LePontosCurva())
			return (false);
	}
	// seleciona Curva
	if (!SelecionaCurva())
		return (false);

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtGer::LeDadosGerador(void)
{
	// variáveis locais
	double v_pu;

	try
	{ // atributos do gerador
		pger->Codigo = EditCodigo->Text.Trim();
		pger->vnom = StrToDouble(edtVnom->Text);
		pger->snom = StrToDouble(edtSnom->Text);
		pger->ligacao = cboxLigacao->ItemIndex;

		// valida fases
		if ((cboxLigacao->ItemIndex == 3) && (CBoxFases->ItemIndex < 0))
		{
			Erro("Selecione uma fase para o gerador");
			return (false);
		}
		else if (cboxLigacao->ItemIndex != 3)
			pger->Fases = faseABC;

		// determina tipo da geração
		switch (CBoxFonte->ItemIndex)
		{
		case 0:
			pger->FonteEnergia = gerDIESEL;
			break;
		case 1:
			pger->FonteEnergia = gerPCH;
			break;
		case 2:
			pger->FonteEnergia = gerVAPOR;
			break;
		case 3:
			pger->FonteEnergia = gerEOLICA;
			break;
		case 4:
			pger->FonteEnergia = gerSOLAR;
			break;
			// case 5:  pger->FonteEnergia = gerTGAS;       break;
			// case 6:  pger->FonteEnergia = gerHIDRAULICA; break;
		default:
			pger->FonteEnergia = gerPCH;
			break;
		}
		// verifica se limita fator de potência
		if (CheckBoxFatPotLim->Checked)
		{
			pger->fplim = StrToDouble(EditFatPotLim->Text);
		}
		else
		{
			pger->fplim = 0.;
		}
	}
	catch (Exception &e)
	{
		Erro("Valor inválido: dados do gerador");
		return (false);
	}
	// valida dados
	// if (pger->Codigo.IsEmpty())
	// {
	// Aviso("Defina um código para o gerador.");
	// return(false);
	// }
	if ((CheckBoxFatPotLim->Checked) && (pger->fplim < 0.1))
	{
		Erro("Limite de fator de potência com valor inferior a 0.1");
		return (false);
	}
	if (pger->fplim > 1.0)
	{
		Erro("Limite de fator de potência com valor superior a 1.0");
		return (false);
	}
	if (!IsDoubleZero(pger->pbarra->vnom))
	{
		v_pu = pger->vnom / pger->pbarra->vnom;
		if ((v_pu < 0.95) || (v_pu > 1.05))
		{
			Erro("Tensão nominal incompatível c/ a tensão da barra");
			return (false);
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtGer::LeDadosImpedancia(void)
{
	try
	{
		// lê valores de z0
		pger->z0.r = StrToDouble(sgZeq->Cells[1][1]);
		pger->z0.x = StrToDouble(sgZeq->Cells[2][1]);
		// lê valores z1
		pger->z1.r = StrToDouble(sgZeq->Cells[1][2]);
		pger->z1.x = StrToDouble(sgZeq->Cells[2][2]);
	}
	catch (...)
	{
		Erro("Valor Inválido: dados de impedância");
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::OpcaoRetira(bool enabled)
{
	butRetira->Visible = enabled;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::PageControlChange(TObject *Sender)
{
	// verifica se está selecionado visualização da Curva
	if (PageControl->ActivePage == TabSheetGrafico)
	{
		FormExibeCurva->DefineCurva(pger, pger->curva);
		FormExibeCurva->HabilitaCurva(ger_PQVF);
		// FormExibeCurva->HabilitaCurva(ger_P);
		// FormExibeCurva->HabilitaCurva(ger_Q);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::PageControlChanging(TObject *Sender, bool &AllowChange)
{
	// verifica se o Gerador está associado a uma Curva
	if (pger->curva == NULL)
	{
		Aviso("Selecione a curva do gerador");
		AllowChange = false;
		return;
	}
	// verifica se deve ler dados editados
	if (PageControl->ActivePage == TabSheetGerador)
	{ // lê os dados editados
		AllowChange = LeDadosGerador();
	}
	else if (PageControl->ActivePage == TabSheetCurva)
	{ // lê os pontos editados da curva
		AllowChange = LeDadosCurva();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtGer::RadioGroupTipoCurvaClick(TObject *Sender)
{
//	// verifica se foi selecionada opção de curva típica
//	if (RadioGroupTipoCurva->ItemIndex == 0)
//	{ // exibe GBoxCurvaTipica
//		GBoxCurvaTipica->Visible = true;
//		// desabilita edição da Curva
//		TabSheetCurva->TabVisible = false;
//	}
//	else
//	{ // esconde GBoxCurvaTipica
//		GBoxCurvaTipica->Visible = false;
//		// habilita edição da Curva
//		TabSheetCurva->TabVisible = true;
//	}
//	// determina Curva p/ Gerador
//	SelecionaCurva();
    AtualizaForm();
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtGer::SelecionaCurva(void)
{
	// variáveis locais
	int index;
	VTCurva *curva_tipica = NULL;

	// verifica se está selecionada opção de Curva típica
	if (RadioGroupTipoCurva->ItemIndex == 0)
	{ // verifica se há curvas típicas
		if (CBoxCurvaTipica->Items->Count > 0)
		{ // verifica se há uma Curva selecionada
			if ((index = CBoxCurvaTipica->ItemIndex) < 0)
				index = 0;
			curva_tipica = (VTCurva*)CBoxCurvaTipica->Items->Objects[index];
		}
		// redefine Curva do Gerador
		pger->curva = curva_tipica;
	}
	else
	{ // verifica se a Curva atual da Carga é típica
		if ((pger->curva == NULL) || (pger->curva != curva.propria))
		{ // redefine Curva própria da Carga
			pger->curva = curva.propria;
		}
	}
	return (pger->curva != NULL);
}

// ---------------------------------------------------------------------------
// eof
