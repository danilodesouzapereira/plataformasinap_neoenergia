//---------------------------------------------------------------------------
#ifndef TBatchCurvaH
#define TBatchCurvaH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTBatchIG.h"

//---------------------------------------------------------------------------
class VTCurva;
class VTBarra;
class VTRadial;
class VTMedicoes;
class VTSalva;

//---------------------------------------------------------------------------
class TBatchCurva : public VTBatchIG
   {
   public:
           __fastcall  TBatchCurva(VTApl *apl_owner);
           __fastcall ~TBatchCurva(void);
      bool __fastcall  Executa(TListBox *LBoxLog);

   private: //métodos
      AnsiString __fastcall CodigoRedeSupridora(void);
      bool       __fastcall CriaBaseAccessDaRede(AnsiString arq_rede);
      bool       __fastcall ExisteCurva(TList *lisCURVA, VTCurva *curva);
      bool       __fastcall ExcluiCurvas(void);
      bool       __fastcall ImportaCurvas(void);
      bool       __fastcall ImportaRedesArqGIS(TDateTime data, AnsiString filename);
      bool       __fastcall ImportaRedesListaArqGIS(TDateTime data, AnsiString filename);
      bool       __fastcall LeRedesBaseSinap(AnsiString filename);
      void       __fastcall Progresso(AnsiString txt);
      void       __fastcall Progresso(TDateTime data);
      void       __fastcall RedefineId(void);
      void       __fastcall RedefineId(TList *lisEQP);
      void       __fastcall RedefineIdCurvaPropria(TList *lisTMP);
      void       __fastcall SalvaBaseCurvaTipica(VTCurva *curva);
      bool       __fastcall SalvaRedeNaBase(TDateTime data);

   private: //objetos externos
      TListBox *LBoxLog;

   private: //dados locais
      VTApl       *apl;
      VTMedicoes  *medicoes;
      VTRadial    *radial;
      VTSalva     *salva;
      TList       *lisEQP;
      TStrings    *files;
   };

#endif
//-----------------------------------------------------------------------------
// eof

