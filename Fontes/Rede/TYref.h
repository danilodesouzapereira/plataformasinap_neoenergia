//---------------------------------------------------------------------------
#ifndef TYrefH
#define TYrefH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTYref.h"
#include "..\Constante\Fases.h"

//---------------------------------------------------------------------------
class TYref : public VTYref
   {
   public:
                      __fastcall  TYref(void);
                      __fastcall ~TYref(void);
      VTYref*         __fastcall  Clone(void);
      void            __fastcall  CopiaAtributosDe(VTEqpto &ref);
      complex<double> __fastcall  DemandaMVA(int num_pat);
      void            __fastcall  Z0_pu(strIMP &z0_pu);
      void            __fastcall  Z1_pu(strIMP &z1_pu);
      complex<double> __fastcall  Y0_s(double Sbase, double Vbase);
      complex<double> __fastcall  Y1_s(double Sbase, double Vbase);
      int             __fastcall  Tipo(void);
      AnsiString      __fastcall  TipoAsString(void);

   public: //FUNÇÃO TEMPORÁRIA ATÉ ALTERAR BASE DE DADOS
      void  __fastcall Inicia(double r_pu, double x_pu);

   protected: //métodos acessados via property
      bool            __fastcall PM_GetReducao(void);
      complex<double> __fastcall PM_GetY0_pu(void);
      complex<double> __fastcall PM_GetY1_pu(void);
      complex<double> __fastcall PM_GetY_pu(int i, int j);
      void            __fastcall PM_SetReducao(bool reducao);
      void            __fastcall PM_SetY0_pu(complex<double> y0);
      void            __fastcall PM_SetY1_pu(complex<double> y1);
      void            __fastcall PM_SetY_pu(int i, int j, complex<double> y_ij);

   private:
      bool __fastcall Inverte(complex<float> &valor, complex<float> &inverso);

   protected: //dados acessados via property
      struct   {
               bool           reducao; //eqpto de redução
               complex<float> y0_pu, y1_pu;
               complex<float> y_pu[MAX_FASE][MAX_FASE];
               }PD;
   };

//---------------------------------------------------------------------------
#endif
//eof

 