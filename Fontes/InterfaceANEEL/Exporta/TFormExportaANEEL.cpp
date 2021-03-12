//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TFormExportaANEEL.h"
#include "VTExportaANEEL.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Diretorio\VTPath.h"
#include "..\..\Geral\VTGeral.h"
#include "..\..\Geral\VTInfoset.h"
#include "..\..\ImportaRedeGIS\VTImportaRedeGIS.h"
#include "..\..\Licenca\VTLicenca.h"
#include "..\..\Licenca\VTLicencas.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormExportaANEEL::TFormExportaANEEL(TComponent *Owner, VTApl *apl, AnsiString filename)
   : TForm(Owner)
   {
   //salva dados recebidos
	this->apl      = apl;
	this->filename = filename;
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
   //destrói objetos
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
	//variáveis locais
	VTPath    *path = (VTPath*)apl->GetObject(__classid(VTPath));

	//verifica modelo de rede selecionado
	if (RadioGroupModelo->ItemIndex < 0)
		{
		Aviso("Selecione a modelagem de rede: modelo ANEEL ou cadastro GIS");
		return;
		}
	//verifica ligação dos trafos DY
	if (RadioGroupTrafoDY->ItemIndex < 0)
		{
		Aviso("Selecione a lgação dos trafos Delta/Estrela");
		return;
		}
	//verifica formato do campo de acoplamento
	if (RadioGroupCampoAcoplamento->ItemIndex < 0)
		{
		Aviso("Selecione o formato do campo 'código do ponto de acoplamento'");
		return;
		}
	//verifica se o arquivo MDB a ser gravado está definido
	if (filename.IsEmpty())
		{//define arquivo TXT
		filename = path->DirExporta() + "\\Validador_" + ChangeFileExt(ExtractFileName(path->ArqRede), "");
      }
   //pede p/ usuário confirmar
   SaveDialog->InitialDir = path->DirExporta();
	SaveDialog->FileName   = ExtractFileName(filename);
   if (! SaveDialog->Execute())
      {//fecha o form
      ModalResult = mrCancel;
      return;
      }
   //salva nome do arquivo XML
	filename = SaveDialog->FileName;
   //executa exportação
	if (ExportaArquivo(filename))
      {
		Aviso("Os dados foram gravados corretamente no arquivo:\n" + filename);
      ModalResult = mrOk;
      }
   else
      {
      Erro("Não foi possível exportar os dados");
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
bool __fastcall TFormExportaANEEL::CopiaBaseVazia(WideString arq_dest)
	{
	//variáveis locais
	bool       fail_exist = false;
	VTPath     *path      = (VTPath*)apl->GetObject(__classid(VTPath));
	WideString base_vazia = path->DirDat() + "\\Aneel\\AneelValidadorOpenDSS.mdb";

	//proteção
	if (! FileExists(base_vazia))
		{
		Aviso("Não existe a base de dados ANEEL:\n" + base_vazia);
		return(false);
		}
	//copia arquivo base vazia
	return(CopyFile(base_vazia.c_bstr(), arq_dest.c_bstr(), fail_exist));
	}

//---------------------------------------------------------------------------
bool __fastcall TFormExportaANEEL::ExportaArquivo(AnsiString filename)
	{
	//variáveis locais
	bool           sucesso;
	strOPCAO_ANEEL opcao;
	//copia base de dados vazia
	if (! CopiaBaseVazia(filename)) return(false);
	//configura opções
	opcao.rede_aneel = (RadioGroupModelo->ItemIndex == 0);
	opcao.trafoDY    = RadioGroupTrafoDY->ItemIndex;
	opcao.fmt_codigo = RadioGroupCampoAcoplamento->ItemIndex;
	//executa exportação
	sucesso = exporta->ExportaRedes(filename, opcao);

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
void __fastcall TFormExportaANEEL::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
//eof


