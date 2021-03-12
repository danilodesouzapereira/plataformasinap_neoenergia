// ---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#include <Math.hpp>
#pragma hdrstop
#include "TFormEdtReguladorBT.h"
// #include "TFormEditaPlan.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormEdtReguladorBT::TFormEdtReguladorBT(TComponent* Owner, VTApl *apl,
	VTRegulador *regulador) : TForm(Owner)
{
	// salva objeto da classe
	this->apl = apl;
	this->regulador = regulador;
	// cria lista
	lisEQP = new TList();
	// inicia CBoxTipoPot
	CBoxTipoPot->ItemIndex = 0;
	// inicia dados default p/ Eqpto novo
	DefValoresDefaultRegulador();
	// salva valor da pot�ncia passante
	spassante_kva = regulador->Spas_mva * 1000.;
	// inicia CBoxRede c/ as Redes
	IniciaCBoxRede();
	// inicia CBoxBarEntrada c/ as Barras do Regulador
	IniciaCBoxBarEntrada();
	// posiciona o Form
	FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
__fastcall TFormEdtReguladorBT::~TFormEdtReguladorBT(void)
{
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva par�metros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// apaga eventual moldura
	Moldura();
	// destr�i lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorBT::ActionCancelaExecute(TObject *Sender)
{
	// fecha o form
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorBT::ActionConfirmaExecute(TObject *Sender)
{
	// salva dados
	if (!(LeDados() && ValidaDados()))
		return;
	// fecha o form
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorBT::ActionRetiraExecute(TObject *Sender)
{
	// fecha o form indicando exclus�o do Eqpto
	ModalResult = mrNo;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorBT::CBoxBarEnter(TObject *Sender)
{
	// vari�veis locais
	TComboBox *CBoxBar = (TComboBox*)Sender;

	// exibe Moldura na Barra selecionada
	if (CBoxBar->ItemIndex < 0)
	{ // apaga Moldura
		Moldura();
	}
	else
	{ // exibe MOldura na Barra selecionada
		Moldura((VTEqpto*)(CBoxBar->Items->Objects[CBoxBar->ItemIndex]));
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorBT::CBoxBarEntradaChange(TObject *Sender)
{
	// verifica se h� uma Barra selecionada
	if (CBoxBarEntrada->ItemIndex < 0)
	{ // apaga eventual moldura
		Moldura();
		return;
	}
	// exibe moldura na Barra selecionada
	Moldura((VTBarra*)CBoxBarEntrada->Items->Objects[CBoxBarEntrada->ItemIndex]);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorBT::CBoxTipoPotChange(TObject *Sender)
{
	// vari�veis locais
	double s_kva, dv_perc;

	try
	{ // verifica se est� selecionada uma op��o v�lida em CBoxTipoPot
		if (CBoxTipoPot->ItemIndex < 0)
			return;
		// l� valor de pot�ncia em EditSmva
		s_kva = StrToDouble(EditS_kva->Text);
		dv_perc = StrToDouble(UpDownFaixaTensao->Position);
		// verifica qual a nova op��o de CBoxTipoPot
		if (CBoxTipoPot->ItemIndex == 0)
		{ // salva valor da pot�ncia passante
			spassante_kva = s_kva;
			// converte valor de passante para nominal
			// DVK 2014.11.12 mant�m s_kva (reclama��o de usu�rios)
			// s_kva = SpassanteToSnom(s_kva, dv_perc);
		}
		else
		{ // converte valor de nominal para passante
			s_kva = SnomToSpassante(s_kva, dv_perc);
			// salva valor da pot�ncia passante
			spassante_kva = s_kva;
		}
		// reapresenta o valor em EditSmva
		EditS_kva->Text = DoubleToStr("%4.3f", s_kva);
	}
	catch (Exception &e)
	{
		// nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorBT::DefValoresDefaultRegulador(void)
{
	// vari�veis locais
	VTRede *rede;
	VTRegulador *ult_reg;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// verifica se o Regulador n�o est� definido
	if ((regulador->snom == 0.) || (regulador->var_tensao == 0.) || (regulador->num_passo == 0))
	{ // define Snom c/ 30kVA
		regulador->snom = 0.30;
		// assume tipo de pot�ncia indefinido
		CBoxTipoPot->ItemIndex = -1;

		regulador->ligacao = lgEST_ISO;
		regulador->fases = faseABC;
		regulador->by_pass = false;
		regulador->var_tensao = 10.;
		regulador->num_passo = 15;
		regulador->modelo = reguENERQCT;
		// sentido do fluxo
		regulador->sentido.bar_ref = regulador->pbarra1;
		// configura��o autom�tico
		regulador->cfg_auto.bar_ref = regulador->pbarra2;
		regulador->cfg_auto.bar_ref_inv = regulador->pbarra1;
		regulador->cfg_auto.v_pu = 1.0;
		regulador->cfg_auto.vinv_pu = 1.0;
		// configura��o fixo
		regulador->cfg_fixo.passo = 0;
		// verifica se existe algum Regulador na Rede
		lisEQP->Clear();
		redes->LisLigacao(lisEQP, eqptoREGULADOR);
		if (lisEQP->Count == 0)
			return;
		if ((ult_reg = (VTRegulador*)lisEQP->Last()) == NULL)
			return;
		// copia dados do Regulador
		regulador->snom = ult_reg->snom;
		regulador->ligacao = ult_reg->ligacao;
		regulador->var_tensao = ult_reg->var_tensao;
		regulador->num_passo = ult_reg->num_passo;
		// configura��o autom�tico
		regulador->cfg_auto.v_pu = ult_reg->cfg_auto.v_pu;
		regulador->cfg_auto.vinv_pu = ult_reg->cfg_auto.vinv_pu;
		// configura��o fixo
		regulador->cfg_fixo.passo = ult_reg->cfg_fixo.passo;
	}
}

/* ---------------------------------------------------------------------------
 �ndices de fases do regulador
 [ 0] 1 unidade:  Fases AB
 [ 1] 1 unidade:  Fases BC
 [ 2] 1 unidade:  Fases CA
 [ 3] 1 unidade:  Fases AN
 [ 4] 1 unidade:  Fases BN
 [ 5] 1 unidade:  Fases CN
 [ 6] 1 unidade:  Fases ABN
 [ 7] 1 unidade:  Fases BCN
 [ 8] 1 unidade:  Fases CAN
 [ 9] 2 unidades: Delta aberto (CA, AB)
 [10] 2 unidades: Delta aberto (AB, BC)
 [11] 2 unidades: Delta aberto (BC, CA)
 [12] 3 unidades: Delta fechado
 [13] 3 unidades: Estrela
 //--------------------------------------------------------------------------- */
void __fastcall TFormEdtReguladorBT::ExibeDadosRegulador(void)
{
	// vari�veis locais
	double s_kva;

	// exibe dados basicos do regulador
	EditCodigo->Text = regulador->Codigo;
	// exibe pot�ncia nominal ou passante, em fun��o do CBoxTipoPot
	s_kva = regulador->snom * 1000.; // MVA =>kVa;
	if (CBoxTipoPot->ItemIndex == 1)
		s_kva = SnomToSpassante(s_kva, regulador->var_tensao);
	EditS_kva->Text = DoubleToStr("%4.3f", s_kva);
	checkBypass->Checked = regulador->by_pass;
	// exibe configura��o (unidades/fases)
	switch (regulador->ligacao)
	{
	case lgEST_ISO: // 3 unidades, estrela
		CBoxConfiguracao->ItemIndex = 13;
		break;
	case lgDELTA_FECHADO: // 3 unidades, delta fechado
		CBoxConfiguracao->ItemIndex = 12;
		break;
	case lgDELTA_ABERTO: // 2 unidades, delta aberto
		// verifica a fase comum das unidade
		switch (regulador->fases)
		{
		case faseA:
			CBoxConfiguracao->ItemIndex = 9;
			break;
		case faseB:
			CBoxConfiguracao->ItemIndex = 10;
			break;
		case faseC:
			CBoxConfiguracao->ItemIndex = 11;
			break;
		default:
			CBoxConfiguracao->ItemIndex = 9;
			break;
		}
		break;
	case lgMONOFASICO: // 1 unidade
		// verifica a fase da unidade
		switch (regulador->fases)
		{
		case faseAB:
			CBoxConfiguracao->ItemIndex = 0;
			break;
		case faseBC:
			CBoxConfiguracao->ItemIndex = 1;
			break;
		case faseCA:
			CBoxConfiguracao->ItemIndex = 2;
			break;
		case faseAN:
			CBoxConfiguracao->ItemIndex = 3;
			break;
		case faseBN:
			CBoxConfiguracao->ItemIndex = 4;
			break;
		case faseCN:
			CBoxConfiguracao->ItemIndex = 5;
			break;
		case faseABN:
			CBoxConfiguracao->ItemIndex = 6;
			break;
		case faseBCN:
			CBoxConfiguracao->ItemIndex = 7;
			break;
		case faseCAN:
			CBoxConfiguracao->ItemIndex = 8;
			break;
		default:
			CBoxConfiguracao->ItemIndex = 0;
			break;
		}
		break;
	default: // 3 unidades, estrela
		CBoxConfiguracao->ItemIndex = 13;
		break;
	}
	// exibe faixa de tens�o e faixa de passos
	UpDownFaixaTensao->Position = int(regulador->var_tensao);
	UpDownFaixaPasso->Position = regulador->num_passo;
	// seleciona barra de entrada
	if (CBoxBarEntrada->ItemIndex < 0)
		CBoxBarEntrada->ItemIndex = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorBT::FormClose(TObject *Sender, TCloseAction &Action)
{
	// esconde o Form
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorBT::FormIniciaPosicao(void)
{
	// vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // l� par�metros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
	// posiciona o Form sem alinhamento
	Left = 0;
	Top = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorBT::FormShow(TObject *Sender)
{
	// exibe dados do Regulador
	ExibeDadosRegulador();
	// posiciona o Cursor em EditCodigo
	EditCodigo->SetFocus();
	// FKM - 2015.10.07 Ajusta o tamanho do form \\
	this->AutoSize = true;
	this->AutoSize = false;
	// FKM - 2015.10.07 Ajusta o tamanho do form //
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorBT::IniciaCBoxBarEntrada(void)
{
	// vari�veis locais
	int index;

	// reinicia CBoxBarEntrada
	CBoxBarEntrada->Clear();
	// insere as duas Barras do Regulador
	CBoxBarEntrada->Items->AddObject(regulador->pbarra1->Codigo, regulador->pbarra1);
	CBoxBarEntrada->Items->AddObject(regulador->pbarra2->Codigo, regulador->pbarra2);
	// verifica se a Barra que indica sentido do fluxo est� definida
	if (regulador->sentido.bar_ref == NULL)
		index = 0;
	else
	{
		index = CBoxBarEntrada->Items->IndexOfObject(regulador->sentido.bar_ref);
	}
	CBoxBarEntrada->ItemIndex = index;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorBT::IniciaCBoxRede(void)
{
	// vari�veis locais
	int index;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();

	// reinicia CBox c/ as Redes
	CBoxRede->Clear();
	if (lisREDE->Count == 0)
		return;
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		if (!rede->Carregada)
			continue;
		CBoxRede->Items->AddObject(rede->Codigo, rede);
	}
	// define ordem alfab�tica
	CBoxRede->Sorted = true;
	// pr�-seleciona Rede que conecta as Barras do Regulador
	index = 0;
	if ((rede = SelecionaRede(lisREDE)) != NULL)
		index = CBoxRede->Items->IndexOfObject(rede);
	CBoxRede->ItemIndex = index;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtReguladorBT::LeDados(void)
{
	// vari�veis locais
	double s_kva;

	// determina Rede do Regulador
	regulador->rede = (VTRede*)CBoxRede->Items->Objects[CBoxRede->ItemIndex];
	// l� dados basicos
	regulador->Codigo = EditCodigo->Text;
	regulador->by_pass = checkBypass->Checked;
	regulador->var_tensao = UpDownFaixaTensao->Position;
	regulador->num_passo = UpDownFaixaPasso->Position;
	// l� pot�ncia editada
	s_kva = StrToDouble(EditS_kva->Text);
	// verifica se pot.editada foi definida como passante
	if (CBoxTipoPot->ItemIndex)
	{ // calcula pot�ncia nominal
		s_kva = SpassanteToSnom(s_kva, regulador->var_tensao);
	}
	regulador->snom = s_kva * 0.001; // kVA => MVA
	// sentido do fluxo
	regulador->sentido.bar_ref =
		(VTBarra*)(CBoxBarEntrada->Items->Objects[CBoxBarEntrada->ItemIndex]);
	// define tipo de liga��o e fases
	switch (CBoxConfiguracao->ItemIndex)
	{
	case 0: // 1 unidade, faseAB
		regulador->ligacao = lgMONOFASICO;
		regulador->fases = faseAB;
		break;
	case 1: // 1 unidade, faseBC
		regulador->ligacao = lgMONOFASICO;
		regulador->fases = faseBC;
		break;
	case 2: // 1 unidade, faseCA
		regulador->ligacao = lgMONOFASICO;
		regulador->fases = faseCA;
		break;
	case 3: // 1 unidade, faseAN
		regulador->ligacao = lgMONOFASICO;
		regulador->fases = faseAN;
		break;
	case 4: // 1 unidade, faseBN
		regulador->ligacao = lgMONOFASICO;
		regulador->fases = faseBN;
		break;
	case 5: // 1 unidade, faseCN
		regulador->ligacao = lgMONOFASICO;
		regulador->fases = faseCN;
		break;
	case 6: // 1 unidade, faseABN
		regulador->ligacao = lgMONOFASICO;
		regulador->fases = faseABN;
		break;
	case 7: // 1 unidade, faseBCN
		regulador->ligacao = lgMONOFASICO;
		regulador->fases = faseBCN;
		break;
	case 8: // 1 unidade, faseCAN
		regulador->ligacao = lgMONOFASICO;
		regulador->fases = faseCAN;
		break;
	case 9: // 2 unidades, delta aberto, faseA como fase comum
		regulador->ligacao = lgDELTA_ABERTO;
		regulador->fases = faseA;
		break;
	case 10: // 2 unidades, delta aberto, faseB como fase comum
		regulador->ligacao = lgDELTA_ABERTO;
		regulador->fases = faseB;
		break;
	case 11: // 2 unidades, delta aberto, faseC como fase comum
		regulador->ligacao = lgDELTA_ABERTO;
		regulador->fases = faseC;
		break;
	case 12: // 3 unidades, delta fechado
		regulador->ligacao = lgDELTA_FECHADO;
		regulador->fases = faseABC;
		break;
	case 13: // 3 unidades, estrela
		regulador->ligacao = lgEST_ISO;
		regulador->fases = faseABC;
		break;
	default: // 3 unidades, estrela
		regulador->ligacao = lgEST_ISO;
		regulador->fases = faseABC;
		break;
	}
	// define op��es gr�ficas
	regulador->Show[eqp_FIXO] = true;
	regulador->Show[lig_CARGTO] = true;
	regulador->Show[lig_S] = true;

	// informa��es especificas do modelo
	regulador->modelo = reguENERQCT;
	// define barra de refer�ncia direta igual � barra de entrada
	regulador->cfg_auto.bar_ref =
		(VTBarra*)CBoxBarEntrada->Items->Objects[CBoxBarEntrada->ItemIndex];
	regulador->cfg_auto.bar_ref_inv =
		(VTBarra*)CBoxBarEntrada->Items->Objects[CBoxBarEntrada->ItemIndex];
	// define demais dados
	regulador->cfg_auto.v_pu = 1.;
	regulador->cfg_auto.vinv_pu = 1.;
	regulador->cfg_fixo.passo = 0;

	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorBT::Moldura(VTEqpto *eqpto)
{
	// vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	grafico->Moldura(eqpto);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorBT::OpcaoRetira(bool enabled)
{
	ActionRetira->Visible = enabled;
}

// ---------------------------------------------------------------------------
VTRede* __fastcall TFormEdtReguladorBT::SelecionaRede(TList *lisREDE)
{
	// vari�veis locais
	VTRede *rede;

	// verifica se o Trafo j� possui uma Rede definida
	if (regulador->rede)
		return (regulador->rede);
	// procura uma Rede que conecta as Barras do Regulador
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		if ((rede->ExisteBarra(regulador->pbarra1)) || (rede->ExisteBarra(regulador->pbarra2)))
			return (rede);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
double __fastcall TFormEdtReguladorBT::SnomToSpassante(double Snom, double dv_perc)
{
	// vari�veis locais
	double Spas = Snom;

	if (dv_perc > 0)
	{ // pot�ncia passante
		Spas = Snom / (dv_perc * 0.01);
	}
	return (Spas);
}

// ---------------------------------------------------------------------------
double __fastcall TFormEdtReguladorBT::SpassanteToSnom(double Spas, double dv_perc)
{
	// vari�veis locais
	double Snom = Spas;

	if (dv_perc > 0)
	{ // pot�ncia passante
		Snom = Spas * (dv_perc * 0.01);
	}
	return (Snom);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtReguladorBT::ValidaDados(void)
{
	// variaveis locais
	double valor;

	try
	{ // prote��o: verifica se a Rede est� definida
		if (CBoxRede->ItemIndex < 0)
		{
			Aviso("Defina a rede do regulador de tens�o");
			CBoxRede->DroppedDown = true; // FKM 2015.10.01
			return (false);
		}
		if (CBoxTipoPot->ItemIndex < 0)
		{
			Aviso("Defina se a Pot�ncia � Nominal ou Passante");
			CBoxTipoPot->DroppedDown = true; // FKM 2015.10.01
			return (false);
		}
		if (CBoxConfiguracao->ItemIndex < 0)
		{
			Aviso("Selecione uma configura��o para as unidades monof�sicas.");
			CBoxConfiguracao->DroppedDown = true; // FKM 2015.10.01
			return (false);
		}
		if (CBoxBarEntrada->ItemIndex < 0)
		{
			Aviso("Defina a barra a montante");
			CBoxBarEntrada->DroppedDown = true; // FKM 2015.10.01
			return (false);
		}
		if (EditS_kva->Text.IsEmpty())
		{
			Aviso("Digite um valor para pot�ncia nominal.");
			return (false);
		}
		if (!StrToDouble(EditS_kva->Text))
		{
			Aviso("Pot�ncia nominal inv�lida.");
			return (false);
		}

		// verifica compatibilidade de fases do regulador com a rede
		if (regulador->pbarra1->zona != NULL)
		{ // DVK 2016.09.08 verifica se a zona � indefinida
			if (regulador->pbarra1->zona->Tipo == zonaINDEFINIDA)
				return (true);
			// if ((regulador->fases & faseABC) != (regulador->pbarra1->zona->Fases & faseABC))
			if ((regulador->fases & regulador->pbarra1->zona->Fases) != regulador->fases)
			{
				Aviso("N�mero de fases do regulador incompat�vel com as fases da rede.");
				return (false);
			}
			if ((regulador->fases > 7) && ((regulador->fases & faseN) !=
				(regulador->pbarra1->zona->Fases & faseN)))
			{
				Aviso("N�o existe neutro na rede ou no regulador.");
				return (false);
			}
		}
	}
	catch (Exception &e)
	{
		Aviso("Texto inv�lido: " + e.Message);
		return (false);
	}
	return (true);

}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorBT::UpDownFaixaPassoClick(TObject *Sender, TUDBtnType Button)
{
	// vari�veis locais
	AnsiString txt;

	// atualiza EditFaixaPasso
	txt.sprintf("-%02d/%02d", UpDownFaixaPasso->Position, UpDownFaixaPasso->Position);
	EditFaixaPasso->Text = txt;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorBT::UpDownFaixaTensaoClick(TObject *Sender, TUDBtnType Button)
{
	// vari�veis locais
	AnsiString txt;
	int pos = 0;

	// DVK 2014.11.12 limita faixa de tens�o at� 10%
	UpDownFaixaTensao->Max = 10;
	// atualiza EditFaixaTensao
	pos = SendMessage(UpDownFaixaTensao->Handle, (UINT)UDM_GETPOS32, 0, 0); // FKM 2016.02.24
	// txt.sprintf("-%02d/%02d", UpDownFaixaTensao->Position, UpDownFaixaTensao->Position);
	txt.sprintf("-%02d/%02d", pos, pos); // FKM 2016.02.24
	EditFaixaTensao->Text = txt;

}

// ---------------------------------------------------------------------------
// eof
