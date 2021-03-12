// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <algorithm>
#include <math.h>
#include "TFormEqpto.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Legenda\TAtributo.h"
#include "..\Legenda\VTLegenda.h"
#include "..\Legenda\VTDado.h"
#include "..\Legenda\VTGrupo.h"
#include "..\Legenda\VTVetval.h"
#include "..\Mercado\VTMercado.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormEqpto::TFormEqpto(TComponent* Owner, TWinControl *parent, VTApl *apl) : TForm(Owner)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	// insere este Form em seu parent
	if (parent)
	{
		Parent = parent;
		Align = alClient;
	}
	// cria objetos
	lisATR = new TList();
	lisVET = new TList();
	lisEQP = new TList();
	foto.strlis = new TStringList();
	Lines = new TStringList();
	// altera atributo p/ evitar flicker na exibição de imagens
	TabSheetFoto->DoubleBuffered = true;
	// pré-seleciona TabSheetAtributo
	PageControl->ActivePage = TabSheetDado;
	// pré-define atributo a ser mostrado no TChart p/ todos os eqptos
	cfg_bar.chart_atrib = bar_VFN_PU_MIN;
	cfg_can.chart_atrib = med_CANAL;
	cfg_cap.chart_atrib = cap_Q;
	cfg_caps.chart_atrib = lig_S;
	cfg_car.chart_atrib = car_S;
	cfg_chv.chart_atrib = lig_I_MAX;
	cfg_med.chart_atrib = med_CANAL;
	cfg_ger.chart_atrib = ger_S;
	cfg_reat.chart_atrib = reat_Q;
	cfg_regu.chart_atrib = lig_S;
	cfg_sup.chart_atrib = sup_S;
	cfg_trf.chart_atrib = lig_S;
	cfg_trf3.chart_atrib = lig_S;
	cfg_tre.chart_atrib = lig_S;
	cfg_zzz.chart_atrib = 0;
	cfg = &cfg_zzz;
}

// ---------------------------------------------------------------------------
__fastcall TFormEqpto::~TFormEqpto(void)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// apaga moldura se o eqpto não for nulo
	if (eqpto != NULL)
	{
		eqpto = NULL;
		grafico->Moldura(eqpto);
	}
	// destrói objetos
	if (grupo)
	{
		delete grupo;
		grupo = NULL;
	}
	if (foto.strlis)
	{
		delete foto.strlis;
		foto.strlis = NULL;
	}
	// destrói listas sem destruir seus obejtos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	if (lisVET)
	{
		delete lisVET;
		lisVET = NULL;
	}
	// destrói listas e seus obejtos
	if (lisATR)
	{
		LimpaTList(lisATR);
		delete lisATR;
		lisATR = NULL;
	}
	if (Lines)
	{
		delete Lines;
		Lines = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::ActionAtributoAllEqptoExecute(TObject *Sender)
{
	AtualizaAtributoAllEqpto();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::ActionAtributoChangedExecute(TObject *Sender)
{
	// variáveis locais
	TAtributo *atributo;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// proteção
	if (eqpto == NULL)
		return;
	// atualiza atributos do Eqpto
	for (int n = 0; n < CLBoxAtributo->Items->Count; n++)
	{
		atributo = (TAtributo*)CLBoxAtributo->Items->Objects[n];
		eqpto->Show[atributo->bit_index] = CLBoxAtributo->Checked[n];
	}
	// atualiza LView c/ valores dos atributos habilitados
	// TViewInicia();
	// atualiza gráfico
	if (grafico)
		grafico->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::ActionAtributoSelAllExecute(TObject *Sender)
{
	// proteção
	if (eqpto == NULL)
		return;
	// "check" todos os items do CLBoxAtributo
	for (int n = 0; n < CLBoxAtributo->Items->Count; n++)
	{
		CLBoxAtributo->Checked[n] = true;
	}
	// executa ActionAtributoChanged p
	ActionAtributoChangedExecute(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::ActionAtributoSelNoneExecute(TObject *Sender)
{
	// proteção
	if (eqpto == NULL)
		return;
	// cancela "check" todos os items do CLBoxAtributo
	for (int n = 0; n < CLBoxAtributo->Items->Count; n++)
	{
		CLBoxAtributo->Checked[n] = false;
	}
	// executa ActionAtributoChanged p
	ActionAtributoChangedExecute(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::ActionFotoFirstExecute(TObject *Sender)
{
	// proteção
	if (foto.strlis->Count == 0)
		return;
	if (foto.index == 0)
		return;
	// exibe primeira foto
	foto.index = 0;
	Image->Picture->LoadFromFile(foto.strlis->Strings[foto.index]);
	// habilita/desabilita Actions
	HabilitaActionFoto();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::ActionFotoLastExecute(TObject *Sender)
{
	// proteção
	if (foto.strlis->Count == 0)
		return;
	if (foto.index == foto.strlis->Count - 1)
		return;
	// exibe última foto
	foto.index = foto.strlis->Count - 1;
	Image->Picture->LoadFromFile(foto.strlis->Strings[foto.index]);
	// habilita/desabilita Actions
	HabilitaActionFoto();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::ActionFotoNextExecute(TObject *Sender)
{
	// proteção
	if (foto.strlis->Count == 0)
		return;
	if (foto.index >= foto.strlis->Count - 1)
		return;
	// exibe foto posterior
	foto.index++;
	Image->Picture->LoadFromFile(foto.strlis->Strings[foto.index]);
	// habilita/desabilita Actions
	HabilitaActionFoto();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::ActionFotoPrevExecute(TObject *Sender)
{
	// proteção
	if (foto.strlis->Count == 0)
		return;
	if (foto.index <= 0)
		return;
	// exibe foto anterior
	foto.index--;
	Image->Picture->LoadFromFile(foto.strlis->Strings[foto.index]);
	// habilita/desabilita Actions
	HabilitaActionFoto();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::Ativo(void)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// exibe moldura no Eqpto
	if ((grafico != NULL) && (eqpto != NULL))
	{
		grafico->Moldura(eqpto);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::Atualiza(void)
{
	// proteção
	if (eqpto == NULL)
		return;
	// atualiza TView c/ valores dos atributos habilitados
	TViewInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::AtualizaAtributoAllEqpto(void)
{
	// variáveis locais
	TAtributo *atributo;
	VTEqpto *um_eqpto;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// proteção
	if (eqpto == NULL)
		return;
	// obtém lista de todos Eqptos do mesmo tipo
	lisEQP->Clear();
	if (eqpto->Tipo() == eqptoREDUCAO)
	{
		redes->LisReducaoCarregada(lisEQP);
	}
	else
	{
		redes->LisEqpto(lisEQP, eqpto->Tipo());
	}
	for (int ne = 0; ne < lisEQP->Count; ne++)
	{
		um_eqpto = (VTEqpto*)lisEQP->Items[ne];
		// aplica a configuração de atributos a todos Eqptos do mesmo tipo
		for (int n = 0; n < CLBoxAtributo->Items->Count; n++)
		{
			atributo = (TAtributo*)CLBoxAtributo->Items->Objects[n];
			um_eqpto->Show[atributo->bit_index] = CLBoxAtributo->Checked[n];
		}
	}
	// atualiza gráfico
	if (grafico)
		grafico->Redraw();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::ChartInsereCurva(VTVetval *vetval, TChartSeries *serie, TColor cor,
	bool first)
{
	// variáveis locais
	AnsiString txt;
	double val, val_max, val_min;
	VTPatamar *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// NÃO atualiza o título
	// if (chart->Title->Text->Count == 0)
	// {chart->Title->Text->Add(vetval->DescUnid());}
	// determina os valores máximo e mínimo já definido p/ o TChart
	// val_min = chart->LeftAxis->Minimum ;
	// val_max = chart->LeftAxis->Maximum;
	// reinicia série
	serie->Clear();
	serie->Active = true;
	serie->Title = vetval->DescUnid();
	serie->SeriesColor = cor;
	// preenche a Série c/ os valores em todos patamares
	for (int np = 0; np < patamares->NumPatamar(); np++)
	{ // determina o Patamar
		patamar = patamares->Patamar[np];
		// determina valor no patamar
		if (vetval->unidade == "V") // ========================DVK 2011.11.3 inicio
		{
			// correção dos valores de tensão no gráfico
			val = (vetval->Valor[np]) * 1000.;
		}
		else
			val = vetval->Valor[np]; // ================DVK 2011.11.3 fim
		// insere ponto na Serie
		// serie->AddXY(np, val, "", cor);
		(patamar->MinutoIni == 0) ? txt = IntToStr(patamar->HoraIni) : txt = "";
		serie->AddXY(np, val, txt, cor);
		// inclui ponto adicional p/ representar o fim do patamar(histograma)
		// serie->AddXY(np+1, val, "", cor);
		(patamar->MinutoFim == 0) ? txt = IntToStr(patamar->HoraFim) : txt = "";
		serie->AddXY(np + 1, val, txt, cor);
		// determina valores máximo e mínimo
		if (np == 0)
		{
			val_max = val_min = val;
		}
		val_max = max(val_max, val);
		val_min = min(val_min, val);
	}
	// define valores mínimo e máximo do TChart
	if (!first)
	{
		val_max = max(chart->LeftAxis->Maximum, val_max);
		val_min = min(chart->LeftAxis->Minimum, val_min);
	}
	// atualiza eixo y
	if (val_max >= 0)
		chart->LeftAxis->Maximum = 1.1 * val_max;
	if (val_min < 0)
		chart->LeftAxis->Minimum = 1.1 * val_min;
	if (val_max < 0)
		chart->LeftAxis->Maximum = 0.9 * val_max;
	if (val_min >= 0)
		chart->LeftAxis->Minimum = 0.9 * val_min;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::CLBoxSerieDrawItem(TWinControl *Control, int Index, TRect &Rect,
	TOwnerDrawState State)
{
	// variáveis locais
	TCanvas *Canvas = CLBoxSerie->Canvas;
	TChartSeries *series;
	TColor color_ori;
	TBrushStyle style_ori;

	// limpa a área do item
	Canvas->FillRect(Rect);
	// determina objeto TChartSeries
	if ((series = (TChartSeries*)CLBoxSerie->Items->Objects[Index]) == NULL)
		return;
	// salva cor/style original
	color_ori = Canvas->Brush->Color;
	style_ori = Canvas->Brush->Style;
	// define área do retângulo p/ a cor da série
	TRect r = Rect;
	r.Top += 2;
	r.Bottom -= 2;
	r.Right = r.Left + (r.Bottom - r.Top) + 2;
	// escreve nome da Serie
	Canvas->TextOut(r.Right + 1, Rect.Top, series->Title);
	// desenha retângulo c/ a cor
	Canvas->Brush->Color = series->SeriesColor;
	Canvas->Brush->Style = bsSolid;
	Canvas->FillRect(r); // desenha o retângulo da cor
	// restaura cor/style original
	Canvas->Brush->Color = color_ori;
	Canvas->Brush->Style = style_ori;
	/*
	 //define área para apresentação da cor, após o nome
	 TRect r = Rect;
	 r.Top++;  r.Bottom--; r.Right--;
	 //r.Left = r.Right -(2*(r.Bottom - r.Top));
	 r.Left = r.Right -(r.Bottom - r.Top);
	 Canvas->Brush->Color = series->SeriesColor;
	 Canvas->Brush->Style = bsSolid;
	 Canvas->FillRect(r);  //desenha o retângulo da cor
	 //restaura cor/style original
	 Canvas->Brush->Color = color_ori;
	 Canvas->Brush->Style = style_ori;
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::CLBoxSerieClickCheck(TObject *Sender)
{
	// variáveis locais
	TChartSeries *serie;

	// exibe/esconde Serie
	for (int n = 0; n < CLBoxSerie->Count; n++)
	{
		serie = (TChartSeries*)CLBoxSerie->Items->Objects[n];
		serie->Active = CLBoxSerie->Checked[n];
	}
}

// ---------------------------------------------------------------------------
TColor __fastcall TFormEqpto::DefineCorSerie(int n)
{
	TColor color[] =
	{clBlue, clRed, clLime, clYellow, clFuchsia};

	// proteção
	n = (n % (sizeof(color) / sizeof(TColor)));
	return (color[n]);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::DefineEqpto(VTEqpto *eqpto)
{
	// variáveis locais
	TAtributo *atributo;
	VTLegenda *legenda = (VTLegenda*)apl->GetObject(__classid(VTLegenda));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// proteção: verifica se é o mesmo Eqpto já selecionado
	if (this->eqpto == eqpto)
		return;
	// salva ponteiro p/ objeto
	this->eqpto = eqpto;
	// exibe moldura no Eqpto
	Ativo();
	// reinicia lisVET
	lisVET->Clear();
	// reinicia listas de Atributos e Dados do eqpto anterior
	LimpaTList(lisATR);
	// determina atributos do Eqpto a serem exibidos no TChart
	switch (eqpto->Tipo())
	{
	case eqptoBARRA:
		{
			cfg = &cfg_bar;
			break;
		}
	case eqptoCANAL:
		{
			cfg = &cfg_can;
			break;
		}
	case eqptoCAPACITOR:
		{
			cfg = &cfg_cap;
			break;
		}
	case eqptoCAPSERIE:
		{
			cfg = &cfg_caps;
			break;
		}
	case eqptoCARGA:
		{
			cfg = &cfg_car;
			break;
		}
	case eqptoCHAVE:
		{
			cfg = &cfg_chv;
			break;
		}
	case eqptoGERADOR:
		{
			cfg = &cfg_ger;
			break;
		}
	case eqptoMEDIDOR:
		{
			cfg = &cfg_med;
			break;
		}
	case eqptoREATOR:
		{
			cfg = &cfg_reat;
			break;
		}
	case eqptoREDUCAO:
		{
			cfg = &cfg_redu;
			break;
		}
	case eqptoREGULADOR:
		{
			cfg = &cfg_regu;
			break;
		}
	case eqptoSUPRIMENTO:
		{
			cfg = &cfg_sup;
			break;
		}
	case eqptoTRAFO:
		{
			cfg = &cfg_trf;
			break;
		}
	case eqptoTRAFO3E:
		{
			cfg = &cfg_trf3;
			break;
		}
	case eqptoTRECHO:
		{
			cfg = &cfg_tre;
			break;
		}
	default:
		{
			cfg = &cfg_zzz;
			break;
		}
	}
	// obtém lista de Atributos do Eqpto
	legenda->LisAtributos(eqpto, lisATR);
	// inicia CListBox e CBoxAtributo de atributos
	IniciaCLBoxAtributo(lisATR);
	// inicia TView de dados
	TViewInicia();
	// inicia StringList de arquivos de fotos
	IniciaStringListFoto();
	// seleciona TabSheet
	if ((PageControl->ActivePage == TabSheetFoto) && (foto.strlis->Count == 0))
	{
		PageControl->ActivePage = TabSheetDado;
	}
	// exibe/esconde TabSheetFoto
	TabSheetFoto->TabVisible = (foto.strlis->Count > 0);
	// força exibição da primeira foto
	if (foto.strlis->Count > 0)
	{
		foto.index = -1;
		ActionFotoFirstExecute(NULL);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::EscondeGrafico(void)
{
	chart->Visible = false;
	CLBoxSerie->Visible = false;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::ExibeGrafico(TList *lisVET)
{
	// variáveis locais
	double val_min, val_max, increment;
	VTVetval *vetval;
	VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

	// exibe somente as Series necessarias
	for (int n = lisVET->Count; n < 4; n++)
	{
		chart->Series[n]->Active = false;
	}
	for (int n = 0; (n < lisVET->Count) && (n < 4); n++)
	{
		chart->Series[n]->Active = true;
	}
	// reinicia TChart
	chart->Title->Text->Clear();
	if (mercado != NULL)
		chart->Title->Text->Add("Ano de referência: " + IntToStr(mercado->AnoAtual));
	else
		chart->Title->Text->Add("Ano de referência: " + IntToStr(YearOf(Now())));
	if (chart->LeftAxis->Maximum >= 0)
	{
		chart->LeftAxis->Minimum = 0.;
		chart->LeftAxis->Maximum = 0.;
	}
	else
	{
		chart->LeftAxis->Maximum = 0.;
		chart->LeftAxis->Minimum = 0.;
	}
	// reinicia CLBoxSerie: exibe uma Serie p/ cada VetVal
	CLBoxSerie->Clear();
	for (int n = 0; (n < lisVET->Count) && (n < chart->SeriesCount()); n++)
	{
		vetval = (VTVetval*)lisVET->Items[n];
		ChartInsereCurva(vetval, chart->Series[n], DefineCorSerie(n), (n == 0));
		CLBoxSerie->Items->AddObject(vetval->DescUnid(), chart->Series[n]);
		CLBoxSerie->Checked[n] = true;
	}
	// determina valores máximo e mínimo do eixo y
	val_max = chart->LeftAxis->Maximum;
	val_min = chart->LeftAxis->Minimum;
	// inclui uma margem adicional nos valores máximo e mínimo
	increment = fabs(val_max) * 0.05;
	// (val_max >= 0) ? (val_max += increment) : (val_max -= increment);
	// (val_min >= 0) ? (val_min -= increment) : (val_min += increment);
	val_max += increment;
	val_min -= increment;
	// define incremento
	if (increment > 10000)
		increment = 10000.;
	else if (increment > 1000)
		increment = 1000.;
	else if (increment > 100)
		increment = 100.;
	else if (increment > 10)
		increment = 10.;
	else if (increment > 1.)
		increment = 1.;
	else if (increment > 0.1)
		increment = 0.1;
	else if (increment > 0.01)
		increment = 0.01;
	else
		increment = 0.001;
	// redefine valores mínimo e máximo do eixo y
	chart->LeftAxis->Visible = true;
	chart->LeftAxis->Automatic = false;
	chart->LeftAxis->AxisValuesFormat = "#0.#0"; // DVK 2011.11.3
	if (val_max > 0)
	{
		chart->LeftAxis->Maximum = val_max;
		chart->LeftAxis->Minimum = val_min;
	}
	else
	{
		chart->LeftAxis->Minimum = val_min;
		chart->LeftAxis->Maximum = val_max;
	}
	chart->LeftAxis->Increment = increment;
	if (val_max < 0.001 && val_max > -0.001) // ===============DVK 2011.11.3 inicio
	{ // para valores máximos muito próximos de zero
		chart->LeftAxis->AxisValuesFormat = "0.0E+";
		// chart->LeftAxis->Increment = val_max -= 0.1 * val_max;
	} // ================DVK 2011.11.3 fim
	chart->Visible = true;
	CLBoxSerie->Visible = true;
	chart->Refresh();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::ExportaGrupo(VTGrupo *grupo)
{
	// variáveis locais                   //===============DVK 2011.12.13 inicio
	VTGrupo *grupofilho;
	TList *lisGRUPO = grupo->LisGrupo;

	// insere dados na StringList
	Lines->Add(grupo->Descricao);
	ExportaLisDado(grupo->LisDado);
	ExportaLisVetval(grupo->LisVetval);

	// loop p/ todos os Grupos de lisGRUPO
	for (int n = 0; n < lisGRUPO->Count; n++)
	{
		grupofilho = (VTGrupo*)lisGRUPO->Items[n];
		ExportaGrupo(grupofilho);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::ExportaLisDado(TList *lisDADO)
{
	// variáveis locais
	VTDado *dado;

	// loop p/ todos os Dados de lisDADO
	for (int n = 0; n < lisDADO->Count; n++)
	{
		dado = (VTDado*)lisDADO->Items[n];
		// insere um novo dado na StringList
		Lines->Add(dado->DescUnid() + ";" + dado->valor);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::ExportaLisVetval(TList *lisVETVAL)
{
	// variáveis locais
	VTVetval *vetval;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// loop p/ todos os Dados de lisVETVAL
	for (int n = 0; n < lisVETVAL->Count; n++)
	{
		vetval = (VTVetval*)lisVETVAL->Items[n];
		Lines->Add("Patamar ; " + vetval->DescUnid());
		// preenche a Série c/ os valores em todos patamares
		for (int np = 0; np < patamares->NumPatamar(); np++)
		{
			Lines->Add(IntToStr(np) + ";" + vetval->ValorAsStr[np]);
		}
		Lines->Add(";");
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEqpto::ExportaRelatorioMedidor(void)
{
	// variáveis locais
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString arq_medidor = path->DirExporta() + "\\Medidor.csv";

	try
	{
		// insere dados na StringList
		ExportaGrupo(grupo);

		// salva StringList no arquivo
		Lines->SaveToFile(arq_medidor);

		// limpa StringList
		Lines->Clear();

		Aviso("Os dados do medidor foram exportados para " + arq_medidor + ".");
		return true;
	}
	catch (...)
	{
		return false;
	}
} // ================DVK 2011.12.13 fim

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::HabilitaActionFoto(void)
{
	// verifica número de fotos disponíveis
	if (foto.strlis->Count <= 1)
	{ // desabilita todas Actions
		ActionFotoFirst->Enabled = false;
		ActionFotoLast->Enabled = false;
		ActionFotoNext->Enabled = false;
		ActionFotoPrev->Enabled = false;
		return;
	}
	// verifica se está selecionada primeira foto
	if (foto.index == 0)
	{ // desabilita ActionFotoFirst e ActionFotoPrev
		ActionFotoFirst->Enabled = false;
		ActionFotoLast->Enabled = true;
		ActionFotoNext->Enabled = true;
		ActionFotoPrev->Enabled = false;
		return;
	}
	// verifica se está selecionada última foto
	if (foto.index >= foto.strlis->Count - 1)
	{ // desabilita ActionFotoFirst e ActionFotoPrev
		ActionFotoFirst->Enabled = true;
		ActionFotoLast->Enabled = false;
		ActionFotoNext->Enabled = false;
		ActionFotoPrev->Enabled = true;
		return;
	}
	// está selecioanada uma foto intermediária: habilita todas Actions
	ActionFotoFirst->Enabled = true;
	ActionFotoLast->Enabled = true;
	ActionFotoNext->Enabled = true;
	ActionFotoPrev->Enabled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::IniciaCLBoxAtributo(TList *lisATR)
{
	// variáveis locais
	TAtributo *atributo;

	// reinicia
	CLBoxAtributo->Clear();
	// proteção
	if (eqpto == NULL)
		return;
	// apreenche CLBox c/ atributos
	for (int n = 0; n < lisATR->Count; n++)
	{
		atributo = (TAtributo*)lisATR->Items[n];
		CLBoxAtributo->Items->AddObject(atributo->nome, atributo);
		CLBoxAtributo->Checked[n] = eqpto->Show[atributo->bit_index];
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::IniciaStringListFoto(void)
{
	/*
	 //variáveis locais
	 VTBD_foto *bd_foto = (VTBD_foto*)apl->GetObject(__classid(VTBD_foto));

	 //reinicia StringList c/ nome dos arquivos de fotos
	 foto.strlis->Clear();
	 //proteção
	 if (bd_foto == NULL) return;
	 //obtém lista de arquivo de fotos do Eqpto
	 bd_foto->LeArqFoto(eqpto, foto.strlis);
	 //verifica se existem os arquivos das fotos
	 for (int n = foto.strlis->Count-1; n >= 0; n--)
	 {
	 if (! FileExists(foto.strlis->Strings[n])) foto.strlis->Delete(n);
	 }
	 */
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TFormEqpto::PM_GetShow(unsigned bit)
 {
 //return((PD.show & bit) == bit);
 return((PD.cfg->show & bit) == bit);
 }

 //---------------------------------------------------------------------------
 void __fastcall TFormEqpto::PM_SetShow(unsigned bit, bool enabled)
 {
 if (enabled)
 {PD.cfg->show |= bit;}
 else
 {PD.cfg->show &= ~bit;}
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::TViewAdvancedCustomDrawItem(TCustomTreeView *Sender, TTreeNode *Node,
	TCustomDrawState State, TCustomDrawStage Stage, bool &PaintImages, bool &DefaultDraw)
{
	// variáveis locais
	AnsiString class_name;
	int ind_pat;
	TColor color_brush, color_text;
	TRect NodeRect;
	TObject *object = (TObject*)(Node->Data);
	VTDado *dado;
	VTGrupo *grupo;
	VTVetval *vetval;
	VTPatamar *patamar;

	// proteção
	if (Stage != cdPostPaint)
		return;
	// verifica se é um TreNode sem nenhum objeto associado
	if (object == NULL)
		return;
	// obtém área de texto do TreeNode
	NodeRect = Node->DisplayRect(true);
	NodeRect.Right = TView->ClientRect.Right;
	// define cores
	color_brush = TView->Canvas->Brush->Color;
	color_text = TView->Canvas->Font->Color;
	if (Node == TView->Selected)
	{
		TView->Canvas->Font->Color = clWhite;
		TView->Canvas->Brush->Color = clHighlight;
	}
	// limpa área de texto do TreeNode
	TView->Canvas->FillRect(NodeRect);
	// divide a área de texto em duas colunas
	TRect rect_col1(NodeRect);
	TRect rect_col2(NodeRect);
	// rect_col1.Right = (NodeRect.Left + 100);
	rect_col1.Right = TViewHeader->Sections->Items[0]->Width;
	rect_col2.Left = rect_col1.Right + 2;
	// verifica o objeto associado ao TreeNode
	class_name = object->ClassName();
	if (class_name.AnsiCompareIC("TGrupo") == 0)
	{ // exibe descrição do Grupo
		// grupo = (VTGrupo*)object;
		// TView->Canvas->TextRect(rect_col1, rect_col1.Left, rect_col1.Top + 2, grupo->Descricao));
		TView->Canvas->TextRect(rect_col1, rect_col1.Left, rect_col1.Top + 2, Node->Text);
	}
	else if (class_name.AnsiCompareIC("TDado") == 0)
	{ // exibe descrição do Dado
		dado = (VTDado*)object;
		TView->Canvas->TextRect(rect_col1, rect_col1.Left, rect_col1.Top + 2, dado->DescUnid());
		// exibe valor do Dado
		TView->Canvas->TextRect(rect_col2, rect_col2.Left, rect_col2.Top + 2, dado->valor);
	}
	else if (class_name.AnsiCompareIC("TVetval") == 0)
	{ // obtém vetval
		vetval = (VTVetval*)object;
		// o texto do Node contém o índice do patamar
		ind_pat = StrToInteger(Node->Text);
		if (ind_pat < 0)
		{ // exibe apenas descrição do Vetval
			TView->Canvas->TextRect(rect_col1, rect_col1.Left, rect_col1.Top + 2,
			vetval->DescUnid());
		}
		else
		{ // exibe hora do patamar
			patamar = patamares->Patamar[ind_pat];
			TView->Canvas->TextRect(rect_col1, rect_col1.Left, rect_col1.Top + 2, patamar->Nome);
			// exibe valor
			TView->Canvas->TextRect(rect_col2, rect_col2.Left, rect_col2.Top + 2,
				vetval->ValorAsStr[ind_pat]);
		}
	}
	// restaura cores
	TView->Canvas->Brush->Color = color_brush;
	TView->Canvas->Font->Color = color_text;
	// bloqueia alteração automática
	DefaultDraw = false;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::TViewClick(TObject *Sender)
{
	// variáveis locais
	int count = TView->SelectionCount;
	AnsiString class_name;
	TTreeNode *Node;
	VTVetval *vetval;

	// verifica se houve alteração
	if (!TViewSelectionChanged(lisVET))
	{
		if (lisVET->Count == 0)
		{
			EscondeGrafico();
		}
		return;
	}
	// monta lisVET c/ os objetos Vetval dos TreeNode selecionados
	lisVET->Clear();
	// OBS: os TreNodes selecionados estão ordenados na ordem inversa à seq. da seleção
	for (int n = count - 1; n >= 0; n--)
	{
		Node = TView->Selections[n];
		// verifica se o TreeNode possui um objeto associado
		if (Node->Data == NULL)
			continue;
		// verifica se o objeto associado ao TreeNode é um Vetval
		class_name = ((TObject*)Node->Data)->ClassName();
		if (class_name.AnsiCompareIC("TVetval") == 0)
		{ // insere o Vetval em lisVET
			lisVET->Add(Node->Data);
		}
	}
	// reinicia configuração de atributos que devem ser exibidos no TChart
	cfg->chart_atrib = 0;
	for (int n = 0; n < lisVET->Count; n++)
	{
		vetval = (VTVetval*)lisVET->Items[n];
		// atualiza configuração de atributo que devem ser exibidos no TChart
		cfg->chart_atrib |= vetval->ind_atrib;
	}
	// verifica se deve exibir ou esconde o gráfico
	if (lisVET->Count == 0)
	{
		EscondeGrafico();
	}
	else
	{
		ExibeGrafico(lisVET);
	}
}

// ---------------------------------------------------------------------------
VTEqpto* __fastcall TFormEqpto::TViewEqptoSelecionado(void)
{
	// variáveis locais
	AnsiString class_name;
	TObject *object;
	VTEqpto *eqpto;
	TTreeNode *node;

	// determina TreeNode selecionado
	if ((node = TView->Selected) == NULL)
		return (NULL);
	// verifica se o TreeNode selecionado ou um Parent possui um Eqpto selecionado
	do
	{
		if ((object = (TObject*)node->Data) == NULL)
			continue;
		if ((eqpto = VTEqpto::TipoEqpto(object)) != NULL)
			return (eqpto);
	}
	while ((node = node->Parent) != NULL);
	return (NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::TViewHeaderSectionResize(THeaderControl *TViewHeader,
	THeaderSection *Section)
{
	// variáveis locais
	int width;
	/*
	 TViewHeader->Sections->Items[1]->Width = TViewHeader->ClientWidth - TViewHeader->Sections->Items[0]->Width + 1;
	 //atualiza o TView
	 TView->Refresh();
	 */
	width = TViewHeader->ClientWidth - TViewHeader->Sections->Items[0]->Width + 1;
	if (TViewHeader->Sections->Items[1]->Width != width)
	{
		TViewHeader->Sections->Items[1]->Width = width;
	}
	TView->Refresh();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::TViewInicia(void)
{
	// variáveis locais
	unsigned all_atributo = 0xFFFFFFFF;
	unsigned show_orig;
	TTreeNode *node_raiz;
	VTLegenda *legenda = (VTLegenda*)apl->GetObject(__classid(VTLegenda));

	// esconde o TreeView p/ evitar flicker
	TView->Visible = false;
	// reinicia LView
	TView->Items->Clear();
	// destrói eventual Grupo de dados lido anteriormente
	if (grupo != NULL)
	{
		delete grupo;
		grupo = NULL;
	}
	// salva configuração original dos atributos do Eqpto
	show_orig = eqpto->ShowAsUnsigned;
	// redefine configuração dos atributos do Eqpto p/ obter todos os atributos do Eqpto
	eqpto->ShowAsUnsigned = all_atributo;
	// obtém Grupo de dados do Eqpto
	if ((grupo = legenda->IniciaGrupo(eqpto)) == NULL)
		return;
	// restaura configuração original dos atributos do Eqpto
	eqpto->ShowAsUnsigned = show_orig;
	// insere TreeNode raiz c/ o Grupo
	TViewInsereGrupo(NULL, grupo);
	// simula um click p/ atualizar TChart
	TViewClick(NULL);
	// seleciona node raiz
	TView->TopItem = TView->Items->GetFirstNode();
	// volta a exibir o TreeView já montado
	TView->Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::TViewInsereGrupo(TTreeNode *node_pai, VTGrupo *grupo)
{
	// variáveis locais
	TTreeNode *node;

	// verifica se o TreeNode pai é nulo
	if (node_pai == NULL)
	{ // insere um TreeNode de Level = 0
		node = TView->Items->Add(NULL, grupo->Descricao);
	}
	else
	{ // insere um TreeNode filho
		node = TView->Items->AddChildObject(node_pai, grupo->Descricao, grupo);
	}
	// insere TreeNodes p/ os Dados cadastrados no Grupo
	TViewInsereLisDado(node, grupo->LisDado);
	// insere TreeNodes p/ os Vetval cadastrados no Grupo
	TViewInsereLisVetval(node, grupo->LisVetval);
	// insere TreeNodes p/ os Grupos cadastrados no Grupo
	TViewInsereLisGrupo(node, grupo->LisGrupo);
	// expande o TreeNode
	if ((node->Level <= 1) && (!node->Expanded))
		node->Expand(false);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::TViewInsereLisDado(TTreeNode *node_pai, TList *lisDADO)
{
	// variáveis locais
	VTDado *dado;

	// loop p/ todos os Dados de lisDADO
	for (int n = 0; n < lisDADO->Count; n++)
	{
		dado = (VTDado*)lisDADO->Items[n];
		// insere um novo TTreeNode
		TView->Items->AddChildObject(node_pai, dado->DescUnid(), dado);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::TViewInsereLisGrupo(TTreeNode *node_pai, TList *lisGRUPO)
{
	// variáveis locais
	VTGrupo *grupo;

	// loop p/ todos os Grupos de lisGRUPO
	for (int n = 0; n < lisGRUPO->Count; n++)
	{
		grupo = (VTGrupo*)lisGRUPO->Items[n];
		TViewInsereGrupo(node_pai, grupo);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEqpto::TViewInsereLisVetval(TTreeNode *node_pai, TList *lisVETVAL)
{
	// variáveis locais
	AnsiString txt;
	TTreeNode *node;
	VTVetval *vetval;

	// loop p/ todos os Dados de lisDADO
	for (int n = 0; n < lisVETVAL->Count; n++)
	{
		vetval = (VTVetval*)lisVETVAL->Items[n];
		// insere um novo TTreeNode c/ a descrição do Vetval
		// node = TView->Items->AddChildObject(node_pai, vetval->DescUnid(), vetval);
		// TRUQUE SUJO: passa o valor -1 no atributo Text (ver TViewAdvancedCustomDrawItem())
		txt.sprintf("                     -1");
		node = TView->Items->AddChildObject(node_pai, txt, vetval);
		// cria um TreeNode p/ cada valor
		for (int np = 0; np < vetval->Len; np++)
		{ // insere um novo TTreeNode c/ hora do patamar e valor
			// TRUQUE SUJO: passa o índice do patamar no atributo Text (ver TViewAdvancedCustomDrawItem())
			txt.sprintf("                     %d", np);
			TView->Items->AddChildObject(node, txt, vetval);
		}
		// verifica se o node deve ser selecionado p/ ser exibido no TChart
		if (TView->SelectionCount == 0)
		{ // seleciona no máximo um TreeNode
			node->Selected = ((vetval->ind_atrib & cfg->chart_atrib) != 0);
		}
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEqpto::TViewSelectionChanged(TList *lisVET)
{
	// variáveis locais
	int count = TView->SelectionCount;

	// verifica se o número de objetos selecionados em Selections é diferente
	// do número de objetos em lisVET
	if (lisVET->Count != count)
		return (true);
	// verifica se lisVET contem os mesmos objetos de Selections
	for (int n = 0; n < count; n++)
	{
		if (lisVET->IndexOf(TView->Selections[n]->Data) < 0)
			return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
// eof
