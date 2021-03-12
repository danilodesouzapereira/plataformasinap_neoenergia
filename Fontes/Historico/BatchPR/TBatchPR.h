//---------------------------------------------------------------------------
#ifndef TBatchPRH
#define TBatchPRH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTBatchPR.h"
#include <Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>

//---------------------------------------------------------------------------
class VTRadial;
class VTSalva;
class VTRedes;
class TArqMedicao;
class TArqRede;

//---------------------------------------------------------------------------
class TBatchPR : public VTBatchPR
   {
   public:
           __fastcall  TBatchPR(VTApl *apl_owner);
           __fastcall ~TBatchPR(void);
      bool __fastcall  Executa(void);

   private: //métodos
	  void         __fastcall DefineOpcoes(strOPImporta &opcao, TDateTime data);
	  bool         __fastcall ExecutaCalculoPerdaRegulatoria(TDateTime data);
      void         __fastcall ExibeRede(VTRedes *redes);
      bool         __fastcall ExisteTrafoNaRede(void);
      TArqMedicao* __fastcall ExisteArquivoMedicao(TDateTime data);
      void         __fastcall GraficoExibeRede(void);
	  bool         __fastcall ImportaRedesArqGIS(TDateTime data, AnsiString filename);
	  bool         __fastcall ImportaRedesArqGIS(TDateTime data, TArqRede *arqRede);
	  bool         __fastcall ImportaRedesListaArqGIS(TDateTime data, AnsiString filename);
	  bool         __fastcall ImportaRedesSubestacaoGIS(TDateTime data, TArqRede *arqRede);
      bool         __fastcall LeRedesBaseSinap(AnsiString filename);
      bool         __fastcall MedicaoMenorEinj(void);
      void         __fastcall ProgressoPasso(AnsiString txt, bool clear=false);
      void         __fastcall ProgressoRede(TDateTime data, AnsiString filename, int fileindex, int filecount);
      void         __fastcall ProgressoRedeResultado(bool sucesso);
      void         __fastcall Refresh(void);
      void         __fastcall ReiniciaRede(void);

   private: //objetos externos
      TListBox *LBoxLog;

   private: //dados locais
      VTApl       *apl;
		VTRadial    *radial;
		VTSalva     *salva;
      TList       *lisEQP;
      TStrings    *files;
   };

#endif
//-----------------------------------------------------------------------------
// eof
