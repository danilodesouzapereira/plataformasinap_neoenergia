//---------------------------------------------------------------------------
#ifndef TBDhistOracleH
#define TBDhistOracleH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ADODB.hpp>
#include <Oracle.hpp>
#include "VTBDhist.h"
#include "BD_inc.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTLog;
class VTResumo;
class VTRede;
class VTBalanco;
class VTPerda;

//---------------------------------------------------------------------------
class TBDhistOracle : public VTBDhist
   {
   public:
           __fastcall  TBDhistOracle(VTApl *apl_owner, TOracleSession* ptrOracleSession);
           __fastcall ~TBDhistOracle(void);
		bool __fastcall  ArvoreContemNo(VTArvore* arvore, AnsiString codigo);
		bool __fastcall  InsereRede(TList* lisPRI);
      bool __fastcall  InsereArvore(VTArvore* arvore);
      bool __fastcall  InsereDensidadeCarga(VTArvore* arvore);
      bool __fastcall  InserePerdaTecnica(VTArvore* arvore);
      bool __fastcall  LeListaArvore(TList* lstArvore);
      bool __fastcall  LeArvoreDensidadeCarga(VTArvore* arvore, int intLevel = -1);
      bool __fastcall  LeArvorePerdaTecnica(VTArvore* arvore, int intLevel = -1);

   private:  //objetos externos
      VTApl          *apl;
      TOracleSession *bd_session;
      VTLog          *plog;

   private:  //métodos locais
   };

#endif
//---------------------------------------------------------------------------
//eof