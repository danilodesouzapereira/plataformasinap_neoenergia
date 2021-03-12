//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAplMedidor.h"
#include "..\Busca\VTBusca.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Legenda\VTLegenda.h"
#include "..\..\DLL_Inc\Busca.h"
#include "..\..\DLL_Inc\Consulta.h"
#include "..\..\DLL_Inc\Legenda.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TAplMedidor::TAplMedidor(TForm *form_owner, VTApl *apl_owner)
                        :TApl(form_owner, apl_owner)
   {
   //variáveis locais
   TObject *obj;

   //cria objetos necessários ao Editor
   if ((obj = DLL_NewObjBusca(this))   != NULL) lisOBJ->Add(obj);
   if ((obj = DLL_NewObjConsulta(this))!= NULL) lisOBJ->Add(obj);
   if ((obj = DLL_NewObjLegenda(this)) != NULL) lisOBJ->Add(obj);
  }

//---------------------------------------------------------------------------
__fastcall TAplMedidor::~TAplMedidor(void)
   {
   //OBS: a classe base destrói a lista e seus objetos
   }

//---------------------------------------------------------------------------
//eof