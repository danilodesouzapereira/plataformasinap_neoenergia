//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMsgManobraChave.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMsgManobraChave* __fastcall NewMsgManobraChave(void)
   {
   try{//cria objeto TMsgManobraChave
      return(new TMsgManobraChave());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall  TMsgManobraChave::TMsgManobraChave(void)
   {
   //monta mensagem
   pcte.funcao     = funcaoMANOBRA_CHAVE;
   pcte.num_bytes  = Length();
   //aloca memória
   buffer.length = Length();
   buffer.memory = new char[buffer.length];
   }

//---------------------------------------------------------------------------
__fastcall  TMsgManobraChave::~TMsgManobraChave(void)
   {
   //libera memória
   if (buffer.memory) {delete[] buffer.memory; buffer.memory = NULL;}
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMsgManobraChave::ConverteBufToMsg(strBUF *buf)
   {
   //proteção: verifica se o buf recebido tem o número de bytes esperado
   if (buffer.length != buf->length) return(false);
   //copia dados p/ buffer
   memcpy(buffer.memory, buf->memory, buf->length);
   //copia dados p/ strMSG_BARRA
   memcpy(&pcte, buf->memory, buf->length);

   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TMsgManobraChave::ConverteBufToMsg(strBUF *buf, int index)
   {
   //proteção: verifica se o buf recebido tem o número de bytes esperado
   if ((buf->length - index) < buffer.length) return(false);
   //copia dados p/ buffer
   memcpy(buffer.memory, buf->memory + index, buffer.length);
   //copia dados p/ strPCTE_ACK
   memcpy(&pcte, buffer.memory, buffer.length);

   return(true);
   }

//---------------------------------------------------------------------------
strBUF* __fastcall TMsgManobraChave::ConverteMsgToBuf(void)
   {
   memcpy(buffer.memory, &pcte, Length());
   return(&buffer);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgManobraChave::EqptoId(void)
   {
   return(pcte.eqpto_id);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgManobraChave::EqptoTipo(void)
   {
   return(pcte.eqpto_tipo);
   }

//---------------------------------------------------------------------------
bool __fastcall TMsgManobraChave::HoraMinutoSegundo(strHMS &hms)
   {
   hms.hora    = pcte.hora;
   hms.minuto  = pcte.minuto;
   hms.segundo = pcte.segundo;

   return(true);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgManobraChave::Length(void)
   {
   return(sizeof(strPCTE_MANOBRA));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMsgManobraChave::MsgAsString(void)
   {
   //variáveis locais
   AnsiString  txt;

   txt  = "Manobra Chave: ";
   txt += IntToStr(pcte.funcao)     + ";";
   txt += IntToStr(pcte.num_bytes)  + ";";
   txt += IntToStr(pcte.hora)       + ";";
   txt += IntToStr(pcte.minuto)     + ";";
   txt += IntToStr(pcte.segundo)    + ";";
   txt += IntToStr(pcte.eqpto_tipo) + ";";
   txt += IntToStr(pcte.eqpto_id)   + ";";

   return(txt);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgManobraChave::PM_GetEqptoId(void)
   {
   return(pcte.eqpto_id);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgManobraChave::PM_GetEqptoTipo(void)
   {
   return(pcte.eqpto_tipo);
   }
//---------------------------------------------------------------------------
char __fastcall TMsgManobraChave::PM_GetFuncao(void)
   {
   return(pcte.funcao);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgManobraChave::PM_GetNumBytes(void)
   {
   return(pcte.num_bytes);
   }

//---------------------------------------------------------------------------
void __fastcall TMsgManobraChave::PM_SetEqptoId(int eqpto_id)
   {
   pcte.eqpto_id = eqpto_id;
   }

//---------------------------------------------------------------------------
void __fastcall TMsgManobraChave::PM_SetEqptoTipo(char eqpto_tipo)
   {
   pcte.eqpto_tipo = eqpto_tipo;
   }

//---------------------------------------------------------------------------
//eof
