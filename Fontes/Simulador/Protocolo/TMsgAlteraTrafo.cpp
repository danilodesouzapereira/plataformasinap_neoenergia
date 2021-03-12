//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMsgAlteraTrafo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMsgAlteraTrafo* __fastcall NewMsgAlteraTrafo(void)
   {
   try{//cria objeto TMsgAlteraTrafo
      return(new TMsgAlteraTrafo());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall  TMsgAlteraTrafo::TMsgAlteraTrafo(void)
   {
   //monta mensagem
	pcte.funcao     = funcaoALTERA_TRAFO;
   pcte.num_bytes  = Length();
   //aloca memória
   buffer.length = Length();
   buffer.memory = new char[buffer.length];
   }

//---------------------------------------------------------------------------
__fastcall  TMsgAlteraTrafo::~TMsgAlteraTrafo(void)
   {
   //libera memória
   if (buffer.memory) {delete[] buffer.memory; buffer.memory = NULL;}
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMsgAlteraTrafo::ConverteBufToMsg(strBUF *buf)
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
bool __fastcall TMsgAlteraTrafo::ConverteBufToMsg(strBUF *buf, int index)
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
strBUF* __fastcall TMsgAlteraTrafo::ConverteMsgToBuf(void)
   {
   memcpy(buffer.memory, &pcte, Length());
   return(&buffer);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgAlteraTrafo::EqptoId(void)
   {
   return(pcte.eqpto_id);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgAlteraTrafo::EqptoTipo(void)
   {
   return(pcte.eqpto_tipo);
   }

//---------------------------------------------------------------------------
bool __fastcall TMsgAlteraTrafo::HoraMinutoSegundo(strHMS &hms)
   {
   hms.hora    = pcte.hora;
   hms.minuto  = pcte.minuto;
   hms.segundo = pcte.segundo;

   return(true);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgAlteraTrafo::Length(void)
   {
	return(sizeof(strPCTE_TRAFO));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMsgAlteraTrafo::MsgAsString(void)
   {
   //variáveis locais
   AnsiString  txt;

	txt  = "Altera Trafo: ";
   txt += IntToStr(pcte.funcao)       + ";";
	txt += IntToStr(pcte.num_bytes)    + ";";
	txt += IntToStr(pcte.hora)         + ";";
	txt += IntToStr(pcte.minuto)       + ";";
	txt += IntToStr(pcte.segundo)      + ";";
	txt += IntToStr(pcte.eqpto_tipo)   + ";";
	txt += IntToStr(pcte.eqpto_id)     + ";";
	//variação do tap do trafo
	txt += IntToStr(pcte.variacao_tap) + ";";

   return(txt);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgAlteraTrafo::PM_GetEqptoId(void)
   {
   return(pcte.eqpto_id);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgAlteraTrafo::PM_GetEqptoTipo(void)
   {
   return(pcte.eqpto_tipo);
   }
//---------------------------------------------------------------------------
char __fastcall TMsgAlteraTrafo::PM_GetFuncao(void)
   {
   return(pcte.funcao);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgAlteraTrafo::PM_GetNumBytes(void)
   {
   return(pcte.num_bytes);
   }

//---------------------------------------------------------------------------
void __fastcall TMsgAlteraTrafo::PM_SetEqptoId(int eqpto_id)
   {
   pcte.eqpto_id = eqpto_id;
   }

//---------------------------------------------------------------------------
void __fastcall TMsgAlteraTrafo::PM_SetEqptoTipo(char eqpto_tipo)
   {
   pcte.eqpto_tipo = eqpto_tipo;
   }

//---------------------------------------------------------------------------
//eof
