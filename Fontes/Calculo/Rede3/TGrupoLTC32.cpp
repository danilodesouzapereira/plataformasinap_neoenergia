//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TGrupoLTC32.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TGrupoLTC32::TGrupoLTC32(TBarra *bar_ref,
                                    int     tipo)
   {
   this->bar_ref = bar_ref;
   this->tipo    = tipo;
   lisLTC        = new TList;
   }

//---------------------------------------------------------------------------
__fastcall TGrupoLTC32::~TGrupoLTC32(void)
   {
   delete lisLTC;
   }

//---------------------------------------------------------------------------





