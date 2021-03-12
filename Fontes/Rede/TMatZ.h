//---------------------------------------------------------------------------
#ifndef TMatZH
#define TMatZH
using namespace std;

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTMatZ.h"
#include "VTFases.h"
//#include "..\Constante\Const.h"

//---------------------------------------------------------------------------
class TMatZ : public VTMatZ
   {
   public:
              __fastcall  TMatZ(void);
			     __fastcall ~TMatZ(void);
      VTMatZ* __fastcall  Clone(void);
      void    __fastcall  CopiaAtributosDe(VTMatZ &ref);
      void    __fastcall  Zera(void);

   protected: //métodos acessados via property
      int             __fastcall PM_GetDimensao(void);
      bool            __fastcall PM_GetMontada(void);
      double          __fastcall PM_GetR(int i, int j);
      bool            __fastcall PM_GetSimetrica(void);
      double          __fastcall PM_GetX(int i, int j);
		complex<double> __fastcall PM_GetZ(int i, int j);
		void            __fastcall PM_SetDimensao(int dim);
      void            __fastcall PM_SetMontada(bool montada);
      void            __fastcall PM_SetR(int i, int j, double r_ij);
      void            __fastcall PM_SetX(int i, int j, double x_ij);
      void            __fastcall PM_SetZ(int i, int j, complex<double> z_ij);

	private:  //
		struct	{
					int  dim;
               bool montada;
		         complex<float> mat_z[MAX_FASE][MAX_FASE];  //matriz de impedâncias
					} PD;
   };

//---------------------------------------------------------------------------
#endif
//eof


 