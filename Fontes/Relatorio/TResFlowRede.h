//---------------------------------------------------------------------------
#ifndef TResFlowRedeH
#define TResFlowRedeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTResFlowRede.h"

//---------------------------------------------------------------------------
class TResFlowRede : public VTResFlowRede
   {
   public:
                     __fastcall  TResFlowRede(VTApl *apl);
                     __fastcall ~TResFlowRede(void);
      void           __fastcall  Inicia(int np);
      double          __fastcall Pcarga_mw(VTBarra *barra, int np);
      double          __fastcall Pgerador_mw(VTBarra *barra, int np);
      double          __fastcall Psuprimento_mw(VTBarra *barra, int np);
      double          __fastcall Qcapacitor_mvar(VTBarra *barra, int np);
      double          __fastcall Qcarga_mvar(VTBarra *barra, int np);
      double          __fastcall Qgerador_mvar(VTBarra *barra, int np);
      double          __fastcall Qreator_mvar(VTBarra *barra, int np);
      double          __fastcall Qsuprimento_mvar(VTBarra *barra, int np);


   protected: //funções acessadas via property
      double          __fastcall PM_GetPcar_mw(int np);
      double          __fastcall PM_GetPerda_kw(int np);
      double          __fastcall PM_GetPerda_kvar(int np);
      double          __fastcall PM_GetPger_mw(int np);
      double          __fastcall PM_GetPsup_mw(int np);
      double          __fastcall PM_GetQcap_mvar(int np);
      double          __fastcall PM_GetQcar_mvar(int np);
      double          __fastcall PM_GetQger_mvar(int np);
      double          __fastcall PM_GetQrea_mvar(int np);
      double          __fastcall PM_GetQsup_mvar(int np);
      VTRede*         __fastcall PM_GetRede(void);
      complex<double> __fastcall PM_GetScar_mva(int np);
      complex<double> __fastcall PM_GetSger_mva(int np);
      complex<double> __fastcall PM_GetSsup_mva(int np);
      //
      void            __fastcall PM_SetRede(VTRede *rede);

   private: //métodos
      double          __fastcall Qcapacitor_mvar(TList *lisEQP, int np);
      double          __fastcall Qreator_mvar(TList *lisEQP, int np);
      complex<double> __fastcall Scarga_mva(TList *lisEQP, int np);
      complex<double> __fastcall Sgerador_mva(TList *lisEQP, int np);
      complex<double> __fastcall Ssuprimento_mva(TList *lisEQP, int np);

   private: //objetos externos
      VTApl    *apl;
      VTRede   *rede;

   private: //dados locais
      TList *lisEQP;
   };

#endif
//---------------------------------------------------------------------------
//eof

