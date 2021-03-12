// ---------------------------------------------------------------------------
#include <vcl.h>
#include <map>
#include <math.h>
#include <Math.hpp>
#pragma hdrstop
#include "TFormEdtReguladorMT.h"
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
__fastcall TFormEdtReguladorMT::TFormEdtReguladorMT(TComponent* Owner, VTApl *apl,
	VTRegulador *regulador) : TForm(Owner)
{
	// salva objeto da classe
	this->apl = apl;
	this->regulador = regulador;
	// cria lista
	lisEQP = new TList();
	// inicia CBoxTipoPot
	IniciaCBoxTipoPotCorr();
	// inicia maps
	IniciaMaps();
	// inicia dados default p/ Eqpto novo
	DefValoresDefaultRegulador();
	// inicia CBoxRede c/ as Redes
	IniciaCBoxRede();
	// inicia CBoxBarEntrada c/ as Barras do Regulador
	IniciaCBoxBarEntrada();
	// Regulador padrão: inicia CBoxBarSaida c/ as Barras do Regulador
	IniciaCBoxBarSaida();
	// inicia barras de tensão controlada
	IniciaCBoxBarControle();
	// posiciona o Form
	FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
__fastcall TFormEdtReguladorMT::~TFormEdtReguladorMT(void)
{
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// apaga eventual moldura
	Moldura();
	// destrói lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::ActionBarrasDefaultExecute(TObject *Sender)
{
	// variaveis locais
	VTBarra *barra;

	// determina barra de saida
	barra = (VTBarra*)CBoxBarSaida->Items->Objects[CBoxBarSaida->ItemIndex];
	CBoxBarraDireto->ItemIndex = CBoxBarraDireto->Items->IndexOfObject(barra);
	// determina barra de entrada
	barra = (VTBarra*)CBoxBarEntrada->Items->Objects[CBoxBarEntrada->ItemIndex];
	CBoxBarraInverso->ItemIndex = CBoxBarraInverso->Items->IndexOfObject(barra);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::ActionCancelaExecute(TObject *Sender)
{
	// fecha o form
	ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::ActionConfirmaExecute(TObject *Sender)
{
	// salva dados
	// if (!(LeDados() && ValidaDados()))
	if (!(ValidaDados() && LeDados()))
		return;
	// fecha o form
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::ActionRetiraExecute(TObject *Sender)
{
	// fecha o form indicando exclusão do Eqpto
	ModalResult = mrNo;
}

/*
 // ---------------------------------------------------------------------------
 void __fastcall TFormEdtReguladorMT::ActionSelPassoFixoExecute(TObject *Sender)
 {
 // variáveis locais
 double dv;
 double dv_real, dv_imag;

 // atualiza EditFixoPasso
 EditFixoPasso->Text = IntToStr(TrackBarPassoFixo->Position);
 // caso especial: delta fechado
 if (CBoxConfiguracao->ItemIndex == 12)
 { // calcula dv
 dv = double(UpDownFaixaTensao->Position * TrackBarPassoFixo->Position) / double(passoMax);
 // (UpDownFaixaPasso->Position);
 dv_real = (1.5 * (1. + (dv / 100.))) - 0.5;
 dv_imag = (0.5 * sqrt(3.0)) * ((1. + (dv / 100.)) - 1.0);
 dv = (sqrt((dv_real * dv_real) + (dv_imag * dv_imag)) - 1.0) * 100.;
 }
 else
 { // atualiza EditFixoTensao
 dv = double(UpDownFaixaTensao->Position * TrackBarPassoFixo->Position) / double(passoMax);
 // (UpDownFaixaPasso->Position);
 }
 EditFixoTensao->Text = DoubleToStr("%2.1f", dv);
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::ActionSelPassoFixoExecute(TObject *Sender)
{
	// variáveis locais
	double dv, faixa_tensao;
	double dv_real, dv_imag;
	int passoMax;

	// atualiza EditFixoPasso
	EditFixoPasso->Text = IntToStr(TrackBarPassoFixo->Position);
	faixa_tensao = DeterminaFaixaTensao();
	passoMax = TrackBarPassoFixo->Max;
	passoTemp = TrackBarPassoFixo->Position;
	// caso especial: delta fechado
	if (CBoxConfiguracao->ItemIndex == 12)
	{ // calcula dv
		dv = double(faixa_tensao * TrackBarPassoFixo->Position) / double(passoMax);
		dv_real = (1.5 * (1. + (dv / 100.))) - 0.5;
		dv_imag = (0.5 * sqrt(3.0)) * ((1. + (dv / 100.)) - 1.0);
		dv = (sqrt((dv_real * dv_real) + (dv_imag * dv_imag)) - 1.0) * 100.;
	}
	else
	{ // atualiza EditFixoTensao
		dv = double(faixa_tensao * TrackBarPassoFixo->Position) / double(passoMax);
	}
	EditFixoTensao->Text = DoubleToStr("%3.2f", dv);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::ActionSelTipoAjusteExecute(TObject *Sender)
{
	// verifica o tipo de ajuste selecionado
	switch (ComboBoxTipoAjuste->ItemIndex)
	{
	case 0: // ajuste fixo
		GroupBoxVrefDir->Visible = false;
		GroupBoxVrefInv->Visible = false;
		gboxTapFixo->Visible = true;
		gboxBMdir->Visible = false;
		gboxBMinv->Visible = false;
		RadioGroupBarraRef->Visible = false;
		psheetReferencia->TabVisible = false;
		CarregaPassos();
		// FKM - 2015.10.07 Ajusta o tamanho do form \\
		this->AutoSize = true;
		this->AutoSize = false;
		// FKM - 2015.10.07 Ajusta o tamanho do form //
		break;
	case 1: // ajuste automático bidirecional
		GroupBoxVrefInv->Visible = true;
		GroupBoxVrefDir->Visible = true;
		gboxTapFixo->Visible = false;
		// PanelDireto->Visible = true;
		// PanelInverso->Visible = true;
		// gboxBandaMorta->Visible = true;
		// gboxBandaMorta->Height = 126;//70;
		gboxBMdir->Visible = true;
		gboxBMinv->Visible = true;
		RadioGroupBarraRef->Visible = true;
		psheetReferencia->TabVisible = true;
		// FKM - 2015.10.07 Ajusta o tamanho do form \\
		this->AutoSize = true;
		this->AutoSize = false;
		// FKM - 2015.10.07 Ajusta o tamanho do form //
		break;
	case 2: // ajuste automático (outros)
	case 3:
	case 4: // HPS, 2018.04.05
		GroupBoxVrefDir->Visible = true;
		GroupBoxVrefInv->Visible = false;
		gboxTapFixo->Visible = false;
		// PanelDireto->Visible = true;
		// PanelInverso->Visible = false;
		// gboxBandaMorta->Visible = true;
		// gboxBandaMorta->Height = 72;//44;
		gboxBMdir->Visible = true;
		gboxBMinv->Visible = false;
		RadioGroupBarraRef->Visible = true;
		psheetReferencia->TabVisible = true;
		// FKM - 2015.10.07 Ajusta o tamanho do form \\
		this->AutoSize = true;
		this->AutoSize = false;
		// FKM - 2015.10.07 Ajusta o tamanho do form //
		break;

	default:
		Aviso("Selecione um tipo de operação para o regulador de tensão");
		break;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::AtualizaTrackBarPassofixo(void)
{
	int n_passos;

	n_passos = DeterminaFaixaPassos();

	TrackBarPassoFixo->Max = n_passos;
	TrackBarPassoFixo->Min = -n_passos;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::CarregaPassos(void)
{
	TrackBarPassoFixo->Position = passoTemp;
	ActionSelPassoFixo->Execute();
}

// ---------------------------------------------------------------------------
double __fastcall TFormEdtReguladorMT::ConverteInsensibilidadeVtoPerc(double ins_v, double nv_v)
{
	double ins_perc;
	double rtp;

	rtp = StrToDouble(EditRTP->Text);
	if (!IsDoubleZero(nv_v))
	{
		// ins_perc = (ins_v*rtp*100)/nv_v;
		ins_perc = (ins_v / nv_v) * 100;
	}
	else
	{
		ins_perc = 0;
	}

	return ins_perc;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::CBoxBarEnter(TObject *Sender)
{
	// variáveis locais
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
void __fastcall TFormEdtReguladorMT::CBoxBarEntradaChange(TObject *Sender)
{
	// variaveis locais
	VTBarra *barra;
	AnsiString texto;

	// verifica se há uma Barra selecionada
	if (CBoxBarEntrada->ItemIndex < 0)
	{ // apaga eventual moldura
		Moldura();
		return;
	}
	// atualiza CBoxBarSaida
	CBoxBarSaida->ItemIndex = 1 - CBoxBarEntrada->ItemIndex;
	// exibe moldura na Barra selecionada
	barra = (VTBarra*)CBoxBarEntrada->Items->Objects[CBoxBarEntrada->ItemIndex];
	Moldura(barra);
	// altera label da barra no sentido inverso
	// if (barra->Codigo.IsEmpty())
	// texto = "Id: " + IntToStr(barra->Id);
	// else
	// texto = barra->Codigo;
	// LabelBarraInverso->Caption = "Fluxo inverso (barra " + texto + ")";
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::CBoxBarSaidaChange(TObject *Sender)
{
	// variaveis locais
	VTBarra *barra;
	AnsiString texto;

	// verifica se há uma Barra selecionada
	if (CBoxBarSaida->ItemIndex < 0)
	{ // apaga eventual moldura
		Moldura();
		return;
	}
	// atualiza CBoxBarEntrada
	CBoxBarEntrada->ItemIndex = 1 - CBoxBarSaida->ItemIndex;
	// exibe moldura na Barra selecionada
	barra = (VTBarra*)CBoxBarSaida->Items->Objects[CBoxBarSaida->ItemIndex];
	Moldura(barra);
	// altera label da barra no sentido direto
	// if (barra->Codigo.IsEmpty())
	// texto = "Id: " + IntToStr(barra->Id);
	// else
	// texto = barra->Codigo;
	// LabelBarraDireto->Caption = "Fluxo direto (barra " + texto + ")";
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::CBoxBarraDiretoChange(TObject *Sender)
{
	// variaveis locais
	VTBarra *barra;
	AnsiString texto;

	// verifica se há uma Barra selecionada
	if (CBoxBarraDireto->ItemIndex < 0)
	{ // apaga eventual moldura
		Moldura();
		return;
	}
	// exibe moldura na Barra selecionada
	barra = (VTBarra*)CBoxBarraDireto->Items->Objects[CBoxBarraDireto->ItemIndex];
	Moldura(barra);
	// verifica se a barra é do regulador
	if ((barra != regulador->pbarra1) && (barra != regulador->pbarra2))
	{
		RadioGroupBarraRef->ItemIndex = 1;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::CBoxBarraInversoChange(TObject *Sender)
{
	// variaveis locais
	VTBarra *barra;
	AnsiString texto;

	// verifica se há uma Barra selecionada
	if (CBoxBarraInverso->ItemIndex < 0)
	{ // apaga eventual moldura
		Moldura();
		return;
	}
	// exibe moldura na Barra selecionada
	barra = (VTBarra*)CBoxBarraInverso->Items->Objects[CBoxBarraInverso->ItemIndex];
	Moldura(barra);
	// verifica se a barra é do regulador
	if ((barra != regulador->pbarra1) && (barra != regulador->pbarra2))
	{
		RadioGroupBarraRef->ItemIndex = 1;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::CBoxConfiguracaoChange(TObject *Sender)
{
	// atualiza nivel de tensão direta (leitura)
	double nv = ValorNivelTensaoDir(); // V
	EditNTdir->Text = DoubleToStr("%3.2f", nv);
	// nivel de tensao inversa (leitura)
	nv = ValorNivelTensaoInv(); // V
	EditNTinv->Text = DoubleToStr("%3.2f", nv);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::cboxUnidadeBMChange(TObject *Sender)
{
	// variáveis locais
	double ins, rtp, nv, bm_inf, bm_sup;
	AnsiString texto;

	// proteção
	EditBMdireto->Text = "";
	ins = StrToDouble(edtInsDireto->Text);
	if (ins < 0.)
		return;
	// if (!StrToDouble(edtInsDireto->Text))
	// return;
	nv = ValorNivelTensaoDir(); // V
	try
	{
		rtp = StrToInt(EditRTP->Text);
		if (rtp <= 0)
			return;
	}
	catch (Exception &e)
	{
		Aviso("Relação de Tap deve ser um número.");
		return;
	}
	if (nv <= 0.)
	{
		Aviso("Relação de Tap deve ser um número.");
		return;
	}
	// calcula banda morta
	switch (cboxUnidadeBM->ItemIndex)
	{
	case 1: // %
		// proteção
		if (ins > 100)
		{
			Aviso("Valor de insensibilidade maior que 100%.");
			return;
		}
		bm_inf = (rtp * (nv * (1. - (ins / 100.))));
		bm_sup = (rtp * (nv * (1. + (ins / 100.))));
		// bm_inf = (rtp * nv) - (ins*ReguladorVnom_kv()*10); //*1000*0.01 = *10
		// bm_sup = (rtp * nv) + (ins*ReguladorVnom_kv()*10);
		break;
	case 0: // V
		// proteção
		if (ins > nv)
		{
			Aviso("Valor de insensibilidade maior que valor de nivel de tensão.");
			return;
		}
		bm_inf = (rtp * (nv - ins));
		bm_sup = (rtp * (nv + ins));
		break;
	default:
		break;
	}
	// monta texto
	texto = DoubleToStr("%4.1f", bm_inf) + " < V < " + DoubleToStr("%4.1f", bm_sup);
	EditBMdireto->Text = texto;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::cboxUnidadeBMInvChange(TObject *Sender)
{
	// variáveis locais
	double ins, rtp, nv, bm_inf, bm_sup;
	AnsiString texto;

	// proteção
	EditBMinverso->Text = "";
	ins = StrToDouble(edtInsInverso->Text);
	if (ins < 0.)
		return;
	// if (!StrToDouble(edtInsInverso->Text))
	// return;
	nv = ValorNivelTensaoInv(); // V
	if (nv <= 0.)
		return;
	try
	{
		rtp = StrToInt(EditRTP->Text);
		if (rtp <= 0)
			return;
	}
	catch (Exception &e)
	{
		return;
	}
	// calcula banda morta
	switch (cboxUnidadeBMInv->ItemIndex)
	{
	case 1: // %
		bm_inf = (rtp * (nv * (1. - (ins / 100.))));
		bm_sup = (rtp * (nv * (1. + (ins / 100.))));
		break;
	case 0: // V
		// proteção
		if (ins > nv)
			return;
		bm_inf = (rtp * (nv - ins));
		bm_sup = (rtp * (nv + ins));
		break;
	default:
		break;
	}
	// monta texto
	texto = DoubleToStr("%4.1f", bm_inf) + " < V < " + DoubleToStr("%4.1f", bm_sup);
	EditBMinverso->Text = texto;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::CBoxFaixaPassosChange(TObject *Sender)
{
	CBoxFaixaTensao->ItemIndex = CBoxFaixaPassos->ItemIndex;
	AtualizaTrackBarPassofixo();
	CarregaPassos();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::CBoxFaixaTensaoChange(TObject *Sender)
{
	CBoxFaixaPassos->ItemIndex = CBoxFaixaTensao->ItemIndex;
	CarregaPassos();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::CBoxTipoPotCorrChange(TObject *Sender)
{
	// variáveis locais
	double valor = 0.;
	double novo = 0.;

	// conversões envolvendo corrente comentadas temporariamente
	try
	{ // lê valor em EditValor
		valor = StrToDouble(EditValor->Text);
		if (IsDoubleZero(valor))
		{
			Aviso("Insira um valor válido para potência ou corrente");
			CBoxTipoPotCorr->ItemIndex = opPotCor;
			return;
		}
		novo = valor;
		switch (CBoxTipoPotCorr->ItemIndex)
		{
		case 0: // NOMINAL
			if (opPotCor == 1)
			{ // passante para nominal
				novo = SpassanteToSnom(valor);
			}
			else if (opPotCor == 2)
			{ // corrente para pot nominal
				novo = InomToSnom_kva(valor);
			}
			break;
		case 1: // PASSANTE
			if (opPotCor == 0)
			{ // nominal para passante
				novo = SnomToSpassante(valor);
			}
			else if (opPotCor == 2)
			{ // corrente para pot passante
				novo = InomToSpassante_kva(valor);
			}
			break;
		case 2: // CORRENTE
			if (opPotCor == 0)
			{ // pot nominal para corrente
				// regulador->snom = valor *0.001; // kVA => MVA
				// novo = regulador->InomA();
				novo = SnomToInom(valor);
			}
			else if (opPotCor == 1)
			{ // passante para corrente
				// novo = SpassanteToSnom(valor); // pot nominal
				// valor = novo;
				// regulador->snom = valor *0.001; // kVA => MVA
				// novo = regulador->InomA();
				novo = SpassanteToInom(valor);
			}
			break;
		default:
			break;
		}
		// salva opção
		opPotCor = CBoxTipoPotCorr->ItemIndex;
		// reapresenta valor convertido
		EditValor->Text = DoubleToStr("%4.3f", novo);
	}
	catch (Exception &e)
	{
		// nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::DefValoresDefaultRegulador(void)
{
	// variáveis locais
	VTRede *rede;
	VTRegulador *ult_reg;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// verifica se o Regulador não está definido
	if ((regulador->snom == 0.) || (regulador->var_tensao == 0.) || (regulador->num_passo == 0))
	{ // define Snom c/ 300kVA
		regulador->snom = 0.300;
		// verifica se há uma Rede selecionada
		if (CBoxRede->ItemIndex >= 0)
		{ // determina Rede
			rede = (VTRede*)(CBoxRede->Items->Objects[CBoxRede->ItemIndex]);
			// define Snom em função do tipo da rede
			if (rede->TipoRede->Segmento != redeSEC)
			{
				regulador->snom = 0.030;
			}
		}
		regulador->ligacao = lgEST_ISO;
		regulador->fases = faseABC;
		regulador->by_pass = false;
		regulador->var_tensao = 10.;
		regulador->num_passo = 15;
		regulador->modelo = reguAUTO_BI;
		// sentido do fluxo
		regulador->sentido.bar_ref = regulador->pbarra1;
		// configuração automático
		regulador->cfg_auto.bar_ref = regulador->pbarra2;
		regulador->cfg_auto.bar_ref_inv = regulador->pbarra1;
		regulador->cfg_auto.v_pu = 1.0;
		regulador->cfg_auto.vinv_pu = 1.0;
		// configuração fixo
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
		regulador->modelo = ult_reg->modelo;
		// copia os valores de acordo com o tipo
		switch (regulador->modelo)
		{
		case reguAUTO_BI:
		case reguAUTO_UNI:
		case reguAUTO_FIXA:
			// configuração automático
			regulador->cfg_auto.v_pu = ult_reg->cfg_auto.v_pu;
			regulador->cfg_auto.vinv_pu = ult_reg->cfg_auto.vinv_pu;
			break;
		case reguFIXO:
			// configuração fixo
			regulador->cfg_fixo.passo = ult_reg->cfg_fixo.passo;
			break;
		default:
			break;
		}
		regulador->var_tensao = ult_reg->var_tensao;
		regulador->num_passo = ult_reg->num_passo;
		regulador->rel_tap = ult_reg->rel_tap;
	}
}

// ---------------------------------------------------------------------------
double __fastcall TFormEdtReguladorMT::DeterminaCapacidadeMax(void)
{
	// var local
	double faixa_reg = 10.;

	// determina valor
	faixa_reg = DeterminaFaixaTensao();
	capacidade_max.begin();
	return (capacidade_max.find(faixa_reg)->second);
}

// ---------------------------------------------------------------------------
int __fastcall TFormEdtReguladorMT::DeterminaFaixaPassos(void)
{
	// proteção
	if (CBoxFaixaPassos->ItemIndex < 0)
		return 10.;
	// determina valor
	faixa_passos_val.begin();
	return (faixa_passos_val.find(CBoxFaixaPassos->ItemIndex)->second);
}

// ---------------------------------------------------------------------------
double __fastcall TFormEdtReguladorMT::DeterminaFaixaTensao(void)
{
	// proteção
	if (CBoxFaixaTensao->ItemIndex < 0)
		return 10.;
	// determina valor
	faixa_tensao_val.begin();
	return (faixa_tensao_val.find(CBoxFaixaTensao->ItemIndex)->second);
}

// ---------------------------------------------------------------------------
int __fastcall TFormEdtReguladorMT::DeterminaFaixaPassosInd(int valor)
{
	// variáveis locais
	int indice = 4; // default
	// determina indice
	faixa_passos_ind.begin();
	indice = faixa_passos_ind.find(valor)->second;
	if (indice > (CBoxFaixaPassos->Items->Count - 1))
		indice = 4; // default
	return indice;
}

// ---------------------------------------------------------------------------
int __fastcall TFormEdtReguladorMT::DeterminaFaixaTensaoInd(double valor)
{
	// variáveis locais
	int indice = 4; // default
	// determina indice
	faixa_tensao_ind.begin();
	indice = faixa_tensao_ind.find(valor)->second;
	if (indice > (CBoxFaixaTensao->Items->Count - 1))
		indice = 4; // default
	return indice;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::EditRTPKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == '\r')
	{ // nivel de tensao direta (leitura)
		double nv = ValorNivelTensaoDir(); // V
		EditNTdir->Text = DoubleToStr("%3.2f", nv);
		// nivel de tensao inversa (leitura)
		nv = ValorNivelTensaoInv(); // V
		EditNTinv->Text = DoubleToStr("%3.2f", nv);
		// atualiza faixas de tensão direto e inverso
		cboxUnidadeBMChange(NULL);
		cboxUnidadeBMInvChange(NULL);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::EditVdirKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == '\r')
	{ // nivel de tensao direta (leitura)
		double nv = ValorNivelTensaoDir(); // V
		EditNTdir->Text = DoubleToStr("%3.2f", nv);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::EditVinvKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == '\r')
	{ // nivel de tensao inversa (leitura)
		double nv = ValorNivelTensaoInv(); // V
		EditNTinv->Text = DoubleToStr("%3.2f", nv);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::edtInsDiretoExit(TObject *Sender)
{
	cboxUnidadeBMChange(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::edtInsDiretoKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == '\r')
		cboxUnidadeBMChange(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::edtInsInversoExit(TObject *Sender)
{
	cboxUnidadeBMInvChange(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::edtInsInversoKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == '\r')
		cboxUnidadeBMInvChange(NULL);
}

/* ---------------------------------------------------------------------------
 índices de fases do regulador
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
void __fastcall TFormEdtReguladorMT::ExibeDadosRegulador(void)
{
	// variáveis locais
	double s_kva, nv;
	double insensibilidade_perc;

	// exibe dados basicos do regulador
	EditCodigo->Text = regulador->Codigo;
	// exibe potência nominal
	s_kva = regulador->snom * 1000.; // MVA =>kVA;
	EditValor->Text = DoubleToStr("%4.3f", s_kva);
	// by-pass
	checkBypass->Checked = regulador->by_pass;
	// relação de tap
	EditRTP->Text = IntToStr(regulador->rel_tap);
	// exibe modelo
	switch (regulador->modelo)
	{
	case reguPADRAO:
		ComboBoxTipoAjuste->ItemIndex = 4; // HPS, 2018.04.05
		gboxBMdir->Visible = true;
		gboxBMinv->Visible = false;
		break;
	case reguAUTO_BI:
		ComboBoxTipoAjuste->ItemIndex = 1;
		gboxBMdir->Visible = true;
		gboxBMinv->Visible = true;
		break;
	case reguAUTO_FIXA:
		ComboBoxTipoAjuste->ItemIndex = 2;
		gboxBMdir->Visible = true;
		gboxBMinv->Visible = false;
		break;
	case reguAUTO_UNI:
		ComboBoxTipoAjuste->ItemIndex = 3;
		gboxBMdir->Visible = true;
		gboxBMinv->Visible = false;
		break;
	case reguFIXO:
		ComboBoxTipoAjuste->ItemIndex = 0;
		gboxBMdir->Visible = false;
		gboxBMinv->Visible = false;
		break;
	case reguENERQCT:
	default:
		ComboBoxTipoAjuste->ItemIndex = -1;
		gboxBMdir->Visible = false;
		gboxBMinv->Visible = false;
		break;
	}
	// força atualização em função do modelo
	ActionSelTipoAjuste->Execute();
	// exibe configuração (unidades/fases)
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
	// exibe faixa de tensão e faixa de passos
	// UpDownFaixaTensao->Position = int(regulador->var_tensao);
	// UpDownFaixaPasso->Position = regulador->num_passo;
	CBoxFaixaTensao->ItemIndex = DeterminaFaixaTensaoInd(regulador->var_tensao);
	CBoxFaixaPassos->ItemIndex = DeterminaFaixaPassosInd(regulador->num_passo);
	// exibe dados de configuração automático
	EditVdir->Text = DoubleToStr("%5.4f", regulador->cfg_auto.v_pu);
	EditVinv->Text = DoubleToStr("%5.4f", regulador->cfg_auto.vinv_pu);
	// exibe dados de configuração fixo
	passoTemp = regulador->cfg_fixo.passo;
	TrackBarPassoFixo->Position = regulador->cfg_fixo.passo;
	// simula click em UpDownFaixaPasso p/ atualizar EditFaixaPasso
	// UpDownFaixaPassoClick(UpDownFaixaPasso, NULL);
	// simula click em UpDownFaixaTensao p/ atualizar EditFaixaTensao
	// UpDownFaixaTensaoClick(UpDownFaixaTensao, NULL);
	// preenche o valor de bandamorta
	if (pnlBMDireto->Visible)
	{
		insensibilidade_perc = ConverteInsensibilidadeVtoPerc
			(regulador->cfg_auto.larg_bandamorta_pu / 2., ValorNivelTensaoDir());
		// insensibilidade_perc = ConverteInsensibilidadeVtoPerc(regulador->cfg_auto.larg_bandamorta_pu / 2.,ReguladorVnom_kv()*1000);
		edtInsDireto->Text = DoubleToStr("%4.3f", insensibilidade_perc);
		cboxUnidadeBM->ItemIndex = 1; // %
		cboxUnidadeBMChange(NULL);
	}
	if (pnlBMInverso->Visible)
	{
		insensibilidade_perc = ConverteInsensibilidadeVtoPerc
			(regulador->cfg_auto.larg_bandamorta_inv_pu / 2., ValorNivelTensaoInv());
		// insensibilidade_perc = ConverteInsensibilidadeVtoPerc(regulador->cfg_auto.larg_bandamorta_inv_pu / 2.,ReguladorVnom_kv()*1000);
		edtInsInverso->Text = DoubleToStr("%4.3f", insensibilidade_perc);
		cboxUnidadeBMInv->ItemIndex = 1; // %
		cboxUnidadeBMInvChange(NULL);
	}
	// nivel de tensao (leitura)
	nv = ValorNivelTensaoDir(); // V
	EditNTdir->Text = DoubleToStr("%3.2f", nv);
	nv = ValorNivelTensaoInv(); // V
	EditNTinv->Text = DoubleToStr("%3.2f", nv);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::FormClose(TObject *Sender, TCloseAction &Action)
{
	// esconde o Form
	Action = caHide;
	// ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
	// posiciona o Form sem alinhamento
	Left = 0;
	Top = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::FormShow(TObject *Sender)
{
	// exibe dados do Regulador
	ExibeDadosRegulador();
	// posiciona o Cursor em EditCodigo
	PageControl->ActivePage = psheetDados;
	EditCodigo->SetFocus();
	// FKM - 2015.10.07 Ajusta o tamanho do form \\
	this->AutoSize = true;
	this->AutoSize = false;
	// FKM - 2015.10.07 Ajusta o tamanho do form //
	// caso especial: alocação de regulador (tipo de operação não enabled)
	if (!ComboBoxTipoAjuste->Enabled)
	{
		gboxTapFixo->Visible = false;
		psheetReferencia->TabVisible = false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::IniciaCBoxBarEntrada(void)
{
	// variáveis locais
	int index;

	// reinicia CBoxBarEntrada
	CBoxBarEntrada->Clear();
	// insere as duas Barras do Regulador
	CBoxBarEntrada->Items->AddObject(regulador->pbarra1->Codigo, regulador->pbarra1);
	CBoxBarEntrada->Items->AddObject(regulador->pbarra2->Codigo, regulador->pbarra2);
	// verifica se a Barra que indica sentido do fluxo está definida
	if (regulador->sentido.bar_ref == NULL)
		index = 0;
	else
	{
		// index = CBoxBarEntrada->Items->IndexOf(regulador->sentido.bar_ref->Codigo);
		index = CBoxBarEntrada->Items->IndexOfObject(regulador->sentido.bar_ref);
	}
	CBoxBarEntrada->ItemIndex = index;
	// força atualização do label
	// CBoxBarEntradaChange(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::IniciaCBoxBarSaida(void)
{
	// reinicia CBoxBarSaida
	CBoxBarSaida->Clear();
	// insere as duas Barras do Regulador
	CBoxBarSaida->Items->AddObject(regulador->pbarra1->Codigo, regulador->pbarra1);
	CBoxBarSaida->Items->AddObject(regulador->pbarra2->Codigo, regulador->pbarra2);
	// seleciona a Barra diferente da Barra de entrada
	CBoxBarSaida->ItemIndex = 1 - CBoxBarEntrada->ItemIndex;
	// força atualização do label
	// CBoxBarSaidaChange(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::IniciaCBoxBarControle(void)
{
	// vari�veis locais
	VTBarra *pbarra;
	VTRede *rede;
	TList *lisBAR;

	// reinicia CBoxBar
	CBoxBarraDireto->Clear();
	CBoxBarraInverso->Clear();
	// verifica se há uma Rede selecionada
	if (CBoxRede->ItemIndex < 0)
		return;
	// determina Rede selecionada
	rede = (VTRede*)CBoxRede->Items->Objects[CBoxRede->ItemIndex];
	// loop p/ todas Barras da Rede selecionada
	lisBAR = rede->LisBarra();
	for (int n = 0; n < lisBAR->Count; n++)
	{
		pbarra = (VTBarra*)lisBAR->Items[n];
		CBoxBarraDireto->Items->AddObject(pbarra->Codigo, pbarra);
		CBoxBarraInverso->Items->AddObject(pbarra->Codigo, pbarra);
	}
	// inclui Barras do Regulador (que podem não estar incluidas na lista de Barras da Rede)
	for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
	{
		if ((pbarra = regulador->Barra(ind_bar)) == NULL)
			continue;
		// verifica se a Barra não existe em lisBAR
		if (lisBAR->IndexOf(pbarra) < 0)
		{
			CBoxBarraDireto->Items->AddObject(pbarra->Codigo, pbarra);
			CBoxBarraInverso->Items->AddObject(pbarra->Codigo, pbarra);
		}
	}
	// habilita ordem alfabetica
	CBoxBarraDireto->Sorted = true;
	CBoxBarraInverso->Sorted = true;
	// barra de referencia default: barra oposta à barra de entrada
	pbarra = (regulador->sentido.bar_ref == regulador->pbarra1) ? regulador->pbarra2 :
		regulador->pbarra1;
	// verifica se há Barras no CBox
	if (CBoxBarraDireto->Items->Count > 0)
	{ // pré-seleciona Barra controlada direta
		if (regulador->cfg_auto.bar_ref)
		{
			// CBoxBarraDireto->ItemIndex =
			// CBoxBarraDireto->Items->IndexOf(regulador->cfg_auto.bar_ref->Codigo);
			CBoxBarraDireto->ItemIndex =
				CBoxBarraDireto->Items->IndexOfObject(regulador->cfg_auto.bar_ref);
		}
		else
		{
			CBoxBarraDireto->ItemIndex = CBoxBarraDireto->Items->IndexOfObject(pbarra);
			// CBoxBarraDireto->ItemIndex = CBoxBarraDireto->Items->IndexOf(pbarra->Codigo);
		}
	}
	if (CBoxBarraInverso->Items->Count > 0)
	{ // pré-seleciona Barra controlada inversa
		if (regulador->cfg_auto.bar_ref_inv)
		{
			// CBoxBarraInverso->ItemIndex =
			// CBoxBarraInverso->Items->IndexOf(regulador->cfg_auto.bar_ref_inv->Codigo);
			CBoxBarraInverso->ItemIndex =
				CBoxBarraInverso->Items->IndexOfObject(regulador->cfg_auto.bar_ref_inv);
		}
		else
		{
			CBoxBarraInverso->ItemIndex = CBoxBarraInverso->Items->IndexOfObject(pbarra);
			// CBoxBarraInverso->ItemIndex = CBoxBarraInverso->Items->IndexOf(pbarra->Codigo);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::IniciaCBoxRede(void)
{
	// variáveis locais
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
	// define ordem alfabética
	CBoxRede->Sorted = true;
	// pré-seleciona Rede que conecta as Barras do Regulador
	index = 0;
	if ((rede = SelecionaRede(lisREDE)) != NULL)
		index = CBoxRede->Items->IndexOfObject(rede);
	CBoxRede->ItemIndex = index;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::IniciaCBoxTipoPotCorr(void)
{
	CBoxTipoPotCorr->Clear();
	CBoxTipoPotCorr->Items->Add("Potência Nominal (unid. monofásica) [kVA]");
	CBoxTipoPotCorr->Items->Add("Potência Passante [kVA]");
	CBoxTipoPotCorr->Items->Add("Corrente Nominal [A]");
	CBoxTipoPotCorr->ItemIndex = opPotCor = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::IniciaMaps(void)
{
	faixa_passos_val.insert(pair<int, int>(0, 8));
	faixa_passos_val.insert(pair<int, int>(1, 10));
	faixa_passos_val.insert(pair<int, int>(2, 12));
	faixa_passos_val.insert(pair<int, int>(3, 14));
	faixa_passos_val.insert(pair<int, int>(4, 16));

	faixa_passos_ind.insert(pair<int, int>(8, 0));
	faixa_passos_ind.insert(pair<int, int>(10, 1));
	faixa_passos_ind.insert(pair<int, int>(12, 2));
	faixa_passos_ind.insert(pair<int, int>(14, 3));
	faixa_passos_ind.insert(pair<int, int>(16, 4));

	faixa_tensao_val.insert(pair<int, double>(0, 5.00));
	faixa_tensao_val.insert(pair<int, double>(1, 6.25));
	faixa_tensao_val.insert(pair<int, double>(2, 7.50));
	faixa_tensao_val.insert(pair<int, double>(3, 8.75));
	faixa_tensao_val.insert(pair<int, double>(4, 10.0));

	faixa_tensao_ind.insert(pair<double, int>(5.00, 0));
	faixa_tensao_ind.insert(pair<double, int>(6.25, 1));
	faixa_tensao_ind.insert(pair<double, int>(7.50, 2));
	faixa_tensao_ind.insert(pair<double, int>(8.75, 3));
	faixa_tensao_ind.insert(pair<double, int>(10.0, 4));

	capacidade_max.insert(pair<double, double>(5.00, 1.60));
	capacidade_max.insert(pair<double, double>(6.25, 1.35));
	capacidade_max.insert(pair<double, double>(7.50, 1.20));
	capacidade_max.insert(pair<double, double>(8.75, 1.10));
	capacidade_max.insert(pair<double, double>(10.0, 1.00));
}

// ---------------------------------------------------------------------------
double __fastcall TFormEdtReguladorMT::InomToSnom_kva(double inom_a)
{
	double vnom_kv, cap_max;
	double snom_kva = 0;

	vnom_kv = ReguladorVnom_kv();
	cap_max = DeterminaCapacidadeMax();
	//snom_kva = ((inom_a * vnom_kv) * 0.1) * cap_max;
	snom_kva = ((inom_a * vnom_kv) * 0.1) / cap_max;

	return snom_kva;
}

// ---------------------------------------------------------------------------
double __fastcall TFormEdtReguladorMT::InomToSpassante_kva(double inom_a)
{
	double vnom_kv;
	double spass_kva = 0;

	vnom_kv = ReguladorVnom_kv();
	spass_kva = inom_a * vnom_kv;

	return spass_kva;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtReguladorMT::LeDados(void)
{
	// variáveis locais
	double s_kva;

	// determina Rede do Regulador
	regulador->rede = (VTRede*)CBoxRede->Items->Objects[CBoxRede->ItemIndex];
	// lê dados basicos
	regulador->Codigo = EditCodigo->Text;
	regulador->by_pass = checkBypass->Checked;
	// regulador->var_tensao = UpDownFaixaTensao->Position;
	// regulador->num_passo = UpDownFaixaPasso->Position;
	regulador->var_tensao = DeterminaFaixaTensao();
	regulador->num_passo = DeterminaFaixaPassos();
	// lê potência editada em MVA
	regulador->snom = ValorSnom_mva();
	// lê relação de tap
	regulador->rel_tap = StrToDouble(EditRTP->Text);
	// sentido do fluxo
	regulador->sentido.bar_ref =
		(VTBarra*)(CBoxBarEntrada->Items->Objects[CBoxBarEntrada->ItemIndex]);
	// define tipo de ligação e fases
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
	// define opções gráficas
	regulador->Show[eqp_FIXO] = true;
	regulador->Show[lig_CARGTO] = true;
	regulador->Show[lig_S] = true;

	// informações especificas do modelo
	switch (ComboBoxTipoAjuste->ItemIndex)
	{
	case 0:
		regulador->modelo = reguFIXO;
		// define barra de referência direta e inversa como sendo a mesma barra direta
		regulador->cfg_auto.bar_ref =
			(VTBarra*)CBoxBarraDireto->Items->Objects[CBoxBarraDireto->ItemIndex];
		regulador->cfg_auto.bar_ref_inv =
			(VTBarra*)CBoxBarraDireto->Items->Objects[CBoxBarraDireto->ItemIndex];
		// define demais dados
		regulador->cfg_auto.v_pu = 1.; // StrToDouble(EditVdir->Text);
		regulador->cfg_auto.vinv_pu = 1.; // StrToDouble(EditVinv->Text);
		regulador->cfg_fixo.passo = TrackBarPassoFixo->Position;
		regulador->cfg_auto.larg_bandamorta_pu = 0;
		regulador->cfg_auto.larg_bandamorta_inv_pu = 0;
		break;
	case 1:
		regulador->modelo = reguAUTO_BI;
		// define barra de referência direta
		regulador->cfg_auto.bar_ref =
			(VTBarra*)CBoxBarraDireto->Items->Objects[CBoxBarraDireto->ItemIndex];
		// define barra de referência inversa
		regulador->cfg_auto.bar_ref_inv =
			(VTBarra*)CBoxBarraInverso->Items->Objects[CBoxBarraInverso->ItemIndex];
		// define demais dados
		regulador->cfg_auto.v_pu = StrToDouble(EditVdir->Text);
		regulador->cfg_auto.vinv_pu = StrToDouble(EditVinv->Text);
		regulador->cfg_fixo.passo = 0;
		regulador->cfg_auto.larg_bandamorta_pu = ValorBandaMorta_pu();
		regulador->cfg_auto.larg_bandamorta_inv_pu = ValorBandaMortaInv_pu();
		break;
	case 2:
		regulador->modelo = reguAUTO_FIXA;
		// define barra de referência direta e inversa como sendo a mesma barra direta
		regulador->cfg_auto.bar_ref =
			(VTBarra*)CBoxBarraDireto->Items->Objects[CBoxBarraDireto->ItemIndex];
		regulador->cfg_auto.bar_ref_inv =
			(VTBarra*)CBoxBarraDireto->Items->Objects[CBoxBarraDireto->ItemIndex];
		// define demais dados
		regulador->cfg_auto.v_pu = StrToDouble(EditVdir->Text);
		regulador->cfg_auto.vinv_pu = 1.; // StrToDouble(EditVinv->Text);
		regulador->cfg_fixo.passo = 0;
		regulador->cfg_auto.larg_bandamorta_pu = ValorBandaMorta_pu();
		regulador->cfg_auto.larg_bandamorta_inv_pu = 0;
		break;
	case 4: // HPS, 2018.04.05
		regulador->modelo = reguPADRAO;
		// define barra de referência direta e inversa como sendo a mesma barra direta
		regulador->cfg_auto.bar_ref =
			(VTBarra*)CBoxBarraDireto->Items->Objects[CBoxBarraDireto->ItemIndex];
		regulador->cfg_auto.bar_ref_inv =
			(VTBarra*)CBoxBarraDireto->Items->Objects[CBoxBarraDireto->ItemIndex];
		// define demais dados
		regulador->cfg_auto.v_pu = StrToDouble(EditVdir->Text);
		regulador->cfg_auto.vinv_pu = 1.; // StrToDouble(EditVinv->Text);
		regulador->cfg_fixo.passo = 0;
		regulador->cfg_auto.larg_bandamorta_pu = ValorBandaMorta_pu();
		regulador->cfg_auto.larg_bandamorta_inv_pu = 0;
		break;
	case 3:
	default:
		regulador->modelo = reguAUTO_UNI;
		// define barra de referência direta e inversa como sendo a mesma barra direta
		regulador->cfg_auto.bar_ref =
			(VTBarra*)CBoxBarraDireto->Items->Objects[CBoxBarraDireto->ItemIndex];
		regulador->cfg_auto.bar_ref_inv =
			(VTBarra*)CBoxBarraDireto->Items->Objects[CBoxBarraDireto->ItemIndex];
		// define demais dados
		regulador->cfg_auto.v_pu = StrToDouble(EditVdir->Text);
		regulador->cfg_auto.vinv_pu = 1.; // StrToDouble(EditVinv->Text);
		regulador->cfg_fixo.passo = 0;
		regulador->cfg_auto.larg_bandamorta_pu = ValorBandaMorta_pu();
		regulador->cfg_auto.larg_bandamorta_inv_pu = 0;
		break;
	}

	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::Moldura(VTEqpto *eqpto)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	grafico->Moldura(eqpto);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::OpcaoRetira(bool enabled)
{
	ActionRetira->Visible = enabled;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtReguladorMT::RadioGroupBarraRefClick(TObject *Sender)
{
	switch (RadioGroupBarraRef->ItemIndex)
	{
	case 0: // barras do regulador
		ActionBarrasDefault->Execute();
		break;
	default: // outras barras
		break;
	}
}

// ---------------------------------------------------------------------------
VTRede* __fastcall TFormEdtReguladorMT::SelecionaRede(TList *lisREDE)
{
	// variáveis locais
	VTRede *rede;

	// verifica se o Trafo já possui uma Rede definida
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
double __fastcall TFormEdtReguladorMT::SnomToInom(double snom_kva)
{
	double vnom_kv, cap_max;
	double inom_a = 0;

	vnom_kv = ReguladorVnom_kv();
	cap_max = DeterminaCapacidadeMax();
	if ((!IsDoubleZero(vnom_kv)) && (!IsDoubleZero(cap_max)))
	{
		//inom_a = snom_kva / ((vnom_kv * 0.1) * cap_max);
		inom_a = (snom_kva * cap_max) / (vnom_kv * 0.1);
	}

	return inom_a;
}

// ---------------------------------------------------------------------------
double __fastcall TFormEdtReguladorMT::SnomToSpassante(double Snom)
{
	// variáveis locais
	double spas;
	double inom_a;

	// calcula inom e converte para spassante
	inom_a = SnomToInom(Snom);
	spas = InomToSpassante_kva(inom_a);
	return (spas);
}

// ---------------------------------------------------------------------------
double __fastcall TFormEdtReguladorMT::SpassanteToSnom(double Spas_kva)
{
	// variáveis locais
	double snom;
	double inom_a;

	// calcula inom e converte para snom
	inom_a = SpassanteToInom(Spas_kva);
	snom = InomToSnom_kva(inom_a);
	return (snom);
}

// ---------------------------------------------------------------------------
double __fastcall TFormEdtReguladorMT::SpassanteToInom(double Spas_kva)
{
	double vnom_kv;
	double inom_a = 0;

	vnom_kv = ReguladorVnom_kv();
	if (!IsDoubleZero(vnom_kv))
	{
		inom_a = Spas_kva / vnom_kv;
	}
	return inom_a;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtReguladorMT::ValidaDados(void)
{
	// variaveis locais
	double valor;

	try
	{ // proteção
		if (CBoxRede->ItemIndex < 0)
		{
			Aviso("Defina a rede do regulador de tensão");
			PageControl->ActivePage = psheetDados;
			CBoxRede->DroppedDown = true; // FKM 2015.10.01
			return (false);
		}
		if (CBoxTipoPotCorr->ItemIndex < 0)
		{
			Aviso("Defina a opção Potência Nominal, Potência Passante ou Corrente Nominal");
			PageControl->ActivePage = psheetDados;
			CBoxTipoPotCorr->DroppedDown = true; // FKM 2015.10.01
			return (false);
		}
		if (CBoxConfiguracao->ItemIndex < 0)
		{
			Aviso("Selecione uma configuração para as unidades monofásicas.");
			PageControl->ActivePage = psheetDados;
			CBoxConfiguracao->DroppedDown = true; // FKM 2015.10.01
			return (false);
		}
		if (CBoxBarEntrada->ItemIndex < 0)
		{
			Aviso("Defina a barra a montante");
			PageControl->ActivePage = psheetDados;
			CBoxBarEntrada->DroppedDown = true; // FKM 2015.10.01
			return (false);
		}
		if (CBoxBarSaida->ItemIndex < 0)
		{
			Aviso("Defina a barra a jusante");
			PageControl->ActivePage = psheetDados;
			CBoxBarSaida->DroppedDown = true; // FKM 2015.10.01
			return (false);
		}
		if (ComboBoxTipoAjuste->ItemIndex < 0)
		{
			Aviso("Defina o tipo de operação do regulador de tensão");
			return (false);
		}
		if (EditValor->Text.IsEmpty())
		{
			Aviso("Digite um valor para potência ou corrente");
			return (false);
		}
		if (!StrToDouble(EditValor->Text))
		{
			Aviso("Valor de potência ou corrente inválido.");
			return (false);
		}
		try
		{
			int rtp = StrToInt(EditRTP->Text);
			if (rtp <= 0)
			{
				Aviso("Valor de relação de tap inválido.");
				return (false);
			}
		}
		catch (Exception &e)
		{
			Aviso("Valor de relação de tap inválido.");
			return (false);
		}
		// verifica ajuste para cada tipo de operação
		switch (ComboBoxTipoAjuste->ItemIndex)
		{
		case 0: // tap fixo
			if (EditFixoPasso->Text.IsEmpty())
			{
				Aviso("Selecione o passo para o ajuste.");
				return (false);
			}
			if (abs(TrackBarPassoFixo->Position) > DeterminaFaixaPassos())
			{
				Aviso("Passo selecionado para comutador inválido.");
				return (false);
			}
			break;

		case 1: // bidirecional
			// valida tensão de referência
			if (EditVdir->Text.IsEmpty())
			{
				Aviso("Digite um valor para tensão de referência sentido direto.");
				return (false);
			}
			if (EditVinv->Text.IsEmpty())
			{
				Aviso("Digite um valor para tensão de referência  sentido inverso.");
				return (false);
			}
			valor = StrToDouble(EditVdir->Text);
			if ((valor <= 0.5) || (valor >= 1.5))
			{
				Aviso("Tensão da barra controlada sentido direto inválido.");
				return (false);
			}
			valor = StrToDouble(EditVinv->Text);
			if ((valor <= 0.5) || (valor >= 1.5))
			{
				Aviso("Tensão da barra controlada sentido direto inválido.");
				return (false);
			}
			if (CBoxBarraDireto->ItemIndex < 0)
			{
				Aviso("Defina a barra de tensão controlada sentido direto");
				PageControl->ActivePage = psheetReferencia;
				CBoxBarraDireto->DroppedDown = true; // FKM 2015.10.01
				return (false);
			}
			if (CBoxBarraInverso->ItemIndex < 0)
			{
				Aviso("Defina a barra de tensão controlada sentido inverso");
				PageControl->ActivePage = psheetReferencia;
				CBoxBarraInverso->DroppedDown = true; // FKM 2015.10.01
				return (false);
			}
			if (CBoxBarraInverso->ItemIndex == CBoxBarraDireto->ItemIndex)
			{
				Aviso("Defina a barra de tensão controlada sentido inverso diferente da barra no sentido direto"
					);
				PageControl->ActivePage = psheetReferencia;
				CBoxBarraInverso->DroppedDown = true; // FKM 2015.10.01
				return (false);
			}
			// verifica os dados da banda morta
			try
			{
				StrToDouble(edtInsDireto->Text, valor);
			}
			catch (Exception &e)
			{
				valor = -1;
			}
			if (valor < 0)
			{
				Aviso("Insensibilidade sentido direto inválida.");
				return (false);
			}
			// verifica os dados da banda morta inversa
			try
			{
				StrToDouble(edtInsInverso->Text, valor);
			}
			catch (Exception &e)
			{
				valor = -1;
			}
			if (valor < 0)
			{
				Aviso("Insensibilidade sentido inverso inválida.");
				return (false);
			}
			break;

		default: // demais casos automatico
			// valida tensão de referência
			if (EditVdir->Text.IsEmpty())
			{
				Aviso("Digite um valor para tensão de referência");
				return (false);
			}
			valor = StrToDouble(EditVdir->Text);
			if ((valor <= 0.5) || (valor >= 1.5))
			{
				Aviso("Tensão de referência inválida");
				return (false);
			}
			if (CBoxBarraDireto->ItemIndex < 0)
			{
				Aviso("Defina a barra de tensão controlada");
				PageControl->ActivePage = psheetAjustes;
				CBoxBarraDireto->DroppedDown = true; // FKM 2015.10.01
				return (false);
			}
			// verifica os dados da banda morta
			try
			{
				StrToDouble(edtInsDireto->Text, valor);
			}
			catch (Exception &e)
			{
				valor = -1;
			}
			if (valor < 0)
			{
				Aviso("Insensibilidade sentido direto inválida.");
				return (false);
			}
			break;
		}

		// verifica compatibilidade de fases do regulador com a rede
		if (regulador->pbarra1->zona != NULL)
		{ // DVK 2016.09.08 verifica se a zona é indefinida
			if (regulador->pbarra1->zona->Tipo == zonaINDEFINIDA)
				return (true);
			// if ((regulador->fases & faseABC) != (regulador->pbarra1->zona->Fases & faseABC))
			if ((regulador->fases & regulador->pbarra1->zona->Fases) != regulador->fases)
			{
				Aviso("Número de fases do regulador incompatível com as fases da rede.");
				return (false);
			}
			if ((regulador->fases > 7) && ((regulador->fases & faseN) !=
				(regulador->pbarra1->zona->Fases & faseN)))
			{
				Aviso("Não existe neutro na rede ou no regulador.");
				return (false);
			}
		}
	}
	catch (Exception &e)
	{
		Aviso("Texto inválido: " + e.Message);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
double __fastcall TFormEdtReguladorMT::ValorBandaMorta_pu(void)
{
	double ins = 0;

	try
	{
		switch (cboxUnidadeBM->ItemIndex)
		{
		case 1: // %
			// converte para volts
			ins = (StrToDouble(edtInsDireto->Text) / 100.) * ValorNivelTensaoDir();
			// ins = (StrToDouble(edtInsDireto->Text) / 100.) * ReguladorVnom_kv();
			break;
		case 0: // volts
			ins = StrToDouble(edtInsDireto->Text);
			break;
		default:
			break;
		}
	}
	catch (Exception &e)
	{
		return 0.;
	}
	return (ins * 2.);
}

// ---------------------------------------------------------------------------
double __fastcall TFormEdtReguladorMT::ValorBandaMortaInv_pu(void)
{
	double ins = 0;

	try
	{
		switch (cboxUnidadeBMInv->ItemIndex)
		{
		case 1: // %
			ins = (StrToDouble(edtInsInverso->Text) / 100.) * ValorNivelTensaoInv();
			// ins = (StrToDouble(edtInsInverso->Text) / 100.) * ReguladorVnom_kv();
			break;
		case 0: // volts
			ins = StrToDouble(edtInsInverso->Text);
			break;
		default:
			break;
		}
	}
	catch (Exception &e)
	{
		return 0.;
	}

	return (ins * 2.);
}

// ---------------------------------------------------------------------------
double __fastcall TFormEdtReguladorMT::ValorNivelTensaoDir(void)
{
	// variáveis locais
	double rtp, v_ref;

	try
	{ // lê valor em EditRTP
		rtp = StrToDouble(EditRTP->Text);
		if (IsDoubleZero(rtp))
			return 0.;
		// determina tensão de referência do regulador
		// v_ref = StrToDouble(EditVdir->Text) * regulador->pbarra1->vnom * 1000.; // kV => V
		v_ref = StrToDouble(EditVdir->Text) * ReguladorVnom_kv() * 1000.; // kV => V
		// calcula nivel de tensão
		return (v_ref / rtp);
	}
	catch (Exception &e)
	{
		return 0.;
	}
}

// ---------------------------------------------------------------------------
double __fastcall TFormEdtReguladorMT::ValorNivelTensaoInv(void)
{
	// variáveis locais
	double rtp, v_ref;

	try
	{ // lê valor em EditRTP
		rtp = StrToDouble(EditRTP->Text);
		if (IsDoubleZero(rtp))
			return 0.;
		// determina tensão de referência do regulador
		// v_ref = StrToDouble(EditVinv->Text) * regulador->pbarra1->vnom * 1000.; // kV => V
		v_ref = StrToDouble(EditVinv->Text) * ReguladorVnom_kv() * 1000.; // kV => V
		// calcula nivel de tensão
		return (v_ref / rtp);
	}
	catch (Exception &e)
	{
		return 0.;
	}
}

// ---------------------------------------------------------------------------
double __fastcall TFormEdtReguladorMT::ValorSnom_mva(void)
{
	// variáveis locais
	double dv_perc;
	double valor = 0.;
	double s_mva = 0.;

	try
	{ // lê valor em EditValor
		valor = StrToDouble(EditValor->Text);
		switch (CBoxTipoPotCorr->ItemIndex)
		{
		case 0: // NOMINAL
			s_mva = valor * 0.001; // kVA => MVA
			break;
		case 1: // PASSANTE
			s_mva = SpassanteToSnom(valor) * 0.001; // kVA => MVA
			break;
		case 2: // CORRENTE
			// dv_perc = regulador->pbarra1->vnom; // kV
			// s_mva = valor * dv_perc * 0.1 / 1000.; // A * kV => MVA
			s_mva = InomToSnom_kva(valor) * 0.001; // kVA => MVA
			break;
		default:
			break;
		}
		return s_mva;
	}
	catch (Exception &e)
	{
		return 0.;
	}
}

// ---------------------------------------------------------------------------
double __fastcall TFormEdtReguladorMT::ReguladorVnom_kv(void)
{
	double vnom_kv = 0;

	switch (CBoxConfiguracao->ItemIndex)
	{
	case 0: // 1 unidade, faseAB
	case 1: // 1 unidade, faseBC
	case 2: // 1 unidade, faseCA
		if (regulador->pbarra1->zona != NULL)
		{
			vnom_kv = regulador->pbarra1->zona->Vff_kv;
		}
		else
		{
			vnom_kv = regulador->pbarra1->vnom;
		}
		break;
	case 3: // 1 unidade, faseAN
		if (regulador->pbarra1->zona != NULL)
		{
			vnom_kv = regulador->pbarra1->zona->Vfn_kv[indFASE_A];
		}
		else
		{
			vnom_kv = regulador->pbarra1->vnom * 0.57735026919;
		} // div por raiz(3)
		break;
	case 4: // 1 unidade, faseBN
		if (regulador->pbarra1->zona != NULL)
		{
			vnom_kv = regulador->pbarra1->zona->Vfn_kv[indFASE_B];
		}
		else
		{
			vnom_kv = regulador->pbarra1->vnom * 0.57735026919;
		} // div por raiz(3)
		break;
	case 5: // 1 unidade, faseCN
		if (regulador->pbarra1->zona != NULL)
		{
			vnom_kv = regulador->pbarra1->zona->Vfn_kv[indFASE_C];
		}
		else
		{
			vnom_kv = regulador->pbarra1->vnom * 0.57735026919;
		} // div por raiz(3)
		break;
	case 6: // 1 unidade, faseABN
	case 7: // 1 unidade, faseBCN
	case 8: // 1 unidade, faseCAN
		// ai eu nao sei o vnom (conferir)
		if (regulador->pbarra1->zona != NULL)
		{
			vnom_kv = regulador->pbarra1->zona->Vff_kv;
		}
		else
		{
			vnom_kv = regulador->pbarra1->vnom;
		}
		break;
	case 9: // 2 unidades, delta aberto, faseA como fase comum
	case 10: // 2 unidades, delta aberto, faseB como fase comum
	case 11: // 2 unidades, delta aberto, faseC como fase comum
	case 12: // 3 unidades, delta fechado
		if (regulador->pbarra1->zona != NULL)
		{
			vnom_kv = regulador->pbarra1->zona->Vff_kv;
		}
		else
		{
			vnom_kv = regulador->pbarra1->vnom;
		}
		break;
	case 13: // 3 unidades, estrela
	default: // 3 unidades, estrela
		vnom_kv = regulador->pbarra1->vnom * 0.57735026919; // div por raiz(3)
		break;
	}

	return vnom_kv;
}

/*
 // ---------------------------------------------------------------------------
 void __fastcall TFormEdtReguladorMT::UpDownFaixaPassoClick(TObject *Sender, TUDBtnType Button)
 {
 // variáveis locais
 double ganho;
 AnsiString txt;


 passoMax = SendMessage(UpDownFaixaPasso->Handle, (UINT)UDM_GETPOS32, 0, 0);
 // atualiza EditFaixaPasso
 //	txt.sprintf("-%02d/%02d", UpDownFaixaPasso->Position, UpDownFaixaPasso->Position);
 txt.sprintf("-%02d/%02d", passoMax, passoMax);
 EditFaixaPasso->Text = txt;
 // atualiza TrackBarPassoFixo
 //	TrackBarPassoFixo->Max = UpDownFaixaPasso->Position;
 //	TrackBarPassoFixo->Min = -UpDownFaixaPasso->Position;
 TrackBarPassoFixo->Max = passoMax;
 TrackBarPassoFixo->Min = -passoMax;
 if (TrackBarPassoFixo->Position > TrackBarPassoFixo->Max)
 TrackBarPassoFixo->Position = TrackBarPassoFixo->Max;
 if (TrackBarPassoFixo->Position < TrackBarPassoFixo->Min)
 TrackBarPassoFixo->Position = TrackBarPassoFixo->Min;
 // atualiza EditFixoPasso e EditFixoTensao
 ActionSelPassoFixoExecute(NULL);
 }
 */
/*
 // ---------------------------------------------------------------------------
 void __fastcall TFormEdtReguladorMT::UpDownFaixaPassoClick(TObject *Sender, TUDBtnType Button)
 {
 // variáveis locais
 double ganho;
 AnsiString txt;
 int pos = 0;

 // atualiza EditFaixaPasso
 pos = SendMessage(UpDownFaixaPasso->Handle, (UINT)UDM_GETPOS32, 0, 0);
 txt.sprintf("-%02d/%02d", pos, pos);
 EditFaixaPasso->Text = txt;
 // atualiza TrackBarPassoFixo
 TrackBarPassoFixo->Max = UpDownFaixaPasso->Position;
 TrackBarPassoFixo->Min = -UpDownFaixaPasso->Position;
 if (TrackBarPassoFixo->Position > TrackBarPassoFixo->Max)
 TrackBarPassoFixo->Position = TrackBarPassoFixo->Max;
 if (TrackBarPassoFixo->Position < TrackBarPassoFixo->Min)
 TrackBarPassoFixo->Position = TrackBarPassoFixo->Min;
 // atualiza EditFixoPasso e EditFixoTensao
 ActionSelPassoFixoExecute(NULL);
 }
 */
/*
 // ---------------------------------------------------------------------------
 void __fastcall TFormEdtReguladorMT::UpDownFaixaTensaoClick(TObject *Sender, TUDBtnType Button)
 {
 // variáveis locais
 AnsiString txt;
 double dv_perc;
 int pos = 0;

 // DVK 2014.11.12 limita faixa de tensão até 10%
 // UpDownFaixaTensao->Max = 10;
 // limita faixa de tensão até 20% (a pedidos, pois antes era 10%)
 UpDownFaixaTensao->Max = 20; // 10;
 try
 { // atualiza EditFaixaTensao
 pos = SendMessage(UpDownFaixaTensao->Handle, (UINT)UDM_GETPOS32, 0, 0);
 // FKM 2016.02.24
 // txt.sprintf("-%02d/%02d", UpDownFaixaTensao->Position, UpDownFaixaTensao->Position);
 txt.sprintf("-%02d/%02d", pos, pos); // FKM 2016.02.24
 EditFaixaTensao->Text = txt;
 // atualiza EditFixoPasso e EditFixoTensao
 ActionSelPassoFixoExecute(NULL);
 // verifica se está mostrando valor de potência nominal
 if (CBoxTipoPot->ItemIndex == 0)
 { // recalcula potência nominal, que é função da faixa de variação de tensão
 // dv_perc  = StrToDouble(UpDownFaixaTensao->Position);
 dv_perc = StrToDouble(pos); // FKM 2016.02.24
 snom_kva = SpassanteToSnom(spassante_kva, dv_perc);
 // reapresenta o valor de Snominal em EditSmva
 EditS_kva->Text = DoubleToStr("%4.3f", snom_kva);
 }
 }
 catch (Exception &e)
 { // nada a fazer
 }
 }
 */
// ---------------------------------------------------------------------------
// eof

