//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCriptStringList.h"
#include "..\Cript\VTBcd.h"
#include "..\Cript\VTCript.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TCriptStringList::TCriptStringList(void)
   {
   //inicia dados
   Enabled = false;
   Chave   = "Sinapsis Inova��o em Energia";
   buffer.len = 0;
   buffer.vet = NULL;
   //aloca buffer p/ criptografia
   BufferNew(512);
   //cria objetos
   bcd   = NewObjBcd();
   cript = NewObjCript();
   }

//---------------------------------------------------------------------------
__fastcall TCriptStringList::~TCriptStringList(void)
   {
   //libera mem�ria
   BufferDelete();
   //destr�i objetos
   if (bcd)   {delete bcd;   bcd   = NULL;}
   if (cript) {delete cript; cript = NULL;}
   }

//---------------------------------------------------------------------------
int __fastcall TCriptStringList::Add(AnsiString txt)
   {
   //vari�veis locais
   char *pi;
   int  len;

   //verifica se o string deve sere criptografado
   if (Enabled)
      {//criptografa string
      pi  = txt.c_str();
      len = txt.Length();
      if (! cript->Executa(pi, len)) return(false);
      //prote��o: se necess�rio aloca buffer maior
      BufferNew((len * 2) + 32);
      //converte string criptografado de bin�rio p/ hexa, colocando EOS no final
      if (! bcd->BinToHexa(pi, len, buffer.vet, buffer.len)) return(false);
      }
   //salva string
   return(TStringList::Add(buffer.vet));
   }

//---------------------------------------------------------------------------
void __fastcall TCriptStringList::BufferDelete(void)
   {
   //libera mem�ria
   if (buffer.vet) {delete[] buffer.vet; buffer.vet = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TCriptStringList::BufferNew(int len)
   {
   if (len > buffer.len)
      {//libera mem�ria atualmente alocada
      BufferDelete();
      //aloca mem�ria
      buffer.len = len;
      buffer.vet = new char[buffer.len];
      }
   }

//---------------------------------------------------------------------------
//eof
