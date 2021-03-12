//---------------------------------------------------------------------------
#ifndef TDbar_H
#define TDbar_H

//arquivos incluídos---------------------------------------------------------
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
      int        NB;                     // 00 - NÚMERO DA BARRA
      int        CHNG;                   // 01 - CÓDIGO DE ATUALIZAÇÃO
      int        MP;                     // 02 - TIPO DA BARRA (0=normal,1=fictícia,2=derivação,3=referência)
      AnsiString BN;                     // 03 - NOME DA BARRA
      double     VPRE;                   // 05 - MÓDULO DA TENSÃO PRÉ-FALTA
      double     ANG;                    // 06 - ÂNGULO DA TENSÃO PRÉ-FALTA
      double     VBASE;                  // 07 - TENSÃO DE BASE
      int        IA;                     // 08 - NÚMERO DA ÁREA

   public:  //dados adicionais
      int   id;
      int   coord_x, coord_y;
   };

#endif
//-----------------------------------------------------------------------------
// eof
