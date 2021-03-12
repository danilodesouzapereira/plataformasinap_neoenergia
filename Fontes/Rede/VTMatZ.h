//---------------------------------------------------------------------------
#ifndef VTMatZH
#define VTMatZH
using namespace std;

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <complex>

//---------------------------------------------------------------------------
class VTMatZ
   {
   public:  //property
		__property int             Dimensao        = {read=PM_GetDimensao, write=PM_SetDimensao};
      __property bool            Montada         = {read=PM_GetMontada,  write=PM_SetMontada};
      __property bool            Simetrica       = {read=PM_GetSimetrica};
		__property double          R[int i][int j] = {read=PM_GetR,        write=PM_SetR};
		__property double          X[int i][int j] = {read=PM_GetX,        write=PM_SetX};
		__property complex<double> Z[int i][int j] = {read=PM_GetZ,        write=PM_SetZ};

	public:
                       __fastcall  VTMatZ(void) {};
      virtual          __fastcall ~VTMatZ(void) {};
      virtual  VTMatZ* __fastcall  Clone(void) = 0;
      virtual  void    __fastcall  CopiaAtributosDe(VTMatZ &ref) = 0;
      virtual  void    __fastcall  Zera(void) = 0;

   protected: //métodos acessados via property
		virtual int             __fastcall PM_GetDimensao(void) = 0;
      virtual bool            __fastcall PM_GetMontada(void) = 0;
      virtual double          __fastcall PM_GetR(int i, int j) = 0;
      virtual bool            __fastcall PM_GetSimetrica(void) = 0;
      virtual double          __fastcall PM_GetX(int i, int j) = 0;
      virtual complex<double> __fastcall PM_GetZ(int i, int j) = 0;
		virtual void            __fastcall PM_SetDimensao(int dim) = 0;
		virtual void            __fastcall PM_SetMontada(bool montada) = 0;
		virtual void            __fastcall PM_SetR(int i, int j, double r_ij) = 0;
		virtual void            __fastcall PM_SetX(int i, int j, double x_ij) = 0;
      virtual void            __fastcall PM_SetZ(int i, int j, complex<double> z_ij) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMatZ
//---------------------------------------------------------------------------
VTMatZ* __fastcall NewObjMatZ(void);

//---------------------------------------------------------------------------
#endif
//eof


 