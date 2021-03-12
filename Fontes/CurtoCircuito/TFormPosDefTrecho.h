//---------------------------------------------------------------------------
#ifndef TFormPosDefTrechoH
#define TFormPosDefTrechoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTTrecho;

//---------------------------------------------------------------------------
class TFormPosDefTrecho : public TForm
{
__published:	// IDE-managed Components
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TToolBar *ToolBar1;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TGroupBox *gboxDefTrecho;
   TTrackBar *TrackBar;
   TEdit *EditComp1;
   TEdit *EditComp2;
   TLabel *Label1;
   TLabel *Label2;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall TrackBarChange(TObject *Sender);

public:		// User declarations
          __fastcall  TFormPosDefTrecho(TComponent* Owner, VTApl *apl);
          __fastcall ~TFormPosDefTrecho(void);
   void   __fastcall  DefinePosDefeito(VTTrecho *trecho, double dist_bar1);
   double __fastcall  DistanciaBarra1KM(void);

private:	//métodos
   void __fastcall TrackBarInicia(void);
   void __fastcall FormIniciaPosicao(void);

private:	//objtos externos
   VTApl *apl;

private:	//dados locais
   struct   {
            VTTrecho *trecho;
            double   dist_bar1;
            double   dist_bar2;
            } defeito;
};

//---------------------------------------------------------------------------
#endif
//eof
