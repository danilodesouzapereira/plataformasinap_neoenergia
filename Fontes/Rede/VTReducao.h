//---------------------------------------------------------------------------
#ifndef VTReducaoH
#define VTReducaoH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTEqpto.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTEqbar;
class VTLigacao;
class VTMRede;
class VTCorte;
class VTCrescimento;

//---------------------------------------------------------------------------
class VTReducao : public VTEqpto
   {
   public:  //property
      __property bool           Carregada   = {read=PM_GetCarregada};
      __property bool           Valida      = {read=PM_GetValida,      write=PM_SetValida};
      __property VTCorte*       Corte       = {read=PM_GetCorte,       write=PM_SetCorte};
      __property VTCrescimento* Crescimento = {read=PM_GetCrescimento, write=PM_SetCrescimento};
      __property VTMRede*       MRede       = {read=PM_GetMRede,       write=PM_SetMRede};
      __property int            NumeroEqpto = {read=PM_GetNumeroEqpto};

   public:
                         __fastcall VTReducao(void) {};
      virtual            __fastcall ~VTReducao(void) {};
      virtual VTBarra*   __fastcall BarraFicticia(VTLigacao *ligacao=NULL) = 0;
      virtual VTReducao* __fastcall Clone(void) = 0;
      virtual bool       __fastcall ExisteGeracao(void) = 0;
      virtual void       __fastcall InsereEqbar(VTEqbar *eqbar) = 0;
      virtual void       __fastcall InsereLigacao(VTLigacao *ligacao) = 0;
      virtual void       __fastcall IniciaEsquematico(int num_pat) = 0;
      virtual void       __fastcall IniciaInterconexao(int num_pat) = 0;
      virtual TList*     __fastcall LisBarra(void) = 0;
      virtual void       __fastcall LisBarra(TList *lisEQP) = 0;
      virtual TList*     __fastcall LisBarraEsquematico(void) = 0;
      virtual TList*     __fastcall LisEqbar(void) = 0;
      virtual void       __fastcall LisEqbar(TList *lisEQP) = 0;
      virtual void       __fastcall LisEqbar(TList *lisEQP, int tipo) = 0;
      virtual void       __fastcall LisEqpto(TList *lisEQP) = 0;
      virtual void       __fastcall LisEqpto(TList *lisEQP, int tipo) = 0;
      virtual TList*     __fastcall LisLigacao(void) = 0;
      virtual void       __fastcall LisLigacao(TList *lisEQP) = 0;
      virtual void       __fastcall LisLigacao(TList *lisEQP, int tipo) = 0;
      virtual TList*     __fastcall LisLigacaoEsquematico(void) = 0;
      virtual int        __fastcall NumeroDeEqbar(int tipo=-1) = 0;
      virtual void       __fastcall Reinicia(void) = 0;
      virtual bool       __fastcall Visible(void) = 0;

   protected:  //métodos acessados via property
      virtual bool           __fastcall PM_GetCarregada(void) = 0;
      virtual VTCorte*       __fastcall PM_GetCorte(void) = 0;
      virtual VTCrescimento* __fastcall PM_GetCrescimento(void) = 0;
      virtual bool           __fastcall PM_GetValida(void) = 0;
      virtual VTMRede*       __fastcall PM_GetMRede(void) = 0;
      virtual int            __fastcall PM_GetNumeroEqpto(void) = 0;
      virtual void           __fastcall PM_SetCorte(VTCorte *corte) = 0;
      virtual void           __fastcall PM_SetCrescimento(VTCrescimento *crescimento) = 0;
      virtual void           __fastcall PM_SetMRede(VTMRede *mrede) = 0;
      virtual void           __fastcall PM_SetValida(bool valida) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTReducao
//---------------------------------------------------------------------------
VTReducao* __fastcall NewObjReducao(int num_pat);

//---------------------------------------------------------------------------
#endif
//eof



 