//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMsgSupervisaoAuto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMsgSupervisaoAuto* __fastcall NewMsgSupervisaoAuto(void)
   {
   try{//cria objeto TMsgSupervisaoAuto
      return(new TMsgSupervisaoAuto());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall  TMsgSupervisaoAuto::TMsgSupervisaoAuto(void)
   {
   //monta mensagem inv�lida
	pcte.funcao       = funcaoSUPERVISAO_AUTO;
   pcte.num_bytes    = Length();
   //aloca mem�ria
   buffer.length = Length();
   buffer.memory = new char[buffer.length];
   }

 //---------------------------------------------------------------------------
__fastcall  TMsgSupervisaoAuto::~TMsgSupervisaoAuto(void)
   {
   //libera mem�ria
   if (buffer.memory) {delete[] buffer.memory; buffer.memory = NULL;}
   }

//---------------------------------------------------------------------------
short __fastcall TMsgSupervisaoAuto::Length(void)
   {
	return(sizeof(strPCTE_SUPERVISAO_AUTO));
   }

//---------------------------------------------------------------------------
bool __fastcall TMsgSupervisaoAuto::ConverteBufToMsg(strBUF *buf, int index)
   {
   //prote��o: verifica se o buf recebido tem o n�mero de bytes esperado
   if ((buf->length - index) < buffer.length) return(false);
   //copia dados p/ buffer
   memcpy(buffer.memory, buf->memory + index, buffer.length);
	//copia dados p/ strPCTE_SUPERVISAO_AUTO
   memcpy(&pcte, buffer.memory, buffer.length);

   return(true);
   }

//---------------------------------------------------------------------------
strBUF* __fastcall TMsgSupervisaoAuto::ConverteMsgToBuf(void)
   {
   memcpy(buffer.memory, &pcte, Length());
   return(&buffer);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMsgSupervisaoAuto::MsgAsString(void)
   {
   //vari�veis locais
   AnsiString  txt;

	txt  = "SUPERVIS�O_AUTO: ";
   txt += IntToStr(pcte.funcao)     + ";";
	txt += IntToStr(pcte.num_bytes)  + ";";
   txt += IntToStr(pcte.hora)       + ";";
   txt += IntToStr(pcte.minuto)     + ";";
   txt += IntToStr(pcte.segundo)    + ";";
	txt += IntToStr(pcte.enabled)  + ";";

   return(txt);
   }

//---------------------------------------------------------------------------
bool __fastcall TMsgSupervisaoAuto::PM_GetEnabled(void)
	{
	return(pcte.enabled == 1);
	}

//---------------------------------------------------------------------------
int __fastcall TMsgSupervisaoAuto::PM_GetEqptoId(void)
   {
	//n�o se aplica
   return(0);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgSupervisaoAuto::PM_GetEqptoTipo(void)
   {
   //n�o se aplica
   return(0);
   }
//---------------------------------------------------------------------------
char __fastcall TMsgSupervisaoAuto::PM_GetFuncao(void)
   {
   return(pcte.funcao);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgSupervisaoAuto::PM_GetNumBytes(void)
   {
	return(pcte.num_bytes);
   }

//---------------------------------------------------------------------------
void __fastcall TMsgSupervisaoAuto::PM_SetEnabled(bool enabled)
	{
	pcte.enabled = (enabled) ? 1 : 0;
	}

//---------------------------------------------------------------------------
void __fastcall TMsgSupervisaoAuto::PM_SetEqptoId(int eqpto_id)
	{
	//n�o se aplica
	}

//---------------------------------------------------------------------------
void __fastcall TMsgSupervisaoAuto::PM_SetEqptoTipo(char eqpto_tipo)
   {
   //n�o se aplica
   }

//---------------------------------------------------------------------------
//eof
