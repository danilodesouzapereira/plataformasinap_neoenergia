//---------------------------------------------------------------------------
#ifndef TEditorH
#define TEditorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEditor.h"

//---------------------------------------------------------------------------
class VTEdt;

//---------------------------------------------------------------------------
class TEditor : public VTEditor
   {
   public:
			   __fastcall  TEditor(VTApl *apl);
			   __fastcall ~TEditor(void);
		void   __fastcall Disable(void);
		void   __fastcall EditaAltera(TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
		void   __fastcall EditaBarra(TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
		void   __fastcall   EditaBateria(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaCapacitor(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaCapserie(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaCarga(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaCargaNL(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaCargaIP(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaChave(TAction *ResetFocus, TAction *SetFocus);
		VTEdt* __fastcall EditaColaEqptos(TAction *ResetFocus, TAction *SetFocus);
		VTEdt* __fastcall EditaCopiaEqptos(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaEsquematicoAlign(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaFigComentario(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaFigLinkComentarioEqpto(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaFigRetangulo(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaFiltro(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaGerador(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaGiro(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaManobra(TAction *ResetFocus, TAction *SetFocus, TWinControl *parent);
		void   __fastcall EditaMedicao(TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
		void   __fastcall EditaMotor(TAction *ResetFocus, TAction *SetFocus, TAction *OnDone);
		void   __fastcall EditaMutua(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaPolyline(TAction *ResetFocus, TAction *SetFocus, TWinControl *parent);
		void   __fastcall EditaReator(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaRede(void);
		void   __fastcall EditaRegulador(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaSelEqpto(TList *lisEQP);
		void   __fastcall EditaSincronismo(TAction *ResetFocus, TAction *SetFocus, TWinControl *parent);
		void   __fastcall EditaSuprimento(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaSuprimentoPorFase(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaSwitch(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaTrafo(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaTrafo3E(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaTrafoMono(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaTrafoZZ(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaTrecho(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EditaTrechoGeo(TAction *ResetFocus, TAction *SetFocus, TWinControl *parent);
		void   __fastcall EditaZoom(TAction *ResetFocus, TAction *SetFocus);
		void   __fastcall EvtMove(bool down, bool left, bool right, bool up);
		void   __fastcall EvtDelete(void);
		TList* __fastcall LisEqptoSelecionado(void);
		bool   __fastcall RedeAlterada(void);
		void   __fastcall Redo(void);
		void   __fastcall Undo(void);

   protected:  //métodos acessados via property
		bool   __fastcall PM_GetCompleto(void);
		void   __fastcall PM_SetCompleto(bool completo);

	private:  //dados acessados via property
      struct{
            bool completo;
            }tipo;

	private:  //dados
      VTApl    *apl;
	  VTEdt    *edt;
      VTEdt    *edt_copia;
   };

#endif
//---------------------------------------------------------------------------
//eof
