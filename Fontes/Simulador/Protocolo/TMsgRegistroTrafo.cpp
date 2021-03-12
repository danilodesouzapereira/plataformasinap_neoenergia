//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "TMsgRegistroTrafo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMsgRegistroTrafo* __fastcall NewMsgRegistroTrafo(void)
   {
   try{//cria objeto TMsgRegistroTrafo
      return(new TMsgRegistroTrafo());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall  TMsgRegistroTrafo::TMsgRegistroTrafo(void)
   {
   //monta mensagem com dados da Barra
   pcte.funcao     = funcaoENVIO_REGISTRO;
   pcte.num_bytes  = Length();
   //aloca memória
   buffer.length = Length();
   buffer.memory = new char[buffer.length];
   }

//---------------------------------------------------------------------------
__fastcall  TMsgRegistroTrafo::~TMsgRegistroTrafo(void)
   {
   //libera memória
   if (buffer.memory) {delete[] buffer.memory; buffer.memory = NULL;}
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMsgRegistroTrafo::ConverteBufToMsg(strBUF *buf)
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
bool __fastcall TMsgRegistroTrafo::ConverteBufToMsg(strBUF *buf, int index)
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
strBUF* __fastcall TMsgRegistroTrafo::ConverteMsgToBuf(void)
   {
   memcpy(buffer.memory, &pcte, Length());
   return(&buffer);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgRegistroTrafo::Length(void)
   {
	return(sizeof(strPCTE_TRAFO));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMsgRegistroTrafo::MsgAsString(void)
   {
   //variáveis locais
   AnsiString txt;

   //função
   txt  = "Envio Registro(Trafo): ";
   txt += IntToStr(pcte.funcao)      + ";";
   txt += IntToStr(pcte.num_bytes)   + ";";
   txt += IntToStr(pcte.hora)        + ";";
   txt += IntToStr(pcte.minuto)      + ";";
   txt += IntToStr(pcte.segundo)     + ";";
   txt += IntToStr(pcte.fluxo_curto) + ";";
   txt += IntToStr(pcte.eqpto_tipo)  + ";";
   txt += IntToStr(pcte.eqpto_id)    + ";";
   //módulo e ângulo das tensões da barra 1
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
		txt += DoubleToStr("%5.4f;", pcte.bar1_vf[ind_fase].mod_volt);
      txt += DoubleToStr("%3.2f;", pcte.bar1_vf[ind_fase].ang_grau);
      }
   //módulo e ângulo das tensões da barra 2
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
		{
		txt += DoubleToStr("%5.4f;", pcte.bar2_vf[ind_fase].mod_volt);
      txt += DoubleToStr("%3.2f;", pcte.bar2_vf[ind_fase].ang_grau);
      }
	//módulo e ângulo das correntes do primário
	for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
		{
		txt += DoubleToStr("%5.4f;", pcte.pri_if[ind_fase].mod_amp);
		txt += DoubleToStr("%3.2f;", pcte.pri_if[ind_fase].ang_grau);
		}
	//módulo e ângulo das correntes do secundário
	for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
		{
		txt += DoubleToStr("%5.4f;", pcte.sec_if[ind_fase].mod_amp);
		txt += DoubleToStr("%3.2f;", pcte.sec_if[ind_fase].ang_grau);
		}
	//tap da chave
	txt += IntToStr(pcte.tap) + ";";

   return(txt);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgRegistroTrafo::PM_GetEqptoId(void)
   {
   return(pcte.eqpto_id);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgRegistroTrafo::PM_GetEqptoTipo(void)
   {
   return(pcte.eqpto_tipo);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgRegistroTrafo::PM_GetFuncao(void)
   {
   return(pcte.funcao);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgRegistroTrafo::PM_GetNumBytes(void)
   {
   return(pcte.num_bytes);
   }

//---------------------------------------------------------------------------
void __fastcall TMsgRegistroTrafo::PM_SetEqptoId(int eqpto_id)
   {
   pcte.eqpto_id = eqpto_id;
   }

//---------------------------------------------------------------------------
void __fastcall TMsgRegistroTrafo::PM_SetEqptoTipo(char eqpto_tipo)
   {
   pcte.eqpto_tipo = eqpto_tipo;
   }

//---------------------------------------------------------------------------
//eof
