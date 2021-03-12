//---------------------------------------------------------------------------

#ifndef TBDhistAccessH
#define TBDhistAccessH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ADODB.hpp>
#include "VTBDhist.h"
#include "BD_inc.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTCarga;
class VTCurva;
class VTLog;
class VTResumo;
class VTRede;
class VTBalanco;
class VTPath;
class VTPerda;
struct strCURVA;

// Estrutura auxiliar para geração dos identificadores sem consultar a base de dados
struct sttIndexHist
         {
         int ndxArvoreId;
         int ndxNoId;
         int ndxRedeId;
         };

//---------------------------------------------------------------------------
class TBDhistAccess : public VTBDhist
   {
   public:
           __fastcall  TBDhistAccess(VTApl *apl_owner, VTSNPDBConn *bd_conn);
			  __fastcall ~TBDhistAccess(void);
		bool __fastcall  ArvoreContemNo(VTArvore* arvore, AnsiString codigo);
      bool __fastcall  InsereArvore(VTArvore* arvore);
      bool __fastcall  InsereDensidadeCarga(VTArvore* arvore);
      bool __fastcall  InserePerdaTecnica(VTArvore* arvore);
      bool __fastcall  InsereRede(TList* lisRede);
      bool __fastcall  LeArvoreDensidadeCarga(VTArvore* arvore, int intLevel = -1);
      bool __fastcall  LeArvorePerdaTecnica(VTArvore* arvore, int intLevel = -1);
      bool __fastcall  LeListaArvore(TList* lstArvore);

   private:
      //objetos externos
      VTApl               *apl;
      VTSNPDBConn         *bd_conn;
      VTLog               *plog;
      VTPath              *path;
      //objetos locais
      struct sttIndexHist index;
      TStringList         *lstStrData;  // Dados a serem salvos em modo batch


   private:  //métodos locais
      VTArvore* __fastcall ExisteArvoreNaBase(int tipo_arvore, TDateTime data);
      VTBarra*  __fastcall ExisteBarra(VTRede *rede, int utm_x, int utm_y);
      VTRede*   __fastcall ExisteRede(int rede_id, int tiporede_id);
      VTCurva*  __fastcall InsereCurvaDemanda(VTCarga *carga, strCURVA &str);
      int       __fastcall FNSNP_rdArvoreIdProx(bool bolLocaIndex = true, bool bolBeginNext = false);
      int       __fastcall FNSNP_rdNoIdProx(bool bolLocaIndex = true, bool bolBeginNext = false);
      int       __fastcall FNSNP_rdRedeIdProx(bool bolLocaIndex = true, bool bolBeginNext = false);
      bool      __fastcall SPSNP_wrArvore(VTArvore* arvore);
      bool      __fastcall SPSNP_wrRedeCarga(TList* lisResumoCarga);

   };

#endif
//---------------------------------------------------------------------------
//eof