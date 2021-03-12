//---------------------------------------------------------------------------
#ifndef TFormEdtTreMSH
#define TFormEdtTreMSH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <System.Actions.hpp>
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRede;
class VTTrecho;

//---------------------------------------------------------------------------
class TFormEdtTreMS : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar1;
   TLabel *Label1;
   TEdit *EditCodigo;
   TLabel *LabelImax;
   TEdit *EditImax;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TPanel *Panel1;
   TPanel *Panel2;
   TPanel *Panel3;
   TLabel *Label2;
   TComboBox *CBoxRede;
   TPanel *Panel4;
   TLabel *Label3;
   TEdit *EditXseq1;
   TPanel *Panel5;
   TLabel *Label4;
   TEdit *EditRseq1;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   
public:      // User declarations
        __fastcall  TFormEdtTreMS(TComponent* Owner, VTApl *apl, VTTrecho *trecho, bool op_retira);
        __fastcall ~TFormEdtTreMS(void);

private: //métodos
   void      __fastcall CBoxRedeInicia(void);
   void      __fastcall DefValoresDefaultTrecho(void);
   VTTrecho* __fastcall ExisteTrecho(AnsiString codigo);
   void      __fastcall ExibeDadosTrecho(void);
   void      __fastcall FormIniciaPosicao(void);
   void      __fastcall IniciaLisTrecho(void);
   bool      __fastcall LeDadosTrecho(void);
   VTRede*   __fastcall SelecionaRede(void);

private: //objeto externos
   VTApl     *apl;
   VTTrecho  *trecho;

private: //dados locais
   TList *lisTRECHO;

};

#endif
//---------------------------------------------------------------------------
//eof

