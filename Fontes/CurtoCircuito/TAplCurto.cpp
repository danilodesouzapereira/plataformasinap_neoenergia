//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAplCurto.h"
#include "CurtoNo\VTCurtoNo.h"
#include "CurtoReducao\VTCurtoReducao.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Flow\VTFlow.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Log\VTLog.h"
#include "..\Monta\VTMontaCC.h"
#include "..\Ordena\VTOrdena.h"
#include "..\RedeCC\VTRedeCC.h"
#include "..\ResCC\VTResCC.h"
#include "..\Zona\VTZonas.h"
#include "..\..\DLL_Inc\Consulta.h"
#include "..\..\DLL_Inc\Flow.h"
#include "..\..\DLL_Inc\Log.h"
#include "..\..\DLL_Inc\Ordena.h"
#include "..\..\DLL_Inc\RedeCC.h"
#include "..\..\DLL_Inc\ResCC.h"
#include "..\..\DLL_Inc\Zona.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TAplCurto::TAplCurto(TForm *form_owner, VTApl *apl_owner)
                        :TApl(form_owner, apl_owner)
   {
   //variáveis locais
   VTRedeCC  *redeCC;
   VTGrafico *grafico = (VTGrafico*)apl_owner->GetObject(__classid(VTGrafico));

   //cria objetos necessários ao cálculo de curto-ciruito
   Add(redeCC = DLL_NewObjRedeCC(this));
   Add(NewObjCurtoNo(this));
   Add(NewObjCurtoReducao(this));
   Add(DLL_NewObjConsulta(this));
   Add(DLL_NewObjMontaCC(this));
   Add(DLL_NewObjOrdena(this));
   Add(DLL_NewObjLog(this));
   Add(DLL_NewObjResCC(this));
   Add(DLL_NewObjFlow(this));
   //ATENÇÂO: insere objeto RedeCC no mesmo objeto Apl acessado pelo objeto Grafico
   grafico->Apl->Add(redeCC);
   }

//---------------------------------------------------------------------------
__fastcall TAplCurto::~TAplCurto(void)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)GetObject(__classid(VTGrafico));
   VTRedeCC  *redeCC  = (VTRedeCC*)GetObject(__classid(VTRedeCC));

   //ATENÇÂO: retira objeto RedeCC incluído no objeto Apl acessado pelo objeto Grafico
   if (redeCC) grafico->Apl->Remove(redeCC);
   //OBS: a classe base destrói a lista e seus objetos
   }

//---------------------------------------------------------------------------
//eof