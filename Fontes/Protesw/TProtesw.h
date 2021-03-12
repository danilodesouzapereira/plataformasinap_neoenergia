//---------------------------------------------------------------------------
#ifndef TProteswH
#define TProteswH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include "VTProtesw.h"

//---------------------------------------------------------------------------
class VTBcd;
class VTCript;

//---------------------------------------------------------------------------
//classe para proteção de arquivos
class TProtesw : public VTProtesw
    {
    public://funções públicas
        __fastcall TProtesw(void);
        __fastcall ~TProtesw(void);
        bool __fastcall Gera(AnsiString dirBin, AnsiString arq_texto);
        bool __fastcall Valida(AnsiString dirBin);

    private://métodos
      bool __fastcall AbreArquivo(void);
      bool __fastcall CalculaCRC(AnsiString arq, int &crc);
      bool __fastcall LeNomeArqCRC(AnsiString txt, AnsiString &arq, int &crc);
      void __fastcall FechaArquivo(void);
      bool __fastcall GravaArqProtesw(void);
      bool __fastcall GravaNomeArqCRC(AnsiString arq, int crc);
      bool __fastcall ValidaProtesw(void);

    private://dados locais
      AnsiString dirBin;
      AnsiString arq_ori, arq_des;
      char       buf[512];
      VTBcd      *bcd;
      VTCript    *cript;
      FILE       *fpr, *fpw;
    };

//---------------------------------------------------------------------------
#endif

