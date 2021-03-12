//---------------------------------------------------------------------------
#ifndef VTMRedeH
#define VTMRedeH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTEqpto.h"
#include "..\Constante\Const.h"

//---------------------------------------------------------------------------
class VTCorte;
class VTRede;
class VTReducao;
class VTMNet;

//---------------------------------------------------------------------------
class VTMRede : public VTEqpto
   {
   public:  //property
      __property AnsiString Codigo     = {read=PM_GetCodigo};
      __property TColor     Color      = {read=PM_GetColor};
      __property bool       Carregada  = {read=PM_GetCarregada,  write=PM_SetCarregada};
      __property bool       Geracao    = {read=PM_GetGeracao};
      __property bool       Malha      = {read=PM_GetMalha,      write=PM_SetMalha};
      __property bool       Redutivel  = {read=PM_GetRedutivel,  write=PM_SetRedutivel};
      __property TDate      Revisao    = {read=PM_GetRevisao,    write=PM_SetRevisao};
      __property bool       Valida     = {read=PM_GetValida};
      __property VTMNet     *MNet      = {read=PM_GetMNet,       write=PM_SetMNet};
      __property VTMRede    *Pai       = {read=PM_GetPai,        write=PM_SetPai};

   public:
                         __fastcall VTMRede(void) {};
      virtual            __fastcall ~VTMRede(void) {};
      virtual VTMRede*   __fastcall Clone(void) = 0;
      virtual bool       __fastcall AreaEsquematico(int &x1, int &y1, int &x2, int &y2) = 0;
      virtual bool       __fastcall AreaUtm_cm(int &x1, int &y1, int &x2, int &y2) = 0;
      virtual bool       __fastcall ExisteCorte(VTCorte *corte) = 0;
      virtual VTCorte*   __fastcall ExisteCorte(VTMRede *mrede) = 0;
      virtual bool       __fastcall ExisteRede(VTRede *rede) = 0;
      virtual VTReducao* __fastcall ExisteReducao(VTCorte *corte) = 0;
      virtual VTReducao* __fastcall ExisteReducaoMRedeNaoCarregada(void) = 0;
      virtual void       __fastcall IncorporaMRede(TList *lisMR) = 0;
      virtual void       __fastcall IncorporaMRede(VTMRede *mrede) = 0;
      virtual void       __fastcall InsereCorte(VTCorte *corte) = 0;
      virtual void       __fastcall InsereRede(VTRede *rede) = 0;
      virtual void       __fastcall InsereRede(TList *lisEQP) = 0;
      virtual void       __fastcall InsereReducao(VTReducao *reducao, VTCorte *corte) = 0;
      virtual void       __fastcall LisBarra(TList *lisEQP, bool barra_isolada=true) = 0;
      virtual TList*     __fastcall LisCorte(void) = 0;
      virtual void       __fastcall LisCorte(TList *lisEQP) = 0;
      virtual void       __fastcall LisEqbar(TList *lisEQP, int tipo, bool barra_isolada=true) = 0;
      virtual void       __fastcall LisEqpto(TList *lisEQP, int tipo, bool barra_isolada=true) = 0;
      virtual void       __fastcall LisLigacao(TList *lisEQP, int tipo=-1) = 0;
      virtual void       __fastcall LisMRedeVizinha(TList *lisMRV) = 0;
      virtual void       __fastcall LisMRedeVizinha(VTCorte *corte, TList *lisMRV) = 0;
      virtual void       __fastcall LisMRedeVizinhaCarregada(VTCorte *corte, TList *lisMRV) = 0;
      virtual void       __fastcall LisMutua(TList *lisEQP) = 0;
      virtual TList*     __fastcall LisRede(void) = 0;
      virtual void       __fastcall LisRede(TList *lisEQP) = 0;
      virtual TList*     __fastcall LisReducao(void) = 0;
      virtual void       __fastcall LisReducao(TList *lisEQP) = 0;
      virtual void       __fastcall LisReducaoVizinha(TList *lisEQP) = 0;
      virtual int        __fastcall NumeroDeCortes(void) = 0;
      virtual int        __fastcall NumeroDeGerSup(void) = 0;
      virtual int        __fastcall NumeroDeRedes(void) = 0;
      virtual void       __fastcall ReiniciaReducao(void) = 0;
      virtual void       __fastcall RemoveCorte(VTCorte *corte) = 0;
      //virtual void       __fastcall RetiraReducao(VTReducao *reducao) = 0;

   protected:  //métodos acessados via property
      virtual bool       __fastcall PM_GetCarregada(void) = 0;
      virtual TColor     __fastcall PM_GetColor(void) = 0;
      virtual AnsiString __fastcall PM_GetCodigo(void) = 0;
      virtual bool       __fastcall PM_GetGeracao(void) = 0;
      virtual bool       __fastcall PM_GetMalha(void) = 0;
      virtual VTMNet*    __fastcall PM_GetMNet(void) = 0;
      virtual VTMRede*   __fastcall PM_GetPai(void) = 0;
      virtual bool       __fastcall PM_GetRedutivel(void) = 0;
      virtual TDate      __fastcall PM_GetRevisao(void) = 0;
      virtual bool       __fastcall PM_GetValida(void) = 0;
      virtual void       __fastcall PM_SetCarregada(bool carregada) = 0;
      virtual void       __fastcall PM_SetMalha(bool malha) = 0;
      virtual void       __fastcall PM_SetMNet(VTMNet *mnet) = 0;
      virtual void       __fastcall PM_SetPai(VTMRede *pai) = 0;
      virtual void       __fastcall PM_SetRedutivel(bool redutivel) = 0;
      virtual void       __fastcall PM_SetRevisao(TDate revisao) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMRede
//---------------------------------------------------------------------------
VTMRede* __fastcall NewObjMRede(void);

//---------------------------------------------------------------------------
#endif
//eof


 