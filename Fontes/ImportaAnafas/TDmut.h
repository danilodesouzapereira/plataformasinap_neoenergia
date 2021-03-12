//---------------------------------------------------------------------------
#ifndef TDmut_H
#define TDmut_H

//arquivos inclu�dos---------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TDcir;

//---------------------------------------------------------------------------
class TDmut : TObject
   {
   public:
           __fastcall TDmut(void);
           __fastcall ~TDmut(void);
      bool __fastcall Excluida(void);

   public:  //dados oriundos do Anarede
      int        BF1;   // 0 - N�MERO DA BARRA 'DE' DA LINHA 1
      int        CHNG;  // 1 - C�DIGO DE ATUALIZA��O
      int        BT1;   // 2 - N�MERO DA BARRA 'PARA' DA LINHA 1
      int        NC1;   // 3 - N�MERO DO CIRCUITO  DA LINHA 1
      int        BF2;   // 4 - N�MERO DA BARRA 'DE' DA LINHA 2
      int        BT2;   // 5 - N�MERO DA BARRA 'PARA' DA LINHA 2
      int        NC2;   // 6 - N�MERO DO CIRCUITO  DA LINHA 2
      double     RM;    // 7 - RESIST�NCIA DE SEQ ZERO
      double     XM;    // 8 - REAT�NCIA DE SEQ ZERO
      int        IA;    // 9 - N�MERO DA �REA

   public:  //dados adicionais
      int   id;
      TDcir *dcir1, *dcir2;
   };

#endif
//-----------------------------------------------------------------------------
// eof
