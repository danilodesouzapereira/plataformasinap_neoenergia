//---------------------------------------------------------------------------
#ifndef TMatYH
#define TMatYH
using namespace std;

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTMatY.h"
#include "VTFases.h"
//#include "..\Constante\Const.h"

//---------------------------------------------------------------------------
class TMatY : public VTMatY
   {
   public:
              __fastcall  TMatY(void);
			     __fastcall ~TMatY(void);
      VTMatY* __fastcall  Clone(void);
      void    __fastcall  CopiaAtributosDe(VTMatY &ref);
      void    __fastcall  Zera(void);

   protected: //métodos acessados via property
      double          __fastcall PM_GetB(int i, int j);
		int             __fastcall PM_GetDimensao(void);
      double          __fastcall PM_GetG(int i, int j);
		int             __fastcall PM_GetModeloEqv(void);
      bool            __fastcall PM_GetMontada(void);
      bool            __fastcall PM_GetSimetrica(void);
      complex<double> __fastcall PM_GetY(int i, int j);
		void            __fastcall PM_SetB(int i, int j, double b_ij);
		void            __fastcall PM_SetDimensao(int dim);
		void            __fastcall PM_SetG(int i, int j, double g_ij);
		void            __fastcall PM_SetModeloEqv(int modelo_eqv);
		void            __fastcall PM_SetMontada(bool montada);
      void            __fastcall PM_SetY(int i, int j, complex<double> y_ij);

	private:  //
		struct	{
					int  dim;
               int  modelo_eqv;
               bool montada;
 		         complex<float> mat_y[MAX_FASE][MAX_FASE];  //matriz de admitâncias
					} PD;
   };

//---------------------------------------------------------------------------
#endif
//eof


 