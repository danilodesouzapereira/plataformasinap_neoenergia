//---------------------------------------------------------------------------
#ifndef TBatchIGH
#define TBatchIGH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTBatchIG.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTRadial;
class VTMedicoes;
class VTMercado;
class VTSalva;
class TArqRede;

//---------------------------------------------------------------------------
class TBatchIG : public VTBatchIG
   {
   public:
           __fastcall  TBatchIG(VTApl *apl_owner);
           __fastcall ~TBatchIG(void);
      bool __fastcall  Executa(TListBox *LBoxLog);

   private: //métodos
      AnsiString __fastcall CodigoRedeSupridora(void);
      bool       __fastcall CriaBaseAccessDaRede(AnsiString arq_rede);
      bool       __fastcall DefineDiretorioRede(AnsiString &dir_rede);
      bool       __fastcall ExecutaAjusteDemanda(TDateTime data);
	  bool       __fastcall ExisteGeracao(void);
      bool       __fastcall ExisteMaeSelecionada(VTSub* sub, TList* lisSUB);
      bool       __fastcall ExisteTrafoNaRede(void);
      bool       __fastcall ImportaMedicao(TDateTime data);
	  bool       __fastcall ImportaRedesArqGIS(TDateTime data, TArqRede *arqRede);
	  bool       __fastcall ImportaRedesArqGIS(TDateTime data, AnsiString filename);
	  bool       __fastcall ImportaRedesListaArqGIS(TDateTime data, AnsiString filename);
	  bool       __fastcall ImportaRedesSubestacaoGIS(TDateTime data, TArqRede *arqRede);
      bool       __fastcall LeRedesBaseSinap(AnsiString filename);
      bool       __fastcall MercadoGlobalAtualiza(void);
      void       __fastcall MercadoGlobalAtualizaClasse(void);
	  bool       __fastcall MercadoGlobalSalvaNaBase(void);
	  void       __fastcall MontaNomeBase(TList *lisSub);
	  void       __fastcall Progresso(AnsiString txt);
      void       __fastcall Progresso(TDateTime data);
      void       __fastcall RedefineId(void);
      void       __fastcall RedefineId(TList *lisEQP);
      void       __fastcall RedefineIdCurvaPropria(TList *lisTMP);
      bool       __fastcall SalvaRedeNaBase(TDateTime data);

   private: //objetos externos
      TListBox *LBoxLog;

   private: //dados locais
      AnsiString  arq_rede, nome_base;
      bool        usuario_confirmou_base_unica;
      VTApl       *apl;
      VTMercado   *mercado_global;
      TList       *lisEQP;
      TStrings    *files;
   };

#endif
//-----------------------------------------------------------------------------
// eof
