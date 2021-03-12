//---------------------------------------------------------------------------
#ifndef TFlowH
#define TFlowH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\VTFlow.h"

//---------------------------------------------------------------------------
class VTDiag;
class VTOrdena;
class VTNet;
class VTLog;
class VTMNet;
class VTReduz;
class VTArvore;
class VTProgresso;

//---------------------------------------------------------------------------
class TFlow : public VTFlow
   {
   public:
           __fastcall  TFlow(VTApl *apl_owner);
           __fastcall ~TFlow(void);
      bool __fastcall  CalculaPorRede(VTRedes *redes, bool dupla_precisao);
      bool __fastcall  CalculaRedeUnica(VTRedes *redes);
      bool __fastcall  CalculaUmaRede(VTRedes *redes, VTRede *rede);

   protected:  //métodos acessados via property
      bool __fastcall PM_GetAvisoEnabled(void);
      bool __fastcall PM_GetCronometroEnabled(void);
      int  __fastcall PM_GetModeloCarga(void);
      bool __fastcall PM_GetMultiThreadEnabled(void);
      bool __fastcall PM_GetPerdaFerroEnabled(void);
      bool __fastcall PM_GetPorRedeEnabled(void);
      bool __fastcall PM_GetProgressoEnabled(void);
      bool __fastcall PM_GetRedeComCarga(void);
      bool __fastcall PM_GetReguladorBypass(void);
      int  __fastcall PM_GetTipoFluxo(void);
      int  __fastcall PM_GetTipoFluxoAlt(void);
      int  __fastcall PM_GetTipoRelatorio(void);
      bool __fastcall PM_GetTramoEnabled(void);
      bool __fastcall PM_GetRelatorioBarDesc(void);
      bool __fastcall PM_GetUpToDate(void);
      //set
      void __fastcall PM_SetAvisoEnabled(bool enabled);
      void __fastcall PM_SetCronometroEnabled(bool enabled);
      void __fastcall PM_SetModeloCarga(int modelo_carga);
      void __fastcall PM_SetMultiThreadEnabled(bool enabled);
      void __fastcall PM_SetPerdaFerroEnabled(bool enabled);
      void __fastcall PM_SetPorRedeEnabled(bool enabled);
      void __fastcall PM_SetProgressoEnabled(bool enabled);
      void __fastcall PM_SetRedeComCarga(bool rede_com_carga);
      void __fastcall PM_SetReguladorBypass(bool enabled);
      void __fastcall PM_SetTipoFluxo(int tipo_fluxo);
      void __fastcall PM_SetTipoFluxoAlt(int tipo_fluxo);
      void __fastcall PM_SetTipoRelatorio(int tipo_rel);
      void __fastcall PM_SetTramoEnabled(bool enabled);
      void __fastcall PM_SetRelatorioBarDesc(bool enabled);
      void __fastcall PM_SetUpToDate(bool uptodate);

   private: //métodos
      bool   __fastcall AlgoritmoEquilibrado(void);
      //void   __fastcall DebugRedesMNet(VTMNet *mnet);
      bool   __fastcall ExecutaCalculoMNet(VTMNet *mnet);
      bool   __fastcall ExecutaCalculoPorRedeComThread(bool dupla_precisao);
      bool   __fastcall ExecutaCalculoPorRedeSemThread(bool dupla_precisao);
      bool   __fastcall ExecutaCalculoRedeUnica(void);
      bool   __fastcall ExecutaCalculoReducaoFicticia(void);
      void   __fastcall GeraMsgAlerta(VTMNet *mnet);
      bool   __fastcall IniciaLisNet(TList *lisNET, TList *lisMNET);
      void   __fastcall LogNetCalculada(TList *lisMNET);
      //void   __fastcall LogNetIniciada(VTMNet  *mnet);
      bool   __fastcall MontaArvoreMNet(TList *lisMNET);
      int    __fastcall SelecionaMNetCargaComTodasMNetFilhasCalculadas(TList *lisMNET_ORIG, TList *lisMNET_DEST);
      int    __fastcall SelecionaMNetCargaSemMNetFilha(TList *lisMNET_ORIG, TList *lisMNET_DEST);
      int    __fastcall SelecionaMNetGeracao(TList *lisMNET_ORIG, TList *lisMNET_DEST);
      int    __fastcall SelecionaMNetFilhas(TList *lisMNET_ORIG, TList *lisMNET_DEST);
      int    __fastcall SelecionaMNetFilhas(VTMNet *mnet, TList *lisMNET_DEST);
      int    __fastcall SubsituiMNetFilhaPorMNetPai(TList *lisMNET);
      void   __fastcall TipoFluxoDefault(void);
      void   __fastcall TrataMNetComErroNoFluxo(TList *lisMNET, bool excluir);
      void   __fastcall TrataMNetComErroNoFluxo(VTMNet *mnet);
      void   __fastcall VerificaAlteracaoFluxoRedeNaoRedutivel(void);

   private: //dados externos
      VTRedes    *redes;

   protected:  //dados acessados via property
      struct   {
               bool uptodate;
               bool rede_com_carga;
               int  modelo_carga;
               int  algoritmo_fluxo;
               int  algoritmo_fluxo_alt;
               int  tipo_relatorio;
               bool aviso_enabled;
               bool crono_enabled;
               bool tramo_enabled;
               bool threads_enabled;
               bool perda_ferro_enabled;
               bool por_rede_enabled;
               bool progresso_enabled;
               bool bypass_enabled;
               bool relatorio_bar_desc_enabled;
               }PD;

   private: //dados locais
      double            Sbase;
      VTApl             *apl;
      VTArvore          *arvore;
      VTDiag            *diag;
      VTOrdena          *ordena;
      VTReduz           *reduz;
      VTLog             *plog;
      VTLog             *plogthr;
      VTProgresso       *progresso;
      TList             *lisEQP;
      TList             *lisNET;
      TList             *lisMNET_SEL;
      TList             *lisMNET_CAL;
      };

#endif
//---------------------------------------------------------------------------
//eof

