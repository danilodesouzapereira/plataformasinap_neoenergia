//---------------------------------------------------------------------------
#ifndef TEtH
#define TEtH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEt.h"

//---------------------------------------------------------------------------
class VTCarga;

//---------------------------------------------------------------------------
class TEt : public VTEt
   {
   public:
           __fastcall  TEt(VTApl *apl);
           __fastcall ~TEt(void);
      void __fastcall  MontaPerfilDemandaFinal(TList *lisCARGA, VTBarra *barra_ini, VTBarra *barra_car);
      void __fastcall  MontaPerfilDemandaInicial(TList *lisCARGA);
      void __fastcall  MontaPerfilTensaoSuprimento(VTSuprimento *suprimento, VTBarra *barra_ini, VTBarra *barra_car);

   private: //métodos
      bool   __fastcall CorrigeDemandaCarga(VTCarga* carga, int ind_pat, double *fatcor);
      bool   __fastcall DesequilibraDemandaCarga(VTCarga* carga, int ind_pat, double *deseq);
      double __fastcall EstimaMedicao(double vfn_med_kv[MAX_FASE], int ind_pat, int ind_fase);

   private: //objetos externos
      VTApl   *apl;
      VTBarra *barra_car;
   };

//---------------------------------------------------------------------------
#endif
//eof

