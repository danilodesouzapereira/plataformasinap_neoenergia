//-----------------------------------------------------------------------------
#ifndef VTSecundario_H
#define VTSecundario_H
using namespace std;

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>
#include <complex>

//-----------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTRede;
class VTPrimario;
class VTPatamar;

//-----------------------------------------------------------------------------
class VTSecundario  : public TObject
   {
  public:  //property
      __property VTBarra*    BarraInicial = {read=PM_GetBarraInicial, write=PM_SetBarraInicial};
      __property VTRede*     Rede         = {read=PM_GetRede,         write=PM_SetRede};
      __property VTPrimario* Primario     = {read=PM_GetPrimario,     write=PM_SetPrimario};

   public:
                         __fastcall  VTSecundario(void)  {};
      virtual            __fastcall ~VTSecundario(void) {};
      virtual bool       __fastcall  Ordena(TList *lisEXT_BAR, TList *lisEXT_LIG) = 0;
      virtual VTPatamar* __fastcall  PatamarDemandaMaxima(complex<double>&s_mva) = 0;
      virtual VTPatamar* __fastcall  PatamarDemandaMinima(complex<double>&s_mva) = 0;

   protected:  //métodos acessados via property
      virtual VTBarra*    __fastcall PM_GetBarraInicial(void) = 0;
      virtual VTPrimario* __fastcall PM_GetPrimario(void) = 0;
      virtual VTRede*     __fastcall PM_GetRede(void) = 0;
      virtual void        __fastcall PM_SetBarraInicial(VTBarra *barra) = 0;
      virtual void        __fastcall PM_SetPrimario(VTPrimario *primario) = 0;
      virtual void        __fastcall PM_SetRede(VTRede *rede) = 0;
    };

//-----------------------------------------------------------------------------
//função global
VTSecundario* __fastcall NewObjSecundario(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof
