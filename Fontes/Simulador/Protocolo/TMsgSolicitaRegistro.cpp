//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMsgSolicitaRegistro.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMsgSolicitaRegistro* __fastcall NewMsgSolicitaRegistro(void)
   {
   try{//cria objeto TMsgSolicitaRegistro
      return(new TMsgSolicitaRegistro());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall  TMsgSolicitaRegistro::TMsgSolicitaRegistro(void)
   {
   //monta mensagem
   pcte.funcao     = funcaoSOLICITA_REGISTRO;
   pcte.num_bytes  = Length();
   //aloca memória
   buffer.length = Length();
   buffer.memory = new char[buffer.length];
   }

//---------------------------------------------------------------------------
__fastcall  TMsgSolicitaRegistro::~TMsgSolicitaRegistro(void)
   {
   //libera memória
   if (buffer.memory) {delete[] buffer.memory; buffer.memory = NULL;}
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMsgSolicitaRegistro::ConverteBufToMsg(strBUF *buf)
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
bool __fastcall TMsgSolicitaRegistro::ConverteBufToMsg(strBUF *buf, int index)
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
strBUF* __fastcall TMsgSolicitaRegistro::ConverteMsgToBuf(void)
   {
   memcpy(buffer.memory, &pcte, Length());
   return(&buffer);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgSolicitaRegistro::EqptoId(void)
   {
   return(pcte.eqpto_id);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgSolicitaRegistro::EqptoTipo(void)
   {
   return(pcte.eqpto_tipo);
   }

//---------------------------------------------------------------------------
bool __fastcall TMsgSolicitaRegistro::HoraMinutoSegundo(strHMS &hms)
   {
   hms.hora    = pcte.hora;
   hms.minuto  = pcte.minuto;
   hms.segundo = pcte.segundo;

   return(true);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgSolicitaRegistro::Length(void)
   {
   return(sizeof(strPCTE_SOLICITA));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMsgSolicitaRegistro::MsgAsString(void)
   {
   //variáveis locais
   AnsiString  txt;

   txt  = "Solicitação Registro: ";
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
int __fastcall TMsgSolicitaRegistro::PM_GetEqptoId(void)
   {
   return(pcte.eqpto_id);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgSolicitaRegistro::PM_GetEqptoTipo(void)
   {
   return(pcte.eqpto_tipo);
   }
//---------------------------------------------------------------------------
char __fastcall TMsgSolicitaRegistro::PM_GetFuncao(void)
   {
   return(pcte.funcao);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgSolicitaRegistro::PM_GetNumBytes(void)
   {
   return(pcte.num_bytes);
   }

//---------------------------------------------------------------------------
void __fastcall TMsgSolicitaRegistro::PM_SetEqptoId(int eqpto_id)
   {
   pcte.eqpto_id = eqpto_id;
   }

//---------------------------------------------------------------------------
void __fastcall TMsgSolicitaRegistro::PM_SetEqptoTipo(char eqpto_tipo)
   {
   pcte.eqpto_tipo = eqpto_tipo;
   }

//---------------------------------------------------------------------------
//eof
