// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "TFormExibeCurvaBateria.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Const.h"
#include "..\Curvas\VTCurva.h"
#include "..\Legenda\VTLegenda.h"
#include "..\Legenda\VTVetval.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBateria.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Legenda.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;

// ---------------------------------------------------------------------------
__fastcall TFormExibeCurvaBateria::TFormExibeCurvaBateria(TForm* form_owner, VTApl *apl_owner,
	TWinControl *parent) : TForm(Owner)
{
	// salva ponteiro p/ objetos
	this->apl = apl_owner;
	// insere em seu Parent
	if (parent)
	{
		Parent = parent;
		Align = alClient;
	}
	// cria objetos
	legenda = DLL_NewObjLegenda(apl);
	lisVETVAL = new TList();
	// oculta gbTipo (apresentado somente para carga)
	gbTipo->Visible = false;
	CBoxTipoCurva->ItemIndex = 0;
	IniciaAbcissa(bat_P);
	IniciaAbcissa(bat_Q);
}

// ---------------------------------------------------------------------------
__fastcall TFormExibeCurvaBateria::~TFormExibeCurvaBateria(void)
{
	// destrói objetos criados
	if (legenda)
	{
		delete legenda;
		legenda = NULL;
	}
	// destrói listas e seus obejtos
	if (lisVETVAL)
	{
		LimpaTList(lisVETVAL);
		delete lisVETVAL;
		lisVETVAL = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormExibeCurvaBateria::CBoxTipoCurvaChange(TObject *Sender)
{
	// reinicia RGroupSerie
	RGroupSerie->Items->Clear();
	// verifica tipo de curva selecionado
	if (CBoxTipoCurva->ItemIndex == 0)
	{
		HabilitaCurva(bat_P);
		HabilitaCurva(bat_Q);
	}
	else
	{
		HabilitaCurvaPorFase();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormExibeCurvaBateria::ChartClick(TObject *Sender)
{
	// variáveis locais
	double val_min, val_max, delta;
	TChartSeries *Serie;
	TChartSeries *SerieAbcissa;

	// esconde todas as Series
	for (int n = 0; n < Chart->SeriesCount(); n++)
	{
		Chart->Series[n]->Visible = false;
	}
	// reinicia dimensões do eixo Y
	if (Chart->LeftAxis->Minimum > 0)
	{
		Chart->LeftAxis->Minimum = 0;
		Chart->LeftAxis->Maximum = 0;
	}
	else
	{
		Chart->LeftAxis->Maximum = 0;
		Chart->LeftAxis->Minimum = 0;
	}
	// proteção
	if (RGroupSerie->ItemIndex < 0)
		return;
	// determina Serie selecionada
	Serie = (TChartSeries*)RGroupSerie->Items->Objects[RGroupSerie->ItemIndex];
	// redimensiona o eixo Y
	// Chart->LeftAxis->Maximum = 1.2 * Serie->MaxYValue();
	// Chart->LeftAxis->Minimum = 0.8 * Serie->MinYValue();
	// determina valores máximo e mínimo do eixo y
	val_max = Serie->MaxYValue();
	val_min = Serie->MinYValue();
	// inclui uma margem adicional nos valores máximo e mínimo
	delta = fabs(val_max) * 0.05;
	val_max += delta;
	val_min -= delta;
	// redimensiona o eixo Y
	if (val_max > 0)
	{
		Chart->LeftAxis->Maximum = val_max;
		Chart->LeftAxis->Minimum = val_min;
	}
	else
	{
		Chart->LeftAxis->Minimum = val_min;
		Chart->LeftAxis->Maximum = val_max;
	}
	// exibe a Serie
	Serie->Visible = true;
    // exibe a serie das abcissas
	for (int ns = 0; ns < Chart->SeriesCount(); ns++)
	{
		SerieAbcissa = (TChartSeries*)Chart->Series[ns];
		if ((SerieAbcissa->Name) == ("AbcissaP") || (SerieAbcissa->Name) == ("AbcissaQ"))
		{
			SerieAbcissa->Visible = true;
            break;
		}
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormExibeCurvaBateria::DefineCurva(VTEqpto *eqpto, VTCurva *curva)
{
	// salva ponteiro p/ objeto
	this->eqpto = eqpto;
	this->curva = curva;
	// reinicia CLBoxSerie
	RGroupSerie->Items->Clear();
}

// ---------------------------------------------------------------------------
void __fastcall TFormExibeCurvaBateria::HabilitaCurva(int atributo_id)
{
	// variáveis locais
	int index;
	double valor;
	TChartSeries *Serie;
	VTVetval *vetval;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// reinicia RGroupSerie
	// RGroupSerie->Items->Clear();
	// obtém lista de valores do atributo indicado
	legenda->LisVetValEqpto(eqpto, lisVETVAL, atributo_id);
	if (lisVETVAL->Count == 0)
		return;
	// percorre listVETVAL
	for (int n = 0; n < lisVETVAL->Count; n++)
	{
		vetval = (VTVetval*)lisVETVAL->Items[n];
		// verifica se existe ChartSeries disponível
		if (RGroupSerie->Items->Count >= Chart->SeriesCount())
			return;
		// determina Serie p/ a Curva
		Serie = Chart->Series[RGroupSerie->Items->Count];
		Serie->Title = vetval->DescUnid();
		Serie->Clear();
		// insere Serie no TChart p/ o Vetval
		// preenche a Série c/ os valores em todos patamares
		for (int np = 0; np < patamares->NumPatamar(); np++)
		{ // determina valor no patamar
			valor = vetval->Valor[np];
			// insere ponto da curva na Serie
			Serie->AddXY(np, valor, IntToStr(np + 1), Serie->Color);
			Serie->AddXY(np + 1, valor, IntToStr(np + 1), Serie->Color);
		}
		// inclui Serie em RGroupSerie
		index = RGroupSerie->Items->AddObject(Serie->Title, Serie);
		if (index == 0)
		{
			RGroupSerie->ItemIndex = 0;
			// força atualização do Chart
			ChartClick(NULL);
		}
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormExibeCurvaBateria::HabilitaCurvaPorFase(void)
{
	// variáveis locais
	AnsiString caption;
	double valor[nvCURVA_RRCAR];
	strHM hm;
	int max_val = 6;
	TChartSeries *Serie;
	VTBateria *bateria;
	VTPatamar *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// verifica se é possível incluir uma nova Curva
	if (RGroupSerie->Items->Count >= Chart->SeriesCount())
		return;
	//limpa e oculta todas as series
	for(int n = 0; n < Chart->SeriesCount(); n++)
	{
		Serie = Chart->Series[n];
		Serie->Clear();
		Serie->Visible = false;
	}
	RGroupSerie->Items->Clear();
	// determina fases disponíveis da carga p/ curva
	bateria = (VTBateria*)eqpto;
	if(bateria->Fases & faseA)
	{
		Serie = Chart->Series[0];
		Serie->Title = "Pa(kW)";
		Serie->Visible = true;
		//RGroupSerie->ItemIndex = RGroupSerie->Items->AddObject(Serie->Title, Serie);
		RGroupSerie->Items->AddObject(Serie->Title, Serie);
		Serie = Chart->Series[1];
		Serie->Title = "Qa(kvar)";
		Serie->Visible = true;
		//RGroupSerie->ItemIndex = RGroupSerie->Items->AddObject(Serie->Title, Serie);
		RGroupSerie->Items->AddObject(Serie->Title, Serie);
	}
	if(bateria->Fases & faseB)
	{
		Serie = Chart->Series[2];
		Serie->Title = "Pb(kW)";
		Serie->Visible = true;
		//RGroupSerie->ItemIndex = RGroupSerie->Items->AddObject(Serie->Title, Serie);
		RGroupSerie->Items->AddObject(Serie->Title, Serie);
		Serie = Chart->Series[3];
		Serie->Title = "Qb(kvar)";
		Serie->Visible = true;
		//RGroupSerie->ItemIndex = RGroupSerie->Items->AddObject(Serie->Title, Serie);
		RGroupSerie->Items->AddObject(Serie->Title, Serie);
	}
	if(bateria->Fases & faseC)
	{
		Serie = Chart->Series[4];
		Serie->Title = "Pc(kW)";
		Serie->Visible = true;
		//RGroupSerie->ItemIndex = RGroupSerie->Items->AddObject(Serie->Title, Serie);
		RGroupSerie->Items->AddObject(Serie->Title, Serie);
		Serie = Chart->Series[5];
		Serie->Title = "Qc(kvar)";
		Serie->Visible = true;
		//RGroupSerie->ItemIndex = RGroupSerie->Items->AddObject(Serie->Title, Serie);
		RGroupSerie->Items->AddObject(Serie->Title, Serie);
	}
	// preenche a Série c/ os valores em todos patamares
	for (int np = 0; np < patamares->NumPatamar(); np++)
	{ // determina Patamar
		patamar = patamares->Patamar[np];
		// determina valores da Curva  no patamar
		hm.hora = patamar->HoraIni;
		hm.minuto = patamar->MinutoIni;
		if (!curva->GetValor(hm, valor, curva->NumeroValores))
			continue;
		//loop em todos os valores
		for(int nv = 0; nv < max_val; nv++)
		{
			Serie = Chart->Series[nv];
			if(!Serie->Visible) continue;
			// insere ponto na Serie
			Serie->AddXY(np, valor[nv], "", Serie->Color);
			// inclui ponto adicional p/ representar o fim do patamar(histograma)
			// serie->AddXY(np+1, val, IntToStr(np+1), cor);
			Serie->AddXY(np + patamar->Duracao, valor[nv], "", Serie->Color);
		}
	}

	// força atualização do Chart
//	if(Serie != NULL)
//		RGroupSerie->ItemIndex = RGroupSerie->Items->IndexOfObject(Serie);
	RGroupSerie->ItemIndex = 0;
	//ChartClick(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormExibeCurvaBateria::IniciaAbcissa(int atributo_id)
{
	TChartSeries *SerieAbcissa;
    VTPatamar *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	// determina Serie p/ a Abcissa
	SerieAbcissa = Chart->Series[RGroupSerie->Items->Count-1];
	SerieAbcissa->Clear();
	SerieAbcissa->Visible = true;
	SerieAbcissa->Color = clBlack;
	if (atributo_id == bat_P) SerieAbcissa->Name = "AbcissaP";
	if (atributo_id == bat_Q) SerieAbcissa->Name = "AbcissaQ";
	// preenche a Série c/ os valores em todos patamares
	for (int np = 0; np < patamares->NumPatamar(); np++)
	{
		// insere ponto da abcissa na Serie
		SerieAbcissa->AddXY(np, 0.0, IntToStr(np+1), clBlack);
		SerieAbcissa->AddXY(np+1, 0.0, IntToStr(np+1), clBlack);
	}

}

// ---------------------------------------------------------------------------
// eof
