//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "TMsgSimulaFalta.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMsgSimulaFalta* __fastcall NewMsgSimulaFalta(void)
   {
   try{//cria objeto TMsgSimulaFalta
      return(new TMsgSimulaFalta());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall  TMsgSimulaFalta::TMsgSimulaFalta(void)
   {
   //monta mensagem
   pcte.funcao     = funcaoSIMULA_FALTA;
   pcte.num_bytes  = Length();
   //aloca memória
   buffer.length = Length();
   buffer.memory = new char[buffer.length];
   }

//---------------------------------------------------------------------------
__fastcall  TMsgSimulaFalta::~TMsgSimulaFalta(void)
   {
   //libera memória
   if (buffer.memory) {delete[] buffer.memory; buffer.memory = NULL;}
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMsgSimulaFalta::ConverteBufToMsg(strBUF *buf)
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
bool __fastcall TMsgSimulaFalta::ConverteBufToMsg(strBUF *buf, int index)
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
strBUF* __fastcall TMsgSimulaFalta::ConverteMsgToBuf(void)
   {
   memcpy(buffer.memory, &pcte, Length());
   return(&buffer);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgSimulaFalta::EqptoId(void)
   {
   return(pcte.eqpto_id);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgSimulaFalta::EqptoTipo(void)
   {
   return(pcte.eqpto_tipo);
   }

//---------------------------------------------------------------------------
bool __fastcall TMsgSimulaFalta::HoraMinutoSegundo(strHMS &hms)
   {
   hms.hora    = pcte.hora;
   hms.minuto  = pcte.minuto;
   hms.segundo = pcte.segundo;

   return(true);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgSimulaFalta::Length(void)
   {
   return(sizeof(strPCTE_SIMULA_FALTA));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMsgSimulaFalta::MsgAsString(void)
   {
   //variáveis locais
   AnsiString  txt;

   txt  = "Simulação de Falta: ";
   txt += IntToStr(pcte.funcao)                   + ";";
   txt += IntToStr(pcte.num_bytes)                + ";";
   txt += IntToStr(pcte.hora)                     + ";";
   txt += IntToStr(pcte.minuto)                   + ";";
   txt += IntToStr(pcte.segundo)                  + ";";
   txt += IntToStr(pcte.eqpto_tipo)               + ";";
   txt += IntToStr(pcte.eqpto_id)                 + ";";
   txt += IntToStr(pcte.tipo_falha)               + ";";
   txt += DoubleToStr("%5.4f", pcte.dist_bar1_pu) + ";";
   txt += DoubleToStr("%5.4f", pcte.rdef_ohm)     + ";";
   txt += IntToStr(pcte.pre_falta)                + ";";

   return(txt);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgSimulaFalta::PM_GetEqptoId(void)
   {
   return(pcte.eqpto_id);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgSimulaFalta::PM_GetEqptoTipo(void)
   {
   return(pcte.eqpto_tipo);
   }
//---------------------------------------------------------------------------
char __fastcall TMsgSimulaFalta::PM_GetFuncao(void)
   {
   return(pcte.funcao);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgSimulaFalta::PM_GetNumBytes(void)
   {
   return(pcte.num_bytes);
   }

//---------------------------------------------------------------------------
void __fastcall TMsgSimulaFalta::PM_SetEqptoId(int eqpto_id)
   {
   pcte.eqpto_id = eqpto_id;
   }

//---------------------------------------------------------------------------
void __fastcall TMsgSimulaFalta::PM_SetEqptoTipo(char eqpto_tipo)
   {
   pcte.eqpto_tipo = eqpto_tipo;
   }

//---------------------------------------------------------------------------
//eof
