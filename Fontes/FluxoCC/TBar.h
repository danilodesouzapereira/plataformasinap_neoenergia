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
      int        index;     //�ndice de Bar p/ matriz Y
      double     p_car;     //pot�ncia ativa de carga em pu
      double     p_ger;     //pot�ncia ativa de gera��o em pu
      double     vnom_kv;   //tens�o nominal
      struct {
             double v_pu;      //m�dulo da tens�o em pu
             double angulo_rad;
             }res;
   };

//---------------------------------------------------------------------------
#endif
//eof
