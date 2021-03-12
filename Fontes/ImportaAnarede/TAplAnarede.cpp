//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAplAnarede.h"
#include "VTAnarede.h"
#include "VTPreMonta.h"
#include "..\Importa\Monta\VTMonta.h"
#include "..\Log\VTLog.h"
#include "..\Diretorio\VTPath.h"
#include "..\..\DLL_Inc\Importa.h"
#include "..\..\DLL_Inc\Log.h"
#include "..\..\DLL_Inc\MontaZ.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TAplAnarede::TAplAnarede(TForm *form_owner, VTApl *apl_owner)
                       :TApl(form_owner, apl_owner)
   {
   //variáveis locais
   AnsiString arq;

   //cria objetos
   lisOBJ->Add(DLL_NewObjLog(this));
   lisOBJ->Add(NewObjAnarede(this));
	lisOBJ->Add(DLL_NewObjMontaRede(this));
   lisOBJ->Add(NewObjPreMonta(this));   //precisa ser criado depois de VTMonta e VTmontaEsq
   //lisOBJ->Add(NewObjPosMonta(this));
   lisOBJ->Add(DLL_NewObjMontaZ(this));
   }

//---------------------------------------------------------------------------
__fastcall TAplAnarede::~TAplAnarede(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
//eof