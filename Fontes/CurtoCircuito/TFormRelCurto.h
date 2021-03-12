//---------------------------------------------------------------------------
#ifndef TFormRelCurtoH
#define TFormRelCurtoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;
class VTRedeCC;
class VTLigCC;

//---------------------------------------------------------------------------
class TFormRelCurto : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar1;
   TImageList *ImageList;
   TActionList *ActionList;
   TComboBox *CBoxDefeito;
   TPageControl *PageControl;
   TTabSheet *TabSheetPtoDefeito;
   TTabSheet *TabSheetContribuicao;
   TTabSheet *TabSheetTensao;
   TTabSheet *TabSheetCorrente;
	TListView *LViewDefIcc;
   TListView *LViewDefVcc;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
	TComboBox *CBoxUnidTensao;
   TComboBox *CBoxUnidCorrente;
   TAction *ActionAtualiza;
   TListView *LViewBarVcc;
	TListView *LViewSupIcc;
   TListView *LViewLigIcc;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionAtualizaExecute(TObject *Sender);
	void __fastcall PageControlChange(TObject *Sender);
	void __fastcall LViewChange(TObject *Sender, TListItem *Item,
          TItemChange Change);

public:		// User declarations
   __fastcall TFormRelCurto(TComponent* Owner, TWinControl *parent, VTApl *apl);
   __fastcall ~TFormRelCurto(void);

private:	//métodos
   void       __fastcall IniciaCBoxDefeito(void);
   void       __fastcall IniciaLViewBarVcc(void);
   void       __fastcall IniciaLViewDefIcc(void);
   void       __fastcall IniciaLViewDefVcc(void);
   void       __fastcall IniciaLViewLigIcc(void);
   void       __fastcall IniciaLViewLigIcc(VTLigCC *ligCC, AnsiString codigo, int ind_bar);
	void       __fastcall IniciaLViewSupIcc(void);
	void       __fastcall Moldura(VTEqpto *eqpto=NULL);
	AnsiString __fastcall NomeDefeito(int ind_def);

private:	//objetos externos
	VTApl    *apl;
	VTRedeCC *redeCC;

private:	//dados locais
};

//---------------------------------------------------------------------------
#endif
//eof
