//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <complex>
#include "TFluxoNR1.h"
#include "..\Calculo\VTData.h"
#include "..\Flow\VTFlow.h"
#include "..\Geral\VTGeral.h"
#include "..\Rede\VTEqpto.h"
#include "..\NR1\VTNR1.h"
#include "..\..\DLL_Inc\NR1.h"
#include "..\..\DLL_Inc\Ordena.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
using namespace std;

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFluxo
//---------------------------------------------------------------------------
VTFluxo* __fastcall NewObjFluxoNR1(VTApl *apl)
   {
   return(new TFluxoNR1(apl));
   }

//---------------------------------------------------------------------------
__fastcall TFluxoNR1::TFluxoNR1(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //inicia dados
   RedeComCarga      = true;
   ModeloCarga       = cargaAUTO;
   PerdaFerroEnabled = false;
   RelatorioBarDesc  = false;  // Não gera relatório de barras desconexas
   TipoRelatorio     = relNENHUM;
   //cria objetos
   data = DLL_NewObjData(apl);
   nr   = DLL_NewObjNR1(apl);
   }

//---------------------------------------------------------------------------
__fastcall TFluxoNR1::~TFluxoNR1(void)
   {
   //destrói objetos
   if (data) {delete data; data = NULL;}
   if (nr)   {delete nr;   nr = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFluxoNR1::CfgAvisoErro(bool enabled)
   {
   // nr->CfgAvisoErro(enabled);  // HPS, 20100607

   aviso_enabled = enabled;
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoNR1::Executa(VTNet *net, double Sbase,
                                   AnsiString arqRelFlow, AnsiString arqRelRede)
   {
   //variáveis locais
   bool retorno;

   // Inicia objeto Data
   data->RedeComCarga      = RedeComCarga;
   data->ModeloCarga       = ModeloCarga;
   data->PerdaFerroEnabled = PerdaFerroEnabled;
   data->Inicia(apl, net);
   //define o tipo de relatório a ser gerado
   nr->TipoRelatorio = TipoRelatorio;
   // Define impressão de relatório de barras desconexas
   nr->RelatorioBarDesc = RelatorioBarDesc;
   //executa NR1
   retorno = nr->Executa(data);
   //determina número de patamares calculados
   PD.num_patamares_ok = nr->NumPatamaresOK();

   return(retorno);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFluxoNR1::Modelo(void)
   {
   return("NewtonRaphson");
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoNR1::PM_GetEquilibrado(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoNR1::UtilizaBarraPV(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFluxoNR1::ValidaAderencia(VTNet *net, VTLog *plog)
   {
   return(true);
   }

//---------------------------------------------------------------------------
//eof
