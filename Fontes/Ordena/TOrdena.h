//---------------------------------------------------------------------------
#ifndef TOrdenaH
#define TOrdenaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTOrdena.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTLigacao;
class VTMonta;
class VTMontaZ;

//---------------------------------------------------------------------------
class TOrdena : public VTOrdena
   {
   public:
             __fastcall  TOrdena(VTApl *apl);
             __fastcall ~TOrdena(void);
      VTNet* __fastcall  Executa(TList *lisBAR, TList *lisEQB, TList *lisLIG, TList *lisMUTUA);
      VTNet* __fastcall  Executa(VTMNet *mnet);
      VTNet* __fastcall  Executa(VTRede *rede);
      VTNet* __fastcall  Executa(VTRedes *redes);
      bool   __fastcall  Executa(VTNet *net);
      void   __fastcall  MontaGrupoMutuaDefinidaPorArranjo(void);

   private: //métodos
      void     __fastcall DefineFasesNo(void);
      void     __fastcall DefineTipoNo(void);
      bool     __fastcall Executa(void);
      VTBarra* __fastcall ExisteBarraGerSupPV(TList *lisGS);
      VTBarra* __fastcall ExisteBarraGerSupVF(TList *lisGS);
      bool     __fastcall LigacaoChaveFechadaReguladorBypass(VTLigacao *ligacao);
      void     __fastcall LimpaListas(void);
      void     __fastcall OrdenaBarraLigacao(void);
      void     __fastcall OrdenaBarraLigacao(VTBarra *pbarra);
      void     __fastcall MontaEqbar(void);
      void     __fastcall MontaLisLigacaoDaBarra(VTBarra *barra, TList *lisLIG1, TList *lisLIG2, TList *lisLB);
      void     __fastcall MontaMatZ(void);
      void     __fastcall MontaNo(void);
      void     __fastcall MontaNoParaCadaBarra(void);
      void     __fastcall MontaTramo(void);
      //void     __fastcall ReiniciaBarras(TList *lisEQP);
      void     __fastcall RemoveLigacoesDeNo(void);
      void     __fastcall RetiraChaveAberta(TList *lisEQP);
      void     __fastcall TransfereBarra(VTBarra *pbarra);
      //void     __fastcall VerificaMRT(VTNet *net, TList *lisREDE);

   private: //dados externos
      VTApl   *apl;
      //VTRede *rede;

   private: //dados locais
      bool     add_isolado;
      VTMonta  *monta;
      VTMontaZ *montaZ;
      VTNet   *net;
      struct   {
               TList   *lisBAR;    //lista de Barras
               TList   *lisEQB;    //lista de todos Eqbar
               TList   *lisEQP;    //lista de equipamentos
               TList   *lisLIG;    //lista de Ligacoes
               TList   *lisMUT;    //lista de Mutuas
               TList   *lisGS;     //lista de Geradores e Suprimentos
               TList   *lisSWING;  //lista de Barras swings
               } tmp;
   };

//---------------------------------------------------------------------------
#endif
//eof

