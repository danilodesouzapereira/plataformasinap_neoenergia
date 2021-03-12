//-----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "TEqp.h"
#include "..\Protocolo\VTMsg.h"

//-----------------------------------------------------------------------------
__fastcall TEqp::TEqp(int tipo_eqpto, AnsiString cod_eqpto)
   {
   //cria lista
   lisMSG = new TList();
   //inicia dados
   enabled = false;
   id      = 0;
   tipo    = tipo_eqpto;
   codigo  = cod_eqpto;
   }

//-----------------------------------------------------------------------------
__fastcall TEqp::~TEqp(void)
   {
   //destró lista e seus objetos
   if (lisMSG) {LimpaTList(lisMSG); delete lisMSG; lisMSG = NULL;}
   }

//-----------------------------------------------------------------------------
void __fastcall TEqp::InsereMsg(VTMsg *msg)
   {
   if (lisMSG->IndexOf(msg) < 0)
      {
      lisMSG->Add(msg);
      }
   }

//---------------------------------------------------------------------------
//eof
