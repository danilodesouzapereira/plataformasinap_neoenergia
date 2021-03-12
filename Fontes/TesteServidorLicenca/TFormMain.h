//---------------------------------------------------------------------------
#ifndef TFormMainH
#define TFormMainH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <ScktComp.hpp>

//---------------------------------------------------------------------------
class VTPacote;
class VTLTPserver;

//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
   TToolBar *ToolBar;
   TImageList *ImageList;
   TActionList *ActionList;
   TServerSocket *ServerSocket;
   TStatusBar *StatusBar;
   TRichEdit *RichEdit;
   TToolButton *ButLogClear;
   TAction *ActionClear;
   TToolButton *ButListaIP;
   TAction *ActionListaIP;
   void __fastcall ServerSocketClientConnect(TObject *Sender,
          TCustomWinSocket *Socket);
   void __fastcall ServerSocketClientDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
   void __fastcall ServerSocketClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
   void __fastcall ServerSocketClientWrite(TObject *Sender,
          TCustomWinSocket *Socket);
   void __fastcall ActionClearExecute(TObject *Sender);
   void __fastcall ActionListaIPExecute(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);

public:		// User declarations
        __fastcall TFormMain(TComponent* Owner);
        __fastcall ~TFormMain(void);
   bool __fastcall EnviaPacote(TCustomWinSocket *Socket, VTPacote *pacote);

private:	//métodos
   void __fastcall Log(AnsiString txt);

private:	//dados
   VTPacote    *pacote;
   VTLTPserver *ltp_server;
};

//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;

//---------------------------------------------------------------------------
#endif
//eof

