//---------------------------------------------------------------------------
#include <vcl.h>
#include <string.h>
#pragma hdrstop
#include "TCript.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCript* __fastcall NewObjCript(void)
   {
   return(new TCript());
   }
   
//---------------------------------------------------------------------------
__fastcall TCript::TCript(void)
   {
   //pré-define chave de criptografia
   DefineChave("Copyright Guaraldo 2006");
   }

//---------------------------------------------------------------------------
__fastcall TCript::~TCript(void)
   {
   //libera memória alocada
   if (key) {delete[] key; key = NULL;}
   }

//-----------------------------------------------------------------------------
void __fastcall TCript::DefineChave(AnsiString txt)
   {
   //salva chave de criptografia
   chave = txt;
   //aloca memória p/ criptografia
   if (key) delete[] key;
   sizekey = chave.Length();
   key     = new char[sizekey+10]; //+10 por segurança
   }

//---------------------------------------------------------------------------
//criptografa/descriptografa um vetor de caracteres
//---------------------------------------------------------------------------
bool __fastcall TCript::Executa(char *buf, int sizebuf)
   {
   //variáveis locais
   int n, i;

   //proteção
   if ((chave.IsEmpty())||(buf == NULL)||(sizebuf == 0)) return(false);
   //faz uma cópia da chave que será alterada durante a criptografia
   memcpy (key, chave.c_str(), sizekey);
   //loop de criptografia
   for (n = 0, i = 0 ; n < sizebuf; n++, buf++)
      {
      *buf ^= (char)(key[i] ^ (key[0] * i));
      key[i] += ((i < (sizekey - 1)) ? key[i+1] : key[0]);
      if (!key[i])    key[i] = (char)(key[i]+1);
      if ((++i) >= sizekey) i = 0;
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

