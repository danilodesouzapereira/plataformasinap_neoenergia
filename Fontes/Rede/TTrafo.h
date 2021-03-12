//---------------------------------------------------------------------------
#ifndef TTrafoH
#define TTrafoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTTrafo.h"

//---------------------------------------------------------------------------
class TTrafo : public VTTrafo
   {
   public:
                 __fastcall  TTrafo(int num_pat);
                 __fastcall ~TTrafo(void);
      bool       __fastcall  AngulosNoPrimario(double vet_ang_grau[MAX_FASE]);
      bool       __fastcall  AngulosNoSecundario(double vet_ang_grau[MAX_FASE]);
      VTBarra*   __fastcall  BarraPrimario(void);
      VTBarra*   __fastcall  BarraSecundario(void);
      VTTrafo*   __fastcall  Clone(void);
      void       __fastcall  CopiaAtributosDe(VTEqpto &ref);
      double     __fastcall  DefasagemPriSec(void);
      double     __fastcall  DefasagemSecPri(void);
      void       __fastcall  OrdenaBar12PriSec(void);
      double     __fastcall  PerdaNoFerroMW(void);
      void       __fastcall  RedefineBarraDoPrimario(VTBarra *pbar);
      void       __fastcall  RedefineVnom(VTBarra *pbar);
      int        __fastcall  Tipo(void);
      AnsiString __fastcall  TipoAsString(void);
      bool       __fastcall  Z0_Z1_pu(double Sbase, strIMP &z0_pu, strIMP &z1_pu);

   protected: //métodos virtuais redefinidos
      AnsiString __fastcall Alias(int ind_bar);
      int        __fastcall Fases(VTBarra *barra);
      VTMatY*    __fastcall PM_GetMatYij(void);
      VTMatY*    __fastcall PM_GetMatYji(void);
      VTMatZ*    __fastcall PM_GetMatZ(void);
      int        __fastcall PM_GetModeloEqv(void);
      bool       __fastcall PM_GetMonofasico(void);
      AnsiString __fastcall PM_GetPadrao(void);
      bool       __fastcall PM_GetReducao(void);
      int        __fastcall PM_GetTap(void);
      void       __fastcall PM_SetModeloEqv(int modelo_eqv);
      void       __fastcall PM_SetMonofasico(bool monofasico);
      void       __fastcall PM_SetPadrao(AnsiString padrao);
      void       __fastcall PM_SetReducao(bool reducao);
      void       __fastcall PM_SetTap(int tap);

   protected: //métodos acessados via property
      int             __fastcall PM_GetLigacao(void);
      complex<double> __fastcall PM_GetZ_pu(int i, int j);

   private:
      AnsiString padrao;

   };

//---------------------------------------------------------------------------
#endif
//eof

