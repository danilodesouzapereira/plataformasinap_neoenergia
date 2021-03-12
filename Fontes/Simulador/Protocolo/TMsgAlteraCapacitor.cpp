//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMsgAlteraCapacitor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMsgAlteraCapacitor* __fastcall NewMsgAlteraCapacitor(void)
   {
   try{//cria objeto TMsgAlteraCapacitor
      return(new TMsgAlteraCapacitor());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall  TMsgAlteraCapacitor::TMsgAlteraCapacitor(void)
   {
   //monta mensagem
	pcte.funcao     = funcaoALTERA_CAPACITOR;
	pcte.num_bytes  = Length();
   //aloca memória
   buffer.length = Length();
   buffer.memory = new char[buffer.length];
   }

//---------------------------------------------------------------------------
__fastcall  TMsgAlteraCapacitor::~TMsgAlteraCapacitor(void)
   {
   //libera memória
   if (buffer.memory) {delete[] buffer.memory; buffer.memory = NULL;}
	}

//---------------------------------------------------------------------------
bool __fastcall TMsgAlteraCapacitor::ConverteBufToMsg(strBUF *buf, int index)
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
strBUF* __fastcall TMsgAlteraCapacitor::ConverteMsgToBuf(void)
   {
   memcpy(buffer.memory, &pcte, Length());
   return(&buffer);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgAlteraCapacitor::EqptoId(void)
   {
   return(pcte.eqpto_id);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgAlteraCapacitor::EqptoTipo(void)
   {
   return(pcte.eqpto_tipo);
   }

//---------------------------------------------------------------------------
bool __fastcall TMsgAlteraCapacitor::HoraMinutoSegundo(strHMS &hms)
   {
   hms.hora    = pcte.hora;
   hms.minuto  = pcte.minuto;
   hms.segundo = pcte.segundo;

   return(true);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgAlteraCapacitor::Length(void)
   {
	return(sizeof(strPCTE_CAPACITOR));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMsgAlteraCapacitor::MsgAsString(void)
   {
   //variáveis locais
   AnsiString  txt;

	txt  = "Altera Capacitor: ";
   txt += IntToStr(pcte.funcao)       + ";";
	txt += IntToStr(pcte.num_bytes)    + ";";
	txt += IntToStr(pcte.hora)         + ";";
	txt += IntToStr(pcte.minuto)       + ";";
	txt += IntToStr(pcte.segundo)      + ";";
	txt += IntToStr(pcte.eqpto_tipo)   + ";";
	txt += IntToStr(pcte.eqpto_id)     + ";";
	//estado ligado/desligado
	txt += IntToStr(pcte.ligado)       + ";";

   return(txt);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgAlteraCapacitor::PM_GetEqptoId(void)
   {
   return(pcte.eqpto_id);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgAlteraCapacitor::PM_GetEqptoTipo(void)
   {
   return(pcte.eqpto_tipo);
   }
//---------------------------------------------------------------------------
char __fastcall TMsgAlteraCapacitor::PM_GetFuncao(void)
   {
   return(pcte.funcao);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgAlteraCapacitor::PM_GetNumBytes(void)
   {
   return(pcte.num_bytes);
   }

//---------------------------------------------------------------------------
void __fastcall TMsgAlteraCapacitor::PM_SetEqptoId(int eqpto_id)
   {
   pcte.eqpto_id = eqpto_id;
   }

//---------------------------------------------------------------------------
void __fastcall TMsgAlteraCapacitor::PM_SetEqptoTipo(char eqpto_tipo)
   {
   pcte.eqpto_tipo = eqpto_tipo;
   }

//---------------------------------------------------------------------------
//eof
