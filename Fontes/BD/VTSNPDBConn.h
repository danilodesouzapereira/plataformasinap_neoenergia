//---------------------------------------------------------------------------
//
// TSNPDBConn.h
//
// Classe para acesso à bases de dados.
//
//   Data   | Autor | Versão | Descrição
// ==========================================================================
// 20080610 |  MML  |  0.00  | Versão inicial.
//          |       |          Baseada na TGGCDBConn de 20050510. Última revi-
//          |       |          são em 20040518.
// 20080625 |  MML  |  0.01  | Correção na passagem dos parâmetros. Agora
//                           | a ordem considerada é a da string de chamada.
//---------------------------------------------------------------------------
#ifndef VTSNPDBConnH
#define VTSNPDBConnH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ADODB.hpp>
#include <DB.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
// Constantes

// Declaração de parâmetros
#define PARAMIN_INT(var)   ftInteger , pdInput, sizeof(int)  , Variant(var)
#define PARAMIN_STR(var)   ftString  , pdInput, sizeof(int)  , Variant(var)
#define PARAMIN_FLT(var)   ftFloat   , pdInput, sizeof(float), Variant(var)
#define PARAMIN_DTT(var)   ftDateTime, pdInput, sizeof(int)  , Variant(var)
#define PARAMIN_BLB(var)   ftBlob    , pdInput, sizeof(int)  , Variant(var)

// Acesso aos bancos por tipo de banco
#define SNPDBTYPE_INDEFINIDO      -1
#define SNPDBTYPE_ORACLE           1
#define SNPDBTYPE_ACCESS           2
#define SNPDBTYPE_SQLSERVER        3

//---------------------------------------------------------------------------
class VTSNPDBConn : public TObject
   {
   protected:  // User declarations
      // Atributos da classe
      AnsiString      m_strDBName;          // Nome do Banco
      AnsiString      m_strLoginName;       // Nome do usuário
      AnsiString      m_strLoginPwd;        // Senha
      AnsiString      m_strConnection;      // String de conexão: Provider, etc.
      int             m_intDBType;          // Tipo de banco de dados: DBTYPE_ORACLE, DBTYPE_ACCESS
      bool            m_bolInitOK;          // Indica se iniciação foi OK
      TADOConnection* m_ptrADOConnection;   // Conexão com o banco
      TADOCommand*    m_ptrADOCmd;          // Comando a ser executado
      TADOQuery*      m_ptrQryDS;           // Ponteiro para o recordset de retorno
      bool            m_bolMultiDataSet;    // Indica se haverá somente um dataset para todas execuções
                                            // ou um dataset para cada execução, eliminados pela classe.


   public:  // User declarations
      __fastcall VTSNPDBConn(void) {};
      __fastcall ~VTSNPDBConn(void) {};
      virtual bool        __fastcall Init(void) = 0;
      virtual bool        __fastcall Shut(void) = 0;
      virtual bool        __fastcall ParseCmd(AnsiString strCommand, Adodb::TCommandType cmdType = cmdText) = 0;
      virtual TParameter* __fastcall SetParam(AnsiString strName, TFieldType tftDatatype, TParameterDirection tpdDirection, int intSize, Variant varValue) = 0;
      virtual bool        __fastcall Execute(bool bolRetDataSet = false) = 0;
      virtual bool        __fastcall FindTable(AnsiString strTableName, bool bolSystemTables = false) = 0;

      // get
      virtual bool __fastcall getConnectedOK(bool bolRetry = true) = 0;
      virtual bool __fastcall getInitOK(void) = 0;
      virtual bool         __fastcall getMultiDataSet(void) = 0;
      virtual TADOQuery*   __fastcall getDataSet(void) = 0;
      virtual AnsiString   __fastcall getConnString(void) = 0;
      virtual AnsiString   __fastcall getLoginName(void) = 0;
	  virtual AnsiString   __fastcall getLoginPwd(void) = 0;
	  virtual AnsiString   __fastcall getDBName(void) = 0;

      // set
      virtual bool __fastcall setDBName(AnsiString strDBName) = 0;
      virtual bool __fastcall setConnString(AnsiString strConnection = "") = 0;
      virtual bool __fastcall setLoginName(AnsiString strLoginName) = 0;
      virtual bool __fastcall setLoginPwd(AnsiString strLoginPwd) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTSNPDBConn* __fastcall NewObjSNPDBConn(VTApl* apl, AnsiString strDBName, AnsiString strLoginName, AnsiString strLoginPwd, int intDBType, bool bolMultiDataSet);

// função global p/ compactação do banco de dados Access
// Exemplo de chamada:
//    VTPath       *path  = (VTPath*)apl->GetObject(__classid(VTPath));
//    CompactDatabase(path->ArqRede(), "", "ultraplan2006");
bool __fastcall CompactDatabase(AnsiString strDBName, AnsiString strLoginName, AnsiString strLoginPwd);
#endif
//---------------------------------------------------------------------------
//eof
