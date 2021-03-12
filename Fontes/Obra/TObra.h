//---------------------------------------------------------------------------
#ifndef TObraH
#define TObraH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTObra.h"

//---------------------------------------------------------------------------
class VTLixeira;

//---------------------------------------------------------------------------
class TObra : public VTObra
   {
   public:
              __fastcall  TObra(VTApl *apl);
              __fastcall ~TObra(void);
      void    __fastcall  AddAcao(VTAcao *acao);
      VTAcao* __fastcall  AddAcaoAltera(VTEqpto *original, VTEqpto *alterado, VTEqpto *copia_original);
      VTAcao* __fastcall  AddAcaoAltera(VTEqpto *original, VTEqpto *alterado, VTAcao *owner);
      VTAcao* __fastcall  AddAcaoComposta(void);
      VTAcao* __fastcall  AddAcaoInsere(VTEqpto *eqpto, VTAcao *owner);
	  VTAcao* __fastcall  AddAcaoRetira(VTEqpto *eqpto, VTAcao *owner);
	  VTItemObra* __fastcall AddItemObra(void);
	  bool	  __fastcall 	AddItemObra(VTItemObra *itemObra);
	  bool	  __fastcall  AddRede(VTRede *rede);
      void    __fastcall  CancelaRedo(void);
      void    __fastcall  CancelaUndo(void);
      void    __fastcall  Clear(void);
	  VTObra* __fastcall  Clone(TDuplicacao &duplicacao);
	  VTItemObra* __fastcall CriaItemObra(void);
      void    __fastcall  DefineActions(TAction *ActionUndo, TAction *ActionRedo);
	  void    __fastcall  DeleteRedo(VTAcao *acao);
	  bool    __fastcall  DeleteItemObra(VTItemObra *itemObra);
	  VTItemObra* __fastcall  ExisteItemObra(int id);
      void    __fastcall  FundeAcoes(void);
	  void    __fastcall  HabilitaActionsUndoRedo(void);
	  void    __fastcall  IniciaLisRede(TList *lisEXT);
      VTAcao* __fastcall  LastAcao(void);
	  VTAcao* __fastcall  LastAcao(int tipo, VTEqpto *eqpto);
	  TList*  __fastcall  LisRede(void);
      VTAcao* __fastcall  Redo(bool valida_acao_fixa);
		VTAcao* __fastcall  Undo(bool valida_acao_fixa);

   protected:  //métodos acessados via property
		bool       __fastcall PM_GetAlteraTaxaFalha(void);
      int        __fastcall PM_GetAnoAtivacao(void);
		int        __fastcall PM_GetAnoInicio(void);
		TColor     __fastcall PM_GetColor(void);
		bool       __fastcall PM_GetColorEnabled(void);
		double     __fastcall PM_GetCusto(void);
      TDateTime  __fastcall PM_GetDataAtivacao(void);
      TDateTime  __fastcall PM_GetDataInicio(void);
      bool       __fastcall PM_GetFixa(void);
      int        __fastcall PM_GetId(void);
      bool       __fastcall PM_GetIncorporada(void);
      TList*     __fastcall PM_GetLisAcao(void);
	  TList*     __fastcall PM_GetLisObject(void);
	  TList*     __fastcall PM_GetLisItemObra(void);
      VTLixeira* __fastcall PM_GetLixeira(void);
      int        __fastcall PM_GetModalResult(void);
      AnsiString __fastcall PM_GetNome(void);
      double     __fastcall PM_GetObEspecial(void);
      VTObras*   __fastcall PM_GetObras(void);
      double     __fastcall PM_GetPartOtr(void);
      AnsiString __fastcall PM_GetResponsavel(void);
      double     __fastcall PM_GetTaxaFalhaInsercao(void);
      double     __fastcall PM_GetTaxaFalhaSubstituicao(void);
      int        __fastcall PM_GetTipo(void);
      bool       __fastcall PM_GetVisible(void);
      //
      void       __fastcall PM_SetAlteraTaxaFalha(bool altera);
		void       __fastcall PM_SetColor(TColor color);
		void       __fastcall PM_SetColorEnabled(bool enabled);
		void       __fastcall PM_SetCusto(double custo);
      void       __fastcall PM_SetDataAtivacao(TDateTime data);
      void       __fastcall PM_SetDataInicio(TDateTime data);
      void       __fastcall PM_SetFixa(bool fixa);
      void       __fastcall PM_SetId(int id);
      void       __fastcall PM_SetIncorporada(bool incorporada);
      void       __fastcall PM_SetModalResult(int modal_result);
      void       __fastcall PM_SetNome(AnsiString nome);
      void       __fastcall PM_SetObEspecial(double ob_esp);
      void       __fastcall PM_SetObras(VTObras *obras);
      void       __fastcall PM_SetPartOtr(double part_otr);
      void       __fastcall PM_SetResponsavel(AnsiString responsavel);
      void       __fastcall PM_SetTaxaFalhaInsercao(double taxa);
      void       __fastcall PM_SetTaxaFalhaSubstituicao(double taxa);
      void       __fastcall PM_SetTipo(int tipo);
      void       __fastcall PM_SetVisible(bool visible);

   private:  //métodos
      void    __fastcall AtualizaMultiObra(void);
      void    __fastcall CopiaTListRedes(TList *origem, TList* destino);
      void    __fastcall EliminaAcao(TList *lisACAO, VTEqpto *eqpto, int acao_tipo=-1);
      VTAcao* __fastcall ExisteAcao(TList *lisACAO, VTEqpto *eqpto, int acao_tipo=-1);
      void    __fastcall FundeAcaoPosterior(VTAcao *acao, TList *lisDEST);
      void    __fastcall FundeAcoesRedo(void);
	  void    __fastcall FundeAcoesUndo(void);

   private:  //dados externos
		VTApl   *apl;
      TAction *ActionUndo;
      TAction *ActionRedo;

   private:  

   private:  //dados locais
      VTLixeira *lixeira;
      TList     *lisREDO;
      TList     *lisUNDO;
	  TList     *lisOBJ; //lista p/ objetos externos associados à Obra
	  TList     *lisREDE;
	  TList     *lisITEMOBRA;
   };

#endif
//---------------------------------------------------------------------------
//eof
