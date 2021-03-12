//---------------------------------------------------------------------------
#ifndef TFaixaH
#define TFaixaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TFaixa : public TObject
   {
   public:
      __fastcall  TFaixa(void);
      __fastcall ~TFaixa(void);

   public:  //dados
      bool   enabled;
      bool   visible;
      TColor color;
      double dem_min_mva;
      double dem_max_mva;
      double dem_tot_mva;
      double dem_tot_perc;
      double num_cel_perc;
      TList  *lisCELULA;
   };
//---------------------------------------------------------------------------
#endif
//eof
