// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
// #include <complexo.h>
#include <Math.hpp>
#include <DateUtils.hpp>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjo.h>
#include <Fontes\Arranjo\VTArranjos.h>
#include <Fontes\Arranjo\VTCabo.h>
#include <Fontes\Arranjo\VTSuporte.h>
#include <Fontes\Distancia\VTCaminho.h>
#include <Fontes\Cartografia\VTScg.h>
#include <Fontes\Classes\VTClasse.h>
#include <Fontes\Classes\VTClasses.h>
#include <Fontes\Complexo\Complexo.h>
#include <Fontes\Constante\Fases.h>
#include <Fontes\Importa\Monta\VTMonta.h>
#include <Fontes\ImportaRedeGis\VTImportaRedeGis.h>
#include <Fontes\Licenca\VTLicenca.h>
#include <Fontes\Licenca\VTLicencas.h>
#include <Fontes\Log\VTLog.h>
#include <Fontes\Patamar\VTPatamar.h>
#include <Fontes\Patamar\VTPatamares.h>
#include <Fontes\Potencia\VTDemanda.h>
#include <Fontes\Radial\VTRadial.h>
#include <Fontes\Radial\VTPrimario.h>
#include <Fontes\Radial\VTSubestacao.h>
#include <Fontes\Rede\Estrutura.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTChave.h>
#include <Fontes\Rede\VTDominio.h>
#include <Fontes\Rede\VTFases.h>
#include <Fontes\Rede\VTGerador.h>
#include <Fontes\Rede\VTLigacao.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTRegulador.h>
#include <Fontes\Rede\VTSuprimento.h>
#include <Fontes\Rede\VTTipoChave.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <Fontes\Rede\VTTipos.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Rede\VTTrafo3E.h>
#include <Fontes\Rede\VTTrecho.h>

#include <DLL_Inc\Cartografia.h>
#include <DLL_Inc\Distancia.h>
#include <DLL_Inc\Funcao.h>

#include <DLL_Inc\Radial.h>
#include <DLL_Inc\Rede.h>

#include "TPreMonta.h"
#include <..\Commons\VTCommons.h>
// Project include - <Commons\Dominio>
#include <Dominio\TAreaLocalizacao.h>
#include <Dominio\TBitolaCabo.h>
#include <Dominio\TCapElo.h>
#include <Dominio\TClasseSubclasse.h>
#include <Dominio\TClasseTensao.h>
#include <Dominio\TConfCircuito.h>
#include <Dominio\TCorrenteNom.h>
#include <Dominio\TFasesConexao.h>
#include <Dominio\TFormacaoCabo.h>
#include <Dominio\TGeometriaCabo.h>
#include <Dominio\TGrupoTarifario.h>
#include <Dominio\TIsolacaoCabo.h>
#include <Dominio\TLigacaoTrafo.h>
#include <Dominio\TMaterialCabo.h>
#include <Dominio\TPosse.h>
#include <Dominio\TTensao.h>
#include <Dominio\TTipoPotAparente.h>
#include <Dominio\TTipoPotReativa.h>
#include <Dominio\TTipoRegulador.h>
#include <Dominio\TTipoTrafo.h>
#include <Dominio\TTipoUnidade.h>
//
#include <..\Commons\Entidades\TArranjoBDGD.h>
#include <..\Commons\Entidades\TBarraBDGD.h>
#include <..\Commons\Entidades\TBarramento.h>
#include <..\Commons\Entidades\TRedeMT.h>
#include <..\Commons\Entidades\TSubestacao.h>
#include <..\Commons\Entidades\TTrafoSubestacao.h>
#include <Entidades\TPri.h>
#include <Entidades\TSe.h>
#include <Entidades\TSec.h>

#include <TLogThread.h>
#include <TLogManager.h>

// ---------------------------------------------------------------------------
__fastcall TPreMonta::TPreMonta(VTApl *apl)
{
	// variaveis locais
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTLicenca  *licenca  = licencas->ExisteLicenca();
   // salva ponteiros p/ objetos
	this->apl = apl;
	arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
	fases = (VTFases*)apl->GetObject(__classid(VTFases));
	plog = (VTLog*)apl->GetObject(__classid(VTLog));
	monta = (VTMonta*)apl->GetObject(__classid(VTMonta));
	patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	scg = DLL_NewObjScg(apl, licenca->Empresa, licenca->EmpresaStr);
	// cria listas
	lisEQP = new TList();
	lisSE = new TList();

	// inicia horários dos patamares da EDP
	for (int ind_pat = 0; ind_pat < pat01HORA; ind_pat++)
	{
		pat_edp[ind_pat].ponta = (ind_pat >= INIPONTA) && (ind_pat <= FIMPONTA);
		pat_edp[ind_pat].hm_ini.hora = 0;
		pat_edp[ind_pat].hm_ini.minuto = 0;
		pat_edp[ind_pat].hm_fim.hora = ind_pat + 1;
		pat_edp[ind_pat].hm_fim.minuto = 0;
	}
	// inicia data para Domínio
	DataDominio = Now();
	// cria arranjos default
	CriaArranjoTipicoMT();
	CriaArranjoTipicoBT();

	function = __FUNC__;
	log_manager = (TLogManager*)apl->GetObject(__classid(TLogManager));
	// if(log_manager == NULL){
	// apl->Add(log_manager = new TLogManager(apl));
	// }
	if (log_manager)
	{
		log_thread = log_manager->NewLogThread(function);
	}
}

// ---------------------------------------------------------------------------
__fastcall TPreMonta::~TPreMonta(void)
{
	// destrói lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	// destrói listas e seus objetos
	if (lisSE)
	{
		LimpaTList(lisSE);
		delete lisSE;
		lisSE = NULL;
	}
	if (scg)    {delete scg;    scg    = NULL;}
}

// ---------------------------------------------------------------------------
VTArranjo* __fastcall TPreMonta::ArranjoBTcerto(int fases_tag)
{
	// variáveis locais
	AnsiString codigo;
	VTArranjo *arranjo;

	// o Arranjo BT tem fases diferentes da indicada:
	// verifica se existe um Arranjo cujo código é igual ao do ArranjoBT acrescido com string das fases
	codigo = arranjoBT->Codigo + "_" + fases->AsStrABCN[fases_tag];
	if ((arranjo = arranjos->ExisteArranjo(codigo)) == NULL)
	{ // cria uma cópia do Arranjo original e redefine seu código e fases
		if ((arranjo = arranjoBT->Clone()) != NULL)
		{
			arranjo->Id = FalsoId();
			arranjo->Codigo = codigo;
			arranjo->Fases = fases_tag;
			arranjo->Status[sttNOVO] = true;
			arranjo->ramal = false;
			// insere o novo Arranjo em Arranjos
			arranjos->InsereArranjo(arranjo);
		}
	}

	return (arranjo);
}

// ---------------------------------------------------------------------------
VTArranjo* __fastcall TPreMonta::ArranjoMTcerto(int fases_tag)
{
	// variáveis locais
	AnsiString codigo;
	VTArranjo *arranjo;

	// o Arranjo MT tem fases diferentes da indicada:
	// verifica se existe um Arranjo cujo código é igual ao do ArranjoMT acrescido com string das fases
	codigo = arranjoMT->Codigo + "_" + fases->AsStrABCN[fases_tag];
	if ((arranjo = arranjos->ExisteArranjo(codigo)) == NULL)
	{ // cria uma cópia do Arranjo original e redefine seu código e fases
		if ((arranjo = arranjoMT->Clone()) != NULL)
		{
			arranjo->Id = FalsoId();
			arranjo->Codigo = codigo;
			arranjo->Fases = fases_tag;
			arranjo->Status[sttNOVO] = true;
			arranjo->ramal = false;
			// insere o novo Arranjo em Arranjos
			arranjos->InsereArranjo(arranjo);
		}
	}

	return (arranjo);
}

/*
// -----------------------------------------------------------------------------
void __fastcall TPreMonta::AtualizaCargaEquivalente(TEt *et, VTCarga *carga)
{
	// variáveis locais
	complex<double>s_mva;
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

	try
	{ // incorpora demanda da Carga na demanda da Carga equivalente
		for (int np = 0; np < patamares->NumPatamar(); np++)
		{ // determina demanda da Carga equivalente da Et no patamar
			s_mva = demanda->S_mva[et->carga][np];
			// soma demanda da Carga no patamar
			s_mva += demanda->S_mva[carga][np];
			// atualiza demanda da Carga equivalente da Et no patamar
			demanda->S_mva[et->carga][np] = s_mva;
		}
		// incrementa quantidade de consumidores da classe (DVK 2013.08.21)
		switch (carga->classe->Tag)
		{
		case consRESIDENCIAL:
			et->carga->NumConsResidencial++;
			break;
		case consCOMERCIAL:
			et->carga->NumConsComercial++;
			break;
		case consINDUSTRIAL:
			et->carga->NumConsIndustrial++;
			break;
		case consRURAL:
			et->carga->NumConsRural++;
			break;
		case consOUTROS:
			et->carga->NumConsOutros++;
			break;
		case consA4:
			et->carga->NumConsA4++;
			break;
		case consIP: // DVK 2014.10.06 não conta consIP
			break;
		default:
			et->carga->NumConsOutros++;
		}
		// altera a definição de Classe da Carga equivalente   //DVK 2014.05.27
		if (et->carga->NumConsTotal == 1)
		{ // assume mesma classe da Carga parcial
			et->carga->DefineClasse(carga->classe);
		}
		else
		{ // assume Equivalente
			et->carga->DefineClasse(classes->ExisteClasseTag(consEQUIVALENTE));
		}
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro ao atualizar carga equivalente");
	}
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TPreMonta::BarraSolta(VTChave *chave)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	VTBarra *barra1, *barra2;
	TList *lisAuxiliar;
	VTBarra *barraEscolhida = NULL;

	try
	{ // proteção
		if (chave == NULL)
		{
			return (NULL);
		}
		if ((rede = chave->rede) == NULL)
		{
			return (NULL);
		}
		barra1 = chave->pbarra1;
		barra2 = chave->pbarra2;
		if ((barra1 == NULL) || (barra2 == NULL))
		{
			return (NULL);
		}
		// verifica cada barra da chave
		lisAuxiliar = new TList;
		redes->LisLigacao(lisAuxiliar, barra1);
		if (lisAuxiliar->Count == 1)
		{
			barraEscolhida = barra1;
		}
		else
		{
			lisAuxiliar->Clear();
			redes->LisLigacao(lisAuxiliar, barra2);
			if (lisAuxiliar->Count == 1)
			{
				barraEscolhida = barra2;
			}
		}
		DestroiTObject(lisAuxiliar);
		return barraEscolhida;
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro ao localizar barra de chave de socorro");
		DestroiTObject(lisAuxiliar);
		barraEscolhida = NULL;
	}
	return barraEscolhida;
}


// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::ConectaSocorro(void)
{
	TSe *se1, *se2;
	TCircuito *circ1, *circ2;
	VTBarra *barra1, *barra2;
	VTChave *chave, *chaveAux;
	VTRede *rede1, *rede2;
	TList *lisAuxiliar;
	bool sucesso = false;
	TSocorro *socorro;
	VTBarra *barraRef, *barraProx, *outraBarra;
	VTRede *redeChave, *redeConexao;
	double multiplicador;
	TList *lisBarrasIndesej;

	__try
	{
		try
		{
			lisAuxiliar = new TList;
			lisBarrasIndesej = new TList;
			// percorre a lista de socorros
			for (int ns = 0; ns < lisSocorro->Count; ns++)
			{
				se1 = se2 = NULL;
				circ1 = circ2 = NULL;
				barra1 = barra2 = NULL;
				chave = chaveAux = NULL;
				rede1 = rede2 = NULL;
				barraRef = barraProx = NULL;
				redeChave = redeConexao = NULL;
				socorro = (TSocorro*) lisSocorro->Items[ns];
				// se socorro inativo
				if (socorro->ativo == false)
					continue;
				// acha as SEs
				se1 = ExisteSubestacao(socorro->se1_id);
				se2 = ExisteSubestacao(socorro->se2_id);
				// caso nao existam
				if ((se1 == NULL) || (se2 == NULL))
					continue;
				// verifica os circuitos envolvidos
				circ1 = se1->ExisteCircuito(socorro->circuito1_id);
				circ2 = se2->ExisteCircuito(socorro->circuito2_id);
				// caso existam
				if ((circ1 == NULL) || (circ2 == NULL))
					continue;
				rede1 = circ1->rede;
				rede2 = circ2->rede;
				lisAuxiliar->Clear();
				// encontra a chave
				rede1->LisLigacao(lisAuxiliar, eqptoCHAVE);
				// percorre a lista
				for (int nc = 0; nc < lisAuxiliar->Count; nc++)
				{
					chaveAux = (VTChave*)lisAuxiliar->Items[nc];
					if (chaveAux->Id == socorro->chave_id)
					{
						chave = chaveAux;
						break;
					}
				}
				// se nao achar na primeira, procura na 2a
				if (chave == NULL)
				{
					lisAuxiliar->Clear();
					// encontra a chave
					rede2->LisLigacao(lisAuxiliar, eqptoCHAVE);
					// percorre a lista
					for (int nc = 0; nc < lisAuxiliar->Count; nc++)
					{
						chaveAux = (VTChave*)lisAuxiliar->Items[nc];
						if (chaveAux->Id == socorro->chave_id)
						{
							chave = chaveAux;
							break;
						}
					}
				}
				// se nao encontrar...
				if (chave == NULL)
					continue;
				lisBarrasIndesej->Clear();
				// verifica qual das barras da chave está solta
				barraRef = BarraSolta(chave);
				// se nao encontrar...
				if (barraRef == NULL)
					continue;
				if (barraRef == chave->pbarra1)
					outraBarra = chave->pbarra2;
				else
					outraBarra = chave->pbarra1;
				lisBarrasIndesej->Add(barraRef);
				lisBarrasIndesej->Add(outraBarra);
				// verifica qual eh a rede da conexao
				redeChave = chave->rede;
				if (rede1 == redeChave)
					redeConexao = rede2;
				else
					redeConexao = rede1;
				// encontra a barra proxima
				// 0.0056506043007377
				for (int retry = 0; retry < 3; retry++)
				{
					barraProx = NULL;
					// por quantas vezes o range sera multiplicado
					multiplicador = pow((double)10, retry);
					barraProx = ExisteBarra(barraRef, lisBarrasIndesej, redeConexao, redeChave,
						multiplicador * 1);
					if (barraProx != NULL)
						break;
				}
				// se nao encontrar...
				if (barraProx == NULL)
					continue;
				chave->DefineObjBarra(outraBarra, barraProx);
			}
		}
		catch (Exception &e)
		{
			if (plog)
				plog->Add(ERRO_DE_SISTEMA);
			sucesso = false;
		}
	}
	__finally
	{
		DestroiTObject(lisAuxiliar);
		DestroiTObject(lisBarrasIndesej);
	}
	return (sucesso);
}
 */

// ---------------------------------------------------------------------------
int __fastcall TPreMonta::ConverteClasseConsumidor(AnsiString sigla_classe)
{
	// retorna código da classe
	if(sigla_classe.AnsiPos("RE") >0)
	{ return (consRESIDENCIAL);}
	else if(sigla_classe.AnsiPos("IN") >0)
	{ return (consINDUSTRIAL);}
	else if(sigla_classe.AnsiPos("CO") >0)
	{ return (consCOMERCIAL);}
	else if(sigla_classe.AnsiPos("RU") >0)
	{	return (consRURAL);}
//	else if(sigla_classe.AnsiPos("IP") >0)
//	{	return (consIP);}
	else
	{
		return (consOUTROS);
	}
}

// ---------------------------------------------------------------------------------------------------
int __fastcall TPreMonta::ConverteTipoLigacao(AnsiString codigo_lig)
{
     // verifica o tipo de ligação string
	codigo_lig = codigo_lig.UpperCase();
	if (codigo_lig.AnsiPos("M") > 0)
		return (lgMONOFASICO);
	if (codigo_lig.AnsiPos("T") > 0)
		return (lgEST_AT);
	if (codigo_lig.AnsiPos("DA") > 0)
		return (lgDELTA_ABERTO);
	if (codigo_lig.AnsiPos("DF") > 0)
		return (lgDELTA_FECHADO);
	// default
	return (lgINDEFINIDA);
}
// ---------------------------------------------------------------------------------------------------
bool __fastcall TPreMonta::CriaArranjoTipicoBT(void)
{
	// variáveis locais
	strARRANJO2 str;
	// verifica se já existe o Arranjo BT default
	if (arranjoBT != NULL)
	{
		// #ifdef _DEBUG
		function = __FUNC__;
		msglog = "Já existe Arranjo BT default em " + AnsiString(function);
		if (log_thread)
			log_thread->AddLog(msglog, function);
		// #endif
		return (true);
	}
	try
	{ // inicia atributos comuns
		str.tipico = true;
		str.data = DateOf(Now());
		str.arranjo_id = FalsoId();
		str.tipo_cfg = arranjoZ0Z1;
		str.fases = faseABCN;
		str.ramal = false;
		// define código do Arranjo compondo código dos cabos de fase e neutro
		str.codigo = "Arranjo_BT";
		str.extern_id = "";
		str.status = sttNOVO;
		// define atributo específicos do arranjo Z0Z1 (baseado no 4 CAA)
		str.cfg_arranjoZ0Z1.iadm_amp = 153;
		str.cfg_arranjoZ0Z1.z0.r = 1.8046;
		str.cfg_arranjoZ0Z1.z0.x = 0.5495;
		str.cfg_arranjoZ0Z1.z1.r = 1.8046;
		str.cfg_arranjoZ0Z1.z1.x = 0.5495;
		// converte capacitância para nF/km
		str.cfg_arranjoZ0Z1.c0 = 0.;
		str.cfg_arranjoZ0Z1.c1 = 0.;
		str.cfg_arranjoZ0Z1.suporte_id = ID_NULO;
		// insere Arranjo
		arranjoBT = monta->InsereArranjo(str);
		// #ifdef _DEBUG
		if (arranjoBT)
		{
			// function = __FUNC__;
			// msglog = "Inseriu arranjo com sucesso em " + AnsiString(function);
			// if(log_thread)log_thread->AddLog(msglog,function);
		}
		else
		{
			function = __FUNC__;
			msglog = "Inserção falhou em " + AnsiString(function);
			if (log_thread)
				log_thread->AddLog(msglog, function, true);
		}
		// #endif
	}
	catch (Exception &e)
	{
		// #ifdef _DEBUG
		function = __FUNC__;
		msglog = "Exception caught em ";
		msglog += AnsiString(function);
		msglog += " " + e.Message;
		if (log_thread)
			log_thread->AddLog(msglog, function, true);
		// #endif
		return (false);
	}
	return (arranjoBT != NULL);
}

// ---------------------------------------------------------------------------------------------------
bool __fastcall TPreMonta::CriaArranjoTipicoMT(void)
{
	// variáveis locais
	strARRANJO2 str;
	// verifica se já existe o Arranjo MT default
	if (arranjoMT != NULL)
	{
		// #ifdef _DEBUG
		function = __FUNC__;
		msglog = "Já existe Arranjo MT default em " + AnsiString(function);
		if (log_thread)
			log_thread->AddLog(msglog, function);
		// #endif
		return (true);
	}
	try
	{ // inicia atributos comuns
		str.tipico = true;
		str.data = DateOf(Now());
		str.arranjo_id = FalsoId();
		str.tipo_cfg = arranjoZ0Z1;
		str.fases = faseABCN;
		str.ramal = false;
		// define código do Arranjo compondo código dos cabos de fase e neutro
		str.codigo = "Arranjo_MT";
		str.extern_id = "";
		str.status = sttNOVO;
		// define atributo específicos do arranjo Z0Z1 (baseado no 336 CAA)
		str.cfg_arranjoZ0Z1.iadm_amp = 591;
		str.cfg_arranjoZ0Z1.z0.r = 0.2240;
		str.cfg_arranjoZ0Z1.z0.x = 0.4165;
		str.cfg_arranjoZ0Z1.z1.r = 0.2240;
		str.cfg_arranjoZ0Z1.z1.x = 0.4165;
		// converte capacitância para nF/km
		str.cfg_arranjoZ0Z1.c0 = 0.;
		str.cfg_arranjoZ0Z1.c1 = 0.;
		str.cfg_arranjoZ0Z1.suporte_id = ID_NULO;
		// insere Arranjo
		arranjoMT = monta->InsereArranjo(str);
		// #ifdef _DEBUG
		if (arranjoMT)
		{
			// function = __FUNC__;
			// msglog = "Inseriu arranjo com sucesso em " + AnsiString(function);
			// if(log_thread)log_thread->AddLog(msglog,function);
		}
		else
		{
			function = __FUNC__;
			msglog = "Inserção falhou em " + AnsiString(function);
			if (log_thread)
				log_thread->AddLog(msglog, function, true);
		}
		// #endif
	}
	catch (Exception &e)
	{
		// #ifdef _DEBUG
		function = __FUNC__;
		msglog = "Exception caught em ";
		msglog += AnsiString(function);
		msglog += " " + e.Message;
		if (log_thread)
			log_thread->AddLog(msglog, function, true);
		// #endif
		return (false);
	}
	return (arranjoMT != NULL);
}

// -------------------------------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta::CriaConsMTdemanda(strCARGA &str, TCargaBDGD *cargaMT)
{
	// Variáveis locais
	VTCarga *carga;
	int hora_ini_ponta = 18;
	int hora_fim_ponta = 21;
	double demanda_ativa_fora_ponta, demanda_ativa_ponta;
	double demanda_reativa_fora_ponta, demanda_reativa_ponta;

	demanda_ativa_fora_ponta = (cargaMT->Dem_Max_kW[1] * 0.9) * 0.001;
	demanda_reativa_fora_ponta = 0.0 * 0.001;
	demanda_ativa_ponta = cargaMT->Dem_Max_kW[1] * 0.001;
	demanda_reativa_ponta = 0.0 * 0.001;

	// cria curva própria
	str.curva.id = -1;
	str.curva.tipica = false;
	str.curva.num_pat = pat01HORA;
	str.curva.num_valor = nvCURVA_PQ;
	for (int np = 0; np < str.curva.num_pat; np++)
	{ // define hora inicial e final do patamar
		str.curva.patamar[np].hm_ini.hora = np;
		str.curva.patamar[np].hm_ini.minuto = 0;
		str.curva.patamar[np].hm_fim.hora = np + 1;
		str.curva.patamar[np].hm_fim.minuto = 0;
		if ((np < hora_ini_ponta) || (np > hora_fim_ponta))
		{ // fora ponta
			str.curva.patamar[np].valor[0] = demanda_ativa_fora_ponta;
			// kVA =>MVA
			str.curva.patamar[np].valor[1] = demanda_reativa_fora_ponta;
			// kVA =>MVA
		}
		// ponta
		else
		{
			str.curva.patamar[np].valor[0] = demanda_ativa_ponta;
			// kVA =>MVA
			str.curva.patamar[np].valor[1] = demanda_reativa_ponta;
			// kVA =>MVA
		}
	}
	// insere Carga em VTMonta
	carga = (VTCarga*)(monta->InsereCarga(str));
	return (carga);
}

// -------------------------------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta::CriaConsMTenergia(strCARGA &str)
{
	// Variáveis locais
	VTCarga *carga;

	// cancela vetor de demanda p/ curva própria
	str.curva.id = -1;
	str.curva.tipica = true;
	str.curva.num_pat = 0;
	str.curva.num_valor = 2;
	// insere Carga em VTMonta
	carga = (VTCarga*)(monta->InsereCarga(str));
	return (carga);
}

// ---------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta::CriaCargaConsumidorBT(TCargaBDGD *cargaBT, int barra_id)
{
	// variáveis locais
	double kwhmes_max, consumo_kwh_mes;
	int fases_car;
	strCARGA str;
	VTCarga *carga;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Cons.BT");
		if (plog)
			plog->DefineValor("Código", cargaBT->Codigo);
		// Determina as fases da Carga
		if (cargaBT->Fases_Con != NULL)
		{
			fases_car = fases->ExisteFase(cargaBT->Fases_Con->Codigo);
		}
		// garante consumidor com neutro
		fases_car |= faseN;
		// preenche estrutura strCARGA
		str.car_id = cargaBT->Id;
		str.bar_id = barra_id;
		str.codigo = cargaBT->Codigo;
		str.fases = fases_car;
		// Verifica energia de consumo no mês
		consumo_kwh_mes = cargaBT->Ene_kWh[1];
		// valida valor zerado
		if (IsDoubleZero(consumo_kwh_mes))
		{
			if (plog)
				plog->Add("Consumo zerado");
		}
		else
		{
			// verifica consumo muito grande
			switch (fases->NumeroDeFases(fases_car))
			{
			case 1:
				kwhmes_max = 1000.;
				break;
			case 2:
				kwhmes_max = 5000.;
				break;
			case 3:
				kwhmes_max = 15000.;
				break;
			default:
				kwhmes_max = 1000.;
				break;
			}
		}
		if (consumo_kwh_mes > kwhmes_max)
		{
			if (plog)
				plog->Add("Consumo de " + DoubleToStr("%2.1f kWh/mês", consumo_kwh_mes));
			// reduz valor de consumo
			consumo_kwh_mes = kwhmes_max;
		}
		// define classe de consumidor/energia
		str.classe_cons = ConverteClasseConsumidor(cargaBT->Classe_Subclasse->Codigo);
		str.grupoA = false;
		// Verificação
		if(cargaBT->Resultado_Inspecao <= 0)
		{
            cargaBT->Resultado_Inspecao = -1;
		}
		str.categoria = cargaBT->Resultado_Inspecao;
		// define modelo de carga Icte
		str.modelo.icte_pu = 1;
		str.modelo.scte_pu = 0;
		str.modelo.zcte_pu = 0;
		str.energia_mwhmes = consumo_kwh_mes * 0.001;
		// cancela vetor de demanda p/ curva própria
		str.curva.id = -1;
		str.curva.tipica = true;
		str.curva.num_pat = 0;
		str.curva.num_valor = nvCURVA_PQ;   //2;
		// cancela dados de IP
		str.ip.num_lamp = 0;
		str.ip.pot_kw = 0.;
		str.ip.fat_pot = 1.;
		// esquemático
		str.esq.posbar = 50.;
		str.esq.dxbar = DXBAR;
		str.esq.dybar = -DYBAR;
		// insere Carga em VTMonta
		carga = (VTCarga*)(monta->InsereCarga(str));
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		carga = NULL;
	}
	return (carga);
}

// -----------------------------------------------------------------------------
VTCarga* __fastcall TPreMonta::CriaCargaConsumidorIP(TCargaIP *cargaIP, int barra_id)
{
	// variáveis locais
	int fases_car;
	double p_kw, q_kvar, s_kva;
	strCARGA str;
	VTCarga *carga;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Código", cargaIP->Codigo);
		// preenche estrutura strCARGA
		str.car_id = cargaIP->Id;
		str.bar_id = barra_id;
		str.codigo = cargaIP->Codigo;
		str.classe_cons = consIP;
		str.grupoA = false;
		// Determina as fases da Carga
		fases_car = fases->ExisteFase(cargaIP->Fases_Con->Codigo);
		// verifca se há apenas uma fase
		if ((fases_car == faseA) || (fases_car == faseB) || (fases_car == faseC))
		{ // inclui neutro
			fases_car = fases_car + faseN;
		}
		str.fases = fases_car;
		// define modelo Scte
		str.modelo.icte_pu = 0.;
		str.modelo.scte_pu = 1.;
		str.modelo.zcte_pu = 0.;
		// cancela energia
		str.energia_mwhmes = 0.;
		// define curva típica
		str.curva.id = -1;
		str.curva.tipica = true;
		str.curva.num_pat = 0;
		str.curva.num_valor = nvCURVA_PFP;
		// define dados de IP
		str.ip.num_lamp = 1;
		// determina potência ativa
		str.ip.pot_kw = cargaIP->Carga_Instal_kW;
		// determina fator de potência
		str.ip.fat_pot = IP_FATPOT;
		// esquemático
		str.esq.posbar = 50.;
		str.esq.dxbar = DXBAR;
		str.esq.dybar = -DYBAR;
		// insere Carga em VTMonta
		carga = (VTCarga*)monta->InsereCarga(str);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		carga = NULL;
	}
	return (carga);
}


// ---------------------------------------------------------------------------
TColor __fastcall TPreMonta::DefineCorRedePrimaria(TRedeMT *primario)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	TSubestacao *subestacao;
	int vet_size, index;
	TColor vet_color[] =
	{clBlue, clRed, clGreen, clYellow, clSkyBlue, clFuchsia, clOlive, clPurple, clNavy, clMaroon,
		clTeal, clLime};

	// proteção
	if ((se == NULL) || (primario == NULL))
		return (clBlue);
	//
	subestacao = commons->GetSubestacao(se->id);
	// determina dimensão do vetor de cores
	vet_size = sizeof(vet_color) / sizeof(vet_color[0]);
	index = subestacao->LisPrimario()->IndexOf(primario);
	//index = se->lisPRI->IndexOf(pri);
	while (index >= vet_size)
		index -= vet_size;

	return (vet_color[index]);
}

// ---------------------------------------------------------------------------
VTArranjo* __fastcall TPreMonta::ExisteArranjo(int arranjo_id, int fases_tag, bool ramal)
{
	// variáveis locais
	AnsiString codigo;
	VTArranjo *arranjo_original, *arranjo;

	// proteção
	if (arranjo_id == 0)
		return (NULL);
	// corrige id do arranjo
	arranjo_id += ARRANJO_ID;
	// procura Arranjo original pelo seu ID
	if ((arranjo_original = arranjos->ExisteArranjo(arranjo_id)) == NULL)
		return (NULL);
	// verifica se o Arranjo tem as fases indicadas
	if (arranjo_original->Fases == fases_tag)
		arranjo = arranjo_original;
	else
	{ // o Arranjo original da EDP tem fases diferentes da indicada:
		// verifica se existe um Arranjo cujo código é igual ao do Arranjo da EDP acrescido com string das fases
		codigo = arranjo_original->Codigo + "_" + fases->AsStrABCN[fases_tag];
		if ((arranjo = arranjos->ExisteArranjo(codigo)) == NULL)
		{ // cria uma cópia do Arranjo original e redefine seu código e fases
			if ((arranjo = arranjo_original->Clone()) != NULL)
			{
				arranjo->Id = FalsoId();
				arranjo->Codigo = codigo;
				arranjo->Fases = fases_tag;
				arranjo->Status[sttNOVO] = true;
				// insere o novo Arranjo em Arranjos
				arranjos->InsereArranjo(arranjo);
			}
		}
	}
	// define Arranjo como ramal ou não
	arranjo->ramal = ramal;

	return (arranjo);
}

//// ---------------------------------------------------------------------------
//VTBarra* __fastcall TPreMonta::ExisteBarra(VTBarra *barra_ref, TList *lisBarrasIndesejaveis,
//	VTRede *rede1, VTRede *rede2, double raio_m)
//{
//	// variáveis locais
//	double menor_dist, dist;
//	int x, y;
//	int coord_x, coord_y;
//	int delta_x = 0;
//	int delta_y = 0;
//	VTBarra *barra, *barraIndesej;
//	VTBarra *barra_escolhida = NULL;
//	TList *lisBarra;
//	TList *lisCandidatas;
//	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
//
//	// proteção
//	if (barra_ref == NULL)
//		return NULL;
//	// inicia x e y
//	x = barra_ref->utm.x;
//	y = barra_ref->utm.y;
//	// cria listas
//	lisBarra = new TList;
//	lisCandidatas = new TList;
//	// verifica se foi indicada a Rede para busca da Barra
//	if (rede1 != NULL)
//		rede1->LisBarra(lisBarra);
//	else
//		lisBarra = redes->LisBarra();
//	if (rede2 != NULL)
//		rede2->LisBarra(lisBarra);
//	if (IsDoubleZero(raio_m, 0.01))
//	{
//		delta_x = 100;
//		delta_y = 100;
//	}
//	else
//	{
//		delta_x = raio_m * 100;
//		delta_y = raio_m * 100;
//	}
//
//	coord_x = x;
//	coord_y = y;
//	// determina a lista de candidatas
//	for (int nb = 0; nb < lisBarra->Count; nb++)
//	{
//		barra = (VTBarra*) lisBarra->Items[nb];
//		// verifica se esta dentro do quadrado
//		if ((barra->utm.x > (coord_x - delta_x)) && (barra->utm.x < (coord_x + delta_x)))
//		{
//			if ((barra->utm.y > (coord_y - delta_y)) && (barra->utm.y < (coord_y + delta_y)))
//			{
//				if (barra_ref != barra)
//				{
//					if (lisBarrasIndesejaveis != NULL)
//					{
//						if (lisBarrasIndesejaveis->IndexOf(barra) < 0)
//						{
//							lisCandidatas->Add(barra);
//						}
//					}
//					else
//					{
//						lisCandidatas->Add(barra);
//					}
//				}
//			}
//		}
//	}
//	// verifica menor distancia
//	for (int nb = 0; nb < lisCandidatas->Count; nb++)
//	{
//		barra = (VTBarra*) lisCandidatas->Items[nb];
//		if (nb == 0)
//		{
//			menor_dist =
//				sqrt(fabs((long double)coord_x - barra->utm.x) * fabs((long double)coord_x -
//				barra->utm.x) + fabs((long double)coord_y - barra->utm.y) * fabs
//				((long double)coord_y - barra->utm.y));
//			barra_escolhida = barra;
//			continue;
//		}
//		dist = sqrt(fabs((long double)coord_x - barra->utm.x) * fabs((long double)coord_x -
//			barra->utm.x) + fabs((long double)coord_y - barra->utm.y) * fabs((long double)coord_y -
//			barra->utm.y));
//		if (dist < menor_dist)
//		{
//			barra_escolhida = barra;
//			menor_dist = dist;
//		}
//	}
//	// delete
//	DestroiTObject(lisCandidatas);
//	DestroiTObject(lisBarra);
//
//	return (barra_escolhida);
//}


// -------------------------------------------------------------------------------------------------
int __fastcall TPreMonta::ExisteTipoChaveSinap(int id_chave)
{
	// variaveis locais
	VTTipoChave *tipo_chave = tipos->ExisteTipoChave("Seccionalizadora");
	int tipo_seccionalizadora = chaveSECCIONADORA; // default, caso nao exista no banco
	int vet_size;

	struct strTIPOCHVSNP
	{
		int tipo_sinap;
		int tipo_BDGD;
	};

	if (tipo_chave != NULL)
		tipo_seccionalizadora = tipo_chave->Tipo;
	strTIPOCHVSNP vet_tipo[] =
	{  {chaveFACA, 19}, {chaveFACA, 20}, {chaveFACA, 21},
	   {chaveBF, 22}, {chaveBF, 23}, {chaveBF, 24}, {chaveBF, 25}, {chaveBF, 26}, {chaveBF, 27},
	   {chaveDISJUNTOR, 29}, {chaveDISJUNTOR, 30}, {chaveRELIGADORA, 32},
		{chaveSECCIONADORA, 33}, {chaveSECCIONADORA, 34},
		{tipo_seccionalizadora, 36}, {tipo_seccionalizadora, 36}};
	// determina dimensão do vetor de tipos
	vet_size = (sizeof(vet_tipo) / sizeof(*vet_tipo));
	// localiza tipo de chave sinap, considerando o tipo de chave no gis
	for (int ind_tipo = 0; ind_tipo < vet_size; ind_tipo++)
	{
		if (id_chave == vet_tipo[ind_tipo].tipo_BDGD)
			return (vet_tipo[ind_tipo].tipo_sinap);
	}
	return (chaveINDEFINIDA);
}

// ---------------------------------------------------------------------------
TSe* __fastcall TPreMonta::ExisteSubestacao(int se_id)
{
	// variáveis locais
	TSe *se;

	// procura TSe
	for (int n = 0; n < lisSE->Count; n++)
	{
		se = (TSe*)lisSE->Items[n];
		if (se->id == se_id)
			return (se);
	}
	return (NULL);
}




// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::IniciaDominio(void)
{
	// variáveis locais
	AnsiString txt;
	strDOMINIO str_dominio;

	try
	{ // insere um Dominio em VTMonta
		txt.sprintf("Rede_%04d.%02d.%02d", YearOf(DataDominio), MonthOf(DataDominio),
			DayOf(DataDominio));
		str_dominio.obra = false;
		str_dominio.dominio_id = FalsoId();
		str_dominio.codigo = txt;
		str_dominio.data_criacao = DataDominio;
		dominio = monta->InsereDominio(str_dominio);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na inserção de domínio");
		return (false);
	}
	return (dominio != NULL);
}

// -------------------------------------------------------------------------------------------------
void __fastcall TPreMonta::InsereBarraStart(void)
{
	monta->InsereBarraStart();
}

// -------------------------------------------------------------------------------------------------
void __fastcall TPreMonta::InsereBarraStop(void)
{
	monta->InsereBarraStop();
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCfg_Arranjo(TArranjoBDGD *arranjoBDGD)
{
	// variáveis locais
	strARRANJO2 str;
//	double freq = 60.; // hertz
//	double w = 2. * M_PI * freq;

	try
	{ // inicia atributos comuns
		str.tipico = true;
		str.data = DateOf(Now());
		str.arranjo_id = ARRANJO_ID + arranjoBDGD->Id;
		str.tipo_cfg = arranjoZ0Z1;
		str.fases = faseABCN;
		str.ramal = false;
		str.codigo = arranjoBDGD->Nome();
		str.extern_id = "";
		str.status = sttNOVO;
		// define atributo específicos do arranjo Z0Z1
		str.cfg_arranjoZ0Z1.iadm_amp = arranjoBDGD->Corrente_Max_A;
//		str.cfg_arranjoZ0Z1.z0.r = 0.0;
//		str.cfg_arranjoZ0Z1.z0.x = 0.0;
		str.cfg_arranjoZ0Z1.z1.r = arranjoBDGD->R1;
		str.cfg_arranjoZ0Z1.z1.x = arranjoBDGD->X1;
		str.cfg_arranjoZ0Z1.z0.r = arranjoBDGD->R1;
		str.cfg_arranjoZ0Z1.z0.x = arranjoBDGD->X1;
		// converte capacitância para nF/km
		str.cfg_arranjoZ0Z1.c0 = 0.0;
		str.cfg_arranjoZ0Z1.c1 = 0.0;
//		str.cfg_arranjoZ0Z1.c0 = (sdi.b0_s_km / w) * 1e-9;
//		str.cfg_arranjoZ0Z1.c1 = (sdi.b1_s_km / w) * 1e-9;
		str.cfg_arranjoZ0Z1.suporte_id = ID_NULO;
//		// insere Arranjo
		if (monta->InsereArranjo(str) == NULL)
			return (false);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add("Erro na inserção de cabo: " + arranjoBDGD->Nome());
		return (false);
	}
	return (true);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereCfg_TipoChave(TTipoUnidade* tipo_ch)
{
	// variáveis locais
	int tipochave_sinap;
	strTIPO_CHAVE str;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Tipo Chave");
		if (plog)
			plog->DefineValor("Código", tipo_ch->Descricao);
		// Verifica se é uma chave
		if((tipo_ch->Unidade).AnsiCompareIC("Unidade Seccionadora") != 0)
		return (true);
		// verifica se existe um Tipochave padrão que pode ser associado ao tipo indicado
		if ((tipochave_sinap = ExisteTipoChaveSinap(tipo_ch->Id)) == chaveINDEFINIDA)
		{ // define um novo tipo de chave
				tipochave_sinap = chaveINDEFINIDA + 1 + tipo_ch->Id;
				// insere novo TipoChave com ID negativo e tipo maior que chaveINDEFINIDA
				str.id = FalsoId();
				str.tipo = tipochave_sinap;
				str.codigo = tipo_ch->Unidade;
				str.modo_operacao = chaveOP_INDEFINIDA;
				// insere TipoChave
				if (!monta->InsereTipoChave(str))
					return (false);
		}
	}
	catch (Exception &e)
	{
		// #ifdef _DEBUG
		function = AnsiString(__FUNC__).c_str();
		msglog = "Exception caught em ";
		msglog += AnsiString(function);
		msglog += " sdi.descricao= " + tipo_ch->Unidade;
		msglog += " " + e.Message;
		if (log_thread)
			log_thread->AddLog(msglog, function, true);
		// #endif
		if (plog)
			plog->Add("Erro na inserção de tipo de chave: " + tipo_ch->Unidade);
		return (false);
	}
	return (true);
}


// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereEtd_Barra(TBarramento *barramento)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	strBARRA str;
	TBarraBDGD *barraBDGD;
	VTBarra *barra;
	double utm_x, utm_y;
	double lat, lon;

	try
	{ // cofigura Log
		if (plog)
			plog->DefineValor("Eqpto", "Barramento");
		if (plog)
			plog->DefineValor("Código", barramento->Codigo);

		// insere uma nova Barra
		str.bar_id = barramento->Id_Barra;
		str.codigo = barramento->Codigo;
		str.extern_id = barramento->Codigo;
		str.vnom = (barramento->Tensao->Tensao_V)/1000.;   // kV
		//variáveis locais
		barraBDGD = commons->GetBarra(barramento->Id_Barra);
		lat = lon = 0.0;
		if(barraBDGD != NULL)
		{
			lat = barraBDGD->Latitude;
			lon = barraBDGD->Longitude;
		}
		//PENDENTE DVC
		scg->LatLon_to_NorthEast(lat, lon, utm_y, utm_x);
		// ATENÇÃO: as coordenadas do barramento estão em cm mas a unidade indicada é metro
		str.utm.x = int(utm_x *100.); //
		str.utm.y = int(utm_y * 100.); //
		//
		// define coordenadas do esquemático
		str.esq.x1 = str.utm.x;
		str.esq.y1 = str.utm.y;
		str.esq.x2 = str.utm.x;
		str.esq.y2 = str.utm.y;
		InsereBarraStart();
		// insere Barra em VTMonta
		if ((barra = (VTBarra*)monta->InsereBarraEqpto(str)) == NULL)
			return (false);
		InsereBarraStop();
		// salva Barra na Se
		se->InsereBarra(barra);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
VTBarra* __fastcall TPreMonta::InsereEtd_Barra(TTrafoSubestacao *trafo_sub, int barra_id)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	AnsiString codigo;
	double vnom_kv;
	strBARRA str;
	TBarraBDGD *barraBDGD;
	VTBarra *barra;
	double utm_x, utm_y;
	double lat, lon;

	try
	{ // determina tensão nominal da Barra
		if (barra_id == trafo_sub->Id_Barra_1)
		{
			vnom_kv = (trafo_sub->Tensao_Pri->Tensao_V)/1000.;
			codigo = trafo_sub->Codigo + ".at";
		}
		else if (barra_id == trafo_sub->Id_Barra_1)
		{
			vnom_kv = (trafo_sub->Tensao_Sec->Tensao_V)/1000.;
			codigo = trafo_sub->Codigo + ".mt1";
		}
		else if (barra_id == trafo_sub->Id_Barra_1)
		{
			vnom_kv = (trafo_sub->Tensao_Ter->Tensao_V)/1000.;
			codigo = trafo_sub->Codigo + ".mt2";
		}
		// cofigura Log
		if (plog)
			plog->DefineValor("Eqpto", "Barra");
		if (plog)
			plog->DefineValor("Código", trafo_sub->Codigo);
		// insere uma nova Barra
		str.bar_id = barra_id;
		str.codigo = codigo;
		str.extern_id = codigo;
		str.vnom = vnom_kv;
		//variáveis locais
		barraBDGD = commons->GetBarra(trafo_sub->Id_Barra_1);
		// PENDENTE DVC
		lat = lon = 0.0;
		if(barraBDGD != NULL)
		{
			lat = barraBDGD->Latitude;
			lon = barraBDGD->Longitude;
		}
		//converte lat/lon para coordendas utm
		scg->LatLon_to_NorthEast(lat, lon, utm_y, utm_x);
		// ATENÇÃO: as coordenadas do barramento estão em cm mas a unidade indicada é metro
		str.utm.x = int(utm_x *100.); //
		str.utm.y = int(utm_y * 100.); //
		// define coordenadas do esquemático
		str.esq.x1 = str.utm.x;
		str.esq.y1 = str.utm.y;
		str.esq.x2 = str.utm.x;
		str.esq.y2 = str.utm.y;
		InsereBarraStart();
		// insere Barra em VTMonta
		if ((barra = (VTBarra*)monta->InsereBarraEqpto(str)) != NULL)
		{ // salva Barra na Se
			se->InsereBarra(barra);
		}
		InsereBarraStop();
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		barra = NULL;
	}
	return (barra);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereEtd_Rede(TSubestacao *subestacao)
{
	// variáveis locais
	strREDE str_rede;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Rede", subestacao->Codigo);
		if (plog)
			plog->DefineValor("Eqpto", "Rede");
		if (plog)
			plog->DefineValor("Código", subestacao->Codigo);
		// reinicia ponteiro p/ Se e Circuitos correntes
		se = NULL;
		// reinicia controle de Barra do Monta
		monta->InsereBarraStop();
		monta->InsereBarraStart();
        // proteção: verifica se já existe a Subestacao
		if ((se = ExisteSubestacao(subestacao->Id)) != NULL)
		{
			if (plog)
				plog->Add("Já existe a subestação");
			return (true);
		}
		// cria um objeto TSe e inicia seus dados
		if ((se = NewObjSe()) == NULL)    //destruido nesse destrutor, pois esta na lisSE
		{
			if (plog)
				plog->Add("Impossível criar subestação");
			return (false);
		}
		// inicia objeto Se
		se->id = subestacao->Id;
		se->codigo = subestacao->Codigo;
		se->nome = subestacao->Nome;
		// armazena Subestacao em lisSE
		lisSE->Add(se);
		subestacao->Rede = (VTRede*)monta->ExisteRede(subestacao->Codigo);
		// proteção: verifica se já existe a Rede
		if (subestacao->Rede != NULL)
		{
			if (plog)
				plog->Add("Já existe a rede da subestação");
			return (true);
		}
		// insere Rede em VTMonta
		str_rede.dominio_id = dominio->Id;
		str_rede.rede_id = subestacao->Id;
		str_rede.mrede_id = -1;
		str_rede.cluster_id = -1;
		str_rede.barini_id = -1;
		str_rede.color = clBlack;
		str_rede.codigo = subestacao->Nome;
        str_rede.extern_id = subestacao->Codigo;
		str_rede.tiporede.id = ID_NULO;
		str_rede.tiporede.segmento = redeETD;
		str_rede.tiporede.codigo = "";
		str_rede.estatica = true;
		str_rede.radial = false;
		subestacao->Rede = (VTRede*)monta->InsereRede(str_rede);
		if (subestacao->Rede == NULL)
		{
			if (plog)
				plog->Add("Não foi possível criar a subestação");
			return (false);
		}
		se->rede = subestacao->Rede;
	}
	catch (Exception &e)
	{ // cancela última Subestacao criada
		se = NULL;
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereEtd_Suprimento(TTrafoSubestacao *trafo_sub)
{
	// variáveis locais
	int ind_pat;
	strSUPRIMENTO str;
	VTSuprimento *suprimento;

	try
	{ // configura Resumo
		if (plog)
			plog->DefineValor("Eqpto", "Suprimento");
		str.sup_id = FalsoId();
		str.bar_id = trafo_sub->Id_Barra_1;
		str.codigo = "";
		str.extern_id = "";
		str.tiposup = supVF;
		str.smax = 4 * trafo_sub->S_Nom;
		str.vnom_kv = (trafo_sub->Tensao_Pri->Tensao_V)* 0.001;
		str.zeq0.r = 0;
		str.zeq0.x = 0;
		str.zeq1.r = 0;
		str.zeq1.x = 0;
		str.pcc_3f.p = 0;
		str.pcc_3f.q = 600;
		str.pcc_ft.p = 0;
		str.pcc_ft.q = 400;
		// define um patamar único
		str.curva.id = -1;
		str.curva.tipica = false;
		str.curva.num_pat = 1;
		str.curva.num_valor = nvCURVA_SUP;
		ind_pat = 0;
		str.curva.patamar[ind_pat].hm_ini.hora = 0;
		str.curva.patamar[ind_pat].hm_ini.minuto = 0;
		str.curva.patamar[ind_pat].hm_fim.hora = 24;
		str.curva.patamar[ind_pat].hm_fim.minuto = 0;
		str.curva.patamar[ind_pat].valor[indP] = 0;
		str.curva.patamar[ind_pat].valor[indQ] = 0;
		str.curva.patamar[ind_pat].valor[indV] = 1;
		str.curva.patamar[ind_pat].valor[indT] = 0;
		// define posição na Barra
		str.esq.posbar = POSBAR;
		str.esq.dxbar = DXBAR;
		str.esq.dybar = DYBAR;
		// insere Suprimento em Monta
		if ((suprimento = (VTSuprimento*)monta->InsereSuprimento(str)) == NULL)
			return (false);
		// associa o Suprimento com a Se
		// se->suprimento = suprimento;

	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereEtd_Suprimento(VTBarra *barra)
{
	// variáveis locais
	int ind_pat;
	strSUPRIMENTO str;
	VTSuprimento *suprimento;

	try
	{ // configura Resumo
		if (plog)
			plog->DefineValor("Eqpto", "Suprimento");
		str.sup_id = FalsoId();
		str.bar_id = barra->Id;
		str.codigo = "";
		str.extern_id = "";
		str.tiposup = supVF;
		str.smax = 100;
		str.vnom_kv = barra->vnom;
		str.zeq0.r = 0;
		str.zeq0.x = 0;
		str.zeq1.r = 0;
		str.zeq1.x = 0;
		str.pcc_3f.p = 0;
		str.pcc_3f.q = 600;
		str.pcc_ft.p = 0;
		str.pcc_ft.q = 400;
		// define um patamar único
		str.curva.id = -1;
		str.curva.tipica = false;
		str.curva.num_pat = 1;
		str.curva.num_valor = nvCURVA_SUP;
		ind_pat = 0;
		str.curva.patamar[ind_pat].hm_ini.hora = 0;
		str.curva.patamar[ind_pat].hm_ini.minuto = 0;
		str.curva.patamar[ind_pat].hm_fim.hora = 24;
		str.curva.patamar[ind_pat].hm_fim.minuto = 0;
		str.curva.patamar[ind_pat].valor[indP] = 0;
		str.curva.patamar[ind_pat].valor[indQ] = 0;
		str.curva.patamar[ind_pat].valor[indV] = 1;
		str.curva.patamar[ind_pat].valor[indT] = 0;
		// define posição na Barra
		str.esq.posbar = POSBAR;
		str.esq.dxbar = DXBAR;
		str.esq.dybar = DYBAR;
		// insere Suprimento em Monta
		if ((suprimento = (VTSuprimento*)monta->InsereSuprimento(str)) == NULL)
			return (false);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereEtd_Trafo(TTrafoSubestacao *trafo_sub)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso;
	VTBarra *bar_at;
	double s_vent_1, s_vent_2, s_vent;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Trafo");
		if (plog)
			plog->DefineValor("Código", trafo_sub->Codigo);
		// proteção: verifica se existe a Se
		if ((trafo_sub->Id_Sub == NULL) || (se->id != trafo_sub->Id_Sub))
		{
			if (plog)
				plog->Add("Trafo não está associado a ETD");
			return (true);
		}
		// verifica potência nominal
		s_vent_1 = trafo_sub->S_VentFor_1_MVA;
		if (IsDoubleZero(s_vent_1))
		{
			if (plog)
				plog->Add("Trafo de ETD c/ potência ventilada_1 nula");
		}
		s_vent_2 = trafo_sub->S_VentFor_2_MVA;
		if (IsDoubleZero(s_vent_2))
		{
			if (plog)
				plog->Add("Trafo de ETD c/ potência ventilada_2 nula");
		}
		s_vent = (s_vent_1 >= s_vent_2) ? s_vent_1:s_vent_2;
		if (IsDoubleZero(s_vent))
		{
			if (IsDoubleZero(trafo_sub->S_Nom))
			{
				if (plog)
					plog->Add("Trafo de ETD c/ potência nominal nula");
				trafo_sub->S_Nom = 100.;
			}
			else
			{
				trafo_sub->S_Nom = trafo_sub->S_Nom *0.001;
			}
		}
		else
		{
			trafo_sub->S_Nom = s_vent;
		}
		// verifica tensão do primário
		if (IsDoubleZero(trafo_sub->Tensao_Pri->Tensao_V))
		{
			if (plog)
				plog->Add("Tensão nominal primária nula");
			// Tensao de 88 kV   - Tem ID = 84 na tabela de domínio
			trafo_sub->Tensao_Pri = commons->GetTensao(84);
		}
		// verifica tensão do secundário
		if (IsDoubleZero(trafo_sub->Tensao_Sec->Tensao_V))
		{
			if (plog)
				plog->Add("Tensão nominal secundária nula");
			// Tensao de 13.8 kV   - Tem ID = 49 na tabela de domínio
			trafo_sub->Tensao_Sec = commons->GetTensao(49);
		}
		// verifica tensão do terciário, caso tenha sido definida a Barra
		if (trafo_sub->Id_Barra_3 > 0)
		{
			if (IsDoubleZero(trafo_sub->Tensao_Ter->Tensao_V))
			{
				if (plog)
					plog->Add("Tensão nominal terciária nula");
				trafo_sub->Tensao_Ter = trafo_sub->Tensao_Sec;
			}
		}
		// verifica se existe a Barra do primário
		if (se->ExisteBarra(trafo_sub->Id_Barra_1) == NULL)
		{
		   if (InsereEtd_Barra(trafo_sub, trafo_sub->Id_Barra_1) == NULL)
				return (false);
			// insere Suprimento na Barra do primário
			InsereEtd_Suprimento(trafo_sub);
		}
		// verifica se existe a Barra do secundário
		if (se->ExisteBarra(trafo_sub->Id_Barra_2) == NULL)
		{ // cria Barra
			if (!InsereEtd_Barra(trafo_sub, trafo_sub->Id_Barra_2))
				return (false);
		}
		// verifica se foi definida Barra do terciário
		if (trafo_sub->Id_Barra_3 > 0)
		{ // verifica se existe a Barra do terciário
			if (se->ExisteBarra(trafo_sub->Id_Barra_3) == NULL)
			{ // cria Barra
				if (!InsereEtd_Barra(trafo_sub, trafo_sub->Id_Barra_3))
					return (false);
			}
		}
		// verifica se é um trafo de 2 ou 3 enrolamentos
		if (trafo_sub->Id_Barra_3 > 0)
			sucesso = InsereEtd_Trafo3E(trafo_sub);
		else
			sucesso = InsereEtd_Trafo2E(trafo_sub);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereEtd_Trafo2E(TTrafoSubestacao *trafo_sub)
{
	// variáveis locais
	bool sucesso;
	strTRAFO str;
	VTTrafo *trafo;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Trafo");
		if (plog)
			plog->DefineValor("Código", trafo_sub->Codigo);
		// insere Trafo na Rede
		str.trf_id = trafo_sub->Id;
		str.rede_id = se->rede->Id;
		str.bar1_id = trafo_sub->Id_Barra_1;
		str.bar2_id = trafo_sub->Id_Barra_2;
		str.codigo = trafo_sub->Codigo;
		str.extern_id = trafo_sub->Codigo;
		str.snom_mva = trafo_sub->S_Nom;
		str.z0.r = 0.0;
		str.z0.x = 0.1;
		str.z1.r = 0.0;
		str.z1.x = 0.1;
		str.perda_ferro = trafo_sub->Perda_Fe_Perc;
		// primário
		str.pri.fases = faseABC;
		str.pri.ligacao = lgTRIANG;
		str.pri.vnom = (trafo_sub->Tensao_Pri->Tensao_V)/1000.;
		str.pri.tap = (trafo_sub->Tensao_Pri->Tensao_V)/1000.;
		str.pri.defasagem = 0;
		str.pri.zterra.r = 0;
		str.pri.zterra.x = 0;
		// secundário
		str.sec.fases = faseABCNT;
		str.sec.ligacao = lgEST_AT;
		str.sec.vnom = (trafo_sub->Tensao_Sec->Tensao_V)/1000.;
		str.sec.tap = (trafo_sub->Tensao_Sec->Tensao_V)/1000.;
		str.sec.defasagem = -30.;
		str.sec.zterra.r = 0;
		str.sec.zterra.x = 0;
		// dados de LTC
		str.ltc.tipo = ltcFIXO;
		str.ltc.bar_id = ID_NULO;
		str.ltc.v_pu = 1.0;
		// posição nas barras
		str.esq.posbar1 = 50.;
		str.esq.posbar2 = 50.;
		if ((trafo = (VTTrafo*)monta->InsereTrafo(str)) == NULL)
			return (false);
		// salva Trafo na Se
		se->InsereTrafo(trafo);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereEtd_Trafo3E(TTrafoSubestacao *trafo_sub)
{
	// variáveis locais
	bool sucesso;
	double z_pu, r_pu, x_pu, k;
	strTRAFO3E str;
	VTTrafo3E *trafo3E;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Trafo3E");
		if (plog)
			plog->DefineValor("Código", trafo_sub->Codigo);
		// Assome esses valores de impedâncias porque no BDGD não se tem registro
		str.zps1.r = str.zpt1.r = str.zst1.r = 0.001;
		str.zps1.x = str.zpt1.x = str.zst1.x = 0.03;
		// Coloca os mesmos valores na seq zero
		str.zps0 = str.zpt0 = str.zst0 = str.zps1;
		// insere Trafo3E na Rede
		str.trf_id = trafo_sub->Id;
		str.rede_id = se->rede->Id;
		str.bar1_id = trafo_sub->Id_Barra_1;
		str.bar2_id = trafo_sub->Id_Barra_2;
		str.bar3_id = trafo_sub->Id_Barra_3;
		str.codigo = trafo_sub->Codigo;
		str.extern_id = trafo_sub->Codigo;
		str.perda_ferro = trafo_sub->Perda_Fe_Perc;
		// primário
		str.pri.fases = faseABC;
		str.pri.ligacao = lgTRIANG;
		str.pri.snom = trafo_sub->S_Nom;
		str.pri.vnom = (trafo_sub->Tensao_Pri->Tensao_V)/1000.;
		str.pri.tap = (trafo_sub->Tensao_Pri->Tensao_V)/1000.;
		str.pri.defasagem = 0;
		str.pri.zterra.r = 0;
		str.pri.zterra.x = 0;
		// secundário
		str.sec.fases = faseABCNT;
		str.sec.ligacao = lgEST_AT;
		str.sec.snom = trafo_sub->S_Nom * 0.5;
		str.sec.vnom = (trafo_sub->Tensao_Sec->Tensao_V)/1000.;
		str.sec.tap = (trafo_sub->Tensao_Sec->Tensao_V)/1000.;
		str.sec.defasagem = -30.;
		str.sec.zterra.r = 0;
		str.sec.zterra.x = 0;
		// terciário
		str.ter.fases = faseABCNT;
		str.ter.ligacao = lgEST_AT;
		str.ter.snom = trafo_sub->S_Nom * 0.5;
		str.ter.vnom = (trafo_sub->Tensao_Ter->Tensao_V)/1000.;
		str.ter.tap = (trafo_sub->Tensao_Ter->Tensao_V)/1000.;
		str.ter.defasagem = -30.;
		str.ter.zterra.r = 0;
		str.ter.zterra.x = 0;
		// posição nas barras
		str.esq.posbar1 = 50.;
		str.esq.posbar2 = 50.;
		str.esq.posbar3 = 50.;
		if ((trafo3E = (VTTrafo3E*)monta->InsereTrafo3E(str)) == NULL)
			return (false);
		// salva Trafo3E na Se
		se->InsereTrafo3E(trafo3E);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InserePri_Barra(TBarraBDGD *barraBDGD)
{
	// variáveis locais
	strBARRA str;
	VTBarra *barra = NULL;
	double utm_x, utm_y;
	double lat, lon;

	try
	{ // atualiza log
		if (plog)
			plog->DefineValor("Eqpto", "Barra");
		if (plog)
			plog->DefineValor("Código", barraBDGD->Codigo);
		// proteção
		if (pri == NULL)
		{
			if (plog)
				plog->Add("Não existe rede primária ativa");
			return (false);
		}
		// cria uma nova Barra
		str.bar_id = barraBDGD->Id;
		str.codigo = barraBDGD->Codigo;
		str.vnom = pri->vnom_kv;
		// PENDENTE DVC
		lat = lon = 0.0;
		if(barraBDGD != NULL)
		{
			lat = barraBDGD->Latitude;
			lon = barraBDGD->Longitude;
		}
		// UTM
		scg->LatLon_to_NorthEast(lat, lon, utm_y, utm_x);
		// ATENÇÃO: as coordenadas do barramento estão em cm mas a unidade indicada é metro
		str.utm.x = int(utm_x *100.); //
		str.utm.y = int(utm_y * 100.); //
		// esquemático
		str.esq.x1 = str.utm.x;
		str.esq.y1 = str.utm.y;
		str.esq.x2 = str.utm.x;
		str.esq.y2 = str.utm.y;
		// Start
		InsereBarraStart();
		// insere Barra em VTMonta
		if ((barra = (VTBarra*)monta->InsereBarraEqpto(str)) == NULL)
			return (false);
		InsereBarraStop();
		// salva Barra no primario
		pri->InsereBarra(barra);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}
//
//// ---------------------------------------------------------------------------
//VTBarra* __fastcall TPreMonta::InserePri_BarraFicticia(VTBarra *bar_ref)
//{
//	// variáveis locais
//	strBARRA str;
//	VTBarra *barra;
//
//	try
//	{ // cria uma nova Barra
//		str.bar_id = FalsoId();
//		str.codigo = "";
//		str.extern_id = "";
//		str.vnom = bar_ref->vnom;
//		str.utm.x = bar_ref->utm.x;
//		str.utm.y = bar_ref->utm.y + 500;
//		// esquemático
//		str.esq.x1 = str.utm.x;
//		str.esq.y1 = str.utm.y;
//		str.esq.x2 = str.utm.x;
//		str.esq.y2 = str.utm.y;
//		// insere Barra em VTMonta
//		// monta->InsereBarraStart();
//		barra = (VTBarra*)monta->InsereBarraEqpto(str);
//		// monta->InsereBarraStop();
//	}
//	catch (Exception &e)
//	{
//		if (plog)
//			plog->Add(ERRO_DE_SISTEMA);
//		barra = NULL;
//	}
//	return (barra);
//}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InserePri_Capacitor(TCapacitorBDGD *cap)
{
	// variaveis locais
	bool sucesso = true;

	try
	{ // cofigura Log
		if (plog)
			plog->DefineValor("Eqpto", "Capacitor");
		if (plog)
			plog->DefineValor("Código", cap->Codigo);
		// verifica se o Capacitor série pertence ao Circuito selecionado
		if (pri == NULL)
		{
			if (plog)
				plog->Add("Não está associado a rede primária");
			return (true);
		}
		if(cap->Barra_2 != NULL)
		{
			// Capacitor serie
			sucesso = InserePri_Cap_Serie(cap);
		}
		else
		{
			// Capacitor paralelo - se assume que a barra1 deve estar sempre definida
			sucesso = InserePri_Cap_Paralelo(cap);
		}
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InserePri_Cap_Paralelo(TCapacitorBDGD *cap)
{
	// variáveis locais
	strCAPACITOR str;
	VTBarra *barra = NULL;

	// Insere a barra
	if(cap->Barra_1 != NULL)
	{
		if(pri->ExisteBarra(cap->Barra_1->Id) == NULL)
		{
				InserePri_Barra(cap->Barra_1);
		}
		barra = monta->ExisteBarra(cap->Barra_1->Id);
	}
	if(barra == NULL)
	{
	   if (plog)
		   plog->Add("Não existe barra de conexão");
		   return (true);
	}
	// preenche estrutura strCAPACITOR
	str.cap_id = cap->Id;
	str.codigo = cap->Codigo;
	str.bar_id = barra->Id;
	str.ligacao = ConverteTipoLigacao(cap->Tipo_Unidade->Unidade);
	if (str.ligacao == lgINDEFINIDA)
		{
			if (plog)
				plog->Add("Tipo de ligação inválida");
			str.ligacao = lgEST_AT;
		}
	str.vnom = pri->vnom_kv;
	str.q_mvar = cap->Tipo_Q->Pot_Reativa_kVAr * 0.001;
	str.esq.posbar = 50.;
	str.esq.dxbar = 0;
	str.esq.dybar = -DYBAR;
	// insere Capacitor paralelo em VTMonta
	if (monta->InsereCapacitor(str) == NULL)
			return (false);
}


// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InserePri_Cap_Serie(TCapacitorBDGD *cap)
{
	// variáveis locais
	strCAPSERIE str;
	VTBarra *barra1, *barra2;

	barra1 = barra2 = NULL;

	// Insere a barra 1 e 2
	if(cap->Barra_1 != NULL)
	{
		if(pri->ExisteBarra(cap->Barra_1->Id) == NULL)
		{
				InserePri_Barra(cap->Barra_1);
		}
		barra1 = monta->ExisteBarra(cap->Barra_1->Id);
	}
	if(cap->Barra_2 != NULL)
	{
		if(pri->ExisteBarra(cap->Barra_2->Id) == NULL)
		{
				InserePri_Barra(cap->Barra_2);
		}
		barra2 = monta->ExisteBarra(cap->Barra_2->Id);
	}
	if((barra1 == NULL) || (barra2 == NULL))
	{
	   if (plog)
		   plog->Add("Não existe barra de conexão");
		   return (true);
	}
	// preenche estrutura strCAPACITOR
	str.id = cap->Id;
	str.rede_id = cap->Id_Rede;
	str.codigo = cap->Codigo;
	str.bar1_id = barra1->Id;
	str.bar2_id = barra2->Id;
	str.vnom = pri->vnom_kv;
	if(cap->Tipo_Q != NULL)
	{
		str.snom = cap->Tipo_Q->Pot_Reativa_kVAr * 0.001 ;
	}
	str.esq.posbar1 = 50.;
	str.esq.posbar2 = 50.;
	// insere Capacitor série em VTMonta
	if (monta->InsereCapserie(str) == NULL)
			return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InserePri_Chave(TChaveBDGD *chaveBDGD)
{
	// variáveis locais
	strCHAVE str;
	VTBarra *barra1, *barra2;
	VTTipoChave *tipo_ch = NULL;

	try
	{ // cofigura Log
		if (plog)
			plog->DefineValor("Eqpto", "Chave");
		if (plog)
			plog->DefineValor("Código", chaveBDGD->Codigo);
		// verifica se a Chave pertence ao Circuito selecionado
		if (pri == NULL)
		{
			if (plog)
				plog->Add("Não está associada a rede primária");
			return (true);
		}
		// verifica se Barras iguais
		if (chaveBDGD->Barra_1 == chaveBDGD->Barra_2)
		{ // não cria trecho com barras iguais
			if (plog)
				plog->Add("Barra inicial igual à barra final");
			return (true);
		}
		barra1 = barra2 = NULL;
		// Insere as barras 1 e 2
		if(chaveBDGD->Barra_1 != NULL)
		{
			if(pri->ExisteBarra(chaveBDGD->Barra_1->Id) == NULL)
			{
				InserePri_Barra(chaveBDGD->Barra_1);
			}
			// verifica se existem as Barras
			barra1 = monta->ExisteBarra(chaveBDGD->Barra_1->Id);
		}

		if(chaveBDGD->Barra_2 != NULL)
		{
			if(chaveBDGD->Barra_2->Id == 80616)
			{
				int a =0;
            }
			if(pri->ExisteBarra(chaveBDGD->Barra_2->Id) == NULL)
			{
			InserePri_Barra(chaveBDGD->Barra_2);
			}
            // verifica se existem as Barras
			barra2 = monta->ExisteBarra(chaveBDGD->Barra_2->Id);
		}
		if ((barra1 == NULL) && (barra2 == NULL))
		{
			if (plog)
				plog->Add("Não existem as duas barras");
			return (true);
		}
		if ((barra1 != NULL) && (barra2 == NULL))
		{
			if (plog)
				plog->Add("Não existe a barra " + IntToStr(chaveBDGD->Barra_2->Id));
			return (true);
		}
		else if ((barra1 == NULL) && (barra2 != NULL))
		{
			if (plog)
				plog->Add("Não existe a barra " + IntToStr(chaveBDGD->Barra_1->Id));
			return (true);
		}
		// preenche estrutura strCHAVE
		str.chv_id = chaveBDGD->Id;
		str.rede_id = chaveBDGD->Id_Rede;
		str.codigo = chaveBDGD->Codigo;
		if(chaveBDGD->Tipo_Unidade != NULL)
		{
			str.tipo_gis = chaveBDGD->Tipo_Unidade->Descricao;
			//str.tipo_gis = "";
			//tipo_ch = tipos->ExisteTipoChave(chaveBDGD->Id);
			tipo_ch = tipos->ExisteTipoChave(chaveBDGD->Tipo_Unidade->Descricao);
		}
		if(chaveBDGD->Elo_Fusivel != NULL)
		{
			str.elo_fusivel = chaveBDGD->Elo_Fusivel->Codigo;
		}
		str.bar1_id = barra1->Id;
		str.bar2_id = barra2->Id;
		str.estado = chaveBDGD->Estado_Normal;
		if(chaveBDGD->Corrente_Nom != NULL)
		{
			str.corn = chaveBDGD->Corrente_Nom->Corrente_A;
		}
		//
		str.tipochave.id = (tipo_ch != NULL) ? tipo_ch->Id : ID_NULO;
		str.tipochave.tipo = (tipo_ch != NULL) ? tipo_ch->Tipo : chaveINDEFINIDA;
		//str.tipochave.codigo = (tipo_ch != NULL) ? tipo_ch->Codigo : "Indefinida";
		str.tipochave.codigo = (chaveBDGD->Tipo_Unidade != NULL) ? chaveBDGD->Tipo_Unidade->Descricao : AnsiString("Indefinida");
		str.telecomandada = chaveBDGD->Telecomandada;
		str.tecno_comunicacao = comDESCONHECIDO;
		str.esq.posbar1 = 50.;
		str.esq.posbar2 = 50.;
		// insere Chave em VTMonta
		if (monta->InsereChave(str) == NULL)
			return (true);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InserePri_Consumidor(TCargaBDGD *cargaMT)
{
	// variáveis locais
	VTCarga *carga;
	int fases_tag;
	strCARGA str;

	//MonthOfTheYear(Now());
	//mes = Month(Now());

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Cons.MT");
		if (plog)
			plog->DefineValor("Código", cargaMT->Codigo);
		if (pri == NULL)
		{
			if (plog)
				plog->Add("Não está associado à primario");
			return (true);
		}
		// preenche estrutura strCARGA
		str.car_id = cargaMT->Id;
		str.bar_id = cargaMT->Barra->Id;
		str.codigo = cargaMT->Codigo;
		str.classe_cons = ConverteClasseConsumidor(cargaMT->Classe_Subclasse->Codigo);
		// Verificação
		if(cargaMT->Resultado_Inspecao <= 0)
		{
            cargaMT->Resultado_Inspecao = -1;
		}
		str.categoria = cargaMT->Resultado_Inspecao;
		// consA4;
		str.grupoA = true;
		str.fases = fases->ExisteFase(cargaMT->Fases_Con->Codigo);
		// modelo de carga Icte
		str.modelo.icte_pu = 1;
		str.modelo.scte_pu = 0;
		str.modelo.zcte_pu = 0;
		// cancela dados de IP
		str.ip.num_lamp = 0.;
		str.ip.pot_kw = 0.;
		str.ip.fat_pot = 1.;
		// esquemático
		str.esq.posbar = 50.;
		str.esq.dxbar = 0;
		str.esq.dybar = -DYBAR;
        // Verifica o tipo de importação que foi selecionada
		switch (ConsA4)
		{
		case opENERGIA:
			{
				// energia
				str.energia_mwhmes = cargaMT->Ene_kWh[1] * 0.001;
				// valida valor zerado
				if (IsDoubleZero(str.energia_mwhmes))
				{
					if (plog)
						plog->Add("Consumo zerado");
				}
				carga = CriaConsMTenergia(str);
				break;
			}
			case opCURVA:
			{
				if (IsDoubleZero(cargaMT->Dem_Max_kW[1]))
				{
					if (plog)
						plog->Add("Demanda faturada ponta zerada");
				}
				// Considerei fora de ponta como 90% do valor de ponta
				if (IsDoubleZero(cargaMT->Dem_Max_kW[1]*0.9))
				{
					if (plog)
						plog->Add("Demanda faturada fora ponta zerada");
				}
				carga = CriaConsMTdemanda(str, cargaMT);
				break;
			}
		default:
			break;
		}
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

//// ---------------------------------------------------------------------------
//bool __fastcall TPreMonta::InserePri_DisjuntorFicticio(void)
//{
//	// variáveis locais
//	strCHAVE str;
//	TCircuito *circuito;
//	TSe *se;
//	VTChave *chave;
//	VTLigacao *ligacao;
//	VTBarra *bar_ini;
//	VTBarra *bar_dj;
//
//	// loop para todas Se
//	for (int i = 0; i < lisSE->Count; i++)
//	{
//		se = (TSe*)lisSE->Items[i];
//		// loop para todos Circuitos da Se
//		for (int j = 0; j < se->lisCIRCUITO->Count; j++)
//		{
//			circuito = (TCircuito*)se->lisCIRCUITO->Items[j];
//			// determina Barra inicial da Rede
//			if ((bar_ini = circuito->rede->BarraInicial()) == NULL)
//				continue;
//			// cria uma nova Barra conectar o disjuntor
//			if ((bar_dj = InserePri_BarraFicticia(bar_ini)) == NULL)
//				continue;
//			// cria uma Chave do tipo DJ
//			str.chv_id = FalsoId();
//			str.rede_id = circuito->rede->Id;
//			str.codigo = "DJ." + circuito->rede->Codigo;
//			str.extern_id = "";
//			str.bar1_id = bar_ini->Id;
//			str.bar2_id = bar_dj->Id;
//			str.estado = chvFECHADA;
//			str.corn = 600;
//			str.tipochave.id = -1;
//			str.tipochave.tipo = chaveDJ;
//			str.tipochave.codigo = "";
//			str.esq.posbar1 = 50.;
//			str.esq.posbar2 = 50.;
//			// insere Chave em VTMonta
//			if ((chave = (VTChave*)monta->InsereChave(str)) == NULL)
//				continue;
//			// redefine barra das Ligacoes do primário que conectavam bar_ini
//			lisEQP->Clear();
//			circuito->rede->LisLigacao(lisEQP, bar_ini);
//			for (int k = 0; k < lisEQP->Count; k++)
//			{
//				ligacao = (VTLigacao*)lisEQP->Items[k];
//				// proteção: não altera as barras do disjuntor que foi criado agora
//				if (ligacao == chave)
//					continue;
//				if (ligacao->pbarra1 == bar_ini)
//					ligacao->pbarra1 = bar_dj;
//				else if (ligacao->pbarra2 == bar_ini)
//					ligacao->pbarra2 = bar_dj;
//				else if (ligacao->pbarra3 == bar_ini)
//					ligacao->pbarra3 = bar_dj;
//			}
//		}
//	}
//	return (true);
//}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InserePri_DisjuntorFicticio(VTBarra *barraIni, VTBarra *barraTrafo)
{
	//variávies locais
	strCHAVE str;
	VTChave *chave;

	if ((barraTrafo == NULL) || (barraIni == NULL))
	{
		if (plog)
			plog->Add("Não existem as duas barras");
				return (true);
	}
	// cria uma Chave do tipo DJ
	str.chv_id = FalsoId();
	str.rede_id = pri->id;
	str.codigo = "DJ." + pri->codigo;
	str.extern_id = "";
	str.bar1_id = barraTrafo->Id;
	str.bar2_id = barraIni->Id;
	str.estado = chvFECHADA;
	str.corn = 600;
	str.tipochave.id = -1;
	str.tipochave.tipo = chaveDJ;
	str.tipochave.codigo = "";
	str.esq.posbar1 = 50.;
	str.esq.posbar2 = 50.;
	// insere Chave em VTMonta
	if ((chave = (VTChave*)monta->InsereChave(str)) == NULL)
		return (false);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InserePri_Gerador(TGeradorBDGD *geradorMT)
{
	// variáveis locais
	double s_mva, p_mw, q_mvar;
	strGERADOR str;
	VTBarra *barra;

	try
	{ // configura Resumo
		if (plog)
			plog->DefineValor("Eqpto", "Gerador");
		if (plog)
			plog->DefineValor("Código", geradorMT->Codigo);
		// proteção
		if (pri == NULL)
		{
			if (plog)
				plog->Add("Não está associado á rede primária");
			return (true);
		}
		// inicia estrutura strGerador
		str.obra = false;
		str.ger_id = geradorMT->Id;
		// Insere a barra
		if(geradorMT->Barra != NULL)
		{
			if(pri->ExisteBarra(geradorMT->Barra->Id) == NULL)
			{
					InserePri_Barra(geradorMT->Barra);
			}
			barra = monta->ExisteBarra(geradorMT->Barra->Id);
		}
		if (barra != NULL)
		{ // caso exista e nao seja isolada
			if (!IsBarraIsolada(barra))
			{
				str.bar_id = geradorMT->Barra->Id;
			}
		}
		str.codigo = geradorMT->Codigo;
		str.extern_id = geradorMT->Codigo;
		str.tiposup = supPQ;           // Default
		str.tipo_geracao = gerPCH;     // Default
		str.ligacao = lgEST_AT;        // Default
		str.fases = fases->ExisteFase(geradorMT->Fases_Con->Codigo);;
		str.vnom = (geradorMT->Tensao->Tensao_V) * 0.001;
		str.snom_mva = (geradorMT->P_Inst_kW / 0.92) * 0.001;
		str.smax_mva = str.snom_mva * 1.2;
		str.fplim = 0.9;
		str.z0.r = geradorMT->R0;
		str.z0.x = geradorMT->X0;
		str.z1.r = geradorMT->R1;
		str.z1.x = geradorMT->X1;
		// define curva de geração própria
		str.curva.id = -1;
		str.curva.tipica = false;
		str.curva.num_pat = pat01HORA;
		str.curva.num_valor = nvCURVA_PQVT;
		// define pot.ativa e reativa a partir de Snom e considerando fatpot = 0.92
		s_mva = str.snom_mva;
		p_mw = geradorMT->P_Inst_kW * 0.001;
		q_mvar = sqrt((s_mva * s_mva) - (p_mw * p_mw));
		for (int np = 0; np < pat01HORA; np++)
		{ // define hora inicial e final do patamar
			str.curva.patamar[np].hm_ini = pat_edp[np].hm_ini;
			str.curva.patamar[np].hm_fim = pat_edp[np].hm_fim;
			// define valores de ponta/fora de ponta
			if (pat_edp[np].ponta)
			{ // demanda de ponta
				str.curva.patamar[np].valor[indP] = p_mw;
				str.curva.patamar[np].valor[indQ] = q_mvar;
				str.curva.patamar[np].valor[indV] = 1.;
				str.curva.patamar[np].valor[indT] = 0.;
			}
			else
			{ // demanda fora de ponta
				str.curva.patamar[np].valor[indP] = p_mw;
				str.curva.patamar[np].valor[indQ] = q_mvar;
				str.curva.patamar[np].valor[indV] = 1.;
				str.curva.patamar[np].valor[indT] = 0.;
			}
		}
		// define coordenadas p/ esquemático
		str.esq.dxbar = -2000;
		str.esq.dybar = 0;
		// insere Gerador em VTMonta
		if (!monta->InsereGerador(str))
			return (false);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InserePri_Rede(TRedeMT *primario)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	strREDE str;
	VTRede *rede;
	VTTrafo *trafo = NULL;
	VTTrafo3E *trafo3E = NULL;
	VTBarra *barra, *barraInicial, *barraTrafo;
	TBarraBDGD *barraIni;
	int id_barra_mt;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Rede", primario->Codigo);
		if (plog)
			plog->DefineValor("Eqpto", "Rede MT");
		if (plog)
			plog->DefineValor("Código", primario->Codigo);
		// reinicia ponteiro p/ Circuito corrente
		pri = NULL;
		// reinicia controle de Barra do Monta
		monta->InsereBarraStop();
		monta->InsereBarraStart();
		barra = NULL;
		// proteção: verifica se existe uma Subestacao
		if (se == NULL)
		{
			if (plog)
				plog->Add("Primário não está associado a ETD");
			return (false);
		}
		// proteção: verifica se já existe um Pri c/ mesmo ID
		if (se->ExistePri(primario->Id))
		{
			if (plog)
				plog->Add("ID repetido");
			return (false);
		}
		// proteção: verifica se existe uma Rede com o mesmo código
		if (monta->ExisteRede(primario->Codigo) != NULL)
		{
			if (plog)
				plog->Add("Primário já existente");
			return (false);
		}
	   // insere Rede em VTMonta
		str.dominio_id = dominio->Id;
		str.rede_id = primario->Id;
		str.mrede_id = ID_NULO;
		str.cluster_id = ID_NULO;
		str.barini_id = primario->Id_Barra_Ini;
		str.codigo = primario->Nome;
		//str.extern_id = IntToStr(primario->Id);
		str.extern_id = primario->Codigo;
		str.estatica = false;
		str.radial = true;
		str.color = DefineCorRedePrimaria(primario);
		str.tiporede.id = ID_NULO;
		str.tiporede.segmento = redePRI;
		str.tiporede.codigo = "";
		// insere Rede em Monta
		if ((rede = (VTRede*)monta->InsereRede(str)) == NULL)
			return (false);
		// cria um objeto Circuito e inicia seus dados
		if ((pri = NewObjPri()) == NULL)
		{
			if (plog)
				plog->Add(ERRO_DE_SISTEMA);
			return (false);
		}
		// atualiza pri
		pri->id = primario->Id;
		pri->codigo = primario->Codigo;
		pri->nome = primario->Nome;
		pri->vnom_kv = (primario->Tensao_Nom->Tensao_V) * 0.001;
		pri->rede = rede;
		//Insere Barra inicial
		barraInicial = monta->ExisteBarra(primario->Id_Barra_Ini);
		if(barraInicial == NULL)
		{
			barraIni = commons->GetBarra(primario->Id_Barra_Ini);
			if (barraIni != NULL)
			{
				InserePri_Barra(barraIni);
			}
			barraInicial = monta->ExisteBarra(primario->Id_Barra_Ini);
			// insere Pri na SE
			se->InserePri(pri);
		}
//		barraTrafo = se->ExisteBarra(primario->Id_Barra_Trafo);
//		if(barraTrafo != NULL)
//		{
//			InserePri_DisjuntorFicticio(barraInicial, barraTrafo);
//		}
//		else
//		{
			// Coloca um suprimento diretamente no alimentador
			InsereEtd_Suprimento(barraInicial);
//      }
	}
	catch (Exception &e)
	{
		pri = NULL;
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InserePri_Regulador(TReguladorMT *reg)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	int reg_max_percent;
	strREGULADOR str;
	double reg_max_pu;
	VTBarra *barra1, *barra2;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Regulador");
		if (plog)
			plog->DefineValor("Código", reg->Codigo);
		// verifica se o Regulador pertence ao Circuito selecionado
		if (pri == NULL)
		{
			if (plog)
				plog->Add("Não está associado a rede primária");
			return (true);
		}
				// verifica se Barras iguais
		if (reg->Barra_1 == reg->Barra_2)
		{ // não cria trecho com barras iguais
			if (plog)
				plog->Add("Barra inicial igual à barra final");
			return (true);
		}
		barra1 = barra2 = NULL;
		// Insere as barras 1 e 2
		if(reg->Barra_1 != NULL)
		{
			if(pri->ExisteBarra(reg->Barra_1->Id) == NULL)
			{
				InserePri_Barra(reg->Barra_1);
			}
			// verifica se existem as Barras
			barra1 = monta->ExisteBarra(reg->Barra_1->Id);
		}
		if(reg->Barra_2 != NULL)
		{
			if(pri->ExisteBarra(reg->Barra_2->Id) == NULL)
			{
				InserePri_Barra(reg->Barra_2);
			}
			barra2 = monta->ExisteBarra(reg->Barra_2->Id);
        }
		if ((barra1 == NULL) && (barra2 == NULL))
		{
			if (plog)
				plog->Add("Não existem as duas barras");
			return (true);
		}
		if ((barra1 != NULL) && (barra2 == NULL))
		{
			if (plog)
				plog->Add("Não existe a barra " + IntToStr(reg->Barra_2->Id));
			return (true);
		}
		else if ((barra1 == NULL) && (barra2 != NULL))
		{
			if (plog)
				plog->Add("Não existe a barra " + IntToStr(reg->Barra_1->Id));
			return (true);
		}
		if (IsDoubleZero(reg->Tipo_S_Nom->Pot_Aparente_kVA))
		{
			if (plog)
				plog->Add("Potência nominal nula");
			// potencia default de 300kVA
			reg->Tipo_S_Nom = commons->GetTipoPotAparente(34);
		}
		reg_max_percent = 10;
		// proteção p/ tap_pu
		reg_max_pu = reg_max_percent * 0.01;
		if ((reg->Tensao_Regulacao < (1.0 - reg_max_pu)) || (reg->Tensao_Regulacao > (1.0 + reg_max_pu)))
		{
			if (plog)
				plog->Add("Valor de tap_pu inválido");
			if (IsDoubleZero(reg->Tensao_Regulacao))
				reg->Tensao_Regulacao = 1.0;
			else if (reg->Tensao_Regulacao < (1. - reg_max_pu))
				reg->Tensao_Regulacao= 1.0 - reg_max_pu;
			else
				reg->Tensao_Regulacao = 1.0 + reg_max_pu;
		}
		// preenche estrutura strREGULADOR
		str.obra = false;
		str.id = reg->Id;
		str.rede_id = reg->Id_Rede;
		// define Barras do Regulador a partir das Barras do Trecho
		str.bar1_id = barra1->Id;
		str.bar2_id = barra2->Id;
		str.modelo = (reg->Tipo_Unidade->Id == 13) ? reguENERQCT : reguAUTO_BI;
		str.codigo = reg->Codigo;
		str.ligacao = ConverteTipoLigacao(reg->Tipo_Regulador->Codigo);
		str.fases = fases->ExisteFase(reg->Fases_Con->Codigo);
		str.snom_mva = (reg->Tipo_S_Nom->Pot_Aparente_kVA) / 1000.; // kVA =>MVA
		str.rel_tap = reg->RTP;
		str.var_tensao = reg_max_percent;
		str.z0.r = 0.0;
		str.z0.x = 0.01;
		str.z1.r = (reg->R)/100.;
		str.z1.x = (reg->X_Pri_Sec)/100.;
		//
		str.cfg_auto.bar_ref_id = str.bar1_id;
		str.cfg_auto.v_pu = reg->Tensao_Regulacao;
		str.cfg_auto.bar_ref_inv_id = str.bar2_id;
		str.cfg_auto.vinv_pu = reg->Tensao_Regulacao;
		str.cfg_fixo.passo = (reg->Tensao_Regulacao - 1) / reg_max_pu;
		// esquemático
		str.esq.posbar1 = 50.;
		str.esq.posbar2 = 50.;
		// insere Regulador em VTMonta
		if (monta->InsereRegulador(str) == NULL)
			return (false);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}


//// ---------------------------------------------------------------------------
//bool __fastcall TPreMonta::InserePri_Trafo(sdi_TRAFO_LINHA &sdi)
//{
//	// variáveis locais
//	strTRAFO str;
//	VTLigacao *ligacao;
//
//	try
//	{ // configura Log
//		if (plog)
//			plog->DefineValor("Eqpto", "Trafo");
//		if (plog)
//			plog->DefineValor("Código", sdi.codigo);
//		// verifica se o Regulador pertence ao Circuito selecionado
//		if ((circuito == NULL) || (circuito->id != sdi.circ_id))
//		{
//			if (plog)
//				plog->Add("Não está associado a rede primária");
//			return (true);
//		}
//		// determina Trecho correspondente ao Tafo
//		if ((ligacao = circuito->rede->ExisteLigacao(eqptoTRECHO, sdi.trecho_id)) == NULL)
//		{
//			if (plog)
//				plog->Add("Não está associado a um trecho");
//			return (true);
//		}
//		// verifica potência nominal
//		if (IsDoubleZero(sdi.snom_kva))
//		{
//			if (plog)
//				plog->Add("Potência nominal nula");
//			sdi.snom_kva = 5000;
//		}
//		// verifica tensão do primário
//		if (IsDoubleZero(sdi.valta_kv))
//		{
//			if (plog)
//				plog->Add("Tensão nominal primária nula");
//			sdi.valta_kv = circuito->vnom_kv;
//		}
//		else if (!IsDoubleZero(sdi.valta_kv - circuito->vnom_kv))
//		{
//			if (plog)
//				plog->Add("Tensão nominal primária diferente do circuito");
//			sdi.valta_kv = circuito->vnom_kv;
//		}
//		// verifica tensão do secundário
//		if (IsDoubleZero(sdi.vbaixa_kv))
//		{
//			if (plog)
//				plog->Add("Tensão nominal secundária nula");
//			sdi.vbaixa_kv = 13.8;
//		}
//		// insere Trafo na Rede
//		str.trf_id = sdi.id;
//		str.rede_id = circuito->rede->Id;
//		str.bar1_id = ligacao->pbarra1->Id;
//		str.bar2_id = ligacao->pbarra2->Id;
//		str.codigo = sdi.codigo;
//		str.extern_id = sdi.extern_id;
//		str.snom_mva = sdi.snom_kva * 0.001;
//		str.z0.r = sdi.r0_pu;
//		str.z0.x = sdi.x0_pu;
//		str.z1.r = sdi.r1_pu;
//		str.z1.x = sdi.x1_pu;
//		str.perda_ferro = (sdi.perda_fe_kw / sdi.snom_kva) * 100.;
//		// primário
//		str.pri.fases = faseABC;
//		str.pri.ligacao = lgTRIANG;
//		str.pri.vnom = sdi.valta_kv;
//		str.pri.tap = sdi.valta_kv;
//		str.pri.defasagem = 0;
//		str.pri.zterra.r = 0;
//		str.pri.zterra.x = 0;
//		// secundário
//		str.sec.fases = faseABCNT;
//		str.sec.ligacao = lgEST_AT;
//		str.sec.vnom = sdi.vbaixa_kv;
//		str.sec.tap = sdi.vbaixa_kv;
//		str.sec.defasagem = -30.;
//		str.sec.zterra.r = 0;
//		str.sec.zterra.x = 0;
//		// dados de LTC
//		str.ltc.tipo = ltcFIXO;
//		str.ltc.bar_id = ID_NULO;
//		str.ltc.v_pu = 1.0;
//		// posição nas barras
//		str.esq.posbar1 = 50.;
//		str.esq.posbar2 = 50.;
//		// retira Ligacao da sua Rede e destrói objeto
//		circuito->rede->RemoveLigacao(ligacao);
//		DLL_DeleteEqpto(ligacao);
//		if (monta->InsereTrafo(str) == NULL)
//			return (false);
//	}
//	catch (Exception &e)
//	{
//		if (plog)
//			plog->Add(ERRO_DE_SISTEMA);
//		return (false);
//	}
//	return (true);
//}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InserePri_Trecho(TTrechoBDGD *trechoBDGD)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	int fases_tag;
	AnsiString codigo;
	strTRECHO str_tre;
	VTArranjo *arranjo;
	VTBarra *barra1, *barra2;
	VTTrecho *trecho;
	bool trechoCurto = false;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Trecho MT");
		if (plog)
			plog->DefineValor("Código", trechoBDGD->Codigo);
		// verifica se o Trecho pertence ao Circuito selecionado
		//if ((pri == NULL) || (pri->id != trechoBDGD->Id_Rede))
		if (pri == NULL)
		{
			if (plog)
				plog->Add("Não está associado a rede primária");
			return (true);
		}
		// verifica se Barras iguais
		if (trechoBDGD->Barra_1 == trechoBDGD->Barra_2)
		{ // não cria trecho com barras iguais
			if (plog)
				plog->Add("Barra inicial igual à barra final");
			return (true);
		}
		barra1 = barra2 = NULL;
		// Insere as barras 1 e 2
		if (trechoBDGD->Barra_1 != NULL)
		{
			if(pri->ExisteBarra(trechoBDGD->Barra_1->Id) == NULL)
			{
				InserePri_Barra(trechoBDGD->Barra_1);
			}
			// verifica se existem as Barras
			barra1 = monta->ExisteBarra(trechoBDGD->Barra_1->Id);
		}
		if (trechoBDGD->Barra_1 != NULL)
		{
			if(pri->ExisteBarra(trechoBDGD->Barra_2->Id) == NULL)
			{
				InserePri_Barra(trechoBDGD->Barra_2);
			}
			// verifica se existem as Barras
			barra2 = monta->ExisteBarra(trechoBDGD->Barra_2->Id);
		}
		if ((barra1 == NULL) && (barra2 == NULL))
		{
			if (plog)
				plog->Add("Não existem as duas barras");
			return (true);
		}
		if ((barra1 != NULL) && (barra2 == NULL))
		{
			if (plog)
				plog->Add("Não existe a barra " + IntToStr(trechoBDGD->Barra_2->Id));
			return (true);
		}
		else if ((barra1 == NULL) && (barra2 != NULL))
		{
			if (plog)
				plog->Add("Não existe a barra " + IntToStr(trechoBDGD->Barra_1->Id));
			return (true);
		}
		// determina fases do Trecho e inclui o neutro
		if (trechoBDGD->Fases_Con == NULL)
		{
			if (plog)
				plog->Add("Fases indefinidas");
			// assume fases ABCN
			trechoBDGD->Fases_Con = commons->GetFasesConexao(1) ;
		}
		fases_tag = fases->ExisteFase(trechoBDGD->Fases_Con->Codigo) | faseN;
		// verifica se existe Arranjo (criando um novo Arranjo se for necessário)
		if ((arranjo = ExisteArranjo(trechoBDGD->Arranjo->Id, fases_tag)) == NULL)
		{
			if (plog)
				plog->Add("Não existe cabo");
			// assume default
			arranjo = arranjoMT;
			// verifica se precisa trocar as fases do arranjoMT
			if (arranjo->Fases != fases_tag)
			{
				arranjo = ArranjoMTcerto(fases_tag);
			}
		}

		if (IsDoubleZero(trechoBDGD->Comprimento_m))
		{ // verifica se comprimento nulo
			trechoCurto = true;
			trechoBDGD->Comprimento_m = 1.;
		}
		else if (trechoBDGD->Comprimento_m < 1.0)
		{ // verifica se comprimento é curto
			trechoCurto = true;
			trechoBDGD->Comprimento_m = 1.;
		}
		// preenche estrutura strTRECHO
		str_tre.tre_id = trechoBDGD->Id;
		str_tre.rede_id = trechoBDGD->Id_Rede;
		str_tre.codigo = trechoBDGD->Codigo;
		str_tre.bar1_id = trechoBDGD->Barra_1->Id;
		str_tre.bar2_id = trechoBDGD->Barra_2->Id;
		str_tre.fases = fases_tag;
		str_tre.comp_km = trechoBDGD->Comprimento_m * 0.001;
		str_tre.esq.posbar1 = 50.;
		str_tre.esq.posbar2 = 50.;
		// insere Trecho com o Arranjo definido
		if ((monta->InsereTrecho(str_tre, barra1, barra2, arranjo)) == NULL)
		{
			return (false);
		}
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereSec_Trafo2E(TTrafoDistribuicao *trafoMTBT)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso;
	strTRAFO str;
	VTTrafo *trafo;
	VTBarra *barra1, *barra2;
	int tipo_liga_pri, tipo_liga_sec, tipo_liga_ter;

	try
	{ // configura Log
		if (plog)
			plog->DefineValor("Eqpto", "Trafo");
		if (plog)
			plog->DefineValor("Código", trafoMTBT->Codigo);
		// verifica tensão do primário
		if (IsDoubleZero(trafoMTBT->Tensao_Pri->Tensao_V))
		{
			if (plog)
				plog->Add("Tensão nominal primária nula");
			// Tensao de 13.8 kV   - Tem ID = 49 na tabela de domínio
			trafoMTBT->Tensao_Pri = commons->GetTensao(49);
		}
		// verifica tensão do secundário
		if (IsDoubleZero(trafoMTBT->Tensao_Sec->Tensao_V))
		{
			if (plog)
				plog->Add("Tensão nominal secundária nula");
			// Tensao de 220v  - Tem ID = 10 na tabela de domínio
			trafoMTBT->Tensao_Sec = commons->GetTensao(10);
		}
		tipo_liga_pri =  tipo_liga_sec = tipo_liga_ter = lgINDEFINIDA;
		// insere Trafo na Rede
		str.trf_id = trafoMTBT->Id;
		str.rede_id = trafoMTBT->Id_Rede_MT;
		barra1 = monta->ExisteBarra(trafoMTBT->Barra_1->Id);
		barra2 = monta->ExisteBarra(trafoMTBT->Barra_2->Id);
		if(barra1 != NULL)
		{
			str.bar1_id = barra1->Id;
		}
		if(barra2 != NULL)
		{
			str.bar2_id = barra2->Id;
		}
		str.codigo = trafoMTBT->Codigo;
		str.extern_id = trafoMTBT->Codigo;
		str.snom_mva = trafoMTBT->S_Nom_kVA * 0.001;
		str.z0.r = (trafoMTBT->R)*0.01;
		str.z0.x = (trafoMTBT->X_Pri_Sec)*0.01;
		str.z1.r = (trafoMTBT->R)*0.01;
		str.z1.x = (trafoMTBT->X_Pri_Sec)*0.01;
		str.perda_ferro = ((trafoMTBT->Perda_Fe_W*0.001)/trafoMTBT->S_Nom_kVA)*100.;
		// LIGACAO
		TraduzTipoLigacaoTrafo(trafoMTBT->Ligacao_Trafo, tipo_liga_pri, tipo_liga_sec, tipo_liga_ter);
		if((trafoMTBT->Tipo_Trafo->Id == 1) || (trafoMTBT->Tipo_Trafo->Id == 4))
		{
		   tipo_liga_pri = tipo_liga_sec = lgMONOFASICO;
		}
		// primário
		str.pri.fases = faseABC; // Default
		str.pri.ligacao = lgTRIANG; // Default
		if(trafoMTBT->Fases_Con_Pri != NULL)
		{
			str.pri.fases = fases->ExisteFase(trafoMTBT->Fases_Con_Pri->Codigo);
		}
		if (fases->NumeroDeFases(str.pri.fases) == 1)
		{
			str.pri.fases = str.pri.fases | faseNT;
		}
		str.pri.ligacao = (tipo_liga_pri == lgINDEFINIDA) ? lgTRIANG: tipo_liga_pri;
		str.pri.vnom = (trafoMTBT->Tensao_Pri->Tensao_V)/1000.;
		str.pri.tap = (trafoMTBT->Tensao_Pri->Tensao_V)/1000.;
		str.pri.defasagem = 0;
		str.pri.zterra.r = 0;
		str.pri.zterra.x = 0;
		// secundário
		str.sec.fases = faseABCNT;
		str.sec.ligacao = (tipo_liga_sec == lgINDEFINIDA) ? lgEST_AT : tipo_liga_sec;
		if(trafoMTBT->Fases_Con_Sec != NULL)
		{
			str.sec.fases = fases->ExisteFase(trafoMTBT->Fases_Con_Pri->Codigo) | faseNT;
		}
		str.sec.vnom = (trafoMTBT->Tensao_Sec->Tensao_V)/1000.;
		str.sec.tap = (trafoMTBT->Tensao_Sec->Tensao_V)/1000.;
		str.sec.defasagem = ((str.sec.fases & faseABC) == faseABC) ? 30. : 0.0;
		str.sec.zterra.r = 0;
		str.sec.zterra.x = 0;
		// dados de LTC
		str.ltc.tipo = ltcFIXO;
		str.ltc.bar_id = trafoMTBT->Barra_2->Id;
		str.ltc.v_pu = 1.0;
		// posição nas barras
		str.esq.posbar1 = 50.;
		str.esq.posbar2 = 50.;
		if ((trafo = (VTTrafo*)monta->InsereTrafo(str)) == NULL)
			return (false);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::InsereSec_Trafo3E(TTrafoDistribuicao *trafoMTBT)
{
	// variáveis locais
	VTCommons* commons =  (VTCommons*)apl->GetObject(__classid(VTCommons));
	bool sucesso;
	strTRAFO3E str;
	VTTrafo3E *trafo3E;
	int tipo_liga_pri, tipo_liga_sec, tipo_liga_ter;

	try
	{ // configura Log
			if (plog)
			plog->DefineValor("Eqpto", "Trafo");
		if (plog)
			plog->DefineValor("Código", trafoMTBT->Codigo);
			// verifica tensão do primário
		if (IsDoubleZero(trafoMTBT->Tensao_Pri->Tensao_V))
		{
			if (plog)
				plog->Add("Tensão nominal primária nula");
			// Tensao de 13.8 kV   - Tem ID = 49 na tabela de domínio
			trafoMTBT->Tensao_Pri = commons->GetTensao(49);
		}
		// verifica tensão do secundário
		if (IsDoubleZero(trafoMTBT->Tensao_Sec->Tensao_V))
		{
			if (plog)
				plog->Add("Tensão nominal secundária nula");
			// Tensao de 220v  - Tem ID = 10 na tabela de domínio
			trafoMTBT->Tensao_Sec = commons->GetTensao(10);
		}
		// verifica tensão do terciário, caso tenha sido definida a Barra
		if (trafoMTBT->Barra_3 != NULL)
		{
			if (IsDoubleZero(trafoMTBT->Tensao_Ter->Tensao_V))
			{
				if (plog)
					plog->Add("Tensão nominal terciária nula");
				trafoMTBT->Tensao_Ter = trafoMTBT->Tensao_Sec;
			}
		}
		tipo_liga_pri =  tipo_liga_sec = tipo_liga_ter = lgINDEFINIDA;
		// insere Trafo na Rede
		str.trf_id = trafoMTBT->Id;
		str.rede_id = trafoMTBT->Id_Rede_MT;
		str.bar1_id = trafoMTBT->Barra_1->Id;
		str.bar2_id = trafoMTBT->Barra_2->Id;
		str.codigo = trafoMTBT->Codigo;
		str.extern_id = trafoMTBT->Codigo;
		// impedâncias DEFAULT
		str.zps1.r = str.zpt1.r = str.zst1.r = (trafoMTBT->R)/100.;
		str.zps1.x = (trafoMTBT->X_Pri_Sec)/100.;
		str.zpt1.x = (trafoMTBT->X_Pri_Ter)/100.;
		str.zst1.x = (trafoMTBT->X_Sec_Ter)/100.;
		// Coloca os mesmos valores na seq zero
		str.zps0 = str.zpt0 = str.zst0 = str.zps1;
		str.perda_ferro = ((trafoMTBT->Perda_Fe_W*0.001)/trafoMTBT->S_Nom_kVA)*100.;
		// LIGACAO
		TraduzTipoLigacaoTrafo(trafoMTBT->Ligacao_Trafo, tipo_liga_pri, tipo_liga_sec, tipo_liga_ter);
		// primário
		str.pri.fases = faseABC; // Default
		str.pri.ligacao = lgTRIANG; // Default
		if(trafoMTBT->Fases_Con_Pri != NULL)
		{
			str.pri.fases = fases->ExisteFase(trafoMTBT->Fases_Con_Pri->Codigo);
		}
		if (fases->NumeroDeFases(str.pri.fases) == 1)
		{
			str.pri.fases = str.pri.fases | faseNT;
		}
		str.pri.ligacao = (tipo_liga_pri == lgINDEFINIDA) ? lgTRIANG: tipo_liga_pri;
		str.pri.snom = trafoMTBT->S_Nom_kVA * 0.001;
		str.pri.vnom = (trafoMTBT->Tensao_Pri->Tensao_V)/1000.;
		str.pri.tap = (trafoMTBT->Tensao_Pri->Tensao_V)/1000.;
		str.pri.defasagem = 0;
		str.pri.zterra.r = 0;
		str.pri.zterra.x = 0;
		// secundário
		str.sec.fases = faseABCNT;
		str.sec.ligacao = (tipo_liga_sec == lgINDEFINIDA) ? lgEST_AT : tipo_liga_sec;
		if(trafoMTBT->Fases_Con_Sec != NULL)
		{
			str.sec.fases = fases->ExisteFase(trafoMTBT->Fases_Con_Sec->Codigo) | faseNT;
		}
		str.sec.vnom = (trafoMTBT->Tensao_Sec->Tensao_V)/1000.;
		str.sec.snom = str.pri.snom * 0.5;
		str.sec.tap = (trafoMTBT->Tensao_Sec->Tensao_V)/1000.;
		str.sec.defasagem = ((str.sec.fases & faseABC) == faseABC) ? 30. : 0.0;
		str.sec.zterra.r = 0;
		str.sec.zterra.x = 0;
		// terciario
		str.ter.fases = faseABCNT;
		str.ter.ligacao = (tipo_liga_ter == lgINDEFINIDA) ? lgEST_AT : tipo_liga_ter;
		if(trafoMTBT->Fases_Con_Ter!= NULL)
		{
			str.ter.fases = fases->ExisteFase(trafoMTBT->Fases_Con_Ter->Codigo) | faseNT;
		}
		str.ter.vnom = (trafoMTBT->Tensao_Ter->Tensao_V)/1000.;
		str.ter.snom = str.pri.snom * 0.5;
		str.ter.tap = (trafoMTBT->Tensao_Ter->Tensao_V)/1000.;
		str.ter.defasagem = ((str.ter.fases & faseABC) == faseABC) ? 30.0:0.0;
		str.ter.zterra.r = 0;
		str.ter.zterra.x = 0;
		// dados de LTC
		str.ltc.tipo = ltcFIXO;
		// posição nas barras
		str.esq.posbar1 = 50.;
		str.esq.posbar2 = 50.;
		str.esq.posbar3 = 50.;
		if ((trafo3E = (VTTrafo3E*)monta->InsereTrafo3E(str)) == NULL)
			return (false);
		// salva Trafo3E na Se
		se->InsereTrafo3E(trafo3E);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::IsBarraIsolada(VTBarra *barra)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisLig = new TList;
	bool isolada = false;

	try
	{
		// protecao
		if (redes == NULL)
		{
			return false;
		}
		redes->LisLigacao(lisLig, barra);
		if (lisLig->Count == 0)
		{
			isolada = true;
		}
	}
	catch (...)
	{
		// nao sei haha
	}
	DestroiTObject(lisLig);
	return isolada;
}

// ---------------------------------------------------------------------------
void __fastcall TPreMonta::RemoveTrechosParalelo(void)
{
	VTLigacao *ligacao, *ligacaoAux;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisLig;
	VTRede *rede;
	TList *lisLigComum;
	VTBarra *barra1, *barra2, *barraAux1, *barraAux2;

	try
	{ // cria listas
		lisLig = new TList;
		lisLigComum = new TList;
		for (int nr = 0; nr < redes->LisRede()->Count; nr++)
		{
			rede = (VTRede*)redes->LisRede()->Items[nr];
			// limpa lista
			lisLig->Clear();
			rede->LisLigacao(lisLig, eqptoTRECHO);
			// percorre todas ligacoes
			for (int nl = (lisLig->Count - 1); nl >= 0; nl--)
			{
				ligacao = (VTLigacao*) lisLig->Items[nl];
				if (ligacao->Tipo() != eqptoTRECHO)
				{
					continue;
				}
				barra1 = ligacao->pbarra1;
				barra2 = ligacao->pbarra2;
				// compara essa ligacao com todas as outras
				for (int nla = (nl - 1); nla >= 0; nla--)
				{
					ligacaoAux = (VTLigacao*) lisLig->Items[nla];
					if (ligacao == ligacaoAux)
					{
						continue;
					}
					barraAux1 = ligacaoAux->pbarra1;
					barraAux2 = ligacaoAux->pbarra2;
					// verifica se as barras são iguais
					if ((barra1 == barraAux1) && (barra2 == barraAux2))
					{
						rede->RemoveLigacao(ligacao);
						lisLig->Remove(ligacao);
					}
					else if ((barra1 == barraAux2) && (barra2 == barraAux1))
					{
						rede->RemoveLigacao(ligacao);
						lisLig->Remove(ligacao);
					}
				}
			}
		}
		// destroi listas
		DestroiTObject(lisLig);
		DestroiTObject(lisLigComum);
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::Sucesso(void)
{
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::TraduzTipoLigacaoTrafo(TLigacaoTrafo *lig_trafo, int tipo_liga_pri, int tipo_liga_sec, int tipo_liga_terc)
{
	// a ligação do terciario é sempre indefinida a não ser que seja tipo 5 ou 10
	tipo_liga_terc = lgINDEFINIDA;

	switch (lig_trafo->Id)
		{
		case 1:
			tipo_liga_pri = lgEST_ISO;
			tipo_liga_sec = lgEST_AT;
			break;
		case 2:
			tipo_liga_pri = lgTRIANG;
			tipo_liga_sec = lgEST_AT;
			break;
		case 4:
			tipo_liga_pri = lgEST_AT;
			tipo_liga_sec = lgEST_AT;
			break;
		case 5:
			tipo_liga_pri = lgTRIANG;
			tipo_liga_sec = lgEST_ISO;
			tipo_liga_terc = lgEST_AT;
			break;
		case 7:
			tipo_liga_pri = lgTRIANG;
			tipo_liga_sec = lgTRIANG;
			break;
//		case 8:
//		case 9:
//			tipo_liga_pri = lgMONOFASICO;
//			tipo_liga_sec = lgMONOFASICO;
//			break;
		case 10:
			tipo_liga_pri = lgEST_ISO;
			tipo_liga_sec = lgEST_AT;
			tipo_liga_terc = lgTRIANG;
			break;
		case 11:
			tipo_liga_pri = lgEST_ISO;
			tipo_liga_sec = lgTRIANG;
			break;
		default:
			tipo_liga_pri = lgINDEFINIDA;
			tipo_liga_sec = lgINDEFINIDA;
			tipo_liga_terc = lgINDEFINIDA;
            break;
		}
return(true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::TrataInicioDeDados(void)
{
	try
	{ // reinicia objetos locais
		se = NULL;
		// destrói objetos Se
		LimpaTList(lisSE);
		// inicia listas
		lisEQP->Clear();
		// se ainda não existe, cria um Dominio
		IniciaDominio();
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TPreMonta::TrataFimDeDados(void)
{
	try
	{ // cria disjuntor no início de cada primário
		// PENDENTE DVC
		//InserePri_DisjuntorFicticio();
		// remove os trechos em paralelo
		RemoveTrechosParalelo();
		// Insere suprimento nos alimentadores sem trafo
		TrataSESemTrafo();
		// reinicia objetos locais
		se = NULL;
		// destrói objetos Se
		LimpaTList(lisSE);
		// executa tratamento final do MontaRede
		monta->TrataFimDeDados();
	}
	catch (Exception &e)
	{
		if (plog)
			plog->Add(ERRO_DE_SISTEMA);
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
void __fastcall TPreMonta::TrataSESemTrafo(void)
{ // variáveis locais
	TSe *se;
	TPri *pri;
	VTBarra *barraIni;

	// procura TSe
	for (int n = 0; n < lisSE->Count; n++)
	{
		se = (TSe*)lisSE->Items[n];
		// verifica se a se possui trafos
		if ((se->lisTRF->Count == 0) && (se->lisTRF3E->Count == 0))
		{
			// percorre todos circuitos
			for (int nc = 0; nc < se->lisPRI->Count; nc++)
			{
				pri = (TPri*)se->lisPRI->Items[nc];
				barraIni = pri->rede->BarraInicial();
				if (barraIni == NULL)
				{
					continue;
				}
				// se nao existir suprimento
				if (!barraIni->ExisteGeracao())
				{
					InsereEtd_Suprimento(barraIni);
				}
			}
		}
	}
}

// -----------------------------------------------------------------------------
// eof
