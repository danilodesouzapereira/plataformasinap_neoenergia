//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "TMsgRegistroTrecho.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMsgRegistroTrecho* __fastcall NewMsgRegistroTrecho(void)
   {
   try{//cria objeto TMsgRegistroTrecho
      return(new TMsgRegistroTrecho());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall  TMsgRegistroTrecho::TMsgRegistroTrecho(void)
   {
   //monta mensagem com dados da Barra
   pcte.funcao     = funcaoENVIO_REGISTRO;
   pcte.num_bytes  = Length();
   //aloca mem�ria
   buffer.length = Length();
   buffer.memory = new char[buffer.length];
   }

//---------------------------------------------------------------------------
__fastcall  TMsgRegistroTrecho::~TMsgRegistroTrecho(void)
   {
   //libera mem�ria
   if (buffer.memory) {delete[] buffer.memory; buffer.memory = NULL;}
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMsgRegistroTrecho::ConverteBufToMsg(strBUF *buf)
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
bool __fastcall TMsgRegistroTrecho::ConverteBufToMsg(strBUF *buf, int index)
   {
   //prote��o: verifica se o buf recebido tem o n�mero de bytes esperado
   if ((buf->length - index) < buffer.length) return(false);
   //copia dados p/ buffer
   memcpy(buffer.memory, buf->memory + index, buffer.length);
   //copia dados p/ strPCTE_ACK
   memcpy(&pcte, buffer.memory, buffer.length);

   return(true);
   }

//---------------------------------------------------------------------------
strBUF* __fastcall TMsgRegistroTrecho::ConverteMsgToBuf(void)
   {
   memcpy(buffer.memory, &pcte, Length());
   return(&buffer);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgRegistroTrecho::Length(void)
   {
   return(sizeof(strPCTE_TRECHO));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMsgRegistroTrecho::MsgAsString(void)
   {
   //vari�veis locais
   AnsiString txt;

   //fun��o
   txt  = "Envio Registro(Trecho): ";
   txt += IntToStr(pcte.funcao)      + ";";
   txt += IntToStr(pcte.num_bytes)   + ";";
   txt += IntToStr(pcte.hora)        + ";";
   txt += IntToStr(pcte.minuto)      + ";";
   txt += IntToStr(pcte.segundo)     + ";";
   txt += IntToStr(pcte.fluxo_curto) + ";";
   txt += IntToStr(pcte.eqpto_tipo)  + ";";
   txt += IntToStr(pcte.eqpto_id)    + ";";
   //m�dulo e �ngulo das correntes
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      //tens�o fase A
		txt += DoubleToStr("%5.4f;", pcte.tre_if[ind_fase].mod_amp);
      txt += DoubleToStr("%3.2f;", pcte.tre_if[ind_fase].ang_grau);
      }
	return(txt);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgRegistroTrecho::PM_GetEqptoId(void)
   {
   return(pcte.eqpto_id);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgRegistroTrecho::PM_GetEqptoTipo(void)
   {
   return(pcte.eqpto_tipo);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgRegistroTrecho::PM_GetFuncao(void)
   {
   return(pcte.funcao);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgRegistroTrecho::PM_GetNumBytes(void)
   {
   return(pcte.num_bytes);
   }

//---------------------------------------------------------------------------
void __fastcall TMsgRegistroTrecho::PM_SetEqptoId(int eqpto_id)
   {
   pcte.eqpto_id = eqpto_id;
   }

//---------------------------------------------------------------------------
void __fastcall TMsgRegistroTrecho::PM_SetEqptoTipo(char eqpto_tipo)
   {
   pcte.eqpto_tipo = eqpto_tipo;
   }

//---------------------------------------------------------------------------
//eof
