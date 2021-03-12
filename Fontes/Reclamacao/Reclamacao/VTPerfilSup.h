//---------------------------------------------------------------------------
#ifndef VTPerfilSupH
#define VTPerfilSupH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <complex>
#include <Fontes\Constante\Fases.h>

//---------------------------------------------------------------------------
class VTApl;
class VTMedicao;
class VTSuprimento;

//---------------------------------------------------------------------------
class VTPerfilSup : public TObject
   {
   public:  //property
      __property VTSuprimento*  Suprimento = {read=PD.suprimento  };

   public:
                   __fastcall  VTPerfilSup(void) {};
                   __fastcall ~VTPerfilSup(void) {};
      virtual bool __fastcall  AtualizaTensaoSuprimento(TDateTime data) = 0;
      virtual bool __fastcall  SalvaTensao(TDateTime data, VTMedicao *medicao) = 0;
      virtual bool __fastcall  Vfn_pu(int ind_dia, int ind_pat, double v_pu[MAX_FASE]) = 0;
      virtual bool __fastcall  Vfn_pu(int ind_dia, int ind_pat, complex<double> v_pu[MAX_FASE]) = 0;

   protected:  //dados acessados vias property
      struct   {
               VTSuprimento *suprimento;
               }PD;
   };
                                      
//---------------------------------------------------------------------------
VTPerfilSup*  __fastcall NewObjPerfilSup(VTApl *apl, VTSuprimento *suprimento);

//---------------------------------------------------------------------------
#endif
//eof


