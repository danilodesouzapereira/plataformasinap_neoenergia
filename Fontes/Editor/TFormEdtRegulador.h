//---------------------------------------------------------------------------
#ifndef TFormEdtReguladorH
#define TFormEdtReguladorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <System.Actions.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEqpto;
class VTRede;
class VTRegulador;

//---------------------------------------------------------------------------
class TFormEdtRegulador : public TForm
{
__published:   // IDE-managed Components
	TToolBar *ToolBar;
   TToolButton *butConfirma;
   TToolButton *butCancela;
   TToolButton *butRetira;
   TActionList *ActionList1;
   TAction *ActionCancela;
   TAction *ActionConfirma;
   TAction *ActionRetira;
   TImageList *ImageList1;
	TRadioGroup *RGroupTipoReg;
   void __fastcall ActionCancelaExecute(TObject *Sender);
   void __fastcall ActionConfirmaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall FormShow(TObject *Sender);

public: //construtor e destrutor
        __fastcall  TFormEdtRegulador(TComponent* Owner, VTApl *apl, VTRegulador *regulador);
        __fastcall ~TFormEdtRegulador(void);
   void __fastcall  OpcaoRetira(bool enabled);

private: //métodos
   void    __fastcall FormIniciaPosicao(void);


private: //objetos externos
   VTApl       *apl;
   VTRegulador *regulador;

};

//---------------------------------------------------------------------------
#endif
//eof
