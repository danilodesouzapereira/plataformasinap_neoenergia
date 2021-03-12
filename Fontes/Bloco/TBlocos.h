//---------------------------------------------------------------------------
#ifndef TBlocosH
#define TBlocosH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBlocos.h"

//---------------------------------------------------------------------------
//class

//---------------------------------------------------------------------------
class TBlocos : public VTBlocos
   {
   public:
               __fastcall  TBlocos(void);
               __fastcall ~TBlocos(void);
      bool     __fastcall  EliminaBloco(VTBloco *bloco);
      bool     __fastcall  EliminaLisBloco(TList *lisEXT);
      bool     __fastcall  EliminaBlocos(TList *lisEXT);
      bool     __fastcall  EliminaBlocosNaoEnergizados(void);
      bool     __fastcall  EliminaElosComBlocosIguais(void);
      bool     __fastcall  EliminaElosComChaveAberta(void);
      bool     __fastcall  Executa(VTEqpto *eqpto);
      bool     __fastcall  Executa(VTRede *rede);
      bool     __fastcall  Executa(VTRedes *redes);
      VTBloco* __fastcall  ExisteBloco(VTBarra *barra);
      VTBloco* __fastcall  ExisteBloco(VTLigacao *ligacao);
      VTElo*   __fastcall  ExisteElo(VTBloco* bloco1, VTBloco* bloco2);
      VTElo*   __fastcall  ExisteElo(VTChave *chave);
      bool     __fastcall  FundeBlocos(VTBloco* bloco, TList* lisEXT);         //hk
      bool     __fastcall  FundeBlocos(VTElo *elo);
      TList*   __fastcall  LisBloco(void);
      void     __fastcall  LisBlocoEnergizado(TList *lisEXT);
      void     __fastcall  LisBlocoGeracao(TList *lisEXT);
      void     __fastcall  LisBlocoGeracao(VTRede*rede, TList *lisEXT);
      void     __fastcall  LisBlocoNaoEnergizado(TList *lisEXT);
      void     __fastcall  LisBlocoRede(VTRede *rede, TList *lisEXT);
      void     __fastcall  LisBlocoVizinho(VTBloco *bloco, TList *lisEXT);
      void     __fastcall  LisBlocoVizinho1(VTBloco *bloco, TList *lisEXT, TList* lisCHV);//hk
      void     __fastcall  LisBlocoVizinhoConectado(VTBloco *bloco, TList *lisEXT);
      void     __fastcall  LisChaveBloco(VTBarra *barra, TList *lisEXT);
      void     __fastcall  LisChaveBloco(VTBloco *bloco, TList *lisEXT);
      void     __fastcall  LisChaveBloco(VTLigacao *ligacao, TList *lisEXT);
      void     __fastcall  LisChaveFronteira(VTBarra *barra, TList *lisEXT);
      void     __fastcall  LisChaveFronteira(VTBloco *bloco, TList *lisEXT);
      void     __fastcall  LisChaveFronteira(VTLigacao *ligacao, TList *lisEXT);
      void     __fastcall  LisChaveFronteira(VTRede *rede, TList *lisEXT);
      void     __fastcall  LisChaveInterna(VTRede *rede, TList *lisEXT);
      TList*   __fastcall  LisElo(void);
      void     __fastcall  LisElo(VTBloco *bloco, TList *lisEXT);
      void     __fastcall  LisEloFronteira(VTBloco *bloco, TList *lisEXT);
      void     __fastcall  LisEloFronteira(VTRede *rede, TList *lisEXT);
      void     __fastcall  LisEloFronteiraSuper(VTBloco *bloco, TList *lisEXT);
      void     __fastcall  LisEloInterno(VTBloco *bloco, TList *lisEXT);
      void     __fastcall  LisEloInterno(VTRede *rede, TList *lisEXT);
      void     __fastcall  LisRedeVizinha(VTBloco *bloco, TList *lisEXT);

   private: //métodos
      VTBloco* __fastcall ExisteBloco(VTChave *chave);
      bool     __fastcall FundeBloco2noBloco1(VTBloco* bloco1, VTBloco* bloco2);
      bool     __fastcall MontaBloco(TList *lisBARRA, TList *lisLIGACAO);
      bool     __fastcall MontaElo(TList *lisBLOCO, TList *lisCHAVE);

   private: //objetos externos
      //VTRedes *redes;

   private: //dados locais
      //listas usadas internamente
      TList      *lisBAR;
      TList      *lisBLO;
      TList      *lisCHV;
      TList      *lisELO;
      TList      *lisLIG;
      //listas de uso temporário
      TList      *lisTMP;
      TList      *lisVIZ;
   };

#endif
//---------------------------------------------------------------------------
//eof
