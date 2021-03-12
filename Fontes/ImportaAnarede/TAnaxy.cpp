// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAnaxy.h"
#include "..\Apl\VTApl.h"
#include "..\Edita\VTEdita.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipos.h"
#include "..\Topologia\VTTopologia.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"

// ---------------------------------------------------------------------------
VTAnaxy* NewObjAnaxy(VTApl *apl)
{
	return (new TAnaxy(apl));
}

// ---------------------------------------------------------------------------
__fastcall TAnaxy::TAnaxy(VTApl *apl)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	// cria objetos
	campos = new TStringList();
	lisALT = new TList();
	lisBAR = new TList();
	lisEQP = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TAnaxy::~TAnaxy(void)
{
	// destrói objetos criados
	if (campos)
	{
		delete campos;
		campos = NULL;
	}
	if (lisALT)
	{
		delete lisALT;
		lisALT = NULL;
	}
	if (lisBAR)
	{
		delete lisBAR;
		lisBAR = NULL;
	}
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::AlteraBarra(strCOMPONENTE *componente)
{
	// variáveis locais
	VTBarra *barra;

	// verifica se existe a Barra
	if ((barra = ExisteBarra(componente->ch1)) == NULL)
		return;
	// guarda extern id
	barra->Extern_id = componente->numero;
	// redefine coordenadas da Barra
	barra->esq.x1 = int(componente->px + DX);
	barra->esq.y1 = int(-componente->py + DY); // py deve ser negativo...
	barra->esq.x2 = barra->esq.x1;
	barra->esq.y2 = barra->esq.y1;
	// verifica se a barra é um ponto ou uma barra esticada
	if (componente->subtp.AnsiCompareIC("1") != 0)
	{ // determina direção e dimensão da barra
		switch (componente->rot % 256)
		{
		case 0:
			barra->esq.x2 += int((componente->rot - 0) / 256.) * DIM_BAR;
			break;
		case 1:
			barra->esq.y2 -= int((componente->rot - 1) / 256.) * DIM_BAR;
			break;
		case 2:
			barra->esq.x2 -= int((componente->rot - 2) / 256.) * DIM_BAR;
			break;
		case 3:
			barra->esq.y2 += int((componente->rot - 3) / 256.) * DIM_BAR;
			break;
		default:
			break;
		}
	}
	// insere barra na lista
	lisALT->Add(barra);
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::AlteraBarrasChaves(VTBarra *barra)
{
	// variáveis locais
	int ind_barra;
	VTBarra *barra_ch;
	VTLigacao *ligacao;
	TList *lisCHAVE = new TList();

	// lista chaves conectadas à barra
	redes->LisLigacao(lisCHAVE, barra);
	for (int nl = 0; nl < lisCHAVE->Count; nl++)
	{
		ligacao = (VTLigacao*)lisCHAVE->Items[nl];
		if (ligacao->Tipo() != eqptoCHAVE)
			continue;
		// determina outra barra
		ind_barra = ligacao->IndiceBarra(barra);
		barra_ch = ligacao->Barra(1 - ind_barra);
		// copia coordenadas esquematicas médias da barra para barra_ch
		barra_ch->esq.x1 = int((barra->esq.x1 + barra->esq.x2) / 2.);
		barra_ch->esq.y1 = int((barra->esq.y1 + barra->esq.y2) / 2.);
		barra_ch->esq.x2 = barra_ch->esq.x1;
		barra_ch->esq.y2 = barra_ch->esq.y1;
		// insere barra na lista
		lisALT->Add(barra_ch);
	}
	delete lisCHAVE;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::AlteraBarrasTrechos(void)
{
	// variáveis locais
	int ind_barra;
	double dx, dy;
	double delta = 0.1;
	VTBarra *barra_1, *barra_2;
	VTLigacao *ligacao;
	TList *lisTRECHO = new TList();

	// lista todos os trechos de rede
	redes->LisEqpto(lisTRECHO, eqptoTRECHO);
	for (int nl = 0; nl < lisTRECHO->Count; nl++)
	{
		ligacao = (VTLigacao*)lisTRECHO->Items[nl];
		barra_1 = ligacao->pbarra1;
		barra_2 = ligacao->pbarra2;
		dx = (barra_2->esq.x1 - barra_1->esq.x1);
		dy = (barra_2->esq.y1 - barra_1->esq.y1);
		// move barra1 a 10% pra "frente"
		barra_1->esq.x1 = barra_1->esq.x1 + (delta * dx);
		barra_1->esq.y1 = barra_1->esq.y1 + (delta * dy);
		barra_1->esq.x2 = barra_1->esq.x1;
		barra_1->esq.y2 = barra_1->esq.y1;
		// move barra2 a 10% pra "trás"
		barra_2->esq.x1 = barra_2->esq.x1 - (delta * dx);
		barra_2->esq.y1 = barra_2->esq.y1 - (delta * dy);
		barra_2->esq.x2 = barra_2->esq.x1;
		barra_2->esq.y2 = barra_2->esq.y1;
	}

	delete lisTRECHO;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::AlteraCapacitor(strCOMPONENTE *componente)
{
	// variáveis locais
	AnsiString cod;
	TList *lisCAP = new TList();
	VTEqbar *capacitor;

	// localiza capacitor
	cod = componente->ch2;
	LisEqbar(cod, eqptoCAPACITOR, lisCAP);
	for (int nc = 0; nc < lisCAP->Count; nc++)
	{
		capacitor = (VTEqbar*)lisCAP->Items[nc];
		// guarda extern id
		capacitor->Extern_id = componente->numero;
		// altera coordenada
		capacitor->esq.dxbar = 0; // int(abs(componente->px/DIM_BAR));
		capacitor->esq.dybar = 0; // int(abs(componente->py/DIM_BAR));
		// determina rotação
		switch (componente->rot)
		{
		case 0:
			capacitor->esq.dybar -= DIM_BAR;
			break;
		case 1:
			capacitor->esq.dxbar -= DIM_BAR;
			break;
		case 2:
			capacitor->esq.dybar += DIM_BAR;
			break;
		case 3:
			capacitor->esq.dxbar += DIM_BAR;
			break;
		default:
			capacitor->esq.dybar -= DIM_BAR;
			break;
		}
	}
	// destroi lista
	delete lisCAP;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::AlteraCarga(strCOMPONENTE *componente)
{
	// variáveis locais
	AnsiString cod;
	VTEqbar *carga;

	// localiza carga
	cod = componente->ch2;
	carga = ExisteEqbar(cod, eqptoCARGA);
	if (carga != NULL)
	{ // guarda extern id
		carga->Extern_id = componente->numero;
		// altera coordenada
		carga->esq.dxbar = 0; // int(abs(componente->px/DIM_BAR));
		carga->esq.dybar = 0; // int(abs(componente->py/DIM_BAR));
		// determina rotação
		switch (componente->rot)
		{
		case 0:
			carga->esq.dybar -= DIM_BAR;
			break;
		case 1:
			carga->esq.dxbar -= DIM_BAR;
			break;
		case 2:
			carga->esq.dybar += DIM_BAR;
			break;
		case 3:
			carga->esq.dxbar += DIM_BAR;
			break;
		default:
			carga->esq.dybar -= DIM_BAR;
			break;
		}
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TAnaxy::AlteraGerador(strCOMPONENTE *componente)
 {
 //variáveis locais
 AnsiString cod;
 VTEqbar    *gerador;

 //localiza gerador
 cod     = componente->ch2;
 gerador = ExisteEqbar(cod, eqptoGERADOR);
 if(gerador != NULL)
 {//guarda extern id
 gerador->Extern_id = componente->numero;
 //altera coordenada
 gerador->esq.dxbar = 0;//int(abs(componente->px/DIM_BAR));
 gerador->esq.dybar = 0;//int(abs(componente->py/DIM_BAR));
 //determina rotação
 switch(componente->rot)
 {
 case 0:  gerador->esq.dybar -= DIM_BAR; break;
 case 1:  gerador->esq.dxbar -= DIM_BAR; break;
 case 2:  gerador->esq.dybar += DIM_BAR; break;
 case 3:  gerador->esq.dxbar += DIM_BAR; break;
 default: gerador->esq.dybar -= DIM_BAR; break;
 }
 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TAnaxy::AlteraLigacao(strCOMPONENTE *componente)
{
	// variáveis locais
	AnsiString cod;
	VTBarra *barra1, *barra2;
	int circ;

	circ = StrToInt(componente->ch1);
	// localiza barras
	barra1 = ExisteBarra(componente->ch2);
	barra2 = ExisteBarra(componente->ch3);
	if ((barra1 == NULL) || (barra2 == NULL))
		return;
	// localiza ligacao
	ligacao = ExisteLigacao(barra1, barra2, circ);
	if (ligacao != NULL)
	{ // guarda extern id
		ligacao->Extern_id = componente->numero;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::AlteraLigacao(strLIGACOES *liga)
{
	// variáveis locais
	AnsiString cod;
	VTLigacao *ligacao;
	VTBarra *barra1, *barra2;
	VTEqbar *eqbar;

	// as possibilidades são
	// TRECHO: ID = linhas != -1; BARRA1 = liga->inic; BARRA2 = liga->fin
	// TRAFO:  ID = liga->inic  ; BARRA1 = NULL;       BARRA2 = liga->fin
	// ID = liga->fin   ; BARRA1 = liga->inic; BARRA2 = NULL;
	// EQBAR:  ID = liga->inic  ; BARRA1 = NULL;       BARRA2 = liga->fin
	// ID = liga->fin   ; BARRA1 = liga->inic; BARRA2 = NULL;

	// verifica se a ligação é um trecho
	if (liga->linhas != -1)
	{ // localiza barras
		barra1 = ExisteBarraExtId(liga->inic);
		barra2 = ExisteBarraExtId(liga->fin);
		// localiza trecho
		ligacao = ExisteLigacao(liga->linhas);
		// redefine posição do trecho
		RedefineLigacao(ligacao, barra1, liga->coni);
		RedefineLigacao(ligacao, barra2, liga->conf);
		return;
	}
	// não é trecho: pode ser trafo ou eqbar
	// localiza trafo
	if ((ligacao = ExisteLigacao(liga->fin)) != NULL)
	{ // localiza barra
		barra1 = ExisteBarraExtId(liga->inic);
		// redefine posição do trafo
		RedefineLigacao(ligacao, barra1, liga->coni);
	}
	else if ((ligacao = ExisteLigacao(liga->inic)) != NULL)
	{ // localiza barra
		barra2 = ExisteBarraExtId(liga->fin);
		// redefine posição do trafo
		RedefineLigacao(ligacao, barra2, liga->conf);
	}
	else
	{ // é eqbar
		// localiza barra e eqbar
		if ((barra1 = ExisteBarraExtId(liga->inic)) != NULL)
		{
			eqbar = ExisteEqbar(liga->fin, barra1);
			RedefineEqbar(eqbar, liga->coni);
		}
		else
		{ // localiza barra e eqbar
			barra2 = ExisteBarraExtId(liga->fin);
			eqbar = ExisteEqbar(liga->inic, barra2);
			RedefineEqbar(eqbar, liga->conf);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::AlteraLigacao(strLINHAS *linha)
{
	// variáveis locais
	VTLigacao *ligacao;
	VTBarra *barra1, *barra2;

	// localiza barras
	barra1 = ExisteBarraExtId(linha->de);
	barra2 = ExisteBarraExtId(linha->para);
	// localiza ligacao
	ligacao = ExisteLigacao(barra1, barra2, linha->ch1);
	// guarda extern_id
	if (ligacao != NULL)
		ligacao->Extern_id = IntToStr(linha->numero);
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::AlteraSuprimento(strCOMPONENTE *componente)
{
	// variáveis locais
	AnsiString cod;
	VTEqbar *suprimento;

	// localiza gerador
	cod = componente->ch2;
	suprimento = ExisteEqbar(cod, eqptoSUPRIMENTO);
	if (suprimento != NULL)
	{ // guarda extern id
		suprimento->Extern_id = componente->numero;
		// altera coordenada
		suprimento->esq.dxbar = 0; // int(abs(componente->px/DIM_BAR));
		suprimento->esq.dybar = 0; // int(abs(componente->py/DIM_BAR));
		// determina rotação
		switch (componente->rot)
		{
		case 0:
			suprimento->esq.dybar -= DIM_BAR;
			break;
		case 1:
			suprimento->esq.dxbar -= DIM_BAR;
			break;
		case 2:
			suprimento->esq.dybar += DIM_BAR;
			break;
		case 3:
			suprimento->esq.dxbar += DIM_BAR;
			break;
		default:
			suprimento->esq.dybar -= DIM_BAR;
			break;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::ClonaBarra(VTBarra *barra)
{
	// variaveis locais
	VTBarra *clone;

	// cria clone da barra
	clone = barra->Clone();
	barra->Obj = clone;
	clone->Id = FalsoId();
	clone->Codigo = "B_" + barra->Codigo;
	redes->InsereBarra(clone);
}

// ---------------------------------------------------------------------------
bool __fastcall TAnaxy::Comentario(AnsiString txt)
{
	if (txt[1] == '!')
		return (true);
	return (false);
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::CriaChave(VTBarra *barra, VTLigacao *ligacao)
{
	// variáveis locais
	VTChave *chave;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));

	try
	{ // cria obj chave
		chave = DLL_NewObjChave(patamares->NumPatamar());
		chave->Id = FalsoId();
		chave->Extern_id = "";
		chave->rede = ligacao->rede;
		chave->TipoChave = tipos->ExisteTipoChave(chaveDJ);
		chave->EloFusivel = "";
		chave->EstadoNormal = chave->Estado;
		chave->ProtecaoAtiva = false;
		chave->Telecomandada = false;
		chave->TecnoComunicacao = comNAO_DISPONIVEL;
		chave->Show[eqp_VISIBLE] = ligacao->Show[eqp_VISIBLE];
		// define barras e insere na rede
		chave->DefineObjBarra(barra, (VTBarra*)barra->Obj);
		ligacao->rede->InsereLigacao(chave);
		if (barra == ligacao->pbarra1)
		{ // determina estado da chave na barra 1
			chave->Estado = (ligacao->Tag >= 10) ? chvFECHADA : chvABERTA;
			chave->esq.posbar1 = ligacao->esq.posbar1;
			chave->esq.posbar2 = 50.;
		}
		else
		{ // determina estado da chave na barra 2
			chave->Estado = ((ligacao->Tag % 10) == 1) ? chvFECHADA : chvABERTA;
			chave->esq.posbar1 = ligacao->esq.posbar2;
			chave->esq.posbar2 = 50.;
		}
	}
	catch (Exception &e)
	{
		if (chave)
			delete chave;
		return;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TAnaxy::Executa(TStrings *Lines)
{
	// variáveis locais
	AnsiString line;
	int estado = estINICIO;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));
	strCOMPONENTE componente;
	strLIGACOES liga;
	strLINHAS linha;

	// inicia lisBAR com as barras da rede
	lisBAR->Clear();
	redes->LisBarra(lisBAR);
	// interpreta todas as linhas
	for (int n = 0; (n < Lines->Count) && (estado != estFIM); n++)
	{ // verifica se é linha de comentario
		if (Comentario(Lines->Strings[n]))
		{
			if (Lines->Strings[n].Pos("- Componentes") > 0)
				estado = estCOORD;
			if (Lines->Strings[n].Pos("- Linhas") > 0)
				estado = estLINHA;
			if (Lines->Strings[n].Pos("- Ligacoes") > 0)
				estado = estLIGA;
			if (Lines->Strings[n].Pos("- Posição") > 0)
				estado = estFIM;
			continue;
		}
		// corta primeiro caractere
		line = Lines->Strings[n].SubString(2, Lines->Strings[n].Length());
		switch (estado)
		{
		case estINICIO: // verifica se a linha contém palavra chave que indica início de dados
			// if (Lines->Strings[n].AnsiPos("Componentes") > 0) estado = estCOORD;
			break;
		case estCOORD:
			// extrai coordenadas da barra
			if (ExtraiStrings(line, " ", campos) < 14)
				continue;
			LeComponente(&componente);
			if (componente.tipo == 1)
				AlteraBarra(&componente);
			// else if(componente.tipo == 4)  AlteraGerador(&componente);
			else if (componente.tipo == 4)
				AlteraSuprimento(&componente);
			else if (componente.tipo == 5)
				AlteraCarga(&componente);
			else if (componente.tipo == 9)
				AlteraLigacao(&componente); // trafo
			else if (componente.tipo == 23)
				AlteraCapacitor(&componente);
			// else if(componente.tipo == 25)  AlteraLigacao(&componente);//texto
			break;
		case estLINHA:
			// extrai dados de linha
			if (ExtraiStrings(line, " ", campos) < 9)
				continue;
			LeLinha(&linha);
			AlteraLigacao(&linha); // apenas guarda id dos trechos
			break;
		case estLIGA:
			// extrai dados de ligação
			if (ExtraiStrings(line, " ", campos) < 9)
				continue;
			LeLigacoes(&liga);
			AlteraLigacao(&liga);
			break;
		case estFIM: // fim de leitura dos dados
			break;
		default:
			break;
		}
	}
	/* //bloco comentado
	 // remove barras que não tiveram seu esquemático redefinido
	 // (exceto se nenhuma barra foi alterada)
	 if (lisBAR->Count > lisALT->Count)
	 {
	 lisEQP->Clear();
	 lisEQP->Assign(lisBAR, laXor, lisALT);
	 edita->RetiraLisEqpto(lisEQP);
	 }
	 */
	// deixa invisivel tudo que nao foi desenhado
	if (lisBAR->Count > lisALT->Count)
	{
		lisEQP->Clear();
		lisEQP->Assign(lisBAR, laXor, lisALT);
		Visivel(false, lisEQP);
	}
	// insere chaves nos extremos de cada trecho
	InsereChaves();
	// atualiza tela
	topologia->Reconfigura(redes);
	// calcula área das Redes
	redes->CalculaArea();
	grafico->Redraw();
	return (true);
}
/*
 //---------------------------------------------------------------------------
 void __fastcall TAnaxy::ExtraiCampos(AnsiString txt, TStringList *campos)
 {
 //variáveis locais
 #define EOS '\0'
 char    separador = ' ';
 char    *p_ini, *p_fim;

 //insere caractere separador no final de ttxt
 if (txt[txt.Length()] != separador) txt += separador;
 //reinicia StringList campos
 campos->Clear();
 //extrai os campos delimitados por ' ' em txt
 p_ini = txt.c_str();
 while (*p_ini != EOS)
 {
 while ((*p_ini != EOS)&&(*p_ini == separador)) p_ini++;
 if (*p_ini == EOS) return;
 p_fim = p_ini;
 while (*p_fim != separador) p_fim++;
 *p_fim = EOS; //insere delimitador de string
 campos->Add(AnsiString(p_ini).Trim());
 //copia em pstr o substring p_ini a p_fim-1
 *p_fim = separador;     //restaura separador no string original
 p_ini = p_fim;
 }
 }
 */

// ---------------------------------------------------------------------------
VTBarra* __fastcall TAnaxy::ExisteBarra(AnsiString cod_bar)
{
	// variáveis locais
	VTBarra *barra;

	// teste
	if (cod_bar.AnsiCompareIC("96800") == 0)
	{
		int a = 0;
	}

	for (int nb = 0; nb < lisBAR->Count; nb++)
	{ // localiza barra pelo código, antes dos :
		barra = (VTBarra*)lisBAR->Items[nb];
		if (ExtraiStrings(barra->Codigo, ":", campos) < 2)
			continue;
		if (campos->Strings[0].CompareIC(cod_bar) == 0)
			return (barra);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TAnaxy::ExisteBarraExtId(AnsiString ext_id)
{
	// variáveis locais
	VTBarra *barra;

	for (int nb = 0; nb < lisBAR->Count; nb++)
	{ // localiza barra pelo ext_id
		barra = (VTBarra*)lisBAR->Items[nb];
		if (barra->Extern_id.AnsiCompareIC(ext_id) == 0)
			return (barra);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTEqbar* __fastcall TAnaxy::ExisteEqbar(AnsiString ext_id, VTBarra *barra)
{
	// variáveis locais
	VTEqbar *eqbar;
	TList *lisEQB;

	// proteção
	if (barra == NULL)
		return (NULL);

	lisEQB = barra->LisEqbar();
	for (int nb = 0; nb < lisEQB->Count; nb++)
	{ // localiza eqbar pelo ext_id
		eqbar = (VTEqbar*)lisEQB->Items[nb];
		if (eqbar->Extern_id.AnsiCompareIC(ext_id) == 0)
			return (eqbar);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTEqbar* __fastcall TAnaxy::ExisteEqbar(AnsiString cod, int tipo)
{
	// variáveis locais
	VTBarra *barra;
	VTEqbar *eqbar;
	TList *lisTMP;

	// localiza barra
	barra = ExisteBarra(cod);
	if (barra == NULL)
		return (NULL);
	// lista eqbar da barra
	lisTMP = barra->LisEqbar();
	// localiza eqbar pela barra
	for (int nb = 0; nb < lisTMP->Count; nb++)
	{
		eqbar = (VTEqbar*)lisTMP->Items[nb];
		if (eqbar->Tipo() == tipo)
			return (eqbar);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTLigacao* __fastcall TAnaxy::ExisteChave(AnsiString ext_id, VTBarra *barra)
{
	// variáveis locais
	VTLigacao *ligacao;
	VTLigacao *chave;

	// proteção
	if (barra == NULL)
		return (NULL);
	// limpa lista
	lisEQP->Clear();
	redes->LisLigacao(lisEQP);
	// localiza ligacao pelo ext_id
	for (int nb = 0; nb < lisEQP->Count; nb++)
	{
		ligacao = (VTLigacao*)lisEQP->Items[nb];
		if (ligacao->Extern_id.AnsiCompareIC(ext_id) == 0)
		{ // localiza chave ligada à barra e uma das barras da ligação
			chave = ligacao->rede->ExisteLigacao(ligacao->pbarra1, barra);
			if (chave == NULL)
				chave = ligacao->rede->ExisteLigacao(ligacao->pbarra2, barra);
			return (chave);
		}
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTLigacao* __fastcall TAnaxy::ExisteLigacao(AnsiString ext_id)
{
	// variáveis locais
	VTLigacao *ligacao;

	// limpa lista
	lisEQP->Clear();
	redes->LisLigacao(lisEQP);
	// localiza ligacao pelo ext_id
	for (int nb = 0; nb < lisEQP->Count; nb++)
	{
		ligacao = (VTLigacao*)lisEQP->Items[nb];
		if (ligacao->Extern_id.AnsiCompareIC(ext_id) == 0)
			return (ligacao);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTLigacao* __fastcall TAnaxy::ExisteLigacao(VTBarra *barra1, VTBarra *barra2, int num)
{
	// variáveis locais
	VTLigacao *ligacao;
	AnsiString circ;
	int ini, num_circ;

	// proteção
	if (barra1 == NULL)
		return (NULL);
	if (barra2 == NULL)
		return (NULL);
	try
	{ // limpa lista
		lisEQP->Clear();
		redes->LisLigacao(lisEQP);
		// localiza ligacao pelas barras
		for (int nb = 0; nb < lisEQP->Count; nb++)
		{
			ligacao = (VTLigacao*)lisEQP->Items[nb];
			if ((ligacao->ConectaBarra(barra1)) && (ligacao->ConectaBarra(barra2)))
			{ // verifica se tem num
				if (num < 0)
					return (ligacao);
				// verifica numero do circuito
				ini = ligacao->Codigo.Length();
				circ = ligacao->Codigo.SubString(ini, 1);
				num_circ = StrToInt(circ);
				if (num_circ == num)
					return (ligacao);
			}
		}
	}
	catch (Exception &e)
	{
		return (NULL);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::InsereChaves(void)
{
	// variáveis locais
	int ind_barra;
	VTBarra *barra_1, *barra_2;
	VTLigacao *ligacao;
	TList *lisTRECHO = new TList();

	// lista todos os trechos de rede
	redes->LisEqpto(lisTRECHO, eqptoTRECHO);
	for (int nl = 0; nl < lisTRECHO->Count; nl++)
	{
		ligacao = (VTLigacao*)lisTRECHO->Items[nl];
		// determina barras
		barra_1 = ligacao->pbarra1;
		barra_2 = ligacao->pbarra2;
		// clona barras
		ClonaBarra(barra_1);
		ClonaBarra(barra_2);
		// redefine coordenadas das barras clone
		RedefineBarrasClone(ligacao);
		// insere chave em cada extremo da ligação
		CriaChave(barra_1, ligacao);
		CriaChave(barra_2, ligacao);
		// associa trechos às barras clone
		ligacao->DefineObjBarra((VTBarra*)barra_1->Obj, (VTBarra*)barra_2->Obj);
	}
	// destroi lista
	delete lisTRECHO;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::LeComponente(strCOMPONENTE *componente)
{
	// variáveis locais
	int ind = 0;

	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	componente->numero = (campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	componente->tipo = StrToInteger(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	componente->rot = StrToInteger(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	componente->hac = StrToInteger(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	componente->px = StrToDouble(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	componente->py = StrToDouble(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	componente->cor = StrToInteger(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	componente->sl = StrToInteger(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	componente->subtp = (campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	componente->ch1 = (campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	componente->ch2 = (campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	componente->ch3 = (campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	componente->ch4 = (campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	componente->descrip = (campos->Strings[ind]);
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::LeLigacoes(strLIGACOES *liga)
{
	// variáveis locais
	int ind = 0;

	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	liga->numero = StrToInteger(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	liga->flags = StrToInteger(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	liga->cor = StrToInteger(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	liga->inic = (campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	liga->coni = StrToInteger(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	liga->fin = (campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	liga->conf = StrToInteger(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	liga->linhas = StrToInteger(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	liga->nqb = StrToInteger(campos->Strings[ind]);

}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::LeLinha(strLINHAS *linha)
{
	// variáveis locais
	int ind = 0;

	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	linha->numero = StrToInteger(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	linha->tipo = StrToInteger(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	linha->hac = StrToInteger(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	linha->sl = StrToInteger(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	linha->de = (campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	linha->para = (campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	linha->ch1 = StrToInteger(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	linha->nsg = StrToInteger(campos->Strings[ind]);

	ind++;
	while (campos->Strings[ind].IsEmpty())
		ind++;
	if (ind >= campos->Count)
		return;
	linha->segmentos = StrToInteger(campos->Strings[ind]);
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::LisEqbar(AnsiString cod, int tipo, TList *lisEQB)
{
	// variáveis locais
	VTBarra *barra;
	VTEqbar *eqbar;
	TList *lisTMP;

	// localiza barra
	barra = ExisteBarra(cod);
	if (barra == NULL)
		return;
	// lista eqbar da barra
	lisTMP = barra->LisEqbar();
	// localiza eqbar pela barra
	for (int nb = 0; nb < lisTMP->Count; nb++)
	{
		eqbar = (VTEqbar*)lisTMP->Items[nb];
		if (eqbar->Tipo() != tipo)
			continue;
		// insere todos Eqbar na lista
		if (lisEQB->IndexOf(eqbar) < 0)
			lisEQB->Add(eqbar);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::RedefineBarrasClone(VTLigacao *ligacao)
{
	// variáveis locais
	VTBarra *clone_1, *clone_2;
	double dx, dy;
	double delta = 0.1;
	int x1, y1, x2, y2;

	// guarda barras clones
	clone_1 = (VTBarra*)ligacao->pbarra1->Obj;
	clone_2 = (VTBarra*)ligacao->pbarra2->Obj;
	// determina coordenadas da ligação
	ligacao->CoordenadasEsquematico(x1, y1, x2, y2);
	// determina dx e dy
	dx = x2 - x1;
	dy = y2 - y1;
	// move clone_1 a 10% pra "frente" de x
	clone_1->esq.x1 = x1 + (delta * dx);
	clone_1->esq.y1 = y1 + (delta * dy);
	// move clone_2 a 10% pra "trás" de x
	clone_2->esq.x1 = x2 - (delta * dx);
	clone_2->esq.y1 = y2 - (delta * dy);
	// copia coordenadas x2 e y2 nos 2 clones
	clone_1->esq.x2 = clone_1->esq.x1;
	clone_1->esq.y2 = clone_1->esq.y1;
	clone_2->esq.x2 = clone_2->esq.x1;
	clone_2->esq.y2 = clone_2->esq.y1;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::RedefineEqbar(VTEqbar *eqbar, int valor)
{
	// variáveis locais
	VTBarra *barra;
	int dim; // dimensão
	double pos; // posição

	// A posição de um eqbar em relação à barra é dada da seguinte forma:
	// todas as barras possuem comprimento proporcional a 256 (na função
	// TAnaxy::AlteraBarra(strCOMPONENTE *componente) foi dividido por 256 e
	// multiplicado por DIM_BAR). O valor da proporção do comprimento da barra pode
	// ser chamado dimensão. Se uma barra tem dimensão 1 (isto é, 256/256 x DIM_BAR), a
	// ligação pode estar em 3 posições; se tem dimensão 2, em 6 posições; e assim
	// por diante, mantendo o padrão (3 x dimensão).
	// Como o Sinap define a posição do eqbar como um percentual em relação à x1
	// ou y1 da barra (de 0 a 100%), a fórmula para determinar a posição é:
	// pos_sinap = (100. / (((dim / DIM_BAR) * 3.) - 1.)) * (pos_anarede - 1);

	// proteção
	if (eqbar == NULL)
		return;

	barra = eqbar->pbarra;
	dim = (barra->Vertical() ? fabs((long double)barra->esq.y2 - barra->esq.y1) :
		fabs((long double)barra->esq.x2 - barra->esq.x1));
	if (IsDoubleZero(dim))
		pos = 50.;
	else
		pos = (100. / (((dim / DIM_BAR) * 3.) - 1.)) * (valor - 1.);
	eqbar->esq.posbar = pos;
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::RedefineLigacao(VTLigacao *ligacao, VTBarra *barra, int valor)
{
	// variáveis locais
	int dim; // dimensão
	double pos; // posição

	// A posição de uma ligação em relação às barras é dada da seguinte forma:
	// todas as barras possuem comprimento proporcional a 256 (na função
	// TAnaxy::AlteraBarra(strCOMPONENTE *componente) foi dividido por 256 e
	// multiplicado por DIM_BAR). O valor da proporção do comprimento da barra pode
	// ser chamado dimensão. Se uma barra tem dimensão 1 (isto é, 256/256 x DIM_BAR), a
	// ligação pode estar em 3 posições; se tem dimensão 2, em 6 posições; e assim
	// por diante, mantendo o padrão (3 x dimensão).
	// Como o Sinap define a posição da ligação como um percentual em relação à x1
	// ou y1 da barra (de 0 a 100%), a fórmula para determinar a posição é:
	// pos_sinap = (100. / (((dim / DIM_BAR) * 3.) - 1.)) * (pos_anarede - 1);

	// proteção
	if (barra == NULL)
		return;
	if (ligacao == NULL)
		return;

	dim = (barra->Vertical() ? fabs((long double)barra->esq.y2 - barra->esq.y1) :
		fabs((long double)barra->esq.x2 - barra->esq.x1));
	if (IsDoubleZero(dim))
		pos = 50.;
	else
		pos = (100. / (((dim / DIM_BAR) * 3.) - 1.)) * (valor - 1.);
	ligacao->DefinePosBar(barra, pos);
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::Visivel(bool mostra)
{
	// variáveis locais
	VTEqpto *eqpto;

	lisEQP->Clear();
	redes->LisEqpto(lisEQP);
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		eqpto->Show[eqp_VISIBLE] = mostra;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAnaxy::Visivel(bool mostra, TList *lisEQPT)
{
	// variáveis locais
	VTBarra *barra;
	VTEqpto *eqpto;
	TList *lisTMP, *lisEQB;

	for (int n = 0; n < lisEQPT->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQPT->Items[n];
		eqpto->Show[eqp_VISIBLE] = mostra;
		if (eqpto->TipoBarra())
		{ // altera para visivel/invisivel a barra, os eqbar e as ligações dela
			barra = (VTBarra*)eqpto;
			lisEQB = barra->LisEqbar();
			if (lisEQB->Count > 0)
				Visivel(mostra, lisEQB);
			lisTMP = new TList();
			lisTMP->Clear();
			redes->LisLigacao(lisTMP, barra);
			if (lisTMP->Count > 0)
				Visivel(mostra, lisTMP);
			delete lisTMP;
		}
	}
}

// -----------------------------------------------------------------------------
// eof
