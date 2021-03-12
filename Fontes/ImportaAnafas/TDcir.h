//---------------------------------------------------------------------------
#ifndef TDcir_H
#define TDcir_H

//arquivos inclu�dos---------------------------------------------------------
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
      int        BF;        // 00 - N�MERO DA BARRA 'DE'
      int        CHNG;      // 01 - C�DIGO DE ATUALIZA��O
      int        BT;        // 02 - N�MERO DA BARRA 'PARA'
      int        NC;        // 03 - N�MERO DO CIRCUITO
      AnsiString TIPC;      // 04 - TIPO DA LIGA��O
      double     R1;        // 05 - RESIST�NCIA DE SEQ +
      double     X1;        // 06 - REAT�NCIA DE SEQ +
      double     R0;        // 07 - RESIST�NCIA DE SEQ ZERO
      double     X0;        // 08 - REAT�NCIA DE SEQ ZERO
      AnsiString CN;        // 09 - NOME DO CIRCUITO
      double     S1;        // 10 - SUSCEPT�NCIA NA SEQ +
      double     S0;        // 11 - SUSCEPT�NCIA NA SEQ ZERO
      double     TAP;       // 12 - RELA��O DE TRANSFORMA��O (TAP)
      int        TB;        // 13 - N�MERO DA BARRA TERMINAL DO RAMO S�RIE ASSOCIADO
      int        TC;        // 14 - N�MERO DO CIRCUITO DO RAMO S�RIE ASSOCIADO
      int        IA;        // 15 - N�MERO DA �REA
      int        DEF;       // 16 - DEFASAGEM DE TRAFO DELTA-ESTRELA
      AnsiString IE;        // 17 - INDICADOR DE DEFASAMENTO EXPL�CITO
      
   public:  //dados adicionais
      int   id;
      TDbar *dbar1, *dbar2;
   };

#endif
//-----------------------------------------------------------------------------
// eof
