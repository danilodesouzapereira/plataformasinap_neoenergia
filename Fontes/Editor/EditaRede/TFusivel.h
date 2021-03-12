//---------------------------------------------------------------------------

#ifndef TFusivelH
#define TFusivelH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TFusivel : public TObject
   {
   public:  //property
      __property int        TipoTrafo = {read=PD.tipo_trafo, write=PD.tipo_trafo};
      __property double     Snom_kva  = {read=PD.snom_kva, write=PD.snom_kva};
      __property double     Iadm_a    = {read=PD.iadm_a, write=PD.iadm_a};
      __property AnsiString Elo       = {read=PD.elo, write=PD.elo};

   public:
           __fastcall  TFusivel(void);
           __fastcall ~TFusivel(void);

   protected: //dados acessados via property
      struct {
             int        tipo_trafo; //0= indefinido, 1=monofasico, 3=trifasico
             double     snom_kva;   //potência do trafo
             double     iadm_a;     //corrente admissivel do fusivel
             AnsiString elo;        //tipo de elo fusivel
             } PD;
   };

//---------------------------------------------------------------------------
#endif
