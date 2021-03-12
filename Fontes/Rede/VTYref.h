//---------------------------------------------------------------------------
#ifndef VTYrefH
#define VTYrefH
using namespace std;

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "Estrutura.h"
#include "TEqbar.h"
#include "..\Constante\Const.h"

//---------------------------------------------------------------------------
class VTYref : public TEqbar
   {
   public:  //property
      __property complex<double> Y0_pu              = {read=PM_GetY0_pu, write=PM_SetY0_pu};
      __property complex<double> Y1_pu              = {read=PM_GetY1_pu, write=PM_SetY1_pu};
      __property complex<double> Y_pu[int i][int j] = {read=PM_GetY_pu,  write=PM_SetY_pu};
      __property bool            Reducao            = {read=PM_GetReducao,    write=PM_SetReducao};

   public:
                              __fastcall VTYref(void) {};
      virtual                 __fastcall ~VTYref(void) {};
      virtual VTYref*         __fastcall Clone(void) = 0;
      virtual complex<double> __fastcall DemandaMVA(int num_pat) = 0;
      virtual complex<double> __fastcall Y0_s(double Sbase, double Vbase) = 0;
      virtual complex<double> __fastcall Y1_s(double Sbase, double Vbase) = 0;
      //virtual complex<double> __fastcall Z1_pu(double Sbase, double Vbase) = 0;
      //virtual void            __fastcall Z_ohm(strIMP &z_ohm) = 0;
      virtual void            __fastcall Z0_pu(strIMP &z0_pu) = 0;
      virtual void            __fastcall Z1_pu(strIMP &z1_pu) = 0;

   public: //FUNÇÃO TEMPORÁRIA ATÉ ALTERAR BASE DE DADOS
      virtual void  __fastcall Inicia(double r_pu, double x_pu) = 0;

   protected: //métodos acessados via property
      virtual bool            __fastcall PM_GetReducao(void) = 0;
      virtual complex<double> __fastcall PM_GetY0_pu(void) = 0;
      virtual complex<double> __fastcall PM_GetY1_pu(void) = 0;
      virtual complex<double> __fastcall PM_GetY_pu(int i, int j) = 0;
      virtual void            __fastcall PM_SetReducao(bool reducao) = 0;
      virtual void            __fastcall PM_SetY0_pu(complex<double> y0) = 0;
      virtual void            __fastcall PM_SetY1_pu(complex<double> y1) = 0;
      virtual void            __fastcall PM_SetY_pu(int i, int j, complex<double> y_ij) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTYref
//---------------------------------------------------------------------------
VTYref* __fastcall NewObjYref(void);

//---------------------------------------------------------------------------
#endif
//eof

 