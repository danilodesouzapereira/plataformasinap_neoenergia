//---------------------------------------------------------------------------
#ifndef VTBDH
#define VTBDH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTSNPDBConn.h"

//---------------------------------------------------------------------------
class VTApl;
class VTBDgeral;
class VTBDhist;
class VTBDeqpto;
class VTBDfigura;

//---------------------------------------------------------------------------
class VTBD : public TObject
   {
   public:  //property
	  __property VTApl*		  AplBD             = { read =apl                                    };
      __property AnsiString   Revisao           = { read =revisao                                 };
      __property bool         BaseRemota        = { read=PM_GetBaseRemota, write=PM_SetBaseRemota };
      __property bool         Conectado         = { read=PM_GetConectado                          };
      __property bool         CronometroEnabled = { write=cronometro_enabled                      };
      __property bool         ProgressoEnabled  = { write=progresso_enabled                       };
      __property VTBDgeral*   Geral             = { read =bd_geral                                };
      __property VTBDeqpto*   Eqpto             = { read =bd_eqpto                                };
      __property VTBDhist*    Hist              = { read =bd_hist                                 };
	  __property VTBDfigura*  Figura            = { read =bd_figura                               };

   public:
                   __fastcall  VTBD(void) {};
      virtual      __fastcall ~VTBD(void) {};
      virtual bool __fastcall  CommitHist(void) = 0;
      virtual bool __fastcall  CommitRede(void) = 0;
      virtual bool __fastcall  Conecta(int intTipoConexao = SNPDBTYPE_INDEFINIDO) = 0;
	  virtual bool __fastcall  ConectaBaseHist(int intTipoConexao = SNPDBTYPE_INDEFINIDO) = 0;
      virtual bool __fastcall  ConectaBaseRede(int intTipoConexao = SNPDBTYPE_INDEFINIDO) = 0;
      virtual bool __fastcall  ConectaBaseHistAccess(AnsiString arq_hist) = 0;
      virtual bool __fastcall  ConectaBaseHistOracle(AnsiString tns, AnsiString user_name, AnsiString password) = 0;
      virtual bool __fastcall  ConectaBaseHistSqlServer(AnsiString tns, AnsiString user_name, AnsiString password) = 0;
      virtual bool __fastcall  ConectaBaseRedeAccess(AnsiString arq_rede) = 0;
      virtual bool __fastcall  ConectaBaseRedeOracle(AnsiString tns, AnsiString user_name, AnsiString password) = 0;
      virtual bool __fastcall  ConectaBaseRedeSqlServer(AnsiString tns, AnsiString user_name, AnsiString password) = 0;
      virtual bool __fastcall  ConectaServidorHistLocal(int intTipoConexao = SNPDBTYPE_INDEFINIDO) = 0;
      virtual bool __fastcall  ConectaServidorHistRemoto(int intTipoConexao = SNPDBTYPE_INDEFINIDO) = 0;
      virtual bool __fastcall  ConectaServidorRedeLocal(int intTipoConexao = SNPDBTYPE_INDEFINIDO) = 0;
      virtual bool __fastcall  ConectaServidorRedeRemoto(int intTipoConexao = SNPDBTYPE_INDEFINIDO) = 0;
      virtual bool __fastcall  Desconecta(void) = 0;
      virtual bool __fastcall  DesconectaHist(void) = 0;
      virtual bool __fastcall  DesconectaRede(void) = 0;
      virtual bool __fastcall  RollbackHist(void) = 0;
      virtual bool __fastcall  RollbackRede(void) = 0;

   protected:  //métodos acessados via property
      virtual bool __fastcall  PM_GetConectado(void) = 0;
      virtual bool __fastcall  PM_GetBaseRemota(void) = 0;
      virtual bool __fastcall  PM_SetBaseRemota(bool bolFlagRemoto) = 0;

   protected:  //dados acessados via property
      AnsiString   revisao;
      bool         cronometro_enabled;
      bool         progresso_enabled;
      bool         base_remota;
      VTBDgeral   *bd_geral;
      VTBDeqpto   *bd_eqpto;
      VTBDhist    *bd_hist;
	  VTBDfigura  *bd_figura;
	  VTApl  	  *apl;
   };

//---------------------------------------------------------------------------
//função global
//---------------------------------------------------------------------------
VTBD* NewObjBD(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof