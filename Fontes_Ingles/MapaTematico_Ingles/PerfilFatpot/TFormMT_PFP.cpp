//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TFormMT_PFP.h"
#include "..\Basico\VTCfgCor.h"
#include "..\Tensao\TTemaTensao.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Const.h"
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
#include "..\..\Rede\VTResFlowLig.h"
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
//vari�veis static (Que droga, n�o encontrei outra solu��o!)
//static int static_ind_pat;

//---------------------------------------------------------------------------
__fastcall TFormMT_PFP::TFormMT_PFP(TComponent *owner, VTApl *apl_owner, TWinControl *parent)
   : TFormEdt(owner, parent)
   {
   //vari�veis locais
   VTMercado  *mercado = (VTMercado*)apl_owner->GetObject(__classid(VTMercado));

   //salva ano do mercado
   ano_mercado = mercado->AnoAtual;
	//cria objeto Apl pr�prio e seus objetos
	apl = NewObjApl(this, apl_owner);
   apl->Add(caminho = DLL_NewObjCaminho(apl));
	apl->Add(tema    = new TTemaTensao(apl));
	apl->Add(cfg_cor = NewObjCfgCor(apl));
	apl->Add(arvore  = DLL_NewObjArvore(apl));
	apl->Add(radial  = DLL_NewObjRadial(apl));
   apl->Add(tronco  = DLL_NewObjTronco(apl));
	//configura gr�fico em modo geo-referenciado
   //SelecionaModoGrafico(modoGEOREFERENCIADO);
   //cria listas
	lisEQP  = new TList();
   lisLIG  = new TList();
   //inicia estutura de controle de ordena��o dos LView
   LViewSort.Column         = NULL;
   LViewSort.OrdemCrescente = true;
   //inicia CBoxAno e CBoxPatamar
   CBoxAnoInicia();
   CBoxPatamarInicia();
   //exibe esconde op��es
   ActionShowBarras->Execute();
   ActionShowRedes->Execute();
   //verifica necessidade de rodar fluxo de pot�ncia
   if (! RecalculaFluxo(false)) Close();
	//inicia TView
	TViewInicia();
   }

//---------------------------------------------------------------------------
__fastcall TFormMT_PFP::~TFormMT_PFP(void)
   {
   //vari�veis locais
   VTGeral   *geral   = (VTGeral*)apl->GetObject(__classid(VTGeral));
   VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

   //desabilita Timer
   if (Timer->Enabled) ActionAnimacaoExecute(NULL);
   //desabilita mapa tem�tico
   HabilitaMapaTematico(false);
   //restaura modo gr�fico
   //SelecionaModoGrafico(modoESQUEMATICO);
   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //apaga eventual moldura
   Moldura(NULL);
   //cancela destaque dos Eqptos
	DestacaEqpto(NULL);
	//destr�i lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
	if (lisLIG) {delete lisLIG; lisLIG = NULL;}
   //destr�i objetos
	if (edt_busca) {delete edt_busca; edt_busca = NULL;}
	if (apl)       {delete apl;       apl       = NULL;}
   //restaura ano do mercado
   mercado->AnoAtual = ano_mercado;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::ActionAnimacaoExecute(TObject *Sender)
   {
   //atualiza lista de patamares habilitados
//   CBoxPatamarInicia();  //DVK 2013.07.17
   //habilita/desabilita Timer
	Timer->Enabled = ! Timer->Enabled;
	//atualiza Caption
	ActionAnimacao->Caption = (Timer->Enabled) ? "Parar" : "Iniciar";
	}

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::ActionEdtExecute(TObject *Sender)
   {
   //vari�veis locais
	try{//se necess�rio, cria um objeto TEdtBusca
      if (edt_busca != NULL) delete edt_busca;
      //cria EdtSpy p/ controle do mouse
		edt_busca = DLL_NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
      //atualiza indica��o de mouse ativo e for�a bot�o Down
      AtualizaIndicacaoEditorOn(butEdt);
      } catch (Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::ActionExportaExecute(TObject *Sender)
   { //DVK 2013.07.17
   //variaveis locais
   AnsiString  filename;
   VTPath      *path   = (VTPath*)apl->GetObject(__classid(VTPath));
   VTReport    *report;

	//cria objeto report
	report = DLL_NewObjReport(apl);
	//esconde form, para a janela Salvar Como n�o ficar por baixo
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
void __fastcall TFormMT_PFP::ActionHelpExecute(TObject *Sender)
   {//variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Mapa_Tem�tico_Perfil_de_Fator_de_Pot�ncia");
	if(formHelp) formHelp->Show();
}
//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::ActionSelEqptoExecute(TObject *Sender)
   {
   //vari�veis locais
   VTBarra *barra;
   VTRede  *rede;
   VTEqpto *eqpto;
   TList   *lisEXT = (TList*)Sender;

   //prote��o
   if (lisEXT == NULL) return;
   if (lisEXT->Count == 0) return;
   //determina Eqpto selecionado
   if ((eqpto = (VTEqpto*)lisEXT->First()) == NULL) return;
   //desabilita o Timer
   Timer->Enabled = false;
   //verifica se Eqpto � uma Barra ou Ligacao
   if      (eqpto->TipoBarra())   barra = (VTBarra*)eqpto;
   else if (eqpto->TipoEqbar())   barra = ((VTEqbar*)eqpto)->pbarra;
   else if (eqpto->TipoLigacao()) barra = ((VTLigacao*)eqpto)->pbarra1;
   else                           barra = NULL;
   //prote��o
   if (barra == NULL) return;
   //determina Rede da Barra
   if ((rede = RedeDaBarra(barra)) == NULL) return;
   //verifica se a primeira Barra est� selecionada
   if (dist.bar1 == NULL)
      {DefineBarraInicial(barra, rede);}
   else
      {//verifica se a segunda barra est� selecionada
      if (dist.bar2 != NULL)
         {DefineBarraInicial(barra, rede);}
      else if (dist.rede == rede)
         {DefineBarraFinal(barra);}
      else
         {DefineBarraInicial(barra, rede);}
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::ActionSelTroncoExecute(TObject *Sender)
   {
	//vari�veis locais
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
   //FKM - 2015.09.10 - /* Como s� funciona para redes primarias e secundarias...
   else
      {
      if((rede->TipoRede->Segmento != redePRI) && (rede->TipoRede->Segmento != redeSEC))
         {
         Aviso("Este m�dulo s� exibe o perfil para redes do tipo prim�ria e secund�ria");
         return;
         }
      node = TView->Selected;
      //verifica quem � o node pai de todos
      for(int i = 0; i < TView->Selected->Level + 1; i++)
         {
         if (node->Parent){node = node->Parent;}
         else
            {
            nome_node = node->Text;
            if(nome_node.AnsiCompareIC("Redes Isoladas") == 0)
               {
               Aviso("N�o existe o perfil de uma rede isolada");
               }
            }
         }
      }
   //FKM - 2015.09.10
   //determina Patamar selecionado
   if ((ind_pat = IndicePatamarSelecionado()) < 0) return;
//	//verifica se a rede � do tipo prim�rio
//	if (rede->TipoRede->Segmento == redePRI)
//		{//inicia objeto Radial
//      //radial->Inicia(rede->Redes);   //DVK 2013.07.17
//		if(! radial->Inicia(rede->Redes)) return;
//		//seleciona prim�rio da rede
//		if ((primario = radial->ExistePrimario(rede)) == NULL) return;
//		//inicia objeto tronco
//		//tronco->DefinePrimario(primario, static_ind_pat);
//		tronco->DefinePrimario(primario, ind_pat);
//		//obt�m lista de liga��es que s�o do tronco
//		lisLIG->Clear();
//		CopiaTList(tronco->LisLigacaoTronco(),lisLIG);
//      //redefine Barra inicial e final do caminho el�trico
//      dist.barra_ini = primario->BarraInicial;
//      dist.barra_fim = NULL;
//		}
//	//verifica se a rede � do tipo secund�rios
//	else if (rede->TipoRede->Segmento == redeSEC)
//		{//inicia objeto Radial
//      //radial->Inicia(rede->Redes);   //DVK 2013.07.17
//		if(! radial->Inicia(rede->Redes)) return;
//		//seleciona Secund�rio da rede
//		if ((secundario = radial->ExisteSecundario(rede)) == NULL) return;
//		//tronco->DefinePrimario(primario, static_ind_pat);
//		tronco->DefineSecundario(secundario, ind_pat);
//		//obt�m lista de liga��es que s�o do tronco
//		lisLIG->Clear();
//		CopiaTList(tronco->LisLigacaoTronco(),lisLIG);
//      //redefine Barra inicial e final do caminho el�trico
//      dist.barra_ini = secundario->BarraInicial;
//      dist.barra_fim = NULL;
//		}

   //FKM 2015.09.10 /* preparando para outros tipos de segmento ----------------
   //verifica se a rede � do tipo secund�rios
   if(! radial->Inicia(rede->Redes)) return;
   //age conforme o segmento
   switch (rede->TipoRede->Segmento)
      {
      case redePRI:
         //seleciona prim�rio da rede
         if ((primario = radial->ExistePrimario(rede)) == NULL) return;
         //inicia objeto tronco
         tronco->DefinePrimario(primario, ind_pat);
         //obt�m lista de liga��es que s�o do tronco
         lisLIG->Clear();
         CopiaTList(tronco->LisLigacaoTronco(),lisLIG);
         //redefine Barra inicial e final do caminho el�trico
         dist.barra_ini = primario->BarraInicial;
         dist.barra_fim = NULL;
         break;
      case redeSEC:
         //seleciona Secund�rio da rede
         if ((secundario = radial->ExisteSecundario(rede)) == NULL) return;
         //inicia objeto tronco
         tronco->DefineSecundario(secundario, ind_pat);
         //obt�m lista de liga��es que s�o do tronco
         lisLIG->Clear();
         CopiaTList(tronco->LisLigacaoTronco(),lisLIG);
         //redefine Barra inicial e final do caminho el�trico
         dist.barra_ini = secundario->BarraInicial;
         dist.barra_fim = NULL;
         break;
      default:
         return;
      }
      //FKM 2015.09.10 -------------------------------------------------------*/
	//atualiza dados da tela
	AtualizaTela();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::ActionShowBarrasExecute(TObject *Sender)
   {
   //exibe/esconde GroupBoxRedes
   GroupBoxBarras->Visible = ButShowBarras->Down;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::ActionShowRedesExecute(TObject *Sender)
   {
   //exibe/esconde GroupBoxRedes
   if (ButShowRedes->Down)
      {//exibe GroupBoxRedes
      GroupBoxRedes->Visible = true;
      //aumenta �rea do Form
      Width = Width + GroupBoxRedes->Width;
      }
   else
      {//esconde GroupBoxRedes
      GroupBoxRedes->Visible = false;
      //diminui �rea do Form
      Width = Width - GroupBoxRedes->Width;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::Atualiza(void)
   {
   //recalcula fluxo de pot�ncia
   RecalculaFluxo(true);
   //atualiza dados na tela
   AtualizaTela();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::AtualizaTela(void)
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
double __fastcall TFormMT_PFP::CalculaDistancia_m(VTLigacao *ligacao)
   {
   //vari�veis locais
   int    x1, y1, x2, y2;
   double dist_m;

   //verifica se a Ligacao � um trecho
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
void __fastcall TFormMT_PFP::CBoxAnoChange(TObject *Sender)
   {
   //atualiza a tela
   Atualiza();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::CBoxAnoInicia(void)
   {
   //vari�veis locais
   VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

   //reinicia CBoxPatamar c/ Patamares
   CBoxAno->Clear();
   for (int ano = mercado->AnoInicial; ano <= mercado->AnoFinal; ano++)
      {
      CBoxAno->Items->Add(IntToStr(ano));
      }
   //pr�-seleciona primeiro ano
   if (CBoxAno->Items->Count > 0) CBoxAno->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::CBoxPatamarChange(TObject *Sender)
   {
   //atualiza a tela
   AtualizaTela();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::CBoxPatamarInicia(void)
   {
   //vari�veis locais
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
   //pr�-seleciona primeiro patamar
   if (CBoxPatamar->Items->Count > 0) CBoxPatamar->ItemIndex = 0;
   delete lisPAT;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::ChartInicia(void)
   {
   //vari�veis locais
   int         ind_pat, valor_x;
   double      val_min, val_max, increment, dist_total_m;
   VTBarra     *barra;
   VTLigacao   *ligacao;

   //reinicia Series
   SeriesVan->Clear();
   SeriesVbn->Clear();
   SeriesVcn->Clear();
   //prote��o
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
   Chart->LeftAxis->Title->Caption = "Fator de Pot�ncia";
   if (CheckBoxDistancia->Checked)
      {Chart->BottomAxis->Title->Caption = "Dist�ncia (m)";}
   else
      {Chart->BottomAxis->Title->Caption = "N�mero de barras";}
   ///FKM -2015.09.08
   //inicia ditancia total
   dist_total_m = 0;
   //insere ponto p/ Barra inicial no Chart
   //ChartInsereBarra(dist.barra_ini, index=0, ind_pat);
   //insere ListItem p/ demais Barras
   //barra = dist.barra_ini;
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao    = (VTLigacao*)lisLIG->Items[nl];
      if (nl == 0)
         {//insere Fator de pot�ncia p/ barra inicial da Ligacao (DVK 2013.07.18: valor_x=0)
         ChartInsereFatorPotencia(ligacao, dist.barra_ini, valor_x=0, ind_pat);
         //define Barra
         barra = dist.barra_ini;
         }
      //acumula dist�ncia
      dist_total_m += CalculaDistancia_m(ligacao);
      //verifica se deve considerar dist�ncia no eixo x
      if (CheckBoxDistancia->Checked)
         {valor_x = dist_total_m;}
      else
         {valor_x = nl+1;}
      //seleciona Barra final da Ligacao
      barra   = (ligacao->pbarra1 == barra) ? ligacao->pbarra2 : ligacao->pbarra1;
      //insere Fator de pot�ncia p/ barra final da Ligacao
      ChartInsereFatorPotencia(ligacao, barra, valor_x, ind_pat);
      }
//   //determina valor m�nimo do eixo Y
//   val_min = SeriesVan->MinYValue();
//   val_min = min(val_min, SeriesVbn->MinYValue());
//   val_min = min(val_min, SeriesVcn->MinYValue());
//   //determina valor m�ximo do eixo Y
//   val_max = SeriesVan->MaxYValue();
//   val_max = max(val_max, SeriesVbn->MaxYValue());
//   val_max = max(val_max, SeriesVcn->MaxYValue());
//   //inclui uma margem adicional nos valores m�ximo e m�nimo
//   increment = 0.05;
//	(val_max >= 0) ? (val_max += increment) : (val_max -= increment);
//   (val_min >= 0) ? (val_min -= increment) : (val_min += increment);
//   //arredonda valores m�ximo e m�nimo
//   val_max = RoundTo(val_max, -2);
//   val_min = RoundTo(val_min, -2);
//   //configura valores m�nimo e m�ximo do eixo y
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
void __fastcall TFormMT_PFP::ChartInsereFatorPotencia(VTLigacao *ligacao, VTBarra *barra, int valor_x, int ind_pat)
   {
   //vari�veis locais
   int             ind_bar;
   double          fatpot;
   double          pf, qf; //FKM 2015.09.10
   int             vet_fase[MAX_FASE] = {faseA, faseB, faseC, faseN};
   TFastLineSeries *Series[]          = {SeriesVan, SeriesVbn, SeriesVcn};

   //determina indice da barra na Ligacao
   ind_bar = ligacao->IndiceBarra(barra);
//   for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
//      {//verifica se a Barra possui a fase
//      if (barra->resflow->Fases & vet_fase[ind_fase])
//         {
//         fatpot = ligacao->resflow->FPf_pu[ind_bar][ind_pat][ind_fase];
//         //insere ponto
//         Series[ind_fase]->AddXY(valor_x,   fatpot, "", Series[ind_fase]->Color);
//         //Series[ind_fase]->AddXY(valor_x+1, vfn_pu, "", Series[ind_fase]->Color);
//         }
//      }
//   }
      //FKM 2015.09.10 - Verifica se circula corrente
      for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
      {//verifica se a Barra possui a fase
      if (barra->resflow->Fases & vet_fase[ind_fase])
         {
         //verifica se existe "potencia" circulando na fase
         pf = ligacao->resflow->Pf_mw[ind_bar][ind_pat][ind_fase];
         qf = ligacao->resflow->Qf_mvar[ind_bar][ind_pat][ind_fase];
         if(!IsDoubleZero(pf) || !IsDoubleZero(qf))
            {//caso haja potencia circulando na fase
            fatpot = ligacao->resflow->FPf_pu[ind_bar][ind_pat][ind_fase];
            //insere ponto
            Series[ind_fase]->AddXY(valor_x,   fatpot, "", Series[ind_fase]->Color);
            }
         }
      }
      //FKM 2015.09.10 - Verifica se circula corrente
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::CheckBoxDistanciaClick(TObject *Sender)
   {
   //atualiza o Chart
   ChartInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::CheckBoxAnoClick(TObject *Sender)
{  //variaveis locais
   int num_redes;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //conta o numero de redes
   num_redes = redes->LisRede()->Count;
   if(num_redes > 150)
      {
      if(CheckBoxAno->Checked)
         {
         if (Confirma("H� " +IntToStr(num_redes) + " redes abertas. O c�lculo do fluxo a cada ano pode demorar,",
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
   //vari�veis locais
   VTBarra *bar1 = (VTBarra*)Item1;
   VTBarra *bar2 = (VTBarra*)Item2;
   double   bar1_vfn_pu = bar1->resflow->Vfn_pu_min[static_ind_pat];
   double   bar2_vfn_pu = bar2->resflow->Vfn_pu_min[static_ind_pat];

   return(CompareValue(bar1_vfn_pu, bar2_vfn_pu));
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::DefineBarraFinal(VTBarra *barra)
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
void __fastcall TFormMT_PFP::DefineBarraInicial(VTBarra *barra, VTRede *rede)
   {
   //salva primeria Barra selecionada
   dist.rede      = rede;
   dist.bar1      = barra;
   dist.bar2      = NULL;
   //cancela Barra inicial e Barra final
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
void __fastcall TFormMT_PFP::DestacaEqpto(TList *lisEQP)
   {
   //vari�veis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //atualiza moldura
   grafico->DestacaEqpto(lisEQP, clAqua, 1.5);  //clLime
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormMT_PT::ExibeBarraRede(VTRede *rede)
   {
   //vari�veis locais
   VTRede  *rede_sel = rede;
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //verifica se foi indicada uma Rede
   if (rede_sel == NULL)
      {//seleciona Rede com maior n�mero de Barras
      for (int n = 0; n < lisREDE->Count; n++)
         {
         rede = (VTRede*)lisREDE->Items[n];
         if (! rede->Carregada) continue;
         if (rede_sel == NULL) rede_sel = rede;
         //seleciona Rede com maior n�mero de Barras
         if (rede->NumeroDeBarras() > rede_sel->NumeroDeBarras()) rede_sel = rede;
         }
      }
   //prote��o
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
	//vari�veis locais
   VTPrimario  *primario;

	//inicia objeto Radial
	radial->Inicia(rede->Redes);
	//seleciona prim�rio da rede
	if ((primario = radial->ExistePrimario(rede)) == NULL) return;
	//inicia objeto tronco
	//tronco->DefinePrimario(primario, static_ind_pat);
	tronco->DefinePrimario(primario, CBoxPatamar->ItemIndex);
	//obt�m lista de liga��es que s�o do tronco
	lisLIG->Clear();
	CopiaTList(tronco->LisLigacaoTronco(),lisLIG);
   //redefine Barra inicial e final do caminho el�trico
   dist.barra_ini = primario->BarraInicial;
   dist.barra_fim = NULL;
	//atualiza dados da tela
	AtualizaTela();
   }
*/

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form sem alinhamento
   ActionAlignNoneExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destr�i o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   //inicia objeto EdtMedidor
   ActionEdtExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::HabilitaMapaTematico(bool enabled)
   {
   //vari�veis locais
   VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //habilita/desabilita apresenta��o do mapa tem�tico
   grafico->MapaTematicoEnabled = enabled;
   }

//---------------------------------------------------------------------------
int __fastcall TFormMT_PFP::IndicePatamarSelecionado(void)
   {
   //vari�veis locais
   VTPatamar *patamar;

   //verifica o patamar selecionado
   if (CBoxPatamar->ItemIndex < 0) return(-1);
   patamar = (VTPatamar*)(CBoxPatamar->Items->Objects[CBoxPatamar->ItemIndex]);
   return(patamar->Index);
   }

//---------------------------------------------------------------------------
TList* __fastcall TFormMT_PFP::LisBarra(TList *lisLIG)
   {
   //vari�veis locais
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
   //ordena��o da lista de barras bloqueada
   //IMPORTANTE: atualiza vari�vel da classe
   static_ind_pat = CBoxPatamar->ItemIndex;
   //ordena lista de barras
   lisEQP->Sort(ComparaTensaoBarra);
   */
   return(lisEQP);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::LViewBarraInicia(TList *lisLIG)
   {
   //vari�veis locais
   int        ind_pat;
   double     dist_total_m;
   VTBarra   *barra;
   VTLigacao *ligacao;

   //reinicia LView
   LView->Clear();
   //prote��o
   if (lisLIG->Count == 0) return;
   if((dist.barra_ini == NULL)&&(dist.barra_fim == NULL)) return; //DVK 2013.07.18
   //determina Patamar selecionado
   if ((ind_pat = IndicePatamarSelecionado()) < 0) return;
   //inicia distancia total em m
   dist_total_m = 0.;
   //loop p/ todas Ligacoes
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      if (nl == 0)
         {//insere Fator de pot�ncia p/ barra inicial da Ligacao
         LViewBarraInsereFatorPotencia(ligacao, dist.barra_ini, dist_total_m, ind_pat);
         //define Barra igual � Barra inicial da Ligacao
         barra = dist.barra_ini;
         }
      ligacao  = (VTLigacao*)lisLIG->Items[nl];
      //determina pr�xima Barra
      barra = (ligacao->pbarra1 == barra) ? ligacao->pbarra2 : ligacao->pbarra1;
      //determina dist�ncia total da barra � barra inicial
      dist_total_m += CalculaDistancia_m(ligacao);
      //insere ListItem para primeira Barra
      LViewBarraInsereFatorPotencia(ligacao, barra, dist_total_m, ind_pat);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::LViewBarraInsereFatorPotencia(VTLigacao *ligacao, VTBarra *barra, double dist_m, int ind_pat)
   {
   //vari�veis locais
   int        ind_bar;
   int        vet_fase[MAX_FASE] = {faseA, faseB, faseC, faseN};
   double     fatpot, tmpFatPot; //FKM - 2015.09.10
   double     pf, qf;            //FKM - 2015.09.10
   bool       fpIndutivo;        //FKM - 2015.09.10
   int        ind_fase_min;      //FKM - 2016.01.26
   TListItem *Item;

   //determina indice da barra na Ligacao
   ind_bar = ligacao->IndiceBarra(barra);
   //determina menor fp
   //fatpot = ligacao->resflow->FPf_pu_min[ind_bar][ind_pat];
   //FKM - 2015.09.10 /* resflow parece n�o verificar as fases presentes na ligacao
   fatpot = 2;
   for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
      {//verifica se a Barra possui a fase
      if (barra->resflow->Fases & vet_fase[ind_fase])
         {//verifica se existe "potencia" circulando na fase
         pf = ligacao->resflow->Pf_mw[ind_bar][ind_pat][ind_fase];
         qf = ligacao->resflow->Qf_mvar[ind_bar][ind_pat][ind_fase];
         //verifica se existe "potencia" circulando na fase
         if(!IsDoubleZero(pf) || !IsDoubleZero(qf))
            {//caso haja potencia circulando na fase verifica se � o minimo
            tmpFatPot = ligacao->resflow->FPf_pu[ind_bar][ind_pat][ind_fase];
            //caso o novo fator seja o minimo,
            if(tmpFatPot < fatpot)
               {//guarda o indice da fase
               ind_fase_min = ind_fase;
               //guarda o valor minimo
               fatpot = tmpFatPot;
               }
            }
         }
      }
   //apos descobrir qual a fase que possui o fator de potencia m�nimo
   //verifica se � indutivo ou capacitivo

   pf = ligacao->resflow->Pf_mw[ind_bar][ind_pat][ind_fase_min];
   qf = ligacao->resflow->Qf_mvar[ind_bar][ind_pat][ind_fase_min];
   //caso o sentido do fluxo seja negativo, inverte o sinal
   if(ligacao->resflow->SentidoP[ind_bar][ind_pat] == fluxoNEG)
      {//P e Q podem ter sentidos opostos...
      pf = pf*(-1.0);
      qf = qf*(-1.0);
      }
   if(qf > 0){fpIndutivo = true;}
   else      {fpIndutivo = false;}
   //FKM - 2016.01.26
   //insere Listitem
   Item = LView->Items->Add();
   Item->Data    = barra;
   Item->Caption = barra->Codigo;
   Item->SubItems->Add(ligacao->Codigo);    //DVK 2013.07.19
   Item->SubItems->Add(DoubleToStr("%2.1f", dist_m));
   //Item->SubItems->Add(DoubleToStr("%5.4f", fatpot));
   if (fatpot < 1){Item->SubItems->Add(DoubleToStr("%5.4f", fatpot));}//FKM - 2015.09.10
   else           {Item->SubItems->Add("-");}                         //FKM - 2015.09.10
   if (fpIndutivo){Item->SubItems->Add("Indutivo");}                  //FKM - 2015.09.10
   else           {Item->SubItems->Add("Capacitivo");}                //FKM - 2015.09.10
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::LViewColumnClick(TObject *Sender, TListColumn *Column)
   {
   //verifica se selecionou a mesma coluna
   if (LViewSort.Column == Column)
      {//inverte ordem
      LViewSort.OrdemCrescente  = ! LViewSort.OrdemCrescente;
      }
   else
      {//salva coluna usada p/ ordena��o
      LViewSort.Column         = Column;
      LViewSort.OrdemCrescente = true;
      }
   //for�a reordena��o
   ((TListView*)Sender)->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::LViewCompare(TObject *Sender,
                TListItem *Item1, TListItem *Item2, int Data, int &Compare)
   {
   //vari�veis locais
   int       index;
   double    valor1, valor2;

   //prote��o
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
      {//coluna com valor num�rico
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
void __fastcall TFormMT_PFP::LViewDblClick(TObject *Sender)
   {
   //vari�veis locais
   VTEqpto *eqpto;
   TListView *LView = (TListView*)Sender;

   //verifica se h� um ListItem selecionado
   if (LView->Selected == NULL) return;
   if ((eqpto = (VTEqpto*)LView->Selected->Data) == NULL) return;
   //destaca Eqpto c/ Zoom
   Zoom(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::LViewSelectItem(TObject *Sender, TListItem *Item, bool Selected)
   {
   //vari�veis locais
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
void __fastcall TFormMT_PFP::Moldura(TList *lisEQP)
   {
   //vari�veis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //atualiza moldura
   grafico->Moldura(lisEQP);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormMT_PFP::RecalculaFluxo(bool obrigatorio)
   {
   //vari�veis locais
   int       ano_carga = 0;
   VTFlow    *flow     = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTMercado *mercado  = (VTMercado*)apl->GetObject(__classid(VTMercado));
   VTRedes   *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));

   if (! obrigatorio)
      {
      if (flow->UpToDate) return(true);
      //pede p/ usu�rio confirmar
      if (Confirma("Este m�dulo necessita do c�lculo de fluxo de pot�ncia.",
                   "Confirma sua execu��o ?") != IDYES) return(false);
      }
   //redefine ano do Mercado
   mercado->AnoAtual = mercado->AnoInicial + CBoxAno->ItemIndex;
   //executa c�lculo de fluxo de pot�ncia
   flow->UpToDate = false;
   flow->CalculaPorRede(redes, ano_carga);
   return(true);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TFormMT_PFP::RedeDaBarra(VTBarra *barra)
   {
	//vari�veis locais
   VTRede  *rede;
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //verifica se j� existe a Rede de uma Barra selecionada
   if (dist.rede)
      {//verifica se a Rede cont�m a Barra indicada
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
VTRede* __fastcall TFormMT_PFP::RedeDaBarra(TList *lisLIG, VTBarra *barra)
   {
	//vari�veis locais
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
   //vari�veis locais
   VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

   //seleciono modo gr�fico indicado
   redegraf->ModoGrafico = modo;
   grafico->ModoGraficoAlterado();
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::TimerTimer(TObject *Sender)
   {
   //verifica se est� habilitada altera��o do patamar
   if (CheckBoxPatamar->Checked)
      {//seleciona novo patamar
      if (CBoxPatamar->ItemIndex < (CBoxPatamar->Items->Count - 1))
         {//seleciona pr�ximo patamar
         CBoxPatamar->ItemIndex = CBoxPatamar->ItemIndex + 1;
         }
      else
         {//seleciona primeiro patamar
         CBoxPatamar->ItemIndex = 0;
         //verifica se est� habilitada altera��o do ano
         if (CheckBoxAno->Checked)
            {//seleciona novo ano
            if (CBoxAno->ItemIndex < (CBoxAno->Items->Count - 1))
               {//seleciona pr�ximo ano
               CBoxAno->ItemIndex = CBoxAno->ItemIndex + 1;
               }
            else
               {
               CBoxAno->ItemIndex = 0;
               }
            //recalcula fluxo de pot�ncia
            RecalculaFluxo(true);
            }
         }
      }
   //verifica se est� habilitada a altera��o do ano
   else if (CheckBoxAno->Checked)
      {//seleciona novo ano
      if (CBoxAno->ItemIndex < (CBoxAno->Items->Count - 1))
         {//seleciona pr�ximo ano
         CBoxAno->ItemIndex = CBoxAno->ItemIndex + 1;
         }
      else
         {
         CBoxAno->ItemIndex = 0;
         }
      //recalcula fluxo de pot�ncia
      RecalculaFluxo(true);
      }
	//atualiza dados da tela
	AtualizaTela();
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormMT_PFP::TViewInicia(void)
	{
	//vari�veis locais
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
VTRede* __fastcall TFormMT_PFP::TViewRedeSelecionada(void)
   {
   //vari�veis locais
	AnsiString 	class_name;
   VTNode      *node;
	VTRede 		*rede;
   TList       *lisREDE;

   //verifica se h� um TreeNode selecionado
	if ((TView->Selected == NULL)||(TView->Selected->Data == NULL)) return(NULL);
	//verifica se objeto associado ao TreeNode � um VTNode
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
VTRede*    __fastcall TFormMT_PFP::TViewVerificaNodeRedes(AnsiString cod_rede, TList *lisREDE)
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
void __fastcall TFormMT_PFP::Zoom(VTEqpto *eqpto)
   {
   //vari�veis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //atualiza moldura
   grafico->SelecionaZoom(eqpto);
   }

//---------------------------------------------------------------------------
//eof




