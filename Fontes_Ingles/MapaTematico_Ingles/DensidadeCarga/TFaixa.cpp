//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFaixa.h"
#include "VTCfgCor.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\Redegraf\VTCelula.h"
#include "..\Redegraf\VTCelulas.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TFaixa::TFaixa(void)
   {
   //cria lista
   lisCELULA = new TList();
   //inicia dados
   visible       = false;
   enabled       = false;
   color         = clBlack;
   dem_min_mva   = 0;
   dem_max_mva   = 0;
   dem_tot_mva   = 0;
   dem_tot_perc  = 0;
   num_cel_perc  = 0;
   }

//---------------------------------------------------------------------------
__fastcall TFaixa::~TFaixa(void)
   {
   //destrói lista sem destrui seus objetos
   if (lisCELULA) {delete lisCELULA; lisCELULA = NULL;}
   }

//---------------------------------------------------------------------------
//eof

