//---------------------------------------------------------------------------
#ifndef TFlowNetH
#define TFlowNetH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTFlowNet.h"

//---------------------------------------------------------------------------
class VTApl;
class VTDiag;
class VTFlow;
class VTLog;
class VTNet;
class VTRede;
class VTRedes;
class VTReduz;

//---------------------------------------------------------------------------
class TFlowNet : public VTFlowNet
   {
   public:
           __fastcall  TFlowNet(VTApl* apl);
           __fastcall ~TFlowNet(void);
      bool __fastcall  Executa(VTMNet *mnet);
      bool __fastcall  Executa(VTNet *net);

   private: //métodos
      void   __fastcall EliminaRelatorios(void);
      bool   __fastcall ExecutaCalculoNet(VTNet *net , int tipo_fluxo);
      bool   __fastcall ExecutaCalculoRedeDeChaves(void);
      bool   __fastcall ExecutaCalculoRedeUnica(void);
      bool   __fastcall ExecutaCalculoUmaRede(VTRede *rede);
      bool   __fastcall ExisteCargaFasesInvalidas(VTNet *net);
      bool   __fastcall ExisteSuprimentoFasesInvalidas(VTNet *net);
      bool   __fastcall ExisteSuprimentoTensaoValida(VTNet *net);
      bool   __fastcall ExisteZonaInvalida(VTNet *net);
      void   __fastcall ReconfiguraZonaFluxoEquilibrado(VTNet *net);
      void   __fastcall ReiniciaResFluxoEqptoNet(VTNet *net);
      int    __fastcall SelecionaAlgoritmoFluxo(VTNet *net, int tipo_fluxo);

   private: //objetos externos
      VTApl  *apl;
      VTFlow *flow;

   private: //dados locais
      AnsiString         arqRelRede, arqRelFlow;
      bool               aviso_enabled;
      bool               crono_enabled;
      int                tipo_relatorio;
      bool               tramo_enabled;
      double             Sbase;     // Potência de base
      VTDiag             *diag;
      VTLog              *plog;
      VTReduz            *reduz;
      TList              *lisEQP;
      TList              *lisGERSUP;
   };

#endif
//---------------------------------------------------------------------------
//eof

