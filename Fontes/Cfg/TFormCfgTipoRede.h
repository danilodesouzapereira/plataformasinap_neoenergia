//---------------------------------------------------------------------------
#ifndef TFormCfgTipoRedeH
#define TFormCfgTipoRedeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class TAplCfg;
class VTApl;
class VTBD;

//---------------------------------------------------------------------------
class TFormCfgTipoRede : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *ButConfirma;
   TToolButton *ButCancela;
   TToolButton *ToolButton1;
   TToolButton *ButInsere;
   TToolButton *ButRetira;
   TActionList *ActionList;
   TAction *ActionConfirma;
   TAction *ActionCancela;
   TAction *ActionInsere;
   TAction *ActionRetira;
   TImageList *ImageList;
   TListView *LViewRede;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall ActionInsereExecute(TObject *Sender);
   void __fastcall ActionRetiraExecute(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall LViewRedeCustomDrawSubItem(TCustomListView *Sender,
          TListItem *Item, int SubItem, TCustomDrawState State,
          bool &DefaultDraw);
   void __fastcall LViewRedeDblClick(TObject *Sender);

public:      // User declarations
   __fastcall  TFormCfgTipoRede(TComponent* Owner, VTApl *apl_owner);
   __fastcall ~TFormCfgTipoRede(void);

private:   //métodos
   bool       __fastcall ExisteTipoRede(AnsiString codigo);
   AnsiString __fastcall NextCodigo(void);
   int        __fastcall NextValorSegmento(void);
   void       __fastcall SalvaBaseDados(void);
   void       __fastcall LViewRedeInicia(void);

private:   //objetos externos

private:   //dados locais
   TAplCfg     *apl;
};

//---------------------------------------------------------------------------
#endif
//eof

