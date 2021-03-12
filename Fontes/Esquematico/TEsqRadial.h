//---------------------------------------------------------------------------
#ifndef TEsqRadialH
#define TEsqRadialH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEsqRadial.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTCorte;
class VTLigacao;
class VTMonta;

//---------------------------------------------------------------------------
class TEsqRadial : public VTEsqRadial
   {
   public:
               __fastcall TEsqRadial(VTApl *apl);
               __fastcall ~TEsqRadial(void);
      bool     __fastcall Executa(VTRede *rede, bool ordena);
      VTSecao* __fastcall ExisteSecao(VTBarra *barra);
      TList*   __fastcall LisBarra(void);
      TList*   __fastcall LisLigacao(void);
      TList*   __fastcall LisSecao(void);
      VTSecao* __fastcall SecaoFinal(void);
      VTSecao* __fastcall SecaoInicial(void);

   private: //métodos
      VTBarra*   __fastcall ExisteBarraGeracao(VTRede *rede);
      VTCorte*   __fastcall ExisteCorteDeSuprimento(VTRede *rede);
      VTLigacao* __fastcall ExisteLigacaoFilhaUnica(VTLigacao *pai, TList *lisLIG);
      VTSecao*   __fastcall ExisteSecao(VTLigacao *ligacao);
      void       __fastcall IniciaLisBarFixa(void);
      void       __fastcall InsereLigacao(VTBarra *barra, VTLigacao *ligacao_pai);
      void       __fastcall MontaSecaoRadial(bool ordena);
      void       __fastcall MontaUmaSecao(VTSecao *secao, VTLigacao *ligacao, TList *lisLIG);

   private: //dados externos
      VTApl   *apl;
      VTRede  *rede;

   private: //dados locais
      TList   *lisBAR;      //lista de Barras
      TList   *lisBAR_FIXA; //lista de Barras
      TList   *lisLIG;      //lista de Ligacoes
      TList   *lisREDU ;    //lista de Cortes
      TList   *lisSECAO;    //lista de Secoes
      TList   *lisTMP;      //lista p/ uso temporário
      struct   {
               TList *lisEQP;    //lista de Eqptos
               TList *lisLIG;    //lista de Ligacoes
               } tmp;
      struct   {
               bool radial;
               } diagnostico;
   };

//---------------------------------------------------------------------------
#endif
//eof

