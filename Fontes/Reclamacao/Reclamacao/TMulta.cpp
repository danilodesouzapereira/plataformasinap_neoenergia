//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include <DateUtils.hpp>
#include "TMulta.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Classes\VTClasse.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTFases.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRegulador.h>
#include <Fontes\Rede\VTSuprimento.h>
#include "..\Reclamacao\VTPerfilBarra.h"
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTMulta*  __fastcall NewObjMulta(VTApl *apl)
   {
   try{
      return(new TMulta(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TMulta::TMulta(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //limpa variaveis
   Reinicia();
   }

//---------------------------------------------------------------------------
__fastcall TMulta::~TMulta(void)
   {
   //
   }

//---------------------------------------------------------------------------
double __fastcall TMulta::CalculaMulta(double energia_kwh, VTClasse *classe, bool ca)
   {
   //variaveis locais
   int k1, k2;
   double DRCM, DRPM, valor, TUSD, EUSD;

   //Valores máximos do PRODIST
   DRPM = 3.0;
   DRCM = 0.5;
   //DRP = valor do DRP expresso em %, apurado na última medição;
   //DRC = valor do DRC expresso em %, apurado na última medição;

   //define maiores índices DRC e DRP
   MaioresDRCeDRP();
   //determina k1 e k2 conforme PRODIST
   (PD.piorDRP>DRPM) ? k1 = 3 : k1 = 0;
   (PD.piorDRC>DRCM) ? k2 = 7 : k2 = 0;
   //verifica se a multa é para o consumidor analisado (ca)
   if(ca)
      {//verifica se há valor de EUSD para o CA
      EUSD = ValorEUSD();
      if(! IsDoubleZero(EUSD))
         {//multa, conforme fórmula 2.13.2 do PRODIST considerando EUSD
         valor = ( ((PD.piorDRP-DRPM)/100.)*k1 +
                                           ((PD.piorDRC-DRCM)/100.)*k2 ) * EUSD;
         return(valor);
         }
      }
   //determina valor de TUSD pela classe
   TUSD = ValorTUSD(classe->Codigo);
   //multa, conforme fórmula 2.13.2 do PRODIST considerando TUSD * MUSD
   valor = ( ((PD.piorDRP-DRPM)/100.)*k1 +
                              ((PD.piorDRC-DRCM)/100.)*k2 )* TUSD * energia_kwh;

   return(valor);
   }

//---------------------------------------------------------------------------
double __fastcall TMulta::CalculaMultaPE(void)
   {
   //variaveis locais
   double   consumo=0.0, multaPE=0.0;
   TList    *lisEQP = new TList();
   VTCarga  *carga;

   //limpa lisEQP e preenche com todas as cargas do PE
   lisEQP->Clear();
   PD.carga_PE->pbarra->LisEqbar(lisEQP, eqptoCARGA);
   //loop para todas as cargas
   for(int nc = 0; nc < lisEQP->Count; nc++)
      {//define p/os consumidores no mesmo ponto de conexao
      carga = (VTCarga*) lisEQP->Items[nc];
      if(carga == PD.carga_PE) multaPE += CalculaMulta(consumo, carga->classe, true);
      else
         {
         consumo = carga->Energia_kwhmes;
         multaPE += CalculaMulta(consumo, carga->classe);
         }
      }
   //destroi lista
   delete lisEQP;
   return(multaPE);
   }

//---------------------------------------------------------------------------
bool __fastcall TMulta::Executa(void)
   {
   //reinicia variaveis
   Reinicia();
   //verifica se há perfil_barra e carga
   if(PD.perfil_barra_PE && PD.carga_PE)
      {//não calcula multa em carga equivalente
      if(PD.carga_PE->classe->Codigo.AnsiCompare("Equivalente") == 0)
         {
         return(false);
         }
      //calcula as multas
      PD.multa_CA = CalculaMulta(PD.carga_PE->Energia_kwhmes, PD.carga_PE->classe, true);
      if(PD.carga_PE->classe->Codigo.AnsiCompare("A4") != 0)
         {//só calcula multa para o ponto de entrega em redes BT
         PD.multa_PE = CalculaMultaPE();
         }
      return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TMulta::MaioresDRCeDRP(void)
   {
   //variaveis locais
   double    DRCa, DRPa;
   int       fase_tag;
   VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

   for(int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
      {//verifica se a Carga possui a fase selecionada
      fase_tag = fases->IndexToTag[ind_fase];
      if((PD.carga_PE->Fases & fase_tag) != fase_tag) continue;
      //obtem indices DRC, DRP
      DRPa = PD.perfil_barra_PE->DRP_perc(fase_tag);
      DRCa = PD.perfil_barra_PE->DRC_perc(fase_tag);
      //define maiores índices DRC e DRP
      PD.piorDRP = Max(DRPa,PD.piorDRP);
      PD.piorDRC = Max(DRCa,PD.piorDRC);
      }
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TMulta::PM_GetCarga(void)
   {
   return(PD.carga_PE);
   }

//---------------------------------------------------------------------------
VTPerfilBarra* __fastcall TMulta::PM_GetPerfilBarraPE(void)
   {
   return(PD.perfil_barra_PE);
   }

//---------------------------------------------------------------------------
double __fastcall TMulta::PM_GetPiorDRC(void)
   {
   return(PD.piorDRC);
   }

//---------------------------------------------------------------------------
double __fastcall TMulta::PM_GetPiorDRP(void)
   {
   return(PD.piorDRP);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TMulta::PM_GetRedeSec(void)
   {
   return(PD.rede_sec);
   }

//---------------------------------------------------------------------------
VTRegulador* __fastcall TMulta::PM_GetReg(void)
   {
   return(PD.reg);
   }

//---------------------------------------------------------------------------
VTSuprimento* __fastcall TMulta::PM_GetSupr(void)
   {
   return(PD.supr);
   }

//---------------------------------------------------------------------------
double __fastcall TMulta::PM_GetValorMultaCA(void)
   {
   return(PD.multa_CA);
   }

//---------------------------------------------------------------------------
double __fastcall TMulta::PM_GetValorMultaPE(void)
   {
   return(PD.multa_PE);
   }

//---------------------------------------------------------------------------
void __fastcall TMulta::PM_SetCarga(VTCarga* carga)
   {
   PD.carga_PE = carga;
   }

//---------------------------------------------------------------------------
void __fastcall TMulta::PM_SetRedeSec(VTRede* rede_sec)
   {
   PD.rede_sec = rede_sec;
   }

//---------------------------------------------------------------------------
void __fastcall TMulta::PM_SetReg(VTRegulador* reg)
   {
   PD.reg = reg;
   }

//---------------------------------------------------------------------------
void __fastcall TMulta::PM_SetSupr(VTSuprimento* supr)
   {
   PD.supr = supr;
   }

//---------------------------------------------------------------------------
void __fastcall TMulta::PM_SetPerfilBarraPE(VTPerfilBarra*   perfil_barra)
   {
   PD.perfil_barra_PE = perfil_barra;
   }

//---------------------------------------------------------------------------
void __fastcall TMulta::Reinicia(void)
   {
   //zera variáveis, exceto TUSD
   PD.multa_CA = 0.0;
   PD.multa_PE = 0.0;
   PD.piorDRC  = 0.0;
   PD.piorDRP  = 0.0;
   }

//---------------------------------------------------------------------------
double __fastcall TMulta::ValorTUSD(AnsiString cod_classe)
   {
   //variáveis locais
   VTGeral      *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
   AnsiString   valor;
   double       TUSD = 100.0;

   //lê parâmetros do infoset
   if (! geral) return(TUSD);
   if (geral->Infoset->LoadFromFile("ValoresTUSD"))
      {//verifica se a classe é A4, Residencial, IP ou outros
      if(cod_classe.AnsiCompareIC("Comercial")  == 0) cod_classe = "Outros";
      if(cod_classe.AnsiCompareIC("Industrial") == 0) cod_classe = "Outros";
      if(geral->Infoset->GetInfo(cod_classe, valor)) TUSD = StrToDouble(valor);
      }
   return(TUSD);
   }

//---------------------------------------------------------------------------
double __fastcall TMulta::ValorEUSD(void)
   {
   //variáveis locais
   VTGeral      *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
   AnsiString   valor;
   double       EUSD = 100.0;

   //lê parâmetros do infoset
   if (! geral) return(EUSD);
   if (geral->Infoset->LoadFromFile("ValoresTUSD"))
      {
      if(geral->Infoset->GetInfo("EUSD", valor)) EUSD = StrToDouble(valor);
      }
   return(EUSD);
   }

//---------------------------------------------------------------------------
