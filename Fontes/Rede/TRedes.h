//---------------------------------------------------------------------------
#ifndef TRedesH
#define TRedesH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTRedes.h"

//---------------------------------------------------------------------------
class VTListeqp;

//---------------------------------------------------------------------------
class TRedes : public VTRedes
   {
   public:
                    __fastcall  TRedes(VTApl *apl);
                    __fastcall ~TRedes(void);
      VTArea*       __fastcall  AreaEsquematico(void);
      VTArea*       __fastcall  AreaUtm_cm(void);
      void          __fastcall  CalculaArea(void);
      //void          __fastcall  Clear(bool inclusive_dominio);
      void          __fastcall  Clear(bool inclusive_dominio, bool mantem_barras);
      void          __fastcall  ClearEqptosIsolados(void);
      void          __fastcall  DefineStatus(unsigned bitmask, bool status);
      void          __fastcall  DiagEsquematicoAlignClient(void);
      void          __fastcall  DiagEsquematicoResize(double fator);
      void          __fastcall  DiagEsquematicoResize(TList *lisBAR_SEL, double fator);
      void          __fastcall  DiagGeorefenciadoResize(double fator);
      bool          __fastcall  ExisteAlteracao(void);
      VTBarra*      __fastcall  ExisteBarra(int barra_id);
      VTBarra*      __fastcall  ExisteBarra(AnsiString codigo);
      VTBarra*      __fastcall  ExisteBarraCoordUtm(int utm_x, int utm_y);
      VTCluster*    __fastcall  ExisteCluster(int cluster_id);
      VTCluster*    __fastcall  ExisteCluster(AnsiString codigo);
      VTComentario* __fastcall  ExisteComentario(VTEqpto *eqpto);
      VTCorte*      __fastcall  ExisteCorte(int corte_id);
      VTDominio*    __fastcall  ExisteDominio(int dominio_id);
      VTDominio*    __fastcall  ExisteDominio(AnsiString codigo);
      VTEqpto*      __fastcall  ExisteEqpto(int tipo_eqpto, AnsiString codigo);
      VTEqpto*      __fastcall  ExisteEqpto(int tipo_eqpto, int eqpto_id);
      VTMRede*      __fastcall  ExisteMRede(int mrede_id);
      VTRede*       __fastcall  ExisteRede(int rede_id);
      VTRede*       __fastcall  ExisteRede(AnsiString codigo);
      void          __fastcall  InsereBarra(VTBarra *barra);
      void          __fastcall  InsereCluster(VTCluster *cluster);
      void          __fastcall  InsereComentario(VTComentario *comentario);
      void          __fastcall  InsereCorte(VTCorte *corte);
      void          __fastcall  InsereDominio(VTDominio *dominio);
      void          __fastcall  InsereMRede(VTMRede *mrede);
      void          __fastcall  InsereRede(VTRede *rede);
      TList*        __fastcall  LisBarra(void);
      void          __fastcall  LisBarra(TList *lisEQP);
      void          __fastcall  LisBarraExclusiva(TList *lisEQP, VTRede *rede);
      TList*        __fastcall  LisBarraIsolada(void);
      TList*        __fastcall  LisCluster(void);
      TList*        __fastcall  LisComentario(void);
      TList*        __fastcall  LisCorte(void);
      void          __fastcall  LisCorte(TList *lisEQP);
      TList*        __fastcall  LisDominio(void);
      void          __fastcall  LisDominio(TList *lisEXT);
      void          __fastcall  LisDominio(TList *lisEXT, VTRede *rede);
      void          __fastcall  LisEqbar(TList *lisEQP);
      void          __fastcall  LisEqbar(TList *lisEQP, int tipo);
      void          __fastcall  LisEqpto(TList *lisEQP);
      void          __fastcall  LisEqpto(TList *lisEQP, int tipo);
      void          __fastcall  LisEqptoIsolado(TList *lisEQP);
      void          __fastcall  LisEqptoIsolado(TList *lisEQP, int tipo);
      void          __fastcall  LisEqptoNaoIsolado(TList *lisEQP, int tipo);
      void          __fastcall  LisLigacao(TList *lisEQP);
      void          __fastcall  LisLigacao(TList *lisEQP, int tipo);
      void          __fastcall  LisLigacao(TList *lisEQP, VTBarra *barra);
      TList*        __fastcall  LisMNetFluxo(void);
      TList*        __fastcall  LisMRede(void);
      void          __fastcall  LisMRede(TList *lisEQP);
      void          __fastcall  LisMRedeCarregada(TList *lisEQP);
      void          __fastcall  LisMRedeReduzida(TList *lisEQP);
      void          __fastcall  LisMutua(TList *lisEQP);
      TList*        __fastcall  LisRede(void);
      void          __fastcall  LisRede(TList *lisEQP);
      void          __fastcall  LisRede(TList *lisEQP, int tipo_rede_id);
      void          __fastcall  LisRedeCarregada(TList *lisEQP);
      void          __fastcall  LisReducao(TList *lisEQP);
      void          __fastcall  LisReducaoCarregada(TList *lisEQP);
	  bool          __fastcall  RedefineCurvaTipicaCarga(int dia);
	  bool          __fastcall  RedefineIds(void);
	  VTRede*       __fastcall  RedeIsolada(void);
	  void          __fastcall  ReiniciaLisBarra(TList *lisEQP);
      void          __fastcall  ReiniciaLisBarraIsolada(void);
      void          __fastcall  ReiniciaLisCorte(TList *lisEQP);
      void          __fastcall  ReiniciaLisMNetFluxo(TList *lisEQP);
      void          __fastcall  ReiniciaLisMRede(TList *lisEQP);
      void          __fastcall  ReiniciaLisRede(TList *lisEQP);
      void          __fastcall  RemoveBarra(VTBarra *barra);
      void          __fastcall  RemoveCluster(VTCluster *cluster);
      void          __fastcall  RemoveDominio(VTDominio *dominio);
      void          __fastcall  RemoveRede(VTRede *rede);
      bool          __fastcall  ValidaInsercaoGeracao(void);
      bool          __fastcall  ViolaLimiteLicencaDemo(void);

   protected:  //nétodos acessados via property
      TDateTime __fastcall PM_GetDataReferencia(void);
	  bool 		__fastcall PM_GetAllowRedefineIds(void);
	  void 		__fastcall PM_SetAllowRedefineIds(bool allow);
   private: //métodos
      void __fastcall DiagEsquematicoAjusta(void);
      void __fastcall DiagEsquematicoAreaDefault(void);
      void __fastcall CalculaAreaEsq(void);
      bool __fastcall CalculaAreaEsq(int &x1, int &y1, int &x2, int &y2);
      bool __fastcall CalculaAreaEsq(int &x1, int &y1, int &x2, int &y2, TList *lisBARRA,
                                     bool inclui_reducao=true);
	  void __fastcall CalculaAreaUtm(void);
	  void __fastcall LisCanal(TList *lisEQP);
	  bool __fastcall RedefineIds(TList *lisEQP);

   private: //dados externos
      VTApl *apl;

   private: //dados locais
      VTListeqp   *lisBAR;          //lista de objetos VTBarra
      TList       *lisCLUSTER;      //lista de objetos VTCluster
      TList       *lisREDE;         //lista de objetos VTRede
      TList       *lisCOR;          //lista de objetos VTCorte
      TList       *lisMR;           //lista de objetos VTMRede
      TList       *lisMNET;         //lista de objetos VTMRede ordenadas p/ cálculo parcial de fluxo
      TList       *lisDOMINIO;      //lista de objetos VTDominio
      TList       *lisCOMENTARIO;   //lista de objetos VTComentario
      TList       *lisTMP;          //lista p/ uso temporário interno
      struct   {
               VTRede *rede;        //rede usada p/ armazenar Barras e Ligacoes isoladas
               }isolada;
      struct   {//áreas da rede
               VTArea *esq;
               VTArea *utm;          //cm
               int    esq_width_min, esq_height_min;
               int    utm_width_min, utm_height_min;
			   } area;
	  struct
			  {
			  bool allowRedefineIds;
			  }PD;
   };

//---------------------------------------------------------------------------
#endif
//eof
