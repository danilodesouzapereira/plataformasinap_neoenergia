//---------------------------------------------------------------------------
#ifndef TBatchPTH
#define TBatchPTH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTBatchPT.h"
#include <Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>

//---------------------------------------------------------------------------
class VTBarra;
class VTRadial;
class VTMedicoes;
class VTRedes;
class VTResumos;
class VTSalva;
class TArqRede;
class VTImportaRedeGIS;
class TRelRegulador;
//---------------------------------------------------------------------------
class TBatchPT : public VTBatchPT
   {
   public:
           __fastcall  TBatchPT(VTApl *apl_owner);
           __fastcall ~TBatchPT(void);
      bool __fastcall  Executa(void);

   private: //métodos
	  void __fastcall AtualizaResumosComEnergiaOriginalDasRedes(VTResumos *resumos);
	  void __fastcall DefineOpcoes(strOPImporta &opcao, TDateTime data);
      bool __fastcall ExecutaAjusteDemanda(TDateTime data);
	  bool __fastcall ExecutaCalculoPerdaTecnica(TDateTime data);
      void __fastcall ExibeRede(VTRedes *redes);
	  bool __fastcall ExisteGeracao(void);
	  bool __fastcall ExisteMaeSelecionada(VTSub* sub, TList* lisSUB);
	  bool __fastcall ExisteTrafoNaRede(void);
      bool __fastcall ImportaMedicao(TDateTime data);
	  bool __fastcall ImportaRedesArqGIS(TDateTime data, AnsiString filename);
	  bool __fastcall ImportaRedesArqGIS(TDateTime data, TArqRede *arqRede);
	  bool __fastcall ImportaRedesListaArqGIS(TDateTime data, AnsiString filename);
	  bool __fastcall ImportaRedesSubestacaoGIS(TDateTime data, TArqRede *arqRede);
	  bool __fastcall LeRedesBaseSinap(AnsiString filename);
      void __fastcall ProgressoPasso(AnsiString txt, bool clear=false);
      void __fastcall ProgressoRede(TDateTime data, AnsiString filename, int fileindex, int filecount);
      void __fastcall ProgressoRedeResultado(bool sucesso);
      void __fastcall Refresh(void);
      void __fastcall ReiniciaRede(void);
	  void __fastcall SalvaEnergiaOriginalDasRedes(void);
	  //relatorio de energias passante de reguladores (EDAC e EDRO)
	  void __fastcall ExecutaRelReguladores(TDateTime data);
	  void __fastcall SalvaRelReguladores(TDateTime data);

   private: //objetos externos
      TListBox *LBoxLog;

   private: //dados locais
      VTApl       *apl;
      TList       *lisEQP;
	  TStrings    *files;
	  TRelRegulador *relRegulador;

   };

#endif
//-----------------------------------------------------------------------------
// eof
