//-----------------------------------------------------------------------------
#ifndef VTSubestacao_H
#define VTSubestacao_H
using namespace std;

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>
#include <complex>

//-----------------------------------------------------------------------------
class VTApl;
class VTRede;
class VTPrimario;
class VTReticulado;
class VTPatamar;
class VTTrafo;
class VTTrafo3E;

//-----------------------------------------------------------------------------
class VTSubestacao  : public TObject
   {
  public:  //property
      __property VTRede* Rede       = {read=PM_GetRede, write=PM_SetRede};
      __property double  Snom_mva   = {read=PM_GetSnom_mva};
      __property double  Sfirme_mva = {read=PM_GetSfirme_mva};

   public:
                          __fastcall  VTSubestacao(void)  {};
      virtual             __fastcall ~VTSubestacao(void) {};
      virtual VTPrimario* __fastcall  ExistePrimario(VTRede *rede) = 0;
      virtual void        __fastcall  InserePrimario(VTPrimario *primario) = 0;
      virtual void        __fastcall  InsereReticulado(VTReticulado *reticulado) = 0;
      virtual void        __fastcall  LisEqptoSubPriSec(TList *lisEXT, int tipo_eqpto, bool barra_isolada=true) = 0;
      virtual TList*      __fastcall  LisPrimario(void) = 0;
		virtual int         __fastcall  LisPrimario(VTTrafo *trafo, TList *lisEXT) = 0;
		virtual int         __fastcall  LisPrimario(VTTrafo3E *trafo3E, TList *lisEXT) = 0;
		virtual void        __fastcall  LisRede(TList *lisEXT, int segmento) = 0;
      virtual TList*      __fastcall  LisReticulado(void) = 0;
      virtual VTPatamar*  __fastcall  PatamarDemandaMaxima(complex<double>&s_mva) = 0;
      virtual VTPatamar*  __fastcall  PatamarDemandaMinima(complex<double>&s_mva) = 0;

   protected:  //métodos acessados via property
      virtual VTRede* __fastcall PM_GetRede(void) = 0;
      virtual double  __fastcall PM_GetSfirme_mva(void) = 0;
      virtual double  __fastcall PM_GetSnom_mva(void) = 0;
      virtual void    __fastcall PM_SetRede(VTRede *rede) = 0;
    };

//-----------------------------------------------------------------------------
//função global
VTSubestacao* __fastcall NewObjSubestacao(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof
