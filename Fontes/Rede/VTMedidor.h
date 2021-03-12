//---------------------------------------------------------------------------
#ifndef VTMedidorH
#define VTMedidorH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "TEqbar.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTCanal;
class VTEqpto;

//---------------------------------------------------------------------------
class VTMedidor : public TEqbar
   {
   public:  //property
		__property bool     CanaisEnabled  = {read=PM_GetCanaisEnabled                  };
      __property VTBarra* Barra          = {read=PM_GetBarra,        write=PM_SetBarra};

   public:
                         __fastcall  VTMedidor(void) {};
      virtual            __fastcall ~VTMedidor(void) {};
      virtual void       __fastcall  Clear(void) = 0;
      virtual VTMedidor* __fastcall  Clone(void) = 0;
      virtual VTCanal*   __fastcall  ExisteCanal(int numero)=0;
      virtual VTCanal*   __fastcall  ExisteCanal(AnsiString codigo)=0;
      virtual bool       __fastcall  ExisteCanal(VTCanal *canal)=0;
		virtual VTCanal*   __fastcall  ExisteCanal(VTEqpto *eqpto, int tipo_canal, int fases)=0;
		virtual bool       __fastcall  InsereCanal(VTCanal *canal)=0;
		virtual VTCanal*   __fastcall  InsereCanal(VTEqpto *eqpto, int tipo_canal, int fases)=0;
		virtual TList*     __fastcall  LisCanal(void)=0;
		virtual int        __fastcall  LisCanal(TList *lisEXT, VTEqpto *eqpto)=0;
      virtual int        __fastcall  LisEqptoDosCanais(TList *lisEXT)=0;
      virtual bool       __fastcall  RemoveCanal(VTCanal *canal)=0;

   protected:  //métodos acessados via property
      virtual VTBarra* __fastcall PM_GetBarra(void)=0;
      virtual bool     __fastcall PM_GetCanaisEnabled(void) = 0;
      virtual void     __fastcall PM_SetBarra(VTBarra *barra)=0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe Medidor
//---------------------------------------------------------------------------
VTMedidor* __fastcall NewObjMedidor(void);

//---------------------------------------------------------------------------
#endif
//eof

 