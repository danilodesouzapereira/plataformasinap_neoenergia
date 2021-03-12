//-----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TLog.h"
#include "TFormLog1.h"
#include "TFormLog2.h"

//-----------------------------------------------------------------------------
VTLog* NewObjLog(VTApl *apl_owner)
   {
   try{
      return(new TLog(apl_owner));
      }catch(Exception &e)
         {//nada a fazer
         }
   //erro na criação do objeto
   return(NULL);
   }

//-----------------------------------------------------------------------------
__fastcall TLog::TLog(VTApl *apl_owner)
   {
   //salva ponteiro p/ objeto
   apl = apl_owner;
   //inicia variáveis
   fp             = NULL;
   ExibirDataHora = false;
   //cria objetos
   StrListLog           = new TStringList();
   campos.StrListNome  = new TStringList();
   campos.StrListValor = new TStringList();
   }

//-----------------------------------------------------------------------------
__fastcall TLog::~TLog(void)
   {
   //fecha arquivo de log
   Close();
   //destrói objetos
   if (campos.StrListNome)  {delete campos.StrListNome;  campos.StrListNome  = NULL;}
   if (campos.StrListValor) {delete campos.StrListValor; campos.StrListValor = NULL;}
   }

//-----------------------------------------------------------------------------
bool __fastcall TLog::Add(AnsiString msg)
   {
   //variáveis locais
   AnsiString campo;
   AnsiString txt;

   if (campos.StrListValor->Count == 0)
      {
      return(WriteToFile(msg));
      }
   else
      {//define valor do último campo
      campo = campos.StrListNome->Strings[campos.StrListNome->Count-1];
      DefineValor(campo, msg);
      //monta string c/ valores de todos os campos
      for (int n = 0; n < campos.StrListValor->Count; n++)
         {
         txt += campos.StrListValor->Strings[n] + ";";
         }
      return(WriteToFile(txt));
      }
   }


//-----------------------------------------------------------------------------
AnsiString __fastcall TLog::ArqLog(void)
   {
   return(arq);
   }

//-----------------------------------------------------------------------------
TStrings* __fastcall TLog::Campos(void)
   {
   return(campos.StrListNome);
   }

//-----------------------------------------------------------------------------
void __fastcall TLog::Close(void)
   {
   //fecha arquivo
   if (fp != NULL) {fclose(fp); fp = NULL;}
   //destrói ponteiro p/ StrListLog
   if (StrListLog) {delete StrListLog; StrListLog = NULL;}
   }

//-----------------------------------------------------------------------------
void __fastcall TLog::DefineValor(AnsiString campo, AnsiString valor)
   {
   //variáveis locais
   int index;

   //verifica se existe o campo
   if ((index = campos.StrListNome->IndexOf(campo)) >= 0)
      {//atualiza o valor
      if (valor.IsEmpty()) valor = " ";
      campos.StrListValor->Strings[index] = valor;
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TLog::InsereCampo(AnsiString campo, AnsiString valor)
   {
   //verifica se existe o campo
   if (campos.StrListNome->IndexOfName(campo) < 0)
      {//insere o novo campo
      campos.StrListNome->Add(campo);
      campos.StrListValor->Add(valor);
      }
   else
      {//atualiza valor do campo
      DefineValor(campo, valor);
      }
   }

//-----------------------------------------------------------------------------
bool __fastcall TLog::Open(AnsiString arq_log, bool append)
   {
   //proteção: fecha eventual arquivo já aberto
   Close();
   //salva nome do arquivo
   arq = arq_log.Trim();
   try{//verifica se foi definido um arquivo
      if (arq.IsEmpty())
         {//cria objeto TStringList
         StrListLog = new TStringList();
         }
      else
         {//abre arquivo de log
         if (append) {fp = fopen(arq.c_str(), "a");}
         else        {fp = fopen(arq.c_str(), "w");}
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return((StrListLog != NULL)||(fp != NULL));
   }

//-----------------------------------------------------------------------------
void __fastcall TLog::ShowModal(AnsiString Caption)
   {
   //variáveis locais
   TFormLog1 *FormLog1;
   TFormLog2 *FormLog2;

   //proteção:
   if ((StrListLog == NULL)&&(! FileExists(arq))) return;
   try{//verifica se deve mostrar texto simples ou planilha
      if (campos.StrListNome->Count == 0)
         {//cria TFormLog1
         FormLog1 = new TFormLog1(NULL);
         FormLog1->Caption = (Caption.IsEmpty()) ? AnsiString("Log") : Caption;
         //exibe texto do log
         if      (FileExists(arq))    FormLog1->ExibeTexto(arq);
         else if (StrListLog != NULL) FormLog1->ExibeTexto(StrListLog);
         //exibe arquivo como janela modal
         FormLog1->ShowModal();
         //destrói FormLog1
         delete FormLog1;
         }
      else
         {//cria FormLog2
         FormLog2 = new TFormLog2(NULL, apl);
         FormLog2->Caption = (Caption.IsEmpty()) ? AnsiString("Log") : Caption;
         //exibe texto do log
         if      (FileExists(arq))    FormLog2->ExibeLog(arq, campos.StrListNome);
         else if (StrListLog != NULL) FormLog2->ExibeLog(StrListLog,  campos.StrListNome);
         //exibe arquivo como janela modal
         FormLog2->ShowModal();
         //destrói FormLog2
         delete FormLog2;
         }
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//-----------------------------------------------------------------------------
bool __fastcall TLog::WriteToFile(AnsiString txt)
   {
   //variáveis locais
   Word       ano, mes, dia, hora, minuto, segundo, mseg;
   AnsiString datahora;

   //verifica se deve incluir data/hora
   if (ExibirDataHora)
      {
	  DecodeDateTime(Now(),  ano, mes, dia, hora, minuto, segundo, mseg);
	  datahora.sprintf("%4d/%02d/%02d %02d:%02d:%02d.%03d; ", ano, mes, dia, hora, minuto, segundo, mseg);
      txt = datahora + txt;
      }
   //verifica se está gravando em arquivo
   if (fp != NULL)
      {//inclui 'new line' e grava mensagem
      txt += "\n";
      fputs(txt.c_str(), fp);
      fflush(fp);
      return(true);
      }
   //verifica se está gravando em um TStringList
   if (StrListLog != NULL)
      {
      StrListLog->Add(txt);
      return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
//eof