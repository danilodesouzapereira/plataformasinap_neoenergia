//---------------------------------------------------------------------------
#ifndef TFormEdtChvMSH
#define TFormEdtChvMSH

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
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTChave;
class VTRede;

//---------------------------------------------------------------------------
class TFormEdtChvMS : public TForm
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
   TEdit *EditFabricante;
   TPanel *Panel5;
   TLabel *Label4;
   TComboBox *CBoxTipoChave;
   TRadioGroup *RadioGroupEstado;
   TPanel *Panel6;
   TPanel *PanelLeft;
	TPanel *PanelDisparo;
	TCheckBox *CheckBoxDisparo;
	TLabel *LblDelay;
	TEdit *EditDelay;
	TPanel *PnlDelay;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall RadioGroupEstadoClick(TObject *Sender);
	void __fastcall CheckBoxDisparoClick(TObject *Sender);
	void __fastcall CBoxTipoChaveChange(TObject *Sender);
   
public:      // User declarations
        __fastcall  TFormEdtChvMS(TComponent* Owner, VTApl *apl, VTChave *trecho, bool op_retira);
        __fastcall ~TFormEdtChvMS(void);

private: //métodos
   void      __fastcall CBoxRedeInicia(void);
   void      __fastcall CBoxTipoChaveInicia(void);
   void      __fastcall DefValoresDefaultChave(void);
   VTChave*  __fastcall ExisteChave(AnsiString codigo);
   void      __fastcall ExibeDadosChave(void);
   void      __fastcall ExibeDadosDisparo(void);
   void      __fastcall FormIniciaPosicao(void);
   void      __fastcall IniciaLisChave(void);
   bool      __fastcall LeDadosChave(void);
   VTRede*   __fastcall SelecionaRede(void);
   bool	     __fastcall TipoChaveIsTipoDisparo(void);

private: //objeto externos
   VTApl    *apl;
   VTChave  *chave;

private: //dados locais
   TList *lisCHAVE;

};

#endif
//---------------------------------------------------------------------------
//eof

