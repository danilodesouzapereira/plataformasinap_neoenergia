//---------------------------------------------------------------------------
#ifndef TDbar_H
#define TDbar_H

//arquivos inclu�dos---------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TDbar : TObject
   {
   public:
           __fastcall TDbar(void);
           __fastcall ~TDbar(void);
      bool __fastcall Derivacao(void);
      bool __fastcall Excluida(void);
      bool __fastcall MidPoint(void);
      bool __fastcall Referencia(void);

   public:  //dados oriundos do arquivo Anafas
      int        NB;                     // 00 - N�MERO DA BARRA
      int        CHNG;                   // 01 - C�DIGO DE ATUALIZA��O
      int        MP;                     // 02 - TIPO DA BARRA (0=normal,1=fict�cia,2=deriva��o,3=refer�ncia)
      AnsiString BN;                     // 03 - NOME DA BARRA
      double     VPRE;                   // 05 - M�DULO DA TENS�O PR�-FALTA
      double     ANG;                    // 06 - �NGULO DA TENS�O PR�-FALTA
      double     VBASE;                  // 07 - TENS�O DE BASE
      int        IA;                     // 08 - N�MERO DA �REA

   public:  //dados adicionais
      int   id;
      int   coord_x, coord_y;
   };

#endif
//-----------------------------------------------------------------------------
// eof
