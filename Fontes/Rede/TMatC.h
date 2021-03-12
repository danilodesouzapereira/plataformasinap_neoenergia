//---------------------------------------------------------------------------
#ifndef TMatCH
#define TMatCH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTMatC.h"
#include "VTFases.h"


//---------------------------------------------------------------------------
class TMatC : public VTMatC
   {
   public:
              __fastcall  TMatC(void);
              __fastcall ~TMatC(void);
      VTMatC* __fastcall  Clone(void);
      void    __fastcall  CopiaAtributosDe(VTMatC &ref);
      void    __fastcall  Zera(void);

   protected: //métodos acessados via property
		double __fastcall PM_GetCap(int i, int j);
		int    __fastcall PM_GetDimensao(void);
      bool   __fastcall PM_GetMontada(void);
		void   __fastcall PM_SetCap(int i, int j, double cap);
		void   __fastcall PM_SetDimensao(int dim);
      void   __fastcall PM_SetMontada(bool montada);

	protected:  //dados
		struct  	{
					int   dim;
               bool  montada;
               float mat_c[MAX_FASE][MAX_FASE];  //matriz de capacitâncias
					}PD;
   };

//---------------------------------------------------------------------------
#endif
//eof


 