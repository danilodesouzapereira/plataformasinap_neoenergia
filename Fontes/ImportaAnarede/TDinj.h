//---------------------------------------------------------------------------
#ifndef TDinj_H
#define TDinj_H

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TDbar;

//---------------------------------------------------------------------------
class TDinj : TObject
   {
   public:
           __fastcall TDinj(void);
           __fastcall ~TDinj(void);
      bool __fastcall Excluido(void);
      bool __fastcall ExisteCapacitor(void);
      bool __fastcall ExisteReator(void);

   public:  //dados oriundos do Anarede
      int        numero_barra;            // 00
      AnsiString operacao;                // 01
      double     injecao_mw;              // 02
      double     injecao_mvar;            // 03
      double     shunt_mvar;              // 04
      double     fator_part_pu;           // 05

   public:  //dados adicionais
      int   id;
    };

#endif
//-----------------------------------------------------------------------------
// eof
