//---------------------------------------------------------------------------
#ifndef VTEdtH
#define VTEdtH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
enum redrawGRAF {redrawEQP_SEL_COR_ORIG=0, redrawEQP_SEL_COR_UNICA, redrawEQP_NOVO,
                 redrawFIG_SEL_COR_ORIG,   redrawFIG_SEL_COR_UNICA, redrawFIG_NOVA,
                 redrawMARCA, redrawREGIAO};

//---------------------------------------------------------------------------
class VTEdt : public TObject
   {
   public:
                     __fastcall VTEdt(void) {};
      virtual        __fastcall ~VTEdt(void) {};
      virtual void   __fastcall CoordMouse(int &x, int &y) = 0;
      virtual void   __fastcall EvtEnter(void) = 0;
      virtual void   __fastcall EvtEscape(void) = 0;
      virtual void   __fastcall EvtDelete(void) = 0;
	  virtual void   __fastcall EvtMouse(int Action, TMouseButton Button, TShiftState Shift,
													  int x_utm, int y_utm, int x, int y) = 0;
	  virtual void   __fastcall EvtMove(bool down, bool left, bool right, bool up) = 0;
	  virtual void   __fastcall EvtSelArea(int x1, int y1, int x2, int y2) = 0;
	  virtual void   __fastcall EvtSelEqpto(TList *lisEXT) = 0;
	  virtual TList* __fastcall LisEqptoNovo(void) = 0;
	  virtual TList* __fastcall LisEqptoSel(void) = 0;
	  virtual TList* __fastcall LisMarca(void) = 0;
      virtual void   __fastcall MarcaCancela(void) = 0;;
      virtual void   __fastcall ResetGrafFocus(void) = 0;
      virtual void   __fastcall SetGrafFocus(void) = 0;
   };

//---------------------------------------------------------------------------
//funções globais p/ criar objetos da classe VTEdt
//---------------------------------------------------------------------------
VTEdt* NewObjEdtAlign        (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtAlt          (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtBateria      (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtBar          (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtCarga        (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtCargaIP      (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtCap          (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtCaps         (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtChv          (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtCopia        (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtCola         (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtFigComentario(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtFigLink      (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtFigRetangulo (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtFiltro       (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtGer          (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtGiro         (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtManobra      (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL, TWinControl *parent=NULL);
VTEdt* NewObjEdtMedicao      (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtMotor        (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtMutua        (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtPolyline     (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL, TWinControl *parent=NULL);
VTEdt* NewObjEdtReator       (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtRegulador    (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtSinc         (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL, TWinControl *parent=NULL);
VTEdt* NewObjEdtSup          (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtSup3         (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtTre          (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtTrf          (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtTrf3E        (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtTrfMono      (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtTrfZZ        (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtZoom         (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
//objetos EDT usados por outros módulos
VTEdt* NewObjEdtPoint        (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtArea         (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtBusca        (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtAltCargaNL   (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtCargaNL      (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtCurto        (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtSwitch       (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);
VTEdt* NewObjEdtMedidor      (VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone=NULL);

#endif
//---------------------------------------------------------------------------
//eof
