//---------------------------------------------------------------------------
#ifndef VTRedesH
#define VTRedesH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTArea;
class VTApl;
class VTBarra;
class VTCorte;
class VTCluster;
class VTDominio;
class VTEqpto;
class VTMRede;
class VTRede;
class VTComentario;

//---------------------------------------------------------------------------
class VTRedes : public TObject
   {
   public:  //property
	  __property bool AllowRedefineIds 	  = {read=PM_GetAllowRedefineIds, write=PM_SetAllowRedefineIds};
	  __property TDateTime DataReferencia = {read=PM_GetDataReferencia};


   public:
                            __fastcall  VTRedes(void) {};
      virtual               __fastcall ~VTRedes(void) {};
      virtual VTArea*       __fastcall  AreaEsquematico(void) = 0;
      virtual VTArea*       __fastcall  AreaUtm_cm(void) = 0;
      virtual void          __fastcall  CalculaArea(void) = 0;
      //virtual void          __fastcall  Clear(bool inclusive_dominio=true) = 0;
      virtual void          __fastcall  Clear(bool inclusive_dominio=true, bool mantem_barras = false) = 0;
      virtual void          __fastcall  ClearEqptosIsolados(void) = 0;
      virtual void          __fastcall  DefineStatus(unsigned bitmask, bool status) = 0;
      virtual void          __fastcall  DiagEsquematicoAlignClient(void) = 0;
      virtual void          __fastcall  DiagEsquematicoResize(double fator=-1) = 0;
      virtual void          __fastcall  DiagEsquematicoResize(TList *lisBAR_SEL, double fator=-1) = 0;
      virtual void          __fastcall  DiagGeorefenciadoResize(double fator) = 0;
      virtual bool          __fastcall  ExisteAlteracao(void) = 0;
      virtual VTBarra*      __fastcall  ExisteBarra(int barra_id) = 0;
      virtual VTBarra*      __fastcall  ExisteBarra(AnsiString codigo) = 0;
      virtual VTBarra*      __fastcall  ExisteBarraCoordUtm(int utm_x, int utm_y) = 0;
      virtual VTCluster*    __fastcall  ExisteCluster(int cluster_id) = 0;
      virtual VTCluster*    __fastcall  ExisteCluster(AnsiString codigo) = 0;
      virtual VTComentario* __fastcall  ExisteComentario(VTEqpto *eqpto) = 0;
      virtual VTCorte*      __fastcall  ExisteCorte(int corte_id) = 0;
      virtual VTDominio*    __fastcall  ExisteDominio(int dominio_id) = 0;
      virtual VTDominio*    __fastcall  ExisteDominio(AnsiString codigo) = 0;
      virtual VTEqpto*      __fastcall  ExisteEqpto(int tipo_eqpto, AnsiString codigo) = 0;
      virtual VTEqpto*      __fastcall  ExisteEqpto(int tipo_eqpto, int eqpto_id) = 0;
      virtual VTMRede*      __fastcall  ExisteMRede(int mrede_id) = 0;
      virtual VTRede*       __fastcall  ExisteRede(int rede_id) = 0;
      virtual VTRede*       __fastcall  ExisteRede(AnsiString codigo) = 0;
      virtual void          __fastcall  InsereBarra(VTBarra *barra) = 0;
      virtual void          __fastcall  InsereCluster(VTCluster *cluster) = 0;
      virtual void          __fastcall  InsereComentario(VTComentario *comentario) = 0;
      virtual void          __fastcall  InsereCorte(VTCorte *corte) = 0;
      virtual void          __fastcall  InsereDominio(VTDominio *dominio) = 0;
      virtual void          __fastcall  InsereMRede(VTMRede *mrede) = 0;
      virtual void          __fastcall  InsereRede(VTRede *rede) = 0;
      virtual TList*        __fastcall  LisBarra(void) = 0;
      virtual void          __fastcall  LisBarra(TList *lisEQP) = 0;
      virtual void          __fastcall  LisBarraExclusiva(TList *lisEQP, VTRede *rede) = 0;
      virtual TList*        __fastcall  LisBarraIsolada(void) = 0;
      virtual TList*        __fastcall  LisCluster(void) = 0;
      virtual TList*        __fastcall  LisComentario(void) = 0;
      virtual TList*        __fastcall  LisCorte(void) = 0;
      virtual void          __fastcall  LisCorte(TList *lisEQP) = 0;
      virtual TList*        __fastcall  LisDominio(void) = 0;
      virtual void          __fastcall  LisDominio(TList *lisEXT) = 0;
      virtual void          __fastcall  LisDominio(TList *lisEXT, VTRede *rede) = 0;
      virtual void          __fastcall  LisEqbar(TList *lisEQP) = 0;
      virtual void          __fastcall  LisEqbar(TList *lisEQP, int tipo) = 0;
      virtual void          __fastcall  LisEqpto(TList *lisEQP) = 0;
      virtual void          __fastcall  LisEqpto(TList *lisEQP, int tipo) = 0;
      virtual void          __fastcall  LisEqptoIsolado(TList *lisEQP) = 0;
      virtual void          __fastcall  LisEqptoIsolado(TList *lisEQP, int tipo) = 0;
      virtual void          __fastcall  LisEqptoNaoIsolado(TList *lisEQP, int tipo) = 0;
      virtual void          __fastcall  LisLigacao(TList *lisEQP) = 0;
      virtual void          __fastcall  LisLigacao(TList *lisEQP, int tipo) = 0;
      virtual void          __fastcall  LisLigacao(TList *lisEQP, VTBarra *barra) = 0;
      virtual TList*        __fastcall  LisMNetFluxo(void) = 0;
      virtual TList*        __fastcall  LisMRede(void) = 0;
      virtual void          __fastcall  LisMRede(TList *lisEQP) = 0;
      virtual void          __fastcall  LisMRedeCarregada(TList *lisEQP) = 0;
      virtual void          __fastcall  LisMRedeReduzida(TList *lisEQP) = 0;
      virtual void          __fastcall  LisMutua(TList *lisEQP) = 0;
      virtual TList*        __fastcall  LisRede(void) = 0;
      virtual void          __fastcall  LisRede(TList *lisEQP) = 0;
      virtual void          __fastcall  LisRede(TList *lisEQP, int tipo_rede_id) = 0;
      virtual void          __fastcall  LisRedeCarregada(TList *lisEQP) = 0;
      virtual void          __fastcall  LisReducao(TList *lisEQP) = 0;
      virtual void          __fastcall  LisReducaoCarregada(TList *lisEQP) = 0;
	  virtual bool          __fastcall  RedefineCurvaTipicaCarga(int dia) = 0;
      virtual bool          __fastcall  RedefineIds(void) = 0;
	  virtual VTRede*       __fastcall  RedeIsolada(void) = 0;
      virtual void          __fastcall  ReiniciaLisBarra(TList *lisEQP) = 0;
      virtual void          __fastcall  ReiniciaLisBarraIsolada(void) = 0;
      virtual void          __fastcall  ReiniciaLisCorte(TList *lisEQP) = 0;
      virtual void          __fastcall  ReiniciaLisMNetFluxo(TList *lisEQP) = 0;
      virtual void          __fastcall  ReiniciaLisMRede(TList *lisEQP) = 0;
      virtual void          __fastcall  ReiniciaLisRede(TList *lisEQP) = 0;
      virtual void          __fastcall  RemoveBarra(VTBarra *barra) = 0;
      virtual void          __fastcall  RemoveCluster(VTCluster *cluster) = 0;
      virtual void          __fastcall  RemoveDominio(VTDominio *dominio) = 0;
      virtual void          __fastcall  RemoveRede(VTRede *rede) = 0;
      virtual bool          __fastcall  ValidaInsercaoGeracao(void) = 0;
      virtual bool          __fastcall  ViolaLimiteLicencaDemo(void) = 0;

   protected:  //nétodos acessados via property
	  virtual TDateTime __fastcall PM_GetDataReferencia(void) = 0;
	  virtual bool __fastcall PM_GetAllowRedefineIds(void) = 0;
	  virtual void __fastcall PM_SetAllowRedefineIds(bool allow) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRedes
//---------------------------------------------------------------------------
VTRedes* __fastcall NewObjRedes(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof

 