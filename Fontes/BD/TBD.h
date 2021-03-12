//---------------------------------------------------------------------------
#ifndef TBDH
#define TBDH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Oracle.hpp>
#include "VTBD.h"

//---------------------------------------------------------------------------
class VTSNPDBConn;
class VTPath;
class VTBDFigura;
//---------------------------------------------------------------------------
class TBD : public VTBD
   {
   public:
           __fastcall  TBD(VTApl *apl_owner);
           __fastcall ~TBD(void);
      bool __fastcall  CommitHist(void);
      bool __fastcall  CommitRede(void);
      bool __fastcall  Conecta(int intTipoConexao = SNPDBTYPE_INDEFINIDO);
      bool __fastcall  ConectaBaseHist(int intTipoConexao = SNPDBTYPE_INDEFINIDO);
      bool __fastcall  ConectaBaseRede(int intTipoConexao = SNPDBTYPE_INDEFINIDO);
      bool __fastcall  ConectaBaseHistAccess(AnsiString arq_hist);
      bool __fastcall  ConectaBaseHistOracle(AnsiString TNS, AnsiString user_name, AnsiString password);
      bool __fastcall  ConectaBaseHistSqlServer(AnsiString TNS, AnsiString user_name, AnsiString password);
      bool __fastcall  ConectaBaseRedeAccess(AnsiString arq_rede);
      bool __fastcall  ConectaBaseRedeOracle(AnsiString TNS, AnsiString user_name, AnsiString password);
      bool __fastcall  ConectaBaseRedeSqlServer(AnsiString TNS, AnsiString user_name, AnsiString password);
      bool __fastcall  ConectaServidorHistLocal(int intTipoConexao = SNPDBTYPE_INDEFINIDO);
      bool __fastcall  ConectaServidorHistRemoto(int intTipoConexao = SNPDBTYPE_INDEFINIDO);
      bool __fastcall  ConectaServidorRedeLocal(int intTipoConexao = SNPDBTYPE_INDEFINIDO);
      bool __fastcall  ConectaServidorRedeRemoto(int intTipoConexao = SNPDBTYPE_INDEFINIDO);
      bool __fastcall  Desconecta(void);
      bool __fastcall  DesconectaHist(void);
      bool __fastcall  DesconectaRede(void);
      bool __fastcall  RollbackHist(void);
	  bool __fastcall  RollbackRede(void);

   protected:  //métodos acessados via property
      bool __fastcall  PM_GetConectado(void);
      bool __fastcall  PM_GetBaseRemota(void);
	  bool __fastcall  PM_SetBaseRemota(bool bolFlagRemoto);

   private:
	  VTBDfigura* __fastcall CriaBDFigura(AnsiString versaoBase,VTApl *apl_owner, VTSNPDBConn *bd_conn);

   private:  //objetos externos
//      VTApl  *apl;
      VTPath *path;

   private:  //objetos locais
      TOracleSession *oracleSessionHist;
      TOracleSession *oracleSessionRede;
      //VTSNPDBConn    *accessConnGeral;
      VTSNPDBConn    *accessConnRede;
      VTSNPDBConn    *accessConnHist;

   private:  //métodos
      VTSNPDBConn*    __fastcall IniciaConexaoAccess(AnsiString arq_mdb);
      TOracleSession* __fastcall IniciaConexaoOracle(AnsiString strTNS, AnsiString strLoginName, AnsiString strLoginPwd);

   private:  //dados locais

   };

#endif
//---------------------------------------------------------------------------
//eof

