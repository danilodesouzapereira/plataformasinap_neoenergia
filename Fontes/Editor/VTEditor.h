//---------------------------------------------------------------------------
#ifndef VTEditorH
#define VTEditorH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
enum edtCFG {edtALL=0, edtANOREF, edtMANOBRA, edtREDE_MT};

//---------------------------------------------------------------------------
class VTApl;
class VTEdt;

//---------------------------------------------------------------------------
class VTEditor : public TObject
	{
	public:  //property
      __property bool Completo = {read=PM_GetCompleto, write=PM_SetCompleto};

	public:
						   __fastcall VTEditor(void) {};
		virtual        __fastcall ~VTEditor(void) {};
		virtual void   __fastcall Disable(void) = 0;
		virtual void   __fastcall EditaBarra(TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL) = 0;
        virtual void   __fastcall EditaBateria(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaAltera(TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL) = 0;
		virtual void   __fastcall EditaCapacitor(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaCapserie(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaCarga(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaCargaIP(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaChave(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual VTEdt* __fastcall EditaColaEqptos(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual VTEdt* __fastcall EditaCopiaEqptos(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaEsquematicoAlign(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaFigComentario(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaFigLinkComentarioEqpto(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaFigRetangulo(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaFiltro(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaGerador(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaGiro(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaManobra(TAction *ResetFocus, TAction *SetFocus, TWinControl *parent) = 0;
		virtual void   __fastcall EditaMedicao(TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL) = 0;
		virtual void   __fastcall EditaMotor(TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL) = 0;
		virtual void   __fastcall EditaMutua(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaPolyline(TAction *ResetFocus, TAction *SetFocus, TWinControl *parent)=0;
		virtual void   __fastcall EditaReator(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaRede(void) = 0;
		virtual void   __fastcall EditaRegulador(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaSelEqpto(TList *lisEQP) = 0;
		virtual void   __fastcall EditaSincronismo(TAction *ResetFocus, TAction *SetFocus, TWinControl *parent) = 0;
		virtual void   __fastcall EditaSuprimento(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaSuprimentoPorFase(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaSwitch(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaTrafo(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaTrafo3E(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaTrafoMono(TAction *ResetFocus, TAction *SetFocus) = 0;
        virtual void   __fastcall EditaTrafoZZ(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaTrecho(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EditaZoom(TAction *ResetFocus, TAction *SetFocus) = 0;
		virtual void   __fastcall EvtDelete(void) = 0;
		virtual void   __fastcall EvtMove(bool down, bool left, bool right, bool up) = 0;
		virtual TList* __fastcall LisEqptoSelecionado(void) = 0;
		virtual bool   __fastcall RedeAlterada(void) = 0;
		virtual void   __fastcall Redo(void) = 0;
		virtual void   __fastcall Undo(void) = 0;

   protected:  //métodos acessados via property
		virtual bool   __fastcall PM_GetCompleto(void) = 0;
		virtual void   __fastcall PM_SetCompleto(bool completo) = 0;
	};

//---------------------------------------------------------------------------
//funções globais p/ criar objetos da classe VTEditor
//---------------------------------------------------------------------------
VTEditor* NewObjEditor(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
