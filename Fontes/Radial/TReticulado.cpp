//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TReticulado.h"
#include "VTSecundario.h"
#include "VTSubestacao.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTReticulado* __fastcall NewObjReticulado(void)
   {
   try{
      return (new TReticulado());
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TReticulado::TReticulado(void)
   {
   //cria objetos locais
   lisPRI = new TList();
   lisSEC = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TReticulado::~TReticulado(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisSEC)    {delete lisSEC;    lisSEC    = NULL;}
   if (lisPRI)    {delete lisPRI;    lisPRI    = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TReticulado::ExistePrimario(VTPrimario *primario)
   {
   return(lisPRI->IndexOf(primario) >= 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TReticulado::ExisteSecundario(VTSecundario *secundario)
   {
   return(lisSEC->IndexOf(secundario) >= 0);
   }

//---------------------------------------------------------------------------
void   __fastcall TReticulado::InserePrimario(VTPrimario *primario)
   {
   //proteção
   if (primario == NULL) return;
   //insere rede na lista
   if (lisPRI->IndexOf(primario) < 0)
      {//insere rede na lista
      lisPRI->Add(primario);
      }
   }

//---------------------------------------------------------------------------
void   __fastcall TReticulado::InsereSecundario(VTSecundario *secundario)
   {
   //proteção
   if (secundario == NULL) return;
   //insere rede na lista
   if (lisSEC->IndexOf(secundario) < 0)
      {//insere Secundario na lista
      lisSEC->Add(secundario);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TReticulado::LisPrimario(void)
   {
   return(lisPRI);
   }

//---------------------------------------------------------------------------
TList* __fastcall TReticulado::LisSecundario(void)
   {
   return(lisSEC);
   }

//---------------------------------------------------------------------------
VTCluster* __fastcall TReticulado::PM_GetCluster(void)
   {
   return(PD.cluster);
   }

//---------------------------------------------------------------------------
VTSubestacao* __fastcall TReticulado::PM_GetSubestacao(void)
   {
   return(PD.subestacao);
   }

//---------------------------------------------------------------------------
void __fastcall TReticulado::PM_SetCluster(VTCluster *cluster)
   {
   PD.cluster = cluster;
   }
   
//---------------------------------------------------------------------------
void __fastcall TReticulado::PM_SetSubestacao(VTSubestacao* subestacao)
   {
   PD.subestacao = subestacao;
   }

//---------------------------------------------------------------------------
//eof
