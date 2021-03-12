//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <math.hpp>
#include "TFluxoG1.h"
#include "..\Apl\VTApl.h"
#include "..\Calculo\VTData.h"
#include "..\Flow\VTFlow.h"
#include "..\Gauss1\VTGauss1.h"
#include "..\MontaZ\VTMontaZ.h"
#include "..\Rede\VTCarga.h"
#include "..\..\DLL_Inc\Ordena.h"
#include "..\..\DLL_Inc\Gauss1.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
using namespace std;

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFluxo
//---------------------------------------------------------------------------
VTFluxo* __fastcall NewObjFluxoG1(VTApl *apl)
   {
   return(new TFluxoG1(apl));
   }

//---------------------------------------------------------------------------
__fastcall TFluxoG1::TFluxoG1(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl   = apl;
   //inicia dados
   RedeComCarga      = true;
   ModeloCarga       = cargaAUTO;
   PerdaFerroEnabled = false;
   RelatorioBarDesc  = false;  // Não gera relatório de barras desconexas
   TipoRelatorio     = relNENHUM;
   //cria objetos
   data   = DLL_NewObjData(apl);
   gauss1 = DLL_NewObjGauss1(apl);
   }

//---------------------------------------------------------------------------
__fastcall TFluxoG1::~TFluxoG1(void)
   {
   //destrói objetos
   if (data)    {delete data;    data    = NULL;}
   if (gauss1)  {delete gauss1;  gauss1  = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoG1::CfgAvisoErro(bool enabled)
   {
   aviso_enabled = enabled;
   }
   
//---------------------------------------------------------------------------
bool __fastcall TFluxoG1::Executa(VTNet      *net,
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
   gauss1->TipoRelatorio = TipoRelatorio;
   // Define impressão de relatório de barras desconexas
   gauss1->RelatorioBarDesc = RelatorioBarDesc;
   //executa Gauss1
   retorno = gauss1->Executa(data);
   //determina número de patamares calculados
   PD.num_patamares_ok = gauss1->NumPatamaresOK();

   return(retorno);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFluxoG1::Modelo(void)
   {
   return("Gauss Equilibrado");
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoG1::PM_GetEquilibrado(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoG1::UtilizaBarraPV(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoG1::ValidaAderencia(VTNet *net, VTLog *plog)
   {
   return(true);
   }

//---------------------------------------------------------------------------
//eof
