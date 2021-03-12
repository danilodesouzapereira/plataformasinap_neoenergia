//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TFormExportaANEEL.h"
#include "VTExportaANEEL.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormExportaANEEL::TFormExportaANEEL(TComponent *Owner, VTApl *apl, AnsiString arq_txt)
   : TForm(Owner)
   {
   //salva dados recebidos
   this->apl     = apl;
   this->arq_txt = arq_txt;
   try{//cria objetos
      exporta = NewObjExportaANEEL(apl);
      //configura componentes da tela
      ConfiguraTela();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TFormExportaANEEL::~TFormExportaANEEL(void)
   {
   //destr�i objetos
   if (exporta) {delete exporta; exporta = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaANEEL::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaANEEL::ActionExportaExecute(TObject *Sender)
   {
	//vari�veis locais
   VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //verifica se o arquivo TXT a ser gravado est� definido
   if (arq_txt.IsEmpty())
      {//define arquivo TXT
      arq_txt = path->DirExporta() + "\\" + ExtractFileName(path->DirRedeBase()) + ".txt";
      }
   //pede p/ usu�rio confirmar
   SaveDialog->InitialDir = path->DirExporta();
   SaveDialog->FileName   = arq_txt;
   if (! SaveDialog->Execute())
      {//fecha o form
      ModalResult = mrCancel;
      return;
      }
   //salva nome do arquivo XML
   arq_txt = SaveDialog->FileName;
   //executa exporta��o
	if (ExportaArquivo(arq_txt, DateTimePicker->Date))
      {
      Aviso("Os dados foram gravados corretamente no arquivo:\n" + arq_txt);
      ModalResult = mrOk;
      }
   else
      {
      Erro("N�o foi poss�vel exportar os dados");
      ModalResult = mrCancel;
      }
	}

//---------------------------------------------------------------------------
void __fastcall TFormExportaANEEL::ConfiguraTela(void)
	{
	//atualiza DateTimePicker->Date
	DateTimePicker->Date = DateOf(Now());
	}

//---------------------------------------------------------------------------
bool __fastcall TFormExportaANEEL::ExportaArquivo(AnsiString arq_txt, TDateTime data)
	{
   //vari�veis locais
   bool sucesso;

	//configura op��es do arquivo
	sucesso = exporta->ExportaRedes(arq_txt, data);

   return(sucesso);
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaANEEL::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaANEEL::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form no canto superior esquerdo
   Left = 0;
   Top  = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaANEEL::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
//eof

