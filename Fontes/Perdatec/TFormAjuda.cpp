//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormAjuda.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormAjuda::TFormAjuda(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //salva ponteiro para objeto
   this->apl = apl;
	//posiciona o Form
	FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormAjuda::~TFormAjuda(void)
   {
   //vari�veis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	//salva par�metros do Form
	if (geral) geral->Infoset->SaveToFile(this);
   }

//---------------------------------------------------------------------------
void __fastcall TFormAjuda::ActionFecharExecute(TObject *Sender)
   {
   Close();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormAjuda::CarregaArquivoAjuda(void)
   {
   //vari�veis locais
   VTPath    *path     = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString arq_ajuda = path->DirBase() + "\\Help\\PerdaTecnica.hlp";

   //carrega o arquivo de Help
   if (! FileExists(arq_ajuda))
      {
      Aviso("O arquivo de ajuda n�o est� dispon�vel");
      return(false);
      }
   RichEditHelp->Lines->LoadFromFile(arq_ajuda);
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormAjuda::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAjuda::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form sem alinhamento
   Left = 0;
   Top  = 0;
	}

//---------------------------------------------------------------------------
//eof

