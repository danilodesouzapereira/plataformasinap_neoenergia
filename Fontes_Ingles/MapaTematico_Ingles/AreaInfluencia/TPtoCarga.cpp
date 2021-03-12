//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPtoCarga.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TPtoCarga::TPtoCarga(VTCarga *carga)
   {
   //salva ponteiro p/ objetos
   PD.carga    = carga;
   PD.incluido = false;
   }

//---------------------------------------------------------------------------
__fastcall TPtoCarga::~TPtoCarga(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
//eof

