//---------------------------------------------------------------------------
#ifndef VTCapacitorH
#define VTCapacitorH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "TEqbar.h"
#include "..\Constante\Const.h"

//---------------------------------------------------------------------------
class VTCurva;

//---------------------------------------------------------------------------
class VTCapacitor : public TEqbar
   {
   public:  //property
      __property double Q_mvar = {read=PM_GetQ_mvar};

   public: //objetos externos
      VTCurva  *curva;

   public:
                           __fastcall VTCapacitor(void) {};
      virtual              __fastcall ~VTCapacitor(void) {};
      virtual VTCapacitor* __fastcall Clone(void) = 0;
      virtual void         __fastcall DefineCurva(VTCurva *curva)=0;

   public:  //dados locais
      int     ligacao;
      float   vnom;  //kV
      float   q;     //MVAr

   protected: //métodos acessados via property
      virtual double __fastcall PM_GetQ_mvar(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe Capacitor
//---------------------------------------------------------------------------
VTCapacitor* __fastcall NewObjCapacitor(int num_pat);

//---------------------------------------------------------------------------
#endif
//eof

 