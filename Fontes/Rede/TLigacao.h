//---------------------------------------------------------------------------
#ifndef TLigacaoH
#define TLigacaoH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTLigacao.h"

//---------------------------------------------------------------------------
class TLigacao : public VTLigacao
   {
   public:
                 __fastcall TLigacao(void);
      virtual    __fastcall ~TLigacao(void);
      VTBarra*   __fastcall Barra(int ind_bar);
      VTBarra*   __fastcall BarraComum(VTLigacao *ligacao);
      int        __fastcall ComprimentoEsq(void);
      int        __fastcall ComprimentoUtm_cm(void);
      bool       __fastcall ConectaBarra(VTBarra *pbarra);
      void       __fastcall CoordenadasEsquematico(int &x1, int &y1, int &x2, int &y2);
      void       __fastcall CoordenadasEsquematico(int &xm, int &ym, int &x1, int &y1,
                                                   int &x2, int &y2, int &x3, int &y3);
      bool       __fastcall CoordenadasUtm_cm(int &x, int &y);
      bool       __fastcall CoordenadasUtm_cm(int &x1, int &y1, int &x2, int &y2);
      bool       __fastcall CoordenadasUtm_cm(int &xm, int &ym, int &x1, int &y1,
                                              int &x2, int &y2, int &x3, int &y3);
      bool       __fastcall CoordenadasUtm_m(int &x, int &y);
      bool       __fastcall CoordenadasUtm_m(int &x1, int &y1, int &x2, int &y2);
      bool       __fastcall CoordenadasUtm_m(int &xm, int &ym, int &x1, int &y1,
                                             int &x2, int &y2, int &x3, int &y3);
      void       __fastcall CopiaAtributosDe(VTEqpto &ref);
      double     __fastcall Corrente(int num_pat);
      double     __fastcall Corrente(int ind_lig, int num_pat);
      int        __fastcall Declividade(void);
      void       __fastcall DefineObjBarra(VTBarra *pbar1, VTBarra *pbar2, VTBarra *pbar3=NULL);
      void       __fastcall DefineObjRede(VTRede *rede);
      void       __fastcall DefinePosBar(VTBarra *pbar, double posbar);
      void       __fastcall DefinePosBar(VTBarra *pbar, int x, int y);
      double     __fastcall FluxoMVA(int num_pat);
      double     __fastcall FluxoMVA(int ind_lig, int num_pat);
      void       __fastcall FluxoPQ(int num_pat, double &p, double &q);
      int        __fastcall IndiceBarra(int barra_id);
      int        __fastcall IndiceBarra(VTBarra *pbar);
      void       __fastcall LisBarra(TList *lisEQP);
      bool       __fastcall TipoLigacao(void);
      bool       __fastcall Vertical(void);

   public: //métodos que podem ser redefinidos
      virtual AnsiString __fastcall Alias(int ind_bar);
      virtual int        __fastcall Fases(VTBarra *barra);
      virtual int        __fastcall NumeroBarras(void);
      virtual void       __fastcall OrdenaBarras(VTBarra *bar_ref);

   protected:
      void __fastcall AlocaResFlowLig(int num_bar, int num_pat);
   };

//---------------------------------------------------------------------------
#endif
//eof

