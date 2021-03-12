//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMsgAck.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMsgAck* __fastcall NewMsgAck(void)
   {
   try{//cria objeto TMsgAck
      return(new TMsgAck());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall  TMsgAck::TMsgAck(void)
   {
   //monta mensagem inválida
   pcte.funcao       = funcaoACK;
   pcte.num_bytes    = Length();
   //aloca memória
   buffer.length = Length();
   buffer.memory = new char[buffer.length];
   }

 //---------------------------------------------------------------------------
__fastcall  TMsgAck::~TMsgAck(void)
   {
   //libera memória
   if (buffer.memory) {delete[] buffer.memory; buffer.memory = NULL;}
   }

//---------------------------------------------------------------------------
short __fastcall TMsgAck::Length(void)
   {
   return(sizeof(strPCTE_ACK));
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMsgAck::ConverteBufToMsg(strBUF *buf)
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
bool __fastcall TMsgAck::ConverteBufToMsg(strBUF *buf, int index)
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
strBUF*  __fastcall  TMsgAck::ConverteMsgToBuf(void)
   {
   memcpy(buffer.memory, &pcte, Length());
   return(&buffer);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMsgAck::MsgAsString(void)
   {
   //variáveis locais
   AnsiString  txt;

   txt  = "ACK: ";
   txt += IntToStr(pcte.funcao)        + ";";
   txt += IntToStr(pcte.num_bytes)     + ";";
   txt += IntToStr(pcte.funcao_acked)  + ";";
   txt += IntToStr(pcte.sucesso)       + ";";

   return(txt);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgAck::PM_GetEqptoId(void)
   {
   //não se aplica
   return(0);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgAck::PM_GetEqptoTipo(void)
   {
   //não se aplica
   return(0);
   }
//---------------------------------------------------------------------------
char __fastcall TMsgAck::PM_GetFuncao(void)
   {
   return(pcte.funcao);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgAck::PM_GetNumBytes(void)
   {
   return(pcte.num_bytes);
   }

//---------------------------------------------------------------------------
void __fastcall TMsgAck::PM_SetEqptoId(int eqpto_id)
   {
   //não se aplica
   }

//---------------------------------------------------------------------------
void __fastcall TMsgAck::PM_SetEqptoTipo(char eqpto_tipo)
   {
   //não se aplica
   }

//---------------------------------------------------------------------------
//eof
