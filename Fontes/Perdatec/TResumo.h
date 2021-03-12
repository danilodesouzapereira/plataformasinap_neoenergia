//---------------------------------------------------------------------------
#ifndef TResumoH
#define TResumoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTResumo.h"

//---------------------------------------------------------------------------
class TResumo : public VTResumo
   {
   public:  //métodos
           __fastcall  TResumo(void);
           __fastcall ~TResumo(void);
      void __fastcall  Agrega(TList *lisRESUMO);
      void __fastcall  Agrega(VTResumo *resumo);
      void __fastcall  CopiaDe(VTResumo *resumo);
      void __fastcall  ZeraValores(void);

   private:  //dados locais

   };

//---------------------------------------------------------------------------
#endif
//eof
