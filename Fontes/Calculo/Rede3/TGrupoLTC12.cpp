//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TGrupoLTC12.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TGrupoLTC12::TGrupoLTC12(TBarra *bar_ref,
                                    int    fase_ref1,
                                    int    fase_ref2,
                                    int    fase_ref3,
                                    int    tipo)
   {
   this->bar_ref   = bar_ref;
   this->fase_ref1 = fase_ref1;
   this->fase_ref2 = fase_ref2;
   this->fase_ref3 = fase_ref3;
   this->tipo      = tipo;
   lisLTC          = new TList;
   }

//---------------------------------------------------------------------------
__fastcall TGrupoLTC12::~TGrupoLTC12(void)
   {
   delete lisLTC;
   }

//---------------------------------------------------------------------------





