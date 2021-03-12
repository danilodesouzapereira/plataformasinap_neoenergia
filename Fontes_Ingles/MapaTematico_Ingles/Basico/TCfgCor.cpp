//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCfgCor.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Geral\VTGeral.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTCfgCor* __fastcall NewObjCfgCor(VTApl *apl)
   {
   return(new TCfgCor(apl));
   }
//---------------------------------------------------------------------------
__fastcall TCfgCor::TCfgCor(VTApl *apl)
   {
   //varia´veis locais
   VTGeral *geral;

   //proteção: o objeto Apl é opcional
   if (apl == NULL) return;
   //inicia valores default p/ tensão
   geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
   TENSAO.corInf  = clMaroon;
   TENSAO.corV1   = geral->COR.Faixa[diagRUIM];
   TENSAO.corV2   = geral->COR.Faixa[diagREGULAR];
   TENSAO.corV3   = geral->COR.Faixa[diagBOM];
   TENSAO.corV4   = geral->COR.Faixa[diagREGULAR];
   TENSAO.corV5   = geral->COR.Faixa[diagRUIM];
   TENSAO.corSup  = clMaroon;
   }

//---------------------------------------------------------------------------
__fastcall TCfgCor::~TCfgCor(void)
   {
   }

//---------------------------------------------------------------------------
