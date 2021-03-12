// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtCar.h"
//#include "TFormEditaCurva.h"
#include "TFormEditaCurvaCarga3F.h"
#include "TFormExibeCurva.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Mercado\VTMercado.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\ImportaCurva.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;

// ---------------------------------------------------------------------------
__fastcall TFormEdtCar::TFormEdtCar(TComponent* Owner, VTApl *apl, VTCarga *pcarga, bool op_retira)
	: TForm(Owner)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	this->pcarga = pcarga;
	// configura opção de retirar
	ActionRetira->Visible = op_retira;
	// inicia dados default p/ Eqpto novo
	IniciaDadosCargaNova();
	IniciaCBoxFase();
	// cria objetos
//	lisTMP = new TList();
	// cria TFormEditaCurva p/ edição da Curva
	FormEditaCurvaCarga3F = new TFormEditaCurvaCarga3F(NULL, this, apl, TabSheetEdicao);
	FormEditaCurvaCarga3F->DefineCurva(pcarga, curva.propria);
	FormEditaCurvaCarga3F->Show();
	// cria TFormExibeCurva p/ exibição da Curva
	FormExibeCurva = new TFormExibeCurva(this, apl, TabSheetGrafico);
	FormExibeCurva->gbTipo->Visible = true;
	FormExibeCurva->Show();
	//TabSheetGrafico->Visible = false;
	//TabSheetGrafico->Enabled = false;
	// posiciona o Form
	FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
__fastcall TFormEdtCar::~TFormEdtCar(void)
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
	// destrói Forms
	if (FormEditaCurvaCarga3F)
	{
		delete FormEditaCurvaCarga3F;
		FormEditaCurvaCarga3F = NULL;
	}
	if (FormExibeCurva)
	{
		delete FormExibeCurva;
		FormExibeCurva = NULL;
	}
	// if (FormEdtPlan) {delete FormEdtPlan; FormEdtPlan = NULL;}
	// destrói lista sem destruir seus objetos
//	if (lisTMP)
//	{
//		delete lisTMP;
//		lisTMP = NULL;
//	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::ActionCancelaExecute(TObject *Sender)
{
	/*
	 //associa Carga c/ Curva própria
	 pcarga->DefineCurva(curva.propria);
	 //cancela ponteiro p/ Curva própria
	 curva.propria = NULL;
	 */
	// fecha o form
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::ActionConfirmaExecute(TObject *Sender)
{
	// lê dados de planejamento (importante p/ definir ano da Carga)
	// if (! FormEdtPlan->LeDados()) return;
	if (!LeDadosCarga())
		return;
	/*
	 //verifica se Carga ficou c/ Curva própria
	 if (pcarga->curva == curva.propria)
	 {//cancela ponteiro p/ Curva própria
	 curva.propria = NULL;
	 }
	 */
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::ActionImportaExecute(TObject *Sender)
{
	// proteção: verifica se a Curva da Carga é própria
	if (pcarga->curva->Tipica)
		return;
	// cria FormImportaCurva como janela modal
	if (DLL_ImportaCurva(this, apl, pcarga, pcarga->curva))
	{ // Curva foi alterada
		FormEditaCurvaCarga3F->DefineCurva(pcarga, curva.propria);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::ActionRetiraExecute(TObject *Sender)
{
	ModalResult = mrNo;
}

/*
 //---------------------------------------------------------------------------
 VTCarga* __fastcall TFormEdtCar::CargaAtualizada(void)
 {
 //variaveis locais
 //VTCarga *novaCarga;

 //if(! LeDadosCarga() ) return(NULL);
 return(pcarga);
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::CBoxClasseChange(TObject *Sender)
{
	// variáveis locais
	VTClasse *classe;

	// proteção
	if (CBoxClasse->ItemIndex < 0)
		return;
	// verifica se a classe selecionada é equivalente
	classe = (VTClasse*)CBoxClasse->Items->Objects[CBoxClasse->ItemIndex];
	if (classe == NULL)
		return;
	TabSheetQtdCons->TabVisible = (classe->Tag == consEQUIVALENTE);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::CBoxMBTChange(TObject *Sender)
{
	// reinicia CBoxFase
	IniciaCBoxFase();
	if (RadioGroupTipoCurva->ItemIndex == 1)
	{
		FormEditaCurvaCarga3F->SGridInicia();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::CBoxFaseChange(TObject *Sender)
{
	if (RadioGroupTipoCurva->ItemIndex == 1)
	{
		FormEditaCurvaCarga3F->SGridInicia();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::CBoxUnidEnergiaChange(TObject *Sender)
{
	// verifica se está selecionada curva típica
	if (RadioGroupTipoCurva->ItemIndex == 0)
	{
		SelecionaCurva();
	}
}

// ---------------------------------------------------------------------------
VTCurva* __fastcall TFormEdtCar::CriaCurva(void)
{
	// variáveis locais
	VTCurva *curva;
	double valor[nvCURVA_PQ];

	// cria Curva
	curva = DLL_NewObjCurva();
	curva->Tipica = false;
	curva->Escala = patEDITAVEL;
	curva->Tipo = curvaPQ;
	curva->Unidade = unidMVA;
	curva->NumeroValores = nvCURVA_PQ;
	curva->Reconfigura(patEDITAVEL);
	return (curva);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::DefineCrescimento(void)
{
	// variáveis locais
	VTRede *rede;
	VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();

	// determina Rede que contém Barra da Carga
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		if (rede->ExisteBarra(pcarga->pbarra))
		{
			mercado->DefineCrescimento(pcarga, rede);
			return;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::ExibeDadosCarga(void)
{
	// apresenta código
	EditCodigo->Text = pcarga->Codigo;
	// apresenta indicação de VIC
	CheckBoxVIC->Checked = pcarga->VeryImportant;
	// apresenta indicação de grupo A //DVK 2016.10.05
	CheckBoxGrupoA->Checked = pcarga->GrupoA;
	// inicia CBoxMBT(monofásica, bifásica, trifásica)
	CBoxMBT->ItemIndex = pcarga->MBTfasica - cargaMONOFASICA;
	// inicia CBoxFase c/ as possíveis fases da Carga
	IniciaCBoxFase();
	// inicia CBoxClasse p/ identificar a Classe da Carga
	CBoxClasse->ItemIndex = CBoxClasse->Items->IndexOfObject(pcarga->classe);
	CBoxClasseChange(NULL);
	// inicia CBoxModCarga p/ identificar o modelo de carga da Carga
	switch (pcarga->ModeloCarga)
	{
	case cargaICTE:
		CBoxModCarga->ItemIndex = 0;
		break;
	case cargaSCTE:
		CBoxModCarga->ItemIndex = 1;
		break;
	case cargaZCTE:
		CBoxModCarga->ItemIndex = 2;
		break;
	default:
		CBoxModCarga->ItemIndex = 0;
		break;
	}
	// verifica se a Energia está definida
	if (IsDoubleZero(pcarga->Energia_mwhmes))
	{ // Carga definida por Curva propria
		EditEnergia->Text = "0";
	}
	else if (pcarga->Energia_mwhmes < 0.5)
	{ // exibe valor em kWh/mês
		EditEnergia->Text = DoubleToStr("%2.1f", pcarga->Energia_kwhmes);
		CBoxUnidEnergia->ItemIndex = 0;
	}
	else
	{ // exibe valor em MWh/mês
		EditEnergia->Text = DoubleToStr("%4.3f", pcarga->Energia_mwhmes);
		CBoxUnidEnergia->ItemIndex = 1;
	}
	// inicia CBoxTipo em função da Curva da Carga
	RadioGroupTipoCurva->ItemIndex = (pcarga->curva->Tipica) ? 0 : 1;
	// simula seleção de Curva típica/própria
	RadioGroupTipoCurvaClick(NULL);
}

// ---------------------------------------------------------------------------
VTCurva* __fastcall TFormEdtCar::ExisteCurvaTipica(void)
{
	// variáveis locais
	VTCurva *curva;
	VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

	if (!LeValorEnergia(true))
		return (NULL);
	if (!SelecionaClasse())
		return (NULL);
	// classe e energia definidos: detemina Curva típica
	curva = curvas->ExisteCurvaTipicaCarga(pcarga->classe, pcarga->Energia_kwhmes);
	if (curva == NULL)
		Aviso("Não existe curva típica p/ a carga");
	return (curva);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		geral->Infoset->LoadFromFile(this);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	// verifica se Carga ficou c/ Curva própria
	if (pcarga->curva == curva.propria)
	{ // cancela ponteiro p/ Curva própria
		curva.propria = NULL;
	}
	CanClose = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::FormShow(TObject *Sender)
{
	// inicia Captions,Labels etc da tela
//	IniciaParteFixaTela();
	// inicia CBoxClasse
	IniciaCBoxClasse();
	// inicia quantidade de consumidores
	IniciaQuantidadeConsumidores();
	// exibe dados da Carga
	ExibeDadosCarga();
	// seleciona TableSheet
	PageControl->ActivePage = TabSheetEdicao;
	PageControl->ActivePage = TabSheetCarga;
	// posiciona o Cursor em EditCodigo
	EditCodigo->SetFocus();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::IniciaCBoxClasse(void)
{
	// variáveis locais
	VTClasse *classe;
	TList *lisCLASSE;
	VTClasses *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));

	// limpa CBoxClasse
	CBoxClasse->Clear();
	// preenche CBoxClasse c/ as Classes
	lisCLASSE = classes->LisClasse();
	for (int n = 0; n < lisCLASSE->Count; n++)
	{
		classe = (VTClasse*)lisCLASSE->Items[n];
		// exclui classeIP
		if (classe->Tag == consIP)
			continue;
		CBoxClasse->Items->AddObject(classe->Codigo, classe);
	}
	// pré-seleciona primeiro item
	if (CBoxClasse->Items->Count > 0)
		CBoxClasse->ItemIndex = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::IniciaCBoxFase(void)
{
	// variáveis locais
	TFase *fase;
	TList *lisFASE;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

	// inicia lisFASE c/ o conjunto de fases definidas pelo CBoxMBT
	switch (CBoxMBT->ItemIndex)
	{
	case 0:
		lisFASE = fases->LisFasesCarga(cargaMONOFASICA);
		break;
	case 1:
		lisFASE = fases->LisFasesCarga(cargaBIFASICA);
		break;
	default:
		lisFASE = fases->LisFasesCarga(cargaTRIFASICA);
		break;
	}
	// reinicia CBoxFase
	CBoxFase->Clear();
	for (int n = 0; n < lisFASE->Count; n++)
	{
		fase = (TFase*)lisFASE->Items[n];
		CBoxFase->Items->AddObject(fase->codigo, fase);
	}
	// pré-seleciona fases da Carga
	if (CBoxFase->Items->Count < 0)
		return;
	CBoxFase->ItemIndex = CBoxFase->Items->IndexOf(fases->AsStrUpper[pcarga->Fases]);
	if (CBoxFase->ItemIndex < 0)
		CBoxFase->ItemIndex = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::IniciaDadosCargaNova(void)
{
	// variáveis locais
	VTClasses *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));

	// verifica se a Carga possui Classe
	if (pcarga->classe == NULL)
	{ // define Classe
		pcarga->classe = classes->First();
	}
	// verifica se a Carga ainda não tem uma Curva associada
	if (pcarga->curva == NULL)
	{ // cria Curva própria p/ a Carga
		pcarga->DefineCurva(CriaCurva());
	}
	// verifica se a Curva da Carga é típica
	if (pcarga->curva->Tipica)
	{ // cria uma Curva própria para a Carga e salva seu ponteiro
		curva.propria = CriaCurva();
	}
	else
	{ // salva ponteiro p/ Curva própria
		curva.propria = pcarga->curva;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::IniciaParteFixaTela(void)
{
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::IniciaQuantidadeConsumidores(void)
{
	// residencial
	EditNres->Text = pcarga->NumConsResidencial;
	// comercial
	EditNcom->Text = pcarga->NumConsComercial;
	// industrial
	EditNind->Text = pcarga->NumConsIndustrial;
	// rural
	EditNrur->Text = pcarga->NumConsRural;
	// outros
	EditNout->Text = pcarga->NumConsOutros;
	// a4
	EditNa4->Text = pcarga->NumConsA4;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtCar::LeDadosCarga(void)
{
	// variáveis locais
	VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// verifica se foi definido código
	// if (EditCodigo->Text.IsEmpty())
	// {
	// Aviso("Defina código da carga");
	// return(false);
	// }
	pcarga->Codigo = EditCodigo->Text;
	// lê indicação de VIC
	pcarga->VeryImportant = CheckBoxVIC->Checked;
	// lê indicação de grupo A //DVK 2016.10.05
	pcarga->GrupoA = CheckBoxGrupoA->Checked;
	// lê fases, mod.carga e classe
	if (!LeFases())
		return (false);
	if (!LeValorModCarga())
		return (false);
	if (!SelecionaClasse())
		return (false);
	// lê quantidade de consumidores
	LeQuantidadeConsumidoresCarga();
	// verifica se está selecionada curva típica
	if (RadioGroupTipoCurva->ItemIndex == 0)
	{ // lê valor de energia, validando
		if (!LeValorEnergia(true))
			return (false);
	}
	else
	{ // lê dados da Curva própria
		//FormEditaCurvaCarga3F->SGridInicia();
		if (!FormEditaCurvaCarga3F->LePontosCurva())
			return (false);
	}
	// seleciona Curva
	if (!SelecionaCurva())
		return (false);
	// define Crescimento da Carga
	// mercado->DefineCrescimento(pcarga, redes);
	DefineCrescimento();
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtCar::LeFases(void)
{
	// variáveis locais
	TFase *fase;

	// verifica se está selecionada uma fase
	if (CBoxFase->ItemIndex < 0)
	{
		Erro("Defina as fases da carga");
		return (false);
	}
	if ((fase = (TFase*)CBoxFase->Items->Objects[CBoxFase->ItemIndex]) == NULL)
		return (false);
	// atualiza fase da Carga
	pcarga->Fases = fase->tag;
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::LeQuantidadeConsumidoresCarga(void)
{
	// variáveis locais
	VTClasse *classe;
	int num_cons = 0;

	// zera todos NumCons
	pcarga->NumConsResidencial = num_cons;
	pcarga->NumConsIndustrial = num_cons;
	pcarga->NumConsRural = num_cons;
	pcarga->NumConsComercial = num_cons;
	pcarga->NumConsOutros = num_cons;
	pcarga->NumConsA4 = num_cons;
	try
	{ // verifica se a classe da carga é equivalente
		classe = (VTClasse*)CBoxClasse->Items->Objects[CBoxClasse->ItemIndex];
		if (classe == NULL)
			return;
		if (classe->Tag == consEQUIVALENTE)
		{ // residencial
			num_cons = StrToInt(EditNres->Text);
			pcarga->NumConsResidencial = num_cons;
			// industrial
			num_cons = StrToInt(EditNind->Text);
			pcarga->NumConsIndustrial = num_cons;
			// rural
			num_cons = StrToInt(EditNrur->Text);
			pcarga->NumConsRural = num_cons;
			// comercial
			num_cons = StrToInt(EditNcom->Text);
			pcarga->NumConsComercial = num_cons;
			// outros
			num_cons = StrToInt(EditNout->Text);
			pcarga->NumConsOutros = num_cons;
			// a4
			num_cons = StrToInt(EditNa4->Text);
			pcarga->NumConsA4 = num_cons;
		}
		else
		{
			switch (classe->Tag)
			{
			case consRESIDENCIAL:
				pcarga->NumConsResidencial = 1;
				break;
			case consCOMERCIAL:
				pcarga->NumConsComercial = 1;
				break;
			case consRURAL:
				pcarga->NumConsRural = 1;
				break;
			case consINDUSTRIAL:
				pcarga->NumConsIndustrial = 1;
				break;
			case consOUTROS:
				pcarga->NumConsOutros = 1;
				break;
			case consA4:
				pcarga->NumConsA4 = 1;
				break;
			}
		}
	}
	catch (Exception &e)
	{ // nada
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtCar::LeValorEnergia(bool validar)
{
	// variáveis locais
	double energia;

	try
	{ // verifica se a Energia está definida
		energia = StrToDouble(EditEnergia->Text);
		if ((validar) && (IsDoubleZero(energia)))
		{
			Aviso("Defina o valor da energia mensal da carga");
			return (false);
		}
		// verifica unidade da energia
		if (CBoxUnidEnergia->ItemIndex == 0)
		{ // valor editado de energia em kWh/mês
			pcarga->Energia_kwhmes = energia;
		}
		else
		{ // valor editado de energia em MWh/mês
			pcarga->Energia_mwhmes = energia;
		}
	}
	catch (Exception &e)
	{ // assume valor nulo
		pcarga->Energia_mwhmes = 0;
		if (validar)
		{
			Aviso("Energia c/ valor inválido");
			return (false);
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtCar::LeValorModCarga(void)
{
	// variáveis locais
	double icte = 0., scte = 0., zcte = 0.;

	switch (CBoxModCarga->ItemIndex)
	{
	case 0:
		icte = 1.;
		break;
	case 1:
		scte = 1.;
		break;
	case 2:
		zcte = 1.;
		break;
	default:
		icte = 1.;
		break;
	}
	// atualiza Carga
	pcarga->Icte_pu = icte;
	pcarga->Scte_pu = scte;
	pcarga->Zcte_pu = zcte;
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::PageControlChange(TObject *Sender)
{
	if (PageControl->ActivePage != TabSheetCarga)
	{
		FormEditaCurvaCarga3F->DefineCurva(pcarga, pcarga->curva);
		FormEditaCurvaCarga3F->SGridInicia();
	}
	// verifica se está selecionado visualização da Curva
	if (PageControl->ActivePage == TabSheetGrafico)
	{ // le fases e curva da carga
		LeFases();
		SelecionaCurva();
		// atualiza FormExibeCurva
		FormExibeCurva->DefineCurva(pcarga, pcarga->curva);
		FormExibeCurva->CBoxTipoCurva->ItemIndex = 0;
		//verifica quantidade de valores da curva
		if(pcarga->curva->NumeroValores == nvCURVA_RRCAR)
		{
			FormExibeCurva->CBoxTipoCurva->ItemIndex = 1;
			FormExibeCurva->CBoxTipoCurvaChange(NULL);
//			FormExibeCurva->HabilitaCurvaPorFase();
		}
		else
		{
			FormExibeCurva->CBoxTipoCurva->ItemIndex = 0;
			FormExibeCurva->CBoxTipoCurvaChange(NULL);
//			FormExibeCurva->HabilitaCurva(car_P);
//			FormExibeCurva->HabilitaCurva(car_Q);
        }
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::PageControlChanging(TObject *Sender, bool &AllowChange)
{
	// verifica se a Carga está associada a uma Curva
	if (pcarga->curva == NULL)
	{
		Aviso("Selecione a curva da carga");
		AllowChange = false;
		return;
	}
	// verifica se deve ler dados editados
	if (PageControl->ActivePage == TabSheetEdicao)
	{ // lê os dados editados
		AllowChange = LeDadosCarga();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtCar::RadioGroupTipoCurvaClick(TObject *Sender)
{
	// determina Curva p/ Carga
	SelecionaCurva();
	// verifica se foi selecionada opção de curva típica
	if (RadioGroupTipoCurva->ItemIndex == 0)
	{ // exibe PanelTipica
		GBoxTipica->Visible = true;
		// desabilita edição da Curva
		TabSheetEdicao->TabVisible = false;
	}
	else
	{ // esconde PanelTipica
		GBoxTipica->Visible = false;
		// habilita edição da Curva
		TabSheetEdicao->TabVisible = true;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtCar::SelecionaClasse(void)
{
	// verifica se a Classe está definida
	if (CBoxClasse->ItemIndex < 0)
	{
		Aviso("Defina a classe da carga");
		return (false);
	}
	pcarga->classe = (VTClasse*)CBoxClasse->Items->Objects[CBoxClasse->ItemIndex];
	return (pcarga->classe != NULL);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtCar::SelecionaCurva(void)
{
	// verifica se está selecionada opção de Curva típica
	if (RadioGroupTipoCurva->ItemIndex == 0)
	{ // redefine Curva da Carga como uma Curva tipica
		pcarga->curva = ExisteCurvaTipica();
	}
	else
	{ // verifica se a Curva atual da Carga é típica
		if ((pcarga->curva == NULL) || (pcarga->curva != curva.propria))
		{ // redefine Curva própria da Carga
			pcarga->curva = curva.propria;
		}
	}
	return (pcarga->curva != NULL);
}

// ---------------------------------------------------------------------------
// eof

// ---------------------------------------------------------------------------
