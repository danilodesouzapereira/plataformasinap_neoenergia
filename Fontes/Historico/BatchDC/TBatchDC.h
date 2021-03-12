//---------------------------------------------------------------------------
#ifndef TBatchDCH
#define TBatchDCH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTBatchDC.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTRadial;
class VTMedicoes;
class VTSalva;
class TArqRede;

//---------------------------------------------------------------------------
class TBatchDC : public VTBatchDC
   {
   public:
           __fastcall  TBatchDC(VTApl *apl_owner);
           __fastcall ~TBatchDC(void);
      bool __fastcall  Executa(TListBox *LBoxLog);

   private: //métodos
      AnsiString __fastcall CodigoRedeSupridora(void);
      bool       __fastcall CriaBaseAccessDaRede(AnsiString arq_rede);
	  bool       __fastcall ExecutaAjusteDemanda(TDateTime data);
	  bool       __fastcall ExecutaCalculoDensidadeCarga(TDateTime data);
	  bool       __fastcall ExisteGeracao(void);
	  bool       __fastcall ExisteMaeSelecionada(VTSub* sub, TList* lisSUB);
      bool       __fastcall ExisteTrafoNaRede(void);
      bool       __fastcall ImportaMedicao(TDateTime data);
	  bool       __fastcall ImportaRedesArqGIS(TDateTime data, AnsiString filename);
	  bool       __fastcall ImportaRedesArqGIS(TDateTime data, TArqRede *arqRede);
	  bool       __fastcall ImportaRedesListaArqGIS(TDateTime data, AnsiString filename);
	  bool       __fastcall ImportaRedesSubestacaoGIS(TDateTime data, TArqRede *arqRede);
	  bool       __fastcall LeRedesBaseSinap(AnsiString filename);
	  void       __fastcall Progresso(AnsiString txt);
      void       __fastcall Progresso(TDateTime data);

   private: //objetos externos
      TListBox *LBoxLog;

   private: //dados locais
      VTApl       *apl;
      TList       *lisEQP;
      TStrings    *files;
   };

#endif
//-----------------------------------------------------------------------------
// eof
