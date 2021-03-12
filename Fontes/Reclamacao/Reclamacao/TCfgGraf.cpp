//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCfgGraf.h"
#include <algorithm>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Complexo\Complexo.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\Planejamento\VTAlternativa.h>
#include <Fontes\Planejamento\VTEstudo.h>
#include <Fontes\Planejamento\VTPlanejamento.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTLigacao.h>
#include <Fontes\Rede\VTResflowBar.h>
#include <Fontes\Rede\VTResflowLig.h>
#include <Fontes\Zona\VTZona.h>
#include "VTAjusta.h"
#include "VTPerfilAlt.h"
#include "VTPerfilBarra.h"
#include "VTPerfilRede.h"
#include <Fontes\Grafico\VTCfgGraf.h>
//---------------------------------------------------------------------------
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global para criar objeto da classe
//---------------------------------------------------------------------------
VTCfgGraf *NewObjCfgGrafReg(VTApl *apl)
   {
   return (new TCfgGraf(apl));
   }
//---------------------------------------------------------------------------
__fastcall TCfgGraf::TCfgGraf(VTApl *apl)
   {
   //salva ponteiro do apl
   this->apl = apl;

   //lê valores do infoset
   if(! LeDados())
      { //inicia valores default
      PD.drc       = 0.5;//%
      PD.drp       = 3.0;
      PD.snominal = 30; //kVA
      PD.smedio   = 20;
      }
   //inicia com visualização de tensão
   PD.view = viewTENSAO;
   }

//---------------------------------------------------------------------------
__fastcall TCfgGraf::~TCfgGraf(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
TColor __fastcall TCfgGraf::CorBarra(VTBarra *barra)
   {
   //proteção    //DVK 2015.04.01
   if(! barra->zona) return clGray;
   switch(PD.view)
      {
      //DVK 2015.03.26 fases da zona
      case viewTENSAO: return CorTensao(barra, barra->zona->Fases);
      case viewCARREG: return CorCarregamento(barra);
      default: break;
      }
   return clGray;  //default
   }

//---------------------------------------------------------------------------
TColor __fastcall TCfgGraf::CorCarga(VTCarga *carga)
   {
   switch(PD.view)
      {
      case viewTENSAO: return CorTensao(carga);
      case viewCARREG: return CorCarregamento(carga->pbarra);
      default: break;
      }
   return clGray;  //default
   }

//---------------------------------------------------------------------------
TColor __fastcall TCfgGraf::CorLigacao(VTLigacao *ligacao)
   {
   switch(PD.view)
      {
      case viewTENSAO: return CorTensao(ligacao);
      case viewCARREG: return CorCarregamento(ligacao);
      default: break;
      }
   return clGray;  //default
   }

//---------------------------------------------------------------------------
TColor __fastcall TCfgGraf::CorCarregamento(VTBarra *barra)
   {
   //não há informação de potencia na barra
   return clGray;
   }

//---------------------------------------------------------------------------
TColor __fastcall TCfgGraf::CorCarregamento(VTLigacao *ligacao)
   {
   //var local
   double smax = 0, s;

   //calcula o maior carregamento
   for(int npat = 0; npat < ligacao->resflow->Max_pat; npat++)
      {
      for(int nb = 0; nb < ligacao->resflow->Max_bar; nb++)
         {
         s = 1000. * Abs(ligacao->resflow->S_mva[nb][npat]);
         smax = max(smax, s);
         }
      }
   //define a cor
   if(smax > PD.snominal)
      return clRed;

   if(smax > PD.smedio)
      return clYellow;

   return clGreen;  //default
   }

//---------------------------------------------------------------------------
TColor __fastcall TCfgGraf::CorTensao(VTBarra *barra, int fases)
   {
   //variáveis locais
   double         drc_barra, drp_barra;
   VTPerfilAlt    *perfil_alt;
   VTPerfilBarra *perfil_barra;
//   VTAjusta      *ajusta = (VTAjusta*)apl->GetObject(__classid(VTAjusta));
   VTEstudo       *estudo;
   VTPlanejamento *planejamento    = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));

   //proteção
   estudo = planejamento->ExisteEstudo(-1);
   if(! estudo) return(clGreen);
   //carrega perfil_barra
   perfil_alt = (VTPerfilAlt*)estudo->AlternativaAtiva->Object;
   perfil_barra = perfil_alt->ExistePerfilBarra(barra);
//   if      (ajusta->PerfilRedeComObra->Enabled) perfil_barra = ajusta->PerfilRedeComObra->ExistePerfilBarra(barra);
//   else if (ajusta->PerfilRedeSemObra->Enabled) perfil_barra = ajusta->PerfilRedeSemObra->ExistePerfilBarra(barra);
//   else if (ajusta->PerfilRedeInicial->Enabled) perfil_barra = ajusta->PerfilRedeInicial->ExistePerfilBarra(barra);
   //proteção
   if (perfil_barra != NULL)
      {//determina o valor dos índices
      drc_barra = perfil_barra->DRC_perc(fases);
      drp_barra = perfil_barra->DRP_perc(fases);
      //define a cor a ser retornada
      if (drc_barra > PD.drc) return clRed;
      if (drp_barra > PD.drp) return clYellow;
      return clGreen;  //default
      }
   return(clGray);
   }

//---------------------------------------------------------------------------
TColor __fastcall TCfgGraf::CorTensao(VTCarga *carga)
   {
   return(CorTensao(carga->pbarra, carga->Fases));
   }

//---------------------------------------------------------------------------
TColor __fastcall TCfgGraf::CorTensao(VTLigacao *ligacao)
   {
   //proteção    //DVK 2015.04.01
   if(! ligacao->pbarra2->zona) return clGray;
   //retorna os índices da segunda barra da ligação
   //DVK 2015.03.26 fases da zona
   return(CorTensao(ligacao->pbarra2, ligacao->pbarra2->zona->Fases));
   }

//---------------------------------------------------------------------------
bool __fastcall TCfgGraf::LeDados(void)
   {
   //variáveis locais
   VTGeral        *geral        = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //lê informações no infoset
   if(! geral) return(false);
   if(geral->Infoset->LoadFromFile("CfgGraf"))
      {
      geral->Infoset->GetInfo("valDRC", PD.drc);
      geral->Infoset->GetInfo("valDRP", PD.drp);
      geral->Infoset->GetInfo("valSmed",PD.smedio);
      geral->Infoset->GetInfo("valSnom",PD.snominal);
      }
   else
      {//insere valores default
      PD.drc      = 0.5;
      PD.drp      = 3.0;
      PD.smedio   = 20.0;
      PD.snominal = 30.0;
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof
