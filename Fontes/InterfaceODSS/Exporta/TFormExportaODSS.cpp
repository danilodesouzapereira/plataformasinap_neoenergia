//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\Licenca\VTLicenca.h>
#include <Fontes\Licenca\VTLicencas.h>
#include <Fontes\Rede\VTRedes.h>
#include <DLL_Inc\Funcao.h>
#include "TFormExportaODSS.h"
//#include "VTExportaODSS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormExportaODSS::TFormExportaODSS(TComponent *Owner, VTApl *apl)
   : TForm(Owner)
   {
   //salva dados recebidos
   this->apl     = apl;
   try{//cria objetos
      exportaODSS = NewObjExportaODSS(apl);
      //configura componentes da tela
      ConfiguraTela();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TFormExportaODSS::~TFormExportaODSS(void)
   {
   //destrói objetos
   if (exportaODSS) {delete exportaODSS; exportaODSS = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaODSS::ActionCancelaExecute(TObject *Sender)
   {
//   ModalResult = mrCancel;
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaODSS::ActionExportaExecute(TObject *Sender)
   {
	//variáveis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	//verifica modelo de rede selecionado
	if (RadioGroupModelo->ItemIndex < 0)
		{
		Aviso("Selecione a modelagem de rede: modelo ANEEL ou cadastro GIS");
		return;
		}
	opcao.rede_aneel = (RadioGroupModelo->ItemIndex == 0);
	//verifica ligação dos trafos DY
	if (RadioGroupTrafoDY->ItemIndex < 0)
		{
		Aviso("Selecione a ligação dos trafos Delta/Estrela");
		return;
		}
	opcao.trafoDY = RadioGroupTrafoDY->ItemIndex;
	//verifica opção de funidr as cargas de umna mesma barra
   if (RadioGroupCargaEqv->ItemIndex < 0)
      {
      Aviso("Selecione a opção de cargas de uma mesma barra");
      return;
      }
   opcao.carga_eqv = RadioGroupCargaEqv->ItemIndex;
   //verifica se o arquivo TXT a ser gravado está definido
   if (arq_txt.IsEmpty())
      {//define arquivo TXT
      arq_txt = path->DirExporta() + "\\Sinap_" + ChangeFileExt(ExtractFileName(path->ArqRede), ".dss");
      }
   //pede p/ usuário confirmar
   SaveDialog->InitialDir = path->DirExporta();
   SaveDialog->FileName   = arq_txt;
   if (! SaveDialog->Execute())
      {//fecha o form
//      ModalResult = mrCancel;
      return;
      }
   //salva nome do arquivo XML
   arq_txt = SaveDialog->FileName;
   //executa exportação
	if (ExportaArquivo(arq_txt))
      {
      Aviso("Os dados foram gravados corretamente no arquivo:\n" + arq_txt);
//      ModalResult = mrOk;
      }
   else
      {
      Erro("Não foi possível exportar os dados");
//      ModalResult = mrCancel;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaODSS::ConfiguraTela(void)
   {
   //nada a fazer por enquanto
   }

//---------------------------------------------------------------------------
bool __fastcall TFormExportaODSS::ExportaArquivo(AnsiString arq_txt)
   {
   //variáveis locais
   bool sucesso;

   //configura opções do arquivo XML
   sucesso = exportaODSS->ExportaRedes(arq_txt, opcao);

   return(sucesso);
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaODSS::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
//   Action = caHide;
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaODSS::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form no canto superior esquerdo
   Left = 0;
   Top  = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormExportaODSS::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
//eof


