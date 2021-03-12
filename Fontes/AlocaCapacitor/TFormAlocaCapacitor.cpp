//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include <math.h>
#include <algorithm>
#include <complex>
using namespace std;
//---------------------------------------------------------------------------
#include "TFormAlocaCapacitor.h"
#include "VTAlocaBC.h"
#include "TLine.h"
#include "..\Apl\VTApl.h"
#include "..\Curvas\VTCurva.h"
#include "..\Diretorio\VTPath.h"
#include "..\Edita\VTEdita.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"                    //DVK 2013.07.24
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Planejamento\VTAlternativa.h"      //DVK 2013.07.24
#include "..\Planejamento\VTEstudo.h"           //DVK 2013.07.24
#include "..\Planejamento\VTPlanejamento.h"     //DVK 2013.07.24
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Report\VTReport.h"
#include "..\Sinap\VTSinapChild.h"
//---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Edita.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Rede.h"
#include "..\..\DLL_Inc\Report.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormAlocaCapacitor::TFormAlocaCapacitor(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
                              : TFormEdt(Owner, parent)
   {
   //variáveis locais
	VTEdita *edita = (VTEdita*)apl_owner->GetObject(__classid(VTEdita));

   //salva ponteiro p/ Apl
   this->apl   = apl_owner;
   if(! VerificaPlanejamento()) Close();           //DVK 2013.07.24
   //salva Obra originalmente associado a Edita
   obra_original = edita->Obra;
   //cria objeto Obra e associa com objeto Edita
   obra        = DLL_NewObjObra(apl);
   edita->Obra = obra;
   //cria objeto AlocaBC
   alocaBC     = NewObjAlocaBC(apl);
   //crias listas
   lisTabCap         = new TList();
   lisEQP            = new TList();
   lisCAP_ORIG       = new TList();
   //lisCAP_Removidos  = new TList();  //FKM 2015.09.22
   //preenche a lista de capacitores no grid
   CriaUmCapacitor();
	IniciaGrid();
   PageControl->ActivePage   = TabConfiguracao;
   TabCapAlocado->TabVisible = false;
   //habilita opções de alocação
   HabilitaOpcoes();
	//carrega a lista de capacitores existentes
   IniciaListaCapacitoresOriginais();
   //insere Form em seu Parent
	if (parent) {Parent = parent;}
	//posiciona o Form
	FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormAlocaCapacitor::~TFormAlocaCapacitor(void)
	{
   //variáveis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //apaga moldura
   Moldura();
	//salva últimos dados
	SalvaDados();
	//salva parâmetros do Form
	if (geral) geral->Infoset->SaveToFile(this);
   //restaura Obra original ao objeto Edita
   edita->Obra = obra_original;
   //destroi objetos criados
   if (obra)   {delete obra;    obra    = NULL;}
   if(alocaBC) {delete alocaBC; alocaBC = NULL;}
   //destroi a lista sem destruir seus objetos
   if(lisEQP)  {delete lisEQP;  lisEQP  = NULL;}
   if(lisCAP_ORIG) {delete lisCAP_ORIG; lisCAP_ORIG = NULL;}
   //if(lisCAP_Removidos)  {delete lisCAP_Removidos;  lisCAP_Removidos  = NULL;} //FKM
   //destroi a lista e os objetos
   if (lisTabCap) {LimpaTList(lisTabCap); delete lisTabCap; lisTabCap = NULL;}
   }
                                   
//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ActionAlocaBCExecute(TObject *Sender)
	{
	//var local
	VTCapacitor *cap;
	VTEdita     *edita  = (VTEdita*) apl->GetObject(__classid(VTEdita));
	VTGrafico   *graf   = (VTGrafico*) apl->GetObject(__classid(VTGrafico));
   //VTStk       *stk    = (VTStk*) apl->GetObject(__classid(VTStk));

   //valida parametros
   if(! LeParametros())
		{
      Aviso("Parâmetros inválidos !");
      return;
		}
   //lê lista de capacitores disponiveis
   if(! LeListaCapacitores() || lisTabCap->Count == 0)
		{
      Aviso("Capacitores inválidos !");
		return;
      }
	//IMPORTANTE: apaga Moldura para que não fique com eventual Eqpto que será
	//removido da Rede
   Moldura();  //DVK 2013.07.24      
	//restaura rede original
	RestauraRedeOriginal();
	//exibe gráfico de topologia, p/ evitar cálculo de fluxo durante a alocação
	//stk->ActionTopo->Execute();
	//remove capacitores existentes
	if ((checkDeleteLisCap->Checked)&&(lisCAP_ORIG->Count > 0))
		{//retira Capacitores originais da rede
		edita->RetiraLisEqpto(lisCAP_ORIG);
		//reinicia lisCAP_ORIG
		//lisCAP_ORIG->Clear();
		}
	//executa alocação de capacitores
	if (Executa())
		{//mostra a lista de capacitores alocados
		ExibeListaAlocada();
		}
	//atualiza grafico
	if(graf) {graf->AreaRedeAltera(); graf->Redraw();}
	//graf->AreaRedeDefine();
	}

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ActionAddExecute(TObject *Sender)
   {
   //atualiza tabela
   if(! LeListaCapacitores()) {Aviso("Capacitor inválido"); return;}
   //cria mais um capacitor
   CriaUmCapacitor();
   //apresenta na tela
   IniciaGrid();
   }
//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ActionCancelarExecute(TObject *Sender)
   {
   //FKM - 2015.09.22 - Verifica se houve alocação e a cancela mediante confirmacao
   //verifica se há capacitores alocados
   lisEQP->Clear();
   if (alocaBC->ListaCapacitorAlocado(lisEQP) != 0)
   {  //pede confirmação do usuário
      if(Confirma("Foi feita alocação de capacitores.",
                   "Deseja descartar os novos capacitores?", MB_YESNOCANCEL)== IDYES)
       { //caso sim
         CancelaAlocacao();
         //fecha o Form
         Close();
       }
       else
       {
         //nada a fazer
       }
   }else{Close();}
   //FKM - 2015.09.22
//   //incorpora os capacitores à rede
//   CancelaAlocacao();
//	//fecha o Form
//   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ActionConfirmarExecute(TObject *Sender)
   {
   //incorpora os capacitores à rede
   ConfirmaAlocacao();
   //FKM - 2015.09.22   Feedback ao usuário
   Aviso("Os bancos de capacitores foram alocados com sucesso");
   //FKM - 2015.09.22
   //fecha o form
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ActionDeleteExecute(TObject *Sender)
   {
	//proteção:
	int linha = gridLisCap->Row;
	if(linha < 1 ) return;
	//if(lisTabCap->Count == 0) return;
	if(lisTabCap->Count < 2) return;
	if(linha > lisTabCap->Count) return;
	//remove capacitor indicado
	VTCapacitor *cap = (VTCapacitor*) lisTabCap->Items[linha-1];
	lisTabCap->Delete(linha-1);
	DLL_DeleteEqpto(cap);
	//apresenta na tela
	IniciaGrid();
	}

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ActionExportaExecute(TObject *Sender)
	{
   //verifica qual tabela exportar
	if(PageControl->ActivePage == TabCapOriginal)      ExportaExistentes();
	else if(PageControl->ActivePage == TabCapAlocado)  ExportaAlocados();
	else Aviso("Seleciona a aba com capacitores existentes ou alocados para exportar a lista");
	}

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ActionHelpExecute(TObject *Sender)
	{
	//variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Alocação_de_Capacitor");
	if(formHelp) formHelp->Show();
	}

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ActionRefreshExecute(TObject *Sender)
	{
	//atualiza
   if(! LeListaCapacitores()) return;
   IniciaGrid(); 
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ActionRemoveCapExecute(TObject *Sender)
	{
   //var local
   VTEdita *edita = (VTEdita*) apl->GetObject(__classid(VTEdita));

   //obtém lista de capacitores alocados
   lisEQP->Clear();
	alocaBC->ListaCapacitorAlocado(lisEQP);
   //retira todos Capacitores alocados
   if (lisEQP->Count > 0) edita->RetiraLisEqpto(lisEQP);
   lisEQP->Clear();
   //limpa gráfico e list box
   for (int n = chart->SeriesCount()-1; n >= 0; n--)chart->Series[n]->Clear();
	ListBoxCap->Items->Clear();
   //apaga moldura
   Moldura();
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ActionRemoveCapExecute(TObject *Sender)
	{ //DVK 2015.07.22 reescrito para ficar semelhante ao AlocaRT
	//variaveis locais
	TList *lisCap = new TList();
	//VTCapacitor *capacitor; //FKM - 2015.09.23

	for (int i = ListBoxCap->Items->Count - 1; i >=0; i--)
		{
		if(! ListBoxCap->Selected[i]) continue;
		lisCap->Add(ListBoxCap->Items->Objects[i]);
      //FKM 2015.09.22 - Guarda os capacitores removidos para posterior remoção
      //capacitor = (VTCapacitor*)ListBoxCap->Items->Objects[i];
      //lisCAP_Removidos->Add(capacitor->Clone());
      //lisCAP_Removidos->Add(ListBoxCap->Items->Objects[i]);
      //FKM 2015.09.22
		ListBoxCap->Items->Delete(i);
		}
	//remove Capacitores alocados
	alocaBC->RetiraBancosAlocadosDaRede(lisCap);
   //limpa ListBox e Chart
//   ListBoxReg->Items->Clear();
	for (int n = chart->SeriesCount()-1; n >= 0; n--) chart->Series[n]->Clear();
	Moldura();
	//destroi lista
	if(lisCap) delete lisCap;
	}

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ActionSelAllExecute(TObject *Sender)
   {
   for (int i = 0; i < ListBoxCap->Items->Count; i++)
      {
      ListBoxCap->Selected[i] = true;
      }
   ListBoxCapClick(NULL);

   }
//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ActionSelNoneExecute(TObject *Sender)
   {
   for (int i = 0; i < ListBoxCap->Items->Count; i++)
      ListBoxCap->Selected[i] = false;
   ListBoxCapClick(NULL);
   }
//---------------------------------------------------------------------------
void   __fastcall TFormAlocaCapacitor::AjustaCurvas(TChart *Chart)
   {
   TChartSeries *serie;
   double  valor_min, valor_max;
   bool    first = true;
   //redefine os eixos
   //eixo x
   Chart->Axes->Bottom->Automatic = true;

   for(int n = 0; n < Chart->SeriesCount(); n++)
      {
      serie = Chart->Series[n];
      if(! serie->Active) continue;
      if (first)
         {
         first = false;
         valor_min = serie->YValues->MinValue;
         valor_max = serie->YValues->MaxValue;
         }
      else
         {
         valor_min = std::min(valor_min, serie->YValues->MinValue);
         valor_max = std::max(valor_max, serie->YValues->MaxValue);
         }
      }
   //eixo y
   Chart->Axes->Left->Automatic = false;
   Chart->Axes->Left->Minimum   = 0.;
   Chart->Axes->Left->Maximum   = 1.05 * valor_max;
   Chart->Axes->Left->Minimum   = 0.95 * valor_min;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::CancelaAlocacao(void)
   {
   //var local
   VTEdita     *edita  = (VTEdita*)apl->GetObject(__classid(VTEdita));   //DVK 2013.07.24
   VTGrafico   *graf   = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

   //IMPORTANTE: apaga Moldura para que não fique com eventual Eqpto que será removido da Rede
   Moldura();  //DVK 2013.07.24
   //restaura rede original
//   lisCAP_ORIG->Clear();
   RestauraRedeOriginal();
   //restaura Obra original ao objeto Edita
   edita->Obra = obra_original;             //DVK 2013.07.24
   //atualiza grafico
   if (graf) graf->AreaRedeAltera();        //DVK 2013.07.24
   //reinicia AlocaBC para que a lista de Capacitores alocados fique vazia
   alocaBC->Reinicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::checkLigadoClick(TObject *Sender)
   { //DVK 2014.11.11
   GBoxPatamar->Visible = checkLigado->Checked;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::CheckBoxOpcaoClick(TObject *Sender)
   {
   HabilitaOpcoes();
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ConfirmaAlocacao(void)
   {
   //variáveis locais
   TList   *lisCAP;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

   //IMPORTANTE: apaga Moldura para que não fique com eventual Eqpto que será removido da Rede
   Moldura();  //DVK 2013.07.24
   //monta lista com Clones dos Capacitores alocados
   lisCAP = new TList();
   alocaBC->ListaCloneCapacitorAlocado(lisCAP);
	//restaura rede original
	RestauraRedeOriginal();
   //associa Edita com seu objeto Obra original
   edita->Obra = obra_original;
	//verifica se usuário optou por remover capacitores pré-existentes
	if ((checkDeleteLisCap->Checked)&&(lisCAP_ORIG->Count > 0))
		{//retira Capacitores originais da rede
		edita->RetiraLisEqpto(lisCAP_ORIG);
		}
   //insere Capacitores na rede
   edita->InsereLisEqpto(lisCAP);
   //FKM 2015.09.22 - Retira os capacitores removidos (solução feia =( )
//   if(lisCAP_Removidos->Count > 0)
//   {  //remove Capacitores alocados
//	   edita->RetiraLisEqpto(lisCAP_Removidos);
//      //remove Capacitores alocados
//	   //alocaBC->RetiraBancosAlocadosDaRede(lisCAP_Removidos);
//   }
   //FKM 2015.09.22
   //destrói lista local sem destruir seus objetos
   delete lisCAP;
   //reinicia AlocaBC para que a lista de Capacitores alocados fique vazia
   alocaBC->Reinicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::CriaUmCapacitor(void)
   {
	//var locais
	VTGeral		*geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTPatamares *pat  = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTCapacitor *cap  =  DLL_NewObjCapacitor(pat->NumPatamar());  //cria capacitor p/ teste
	AnsiString	cap_q, cap_ligacao, max_mvar;

	cap->q       = 0.3; //300 kvar/unid
	cap->ligacao = 10; //unidades
	max_mvar = RichEditMaxMvar->Text;
	RichEditMaxMvar->Text = "10.0";
	if(geral)
		{//recupera do infoset
		if(geral->Infoset->LoadFromFile("TFormAlocaCapacitor_Dados"))
			{
			geral->Infoset->GetInfo("RichEditMaxMvar", max_mvar);
			RichEditMaxMvar->Text = max_mvar;
			geral->Infoset->GetInfo("cap->q[0]", cap_q);
			geral->Infoset->GetInfo("cap->ligacao[0]", cap_ligacao);
         //proteção
         if (! cap_q.IsEmpty())       cap->q       = StrToDouble(cap_q)/1000.0;
         if (! cap_ligacao.IsEmpty()) cap->ligacao = StrToInt(cap_ligacao);
         }
      }
   lisTabCap->Add(cap);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TFormAlocaCapacitor::Executa(void)
   {
   //variáveis locais
   VTEdita *edita   = (VTEdita*) apl->GetObject(__classid(VTEdita));

   //proteção
   if (! alocaBC) return(false);
   //executa alocação para montar Bancos de capacitores
   if (alocaBC->Executa(lisTabCap))
      {//insere na Rede os Capacitores dos Bancos montados
      lisEQP->Clear();
      alocaBC->ListaCapacitorAlocado(lisEQP);
      edita->InsereLisEqpto(lisEQP);
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TFormAlocaCapacitor::Executa(void)
   {
   //proteção
   if (! alocaBC) return(false);
   //se for primeira alocação, verifica se há primários desconexos
   if (num_alocacao++ == 0) VerificaPrimariosIsolados();
   //executa alocação
   return(alocaBC->Executa(lisTabCap));
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ExibeGrafico(VTEqpto *eqpto)
   {
   //variáveis locais
   double       q_mvar;
   VTPatamar    *patamar;
   VTCapacitor  *capacitor = (VTCapacitor*) eqpto;
   VTDemanda    *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   TChartSeries *series    = chart->Series[0];

   //limpa gráfico
   for(int n = 0; n < chart->SeriesCount(); n++) {chart->Series[n]->Clear();}
   //copia valores dos pontos da Curva própria
   for(int ind_pat = 0 ; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      patamar = patamares->Patamar[ind_pat];
      //obtém demanda do eqpto no patamar
      q_mvar  = demanda->Q_mvar[capacitor][ind_pat];
      series->AddXY(patamar->HoraIni, q_mvar);
      series->AddXY(patamar->HoraFim, q_mvar);
      }
   AjustaCurvas(chart);
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ExibeGrafico(TList *lisCAP)
   {
   double       q_mvar;
   VTCapacitor  *capacitor;
   VTPatamar    *patamar;
   VTDemanda    *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   TChartSeries *series    = chart->Series[0];

   //limpa gráfico
   for(int n = 0; n < chart->SeriesCount(); n++) {chart->Series[n]->Clear();}
   //proteção:
   if (lisCAP->Count == 0) return;
   //copia valores dos pontos da Curva própria
   for(int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      patamar      = patamares->Patamar[ind_pat];
      //soma demanda dos capacitores no patamar
      q_mvar = 0.;
      for(int n = 0; n < lisCAP->Count; n++)
         {
         capacitor = (VTCapacitor*)lisCAP->Items[n];
         q_mvar   += demanda->Q_mvar[capacitor][ind_pat];
         }
      series->AddXY(patamar->HoraIni, q_mvar);
      series->AddXY(patamar->HoraFim, q_mvar);
      }
   AjustaCurvas(chart);
	}
/*
//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ExibeListaAlocada(void)
	{
	//variaveis locais
	AnsiString  txt;
	VTCapacitor *cap;

	//obtém lista de Capacitores alocados
	lisEQP->Clear();
	alocaBC->ListaCapacitorAlocado(lisEQP);
	//mostra Capacitores alocados
	TabCapAlocado->TabVisible = true;
	PageControl->ActivePage   = TabCapAlocado;
	ListBoxCap->Items->Clear();
	for(int n = 0; n < lisEQP->Count; n++)
		{//
		cap = (VTCapacitor*) lisEQP->Items[n];
		ListBoxCap->Items->AddObject(txt.sprintf("Capacitor-%02d %.0f [kvar]",
				n+1,
				cap->Q_mvar * 1000),
				cap);
		}
	//pré-seleciona primeiro item
	if (ListBoxCap->Items->Count > 0) ListBoxCap->Selected[0] = true;
	//simula Click de mouse
	ListBoxCapClick(ListBoxCap);
	}
*/  //DVK 2015.09.25 reescrito
//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ExibeListaAlocada(void)
	{
	//variaveis locais
	AnsiString  txt;
	VTCapacitor *cap;

	//obtém lista de Capacitores alocados
	lisEQP->Clear();
	alocaBC->ListaCapacitorAlocado(lisEQP);
	//mostra Capacitores alocados
	TabCapAlocado->TabVisible = true;
	PageControl->ActivePage   = TabCapAlocado;
	ListBoxCap->Items->Clear();
	for(int n = 0; n < lisEQP->Count; n++)
		{//
		cap = (VTCapacitor*) lisEQP->Items[n];
		ListBoxCap->Items->AddObject(cap->Codigo + txt.sprintf(" %.0f [kvar]",
				cap->Q_mvar * 1000),
				cap);
		}
	//pré-seleciona primeiro item
	if (ListBoxCap->Items->Count > 0) ListBoxCap->Selected[0] = true;
	//simula Click de mouse
	ListBoxCapClick(ListBoxCap);
	}
//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ExportaAlocados(void)
	{
	//variaveis locais
	TLine			line;
	TStrings		*lines;
	VTCapacitor *cap;
	VTPath      *path  = (VTPath*) apl->GetObject(__classid(VTPath));

	//proteção
	if(! path) return;
	if(ListBoxCap->Items->Count == 0)
		{
		Aviso("Nenhum capacitor foi alocado");
		return;
		}
	try{//define pasta padrao para SaveDialog
		SaveDialog->InitialDir = path->DirExporta();
		//define nome padrao do arquivo
		SaveDialog->FileName = "CapacitoresAlocados.csv";
		//define extensão
		SaveDialog->DefaultExt = ".csv";
		//cria lines
		lines = new TStringList();
		if(! lines) return;
		lines->Clear();
		if(SaveDialog->Execute())
			{//primeira linha
			line.Clear();
			line.Separador = ';';
			line.InsereCampo("Lista de capacitores alocados e suas coordenadas (x,y) em [m]");
			//insere linha
			lines->Add(line.Texto());
			for (int nc = 0; nc < ListBoxCap->Items->Count; nc++)
				{//lista capacitores alocados
				cap = (VTCapacitor*)ListBoxCap->Items->Objects[nc];
				//reinicia line
				line.Clear();
				line.Separador = ';';
				//linha: codigo do capacitor; coord_utm x; coord_utm y;
				line.InsereCampo(cap->Codigo);
				line.InsereCampo(cap->pbarra->utm.x/100); //cm=>m
				line.InsereCampo(cap->pbarra->utm.y/100); //cm=>m
				//insere linha
				lines->Add(line.Texto());
				}
			//salva
			lines->SaveToFile(SaveDialog->FileName);
			}
		//destroi lines
		delete lines;
		Aviso("A lista de capacitores alocados foi exportada corretamente");
		}catch(Exception &e)
			{
			Aviso("Erro ao exportar lista de capacitores alocados");
			if(lines) delete lines;
			}
	}

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ExportaExistentes(void)
	{
	//variaveis locais
	AnsiString	nome_arq;
	VTReport 	*report;
	VTPath 		*path = (VTPath*)apl->GetObject(__classid(VTPath));

	//cria objeto report
	report = DLL_NewObjReport(apl);
	//esconde form, para a janela Salvar Como não ficar por baixo
	Hide();
	//define pasta inicial
	SaveDialog->InitialDir = path->DirExporta();
	if(SaveDialog->Execute())
		{//recebe nome do arquivo a ser exportado e exporta listView
		nome_arq = SaveDialog->FileName;
		if(report->ArquivoTexto(nome_arq, ListViewCap, "", false))
			Aviso("A tabela foi exportada corretamente.");
		else Erro("Erro ao exportar tabela.");
      }
	//destroi report
	if(report) {DLL_DeleteObjReport(report); report = NULL;}
	//reapresenta form
	Show();
	}

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o Form
   Action = caFree;
	}

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::FormCloseQuery(TObject *Sender, bool &CanClose)
   {
   //assume que o form pode ser fechado
   CanClose = true;
   //verifica se há capacitores alocados
   lisEQP->Clear();
   if (alocaBC->ListaCapacitorAlocado(lisEQP) == 0) return;
   //pede confirmação do usuário
   switch(Confirma("Foi feita alocação de capacitores.",
                   "Deseja incorporar os novos capacitores à rede ?", MB_YESNOCANCEL))
      {
      case IDYES:
         ConfirmaAlocacao();
         CanClose = true;
         break;
      case IDNO:
         CancelaAlocacao();
         CanClose = true;
         break;
      case IDCANCEL:
      default:
         CanClose = false;
         break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form sem alinhamento
   Left = 0;
   Top  = 0;
	}

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::FormShow(TObject *Sender)
   {
   //reposiciona o Form
	//Top  = 0;
   //Left = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::HabilitaOpcoes(void)
   {
   //DVK 2014.11.11  preenche com patamares
   //variaveis locais
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   VTPatamar   *patamar;

   for(int np = 0; np < patamares->LisPatamar()->Count; np++)
      {
      patamar = (VTPatamar*)patamares->LisPatamar()->Items[np];
      if(! patamar->Enabled) continue;
      CBoxPatamar->AddItem(patamar->Periodo, patamar);
      }
   //seleciona primeiro
   CBoxPatamar->ItemIndex = 0;
   //outras opções
   RichEditMaxMvar->Visible   = checkQLimite->Checked;
   RichEditFatpot->Visible    = CheckBoxFatpot->Checked;
   RichEditDistancia->Visible = CheckBoxDistancia->Checked;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::IniciaGrid(void)
   {
   //variavel local
   VTCapacitor *cap;
   //titulo
   //AnsiString tit[] = {"Índice", "Mvar/unid.", "Unidades", "Mvar"};
	AnsiString tit[] = {"Índice", "kvar/unid.", "Unidades"};
   int        col   = sizeof(tit)/sizeof(tit[0]);

	for(int n=0; n< gridLisCap->RowCount; n++)
      gridLisCap->Rows[n]->Clear();

   gridLisCap->ColCount = col;
   gridLisCap->RowCount = lisTabCap->Count + 1;
	if(gridLisCap->RowCount > 1) gridLisCap->FixedRows = 1;
	if(gridLisCap->ColCount > 1) gridLisCap->FixedCols = 1;
   for(int n=0; n< col; n++) gridLisCap->Cells[n][0] = tit[n];

	//preenche lista de capacitores
	for(int n=0; n< lisTabCap->Count; n++)
		{
      cap = (VTCapacitor *) lisTabCap->Items[n];
      gridLisCap->Cells[0][n+1] = IntToStr(n+1);
		gridLisCap->Cells[1][n+1] = DoubleToStr("%.1f", 1000*cap->Q_mvar);
		gridLisCap->Cells[2][n+1] = IntToStr(cap->ligacao);
		//gridLisCap->Cells[3][n+1] = DoubleToStr("%.1f", 1000*cap->ligacao*cap->Q_mvar);
		}
   }
//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::IniciaListaCapacitoresOriginais(void)
   {
   //var local
//   TListItem   *item;
   TList       *lisCap;
   VTRede      *rede;
//   VTCapacitor *cap;
   VTRedes     *redes   = (VTRedes*) apl->GetObject(__classid(VTRedes));
   TList       *lisRede = redes->LisRede();

   try{//reinicia ListViewCap
      ListViewCap->Clear();
      //reinicia lisCAP_ORIG
      lisCAP_ORIG->Clear();
      //obtem a lista de capacitores por rede
      for(int i = 0; i < lisRede->Count; i++)
         {
         rede = (VTRede*) lisRede->Items[i];
         lisEQP->Clear();
         rede->LisEqbar(lisEQP, eqptoCAPACITOR);
         //copia a lista de capacitores existentes
         CopiaTList(lisEQP, lisCAP_ORIG);
         IniciaListView(rede);
//         for(int ncap = 0; ncap < lisEQP->Count; ncap++)
//            {
//            cap = (VTCapacitor*) lisEQP->Items[ncap];
//            // ID > 0: existente no banco de dados
//            item = ListViewCap->Items->Add();
//            item->Data = cap;
//            item->Caption = rede->Codigo;
//            item->SubItems->Add(cap->Codigo);
//				item->SubItems->Add(DoubleToStr("%f", cap->Q_mvar));
//            }
         }
      }
    catch(Exception &e)
      {
      }
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::IniciaListView(VTRede *rede)
   {   //DVK 2013.07.24
   //var local
   TListItem   *item;
   VTCapacitor *cap;

   //reinicia ListViewCap
   //ListViewCap->Clear();     //DVK 2013.07.30
   //FKM - OBS: lisEQP com todos capacitores da rede
   for(int ncap = 0; ncap < lisEQP->Count; ncap++)
      {
      cap = (VTCapacitor*) lisEQP->Items[ncap];
      // ID > 0: existente no banco de dados
      item = ListViewCap->Items->Add();
      item->Data = cap;
      item->Caption = rede->Codigo;
      item->SubItems->Add(cap->Codigo);
//      item->SubItems->Add(DoubleToStr("%4.3f", cap->Q_mvar));
      //DVK 2014.11.11 unidade kvar
      item->SubItems->Add(DoubleToStr("%2.1f", 1000. * cap->Q_mvar));
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormAlocaCapacitor::LeListaCapacitores(void)
   {
   //variveis locais
   VTCapacitor *cap;
   double kvar_per_unid;
   int    unid;

   try
      {
      for(int ncap = 0; ncap < lisTabCap->Count; ncap++)
         {
         cap = (VTCapacitor *) lisTabCap->Items[ncap];
         //kvar/unid
			kvar_per_unid = StrToDouble(gridLisCap->Cells[1][ncap+1]);
         //unidades
         unid = StrToInt(gridLisCap->Cells[2][ncap+1]);
         if (unid <= 0)
            {
            Aviso("Número de capacitores disponíveis incorreto");
            return(false);
            }
         //valor negativo
         if (kvar_per_unid < 0)
            {
            kvar_per_unid = fabs(kvar_per_unid);
            }
         //salva valores
         cap->q        = RoundTo(kvar_per_unid/1000. , -4); //Mvar
         cap->ligacao  = unid; //temporário
         }
      }
      catch(Exception &e)
         {
         Erro("Erro na leitura dos capacitores disponíveis");
         return false;
         }
   return true;
   }

//---------------------------------------------------------------------------
bool __fastcall TFormAlocaCapacitor::LeParametros(void)
   {
   try{//copia parâmetros
      alocaBC->cfg.qmax_definido   = checkQLimite->Checked;
      alocaBC->cfg.qmax_mvar       = StrToDouble(RichEditMaxMvar->Text);
      alocaBC->cfg.fatpot_definido = CheckBoxFatpot->Checked;
      alocaBC->cfg.fatpot_min      = StrToDouble(RichEditFatpot->Text);
      alocaBC->cfg.dist_definido   = CheckBoxDistancia->Checked;
      alocaBC->cfg.dist_m          = StrToDouble(RichEditDistancia->Text);
      alocaBC->cfg.custo_energia   = StrToDouble(editCusto->Text);
      alocaBC->cfg.tronco          = checkTronco->Checked; //DVK 2013.07.24
      alocaBC->cfg.sempre_ligado   = checkLigado->Checked;   //DVK 2014.11.11
      alocaBC->cfg.ind_patamar     = CBoxPatamar->ItemIndex; //DVK 2014.11.11
      }
      catch(Exception &e)
         {
         Aviso("Parâmetro inválido: \n" + e.Message);
         return false;
         }
   return true;
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ListBoxCapClick(TObject *Sender)
   {
	//var local
   VTEqpto *eqpto;
   VTGrafico *graf   = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

   lisEQP->Clear();
	for (int i = 0; i < ListBoxCap->Items->Count; i++)
      {
      if (ListBoxCap->Selected[i])
         {
         eqpto = (VTEqpto *) ListBoxCap->Items->Objects[i];
         lisEQP->Add(eqpto);
         }
      }
   //desenha a moldura
   graf->Moldura(lisEQP);
   //exibe o gráfico do item selecionado
   ExibeGrafico(lisEQP);
   }
                
//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ListBoxCapDblClick(TObject *Sender)
   {
   //var local
   VTEqpto   *eqpto;
   VTGrafico *graf   = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

   //determina Item selecionado
   if (ListBoxCap->ItemIndex < 0) return;
   //determina Eqpto associado ao Item
   eqpto = (VTEqpto *) ListBoxCap->Items->Objects[ListBoxCap->ItemIndex];
   //executa zoom no gráfico
   if (eqpto) graf->SelecionaZoom(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ListViewCapColumnClick(TObject *Sender,
      TListColumn *Column)
   {
   //verifica se selecionou a mesma coluna
   if (LViewSort.Column == Column)
      {//inverte ordem
      LViewSort.OrdemCrescente  = ! LViewSort.OrdemCrescente;
      }
   else
      {//salva coluna usada p/ ordenação
      LViewSort.Column         = Column;
      LViewSort.OrdemCrescente = true;
      }
   //força reordenação
   ((TListView*)Sender)->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ListViewCapCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
   {
   //variáveis locais
   int       index;
   double    valor1, valor2;

   //proteção
   if (LViewSort.Column == NULL) return;
   //determina index p/ substrings
   index = LViewSort.Column->Index - 1;
   //verifica se coluna de Caption
   if (LViewSort.Column->Index == 0)
      {//coluna de Caption
      Compare = CompareText(Item1->Caption,Item2->Caption);
      }
   else if (LViewSort.Column->Alignment == taLeftJustify)
      {//coluna com texto
      Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
      }
   else
      {//coluna com valor numérico
      valor1  = StrToDouble(Item1->SubItems->Strings[index]);
      valor2  = StrToDouble(Item2->SubItems->Strings[index]);
      if      (valor1 < valor2) Compare = -1;
      else if (valor1 > valor2) Compare =  1;
      else                      Compare =  0;
      }
   //verifica se ordem crescente/decrescente
   if (! LViewSort.OrdemCrescente) Compare = - Compare;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ListViewCapClick(TObject *Sender)
   {
   if(ListViewCap->Selected == NULL) return;
   //moldura no equipamento
   Moldura((VTEqpto*) ListViewCap->Selected->Data);
   }
   
//---------------------------------------------------------------------------

void __fastcall TFormAlocaCapacitor::ListViewCapDblClick(TObject *Sender)
   {
   if(ListViewCap->Selected == NULL) return;
   //zoom no equipamento
   Zoom((VTEqpto*) ListViewCap->Selected->Data);
   }
//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::ListViewCapSelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
   {
   //proteção
   if(! Selected) return;
   Moldura((VTEqpto*) Item->Data);
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::Moldura(VTEqpto *eqpto)
   {
   //var local
   VTGrafico *graf = (VTGrafico*) apl->GetObject(__classid(VTGrafico));

   if (graf) graf->Moldura(eqpto);
	}

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::RestauraRedeOriginal(void)
	{
   //variáveis locais
	VTCapacitor  *cap;
	VTEdita      *edita       = (VTEdita*) apl->GetObject(__classid(VTEdita));
	VTSinapChild *sinap_child = (VTSinapChild*) apl->GetObject(__classid(VTSinapChild));

   //exibe gráfico de topologia, p/ evitar cálculo de fluxo durante alteração da rede
	sinap_child->ActionTopo->Execute();
   //volta topologia original     //DVK 2013.07.24
   edita->UndoCompleto();
   edita->Clear();
   /*
   //remove todos capacitores alocados
   if(checkDeleteLisCap->Checked)
      {
      ActionRemoveCapExecute(NULL); //DVK 2013.07.24
      }
   //insere capacitores originais da rede (caso tenham sido removidos)
   if ((lisCAP_ORIG->Count == 0)&&(ListViewCap->Items->Count > 0))
      {//reinicia lisCAP_ORIG c/ os capacitores originais a partir do ListViewCap
      for (int ncap = 0; ncap < ListViewCap->Items->Count; ncap++)
         {
         cap = (VTCapacitor*) ListViewCap->Items->Item[ncap]->Data;
         if (lisCAP_ORIG->IndexOf(cap) < 0) lisCAP_ORIG->Add(cap);
         }
      //restaura os Capacitores originais
      if (lisCAP_ORIG->Count > 0) edita->InsereLisEqpto(lisCAP_ORIG);
      }

   for(int ncap = 0; ncap < lisCAP_ORIG->Count; ncap++)
      {
      cap = (VTCapacitor*) lisCAP_ORIG->Items[ncap];
      cap->pbarra->InsereEqbar(cap);
      }
   */
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::SalvaDados(void)
	{
	//variaveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	//salva parâmetros
	if (geral)
		{//limite max. do capacitor
		geral->Infoset->AddInfo("RichEditMaxMvar", RichEditMaxMvar->Text.Trim());
		for(int n = 0; n < lisTabCap->Count; n++)
			{//lista de capacitores
			geral->Infoset->AddInfo("cap->q["+AnsiString(n)+"]", gridLisCap->Cells[1][n+1]);
			geral->Infoset->AddInfo("cap->ligacao["+AnsiString(n)+"]", gridLisCap->Cells[2][n+1]);
         }
		geral->Infoset->SaveToFile("TFormAlocaCapacitor_Dados");
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::VerificaPrimariosIsolados(void)
   {
   //variáveis locais
   AnsiString texto;
   VTRede     *rede;

   //determina lista de Primarios isolados
   lisEQP->Clear();
   if (alocaBC->VerificaPrimariosIsolados(lisEQP) > 0)
      {
      texto = "Os seguintes primários estão isolados e não serão analisados:";
      for (int n = 0; n < lisEQP->Count; n++)
         {
         rede = (VTRede*)lisEQP->Items[n];
         texto += "\n " + rede->Codigo;
         }
      Aviso(texto);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormAlocaCapacitor::VerificaPlanejamento(void)
   {
   VTPlanejamento *planejamento = (VTPlanejamento*) apl->GetObject(__classid(VTPlanejamento));
   VTEstudo       *estudo;

   //proteção: rede original //DVK 2013.07.24
   estudo = planejamento->ExisteEstudo();
   if((estudo->Tipo > 0) && (estudo->AlternativaAtiva == estudo->AlternativaPadrao))
      {
      Aviso("Não é possível alocar capacitores na rede original");
      return(false);
      }
   else if((estudo->Tipo > 0) && (estudo->AlternativaAtiva->Obras->LisObra()->Count == 0))
      {
      Aviso("Crie primeiro uma obra para esta alternativa");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormAlocaCapacitor::Zoom(VTEqpto *eqpto)
	{
   //var local
   VTGrafico *graf = (VTGrafico*) apl->GetObject(__classid(VTGrafico));
   if(graf && eqpto) graf->SelecionaZoom(eqpto);
   }

//---------------------------------------------------------------------------
//eof


