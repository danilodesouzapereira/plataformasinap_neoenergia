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
   //aborta execução
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
   //variáveis locais
   STARTUPINFO si;

   //obtém configuração p/ iniciação do processo a ser criado
   GetStartupInfo(&si);
   //inicia processo p/ execução do programa da aplicação
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
      {//erro na iniciação do programa
      ModalResult = mrCancel;
      return;
      }
   //inicia timer p/ controle de término do programa criado
   timer->Interval = 100;
   timer->Enabled  = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCreateProcess::timerTimer(TObject *Sender)
   {
   //variáveis locais
   DWORD exit_code;

   //verifica se o ProgBar atingiu seu limite: 1 seg
   if (pbar->Position >= pbar->Max)
      {//torna este form visível p/ dar ao usuário a opção de cancelar a análise
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
      //Erro("Erro na execução do programa");
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

