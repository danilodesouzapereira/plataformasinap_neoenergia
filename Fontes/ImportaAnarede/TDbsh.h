//---------------------------------------------------------------------------
#ifndef TDbshH
#define TDbshH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//class TDbar;

//---------------------------------------------------------------------------
class TDbsh : TObject
   {
   public:
             __fastcall TDbsh(void);
             __fastcall ~TDbsh(void);
      bool   __fastcall ApagaCapDbar(void);
      bool   __fastcall Capacitor(void);
      double __fastcall Valor_mvar(void);

   public:  //dados próprios
      int    id;
      TList *lisINDIV;

   public:  //dados oriundos do Anarede
      int        da_barra;             //00
      AnsiString operacao;             //01
      int        para_barra;           //02
      int        circuito;             //03
      AnsiString controle;             //04
      double     v_min;                //05
      double     v_max;                //06
      int        barra_controlada;     //07
      double     inj_ini;              //08
      AnsiString tipo_controle;        //09
      AnsiString apaga_dbar;           //10
      int        extremidade;          //11

   };
      //dados individualizados (também do Anarede)
      struct strINDIV : TObject
         {
         public:
            __fastcall strINDIV(void);
            __fastcall ~strINDIV(void);
         public:
            int        grupo;                //00
            AnsiString operacao;             //01
            AnsiString estado;               //02
            int        unidades;             //03
            int        uni_op;               //04
            double     q_mvar;               //05
         };
#endif
//-----------------------------------------------------------------------------
// eof
