//---------------------------------------------------------------------------
#ifndef TFormMultiMonitorH
#define TFormMultiMonitorH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTMultiPanel;

//---------------------------------------------------------------------------
class TFormMultiMonitor : public TForm
{
__published:	// IDE-managed Components
   TStatusBar *StatusBar;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

public:	//property
__property bool Ativo = {read=PM_GetAtivo, write=PM_SetAtivo};

public:		// User declarations
                  __fastcall TFormMultiMonitor(TComponent* Owner);
                 __fastcall ~TFormMultiMonitor(void);
   void          __fastcall ExibePanel2(VTMultiPanel *MultiPanel);
   VTMultiPanel* __fastcall InsereMultiPanel(TPanel *panel1);
   void          __fastcall RetiraMultiPanel(VTMultiPanel *MultiPanel);

private:	//métodos
   void __fastcall TransfereForms(void);

private:	//métodos acessados via property
   bool __fastcall PM_GetAtivo(void);
   void __fastcall PM_SetAtivo(bool ativo);

private:	//dados acessados via property
   struct   {
            bool ativo;
            }PD;

private:	//dados
   VTMultiPanel *MultiPanelAtivo;
};

//---------------------------------------------------------------------------
#endif
//eof
