//---------------------------------------------------------------------------
#ifndef TModificaH
#define TModificaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTModifica.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTCanal;
class VTCluster;
class VTEqbar;
class VTLigacao;
class VTMutua;
class VTRede;
class TRebuild;
class VTMercado;

//---------------------------------------------------------------------------
class TModifica : public VTModifica
   {
   public:  //property
      __property bool AfetaArea      = {read=PM_GetAfetaArea,      write=PM_SetAfetaArea};
      __property bool AfetaEqpto     = {read=PM_GetAfetaEqpto,     write=PM_SetAfetaEqpto};
      __property bool AfetaMercado   = {read=PM_GetAfetaMercado,   write=PM_SetAfetaMercado};
      __property bool AfetaTopologia = {read=PM_GetAfetaTopologia, write=PM_SetAfetaTopologia};

   public:
           __fastcall  TModifica(VTApl *apl);
           __fastcall ~TModifica(void);
      void __fastcall  DefineActionExterna(TList *lisACTION);
      bool __fastcall  Do(VTObra *obra, VTAcao *acao);
      bool __fastcall  Redo(VTObra *obra, VTAcao *acao);
	  bool __fastcall  Redo(TList *lisREDE, TList *lisACAO);
      bool __fastcall  Undo(VTObra *obra, VTAcao *acao);
      bool __fastcall  Undo(TList *lisREDE, TList *lisACAO);

   private:  //métodos acessados via property
      bool __fastcall PM_GetAfetaArea(void);
      bool __fastcall PM_GetAfetaEqpto(void);
      bool __fastcall PM_GetAfetaMercado(void);
      bool __fastcall PM_GetAfetaTopologia(void);
      void __fastcall PM_SetAfetaArea(bool afeta_area);
      void __fastcall PM_SetAfetaEqpto(bool afeta_eqpto);
      void __fastcall PM_SetAfetaMercado(bool afeta_mercado);
      void __fastcall PM_SetAfetaTopologia(bool afeta_topo);

   private: //métodos
      bool    __fastcall AlteraBarra(VTBarra* pbarra, VTBarra *copia_original, VTBarra *copia_alterada);
      bool    __fastcall AlteraCluster(VTCluster *cluster, VTCluster *copia_original, VTCluster *copia_alterada);
      bool    __fastcall AlteraEqbar(VTEqbar* eqbar, VTEqbar *copia_original, VTEqbar *copia_alterada);
      bool    __fastcall AlteraLigacao(VTLigacao* pliga, VTLigacao *copia_original, VTLigacao *copia_alterada);
      bool    __fastcall AlteraMutua(VTMutua* mutua, VTMutua *mutua_original, VTMutua *mutua_alterada);
	  bool    __fastcall AlteraRede(VTRede *rede, VTRede *copia_original, VTRede *copia_alterada);
//	  void    __fastcall AtualizaMultiObra(void);
      VTRede* __fastcall DeterminaRede(VTLigacao *pliga);
      bool    __fastcall Do(VTAcao *acao);
      bool    __fastcall DoAcao(VTAcao *acao);
      bool    __fastcall DoAcaoAltera(VTAcao *acao);
      bool    __fastcall DoAcaoComposta(VTAcao *acao);
      bool    __fastcall DoAcaoInsere(VTAcao *acao);
      bool    __fastcall DoAcaoRetira(VTAcao *acao);
	  //void    __fastcall DoVerificaAlteracaoRedeDeLigacao(VTAcao *acao);
	  bool    __fastcall IdentificaRedesVizinhasAfetadasDo(TList *lisREDE, TList *lisACAO);
	  bool    __fastcall IdentificaRedesVizinhasAfetadasUndo(TList *lisREDE, TList *lisACAO);
      bool    __fastcall InsereBarra(VTBarra* pbarra);
      bool    __fastcall InsereCanal(VTCanal *canal);
      bool    __fastcall InsereCluster(VTCluster *cluster);
      bool    __fastcall InsereEqbar(VTEqbar* eqbar);
      bool    __fastcall InsereLigacao(VTLigacao* pliga);
      bool    __fastcall InsereMutua(VTMutua* mutua);
	  bool    __fastcall InsereRede(VTRede *rede);
	  bool    __fastcall InsereRedeAfetada(TList *lisREDE, VTRede *rede);
      void    __fastcall LisRede(VTLigacao *pliga, TList *lisSEL);
      //bool    __fastcall Redo(VTAcao *acao);
      //bool    __fastcall RedoAcao(VTAcao *acao);
      //bool    __fastcall RedoAcaoAltera(VTAcao *acao);
      //bool    __fastcall RedoAcaoInsere(VTAcao *acao);
      //bool    __fastcall RedoAcaoRetira(VTAcao *acao);
      //bool    __fastcall RedoAcaoComposta(VTAcao *acao);
      bool    __fastcall RetiraBarra(VTBarra* pbarra);
      bool    __fastcall RetiraCanal(VTCanal *canal);
      bool    __fastcall RetiraCluster(VTCluster *cluster);
      bool    __fastcall RetiraEqbar(VTEqbar* eqbar);
      bool    __fastcall RetiraLigacao(VTLigacao* pliga);
      bool    __fastcall RetiraMutua(VTMutua* mutua);
      bool    __fastcall RetiraRede(VTRede *rede);
      void    __fastcall TransfereLigacao(VTRede *rede_orig, VTRede *rede_dest);
      void    __fastcall TransfereLigacao(VTRede *rede_orig, VTRede *rede_dest, VTLigacao *pliga);
      bool    __fastcall Undo(VTAcao *acao);
      bool    __fastcall UndoAcao(VTAcao *acao);
      bool    __fastcall UndoAcaoAltera(VTAcao *acao);
      bool    __fastcall UndoAcaoInsere(VTAcao *acao);
      bool    __fastcall UndoAcaoRetira(VTAcao *acao);
	  bool    __fastcall UndoAcaoComposta(VTAcao *acao);
	  bool	  __fastcall UndoAfetaTopologia(VTAcao *acao);
      //void    __fastcall UndoVerificaAlteracaoRedeDeLigacao(VTAcao *acao);
      void    __fastcall ReiniciaImpactoAcao(VTAcao *acao);
	  void    __fastcall VerificaImpactoAcao(VTAcao *acao);
      void    __fastcall VerificaIntegridade(void);

   private: //dados externos
      VTApl     *apl;
      VTObra    *obra;
      VTMercado *mercado;
      TList     *lisACTION;
      
   private: //dados locais
      TList    *lisTMP;
      TRebuild *rebuild;
      //controle de impacto da Acao executada
      struct   {
               bool  area;
               bool  eqpto;
               bool  mercado;
               bool  topologia;
               }afeta;
   };

#endif
//---------------------------------------------------------------------------
//eof

