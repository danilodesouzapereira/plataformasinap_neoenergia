//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEstudoRnt.h"
#include "VTAbre.h"
#include "..\Apl\VTApl.h"
//#include "..\Arranjo\VTArranjos.h"
#include "..\BD\VTBD.h"
//#include "..\BD\Rede\VTBDgeral.h"
#include "..\BD\Rede\VTBDeqpto.h"
#include "..\BD\VTSNPDBConn.h"
//#include "..\Classes\VTClasses.h"
//#include "..\Curvas\VTCurvas.h"
#include "..\Diretorio\VTPath.h"
//#include "..\Editor\VTEdt.h"
#include "..\Geral\VTGeral.h"
//#include "..\Grafico\VTGrafico.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
//#include "..\Mercado\VTMercado.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
//#include "..\Progresso\VTProgresso.h"
//#include "..\Rede\VTDominio.h"
//#include "..\Rede\VTTipoRede.h"
//#include "..\Rede\VTRede.h"
//#include "..\Rede\VTRedes.h"
//#include "..\Rede\VTTipos.h"
#include "..\..\DLL_Inc\BD.h"
//#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Funcao.h"
//#include "..\..\DLL_Inc\Grafico.h"
//#include "..\..\DLL_Inc\Progresso.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEstudoRnt::TFormEstudoRnt(TComponent* Owner, VTApl *apl_owner, int tipo_estudo)
   : TForm(Owner)
	{
	//prote��o
	if ((tipo_estudo < estudoREDE_ORIGINAL)&&(tipo_estudo > estudoRECLAMACAO_MT))
		{
		tipo_estudo = estudoRECLAMACAO_BT;
		}
	//salva tipo de estudo
	this->tipo_estudo = tipo_estudo;
	//cria objeto Apl local
	apl = NewObjApl(this, apl_owner);
	//cria objeto Abre
	apl->Add(abre = NewObjAbre(this, apl));
	//cria listas
	lisEQP = new TList();
	//inicia dados
	estudo_rnt = NULL;
	//atualiza Caption do Form
	switch(tipo_estudo)
		{
		case estudoREDE_ORIGINAL: Caption = "Abrir Rede Original";             break;
		case estudoPLANEJAMENTO:  Caption = "Abrir Estudo de Planejamento";    break;
		case estudoRECLAMACAO_BT: Caption = "Abrir Estudo de Reclama��o SDBT"; break;
		case estudoRECLAMACAO_MT: Caption = "Abrir Estudo de Reclama��o SDMT"; break;
		}
	}

//---------------------------------------------------------------------------
__fastcall TFormEstudoRnt::~TFormEstudoRnt(void)
   {
	//destr�i objetos
	if (apl) {delete apl; apl = NULL;}
	//destr�i lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudoRnt::ActionAbrirEstudoExecute(TObject *Sender)
	{
	//prote��o
	if (estudo_rnt == NULL)
		{
		Aviso("Selecione um estudo");
		return;
		}
	//abre o estudo selecionado
	if (abre->CarregaEstudoExistente(estudo_rnt))
      {//fecha este form
		ModalResult = mrOk;
		}
	else
		{
		Aviso("N�o foi poss�vel abrir o estudo");
      }
	}

//---------------------------------------------------------------------------
void __fastcall TFormEstudoRnt::ActionSelecionaBaseExecute(TObject *Sender)
   {
   //seleciona a base da Rede
   if (SelecionaBaseRede())
      {
      ExibeDadosBaseSelecionada();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEstudoRnt::ExibeDadosBaseSelecionada(void)
	{
   //vari�veis locais
	VTGeral *geral = (VTGeral*)abre->apl->GetObject(__classid(VTGeral));
	VTPath  *path  = (VTPath*)abre->apl->GetObject(__classid(VTPath));

	//cancela ponteiro para o estudo selecionado
	estudo_rnt = NULL;
   //verifica a base atual
   if (geral->BaseDado == SNPDBTYPE_ORACLE)
		{//atualiza StatusBar c/ path da base de dados
		StatusBar->SimpleText = "Base Oracle";
		//atualiza Caption do GroupBox base de dados
		GBoxEstudo->Caption = "Base Oracle";
		//inicia conex�o com a base selecionada
		if (abre->IniciaConexao())
			{//exibe dados da base selecionada
			ExibeEstudo();
			}
      }
	else if (geral->BaseDado == SNPDBTYPE_ACCESS)
      {//atualiza base de dados selecionada
      path->ArqRede = OpenDialog->FileName;
      //atualiza StatusBar c/ path da base de dados
      StatusBar->SimpleText = path->ArqRede;
      //verifica se o arquivo selecionado � de uma base v�lida
      if (ValidaBaseRede(path->ArqRede))
         {//inicia conex�o com a base selecionada
			if (abre->IniciaConexao())
				{//exibe estudo da base selecionada
				ExibeEstudo();
				}
			}
		else
			{//esconde Panel com dados do estudo
			PanelEstudo->Visible = false;
         }
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormEstudoRnt::ActionCancelaExecute(TObject *Sender)
   {
   //fecha o Form
   ModalResult = mrCancel;
	}

//---------------------------------------------------------------------------
void __fastcall TFormEstudoRnt::DirectoryListBoxChange(TObject *Sender)
	{
	//trata sele��o de nova base de dados
	ActionBDChange->Execute();
	}

//---------------------------------------------------------------------------
void __fastcall TFormEstudoRnt::ExibeEstudo(void)
   {
	//verifica se existe um �nico estudo RNT
	if ((estudo_rnt = ExisteEstudoRNT()) == NULL)
		{//esconde Panel com dados do estudo
		PanelEstudo->Visible = false;
		return;
		}
	//exibe Panel com dados do estudo
	PanelEstudo->Visible = true;
	//exibe dados do Estudo
	EditCodigo->Text = estudo_rnt->Codigo;
	EditAutor->Text  = estudo_rnt->Autor;
	}

//---------------------------------------------------------------------------
VTEstudo* __fastcall TFormEstudoRnt::ExisteEstudoRNT(void)
	{
	//vari�veis locais
	TList          *lisESTUDO;
	VTEstudo       *estudo;
	VTBD           *bd           = (VTBD*)abre->apl->GetObject(__classid(VTBD));
	VTPlanejamento *planejamento = (VTPlanejamento*)abre->apl->GetObject(__classid(VTPlanejamento));

	//verifica se existe a base de dados
	if (bd == NULL) return(NULL);
	//l� lista de Estudos da base (que s�o armazenadas em Planejamento)
	bd->Eqpto->LeListaEstudo();
	//obt�m lista de estudo
	lisESTUDO = planejamento->LisEstudo();
	//determina o Estudo
	for (int n = 0; n < lisESTUDO->Count; n++)
		{
		estudo = (VTEstudo*)lisESTUDO->Items[n];
		//verifica se � o estudo da rede original
		if (estudo->Tipo == tipo_estudo) return(estudo);
		}
	return(NULL);
	}

//---------------------------------------------------------------------------
void __fastcall TFormEstudoRnt::FormClose(TObject *Sender, TCloseAction &Action)
	{
	Action = caFree;
	}

//---------------------------------------------------------------------------
void __fastcall TFormEstudoRnt::FormShow(TObject *Sender)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)abre->apl->GetObject(__classid(VTGeral));
   VTPath  *path  = (VTPath*)abre->apl->GetObject(__classid(VTPath));

   //verifica a base atual
   if (geral->BaseDado == SNPDBTYPE_ORACLE)
      {//base Oracle: for�a tratamento de sele��o de diret�rio da base de dados
      ExibeDadosBaseSelecionada();
      }
   else
      {//verifica se h� uma base de rede pr�-definida
      if (FileExists(path->ArqRede))
         {//solicita que ousu�rio seleciona a base de uma rede
         if (! SelecionaBaseRede()) Close();
         }
      ExibeDadosBaseSelecionada();
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEstudoRnt::SelecionaBaseRede(void)
   {
   //vari�veis locais
   VTPath *path = (VTPath*)abre->apl->GetObject(__classid(VTPath));

   //verifica se o diret�rio inicial est� definido
   if (OpenDialog->InitialDir.IsEmpty())
      {//define diret�rio defualt das redes
      OpenDialog->InitialDir = path->DirRedes();
      }
   //exibe OpenDialog
   if (OpenDialog->Execute())
      {//salva path da base selecionada
      path->ArqRede = OpenDialog->FileName;
      return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEstudoRnt::ValidaBaseRede(AnsiString sdb_name)
   {
   return(true);
   }

//---------------------------------------------------------------------------
//eof


