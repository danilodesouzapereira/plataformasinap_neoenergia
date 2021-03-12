//---------------------------------------------------------------------------
#ifndef VTSuporteH
#define VTSuporteH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\VTEqpto.h"

//---------------------------------------------------------------------------
enum enumTIPOSUPORTE {suporteAEREO=1,
                      suporteSUBTERRANEO,
                      suporteSUBMERSO,
                      suporteINDEFINIDO};

//---------------------------------------------------------------------------
class VTPtoFixacao;

//---------------------------------------------------------------------------
class VTSuporte : public VTEqpto
   {
   public:  //property
      __property double   Vmax_KV       = {read=PM_GetVmax_KV,       write=PM_SetVmax_KV};
      __property double   Vmin_KV       = {read=PM_GetVmin_KV,       write=PM_SetVmin_KV};
      __property int      TipoDeSuporte = {read=PM_GetTipoDeSuporte, write=PM_SetTipoDeSuporte};

   public:
                            __fastcall VTSuporte(void) {};
      virtual               __fastcall ~VTSuporte(void) {};
      virtual VTSuporte*    __fastcall Clone(void) = 0;
      virtual VTPtoFixacao* __fastcall ExistePtoFixacao(AnsiString codigo) = 0;
      virtual VTPtoFixacao* __fastcall ExistePtoFixacao(int id) = 0;
      virtual VTPtoFixacao* __fastcall ExistePtoFixacaoIndex(int index) = 0;

      virtual bool          __fastcall InserePtoFixacao(VTPtoFixacao *pto_fixacao) = 0;
      virtual TList*        __fastcall LisPtoFixacao(void) = 0;
      virtual AnsiString    __fastcall TipoDeSuporteAsString(void) = 0;

   protected: //métodos acessados via property
      virtual int      __fastcall PM_GetTipoDeSuporte(void) = 0;
      virtual double   __fastcall PM_GetVmax_KV(void) = 0;
      virtual double   __fastcall PM_GetVmin_KV(void) = 0;
      virtual void     __fastcall PM_SetTipoDeSuporte(int tipo) = 0;
      virtual void     __fastcall PM_SetVmax_KV(double vmax) = 0;
      virtual void     __fastcall PM_SetVmin_KV(double vmin) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTSuporte
//---------------------------------------------------------------------------
VTSuporte* __fastcall NewObjSuporte(void);

//---------------------------------------------------------------------------
#endif
//eof


 