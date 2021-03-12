//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TResumo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTResumo* __fastcall NewObjResumo(void)
   {
   try{
      return (new TResumo());
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TResumo::TResumo()
   {
   //inicia atributos
   id              =-1;
   tipo_resumo     = resumoREDE;
   eqpto           = NULL;
   object          = NULL;
   rede.Id         = -1;
   rede.Codigo     = "";
   rede.TipoRedeId = -1;
   rede.TipoRede   = "";
   //inicia valores
   ZeraValores();
   }

//---------------------------------------------------------------------------
__fastcall TResumo::~TResumo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TResumo::Agrega(TList *lisRESUMO)
   {
   //variáveis locais
   VTResumo *resumo;

   //loop p/ todos Resumo em lisRESUMO
   for (int n = 0; n < lisRESUMO->Count; n++)
      {
      resumo = (VTResumo*)lisRESUMO->Items[n];
      Agrega(resumo);
      }
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17858579
void __fastcall TResumo::Agrega(VTResumo *resumo)
   {
   //agrega dados de rede
   rede.QtdeRede       += resumo->rede.QtdeRede;
   rede.QtdeTrafo      += resumo->rede.QtdeTrafo;
   rede.QtdeCabo       += resumo->rede.QtdeCabo;
   rede.QtdeCapacitor  += resumo->rede.QtdeCapacitor;
   rede.QtdeReator     += resumo->rede.QtdeReator;
   rede.QtdeRegulador  += resumo->rede.QtdeRegulador;
   rede.QtdeConsA4     += resumo->rede.QtdeConsA4;
   rede.QtdeConsRes    += resumo->rede.QtdeConsRes;
   rede.QtdeConsCom    += resumo->rede.QtdeConsCom;
   rede.QtdeConsInd    += resumo->rede.QtdeConsInd;
   rede.QtdeConsRur    += resumo->rede.QtdeConsRur;
   rede.QtdeConsOut    += resumo->rede.QtdeConsOut;
   rede.QtdeConsNulo   += resumo->rede.QtdeConsNulo;
   rede.EnergiaMercado += resumo->rede.EnergiaMercado;
   //não agrega valor de energia Medida
   //rede.EnergiaMedida += resumo->rede.EnergiaMedida;
   //agrega dados de perda
   perda.PerdaCabo        += resumo->perda.PerdaCabo;
   perda.PerdaTrafoCobre  += resumo->perda.PerdaTrafoCobre;
   perda.PerdaTrafoFerro  += resumo->perda.PerdaTrafoFerro;
   perda.PerdaCapacitor   += resumo->perda.PerdaCapacitor;
   perda.PerdaReator      += resumo->perda.PerdaReator;
   perda.PerdaRegulador   += resumo->perda.PerdaRegulador;
   perda.PerdaMedidor     += resumo->perda.PerdaMedidor;
   perda.PerdaRamal       += resumo->perda.PerdaRamal;
   perda.PerdaOutros      += resumo->perda.PerdaOutros;
   //agrega dados de balanco
   balanco.EnergiaEntrada     += resumo->balanco.EnergiaEntrada;
   balanco.EnergiaSuprimento  += resumo->balanco.EnergiaSuprimento;
   balanco.EnergiaGerador     += resumo->balanco.EnergiaGerador;
   balanco.EnergiaRecebidaExt += resumo->balanco.EnergiaRecebidaExt;
   balanco.EnergiaRecebidaInt += resumo->balanco.EnergiaRecebidaInt;
   balanco.EnergiaEntregueExt += resumo->balanco.EnergiaEntregueExt;
   balanco.EnergiaEntregueInt += resumo->balanco.EnergiaEntregueInt;
   balanco.EnergiaCarga       += resumo->balanco.EnergiaCarga;
   balanco.EnergiaPerdaFluxo  += resumo->balanco.EnergiaPerdaFluxo;
   balanco.EnergiaPerdaTotal  += resumo->balanco.EnergiaPerdaTotal;
   //agrega dados de perda passante
   energiaPassante.A1_A2   += resumo->energiaPassante.A1_A2;
   energiaPassante.A1_A3   += resumo->energiaPassante.A1_A3;
   energiaPassante.A1_A3A  += resumo->energiaPassante.A1_A3A;
   energiaPassante.A1_A4   += resumo->energiaPassante.A1_A4;
   energiaPassante.A2_A3   += resumo->energiaPassante.A2_A3;
   energiaPassante.A2_A3A  += resumo->energiaPassante.A2_A3A;
   energiaPassante.A2_A4   += resumo->energiaPassante.A2_A4;
   energiaPassante.A3_A2   += resumo->energiaPassante.A3_A2;
   energiaPassante.A3_A3A  += resumo->energiaPassante.A3_A3A;
   energiaPassante.A3_A4   += resumo->energiaPassante.A3_A4;
   energiaPassante.A3A_A2  += resumo->energiaPassante.A3A_A2;
   energiaPassante.A3A_A4  += resumo->energiaPassante.A3A_A4;
   energiaPassante.A3A_B   += resumo->energiaPassante.A3A_B;
   energiaPassante.A4_A2   += resumo->energiaPassante.A4_A2;
   energiaPassante.A4_A3   += resumo->energiaPassante.A4_A3;
   energiaPassante.A4_A3A  += resumo->energiaPassante.A4_A3A;
   energiaPassante.A4_A4   += resumo->energiaPassante.A4_A4;
   energiaPassante.A4_B    += resumo->energiaPassante.A4_B;
   energiaPassante.B_A3A   += resumo->energiaPassante.B_A3A;
   energiaPassante.B_A4    += resumo->energiaPassante.B_A4;
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17858579
void __fastcall TResumo::CopiaDe(VTResumo *resumo)
   {
   rede     = resumo->rede;
   perda    = resumo->perda;
   balanco  = resumo->balanco;
   energiaPassante = resumo->energiaPassante;
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17858579
void __fastcall TResumo::ZeraValores(void)
   {
   //agrega dados de rede
   rede.QtdeRede       = 0;
   rede.QtdeTrafo      = 0;
   rede.QtdeCabo       = 0;
   rede.QtdeCapacitor  = 0;
   rede.QtdeReator     = 0;
   rede.QtdeRegulador  = 0;
   rede.QtdeConsA4     = 0;
   rede.QtdeConsRes    = 0;
   rede.QtdeConsCom    = 0;
   rede.QtdeConsInd    = 0;
   rede.QtdeConsRur    = 0;
   rede.QtdeConsOut    = 0;
   rede.QtdeConsNulo   = 0;
   rede.EnergiaMedida  = 0;
   rede.EnergiaMercado = 0;
   //agrega dados de perda
   perda.PerdaCabo        = 0;
   perda.PerdaTrafoCobre  = 0;
   perda.PerdaTrafoFerro  = 0;
   perda.PerdaCapacitor   = 0;
   perda.PerdaReator      = 0;
   perda.PerdaRegulador   = 0;
   perda.PerdaMedidor     = 0;
   perda.PerdaRamal       = 0;
   perda.PerdaOutros      = 0;
   //agrega dados de balanco
   balanco.EnergiaEntrada     = 0; //estava comentada
   balanco.EnergiaSuprimento  = 0;
   balanco.EnergiaGerador     = 0;
   balanco.EnergiaRecebidaExt = 0;
   balanco.EnergiaRecebidaInt = 0;
   balanco.EnergiaEntregueExt = 0;
   balanco.EnergiaEntregueInt = 0;
   balanco.EnergiaCarga       = 0;
   balanco.EnergiaPerdaFluxo  = 0;
   balanco.EnergiaPerdaTotal  = 0;
   //agrega dados de perda passante
   energiaPassante.A1_A2	= 0;
   energiaPassante.A1_A3	= 0;
   energiaPassante.A1_A3A	= 0;
   energiaPassante.A1_A4	= 0;
   energiaPassante.A2_A3	= 0;
   energiaPassante.A2_A3A	= 0;
   energiaPassante.A2_A4	= 0;
   energiaPassante.A3_A2	= 0;
   energiaPassante.A3_A3A	= 0;
   energiaPassante.A3_A4	= 0;
   energiaPassante.A3A_A2	= 0;
   energiaPassante.A3A_A4	= 0;
   energiaPassante.A3A_B	= 0;
   energiaPassante.A4_A2	= 0;
   energiaPassante.A4_A3	= 0;
   energiaPassante.A4_A3A	= 0;
   energiaPassante.A4_A4	= 0;
   energiaPassante.A4_B		= 0;
   energiaPassante.B_A3A	= 0;
   energiaPassante.B_A4		= 0;
   }

//---------------------------------------------------------------------------
//eof

