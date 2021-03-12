//---------------------------------------------------------------------------
#ifndef TDcir_H
#define TDcir_H

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TDbar;

//---------------------------------------------------------------------------
class TDcir : TObject
   {
   public:
           __fastcall TDcir(void);
           __fastcall ~TDcir(void);
      bool __fastcall Excluido(void);

   public:  //dados oriundos do Anarede
      int        BF;        // 00 - NÚMERO DA BARRA 'DE'
      int        CHNG;      // 01 - CÓDIGO DE ATUALIZAÇÃO
      int        BT;        // 02 - NÚMERO DA BARRA 'PARA'
      int        NC;        // 03 - NÚMERO DO CIRCUITO
      AnsiString TIPC;      // 04 - TIPO DA LIGAÇÃO
      double     R1;        // 05 - RESISTÊNCIA DE SEQ +
      double     X1;        // 06 - REATÂNCIA DE SEQ +
      double     R0;        // 07 - RESISTÊNCIA DE SEQ ZERO
      double     X0;        // 08 - REATÂNCIA DE SEQ ZERO
      AnsiString CN;        // 09 - NOME DO CIRCUITO
      double     S1;        // 10 - SUSCEPTÂNCIA NA SEQ +
      double     S0;        // 11 - SUSCEPTÂNCIA NA SEQ ZERO
      double     TAP;       // 12 - RELAÇÃO DE TRANSFORMAÇÃO (TAP)
      int        TB;        // 13 - NÚMERO DA BARRA TERMINAL DO RAMO SÉRIE ASSOCIADO
      int        TC;        // 14 - NÚMERO DO CIRCUITO DO RAMO SÉRIE ASSOCIADO
      int        IA;        // 15 - NÚMERO DA ÁREA
      int        DEF;       // 16 - DEFASAGEM DE TRAFO DELTA-ESTRELA
      AnsiString IE;        // 17 - INDICADOR DE DEFASAMENTO EXPLÍCITO
      
   public:  //dados adicionais
      int   id;
      TDbar *dbar1, *dbar2;
   };

#endif
//-----------------------------------------------------------------------------
// eof
