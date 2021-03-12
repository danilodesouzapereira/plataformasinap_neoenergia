// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEstimaDemanda.h"
#include "TExportaCarga.h"
#include "VTMontaMedicao.h"
#include "TZip.h"
#include "..\Apl\VTApl.h"
#include "..\Calculo\VTData.h"
#include "..\Classes\VTClasse.h"
#include "..\Copia\VTCopia.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Const.h"
#include "..\Curvas\VTCurva.h"
#include "..\Diretorio\VTPath.h"
#include "..\Estimador\VTEstimador.h"
#include "..\Flow\VTFlow.h"
#include "..\ImportaMedicao\VTMedicoes.h"
#include "..\Log\VTLog.h"
#include "..\Ordena\VTOrdena.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Potencia\VTFornecimento.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Redutor\VTRedutor.h"
#include "..\SisLin\VTSisLin.h"
#include "..\Stk\VTStkCarga.h"
#include "..\..\DLL_Inc\Copia.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Estimador1.h"
#include "..\..\DLL_Inc\Estimador3.h"
#include "..\..\DLL_Inc\Flow.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\ImportaMedicao.h"
#include "..\..\DLL_Inc\Log.h"
#include "..\..\DLL_Inc\Ordena.h"
#include "..\..\DLL_Inc\Patamar.h"
#include "..\..\DLL_Inc\Rede.h"
#include "..\..\DLL_Inc\Redutor.h"
#include "..\..\DLL_Inc\Stk.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTEstimaDemanda* __fastcall NewObjEstimaDemanda(VTApl *apl_owner)
{
	return (new TEstimaDemanda(apl_owner));
}

// ---------------------------------------------------------------------------
__fastcall TEstimaDemanda::TEstimaDemanda(VTApl *apl_owner)
{
	// cria objeto Apl pr�prio
	apl = NewObjApl(NULL, apl_owner);
	// obt�m ponteiro de objetos
	demanda      = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));
	// cria objeto locais
	apl->Add(patamares = DLL_NewObjPatamares(apl));
	apl->Add(flow      = DLL_NewObjFlow(apl));
	apl->Add(copia     = DLL_NewObjCopia(apl));
	// cria listas
	lisBAR              = new TList();
	lisEQP              = new TList();
	lisTMP              = new TList();
	diag.StrLisRede_OK  = new TStringList();
	diag.StrLisRede_NOK = new TStringList();
	// Aplica valores de default
	LogEnabled         = true; // false;
	OrdenacaoLinhasQR  = qrlMENOR_NUM_ELEM_FD;
	SalvaCurvaEstimada = true;
	UtilizaVDNM        = false;
	NumMaxMontH        = 2;
	HoraReferencia     = 0;
	HorasAfetadas      = 0;
	MetodoEE           = meeQR_GIVENS;
	TipoEstimacao      = estimacaoDESEQUILIBRADA;
	TipoReducao        = reducaoZIP;
	TipoRelatorio      = relNENHUM;
	FatMultMedChave    = 1.;

	// configura flow
	flow->UpToDate       = false;
	flow->TipoFluxo      = fluxoDESEQUILIBRADO;
	flow->PorRedeEnabled = true;
	// OBS: talvez seja importante n�o habilitar multithread quando usado pelo Wise
	flow->MultiThreadEnabled = true;
	flow->PerdaFerroEnabled  = true;
	flow->TramoEnabled       = true;
	flow->ModeloCarga        = cargaAUTO;
	flow->ProgressoEnabled   = false;
	flow->AvisoEnabled       = false;
	flow->CronometroEnabled  = false;
	flow->TipoRelatorio      = relNENHUM;
}

// ---------------------------------------------------------------------------
__fastcall TEstimaDemanda::~TEstimaDemanda(void)
{
	// destr�i lista sem destruir seus objetos
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
	if (lisTMP)
	{
		delete lisTMP;
		lisTMP = NULL;
	}
	// destr�i lista sem destruir seus objetos
	if (diag.StrLisRede_OK)
	{
		delete diag.StrLisRede_OK;
		diag.StrLisRede_OK = NULL;
	}
	if (diag.StrLisRede_NOK)
	{
		delete diag.StrLisRede_NOK;
		diag.StrLisRede_NOK = NULL;
	}
	// destr�i objeto Apl
	if (apl)
	{
		delete apl;
		apl = NULL;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::AtualizaDemandaCargaClonada(void)
{
	// vari�veis locais
	bool sucesso;
	VTMontaMedicao *monta = (VTMontaMedicao*)apl->GetObject(__classid(VTMontaMedicao));

	try
	{ // reflete valor estimado nas Cargas da rede clonada
		sucesso = monta->RefleteMedicaoNasCargas();
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TEstimaDemanda::AtualizaDemandaCargaOriginal(VTRedes *redes, VTRedes *redes_clone)
 {
 //vari�veis locais
 int     hora;
 double  fatcor;
 VTBarra *barra;
 VTCarga *carga, *carga_clone;

 //obt�m lista de Cargas da Rede original
 lisEQP->Clear();
 redes->LisEqbar(lisEQP, eqptoCARGA);
 //obt�m lista de Cargas da Rede clonada
 lisTMP->Clear();
 redes_clone->LisEqbar(lisTMP, eqptoCARGA);
 //loop para todas Cargas da rede orignal
 for (int n = 0; n < lisEQP->Count; n++)
 {
 carga = (VTCarga*)lisEQP->Items[n];
 //obt�m Carga clonada
 if ((carga_clone = ExisteCarga(lisTMP, carga->Id)) == NULL) continue;
 //verifica se foi definido um n�mero de horas afetadas
 if ((HorasAfetadas >= 1) && (HorasAfetadas <= 24))
 {//determina fator de corre��o na hora de refer�ncia
 fatcor = CalculaFatorCorrecaoHoraReferencia(carga, carga_clone);
 //atualiza demanda da Carga original na hora de refer�ncia
 AtualizaDemandaCargaOriginal(carga, carga_clone, HoraReferencia);
 //atualiza demanda da Carga original nas horas afetadas
 for (int n = 1; n <= HorasAfetadas; n++)
 {
 if ((hora = HoraReferencia + n) >= 24) hora = 0;
 AtualizaDemandaCargaOriginal(carga, hora, fatcor);
 }
 }
 else
 {//atualiza demanda da Carga original na hora de refer�ncia
 AtualizaDemandaCargaOriginal(carga, carga_clone, HoraReferencia);
 }
 }
 return(true);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::AtualizaDemandaCargaOriginal(VTRedes *redes, VTRedes *redes_clone)
{
	// vari�veis locais
	int hora;
	double fatcor;
	VTBarra *barra;
	VTCarga *carga, *carga_clone;
	VTPatamar *patamar;

	// obt�m lista de Cargas da Rede original
	lisEQP->Clear();
	redes->LisEqbar(lisEQP, eqptoCARGA);
	// obt�m lista de Cargas da Rede clonada
	lisTMP->Clear();
	redes_clone->LisEqbar(lisTMP, eqptoCARGA);
	// verifica se deve atualizar a demanda de todos patamares
	if (HoraReferencia < 0)
	{ // atualiza demanda das Cargas em todos patamares
		for (int n = 0; n < lisEQP->Count; n++)
		{ // n�o atualiza carga de IP
			carga = (VTCarga*)lisEQP->Items[n];
			if (carga->IP)
				continue;
			// obt�m Carga clonada
			if ((carga_clone = ExisteCarga(lisTMP, carga->Id)) == NULL)
				continue;
			// loop p/ todos patamares
			for (int np = 0; np < patamares->NumPatamar(); np++)
			{
				patamar = patamares->Patamar[np];
				// verifica se Patamar habilitado
				if (!patamar->Enabled)
					continue;
				// atualiza demanda da Carga original no patamar
				AtualizaDemandaCargaOriginal(carga, carga_clone, patamar->HoraIni);
			}
		}
	}
	else
	{ // atualiza demanda das Cargas apenas para a hora de refer�ncia
		for (int n = 0; n < lisEQP->Count; n++)
		{ // n�o atualiza carga de IP
			carga = (VTCarga*)lisEQP->Items[n];


/*
// DEBUG ESTIMA��O DESEQUILIBRADA
if(carga->Id == 2318)
   {
   int  xxx   = 1;
   bool teste = carga->DefinidaPorFase;
   }
*/


			if (carga->IP)
				continue;
			// obt�m Carga clonada
			if ((carga_clone = ExisteCarga(lisTMP, carga->Id)) == NULL)
				continue;
			// verifica se foi definido um n�mero de horas afetadas
			if (HorasAfetadas == 0)
			{ // atualiza demanda da Carga original na hora de refer�ncia
				AtualizaDemandaCargaOriginal(carga, carga_clone, HoraReferencia);
			}
			else
			{ // determina fator de corre��o na hora de refer�ncia
				fatcor = CalculaFatorCorrecaoHoraReferencia(carga, carga_clone);
				// atualiza demanda da Carga original na hora de refer�ncia
				AtualizaDemandaCargaOriginal(carga, carga_clone, HoraReferencia);
				// atualiza demanda da Carga original nas horas afetadas
				for (int n = 1; n <= HorasAfetadas; n++)
				{
					if ((hora = HoraReferencia + n) >= 24)
						hora = 0;
					AtualizaDemandaCargaOriginal(carga, hora, fatcor);
				}
			}
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::AtualizaDemandaCargaOriginal(VTCarga* carga_orig,
	VTCarga *carga_clone, int hora)
{
	// vari�veis locais
	complex<double>sf_mva[MAX_FASE];
	VTPatamar *patamar = patamares->ExistePatamar(hora, 0);

	// prote��o
	if (patamar == NULL)
		return (false);

	// verifica se foi feita estima��o equilibrada
	if (TipoEstimacao == estimacaoEQUILIBRADA)
	{ // determina demanda trifasica da Carga clone no Patamar e atualiza Carga original
		demanda->S_mva[carga_orig][patamar->Index] = demanda->S_mva[carga_clone][patamar->Index];
	}
	else
	{ // determina demanda por fase da Carga clone no Patamar
		if (!demanda->Get_S3f_mva(carga_clone, patamar->Index, sf_mva))
			return (false);

		// atualiza demanda por fase da Carga original no Patamar
		if (!demanda->Set_S3f_mva(carga_orig, patamar->Index, sf_mva))
			return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::AtualizaDemandaCargaOriginal(VTCarga* carga, int hora,
	double fatcor)
{
	// vari�veis locais
	complex<double>sf_mva[MAX_FASE];
	VTPatamar *patamar = patamares->ExistePatamar(hora, 0);

	// prote��o
	if (patamar == NULL)
		return (false);
	// determina demanda por fase da Carga no Patamar
	if (!demanda->Get_S3f_mva(carga, patamar->Index, sf_mva))
		return (false);
	// corrige a demanda por fase, multiplicando pelo fator de corre��o
	for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
	{
		sf_mva[ind_fase] *= fatcor;
	}
	// atualiza demanda por fase da Carga no Patamar
	demanda->Set_S3f_mva(carga, patamar->Index, sf_mva);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::AtualizaValorEstimadoMedidorOriginal(VTRedes *redes,
	VTRedes *redes_clone)
{
	// vari�veis locais
	int hora;
	double p_mw, q_mvar, i_amp;
	TList *lisCANAL;
	VTCanal *canal, *cloneP, *cloneQ;
	VTMedidor *medidor, *medidor_clone;
	VTPatamar *patamar;

	// prote��o: verifica se deve salvar curva estimada dos Medidores
	if (!SalvaCurvaEstimada)
		return (true);
	// obt�m lista de Medidores da Rede original
	lisEQP->Clear();
	redes->LisEqbar(lisEQP, eqptoMEDIDOR);
	// obt�m lista de Medidores da Rede clonada
	lisTMP->Clear();
	redes_clone->LisEqbar(lisTMP, eqptoMEDIDOR);
	// verifica se deve atualizar a demanda de todos patamares
	if (HoraReferencia < 0)
	{ // atualiza curva estimada dos canais para todos os patamamres
		for (int i = 0; i < lisEQP->Count; i++)
		{
			medidor = (VTMedidor*)lisEQP->Items[i];
			// obt�m Medidor clonado
			if ((medidor_clone = ExisteMedidor(lisTMP, medidor->Id)) == NULL)
				continue;
			// loop para todos Canais do Medidor original
			lisCANAL = medidor->LisCanal();
			for (int j = 0; j < lisCANAL->Count; j++)
			{
				canal = (VTCanal*)lisCANAL->Items[j];
				// prote��o:
				if (!canal->Enabled)
					continue;
				// loop p/ todos patamares
				for (int np = 0; np < patamares->NumPatamar(); np++)
				{
					patamar = patamares->Patamar[np];
					// verifica se Patamar habilitado
					if (!patamar->Enabled)
						continue;
					// determina canais P/Q do medidor
					cloneP = ExisteMedidorCanal(medidor_clone, canal->Eqpto->Id, canalP,
						canal->Fases);
					cloneQ = ExisteMedidorCanal(medidor_clone, canal->Eqpto->Id, canalQ,
						canal->Fases);
					if ((cloneP == NULL) || (cloneQ == NULL))
						continue;
					// verifica o tipo de canal
					if (canal->TipoCanal == canalP)
					{ // valor estimado
						p_mw = cloneP->ValorEstimado[patamar];
						canal->ValorEstimado[patamar] = p_mw;
						// n�o existe valor nominal para medi��o da chave
						// p_mw   = cloneP->ValorNominal[patamar];
						canal->ValorNominal[patamar] = p_mw;
					}
					else if (canal->TipoCanal == canalQ)
					{ // valor estimado
						q_mvar = cloneQ->ValorEstimado[patamar];
						canal->ValorEstimado[patamar] = q_mvar;
						// n�o existe valor nominal para medi��o da chave
						// q_mvar = cloneQ->ValorNominal[patamar];
						canal->ValorNominal[patamar] = q_mvar;
					}
					else if (canal->TipoCanal == canalI)
					{ // valor estimado
						p_mw = cloneP->ValorEstimado[patamar];
						q_mvar = cloneQ->ValorEstimado[patamar];
						i_amp = (sqrt((p_mw * p_mw) + (q_mvar * q_mvar)) * 1000.) /
							((medidor->pbarra->vnom / sqrt(3.)));
						canal->ValorEstimado[patamar] = i_amp;
						// n�o existe valor nominal para medi��o da chave
						// p_mw   = cloneP->ValorNominal[patamar];
						// q_mvar = cloneQ->ValorNominal[patamar];
						// i_amp = (sqrt((p_mw * p_mw)+(q_mvar * q_mvar)) * 1000.) / ((medidor->pbarra->vnom / sqrt(3.)));
						canal->ValorNominal[patamar] = i_amp;
					}
				}
			}
		}
	}
	else
	{ // atualiza curva estimada dos canais para a hora de refer�ncia
		patamar = patamares->ExistePatamar(HoraReferencia, 0);
		for (int i = 0; i < lisEQP->Count; i++)
		{
			medidor = (VTMedidor*)lisEQP->Items[i];
			// obt�m Medidor clonado
			if ((medidor_clone = ExisteMedidor(lisTMP, medidor->Id)) == NULL)
				continue;
			// loop para todos Canais do Medidor original
			lisCANAL = medidor->LisCanal();
			for (int j = 0; j < lisCANAL->Count; j++)
			{
				canal = (VTCanal*)lisCANAL->Items[j];
				// prote��o:
				if (!canal->Enabled)
					continue;
				// determina canais P/Q do medidor
				cloneP = ExisteMedidorCanal(medidor_clone, canal->Eqpto->Id, canalP, canal->Fases);
				cloneQ = ExisteMedidorCanal(medidor_clone, canal->Eqpto->Id, canalQ, canal->Fases);
				// prote��o
				if ((cloneP == NULL) || (cloneQ == NULL))
				{
					// int a=0;
					continue;
				}
				// verifica o tipo de canal
				if (canal->TipoCanal == canalP)
				{ // valor estimado
					p_mw = cloneP->ValorEstimado[patamar];
					canal->ValorEstimado[patamar] = p_mw;
					// n�o existe valor nominal para medi��o da chave
					// p_mw   = cloneP->ValorNominal[patamar];
					canal->ValorNominal[patamar] = p_mw;
				}
				else if (canal->TipoCanal == canalQ)
				{ // valor estimado
					q_mvar = cloneQ->ValorEstimado[patamar];
					canal->ValorEstimado[patamar] = q_mvar;
					// n�o existe valor nominal para medi��o da chave
					// q_mvar = cloneQ->ValorNominal[patamar];
					canal->ValorNominal[patamar] = q_mvar;
				}
				else if (canal->TipoCanal == canalI)
				{ // valor estimado
					p_mw = cloneP->ValorEstimado[patamar];
					q_mvar = cloneQ->ValorEstimado[patamar];
					i_amp = (sqrt((p_mw * p_mw) + (q_mvar * q_mvar)) * 1000.) /
						((medidor->pbarra->vnom / sqrt(3.)));
					canal->ValorEstimado[patamar] = i_amp;
					// n�o existe valor nominal para medi��o da chave
					// p_mw   = cloneP->ValorNominal[patamar];
					// q_mvar = cloneQ->ValorNominal[patamar];
					// i_amp = (sqrt((p_mw * p_mw)+(q_mvar * q_mvar)) * 1000.) / ((medidor->pbarra->vnom / sqrt(3.)));
					canal->ValorNominal[patamar] = i_amp;
				}
			}
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
double __fastcall TEstimaDemanda::CalculaFatorCorrecaoHoraReferencia(VTCarga* carga_orig,
	VTCarga *carga_clone)
{
	// vari�veis locais
	double fatcor = 1.0;
	double s1_mva, s2_mva;
	VTPatamar *patamar = patamares->ExistePatamar(HoraReferencia, 0);

	// prote��o
	if (patamar != NULL)
	{ // determina demanda da Carga original no Patamar
		s1_mva = Abs(demanda->S_mva[carga_orig][patamar->Index]);
		// determina demanda da Carga clone no Patamar
		s2_mva = Abs(demanda->S_mva[carga_clone][patamar->Index]);
		// calcula fator de multiplica��o para carga original
		if (IsDoubleZero(s1_mva))
			fatcor = 1.0;
		else
			fatcor = s2_mva / s1_mva;
	}
	return (fatcor);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TEstimaDemanda::ConverteCargaTipica(VTRedes *redes)
 {
 //vari�veis locais
 VTStkCarga *stk;
 VTCarga   *carga;

 try{//cria objeto
 if ((stk = DLL_NewObjStkCarga(apl)) == NULL) return(false);
 //obt�m lista de todas as Cargas
 lisEQP->Clear();
 redes->LisEqbar(lisEQP, eqptoCARGA);
 //loop para todas Cargas
 for (int n = 0; n < lisEQP->Count; n++)
 {
 carga = (VTCarga*)lisEQP->Items[n];
 //descarta Cargas  de IP
 if (carga->classe->Tag == consIP) continue;
 //descarta Cargas equivalentes
 if (carga->Reducao) continue;
 if (TipoEstimacao == estimacaoEQUILIBRADA)
 stk->RedefineCargaComDemandaPropriaEquilibrada(carga);
 else
 stk->RedefineCargaComDemandaPropriaDesequilibrada(carga);
 }
 //destr�i objeto
 delete stk;
 }catch(Exception &e)
 {
 return(false);
 }
 return(true);
 }
 */
// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15312678
// https://sinapsisenergia.teamwork.com/#tasks/15372453
bool __fastcall TEstimaDemanda::ConverteCargaTipica(VTRedes *redes)
{
	// vari�veis locais
	int num_fase;
	VTStkCarga *stk;
	VTCarga *carga;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

	try
	{ // cria objeto
		if ((stk = DLL_NewObjStkCarga(apl)) == NULL)
			return (false);
		// obt�m lista de todas as Cargas
		lisEQP->Clear();
		redes->LisEqbar(lisEQP, eqptoCARGA);
		// loop para todas Cargas
		for (int n = 0; n < lisEQP->Count; n++)
		{
			carga = (VTCarga*)lisEQP->Items[n];
			// descarta Cargas  de IP


/*
// DEBUG ESTIMA��O DESEQUILIBRADA
if(carga->Id == 2318)
   {
   int  xxx   = 1;
   bool teste = carga->DefinidaPorFase;
   }
*/


			if (carga->classe->Tag == consIP)
				continue;
			// descarta Cargas trifasicas que j� possuam curva pr�pria
			if (!carga->curva->Tipica)
			{
				if ((num_fase = fases->NumeroDeFases(carga->Fases)) == 3)
				{
					if (!carga->curva->Status[sttNOVO])
						carga->curva->Status[sttALTERADO] = true;
					continue;
				}
			}
			// descarta Cargas equivalentes
			if (!carga->Reducao)
			{ // no caso do estimador equilibrado, altera fases da carga para trifasico
				if (TipoEstimacao == estimacaoEQUILIBRADA)
					stk->RedefineCargaComDemandaPropriaEquilibrada(carga);
				else
					stk->RedefineCargaComDemandaPropriaDesequilibrada(carga);
				// marca que a carga foi alterada
				if (!carga->StatusNovo())
					carga->Status[sttALTERADO] = true;
				carga->curva->Status[sttNOVO] = true;
			}
			// 2018.02.01 - IMPORTANTISSIMO PARA A ESTIMA��O
			// se a demanda no patamar for nula, define um valor m�nimo
			// DefineDemandaMinima(carga);


/*
// DEBUG ESTIMA��O DESEQUILIBRADA
if(carga->Id == 2318)
   {
   int  xxx   = 1;
   bool teste = carga->DefinidaPorFase;
   }
*/


		}
		// destr�i objeto
		delete stk;
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::ConverteMedidores(void)
{
	// vari�veis locais
	VTMedicoes *medicoes;

	try
	{ // cria objeto Medicoes
		if ((medicoes = DLL_NewObjMedicoes(apl)) == NULL)
			return (false);
		// verifica se estima��o equilibrada ou desequilibrada
		if (TipoEstimacao == estimacaoEQUILIBRADA)
		{ // converte medi��es de corrente em PQ trif�sica
			medicoes->CorrenteToPQtrifasico();
		}
		else
		{ // converte medi��es de corrente em PQ por fase
			medicoes->CorrenteToPQfase();
			medicoes->ConverteMedidorPtrifasicoToPQfase();
		}
		// destr�i objeto Medicoes
		delete medicoes;
	}
	catch (Exception &e)
	{ // nada a fazer
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TEstimaDemanda::DefineDemandaMinima(VTCarga *carga)
{
	// vari�veis locais
	bool atualizar;
	complex<double>sf_mva[MAX_FASE];
	VTPatamar *patamar;

	// verifica se deve atualizar a demanda de todos patamares
	if (HoraReferencia < 0)
	{ // loop p/ todos patamares
		for (int np = 0; np < patamares->NumPatamar(); np++)
		{
			patamar = patamares->Patamar[np];
			atualizar = false;
			// determina demanda por fase da Carga clone no Patamar
			if (!demanda->Get_S3f_mva(carga, patamar->Index, sf_mva))
				return;
			for (int ind_fase = indFASE_A, fase = faseA; ind_fase <= indFASE_C; ind_fase++, fase++)
			{
				if ((carga->Fases & fase) == fase)
				{
					if (IsComplexZero(sf_mva[ind_fase]))
					{ // insere demanda de 1 W
						sf_mva[ind_fase] = complex<double>(100.e-6, 100.e-6);
						atualizar = true;
					}
				}
			}
			// atualiza demanda por fase da Carga original no Patamar
			if (atualizar)
				demanda->Set_S3f_mva(carga, patamar->Index, sf_mva);
		}
	}
	else
	{ // atualiza somente o patamar selecionado
		patamar = patamares->ExistePatamar(HoraReferencia, 0);
		atualizar = false;
		// determina demanda por fase da Carga clone no Patamar
		if (!demanda->Get_S3f_mva(carga, patamar->Index, sf_mva))
			return;
		for (int ind_fase = indFASE_A, fase = faseA; ind_fase <= indFASE_C; ind_fase++, fase++)
		{
			if ((carga->Fases & fase) == fase)
			{
				if (IsComplexZero(sf_mva[ind_fase]))
				{ // insere demanda de 1 W
					sf_mva[ind_fase] = complex<double>(100.e-6, 100.e-6);
					atualizar = true;
				}
			}
		}
		// atualiza demanda por fase da Carga original no Patamar
		if (atualizar)
			demanda->Set_S3f_mva(carga, patamar->Index, sf_mva);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::DesabilitaCanaisChaveFechadaMedicaoNula(VTRedes *redes_clone)
{
	// vari�veis locais
	TList *lisCANAL;
	VTChave *chave;
	VTCanal *canal;
	VTMedidor *medidor;
	VTPatamar *patamar;
	TList *lisPAT = patamares->LisPatamar();

	// obt�m lista de Medidores
	lisEQP->Clear();
	redes_clone->LisEqbar(lisEQP, eqptoMEDIDOR);
	for (int nm = 0; nm < lisEQP->Count; nm++)
	{
		medidor = (VTMedidor*)lisEQP->Items[nm];
		lisCANAL = medidor->LisCanal();
		// retira os Canais de corrente
		for (int nc = 0; nc < lisCANAL->Count; nc++)
		{
			canal = (VTCanal*)lisCANAL->Items[nc];
			if (canal->Eqpto->Tipo() != eqptoCHAVE)
				continue;
			// verifica se Chave est� fechada
			chave = (VTChave*)canal->Eqpto;
			if (chave->Estado != chvFECHADA)
				continue;
			// verifica se valor medido do Canal � nulo em algum algum dos patamares habilitados
			for (int np = 0; np < lisPAT->Count; np++)
			{
				patamar = (VTPatamar*)lisPAT->Items[np];
				if (patamar->Enabled)
				{
					if (IsDoubleZero(canal->ValorMedido[patamar]))
					{ // desabilita o Canal
						canal->Enabled = false;
						break;
					}
				}
			}
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::EstimacaoPodeSerFeita(VTRede *rede)
{
	// vari�veis locais
	// TList   *lisLIG;
	VTCarga *carga;

	// verifica se a Rede possui uma Ligacao al�m da Chave inicial
	// lisLIG = rede->LisLigacao();
	// if (lisLIG->Count <= 1)  return(false);
	// verifica se h� Cargas ajust�veis
	lisEQP->Clear();
	rede->LisEqbar(lisEQP, eqptoCARGA);
	// verifica se h� Cargas que n�o s�o IP
	for (int n = 0; n < lisEQP->Count; n++)
	{
		carga = (VTCarga*)lisEQP->Items[n];
		if (carga->classe->Tag != consIP)
			return (true);
	}
	Log("Estima��o n�o pode ser feita para rede " + rede->Codigo + ": n�o ha carga");
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::EstimacaoPodeSerFeita(VTRedes *redes)
{
	// vari�veis locais
	// TList   *lisLIG;
	VTCarga *carga;

	// verifica se a Rede possui uma Ligacao al�m da Chave inicial
	// lisLIG = rede->LisLigacao();
	// if (lisLIG->Count <= 1)  return(false);
	// verifica se h� Cargas ajust�veis
	lisEQP->Clear();
	redes->LisEqbar(lisEQP, eqptoCARGA);
	// verifica se h� Cargas que n�o s�o IP
	for (int n = 0; n < lisEQP->Count; n++)
	{
		carga = (VTCarga*)lisEQP->Items[n];
		if (carga->classe->Tag != consIP)
			return (true);
	}
	Log("Estima��o n�o pode ser feita para o conjunto de redes: n�o ha carga");
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::ExecutaEstimacaoDemanda(VTRedes *redes, VTRedes *redes_clone)
{
	try
	{ // converte medi��es de corrente em medi��es de P/Q
		Log("Converte medidores");
		if (!ConverteMedidores())
			return (false);
		// elimina medi��es de corrente
		RetiraMedicaoI();
		// cria medi��es de P/Q para todas Barras com Cargas
		if (!InsereMedidoresBarrasDeCarga())
			return (false);
		// TESTE
		// return(true);
		// verifica op��o de reduzir a rede
		if (TipoReducao == reducaoZIP)
		{ // reduz a Rede clonada
			Log("Monta rede reduzida");
			MontaRedeZIP();
		}
		else if (TipoReducao == reducaoEQV)
		{ // reduz a Rede clonada
			Log("Monta rede equivalente");
			MontaRedeEquivalente();
		}
		// executa Estimador com a Rede clonada
		Log("Inicia estima��o de estado");
		if (!ExecutaEstimacaoEstado(redes_clone))
			return (false);
		// atualiza valores estimados dos medidores da rede original
		Log("Atualiza medidores");
		AtualizaValorEstimadoMedidorOriginal(redes, redes_clone);
		// atualiza demanda de todas Cargas da rede clonada
		Log("Atualiza cargas");
		if (!AtualizaDemandaCargaClonada())
			return (false);
		// prote��o: verifica se h� cargas clonadas com pot.ativa negativa
		if (!ValidaDemandaCargasClonadas(redes_clone))
			return (false);
		// atualiza demanda de todas Cargas da rede original
		AtualizaDemandaCargaOriginal(redes, redes_clone);
		// N�O retira Medidores das Barras de Carga porque s�o criados na rede clonada que ser� destru�da
		// monta->RetiraMedidores();
		//roda fluxo
		flow->UpToDate = false;
                flow->CalculaPorRede(redes);
	}
	catch (Exception &e)
	{
		Log("Erro durante estima��o de demanda");
		return (false);
	}

	// HPS - 2018.12.08 - grava��o de arquivo com demandas das cargas
	// TESTE_GravaArquivoCarga("EstimaDemanda_CargasIntermed1.csv");

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::ExecutaEstimacaoEstado(VTRedes *redes_clone)
{
	// vari�veis locais
	VTEstimador *estimador;
	VTData *data;
	VTNet *net;
	VTOrdena *ordena;
	bool sucesso = false;

	try
	{ // cria objeto Ordena
		if ((ordena = DLL_NewObjOrdena(apl)) != NULL)
		{ // configura op��es de montagem de VTNet
			ordena->ChaveNA_Enabled = false;
			ordena->ChaveNF_Enabled = true;

			// Defini��o HPS - 2017.09.05
			ordena->TramoEnabled = false;
			// ordena->TramoEnabled    = true;

			// cria objeto Net
			Log("Ordena redes");
			if ((net = ordena->Executa(redes_clone)) != NULL)
			{ // cria objeto VTData
				if ((data = DLL_NewObjData(apl)) != NULL)
				{ // configura op��es do objeto VTData
					data->RedeComCarga = true;
					data->IncluirChave = true;
					data->ModeloCarga = cargaAUTO;
					data->PerdaFerroEnabled = true;
					data->Inicia(apl, net);
					// cria Estimador equilibrado/desequilibrado
					if (TipoEstimacao == estimacaoEQUILIBRADA)
					{
						estimador = DLL_NewObjEstimador1(apl);
					}
					else
					{
						estimador = DLL_NewObjEstimador3(apl);
					}
					if (estimador != NULL)
					{ // verifica se m�todo convencional
						if (MetodoEE == meeCONVENCIONAL)
						{
							estimador->LimitaSigmas  = true;
							estimador->SigmaMinimoPu = 5.e-3;
							estimador->SigmaMaximoPu = 1.e-2;
						}
						else // Usa a Decomposi��o QR
						{
							estimador->LimitaSigmas      = false;
							estimador->OrdenacaoLinhasQR = OrdenacaoLinhasQR;
						}
						// executa Estimador
						estimador->TipoBarraRef = brFIXOS_nTHETA_nV;

						// DEBUG
						// estimador->TipoBarraRef    = brFIXOS_1THETA_0V;
						// estimador->TipoBarraRef    = brFIXOS_1THETA_1V;
						// estimador->TipoBarraRef    = brFIXOS_nTHETA_0V;
						// estimador->TipoBarraRef    = brFIXOS_nTHETA_nV;

						estimador->FatMultMedChave = FatMultMedChave;
						estimador->MetodoEE        = MetodoEE;
						estimador->TipoRelatorio   = TipoRelatorio;
						estimador->UtilizaVDNM     = UtilizaVDNM;
						estimador->NumMaxMontH     = NumMaxMontH;
						Log("Inicia processo estimador de estado");
						sucesso = estimador->Executa(data);
					}
				}
			}
		}
		// destr�i objetos
		if (estimador)
		{
			delete estimador;
		}
		if (data)
		{
			delete data;
		}
		if (net)
		{
			DLL_DeleteEqpto(net);
		}
		if (ordena)
		{
			delete ordena;
		}
	}
	catch (Exception &e)
	{
		Alerta("Estima��o de Estado: erro na execu��o");
		Log("Erro na execu��o do estimador de estado");
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::ExecutaPorRedePrimaria(VTRedes *redes)
{
	// vari�veis locais
	bool sucesso;
	VTRede *rede;
	VTRedes *redes_clone;
	TList *lisREDE = redes->LisRede();

	try
	{ // verifica se Log est� habilitado
		IniciaLog();
		// reinicia listas de diagn�stico, assumindo que todas as redes n�o foram estimadas
		diag.StrLisRede_OK->Clear();
		diag.StrLisRede_NOK->Clear();
		// converte cargas t�picas em Cargas com curva pr�pria
		if (!ConverteCargaTipica(redes))
		{
			Log("Cargas n�o convertidas");
			FechaLog();
			return (false);
		}
		// executa fluxo de pot�ncia, caso seja necess�rio
		if (TipoReducao == reducaoEQV)
			flow->ReguladorBypass = true; // OBRIGAT�RIO!!! (HPS, 2016.09.03)
		if (TipoEstimacao == estimacaoEQUILIBRADA)
			flow->TipoFluxo = fluxoNR1;
		else
			flow->TipoFluxo = fluxoNR3;
		if (!flow->CalculaPorRede(redes))
		{
			Log("Fluxo n�o calculado");
			FechaLog();
			return (false);
		}
		// loop para todas Redes em lisREDE
		for (int n = 0; n < lisREDE->Count; n++)
		{
			rede = (VTRede*)lisREDE->Items[n];
			// verifica se a rede est� carregada
			if (!rede->Carregada)
				continue;
			// verifica se � uma Rede prim�ria
			if (rede->TipoRede->Segmento != redePRI)
				continue;
			// prote��o: verifica se a Estima��o � aplic�vel � Rede
			if (!EstimacaoPodeSerFeita(rede))
				continue;
			// monta um novo VTRedes com uma Rede que � c�pia da Rede indicada
			if ((redes_clone = copia->DuplicaUmaRede(redes, rede)) == NULL)
			{ // insere rede da lista de redes n�o estimadas
				diag.StrLisRede_NOK->Add(rede->Codigo);
				continue;
			}
			// retira Cargas de IP da rede clonada
			RetiraCargasIP(redes_clone);
			// retira Canais associados a Chave aberta
			RetiraCanaisChaveAberta(redes_clone);
			// DESATIVADO (HPS, 2017.09.05)
			// seguran�a: elimina Canais desabilitados
			// RetiraCanaisDesabilitados(redes_clone);
			// desabilita canais de chave fechada com medi��o nula
			DesabilitaCanaisChaveFechadaMedicaoNula(redes_clone);
			// insere c�pia de Redes em apl
			apl->Add(redes_clone);
			// insere Suprimento na Barra inicial da Rede clonada
			sucesso = InsereSuprimentoRedeClonada(redes_clone);
			// executa estima��o de demanda
			if (sucesso)
				sucesso = ExecutaEstimacaoDemanda(redes, redes_clone);
			if (sucesso)
			{ // insere rede da lista de redes estimadas
				diag.StrLisRede_OK->Add(rede->Codigo);
				Log("Estima��o calculada para a rede " + rede->Codigo);
			}
			else
			{ // insere rede da lista de redes n�o estimadas
				diag.StrLisRede_NOK->Add(rede->Codigo);
				Log("Estima��o n�o calculada para a rede " + rede->Codigo);
			}
			// remove redes_clone do Apl e destr�i objeto
			apl->Remove(redes_clone);
			delete redes_clone;
		}
		FechaLog();
	}
	catch (Exception &e)
	{
		FechaLog();
		return (false);
	}
	return (diag.StrLisRede_NOK->Count == 0);
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::ExecutaRedeUnica(VTRedes *redes)
{
	// vari�veis locais
	bool sucesso;
	VTRedes *redes_clone;

	try
	{ // verifica se Log est� habilitado
		IniciaLog();
		// converte cargas t�picas em Cargas com curva pr�pria
		Log("Converte cargas t�picas em Cargas com curva pr�pria");
		if (!ConverteCargaTipica(redes))
		{
			Log("Cargas n�o convertidas");
			FechaLog();
			return (false);
		}
		// TESTE: apenas altera curva t�pica para curva pr�pria
		// return(true);
		// executa fluxo de pot�ncia, caso seja necess�rio
		if (TipoReducao == reducaoEQV)
			flow->ReguladorBypass = true; // OBRIGAT�RIO!!! (HPS, 2016.09.03)
		if (TipoEstimacao == estimacaoEQUILIBRADA)
			flow->TipoFluxo = fluxoNR1;
		else
			flow->TipoFluxo = fluxoNR3;
		Log("Roda fluxo");
		if (!flow->CalculaPorRede(redes))
		{
			FechaLog();
			return (false);
		}
		// prote��o: verifica se a Estima��o � aplic�vel � Rede
		Log("Verifica se a Estima��o � aplic�vel � Rede");
		if (!EstimacaoPodeSerFeita(redes))
		{
			Log("O estimador n�o pode ser executado");
			FechaLog();
			return (true);
		}
		// monta um novo VTRedes com c�pia da Rede indicada
		Log("Duplica rede, guardando estado original");
		if ((redes_clone = copia->DuplicaRedes(redes)) == NULL)
		{
			Log("Erro na duplica��o das redes");
			FechaLog();
			return (false);
		}
		// retira Cargas de IP da rede clonada
		RetiraCargasIP(redes_clone);
		// retira Canais associados a Chave aberta
		RetiraCanaisChaveAberta(redes_clone);
		// DESATIVADO (HPS, 2017.09.05)
		// seguran�a: elimina Canais desabilitados
		// RetiraCanaisDesabilitados(redes_clone);
		// desabilita canais de chave fechada com medi��o nula
		DesabilitaCanaisChaveFechadaMedicaoNula(redes_clone);
		// insere c�pia de Redes em apl
		apl->Add(redes_clone);
		// executa estima��o de demanda
		Log("Inicia processo estima��o de demanda");
		sucesso = ExecutaEstimacaoDemanda(redes, redes_clone);
		// remove redes_clone do Apl e destr�i objeto
		apl->Remove(redes_clone);
		delete redes_clone;
		FechaLog();
	}
	catch (Exception &e)
	{
		sucesso = false;
		FechaLog();
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
VTCarga* __fastcall TEstimaDemanda::ExisteCarga(TList *lisCAR, int carga_id)
{
	// vari�veis locais
	VTCarga *carga;

	// loop para todas Cargas em lisCAR
	for (int n = 0; n < lisCAR->Count; n++)
	{
		carga = (VTCarga*)lisCAR->Items[n];
		if (carga->Id == carga_id)
			return (carga);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTMedidor* __fastcall TEstimaDemanda::ExisteMedidor(TList *lisMED, int medidor_id)
{
	// vari�veis locais
	VTMedidor *medidor;

	// loop para todas Medidores em lisMED
	for (int n = 0; n < lisMED->Count; n++)
	{
		medidor = (VTMedidor*)lisMED->Items[n];
		if (medidor->Id == medidor_id)
			return (medidor);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTCanal* __fastcall TEstimaDemanda::ExisteMedidorCanal(VTMedidor *medidor, int eqpto_id,
	int tipo_canal, int fases)
{
	// vari�veis locais
	VTCanal *canal;
	TList *lisCANAL = medidor->LisCanal();

	for (int n = 0; n < lisCANAL->Count; n++)
	{
		canal = (VTCanal*)lisCANAL->Items[n];
		if ((canal->Eqpto->Id == eqpto_id) && (canal->TipoCanal == tipo_canal) &&
			(canal->Fases == fases))
			return (canal);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TEstimaDemanda::FechaLog(void)
{
	// verifica se existe objeto Log
	if (log != NULL)
	{ // fecha Log
		Log("Fim do estimador de demanda");
		log->Close();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEstimaDemanda::IniciaLog(void)
{
	// vari�veis locais
	bool append = true;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// prote��o
	if (path == NULL)
		return;
	// verifica se deve criar objeto Log
	if (LogEnabled && (log == NULL))
	{ // cria objeto Log
		apl->Add(log = DLL_NewObjLog(apl));
		log->Open(path->DirTmp() + "\\EstimaDemanda.log", append);
		Log("In�cio do estimador de demanda");
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::InsereCurvaSuprimento(VTSuprimento *suprimento)
{
	// vari�veis locais
	strSUP str_sup;
	VTCurva *curva;

	// cria uma nova Curva
	if ((curva = DLL_NewObjCurva()) == NULL)
		return (false);
	// associa Curva c/ a Carga
	suprimento->DefineCurva(curva);
	// define dados da Curva
	curva->Codigo = suprimento->Codigo;
	curva->Id = suprimento->Id; // MUITO IMPORTANTE
	curva->Tipo = curvaPQVT;
	curva->Unidade = unidPU;
	curva->Escala = pat01HORA;
	curva->Tipica = false;
	curva->NumeroValores = nvCURVA_PQVT; // {p,q,v,t}
	// inicia valores da Curva
	for (int np = 0; np < curva->Escala; np++)
	{ // inicia tens�es do Suprimento c/ 1 pu
		str_sup.p = 0;
		str_sup.q = 0;
		str_sup.v = 1.0;
		str_sup.t = 0.0;
		fornecimento->Set_PQVT_porfase(suprimento, np, &str_sup);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::InsereMedidoresBarrasDeCarga(void)
{
	// vari�veis locais
	bool sucesso;
	VTMontaMedicao *monta;

	try
	{ // destr�i eventual objeto VTMontaMedicao criado anteriormente
		monta = (VTMontaMedicao*)apl->GetObject(__classid(VTMontaMedicao));
		if (monta != NULL)
		{
			apl->Remove(monta);
			delete monta;
		}
		// cria objeto VTMontaMedicao
		if (TipoEstimacao == estimacaoEQUILIBRADA)
			monta = NewObjMontaMedicaoEquilibrada(apl);
		else
			monta = NewObjMontaMedicaoDesequilibrada(apl);
		// prote��o
		if (monta == NULL)
			return (false);
		// insere VTMontaMedicao em Apl
		apl->Add(monta);
		// destr�i Medidores criados anteriormente
		monta->RetiraMedidores();
		// insere novo conjunto de Medidores
		sucesso = monta->InsereMedidores();
		// destr�i objeto
		// delete monta;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::InsereSuprimentoRedeClonada(VTRedes *redes_clone)
{
	// vari�veis locais
	VTBarra *barra;
	VTRede *rede;
	VTSuprimento *suprimento;
	TList *lisREDE = redes_clone->LisRede();

	// prote��o
	if (lisREDE->Count != 1)
		return (false);
	// determina Rede
	rede = (VTRede*)lisREDE->First();
	// determina Barra inicial da Rede
	if ((barra = rede->BarraInicial()) == NULL)
		return (false);
	// verifica se j� existe um Suprimento na Barra inicial
	if (barra->ExisteGeracaoVF())
		return (true);
	// cria um Suprimento e insere na Barra inicial
	if ((suprimento = DLL_NewObjSuprimento(barra->resflow->Max_pat)) == NULL)
		return (false);
	// cria Curva do Suprimento
	InsereCurvaSuprimento(suprimento);
	// insere SUprimento na Barra inicial
	barra->InsereEqbar(suprimento);

	return (true);
}

// ---------------------------------------------------------------------------
TStrings* __fastcall TEstimaDemanda::StrLisRedeEstimada(void)
{
	return (diag.StrLisRede_OK);
}

// ---------------------------------------------------------------------------
TStrings* __fastcall TEstimaDemanda::StrLisRedeNaoEstimada(void)
{
	return (diag.StrLisRede_NOK);
}

// ---------------------------------------------------------------------------
void __fastcall TEstimaDemanda::Log(AnsiString txt, bool data)
{
	// var locais
	AnsiString texto = txt;
	TDateTime dt;

	if (log)
	{
		if (data)
		{
			dt = Now();
			texto = dt.DateTimeString() + "\t" + txt;
		}
		log->Add(texto);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::MontaRedeEquivalente(void)
{
	// vari�veia locais
	bool calcular_fluxo = false;
	bool sucesso;
	VTRedutor *redutor;

	// monta lista c/ todas Barras que devem permanecer na rede
	lisBAR->Clear();
	SelecionaBarraEqbar(lisBAR, eqptoSUPRIMENTO);
	SelecionaBarraEqbar(lisBAR, eqptoGERADOR);
	SelecionaBarraEqbar(lisBAR, eqptoCARGA);
	SelecionaBarraEqbar(lisBAR, eqptoCAPACITOR);
	// SelecionaBarraChave(lisBAR, chvFECHADA);
	// SelecionaBarraLigacao(lisBAR, eqptoTRAFO);
	SelecionaBarraEqptoComMedicao(lisBAR);
	// SelecionaBarraETD(lisBAR);
	try
	{ // cria Redutor e monta equivalente
		redutor = DLL_NewObjRedutor(apl);
		if (TipoRelatorio == relNENHUM)
			redutor->AvisoEnabled = false;
		else
			redutor->AvisoEnabled = true;

		// HPS, 2016.09.02
		redutor->AvisoEnabled = false; // Rede da EDAL (Pri+Sec) gera relat�rio enorme

		redutor->ModeloEqv = (TipoEstimacao == estimacaoEQUILIBRADA) ? eqvEQUILIBRADO :
			eqvDESEQUILIBRADO;
		sucesso = redutor->ExecutaReducaoAllRede(lisBAR, calcular_fluxo);
		// destr�i Redutor
		delete redutor;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	// reinicia lisBAR
	lisBAR->Clear();
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::MontaRedeZIP(void)
{
	// vari�veia locais
	bool sucesso = false;
	TZip *zip;

	try
	{ // cria objeto
		zip = new TZip(apl);
		if (zip->RetiraChavesSemMedicao())
		{
			sucesso = zip->FundeTrechos();
		}
		delete zip;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
void __fastcall TEstimaDemanda::PM_SetHoraReferencia(int hora)
{
	// vari�veis locais
	VTPatamar *patamar;
	TList *lisPAT = patamares->LisPatamar();

	// salva hora indicada
	PD.hora_referencia = hora;
	// verifica se foi selecionada op��o de todos patamares
	if (hora < 0)
	{ // habilita todos patamares
		for (int np = 0; np < lisPAT->Count; np++)
		{
			patamar = (VTPatamar*)lisPAT->Items[np];
			patamar->Enabled = true;
		}
	}
	else
	{ // habilita o patamar selecionado e desabilita todos os demais
		for (int np = 0; np < lisPAT->Count; np++)
		{
			patamar = (VTPatamar*)lisPAT->Items[np];
			patamar->Enabled = ((hora >= patamar->HoraIni) && (hora < patamar->HoraFim));
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEstimaDemanda::RetiraCanaisChaveAberta(VTRedes *redes_clone)
{
	// vari�veis locais
	TList *lisCANAL;
	VTChave *chave;
	VTCanal *canal;
	VTMedidor *medidor;

	// reinicia lisTMP para armazenar os Canais removidos dos Medidores
	lisTMP->Clear();
	// obt�m lista de Medidores
	lisEQP->Clear();
	redes_clone->LisEqbar(lisEQP, eqptoMEDIDOR);
	for (int nm = 0; nm < lisEQP->Count; nm++)
	{
		medidor = (VTMedidor*)lisEQP->Items[nm];
		lisCANAL = medidor->LisCanal();
		// retira os Canais de corrente
		for (int nc = lisCANAL->Count - 1; nc >= 0; nc--)
		{
			canal = (VTCanal*)lisCANAL->Items[nc];
			if (canal->Eqpto->Tipo() == eqptoCHAVE)
			{
				chave = (VTChave*)(canal->Eqpto);
				if (chave->Aberta)
				{ // retira Canal de lisCANAL e insere em lisTM
					lisCANAL->Delete(nc);
					lisTMP->Add(canal);
				}
			}
		}
	}
	// destr�i todos os Canais em lisTMP
	LimpaTList(lisTMP);
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::RetiraCanaisChaveFechadaMedicaoNula(VTRedes *redes_clone)
{
	// vari�veis locais
	TList *lisCANAL;
	VTChave *chave;
	VTCanal *canal;
	VTMedidor *medidor;
	VTPatamar *patamar;
	TList *lisPAT = patamares->LisPatamar();

	// reinicia lisTMP para armazenar os Canais removidos dos Medidores
	lisTMP->Clear();
	// obt�m lista de Medidores
	lisEQP->Clear();
	redes_clone->LisEqbar(lisEQP, eqptoMEDIDOR);
	for (int nm = 0; nm < lisEQP->Count; nm++)
	{
		medidor = (VTMedidor*)lisEQP->Items[nm];
		lisCANAL = medidor->LisCanal();
		// retira os Canais de corrente
		for (int nc = lisCANAL->Count - 1; nc >= 0; nc--)
		{
			canal = (VTCanal*)lisCANAL->Items[nc];
			if (canal->Eqpto->Tipo() != eqptoCHAVE)
				continue;
			// verifica se Chave est� fechada
			chave = (VTChave*)canal->Eqpto;
			if (chave->Estado != chvFECHADA)
				continue;
			// verifica se valor medido do Canal � nulo em algum algum dos patamares habilitados
			for (int np = 0; np < lisPAT->Count; np++)
			{
				patamar = (VTPatamar*)lisPAT->Items[np];
				if (patamar->Enabled)
				{
					if (IsDoubleZero(canal->ValorMedido[patamar]))
					{ // retira Canal de lisCANAL e insere em lisTMP
						lisCANAL->Delete(nc);
						lisTMP->Add(canal);
						break;
					}
				}
			}
		}
	}
	// destr�i todos os Canais em lisTMP
	LimpaTList(lisTMP);

	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TEstimaDemanda::RetiraCanaisDesabilitados(VTRedes *redes_clone)
{
	// vari�veis locais
	TList *lisCANAL;
	VTCanal *canal;
	VTMedidor *medidor;

	// reinicia lisTMP para armazenar os Canais removidos dos Medidores
	lisTMP->Clear();
	// obt�m lista de Medidores
	lisEQP->Clear();
	redes_clone->LisEqbar(lisEQP, eqptoMEDIDOR);
	for (int nm = 0; nm < lisEQP->Count; nm++)
	{
		medidor = (VTMedidor*)lisEQP->Items[nm];
		lisCANAL = medidor->LisCanal();
		// retira os Canais de corrente
		for (int nc = lisCANAL->Count - 1; nc >= 0; nc--)
		{
			canal = (VTCanal*)lisCANAL->Items[nc];
			if (!canal->Enabled)
			{ // retira Canal de lisCANAL e insere em lisTMP
				lisCANAL->Delete(nc);
				lisTMP->Add(canal);
			}
		}
	}
	// destr�i todos os Canais em lisTMP
	LimpaTList(lisTMP);
}

// ---------------------------------------------------------------------------
void __fastcall TEstimaDemanda::RetiraCargasIP(VTRedes *redes_clone)
{
	// vari�veis locais
	VTCarga *carga;
	VTRede *rede;
	TList *lisREDE = redes_clone->LisRede();

	// loop para todas as redes
	for (int nr = 0; nr < lisREDE->Count; nr++)
	{
		rede = (VTRede*)lisREDE->Items[nr];
		// obt�m lista com todas as Cargas da Rede
		lisTMP->Clear();
		rede->LisEqbar(lisTMP, eqptoCARGA);
		for (int nc = lisTMP->Count - 1; nc >= 0; nc--)
		{
			carga = (VTCarga*)lisTMP->Items[nc];
			if (carga->IP)
			{ // retira Carga da Barra e destr�i objeto
				carga->pbarra->RemoveEqbar(carga);
				DLL_DeleteEqpto(carga);
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEstimaDemanda::RetiraMedicaoI(void)
{
	// vari�veis locais
	TList *lisCANAL;
	VTCanal *canal;
	VTMedidor *medidor;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// reinicia lisTMP para armazenar os Canais removidos dos Medidores
	lisTMP->Clear();
	// obt�m lista de Medidores
	lisEQP->Clear();
	redes->LisEqbar(lisEQP, eqptoMEDIDOR);
	for (int nm = 0; nm < lisEQP->Count; nm++)
	{
		medidor = (VTMedidor*)lisEQP->Items[nm];
		lisCANAL = medidor->LisCanal();
		// retira os Canais de corrente
		for (int nc = lisCANAL->Count - 1; nc >= 0; nc--)
		{
			canal = (VTCanal*)lisCANAL->Items[nc];
			if (canal->TipoCanal == canalI)
			{ // retira Canal de lisCANAL e insere em lisTM
				lisCANAL->Delete(nc);
				lisTMP->Add(canal);
			}
		}
	}
	// destr�i todos os Canais em lisTMP
	LimpaTList(lisTMP);
}

// ---------------------------------------------------------------------------
void __fastcall TEstimaDemanda::SelecionaBarraChave(TList *lisEXT, int estado)
{
	// vari�veis locais
	VTBarra *barra;
	VTChave *chave;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// obt�m lista de Geradores e Suprimentos da rede
	lisTMP->Clear();
	redes->LisLigacao(lisTMP, eqptoCHAVE);
	for (int n = 0; n < lisTMP->Count; n++)
	{
		chave = (VTChave*)lisTMP->Items[n];
		if (chave->Estado != unsigned(estado))
			continue;
		for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
		{
			if ((barra = chave->Barra(ind_bar)) == NULL)
				continue;
			if (lisEXT->IndexOf(barra) < 0)
			{ // insere Barra do Eqbar em lisBAR_PRE
				lisEXT->Add(barra);
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEstimaDemanda::SelecionaBarraEqbar(TList *lisEXT, int tipo_eqbar)
{
	// vari�veis locais
	VTEqbar *eqbar;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// obt�m lista de Geradores e Suprimentos da rede
	lisTMP->Clear();
	redes->LisEqbar(lisTMP, tipo_eqbar);
	for (int n = 0; n < lisTMP->Count; n++)
	{
		eqbar = (VTEqbar*)lisTMP->Items[n];
		if (lisEXT->IndexOf(eqbar->pbarra) < 0)
		{ // insere Barra do Eqbar em lisBAR_PRE
			lisEXT->Add(eqbar->pbarra);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEstimaDemanda::SelecionaBarraLigacao(TList *lisEXT, int tipo_ligacao)
{
	// vari�veis locais
	VTBarra *barra;
	VTLigacao *ligacao;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// obt�m lista de Geradores e Suprimentos da rede
	lisTMP->Clear();
	redes->LisLigacao(lisTMP, tipo_ligacao);
	for (int n = 0; n < lisTMP->Count; n++)
	{
		ligacao = (VTLigacao*)lisTMP->Items[n];
		for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
		{
			if ((barra = ligacao->Barra(ind_bar)) == NULL)
				continue;
			if (lisEXT->IndexOf(barra) < 0)
			{ // insere Barra do Eqbar em lisBAR_PRE
				lisEXT->Add(barra);
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEstimaDemanda::SelecionaBarraEqptoComMedicao(TList *lisEXT)
{
	// vari�veis locais
	TList *lisCANAL;
	VTBarra *barra;
	VTCanal *canal;
	VTChave *chave;
	VTEqpto *eqpto;
	VTLigacao *ligacao;
	VTMedidor *medidor;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// obt�m lista de Geradores e Suprimentos da rede
	lisTMP->Clear();
	redes->LisEqbar(lisTMP, eqptoMEDIDOR);
	for (int n = 0; n < lisTMP->Count; n++)
	{
		medidor = (VTMedidor*)lisTMP->Items[n];
		// inclui Barra do Medidor
		if (lisEXT->IndexOf(medidor->pbarra) < 0)
			lisEXT->Add(medidor->pbarra);
		// obt�m lista de Eqptos associados aos Canais
		lisEQP->Clear();
		medidor->LisEqptoDosCanais(lisEQP);
		// loop p/ todos os Eqptos associados aos Canais
		for (int i = 0; i < lisEQP->Count; i++)
		{
			eqpto = (VTEqpto*)lisEQP->Items[i];
			if (eqpto->TipoBarra())
			{ // inclui Barra
				barra = (VTBarra*)eqpto;
				if (lisEXT->IndexOf(barra) < 0)
					lisEXT->Add(barra);
			}
			else if (eqpto->TipoEqbar())
			{ // inclui Barra do Eqbar
				barra = ((VTEqbar*)eqpto)->pbarra;
				if (lisEXT->IndexOf(barra) < 0)
					lisEXT->Add(barra);
			}
			else if (eqpto->TipoLigacao())
			{ // OBS: n�o inclui Barras de Chave aberta
				ligacao = (VTLigacao*)eqpto;
				// verifica se a Ligacao � uma Chave aberta
				if (ligacao->Tipo() == eqptoCHAVE)
				{
					chave = (VTChave*)ligacao;
					if (chave->Aberta)
					{
						continue;
					}
				}
				// inclui Barras da Ligacao
				for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
				{
					if ((barra = ligacao->Barra(ind_bar)) == NULL)
						continue;
					if (lisEXT->IndexOf(barra) < 0)
						lisEXT->Add(barra);
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEstimaDemanda::SelecionaBarraETD(TList *lisEXT)
{
	// vari�veis locais
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();

	// obt�m lista de Geradores e Suprimentos da rede
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		if (rede->TipoRede->Segmento == redeETD)
		{
			rede->LisBarra(lisEXT);
		}
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TEstimaDemanda::ValidaDemandaCargasClonadas(VTRedes *redes_clone)
{
	// vari�veis locais
	bool atualiza;
	double p_mw;
	complex<double>sf_mva[MAX_FASE];
	VTCarga *carga;
	VTPatamar *patamar;
	// double          pot_descartada_mw = 0;

	// obt�m lista de Cargas da Rede clonada
	lisEQP->Clear();
	redes_clone->LisEqbar(lisEQP, eqptoCARGA);
	// verifica se o ajuste � de um �nico patamar
	if (HoraReferencia < 0)
	{ // loop p/ todos patamares
		for (int np = 0; np < patamares->NumPatamar(); np++)
		{
			patamar = patamares->Patamar[np];
			// zera demandas  pequenas
			for (int n = 0; n < lisEQP->Count; n++)
			{
				carga = (VTCarga*)lisEQP->Items[n];
				// n�o analisa cargas de IP
				if (carga->IP)
					continue;
				atualiza = false;
				// determina demanda por fase da Carga no Patamar
				if (!demanda->Get_S3f_mva(carga, patamar->Index, sf_mva))
				{
					// return(false); // Desabilitado em 2018.08.20, HPS/JCG
					continue;
				}
				// verifica se h� pot.ativa negativa apenas
				for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
				{
					p_mw = sf_mva[ind_fase].real();
					if ((p_mw < 0) && (p_mw > -30.0e-3))
					{ // zera demanda na fase
						sf_mva[ind_fase] = CZero();
						atualiza = true;
						// pot_descartada_mw += p_mw;
					}
				}
				// se demanda alterada, atualiza carga
				if (atualiza)
					demanda->Set_S3f_mva(carga, patamar->Index, sf_mva);
			}
			// verifica se restaram Cargas com pot.ativa negativa
			for (int n = 0; n < lisEQP->Count; n++)
			{
				carga = (VTCarga*)lisEQP->Items[n];
				// determina demanda por fase da Carga no Patamar
				if (!demanda->Get_S3f_mva(carga, patamar->Index, sf_mva))
				{
					// return(false); // Desabilitado em 2018.08.20, HPS/JCG
					continue;
				}
				// verifica se h� pot.ativa negativa
				for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
				{
					p_mw = sf_mva[ind_fase].real();
					if (p_mw < 0)
					{ // valor negativo: inaceit�vel
						return (false);
					}
				}
			}
		}
	}
	else
	{ // trata somente o patamar selecionado
		if ((patamar = patamares->ExistePatamar(HoraReferencia, 0)) == NULL)
			return (false);
		// zera demandas  pequenas
		for (int n = 0; n < lisEQP->Count; n++)
		{
			carga = (VTCarga*)lisEQP->Items[n];
			atualiza = false;
			// determina demanda por fase da Carga no Patamar
			if (!demanda->Get_S3f_mva(carga, patamar->Index, sf_mva))
			{
				// return(false); // Desabilitado em 2018.08.20, HPS/JCG
				continue;
			}
			// verifica se h� pot.ativa negativa apenas
			for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
			{
				p_mw = sf_mva[ind_fase].real();
				if ((p_mw < 0) && (p_mw > -30.0e-3))
				{ // zera demanda na fase
					sf_mva[ind_fase] = CZero();
					atualiza = true;
					// pot_descartada_mw += p_mw;
				}
			}
			// se demanda alterada, atualiza carga
			if (atualiza)
				demanda->Set_S3f_mva(carga, patamar->Index, sf_mva);
		}
		// verifica se restaram Cargas com pot.ativa negativa
		for (int n = 0; n < lisEQP->Count; n++)
		{
			carga = (VTCarga*)lisEQP->Items[n];
			// determina demanda por fase da Carga no Patamar
			if (!demanda->Get_S3f_mva(carga, patamar->Index, sf_mva))
			{
				// return(false); // Desabilitado em 2018.08.20, HPS/JCG
				continue;
			}
			// verifica se h� pot.ativa negativa
			for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
			{
				p_mw = sf_mva[ind_fase].real();
				if (p_mw < 0)
				{ // valor negativo: inaceit�vel
					return (false);
				}
			}
		}
	}
	return (true);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TEstimaDemanda::ValidaDemandaCargasClonadas(VTRedes *redes_clone)
 {
 return(true);
 }
 */

/*
 //---------------------------------------------------------------------------
 bool __fastcall TEstimaDemanda::TESTE_GravaArquivoCarga(AnsiString filename)
 {
 //vari�veis locais
 TExportaCarga *exporta;

 // Modifica��o HPS para que esta fun��o possa funcionar tamb�m em TEstimaDemanda,
 // que n�o conhece CBoxPatamar.
 // VTPatamar     *patamar = (VTPatamar*)CBoxPatamar->Items->Objects[CBoxPatamar->ItemIndex];
 int index = 9;  // Patamar das 9:00 �s 10:00

 // executa somente em  modo _DEBUG
 // #ifdef _DEBUG

 //cria objeto TExportaCarga
 if ((exporta = new TExportaCarga(apl)) != NULL)
 {
 // Modifica��o HPS
 // exporta->Executa(filename, patamar->Index);

 exporta->Executa(filename, index);

 delete exporta;
 }
 return(true);

 // #endif
 }
 */

// ---------------------------------------------------------------------------
// eof
