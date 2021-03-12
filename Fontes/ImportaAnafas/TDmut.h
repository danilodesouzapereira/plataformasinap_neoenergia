//---------------------------------------------------------------------------
#ifndef TDmut_H
#define TDmut_H

//arquivos incluídos---------------------------------------------------------
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
      int        BF1;   // 0 - NÚMERO DA BARRA 'DE' DA LINHA 1
      int        CHNG;  // 1 - CÓDIGO DE ATUALIZAÇÃO
      int        BT1;   // 2 - NÚMERO DA BARRA 'PARA' DA LINHA 1
      int        NC1;   // 3 - NÚMERO DO CIRCUITO  DA LINHA 1
      int        BF2;   // 4 - NÚMERO DA BARRA 'DE' DA LINHA 2
      int        BT2;   // 5 - NÚMERO DA BARRA 'PARA' DA LINHA 2
      int        NC2;   // 6 - NÚMERO DO CIRCUITO  DA LINHA 2
      double     RM;    // 7 - RESISTÊNCIA DE SEQ ZERO
      double     XM;    // 8 - REATÂNCIA DE SEQ ZERO
      int        IA;    // 9 - NÚMERO DA ÁREA

   public:  //dados adicionais
      int   id;
      TDcir *dcir1, *dcir2;
   };

#endif
//-----------------------------------------------------------------------------
// eof
