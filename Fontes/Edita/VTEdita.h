//---------------------------------------------------------------------------
#ifndef VTEditaH
#define VTEditaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ActnList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTAutoTag;
class VTBarra;
class VTChave;
class VTEqbar;
class VTEqpto;
class VTLigacao;
class VTMutua;
class VTRede;
class VTRedes;
class VTTrafo;
class VTTrecho;
class VTObra;

//---------------------------------------------------------------------------
class VTEdita : public TObject
   {
   public:  //property
	  __property VTAutoTag  *AutoTag = {read=PM_GetAutoTag};
	  __property bool       AutoTagHabilitado = {read=PM_GetAutoTagHabilitado, write=PM_SetAutoTagHabilitado};
	  __property AnsiString Nome     = {read=PM_GetNome};
	  __property VTObra     *Obra    = {read=PM_GetObra, write=PM_SetObra};


   public:
                   __fastcall  VTEdita(void) {};
                   __fastcall ~VTEdita(void) {};
      virtual void __fastcall  ActionOnChangeRedeInsere(TAction *OnChangeRede) = 0;
      virtual void __fastcall  ActionOnChangeRedeRetira(TAction *OnChangeRede) = 0;
      virtual bool __fastcall  Altera(VTEqpto *antes, VTEqpto *depois) = 0;
      virtual bool __fastcall  AlteraDiagEsqArea(VTRedes *redes, double fator) = 0;
      virtual bool __fastcall  AlteraDiagEsqArea(VTRedes *redes, TList *lisEQP, double fator) = 0;
      virtual bool __fastcall  AlteraDiagEsqIgualDiagGeo(VTRedes *redes) = 0;
      virtual bool __fastcall  AlteraDiagEsqInverteX(VTRedes *redes) = 0;
      virtual bool __fastcall  AlteraDiagEsqInverteY(VTRedes *redes) = 0;
      virtual bool __fastcall  AlteraDiagGeoArea(VTRedes *redes, double fator) = 0;
      virtual bool __fastcall  AlteraDivideTrecho(VTTrecho *tre_orig, VTTrecho *tre_alt, VTBarra *bar_ref, double dist_m) = 0;
      virtual bool __fastcall  AlteraLisEqpto(TList *lisEQP) = 0;
      virtual bool __fastcall  AlteraSincronismo(bool enabled) = 0;
	  virtual bool __fastcall  AlteraTapTrafo(VTTrafo *trafo, int variacao_tap) = 0;
	  virtual void __fastcall  Clear(void) = 0;
      virtual bool __fastcall  EqptoEditavel(VTEqpto *eqpto) = 0;
      virtual bool __fastcall  EqptoEditavel(TList *lisEXT) = 0;
      virtual bool __fastcall  InsereAlteraRetiraLisEqpto(TList *lisEQP_INS, TList *lisEQP_ALT, TList *lisEQP_RET) = 0;
      virtual bool __fastcall  InsereBarra(VTBarra *pbarra) = 0;
      virtual bool __fastcall  InsereEqbar(VTEqbar *eqbar, VTBarra *pbarra) = 0;
      virtual bool __fastcall  InsereLigacao(VTLigacao *pliga, VTRede *rede) = 0;
      virtual bool __fastcall  InsereLisEqpto(TList *lisEQP) = 0;
      virtual bool __fastcall  InsereMutua(VTMutua *mutua) = 0;
      virtual bool __fastcall  InsereObra(VTObra *obra) = 0;
      virtual bool __fastcall  InsereRede(VTRede *rede) = 0;
      virtual bool __fastcall  Manobra(VTChave *pchave) = 0;
      virtual bool __fastcall  Manobra(TList *lisCHV) = 0;
      virtual bool __fastcall  MoveBarra(VTBarra *pbarra, int dx, int dy, bool moving) = 0;
      virtual bool __fastcall  MoveBarra(VTBarra *pbarra, int x1, int y1, int x2, int y2, bool moving) = 0;
      virtual bool __fastcall  MoveBarra(TList *lisBAR, int dx, int dy, bool moving) = 0;
      virtual bool __fastcall  MoveEqbar(VTEqbar *eqbar, int x, int y, bool moving) = 0;
      virtual bool __fastcall  MoveLigacao(VTLigacao *pliga, VTBarra *pbar, int x, int y, bool moving) = 0;
      virtual bool __fastcall  RedoCompleto(void) = 0;
      virtual bool __fastcall  RedoLisObra(TList *lisOBRA) = 0;
	  virtual bool __fastcall  Retira(VTEqpto *eqpto) = 0;
	  virtual bool __fastcall  RetiraEqptosIsolados(bool apaga_tudo) = 0;
      virtual bool __fastcall  RetiraLisEqpto(TList *lisEXT) = 0;
      virtual bool __fastcall  RetiraLisRede(TList *lisEXT) = 0;
      virtual bool __fastcall  RotacionaBarras(VTBarra *barra_ref, double ang_grau) = 0;
      virtual bool __fastcall  TrocaBarraLigacao(VTLigacao *pliga, VTBarra *pbar_orig, VTBarra *pbar_nova) = 0;
      virtual bool __fastcall  TrocaBarraEqbar(VTEqbar *eqbar, VTBarra *pbar_nova) = 0;
      virtual bool __fastcall  UndoCompleto(void) = 0;
      virtual bool __fastcall  UndoLisObra(TList *lisOBRA) = 0;

   public:  //dados
      TAction *ActionRedo;
      TAction *ActionUndo;
      TAction *ActionUndoSemRedo;

   protected: //métodos acessados via property
	  virtual VTAutoTag* __fastcall PM_GetAutoTag(void) = 0;
	  virtual bool    	 __fastcall PM_GetAutoTagHabilitado(void) = 0;
	  virtual void       __fastcall PM_SetAutoTagHabilitado(bool habilitado) = 0;
      virtual AnsiString __fastcall PM_GetNome(void) = 0;
	  virtual VTObra*    __fastcall PM_GetObra(void) = 0;
	  virtual void       __fastcall PM_SetObra(VTObra *obra) = 0;



   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTEdita
//---------------------------------------------------------------------------
VTEdita* __fastcall NewObjEdita(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 