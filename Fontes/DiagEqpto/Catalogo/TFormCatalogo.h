//---------------------------------------------------------------------------

#ifndef TFormCatalogoH
#define TFormCatalogoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ValEdit.hpp>
//---------------------------------------------------------------------------
class TItemAtributo;
class TItemCatalogo;
class VTDiagEqpto;
class VTApl;
//---------------------------------------------------------------------------
class TFormCatalogo : public TForm
{
__published:	// IDE-managed Components
   TGroupBox *gbTipos;
   TGroupBox *gbAtributos;
   TListBox *lbTipos;
   TActionList *ActionList;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionDefault;
   TImageList *ImageList;
   TToolBar *toolBarEdicao;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *ToolButton1;
   TToolButton *butDefault;
   TToolBar *ToolBar2;
   TToolButton *ToolButton3;
   TToolButton *ToolButton4;
   TAction *ActionInsereTipo;
   TAction *ActionRemoveTipo;
   TValueListEditor *vlAtributos;
   void __fastcall ActionInsereTipoExecute(TObject *Sender);
   void __fastcall lbTiposClick(TObject *Sender);
   void __fastcall vlAtributosGetEditText(TObject *Sender, int ACol, int ARow, UnicodeString &Value);
   void __fastcall butCancelaClick(TObject *Sender);
   void __fastcall butConfirmaClick(TObject *Sender);
   void __fastcall ActionRemoveTipoExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall vlAtributosEditButtonClick(TObject *Sender);

private:	// User declarations

   virtual  TItemCatalogo* __fastcall CriaItem(void)=0;
            void           __fastcall InsereAtributo(TItemAtributo *itemAtributo);
            void           __fastcall IniciaListBox(void);
   virtual  void           __fastcall IniciaListValue(void)=0;
   virtual  void           __fastcall SalvaItemsCatalogo(void)=0;
            bool           __fastcall VerificaValor(AnsiString valor, int dataTYPE);

private: // dados externos
   VTApl *apl;

protected: //dados proprios
   TList         *listaItemsCatalogo;
   bool          alterado;
   VTDiagEqpto   *diagEqpto;

public:		// User declarations
               __fastcall TFormCatalogo(TComponent* Owner, VTApl *apl);
               __fastcall ~TFormCatalogo(void);
   AnsiString  __fastcall BooleanoParaString(bool boolean);
   void        __fastcall Inicia(VTDiagEqpto *diagEqpto);
   void        __fastcall ShowItem(TItemCatalogo *itemCatalogo);
   bool        __fastcall StringParaBool(AnsiString boolean_ptbr);



};
//---------------------------------------------------------------------------
extern PACKAGE TFormCatalogo *FormCatalogo;
//---------------------------------------------------------------------------
#endif
