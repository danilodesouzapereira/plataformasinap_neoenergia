// ---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TBateria.h"
#include "VTFases.h"
#include "..\Classes\VTClasse.h"
#include "..\Curvas\VTCurva.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTBateria* __fastcall NewObjBateria(int num_pat)
{
	return (new TBateria(num_pat));
}

// ---------------------------------------------------------------------------
__fastcall TBateria::TBateria(int num_pat)
{
	// Salva dados
	this->num_pat = num_pat;

	// Inicializa��o
	vet_estado = NULL;
	vet_modo_operacao = NULL;
	vet_earmaz_kwh = NULL;
	vet_pext_kw = NULL;
	vet_qext_kvar = NULL;

	// vetores que ser�o utilizados no c�lculo
	IniciaVetores();
	// valores default
	DefineValoresPadrao();

	// configura��o gr�fica
	Show[bat_P] = true;
	Show[bat_Q] = true;
}

// ---------------------------------------------------------------------------
__fastcall TBateria::~TBateria(void)
{
	// destr�i curvaCargaDescarga somente se n�o for t�pica
	if (curvaCargaDescarga != NULL)
		delete curvaCargaDescarga;
	curvaCargaDescarga = NULL;

	// Libera mem�ria
	delete[]vet_estado;
	delete[]vet_modo_operacao;
	delete[]vet_earmaz_kwh;
	delete[]vet_pext_kw;
	delete[]vet_qext_kvar;
}

// ---------------------------------------------------------------------------
VTBateria* __fastcall TBateria::Clone(void)
{
	// vari�veis locais
	VTBateria *pbateria;

	// cria uma nova bateria
	// if ((pbateria = NewObjBateria(curvaCargaDescarga->Escala)) != NULL) //to comentando pq ainda nao sei como ele faz pra criar curvas no editor
	if ((pbateria = NewObjBateria(num_pat)) != NULL)
	{ // copia dados desta bateria p/ a nova bateria
		pbateria->CopiaAtributosDe(*this);
	}
	return (pbateria);
}

// ---------------------------------------------------------------------------
void __fastcall TBateria::CopiaAtributosDe(VTEqpto &ref)
{
	// vari�veis locais
	TBateria &bateria = (TBateria&)ref;

	// prote��o
	if (ref.Tipo() != eqptoBATERIA)
		return;
	// copia atributos de TEqbar
	TEqbar::CopiaAtributosDe(bateria);
	// copia atributos de VTBateria
	PD_BATERIA = bateria.PD_BATERIA;

	//destr�i Curva pr�pria
	if (curvaCargaDescarga) delete curvaCargaDescarga;
	//cria c�pia de Curva pr�pria
	curvaCargaDescarga = bateria.curvaCargaDescarga->Clone();
	curvaCargaDescarga->Eqpto = this;

	// copia valores dos vetores
	for (int np = 0; np < num_pat; np++)
	{
		Estado[np] = bateria.PM_GetEstado(np);
		Modo_operacao[np] = bateria.PM_GetModo_operacao(np);
		Earmaz_atual_kwh[np] = bateria.PM_GetEarmaz_atual_kwh(np);
		Pext_kw[np] = bateria.PM_GetPext_kw(np);
		Qext_kvar[np] = bateria.PM_GetQext_kvar(np);
	}

}

// ---------------------------------------------------------------------------
void __fastcall TBateria::DefineCurva(VTCurva *curva)
{
	// Esse m�todo se refere a curva de carga e descarga
	// prote��o: verifica se a Curva indicada � a mesma que j� est� associada � bateria
	if (this->curvaCargaDescarga == curva)
		return;
	// destr�i Curva
	if (this->curvaCargaDescarga)
		delete this->curvaCargaDescarga;
	// salva ponteiro p/ objeto
	this->curvaCargaDescarga = curva;
}

// ---------------------------------------------------------------------------
void __fastcall TBateria::DefineValoresPadrao(void)
{
	// Vari�veis nominais (fixos)
	Fases = faseABC;
	Tipo_operacao = toAUTO;
	Icte_pu = 0.;
	Scte_pu = 1.;
	Zcte_pu = 0.; ;
	Pnom_kw = 40.;
	Enom_kwh = 50.;
	Cap_inv_kva = 50.;
	Earmaz_inic_pu = 0.;
	Pext_carga_kw = 20.;
	Qext_carga_kvar = 0.;
	Pext_desc_kw = 15.;
	Qext_desc_kvar = 0.;
	P_ociosa_pu = 0.01;
	Q_ociosa_pu = 0.;
	Rend_carga_pu = 0.9;
	Rend_desc_pu = 0.9;
	Gatilho_carga_pu = 0.05;
	Gatilho_desc_pu = 1.;

	// Estado da bateria
	for (int np = 0; np < num_pat; np++)
	{
		Estado[np] = esDESCARREGADA;
		Modo_operacao[np] = moCARGA;
		Earmaz_atual_kwh[np] = 0.;
		Pext_kw[np] = 0.;
		Qext_kvar[np] = 0.;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TBateria::EstadoBateria(void)
{
	// assume que o fluxo ainda n�o foi calculo
	bool pronto = false;
	// percorre o vetor de energia armazenada em cada patamar
	for (int np = 0; np < num_pat; np++)
	{
        // se encontrar um �nico valor diferente de zero siginfica que o vetor  for preenchido
		if (vet_earmaz_kwh[np] != 0.0)
		{
			pronto = true;
			break;
		}
	}
	return pronto;
}

// ---------------------------------------------------------------------------
VTCurva* __fastcall TBateria::ExisteCurva(void)
{
	// esse m�todo se refere a curva de carga e descarga
	return (curvaCargaDescarga);
}

// ---------------------------------------------------------------------------
void __fastcall TBateria::IniciaVetores(void)
{
	if ((vet_estado == NULL) && (vet_modo_operacao == NULL) &&
	(vet_earmaz_kwh == NULL) && (vet_pext_kw == NULL) &&
	(vet_qext_kvar == NULL) && (num_pat > 0))
	{
		vet_estado = new int[num_pat];
		vet_modo_operacao = new int[num_pat];
		vet_earmaz_kwh = new double[num_pat];
		vet_pext_kw = new double[num_pat];
		vet_qext_kvar = new double[num_pat];
	}
}

// ---------------------------------------------------------------------------
void __fastcall TBateria::LiberaVetores(void)
{
	// Libera mem�ria

	delete[]vet_estado;
	vet_estado = NULL;
	delete[]vet_modo_operacao;
	vet_modo_operacao = NULL;
	delete[]vet_earmaz_kwh;
	vet_earmaz_kwh = NULL;
	delete[]vet_pext_kw;
	vet_pext_kw = NULL;
	delete[]vet_qext_kvar;
	vet_qext_kvar = NULL;

}

// ---------------------------------------------------------------------------
double __fastcall TBateria::PM_GetEarmaz_atual_kwh(int ind_pat)
{
	if ((ind_pat >= 0) && (ind_pat < num_pat))
		return (vet_earmaz_kwh[ind_pat]);
	else
		return (0.);
}

// ---------------------------------------------------------------------------
int __fastcall TBateria::PM_GetEstado(int ind_pat)
{
	if ((ind_pat >= 0) && (ind_pat < num_pat))
		return (vet_estado[ind_pat]);
	else
		return (esDESCARREGADA);
}

// ---------------------------------------------------------------------------
int __fastcall TBateria::PM_GetModeloCarga(void)
{
	// vari�veis locais
	double mc_max;
	int modelo;

	// assume Scte
	modelo = cargaSCTE;
	mc_max = Scte_pu;
	// verifica se � poss�vel modelo Icte
	if (Icte_pu > mc_max)
	{
		modelo = cargaICTE;
		mc_max = Icte_pu;
	}
	// verifica se � poss�vel modelo Zcte
	if (Zcte_pu > mc_max)
	{
		modelo = cargaZCTE;
	}
	return (modelo);
}

// ---------------------------------------------------------------------------
int __fastcall TBateria::PM_GetModeloDesc(void)
{
	// vari�veis locais
	double mc_max;
	int modelo;

	// assume Scte
	modelo = cargaSCTE;
	mc_max = Scte_pu;
	// verifica se � poss�vel modelo Icte
	if (Icte_pu > mc_max)
	{
		modelo = cargaICTE;
		mc_max = Icte_pu;
	}
	// verifica se � poss�vel modelo Zcte
	if (Zcte_pu > mc_max)
	{
		modelo = cargaZCTE;
	}
	return (modelo);
}

// ---------------------------------------------------------------------------
int __fastcall TBateria::PM_GetModo_operacao(int ind_pat)
{
	if ((ind_pat >= 0) && (ind_pat < num_pat))
		return (vet_modo_operacao[ind_pat]);
	else
		return (moOCIOSA);
}

// ---------------------------------------------------------------------------
double __fastcall TBateria::PM_GetPext_kw(int ind_pat)
{
	if ((ind_pat >= 0) && (ind_pat < num_pat))
		return (vet_pext_kw[ind_pat]);
	else
		return (0.);
}

// ---------------------------------------------------------------------------
double __fastcall TBateria::PM_GetQext_kvar(int ind_pat)
{
	if ((ind_pat >= 0) && (ind_pat < num_pat))
		return (vet_qext_kvar[ind_pat]);
	else
		return (0.);
}

// ---------------------------------------------------------------------------
void __fastcall TBateria::PM_SetEarmaz_atual_kwh(int ind_pat, double earmaz_kwh)
{
	if ((ind_pat >= 0) && (ind_pat < num_pat))
		vet_earmaz_kwh[ind_pat] = earmaz_kwh;
}

// ---------------------------------------------------------------------------
void __fastcall TBateria::PM_SetEstado(int ind_pat, int estado)
{
	if ((ind_pat >= 0) && (ind_pat < num_pat))
		vet_estado[ind_pat] = estado;
}

// ---------------------------------------------------------------------------
void __fastcall TBateria::PM_SetModo_operacao(int ind_pat, int modo_operacao)
{
	if ((ind_pat >= 0) && (ind_pat < num_pat))
		vet_modo_operacao[ind_pat] = modo_operacao;
}

// ---------------------------------------------------------------------------
void __fastcall TBateria::PM_SetPext_kw(int ind_pat, double pext_kw)
{
	if ((ind_pat >= 0) && (ind_pat < num_pat))
		vet_pext_kw[ind_pat] = pext_kw;
}

// ---------------------------------------------------------------------------
void __fastcall TBateria::PM_SetQext_kvar(int ind_pat, double qext_kvar)
{
	if ((ind_pat >= 0) && (ind_pat < num_pat))
		vet_qext_kvar[ind_pat] = qext_kvar;
}

// ---------------------------------------------------------------------------
void __fastcall TBateria::ReiniciaNumPat(int np)
{
	num_pat = np;
}

// ---------------------------------------------------------------------------
int __fastcall TBateria::Tipo(void)
{
	return (eqptoBATERIA);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TBateria::TipoAsString(void)
{
	return ("Bateria");
}

// ---------------------------------------------------------------------------
// eof
