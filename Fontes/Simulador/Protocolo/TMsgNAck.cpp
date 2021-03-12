//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMsgNAck.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMsgNAck* __fastcall NewMsgNAck(void)
   {
   try{//cria objeto TMsgNAck
      return(new TMsgNAck());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall  TMsgNAck::TMsgNAck(void)
   {
   //monta mensagem
   pcte.funcao        = funcaoNACK;
   pcte.num_bytes     = Length();
   //aloca mem�ria
   buffer.length = Length();
   buffer.memory = new char[buffer.length];
   }

//---------------------------------------------------------------------------
__fastcall  TMsgNAck::~TMsgNAck(void)
   {
   //libera mem�ria
   if (buffer.memory) {delete[] buffer.memory; buffer.memory = NULL;}
   }

//---------------------------------------------------------------------------
short __fastcall TMsgNAck::Length(void)
   {
   return(sizeof(strPCTE_NACK));
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMsgNAck::ConverteBufToMsg(strBUF *buf)
   {
   //prote��o: verifica se o buf recebido tem o n�mero de bytes esperado
   if (buffer.length != buf->length) return(false);
   //copia dados p/ buffer
   memcpy(buffer.memory, buf->memory, buf->length);
   //copia dados p/ strMSG_BARRA
   memcpy(&pcte, buf->memory, buf->length);

   return(true);
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TMsgNAck::ConverteBufToMsg(strBUF *buf, int index)
   {
   //prote��o: verifica se o buf recebido tem o n�mero de bytes esperado
   if ((buf->length - index) < buffer.length) return(false);
   //copia dados p/ buffer
   memcpy(buffer.memory, buf->memory + index, buffer.length);
   //copia dados p/ strPCTE_NACK
   memcpy(&pcte, buffer.memory, buffer.length);

   return(true);
   }

//---------------------------------------------------------------------------
strBUF* __fastcall TMsgNAck::ConverteMsgToBuf(void)
   {
   memcpy(buffer.memory, &pcte, Length());
   return(&buffer);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMsgNAck::MsgAsString(void)
   {
   //vari�veis locais
   AnsiString  txt;

   txt  = "ACK: ";
   txt += IntToStr(pcte.funcao)        + ";";
   txt += IntToStr(pcte.num_bytes)     + ";";
   txt += IntToStr(pcte.funcao_nacked) + ";";

   return(txt);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgNAck::PM_GetEqptoId(void)
   {
   //n�o se aplica;
   return(0);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgNAck::PM_GetEqptoTipo(void)
   {
   //n�o se aplica;
   return(0);
   }
//---------------------------------------------------------------------------
char __fastcall TMsgNAck::PM_GetFuncao(void)
   {
   return(pcte.funcao);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgNAck::PM_GetNumBytes(void)
   {
   return(pcte.num_bytes);
   }

//---------------------------------------------------------------------------
void __fastcall TMsgNAck::PM_SetEqptoId(int eqpto_id)
   {
   //n�o se aplica;
   }

//---------------------------------------------------------------------------
void __fastcall TMsgNAck::PM_SetEqptoTipo(char eqpto_tipo)
   {
   //n�o se aplica;
   }

//---------------------------------------------------------------------------
//eof
