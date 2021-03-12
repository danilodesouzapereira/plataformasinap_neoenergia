//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TLiga.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTLiga
//---------------------------------------------------------------------------
VTLiga* __fastcall NewObjLiga(void)
   {
   return(new TLiga());
   }

//---------------------------------------------------------------------------
__fastcall TLiga::TLiga(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TLiga::~TLiga(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TLiga::DefineRede(VTRede *rede1, VTRede *rede2)
   {
   this->rede1 = rede1;
   this->rede2 = rede2;
   }


//---------------------------------------------------------------------------
VTRede* __fastcall TLiga::Rede(int index)
   {
   if (index == 0) return(rede1);
   if (index == 1) return(rede2);

   return(rede1);
   }

//---------------------------------------------------------------------------
//eof

