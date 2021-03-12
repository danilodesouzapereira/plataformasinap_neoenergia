// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <DateUtils.hpp>
#include "TMontaRede.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Arranjo\VTArranjo.h"
#include "..\..\Arranjo\VTArranjos.h"
#include "..\..\Arranjo\VTCabo.h"
#include "..\..\Arranjo\VTPtoFixacao.h"
#include "..\..\Arranjo\VTSuporte.h"
#include "..\..\Classes\VTClasse.h"
#include "..\..\Classes\VTClasses.h"
#include "..\..\Curvas\VTCurvas.h"
#include "..\..\Curvas\VTCurva.h"
#include "..\..\Esquematico\VTEsquematico.h"
#include "..\..\Geral\VTGeral.h"
#include "..\..\Importa\ValEqpto\VTValEqpto.h"
#include "..\..\Log\VTLog.h"
#include "..\..\MontaZ\VTMontaZ.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Potencia\VTDemanda.h"
#include "..\..\Rede\VTListeqp.h"
#include "..\..\Rede\VTArea.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTBateria.h"
#include "..\..\Rede\VTCanal.h"
#include "..\..\Rede\VTCapacitor.h"
#include "..\..\Rede\VTCapserie.h"
#include "..\..\Rede\VTCarga.h"
#include "..\..\Rede\VTCargaNL.h"
#include "..\..\Rede\VTChave.h"
#include "..\..\Rede\VTCluster.h"
#include "..\..\Rede\VTComentario.h"
#include "..\..\Rede\VTCorte.h"
#include "..\..\Rede\VTDadosExt.h"
#include "..\..\Rede\VTDominio.h"
#include "..\..\Rede\VTFases.h"
#include "..\..\Rede\VTFiltro.h"
#include "..\..\Rede\VTGerador.h"
#include "..\..\Rede\VTListeqp.h"
#include "..\..\Rede\VTMatC.h"
#include "..\..\Rede\VTMatZ.h"
#include "..\..\Rede\VTMatY.h"
#include "..\..\Rede\VTMedidor.h"
#include "..\..\Rede\VTMutua.h"
#include "..\..\Rede\VTMRede.h"
#include "..\..\Rede\VTReator.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTReducao.h"
#include "..\..\Rede\VTRegulador.h"
#include "..\..\Rede\VTResFlowBar.h"
#include "..\..\Rede\VTSuprimento.h"
#include "..\..\Rede\VTTrafo.h"
#include "..\..\Rede\VTTrafo3E.h"
#include "..\..\Rede\VTTrafoZZ.h"
#include "..\..\Rede\VTTrecho.h"
#include "..\..\Rede\VTTipoChave.h"
#include "..\..\Rede\VTTipoRede.h"
#include "..\..\Rede\VTTipos.h"
#include "..\..\Rede\VTYref.h"
#include "..\..\Topologia\VTTopologia.h"
#include "..\..\Zona\VTZona.h"
#include "..\..\Zona\VTZonas.h"
#include "..\..\..\DLL_Inc\Arranjo.h"
#include "..\..\..\DLL_Inc\Curvas.h"
#include "..\..\..\DLL_Inc\Esquematico.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Rede.h"

// ---------------------------------------------------------------------------
VTMonta* NewObjMontaRede(VTApl *apl)
{
	return (new TMontaRede(apl));
}

// ---------------------------------------------------------------------------
__fastcall TMontaRede::TMontaRede(VTApl *apl)
{
	// salva ponteiros p/ objetos
	this->apl = apl;
	redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	// mercado   = (VTMercado*)apl->GetObject(__classid(VTMercado));
	patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
	demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	fases = (VTFases*)apl->GetObject(__classid(VTFases));
	classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
	plog = (VTLog*)apl->GetObject(__classid(VTLog));
	montaZ = (VTMontaZ*)apl->GetObject(__classid(VTMontaZ));
	valida = (VTValEqpto*)apl->GetObject(__classid(VTValEqpto));
	// cria objetos locais
	esquematico = DLL_NewObjEsquematico(apl);
	lisTMP = new TList();
	// cria lisBAR como VTListeqp e habilita busca binária
	lisBAR = DLL_NewObjListeqp();
	lisBAR->BuscaBinariaEnabled = true;
}

// ---------------------------------------------------------------------------
__fastcall TMontaRede::~TMontaRede(void)
{
	// destrói  objetos
	if (esquematico)
	{
		delete esquematico;
		esquematico = NULL;
	}
	// destrói listas sem destruir seus objetos
	if (lisBAR)
	{
		delete lisBAR;
		lisBAR = NULL;
	}
	if (lisTMP)
	{
		delete lisTMP;
		lisTMP = NULL;
	}
}

// -----------------------------------------------------------------------------
void __fastcall TMontaRede::AcumulaCarga(VTCarga *carga, strCARGA &str)
{
	// variáveis locais
	strHM hm_ini, hm_fim;
	union
	{
		double vet[2];
		struct
		{
			double p, q;
		} var;
	} valor;

	// atualiza pontos da Curva
	for (int np = 0; np < MAX_PAT; np++)
	{ // obtém valor atual do ponto
		carga->curva->GetPonto(np, hm_ini, hm_fim, valor.vet, sizeof(valor.vet) / sizeof(double));
		// acumula a demanda
		// valor.var.p += str.demanda[np].p_mw;
		// valor.var.q += str.demanda[np].q_mvar;
		valor.var.p += str.curva.patamar[np].valor[indP];
		valor.var.q += str.curva.patamar[np].valor[indQ];
		// atualiza o ponto
		carga->curva->SetPonto(hm_ini, hm_fim, valor.vet, sizeof(valor.vet) / sizeof(double));
	}
}

// -----------------------------------------------------------------------------
VTArranjo* __fastcall TMontaRede::CriaArranjoProprioPU_SB100(strARRANJO1 &str, double comp_km)
{
	// variáveis locais
	VTArranjo *arranjo;

	// proteção
	if (str.tipo_cfg != arranjoPU_SB100)
		return (NULL);
	// cria um novo objeto Arranjo do tipo arranjoZ0Z1
	if ((arranjo = DLL_NewObjArranjo()) == NULL)
		return (NULL);
	// define Arranjo como novo
	arranjo->Status[sttNOVO] = true;
	arranjo->Owner = NULL;
	arranjo->Data = str.data;
	arranjo->Codigo = "";
	arranjo->Extern_id = "";
	arranjo->TipoCfg = arranjoPU_SB100;
	arranjo->Fases = faseABC;
	arranjo->ramal = str.ramal;
	arranjo->Iadm_amp = str.cfg_pu.iadm_amp;
	arranjo->ramal = str.ramal;
	// define impedâncias e capacitâncias
	arranjo->DefZ0Z1_pu(str.cfg_pu.sbase_mva, str.cfg_pu.vbase_kv, comp_km, str.cfg_pu.z0_pu,
		str.cfg_pu.z1_pu);
	arranjo->DefC0C1_pu(str.cfg_pu.sbase_mva, str.cfg_pu.vbase_kv, comp_km, str.cfg_pu.yc0_pu,
		str.cfg_pu.yc1_pu);
	// monta matriz de impedâncias
	if (!montaZ->Executa(arranjo))
		return (NULL);

	return (arranjo);
}

// -----------------------------------------------------------------------------
VTArranjo* __fastcall TMontaRede::CriaArranjoProprioZ0Z1(strARRANJO1 &str)
{
	// variáveis locais
	VTArranjo *arranjo;

	// cria um novo objeto Arranjo do tipo arranjoZ0Z1
	if ((arranjo = DLL_NewObjArranjo()) == NULL)
		return (NULL);
	// define Arranjo como novo
	arranjo->Status[sttNOVO] = true;
	arranjo->Owner = NULL;
	arranjo->Data = str.data;
	// verifica o tipo de configuração
	if (str.tipo_cfg == arranjoZ0Z1)
	{ // define dados do Arranjo
		arranjo->Codigo = "";
		arranjo->Extern_id = "";
		arranjo->TipoCfg = arranjoZ0Z1;
		arranjo->Fases = (str.fases != faseINV) ? str.fases : faseABCN;
		arranjo->ramal = str.ramal;
		arranjo->z0 = str.cfg_arranjoZ0Z1.z0;
		arranjo->z1 = str.cfg_arranjoZ0Z1.z1;
		arranjo->c0 = str.cfg_arranjoZ0Z1.c0;
		arranjo->c1 = str.cfg_arranjoZ0Z1.c1;
		arranjo->Iadm_amp = str.cfg_arranjoZ0Z1.iadm_amp;
		// proteção: r0 >= r1
		if (arranjo->z0.r < arranjo->z1.r)
			arranjo->z0.r = arranjo->z1.r;
		// proteção: x0 >= x1
		if (arranjo->z0.x < arranjo->z1.x)
			arranjo->z0.x = arranjo->z1.x;
		// proteção: z0 > 0
		if (IsDoubleZero(arranjo->z0.r) && IsDoubleZero(arranjo->z0.x))
		{
			arranjo->z0 = arranjo->z1;
		}
		// proteção: z1 > 0
		if (IsDoubleZero(arranjo->z1.r) && IsDoubleZero(arranjo->z1.x))
		{
			arranjo->z1 = arranjo->z0;
		}
		// proteção: (z0 > 0)ou (z1 > 0)
		if (IsDoubleZero(arranjo->z0.r) && IsDoubleZero(arranjo->z0.x) && IsDoubleZero
			(arranjo->z1.r) && IsDoubleZero(arranjo->z1.x))
		{
			arranjo->z0.r = 0.02;
			arranjo->z0.x = 0.05;
			arranjo->c0 = 0;
			arranjo->z1.r = 0.02;
			arranjo->z1.x = 0.05;
			arranjo->c1 = 0;
		}
	}
	else
	{ // define dados default p/ o Arranjo
		arranjo->Codigo = "";
		arranjo->TipoCfg = arranjoZ0Z1;
		arranjo->Fases = faseABCN;
		arranjo->ramal = str.ramal;
		arranjo->z0.r = 0.02;
		arranjo->z0.x = 0.05;
		arranjo->c0 = 0;
		arranjo->z1.r = 0.02;
		arranjo->z1.x = 0.05;
		arranjo->c1 = 0;
		arranjo->Iadm_amp = 350;
	}
	// monta matriz de impedâncias
	if (!montaZ->Executa(arranjo))
		return (NULL);
	return (arranjo);
}

// -----------------------------------------------------------------------------
VTArranjo* __fastcall TMontaRede::CriaArranjoTipicoCABOxFASE(AnsiString codigo, TDate data,
	VTSuporte *suporte, VTCabo *cabo[])
{
	// variáveis locais
	int tag = 0;
	int num_cabo = 0;
	VTArranjo *arranjo;
	VTPtoFixacao *pto_fixacao;

	// proteção: verifica se há Cabos definidos
	for (int nf = 0; nf < MAX_FASE; nf++)
	{
		if (cabo[nf] != NULL)
			num_cabo++;
	}
	// if (num_cabo < 2) return(NULL);
	if (num_cabo == 0)
		return (NULL);
	// cria um novo Arranjo
	if ((arranjo = DLL_NewObjArranjo()) == NULL)
		return (NULL);
	// define Arranjo como novo
	arranjo->Status[sttNOVO] = true;
	arranjo->Owner = NULL;
	arranjo->Data = data;
	// inicia dados
	arranjo->Id = FalsoId();
	arranjo->TipoCfg = arranjoCABOxFASE;
	arranjo->Codigo = (!codigo.IsEmpty()) ? codigo : DefineCodigoArranjo();
	arranjo->Extern_id = "";
	// define fases do Arranjo em função de existência de cabos nas fases
	if (cabo[0] != NULL)
		tag = fases->Fase1UniaoFase2(tag, faseA);
	if (cabo[1] != NULL)
		tag = fases->Fase1UniaoFase2(tag, faseB);
	if (cabo[2] != NULL)
		tag = fases->Fase1UniaoFase2(tag, faseC);
	if (cabo[3] != NULL)
		tag = fases->Fase1UniaoFase2(tag, faseN);
	// proteção temporária
	// int PENDENCIA_MONTAGEM_DE_ARRANJO=1;
	// if (tag != faseABCN) {tag = faseABCN;}
	// define fases do Arranjo
	arranjo->Fases = tag;
	// define Suporte
	arranjo->suporte = suporte;
	// define PtoFixacao e Cabo do Arranjo
	for (int nf = 0; nf < MAX_FASE; nf++)
	{ // verifica se tem a fase
		if (cabo[nf] == NULL)
			continue;
		// verifica se existe PtoFixacao p/ fase
		if ((pto_fixacao = suporte->ExistePtoFixacaoIndex(nf)) == NULL)
			return (NULL);
		// define PtoFixacao e Cabo p/ fase do Arranjo
		arranjo->DefPtoFixacaoFase(nf, pto_fixacao);
		arranjo->DefCaboFase(nf, cabo[nf]);
	}
	// calcula Matriz de impedâncias do Arranjo
	if (!montaZ->Executa(arranjo))
	{ // destrói arranjo
		delete arranjo;
		return (NULL);
	}
	// insere Arranjo em Arranjos
	arranjos->InsereArranjo(arranjo);
	return (arranjo);
}

// -----------------------------------------------------------------------------
VTArranjo* __fastcall TMontaRede::CriaArranjoTipicoCABO_Z0Z1(AnsiString codigo, TDate data,
	VTSuporte *suporte, VTCabo *cabo)
{
	// variáveis locais
	VTArranjo *arranjo;

	// cria um novo Arranjo
	if ((arranjo = DLL_NewObjArranjo()) == NULL)
		return (NULL);
	// define Arranjo como novo
	arranjo->Status[sttNOVO] = true;
	arranjo->Owner = NULL;
	arranjo->Data = data;
	// inicia dados
	arranjo->Id = FalsoId();
	arranjo->TipoCfg = arranjoCABO_Z0Z1;
	arranjo->Fases = faseABCN;
	arranjo->Codigo = (!codigo.IsEmpty()) ? codigo : DefineCodigoArranjo();
	arranjo->Extern_id = "";
	// define Suporte
	arranjo->suporte = suporte;
	// define CaboZ0Z1
	arranjo->caboZ0Z1 = cabo;
	// calcula Matriz de impedâncias do Arranjo
	if (!montaZ->Executa(arranjo))
	{ // destrói arranjo
		delete arranjo;
		return (NULL);
	}
	// insere Arranjo em Arranjos
	arranjos->InsereArranjo(arranjo);
	return (arranjo);
}

// -----------------------------------------------------------------------------
VTArranjo* __fastcall TMontaRede::CriaArranjoTipicoZ0Z1(strARRANJO1 &str)
{
	// variáveis locais
	VTArranjo *arranjo;

	// cria um novo objeto Arranjo do tipo arranjoZ0Z1
	if ((arranjo = DLL_NewObjArranjo()) == NULL)
		return (NULL);
	// define Arranjo como novo
	arranjo->Status[sttNOVO] = true;
	arranjo->Owner = NULL;
	arranjo->Data = str.data;
	// verifica o tipo de configuração
	if (str.tipo_cfg == arranjoZ0Z1)
	{ // define dados do Arranjo
		arranjo->Codigo = (!str.codigo.IsEmpty()) ? str.codigo : DefineCodigoArranjo();
		arranjo->Extern_id = str.extern_id;
		arranjo->TipoCfg = arranjoZ0Z1;
		arranjo->Vmax_KV = 0.;
		arranjo->Fases = faseABCN;
		arranjo->ramal = str.ramal;
		arranjo->z0 = str.cfg_arranjoZ0Z1.z0;
		arranjo->z1 = str.cfg_arranjoZ0Z1.z1;
		arranjo->c0 = str.cfg_arranjoZ0Z1.c0;
		arranjo->c1 = str.cfg_arranjoZ0Z1.c1;
		arranjo->Iadm_amp = str.cfg_arranjoZ0Z1.iadm_amp;
		// proteção: r0 >= r1
		if (arranjo->z0.r < arranjo->z1.r)
			arranjo->z0.r = arranjo->z1.r;
		// proteção: x0 >= x1
		if (arranjo->z0.x < arranjo->z1.x)
			arranjo->z0.x = arranjo->z1.x;
		// proteção: z0 > 0
		if (IsDoubleZero(arranjo->z0.r) && IsDoubleZero(arranjo->z0.x))
		{
			arranjo->z0 = arranjo->z1;
		}
		// proteção: z1 > 0
		if (IsDoubleZero(arranjo->z1.r) && IsDoubleZero(arranjo->z1.x))
		{
			arranjo->z1 = arranjo->z0;
		}
		// proteção: (z0 > 0)ou (z1 > 0)
		if (IsDoubleZero(arranjo->z0.r) && IsDoubleZero(arranjo->z0.x) && IsDoubleZero
			(arranjo->z1.r) && IsDoubleZero(arranjo->z1.x))
		{
			arranjo->z0.r = 0.02;
			arranjo->z0.x = 0.05;
			arranjo->c0 = 0;
			arranjo->z1.r = 0.02;
			arranjo->z1.x = 0.05;
			arranjo->c1 = 0;
		}
	}
	else
	{ // define dados default p/ o Arranjo
		arranjo->Codigo = (!str.codigo.IsEmpty()) ? str.codigo : DefineCodigoArranjo();
		arranjo->TipoCfg = arranjoZ0Z1;
		arranjo->Fases = faseABCN;
		arranjo->ramal = str.ramal;
		arranjo->z0.r = 0.02;
		arranjo->z0.x = 0.05;
		arranjo->c0 = 0;
		arranjo->z1.r = 0.02;
		arranjo->z1.x = 0.05;
		arranjo->c1 = 0;
		arranjo->Iadm_amp = 350;
	}
	// monta matriz de impedâncias
	if (!montaZ->Executa(arranjo))
		return (NULL);
	// insere Arranjo em Arranjos
	arranjos->InsereArranjo(arranjo);
	return (arranjo);
}

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaRede::CriaCurvaBateria(strCURVA &str)
   {
   //variáveis locais
   VTCurva *curva;

   //proteção: verifica se o número de valores da curva está definido
	if (str.num_valor == 0)
		str.num_valor = nvCURVA_PQ;
   //cria uma nova Curva
   curva                 = DLL_NewObjCurva();
   curva->Id             = str.id;
   curva->Codigo         = str.codigo;
   curva->Tipo           = curvaPQ;
   curva->Unidade        = unidKVA;
   curva->Escala         = patEDITAVEL;
   curva->NumeroValores  = str.num_valor;
   curva->Tipica         = NULL;
   curva->DiaMedicao     = SEMANA_COMPLETA;
   curva->Eqpto          = NULL;
   curva->Classe         = NULL;
   curva->LimiteInferior = 0;
   curva->LimiteSuperior = 0;
	if (!LePontosCurva(curva, str))
		return (NULL);
   return(curva);
   }

//-----------------------------------------------------------------------------
VTCurva* __fastcall TMontaRede::CriaCurvaCanal(strCURVA &str, int tipo_canal)
{
	// variáveis locais
	VTCurva *curva;

	// cria uma nova Curva
	curva = DLL_NewObjCurva();
	curva->Id = str.id;
	curva->Codigo = str.codigo;
	curva->Tipica = false;
	curva->Unidade = str.unidade;
	curva->NumeroValores = str.num_valor;
	curva->Escala = str.num_pat;
	curva->Classe = NULL;
	curva->LimiteInferior = 0;
	curva->LimiteSuperior = 0;
	switch (tipo_canal)
	{
	case canalV:
		curva->Tipo = curvaV;
		break;
	case canalI:
		curva->Tipo = curvaI;
		break;
	case canalP:
		curva->Tipo = curvaP;
		break;
	case canalQ:
		curva->Tipo = curvaQ;
		break;
	case canalS:
		curva->Tipo = curvaS;
	default:
		return (NULL);
	}
	// lê pontos da Curva
	if (!LePontosCurva(curva, str))
		return (NULL);
	// inicia pontos da Curva
	curva->Reconfigura(pat01HORA);
	return (curva);
}

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaRede::CriaCurvaCapacitor(strCURVA &str)
{
	// variáveis locais
	VTCurva *curva;

	// cria uma nova Curva
	curva = DLL_NewObjCurva();
	curva->Id = str.id;
	curva->Codigo = str.codigo;
	curva->Tipo = curvaPQ;
	curva->Unidade = unidMVA;
	curva->Escala = patEDITAVEL;
	curva->NumeroValores = nvCURVA_PQ;
	curva->Tipica = str.tipica;
	curva->DiaMedicao = SEMANA_COMPLETA;
	// curva->Eqpto       = pger;
	curva->Classe = NULL;
	curva->LimiteInferior = 0;
	curva->LimiteSuperior = 0;
	// lê pontos da Curva
	if (!LePontosCurva(curva, str))
		return (NULL);
	return (curva);
}

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaRede::CriaCurvaCarga(strCURVA &str)
{
	// variáveis locais
	VTCurva *curva;

	// JCG 2011.12.15
	// IMPORTANTE: strCURVA pode ser usado p/ definir curva c/ demanda total (nvCURVA_CAR valores) ou
	// curva com demanda por fase (nvCURVA_RRCAR valores). Se strCURVA.num valor não estiver
	// definido, assume curva de demanda total (default)

	// proteção: verifica se o número de valores da curva está definido
	if (str.num_valor == 0)
		str.num_valor = nvCURVA_PQ;
	// cria uma nova Curva
	curva = DLL_NewObjCurva();
	curva->Id = str.id;
	curva->Codigo = str.codigo;
	curva->Tipo = curvaPQ;
	curva->Unidade = unidMVA;
	curva->Escala = patEDITAVEL;
	curva->NumeroValores = str.num_valor;
	curva->Tipica = str.tipica;
	curva->DiaMedicao = SEMANA_COMPLETA;
	curva->Eqpto = NULL;
	curva->Classe = NULL;
	curva->LimiteInferior = 0;
	curva->LimiteSuperior = 0;
	if (!LePontosCurva(curva, str))
		return (NULL);
	return (curva);
}
/*
 //-----------------------------------------------------------------------------
 VTCurva* __fastcall TMontaRede::CriaCurvaCargaReducao(strCURVA &str)
 {
 //variáveis locais
 strHM     hm_ini, hm_fim;
 double    valor[32];
 VTCurva   *curva;

 //cria uma nova Curva
 curva                = DLL_NewObjCurva();
 curva->Id            = str.id;
 curva->Codigo        = str.codigo;
 curva->Tipo          = curvaPQ;
 curva->Unidade       = unidMVA;
 curva->Escala        = patEDITAVEL;
 curva->NumeroValores = nvCURVA_RRCAR;
 curva->Tipica        = false;
 curva->Eqpto         = NULL;
 curva->Classe        = NULL;
 //define pontos da Curva
 for (int np = 0; np < str.curva.num_pat; np++)
 {
 hm_ini = str.curva.patamar[np].hm_ini;
 hm_fim = str.curva.patamar[np].hm_fim;
 for (int ind = 0; ind < nvCURVA_RRCAR; ind++)
 {
 valor[ind] = str.curva.patamar[np].valor[ind];
 }
 curva->SetPonto(hm_ini, hm_fim, valor, nvCURVA_RRCAR);
 }
 return(curva);
 }
 */

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaRede::CriaCurvaGerador(strCURVA &str)
{
	// variáveis locais
	VTCurva *curva;

	// cria uma nova Curva
	curva = DLL_NewObjCurva();
	curva->Id = str.id;
	curva->Codigo = str.codigo;
	curva->Tipo = curvaPQVT;
	curva->Unidade = unidMVA;
	curva->Escala = patEDITAVEL;
	curva->NumeroValores = nvCURVA_PQVT;
	curva->Tipica = str.tipica;
	curva->DiaMedicao = SEMANA_COMPLETA;
	// curva->Eqpto       = pger;
	curva->Classe = NULL;
	curva->LimiteInferior = 0;
	curva->LimiteSuperior = 0;
	// lê pontos da Curva
	if (!LePontosCurva(curva, str))
		return (NULL);
	/*
	 //define pontos da Curva (unck@20080507)
	 for (int np = 0; np < str.curva.num_pat; np++)
	 {
	 hm_ini = str.curva.patamar[np].hm_ini;
	 hm_fim = str.curva.patamar[np].hm_fim;
	 valor.var.p = str.curva.patamar[np].valor[indP];
	 valor.var.q = str.curva.patamar[np].valor[indQ];
	 valor.var.v = str.curva.patamar[np].valor[indV];
	 valor.var.t = str.curva.patamar[np].valor[indT];
	 //proteção
	 if (valor.var.v <= 0.) valor.var.v = 1.0;
	 curva->SetPonto(hm_ini, hm_fim, valor.vet, sizeof(valor.vet)/sizeof(double));
	 }
	 */
	return (curva);
}

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaRede::CriaCurvaReator(strCURVA &str)
{
	// variáveis locais
	VTCurva *curva;

	// cria uma nova Curva
	curva = DLL_NewObjCurva();
	curva->Id = str.id;
	curva->Codigo = str.codigo;
	curva->Tipo = curvaPQ;
	curva->Unidade = unidMVA;
	curva->Escala = patEDITAVEL;
	curva->NumeroValores = nvCURVA_PQ;
	curva->Tipica = str.tipica;
	curva->DiaMedicao = SEMANA_COMPLETA;
	// curva->Eqpto       = pger;
	curva->Classe = NULL;
	curva->LimiteInferior = 0;
	curva->LimiteSuperior = 0;
	// lê pontos da Curva
	if (!LePontosCurva(curva, str))
		return (NULL);
	/*
	 //define pontos da Curva
	 for (int np = 0; np < str.curva.num_pat; np++)
	 {
	 hm_ini      = str.curva.patamar[np].hm_ini;
	 hm_fim      = str.curva.patamar[np].hm_fim;
	 valor.var.p = 0;
	 //valor.var.q = str.demanda[np].q_mvar;
	 valor.var.q = str.curva.patamar[np].valor[indQ];
	 curva->SetPonto(hm_ini, hm_fim, valor.vet, sizeof(valor.vet)/sizeof(double));
	 }
	 */
	return (curva);
}

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaRede::CriaCurvaSuprimento(strCURVA &str)
{
	// variáveis locais
	VTCurva *curva;

	// proteção
	if (str.num_valor < nvCURVA_PQVT)
		str.num_valor = nvCURVA_PQVT;
	// cria uma nova Curva
	curva = DLL_NewObjCurva();
	curva->Id = str.id;
	curva->Codigo = str.codigo;
	curva->Tipo = curvaPQVT;
	curva->Unidade = unidMVA;
	curva->Escala = patEDITAVEL;
	// curva->NumeroValores = nvCURVA_PQVT;
	curva->NumeroValores = str.num_valor;
	curva->Tipica = str.tipica;
	// curva->Eqpto       = psup;
	curva->Classe = NULL;
	curva->LimiteInferior = 0;
	curva->LimiteSuperior = 0;
	// lê pontos da Curva
	if (!LePontosCurva(curva, str))
		return (NULL);
	/*
	 //define pontos da Curva (unck@20080507)
	 for (int np = 0; np < str.curva.num_pat; np++)
	 {
	 hm_ini = str.curva.patamar[np].hm_ini;
	 hm_fim = str.curva.patamar[np].hm_fim;
	 valor.var.p = str.curva.patamar[np].valor[indP];
	 valor.var.q = str.curva.patamar[np].valor[indQ];
	 valor.var.v = str.curva.patamar[np].valor[indV];
	 valor.var.t = str.curva.patamar[np].valor[indT];
	 //proteção
	 if (valor.var.v <= 0.) valor.var.v = 1.0;
	 curva->SetPonto(hm_ini, hm_fim, valor.vet, sizeof(valor.vet)/sizeof(double));
	 }
	 */
	return (curva);
}

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaRede::CriaCurvaSuprimentoReducao(strCURVA &str)
{
	// variáveis locais
	VTCurva *curva;

	// cria uma nova Curva
	curva = DLL_NewObjCurva();
	curva->Id = str.id;
	curva->Codigo = str.codigo;
	curva->Tipo = curvaPQVT;
	curva->Unidade = unidMVA;
	curva->Escala = patEDITAVEL;
	curva->NumeroValores = nvCURVA_RRSUP;
	curva->Tipica = false;
	curva->Eqpto = NULL;
	curva->Classe = NULL;
	curva->LimiteInferior = 0;
	curva->LimiteSuperior = 0;
	// lê pontos da Curva
	if (!LePontosCurva(curva, str))
		return (NULL);
	/*
	 //define pontos da Curva (unck@20080507)
	 for (int np = 0; np < str.curva.num_pat; np++)
	 {
	 hm_ini = str.curva.patamar[np].hm_ini;
	 hm_fim = str.curva.patamar[np].hm_fim;
	 for (int ind = 0; ind < nvCURVA_RRSUP; ind++)
	 {
	 valor[ind] = str.curva.patamar[np].valor[ind];
	 }
	 curva->SetPonto(hm_ini, hm_fim, valor, nvCURVA_RRSUP);
	 }
	 */
	return (curva);
}

// -----------------------------------------------------------------------------
bool __fastcall TMontaRede::DefineBarrraInicial(AnsiString rede_cod, int barra_id)
{
	// variáveis locais
	VTBarra *barra;
	VTRede *rede;

	// determina Rede
	if ((rede = redes->ExisteRede(rede_cod)) == NULL)
		return (false);
	// determina Barra
	if ((barra = ExisteBarra(barra_id)) == NULL)
		return (false);
	// define Barra inicial da Rede
	rede->DefineBarraInicial(barra);

	return (true);
}

// -----------------------------------------------------------------------------
void __fastcall TMontaRede::DefineBarraInicial(VTBarra *barra)
{
	// variáveis locais
	VTRede *rede;
	TList *lisREDE = redes->LisRede();

	for (int nr = 0; nr < lisREDE->Count; nr++)
	{
		rede = (VTRede*)lisREDE->Items[nr];
		if (rede->ExisteBarra(barra))
		{
			rede->DefineBarraInicial(barra);
		}
	}
}

// -----------------------------------------------------------------------------
AnsiString __fastcall TMontaRede::DefineCodigoArranjo(void)
{
	// variáveis locais
	AnsiString codigo;
	int count = 1;

	do
	{ // cria um novo código
		codigo.sprintf("Arranjo_%04d", count++);
	}
	while (arranjos->ExisteArranjo(codigo) != NULL);
	return (codigo);
}

// -----------------------------------------------------------------------------
VTArranjo* __fastcall TMontaRede::ExisteArranjoTipico(int arranjo_id)
{
	return (arranjos->ExisteArranjo(arranjo_id));
}

// -----------------------------------------------------------------------------
VTArranjo* __fastcall TMontaRede::ExisteArranjoTipico(AnsiString cod_arranjo, int cfg_arranjo)
{
	// variáveis locais
	VTArranjo *arranjo = NULL;

	// proteção
	if (cod_arranjo.IsEmpty())
		return (NULL);
	// verifica se já existe arranjo na lista
	arranjo = arranjos->ExisteArranjo(cod_arranjo, cfg_arranjo);

	return (arranjo);
}

// -----------------------------------------------------------------------------
VTArranjo* __fastcall TMontaRede::ExisteArranjoTipico_CABO_FASE(strARRANJO1 &str,
	VTTipoRede *tiporede)
{
	// variáveis locais
	int num_fase = 0;
	VTArranjo *arranjo;
	VTCabo *cabo[MAX_FASE];
	VTSuporte *suporte;

	// determina Suporte do Arranjo
	if (str.cfg_suporte_cabo.estgeo_id.IsEmpty())
	{
		if ((suporte = arranjos->ExisteSuporte(tiporede->Codigo)) == NULL)
		{
			if (plog)
				plog->DefineValor("Eqpto", "Suporte");
			if (plog)
				plog->Add("Não existe suporte p/ rede do tipo " +
				AnsiQuotedStr(tiporede->Codigo, '\''));
			return (NULL);
		}
	}
	else
	{
		if ((suporte = arranjos->ExisteSuporte(str.cfg_suporte_cabo.estgeo_id)) == NULL)
		{
			if (plog)
				plog->DefineValor("Eqpto", "Suporte");
			if (plog)
				plog->Add("Não existe suporte " + AnsiQuotedStr
				(str.cfg_suporte_cabo.estgeo_id, '\''));
			return (NULL);
		}
	}
	// determina Cabos do Arranjo
	for (int nf = 0; nf < MAX_FASE; nf++)
		cabo[nf] = NULL;
	// determina Cabo da fase A
	if (!str.cfg_suporte_cabo.cod_caboA.IsEmpty())
	{
		if ((cabo[0] = arranjos->ExisteCabo(str.cfg_suporte_cabo.cod_caboA)) == NULL)
		{
			if (plog)
				plog->DefineValor("Eqpto", "Cabo");
			if (plog)
				plog->Add("Não existe cabo " + AnsiQuotedStr(str.cfg_suporte_cabo.cod_caboA, '\''));
			return (NULL);
		}
		num_fase++;
	}
	// determina Cabo da fase B
	if (!str.cfg_suporte_cabo.cod_caboB.IsEmpty())
	{
		if ((cabo[1] = arranjos->ExisteCabo(str.cfg_suporte_cabo.cod_caboB)) == NULL)
		{
			if (plog)
				plog->DefineValor("Eqpto", "Cabo");
			if (plog)
				plog->Add("Não existe cabo " + AnsiQuotedStr(str.cfg_suporte_cabo.cod_caboB, '\''));
			return (NULL);
		}
		num_fase++;
	}
	// determina Cabo da fase C
	if (!str.cfg_suporte_cabo.cod_caboC.IsEmpty())
	{
		if ((cabo[2] = arranjos->ExisteCabo(str.cfg_suporte_cabo.cod_caboC)) == NULL)
		{
			if (plog)
				plog->DefineValor("Eqpto", "Cabo");
			if (plog)
				plog->Add("Não existe cabo " + AnsiQuotedStr(str.cfg_suporte_cabo.cod_caboC, '\''));
			return (NULL);
		}
		num_fase++;
	}
	// determina Cabo do neutro
	if (!str.cfg_suporte_cabo.cod_caboN.IsEmpty())
	{
		if ((cabo[3] = arranjos->ExisteCabo(str.cfg_suporte_cabo.cod_caboN)) == NULL)
		{
			if (plog)
				plog->DefineValor("Eqpto", "Cabo");
			if (plog)
				plog->Add("Não existe cabo " + AnsiQuotedStr(str.cfg_suporte_cabo.cod_caboN, '\''));
			return (NULL);
		}
	}
	// verifica se existe um Arranjo típico
	arranjo = arranjos->ExisteArranjoCABOxFASE(suporte, cabo[0], cabo[1], cabo[2], cabo[3]);
	if (arranjo == NULL)
	{ // cria Arranjo típico
		arranjo = CriaArranjoTipicoCABOxFASE(str.codigo, str.data, suporte, cabo);
		if (arranjo)
			arranjo->ramal = str.ramal;
	}
	return (arranjo);
}

// -----------------------------------------------------------------------------
VTArranjo* __fastcall TMontaRede::ExisteArranjoTipico_CABO_Z0Z1(strARRANJO1 &str)
{
	// variáveis locais
	VTArranjo *arranjo;
	VTCabo *cabo;

	// determina Cabo
	if ((cabo = arranjos->ExisteCabo(str.cfg_caboZ0Z1.cod_cabo, caboZ0Z1)) == NULL)
	{
		if (plog)
			plog->DefineValor("Eqpto", "Cabo");
		if (plog)
			plog->Add("Não existe cabo " + AnsiQuotedStr(str.cfg_caboZ0Z1.cod_cabo, '\''));
		return (NULL);
	}
	// verifica se existe Arranjo típico, do tipo arranjoCABO_Z0Z1
	if ((arranjo = arranjos->ExisteArranjoCABO_Z0Z1(cabo)) == NULL)
	{ // cria um novo Arranjo típico
		arranjo = CriaArranjoTipicoCABO_Z0Z1(str.codigo, str.data, NULL, cabo);
		if (arranjo)
			arranjo->ramal = str.ramal;
	}
	return (arranjo);
}

// -----------------------------------------------------------------------------
VTArranjo* __fastcall TMontaRede::ExisteArranjoTipico_Z0Z1(strARRANJO1 &str)
{
	// variáveis locais
	double r0, x0, c0, r1, x1, c1;
	VTArranjo *arranjo;

	// verifica se existe o arranjo típico por código
	if ((arranjo = ExisteArranjoTipico(str.codigo, arranjoZ0Z1)) == NULL)
	{ // verifica se existe Arranjo típico por valores de Z0,Z1,C0 e C1
		r0 = str.cfg_arranjoZ0Z1.z0.r;
		x0 = str.cfg_arranjoZ0Z1.z0.x;
		c0 = str.cfg_arranjoZ0Z1.c0;
		r1 = str.cfg_arranjoZ0Z1.z1.r;
		x1 = str.cfg_arranjoZ0Z1.z1.x;
		c1 = str.cfg_arranjoZ0Z1.c1;
		if ((arranjo = arranjos->ExisteArranjoZ0Z1(r0, x0, c0, r1, x1, c1)) == NULL)
		{ // verifica se é um Arranjo típico
			if (str.tipico)
			{
				arranjo = CriaArranjoTipicoZ0Z1(str);
			}
		}
	}
	return (arranjo);
}

/*
 //-----------------------------------------------------------------------------
 VTBarra* __fastcall TMontaRede::ExisteBarra(int barra_id)
 {
 //variáveis locais
 VTBarra *barra;

 //procura Barra na lista local lisBAR
 for (int nb = 0; nb < lisBAR->Count; nb++)
 {
 barra = (VTBarra*)lisBAR->Items[nb];
 if (barra->Id == barra_id) return(barra);
 }
 //não encontrou Barra em lisBAR: procura em Redes
 barra = redes->ExisteBarra(barra_id);
 //insere Barra em lisBAR
 if (barra) lisBAR->Add(barra);
 return(barra);
 }
 */
// -----------------------------------------------------------------------------
VTBarra* __fastcall TMontaRede::ExisteBarra(int barra_id)
{
	// variáveis locais
	VTBarra *barra, *bar;
	VTEqpto *eqpto;

	// procura Barra na lista local lisBAR
	if ((eqpto = lisBAR->ExisteEqpto(barra_id)) != NULL)
		return ((VTBarra*)eqpto);
	// não encontrou Barra em lisBAR: procura em Redes
	barra = redes->ExisteBarra(barra_id);
	// teste
	// if(! barra)
	// {
	// for(int nb = 0; nb < redes->LisBarra()->Count; nb++)
	// {
	// bar = (VTBarra*)redes->LisBarra()->Items[nb];
	// if(bar->Id == barra_id) {barra = bar; break;}
	// }
	// }
	// insere Barra em lisBAR
	// JCG 2016.01.27
	// if (barra) lisBAR->Add(barra);
	return (barra);
}

// -----------------------------------------------------------------------------
VTBarra* __fastcall TMontaRede::ExisteBarra(AnsiString codigo)
{
	// variáveis locais
	VTBarra *barra;

	// procura Barra na lista lisBAR local
	for (int nb = 0; nb < lisBAR->Count; nb++)
	{
		barra = (VTBarra*)lisBAR->Items[nb];
		if (codigo.AnsiCompareIC(barra->Codigo) == 0)
			return (barra);
	}
	// não encontrou Barra em lisBAR: procura em Redes
	barra = redes->ExisteBarra(codigo);
	// insere Barra em lisBAR
	// JCG 2016.01.27
	// if (barra) lisBAR->Add(barra);
	return (barra);
}

// -----------------------------------------------------------------------------
VTBarra* __fastcall TMontaRede::ExisteBarraCoordUtm(int utm_x, int utm_y)
{
	// variáveis locais
	VTBarra *barra;
	TList *lisBARRRA = redes->LisBarra();

	// procura Barra na lista lisBAR local
	for (int nb = 0; nb < lisBAR->Count; nb++)
	{
		barra = (VTBarra*)lisBAR->Items[nb];
		if ((barra->utm.x == utm_x) && (barra->utm.y == utm_y))
			return (barra);
	}
	// procura Barra por suas coord.utm na lista de Barras de Redes
	for (int nb = 0; nb < lisBARRRA->Count; nb++)
	{
		barra = (VTBarra*)lisBARRRA->Items[nb];
		if ((barra->utm.x == utm_x) && (barra->utm.y == utm_y))
		{ // insere Barra em lisBAR
			// JCG 2016.01.27
			// lisBAR->Add(barra);
			return (barra);
		}
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
VTBarra* __fastcall TMontaRede::ExisteBarraCoordUtm(int utm_x, int utm_y, double vnom_kv)
{
	// variáveis locais
	VTBarra *barra;
	TList *lisBARRRA = redes->LisBarra();

	// procura Barra na lista lisBAR local
	for (int nb = 0; nb < lisBAR->Count; nb++)
	{
		barra = (VTBarra*)lisBAR->Items[nb];
		if ((barra->utm.x == utm_x) && (barra->utm.y == utm_y))
		{
			if (IsDoubleZero(barra->vnom - vnom_kv))
				return (barra);
		}
	}
	// procura Barra por suas coord.utm na lista de Barras de Redes
	for (int nb = 0; nb < lisBARRRA->Count; nb++)
	{
		barra = (VTBarra*)lisBARRRA->Items[nb];
		if ((barra->utm.x == utm_x) && (barra->utm.y == utm_y))
		{
			if (IsDoubleZero(barra->vnom - vnom_kv))
			{ // insere Barra em lisBAR
				// JCG 2016.01.27
				// lisBAR->Add(barra);
				return (barra);
			}
		}
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
VTBarra* __fastcall TMontaRede::ExisteBarraCoordEsq(int esq_x1, int esq_y1, int esq_x2, int esq_y2)
{
	// variáveis locais
	VTBarra *barra;
	TList *lisBARRRA = redes->LisBarra();

	// procura Barra por suas coord.esq na lista local lisBAR
	for (int nb = 0; nb < lisBAR->Count; nb++)
	{
		barra = (VTBarra*)lisBAR->Items[nb];
		if ((barra->esq.x1 == esq_x1) && (barra->esq.y1 == esq_y1) && (barra->esq.x2 == esq_x2) &&
			(barra->esq.y2 == esq_y2))
			return (barra);
	}
	// procura Barra por suas coord.esq na lista de Barras de Redes
	for (int nb = 0; nb < lisBARRRA->Count; nb++)
	{
		barra = (VTBarra*)lisBARRRA->Items[nb];
		if ((barra->esq.x1 == esq_x1) && (barra->esq.y1 == esq_y1) && (barra->esq.x2 == esq_x2) &&
			(barra->esq.y2 == esq_y2))
		{ // insere Barra em lisBAR
			// JCG 2016.01.27
			// lisBAR->Add(barra);
			return (barra);
		}
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
VTCarga* __fastcall TMontaRede::ExisteCarga(VTBarra *pbarra)
{
	// variáveis locais
	VTCarga *pcarga = NULL;

	// obtém lista de Cargas da Barra
	lisTMP->Clear();
	pbarra->LisEqbar(lisTMP, eqptoCARGA);
	if (lisTMP->Count > 0)
	{
		pcarga = (VTCarga*)lisTMP->First();
	}
	// limpa lisTMP
	lisTMP->Clear();
	return (pcarga);
}

// -----------------------------------------------------------------------------
VTCargaNL* __fastcall TMontaRede::ExisteCargaNL(VTBarra *pbarra, int carga_id)
{
	// variáveis locais
	VTCargaNL *pcargaNL = NULL;

	// protege
	if (!pbarra)
		return (NULL);

	// obtém lista de Cargas da Barra
	lisTMP->Clear();
	pbarra->LisEqbar(lisTMP, eqptoCARGANL);
	for (int i = 0; i < lisTMP->Count; i++)
	{
		if ((pcargaNL = (VTCargaNL*)lisTMP->Items[i]) == NULL)
			return (NULL);
		if (pcargaNL->Id == carga_id)
		{
			// limpa lisTMP
			lisTMP->Clear();
			return (pcargaNL);
		}
	}
	// limpa lisTMP
	lisTMP->Clear();
	return (NULL);
}

// -----------------------------------------------------------------------------
VTClasse* __fastcall TMontaRede::ExisteClasse(strCARGA &str)
{
	// variáveis locais
	VTClasse *classe;

	// verifica se existe a Classe indicada
	if ((classe = classes->ExisteClasseTag(str.classe_cons)) == NULL)
	{ // assume Classe igual a consOUTROS
		classe = classes->ExisteClasseTag(consOUTROS);
	}
	return (classe);
}

// -----------------------------------------------------------------------------
VTChave* __fastcall TMontaRede::ExisteChave(VTRede *rede, int chave_id)
{
	// variáveis locais
	VTChave *pchave;

	// determina lista de Chaves da Rede
	lisTMP->Clear();
	rede->LisLigacao(lisTMP, eqptoCHAVE);
	// procura Chave na lista
	for (int n = 0; n < lisTMP->Count; n++)
	{
		pchave = (VTChave*)lisTMP->Items[n];
		if (pchave->Id == chave_id)
			return (pchave);
	}
	// não encontrou Chave na lista
	return (NULL);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::ExisteEqpto(int eqpto_tipo, AnsiString codigo)
{
	// variáveis locais
	VTEqpto* eqpto;

	// obtém lista de Eqptos do tipo indicado
	lisTMP->Clear();
	redes->LisEqpto(lisTMP, eqpto_tipo);
	for (int n = 0; n < lisTMP->Count; n++)
	{
		eqpto = (VTEqpto*)lisTMP->Items[n];
		if (eqpto->Codigo.AnsiCompareIC(codigo) == 0)
			return (eqpto);
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::ExisteEqpto(int eqpto_tipo, int eqpto_id)
{
	// variáveis locais
	VTEqpto* eqpto;

	// obtém lista de Eqptos do tipo indicado
	lisTMP->Clear();
	redes->LisEqpto(lisTMP, eqpto_tipo);
	for (int n = 0; n < lisTMP->Count; n++)
	{
		eqpto = (VTEqpto*)lisTMP->Items[n];
		if (eqpto->Id == eqpto_id)
			return (eqpto);
	}
	return (NULL);
}

/*
 //-----------------------------------------------------------------------------
 int __fastcall TMontaRede::ExisteFase(AnsiString codigo)
 {
 //variáveis locais
 int len;

 //substitui DEF por ABC
 len = codigo.Length();
 for (int n = 1; n <= len; n++)
 {
 if      (toupper(codigo[n]) == 'D') codigo[n] = 'A';
 else if (toupper(codigo[n]) == 'E') codigo[n] = 'B';
 else if (toupper(codigo[n]) == 'F') codigo[n] = 'C';
 }
 return(fases->ExisteFase(codigo));
 }
 */
// -----------------------------------------------------------------------------
int __fastcall TMontaRede::ExisteFase(AnsiString codigo)
{
	return (fases->ExisteFase(codigo));
}

// -----------------------------------------------------------------------------
int __fastcall TMontaRede::ExisteFaseCarga(AnsiString codigo)
{
	// verifica se o código possui uma única letra
	if (codigo.Length() == 1)
	{ // inclui neutro
		codigo += "N";
	}
	return (ExisteFase(codigo));
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::ExisteRede(AnsiString codigo)
{
	return (redes->ExisteRede(codigo));
}

// -----------------------------------------------------------------------------
VTTipoChave* __fastcall TMontaRede::ExisteTipoChave(int id, int tipo, AnsiString codigo)
{
	// variáveis locais
	VTTipoChave *tipochave = NULL;

	// dtermina TipoChave por Id
	if ((tipochave = tipos->ExisteTipoChave(id)) == NULL)
	{ // não encontrou: procura por tipo ou código
		if ((tipochave = tipos->ExisteTipoChave(codigo, tipo)) == NULL)
		{ // determina TipoChave indefinida

			tipochave = tipos->ExisteTipoChave("Indefinida", chaveINDEFINIDA);
		}
	}
	return (tipochave);
}

// -----------------------------------------------------------------------------
VTTipoRede* __fastcall TMontaRede::ExisteTipoRede(int id, int segmento, AnsiString codigo)
{
	// variáveis locais
	VTTipoRede *tiporede = NULL;

	// determina TipoRede por ID
	if ((tiporede = tipos->ExisteTipoRede(id)) == NULL)
	{ // não encontrou: determina TipoRede por Codigo ou Segmento
		if ((tiporede = tipos->ExisteTipoRede(codigo, segmento)) == NULL)
		{ // verifica se existe TipoRede indefinida
			//tiporede = ((VTTipoRede*)(tipos->LisTipoRede())->First());
         tiporede = tipos->ExisteTipoRede(redeINDEF);
		}
	}
	return (tiporede);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::ExisteTrecho(int utm_x1, int utm_y1, int utm_x2, int utm_y2)
{
	// variáveis locais
	VTBarra *barra1, *barra2;
	VTLigacao *ligacao;

	// determina Barras
	barra1 = (VTBarra*)ExisteBarraCoordUtm(utm_x1, utm_y1, 0);
	barra2 = (VTBarra*)ExisteBarraCoordUtm(utm_x2, utm_y2, 0);
	// proteção
	if ((barra1 == NULL) || (barra2 == NULL))
		return (NULL);
	// determina Trecho que conecta as duas Barras
	lisTMP->Clear();
	redes->LisLigacao(lisTMP, eqptoTRECHO);
	for (int n = 0; n < lisTMP->Count; n++)
	{
		ligacao = (VTLigacao*)lisTMP->Items[n];
		if ((ligacao->pbarra1 == barra1) && (ligacao->pbarra2 == barra2))
			return (ligacao);
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
VTArranjo* __fastcall TMontaRede::InsereArranjo(strARRANJO2 &str)
{
	// variáveis locais
	int dim, cabo_id, pto_fixacao_id;
	VTArranjo *arranjo, *clone;
	VTCabo *cabo;
	VTPtoFixacao *pto_fixacao;
	VTSuporte *suporte;

	try
	{ // proteção
		if (arranjos == NULL)
			return (NULL);
		// verifica se existe o Arranjo
		if (str.arranjo_id > 0)
		{ // procura Arranjo por ID
			if ((arranjo = arranjos->ExisteArranjo(str.arranjo_id)) != NULL)
				return (arranjo);
		}
		// validação dados de arranjo
		if (valida && valida->Enabled)
			valida->ValidaArranjo(str);
		// verifica se data do Arranjo está definida
		if (IsDoubleZero(str.data))
			str.data = DateOf(Now());
		// cria um novo objeto Arranjo
		if ((arranjo = DLL_NewObjArranjo()) == NULL)
			return (NULL);
		// arranjo->Id        = str.arranjo_id;
		// arranjo->Id        = (str.arranjo_id > 0) ? str.arranjo_id : FalsoId();
		arranjo->Id = str.arranjo_id;
		arranjo->Codigo = str.codigo;
		arranjo->Extern_id = str.extern_id;
		arranjo->Owner = NULL;
		arranjo->Data = str.data;
		arranjo->Fases = str.fases;
		arranjo->TipoCfg = str.tipo_cfg;
		arranjo->ramal = str.ramal;
		arranjo->ResTerra = str.res_terra;
		arranjo->Vmin_KV = str.vmin_kv;
		arranjo->Vmax_KV = str.vmax_kv;
		arranjo->Color = str.color;
		// PRECISA salvar o status indicado na estrutura por causa da leitura da base de dados
		// VERIFICAR importadores
		// arranjo->StatusAsUnsigned = sttNOVO;
		arranjo->StatusAsUnsigned = str.status;
		// determina Suporte (se definido)
		// verifica o tipo de configuração
		switch (str.tipo_cfg)
		{
		case arranjoCABOxFASE:
			// determina Suporte
			if ((suporte = arranjos->ExisteSuporte(str.cfg_suporte_cabo.suporte_id)) == NULL)
			{
				delete arranjo;
				return (NULL);
			}
			// define Suporte
			arranjo->suporte = suporte;
			// determina Cabos e PtoFixao no Suporte
			for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
			{ // verifica se o Cabo está definido p/ a fase
				if (str.cfg_suporte_cabo.fase[ind_fase].pto_fixacao_id < 0)
					continue;
				// determina o Cabo
				// if ((cabo = arranjos->ExisteCabo(str.cfg_suporte_cabo.fase[nf].cabo_id)) == NULL)
				cabo_id = str.cfg_suporte_cabo.fase[ind_fase].cabo_id;
				if ((cabo = arranjos->ExisteCabo(cabo_id)) == NULL)
				{
					delete arranjo;
					return (NULL);
				}
				// determina PtoFixacao no Suporte
				pto_fixacao_id = str.cfg_suporte_cabo.fase[ind_fase].pto_fixacao_id;
				if ((pto_fixacao = suporte->ExistePtoFixacao(pto_fixacao_id)) == NULL)
				{
					delete arranjo;
					return (NULL);
				}
				// define PtoFixacao e Cabo p/ fase do Arranjo
				arranjo->DefPtoFixacaoFase(ind_fase, pto_fixacao);
				arranjo->DefCaboFase(ind_fase, cabo);
			}
			break;
		case arranjoCABO_Z0Z1:
			// determina o Cabo
			if ((cabo = arranjos->ExisteCabo(str.cfg_caboZOZ1.cabo_id)) == NULL)
			{
				delete arranjo;
				return (NULL);
			}
			// define CaboZ0Z1
			arranjo->caboZ0Z1 = cabo;
			// determina Suporte (opcional => pode ser NULL)
			arranjo->suporte = arranjos->ExisteSuporte(str.cfg_caboZOZ1.suporte_id);
			break;
		case arranjoZ0Z1:
			arranjo->z0 = str.cfg_arranjoZ0Z1.z0;
			arranjo->z1 = str.cfg_arranjoZ0Z1.z1;
			arranjo->c0 = str.cfg_arranjoZ0Z1.c0;
			arranjo->c1 = str.cfg_arranjoZ0Z1.c1;
			// define corrente admissível
			arranjo->Iadm_amp = str.cfg_arranjoZ0Z1.iadm_amp;
			// determina Suporte (opcional => pode ser NULL)
			arranjo->suporte = arranjos->ExisteSuporte(str.cfg_arranjoZ0Z1.suporte_id);
			break;
		case arranjoMAT_Z:
			arranjo->Iadm_amp = str.cfg_matZ.iadm_amp;
			// inicia MatZ
			dim = arranjo->MatZ->Dimensao;
			for (int i = 0; i < dim; i++)
			{
				for (int j = 0; j < dim; j++)
				{
					arranjo->MatZ->R[i][j] = str.cfg_matZ.mat_z[i][j].r;
					arranjo->MatZ->X[i][j] = str.cfg_matZ.mat_z[i][j].x;
				}
			}
			// inicia MatC
			dim = arranjo->MatC->Dimensao;
			for (int i = 0; i < dim; i++)
			{
				for (int j = 0; j < dim; j++)
				{
					arranjo->MatC->Cap[i][j] = str.cfg_matZ.mat_c[i][j];
				}
			}
			// define Matz como montada
			arranjo->MatZ->Montada = true;
			break;
		case arranjoPU_SB100: // na base de dados, z0 e z1 estão em ohm/km
			arranjo->Iadm_amp = str.cfg_puSB100.iadm_amp;
			arranjo->z0 = str.cfg_puSB100.z0;
			arranjo->z1 = str.cfg_puSB100.z1;
			arranjo->c0 = str.cfg_puSB100.c0;
			arranjo->c1 = str.cfg_puSB100.c1;
			// determina Suporte (opcional => pode ser NULL)
			arranjo->suporte = arranjos->ExisteSuporte(str.cfg_puSB100.suporte_id);
			break;
		default:
			delete arranjo;
			return (NULL);
		}
		// monta matriz de impedâncias
		if (!montaZ->Executa(arranjo))
		{ // destrói Arranjo
			delete arranjo;
			return (NULL);
		}
		// verifica se é um Arranjo típico
		if (str.tipico)
			arranjos->InsereArranjo(arranjo);
	}
	catch (Exception &e)
	{
		arranjo = NULL;
	}
	return (arranjo);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereBarraEqpto(strBARRA &str)
{
	// variáveis locais
	VTBarra *pbarra;
	VTComentario *comentario;
	int delta_x, delta_y;
	/*
	 //assume que as Barras inseridas dentro de um bloco InsereBarraStart/InsereBarraStop tem ID único
	 try{//verifica se já existe em VTRedes
	 if ((pbarra = redes->ExisteBarra(str.bar_id)) != NULL)
	 {//verifica se tem as mesmas coordenadas UTM
	 delta_x = pbarra->utm.x - str.utm.x;
	 delta_y = pbarra->utm.y - str.utm.y;
	 if((delta_x < 100.) && (delta_y < 100.))
	 {//insere Barra na lista local lisBAR
	 if (lisBAR->IndexOf(pbarra) < 0) lisBAR->Add(pbarra);
	 return(pbarra);
	 }
	 }
	 */
	// assume que as Barras inseridas dentro de um bloco InsereBarraStart/InsereBarraStop tem ID único
	try
	{ // verifica se já existe em VTRedes
		if ((pbarra = redes->ExisteBarra(str.bar_id)) != NULL)
			return (pbarra);
		// cria um novo Eqpto
		if ((pbarra = DLL_NewObjBarra(patamares->NumPatamar())) == NULL)
			return (NULL);
		// inicia dados do Eqpto
		// pbarra->Id        = (str.bar_id > 0) ? str.bar_id : FalsoId();
		pbarra->Id = (str.bar_id == 0) ? FalsoId() : str.bar_id;
		pbarra->Codigo = str.codigo;
		pbarra->Extern_id = str.extern_id;
		pbarra->ShowAsUnsigned = str.show;
		pbarra->vnom = str.vnom;
		// pbarra->ShowAsUnsigned = str.show;
		pbarra->utm = str.utm;
		pbarra->esq = str.esq;
		// pbarra->index          = str.index;
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(pbarra)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		// NÃO insere Eqpto em Redes: isso será feito em InsereBarraStop()
		// redes->InsereBarra(pbarra);
		// insere Barra apenas na lista local lisBAR
		lisBAR->Add(pbarra);
		// define código p/ a Barra
		// if (pbarra->Codigo.IsEmpty()) pbarra->Codigo = "B_" + IntToStr(redes->LisBarra()->Count);
		if (pbarra->Codigo.IsEmpty())
			pbarra->Codigo = "B_" + IntToStr(lisBAR->Count); // DVK 2014.02.25
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (pbarra);
}

// -----------------------------------------------------------------------------
void __fastcall TMontaRede::InsereBarraStart(void)
{
	// reinicia lista localde Barras
	lisBAR->Clear();
	// finaliza eventual inserção de Barra anterior
	// InsereBarraStop();
}

// -----------------------------------------------------------------------------
void __fastcall TMontaRede::InsereBarraStop(void)
{
	// insere Barras armazenadas em lisBAR local em VTRedes
	for (int n = 0; n < lisBAR->Count; n++)
	{
		redes->InsereBarra((VTBarra*)lisBAR->Items[n]);
	}
	// reinicia lista local de Barras
	// lisBAR->Clear();
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereBateria(strBATERIA &str)
   {
   //variáveis locais
   VTBarra      *pbarra;
   VTBateria    *pbateria;
   VTCurva		*curva;
   VTEqbar      *eqbar;

	try
	{
	  //determina Barra da Bateria
	  //procura Barra pelo seu ID
	  if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
	  {
		if (plog)
		{
			plog->Add("Não existe Barra da Bateria " + AnsiQuotedStr(str.codigo, '\''));
			return(NULL);
        }
	  }
	  //verifica se a Bateria já existe
	  if ((eqbar = pbarra->ExisteEqbar(eqptoBATERIA, str.bat_id)) != NULL)
			return (eqbar);
	  //cria uma nova Bateria
		if ((pbateria = DLL_NewObjBateria(patamares->NumPatamar())) == NULL)
			return (NULL);
	  //inicia dados da Bateria
	  pbateria->Id                 = str.bat_id;
	  pbateria->Codigo             = str.codigo;
	  pbateria->Extern_id          = str.extern_id;
	  pbateria->Fases 		       = str.fases;
	  pbateria->Tipo_operacao      = str.tipo_operacao;
	  pbateria->Pnom_kw		       = str.p_nom_kw;
	  pbateria->Enom_kwh           = str.e_nom_kwh;
	  pbateria->Cap_inv_kva        = str.cap_inv_kva;
	  pbateria->P_ociosa_pu        = str.p_ociosa_pu;
	  pbateria->Q_ociosa_pu        = str.q_ociosa_pu;
	  pbateria->Rend_carga_pu      = str.rend_carga_pu;
	  pbateria->Rend_desc_pu       = str.rend_desc_pu;
	  pbateria->Gatilho_carga_pu   = str.gatilho_carga_pu;
	  pbateria->Gatilho_desc_pu	   = str.gatilho_desc_pu;
	  //modelo de carga e descarga
	  if (! IsDoubleZero(1 - (str.modelo.icte_pu + str.modelo.scte_pu + str.modelo.zcte_pu)))
		 {
		 str.modelo.icte_pu = 1.;
		 str.modelo.scte_pu = 0.;
         str.modelo.zcte_pu = 0.;
         }
	  pbateria->Icte_pu    = str.modelo.icte_pu;
	  pbateria->Scte_pu    = str.modelo.scte_pu;
	  pbateria->Zcte_pu    = str.modelo.zcte_pu;

	  //valida strESQ_EQBAR
	  ValidaEsquematicoEqbar(str.esq);
	  pbateria->esq        = str.esq;

	  //Define curva de carga e descarga para a bateria
	  curva = CriaCurvaBateria(str.curva);

	  //proteção: verifica se existe Curva da Carga
	  if (curva == NULL)
         {
			if (plog)
				plog->Add("Não existe Curva de carga e descarga " + AnsiQuotedStr(str.codigo,
			'\''));
		 //destrói objeto
		 DLL_DeleteEqpto(pbateria);
		 return(NULL);
		 }

	  //associa Bateria c/ Barra e Curva
	  pbateria->DefineObjBarra(pbarra);
	  pbateria->DefineCurva(curva);

	  //insere Carga na Barra
      pbarra->InsereEqbar(pbateria);
	}
	catch (Exception &e)
		{//nada a fazer
		return(NULL);
		}
   return(pbateria);

   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereCabo(strCABO &str)
{
	// variáveis locais
	VTCabo *cabo;

	try
	{ // proteção
		if (arranjos == NULL)
			return (NULL);
		// verifica se existe o Cabo
		if (str.id > 0)
		{ // procura Cabo por ID
			if ((cabo = arranjos->ExisteCabo(str.id)) != NULL)
				return (cabo);
		}
		// valida atributos de cabo
		if ((valida) && valida->Enabled)
			valida->ValidaCabo(str);
		// valida parâmetros
		switch (str.tipo_cfg)
		{
		case caboR_GMR:
			if (!ValidaCaboR_GMR(str))
				return (NULL);
			break;
		case caboZ0Z1:
			if (!ValidaCaboZOZ1(str))
				return (NULL);
			break;
		default:
			return (NULL);
		}
		// cria um novo Eqpto
		if ((cabo = DLL_NewObjCabo()) == NULL)
			return (NULL);
		// define Cabo como novo
		// cabo->Status[sttNOVO] = true;
		// copia dados do Cabo
		cabo->Id = str.id;
		cabo->Codigo = str.codigo;
		cabo->Extern_id = str.extern_id;
		cabo->TipoCfg = str.tipo_cfg;
		cabo->Iadm_A = str.iadm;
		cabo->Vmax_KV = str.vmin;
		cabo->Vmin_KV = str.vmax;
		cabo->r = str.r;
		cabo->gmr = str.gmr;
		cabo->z0 = str.z0;
		cabo->c0 = str.c0;
		cabo->z1 = str.z1;
		cabo->c1 = str.c1;
		cabo->StatusAsUnsigned = str.status;
		// insere Cabo em Arranjos
		arranjos->InsereCabo(cabo);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (cabo);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereCapacitor(strCAPACITOR &str)
{
	// variáveis locais
	VTBarra *pbarra;
	VTCapacitor *pcap;
	VTCurva *curva;
	VTEqbar *eqbar;
	VTComentario *comentario;

	try
	{ // valida atributos de capacitor
		if ((valida) && valida->Enabled)
			valida->ValidaCapacitor(str);
		// determina Barra do Capacitor
		if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe Barra do Capacitor " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// verifica se Capacitor definido por Curva típica
		if (str.curva.tipica)
		{ // verifica se Curva típica já definida por ID
			if ((curva = curvas->ExisteCurvaTipicaCapacitor(str.curva.id)) == NULL)
			{ // seleciona uma Curva típica
				curva = curvas->ExisteCurvaTipicaCapacitor();
			}
			if (curva == NULL)
			{
				if (plog)
					plog->Add("Não existe curva típica do Capacitor " +
					AnsiQuotedStr(str.codigo, '\''));
				return (NULL);
			}
		}
		else
		{
			if ((curva = CriaCurvaCapacitor(str.curva)) == NULL)
			{
				if (plog)
					plog->Add("Não foi possível criar curva do Capacitor " +
					AnsiQuotedStr(str.codigo, '\''));
				return (NULL);
			}
		}
		// verifica se o Capacitor já existe
		if ((eqbar = pbarra->ExisteEqbar(eqptoCAPACITOR, str.cap_id)) != NULL)
			return (eqbar);
		// cria um novo Eqpto
		if ((pcap = DLL_NewObjCapacitor(patamares->NumPatamar())) == NULL)
			return (NULL);
		// inicia dados do Eqpto
		pcap->Id = str.cap_id;
		pcap->Codigo = str.codigo;
		pcap->Extern_id = str.codigo;
		pcap->ShowAsUnsigned = str.show;
		pcap->StatusAsUnsigned = str.status;
		pcap->vnom = str.vnom;
		// pcap->ligacao        = lgTRIANG;
		pcap->ligacao = str.ligacao;
		pcap->q = str.q_mvar;
		// valida strESQ_EQBAR
		ValidaEsquematicoEqbar(str.esq);
		pcap->esq = str.esq;
		// verifica tensão nominal em relação à barra
		if (!IsDoubleZero(pcap->vnom - pbarra->vnom))
			pcap->vnom = pbarra->vnom;
		// define Curva
		pcap->DefineCurva(curva);
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(pcap)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		// insere Eqpbar na Barra
		pbarra->InsereEqbar(pcap);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (pcap);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereCapserie(strCAPSERIE &str)
{
	// variáveis locais
	VTCapserie *capserie;
	VTBarra *pbar1, *pbar2;
	VTRede *rede;
	VTComentario *comentario;

	try
	{ // determina Rede da Ligacao
		if ((rede = redes->ExisteRede(str.rede_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe Rede do Capacitor Série " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// determina Barras da Ligacao
		pbar1 = ExisteBarra(str.bar1_id);
		pbar2 = ExisteBarra(str.bar2_id);
		if ((pbar1 == NULL) || (pbar2 == NULL))
		{
			if (plog)
				plog->Add("Não existe Barra do Capacitor Série " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		if (pbar1 == pbar2)
		{
			if (plog)
				plog->Add("Capacitor Série com barras iguais: " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// cria um novo Eqpto
		if ((capserie = DLL_NewObjCapserie(patamares->NumPatamar())) == NULL)
			return (NULL);
		// inicia dados do Eqpto
		capserie->Id = str.id;
		capserie->Codigo = str.codigo;
		capserie->Extern_id = str.extern_id;
		capserie->ShowAsUnsigned = str.show;
		capserie->snom = str.snom;
		capserie->vnom = str.vnom;
		capserie->z = str.z;
		// valida strESQ_LIGAR
		ValidaEsquematicoLigacao(str.esq);
		capserie->esq = str.esq;
		// define Barras da Ligacao
		capserie->DefineObjBarra(pbar1, pbar2);
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(capserie)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		// insere Ligacao na Rede
		rede->InsereLigacao(capserie);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (capserie);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereCarga(strCARGA &str)
{
	// variáveis locais
	VTBarra *pbarra;
	VTCarga *pcarga;
	VTClasse *classe;
	VTCurva *curva;
	VTEqbar *eqbar;
	VTComentario *comentario;

	try
	{ // valida atributos de carga
		if ((valida) && valida->Enabled)
		{
			if (str.classe_cons == consA4)
				valida->ValidaCargaA(str);
			else if (str.classe_cons == consEQUIVALENTE)
				valida->ValidaCargaEQV(str);
			else
				valida->ValidaCargaB(str);
		}
		// determina Barra do Carga
		if ((pbarra = str.obj.barra) == NULL)
		{ // procura Barra pelo seu ID
			if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
			{
				if (plog)
					plog->Add("Não existe Barra da Carga " + AnsiQuotedStr(str.codigo, '\''));
				return (NULL);
			}
		}
		// determina Classe de consumidor da Carga
		if ((classe = ExisteClasse(str)) == NULL)
		{
			if (plog)
				plog->Add("Não existe classe de consumidor da Carga " +
				AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// verifica se a Carga já existe
		if ((eqbar = pbarra->ExisteEqbar(eqptoCARGA, str.car_id)) != NULL)
			return (eqbar);
		// cria uma nova Carga
		if ((pcarga = DLL_NewObjCarga(patamares->NumPatamar())) == NULL)
			return (NULL);
		// inicia dados da Carga
		pcarga->Id = str.car_id;
		pcarga->Codigo = str.codigo;
		pcarga->Extern_id = str.extern_id;
		pcarga->ShowAsUnsigned = str.show;
		pcarga->VeryImportant = str.vip;
		pcarga->GrupoA = str.grupoA;
		pcarga->Energia_mwhmes = str.energia_mwhmes;
		pcarga->Fases = str.fases;
		pcarga->IP_NumLamp = str.ip.num_lamp;
		pcarga->IP_Pot_kw = str.ip.pot_kw;
		pcarga->IP_Fatpot = str.ip.fat_pot;
		// modelo de carga
		if (!IsDoubleZero(1 - (str.modelo.icte_pu + str.modelo.scte_pu + str.modelo.zcte_pu)))
		{
			str.modelo.icte_pu = 1.;
			str.modelo.scte_pu = 0.;
			str.modelo.zcte_pu = 0.;
		}
		pcarga->Icte_pu = str.modelo.icte_pu;
		pcarga->Scte_pu = str.modelo.scte_pu;
		pcarga->Zcte_pu = str.modelo.zcte_pu;
		// número de consumidores para Carga equivalente
		// if (str.classe_cons == consEQUIVALENTE)
		// {
		// pcarga->NumConsResidencial = str.num_cons.residencial;
		// pcarga->NumConsComercial   = str.num_cons.comercial;
		// pcarga->NumConsIndustrial  = str.num_cons.industrial;
		// pcarga->NumConsRural       = str.num_cons.rural;
		// pcarga->NumConsOutros      = str.num_cons.outros;
		// pcarga->NumConsA4          = str.num_cons.a4;
		// }
		switch (str.classe_cons)
		{
		case consEQUIVALENTE:
			pcarga->NumConsResidencial = str.num_cons.residencial;
			pcarga->NumConsComercial = str.num_cons.comercial;
			pcarga->NumConsIndustrial = str.num_cons.industrial;
			pcarga->NumConsRural = str.num_cons.rural;
			pcarga->NumConsOutros = str.num_cons.outros;
			pcarga->NumConsA4 = str.num_cons.a4;
			break;
		case consRESIDENCIAL:
			pcarga->NumConsResidencial = 1;
			break;
		case consCOMERCIAL:
			pcarga->NumConsComercial = 1;
			break;
		case consINDUSTRIAL:
			pcarga->NumConsIndustrial = 1;
			break;
		case consRURAL:
			pcarga->NumConsRural = 1;
			break;
		case consA4:
			pcarga->NumConsA4 = 1;
			break;
		case consOUTROS:
			pcarga->NumConsOutros = 1;
			break;
		case consIP:
			break;
		default: ;
		}

		// valida strESQ_EQBAR
		ValidaEsquematicoEqbar(str.esq);
		pcarga->esq = str.esq;
		// verifica se a Curva está indicada em str
		if ((curva = str.curva_tipica) == NULL)
		{ // verifica se Carga de IP
			if (str.classe_cons == consIP)
			{ // determina Curva típica de IP
				curva = curvas->ExisteCurvaTipicaIP();
			}
			// verifica se Carga definida por curva típica ou curva própria
			else if (str.curva.tipica)
			{ // determina Curva típica por classe/consumo
				// curva = curvas->ExisteCurvaTipicaCarga(classe, str.energia_mwhmes * 1000.);
				curva = curvas->ExisteCurvaTipicaCarga(classe, pcarga->Energia_kwhmes);
			}
			else
			{ // carga definida por Curva própria: cria Curva própria
				curva = CriaCurvaCarga(str.curva);
			}
		}
		// proteção: verifica se existe Curva da Carga
		if (curva == NULL)
		{
			if (plog)
				plog->Add("Não existe Curva de demanda da Carga " + AnsiQuotedStr(str.codigo,
			'\''));
			// destrói objeto
			DLL_DeleteEqpto(pcarga);
			return (NULL);
		}
		// associa Carga c/ Barra, Classe e Curva
		pcarga->DefineObjBarra(pbarra);
		pcarga->DefineClasse(classe);
		pcarga->DefineCurva(curva);
		// verifica carga  de IP com potência nula da lâmpada
		if (str.classe_cons == consIP)
		{
			if (IsDoubleZero(pcarga->IP_Pot_kw))
			{
				if (plog)
					plog->Add("Potência nula para Carga de IP " + AnsiQuotedStr(str.codigo, '\''));
			}
		}
		// JCG 2017.01.20 - não define Crescimento de Mercado - isso passou a ser feito pelo Topologia
		// mercado->DefineCrescimento(pcarga, redes);
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(pcarga)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		pcarga->Categoria = str.categoria;
		// insere Carga na Barra
		pbarra->InsereEqbar(pcarga);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (pcarga);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereCargaNL(strCARGANL &str)
{
	// variáveis locais
	VTBarra *pbarra;
	VTCargaNL *pcargaNL;
	VTEqbar *eqbar;
	VTComentario *comentario;
	struct strFUN fun;
	struct strHRM hrm;

	try
	{ // determina Barra do Carga
		if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe Barra da CargaNL " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// Verifica se trata-se de uma nova carga (fundamental) ou se é uma harmônica
		// Verifica se a potência é em kVA (poderia ser verificada a ordem)
		if (str.flag_kVA)
		{
			//// É a fundamental, verifica se a CargaNL já existe
			if (pbarra->ExisteEqbar(eqptoCARGANL, str.car_id) != NULL)
				return (NULL);
			// cria a carga e salva os parâmetros que interessam
			if ((pcargaNL = DLL_NewObjCargaNL()) == NULL)
				return (NULL);
			// inicia dados da Carga
			pcargaNL->Id = str.car_id;
			pcargaNL->Codigo = str.codigo;
			pcargaNL->Extern_id = str.extern_id;
			pcargaNL->ShowAsUnsigned = str.show;
			// pcargaNL->FatPot_pu  = str.fat_pot;
			// pcargaNL->Indutiva   = str.flag_indutivo;
			// Dados da fundamental
			fun.indutiva = str.flag_indutivo;
			fun.fatpot_pu = str.fat_pot;
			fun.sa_mod_kva = str.snomA;
			fun.sb_mod_kva = str.snomB;
			fun.sc_mod_kva = str.snomC;
			fun.sa_ang_grau = str.angA;
			fun.sb_ang_grau = str.angB;
			fun.sc_ang_grau = str.angC;
			pcargaNL->DefineFundamental(&fun);

			// valida strESQ_EQBAR
			ValidaEsquematicoEqbar(str.esq);
			pcargaNL->esq = str.esq;

			// associa Carga c/ Barra, Classe e Curva
			pcargaNL->DefineObjBarra(pbarra);
			// insere Carga na Barra
			pbarra->InsereEqbar(pcargaNL);
		}
		else
		{
			// Trata-se de uma harmônica, então procura a carga com o mesmo id
			if ((pcargaNL = ExisteCargaNL(pbarra, str.car_id)) == NULL)
				return (NULL);
			// Carrega os dados da harmônica
			hrm.ordem = str.ordem;
			hrm.sa_mod_pu = str.snomA;
			hrm.sb_mod_pu = str.snomB;
			hrm.sc_mod_pu = str.snomC;
			hrm.sa_ang_grau = str.angA;
			hrm.sb_ang_grau = str.angB;
			hrm.sc_ang_grau = str.angC;
			if (!pcargaNL->InsereHarmonica(&hrm))
				return (NULL);
		}
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (pcargaNL);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereChave(strCHAVE &str)
{
	// variáveis locais
	VTChave *pchave;
	VTBarra *pbar1, *pbar2;
	VTRede *rede;
	VTComentario *comentario;

	try
	{ // determina Rede da Ligacao
		if ((rede = redes->ExisteRede(str.rede_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe Rede da Chave " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// valida atributos de chave
		if ((valida) && valida->Enabled)
			valida->ValidaChave(str);
		// determina Barras da Ligacao
		pbar1 = ExisteBarra(str.bar1_id);
		pbar2 = ExisteBarra(str.bar2_id);
		if ((pbar1 == NULL) || (pbar2 == NULL))
		{
			if (plog)
				plog->Add("Não existe Barra da Chave " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		if (pbar1 == pbar2)
		{
			if (plog)
				plog->Add("Chave com barras iguais: " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// cria um novo Eqpto
		if ((pchave = DLL_NewObjChave(patamares->NumPatamar())) == NULL)
			return (NULL);
		// inicia dados do Eqpto
		pchave->Id = str.chv_id;
		pchave->Codigo = str.codigo;
		pchave->Extern_id = str.extern_id;
		pchave->EloFusivel = str.elo_fusivel;
		pchave->TipoGIS = str.tipo_gis;
		pchave->ShowAsUnsigned = str.show;
		pchave->Inom_amp = str.corn;
		pchave->Estado = (str.estado == chvABERTA) ? chvABERTA : chvFECHADA;
		// JCG 2015.07.17 - isso precisará ser alterado
		pchave->EstadoNormal = pchave->Estado;
		//
		pchave->ProtecaoAtiva = str.protecao_ativa;
		pchave->Telecomandada = str.telecomandada;
		pchave->TecnoComunicacao = str.tecno_comunicacao;

		// valida strESQ_LIGAR
		ValidaEsquematicoLigacao(str.esq);
		pchave->esq = str.esq;
		// determina TipoChave
		pchave->TipoChave = ExisteTipoChave(str.tipochave.id, str.tipochave.tipo,
			str.tipochave.codigo);
		if (pchave->TipoChave == NULL)
		{
			if (plog)
				plog->Add("Não existe tipo da chave " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// define Barras da Ligacao
		pchave->DefineObjBarra(pbar1, pbar2);
		// TESTES
		// ordena barras da Chave por ordem alfabética
		// OrdenaBarraPorNome(pchave);
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(pchave)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		// insere Ligacao na Rede
		rede->InsereLigacao(pchave);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (pchave);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereCluster(strCLUSTER &str)
{
	// variáveis locais
	VTCluster *cluster;

	// verifica se o ID é válido
	if (str.cluster_id > 0)
	{ // procura Cluster pelo ID
		cluster = redes->ExisteCluster(str.cluster_id);
	}
	else
	{ // procura Cluster pelo código
		cluster = redes->ExisteCluster(str.codigo);
	}
	// verifica se já existe o Cluster
	if (cluster == NULL)
	{ // cria um novo Cluster
		cluster = DLL_NewObjCluster();
		cluster->Id = str.cluster_id;
		cluster->Codigo = str.codigo;
		// insere Cluster em Redes
		redes->InsereCluster(cluster);
	}
	return (cluster);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereCorte(int corte_id, int mrede_id)
{
	// variáveis locais
	VTCorte *corte;
	VTMRede *mrede;

	// verifica se existe o Corte
	if ((corte = redes->ExisteCorte(corte_id)) == NULL)
	{ // cria um novo Corte
		if ((corte = DLL_NewObjCorte()) == NULL)
			return (NULL);
		corte->Id = corte_id;
		redes->InsereCorte(corte);
	}
	// verifica se existe a MRede
	if ((mrede = redes->ExisteMRede(mrede_id)) == NULL)
	{ // cria uma nova MRede
		if ((mrede = DLL_NewObjMRede()) == NULL)
			return (NULL);
		mrede->Id = mrede_id;
		redes->InsereMRede(mrede);
	}
	// associa MRede c/ Corte
	mrede->InsereCorte(corte);
	// associa Corte c/ Redes da MRede
	corte->InsereLisRede(mrede->LisRede());

	return (corte);
}

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaRede::InsereCurvaDemanda(VTCarga *carga, strCURVA &str)
{
	// variáveis locais
	VTCurva *curva;

	// cria Curva da carga
	if ((curva = CriaCurvaCarga(str)) != NULL)
	{ // associa Carga com Curva
		carga->DefineCurva(curva);
	}
	return (curva);
}

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaRede::InsereCurvaTipicaCapacitor(strCURVA_TIPICA &str)
{
	// variáveis locais
	VTCurva *curva;
	VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

	// verifica se existe o Cabo
	if (str.id > 0)
	{ // procura Curva por ID
		if ((curva = curvas->ExisteCurvaTipicaCapacitor(str.id)) != NULL)
			return (curva);
	}
	// cria uma nova Curva
	curva = CriaCurvaCapacitor(str);
	// insere VTCurva em VTCurvas
	curvas->InsereCurvaTipicaCapacitor(curva);
	return (curva);
}

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaRede::InsereCurvaTipicaCarga(strCURVA_TIPICA &str)
{
	// variáveis locais
	VTCurva *curva;
	VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

	// verifica se existe o Cabo
	if (str.id > 0)
	{ // procura Curva por ID
		if ((curva = curvas->ExisteCurvaTipicaCarga(str.id)) != NULL)
			return (curva);
	}
	// cria uma nova Curva
	if ((curva = CriaCurvaCarga(str)) != NULL)
	{ // define atributos adicionais específicos
		curva->DiaMedicao = str.dia;
		curva->Tipo = str.tipo;
		curva->Unidade = str.unidade; // unidPU_MED;
		curva->Tipica = true;
		curva->Classe = classes->ExisteClasseTag(str.classe_tag);
		curva->LimiteInferior = str.energia_min_kwhmes;
		curva->LimiteSuperior = str.energia_max_kwhmes;
		// insere VTCurva em VTCurvas
		curvas->InsereCurvaTipicaCarga(curva);
	}
	return (curva);
}

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaRede::InsereCurvaTipicaGerador(strCURVA_TIPICA &str)
{
	// variáveis locais
	VTCurva *curva;
	VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

	// verifica se existe o Cabo
	if (str.id > 0)
	{ // procura Curva por ID
		if ((curva = curvas->ExisteCurvaTipicaGeradorFotovoltaicoId(str.id)) != NULL)
			return (curva);
	}
	// cria uma nova Curva
	if ((curva = CriaCurvaGerador(str)) != NULL)
	{ // insere VTCurva em VTCurvas
		curvas->InsereCurvaTipicaGeradorFotovoltaico(curva);
	}
	return (curva);
}

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaRede::InsereCurvaTipicaIP(strCURVA_TIPICA &str)
{
	// variáveis locais
	VTCurva *curva;
	VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

	// verifica se existe o Cabo
	if (str.id > 0)
	{ // procura Curva por ID
		if ((curva = curvas->ExisteCurvaTipicaIP(str.id)) != NULL)
			return (curva);
	}
	// cria uma nova Curva
	if ((curva = CriaCurvaCarga(str)) != NULL)
	{ // define atributos específicos
		curva->Tipica = true;
		curva->Tipo = str.tipo;
		curva->Unidade = unidON_OFF;
		curva->Classe = classes->ExisteClasseTag(consIP);
		curva->DiaMedicao = SEMANA_COMPLETA;
		// insere VTCurva em VTCurvas
		curvas->InsereCurvaTipicaIP(curva);
	}
	return (curva);
}

// -----------------------------------------------------------------------------
VTCurva* __fastcall TMontaRede::InsereCurvaTipicaReator(strCURVA_TIPICA &str)
{
	// variáveis locais
	VTCurva *curva;
	VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

	// verifica se existe o Cabo
	if (str.id > 0)
	{ // procura Curva por ID
		if ((curva = curvas->ExisteCurvaTipicaReator(str.id)) != NULL)
			return (curva);
	}
	// cria uma nova Curva
	if ((curva = CriaCurvaReator(str)) != NULL)
	{ // insere VTCurva em VTCurvas
		curvas->InsereCurvaTipicaReator(curva);
	}
	return (curva);
}

// -----------------------------------------------------------------------------
VTDominio* __fastcall TMontaRede::InsereDominio(strDOMINIO &str)
{
	// variáveis locais
	VTDominio *dominio;

	try
	{ // verifica se já existe o Dominio
		if (str.dominio_id > 0)
			dominio = redes->ExisteDominio(str.dominio_id);
		else
			dominio = redes->ExisteDominio(str.codigo);
		if (dominio == NULL)
		{ // cria um novo Dominio
			dominio = DLL_NewObjDominio();
			dominio->Id = (str.dominio_id > 0) ? str.dominio_id : FalsoId();
			dominio->Codigo = str.codigo;
			dominio->DataCriacao = str.data_criacao;
			// insere Dominio em Redes
			redes->InsereDominio(dominio);
		}
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (dominio);
}

// -----------------------------------------------------------------------------
bool __fastcall TMontaRede::InsereEstudo(strPLAN_ACAO &str)
{
	return (false);
}

// -----------------------------------------------------------------------------
bool __fastcall TMontaRede::InsereEstudo(strPLAN_ALTERNATIVA &str)
{
	return (false);
}

// -----------------------------------------------------------------------------
VTEstudo* __fastcall TMontaRede::InsereEstudo(strPLAN_ESTUDO &str)
{
	return (NULL);
}

// -----------------------------------------------------------------------------
bool __fastcall TMontaRede::InsereEstudo(strPLAN_IPE &str)
{
	return (false);
}

// -----------------------------------------------------------------------------
bool __fastcall TMontaRede::InsereEstudo(strPLAN_OBRA &str)
{
	return (false);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereFiltro(strFILTRO &str)
{
	// variáveis locais
	VTBarra *pbarra;
	VTEqbar *eqbar;
	VTFiltro *filtro;
	VTComentario *comentario;

	try
	{ // determina Barra do Eqbar
		if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe Barra do Filtro " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// verifica se o Filtro já existe
		if ((eqbar = pbarra->ExisteEqbar(eqptoFILTRO, str.id)) != NULL)
			return (eqbar);
		// cria um novo Eqpto
		if ((filtro = DLL_NewObjFiltro()) == NULL)
			return (NULL);
		// inicia dados do Eqpto
		filtro->Id = str.id;
		filtro->Codigo = str.codigo;
		filtro->Extern_id = str.extern_id;
		filtro->ShowAsUnsigned = str.show;
		filtro->vnom = str.vnom;
		filtro->snom = str.snom;
		filtro->z0 = str.z0;
		// valida strESQ_EQBAR
		ValidaEsquematicoEqbar(str.esq);
		filtro->esq = str.esq;
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(filtro)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		// insere Eqpbar na Barra
		pbarra->InsereEqbar(filtro);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (filtro);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereGerador(strGERADOR &str)
{
	// variáveis locais
	VTBarra *pbarra;
	VTCurva *curva;
	VTEqbar *eqbar;
	VTGerador *pger;
	VTComentario *comentario;

	try
	{ // valida atributos de gerador
		if ((valida) && valida->Enabled)
			valida->ValidaGerador(str);
		// determina Barra do Gerador
		if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe Barra do Gerador " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// verifica se o Gerador já existe
		if ((eqbar = pbarra->ExisteEqbar(eqptoGERADOR, str.ger_id)) != NULL)
			return (eqbar);
		// cria um novo Eqpto
		if ((pger = DLL_NewObjGerador(patamares->NumPatamar())) == NULL)
			return (NULL);
		// inicia dados do Eqpto
		pger->Id = str.ger_id;
		pger->Codigo = str.codigo;
		pger->Extern_id = str.extern_id;
		pger->ShowAsUnsigned = str.show;
		pger->ligacao = str.ligacao;
		pger->Fases = str.fases;
		pger->vnom = str.vnom;
		pger->snom = str.snom_mva;
		pger->smax = str.smax_mva;
		pger->tiposup = str.tiposup;
		pger->FonteEnergia = str.tipo_geracao;
		pger->fplim = str.fplim;
		pger->z0.r = str.z0.r;
		pger->z0.x = str.z0.x;
		pger->z1.r = str.z1.r;
		pger->z1.x = str.z1.x;
		// valida strESQ_EQBAR
		ValidaEsquematicoEqbar(str.esq);
		pger->esq = str.esq;
		// verifica se o gerador está associado a uma curva típica
		if (str.curva.tipica)
		{ // obtém curva típica
			curva = curvas->ExisteCurvaTipicaGeradorFotovoltaicoId(str.curva.id);
		}
		else
		{ // cria Curva própria p/ o Gerador
			curva = CriaCurvaGerador(str.curva);
		}
		// proteção
		if (curva == NULL)
		{
			DLL_DeleteEqpto(pger);
			return (NULL);
		}
		pger->DefineCurva(curva);
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(pger)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		// insere Eqpbar na Barra
		pbarra->InsereEqbar(pger);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (pger);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereMedidor(strMEDIDOR &str)
{
	// variáveis locais
	VTBarra *pbarra;
	VTEqbar *eqbar;
	VTMedidor *medidor;
	VTComentario *comentario;

	try
	{ // determina Barra do Eqbar
		if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe Barra do Medidor " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// verifica se o Medidor já existe
		if ((eqbar = pbarra->ExisteEqbar(eqptoMEDIDOR, str.medidor_id)) != NULL)
			return (eqbar);
		// cria um novo Eqpto
		if ((medidor = DLL_NewObjMedidor()) == NULL)
			return (NULL);
		// inicia dados do Eqpto
		medidor->Id = str.medidor_id;
		medidor->Codigo = str.codigo;
		medidor->Extern_id = str.extern_id;
		medidor->ShowAsUnsigned = str.show;
		medidor->esq = str.esq;
		// valida strESQ_EQBAR
		ValidaEsquematicoEqbar(str.esq);
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(medidor)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		// insere Eqpbar na Barra
		pbarra->InsereEqbar(medidor);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (medidor);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereMedidorCanal(strCANAL &str)
{
	// variáveis locais
	VTCanal *canal;
	VTCurva *curva;
	VTEqpto *eqpto;
	VTMedidor *medidor;

	try
	{ // determina Medidor do Canal
		if ((eqpto = ExisteEqpto(eqptoMEDIDOR, str.medidor_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe Medidor do Canal " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		medidor = (VTMedidor*)eqpto;
		// determina Eqpto associado à medição
		if ((eqpto = ExisteEqpto(str.eqpto_medido.tipo, str.eqpto_medido.id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe Eqpto associado ao Canal " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// proteção: necessária p/ Barras de Corte
		// verifica se o Medidor já tem um Canal p/ o Eqpto
		if ((canal = medidor->ExisteCanal(eqpto, str.tipo_canal, str.eqpto_medido.fases)) != NULL)
			return (canal);
		// insere um novo Canal no medidor
		if ((canal = medidor->InsereCanal(eqpto, str.tipo_canal, str.eqpto_medido.fases)) == NULL)
			return (NULL);
		// inicia dados Canal
		canal->Id = str.canal_id;
		canal->Codigo = str.codigo;
		canal->Tolerancia_perc = str.tolerancia_perc;
		// cria Curva do Canal
		if ((curva = CriaCurvaCanal(str.curva, str.tipo_canal)) == NULL)
		{
			DLL_DeleteEqpto(canal);
			return (NULL);
		}
		// define Curva do Canal
		canal->Curva = curva;
	}
	catch (Exception &e)
	{
		canal = NULL;
	}
	return (canal);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereMutua(strMUTUA &str)
{
	// variáveis locais
	VTBarra *bar_tre1, *bar_tre2;
	VTTrecho *trecho1, *trecho2;
	VTMutua *mutua;
	VTRede *rede;
	VTComentario *comentario;

	try
	{ // determina Rede da Mutua
		if ((rede = redes->ExisteRede(str.rede_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe Rede da Mútua " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// determina Trechos da Mutua
		trecho1 = (VTTrecho*)rede->ExisteLigacao(eqptoTRECHO, str.tre1_id);
		trecho2 = (VTTrecho*)rede->ExisteLigacao(eqptoTRECHO, str.tre2_id);
		if ((trecho1 == NULL) || (trecho2 == NULL))
		{
			if (plog)
				plog->Add("Não existem Trechos do Mútua " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// determina Barras de referências da Mutua
		// bar_tre1 = (str.sentido_tre1 == sentidoB1B2) ? trecho1->pbarra1 : trecho1->pbarra2;
		// bar_tre2 = (str.sentido_tre2 == sentidoB1B2) ? trecho2->pbarra1 : trecho2->pbarra2;
		bar_tre1 = (str.tre1_bar_id == trecho1->pbarra2->Id) ? trecho1->pbarra2 : trecho1->pbarra1;
		bar_tre2 = (str.tre2_bar_id == trecho2->pbarra2->Id) ? trecho2->pbarra2 : trecho2->pbarra1;
		// se necessário, inverte as Barras dos Trecho1 p/ coincidir com as Barras de de referência
		if (bar_tre1 == trecho1->pbarra2)
		{
			trecho1->pbarra2 = trecho1->pbarra1;
			trecho1->pbarra1 = bar_tre1;
		}
		// se necessário, inverte as Barras dos Trecho2 p/ coincidir com as Barras de de referência
		if (bar_tre2 == trecho2->pbarra2)
		{
			trecho2->pbarra2 = trecho2->pbarra1;
			trecho2->pbarra1 = bar_tre2;
		}
		// cria um novo Eqpto
		if ((mutua = DLL_NewObjMutua()) == NULL)
			return (NULL);
		// inicia dados do Eqpto
		mutua->Id = str.id;
		mutua->Codigo = str.codigo;
		mutua->Extern_id = str.extern_id;
		mutua->ShowAsUnsigned = str.show;
		mutua->ptre1 = trecho1;
		mutua->pbar_tre1 = bar_tre1;
		mutua->ptre2 = trecho2;
		mutua->pbar_tre2 = bar_tre2;
		mutua->z0 = str.z0;
		mutua->z1 = str.z1;
		mutua->postre1 = str.postre1;
		mutua->postre2 = str.postre2;
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(mutua)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		// insere Mutua na Rede
		rede->InsereMutua(mutua);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (mutua);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereReator(strREATOR &str)
{
	// variáveis locais
	VTBarra *pbarra;
	VTCurva *curva;
	VTEqbar *eqbar;
	VTReator *reator;
	VTComentario *comentario;

	try
	{ // determina Barra do Eqbar
		if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe Barra do Reator " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// verifica se o Reator já existe
		if ((eqbar = pbarra->ExisteEqbar(eqptoREATOR, str.id)) != NULL)
			return (eqbar);
		// verifica se Reator definido por Curva típica
		if (str.curva.tipica)
		{ // verifica se existe Curva típica já existe
			if ((curva = curvas->ExisteCurvaTipicaReator(str.curva.id)) == NULL)
			{ // assume uma Curva típica
				curva = curvas->ExisteCurvaTipicaReator();
			}
			if (curva == NULL)
			{
				if (plog)
					plog->Add("Não existe curva típica do Reator " +
					AnsiQuotedStr(str.codigo, '\''));
				return (NULL);
			}
		}
		else
		{
			if ((curva = CriaCurvaReator(str.curva)) == NULL)
			{
				if (plog)
					plog->Add("Não foi possível criar curva do Reator " +
					AnsiQuotedStr(str.codigo, '\''));
				return (NULL);
			}
		}
		// cria um novo Eqpto
		if ((reator = DLL_NewObjReator(patamares->NumPatamar())) == NULL)
			return (NULL);
		// inicia dados do Eqpto
		reator->Id = str.id;
		reator->Codigo = str.codigo;
		reator->Extern_id = str.extern_id;
		reator->ShowAsUnsigned = str.show;
		reator->ligacao = str.ligacao;
		reator->vnom = str.vnom;
		reator->q = str.q_mvar;
		// valida strESQ_EQBAR
		ValidaEsquematicoEqbar(str.esq);
		reator->esq = str.esq;
		// define Curva típica
		reator->DefineCurva(curva);
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(reator)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		// insere Eqpbar na Barra
		pbarra->InsereEqbar(reator);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (reator);
}

// -----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16666485
//https://sinapsisenergia.teamwork.com/#/tasks/18761472
VTEqpto* __fastcall TMontaRede::InsereRede(strREDE &str)
{
	// variáveis locais
	VTArea *area_esq;
	VTBarra *barra;
	VTMRede *mrede = NULL;
	VTRede *rede;
	VTCluster *cluster;
	VTDominio *dominio;
	VTComentario *comentario;
    VTCurva *curva;

	// reinicia controle de inserção de Barras
	// InsereBarraStart();
	// verifica se pertence a um Dominio;
	// dominio = NULL;
	// if (str.dominio_id > 0) dominio = redes->ExisteDominio(str.dominio_id);
	dominio = redes->ExisteDominio(str.dominio_id);
	// verifica se Rede já existe
	if ((rede = redes->ExisteRede(str.rede_id)) != NULL)
	{ // insere Rede no Dominio
		if (dominio != NULL)
			dominio->InsereRede(rede);
		return (rede);
	}
	// verifica se pertence a um Cluster;
	cluster = redes->ExisteCluster(str.cluster_id);
	// verifica se está definida a MRede
	if (str.mrede_id > 0)
	{ // verifica se já existe a MRede
		if ((mrede = redes->ExisteMRede(str.mrede_id)) == NULL)
		{ // cria uma nova MRede
			if ((mrede = DLL_NewObjMRede()) == NULL)
				return (NULL);
			mrede->Id = str.mrede_id;
			mrede->Redutivel = str.mrede_redutivel;
			redes->InsereMRede(mrede);
		}
	}
	try
	{ // cria um novo Eqpto
		if ((rede = DLL_NewObjRede(patamares->NumPatamar())) == NULL)
			return (NULL);
		// insere VTRede em VTRedes
		redes->InsereRede(rede);
		// insere VTRede no seu Cluster
		if (cluster)
			cluster->InsereRede(rede);
		// insere VTRede no seu Dominio
		if (dominio)
			dominio->InsereRede(rede);
		// inicia dados do Eqpto
		rede->Id = str.rede_id;
		rede->Codigo = str.codigo;
		rede->Extern_id = str.extern_id;
		rede->ShowAsUnsigned = str.show;
		rede->TipoRede = ExisteTipoRede(str.tiporede.id, str.tiporede.segmento,
			str.tiporede.codigo);
		if (rede->TipoRede == NULL)
		{
			if (plog)
				plog->Add("Não existe tipo da rede " + AnsiQuotedStr(str.tiporede.codigo, '\''));
			return (NULL);
		}
		// salva ID da barra inicial
		rede->BarraInicialId = str.barini_id;
		rede->Estatica = str.estatica;
		rede->Radial = str.radial;
		// proteção contra cor indefinida
		if (str.color == clBlack)
			str.color = rede->TipoRede->Color;
		rede->Color = (str.color == clBlack) ? clWhite : str.color;
		// rede->Carregada      = true;
		// rede->Valida         = (str.equivalente) ? false : true;
		rede->Carregada = (str.equivalente) ? false : true;
		area_esq = rede->AreaEsquematico();
		area_esq->x1 = str.area_esq.x1;
		area_esq->y1 = str.area_esq.y1;
		area_esq->x2 = str.area_esq.x2;
		area_esq->y2 = str.area_esq.y2;
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(rede)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		// insere Rede na MRede
		if (mrede)
		{
			mrede->InsereRede(rede);
			mrede->Carregada = rede->Carregada;
		}
        //insere dados externos
        rede->dados_ext->IdAlimPai = str.dados_ext.id_alim_pai;
        rede->dados_ext->CodChave = str.dados_ext.cod_chave;
        rede->dados_ext->Sigla = str.dados_ext.sigla;
		rede->dados_ext->Regional = str.dados_ext.regional;
        rede->dados_ext->MaeFilhaConectada = str.dados_ext.mae_filha_conectada;
        if ((curva = CriaCurvaCarga(str.dados_ext.curva_demanda)) != NULL)
        {
            rede->dados_ext->DefineCurva(curva);
        }
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (rede);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereReducaoCarga(int mrede_id, strCARGA &str)
{
	// variáveis locais
	VTBarra *barra;
	VTCarga *carga;
	VTClasse *classe;
	VTCurva *curva;
	VTCorte *corte;
	VTMRede *mrede;
	VTReducao *reducao;
	TList *lisCORTE;
	TList *lisREDUCAO;

	// determina Barra da Carga
	if ((barra = ExisteBarra(str.bar_id)) == NULL)
		return (NULL);
	// determina Classe da Carga
	if ((classe = classes->ExisteClasseTag(consEQUIVALENTE)) == NULL)
		return (NULL);
	// verifica se existe MRede
	if ((mrede = redes->ExisteMRede(mrede_id)) == NULL)
		return (NULL);
	// a MRede deve possuir um único Corte
	lisCORTE = mrede->LisCorte();
	if (lisCORTE->Count == 0)
		return (NULL);
	corte = (VTCorte*)lisCORTE->First();
	// verifica se já existe Reducao da MRede
	lisREDUCAO = mrede->LisReducao();
	if (lisREDUCAO->Count == 0)
	{ // cria Reducao p/ a MRede
		reducao = DLL_NewObjReducao(patamares->NumPatamar());
		mrede->InsereReducao(reducao, corte);
	}
	else
	{ // obtém Redução da MRede
		reducao = (VTReducao*)lisREDUCAO->First();
	}
	// define MRede como não carregada
	mrede->Carregada = false;
	try
	{ // cria objeto Carga
		if ((carga = DLL_NewObjCarga(patamares->NumPatamar())) == NULL)
			return (NULL);
		// define Carga de redução
		carga->Reducao = true;
		// define atributos da Carga
		carga->Id = str.car_id;
		carga->Codigo = str.codigo;
		carga->Extern_id = str.extern_id;
		carga->ShowAsUnsigned = str.show;
		carga->StatusAsUnsigned = str.status;
		carga->Energia_mwhmes = 0;
		carga->Fases = str.fases;
		carga->IP_NumLamp = 0;
		carga->IP_Pot_kw = 0;
		carga->IP_Fatpot = 1;
		// valida strESQ_EQBAR
		ValidaEsquematicoEqbar(str.esq);
		carga->esq = str.esq;
		// define modelo de Carga
		carga->Icte_pu = str.modelo.icte_pu;
		carga->Scte_pu = str.modelo.scte_pu;
		carga->Zcte_pu = str.modelo.zcte_pu;
		// carga definida por Curva própria: cria Curva própria
		// if ((curva = CriaCurvaCargaReducao(str.curva)) == NULL)
		if ((curva = CriaCurvaCarga(str.curva)) == NULL)
		{
			DLL_DeleteEqpto(carga);
			return (NULL);
		}
		carga->DefineCrescimento(reducao->Crescimento);
		// associa Carga c/ Barra , Classe e Curva
		carga->DefineObjBarra(barra);
		carga->DefineClasse(classe);
		carga->DefineCurva(curva);
		// insere Carga na Reducao
		reducao->InsereEqbar(carga);
		// insere Barra da Carga no Corte
		// corte->InsereBarra(carga->pbarra);
		// redefine status do Eqpto
		carga->DefineStatus(sttNOVO, false);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (carga);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereReducaoSuprimento(int mrede_id, strSUPRIMENTO &str)
{
	// variáveis locais
	TList *lisCORTE;
	TList *lisREDUCAO;
	VTBarra *barra;
	VTCorte *corte;
	VTCurva *curva;
	VTEqbar *eqbar;
	VTMRede *mrede;
	VTReducao *reducao;
	VTSuprimento *suprimento;

	// determina Barra do Suprimento
	if ((barra = ExisteBarra(str.bar_id)) == NULL)
		return (NULL);
	// verifica se existe MRede
	if ((mrede = redes->ExisteMRede(mrede_id)) == NULL)
		return (NULL);
	// a MRede deve possuir um único Corte
	lisCORTE = mrede->LisCorte();
	if (lisCORTE->Count == 0)
		return (NULL);
	corte = (VTCorte*)lisCORTE->First();
	// verifica se já existe Reducao da MRede
	lisREDUCAO = mrede->LisReducao();
	if (lisREDUCAO->Count == 0)
	{ // cria Reducao p/ a MRede
		reducao = DLL_NewObjReducao(patamares->NumPatamar());
		mrede->InsereReducao(reducao, corte);
	}
	else
	{ // obtém Redução da MRede
		reducao = (VTReducao*)lisREDUCAO->First();
	}
	// define MRede como não carregada
	mrede->Carregada = false;
	try
	{ // cria um novo Eqpto
		if ((suprimento = DLL_NewObjSuprimento(patamares->NumPatamar())) == NULL)
			return (NULL);
		// define Suprimento de redução
		suprimento->Reducao = true;
		// inicia dados do Eqpto
		suprimento->Id = str.sup_id;
		suprimento->Codigo = str.codigo;
		suprimento->Extern_id = str.extern_id;
		suprimento->ShowAsUnsigned = str.show;
		suprimento->vnom = str.vnom_kv;
		suprimento->smax = str.smax;
		suprimento->tiposup = str.tiposup;
		suprimento->zeq0 = str.zeq0;
		suprimento->zeq1 = str.zeq1;
		suprimento->pcc_3f = str.pcc_3f;
		suprimento->pcc_ft = str.pcc_ft;
		// valida estrutura strESQ_EQBAR
		ValidaEsquematicoEqbar(str.esq);
		suprimento->esq = str.esq;
		// cria Curva própria p/ o Suprimento
		if ((curva = CriaCurvaSuprimentoReducao(str.curva)) == NULL)
		{
			DLL_DeleteEqpto(suprimento);
			return (NULL);
		}
		suprimento->DefineCurva(curva);
		// valida atributos de equivalente
		if (!ValidaZeqv(suprimento) && (!ValidaPotCC(suprimento)))
		{ // assume Pcc default
			suprimento->pcc_3f.p = 0.;
			suprimento->pcc_3f.q = 600.;
			suprimento->pcc_ft.p = 0.;
			suprimento->pcc_ft.q = 400.;
		}
		// associa Suprimento c/ a Barra (SEM inserir na Barra)
		suprimento->DefineObjBarra(barra);
		// insere Suprimento na Reducao
		reducao->InsereEqbar(suprimento);
		// insere Barra do Suprimento no Corte
		// corte->InsereBarra(suprimento->pbarra);
		// redefine status do Eqpto
		suprimento->DefineStatus(sttNOVO, false);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (suprimento);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereReducaoTrafo(int mrede_id, strTRAFO &str)
{
	TList *lisCORTE;
	TList *lisREDUCAO;
	VTBarra *pbar1, *pbar2;
	VTCorte *corte;
	VTCurva *curva;
	VTEqbar *eqbar;
	VTMRede *mrede;
	VTReducao *reducao;
	VTTrafo *trafo;

	// determina Barras do Trafo
	if ((pbar1 = ExisteBarra(str.bar1_id)) == NULL)
		return (NULL);
	if ((pbar2 = ExisteBarra(str.bar2_id)) == NULL)
		return (NULL);
	// verifica se existe MRede
	if ((mrede = redes->ExisteMRede(mrede_id)) == NULL)
		return (NULL);
	// a MRede deve possuir um único Corte
	lisCORTE = mrede->LisCorte();
	if (lisCORTE->Count == 0)
		return (NULL);
	corte = (VTCorte*)lisCORTE->First();
	// verifica se já existe Reducao da MRede
	lisREDUCAO = mrede->LisReducao();
	if (lisREDUCAO->Count == 0)
	{ // cria Reducao p/ a MRede
		reducao = DLL_NewObjReducao(patamares->NumPatamar());
		mrede->InsereReducao(reducao, corte);
	}
	else
	{ // obtém Redução da MRede
		reducao = (VTReducao*)lisREDUCAO->First();
	}
	// define MRede como não carregada
	mrede->Carregada = false;
	try
	{ // cria um novo Eqpto
		if ((trafo = DLL_NewObjTrafo(patamares->NumPatamar())) == NULL)
			return (NULL);
		// define Trafo de redução
		trafo->Reducao = true;
		// inicia dados do Eqpto
		trafo->Id = str.trf_id;
		trafo->Codigo = str.codigo;
		trafo->Extern_id = str.extern_id;
		trafo->ShowAsUnsigned = str.show;
		trafo->snom = str.snom_mva;
		trafo->z0 = str.z0;
		trafo->z1 = str.z1;
		trafo->perda_ferro = str.perda_ferro;
		trafo->pri.fase = str.pri.fases;
		trafo->pri.ligacao = str.pri.ligacao;
		trafo->pri.vnom = str.pri.vnom;
		trafo->pri.tap = str.pri.tap;
		trafo->pri.defasagem = str.pri.defasagem;
		trafo->pri.zterra = str.pri.zterra;
		trafo->sec.fase = str.sec.fases;
		trafo->sec.ligacao = str.sec.ligacao;
		trafo->sec.vnom = str.sec.vnom;
		trafo->sec.tap = str.sec.tap;
		trafo->sec.defasagem = str.sec.defasagem;
		trafo->sec.zterra = str.sec.zterra;
		// dados do equivalente
		trafo->eqv.modelo = str.eqv.modelo;
		trafo->eqv.y0_ij_pu = str.eqv.y0_ij_pu;
		trafo->eqv.y0_ji_pu = str.eqv.y0_ji_pu;
		trafo->eqv.y1_ij_pu = str.eqv.y1_ij_pu;
		trafo->eqv.y1_ji_pu = str.eqv.y1_ji_pu; ;
		// copia matYij_s
		for (int i = 0; i < MAX_FASE; i++)
		{
			for (int j = 0; j < MAX_FASE; j++)
			{
				trafo->eqv.matYij_s->Y[i][j] =
					complex<double>(str.eqv.matYij_s[i][j].g, str.eqv.matYij_s[i][j].b);
			}
		}
		// copia matYji_s
		for (int i = 0; i < MAX_FASE; i++)
		{
			for (int j = 0; j < MAX_FASE; j++)
			{
				trafo->eqv.matYji_s->Y[i][j] =
					complex<double>(str.eqv.matYji_s[i][j].g, str.eqv.matYji_s[i][j].b);
			}
		}
		// valida strESQ_LIGAR
		ValidaEsquematicoLigacao(str.esq);
		trafo->esq = str.esq;
		// dados de LTC - não utilizado
		trafo->ltc.tipo = ltcFIXO;
		trafo->ltc.ajuste_auto.v_pu = 1.;
		trafo->ltc.ajuste_auto.bar_ref = NULL;
		// associa Tracho c/suas Barras
		trafo->DefineObjBarra(pbar1, pbar2);
		// insere Trafo na Reducao
		reducao->InsereLigacao(trafo);
		// rededine status do Eqpto
		trafo->DefineStatus(sttNOVO, false);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (trafo);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereReducaoYref(int mrede_id, strYREF &str)
{
	// variáveis locais
	TList *lisCORTE;
	TList *lisREDUCAO;
	VTBarra *barra;
	VTCorte *corte;
	VTEqbar *eqbar;
	VTMRede *mrede;
	VTReducao *reducao;
	VTYref *yref;

	// determina Barra de Yref
	if ((barra = ExisteBarra(str.barra_id)) == NULL)
		return (NULL);
	// verifica se existe MRede
	if ((mrede = redes->ExisteMRede(mrede_id)) == NULL)
		return (NULL);
	// a MRede deve possuir um único Corte
	lisCORTE = mrede->LisCorte();
	if (lisCORTE->Count == 0)
		return (NULL);
	corte = (VTCorte*)lisCORTE->First();
	// verifica se já existe Reducao da MRede
	lisREDUCAO = mrede->LisReducao();
	if (lisREDUCAO->Count == 0)
	{ // cria Reducao p/ a MRede
		reducao = DLL_NewObjReducao(patamares->NumPatamar());
		mrede->InsereReducao(reducao, corte);
	}
	else
	{ // obtém Redução da MRede
		reducao = (VTReducao*)lisREDUCAO->First();
	}
	// define MRede como não carregada
	mrede->Carregada = false;
	try
	{ // cria um novo Eqpto
		if ((yref = DLL_NewObjYref()) == NULL)
			return (NULL);
		// define Yref de redução
		yref->Reducao = true;
		// inicia dados do Eqpto
		yref->Id = str.yref_id;
		yref->Codigo = str.codigo;
		yref->Extern_id = str.extern_id;
		yref->ShowAsUnsigned = str.show;
		yref->Y0_pu = complex<double>(str.y0_pu.g, str.y0_pu.b);
		yref->Y1_pu = complex<double>(str.y1_pu.g, str.y1_pu.b);
		// copia matriz de admitâncias
		for (int i = 0; i < MAX_FASE; i++)
		{
			for (int j = 0; j < MAX_FASE; j++)
			{
				yref->Y_pu[i][j] = complex<double>(str.maty_pu[i][j].g, str.maty_pu[i][j].b);
			}
		}
		// valida estrutura strESQ_EQBAR
		ValidaEsquematicoEqbar(str.esq);
		yref->esq = str.esq;
		// yref->ShowAsUnsigned = str.show;
		// associa Yref c/sua Barra
		yref->DefineObjBarra(barra);
		// insere Yref na Reducao
		reducao->InsereEqbar(yref);
		// insere Barra do Yref no Corte
		// corte->InsereBarra(yref->pbarra);
		// rededine status do Eqpto
		yref->DefineStatus(sttNOVO, false);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (yref);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereRegulador(strREGULADOR &str)
{
	// variáveis locais
	VTRegulador *regulador;
	VTBarra *pbar1, *pbar2, *barra_ref, *barra_ref_inv, *barra_sentido;
	VTRede *rede;
	VTComentario *comentario;

	try
	{ // valida atributos de regulador
		if ((valida) && valida->Enabled)
		{
			valida->ValidaRegulador(str);
		}
		// determina Rede da Ligacao
		if ((rede = redes->ExisteRede(str.rede_id)) == NULL)
		{
			if (plog)
			{
				plog->Add("Não existe Rede do Regulador de Tensão " +
					AnsiQuotedStr(str.codigo, '\''));
			}
			return (NULL);
		}
		// determina Barras da Ligacao
		pbar1 = ExisteBarra(str.bar1_id);
		pbar2 = ExisteBarra(str.bar2_id);
		if ((pbar1 == NULL) || (pbar2 == NULL))
		{
			if (plog)
			{
				plog->Add("Não existe Barra do Regulador de Tensão " +
					AnsiQuotedStr(str.codigo, '\''));
			}
			return (NULL);
		}
		if (pbar1 == pbar2)
		{
			if (plog)
			{
				plog->Add("Regulador de Tensão com barras iguais: " +
					AnsiQuotedStr(str.codigo, '\''));
			}
			return (NULL);
		}
		// Validasentido do fluxo no regulador
		barra_sentido = ExisteBarra(str.sentido.bar_ref_id);
		if ((barra_sentido) == NULL)
		{
			barra_sentido = pbar1;
			if (plog)
			{
				plog->Add
					("Não existe Barra de referência do sentido do Fluxo no Regulador de Tensão " +
					AnsiQuotedStr(str.codigo, '\''));
			}
		}
		// determina barra de referência p / ajuste automático
		barra_ref = pbar2;
		barra_ref_inv = pbar1;
		// valida barras de referencias
		if (str.modelo != reguFIXO)
		{ // caso não seja o não automatico, verifica a barra de referencia no sentido direto
			if ((barra_ref = ExisteBarra(str.cfg_auto.bar_ref_id)) == NULL)
			{ // nao possuindo a barra, loga e altera o tipo para fixo
				if (plog)
				{
					plog->Add("Não existe Barra de referência do Regulador de Tensão " +
						AnsiQuotedStr(str.codigo, '\''));
				}
				// redefine regulador com tap fixo
				if (str.num_passo == 0)
				{
					str.num_passo = 16;
				}
				str.cfg_fixo.passo = 0;
				str.modelo = reguFIXO;
				barra_ref = pbar2;
			}
			if ((str.modelo == reguAUTO_BI) && ((barra_ref_inv =
				ExisteBarra(str.cfg_auto.bar_ref_inv_id)) == NULL))
			{
				if (plog)
				{
					plog->Add
						("Não existe Barra de referência sentido inverso do Regulador de Tensão " +
						AnsiQuotedStr(str.codigo, '\''));
				}
				// redefine regulador automático unidirecional
				str.modelo = reguAUTO_UNI;
				barra_ref_inv = pbar1;
			}
		}
		// cria um novo Eqpto
		if ((regulador = DLL_NewObjRegulador(patamares->NumPatamar())) == NULL)
		{
			return (NULL);
		}
		// inicializa configuracoes default
		regulador->var_tensao = 10;
		regulador->num_passo = 16;
		regulador->cfg_fixo.passo = 0;
		// regulador->cfg_auto.bar_ref   = barra_ref;
		// regulador->cfg_auto.bar_ref_inv = barra_ref_inv;
		// regulador->cfg_auto.v_pu      = 1;
		// regulador->cfg_auto.vinv_pu   = 1;
		// regulador->cfg_auto.larg_bandamorta_pu 	   = 0.01;
		// regulador->cfg_auto.larg_bandamorta_inv_pu = 0.01;
		// inicia dados do Eqpto
		regulador->Id = str.id;
		regulador->modelo = str.modelo;
		regulador->Codigo = str.codigo;
		regulador->Extern_id = str.extern_id;
		regulador->ShowAsUnsigned = str.show;
		regulador->ligacao = str.ligacao;
		// proteção na definição das fases
		switch (str.ligacao)
		{
		case lgEST_ISO: // define fases
			regulador->fases = faseABC;
			break;
		case lgDELTA_FECHADO: // define fases
			regulador->fases = faseABC;
			break;
		case lgDELTA_ABERTO: // define fase comum
			if (str.fases == faseA)
				regulador->fases = faseA;
			else if (str.fases == faseB)
				regulador->fases = faseB;
			else if (str.fases == faseC)
				regulador->fases = faseC;
			else
				regulador->fases = faseB;
			break;
		case lgMONOFASICO: // define fases
			if (str.fases == faseAN)
				regulador->fases = faseAN;
			else if (str.fases == faseBN)
				regulador->fases = faseBN;
			else if (str.fases == faseCN)
				regulador->fases = faseCN;
			else if (str.fases == faseAB)
				regulador->fases = faseAB;
			else if (str.fases == faseBC)
				regulador->fases = faseBC;
			else if (str.fases == faseCA)
				regulador->fases = faseCA;
			else if (str.fases == faseABN)
				regulador->fases = faseABN;
			else if (str.fases == faseBCN)
				regulador->fases = faseBCN;
			else if (str.fases == faseCAN)
				regulador->fases = faseCAN;
			else
				regulador->fases = faseABN;
			break;
		default:
			regulador->ligacao = lgEST_ISO;
			regulador->fases = faseABC;
			break;
		}
		regulador->snom = str.snom_mva;
		regulador->by_pass = str.by_pass;
		regulador->sentido.bar_ref = barra_sentido;
		regulador->var_tensao = str.var_tensao;
		regulador->num_passo = str.num_passo;
		regulador->cfg_fixo.passo = str.cfg_fixo.passo;
		regulador->cfg_auto.bar_ref = barra_ref;
		regulador->cfg_auto.bar_ref_inv = barra_ref_inv;
		switch (str.modelo)
		{
        case reguPADRAO:
		case reguAUTO_BI:
		case reguAUTO_UNI:
		case reguAUTO_FIXA:
			regulador->cfg_auto.v_pu = str.cfg_auto.v_pu;
			regulador->cfg_auto.vinv_pu = str.cfg_auto.vinv_pu;
			regulador->cfg_auto.larg_bandamorta_pu = str.cfg_auto.larg_bandamorta_pu;
			regulador->cfg_auto.larg_bandamorta_inv_pu = str.cfg_auto.larg_bandamorta_inv_pu;
			break;
		default:
			regulador->cfg_auto.v_pu = 1;
			regulador->cfg_auto.vinv_pu = 1;
			regulador->cfg_auto.larg_bandamorta_pu = 0.01;
			regulador->cfg_auto.larg_bandamorta_inv_pu = 0.01;
			break;
		}
		//relação de tap
	    regulador->rel_tap            = str.rel_tap;
		// valida strESQ_LIGAR
		ValidaEsquematicoLigacao(str.esq);
		regulador->esq = str.esq;
		// define Barras da Ligacao
		regulador->DefineObjBarra(pbar1, pbar2);
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(regulador)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		// insere Ligacao na Rede
		rede->InsereLigacao(regulador);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (regulador);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereSuporte(strSUPORTE &str)
{
	// variáveis locais
	VTPtoFixacao *pto_fixacao;
	VTSuporte *suporte;

	try
	{ // proteção
		if (arranjos == NULL)
			return (NULL);
		// verifica se já existe o Suporte
		if (str.id > 0)
		{ // procura Suporte por ID
			if ((suporte = arranjos->ExisteSuporte(str.id)) != NULL)
				return (suporte);
		}
		// valida atributos de suporte
		if ((valida) && valida->Enabled)
			valida->ValidaSuporte(str);
		// cria um novo Suporte
		suporte = DLL_NewObjSuporte();
		suporte->Id = str.id;
		suporte->Codigo = str.codigo;
		suporte->Extern_id = str.extern_id;
		suporte->TipoDeSuporte = str.tipo;
		suporte->Vmin_KV = str.vmin;
		suporte->Vmax_KV = str.vmax;
		// PRECISA salvar o status indicado na estrutura por causa da leitura da base de dados
		// VERIFICAR importadores
		suporte->StatusAsUnsigned = str.status;
		// cria pontos de fixação
		for (int n = 0; n < str.num_pto; n++)
		{ // cria um novo PtoFixacao
			if ((pto_fixacao = DLL_NewObjPtoFixacao()) == NULL)
				continue;
			// lê dados do PtoFixacao
			pto_fixacao->id = str.pto_fixacao[n].id;
			pto_fixacao->codigo = str.pto_fixacao[n].codigo;
			pto_fixacao->coord.x = str.pto_fixacao[n].coord_x;
			pto_fixacao->coord.y = str.pto_fixacao[n].coord_y;
			// insere PtoFixacao no Suporte após iniciar dados pois Suporte ordena a lista de PtoFixacao
			suporte->InserePtoFixacao(pto_fixacao);
		}
		// insere Suporte em Arranjos
		arranjos->InsereSuporte(suporte);
	}
	catch (Exception &e)
	{
		return (NULL);
	}
	return (suporte);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereSuprimento(strSUPRIMENTO &str)
{
	// variáveis locais
	VTBarra *pbarra;
	VTCurva *curva;
	VTEqbar *eqbar;
	VTSuprimento *suprimento;
	VTComentario *comentario;

	try
	{ // valida atributos de suprimento
		if ((valida) && valida->Enabled)
			valida->ValidaSuprimento(str);
		// determina Barra do Suprimento
		if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe Barra do Suprimento " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// verifica se o Suprimento já existe
		if ((eqbar = pbarra->ExisteEqbar(eqptoSUPRIMENTO, str.sup_id)) != NULL)
			return (eqbar);
		// cria um novo Eqpto
		if ((suprimento = DLL_NewObjSuprimento(patamares->NumPatamar())) == NULL)
			return (NULL);
		// inicia dados do Eqpto
		suprimento->Id = str.sup_id;
		suprimento->Codigo = str.codigo;
		suprimento->Extern_id = str.extern_id;
		suprimento->ShowAsUnsigned = str.show;
		suprimento->vnom = str.vnom_kv;
		suprimento->smax = str.smax;
		suprimento->tiposup = str.tiposup;
		suprimento->zeq0 = str.zeq0;
		suprimento->zeq1 = str.zeq1;
		suprimento->pcc_3f = str.pcc_3f;
		suprimento->pcc_ft = str.pcc_ft;
		// proteção
		if (IsDoubleZero(suprimento->vnom))
			suprimento->vnom = pbarra->vnom;
		if (IsDoubleZero(pbarra->vnom))
			pbarra->vnom = suprimento->vnom; ;
		// valida estrutura strESQ_EQBAR
		ValidaEsquematicoEqbar(str.esq);
		suprimento->esq = str.esq;
		// cria Curva própria p/ o Suprimento
		if ((curva = CriaCurvaSuprimento(str.curva)) == NULL)
		{
			DLL_DeleteEqpto(suprimento);
			return (NULL);
		}
		suprimento->DefineCurva(curva);
		// valida atributos de equivalente
		if (!ValidaZeqv(suprimento) && (!ValidaPotCC(suprimento)))
		{ // assume Pcc default
			suprimento->pcc_3f.p = 0.;
			suprimento->pcc_3f.q = 600.;
			suprimento->pcc_ft.p = 0.;
			suprimento->pcc_ft.q = 400.;
		}
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(suprimento)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		// insere Eqpbar na Barra
		pbarra->InsereEqbar(suprimento);
		// define Barra do Suprimento como Barra inicial da sua Rede
		DefineBarraInicial(pbarra);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (suprimento);
}

// -----------------------------------------------------------------------------
bool __fastcall TMontaRede::InsereTipoChave(strTIPO_CHAVE &str)
{
	// variáveis locais
	VTTipoChave* tipochave;

	try
	{ // verifica se já existe o Tipochave
		if (tipos->ExisteTipoChave(str.id) != NULL)
			return (true);
		// cria objeto VTTipoChave
		if ((tipochave = DLL_NewObjTipoChave()) == NULL)
			return (false);
		// lê dados do TipoChave
		tipochave->Id = str.id;
		tipochave->Codigo = str.codigo;
		tipochave->Tipo = str.tipo;
		tipochave->Operacao = str.modo_operacao;
		tipochave->StatusAsUnsigned = str.status;
		// insere TipoChave Redes
		tipos->InsereTipoChave(tipochave);
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TMontaRede::InsereTipoRede(strTIPO_REDE &str)
{
	// variáveis locais
	VTTipoRede *tiporede;

	try
	{ // verifica se já existe o Tipochave
		if (tipos->ExisteTipoRede(str.id) != NULL)
			return (true);
		// cria objeto VTTipoRede
		if ((tiporede = DLL_NewObjTipoRede()) == NULL)
			return (false);
		// lê dados do TipoChave
		tiporede->Id = str.id;
		tiporede->Segmento = str.segmento;
		tiporede->Codigo = str.codigo;
		tiporede->Descricao = str.descricao;
		tiporede->Vmax_kv = str.vmax_kv;
		tiporede->Vmin_kv = str.vmin_kv;
		tiporede->Color = (str.color == clBlack) ? clWhite : str.color;
		tiporede->StatusAsUnsigned = str.status;
		// quantidade de Redes deste tipo existente na base de dados
		tiporede->ValorAux = str.qtde_rede; ;
		// insere TipoRede
		tipos->InsereTipoRede(tiporede);
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}
/*
// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereTrafo(strTRAFO &str)
{
	// variáveis locais
	double vtap_pu;
	VTTrafo *ptrafo;
	VTBarra *pbar1, *pbar2;
	VTRede *rede;
	VTComentario *comentario;

	try
	{ // determina Rede da Ligacao
		if ((rede = redes->ExisteRede(str.rede_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe Rede do Trafo " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}

		// valida atributos de trafo
		if ((valida) && valida->Enabled)
		{
			if (rede->TipoRede->Segmento == redeETD)
				valida->ValidaTrafoSED(str);
			else
				valida->ValidaTrafoMTBT(str);
		}
		// determina Barras da Ligacao
		pbar1 = ExisteBarra(str.bar1_id);
		pbar2 = ExisteBarra(str.bar2_id);
		if ((pbar1 == NULL) || (pbar2 == NULL))
		{
			if (plog)
				plog->Add("Não existe Barra do Trafo " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		if (pbar1 == pbar2)
		{
			if (plog)
				plog->Add("Trafo com barras iguais: " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// proteção contra valor de tap primário incorreto
		if (!IsDoubleZero(str.pri.vnom))
		{
			vtap_pu = str.pri.tap / str.pri.vnom;
			if ((vtap_pu < 0.7) || (vtap_pu > 1.3))
			{ // redefine tensão de tap igual a tensão nominal
				str.pri.tap = str.pri.vnom;
				if (plog)
					plog->Add("Trafo com tensão de tap primário inválida: " +
					AnsiQuotedStr(str.codigo, '\''));
			}
		}
		// proteção contra valor de tap secundário incorreto
		if (!IsDoubleZero(str.sec.vnom))
		{
			vtap_pu = str.sec.tap / str.sec.vnom;
			if ((vtap_pu < 0.7) || (vtap_pu > 1.3))
			{ // redefine tensão de tap igual a tensão nominal
				str.sec.tap = str.sec.vnom;
				if (plog)
					plog->Add("Trafo com tensão de tap secundário inválida: " +
					AnsiQuotedStr(str.codigo, '\''));
			}
		}
		// cria um novo Eqpto
		if ((ptrafo = DLL_NewObjTrafo(patamares->NumPatamar())) == NULL)
			return (NULL);
		// inicia dados do Eqpto
		ptrafo->Id = str.trf_id;
		ptrafo->Codigo = str.codigo;
		ptrafo->Extern_id = str.extern_id;
		ptrafo->ShowAsUnsigned = str.show;
		ptrafo->snom = str.snom_mva;
		ptrafo->z0 = str.z0;
		ptrafo->z1 = str.z1;
		ptrafo->perda_ferro = str.perda_ferro;
		ptrafo->auto_trafo = str.auto_trafo;
		ptrafo->pri.fase = str.pri.fases;
		ptrafo->pri.ligacao = str.pri.ligacao;
		ptrafo->pri.vnom = str.pri.vnom;
		ptrafo->pri.tap = str.pri.tap;
		ptrafo->pri.defasagem = str.pri.defasagem;
		ptrafo->pri.zterra = str.pri.zterra;
		ptrafo->sec.fase = str.sec.fases;
		ptrafo->sec.ligacao = str.sec.ligacao;
		ptrafo->sec.vnom = str.sec.vnom;
		ptrafo->sec.tap = str.sec.tap;
		ptrafo->sec.defasagem = str.sec.defasagem;
		ptrafo->sec.zterra = str.sec.zterra;
		// valida strESQ_LIGAR
		ValidaEsquematicoLigacao(str.esq);
		ptrafo->esq = str.esq;
		// dados de LTC
		ptrafo->ltc.tipo = str.ltc.tipo;
		ptrafo->ltc.ajuste_auto.v_pu = str.ltc.v_pu;
		if (ptrafo->ltc.tipo != ltcFIXO)
		{
			ptrafo->ltc.ajuste_auto.bar_ref = ExisteBarra(str.ltc.bar_id);
			if (ptrafo->ltc.ajuste_auto.bar_ref == NULL)
				ptrafo->ltc.tipo = ltcFIXO;
		}
		// proteções
		if (ptrafo->pri.tap == 0.)
			ptrafo->pri.tap = ptrafo->pri.vnom;
		if (ptrafo->sec.tap == 0.)
			ptrafo->sec.tap = ptrafo->sec.vnom;
		if (IsDoubleZero(ptrafo->snom))
		{
			ptrafo->snom = 0.1;
			if (plog)
				plog->Add("Trafo " + AnsiQuotedStr(ptrafo->Codigo, '\'') +
				" com potência nominal nula");
		}
		if ((ptrafo->z1.r == 0.) && (ptrafo->z1.x == 0.))
		{
			ptrafo->z1.x = 0.05; // pu
			if (plog)
				plog->Add("Trafo " + AnsiQuotedStr(ptrafo->Codigo, '\'') +
				" com impedância de seq.1 nula");
		}
		if ((ptrafo->z0.r == 0.) && (ptrafo->z0.x == 0.))
		{
			ptrafo->z0.x = 0.05; // pu
			if (plog)
				plog->Add("Trafo " + AnsiQuotedStr(ptrafo->Codigo, '\'') +
				" com impedância de seq.0 nula");
		}
		// define Barras da Ligacao
		ptrafo->DefineObjBarra(pbar1, pbar2);
		// garante ordenação das barras
		ptrafo->OrdenaBar12PriSec();
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(ptrafo)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		// insere Ligacao na Rede
		rede->InsereLigacao(ptrafo);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (ptrafo);
}
*/
//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereTrafo(strTRAFO &str)
   {
   //variáveis locais
   VTBarra *pbar1, *pbar2;

   try{//determina Barras da Ligacao
      pbar1 = ExisteBarra(str.bar1_id);
      pbar2 = ExisteBarra(str.bar2_id);
      return(InsereTrafo(str, pbar1, pbar2));
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereTrafo(strTRAFO &str, VTBarra* barra1, VTBarra *barra2)
   {
   //variáveis locais
   double       vtap_pu;
   VTTrafo      *ptrafo;
   VTRede       *rede;
   VTComentario *comentario;

   try{//determina Rede da Ligacao
      if ((rede = redes->ExisteRede(str.rede_id)) == NULL)
         {
         if (plog) plog->Add("Não existe Rede do Trafo " + AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //valida atributos de trafo
      if((valida) && valida->Enabled)
         {
         if (rede->TipoRede->Segmento == redeETD) valida->ValidaTrafoSED(str);
         else                                     valida->ValidaTrafoMTBT(str);
         }
      //determina Barras da Ligacao
      //barra1 = ExisteBarra(str.bar1_id);
      //barra2 = ExisteBarra(str.bar2_id);
      if ((barra1 == NULL)||(barra2 == NULL))
         {
         if (plog) plog->Add("Não existe Barra do Trafo " + AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      if (barra1 == barra2)
         {
         if (plog) plog->Add("Trafo com barras iguais: " + AnsiQuotedStr(str.codigo, '\''));
         return(NULL);
         }
      //proteção contra valor de tap primário incorreto
      if (! IsDoubleZero(str.pri.vnom))
         {
         vtap_pu = str.pri.tap / str.pri.vnom;
         if ((vtap_pu < 0.7)||(vtap_pu > 1.3))
            {//redefine tensão de tap igual a tensão nominal
            str.pri.tap = str.pri.vnom;
            if (plog) plog->Add("Trafo com tensão de tap primário inválida: " + AnsiQuotedStr(str.codigo, '\''));
            }
         }
      //proteção contra valor de tap secundário incorreto
      if (! IsDoubleZero(str.sec.vnom))
         {
         vtap_pu = str.sec.tap / str.sec.vnom;
         if ((vtap_pu < 0.7)||(vtap_pu > 1.3))
            {//redefine tensão de tap igual a tensão nominal
            str.sec.tap = str.sec.vnom;
            if (plog) plog->Add("Trafo com tensão de tap secundário inválida: " + AnsiQuotedStr(str.codigo, '\''));
            }
         }
      //cria um novo Eqpto
      if ((ptrafo = DLL_NewObjTrafo(patamares->NumPatamar())) == NULL) return(NULL);
      //inicia dados do Eqpto
      ptrafo->Id            = str.trf_id;
      ptrafo->Codigo        = str.codigo;
      ptrafo->Extern_id     = str.extern_id;
      ptrafo->ShowAsUnsigned = str.show;
      ptrafo->snom          = str.snom_mva;
      ptrafo->z0            = str.z0;
      ptrafo->z1            = str.z1;
      ptrafo->perda_ferro   = str.perda_ferro;
      ptrafo->auto_trafo    = str.auto_trafo;
      ptrafo->pri.fase      = str.pri.fases;
      ptrafo->pri.ligacao   = str.pri.ligacao;
      ptrafo->pri.vnom      = str.pri.vnom;
      ptrafo->pri.tap       = str.pri.tap;
      ptrafo->pri.defasagem = str.pri.defasagem;
      ptrafo->pri.zterra    = str.pri.zterra;
      ptrafo->sec.fase      = str.sec.fases;
      ptrafo->sec.ligacao   = str.sec.ligacao;
      ptrafo->sec.vnom      = str.sec.vnom;
      ptrafo->sec.tap       = str.sec.tap;
      ptrafo->sec.defasagem = str.sec.defasagem;
      ptrafo->sec.zterra    = str.sec.zterra;
      //valida strESQ_LIGAR
      ValidaEsquematicoLigacao(str.esq);
      ptrafo->esq           = str.esq;
      //dados de LTC
      ptrafo->ltc.tipo             = str.ltc.tipo;
      ptrafo->ltc.ajuste_auto.v_pu = str.ltc.v_pu;
      if (ptrafo->ltc.tipo != ltcFIXO)
         {
         ptrafo->ltc.ajuste_auto.bar_ref  = ExisteBarra(str.ltc.bar_id);
         if (ptrafo->ltc.ajuste_auto.bar_ref == NULL) ptrafo->ltc.tipo = ltcFIXO;
         }
      //proteções
      if (ptrafo->pri.tap == 0.) ptrafo->pri.tap = ptrafo->pri.vnom;
      if (ptrafo->sec.tap == 0.) ptrafo->sec.tap = ptrafo->sec.vnom;
      if (IsDoubleZero(ptrafo->snom))
         {
         ptrafo->snom = 0.1;
         if (plog) plog->Add("Trafo " + AnsiQuotedStr(ptrafo->Codigo, '\'') + " com potência nominal nula");
         }
      if ((ptrafo->z1.r == 0.)&&(ptrafo->z1.x == 0.))
         {
         ptrafo->z1.x = 0.05;  //pu
         if (plog) plog->Add("Trafo " + AnsiQuotedStr(ptrafo->Codigo, '\'') + " com impedância de seq.1 nula");
         }
      if ((ptrafo->z0.r == 0.)&&(ptrafo->z0.x == 0.))
         {
         ptrafo->z0.x = 0.05;  //pu
         if (plog) plog->Add("Trafo " + AnsiQuotedStr(ptrafo->Codigo, '\'') + " com impedância de seq.0 nula");
         }
      //define Barras da Ligacao
      ptrafo->DefineObjBarra(barra1, barra2);
      //garante ordenação das barras
      ptrafo->OrdenaBar12PriSec();
      // Comentário do equipamento
      if (!str.comentario.comentario.IsEmpty())
         {
         if ((comentario = DLL_NewObjComentario(ptrafo)) != NULL)
            {
            comentario->Texto = str.comentario.comentario;
            comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
            redes->InsereComentario(comentario);
            }
         }
      //insere Ligacao na Rede
      rede->InsereLigacao(ptrafo);
      }catch(Exception &e)
         {//nada a fazer
         return(NULL);
         }
   return(ptrafo);
   }

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereTrafo3E(strTRAFO3E &str)
{
	// variáveis locais
	VTTrafo3E *ptrafo;
	VTBarra *pbar1, *pbar2, *pbar3;
	VTRede *rede;
	VTComentario *comentario;

	try
	{ // determina Rede da Ligacao
		if ((rede = redes->ExisteRede(str.rede_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe Rede do Trafo3E " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// determina Barras da Ligacao
		pbar1 = ExisteBarra(str.bar1_id);
		pbar2 = ExisteBarra(str.bar2_id);
		pbar3 = ExisteBarra(str.bar3_id);
		if ((pbar1 == NULL) || (pbar2 == NULL) || (pbar3 == NULL))
		{
			if (plog)
				plog->Add("Não existe Barra do Trafo3E " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		if ((pbar1 == pbar2) || (pbar1 == pbar3) || (pbar2 == pbar3))
		{
			if (plog)
				plog->Add("Trafo3E com barras iguais: " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// cria um novo Eqpto
		if ((ptrafo = DLL_NewObjTrafo3E(patamares->NumPatamar())) == NULL)
			return (NULL);
		// inicia dados do Eqpto
		ptrafo->Id = str.trf_id;
		ptrafo->Codigo = str.codigo;
		ptrafo->Extern_id = str.extern_id;
		ptrafo->ShowAsUnsigned = str.show;
		// ptrafo->pri.fases   = ExisteFases(str.pri.ligacao);
		ptrafo->pri.ligacao = str.pri.ligacao;
		ptrafo->pri.snom = str.pri.snom;
		ptrafo->pri.vnom = str.pri.vnom;
		ptrafo->pri.tap = str.pri.tap;
		ptrafo->pri.defasagem = str.pri.defasagem;
		ptrafo->pri.zterra = str.pri.zterra;
		// ptrafo->sec.fases   = ExisteFases(str.sec.ligacao);
		ptrafo->sec.ligacao = str.sec.ligacao;
		ptrafo->sec.snom = str.sec.snom;
		ptrafo->sec.vnom = str.sec.vnom;
		ptrafo->sec.tap = str.sec.tap;
		ptrafo->sec.defasagem = str.sec.defasagem;
		ptrafo->sec.zterra = str.sec.zterra;
		// ptrafo->ter.fases   = ExisteFases(str.ter.ligacao);
		ptrafo->ter.ligacao = str.ter.ligacao;
		ptrafo->ter.snom = str.ter.snom;
		ptrafo->ter.vnom = str.ter.vnom;
		ptrafo->ter.tap = str.ter.tap;
		ptrafo->ter.defasagem = str.ter.defasagem;
		ptrafo->ter.zterra = str.ter.zterra;
		// impedâncias de sequência
		ptrafo->zps0 = str.zps0;
		ptrafo->zps1 = str.zps1;
		ptrafo->zpt0 = str.zpt0;
		ptrafo->zpt1 = str.zpt1;
		ptrafo->zst0 = str.zst0;
		ptrafo->zst1 = str.zst1;
		ptrafo->perda_ferro = str.perda_ferro;
		// ltc
		ptrafo->ltc.tipo = str.ltc.tipo;
		ptrafo->ltc.dv_perc = str.ltc.dv_perc;
		ptrafo->ltc.num_passo = str.ltc.num_passo;
		ptrafo->ltc.passo = str.ltc.passo;
		// valida strESQ_LIGAR
		ValidaEsquematicoLigacao(str.esq);
		ptrafo->esq = str.esq;
		// define Barras da Ligacao
		ptrafo->DefineObjBarra(pbar1, pbar2, pbar3);
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(ptrafo)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		// insere Ligacao na Rede
		rede->InsereLigacao(ptrafo);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (ptrafo);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereTrafoZZ(strTRAFOZZ &str)
{
	// variáveis locais
	VTBarra *pbarra;
	VTEqbar *eqbar;
	VTTrafoZZ *trafoZZ;
	VTComentario *comentario;

	try
	{ // determina Barra do Eqbar
		if ((pbarra = ExisteBarra(str.bar_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe Barra do TrafoZZ " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// verifica se o TrafoZZ já existe
		if ((eqbar = pbarra->ExisteEqbar(eqptoTRAFOZZ, str.id)) != NULL)
			return (eqbar);
		// cria um novo Eqpto
		if ((trafoZZ = DLL_NewObjTrafoZZ()) == NULL)
			return (NULL);
		// inicia dados do Eqpto
		trafoZZ->Id = str.id;
		trafoZZ->Codigo = str.codigo;
		trafoZZ->Extern_id = str.extern_id;
		trafoZZ->ShowAsUnsigned = str.show;
		trafoZZ->vnom = str.vnom;
		trafoZZ->snom = str.snom;
		trafoZZ->z0 = str.z0;
		// valida strESQ_EQBAR
		ValidaEsquematicoEqbar(str.esq);
		trafoZZ->esq = str.esq;
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(trafoZZ)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		// insere Eqpbar na Barra
		pbarra->InsereEqbar(trafoZZ);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (trafoZZ);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereTrecho(strTRECHO &str, strARRANJO1 &str_arr)
{
	// variáveis locais
	VTArranjo *arranjo;
	VTBarra *pbar1, *pbar2;
	VTCabo *cabo[MAX_FASE];
	VTSuporte *suporte;
	VTTrecho *trecho;
	VTRede *rede;
	VTComentario *comentario;

	try
	{ // determina Rede da Ligacao
		if ((rede = redes->ExisteRede(str.rede_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe Rede do Trecho " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// valida atributos de trecho
		if ((valida) && valida->Enabled)
		{
			if (rede->TipoRede->Segmento == redeSEC)
				valida->ValidaTrechoBT(str, str_arr);
			else if (rede->TipoRede->Segmento == redePRI)
				valida->ValidaTrechoMT(str, str_arr);
		}
		// proteção para informação de fases inconsistentes
		if (str_arr.fases != str.fases)
		{
			str_arr.fases = str.fases;
		}
		// determina Barras da Ligacao
		pbar1 = ExisteBarra(str.bar1_id);
		pbar2 = ExisteBarra(str.bar2_id);
		if ((pbar1 == NULL) || (pbar2 == NULL))
		{
			if (plog)
				plog->Add("Não existe Barra do Trecho " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		if (pbar1 == pbar2)
		{
			if (plog)
				plog->Add("Trecho com barras iguais: " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// verifica se existe o Arranjo
		if ((arranjo = arranjos->ExisteArranjo(str_arr.codigo, str_arr.tipo_cfg)) == NULL)
		{ // verifica tipo do arranjo
			switch (str_arr.tipo_cfg)
			{
			case arranjoCABO_Z0Z1: // arranjo típico definido por um único Cabo do tipo caboZ0Z1
				if ((arranjo = ExisteArranjoTipico_CABO_Z0Z1(str_arr)) == NULL)
				{ // não existe Arranjo típico: cria um Arranjo próprio
					if (plog)
						plog->DefineValor("Eqpto", "Arranjo");
					if (plog)
						plog->Add("Não existe Arranjo típico definido por cabo multiplexado: " +
						AnsiQuotedStr(str_arr.codigo, '\''));
					arranjo = CriaArranjoProprioZ0Z1(str_arr);
				}
				break;
			case arranjoCABOxFASE: // arranjo típico definido por Supote + Cabos por fase
				if ((arranjo = ExisteArranjoTipico_CABO_FASE(str_arr, rede->TipoRede)) == NULL)
				{ // não existe Arranjo típico: cria um Arranjo próprio
					if (plog)
						plog->DefineValor("Eqpto", "Arranjo");
					if (plog)
						plog->Add("Não existe Arranjo típico defindo por cabos unipolares: " +
						AnsiQuotedStr(str_arr.codigo, '\''));
					// cria arranjo próprio somente p/ trechos trifásicos
					if ((str.fases & faseABC) == faseABC)
					{
						arranjo = CriaArranjoProprioZ0Z1(str_arr);
					}
				}
				break;
			case arranjoZ0Z1: // arranjo definido por Z0 e Z1, em ohm/km
				// verifica se Arranjo típico
				if (str_arr.tipico)
				{ // tenta localizar um Arranjo típico
					if ((arranjo = ExisteArranjoTipico_Z0Z1(str_arr)) == NULL)
					{ // não existe Arranjo típico: cria um Arranjo próprio
						if (plog)
							plog->DefineValor("Eqpto", "Arranjo");
						if (plog)
							plog->Add
								("Não existe Arranjo típico defindo por impedâncias de sequência: "
							+ AnsiQuotedStr(str_arr.codigo, '\''));
						arranjo = CriaArranjoProprioZ0Z1(str_arr);
					}
				}
				else
				{ // cria Arranjo próprio
					arranjo = CriaArranjoProprioZ0Z1(str_arr);
				}
				break;
			case arranjoPU_SB100: // arranjo definido por Z0 e Z1, em pu com Sb=100 MVA
				// sempre assume como arranjo próprio
				arranjo = CriaArranjoProprioPU_SB100(str_arr, str.comp_km);
				break;
			default:
				arranjo = NULL;
			}
		}
		// verifica se existe o Arranjo do Trecho
		if (arranjo == NULL)
		{
			if (plog)
				plog->DefineValor("Eqpto", "Trecho");
			if (plog)
				plog->Add("Não existe Arranjo do Trecho " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// cria um novo Eqpto
		if ((trecho = DLL_NewObjTrecho(patamares->NumPatamar())) == NULL)
			return (NULL);
		// inicia dados do Eqpto
		trecho->Id = str.tre_id;
		trecho->Codigo = str.codigo;
		trecho->Extern_id = str.extern_id;
		trecho->ShowAsUnsigned = str.show;
		// valida strESQ_LIGAR
		ValidaEsquematicoLigacao(str.esq);
		trecho->esq = str.esq;
		trecho->Comprimento_km = str.comp_km;
		// define Barras da Ligacao
		trecho->DefineObjBarra(pbar1, pbar2);
		// TESTES
		// ordena barras do Trecho por ordem alfabética
		// OrdenaBarraPorNome(trecho);
		// define Arranjo do Trecho
		trecho->DefineObjArranjo(arranjo);
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(trecho)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		// define fases disponíveis nas Barras
		pbar1->resflow->Fases |= trecho->arranjo->Fases;
		pbar2->resflow->Fases |= trecho->arranjo->Fases;
		// insere Ligacao na Rede
		rede->InsereLigacao(trecho);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (trecho);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereTrecho(strTRECHO &str, strARRANJO2 &str_arr)
{
	// variáveis locais
	VTArranjo *arranjo;
	VTTrecho *trecho;

	try
	{ // verifica se Arranjo típico
		if (str_arr.tipico)
		{ // determina Arranjo típico
			arranjo = ExisteArranjoTipico(str_arr.arranjo_id);
		}
		else
		{ // cria Arranjo próprio
			arranjo = InsereArranjo(str_arr);
		}
		if (arranjo == NULL)
		{
			if (plog)
				plog->Add("Não existe Arranjo do Trecho " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// cria o Trecho
		trecho = (VTTrecho*)InsereTrecho(str, arranjo);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (trecho);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereTrecho(strTRECHO &str, VTArranjo *arranjo)
{
	// variáveis locais
	VTBarra *pbar1, *pbar2;
	VTCabo *cabo[MAX_FASE];
	VTSuporte *suporte;
	VTTrecho *trecho;
	VTRede *rede;
	VTComentario *comentario;

	try
	{ // determina Rede da Ligacao
		if ((rede = redes->ExisteRede(str.rede_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe Rede do Trecho " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// determina Barras da Ligacao
		pbar1 = ExisteBarra(str.bar1_id);
		pbar2 = ExisteBarra(str.bar2_id);
		if ((pbar1 == NULL) || (pbar2 == NULL))
		{
			if (plog)
				plog->Add("Não existe Barra do Trecho " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		if (pbar1 == pbar2)
		{
			if (plog)
				plog->Add("Trecho com barras iguais: " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// cria um novo Eqpto
		if ((trecho = DLL_NewObjTrecho(patamares->NumPatamar())) == NULL)
			return (NULL);
		// inicia dados do Eqpto
		trecho->Id = str.tre_id;
		trecho->Codigo = str.codigo;
		trecho->Extern_id = str.extern_id;
		trecho->ShowAsUnsigned = str.show;
		// valida strESQ_LIGAR
		ValidaEsquematicoLigacao(str.esq);
		trecho->esq = str.esq;
		trecho->Comprimento_km = str.comp_km;
		// define Barras da Ligacao
		trecho->DefineObjBarra(pbar1, pbar2);
		// TESTES
		// ordena barras do Trecho por ordem alfabética
		// OrdenaBarraPorNome(trecho);
		// define Arranjo do Trecho
		trecho->DefineObjArranjo(arranjo);
		// verifica comprimento com coordenadas
		ValidaComprimento(trecho);
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(trecho)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		// define fases disponíveis nas Barras
		pbar1->resflow->Fases |= trecho->arranjo->Fases;
		pbar2->resflow->Fases |= trecho->arranjo->Fases;
		// insere Ligacao na Rede
		rede->InsereLigacao(trecho);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (trecho);
}

// -----------------------------------------------------------------------------
VTEqpto* __fastcall TMontaRede::InsereTrecho(strTRECHO &str, VTBarra* barra1, VTBarra *barra2,
	VTArranjo *arranjo)
{
	// variáveis locais
	VTSuporte *suporte;
	VTTrecho *trecho;
	VTRede *rede;
	VTComentario *comentario;

	try
	{ // determina Rede da Ligacao
		if ((rede = redes->ExisteRede(str.rede_id)) == NULL)
		{
			if (plog)
				plog->Add("Não existe Rede do Trecho " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		if (barra1 == barra2)
		{
			if (plog)
				plog->Add("Trecho com barras iguais: " + AnsiQuotedStr(str.codigo, '\''));
			return (NULL);
		}
		// cria um novo Eqpto
		if ((trecho = DLL_NewObjTrecho(patamares->NumPatamar())) == NULL)
			return (NULL);
		// inicia dados do Eqpto
		trecho->Id = str.tre_id;
		trecho->Codigo = str.codigo;
		trecho->Extern_id = str.extern_id;
		trecho->ShowAsUnsigned = str.show;
		// valida strESQ_LIGAR
		ValidaEsquematicoLigacao(str.esq);
		trecho->esq = str.esq;
		trecho->Comprimento_km = str.comp_km;
		// define Barras da Ligacao
		trecho->DefineObjBarra(barra1, barra2);
		// TESTES
		// ordena barras do Trecho por ordem alfabética
		// OrdenaBarraPorNome(trecho);
		// define Arranjo do Trecho
		trecho->DefineObjArranjo(arranjo);
		// verifica comprimento com coordenadas
		ValidaComprimento(trecho);
		// Comentário do equipamento
		if (!str.comentario.comentario.IsEmpty())
		{
			if ((comentario = DLL_NewObjComentario(trecho)) != NULL)
			{
				comentario->Texto = str.comentario.comentario;
				comentario->DefinePosicaoEsquematico(str.comentario.dx, str.comentario.dy);
				redes->InsereComentario(comentario);
			}
		}
		// define fases disponíveis nas Barras
		barra1->resflow->Fases |= trecho->arranjo->Fases;
		barra2->resflow->Fases |= trecho->arranjo->Fases;
		// insere Ligacao na Rede
		rede->InsereLigacao(trecho);
	}
	catch (Exception &e)
	{ // nada a fazer
		return (NULL);
	}
	return (trecho);
}

// ---------------------------------------------------------------------------
void __fastcall TMontaRede::InverteCoordUTM(void)
{
	// variáveis locais
	int dy1, dy2;
	VTBarra *pbarra;
	VTArea *area = redes->AreaEsquematico();
	TList *lisBAR = redes->LisBarra();

	// loop p/ todas Barras de Redes
	for (int nb = 0; nb < lisBAR->Count; nb++)
	{
		pbarra = (VTBarra*)lisBAR->Items[nb];
		// determina ordenadas y1 e y2 atuais da Barra
		dy1 = pbarra->esq.y1 - area->y1;
		dy2 = pbarra->esq.y2 - area->y1;
		// redefine ordenadas Y da Barra
		pbarra->esq.y1 = area->y2 - dy1;
		pbarra->esq.y2 = area->y2 - dy2;
	}
}

// -----------------------------------------------------------------------------
bool __fastcall TMontaRede::LePontosCurva(VTCurva *curva, strCURVA &str)
{
	// variáveis locais
	strHM hm_ini, hm_fim;
	double valor[144];

	try
	{ // define pontos da Curva
		for (int np = 0; np < str.num_pat; np++)
		{
			hm_ini = str.patamar[np].hm_ini;
			hm_fim = str.patamar[np].hm_fim;
			for (int ind_val = 0; ind_val < str.num_valor; ind_val++)
			{
				valor[ind_val] = str.patamar[np].valor[ind_val];
			}
			if (!curva->SetPonto(hm_ini, hm_fim, valor, str.num_valor))
				return (false);
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TMontaRede::MontaEsquematicoReducao(void)
{
	// variáveis locais
	TList *lisREDUCAO;
	VTMRede *mrede;
	VTReducao *reducao;
	TList *lisMREDE = redes->LisMRede();

	for (int n = 0; n < lisMREDE->Count; n++)
	{
		mrede = (VTMRede*)lisMREDE->Items[n];
		if (mrede->Carregada)
			continue;
		lisREDUCAO = mrede->LisReducao();
		if (lisREDUCAO->Count == 1)
		{
			reducao = (VTReducao*)lisREDUCAO->First();
			reducao->IniciaEsquematico(patamares->NumPatamar());
		}
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TMontaRede::Ordena(VTRede *rede)
 {
 //variávei locais
 int        ligacao;
 double     vnom, tap;
 double     defasagem;
 strIMP     zterra;
 VTLigacao *pliga;
 VTTrafo   *ptrafo;
 VTBarra   *pbarra, *pbar[NBAR_LIG];

 //inicia lisBAR c/ a Barra inicial da Rede
 if ((pbarra = rede->BarraInicial()) == NULL) return;
 lisBAR->Clear();
 lisBAR->Add(pbarra);
 //inicia lisTMP c/ as Ligacoes da Rede
 lisTMP->Clear(); CopiaTList(rede->LisLigacao(), lisTMP);
 //ordena as Ligacoes e Barras da Rede
 for (int nbar = 0; nbar < lisBAR->Count; nbar++)
 {
 pbarra = (VTBarra*)lisBAR->Items[nbar];
 //loop p/ todas as ligações em lisTMP
 for (int nlig = lisTMP->Count-1; nlig >= 0; nlig--)
 {
 pliga = (VTLigacao*)lisTMP->Items[nlig];
 //verifica se a Ligacao conecta a Barra
 if ((pliga->pbarra1 == pbarra)||(pliga->pbarra2 == pbarra)||(pliga->pbarra3 == pbarra))
 {//retira Ligacao de lisTMP
 lisTMP->Delete(nlig);
 //ordena Barras da Ligacao
 if (pliga->pbarra2 == pbarra)
 {//inverte Barras 1 e 2 da Ligacao
 pliga->pbarra2 = pliga->pbarra1;
 pliga->pbarra1 = pbarra;
 }
 else if (pliga->pbarra3 == pbarra)
 {//inverte Barras 1 e 3 da Ligacao
 pliga->pbarra3 = pliga->pbarra1;
 pliga->pbarra1 = pbarra;
 }
 //insere Barras em lisBAR
 if ((pliga->pbarra2)&&(lisBAR->IndexOf(pliga->pbarra2) < 0)) lisBAR->Add(pliga->pbarra2);
 if ((pliga->pbarra3)&&(lisBAR->IndexOf(pliga->pbarra3) < 0)) lisBAR->Add(pliga->pbarra3);
 }
 //verifica se é um Trafo
 if (pliga->Tipo() == eqptoTRAFO)
 {//mantém compatibilidade de tensões do primário c/ barra 1
 ptrafo = (VTTrafo*)pliga;
 if (ptrafo->pri.vnom != ptrafo->pbarra1->vnom)
 {//salva dados do primário
 ligacao     = ptrafo->pri.ligacao;
 vnom        = ptrafo->pri.vnom;
 tap         = ptrafo->pri.tap;
 defasagem   = ptrafo->pri.defasagem;
 zterra      = ptrafo->pri.zterra;
 //atualiza dados do primário c/ os dados do secundário
 ptrafo->pri = ptrafo->sec;
 //atualiza dados do secundário c/ os dados salvos do primário
 ptrafo->sec.ligacao   = ligacao;
 ptrafo->sec.vnom      = vnom;
 ptrafo->sec.tap       = tap;
 ptrafo->sec.defasagem = defasagem;
 ptrafo->sec.zterra    = zterra;
 }
 }
 }
 }
 //limpa listas temporárias
 lisBAR->Clear();
 lisTMP->Clear();
 }
 */

// ---------------------------------------------------------------------------
void __fastcall TMontaRede::OrdenaBarraPorNome(VTLigacao *ligacao)
{
	// variáveis locais
	VTBarra *bar_aux;

	if (ligacao->pbarra2->Codigo.AnsiCompareIC(ligacao->pbarra1->Codigo) < 0)
	{ // inverte Barras
		bar_aux = ligacao->pbarra1;
		ligacao->pbarra1 = ligacao->pbarra2;
		ligacao->pbarra2 = bar_aux;
	}
}

//// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
//void __fastcall TMontaRede::RedefineId(void)
//{
//	// variáveis locais
//	VTRede *rede;
//	TList *lisREDE = redes->LisRede();
//
//	// redefine IDs de todos Eqptos
//	RedefineId(arranjos->LisSuporte()); // DVK 2013.05.21
//	RedefineId(arranjos->LisCabo()); // DVK 2013.05.21
//	RedefineId(arranjos->LisArranjo()); // DVK 2013.05.21
//	RedefineId(redes->LisDominio());
//	RedefineId(redes->LisRede());
//	RedefineId(redes->LisCorte());
//	RedefineId(redes->LisBarra());
//	lisTMP->Clear();
//	redes->LisEqbar(lisTMP);
//	RedefineId(lisTMP);
//	lisTMP->Clear();
//	redes->LisLigacao(lisTMP);
//	RedefineId(lisTMP);
//	lisTMP->Clear();
//	redes->LisReducao(lisTMP);
//	RedefineId(lisTMP);
//	lisTMP->Clear();
//	redes->LisMutua(lisTMP);
//	RedefineId(lisTMP);
//	lisTMP->Clear();
//	// reinicia id das Barras inicias das redes
//	for (int n = 0; n < lisREDE->Count; n++)
//	{
//		rede = (VTRede*)lisREDE->Items[n];
//		rede->DefineBarraInicial(rede->BarraInicial());
//	}
//	// importante: reordena lista de Barras de VTRedes
//	VTListeqp *lisBAR = (VTListeqp*)(redes->LisBarra());
//	lisBAR->Ordena();
//}// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TMontaRede::RedefineId(void)
{

	// redefine IDs de todos Eqptos
	redes->RedefineIds();
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
//void __fastcall TMontaRede::RedefineId(TList *lisEQP)
//{
//	// variáveis locais
//	VTEqpto *eqpto;
//
//	// loop p/ todos Eqptos
//	for (int n = 0; n < lisEQP->Count; n++)
//	{
//		eqpto = (VTEqpto*)lisEQP->Items[n];
//		// eqpto->Id = FalsoId();
//		if (eqpto->StatusNovo())
//			eqpto->Id = FalsoId();
//	}
//}

// ---------------------------------------------------------------------------
bool __fastcall TMontaRede::RedefineTipoChave(AnsiString &codigo, int &tipo)
{
	// variáveis locais
	AnsiString tipoStr;
	int tipoChave = -1;

	// determina tipo da chave
	if (codigo.AnsiCompare("CHAVE A OLEO") == 0)
		tipoChave = chaveSEC;
	else if (codigo.AnsiCompare("CHAVE FACA ABERTURA COM CARGA") == 0)
		tipoChave = chaveSEC;
	else if (codigo.AnsiCompare("CHAVE FACA ABERTURA SEM CARGA") == 0)
		tipoChave = chaveSEC;
	else if (codigo.AnsiCompare("CHAVE FACA TRIPOLAR COM LOADBUSTER") == 0)
		tipoChave = chaveSEC;
	else if (codigo.AnsiCompare("CHAVE FUSÍVEL ABERTURA COM CARGA") == 0)
		tipoChave = chaveBF;
	else if (codigo.AnsiCompare("CHAVE FUSÍVEL RELIGADORA") == 0)
		tipoChave = chaveRA;
	else if (codigo.AnsiCompare("CHAVE FUSÍVEL RELIGADORA 3 ESTÁGIOS") == 0)
		tipoChave = chaveRA;
	else if (codigo.AnsiCompare("CHAVE REMOTA ABERTURA COM CARGA") == 0)
		tipoChave = chaveSEC;
	else if (codigo.AnsiCompare("RELIGADOR DE TENSAO") == 0)
		tipoChave = chaveRA;
	else if (codigo.AnsiCompare("UNIDADE SECCIONADORA AUXILIAR") == 0)
		tipoChave = chaveSEC;
	// define código em função do tipo definido
	switch (tipoChave)
	{
	case chaveDJ:
		tipoStr = "Disjuntor";
		break;
	case chaveRA:
		tipoStr = "Religadora";
		break;
	case chaveBF:
		tipoStr = "Base Fusível";
		break;
	case chaveSEC:
		tipoStr = "Seccionadora";
		break;
	default:
		return (false);
	}
	// atualiza parâmetros passados por referência
	tipo = tipoChave;
	codigo = tipoStr;
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TMontaRede::RedefineVnomBarra(void)
{
	// variáveis locais
	TList *lisBAR_ZONA;
	TList *lisZONA;
	VTBarra *barra;
	VTZona *zona;
	VTZonas *zonas = (VTZonas*)apl->GetObject(__classid(VTZonas));

	// monta Zonas com opção de incluir Chave aberta nas Zonas (p/ definir tensões de cada Zona)
	zonas->AvisoEnabled = false;
	zonas->ChaveAbertaEnabled = true;
	zonas->Executa();
	// define tensões das Barras iguais às tensões das Zonas
	lisZONA = zonas->LisZona();
	for (int nz = 0; nz < lisZONA->Count; nz++)
	{
		zona = (VTZona*)lisZONA->Items[nz];
		if (zona->Tipo == zonaINDEFINIDA)
			continue;
		// define tensões das Barras da Zona
		lisBAR_ZONA = zona->LisBarra();
		for (int nb = 0; nb < lisBAR_ZONA->Count; nb++)
		{
			barra = (VTBarra*)lisBAR_ZONA->Items[nb];
			barra->vnom = zona->Vff_kv;
		}
	}
	// cancela opção de inclusão de chaves abertas na montagems das zonas
	zonas->ChaveAbertaEnabled = false;
}

// ---------------------------------------------------------------------------
void __fastcall TMontaRede::RetiraBarraIsolada(void)
{
	// variáveis locais
	TList *lisBAR;
	VTBarra *barra;

	// obtém lista de Barras isoladas
	lisBAR = redes->LisBarraIsolada();
	// retira Barras isoladas
	for (int nb = lisBAR->Count - 1; nb >= 0; nb--)
	{
		barra = (VTBarra*)lisBAR->Items[nb];
		redes->RemoveBarra(barra);
		// delete barra;
		DLL_DeleteEqpto(barra);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMontaRede::RetiraRede(VTRede *rede)
{
	// remove Rede de Redes e destrói objeto
	redes->RemoveRede(rede);
	DLL_DeleteEqpto(rede);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TMontaRede::TransformaIlhasEmRedes(void)
 {
 //variáveis locais
 VTPosMonta *posmonta = (VTPosMonta*)apl->GetObject(__classid(VTPosMonta));

 return(posmonta->TransformaIlhasEmRedes());
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TMontaRede::TrataFimDeDados(bool define_vnom)
{
	// variáveis locais
	VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	TList *lisREDE;

	// reinicia controle de inserção de Barras
	// InsereBarraStart();
	// finaliza eventual inserção de Barra que ficou pendente
	InsereBarraStop();
	// verifica se há Trafos c/ LTC cuja barra controlada possui geração
	ValidaLtcTrafo();
	// define Barra inicial das Redes
	lisREDE = redes->LisRede();
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		// verifica se a Barra inicial ainda não foi definida
		if ((rede->Carregada) && (rede->BarraInicial() == NULL))
		{ // determina Barra inicial
			rede->DefineBarraInicial(redes->ExisteBarra(rede->BarraInicialId));
		}
		// reinicia lista de Barra
		// rede->IniciaLisBarra();
	}
	// reconfigura Redes
	topologia->Reconfigura();
	// redefine IDs
	RedefineId();
	// define tensão nominal das Barras
	if (define_vnom)
		RedefineVnomBarra();
	// verifica se há barra sem coord.esquemático
	if (esquematico->ExisteBarraSemCoordEsq(redes))
	{ // inicia esquemático igual ao geo (e calcula áreas da rede)
		/*
		 if (! esquematico->IniciaEsquematicoIgualGeo(redes))
		 {//esquemático não foi montado: inicia áreas das redes
		 redes->CalculaArea();
		 }
		 */
		esquematico->IniciaEsquematicoIgualGeo(redes);
	}
	// monta esquemático das Reducoes
	MontaEsquematicoReducao();
	// limpa lista de Barras local
	lisBAR->Clear();
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TMontaRede::ValidaCaboR_GMR(strCABO &str)
{
	if (IsDoubleZero(str.r))
	{
		if (plog)
			plog->Add("Cabo " + str.codigo + " com resistência nula");
		return (false);
	}
	if (IsDoubleZero(str.gmr))
	{
		if (plog)
			plog->Add("Cabo " + str.codigo + " com raio médio geométrico nulo");
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TMontaRede::ValidaCaboZOZ1(strCABO &str)
{
	if (IsDoubleZero(str.z0.r) && IsDoubleZero(str.z0.x))
	{
		if (plog)
			plog->Add("Cabo " + str.codigo + " com impedância Zseq0 nula");
		return (false);
	}
	if (IsDoubleZero(str.z1.r) && IsDoubleZero(str.z1.x))
	{
		if (plog)
			plog->Add("Cabo " + str.codigo + " com impedância Zseq1 nula");
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/16968316
void __fastcall TMontaRede::ValidaComprimento(VTTrecho *trecho)
{//código comentado
    /*
	// variáveis locais
	int x1, y1, x2, y2;
	double comp_tre_km, comp_cal_km;
	AnsiString txt;

	// verifica se o Trecho possui coordenadas utm
	if (trecho->CoordenadasUtm_cm(x1, y1, x2, y2))
	{ // determina comprimento indicado
		comp_tre_km = trecho->Comprimento_km;
		// calcula comprimento em função das coordenadas utm
		comp_cal_km = double(Distancia_cm(x1, y1, x2, y2)) * 1e-5;
		if (!IsDoubleZero(comp_tre_km - comp_cal_km, 1e-3))
		{
			txt.sprintf("Trecho c/ comprimento(%6.5fkm) diferente do calculado(%6.5fkm)",
				comp_tre_km, comp_cal_km);
			if (plog)
				plog->Add(txt);
		}
	}
    */
}

// -----------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17926800
void __fastcall TMontaRede::ValidaLtcTrafo(void)
{
	// variáveis locais
	VTBarra *barra;
	VTTrafo *ptrafo;

	// obtém lista de Trafos
	lisTMP->Clear();
	redes->LisLigacao(lisTMP, eqptoTRAFO);
	// loop p/ todos Trafos
	for (int n = 0; n < lisTMP->Count; n++)
	{
		ptrafo = (VTTrafo*)lisTMP->Items[n];
		// verifica se o Trafo possui LTC
		if ((ptrafo->ltc.tipo != ltcFIXO) && (ptrafo->ltc.ajuste_auto.bar_ref != NULL))
		{ // verifica se a Barra controlada possui um eqpto Suprimento ou Gerador (Redução não vale)
			if (ptrafo->ltc.ajuste_auto.bar_ref->ExisteGeracao(false))
			{ // desabilita o LTC
				ptrafo->ltc.tipo = ltcFIXO;
			}
		}
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TMontaRede::ValidaPotCC(VTSuprimento *suprimento)
{
	// variáveis locais
	complex<double>pcc_ft(suprimento->pcc_ft.p, suprimento->pcc_ft.q);
	complex<double>pcc_3f(suprimento->pcc_3f.p, suprimento->pcc_3f.q);

	// verifica valores nulos
	if (IsDoubleZero(suprimento->pcc_3f.p) && IsDoubleZero(suprimento->pcc_3f.q))
		return (false);
	if (IsDoubleZero(suprimento->pcc_ft.p) && IsDoubleZero(suprimento->pcc_ft.q))
		return (false);
	// verifica se pcc_3f > pcc_ft
	// if (Abs(pcc_3f) < Abs(pcc_ft)) return(false);

	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TMontaRede::ValidaEsquematicoEqbar(strESQ_EQBAR &str_esq)
{
	// valida posbar
	if (str_esq.posbar < 0)
		str_esq.posbar = 0;
	else if (str_esq.posbar > 100)
		str_esq.posbar = 100;
	// valida dxbar e dybar nulos
	if ((str_esq.dxbar == 0) && (str_esq.dybar == 0))
	{
		str_esq.dxbar = 2000;
		str_esq.dybar = -2000;
	}
	/*
	 //valida dxbar
	 if      (str_esq.dxbar < -2000) str_esq.dxbar = -2000;
	 else if (str_esq.dxbar >  2000) str_esq.dxbar =  2000;
	 //valida dybar
	 if      (str_esq.dybar < -2000) str_esq.dybar = -2000;
	 else if (str_esq.dybar >  2000) str_esq.dybar =  2000;
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TMontaRede::ValidaEsquematicoLigacao(strESQ_LIGA &str_esq)
{
	// valida posbar1
	if (str_esq.posbar1 < 0)
		str_esq.posbar1 = 0;
	else if (str_esq.posbar1 > 100)
		str_esq.posbar1 = 100;
	// valida posbar2
	if (str_esq.posbar2 < 0)
		str_esq.posbar2 = 0;
	else if (str_esq.posbar2 > 100)
		str_esq.posbar2 = 100;
	// valida posbar3
	if (str_esq.posbar3 < 0)
		str_esq.posbar3 = 0;
	else if (str_esq.posbar3 > 100)
		str_esq.posbar3 = 100;
}

// -----------------------------------------------------------------------------
bool __fastcall TMontaRede::ValidaZeqv(VTSuprimento *suprimento)
{
	// variáveis locais
	complex<double>zeq0(suprimento->zeq0.r, suprimento->zeq0.x);
	complex<double>zqv1(suprimento->zeq1.r, suprimento->zeq1.x);

	// verifica valores negativos
	if ((suprimento->zeq0.r < 0) || (suprimento->zeq0.x < 0))
		return (false);
	if ((suprimento->zeq1.r < 0) || (suprimento->zeq1.x < 0))
		return (false);
	// verifica Zeqv nulo
	if (IsDoubleZero(suprimento->zeq0.r) && IsDoubleZero(suprimento->zeq0.x))
		return (false);
	if (IsDoubleZero(suprimento->zeq1.r) && IsDoubleZero(suprimento->zeq1.x))
		return (false);
	// verifica Zeqv0 > Zeqv1
	// if (Abs(zeq0) < Abs(zqv1)) return(false);

	return (true);
}

// -----------------------------------------------------------------------------
// eof
