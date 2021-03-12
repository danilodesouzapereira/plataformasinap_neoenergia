//---------------------------------------------------------------------------
#ifndef VTMatYH
#define VTMatYH
using namespace std;

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <complex>

//---------------------------------------------------------------------------
class VTMatY
   {
   public:  //property
		__property int             Dimensao        = {read=PM_GetDimensao, write=PM_SetDimensao};
		__property int             ModeloEqv       = {read=PM_GetModeloEqv,write=PM_SetModeloEqv};
      __property bool            Montada         = {read=PM_GetMontada,  write=PM_SetMontada};
      __property bool            Simetrica       = {read=PM_GetSimetrica};
		__property double          G[int i][int j] = {read=PM_GetG,        write=PM_SetG};
		__property double          B[int i][int j] = {read=PM_GetB,        write=PM_SetB};
		__property complex<double> Y[int i][int j] = {read=PM_GetY,        write=PM_SetY};

	public:
                       __fastcall  VTMatY(void) {};
      virtual          __fastcall ~VTMatY(void) {};
      virtual  VTMatY* __fastcall  Clone(void) = 0;
      virtual  void    __fastcall  CopiaAtributosDe(VTMatY &ref) = 0;
      virtual  void    __fastcall  Zera(void) = 0;

   protected: //métodos acessados via property
      virtual double          __fastcall PM_GetB(int i, int j) = 0;
		virtual int             __fastcall PM_GetDimensao(void) = 0;
      virtual double          __fastcall PM_GetG(int i, int j) = 0;
		virtual int             __fastcall PM_GetModeloEqv(void) = 0;
      virtual bool            __fastcall PM_GetMontada(void) = 0;
      virtual bool            __fastcall PM_GetSimetrica(void) = 0;
      virtual complex<double> __fastcall PM_GetY(int i, int j) = 0;
		virtual void            __fastcall PM_SetB(int i, int j, double b_ij) = 0;
		virtual void            __fastcall PM_SetDimensao(int dim) = 0;
		virtual void            __fastcall PM_SetG(int i, int j, double g_ij) = 0;
		virtual void            __fastcall PM_SetModeloEqv(int modelo_eqv) = 0;
		virtual void            __fastcall PM_SetMontada(bool montada) = 0;
      virtual void            __fastcall PM_SetY(int i, int j, complex<double> y_ij) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMatY
//---------------------------------------------------------------------------
VTMatY* __fastcall NewObjMatY(void);

//---------------------------------------------------------------------------
#endif
//eof


 