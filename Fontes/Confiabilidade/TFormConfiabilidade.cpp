//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAplConfiab.h"
#include "TFormConfiabilidade.h"
#include "TFormCfgGraf.h"
#include "TFormParam.h"
#include "TUtilVCL.h"
#include "VTCfgGrafConfiab.h"
#include "..\Apl\VTApl.h"
#include "..\Bloco\VTBloco.h"
#include "..\Bloco\VTBlocos.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Arranjo\VTCabo.h"
#include "..\Confiabilidade\VTConfiab.h"
#include "..\Confiabilidade\VTDadoConfiab.h"
#include "..\Confiabilidade\GeraCorte\TCorteConf.h"
#include "..\Confiabilidade\TDataRede.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Diretorio\VTPath.h"
#include "..\Diretorio\VTDir.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Editor\VTEdt.h"
#include "..\GeraPath\VTArvore.h"
#include "..\GeraPath\VTArvores.h"
#include "..\GeraPath\TEqptoPath.h"
#include "..\GeraRede\TLigaSerie.h"
#include "..\GeraRede\TEqptoLiga.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Curvas\VTCurva.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Report\VTReport.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\..\DLL_Inc\Bloco.h"
#include "..\..\DLL_Inc\Consulta.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Report.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFormConfiabilidade::TFormConfiabilidade(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
										: TFormEdt(form_owner, parent)
	{
	//salva ponteiro
	this->apl = apl_owner;

	//cria objetos no apl do pai
	VTConfiab        *confiab = NewObjConfiab(apl_owner);     //cria objeto de confiabilidade
	VTCfgGrafConfiab *cfg     = NewObjCfgGrafConfiab();     //cria objeto de visualização
   if(confiab) apl_owner->Add(confiab);
   if(cfg)     apl_owner->Add(cfg);

   //insere Form em seu Parent
	if (parent) {Parent = parent;}

   //Cria lista auxiliar
   lisEQS = new TList();
   lisTMP = new TList();

   //cria objetos de configuração
   confiab = (VTConfiab*)apl->GetObject(__classid(VTConfiab));
   dataSeco  = confiab->DadoConfiabNovo();  dataSeco->descricao  = "Período Seco";
   dataUmido = confiab->DadoConfiabNovo();  dataUmido->descricao = "Período Úmido";

   //posiciona p/ esquerda
   ActionAlignLeftExecute(NULL);
   //trata eventos de mouse
   //butEdt->Down = true;  ActionEdtExecute(NULL);

	//inicia configuração
	Inicia();
   //Retorna a visualizacao de topologia da rede
	ActionViewTOPOExecute(NULL);
	}


//---------------------------------------------------------------------------
__fastcall TFormConfiabilidade::~TFormConfiabilidade(void)
   {
   //var local
   VTGrafico        *graf;
	VTConfiab        *confiab;
	VTCfgGrafConfiab *cfg;
	if(apl) graf    = (VTGrafico*) apl->GetObject(__classid(VTGrafico));
	if(apl) confiab = (VTConfiab*) apl->GetObject(__classid(VTConfiab));
	if(apl) cfg     = (VTCfgGrafConfiab*) apl->GetObject(__classid(VTCfgGrafConfiab));

   //cancela moldura
   if(graf) graf->Moldura();
   if(graf) graf->Show(grafTOPO);

   //cancela seleção de mouse
   AtualizaIndicacaoEditorOff();       

   //destrói objetos
   if(lisEQS) {delete lisEQS; lisEQS = NULL;}
   if(lisTMP) {delete lisTMP; lisTMP = NULL;}
   if(dataUmido) {delete dataUmido; dataUmido = NULL;}
   if(dataSeco)  {delete dataSeco;  dataSeco  = NULL;}
   if(edt_busca) {delete edt_busca;  edt_busca  = NULL;}

   //remove objetos inseridos no pai
   if(confiab) {apl->Remove(confiab);  delete confiab; confiab = NULL;}
   if(cfg)     {apl->Remove(cfg);      delete cfg;     cfg = NULL;}
   }
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ActionCfgGrafExecute(TObject *Sender)
{
   //var local
   VTCfgGrafConfiab *cfg = (VTCfgGrafConfiab*)apl->GetObject(__classid(VTCfgGrafConfiab));
   VTGrafico        *graf  = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

   TFormCfgGraf *form;
   form = new TFormCfgGraf(this, cfg);
   form->ShowModal();
   if(form->ModalResult == mrOk && graf)
      {
      //atualização visualização
      //graf->Show(grafCONFIAB);
      switch(cfg->index)
         {
            case eDEC:  ActionViewDECExecute(Sender); break;
            case eFEC:  ActionViewFECExecute(Sender); break;
            case eEND:  ActionViewENDExecute(Sender); break;              
            case ePOT:  ActionViewPOTExecute(Sender); break;
            default: break;
         }
      }
   if(form) {delete form;  form = NULL;}
}
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ActionCfgSecoExecute(TObject *Sender)
	{
	//var local
   TFormParam    *formParam;
	VTConfiab     *confiab = (VTConfiab*)apl->GetObject(__classid(VTConfiab));
	//proteção
   if(! confiab) return;
   //cria form
   if ((formParam = new TFormParam(this, NULL, apl)) == NULL) return;


   //define configuração atual
	confiab->DefDadoConfiab(dataSeco);
	formParam->Atualiza(confiab->DadoConfiab());
	formParam->ShowModal();
   if(formParam->ModalResult == mrOk) dataSeco->CopiaDados(confiab->DadoConfiab());
   //libera memoria alocada
   if(formParam) {delete formParam; formParam = NULL;}
   DefineViewMenu(this->PopupMenuConfiabCfg, 0, IMAGE);
   StatusBar->Panels->Items[0]->Text = "Parâmetros selecionados: " + confiab->DadoConfiab()->descricao ;

   }
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ActionCfgUmidoExecute(TObject *Sender)
   {
   //var local
   TFormParam    *formParam;
   VTConfiab     *confiab = (VTConfiab*)apl->GetObject(__classid(VTConfiab));
   //proteção
   if(! confiab) return;
   //cria form
   if ((formParam = new TFormParam(this, NULL, apl)) == NULL) return;
   //define configuração atual
   confiab->DefDadoConfiab(dataUmido);
   formParam->Atualiza(confiab->DadoConfiab());
   formParam->ShowModal();
   if(formParam->ModalResult == mrOk) dataUmido->CopiaDados(confiab->DadoConfiab());
   //libera memoria alocada
   if(formParam) {delete formParam; formParam = NULL;}
   DefineViewMenu(this->PopupMenuConfiabCfg, 1, IMAGE);
   StatusBar->Panels->Items[0]->Text = "Parâmetros selecionados: " + confiab->DadoConfiab()->descricao ;
   }
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ActionConfiabExecute(TObject *Sender)
	{
	//executa confiabilidade
	ExecutaConfSC();

	MostraArvores();

	MostraCortes(tviewCorte);
   //apresenta os dados
   TreeViewRedesClick(Sender);
   }
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ActionEdtExecute(TObject *Sender)
   {
   try{//se necessário, cria um objeto TEdtMedidor
      if (edt_busca != NULL) delete edt_busca;
      //cria TEdtMedidor p/ controle do mouse
		edt_busca = DLL_NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
		//atualiza indicação de mouse ativo e força botão Down
      AtualizaIndicacaoEditorOn(butEdt);
      } catch (Exception &e)
         {//nada a fazer
         }
   }
//---------------------------------------------------------------------------

void __fastcall TFormConfiabilidade::ActionExecSecoExecute(TObject *Sender)
   {
   //var local
   VTConfiab     *confiab = (VTConfiab*)apl->GetObject(__classid(VTConfiab));

   //proteção
   if(! confiab) return;

   //configura p/ período seco
   confiab->DefDadoConfiab(dataSeco);

   //executa confiab
   ActionConfiabExecute(Sender);
   DefineViewMenu(this->PopupMenuConfiabExec, 0, IMAGE);

   }
//---------------------------------------------------------------------------

void __fastcall TFormConfiabilidade::ActionExecUmidoExecute(TObject *Sender)
   {
   //var local
   VTConfiab     *confiab = (VTConfiab*)apl->GetObject(__classid(VTConfiab));

   //proteção
   if(! confiab) return;

   //configura p/ período úmido
   confiab->DefDadoConfiab(dataUmido);

   //executa confiab
   ActionConfiabExecute(Sender);
   DefineViewMenu(this->PopupMenuConfiabExec, 1, IMAGE);
   }
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ActionExportaResultadosExecute(
		TObject *Sender)
	{
	VTReport *report;// = (VTReport*)apl->GetObject(__classid(VTReport));
	VTPath   *path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString arq;   //hkhk01


	saveDialog->InitialDir = path->DirExporta();
	if(saveDialog->Execute())
		{
		arq = saveDialog->FileName;//ExtractFileDir(OpenDialog->FileName);
		report = DLL_NewObjReport(apl);
		//arq = path->DirExporta() + "\\ResRiscoTecnico.txt";
		report->ArquivoTexto(arq, ListViewConfiab);
		delete report;
		}
	}
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ActionImportaParamExecute(TObject *Sender)
	{
	//variáveis locais
	VTConfiab     *confiab = (VTConfiab*)apl->GetObject(__classid(VTConfiab));
	AnsiString    arqParam, dirArq;

	//configura OpenDialog
	OpenDialog->Filter = "Arquivos TXT (*.txt)|*.txt";
	OpenDialog->FileName = "";
	//assume diretório default de importação

	OpenDialog->InitialDir = confiab->PathParam;
	//exibe OpenDialog
	if (OpenDialog->Execute())
		{//identifica caminho do arquivo
		arqParam = OpenDialog->FileName;
		dirArq   = ExtractFileDir(arqParam) + "\\";
		//executa leitura
		try{//leitura dos parâmetros
			//if(calcIndCont->LeArqParam(arqParam) == true) arqParamAtual = arqParam;
			confiab->LeArquivos(dirArq);
			if(confiab->DadoConfiab()->descricao.AnsiCompare("Período Seco") == 0)
				  dataSeco->CopiaDados(confiab->DadoConfiab());
			else
				  dataUmido->CopiaDados(confiab->DadoConfiab());
			}catch(Exception &e)
				{
				return;
				}
		}
	}
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ActionMostraArvExecute(TObject *Sender)
   {
   TTreeNode* node;
	TCorteConf* pcorte;
   VTGrafico* grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   TList *lisAux, *lisBarra;
   TMetaClass *obj_class;
   AnsiString obj_name;
   VTArvore*  arv1;
   VTLigacao    *pliga;
   VTEqpto      *eqpto;

   //verifica se há um TreeNode selecionado
   if (tviewArv->Selected == NULL) return;
   node = tviewArv->Selected;
   if(!node->Data) return;

   grafico->Moldura();
   lisAux = new TList();
   lisBarra= new TList();
	obj_class = ((TObject*)(node->Data))->ClassType();
   obj_name  = obj_class->ClassName();
   if(obj_name.AnsiCompareIC("TArvore") == 0)
     {

     arv1 = (VTArvore*)(node->Data);
     while(arv1 != NULL)
         {
			eqpto = arv1->Eqpto;
         lisAux->Add(eqpto);
			arv1 = arv1->ArvOri;//--
         }
     /*while(arv1 != NULL)
         {
         pliga = arv1->Ligacao();
         if(pliga != NULL) {lisBarra->Clear();
                            pliga->LisBarra(lisBarra);
                            lisAux->Assign(lisBarra, laOr);//lisAux->Add(eqpto);
                           }
         arv1 = arv1->ArvOri();
         }
     */
     }

   grafico->Moldura(lisAux);

   delete lisAux;
   delete lisBarra;
   }
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ActionMostraEqptosExecute(TObject *Sender)
	{
	TListItem *item;

	if(ListViewConfiab->SelCount == 0) return;
	item = ListViewConfiab->Selected;
	if(item->Data == NULL) return;

	MostraEqptos((TObject*)item->Data);

	}
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::MostraEqptos(TObject *pobj)
	{
	VTGrafico* grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRede    *rede;
	TMetaClass *obj_class;
	AnsiString obj_name;
	TList      *lisAux;

	grafico->Moldura();

	obj_class = pobj->ClassType();
	obj_name  = obj_class->ClassName();

	if(obj_name.AnsiCompareIC("TCarga") == 0)
			 {grafico->Moldura((VTEqpto*)pobj);
			  return;
			 }

	if(obj_name.AnsiCompareIC("TRede") == 0)
			 {
			  rede = (VTRede*)pobj;
			  lisAux = new TList();
			  rede->LisEqbar(lisAux, eqptoCARGA);
			  grafico->Moldura(lisAux);
			  delete lisAux;
			  return;
			 }

	}
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ActionMostraFontesExecute(TObject *Sender)
	{
	VTGrafico* grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TCorteConf* pcorte;
	TTreeNode* node;
	TMetaClass *obj_class;
	AnsiString obj_name;
	TList      *lisAux;
	VTArvores  *arvs;

	//verifica se há um TreeNode selecionado
	if (tviewCorte->Selected == NULL) return;
	node = tviewCorte->Selected;
	if(!node->Data) return;

	lisAux = new TList();
	obj_class = ((TObject*)(node->Data))->ClassType();
	obj_name  = obj_class->ClassName();
	if(obj_name.AnsiCompareIC("TCorteConf") == 0)
	  {
	  pcorte = (TCorteConf*)(node->Data);
	  grafico->Moldura();
	  for(int nc = 0; nc < pcorte->lisFontes->Count; nc++)
		  {
		  arvs = (VTArvores*)pcorte->lisFontes->Items[nc];
		  lisAux->Add(arvs->EqRaiz());
		  }
	  grafico->Moldura(lisAux);
	  }
	delete lisAux;
	}
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ActionMostraCorteExecute(TObject *Sender)
	{
	TTreeNode* node;
	TCorteConf* pcorte;
	TList *lisAux;
	VTGrafico* grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTConfiab     *confiab  = (VTConfiab*) apl->GetObject(__classid(VTConfiab));
   TList      *lisC, *lisGrupoCargas;
	TMetaClass *obj_class;
	AnsiString obj_name;
	VTEqpto    *eqpto;
	TLigaSerie *pligaS;
	VTArvore   *arv1;
	VTArvores  *arvs;

	//verifica se há um TreeNode selecionado
	if (tviewCorte->Selected == NULL) return;
	node = tviewCorte->Selected;
	if(!node->Data) return;

	grafico->Moldura();

   lisAux = new TList();
	obj_class = ((TObject*)(node->Data))->ClassType();
	obj_name  = obj_class->ClassName();
	if(obj_name.AnsiCompareIC("TCorteConf") == 0)
	  {
	  pcorte = (TCorteConf*)(node->Data);
	  //Seleciona cargas cortadas
	  for(int nc = 0; nc < pcorte->lisFontes->Count; nc++)
		  {
		  arvs = (VTArvores*)pcorte->lisFontes->Items[nc];
		  lisAux->Add(arvs->EqRaiz());
		  }
	  while(pcorte != NULL)
		  {
		  obj_name = (pcorte->eqpto)->ClassName();
		  if(obj_name.AnsiCompareIC("TLigaSerie") == 0)
			 {
			 pligaS = (TLigaSerie*)(pcorte->eqpto);
			 lisAux->Assign(pligaS->LisEqpto(), laOr);
			 }
		  else
			 {
			 lisAux->Add(pcorte->eqpto);
			 }
        pcorte = pcorte->pcorte0;
		  }
	  grafico->Moldura(lisAux);
	  delete lisAux;
	  return;
	  }

	if(obj_name.AnsiCompareIC("TArvore") == 0)
	  {
	  arv1 = (VTArvore*)(node->Data);
	  while(arv1 != NULL)
			{
			eqpto = arv1->Eqpto;
			lisAux->Add(eqpto);
			arv1 = arv1->ArvOri;//--
         }
     grafico->Moldura(lisAux);
     }

	if(obj_name.AnsiCompareIC("TLigaSerie") == 0)
	  {
	  pligaS = (TLigaSerie*)(node->Data);
	  lisAux->Assign(pligaS->LisEqpto(), laOr);
	  grafico->Moldura(lisAux);
	  }

	if((obj_name.AnsiCompareIC("TLigacao") == 0)||(obj_name.AnsiCompareIC("TTrecho") == 0)||(obj_name.AnsiCompareIC("TChave") == 0))
	  {
	  grafico->Moldura((TLigacao*)node->Data);
	  }
	//else
	//  {
   //  lisC  = (TList*)(node->Data);
   //  grafico->Moldura(lisC);
   //  }

   delete lisAux;

	}
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ActionRelatCargaBlocosExecute(
		TObject *Sender)
	{
	VTReport *report;// = (VTReport*)apl->GetObject(__classid(VTReport));
	VTPath   *path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString arq;     //hkhk01

	arq = path->DirTmp() + "\\repCargaBlocos.txt";
	report = DLL_NewObjReport(apl);

	report->ArquivoTexto(arq, sgridDadosEqpto, "", false);
	delete report;

	}
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ActionSelEqptoExecute(TObject *Sender)
   {
   //variáveis locais
   TTreeNode *node;
   TList     *lisEqpto = (TList*) Sender;
   VTEqpto   *eqpto_sel;
   //VTEqpto   *eqpto;

   //proteção
   if (lisEqpto == NULL) return;

   //loop para lista de eqptos selecionados
   for(int ne = 0; ne < lisEqpto->Count; ne++)
      {
      eqpto_sel = (VTEqpto*) lisEqpto->Items[ne];
      //determina Node do TreeView c/ o Eqpto selecionado
      for (int n = 0; n < TreeViewRedes->Items->Count; n++)
			{
         node = TreeViewRedes->Items->Item[n];
         if(! node->Data) continue;
         if (node->Data == eqpto_sel)
            {//expande TreeNode
				node->Expand(false);
            //seleciona TreeNode
            TreeViewRedes->Selected = node;
            TreeViewRedesClick(NULL);
            return;
            }
         }
      }
   }
//---------------------------------------------------------------------------

void __fastcall TFormConfiabilidade::ActionViewDECExecute(TObject *Sender)
   {
   //var local
   VTCfgGrafConfiab *cfg = (VTCfgGrafConfiab*)apl->GetObject(__classid(VTCfgGrafConfiab));
   VTGrafico        *graf  = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

   if(cfg)  cfg->index = eDEC;
   if(graf) graf->Show(grafCONFIAB);

   DefineViewMenu(PopupMenuView, eDEC, IMAGE);
   StatusBar->Panels->Items[2]->Text = "Visualização por DEC";

   }
//---------------------------------------------------------------------------

void __fastcall TFormConfiabilidade::ActionViewENDExecute(TObject *Sender)
   {
   //var local
   VTCfgGrafConfiab *cfg = (VTCfgGrafConfiab*)apl->GetObject(__classid(VTCfgGrafConfiab));
   VTGrafico        *graf  = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

   if(cfg)  cfg->index = eEND;
   if(graf) graf->Show(grafCONFIAB);
   DefineViewMenu(PopupMenuView, eEND, IMAGE);
   StatusBar->Panels->Items[2]->Text = "Visualização por END";
   }
//---------------------------------------------------------------------------

void __fastcall TFormConfiabilidade::ActionViewFECExecute(TObject *Sender)
   {
   //var local
   VTCfgGrafConfiab *cfg = (VTCfgGrafConfiab*)apl->GetObject(__classid(VTCfgGrafConfiab));
   VTGrafico        *graf  = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

   if(cfg)  cfg->index = eFEC;
   if(graf) graf->Show(grafCONFIAB);
   DefineViewMenu(PopupMenuView, eFEC, IMAGE);
   StatusBar->Panels->Items[2]->Text = "Visualização por FEC";
	}
//---------------------------------------------------------------------------

void __fastcall TFormConfiabilidade::ActionViewPOTExecute(TObject *Sender)
   {
   //var local
   VTCfgGrafConfiab *cfg = (VTCfgGrafConfiab*)apl->GetObject(__classid(VTCfgGrafConfiab));
   VTGrafico        *graf  = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

	if(cfg)  cfg->index = ePOT;
	if(graf) graf->Show(grafCONFIAB);
   DefineViewMenu(PopupMenuView, ePOT, IMAGE);
   StatusBar->Panels->Items[2]->Text = "Visualização por Potência";
   }

//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ActionViewTOPOExecute(TObject *Sender)
   {
   //var local
   VTCfgGrafConfiab *cfg = (VTCfgGrafConfiab*)apl->GetObject(__classid(VTCfgGrafConfiab));
   VTGrafico        *graf  = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

   if(cfg)  cfg->index = eTOPO;
   if(graf) graf->Show(grafTOPO);
   DefineViewMenu(PopupMenuView, eTOPO, IMAGE);
	StatusBar->Panels->Items[2]->Text = "Topologia";
	graf->Redraw();
   }
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ActionMostraLigaBlocosExecute(
		TObject *Sender)
	{
	VTGrafico  *grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TTreeNode  *node;
	TTreeView  *tview = tviewBlocos;
	TMetaClass *obj_class;
	AnsiString obj_name;
	TList      *lisLiga;

	//verifica se há um TreeNode selecionado
	if (tview->Selected == NULL) return;
	node = tview->Selected;
	if(!node->Data) return;


	grafico->Moldura();
	obj_class = ((TObject*)(node->Data))->ClassType();
	obj_name  = obj_class->ClassName();
	if(obj_name.AnsiCompareIC("TList") == 0)
	  {
	  lisLiga = (TList*)node->Data;
	  grafico->Moldura(lisLiga);
	  }

	if(obj_name.AnsiCompareIC("TCarga") == 0)
	  {
	  grafico->Moldura((VTEqpto*)node->Data);
     }

	}
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ActionMostraBlocosExecute(TObject *Sender)
	{
	VTRedes   *redes    = (VTRedes*)   apl->GetObject(__classid(VTRedes));
	VTRede    *rede;
	VTConfiab *confiab = (VTConfiab*)apl->GetObject(__classid(VTConfiab));
	TTreeNode *node0, *node1;
	TTreeView *tview = tviewBlocos;
	VTBlocos  *blocos = confiab->Blocos;
	VTBloco   *bloco;
	TList     *lisBlocos, *lisRede, *lisCargas;
	VTCarga   *pcarga;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTDemanda* demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	VTPatamar   *patamar;
	int   max_pat = patamares->NumPatamar();
	double duracao, valor, valorAux;
	AnsiString aux;

	tview->Items->Clear();
	if(blocos == NULL) return;
	if(max_pat == 0) return;

	lisRede   = redes->LisRede();
	lisBlocos = new TList();
	lisCargas = new TList();

	sgridDadosEqpto->RowCount = 2;
	sgridDadosEqpto->ColCount = 2;
	sgridDadosEqpto->ColWidths[0] = 100;
	sgridDadosEqpto->ColWidths[1] = 100;
	sgridDadosEqpto->Cells[0][0] = "Bloco";
	sgridDadosEqpto->Cells[1][0] = "PotMedia(kW)";


	for(int nr = 0; nr < lisRede->Count; nr++)
		{
		rede = (VTRede*)lisRede->Items[nr];
		node0 = tview->Items->AddChildObject(NULL, rede->Codigo, rede->LisLigacao());
		lisBlocos->Clear();
		blocos->LisBlocoRede(rede, lisBlocos);
		for(int nb = 0; nb < lisBlocos->Count; nb++)
			{
			bloco = (VTBloco*)lisBlocos->Items[nb];
			lisCargas->Clear();
			bloco->LisEqbar(lisCargas, eqptoCARGA);
			valor = 0.0;
			node1 = tview->Items->AddChildObject(node0, "", bloco->LisLigacao());
			for(int nc = 0; nc < lisCargas->Count; nc++)
				{
				pcarga = (VTCarga*)lisCargas->Items[nc];
				duracao = 0.;
				valorAux   = 0.;
				for(int np = 0; np < max_pat; np++)
					{
					patamar = patamares->Patamar[np];
					duracao += patamar->Duracao;
					//iconta = 0;
					valorAux += demanda->P_mw[pcarga][np] * patamar->Duracao;
					}
				valorAux /= duracao;
				valor += (valorAux * 1000.);
				aux = IntToStr(nc) + ":" + DoubleToStr("%8.3f", valorAux * 1000);
				tview->Items->AddChildObject(node1, aux, pcarga);
				}
			aux = rede->Codigo + ":" + IntToStr(nb);
			sgridDadosEqpto->Cells[0][sgridDadosEqpto->RowCount - 1] = aux;
			sgridDadosEqpto->Cells[1][sgridDadosEqpto->RowCount - 1] = DoubleToStr("%8.3f", valor);
			aux += ":PMed = " + DoubleToStr("%8.3f", valor);
			sgridDadosEqpto->RowCount ++;
			node1->Text = aux;
			}
		}
		sgridDadosEqpto->RowCount --;

	delete lisCargas;
	delete lisBlocos;
	}
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ApresentaCargas(VTRede *rede)
	{
	//var locais
	VTConsulta   *consulta = (VTConsulta*) DLL_NewObjConsulta(apl);
	VTConfiab     *confiab  = (VTConfiab*) apl->GetObject(__classid(VTConfiab));
	RES_CONF      res_conf;
	VTEqpto       *eqpto;
	TList         *lisCargas;

	//obtem listas agrupadas
	//obtem lista de cargas da rede
	lisCargas = consulta->LisEqpto(rede, eqptoCARGA);
	for(int neq = 0; neq < lisCargas->Count; neq++)
			{
			eqpto = (VTEqpto*)lisCargas->Items[neq];
			if(!confiab->AgregaRes(res_conf, eqpto)) continue;
			ListViewInsereConf(ListViewConfiab, res_conf, eqpto, eqpto->Codigo);
			}
	if(consulta) delete consulta;
	}
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ApresentaEqpto(VTEqpto *eqpto)
   {
	//var locais
   VTConfiab     *confiab  = (VTConfiab*) apl->GetObject(__classid(VTConfiab));
   TList         *lisAux;
   RES_CONF      res_conf;

   //Resultados por carga
   lisAux  = new TList();
   lisAux->Add(eqpto);
   Moldura(eqpto);
   if(confiab->AgregaRes(res_conf, lisAux)) {
         ListViewInsereConf(ListViewConfiab, res_conf, eqpto, eqpto->Codigo);
   }

   //destroi lista
   if(lisAux) delete lisAux;

   }
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ApresentaGeral(void)
   {
   //var locais
	VTConfiab     *confiab  = (VTConfiab*) apl->GetObject(__classid(VTConfiab));
	VTRedes       *redes    = (VTRedes*)   apl->GetObject(__classid(VTRedes));
	RES_CONF      res_conf;
   TList           *lisAux;
   
	//obtem lista de cargas de todas redes
   lisAux  = new TList();
   redes->LisEqpto(lisAux, eqptoCARGA);

   //Resultados gerais
   if(confiab->AgregaRes(res_conf, lisAux)) {
      if(lisAux->Count > 0) ListViewInsereConf(ListViewConfiab, res_conf, NULL, "Índice geral");
   }
   //destroi lista
   if(lisAux) delete lisAux;
   }
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ApresentaIndices(TTreeNode *node)
   {
   //limpa list view
   ListViewConfiab->Items->Clear();

   //proteção:
   if(! node)  return;

   switch(node->Level)
      {
         case 0: //redes em estudo
            ApresentaGeral();
            break;

			case 1: //tipo de rede
				ApresentaTipoRede((VTTipoRede*) node->Data);
				break;

			case 2: //rede
				ApresentaRede((VTRede*) node->Data);
				MostraEqptos((VTRede*) node->Data);
				break;

			case 3: //lista de eqptos
				if(node->Parent->Data) ApresentaCargas((VTRede*) node->Parent->Data);
				MostraEqptos((VTRede*) node->Parent->Data);
				break;

			case 4: //eqpto
				ApresentaEqpto((VTEqpto*) node->Data);
				MostraEqptos((VTEqpto*) node->Data);
				break;

			default:
				break;
		}
	}
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ApresentaRede(VTRede *rede)
	{
	//var locais
	VTConsulta   *consulta = (VTConsulta*) DLL_NewObjConsulta(apl);
   VTConfiab     *confiab  = (VTConfiab*) apl->GetObject(__classid(VTConfiab));
   //VTConsulta    *consulta = (VTConsulta*)apl->GetObject(__classid(VTConsulta));
	TList         *lisCargas;
	RES_CONF      res_conf;

	//obtem lista de cargas da rede
	lisCargas = consulta->LisEqpto(rede, eqptoCARGA);

	//call indices
	if(confiab->AgregaRes(res_conf, lisCargas))
		{
		if(lisCargas->Count > 0) ListViewInsereConf(ListViewConfiab, res_conf, rede, rede->Codigo);//lisCargas, rede->Codigo);
		}
	if(consulta) delete consulta;

	}
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ApresentaTipoRede(VTTipoRede *tipoRede)
	{
	//var locais
	VTConsulta   *consulta = (VTConsulta*) DLL_NewObjConsulta(apl);
	VTConfiab     *confiab  = (VTConfiab*) apl->GetObject(__classid(VTConfiab));
	//VTConsulta    *consulta = (VTConsulta*)apl->GetObject(__classid(VTConsulta));
	VTRedes       *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede        *rede;
	RES_CONF      res_conf;
	TList           *lisRede;
	TList           *lisAux;

	//Resultados por tipo de rede
	lisAux  = new TList();
	lisRede = new TList();
	lisAux->Clear();
	lisRede->Clear();

	redes->LisRede(lisRede, tipoRede->Id);
	for(int nrr = 0; nrr < lisRede->Count; nrr++)
		{
		rede = (VTRede*)lisRede->Items[nrr];
		lisAux->Assign(consulta->LisEqpto(rede, eqptoCARGA), laOr);
		}
	if(lisAux->Count)
		if(confiab->AgregaRes(res_conf, lisAux))
			ListViewInsereConf(ListViewConfiab, res_conf, NULL, tipoRede->Descricao);
	//destroi listas criadas
	if(lisAux)  delete lisAux;
	if(lisRede)  delete lisRede;
	if(consulta) delete consulta;

	}
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::DefineCargas(TList* lisGrupoCargas)
	{
	VTConfiab     *confiab  = (VTConfiab*) apl->GetObject(__classid(VTConfiab));
	VTRedes*      redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTCarga*      carga;
	TList        *lisAux, *lisAux2, *lisBloco;
	VTBlocos     *blocos;
	VTBloco      *bloco;
	VTDadoConfiab *dataAtual;

	dataAtual = confiab->DadoConfiab();

	if(dataAtual->flag_BLOCO)
	  {
	  //Cria objeto blocos
	  blocos = (VTBlocos*)DLL_NewObjBlocos();
	  blocos->Executa(redes);
	  LimpaTList(lisGrupoCargas);
	  lisBloco = blocos->LisBloco();
	  for(int nb = 0; nb < lisBloco->Count; nb++)
		  {
		  bloco = (VTBloco*)lisBloco->Items[nb];
		  lisGrupoCargas->Add(lisAux2 = new TList());
		  bloco->LisEqbar(lisAux2, eqptoCARGA);
		  if(lisAux2->Count > 0) continue;
		  lisGrupoCargas->Remove(lisAux2);
		  delete lisAux2;
		  }
	  delete blocos;
	  }
	else
	  {
	  LimpaTList(lisGrupoCargas);
	  lisAux  = new TList();
	  redes->LisEqpto(lisAux, eqptoCARGA);
	  for(int nc = 0; nc < lisAux->Count; nc++)
			{
			carga = (VTCarga*)lisAux->Items[nc];
			lisGrupoCargas->Add(lisAux2 = new TList());
			lisAux2->Add(carga);
			}
	  delete lisAux;
	  }
	/*
	VTRedes*      redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTCarga*      carga;
	TList        *lisAux, *lisAux2;

   LimpaTList(lisGrupoCargas);
   lisAux  = new TList();
   redes->LisEqpto(lisAux, eqptoCARGA);
	for(int nc = 0; nc < lisAux->Count; nc++)
      {
      carga = (VTCarga*)lisAux->Items[nc];
      lisGrupoCargas->Add(lisAux2 = new TList());
      lisAux2->Add(carga);
      }
   delete lisAux;
   */
   }

//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::DefLigaExclud(TList* lisExcl)
   {
   //variáveis locais
   VTConfiab    *confiab = (VTConfiab*)apl->GetObject(__classid(VTConfiab));
   VTRedes*     redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTRede       *rede;
   VTTipoRede   *tiporede;
   TList        *lisAux, *lisLiga, *lisTrafo;
   VTLigacao    *pliga, *pliga1;
   VTBarra      *pbarra0, *pbarra1;
   bool         flag_12;

	//lisExcl - lista de ligações excluidas da agregacao de rede
   lisExcl->Clear();
	//Inicia lista com ligacoes a serem desconsideradas nos caminhos
	confiab->AddLigaExc(NULL);
   //Define lista de equipamentos para não participarem de
   //geração de lista agregada
   redes->LisEqpto(lisExcl, eqptoCHAVE);
   }
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::DefineSuprimentos(TList* lisSUP)
   {
   //var locais
   VTConsulta   *consulta = (VTConsulta*) DLL_NewObjConsulta(apl);

   VTRedes       *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   //VTConsulta    *consulta = (VTConsulta*)apl->GetObject(__classid(VTConsulta));
   VTReducao     *redu;
   TList         *lisFontes, *lisBarra, *lisRedu;


   lisSUP->Clear();

   lisFontes = consulta->LisEqptoRede(redes, eqptoGERADOR);
   lisSUP->Assign(lisFontes, laOr);
   lisFontes = consulta->LisEqptoRede(redes, eqptoSUPRIMENTO);
	lisSUP->Assign(lisFontes, laOr);
   //REDUÇÕES COM SUPRIMENTO
   lisRedu = new TList();
   redes->LisReducao(lisRedu);
   for(int nr = 0; nr < lisRedu->Count; nr++)
      {
      redu = (VTReducao*)lisRedu->Items[nr];
      if(!redu->ExisteGeracao()) continue;
      lisBarra = redu->LisBarra();
      for(int nbar = 0; nbar < lisBarra->Count; nbar++)
         lisSUP->Add(lisBarra->Items[nbar]);
      }
   if(lisRedu) delete lisRedu;
   if(consulta) delete consulta;
	}
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::DefineViewMenu(TPopupMenu *menu, int index, int imageIndex)
   {
   //var local
   TMenuItem *item;

   for(int n = 0; n < menu->Items->Count; n++)
      {
      item = menu->Items->Items[n];
      item->ImageIndex = (index == n) ? imageIndex :  -1;
      }
	}
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ExecutaConfSC(void)
   {
   //var local
	VTProgresso  *prog = (VTProgresso*) DLL_NewObjProgresso(this, this);
	//Executa confiabilidade com geracao de caminhos de cada suprimento para todas
	VTGrafico    *graf  = (VTGrafico*) apl->GetObject(__classid(VTGrafico));
	VTConfiab    *confiab = (VTConfiab*)   apl->GetObject(__classid(VTConfiab));
	//VTProgresso  *prog    = (VTProgresso*)   apl->GetObject(__classid(VTProgresso));
	VTEqpto*      eqpto;
	AnsiString    aux;
   TList         *lisFontes, *lisBarra, *lisAux, *lisRedu;
   TList         *lisGrupoCargas, *lisSUP;

   //Popula lisGrupoCargas
   lisGrupoCargas = confiab->ListaGrupoCargas();
   DefineCargas(lisGrupoCargas);
   if(lisGrupoCargas->Count == 0) return;

   //Popula lisSUP
   lisSUP = confiab->ListaSup();
   DefineSuprimentos(lisSUP);
   if(lisSUP->Count == 0) {Aviso("Inserir suprimento na rede"); return;}

	//prog->Show("Gerando índices", 1, lisSUP->Count);
   if(prog) confiab->DefProgresso(prog);

   //Executa caminhos->cortes->indices
	confiab->ExecutaSC();
   gboxGrid->Caption = "Indicadores: " + confiab->DadoConfiab()->descricao ;
   StatusBar->Panels->Items[1]->Text = "Indicadores gerados: " + confiab->DadoConfiab()->descricao ;
	ActionViewTOPOExecute(NULL);//if(graf) graf->Show(grafCONFIAB);
	if(prog) {delete prog; prog = NULL;}
	}

//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::FormClose(TObject *Sender, TCloseAction &Action)
	{
	//variáveis locais
	VTSinapChild *sinap_child;

	if(apl)
      {
      sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
	   //força botão Topologia (que já manda destruir este form) //DVK 2015.10.02
	   if (sinap_child) sinap_child->ActionTopo->Execute();
      }
   else
      {//destrói o Form
      Action = caFree;
      }
	}

//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::FormShow(TObject *Sender)
   {
   //reposiciona o Form
   Top  = 0;
   Left = 0;
	}
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ImprimeDataConfiab(VTDadoConfiab *data)
   {
   VTRedes   *redes   = (VTRedes*) apl->GetObject(__classid(VTRedes));
   FILE *fout;
   VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString arq;
   arq = path->DirTmp() + "\\" + "Rededata.tmp";
   fout = fopen(arq.c_str(), "wt");
   TList* lisAux = new TList();
   PAR_CONF pconf;
   VTLigacao* pliga;
   redes->LisLigacao(lisAux);

   fprintf(fout, "Descricao:\t%s\n", dataSeco->descricao.c_str());
   for(int n = 0; n < lisAux->Count; n++)
      {
      pliga = (VTLigacao*)lisAux->Items[n];
      dataSeco->ParamEqpto(pconf, pliga);
		fprintf(fout, "%s\t", pliga->Codigo.c_str());
      fprintf(fout, "txFalha:\t%6.3f\t", pconf.tx_falha);
		fprintf(fout, "txInter:\t%6.3f\t", pconf.tx_interr);
      fprintf(fout, "treparo:\t%6.3f\t", pconf.treparo);
		fprintf(fout, "tinterr:\t%6.3f\t", pconf.tman);
      fprintf(fout, "fat_isola:\t%6.3f\n", pconf.fat_isola);
		}
   delete lisAux;

   //dataSeco->ImprimeDado(fout);
   fclose(fout);
   }
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::Inicia(void)
   {
	//var local
	VTConfiab *confiab = (VTConfiab*)apl->GetObject(__classid(VTConfiab));
	VTRedes   *redes   = (VTRedes*) apl->GetObject(__classid(VTRedes));


	//parametros
	//IniciaParamConf(dataSeco);
	//IniciaParamConf(dataUmido);

	//ImprimeDataConfiab(dataSeco);



	//inicia com período seco
	confiab->DefDadoConfiab(dataSeco);

	//inicia Rede e combo com seleção de minis
	if(confiab->Blocos) confiab->Blocos->Executa(redes);
   IniciaRede();

   StatusBar->Panels->Items[0]->Text = "Parâmetros selecionados: " + confiab->DadoConfiab()->descricao ;
   StatusBar->Panels->Items[1]->Text = "";
   StatusBar->Panels->Items[2]->Text = "Visualização por DEC";

   //executa confiabilidade
   //ExecutaConfSC();

   //inicia tela
   IniciaTreeViewRede();
   //popup´s
   PopupMenuConfiabCfg->Items->Items[0]->ImageIndex = IMAGE;
   PopupMenuConfiabExec->Items->Items[0]->ImageIndex = IMAGE;
   PopupMenuView->Items->Items[0]->ImageIndex = IMAGE;

   }
//---------------------------------------------------------------------------
bool __fastcall TFormConfiabilidade::IniciaParamConf(VTDadoConfiab* dataConfiab)
   {
   VTConfiab *confiab = (VTConfiab*)apl->GetObject(__classid(VTConfiab));

   return(confiab->IniciaParamConf(dataConfiab));

   /*PAR_CONF    pconf;
   VTRedes     *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList       *lisTIPO = redes->LisTipoChave();
	TList *lisCabos;
   TList *lisTipoChaves;
   VTTipoChave* ptipochave;
   //TList *lisLig;
   VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   VTArranjo  *arranjo;
   VTTrecho   *ptrecho;
   VTCabo     *cabo;


   pconf.tx_falha = 0.10;
   pconf.tx_interr= 0.05;
   pconf.treparo  = 1.50;
   pconf.tman     = 0.66;
   pconf.fat_isola= 1.00;
   pconf.comp     = 1.00;

   dataConfiab->DefParamGeralTrecho(pconf);
   dataConfiab->DefParamGeralTipo(pconf, eqptoTRECHO);

   pconf.tx_falha = 0.10;
   pconf.tx_interr= 0.05;
   pconf.treparo  = 1.50;
   pconf.tman     = 0.66;
   pconf.fat_isola= 1.00;
   pconf.comp     = 1.00;

   dataConfiab->DefParamGeralEqpto(pconf);
   dataConfiab->DefParamGeralTipo(pconf, eqptoGERADOR);
   dataConfiab->DefParamGeralTipo(pconf, eqptoTRAFO);
   dataConfiab->DefParamGeralTipo(pconf, eqptoCHAVE);


   //Ordem das contingencias
   dataConfiab->icont = 0;

   //Define se calculo será realizado por bloco ou por carga
   dataConfiab->flag_BLOCO = true;//false;

   //Inicia lista com tipos de chaves
   dataConfiab->lisTipoChv->Clear();
   dataConfiab->lisTipoChv->Assign(lisTIPO, laOr);
   dataConfiab->lisTipoChv->Remove(dataConfiab->lisTipoChv->Last());
   dataConfiab->lisTipoChv->Remove(dataConfiab->lisTipoChv->Last());
   dataConfiab->lisTipoChv->Remove(dataConfiab->lisTipoChv->Last());
   dataConfiab->lisTipoChv->Remove(dataConfiab->lisTipoChv->Last());


   lisCabos = arranjos->LisCabo();

   dataConfiab->ParamGeralTipo(pconf, eqptoTRECHO);
   for(int nl = 0; nl < lisCabos->Count; nl++)
		{
      cabo = (VTCabo*)lisCabos->Items[nl];
      dataConfiab->DefParamGeralTipoTrecho(pconf, cabo);
      }


   lisTipoChaves = redes->LisTipoChave();
   dataConfiab->ParamGeralTipo(pconf, eqptoCHAVE);
   for(int nl = 0; nl < lisTipoChaves->Count; nl++)
      {
      cabo = (VTCabo*)lisTipoChaves->Items[nl];
      dataConfiab->DefParamGeralTipoChave(pconf, cabo);
      }

   //Insere dados de trafos
   dataConfiab->ParamGeralTipo(pconf, eqptoTRAFO);
   dataConfiab->DefParamGeralTipoTrafo(pconf, 0., 1.);
   dataConfiab->DefParamGeralTipoTrafo(pconf, 1., 10.);
   dataConfiab->DefParamGeralTipoTrafo(pconf, 10., 999.);

   //Insere dados de geradores
   dataConfiab->ParamGeralTipo(pconf, eqptoGERADOR);
   dataConfiab->DefParamGeralTipoGerador(pconf, 0., 1.);
   dataConfiab->DefParamGeralTipoGerador(pconf, 1., 10.);
   dataConfiab->DefParamGeralTipoGerador(pconf, 10., 999.);


   return(true);
   */
   }

//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::IniciaRede(void)
   {
   //var local
   VTConfiab   *confiab = (VTConfiab*)apl->GetObject(__classid(VTConfiab));
   TList       *lisExcl;

   confiab->Inicia();
   //Define Ligacoes a serem excluídas
   lisExcl = new TList();
   //Define ligações a serem excluidas da agregacao de rede
   DefLigaExclud(lisExcl);
   //Gera rede agregada
   //201711mutua: sem agregacao de ligacoes (antes com)
   //confiab->GeraRedeAgregada(lisExcl, true);
   confiab->GeraRedeAgregada(NULL, false);
   delete lisExcl;
   }
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::IniciaTreeViewRede(void)
   {
   //var local
   VTRedes   *redes  = (VTRedes*) apl->GetObject(__classid(VTRedes));
	TTreeNode *node;
   TUtilVCL utilVCL(this->apl);
   int eqptos[] = { eqptoCARGA };
                  //eqptoBARRA,
                  //eqbar
                  //eqptoCAPACITOR,
                  //eqptoCARGA,
                  //eqptoGERADOR,
                  //eqptoSUPRIMENTO,
                  //ligação
                  //eqptoFILTRO,
                  //eqptoREATOR,
                  //eqptoCAPSERIE,
                  //eqptoCHAVE,
                  //eqptoREGULADOR,
                  //eqptoTRAFO,
                  //eqptoTRAFO3E,
                  //eqptoMUTUA,
                  //eqptoTRECHO
   //proteção
   if(! redes) return;
   //preenche treeview
   utilVCL.TreeViewInsereRedes(TreeViewRedes, redes, eqptos, sizeof(eqptos)/sizeof(eqptos[0]));
   if(TreeViewRedes->Items->Count)
      {
      //seleciona  primeiro
		node = TreeViewRedes->Items->Item[0];
      TreeViewRedes->Select(node);
      //exibe os cosnumidores
      node->Expand(false);
      ApresentaIndices(node);
      }
   }
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::ListViewInsereConf(TListView *LView, RES_CONF rconf, TObject* pobj, AnsiString txt_col1)
   {
   //variáveis locais
   TListItem *item;
   //insere um novo ListItem em ListView
   item          = LView->Items->Add();
   item->Data    = pobj;
   //caption
   item->Caption = txt_col1;
   //coluna 1
   item->SubItems->Add(IntToStr(rconf.ncons));
   item->SubItems->Add(DoubleToStr("%.3f", rconf.pmed_mw));
   item->SubItems->Add(DoubleToStr("%.3f", rconf.dec));
   item->SubItems->Add(DoubleToStr("%.3f", rconf.fec));
   item->SubItems->Add(DoubleToStr("%.3f", rconf.end));
   }

//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::Moldura(VTEqpto *eqpto)
   {
   //var local
   VTGrafico  *graf  = (VTGrafico*) apl->GetObject(__classid(VTGrafico));
   if(! graf) return;
   //desenha moldura
   if(eqpto)   graf->Moldura(eqpto);
   //cancela moldura
   else        graf->Moldura();

   }
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::MostraArvores(void)
   {
   VTConfiab    *confiab = (VTConfiab*)   apl->GetObject(__classid(VTConfiab));

   MostraArvores(tviewArv, confiab->ListaArvores());

   }
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::MostraCortes(TTreeView* tview)
	{
	VTConfiab   *confiab = (VTConfiab*)apl->GetObject(__classid(VTConfiab));
	VTDadoConfiab *dadoConfiab = confiab->DadoConfiab();
	TList      *lisC;
	TTreeNode  *node0, *node1, *node2, *node3;
	TCorteConf *pcorte1, *pcorte2, *pcorte3;
	VTCarga    *carga;
	VTEqpto    *eqpto;
	TList      *lisAux;
	PAR_CONF   pconf;
	bool       flag_eq;
	AnsiString aux;
	VTArvores  *arvs;
	VTArvore   *arv;
	TList      *lisPathP, *lisPathD;

	TList* lisCortes      = confiab->ListaCortes();
	TList *lisGrupoCargas = confiab->ListaGrupoCargas();

	tview->Items->Clear();
	for(int n1 = 0; n1 < lisCortes->Count; n1++)
		{
		pcorte1 = (TCorteConf*)lisCortes->Items[n1];
		node0   = tview->Items->AddChildObject(NULL, pcorte1->eqpto->Codigo, pcorte1);
		for(int n2 = 0; n2 < pcorte1->lisCortes->Count; n2++)
			{
			pcorte2 = (TCorteConf*)pcorte1->lisCortes->Items[n2];
			node1   = tview->Items->AddChildObject(node0, pcorte2->eqpto->Codigo, pcorte2);
			for(int n3 = 0; n3 < pcorte2->lisCortes->Count; n3++)
				{
				pcorte3 = (TCorteConf*)pcorte2->lisCortes->Items[n3];
				tview->Items->AddChildObject(node1, pcorte3->eqpto->Codigo, pcorte3);
				}
			}
		}

	TList *lisArvs = confiab->ListaArvores();
	TList *lisEqptoPath, *lisArv;
	TEqptoPath *eqPath;

	for(int na = 0; na < lisArvs->Count; na++)
		{
		arvs = (VTArvores*)lisArvs->Items[na];
		node0 = tview->Items->AddChildObject(NULL, "EQPATH_" + arvs->EqRaiz()->Codigo, arvs);
		lisEqptoPath = arvs->LisEqptoPath();
		for(int neq = 0; neq < lisEqptoPath->Count; neq++)
			{
			eqPath = (TEqptoPath*)lisEqptoPath->Items[neq];
			node1   = tview->Items->AddChildObject(node0, eqPath->Eqpto()->Codigo, eqPath->Eqpto());
			lisArv  = eqPath->LisPath(false);
			for(int narv = 0; narv < lisArv->Count; narv++)
				{
				arv = (VTArvore*)lisArv->Items[narv];
				node2   = tview->Items->AddChildObject(node1, arv->Eqpto->Codigo, arv);
				}
			}
		}

	VTRedes*      redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList         *lisCargas, *lisC1, *lisC2, *lisC3;
	lisCargas = new TList();
	lisC1 = new TList();
	lisC2 = new TList();
	lisC3 = new TList();

	redes->LisEqbar(lisCargas, eqptoCARGA);
	for(int nc  = 0; nc < lisCargas->Count; nc++)
		{
		carga = (VTCarga*)lisCargas->Items[nc];
		lisC1->Clear(); lisC2->Clear(); lisC3->Clear();
		confiab->ListaCortes(carga, lisC1, lisC2, lisC3);
		node0 = tview->Items->AddChildObject(NULL, "CORTES_" + carga->Codigo, carga);
		node1 = tview->Items->AddChildObject(node0, "1aOrdem: " + IntToStr(lisC1->Count), NULL);
		for(int n1 = 0; n1 < lisC1->Count; n1++)
			{
			tview->Items->AddChildObject(node1, IntToStr(n1), lisC1->Items[n1]);
			}
		node1 = tview->Items->AddChildObject(node0, "2aOrdem: " + IntToStr(lisC2->Count), NULL);
		for(int n1 = 0; n1 < lisC2->Count; n1++)
			{
			tview->Items->AddChildObject(node1, IntToStr(n1), lisC2->Items[n1]);
			}
		node1 = tview->Items->AddChildObject(node0, "3aOrdem: " + IntToStr(lisC3->Count), NULL);
		for(int n1 = 0; n1 < lisC3->Count; n1++)
			{
			tview->Items->AddChildObject(node1, IntToStr(n1), lisC3->Items[n1]);
			}
		}

	delete lisCargas;
	delete lisC1;
	delete lisC2;
	delete lisC3;

	return;
	TList *lisCargasAux   = new TList();
	LimpaTList(lisCargasAux);
	//rchParCorte->Lines->Clear();
	for(int n1 = 0; n1 < lisCortes->Count; n1++)
		{
		pcorte1 = (TCorteConf*)lisCortes->Items[n1];
		node0   = tview->Items->AddChildObject(NULL, pcorte1->eqpto->Codigo, pcorte1);
		for(int nc = 0; nc < pcorte1->lisFontes->Count; nc++)
			{
			arvs = (VTArvores*)pcorte1->lisFontes->Items[nc];
			lisPathD = arvs->LisPath(false, pcorte1->eqpto);
			lisPathP = arvs->LisPathProt(pcorte1->eqpto);
			node1 = tview->Items->AddChildObject(node0, "lisPathP", arvs);
			for(int np = 0; np < lisPathP->Count; np++)
				{
				arv = (VTArvore*)lisPathP->Items[np];
				tview->Items->AddChildObject(node1, arv->Eqpto->Codigo, arv);
				}
			node1 = tview->Items->AddChildObject(node0, "lisPathD", arvs);
			for(int np = 0; np < lisPathD->Count; np++)
				{
				arv = (VTArvore*)lisPathD->Items[np];
				tview->Items->AddChildObject(node1, arv->Eqpto->Codigo, arv);
				}
			}

		for(int nc = 0; nc < pcorte1->lisFontesP->Count; nc++)
			{
         arvs = (VTArvores*)pcorte1->lisFontesP->Items[nc];
         lisPathD = arvs->LisPath(false, pcorte1->eqpto);
         lisPathP = arvs->LisPathProt(pcorte1->eqpto);
         node1 = tview->Items->AddChildObject(node0, "lisPathP", arvs);
         for(int np = 0; np < lisPathP->Count; np++)
            {
            arv = (VTArvore*)lisPathP->Items[np];
				tview->Items->AddChildObject(node1, arv->Eqpto->Codigo, arv);
				}
         node1 = tview->Items->AddChildObject(node0, "lisPathD", arvs);
         for(int np = 0; np < lisPathD->Count; np++)
				{
				arv = (VTArvore*)lisPathD->Items[np];
				tview->Items->AddChildObject(node1, arv->Eqpto->Codigo, arv);
            }
         }
      //continue;

		lisCargasAux->Add(lisAux = new TList());
      node1   = tview->Items->AddChildObject(node0, "lisFontesD", lisAux);
      for(int nc = 0; nc < pcorte1->lisFontes->Count; nc++)
         {
         eqpto = ((VTArvores*)pcorte1->lisFontes->Items[nc])->EqRaiz();
         //define o grupo de cargas
         for(int ng = 0; ng < lisGrupoCargas->Count; ng++)
            {
            lisC = (TList*)lisGrupoCargas->Items[ng];
            if(lisC->IndexOf(eqpto) != -1) break;
            }
         tview->Items->AddChildObject(node1, "C-"+eqpto->Codigo, lisC);
         lisAux->Assign(lisC, laOr);
         }
      pconf.tx_falha = pconf.treparo = -1;
//      if(pcorte1->eqpto->Tipo() == eqptoCHAVE)
//        {
//        int jjjj = 0;
//        }
		dadoConfiab->ParamEqpto(pconf, pcorte1->eqpto);
		for(int ncc = 0; ncc < lisAux->Count; ncc++)
         {
         //Imprime eqpto, carga[ncc], tx_falha[eqpto], trep[eqpto], "DIR"
         carga = (VTCarga*)lisAux->Items[ncc];
		 aux.sprintf("%s\t%s\t", pcorte1->eqpto->Codigo.c_str(), carga->Codigo.c_str());
         aux.cat_sprintf("%6.2f\t%6.2f\t%6.2f\t", pconf.tx_falha, pconf.treparo, pconf.comp);
         aux.cat_sprintf("DIR");
         //rchParCorte->Lines->Add(aux);
         }
      lisCargasAux->Add(lisAux = new TList());
      node1   = tview->Items->AddChildObject(node0, "lisFontesP", lisAux);
      for(int nc = 0; nc < pcorte1->lisFontesP->Count; nc++)
         {
         eqpto = ((VTArvores*)pcorte1->lisFontesP->Items[nc])->EqRaiz();
         //define o grupo de cargas
         for(int ng = 0; ng < lisGrupoCargas->Count; ng++)
            {
            lisC = (TList*)lisGrupoCargas->Items[ng];
            if(lisC->IndexOf(eqpto) != -1) break;
            }
         tview->Items->AddChildObject(node1, "C-"+eqpto->Codigo, lisC);
         lisAux->Assign(lisC, laOr);
         }

      for(int ncc = 0; ncc < lisAux->Count; ncc++)
         {
			//Imprime eqpto, carga[ncc], tx_falha[eqpto], trep[eqpto], "PRT"
         carga = (VTCarga*)lisAux->Items[ncc];
		 aux.sprintf("%s\t%s\t", pcorte1->eqpto->Codigo.c_str(), carga->Codigo.c_str());
         aux.cat_sprintf("%6.2f\t%6.2f\t%6.2f\t", pconf.tx_falha, pconf.treparo, pconf.comp);
         aux.cat_sprintf("PRT");
         //rchParCorte->Lines->Add(aux);
         }
      for(int n2 = 0; n2 < pcorte1->lisCortes->Count; n2++)
         {pcorte2 = (TCorteConf*)pcorte1->lisCortes->Items[n2];
          node2    = tview->Items->AddChildObject(node1, pcorte2->eqpto->Codigo, pcorte2);
          for(int n3 = 0; n3 < pcorte2->lisCortes->Count; n3++)
             {
             pcorte3 = (TCorteConf*)pcorte2->lisCortes->Items[n3];
             node3    = tview->Items->AddChildObject(node2, pcorte3->eqpto->Codigo, pcorte3);
             }
         }
      }
   delete lisCargasAux;
	}
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::MostraArvores(TTreeView *tview, TList *lisArvs)
   {
   VTArvores *arvs;
   VTArvore  *arv;
   TList     *lisPath;
   TTreeNode  *node1, *node2, *node3;
	AnsiString aux;

	tview->Items->Clear();

   for(int n = 0; n < lisArvs->Count; n++)
      {
      arvs = (VTArvores*)lisArvs->Items[n];
      aux  = (arvs->arvs_ori == NULL) ? "ORI_" : "COP_";
      aux.cat_sprintf("%s_%i", (arvs->EqRaiz())->Codigo.c_str(), n);
      lisPath = arvs->LisPath(false);
      node1 = tview->Items->AddChildObject(NULL, aux, lisPath);
      for(int na = 0; na < lisPath->Count; na++)
         {
         arv = (VTArvore*)lisPath->Items[na];
			node2 = tview->Items->AddChildObject(node1, IntToStr(na) + "-"+arv->Eqpto->Codigo, arv);
			}
      }
   }
///---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::TreeViewRedesClick(TObject *Sender)
   {
   //var local
   TTreeNode *node =  TreeViewRedes->Selected;
   //proteção
   if(! node) return;
   //cancela moldula
	Moldura();
   //apresenta indices de confiab no list view
   ApresentaIndices(node);
   }
//---------------------------------------------------------------------------
void __fastcall TFormConfiabilidade::TreeViewRedesChange(TObject *Sender,
      TTreeNode *Node)
   {
   //proteção
   if(! Node) return;
   //cancela moldula
   Moldura();
   //apresenta indices de confiab no list view
   ApresentaIndices(Node);
   }
//---------------------------------------------------------------------------
//eof





//---------------------------------------------------------------------------










