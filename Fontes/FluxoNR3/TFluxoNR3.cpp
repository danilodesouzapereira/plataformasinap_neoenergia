//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <math.hpp>
#include ".\TFluxoNR3.h"
#include "..\Apl\VTApl.h"
//#include "..\Arranjo\VTArranjo.h"
#include "..\Calculo\VTData.h"
//#include "..\Complexo\Complexo.h"
//#include "..\Diretorio\VTPath.h"
#include "..\Flow\VTFlow.h"
#include "..\NR3\VTNR3.h"
#include "..\Log\VTLog.h"
#include "..\MontaZ\VTMontaZ.h"
//#include "..\Patamar\VTpatamar.h"
//#include "..\Patamar\VTPatamares.h"
//#include "..\Potencia\VTDemanda.h"
//#include "..\Potencia\VTDemFase.h"
//#include "..\Potencia\VTFornecimento.h"
//#include "..\Potencia\VTListaDemFase.h"
//#include "..\Potencia\VTListaSupFase.h"
//#include "..\Potencia\VTSupFase.h"
//#include "..\Rede\VTBarra.h"
//#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCarga.h"
//#include "..\Rede\VTChave.h"
//#include "..\Rede\VTFases.h"
//#include "..\Rede\VTGerador.h"
//#include "..\Rede\VTLigacao.h"
//#include "..\Rede\VTMatz.h"
//#include "..\Rede\VTNo.h"
#include "..\Rede\VTNet.h"
//#include "..\Rede\VTReator.h"
//#include "..\Rede\VTRegulador.h"
//#include "..\Rede\VTResFlowBar.h"
//#include "..\Rede\VTResFlowLig.h"
//#include "..\Rede\VTResFlowSup.h"
//#include "..\Rede\VTSuprimento.h"
//#include "..\Rede\VTTramo.h"
//#include "..\Rede\VTTrecho.h"
//#include "..\Rede\VTTrafo.h"
//#include "..\Rede\VTTrafo3E.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Ordena.h"
#include "..\..\DLL_Inc\NR3.h"
//#include "..\..\DLL_Inc\MontaZ.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
using namespace std;

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFluxo
//---------------------------------------------------------------------------
VTFluxo* __fastcall NewObjFluxoNR3(VTApl *apl)
   {
   return(new TFluxoNR3(apl));
   }

//---------------------------------------------------------------------------
__fastcall TFluxoNR3::TFluxoNR3(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl    = apl;
	//demanda      = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	//fases        = (VTFases*)apl->GetObject(__classid(VTFases));
	//fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));
   //inicia constantes
   RedeComCarga      = true;
   ModeloCarga       = cargaAUTO;
   PerdaFerroEnabled = false;
   RelatorioBarDesc  = false;  // Não gera relatório de barras desconexas
   TipoRelatorio     = relNENHUM;
   //cria objetos
   data   = DLL_NewObjData(apl);
   nr3    = DLL_NewObjNR3(apl);
	//montaZ = DLL_NewObjMontaZ(apl);
	lisEQP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TFluxoNR3::~TFluxoNR3(void)
   {
   //destrói objetos
   if (data)    {delete data;    data    = NULL;}
   if (nr3)     {delete nr3;     nr3     = NULL;}
	//if (montaZ)  {delete montaZ;  montaZ  = NULL;}
   //destrói lista sem destruir seus objetos
	if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoNR3::CfgAvisoErro(bool enabled)
	{
   aviso_enabled = enabled;
   }
   
//---------------------------------------------------------------------------
bool __fastcall TFluxoNR3::Executa(VTNet      *net,
                                   double     Sbase,
                                   AnsiString str1,
                                   AnsiString str2)
   {
   // Variáveis locais
   bool retorno;
                                       
   // Salva ponteiros p/ objetos
	//this->net = net;

   // Inicia objeto Data
   data->RedeComCarga      = RedeComCarga;
   data->ModeloCarga       = ModeloCarga;
   data->PerdaFerroEnabled = PerdaFerroEnabled;
   data->Inicia(apl, net);
   //define o tipo de relatório a ser gerado
   nr3->TipoRelatorio = TipoRelatorio;
   // Define impressão de relatório de barras desconexas
   nr3->RelatorioBarDesc = RelatorioBarDesc;
   //executa NR3
   retorno = nr3->Executa(data);
   //determina número de patamares calculados
   PD.num_patamares_ok = nr3->NumPatamaresOK();

   return(retorno);
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TFluxoNR3::Modelo(void)
   {
	return("NewtonRaphson Desequilibrado");
	}
	
//---------------------------------------------------------------------------
bool __fastcall TFluxoNR3::PM_GetEquilibrado(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoNR3::UtilizaBarraPV(void)
	{
	return(true);
	}
/*
//---------------------------------------------------------------------------
bool __fastcall TFluxoNR3::ValidaAderencia(VTNet *net, VTLog *plog)
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
bool __fastcall TFluxoNR3::ValidaAderencia(VTNet *net, VTLog *plog)
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
