//---------------------------------------------------------------------------
#ifndef TPerfilSupH
#define TPerfilSupH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTPerfilSup.h"

//---------------------------------------------------------------------------
class VTTensao;

//---------------------------------------------------------------------------
class TPerfilSup  : public VTPerfilSup
   {
   public:
           __fastcall  TPerfilSup(VTApl *apl, VTSuprimento *suprimento);
           __fastcall ~TPerfilSup(void);
      bool __fastcall  AtualizaTensaoSuprimento(TDateTime data);
      bool __fastcall  SalvaTensao(TDateTime data, VTMedicao *medicao);
      bool __fastcall  Vfn_pu(int ind_dia, int ind_pat, double v_pu[MAX_FASE]);
      bool __fastcall  Vfn_pu(int ind_dia, int ind_pat, complex<double> v_pu[MAX_FASE]);

   private: //métodos
      VTTensao* __fastcall ExisteTensao(TDateTime data);

   private: //objetos externos
      VTApl        *apl;

   private: //dados locais
      TList *lisTENSAO;
   };

//---------------------------------------------------------------------------
#endif
//eof


