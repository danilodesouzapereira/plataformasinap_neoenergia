// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TStkCarga.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Curvas\VTCurva.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Curvas.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTStkCarga* __fastcall NewObjStkCarga(VTApl *apl_owner)
{
	try
	{ // cria objeto
		return (new TStkCarga(apl_owner));
	}
	catch (Exception &e)
	{
		return (NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TStkCarga::TStkCarga(VTApl *apl_owner)
{
	// salva ponteiro para objeto
	this->apl = apl_owner;
	// obtém informação da memória
}

// ---------------------------------------------------------------------------
__fastcall TStkCarga::~TStkCarga(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
bool __fastcall TStkCarga::AlteraDemanda(VTCarga *carga, int ind_pat, double p_kw, double q_kvar)
{
	// variáveis locais
	complex<double>s_mva[MAX_FASE];
	double p_mw = p_kw * 0.001;
	double q_mvar = q_kvar * 0.001;
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

	// verifica se a Carga possui curva própria
	if (carga->curva->Tipica)
	{ // redefine Carga com curva própria equilibrada
		if (!RedefineCargaComDemandaPropriaEquilibrada(carga))
			return (false);
		// altera valor da demanda no patamar
		demanda->S_mva[carga][ind_pat] = complex<double>(p_mw, q_mvar);
	}
	// verifica se a Carga é equilibrada
	else if (carga->curva->NumeroValores == nvCURVA_CAR)
	{ // altera valor da demanda no patamar
		demanda->S_mva[carga][ind_pat] = complex<double>(p_mw, q_mvar);
	}
	else if (carga->curva->NumeroValores == nvCURVA_RRCAR)
	{ // calcula demanda por fase
		for (int ind_fase = 0; ind_fase < MAX_FASE - 1; ind_fase++)
		{
			s_mva[ind_fase] = complex<double>(p_mw, q_mvar) / 3.;
		}
		s_mva[indNEUTRO] = complex<double>(0., 0.);
		// altera valor da demanda por fase no patamar
		demanda->Set_S3f_mva(carga, ind_pat, s_mva);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TStkCarga::ConverteCargasParaTrifasicas(TList *lisCARGA)
{
	// variáveis locais
	bool sucesso = true;
	VTCarga *carga;

	try
	{
		for (int n = 0; n < lisCARGA->Count; n++)
		{ // altera curva da carga e fases da carga
			carga = (VTCarga*)lisCARGA->Items[n];
			RedefineCargaComDemandaPropriaEquilibrada(carga);
		}
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return sucesso;
}

// ---------------------------------------------------------------------------
bool __fastcall TStkCarga::EquilibraRedes(VTRedes *redes)
{
	// variáveis locais
	bool sucesso = true;
	TList *lisEQP;

	try
	{ // cria lista
		lisEQP = new TList();
		// verifica se há trechos não trifásicos
		lisEQP->Clear();
		LisTrechoNaoTrifasico(lisEQP);
		if (lisEQP->Count > 0)
		{
			// Aviso("Todos os trechos devem ser trifásicos para converter as cargas");
			// destroi lista
			delete lisEQP;
			sucesso = false;
		}
		// verifica se há trafos não trifásicos
		lisEQP->Clear();
		LisTrafoNaoTrifasico(lisEQP);
		if (lisEQP->Count > 0)
		{
			// Aviso("Todos os trafos devem ser trifásicos para converter as cargas");
			// destroi lista
			delete lisEQP;
			sucesso = false;
		}
		// determina cargas não trifásicas
		lisEQP->Clear();
		LisCargaNaoTrifasica(lisEQP);
		// converte cargas para trifasicas
		sucesso = ConverteCargasParaTrifasicas(lisEQP);
		// destroi lista
		delete lisEQP;
	}
	catch (Exception &e)
	{
		if (lisEQP)
			delete lisEQP;
		sucesso = false;
	}
	return sucesso;
}

// ---------------------------------------------------------------------------
void __fastcall TStkCarga::LisCargaNaoTrifasica(TList *lisCARGA)
{
	// variáveis locais
	int num_fase;
	TList *lisEQP;
	VTCarga *carga;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// cria lista
	lisEQP = new TList();
	// determina cargas
	redes->LisEqbar(lisEQP, eqptoCARGA);
	for (int n = 0; n < lisEQP->Count; n++)
	{
		carga = (VTCarga*)lisEQP->Items[n];
		// determina número de fases da Carga
		if ((num_fase = fases->NumeroDeFases(carga->Fases)) != 3)
		{
			lisCARGA->Add(carga);
		}
	}
	// destroi lista
	delete lisEQP;
}

// ---------------------------------------------------------------------------
void __fastcall TStkCarga::LisTrafoNaoTrifasico(TList *lisTRAFO)
{
	// variáveis locais
	int num_fase;
	TList *lisEQP;
	VTTrafo *trafo;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// cria lista
	lisEQP = new TList();
	// determina trafos
	redes->LisEqpto(lisEQP, eqptoTRAFO);
	for (int n = 0; n < lisEQP->Count; n++)
	{
		trafo = (VTTrafo*)lisEQP->Items[n];
		// verifica se é monofásico
		if (trafo->Monofasico)
		{
			lisTRAFO->Add(trafo);
		}
	}
	// destroi lista
	delete lisEQP;
}

// ---------------------------------------------------------------------------
void __fastcall TStkCarga::LisTrechoNaoTrifasico(TList *lisTRECHO)
{
	// variáveis locais
	int num_fase;
	TList *lisEQP;
	VTTrecho *trecho;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// cria lista
	lisEQP = new TList();
	// determina trechos
	redes->LisEqpto(lisEQP, eqptoTRECHO);
	for (int n = 0; n < lisEQP->Count; n++)
	{
		trecho = (VTTrecho*)lisEQP->Items[n];
		// determina número de fases da Carga
		if ((num_fase = fases->NumeroDeFases(trecho->arranjo->Fases)) != 3)
		{
			lisTRECHO->Add(trecho);
		}
	}
	// destroi lista
	delete lisEQP;
}

// ---------------------------------------------------------------------------
bool __fastcall TStkCarga::RedeEquilibrada(VTRedes *redes)
{
	// variáveis locais
	bool equilibrada = true;
	TList *lisEQP;
	VTCarga *carga;
	int num_fase;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

	// proteção
	if (redes == NULL)
		return false;
	// cria lista
	lisEQP = new TList();
	// lista cargas
	redes->LisEqbar(lisEQP, eqptoCARGA);
	for (int n = 0; n < lisEQP->Count; n++)
	{
		carga = (VTCarga*)lisEQP->Items[n];
		// determina número de fases da Carga
		if ((num_fase = fases->NumeroDeFases(carga->Fases)) != 3)
		{
			equilibrada = false;
			break;
		}
	}
	// destroi lista
	delete lisEQP;

	return equilibrada;
}

// ---------------------------------------------------------------------------
bool __fastcall TStkCarga::RedefineCargaComDemandaPropriaDesequilibrada(VTCarga *carga)
{
	// variáveis locais
	int num_fase;
	double valor[nvCURVA_RRCAR];
	double p_mw, q_mvar;
	strHM patHM;
	VTCurva *curva;
	VTPatamar *patamar;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	try
	{ // verifica se Carga com curva típica ou com curva própria equilibrada
		if ((carga->curva->Tipica) || (carga->curva->NumeroValores != nvCURVA_RRCAR))
		{ // verifica se a Curva possui Curva siamesa
			if (carga->curva->CurvaSiamesaPUmedia == NULL)
			{ // cria uma cópia da Curva
				curva = carga->curva->Clone();
			}
			else
			{ // cria uma cópia da Curva siamesa
				curva = carga->curva->CurvaSiamesaPUmedia->Clone();
			}
			// cria curva com demanda por fase
			curva->Tipo = curvaPQ;
			curva->Unidade = unidMVA;
			curva->NumeroValores = nvCURVA_RRCAR;
			curva->Tipica = false;
			curva->Eqpto = carga;
			curva->Classe = NULL;
			// determina número de fases da Carga
			if ((num_fase = fases->NumeroDeFases(carga->Fases)) == 0)
			{ // proteção contra fases inválidas
				num_fase = 3;
				carga->Fases = faseABC;
			}
			// se necessário, cria pontos da Curva, c/ valores nulos
			// if (curva->Escala != patamares->Escala)
			// {
			// curva->Reconfigura(patamares->Escala);
			// }
			// cria pontos da Curva, c/ valores nulos
			curva->Reconfigura(patamares->Escala);
			// define valores dos pontos da Curva própria
			for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
			{
				patamar = patamares->Patamar[ind_pat];
				patHM.hora = patamar->HoraIni;
				patHM.minuto = patamar->MinutoIni;
				// obtém demanda da Carga por fase no patamar
				p_mw = demanda->P_mw[carga][ind_pat] / num_fase;
				q_mvar = demanda->Q_mvar[carga][ind_pat] / num_fase;
				// define valores da Curva no patamar
				for (int n = 0; n < nvCURVA_RRCAR; n++)
				{
					valor[n] = 0.;
				}
				if (carga->Fases & faseA)
				{
					valor[0] = p_mw;
					valor[1] = q_mvar;
				}
				if (carga->Fases & faseB)
				{
					valor[2] = p_mw;
					valor[3] = q_mvar;
				}
				if (carga->Fases & faseC)
				{
					valor[4] = p_mw;
					valor[5] = q_mvar;
				}
				curva->SetPonto(patHM, valor, nvCURVA_RRCAR);
			}
			// redefine Curva da Carga
			carga->DefineCurva(curva);
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15372453
bool __fastcall TStkCarga::RedefineCargaComDemandaPropriaEquilibrada(VTCarga *carga)
{
	// variáveis locais
	int num_fase;
	double valor[nvCURVA_PQ];
	double p_mw, q_mvar;
	strHM patHM;
	VTCurva *curva;
	VTPatamar *patamar;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

	try
	{ // determina número de fases da Carga
		if ((num_fase = fases->NumeroDeFases(carga->Fases)) == 3)
		{ // verifica se Carga com curva típica ou com curva própria equilibrada
			if ((!carga->curva->Tipica) && (carga->curva->NumeroValores == nvCURVA_PQ))
			{
				return (true);
			}
		}

		// cria curva com demanda trifásica
		curva = DLL_NewObjCurva();
		curva->Tipo = curvaPQ;
		curva->Unidade = unidMVA;
		curva->NumeroValores = nvCURVA_PQ;
		curva->Tipica = false;
		curva->Eqpto = carga;
		curva->Classe = NULL;

		// se necessário, cria pontos da Curva, c/ valores nulos
		if (curva->Escala != patamares->Escala)
		{
			curva->Reconfigura(patamares->Escala);
		}

		// define valores dos pontos da Curva própria
		for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
		{
			patamar = patamares->Patamar[ind_pat];
			patHM.hora = patamar->HoraIni;
			patHM.minuto = patamar->MinutoIni;
			// obtém demanda da Carga no patamar
			valor[0] = demanda->P_mw[carga][ind_pat];
			valor[1] = demanda->Q_mvar[carga][ind_pat];

			curva->SetPonto(patHM, valor, nvCURVA_PQ);
		}

		// altera para trifasica, mantendo neutro e terra, se houver
		carga->Fases |= faseABC;
		// redefine Curva da Carga
		carga->DefineCurva(curva);
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
// eof
