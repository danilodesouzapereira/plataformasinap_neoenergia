// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormFluxoH3.h"
#include "..\Harmonicos\TImporta.h"
#include <Fontes\BD\VTBD.h>
#include <Fontes\BD\Rede\VTBDEqpto.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Complexo\Complexo.h>
#include <Fontes\Consulta\VTConsulta.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Edita\VTEdita.h>
#include <Fontes\Editor\VTEdt.h>
#include <Fontes\Flow\VTFlow.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <Fontes\Grafico\VTGrafico.h>
#include <Fontes\Legenda\VTVetval.h>
#include <Fontes\Mercado\VTMercado.h>
#include <Fontes\Patamar\VTPatamar.h>
#include <Fontes\Patamar\VTPatamares.h>
#include <Fontes\Planejamento\VTEstudo.h>
#include <Fontes\Planejamento\VTPlanejamento.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTFases.h>
#include <Fontes\Rede\VTLigacao.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\RedeFH\VTBarFH.h>
#include <Fontes\RedeFH\VTLigFH.h>
#include <Fontes\RedeFH\VTRedeFH.h>
#include <Fontes\Sinap\VTSinapChild.h>
#include <DLL_Inc\BD.h>
#include <DLL_Inc\Consulta.h>
#include <DLL_Inc\Editor.h>
#include <DLL_Inc\Flow.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Help.h>
#include <DLL_Inc\Legenda.h>
#include <DLL_Inc\RedeFH.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormFluxoH3::TFormFluxoH3(TForm *form_owner, VTApl *apl_owner, TWinControl *parent)
	: TFormEdt(form_owner, parent)
{
	// cria objeto Apl próprio
	apl = NewObjApl(this, apl_owner);
	// cria objetos necessários ao cálculo do fluxo harmônico
	apl->Add(redeFH = DLL_NewObjRedeFH(apl));
	apl->Add(DLL_NewObjConsulta(apl));
	apl->Add(DLL_NewObjFlow(apl));
	// cria listas
	lisCargaNL = new TList();
	lisEQP = new TList();
	lisVETVAL = new TList();
	lisVETSEL = new TList();
	// Cria ponteiro para o importador
	importa = new TImporta(apl);
	// posiciona o Form
	FormIniciaPosicao();
	// Carrega CNLs
	ActionDBLeituraExecute(NULL);
	Chart->Visible = false;
}

// ---------------------------------------------------------------------------
__fastcall TFormFluxoH3::~TFormFluxoH3(void)
{
	// variáveis locais
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// Grava BD com CNLs
	ActionDBEscritaExecute(NULL);
	// apaga eventual moldura
	Moldura();
	// retira todas CargaNL dda rede e cancela Undo/Redo
	edita->RetiraLisEqpto(lisCargaNL);
	edita->Clear();
	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// destrói objetos
	if (edt)
	{
		delete edt;
		edt = NULL;
	}
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
	if (importa)
	{
		delete importa;
		importa = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lisCargaNL)
	{
		delete lisCargaNL;
		lisCargaNL = NULL;
	}
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	if (lisVETSEL)
	{
		delete lisVETSEL;
		lisVETSEL = NULL;
	}
	// destrói lista e seus objetos
	if (lisVETVAL)
	{
		LimpaTList(lisVETVAL);
		delete lisVETVAL;
		lisVETVAL = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormFluxoH3::ActionAjudaExecute(TObject *Sender)
{
	// variaveis locais
	TForm *FormHelp;

	FormHelp = DLL_NewFormHelp(this, apl, "Fluxo_Harmônico");
	if (FormHelp)
		FormHelp->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormFluxoH3::ActionDBEscritaExecute(TObject *Sender)
{
	// Procura o ponteiro da base de dados no APL
	VTBD *bd = (VTBD*)apl->GetObject(__classid(VTBD));
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	TList *lisEstudo;
	VTEstudo *estudo;

	// verifica se há CargaNL novas ou alteradas
	if (lisCargaNL->Count <= 0)
		return;
	//
	if (!planejamento)
		return;
	if ((lisEstudo = planejamento->LisEstudo()) == NULL)
		return;
	if (lisEstudo->Count <= 0)
		return;
	if ((estudo = (VTEstudo*)lisEstudo->First()) == NULL)
		return;

	// OOPS: modificado em 20120611
	// Caso não tenha o ponteiro para a conexão, cria o objeto
	if (!bd)
	{
		// OOPS: se já está no apl não precisa eliminar o ponteiro
		bd = DLL_NewObjBD(apl);
		if (!bd)
			return;
		bd->Conecta();
	}

	// OOPS: Obra???
	bd->Eqpto->SalvaListaCargaNL(estudo, lisCargaNL, false);
	// OOPS: ao invés de passa para o APL, seria melhor colocar NULL e apagar o
	// ponteiro aqui???
	bd->CommitRede();
	bd->Desconecta();
}

// ---------------------------------------------------------------------------
void __fastcall TFormFluxoH3::ActionDBLeituraExecute(TObject *Sender)
{
	//
	// Procura o ponteiro da base de dados no APL
	TList *lisEstudo;
	VTEstudo *estudo;
	VTBD *bd = (VTBD*)apl->GetObject(__classid(VTBD));
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// Verificações iniciais
	if (!redes)
		return;
	if (!planejamento)
		return;
	if ((lisEstudo = planejamento->LisEstudo()) == NULL)
		return;
	if (lisEstudo->Count <= 0)
		return;
	if ((estudo = (VTEstudo*)lisEstudo->First()) == NULL)
		return;

	// OOPS: modificado em 20120611
	// Caso não tenha o ponteiro para a conexão, cria o objeto
	if (!bd)
	{
		// OOPS: se já está no apl não precisa eliminar o ponteiro
		bd = DLL_NewObjBD(apl);
		if (!bd)
			return;
		bd->Conecta();
	}

	// OOPS: Obra??? Passei false, porque havia escrito como false
	bd->Eqpto->LeListaCargaNL(estudo, redes->LisRede(), lisCargaNL, false);
	// OOPS: ao invés de passa para o APL, seria melhor colocar NULL e apagar o
	// ponteiro aqui???
	bd->Desconecta();
	// verifica se há Cargas NL
	lisCargaNL->Clear();
	redes->LisEqbar(lisCargaNL, eqptoCARGANL);
	// Atualiza gráfico
	if (lisCargaNL->Count > 0)
	{ // atualiza o gráfico
		grafico->AreaRedeAltera();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormFluxoH3::ActionEdtExecute(TObject *Sender)
{
	try
	{ // se necessário, cria um objeto TEdtSpy
		if (edt != NULL)
			delete edt;
		// cria EdtSpy p/ controle do mouse
		// edt = NewObjEdtMedidor(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
		edt = DLL_NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
		// atualiza indicação de mouse ativo e força botão Down
		AtualizaIndicacaoEditorOn(butEdt);
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormFluxoH3::ActionFluxoHarmonicoExecute(TObject *Sender)
{
	// variáveis locais
	VTBarra *barra;
	VTFlow *flow = (VTFlow*) apl->GetObject(__classid(VTFlow));
	VTRedes *redes = (VTRedes*) apl->GetObject(__classid(VTRedes));

	// habilitamensagem de erro do flow
	flow->AvisoEnabled = false;
	// habilita/desabilita relatorio do flow
	// flow->HabilitaRelatorio(true);  // Alterado HPS, 2012.01.26
//	flow->TipoRelatorio = relRESUMIDO;
	flow->TipoRelatorio = relCOMPLETO;  // Cf. enumRELATORIO em VTFlow.h
	// habilita/desabilita medida de tempo do flow
	flow->CronometroEnabled = false;
	// habilita/desabilita otimizações (uso de Tramos)
	flow->TramoEnabled = false; // Alterado HPS, 2012.01.26
	// seleciona modelo de carga
	flow->ModeloCarga = cargaAUTO;
	// seleciona algoritmo de cálculo de fluxo
	flow->TipoFluxo = fluxoHARMO3;
	// executa cálculo de fluxo único das redes
	flow->UpToDate = false;
	flow->CalculaRedeUnica(redes);
	// exibe resultados p/ uma barra com carga harmônica
	barra = (VTBarra*)(redes->LisBarra()->First());
	TViewIniciaBarra(barra);
	// exibe GBox
	GBoxEqpto->Visible = true;
	GBoxChart->Visible = true;
	Chart->Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormFluxoH3::ActionImportaArquivoExecute(TObject *Sender)
{
	// variáveis locais
	AnsiString   arq_cnl;
	TOpenDialog *dlgOpen;
	VTGrafico   *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTPath      *path    = (VTPath*)apl->GetObject(__classid(VTPath));

	try{//proteção
		if (!importa) return;
      //verifica se já existem CargaNL na rede
      if (RetiraCargaNL(true) == IDCANCEL) return;
		dlgOpen             = new TOpenDialog(NULL);
		dlgOpen->DefaultExt = "txt";
		dlgOpen->Filter     = "Arquivos texto (*.txt)|*.TXT";
		dlgOpen->Options.Clear();
		dlgOpen->Options << ofFileMustExist << ofHideReadOnly << ofNoChangeDir;
		dlgOpen->InitialDir = path->DirImporta();
		if (dlgOpen->Execute())
		{
			arq_cnl = dlgOpen->FileName;
			// OOPS! Testar o conteúdo do ponteiro retornado?
			importa->Executa(arq_cnl, lisCargaNL);
			// verifica se foram criadas CargaNL
			if (lisCargaNL->Count > 0)
			{ // atualiza o gráfico
				grafico->AreaRedeAltera();
			}
			else
			{
				Aviso("Não foi incluída nenhuma carga harmônica na rede");
			}
		}
		delete dlgOpen;
	}
	catch (Exception& e)
	{
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormFluxoH3::ActionRetiraCargaNLExecute(TObject *Sender)
   {
	// variáveis locais
	VTGrafico  *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   if (RetiraCargaNL(false) == IDYES)
      {//atualiza Grafico
      grafico->AreaRedeAltera();
      }
   }

// ---------------------------------------------------------------------------
void __fastcall TFormFluxoH3::ActionSelEqptoExecute(TObject *Sender)
{
	// variáveis locais
	bool visible = false;
	VTEqpto *eqpto;
	TList *lisEXT = (TList*)Sender;

	// proteção
	if (lisEXT == NULL)
		return;
	if (lisEXT->Count == 0)
		return;
	// determina primeiro Eqpto da lista
	if ((eqpto = (VTEqpto*)lisEXT->First()) == NULL)
		return;
	// verifica se Eqpto é uma Barra ou uma Ligacao
	if (eqpto->TipoBarra())
	{
		TViewIniciaBarra((VTBarra*)eqpto);
		visible = true;
	}
	else if (eqpto->TipoLigacao())
	{
		TViewIniciaLigacao((VTLigacao*)eqpto);
		visible = true;
	}
	// exibe/esconde GBox
	GBoxEqpto->Visible = visible;
	GBoxChart->Visible = visible;
}

// ---------------------------------------------------------------------------
void __fastcall TFormFluxoH3::ChartInicia(VTEqpto *eqpto, TList *lisVET)
{
	// variáveis locais
	double valor, val_min, val_max;
	VTVetval *vetval;
	VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

	// proteção
	if (eqpto == NULL)
	{
		Chart->Visible = false;
		return;
	}
	// reinicia TChart
	Chart->Title->Text->Clear();
	Chart->Title->Text->Add("Ano de referência: " + IntToStr(mercado->AnoAtual));
	// dtermina  valores máximo e mínimo para Barra ou Ligacao
	/*
	if (eqpto->Tipo() == eqptoBARRA)
	{ // barra: assume valores fixos p/ limite de tensão em pu
		val_min = 0;
		val_max = 1.5;
	}
	else
	{ // determina valor máximo de todos Vetval
	*/
		val_min = 0;
		val_max = 0;
		for (int n = 0; n < lisVET->Count; n++)
		{
			vetval = (VTVetval*)lisVET->Items[n];
			for (int i = 0; i < vetval->Len; i++)
			{
				val_max = max(val_max, vetval->Valor[i]);
			}
		}

		/*
		if(val_max < 0.01)
		{
			//Chart->LeftAxis->AxisValuesFormat = "#.# x10E-###";
			Chart->LeftAxis->AxisValuesFormat = "0.######";
			//Chart->LeftAxis->AxisValuesFormat = "#,##0.###################";
		}
		else
		{
			Chart->LeftAxis->AxisValuesFormat = "#,##0.###";
		}
		*/

        Chart->LeftAxis->AxisValuesFormat = "#,##0.###";

        // ajusta val_max
		val_max = double(int((val_max + 1.) * 1.1));
		//val_max = double(int((val_max) * 1.1));

	//}
	// proteção:
	// if (val_max <= val_min) val_max = val_min + 1.;
	// define valores mínimo e máximo
	Chart->LeftAxis->Minimum = 0.;
	Chart->LeftAxis->Maximum = val_max;
	Chart->LeftAxis->Minimum = val_min;
	// habilita somente as Series necessarias
	for (int n = 0; n < Chart->SeriesCount(); n++)
	{
		Chart->Series[n]->Active = (n < lisVET->Count);
		Chart->Series[n]->Visible = (n < lisVET->Count);
		Chart->Series[n]->ShowInLegend = (n < lisVET->Count);
	}
	// reinicia CLBoxSerie: exibe uma Serie p/ cada VetVal
	for (int n = 0; (n < lisVET->Count) && (n < Chart->SeriesCount()); n++)
	{
		vetval = (VTVetval*)lisVET->Items[n];
		ChartInsereCurva(vetval, Chart->Series[n]);
	}
	Chart->Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormFluxoH3::ChartInsereCurva(VTVetval *vetval, TChartSeries *serie)
{
	// variáveis locais
	AnsiString txt;
	double valor;
	VTPatamar *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// reinicia série
	serie->Clear();
	// serie->Active      = true;
	// serie->Visible     = true;
	serie->Title = vetval->DescUnid();
	// preenche a Série c/ os valores em todos patamares
	for (int np = 0; np < patamares->NumPatamar(); np++)
	{ 	// determina o Patamar
		patamar = patamares->Patamar[np];
		valor = vetval->Valor[np];
		// insere ponto na Serie
		(patamar->MinutoIni == 0) ? txt = IntToStr(patamar->HoraIni) : txt = "";
		serie->AddXY(np, valor, txt);
		// inclui ponto adicional p/ representar o fim do patamar(histograma)
		(patamar->MinutoFim == 0) ? txt = IntToStr(patamar->HoraFim) : txt = "";
		serie->AddXY(np + 1, valor, txt);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormFluxoH3::FormClose(TObject *Sender, TCloseAction &Action)
{
	// variáveis locais
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

	// esconde e desabilita o Form
	Hide();
	Enabled = false;
	// seleciona gráfico de topologia
	sinap_child->ActionTopo->Execute();
	// destrói o form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormFluxoH3::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
	// posiciona o Form sem alinhamento
	ActionAlignNoneExecute(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormFluxoH3::FormShow(TObject *Sender)
{
	// cria objeto EdtBusca
	ActionEdtExecute(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormFluxoH3::Moldura(VTEqpto *eqpto)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	grafico->Moldura(eqpto);
}

// ---------------------------------------------------------------------------
VTVetval* __fastcall TFormFluxoH3::MontaVetvalCorrenteEficaz(VTLigFH *ligFH, int ind_fase)
{
	// variáveis locais
	const char  *cod_fase[] = {"a", "b", "c"};
	AnsiString  txt;
	VTVetval    *vetval;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// cria novo objeto Vteval
	txt.sprintf("I%s_eficaz(pu)", cod_fase[ind_fase]);
	if ((vetval = DLL_NewObjVetval(-1, txt, patamares->NumPatamar())) == NULL)
		return (NULL);
	// preenche Vetval
	for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
	{
		vetval->Valor[ind_pat] = ligFH->Ief_amp[ind_pat][ind_fase];
	}
	return (vetval);
}

// ---------------------------------------------------------------------------
VTVetval* __fastcall TFormFluxoH3::MontaVetvalCorrenteHarmonica(VTLigFH *ligFH, int ind_fase,
	int ind_freq)
{
	// variáveis locais
	const char  *cod_fase[] = {"a", "b", "c"};
	AnsiString  txt;
	VTVetval    *vetval;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// cria novo objeto Vteval
	txt.sprintf("I%s_%dh(pu)", cod_fase[ind_fase], redeFH->OrdemHarmonica[ind_freq]);
	if ((vetval = DLL_NewObjVetval(-1, txt, patamares->NumPatamar())) == NULL)
		return (NULL);
	// preenche Vetval
	for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
	{
		vetval->Valor[ind_pat] = Abs(ligFH->Iamp[ind_pat][ind_fase][ind_freq]);
	}
	return (vetval);
}

// ---------------------------------------------------------------------------
VTVetval* __fastcall TFormFluxoH3::MontaVetvalTensaoEficaz(VTBarFH *barFH, int ind_fase)
{
	// variáveis locais
	const char  *cod_fase[] = {"an", "bn", "cn"};
	AnsiString  txt;
	VTVetval    *vetval;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// cria novo objeto Vteval
	txt.sprintf("V%s_eficaz(pu)", cod_fase[ind_fase]);
	if ((vetval = DLL_NewObjVetval(-1, txt, patamares->NumPatamar())) == NULL)
		return (NULL);
	// preenche Vetval
	for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
	{
		vetval->Valor[ind_pat] = barFH->Vef_pu[ind_pat][ind_fase];
	}

	//vetval->Valor[patamares->NumPatamar()+1] = Abs(barFH->DHT_Tensao[patamares->NumPatamar()+1][ind_fase]);

	return (vetval);
}

// ---------------------------------------------------------------------------
VTVetval* __fastcall TFormFluxoH3::MontaVetvalDHTensao(VTBarFH *barFH, int ind_fase, int ind_freq)
{
	// variáveis locais
	const char  *cod_fase[] = {"an", "bn", "cn"};
	AnsiString  txt;
	VTVetval    *vetval;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// cria novo objeto Vteval
	txt.sprintf("DTIh%s_%dh(%)", cod_fase[ind_fase], redeFH->OrdemHarmonica[ind_freq]);
	if ((vetval = DLL_NewObjVetval(-1, txt, patamares->NumPatamar())) == NULL)
		return (NULL);

	// preenche Vetval
	for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
	{
		vetval->Valor[ind_pat] = Abs(barFH->DH_Tensao[ind_pat][ind_fase][ind_freq]);
	}

	return (vetval);
}

// ---------------------------------------------------------------------------
VTVetval* __fastcall TFormFluxoH3::MontaVetvalDHTTensao(VTBarFH *barFH, int ind_fase)
{
	// variáveis locais
	const char  *cod_fase[] = {"an", "bn", "cn"};
	AnsiString  txt;
	VTVetval    *vetval;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// cria novo objeto Vteval
	txt.sprintf("DTTh%s_%Total(%)", cod_fase[ind_fase]);
	if ((vetval = DLL_NewObjVetval(-1, txt, patamares->NumPatamar())) == NULL)
		return (NULL);

	// preenche Vetval
	for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
	{
		vetval->Valor[ind_pat] = Abs(barFH->DHT_Tensao[ind_pat][ind_fase]);
	}

	return (vetval);
}

// ---------------------------------------------------------------------------
VTVetval* __fastcall TFormFluxoH3::MontaVetvalTensaoHarmonica(VTBarFH *barFH, int ind_fase,	int ind_freq)
{
	// variáveis locais
	const char  *cod_fase[] = {"an", "bn", "cn"};
	AnsiString  txt;
	VTVetval    *vetval;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// cria novo objeto Vteval
	txt.sprintf("V%s_%dh(pu)", cod_fase[ind_fase], redeFH->OrdemHarmonica[ind_freq]);
	if ((vetval = DLL_NewObjVetval(-1, txt, patamares->NumPatamar())) == NULL)
		return (NULL);
	// preenche Vetval
	for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
	{
		vetval->Valor[ind_pat] = Abs(barFH->Vpu[ind_pat][ind_fase][ind_freq]);
	}

	//vetval->Valor[patamares->NumPatamar()+1] = Abs(barFH->DH_Tensao[patamares->NumPatamar()+1][ind_fase][ind_freq]);

	return (vetval);
}

// ---------------------------------------------------------------------------
int __fastcall TFormFluxoH3::RetiraCargaNL(bool confirma)
   {
   //variáveis locais
   int         resp     = IDYES;
	VTEdita     *edita   = (VTEdita*)apl->GetObject(__classid(VTEdita));

   //verifica se há Cargas NL
   if (lisCargaNL->Count > 0)
      {//verifica se deve pedir confirmação do usuário
      if (confirma)
         {//pergunta se usuário quer retirar as Cargas existentes
         resp = Confirma("Já existem cargas harmônicas na rede.", "Deseja retirá-las ?");
         }
      if (resp == IDYES)
         {//retira  CargaNL da rede e cancela Undo/Redo
	      edita->RetiraLisEqpto(lisCargaNL);
         //reinicia lisCargaNL sem destruir seus objetos
         lisCargaNL->Clear();
         }
      }
   return(resp);
   }

// ---------------------------------------------------------------------------
void __fastcall TFormFluxoH3::TViewChange(TObject *Sender, TTreeNode *Node)
{
	// variáveis locais
	VTEqpto *eqpto = NULL;

	// verifica se houve alteração nos TreeNodes selecionados
	if (!TViewSelecionAlterado())
		return;
	// reinicia lisVETSEL com os Vetval selecionados, em ordem inversa
	lisVETSEL->Clear();
	for (int n = TView->SelectionCount - 1; n >= 0; n--)
	{
		Node = TView->Selections[n];
		if (Node->Level == NodeVetvalLevel)
		{
			lisVETSEL->Add(Node->Data);
			// verifica se já identificou o Eqpto
			if (eqpto == NULL)
			{
				while (Node->Level > 0)
					Node = Node->Parent;
				eqpto = (VTEqpto*)Node->Data;
			}
		}
	}
	// reinnicia TChart
	ChartInicia(eqpto, lisVETSEL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormFluxoH3::TViewIniciaBarra(VTBarra *barra)
{
	// variáveis locais
	TTreeNode *node_raiz, *node_fase, *node_atributo;
	TFase *fase;
	VTBarFH *barFH;
	VTVetval *vetval;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

	// bloqueia evento TView::OnChange
	TView->OnChange = NULL;
	// reinicia TView
	TView->Items->Clear();
	// reinicia lisVETVAL
	LimpaTList(lisVETVAL);
	// determina BarFH da Barra
	if ((barFH = redeFH->ExisteBarFH(barra)) == NULL)
		return;
	// exibe moldura
	Moldura(barra);
	// insere TreeNode com a Barra
	node_raiz = TView->Items->AddChildObject(NULL, barra->Codigo, barra);
	// insere TreeNode p/ fase
	for (int ind_fase = 0; ind_fase < (MAX_FASE - 1); ind_fase++)
	{
		fase = fases->IndexToFase[ind_fase];
		node_fase = TView->Items->AddChildObject(node_raiz, "Fase " + fase->codigo, fase);
		// cria objeto Vetval p/ tensão eficaz da fase
		lisVETVAL->Add(vetval = MontaVetvalTensaoEficaz(barFH, ind_fase));
		TViewInsereVetval(node_fase, vetval);

		// loop p/ todas harmônicas
		for (int ind_freq = 0; ind_freq < redeFH->NumHarmonicas; ind_freq++)
		{ // cria objeto Vetval p/ tensão harmônica da fase/frequencia
			lisVETVAL->Add(vetval = MontaVetvalTensaoHarmonica(barFH, ind_fase, ind_freq));
			TViewInsereVetval(node_fase, vetval);
		}


		//Não faz DH para 1
		for (int ind_freq = 1; ind_freq < redeFH->NumHarmonicas; ind_freq++)
		{ // cria objeto Vetval p/ DITH
			lisVETVAL->Add(vetval = MontaVetvalDHTensao(barFH, ind_fase, ind_freq));
			TViewInsereVetval(node_fase, vetval);
		}

		lisVETVAL->Add(vetval = MontaVetvalDHTTensao(barFH, ind_fase));
		TViewInsereVetval(node_fase, vetval);

		// expande TreeNode da fase
		node_fase->Expand(false);
	}
	// habilita evento TView::OnChange
	TView->OnChange = TViewChange;
	// expande TreeView, somente primeiro nível
	node_raiz->Expand(false);
	// seleciona TreeNode p/ iniciar Chart
	node_raiz->Selected = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormFluxoH3::TViewIniciaLigacao(VTLigacao *ligacao)
{
	// variáveis locais
	TTreeNode *node_raiz, *node_fase, *node_atributo;
	TFase *fase;
	VTLigFH *ligFH;
	VTVetval *vetval;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

	// bloqueia evento TView::OnChange
	TView->OnChange = NULL;
	// reinicia TView
	TView->Items->Clear();
	// reinicia lisVETVAL
	LimpaTList(lisVETVAL);
	// determina BarFH da Barra
	if ((ligFH = redeFH->ExisteLigFH(ligacao)) == NULL)
		return;
	// exibe moldura
	Moldura(ligacao);
	// insere TreeNode com a Ligacao
	node_raiz = TView->Items->AddChildObject(NULL, ligacao->Codigo, ligacao);
	// insere TreeNode p/ fase
	for (int ind_fase = 0; ind_fase < (MAX_FASE - 1); ind_fase++)
	{
		fase = fases->IndexToFase[ind_fase];
		node_fase = TView->Items->AddChildObject(node_raiz, "Fase " + fase->codigo, fase);
		// cria objeto Vetval p/ corrente eficaz da fase
		lisVETVAL->Add(vetval = MontaVetvalCorrenteEficaz(ligFH, ind_fase));
		TViewInsereVetval(node_fase, vetval);
		// loop p/ todas harmônicas
		for (int ind_freq = 0; ind_freq < redeFH->NumHarmonicas; ind_freq++)
		{ // cria objeto Vetval p/ corrente harmônica da fase/frequencia
			lisVETVAL->Add(vetval = MontaVetvalCorrenteHarmonica(ligFH, ind_fase, ind_freq));
			TViewInsereVetval(node_fase, vetval);
		}
		// expande TreeNode da fase
		node_fase->Expand(false);
	}
	// habilita evento TView::OnChange
	TView->OnChange = TViewChange;
	// expande TreeView, somente primeiro nível
	node_raiz->Expand(false);
	// seleciona TreeNode p/ iniciar Chart
	node_raiz->Selected = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormFluxoH3::TViewInsereVetval(TTreeNode *node_pai, VTVetval *vetval)
{
	// variáveis locais
	TTreeNode *node_vetval;
	VTPatamar *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// insere TreeNode p/ o Vetval
	node_vetval = TView->Items->AddChildObject(node_pai, vetval->DescUnid(), vetval);
	// insere TreeNodes p/ valores
	for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
	{
		patamar = patamares->Patamar[ind_pat];
		TView->Items->AddChild(node_vetval, patamar->Nome + " | " + DoubleToStr("%5.4f", vetval->Valor[ind_pat]));
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormFluxoH3::TViewSelecionAlterado(void)
{
	// variáveis locais
	int count = 0, nelem;
	TTreeNode *Node;

	// verifica se o conjunto de elementos selecionados foi alterado
	nelem = TView->SelectionCount;
	for (int n = 0; n < nelem; n++)
	{
		Node = TView->Selections[n];
		if (Node->Level == NodeVetvalLevel)
		{ // incrementa número de elementos selecionados
			count++;
			// verifica se o Vetval já existe em lisVETSEL
			if (lisVETSEL->IndexOf(Node->Data) < 0)
				return (true);
		}
	}
	// verifica se o número de elementos selecionados foi alterado
	return (count != lisVETSEL->Count);
}

// ---------------------------------------------------------------------------
// eof

