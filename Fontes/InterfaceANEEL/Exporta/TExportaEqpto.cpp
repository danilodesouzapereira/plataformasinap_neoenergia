//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <Math.hpp>
#include "TExportaEqpto.h"
#include "..\BDaneel\VTBDaneel.h"
#include "..\Importa\VTExtrator.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjo.h>
#include <Fontes\Classes\VTClasse.h>
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Curvas\VTCurvas.h>
#include <Fontes\Log\VTLog.h>
#include <Fontes\Potencia\VTDemanda.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTChave.h>
#include <Fontes\Rede\VTFases.h>
#include <Fontes\Rede\VTGerador.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRegulador.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTRegulador.h>
#include <Fontes\Rede\VTSuprimento.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Rede\VTTrecho.h>
#include <Fontes\Zona\VTZona.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTExportaEqpto* NewObjExportaEqpto(VTApl *apl)
   {
	return(new TExportaEqpto(apl));
   }

//---------------------------------------------------------------------------
__fastcall TExportaEqpto::TExportaEqpto(VTApl *apl)
   {
   //salva ponteiros p/ objetos
	this->apl = apl;
	BDaneel   = (VTBDaneel*)apl->GetObject(__classid(VTBDaneel));
	fases     = (VTFases*)apl->GetObject(__classid(VTFases));
	demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	//cria objetos
	lisEQP    = new TList();
	//inicia dados
	codbase   = 1;
	}

//---------------------------------------------------------------------------
__fastcall TExportaEqpto::~TExportaEqpto(void)
   {
	//destrói lista sem destruir objetos
	if (lisEQP) {delete lisEQP; lisEQP = NULL;}
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::LigacaoAsStr(int ligacao)
   {
   switch(ligacao)
      {
      case lgEST_AT:        return("Yaterrado");
      case lgTRIANG:        return("Delta");
      case lgEST_ISO:       return("Yisolado");
      case lgMONOFASICO:    return("Monofasico");
		case lgDELTA_ABERTO:  return("DeltaAberto");
      case lgDELTA_FECHADO: return("DeltaFechado");
      default:              return("Indefinido");
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto::MontaCargaBT(VTRede *rede, VTCarga *carga, sdi_CARGA_BT &sdi)
	{
	//variáveis locais
	double energia_mwhmes;
	int    rede_associada = 0;
	int    tipo_medicao   = 1;

	//preenche sdi_CARGA_BT
	sdi.CodBase             = codbase;
	sdi.CodTrafo            = rede->Codigo;
	sdi.CodRamal            = "";
	sdi.CodConsumidorBT     = carga->Codigo;
	sdi.CodFases            = fases->AsStrABCN[carga->Fases];
	sdi.CodPontoAcoplamento = carga->pbarra->Codigo;
	sdi.SemRedeAssociada    = rede_associada;
	sdi.TipoMedicao         = tipo_medicao;
	//código da curva típica
	if (carga->curva->Tipica)
		{sdi.TipologiaCurvaCarga = carga->curva->Codigo;}
	else
		{sdi.TipologiaCurvaCarga = carga->classe->Codigo;}
	//determina energia mensal da Carga
	if (carga->classe->Tag == consIP)
		{energia_mwhmes = demanda->Emes_mwh[carga];}
	else
		{energia_mwhmes = carga->Energia_mwhmes;}
	//consumo em MWh/mês
	for (int n = 0; n < 12; n++)
		{
		sdi.Energia_mwhmes[n] =  energia_mwhmes;
		}
	sdi.Descricao = carga->Codigo;

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto::MontaCargaMT(VTRede *rede, VTCarga *carga, sdi_CARGA_MT &sdi)
	{
	//variáveis locais
	double energia_mwhmes;
	int    rede_associada = 0;

	//preenche sdi_CARGA_MT
	sdi.CodBase             = codbase;
	sdi.CodAlimentador      = rede->Codigo;
	sdi.CodConsumidorMT     = carga->Codigo;
	sdi.CodFases            = fases->AsStrABCN[carga->Fases];
	sdi.CodPontoAcoplamento = carga->pbarra->Codigo;
	sdi.SemRedeAssociada    = rede_associada;
	//código da curva típica
	if (carga->curva->Tipica)
		{sdi.TipologiaCurvaCarga = carga->curva->Codigo;}
	else
		{sdi.TipologiaCurvaCarga = carga->classe->Codigo;}
	//consumo em MWh/mês
	for (int n = 0; n < 12; n++)
		{
		sdi.Energia_mwhmes[n] = carga->Energia_mwhmes;
		}
	sdi.Descricao = carga->Codigo;
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto::MontaChaveBT(VTChave *chave, sdi_CHAVE_BT &sdi)
	{
	//preenche sdi_CHAVE_BT
	sdi.CodBase              = codbase;
	sdi.CodTrafo             = chave->rede->Codigo;
	sdi.CodChave             = chave->Codigo;
	sdi.CodPontoAcoplamento1 = chave->pbarra1->Codigo;
	sdi.CodPontoAcoplamento2 = chave->pbarra2->Codigo;
	sdi.CodFases             = fases->AsStrABCN[chave->pbarra1->zona->Fases];
	sdi.Estado               = chave->Aberta ? 1 : 2;
	sdi.Descricao            = chave->Codigo;

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto:: MontaChaveMT(VTChave *chave, sdi_CHAVE_MT &sdi)
	{
	//preenche sdi_CHAVE_MT
	sdi.CodBase              = codbase;
	sdi.CodAlimentador       = chave->rede->Codigo;
	sdi.CodChave             = chave->Codigo;
	sdi.CodPontoAcoplamento1 = chave->pbarra1->Codigo;
	sdi.CodPontoAcoplamento2 = chave->pbarra2->Codigo;
	sdi.CodFases             = fases->AsStrABCN[chave->pbarra1->zona->Fases];
	sdi.Estado               = chave->Aberta ? 1 : 2;
	sdi.Descricao            = chave->Codigo;

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto::MontaCircuitoMT(VTRede *rede_sub, VTRede *rede_pri, sdi_CIRCUITO &sdi)
	{
	//variáveis locais
	int          tipico         = 0;
	double       energia_mwhmes = 0;
	double       perda_mwhmes   = 0;
	double       vnom_kv, vope_pu;

	//determina tensão nominal
	if (! TensaoNominal(rede_pri, vnom_kv))  return(false);
	//determina tensão de operação
	if (rede_sub != NULL)
		{//obtém tensão de operação a partir da subestação
		if (! TensaoOperacao(rede_sub, vope_pu)) return(false);
		}
	else
		{//obtém tensão de operação a partir do primário
		if (! TensaoOperacao(rede_pri, vope_pu)) return(false);
		}
	//preenche sdi_CIRCUITO
	sdi.CodBase             = codbase;
	sdi.CodSubestacao       = (rede_sub == NULL) ? AnsiString("") : rede_sub->Codigo;
	sdi.CodAlimentador      = rede_pri->Codigo;
	sdi.Vnom_kv             = vnom_kv;
	sdi.Vope_pu             = vope_pu;
	sdi.CodPontoAcoplamento = rede_pri->BarraInicial()->Codigo;
	sdi.CircAtipico         = tipico;
	//energia medida
	for (int n = 0; n < 12; n++)
		{sdi.Energia_mwhmes[n] = energia_mwhmes;}
	//energia de perda
	for (int n = 0; n < 12; n++)
		{sdi.Perda_mwhmes[n] = perda_mwhmes;}
	//descrição
	sdi.Descricao           = rede_pri->Codigo;

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto::MontaCondutor(VTArranjo *arranjo, sdi_CONDUTOR &sdi)
	{
	//variáveis locais
	double       comp_1km = 1.0;
	double       yc1_mho;
	strIMP       z1_ohm;

	//obtém impedância Z1 do Arranjo
	if (! arranjo->Z1_ohm(comp_1km, z1_ohm, yc1_mho))
		{
		if (plog) plog->Add("Não foi possível obter z1 do Arranjo: " + arranjo->Codigo);
		return(false);
		}
	//preenche sdi_CIRCUITO
	sdi.CodBase     = codbase;
	sdi.CodCondutor = arranjo->Codigo;
	sdi.Rohm_km     = z1_ohm.r;
	sdi.Xohm_km     = z1_ohm.x;
	sdi.Imax_amp    = arranjo->Iadm_amp;
	sdi.Descricao   = arranjo->Codigo;

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto::MontaCurvaTipica(VTCurva *curva, sdi_CURVA &sdi)
	{
	//variáveis locais
	AnsiString tipo_dia;
	double     vetp_kw[SIZEOF_VET_96];

	//monta vetor de potência ativa com 96 pontos
	MontaVetValoresCurva_kw(curva, vetp_kw, SIZEOF_VET_96);
	//define tipo de curva
	switch(curva->DiaMedicao)
		{
		case DOMINGO:         tipo_dia = "DO"; break;
		case SABADO:          tipo_dia = "SA"; break;
		case DIA_UTIL:        tipo_dia = "DU"; break;
		case SEMANA_COMPLETA: tipo_dia = "DU"; break;
		default:              tipo_dia = "DU"; break;
		}
	//preenche sdi_CURVA_CARGA
	sdi.CodBase   = codbase;
	sdi.CodCurva  = curva->Codigo;
	sdi.TipoDia   = tipo_dia;
	for (int n = 0; n < SIZEOF_VET_96; n++)
		{sdi.P_kw[n] = vetp_kw[n];}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto::MontaGeradorBT(VTRede *rede, VTGerador *gerador, sdi_GERADOR_BT &sdi)
	{
	//variáveis locais
	int            fases_tag      = faseABCN;
	double         energia_mwhmes = 0;

	//preenche sdi_GERADOR_BT
	sdi.CodBase              = codbase;
	sdi.CodRamal             = "";
	sdi.CodGerador           = gerador->Codigo;
	sdi.CodPontoAcoplamento = gerador->pbarra->Codigo;
	sdi.CodFases            = fases_tag;
	for (int n = 0; n < 12; n++)
		{sdi.Energia_mwhmes[n] = energia_mwhmes;}
	sdi.Descricao = gerador->Codigo;

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto::MontaGeradorMT(VTRede *rede, VTGerador *gerador, sdi_GERADOR_MT &sdi)
	{
	//variáveis locais
	int            fases_tag      = faseABCN;
	double         energia_mwhmes = 0;

	//preenche sdi_GERADOR_MT
	sdi.CodBase              = codbase;
	sdi.CodAlimentador       = rede->Codigo;
	sdi.CodGerador           = gerador->Codigo;
	sdi.CodPontoAcoplamento = gerador->pbarra->Codigo;
	sdi.CodFases            = fases_tag;
	for (int n = 0; n < 12; n++)
		{sdi.Energia_mwhmes[n] = energia_mwhmes;}
	sdi.Descricao = gerador->Codigo;

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto::MontaRamalBT(VTTrecho *trecho, sdi_RAMAL &sdi)
	{
	//variáveis locais
	double       comp_km;

	//proteção: define comprimento com pelo menos 1 metro
	if ((comp_km = trecho->Comprimento_km) < 0.001)
		{
		if (plog) plog->Add("Ramal com comprimento inferior a 1 metro: " + trecho->Codigo);
		comp_km = 0.001;
		}
	//preenche sdi_RAMAL_BT
	sdi.CodBase              = codbase;
	sdi.CodTrafo             = trecho->rede->Codigo;
	sdi.CodRamal             = trecho->Codigo;
	sdi.CodPontoAcoplamento1 = trecho->pbarra1->Codigo;
	sdi.CodPontoAcoplamento2 = trecho->pbarra2->Codigo;
	sdi.CodFases             = fases->AsStrABCN[trecho->arranjo->Fases];
	sdi.CodCondutor          = trecho->arranjo->Codigo;
	sdi.Comprimento_km       = comp_km;
	sdi.Descricao            = trecho->Codigo;;

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto::MontaReguladorMT(VTRegulador *regulador, int seq, int fases_tag, sdi_REGULADOR &sdi)
	{
	//variáveis locais
	int           tipo_regulador;
	double        energia_mwh_ano = 0;
	double        vreg_pu, dv_pu;
	double        r_perc, xhl_perc;
	struct{
			double  total_w;
			double  vazio_w;
			} perda;

	//calcula r_perc, xhl_perc
	r_perc   = regulador->z1.r * 100.;
	xhl_perc = (0.1 * (regulador->Spas_mva / 100.))*100.;
	//calcula perda total e em vazio
	perda.vazio_w = 0.;
	perda.total_w = perda.vazio_w + (regulador->z1.r * regulador->Spas_mva * 1.e6);
	//define ligacao
	switch(regulador->ligacao)
		{
		case lgMONOFASICO:    tipo_regulador = 1; break;
		case lgDELTA_ABERTO:  tipo_regulador = 2; break;
		case lgDELTA_FECHADO: tipo_regulador = 4; break;
		case lgEST_AT:        tipo_regulador = 1; break;
		case lgEST_ISO:       tipo_regulador = 1; break;
		}
	//define tensão regulada
	if ((regulador->modelo != reguENERQCT) && (regulador->modelo != reguFIXO))
		{
		vreg_pu = regulador->cfg_auto.v_pu;
		}
	else
		{
		dv_pu   = (regulador->var_tensao * 0.01) / regulador->num_passo;
		vreg_pu = 1.0 + (regulador->cfg_fixo.passo * dv_pu);
		}
	//preenche sdi_REGULADOR
	sdi.CodBase              = codbase;
	sdi.CodAlimentador       = regulador->rede->Codigo;
	sdi.CodRegulador         = regulador->Codigo;
	sdi.SeqBanco             = seq;
	sdi.CodPontoAcoplamento1 = regulador->pbarra1->Codigo;
	sdi.CodPontoAcoplamento2 = regulador->pbarra2->Codigo;
	sdi.Snom_kva             = (regulador->snom * 1000.) / 3.;
	sdi.TipoRegulador        = tipo_regulador;
	sdi.Vreg_pu              = vreg_pu;
	sdi.CodFasesPri          = fases->AsStrABCN[fases_tag];
	sdi.CodFasesSec          = fases->AsStrABCN[fases_tag];
	sdi.R_perc               = r_perc;
	sdi.Xhl_perc             = xhl_perc;
	sdi.PerdaTotal_w         = perda.total_w;
	sdi.PerdaVazio_w         = perda.vazio_w;
	sdi.Energia_mwhano       = energia_mwh_ano;
	sdi.Descricao            = regulador->Codigo;

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto::MontaSegmentoBT(VTTrecho *trecho, sdi_SEGMENTO_BT &sdi)
	{
	//variáveis locais
	double          comp_km;

	//proteção: define comprimento com pelo menos 1 metro
	if ((comp_km = trecho->Comprimento_km) < 0.001)
		{
		if (plog) plog->Add("Segmento BT com comprimento inferior a 1 metro: " + trecho->Codigo);
		comp_km = 0.001;
		}
	//preenche sdi_SEGMENTO_BT
	sdi.CodBase              = codbase;
	sdi.CodTrafo             = trecho->rede->Codigo;
	sdi.CodSegmento          = trecho->Codigo;
	sdi.CodPontoAcoplamento1 = trecho->pbarra1->Codigo;
	sdi.CodPontoAcoplamento2 = trecho->pbarra2->Codigo;
	sdi.CodFases             = fases->AsStrABCN[trecho->arranjo->Fases];
	sdi.CodCondutor          = trecho->arranjo->Codigo;
	sdi.Comprimento_km       = comp_km;
	sdi.Descricao            = trecho->Codigo;

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto::MontaSegmentoMT(VTTrecho *trecho, sdi_SEGMENTO_MT &sdi)
	{
	//variáveis locais
	double comp_km;

	//proteção: define comprimento com pelo menos 1 metro
	if ((comp_km = trecho->Comprimento_km) < 0.001)
		{
		if (plog) plog->Add("Segmento MT com comprimento inferior a 1 metro: " + trecho->Codigo);
		comp_km = 0.001;
		}
	//preenche sdi_SEGMENTO_BT
	sdi.CodBase              = codbase;
	sdi.CodAlimentador       = trecho->rede->Codigo;
	sdi.CodSegmento          = trecho->Codigo;
	sdi.CodPontoAcoplamento1 = trecho->pbarra1->Codigo;
	sdi.CodPontoAcoplamento2 = trecho->pbarra2->Codigo;
	sdi.CodFases             = fases->AsStrABCN[trecho->arranjo->Fases];
	sdi.CodCondutor          = trecho->arranjo->Codigo;
	sdi.Comprimento_km       = comp_km;
	sdi.ClasseSegmento       = "UR";
	sdi.Descricao            = trecho->Codigo;;

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto::MontaTrafoAT_MT(VTRede *rede_sub, VTTrafo *trafo, int seq, sdi_TRAFO_AT_MT &sdi)
	{
	//variáveis locais
	int             tipo_trafo;
	double          energia_mwhmes = 0;
	double          vnom_kv, vtap_pu;
	double          r_perc, x_perc;
	AnsiString      fases_pri, fases_sec, fases_ter;
	struct{
			double  total_perc;
			double  vazio_perc;
			} perda;

	//define tipo do trafo
	tipo_trafo = fases->NumeroDeFases(trafo->pri.fase);
	//determina fases do primário, secundário e terciário
	VerificaTrafoFases(trafo, fases_pri, fases_sec, fases_ter);
	//define perda total e no ferro
	perda.vazio_perc = trafo->perda_ferro;
	perda.total_perc = (trafo->z1.r * 100.) + trafo->perda_ferro;
	//preenche sdi_TRAFO_AT_MT
	sdi.CodBase           = codbase;
	sdi.CodSubestacao     = rede_sub->Codigo;
	sdi.CodTrafo          = trafo->Codigo;
	sdi.SeqBanco          = seq;
	sdi.TipoTrafo         = tipo_trafo;
	sdi.Vnom_pri_kv       = trafo->pri.vnom;
	sdi.Vnom_sec_kv       = trafo->sec.vnom;
	sdi.Vnom_ter_kv       = 0.;
	sdi.Snom_mva          = trafo->snom;
	sdi.PerdaVazio_perc   = perda.vazio_perc;
	sdi.PerdaTotal_perc   = perda.total_perc;
	for (int n = 0; n < 12; n++)
		{sdi.Energia_mwhmes[n] = energia_mwhmes;}
	sdi.SegAlocacaoPerda  = "A3";
	sdi.Propriedade       = "PR";
	sdi.Descricao         = trafo->Codigo;

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto::MontaTrafoMT_BT(VTRede *rede_pri, VTTrafo *trafo, int seq, sdi_TRAFO_MT_BT &sdi)
	{
	//variáveis locais
	int        tipo_trafo, mrt;
	double     vnom_kv, vtap_pu;
	double     r_perc, xhl_perc, xht_perc, xlt_perc;
	AnsiString fases_pri, fases_sec, fases_ter;
	struct{
			double  total_perc;
			double  vazio_perc;
			} perda;

	//define se trafo MRT
	mrt        = VerificaTrafoMRT(trafo);
	tipo_trafo = VerificaTrafoTipo(trafo);
	//determina fases do rpimário, secundário e terciário
	VerificaTrafoFases(trafo, fases_pri, fases_sec, fases_ter);
	//define tensão nominal do secundário
	vnom_kv = trafo->sec.vnom;
	vtap_pu = trafo->sec.tap / trafo->sec.vnom;
	if (tipo_trafo == 2)
		{//trafo monofásico de 3 enrolamentos
		vnom_kv = trafo->sec.vnom * 0.5;
		}
	//define perda total e no ferro
	perda.vazio_perc = trafo->perda_ferro;
	perda.total_perc = (trafo->z1.r * 100.) + trafo->perda_ferro;
	//define resistência e reatâncias
	r_perc   = trafo->z1.r * 100.;
	xhl_perc = trafo->z1.x * 100.;
	xht_perc = 0.;
	xlt_perc = 0.;
	if (tipo_trafo == 2)
		{//trafo monofásico de 3 enrolamentos
		xhl_perc = (trafo->z1.x * 100.) * 0.75;
		xht_perc = (trafo->z1.x * 100.) * 0.75;
		xlt_perc = (trafo->z1.x * 100.) * 0.5;
		}
	//preenche sdi_TRAFO_AT_MT
	sdi.CodBase              = codbase;
	sdi.CodAlimentador       = rede_pri->Codigo;
	sdi.CodTrafo             = trafo->rede->Codigo;
	sdi.SeqBanco             = seq;
	sdi.CodPontoAcoplamento1 = trafo->pbarra1->Codigo;
	sdi.CodPontoAcoplamento2 = trafo->pbarra2->Codigo;
	sdi.Snom_kva             = trafo->snom * 1000.;
	sdi.MRT                  = mrt;
	sdi.TipoTrafo            = tipo_trafo;
	sdi.CodFasesPri          = fases_pri;
	sdi.CodFasesSec          = fases_sec;
	sdi.CodFasesTer          = fases_ter;
	sdi.Vnom_sec_kv          = vnom_kv;
	sdi.Vtap_pu              = vtap_pu;
	sdi.R_perc               = r_perc;
	sdi.Xhl_perc             = xhl_perc;
	sdi.Xht_perc             = xht_perc;
	sdi.Xlt_perc             = xlt_perc;
	sdi.PerdaTotal_perc      = perda.total_perc;
	sdi.PerdaVazio_perc      = perda.vazio_perc;
	sdi.ClasseTrafo          = "UR";
	sdi.Propriedade          = "PR";
	sdi.Descricao            = trafo->Codigo;

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto::MontaVetValoresCurva_kw(VTCurva *curva, double *vetp_kw, int vetp_size)
	{
	//variáveis locais
	strHM  hm;
	int    num_valor_curva;
	double p_kw;
	double valor[nvCURVA_PFP_MDP];

	//proteção
	if (vetp_size != 96) return(false);
	//obtém número de valores da Curva
	num_valor_curva = curva->NumeroValores;
	//define o valor de potência a ser atribuído à curva
	if ((curva->Classe != NULL)&&(curva->Classe->Tag == consIP)) p_kw =   0.4;
	else                                                         p_kw = 100.0;
	//obtém os valores da Curva para as 24 horas
	hm.minuto = 0;
	for (hm.hora = 0; hm.hora < 24; hm.hora++)
		{
		if (! curva->GetValor(hm, valor, num_valor_curva)) return(false);
		//preenche o vetor vetp_kw
		for (int n = 0; n < 4; n++)
			{
			vetp_kw[(hm.hora * 4)+n] = valor[indP] * p_kw;
			}
		}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto::TensaoNominal(VTRede *rede_pri, double &vnom_kv)
	{
	//variáveis locais
	VTBarra *barra;

	//assume tensão de 13.8 kV
	vnom_kv = 13.8;
	if ((barra = rede_pri->BarraInicial()) == NULL) return(false);
	vnom_kv = barra->vnom;
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TExportaEqpto::TensaoOperacao(VTRede *rede_sub, double &vope_pu)
	{
	//variáveis locais
	VTSuprimento *suprimento;
	VTCurva      *curva;

	//assume tensão de 1 pu
	vope_pu = 1.0;
	//determina Suprimento da Subestacao
	lisEQP->Clear();
	rede_sub->LisEqbar(lisEQP, eqptoSUPRIMENTO);
	if (lisEQP->Count == 0) return(false);
	suprimento = (VTSuprimento*)lisEQP->First();
	//determina tensão de operação média do Suprimento
	vope_pu = suprimento->curva->Media(indV);

	return(true);
	}

//---------------------------------------------------------------------------
void __fastcall TExportaEqpto::VerificaTrafoFases(VTTrafo *trafo, AnsiString &fases_pri,
																  AnsiString &fases_sec, AnsiString &fases_ter)
	{
	//define fases do primário
	fases_pri = fases->AsStrABCN[trafo->pri.fase];
	fases_sec = fases->AsStrABCN[trafo->sec.fase];
	fases_ter = "";
	//verifica se Trafo monofásico
	if (trafo->Monofasico)
		{//verifica se trafo monofásico de 3 enrolamentos
		if ((fases->NumeroDeFases(trafo->sec.fase) == 2) && ((trafo->sec.fase & faseN) == faseN))
			{//trafo monofásico de 3 enrolamentos: redefine fases para secundário e terciário
			if ((trafo->sec.fase & faseAB) == faseAB)
				{
				fases_sec = "ANT";
				fases_ter = "BNT";
				}
			else if ((trafo->sec.fase & faseBC) == faseBC)
				{
				fases_sec = "BNT";
				fases_ter = "CNT";
				}
			else if ((trafo->sec.fase & faseCA) == faseCA)
				{
				fases_sec = "CNT";
				fases_ter = "ANT";
				}
			}
		}
	}

//---------------------------------------------------------------------------
int __fastcall TExportaEqpto::VerificaTrafoMRT(VTTrafo *trafo)
	{
	//variáveis locais
	int mrt = 0;

	if ((fases->NumeroDeFases(trafo->sec.fase) == 1) && ((trafo->pri.fase & faseN) == 0))
		{mrt = 1;}
	return(mrt);
	}

//---------------------------------------------------------------------------
int __fastcall TExportaEqpto::VerificaTrafoTipo(VTTrafo *trafo)
	{
	//variáveis locais
	int tipo;

	//define tipo do Trafo em função do número de fases do primário
	switch(fases->NumeroDeFases(trafo->pri.fase))
		{
		case 1:	//verifica número de fases do secundário
			if (fases->NumeroDeFases(trafo->sec.fase) == 1) tipo = 1;
			else                                            tipo = 2;
			break;
		case 2:
			tipo = 3;
			break;
		case 3:
			tipo = 4;
			break;
		default:
			tipo = 4;
			break;
		}
	//redefine tipo do trafo se ele estiver ligado em Delta aberto ou fechado
	switch(trafo->BarraSecundario()->zona->Tipo)
		{
		case zonaTRIFASICA:     tipo = 4; break;
		case zonaDELTA_FECHADO: tipo = 5; break;
		case zonaDELTA_ABERTO:  tipo = 6; break;
		}
	return(tipo);
	}

//---------------------------------------------------------------------------
//eof
