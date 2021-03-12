//---------------------------------------------------------------------------
#ifndef TFormConfigH
#define TFormConfigH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <CheckLst.hpp>
#include <ExtCtrls.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEstudo;
class VTGeral;
class VTPlanejamento;

//---------------------------------------------------------------------------
class TFormConfig : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *ButFecha;
   TToolButton *ButImprime;
   TActionList *ActionList;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TToolButton *ToolButton1;
   TImageList *ImageList;
   TGroupBox *GBoxPeriodo;
   TLabel *Label1;
   TEdit *EditAnoIni;
   TLabel *Label2;
   TEdit *EditAnoFim;
   TPanel *Panel1;
   TLabel *LabelDataUltObra;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);

public:  // User declarations
        __fastcall  TFormConfig(TComponent* Owner, VTApl *apl_owner);
        __fastcall ~TFormConfig(void);

private: //métodos
   void __fastcall FormIniciaPosicao(void);
   void __fastcall CLBoxAlternativaInicia(void);
   void __fastcall ExibeDataAnalise(void);
   bool __fastcall LeDataAnalise(void);

private: //objetos externos
   VTApl          *apl;
   VTEstudo       *estudo;
   VTGeral        *geral;
   VTPlanejamento *planejamento;

private: //dados locais
};

//---------------------------------------------------------------------------
#endif
//eof
