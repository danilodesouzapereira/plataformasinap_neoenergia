// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TFormMT_Sinal.h"
// #include "TFormRel_Sinal.h"
#include "TFormLimites_Sinal.h"
#include "VTTema_Sinal.h"
#include "TFaixa_Sinal.h"
#include "..\..\Apl\TApl.h"
#include "..\..\Classes\VTClasse.h"
#include "..\..\Classes\VTClasses.h"
#include "..\..\Consulta\VTConsulta.h"
#include "..\..\Diretorio\VTPath.h"
#include "..\..\Geral\VTGeral.h"
#include "..\..\Geral\VTInfoset.h"
#include "..\..\Grafico\VTGrafico.h"
#include "..\..\Grafico\VTDraw.h"
#include "..\..\Licenca\VTEmpresa.h"
#include "..\..\Licenca\VTLicenca.h"
#include "..\..\Licenca\VTLicencas.h"
#include "..\..\Mercado\VTMercado.h"
#include "..\..\Patamar\VTPatamar.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Progresso\VTProgresso.h" //FKM
// #include "..\..\Radial\VTRadial.h"
#include "..\..\Radial\VTSubestacao.h"
#include "..\..\Rede\VTArea.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTTipoRede.h"
#include "..\..\Redegraf\VTCelula.h"
#include "..\..\Redegraf\VTCelulas.h"
#include "..\..\Redegraf\VTRedegraf.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Help.h"
#include "..\..\..\DLL_Inc\Consulta.h"
#include "..\..\..\DLL_Inc\Progresso.h"  //FKM
#include "..\..\..\DLL_Inc\Radial.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormMT_Sinal::TFormMT_Sinal(TComponent* Owner, VTApl *apl_owner, TWinControl *parent,
	TAction *ActionOnClose) : TForm(Owner)
{
	// variáveis locais
	int dim_celula_m;
	VTCelulas *celulas = (VTCelulas*)apl_owner->GetObject(__classid(VTCelulas));
	VTRedegraf *redegraf = (VTRedegraf*)apl_owner->GetObject(__classid(VTRedegraf));

	// marca o valor da resolucção
	resolucao = 11; // 3*30.87;
	// salva ActionOnClose
	this->ActionOnClose = ActionOnClose;
	// salva modo gráfico
	original.modo_grafico = redegraf->ModoGrafico;
	// insere Form em seu Parent
	if (parent)
		Parent = parent;
	// cria objeto Apl próprio
	apl = NewObjApl(this, apl_owner);
	// cria progresso
	progresso = DLL_NewObjProgresso(this, NULL); // FKM
	// cria demais objetos
	apl->Add(tema_Sinal = NewObjTema_Sinal(apl));
	listData = new TList();
	lines = new TStringList();
	campos = new TStringList();

	// verifica se foi definida ActionOnClose
	if (ActionOnClose != NULL)
	{ // desabilita opção exibir/esconder redes
		GBoxRedeVisible->Visible = false;
		// CheckBoxRedeVisible->OnClick = NULL;
		ExibeRedes(false);
	}
	// configura gráfico em modo geo-referenciado
	SelecionaModoGrafico(modoGEOREFERENCIADO);
	// inicia cores padrao
	ButCorConsMenor->SymbolColor = clWebDarkOrange;
	ButCorConsMaior->SymbolColor = clLime;
	CBoxAreaInicia();
	// desabilita o form
	HabilitaForm(false);
	AutoRange(false);
	latOff = 0;
	lonOff = 0;
}

// ---------------------------------------------------------------------------
__fastcall TFormMT_Sinal::~TFormMT_Sinal(void)
{ // variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	TList *listaEstruturas_Aux;
	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// torna a exibir as redes
	if (!CheckBoxRedeVisible->Checked)
		ExibeRedes(true);
	// desabilita mapa temático
	HabilitaMapaTematico(false);
	// restaura modo gráfico
	SelecionaModoGrafico(original.modo_grafico);
	// fecha FormRel_Sinal
	// if (FormLimites_Sinal) {delete FormLimites_Sinal; FormLimites_Sinal = NULL;}
	// destrói objetos
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lines)
	{
		delete lines;
		lines = NULL;
	}
	if (campos)
	{
		delete campos;
		campos = NULL;
	}
	if (progresso)
	{
		delete progresso;
		progresso = NULL;
	}
	// limpa lista de dados e lista de estruturas strImpCelPlanner
	if (listData)
	{
		for (int nl = (listData->Count - 1); nl >= 0; nl--)
		{
			listaEstruturas_Aux = (TList*)listData->Items[nl];
			LimpaTList(listaEstruturas_Aux);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::ActionCalculoExecute(TObject *Sender)
{
	// recalcula densidade de carga e atualiza mapa temático
	Atualiza();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::ActionEditaFaixasExecute(TObject *Sender)
{ // variaveis locais

	// cria FormLimites_DC
	FormLimites_Sinal = new TFormLimites_Sinal(this, apl, ActionRemontaMapaDegrade);
	// seta o auto range off
	cboxAutoRange->Checked = false;

	// exibe
	if (FormLimites_Sinal->ShowModal() == mrOk)
	{ // atualiza mapa temático
		// tema_Sinal->MontaFaixaSinal();
		// tema_Sinal->MontaGraficoPadrao();
		if (ActionRemontaMapaPadrao)
			ActionRemontaMapaPadrao->Execute();
		// atualiza LView
		Atualiza();
		// FKM-2015.10.01 atualiza cores das faixas de degradê
		// AtualizaDegrade();
	}
	// destroi FormLimites_DC
	delete FormLimites_Sinal;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::ActionFechaExecute(TObject *Sender)
{
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::ActionHelpExecute(TObject *Sender)
{
	// variaveis locais
	// TForm *formHelp;

	// formHelp = DLL_NewFormHelp(this, apl, "Mapa_Temático_Densidade_de_Carga");
	// if(formHelp) formHelp->Show();

}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::ActionImportarExecute(TObject *Sender)
{

	if (OpenDialog1->Execute())
	{
		progresso->Start(progDEFAULT);
		if (VerificaOffset())
		{
			ImportaTxt(OpenDialog1->Files->Strings[0]);
			//calcula resolucao minima
			CalculaDimMinCelula();
			//atualiza combo
			CBoxAreaInicia();
			// atualiza o grafico
			CalculaDensidadeMTSinal();
			tema_Sinal->FaixaValoresCelPlanner();
			tema_Sinal->MontaFaixaSinal();
			tema_Sinal->MontaGraficoPadrao();
			// tema_Sinal->MontaGraficoDegradeFaixa(ButCorConsMenor->SymbolColor,
			// ButCorConsMaior->SymbolColor,false);
			// habilita mapa temático
			HabilitaMapaTematico(true);
			// inicia listview
			LViewIniciaValor();
			// habilita o orm
			HabilitaForm(true);
		}
		progresso->Stop();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::ActionRelatorioExecute(TObject *Sender)
{
	//
	// //verifica se já existe FormRel_Sinal
	// if (FormRel_Sinal == NULL)
	// {//cria FormRel_Sinal
	// FormRel_Sinal = new TFormRel_Sinal(this, apl, Parent, ActionRemontaMapaPadrao);
	// FormRel_Sinal->Atualiza(RadioGroupGrafico->ItemIndex != 1);
	// }
	// //exibe
	// FormRel_Sinal->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::ActionRemontaMapaDegradeExecute(TObject *Sender)
{
	CalculaDensidadeMTSinal();
	tema_Sinal->MontaFaixaSinal();
	// tema_Sinal->MontaGraficoPadrao();
	tema_Sinal->MontaGraficoDegradeFaixa(ButCorConsMenor->SymbolColor,
		ButCorConsMaior->SymbolColor, false);
	// habilita mapa temático
	HabilitaMapaTematico(true);
	// inicia listview
	LViewIniciaValor();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::ActionRemontaMapaPadraoExecute(TObject *Sender)
{
	CalculaDensidadeMTSinal();
	tema_Sinal->MontaFaixaSinal();
	tema_Sinal->MontaGraficoPadrao();
	// habilita mapa temático
	HabilitaMapaTematico(true);
	// inicia listview
	LViewIniciaValor();
	// habilita mapa temático
	HabilitaMapaTematico(true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::AlteraOffset(void)
{ // variaveis locais
	int latOffGrau, latOffMinuto;
	double latOffSegundo;
	int lonOffGrau, lonOffMinuto;
	double lonOffSegundo;

	strImpCelPlanner *impCelPlanner;

	// protecao
	if (listSTR == NULL)
	{
		return;
	}
	// varre a lista alterando as estruturas
	for (int ns = 0; ns < listSTR->Count; ns++)
	{
		impCelPlanner = (strImpCelPlanner*)listSTR->Items[ns];
		// inicializa offset
		latOffGrau = latOffMinuto = lonOffGrau = lonOffMinuto = 0;
		latOffSegundo = lonOffSegundo = 0.0;
		// converte pra GMS
		OffsetSegundoToGMS(latOff - latOff_anterior, latOffGrau, latOffMinuto, latOffSegundo);
		OffsetSegundoToGMS(lonOff - lonOff_anterior, lonOffGrau, lonOffMinuto, lonOffSegundo);
		// atualiza valor
		impCelPlanner->lat.grau = impCelPlanner->lat.grau + latOffGrau;
		impCelPlanner->lat.minuto = impCelPlanner->lat.minuto + latOffMinuto;
		impCelPlanner->lat.segundo = impCelPlanner->lat.segundo + latOffSegundo;
		impCelPlanner->lon.grau = impCelPlanner->lon.grau + lonOffGrau;
		impCelPlanner->lon.minuto = impCelPlanner->lon.minuto + lonOffMinuto;
		impCelPlanner->lon.segundo = impCelPlanner->lon.segundo + lonOffSegundo;
	}
	latOff_anterior = latOff;
	lonOff_anterior = lonOff;
	// latOff = 0;
	// lonOff = 0;
	// editLatOff->Text = "0";
	// editLonOff->Text = "0";
	// converte as coordenadas em utm
	tema_Sinal->ConverteLatLonGMStoUTM(listSTR);
	// atualiza o grafico
	CalculaDensidadeMTSinal();
	tema_Sinal->FaixaValoresCelPlanner();
	tema_Sinal->MontaFaixaSinal();
	tema_Sinal->MontaGraficoPadrao();
	// habilita mapa temático
	HabilitaMapaTematico(true);
	// inicia listview
	LViewIniciaValor();
	// habilita o orm
	HabilitaForm(true);
}
// ---------------------------------------------------------------------------

void __fastcall TFormMT_Sinal::Atualiza(void)
{
	progresso->Start(progDEFAULT);
	// atualiza as cores do mapa temático degrade
	ActionRemontaMapaPadrao->Execute();
	progresso->Stop();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::AtualizaDegrade(void)
{
	progresso->Start(progDEFAULT);
	if (tema_Sinal->MontaGraficoDegradeFaixa(ButCorConsMenor->SymbolColor,
		ButCorConsMaior->SymbolColor, true))
	{
		ActionRemontaMapaDegrade->Execute();
		// atualiza LView
		LViewIniciaValor();
	}
	progresso->Stop();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::AutoRange(bool enabled)
{
	ButCorConsMaior->Visible = enabled;
	ButCorConsMenor->Visible = enabled;
	LabelSignalLevel->Visible = enabled;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::ButCorConsMaiorClick(TObject *Sender)
{
	AtualizaDegrade();
}
// ---------------------------------------------------------------------------

void __fastcall TFormMT_Sinal::ButCorConsMenorClick(TObject *Sender)
{
	AtualizaDegrade();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::CalculaDensidadeMTSinal(void)
{ // variáveis locais
	int dim_celula_m;
	VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
    VTLicencas *licencas =(VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca *licenca = licencas->ExisteLicenca();

	// determina resolução (dimensão da célula)
	dim_celula_m = StrToDouble(CBoxArea->Text);
	//age de acordo com a empresa
	switch (licenca->Empresa)
	{
		case COELBA:
			dim_celula_m = ceil(dim_celula_m *1.1112);
			break;
		case CELPE:
			dim_celula_m = ceil(dim_celula_m *1.1112);
			break;
		case COSERN:
			dim_celula_m = ceil(dim_celula_m *1.1112);
			break;
		default:
			break;
	}
	// redefine Grid de células
	celulas->DefineGridQuadrado(dim_celula_m);
	try
	{
		tema_Sinal->Executa(listSTR);
	}
	catch (Exception &e)
	{
		Aviso("Error");
	}
	// tema_Sinal->FaixaValoresUniforme();
	// tema_Sinal->MontaFaixaSinal();

}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::CalculaDimMinCelula(void)
{
	double menor_dist, dist;
	double x0;
	strImpCelPlanner *impCelPlannerN_0;
	strImpCelPlanner *impCelPlannerN_1;
	double valor_anterior;


	menor_dist = 10000000;
	if(listSTR->Count < 2)
	{
		resolucao = 50;
		return;
	}
	impCelPlannerN_0 = (strImpCelPlanner*) listSTR->First();
	x0 = impCelPlannerN_0->utm.x;
	for (int ns = 1; ns  < listSTR->Count; ns++)
	{
		impCelPlannerN_1 = (strImpCelPlanner*)listSTR->Items[ns];
        //se nao for igual
		if(!IsDoubleZero(impCelPlannerN_1->utm.x - x0))
		{
			dist = fabs(impCelPlannerN_1->utm.x - x0);
			if (dist < menor_dist)
			{
				menor_dist = dist;
			}
		}
	}

	resolucao = ceil(menor_dist);
}
// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::CBoxAreaChange(TObject *Sender)
{ // variáveis locais
	int dim_celula_m;
	VTCelulas *celulas   = (VTCelulas*)apl->GetObject(__classid(VTCelulas));


	progresso->Start(progDEFAULT);
	// determina resolução (dimensão da célula)
	dim_celula_m = StrToDouble(CBoxArea->Text);
	// redefine Grid de células
	celulas->DefineGridQuadrado(dim_celula_m);
	// recalcula densidade de carga e atualiza o gráfico
	Atualiza();
	// guarda resoluca
	resolucao = dim_celula_m;
	progresso->Stop();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::CBoxAreaInicia(void)
{
	// variáveis locais
	int dim_max, dim_min, exp, delta;
	int dim_ref[] =
	{10, 20, 30, 50, 100, 200, 300, 400, 500};
	VTArea *area;
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
    

	// determina área da rede
	area = redegraf->Area(redes);
	// determina dimensão da celula p/ que resulte um grid com 50x50 celulas
	dim_max = (max(area->Width(), area->Height())) / 50;
	// determina dimensão mínima da célula p/ que resulte em um grid de 80x80
	dim_min = (max(area->Width(), area->Height())) / 80;
	// converte dimensões p/ metros
	if ((dim_max *= 0.01) <= 0)
		dim_max = 1;
	if ((dim_min *= 0.01) <= 0)
		dim_min = 1;
	// arredonda dim_max p/ valor múltiplo de 10
	exp = Log10(double(dim_max));
	dim_max = RoundTo(double(dim_max), exp);
	// arredonda dim_min p/ valor múltiplo de 10
	exp = Log10(double(dim_min));
	dim_min = RoundTo(double(dim_min), exp);
	if (dim_max <= dim_min)
		dim_max++;
	// determina incremento
	delta = dim_max - dim_min;
	if (delta < 10)
	{
		delta = 1;
	}
	else
	{
		exp = Log10(double(delta));
		delta = RoundTo(double(delta), exp) / 5;
	}
	// preenche CBoxArea
	CBoxArea->Clear();
	//preenche com o valor da resolucao
	CBoxArea->Items->Add(DoubleToStr("%5.1f", resolucao));
	// preenche dimensões obrigatórias no início
	for (int n = 0; n < sizeof(dim_ref) / sizeof(int); n++)
	{
		if (dim_min > dim_ref[n])
		{
			if (dim_ref[n] > resolucao)
			{ // insere opção obrigatória de quadrícula
				CBoxArea->Items->Add(DoubleToStr("%5.1f", dim_ref[n]));
			}
		}
	}
	// preenche com as opções entre dim_min e dim_max
	for (int dim = dim_min; dim <= (dim_max + delta); dim += delta)
	{
		if (dim > resolucao)
		{
			CBoxArea->Items->Add(DoubleToStr("%5.1f", dim));
		}
	}
	// preenche dimensões obrigatórias no final
	for (int n = 0; n < sizeof(dim_ref) / sizeof(int); n++)
	{
		if (dim_max < dim_ref[n])
		{ // insere opção obrigatória de quadrícula
			CBoxArea->Items->Add(DoubleToStr("%5.1f", dim_ref[n]));
		}
	}
	// pré seleciona valor intermediário
	CBoxArea->ItemIndex = 4;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::cboxAutoRangeClick(TObject *Sender)
{
	progresso->Start(progDEFAULT);
	if (cboxAutoRange->Checked)
	{
		AutoRange(true);
		tema_Sinal->FaixaValoresUniforme();
		if (ActionRemontaMapaDegrade)
		{
			ActionRemontaMapaDegrade->Execute();
		}
	}
	else
	{
		if (ActionRemontaMapaPadrao)
		{
			ActionRemontaMapaPadrao->Execute();
		}
	}
	progresso->Stop();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::cboxDataChange(TObject *Sender)
{
	// muda o listSTR pro dado novo
	try
	{
		// proteção
		if (cboxData->ItemIndex < 0)
			return;
		listSTR = (TList*)cboxData->Items->Objects[cboxData->ItemIndex];
		progresso->Start(progDEFAULT);
		// tema_Sinal->MontaFaixaSinal();
		//atualiza combo
		CalculaDensidadeMTSinal();
		tema_Sinal->FaixaValoresCelPlanner();
		tema_Sinal->MontaFaixaSinal();
		tema_Sinal->MontaGraficoPadrao();
		tema_Sinal->MontaFaixaSinal();
		// tema_Sinal->MontaGraficoPadrao();
		// tema_Sinal->MontaGraficoDegradeFaixa(ButCorConsMenor->SymbolColor,
		// ButCorConsMaior->SymbolColor,false);
		// habilita mapa temático
		HabilitaMapaTematico(true);
		// inicia listview
		LViewIniciaValor();
		// habilita o orm
		HabilitaForm(true);
		// para aguarde
		progresso->Stop();
	}
	catch (Exception &e)
	{ // para aguarde
		progresso->Stop();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::CheckBoxRedeVisibleClick(TObject *Sender)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// exibe/esconde as redes
	ExibeRedes(CheckBoxRedeVisible->Checked);
	// atualiza Grafico
	grafico->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::editLatOffClick(TObject *Sender)
{
	editLatOff->SetFocus();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::editLatOffExit(TObject *Sender)
{
	// se tiver estruturas
	if (listSTR != NULL)
	{
		if (VerificaOffset())
		{ // se nao for igual
			if (!IsDoubleZero(latOff - latOff_anterior))
			{
				AlteraOffset();
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::editLatOffKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == VK_RETURN)
	{
		editLatOffExit(NULL);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormMT_Sinal::editLonOffClick(TObject *Sender)
{
	editLonOff->SetFocus();
}

// --------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::editLonOffExit(TObject *Sender)
{
	// se tiver estruturas
	if (listSTR != NULL)
	{
		if (VerificaOffset())
		{ // se nao for igual
			if (!IsDoubleZero(lonOff - lonOff_anterior))
			{
				AlteraOffset();
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::editLonOffKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == VK_RETURN)
	{
		editLonOffExit(NULL);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::ExibeRedes(bool visible)
{
	// variáveis locais
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();

	// exibe/esconde as redes
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		rede->Show[eqp_VISIBLE] = visible;
	}
}

// ---------------------------------------------------------------------------
int __fastcall TFormMT_Sinal::ExtraiCampos(AnsiString linha, AnsiString separadores)
{ // variáveis locais
	// AnsiString separadores = ";:";

	// reinicia campos e extrai os campos do string
	campos->Clear();
	ExtraiStrings(linha.Trim(), separadores, campos);
	return (campos->Count);
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::FormClose(TObject *Sender, TCloseAction &Action)
{
	// executa ActionOnClose
	if (ActionOnClose)
		ActionOnClose->Execute();
	// destrói o Form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
	// posiciona no canto superior esquerdo
	Top = 0;
	Left = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::FormShow(TObject *Sender)
{
	// posiciona o Form
	FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::HabilitaForm(bool enabled)
{
	tbEditRange->Enabled = enabled;
	gbRanges->Enabled = enabled;
	CBoxArea->Enabled = enabled;
	cboxAutoRange->Enabled = enabled;
	panelData->Enabled = enabled;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::HabilitaMapaTematico(bool enabled)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// habilita/desabilita apresentação do mapa temático
	grafico->MapaTematicoEnabled = enabled;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::ImportaTxt(AnsiString filename)
{
	AnsiString linha;
	strImpCelPlanner *impCelPlanner;
	double deltaX, deltaY;
	double auxX, auxY;
	// AnsiString        tipoDado;
	int latOffGrau, latOffMinuto;
	double latOffSegundo;
	int lonOffGrau, lonOffMinuto;
	double lonOffSegundo;

	// salva o offset vigente
	latOff_anterior = latOff;
	lonOff_anterior = lonOff;
	// inicializa offset
	latOffGrau = latOffMinuto = lonOffGrau = lonOffMinuto = 0;
	latOffSegundo = lonOffSegundo = 0.0;
	// converte pra GMS
	OffsetSegundoToGMS(latOff, latOffGrau, latOffMinuto, latOffSegundo);
	OffsetSegundoToGMS(lonOff, lonOffGrau, lonOffMinuto, lonOffSegundo);
	// começa progresso
	// progresso->Start(progDEFAULT);
	try
	{ // cria nova lista de importações
		listSTR = new TList();
		LimpaTList(listSTR);
		listSTR->Clear();
		// proteção: verifica se o arquivo existe
		if (FileExists(filename))
		{ // carrega o arquivo em  lines
			lines->LoadFromFile(filename);
		}
		// carrega o arquivo em  lines
		lines->LoadFromFile(filename);
		// verifica se o arquivo é do CelPlanner
		if (lines->Count > 0)
		{
			linha = lines->Strings[0];
			if (linha.AnsiCompare("[CelPlan DT PCB]") != 0)
			{
				Aviso("Arquivo em formatação inválida.");
				return;
			}
		}
		else
		{
			return;
		}
		// guarda o nome do tipo de dados
		tipoDado = lines->Strings[4];
		// extrai informações das linhas   (pula cabeçalho
		for (int n = 7; n < lines->Count; n++)
		{
			linha = lines->Strings[n];
			if (linha.IsEmpty())
			{
				break;
			}
			// extrai campos da linha
			if (ExtraiCampos(linha, "\t") < 9)
				continue;
			impCelPlanner = new strImpCelPlanner;
			impCelPlanner->titulo = tipoDado;
			impCelPlanner->lat.grau = StrToInt(campos->Strings[0].Trim()) + latOffGrau;
			impCelPlanner->lat.minuto = StrToInt(campos->Strings[1].Trim()) + latOffMinuto;
			// impCelPlanner->lat.segundo = StrToDouble(campos->Strings[2].Trim());
			impCelPlanner->lat.segundo = StrToDouble(campos->Strings[2].Trim()) + latOffSegundo;
			impCelPlanner->lat.hemisferio = campos->Strings[3].Trim();
			impCelPlanner->lon.grau = StrToInt(campos->Strings[4].Trim()) + lonOffGrau;
			impCelPlanner->lon.minuto = StrToInt(campos->Strings[5].Trim()) + lonOffMinuto;
			// impCelPlanner->lon.segundo = StrToDouble(campos->Strings[6].Trim());
			impCelPlanner->lon.segundo = StrToDouble(campos->Strings[6].Trim()) + lonOffSegundo;
			impCelPlanner->lon.hemisferio = campos->Strings[7].Trim();
			impCelPlanner->valor = StrToDouble(campos->Strings[8].Trim());

			listSTR->Add(impCelPlanner);
		}
		// converte as coordenadas em utm
		tema_Sinal->ConverteLatLonGMStoUTM(listSTR);
		// adiciona o tipo de dado ao combo
		cboxData->AddItem(tipoDado, listSTR);
		cboxData->ItemIndex = cboxData->Items->Count - 1;
		// adiciona a lista de dados
		listData->Add(listSTR);
		// atualiza os offsets
		// latOff = 0;
		// lonOff = 0;
		// editLatOff->Text = "0";
		// editLonOff->Text = "0";
	}
	catch (Exception &e)
	{
		if (listSTR)
		{
			LimpaTList(listSTR);
			delete listSTR;
			listSTR = NULL;
		}
	}
	// para aguarde
	// progresso->Stop();

}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::LViewAdvancedCustomDrawSubItem(TCustomListView *Sender,
	TListItem *Item, int SubItem, TCustomDrawState State, TCustomDrawStage Stage, bool &DefaultDraw)
{ // variáveis locais
	TFaixa_Sinal *faixa = (TFaixa_Sinal*)Item->Data;

	// proteção
	if (faixa == NULL)
		return;
	if (SubItem == (Item->SubItems->Count + 1))
	{
		LView->Canvas->Brush->Color = faixa->color;
		LView->Canvas->Brush->Style = bsSolid;
	}
	else
	{
		LView->Canvas->Brush->Color = clWindow;
		LView->Canvas->Brush->Style = bsSolid;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::LViewColumnClick(TObject *Sender, TListColumn *Column)
{
	// verifica se selecionou a mesma coluna
	if (LViewSort.Column == Column)
	{ // inverte ordem
		LViewSort.OrdemCrescente = !LViewSort.OrdemCrescente;
	}
	else
	{ // salva coluna usada p/ ordenação
		LViewSort.Column = Column;
		LViewSort.OrdemCrescente = true;
	}
	// força reordenação
	((TListView*)Sender)->AlphaSort();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::LViewCompare(TObject *Sender, TListItem *Item1, TListItem *Item2,
	int Data, int &Compare)
{
	// variáveis locais
	int index;
	double valor1, valor2;

	// proteção
	if (LViewSort.Column == NULL)
		return;
	// determina index p/ substrings
	index = LViewSort.Column->Index - 1;
	// verifica se coluna de Caption
	if (LViewSort.Column->Index == 0)
	{ // coluna de Caption
		Compare = CompareText(Item1->Caption, Item2->Caption);
	}
	else if (LViewSort.Column->Alignment == taLeftJustify)
	{ // coluna com texto
		Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
	}
	else
	{ // coluna com valor numérico
		valor1 = StrToDouble(Item1->SubItems->Strings[index]);
		valor2 = StrToDouble(Item2->SubItems->Strings[index]);
		if (valor1 < valor2)
			Compare = -1;
		else if (valor1 > valor2)
			Compare = 1;
		else
			Compare = 0;
	}
	// verifica se ordem crescente/decrescente
	if (!LViewSort.OrdemCrescente)
		Compare = -Compare;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::LViewDblClick(TObject *Sender)
{ // variáveis locais
	TFaixa_Sinal *faixa;

	// verifica se há um ListItem selecionado
	if (LView->Selected == NULL)
		return;
	// determina Faixa
	if ((faixa = (TFaixa_Sinal*)LView->Selected->Data) == NULL)
		return;
	// abre TDialogColor p/ usuário selecionar cor p/ a Faixa
	ColorDialog->Color = faixa->color;
	if (ColorDialog->Execute())
	{ // verifica se houve troca de cor
		if (ColorDialog->Color != faixa->color)
		{
			faixa->color = ColorDialog->Color;
			// atualiza mapa temático
			if (ActionRemontaMapaPadrao)
				ActionRemontaMapaPadrao->Execute();
			// atualiza LView e Chart
			LViewIniciaValor();
		}
	}
}

// ---------------------------------------------------------------------------
// void __fastcall TFormMT_Sinal::LViewCustomDraw(TCustomListView *Sender, const TRect &ARect,
// bool &DefaultDraw)
// {//variáveis locais
// TFaixa_Sinal *faixa = (TFaixa_Sinal*)Item->Data;
//
// //proteção
// if (faixa == NULL) return;
// if (SubItem == (Item->SubItems->Count + 1))
// {
// LView->Canvas->Brush->Color = faixa->color;
// LView->Canvas->Brush->Style = bsSolid;
// }
// else
// {
// LView->Canvas->Brush->Color = clWindow;
// LView->Canvas->Brush->Style = bsSolid;
// }
// }
// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::LViewIniciaValor(void)
{ // variáveis locais
	double valor;
	TListItem *Item;
	TFaixa_Sinal *faixa;
	TList *lisFAIXA = tema_Sinal->LisFaixa();

	// reincia lView
	LView->Clear();
	// altera caption das colunas com unidade
	LView->Columns->Items[1]->Caption = "Lim.Inf (dBm)";
	LView->Columns->Items[2]->Caption = "Lim.Sup (dBm)";
	// loop p/ todas Faixas
	for (int nf = 0; nf < lisFAIXA->Count; nf++)
	{
		faixa = (TFaixa_Sinal*)lisFAIXA->Items[nf];
		if (!faixa->enabled)
			break;
		// insere ListItem
		Item = LView->Items->Add();
		Item->Data = faixa;
		Item->Checked = faixa->visible;
		// Item->Caption = "";
		// faixa
		Item->Caption = IntToStr(nf + 1);
		// limite inferior e superior
		valor = faixa->sinal_min_db;
		Item->SubItems->Add(DoubleToStr("%2.1f", valor));
		// verifica se é a última faixa
		if (nf < (lisFAIXA->Count - 1))
		{
			valor = faixa->sinal_max_db;
			Item->SubItems->Add(DoubleToStr("%2.1f", valor));
		}
		else
		{
			valor = faixa->sinal_min_db;
			Item->SubItems->Add(DoubleToStr("> %4.3f", valor));
		}
	}
	// inclui mais uma linha com total de células e de demanda
	Item = LView->Items->Add();
	Item->Data = NULL;
	Item->Checked = false;
	Item->Caption = "";
	// faixa e limite inferior/superior
	Item->SubItems->Add("Total");
	Item->SubItems->Add("-");
	Item->SubItems->Add("-");
	// número de células (proporcional à área)
	Item->SubItems->Add(IntToStr(tema_Sinal->TotalCelula()));
	// percentual de células
	if (tema_Sinal->TotalCelula() == 0)
	{
		Item->SubItems->Add("0");
	}
	else
	{
		Item->SubItems->Add("100");
	}
}
// ---------------------------------------------------------------------------
// void __fastcall TFormMT_Sinal::PanelColorClick(TObject *Sender)
// {
// //exibe ColorDialog p/ usuário selecionar a cor
// ColorDialog->Color = PanelColor->Color;
// if (ColorDialog->Execute())
// {//verifica se houve troca de cor
// if (ColorDialog->Color != PanelColor->Color)
// {
// PanelColor->Color = ColorDialog->Color;
// //atualiza as cores do mapa temático degrade
// ActionRemontaMapaDegrade->Execute();
// }
// }
// }

// ---------------------------------------------------------------------------
// void __fastcall TFormMT_Sinal::RadioGroupGraficoClick(TObject *Sender)
// {
//
// if (RadioGroupGrafico->ItemIndex == 0)
// {//refaz cálculos e monta o gráfico
// ActionRemontaMapaPadrao->Execute();
// //atualiza FormRel_Sinal p/ exibir/esconder Checkboxes
// if (FormRel_Sinal) FormRel_Sinal->Atualiza(RadioGroupGrafico->ItemIndex == 0);
// }
// if (RadioGroupGrafico->ItemIndex == 1)
// {//apenas monta o gráfico indicado, não é necessário refazer os cálculos
// ActionRemontaMapaDegrade->Execute();
// }
// if (RadioGroupGrafico->ItemIndex == 2)
// {//refaz cálculos e monta o gráfico
// ActionRemontaPorSinal->Execute();
// //atualiza FormRel_Sinal p/ exibir/esconder Checkboxes
// if (FormRel_Sinal) FormRel_Sinal->Atualiza(RadioGroupGrafico->ItemIndex == 2);
// }
//
// //exibe/esconde parâmetros de patamares e anos
// //GroupBox2->Visible = !(RadioGroupGrafico->ItemIndex == 2);
// Panel2->Visible = !(RadioGroupGrafico->ItemIndex == 2);
// //GroupBox4->Visible = !(RadioGroupGrafico->ItemIndex == 2);
// //exibe/esconde PanelColor
// PanelColor->Visible = (RadioGroupGrafico->ItemIndex == 1);
// }

// ---------------------------------------------------------------------------
bool __fastcall TFormMT_Sinal::VerificaOffset(void)
{ // variaveis locais
	double valor;
	bool valido = false;

	try
	{
		latOff = StrToDouble(editLatOff->Text);
		lonOff = StrToDouble(editLonOff->Text);
		valido = true;
		// verifica se o numero é muito grande ou muito pequeno
		if ((latOff >= 324000) || (latOff <= -324000))
		{
			valido = false;
			throw;
		}
		if ((lonOff >= 324000) || (lonOff <= -324000))
		{
			valido = false;
			throw;
		}
		if ((latOff >= 3600) || (latOff <= -3600))
		{
			Aviso("High offset values might result in wrong location due UTM zones.");
		}
		if ((lonOff >= 3600) || (lonOff <= -3600))
		{
			Aviso("High offset values might result in wrong location due UTM zones.");
		}
	}
	catch (Exception &e)
	{
		valido = false;
		Aviso("Invalid offset value.");
		if (StrToDouble(editLatOff->Text, valor))
		{
			latOff = 0;
			editLatOff->Text = "0";
			editLatOff->SetFocus();
		}
		else if (StrToDouble(editLonOff->Text, valor))
		{
			lonOff = 0;
			editLonOff->Text = "0";
			editLonOff->SetFocus();
		}
		else
		{
			latOff = 0;
			lonOff = 0;
		}
	}

	return valido;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::OffsetSegundoToGMS(double offset_s, int &graus, int &minutos,
	double &segundos)
{ // variaveis locais
	double resto;

	graus = offset_s / 3600;
	resto = offset_s - graus * 3600;
	minutos = resto / 60;
	resto = resto - minutos * 60;
	segundos = resto;
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::panelLatOffClick(TObject *Sender)
{
	//
	editLatOff->SetFocus();
}
// ---------------------------------------------------------------------------

void __fastcall TFormMT_Sinal::panelLonOffClick(TObject *Sender)
{
	editLonOff->SetFocus();
}

// ---------------------------------------------------------------------------
void __fastcall TFormMT_Sinal::SelecionaModoGrafico(int modo)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	// verifica se está sendo alterado o modo gráfico
	if (redegraf->ModoGrafico != modo)
	{ // seleciono modo gráfico indicado
		redegraf->ModoGrafico = modo;
		grafico->ModoGraficoAlterado();
	}
}
// ---------------------------------------------------------------------------
// eof


