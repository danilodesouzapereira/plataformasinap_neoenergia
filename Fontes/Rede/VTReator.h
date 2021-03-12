//---------------------------------------------------------------------------
#ifndef VTReatorH
#define VTReatorH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "Estrutura.h"
#include "TEqbar.h"
#include "..\Constante\Const.h"

//---------------------------------------------------------------------------
class VTCurva;

//---------------------------------------------------------------------------
class VTReator : public TEqbar
   {
   public:  //property
      __property double Q_mvar = {read=PM_GetQ_mvar};

   public:
                        __fastcall VTReator(void) {};
      virtual           __fastcall ~VTReator(void) {};
      virtual VTReator* __fastcall Clone(void) = 0;
      virtual void      __fastcall DefineCurva(VTCurva *curva)=0;
		virtual void      __fastcall Z0_pu(double Sbase, double Vbase, strIMP &z0_pu) = 0;
		virtual void      __fastcall Z1_pu(double Sbase, double Vbase, strIMP &z1_pu) = 0;

   public: //objetos externos
      VTCurva  *curva;

   public: //dados
      int   ligacao;
      float vnom;   //kV
      float q;      //MVAr

   protected: //métodos acessados via property
      virtual double __fastcall PM_GetQ_mvar(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe Reator
//---------------------------------------------------------------------------
VTReator* __fastcall NewObjReator(int num_pat);

//---------------------------------------------------------------------------
#endif
//eof

 