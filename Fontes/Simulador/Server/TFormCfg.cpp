//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Editor\VTEdt.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Rede\VTCapacitor.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTRedes.h>
#include <PlataformaSinap\Fontes\Rede\VTTrafo.h>
#include <PlataformaSinap\Fontes\Rede\VTTipoChave.h>
#include <PlataformaSinap\DLL_Inc\Editor.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include "TFormCfg.h"
#include "VTSinapServer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCfg::TFormCfg(TComponent* Owner, VTApl *apl_owner, TWinControl *parent, TAction *ActionOnClose)
	: TFormEdt(Owner, parent)
	{
	//salva ponteiro para objetos
	this->apl           = apl_owner;
	this->ActionOnClose = ActionOnClose;
	server              = (VTSinapServer*)apl->GetObject(__classid(VTSinapServer));
	//cria objetos
	lisEQP = new TList();
	//inicia CLBoxChave, CLBoxTrafo e CLBoxCapacitor
	CLBoxChaveInicia();
	CLBoxTrafoInicia();
   CLBoxCapacitorInicia();
	}

//---------------------------------------------------------------------------
__fastcall TFormCfg::~TFormCfg(void)
	{
	//canecla moldura
	Moldura(NULL);
	//destr�i objetos
	if (edt) {delete edt; edt = NULL;}
	//destr�i lista sem destruir seus objetos
	if (lisEQP) {delete lisEQP; lisEQP = NULL;}
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::ActionArqChaveLeExecute(TObject *Sender)
	{
	//vari�veis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	//solicita nome do arquivo ao usu�rio
	if (OpenDialog->InitialDir.IsEmpty())
		{
		SaveDialog->InitialDir = path->DirImporta();
		}
	if (OpenDialog->Execute())
		{//salva arquivo
		LeArquivoSupervisaoAutomatica(OpenDialog->FileName);
		//verifica se foram lidos Eqptos
		if (lisEQP->Count > 0)
			{//atualiza CLBoxChave e CLBoxTrafo
			CLBoxChaveAtualiza(lisEQP);
			}
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::ActionArqChaveSalvaExecute(TObject *Sender)
	{
	//vari�veis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	//verifica se h� Chaves selecionadas
	if (! ValidaSelecao()) return;
	//solicita nome do arquivo ao usu�rio
	if (SaveDialog->FileName.IsEmpty())
		{
		SaveDialog->FileName = path->DirImporta() + "\\SupervisaoAutomatica.txt";
		}
	if (SaveDialog->Execute())
		{//salva arquivo
		SalvaArquivoSupervisaoAutomatica(SaveDialog->FileName);
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::ActionCancelaExecute(TObject *Sender)
	{
	//cancela todos Eqptos selecionados
	ActionCapacitorSelNone->Execute();
	ActionChaveSelNone->Execute();
	ActionTrafoSelNone->Execute();
	//sinaliza fechamento do Form
	if (ActionOnClose) ActionOnClose->OnExecute((TObject*)0);
	//fecha  form
	Close();
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::ActionConfirmaExecute(TObject *Sender)
	{
	//verifica se h� Chaves selecionadas
	if (! ValidaSelecao()) return;
	//inicia a supervis�o autom�tica
	server->SupervisaoAutomaticaStart(lisEQP);
	//sinaliza fechamento do Form
	if (ActionOnClose) ActionOnClose->OnExecute((TObject*)1);
	//fecha  form
	Close();
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::ActionCapacitorSelAllExecute(TObject *Sender)
	{
	//vari�veis locais
   TCheckListBox *CLBox = CLBoxCapacitor;

	//seleciona todos Eqptos
	for (int n = 0; n < CLBox->Items->Count; n++) CLBox->Checked[n] = true;
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::ActionCapacitorSelNoneExecute(TObject *Sender)
	{
	//vari�veis locais
   TCheckListBox *CLBox = CLBoxCapacitor;

	//cancela sele��o de todos Eqptos
	for (int n = 0; n < CLBox->Items->Count; n++) CLBox->Checked[n] = false;
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::ActionChaveSelAllExecute(TObject *Sender)
	{
	//vari�veis locais
   TCheckListBox *CLBox = CLBoxChave;

	//seleciona todos Eqptos
	for (int n = 0; n < CLBox->Items->Count; n++) CLBox->Checked[n] = true;
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::ActionChaveSelNoneExecute(TObject *Sender)
	{
	//vari�veis locais
   TCheckListBox *CLBox = CLBoxChave;

	//cancela sele��o de todos Eqptos
	for (int n = 0; n < CLBox->Items->Count; n++) CLBox->Checked[n] = false;
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::ActionTrafoSelAllExecute(TObject *Sender)
	{
	//vari�veis locais
   TCheckListBox *CLBox = CLBoxTrafo;

	//seleciona todos Eqptos
	for (int n = 0; n < CLBox->Items->Count; n++) CLBox->Checked[n] = true;
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::ActionTrafoSelNoneExecute(TObject *Sender)
	{
	//vari�veis locais
   TCheckListBox *CLBox = CLBoxTrafo;

	//cancela sele��o de todos Eqptos
	for (int n = 0; n < CLBox->Items->Count; n++) CLBox->Checked[n] = false;
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::ActionEdtExecute(TObject *Sender)
	{
	try{//se necess�rio, cria um objeto TEdtSpy
		if (edt != NULL) delete edt;
		//cria EdtBusca
		edt = DLL_NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionEqptoSelMouse);
		//atualiza indica��o de mouse ativo e for�a bot�o Down
		AtualizaIndicacaoEditorOn(butEdt);
		} catch (Exception &e)
			{//nada a fazer
			}
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::ActionEqptoSelMouseExecute(TObject *Sender)
	{
	//vari�veis locais
	int     index;
	VTEqpto *eqpto;
	TList   *lisEXT = (TList*)Sender;

	//prote��o
	if (lisEXT == NULL) return;
	if (lisEXT->Count == 0) return;
	//loop p/ todos Eqptos da lista
	for (int n = 0; n < lisEXT->Count; n++)
		{
		eqpto = (VTEqpto*)lisEXT->Items[n];
		if (lisEQP->IndexOf(eqpto) < 0)
			{//insere Eqpto em lisEQP
			lisEQP->Add(eqpto);
			}
		}
	//atulzia CLBoxChave
	CLBoxChaveAtualiza(lisEQP);
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::CLBoxCapacitorAtualiza(TList *lisCAP)
	{
	//vari�veis locais
	int     index;
	VTEqpto *eqpto;

	//marcas os Capacitores de lisEQP
	for (int n = 0; n < lisCAP->Count; n++)
		{
		eqpto = (VTEqpto*)lisCAP->Items[n];
      //verifica se � uma Capacitor
      if (eqpto->Tipo() == eqptoCAPACITOR)
         {
		   index = CLBoxCapacitor->Items->IndexOfObject(eqpto);
		   if (index >= 0) CLBoxCapacitor->Checked[index] = true;
         }
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::CLBoxCapacitorInicia(void)
	{
	//  vari�veis locais
	VTCapacitor *capacitor;
	VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//obt�m lista de todas Capacitores
	lisEQP->Clear();
	redes->LisEqbar(lisEQP, eqptoCAPACITOR);
	//reinicia CLBoxCapacitor e cancela ordena��o
	CLBoxCapacitor->Clear();
	CLBoxCapacitor->Sorted = false;
	//insere em CLBoxCapacitor as chaves de lisEQP
	for (int n = 0; n < lisEQP->Count; n++)
		{
		capacitor = (VTCapacitor*)lisEQP->Items[n];
      CLBoxCapacitor->Items->AddObject(capacitor->Codigo, capacitor);
		}
	//ordena CLBoxCapacitor
	CLBoxCapacitor->Sorted = true;
	//reinicia lisEQP
	lisEQP->Clear();
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::CLBoxChaveAtualiza(TList *lisCHV)
	{
	//vari�veis locais
	int     index;
	VTEqpto *eqpto;

	//marcas as chaves de lisEQP
	for (int n = 0; n < lisCHV->Count; n++)
		{
		eqpto = (VTEqpto*)lisCHV->Items[n];
      //verifica se � uma Chave
      if (eqpto->Tipo() == eqptoCHAVE)
         {
		   index = CLBoxChave->Items->IndexOfObject(eqpto);
		   if (index >= 0) CLBoxChave->Checked[index] = true;
         }
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::CLBoxChaveInicia(void)
	{
	//  vari�veis locais
	VTChave *chave;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//obt�m lista de todas Chaves
	lisEQP->Clear();
	redes->LisLigacao(lisEQP, eqptoCHAVE);
	//reinicia CLBoxChave e cancela ordena��o
	CLBoxChave->Clear();
	CLBoxChave->Sorted = false;
	//insere em CLBoxChave as chaves de lisEQP
	for (int n = 0; n < lisEQP->Count; n++)
		{
		chave = (VTChave*)lisEQP->Items[n];
		if ((chave->TipoChave->Tipo == chaveDJ)||
          (chave->TipoChave->Tipo == chaveRA)||
          (chave->TipoChave->Tipo == chaveNETWORK_PROTECTOR))
			{
			CLBoxChave->Items->AddObject(chave->Codigo, chave);
         }
		}
	//ordena CLBoxChave
	CLBoxChave->Sorted = true;
	//reinicia lisEQP
	lisEQP->Clear();
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::CLBoxClick(TObject *Sender)
	{
	//vari�veis locais
	VTEqpto       *eqpto;
   TCheckListBox *CLBox = (TCheckListBox*)Sender;

   //prote��o
   if (CLBox == NULL) return;
	//determina Eqpto selecionada
	if (CLBox->ItemIndex < 0) return;
	eqpto = (VTEqpto*)(CLBox->Items->Objects[CLBox->ItemIndex]);
	//exibe moldura
	Moldura(eqpto);
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::CLBoxDblClick(TObject *Sender)
	{
	//vari�veis locais
	VTEqpto       *eqpto;
   TCheckListBox *CLBox = (TCheckListBox*)Sender;
	VTGrafico     *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //prote��o
   if (CLBox == NULL) return;
	//determina Eqpto selecionada
	if (CLBox->ItemIndex < 0) return;
	eqpto = (VTEqpto*)(CLBox->Items->Objects[CLBox->ItemIndex]);
	//seleciona �rea de zoom
	grafico->SelecionaZoom(eqpto);
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::CLBoxTrafoAtualiza(TList *lisTRF)
	{
	//vari�veis locais
	int     index;
	VTEqpto *eqpto;

	//marcas as chaves de lisEQP
	for (int n = 0; n < lisTRF->Count; n++)
		{
		eqpto = (VTEqpto*)lisTRF->Items[n];
      //verifica se o Eqpto � um Trafo
      if (eqpto->Tipo() == eqptoTRAFO)
         {
		   index = CLBoxTrafo->Items->IndexOfObject(eqpto);
		   if (index >= 0) CLBoxTrafo->Checked[index] = true;
         }
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::CLBoxTrafoInicia(void)
	{
	//  vari�veis locais
	VTTrafo *trafo;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//obt�m lista de todos Trafos
	lisEQP->Clear();
	redes->LisLigacao(lisEQP, eqptoTRAFO);
	//reinicia CLBoxTrafo e cancela ordena��o
	CLBoxTrafo->Clear();
	CLBoxTrafo->Sorted = false;
	//insere em CLBoxTrafo os Trafos de lisEQP
	for (int n = 0; n < lisEQP->Count; n++)
		{
		trafo = (VTTrafo*)lisEQP->Items[n];
      CLBoxTrafo->Items->AddObject(trafo->Codigo, trafo);
		}
	//ordena CLBoxTrafo
	CLBoxTrafo->Sorted = true;
	//reinicia lisEQP
	lisEQP->Clear();
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::FormClose(TObject *Sender, TCloseAction &Action)
	{
	//destr�i objeto Edt
	if (edt) {delete edt; edt = NULL;}
	//sinaliza fechamento do Form
	if (ActionOnClose) ActionOnClose->Execute();
	//esconde o Form
	Action = caHide;
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::FormShow(TObject *Sender)
	{
	//executa ActionEdt p/ obter controle do mouse
	ActionEdt->Execute();
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::LeArquivoSupervisaoAutomatica(AnsiString filename)
	{
	//vari�veis locais
	int        tipo_eqpto;
	AnsiString separador = ";";
	TStrings   *campos, *lines;
	VTEqpto    *eqpto;
	VTRedes    *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//reinicia lisEQP
	lisEQP->Clear();
	//cria objetos
	lines  = new TStringList();
	campos = new TStringList();
	//l� conte�do do arquivo em TStrings
	lines->LoadFromFile(filename);
	//loop p/ todas linhas em lines
	for (int n = 0; n < lines->Count; n++)
		{//extrai campos da linha
		if (ExtraiStrings(lines->Strings[n], separador, campos) == 2)
			{
			tipo_eqpto = StrToInteger(campos->Strings[0]);
			eqpto      = redes->ExisteEqpto(tipo_eqpto, campos->Strings[1]);
			if (eqpto != NULL)
				{
				lisEQP->Add(eqpto);
				}
			}
		}
	//destr�i objeto
	delete lines;
	delete campos;
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::Moldura(VTEqpto *eqpto)
	{
	//vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	//exibe moldura
	grafico->Moldura(eqpto);
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::SalvaArquivoSupervisaoAutomatica(AnsiString filename)
	{
	//vari�veis locais
	char     separador = ';';
	TStrings *lines;
	VTEqpto  *eqpto;

	//cria objeto
	lines = new TStringList();
	//loop p/ todos Eqptos em lisEQP
	for (int n = 0; n < lisEQP->Count; n++)
		{
		eqpto = (VTChave*)lisEQP->Items[n];
		//inclui uma linha com tipo de eqpto e seu c�digo
		lines->Add(IntToStr(eqpto->Tipo()) + separador + eqpto->Codigo + separador);
		}
	//salva TStrings eno arquivo
	lines->SaveToFile(filename);
	//destr�i objeto
	delete lines;
	}

//---------------------------------------------------------------------------
void __fastcall TFormCfg::Stop(void)
	{
	//finaliza a supervis�o autom�tica
	server->SupervisaoAutomaticaStop();
	//cancela todos Eqptos selecionados
	lisEQP->Clear();
	ActionCapacitorSelNone->Execute();
	ActionChaveSelNone->Execute();
	ActionTrafoSelNone->Execute();
	}

//---------------------------------------------------------------------------
bool __fastcall TFormCfg::ValidaSelecao(void)
	{
   //reinicia lisEQP com Chaves e Trafos selecionados
	lisEQP->Clear();
	//determina conjunto de Chaves selecionadas
	for (int n = 0; n < CLBoxChave->Items->Count; n++)
		{
		if (CLBoxChave->Checked[n])
			{
			lisEQP->Add(CLBoxChave->Items->Objects[n]);
			}
		}
	//determina conjunto de Trafos selecionados
	for (int n = 0; n < CLBoxTrafo->Items->Count; n++)
		{
		if (CLBoxTrafo->Checked[n])
			{
			lisEQP->Add(CLBoxTrafo->Items->Objects[n]);
			}
		}
	//determina conjunto de Capacitores selecionados
	for (int n = 0; n < CLBoxCapacitor->Items->Count; n++)
		{
		if (CLBoxCapacitor->Checked[n])
			{
			lisEQP->Add(CLBoxCapacitor->Items->Objects[n]);
			}
		}
	//verifica se h� pelo menos um Eqpto selecionada
	if (lisEQP->Count == 0)
		{
		Aviso("N�o h� eqpto selecionado para supervis�o");
		return(false);
		}
	return(true);
	}

//---------------------------------------------------------------------------
//eof


