//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TBcd.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
VTBcd* __fastcall NewObjBcd(void)
   {
   return(new TBcd());
   }

//---------------------------------------------------------------------------
__fastcall TBcd::TBcd(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TBcd::~TBcd(void)
   {
   //nada a fazer
   }


//---------------------------------------------------------------------------
bool __fastcall TBcd::BinToHexa(char *orig, int len_orig,
                                     char *dest, int len_dest)
   {
   //variáveis locais
   char msb, lsb;

   //verifica se o buffer de destino tem o dobro do tamanho do buffer de origem
   if ((len_orig << 1) >= len_dest) return(false);
   //converte cada byte do buffer de origem em dois bytes no buffer de destino
   for (int n = 0; n < len_orig; n++)
      {//quebra o byte de origem em dois bytes: msb e lsb
      msb = ((*orig) >> 4) & 0x0f;
      lsb = (*orig) & 0x0f;
      //converte msb e lsb de binário p/ hexa
      msb   = (msb < 10) ? (msb + '0') : msb-10 + 'A';
      lsb   = (lsb < 10) ? (lsb + '0') : lsb-10 + 'A';
      //armazena os dois bytes no buffer destino
      dest[0] = msb;
      dest[1] = lsb;
      //acerta ponteiros
      orig += 1;
      dest += 2;
      }
   //inclui delimitador de string
   *dest = '\0';
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBcd::HexaToBin(char *orig, int len_orig,
                                     char *dest, int len_dest)
   {
   //variáveis locais
   char msb, lsb;

   //verifica se o buffer de destino tem a metade do tamanho do buffer de origem
   if ((len_orig >> 1) >= len_dest) return(false);
   //converte 2 caracteres do buffer de origem em um byte no buffer de destino
   for (int n = 0; n < len_orig; n+=2)
      {//determina dois bytes do buffer de origem
      msb = orig[0];
      lsb = orig[1];
      //converte msb e lsb de hexa p/ binário
      msb = (msb <= '9') ? (msb - '0') : msb - 'A' + 10;
      lsb = (lsb <= '9') ? (lsb - '0') : lsb - 'A' + 10;
      //armazena um byte no buffer destino
      *dest = ((msb << 4) & 0xf0) | (lsb & 0x0f);
      //acerta ponteiros
      orig += 2;
      dest += 1;
      }
   //inclui delimitador de string
   *dest = '\0';
   return(true);
   }
//-----------------------------------------------------------------------------
//eof



