//---------------------------------------------------------------------------
#ifndef TTroncoH
#define TTroncoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTTronco.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTCabo;
class VTLigacao;
//class VTOrdena;

//---------------------------------------------------------------------------
class TTronco : public VTTronco
   {
   public:
                 __fastcall TTronco(VTApl *apl);
                 __fastcall ~TTronco(void);
      VTArranjo* __fastcall ArranjoRamalBifasico(void);
      VTArranjo* __fastcall ArranjoRamalMonofasico(void);
      VTArranjo* __fastcall ArranjoRamalTrifasico(void);
      VTArranjo* __fastcall ArranjoTroncoBifasico(void);
      VTArranjo* __fastcall ArranjoTroncoMonofasico(void);
      VTArranjo* __fastcall ArranjoTroncoTrifasico(void);
      VTCabo*    __fastcall CaboRamalBifasico(void);
      VTCabo*    __fastcall CaboRamalMonofasico(void);
      VTCabo*    __fastcall CaboRamalTrifasico(void);
      VTCabo*    __fastcall CaboTroncoBifasico(void);
      VTCabo*    __fastcall CaboTroncoMonofasico(void);
      VTCabo*    __fastcall CaboTroncoTrifasico(void);
      AnsiString __fastcall CodigoCaboRamalBifasico(void);
      AnsiString __fastcall CodigoCaboRamalMonofasico(void);
      AnsiString __fastcall CodigoCaboRamalTrifasico(void);
      AnsiString __fastcall CodigoCaboTroncoBifasico(void);
      AnsiString __fastcall CodigoCaboTroncoMonofasico(void);
      AnsiString __fastcall CodigoCaboTroncoTrifasico(void);
      int        __fastcall CompRamalBifasico(void);
      int        __fastcall CompRamalMonofasico(void);
      int        __fastcall CompRamalTrifasico(void);
      int        __fastcall CompTroncoBifasico(void);
      int        __fastcall CompTroncoMonofasico(void);
      int        __fastcall CompTroncoTrifasico(void);
      bool       __fastcall DefinePrimario(VTPrimario *primario, int ind_pat);
      bool       __fastcall DefineSecundario(VTSecundario *secundario, int ind_pat);
      TList*     __fastcall LisBarraTronco(void);
      TList*     __fastcall LisLigacaoRamal(void);
      TList*     __fastcall LisLigacaoTronco(void);
      double     __fastcall MaiorTensaoTronco(int ind_pat);
      double     __fastcall MenorTensaoTronco(int ind_pat);

   private: //métodos
      bool       __fastcall ExisteLigacaoQueConectaBarra(VTBarra *barra, TList *lisORIG, TList *lisDEST);
      VTLigacao* __fastcall ExisteLigacaoTronco(TList *lisLIG);
      bool       __fastcall IdentificaTroncoRamal(VTPrimario *primario);
      bool       __fastcall IdentificaTroncoRamal(VTSecundario *secundario);
      void       __fastcall IniciaDadosRamalBifasico(void);
      void       __fastcall IniciaDadosRamalMonofasico(void);
      void       __fastcall IniciaDadosRamalTrifasico(void);
      void       __fastcall IniciaDadosTroncoBifasico(void);
      void       __fastcall IniciaDadosTroncoMonofasico(void);
      void       __fastcall IniciaDadosTroncoTrifasico(void);
      int        __fastcall NumeroFases(VTArranjo *arranjo);
      void       __fastcall SalvaLigacaoRamal(TList *lisLIG);
      VTArranjo* __fastcall SelecionaArranjoDeMaiorCorrente(TList *lisTRE);
      VTCabo*    __fastcall SelecionaCaboDeMaiorCorrente(VTArranjo *arranjo);
      void       __fastcall SelecionaTrecho(int num_fase, TList *lisORIG, TList *lisDEST);
      double     __fastcall SomaComprimento(TList *lisTRE);
      bool       __fastcall ValidaLigacaoTronco(VTLigacao *ligacao);

   private: //objetos externos
      VTApl      *apl;

   private: //dados locais
      int       ind_pat;
      TList     *lisBARRA;
      TList     *lisRAMAL;
      TList     *lisTRONCO;
      TList     *lisLIGACAO;
      TList     *lisTMP;
      VTLigacao *lig_ramal_imax;
      VTLigacao *lig_inicial;
      struct   {
               struct   {
                        VTArranjo *arranjo;
                        VTCabo    *cabo;
                        int        comp;
                        }mon, bif, tri;
               }ramal, tronco;
   };

#endif
//---------------------------------------------------------------------------
//eof