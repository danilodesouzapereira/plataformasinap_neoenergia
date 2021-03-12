#ifndef TSNPDBConnH
#define TSNPDBConnH

//---------------------------------------------------------------------------
class VTApl;
class VTLog;

//---------------------------------------------------------------------------
#include "VTSNPDBConn.h"

#define NDEBUG  // esta linha desabilita os asserts...
#include <assert.h>

#include "SNPGlobal.h" // Criar um global.h, contendo, no mínimo o include abaixo.
                        // #include "..\..\GenFunc\GlobalGenFunc.h"
                        // Serve para substituir macros default, se necessário

//---------------------------------------------------------------------------
class TSNPDBConn : public VTSNPDBConn
{
private:
   VTApl* apl;
   VTLog* plog;

public:  // User declarations
  __fastcall TSNPDBConn(VTApl* apl, AnsiString strDBName, AnsiString strLoginName, AnsiString strLoginPwd, int intDBType, bool bolMultiDataSet);
  __fastcall ~TSNPDBConn(void);
  virtual bool        __fastcall Init(void);
  virtual bool        __fastcall Shut(void);
  virtual bool        __fastcall ParseCmd(AnsiString strCommand, Adodb::TCommandType cmdType = cmdText);
  virtual TParameter* __fastcall SetParam(AnsiString strName, TFieldType tftDatatype, TParameterDirection tpdDirection, int intSize, Variant varValue);
  virtual bool        __fastcall Execute(bool bolRetDataSet = false);
  virtual bool        __fastcall FindTable(AnsiString strTableName, bool bolSystemTables = false);

  // Monta a string de conexão do MS Access
  static AnsiString __fastcall getConnStringMSAccess(AnsiString strDBName, AnsiString strLoginName, AnsiString strLoginPwd);

  // get
  bool __fastcall getConnectedOK(bool bolRetry = true);
  bool __fastcall getInitOK(void)                { return(m_bolInitOK);                                                 };
  bool __fastcall getMultiDataSet(void)          { return(m_bolMultiDataSet);                                           };
  TADOQuery*   __fastcall getDataSet(void)       { return(m_ptrQryDS);                                                  };
  AnsiString   __fastcall getConnString(void)    { return(m_strConnection);                                             };
  AnsiString   __fastcall getLoginName(void)     { return(m_strLoginName);                                              };
  AnsiString   __fastcall getLoginPwd(void)      { return(m_strLoginPwd);                                               };
  AnsiString   __fastcall getDBName(void)        { return(m_strDBName);                                                 };

  // set
  bool __fastcall setDBName(AnsiString strDBName);
  bool __fastcall setConnString(AnsiString strConnection = "");  // Se não passar nada, constroi a string
  bool __fastcall setLoginName(AnsiString strLoginName)   { m_strLoginName = strLoginName; return(true);   };
  bool __fastcall setLoginPwd(AnsiString strLoginPwd)     { m_strLoginPwd = strLoginPwd;   return(true);   };
};
//---------------------------------------------------------------------------
#endif

