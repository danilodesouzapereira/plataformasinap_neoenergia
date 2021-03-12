//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Fontes\Constante\Fases.h>
#include <Fontes\Rede\VTEqpto.h>
#include <Fontes\Zona\VTZona.h>
#include <DLL_Inc\Funcao.h>
#include "TMonta.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMonta(TStrings *lines)
   {
   return(new TMonta(lines));
   }

//---------------------------------------------------------------------------
__fastcall TMonta::TMonta(TStrings *lines)
   {
   //salva ponteiros p/ objetos
   this->lines = lines;
   }

//---------------------------------------------------------------------------
__fastcall TMonta::~TMonta(void)
   {
   //nada a fazer
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TMonta::AlgoritmoDeCalculo(void)
	{
	return("Newton");
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TMonta::ModeloDeRede(void)
	{
	return("Modelo de rede: cadastro GIS");
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TMonta::MontaBusNodeCapacitor(odssBUS bus2, int ligacao_sinap)
   {
   //variáveis locais
   AnsiString nodes = "";

   switch(ligacao_sinap)
      {
      case lgTRIANG:  nodes.sprintf("%d",       bus2.sinap_id); break;
      case lgEST_ISO: nodes.sprintf("%d.5.5.5", bus2.sinap_id); break;
      case lgEST_AT:  nodes.sprintf("%d.0.0.0", bus2.sinap_id); break;
      default:        nodes.sprintf("%d.0.0.0", bus2.sinap_id); break;
      }
   return(nodes);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMonta::MontaBusNodeCarga(odssBUS bus, int fases_sinap)
   {
   //variáveis locais
   AnsiString nodes = "";

   //pré-análise: verifica se a carga possui uma única fase
   switch(fases_sinap & faseABC)
      {
      case faseA:
		case faseB:
		case faseC:
         //verifica se o bus possui neutro
         if (bus.sinap_fases & faseN)
            {//define Carga ligada ao neutro
            fases_sinap = fases_sinap | faseN;
            }
      }
   //verifica se a Carga deve ser conectada ao Node 4 ou ao Node 0
   if ((fases_sinap & faseN) && (bus.sinap_fases & faseT))
      {
      fases_sinap = fases_sinap | faseT;
      }
   //define os Nodes em função das fases
   switch(fases_sinap & bus.sinap_fases)
      {
      case faseA    : nodes = ".1.0";              break;
		case faseB    : nodes = ".2.0";              break;
		case faseC    : nodes = ".3.0";              break;
		//case faseN  : nodes = "";                  break;
		//case faseT  : nodes += "";                 break;
		case faseAN   : nodes = ".1.4";              break;
		case faseBN   : nodes = ".2.4";              break;
		case faseCN   : nodes = ".3.4";              break;
		case faseAT   : nodes = ".1.0";              break;
		case faseBT   : nodes = ".2.0";              break;
		case faseCT   : nodes = ".3.0";              break;
		//case faseNT : nodes = "";                  break;
		case faseANT  : nodes = ".1.0";              break;
		case faseBNT  : nodes = ".2.0";              break;
		case faseCNT  : nodes = ".3.0";              break;
		case faseAB   : nodes = ".1.2";              break;
		case faseBC   : nodes = ".2.3";              break;
		case faseCA   : nodes = ".3.1";              break;
		case faseABN  : nodes = ".1.2.4";            break;
		case faseBCN  : nodes = ".2.3.4";            break;
		case faseCAN  : nodes = ".3.1.4";            break;
		case faseABT  : nodes = ".1.2.0";            break;
		case faseBCT  : nodes = ".2.3.0";            break;
		case faseCAT  : nodes = ".3.1.0";            break;
		case faseABNT : nodes = ".1.2.0";            break;
		case faseBCNT : nodes = ".2.3.0";            break;
		case faseCANT : nodes = ".3.1.0";            break;
		case faseABC  : nodes = ".1.2.3 conn=delta"; break;
		case faseABCN : nodes = ".1.2.3.4";          break;
		case faseABCNT: nodes = ".1.2.3.0";          break;
		default:                                     break;
      }
   //monta string do bus
   return(IntToStr(bus.sinap_id) + nodes);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMonta::MontaBusNodeTrecho(odssBUS bus, int fases)
   {
   //variáveis locais
   AnsiString nodes = "";

   //define os Nodes em função das fases
   if (fases & faseA) nodes += ".1";
   if (fases & faseB) nodes += ".2";
   if (fases & faseC) nodes += ".3";
   if (fases & faseN)
      {//verifica se o bus tem o neutro aterrado
      if ((bus.sinap_fases & faseNT) == faseNT) nodes += ".0";
      else                                      nodes += ".4";
      }
   //monta string do bus
   return(IntToStr(bus.sinap_id) + nodes);
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TMonta::MontaBusNodeTrafo(odssBUS bus, int fases, bool polar_fase)
	{
	//variáveis locais
	int        num_fase;
	AnsiString nodes;

	//define os Nodes em função das fases, sem considerar NT
	switch(fases & faseABC)
		{
		case faseA    : num_fase = 1; nodes = ".1";       break;
		case faseB    : num_fase = 1; nodes = ".2";       break;
		case faseC    : num_fase = 1; nodes = ".3";       break;
		case faseAB   : num_fase = 2; nodes = ".1.2";     break;
		case faseBC   : num_fase = 2; nodes = ".2.3";     break;
		case faseCA   : num_fase = 2; nodes = ".3.1";     break;
		case faseABC  : num_fase = 3; nodes = ".1.2.3";   break;
		default:        num_fase = 3; nodes = ".1.2.3";   break;
      }
   //verifica se existe o neutro aterrado
   if ((fases & faseNT) == faseNT)
      {//verifica se polaridade na fase ou no neutro
      if (polar_fase) nodes += ".0";
      else            nodes  = ".0" + nodes;
      }
   //verifica se existe o neutro não aterrado
   else if ((fases & faseN) == faseN)
      {//verifica se polaridade na fase ou no neutro
      if (polar_fase) nodes += ".4";
      else            nodes  = ".4" + nodes;
      }
   //verifica se é apenas uma fase (sem neutro e terra)
   else if (num_fase == 1)
      {//verifica se polaridade na fase ou no neutro
      if (polar_fase) nodes += ".4";
      else            nodes  = ".4" + nodes;
      }
   //monta string do bus
   return(IntToStr(bus.sinap_id) + nodes);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta::NewCapacitor(odssCAPACITOR &odss)
   {
   //variáveis
   AnsiString texto, bus2_string;

   //define bus2 do Capacitor em função da sua ligação
   bus2_string = MontaBusNodeCapacitor(odss.bus2, odss.sinap_ligacao);
   //insere comando
   lines->Add(texto.sprintf("New Capacitor.%d bus1=%d bus2=%s phases=%d conn=%s kvar=%4.3f kv=%9.8f",
                            odss.sinap_id,
                            odss.bus1.sinap_id,
							bus2_string.c_str(),
							odss.phases,
							odss.conn.c_str(),
                            odss.q_kvar,
                            odss.vff_kv));
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta::NewCircuit(odssCIRCUIT &circuit)
   {
   //variáveis
   AnsiString texto;

   //insere comando New Circuit
   lines->Add("New Circuit." + circuit.codigo);
   //insere complemento do comando
   lines->Add(texto.sprintf("~ bus1=%d basekv=%9.8f pu=%4.3f phases=%d mvasc3=%2.1f %2.1f",
                            circuit.bus1.sinap_id,
                            circuit.vbase_kv,
                            circuit.vref_pu,
                            circuit.phases,
                            circuit.pcc3f_mw,
                            circuit.qcc3f_mvar));
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall  TMonta::NewFault(odssFAULT &fault)
{	//variáveis
   AnsiString texto;
   //ta hardcoded uma falta monofasica na A
   lines->Add(texto.sprintf("New Fault.%s bus1=%d.1 bus2=%d.0 phases=%d r=%4.3f ONtime=%4.3f",
							fault.codigo.c_str(),
							fault.bus1.sinap_id,
							fault.bus2.sinap_id,
							fault.phases,
							fault.r,
							fault.onTime));
   return(true);
}
//---------------------------------------------------------------------------
bool __fastcall TMonta::NewGeneratorPV(odssGENERATOR &generator)
   {
   //variáveis
   AnsiString texto;

   lines->Add(texto.sprintf("New Generator.%s phases=%d bus1=%d kv=%9.8f kw=%4.3f  model=%d",
							generator.codigo.c_str(),
							generator.phases,
							generator.bus1.sinap_id,
							generator.vnom_kv,
							generator.p_kw,
							generator.model));
   //insere complemento do comando
   lines->Add(texto.sprintf("~ vpu=%4.3f maxkvar=%4.3f  minkvar=%4.3f  pvfactor=%3.2f",
							generator.vref_pu,
							generator.qmax_kvar,
							generator.qmin_kvar,
							generator.pvfactor));
   return(true);
   }
//---------------------------------------------------------------------------
bool __fastcall TMonta::NewIndMach012(odssINDMACH012 &indmach012)
   {
   //variáveis
   AnsiString texto;
   AnsiString ligacao;


   switch (indmach012.sinap_ligacao)
   {
		case lgTRIANG:
			ligacao = "delta";
			break;
		case lgEST_AT:
			ligacao = "wye";
			break;
	   default:
		   ligacao = "delta";
   }
   lines->Add(texto.sprintf("New IndMach012.%s bus1=%d kv=%9.8f kW=%4.3f  kVA=%4.3f conn=%s",
							indmach012.codigo.c_str(),
							indmach012.bus1.sinap_id,
							indmach012.vnom_kv,
							indmach012.p_kw,
							indmach012.s_kva,
							ligacao.c_str()));
   //insere complemento do comando
   lines->Add(texto.sprintf("~ H=%4.3f purs=%4.3f  puxs=%4.3f  purr=%4.3f  puxr=%4.3f",
							indmach012.h_minercia,
							indmach012.rs_pu,
							indmach012.xs_pu,
							indmach012.rr_pu,
							indmach012.xr_pu));
   //insere complemento do comando
   lines->Add(texto.sprintf("~ puxm=%4.3f slip=%4.3f SlipOption=variableslip !debugtrace=yes",
							indmach012.xm_pu,
							indmach012.slip));
   return(true);
   }
//---------------------------------------------------------------------------
bool __fastcall TMonta::NewLine_ComLineCode(odssLINE &line)
   {
   //variáveis
   AnsiString texto, bus1_idnode, bus2_idnode;

   //monta strings de bus1 e bus2
   bus1_idnode = MontaBusNodeTrecho(line.bus1, line.sinap_fases);
   bus2_idnode = MontaBusNodeTrecho(line.bus2, line.sinap_fases);
   //monta string New Line
   lines->Add(texto.sprintf("New Line.%s phases=%d bus1=%s bus2=%s linecode=%s length=%5.4f",
                            line.codigo.c_str(),
                            line.phases,
							bus1_idnode.c_str(),
							bus2_idnode.c_str(),
							line.linecode.c_str(),
                            line.comp_km));
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta::NewLine_SemLineCode(odssLINE &line)
   {
   //variáveis
   AnsiString texto, bus1_idnode, bus2_idnode;

   //monta strings de bus1 e bus2
   bus1_idnode = MontaBusNodeTrecho(line.bus1, line.sinap_fases);
   bus2_idnode = MontaBusNodeTrecho(line.bus2, line.sinap_fases);
   lines->Add(texto.sprintf("New Line.%s phases=%d bus1=%s bus2=%s r1=%9.8f x1=%9.8f r0=%9.8f x0=%9.8f c1=%4.3f c0=%4.3f length=%5.4f",
                            line.codigo.c_str(),
                            line.phases,
                            //line.bus1.id,
                            //line.bus2.id,
							bus1_idnode.c_str(),
							bus2_idnode.c_str(),
                            line.r1_ohm_km,
                            line.x1_ohm_km,
                            line.r0_ohm_km,
                            line.x0_ohm_km,
                            line.c1_nF_km,
                            line.c0_nF_km,
                            line.comp_km));
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta::NewLineCode_MatZ(odssLINECODE &odss)
   {
   //variáveis
   AnsiString texto, rmatrix, xmatrix, cmatrix;

   //insere linha com dados gerais do LineCode
   lines->Add(texto.sprintf("New Linecode.%d nphases=%d basefreq=%d units=%s normamps=%2.1f kron=%s   !código=%s",
                            odss.id,
                            odss.nphases,
                            odss.basefreq,
							odss.units.c_str(),
                            odss.normamps,
                            odss.kron ? "Y" : "N",
							odss.codigo.c_str()));
   //monta strings de rmatrix, xmatix, cmatrix;
   for (int i = 0; i < odss.nphases; i++)
      {//inclui separador de linha da matriz
      if (i > 0)
         {
         rmatrix += "| ";
         xmatrix += "| ";
         cmatrix += "| ";
         }
      for (int j = 0; j <= i; j++)
         {
         rmatrix += DoubleToStr("%9.8f ", odss.rmatrix[i][j]);
         xmatrix += DoubleToStr("%9.8f ", odss.xmatrix[i][j]);
         cmatrix += DoubleToStr("%9.8f ", odss.cmatrix[i][j]);
         }
       }
   //insere linhas com as matrizes
   lines->Add(texto.sprintf("~ rmatrix = (%s)", rmatrix.c_str()));
   lines->Add(texto.sprintf("~ xmatrix = (%s)", xmatrix.c_str()));
//   lines->Add(texto.sprintf("//~ cmatrix = (%s)", cmatrix));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta::NewLineCode_Z0Z1(odssLINECODE &odss)
   {
   //variáveis
   AnsiString texto;

   lines->Add(texto.sprintf("New Linecode.%d nphases=%d basefreq=%d units=%s normamps=%2.1f r1=%9.8f x1=%9.8f c1=%4.3f   ! código=%s",
                            odss.id,
                            odss.nphases,
                            odss.basefreq,
							odss.units.c_str(),
                            odss.normamps,
                            odss.r1_ohm_km,
                            odss.x1_ohm_km,
                            odss.c1_nF_km,
							odss.codigo.c_str()));
   return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TMonta::NewLoad(odssLOAD &load)
   {
   //variáveis
   int        phases;
   double     vnom_kv;
   AnsiString texto;
   AnsiString bus1_string;

   //define tensão de linha ou de fase
   switch(load.sinap_numfase)
      {
      case 1:
         phases  = 1;
         vnom_kv = load.vfn_kv;
         break;
      case 2:   //verifica se há neutro
         if ((load.sinap_fases & faseN) == faseN)
            {//carga com 2 fases e neutro: verifica se está em  Zona monofásica
            phases  = 2;
            if (load.sinap_zona == zonaMONOFASICA) vnom_kv = load.vfn_kv;
            else                                   vnom_kv = load.vff_kv;
            }
         else
            {//define carga com uma fase e tensão fase/fase
            phases  = 1;
            vnom_kv = load.vff_kv;
            }
         break;
      case 3:
         phases  = 3;
         vnom_kv = load.vff_kv;
         break;
      default:
         return(false);
      }
   //define bus1 da Carga
   bus1_string = MontaBusNodeCarga(load.bus1, load.sinap_fases);
   //monta comando New Load
   lines->Add(texto.sprintf("New Load.%s phases=%d model=%d bus1=%s kv=%9.8f vminpu=%4.3f kw=%6.5f kvar=%6.5f daily=%d",
                            load.codigo.c_str(),
                            phases,
                            load.model,
							bus1_string.c_str(),
                            vnom_kv,
                            load.vmin_pu,
                            load.p_kw,
                            load.q_kvar,
                            load.loadshape_id));
   return(true);
   }


//---------------------------------------------------------------------------
bool __fastcall TMonta::NewLoadShape(odssLOADSHAPE &odss)
   {
   //variáveis
   AnsiString texto, curva_p;

   //proteção
   if (! odss.curva_p.enabled) return(false);
   //monta string com valores da curva_p
   for (int n = 0; n < odss.npts; n++)
      {
      if (n == 0) curva_p  = DoubleToStr("%9.8f",   odss.curva_p.valor[n]);
      else        curva_p += DoubleToStr(" %9.8f",  odss.curva_p.valor[n]);
      }
   //insere linha, definindo intervalo em horas ou minutos
   if (odss.npts == 24)
      {
      lines->Add(texto.sprintf("New Loadshape.%d  npts=24  interval=1  mult=(%s)    ! código=%s",
							   odss.id,
							   curva_p.c_str(),
							   odss.codigo.c_str()));
      }
   else
      {
      lines->Add(texto.sprintf("New Loadshape.%d  npts=%d  minterval=%d  mult=(%s)    ! código=%s",
                               odss.id,
                               odss.npts,
                               (24. * 60.)/odss.npts,
							   curva_p.c_str(),
							   odss.codigo.c_str()));
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta::NewLoadShape_pq(odssLOADSHAPE &odss)
   {
   //variáveis
   AnsiString texto, curva_p, curva_q;

   //proteção
   if (! odss.curva_p.enabled) return(false);
   if (! odss.curva_q.enabled) return(false);
   if (odss.npts != 24)        return(false);
   //monta string com valores da curva_p
   for (int n = 0; n < odss.npts; n++)
      {
      if (n == 0) curva_p  = DoubleToStr("%9.8f",   odss.curva_p.valor[n]);
      else        curva_p += DoubleToStr(" %9.8f",  odss.curva_p.valor[n]);
      }
   //monta string com valores da curva_q
   for (int n = 0; n < odss.npts; n++)
      {
      if (n == 0) curva_q  = DoubleToStr("%9.8f",   odss.curva_q.valor[n]);
      else        curva_q += DoubleToStr(" %9.8f",  odss.curva_q.valor[n]);
      }
   //insere linha, definindo intevalo em horas
   lines->Add(texto.sprintf("New Loadshape.%d  npts=24  interval=1   ! código=%s",
                            odss.id,
							odss.codigo.c_str()));
   lines->Add(texto.sprintf("~ pmult=(%s)",
							curva_p.c_str()));
   lines->Add(texto.sprintf("~ qmult=(%s)",
							curva_q.c_str()));
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall  TMonta::NewMonitor(odssMONITOR &monitor)
{
	//variáveis
   AnsiString texto;
   AnsiString ppolar;

   if (monitor.ppolar == true)
   {
		ppolar = "";
   }
   else
   {
		ppolar = "ppolar=no";
   }
   //insere linha, definindo intevalo em horas
   lines->Add(texto.sprintf("New Monitor.%s  element=%s  terminal=%d mode=%d %s ",
							monitor.codigo.c_str(),
							monitor.element.c_str(),
							monitor.terminal,
							monitor.mode,
							ppolar.c_str()));

   return(true);
}
//---------------------------------------------------------------------------
bool __fastcall TMonta::NewRegulador(odssREGCONTROL &odss)
   {
   //variáveis
   AnsiString texto;
   bool sucesso = false;

   struct{
         AnsiString conn, bus_nodes;
         }pri, sec;

      //determina tipo da conexão dos enrolamentos primário/secundario
   if (! TraduzTipoEnrolamentoTrafo(odss.win1.sinap_ligacao, odss.win1.sinap_fases, pri.conn)) return(false);
   if (! TraduzTipoEnrolamentoTrafo(odss.win2.sinap_ligacao, odss.win2.sinap_fases, sec.conn)) return(false);
   //define bus/nodes do primário/secundario
   pri.bus_nodes = MontaBusNodeTrafo(odss.win1.bus, odss.win1.sinap_fases);
   sec.bus_nodes = MontaBusNodeTrafo(odss.win2.bus, odss.win2.sinap_fases);
   //monta linha com dados gerais do regulador
   lines->Add(texto.sprintf("New Transformer.%s phases=%d windings=2 xhl=%6.5f %%loadloss=%6.5f %%noloadloss=%6.5f",
			  odss.codigo.c_str(),
              odss.phases,
              odss.xhl,
              odss.loadloss,
              odss.noloadloss));
   //monta linha com dados do enrolamento primário
   lines->Add(texto.sprintf("~ wdg=1 bus=%s conn=%s kv=%5.4f kva=%4.1f",
			  pri.bus_nodes.c_str(),
			  pri.conn.c_str(),
              odss.win1.vnom_kv,
              odss.win1.snom_kva));
   //monta linha com dados do enrolamento secundário
   if(odss.automatico)
      {
      lines->Add(texto.sprintf("~ wdg=2 bus=%s conn=%s kv=%5.4f kva=%4.1f numtap=%3.1f mintap=%3.2f maxtap=%3.2f",
			  sec.bus_nodes.c_str(),
			  sec.conn.c_str(),
              odss.win2.vnom_kv,
              odss.win2.snom_kva,
              odss.num_tap,
              odss.min_tap,
              odss.max_tap));
      lines->Add(texto.sprintf("New regcontrol.C%s transformer=%s winding=2 vreg=%d ptratio=%9.8f",
			  odss.codigo.c_str(),
			  odss.codigo.c_str(),
              odss.vreg,
              odss.ptratio));
      }
   else
      {
      lines->Add(texto.sprintf("~ wdg=2 bus=%s conn=%s kv=%5.4f kva=%4.1f tap=%5.4f",
			  sec.bus_nodes.c_str(),
			  sec.conn.c_str(),
              odss.win2.vnom_kv,
              odss.win2.snom_kva,
              //odss.win2.r_perc,
              odss.win2.vtap_pu));
	  }

      return sucesso;
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta::NewTransformer_1F2E(odssTRANSFORMER &odss)
   {
   //variáveis
   AnsiString texto;
   struct{
         AnsiString conn, bus_nodes;
         }pri, sec;

//ANALISAR MELHOR A DEINFIÇÃO DE BUS1/BUS2

   //determina tipo da conexão dos enrolamentos primário/secundario
   if (! TraduzTipoEnrolamentoTrafo(odss.win1.sinap_ligacao, odss.win1.sinap_fases, pri.conn)) return(false);
   if (! TraduzTipoEnrolamentoTrafo(odss.win2.sinap_ligacao, odss.win2.sinap_fases, sec.conn)) return(false);
   //define bus/nodes do primário/secundario
   pri.bus_nodes = MontaBusNodeTrafo(odss.win1.bus, odss.win1.sinap_fases);
   sec.bus_nodes = MontaBusNodeTrafo(odss.win2.bus, odss.win2.sinap_fases);
   //monta linha com dados gerais do trafo
   lines->Add(texto.sprintf("New Transformer.%s phases=%d windings=2 xhl=%6.5f %%loadloss=%6.5f %%noloadloss=%6.5f",
			  odss.codigo.c_str(),
              odss.phases,
              odss.xhl,
              odss.loadloss,
              odss.noloadloss));
   //monta linha com dados do enrolamento primário
   lines->Add(texto.sprintf("~ wdg=1 bus=%s conn=%s kv=%9.8f kva=%4.3f tap=%4.3f",
			  pri.bus_nodes.c_str(),
			  pri.conn.c_str(),
              odss.win1.vnom_kv,
              odss.win1.snom_kva,
              //odss.win1.r_perc,
              odss.win1.vtap_pu));
   //monta linha com dados do enrolamento secundário
   lines->Add(texto.sprintf("~ wdg=2 bus=%s conn=%s kv=%9.8f kva=%4.3f tap=%4.3f",
			  sec.bus_nodes.c_str(),
			  sec.conn.c_str(),
              odss.win2.vnom_kv,
              odss.win2.snom_kva,
              //odss.win2.r_perc,
              odss.win2.vtap_pu));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta::NewTransformer_1F3E(odssTRANSFORMER &odss)
   {
   //variáveis
   AnsiString texto;
   bool       polar_fase;
   struct{
         AnsiString conn, bus_nodes;
         }pri, sec, ter;

//ANALISAR MELHOR A DEINFIÇÃO DE BUS1/BUS2

   //determina tipo da conexão dos enrolamentos primário/secundario/terciário
   if (! TraduzTipoEnrolamentoTrafo(odss.win1.sinap_ligacao, odss.win1.sinap_fases, pri.conn)) return(false);
   if (! TraduzTipoEnrolamentoTrafo(odss.win2.sinap_ligacao, odss.win2.sinap_fases, sec.conn)) return(false);
   if (! TraduzTipoEnrolamentoTrafo(odss.win3.sinap_ligacao, odss.win3.sinap_fases, ter.conn)) return(false);
   //define bus/nodes do primário/secundario
   pri.bus_nodes = MontaBusNodeTrafo(odss.win1.bus, odss.win1.sinap_fases);
   sec.bus_nodes = MontaBusNodeTrafo(odss.win2.bus, odss.win2.sinap_fases);
   ter.bus_nodes = MontaBusNodeTrafo(odss.win3.bus, odss.win3.sinap_fases, polar_fase=false);
   //monta linha com dados gerais do trafo
   lines->Add(texto.sprintf("New Transformer.%s phases=%d windings=3 xhl=%9.8f xht=%9.8f xlt=%9.8f %%loadloss=%9.8f %%noloadloss=%9.8f",
			  odss.codigo.c_str(),
              odss.phases,
              odss.xhl,
              odss.xht,
              odss.xlt,
              odss.loadloss,
              odss.noloadloss));
   //monta linha com dados do enrolamento primário
   lines->Add(texto.sprintf("~ wdg=1 bus=%s conn=%s kv=%9.8f kva=%4.3f tap=%4.3f",
			  pri.bus_nodes.c_str(),
              pri.conn.c_str(),
              odss.win1.vnom_kv,
              odss.win1.snom_kva,
              //odss.win1.r_perc,
              odss.win1.vtap_pu));
   //monta linha com dados do enrolamento secundário
   lines->Add(texto.sprintf("~ wdg=2 bus=%s conn=%s kv=%9.8f kva=%4.3f tap=%4.3f",
			  sec.bus_nodes.c_str(),
			  sec.conn.c_str(),
              odss.win2.vnom_kv,
              odss.win2.snom_kva,
              //odss.win2.r_perc,
              odss.win2.vtap_pu));
   //monta linha com dados do enrolamento terciário
   lines->Add(texto.sprintf("~ wdg=3 bus=%s conn=%s kv=%9.8f kva=%4.3f tap=%4.3f",
			  ter.bus_nodes.c_str(),
			  ter.conn.c_str(),
              odss.win3.vnom_kv,
              odss.win3.snom_kva,
              //odss.win2.r_perc,
              odss.win3.vtap_pu));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta::NewTransformer_3F2E(odssTRANSFORMER &odss)
   {
   //variáveis
   AnsiString texto;
   struct{
         AnsiString conn;
         }pri, sec;

//ANALISAR MELHOR A DEINFIÇÃO DE BUS1/BUS2

   //determina tipo conexão dos enrolamentos primário/secundario
   if (! TraduzTipoEnrolamentoTrafo(odss.win1.sinap_ligacao, odss.win1.sinap_fases, pri.conn)) return(false);
   if (! TraduzTipoEnrolamentoTrafo(odss.win2.sinap_ligacao, odss.win2.sinap_fases, sec.conn)) return(false);
   //monta linha com dados gerais do trafo
   lines->Add(texto.sprintf("New Transformer.%s phases=%d windings=2 xhl=%9.8f %%loadloss=%9.8f %%noloadloss=%9.8f",
			  odss.codigo.c_str(),
              odss.phases,
              odss.xhl,
              odss.loadloss,
              odss.noloadloss));
   //monta linha com dados do enrolamento primário
   lines->Add(texto.sprintf("~ wdg=1 bus=%d conn=%s kv=%9.8f kva=%4.3f tap=%4.3f",
              odss.win1.bus.sinap_id,
			  pri.conn.c_str(),
              odss.win1.vnom_kv,
              odss.win1.snom_kva,
              //odss.win1.r_perc,
              odss.win1.vtap_pu));
   //monta linha com dados do enrolamento secundário
   lines->Add(texto.sprintf("~ wdg=2 bus=%d conn=%s kv=%9.8f kva=%4.3f tap=%4.3f",
              odss.win2.bus.sinap_id,
              sec.conn.c_str(),
              odss.win2.vnom_kv,
              odss.win2.snom_kva,
              //odss.win2.r_perc,
              odss.win2.vtap_pu));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta::NewTransformer_3F3E(odssTRANSFORMER &odss)
   {
Aviso("TMonta::NewTransformer_3F3E() - falta implementar");

   return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TMonta::TraduzTipoEnrolamentoTrafo(int ligacao, int fases, AnsiString &conn)
   {
   //assume ligação inválida
   conn = "";
   switch(ligacao)
      {
      case lgEST_AT:     conn = "wye";   break;
      case lgTRIANG:     conn = "delta"; break;
      case lgEST_ISO:    conn = "wye";   break;
      case lgMONOFASICO:
         //verifica se existe neutro
         if ((fases &faseN) == faseN) conn = "wye";
         else                         conn = "delta";
         break;
      default: return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

