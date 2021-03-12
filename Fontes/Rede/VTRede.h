//---------------------------------------------------------------------------
#ifndef VTRedeH
#define VTRedeH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTEqpto.h"
#include "..\Constante\Const.h"

//---------------------------------------------------------------------------
class VTApl;
class VTArea;
class VTArranjo;
class VTBarra;
class VTCorte;
class VTCluster;
class VTDadosExt;
class VTLigacao;
class VTMutua;
class VTTrecho;
class VTRedes;
class VTTipoRede;
class VTEqpto;
class VTMRede;

//---------------------------------------------------------------------------
class VTRede : public VTEqpto
   {
   public:  //property
      __property int         BarraInicialId = {read=PM_GetBarraInicialId, write=PM_SetBarraInicialId};
      __property VTCluster*  Cluster        = {read=PM_GetCluster,        write=PM_SetCluster};
      __property VTMRede*    MRede          = {read=PM_GetMRede,          write=PM_SetMRede};
      __property VTRedes*    Redes          = {read=PM_GetRedes,          write=PM_SetRedes};
//      __property VTRegiao*   Regiao         = {read=PM_GetRegiao,         write=PM_SetRegiao};
      __property VTTipoRede* TipoRede       = {read=PM_GetTipoRede,       write=PM_SetTipoRede};
      __property TDate       Revisao        = {read=PM_GetRevisao,        write=PM_SetRevisao};
      __property TColor      Color          = {read=PM_GetColor,          write=PM_SetColor};
      __property bool        Carregada      = {read=PM_GetCarregada,      write=PM_SetCarregada};
      __property bool        Estatica       = {read=PM_GetEstatica,       write=PM_SetEstatica};
      __property bool        FluxoOK        = {read=PM_GetFluxoOK,        write=PM_SetFluxoOK};
      __property bool        Radial         = {read=PM_GetRadial,         write=PM_SetRadial};
      __property bool        Valida         = {read=PM_GetValida,         write=PM_SetValida};

   public:
								  __fastcall  VTRede(void) {};
        virtual             __fastcall ~VTRede(void) {};
		virtual VTArea*     __fastcall  AreaEsquematico(void) = 0;
		virtual VTArea*     __fastcall  AreaUtm_cm(void) = 0;
		virtual VTBarra*    __fastcall  BarraFicticia(void) = 0;
		virtual VTBarra*    __fastcall  BarraInicial(void) = 0;
		virtual void        __fastcall  Clear(void) = 0;
		virtual void        __fastcall  CalculaArea(void) = 0;
		virtual VTRede*     __fastcall  Clone(void) = 0;
		virtual double      __fastcall  Comprimento_km(VTArranjo *arranjo=NULL) = 0;
		virtual void        __fastcall  DefineBarraInicial(VTBarra *pbarra) = 0;
		virtual bool        __fastcall  ExisteAlteracao(void) = 0;
		virtual bool        __fastcall  ExisteBarra(VTBarra *pbarra) = 0;
		virtual VTCorte*    __fastcall  ExisteCorte(VTRede *rede_viz) = 0;
		virtual bool        __fastcall  ExisteGeracao(void) = 0;
		virtual bool        __fastcall  ExisteLigacao(VTLigacao *pliga) = 0;
		virtual VTLigacao*  __fastcall  ExisteLigacao(int tipo_eqpto, AnsiString codigo) = 0;
		virtual VTLigacao*  __fastcall  ExisteLigacao(int tipo_eqpto, int eqpto_id) = 0;
		virtual VTLigacao*  __fastcall  ExisteLigacao(VTBarra *barra) = 0;
		virtual VTLigacao*  __fastcall  ExisteLigacao(VTBarra *barra1, VTBarra *barra2, VTBarra *barra3=NULL) = 0;
		virtual bool        __fastcall  ExisteMutua(VTTrecho *trecho) = 0;
		virtual void        __fastcall  IniciaLisBarra(void) = 0;
		virtual void        __fastcall  IniciaLisLigacao(TList *lisEQP) = 0;
		virtual void        __fastcall  InsereCorte(VTCorte *corte) = 0;
		virtual void        __fastcall  InsereLigacao(VTLigacao *ligacao) = 0;
		virtual void        __fastcall  InsereMutua(VTMutua *mutua) = 0;
		virtual void        __fastcall  LisArranjo(TList *lisEQP) = 0;
		virtual TList*      __fastcall  LisBarra(void) = 0;
		virtual void        __fastcall  LisBarra(TList *lisEQP, bool barra_isolada=true) = 0;
		virtual void        __fastcall  LisChave(TList *lisEQP, int estado=-1) = 0;
		virtual TList*      __fastcall  LisCorte(void) = 0;
		virtual void        __fastcall  LisCorte(TList *lisEQP) = 0;
		virtual void        __fastcall  LisEqbar(TList *lisEQP, int tipo=-1, bool barra_isolada=true) = 0;
		virtual void        __fastcall  LisEqpto(TList *lisEQP, int tipo=-1, bool barra_isolada=true) = 0;
		virtual TList*      __fastcall  LisLigacao(void) = 0;
		virtual void        __fastcall  LisLigacao(TList *lisEQP, int tipo=-1) = 0;
		virtual void        __fastcall  LisLigacao(TList *lisEQP, VTBarra *barra) = 0;
		virtual TList*      __fastcall  LisLigacaoEsquematico(void) = 0;
		virtual TList*      __fastcall  LisMutua(void) = 0;
		virtual void        __fastcall  LisMutua(TList *lisEQP) = 0;
		virtual void        __fastcall  LisMutua(TList *lisEQP, VTTrecho *trecho) = 0;
		virtual void        __fastcall  LisRedeVizinha(TList *lisEQP) = 0;
		virtual int         __fastcall  NumeroDeBarras(void) = 0;
		virtual int         __fastcall  NumeroDeConsumidor(int classe_tag=-1) = 0;
		virtual int         __fastcall  NumeroDeCortes(void) = 0;
		virtual int         __fastcall  NumeroDeEqbar(int tipo=-1) = 0;
		virtual int         __fastcall  NumeroDeLigacoes(int tipo=-1) = 0;
		virtual void        __fastcall  ReiniciaLisCorte(void) = 0;
		virtual void        __fastcall  RemoveBarra(VTBarra *pbarra) = 0;
		virtual void        __fastcall  RemoveCorte(VTCorte *corte) = 0;
		virtual void        __fastcall  RemoveLigacao(VTLigacao *ligacao) = 0;
		virtual void        __fastcall  RemoveMutua(VTMutua *mutua) = 0;
		virtual void        __fastcall  ReiniciaEsquematico(VTBarra *bar_fic=NULL, TList *lisLIG_FIC=NULL) = 0;

   protected:  //métodos acessados via property
      virtual int         __fastcall PM_GetBarraInicialId(void) = 0;
      virtual bool        __fastcall PM_GetCarregada(void) = 0;
      virtual VTCluster*  __fastcall PM_GetCluster(void) = 0;
      virtual TColor      __fastcall PM_GetColor(void) = 0;
      virtual bool        __fastcall PM_GetEstatica(void) = 0;
      virtual bool        __fastcall PM_GetFluxoOK(void) = 0;
      virtual VTMRede*    __fastcall PM_GetMRede(void) = 0;
		virtual bool        __fastcall PM_GetRadial(void) = 0;
		virtual VTRedes*    __fastcall PM_GetRedes(void) = 0;
//      virtual VTRegiao*   __fastcall PM_GetRegiao(void) = 0;
		virtual TDate       __fastcall PM_GetRevisao(void) = 0;
      virtual VTTipoRede* __fastcall PM_GetTipoRede(void) = 0;
      virtual bool        __fastcall PM_GetValida(void) = 0;
      virtual void        __fastcall PM_SetBarraInicialId(int barra_id) = 0;
      virtual void        __fastcall PM_SetCarregada(bool carregada) = 0;
      virtual void        __fastcall PM_SetColor(TColor color) = 0;
      virtual void        __fastcall PM_SetCluster(VTCluster *cluster) = 0;
      virtual void        __fastcall PM_SetEstatica(bool estatica) = 0;
      virtual void        __fastcall PM_SetFluxoOK(bool fluxoOK) = 0;
      virtual void        __fastcall PM_SetMRede(VTMRede *mrede) = 0;
      virtual void        __fastcall PM_SetRadial(bool radial) = 0;
      virtual void        __fastcall PM_SetRedes(VTRedes *redes) = 0;
//      virtual void        __fastcall PM_SetRegiao(VTRegiao *regiao) = 0;
		virtual void        __fastcall PM_SetRevisao(TDate revisao) = 0;
      virtual void        __fastcall PM_SetValida(bool valida) = 0;
      virtual void        __fastcall PM_SetTipoRede(VTTipoRede *tipo_rede) = 0;

   public: //dados
      VTCluster  *cluster;
      VTDadosExt *dados_ext;
      struct {
             DynamicArray<bool> flow;
             bool curto;
             } diag;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRede
//---------------------------------------------------------------------------
VTRede* __fastcall NewObjRede(int num_pat);

//---------------------------------------------------------------------------
#endif
//eof


 
