//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TFormMT_PT.h"
#include "..\Basico\VTCfgCor.h"
#include "..\Tensao\TTemaTensao.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Fases.h"
#include "..\..\Diretorio\VTPath.h"
#include "..\..\Distancia\VTCaminho.h"
#include "..\..\Editor\VTEdt.h"
#include "..\..\Flow\VTFlow.h"
#include "..\..\Geral\VTGeral.h"
#include "..\..\Geral\VTInfoset.h"
#include "..\..\Grafico\VTGrafico.h"
#include "..\..\Mercado\VTMercado.h"
#include "..\..\Patamar\VTPatamar.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Radial\VTArvore.h"
#include "..\..\Radial\VTNode.h"
#include "..\..\Radial\VTPrimario.h"
#include "..\..\Radial\VTSecundario.h"
#include "..\..\Radial\VTRadial.h"
#include "..\..\Radial\VTTronco.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTEqbar.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTNo.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTResFlowBar.h"
#include "..\..\Rede\VTTipoRede.h"
#include "..\..\Rede\VTTrecho.h"
#include "..\..\Report\VTReport.h"
#include "..\..\..\DLL_Inc\Editor.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Help.h"
#include "..\..\..\DLL_Inc\Distancia.h"
#include "..\..\..\DLL_Inc\Radial.h"
#include "..\..\..\DLL_Inc\Report.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
//variáveis static (Que droga, não encontrei outra solução!)
//static int static_ind_pat;

//---------------------------------------------------------------------------
__fastcall TFormMT_PT::TFormMT_PT(TComponent *owner, VTApl *apl_owner, TWinControl *parent)
   : TFormEdt(owner, parent)
   {
   //variáveis locais
   VTMercado  *mercado = (VTMercado*)apl_owner->GetObject(__classid(VTMercado));

   //salva ano do mercado
   ano_mercado = mercado->AnoAtual;
	//cria objeto Apl próprio e seus objetos
	apl = NewObjApl(this, apl_owner);
   apl->Add(caminho = DLL_NewObjCaminho(apl));
	apl->Add(tema    = new TTemaTensao(apl));
	apl->Add(cfg_cor = NewObjCfgCor(apl));
	apl->Add(arvore  = DLL_NewObjArvore(apl));
	apl->Add(radial  = DLL_NewObjRadial(apl));
   apl->Add(tronco  = DLL_NewObjTronco(apl));
	//configura gráfico em modo geo-referenciado
   //SelecionaModoGrafico(modoGEOREFERENCIADO);
   //cria listas
	lisEQP  = new TList();
   lisLIG  = new TList();
   //inicia estutura de controle de ordenação dos LView
   LViewSort.Column         = NULL;
   LViewSort.OrdemCrescente = true;
   //inicia CBoxAno e CBoxPatamar
   CBoxAnoInicia();
   CBoxPatamarInicia();
   //exibe esconde opções
   ActionShowBarras->Execute();
   ActionShowRedes->Execute();
   //verifica necessidade de rodar fluxo de potência
   if (! RecalculaFluxo(false)) Close();
   //reinicia mapa temático
   RecalculaMapaTematico();
   //seleciona Barras do tronco da primeira Rede
	//ExibeBarraRede();
	//inicia TView
	TViewInicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormMT_PT::~TFormMT_PT(void)
   {
   //variáveis locais
   VTGeral   *geral   = (VTGeral*)apl->GetObject(__classid(VTGeral));
   VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

   //desabilita Timer
   if (Timer->Enabled) ActionAnimacaoExecute(NULL);
   //desabilita mapa temático
   HabilitaMapaTematico(false);
   //restaura modo gráfico
   //SelecionaModoGrafico(modoESQUEMATICO);
   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //apaga eventual moldura
   Moldura(NULL);
   //cancela destaque dos Eqptos
	DestacaEqpto(NULL);
	//destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
	if (lisLIG) {delete lisLIG; lisLIG = NULL;}
   //destrói objetos
	if (edt_busca) {delete edt_busca; edt_busca = NULL;}
	if (apl)       {delete apl;       apl       = NULL;}
   //restaura ano do mercado
   mercado->AnoAtual = ano_mercado;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::ActionAnimacaoExecute(TObject *Sender)
   {
   //atualiza lista de patamares habilitados
//   CBoxPatamarInicia();  //DVK 2013.07.17
   //habilita/desabilita Timer
	Timer->Enabled = ! Timer->Enabled;
	//atualiza Caption
	ActionAnimacao->Caption = (Timer->Enabled) ? "Parar" : "Iniciar";
	}

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::ActionEdtExecute(TObject *Sender)
   {
	try{//se necessário, cria um objeto TEdtBusca
      if (edt_busca != NULL) delete edt_busca;
      //cria EdtSpy p/ controle do mouse
		edt_busca = DLL_NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
      //atualiza indicação de mouse ativo e força botão Down
      AtualizaIndicacaoEditorOn(butEdt);
      } catch (Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::ActionExportaExecute(TObject *Sender)
   { //DVK 2013.07.17
   //variaveis locais
   AnsiString  filename;
   VTPath      *path   = (VTPath*)apl->GetObject(__classid(VTPath));
   VTReport    *report;

	//cria objeto report
	report = DLL_NewObjReport(apl);
	//esconde form, para a janela Salvar Como não ficar por baixo
	Hide();
   //define pasta inicial
   SaveDialog->InitialDir = path->DirExporta();
   if(SaveDialog->Execute())
      {//exporta conteudo do listView
      filename = SaveDialog->FileName;     
      if(report->ArquivoTexto(filename,LView))
         Aviso("A tabela foi exportada corretamente.");
		else Erro("Erro ao exportar tabela.");
      }
   //destroi report
	if(report) {DLL_DeleteObjReport(report); report = NULL;}
   //reapresenta form
	Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::ActionHelpExecute(TObject *Sender)
{  //variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Mapa_Temático_Perfil_de_Tensão");
	if(formHelp) formHelp->Show();

}
//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::ActionMapaTematicoExecute(TObject *Sender)
   {
   //recalcula e exibe mapa temático
   if (ButMapaTematico->Down) RecalculaMapaTematico();
   else                       HabilitaMapaTematico(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::ActionSelEqptoExecute(TObject *Sender)
   {
   //variáveis locais
   VTBarra *barra;
   VTRede  *rede;
   VTEqpto *eqpto;
   TList   *lisEXT = (TList*)Sender;

   //proteção
   if (lisEXT == NULL) return;
   if (lisEXT->Count == 0) return;
   //determina Eqpto selecionado
   if ((eqpto = (VTEqpto*)lisEXT->First()) == NULL) return;
   //desabilita o Timer
   Timer->Enabled = false;
   //verifica se Eqpto é uma Barra ou Ligacao
   if      (eqpto->TipoBarra())   barra = (VTBarra*)eqpto;
   else if (eqpto->TipoEqbar())   barra = ((VTEqbar*)eqpto)->pbarra;
   else if (eqpto->TipoLigacao()) barra = ((VTLigacao*)eqpto)->pbarra1;
   else                           barra = NULL;
   //proteção
   if (barra == NULL) return;
   //determina Rede da Barra
   if ((rede = RedeDaBarra(barra)) == NULL) return;
   //verifica se a primeira Barra está selecionada
   if (dist.bar1 == NULL)
      {DefineBarraInicial(barra, rede);}
   else
      {//verifica se a segunda barra está selecionada
      if (dist.bar2 != NULL)
         {DefineBarraInicial(barra, rede);}
      else if (dist.rede == rede)
         {DefineBarraFinal(barra);}
      else
         {DefineBarraInicial(barra, rede);}
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::ActionSelTroncoExecute(TObject *Sender)
   {
	//variáveis locais
   int          ind_pat;
	AnsiString 	 class_name;
	VTRede 		 *rede;
   VTPrimario   *primario;
   VTSecundario *secundario;
   TTreeNode    *node;
   AnsiString   nome_node;

	//verifica se existe uma Rede associada ao TreeNode
   if ((rede = TViewRedeSelecionada()) == NULL)
		{
		Aviso("Selecione uma rede");
		return;
		}
   //FKM - 2015.09.10 - /* Como só funciona para redes primarias e secundarias...
   else
      {
      if((rede->TipoRede->Segmento != redePRI) && (rede->TipoRede->Segmento != redeSEC))
         {
         Aviso("Este módulo só exibe o perfil para redes do tipo primária e secundária");
         return;
         }
      node = TView->Selected;
      //verifica quem é o node pai de todos
      for(int i = 0; i < TView->Selected->Level + 1; i++)
         {
         if (node->Parent){node = node->Parent;}
         else
            {
            nome_node = node->Text;
            if(nome_node.AnsiCompareIC("Redes Isoladas") == 0)
               {
               Aviso("Não existe o perfil de uma rede isolada");
               return;
               }
            }
         }
      }
   //FKM - 2015.09.10
   //determina Patamar selecionado
   if ((ind_pat = IndicePatamarSelecionado()) < 0) return;
//	//verifica se a rede é do tipo primário
//	if (rede->TipoRede->Segmento == redePRI)
//		{//inicia objeto Radial
////      radial->Inicia(rede->Redes);   //DVK 2013.07.17
//		if(! radial->Inicia(rede->Redes)) return;
//		//seleciona primário da rede
//		if ((primario = radial->ExistePrimario(rede)) == NULL) return;
//		//inicia objeto tronco
//		//tronco->DefinePrimario(primario, static_ind_pat);
//		tronco->DefinePrimario(primario, ind_pat);
//		//obtém lista de ligações que são do tronco
//		lisLIG->Clear();
//		CopiaTList(tronco->LisLigacaoTronco(),lisLIG);
//      //redefine Barra inicial e final do caminho elétrico
//      dist.barra_ini = primario->BarraInicial;
//      dist.barra_fim = NULL;
//		}
//	//verifica se a rede é do tipo secundários
//	else if (rede->TipoRede->Segmento == redeSEC)
//		{//inicia objeto Radial
////      radial->Inicia(rede->Redes);   //DVK 2013.07.17
//		if(! radial->Inicia(rede->Redes)) return;
//		//seleciona Secundário da rede
//		if ((secundario = radial->ExisteSecundario(rede)) == NULL) return;
//		//tronco->DefinePrimario(primario, static_ind_pat);
//		tronco->DefineSecundario(secundario, ind_pat);
//		//obtém lista de ligações que são do tronco
//		lisLIG->Clear();
//		CopiaTList(tronco->LisLigacaoTronco(),lisLIG);
//      //redefine Barra inicial e final do caminho elétrico
//      dist.barra_ini = secundario->BarraInicial;
//      dist.barra_fim = NULL;
//		}
   //FKM 2015.09.10 /* preparando para outros tipos de segmento ----------------
   if(! radial->Inicia(rede->Redes)) return;
   //age conforme o segmento
   switch (rede->TipoRede->Segmento)
      {
      case redePRI:
         //seleciona primário da rede
         if ((primario = radial->ExistePrimario(rede)) == NULL) return;
         //inicia objeto tronco
         tronco->DefinePrimario(primario, ind_pat);
         //obtém lista de ligações que são do tronco
         lisLIG->Clear();
         CopiaTList(tronco->LisLigacaoTronco(),lisLIG);
         //redefine Barra inicial e final do caminho elétrico
         dist.barra_ini = primario->BarraInicial;
         dist.barra_fim = NULL;
         break;
      case redeSEC:
         //seleciona Secundário da rede
         if ((secundario = radial->ExisteSecundario(rede)) == NULL) return;
         //inicia objeto tronco
         tronco->DefineSecundario(secundario, ind_pat);
         //obtém lista de ligações que são do tronco
         lisLIG->Clear();
         CopiaTList(tronco->LisLigacaoTronco(),lisLIG);
         //redefine Barra inicial e final do caminho elétrico
         dist.barra_ini = secundario->BarraInicial;
         dist.barra_fim = NULL;
         break;
      default:
         return;
      }
      //FKM 2015.09.10 -------------------------------------------------------*/
	//reinicia mapa temático
	RecalculaMapaTematico();
	//atualiza dados da tela
	AtualizaTela();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::ActionShowBarrasExecute(TObject *Sender)
   {
   //exibe/esconde GroupBoxRedes
   GroupBoxBarras->Visible = ButShowBarras->Down;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::ActionShowRedesExecute(TObject *Sender)
   {
   //exibe/esconde GroupBoxRedes
   if (ButShowRedes->Down)
      {//exibe GroupBoxRedes
      GroupBoxRedes->Visible = true;
      //aumenta área do Form
      Width = Width + GroupBoxRedes->Width;
      }
   else
      {//esconde GroupBoxRedes
      GroupBoxRedes->Visible = false;
      //diminui área do Form
      Width = Width - GroupBoxRedes->Width;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::Atualiza(void)
   {
   //recalcula fluxo de potência
   RecalculaFluxo(true);
   //reinicia mapa temático
   RecalculaMapaTematico();
   //atualiza dados na tela
   AtualizaTela();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::AtualizaTela(void)
   {
   //reinicia LView
   LViewBarraInicia(lisLIG);
   //atualiza Chart
   ChartInicia();
   //exibe moldura c/ Eqpto inicial e final
   lisEQP->Clear();
   if (dist.barra_ini) lisEQP->Add(dist.barra_ini);
   if (dist.barra_fim) lisEQP->Add(dist.barra_fim);
   Moldura(lisEQP);
   //destaca ligacoes
   DestacaEqpto(lisLIG);
   }

//---------------------------------------------------------------------------
double __fastcall TFormMT_PT::CalculaDistancia_m(VTLigacao *ligacao)
   {
   //variáveis locais
   int    x1, y1, x2, y2;
   double dist_m;

   //verifica se a Ligacao é um trecho
   if (ligacao->Tipo() == eqptoTRECHO)
      {
      dist_m = ((VTTrecho*)ligacao)->Comprimento_m;
      }
   //verifica se as Barras tem coord.utm
   else if ((ligacao->pbarra1->CoordenadasUtm_cm(x1, y1))&&(ligacao->pbarra2->CoordenadasUtm_cm(x2, y2)))
      {
      dist_m  = 0.01 * Distancia_cm(x1, y1, x2, y2);
      }
   else
      {
      dist_m = 2;
      }
   return(dist_m);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::CBoxAnoChange(TObject *Sender)
   {
   //atualiza a tela
   Atualiza();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::CBoxAnoInicia(void)
   {
   //variáveis locais
   VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

   //reinicia CBoxPatamar c/ Patamares
   CBoxAno->Clear();
   for (int ano = mercado->AnoInicial; ano <= mercado->AnoFinal; ano++)
      {
      CBoxAno->Items->Add(IntToStr(ano));
      }
   //pré-seleciona primeiro ano
   if (CBoxAno->Items->Count > 0) CBoxAno->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::CBoxPatamarChange(TObject *Sender)
   {
   //reinicia mapa temático
   RecalculaMapaTematico();
   //atualiza a tela
   AtualizaTela();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::CBoxPatamarInicia(void)
   {
   //variáveis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //TList       *lisPAT    = patamares->LisPatamar();
   TList	   *lisPAT  = new TList();

   //guarda patamares habilitados
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
   delete lisPAT;
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::ChartInicia(void)
   {
   //variáveis locais
   bool        first = true;
   int         ind_pat, dist_m, index;
   double      vfn_pu, val_min, val_max;
   TFastLineSeries *Series[] = {SeriesVan, SeriesVbn, SeriesVcn};
   //TList           *lisBAR;
   VTBarra         *barra, *bar_ref;

   //reinicia Series
   SeriesVan->Clear();
   SeriesVbn->Clear();
   SeriesVcn->Clear();
   //proteção
   if (lisLIG->Count == 0) return;
   //determina Patamar selecionado
   if ((ind_pat = IndicePatamarSelecionado()) < 0) return;
   //obtém lista de Barras das Ligacoes do caminho elétrico selecionado
   lisBAR = LisBarra(lisLIG);
   //reinicia limites dos eixos
   if(Chart->LeftAxis->Maximum < Chart->LeftAxis->Minimum)
      {
      Chart->LeftAxis->Maximum = fabs(10 * Chart->LeftAxis->Minimum);
      }
   Chart->LeftAxis->Minimum = 0;
   Chart->LeftAxis->Maximum = 0;
   //determina Barra de referência p/ distãncia
   bar_ref = (VTBarra*)lisBAR->First();
   //insere ListItem p/ cada Barra
   dist_m = 0;
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
		barra = (VTBarra*)lisBAR->Items[nb];
      //verifica se deve considerar distãncia no eixo x
      if (CheckBoxDistancia->Checked)
         {index = dist_m;}
      else
         {index = nb;}
      //acumula distância entre barras e redefine barra de referência
      dist_m += Distancia_m(bar_ref->utm.x, bar_ref->utm.y, barra->utm.x, barra->utm.y);
      bar_ref = barra;
      for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
         {
         vfn_pu = Abs(barra->resflow->Vfn_pu[ind_pat][ind_fase]);
         //salva valor mínimo e máximo
         if (first) {first = false; val_min = val_max = vfn_pu;}
         val_min = min(val_min, vfn_pu);
         val_max = max(val_max, vfn_pu);
         //insere ponto
         Series[ind_fase]->AddXY(index,   vfn_pu, "", Series[ind_fase]->Color);
         //Series[ind_fase]->AddXY(index+1, vfn_pu, "", Series[ind_fase]->Color);
         }
      }
   //ajusta valores mínimo e máximo
   //val_max = RoundTo(val_max, -1);
   //val_min = RoundTo(val_min - 0.1, -1);
   val_max = RoundTo(val_max + 0.02, -2);
   val_min = RoundTo(val_min - 0.02, -2);
   //define valores mínimo e máximo do eixo
   Chart->LeftAxis->Maximum = val_max;
   Chart->LeftAxis->Minimum = val_min;
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::ChartInicia(void)
   {
   //variáveis locais
   int         ind_pat, index;
   double      val_min, val_max, increment, dist_total_m;
   VTBarra     *barra;
   VTLigacao   *ligacao;

   //reinicia Series
   SeriesVan->Clear();
   SeriesVbn->Clear();
   SeriesVcn->Clear();
   //proteção
   if (lisLIG->Count == 0) return;
   if((dist.barra_ini == NULL)&&(dist.barra_fim == NULL)) return; //DVK 2013.07.18
   //determina Patamar selecionado
   if ((ind_pat = IndicePatamarSelecionado()) < 0) return;
   //reinicia limites dos eixos
	if (Chart->LeftAxis->Maximum >= 0)
      {
      Chart->LeftAxis->Minimum  = 0.;
      Chart->LeftAxis->Maximum  = 0.;
      }
   else
      {
      Chart->LeftAxis->Maximum  = 0.;
      Chart->LeftAxis->Minimum  = 0.;
      }
   ///FKM -2015.09.08 /* Configura os titulos dos eixos
   if (CheckBoxDistancia->Checked)
      {Chart->BottomAxis->Title->Caption = "Distância (m)";}
   else
      {Chart->BottomAxis->Title->Caption = "Número de barras";}
   ///FKM -2015.09.08
   //inicia ditancia total
   dist_total_m = 0;
   //insere ponto p/ Barra inicial no Chart
   ChartInsereBarra(dist.barra_ini, index=0, ind_pat);
   //insere ListItem p/ demais Barras
   barra = dist.barra_ini;
   ///FKM -2015.09.08 /* Configura os titulos dos eixos
   Chart->LeftAxis->Title->Caption = "Tensão (pu)";
   if (CheckBoxDistancia->Checked)
      {Chart->BottomAxis->Title->Caption = "Distância (m)";}
   else
      {Chart->BottomAxis->Title->Caption = "Número de barras";}
   ///FKM -2015.09.08                                    */
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao    = (VTLigacao*)lisLIG->Items[nl];
      //determina próxima Barra
      barra = (ligacao->pbarra1 == barra) ? ligacao->pbarra2 : ligacao->pbarra1;
      //acumula distância
      dist_total_m += CalculaDistancia_m(ligacao);
      //verifica se deve considerar distãncia no eixo x
      if (CheckBoxDistancia->Checked)
         {index = dist_total_m;}
      else
         {index = nl+1;}
      ChartInsereBarra(barra, index, ind_pat);
      }
//   //determina valor mínimo do eixo Y
//   val_min = SeriesVan->MinYValue();
//   val_min = min(val_min, SeriesVbn->MinYValue());
//   val_min = min(val_min, SeriesVcn->MinYValue());
//   //determina valor máximo do eixo Y
//   val_max = SeriesVan->MaxYValue();
//   val_max = max(val_max, SeriesVbn->MaxYValue());
//   val_max = max(val_max, SeriesVcn->MaxYValue());
//   //inclui uma margem adicional nos valores máximo e mínimo
//   increment = 0.05;
//	(val_max >= 0) ? (val_max += increment) : (val_max -= increment);
//   (val_min >= 0) ? (val_min -= increment) : (val_min += increment);
//   //arredonda valores máximo e mínimo
//   val_max = RoundTo(val_max, -2);
//   val_min = RoundTo(val_min, -2);
//   //configura valores mínmo e máximo do eixo y
//	if (val_max > 0)
//      {
//		Chart->LeftAxis->Maximum   = val_max;
//      Chart->LeftAxis->Minimum   = val_min;
//		}
//   else
//      {
//      Chart->LeftAxis->Minimum   = val_min;
//      Chart->LeftAxis->Maximum   = val_max;
//		}
   //FKM automatiza o chart
   Chart->LeftAxis->Automatic = true;
   Chart->LeftAxis->Increment = 0.;
   //FKM automatiza o chart
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::ChartInsereBarra(VTBarra *barra, int index, int ind_pat)
   {
   //variáveis locais
   double          vfn_pu;
   int             vet_fase[MAX_FASE] = {faseA, faseB, faseC, faseN};
   TFastLineSeries *Series[]          = {SeriesVan, SeriesVbn, SeriesVcn};

   for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
      {//verifica se a Barra possui a fase
      if (barra->resflow->Fases & vet_fase[ind_fase])
         {
         vfn_pu = Abs(barra->resflow->Vfn_pu[ind_pat][ind_fase]);
         //insere ponto
         Series[ind_fase]->AddXY(index,   vfn_pu, "", Series[ind_fase]->Color);
         //Series[ind_fase]->AddXY(index+1, vfn_pu, "", Series[ind_fase]->Color);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::CheckBoxDistanciaClick(TObject *Sender)
   {
   //atualiza o Chart
   ChartInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::CheckBoxAnoClick(TObject *Sender)
{  //variaveis locais
   int num_redes;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //conta o numero de redes
   num_redes = redes->LisRede()->Count;
   if(num_redes > 150)
      {
      if(CheckBoxAno->Checked)
         {
         if (Confirma("Há " +IntToStr(num_redes) + " redes abertas. O cálculo do fluxo a cada ano pode demorar,",
                   "Deseja confirmar sua escolha ?") != IDYES)
            {
               CheckBoxAno->Checked = false;
            }
         }
      }

}
   /*
//---------------------------------------------------------------------------
int __fastcall TFormMT_PT::ComparaTensaoBarra(void *Item1, void *Item2)
   {
   //variáveis locais
   VTBarra *bar1 = (VTBarra*)Item1;
   VTBarra *bar2 = (VTBarra*)Item2;
   double   bar1_vfn_pu = bar1->resflow->Vfn_pu_min[static_ind_pat];
   double   bar2_vfn_pu = bar2->resflow->Vfn_pu_min[static_ind_pat];

   return(CompareValue(bar1_vfn_pu, bar2_vfn_pu));
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::DefineBarraFinal(VTBarra *barra)
   {
   //salva segunda Barra selecionada
   dist.bar2      = barra;
   //define Barra inicial e final
   dist.barra_ini = dist.bar1;
   dist.barra_fim = dist.bar2;
   //determina Ligacoes entre Barra inicial e final (passa em ordem inversa)
   lisLIG->Clear();
   caminho->Executa(dist.barra_fim, dist.barra_ini, lisLIG);
   //atualiza tela
   AtualizaTela();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::DefineBarraInicial(VTBarra *barra, VTRede *rede)
   {
   //salva primeria Barra selecionada
   dist.rede      = rede;
   dist.bar1      = barra;
   dist.bar2      = NULL;
   //cancela Barra inicial e final final
   dist.barra_ini = NULL;
   dist.barra_fim = NULL;
   //limpa lisLIG
   lisLIG->Clear();
   //verifica se a Rede possui barra inicial
   if ((barra = rede->BarraInicial()) != NULL)
      {//define Barra inicial e final
      dist.barra_ini = barra;
      dist.barra_fim = dist.bar1;
      //determina Ligacoes entre Barra inicial e final (passa em ordem inversa)
      caminho->Executa(dist.barra_fim, dist.barra_ini, lisLIG);
      }
   else
      {//define apenas Barra inicial
      dist.barra_ini = dist.bar1;
      }
   //atualiza tela
   AtualizaTela();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::DestacaEqpto(TList *lisEQP)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //atualiza moldura
   grafico->DestacaEqpto(lisEQP, clAqua, 1.5);  //clLime
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::ExibeBarraRede(VTRede *rede)
   {
   //variáveis locais
   VTRede  *rede_sel = rede;
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //verifica se foi indicada uma Rede
   if (rede_sel == NULL)
      {//seleciona Rede com maior número de Barras
      for (int n = 0; n < lisREDE->Count; n++)
         {
         rede = (VTRede*)lisREDE->Items[n];
         if (! rede->Carregada) continue;
         if (rede_sel == NULL) rede_sel = rede;
         //seleciona Rede com maior número de Barras
         if (rede->NumeroDeBarras() > rede_sel->NumeroDeBarras()) rede_sel = rede;
         }
      }
   //proteção
   if (rede_sel != NULL)
      {//seleciona todas as Ligacoes da Rede
      lisLIG->Clear();
      rede_sel->LisLigacao(lisLIG);
      //atualiza dados da tela
      AtualizaTela();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::ExibeBarraTronco(VTRede *rede)
   {
	//variáveis locais
   VTPrimario  *primario;

	//inicia objeto Radial
	radial->Inicia(rede->Redes);
	//seleciona primário da rede
	if ((primario = radial->ExistePrimario(rede)) == NULL) return;
	//inicia objeto tronco
	//tronco->DefinePrimario(primario, static_ind_pat);
	tronco->DefinePrimario(primario, CBoxPatamar->ItemIndex);
	//obtém lista de ligações que são do tronco
	lisLIG->Clear();
	CopiaTList(tronco->LisLigacaoTronco(),lisLIG);
   //redefine Barra inicial e final do caminho elétrico
   dist.barra_ini = primario->BarraInicial;
   dist.barra_fim = NULL;
	//reinicia mapa temático
	RecalculaMapaTematico();
	//atualiza dados da tela
	AtualizaTela();
   }
*/

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form sem alinhamento
   ActionAlignNoneExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   //inicia objeto EdtMedidor
   ActionEdtExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::HabilitaMapaTematico(bool enabled)
   {
   //variáveis locais
   VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //habilita/desabilita apresentação do mapa temático
   grafico->MapaTematicoEnabled = enabled;
   }

//---------------------------------------------------------------------------
int __fastcall TFormMT_PT::IndicePatamarSelecionado(void)
   {
   //variáveis locais
   VTPatamar *patamar;

   //verifica o patamar selecionado
   if (CBoxPatamar->ItemIndex < 0) return(-1);
   patamar = (VTPatamar*)(CBoxPatamar->Items->Objects[CBoxPatamar->ItemIndex]);
   return(patamar->Index);
   }

//---------------------------------------------------------------------------
TList* __fastcall TFormMT_PT::LisBarra(TList *lisLIG)
   {
   //variáveis locais
   VTBarra   *barra;
   VTLigacao *ligacao;

   //reinicia lisEQP com Barra inicial, se estiver definida
   lisEQP->Clear();
   if (dist.barra_ini) lisEQP->Add(dist.barra_ini);
   //insere Barras das Ligacoes selecionadas
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      ligacao->LisBarra(lisEQP);
      }
   /*
   //ordenação da lista de barras bloqueada
   //IMPORTANTE: atualiza variável da classe
   static_ind_pat = CBoxPatamar->ItemIndex;
   //ordena lista de barras
   lisEQP->Sort(ComparaTensaoBarra);
   */
   return(lisEQP);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::LViewBarraInicia(TList *lisLIG)
   {
   //variáveis locais
   int        ind_pat;
   double     vfn_pu_min, dist_m;
   TListItem *Item;
   TList     *lisBAR;
   VTBarra   *barra, *bar_ref;

   //reinicia LView
   LView->Clear();
   //proteção
   if (lisLIG->Count == 0) return;
   //determina Patamar selecionado
   if ((ind_pat = IndicePatamarSelecionado()) < 0) return;
   //obtém lista de Barras das Ligacoes do caminho elétrico selecionado
   lisBAR = LisBarra(lisLIG);
	//determina Barra de referência p/ distância
   bar_ref = (VTBarra*)lisBAR->First();
   //insere ListItem p/ cada Barra
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra      = (VTBarra*)lisBAR->Items[nb];
      dist_m     = 0.01 * Distancia_cm(bar_ref->utm.x, bar_ref->utm.y, barra->utm.x, barra->utm.y);
      vfn_pu_min = barra->resflow->Vfn_pu_min[ind_pat];
      //insere Listitem
      Item = LView->Items->Add();
      Item->Data    = barra;
      Item->Caption = barra->Codigo;
      Item->SubItems->Add(DoubleToStr("%2.1f", dist_m));
      Item->SubItems->Add(DoubleToStr("%5.4f", vfn_pu_min));
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::LViewBarraInicia(TList *lisLIG)
   {
   //variáveis locais
   int        ind_pat;
   double     dist_total_m;
   VTBarra   *barra;
   VTLigacao *ligacao;

   //reinicia LView
   LView->Clear();
   //proteção
   if (lisLIG->Count == 0) return;
   if((dist.barra_ini == NULL)&&(dist.barra_fim == NULL)) return; //DVK 2013.07.18
   //determina Patamar selecionado
   if ((ind_pat = IndicePatamarSelecionado()) < 0) return;
   //inicia distancia total em m
   dist_total_m = 0.;
   //insere ListItem para primeira Barra
   LViewBarraInsereBarra(dist.barra_ini, dist_total_m, ind_pat);
   //insere ListItem p/ demais Barras
   barra = dist.barra_ini;
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao  = (VTLigacao*)lisLIG->Items[nl];
      //determina próxima Barra
      barra = (ligacao->pbarra1 == barra) ? ligacao->pbarra2 : ligacao->pbarra1;
      //determina distância total da barra à barra inicial
      dist_total_m += CalculaDistancia_m(ligacao);
      //insere ListItem para primeira Barra
      LViewBarraInsereBarra(barra, dist_total_m, ind_pat);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::LViewBarraInsereBarra(VTBarra *barra, double dist_m, int ind_pat)
   {
   //variáveis locais
   double     vfn_pu_min;
   TListItem *Item;

   //determina menor tensão na Barra
   vfn_pu_min = barra->resflow->Vfn_pu_min[ind_pat];
   //insere Listitem
   Item = LView->Items->Add();
   Item->Data    = barra;
   Item->Caption = barra->Codigo;
   Item->SubItems->Add(DoubleToStr("%2.1f", dist_m));
   Item->SubItems->Add(DoubleToStr("%5.4f", vfn_pu_min));
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::LViewColumnClick(TObject *Sender, TListColumn *Column)
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
void __fastcall TFormMT_PT::LViewCompare(TObject *Sender,
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
void __fastcall TFormMT_PT::LViewDblClick(TObject *Sender)
   {
   //variáveis locais
   VTEqpto *eqpto;
   TListView *LView = (TListView*)Sender;

   //verifica se há um ListItem selecionado
   if (LView->Selected == NULL) return;
   if ((eqpto = (VTEqpto*)LView->Selected->Data) == NULL) return;
   //destaca Eqpto c/ Zoom
   Zoom(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::LViewSelectItem(TObject *Sender, TListItem *Item, bool Selected)
   {
   //variáveis locais
   VTEqpto *eqpto = (VTEqpto*)Item->Data;

   //exibe moldura na Barra selecionada
   if (Selected &&(eqpto != NULL))
      {//destaca Eqpto c/ Moldura
      lisEQP->Clear();
      if (dist.barra_ini) lisEQP->Add(dist.barra_ini);
      if (dist.barra_fim) lisEQP->Add(dist.barra_fim);
      lisEQP->Add(eqpto);
      Moldura(lisEQP);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::Moldura(TList *lisEQP)
   {
   //variáveis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //atualiza moldura
   grafico->Moldura(lisEQP);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormMT_PT::RecalculaFluxo(bool obrigatorio)
   {
   //variáveis locais
   int       ano_carga = 0;
   VTFlow    *flow     = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTMercado *mercado  = (VTMercado*)apl->GetObject(__classid(VTMercado));
   VTRedes   *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));

   if (! obrigatorio)
      {
      if (flow->UpToDate) return(true);
      //pede p/ usuário confirmar
      if (Confirma("Este módulo necessita do cálculo de fluxo de potência.",
                   "Confirma sua execução ?") != IDYES) return(false);
      }
   //redefine ano do Mercado
   mercado->AnoAtual = mercado->AnoInicial + CBoxAno->ItemIndex;
   //executa cálculo de fluxo de potência
   flow->UpToDate = false;
   flow->CalculaPorRede(redes, ano_carga);
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::RecalculaMapaTematico(void)
   {
   //variáveis locais
   int ind_pat;

   //determina Patamar selecionado
   if ((ind_pat = IndicePatamarSelecionado()) < 0) return;
   //verifica se a exibição do mapa está habilitada
   if (ButMapaTematico->Down)
      {//reinicia mapa temático
      tema->Reinicia(ind_pat);
      //atualiza grafico
      HabilitaMapaTematico(true);
      }
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TFormMT_PT::RedeDaBarra(VTBarra *barra)
   {
	//variáveis locais
   VTRede  *rede;
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //verifica se já existe a Rede de uma Barra selecionada
   if (dist.rede)
      {//verifica se a Rede contém a Barra indicada
      if (dist.rede->ExisteBarra(barra)) return(dist.rede);
      }
   //reinicia lisEQP
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (rede->ExisteBarra(barra)) return(rede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TFormMT_PT::RedeDaBarra(TList *lisLIG, VTBarra *barra)
   {
	//variáveis locais
   VTLigacao *ligacao;

   //reinicia lisEQP
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[n];
      if (ligacao->ConectaBarra(barra)) return(ligacao->rede);
      }
   return(NULL);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::SelecionaModoGrafico(int modo)
   {
   //variáveis locais
   VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

   //seleciono modo gráfico indicado
   redegraf->ModoGrafico = modo;
   grafico->ModoGraficoAlterado();
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::TimerTimer(TObject *Sender)
   {
   //verifica se está habilitada alteração do patamar
   if (CheckBoxPatamar->Checked)
      {//seleciona novo patamar
      if (CBoxPatamar->ItemIndex < (CBoxPatamar->Items->Count - 1))
         {//seleciona próximo patamar
         CBoxPatamar->ItemIndex = CBoxPatamar->ItemIndex + 1;
         }
      else
         {//seleciona primeiro patamar
         CBoxPatamar->ItemIndex = 0;
         //verifica se está habilitada alteração do ano
         if (CheckBoxAno->Checked)
            {//seleciona novo ano
            if (CBoxAno->ItemIndex < (CBoxAno->Items->Count - 1))
               {//seleciona próximo ano
               CBoxAno->ItemIndex = CBoxAno->ItemIndex + 1;
               }
            else
               {
               CBoxAno->ItemIndex = 0;
               }
            //recalcula fluxo de potência
            RecalculaFluxo(true);
            }
         }
      }
   //verifica se está habilitada a alteração do ano
   else if (CheckBoxAno->Checked)
      {//seleciona novo ano
      if (CBoxAno->ItemIndex < (CBoxAno->Items->Count - 1))
         {//seleciona próximo ano
         CBoxAno->ItemIndex = CBoxAno->ItemIndex + 1;
         }
      else
         {
         CBoxAno->ItemIndex = 0;
         }
      //recalcula fluxo de potência
      RecalculaFluxo(true);
      }
	//reinicia mapa temático
	RecalculaMapaTematico();
	//atualiza dados da tela
	AtualizaTela();
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::TViewInicia(void)
	{
	//variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//reinicia TView
   TView->Items->Clear();
	//inicia Arvore e o TreeView
	arvore->Inicia(redes);
	arvore->TreeViewInicia(TView);
	//expande o TView
	TView->FullExpand();
	}

//---------------------------------------------------------------------------
VTRede* __fastcall TFormMT_PT::TViewRedeSelecionada(void)
   {
   //variáveis locais
	AnsiString 	class_name;
   VTNode      *node;
	VTRede 		*rede;
   TList       *lisREDE;

   //verifica se há um TreeNode selecionado
	if ((TView->Selected == NULL)||(TView->Selected->Data == NULL)) return(NULL);
	//verifica se objeto associado ao TreeNode é um VTNode
	class_name = ((TObject*)(TView->Selected->Data))->ClassName();
	if (class_name.AnsiCompareIC("TNode") == 0)
		{//determina Rede associada ao TreeNode
      node = (VTNode*)TView->Selected->Data;
      lisREDE = node->LisObjectNode();
      if (lisREDE->Count == 1)
         {
         return((VTRede*)lisREDE->First());
         }
      //FKM - 2015.09.10 /* copiando da Dani
      if(lisREDE->Count > 1)
         {//localiza a rede selecionada na lista de redes do node
         rede = TViewVerificaNodeRedes(TView->Selected->Text, lisREDE);
         return(rede);
         }
      //FKM - 2015.09.10                     */
      }
	return(NULL);
   }
 //---------------------------------------------------------------------------
//FKM - 2015.09.10  /*copiando da dani
VTRede*    __fastcall TFormMT_PT::TViewVerificaNodeRedes(AnsiString cod_rede, TList *lisREDE)
{  //variaveis locais
   VTRede *rede;

   for(int nr = 0; nr < lisREDE->Count; nr++)
      {//verifica se existe uma rede com o cod_rede
      rede = (VTRede*)lisREDE->Items[nr];
      if(rede->Codigo.AnsiCompareIC(cod_rede) == 0) return(rede);
      }
   return(NULL);
}

//FKM - 2015.09.10     */
//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::Zoom(VTEqpto *eqpto)
   {
   //variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //atualiza moldura
   grafico->SelecionaZoom(eqpto);
   }

//---------------------------------------------------------------------------
//eof






