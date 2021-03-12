//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAplFasometro.h"
#include "VTFasores.h"
#include "..\Busca\VTBusca.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Legenda\VTLegenda.h"
#include "..\..\DLL_Inc\Busca.h"
#include "..\..\DLL_Inc\Consulta.h"
#include "..\..\DLL_Inc\Legenda.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TAplFasometro::TAplFasometro(TForm *form_owner, VTApl *apl_owner)
                         :TApl(form_owner, apl_owner)
   {
   //variáveis locais
   TObject *obj;

   //cria objetos necessários ao Editor
   if ((obj = NewObjFasores())         != NULL) lisOBJ->Add(obj);
   if ((obj = DLL_NewObjBusca(this))   != NULL) lisOBJ->Add(obj);
   if ((obj = DLL_NewObjConsulta(this))!= NULL) lisOBJ->Add(obj);
   if ((obj = DLL_NewObjLegenda(this)) != NULL) lisOBJ->Add(obj);
  }

//---------------------------------------------------------------------------
__fastcall TAplFasometro::~TAplFasometro(void)
   {
   //OBS: a classe base destrói a lista e seus objetos
   }

//---------------------------------------------------------------------------
//eof