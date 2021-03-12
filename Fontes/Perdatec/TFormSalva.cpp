//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TFormSalva.h"
#include "..\Apl\VTApl.h"
#include "..\BD\VTBD.h"
#include "..\BD\Hist\VTBDhist.h"
#include "..\BDihm\VTSalva.h"
#include "..\BDihm\VTSalva.h"
#include "..\Diretorio\VTDir.h"
#include "..\Diretorio\VTPath.h"
#include "..\Radial\VTArvore.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormSalva::TFormSalva(TComponent* Owner, VTApl *apl_owner)
	: TForm(Owner)
	{
	//salva ponteiro para Apl
	this->apl = apl_owner;
	//cria objetos
	files  = new TStringList();
	lisARV =  new TList();
	//inicia CBoxBase
	CBoxBaseInicia();
	//inicia CBoxData para a base selecionada
	CBoxDataInicia();
	//inicia DateTimePicker
	DateTimePicker->Date = DateOf(Now());
	//pré-seleciona data existente
	if (CBoxData->Items->Count == 0) CheckBoxDataNova->Checked      = true;
	else                             CheckBoxDataExistente->Checked = true;
	}

//---------------------------------------------------------------------------
__fastcall TFormSalva::~TFormSalva(void)
	{
	//destrói objetos
	if (files) {delete files; files = NULL;}
	//destrói lista e seus objetos
	if (lisARV) {LimpaTList(lisARV); delete lisARV; lisARV = NULL;}
	}

//---------------------------------------------------------------------------
void __fastcall TFormSalva::ActionCancelaExecute(TObject *Sender)
	{
	//fecha o form
	ModalResult = mrCancel;
	}

//---------------------------------------------------------------------------
void __fastcall TFormSalva::ActionConfirmaExecute(TObject *Sender)
	{
	//salva dados de Perdas na base
	if (SalvaPerdasNaBase())
		{
		ModalResult = mrOk;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormSalva::ActionCriaBaseNovaExecute(TObject *Sender)
	{
	//variaveis locais
	WideString  hist_model, hist;
	VTPath      *path = (VTPath*)apl->GetObject(__classid(VTPath));

	//esconde este Form
	Visible = false;
	//inicia dialog
   SaveDialog->InitialDir = ExtractFileDir(path->ArqHistorico);
   SaveDialog->DefaultExt = ".mdb";
   SaveDialog->Filter = "Base Historico (*.mdb)|*.mdb|Todos(*.*)|*.*";
   if(SaveDialog->Execute())
      {
      hist = SaveDialog->FileName;
      //copia base de dado modelo (vazio) para o Historico Novo
		hist_model = path->DirDat() + "\\Historico\\HistoricoVazio.mdb";
		CopyFile(hist_model.c_bstr(), hist.c_bstr(), false);
		//inclui nova Base em CBoxBase
		CBoxBaseInicia(hist);
		}
	//torna a exibir este Form
	Visible = true;
	}

//---------------------------------------------------------------------------
void __fastcall TFormSalva::CBoxBaseChange(TObject *Sender)
	{
	//variáveis locais
	AnsiString  arquivo;

	//verifica se há uma base selecionada
	if (CBoxBase->ItemIndex < 0) return;
	//reinicia CBoxData
	CBoxDataInicia();
	}

//---------------------------------------------------------------------------
void __fastcall TFormSalva::CBoxBaseInicia(AnsiString base_ref)
	{
	//variáveis locais
	AnsiString  arquivo;
	VTDir       *dir  = (VTDir*)apl->GetObject(__classid(VTDir));
	VTPath      *path = (VTPath*)apl->GetObject(__classid(VTPath));

	//obtém lista de bases de dados de histórico
	files->Clear();
	dir->Arquivos(path->DirHistorico(), files, "*.mdb");
	//reinicia CBoxBase
	CBoxBase->Items->Clear();
	for (int n = 0; n < files->Count; n++)
		{//filtra arquivo de Historico vazio
		arquivo = ChangeFileExt(files->Strings[n], "");
		if (arquivo.AnsiCompareIC("HistoricoVazio") == 0) continue;
		CBoxBase->Items->Add(arquivo);
		}
	//pré-seleciona arquivo Historico
	CBoxBase->ItemIndex = CBoxBase->Items->IndexOf(base_ref);
	}

//---------------------------------------------------------------------------
void __fastcall TFormSalva::CBoxDataInicia(void)
	{
	//variáveis locais
	VTArvore *arvore;
	VTBD     *bd = (VTBD*)apl->GetObject(__classid(VTBD));

	//reinicia CBoxData
	CBoxData->Items->Clear();
	//determina base de dados selecionada
	if (! ExisteBaseSelecionada()) return;
	//abre base de dados
	if (! bd->ConectaBaseHist()) return;
	//obtém lista de Árvores já existentes na base
	LimpaTList(lisARV);
	bd->Hist->LeListaArvore(lisARV);
	for (int n = 0; n < lisARV->Count; n++)
		{
		arvore = (VTArvore*)lisARV->Items[n];
		CBoxData->Items->AddObject(DateToStr(arvore->Data), arvore);
		}
	//pré seleciona última Data
	if (CBoxData->Items->Count > 0)
		{
		CBoxData->ItemIndex = CBoxData->Items->Count - 1;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormSalva::CheckBoxDataExistenteClick(TObject *Sender)
	{
	//habilita/desabilita CBoxData
	CBoxData->Enabled = CheckBoxDataExistente->Checked;
	//altera CheckBoxDataNova
	CheckBoxDataNova->Checked = ! CheckBoxDataExistente->Checked;
	}

//---------------------------------------------------------------------------
void __fastcall TFormSalva::CheckBoxDataNovaClick(TObject *Sender)
	{
	//habilita/desabilita DateTimePicker
	DateTimePicker->Enabled = CheckBoxDataNova->Checked;
	//altera CheckBoxDataExistente
	CheckBoxDataExistente->Checked = ! CheckBoxDataNova->Checked;
	}

//---------------------------------------------------------------------------
void __fastcall TFormSalva::DateTimePickerChange(TObject *Sender)
	{
	/*
	//variáveis locais
	int           index;
	UnicodeString data;

	//verifica se a Data já existe em RadioGropuData
	data = FormataData(DateTimePicker->Date);
	if ((index = CBoxData->Items->IndexOf(data)) < 0)
		{//inclui Data em RadioGropuData
		index = CBoxData->Items->Add(data);
		CBoxData->ItemIndex = index;
		}
	*/
	}

//---------------------------------------------------------------------------
bool __fastcall TFormSalva::ExisteBaseSelecionada(void)
	{
	//variáveis locais
	AnsiString filename;
	VTPath     *path   = (VTPath*)apl->GetObject(__classid(VTPath));

	//verifica se foi selecionada uma base
	if (CBoxBase->ItemIndex < 0) return(false);
	//define pathname da base de dados
	filename = CBoxBase->Items->Strings[CBoxBase->ItemIndex]  + ".mdb";
	//atualiza objeto Path
	path->ArqHistorico = path->DirHistorico() + "\\" + filename;

	return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormSalva::SalvaPerdasNaBase(void)
	{
	//variáveis locais
	bool       sucesso;
	AnsiString base_historico;
	TDateTime  data_ref;
	VTArvore   *arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
	VTSalva    *salva  = (VTSalva*)apl->GetObject(__classid(VTSalva));

	try{//verifica se foi selecionada uma base
		if (! ExisteBaseSelecionada())
			{
			Aviso("Selecione uma base consolidada de perdas técnicas ");
			return(false);
			}
		//verifica se está usando uma data existente
		if (CheckBoxDataExistente->Checked)
			{//determina data existente selecionada
			if (CBoxData->ItemIndex < 0)
				{
				Aviso("Selecione a data de referência para as perdas técnicas ");
				return(false);
				}
			data_ref = StrToDate(CBoxData->Items->Strings[CBoxData->ItemIndex]);
			}
		else
			{//determina nova data indicada
			data_ref = DateOf(DateTimePicker->Date);
			}
		//atualiza informações da Arvore
		arvore->TipoArvore = arvorePERDATEC;
		arvore->Data       = data_ref;
		//salvar dados na base
		sucesso = salva->SalvaHistoricoPerdaTecnica(arvore);
		}catch(Exception &e)
			{
			sucesso = false;
			}
	//gera aviso ao usuário
	if (sucesso) Aviso("Os dados de perdas foram salvos na base de dados");
	else         Erro ("Os dados de perdas  não foram salvos na base");

	return(sucesso);
	}

//---------------------------------------------------------------------------
//eof


