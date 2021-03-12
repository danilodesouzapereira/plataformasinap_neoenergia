//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "TMsgEliminaFalta.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMsgEliminaFalta* __fastcall NewMsgEliminaFalta(void)
   {
   try{//cria objeto TMsgEliminaFalta
      return(new TMsgEliminaFalta());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall  TMsgEliminaFalta::TMsgEliminaFalta(void)
   {
   //monta mensagem
   pcte.funcao     = funcaoELIMINA_FALTA;
   pcte.num_bytes  = Length();
   //aloca memória
   buffer.length = Length();
   buffer.memory = new char[buffer.length];
   }

//---------------------------------------------------------------------------
__fastcall  TMsgEliminaFalta::~TMsgEliminaFalta(void)
   {
   //libera memória
   if (buffer.memory) {delete[] buffer.memory; buffer.memory = NULL;}
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMsgEliminaFalta::ConverteBufToMsg(strBUF *buf)
   {
   //proteção: verifica se o buf recebido tem o número de bytes esperado
   if (buffer.length != buf->length) return(false);
   //copia dados p/ buffer
   memcpy(buffer.memory, buf->memory, buf->length);
   //copia dados p/ strPCTE_FALTA
   memcpy(&pcte, buf->memory, buf->length);

   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TMsgEliminaFalta::ConverteBufToMsg(strBUF *buf, int index)
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
strBUF* __fastcall TMsgEliminaFalta::ConverteMsgToBuf(void)
   {
   memcpy(buffer.memory, &pcte, Length());
   return(&buffer);
   }
/*
//---------------------------------------------------------------------------
int __fastcall TMsgEliminaFalta::EqptoId(void)
   {
   return(pcte.eqpto_id);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgEliminaFalta::EqptoTipo(void)
   {
   return(pcte.eqpto_tipo);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TMsgEliminaFalta::HoraMinutoSegundo(strHMS &hms)
   {
   hms.hora    = pcte.hora;
   hms.minuto  = pcte.minuto;
   hms.segundo = pcte.segundo;

   return(true);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgEliminaFalta::Length(void)
   {
   return(sizeof(strPCTE_ELIMINA_FALTA));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMsgEliminaFalta::MsgAsString(void)
   {
   //variáveis locais
   AnsiString  txt;

   txt  = "Eimina Falta: ";
   txt += IntToStr(pcte.funcao)    + ";";
   txt += IntToStr(pcte.num_bytes) + ";";
   txt += IntToStr(pcte.hora)      + ";";
   txt += IntToStr(pcte.minuto)    + ";";
   txt += IntToStr(pcte.segundo)   + ";";

   return(txt);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgEliminaFalta::PM_GetEqptoId(void)
   {
   //não se aplica
   return(0);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgEliminaFalta::PM_GetEqptoTipo(void)
   {
   //não se aplica
   return(0);
   }
//---------------------------------------------------------------------------
char __fastcall TMsgEliminaFalta::PM_GetFuncao(void)
   {
   return(pcte.funcao);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgEliminaFalta::PM_GetNumBytes(void)
   {
   return(pcte.num_bytes);
   }

//---------------------------------------------------------------------------
void __fastcall TMsgEliminaFalta::PM_SetEqptoId(int eqpto_id)
   {
   //não se aplica
   }

//---------------------------------------------------------------------------
void __fastcall TMsgEliminaFalta::PM_SetEqptoTipo(char eqpto_tipo)
   {
   //não se aplica
   }

//---------------------------------------------------------------------------
//eof
