//---------------------------------------------------------------------------
#ifndef TTroncoH
#define TTroncoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTTronco.h"

//---------------------------------------------------------------------------
class VTArranjo;
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
      void       __fastcall DefinePrimario(VTPrimario *primario, int ind_pat);
      AnsiString __fastcall CaboRamalBifasico(void);
      AnsiString __fastcall CaboRamalMonofasico(void);
      AnsiString __fastcall CaboRamalTrifasico(void);
      AnsiString __fastcall CaboTroncoBifasico(void);
      AnsiString __fastcall CaboTroncoMonofasico(void);
      AnsiString __fastcall CaboTroncoTrifasico(void);
      int        __fastcall CompRamalBifasico(void);
      int        __fastcall CompRamalMonofasico(void);
      int        __fastcall CompRamalTrifasico(void);
      int        __fastcall CompTroncoBifasico(void);
      int        __fastcall CompTroncoMonofasico(void);
      int        __fastcall CompTroncoTrifasico(void);
      TList*     __fastcall LisBarraTronco(void);
		TList*     __fastcall LisLigacaoRamal(void);
		TList*     __fastcall LisLigacaoTronco(void);
      double     __fastcall MaiorTensaoTronco(int ind_pat);
      double     __fastcall MenorTensaoTronco(int ind_pat);

   private: //métodos
      bool       __fastcall ExisteLigacaoQueConectaBarra(VTBarra *barra, TList *lisORIG, TList *lisDEST);
      VTLigacao* __fastcall ExisteLigacaoTronco(TList *lisLIG);
      void       __fastcall IdentificaTroncoRamal(void);
      void       __fastcall IniciaDadosRamalBifasico(void);
      void       __fastcall IniciaDadosRamalMonofasico(void);
      void       __fastcall IniciaDadosRamalTrifasico(void);
      void       __fastcall IniciaDadosTroncoBifasico(void);
      void       __fastcall IniciaDadosTroncoMonofasico(void);
      void       __fastcall IniciaDadosTroncoTrifasico(void);
      int        __fastcall NumeroFases(VTArranjo *arranjo);
      void       __fastcall SalvaLigacaoRamal(TList *lisLIG);
      void       __fastcall SelecionaTrecho(int num_fase, TList *lisORIG, TList *lisDEST);
      VTCabo*    __fastcall SelecionaCaboDeMaiorCorrente(TList *lisTRE);
      double     __fastcall SomaComprimento(TList *lisTRE);
      bool       __fastcall ValidaLigacaoTronco(VTLigacao *ligacao);

   private: //objetos externos
      VTApl      *apl;
      VTPrimario *primario;
      //VTOrdena   *ordena;

   private: //dados locais
      int       ind_pat;
      TList     *lisBARRA;
      TList     *lisRAMAL;
      TList     *lisTRONCO;
      TList     *lisLIGACAO;
      TList     *lisTMP;
      VTLigacao *lig_ramal_imax;
      struct   {
               struct   {
                        AnsiString cabo;
                        int        comp;
                        }mon, bif, tri;
               }ramal, tronco;
   };

#endif
//---------------------------------------------------------------------------
//eof