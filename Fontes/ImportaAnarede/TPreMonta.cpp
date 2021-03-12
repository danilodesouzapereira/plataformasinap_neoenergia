// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <Math.hpp>
#include <math.h>
#include <algorithm>
#include "TPreMonta.h"
#include "TDbar.h"
#include "TDbsh.h"
#include "TDinj.h"
#include "TDlin.h"
#include "TDgbt.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Classes\VTClasse.h"
#include "..\Importa\Monta\VTMonta.h"
#include "..\Log\VTLog.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
using namespace std;

// ---------------------------------------------------------------------------
VTPreMonta* NewObjPreMonta(VTApl *apl)
{
	return (new TPreMonta(apl));
}

// ---------------------------------------------------------------------------
__fastcall TPreMonta::TPreMonta(VTApl *apl)
{
	// salva ponteiros p/ objetos
	this->apl = apl;
	plog = (VTLog*)apl->GetObject(__classid(VTLog));
	monta = (VTMonta*)apl->GetObject(__classid(VTMonta));
	// cria listas
	lisDBAR_SEL = new TList();
	lisDBAR = new TList();
	lisDBSH = new TList();
	lisDINJ = new TList();
	lisDLIN = new TList();
	lisDGBT = new TList();
	// inicia flag que indica sucesso/falha na montagem da rede
	sucesso = true;
	coord_definida = false;
	rede_id = -1;
	Sbase = 100.; // MVA
}

// ---------------------------------------------------------------------------
__fastcall TPreMonta::~TPreMonta(void)
{
	// destrói lista sem destruir seus objetos
	if (lisDBAR_SEL)
	{
		delete lisDBAR_SEL;
		lisDBAR_SEL = NULL;
	}
	// destrói lista e seus objetos
	if (lisDBAR)
	{
		LimpaTList(lisDBAR);
		delete lisDBAR;
		lisDBAR = NULL;
	}
	if (lisDBSH)
	{
		LimpaTList(lisDBSH);
		delete lisDBSH;
		lisDBSH = NULL;
	}
	if (lisDINJ)
	{
		LimpaTList(lisDINJ);
		delete lisDINJ;
		lisDINJ = NULL;
	}
	if (lisDLIN)
	{
		LimpaTList(lisDLIN);
		delete lisDLIN;
		lisDLIN = NULL;
	}
	if (lisDGBT)
	{
		LimpaTList(lisDGBT);
		delete lisDGBT;
		lisDGBT = NULL;
	}
}

/*
 //---------------------------------------------------------------------------
 double __fastcall TPreMonta::Converte_pu_nF(double yc_pu, double Sbase_mva, double Vbase_kv, double comp_km)
 {
 //variáveis locais
 double yc_s, cap_nf;
 double freq = 60.; //hertz
 double w    =  2. * M_PI * freq;
 double Zbase_ohm = (Vbase_kv * Vbase_kv)/Sbase_mva;

 //calcula admitância em siemen
 yc_s = yc_pu / Zbase_ohm;
 //calcula capacitância em nf/km
 cap_nf = (yc_s / w) * 1e9;
 return(cap_nf / comp_km);
 }
 */
// -----------------------------------------------------------------------------
double __fastcall TPreMonta::Converte_pu_ohm(double valor_pu, double Sbase_mva, double Vbase_kv)
{
	// variáveis locais
	double Zbase_ohm = (Vbase_kv * Vbase_kv) / Sbase_mva;
	return (valor_pu * Zbase_ohm);
}

// -----------------------------------------------------------------------------
void __fastcall TPreMonta::ConverteCoordenadaDbar(void)
{
	// variáveis locais
	int x1, y1, x2, y2, dx, dy;
	double fator;
	TDbar *dbar;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTArea *area = redes->AreaEsquematico();

	// proteção
	if (lisDBAR->Count == 0)
		return;
	// calcula área definida pelos Dbar
	dbar = (TDbar*)lisDBAR->First();
	x1 = x2 = dbar->coord_x;
	y1 = y2 = dbar->coord_y;
	for (int n = 1; n < lisDBAR->Count; n++)
	{
		dbar = (TDbar*)lisDBAR->Items[n];
		x1 = min(x1, dbar->coord_x);
		y1 = min(y1, dbar->coord_y);
		x2 = max(x2, dbar->coord_x);
		y2 = max(y2, dbar->coord_y);
	}
	// calcula fator entre áreas da rede e área definda pelos DBar
	dx = x2 - x1 + 1;
	dy = y2 - y1 + 1;
	fator = min((area->Width() / dx), (area->Height() / dy));
	// loop p/ todos TDbar
	for (int n = 0; n < lisDBAR->Count; n++)
	{
		dbar = (TDbar*)lisDBAR->Items[n];
		dbar->coord_x = area->x1 + ((dbar->coord_x - x1) * fator);
		dbar->coord_y = area->y1 + ((dbar->coord_y - y1) * fator);
	}
}

// -----------------------------------------------------------------------------
void __fastcall TPreMonta::DefineCoordenadaDbar(void)
{
	// variáveis locais
	int dx, dy, x, y, num_bar;
	VTArea *area;
	TDbar *dbar;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// determina número de barras da rede
	if ((num_bar = lisDBAR->Count) == 0)
		return;
	// define coordenadas iniciais p/ barras
	area = redes->AreaEsquematico();
	x = area->x1;
	y = area->y1;
	dx = area->Width() / num_bar;
	dy = area->Height() / num_bar;
	// loop p/ todos TDbar
	for (int n = 0; n < lisDBAR->Count; n++)
	{
		dbar = (TDbar*)lisDBAR->Items[n];
		dbar->coord_x = x;
		dbar->coord_y = y;
		x += dx;
		y += dy;
	}
}

// -----------------------------------------------------------------------------
int __fastcall TPreMonta::DefineTiposup(TDbar *dbar)
{
	if (dbar->TipoPV())
		return (supPV);
	if (dbar->TipoVF())
		return (supVF);
	return (supPQ);
}

/*
 //-----------------------------------------------------------------------------
 void __fastcall TPreMonta::DefineDistEqbar(void)
 {
 //variáveis locais
 int    x1, y1, x2, y2;
 int    dx, dy;
 TArea  *area;
 TDbar *dbar;
 TEqbar *eqbar;
 TList  *lisBAR;
 TList  *lisEQP;
 struct strEQPTOS{
 int tipo;
 int sinal_x;
 int sinal_y;
 } eqptos[] = {{eqptoGERADOR,    -1,  0},
 {eqptoSUPRIMENTO,  0, -1},
 {eqptoCARGA,       1,  1},
 {eqptoCAPACITOR,  -1,  1},
 {eqptoREATOR,      0,  1},
 {eqptoFILTRO,      1, -1},
 {eqptoTRAFOZZ,     1,  0}};

 //determina ponteiro p/ Área da Rede
 area   = rede->AreaDiagEsquematico();
 //calcula área definida pelas Dbars
 lisBAR   = rede->LisDbar();
 if (lisBAR->Count == 0) return;
 dbar   = (TDbar*)lisBAR->First();
 area->x1 = min(dbar->x1, dbar->x2);
 area->y1 = min(dbar->y1, dbar->y2);
 area->x2 = max(dbar->x1, dbar->x2);
 area->y2 = max(dbar->y1, dbar->y2);
 for (int n = 1; n < lisBAR->Count; n++)
 {
 dbar = (TDbar*)lisBAR->Items[n];
 area->x1 = min(area->x1, min(dbar->x1, dbar->x2));
 area->y1 = min(area->y1, min(dbar->y1, dbar->y2));
 area->x2 = max(area->x2, max(dbar->x1, dbar->x2));
 area->y2 = max(area->y2, max(dbar->y1, dbar->y2));
 }
 //define distância de Eqbar
 dx = (area->Width()  * 0.005);
 dy = (area->Height() * 0.005);
 //loop p/ todos Eqbar
 for (int n = 0; n < (sizeof(eqptos)/sizeof(strEQPTOS)); n++)
 {
 lisEQP = rede->LisEqbar(eqptos[n].tipo);
 for (int i = 0; i < lisEQP->Count; i++)
 {
 eqbar = (TEqbar*)lisEQP->Items[i];
 eqbar->diag.dxbar = dx * eqptos[n].sinal_x;
 eqbar->diag.dybar = dy * eqptos[n].sinal_y;
 }
 }
 }

 //-----------------------------------------------------------------------------
 bool __fastcall TPreMonta::DefineIadmTrecho(void)
 {
 //variáveis locais
 TCabo   *pcabo;
 TTrecho *ptrecho;

 //define Iadm dos Trechos
 for (int n = 0; n < lisTRE->Count; n++)
 {
 ptrecho = (TTrecho*)lisTRE->Items[n];
 pcabo = ptrecho->pcabo_local;
 pcabo->iadm = (pcabo->iadm/(sqrt(3.)*ptrecho->pbar1->vnom)) * 1000.; //conversão kA ==> A
 }
 return(true);
 }

 //-----------------------------------------------------------------------------
 bool __fastcall TPreMonta::DefineVnomTrafo(void)
 {
 //variáveis locais
 TTrafo *ptrafo;

 //define tensões dos Trafos
 for (int n = 0; n < lisTRF->Count; n++)
 {
 ptrafo = (TTrafo*)lisTRF->Items[n];
 //verifica se Vnom da Dbar2 é maior que a Vnom da Dbar1
 if (ptrafo->pbar2->vnom > ptrafo->pbar1->vnom)
 {//redefine a Dbar do primário como sendo a Dbar2
 ptrafo->RedefineDbarDoPrimario(ptrafo->pbar2);
 }
 //define Vnom do primário e do secundário
 ptrafo->pri.vnom = ptrafo->pbar1->vnom;
 ptrafo->sec.vnom = ptrafo->pbar2->vnom;
 //converte os taps de pu p/ kV
 ptrafo->pri.tap = ptrafo->pri.tap * ptrafo->pri.vnom;
 ptrafo->sec.tap = ptrafo->sec.tap * ptrafo->sec.vnom;
 }
 return(true);
 }
 */

// -----------------------------------------------------------------------------
TDbar* __fastcall TPreMonta::ExisteDbar(int numero)
{
	// variáveis locais
	TDbar *dbar;

	// procura Dbar na lista lisDBAR
	for (int n = 0; n < lisDBAR->Count; n++)
	{
		dbar = (TDbar*)lisDBAR->Items[n];
		if (dbar->numero == numero)
			return (dbar);
	}
	// não encontrou Dbar
	return (NULL);
}

// -----------------------------------------------------------------------------
TDgbt* __fastcall TPreMonta::ExisteDgbt(AnsiString grupo)
{
	// variáveis locais
	TDgbt *dgbt;

	// procura TDgbt na lista
	for (int n = 0; n < lisDGBT->Count; n++)
	{
		dgbt = (TDgbt*)lisDGBT->Items[n];
		if (dgbt->grupo.AnsiCompareIC(grupo) == 0)
			return (dgbt);
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereDBAR(TStringList *strList)
{
	// variáveis locais
	TDbar *dbar;

	try
	{ // cria nova Dbar
		dbar = new TDbar();
		// lê dados da própria Dbar
		dbar->id = lisDBAR->Count + 1;
		dbar->numero = StrToInteger(strList->Strings[0]);
		dbar->operacao = strList->Strings[1];
		dbar->estado = strList->Strings[2];
		dbar->tipo = StrToInteger(strList->Strings[3]);
		dbar->grupo_base_tensao = strList->Strings[4];
		dbar->nome = strList->Strings[5];
		dbar->grupo_limite_tensao = strList->Strings[6];
		dbar->tensao_pu = LeValorPtoImplicito(strList->Strings[7], 3);
		dbar->angulo_graus = StrToDouble(strList->Strings[8]);
		dbar->geracao_mw = StrToDouble(strList->Strings[9]);
		dbar->geracao_mvar = StrToDouble(strList->Strings[10]);
		dbar->geracao_min_mvar = StrToDouble(strList->Strings[11]);
		dbar->geracao_max_mvar = StrToDouble(strList->Strings[12]);
		dbar->barra_controlada = StrToInteger(strList->Strings[13]);
		dbar->carga_mw = StrToDouble(strList->Strings[14]);
		dbar->carga_mvar = StrToDouble(strList->Strings[15]);
		dbar->capac_reator_mvar = StrToDouble(strList->Strings[16]);
		dbar->area = StrToInteger(strList->Strings[17]);
		dbar->tensao_carga_pu = LeValorPtoImplicito(strList->Strings[18], 3);
		// verifica se Dbar está sendo excluído
		if (dbar->Excluida())
		{ // destrói objeto
			delete dbar;
			return (true);
		}
		// insere DBAR em lisDBAR
		lisDBAR->Add(dbar);
		// verifica se a barra atende ao filtro de área
		if ((AreaFiltrada < 0) || (dbar->area == AreaFiltrada))
		{ // insere Dbar em lisDBAR
			lisDBAR_SEL->Add(dbar);
		}
	}
	catch (Exception &e)
	{
		plog->Add("Erro na leitura do cartão DBAR " + AnsiQuotedStr(strList->Strings[0], '\''));
		// destrói objeto
		if (dbar)
			delete dbar;
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereDBSH(TStringList *strList)
{
	// variáveis locais
	TDbsh *dbsh;
	int nc = -1;
	strINDIV *indiv;

	try
	{ // cria nova Dbsh
		dbsh = new TDbsh();
		// lê dados da própria Dbsh
		dbsh->id = lisDBSH->Count + 1;
		dbsh->da_barra = StrToInteger(strList->Strings[++nc]);
		dbsh->operacao = strList->Strings[++nc];
		dbsh->para_barra = StrToInteger(strList->Strings[++nc]);
		dbsh->circuito = StrToInteger(strList->Strings[++nc]);
		dbsh->controle = strList->Strings[++nc];
		dbsh->v_min = LeValorPtoImplicito(strList->Strings[++nc], 3);
		dbsh->v_max = LeValorPtoImplicito(strList->Strings[++nc], 3);
		dbsh->barra_controlada = StrToInteger(strList->Strings[++nc]);
		dbsh->inj_ini = StrToDouble(strList->Strings[++nc]);
		dbsh->tipo_controle = strList->Strings[++nc];
		dbsh->apaga_dbar = strList->Strings[++nc];
		dbsh->extremidade = StrToInteger(strList->Strings[++nc]);
		// dados individualizados
		while (nc < (strList->Count - 1))
		{
			indiv = new strINDIV();
			indiv->grupo = StrToInteger(strList->Strings[++nc]);
			indiv->operacao = strList->Strings[++nc];
			indiv->estado = strList->Strings[++nc];
			indiv->unidades = StrToInteger(strList->Strings[++nc]);
			indiv->uni_op = StrToInteger(strList->Strings[++nc]);
			indiv->q_mvar = StrToDouble(strList->Strings[++nc]);
			dbsh->lisINDIV->Add(indiv);
		}
		// insere DBSH em lisDBSH
		lisDBSH->Add(dbsh);
	}
	catch (Exception &e)
	{
		plog->Add("Erro na leitura do cartão DBSH " + AnsiQuotedStr(strList->Strings[0], '\''));
		// destrói objeto
		if (dbsh)
			delete dbsh;
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereDGBA(TStringList *strList)
{
	// variáveis locais
	int numero;
	TDbar *dbar;

	try
	{ // determina Dbar
		numero = StrToInteger(strList->Strings[0]);
		if ((dbar = ExisteDbar(numero)) == NULL)
			return (true);
		// determina coordenadas da Dbar
		dbar->coord_x = StrToInteger(strList->Strings[1]);
		dbar->coord_y = StrToInteger(strList->Strings[2]);
	}
	catch (Exception &e)
	{
		plog->Add("Erro na leitura do cartão DGBA " + strList->Strings[0]);
		return (true);
	}
	// assume que as coordenadas estão definidas
	coord_definida = true;
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereDGBT(TStringList *strList)
{
	// variáveis locais
	TDgbt *dgbt;

	// verifica se existe TDgbt
	if ((dgbt = ExisteDgbt(strList->Strings[0])) != NULL)
		return (true);
	try
	{ // cria um novo TDgbt e insere na lista
		dgbt = new TDgbt();
		// lê dado do Dgbt
		dgbt->grupo = strList->Strings[0];
		dgbt->tensao_kv = StrToDouble(strList->Strings[1]);
	}
	catch (Exception &e)
	{
		plog->Add("Erro na leitura do cartão DGBT " + strList->Strings[0]);
		return (true);
	}
	// verifica DGBT com tensão não convencional
	if ((dgbt->tensao_kv > 700) || (dgbt->tensao_kv < 13))
	{
		plog->Add("DGBT " + AnsiQuotedStr(dgbt->grupo, '\'') + ": tensão = " + strList->Strings[1]);
	}
	// define ID
	dgbt->id = lisDGBT->Count + 1;
	// insere TDgbt na lista lisDGBT
	lisDGBT->Add(dgbt);

	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereDINJ(TStringList *strList)
{
	// variáveis locais
	TDinj *dinj;

	try
	{ // cria objeto Dinj
		dinj = new TDinj();
		dinj->numero_barra = StrToInteger(strList->Strings[0]);
		dinj->operacao = strList->Strings[1];
		dinj->injecao_mw = StrToDouble(strList->Strings[2]);
		dinj->injecao_mvar = StrToDouble(strList->Strings[3]);
		dinj->shunt_mvar = StrToDouble(strList->Strings[4]);
		dinj->fator_part_pu = (StrToDouble(strList->Strings[5])) / 100.; // conversão % ==> pu
	}
	catch (Exception &e)
	{ // erro na leitura dos dados: nada a fazer
		plog->Add("Erro na leitura do cartão DINJ " + strList->Strings[0]);
	}
	// define ID
	dinj->id = lisDINJ->Count + 1;
	// insere Dinj em lisDINJ
	lisDINJ->Add(dinj);

	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereDLIN(TStringList *strList)
{
	// variáveis locais
	int index_dbar1, index_dbar2;
	AnsiString txt;
	TDlin *dlin;

	try
	{ // cria objeto Dlin
		dlin = new TDlin();
		dlin->da_barra = StrToInteger(strList->Strings[0]);
		dlin->abertura_da_barra = strList->Strings[1];
		dlin->operacao = strList->Strings[2];
		dlin->abertura_para_barra = strList->Strings[3];
		dlin->para_barra = StrToInteger(strList->Strings[4]);
		dlin->circuito = StrToInteger(strList->Strings[5]);
		dlin->estado = strList->Strings[6];
		dlin->proprietario = strList->Strings[7];
		dlin->resistencia_pu = LeValorPtoImplicito(strList->Strings[8], 2) / 100.;
		// conversão % ==> pu
		dlin->reatancia_pu = LeValorPtoImplicito(strList->Strings[9], 2) / 100.;
		// conversão % ==> pu
		dlin->susceptancia_shunt_mvar = LeValorPtoImplicito(strList->Strings[10], 3);
		dlin->tap_pu = LeValorPtoImplicito(strList->Strings[11], 3);
		dlin->tap_min_pu = LeValorPtoImplicito(strList->Strings[12], 3);
		dlin->tap_max_pu = LeValorPtoImplicito(strList->Strings[13], 3);
		dlin->defasagem = LeValorPtoImplicito(strList->Strings[14], 2);
		dlin->barra_controlada = StrToInteger(strList->Strings[15]);
		dlin->capacidade_normal = StrToDouble(strList->Strings[16]);
		dlin->capacidade_emergencia = StrToDouble(strList->Strings[17]);
		dlin->numero_steps = StrToInteger(strList->Strings[18]);
	}
	catch (Exception &e)
	{ // erro na leitura dos dados: nada a fazer
		plog->Add("Erro na leitura do cartão DLIN de " + AnsiQuotedStr(strList->Strings[0], '\'') +
			" para " + AnsiQuotedStr(strList->Strings[2], '\''));
		return (false);
	}
	// determina Barras da Ligação
	dlin->dbar1 = ExisteDbar(dlin->da_barra);
	dlin->dbar2 = ExisteDbar(dlin->para_barra);
	if ((dlin->dbar1 == NULL) || (dlin->dbar2 == NULL))
	{
		txt = "Ligação inválida: ";
		if (dlin->dbar1 == NULL)
			txt += "falta barra(" + strList->Strings[0] + ") ";
		if (dlin->dbar2 == NULL)
			txt += "falta barra(" + strList->Strings[4] + ")";
		plog->Add(txt);
		// destrói objeto
		delete dlin;
		return (true);
	}
	// verifica se ligação conecta barras da área filtrada
	if ((AreaFiltrada < 0) || (dlin->dbar1->area == AreaFiltrada) ||
		(dlin->dbar2->area == AreaFiltrada))
	{ // ligação conecta barras da área filtrada: insere barras em lisBAR_SEL
		if (lisDBAR_SEL->IndexOf(dlin->dbar1) < 0)
			lisDBAR_SEL->Add(dlin->dbar1);
		if (lisDBAR_SEL->IndexOf(dlin->dbar2) < 0)
			lisDBAR_SEL->Add(dlin->dbar2);
		// define ID
		dlin->id = lisDLIN->Count + 1;
		// insere Dlin em lisDLIN
		lisDLIN->Add(dlin);
		// marca as duas barras estão conectadas
		dlin->dbar1->conectada = true;
		dlin->dbar2->conectada = true;
	}
	else
	{ // destrói objeto
		delete dlin;
	}
	return (true);
}

// -----------------------------------------------------------------------------
double __fastcall TPreMonta::LeValorPtoImplicito(AnsiString valor, int ncd)
{
	// variáveis locais
	double val = 0.;

	// proteção: verifica se string nulo
	if ((valor.Trim()).IsEmpty())
		return (val);
	try
	{ // converte string p/ double
		val = StrToDouble(valor);
		// verifica se string contém ponto decimal explícito
		if (!PontoDecimalExplicito(valor))
		{ // converte valor
			val /= pow(10., double(ncd));
		}
	}
	catch (Exception &e)
	{
	}
	return (val);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaBarra(void)
{
	// variáveis locais
	AnsiString txt;
	strBARRA str_barra;
	TDbar *dbar;

	// inicia inserção de Barra
	monta->InsereBarraStart();
	// loop p/ todos TDbar em lisDBAR_SEL
	for (int n = 0; n < lisDBAR_SEL->Count; n++)
	{
		dbar = (TDbar*)lisDBAR_SEL->Items[n];
		// proteção: verifica se DBAR foi conectao por algum DLIN
		if (!dbar->conectada)
			continue;
		// determina Dgbt
		if ((dbar->dgbt = ExisteDgbt(dbar->grupo_base_tensao)) == NULL)
		{
			plog->Add("Barra sem Grupo Base de Tensão" + AnsiQuotedStr
				(IntToStr(dbar->numero), '\''));
			return (false);
		}
		if ((dbar->dgbt->tensao_kv > 700) || (dbar->dgbt->tensao_kv < 13))
		{
			plog->Add(txt.sprintf("Barra '%d' com Vnom=%2.1f kV (grupo '%s')", dbar->numero,
				dbar->dgbt->tensao_kv, dbar->dgbt->grupo.c_str()));
		}
		// preenche estrutura strREDE
		str_barra.bar_id = dbar->id; // dbar->numero;
		// str_barra.codigo = IntToStr(dbar->numero);
		str_barra.codigo = IntToStr(dbar->numero) + ":" + dbar->nome;
		str_barra.vnom = dbar->dgbt->tensao_kv;
		str_barra.utm.x = 0;
		str_barra.utm.y = 0;
		str_barra.esq.x1 = dbar->coord_x;
		str_barra.esq.y1 = dbar->coord_y;
		str_barra.esq.x2 = dbar->coord_x;
		str_barra.esq.y2 = dbar->coord_y;
		// insere Rede em Monta
		if (!monta->InsereBarraEqpto(str_barra))
			return (false);
	}
	// finaliza inserção de Barra
	monta->InsereBarraStop();
	return (true);
}

/*
 //-----------------------------------------------------------------------------
 bool __fastcall TPreMonta::MontaCapacitor(void)
 {
 //variáveis locais
 int          num_cap = 1;
 strCAPACITOR str_cap;
 TDbar        *dbar;

 //loop p/ todos TDbar em lisDBAR
 for (int n = 0; n < lisDBAR->Count; n++)
 {
 dbar = (TDbar*)lisDBAR->Items[n];
 //verifica se Dbar está sendo excluído
 if (dbar->Excluida()) continue;
 //verifica se Dbar possui Capacitor
 if (! dbar->ExisteCapacitor()) continue;
 //preenche estrutura strCAPACITOR
 str_cap.cap_id = -1;
 str_cap.bar_id = dbar->id; //dbar->numero;
 str_cap.codigo = "BC_" + IntToStr(num_cap++);
 str_cap.vnom   = dbar->dgbt->tensao_kv;
 for (int np = 0; np < MAX_PAT; np++)
 {
 str_cap.q[np] = fabs(dbar->capac_reator_mvar);
 }
 str_cap.esq.posbar = 50;
 str_cap.esq.dxbar  = 0;
 str_cap.esq.dybar  = 0;
 //insere Capacitor em Monta
 if (! monta->InsereCapacitor(str_cap)) return(false);
 }
 return(true);
 }
 */
// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaCapacitor(void)
{
	// variáveis locais
	int num_cap = 1;
	strCAPACITOR str_cap;
	TDbar *dbar;

	// loop p/ todos TDbar em lisDBAR
	for (int n = 0; n < lisDBAR->Count; n++)
	{
		dbar = (TDbar*)lisDBAR->Items[n];
		// proteção: verifica se DBAR foi conectado por algum DLIN
		if (!dbar->conectada)
			continue;
		// proteção
		if (dbar->dgbt == NULL)
		{
			plog->Add("Capacitor associado a Barra sem Grupo Base de Tensão" +
				AnsiQuotedStr(IntToStr(dbar->numero), '\''));
			continue;
		}
		// verifica se Dbar está sendo excluído
		if (dbar->Excluida())
			continue;
		// verifica se Dbar possui Capacitor
		if (!dbar->ExisteCapacitor())
			continue;
		// preenche estrutura strCAPACITOR
		str_cap.cap_id = -1;
		str_cap.bar_id = dbar->id; // dbar->numero;
		str_cap.codigo = "BC_" + IntToStr(num_cap++);
		str_cap.vnom = dbar->dgbt->tensao_kv;
		str_cap.q_mvar = fabs(dbar->capac_reator_mvar);
		str_cap.ligacao = lgTRIANG;
		str_cap.curva.tipica = true;
		str_cap.curva.id = -1;
		str_cap.curva.num_pat = 0;
		str_cap.esq.posbar = 50;
		str_cap.esq.dxbar = 0;
		str_cap.esq.dybar = DXY_BAR;
		// insere Capacitor em Monta
		if (!monta->InsereCapacitor(str_cap))
			return (false);
	}
	return (true);
}

/*
 //-----------------------------------------------------------------------------
 bool __fastcall TPreMonta::MontaCapacitorEquivalente(void)
 {
 //variáveis locais
 int          num_seq = 1;
 strCAPACITOR str_cap;
 TDinj        *dinj;
 TDbar        *dbar;

 //loop p/ todos TDinj em lisDINJ
 for (int n = 0; n < lisDINJ->Count; n++)
 {
 dinj = (TDinj*)lisDINJ->Items[n];
 //verifica se Dinj está sendo excluído
 if (dinj->Excluido()) continue;
 //verifica se existe reator shunt
 if (! dinj->ExisteCapacitor()) continue;
 //determina Dbar
 if ((dbar = ExisteDbar(dinj->numero_barra)) == NULL)
 {
 plog->Add("Erro na leitura do cartão DINJ: Barra inexistente " +
 AnsiQuotedStr(IntToStr(dinj->numero_barra), '\''));
 return(false);
 }
 //preenche estrutura strCAPACITOR
 str_cap.cap_id = -1;
 str_cap.bar_id = dbar->id; //dbar->numero;
 str_cap.codigo = "BCeqv_" + IntToStr(num_seq++);
 str_cap.vnom   = dbar->dgbt->tensao_kv;
 for (int np = 0; np < MAX_PAT; np++)
 {
 str_cap.q[np] = fabs(dinj->shunt_mvar);
 }
 str_cap.esq.posbar = 50;
 str_cap.esq.dxbar  =  0;
 str_cap.esq.dybar  =  0;
 //insere Capacitor em Monta
 if (! monta->InsereCapacitor(str_cap)) return(false);
 }
 return(true);
 }
 */
// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaCapacitorEquivalente(void)
{
	// variáveis locais
	int num_seq = 1;
	strCAPACITOR str_cap;
	TDinj *dinj;
	TDbar *dbar;

	// loop p/ todos TDinj em lisDINJ
	for (int n = 0; n < lisDINJ->Count; n++)
	{
		dinj = (TDinj*)lisDINJ->Items[n];
		// verifica se Dinj está sendo excluído
		if (dinj->Excluido())
			continue;
		// verifica se existe reator shunt
		if (!dinj->ExisteCapacitor())
			continue;
		// determina Dbar
		if ((dbar = ExisteDbar(dinj->numero_barra)) == NULL)
		{
			plog->Add("Erro na leitura do cartão DINJ: Barra inexistente " +
				AnsiQuotedStr(IntToStr(dinj->numero_barra), '\''));
			return (false);
		}
		// proteção: verifica se DBAR foi conectao por algum DLIN
		if (!dbar->conectada)
			continue;
		// preenche estrutura strCAPACITOR
		str_cap.cap_id = -1;
		str_cap.bar_id = dbar->id; // dbar->numero;
		str_cap.codigo = "BCeqv_" + IntToStr(num_seq++);
		str_cap.vnom = dbar->dgbt->tensao_kv;
		str_cap.q_mvar = fabs(dbar->capac_reator_mvar);
		str_cap.ligacao = lgTRIANG;
		str_cap.curva.tipica = true;
		str_cap.curva.id = -1;
		str_cap.curva.num_pat = 0;
		str_cap.esq.posbar = 50;
		str_cap.esq.dxbar = 0;
		str_cap.esq.dybar = DXY_BAR;
		// insere Capacitor em Monta
		if (!monta->InsereCapacitor(str_cap))
			return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaCapacitorShunt(void)
{
	// variáveis locais
	double valor_mvar;
	int num_cap = 1;
	strCAPACITOR str_cap;
	strINDIV *str_indiv;
	TDbsh *dbsh;
	TDbar *dbar;

	// loop p/ todos TDbsh em lisDBSH
	for (int n = 0; n < lisDBSH->Count; n++)
	{
		dbsh = (TDbsh*)lisDBSH->Items[n];
		for (int ni = 0; ni < dbsh->lisINDIV->Count; ni++)
		{ // determina valor de cada capacitor do banco
			str_indiv = (strINDIV*)dbsh->lisINDIV->Items[ni];
			valor_mvar = str_indiv->uni_op * str_indiv->q_mvar;
			if (IsDoubleZero(valor_mvar))
				continue;
			// desconsidera se estiver desligado
			if (str_indiv->estado.AnsiCompareIC("D") == 0)
				continue;
			// verifica se existe o dbar
			dbar = ExisteDbar(dbsh->da_barra);
			if (dbar == NULL)
				continue;
			// preenche estrutura strCAPACITOR
			str_cap.cap_id = FalsoId();
			str_cap.bar_id = dbar->id; // dbsh->da_barra;
			str_cap.codigo = "BC_" + IntToStr(num_cap++);
			str_cap.vnom = dbar->dgbt->tensao_kv;
			str_cap.q_mvar = fabs(valor_mvar);
			str_cap.ligacao = lgTRIANG;
			str_cap.curva.tipica = true;
			str_cap.curva.id = -1;
			str_cap.curva.num_pat = 0;
			str_cap.esq.posbar = 50;
			str_cap.esq.dxbar = 0;
			str_cap.esq.dybar = DXY_BAR;
			// insere Capacitor em Monta
			if (!monta->InsereCapacitor(str_cap))
				return (false);
		}
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaCapserie(void)
{
	// variáveis locais
	int num_seq = 1;
	TDlin *dlin;
	strCAPSERIE str;

	// loop p/ todos Dlin
	for (int n = 0; n < lisDLIN->Count; n++)
	{
		dlin = (TDlin*)lisDLIN->Items[n];
		// verifica se Dlin está sendo excluído
		if (dlin->Excluida())
			continue;
		// verifica se Dlin é um Capserie
		if (!dlin->TipoCapserie())
			continue;
		// monta estrutura strCAPSERIE
		str.id = -1;
		str.rede_id = rede_id;
		str.bar1_id = dlin->dbar1->id; // dbar1->numero;
		str.bar2_id = dlin->dbar2->id; // dbar2->numero;
		str.codigo = "CS_" + IntToStr(num_seq++);
		str.extern_id = "";
		str.vnom = dlin->dbar1->dgbt->tensao_kv;
		str.snom = Sbase;
		str.z.r = 0.;
		str.z.x = Converte_pu_ohm(dlin->reatancia_pu, Sbase, dlin->dbar1->dgbt->tensao_kv);
		str.esq.posbar1 = 50.;
		str.esq.posbar2 = 50.;
		str.esq.posbar3 = 0;
		// insere Capserie na rede
		if (!monta->InsereCapserie(str))
			return (false);
	}
	return (true);
}

/*
 //-----------------------------------------------------------------------------
 bool __fastcall TPreMonta::MontaCarga(void)
 {
 //variáveis locais
 int          num_car = 1;
 strCARGA     str_carga;
 TDbar        *dbar;

 //loop p/ todos TDbar em lisDBAR
 for (int n = 0; n < lisDBAR->Count; n++)
 {
 dbar = (TDbar*)lisDBAR->Items[n];
 //verifica se Dbar está sendo excluído
 if (dbar->Excluida()) continue;
 //verifica se Dbar possui Carga
 if (! dbar->ExisteCarga()) continue;
 //preenche estrutura strCARGA
 str_carga.car_id     = -1;
 str_carga.bar_id     = dbar->id; //dbar->numero;
 str_carga.codigo     = "Carga_" + IntToStr(num_car++);
 //assume modelo de carga Scte
 str_carga.Icte      = 0.;
 str_carga.Scte      = 1.;
 str_carga.Zcte      = 0.;
 for (int np = 0; np < MAX_PAT; np++)
 {
 str_carga.demanda[np].p = dbar->carga_mw;
 str_carga.demanda[np].q = dbar->carga_mvar;
 }
 str_carga.esq.posbar = 50;
 str_carga.esq.dxbar  = 0;
 str_carga.esq.dybar  = 0;
 //insere Carga em Monta
 if (! monta->InsereCarga(str_carga)) return(false);
 }
 return(true);
 }
 */
// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaCarga(void)
{
	// variáveis locais
	int num_car = 1;
	strCARGA str_carga;
	TDbar *dbar;

	// loop p/ todos TDbar em lisDBAR
	for (int n = 0; n < lisDBAR->Count; n++)
	{
		dbar = (TDbar*)lisDBAR->Items[n];
		// proteção: verifica se DBAR foi conectao por algum DLIN
		if (!dbar->conectada)
			continue;
		// verifica se Dbar está sendo excluído
		if (dbar->Excluida())
			continue;
		// verifica se Dbar possui Carga
		if (!dbar->ExisteCarga())
			continue;
		// preenche estrutura strCARGA
		str_carga.car_id = -1;
		str_carga.bar_id = dbar->id; // dbar->numero;
		str_carga.codigo = "Carga_" + IntToStr(num_car++);
		// assume modelo de carga Scte
		str_carga.modelo.icte_pu = 0.;
		str_carga.modelo.scte_pu = 1.;
		str_carga.modelo.zcte_pu = 0.;
		str_carga.classe_cons = consEQUIVALENTE;
		str_carga.fases = faseABC;
		str_carga.num_cons.comercial = 0;
		str_carga.num_cons.residencial = 0;
		str_carga.num_cons.industrial = 0;
		str_carga.num_cons.rural = 0;
		str_carga.num_cons.outros = 0;
		str_carga.num_cons.a4 = 0;
		str_carga.curva.id = -1;
		str_carga.curva.tipica = false;
		str_carga.ip.num_lamp = 0;
		str_carga.ip.pot_kw = 0;
		str_carga.ip.fat_pot = 0;
		str_carga.curva.num_pat = MAX_PAT;
		str_carga.curva.num_valor = nvCURVA_PQ;
		for (int np = 0, hora = 0; np < MAX_PAT; np++, hora += 6)
		{
			str_carga.curva.patamar[np].hm_ini.hora = hora;
			str_carga.curva.patamar[np].hm_ini.minuto = 0;
			str_carga.curva.patamar[np].hm_fim.hora = hora + 6;
			str_carga.curva.patamar[np].hm_fim.minuto = 0;
			// str_carga.demanda[np].p_mw        = dbar->carga_mw;
			// str_carga.demanda[np].q_mvar      = dbar->carga_mvar;
			str_carga.curva.patamar[np].valor[indP] = dbar->carga_mw;
			str_carga.curva.patamar[np].valor[indQ] = dbar->carga_mvar;
		}
		str_carga.esq.posbar = 50;
		str_carga.esq.dxbar = 0;
		str_carga.esq.dybar = DXY_BAR;
		// insere Carga em Monta
		if (!monta->InsereCarga(str_carga))
			return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
void __fastcall TPreMonta::MontaChaves(TDlin *dlin, VTTrecho *trecho)
{
	// variáveis locais
	strCHAVE str_chave;
	VTBarra *barra, *clone1, *clone2;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// verifica se existe barra 1
	barra = redes->ExisteBarra(dlin->dbar1->id);
	if (barra == NULL)
		return;
	// clona barra
	clone1 = barra->Clone();
	clone1->Id = FalsoId();
	clone1->Codigo = "B_" + barra->Codigo;
	redes->InsereBarra(clone1);
	// preenche estrutura strCHAVE (lado dbar1)
	str_chave.chv_id = FalsoId();
	str_chave.rede_id = rede_id;
	str_chave.bar1_id = dlin->dbar1->id;
	str_chave.bar2_id = clone1->Id;
	str_chave.codigo = "CH_" + IntToStr(dlin->id) + "_1";
	str_chave.corn = 500.;
	str_chave.tipochave.tipo = chaveDJ;
	str_chave.estado = (dlin->abertura_da_barra.AnsiCompareIC("D") != 0);
	// insere Chave em Monta
	if (!monta->InsereChave(str_chave))
		return;

	// verifica se existe barra 2
	barra = redes->ExisteBarra(dlin->dbar2->id);
	if (barra == NULL)
		return;
	// clona barra
	clone2 = barra->Clone();
	clone2->Id = FalsoId();
	clone2->Codigo = "B_" + barra->Codigo;
	redes->InsereBarra(clone2);
	// preenche estrutura strCHAVE (lado dbar2)
	str_chave.chv_id = FalsoId();
	str_chave.rede_id = rede_id;
	str_chave.bar1_id = dlin->dbar2->id;
	str_chave.bar2_id = clone2->Id;
	str_chave.codigo = "CH_" + IntToStr(dlin->id) + "_2";
	str_chave.corn = 500.;
	str_chave.tipochave.tipo = chaveDJ;
	str_chave.estado = (dlin->abertura_para_barra.AnsiCompareIC("D") != 0);
	// insere Chave em Monta
	if (!monta->InsereChave(str_chave))
		return;

	// redefine barras do trecho
	trecho->DefineObjBarra(clone1, clone2);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaEqbar(void)
{
	if (!MontaCapacitor())
		return (false);
	if (!MontaCapacitorShunt())
		return (false);
	if (!MontaCarga())
		return (false);
	if (!MontaGerador())
		return (false);
	if (!MontaReator())
		return (false);
	// equivalentes
	if (!MontaCapacitorEquivalente())
		return (false);
	if (!MontaReatorEquivalente())
		return (false);
	if (!MontaSuprimentoEquivalente())
		return (false);
	return (true);
}

/*
 //-----------------------------------------------------------------------------
 bool __fastcall TPreMonta::MontaGerador(void)
 {
 //variáveis locais
 int          num_ger = 1;
 strGERADOR   str_ger;
 TDbar        *dbar;

 //loop p/ todos TDbar em lisDBAR
 for (int n = 0; n < lisDBAR->Count; n++)
 {
 dbar = (TDbar*)lisDBAR->Items[n];
 //verifica se Dbar está sendo excluído
 if (dbar->Excluida()) continue;
 //verifica se Dbar possui Gerador
 if (! dbar->ExisteGeracao()) continue;
 //monta strGERADOR
 str_ger.ger_id     = -1;
 str_ger.bar_id     = dbar->id; //dbar->numero;
 str_ger.codigo     = "G_" + IntToStr(num_ger++);
 str_ger.extern_id  = "";
 str_ger.tiposup    = DefineTiposup(dbar);
 str_ger.ligacao    = lgEST_AT;
 str_ger.vnom       = dbar->dgbt->tensao_kv;
 str_ger.snom       = 100.;
 str_ger.fplim      = 0.8;
 str_ger.z0.r       = 0.01;
 str_ger.z0.x       = 0.05;
 str_ger.z1.r       = 0.01;
 str_ger.z1.x       = 0.05;
 for (int np = 0; np < MAX_PAT; np++)
 {
 str_ger.p[np]  = dbar->geracao_mw;
 str_ger.q[np]  = dbar->geracao_mvar;
 str_ger.v[np]  = dbar->tensao_pu;
 str_ger.t[np]  = DegToRad(dbar->angulo_graus);
 }
 str_ger.esq.posbar = 50;
 str_ger.esq.dxbar  = 0;
 str_ger.esq.dybar  = 0;
 //insere Gerador em Monta
 if (! monta->InsereGerador(str_ger)) return(false);
 }
 return(true);
 }
 */
// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaGerador(void)
{
	// variáveis locais
	int num_ger = 0;
	// strGERADOR    str_ger;
	strSUPRIMENTO str_ger;
	TDbar *dbar;

	// loop p/ todos TDbar em lisDBAR
	for (int n = 0; n < lisDBAR->Count; n++)
	{
		dbar = (TDbar*)lisDBAR->Items[n];
		// verifica se Dbar está sendo excluído
		if (dbar->Excluida())
			continue;
		// proteção: verifica se DBAR foi conectao por algum DLIN
		if (!dbar->conectada)
			continue;
		// verifica se Dbar possui Gerador
		if (!dbar->ExisteGeracao())
			continue;
		// monta strGERADOR
		// str_ger.ger_id       = -1;
		str_ger.sup_id = -1;
		str_ger.bar_id = dbar->id; // dbar->numero;
		str_ger.codigo = "G_" + IntToStr(num_ger++);
		str_ger.extern_id = "";
		str_ger.tiposup = DefineTiposup(dbar);
		// str_ger.tipo_geracao = gerVAPOR;
		// str_ger.ligacao      = lgEST_AT;
		// str_ger.vnom         = dbar->dgbt->tensao_kv;
		str_ger.vnom_kv = dbar->dgbt->tensao_kv;
		// str_ger.snom_mva     = 100.;
		// str_ger.smax_mva     = str_ger.snom_mva;
		str_ger.smax = 100.;
		// str_ger.fplim        = 0.8;
		// str_ger.z0.r         = 0.01;
		// str_ger.z0.x         = 0.05;
		// str_ger.z1.r         = 0.01;
		// str_ger.z1.x         = 0.05;
		str_ger.zeq0.r = 0.01;
		str_ger.zeq0.x = 0.05;
		str_ger.zeq1.r = 0.01;
		str_ger.zeq1.x = 0.05;
		// define curva com um único ponto
		str_ger.curva.id = -1;
		str_ger.curva.tipica = false;
		str_ger.curva.num_pat = 1;
		str_ger.curva.num_valor = nvCURVA_PQVT;
		str_ger.curva.patamar[0].hm_ini.hora = 0;
		str_ger.curva.patamar[0].hm_ini.minuto = 0;
		str_ger.curva.patamar[0].hm_fim.hora = 24;
		str_ger.curva.patamar[0].hm_fim.minuto = 0;
		// str_ger.demanda[0].p_mw        = dbar->geracao_mw;
		// str_ger.demanda[0].q_mvar      = dbar->geracao_mvar;
		// str_ger.demanda[0].v_pu        = dbar->tensao_pu;
		// str_ger.demanda[0].t_rad       = DegToRad(dbar->angulo_graus);
		str_ger.curva.patamar[0].valor[indP] = dbar->geracao_mw;
		str_ger.curva.patamar[0].valor[indQ] = dbar->geracao_mvar;
		str_ger.curva.patamar[0].valor[indV] = dbar->tensao_pu;
		str_ger.curva.patamar[0].valor[indT] = DegToRad(dbar->angulo_graus);
		// define posição na barra
		str_ger.esq.posbar = 50;
		str_ger.esq.dxbar = 0;
		str_ger.esq.dybar = DXY_BAR;
		// insere Gerador em Monta
		// if (! monta->InsereGerador(str_ger)) return(false);
		if (!monta->InsereSuprimento(str_ger))
			return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaLigacao(void)
{
	if (!MontaCapserie())
		return (false);
	if (!MontaRegulador())
		return (false);
	if (!MontaTrecho())
		return (false);
	if (!MontaTrafo())
		return (false);
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaReator(void)
{
	int num_seq = 1;
	strREATOR str_rea;
	TDbar *dbar;

	// loop p/ todos TDbar em lisDBAR
	for (int n = 0; n < lisDBAR->Count; n++)
	{
		dbar = (TDbar*)lisDBAR->Items[n];
		// proteção: verifica se DBAR foi conectao por algum DLIN
		if (!dbar->conectada)
			continue;
		// verifica se Dbar está sendo excluída
		if (dbar->Excluida())
			continue;
		// verifica Dbar possui Reator
		if (!dbar->ExisteReator())
			continue;
		// preenche estrutura strREATOR
		str_rea.id = -1;
		str_rea.bar_id = dbar->id; // dbar->numero;
		str_rea.codigo = "R_" + IntToStr(num_seq++);
		str_rea.extern_id = "";
		str_rea.ligacao = lgTRIANG;
		str_rea.vnom = dbar->dgbt->tensao_kv;
		str_rea.q_mvar = fabs(dbar->capac_reator_mvar);
		str_rea.curva.tipica = true;
		str_rea.curva.id = -1;
		str_rea.curva.num_pat = 0;
		str_rea.esq.posbar = 50;
		str_rea.esq.dxbar = 0;
		str_rea.esq.dybar = 0;
		// insere Reator em Monta
		if (!monta->InsereReator(str_rea))
			return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaReatorEquivalente(void)
{
	// variáveis locais
	int num_seq = 1;
	strREATOR str_rea;
	TDinj *dinj;
	TDbar *dbar;

	// loop p/ todos TDinj em lisDINJ
	for (int n = 0; n < lisDINJ->Count; n++)
	{
		dinj = (TDinj*)lisDINJ->Items[n];
		// verifica se Dinj está sendo retirado
		if (dinj->Excluido())
			continue;
		// verifica se existe reator shunt
		if (!dinj->ExisteReator())
			continue;
		// determina Dbar de referência (barra controlada)
		if ((dbar = ExisteDbar(dinj->numero_barra)) == NULL)
		{
			plog->Add("Erro no cartão DINJ: Barra inexistente " +
				AnsiQuotedStr(IntToStr(dinj->numero_barra), '\''));
			return (false);
		}
		// proteção: verifica se DBAR foi conectao por algum DLIN
		if (!dbar->conectada)
			continue;
		// preenche estrutua strREATOR
		str_rea.id = -1;
		str_rea.bar_id = dbar->id; // dbar->numero;
		str_rea.codigo = "Reqv_" + IntToStr(num_seq++);
		str_rea.extern_id = "";
		str_rea.ligacao = lgTRIANG;
		str_rea.vnom = dbar->dgbt->tensao_kv;
		str_rea.q_mvar = fabs(dinj->shunt_mvar);
		str_rea.esq.posbar = 50;
		str_rea.esq.dxbar = 0;
		str_rea.esq.dybar = 0;
		// insere Reator em Monta
		if (!monta->InsereReator(str_rea))
			return (false);
	}
	return (true);
}

/*
 //-----------------------------------------------------------------------------
 bool __fastcall TPreMonta::MontaRede(void)
 {
 //variáveis locais
 strREDE str;

 //preenche estrutura strREDE
 str.rede_id   = rede_id;
 str.barini_id = -1;
 str.codigo    = "Rede_1";
 str.tipo_rede = redeSUB;
 str.estatica  = true;
 str.radial    = false;
 //insere Rede em Monta
 if (! monta->InsereRede(str)) return(false);
 return(true);
 }
 */
// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaRede(void)
{
	// variáveis locais
	strREDE str;

	// preenche estrutura strREDE
	str.rede_id = rede_id;
	str.mrede_id = -1;
	str.cluster_id = -1;
	str.barini_id = -1;
	str.codigo = "Rede_1";
	str.tiporede.segmento = redeSUB;
	str.estatica = true;
	str.radial = false;
	str.color = clBlack;
	str.area_esq.x1 = str.area_esq.y1 = -1;
	str.area_esq.x2 = str.area_esq.y2 = -1;
	// insere Rede em Monta
	if (!monta->InsereRede(str))
		return (false);
	return (true);
}

/*
 //-----------------------------------------------------------------------------
 bool __fastcall TPreMonta::MontaRegulador(void)
 {
 //variáveis locais
 int          num_seq = 1;
 strREGULADOR str_reg;
 TDbar        *dbar_ref;
 TDlin        *dlin;

 //loop p/ todos Dlin
 for (int n = 0; n < lisDLIN->Count; n++)
 {
 dlin = (TDlin*)lisDLIN->Items[n];
 //verifica se Dlin está sendo excluído
 if (dlin->Excluida()) continue;
 //verifica se Dlin é um Regulador
 if (! dlin->TipoRegulador()) continue;
 //determina Dbar controlada
 if ((dbar_ref = ExisteDbar(dlin->barra_controlada)) == NULL) continue;
 //preenche estrutura strTRECHO
 str_reg.id                  = -1;
 str_reg.rede_id             = rede_id;
 str_reg.bar1_id             = dlin->dbar1->id;  //dbar1->numero;
 str_reg.bar2_id             = dlin->dbar2->id;  //dbar2->numero;
 str_reg.codigo              = "RT_" + IntToStr(num_seq++);
 str_reg.by_pass             = false;
 str_reg.reversivel          = true;
 str_reg.ligacao             = lgEST_ISO;
 str_reg.snom                = (ValorInfinito(dlin->capacidade_normal)) ? Sbase : dlin->capacidade_normal;
 str_reg.var_tensao          = max(dlin->tap_min_pu, dlin->tap_max_pu) * 100;  //pu ==> %
 str_reg.num_passo           = (dlin->numero_steps) / 2;
 str_reg.ajuste_auto         = true;
 str_reg.cfg_fixo.passo      = 0;
 str_reg.cfg_auto.bar_ref_id = dbar_ref->numero;
 str_reg.cfg_auto.v_pu       = dbar_ref->tensao_pu;
 str_reg.esq.posbar1         = 50.;
 str_reg.esq.posbar2         = 50.;
 //insere Regulador em Monta
 if(! monta->InsereRegulador(str_reg)) return(false);
 }
 return(true);
 }
 */
// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaRegulador(void)
{
	// variáveis locais
	int num_seq = 1;
	strREGULADOR str_reg;
	TDbar *dbar_ref;
	TDlin *dlin;

	// loop p/ todos Dlin
	for (int n = 0; n < lisDLIN->Count; n++)
	{
		dlin = (TDlin*)lisDLIN->Items[n];
		// verifica se Dlin está sendo excluído
		if (dlin->Excluida())
			continue;
		// verifica se Dlin é um Regulador
		if (!dlin->TipoRegulador())
			continue;
		// determina Dbar controlada
		if ((dbar_ref = ExisteDbar(dlin->barra_controlada)) == NULL)
			continue;
		// preenche estrutura strREG
		str_reg.id = -1;
		str_reg.rede_id = rede_id;
		str_reg.bar1_id = dlin->dbar1->id; // dbar1->numero;
		str_reg.bar2_id = dlin->dbar2->id; // dbar2->numero;
		str_reg.codigo = "RT_" + IntToStr(num_seq++);
		str_reg.by_pass = false;
		//str_reg.sentido.reversivel = true;
		str_reg.ligacao = lgEST_ISO;
		str_reg.snom_mva = (ValorInfinito(dlin->capacidade_normal)) ? Sbase :
			dlin->capacidade_normal;
		str_reg.var_tensao = max(dlin->tap_min_pu, dlin->tap_max_pu) * 100; // pu ==> %
		str_reg.num_passo = (dlin->numero_steps) / 2;
		//str_reg.ajuste_auto = true;
		str_reg.cfg_fixo.passo = 0;
		str_reg.cfg_auto.bar_ref_id = dbar_ref->numero;
		str_reg.cfg_auto.v_pu = dbar_ref->tensao_pu;
		str_reg.esq.posbar1 = 50.;
		str_reg.esq.posbar2 = 50.;
		// insere Regulador em Monta
		if (!monta->InsereRegulador(str_reg))
			return (false);
	}
	return (true);
}

/*
 //-----------------------------------------------------------------------------
 bool __fastcall TPreMonta::MontaSuprimentoEquivalente(void)
 {
 //variáveis locais
 int           num_sup = 1;
 strSUPRIMENTO str_sup;
 TDinj         *dinj;
 TDbar         *dbar;

 //loop p/ todos TDinj em lisDINJ
 for (int n = 0; n < lisDINJ->Count; n++)
 {
 dinj = (TDinj*)lisDINJ->Items[n];
 //verifica se Dinj está sendo retirado
 if (dinj->Excluido()) continue;
 //determina Dbar
 if ((dbar = ExisteDbar(dinj->numero_barra)) == NULL)
 {
 plog->Add("Erro no cartão DINJ: Barra inexistente " +
 AnsiQuotedStr(IntToStr(dinj->numero_barra), '\''));
 return(false);
 }
 //preenche estrutura strSUPRIMENTO
 str_sup.sup_id    = -1;
 str_sup.bar_id    = dbar->id; //dbar->numero;
 str_sup.codigo    = "S_" + IntToStr(num_sup++);
 str_sup.extern_id = "";
 str_sup.tiposup   = supPQ;
 str_sup.smax      = 2. * dinj->injecao_mw;
 str_sup.zeq0.x    = 0.5;
 str_sup.zeq1.x    = 0.5;
 str_sup.pcc_3f.p  = 0.;
 str_sup.pcc_3f.q  = 0.;
 str_sup.pcc_ft.p  = 0.;
 str_sup.pcc_ft.q  = 0.;
 for (int np = 0; np < MAX_PAT; np++)
 {
 str_sup.p[np] = dinj->injecao_mw;
 str_sup.q[np] = dinj->injecao_mvar;
 str_sup.v[np] = 1.0;
 str_sup.t[np] = 0.;
 }
 str_sup.esq.posbar = 50;
 str_sup.esq.dxbar  = 0;
 str_sup.esq.dybar  = 0;
 //insere Suprimento em Monta
 if (! monta->InsereSuprimento(str_sup)) return(false);
 }
 return(true);
 }
 */
// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaSuprimentoEquivalente(void)
{
	// variáveis locais
	int num_sup = 1;
	strSUPRIMENTO str_sup;
	TDinj *dinj;
	TDbar *dbar;

	// loop p/ todos TDinj em lisDINJ
	for (int n = 0; n < lisDINJ->Count; n++)
	{
		dinj = (TDinj*)lisDINJ->Items[n];
		// verifica se Dinj está sendo retirado
		if (dinj->Excluido())
			continue;
		// determina Dbar
		if ((dbar = ExisteDbar(dinj->numero_barra)) == NULL)
		{
			plog->Add("Erro no cartão DINJ: Barra inexistente " +
				AnsiQuotedStr(IntToStr(dinj->numero_barra), '\''));
			return (false);
		}
		// proteção: verifica se DBAR foi conectao por algum DLIN
		if (!dbar->conectada)
			continue;
		// preenche estrutura strSUPRIMENTO
		str_sup.sup_id = -1;
		str_sup.bar_id = dbar->id; // dbar->numero;
		str_sup.codigo = "S_" + IntToStr(num_sup++);
		str_sup.extern_id = "";
		str_sup.tiposup = supPQ;
		str_sup.smax = 2. * dinj->injecao_mw;
		str_sup.zeq0.x = 0.5;
		str_sup.zeq1.x = 0.5;
		str_sup.pcc_3f.p = 0.;
		str_sup.pcc_3f.q = 0.;
		str_sup.pcc_ft.p = 0.;
		str_sup.pcc_ft.q = 0.;
		// monta curva com um único ponto
		str_sup.curva.id = -1;
		str_sup.curva.tipica = false;
		str_sup.curva.num_pat = 1;
		str_sup.curva.patamar[0].hm_ini.hora = 0;
		str_sup.curva.patamar[0].hm_ini.minuto = 0;
		str_sup.curva.patamar[0].hm_fim.hora = 23;
		str_sup.curva.patamar[0].hm_fim.minuto = 59;
		// str_sup.fornecimento[0].p_mw        = dbar->geracao_mw;
		// str_sup.fornecimento[0].q_mvar      = dbar->geracao_mvar;
		// str_sup.fornecimento[0].v_pu        = dbar->tensao_pu;
		// str_sup.fornecimento[0].t_rad       = DegToRad(dbar->angulo_graus);
		str_sup.curva.patamar[0].valor[indP] = dbar->geracao_mw;
		str_sup.curva.patamar[0].valor[indQ] = dbar->geracao_mvar;
		str_sup.curva.patamar[0].valor[indV] = dbar->tensao_pu;
		str_sup.curva.patamar[0].valor[indT] = DegToRad(dbar->angulo_graus);
		str_sup.esq.posbar = 50;
		str_sup.esq.dxbar = 0;
		str_sup.esq.dybar = 0;
		// insere Suprimento em Monta
		if (!monta->InsereSuprimento(str_sup))
			return (false);
	}
	return (true);
}

/*
 //-----------------------------------------------------------------------------
 bool __fastcall TPreMonta::MontaTrafo(void)
 {
 //variáveis locais
 int       num_seq = 1;
 strTRAFO  str_trf;
 TDlin     *dlin;
 TDbar     *dbar_ref;

 //loop p/ todos Dlin
 for (int n = 0; n < lisDLIN->Count; n++)
 {
 dlin = (TDlin*)lisDLIN->Items[n];
 //verifica se Dlin está sendo excluído
 if (dlin->Excluida()) continue;
 //verifica se Dlin é um Trafo
 if (! dlin->TipoTrafo()) continue;
 //determina Dbar controlada
 dbar_ref = ExisteDbar(dlin->barra_controlada);
 //preenche estrutura strTRAFO
 str_trf.trf_id        = -1;
 str_trf.rede_id       = rede_id;
 str_trf.bar1_id       = dlin->dbar1->id;  //dbar1->numero;
 str_trf.bar2_id       = dlin->dbar2->id;  //dbar2->numero;
 str_trf.codigo        = "TR_" + IntToStr(num_seq++);
 str_trf.snom          = (ValorInfinito(dlin->capacidade_normal)) ? Sbase : dlin->capacidade_normal;
 str_trf.auto_trafo    = false;
 str_trf.pri.ligacao   = lgTRIANG;
 str_trf.pri.defasagem = 0.;
 str_trf.pri.vnom      = dlin->dbar1->dgbt->tensao_kv;
 str_trf.pri.tap       = dlin->tap_pu * str_trf.pri.vnom;
 str_trf.pri.zterra.r  = 0.;
 str_trf.pri.zterra.x  = 0.;
 str_trf.sec.ligacao   = lgEST_AT;
 str_trf.sec.defasagem = 30.;
 str_trf.sec.vnom      = dlin->dbar2->dgbt->tensao_kv;
 str_trf.sec.tap       = str_trf.sec.vnom;
 str_trf.sec.zterra.r  = 40.;
 str_trf.sec.zterra.x  = 0.;
 str_trf.z1.r          = (dlin->resistencia_pu * str_trf.snom) / Sbase;
 str_trf.z1.x          = (dlin->reatancia_pu   * str_trf.snom) / Sbase;
 str_trf.z0.r          = str_trf.z1.r;
 str_trf.z0.x          = str_trf.z0.x;
 str_trf.perda_ferro   = 0.1;
 str_trf.esq.posbar1   = 50.;
 str_trf.esq.posbar2   = 50.;
 //dados de LTC
 str_trf.ltc.pri_sec   = ltcNAO_USADO;
 str_trf.ltc.bar_id    = -1;
 str_trf.ltc.v_pu[0]   = 0.;
 str_trf.ltc.v_pu[1]   = 0.;
 str_trf.ltc.v_pu[2]   = 0.;
 str_trf.ltc.v_pu[3]   = 0.;
 if (dbar_ref != NULL)
 {
 str_trf.ltc.pri_sec   = ltcPRIMARIO;
 str_trf.ltc.bar_id    = dbar_ref->numero;
 str_trf.ltc.v_pu[0]   = dbar_ref->tensao_pu;
 str_trf.ltc.v_pu[1]   = dbar_ref->tensao_pu;
 str_trf.ltc.v_pu[2]   = dbar_ref->tensao_pu;
 str_trf.ltc.v_pu[3]   = dbar_ref->tensao_pu;
 }
 //insere Trafo em Monta
 if (! monta->InsereTrafo(str_trf)) return(false);
 }
 return(true);
 }
 */
// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaTrafo(void)
{
	// variáveis locais
	int num_seq = 1;
	strTRAFO str_trf;
	TDlin *dlin;
	TDbar *dbar_pri, *dbar_sec, *dbar_ref;

	// loop p/ todos Dlin
	for (int n = 0; n < lisDLIN->Count; n++)
	{
		dlin = (TDlin*)lisDLIN->Items[n];
		// verifica se Dlin está sendo excluído
		if (dlin->Excluida())
			continue;
		// verifica se Dlin é um Trafo
		if (!dlin->TipoTrafo())
			continue;
		// determina Dbar controlada
		dbar_ref = ExisteDbar(dlin->barra_controlada);
		// preenche estrutura strTRAFO
		str_trf.trf_id = -1;
		str_trf.rede_id = rede_id;
		str_trf.bar1_id = dlin->dbar1->id; // dbar1->numero;
		str_trf.bar2_id = dlin->dbar2->id; // dbar2->numero;
		str_trf.codigo = "TR_" + IntToStr(num_seq++) + "_" + IntToStr(dlin->circuito);
		// str_trf.snom_mva      = (ValorInfinito(dlin->capacidade_normal)) ? Sbase : dlin->capacidade_normal;
		// considera valor nominal ventilação forçada (capacidade emergência)
		if (IsDoubleZero(dlin->capacidade_emergencia))
			dlin->capacidade_emergencia = dlin->capacidade_normal;
		str_trf.snom_mva = (ValorInfinito(dlin->capacidade_emergencia)) ? Sbase :
			dlin->capacidade_emergencia;
		str_trf.auto_trafo = false;
		// JCG 2011.12.06 - assume primário ligado na barra de maior tensão
		if (dlin->dbar1->dgbt->tensao_kv >= dlin->dbar2->dgbt->tensao_kv)
		{
			dbar_pri = dlin->dbar1;
			dbar_sec = dlin->dbar2;
		}
		else
		{
			dbar_pri = dlin->dbar2;
			dbar_sec = dlin->dbar1;
		}
		str_trf.pri.ligacao = lgTRIANG;
		str_trf.pri.defasagem = 0.;
		str_trf.pri.vnom = dbar_pri->dgbt->tensao_kv;
		str_trf.pri.tap = dlin->tap_pu * str_trf.pri.vnom;
		str_trf.pri.zterra.r = 0.;
		str_trf.pri.zterra.x = 0.;
		str_trf.sec.ligacao = lgEST_AT;
		str_trf.sec.defasagem = 30.;
		str_trf.sec.vnom = dbar_sec->dgbt->tensao_kv;
		str_trf.sec.tap = str_trf.sec.vnom;
		str_trf.sec.zterra.r = 0.;
		str_trf.sec.zterra.x = 0.;
		str_trf.z1.r = (dlin->resistencia_pu * str_trf.snom_mva) / Sbase;
		str_trf.z1.x = (dlin->reatancia_pu * str_trf.snom_mva) / Sbase;
		str_trf.z0.r = str_trf.z1.r;
		str_trf.z0.x = str_trf.z0.x;
		str_trf.perda_ferro = 0.1;
		str_trf.esq.posbar1 = 50.;
		str_trf.esq.posbar2 = 50.;
		// dados de LTC
		str_trf.ltc.tipo = ltcFIXO;
		str_trf.ltc.bar_id = -1;
		str_trf.ltc.v_pu = 1.;
		if (dbar_ref != NULL)
		{
			str_trf.ltc.tipo = ltcPRIMARIO;
			// str_trf.ltc.bar_id = dbar_ref->numero;
			str_trf.ltc.bar_id = dbar_ref->id;
			str_trf.ltc.v_pu = dbar_ref->tensao_pu;
		}
		// insere Trafo em Monta
		if (!monta->InsereTrafo(str_trf))
			return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::MontaTrecho(void)
{
	// variáveis locais
	strARRANJO1 str_arr;
	strTRECHO str_tre;
	TDlin *dlin;
	VTTrecho *trecho;
	int estado;

	// loop p/ todos Dlin
	for (int n = 0; n < lisDLIN->Count; n++)
	{
		dlin = (TDlin*)lisDLIN->Items[n];
		// verifica se Dlin está sendo excluído
		// if (dlin->Excluida()) continue;
		// verifica se Dlin é um Trecho
		if (!dlin->TipoTrecho())
			continue;
		// preenche estrutura strTRECHO
		str_tre.tre_id = -1;
		str_tre.rede_id = rede_id;
		str_tre.bar1_id = dlin->dbar1->id; // dbar1->numero;
		str_tre.bar2_id = dlin->dbar2->id; // dbar2->numero;
		// str_tre.codigo      = "T_" + IntToStr(num_seq++);
		str_tre.codigo = "T_" + IntToStr(dlin->dbar1->numero) + "_" + IntToStr(dlin->dbar2->numero)
			+ "_" + IntToStr(dlin->circuito); ;
		// assume comprimento de 1km
		str_tre.comp_km = 1.0;
		// define arranjo próprio do tipo arranjoPU_SB100
		str_arr.tipo_cfg = arranjoPU_SB100;
		str_arr.tipico = false;
		str_arr.data = DateOf(Now());
		str_arr.cfg_pu.sbase_mva = Sbase;
		str_arr.cfg_pu.vbase_kv = dlin->dbar1->dgbt->tensao_kv;
		if (IsDoubleZero(dlin->resistencia_pu, 1e-8) && IsDoubleZero(dlin->reatancia_pu, 1e-8))
		{
			return (true);
		}
		str_arr.cfg_pu.z1_pu.r = dlin->resistencia_pu;
		str_arr.cfg_pu.z1_pu.x = dlin->reatancia_pu;
		str_arr.cfg_pu.yc1_pu = dlin->susceptancia_shunt_mvar / Sbase;
		// assume z0 igual a z1 e yc0 igual a yc1
		str_arr.cfg_pu.z0_pu = str_arr.cfg_pu.z1_pu;
		str_arr.cfg_pu.yc0_pu = str_arr.cfg_pu.yc1_pu;
		// define corrente admissível
		if (ValorInfinito(dlin->capacidade_normal))
			dlin->capacidade_normal = Sbase;
		str_arr.cfg_pu.iadm_amp =
			(dlin->capacidade_normal / (sqrt(3.) * dlin->dbar1->dgbt->tensao_kv)) * 1000.; // A
		// define posição nos trechos
		str_tre.esq.posbar1 = 50.;
		str_tre.esq.posbar2 = 50.;
		// insere Trecho em Monta
		if ((trecho = (VTTrecho*)monta->InsereTrecho(str_tre, str_arr)) == NULL)
			return (false);
		// insere chaves nos extremos do trecho
		// MontaChaves(dlin, trecho);
		// insere estados de chaves nos extremos do trecho
		estado = (dlin->abertura_da_barra.AnsiCompareIC("D") == 0) ? 0 : 10;
		estado += (dlin->abertura_para_barra.AnsiCompareIC("D") == 0) ? 0 : 1;
		trecho->Tag = estado;
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::PontoDecimalExplicito(AnsiString valor)
{
	// variáveis locais
	int len = valor.Length();

	// verifica se o string possui o caractere '.' ou o caractere ','
	for (int n = 1; n <= len; n++)
	{
		if ((valor[n] == '.') || (valor[n] == ','))
			return (true);
	}
	return (false);
}

// -----------------------------------------------------------------------------
void __fastcall TPreMonta::RedefineTensaoDgbt_1KV(void)
{
	// variáveis locais
	TDlin *dlin;

	// loop p/ todos Dlin
	for (int n = 0; n < lisDLIN->Count; n++)
	{
		dlin = (TDlin*)lisDLIN->Items[n];
		// verifica se Dbar1 está associado a Dgbt de 1kV
		if (IsDoubleZero(dlin->dbar1->dgbt->tensao_kv - 1.))
		{
			dlin->dbar1->dgbt->tensao_kv = dlin->dbar2->dgbt->tensao_kv;
		}
		// verifica se Dbar2 está associado a Dgbt de 1kV
		if (IsDoubleZero(dlin->dbar2->dgbt->tensao_kv - 1.))
		{
			dlin->dbar2->dgbt->tensao_kv = dlin->dbar1->dgbt->tensao_kv;
		}
	}
}

// -----------------------------------------------------------------------------
void __fastcall TPreMonta::Reinicia(void)
{
	sucesso = true;
	LimpaTList(lisDBAR);
	LimpaTList(lisDINJ);
	LimpaTList(lisDLIN);
	LimpaTList(lisDGBT);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::RelacionaDbar_Dgbt(void)
{
	// variáveis locais
	TDbar *dbar;

	// loop p/ todos TDbar em lisDBAR_SEL
	for (int n = 0; n < lisDBAR_SEL->Count; n++)
	{
		dbar = (TDbar*)lisDBAR_SEL->Items[n];
		// proteção: verifica se DBAR foi conectao por algum DLIN
		if (!dbar->conectada)
			continue;
		// determina Dgbt
		if ((dbar->dgbt = ExisteDgbt(dbar->grupo_base_tensao)) == NULL)
		{
			plog->Add("Barra sem Grupo Base de Tensão" + AnsiQuotedStr
				(IntToStr(dbar->numero), '\''));
			return (false);
		}
	}
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::Sucesso(void)
{
	return (sucesso);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::TrataFimDados(void)
{
	// inclui mensagem de Log c/ dimensão da rede
	plog->Add("Número de registros DBAR: " + IntToStr(lisDBAR->Count));
	plog->Add("Número de registros DLIN: " + IntToStr(lisDLIN->Count));
	// assume erro na pré-montagem dos dados
	sucesso = false;
	// verifica se todos Gbt estão c/ tensão definida
	if (!ValidaDgbt())
		return (false);
	// associa DBAR com seu respectivo DGBT
	RelacionaDbar_Dgbt();
	// redefine tensão do DGBT com tensão 1 kV
	// RedefineTensaoDgbt_1KV();
	// ajuste inicial das coordenadas das Dbars
	if (coord_definida)
	{ // corrige valores das coordenadas p/ valores compatíveis c/ SINAP
		ConverteCoordenadaDbar();
	}
	else
	{ // define valores fictícios p/ coordendas das barras
		DefineCoordenadaDbar();
	}
	// passa os dados p/ Monta
	if (!MontaRede())
		return (false);
	if (!MontaBarra())
		return (false);
	if (!MontaEqbar())
		return (false);
	if (!MontaLigacao())
		return (false);
	// tratamento final dos dados
	if (!monta->TrataFimDeDados())
		return (false);
	// término da pré-montagem dos dados c/ sucesso
	sucesso = true;
	return (true);
}

// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::ValidaDgbt(void)
{
	// variáveis locais
	TDgbt *dgbt;

	// verifica se há Dgbts
	if (lisDGBT->Count == 0)
	{
		plog->Add("Erro: Não há nenhum 'Grupo Base de Tensão' definido");
		return (false);
	}
	// verifica se algum Dgbt está c/ Vnom nula
	for (int n = 0; n < lisDGBT->Count; n++)
	{
		dgbt = (TDgbt*)lisDGBT->Items[n];
		if (dgbt->tensao_kv == 0.)
		{
			plog->Add("Erro: Grupo Base de Tensão c/ valor nulo: " +
				AnsiQuotedStr(dgbt->grupo, '\''));
			return (false);
		}
	}
	return (true);
}

/*
 //-----------------------------------------------------------------------------
 bool __fastcall TPreMonta::ValorInfinito(AnsiString valor)
 {
 return((valor.AnsiCompareIC("8888") == 0)||
 (valor.AnsiCompareIC("9999") == 0));
 }
 */
// -----------------------------------------------------------------------------
bool __fastcall TPreMonta::ValorInfinito(double valor)
{
	return ((IsDoubleZero(valor - 8888.)) || (IsDoubleZero(valor - 9999.)));
}

// -----------------------------------------------------------------------------
// eof
