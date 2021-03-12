//---------------------------------------------------------------------------
#ifndef TFormAvaliaH
#define TFormAvaliaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEstudo;

//---------------------------------------------------------------------------
class TFormAvalia : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *ButFecha;
   TToolButton *ButImprime;
   TActionList *ActionList1;
   TImageList *ImageList;
   TAction *ActionFecha;
   TAction *ActionImprime;
   TListView *LViewAnaTec;
   TToolButton *ToolButton1;
	TSaveDialog *SaveDialogExporta;
	TAction *ActionExportaAlim;
	TToolButton *ToolButton2;
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall ActionImprimeExecute(TObject *Sender);
	void __fastcall ActionExportaAlimExecute(TObject *Sender);

public:  // User declarations
        __fastcall  TFormAvalia(TComponent* Owner, VTApl *apl_owner);
        __fastcall ~TFormAvalia(void);
   void __fastcall  ExibirRede(bool enabled);

private: //métodos
   bool      __fastcall ExecutaAnaliseTecnica(void);
   VTEstudo* __fastcall ExisteEstudo(void);
   void      __fastcall FormIniciaPosicao(void);
   void      __fastcall LViewInicia(void);

private: //objetos externos
   VTApl    *apl;

private: //dados locais
   bool exibir_redes;
};

//---------------------------------------------------------------------------
#endif
//eof
