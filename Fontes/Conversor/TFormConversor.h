//---------------------------------------------------------------------------
#ifndef TFormConversorH
#define TFormConversorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <CheckLst.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormConversor : public TForm
{
__published:	// IDE-managed Components
   TActionList *ActionList;
   TAction *ActionSelSinap;
   TOpenDialog *OpenDialog;
   TAction *ActionSelRede;
   TAction *ActionConverte;
   TGroupBox *GBoxDirSinap2;
   TEdit *EditDirSinap2;
   TGroupBox *GroupBox1;
   TListBox *LBoxRede;
   TImageList *ImageList;
   TToolBar *ToolBar1;
   TToolButton *ToolButton2;
   TAction *ActionExcluiRede;
   TAction *ActionClearSelecao;
   TToolButton *ToolButton1;
   TToolButton *ButConverte;
   TToolBar *ToolBar2;
   TToolButton *ToolButton8;
   TToolButton *ToolButton9;
   TToolButton *ToolButton4;
   TAction *ActionFecha;
   TToolButton *ButFecha;
   TToolButton *ButSelRede;
   TPanel *PanelProgresso;
   TLabel *LabelProgresso;
   TGroupBox *GroupBox2;
   TCheckBox *CheckBoxExportaTXT;
   TCheckBox *CheckBoxBaseAccess;
   TCheckBox *CheckBoxDescartaArranjo;
   TCheckBox *CheckBoxDirRaiz;
   void __fastcall ActionSelSinapExecute(TObject *Sender);
   void __fastcall ActionSelRedeExecute(TObject *Sender);
   void __fastcall ActionConverteExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionExcluiRedeExecute(TObject *Sender);
   void __fastcall ActionClearSelecaoExecute(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall LBoxRedeDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
   void __fastcall FormShow(TObject *Sender);

public:		// User declarations
        __fastcall  TFormConversor(TComponent *Owner, VTApl *apl_owner);
        __fastcall ~TFormConversor(void);

private:	//métodos
   void __fastcall DescartaArranjosNaoUsados(int num_arranjos);
   void __fastcall FormIniciaPosicao(void);
   void __fastcall IdentificaRedesNoDiretorio(AnsiString dir_raiz, TStrings *StrListRede);
   void __fastcall ImportaListaDeRedes(TStrings *bases);
   bool __fastcall SalvaRedeArquivoTexto(AnsiString nome_rede);
   bool __fastcall SalvaRedeBaseDado(AnsiString dir_rede);

private:	//objetos externos
   VTApl *apl;

private: //dados locais
   TStrings *StrListRede;
};

//---------------------------------------------------------------------------
#endif
//eof
