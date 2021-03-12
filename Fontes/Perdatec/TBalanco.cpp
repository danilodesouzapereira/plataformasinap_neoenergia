//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "ConstantesPerda.h"
#include "TBalanco.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTBalanco* __fastcall NewObjBalanco(void)
   {
   return(new TBalanco());
   }

//---------------------------------------------------------------------------
__fastcall TBalanco::TBalanco(void)
   {
   //inicia valores
   EnergiaSuprimento_mwh  = 0.;
   EnergiaRecebidaExt_mwh = 0.;
   EnergiaRecebidaInt_mwh = 0.;
   EnergiaGerador_mwh     = 0.;
   EnergiaEntregueExt_mwh = 0.;
   EnergiaEntregueInt_mwh = 0.;
   EnergiaCarga_mwh       = 0.;
   EnergiaPerdaFluxo_mwh  = 0.;
   EnergiaPerdaFora_mwh   = 0.;
   EnergiaPerdaTotal_mwh  = 0.;
   }

//--------------EnergiaPerdaFora_mwh-------------------------------------------------------------
__fastcall TBalanco::~TBalanco(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TBalanco::Agrega(VTBalanco *balanco)
   {
   //soma energias
   EnergiaSuprimento_mwh  = EnergiaSuprimento_mwh  + balanco->EnergiaSuprimento_mwh;
   EnergiaRecebidaExt_mwh = EnergiaRecebidaExt_mwh + balanco->EnergiaRecebidaExt_mwh;
   EnergiaRecebidaInt_mwh = EnergiaRecebidaInt_mwh + balanco->EnergiaRecebidaInt_mwh;
   EnergiaGerador_mwh     = EnergiaGerador_mwh     + balanco->EnergiaGerador_mwh;
   EnergiaEntregueExt_mwh = EnergiaEntregueExt_mwh + balanco->EnergiaEntregueExt_mwh;
   EnergiaEntregueInt_mwh = EnergiaEntregueInt_mwh + balanco->EnergiaEntregueInt_mwh;
   EnergiaCarga_mwh       = EnergiaCarga_mwh       + balanco->EnergiaCarga_mwh;
   EnergiaPerdaFluxo_mwh  = EnergiaPerdaFluxo_mwh  + balanco->EnergiaPerdaFluxo_mwh;
   EnergiaPerdaFora_mwh   = EnergiaPerdaFora_mwh   + balanco->EnergiaPerdaFora_mwh;
   EnergiaPerdaTotal_mwh  = EnergiaPerdaTotal_mwh  + balanco->EnergiaPerdaTotal_mwh;

   return(true);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TBalanco::PM_GetDescricao(void)
   {
   return(PD.descricao);
   }

//---------------------------------------------------------------------------
double __fastcall TBalanco::PM_GetEnergiaCarga_mwh(void)
   {
   return(PD.energia.carga_mwh);
   }

//---------------------------------------------------------------------------
double __fastcall TBalanco::PM_GetEnergiaEntrada_mwh(void)
   {
   return(PD.energia.gerador_mwh      + PD.energia.suprimento_mwh +
          PD.energia.recebida_ext_mwh + PD.energia.recebida_int_mwh);
   }

//---------------------------------------------------------------------------
double __fastcall TBalanco::PM_GetEnergiaEntregueExt_mwh(void)
   {
   return(PD.energia.entregue_ext_mwh);
   }

//---------------------------------------------------------------------------
double __fastcall TBalanco::PM_GetEnergiaEntregueInt_mwh(void)
   {
   return(PD.energia.entregue_int_mwh);
   }

//---------------------------------------------------------------------------
double __fastcall TBalanco::PM_GetEnergiaGerador_mwh(void)
   {
   return(PD.energia.gerador_mwh);
   }

//---------------------------------------------------------------------------
double __fastcall TBalanco::PM_GetEnergiaPerdaFluxo_mwh(void)
   {
   return(PD.energia.perda_fluxo_mwh);
   }

//---------------------------------------------------------------------------
double __fastcall TBalanco::PM_GetEnergiaPerdaFora_mwh(void)
   {
   return(PD.energia.perda_fora_mwh);
   }

//---------------------------------------------------------------------------
double __fastcall TBalanco::PM_GetEnergiaPerdaTotal_mwh(void)
   {
   return(PD.energia.perda_total_mwh);
   }

//---------------------------------------------------------------------------
double __fastcall TBalanco::PM_GetEnergiaRecebidaExt_mwh(void)
   {
   return(PD.energia.recebida_ext_mwh);
   }

//---------------------------------------------------------------------------
double __fastcall TBalanco::PM_GetEnergiaRecebidaInt_mwh(void)
   {
   return(PD.energia.recebida_int_mwh);
   }

//---------------------------------------------------------------------------
double __fastcall TBalanco::PM_GetEnergiaSuprimento_mwh(void)
   {
   return(PD.energia.suprimento_mwh);
   }

//---------------------------------------------------------------------------
TObject* __fastcall TBalanco::PM_GetObject(void)
   {
   return(PD.object);
   }

//---------------------------------------------------------------------------
void __fastcall TBalanco::PM_SetDescricao(AnsiString descricao)
   {
   PD.descricao = descricao;
   }

//---------------------------------------------------------------------------
void __fastcall TBalanco::PM_SetEnergiaCarga_mwh(double energia_mwh)
   {
   PD.energia.carga_mwh = RoundTo(energia_mwh, ROUND_PRECISION);
   }

//---------------------------------------------------------------------------
void __fastcall TBalanco::PM_SetEnergiaEntregueExt_mwh(double energia_mwh)
   {
   PD.energia.entregue_ext_mwh = RoundTo(energia_mwh, ROUND_PRECISION);
   }

//---------------------------------------------------------------------------
void __fastcall TBalanco::PM_SetEnergiaEntregueInt_mwh(double energia_mwh)
   {
   PD.energia.entregue_int_mwh = RoundTo(energia_mwh, ROUND_PRECISION);
   }

//---------------------------------------------------------------------------
void __fastcall TBalanco::PM_SetEnergiaGerador_mwh(double energia_mwh)
   {
   PD.energia.gerador_mwh = RoundTo(energia_mwh, ROUND_PRECISION);
   }

//---------------------------------------------------------------------------
void __fastcall TBalanco::PM_SetEnergiaPerdaFluxo_mwh(double energia_mwh)
   {
   PD.energia.perda_fluxo_mwh = RoundTo(energia_mwh, ROUND_PRECISION);
   }

//---------------------------------------------------------------------------
void __fastcall TBalanco::PM_SetEnergiaPerdaFora_mwh(double energia_mwh)
   {
   PD.energia.perda_fora_mwh = RoundTo(energia_mwh, ROUND_PRECISION);
   }

//---------------------------------------------------------------------------
void __fastcall TBalanco::PM_SetEnergiaPerdaTotal_mwh(double energia_mwh)
   {
   PD.energia.perda_total_mwh = RoundTo(energia_mwh, ROUND_PRECISION);
   }

//---------------------------------------------------------------------------
void __fastcall TBalanco::PM_SetEnergiaRecebidaExt_mwh(double energia_mwh)
   {
   PD.energia.recebida_ext_mwh = RoundTo(energia_mwh, ROUND_PRECISION);
   }

//---------------------------------------------------------------------------
void __fastcall TBalanco::PM_SetEnergiaRecebidaInt_mwh(double energia_mwh)
   {
   PD.energia.recebida_int_mwh = RoundTo(energia_mwh, ROUND_PRECISION);
   }

//---------------------------------------------------------------------------
void __fastcall TBalanco::PM_SetEnergiaSuprimento_mwh(double energia_mwh)
   {
   PD.energia.suprimento_mwh = RoundTo(energia_mwh, ROUND_PRECISION);
   }

//---------------------------------------------------------------------------
void __fastcall TBalanco::PM_SetObject(TObject *object)
   {
   PD.object = object;
   }

//---------------------------------------------------------------------------
//eof



