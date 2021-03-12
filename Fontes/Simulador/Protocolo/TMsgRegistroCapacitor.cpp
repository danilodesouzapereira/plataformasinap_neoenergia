//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "TMsgRegistroCapacitor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMsgRegistroCapacitor* __fastcall NewMsgRegistroCapacitor(void)
   {
   try{//cria objeto TMsgRegistroCapacitor
      return(new TMsgRegistroCapacitor());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall  TMsgRegistroCapacitor::TMsgRegistroCapacitor(void)
   {
   //monta mensagem com dados da Capacitor
   pcte.funcao     = funcaoENVIO_REGISTRO;
   pcte.num_bytes  = Length();
   //aloca memória
   buffer.length = Length();
   buffer.memory = new char[buffer.length];
   }

//---------------------------------------------------------------------------
__fastcall  TMsgRegistroCapacitor::~TMsgRegistroCapacitor(void)
   {
   //libera memória
   if (buffer.memory) {delete[] buffer.memory; buffer.memory = NULL;}
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMsgRegistroCapacitor::ConverteBufToMsg(strBUF *buf, int index)
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
bool __fastcall TMsgRegistroCapacitor::ConverteBufToMsg(strBUF *buf, int index)
   {
   //proteção: verifica se o buf recebido tem o número de bytes esperado
   if ((buf->length - index) < buffer.length) return(false);
   //copia dados p/ buffer
   memcpy(buffer.memory, buf->memory + index, buffer.length);
   //copia dados p/ strPCTE_CAPACITOR
   memcpy(&pcte, buffer.memory, buffer.length);

   return(true);
   }

//---------------------------------------------------------------------------
strBUF* __fastcall TMsgRegistroCapacitor::ConverteMsgToBuf(void)
   {
   memcpy(buffer.memory, &pcte, Length());
   return(&buffer);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgRegistroCapacitor::Length(void)
   {
   return(sizeof(strPCTE_CAPACITOR));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMsgRegistroCapacitor::MsgAsString(void)
   {
   //variáveis locais
   AnsiString txt;

   //função
   txt  = "Envio Registro(Capacitor): ";
   txt += IntToStr(pcte.funcao)      + ";";
   txt += IntToStr(pcte.num_bytes)   + ";";
   txt += IntToStr(pcte.hora)        + ";";
   txt += IntToStr(pcte.minuto)      + ";";
   txt += IntToStr(pcte.segundo)     + ";";
   txt += IntToStr(pcte.fluxo_curto) + ";";
   txt += IntToStr(pcte.eqpto_tipo)  + ";";
   txt += IntToStr(pcte.eqpto_id)    + ";";
   //estado
	txt += IntToStr(pcte.estado)  + ";";
	//potência reativa
	txt += DoubleToStr("%4.3f;", pcte.q_mvar);

   return(txt);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgRegistroCapacitor::PM_GetEqptoId(void)
   {
   return(pcte.eqpto_id);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgRegistroCapacitor::PM_GetEqptoTipo(void)
   {
   return(pcte.eqpto_tipo);
   }
//---------------------------------------------------------------------------
char __fastcall TMsgRegistroCapacitor::PM_GetFuncao(void)
   {
   return(pcte.funcao);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgRegistroCapacitor::PM_GetNumBytes(void)
   {
   return(pcte.num_bytes);
   }

//---------------------------------------------------------------------------
void __fastcall TMsgRegistroCapacitor::PM_SetEqptoId(int eqpto_id)
   {
   pcte.eqpto_id = eqpto_id;
   }

//---------------------------------------------------------------------------
void __fastcall TMsgRegistroCapacitor::PM_SetEqptoTipo(char eqpto_tipo)
   {
   pcte.eqpto_tipo = eqpto_tipo;
   }

//---------------------------------------------------------------------------
//eof
