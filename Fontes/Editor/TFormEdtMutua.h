//---------------------------------------------------------------------------
#ifndef TFormEdtMutuaH
#define TFormEdtMutuaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTBarra;
class VTMutua;
class VTApl;

//---------------------------------------------------------------------------
class TFormEdtMutua : public TForm
{
__published:	// IDE-managed Components
   TPageControl *pgCtrl;
   TTabSheet *tsMutua;
   TGroupBox *gboxChv1;
   TTabSheet *tsPlaneja;
   TEdit *EditCodigo;
   TLabel *Label1;
   TLabel *lbReatancia;
   TEdit *edtX1;
   TLabel *Label2;
   TEdit *edtX0;
   TGroupBox *GroupBox1;
   TComboBox *cboxBarTre1;
   TComboBox *cboxBarTre2;
   TLabel *LabelTre1;
   TLabel *LabelTre2;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TToolBar *ToolBar2;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
   TPanel *Panel5;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);

public:		// métodos
        __fastcall TFormEdtMutua(TComponent* Owner, VTApl *apl, VTMutua* mutua);
        __fastcall ~TFormEdtMutua(void);
   void __fastcall OpcaoRetira(bool enabled);

private:    //métodos
   void __fastcall DefValoresDefaultMutua(void);
   void __fastcall ExibeDadosMutua(void);
   void __fastcall FormIniciaPosicao(void);
   void __fastcall IniciaCBoxBar(TComboBox *cboxBar, VTBarra *pbar1, VTBarra *pbar2);
   void __fastcall IniciaParteFixaTela(void);
   bool __fastcall LeDadosMutua(void);

private:    //objetos externos
   VTApl   *apl;
   VTMutua *mutua;

private: //dados locais
   TList  *lisMUTUA;
};

#endif
//---------------------------------------------------------------------------
//eof
