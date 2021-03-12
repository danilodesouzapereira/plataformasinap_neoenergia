//---------------------------------------------------------------------------
#ifndef TResumoCargaH
#define TResumoCargaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTResumoCarga.h"

//---------------------------------------------------------------------------
class TResumoCarga : public VTResumoCarga
   {
   public:
             __fastcall  TResumoCarga(void);
             __fastcall ~TResumoCarga(void);
      void   __fastcall  InsereCarga(VTCarga *carga);
      TList* __fastcall  LisCarga(void);

   private:  //dados
      TList *lisCAR;
   };

//---------------------------------------------------------------------------
#endif
//eof

