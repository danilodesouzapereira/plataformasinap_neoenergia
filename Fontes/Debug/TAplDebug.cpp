//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAplDebug.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Bloco\VTBlocos.h"
#include "..\Explorer\VTAtributo.h"
#include "..\..\DLL_Inc\Bloco.h"
#include "..\..\DLL_Inc\Consulta.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TAplDebug::TAplDebug(TForm *form_owner, VTApl *apl_owner)
                        :TApl(form_owner, apl_owner)
   {
   //variáveis locais
   TObject *obj;

   //cria objetos necessários ao Editor
   if ((obj = DLL_NewObjConsulta(this)) != NULL) lisOBJ->Add(obj);
   if ((obj = NewObjAtributo(this))     != NULL) lisOBJ->Add(obj);
   if ((obj = DLL_NewObjBlocos())       != NULL) lisOBJ->Add(obj);
  }

//---------------------------------------------------------------------------
__fastcall TAplDebug::~TAplDebug(void)
   {
   //OBS: a classe base destrói a lista e seus objetos
   }

//---------------------------------------------------------------------------
//eof