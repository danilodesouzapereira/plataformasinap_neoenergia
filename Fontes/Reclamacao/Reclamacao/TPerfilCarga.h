//---------------------------------------------------------------------------
#ifndef TPerfilCargaH
#define TPerfilCargaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTPerfilCarga.h"

//---------------------------------------------------------------------------
class VTTensao;

//---------------------------------------------------------------------------
class TPerfilCarga  : public VTPerfilCarga
   {
   public:
             __fastcall  TPerfilCarga(VTApl *apl, VTCarga *carga);
             __fastcall ~TPerfilCarga(void);
      bool   __fastcall  AtualizaDemandaDaCarga(TDateTime data);
      bool   __fastcall  SalvaDemanda(TDateTime data, VTMedicao *medicao);
      bool   __fastcall  Sf_mva(int ind_dia, int ind_pat, double sf_mva[MAX_FASE]);
      bool   __fastcall  Sf_mva(int ind_dia, int ind_pat, complex<double> sf_mva[MAX_FASE]);

   private: //métodos
      VTTensao* __fastcall ExisteTensao(TDateTime data);

   private: //objetos externos
      VTApl *apl;

   private: //dados
      TList *lisTENSAO;
   };

//---------------------------------------------------------------------------
#endif
//eof


