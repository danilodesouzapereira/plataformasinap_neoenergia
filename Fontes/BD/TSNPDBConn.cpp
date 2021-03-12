//---------------------------------------------------------------------------
//
// TSNPDBConn.cpp
//
// Verificar descri��o e controle de vers�es em TSNPDBConn.h.
//
//---------------------------------------------------------------------------

#include "VTSNPDBConn.h"
#include "TSNPDBConn.h"
#include "ADOInt.hpp"
#include "ADODB.hpp"
#include "..\Apl\VTApl.h"
#include "..\Log\VTLog.h"

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe
VTSNPDBConn* __fastcall NewObjSNPDBConn(VTApl* apl, AnsiString strDBName, AnsiString strLoginName, AnsiString strLoginPwd, int intDBType, bool bolMultiDataSet)
{
  return new TSNPDBConn(apl, strDBName, strLoginName, strLoginPwd, intDBType, bolMultiDataSet);
} // NewObjSNPDBConn

//---------------------------------------------------------------------------
// 1) Depende de que o �ltimo item da string de conex�o seja o nome banco de dados e sem ponto e v�rgula!
bool __fastcall CompactDatabase(AnsiString strDBName, AnsiString strLoginName, AnsiString strLoginPwd)
   {
   bool       bolret   = false;
   Variant    olvJet   = NULL;
   AnsiString strSufix = "_compactado";
   AnsiString strDB01  = TSNPDBConn::getConnStringMSAccess(strDBName, strLoginName, strLoginPwd);
   AnsiString strDB02  = "Jet OLEDB:Engine type=5;" + strDB01 + strSufix;

   try
      {
      olvJet = olvJet.CreateObject("JRO.JetEngine");
      olvJet.OleProcedure("CompactDatabase", strDB01.c_str(), strDB02.c_str());
      if (DeleteFile(strDBName))
         {
         if (RenameFile(strDBName + strSufix, strDBName))
            {
            bolret = true;
            }
         }
      }
   catch (Exception& e)
      {
      bolret = false;
      // oops: Logar alguma coisa aqui???
      }

   // Limpa a vari�vel
   olvJet = NULL;

   return(bolret);
   } // CompactDatabase

//---------------------------------------------------------------------------
__fastcall TSNPDBConn::TSNPDBConn(VTApl* apl, AnsiString strDBName,
                                  AnsiString strLoginName, AnsiString strLoginPwd,
                                  int intDBType        /* SNPDBTYPE_ORACLE, SNPDBTYPE_ACCESS */,
                                  bool bolMultiDataSet /* Indica um �nico dataset gerenciado pela classe ou */
                                                       /* v�rios datasets, destru�dos manualmente           */

                                 )
   {
   AnsiString strlog;

   // Objetos globais
   this->apl = apl;
   // Obt�m o ponteiro para log
   plog = (VTLog*)apl->GetObject(__classid(VTLog));

   m_ptrADOConnection = NULL;
   m_ptrADOCmd = NULL;
   m_ptrQryDS = NULL;

   m_strDBName = strDBName;
   m_strLoginName = strLoginName;
   m_strLoginPwd = strLoginPwd;
   m_intDBType = intDBType;
   m_bolMultiDataSet = bolMultiDataSet;

   m_bolInitOK = Init();
   if (!m_bolInitOK)
      {
      SNPSTRLOGLINE(strlog)
      }
   } // TSNPDBConn

//---------------------------------------------------------------------------
__fastcall TSNPDBConn::~TSNPDBConn(void)
{
 AnsiString strlog;

  if (!Shut()) {
    SNPSTRLOGLINE(strlog)
  }
} // ~TSNPDBConn

bool __fastcall TSNPDBConn::getConnectedOK(bool bolRetry)
{
  if (m_ptrADOConnection) {
    if (m_ptrADOConnection->Connected) {
      return(true);
    }
    else {
      if (bolRetry) {
        Init();
        return(m_ptrADOConnection ? m_ptrADOConnection->Connected : false);
      }
      else {
        return(false);
      }
    }
  }
  else {
    // Se n�o tem conex�o, tenta reconectar
    if (bolRetry) {
      Init();
      return(m_ptrADOConnection ? m_ptrADOConnection->Connected : false);
    }
    else {
      return(false);
    }
  }
}

//---------------------------------------------------------------------------
bool __fastcall TSNPDBConn::FindTable(AnsiString strTableName, bool bolSystemTables)
    {
    bool         sucesso   = true;
    TStringList* ptrStrLst = NULL;
    int          intaux    = -1;
    AnsiString   strlog;

    try
        {
        if (!m_ptrADOConnection) return(false);

        ptrStrLst = new TStringList();
        m_ptrADOConnection->GetTableNames(ptrStrLst, bolSystemTables);
        ptrStrLst->CaseSensitive = false;
        if (!ptrStrLst->Find(strTableName, intaux))
          sucesso = false;
        delete ptrStrLst;
        ptrStrLst = NULL;
        }
    catch(Exception &e)
        {
        SNPSTRLOGLINE(strlog)
        SNPSTRLOG(e.Message);
        sucesso = false;
        assert(bolret);
        }  // catch new m_ptrADOConnection

    return(sucesso);
    } // FindTable

//---------------------------------------------------------------------------
bool __fastcall TSNPDBConn::Init()
{
 bool bolret;
 AnsiString strlog;

 try {
   // Constroi a string de conex�o
   if (!setConnString()) {
     SNPSTRLOGLINE(strlog)
     return(false);
   }

   // Somente aloca se j� n�o tiver sido alocado
   if (!m_ptrADOConnection)
     m_ptrADOConnection = new TADOConnection(NULL);

   m_ptrADOConnection->ConnectionString = m_strConnection;
   m_ptrADOConnection->LoginPrompt = false;
   m_ptrADOConnection->Connected = true;
   bolret = m_ptrADOConnection->Connected;
   if (bolret) {
     try {
      // No caso de uma conex�o ACCESS, limita a quantidade de mem�ria utilizada
/*
      if (m_intDBType == SNPDBTYPE_ACCESS)
         {
         Adoint::_di_Property_ adoProp;
         AnsiString strName;

         // Como n�o surtiu o efeito desejado, mantive a configura��o original,
         // por�m, deixei o m�todo de acesso aos par�metros, para futuras neces-
         // sidades.
         // mml@20120116 - Reativado, pois houve melhora de 20% na leitura das redes.
         // mml@20120127 - Retirado novamente, pois na vers�o release n�o funciona
         // Colocando ainda valores de 256, 128 ou 64 MB n�o funciona (erro E_FAIL).
         // Colocando 8, 16 ou 32MB n�o muda o desempenho em nada.
         for (int i = 0; i < m_ptrADOConnection->Properties->Count; i++)
            {
            adoProp = m_ptrADOConnection->Properties->Item[i];
            strName = adoProp->Name;
//            if (strName.Pos("Jet OLEDB:Max Buffer Size") > 0)
//               {
//               MessageDlg("Max Buffer Size OK", mtInformation, TMsgDlgButtons() << mbOK, 0);
//               adoProp->Value = 1024 * 1024;
//               break;
//               }

//            if (strName.Pos("Jet OLEDB:Enable Fat Cursors") > 0)
//               {
//               MessageDlg("Max Buffer Size OK", mtInformation, TMsgDlgButtons() << mbOK, 0);
//               adoProp->Value = "true";
//               break;
               }
            }
         }
*/         

       // Somente aloca se j� n�o tiver sido alocado
       if (!m_ptrADOCmd)
         m_ptrADOCmd = new TADOCommand(NULL);
       m_ptrADOCmd->Connection = m_ptrADOConnection;
     }
     catch(Exception &e) {
       SNPSTRLOGLINE(strlog)
       SNPSTRLOG(e.Message);
       bolret = false;
       assert(bolret);
     }

     if (!m_bolMultiDataSet)
         {
         // Somente um dataset para todas as execu��es.
         // A cada execu��o � detonado...
         try
             {
             // Somente aloca se j� n�o tiver sido alocado
             if (!m_ptrQryDS)
               m_ptrQryDS = new TADOQuery(NULL);
             m_ptrQryDS->Connection = m_ptrADOConnection;
             }
         catch(Exception &e)
             {
             SNPSTRLOGLINE(strlog)
             SNPSTRLOG(e.Message);
             bolret = false;
             assert(bolret);
             }
         }
   } // if bolret
 }
 catch(Exception &e) {
   SNPSTRLOGLINE(strlog)
   SNPSTRLOG(e.Message);
   bolret = false;
   assert(bolret);
 }  // catch new m_ptrADOConnection

  return(bolret);
} // Init

//---------------------------------------------------------------------------
bool __fastcall TSNPDBConn::setDBName(AnsiString strDBName)
{
 bool bolret;

  bolret = true;
  if (strDBName != m_strDBName) {
    m_strDBName = strDBName;
    Shut();
    bolret = Init();
  }

  return(bolret);
} // setDBName

//---------------------------------------------------------------------------
AnsiString __fastcall TSNPDBConn::getConnStringMSAccess(AnsiString strDBName, AnsiString strLoginName, AnsiString strLoginPwd)
   {
   AnsiString strret;

   strret = "";

   if (!FileExists(strDBName))
     return("");

   strret = "Provider=Microsoft.ACE.OLEDB.12.0;";
   if ((strLoginName.IsEmpty()) && (!strLoginPwd.IsEmpty()))
       {
       strret += "Jet OLEDB:Database Password=" + strLoginPwd + ";";
       }
   else
       {
       strret += "User Id=" + strLoginName + ";";
       strret += "Password=" + strLoginPwd + ";";
       }
   //strret += "Jet OLEDB:Mode=Share Exclusive;";
   // Colocando em modo exclusive vem a mensagem: "Could not find installable ISAM"
   // Coloquei o nome do banco por �ltimo propositalmente, devido � rotina
   // para compacta��o.
   strret += "Data Source=" + strDBName;

   return(strret);
   }

//---------------------------------------------------------------------------
bool __fastcall TSNPDBConn::setConnString(AnsiString strConnection)
{
  if (strConnection.IsEmpty())
      {
      switch (m_intDBType)
          {
          // -------------------------------------------------------------------
          // MS ACCESS
          //
          // Para bancos de dados com senha no banco, precisa ser Jet OLDB:..."
          // m_strConnection += "Jet OLEDB:Database Password=" + strLoginPwd + ";";
          case SNPDBTYPE_ACCESS : // Testa se o banco existe. Caso n�o exista, retorna false
                                  m_strConnection = getConnStringMSAccess(m_strDBName, m_strLoginName, m_strLoginPwd);
                                  break;

          // -------------------------------------------------------------------
          // ORACLE
          //
          // "Provider=msdaora;";
          // mml@20050815
          // Quando instalei o Oracle 9 no Enerq, parou de funcionar com o msdaora.
          // Em um micro "virgem", havia funcionado OK. N�o sei se � a vers�o antiga...

          // "Provider=OraOLEDB.Oracle;";
          // mml@20051202
          // Com Stored Procedures que retornam cursores n�o funcionavam. Voltei para MSDAORA
          // Em um micro "virgem", havia funcionado OK. N�o sei se � a vers�o antiga...
          case SNPDBTYPE_ORACLE : m_strConnection  = "Provider=MSDAORA.1;";  //
                                  m_strConnection += "Data Source=" + m_strDBName + ";";
                                  m_strConnection += "User Id=" + m_strLoginName + ";";
                                  m_strConnection += "Password=" + m_strLoginPwd + ";";
                                  break;

          // -------------------------------------------------------------------
          // SQLSERVER
          // ATEN��O: Deve ser criada uma "System DSN" (Ferramentas Administrativas -> ODBC)
          // Nessa conex�o criada (cujo nome � passado em m_strDBName), os valores que indicam
          // o servidor, nome da base de dados, etc. s�o configurados
          case SNPDBTYPE_SQLSERVER : m_strConnection  = "Provider=MSDASQL;";  //
                                     m_strConnection += "Data Source=" + m_strDBName + ";";
                                     if (m_strLoginName.IsEmpty())
                                        {
                                        m_strConnection += "Trusted_Connection=yes;";
                                        }
                                     else
                                        {
                                        m_strConnection += "User Id=" + m_strLoginName + ";";
                                        m_strConnection += "Password=" + m_strLoginPwd + ";";
                                        m_strConnection += "Trusted_Connection=no;";
                                        }
                                  break;
          }
      }
  else
      {
      m_strConnection = strConnection;
      }

  return(true);
} // setConnString

//---------------------------------------------------------------------------
bool __fastcall TSNPDBConn::Shut(void)
{
 bool bolabort;
 AnsiString strlog;

  m_bolInitOK = false;
  bolabort = false;
  if (m_ptrADOConnection) {
    if (!m_bolMultiDataSet)
        {
        // Somente elimina se n�o for multidataset (ini)
        if (m_ptrQryDS)
            {
            try
                {
                if (m_ptrQryDS->State != dsInactive)
                    {
                    m_ptrQryDS->Close();
                    }
                delete m_ptrQryDS;
                m_ptrQryDS = NULL;
                }
            catch (Exception &e)
                {
                SNPSTRLOG(e.Message);
                bolabort = true;
                } // catch delete m_ptrADOds;
            } // m_ptrADOds
        // Somente elimina se n�o for multidataset (fim)
        }
    if (bolabort) {
      SNPSTRLOGLINE(strlog)
    }

    if (!bolabort) {
      if (m_ptrADOCmd) {

        try {
          if (m_ptrADOCmd->States != (TObjectStates() << stClosed))
              {
              m_ptrADOCmd->Cancel();
              }

          delete m_ptrADOCmd;
          m_ptrADOCmd = NULL;
        }
        catch (Exception &e) {
          SNPSTRLOG(e.Message);
          bolabort = true;
        }
      }
    } // !bolabort
    if (bolabort) {
      SNPSTRLOGLINE(strlog)
    }

    if (!bolabort) {
      if (m_ptrADOConnection->Connected)
        m_ptrADOConnection->Close();
      try {
        delete m_ptrADOConnection;
        m_ptrADOConnection = NULL;
      }
      catch (Exception &e) {
        SNPSTRLOG(e.Message);
        bolabort = true;
      }
    } // !bolabort
    if(bolabort) {
      SNPSTRLOGLINE(strlog)
    }

  } // m_trpADOConnection

  return(!bolabort);
} // Terminate


bool __fastcall TSNPDBConn::ParseCmd(AnsiString strCommand, TCommandType cmdType)
   {
   if (!m_ptrADOCmd)
      {
      return(false);
      }

   // Coloquei isso aqui somente para garantir
   m_ptrADOCmd->Parameters->Clear();
   
   // Comando a ser executado
   // Olha o help do Borlando
   // When you set the CommandText property, the Parameters property is
   // automatically updated to reflect the parameters in the query or those
   // used by the stored procedure.
   // Ou seja, magicamente, todos par�metros estar�o noemados corretamente...
   m_ptrADOCmd->CommandText = strCommand;

   // mml@20051202
   // Se for definida uma procedure dentro de uma package que retorne um recordset
   // precisa setar o tipo de comando para cmdStoredProc.
   // O par�metro referente ao cursor n�o � colocado na lista de par�metros
   // Testado com o provider
   m_ptrADOCmd->CommandType = cmdType;

   return(true);
   }

TParameter* __fastcall TSNPDBConn::SetParam(AnsiString strName, TFieldType tftDatatype, TParameterDirection tpdDirection, int intSize, Variant varValue)
   {
   TParameter* ptrParam;

   if (!m_ptrADOCmd)
      {
      return(NULL);
      }

   ptrParam = m_ptrADOCmd->Parameters->ParamByName(strName);
   if (ptrParam == NULL)
      {
      return(NULL);
      }

   // Seta os valores
   ptrParam->DataType = tftDatatype;
   ptrParam->Direction = tpdDirection;
   ptrParam->Size = intSize;
   // Somente salva o valor se n�o for Blob. Blob tem que ter a stream carregada posteriormente
   if (ptrParam->DataType != ftBlob)
      {
      ptrParam->Value = varValue;
      }

   return(ptrParam);
   }

//---------------------------------------------------------------------------
// Caso o comando n�o retorne nenhum dado (por exemplo, uma procedure sem select
// no final ou uma consulta a��o no access), bolRetDataSet deve ser falso.
//
// Na vers�o original, utilizava vetor de variants. Agora, tudo mudou...
//
bool __fastcall TSNPDBConn::Execute(bool bolRetDataSet)
{
 bool           bolret;
 AnsiString     straux;
 AnsiString     strlog;
 TFieldType     fldType;
 TParam*        ptrParam;

  // ---------------------------------------------------------------------------
  // Verifica explos�es (begin)
  if (!getConnectedOK())   { SNPSTRLOGLINE(strlog) return(false); }
  if (!m_ptrADOConnection) { SNPSTRLOGLINE(strlog) return(false); }
  if (!m_ptrADOCmd)        { SNPSTRLOGLINE(strlog) return(false); }
  if (!m_bolMultiDataSet)
      {
      if (!m_ptrQryDS)     { SNPSTRLOGLINE(strlog) return(false); }
      }

  // Verifica explos�es (end)
  // ---------------------------------------------------------------------------

  bolret = false;
  if (m_ptrADOConnection->Connected)
      {
      try
          {
          // Caso esteja ativo, fecha o Recordset...
          if (!m_bolMultiDataSet)
              {
              if (m_ptrQryDS)
                m_ptrQryDS->Close();
              }
          else
              {
              if (bolRetDataSet)
                  {
                  try
                      {
                      // Mesmo que j� exista um recordset, aloca um novo.
                      // A responsabilidade de matar o recordset � de quem chamou!
                      m_ptrQryDS = new TADOQuery(NULL);
                      m_ptrQryDS->Connection = m_ptrADOConnection;
                      }
                  catch(Exception &e)
                      {
                      SNPSTRLOGLINE(strlog)
                      SNPSTRLOG(e.Message);
                      bolret = false;
                      assert(bolret);
                      }
                  }
              }

          // Comando a ser executado
          // Olha o help do Borlando
          // When you set the CommandText property, the Parameters property is
          // automatically updated to reflect the parameters in the query or those
          // used by the stored procedure.
          // Ou seja, magicamente, todos par�metros estar�o noemados corretamente...
          //m_ptrADOCmd->CommandText = strCommand;

          // mml@20051202
          // Se for definida uma procedure dentro de uma package que retorne um recordset
          // precisa setar o tipo de comando para cmdStoredProc.
          // O par�metro referente ao cursor n�o � colocado na lista de par�metros
          // Testado com o provider
          //m_ptrADOCmd->CommandType = cmdType;

          // Se tentar copiar para o DataSet sem ter retornado nada, d� erro.
          if (bolRetDataSet)
              {
              // mml@20120116 - DisableControls eu vi em http://edn.embarcadero.com/article/27790
              m_ptrADOCmd->ExecuteOptions = TExecuteOptions() >> eoExecuteNoRecords;
              m_ptrQryDS->DisableControls();

              m_ptrQryDS->CursorType = ctOpenForwardOnly;
              m_ptrQryDS->LockType = ltReadOnly;
              m_ptrQryDS->Recordset = m_ptrADOCmd->Execute();
              }
          else
              {
              // mml@20120116 - DisableControls eu vi em http://edn.embarcadero.com/article/27790
              m_ptrADOCmd->ExecuteOptions = TExecuteOptions() << eoExecuteNoRecords;

              m_ptrADOCmd->Execute();
              }
          bolret = true;
          }
      catch (Exception &e)
          {
          // Pau na execu��o
          SNPSTRLOGLINE(strlog)
          SNPSTRLOG(m_ptrADOCmd->CommandText);
          SNPSTRLOG("Params:");
          for (int i = 0; i < m_ptrADOCmd->Parameters->Count; i++) {
              straux  = m_ptrADOCmd->Parameters->Items[i]->Name;
              straux += " = ";
              straux += m_ptrADOCmd->Parameters->Items[i]->Value;
              SNPSTRLOG(straux);
          }
          SNPSTRLOG(e.Message);
          bolret = false;
          }
      }
  else
      {
      // N�o est� conectado
      SNPSTRLOGLINE(strlog)
      }

  return(bolret);
} // Execute
//eof

