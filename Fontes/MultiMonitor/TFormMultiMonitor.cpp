//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormMultiMonitor.h"
#include "VTMultiPanel.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormMultiMonitor::TFormMultiMonitor(TComponent* Owner)
   : TForm(Owner)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TFormMultiMonitor::~TFormMultiMonitor(void)
   {
   //OBS: não precisa destruir os Panels inseridos neste Form
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormMultiMonitor::ExibePanel2(VTMultiPanel *MultiPanel)
   {
   //proteção: verifica se MultiPanel já está ativo
   if (MultiPanelAtivo == MultiPanel) return;
   //verifica se há um MultiPanel ativo e esconde seu Panel2
   if (MultiPanelAtivo) MultiPanelAtivo->Panel2->Hide();
   //salva MultiPanel ativo
   MultiPanelAtivo = MultiPanel;
   //redefine MultiPanel ativo e exibe seu Panel2
   if (Ativo) MultiPanelAtivo->Panel2->Show();
   //habilita/desabilita Panel2 do MultiPanel ativo
   MultiPanelAtivo->Panel2_Enabled = Ativo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMultiMonitor::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }
   
//---------------------------------------------------------------------------
VTMultiPanel* __fastcall TFormMultiMonitor::InsereMultiPanel(TPanel *panel1)
   {
   //variáveis locais
   VTMultiPanel *MultiPanel;

   try{//cria um novo objeto MultiPanel
      MultiPanel = NewObjMultiPanel();
      //insere Painel1 no MultiPanel criado
      MultiPanel->Panel1          = panel1;
      //cria Panel2 do MultiPanel
      MultiPanel->Panel2          = new TPanel((TComponent*)NULL);
      MultiPanel->Panel2->Parent  = this;
      MultiPanel->Panel2->Visible = false;
      MultiPanel->Panel2->Align   = alClient;
      //indica ao MultiPanel se o segundo monitor está ativo
      MultiPanel->Panel2_Enabled = Ativo;
      }catch(Exception &e)
         {
         }
   return(MultiPanel);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormMultiMonitor::PM_GetAtivo(void)
   {
   return(PD.ativo);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMultiMonitor::PM_SetAtivo(bool ativo)
   {
   //atualiza estado
   PD.ativo = ativo;
   //habilita/desabilita Panel2 do MultiPanel ativo
   if (MultiPanelAtivo)
      {
      MultiPanelAtivo->Panel2->Visible = ativo;
      MultiPanelAtivo->Panel2_Enabled  = ativo;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMultiMonitor::RetiraMultiPanel(VTMultiPanel *MultiPanel)
   {
   //cacela MultiPanel ativo
   MultiPanelAtivo = NULL;
   }

//---------------------------------------------------------------------------
//eof


