//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <DateUtils.hpp>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Constante\Fases.h>
#include <Fontes\Importa\Monta\VTMonta.h>
#include <Fontes\Log\VTLog.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTFases.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Rede.h>
#include "TPreMonta1.h"

//---------------------------------------------------------------------------
VTPreMonta* NewObjPreMonta1(VTApl *apl)
   {
   try{//cria objeto TPreMonta1
      return(new TPreMonta1(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TPreMonta1::TPreMonta1(VTApl *apl)
                      :TPreMonta(apl)
   {
   //inicia controle indicando que não está importando uma rede secundária
   et.enabled = false;
   }

//---------------------------------------------------------------------------
__fastcall TPreMonta1::~TPreMonta1(void)
   {
   //nada a fazer
   }

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereCondutor(sdi_CONDUTOR &sdi)
	{
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereCoordenada(sdi_COORDENADA &sdi)
	{
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereCargaBT(sdi_CARGA_BT &sdi)
	{
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereCargaMT(sdi_CARGA_MT &sdi)
	{
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereChaveBT(sdi_CHAVE_BT &sdi)
	{
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereChaveMT(sdi_CHAVE_MT &sdi)
	{
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereCircuito(sdi_CIRCUITO &sdi)
	{
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereGeradorBT(sdi_GERADOR_BT &sdi)
	{
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereGeradorMT(sdi_GERADOR_MT &sdi)
	{
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereRamalBT(sdi_RAMAL_BT &sdi)
	{
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereReguladorMT(sdi_REGULADOR &sdi)
	{
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereSegmentoBT(sdi_SEGMENTO_BT &sdi)
	{
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereSegmentoMT(sdi_SEGMENTO_MT &sdi)
	{
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereTrafoAT_MT(sdi_TRAFO_AT_MT &sdi)
	{
	return(true);
	}

//-----------------------------------------------------------------------------
bool __fastcall TPreMonta1::InsereTrafoMT_BT(sdi_TRAFO_MT_BT &sdi)
	{
	return(true);
	}


//-----------------------------------------------------------------------------
// eof



