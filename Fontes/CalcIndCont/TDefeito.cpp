// ---------------------------------------------------------------------------
#pragma hdrstop
#include <DateUtils.hpp>
#include "TDefeito.h"
#include "VTEstratSoc.h"
#include "VTVisRede.h"
#include <PlataformaSinap\Fontes\Bloco\VTBloco.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamares.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// COMENTÁRIOS
// hk201610: sai adicao redeundante em lisBlocosAgreg->Add(blocoDef);
// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
// ---------------------------------------------------------------------------
VTDefeito* __fastcall NewObjDefeito(VTBloco *blocoDef, int numPat)
{
	return (new TDefeito(blocoDef, numPat));
}

// ---------------------------------------------------------------------------
__fastcall TDefeito::TDefeito(VTBloco *blocoDef, int numPat)
{
	// listas em funcao do bloco de defeito e equipamentos de protecao
	lisProt = new TList();
	lisChvMon = new TList(); // lista de chaves para isolamento do defeito a montante
	lisChvJus = new TList(); // lista de chaves do bloco de defeito a jusante
	lisChvVisDef = new TList(); // lista de TVisRede com ponteiro para chaves NA de transferencia
	lisVisRedeDef = new TList(); // lista de objtos visRede do defeito
	lisBlocosMan = new TList();
	// lista de blocos de blocos a jusante do desligamento, com possibilidade de socorro
	lisChavesBlocosMan = new TList();
	// lista de chaves internas a lisBlocosMan, com possibilidade de manobra
	lisBlocoManVis = new TList();
	// lista de blocos dos circuitos de vis fora dos blocos troncais da chave vis ao suprimento

	lisChvAlivioVis = new TList();
	// lista de chaves com possibilidadedes de corte no circuito de vis ou de socorro, imediatamente ligado ao tronco
	lisChvBlocoVis = new TList();
	// Lista de chaves troncais nos circuitos de socorro da chave vis ao suprimento

	lisBlocosVis = new TList();
	// Lista de blocos troncais nos circuitos de socorro da chave vis ao suprimento
	lisBlocosJusProt = new TList(); // lista de blocos a justante da chave de protecao
	lisBlocosJusAuto = new TList();
	// lista de blocos a justante da chave de automática de isolamento a montante
	lisBlocosJusDef = new TList(); // lista de blocos a justante do bloco de defeito
	lisBlocosProtDef = new TList(); // lista de blocos entre a chave de protecao e o defeito
	lisBlocosProtAuto = new TList();
	// lista de blocos entre a chave de protecao e chave automática de isolamento a montante
	lisBlocosAutoDef = new TList();
	// lista de blocos entre a chave automática de isolamento a montante bloco de defeito
	lisBlocosAgreg = new TList();
	// lista de blocos agregadadas devido a chaves novas nao implementadas
	lisLigaVis = new TList(); // lista de ligacões do socorro ao suprimento
	lisLigaVisDef = new TList(); // lista de ligações do defeito ao socorro
	lisBlocoRadJusDef = new TList(); // lista de blocoRad manobráveis a jusante do bloco de defeito

	// Listas em funcao do bloco de defeito e patamar de carga - Estrategias de socorro
	// lisBlocosSoc     = new TList();     //lista de blocos a justante socorridos
	// lisVisRedeSoc    = new TList();     //lista de visRede atuadas relativa ao defeito
	// lisJusVisSoc     = new TList();     //lista de chaves a jusante do defeito associadas ao defeito
	lisEstratSoc = new TList(); // lista de estratégias de socorro por patamar de demanda

	// Seta ponteiros relativos a localizacao dos defeitos
	this->numPat = numPat;
	// Adiciona a lisBlocosAgreg->Add(blocoDef) e seta rede = bloco->Rede;
	this->Bloco = blocoDef;
	eqpto = NULL;

	// Adiciona bloco de defeito a lisBlocosAgreg
	// hkv201610: sai adicao redeundante em lisBlocosAgreg->Add(blocoDef);

	// Define os instantes padrôes
	TemposDefault();
	// Inicia chave de isolamento de montante
	chvIsolaMont = NULL;
	// Inicia chave de isolamento automático a montante
	chvIsolaAutoMont = NULL;
	// Cria estratégias de socorro
	CriaLisEstratSoc();

	// Inicia chave de protecao que atuou
	chvProt = NULL;
	chvReliga = NULL;
	// Inicia número de falhas
	nfalhas = 0.0;
	// Inicia resultados
	dcons = 0.0; // desliga x cons
	tcons = 0.0; // tdesligax cons
	nmanAuto = 0; // #manobras automáticas
	nmanMan = 0; // #manobras manuais

	// Inicia flag de self healing para corte de carga em circuitos de socorro
	flagCortaVis = false;

	iPat = 0; // patamar de referência para gerar estratégia de socorro
	fc = 1.0; // fator de carga para os blocos desligados entre iniPat e fimPat

	ManProtAut.tempo = ManProtAut.clih = ManProtAut.cli = 0.0;
	ManProtAut.nBlocosManobra = 0;
	ManProtAut.chaveAbre = ManProtAut.chaveFecha = NULL;

	ManAutoDef.tempo = ManAutoDef.clih = ManAutoDef.cli = 0.0;
	ManAutoDef.nBlocosManobra = 0;
	ManAutoDef.chaveAbre = ManAutoDef.chaveFecha = NULL;

	ManProtDef.tempo = ManProtDef.clih = ManProtDef.cli = 0.0;
	ManProtDef.nBlocosManobra = 0;
	ManProtDef.chaveAbre = ManProtDef.chaveFecha = NULL;
	for (int n = 0; n < 24; n++)
	{
		ManReparo[n].tempo = ManReparo[n].clih = ManReparo[n].cli = 0.0;
		ManReparo[n].nBlocosManobra = 0;
		ManReparo[n].chaveAbre = ManReparo[n].chaveFecha = NULL;

		ManSocRapida[n].tempo = ManSocRapida[n].clih = ManSocRapida[n].cli = 0.0;
		ManSocRapida[n].nBlocosManobra = 0;
		ManSocRapida[n].chaveAbre = ManSocRapida[n].chaveFecha = NULL;

		ManSocLenta[n].tempo = ManSocLenta[n].clih = ManSocLenta[n].cli = 0.0;
		ManSocLenta[n].nBlocosManobra = 0;
		ManSocLenta[n].chaveAbre = ManSocLenta[n].chaveFecha = NULL;
	}
	// hk201703: adaptações para planTurmas: : acréscimo de fator de ajuste do tma
	fatTMA = 1.0;
}

// ---------------------------------------------------------------------------
__fastcall TDefeito::~TDefeito(void)
{
	// destroi listas e objetos
	if (lisProt)
	{
		delete lisProt;
		lisProt = NULL;
	}
	if (lisChvMon)
	{
		delete lisChvMon;
		lisChvMon = NULL;
	}
	if (lisChvJus)
	{
		delete lisChvJus;
		lisChvJus = NULL;
	}
	if (lisChvVisDef)
	{
		delete lisChvVisDef;
		lisChvVisDef = NULL;
	}
	if (lisVisRedeDef)
	{
		delete lisVisRedeDef;
		lisVisRedeDef = NULL;
	}
	if (lisBlocosMan)
	{
		delete lisBlocosMan;
		lisBlocosMan = NULL;
	}
	if (lisChavesBlocosMan)
	{
		delete lisChavesBlocosMan;
		lisChavesBlocosMan = NULL;
	}
	if (lisBlocoManVis)
	{
		delete lisBlocoManVis;
		lisBlocoManVis = NULL;
	}
	if (lisBlocosVis)
	{
		delete lisBlocosVis;
		lisBlocosVis = NULL;
	}
	if (lisChvAlivioVis)
	{
		delete lisChvAlivioVis;
		lisChvAlivioVis = NULL;
	}
	if (lisChvBlocoVis)
	{
		delete lisChvBlocoVis;
		lisChvBlocoVis = NULL;
	}
	if (lisBlocosJusProt)
	{
		delete lisBlocosJusProt;
		lisBlocosJusProt = NULL;
	}
	if (lisBlocosJusDef)
	{
		delete lisBlocosJusDef;
		lisBlocosJusDef = NULL;
	}
	if (lisBlocosProtDef)
	{
		delete lisBlocosProtDef;
		lisBlocosProtDef = NULL;
	}
	// if (lisBlocosSoc) {delete lisBlocosSoc; lisBlocosSoc = NULL;}
	// if (lisVisRedeSoc) {delete lisVisRedeSoc; lisVisRedeSoc = NULL;}
	// if (lisJusVisSoc) {delete lisJusVisSoc; lisJusVisSoc = NULL;}
	if (lisBlocosJusAuto)
	{
		delete lisBlocosJusAuto;
		lisBlocosJusAuto = NULL;
	}
	if (lisBlocosProtAuto)
	{
		delete lisBlocosProtAuto;
		lisBlocosProtAuto = NULL;
	}
	if (lisBlocosAutoDef)
	{
		delete lisBlocosAutoDef;
		lisBlocosAutoDef = NULL;
	}
	if (lisBlocosAgreg)
	{
		delete lisBlocosAgreg;
		lisBlocosAgreg = NULL;
	}
	if (lisEstratSoc)
	{
		LimpaTList(lisEstratSoc);
		delete lisEstratSoc;
		lisEstratSoc = NULL;
	}
	if (lisLigaVis)
	{
		delete lisLigaVis;
		lisLigaVis = NULL;
	}
	if (lisLigaVisDef)
	{
		delete lisLigaVisDef;
		lisLigaVisDef = NULL;
	}
	if (lisBlocoRadJusDef)
	{
		delete lisBlocoRadJusDef;
		lisBlocoRadJusDef = NULL;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TDefeito::PM_GetFlagTPre(void)
	{
   //hk201706: Modelagem com acréscimo de percentuais do tat (prep, desl+loc, exe)
   //Flag se utiliza percentuais de atendimento ou se somente o tempo total de atendimento e o tempo de localizacao fixo
   return((ptpre + ptdlo + ptexe) > 0.99);
   }
//---------------------------------------------------------------------------
void __fastcall TDefeito::Inicia(void)
{
	// Reinicia listas da classe TDefeito
	// Reinicia estratégisas de socorro: LimpaTList(lisEstratSoc);
	// Define os instantes padrôes
	// Inicia chave de isolamento de montante 				: chvIsolaMont     = NULL;
	// Inicia chave de isolamento automático a montante : chvIsolaAutoMont = NULL;
	// Inicia chave de protecao que atuou 					: chvProt = chvReliga = NULL;
	// Inicia número de falhas : nfalhas  = 0.0;
	// Inicia número de piscadas por religamento média por ano no bloco do defeito : npisca   = 0.0;
	// Inicia resultados: dcons    = tcons    = nmanAuto = tcons    = 0.0
	// Inicia resultados intermediarios:
	// Man*.tempo = cli = clih, nBlocosManobra = 0. chaveAbre = chaveFecha = NULL
	// ManProtAut, ManAutoDef, ManProtDef, ManReparo[np], ManSocRapida[np], ManSocLenta[np]

	// Reinicia listas da classe
	lisProt->Clear();
	lisChvMon->Clear();
	lisChvJus->Clear();
	lisChvVisDef->Clear();
	lisVisRedeDef->Clear();
	lisBlocosMan->Clear();
	lisBlocoManVis->Clear();
	lisBlocosVis->Clear();
	lisBlocosJusProt->Clear();
	lisBlocosJusDef->Clear();
	lisBlocosProtDef->Clear();
	lisBlocosJusAuto->Clear();
	lisBlocosProtAuto->Clear();
	lisBlocosAutoDef->Clear();
	lisBlocosAgreg->Clear();
	lisLigaVis->Clear();
	lisLigaVisDef->Clear();
	lisBlocoRadJusDef->Clear();
	// Reinicia estratégisas de socorro
	IniciaLisEstratSoc(); // LimpaTList(lisEstratSoc);
	// Define os instantes padrôes
	TemposDefault();
	// Inicia chave de isolamento de montante : chvIsolaMont     = NULL;
	chvIsolaMont = NULL;
	// Inicia chave de isolamento automático a montante : chvIsolaAutoMont = NULL;
	chvIsolaAutoMont = NULL;

	// Inicia chave de protecao que atuou : chvProt = chvReliga = NULL;
	chvProt = NULL;
	chvReliga = NULL;
	// Inicia número de falhas : nfalhas  = 0.0;
	nfalhas = 0.0;
	// Inicia número de piscadas por religamento média por ano no bloco do defeito : npisca   = 0.0;
	npisca = 0.0;
	// Inicia resultados: dcons    = tcons    = nmanAuto = tcons    = 0.0
	dcons = 0.0; // desliga x cons
	tcons = 0.0; // tdesligax cons
	nmanAuto = 0; // #manobras automáticas
	nmanMan = 0; // #manobras manuais

	// Inicia resultados intermediarios:
	// Man*.tempo = cli = clih, nBlocosManobra = 0. chaveAbre = chaveFecha = NULL
	// ManProtAut, ManAutoDef, ManProtDef, ManReparo[np], ManSocRapida[np], ManSocLenta[np]
	ManProtAut.tempo = ManProtAut.clih = ManProtAut.cli = 0.0;
	ManProtAut.nBlocosManobra = 0;
	ManProtAut.chaveAbre = ManProtAut.chaveFecha = NULL;

	ManAutoDef.tempo = ManAutoDef.clih = ManAutoDef.cli = 0.0;
	ManAutoDef.nBlocosManobra = 0;
	ManAutoDef.chaveAbre = ManAutoDef.chaveFecha = NULL;

	ManProtDef.tempo = ManProtDef.clih = ManProtDef.cli = 0.0;
	ManProtDef.nBlocosManobra = 0;
	ManProtDef.chaveAbre = ManProtDef.chaveFecha = NULL;
	for (int n = 0; n < 24; n++)
	{
		ManReparo[n].tempo = ManReparo[n].clih = ManReparo[n].cli = 0.0;
		ManReparo[n].nBlocosManobra = 0;
		ManReparo[n].chaveAbre = ManReparo[n].chaveFecha = NULL;

		ManSocRapida[n].tempo = ManSocRapida[n].clih = ManSocRapida[n].cli = 0.0;
		ManSocRapida[n].nBlocosManobra = 0;
		ManSocRapida[n].chaveAbre = ManSocRapida[n].chaveFecha = NULL;

		ManSocLenta[n].tempo = ManSocLenta[n].clih = ManSocLenta[n].cli = 0.0;
		ManSocLenta[n].nBlocosManobra = 0;
		ManSocLenta[n].chaveAbre = ManSocLenta[n].chaveFecha = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDefeito::TemposDefault(void)
{
	unsigned short year, month, day;
	unsigned short hour, min, sec, msec;
	hsist = hsist.CurrentDateTime();
	duraPreFalta = 60.0;
	durarep = 120.0; // default 2 horas

	ReiniciaTempos();
	// hpreDef      = IncMinute(hsist, - duraPreFalta);
	// hfimDef      = IncMinute(hsist, durarep);

	// hsist.DecodeDate(&year, &month, &day);
	// hsist.DecodeTime(&hour, &min, &sec, &msec);
	// hpreDef.DecodeDate(&year, &month, &day);
	// hpreDef.DecodeTime(&hour, &min, &sec, &msec);
	// hfimDef.DecodeDate(&year, &month, &day);
	// hfimDef.DecodeTime(&hour, &min, &sec, &msec);
	// TDateTime hsist;         //Instante iniciado sistema
	// TDateTime hpreDef;       //Instante préfalta
	// TDateTime hfimDef;       //Término do reparo
	// double    hrep;          //duração a partir de hsist para o reparo
}

// ---------------------------------------------------------------------------
void __fastcall TDefeito::ReiniciaTempos(void)
{
	unsigned short year, month, day, hour, min, sec, msec;

	hpreDef = IncMinute(hsist, -duraPreFalta);
	hfimDef = IncMinute(hsist, durarep);
	// Define patamar de referência para gerar estratégia de socorro
	// na hora da pré-falta
	hpreDef.DecodeTime(&hour, &min, &sec, &msec);
	iPat = hour;
	// hsist.DecodeDate(&year, &month, &day);
	// hsist.DecodeTime(&hour, &min, &sec, &msec);
	// hpreDef.DecodeDate(&year, &month, &day);
	// hpreDef.DecodeTime(&hour, &min, &sec, &msec);
	// hfimDef.DecodeDate(&year, &month, &day);
	// hfimDef.DecodeTime(&hour, &min, &sec, &msec);
	// TDateTime hsist;         //Instante iniciado sistema
	// TDateTime hpreDef;       //Instante préfalta
	// TDateTime hfimDef;       //Término do reparo
	// double    hrep;          //duração a partir de hsist para o reparo
}

// ---------------------------------------------------------------------------
double __fastcall TDefeito::ContribTransf(void)
{
	// Calculo de ganhos em clih resultado da transferencia de carga
	// entre circuitos
	double contrib;
	double jusanteCli, jusanteClih, reparoClih;
	double jusanteReparoSTClih; // reparo e jusante sem transferencia
	double JusanteReparoClih; // reparo e jusante com transferencia

	// jusanteCli  = 0.0;
	jusanteClih = 0.0;
	// reparoClih  = 0.0;
	for (int n = 0; n < 24; n++)
	{
		// jusanteCli  += (ManSocRapida[n].cli  + ManSocLenta[n].cli);
		jusanteClih += (ManSocRapida[n].clih + ManSocLenta[n].clih);
		// reparoClih  += ManReparo[n].clih;
	}

	// jusanteReparoSTClih = jusanteCli * treparoTotal + reparoClih;
	// JusanteReparoClih   = jusanteClih + reparoClih;

	ganhoTransf = 0.0;
	// if(jusanteReparoSTClih > 0) ganhoTransf = (jusanteReparoSTClih - JusanteReparoClih) / jusanteReparoSTClih;
	// ganhoTransf = (jusanteReparoSTClih - JusanteReparoClih);
	// ganhoTransf = (JusanteReparoClih);
	ganhoTransf = (jusanteClih);

	return (ganhoTransf);
}

// ---------------------------------------------------------------------------
void __fastcall TDefeito::CriaLisEstratSoc(void)
{
	VTEstratSoc *estratSoc;
	// Busca estratégia de socorro para o patamar
	for (int n = 0; n < numPat; n++)
	{
		lisEstratSoc->Add(estratSoc = NewObjEstratSoc(n));
		estratSoc->Defeito = this;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDefeito::IniciaLisEstratSoc(void)
{
	VTEstratSoc *estratSoc;
	for (int n = 0; n < lisEstratSoc->Count; n++)
	{
		estratSoc = (VTEstratSoc*)lisEstratSoc->Items[n];
		estratSoc->Inicia();
	}
}

// ---------------------------------------------------------------------------
TList* __fastcall TDefeito::PM_GetLisChvVisDef(void)
{
	VTVisRede* visRede;
	lisChvVisDef->Clear();
	for (int nv = 0; nv < lisVisRedeDef->Count; nv++)
	{
		lisChvVisDef->Add(((VTVisRede*)lisVisRedeDef->Items[nv])->pchave);
	}
	return (lisChvVisDef);
}

// ---------------------------------------------------------------------------
VTEstratSoc* __fastcall TDefeito::PM_GetEstratSoc(int np)
{
	VTEstratSoc *estratSoc = NULL;

	if (!(np < lisEstratSoc->Count))
		return (NULL);
	// Retorna estratégia do patamar
	estratSoc = (VTEstratSoc*)lisEstratSoc->Items[np];

	// Busca estratégia de socorro para o patamar
	// for(int n = 0; n < lisEstratSoc->Count; n++)
	// {
	// estratSoc = (VTEstratSoc*)lisEstratSoc->Items[n];
	// if(estratSoc->npat == np) break;
	// estratSoc = NULL;
	// }
	// if(estratSoc == NULL) {lisEstratSoc->Add(estratSoc = NewObjEstratSoc(np));
	// estratSoc->Defeito = this;
	// }

	return (estratSoc);
}

// ---------------------------------------------------------------------------
void __fastcall TDefeito::PM_SetBloco(VTBloco *bloco)
{
	this->bloco = bloco;
	rede = NULL;
	if (bloco != NULL)
	{
		rede = bloco->Rede;
		if (lisBlocosAgreg->IndexOf(bloco) == -1)
			lisBlocosAgreg->Add(bloco);
	}
}
// ---------------------------------------------------------------------------
