//---------------------------------------------------------------------------
#ifndef TBarH
#define TBarH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
enum enumTIPO_BAR {barCARGA=0, barSWING};

//---------------------------------------------------------------------------
class TBar : public TObject
   {
   public:
          __fastcall  TBar(void       *eqpto,
                           int        id,
                           AnsiString codig);
          __fastcall ~TBar(void);
     void __fastcall  InsereCarga(double p);    //p = pot.ativa em pu
     void __fastcall  InsereGeracao(double p);  //p = pot.ativa em pu

   public:  //dados locais
      void       *eqpto;    //ponteiro p/ objeto externo
      AnsiString codigo;
      int        id;
      int        tipo;      //ver enumTIPO_BAR acima
      int        index;     //índice de Bar p/ matriz Y
      double     p_car;     //potência ativa de carga em pu
      double     p_ger;     //potência ativa de geração em pu
      double     vnom_kv;   //tensão nominal
      struct {
             double v_pu;      //módulo da tensão em pu
             double angulo_rad;
             }res;
   };

//---------------------------------------------------------------------------
#endif
//eof
