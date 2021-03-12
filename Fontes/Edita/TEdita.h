//---------------------------------------------------------------------------
#ifndef TEditaH
#define TEditaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEdita.h"

//---------------------------------------------------------------------------
class VTAcao;
class VTModifica;
class VTSincroniza;
class VTEsquematico;

//---------------------------------------------------------------------------
class TEdita : public VTEdita
   {
   public:
           __fastcall  TEdita(VTApl *apl);
           __fastcall ~TEdita(void);
      void __fastcall  ActionOnChangeRedeInsere(TAction *OnChangeRede);
      void __fastcall  ActionOnChangeRedeRetira(TAction *OnChangeRede);
      bool __fastcall  Altera(VTEqpto *antes, VTEqpto *depois);
      bool __fastcall  AlteraDiagEsqArea(VTRedes *redes, double fator);
      bool __fastcall  AlteraDiagEsqArea(VTRedes *redes, TList *lisEQP, double fator);
      bool __fastcall  AlteraDiagEsqIgualDiagGeo(VTRedes *redes);
      bool __fastcall  AlteraDiagEsqInverteX(VTRedes *redes);
      bool __fastcall  AlteraDiagEsqInverteY(VTRedes *redes);
      bool __fastcall  AlteraDiagGeoArea(VTRedes *redes, double fator);
      bool __fastcall  AlteraDivideTrecho(VTTrecho *tre_orig, VTTrecho *tre_alt, VTBarra *bar_ref, double dist_m);
      bool __fastcall  AlteraLisEqpto(TList *lisEQP);
      bool __fastcall  AlteraSincronismo(bool enabled);
		bool __fastcall  AlteraTapTrafo(VTTrafo *trafo, int variacao_tap);
		void __fastcall  Clear(void);
      bool __fastcall  EqptoEditavel(VTEqpto *eqpto);
      bool __fastcall  EqptoEditavel(TList *lisEXT);
      bool __fastcall  InsereAlteraRetiraLisEqpto(TList *lisEQP_INS, TList *lisEQP_ALT, TList *lisEQP_RET);
	  bool __fastcall  InsereBarra(VTBarra *pbarra);
      bool __fastcall  InsereEqbar(VTEqbar *eqbar, VTBarra *pbarra);
      bool __fastcall  InsereEqbarComChave(VTEqbar *eqbar, VTBarra *pbarra, VTChave *pchave, VTRede *rede);
      bool __fastcall  InsereLigacao(VTLigacao *pliga, VTRede *rede);
      bool __fastcall  InsereLisEqpto(TList *lisEQP);
      bool __fastcall  InsereMutua(VTMutua *mutua);
      bool __fastcall  InsereObra(VTObra *obra);
      bool __fastcall  InsereRede(VTRede *rede);
      bool __fastcall  Manobra(VTChave *pchave);
      bool __fastcall  Manobra(TList *lisCHV);
      bool __fastcall  MoveBarra(VTBarra *pbarra, int dx, int dy, bool moving);
      bool __fastcall  MoveBarra(VTBarra *pbarra, int x1, int y1, int x2, int y2, bool moving);
      bool __fastcall  MoveBarra(TList *lisBAR, int dx, int dy, bool moving);
      bool __fastcall  MoveEqbar(VTEqbar *eqbar,  int x, int y, bool moving);
      bool __fastcall  MoveLigacao(VTLigacao *pliga, VTBarra *pbar, int x, int y, bool moving);
      bool __fastcall  RedoCompleto(void);
      bool __fastcall  RedoLisObra(TList *lisOBRA);
	  bool __fastcall  Retira(VTEqpto *eqpto);
	  bool __fastcall  RetiraEqptosIsolados(bool apaga_tudo);
      bool __fastcall  RetiraLisEqpto(TList *lisEXT);
      bool __fastcall  RetiraLisRede(TList *lisEXT);
      bool __fastcall  RotacionaBarras(VTBarra *barra_ref, double ang_grau);
      bool __fastcall  TrocaBarraLigacao(VTLigacao *pliga, VTBarra *pbar_orig, VTBarra *pbar_nova);
      bool __fastcall  TrocaBarraEqbar(VTEqbar *eqbar, VTBarra *pbar_nova);
      bool __fastcall  UndoCompleto(void);
      bool __fastcall  UndoLisObra(TList *lisOBRA);

   protected: //métodos acessados via property
	  VTAutoTag* __fastcall PM_GetAutoTag(void);
	  bool    	 __fastcall PM_GetAutoTagHabilitado(void);
	  void       __fastcall PM_SetAutoTagHabilitado(bool habilitado);
	  AnsiString __fastcall PM_GetNome(void);
      VTObra*    __fastcall PM_GetObra(void);
      void       __fastcall PM_SetObra(VTObra *obra);


   private: //métodos
      void    __fastcall ActionRedoExecute(TObject *Sender);
      void    __fastcall ActionUndoExecute(TObject *Sender);
	  void    __fastcall ActionUndoSemRedoExecute(TObject *Sender);
	  void    __fastcall AtualizaMultiObra(void);
      bool    __fastcall BarraInicialDeRede(VTBarra *barra);
	  void    __fastcall CopiaLisOrigLisDest(TList *lisORIG, TList *lisDEST);
	  void    __fastcall CopiaTListRedes(TList *origem, TList* destino);
      TForm*  __fastcall EditorAtivo(void);
      VTAcao* __fastcall ExisteMoveEmAndamento(VTEqpto *eqpto);
      VTAcao* __fastcall ExisteMoveEmAndamento(TList *lisBAR);
      bool    __fastcall LisEqptosAloca(TList *lisEQP);
      void    __fastcall LisEqptosLibera(void);
      void    __fastcall MontaLisCanalMedidorRetira(VTEqpto *eqpto, TList *lisEQP);
      void    __fastcall MontaLisEqptoRetira(VTEqpto *eqpto, TList *lisEQP);
      bool    __fastcall RetiraLisEqptoSelecionado(TList *lisEQP);
      bool    __fastcall ValidaUndoRedoCompleto(VTAcao *acao);

   private: //dados externos
      VTApl *apl;

   private: //dados locais
      struct   {
               VTObra *obra_externa;
               VTObra *obra_local;
               }PD;
      VTModifica    *modifica;
      VTSincroniza  *sincroniza;
      VTEsquematico *esquematico;
      TList         *lisACTION;
      TList         *lisRET;
	  TList         *lisTMP;
      VTAutoTag     *autoTag;
      struct{
            TList  *lisCLUSTER;
            TList  *lisREDE;
            TList  *lisBARRA;
            TList  *lisEQP;
            }insere;
      struct{//listas para Redo/Undo aplicado a um conjunto de Acoes
            TList  *lisREDE;
            TList  *lisACAO;
            }ru;
   };

#endif
//---------------------------------------------------------------------------
//eof

