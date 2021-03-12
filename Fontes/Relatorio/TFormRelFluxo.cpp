//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TFormRelFluxo.h"
#include "TFormSelArranjo.h"
#include "TFormSelClasse.h"
#include "TFormSelRedePorHierarquia.h"
#include "TFormSelRedePorTipo.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Flow\VTFlow.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTTrecho.h"
#include "..\Report\VTReport.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Report.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormRelFluxo::TFormRelFluxo(TForm *Owner, VTApl *apl_owner, TWinControl *parent)
   : TForm(Owner)
   {
   //variáveis locais
   VTRedes *redes = (VTRedes*)apl_owner->GetObject(__classid(VTRedes));

   //salva ponteiro p/ objetos
   apl = apl_owner;
   //redefine parent
   if (parent) Parent = parent;
   //cria objetos
   lisAUX      = new TList();  //FKM
   lisEQP      = new TList();
   lisARRANJO  = new TList();
   lisCLASSE   = new TList();
   lisREDE     = new TList();
   lisADEQUADA = new TList();
   lisPRECARIA = new TList();
   lisCRITICA  = new TList();
   lisISOLADA  = new TList();
   //inicia o check sem fluxo
   check_semFluxo = false;
   //define ordenação do ListView
   ListViewSort.ColumnIndex    = 1;
   ListViewSort.OrdemCrescente = true;
   //se necessário, executa cálculo de fluxo
   ExecutaFluxoPotencia();
   //inicia lisREDE com todas as Redes
   redes->LisRede(lisREDE);
   //cria TFormSelRedePorHieraquia
   FormSelRedePorHierarquia = new TFormSelRedePorHierarquia(this, apl, GBoxFiltroRede);
   FormSelRedePorHierarquia->DefineActionOnSelRede(ActionSelLisRede);
   //FormSelRedePorHieraquia->Show();
   //cria TFormSelRedePorTipo
   FormSelRedePorTipo = new TFormSelRedePorTipo(this, apl, GBoxFiltroRede);
   FormSelRedePorTipo->DefineActionOnSelRede(ActionSelLisRede);
   //FormSelRedePorTipo->Show();
   //cria TFormSelArranjo
   FormSelArranjo = new TFormSelArranjo(this, apl, GBoxFiltroArranjo);
   FormSelArranjo->DefineActionOnSelArranjo(ActionSelLisArranjo);
   FormSelArranjo->Show();
   //cria TFormSelClasse
   FormSelClasse = new TFormSelClasse(this, apl, GBoxFiltroClasse);
   FormSelClasse->DefineActionOnSelClasse(ActionSelLisClasse);
   FormSelClasse->Show();
	//cor default //DVK 2014.12.10
	cor_eqpto = clLime;
   //inicia CBoxPatamar
   CBoxPatamarInicia();
   //simula seleção do modo de apresentação das redes
   RadioGroupModoClick(NULL);
   //exibe relatório de Barras
	Atualiza();
	}

//---------------------------------------------------------------------------
__fastcall TFormRelFluxo::~TFormRelFluxo(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	//cancela destaque de Eqptos
	GraficoDestacaEqpto();
   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //não destrói TFormSelArranjo, FormSelClasse e TFormSelRedePorTipo
   //if (FormSelArranjo) {delete FormSelArranjo; FormSelArranjo = NULL;}
   //if (FormSelClasse)  {delete FormSelClasse;  FormSelClasse  = NULL;}
   //if (FormSelRedePorTipo)    {delete FormSelRedePorTipo;    FormSelRedePorTipo    = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisAUX)      {delete lisAUX;      lisAUX      = NULL;}
   if (lisEQP)      {delete lisEQP;      lisEQP      = NULL;}
   if (lisARRANJO)  {delete lisARRANJO;  lisARRANJO  = NULL;}
   if (lisCLASSE)   {delete lisCLASSE;   lisCLASSE   = NULL;}
   if (lisREDE)     {delete lisREDE;     lisREDE     = NULL;}
   if (lisADEQUADA) {delete lisADEQUADA; lisADEQUADA = NULL;}
   if (lisPRECARIA) {delete lisPRECARIA; lisPRECARIA = NULL;}
   if (lisCRITICA)  {delete lisCRITICA;  lisCRITICA  = NULL;}
   if (lisISOLADA)  {delete lisISOLADA;  lisISOLADA  = NULL;}
   //libera memória do DynamicArray
   rel_qtde.array.Length = 0;
   rel_perc.array.Length = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ActionAnimacaoExecute(TObject *Sender)
   {
   Timer->Enabled = ! Timer->Enabled;
   //atualiza Caption
   ActionAnimacao->Caption = (Timer->Enabled) ? "Parar" : "Iniciar";
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ActionDefineCorExecute(TObject *Sender)
	{//DVK 2014.12.10
	//determina cor de destaque dos eqptos
	ColorDialog->Color = cor_eqpto;
	if(ColorDialog->Execute())
		{
		cor_eqpto = ColorDialog->Color;
		}
	//atualiza
	if(MenuItemFaixaAdequada->Checked) ActionDestacaEqptoAdequadoExecute(NULL);
	if(MenuItemFaixaCritica->Checked)  ActionDestacaEqptoCriticoExecute(NULL);
	if(MenuItemFaixaPrecaria->Checked) ActionDestacaEqptoPrecarioExecute(NULL);
	}

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ActionDestacaEqptoAdequadoExecute(TObject *Sender)
	{
	//desmarca outros MenuItems //DVK 2014.12.10
	MenuItemFaixaAdequada->Checked = true;
	MenuItemFaixaPrecaria->Checked = false;
	MenuItemFaixaCritica->Checked  = false;
   //teste
   ListaEqptoDestaque(lisAUX);
	//destaca
	GraficoDestacaEqpto(lisADEQUADA);
	}

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ActionDestacaEqptoCriticoExecute(TObject *Sender)
	{
	//desmarca outros MenuItems //DVK 2014.12.10
	MenuItemFaixaAdequada->Checked = false;
	MenuItemFaixaPrecaria->Checked = false;
	MenuItemFaixaCritica->Checked  = true;
   //teste
   ListaEqptoDestaque(lisAUX);
	//destaca
	GraficoDestacaEqpto(lisCRITICA);
	}

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ActionDestacaEqptoPrecarioExecute(TObject *Sender)
	{
	//desmarca outros MenuItems //DVK 2014.12.10
	MenuItemFaixaAdequada->Checked = false;
	MenuItemFaixaPrecaria->Checked = true;
	MenuItemFaixaCritica->Checked  = false;
   //teste
   ListaEqptoDestaque(lisAUX);
	//destaca
	GraficoDestacaEqpto(lisPRECARIA);
	}

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ActionExportaExecute(TObject *Sender)
   {
   //variáveis locais
   VTReport   *report;
   AnsiString arq_ext;

   try{//determina o arquivo
      if (! SelecionaArquivo()) return;
      //cria objeto VTReport
      report = DLL_NewObjReport(apl);
      //imprime o relatório
      arq_ext = ExtractFileExt(SaveDialog->FileName);
      if (arq_ext.AnsiCompareIC(".xls") == 0)
         {
         report->PlanilhaExcel(SaveDialog->FileName, "Demanda", LView);
         }
      else
         {
         report->ArquivoTexto(SaveDialog->FileName, LView);
         }
      //destrói VTReport
      DLL_DeleteObjReport(report);
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ActionHelpExecute(TObject *Sender)
{  //variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Relatórios_Relatório_de_Fluxo_de_Potência.mht");
	if(formHelp) formHelp->Show();

}
//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ActionRelBarraExecute(TObject *Sender)
   {
	//variáveis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int         num_pat    = patamares->NumPatamar();
   int         num_patHabilitado    = patamares->NumPatamarHabilitado();
   int		   indexPatHabilitado = 0;

   //marca que esta no modo Barras
   modoSelecionado = modoBARRAS;
	//cancela destaque de Eqptos
	GraficoDestacaEqpto();
	//define captions para os componentes da tela
	rel_qtde.val_inteiro = true;
	//DVK 2014.12.10 quantidade, ao invés de número
	rel_qtde.caption     = "Quantidade de barras por faixa de tensão";
   rel_perc.val_inteiro = false;
   rel_perc.caption     = "Percentual de barras por faixa de tensão";
   //atualiza ButRelBarra
	ButRelBarra->Down = true;
	//esconde GBoxFiltroClasse e GBoxFiltroArranjo //DVK 2015.09.14
	GBoxFiltroClasse->Visible  = false;
	GBoxFiltroArranjo->Visible = false;
   //monta vetor com quantidade de barras por patamar
   if (rel_qtde.array.Length != num_patHabilitado) rel_qtde.array.Length = num_patHabilitado;
   //obtém lista de Barras das Redes selecionadas em lisREDE
   lisEQP->Clear();
   IniciaLisEqpto(lisEQP, eqptoBARRA);
   //loop p/ todos patamares
   for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
      {
	  patamar = patamares->Patamar[ind_pat];
	  if(patamar->Enabled == false)
		  {continue;}
	  //classifica Barras por nível de tensão
      ClassificaBarra(lisEQP, patamar->Index);
      //atualiza rel_qtde.array
	  rel_qtde.array[indexPatHabilitado].patamar   = patamar;
	  rel_qtde.array[indexPatHabilitado].adequada  = lisADEQUADA->Count;
	  rel_qtde.array[indexPatHabilitado].precaria  = lisPRECARIA->Count;
	  rel_qtde.array[indexPatHabilitado].critica   = lisCRITICA->Count;
	  rel_qtde.array[indexPatHabilitado].semfluxo  = lisISOLADA->Count;
	  indexPatHabilitado++;
      //teste
//      if(check_semFluxo)
//         {
//         rel_qtde.array[ind_pat].total = lisADEQUADA->Count + lisPRECARIA->Count
//                                          + lisCRITICA->Count + lisISOLADA->Count;
//         }
//      else
//         {
//         rel_qtde.array[ind_pat].total = lisADEQUADA->Count + lisPRECARIA->Count
//                                          + lisCRITICA->Count;
//         }
	  }
   //inicia rel_qtde.array.total
   IniciaArrayValorTotal();
   //inicia rel_perc.array
   IniciaArrayValorPercentual();
   //atualiza LView
   LViewInicia();
   //atualiza Chart
	Chart01patInicia();
	Chart24patInicia();
	//atualiza eqptos destacados  //DVK 2014.12.10
	if(MenuItemFaixaAdequada->Checked) ActionDestacaEqptoAdequadoExecute(NULL);
	if(MenuItemFaixaCritica->Checked)  ActionDestacaEqptoCriticoExecute(NULL);
	if(MenuItemFaixaPrecaria->Checked) ActionDestacaEqptoPrecarioExecute(NULL);
	}

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ActionRelCargaExecute(TObject *Sender)
   {
	//variáveis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int         num_pat    = patamares->NumPatamar();
   int         num_patHabilitado    = patamares->NumPatamarHabilitado();
   int		   indexPatHabilitado = 0;

   //marca que esta no modo CArgas
   modoSelecionado = modoCARGAS;
	//cancela destaque de Eqptos
	GraficoDestacaEqpto();
	//define cpations para os componentes da tela
	rel_qtde.val_inteiro = true;
	//DVK 2014.12.10 quantidade, ao invés de número
	rel_qtde.caption     = "Quantidade de cargas por faixa de tensão";
   rel_perc.val_inteiro = false;
   rel_perc.caption     = "Percentual de cargas por faixa de tensão";
   //atualiza ButRelBarra
   ButRelCarga->Down = true;
   //exibe GBoxFiltroClasse e esconde GBoxFiltroArranjo
   GBoxFiltroClasse->Visible  = true;
   GBoxFiltroArranjo->Visible = false;
   //monta vetor com quantidade de barras por patamar
   if (rel_qtde.array.Length != num_patHabilitado) rel_qtde.array.Length = num_patHabilitado;
   //obtém lista de Cargas das Redes selecionadas em lisREDE
   lisEQP->Clear();
   IniciaLisEqpto(lisEQP, eqptoCARGA);
   FiltraLisCarga(lisEQP);
   //loop p/ todos patamares
   for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
      {
	  patamar = patamares->Patamar[ind_pat];
	  if(patamar->Enabled == false)
		  {continue;}
      //classifica Cargas por nível de tensão
      ClassificaCarga(lisEQP, patamar->Index);
      //atualiza rel_qtde.array
	  rel_qtde.array[indexPatHabilitado].patamar  = patamar;
	  rel_qtde.array[indexPatHabilitado].adequada = lisADEQUADA->Count;
	  rel_qtde.array[indexPatHabilitado].precaria = lisPRECARIA->Count;
	  rel_qtde.array[indexPatHabilitado].critica  = lisCRITICA->Count;
	  rel_qtde.array[indexPatHabilitado].semfluxo  = lisISOLADA->Count;
	  indexPatHabilitado++;
      //teste
//      if(check_semFluxo)
//         {
//         rel_qtde.array[ind_pat].total = lisADEQUADA->Count + lisPRECARIA->Count
//                                          + lisCRITICA->Count + lisISOLADA->Count;
//         }
//      else
//         {
//         rel_qtde.array[ind_pat].total = lisADEQUADA->Count + lisPRECARIA->Count
//                                          + lisCRITICA->Count;
//         }
      }
   //inicia rel_qtde.array.total
   IniciaArrayValorTotal();
   //inicia rel_perc.array
   IniciaArrayValorPercentual();
   //atualiza LView
   LViewInicia();
   //atualiza Chart
	Chart01patInicia();
	Chart24patInicia();
	//atualiza eqptos destacados   //DVK 2014.12.10
	if(MenuItemFaixaAdequada->Checked) ActionDestacaEqptoAdequadoExecute(NULL);
	if(MenuItemFaixaCritica->Checked)  ActionDestacaEqptoCriticoExecute(NULL);
	if(MenuItemFaixaPrecaria->Checked) ActionDestacaEqptoPrecarioExecute(NULL);
	}

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ActionRelDemandaExecute(TObject *Sender)
   {
	//variáveis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int         num_pat    = patamares->NumPatamar();
   int         num_patHabilitado    = patamares->NumPatamarHabilitado();
   int		   indexPatHabilitado = 0;

   //marca que esta no modo DEmanda
   modoSelecionado = modoDEMANDA;
	//cancela destaque de Eqptos
	GraficoDestacaEqpto();
   //confiura parâmetros para os relatórios
   rel_qtde.val_inteiro = false;
   rel_qtde.caption     = "Demanda por faixa de tensão (kW)";
   rel_perc.val_inteiro = false;
   rel_perc.caption     = "Percentual de demanda por faixa de tensão";
   //atualiza ButRelBarra
   ButRelDemanda->Down = true;
   //exibe GBoxFiltroClasse e esconde GBoxFiltroArranjo
   GBoxFiltroClasse->Visible  = true;
   GBoxFiltroArranjo->Visible = false;
   //monta vetor com quantidade de Cargas por patamar
	if (rel_qtde.array.Length != num_patHabilitado) rel_qtde.array.Length = num_patHabilitado;
   //obtém lista de Cargas das Redes selecionadas em lisREDE
   lisEQP->Clear();
   IniciaLisEqpto(lisEQP, eqptoCARGA);
   FiltraLisCarga(lisEQP);
   //loop p/ todos patamares
   for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
      {
	  patamar = patamares->Patamar[ind_pat];
	  if(patamar->Enabled == false)
		  {continue;}
      //classifica Cargas por nível de tensão
      ClassificaCarga(lisEQP, patamar->Index);
      //atualiza rel_qtde.array
	  rel_qtde.array[indexPatHabilitado].patamar  = patamar;
	  rel_qtde.array[indexPatHabilitado].adequada = Demanda_kw(lisADEQUADA, patamar->Index);
	  rel_qtde.array[indexPatHabilitado].precaria = Demanda_kw(lisPRECARIA, patamar->Index);
	  rel_qtde.array[indexPatHabilitado].critica  = Demanda_kw(lisCRITICA,  patamar->Index);
	  rel_qtde.array[indexPatHabilitado].semfluxo = Demanda_kw(lisISOLADA,  patamar->Index);
	  indexPatHabilitado++;
	  //teste
//      if(check_semFluxo)
//         {
//         rel_qtde.array[ind_pat].total =  Demanda_kw(lisADEQUADA, patamar->Index)
//                                        + Demanda_kw(lisPRECARIA, patamar->Index)
//                                        + Demanda_kw(lisCRITICA,  patamar->Index)
//                                        + Demanda_kw(lisISOLADA,  patamar->Index);
//         }
//      else
//         {
//         rel_qtde.array[ind_pat].total =  Demanda_kw(lisADEQUADA, patamar->Index)
//                                        + Demanda_kw(lisPRECARIA, patamar->Index)
//                                        + Demanda_kw(lisCRITICA,  patamar->Index);
//         }
      }
   //inicia rel_qtde.array.total
   IniciaArrayValorTotal();
   //inicia rel_perc.array
   IniciaArrayValorPercentual();
   //atualiza LView
   LViewInicia();
	//atualiza Chart
	Chart01patInicia();
	Chart24patInicia();
	//atualiza eqptos destacados   //DVK 2014.12.10
	if(MenuItemFaixaAdequada->Checked) ActionDestacaEqptoAdequadoExecute(NULL);
	if(MenuItemFaixaCritica->Checked)  ActionDestacaEqptoCriticoExecute(NULL);
	if(MenuItemFaixaPrecaria->Checked) ActionDestacaEqptoPrecarioExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ActionRelErroExecute(TObject *Sender)
   {//variáveis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int         num_pat    = patamares->NumPatamar();
   int         num_patHabilitado    = patamares->NumPatamarHabilitado();
   int		   indexPatHabilitado = 0 ;

   //marca que esta no modo DEmanda
   modoSelecionado = modoDEMANDA;
	//cancela destaque de Eqptos
	GraficoDestacaEqpto();
   //confiura parâmetros para os relatórios
   rel_qtde.val_inteiro = false;
   rel_qtde.caption     = "Demanda por faixa de tensão (kW)";
   rel_perc.val_inteiro = false;
   rel_perc.caption     = "Percentual de demanda por faixa de tensão";
   //atualiza ButRelBarra
   ButRelDemanda->Down = true;
   //exibe GBoxFiltroClasse e esconde GBoxFiltroArranjo
   GBoxFiltroClasse->Visible  = true;
   GBoxFiltroArranjo->Visible = false;
   //monta vetor com quantidade de Cargas por patamar
   if (rel_qtde.array.Length != num_patHabilitado) rel_qtde.array.Length = num_patHabilitado;
   //obtém lista de Cargas das Redes selecionadas em lisREDE
   lisEQP->Clear();
   IniciaLisEqpto(lisEQP, eqptoCARGA);
   FiltraLisCarga(lisEQP);
   //loop p/ todos patamares
   for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
      {
	  patamar = patamares->Patamar[ind_pat];
	  if(patamar->Enabled == false)
		  {continue;}
      //classifica Cargas por nível de tensão
      ClassificaCarga(lisEQP, patamar->Index);
      //atualiza rel_qtde.array
	  rel_qtde.array[indexPatHabilitado].patamar  = patamar;
	  rel_qtde.array[indexPatHabilitado].adequada = Demanda_kw(lisADEQUADA, patamar->Index);
	  rel_qtde.array[indexPatHabilitado].precaria = Demanda_kw(lisPRECARIA, patamar->Index);
	  rel_qtde.array[indexPatHabilitado].critica  = Demanda_kw(lisCRITICA,  patamar->Index);
	  rel_qtde.array[indexPatHabilitado].semfluxo = Demanda_kw(lisISOLADA,  patamar->Index);
	  indexPatHabilitado++;
	  //teste
//      if(check_semFluxo)
//         {
//         rel_qtde.array[ind_pat].total =  Demanda_kw(lisADEQUADA, patamar->Index)
//                                        + Demanda_kw(lisPRECARIA, patamar->Index)
//                                        + Demanda_kw(lisCRITICA,  patamar->Index)
//                                        + Demanda_kw(lisISOLADA,  patamar->Index);
//         }
//      else
//         {
//         rel_qtde.array[ind_pat].total =  Demanda_kw(lisADEQUADA, patamar->Index)
//                                        + Demanda_kw(lisPRECARIA, patamar->Index)
//                                        + Demanda_kw(lisCRITICA,  patamar->Index);
//         }
      }
   //inicia rel_qtde.array.total
   IniciaArrayValorTotal();
   //inicia rel_perc.array
   IniciaArrayValorPercentual();
   //atualiza LView
   LViewInicia();
	//atualiza Chart
	Chart01patInicia();
	Chart24patInicia();
	//atualiza eqptos destacados   //DVK 2014.12.10
	if(MenuItemFaixaAdequada->Checked) ActionDestacaEqptoAdequadoExecute(NULL);
	if(MenuItemFaixaCritica->Checked)  ActionDestacaEqptoCriticoExecute(NULL);
	if(MenuItemFaixaPrecaria->Checked) ActionDestacaEqptoPrecarioExecute(NULL);
   }
//---------------------------------------------------------------------------

void __fastcall TFormRelFluxo::ActionRelTrafoExecute(TObject *Sender)
   {
	//variáveis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int         num_pat    = patamares->NumPatamar();
   int         num_patHabilitado    = patamares->NumPatamarHabilitado();
   int		   indexPatHabilitado = 0;

   //marca que esta no modo Trafos
   modoSelecionado = modoTRAFOS;
	//cancela destaque de Eqptos
	GraficoDestacaEqpto();
   //define captions para os componentes da tela
	rel_qtde.val_inteiro = false;
	//DVK 2014.12.10 sem unidade (km)
	rel_qtde.caption     = "Transformadores por faixa de carregamento";
   rel_perc.val_inteiro = false;
   rel_perc.caption     = "Percentual transformadores por faixa de carregamento";
   //atualiza ButRelBarra
   ButRelTrafo->Down = true;
	//esconde GBoxFiltroArranjo e GBoxFiltroClasse  //DVK 2014.12.10 os dois
   GBoxFiltroArranjo->Visible = false;//true;
   GBoxFiltroClasse->Visible  = false;
   //monta vetor com km de trecho por patamar
  if (rel_qtde.array.Length != num_patHabilitado) rel_qtde.array.Length = num_patHabilitado;
   //obtém lista de Trafo das Redes selecionadas em lisREDE
   lisEQP->Clear();
   IniciaLisEqpto(lisEQP, eqptoTRAFO);
   //loop p/ todos patamares
   for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
      {
		patamar = patamares->Patamar[ind_pat];
		if(patamar->Enabled == false)
		  {continue;}
      //classifica Trafos por nível de tensão
      ClassificaLigacao(lisEQP, patamar->Index);
      //atualiza rel_qtde.array
	  rel_qtde.array[indexPatHabilitado].patamar  = patamar;
	  rel_qtde.array[indexPatHabilitado].adequada = lisADEQUADA->Count;
	  rel_qtde.array[indexPatHabilitado].precaria = lisPRECARIA->Count;
	  rel_qtde.array[indexPatHabilitado].critica  = lisCRITICA->Count;
	  rel_qtde.array[indexPatHabilitado].semfluxo = lisISOLADA->Count;
	  indexPatHabilitado++;
      //teste
//      if(check_semFluxo)
//         {
//         rel_qtde.array[ind_pat].total = lisADEQUADA->Count + lisPRECARIA->Count
//                                          + lisCRITICA->Count + lisISOLADA->Count;
//         }
//      else
//         {
//         rel_qtde.array[ind_pat].total = lisADEQUADA->Count + lisPRECARIA->Count
//                                          + lisCRITICA->Count;
//         }
      }
   //inicia rel_qtde.array.total
   IniciaArrayValorTotal();
   //inicia rel_perc.array
   IniciaArrayValorPercentual();
   //atualiza LView
   LViewInicia();
   //atualiza Chart
	Chart01patInicia();
	Chart24patInicia();
	//atualiza eqptos destacados //DVK 2014.12.10
	if(MenuItemFaixaAdequada->Checked) ActionDestacaEqptoAdequadoExecute(NULL);
	if(MenuItemFaixaCritica->Checked)  ActionDestacaEqptoCriticoExecute(NULL);
	if(MenuItemFaixaPrecaria->Checked) ActionDestacaEqptoPrecarioExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ActionRelTrechoExecute(TObject *Sender)
   {
	//variáveis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int         num_pat    = patamares->NumPatamar();
   int         num_patHabilitado    = patamares->NumPatamarHabilitado();
   int		   indexPatHabilitado = 0;

   //marca que esta no modo Barras
   modoSelecionado = modoTRECHOS;
	//cancela destaque de Eqptos
	GraficoDestacaEqpto();
   //define cpations para os componentes da tela
   rel_qtde.val_inteiro = false;
   rel_qtde.caption     = "Extensão de rede por faixa de carregamento (km)";
   rel_perc.val_inteiro = false;
   rel_perc.caption     = "Percentual de rede por faixa de carregamento";
   //atualiza ButRelBarra
   ButRelTrecho->Down = true;
   //exibe GBoxFiltroArranjo e esconde GBoxFiltroClasse
   GBoxFiltroArranjo->Visible = true;
   GBoxFiltroClasse->Visible  = false;
   //monta vetor com km de trecho por patamar
   if (rel_qtde.array.Length != num_patHabilitado) rel_qtde.array.Length = num_patHabilitado;
   //obtém lista de Trecho das Redes selecionadas em lisREDE
   lisEQP->Clear();
   IniciaLisEqpto(lisEQP, eqptoTRECHO);
   FiltraLisTrecho(lisEQP);
   //loop p/ todos patamares
   for (int ind_pat = 0; ind_pat < num_pat; ind_pat++)
      {
	  patamar = patamares->Patamar[ind_pat];
	  if(patamar->Enabled == false)
		  {continue;}
      //classifica Cargas por nível de tensão
	  ClassificaLigacao(lisEQP, patamar->Index);
      //atualiza rel_qtde.array
	  rel_qtde.array[indexPatHabilitado].patamar  = patamar;
	  rel_qtde.array[indexPatHabilitado].adequada = ComprimentoTrecho_km(lisADEQUADA);
	  rel_qtde.array[indexPatHabilitado].precaria = ComprimentoTrecho_km(lisPRECARIA);
	  rel_qtde.array[indexPatHabilitado].critica  = ComprimentoTrecho_km(lisCRITICA);
	  rel_qtde.array[indexPatHabilitado].semfluxo = ComprimentoTrecho_km(lisISOLADA);
	  indexPatHabilitado++;
      //teste
//      if(check_semFluxo)
//         {
//         rel_qtde.array[ind_pat].total =  ComprimentoTrecho_km(lisADEQUADA)
//                                        + ComprimentoTrecho_km(lisPRECARIA)
//                                        + ComprimentoTrecho_km(lisCRITICA)
//                                        + ComprimentoTrecho_km(lisISOLADA);
//         }
//      else
//         {
//         rel_qtde.array[ind_pat].total =  ComprimentoTrecho_km(lisADEQUADA)
//                                        + ComprimentoTrecho_km(lisPRECARIA)
//                                        + ComprimentoTrecho_km(lisCRITICA);
//         }
      }
   //inicia rel_qtde.array.total
   IniciaArrayValorTotal();
   //inicia rel_perc.array
   IniciaArrayValorPercentual();
   //atualiza LView
   LViewInicia();
   //atualiza Chart
	Chart01patInicia();
	Chart24patInicia();
	//atualiza eqptos destacados  //DVK 2014.12.10
	if(MenuItemFaixaAdequada->Checked) ActionDestacaEqptoAdequadoExecute(NULL);
	if(MenuItemFaixaCritica->Checked)  ActionDestacaEqptoCriticoExecute(NULL);
	if(MenuItemFaixaPrecaria->Checked) ActionDestacaEqptoPrecarioExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ActionSelLisArranjoExecute(TObject *Sender)
   {
   //variáveis locais
   VTRede *rede;
   TList  *lisEXT = (TList*)Sender;

   //copia lista de Arranjos selecionados
   lisARRANJO->Clear();
   CopiaTList(lisEXT, lisARRANJO);
   //reinicia relatório selecionado
   if (ButRelTrecho->Down)  ButRelTrecho->Action->Execute();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ActionSelLisClasseExecute(TObject *Sender)
   {
   //variáveis locais
   VTRede *rede;
   TList  *lisEXT = (TList*)Sender;

   //copia lista de Classes selecionadas
   lisCLASSE->Clear();
   CopiaTList(lisEXT, lisCLASSE);
   //reinicia relatório selecionado
   if      (ButRelCarga->Down)   ButRelCarga->Action->Execute();
   else if (ButRelDemanda->Down) ButRelDemanda->Action->Execute();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ActionSelLisRedeExecute(TObject *Sender)
   {
   //variáveis locais
   VTRede *rede;
   TList  *lisEXT = (TList*)Sender;

   //copia lista de Redes selecionadas
   lisREDE->Clear();
   CopiaTList(lisEXT, lisREDE);
   //reinicia relatório selecionado
   if      (ButRelBarra->Down)   ButRelBarra->Action->Execute();
   else if (ButRelCarga->Down)   ButRelCarga->Action->Execute();
	else if (ButRelDemanda->Down) ButRelDemanda->Action->Execute();
	//DVK 2015.09.14 trata trafos e trechos
	else if (ButRelTrafo->Down)   ButRelTrafo->Action->Execute();
	else if (ButRelTrecho->Down)  ButRelTrecho->Action->Execute();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::Atualiza(void)
   {
   //exibe relatório de Barras
   ActionRelBarra->Execute();
	}

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::CBoxPatamarChange(TObject *Sender)
   {
   //atualiza Chart01pat
   Chart01patInicia();
   }

//---------------------------------------------------------------------
void __fastcall TFormRelFluxo::CBoxPatamarInicia(void)
   {
   //variáveis locais
   VTPatamar   *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   TList       *lisPAT  = new TList();

   //obtém lista de patamares habilitados
   patamares->LisPatamarHabilitado(lisPAT);
   //reinicia CBoxPatamar c/ Patamares
   CBoxPatamar->Clear();
   for (int np = 0; np < lisPAT->Count; np++)
      {
      patamar = (VTPatamar*)lisPAT->Items[np];
      CBoxPatamar->Items->AddObject(patamar->Nome, patamar);
      }
   //pré-seleciona primeiro patamar
   if (CBoxPatamar->Items->Count > 0) CBoxPatamar->ItemIndex = 0;
   //destrói lista
   delete lisPAT;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::Chart01patInicia(void)
   {
   //variáveis locais
   int       index;
	StructRel  *rel;
   VTPatamar *patamar;

   //determina array a ser exibido
//	if (CheckBoxPercentual->Checked) rel = &rel_perc;
//   else                             rel = &rel_qtde;
   rel = &rel_perc; //sempre exibe o "%" na legenda
   //determina Patamar selecionado
   if ((index = CBoxPatamar->ItemIndex) < 0) return;
   if ((patamar = (VTPatamar*)CBoxPatamar->Items->Objects[index]) == NULL) return;
   index = patamar->Index;
   //reinicia PieSeriesComObra
   PieSeriesComObra->Clear();
	//insere valores
	PieSeriesComObra->Add(rel->array[ConverteIndexPatHabilitado(patamar)].adequada, "Adequada", clGreen);
	PieSeriesComObra->Add(rel->array[ConverteIndexPatHabilitado(patamar)].precaria, "Precária", clYellow);
   PieSeriesComObra->Add(rel->array[ConverteIndexPatHabilitado(patamar)].critica,  "Crítica",  clRed);
   //se tiver o sem fluxo, adiciona
   if(check_semFluxo)
      {PieSeriesComObra->Add(rel->array[ConverteIndexPatHabilitado(patamar)].semfluxo,  "Sem Fluxo",  clDkGray);}
	}

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::Chart24patInicia(void)
   {
	//variaveis locais
	double     duracao, y;
	TTime      x, ultimoX;
	StructRel  *rel;
	bool isPercent;
	VTPatamar   *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	int         num_pat    = patamares->NumPatamar();
	int         num_patHabilitado    = patamares->NumPatamarHabilitado();
	int		   indexPatHabilitado = 0;
	double valorInicio;

   isPercent = CheckBoxPercentual->Checked;
   //determina array a ser exibido
	if (CheckBoxPercentual->Checked)
      {
      rel = &rel_perc;
      Chart24pat->Axes->Left->AxisValuesFormat = "#,##0.##";
      }
   else
      {
      rel = &rel_qtde;
      Chart24pat->Axes->Left->AxisValuesFormat = "#0.###";
      }
   //atualiza título
   Chart24pat->Title->Caption = GBoxPlanilha->Caption;
	//reinicia max e series
   SeriesAdequada->Clear();
   SeriesPrecaria->Clear();
	SeriesCritica->Clear();
   //teste
   SeriesSemFluxo->Clear();
   SeriesTotal->Clear();
	//inicia x
	x = 0.;
	ultimoX=0;
	for (int n = 0; n < rel->array.Length; n++)
		{

		x = ((double)(rel->array[n].patamar->HoraIni)/24) + (((double)(rel->array[n].patamar->MinutoIni)/60)/24);
		//verifica se é preciso preencher com zeros
		if( (!IsDoubleZero(ultimoX - x)) && (n != 0))
		{
			//adequada
			SeriesAdequada->AddNullXY(ultimoX,   NULL, "");
			SeriesAdequada->AddNullXY(x, NULL, "");
			//precária
			SeriesPrecaria->AddNullXY(ultimoX,   NULL, "");
			SeriesPrecaria->AddNullXY(x, NULL, "");
			//crítica
			SeriesCritica->AddNullXY(ultimoX,  NULL,   "");
			SeriesCritica->AddNullXY(x, NULL,   "");
		  //sem fluxo
			if(check_semFluxo)
			 {
			 SeriesSemFluxo->AddNullXY(ultimoX,   NULL,   "");
			   SeriesSemFluxo->AddNullXY(x, NULL,   "");
			 }
		  else
			 {
			 SeriesSemFluxo->AddNullXY(ultimoX,   NULL,   "");
			   SeriesSemFluxo->AddNullXY(x, NULL,   "");
			 }
		  //teste
		  if(isPercent)
			 {
			 SeriesTotal->AddNullXY(ultimoX,   NULL,   "");
			   SeriesTotal->AddNullXY(x, NULL,   "");
			 }
		  else
			 {
			 SeriesTotal->AddNullXY(ultimoX,  NULL,   "");
			 SeriesTotal->AddNullXY(x, NULL,   "");
			 }
		}

		//determina duração do patamar
		duracao = rel->array[n].patamar->Duracao / 24;
		//adequada
		SeriesAdequada->AddXY(x,   rel->array[n].adequada, "", SeriesAdequada->Color);
		SeriesAdequada->AddXY(x+duracao, rel->array[n].adequada, "", SeriesAdequada->Color);
		//precária
		SeriesPrecaria->AddXY(x,   rel->array[n].precaria, "", SeriesPrecaria->Color);
		SeriesPrecaria->AddXY(x+duracao, rel->array[n].precaria, "", SeriesPrecaria->Color);
		//crítica
		SeriesCritica->AddXY(x,   rel->array[n].critica,   "", SeriesCritica->Color);
		SeriesCritica->AddXY(x+duracao, rel->array[n].critica,   "", SeriesCritica->Color);
      //sem fluxo
		if(check_semFluxo)
         {
         SeriesSemFluxo->AddXY(x,   rel->array[n].semfluxo,   "", SeriesSemFluxo->Color);
		   SeriesSemFluxo->AddXY(x+duracao, rel->array[n].semfluxo,   "", SeriesSemFluxo->Color);
         }
      else
         {
         SeriesSemFluxo->AddXY(x,   0,   "", SeriesSemFluxo->Color);
		   SeriesSemFluxo->AddXY(x+duracao, 0,   "", SeriesSemFluxo->Color);
         }
      //teste
      if(isPercent)
         {
         SeriesTotal->AddXY(x,   0,   "", SeriesTotal->Color);
		   SeriesTotal->AddXY(x+duracao, 0,   "", SeriesTotal->Color);
         }
      else
         {
         SeriesTotal->AddXY(x,   rel->array[n].total,   "", SeriesTotal->Color);
			SeriesTotal->AddXY(x+duracao, rel->array[n].total,   "", SeriesTotal->Color);
         }
		//atualiza horario, incrementando a duração do patamar
		//x += duracao;
		ultimoX = x+duracao;
		}
	//define limites do Chart
	Chart24pat->LeftAxis->Minimum = 0.0;
	Chart24pat->LeftAxis->Increment = 0.;
	//inicio
	valorInicio = ((double)(rel->array[0].patamar->HoraIni)/24) + (((double)(rel->array[0].patamar->MinutoIni)/60)/24);
	Chart24pat->BottomAxis->Minimum = valorInicio;
    Chart24pat->BottomAxis->Maximum = x +  duracao;
	Chart24pat->BottomAxis->Increment = rel->array[0].patamar->Duracao / 24.;
	Chart24pat->BottomAxis->DateTimeFormat = "hh:mm";
	}


//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ClassificaBarra(TList *lisBARRA, int ind_pat)
   {
   //variáveis locais
   VTBarra *barra;

   //reinicia listas
   lisADEQUADA->Clear();
   lisPRECARIA->Clear();
   lisCRITICA->Clear();
   lisISOLADA->Clear();
   //loop p/ todas Barras
   for (int n = 0; n < lisBARRA->Count; n++)
      {
      barra = (VTBarra*)lisBARRA->Items[n];
      switch(barra->resflow->Diagnostico[ind_pat])
         {
         case diagBOM:     lisADEQUADA->Add(barra); break;
         case diagREGULAR: lisPRECARIA->Add(barra); break;
         case diagRUIM:    lisCRITICA->Add(barra);  break;
         case diagISOLADO: lisISOLADA->Add(barra);  break;     //FKM
         default:                                   break;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ClassificaCarga(TList *lisCARGA, int ind_pat)
   {
   //variáveis locais
   VTCarga *carga;

   //reinicia listas
   lisADEQUADA->Clear();
   lisPRECARIA->Clear();
   lisCRITICA->Clear();
   lisISOLADA->Clear();
   //loop p/ todas Cargas
   for (int n = 0; n < lisCARGA->Count; n++)
      {
      carga = (VTCarga*)lisCARGA->Items[n];
      switch(carga->pbarra->resflow->Diagnostico[ind_pat])
         {
         case diagBOM:     lisADEQUADA->Add(carga); break;
         case diagREGULAR: lisPRECARIA->Add(carga); break;
         case diagRUIM:    lisCRITICA->Add(carga);  break;
         case diagISOLADO: lisISOLADA->Add(carga);  break;     //FKM
         default:                                   break;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ClassificaLigacao(TList *lisLIGACAO, int ind_pat)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //reinicia listas
   lisADEQUADA->Clear();
   lisPRECARIA->Clear();
   lisCRITICA->Clear();
   lisISOLADA->Clear();
   //loop p/ todas Trechos
   for (int n = 0; n < lisLIGACAO->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIGACAO->Items[n];
      switch(ligacao->resflow->Diagnostico[ind_pat])
         {
         case diagBOM:     lisADEQUADA->Add(ligacao); break;
         case diagREGULAR: lisPRECARIA->Add(ligacao); break;
         case diagRUIM:    lisCRITICA->Add(ligacao);  break;
         case diagISOLADO: lisISOLADA->Add(ligacao);  break;     //FKM
         default:                                    break;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::CheckBoxPercentualClick(TObject *Sender)
   {
   //atualiza LView
   LViewInicia();
   //atualiza Chart
	Chart01patInicia();
	Chart24patInicia();
   }
//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::CheckBoxSemFluxoClick(TObject *Sender)
   {
   check_semFluxo = CheckBoxSemFluxo->Checked;

   ExibeInfoEqptoSemFluxo(check_semFluxo);
   //inicia rel_qtde.array total
   IniciaArrayValorTotal();
   //inicia rel_perc.array
   IniciaArrayValorPercentual();
   //atualiza LView
   LViewInicia();
   //atualiza Chart
	Chart01patInicia();
	Chart24patInicia();
   }
//---------------------------------------------------------------------------
double __fastcall TFormRelFluxo::ComprimentoTrecho_km(TList *lisTRECHO)
   {
   //variáveis locais
   double   comp_km = 0;
   VTTrecho *trecho;

   //loop p/ todos Trechos
   for (int n = 0; n < lisTRECHO->Count; n++)
      {
      trecho   = (VTTrecho*)lisTRECHO->Items[n];
      comp_km += trecho->Comprimento_km;
      }
   return(comp_km);
   }

//---------------------------------------------------------------------------
int __fastcall TFormRelFluxo::ConverteIndexPatHabilitado(VTPatamar *patamar)
{
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int index_pat;
   TList *lisPatamaresHabilitados = new TList;

   patamares->LisPatamarHabilitado(lisPatamaresHabilitados);
   index_pat = lisPatamaresHabilitados->IndexOf(patamar);

   DestroiTObject(lisPatamaresHabilitados);
   return (index_pat);
}
//---------------------------------------------------------------------------
double __fastcall TFormRelFluxo::Demanda_kw(TList *lisCARGA, int ind_pat)
   {
   //variáveis locais
   double   p_mw = 0;
	VTCarga *carga;
   VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //loop p/ todas Cargas
   for (int n = 0; n < lisCARGA->Count; n++)
      {
      carga = (VTCarga*)lisCARGA->Items[n];
      p_mw += demanda->P_mw[carga][ind_pat];
      }
   return(p_mw * 1000.);
	}

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ExecutaFluxoPotencia(void)
	{
   //variáveis locais
	VTFlow       *flow        = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

   //verifica se o fluxo está atualizado
   if (! flow->UpToDate)
      {//executa o fluxo
		sinap_child->ActionFlow->Execute();
      }
	}

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ExibeInfoEqptoSemFluxo(bool exibe)
   {//variavel local
   TListColumn *coluna;
   int         index;
   AnsiString  nomeCol;


   if(exibe)
      {
      //adiciona a coluna no listview
      coluna = LView->Columns->Add();
      coluna->Caption = strSEM_FLUXO;
      coluna->Width = 65;
      coluna->Alignment = taRightJustify;
      }
   else
      {//verifica a ultima coluna
      index = LView->Columns->Count - 1;
      //proteção
      if(index < 0) return;
      //seleciona a ultima coluna
      coluna = LView->Columns->Items[index];
      //verifica se é a "Sem Fluxo"
      if(coluna != NULL)
         {
         nomeCol = coluna->Caption;
         if(nomeCol.AnsiCompare(strSEM_FLUXO) == 0)
            {
            LView->Columns->Delete(index);
            }
         }
      }
   }
//---------------------------------------------------------------------------
int __fastcall TFormRelFluxo::FiltraLisCarga(TList *lisCARGA)
   {
   //variáveis locais
   VTCarga *carga;

   //mantém em lisCARGA somente as Cragas cujas Classes foram selecionadas
   for (int n = lisCARGA->Count-1; n >= 0; n--)
      {
      carga = (VTCarga*)lisCARGA->Items[n];
      //verifica se a Classe da Carga existe na lista de Classes
      if (lisCLASSE->IndexOf(carga->classe) < 0)
         {//retira Carga da lista
         lisCARGA->Delete(n);
         }
      }
   return(lisCARGA->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TFormRelFluxo::FiltraLisTrecho(TList *lisTRECHO)
   {
   //variáveis locais
   VTTrecho *trecho;

   //mantém em lisTRECHO somente os Trechos cujos Arranjos foram selecionadas
   for (int n = lisTRECHO->Count-1; n >= 0; n--)
      {
      trecho = (VTTrecho*)lisTRECHO->Items[n];
      //verifica se o Arranjo do Trecho existe na lista de Arranjos
      if (lisARRANJO->IndexOf(trecho->arranjo) < 0)
         {//retira Trecho da lista
         lisTRECHO->Delete(n);
         }
      }
   return(lisTRECHO->Count);
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form canto superior esquerdo
   Top  = 0;
   Left = 0;;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::GraficoDestacaEqpto(TList *lisEQP)
	{
	//variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	grafico->DestacaEqpto(lisEQP, cor_eqpto);
	}

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::IniciaArrayValorPercentual(void)
   {
   //variáveis locais
   double total;

   //proteção
   if (rel_perc.array.Length != rel_qtde.array.Length) rel_perc.array.Length = rel_qtde.array.Length;

   //calcula o percentual incluindo os sem fluxo
   if(check_semFluxo == true)
      {
      //loop para todos elementos do array
      for (int ind_pat = 0; ind_pat < rel_perc.array.Length; ind_pat++)
         {//determina total do arrqy_qtde
         total = rel_qtde.array[ind_pat].adequada + rel_qtde.array[ind_pat].precaria
                  + rel_qtde.array[ind_pat].critica + rel_qtde.array[ind_pat].semfluxo;
         //proteção
         if (IsDoubleZero(total)) total = 1.;
         //atualiza rel_perc.array com valores percentuais
         rel_perc.array[ind_pat].patamar   =  rel_qtde.array[ind_pat].patamar;
         rel_perc.array[ind_pat].adequada  = (rel_qtde.array[ind_pat].adequada / total) * 100;
         rel_perc.array[ind_pat].precaria  = (rel_qtde.array[ind_pat].precaria / total) * 100;
         rel_perc.array[ind_pat].critica   = (rel_qtde.array[ind_pat].critica  / total) * 100;
         rel_perc.array[ind_pat].semfluxo  = (rel_qtde.array[ind_pat].semfluxo / total) * 100;
         rel_perc.array[ind_pat].total     = 100;
         }
      }
   else
      {
      //loop para todos elementos do array
      for (int ind_pat = 0; ind_pat < rel_perc.array.Length; ind_pat++)
         {//determina total do arrqy_qtde
         total = rel_qtde.array[ind_pat].adequada + rel_qtde.array[ind_pat].precaria + rel_qtde.array[ind_pat].critica;
         //proteção
         if (IsDoubleZero(total)) total = 1.;
         //atualiza rel_perc.array com valores percentuais
         rel_perc.array[ind_pat].patamar   =  rel_qtde.array[ind_pat].patamar;
         rel_perc.array[ind_pat].adequada  = (rel_qtde.array[ind_pat].adequada / total) * 100;
         rel_perc.array[ind_pat].precaria  = (rel_qtde.array[ind_pat].precaria / total) * 100;
         rel_perc.array[ind_pat].critica   = (rel_qtde.array[ind_pat].critica  / total) * 100;
         rel_perc.array[ind_pat].semfluxo  = 0;
         rel_perc.array[ind_pat].total     = 100;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::IniciaArrayValorTotal(void)
   {//variáveis locais
   double total;

   //proteção
   if (rel_perc.array.Length != rel_qtde.array.Length) rel_perc.array.Length = rel_qtde.array.Length;

   //calcula o percentual incluindo os sem fluxo
   if(check_semFluxo == true)
      {
      //loop para todos elementos do array
      for (int ind_pat = 0; ind_pat < rel_perc.array.Length; ind_pat++)
         {//determina total do arrqy_qtde
         total = rel_qtde.array[ind_pat].adequada + rel_qtde.array[ind_pat].precaria
                  + rel_qtde.array[ind_pat].critica + rel_qtde.array[ind_pat].semfluxo;
         //proteção
		 //if (IsDoubleZero(total)) total = 1.;
         //atualiza rel_perc.array com valores percentuais
         rel_qtde.array[ind_pat].total = total;
         }
      }
   else
      {
      //loop para todos elementos do array
      for (int ind_pat = 0; ind_pat < rel_perc.array.Length; ind_pat++)
         {//determina total do arrqy_qtde
         total = rel_qtde.array[ind_pat].adequada + rel_qtde.array[ind_pat].precaria
                  + rel_qtde.array[ind_pat].critica;
         //proteção
		 //if (IsDoubleZero(total)) total = 1.;
         //atualiza rel_perc.array com valores percentuais
         rel_qtde.array[ind_pat].total     = total;
         }
      }

   }
//---------------------------------------------------------------------------
int __fastcall TFormRelFluxo::IniciaLisEqpto(TList *lisEQP, int eqpto_tipo)
   {
   //variáveis locais
   VTRede *rede;

   //loop para todas Redes em lisREDE
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      rede->LisEqpto(lisEQP, eqpto_tipo);
      }
   return(lisEQP->Count);
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::ListaEqptoDestaque(TList *lisEXT)
   {//variaveis locais
   int index_pat;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //limpa a lista
   lisEXT->Clear();
   //define patamar escolhido
   if (CBoxPatamar->ItemIndex >= 0)
      {
      index_pat = CBoxPatamar->ItemIndex;
      }
   else
      {
	  if(patamares->Patamar_ativo)
         {index_pat = patamares->Patamar_ativo->Index;}
      else
         {index_pat = 0;}
      }
   //age de acordo com o modo do form
   switch (modoSelecionado)
      {
      case modoBARRAS:
         IniciaLisEqpto(lisEXT, eqptoBARRA);
         ClassificaBarra(lisEXT,index_pat);
         break;
      case modoCARGAS:
         IniciaLisEqpto(lisEXT, eqptoCARGA);
         ClassificaCarga(lisEXT,index_pat);
         break;
      case modoDEMANDA:
         break;
      case modoTRAFOS:
         IniciaLisEqpto(lisEXT, eqptoTRAFO);
         ClassificaLigacao(lisEXT,index_pat);
         break;
      case modoTRECHOS:
         IniciaLisEqpto(lisEXT, eqptoTRECHO);
         ClassificaLigacao(lisEXT,index_pat);
         break;
      default:
        ;
      }
   }
//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::LViewColumnClick(TObject *Sender, TListColumn *Column)
   {
   //verifica se selecionou a mesma coluna
   if (ListViewSort.ColumnIndex == Column->Index)
      {//inverte ordem
      ListViewSort.OrdemCrescente  = ! ListViewSort.OrdemCrescente;
      }
   else
      {//redefine índice da coluna usada p/ ordenação
      ListViewSort.ColumnIndex = Column->Index;
      }
   //força reordenação
   LView->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::LViewCompare(TObject *Sender, TListItem *Item1,
      TListItem *Item2, int Data, int &Compare)
   {
   //variáveis locais
   int    index = ListViewSort.ColumnIndex-1;
   double valor1, valor2;

   //proteção
   if ((index < 0)||(index >= LView->Columns->Count)) index = 0;
   switch(ListViewSort.ColumnIndex)
      {
      case 0:  //patamar
         Compare = CompareText(Item1->Caption, Item2->Caption);
         break;
      case 1:
      case 2:
      case 3:
      default:  //quantidade de eqpto
         valor1 = StrToDouble(Item1->SubItems->Strings[index]);
         valor2 = StrToDouble(Item2->SubItems->Strings[index]);
         if      (valor1 < valor2) Compare = -1;
         else if (valor1 > valor2) Compare =  1;
         else                      Compare =  0;
         break;
      }
   //verifica se ordem crescente/decrescente
   if (! ListViewSort.OrdemCrescente) Compare = - Compare;
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::LViewInicia(void)
   {
   //variáveis locais
   StructRel *rel;
   TListItem *Item;

   //determina array a ser exibido
   if (CheckBoxPercentual->Checked) rel = &rel_perc;
   else                             rel = &rel_qtde;
   //atualiza caption de GBoxPlanilha
   GBoxPlanilha->Caption = rel->caption;
   //reinicia LView
   LView->Clear();
   //loop p/ todos elementos de arraqy_qtde
   for (int n = 0; n < rel_qtde.array.Length; n++)
      {//insere ListItem
      Item = LView->Items->Add();
      Item->Data    = rel->array[n].patamar;
      Item->Caption = rel->array[n].patamar->Nome;
      if (rel->val_inteiro)
         {//formata valores inteiros
         Item->SubItems->Add(IntToStr(int(rel->array[n].adequada)));
         Item->SubItems->Add(IntToStr(int(rel->array[n].precaria)));
         Item->SubItems->Add(IntToStr(int(rel->array[n].critica)));
         if(check_semFluxo)
            {Item->SubItems->Add(IntToStr(int(rel->array[n].semfluxo)));}
         }
      else
         {//formata valores reais
         Item->SubItems->Add(DoubleToStr("%3.2f", rel->array[n].adequada));
         Item->SubItems->Add(DoubleToStr("%3.2f", rel->array[n].precaria));
         Item->SubItems->Add(DoubleToStr("%3.2f", rel->array[n].critica));
         if(check_semFluxo)
            {Item->SubItems->Add(DoubleToStr("%3.2f", rel->array[n].semfluxo));}
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::RadioGroupModoClick(TObject *Sender)
   {
   if (RadioGroupModo->ItemIndex == 0)
      {//seleção de redes por TipoRede: esconde FormSelRedePorHierarquia
      FormSelRedePorHierarquia->Visible   = false;
      //exibe FormSelRedePorTipoPorTipo
      FormSelRedePorTipo->Align   = alClient;
      FormSelRedePorTipo->Visible = true;
      }
   else
      {//seleção de redes por hieraquia: esconde FormSelRedePorTipo
      FormSelRedePorTipo->Visible   = false;
      //exibe FormSelRedePorTipoPorHierarquia
      FormSelRedePorHierarquia->Align   = alClient;
      FormSelRedePorHierarquia->Visible = true;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormRelFluxo::SelecionaArquivo(void)
   {
   //variáveis locais
   VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //verifica se o diretório SaveDialog já foi definido
   if (SaveDialog->InitialDir.IsEmpty()) SaveDialog->InitialDir = path->DirExporta();
   return(SaveDialog->Execute());
   }

//---------------------------------------------------------------------------
void __fastcall TFormRelFluxo::TimerTimer(TObject *Sender)
	{
   //seleciona novo patamar
   if (CBoxPatamar->ItemIndex < (CBoxPatamar->Items->Count - 1))
      {//seleciona próximo patamar
      CBoxPatamar->ItemIndex = CBoxPatamar->ItemIndex + 1;
      }
   else
      {//seleciona primeiro patamar
      CBoxPatamar->ItemIndex = 0;
      }
   //atualiza Chart01pat
   Chart01patInicia();
   }

//---------------------------------------------------------------------------
//eof








