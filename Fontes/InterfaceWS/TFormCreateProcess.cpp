//---------------------------------------------------------------------------
#include <vcl.h>
#include <winbase.h>
#pragma hdrstop
#include "TFormCreateProcess.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCreateProcess::TFormCreateProcess(TComponent* Owner, AnsiString cmd_line)
                     : TForm(Owner)
   {
   //salva cmd_line
   this->cmd_line = cmd_line;
   //bloqueia o timer
   timer->Enabled = false;
   //esconde este form:
   WindowState = wsMinimized;
   //inicia ProgBar
   pbar->Min      = 1;
   pbar->Max      = 10;
   pbar->Position = pbar->Min;
   }

//---------------------------------------------------------------------------
__fastcall TFormCreateProcess::~TFormCreateProcess(void)
   {
   //nada a fazer
   }


//---------------------------------------------------------------------------
void __fastcall TFormCreateProcess::ButCancelaClick(TObject *Sender)
   {
   //aborta execu��o
   if (pi.hProcess) TerminateProcess(pi.hProcess, 0);
   timer->Enabled = false;
   ModalResult    = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCreateProcess::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormCreateProcess::FormShow(TObject *Sender)
   {
   //vari�veis locais
   STARTUPINFO si;

   //obt�m configura��o p/ inicia��o do processo a ser criado
   GetStartupInfo(&si);
   //inicia processo p/ execu��o do programa da aplica��o
   if (! CreateProcess(NULL,                       //pointer to name of executable module
                       cmd_line.c_bstr(),          // pointer to command line string
                       NULL,                       // pointer to process security attributes
                       NULL,                       // pointer to thread security attributes
                       TRUE,                       // handle inheritance flag
                       CREATE_DEFAULT_ERROR_MODE,  // creation flags
                       NULL,                       // pointer to new environment block
                       NULL,                       // pointer to current directory name
                       &si,                        // pointer to STARTUPINFO
                       &pi))                       // pointer to PROCESS_INFORMATION
      {//erro na inicia��o do programa
      ModalResult = mrCancel;
      return;
      }
   //inicia timer p/ controle de t�rmino do programa criado
   timer->Interval = 100;
   timer->Enabled  = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCreateProcess::timerTimer(TObject *Sender)
   {
   //vari�veis locais
   DWORD exit_code;

   //verifica se o ProgBar atingiu seu limite: 1 seg
   if (pbar->Position >= pbar->Max)
      {//torna este form vis�vel p/ dar ao usu�rio a op��o de cancelar a an�lise
      WindowState = wsNormal;
      Application->BringToFront();
      pbar->Position = pbar->Min;
      pbar->Max      = 100;
      pbar->Refresh();
      }
   //atualiza ProgBar
   pbar->Position += 1;
   //verifica status do processo criado
   if (! GetExitCodeProcess(pi.hProcess, &exit_code))
      {
      timer->Enabled = false;
      //Erro("Erro na execu��o do programa");
      ModalResult = mrCancel;
      }
   if (exit_code != STILL_ACTIVE)
      {//processo terminou
      timer->Enabled = false;
      ModalResult = mrOk;
      }
   }

//---------------------------------------------------------------------------
//eof

