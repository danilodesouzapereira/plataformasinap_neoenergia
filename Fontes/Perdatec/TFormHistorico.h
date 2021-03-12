//---------------------------------------------------------------------------
#ifndef TFormHistoricoH
#define TFormHistoricoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Menus.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>

//---------------------------------------------------------------------------
class TFormPerda;
class VTApl;
class VTBD;
class VTArvore;

//---------------------------------------------------------------------------
class TFormHistorico : public TForm
{
__published:   // IDE-managed Components
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionFecha;
   TAction *ActionMnuGrupos_RegiaoAdiciona;
   TAction *ActionMnuGrupos_RegiaoRemove;
   TAction *ActionMnuGrupos_RegiaoRenomeia;
   TAction *ActionMnuGrupos_LocalAdiciona;
   TAction *ActionMnuGrupos_LocalRenomeia;
   TAction *ActionMnuGrupos_LocalRemove;
   TAction *ActionSelBasePerdaTecnica;
   TOpenDialog *OpenDialog;
   TPanel *Panel;
   TPanel *PanelToolBar;
   TPanel *PanelBase;
   TLabel *Label1;
   TComboBox *CBoxData;
   TBitBtn *BitBtn1;
   TPanel *PanelRelatorio;
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall CBoxDataChange(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall ActionSelBasePerdaTecnicaExecute(TObject *Sender);

public:      // User declarations
   __fastcall  TFormHistorico(TComponent *Owner, VTApl *apl_owner, TAction *ActionOnClose, int tipo);
   __fastcall ~TFormHistorico(void);

private:   //métodos
   bool       __fastcall AdicionaGrupoPerda(void);
   void       __fastcall CBoxDataInicia(void);
   void       __fastcall ConectaBaseHistorico(void);
   void       __fastcall FiltraArvore(VTArvore* arvore);
   void       __fastcall VerificaModulos(void);

   //objetos externos
   TAction *ActionOnClose;

private:   //dados locais
   TFormPerda  *FormPerda;
   VTApl       *apl;
   VTBD        *bd;
   TList       *lisARVORE;
};

//---------------------------------------------------------------------------
#endif
//eof
