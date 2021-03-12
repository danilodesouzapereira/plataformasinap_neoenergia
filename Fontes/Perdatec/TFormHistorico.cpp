//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TFormHistorico.h"
#include "TFormPerda.h"
#include "VTResumos.h"
#include "..\Apl\VTApl.h"
#include "..\BD\VTBD.h"
#include "..\BD\VTSNPDBConn.h"
#include "..\BD\Hist\VTBDHist.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\BD.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Radial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormHistorico::TFormHistorico(TComponent *Owner, VTApl *apl_owner, TAction *ActionOnClose, int tipo)
   : TForm(Owner)
   {
   //salva Action externa
   this->ActionOnClose = ActionOnClose;
   //cria objeto Apl próprio
   apl = NewObjApl(this, apl_owner);
   apl->Add(DLL_NewObjArvore(apl));
   apl->Add(NewObjResumos(apl));
   apl->Add(DLL_NewObjProgresso(NULL, NULL));
   //cria objetos
   lisARVORE   = new TList();
   //empresaMTBT->Resumo->TipoGrupoPerdaId = gpEMPRESA;
   //empresaAT->Resumo->TipoGrupoPerdaId   = gpEMPRESA;
   //cria FormPerda
   FormPerda  = new TFormPerda(this, apl, PanelRelatorio);
   //redefine Captions
   if (tipo == 1)
      {//perda técnica
      Caption            = "Histórico de Perda Técnica";
      FormPerda->Caption = "Perda Técnica";
      }
   else if (tipo == 2)
      {
      Caption            = "Histórico de Perda Regulatória";
      FormPerda->Caption = "Perda Técnica Regulatória";
      }
   //FormPerda->Show();
   //FormEvolucao = new TFormEvolucao(this, apl, TabSheetEvolucao);
   //inicia conexão com base de dados
   ConectaBaseHistorico();
   //inicia CBoxData
   CBoxDataInicia();
   //simula seleção de uma Data
   CBoxDataChange(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TFormHistorico::~TFormHistorico(void)
   {
   // destrói TFormPerda
   if (FormPerda) {delete FormPerda; FormPerda = NULL;}
   //destrói lista e seus obejtos
   if (lisARVORE) {LimpaTList(lisARVORE); delete lisARVORE; lisARVORE = NULL;}
   //destrói objetos
   if (apl)              {delete apl;         apl         = NULL;}
   //executa Action externa
   if (ActionOnClose) ActionOnClose->Execute();
   }

//---------------------------------------------------------------------------
void __fastcall TFormHistorico::ActionFechaExecute(TObject *Sender)
   {
   //fecha o form
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormHistorico::ActionSelBasePerdaTecnicaExecute(TObject *Sender)
   {
   //variáveis locais
   VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //configura OpenDialog com a base atual
   OpenDialog->InitialDir = ExtractFileDir(path->ArqHistorico);
   OpenDialog->FileName   = ExtractFileName(path->ArqHistorico);
   if (OpenDialog->Execute())
      {//verifica se foi selecionada uma nova base
      if (OpenDialog->FileName.CompareIC(path->ArqHistorico) != 0)
         {//atualiza Path
         path->ArqHistorico = OpenDialog->FileName;
         //reinicia conexão com base de dados de perdas
         ConectaBaseHistorico();
         //inicia CBoxData
         CBoxDataInicia();
         //simula seleção de uma Data
         CBoxDataChange(NULL);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormHistorico::CBoxDataChange(TObject *Sender)
   {
   //variáveis locais
   VTArvore *arvore_aux;
   VTArvore *arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));

   // Testes preliminares
   if (!bd)       return;
   if (!bd->Hist) return;

   //proteção
   if (CBoxData->ItemIndex < 0)
      {//reinicia e esconde FormPerda
      //FormPerda->Visible = false;
      return;
      }
   //exibe PanelPerda
   //FormPerda->Visible = true;
   //determina Arvore selecionada e copia seus dados
   arvore_aux   = (VTArvore*)(CBoxData->Items->Objects[CBoxData->ItemIndex]);
   arvore->Clear();
   arvore->Id   = arvore_aux->Id;
   arvore->Data = arvore_aux->Data;
   //lê dados de perdas técnicas
   bd->Hist->LeArvorePerdaTecnica(arvore);
	//proteção: filtra dados redundantes
	FiltraArvore(arvore);
   //exibe FormPerda
   FormPerda->Inicia(arvore->Data);
   }

//---------------------------------------------------------------------------
void __fastcall TFormHistorico::CBoxDataInicia(void)
   {
   //varia´veis locais
   VTArvore *arvore;

   // Testes preliminares
   if (! bd)       return;
   if (! bd->Hist) return;
   //reinicia CBoxData
   CBoxData->Clear();
   //obtém lista de Árvores da base dedados
   LimpaTList(lisARVORE);
   if (! bd->Hist->LeListaArvore(lisARVORE)) return;
   //inicia CBoxData
   for (int n = 0; n < lisARVORE->Count; n++)
      {
      arvore = (VTArvore*)lisARVORE->Items[n];
      //verifica o tipo da árvore
      if ((arvore->TipoArvore & arvorePERDATEC) == arvorePERDATEC)
         {//insere Arvore no ComboBox
         CBoxData->Items->AddObject(DateToStr(arvore->Data), arvore);
         }
      }
   //seleciona última Arvore
   if (CBoxData->Items->Count > 0) CBoxData->ItemIndex = CBoxData->Items->Count - 1;
   }

//---------------------------------------------------------------------------
void __fastcall TFormHistorico::ConectaBaseHistorico(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*) apl->GetObject(__classid(VTGeral));
   int     intTipoBaseDado;

   bd = (VTBD*)apl->GetObject(__classid(VTBD));
   if (!bd)
      {
      apl->Add(bd = DLL_NewObjBD(apl));
      }
   if (bd)
      {
      if (!bd->Conectado)
         {
         intTipoBaseDado = SNPDBTYPE_INDEFINIDO;
         if (geral)
           {
           intTipoBaseDado = geral->BaseDado;
           }
         bd->ConectaBaseHist(intTipoBaseDado);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormHistorico::FiltraArvore(VTArvore* arvore)
	{
	//variáveis locais
	int    level_1 = 1;
	bool   node_excluido;
	TList  *lisNODE, *lisNODE_L1;
	VTNode *nodeI, *nodeJ, *nodeL1;

	//cria lista local
	lisNODE_L1 = new TList();
	//obtém lista de Nodes de level=1 da árvore
	arvore->LisNode(lisNODE_L1, level_1);
	for (int n = 0; n < lisNODE_L1->Count; n++)
		{
		nodeL1 = (VTNode*)lisNODE_L1->Items[n];
		do{//obtém lista de Nodes filhos do Node de level=1
			lisNODE = nodeL1->LisNode();
			//elimina Nodes de lisNODE que tem código repetido
			node_excluido = false;
			for (int i = 0; (! node_excluido)&&(i < lisNODE->Count); i++)
				{
				nodeI = (VTNode*)lisNODE->Items[i];
				for (int j = i+1; (! node_excluido)&&(j < lisNODE->Count); j++)
					{
					nodeJ = (VTNode*)lisNODE->Items[j];
					if (nodeI->Codigo.AnsiCompareIC(nodeJ->Codigo) == 0)
						{//exclui Node mais antigo (com ID menor)
						if (nodeI->Id <= nodeJ->Id) arvore->RetiraNode(nodeI);
						else                        arvore->RetiraNode(nodeJ);
						//atualiza controle para sair dos loops
						node_excluido = true;
						}
					}
				}
			}while(node_excluido);
		}
	//destrói lista local
	delete lisNODE_L1;
	}

//---------------------------------------------------------------------------
void __fastcall TFormHistorico::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormHistorico::FormShow(TObject *Sender)
   {
   //exibe FormEvolucao
   //FormEvolucao->Show();
   FormPerda->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormHistorico::VerificaModulos(void)
   {
   //verifica se usuário master
   VTLicencas    *licencas      = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTLicenca     *licenca       = licencas->ExisteLicenca();

   //habilita/desabilita módulos
   ActionSelBasePerdaTecnica->Visible  = (licenca->Tipo == tipoMASTER);
   }

//---------------------------------------------------------------------------
//eof


