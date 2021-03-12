//---------------------------------------------------------------------------
#ifndef VTPerfilCargaH
#define VTPerfilCargaH

//---------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <complex>
#include <Fontes\Constante\Fases.h>

//---------------------------------------------------------------------------
class VTApl;
class VTCarga;
class VTMedicao;
class VTRes505;

//---------------------------------------------------------------------------
class VTPerfilCarga : public TObject
   {
   public:  //property
      __property VTCarga* Carga = {read=PD.carga};

   public:
                     __fastcall  VTPerfilCarga(void) {};
                     __fastcall ~VTPerfilCarga(void) {};
      virtual bool   __fastcall  AtualizaDemandaDaCarga(TDateTime data) = 0;
      virtual bool   __fastcall  SalvaDemanda(TDateTime data, VTMedicao *medicao) = 0;
      virtual bool   __fastcall  Sf_mva(int ind_dia, int ind_pat, double sf_mva[MAX_FASE]) = 0;
      virtual bool   __fastcall  Sf_mva(int ind_dia, int ind_pat, complex<double> sf_mva[MAX_FASE]) = 0;

   protected:  //dados acessados vias property
      struct   {
               VTCarga *carga;
               }PD;
   };

//---------------------------------------------------------------------------
VTPerfilCarga* __fastcall NewObjPerfilCarga(VTApl *apl, VTCarga *carga);

//---------------------------------------------------------------------------
#endif
//eof


