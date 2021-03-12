//---------------------------------------------------------------------------
#ifndef TFaixa_SinalH
#define TFaixa_SinalH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TFaixa_Sinal : public TObject
   {
   public:
      __fastcall  TFaixa_Sinal(void);
      __fastcall ~TFaixa_Sinal(void);

   public:  //dados
      bool   enabled;
      bool   visible;
      TColor color;
      double sinal_min_db;
      double sinal_max_db;
      //double dem_tot_mva;
      //double dem_tot_perc;
      double num_cel_perc;
	  AnsiString codigo;
      TList  *lisCELULA;
   };
//---------------------------------------------------------------------------
#endif
//eof
