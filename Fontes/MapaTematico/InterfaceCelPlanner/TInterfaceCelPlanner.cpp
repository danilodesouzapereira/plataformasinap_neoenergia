// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.hpp>
#include "TFormEdtEqptosCodigo.h"
#include "TInterfaceCelPlanner.h"
#include "TCPCapacitor.h"
#include "TCPBarra.h"
#include "TCPCarga.h"
#include "TCPChave.h"
#include "TCPEqpto.h"
#include "TCPGerador.h"
#include "TCPRegulador.h"
#include "TCPSubestacao.h"
#include "TCPTrafo.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Radial\VTRadial.h"
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTSecundario.h"
#include "..\Radial\VTSubestacao.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Radial.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTInterfaceCelPlanner* __fastcall NewObjInterfaceCelPlanner(VTApl *apl)
{
	return (new TInterfaceCelPlanner(apl));
}
//---------------------------------------------------------------------------
static int __fastcall ComparaCodigoEqpto(void *item1, void *item2)
{//função para ordenar lista
	//variaveis locais
	VTEqpto *c1 = (VTEqpto*) item1;
	VTEqpto *c2 = (VTEqpto*) item2;

	return (c1->Codigo.AnsiCompareIC(c2->Codigo));
}
// ---------------------------------------------------------------------------
__fastcall TInterfaceCelPlanner::TInterfaceCelPlanner(VTApl *apl)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	// cria objetos
	lisEQP = new TList();
	lisBARRAS = new TList();
	lisLIG = new TList();
	lisSEDS = new TList();
	lisRedesCarregadas = new TList(); // FKM 2016.04.27
	radial = DLL_NewObjRadial(apl);
	// inicializa o maioID
	maiorId = 0;
	// inicializa cfg
	cfg.exporta_cargas = true;
	cfg.exporta_chaves = true;
	cfg.exporta_geradores = true;
	cfg.exporta_religadores = true;
	cfg.exporta_reguladores = true;
	cfg.exporta_capacitores = true;
	cfg.exporta_subestacao = true;
	cfg.exporta_seccionadoras = true;
	cfg.exporta_trafos = true;
	cfg.utm = false;
	// cria uma lsita de strings
	lines_eqptos = new TStringList;
}

// ---------------------------------------------------------------------------
__fastcall TInterfaceCelPlanner::~TInterfaceCelPlanner(void)
{
	// destrói objetos
	if (radial)
	{
		delete radial;
		radial = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	if (lisBARRAS)
	{
		delete lisBARRAS;
		lisBARRAS = NULL;
	}
	if (lisLIG)
	{
		delete lisLIG;
		lisLIG = NULL;
	}
	if (lisSEDS)
	{
		delete lisSEDS;
		lisSEDS = NULL;
	}

	if (lisRedesCarregadas)
	{
		delete lisRedesCarregadas;
		lisRedesCarregadas = NULL;
	} // FKM 2016.04.27
}

// ---------------------------------------------------------------------------
strCPAntena __fastcall TInterfaceCelPlanner::AntenaDefault(void)
{ // variaveis locais
	strCPAntena antena;

	antena.ota = 0;
	antena.tal = 0;
	antena.typ = "TR";
	antena.aht = 5;
	antena.pwr = 1;
	antena.txl = 1;
	antena.rxl = 1;
	antena.rxs = 0;
	antena.rnf = 8;
	antena.trd = 0.05;
	antena.tru = 0.05;
	antena.drd = 0;
	antena.dru = 0;
	antena.prb = 100;
	antena.ant = "AntDef";
	antena.dst = 0;
	antena.azt = 0;
	antena.azm = 0;
	antena.ind = 0;
	antena.inu = 0;
	antena.celld = "";
	antena.cellu = "";

	return (antena);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TInterfaceCelPlanner::AntenaDefaultAsString(void)
{ // variáveis locais
	AnsiString separador = "\t";
	AnsiString txt;
	strCPAntena antena;

	antena = AntenaDefault();
	txt = IntToStr(antena.ota) + separador;
	txt += IntToStr(antena.tal) + separador;
	txt += antena.typ + separador;
	txt += IntToStr(antena.aht) + separador;
	txt += IntToStr(antena.pwr) + separador;
	txt += IntToStr(antena.txl) + separador;
	txt += IntToStr(antena.rxl) + separador;
	txt += " " + separador;
	txt += DoubleToStr("%2.1f", antena.rnf) + separador;
	txt += DoubleToStr("%2.1f", antena.trd) + separador;
	txt += DoubleToStr("%2.1f", antena.tru) + separador;
	txt += " " + separador;
	txt += " " + separador;
	txt += IntToStr(antena.prb) + separador;
	txt += antena.ant + separador;
	txt += antena.celld + separador;
	txt += antena.cellu + separador;
	txt += " " + separador; // DoubleToStr("%2.1f", antena.dst) + separador;
	txt += " " + separador; // DoubleToStr("%2.1f", antena.azt) + separador;
	txt += " " + separador; // DoubleToStr("%2.1f", antena.azm) + separador;
	txt += " " + separador; // DoubleToStr("%2.1f", antena.ind) + separador;
	txt += " " + separador; // DoubleToStr("%2.1f", antena.inu) + separador;

	return (txt);
}

// ---------------------------------------------------------------------------
void __fastcall TInterfaceCelPlanner::CriaCabecalho(int nSb)
{ // variáveis locais
	AnsiString separador = "\t";
	AnsiString txt;

	// adiciona o nome
	lines->Add("[CelPlan DT SBC]");
	// quebra mais uma linha
	lines->Add("");
	// unidade
	txt = "Uni" + separador;
	txt += "MT";
	lines->Add(txt);
	// descobrir o que é
	txt = "Cst" + separador;
	txt += "0" + separador;
	txt += "0" + separador;
	txt += "0" + separador;
	txt += "0" + separador;
	txt += "0" + separador;
	txt += "0" + separador;
	lines->Add(txt);
	// quebra mais uma linha
	lines->Add("");
	// acho que é descrição
	txt = "Dsc" + separador;
	txt += "Ud1" + separador;
	txt += "Ud2" + separador;
	txt += "Ud3" + separador;
	txt += "Ud4" + separador;
	txt += "Ud5" + separador;
	txt += "Ud6" + separador;
	txt += "Ud7" + separador;
	txt += "Ud8" + separador;
	txt += "nSB" + separador;
	lines->Add(txt);
	// campos do usuario
	txt = "Subscriber CelPlan" + separador;
	txt += "UserData1" + separador;
	txt += "UserData2" + separador;
	txt += "UserData3" + separador;
	txt += "UserData4" + separador;
	txt += "UserData5" + separador;
	txt += "UserData6" + separador;
	txt += "UserData7" + separador;
	txt += "UserData8" + separador;
	txt += IntToStr(nSb) + separador;
	lines->Add(txt);
	// quebra mais uma linha
	lines->Add("");
	// cabecalho da tabela
	txt = "Sbc" + separador;
	txt += "Ud1" + separador;
	txt += "Ud2" + separador;
	txt += "Ud3" + separador;
	txt += "Ud4" + separador;
	txt += "Ud5" + separador;
	txt += "Ud6" + separador;
	txt += "Ud7" + separador;
	txt += "Ud8" + separador;
	txt += "Act" + separador;
	txt += "Phs" + separador;
	txt += "Are" + separador;
	txt += "Fg1" + separador;
	txt += "Fg2" + separador;
	txt += "Fg3" + separador;
	txt += "Fg4" + separador;
	txt += "Fg5" + separador;
	txt += "Fg6" + separador;
	txt += "Fg7" + separador;
	txt += "Fg8" + separador;
	txt += "Col" + separador;
	if (cfg.utm)
	{
		txt += "Eas" + separador;
		txt += "Nor" + separador;
	}
	else
	{
		txt += "Lat" + separador;
		txt += separador;
		txt += separador;
		txt += separador;
		txt += "Lon" + separador;
		txt += separador;
		txt += separador;
		txt += separador;
	}
	txt += "Ota" + separador;
	txt += "Tal" + separador;
	txt += "Typ" + separador;
	txt += "Aht" + separador;
	txt += "Pwr" + separador;
	txt += "Txl" + separador;
	txt += "Rxl" + separador;
	txt += "Rxs" + separador;
	txt += "Rnf" + separador;
	txt += "TrD" + separador;
	txt += "TrU" + separador;
	txt += "DrD" + separador;
	txt += "DrU" + separador;
	txt += "Prb" + separador;
	txt += "Ant" + separador;
	txt += "CellD" + separador;
	txt += "CellU" + separador;
	txt += "Dst" + separador;
	txt += "AzT" + separador;
	txt += "AzM" + separador;
	txt += "InD" + separador;
	txt += "InU" + separador;
	lines->Add(txt);
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceCelPlanner::ExportaArqCelPlanner(TStrings *lines)
{ // variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	int nSb = 0;
	bool sucesso = true;

	//primeiro verifica se possui nomes repetidos
	if (ValidaCodigosEqptos())
	{
		// obtem a lista de redes carregadas
		// redes->LisRedeCarregada(lisRedesCarregadas);
		// salva ponteiro para objeto
		this->lines = lines;
		// exporta blocos de dados do arquivo CelPlanner
		lines->Clear();
		lines_eqptos->Clear();
		// Gera lista de todas barras
		lisBARRAS->Clear();
		redes->LisBarra(lisBARRAS);
		// gera a lista de todas chaves
		lisLIG->Clear();
		redes->LisLigacao(lisLIG, eqptoCHAVE);
		// gera os blocos
		if (cfg.exporta_cargas)
		{
			if (!ExportaBlocoCargas(lisBARRAS))
			{
				return (false);
			}
		}
		if (cfg.exporta_chaves)
		{
			if (!ExportaBlocoChaves(lisLIG))
			{
				return (false);
			}
		}
		if (cfg.exporta_geradores)
		{
			if (!ExportaBlocoGeradores(lisBARRAS))
			{
				return (false);
			}
		}
		// gera a lista de todos reguladores
		lisLIG->Clear();
		redes->LisLigacao(lisLIG, eqptoREGULADOR);
		if (cfg.exporta_reguladores)
		{
			if (!ExportaBlocoReguladores(lisLIG))
			{
				return (false);
			}
		}
		// gera a lista de todos trafos
		lisLIG->Clear();
		redes->LisLigacao(lisLIG, eqptoTRAFO);
		if (cfg.exporta_trafos)
		{
			if (!ExportaBlocoTrafos(lisLIG))
			{
				return (false);
			}
		}
		if (cfg.exporta_capacitores)
		{
			if (!ExportaBlocoCapacitores(lisBARRAS))
			{
				return (false);
			}
		}
		// Gera lista de todas barras
		lisSEDS->Clear();
		redes->LisRede(lisSEDS, redeETD);
		if (cfg.exporta_subestacao)
		{
			if (!ExportaBlocoSubestacoes(lisSEDS))
			{
				return (false);
			}
		}
		// conta quantos subscribers
		nSb = lines_eqptos->Count;
		// cria cabecçalho
		CriaCabecalho(nSb);
		// copia o lines_eqpto no lines
		for (int nl = 0; nl < lines_eqptos->Count; nl++)
		{
			lines->Add(lines_eqptos->Strings[nl]);
		}
	}
	else
	{
		sucesso = false;
	}



	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceCelPlanner::ExportaArqCelPlannerBarras(TStrings *lines, TList *lisBarras)
{ // variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	int nSb = 0;
	bool sucesso = true;

	//primeiro verifica se possui nomes repetidos
	if (ValidaCodigosEqptos())
	{
		// obtem a lista de redes carregadas
		// redes->LisRedeCarregada(lisRedesCarregadas);
		// salva ponteiro para objeto
		this->lines = lines;
		// exporta blocos de dados do arquivo CelPlanner
		lines->Clear();
		lines_eqptos->Clear();
		// Gera lista de todas barras
		lisBARRAS->Clear();
		redes->LisBarra(lisBARRAS);
		// gera a lista de todas chaves
		lisLIG->Clear();
		redes->LisLigacao(lisLIG, eqptoCHAVE);
		// gera os blocos
		ExportaBlocoBarras(lisBarras);
		/*
		if (cfg.exporta_cargas)
		{
			if (!ExportaBlocoCargas(lisBARRAS))
			{
				return (false);
			}
		}
		if (cfg.exporta_chaves)
		{
			if (!ExportaBlocoChaves(lisLIG))
			{
				return (false);
			}
		}
		if (cfg.exporta_geradores)
		{
			if (!ExportaBlocoGeradores(lisBARRAS))
			{
				return (false);
			}
		}
		// gera a lista de todos reguladores
		lisLIG->Clear();
		redes->LisLigacao(lisLIG, eqptoREGULADOR);
		if (cfg.exporta_reguladores)
		{
			if (!ExportaBlocoReguladores(lisLIG))
			{
				return (false);
			}
		}
		// gera a lista de todos trafos
		lisLIG->Clear();
		redes->LisLigacao(lisLIG, eqptoTRAFO);
		if (cfg.exporta_trafos)
		{
			if (!ExportaBlocoTrafos(lisLIG))
			{
				return (false);
			}
		}
		if (cfg.exporta_capacitores)
		{
			if (!ExportaBlocoCapacitores(lisBARRAS))
			{
				return (false);
			}
		}
		// Gera lista de todas barras
		lisSEDS->Clear();
		redes->LisRede(lisSEDS, redeETD);
		if (cfg.exporta_subestacao)
		{
			if (!ExportaBlocoSubestacoes(lisSEDS))
			{
				return (false);
			}
		}
		*/
		// conta quantos subscribers
		nSb = lines_eqptos->Count;
		// cria cabecçalho
		CriaCabecalho(nSb);
		// copia o lines_eqpto no lines
		for (int nl = 0; nl < lines_eqptos->Count; nl++)
		{
			lines->Add(lines_eqptos->Strings[nl]);
		}
	}
	else
	{
		sucesso = false;
	}



	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceCelPlanner::ExportaBlocoBarras(TList *listaBarras)
{ // variáveis locais
	VTCarga *carga;
	VTBarra *barra;
	VTRede *rede;
	AnsiString antena;

	try
	{ // cria a string da antena
		antena = AntenaDefaultAsString();
		// percorre todas barras e lista as cargas
		for (int nb = 0; nb < listaBarras->Count; nb++)
		{
			barra = (VTBarra*) listaBarras->Items[nb];
			TCPBarra cp_barra(apl, barra);

			// insere linha em StringLista
			lines_eqptos->Add(cp_barra.LinhaArqCsv(cfg.utm) + antena);

		}
		return (true);
	}
	catch (Exception &e)
	{
		return (false);
	}
}
// ---------------------------------------------------------------------------
bool __fastcall TInterfaceCelPlanner::ExportaBlocoCargas(TList *listaBarras)
{ // variáveis locais
	VTCarga *carga;
	VTBarra *barra;
	VTRede *rede;
	TList *lisCargas = new TList;
	AnsiString antena;

	try
	{ // cria a string da antena
		antena = AntenaDefaultAsString();
		// percorre todas barras e lista as cargas
		for (int nb = 0; nb < listaBarras->Count; nb++)
		{
			barra = (VTBarra*) listaBarras->Items[nb];
			barra->LisEqbar(lisCargas, eqptoCARGA);
		}
		// percorre todas as cargas e adiciona no txt
		for (int n = 0; n < lisCargas->Count; n++)
		{
			carga = (VTCarga*)lisCargas->Items[n];
			// se a carga nao for ip
			if (!carga->IP)
			{
				TCPCarga cp_carga(apl, carga);
				// insere linha em StringLista
				lines_eqptos->Add(cp_carga.LinhaArqCsv(cfg.utm) + antena);
			}
		}
		// destroi lista
		if (lisCargas)
		{
			delete lisCargas;
			lisCargas = NULL;
		}
		return (true);
	}
	catch (Exception &e)
	{
		return (false);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceCelPlanner::ExportaBlocoCapacitores(TList *listaBarras)
{ // variáveis locais
	VTCapacitor *capacitor;
	VTBarra *barra;
	VTRede *rede;
	TList *lisCap = new TList;
	AnsiString antena;

	try
	{ // cria a string da antena
		antena = AntenaDefaultAsString();
		// percorre todas barras e lista as cargas
		for (int nb = 0; nb < listaBarras->Count; nb++)
		{
			barra = (VTBarra*) listaBarras->Items[nb];
			barra->LisEqbar(lisCap, eqptoCAPACITOR);
		}
		// percorre todas os capacitores e adiciona no txt
		for (int n = 0; n < lisCap->Count; n++)
		{
			capacitor = (VTCapacitor*)lisCap->Items[n];
			TCPCapacitor cp_capacitor(apl, capacitor);
			// insere linha em StringLista
			lines_eqptos->Add(cp_capacitor.LinhaArqCsv(cfg.utm) + antena);
		}
		// destroi lista
		if (lisCap)
		{
			delete lisCap;
			lisCap = NULL;
		}
		return (true);
	}
	catch (Exception &e)
	{
		return (false);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceCelPlanner::ExportaBlocoChaves(TList *listaLigacoes)
{ // variáveis locais
	VTLigacao *ligacao;
	VTChave *chave;
	TList *lisChaves = new TList;
	AnsiString antena;

	try
	{ // cria a string da antena
		antena = AntenaDefaultAsString();
		// percorre todas barras e lista as cargas
		for (int nb = 0; nb < listaLigacoes->Count; nb++)
		{
			ligacao = (VTLigacao*) listaLigacoes->Items[nb];
			if (ligacao->Tipo() == eqptoCHAVE)
			{ // se for somente RAs
				chave = (VTChave*)ligacao;
				if (cfg.exporta_religadores)
				{
					if (chave->TipoChave->Tipo == chaveRA)
					{
						lisChaves->Add(ligacao);
					}
					else
					{ /* faz Nada */
					}
				}
				if (cfg.exporta_religadores)
				{
					if (chave->TipoChave->Tipo == chaveSEC)
					{
						lisChaves->Add(ligacao);
					}
					else
					{ /* faz Nada */
					}
				}
				else
				{
					lisChaves->Add(ligacao);
				}
			}
		}
		// percorre todas as cargas e adiciona no txt
		for (int n = 0; n < lisChaves->Count; n++)
		{
			chave = (VTChave*)lisChaves->Items[n];
			TCPChave cp_chave(apl, chave);
			// insere linha em StringLista
			lines_eqptos->Add(cp_chave.LinhaArqCsv(cfg.utm) + antena);
		}
		// destroi lista
		if (lisChaves)
		{
			delete lisChaves;
			lisChaves = NULL;
		}
		return (true);
	}
	catch (Exception &e)
	{
		return (false);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceCelPlanner::ExportaBlocoGeradores(TList *listaBarras)
{ // variáveis locais
	VTGerador *gerador;
	VTBarra *barra;
	VTRede *rede;
	TList *lisGeradores = new TList;
	AnsiString antena;

	try
	{ // cria a string da antena
		antena = AntenaDefaultAsString();
		// percorre todas barras e lista as cargas
		for (int nb = 0; nb < listaBarras->Count; nb++)
		{
			barra = (VTBarra*) listaBarras->Items[nb];
			barra->LisEqbar(lisGeradores, eqptoGERADOR);
		}
		// percorre todas as cargas e adiciona no txt
		for (int n = 0; n < lisGeradores->Count; n++)
		{
			gerador = (VTGerador*)lisGeradores->Items[n];
			TCPGerador cp_gerador(apl, gerador);
			// insere linha em StringLista
			lines_eqptos->Add(cp_gerador.LinhaArqCsv(cfg.utm) + antena);
		}
		// destroi lista
		if (lisGeradores)
		{
			delete lisGeradores;
			lisGeradores = NULL;
		}
		return (true);
	}
	catch (Exception &e)
	{
		return (false);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceCelPlanner::ExportaBlocoReguladores(TList *listaLigacoes)
{ // variáveis locais
	VTLigacao *ligacao;
	VTRegulador *regulador;
	TList *lisReg = new TList;
	AnsiString antena;

	try
	{ // cria a string da antena
		antena = AntenaDefaultAsString();
		// percorre todas barras e lista as cargas
		for (int nb = 0; nb < listaLigacoes->Count; nb++)
		{
			ligacao = (VTLigacao*) listaLigacoes->Items[nb];
			if (ligacao->Tipo() == eqptoREGULADOR)
			{ // se for somente RAs
				regulador = (VTRegulador*)ligacao;
				lisReg->Add(ligacao);
			}
		}
		// percorre todas as cargas e adiciona no txt
		for (int n = 0; n < lisReg->Count; n++)
		{
			regulador = (VTRegulador*)lisReg->Items[n];
			TCPRegulador cp_regulador(apl, regulador);
			// insere linha em StringLista
			lines_eqptos->Add(cp_regulador.LinhaArqCsv(cfg.utm) + antena);
		}
		// destroi lista
		if (lisReg)
		{
			delete lisReg;
			lisReg = NULL;
		}
		return (true);
	}
	catch (Exception &e)
	{
		return (false);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceCelPlanner::ExportaBlocoSubestacoes(TList *listaRedes)
{ // variaveis locais
	VTRede *rede;
	AnsiString antena;
	VTBarra *barra;
	try
	{ // cria a string da antena
		antena = AntenaDefaultAsString();
		// percorre todas barras e lista das redes de subestacao
		for (int nb = 0; nb < listaRedes->Count; nb++)
		{
			rede = (VTRede*) listaRedes->Items[nb];
			barra = rede->BarraInicial();
			if (rede->TipoRede->Segmento == redeETD && barra != NULL)
			{ // se for subestacao
				TCPSubestacao cp_subestacao(apl, rede);
				// insere linha em StringLista
				lines_eqptos->Add(cp_subestacao.LinhaArqCsv(cfg.utm) + antena);
			}
		}
		return (true);
	}
	catch (Exception &e)
	{
		return (false);
	}

}

// ---------------------------------------------------------------------------
bool __fastcall TInterfaceCelPlanner::ExportaBlocoTrafos(TList *listaLigacoes)
{ // variáveis locais
	VTLigacao *ligacao;
	VTTrafo *trafo;
	TList *lisTrafos = new TList;
	AnsiString antena;
	VTRede *rede;

	try
	{
		// cria a string da antena
		antena = AntenaDefaultAsString();
		// percorre todas barras e lista as cargas
		for (int nb = 0; nb < listaLigacoes->Count; nb++)
		{
			ligacao = (VTLigacao*) listaLigacoes->Items[nb];
			if (ligacao->Tipo() == eqptoTRAFO)
			{ // se for somente RAs
				lisTrafos->Add(ligacao);
			}
		}
		// percorre todas as cargas e adiciona no txt
		for (int n = 0; n < lisTrafos->Count; n++)
		{
			trafo = (VTTrafo*)lisTrafos->Items[n];
			// se for trafo de força não exporta
			if (trafo->rede->TipoRede->Segmento != redeETD)
			{
				TCPTrafo cp_trafo(apl, trafo);
				// insere linha em StringLista
				lines_eqptos->Add(cp_trafo.LinhaArqCsv(cfg.utm) + antena);
			}
		}
		// destroi lista
		if (lisTrafos)
		{
			delete lisTrafos;
			lisTrafos = NULL;
		}
		return (true);
	}
	catch (Exception &e)
	{
		return (false);
	}

}
// ---------------------------------------------------------------------------
bool __fastcall TInterfaceCelPlanner::ValidaCodigosEqptos(void)
{
	// variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *mLisEQPTO, *lisREPEAT;
	bool valido = true;
	VTEqpto *eqpto1, *eqpto2;
	int result = mrOk;
	int nEqptoRepeat = 1;
	TFormEdtEqptosCodigo *formEqpto;

	mLisEQPTO = new TList;
//	lisREPEAT = new TList;
	// gera a lista de eqptos
	if (cfg.exporta_cargas)
	{
		redes->LisEqbar(mLisEQPTO,eqptoCARGA);
	}
	if (cfg.exporta_chaves)
	{
		redes->LisLigacao(mLisEQPTO, eqptoCHAVE);
	}
	if (cfg.exporta_geradores)
	{
		redes->LisLigacao(mLisEQPTO, eqptoGERADOR);
	}
	if (cfg.exporta_reguladores)
	{
		redes->LisLigacao(mLisEQPTO, eqptoREGULADOR);
	}
	if (cfg.exporta_trafos)
	{
		redes->LisLigacao(mLisEQPTO, eqptoTRAFO);
	}
	if (cfg.exporta_capacitores)
	{
		redes->LisLigacao(mLisEQPTO, eqptoCAPACITOR);
	}
	if (cfg.exporta_subestacao)
	{
		redes->LisRede(mLisEQPTO,redeETD);
	}
	//ordena a lista pelo codigo
	mLisEQPTO->Sort(ComparaCodigoEqpto);
//	lisREPEAT->Clear();
	//percorre a lista em busca de codigos repetidos
	for (int ne = 0; ne < mLisEQPTO->Count - 1; ne++)
	{
		eqpto1 = (VTEqpto*) mLisEQPTO->Items[ne];
		eqpto2 = (VTEqpto*) mLisEQPTO->Items[ne+1];

		if (eqpto1->Codigo.AnsiCompareIC(eqpto2->Codigo) == 0)
		{
			valido = false;
			break;
//			if (lisREPEAT->IndexOf(eqpto1) < 0)
//				{lisREPEAT->Add(eqpto1);}
//			if (lisREPEAT->IndexOf(eqpto2) < 0)
//				{lisREPEAT->Add(eqpto2);}
		}
	}


	delete mLisEQPTO;
//	delete lisREPEAT;
	return valido;

}
//// ---------------------------------------------------------------------------
//bool __fastcall TInterfaceCelPlanner::ValidaCodigosEqptos(void)
//{
//	// variáveis locais
//	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
//	TList *mLisEQPTO, *lisREPEAT;
//	bool valido = true;
//	VTEqpto *eqpto1, *eqpto2;
//	int result = mrOk;
//	int nEqptoRepeat = 1;
//	TFormEdtEqptosCodigo *formEqpto;
//
//	mLisEQPTO = new TList;
//	lisREPEAT = new TList;
//	// gera a lista de eqptos
//	if (cfg.exporta_cargas)
//	{
//		redes->LisEqbar(mLisEQPTO,eqptoCARGA);
//	}
//	if (cfg.exporta_chaves)
//	{
//		redes->LisLigacao(mLisEQPTO, eqptoCHAVE);
//	}
//	if (cfg.exporta_geradores)
//	{
//		redes->LisLigacao(mLisEQPTO, eqptoGERADOR);
//	}
//	if (cfg.exporta_reguladores)
//	{
//		redes->LisLigacao(mLisEQPTO, eqptoREGULADOR);
//	}
//	if (cfg.exporta_trafos)
//	{
//		redes->LisLigacao(mLisEQPTO, eqptoTRAFO);
//	}
//	if (cfg.exporta_capacitores)
//	{
//		redes->LisLigacao(mLisEQPTO, eqptoCAPACITOR);
//	}
//	if (cfg.exporta_subestacao)
//	{
//		redes->LisRede(mLisEQPTO,redeETD);
//	}
//	//com a lista completa de elementos...
//	while ((nEqptoRepeat != 0) || (result != mrCancel))
//	{
//		//ordena a lista pelo codigo
//		mLisEQPTO->Sort(ComparaCodigoEqpto);
//		lisREPEAT->Clear();
//		//percorre a lista em busca de codigos repetidos
//		for (int ne = 0; ne < mLisEQPTO->Count - 1; ne++)
//		{
//			eqpto1 = (VTEqpto*) mLisEQPTO->Items[ne];
//			eqpto2 = (VTEqpto*) mLisEQPTO->Items[ne+1];
//
//			if (eqpto1->Codigo.AnsiCompareIC(eqpto2->Codigo) == 0)
//			{
//				valido = false;
//				if (lisREPEAT->IndexOf(eqpto1) < 0)
//					{lisREPEAT->Add(eqpto1);}
//				if (lisREPEAT->IndexOf(eqpto2) < 0)
//					{lisREPEAT->Add(eqpto2);}
//			}
//		}
//		nEqptoRepeat = lisREPEAT->Count;
//		if (nEqptoRepeat == 0)
//			{result = mrOk;}
//		else
//		{
//			if (formEqpto != NULL)
//				{delete formEqpto;}
//			formEqpto = new TFormEdtEqptosCodigo(NULL, NULL, lisREPEAT);
//			result =  formEqpto->ShowModal();
//        }
//	}
//	if (result == mrOk)
//		{valido = true;}
//	else
//		{valido = false;}
//
//	delete mLisEQPTO;
//	delete lisREPEAT;
//	return valido;
//
//}
// ---------------------------------------------------------------------------
// eof
