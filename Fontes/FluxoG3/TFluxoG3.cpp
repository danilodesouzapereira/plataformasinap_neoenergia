//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <math.hpp>
#include "TFluxoG3.h"
#include "..\Apl\VTApl.h"
#include "..\Calculo\VTData.h"
#include "..\Flow\VTFlow.h"
#include "..\Gauss3\VTGauss3.h"
#include "..\Log\VTLog.h"
#include "..\MontaZ\VTMontaZ.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTNet.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Ordena.h"
#include "..\..\DLL_Inc\Gauss3.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
using namespace std;

//---------------------------------------------------------------------------
VTFluxo* __fastcall NewObjFluxoG3(VTApl *apl)
   {
   //variáveis locais
   bool g3_as_nr3 = false;

   return(new TFluxoG3(apl, g3_as_nr3));
   }

//---------------------------------------------------------------------------
VTFluxo* __fastcall NewObjFluxoG3_AS_NR3(VTApl *apl)
   {
   //variáveis locais
   bool g3_as_nr3 = true;

   return(new TFluxoG3(apl, g3_as_nr3));
   }

//---------------------------------------------------------------------------
__fastcall TFluxoG3::TFluxoG3(VTApl *apl, bool g3_as_nr3)
   {
   //salva ponteiro p/ objeto
   this->apl    = apl;
   //inicia dados
   utiliza_barraPV   = g3_as_nr3;
   RedeComCarga      = true;
   ModeloCarga       = cargaAUTO;
   PerdaFerroEnabled = false;
   RelatorioBarDesc  = false;  // Não gera relatório de barras desconexas
   TipoRelatorio     = relNENHUM;
   //cria objetos
   data   = DLL_NewObjData(apl);
   gauss3 = DLL_NewObjGauss3(apl);
   gauss3->ModelaBarraPV = g3_as_nr3;
   lisEQP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TFluxoG3::~TFluxoG3(void)
   {
   //destrói objetos
   if (data)    {delete data;    data    = NULL;}
   if (gauss3)  {delete gauss3;  gauss3  = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoG3::CfgAvisoErro(bool enabled)
   {
   //if (gauss3) gauss3->AvisoEnabled = enabled;
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoG3::Executa(VTNet      *net,
                                  double     Sbase,
                                  AnsiString str1,
                                  AnsiString str2)
   {
   // Variáveis locais
   bool retorno;

   // Inicia objeto Data
   data->RedeComCarga      = RedeComCarga;
   data->ModeloCarga       = ModeloCarga;
   data->PerdaFerroEnabled = PerdaFerroEnabled;
   data->Inicia(apl, net);
   //define o tipo de relatório a ser gerado
   gauss3->TipoRelatorio = TipoRelatorio;
   // Define impressão de relatório de barras desconexas
   gauss3->RelatorioBarDesc = RelatorioBarDesc;
   //executa Gauss3
   retorno = gauss3->ExecutaFluxo(data);
   //determina número de patamares calculados
   PD.num_patamares_ok = gauss3->NumPatamaresOK();

   return(retorno);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFluxoG3::Modelo(void)
   {
   return("Gauss Desequilibrado");
   }
   
//---------------------------------------------------------------------------
bool __fastcall TFluxoG3::PM_GetEquilibrado(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoG3::UtilizaBarraPV(void)
   {
   return(utiliza_barraPV);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TFluxoG3::ValidaAderencia(VTNet *net, VTLog *plog)
	{
	//variáveis locais
	VTZona *zona;

   //verifica fases dos Trafos
   if (! net->ValidaFasesTrafo())
      {//inclui mensagem em Log
      if (plog) plog->Add("Trafo com fases inconsistentes");
      return(false);
      }
	//obtém lista de Zona da Net
	lisEQP->Clear();
	net->LisZona(lisEQP);
	//verifica se todas Zonas apresentam aterramento
	for (int n = 0; n < lisEQP->Count; n++)
		{
		zona = (VTZona*)lisEQP->Items[n];
		if (! zona->ExisteFase(faseT))
			{//verifica se a Zona possui uma única Barra
         if (zona->LisBarra()->Count > 1)
            {//inclui mensagem em Log
			   if (plog) plog->Add("Inexistência de aterramento");
			   return(false);
            }
			}
		}
	return(true);
	}
*/
//---------------------------------------------------------------------------
bool __fastcall TFluxoG3::ValidaAderencia(VTNet *net, VTLog *plog)
	{
	//variáveis locais
	VTZona *zona;

   //verifica fases dos Trafos
   if (! net->ValidaFasesTrafo())
      {//inclui mensagem em Log
      if (plog) plog->Add("Trafo com fases inconsistentes");
      return(false);
      }
	return(true);
	}

//---------------------------------------------------------------------------
//eof
