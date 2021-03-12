//---------------------------------------------------------------------------
#ifndef VTSecaoH
#define VTSecaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBarra;
class VTLigacao;

//---------------------------------------------------------------------------
enum tipoSECAO {secaoRADIAL, secaoMALHA};

//---------------------------------------------------------------------------
class VTSecao : public TObject
   {
   public:  //property
      __property int      Sentido  = {read=PM_GetSentido};
      __property int      Tipo     = {read=PM_GetTipo};
      __property VTSecao* SecaoPai = {read=PM_GetSecaoPai, write=PM_SetSecaoPai};

   public:
                         __fastcall VTSecao(void) {};
      virtual            __fastcall ~VTSecao(void) {};
      virtual VTBarra*   __fastcall BarraFinal(void) = 0;
      virtual VTBarra*   __fastcall BarraInicial(void) = 0;
      virtual int        __fastcall ComprimentoUtm_cm(void) = 0;
      virtual void       __fastcall DefineSentido(void) = 0;
      virtual bool       __fastcall ExisteBarra(VTBarra *barra) = 0;
      virtual bool       __fastcall ExisteLigacao(VTLigacao *ligacao) = 0;
      virtual void       __fastcall InsereLigacao(VTLigacao *ligacao) = 0;
      virtual VTLigacao* __fastcall LigacaoFinal(void) = 0;
      virtual VTLigacao* __fastcall LigacaoInicial(void) = 0;
      virtual void       __fastcall LisBarra(TList *lisBAR) = 0;
      virtual TList*     __fastcall LisLigacao(void) = 0;
      virtual void       __fastcall Ordena(bool bar_final_fixa) = 0;

   protected:  //métodos acessados via property
      virtual VTSecao* __fastcall PM_GetSecaoPai(void) = 0;
      virtual int      __fastcall PM_GetSentido(void) = 0;
      virtual int      __fastcall PM_GetTipo(void) = 0;
      virtual void     __fastcall PM_SetSecaoPai(VTSecao *pai) = 0;
      virtual void     __fastcall PM_SetTipo(int tipo) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTSecao
//---------------------------------------------------------------------------
VTSecao* __fastcall NewObjSecao(int tipo);

//---------------------------------------------------------------------------
#endif
//eof



 