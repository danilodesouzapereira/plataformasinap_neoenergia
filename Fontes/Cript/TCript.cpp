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
   //pr�-define chave de criptografia
   DefineChave("Copyright Guaraldo 2006");
   }

//---------------------------------------------------------------------------
__fastcall TCript::~TCript(void)
   {
   //libera mem�ria alocada
   if (key) {delete[] key; key = NULL;}
   }

//-----------------------------------------------------------------------------
void __fastcall TCript::DefineChave(AnsiString txt)
   {
   //salva chave de criptografia
   chave = txt;
   //aloca mem�ria p/ criptografia
   if (key) delete[] key;
   sizekey = chave.Length();
   key     = new char[sizekey+10]; //+10 por seguran�a
   }

//---------------------------------------------------------------------------
//criptografa/descriptografa um vetor de caracteres
//---------------------------------------------------------------------------
bool __fastcall TCript::Executa(char *buf, int sizebuf)
   {
   //vari�veis locais
   int n, i;

   //prote��o
   if ((chave.IsEmpty())||(buf == NULL)||(sizebuf == 0)) return(false);
   //faz uma c�pia da chave que ser� alterada durante a criptografia
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

