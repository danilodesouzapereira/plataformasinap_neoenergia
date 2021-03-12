//---------------------------------------------------------------------------
#ifndef VTMatCH
#define VTMatCH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <complex>

//---------------------------------------------------------------------------
class VTMatC
   {
   public:  //property
		__property int    Dimensao          = {read=PM_GetDimensao, write=PM_SetDimensao};
      __property bool   Montada           = {read=PM_GetMontada,  write=PM_SetMontada};
		__property double Cap[int i][int j] = {read=PM_GetCap,      write=PM_SetCap};

   public:
                       __fastcall  VTMatC(void) {};
      virtual          __fastcall ~VTMatC(void) {};
      virtual  VTMatC* __fastcall  Clone(void) = 0;
      virtual  void    __fastcall  CopiaAtributosDe(VTMatC &ref) = 0;
      virtual  void    __fastcall  Zera(void) = 0;

   protected: //métodos acessados via property
		virtual double __fastcall PM_GetCap(int i, int j) = 0;
		virtual int    __fastcall PM_GetDimensao(void) = 0;
      virtual bool   __fastcall PM_GetMontada(void) = 0;
		virtual void   __fastcall PM_SetCap(int i, int j, double cap) = 0;
		virtual void   __fastcall PM_SetDimensao(int dim) = 0;
		virtual void   __fastcall PM_SetMontada(bool montada) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMatC
//---------------------------------------------------------------------------
VTMatC* __fastcall NewObjMatC(void);

//---------------------------------------------------------------------------
#endif
//eof


 