//-----------------------------------------------------------------------------
#ifndef VTPrimario_H
#define VTPrimario_H
using namespace std;

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>
#include <complex>

//-----------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTLigacao;
class VTRede;
class VTSubestacao;
class VTSecundario;
class VTPatamar;

//-----------------------------------------------------------------------------
class VTPrimario  : public TObject
   {
  public:  //property
      __property VTBarra*      BarraInicial   = {read=PM_GetBarraInicial};
      __property VTLigacao*    LigacaoInicial = {read=PM_GetLigacaoInicial};
      __property VTRede*       Rede           = {read=PM_GetRede,         write=PM_SetRede      };
      __property VTSubestacao* Subestacao     = {read=PM_GetSubestacao,   write=PM_SetSubestacao};

   public:
                            __fastcall  VTPrimario(void)  {};
      virtual               __fastcall ~VTPrimario(void) {};
      virtual VTSecundario* __fastcall  ExisteSecundario(VTRede *rede) = 0;
      virtual void          __fastcall  InsereSecundario(VTSecundario* secundario) = 0;
      virtual void          __fastcall  LisEqptoPriSec(TList *lisEXT, int tipo_eqpto, bool barra_isolada=true) = 0;
      virtual TList*        __fastcall  LisSecundario(void) = 0;
      virtual void          __fastcall  LisRede(TList *lisEXT, int segmento) = 0;
      virtual bool          __fastcall  Ordena(TList *lisEXT_BAR, TList *lisEXT_LIG) = 0;
      virtual VTPatamar*    __fastcall  PatamarDemandaMaxima(complex<double>&s_mva) = 0;
      virtual VTPatamar*    __fastcall  PatamarDemandaMinima(complex<double>&s_mva) = 0;

   protected:  //métodos acessados via property
      virtual VTBarra*      __fastcall PM_GetBarraInicial(void) = 0;
      virtual VTLigacao*    __fastcall PM_GetLigacaoInicial(void) = 0;
      virtual VTRede*       __fastcall PM_GetRede(void) = 0;
      virtual VTSubestacao* __fastcall PM_GetSubestacao(void) = 0;
      virtual void          __fastcall PM_SetRede(VTRede *rede) = 0;
      virtual void          __fastcall PM_SetSubestacao(VTSubestacao *subestacao) = 0;
    };

//-----------------------------------------------------------------------------
//função global
VTPrimario* __fastcall NewObjPrimario(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof
