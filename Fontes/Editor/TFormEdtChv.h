//---------------------------------------------------------------------------
#ifndef TFormEdtChvH
#define TFormEdtChvH

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
class VTApl;
class VTChave;
class VTRede;
class TFormEdtPlan;

//---------------------------------------------------------------------------
class TFormEdtChv : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *gboxChv1;
	TLabel *LabelChvCodigo;
   TLabel *LabelChvInom;
   TLabel *Label9;
   TLabel *Label10;
   TEdit *EditCodigo;
   TEdit *edtChvInom;
   TComboBox *CBoxEstado;
   TComboBox *CBoxTipoChave;
   TLabel *Label1;
   TComboBox *CBoxRede;
   TToolBar *ToolBar2;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TToolButton *ToolButton3;
   TImageList *ImageList1;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TCheckBox *CheckBoxBloqueada;
   TGroupBox *GroupBox1;
   TComboBox *CBoxTecnoCom;
   TCheckBox *CheckBoxTelecomandada;
   TPanel *panelCodigo;
   TPanel *panelTipo;
   TPanel *panelRede;
   TPanel *panelCorrente;
   TPanel *panelEstado;
   TPanel *Panel1;
   TPanel *PanelElo;
   TLabel *Label2;
   TEdit *EditElo;
	TLabel *Label3;
	TEdit *edtChvTipoGIS;
	TPanel *panelTipoGIS;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall CBoxTipoChaveChange(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall CBoxTecnoComChange(TObject *Sender);

public:		// métodos
           __fastcall  TFormEdtChv(TComponent* Owner, VTApl *apl, VTChave *chave, bool op_retira);
           __fastcall ~TFormEdtChv(void);

private: //métodos
   void    __fastcall CBoxRedeInicia(void);
   void    __fastcall CBoxTecnoComInicia(void);
   void    __fastcall CBoxTipoChaveInicia(void);
   void    __fastcall ExibeDadosChave(void);
   void    __fastcall FormIniciaPosicao(void);
   void    __fastcall IniciaDadosChaveNova(void);
   bool    __fastcall LeDadosChave(void);
   VTRede* __fastcall SelecionaRede(TList *lisREDE);

private:  //dados
   VTApl        *apl;
   VTChave      *chave;
   TFormEdtPlan *FormEdtPlan;
};

#endif
//---------------------------------------------------------------------------
//eof
