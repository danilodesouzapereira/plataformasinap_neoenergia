// ---------------------------------------------------------------------------
#include <vcl.h>
#include <algorithm>
#pragma hdrstop
#include "TFormCfgPerdatec.h"
#include "TFormCfgEdtMedidor.h"
#include "TFormCfgEdtRamal.h"
#include "TFormCfgEdtTrafo.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Flow\VTFlow.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\Geral\CfgPerdaTec\VTCfgPerdaTec.h>
#include <Fontes\Rede\VTEqpto.h>
#include <DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormCfgPerdatec::TFormCfgPerdatec(TComponent* Owner, VTApl *apl_owner) : TForm(Owner)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl_owner->GetObject(__classid(VTGeral));

	// salva ponteiro para objetos
	this->apl = apl_owner;
	CfgPerdaTec = geral->CfgPerdaTec;
	// posiciona o Form
	FormIniciaPosicao();
	// preenche os vários ComboBox
	CBoxMedidorInicia();
	CBoxRamalInicia();
	CBoxTrafoInicia();
	// carrega parametros anterior
	CarregaParametros();
	// inicia componentes da tela
	ExibeDados();
}

// ---------------------------------------------------------------------------
__fastcall TFormCfgPerdatec::~TFormCfgPerdatec(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::ActionAtualizaFormExecute(TObject *Sender)
{
	// inicia componentes da tela
	ExibeDados();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::ActionCancelaExecute(TObject *Sender)
{
	// fecha o Form
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::ActionCfgAnteriorExecute(TObject *Sender)
{
	// carrega parametros anterior
	CarregaParametros();
	// atualiza a tela
	ExibeDados();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::ActionCfgGis1Execute(TObject *Sender)
{
	// reinicia CfgPerdaTec com valores nulos
	CfgPerdaTec->IniciaConfiguracaoGis(1);
	// atualiza tela
	ExibeDados();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::ActionCfgGis2Execute(TObject *Sender)
{
	// reinicia CfgPerdaTec com valores nulos
	CfgPerdaTec->IniciaConfiguracaoGis(2);
	// atualiza tela
	ExibeDados();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::ActionCfgProdistExecute(TObject *Sender)
{
	// reinicia CfgPerdaTec com valores nulos
	CfgPerdaTec->IniciaConfiguracaoProdist();
	// atualiza tela
	ExibeDados();
	// DVK 2016.01.20 inclui perdas por dentro
	// CheckBoxPerdaFerroPorDentro->Checked   = true;
	// CheckBoxPerdaMedidorPorDentro->Checked = true;
	// CheckBoxPerdaRamalPorDentro->Checked   = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::ActionConfirmaExecute(TObject *Sender)
{
	// lê daddo
	if (!LeDados())
		return;
	// marca que houve alteração em CfgPerdaTec
	CfgPerdaTec->Alterada = true;
	SalvaParametros();

	// fecha o Form
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::ActionEditaMedidorExecute(TObject *Sender)
{
	TFormCfgEdtMedidor *formCfgEdtMedidor;

	// cria o TFormCfgEdtMedidor
	formCfgEdtMedidor = new TFormCfgEdtMedidor(this);
	// passa a lista de perdas para o form
	formCfgEdtMedidor->Inicia(CfgPerdaTec->LisPerdasMedidor);
	// esconde este form para mostrar o outro
	Hide();
	if (formCfgEdtMedidor->ShowModal() == mrOk)
	{ // atualiza indicação de alteração dos dados
		CfgPerdaTec->Alterada = true;
		// atualiza o listview correspondente
		LViewIniciaMedidor(CfgPerdaTec->LisPerdasMedidor);
	}
	delete formCfgEdtMedidor;
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::ActionEditaRamalExecute(TObject *Sender)
{
	TFormCfgEdtRamal *formCfgEdtRamal;

	// cria o TFormCfgEdtRamal
	formCfgEdtRamal = new TFormCfgEdtRamal(this);
	// passa a lista de perdas para o form
	formCfgEdtRamal->Inicia(CfgPerdaTec->LisPerdasRamal);
	// esconde este form
	Hide();
	// exibe formCfgEdtRamal
	if (formCfgEdtRamal->ShowModal() == mrOk)
	{ // atualiza indicação de alteração dos dados
		CfgPerdaTec->Alterada = true;
		// atualiza o listview correspondente
		LViewIniciaRamal(CfgPerdaTec->LisPerdasRamal);
	}
	delete formCfgEdtRamal;
	// reapresenta este Form
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::ActionEditaTrafoMonoEmpresaExecute(TObject *Sender)
{
	TFormCfgEdtTrafo *formCfgEdtTrafo;

	// cria o TFormCfgEdtTrafo
	formCfgEdtTrafo = new TFormCfgEdtTrafo(this);
	// passa a lista de perdas para o form
	formCfgEdtTrafo->Inicia(CfgPerdaTec->LisPerdasTrafoEmpresa, trfMONOFASICO);
	// esconde este form para mostrar o outro
	Hide();
	// exibe formCfgEdtTrafo
	if (formCfgEdtTrafo->ShowModal() == mrOk)
	{ // atualiza indicação de alteração dos dados
		CfgPerdaTec->Alterada = true;
		// atualiza o listview correspondente
		LViewIniciaTrafo(CfgPerdaTec->LisPerdasTrafoEmpresa, trfMONOFASICO);
	}
	delete formCfgEdtTrafo;
	// reapresenta este Form
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::ActionEditaTrafoMonoProdistExecute(TObject *Sender)
{
	TFormCfgEdtTrafo *formCfgEdtTrafo;

	// cria o TFormCfgEdtTrafo
	formCfgEdtTrafo = new TFormCfgEdtTrafo(this);
	// passa a lista de perdas para o form
	formCfgEdtTrafo->Inicia(CfgPerdaTec->LisPerdasTrafoProdist, trfMONOFASICO);
	// esconde este form para mostrar o outro
	Hide();
	// exibe formCfgEdtTrafo
	if (formCfgEdtTrafo->ShowModal() == mrOk)
	{ // atualiza indicação de alteração dos dados
		CfgPerdaTec->Alterada = true;
		// atualiza o listview correspondente
		LViewIniciaTrafo(CfgPerdaTec->LisPerdasTrafoProdist, trfMONOFASICO);
	}
	delete formCfgEdtTrafo;
	// reapresenta este Form
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::ActionEditaTrafoSEDExecute(TObject *Sender)
{
	Aviso("Dunção não disponível nesta versão");
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::ActionEditaTrafoTrifEmpresaExecute(TObject *Sender)
{ // variaveis locais
	TFormCfgEdtTrafo *formCfgEdtTrafo;

	// cria o TFormCfgEdtTrafo
	formCfgEdtTrafo = new TFormCfgEdtTrafo(this);
	// passa a lista de perdas para o form
	formCfgEdtTrafo->Inicia(CfgPerdaTec->LisPerdasTrafoEmpresa, trfTRIFASICO);
	// esconde este form
	Hide();
	// exibe formCfgEdtTrafo
	if (formCfgEdtTrafo->ShowModal() == mrOk)
	{ // atualiza indicação de alteração dos dados
		CfgPerdaTec->Alterada = true;
		// atualiza o listview correspondente
		LViewIniciaTrafo(CfgPerdaTec->LisPerdasTrafoEmpresa, trfTRIFASICO);
	}
	delete formCfgEdtTrafo;
	// reapresenta este Form
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::ActionEditaTrafoTrifProdistExecute(TObject *Sender)
{ // variaveis locais
	TFormCfgEdtTrafo *formCfgEdtTrafo;

	// cria o TFormCfgEdtTrafo
	formCfgEdtTrafo = new TFormCfgEdtTrafo(this);
	// passa a lista de perdas para o form
	formCfgEdtTrafo->Inicia(CfgPerdaTec->LisPerdasTrafoProdist, trfTRIFASICO);
	// esconde este form
	Hide();
	// exibe formCfgEdtTrafo
	if (formCfgEdtTrafo->ShowModal() == mrOk)
	{ // atualiza indicação de alteração dos dados
		CfgPerdaTec->Alterada = true;
		// atualiza o listview correspondente
		LViewIniciaTrafo(CfgPerdaTec->LisPerdasTrafoProdist, trfTRIFASICO);
	}
	delete formCfgEdtTrafo;
	// reapresenta este Form
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::CarregaParametros(void)
{

	AnsiString redeComNeutroTxt;
	AnsiString redeComCapacitorTxt;
	AnsiString zatTxt;
	AnsiString zatOhmTxt;
	AnsiString fluxoTxt;
	AnsiString modeloCargaTxt;
	AnsiString ferroPorForaTxt;
	AnsiString medidorPorForaTxt;
	AnsiString ramalPorForaTxt;
	AnsiString opPerdaMedidorTxt;
	AnsiString medidorTxt;
	AnsiString opPerdaRamalTxt;
	AnsiString ramalTxt;
	AnsiString opPerdaTrafoTxt;
	AnsiString trafoTxt;
	AnsiString opPerdaTrafoSEDTxt;
	AnsiString perdaAdicionalTxt;
	AnsiString edicaoTxt;

	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // leparametros para conexao
		if (geral->Infoset->LoadFromFile("CfgPerdatec"))
		{
			geral->Infoset->GetInfo("redeComNeutro", redeComNeutroTxt);
			geral->Infoset->GetInfo("redeComCapacitor", redeComCapacitorTxt);
			geral->Infoset->GetInfo("zat", zatTxt);
			geral->Infoset->GetInfo("zatOhm", zatOhmTxt);
			geral->Infoset->GetInfo("fluxo", fluxoTxt);
			geral->Infoset->GetInfo("modeloCarga", modeloCargaTxt);
			geral->Infoset->GetInfo("ferroPorFora", ferroPorForaTxt);
			geral->Infoset->GetInfo("medidorPorFora", medidorPorForaTxt);
			geral->Infoset->GetInfo("ramalPorFora", ramalPorForaTxt);
			geral->Infoset->GetInfo("opPerdaMedidor", opPerdaMedidorTxt);
			geral->Infoset->GetInfo("perdaMedidorUnico", medidorTxt);
			geral->Infoset->GetInfo("opPerdaRamal", opPerdaRamalTxt);
			geral->Infoset->GetInfo("perdaRamalUnico", ramalTxt);
			geral->Infoset->GetInfo("opPerdaTrafo", opPerdaTrafoTxt);
			geral->Infoset->GetInfo("perdaTrafoUnico", trafoTxt);
			geral->Infoset->GetInfo("opPerdaTrafoSED", opPerdaTrafoSEDTxt);
			geral->Infoset->GetInfo("perdaAdicional", perdaAdicionalTxt);
			geral->Infoset->GetInfo("edicao", edicaoTxt);

			CfgPerdaTec->RedeComNeutro = StrToBool(redeComNeutroTxt);
			CfgPerdaTec->RedeComCapacitor = StrToBool(redeComCapacitorTxt);
			CfgPerdaTec->Zat_mrt = StrToBool(zatTxt);
			CfgPerdaTec->Zat_ohm = StrToDouble(zatOhmTxt);
			CfgPerdaTec->TipoFluxo = StrToInt(fluxoTxt);
			CfgPerdaTec->ModeloCarga = StrToInt(modeloCargaTxt);
			CfgPerdaTec->FerroPorFora = StrToBool(ferroPorForaTxt);
			CfgPerdaTec->MedidorPorFora = StrToBool(medidorPorForaTxt);
			CfgPerdaTec->RamalPorFora = StrToBool(ramalPorForaTxt);
			CfgPerdaTec->OpcaoPerdaMedidor = StrToInt(opPerdaMedidorTxt);
			// CfgPerdaTec->PerdaMedidorUnico = StrToInt(medidorTxt);
			CfgPerdaTec->OpcaoPerdaRamal = StrToInt(opPerdaRamalTxt);
			// CfgPerdaTec->PerdaRamalUnico = StrToInt(ramalTxt);
			CfgPerdaTec->OpcaoPerdaTrafo = StrToInt(opPerdaTrafoTxt);
			// CfgPerdaTec->PerdaTrafoUnico = StrToInt(trafoTxt);
			CfgPerdaTec->OpcaoPerdaTrafoSED = StrToInt(opPerdaTrafoSEDTxt);
			CfgPerdaTec->PerdaAdicional_perc = StrToDouble(perdaAdicionalTxt);
			// CfgPerdaTec->ItemIndex = StrToInt(edicaoTxt);
		}

	}

}

// ---------------------------------------------------------------------------

void __fastcall TFormCfgPerdatec::CBoxMedidorInicia(void)
{
	// variáveis locais
	int index;
	PERDA_MEDIDOR *perda_medidor;

	// reincia CBoxMedidor
	CBoxMedidor->Clear();
	for (int n = 0; n < CfgPerdaTec->LisPerdasMedidor->Count; n++)
	{
		perda_medidor = (PERDA_MEDIDOR*)CfgPerdaTec->LisPerdasMedidor->Items[n];
		CBoxMedidor->Items->AddObject(perda_medidor->codigo, perda_medidor);
	}
	// pre-seleciona PERDA_MEDIDOR único
	if (CBoxMedidor->Items->Count > 0)
	{
		index = CBoxMedidor->Items->IndexOfObject(CfgPerdaTec->PerdaMedidorUnico);
		if (index < 0)
			index = 0;
		CBoxMedidor->ItemIndex = index;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::CBoxRamalInicia(void)
{
	// variáveis locais
	int index;
	AnsiString txt;
	PERDA_RAMAL *perda_ramal;

	// reincia CBoxRamal
	CBoxRamal->Clear();
	CBoxRamal->Sorted = false;
	for (int n = 0; n < CfgPerdaTec->LisPerdasRamal->Count; n++)
	{
		perda_ramal = (PERDA_RAMAL*)CfgPerdaTec->LisPerdasRamal->Items[n];
		// txt.sprintf("V= %4.3f / Fases: %d", perda_ramal->vmax_kv, perda_ramal->num_fases);
		txt.sprintf("Comp.(m)= %4.1f / R(ohm/km)= %6.4f", perda_ramal->comp_m,
			perda_ramal->r_ohm_km);
		// verifica se já existe este item no CBoxRamal
		if (CBoxRamal->Items->IndexOf(txt) < 0)
		{
			CBoxRamal->Items->AddObject(txt, perda_ramal);
		}
	}
	// pre-seleciona PERDA_RAMAL único
	if (CBoxRamal->Items->Count >= 0)
	{
		index = CBoxRamal->Items->IndexOfObject(CfgPerdaTec->PerdaRamalUnico);
		if (index < 0)
			index = 0;
		CBoxRamal->ItemIndex = index;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::CBoxTrafoInicia(void)
{
	// variáveis locais
	int index;
	AnsiString txt;
	PERDA_TRAFO *perda_trafo;

	// reincia CBoxRamal
	CBoxTrafo->Clear();
	CBoxTrafo->Sorted = false;
	// inclui todos PERDA_TRAFO da opção prodist
	for (int n = 0; n < CfgPerdaTec->LisPerdasTrafoProdist->Count; n++)
	{
		perda_trafo = (PERDA_TRAFO*)CfgPerdaTec->LisPerdasTrafoProdist->Items[n];
		// txt.sprintf("Vsec <= %4.3f / Snom=%4.3f Vazio=%1.0f Total=%1.0f",
		// perda_trafo->vmax_kv, perda_trafo->snom_kva, perda_trafo->perda_vazio_w, perda_trafo->perda_total_w);
		txt.sprintf("Vazio(W)=%4.0f Total(W)=%4.0f", perda_trafo->perda_vazio_w,
			perda_trafo->perda_total_w);
		// verifica se já existe este item no CBoxTrafo
		if (CBoxTrafo->Items->IndexOf(txt) < 0)
		{
			CBoxTrafo->Items->AddObject(txt, perda_trafo);
		}
	}
	// inclui todos PERDA_TRAFO da opção empresa
	for (int n = 0; n < CfgPerdaTec->LisPerdasTrafoEmpresa->Count; n++)
	{
		perda_trafo = (PERDA_TRAFO*)CfgPerdaTec->LisPerdasTrafoEmpresa->Items[n];
		// txt.sprintf("Vsec <= %4.3f / Snom=%4.3f Vazio=%1.0f Total=%1.0f",
		// perda_trafo->vmax_kv, perda_trafo->snom_kva, perda_trafo->perda_vazio_w, perda_trafo->perda_total_w);
		txt.sprintf("Vazio(W)=%4.0f Total(W)=%4.0f", perda_trafo->perda_vazio_w,
			perda_trafo->perda_total_w);
		// verifica se já existe este item no CBoxTrafo
		if (CBoxTrafo->Items->IndexOf(txt) < 0)
		{
			CBoxTrafo->Items->AddObject(txt, perda_trafo);
		}
	}
	// pre-seleciona PERDA_TRAFO único
	if (CBoxTrafo->Items->Count >= 0)
	{
		index = CBoxTrafo->Items->IndexOfObject(CfgPerdaTec->PerdaTrafoUnico);
		if (index < 0)
			index = 0;
		CBoxTrafo->ItemIndex = index;
	}
	// ordenar
	CBoxTrafo->Sorted = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::CheckBoxZatClick(TObject *Sender)
{
	EditZat->Visible = CheckBoxZat->Checked;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::ExibeDados(void)
{
	// variáveis locais
	double valor;
	PERDA_RAMAL *per_ramal;
	PERDA_MEDIDOR *per_med;
	PERDA_TRAFO *per_trafo;
	VTClasse *classe;

	// opção de fluxo: rede com ou sem neutro
	CheckBoxNeutro->Checked = CfgPerdaTec->RedeComNeutro;
	// opção de fluxo: banco de capacitor
	CheckBoxCapacitor->Checked = CfgPerdaTec->RedeComCapacitor;
	// opção de fluxo: Zat de redes MRT
	CheckBoxZat->Checked = CfgPerdaTec->Zat_mrt;
	EditZat->Text = DoubleToStr("%6.2f", CfgPerdaTec->Zat_ohm);
	// opção de fluxo: modelo de carga
	switch (CfgPerdaTec->ModeloCarga)
	{
	case cargaAUTO:
		CBoxModeloCarga->ItemIndex = 0;
		break;
	case cargaICTE:
		CBoxModeloCarga->ItemIndex = 1;
		break;
	case cargaSCTE:
		CBoxModeloCarga->ItemIndex = 2;
		break;
	case cargaZCTE:
		CBoxModeloCarga->ItemIndex = 3;
		break;
	case cargaANEEL:
		CBoxModeloCarga->ItemIndex = 4;
		break;
	default:
		CBoxModeloCarga->ItemIndex = 0;
		break;
	}
	// opção de fluxo: rede equilibrada/desequilibrada
	switch (CfgPerdaTec->TipoFluxo)
	{
	case fluxoGAUSS1:
		CheckBoxFluxo->Checked = true;
		break;
	case fluxoRADIAL1:
		CheckBoxFluxo->Checked = true;
		break;
	case fluxoNR1:
		CheckBoxFluxo->Checked = true;
		break;
	case fluxoEQUILIBRADO:
		CheckBoxFluxo->Checked = true;
		break;
	default:
		CheckBoxFluxo->Checked = false;
		break;
	}
	// perda por fora
	CheckBoxPerdaFerroPorDentro->Checked = (!CfgPerdaTec->FerroPorFora);
	CheckBoxPerdaMedidorPorDentro->Checked = (!CfgPerdaTec->MedidorPorFora);
	CheckBoxPerdaRamalPorDentro->Checked = (!CfgPerdaTec->RamalPorFora);
	// perdas em medidor
	switch (CfgPerdaTec->OpcaoPerdaMedidor)
	{
	case opMEDIDOR_NAO_USAR:
		RadioGroupMedidor->ItemIndex = 0;
		break;
	default:
		RadioGroupMedidor->ItemIndex = 1;
		break;
	}
	// perdas em ramal de ligação
	switch (CfgPerdaTec->OpcaoPerdaRamal)
	{
	case opRAMAL_NAO_USAR:
		RadioGroupRamal->ItemIndex = 0;
		break;
	case opRAMAL_GIS:
		RadioGroupRamal->ItemIndex = 1;
		break;
	case opRAMAL_SELECAO:
		RadioGroupRamal->ItemIndex = 2;
		break;
	case opRAMAL_UNICO:
		RadioGroupRamal->ItemIndex = 3;
		break;
	default:
		RadioGroupRamal->ItemIndex = 3;
		break;
	}
	// perdas em transformador SDBT
	switch (CfgPerdaTec->OpcaoPerdaTrafo)
	{
	case opTRAFO_GIS:
		RadioGroupTrafo->ItemIndex = 0;
		break;
	case opTRAFO_PRODIST:
		RadioGroupTrafo->ItemIndex = 1;
		break;
	case opTRAFO_EMPRESA:
		RadioGroupTrafo->ItemIndex = 2;
		break;
	case opTRAFO_UNICO:
		RadioGroupTrafo->ItemIndex = 3;
		break;
	default:
		RadioGroupTrafo->ItemIndex = 1;
		break;
	}
	// perdas em transformador SED
	switch (CfgPerdaTec->OpcaoPerdaTrafoSED)
	{
	case opTRAFO_GIS:
		RadioGroupTrafoSED->ItemIndex = 0;
		break;
	default:
		RadioGroupTrafoSED->ItemIndex = 1;
		break;
	}
	// perdas adicionais
	EditPerdaAdicional->Text = DoubleToStr("%6.2f", CfgPerdaTec->PerdaAdicional_perc);
	// simula seleção de opções
	RadioGroupMedidorClick(NULL);
	RadioGroupRamalClick(NULL);
	RadioGroupTrafoClick(NULL);
	// seleciona opção de exibir configuração de trafos monofásicos Prodist
	RadioGroupEdicao->ItemIndex = 2;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormCfgPerdatec::LeDados(void)
{
	// variáveis locais
	int index;
	double valor;

	// opção de fluxo: rede com ou sem neutro
	CfgPerdaTec->RedeComNeutro = CheckBoxNeutro->Checked;
	// opção de fluxo: rede com capacitor
	CfgPerdaTec->RedeComCapacitor = CheckBoxCapacitor->Checked;
	// opção de fluxo: Zat de redes MRT
	CfgPerdaTec->Zat_mrt = CheckBoxZat->Checked;
	if (CfgPerdaTec->Zat_mrt)
	{
		if (!StrToDoubleLocal(EditZat->Text, valor))
		{
			Aviso("MRT: Zaterramento com valor inválido");
			return (false);
		}
		CfgPerdaTec->Zat_ohm = valor;
	}
	// opção de fluxo: rede equilibrada/desequilibrada
	if (CheckBoxFluxo->Checked)
		CfgPerdaTec->TipoFluxo = fluxoEQUILIBRADO;
	else
		CfgPerdaTec->TipoFluxo = fluxoDESEQUILIBRADO;
	// opção de fluxo: modelo de carga
	switch (CBoxModeloCarga->ItemIndex)
	{
	case 0:
		CfgPerdaTec->ModeloCarga = cargaAUTO;
		break;
	case 1:
		CfgPerdaTec->ModeloCarga = cargaICTE;
		break;
	case 2:
		CfgPerdaTec->ModeloCarga = cargaSCTE;
		break;
	case 3:
		CfgPerdaTec->ModeloCarga = cargaZCTE;
		break;
	case 4:
		CfgPerdaTec->ModeloCarga = cargaANEEL;
		break;
	default:
		Aviso("Selecione modelo de carga");
		return (false);
	}
	// perda por fora
	CfgPerdaTec->FerroPorFora = !CheckBoxPerdaFerroPorDentro->Checked;
	CfgPerdaTec->MedidorPorFora = !CheckBoxPerdaMedidorPorDentro->Checked;
	CfgPerdaTec->RamalPorFora = !CheckBoxPerdaRamalPorDentro->Checked;
	// perdas em medidor
	if (RadioGroupMedidor->ItemIndex == 0)
	{
		CfgPerdaTec->OpcaoPerdaMedidor = opMEDIDOR_NAO_USAR;
		CfgPerdaTec->PerdaMedidorUnico = NULL;
	}
	else if ((index = CBoxMedidor->ItemIndex) >= 0)
	{
		CfgPerdaTec->OpcaoPerdaMedidor = opMEDIDOR_UNICO;
		CfgPerdaTec->PerdaMedidorUnico = (PERDA_MEDIDOR*)(CBoxMedidor->Items->Objects[index]);
	}
	else
	{
		Aviso("Selecione perda para medidor");
		return (false);
	}
	// perdas em ramal de ligação
	CfgPerdaTec->PerdaRamalUnico = NULL;
	switch (RadioGroupRamal->ItemIndex)
	{
	case 0:
		CfgPerdaTec->OpcaoPerdaRamal = opRAMAL_NAO_USAR;
		break;
	case 1:
		CfgPerdaTec->OpcaoPerdaRamal = opRAMAL_GIS;
		break;
	case 2:
		CfgPerdaTec->OpcaoPerdaRamal = opRAMAL_SELECAO;
		break;
	case 3:
	default:
		if ((index = CBoxRamal->ItemIndex) >= 0)
		{
			CfgPerdaTec->OpcaoPerdaRamal = opRAMAL_UNICO;
			CfgPerdaTec->PerdaRamalUnico = (PERDA_RAMAL*)(CBoxRamal->Items->Objects[index]);
		}
		else
		{
			Aviso("Selecione perda para ramal de ligação");
			return (false);
		}
	}
	// perdas em transformador SDBT
	CfgPerdaTec->PerdaTrafoUnico = NULL;
	switch (RadioGroupTrafo->ItemIndex)
	{
	case 0:
		CfgPerdaTec->OpcaoPerdaTrafo = opTRAFO_GIS;
		break;
	case 1:
		CfgPerdaTec->OpcaoPerdaTrafo = opTRAFO_PRODIST;
		break;
	case 2:
		CfgPerdaTec->OpcaoPerdaTrafo = opTRAFO_EMPRESA;
		break;
	case 3:
	default:
		if ((index = CBoxTrafo->ItemIndex) >= 0)
		{
			CfgPerdaTec->OpcaoPerdaTrafo = opTRAFO_UNICO;
			CfgPerdaTec->PerdaTrafoUnico = (PERDA_TRAFO*)(CBoxTrafo->Items->Objects[index]);
		}
		else
		{
			Aviso("Selecione perda para transformador");
			return (false);
		}
	}
	// perdas em transformador SED
	switch (RadioGroupTrafoSED->ItemIndex)
	{
	case 0:
		CfgPerdaTec->OpcaoPerdaTrafoSED = opTRAFO_GIS;
		break;
		// DVK 2016.04.04
		// default: CfgPerdaTec->OpcaoPerdaTrafo    = opTRAFO_EMPRESA; break;
	default:
		CfgPerdaTec->OpcaoPerdaTrafoSED = opTRAFO_EMPRESA;
		break;
	}
	// perdas adicionais
	if (!StrToDoubleLocal(EditPerdaAdicional->Text, valor))
	{
		Aviso("Perda adicional com valor inválido");
		return (false);
	}
	CfgPerdaTec->PerdaAdicional_perc = valor;
	EditPerdaAdicional->Text = DoubleToStr("%6.2f", CfgPerdaTec->PerdaAdicional_perc);

	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::LViewIniciaMedidor(TList *LisPerdasMedidor)
{
	// variáveis locais
	PERDA_MEDIDOR *perda_medidor;
	TListView *LView = LViewMedidor;
	TListItem *Item;

	// reinicia LView
	LView->Clear();
	LView->Align = alClient;
	// loop p/ todos PERDA_MEDIDOR em LisPerdasMedidor
	for (int n = 0; n < LisPerdasMedidor->Count; n++)
	{
		perda_medidor = (PERDA_MEDIDOR*)LisPerdasMedidor->Items[n];
		Item = LView->Items->Add();
		Item->Data = perda_medidor;
		Item->Caption = perda_medidor->codigo;
		Item->SubItems->Add(DoubleToStr("%2.1f", perda_medidor->perda_w));
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::LViewIniciaRamal(TList *LisPerdasRamal)
{
	// variáveis locais
	PERDA_RAMAL *perda_ramal;
	TListView *LView = LViewRamal;
	TListItem *Item;

	// reinicia LView
	LView->Clear();
	LView->Align = alClient;
	// loop p/ todos PERDA_RAMAL em LisPerdasRamal
	for (int n = 0; n < LisPerdasRamal->Count; n++)
	{
		perda_ramal = (PERDA_RAMAL*)LisPerdasRamal->Items[n];
		Item = LView->Items->Add();
		Item->Data = perda_ramal;
		Item->Caption = DoubleToStr("%4.3f", perda_ramal->vmax_kv);
		Item->SubItems->Add(IntToStr(perda_ramal->num_fases));
		Item->SubItems->Add(DoubleToStr("%2.1f", perda_ramal->comp_m));
		Item->SubItems->Add(DoubleToStr("%5.4f", perda_ramal->r_ohm_km));
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::LViewIniciaTrafo(TList *LisPerdasTrafo, int tipo_trf)
{
	// variáveis locais
	AnsiString txt;
	PERDA_TRAFO *perda_trafo;
	TListItem *Item;
	TListView *LView = LViewTrafo;

	// reinicia LView
	LView->Clear();
	LView->Align = alClient;
	// redefine títulos
	LView->Column[1]->Caption = "Snom(kVA)";
	LView->Column[2]->Caption = "Perda Vazio(W)";
	LView->Column[3]->Caption = "Perda Total(W)"; ;
	// loop p/ todos PERDA_TRAFO em LisPerdasRamal
	for (int n = 0; n < LisPerdasTrafo->Count; n++)
	{
		perda_trafo = (PERDA_TRAFO*)LisPerdasTrafo->Items[n];
		// verifica o tipo
		if (perda_trafo->tipo != tipo_trf)
			continue;
		Item = LView->Items->Add();
		Item->Data = perda_trafo;
		Item->Caption = DoubleToStr("<= %4.3f", perda_trafo->vmax_kv);
		Item->SubItems->Add(DoubleToStr("%2.1f", perda_trafo->snom_kva));
		Item->SubItems->Add(DoubleToStr("%2.1f", perda_trafo->perda_vazio_w));
		Item->SubItems->Add(DoubleToStr("%2.1f", perda_trafo->perda_total_w));
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::LViewIniciaTrafoSED(TList *LisPerdasTrafoSED)
{
	// variáveis locais
	AnsiString txt;
	PERDA_TRAFO_SED *perda_trafo_sed;
	TListItem *Item;
	TListView *LView = LViewTrafo;

	// reinicia LView
	LView->Clear();
	LView->Align = alClient;
	// redefine títulos
	LView->Column[1]->Caption = "Snom(MVA)";
	LView->Column[2]->Caption = "Perda Vazio(%)";
	LView->Column[3]->Caption = "Perda Total(%)"; ;
	// loop p/ todos PERDA_TRAFO em LisPerdasRamal
	for (int n = 0; n < LisPerdasTrafoSED->Count; n++)
	{
		perda_trafo_sed = (PERDA_TRAFO_SED*)LisPerdasTrafoSED->Items[n];
		// verifica o tipo
		Item = LView->Items->Add();
		Item->Data = perda_trafo_sed;
		Item->Caption = ("qualquer");
		if (perda_trafo_sed->snom_mva <= 0)
			Item->SubItems->Add(DoubleToStr("-", perda_trafo_sed->snom_mva));
		else
			Item->SubItems->Add(DoubleToStr("<= %2.1f", perda_trafo_sed->snom_mva));
		Item->SubItems->Add(DoubleToStr("%4.3f", perda_trafo_sed->perda_vazio_perc));
		Item->SubItems->Add(DoubleToStr("%4.3f", perda_trafo_sed->perda_total_perc));
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::RadioGroupEdicaoClick(TObject *Sender)
{
	// exibe/esconde ListView
	LViewMedidor->Visible = (RadioGroupEdicao->ItemIndex == 0);
	LViewRamal->Visible = (RadioGroupEdicao->ItemIndex == 1);
	LViewTrafo->Visible = (RadioGroupEdicao->ItemIndex >= 2);
	// exibe/esconde buttons
	ActionEditaMedidor->Visible = (RadioGroupEdicao->ItemIndex == 0);
	ActionEditaRamal->Visible = (RadioGroupEdicao->ItemIndex == 1);
	ActionEditaTrafoMonoProdist->Visible = (RadioGroupEdicao->ItemIndex == 2);
	ActionEditaTrafoTrifProdist->Visible = (RadioGroupEdicao->ItemIndex == 3);
	ActionEditaTrafoMonoEmpresa->Visible = (RadioGroupEdicao->ItemIndex == 4);
	ActionEditaTrafoTrifEmpresa->Visible = (RadioGroupEdicao->ItemIndex == 5);
	ActionEditaTrafoSED->Visible = (RadioGroupEdicao->ItemIndex == 6);
	// atualiza GBoxDados e LView
	switch (RadioGroupEdicao->ItemIndex)
	{
	case 0:
		GBoxDados->Caption = "SDBT: Perda em Medidor";
		LViewIniciaMedidor(CfgPerdaTec->LisPerdasMedidor);
		break;
	case 1:
		GBoxDados->Caption = "SDBT: Perda em Ramal de Ligação";
		LViewIniciaRamal(CfgPerdaTec->LisPerdasRamal);
		break;
	case 2:
		GBoxDados->Caption = "SDBT: Perda em Transformador Monofásico (Prodist)";
		LViewIniciaTrafo(CfgPerdaTec->LisPerdasTrafoProdist, trfMONOFASICO);
		break;
	case 3:
		GBoxDados->Caption = "SDBT: Perda em Transformador Trifásico (Prodist)";
		LViewIniciaTrafo(CfgPerdaTec->LisPerdasTrafoProdist, trfTRIFASICO);
		break;
	case 4:
		GBoxDados->Caption = "SDBT: Perda em Transformador Monofásico (empresa)";
		LViewIniciaTrafo(CfgPerdaTec->LisPerdasTrafoEmpresa, trfMONOFASICO);
		break;
	case 5:
		GBoxDados->Caption = "SDBT: Perda em Transformador Trifásico (empresa)";
		LViewIniciaTrafo(CfgPerdaTec->LisPerdasTrafoEmpresa, trfTRIFASICO);
		break;
	case 6:
		GBoxDados->Caption = "SED: Perda em Transformador Trifásico (empresa)";
		LViewIniciaTrafoSED(CfgPerdaTec->LisPerdasTrafoSED);
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::RadioGroupMedidorClick(TObject *Sender)
{
	// exibe/esconde CBoxMedidor
	CBoxMedidor->Visible = (RadioGroupMedidor->ItemIndex == 1);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::RadioGroupRamalClick(TObject *Sender)
{
	// exibe/esconde CBoxMRamal
	CBoxRamal->Visible = (RadioGroupRamal->ItemIndex == 3);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::RadioGroupTrafoClick(TObject *Sender)
{
	// exibe/esconde CBoxTrafo
	CBoxTrafo->Visible = (RadioGroupTrafo->ItemIndex == 3);
}

// ---------------------------------------------------------------------------
void __fastcall TFormCfgPerdatec::SalvaParametros(void)
{

	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // salva parametros do Form
		geral->Infoset->Clear();
		geral->Infoset->AddInfo("redeComNeutro", BoolToStr(CfgPerdaTec->RedeComNeutro));
		geral->Infoset->AddInfo("redeComCapacitor", BoolToStr(CfgPerdaTec->RedeComCapacitor));
		geral->Infoset->AddInfo("zat", BoolToStr(CfgPerdaTec->Zat_mrt));
		geral->Infoset->AddInfo("zatOhm", CfgPerdaTec->Zat_ohm);
		geral->Infoset->AddInfo("fluxo", CfgPerdaTec->TipoFluxo);
		geral->Infoset->AddInfo("modeloCarga", CfgPerdaTec->ModeloCarga);
		geral->Infoset->AddInfo("ferroPorFora", IntToStr((signed int)CfgPerdaTec->FerroPorFora));
		geral->Infoset->AddInfo("medidorPorFora",
			IntToStr((signed int)CfgPerdaTec->MedidorPorFora));
		geral->Infoset->AddInfo("ramalPorFora", IntToStr((signed int)CfgPerdaTec->RamalPorFora));
		geral->Infoset->AddInfo("opPerdaMedidor", CfgPerdaTec->OpcaoPerdaMedidor);
		// geral->Infoset->AddInfo("perdaMedidorUnico", CfgPerdaTec->PerdaMedidorUnico);
		geral->Infoset->AddInfo("opPerdaRamal", CfgPerdaTec->OpcaoPerdaRamal);
		// geral->Infoset->AddInfo("perdaRamalUnico", CfgPerdaTec->PerdaRamalUnico);
		geral->Infoset->AddInfo("opPerdaTrafo", CfgPerdaTec->OpcaoPerdaTrafo);
		// geral->Infoset->AddInfo("perdaTrafoUnico", CfgPerdaTec->PerdaTrafoUnico);
		geral->Infoset->AddInfo("opPerdaTrafoSED", CfgPerdaTec->OpcaoPerdaTrafoSED);
		geral->Infoset->AddInfo("perdaAdicional", CfgPerdaTec->PerdaAdicional_perc);
		// grava no arquivo
		geral->Infoset->SaveToFile("CfgPerdatec");
	}

}

// ---------------------------------------------------------------------------
int __fastcall TFormCfgPerdatec::ShowEdita(TListView *listView)
{ // variaveis locais
	// TFormCfgEdita *formCfgEdita;
	//
	// formCfgEdita = new TFormCfgEdita(this);
	// formCfgEdita->Inicia(listView);
	// this->Hide();
	// formCfgEdita->ShowModal();
	// delete formCfgEdita;
	// this->Show();
	//
	// return formCfgEdita->ModalResult;
	return 0;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormCfgPerdatec::StrToDoubleLocal(AnsiString text, double &valor)
{
	try
	{
		valor = StrToDouble(text);
		return (true);
	}
	catch (Exception &e)
	{
		return (false);
	}
}

// ---------------------------------------------------------------------------
// eof
