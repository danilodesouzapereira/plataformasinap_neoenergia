//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAccess.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAccess* __fastcall NewObjAccess(void)
   {
   try{//cria objeto
      return(new TAccess());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TAccess::TAccess(void)
   {
   //cria objetos de acesso ao banco de dados
   connection = new TADOConnection(NULL);
   query      = new TADOQuery(NULL);
   //associa ADOQuery c/ ADOConnection
   query->Connection = connection;
   }

//---------------------------------------------------------------------------
__fastcall TAccess::~TAccess(void)
   {
   //destr�i objetos
   if (query)      {query->Close(); delete query;      query      = NULL;}
   if (connection) {Desconecta();   delete connection; connection = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TAccess::Conecta(AnsiString database_name)
   {
   //vari�veis locais
   AnsiString password = "ultraplan2006";
   AnsiString ParametrosConexao;

   //prote��o: verifica se foi criado objeto TADOConnection
   if (connection == NULL) return(false);
   //verifica se j� est� conectado
   if (Conectado(database_name)) return(true);
   //cancela conex�o atual
   Desconecta();
   //inicia nova conex�o p/ a base de dados indicada
   ParametrosConexao  = "Provider=Microsoft.ACE.OLEDB.12.0;";
   ParametrosConexao += "Data Source=" + database_name + ";";
   //ParametrosConexao += "User Id="     + user_id  + ";";
   ParametrosConexao += "Jet OLEDB:Database Password=" + password + ";";
   try{//tenta iniciar conex�o
      connection->ConnectionString = ParametrosConexao;
      connection->LoginPrompt      = false;
      connection->Connected        = true;
      //salva nome da base de dados
      arq_mdb = database_name;
      }catch (Exception& e)
         {
         return(false);
         }
   return(Conectado());
   }

//---------------------------------------------------------------------------
bool __fastcall TAccess::Conectado(void)
   {
   return(connection->Connected);
   }

//---------------------------------------------------------------------------
bool __fastcall TAccess::Conectado(AnsiString database_name)
   {
   //verifica se j� existe uma conex�o ativa
   if (! Conectado()) return(false);
   //verifica se a base de dados � a mesma
   return(arq_mdb.AnsiCompareIC(database_name) == 0);
   }

//---------------------------------------------------------------------------
void __fastcall TAccess::Desconecta(void)
   {
   //cancela nome da base de dados
   arq_mdb = "";
   //verifica se est� conectado e desconecta
   if (connection->Connected) connection->Connected = false;
   }

//---------------------------------------------------------------------------
TDataSet* __fastcall TAccess::ExecutaCmdSelect(AnsiString cmd_sql)
   {
   //prote��o: verifica se est� conectado
   if (! connection->Connected) return(NULL);
   try{//executa comando SQL
      IniciaQuery(cmd_sql);
      query->Prepared = true;
      query->Open();
      }catch(Exception&e)
         {
         return(NULL);
         }
   return((query->Eof) ? NULL : query);
   }

//---------------------------------------------------------------------------
bool __fastcall TAccess::ExecutaCmdUpdate(AnsiString cmd_sql)
   {
   //prote��o: verifica se est� conectado
   if (! connection->Connected) return(false);
   try{//executa comando SQL
      IniciaQuery(cmd_sql);
      query->Prepared = true;
      query->ExecSQL();
      }catch(Exception&e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TAccess::IniciaQuery(AnsiString cmd_sql)
   {
   try{//reinicia ADOQuery
      if (query->Active) {query->Close();}
      query->Filtered = false;
      query->Filter   = "";
      //inicia ADOQuery
      query->SQL->Clear();
      query->SQL->Add(cmd_sql);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

