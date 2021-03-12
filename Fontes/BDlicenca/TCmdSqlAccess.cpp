//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCmdSqlAccess.h"
#include "..\..\DLL_Inc\\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCmdSql* __fastcall NewObjCmdSqlAccess(void)
   {
   return(new TCmdSqlAccess());
   }

//---------------------------------------------------------------------------
__fastcall TCmdSqlAccess::TCmdSqlAccess(void)
   {
   //cria StringList
   sql.parametro = new TStringList();
   }

//---------------------------------------------------------------------------
__fastcall TCmdSqlAccess::~TCmdSqlAccess(void)
   {
   //destr�i objetos
   if (sql.parametro) {delete sql.parametro; sql.parametro = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TCmdSqlAccess::AddComando(AnsiString cmd)
   {
   //salva o comando
   sql.comando = cmd;
   //reinicia lista de par�metros
   sql.parametro->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TCmdSqlAccess::AddParametro(AnsiString valor)
   {
   //salva par�metro
   //sql.parametro->Add("'" + valor + "'");
   sql.parametro->Add(AnsiQuotedStr(valor, '\''));
   }

//---------------------------------------------------------------------------
void __fastcall TCmdSqlAccess::AddParametro(TDate valor)
   {
   sql.parametro->Add(DataHoraToSql(valor));
   }

//---------------------------------------------------------------------------
void __fastcall TCmdSqlAccess::AddParametro(int valor)
   {
   //salva par�metro
   sql.parametro->Add(IntToStr(valor));
   }

//---------------------------------------------------------------------------
void __fastcall TCmdSqlAccess::AddParametro(double valor)
   {
   //salva par�metro
   sql.parametro->Add(DoubleToStr("%5.4f", valor, '.'));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCmdSqlAccess::DataHoraToSql(TDate data)
   {
   //verifica data nula
   if (int(data) == 0) return("null");
   try{//converte data p/ string
      return("'" + DateTimeToStr(data) + "'");
      }catch(Exception &e)
         {
         }
   return("null");
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCmdSqlAccess::PM_GetText(void)
   {
   //vari�veis locais
   AnsiString cmd_sql;

   //monta o comando sql
   cmd_sql = "EXECUTE " + sql.comando;
   for (int n = 0; n < sql.parametro->Count; n++)
      {//inclui v�rgula a partir do segundo par�metro
      if (n > 0) cmd_sql += ",";
      cmd_sql += " " + sql.parametro->Strings[n];
      }
   return(cmd_sql);
   }

//---------------------------------------------------------------------------
//eof
