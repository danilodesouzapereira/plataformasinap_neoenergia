//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "TMsgRegistroBarra.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMsgRegistroBarra* __fastcall NewMsgRegistroBarra(void)
   {
   try{//cria objeto TMsgRegistroBarra
      return(new TMsgRegistroBarra());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall  TMsgRegistroBarra::TMsgRegistroBarra(void)
   {
   //monta mensagem com dados da Barra
   pcte.funcao     = funcaoENVIO_REGISTRO;
   pcte.num_bytes  = Length();
   //aloca memória
   buffer.length = Length();
   buffer.memory = new char[buffer.length];
   }

//---------------------------------------------------------------------------
__fastcall  TMsgRegistroBarra::~TMsgRegistroBarra(void)
   {
   //libera memória
   if (buffer.memory) {delete[] buffer.memory; buffer.memory = NULL;}
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMsgRegistroBarra::ConverteBufToMsg(strBUF *buf, int index)
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
bool __fastcall TMsgRegistroBarra::ConverteBufToMsg(strBUF *buf, int index)
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
strBUF* __fastcall TMsgRegistroBarra::ConverteMsgToBuf(void)
   {
   memcpy(buffer.memory, &pcte, Length());
   return(&buffer);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgRegistroBarra::Length(void)
   {
   return(sizeof(strPCTE_BARRA));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMsgRegistroBarra::MsgAsString(void)
   {
   //variáveis locais
   AnsiString txt;

   //função
   txt  = "Envio Registro(Barra): ";
   txt += IntToStr(pcte.funcao)      + ";";
   txt += IntToStr(pcte.num_bytes)   + ";";
   txt += IntToStr(pcte.hora)        + ";";
   txt += IntToStr(pcte.minuto)      + ";";
   txt += IntToStr(pcte.segundo)     + ";";
   txt += IntToStr(pcte.fluxo_curto) + ";";
   txt += IntToStr(pcte.eqpto_tipo)  + ";";
   txt += IntToStr(pcte.eqpto_id)    + ";";
   //módulo e ângulo das fases e do neutro
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      //tensão fase A
      txt += DoubleToStr("%5.4f;", pcte.vf[ind_fase].mod_volt);
      txt += DoubleToStr("%3.2f;", pcte.vf[ind_fase].ang_grau);
      }

   return(txt);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgRegistroBarra::PM_GetEqptoId(void)
   {
   return(pcte.eqpto_id);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgRegistroBarra::PM_GetEqptoTipo(void)
   {
   return(pcte.eqpto_tipo);
   }
//---------------------------------------------------------------------------
char __fastcall TMsgRegistroBarra::PM_GetFuncao(void)
   {
   return(pcte.funcao);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgRegistroBarra::PM_GetNumBytes(void)
   {
   return(pcte.num_bytes);
   }

//---------------------------------------------------------------------------
void __fastcall TMsgRegistroBarra::PM_SetEqptoId(int eqpto_id)
   {
   pcte.eqpto_id = eqpto_id;
   }

//---------------------------------------------------------------------------
void __fastcall TMsgRegistroBarra::PM_SetEqptoTipo(char eqpto_tipo)
   {
   pcte.eqpto_tipo = eqpto_tipo;
   }

//---------------------------------------------------------------------------
//eof
