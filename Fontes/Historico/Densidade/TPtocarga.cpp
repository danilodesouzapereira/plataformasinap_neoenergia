//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPtocarga.h"
#include <Fontes\Rede\VTCarga.h>
#include <DLL_Inc\Rede.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TPtocarga::TPtocarga(VTRede *rede, VTBarra *barra)
   {
   //salva ponteiro p/ rede e barra
   this->rede  = rede;
   this->barra = barra;
   //inicia dados
   num_cons_res = 0; cons_res = NULL;
   num_cons_com = 0; cons_com = NULL;
   num_cons_ind = 0; cons_ind = NULL;
   num_cons_rur = 0; cons_rur = NULL;
   num_cons_out = 0; cons_out = NULL;
   num_cons_a4  = 0; cons_a4  = NULL;
   num_cons_ip  = 0; cons_ip  = NULL;
   }

//---------------------------------------------------------------------------
__fastcall TPtocarga::~TPtocarga(void)
   {
   //destrói objetos
   if (cons_res)  {DLL_DeleteEqpto(cons_res); cons_res  = NULL;}
   if (cons_com)  {DLL_DeleteEqpto(cons_com); cons_com  = NULL;}
   if (cons_ind)  {DLL_DeleteEqpto(cons_ind); cons_ind  = NULL;}
   if (cons_rur)  {DLL_DeleteEqpto(cons_rur); cons_rur  = NULL;}
   if (cons_out)  {DLL_DeleteEqpto(cons_out); cons_out  = NULL;}
   if (cons_a4)   {DLL_DeleteEqpto(cons_a4);  cons_a4   = NULL;}
   if (cons_ip)   {DLL_DeleteEqpto(cons_ip);  cons_ip   = NULL;}
   }

//---------------------------------------------------------------------------
//eof

