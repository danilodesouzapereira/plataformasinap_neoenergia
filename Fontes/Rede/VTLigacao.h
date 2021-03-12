//---------------------------------------------------------------------------
#ifndef VTLigacaoH
#define VTLigacaoH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "Estrutura.h"
#include "VTEqpto.h"
#include "..\Constante\Const.h"

//---------------------------------------------------------------------------
enum enumDECLIVIDADE {//declividade das Ligacoes de 2 barras
                     ligaHORIZONTAL=0,      //y1 = y2
                     ligaVERTICAL,          //x1 = x2
                     ligaDIAG_ASCENDENTE,   //x1 < x2 e y1 < y2
                     ligaDIAG_DESCENDENTE}; //x1 < x2 e y1 > y2

                  
//---------------------------------------------------------------------------
class VTRede;
class VTBarra;
class VTResFlowLig;

//---------------------------------------------------------------------------
class VTLigacao : public VTEqpto
   {
   public:
                         __fastcall VTLigacao(void) {};
      virtual            __fastcall ~VTLigacao(void) {};
      virtual AnsiString __fastcall Alias(int ind_bar) = 0;
      virtual VTBarra*   __fastcall Barra(int ind_bar) = 0;
      virtual VTBarra*   __fastcall BarraComum(VTLigacao *ligacao) = 0;
      virtual VTLigacao* __fastcall Clone(void) = 0;
      virtual bool       __fastcall ConectaBarra(VTBarra *pbarra) = 0;
      virtual int        __fastcall ComprimentoEsq(void) = 0;
      virtual int        __fastcall ComprimentoUtm_cm(void) = 0;
      virtual int        __fastcall Declividade(void) = 0;
      virtual void       __fastcall DefineObjBarra(VTBarra *pbar1, VTBarra *pbar2, VTBarra *pbar3=NULL) = 0;
      virtual void       __fastcall DefineObjRede(VTRede *rede) = 0;
      virtual void       __fastcall DefinePosBar(VTBarra *pbar, double posbar) = 0;
      virtual void       __fastcall DefinePosBar(VTBarra *pbar, int x, int y) = 0;
      virtual int        __fastcall Fases(VTBarra *barra) = 0;
      virtual int        __fastcall IndiceBarra(int barra_id) = 0;
      virtual int        __fastcall IndiceBarra(VTBarra *pbar)= 0;
      virtual void       __fastcall LisBarra(TList *lisEQP) = 0;
      virtual int        __fastcall NumeroBarras(void) = 0;
      virtual void       __fastcall OrdenaBarras(VTBarra *bar_ref) = 0;
      virtual bool       __fastcall Vertical(void) = 0;

   public:  //dados externos
      VTRede    *rede;
      VTBarra   *pbarra1, *pbarra2, *pbarra3;
      VTLigacao *ligaPai;

   public:  //dados locais
      #define NBAR_LIG   3
      strESQ_LIGA  esq;               //informações p/ diagrama esquemático geral
      strESQ_BARRA esq_bar[NBAR_LIG]; //coordenadas p/ diagrama esquemático específico da rede
      VTResFlowLig *resflow;
   };

//---------------------------------------------------------------------------
#endif
//eof

