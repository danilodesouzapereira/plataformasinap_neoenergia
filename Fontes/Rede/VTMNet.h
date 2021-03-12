//---------------------------------------------------------------------------
#ifndef VTMNetH
#define VTMNetH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTEqpto.h"
#include "..\Constante\Const.h"

//---------------------------------------------------------------------------
class VTCorte;
class VTMRede;
class VTNet;
class VTRede;
class VTReducao;

//---------------------------------------------------------------------------
class VTMNet : public TObject
   {
   public:  //property
		__property AnsiString Codigo       = {read=PM_GetCodigo};
		__property bool       Enabled      = {read=PM_GetEnabled, write=PM_SetEnabled};
		__property bool       Geracao      = {read=PM_GetGeracao};
		__property int        NumeroGerSup = {read=PM_GetNumeroGerSup};
		__property int        NumeroMRede  = {read=PM_GetNumeroMRede};
		__property VTMNet*    Pai          = {read=PM_GetPai, write=PM_SetPai};
		__property VTNet*     Net          = {read=PM_GetNet, write=PM_SetNet};
		__property TObject*   Obj          = {read=PM_GetObj, write=PM_SetObj};

   public:
						 __fastcall VTMNet(void) {};
	  virtual            __fastcall ~VTMNet(void) {};
	  virtual bool       __fastcall ExisteCorte(VTCorte *corte) = 0;
	  virtual VTCorte*   __fastcall ExisteCorte(VTMNet *mnet) = 0;
	  virtual bool       __fastcall ExisteMRede(VTMRede *mrede) = 0;
	  virtual VTMRede*   __fastcall ExisteMRede(VTRede *rede) = 0;
	  virtual VTMRede*   __fastcall ExisteMRedeCarregada(void) = 0;
	  virtual VTReducao* __fastcall ExisteReducao(VTCorte *corte) = 0;
	  virtual void       __fastcall IncorporaMNet(TList *lisEXT) = 0;
	  virtual void       __fastcall IncorporaMNet(VTMNet *mnet) = 0;
	  virtual void       __fastcall InsereMRede(VTMRede *mrede) = 0;
	  virtual void       __fastcall InsereMRede(TList *lisEXT) = 0;
	  virtual void       __fastcall InsereReducao(VTReducao *reducao, VTCorte *corte) = 0;
	  virtual void       __fastcall LisBarra(TList *lisEXT, bool barra_isolada=true) = 0;
	  virtual TList*     __fastcall LisCorte(void) = 0;
	  virtual void       __fastcall LisCorte(TList *lisEXT) = 0;
	  virtual void       __fastcall LisEqbar(TList *lisEXT, int tipo=-1, bool barra_isolada=true) = 0;
	  virtual void       __fastcall LisEqpto(TList *lisEXT, int tipo=-1, bool barra_isolada=true) = 0;
	  virtual TList*     __fastcall LisGerador(void) = 0;
	  virtual void       __fastcall LisLigacao(TList *lisEXT, int tipo=-1) = 0;
	  virtual void       __fastcall LisMNetVizinha(TList *lisEXT) = 0;
	  virtual void       __fastcall LisMNetVizinha(VTCorte *corte, TList *lisEXT) = 0;
	  virtual TList*     __fastcall LisMRede(void) = 0;
	  virtual void       __fastcall LisMRede(VTCorte *corte, TList *lisEXT) = 0;
	  virtual void       __fastcall LisMRedeCarregada(VTCorte *corte, TList *lisEXT) = 0;
	  virtual void       __fastcall LisMutua(TList *lisEXT) = 0;
	  virtual void       __fastcall LisRede(TList *lisEXT) = 0;
	  virtual TList*     __fastcall LisReducao(void) = 0;
	  virtual void       __fastcall LisReducaoVizinha(TList *lisEXT) = 0;
	  virtual void       __fastcall LisReducaoVizinha(VTCorte *corte, TList *lisEXT) = 0;
	  virtual TList*     __fastcall LisSuprimento(void) = 0;
	  virtual void       __fastcall OrdenaMRedes(VTCorte *corte) = 0;

   protected:  //métodos acessados via property
	  virtual AnsiString __fastcall PM_GetCodigo(void) = 0;
	  virtual bool       __fastcall PM_GetEnabled(void) = 0;
	  virtual bool       __fastcall PM_GetGeracao(void) = 0;
	  virtual VTNet*     __fastcall PM_GetNet(void) = 0;
	  virtual int        __fastcall PM_GetNumeroGerSup(void) = 0;
	  virtual int        __fastcall PM_GetNumeroMRede(void) = 0;
	  virtual TObject*   __fastcall PM_GetObj(void) = 0;
	  virtual VTMNet*    __fastcall PM_GetPai(void) = 0;
	  //
	  virtual void       __fastcall PM_SetEnabled(bool enabled) = 0;
	  virtual void       __fastcall PM_SetNet(VTNet* net) = 0;
	  virtual void       __fastcall PM_SetObj(TObject* obj) = 0;
	  virtual void       __fastcall PM_SetPai(VTMNet *pai) = 0;

	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMNet
//---------------------------------------------------------------------------
VTMNet* __fastcall NewObjMNet(void);

//---------------------------------------------------------------------------
#endif
//eof


 