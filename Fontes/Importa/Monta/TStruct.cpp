// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TStruct.h"
#include <Fontes\Arranjo\VTSuporte.h>
#include <Fontes\Classes\VTClasse.h>
#include <Fontes\Constante\Fases.h>
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Rede\VTCanal.h>
#include <Fontes\Rede\VTChave.h>
#include <Fontes\Rede\VTEqpto.h>
#include <Fontes\Rede\VTGerador.h>
#include <Fontes\Rede\VTRegulador.h>
#include <Fontes\Rede\VTTipoChave.h>
#include <Fontes\Rede\VTTrafo.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall strPATAMAR::strPATAMAR(void)
{
	hm_ini.hora = 0;
	hm_ini.minuto = 0;
	hm_fim.hora = 24;
	hm_fim.minuto = 0;
	for (int n = 0; n < SIZEOF_VALOR; n++)
	{
		valor[n] = 0;
	}
}

// ---------------------------------------------------------------------------
__fastcall strPATAMAR::~strPATAMAR(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strCURVA::strCURVA(void)
{
	id = ID_NULO;
	codigo = "";
	tipica = false;
	status = sttNOVO;
	num_pat = 0;
	num_valor = 0;
	// patamar[] é iniciado pelo construtor da estrutura
}

// ---------------------------------------------------------------------------
__fastcall strCURVA::~strCURVA(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strCURVA_TIPICA::strCURVA_TIPICA(void) : strCURVA()
{

	dia = SEMANA_COMPLETA;
	tipica = true;
	tipo = -1;
	unidade = -1;
	classe_tag = 0;
	energia_min_kwhmes = 0;
	energia_max_kwhmes = 0;
}

// ---------------------------------------------------------------------------
__fastcall strCURVA_TIPICA::~strCURVA_TIPICA(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strCOMENTARIO::strCOMENTARIO(void)
{
	dx = 50;
	dy = 0;
	comentario = "";
}

// ---------------------------------------------------------------------------
__fastcall strCOMENTARIO::~strCOMENTARIO(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strARRANJO1::strARRANJO1(void)
{
	tipico = false;
	data = Now();
	codigo = "";
	extern_id = "";
	tipo_cfg = ID_NULO;
	fases = faseINV;
	ramal = false;
	//
	cfg_caboZ0Z1.cod_cabo = "";
	//
	cfg_suporte_cabo.estgeo_id = "";
	cfg_suporte_cabo.cod_caboA = "";
	cfg_suporte_cabo.cod_caboB = "";
	cfg_suporte_cabo.cod_caboC = "";
	cfg_suporte_cabo.cod_caboN = "";
	//
	cfg_arranjoZ0Z1.iadm_amp = 0.;
	cfg_arranjoZ0Z1.z0.r = 0.;
	cfg_arranjoZ0Z1.z0.x = 0.;
	cfg_arranjoZ0Z1.c0 = 0.;
	cfg_arranjoZ0Z1.z1.r = 0.;
	cfg_arranjoZ0Z1.z1.x = 0.;
	cfg_arranjoZ0Z1.c1 = 0.;
	//
	cfg_pu.iadm_amp = 0.;
	cfg_pu.sbase_mva = 0.;
	cfg_pu.vbase_kv = 0.;
	cfg_pu.z0_pu.r = 0.;
	cfg_pu.z0_pu.x = 0.;
	cfg_pu.yc0_pu = 0.;
	cfg_pu.z1_pu.r = 0.;
	cfg_pu.z1_pu.x = 0.;
	cfg_pu.yc1_pu = 0.;
}

// ---------------------------------------------------------------------------
__fastcall strARRANJO1::~strARRANJO1(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strARRANJO2::strARRANJO2(void)
{
	tipico = false;
	data = Now();
	arranjo_id = ID_NULO;
	tipo_cfg = ID_NULO;
	fases = faseINV;
	ramal = false;
	codigo = "";
	extern_id = "";
	res_terra = 0.;
	vmin_kv = 0.;
	vmax_kv = 0.;
	color = clWhite;
	status = sttNOVO;
	//
	cfg_suporte_cabo.suporte_id = ID_NULO;
	for (int index = 0; index < MAX_FASE; index++)
	{
		cfg_suporte_cabo.fase[index].cabo_id = ID_NULO;
		cfg_suporte_cabo.fase[index].pto_fixacao_id = -1;
		cfg_suporte_cabo.fase[index].multiplicidade = 1;
	}
	//
	cfg_caboZOZ1.cabo_id = ID_NULO;
	cfg_caboZOZ1.suporte_id = ID_NULO;
	//
	cfg_arranjoZ0Z1.iadm_amp = 0.;
	cfg_arranjoZ0Z1.z0.r = 0.;
	cfg_arranjoZ0Z1.z0.x = 0.;
	cfg_arranjoZ0Z1.c0 = 0.;
	cfg_arranjoZ0Z1.z1.r = 0.;
	cfg_arranjoZ0Z1.z1.x = 0.;
	cfg_arranjoZ0Z1.c1 = 0.;
	cfg_arranjoZ0Z1.suporte_id = ID_NULO;
	//
	cfg_matZ.iadm_amp = 0.;
	for (int i = 0; i < MAX_FASE; i++)
	{
		for (int j = 0; j < MAX_FASE; j++)
		{
			cfg_matZ.mat_z[i][j].r = 0.;
			cfg_matZ.mat_z[i][j].x = 0.;
			cfg_matZ.mat_c[i][j] = 0.;
		}
	}
	//
	cfg_puSB100.iadm_amp = 0.;
	cfg_puSB100.z0.r = 0.;
	cfg_puSB100.z0.x = 0.;
	cfg_puSB100.c0 = 0.;
	cfg_puSB100.z1.r = 0.;
	cfg_puSB100.z1.x = 0.;
	cfg_puSB100.c1 = 0.;
	cfg_puSB100.suporte_id = ID_NULO;
}

// ---------------------------------------------------------------------------
__fastcall strARRANJO2::~strARRANJO2(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strBARRA::strBARRA(void)
{
	obra = false;
	bar_id = ID_NULO;
	codigo = "";
	extern_id = "";
	show = eqp_VISIBLE;
	vnom = 0.;
	utm.x = 0;
	utm.y = 0;
	esq.x1 = 0;
	esq.y1 = 0;
	esq.x2 = 0;
	esq.y2 = 0;
	// index     = 0;
	// comentario é iniciado pelo construtor da estrutura
}

// ---------------------------------------------------------------------------
__fastcall strBARRA::~strBARRA(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strBATERIA::strBATERIA(void)
{
	obra = false;
	show = eqp_VISIBLE;
	status = sttNOVO;
	bat_id = 0;
	bar_id = 0;
	codigo = "";
	extern_id = "";
	fases = faseINV;
	tipo_operacao = 0;
	modelo_carga = 0;
	modelo_desc = 0;
	p_nom_kw = 0.;
	e_nom_kwh = 0.;
	cap_inv_kva = 0.;
	p_ociosa_pu = 0.;
	q_ociosa_pu = 0.;
	rend_carga_pu = 0.;
	rend_desc_pu = 0.;
	gatilho_carga_pu = 0.;
    gatilho_desc_pu = 0.;
	// comentario é iniciado pelo construtor da estrutura
}

// ---------------------------------------------------------------------------
__fastcall strBATERIA::~strBATERIA(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strCABO::strCABO(void)
{
	id = ID_NULO;
	codigo = "";
	extern_id = "";
	status = sttNOVO;
	tipo_cfg = ID_NULO;
	iadm = 0.;
	vmin = 0.;
	vmax = 0.;
	r = 0.;
	raio = 0.;
	gmr = 0.;
	z0.r = 0.;
	z0.x = 0.;
	z1.r = 0.;
	z1.x = 0.;
	c0 = 0.;
	c1 = 0.;
}

// ---------------------------------------------------------------------------
__fastcall strCABO::~strCABO(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strCAPACITOR::strCAPACITOR(void)
{
	obra = false;
	cap_id = ID_NULO;
	bar_id = ID_NULO;
	codigo = "";
	extern_id = "";
	show = eqp_VISIBLE | cap_Q;
	status = sttNOVO | sttENABLED;
	ligacao = lgTRIANG;
	vnom = 0.;
	q_mvar = 0.;
	curva.id = ID_NULO;
	curva.tipica = true;
	esq.posbar = 50.;
	esq.dxbar = 0.;
	esq.dybar = 0.;
	// curva é iniciado pelo seu construtor
	// comentario é iniciado pelo seu construtor
}

// ---------------------------------------------------------------------------
__fastcall strCAPACITOR::~strCAPACITOR(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strCAPSERIE::strCAPSERIE(void)
{
	obra = false;
	id = ID_NULO;
	rede_id = ID_NULO;
	bar1_id = ID_NULO;
	bar2_id = ID_NULO;
	codigo = "";
	extern_id = "";
	show = eqp_VISIBLE | eqp_FIXO | lig_P | lig_Q;
	vnom = 0.;
	snom = 0.;
	z.r = 0.;
	z.x = 0.;
	esq.posbar1 = 50.;
	esq.posbar2 = 50.;
	esq.posbar3 = 50.;
	// comentario é iniciado pelo seu construtor
}

// ---------------------------------------------------------------------------
__fastcall strCAPSERIE::~strCAPSERIE(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strCARGA::strCARGA(void)
{
	obra = false;
	car_id = ID_NULO;
	bar_id = ID_NULO;
	codigo = "";
	extern_id = "";
	show = eqp_VISIBLE | car_P | car_Q;
	status = sttNOVO;
	vip = false;
	grupoA = false;
	fases = faseINV;
	classe_cons = consINDEFINIDO;
	curva_tipica = NULL;
	energia_mwhmes = 0.;
	//
	modelo.icte_pu = 1.0;
	modelo.scte_pu = 0.0;
	modelo.zcte_pu = 0.0;
	//
	num_cons.residencial = 0;
	num_cons.comercial = 0;
	num_cons.industrial = 0;
	num_cons.rural = 0;
	num_cons.outros = 0;
	num_cons.a4 = 0;
	// curva é iniciado pelo seu construtor;
	ip.num_lamp = 0;
	ip.pot_kw = 0.;
	ip.fat_pot = 0.;
	//
	esq.posbar = 50.;
	esq.dxbar = 0;
	esq.dybar = 0;
	// ponteiros opcionais para objetos
	obj.barra = NULL;
}

// ---------------------------------------------------------------------------
__fastcall strCARGA::~strCARGA(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strCARGANL::strCARGANL(void)
{
	obra = false;
	car_id = ID_NULO;
	bar_id = ID_NULO;
	codigo = "";
	extern_id = "";
	show = eqp_VISIBLE;
	ordem = -1;
	fat_pot = 0.;
	flag_indutivo = true;
	flag_kVA = true;
	snomA = 0.;
	snomB = 0.;
	snomC = 0.;
	angA = 0.;
	angB = 0.;
	angC = 0.;
	//
	esq.posbar = 50.;
	esq.dxbar = 0;
	esq.dybar = 0;
	// comentario é iniciado pelo seu construtor;
}

// ---------------------------------------------------------------------------
__fastcall strCARGANL::~strCARGANL(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strCHAVE::strCHAVE(void)
{
	obra = false; ;
	chv_id = ID_NULO;
	rede_id = ID_NULO;
	bar1_id = ID_NULO;
	bar2_id = ID_NULO;
	codigo = "";
	extern_id = "";
	elo_fusivel = "";
	tipo_gis = "";
	show = eqp_VISIBLE | lig_I_MAX;
	corn = 0.;
	estado = chvFECHADA;
	tecno_comunicacao = comNAO_DISPONIVEL;
	telecomandada = false;
	protecao_ativa = false;
	tipochave.id = ID_NULO;
	tipochave.tipo = ID_NULO;
	tipochave.codigo = "";
	//
	esq.posbar1 = 50.;
	esq.posbar2 = 50.;
	esq.posbar3 = 0.;
	// comentario é iniciado pelo seu construtor;
}

// ---------------------------------------------------------------------------
__fastcall strCHAVE::~strCHAVE(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strCLUSTER::strCLUSTER(void)
{
	obra = false;
	cluster_id = ID_NULO;
	codigo = "";
}

// ---------------------------------------------------------------------------
__fastcall strCLUSTER::~strCLUSTER(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strDOMINIO::strDOMINIO(void)
{
	obra = false;
	dominio_id = ID_NULO;
	data_criacao = Now();
	codigo = "";
}

// ---------------------------------------------------------------------------
__fastcall strDOMINIO::~strDOMINIO(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strFILTRO::strFILTRO(void)
{
	obra = false;
	id = ID_NULO;
	bar_id = ID_NULO;
	codigo = "";
	extern_id = "";
	show = eqp_VISIBLE | eqp_FIXO;
	vnom = 0.;
	snom = 0.;
	z0.r = 0.;
	z0.x = 0.;
	//
	esq.posbar = 50.;
	esq.dxbar = 0;
	esq.dybar = 0;
	// comentario é iniciado pelo seu construtor;
}

// ---------------------------------------------------------------------------
__fastcall strFILTRO::~strFILTRO(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strGERADOR::strGERADOR(void)
{
	obra = false;
	ger_id = ID_NULO;
	bar_id = ID_NULO;
	codigo = "";
	extern_id = "";
	show = eqp_VISIBLE | eqp_FIXO | eqp_CODIGO | ger_TIPO;
	tiposup = supPV;
	tipo_geracao = gerPCH;
	ligacao = lgEST_AT;
   fases   = faseABCN;
	vnom = 0.;
	snom_mva = 0.;
	smax_mva = 0.;
	fplim = 0.8;
	z0.r = 0.;
	z0.x = 0.;
	z1.r = 0.;
	z1.x = 0.;
	// curva é iniciado pelo seu construtor;
	esq.posbar = 50.;
	esq.dxbar = 0;
	esq.dybar = 0;
	// comentario é iniciado pelo seu construtor;
}

// ---------------------------------------------------------------------------
__fastcall strGERADOR::~strGERADOR(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strCANAL::strCANAL(void)
{
	obra = false;
	canal_id = ID_NULO;
	medidor_id = ID_NULO;
	codigo = "";
	tipo_canal = canalV;
	tolerancia_perc = 1.0;
	//
	eqpto_medido.tipo = -1;
	eqpto_medido.id = ID_NULO;
	eqpto_medido.fases = faseINV;
	// curva é iniciado pelo seu construtor;
}

// ---------------------------------------------------------------------------
__fastcall strCANAL::~strCANAL(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strMEDIDOR::strMEDIDOR(void)
{
	obra = false;
	medidor_id = ID_NULO;
	bar_id = ID_NULO;
	codigo = "";
	extern_id = "";
	show = eqp_VISIBLE;
	//
	esq.posbar = 50.;
	esq.dxbar = 0;
	esq.dybar = 0;
	// comentario é iniciado pelo seu construtor;
}

// ---------------------------------------------------------------------------
__fastcall strMEDIDOR::~strMEDIDOR(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strMUTUA::strMUTUA(void)
{
	obra = false;
	id = ID_NULO;
	rede_id = ID_NULO;
	tre1_id = ID_NULO;
	tre2_id = ID_NULO;
	tre1_bar_id = ID_NULO;
	tre2_bar_id = ID_NULO;
	codigo = "";
	extern_id = "";
	show = eqp_VISIBLE | eqp_FIXO;
	z0.r = 0.;
	z0.x = 0.;
	z1.r = 0.;
	z1.x = 0.;
	postre1 = 50.;
	postre2 = 50.;
	// comentario é iniciado pelo seu construtor;
}

// ---------------------------------------------------------------------------
__fastcall strMUTUA::~strMUTUA(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strPLAN_ACAO::strPLAN_ACAO(void)
{
	tipo_acao = ID_NULO;
	obra_id = ID_NULO;
	alternativa_id = ID_NULO;
	estudo_id = ID_NULO;
	tipo_eqpto = ID_NULO;
	eqpto_id = ID_NULO;
    item_id = ID_NULO;
	eqpto_antes_id = ID_NULO;
	eqpto_depos_id = ID_NULO;
}

// ---------------------------------------------------------------------------
__fastcall strPLAN_ACAO::~strPLAN_ACAO(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strPLAN_ALTERNATIVA::strPLAN_ALTERNATIVA(void)
{
	alternativa_id = ID_NULO;
	estudo_id = ID_NULO;
	codigo = "";
    justificativa = "";
}

// ---------------------------------------------------------------------------
__fastcall strPLAN_ALTERNATIVA::~strPLAN_ALTERNATIVA(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strPLAN_ESTUDO::strPLAN_ESTUDO(void)
{
	estudo_id = ID_NULO;
	tipo = ID_NULO;
	codigo = "";
	autor = "";
	data = Now();
	justificativa = "";
}

// ---------------------------------------------------------------------------
__fastcall strPLAN_ESTUDO::~strPLAN_ESTUDO(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strPLAN_IPE::strPLAN_IPE(void)
{
	estudo_id = ID_NULO;
	alternativa_id = ID_NULO;
	obra_id = ID_NULO;
	atributo_tipo = ID_NULO;
	atributo_id = ID_NULO;
    item_id = ID_NULO;
}

// ---------------------------------------------------------------------------
__fastcall strPLAN_IPE::~strPLAN_IPE(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strPLAN_OBRA::strPLAN_OBRA(void)
{
	obra_id = ID_NULO;
	alternativa_id = ID_NULO;
	estudo_id = ID_NULO;
	tipo = ID_NULO;
	ipe_atributo_id = ID_NULO;
	codigo = "";
	data_inicio = Now();
	data_ativacao = Now();
	responsavel = "";
	ob_especial = 0.;
	custo = 0.;
	altera_taxa_falha = false;
	taxa_falha_ins = 0.;
	taxa_falha_sub = 0.;
	bloqueada = false;
    finalizada = false;
}

// ---------------------------------------------------------------------------
__fastcall strPLAN_OBRA::~strPLAN_OBRA(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strREATOR::strREATOR(void)
{
	obra = false;
	id = ID_NULO;
	bar_id = ID_NULO;
	codigo = "";
	extern_id = "";
	show = eqp_VISIBLE | eqp_FIXO;
	ligacao = lgTRIANG;
	vnom = 0.;
	q_mvar = 0.;
	//
	esq.posbar = 50.;
	esq.dxbar = 0;
	esq.dybar = 0;
	// curva é iniciado pelo seu construtor;
	// comentario é iniciado pelo seu construtor;
}

// ---------------------------------------------------------------------------
__fastcall strREATOR::~strREATOR(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16666485
//https://sinapsisenergia.teamwork.com/#/tasks/18761472
__fastcall strREDE::strREDE(void)
{
	obra = false;
	rede_id = ID_NULO;
	cluster_id = ID_NULO;
	dominio_id = ID_NULO;
	barini_id = ID_NULO;
	mrede_id = ID_NULO;
	mrede_redutivel = true;
	codigo = "";
	extern_id = "";
	show = eqp_VISIBLE;
	estatica = false;
	radial = false;
	equivalente = false;
	color = clWhite;
	//
	tiporede.id = ID_NULO;
	tiporede.segmento = ID_NULO;
	tiporede.codigo = "";
	//
	area_esq.x1 = -1;
	area_esq.y1 = -1;
	area_esq.x2 = -1;
	area_esq.y2 = -1;
	// comentario é iniciado pelo seu construtor;
	//
	dados_ext.mae_filha_conectada = false;
	dados_ext.id_alim_pai = ID_NULO;
    dados_ext.cod_chave = "";
    dados_ext.sigla = "";
    dados_ext.regional = "";
    // curva é iniciado pelo seu construtor
}

// ---------------------------------------------------------------------------
__fastcall strREDE::~strREDE(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strREGULADOR::strREGULADOR(void)
{
	obra = false;
	id = ID_NULO;
	rede_id = ID_NULO;
	bar1_id = ID_NULO;
	bar2_id = ID_NULO;
	modelo = reguPADRAO;
	codigo = "";
	extern_id = "";
	show = eqp_VISIBLE | lig_P | lig_Q;
	by_pass = false;
	ligacao = lgEST_ISO;
	fases = faseABC;
	snom_mva = 0.;
	var_tensao = 10.;
	num_passo = 16;
	z0.r = 0.;
	z0.x = 0.;
	z1.r = 0.;
	z1.x = 0.;
	//ajuste_auto = false;
	//
	//sentido.reversivel = true;
	sentido.bar_ref_id = ID_NULO;
	//
	cfg_fixo.passo = 0;
	//
	cfg_auto.bar_ref_id = ID_NULO;
	cfg_auto.bar_ref_inv_id = ID_NULO;
	cfg_auto.v_pu = 1.0;
	cfg_auto.vinv_pu = 1.0;
	cfg_auto.larg_bandamorta_pu = 0.;
	cfg_auto.larg_bandamorta_inv_pu = 0.;
	//
	rel_tap = 115;
	//
	esq.posbar1 = 50.;
	esq.posbar2 = 50.;
	esq.posbar3 = 0.;
	// comentario é iniciado pelo seu construtor;
}

// ---------------------------------------------------------------------------
__fastcall strREGULADOR::~strREGULADOR(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strSUPORTE::strSUPORTE(void)
{
	id = ID_NULO;
	codigo = "";
	extern_id = "";
	status = sttNOVO;
	tipo = suporteAEREO;
	vmin = 0.;
	vmax = 0.;
	num_pto = 0;
	//
	for (int index = 0; index < 20; index++)
	{
		pto_fixacao[index].id = ID_NULO;
		pto_fixacao[index].codigo = "";
		pto_fixacao[index].coord_x = 0.;
		pto_fixacao[index].coord_y = 0;
	}
}

// ---------------------------------------------------------------------------
__fastcall strSUPORTE::~strSUPORTE(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strSUPRIMENTO::strSUPRIMENTO(void)
{
	obra = false;
	sup_id = ID_NULO;
	bar_id = ID_NULO;
	codigo = "";
	extern_id = "";
	show = eqp_VISIBLE | eqp_FIXO | sup_TIPO;
	tiposup = supVF;
	smax = 100.;
	vnom_kv = 0.;
	zeq0.r = 0.;
	zeq0.x = 0.;
	zeq1.r = 0.;
	zeq1.x = 0.;
	pcc_3f.p = 0.;
	pcc_3f.q = 0.;
	pcc_ft.p = 0.;
	pcc_ft.q = 0.;
	//
	esq.posbar = 50.;
	esq.dxbar = 0;
	esq.dybar = 0;
	// curva é iniciado pelo seu construtor;
	// comentario é iniciado pelo seu construtor;
}

// ---------------------------------------------------------------------------
__fastcall strSUPRIMENTO::~strSUPRIMENTO(void)
{ // nada a fazer
}

/*
 //---------------------------------------------------------------------------
 __fastcall strTIPO_EQPTO::strTIPO_EQPTO(void)
 {
 id        = -1;
 tipo      = -1;;
 codigo    = "";
 descricao = "";
 auxiliar  = -1;
 status    = 0;
 }

 //---------------------------------------------------------------------------
 __fastcall strTIPO_EQPTO::~strTIPO_EQPTO(void)
 {//nada a fazer
 }
 */
// ---------------------------------------------------------------------------
__fastcall strTIPO_CHAVE::strTIPO_CHAVE(void)
{
	id = ID_NULO;
	tipo = ID_NULO;
	codigo = "";
	modo_operacao = chaveOP_INDEFINIDA;
	status = sttNOVO;
}

// ---------------------------------------------------------------------------
__fastcall strTIPO_CHAVE::~strTIPO_CHAVE(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strTIPO_REDE::strTIPO_REDE(void)
{
	id = ID_NULO;
	segmento = ID_NULO;
	codigo = "";
	descricao = "";
	vmin_kv = 0.;
	vmax_kv = 0.;
	qtde_rede = 0;
	color = clWhite;
	status = sttNOVO;
}

// ---------------------------------------------------------------------------
__fastcall strTIPO_REDE::~strTIPO_REDE(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strTRAFO::strTRAFO(void)
{
	obra = false;
	trf_id = ID_NULO;
	rede_id = ID_NULO;
	bar1_id = ID_NULO;
	bar2_id = ID_NULO;
	codigo = "";
	extern_id = "";
	show = eqp_VISIBLE | lig_P | lig_Q;
	snom_mva = 0.;
	auto_trafo = false;
	modelo_eqv = 1; // equivalente equilibrado
	//
	pri.fases = faseINV;
	pri.ligacao = lgTRIANG;
	pri.vnom = 0.;
	pri.tap = 0.;
	pri.defasagem = 0.;
	pri.zterra.r = 0.;
	pri.zterra.x = 0.;
	//
	sec.fases = faseINV;
	sec.ligacao = lgEST_ISO;
	sec.vnom = 0.;
	sec.tap = 0.;
	sec.defasagem = 30.;
	sec.zterra.r = 0.;
	sec.zterra.x = 0.;
	z0.r = 0.;
	z0.x = 0.;
	z1.r = 0.;
	z1.x = 0.;
	perda_ferro = 0.;
	//
	ltc.tipo = ltcFIXO;
	ltc.bar_id = -1;
	ltc.v_pu = 1.;
	// dados de trafo equivalente
	eqv.modelo = 0;
	eqv.y0_ij_pu.g = eqv.y0_ij_pu.b = 0.;
	eqv.y0_ji_pu.g = eqv.y0_ji_pu.b = 0.;
	eqv.y1_ij_pu.g = eqv.y1_ij_pu.b = 0.;
	eqv.y1_ji_pu.g = eqv.y1_ji_pu.b = 0.;
	// matYij_s
	for (int i = 0; i < MAX_FASE; i++)
	{
		for (int j = 0; j < MAX_FASE; j++)
		{
			eqv.matYij_s[i][j].g = 0;
			eqv.matYij_s[i][j].b = 0;
		}
	}
	// matYji_s
	for (int i = 0; i < MAX_FASE; i++)
	{
		for (int j = 0; j < MAX_FASE; j++)
		{
			eqv.matYji_s[i][j].g = 0;
			eqv.matYji_s[i][j].b = 0;
		}
	}
	// esquemático
	esq.posbar1 = 50.;
	esq.posbar2 = 50.;
	esq.posbar3 = 0.;
	// comentario é iniciado pelo seu construtor;
}

// ---------------------------------------------------------------------------
__fastcall strTRAFO::~strTRAFO(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strTRAFO3E::strTRAFO3E(void)
{
	obra = false;
	trf_id = ID_NULO;
	rede_id = ID_NULO;
	bar1_id = ID_NULO;
	bar2_id = ID_NULO;
	bar3_id = ID_NULO;
	codigo = "";
	extern_id = "";
	show = eqp_VISIBLE | lig_P | lig_Q;
	perda_ferro = 0.;
	zps0.r = zps0.r = 0.;
	zps1.r = zps1.x = 0.;
	zpt0.r = zpt0.x = 0.;
	zpt1.r = zpt1.x = 0.;
	zst0.r = zst0.x = 0.;
	zst1.r = zst1.x = 0.;
	//
	pri.fases = faseINV;
	pri.ligacao = lgTRIANG;
	pri.snom = 0.;
	pri.vnom = 0.;
	pri.tap = 0.;
	pri.defasagem = 0.;
	pri.zterra.r = 0.;
	pri.zterra.x = 0.;
	//
	sec.fases = faseINV;
	sec.ligacao = lgEST_ISO;
	sec.snom = 0.;
	sec.vnom = 0.;
	sec.tap = 0.;
	sec.defasagem = 30.;
	sec.zterra.r = 0.;
	sec.zterra.x = 0.;
	//
	ter.fases = faseINV;
	ter.ligacao = lgEST_ISO;
	ter.snom = 0.;
	ter.vnom = 0.;
	ter.tap = 0.;
	ter.defasagem = 30.;
	ter.zterra.r = 0.;
	ter.zterra.x = 0.;
	//
   ltc.tipo      = ltcFIXO;
   ltc.dv_perc   = 20.;
   ltc.num_passo = 20;
   ltc.passo     = 0;
   //
	esq.posbar1 = 50.;
	esq.posbar2 = 50.;
	esq.posbar3 = 50.;
	// comentario é iniciado pelo seu construtor;
}

// ---------------------------------------------------------------------------
__fastcall strTRAFO3E::~strTRAFO3E(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strTRAFOZZ::strTRAFOZZ(void)
{
	obra = false;
	id = ID_NULO;
	bar_id = ID_NULO;
	codigo = "";
	extern_id = "";
	show = eqp_VISIBLE | eqp_FIXO;
	vnom = 0.;
	snom = 0.;
	z0.r = 0.;
	z0.x = 0.;
	//
	esq.posbar = 50.;
	esq.dxbar = 0;
	esq.dybar = 0;
	// comentario é iniciado pelo seu construtor;
}

// ---------------------------------------------------------------------------
__fastcall strTRAFOZZ::~strTRAFOZZ(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strTRECHO::strTRECHO(void)
{
	obra = false;
	tre_id = ID_NULO;
	rede_id = ID_NULO;
	bar1_id = ID_NULO;
	bar2_id = ID_NULO;
	codigo = "";
	extern_id = "";
	show = eqp_VISIBLE | lig_P | lig_Q;
	comp_km = 0.;
	fases = faseINV;
	//
	esq.posbar1 = 50.;
	esq.posbar2 = 50.;
	esq.posbar3 = 0.;
	esq.comprimento = 0;
	// comentario é iniciado pelo seu construtor;
}

// ---------------------------------------------------------------------------
__fastcall strTRECHO::~strTRECHO(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
__fastcall strYREF::strYREF(void)
{
	obra = false;
	yref_id = ID_NULO;
	barra_id = ID_NULO;
	codigo = "";
	extern_id = "";
	show = eqp_VISIBLE | yref_Y_PU100;
	y0_pu.g = 0.;
	y0_pu.b = 0.;
	y1_pu.g = 0.;
	y1_pu.b = 0.;
	//
	for (int i = 0; i < MAX_FASE; i++)
	{
		for (int j = 0; j < MAX_FASE; j++)
		{
			maty_pu[i][j].g = 0.;
			maty_pu[i][j].b = 0.;
		}
	}
	esq.posbar = 50.;
	esq.dxbar = 0;
	esq.dybar = 0;
	// comentario é iniciado pelo seu construtor;
}

// ---------------------------------------------------------------------------
__fastcall strYREF::~strYREF(void)
{ // nada a fazer
}

// ---------------------------------------------------------------------------
// eof
