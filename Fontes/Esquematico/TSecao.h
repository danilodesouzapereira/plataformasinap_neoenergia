//---------------------------------------------------------------------------
#ifndef TSecaoH
#define TSecaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTSecao.h"

//---------------------------------------------------------------------------
//class VTBarra;

//---------------------------------------------------------------------------
class TSecao : public VTSecao
   {
   public:
                 __fastcall TSecao(int tipo);
                 __fastcall ~TSecao(void);
      VTBarra*   __fastcall BarraFinal(void);
      VTBarra*   __fastcall BarraInicial(void);
      int        __fastcall ComprimentoUtm_cm(void);
      void       __fastcall DefineSentido(void);
      bool       __fastcall ExisteBarra(VTBarra *barra);
      bool       __fastcall ExisteLigacao(VTLigacao *ligacao);
      void       __fastcall InsereLigacao(VTLigacao *ligacao);
      VTLigacao* __fastcall LigacaoFinal(void);
      VTLigacao* __fastcall LigacaoInicial(void);
      void       __fastcall LisBarra(TList *lisBAR);
      TList*     __fastcall LisLigacao(void);
      void       __fastcall Ordena(bool bar_final_fixa);

   private: //métodos acessados via property
      VTSecao* __fastcall PM_GetSecaoPai(void);
      int      __fastcall PM_GetSentido(void);
      int      __fastcall PM_GetTipo(void);
      void     __fastcall PM_SetSecaoPai(VTSecao *pai);
      void     __fastcall PM_SetTipo(int tipo);


   private: //métodos
      void __fastcall OrdenaMalha(void);
      void __fastcall OrdenaRetaEquacao(void);
      void __fastcall OrdenaRetaHoriRightLeft(void);
      void __fastcall OrdenaRetaHoriLeftRight(void);
      void __fastcall OrdenaRetaVertBottomTop(void);
      void __fastcall OrdenaRetaVertTopBottom(void);

   private: //dados externos

   private: //dados acessados via property
      struct   {
               int     sentido;
               int     tipo;
               VTSecao *pai;
               } PD;

   private: //dados locais
      //bool  barra_final_fixa;
      TList *lisLIG;
      TList *lisBAR;
   };

//---------------------------------------------------------------------------
#endif
//eof

