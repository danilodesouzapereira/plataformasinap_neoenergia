//---------------------------------------------------------------------------
#ifndef TDivideH
#define TDivideH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBarra;
class VTTrecho;

//---------------------------------------------------------------------------
class TDivide
   {
   public:
                __fastcall  TDivide(VTTrecho *tre_ori, VTTrecho *tre_alt, VTBarra *bar_ref, double dist_m);
                __fastcall ~TDivide(void);
      VTBarra*  __fastcall  BarraNova(void);
      VTTrecho* __fastcall  TrechoAlterado(void);
      VTTrecho* __fastcall  TrechoNovo(void);
      VTTrecho* __fastcall  TrechoOriginal(void);

   private: //métodos
      void __fastcall ConfiguraBarraNova(double dist_m);
      void __fastcall ConfiguraTrechoAlterado(double dist_m);
      void __fastcall ConfiguraTrechoNovo(double dist_m);

   private: //dados externos
      VTBarra  *bar_ref;
      VTBarra  *bar_nova;
      VTTrecho *tre_ori;
      VTTrecho *tre_alt;
      VTTrecho *tre_novo;
   };

#endif
//---------------------------------------------------------------------------
//eof

