// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TRegulador.h"
#include "VTResFlowLig.h"
#include "VTBarra.h"
#include "VTEqpto.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTRegulador* __fastcall NewObjRegulador(int num_pat)
{
	return (new TRegulador(num_pat));
}

// ---------------------------------------------------------------------------
__fastcall TRegulador::TRegulador(int num_pat)
{
	// variávei locais
	int num_bar = 2;

	// assume modelo padrao, com 3 unidades ligados em delta fechado
	modelo = reguPADRAO;
	// ligacao = lgEST_ISO;
	// fases   = faseABC;
	ligacao = lgDELTA_FECHADO;
	fases = faseABC;
	// inicia atributos p/ visualização gráfica
	Show[eqp_FIXO] = true;
	// cria objeto VTResFlowLig p/ armazenar resultados de fluxo
	AlocaResFlowLig(num_bar, num_pat);
	cfg_auto.v_pu = 1.;
	cfg_auto.vinv_pu = 1.;
	cfg_auto.larg_bandamorta_pu = 0.; // HPS, 2018.03.28
	cfg_auto.larg_bandamorta_inv_pu = 0.; // FKM, 2018.04.16
	rel_tap = 115.;
	// JCG - 2018.04.03
	// aloca memória para vetor passo_calc
	PM_SetMaxPat(num_pat);
	//monta map
	capacidade_max.insert(pair<double, double>(5.00, 1.60));
	capacidade_max.insert(pair<double, double>(6.25, 1.35));
	capacidade_max.insert(pair<double, double>(7.50, 1.20));
	capacidade_max.insert(pair<double, double>(8.75, 1.10));
	capacidade_max.insert(pair<double, double>(10.0, 1.00));
}

// ---------------------------------------------------------------------------
__fastcall TRegulador::~TRegulador(void)
{
	// destrói lista e seus objetos
	if (lisUM != NULL)
	{
		LimpaLisUM();
		delete lisUM;
		lisUM = NULL;
	}
	// JCG - 2018.04.03
	// libera memória alocada para vetor passo_calc
	PM_SetMaxPat(0);
}

// ---------------------------------------------------------------------------
VTRegulador* __fastcall TRegulador::Clone(void)
{
	// variáveis locais
	VTRegulador *regulador;

	// cria um novo Regulador
	if ((regulador = NewObjRegulador(resflow->Max_pat)) != NULL)
	{ // copia dados deste Regulador p/ o novo Regulador
		regulador->CopiaAtributosDe(*this);
	}
	return (regulador);
}

// ---------------------------------------------------------------------------
void __fastcall TRegulador::CopiaAtributosDe(VTEqpto &eqpto)
{
	// variáveis locais
	TRegulador &regulador = (TRegulador&)eqpto;

	// proteção
	if (eqpto.Tipo() != eqptoREGULADOR)
		return;
	// copia atributos de TLigacao
	TLigacao::CopiaAtributosDe(regulador);
	// copia atributos de VTRegulador
	modelo = regulador.modelo;
	by_pass = regulador.by_pass;
	sentido = regulador.sentido;
	rel_tap = regulador.rel_tap;
	// ajuste_auto        = regulador.ajuste_auto;
	ligacao = regulador.ligacao;
	fases = regulador.fases;
	snom = regulador.snom;
	z0 = regulador.z0;
	z1 = regulador.z1;
	var_tensao = regulador.var_tensao;
	num_passo = regulador.num_passo;
	cfg_fixo = regulador.cfg_fixo;
	cfg_auto = regulador.cfg_auto;
	// JCG 01/02/2010: NÃO copia lisUM
}

// ---------------------------------------------------------------------------
void __fastcall TRegulador::DefineBarraEntrada(VTBarra *barra_ent)
{
	// verifica se a Barra de entrada do regulador está correta
	if (sentido.bar_ref != barra_ent)
	{ // redefine Barra de entrada
		sentido.bar_ref = barra_ent;
	}
	// verifica se Barra1 é a Barra de entrada
	if (pbarra1 != sentido.bar_ref)
	{ // inverte as Barras do Regulador p/ que a Barra1 seja a barra de entrada
		pbarra2 = pbarra1;
		pbarra1 = sentido.bar_ref;
	}
	// verifica modelo
	if (modelo == reguENERQCT)
	{ // define barra de referência do ajuste automático igual à barra de entrada
		cfg_auto.bar_ref = pbarra1;
	}
	else
	{ // define barra de referência do ajuste automático igual à barra de saída
		cfg_auto.bar_ref = pbarra2;
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15423981
double __fastcall TRegulador::DeterminaTensao(void)
{
	//variáveis locais
	double vnom_kv = 0.;

	//proteção
	if(this->pbarra1 == NULL) return 0.;
	if(this->pbarra2 == NULL) return 0.;
	vnom_kv = this->pbarra1->vnom;
	if(IsDoubleZero(vnom_kv)) return 0.;
	//verifica ligacao
	switch(ligacao)
//	{
//		case lgEST_AT:
//		case lgEST_ISO:
//		case lgDELTA_ABERTO:
//			vnom_kv = vnom_kv * (sqrt(3.)/3.);
//			break;
//		case lgTRIANG:
//		case lgMONOFASICO:
//		case lgDELTA_FECHADO:
//		case lgINDEFINIDA:
//		default:
//			break;
//	}
	{
		case lgEST_AT:
		case lgEST_ISO:
		case lgMONOFASICO:
			vnom_kv = vnom_kv * (sqrt(3.)/3.);
			break;
		case lgTRIANG:
		case lgDELTA_ABERTO:
		case lgDELTA_FECHADO:
		case lgINDEFINIDA:
		default:
			break;
	}
	return vnom_kv;
}

// ---------------------------------------------------------------------------
int __fastcall TRegulador::Fases(VTBarra *barra)
{
	return (fases);
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TRegulador::IniciaLisUM(void)
 {
 //verifica se já existe lisUM
 if (lisUM != NULL)
 {//destrói objetos em lisUM
 LimpaLisUM();
 }
 else
 {//cria lisUM
 lisUM = new TList();
 }
 //monta lisUM com Reguladores p/ o fluxo
 switch(ligacao)
 {
 case lgMONOFASICO: //insere o próprio Regulador em lisUM
 //InsereUM(fases);
 lisUM->Add(this);
 break;
 case lgDELTA_ABERTO: //insere 2 unidades, delta aberto
 //verifica a fase comum das unidade
 switch(fases)
 {
 case faseC:
 InsereUM(faseBC);
 InsereUM(faseCA);
 break;
 case faseB:
 InsereUM(faseAB);
 InsereUM(faseBC);
 break;
 case faseA:
 default:
 InsereUM(faseCA);
 InsereUM(faseAB);
 break;
 }
 break;
 case lgDELTA_FECHADO: //insere o próprio Regulador em lisUM
 //InsereUM(faseAB);
 //InsereUM(faseBC);
 //InsereUM(faseCA);
 lisUM->Add(this);
 break;
 case lgEST_ISO:   //insere o próprio Regulador em lisUM
 default:
 //InsereUM(faseAN);
 //InsereUM(faseBN);
 //InsereUM(faseCN);
 lisUM->Add(this);
 break;
 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TRegulador::IniciaLisUM(void)
{
	// Variáveis locais
	double snom_mva;

	// verifica se já existe lisUM
	if (lisUM != NULL)
	{ // destrói objetos em lisUM
		LimpaLisUM();
	}
	else
	{ // cria lisUM
		lisUM = new TList();
	}
	// monta lisUM com Reguladores p/ o fluxo
	switch (ligacao)
	{
	case lgMONOFASICO:
		switch (fases)
		{
		case faseAB:
		case faseBC:
		case faseCA:
		case faseAN:
		case faseBN:
		case faseCN:
			snom_mva = snom;
			InsereUM(lgMONOFASICO, fases, 0, snom_mva);
			break;
		case faseABN:
		case faseBCN:
		case faseCAN:
			snom_mva = 2. * snom;
			InsereUM(lgMONOFASICO, fases, faseN, snom_mva);
			break;
		}
		break;
	case lgDELTA_ABERTO: // trata como 1 unidade monofásica
		// InsereUM(lgDELTA_ABERTO, faseABC, fases);
		snom_mva = 2. * snom;
		InsereUM(lgMONOFASICO, faseABC, fases, snom_mva);
		break;
	case lgDELTA_FECHADO:
		snom_mva = 3. * snom;
		InsereUM(lgDELTA_FECHADO, faseABC, 0, snom_mva);
		break;
	case lgEST_AT:
		snom_mva = snom;
		InsereUM(lgMONOFASICO, faseAN, faseN, snom_mva);
		InsereUM(lgMONOFASICO, faseBN, faseN, snom_mva);
		InsereUM(lgMONOFASICO, faseCN, faseN, snom_mva);
		break;
	case lgEST_ISO:
	default:
		snom_mva = 3. * snom;
		InsereUM(lgEST_ISO, faseABC, 0, snom_mva);
		break;
	}
}

// ---------------------------------------------------------------------------
double __fastcall TRegulador::InomA(void)
{
	//variáveis locais
	double vnom_kv, inom_a;
	double cap_max = 1.;

	//determina tensão nominal, em função das fases e do tipo de ligação
	vnom_kv = DeterminaTensao();
	//determina capacidade maxima
	capacidade_max.begin();
	cap_max = capacidade_max.find(var_tensao)->second;
	//calcula corrente em função da potência nominal e da tensão nominal na barra (10%)
	inom_a = ((snom * 1000.) * cap_max) / (vnom_kv * 0.1);
//	inom_a = (snom * 1000.) / (vnom_kv * (0.1 * cap_max));
//    inom_a = (snom * 1000.) / (vnom_kv * (this->var_tensao * 0.01));
	return (inom_a);
}

// ---------------------------------------------------------------------------
double __fastcall TRegulador::LarguraBandaMorta_pu(void)
{
	//return (cfg_auto.larg_bandamorta_pu);
	//variáveis locais
	double largura_pu;
	double banda_sup, banda_inf, nivel_tensao;

	//proteção
	if(IsDoubleZero(rel_tap)) return 0.;
	if(this->pbarra1 == NULL) return 0.;
	if(this->pbarra2 == NULL) return 0.;
	nivel_tensao = NivelTensaoDiretoV();
	if(IsDoubleZero(nivel_tensao)) return 0.;
	//calcula valor em pu, considerando relação de tap, nivel de tensão e referência em V
	banda_sup = rel_tap * (nivel_tensao + (cfg_auto.larg_bandamorta_pu/2.));
	banda_inf = rel_tap * (nivel_tensao - (cfg_auto.larg_bandamorta_pu/2.));
	largura_pu = (banda_sup - banda_inf) / (rel_tap * nivel_tensao);
	return (largura_pu);
}

// ---------------------------------------------------------------------------
double __fastcall TRegulador::LarguraBandaMortaInv_pu(void)
{
	//return (cfg_auto.larg_bandamorta_inv_pu);
	//variáveis locais
	double largura_pu;
	double banda_sup, banda_inf, nivel_tensao;

	//proteção
	if(IsDoubleZero(rel_tap)) return 0.;
	if(this->pbarra1 == NULL) return 0.;
	if(this->pbarra2 == NULL) return 0.;
	nivel_tensao = NivelTensaoInversoV();
	if(IsDoubleZero(nivel_tensao)) return 0.;
	//calcula valor em pu, considerando relação de tap, nivel de tensão e largura em V
	banda_sup = rel_tap * (nivel_tensao + (cfg_auto.larg_bandamorta_inv_pu/2.));
	banda_inf = rel_tap * (nivel_tensao - (cfg_auto.larg_bandamorta_inv_pu/2.));
	largura_pu = (banda_sup - banda_inf) / (rel_tap * nivel_tensao);
	return (largura_pu);
}

// ---------------------------------------------------------------------------
void __fastcall TRegulador::InsereUM(int ligacao, int fase, int fase_comum, double snom_mva)
{
	// variáveis locais
	VTRegulador *regulador;

	try
	{ // cria uma cópia do Regulador e insere em lisUM
		lisUM->Add(regulador = Clone());
		// define falso ID e atualiza p/ Regulador original
		regulador->Id = FalsoId();
		regulador->ligaPai = this;
		// atualiza fases do novo Regulador
		regulador->ligacao = ligacao;
		regulador->fases = fase + (fase_comum << 8);
		regulador->snom = snom_mva;
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TRegulador::LimpaLisUM(void)
{
	// variáveis locais
	VTRegulador *regulador;

	// proteção:
	if (lisUM == NULL)
		return;
	// destrói Regualdores de lisUM diferentes do próprio Regulador
	for (int n = 0; n < lisUM->Count; n++)
	{
		regulador = (VTRegulador*)lisUM->Items[n];
		if (regulador != this)
		{
			delete regulador;
		}
	}
	// reinicia lisUM
	lisUM->Clear();
}

// ---------------------------------------------------------------------------
TList* __fastcall TRegulador::LisUM(void)
{
	if (lisUM == NULL)
		IniciaLisUM();
	return (lisUM);
}

// ---------------------------------------------------------------------------
double __fastcall TRegulador::NivelTensaoDiretoV(void)
{
	//variáveis locais
	double nivel_v;

	//proteção
	if(IsDoubleZero(rel_tap)) return 0.;
	if(this->pbarra1 == NULL) return 0.;
	if(this->pbarra2 == NULL) return 0.;
	nivel_v = double(cfg_auto.v_pu / rel_tap) * (this->pbarra1->vnom * 1000.);
	return (nivel_v);
}

// ---------------------------------------------------------------------------
double __fastcall TRegulador::NivelTensaoInversoV(void)
{
	//variáveis locais
	double nivel_v;

	//proteção
	if(IsDoubleZero(rel_tap)) return 0.;
	if(this->pbarra1 == NULL) return 0.;
	if(this->pbarra2 == NULL) return 0.;
	nivel_v = double(cfg_auto.vinv_pu / rel_tap) * (this->pbarra1->vnom * 1000.);
	return (nivel_v);
}

// ---------------------------------------------------------------------------
int __fastcall TRegulador::NumUM(void)
{
	if (lisUM == NULL)
		IniciaLisUM();
	return (lisUM->Count);
}

/*
 //---------------------------------------------------------------------------
 int __fastcall TRegulador::Passo(int fase)
 {
 //variáveis locais
 int ind_fase;

 //verifica se ajuste fixo ou automático
 if (ajuste_auto)
 {//determina passo em funções das tensões
 int PENDENCIA_FALTA_PASSO_AUTO=1;
 return(cfg_fixo.passo);
 }
 else
 {//ajuste fixo
 return(cfg_fixo.passo);
 }
 }
 */
// ---------------------------------------------------------------------------
int __fastcall TRegulador::Passo(int fase)
{
	return (cfg_fixo.passo);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TRegulador::PM_GetPadrao(void)
{
	return (padrao);
}
/*
// ---------------------------------------------------------------------------
double __fastcall TRegulador::PM_GetSpas_mva(void)
{
	// variáveis locais
	double Spas_mva = snom;

	if (var_tensao > 0)
	{ // calcula potência passante
		Spas_mva = snom / (var_tensao * 0.01);
	}
	return (Spas_mva);
}
*/
// ---------------------------------------------------------------------------
double __fastcall TRegulador::PM_GetSpas_mva(void)
{
	// variáveis locais
	double Spas_mva = snom;
	double cap_max = 1.;

	//determina capacidade maxima
	capacidade_max.begin();
	cap_max = capacidade_max.find(var_tensao)->second;
	if (!IsDoubleZero(cap_max))
	{ // calcula potência passante
		Spas_mva = snom / (0.1 * cap_max);
	}
	return Spas_mva;
}

// ---------------------------------------------------------------------------
void __fastcall TRegulador::PM_SetMaxPat(int max_pat)
{
	// variáveis locais
	int old_num_pat = resflow->Max_pat;

	// libera memória alocada anteriormente
	if (passo_calc != NULL)
	{
		for (int ind_pat = 0; ind_pat < old_num_pat; ind_pat++)
		{
			if (passo_calc[ind_pat])
			{
				delete[]passo_calc[ind_pat];
			}
		}
		delete[]passo_calc;
		passo_calc = NULL;
	}
	// verifica se deve alocar um novo vetor
	if (max_pat > 0)
	{
		passo_calc = new int*[max_pat];
		for (int ind_pat = 0; ind_pat < max_pat; ind_pat++)
		{
			passo_calc[ind_pat] = new int[MAX_FASE];
			for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
			{
				passo_calc[ind_pat][ind_fase] = 0;
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TRegulador::PM_SetPadrao(AnsiString padrao)
{
	this->padrao = padrao;
}
/*
// ---------------------------------------------------------------------------
void __fastcall TRegulador::PM_SetSpas_mva(double s_mva)
{
	if (var_tensao > 0)
	{ // calcula potência nominal
		snom = s_mva * (var_tensao * 0.01);
	}
}
*/
// ---------------------------------------------------------------------------
void __fastcall TRegulador::PM_SetSpas_mva(double s_mva)
{
	//variaveis locais
	double cap_max = 1.;

	//determina capacidade maxima
	capacidade_max.begin();
	cap_max = capacidade_max.find(var_tensao)->second;
	if (!IsDoubleZero(cap_max))
	{ // converte potência passante em potencia nominal
		snom = s_mva * (0.1 * cap_max);
	}
}

// ---------------------------------------------------------------------------
int __fastcall TRegulador::Tipo(void)
{
	return (eqptoREGULADOR);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TRegulador::TipoAsString(void)
{
	return ("Regulador");
}

// ---------------------------------------------------------------------------
// eof
