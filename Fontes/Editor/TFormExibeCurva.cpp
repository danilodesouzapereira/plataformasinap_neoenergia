// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "TFormExibeCurva.h"
#include "TConverteCurva.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Const.h"
#include "..\Curvas\VTCurva.h"
#include "..\Legenda\VTLegenda.h"
#include "..\Legenda\VTVetval.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTFases.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Legenda.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;

// ---------------------------------------------------------------------------
__fastcall TFormExibeCurva::TFormExibeCurva(TForm* form_owner, VTApl *apl_owner,
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
	converte = new TConverteCurva(apl);
	//anula ponteiro
	clone = NULL;
	// oculta gbTipo (apresentado somente para carga)
	gbTipo->Visible = false;
	CBoxTipoCurva->ItemIndex = 0;
}

// ---------------------------------------------------------------------------
__fastcall TFormExibeCurva::~TFormExibeCurva(void)
{
	// destrói objetos criados
	if (legenda)
	{
		delete legenda;
		legenda = NULL;
	}
	if (converte)
	{
		delete converte;
		converte = NULL;
	}
	if (clone)
	{
		delete clone;
		clone = NULL;
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
void __fastcall TFormExibeCurva::CBoxTipoCurvaChange(TObject *Sender)
{
	//variaveis locais
	VTCurva *curva_new, *curva_old;

	// reinicia RGroupSerie
	RGroupSerie->Items->Clear();
	// verifica tipo de curva selecionado
	if (CBoxTipoCurva->ItemIndex == 0)
	{
		HabilitaCurva(car_P);
		HabilitaCurva(car_Q);
	}
	else
	{
		HabilitaCurvaPorFase();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormExibeCurva::ChartClick(TObject *Sender)
{
	// variáveis locais
	double val_min, val_max, delta;
	TChartSeries *Serie;

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
	delta = fabs((fabs(val_max) - fabs(val_min))) * 0.1;
	//delta = fabs(val_max) * 0.05;
	val_max += delta;
	val_min -= delta;
    // define incremento
	// redefine valores mínimo e máximo do eixo y
	Chart->LeftAxis->Visible = true;
	Chart->LeftAxis->Automatic = false;
	Chart->LeftAxis->AxisValuesFormat = "#0.###0"; // DVK 2011.11.3
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
	Chart->LeftAxis->Increment = delta;
	if (delta!=0 && delta<=0.0001) // ===============DVK 2011.11.3 inicio
	{ // para valores máximos muito próximos de zero
		Chart->LeftAxis->AxisValuesFormat = "0.0E+";
		// chart->LeftAxis->Increment = val_max -= 0.1 * val_max;
	} // ================DVK 2011.11.3 fim
	Chart->Visible = true;
	Chart->Refresh();
	// exibe a Serie
	Serie->Visible = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormExibeCurva::DefineCurva(VTEqpto *eqpto, VTCurva *curva)
{
	// salva ponteiro p/ objeto
	this->eqpto = eqpto;
	this->curva = curva;
	//destroi clone se ja existir
	if(clone != NULL)
		delete clone;
	//cria clone de eqpto
	clone = eqpto->Clone();
	//salva eqpto e curva original no objeto de conversao
	converte->DefineCurva(eqpto, curva);
	// reinicia CLBoxSerie
	RGroupSerie->Items->Clear();
	//verifica se a curva é propria (bloqueia tipo de grafico)
	CBoxTipoCurva->Enabled = (curva->Tipica);
}
/*
 //---------------------------------------------------------------------------
 void __fastcall TFormExibeCurva::HabilitaCurva(int atributo_id, AnsiString caption)
 {
 //variáveis locais
 double       valor[nvCURVA_PQVT];
 strHM        hm;
 TChartSeries *Serie;
 VTVetval     *vetval;
 VTPatamar    *patamar;
 VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

 //verifica se é possível incluir uma nova Curva
 if (RGroupSerie->Items->Count >= Chart->SeriesCount()) return;
 //determina Serie p/ a Curva
 Serie = Chart->Series[RGroupSerie->Items->Count];
 Serie->Title = caption;
 Serie->Clear();
 //obtém lista de valores do atributo indicado
 legenda->LisVetValFluxo(eqpto, lisVETVAL, atributo_id);
 if (lisVETVAL->Count == 0) return;
 //insere Serie no TChart p/ o Vetval
 vetval = (VTVetval*)lisVETVAL->First();
 ChartExibeCurva(vetval);
 //preenche a Série c/ os valores em todos patamares
 for (int np = 0; np < patamares->NumPatamar(); np++)
 {//determina Patamar
 patamar = patamares->Patamar[np];
 //determina valores da Curva  no patamar
 hm.hora   = patamar->Hora;
 hm.minuto = patamar->Minuto;
 if (! curva->GetValor(hm, valor, curva->NumeroValores)) continue;
 //insere ponto na Serie
 Serie->AddXY(np, valor[ind_valor], "", Serie->Color);
 //inclui ponto adicional p/ representar o fim do patamar(histograma)
 //serie->AddXY(np+1, val, IntToStr(np+1), cor);
 Serie->AddXY(np+patamar->Duracao, valor[ind_valor], "", Serie->Color);
 }
 //inclui Serie em RGroupSerie
 RGroupSerie->ItemIndex = RGroupSerie->Items->AddObject(caption, Serie);
 //força atualização do Chart
 ChartClick(NULL);
 }
 */

// ---------------------------------------------------------------------------
void __fastcall TFormExibeCurva::HabilitaCurva(int atributo_id)
{
	// variáveis locais
	int index;
	double valor;
	TChartSeries *Serie;
	VTVetval *vetval;
	VTPatamar *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	// reinicia RGroupSerie
	// RGroupSerie->Items->Clear();
	// obtém lista de valores do atributo indicado
	legenda->LisVetValEqpto(clone, lisVETVAL, atributo_id);
	if (lisVETVAL->Count == 0)
		return;
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
			// insere ponto na Serie
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
void __fastcall TFormExibeCurva::HabilitaCurvaPorFase(void)
{
	//verifica se a curva está por fase
	VTCarga *carga = (VTCarga*)clone;
	VTCurva *curva = carga->curva;

	if(curva->NumeroValores == nvCURVA_RRCAR)
		HabilitaPorFase();
	else
		HabilitaTrifasico();
}

// ---------------------------------------------------------------------------
void __fastcall TFormExibeCurva::HabilitaPorFase(void)
{
	// variáveis locais
	AnsiString caption, unid_p, unid_q;
	double valor[nvCURVA_RRCAR];
	strHM hm;
	int max_val = 6;
	TChartSeries *Serie;
	VTCarga *carga;
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
	//verifica unidade
	unid_p = Unidade(curva->Unidade) + "W";
	unid_q = Unidade(curva->Unidade) + "var";
	// determina fases disponíveis da carga p/ curva
	carga = (VTCarga*)clone;
	if(carga->Fases & faseA)
	{
		Serie = Chart->Series[0];
		Serie->Title = "Pa(" + unid_p + ")";
		Serie->Visible = true;
		//RGroupSerie->ItemIndex = RGroupSerie->Items->AddObject(Serie->Title, Serie);
		RGroupSerie->Items->AddObject(Serie->Title, Serie);
		Serie = Chart->Series[1];
		Serie->Title = "Qa(" + unid_q + ")";
		Serie->Visible = true;
		//RGroupSerie->ItemIndex = RGroupSerie->Items->AddObject(Serie->Title, Serie);
		RGroupSerie->Items->AddObject(Serie->Title, Serie);
	}
	if(carga->Fases & faseB)
	{
		Serie = Chart->Series[2];
		Serie->Title = "Pb(" + unid_p + ")";
		Serie->Visible = true;
		//RGroupSerie->ItemIndex = RGroupSerie->Items->AddObject(Serie->Title, Serie);
		RGroupSerie->Items->AddObject(Serie->Title, Serie);
		Serie = Chart->Series[3];
		Serie->Title = "Qb(" + unid_q + ")";
		Serie->Visible = true;
		//RGroupSerie->ItemIndex = RGroupSerie->Items->AddObject(Serie->Title, Serie);
		RGroupSerie->Items->AddObject(Serie->Title, Serie);
	}
	if(carga->Fases & faseC)
	{
		Serie = Chart->Series[4];
		Serie->Title = "Pc(" + unid_p + ")";
		Serie->Visible = true;
		//RGroupSerie->ItemIndex = RGroupSerie->Items->AddObject(Serie->Title, Serie);
		RGroupSerie->Items->AddObject(Serie->Title, Serie);
		Serie = Chart->Series[5];
		Serie->Title = "Qc(" + unid_q + ")";
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
void __fastcall TFormExibeCurva::HabilitaTrifasico(void)
{
	// variaveis locais
	AnsiString caption, unid_p, unid_q, unid, fase_str;
	double valor[nvCURVA_RRCAR];
	double valor_tri;
	strHM hm;
	int max_val = 6;
	int fase_tag;
	TChartSeries *SerieP, *SerieQ;
	VTCarga *carga;
	VTPatamar *patamar;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTVetval *vetval_p, *vetval_q;

	// verifica se é possivel incluir uma nova Curva
	if (RGroupSerie->Items->Count >= Chart->SeriesCount())
		return;
	//limpa e oculta todas as series
	for(int n = 0; n < Chart->SeriesCount(); n++)
	{
		SerieP = Chart->Series[n];
		SerieP->Clear();
		SerieP->Visible = false;
	}
	RGroupSerie->Items->Clear();
	//verifica unidade
	// obtem lista de valores do atributo P
	lisVETVAL->Clear();
	legenda->LisVetValEqpto(clone, lisVETVAL, car_P);
	if (lisVETVAL->Count == 0)
		return;
	vetval_p = (VTVetval*)lisVETVAL->First();
	// obtem lista de valores do atributo Q
	lisVETVAL->Clear();
	legenda->LisVetValEqpto(clone, lisVETVAL, car_Q);
	if (lisVETVAL->Count == 0)
		return;
	vetval_q = (VTVetval*)lisVETVAL->First();
	//determina unidade
	unid = vetval_p->unidade.SubString(0,vetval_p->unidade.Length() - 1);
	unid_p = unid + "W";
	unid = vetval_q->unidade.SubString(0,vetval_q->unidade.Length() - 3);
	unid_q = unid + "var";

	// determina fases disponiveis da carga p/ curva
	for(int ind_fase = indFASE_A, index = 0; ind_fase <= indFASE_C; ind_fase++, index+=2)
	{
		carga = (VTCarga*)clone;
		fase_tag = fases->IndexToTag[ind_fase];
		fase_str = fases->AsStrABCN[fase_tag];
		if(carga->Fases & fase_tag)
		{//insere serie P
			SerieP = Chart->Series[index];
			SerieP->Title = "P" + fase_str + "(" + unid_p + ")";
			SerieP->Visible = true;
			RGroupSerie->Items->AddObject(SerieP->Title, SerieP);

			//insere serie Q
            SerieQ = Chart->Series[index+1];
			SerieQ->Title = "Q" + fase_str + "(" + unid_q + ")";
			SerieQ->Visible = true;
			RGroupSerie->Items->AddObject(SerieQ->Title, SerieQ);

			// preenche a Serie c/ os valores em todos patamares
			for (int np = 0; np < patamares->NumPatamar(); np++)
			{ // determina valor no patamar
				patamar = patamares->Patamar[np];
				//P
				valor_tri = vetval_p->Valor[np];
				//converte valor trifasico em valor por fase
				valor[index] =  converte->Valor(ind_fase, valor_tri, carga->Fases);
				// insere ponto na Serie
				SerieP->AddXY(np, valor[index], "", SerieP->Color);
				// inclui ponto adicional p/ representar o fim do patamar(histograma)
				SerieP->AddXY(np + patamar->Duracao, valor[index], "", SerieP->Color);

				//Q
				valor_tri = vetval_q->Valor[np];
				//converte valor trifasico em valor por fase
				valor[index+1] =  converte->Valor(ind_fase, valor_tri, carga->Fases);
				// insere ponto na Serie
				SerieQ->AddXY(np, valor[index+1], "", SerieQ->Color);
				// inclui ponto adicional p/ representar o fim do patamar(histograma)
				SerieQ->AddXY(np + patamar->Duracao, valor[index+1], "", SerieQ->Color);
			}
		}
	}

	// força atualizacao do Chart
	ChartClick(NULL);

	// força atualizacao do Chart
//	if(Serie != NULL)
//		RGroupSerie->ItemIndex = RGroupSerie->Items->IndexOfObject(Serie);
	RGroupSerie->ItemIndex = 0;
	//ChartClick(NULL);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormExibeCurva::Unidade(int unid_curva)
{
	switch(unid_curva)
	{
		case unidKVA:
			return "k";
		case unidMVA:
			return "M";
		default:
			return "";
    }
}

// ---------------------------------------------------------------------------
// eof
