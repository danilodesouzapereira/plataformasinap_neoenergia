//---------------------------------------------------------------------------
#ifndef TEsqMalhaH
#define TEsqMalhaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEsqMalha.h"

//---------------------------------------------------------------------------
class VTLigacao;
class VTMonta;
class VTMalhas;

//---------------------------------------------------------------------------
class TEsqMalha : public VTEsqMalha
   {
   public:
               __fastcall TEsqMalha(VTApl *apl);
               __fastcall ~TEsqMalha(void);
      bool     __fastcall Executa(VTRede *rede);
      VTSecao* __fastcall ExisteSecao(VTBarra *barra);
      TList*   __fastcall LisBarra(void);
      TList*   __fastcall LisLigacao(void);
      TList*   __fastcall LisSecao(void);

   private: //métodos
      VTLigacao* __fastcall ExisteLigacaoFilhaUnica(VTLigacao *pai, TList *lisLIG);
      VTSecao*   __fastcall ExisteSecao(VTLigacao *ligacao);
      void       __fastcall InsereLigacao(VTBarra *barra, VTLigacao *ligacao_pai);
      void       __fastcall IniciaLisBarFixa(void);
      void       __fastcall IniciaLisBarInicial(VTRede *rede, TList *lisBM);
      void       __fastcall MontaSecaoMalha(void);
      void       __fastcall MontaSecaoRadial(bool ordena);
      void       __fastcall MontaUmaSecao(VTSecao *secao, VTLigacao *ligacao, TList *lisLIG);

   private: //dados externos
      VTApl   *apl;
      VTRede  *rede;

   private: //dados locais
      VTMalhas *malhas;
      TList    *lisBAR;      //lista de Barras
      TList    *lisBAR_FIXA; //lista de Barras
      TList    *lisBAR_INI;  //lista de Barras iniciais da rede
      TList    *lisLIG;      //lista de Ligacoes
      TList    *lisSECAO;    //lista de Secoes
      TList    *lisREDU;     //lista de Reducoes vizinhas da Rede
      struct   {
               TList *lisLIG;    //lista de Ligacoes
               } tmp;
      struct   {
               bool radial;
               } diagnostico;
   };

//---------------------------------------------------------------------------
#endif
//eof

