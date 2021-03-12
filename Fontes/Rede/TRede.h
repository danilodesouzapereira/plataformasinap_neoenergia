//---------------------------------------------------------------------------
#ifndef TRedeH
#define TRedeH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "Estrutura.h"
#include "VTRede.h"

//---------------------------------------------------------------------------
class VTTipoRede;

//---------------------------------------------------------------------------
class TRede : public VTRede
   {
   public:
                  __fastcall  TRede(int num_pat);
                  __fastcall ~TRede(void);
      VTArea*     __fastcall  AreaEsquematico(void);
      VTArea*     __fastcall  AreaUtm_cm(void);
      VTBarra*    __fastcall  BarraFicticia(void);
      VTBarra*    __fastcall  BarraInicial(void);
      void        __fastcall  CalculaArea(void);
      void        __fastcall  Clear(void);
      VTRede*     __fastcall  Clone(void);
      double      __fastcall  Comprimento_km(VTArranjo *arranjo);
      void        __fastcall  CopiaAtributosDe(VTEqpto &ref);
      void        __fastcall  DefineBarraInicial(int barra_id);
      void        __fastcall  DefineBarraInicial(VTBarra *pbarra);
      void        __fastcall  DefineObjRedes(VTRedes *redes);
      bool        __fastcall  ExisteAlteracao(void);
      bool        __fastcall  ExisteBarra(VTBarra *pbarra);
      VTCorte*    __fastcall  ExisteCorte(VTRede *rede_viz);
      bool        __fastcall  ExisteGeracao(void);
      bool        __fastcall  ExisteLigacao(VTLigacao *pliga);
      VTLigacao*  __fastcall  ExisteLigacao(int tipo_eqpto, AnsiString codigo);
      VTLigacao*  __fastcall  ExisteLigacao(int tipo_eqpto, int eqpto_id);
      VTLigacao*  __fastcall  ExisteLigacao(VTBarra *barra);
      VTLigacao*  __fastcall  ExisteLigacao(VTBarra *barra1, VTBarra *barra2, VTBarra *barra3=NULL);
      bool        __fastcall  ExisteMutua(VTTrecho *trecho);
      void        __fastcall  IniciaLisBarra(void);
      void        __fastcall  IniciaLisLigacao(TList *lisEQP);
      void        __fastcall  InsereCorte(VTCorte *corte);
      void        __fastcall  InsereLigacao(VTLigacao *ligacao);
      void        __fastcall  InsereMutua(VTMutua *mutua);
      void        __fastcall  LisArranjo(TList *lisEQP);
      TList*      __fastcall  LisBarra(void);
      void        __fastcall  LisBarra(TList *lisEQP, bool barra_isolada=true);
      void        __fastcall  LisChave(TList *lisEQP, int estado=-1);
      TList*      __fastcall  LisCorte(void);
      void        __fastcall  LisCorte(TList *lisEQP);
      void        __fastcall  LisEqbar(TList *lisEQP, int tipo=-1, bool barra_isolada=true);
      void        __fastcall  LisEqpto(TList *lisEQP, int tipo=-1, bool barra_isolada=true);
      TList*      __fastcall  LisLigacao(void);
      void        __fastcall  LisLigacao(TList *lisEQP, int tipo=-1);
      void        __fastcall  LisLigacao(TList *lisEQP, VTBarra *barra);
      TList*      __fastcall  LisLigacaoEsquematico(void);
      TList*      __fastcall  LisMutua(void);
      void        __fastcall  LisMutua(TList *lisEQP);
      void        __fastcall  LisMutua(TList *lisEQP, VTTrecho *trecho);
      void        __fastcall  LisRedeVizinha(TList *lisEQP);
      int         __fastcall  NumeroDeBarras(void);
      int         __fastcall  NumeroDeConsumidor(int classe_tag);
      int         __fastcall  NumeroDeCortes(void);
      int         __fastcall  NumeroDeEqbar(int tipo);
      int         __fastcall  NumeroDeLigacoes(int tipo);
      void        __fastcall  ReiniciaLisCorte(void);
      void        __fastcall  RemoveBarra(VTBarra *pbarra);
      void        __fastcall  RemoveCorte(VTCorte *corte);
      void        __fastcall  RemoveLigacao(VTLigacao *ligacao);
      void        __fastcall  RemoveMutua(VTMutua *mutua);
      void        __fastcall  ReiniciaEsquematico(VTBarra *bar_fic, TList *lisLIG_FIC);
      int         __fastcall  Tipo(void);
      AnsiString  __fastcall  TipoAsString(void);

   protected:  //métodos acessados via property
      int         __fastcall PM_GetBarraInicialId(void);
      bool        __fastcall PM_GetCarregada(void);
      VTCluster*  __fastcall PM_GetCluster(void);
      TColor      __fastcall PM_GetColor(void);
      bool        __fastcall PM_GetEstatica(void);
      bool        __fastcall PM_GetFluxoOK(void);
      VTMRede*    __fastcall PM_GetMRede(void);
      AnsiString  __fastcall PM_GetPadrao(void);
      bool        __fastcall PM_GetRadial(void);
     VTRedes*    __fastcall PM_GetRedes(void);
     //VTRegiao*   __fastcall PM_GetRegiao(void);
      TDate       __fastcall PM_GetRevisao(void);
      VTTipoRede* __fastcall PM_GetTipoRede(void);
      bool        __fastcall PM_GetValida(void);
      void        __fastcall PM_SetBarraInicialId(int barra_id);
      void        __fastcall PM_SetCarregada(bool carregada);
      void        __fastcall PM_SetCluster(VTCluster *cluster);
      void        __fastcall PM_SetColor(TColor color);
      void        __fastcall PM_SetEstatica(bool estatica);
      void        __fastcall PM_SetFluxoOK(bool fluxoOK);
      void        __fastcall PM_SetMRede(VTMRede *mrede);
      void        __fastcall PM_SetPadrao(AnsiString padrao);
      void        __fastcall PM_SetRadial(bool radial);
      void        __fastcall PM_SetRedes(VTRedes *redes);
     //void        __fastcall PM_SetRegiao(VTRegiao *regiao);
     void        __fastcall PM_SetRevisao(TDate revisao);
      void        __fastcall PM_SetValida(bool valida);
      void        __fastcall PM_SetTipoRede(VTTipoRede *tipo_rede);

   private:  //métodos
      void __fastcall CalculaAreaEsquematico(void);
      void __fastcall CalculaAreaUtm(void);
      void __fastcall LisCanal(TList *lisEQP);

   private:  //dados externos
      VTBarra    *pbar_ini;   //Barra inicial (válido apenas p/ Rede radial)
      VTRedes    *redes;
      VTTipoRede *tipo_rede;

   private:  //dados acessados via property
      struct   {
               AnsiString padrao;
               bool       carregada;
               bool       estatica;
               bool       fluxoOK;
               bool       radial;
               bool       valida;  //{true: pode ser carregada; false: só existe sua Reducao}
               TColor     color;
               TDate      revisao;
              //VTRegiao   *regiao;
               }PD;

   private:  //dados locais
      #define clDefault clGray
      VTMRede *mrede;
	  TList   *lisBAR;    //lista de objetos VTBarra
	  TList   *lisBAR_NaoIso;    //lista de objetos VTBarra não isoladas
      TList   *lisLIG;    //lista de objetos VTLigacao
      TList   *lisMUT;    //lista de objetos VTMutua
      TList   *lisCOR;    //lista de objetos VTCorte
      struct   {//áreas da rede
               VTArea *esq;
               VTArea *utm;      //cm
               } area;
      //dados lidos da base de dados
      struct   {
               int barra_id; //ID da Barra inicial (somente p/ Rede radial)
               } bd;
      //estrutura p/ representação esquemática da rede reduzida
      strESQ_REDE  esq;
   };

//---------------------------------------------------------------------------
#endif
//eof
