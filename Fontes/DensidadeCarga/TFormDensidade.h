//---------------------------------------------------------------------------
#ifndef TFormDensidadeH
#define TFormDensidadeH

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
class VTApl;
class VTGrafico;
class VTRedes;
class VTRedegraf;
class VTCelulas;
class VTBD;

//---------------------------------------------------------------------------
class TFormDensidade : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *ButFecha;
   TImageList *ImageList;
   TActionList *ActionList;
   TAction *ActionFecha;
   TPanel *PanelGrafico;
   TPanel *panelToolBarGrafico;
   TPanel *Panel1;
   TToolButton *ToolButton1;
   TToolButton *ToolButton2;
   TLabel *Label1;
   TComboBox *CBoxData;
   TToolButton *ButSelBase;
   TAction *ActionSelBaseDado;
   TOpenDialog *OpenDialog;
   TPanel *Panel2;
   TToolBar *ToolBar1;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionSelecionaAnoExecute(TObject *Sender);
   void __fastcall ActionFechaExecute(TObject *Sender);
   void __fastcall CBoxDataChange(TObject *Sender);
   void __fastcall ActionSelBaseDadoExecute(TObject *Sender);

public:      // User declarations
   __fastcall  TFormDensidade(TComponent *Owner, VTApl *apl_owner, TAction *ActionOnClose);
   __fastcall ~TFormDensidade(void);

private:   //métodos
   void __fastcall CBoxDataInicia(void);
   void __fastcall ConectaBaseHistorico(void);
   void __fastcall IniciaObjetosBasicos(void);
   void __fastcall ReiniciaRede(void);

private:   //objetos externos
   TAction *ActionOnClose;
   TAction *CloseSNPChild;
private:   //dados locais
   VTBD       *bd;
   VTApl      *apl;
   VTCelulas  *celulas;
   VTGrafico  *grafico;
   VTRedegraf *redegraf;
   VTRedes    *redes;
   TList      *lisARVORE;
};

//---------------------------------------------------------------------------
#endif
//eof
