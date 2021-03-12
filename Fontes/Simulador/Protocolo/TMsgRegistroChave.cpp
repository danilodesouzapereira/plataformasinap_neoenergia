//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "TMsgRegistroChave.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TMsgRegistroChave* __fastcall NewMsgRegistroChave(void)
   {
   try{//cria objeto TMsgRegistroChave
      return(new TMsgRegistroChave());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall  TMsgRegistroChave::TMsgRegistroChave(void)
   {
   //monta mensagem com dados da Barra
   pcte.funcao     = funcaoENVIO_REGISTRO;
   pcte.num_bytes  = Length();
   //aloca memória
   buffer.length = Length();
   buffer.memory = new char[buffer.length];
   }

//---------------------------------------------------------------------------
__fastcall  TMsgRegistroChave::~TMsgRegistroChave(void)
   {
   //libera memória
   if (buffer.memory) {delete[] buffer.memory; buffer.memory = NULL;}
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMsgRegistroChave::ConverteBufToMsg(strBUF *buf)
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
bool __fastcall TMsgRegistroChave::ConverteBufToMsg(strBUF *buf, int index)
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
strBUF* __fastcall TMsgRegistroChave::ConverteMsgToBuf(void)
   {
   memcpy(buffer.memory, &pcte, Length());
   return(&buffer);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgRegistroChave::Length(void)
   {
   return(sizeof(strPCTE_CHAVE));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMsgRegistroChave::MsgAsString(void)
   {
   //variáveis locais
   AnsiString txt;

   //função
   txt  = "Envio Registro(Chave): ";
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
      //tensão fase A
      txt += DoubleToStr("%5.4f;", pcte.bar1_vf[ind_fase].mod_volt);
      txt += DoubleToStr("%3.2f;", pcte.bar1_vf[ind_fase].ang_grau);
      }
   //módulo e ângulo das tensões da barra 2
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      //tensão fase A
      txt += DoubleToStr("%5.4f;", pcte.bar2_vf[ind_fase].mod_volt);
      txt += DoubleToStr("%3.2f;", pcte.bar2_vf[ind_fase].ang_grau);
      }
   //módulo e ângulo das correntes da barra 2
   for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
      {
      //tensão fase A
		txt += DoubleToStr("%5.4f;", pcte.chv_if[ind_fase].mod_amp);
      txt += DoubleToStr("%3.2f;", pcte.chv_if[ind_fase].ang_grau);
      }
   //estado da chave
	txt += IntToStr(pcte.estado)  + ";";
	//cbr, cbtl e lockout
	txt += IntToStr(pcte.cbr)     + ";";
	txt += IntToStr(pcte.cbtl)    + ";";
	txt += IntToStr(pcte.lockout) + ";";
	//potência ativa e reativa trifásica
	txt += DoubleToStr("%4.3f;", pcte.p3f_mw);
	txt += DoubleToStr("%4.3f;", pcte.q3f_mvar);
	//potência ativa e reativa faseA
	txt += DoubleToStr("%4.3f;", pcte.pa_mw);
	txt += DoubleToStr("%4.3f;", pcte.qa_mvar);
	//potência ativa e reativa faseB
	txt += DoubleToStr("%4.3f;", pcte.pb_mw);
	txt += DoubleToStr("%4.3f;", pcte.qb_mvar);
	//potência ativa e reativa faseC
	txt += DoubleToStr("%4.3f;", pcte.pc_mw);
	txt += DoubleToStr("%4.3f;", pcte.qc_mvar);
	return(txt);
   }

//---------------------------------------------------------------------------
int __fastcall TMsgRegistroChave::PM_GetEqptoId(void)
   {
   return(pcte.eqpto_id);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgRegistroChave::PM_GetEqptoTipo(void)
   {
   return(pcte.eqpto_tipo);
   }

//---------------------------------------------------------------------------
char __fastcall TMsgRegistroChave::PM_GetFuncao(void)
   {
   return(pcte.funcao);
   }

//---------------------------------------------------------------------------
short __fastcall TMsgRegistroChave::PM_GetNumBytes(void)
   {
   return(pcte.num_bytes);
   }

//---------------------------------------------------------------------------
void __fastcall TMsgRegistroChave::PM_SetEqptoId(int eqpto_id)
   {
   pcte.eqpto_id = eqpto_id;
   }

//---------------------------------------------------------------------------
void __fastcall TMsgRegistroChave::PM_SetEqptoTipo(char eqpto_tipo)
   {
   pcte.eqpto_tipo = eqpto_tipo;
   }

//---------------------------------------------------------------------------
//eof
