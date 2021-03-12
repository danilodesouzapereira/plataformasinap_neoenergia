//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Fontes\Constante\Fases.h>
#include <Fontes\Rede\VTEqpto.h>
#include <Fontes\Zona\VTZona.h>
#include <DLL_Inc\Funcao.h>
#include "TMontaAneel.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaAneel(TStrings *lines)
	{
	return(new TMontaAneel(lines));
   }

//---------------------------------------------------------------------------
__fastcall TMontaAneel::TMontaAneel(TStrings *lines)
							  :TMonta(lines)
   {
	//nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TMontaAneel::~TMontaAneel(void)
   {
   //nada a fazer
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TMontaAneel::AlgoritmoDeCalculo(void)
	{
	return("Normal");
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TMontaAneel::ModeloDeRede(void)
	{
	return("Modelo de rede: padrão ANEEL");
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TMontaAneel::MontaBusNodeCarga(odssBUS bus, int fases_sinap)
	{
	//variáveis locais
	AnsiString nodes = "";

	//define os Nodes em função das fases da carga
	switch(fases_sinap & bus.sinap_fases)
		{
      case faseA    : nodes = ".1.0";              break;
		case faseB    : nodes = ".2.0";              break;
		case faseC    : nodes = ".3.0";              break;
		//case faseN  : nodes = "";                  break;
		//case faseT  : nodes += "";                 break;
		case faseAN   : nodes = ".1.0";              break;
		case faseBN   : nodes = ".2.0";              break;
		case faseCN   : nodes = ".3.0";              break;
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
		case faseABN  : nodes = ".1.2.0";            break;
		case faseBCN  : nodes = ".2.3.0";            break;
		case faseCAN  : nodes = ".3.1.0";            break;
		case faseABT  : nodes = ".1.2.0";            break;
		case faseBCT  : nodes = ".2.3.0";            break;
		case faseCAT  : nodes = ".3.1.0";            break;
		case faseABNT : nodes = ".1.2.0";            break;
		case faseBCNT : nodes = ".2.3.0";            break;
		case faseCANT : nodes = ".3.1.0";            break;
		case faseABC  : nodes = ".1.2.3 conn=delta"; break;
		case faseABCN : nodes = ".1.2.3.0";          break;
		case faseABCNT: nodes = ".1.2.3.0";          break;
		default:                                     break;
      }
   //monta string do bus
   return(IntToStr(bus.sinap_id) + nodes);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMontaAneel::MontaBusNodeTrecho(odssBUS bus, int fases)
   {
   //variáveis locais
   AnsiString nodes = "";

   //define os Nodes em função das fases
	if (fases & faseA) nodes += ".1";
   if (fases & faseB) nodes += ".2";
   if (fases & faseC) nodes += ".3";
	//monta string do bus
	return(IntToStr(bus.sinap_id) + nodes);
	}

//---------------------------------------------------------------------------
//redefine o método porque usa o método MontaBusNodeTrecho() que foi redefinido
//---------------------------------------------------------------------------
bool __fastcall TMontaAneel::NewLine_ComLineCode(odssLINE &line)
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
//redefine o método porque usa o método MontaBusNodeTrecho() que foi redefinido
//---------------------------------------------------------------------------
bool __fastcall TMontaAneel::NewLine_SemLineCode(odssLINE &line)
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
//redefine o método: monta LineCode apenas com impedâncias de sweqência
//---------------------------------------------------------------------------
bool __fastcall TMontaAneel::NewLineCode_MatZ(odssLINECODE &odss)
	{
	//monta LineCode apenas com impedâncias de sequência
	return(NewLineCode_Z0Z1(odss));
	}

//---------------------------------------------------------------------------
//redefine o método porque usa o método MontaBusNodeCarga() que foi redefinido
//---------------------------------------------------------------------------
bool __fastcall TMontaAneel::NewLoad(odssLOAD &load)
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
//eof

