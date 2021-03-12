//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <Math.hpp>
#include "TExportaEqpto.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjo.h>
#include <Fontes\Classes\VTClasse.h>
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Curvas\VTCurvas.h>
#include <Fontes\Log\VTLog.h>
#include <Fontes\Potencia\VTDemanda.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCapacitor.h>
#include <Fontes\Rede\VTCapserie.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTChave.h>
#include <Fontes\Rede\VTCluster.h>
#include <Fontes\Rede\VTFases.h>
#include <Fontes\Rede\VTFiltro.h>
#include <Fontes\Rede\VTGerador.h>
#include <Fontes\Rede\VTMutua.h>
#include <Fontes\Rede\VTReator.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRegulador.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTRegulador.h>
#include <Fontes\Rede\VTSuprimento.h>
#include <Fontes\Rede\VTTipoChave.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <Fontes\Rede\VTTipos.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Rede\VTTrafo3E.h>
#include <Fontes\Rede\VTTrafoZZ.h>
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
	fases     = (VTFases*)apl->GetObject(__classid(VTFases));
	demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   //inicia dados
	Versao    = "Rev1.0";
   Separador = ';';
   }

//---------------------------------------------------------------------------
__fastcall TExportaEqpto::~TExportaEqpto(void)
   {
   //nada a fazer
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
AnsiString __fastcall TExportaEqpto::MontaArranjo(VTArranjo *arranjo)
   {
	//variáveis locais
	double     comp_1km = 1.0;
	double     yc1_mho;
	strIMP     z1_ohm;

	//reinicia Line
	line.Clear();
	line.Separador    = Separador;
	//obtém impedância Z1 do Arranjo
	if (! arranjo->Z1_ohm(comp_1km, z1_ohm, yc1_mho))
		{
		if (plog) plog->Add("Não foi possível obter z1 do Arranjo: " + arranjo->Codigo);
		}
	else
		{
		line.InsereCampo(arranjo->Obj);
		line.InsereCampo(arranjo->Codigo);
		line.InsereCampo(z1_ohm.r,  fmt5_4);
		line.InsereCampo(z1_ohm.x,  fmt5_4);
		line.InsereCampo(arranjo->Iadm_amp, fmt2_1);
		//descrição
		line.InsereCampo("");
		}
	return(line.Texto());
	}

/*
//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaCapacitor(VTCapacitor *capacitor)
	{
	//variáveis locais

	//reinicia Line
	line.Clear();
	line.Separador    = Separador;

	return(line.Texto());
	}
*/

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaCargaBT(VTRede *rede, VTCarga *carga)
	{
	//variáveis locais
	double energia_mwhmes;
	int    mes;
	int    rede_associada = 0;
	int    tipo_medicao   = 1;

	//reinicia Line
	line.Clear();
	line.Separador    = Separador;
   //monta linha com dados da Carga
	line.InsereCampo(carga->Obj);
	line.InsereCampo("CodigoRamal");
	line.InsereCampo(carga->Codigo);
	line.InsereCampo(fases->AsStrABCN[carga->Fases]);
//line.InsereCampo(carga->pbarra->Obj);
	line.InsereCampo(carga->pbarra->Codigo);
	line.InsereCampo(rede_associada);
	line.InsereCampo(tipo_medicao);
	if (carga->curva->Tipica)
		{line.InsereCampo(carga->curva->Codigo);}
	else
		{line.InsereCampo(carga->classe->Codigo);}
	//determina energia mensal da Carga
	if (carga->classe->Tag == consIP)
		{energia_mwhmes = demanda->Emes_mwh[carga];}
	else
		{energia_mwhmes = carga->Energia_mwhmes;}
	//consumo em MWh/mês
	mes = MonthOf(Data);
	for (int n = 1; n <= 12; n++)
		{
		line.InsereCampo((n == mes) ? energia_mwhmes : 0., fmt7_6);
		}
	return(line.Texto());
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaCargaMT(VTRede *rede, VTCarga *carga)
	{
	//variáveis locais
	double energia_mwhmes;
	int    mes;
	int    rede_associada = 0;

	//reinicia Line
	line.Clear();
	line.Separador    = Separador;
   //monta linha com dados da Carga
	line.InsereCampo(carga->Obj);
	line.InsereCampo(rede->Codigo);
	line.InsereCampo(carga->Codigo);
	line.InsereCampo(fases->AsStrABCN[carga->Fases]);
//line.InsereCampo(carga->pbarra->Obj);
	line.InsereCampo(carga->pbarra->Codigo);
	line.InsereCampo(rede_associada);
	if (carga->curva->Tipica)
		{line.InsereCampo(carga->curva->Codigo);}
	else
		{line.InsereCampo(carga->classe->Codigo);}
	//consumo em kWh/mês
	mes = MonthOf(Data);
	for (int n = 1; n <= 12; n++)
		{
		energia_mwhmes = (n == mes) ? carga->Energia_mwhmes : 0.;
		line.InsereCampo(energia_mwhmes, fmt4_3);
		}
	return(line.Texto());
	}

/*
//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaCargaIP(VTCarga *carga)
	{
	//variáveis locais

	//reinicia Line
	line.Clear();
	line.Separador    = Separador;

	return(line.Texto());
	}
*/

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaChaveBT(VTChave *chave)
	{
	//reinicia Line
	line.Clear();
	line.Separador    = Separador;
	//monta linha com dados da Chave
	line.InsereCampo(chave->Obj);
	line.InsereCampo(chave->rede->Codigo);
	line.InsereCampo("CodigoSegmento");
//line.InsereCampo(chave->pbarra1->Obj);
//line.InsereCampo(chave->pbarra2->Obj);
	line.InsereCampo(chave->pbarra1->Codigo);
	line.InsereCampo(chave->pbarra2->Codigo);
	line.InsereCampo(fases->AsStrABCN[chave->pbarra1->zona->Fases]);
	line.InsereCampo(chave->Aberta ? 1 : 2);
	return(line.Texto());
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto:: MontaChaveMT(VTChave *chave)
	{
	//reinicia Line
	line.Clear();
	line.Separador    = Separador;
	//monta linha com dados da Chave
	line.InsereCampo(chave->Obj);
	line.InsereCampo(chave->rede->Codigo);
	line.InsereCampo("");
//line.InsereCampo(chave->pbarra1->Obj);
//line.InsereCampo(chave->pbarra2->Obj);
	line.InsereCampo(chave->pbarra1->Codigo);
	line.InsereCampo(chave->pbarra2->Codigo);
	line.InsereCampo(fases->AsStrABCN[chave->pbarra1->zona->Fases]);
	line.InsereCampo(chave->Aberta ? 1 : 2);
	return(line.Texto());
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaCircuitoMT(VTRede *rede_sub, VTRede *rede_pri, double vnom_kv, double vope_pu)
	{
	//variáveis locais
	int    tipico = 0;
	double energia_mwhmes = 0;
	double perda_mwhmes = 0;

	//reinicia Line
	line.Clear();
	line.Separador    = Separador;
	//monta linha com dados da Rede
	line.InsereCampo(rede_pri->Obj);
	line.InsereCampo(rede_sub->Codigo);
	line.InsereCampo(rede_pri->Codigo);
	line.InsereCampo(vnom_kv, fmt4_3);
	line.InsereCampo(vope_pu, fmt4_3);
//line.InsereCampo(rede_pri->BarraInicial()->Obj);
	line.InsereCampo(rede_pri->BarraInicial()->Codigo);
	line.InsereCampo(tipico);
	//energia medida
	for (int n = 1; n <= 12; n++)
		{
		line.InsereCampo(energia_mwhmes, fmt4_3);
		}
	//energia de perda
	for (int n = 1; n <= 12; n++)
		{
		line.InsereCampo(energia_mwhmes, fmt4_3);
		}
	return(line.Texto());
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaCoordenada(VTBarra *barra)
	{
	//reinicia Line
	line.Clear();
	line.Separador    = Separador;
	//monta linha com dados da Barra
	line.InsereCampo(barra->Obj);
   line.InsereCampo(barra->Codigo);
	line.InsereCampo(double(barra->utm.x)/100., fmt3_2);
	line.InsereCampo(double(barra->utm.y)/100., fmt3_2);
	//line.InsereCampo(barra->vnom, fmt4_3);
	return(line.Texto());
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaGeradorBT(VTRede *rede, VTGerador *gerador)
	{
	//variáveis locais

	//reinicia Line
	line.Clear();
	line.Separador    = Separador;
	return(line.Texto());
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaGeradorMT(VTRede *rede, VTGerador *gerador)
	{
	//variáveis locais

	//reinicia Line
	line.Clear();
	line.Separador    = Separador;

	return(line.Texto());
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaRamalBT(VTTrecho *trecho)
	{
	//variáveis locais
	double comp_km;

	//proteção: define comprimento com pelo menos 1 metro
	if ((comp_km = trecho->Comprimento_km) < 0.001) comp_km = 0.001;
	//reinicia Line
	line.Clear();
	line.Separador    = Separador;
	//monta linha com dados do Trecho
	line.InsereCampo(trecho->Obj);
	line.InsereCampo(trecho->rede->Codigo);
	line.InsereCampo(trecho->Codigo);
//line.InsereCampo(trecho->pbarra1->Obj);
//line.InsereCampo(trecho->pbarra2->Obj);
	line.InsereCampo(trecho->pbarra1->Codigo);
	line.InsereCampo(trecho->pbarra2->Codigo);
	line.InsereCampo(fases->AsStrABCN[trecho->arranjo->Fases]);
	line.InsereCampo(trecho->arranjo->Codigo);
	line.InsereCampo(comp_km, fmt4_3);
	return(line.Texto());
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaReguladorMT(VTRegulador *regulador, int seq, int fases_tag)
	{
	//variáveis locais
	int    ligacao;
	double energia_mwh_ano = 0;
	double vreg_pu, dv_pu;
	double r_perc, xhl_perc;
	struct{
			double total_w;
			double vazio_w;
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
		case lgMONOFASICO:    ligacao = 1; break;
		case lgDELTA_ABERTO:  ligacao = 2; break;
		case lgDELTA_FECHADO: ligacao = 4; break;
		case lgEST_AT:        ligacao = 1; break;
		case lgEST_ISO:       ligacao = 1; break;
		}
	//define tensão regulada
	if (regulador->ajuste_auto)
		{
		vreg_pu = regulador->cfg_auto.v_pu;
		}
	else
		{
		dv_pu   = (regulador->var_tensao * 0.01) / regulador->num_passo;
		vreg_pu = 1.0 + (regulador->cfg_fixo.passo * dv_pu);
		}
	//reinicia Line
	line.Clear();
	line.Separador    = Separador;
	line.InsereCampo(regulador->rede->Codigo);
	line.InsereCampo(regulador->Codigo);
	line.InsereCampo(seq);
//line.InsereCampo(regulador->pbarra1->Obj);
//line.InsereCampo(regulador->pbarra2->Obj);
	line.InsereCampo(regulador->pbarra1->Codigo);
	line.InsereCampo(regulador->pbarra2->Codigo);
	//potência nominal de 1 unidade em kVA
	line.InsereCampo((regulador->snom * 1000.) / 3.);
	line.InsereCampo(ligacao);
	line.InsereCampo(vreg_pu, fmt4_3);
	//fases primário e secundário
	line.InsereCampo(fases->AsStrABCN[fases_tag]);
	line.InsereCampo(fases->AsStrABCN[fases_tag]);
	//r(%), xhl(%), perda total e perda em vazio
	line.InsereCampo(r_perc,   fmt4_3);
	line.InsereCampo(xhl_perc, fmt4_3);
	line.InsereCampo(perda.total_w, fmt4_3);
	line.InsereCampo(perda.vazio_w, fmt4_3);
	//energia medida ou estimada no secundário
	line.InsereCampo(energia_mwh_ano, fmt4_3);
	return(line.Texto());
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaSegmentoBT(VTTrecho *trecho)
	{
	//variáveis locais
	double comp_km;

	//proteção: define comprimento com pelo menos 1 metro
	if ((comp_km = trecho->Comprimento_km) < 0.001) comp_km = 0.001;
	//reinicia Line
	line.Clear();
	line.Separador    = Separador;
	//monta linha com dados do Trecho
	line.InsereCampo(trecho->Obj);
	line.InsereCampo(trecho->rede->Codigo);
	line.InsereCampo(trecho->Codigo);
//line.InsereCampo(trecho->pbarra1->Obj);
//line.InsereCampo(trecho->pbarra2->Obj);
	line.InsereCampo(trecho->pbarra1->Codigo);
	line.InsereCampo(trecho->pbarra2->Codigo);
	line.InsereCampo(fases->AsStrABCN[trecho->arranjo->Fases]);
	line.InsereCampo(trecho->arranjo->Codigo);
	line.InsereCampo(comp_km, fmt4_3);
	return(line.Texto());
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaSegmentoMT(VTTrecho *trecho)
	{
	//variáveis locais
	double comp_km;

	//proteção: define comprimento com pelo menos 1 metro
	if ((comp_km = trecho->Comprimento_km) < 0.001) comp_km = 0.001;
	//reinicia Line
	line.Clear();
	line.Separador    = Separador;
	//monta linha com dados do Trecho
	line.InsereCampo(trecho->Obj);
	line.InsereCampo(trecho->rede->Codigo);
	line.InsereCampo(trecho->Codigo);
//line.InsereCampo(trecho->pbarra1->Obj);
//line.InsereCampo(trecho->pbarra2->Obj);
	line.InsereCampo(trecho->pbarra1->Codigo);
	line.InsereCampo(trecho->pbarra2->Codigo);
	line.InsereCampo(fases->AsStrABCN[trecho->arranjo->Fases]);
	line.InsereCampo(trecho->arranjo->Codigo);
	line.InsereCampo(comp_km, fmt4_3);
	line.InsereCampo("UR");
	return(line.Texto());
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaTrafoAT_MT(VTRede *rede_sub, VTTrafo *trafo, int seq)
	{
	//variáveis locais
	int        tipo;
	double     vnom_kv, vtap_pu;
	double     r_perc, x_perc;
	double     perda_total, perda_vazio;
	AnsiString fases_pri, fases_sec, fases_ter;

	//define tipo do trafo
	tipo = fases->NumeroDeFases(trafo->pri.fase);
	//determina fases do primário, secundário e terciário
	VerificaTrafoFases(trafo, fases_pri, fases_sec, fases_ter);
	//define perda total e no ferro
	perda_vazio = trafo->perda_ferro;
	perda_total = (trafo->z1.r * 100.) + trafo->perda_ferro;
	//reinicia Line
	line.Clear();
	line.Separador    = Separador;
	//monta linha com dados do Trafo
	line.InsereCampo(trafo->Obj);
	line.InsereCampo(rede_sub->Codigo);
	line.InsereCampo(trafo->Codigo);
	line.InsereCampo(seq);
	//tipo
	line.InsereCampo(tipo);
	//tensão primário, secundário e terciário
	line.InsereCampo(trafo->pri.vnom, fmt4_3);
	line.InsereCampo(trafo->sec.vnom, fmt4_3);
	line.InsereCampo("");
	//potência nominal MVA
	line.InsereCampo(trafo->snom);
	//perda total e perda em vazio
	line.InsereCampo(perda_vazio, fmt4_3);
	line.InsereCampo(perda_total, fmt4_3);
	//energia medida nos 12 meses em MWh/mês
	for (int mes = 1; mes <= 12; mes++)
		{
		line.InsereCampo("0.000");
		}
	//identificação do nível de tensão a ser contabilizada as perdas técnicas
	line.InsereCampo("A3");
	//propriedade
	line.InsereCampo("PR");

	return(line.Texto());
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaTrafoMT_BT(VTRede *rede_pri, VTTrafo *trafo, int seq)
	{
	//variáveis locais
	int        tipo, mrt;
	double     vnom_kv, vtap_pu;
	double     r_perc, xhl_perc, xht_perc, xlt_perc;
	double     perda_total, perda_vazio;
	AnsiString fases_pri, fases_sec, fases_ter;

	//define se trafo MRT
	mrt  = VerificaTrafoMRT(trafo);
	tipo = VerificaTrafoTipo(trafo);
	//determina fases do rpimário, secundário e terciário
	VerificaTrafoFases(trafo, fases_pri, fases_sec, fases_ter);
	//define tensão nominal do secundário
	vnom_kv = trafo->sec.vnom;
	vtap_pu = trafo->sec.tap / trafo->sec.vnom;
	if (tipo == 2)
		{//trafo monofásico de 3 enrolamentos
		vnom_kv = trafo->sec.vnom * 0.5;
		}
	//define perda total e no ferro
	perda_vazio = trafo->perda_ferro;
	perda_total = (trafo->z1.r * 100.) + trafo->perda_ferro;
	//define resistência e reatâncias
	r_perc   = trafo->z1.r * 100.;
	xhl_perc = trafo->z1.x * 100.;
	xht_perc = 0.;
	xlt_perc = 0.;
	if (tipo == 2)
		{//trafo monofásico de 3 enrolamentos
		xhl_perc = (trafo->z1.x * 100.) * 0.75;
		xht_perc = (trafo->z1.x * 100.) * 0.75;
		xlt_perc = (trafo->z1.x * 100.) * 0.5;
		}
	//reinicia Line
	line.Clear();
	line.Separador    = Separador;
	//monta linha com dados do Trafo
	line.InsereCampo(trafo->Obj);
	line.InsereCampo(rede_pri->Codigo);
	line.InsereCampo(trafo->rede->Codigo);
	line.InsereCampo(seq);
//line.InsereCampo(trafo->pbarra1->Obj);
//line.InsereCampo(trafo->pbarra2->Obj);
	line.InsereCampo(trafo->pbarra1->Codigo);
	line.InsereCampo(trafo->pbarra2->Codigo);
	//potência nominal em kVA
	line.InsereCampo(trafo->snom * 1000., fmt2_1);
	line.InsereCampo(mrt);
	line.InsereCampo(tipo);
	//fases do primário, secundário e terciário
	line.InsereCampo(fases_pri);
	line.InsereCampo(fases_sec);
	line.InsereCampo(fases_ter);
	//tensão secundário
	line.InsereCampo(vnom_kv, fmt4_3);
	line.InsereCampo(vtap_pu, fmt4_3);
	///resistência, reatâncias
	line.InsereCampo(r_perc, fmt5_4);
	line.InsereCampo(xhl_perc, fmt5_4);
	line.InsereCampo(xht_perc, fmt5_4);
	line.InsereCampo(xlt_perc, fmt5_4);
	//perda total e perda em vazio
	line.InsereCampo(perda_total, fmt4_3);
	line.InsereCampo(perda_vazio, fmt4_3);
	//classificação e propriedade
	line.InsereCampo("UR");
	line.InsereCampo("PR");

	return(line.Texto());
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TExportaEqpto::MontaVersao(void)
	{
	//reinicia Line
	line.Clear();
	line.Separador    = Separador;

	line.InsereCampo("Sinap");
	line.InsereCampo("Interface ANEEL");
	line.InsereCampo(Versao);
	line.InsereCampo(DateToStr(Now()));
	return(line.Texto());
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
