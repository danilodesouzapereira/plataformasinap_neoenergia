//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMsgExisteEqpto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMsgExisteEqpto* __fastcall NewMsgExisteEqpto(void)
   {
   try{//cria objeto TMsgExisteEqpto
      return(new TMsgExisteEqpto());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall  TMsgExisteEqpto::TMsgExisteEqpto(void)
   {
   //monta mensagem
   pcte.funcao     = funcaoEXISTE_EQPTO;
   pcte.num_bytes  = Length();
   //aloca memória
   buffer.length = Length();
   buffer.memory = new char[buffer.length];
   }

//---------------------------------------------------------------------------
__fastcall  TMsgExisteEqpto::~TMsgExisteEqpto(void)
   {
   //libera memória
   if (buffer.memory) {delete[] buffer.memory; buffer.memory = NULL;}
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMsgExisteEqpto::ConverteBufToMsg(strBUF *buf)
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
bool __fastcall TMsgExisteEqpto::ConverteBufToMsg(strBUF *buf, int index)
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
strBUF* __fastcall TMsgExisteEqpto::ConverteMsgToBuf(void)
   {
   memcpy(buffer.memory, &pcte, Length());
   return(&buffer);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgExisteEqpto::EqptoId(void)
   {
   return(pcte.eqpto_id);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgExisteEqpto::EqptoTipo(void)
   {
   return(pcte.eqpto_tipo);
   }

//---------------------------------------------------------------------------
bool __fastcall TMsgExisteEqpto::HoraMinutoSegundo(strHMS &hms)
   {
   hms.hora    = pcte.hora;
   hms.minuto  = pcte.minuto;
   hms.segundo = pcte.segundo;

   return(true);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgExisteEqpto::Length(void)
   {
   return(sizeof(strPCTE_EQPTO));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMsgExisteEqpto::MsgAsString(void)
   {
   //variáveis locais
   AnsiString  txt;

   txt  = "Existe Eqpto: ";
   txt += IntToStr(pcte.funcao)     + ";";
   txt += IntToStr(pcte.num_bytes)  + ";";
   txt += IntToStr(pcte.hora)       + ";";
   txt += IntToStr(pcte.minuto)     + ";";
   txt += IntToStr(pcte.segundo)    + ";";
   txt += IntToStr(pcte.eqpto_tipo) + ";";
   txt += IntToStr(pcte.eqpto_id)   + ";";
   txt += AnsiString(pcte.codigo)   + ";";

   return(txt);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgExisteEqpto::PM_GetEqptoId(void)
   {
   return(pcte.eqpto_id);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgExisteEqpto::PM_GetEqptoTipo(void)
   {
   return(pcte.eqpto_tipo);
   }
//---------------------------------------------------------------------------
char __fastcall TMsgExisteEqpto::PM_GetFuncao(void)
   {
   return(pcte.funcao);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgExisteEqpto::PM_GetNumBytes(void)
   {
   return(pcte.num_bytes);
   }

//---------------------------------------------------------------------------
void __fastcall TMsgExisteEqpto::PM_SetEqptoId(int eqpto_id)
   {
   pcte.eqpto_id = eqpto_id;
   }

//---------------------------------------------------------------------------
void __fastcall TMsgExisteEqpto::PM_SetEqptoTipo(char eqpto_tipo)
   {
   pcte.eqpto_tipo = eqpto_tipo;
   }

//---------------------------------------------------------------------------
//eof
