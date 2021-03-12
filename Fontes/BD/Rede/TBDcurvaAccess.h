//---------------------------------------------------------------------------

#ifndef TBDcurvaAccessH
#define TBDcurvaAccessH
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ADODB.hpp>
#include "BD_inc.h"

//---------------------------------------------------------------------------
class TBDfunc;
class VTApl;
class VTCurva;
class VTEqpto;
class VTEstudo;
class VTLog;
class VTMonta;
class VTSNPDBConn;

//---------------------------------------------------------------------------
// Estrutura auxiliar para geração dos identificadores sem consultar a base de dados
struct sttIndexCurva
         {
         int ndxConjCurvaId;
         //int ndxCurvaId;
         int ndxSegmentoId;
         };

//---------------------------------------------------------------------------
// Essa classe foi utilizada para unificar as chamadas de métodos referentes
// à manipulação de curvas pelo Access.
// Essa classe não existe para o Oracle
class TBDcurvaAccess : public TObject
   {
   public:
                 __fastcall  TBDcurvaAccess(VTApl *apl_owner, VTSNPDBConn *bd_conn);
                 __fastcall ~TBDcurvaAccess(void);
      VTCurva*   __fastcall CurvaEqpto(VTEqpto* eqpto);
      int        __fastcall FNSNP_rdConjCurvaIdProx(bool bolLocaIndex = true, bool bolBeginNext = false);
      //int        __fastcall FNSNP_rdConjCurva_CurvaIdProx(int intConjCurvaId, bool bolLocaIndex = true, bool bolBeginNext = false);
      int        __fastcall FNSNP_rdConjCurva_SegIdProx(int intConjCurvaId, int intCurvaId, bool bolLocaIndex = true, bool bolBeginNext = false);
      bool       __fastcall SPSNP_wrConjCurva(int& intConjCurvaId, AnsiString strDescricao, bool bolTipica, int intStatus);
      bool       __fastcall SPSNP_wrConjCurva(int intTipoEquipId, VTCurva *curva);
      bool       __fastcall SPSNP_wrConjCurvaEquipamento(VTEstudo* estudo, VTEqpto* eqpto, VTCurva* curva);
      bool       __fastcall SPSNP_wrConjCurvaSegmento(int intCurvaId, int intSegmentoId, int intTipoEquipId, VTCurva *curva);
      bool       __fastcall SPSNP_wrConjCurvaSegmento(int intConjCurvaId, int intTipoCurvaId, int intCurvaId, int intSegmentoId, TStream* ptrStream);
      bool       __fastcall SPSNP_wrConjCurvaTipica(int intTipoEquipId, VTCurva *curva);
      bool       __fastcall RenovaStatusCurva(TList *lisCurva);

   private:  //objetos externos
      VTApl                *apl;
      VTSNPDBConn          *bd_conn;
      TBDfunc              *bd_func;     // Classe para manipulação de dados
      VTLog                *plog;        // Ponteiro para objeto de log
      struct sttIndexCurva index;        // Indexação local dos identificadores
   };



#endif
