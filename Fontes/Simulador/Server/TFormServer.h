//---------------------------------------------------------------------------
#ifndef TFormServerH
#define TFormServerH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <System.Actions.hpp>
#include <Vcl.Menus.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTSinapUser;
class VTSinapServer;
class TFormCfg;
class TFormOpAuto;
class TFormOpManual;

//---------------------------------------------------------------------------
class TFormServer : public TForm
{
__published:   // IDE-managed Components
   TToolBar *ToolBar;
   TToolButton *ButStart;
   TToolButton *ButStop;
   TActionList *ActionList;
   TAction *ActionServerStart;
   TAction *ActionServerStop;
   TGroupBox *GBoxTX;
   TGroupBox *GBoxRx;
   TSplitter *Splitter1;
   TRichEdit *RichEditTx;
   TRichEdit *RichEditRx;
   TStatusBar *StatusBar;
   TToolButton *ButListaIP;
   TToolButton *ToolButton2;
   TAction *ActionListaIP;
   TToolButton *ButClear;
   TAction *ActionClearLog;
   TImageList *ImageList;
   TToolButton *ButSupervisaoAuto;
   TActionList *ActionListSupervisao;
   TAction *ActionCfgSup;
   TAction *ActionOpAuto;
   TAction *ActionShow;
   TToolButton *ButOpManual;
   TAction *ActionOpManual;
   TToolButton *ToolButton1;
   TToolButton *ButOpAuto;
   void __fastcall ActionServerStartExecute(TObject *Sender);
   void __fastcall ActionServerStopExecute(TObject *Sender);
   void __fastcall ActionListaIPExecute(TObject *Sender);
   void __fastcall ActionClearLogExecute(TObject *Sender);
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
   void __fastcall ActionOpAutoExecute(TObject *Sender);
   void __fastcall ActionCfgSupExecute(TObject *Sender);
   void __fastcall ActionShowExecute(TObject *Sender);
   void __fastcall ActionOpManualExecute(TObject *Sender);

public:      // SinapUser declarations
   __fastcall  TFormServer(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
   __fastcall ~TFormServer(void);

private:   //métodos
   void __fastcall ConfiguraFlow(void);

private: //objetos externos

private: //dados locais
   VTApl         *apl;
   VTSinapServer *server;
   VTSinapUser   *user;
   TFormCfg      *FormCfg;
   TFormOpAuto   *FormOpAuto;
   TFormOpManual *FormOpManual;
   struct{//supervisão automática
         bool  ativa;
         TList *lisEQP;
         }supervisao;

   struct{//real time
         bool  active;
         int   dt_seg;     //tempo para alteração de patamar
         }real_time;
};

//---------------------------------------------------------------------------
#endif
//eof

