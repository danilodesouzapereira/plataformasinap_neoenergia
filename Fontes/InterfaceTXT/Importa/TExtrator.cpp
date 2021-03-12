// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <SysUtils.hpp>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjo.h>
#include <Fontes\Arranjo\VTCabo.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTPadraoRede.h>
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Log\VTLog.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <DLL_Inc\Funcao.h>
#include "TExtrator.h"
#include "VTPreMonta.h"

// ---------------------------------------------------------------------------
VTExtrator* NewObjExtrator(VTApl *apl)
{
	return (new TExtrator(apl));
}

// ---------------------------------------------------------------------------
__fastcall TExtrator::TExtrator(VTApl *apl)
{
	// salva ponteiros p/ objeto
	this->apl = apl;
	// define versão
	versao = 1;
	// inicia opções da importação
	ArranjoEnabled = true;
	CurvaTipicaEnabled = true;
	TipoChaveEnabled = true;
	TipoConsumidorEnabled = true;
	TipoRedeEnabled = true;
	RedeEnabled = true;
}

// ---------------------------------------------------------------------------
__fastcall TExtrator::~TExtrator(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereArranjoCaboPorfase(TStrings *campos)
{
	// variáveis locais
	int nc;
	AnsiString nao_usado;
	sdi_ARRANJO sdi;

	// verifica se importação habilitada
	if (!ArranjoEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Arranjo Cabo/Fase");
	// valida número de dados recebidos
	if (campos->Count < 16)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.tipo = arranjoCABOxFASE;
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.fases = campos->Strings[++nc];
		sdi.iadm_amp = 0;
		// preenche campos do arranjoCABOxFASE
		sdi.id_suporte = StrToInteger(campos->Strings[++nc]);
		nao_usado = campos->Strings[++nc];
		sdi.id_pto_caboA = StrToInteger(campos->Strings[++nc]);
		sdi.id_caboA = StrToInteger(campos->Strings[++nc]);
		nao_usado = campos->Strings[++nc];
		sdi.id_pto_caboB = StrToInteger(campos->Strings[++nc]);
		sdi.id_caboB = StrToInteger(campos->Strings[++nc]);
		nao_usado = campos->Strings[++nc];
		sdi.id_pto_caboC = StrToInteger(campos->Strings[++nc]);
		sdi.id_caboC = StrToInteger(campos->Strings[++nc]);
		nao_usado = campos->Strings[++nc];
		sdi.id_pto_caboN = StrToInteger(campos->Strings[++nc]);
		sdi.id_caboN = StrToInteger(campos->Strings[++nc]);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere Barra
	if (!pre_monta->InsereArranjoCaboPorfase(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereArranjoCaboZ0Z1(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_ARRANJO sdi;

	// verifica se importação habilitada
	if (!ArranjoEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Arranjo CaboZ0Z1");
	// valida número de dados recebidos
	if (campos->Count < 4)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.tipo = arranjoCABO_Z0Z1;
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.fases = campos->Strings[++nc];
		sdi.iadm_amp = 0;
		// preenche campos do arranjoCABO_Z0Z1
		sdi.id_cabo_Z0Z1 = StrToInteger(campos->Strings[++nc]);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere Barra
	if (!pre_monta->InsereArranjoCaboZ0Z1(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereArranjoPUSB100(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_ARRANJO sdi;

	// verifica se importação habilitada
	if (!ArranjoEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Arranjo PuSb=100");
	// valida número de dados recebidos
	if (campos->Count < 10)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.tipo = arranjoPU_SB100;
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.fases = campos->Strings[++nc];
		sdi.iadm_amp = StrToDouble(campos->Strings[++nc]);
		// preenche campos do arranjoZ0Z1
		sdi.z0_ohm_km.r = StrToDouble(campos->Strings[++nc]);
		sdi.z0_ohm_km.x = StrToDouble(campos->Strings[++nc]);
		sdi.c0 = StrToDouble(campos->Strings[++nc]);
		sdi.z1_ohm_km.r = StrToDouble(campos->Strings[++nc]);
		sdi.z1_ohm_km.x = StrToDouble(campos->Strings[++nc]);
		sdi.c1 = StrToDouble(campos->Strings[++nc]);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere Barra
	if (!pre_monta->InsereArranjoPUSB100(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereArranjoZ0Z1(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_ARRANJO sdi;

	// verifica se importação habilitada
	if (!ArranjoEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Arranjo Z0Z1");
	// valida número de dados recebidos
	if (campos->Count < 10)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.tipo = arranjoZ0Z1;
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.fases = campos->Strings[++nc];
		sdi.iadm_amp = StrToDouble(campos->Strings[++nc]);
		// preenche campos do arranjoZ0Z1
		sdi.z0_ohm_km.r = StrToDouble(campos->Strings[++nc]);
		sdi.z0_ohm_km.x = StrToDouble(campos->Strings[++nc]);
		sdi.c0 = StrToDouble(campos->Strings[++nc]);
		sdi.z1_ohm_km.r = StrToDouble(campos->Strings[++nc]);
		sdi.z1_ohm_km.x = StrToDouble(campos->Strings[++nc]);
		sdi.c1 = StrToDouble(campos->Strings[++nc]);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere Barra
	if (!pre_monta->InsereArranjoZ0Z1(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereBarra(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_BARRA sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Barra");
	// valida número de dados recebidos
	if (campos->Count < 4)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.vnom_kv = StrToDouble(campos->Strings[++nc]);
		// converte coordenadas utm de m p/ cm
		sdi.utm_m.x = StrToDouble(campos->Strings[++nc]);
		sdi.utm_m.y = StrToDouble(campos->Strings[++nc]);
		sdi.esq.x1 = StrToInteger(campos->Strings[++nc]);
		sdi.esq.y1 = StrToInteger(campos->Strings[++nc]);
		sdi.esq.x2 = StrToInteger(campos->Strings[++nc]);
		sdi.esq.y2 = StrToInteger(campos->Strings[++nc]);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere Barra
	if (!pre_monta->InsereBarra(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereBateria(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_BATERIA sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Bateria");
	// valida número de dados recebidos
	if (campos->Count < 20)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.id_barra = StrToInteger(campos->Strings[++nc]);
		sdi.fases = campos->Strings[++nc];
		sdi.tipo_operacao = StrToInteger(campos->Strings[++nc]);
		sdi.i_cte_pu = StrToDouble(campos->Strings[++nc]);
		sdi.s_cte_pu = StrToDouble(campos->Strings[++nc]);
		sdi.z_cte_pu = StrToDouble(campos->Strings[++nc]);
		sdi.modelo_carga = StrToInteger(campos->Strings[++nc]);
		sdi.modelo_desc = StrToInteger(campos->Strings[++nc]);
		sdi.p_nom_kw = StrToDouble(campos->Strings[++nc]);
		sdi.e_nom_kwh = StrToDouble(campos->Strings[++nc]);
		sdi.cap_inv_kva = StrToDouble(campos->Strings[++nc]);
		sdi.p_ociosa_pu = StrToDouble(campos->Strings[++nc]);
		sdi.q_ociosa_pu = StrToDouble(campos->Strings[++nc]);
		sdi.rend_carga_pu = StrToDouble(campos->Strings[++nc]);
		sdi.rend_desc_pu = StrToDouble(campos->Strings[++nc]);
		sdi.gatilho_carga_pu = StrToDouble(campos->Strings[++nc]);
		sdi.gatilho_desc_pu = StrToDouble(campos->Strings[++nc]);

		// lê pontos da Curva
		if (!LePontosCurva(sdi.curva, campos, nc))
		{
			if (plog)
				plog->Add("Pontos inválidos");
			return (false);
		}

		// esquematico
		sdi.esq.posbar = StrToDouble(campos->Strings[++nc]);
		sdi.esq.dxbar = StrToInteger(campos->Strings[++nc]);
		sdi.esq.dybar = StrToInteger(campos->Strings[++nc]);

	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	if (!pre_monta->InsereBateria(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TExtrator::InsereBarraStart(void)
{
	pre_monta->InsereBarraStart();
}

// ---------------------------------------------------------------------------
void __fastcall TExtrator::InsereBarraStop(void)
{
	pre_monta->InsereBarraStop();
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCaboUnipolar(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_CABO sdi;

	// verifica se importação habilitada
	if (!ArranjoEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Cabo Unipolar");
	// valida número de dados recebidos
	if (campos->Count < 6)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.tipo = caboR_GMR;
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.iadm_amp = StrToDouble(campos->Strings[++nc]);
		sdi.tipo = caboR_GMR;
		// preenche campos caboR_GMR
		sdi.raio_mm = StrToDouble(campos->Strings[++nc]);
		sdi.rmg_mm = StrToDouble(campos->Strings[++nc]);
		sdi.r_ohm_km = StrToDouble(campos->Strings[++nc]);
		// insere Cabo
		if (!pre_monta->InsereCaboUnipolar(sdi))
			return (false);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCaboZ0Z1(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_CABO sdi;

	// verifica se importação habilitada
	if (!ArranjoEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Cabo Z0Z1");
	// valida número de dados recebidos
	if (campos->Count < 9)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.tipo = caboZ0Z1;
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.iadm_amp = StrToDouble(campos->Strings[++nc]);
		sdi.tipo = caboZ0Z1;
		// preenche campos do caboZ0Z1
		sdi.z0_ohm_km.r = StrToDouble(campos->Strings[++nc]);
		sdi.z0_ohm_km.x = StrToDouble(campos->Strings[++nc]);
		sdi.c0 = StrToDouble(campos->Strings[++nc]);
		sdi.z1_ohm_km.r = StrToDouble(campos->Strings[++nc]);
		sdi.z1_ohm_km.x = StrToDouble(campos->Strings[++nc]);
		sdi.c1 = StrToDouble(campos->Strings[++nc]);
		// insere Cabo
		if (!pre_monta->InsereCaboZ0Z1(sdi))
			return (false);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCapacitor(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_CAPACITOR sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Capacitor");
	// valida número de dados recebidos
	if (campos->Count < 10)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_barra = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.vnom_kv = StrToDouble(campos->Strings[++nc]);
		sdi.snom_kva = StrToDouble(campos->Strings[++nc]);
		sdi.ligacao = campos->Strings[++nc];
		// lê Curva
		if (!LeCurva(sdi.curva, campos, nc))
			return (false);
		// esquematico
		sdi.esq.posbar = StrToDouble(campos->Strings[++nc]);
		sdi.esq.dxbar = StrToInteger(campos->Strings[++nc]);
		sdi.esq.dybar = StrToInteger(campos->Strings[++nc]);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere Capacitor
	if (!pre_monta->InsereCapacitor(sdi))
		return (false);
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCapacitorSerie(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_CAPSERIE sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Capacitor Série");
	// valida número de dados recebidos
	if (campos->Count < 8)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_bar1 = StrToInteger(campos->Strings[++nc]);
		sdi.id_bar2 = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.vnom_kv = StrToDouble(campos->Strings[++nc]);
		sdi.snom_kva = StrToDouble(campos->Strings[++nc]);
		sdi.z_ohm.r = StrToDouble(campos->Strings[++nc]);
		sdi.z_ohm.x = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar1 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar2 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar3 = 0;
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere Capacitor
	if (!pre_monta->InsereCapacitorSerie(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCargaComMedicao(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_CARGA sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Carga com Medição");
	// valida número de dados recebidos
	if (campos->Count < 11)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_barra = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.grupo_tarifa = campos->Strings[++nc];
		sdi.classe = campos->Strings[++nc];
		sdi.fases = campos->Strings[++nc];
		sdi.i_cte_pu = StrToDouble(campos->Strings[++nc]);
		sdi.s_cte_pu = StrToDouble(campos->Strings[++nc]);
		sdi.z_cte_pu = StrToDouble(campos->Strings[++nc]);
		sdi.consumo_kwhm = StrToDouble(campos->Strings[++nc]);
		// lê pontos da Curva
		if (!LePontosCurva(sdi.curva, campos, nc))
		{
			if (plog)
				plog->Add("Pontos inválidos");
			return (false);
		}
		// esquematico
		sdi.esq.posbar = StrToDouble(campos->Strings[++nc]);
		sdi.esq.dxbar = StrToInteger(campos->Strings[++nc]);
		sdi.esq.dybar = StrToInteger(campos->Strings[++nc]);
		// lê, se disponível, a quantidade de consumidores por classe
		if (campos->Count == (nc + 7))
		{
			sdi.num_cons.residencial = StrToInteger(campos->Strings[++nc]);
			sdi.num_cons.comercial = StrToInteger(campos->Strings[++nc]);
			sdi.num_cons.industrial = StrToInteger(campos->Strings[++nc]);
			sdi.num_cons.rural = StrToInteger(campos->Strings[++nc]);
			sdi.num_cons.outros = StrToInteger(campos->Strings[++nc]);
			sdi.num_cons.a4 = StrToInteger(campos->Strings[++nc]);
		}
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere ConsumidorBT  //DVK 2015.10.26 não interrompe importação
	// if (pre_monta->InsereCargaComMedicao(sdi) == NULL) return(false);
	pre_monta->InsereCargaComMedicao(sdi);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCargaIP(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_CARGA_IP sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Carga IP");
	// valida número de dados recebidos
	if (campos->Count < 10)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_barra = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.fases = campos->Strings[++nc];
		sdi.num_lampada = StrToInteger(campos->Strings[++nc]);
		sdi.p_w = StrToDouble(campos->Strings[++nc]);
		sdi.fp_pu = StrToDouble(campos->Strings[++nc]);
		// esquematico
		sdi.esq.posbar = StrToDouble(campos->Strings[++nc]);
		sdi.esq.dxbar = StrToInteger(campos->Strings[++nc]);
		sdi.esq.dybar = StrToInteger(campos->Strings[++nc]);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere ConsumidorBT //DVK 2015.11.04 não interrompe importação
	// if (pre_monta->InsereCargaIP(sdi) == NULL) return(false);
	pre_monta->InsereCargaIP(sdi);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCargaTipica(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_CARGA sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Carga Típica");
	// valida número de dados recebidos
	if (campos->Count < 13)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_barra = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.grupo_tarifa = campos->Strings[++nc];
		sdi.classe = campos->Strings[++nc];
		sdi.fases = campos->Strings[++nc];
		sdi.i_cte_pu = StrToDouble(campos->Strings[++nc]);
		sdi.s_cte_pu = StrToDouble(campos->Strings[++nc]);
		sdi.z_cte_pu = StrToDouble(campos->Strings[++nc]);
		sdi.consumo_kwhm = StrToDouble(campos->Strings[++nc]);
		// esquematico
		sdi.esq.posbar = StrToDouble(campos->Strings[++nc]);
		sdi.esq.dxbar = StrToInteger(campos->Strings[++nc]);
		sdi.esq.dybar = StrToInteger(campos->Strings[++nc]);
		// lê, se disponível, a quantidade de consumidores por classe
		if (campos->Count == (nc + 7))
		{
			sdi.num_cons.residencial = StrToInteger(campos->Strings[++nc]);
			sdi.num_cons.comercial = StrToInteger(campos->Strings[++nc]);
			sdi.num_cons.industrial = StrToInteger(campos->Strings[++nc]);
			sdi.num_cons.rural = StrToInteger(campos->Strings[++nc]);
			sdi.num_cons.outros = StrToInteger(campos->Strings[++nc]);
			sdi.num_cons.a4 = StrToInteger(campos->Strings[++nc]);
		}
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere ConsumidorBT //DVK 2015.10.22 não interrompe importação
	// if (pre_monta->InsereCargaTipica(sdi) == NULL)    return(false);
	pre_monta->InsereCargaTipica(sdi);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereChave(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_CHAVE sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Chave");
	// valida número de dados recebidos
	if (campos->Count < 15)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_bar1 = StrToInteger(campos->Strings[++nc]);
		sdi.id_bar2 = StrToInteger(campos->Strings[++nc]);
		sdi.tipochave.id = StrToInteger(campos->Strings[++nc]);
		sdi.tipochave.tipo = StrToInteger(campos->Strings[++nc]);
		sdi.tipochave.cod = campos->Strings[++nc];
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.iadm_amp = StrToDouble(campos->Strings[++nc]);
		sdi.estado = StrToInteger(campos->Strings[++nc]);
		sdi.protecao_ativa = bool(StrToInteger(campos->Strings[++nc]));
		sdi.telecomandada = bool(StrToInteger(campos->Strings[++nc]));
		sdi.tecno_comunicacao = StrToInteger(campos->Strings[++nc]);

		sdi.tecno_comunicacao = 0;

		// esquematico
		sdi.esq.posbar1 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar2 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar3 = 0;
		sdi.tipo_gis = "";
		sdi.elo_fusivel = "";
		if (campos->Count == 17)
		{
			sdi.tipo_gis = campos->Strings[++nc];
			sdi.elo_fusivel = campos->Strings[++nc];
		}
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere Chave
	if (!pre_monta->InsereChave(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCurvaCapacitor(TStrings *campos)
{
	// variáveis locais
	sdi_CURVA sdi;

	// verifica se importação habilitada
	if (!CurvaTipicaEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "CurvaCapacitor");
	// lê dados de curva típica
	if (!LeCurvaTipica(campos, sdi))
		return (false);
	// insere Curva típica de capacitor
	return (pre_monta->InsereCurvaCapacitor(sdi));
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCurvaCarga(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_CURVA sdi;

	// verifica se importação habilitada
	if (!CurvaTipicaEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "CurvaDemanda");
	// lê dados de curva típica
	if (!LeCurvaTipica(campos, sdi))
		return (false);
	// insere Curva típica de demanda
	return (pre_monta->InsereCurvaCarga(sdi));
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCurvaCargaIP(TStrings *campos)
{
	// variáveis locais
	int nc, offset;
	sdi_CURVA sdi;

	// verifica se importação habilitada
	if (!CurvaTipicaEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "CurvaIP");
	// lê dados de curva típica
	if (!LeCurvaTipica(campos, sdi))
		return (false);
	// insere Curva típica de IP
	return (pre_monta->InsereCurvaCargaIP(sdi));
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCurvaGerador(TStrings *campos)
{
	// variáveis locais
	int nc, offset;
	sdi_CURVA sdi;

	// verifica se importação habilitada
	if (!CurvaTipicaEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "CurvaGerador");
	// lê dados de curva típica
	if (!LeCurvaTipica(campos, sdi))
		return (false);
	// insere Curva típica de gerador
	return (pre_monta->InsereCurvaGerador(sdi));
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereCurvaReator(TStrings *campos)
{
	// variáveis locais
	int nc, offset;
	sdi_CURVA sdi;

	// verifica se importação habilitada
	if (!CurvaTipicaEnabled)
		return (true);
	// configura CurvaTipicaEnabled
	if (plog)
		plog->DefineValor("Eqpto", "CurvaReator");
	// lê dados de curva típica
	if (!LeCurvaTipica(campos, sdi))
		return (false);
	// insere Curva típica de reator
	return (pre_monta->InsereCurvaReator(sdi));
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15928772
bool __fastcall TExtrator::InsereDiagnostico(TStrings *campos)
{
	// variáveis locais
	int nc;
	double aux;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTPadraoRede *padraoRede;

	// configura log
	if (plog)
		plog->DefineValor("Eqpto", "Diagnóstico para Fluxo");
	// valida número de dados recebidos
	if (campos->Count < 27)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	// lê dados de limite para diagnostico
	try
	{ // carrega valores redeSEC
		padraoRede = geral->ExistePadraoRede(redeSEC);
		padraoRede->V1 = StrToDouble(campos->Strings[nc = 0]);
		padraoRede->V2 = StrToDouble(campos->Strings[++nc]);
		padraoRede->V3 = StrToDouble(campos->Strings[++nc]);
		padraoRede->V4 = StrToDouble(campos->Strings[++nc]);
		padraoRede->FPI1 = StrToDouble(campos->Strings[++nc]);
		padraoRede->FPI2 = StrToDouble(campos->Strings[++nc]);
		padraoRede->FPS1 = StrToDouble(campos->Strings[++nc]);
		padraoRede->FPS2 = StrToDouble(campos->Strings[++nc]);
		// carrega valores redePRI
		padraoRede = geral->ExistePadraoRede(redePRI);
		padraoRede->V1 = StrToDouble(campos->Strings[++nc]);
		padraoRede->V2 = StrToDouble(campos->Strings[++nc]);
		padraoRede->V3 = StrToDouble(campos->Strings[++nc]);
		padraoRede->V4 = StrToDouble(campos->Strings[++nc]);
		padraoRede->FPI1 = StrToDouble(campos->Strings[++nc]);
		padraoRede->FPI2 = StrToDouble(campos->Strings[++nc]);
		padraoRede->FPS1 = StrToDouble(campos->Strings[++nc]);
		padraoRede->FPS2 = StrToDouble(campos->Strings[++nc]);
		// carrega valores redeSUB
		padraoRede = geral->ExistePadraoRede(redeSUB);
		padraoRede->V1 = StrToDouble(campos->Strings[++nc]);
		padraoRede->V2 = StrToDouble(campos->Strings[++nc]);
		padraoRede->V3 = StrToDouble(campos->Strings[++nc]);
		padraoRede->V4 = StrToDouble(campos->Strings[++nc]);
		padraoRede->FPI1 = StrToDouble(campos->Strings[++nc]);
		padraoRede->FPI2 = StrToDouble(campos->Strings[++nc]);
		padraoRede->FPS1 = StrToDouble(campos->Strings[++nc]);
		padraoRede->FPS2 = StrToDouble(campos->Strings[++nc]);
		// cores
		aux = StrToDouble(campos->Strings[++nc]);
		geral->COR.Faixa[diagBOM] = (TColor)(int)aux;
		aux = StrToDouble(campos->Strings[++nc]);
		geral->COR.Faixa[diagREGULAR] = (TColor)(int)aux;
		aux = StrToDouble(campos->Strings[++nc]);
		geral->COR.Faixa[diagRUIM] = (TColor)(int)aux;
		geral->SalvaGeral(false);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	catch (...)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereFiltro(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_FILTRO sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Filtro");
	// valida número de dados recebidos
	if (campos->Count < 10)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_bar = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.vnom_kv = StrToDouble(campos->Strings[++nc]);
		sdi.snom_kva = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.x = StrToDouble(campos->Strings[++nc]);
		// esquematico
		sdi.esq.posbar = StrToDouble(campos->Strings[++nc]);
		sdi.esq.dxbar = StrToInteger(campos->Strings[++nc]);
		sdi.esq.dybar = StrToInteger(campos->Strings[++nc]);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere Capacitor
	if (!pre_monta->InsereFiltro(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereGerador(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_GERADOR sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// atualiza  Resumo
	if (plog)
		plog->DefineValor("Eqpto", "Gerador");
	// valida número de dados recebidos (sem considerar a curva)
	if (campos->Count < 16)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê dados
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_bar = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.tipo_geracao = StrToInteger(campos->Strings[++nc]);
		sdi.vnom_kv = StrToDouble(campos->Strings[++nc]);
		sdi.snom_kva = StrToDouble(campos->Strings[++nc]);
		sdi.fplim_pu = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.x = StrToDouble(campos->Strings[++nc]);
		sdi.z1_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.z1_pu.x = StrToDouble(campos->Strings[++nc]);
		sdi.ligacao = StrToInteger(campos->Strings[++nc]);
		sdi.tipo_sup = StrToInteger(campos->Strings[++nc]);
		if (!LeCurva(sdi.curva, campos, nc))
			return (false);
		// esquematico
		sdi.esq.posbar = StrToDouble(campos->Strings[++nc]);
		sdi.esq.dxbar = StrToInteger(campos->Strings[++nc]);
		sdi.esq.dybar = StrToInteger(campos->Strings[++nc]);
		// verifica se há mais 1 campo, com as fases
		if (campos->Count > (nc + 1))
		{
			sdi.fases = StrToInteger(campos->Strings[++nc]);
		}
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere Suprimento
	if (!pre_monta->InsereGerador(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereMedidor(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_MEDIDOR sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Medidor");
	// valida número de dados recebidos
	if (campos->Count < 7)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_bar = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		// esquemático
		sdi.esq.posbar = StrToDouble(campos->Strings[++nc]);
		sdi.esq.dxbar = StrToInteger(campos->Strings[++nc]);
		sdi.esq.dybar = StrToInteger(campos->Strings[++nc]);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere Medidor
	if (!pre_monta->InsereMedidor(sdi))
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereMedidorCanal(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_CANAL sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Canal");
	// valida número de dados recebidos
	if (campos->Count < 9)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_medidor = StrToInteger(campos->Strings[++nc]);
		sdi.eqpto_tipo = StrToInteger(campos->Strings[++nc]);
		sdi.eqpto_id = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.numero = StrToInteger(campos->Strings[++nc]);
		sdi.tipocanal = StrToInteger(campos->Strings[++nc]);
		sdi.fases = StrToInteger(campos->Strings[++nc]);
		sdi.toler_perc = StrToDouble(campos->Strings[++nc]);
		// lê Curva
		if (!LeCurva(sdi.curva, campos, nc))
			return (false);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere Canal
	if (!pre_monta->InsereMedidorCanal(sdi))
	{
		return (true);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereMutua(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_MUTUA sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Mútua");
	// valida número de dados recebidos
	if (campos->Count < 12)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_tre1 = StrToInteger(campos->Strings[++nc]);
		sdi.id_bar_tre1 = StrToInteger(campos->Strings[++nc]);
		sdi.id_tre2 = StrToInteger(campos->Strings[++nc]);
		sdi.id_bar_tre2 = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.z0.r = StrToDouble(campos->Strings[++nc]);
		sdi.z0.x = StrToDouble(campos->Strings[++nc]);
		sdi.z1.r = StrToDouble(campos->Strings[++nc]);
		sdi.z1.x = StrToDouble(campos->Strings[++nc]);
		// esquematico
		sdi.postre1 = StrToDouble(campos->Strings[++nc]);
		sdi.postre2 = StrToInteger(campos->Strings[++nc]);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere Capacitor
	if (!pre_monta->InsereMutua(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereReator(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_REATOR sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Reator");
	// valida número de dados recebidos
	if (campos->Count < 11)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_bar = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.ligacao = StrToInteger(campos->Strings[++nc]);
		sdi.vnom_kv = StrToDouble(campos->Strings[++nc]);
		sdi.q_kvar = StrToDouble(campos->Strings[++nc]);
		// lê Curva
		if (!LeCurva(sdi.curva, campos, nc))
			return (false);
		// esquematico
		sdi.esq.posbar = StrToDouble(campos->Strings[++nc]);
		sdi.esq.dxbar = StrToInteger(campos->Strings[++nc]);
		sdi.esq.dybar = StrToInteger(campos->Strings[++nc]);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere Reator
	if (!pre_monta->InsereReator(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereRede(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_REDE sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Rede");
	// valida número de dados recebidos
	if (campos->Count < 6)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.tiporede.id = StrToInteger(campos->Strings[++nc]);
		sdi.tiporede.seg = StrToInteger(campos->Strings[++nc]);
		sdi.tiporede.cod = campos->Strings[++nc];
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.area = campos->Strings[++nc];
		// assmume valores default para campos que podem não existir
		sdi.color_r = 0;
		sdi.color_g = 0;
		sdi.color_b = 0;
		sdi.cluster = "";
		sdi.barini_id = 0;
		sdi.radial = 0;
		sdi.estatica = 0;
		// verifica se foram definidoa campos de cor
		if (campos->Count > 9)
		{ // lê informação de cor
			sdi.color_r = StrToInteger(campos->Strings[++nc]);
			sdi.color_g = StrToInteger(campos->Strings[++nc]);
			sdi.color_b = StrToInteger(campos->Strings[++nc]);
		}
		// verifica se existe campo de cluster
		if (campos->Count > 10)
			sdi.cluster = campos->Strings[++nc];
		// verifica se existe campo de barra inicial
		if (campos->Count > 11)
			sdi.barini_id = StrToInteger(campos->Strings[++nc]);
		// verifica se existe campo de radial
		if (campos->Count > 12)
			sdi.radial = StrToInteger(campos->Strings[++nc]);
		// verifica se existe campo de estática
		if (campos->Count > 13)
			sdi.estatica = StrToInteger(campos->Strings[++nc]);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere SE em PreMonta
	if (!pre_monta->InsereRede(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15928772
bool __fastcall TExtrator::InsereReguladorAutomatico(TStrings *campos)
{
	switch (versao)
	{
	case 2:
		return (InsereReguladorAutomaticoVersao2(campos));
	case 3:
		return (InsereReguladorAutomaticoVersao3(campos));
	case 4:
	case 5:
		return (InsereReguladorAutomaticoVersao4(campos));
	default:
		if(plog)
			plog->Add("Versão incompatível para importar Regulador de Tensão");
		return false;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereReguladorAutomaticoVersao2(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_REGULADOR sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Regulador Auto");
	// valida número de dados recebidos
	if (campos->Count < 16)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_bar1 = StrToInteger(campos->Strings[++nc]);
		sdi.id_bar2 = StrToInteger(campos->Strings[++nc]);
		sdi.id_bar_ent = StrToInteger(campos->Strings[++nc]);
		sdi.modelo = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.snom_kva = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.x = StrToDouble(campos->Strings[++nc]);
		sdi.z1_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.z1_pu.x = StrToDouble(campos->Strings[++nc]);
		sdi.num_eqpto = StrToInteger(campos->Strings[++nc]);
		sdi.ligacao = campos->Strings[++nc];
		sdi.fases = campos->Strings[++nc];
		sdi.dv_perc = StrToDouble(campos->Strings[++nc]);
		sdi.num_passo = StrToInteger(campos->Strings[++nc]);
		// tap ajustável
		sdi.ajuste_auto.id_bar_ref = StrToInteger(campos->Strings[++nc]);
		sdi.ajuste_auto.v_pu = StrToDouble(campos->Strings[++nc]);
		// sentido inverso: assume igual ao sentido direto (versão antiga)
		sdi.ajuste_auto.id_bar_ref_inv = sdi.ajuste_auto.id_bar_ref;
		sdi.ajuste_auto.vinv_pu = sdi.ajuste_auto.v_pu;
		// assume largura de banda morta = 0
		sdi.ajuste_auto.larg_bandamorta_pu = 0;
		sdi.ajuste_auto.larg_bandamorta_inv_pu = 0;
		// esquematico
		sdi.esq.posbar1 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar2 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar3 = 0;
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere SE em PreMonta
	if (!pre_monta->InsereReguladorAutomatico(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereReguladorAutomaticoVersao3(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_REGULADOR sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Regulador Auto");
	// valida número de dados recebidos
	if (campos->Count < 23)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_bar1 = StrToInteger(campos->Strings[++nc]);
		sdi.id_bar2 = StrToInteger(campos->Strings[++nc]);
		sdi.id_bar_ent = StrToInteger(campos->Strings[++nc]);
		sdi.modelo = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.snom_kva = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.x = StrToDouble(campos->Strings[++nc]);
		sdi.z1_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.z1_pu.x = StrToDouble(campos->Strings[++nc]);
		sdi.num_eqpto = StrToInteger(campos->Strings[++nc]);
		sdi.ligacao = campos->Strings[++nc];
		sdi.fases = campos->Strings[++nc];
		sdi.dv_perc = StrToDouble(campos->Strings[++nc]);
		sdi.num_passo = StrToInteger(campos->Strings[++nc]);
		// tap ajustável
		sdi.ajuste_auto.id_bar_ref = StrToInteger(campos->Strings[++nc]);
		sdi.ajuste_auto.v_pu = StrToDouble(campos->Strings[++nc]);
		// sentido inverso (versão 3)
		sdi.ajuste_auto.id_bar_ref_inv = StrToInteger(campos->Strings[++nc]);
		sdi.ajuste_auto.vinv_pu = StrToDouble(campos->Strings[++nc]);
		// assume largura de banda morta = 0
		sdi.ajuste_auto.larg_bandamorta_pu = 0;
		sdi.ajuste_auto.larg_bandamorta_inv_pu = 0;
		// esquematico
		sdi.esq.posbar1 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar2 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar3 = 0;
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere SE em PreMonta
	if (!pre_monta->InsereReguladorAutomatico(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereReguladorAutomaticoVersao4(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_REGULADOR sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Regulador Auto");
	// valida número de dados recebidos
	if (campos->Count < 25)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_bar1 = StrToInteger(campos->Strings[++nc]);
		sdi.id_bar2 = StrToInteger(campos->Strings[++nc]);
		sdi.id_bar_ent = StrToInteger(campos->Strings[++nc]);
		sdi.modelo = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.snom_kva = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.x = StrToDouble(campos->Strings[++nc]);
		sdi.z1_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.z1_pu.x = StrToDouble(campos->Strings[++nc]);
		sdi.num_eqpto = StrToInteger(campos->Strings[++nc]);
		sdi.ligacao = campos->Strings[++nc];
		sdi.fases = campos->Strings[++nc];
		sdi.dv_perc = StrToDouble(campos->Strings[++nc]);
		sdi.num_passo = StrToInteger(campos->Strings[++nc]);
		// tap ajustável
		sdi.ajuste_auto.id_bar_ref = StrToInteger(campos->Strings[++nc]);
		sdi.ajuste_auto.v_pu = StrToDouble(campos->Strings[++nc]);
		// sentido inverso (versão 3)
		sdi.ajuste_auto.id_bar_ref_inv = StrToInteger(campos->Strings[++nc]);
		sdi.ajuste_auto.vinv_pu = StrToDouble(campos->Strings[++nc]);
		// banda morta (versao 4)
		sdi.ajuste_auto.larg_bandamorta_pu = StrToDouble(campos->Strings[++nc]);
		sdi.ajuste_auto.larg_bandamorta_inv_pu = StrToDouble(campos->Strings[++nc]);
		// esquematico
		sdi.esq.posbar1 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar2 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar3 = 0;
		// relação de tap
		if (campos->Count == (nc + 2))
		{
			sdi.rtp = StrToInteger(campos->Strings[++nc]);
		}
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere SE em PreMonta
	if (!pre_monta->InsereReguladorAutomatico(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereReguladorTapFixo(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_REGULADOR sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Regulador Fixo");
	// valida número de dados recebidos
	if (campos->Count < 16)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_bar1 = StrToInteger(campos->Strings[++nc]);
		sdi.id_bar2 = StrToInteger(campos->Strings[++nc]);
		sdi.id_bar_ent = StrToInteger(campos->Strings[++nc]);
		sdi.modelo = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.snom_kva = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.x = StrToDouble(campos->Strings[++nc]);
		sdi.z1_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.z1_pu.x = StrToDouble(campos->Strings[++nc]);
		sdi.num_eqpto = StrToInteger(campos->Strings[++nc]);
		sdi.ligacao = campos->Strings[++nc];
		sdi.fases = campos->Strings[++nc];
		sdi.dv_perc = StrToDouble(campos->Strings[++nc]);
		sdi.num_passo = StrToInteger(campos->Strings[++nc]);
		// tap fixo
		sdi.ajuste_fixo.passo = StrToInteger(campos->Strings[++nc]);
		// esquematico
		sdi.esq.posbar1 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar2 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar3 = 0;
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere SE em PreMonta
	if (!pre_monta->InsereReguladorTapFixo(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereSuporte(TStrings *campos)
{
	// variáveis locais
	int nc;
	int num_campos;
	sdi_SUPORTE sdi;

	// verifica se importação habilitada
	if (!ArranjoEnabled)
		return (true);
	// atualiza  Resumo
	if (plog)
		plog->DefineValor("Eqpto", "Suporte");
	// valida número de dados recebidos
	if (campos->Count < 5)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.tipo = campos->Strings[++nc];
		sdi.num_pto_fixacao = StrToInteger(campos->Strings[++nc]);
		// proteção
		if (sdi.num_pto_fixacao > 24)
			sdi.num_pto_fixacao = 24;
		// determina número de campos em função do número de pontos de fixacao
		num_campos = 4 + (sdi.num_pto_fixacao * 4);
		if (campos->Count < num_campos)
		{
			if (plog)
				plog->Add("Número de campos menor do que o esperado");
			return (false);
		}
		for (int n = 0; n < sdi.num_pto_fixacao; n++)
		{
			sdi.pto_fixacao[n].id = StrToInteger(campos->Strings[++nc]);
			sdi.pto_fixacao[n].cod = campos->Strings[++nc];
			sdi.pto_fixacao[n].x_m = StrToDouble(campos->Strings[++nc]);
			sdi.pto_fixacao[n].y_m = StrToDouble(campos->Strings[++nc]);
		}
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere Suporte
	if (!pre_monta->InsereSuporte(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereSuprimento(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_SUPRIMENTO sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// atualiza  Resumo
	if (plog)
		plog->DefineValor("Eqpto", "Suprimento");
	// valida número de dados recebidos
	if (campos->Count < 9)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê dados
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_bar = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.tipo_sup = StrToInteger(campos->Strings[++nc]);
		sdi.fases = campos->Strings[++nc];
		sdi.vnom_kv = StrToDouble(campos->Strings[++nc]);
		sdi.smax_kva = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.x = StrToDouble(campos->Strings[++nc]);
		sdi.z1_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.z1_pu.x = StrToDouble(campos->Strings[++nc]);
		// lê pontos da Curva
		if (!LePontosCurva(sdi.curva, campos, nc))
		{
			if (plog)
				plog->Add("Pontos inválidos");
			return (false);
		}
		// esquematico
		sdi.esq.posbar = StrToDouble(campos->Strings[++nc]);
		sdi.esq.dxbar = StrToInteger(campos->Strings[++nc]);
		sdi.esq.dybar = StrToInteger(campos->Strings[++nc]);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere Suprimento
	if (!pre_monta->InsereSuprimento(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereTipoChave(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_TIPO_CHAVE sdi;

	// verifica se importação habilitada
	if (!TipoChaveEnabled)
		return (true);
	// atualiza  Resumo
	if (plog)
		plog->DefineValor("Eqpto", "Tipo Chave");
	// valida número de dados recebidos
	if (campos->Count < 4)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.tipo = StrToInteger(campos->Strings[++nc]);
		sdi.codigo = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		// temporário
		sdi.extern_id = "";
		sdi.operacao = campos->Strings[++nc];
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere TipoChave
	if (!pre_monta->InsereTipoChave(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereTipoConsumidor(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_TIPO_CONS sdi;

	// verifica se importação habilitada
	if (!TipoConsumidorEnabled)
		return (true);
	// atualiza  Resumo
	if (plog)
		plog->DefineValor("Eqpto", "Tipo Consumidor");
	// valida número de dados recebidos
	if (campos->Count < 6)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.tag = StrToInteger(campos->Strings[++nc]);
		sdi.codigo = campos->Strings[++nc];
		// lê informação de cor
		sdi.color_r = StrToInteger(campos->Strings[++nc]);
		sdi.color_g = StrToInteger(campos->Strings[++nc]);
		sdi.color_b = StrToInteger(campos->Strings[++nc]);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere TrechoBT
	if (!pre_monta->InsereTipoConsumidor(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereTipoRede(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_TIPO_REDE sdi;

	// verifica se importação habilitada
	if (!TipoRedeEnabled)
		return (true);
	// atualiza  Resumo
	if (plog)
		plog->DefineValor("Eqpto", "Tipo Rede");
	// valida número de dados recebidos
	if (campos->Count < 6)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.segmento = StrToInteger(campos->Strings[++nc]);
		sdi.codigo = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		// temporário
		sdi.extern_id = "";
		sdi.descricao = campos->Strings[++nc];
		sdi.vmin_kv = StrToDouble(campos->Strings[++nc]);
		sdi.vmax_kv = StrToDouble(campos->Strings[++nc]);
		if (campos->Count >= 9)
		{ // lê informação de cor
			sdi.color_r = StrToInteger(campos->Strings[++nc]);
			sdi.color_g = StrToInteger(campos->Strings[++nc]);
			sdi.color_b = StrToInteger(campos->Strings[++nc]);
		}
		else
		{ // cor defaul: black
			sdi.color_r = 0;
			sdi.color_g = 0;
			sdi.color_b = 0;
		}
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere TrechoBT
	if (!pre_monta->InsereTipoRede(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereTrafo2E(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_TRAFO2E sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// atualiza  Resumo
	if (plog)
		plog->DefineValor("Eqpto", "Trafo2E");
	// valida número de dados recebidos
	if (campos->Count < 24)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê dados
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.snom_kva = StrToDouble(campos->Strings[++nc]);
		sdi.sbase_kva = StrToDouble(campos->Strings[++nc]);
		sdi.perda_fe_w = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.x = StrToDouble(campos->Strings[++nc]);
		sdi.z1_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.z1_pu.x = StrToDouble(campos->Strings[++nc]);
		// primário
		sdi.pri.id_bar = StrToInteger(campos->Strings[++nc]);
		sdi.pri.ligacao = campos->Strings[++nc];
		sdi.pri.vnom_kv = StrToDouble(campos->Strings[++nc]);
		sdi.pri.vtap_kv = StrToDouble(campos->Strings[++nc]);
		sdi.pri.zat_ohm.r = StrToDouble(campos->Strings[++nc]);
		sdi.pri.zat_ohm.x = StrToDouble(campos->Strings[++nc]);
		// secundário
		sdi.sec.id_bar = StrToInteger(campos->Strings[++nc]);
		sdi.sec.ligacao = campos->Strings[++nc];
		sdi.sec.vnom_kv = StrToDouble(campos->Strings[++nc]);
		sdi.sec.vtap_kv = StrToDouble(campos->Strings[++nc]);
		sdi.sec.zat_ohm.r = StrToDouble(campos->Strings[++nc]);
		sdi.sec.zat_ohm.x = StrToDouble(campos->Strings[++nc]);
		// ajuste
		sdi.ajuste.tipo = campos->Strings[++nc];
		sdi.ajuste.v_pu = StrToDouble(campos->Strings[++nc]);
		// esquematico
		sdi.esq.posbar1 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar2 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar3 = 0;
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere TrafoSE
	if (!pre_monta->InsereTrafo2E(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereTrafo3E(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_TRAFO3E sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// atualiza  Resumo
	if (plog)
		plog->DefineValor("Eqpto", "Trafo3E");
	// valida número de dados recebidos
	if (campos->Count < 38)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê dados
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		// primário
		sdi.pri.id_bar = StrToInteger(campos->Strings[++nc]);
		sdi.pri.ligacao = campos->Strings[++nc];
		sdi.pri.snom_kva = StrToDouble(campos->Strings[++nc]);
		sdi.pri.vnom_kv = StrToDouble(campos->Strings[++nc]);
		sdi.pri.vtap_kv = StrToDouble(campos->Strings[++nc]);
		sdi.pri.zat_ohm.r = StrToDouble(campos->Strings[++nc]);
		sdi.pri.zat_ohm.x = StrToDouble(campos->Strings[++nc]);
		// secundário
		sdi.sec.id_bar = StrToInteger(campos->Strings[++nc]);
		sdi.sec.ligacao = campos->Strings[++nc];
		sdi.sec.snom_kva = StrToDouble(campos->Strings[++nc]);
		sdi.sec.vnom_kv = StrToDouble(campos->Strings[++nc]);
		sdi.sec.vtap_kv = StrToDouble(campos->Strings[++nc]);
		sdi.sec.zat_ohm.r = StrToDouble(campos->Strings[++nc]);
		sdi.sec.zat_ohm.x = StrToDouble(campos->Strings[++nc]);
		// terciário
		sdi.ter.id_bar = StrToInteger(campos->Strings[++nc]);
		sdi.ter.ligacao = campos->Strings[++nc];
		sdi.ter.snom_kva = StrToDouble(campos->Strings[++nc]);
		sdi.ter.vnom_kv = StrToDouble(campos->Strings[++nc]);
		sdi.ter.vtap_kv = StrToDouble(campos->Strings[++nc]);
		sdi.ter.zat_ohm.r = StrToDouble(campos->Strings[++nc]);
		sdi.ter.zat_ohm.x = StrToDouble(campos->Strings[++nc]);
		// perda no ferro
		sdi.perda_fe_w = StrToDouble(campos->Strings[++nc]);
		// potência de base p/ valores em pu
		sdi.sbase_kva = StrToDouble(campos->Strings[++nc]);
		// impedância em pu
		sdi.zps0_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.zps0_pu.x = StrToDouble(campos->Strings[++nc]);
		sdi.zps1_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.zps1_pu.x = StrToDouble(campos->Strings[++nc]);
		sdi.zpt0_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.zpt0_pu.x = StrToDouble(campos->Strings[++nc]);
		sdi.zpt1_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.zpt1_pu.x = StrToDouble(campos->Strings[++nc]);
		sdi.zst0_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.zst0_pu.x = StrToDouble(campos->Strings[++nc]);
		sdi.zst1_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.zst1_pu.x = StrToDouble(campos->Strings[++nc]);
		// esquematico
		sdi.esq.posbar1 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar2 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar3 = StrToDouble(campos->Strings[++nc]);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere TrafoSE
	if (!pre_monta->InsereTrafo3E(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereTrafoMono(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_TRAFO_MONO sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// atualiza  Resumo
	if (plog)
		plog->DefineValor("Eqpto", "Trafo Monofásico");
	// valida número de dados recebidos
	if (campos->Count < 23)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê dados
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.snom_kva = StrToDouble(campos->Strings[++nc]);
		sdi.perda_fe_w = StrToDouble(campos->Strings[++nc]);
		sdi.sbase_kva = StrToDouble(campos->Strings[++nc]);
		sdi.z_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.z_pu.x = StrToDouble(campos->Strings[++nc]);
		// primário
		sdi.pri.id_bar = StrToInteger(campos->Strings[++nc]);
		sdi.pri.fases = campos->Strings[++nc];
		sdi.pri.vnom_kv = StrToDouble(campos->Strings[++nc]);
		sdi.pri.vtap_kv = StrToDouble(campos->Strings[++nc]);
		sdi.pri.zat_ohm.r = StrToDouble(campos->Strings[++nc]);
		sdi.pri.zat_ohm.x = StrToDouble(campos->Strings[++nc]);
		// secundário
		sdi.sec.id_bar = StrToInteger(campos->Strings[++nc]);
		sdi.sec.fases = campos->Strings[++nc];
		sdi.sec.vnom_kv = StrToDouble(campos->Strings[++nc]);
		sdi.sec.vtap_kv = StrToDouble(campos->Strings[++nc]);
		sdi.sec.zat_ohm.r = StrToDouble(campos->Strings[++nc]);
		sdi.sec.zat_ohm.x = StrToDouble(campos->Strings[++nc]);
		// ajuste
		sdi.ajuste.tipo = campos->Strings[++nc];
		sdi.ajuste.v_pu = StrToDouble(campos->Strings[++nc]);
		// esquematico
		sdi.esq.posbar1 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar2 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar3 = 0;
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere TrafoSE
	if (!pre_monta->InsereTrafoMono(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereTrafoZZ(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_TRAFOZZ sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// configura Log
	if (plog)
		plog->DefineValor("Eqpto", "Trafo ZigZag");
	// valida número de dados recebidos
	if (campos->Count < 10)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_bar = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.vnom_kv = StrToDouble(campos->Strings[++nc]);
		sdi.snom_kva = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.r = StrToDouble(campos->Strings[++nc]);
		sdi.z0_pu.x = StrToDouble(campos->Strings[++nc]);
		// esquematico
		sdi.esq.posbar = StrToDouble(campos->Strings[++nc]);
		sdi.esq.dxbar = StrToInteger(campos->Strings[++nc]);
		sdi.esq.dybar = StrToInteger(campos->Strings[++nc]);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere TrafoZZ
	if (!pre_monta->InsereTrafoZZ(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereTrechoTipico(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_TRECHO sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// atualiza  Resumo
	if (plog)
		plog->DefineValor("Eqpto", "Trecho típico");
	// valida número de dados recebidos
	if (campos->Count < 8)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_bar1 = StrToInteger(campos->Strings[++nc]);
		sdi.id_bar2 = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.comp_m = StrToDouble(campos->Strings[++nc]);
		// arranjo típico
		sdi.arranjo_tipico.id_arranjo = StrToInteger(campos->Strings[++nc]);
		// esquematico
		sdi.esq.posbar1 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar2 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar3 = 0;
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere TrechoBT
	if (!pre_monta->InsereTrechoComArranjo(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereTrechoZ0Z1(TStrings *campos)
{
	// variáveis locais
	int nc;
	sdi_TRECHO sdi;

	// verifica se importação habilitada
	if (!RedeEnabled)
		return (true);
	// atualiza  Resumo
	if (plog)
		plog->DefineValor("Eqpto", "Trecho Z0Z1");
	// valida número de dados recebidos
	if (campos->Count < 14)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.id_bar1 = StrToInteger(campos->Strings[++nc]);
		sdi.id_bar2 = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.comp_m = StrToDouble(campos->Strings[++nc]);
		// arranjo próprio
		sdi.arranjo_proprio.fases = campos->Strings[++nc];
		sdi.arranjo_proprio.iadm_amp = StrToDouble(campos->Strings[++nc]);
		sdi.arranjo_proprio.z0.r = StrToDouble(campos->Strings[++nc]);
		sdi.arranjo_proprio.z0.x = StrToDouble(campos->Strings[++nc]);
		sdi.arranjo_proprio.c0 = StrToDouble(campos->Strings[++nc]);
		sdi.arranjo_proprio.z1.r = StrToDouble(campos->Strings[++nc]);
		sdi.arranjo_proprio.z1.x = StrToDouble(campos->Strings[++nc]);
		sdi.arranjo_proprio.c1 = StrToDouble(campos->Strings[++nc]);
		// esquematico
		sdi.esq.posbar1 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar2 = StrToDouble(campos->Strings[++nc]);
		sdi.esq.posbar3 = 0;
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	// insere TrechoBT
	if (!pre_monta->InsereTrechoZ0Z1(sdi))
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::InsereVersao(TStrings *campos)
{
	// atualiza  Resumo
	if (plog)
		plog->DefineValor("Eqpto", "Versão");
	// valida número de dados recebidos
	if (campos->Count < 4)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	// valida campos da versão do arquivo
	if (campos->Strings[0].CompareIC("Sinap") != 0)
		return (false);
	if (campos->Strings[1].CompareIC("Interface Texto") != 0)
		return (false);
	// verifica versão 1.0
	if (campos->Strings[2].CompareIC("Rev1.0") == 0)
	{
		versao = 1;
		return (true);
	}
	// verifica versão 2
	else if (campos->Strings[2].CompareIC("Rev2") == 0)
	{
		versao = 2;
		return (true);
	}
	// verifica versão 3
	else if (campos->Strings[2].CompareIC("Rev3") == 0)
	{
		versao = 3;
		return (true);
	}
	// verifica versão 4
	else if (campos->Strings[2].CompareIC("Rev4") == 0)
	{
		versao = 4;
		return (true);
	}
	// verifica versão 5
	else if (campos->Strings[2].CompareIC("Rev5") == 0)
	{
		versao = 5;
		return (true);
	}
	// versão inválida
	if (plog)
		plog->Add("Arquivo com versão inválida");
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::LeCurva(sdi_CURVA &sdi, TStrings *campos, int &nc)
{
	if (versao >= 2)
		return (LeCurva_Versao2(sdi, campos, nc));
	else
		return (LeCurva_Versao1(sdi, campos, nc));
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::LeCurva_Versao1(sdi_CURVA &sdi, TStrings *campos, int &nc)
{
	// variáveia locais
	bool sucesso;

	try
	{ // lê número de pontos e número de valores
		sdi.id = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.tipica = campos->Strings[++nc];
		sdi.dia = SEMANA_COMPLETA;
		sdi.tipo = StrToInteger(campos->Strings[++nc]);
		sdi.tipo_str = campos->Strings[++nc];
		sdi.unidade = StrToInteger(campos->Strings[++nc]);
		sdi.classe_cons = campos->Strings[++nc];
		sdi.consumo_min_kwhmes = StrToDouble(campos->Strings[++nc]);
		sdi.consumo_max_kwhmes = StrToDouble(campos->Strings[++nc]);
		// lê pontos da Curva
		sucesso = LePontosCurva(sdi, campos, nc);
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::LeCurva_Versao2(sdi_CURVA &sdi, TStrings *campos, int &nc)
{
	// variáveia locais
	bool sucesso;

	try
	{ // lê número de pontos e número de valores
		sdi.id = StrToInteger(campos->Strings[++nc]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.tipica = campos->Strings[++nc];
		sdi.dia = StrToInteger(campos->Strings[++nc]);
		sdi.tipo = StrToInteger(campos->Strings[++nc]);
		sdi.tipo_str = campos->Strings[++nc];
		sdi.unidade = StrToInteger(campos->Strings[++nc]);
		sdi.classe_cons = campos->Strings[++nc];
		sdi.consumo_min_kwhmes = StrToDouble(campos->Strings[++nc]);
		sdi.consumo_max_kwhmes = StrToDouble(campos->Strings[++nc]);
		// lê pontos da Curva
		sucesso = LePontosCurva(sdi, campos, nc);
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::LeCurvaTipica(TStrings *campos, sdi_CURVA &sdi)
{
	if (versao >= 2)
		return (LeCurvaTipica_Versao2(campos, sdi));
	else
		return (LeCurvaTipica_Versao1(campos, sdi));
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::LeCurvaTipica_Versao1(TStrings *campos, sdi_CURVA &sdi)
{
	// variáveis locais
	int nc;

	// valida número de dados recebidos
	if (campos->Count < 9)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.dia = SEMANA_COMPLETA;
		sdi.tipica = "tipica";
		sdi.tipo = StrToInteger(campos->Strings[++nc]);
		sdi.tipo_str = campos->Strings[++nc];
		sdi.unidade = StrToInteger(campos->Strings[++nc]);
		sdi.classe_cons = campos->Strings[++nc];
		sdi.consumo_min_kwhmes = StrToDouble(campos->Strings[++nc]);
		sdi.consumo_max_kwhmes = StrToDouble(campos->Strings[++nc]);
		// lê pontos da Curva
		if (!LePontosCurva(sdi, campos, nc))
		{
			if (plog)
				plog->Add("Pontos inválidos");
			return (false);
		}
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::LeCurvaTipica_Versao2(TStrings *campos, sdi_CURVA &sdi)
{
	// variáveis locais
	int nc;

	// valida número de dados recebidos
	if (campos->Count < 11)
	{
		if (plog)
			plog->Add("Número de campos menor do que o esperado");
		return (false);
	}
	try
	{ // lê campos
		sdi.id = StrToInteger(campos->Strings[nc = 0]);
		sdi.cod = campos->Strings[++nc];
		sdi.extern_id = campos->Strings[++nc];
		sdi.tipica = campos->Strings[++nc];
		sdi.dia = StrToInteger(campos->Strings[++nc]);
		sdi.tipo = StrToInteger(campos->Strings[++nc]);
		sdi.tipo_str = campos->Strings[++nc];
		sdi.unidade = StrToInteger(campos->Strings[++nc]);
		sdi.classe_cons = campos->Strings[++nc];
		sdi.consumo_min_kwhmes = StrToDouble(campos->Strings[++nc]);
		sdi.consumo_max_kwhmes = StrToDouble(campos->Strings[++nc]);
		// lê pontos da Curva
		if (!LePontosCurva(sdi, campos, nc))
		{
			if (plog)
				plog->Add("Pontos inválidos");
			return (false);
		}
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na leitura/conversão do campo " + IntToStr(nc + 1));
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TExtrator::LePontosCurva(sdi_CURVA &sdi, TStrings *campos, int &nc)
{
	try
	{ // lê número de pontos e número de valores
		sdi.num_ponto = StrToInteger(campos->Strings[++nc]);
		sdi.dim_valor = StrToInteger(campos->Strings[++nc]);
		for (int ind_ponto = 0; ind_ponto < sdi.num_ponto; ind_ponto++)
		{ // hora/minuto inicial
			sdi.ponto[ind_ponto].hm_ini.hora = StrToInteger(campos->Strings[++nc]);
			sdi.ponto[ind_ponto].hm_ini.minuto = StrToInteger(campos->Strings[++nc]);
			// hora/minuto final
			sdi.ponto[ind_ponto].hm_fim.hora = StrToInteger(campos->Strings[++nc]);
			sdi.ponto[ind_ponto].hm_fim.minuto = StrToInteger(campos->Strings[++nc]);
			// valores dos pontos
			for (int ind_valor = 0; ind_valor < sdi.dim_valor; ind_valor++)
			{
				sdi.ponto[ind_ponto].valor[ind_valor] = StrToDouble(campos->Strings[++nc]);
			}
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TExtrator::Sucesso(void)
{
	return (sucesso);
}

// -----------------------------------------------------------------------------
bool __fastcall TExtrator::TrataFimDados(void)
{
	// nada a fazer: sinaliza PreMonta
	if (!pre_monta->TrataFimDados())
		return (false);
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TExtrator::TrataIniciaDados(AnsiString cod_dominio)
{
	// inicia flag que indica sucesso/falha na montagem da rede
	sucesso = true;
	// inicia ponteiros p/ objetos
	plog = (VTLog*)apl->GetObject(__classid(VTLog));
	pre_monta = (VTPreMonta*)apl->GetObject(__classid(VTPreMonta));
	// reinicia PreMonta
	pre_monta->TrataIniciaDados(cod_dominio);

	return (true);
}
/*
 //---------------------------------------------------------------------------
 bool __fastcall TExtrator::Valida(int valor, int vet_valor[], int vet_dim)
 {
 for (int n = 0; n < vet_dim; n++)
 {
 if (valor == vet_valor[n]) return(true);
 }
 return(false);
 }

 //---------------------------------------------------------------------------
 bool __fastcall TExtrator::Valida(AnsiString valor, char *vet_valor[], int vet_dim)
 {
 for (int n = 0; n < vet_dim; n++)
 {
 if (valor.AnsiCompareIC(vet_valor[n]) == 0) return(true);
 }
 return(false);
 }

 //---------------------------------------------------------------------------
 bool __fastcall TExtrator::ValidaDominiosConsumidorBT(sdi_CONSUMIDOR_BT &sdi)
 {
 //variáveis locais
 bool  valido       = true;
 char *vet_fase[]   = {"A",   "B",   "C",
 "AN",  "BN",  "CN",
 "AB",  "BC",  "AC",
 "ABN", "BCN", "ACN",
 "ABC", "ABCN"};
 char *vet_classe[] = {"1", "2", "3", "4", "5"};

 //valida domínio de fases
 if (! Valida(sdi.fases, vet_fase, sizeof(vet_fase)/sizeof(char*)))
 {
 if (plog) plog->Add("Fase inválida: " + sdi.fases);
 //valido = false;
 }
 //valida classe de consumidor
 if (! Valida(sdi.classe, vet_classe, sizeof(vet_classe)/sizeof(char*)))
 {
 if (plog) plog->Add("Classe inválida: " + sdi.classe);
 //valido = false;
 }
 //valida consumo medido do consumidor
 if (IsDoubleZero(sdi.consumo_med))
 {
 if (plog) plog->Add("Consumo de energia nulo");
 //valido = false;
 }
 return(valido);
 }

 //---------------------------------------------------------------------------
 bool __fastcall TExtrator::ValidaDominiosConsumidorIP(sdi_CONSUMIDOR_IP &sdi)
 {
 //variáveis locais
 bool  valido      = true;
 char *vet_fase[]   = {"A",   "B",   "C",
 "AN",  "BN",  "CN"};
 int    vet_pot[]  = {40,   60,  70,  80, 100, 125, 150, 160,
 200, 210, 215, 250, 300, 350, 360, 400};

 //valida domínio de fases
 if (! Valida(sdi.fases, vet_fase, sizeof(vet_fase)/sizeof(char*)))
 {
 if (plog) plog->Add("Fase inválida: " + sdi.fases);
 //valido = false;
 }
 //valida domínio de potência
 if (! Valida(sdi.pot, vet_pot, sizeof(vet_pot)/sizeof(int)))
 {
 if (plog) plog->Add("Potência inválida: " + DoubleToStr("%2.1f", sdi.pot));
 //valido = false;
 }
 //valida número de lâmpadas
 if (sdi.num == 0)
 {
 if (plog) plog->Add("Número de lâmpadas: 0");
 //valido = false;
 }
 return(valido);
 }

 //---------------------------------------------------------------------------
 bool __fastcall TExtrator::ValidaDominiosChave(sdi_CHAVE &sdi)
 {
 //variáveis locais
 bool  valido     = true;
 char *vet_tipo[] = {"DISJ", "FALB", "FUSI", "FULB", "REPE", "RELI"};

 //valida domínio de tipo
 if (! Valida(sdi.tipo, vet_tipo, sizeof(vet_tipo)/sizeof(char*)))
 {
 if (plog) plog->Add("Tipo inválido: " + sdi.tipo);
 //valido = false;
 }
 //valida domínio de estado
 if ((sdi.estado != 0)&&(sdi.estado != 1))
 {
 if (plog) plog->Add("Estado inválido: " + StrToInt(sdi.estado));
 //valido = false;
 }
 return(valido);
 }

 //---------------------------------------------------------------------------
 bool __fastcall TExtrator::ValidaDominiosET(sdi_INSTAL_TRAFO &sdi)
 {
 //variáveis locais
 bool  valido     = true;
 char *vet_tipo[] = {"ET",  "EP"};

 //valida domínio de tipo
 if (! Valida(sdi.tipo, vet_tipo, sizeof(vet_tipo)/sizeof(char*)))
 {
 if (plog) plog->Add("Tipo inválido: " + sdi.tipo);
 //valido = false;
 }
 return(valido);
 }


 //-----------------------------------------------------------------------------
 bool __fastcall TExtrator::ValidaDominiosTrechoBT(sdi_TRECHO_BT &sdi)
 {
 bool  valido     = true;
 char *vet_fase[] = {"AN",  "BN",  "CN",
 "ABN", "BCN", "ACN",
 "ABCN"};

 //valida domínio de fases
 if (! Valida(sdi.fases, vet_fase, sizeof(vet_fase)/sizeof(char*)))
 {
 if (plog) plog->Add("Fase inválida: " + sdi.fases);
 //valido = false;
 }
 //valida comprimento
 if (IsDoubleZero(sdi.comp))
 {
 if (plog) plog->Add("Comprimento nulo");
 //valido = false;
 }
 return(valido);
 }

 //-----------------------------------------------------------------------------
 bool __fastcall TExtrator::ValidaDominiosTrechoMT(sdi_TRECHO_MT &sdi)
 {
 bool  valido     = true;
 char *vet_fase[] = {"D",   "E",   "F",
 "DE",  "EF",  "DF",
 "DEF"};

 //valida domínio de fases
 if (! Valida(sdi.fases, vet_fase, sizeof(vet_fase)/sizeof(char*)))
 {
 if (plog) plog->Add("Fase inválida: " + sdi.fases);
 //valido = false;
 }
 //valida comprimento
 if (IsDoubleZero(sdi.comp))
 {
 if (plog) plog->Add("Comprimento nulo");
 //valido = false;
 }
 return(valido);
 }
 */

// -----------------------------------------------------------------------------
// eof
