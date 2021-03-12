//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAplImportaXML.h"
#include "VTFormato.h"
#include "..\Importa\Monta\VTMonta.h"
#include "..\Log\VTLog.h"
#include "..\Diretorio\VTPath.h"
#include "..\..\DLL_Inc\Importa.h"
#include "..\..\DLL_Inc\Log.h"
#include "..\..\DLL_Inc\MontaZ.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TAplImportaXML::TAplImportaXML(TForm *form_owner, VTApl *apl_owner)
                       :TApl(form_owner, apl_owner)
   {
   //variáveis locais
   AnsiString arq;

   //cria objetos
   lisOBJ->Add(DLL_NewObjLog(this));
   lisOBJ->Add(NewObjFormato(this));
	lisOBJ->Add(DLL_NewObjMontaRede(this));
	lisOBJ->Add(DLL_NewObjMontaZ(this));
   }

//---------------------------------------------------------------------------
__fastcall TAplImportaXML::~TAplImportaXML(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
//eof