//---------------------------------------------------------------------------
#ifndef TFormCfgTipoChaveH
#define TFormCfgTipoChaveH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Grids.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class TAplCfg;
class VTApl;
class VTTipos;
class VTBD;

//---------------------------------------------------------------------------
class TFormCfgTipoChave : public TForm
{
__published:   // IDE-managed Components
   TActionList *ActionList;
   TAction *ActionConfirma;
   TAction *ActionCancela;
   TAction *ActionInsere;
   TAction *ActionRetira;
   TImageList *ImageList;
   TToolBar *ToolBar;
   TToolButton *ButConfirma;
   TToolButton *ButCancela;
   TToolButton *ToolButton1;
   TToolButton *ButInsere;
   TToolButton *ButRetira;
   TListView *LViewChv;
   void __fastcall ActionInsereExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall LViewChvCustomDrawSubItem(TCustomListView *Sender,
          TListItem *Item, int SubItem, TCustomDrawState State,
          bool &DefaultDraw);
   void __fastcall LViewChvDblClick(TObject *Sender);

public:      // User declarations
   __fastcall  TFormCfgTipoChave(TComponent* Owner, VTApl *apl_owner);
   __fastcall ~TFormCfgTipoChave(void);

private:   //objetos externos

private:   //métodos
   bool       __fastcall ExisteTipoChave(AnsiString codigo);
   AnsiString __fastcall NextCodigo(void);
   int        __fastcall NextValorTipo(void);
   void       __fastcall SalvaBaseDados(void);
   void       __fastcall LViewChvInicia(void);

private:   //dados locais
	TAplCfg *apl;
	VTTipos *tipos;
};

//---------------------------------------------------------------------------
#endif
//eof

