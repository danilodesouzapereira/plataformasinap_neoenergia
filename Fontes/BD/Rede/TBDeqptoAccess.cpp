// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <map>
#include <DateUtils.hpp>
#include "TBDcurvaAccess.h"
#include "TBDeqptoAccess.h"
#include "TBDfunc.h"
#include "VTBDeqpto.h"
#include "VTBDgeral.h"
#include "VTSNPDBConn.h"
#include "SNPGlobal.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Cronometro\VTCronometro.h"
#include "..\Diretorio\VTPath.h"
#include "..\Edita\VTEdita.h"
#include "..\Log\VTLog.h"
#include "..\Manobra\VTLock.h"
#include "..\Manobra\VTLocked.h"
#include "..\Manobra\VTLocks.h"
#include "..\Manobra\VTManobra.h"
#include "..\Manobra\VTManobras.h"
#include "..\Obra\VTAcao.h"
#include "..\Obra\VTLixeira.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include "..\Obra\ItemObra\VTItemObra.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Planejamento\VTAlternativa.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTBateria.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCapSerie.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTCargaNL.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTDadosExt.h"
#include "..\Rede\VTDominio.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTFiltro.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTMatY.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTComentario.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTMunicipio.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafoZZ.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTYref.h"
#include "..\SinapApl\VTBasico.h"
#include "..\..\DLL_Inc\Cronometro.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

#include <locale.h>

#include <cstdarg>

////---------------------------------------------------------------------------
// VTBDeqpto* NewObjBDeqptoAccess(VTApl *apl_owner, VTSNPDBConn *bd_conn)
// {
// return(new TBDeqptoAccess(apl_owner, bd_conn));
// }

// ---------------------------------------------------------------------------
__fastcall TBDeqptoAccess::TBDeqptoAccess(VTApl *apl_owner, VTSNPDBConn *bd_conn)
{
	AnsiString strlog;

	// salva ponteiro
	this->apl = apl_owner;
	this->bd_conn = bd_conn;
	// inicia ponteiros de uso geral, buscando no Apl
	plog = (VTLog*)apl->GetObject(__classid(VTLog));
	bd_curva = new TBDcurvaAccess(apl_owner, bd_conn);
	// Carrega ponteiros previamente criados
	if ((path = (VTPath*)apl->GetObject(__classid(VTPath))) == NULL)
	{
		MSGERRSYS_PATHINV
	}
	if ((bd_func = (TBDfunc*)apl->GetObject(__classid(TBDfunc))) == NULL)
	{
		MSGERRSYS_BDFUNCINV SNPSTRLOGLINE(strlog);
	}
	if ((monta = (VTMonta*)apl->GetObject(__classid(VTMonta))) == NULL)
	{
		MSGERRSYS_MONTAINV SNPSTRLOGLINE(strlog);
	}
	if ((patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares))) == NULL)
	{
		MSGERRSYS_PATAMARESINV SNPSTRLOGLINE(strlog);
	}
	if ((basico = (VTBasico*)apl->GetObject(__classid(VTBasico))) == NULL)
	{
		MSGERRSYS_PONTINV SNPSTRLOGLINE(strlog);
	}
	// progresso pode ser NULL
	progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
	// Nesse ponto ainda pode não ter sido criado o objeto redes
	redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// para habilitar medição de tempos basta tirar o comentário as duas linhas abaixo
	// cronometro = (VTCronometro*)apl->GetObject(__classid(VTCronometro));
	// if (cronometro == NULL) cronometro = DLL_NewObjCronometro(apl);

	// define o estado inicial dos indexadores
	index.ndxAgrupamentoId = 0;
	index.ndxDominioId = 0;
	index.ndxEquipId = 0;
	index.ndxEstudoId = 0;
	index.ndxAlternativaId = 0;
	index.ndxObraId = 0;
	index.ndxAcaoId = 0;
	index.ndxCorteId = 0;
	index.ndxManobraId = 0;
	index.ndxSincronismoId = 0;

	// Lista de dados a serem salvos em modo batch
	lstStrData = new TStringList();
	// Evita registros duplicados.
	lstStrData->Sorted = true;
	lstStrData->Duplicates = Classes::dupIgnore;
	// Efetua a cópia do arquivo Schema.ini
	CopiaSchemaIni();
}

// ---------------------------------------------------------------------------
__fastcall TBDeqptoAccess::~TBDeqptoAccess(void)
{
	// Elimina os objetos criados localmente
	if (bd_curva)
	{
		delete bd_curva;
		bd_curva = NULL;
	}
	if (lstStrData)
	{
		lstStrData->Clear();
		delete lstStrData;
		lstStrData = NULL;
	}
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
bool __fastcall TBDeqptoAccess::CarregaEstudo(VTEstudo *estudo)
{
	AnsiString strlog;

	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}
	if (!monta)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_MONTAINV return (false);
	}

    reservado = false;
	if (progresso)
	{
		progresso->Start(progTEXTO);
		progresso->NoAutoInc();
		reservado = progresso->MaxStep(MAX_STEP_CARREGA_ESTUDO);
	}

	// inicia controle de inserção de Barras de VTMOnta
	// monta->InsereBarraStart();
	// carrega todos eqptos da rede do Estudo
	if (!CarregaRedeEstudo(estudo))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRBDRD_ESTUDO return (false);
	}
	if (!LeListaCorte(estudo, redes->LisMRede()))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRBDRD_ESTUDO return (false);
	}
	if (!CarregaReducaoEstudo(estudo))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRBDRD_ESTUDO return (false);
	}

	if (estudo->Id > 0)
	{
		if (!CarregaAlternativaEstudo(estudo))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_ESTUDOALTERNATIVA return (false);
		}
	}

	// finaliza montagem da rede
	if (!monta->TrataFimDeDados(true))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_TRATAFIMDADOS return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
bool __fastcall TBDeqptoAccess::CarregaMRedeEstudo(VTEstudo *estudo, TList *lisMREDE,
	TList *lisMREDE_VIZ)
{
	// variáveis locais
	AnsiString strlog;
	int rede_id;
	TList* lisREDE;
	VTMRede* mrede;
	VTRede* rede;

	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisMREDE)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISMREDEINV return (false);
	}
	if (!lisMREDE_VIZ)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISMREDEINV return (false);
	}
	if (!monta)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_MONTAINV return (false);
	}

	// exibe progresso
    reservado = false;
	if (progresso)
	{
		progresso->Start(progTEXTO);
		progresso->NoAutoInc();
		reservado = progresso->MaxStep(MAX_STEP_CARREGA_MREDE_ESTUDO);
        progresso->Add("Leitura das redes");
	}

	// cria lista temporária
	lisREDE = new TList();
	// inicia controle de inserção de Barras de VTMOnta
	// monta->InsereBarraStart();
	// lê as Redes de todas MRede (apenas as Redes, sem seus Eqptos)
	for (int n = 0; n < lisMREDE->Count; n++)
	{
		mrede = (VTMRede*)lisMREDE->Items[n];
		if (!SPSNP_rdRedeMRede(estudo, mrede->Id, lisREDE))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_REDEMREDE return (false);
		}
	}

	if (reservado)
		progresso->Step();
	// IMPORTANTE: as chamadas abaixo estão em BDihm e não podem sair de lá
	// lê todos os Cortes da MRede selecionadas
	// if (! LeListaCorte(estudo, lisMREDE)) return(false);
	// lê todos os Cortes da MRede vizinhas
	// if (! LeListaCorte(estudo, lisMREDE_VIZ)) return(false);
	// carrega Eqptos das Redes lidas
	CarregaRedeEstudo(estudo, lisREDE);
	// carrega Reducao das MRedes vizinhas
	CarregaReducaoEstudo(estudo, lisMREDE_VIZ);
	// destrói lista sem destruir seus objetos
	delete lisREDE;
	// finaliza montagem da rede
	if (!monta->TrataFimDeDados(true))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_TRATAFIMDADOS return (false);
	}

	if (reservado)
		progresso->Step();
	// fecha janela de  progresso
	if (progresso)
		progresso->Stop();

	return (true);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
bool __fastcall TBDeqptoAccess::CarregaAlternativaEstudo(VTEstudo *estudo)
{
	bool sucesso = true;
	VTRede* rede;
	AnsiString strlog;
	AnsiString straux;

	// proteções
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}
	// reinicia cronometros
	if (cronometro)
		cronometro->Reinicia();
	if (cronometro)
		cronometro->Start("RedeTotal");

	// exibe barra de progresso
//	if (progresso)
//		progresso->Start(progTEXTO);
	if (progresso)
		progresso->Add("Leitura da base de dados");
	// barra
	if (sucesso)
	{
		if (cronometro)
			cronometro->Start("Barra");
		if (progresso)
			progresso->Add("Leitura das barras");
		sucesso = SPSNP_rdBarra(estudo, false, true);

		if (reservado)
            progresso->Step();
	if (cronometro)
			cronometro->Stop("Barra");
		if (!sucesso)
		{
			SNPSTRLOGLINE(strlog);
		}
	}
	// redes
	if (sucesso)
	{
		if (cronometro)
			cronometro->Start("Rede");
		if (progresso)
			progresso->Add("Leitura das redes");
		sucesso = SPSNP_rdRede(estudo, IDINDEFINIDO, true);

		if (reservado)
            progresso->Step();
		if (cronometro)
			cronometro->Stop("Rede");
		if (!sucesso)
		{
			SNPSTRLOGLINE(strlog);
			if (progresso)
				progresso->Stop();
			MSGERRBDRD_REDE return (false);
		}
	}

	if (sucesso)
	{
		// Carga de todos os equipamentos do estudo, independentemente da rede
		sucesso = CarregaEqptosRede(estudo, false, true);
	}

    if (reservado)
        progresso->Step();

	if (sucesso)
	{ // executa montagem final das Redes
		if (cronometro)
			cronometro->Start("Alternativas");
		if (progresso)
			progresso->Add("Leitura das alternativas");

		if (!SPSNP_rdEstudoAlternativa(estudo))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_ESTUDOALTERNATIVA sucesso = false;
		}

		if (reservado)
            progresso->Step();
		if (cronometro)
			cronometro->Stop("Alternativas");
	}
	if (sucesso)
	{
		if (cronometro)
			cronometro->Start("Atributos das Obras");
		if (progresso)
			progresso->Add("Leitura dos Atributos das Obras");
		// Esse método é implementado em cada Classe - criada dependendo da base...
		if (!SPSNP_rdEstudoObraAtribExt(estudo))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_ESTUDOALTERNATIVA sucesso = false;
		}

		if (reservado)
            progresso->Step();
	}
	if (cronometro)
		cronometro->Stop("Atributos das Obras");

	// finaliza cornometro total
	if (cronometro)
		cronometro->Stop("RedeTotal");
	// exibe cronometro
	if (cronometro)
		cronometro->Show();
	if (progresso)
		progresso->Stop();
	// retorna resultado
	return (sucesso);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
//
// bolUseRedeTmp = true : utiliza a tabela auxiliar. A função que antecede essa chamada tem que ter
// preparado a tabela TmpEquip corretamente
// bolUseRedeTmp = false: Executa a consulta independentemente de rede
bool __fastcall TBDeqptoAccess::CarregaEqptosRede(VTEstudo *estudo, bool bolUseRedeTmp,
	bool bolObra)
{
	// variáveis locais
	bool sucesso;
	AnsiString strlog;
	AnsiString straux;

	// trecho
	if (progresso)
		progresso->Add("Leitura dos trechos");
	if (cronometro)
		cronometro->Start("Trecho");
	if ((sucesso = SPSNP_rdTrecho(estudo, bolUseRedeTmp, bolObra)) == false)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRBDRD_REDE
	}

	if (reservado)
		progresso->Step();
	if (cronometro)
		cronometro->Stop("Trecho");
	if (sucesso)
	{ // trafo
		if (progresso)
			progresso->Add("Leitura dos trafos");
		if (cronometro)
			cronometro->Start("Trafo");
		if ((sucesso = SPSNP_rdTrafo(estudo, bolUseRedeTmp, bolObra)) == false)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_REDE
		}

		if (reservado)
			progresso->Step();
		if (cronometro)
			cronometro->Stop("Trafo");
	}
	if (sucesso)
	{ // trafo3e
		if (progresso)
			progresso->Add("Leitura dos trafos 3 enrolamentos");
		if (cronometro)
			cronometro->Start("Trafo3E");
		if ((sucesso = SPSNP_rdTrafo3E(estudo, bolUseRedeTmp, bolObra)) == false)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_REDE
		}

		if (reservado)
			progresso->Step();
		if (cronometro)
			cronometro->Stop("Trafo3E");
	}
	if (sucesso)
	{ // bateria
		if (progresso)
			progresso->Add("Leitura das baterias");
		if (cronometro)
			cronometro->Start("Bateria");
		if ((sucesso = SPSNP_rdBateria(estudo, bolUseRedeTmp, bolObra)) == false)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_REDE
		}

		if (reservado)
			progresso->Step();
		if (cronometro)
			cronometro->Stop("Bateria");
	}
	if (sucesso)
	{ // carga
		if (progresso)
			progresso->Add("Leitura das cargas");
		if (cronometro)
			cronometro->Start("Carga");
		if ((sucesso = SPSNP_rdCarga(estudo, bolUseRedeTmp, bolObra)) == false)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_REDE
		}

		if (reservado)
			progresso->Step();
		if (cronometro)
			cronometro->Stop("Carga");
	}
	if (sucesso)
	{ // carga IP
		if (progresso)
			progresso->Add("Leitura das cargas de iluminação pública");
		if (cronometro)
			cronometro->Start("CargaIP");
		if ((sucesso = SPSNP_rdCargaIP(estudo, bolUseRedeTmp, bolObra)) == false)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_REDE
		}

		if (reservado)
			progresso->Step();
		if (cronometro)
			cronometro->Stop("CargaIP");
	}
	if (sucesso)
	{ // capacitor
		if (progresso)
			progresso->Add("Leitura dos capacitores");
		if (cronometro)
			cronometro->Start("Capacitor");
		if ((sucesso = SPSNP_rdCapacitor(estudo, bolUseRedeTmp, bolObra)) == false)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_REDE
		}

		if (reservado)
			progresso->Step();
		if (cronometro)
			cronometro->Stop("Capacitor");
	}
	if (sucesso)
	{ // gerador
		if (progresso)
			progresso->Add("Leitura dos geradores");
		if (cronometro)
			cronometro->Start("Gerador");
		if ((sucesso = SPSNP_rdGerador(estudo, bolUseRedeTmp, bolObra)) == false)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_REDE
		}

		if (reservado)
			progresso->Step();
		if (cronometro)
			cronometro->Stop("Gerador");
	}
	if (sucesso)
	{ // suprimento
		if (progresso)
			progresso->Add("Leitura dos suprimentos");
		if (cronometro)
			cronometro->Start("Suprimento");
		if ((sucesso = SPSNP_rdSuprimento(estudo, bolUseRedeTmp, bolObra)) == false)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_REDE
		}

		if (reservado)
			progresso->Step();
		if (cronometro)
			cronometro->Stop("Suprimento");
	}
	if (sucesso)
	{ // filtro
		if (progresso)
			progresso->Add("Leitura dos filtros");
		if (cronometro)
			cronometro->Start("Filtro");
		if ((sucesso = SPSNP_rdFiltro(estudo, bolUseRedeTmp, bolObra)) == false)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_REDE
		}

		if (reservado)
			progresso->Step();
		if (cronometro)
			cronometro->Stop("Filtro");
	}
	if (sucesso)
	{ // reator
		if (progresso)
			progresso->Add("Leitura dos reatores");
		if (cronometro)
			cronometro->Start("Reator");
		if ((sucesso = SPSNP_rdReator(estudo, bolUseRedeTmp, bolObra)) == false)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_REDE
		}

		if (reservado)
			progresso->Step();
		if (cronometro)
			cronometro->Stop("Reator");
	}
	if (sucesso)
	{ // trafoZZ
		if (progresso)
			progresso->Add("Leitura dos trafos zig zag");
		if (cronometro)
			cronometro->Start("TrafoZZ");
		if ((sucesso = SPSNP_rdTrafoZZ(estudo, bolUseRedeTmp, bolObra)) == false)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_REDE
		}

		if (reservado)
			progresso->Step();
		if (cronometro)
			cronometro->Stop("TrafoZZ");
	}
	if (sucesso)
	{ // capacitor serir
		if (progresso)
			progresso->Add("Leitura dos capacitores série");
		if (cronometro)
			cronometro->Start("CapacitorSerie");
		if ((sucesso = SPSNP_rdCapacitorSerie(estudo, bolUseRedeTmp, bolObra)) == false)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_REDE
		}

		if (reservado)
			progresso->Step();
		if (cronometro)
			cronometro->Stop("CapacitorSerie");
	}
	if (sucesso)
	{ // mutua
		if (progresso)
			progresso->Add("Leitura das mútuas");
		if (cronometro)
			cronometro->Start("Mutua");
		if ((sucesso = SPSNP_rdMutua(estudo, bolUseRedeTmp, bolObra)) == false)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_REDE
		}

		if (reservado)
			progresso->Step();
		if (cronometro)
			cronometro->Stop("Mutua");
	}
	if (sucesso)
	{ // chave
		if (progresso)
			progresso->Add("Leitura das chaves");
		if (cronometro)
			cronometro->Start("Chave");
		if ((sucesso = SPSNP_rdChave(estudo, bolUseRedeTmp, bolObra)) == false)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_REDE
		}

		if (reservado)
			progresso->Step();
		if (cronometro)
			cronometro->Stop("Chave");
	}
	if (sucesso)
	{ // regulador
		if (progresso)
			progresso->Add("Leitura dos reguladores");
		if (cronometro)
			cronometro->Start("Regulador");
		if ((sucesso = SPSNP_rdRegulador(estudo, bolUseRedeTmp, bolObra)) == false)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_REDE
		}

		if (reservado)
			progresso->Step();
		if (cronometro)
			cronometro->Stop("Regulador");
		if (sucesso)
		{ // medidor
			if (progresso)
				progresso->Add("Leitura dos medidores");
			if (cronometro)
				cronometro->Start("Medidor");
			if ((sucesso = SPSNP_rdMedidor(estudo, bolUseRedeTmp, bolObra)) == false)
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBDRD_REDE
			}

			if (reservado)
				progresso->Step();
			if (cronometro)
				cronometro->Stop("Medidor");
		}
		if (sucesso)
		{ // canal
			if (progresso)
				progresso->Add("Leitura dos canais");
			if (cronometro)
				cronometro->Start("Canal");
			if ((sucesso = SPSNP_rdCanal(estudo, bolUseRedeTmp, bolObra)) == false)
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBDRD_REDE
			}

			if (reservado)
				progresso->Step();
			if (cronometro)
				cronometro->Stop("Canal");
		}
	}

	return (true);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
bool __fastcall TBDeqptoAccess::CarregaRedeEstudo(VTEstudo *estudo)
{
	bool sucesso = true;
	VTRede* rede;
	TList* lisREDE = 0;
	AnsiString strlog;
	AnsiString straux;
	int intEtapa = 0;
	int intEtapaTotal = 22;

	// proteções
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}

	// reinicia cronometros
	if (cronometro)
		cronometro->Reinicia();
	if (cronometro)
		cronometro->Start("RedeTotal");
	if (sucesso)
	{ // rede
		if (cronometro)
			cronometro->Start("Rede");
		sucesso = SPSNP_rdRede(estudo);
		if (cronometro)
			cronometro->Stop("Rede");
		if (!sucesso)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_REDE return (false);
		}
	}

	if (reservado)
		progresso->Step();

	//lê municipios
	if (cronometro)
		cronometro->Start("Municipio");
	if (progresso)
		progresso->Add("Municipio");
	if ((sucesso = SPSNP_rdMunicipio()) == false)
	{
		SNPSTRLOGLINE(strlog);
	}

	if (reservado)
		progresso->Step();
	if (cronometro)
		cronometro->Stop("Municipio");

	// Preenche a tabela temporária
	lisREDE = redes->LisRede();
	if (!SPSNP_wrTmpEquip(lisREDE))
	{
		SNPSTRLOGLINE(strlog);
	}
	if (sucesso)
	{ // barras
		if (cronometro)
			cronometro->Start("Barra");
		if (progresso)
			progresso->Add("Leitura das barras");
		sucesso = SPSNP_rdBarra(estudo);

        if (reservado)
            progresso->Step();
		if (cronometro)
			cronometro->Stop("Barra");
		if (!sucesso)
		{
			SNPSTRLOGLINE(strlog);
		}
		// JCG 2016.01.27
		// monta->InsereBarraStop();
	}

	// lê todos Eqptos da Rede
	if (sucesso)
	{
		if ((sucesso = CarregaEqptosRede(estudo, true, false)) == false)
		{
			SNPSTRLOGLINE(strlog);
		}
	}

	if (reservado)
		progresso->Step();

	// Acerta o status das redes
	if (sucesso)
	{
		for (int i = 0; (sucesso) && (i < lisREDE->Count); i++)
		{
			rede = (VTRede*)lisREDE->Items[i];
			// atualiza indicação de que a rede real foi carregada
			// rede->Valida = true;
			rede->MRede->Carregada = true;
		}
	}

	if (reservado)
		progresso->Step();

	// Leitura dos sincronismos e manobras
	if (sucesso)
	{ // Sincronismos (Locks)
		if (cronometro)
			cronometro->Start("Sincronismo");
		if (progresso)
			progresso->Add("Leitura dos Sincronismos entre chaves");
		sucesso = SPSNP_rdSincronismo();

        if (reservado)
            progresso->Step();
		if (cronometro)
			cronometro->Stop("Sincronismo");
		if (!sucesso)
		{
			SNPSTRLOGLINE(strlog);
		}
	}
	if (sucesso)
	{ // Manobras
		if (cronometro)
			cronometro->Start("Manobra");
		if (progresso)
			progresso->Add("Leitura das manobras");
		sucesso = SPSNP_rdManobra();

        if (reservado)
            progresso->Step();
		if (cronometro)
			cronometro->Stop("Manobra");
		if (!sucesso)
		{
			SNPSTRLOGLINE(strlog);
		}
	}
	if (sucesso)
	{ // executa montagem final das Redes
		if (cronometro)
			cronometro->Start("Montagem");
		if (progresso)
			progresso->Add("Montagem das redes");
		// sucesso = monta->TrataFimDeDados(true);

        if (reservado)
            progresso->Step();
		if (cronometro)
			cronometro->Stop("Montagem");
	}
	// finaliza cornometro total
	if (cronometro)
		cronometro->Stop("RedeTotal");
	// exibe cronometro
	if (cronometro)
		cronometro->Show();
	// retorna resultado
	return (sucesso);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TBDeqptoAccess::CarregaReducaoEstudo(VTEstudo *estudo)
 {
 //variáveis locais
 AnsiString strlog;

 //proteções
 if (!estudo)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_ESTUDOINV     return(false); }
 if (!SPSNP_rdRRBarra(estudo))      { SNPSTRLOGLINE(strlog); MSGERRBDRD_RRBARRA      return(false); }
 if (!SPSNP_rdRRCarga(estudo))      { SNPSTRLOGLINE(strlog); MSGERRBDRD_RRCARGA      return(false); }
 if (!SPSNP_rdRRSuprimento(estudo)) { SNPSTRLOGLINE(strlog); MSGERRBDRD_RRSUPRIMENTO return(false); }
 if (!SPSNP_rdRRTrafo(estudo))      { SNPSTRLOGLINE(strlog); MSGERRBDRD_RRTRAFO      return(false); }
 if (!SPSNP_rdRRYRef(estudo))       { SNPSTRLOGLINE(strlog); MSGERRBDRD_RRYREF       return(false); }

 return(true);
 }
 */
// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
bool __fastcall TBDeqptoAccess::CarregaReducaoEstudo(VTEstudo *estudo, TList *lisMREDE)
{
	// variáveis locais
	AnsiString strlog;
	TList *lisCORTE;
	VTMRede *mrede;
	VTCorte *corte;

	// proteções
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	// Limpa a tabela temporária
	if (!SPSNP_dlTmpEstudoRedeCorte())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRBDDL_CORTES return (false);
	}

	// verifica se foi definida uma lista de MRedes das reduções
	if ((estudo->Id != estudoID_ZERO) || (lisMREDE == NULL))
	{ // carrega todas as Reduções do Estudo
		// atualiza progresso
		if (progresso)
			progresso->Add("Leitura das redes equivalentes");
		// lê as Redes da MRedes (apenas as Redes, sem seus Eqptos)
		// if (!SPSNP_rdRedeMRede(estudo, mrede->Id, NULL))         { SNPSTRLOGLINE(strlog); MSGERRBDRD_REDEMREDE    return(false); }
		if (!SPSNP_rdRRBarra(estudo, true))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_RRBARRA return (false);
		}

        if (reservado)
            progresso->Step();
		if (!SPSNP_rdRRCarga(estudo, true))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_RRCARGA return (false);
		}

        if (reservado)
            progresso->Step();
		if (!SPSNP_rdRRSuprimento(estudo, true))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_RRSUPRIMENTO return (false);
		}

        if (reservado)
            progresso->Step();
		if (!SPSNP_rdRRTrafo(estudo, true))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_RRTRAFO return (false);
		}

        if (reservado)
            progresso->Step();
		if (!SPSNP_rdRRYRef(estudo, true))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_RRYREF return (false);
		}

        if (reservado)
            progresso->Step();
	}
	else
	{ // carrega apenas as Reducoes das MRedes definidas em lisMREDE
		if (lisMREDE->Count == 0)
			return (true);
		// atualiza progresso
		if (progresso)
			progresso->Add("Leitura das redes equivalentes");
		// Monta a tabela temporária de cortes (já foi limpa previamente
		if (!SPSNP_wrTmpEstudoMRedeCorte(estudo, lisMREDE))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDDL_CORTES return (false);
		}

        if (reservado)
            progresso->Step();

		// lê as Redes da MRedes (apenas as Redes, sem seus Eqptos)
		// if (!SPSNP_rdRedeMRede(estudo, mrede->Id, NULL))         { SNPSTRLOGLINE(strlog); MSGERRBDRD_REDEMREDE    return(false); }
		if (!SPSNP_rdRRBarra(estudo, false))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_RRBARRA return (false);
		}

        if (reservado)
            progresso->Step();
		if (!SPSNP_rdRRCarga(estudo, false))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_RRCARGA return (false);
		}

        if (reservado)
            progresso->Step();
		if (!SPSNP_rdRRSuprimento(estudo, false))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_RRSUPRIMENTO return (false);
		}

        if (reservado)
            progresso->Step();
		if (!SPSNP_rdRRTrafo(estudo, false))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_RRTRAFO return (false);
		}

        if (reservado)
            progresso->Step();
		if (!SPSNP_rdRRYRef(estudo, false))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDRD_RRYREF return (false);
		}

        if (reservado)
            progresso->Step();
	}
	// atualiza progresso
	// if (progresso) progresso->Stop();
	return (true);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
bool __fastcall TBDeqptoAccess::CarregaRedeEstudo(VTEstudo *estudo, TList *lisREDE)
{
	// Variáveis
	int num_passo;
	AnsiString straux;
	AnsiString strlog;
	bool sucesso = true;
	VTRede *rede;

	// determina número de passos de progresso
	// !OOPS! acertar o úmero final de etapas
	num_passo = 21;
	// reinicia cronometros
	if (cronometro)
		cronometro->Reinicia();
	if (cronometro)
		cronometro->Start("RedeTotal");
	// exibe barra de progresso
	if (progresso)
		progresso->Add("Leitura das redes selecionadas");
	// Preenche a tabela temporária
	if (!SPSNP_wrTmpEquip(lisREDE))
	{
		SNPSTRLOGLINE(strlog);
	}

	if (reservado)
		progresso->Step();

	//lê municipios
	if (cronometro)
		cronometro->Start("Municipio");
	if (progresso)
		progresso->Add("Municipio");
	if ((sucesso = SPSNP_rdMunicipio()) == false)
	{
		SNPSTRLOGLINE(strlog);
	}

	if (reservado)
		progresso->Step();
	if (cronometro)
		cronometro->Stop("Municipio");

	// Lê as barras
	if (cronometro)
		cronometro->Start("Barra");
	if (progresso)
		progresso->Add("Barra");
	if ((sucesso = SPSNP_rdBarra(estudo, true, false)) == false)
	{
		SNPSTRLOGLINE(strlog);
	}

	if (reservado)
		progresso->Step();
	if (cronometro)
		cronometro->Stop("Barra");
	// JCG 2016.01.27
	// monta->InsereBarraStart();

	// lê todos Eqptos da Rede
	if (sucesso)
	{
		if ((sucesso = CarregaEqptosRede(estudo, true, false)) == false)
		{
			SNPSTRLOGLINE(strlog);
		}
	}

	if (reservado)
		progresso->Step();

	// Acerta o status das redes
	if (sucesso)
	{
		for (int i = 0; (sucesso) && (i < lisREDE->Count); i++)
		{
			rede = (VTRede*)lisREDE->Items[i];
			// atualiza indicação de que a rede real foi carregada
			// rede->Valida = true;
			rede->MRede->Carregada = true;
		}
	}

	if (reservado)
		progresso->Step();

	// Leitura dos sincronismos e manobras
	if (sucesso)
	{ // Sincronismos
		if (cronometro)
			cronometro->Start("Sincronismo");
		if (progresso)
			progresso->Add("Leitura dos Sincronismos entre chaves");
		sucesso = SPSNP_rdSincronismo();

		if (reservado)
			progresso->Step();
		if (cronometro)
			cronometro->Stop("Sincronismo");
		if (!sucesso)
		{
			SNPSTRLOGLINE(strlog);
		}
	}
	if (sucesso)
	{ // Manobras
		if (cronometro)
			cronometro->Start("Manobra");
		if (progresso)
			progresso->Add("Leitura das manobras");
		sucesso = SPSNP_rdManobra();

		if (reservado)
			progresso->Step();
		if (cronometro)
			cronometro->Stop("Manobra");
		if (!sucesso)
		{
			SNPSTRLOGLINE(strlog);
		}
	}
	if (sucesso)
	{ // executa montagem final das Redes
		if (cronometro)
			cronometro->Start("Montagem");
		if (progresso)
			progresso->Add("Montagem das redes");

		if (reservado)
			progresso->Step();
		if (cronometro)
			cronometro->Stop("Montagem");
	}
	// if (progresso) progresso->Stop();
	// finaliza cornometro total
	if (cronometro)
		cronometro->Stop("RedeTotal");
	// exibe cronometro
	if (cronometro)
		cronometro->Show();
	// retorna resultado
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::CopiaSchemaIni(void)
{
	// variáveis locais
	AnsiString strlog;
	AnsiString strFileName = "schema.ini";
	WideString strDirSrc;
	WideString strDirDst;

	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}

	strDirSrc = path->DirGeral() + "\\" + strFileName;
	strDirDst = path->DirTmp() + "\\" + strFileName;
	return (CopyFile(strDirSrc.c_bstr(), strDirDst.c_bstr(), false));
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::CriaEstudoNovo(VTEstudo *estudo, TList *lisREDE)
{
	// OOPS: No Oracle é a mesma coisa
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::InsereCampo(AnsiString tabela, AnsiString campo, AnsiString tipo)
{
	// Variáveis locais
	AnsiString strSQL, strlog;

	// Monta a string de execução para inserir campo de string na tabela
	strSQL = "ALTER TABLE " + tabela + " ADD COLUMN " + campo + " " + tipo;
	// Decodifica comando
	bd_conn->ParseCmd(strSQL);
	// Comando da consulta
	if (!bd_conn->Execute(true))
	{
		SNPSTRLOGLINE(strlog);
		return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::LeListaCargaNL(VTEstudo *estudo, TList *lisREDE, TList* lisCargaNL,
	bool bolObra)
{
	// Variáveis locais
	VTRede* rede;

	if (!estudo)
	{
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisREDE)
	{
		MSGERRSYS_LISOBJINV return (false);
	}
	if (!lisCargaNL)
	{
		MSGERRSYS_LISOBJINV return (false);
	}

	for (int i = 0; i < lisREDE->Count; i++)
	{
		if ((rede = (VTRede*)lisREDE->Items[i]) == NULL)
		{
			MSGERRSYS_REDEINV return (false);
		}

		if (!SPSNP_rdCargaNL(estudo, rede->Id, bolObra))
		{
			MSGERRSYS_REDEINV return (false);
		}
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::LeListaCorte(VTEstudo *estudo, TList *lisMREDE, TList *lisCORTE)
{
	AnsiString strlog;

	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisMREDE)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISMREDEINV return (false);
	}

	return (SPSNP_rdCorteMRede(estudo, lisMREDE, lisCORTE));
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::LeListaDominio(void)
{
	return (SPSNP_rdDominio());
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::LeListaEstudo(void)
{
	// vai ser utilizado esse método dependendo da classe criada
	return (SPSNP_rdEstudo());
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::LeListaMRede(VTEstudo *estudo, TList *lisCORTE)
{
	// variáveis locais
	AnsiString strlog;

	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisCORTE)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISCORTEINV return (false);
	}

	return (SPSNP_rdMRedeCorte(estudo, lisCORTE));
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::LeListaRede(VTEstudo *estudo, VTDominio *dominio)
{
	return (SPSNP_rdRedeDominio(estudo, dominio));
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::LeListaRede(VTEstudo *estudo, VTTipoRede *tiporede)
{
	// variáveis locais
	AnsiString strlog;
	int tipo_rede_id = -1;

	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!SPSNP_rdDominio())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}

	// verifica se foi definido um determinado TipoRede
	if (tiporede)
		tipo_rede_id = tiporede->Id;

	return (SPSNP_rdRede(estudo, tipo_rede_id, false));
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::LeListaRedeVizinha(VTEstudo *estudo, VTRede *rede, TList *lisVIZ)
{
	// variáveis locais
	AnsiString strlog;

	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!rede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDEINV return (false);
	}
	if (!lisVIZ)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISOBJINV return (false);
	}
	if (!rede->MRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_MREDEINV return (false);
	}

	return (SPSNP_rdRedeVizinha(estudo, rede->MRede->Id, lisVIZ));
}

/*
 //---------------------------------------------------------------------------
 int __fastcall TBDeqptoAccess::LeQtdeCargaRede(int rede_id)
 {
 int           intret   = IDINDEFINIDO;
 TADOQuery*    ptrQryDS = NULL;
 AnsiString    strSQL;
 AnsiString    strlog;

 if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(intret); }
 if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(intret); }

 __try
 {
 try
 {
 strSQL =
 " \
 SELECT \
 DISTINCT(IIF(ISNULL(COUNT(Equipamento.intEquipId)), 0, COUNT(Equipamento.intEquipId))) AS CargaCount \
 FROM \
 ( \
 ( \
 ( \
 Equipamento \
 INNER JOIN Carga ON \
 Equipamento.intEstudoId = Carga.intEstudoId AND \
 Equipamento.intEquipId = Carga.intCargaId \
 ) \
 INNER JOIN EquipamentoBarra ON \
 Equipamento.intEstudoId = EquipamentoBarra.intEstudoId AND \
 Equipamento.intEquipId = EquipamentoBarra.intEquipId \
 ) \
 INNER JOIN BarraRede ON \
 EquipamentoBarra.intEstudoId = BarraRede.intEstudoId AND \
 EquipamentoBarra.intBarraId = BarraRede.intBarraId \
 ) \
 WHERE \
 Equipamento.intEstudoId = 0 AND Equipamento.intTipoEquipId = 7 AND BarraRede.intRedeId = ";
 strSQL += IntToStr(rede_id);

 bd_conn->ParseCmd(strSQL, cmdText);
 if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(intret); }
 if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(intret); }
 // Recupera o resultado
 if (ptrQryDS->RecordCount != 1)                 { SNPSTRLOGLINE(strlog); return(intret); }
 intret = ptrQryDS->FieldByName("CargaCount")->AsInteger;
 ptrQryDS->Close();
 }
 catch (Exception& e)
 {
 SNPSTRLOGLINE(strlog);
 SNPSTRLOG(e.Message);
 intret = IDINDEFINIDO;
 }
 }
 __finally
 {
 // Colocado dentro do finally para garantir que será deletado, mesmo que
 // haja um return no meio do código

 //destrói objetos locais
 if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
 }

 return(intret);
 }
 */
/*
 //---------------------------------------------------------------------------
 int __fastcall TBDeqptoAccess::LeQtdeCargaRede(int rede_id)
 {
 int           intret   = IDINDEFINIDO;
 TADOQuery*    ptrQryDS = NULL;
 AnsiString    strSQL;
 AnsiString    strlog;

 if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(intret); }
 if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(intret); }

 __try
 {
 try
 {
 strSQL =
 " \
 SELECT \
 DISTINCT(IIF(ISNULL(COUNT(Carga.intCargaId)), 0, COUNT(Carga.intCargaId))) AS CargaCount \
 FROM \
 ( \
 ( \
 Carga \
 INNER JOIN EquipamentoBarra ON \
 Carga.intEstudoId = EquipamentoBarra.intEstudoId AND \
 Carga.intCargaId = EquipamentoBarra.intEquipId \
 ) \
 INNER JOIN BarraRede ON \
 EquipamentoBarra.intEstudoId = BarraRede.intEstudoId AND \
 EquipamentoBarra.intBarraId = BarraRede.intBarraId \
 ) \
 WHERE \
 Carga.intEstudoId = 0 AND BarraRede.intRedeId = ";
 strSQL += IntToStr(rede_id);

 bd_conn->ParseCmd(strSQL, cmdText);
 if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(intret); }
 if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(intret); }
 // Recupera o resultado
 if (ptrQryDS->RecordCount != 1)                 { SNPSTRLOGLINE(strlog); return(intret); }
 intret = ptrQryDS->FieldByName("CargaCount")->AsInteger;
 ptrQryDS->Close();
 }
 catch (Exception& e)
 {
 SNPSTRLOGLINE(strlog);
 SNPSTRLOG(e.Message);
 intret = IDINDEFINIDO;
 }
 }
 __finally
 {
 // Colocado dentro do finally para garantir que será deletado, mesmo que
 // haja um return no meio do código

 //destrói objetos locais
 if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
 }

 return(intret);
 }
 */
// ---------------------------------------------------------------------------
int __fastcall TBDeqptoAccess::LeQtdeCargaRede(int rede_id)
{
	int intret = IDINDEFINIDO;
	TADOQuery* ptrQryDS = NULL;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (intret);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (intret);
	}

	__try
	{
		try
		{
			strSQL = " \
         SELECT \
           IIF(ISNULL(COUNT(Carga.intCargaId)), 0, COUNT(Carga.intCargaId)) AS CargaCount \
         FROM \
           ( \
           ( \
           Carga \
             INNER JOIN EquipamentoBarra ON \
               Carga.intEstudoId = EquipamentoBarra.intEstudoId AND \
               Carga.intCargaId = EquipamentoBarra.intEquipId \
           ) \
             INNER JOIN BarraRede ON \
               EquipamentoBarra.intEstudoId = BarraRede.intEstudoId AND \
               EquipamentoBarra.intBarraId = BarraRede.intBarraId \
           ) \
         WHERE \
           Carga.intEstudoId = 0 AND BarraRede.intRedeId = ";
			strSQL += IntToStr(rede_id);

			bd_conn->ParseCmd(strSQL, cmdText);
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (intret);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (intret);
			}
			// Recupera o resultado
			if (ptrQryDS->RecordCount != 1)
			{
				SNPSTRLOGLINE(strlog);
				return (intret);
			}
			intret = ptrQryDS->FieldByName("CargaCount")->AsInteger;
			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			intret = IDINDEFINIDO;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (intret);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::RemoveDominio(VTEstudo *estudo, VTDominio *dominio)
{
	return (SPSNP_dlDominio(estudo, dominio));
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::RemoveEstudo(VTEstudo *estudo)
{
	return (SPSNP_dlEstudo(estudo, true));
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::RemoveEstudoRede(VTEstudo *estudo, VTRede *rede)
{
	return (SPSNP_dlEstudoRede(estudo, rede));
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::RemoveManobra(VTManobra* manobra)
{
	return (SPSNP_dlManobra(manobra));
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::RemoveSincronismo(VTLock* lock)
{
	return (SPSNP_dlSincronismo(lock));
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
bool __fastcall TBDeqptoAccess::SalvaAlternativaEstudo(VTEstudo *estudo)
{
	AnsiString strlog;
	TList* lisEQP;
	VTEqpto* eqpto;
	VTCurva* curva;
	VTTrecho* trecho;

	// IMPORTANTE: atualiza o separador decimal com o separador original do Windows
	FormatSettings.DecimalSeparator = basico->WinDecimalSeparator;

	// IMPORTANTE: garante que o estudo não tenha Status novo
	estudo->Novo = false;
	// Evita explosões
	if (!bd_curva)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDCURVAINV return (false);
	}

	// exibe progresso

//	if (progresso)
//		progresso->Add("Salvando Estudo");
	if (progresso)
		progresso->Add("Salvando Alternativas");
	// Reinicia o indexador de equipamentos
	FNSNP_rdEquipIdProx(estudo, false, true);

	if (progresso)
		progresso->Step();
	// Reinicia o indexador de curvas
	bd_curva->FNSNP_rdConjCurvaIdProx(false, true);

	if (progresso)
		progresso->Step();

	if (!SPSNP_dlEstudo(estudo, false))
	{
		if (progresso)
			progresso->Stop();
		SNPSTRLOGLINE(strlog);
		MSGERRBDDL_ESTUDOALT return (false);
	}

	if (progresso)
		progresso->Step();

	// Atualiza status dos equipamentos das obras
	lisEQP = new TList();
	estudo->LisEqptoEstudo(lisEQP, NULL, IDINDEFINIDO);
	for (int i = 0; i < lisEQP->Count; i++)
	{
		if ((eqpto = (VTEqpto*)lisEQP->Items[i]) == NULL)
		{
			if (progresso)
				progresso->Stop();
			SNPSTRLOGLINE(strlog);
			MSGERRBDDL_ESTUDOALT return (false);
		}
		eqpto->Id = -1;
		eqpto->DefineStatus(sttNOVO, true);
		// verifica se Trecho com Arranjo próprio
		if (eqpto->Tipo() == eqptoTRECHO)
		{
			trecho = (VTTrecho*)eqpto;
			if (!trecho->ArranjoTipico)
			{ // redefine Arranjo como novo
				trecho->arranjo->Id = -1;
				trecho->arranjo->DefineStatus(sttNOVO, true);
			}
		}
		// define status da Curva do Eqpto, se existir
		if ((curva = eqpto->ExisteCurva()) != NULL)
		{
			if (!curva->Tipica)
			{
				curva->Id = -1;
				curva->DefineStatus(sttNOVO, true);
			}
		}
	}
	delete lisEQP;


	if (progresso)
		progresso->Step();

	// atualiza progresso
	if (progresso)
		progresso->Add("Salvando equipamentos");
	// atualiza dados do Estudo (o usuário pode alterar o código do estudo)

	// versão com campo justificativa do Estudo
	// è implementada a classe da base que está sendo aberta
	if (!SPSNP_wrEstudo(estudo))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRBDWR_ESTUDO return (false);
	}

	if (progresso)
		progresso->Step();

	// grava Eqptos do estudo
	if (!SPSNP_wrEstudoEqpto(estudo))
	{
		if (progresso)
			progresso->Stop();
		SNPSTRLOGLINE(strlog);
		MSGERRBDWR_ESTUDOEQUIP return (false);
	}

	if (progresso)
		progresso->Step();

	// atualiza progresso
	if (progresso)
		progresso->Add("Salvando obras");
	if (!SPSNP_wrEstudoAlternativa(estudo))
	{
		if (progresso)
			progresso->Stop();
		SNPSTRLOGLINE(strlog);
		MSGERRBDWR_ESTUDOALT return (false);
	}

	if (progresso)
		progresso->Step();

	if (!SPSNP_wrAlternativaObra(estudo))
	{
		if (progresso)
			progresso->Stop();
		SNPSTRLOGLINE(strlog);
		MSGERRBDWR_ESTUDOOBRA return (false);
	}

	if (progresso)
		progresso->Step();

	if (!SPSNP_wrObraAcao(estudo))
	{
		if (progresso)
			progresso->Stop();
		SNPSTRLOGLINE(strlog);
		MSGERRBDWR_ESTUDOACAO return (false);
	}

	if (progresso)
		progresso->Step();

	// grava atributos da obra somente para base Rev3.1 ou superior
	if (!SPSNP_wrObraAtribExt(estudo))
	{
		if (progresso)
			progresso->Stop();
		SNPSTRLOGLINE(strlog);
		MSGERRBDWR_ESTUDOOBRA return (false);
	}

	if (progresso)
		progresso->Step();

	// esconde progresso
	if (progresso)
		progresso->Stop();
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::RenovaStatus(VTEstudo* estudo)
{
	bool sucesso = true;
	AnsiString strlog;
	VTEqpto* eqpto;
	VTReducao* reducao;
	TList* lisEQP;
	VTDominio* dominio;
	TList* lisAUX;
	VTManobra* manobra;
	VTLock* lock;
	VTManobras* manobras = (VTManobras*)apl->GetObject(__classid(VTManobras));
	VTLocks* locks = (VTLocks*)apl->GetObject(__classid(VTLocks));

	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}
	if (!manobras)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_MANOBRASINV return (false);
	}
	if (!locks)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SINCRONISMOSINV return (false);
	}

	try
	{
		lisEQP = new TList();

		// Reinicia controle local de indexadores
		FNSNP_rdEquipIdProx(estudo, false, true);
		// Reinicia o indexador de curvas
		bd_curva->FNSNP_rdConjCurvaIdProx(false, true);
		FNSNP_rdAgrupamentoIdProx(false, true);
		FNSNP_rdCorteIdProx(false, true);
		FNSNP_rdManobraIdProx(false, true);
		FNSNP_rdSincronismoIdProx(false, true);

		// reseta status do Estudo
		estudo->Novo = true;
		// Reseta o status dos domínios
		lisAUX = redes->LisDominio();
		for (int nc = 0; nc < lisAUX->Count; nc++)
		{
			dominio = (VTDominio*)lisAUX->Items[nc];
			dominio->DefineStatus(sttNOVO, true);
		}
		// Reseta o status das manobras
		lisAUX = manobras->LisManobra();
		for (int nc = 0; nc < lisAUX->Count; nc++)
		{
			manobra = (VTManobra*)lisAUX->Items[nc];
			manobra->Status[sttALTERADO] = false;
			manobra->Status[sttNOVO] = true;
		}
		// Reseta o status dos sincronismos
		lisAUX = locks->LisLock();
		for (int nc = 0; nc < lisAUX->Count; nc++)
		{
			lock = (VTLock*)lisAUX->Items[nc];
			lock->Status[sttALTERADO] = false;
			lock->Status[sttNOVO] = true;
		}
		/*
		 //JCG - 2011.04.15 - reinicia IDs das MRede
		 lisEQP->Clear();
		 redes->LisMRede(lisEQP);
		 for (int n = 0; n < lisEQP->Count; n++)
		 {
		 eqpto = (VTEqpto*)lisEQP->Items[n];
		 eqpto->Id = IDINDEFINIDO;
		 }
		 */
		// inicia IDs p/ Clusters, MRedes, Redes, Cortes e Barras
		RenovaStatus(redes->LisCluster());
		RenovaStatus(redes->LisMRede());
		RenovaStatus(redes->LisRede());
		RenovaStatus(redes->LisCorte());
		RenovaStatus(redes->LisBarra());
		// define IDs p/ Eqbar
		lisEQP->Clear();
		redes->LisEqbar(lisEQP);
		RenovaStatus(lisEQP);
		// suprimentos já está incluído nos Eqbar
		// lisEQP->Clear();
		// redes->LisEqbar(lisEQP, eqptoSUPRIMENTO);
		// RenovaStatus(lisEQP);

		// define IDs p/ Ligacao
		lisEQP->Clear();
		redes->LisLigacao(lisEQP);
		RenovaStatus(lisEQP);
		// define IDs p/ Mutua
		lisEQP->Clear();
		redes->LisEqpto(lisEQP, eqptoMUTUA);
		RenovaStatus(lisEQP);
		// define IDs p/ Reducoes
		lisEQP->Clear();
		redes->LisReducao(lisEQP);
		RenovaStatus(lisEQP);
		// define IDs p/ Eqptos das reducoes
		for (int n = 0; n < lisEQP->Count; n++)
		{
			reducao = (VTReducao*)lisEQP->Items[n];
			RenovaStatus(reducao->LisEqbar());
			RenovaStatus(reducao->LisLigacao());
		}

		// Deleta a lista criada
		delete lisEQP;
	}
	catch (Exception& e)
	{
		sucesso = false;
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::RenovaStatus(TList *lisEQP)
{
	bool sucesso = true;
	VTCanal* canal;
	VTEqpto* eqpto;
	VTCurva* curva;
	VTMedidor* medidor;
	VTTrecho* trecho;
	TList* lisCANAL;

	// Reinicia os identificadores / status NOVO do equipamentos
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		eqpto->Id = IDINDEFINIDO;
		eqpto->DefineStatus(sttNOVO, true);
		// Tem que renovar o status das curvas também.
		if ((curva = eqpto->ExisteCurva()) != NULL)
		{
			if (!curva->Tipica)
			{
				curva->StatusAsUnsigned = 0x0;
				curva->Status[sttNOVO] = true;
			}
		}
		// atualiza status de Arranjo próprio
		if (eqpto->Tipo() == eqptoTRECHO)
		{
			trecho = (VTTrecho*)eqpto;
			if (!trecho->ArranjoTipico)
			{ // atualiza Status do Arranjo próprio do trecho
				trecho->arranjo->StatusAsUnsigned = 0;
				trecho->arranjo->Status[sttNOVO] = true;
			}
		}
		// atualiza status dos Canais dos Medidores
		else if (eqpto->Tipo() == eqptoMEDIDOR)
		{
			medidor = (VTMedidor*)eqpto;
			lisCANAL = medidor->LisCanal();
			for (int i = 0; i < lisCANAL->Count; i++)
			{
				canal = (VTCanal*)lisCANAL->Items[i];
				canal->Id = IDINDEFINIDO;
				canal->DefineStatus(sttNOVO, true);
			}
		}
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SalvaListaCargaNL(VTEstudo *estudo, TList* lisCargaNL, bool bolObra)
{
	return (SPSNP_wrCargaNL(estudo, lisCargaNL, bolObra));
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/16082112
bool __fastcall TBDeqptoAccess::SalvaRedeEstudo(VTEstudo *estudo)
{
	// variáveis locais
	AnsiString strlog;
	VTMRede* mrede;
	VTRede* rede;
	VTEqpto* eqpto;
	VTReducao* reducao;
	VTDominio* dominio;
	TList* lisREDE;
	TList* lisCLUSTER;
	TList* lisDOMINIO;
	TList* lisREDEDOMS;
	TList* lisEQP = NULL;

	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}
	if (!bd_curva)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDCURVAINV return (false);
	}

	__try
	{
		try
		{ // IMPORTANTE: atualiza o separador decimal com o separador original do Windows
			if (cronometro)
				cronometro->Reinicia();
			if (cronometro)
				cronometro->Start("SalvaRedeEstudo");


            reservado = false;
            if (progresso)
            {
                progresso->Start(progTEXTO);
                progresso->NoAutoInc();
                reservado = progresso->MaxStep(MAX_STEP_SALVA_REDE_ESTUDO);
            }

			FormatSettings.DecimalSeparator = basico->WinDecimalSeparator;
			// Reinicia o indexador de estudos
			FNSNP_rdEstudoIdProx(false, true);
			// Reinicia o indexador de equipamentos
			FNSNP_rdEquipIdProx(estudo, false, true);
			// Reinicia o indexador de curvas
			bd_curva->FNSNP_rdConjCurvaIdProx(false, true);

            if (reservado)
                progresso->Step();

			// Lista auxiliar para carga de equipamentos
			lisEQP = new TList();
			// Lista auxiliar para carga domínios por rede
			lisREDEDOMS = new TList();

			// Se o estudo é novo, registra na base de dados antes de começar
			// a salvar o equipamentos.
			if (estudo->Novo)
			{
				redes->DefineStatus(sttNOVO, true);
				if (!SPSNP_wrEstudo(estudo))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_ESTUDO return (false);
				}
			}

            if (reservado)
                progresso->Step();

			// Salva dados dos Agrupamentos
			// !OOPS! Acertar o número de etapas
			if (progresso)
				progresso->Add("Atualizando base de dados");
			if (cronometro)
				cronometro->Start("Agrupamentos");
			if (progresso)
				progresso->Add("Salvando agrupamentos");
			if ((lisCLUSTER = redes->LisCluster()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBDWR_CLUSTERS return (false);
			}

            if (reservado)
                progresso->Step();

			if (lisCLUSTER->Count > 0)
			{
				if (!SPSNP_wrAgrupamento(estudo, lisCLUSTER))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_CLUSTERS return (false);
				}
			}

            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Agrupamentos");

			// !OOPS!
			// 1) Aqui precisa apagar os clusters inúteis

			// Salva dados de Redes
			lisREDE = redes->LisRede();
			// Atualiza dados de cada Rede (em memória)
			for (int i = 0; i < lisREDE->Count; i++)
			{
				rede = (VTRede*)lisREDE->Items[i];
				rede->Valida = rede->Carregada;

				if (rede->Carregada)
					rede->Revisao = Now();
			}

            if (reservado)
                progresso->Step();


			// salva dados dos Domínios
			if (cronometro)
				cronometro->Start("Domínios");
			lisDOMINIO = redes->LisDominio();
			if (lisDOMINIO->Count > 0)
			{
				if (progresso)
					progresso->Add("Salvando domínios");
				if (!SPSNP_wrDominio(lisDOMINIO, false))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_DOMINIOS return (false);
				}
			}

            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Domínios");

			// salva dados dos Municipios
			if (cronometro)
				cronometro->Start("Municipios");

				if (progresso)
					progresso->Add("Salvando Municipios");
				if (!SPSNP_wrMunicipio())
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

                if (reservado)
                    progresso->Step();

			if (cronometro)
				cronometro->Stop("Municipios");

			// Salva dados das barras
			// Uma mesma barra pode estar em duas redes e será salva para todas as redes
			if (cronometro)
				cronometro->Start("Barras");
			if (progresso)
				progresso->Add("Salvando barras");
			lisEQP->Clear();
			// OOPS. Precisa fazer a alteração no redes para trazer as barras de chaves abertas
			// redes->LisBarra(lisEQP);//, true); // Carrega também as barras das chaves abertas
			redes->LisEqptoNaoIsolado(lisEQP, eqptoBARRA);
			if (!SPSNP_wrBarra(estudo, lisEQP))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBDWR_WREQUIPREDE(strlog, "barras") return (false);
			}

            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Barras");

			// Salva dados de Redes
			// OOPS: Na versão original (e no ORACLE está assim), a barra inicial
			// das redes é atualizada posteriormente. Nessa versão, é colocada na
			// inserção da rede, uma vez que não há relacionamento da BarraIni para barra
			if (cronometro)
				cronometro->Start("Redes");
			if (progresso)
				progresso->Add("Salvando redes");
			if (lisREDE->Count > 0)
			{
				if (!SPSNP_wrRede(estudo, lisREDE))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_REDES return (false);
				}
			}

            if (reservado)
                progresso->Step();
			if (cronometro)
				cronometro->Stop("Redes");

			// Associa as barras e domínios às redes
			if (progresso)
				progresso->Add("Salvando barras por Rede");
			if (cronometro)
				cronometro->Start("BarraRede");
			if (!SPSNP_wrBarraRedeIns(estudo))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBDWR_WREQUIPREDE(strlog, "barras (redes)") return (false);
			}

            if (reservado)
                progresso->Step();
			if (cronometro)
				cronometro->Stop("BarraRede");

			if (progresso)
				progresso->Add("Salvando Redes de Domínios");
			if (cronometro)
				cronometro->Start("RedeDominio");
			if (!SPSNP_wrRedeDominioIns(estudo, lisREDE))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBDWR_DOMREDE return (false);
			}

            if (reservado)
                progresso->Step();
			if (cronometro)
				cronometro->Stop("RedeDominio");

			// ------------------------
			// Salva dados das ligações
			// ------------------------

			// Salva dados dos trechos
			if (cronometro)
				cronometro->Start("Trechos");
			if (progresso)
				progresso->Add("Salvando trechos");
			// Agrupa os trechos das redes carregadas
			lisEQP->Clear();
			for (int n = 0; n < lisREDE->Count; n++)
			{
				// loop redes (ini)
				rede = (VTRede*)lisREDE->Items[n];
				// verifica se a Rede está carregada
				if (!rede->Carregada)
					continue;
				// Aglutina os equipamentos
				rede->LisLigacao(lisEQP, eqptoTRECHO);
			}

            if (reservado)
                progresso->Step();

			// Escreve os trechos das redes carregadas
			if (!SPSNP_wrTrecho(estudo, lisEQP))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBDWR_WREQUIPREDE(strlog, "trechos") return (false);
			}

            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Trechos");

			// Salva dados das chaves
			if (cronometro)
				cronometro->Start("Chaves");
			if (progresso)
				progresso->Add("Salvando chaves");
			// Agrupa os trechos das redes carregadas
			lisEQP->Clear();
			for (int n = 0; n < lisREDE->Count; n++)
			{
				// loop redes (ini)
				rede = (VTRede*)lisREDE->Items[n];
				// verifica se a Rede está carregada
				if (!rede->Carregada)
					continue;
				// Aglutina os equipamentos
				rede->LisLigacao(lisEQP, eqptoCHAVE);
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (!SPSNP_wrChave(estudo, lisEQP))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBDWR_WREQUIPREDE(strlog, "chaves") return (false);
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Chaves");

			// Salva dados dos transformadores
			if (cronometro)
				cronometro->Start("Transformadores");
			if (progresso)
				progresso->Add("Salvando transformadores");
			// Agrupa os trechos das redes carregadas
			lisEQP->Clear();
			for (int n = 0; n < lisREDE->Count; n++)
			{
				// loop redes (ini)
				rede = (VTRede*)lisREDE->Items[n];
				// verifica se a Rede está carregada
				if (!rede->Carregada)
					continue;
				// Aglutina os equipamentos
				rede->LisLigacao(lisEQP, eqptoTRAFO);
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrTrafo(estudo, lisEQP))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "trafos") return (false);
				}
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Transformadores");

			if (cronometro)
				cronometro->Start("Transformadores 3 Enrolamentos");
			lisEQP->Clear();
			for (int n = 0; n < lisREDE->Count; n++)
			{
				// loop redes (ini)
				rede = (VTRede*)lisREDE->Items[n];
				// verifica se a Rede está carregada
				if (!rede->Carregada)
					continue;
				// Aglutina os equipamentos
				rede->LisLigacao(lisEQP, eqptoTRAFO3E);
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrTrafo3E(estudo, lisEQP))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "trafos 3E") return (false);
				}
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Transformadores 3 Enrolamentos");

			// Salva dados dos capacitores série
			if (cronometro)
				cronometro->Start("CapSerie");
			if (progresso)
				progresso->Add("Salvando capacitores série");
			lisEQP->Clear();
			for (int n = 0; n < lisREDE->Count; n++)
			{
				// loop redes (ini)
				rede = (VTRede*)lisREDE->Items[n];
				// verifica se a Rede está carregada
				if (!rede->Carregada)
					continue;
				// Aglutina os equipamentos
				rede->LisLigacao(lisEQP, eqptoCAPSERIE);
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrCapacitorSerie(estudo, lisEQP))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "capacitores série") return (false);
				}
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("CapSerie");

			// Salva dados das mútuas
			if (cronometro)
				cronometro->Start("Mútuas");
			if (progresso)
				progresso->Add("Salvando impedâncias mútuas");
			lisEQP->Clear();
			for (int n = 0; n < lisREDE->Count; n++)
			{
				// loop redes (ini)
				rede = (VTRede*)lisREDE->Items[n];
				// verifica se a Rede está carregada
				if (!rede->Carregada)
					continue;
				// Aglutina os equipamentos
				rede->LisMutua(lisEQP);
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrMutua(estudo, lisEQP))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "mútuas") return (false);
				}
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Mútuas");

			// Salva dados dos reguladores
			if (cronometro)
				cronometro->Start("Reguladores");
			if (progresso)
				progresso->Add("Salvando reguladores");

			lisEQP->Clear();
			for (int n = 0; n < lisREDE->Count; n++)
			{
				// loop redes (ini)
				rede = (VTRede*)lisREDE->Items[n];
				// verifica se a Rede está carregada
				if (!rede->Carregada)
					continue;
				// Aglutina os equipamentos
				rede->LisLigacao(lisEQP, eqptoREGULADOR);
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrRegulador(estudo, lisEQP))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "reguladores") return (false);
				}
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Reguladores");

			// ---------------------
			// Salva dados dos eqbar
			// ---------------------

			// Salva dados das baterias
			if (cronometro)
				cronometro->Start("Baterias");
			if (progresso)
				progresso->Add("Salvando baterias");
			lisEQP->Clear();
			redes->LisEqptoNaoIsolado(lisEQP, eqptoBATERIA);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrBateria(estudo, lisEQP))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "baterias") return (false);
				}
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Baterias");

			// Salva dados das cargas
			if (cronometro)
				cronometro->Start("Cargas");
			if (progresso)
				progresso->Add("Salvando cargas");
			lisEQP->Clear();
			redes->LisEqptoNaoIsolado(lisEQP, eqptoCARGA);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrCarga(estudo, lisEQP))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "cargas") return (false);
				}
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Cargas");

			if (cronometro)
				cronometro->Start("CargasIP");
			if (progresso)
				progresso->Add("Salvando cargas (iluminação pública)");
			lisEQP->Clear();
			redes->LisEqptoNaoIsolado(lisEQP, eqptoCARGA);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrCargaIP(estudo, lisEQP))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "cargas iluminação pública") return (false);
				}
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("CargasIP");

			// Salva dados dos capacitores
			if (cronometro)
				cronometro->Start("Capacitores");
			if (progresso)
				progresso->Add("Salvando capacitores");
			lisEQP->Clear();
			redes->LisEqptoNaoIsolado(lisEQP, eqptoCAPACITOR);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrCapacitor(estudo, lisEQP))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "capacitores") return (false);
				}
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Capacitores");

			// Salva dados dos geradores
			if (cronometro)
				cronometro->Start("Geradores");
			if (progresso)
				progresso->Add("Salvando geradores");
			lisEQP->Clear();
			redes->LisEqptoNaoIsolado(lisEQP, eqptoGERADOR);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrGerador(estudo, lisEQP))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "geradores") return (false);
				}
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Geradores");

			// Salva dados dos suprimentos
			if (cronometro)
				cronometro->Start("Suprimentos");
			if (progresso)
				progresso->Add("Salvando suprimentos");
			lisEQP->Clear();
			redes->LisEqptoNaoIsolado(lisEQP, eqptoSUPRIMENTO);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrSuprimento(estudo, lisEQP))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "suprimentos") return (false);
				}
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Suprimentos");

			// Salva dados dos filtros
			if (cronometro)
				cronometro->Start("Filtros");
			if (progresso)
				progresso->Add("Salvando filtros");
			lisEQP->Clear();
			redes->LisEqptoNaoIsolado(lisEQP, eqptoFILTRO);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrFiltro(estudo, lisEQP))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "filtros") return (false);
				}
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Filtros");

			// Salva dados dos reatores
			if (cronometro)
				cronometro->Start("Reatores");
			if (progresso)
				progresso->Add("Salvando reatores");
			lisEQP->Clear();
			redes->LisEqptoNaoIsolado(lisEQP, eqptoREATOR);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrReator(estudo, lisEQP))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "reatores") return (false);
				}
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Reatores");

			// Salva dados dos trafos ZZ
			if (cronometro)
				cronometro->Start("TrafoZZ");
			if (progresso)
				progresso->Add("Salvando transformadores Zig-Zag");
			lisEQP->Clear();
			redes->LisEqptoNaoIsolado(lisEQP, eqptoTRAFOZZ);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrTrafoZZ(estudo, lisEQP))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "trafos Zig Zag") return (false);
				}
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("TrafoZZ");

			// Salva dados dos medidores
			if (cronometro)
				cronometro->Start("Medidores");
			if (progresso)
				progresso->Add("Salvando medidores");
			lisEQP->Clear();
			redes->LisEqptoNaoIsolado(lisEQP, eqptoMEDIDOR);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrMedidor(estudo, lisEQP))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "medidores") return (false);
				}
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Medidores");

			// Salva dados dos canais
			if (cronometro)
				cronometro->Start("Canais");
			if (progresso)
				progresso->Add("Salvando canais");
			lisEQP->Clear();
			redes->LisEqptoNaoIsolado(lisEQP, eqptoCANAL);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrCanal(estudo, lisEQP))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "canais") return (false);
				}
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Canais");

			// IMPORTANTE: atualiza Status de todos os equipamentos
			// Como as barras e EqBars podem pertencer a mais de uma rede, a atualização
			// não pode ser feita dentro de cada loop.
			// Assim, para uniformizar o código, todas as atualizações são feitas ao final

			// atauliza status das redes
			bd_func->DefineStatusEqpto(lisREDE, false);
			// atualiza status dos eqptos NÃO ISOLADOS
			// lisEQP->Clear();
			// redes->LisEqpto(lisEQP);
			lisEQP->Clear();
			redes->LisEqptoNaoIsolado(lisEQP, eqptoBARRA);
			redes->LisEqptoNaoIsolado(lisEQP, eqptoEQBAR);
			redes->LisEqptoNaoIsolado(lisEQP, eqptoLIGACAO);
			redes->LisEqptoNaoIsolado(lisEQP, eqptoMUTUA);
			bd_func->DefineStatusEqpto(lisEQP, false);
//jss20191003
            if (reservado)
                progresso->Step();


			// Redefine os passos finais
			// Aumentar o número final, se aumentarem os passos finais
			if (progresso)
				progresso->Add("Fase final da transferência");

			// Salva as reduções da rede
			if (progresso)
				progresso->Add("Salvando reduções");
			if (cronometro)
				cronometro->Start("SalvaReducoes");
			if (!SalvaReducoes(estudo))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBDWR_WREQUIPREDE(strlog, "reduções") return (false);
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("SalvaReducoes");

			// Salvando manobras
			if (progresso)
				progresso->Add("Salvando manobras");
			if (cronometro)
				cronometro->Start("SalvaManobras");
			if (!SPSNP_wrSincronismo(estudo))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBDWR_WREQUIPREDE(strlog, "Sincronismo") return (false);
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (!SPSNP_wrManobra(estudo))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBDWR_WREQUIPREDE(strlog, "Manobras") return (false);
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("SalvaManobras");

			if (progresso)
				progresso->Add("Removendo itens eliminados na edição");
			if (cronometro)
				cronometro->Start("Removendo itens eliminados na edição");

			if (!TrataLixeira(estudo))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBDWR_LIXEIRA return (false);
			}
//jss20191003
            if (reservado)
                progresso->Step();

			if (cronometro)
				cronometro->Stop("Removendo itens eliminados na edição");

			if (cronometro)
				cronometro->Stop("SalvaRedeEstudo");
			if (cronometro)
				cronometro->Show();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
		}
	}
	__finally
	{
		if (progresso)
			progresso->Stop();

		// destrói lista sem destruir seus objetos
		if (lisEQP)
		{
			delete lisEQP;
			lisEQP = NULL;
		}
		if (lisREDEDOMS)
		{
			delete lisREDEDOMS;
			lisREDEDOMS = NULL;
		}
	}

	// Aviso("Dados salvos com sucesso - Modificar essa mensagem para o local correto.");
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SalvaRedeEstudoComo(VTEstudo *estudo)
{
	// Redefine os IDs para obter uma rede "reiniciada"
	RenovaStatus(estudo);

	return (SalvaRedeEstudo(estudo));
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SalvaListaDominio(TList *lisDOM)
{
	return (SPSNP_wrDominio(lisDOM));
}

// ---------------------------------------------------------------------------
int __fastcall TBDeqptoAccess::FNSNP_rdAcaoIdProx(VTEstudo *estudo, VTAlternativa* alternativa,
	VTObra* obra, bool bolLocaIndex, bool bolBeginNext)
{
	TADOQuery* ptrQryDS = NULL;
	AnsiString strSQL;
	AnsiString strlog;

	if (bolLocaIndex)
	{
		// índice local (ini)
		index.ndxAcaoId++;
		// índice local (fim)
	}
	else
	{
		// índice da base (ini)
		if (!bd_conn)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}
		if (!bd_conn->getConnectedOK())
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}
		if (!estudo)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_ESTUDOINV return (IDINDEFINIDO);
		}
		if (!alternativa)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_ALTERNATIVAINV return (IDINDEFINIDO);
		}

		__try
		{
			try
			{
				// Monta a string de execução
				strSQL = "EXEC FNSNP_rdAcaoIdProx :p_EstudoId, :p_AlternativaId, :p_ObraId";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (IDINDEFINIDO);
				}
				if (!bd_conn->SetParam("p_AlternativaId", PARAMIN_INT(alternativa->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (IDINDEFINIDO);
				}
				if (!bd_conn->SetParam("p_ObraId", PARAMIN_INT(obra->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (IDINDEFINIDO);
				}
				// Comando da consulta
				if (!bd_conn->Execute(true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				// Recupera o resultado
				if (ptrQryDS->RecordCount != 1)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				index.ndxAcaoId = ptrQryDS->FieldByName("IdProx")->AsInteger;
				// Começa a contagem somente a partir da próxima chamada
				if (bolBeginNext)
				{
					index.ndxAcaoId--;
				}

				ptrQryDS->Close();
			}
			catch (Exception& e)
			{
				SNPSTRLOGLINE(strlog);
				SNPSTRLOG(e.Message);
				index.ndxAcaoId = IDINDEFINIDO;
			}
		}
		__finally
		{
			// Colocado dentro do finally para garantir que será deletado, mesmo que
			// haja um return no meio do código

			// destrói objetos locais
			if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
			{
				delete ptrQryDS;
			}
		}
		// índice da base (fim)
	}

	return (index.ndxAcaoId);
}

// ---------------------------------------------------------------------------
int __fastcall TBDeqptoAccess::FNSNP_rdAgrupamentoIdProx(bool bolLocaIndex, bool bolBeginNext)
{
	TADOQuery* ptrQryDS = NULL;
	AnsiString strSQL;
	AnsiString strlog;

	if (bolLocaIndex)
	{
		// índice local (ini)
		index.ndxAgrupamentoId++;
		// índice local (fim)
	}
	else
	{
		// índice da base (ini)
		if (!bd_conn)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}
		if (!bd_conn->getConnectedOK())
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}

		__try
		{
			try
			{
				// Monta a string de execução
				strSQL = "EXEC FNSNP_rdAgrupamentoIdProx";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				// Comando da consulta
				if (!bd_conn->Execute(true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				// Recupera o resultado
				if (ptrQryDS->RecordCount != 1)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				index.ndxAgrupamentoId = ptrQryDS->FieldByName("IdProx")->AsInteger;
				// Começa a contagem somente a partir da próxima chamada
				if (bolBeginNext)
				{
					index.ndxAgrupamentoId--;
				}
				ptrQryDS->Close();
			}
			catch (Exception& e)
			{
				SNPSTRLOGLINE(strlog);
				SNPSTRLOG(e.Message);
				index.ndxAgrupamentoId = IDINDEFINIDO;
			}
		}
		__finally
		{
			// Colocado dentro do finally para garantir que será deletado, mesmo que
			// haja um return no meio do código

			// destrói objetos locais
			if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
			{
				delete ptrQryDS;
			}
		}
		// índice da base (fim)
	}

	return (index.ndxAgrupamentoId);
}

// ---------------------------------------------------------------------------
int __fastcall TBDeqptoAccess::FNSNP_rdAlternativaIdProx(VTEstudo *estudo, bool bolLocaIndex,
	bool bolBeginNext)
{
	TADOQuery* ptrQryDS = NULL;
	AnsiString strSQL;
	AnsiString strlog;

	if (bolLocaIndex)
	{
		// índice local (ini)
		index.ndxAlternativaId++;
		// índice local (fim)
	}
	else
	{
		// índice da base (ini)
		if (!bd_conn)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}
		if (!bd_conn->getConnectedOK())
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}
		if (!estudo)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_ESTUDOINV return (IDINDEFINIDO);
		}

		__try
		{
			try
			{
				// Monta a string de execução
				strSQL = "EXEC FNSNP_rdAlternativaIdProx :p_EstudoId";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (IDINDEFINIDO);
				}
				// Comando da consulta
				if (!bd_conn->Execute(true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				// Recupera o resultado
				if (ptrQryDS->RecordCount != 1)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				index.ndxAlternativaId = ptrQryDS->FieldByName("IdProx")->AsInteger;
				// Começa a contagem somente a partir da próxima chamada
				if (bolBeginNext)
				{
					index.ndxAlternativaId--;
				}
				ptrQryDS->Close();
			}
			catch (Exception& e)
			{
				SNPSTRLOGLINE(strlog);
				SNPSTRLOG(e.Message);
				index.ndxAlternativaId = IDINDEFINIDO;
			}
		}
		__finally
		{
			// Colocado dentro do finally para garantir que será deletado, mesmo que
			// haja um return no meio do código

			// destrói objetos locais
			if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
			{
				delete ptrQryDS;
			}
		}
		// índice da base (fim)
	}

	return (index.ndxAlternativaId);
}

// ---------------------------------------------------------------------------
int __fastcall TBDeqptoAccess::FNSNP_rdCorteIdProx(bool bolLocaIndex, bool bolBeginNext)
{
	TADOQuery* ptrQryDS = NULL;
	AnsiString strSQL;
	AnsiString strlog;

	if (bolLocaIndex)
	{
		// índice local (ini)
		index.ndxCorteId++;
		// índice local (fim)
	}
	else
	{
		// índice da base (ini)
		if (!bd_conn)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}
		if (!bd_conn->getConnectedOK())
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}

		__try
		{
			try
			{
				// Monta a string de execução
				strSQL = "EXEC FNSNP_rdCorteIdProx";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				// Comando da consulta
				if (!bd_conn->Execute(true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				// Recupera o resultado
				if (ptrQryDS->RecordCount != 1)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				index.ndxCorteId = ptrQryDS->FieldByName("IdProx")->AsInteger;
				// Começa a contagem somente a partir da próxima chamada
				if (bolBeginNext)
				{
					index.ndxCorteId--;
				}
				ptrQryDS->Close();
			}
			catch (Exception& e)
			{
				SNPSTRLOGLINE(strlog);
				SNPSTRLOG(e.Message);
				index.ndxCorteId = IDINDEFINIDO;
			}
		}
		__finally
		{
			// Colocado dentro do finally para garantir que será deletado, mesmo que
			// haja um return no meio do código

			// destrói objetos locais
			if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
			{
				delete ptrQryDS;
			}
		}
		// índice da base (fim)
	}

	return (index.ndxCorteId);
}

// ---------------------------------------------------------------------------
int __fastcall TBDeqptoAccess::FNSNP_rdDominioIdProx(bool bolLocaIndex, bool bolBeginNext)
{
	TADOQuery* ptrQryDS = NULL;
	AnsiString strSQL;
	AnsiString strlog;

	if (bolLocaIndex)
	{
		// índice local (ini)
		index.ndxDominioId++;
		// índice local (fim)
	}
	else
	{
		// índice da base (ini)
		if (!bd_conn)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}
		if (!bd_conn->getConnectedOK())
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}

		__try
		{
			try
			{
				// Monta a string de execução
				strSQL = "EXEC FNSNP_rdDominioIdProx";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				// Comando da consulta
				if (!bd_conn->Execute(true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				// Recupera o resultado
				if (ptrQryDS->RecordCount != 1)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				index.ndxDominioId = ptrQryDS->FieldByName("IdProx")->AsInteger;
				// Começa a contagem somente a partir da próxima chamada
				if (bolBeginNext)
				{
					index.ndxDominioId--;
				}
				ptrQryDS->Close();
			}
			catch (Exception& e)
			{
				SNPSTRLOGLINE(strlog);
				SNPSTRLOG(e.Message);
				index.ndxDominioId = IDINDEFINIDO;
			}
		}
		__finally
		{
			// Colocado dentro do finally para garantir que será deletado, mesmo que
			// haja um return no meio do código

			// destrói objetos locais
			if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
			{
				delete ptrQryDS;
			}
		}
		// índice da base (fim)
	}

	return (index.ndxDominioId);
}

// ---------------------------------------------------------------------------
int __fastcall TBDeqptoAccess::FNSNP_rdEstudoIdProx(bool bolLocaIndex, bool bolBeginNext)
{
	TADOQuery* ptrQryDS = NULL;
	AnsiString strSQL;
	AnsiString strlog;

	if (bolLocaIndex)
	{
		// índice local (ini)
		index.ndxEstudoId++;
		// índice local (fim)
	}
	else
	{
		// índice da base (ini)
		if (!bd_conn)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}
		if (!bd_conn->getConnectedOK())
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}

		__try
		{
			try
			{
				// Monta a string de execução
				strSQL = "EXEC FNSNP_rdEstudoIdProx";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				// Comando da consulta
				if (!bd_conn->Execute(true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				// Recupera o resultado
				if (ptrQryDS->RecordCount != 1)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				index.ndxEstudoId = ptrQryDS->FieldByName("IdProx")->AsInteger;
				// Começa a contagem somente a partir da próxima chamada
				if (bolBeginNext)
				{
					index.ndxEstudoId--;
				}
				ptrQryDS->Close();
			}
			catch (Exception& e)
			{
				SNPSTRLOGLINE(strlog);
				SNPSTRLOG(e.Message);
				index.ndxEstudoId = IDINDEFINIDO;
			}
		}
		__finally
		{
			// Colocado dentro do finally para garantir que será deletado, mesmo que
			// haja um return no meio do código

			// destrói objetos locais
			if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
			{
				delete ptrQryDS;
			}
		}
		// índice da base (fim)
	}

	return (index.ndxEstudoId);
}

// ---------------------------------------------------------------------------
int __fastcall TBDeqptoAccess::FNSNP_rdEquipIdProx(VTEstudo *estudo, bool bolLocaIndex,
	bool bolBeginNext)
{
	TADOQuery* ptrQryDS = NULL;
	AnsiString strSQL;
	AnsiString strlog;

	if (bolLocaIndex)
	{
		// índice local (ini)
		if (cronometro)
			cronometro->Start("IdLocal");
		index.ndxEquipId++;
		if (cronometro)
			cronometro->Stop("IdLocal");
		// índice local (fim)
	}
	else
	{
		if (cronometro)
			cronometro->Start("IdRemoto");
		// índice da base (ini)
		if (!bd_conn)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}
		if (!bd_conn->getConnectedOK())
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}
		if (!estudo)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_ESTUDOINV return (IDINDEFINIDO);
		}

		__try
		{
			try
			{
				// Monta a string de execução
				strSQL = "EXEC FNSNP_rdEquipIdProx :p_EstudoId";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				/*
				 //JCG 2013.07.16 - alterado novamente porque está causando erro
				 //JCG 2013.03.12 - utiliza sempre ID do Estudo 0 p/ garantir IDs sequenciais dos eqptos
				 //if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(IDINDEFINIDO); }
				 //if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudoID_ZERO))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(IDINDEFINIDO); }
				 */
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (IDINDEFINIDO);
				}
				// Comando da consulta
				if (!bd_conn->Execute(true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				// Recupera o resultado
				if (ptrQryDS->RecordCount != 1)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				index.ndxEquipId = ptrQryDS->FieldByName("IdProx")->AsInteger;
				// Começa a contagem somente a partir da próxima chamada
				if (bolBeginNext)
				{
					index.ndxEquipId--;
				}
				ptrQryDS->Close();
			}
			catch (Exception& e)
			{
				SNPSTRLOGLINE(strlog);
				SNPSTRLOG(e.Message);
				index.ndxEquipId = IDINDEFINIDO;
			}
		}
		__finally
		{
			// Colocado dentro do finally para garantir que será deletado, mesmo que
			// haja um return no meio do código

			// destrói objetos locais
			if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
			{
				delete ptrQryDS;
			}
		}
		// índice da base (fim)
		if (cronometro)
			cronometro->Stop("IdRemoto");
	}

	return (index.ndxEquipId);
}


// ---------------------------------------------------------------------------
int __fastcall TBDeqptoAccess::FNSNP_rdItemIdProx(VTEstudo* estudo, VTAlternativa* alternativa, VTObra *obra,
	bool bolLocaIndex, bool bolBeginNext)
{
	TADOQuery* ptrQryDS = NULL;
	AnsiString strSQL;
	AnsiString strlog;

	if (bolLocaIndex)
	{
		// índice local (ini)
		index.ndxItemId++;
		// índice local (fim)
	}
	else
	{
		// índice da base (ini)
		if (!bd_conn)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}
		if (!bd_conn->getConnectedOK())
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}
		if (!estudo)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_ESTUDOINV return (IDINDEFINIDO);
		}
		if (!alternativa)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_ALTERNATIVAINV return (IDINDEFINIDO);
		}

		__try
		{
			try
			{
				// Monta a string de execução
				strSQL = "EXEC FNSNP_rdItemIdProx :p_EstudoId, :p_AlternativaId, :p_ObraId";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (IDINDEFINIDO);
				}
				if (!bd_conn->SetParam("p_AlternativaId", PARAMIN_INT(alternativa->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (IDINDEFINIDO);
				}
				if (!bd_conn->SetParam("p_ObraId", PARAMIN_INT(obra->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (IDINDEFINIDO);
				}
				// Comando da consulta
				if (!bd_conn->Execute(true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				// Recupera o resultado
				if (ptrQryDS->RecordCount != 1)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				index.ndxItemId = ptrQryDS->FieldByName("IdProx")->AsInteger;
				// Começa a contagem somente a partir da próxima chamada
				if (bolBeginNext)
				{
					index.ndxItemId--;
				}
				ptrQryDS->Close();
			}
			catch (Exception& e)
			{
				SNPSTRLOGLINE(strlog);
				SNPSTRLOG(e.Message);
				index.ndxItemId = IDINDEFINIDO;
			}
		}
		__finally
		{
			// Colocado dentro do finally para garantir que será deletado, mesmo que
			// haja um return no meio do código

			// destrói objetos locais
			if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
			{
				delete ptrQryDS;
			}
		}
		// índice da base (fim)
	}

	return (index.ndxItemId);
}

// ---------------------------------------------------------------------------
int __fastcall TBDeqptoAccess::FNSNP_rdObraIdProx(VTEstudo* estudo, VTAlternativa* alternativa,
	bool bolLocaIndex, bool bolBeginNext)
{
	TADOQuery* ptrQryDS = NULL;
	AnsiString strSQL;
	AnsiString strlog;

	if (bolLocaIndex)
	{
		// índice local (ini)
		index.ndxObraId++;
		// índice local (fim)
	}
	else
	{
		// índice da base (ini)
		if (!bd_conn)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}
		if (!bd_conn->getConnectedOK())
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}
		if (!estudo)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_ESTUDOINV return (IDINDEFINIDO);
		}
		if (!alternativa)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_ALTERNATIVAINV return (IDINDEFINIDO);
		}

		__try
		{
			try
			{
				// Monta a string de execução
				strSQL = "EXEC FNSNP_rdObraIdProx :p_EstudoId, :p_AlternativaId";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (IDINDEFINIDO);
				}
				if (!bd_conn->SetParam("p_AlternativaId", PARAMIN_INT(alternativa->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (IDINDEFINIDO);
				}
				// Comando da consulta
				if (!bd_conn->Execute(true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				// Recupera o resultado
				if (ptrQryDS->RecordCount != 1)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				index.ndxObraId = ptrQryDS->FieldByName("IdProx")->AsInteger;
				// Começa a contagem somente a partir da próxima chamada
				if (bolBeginNext)
				{
					index.ndxObraId--;
				}
				ptrQryDS->Close();
			}
			catch (Exception& e)
			{
				SNPSTRLOGLINE(strlog);
				SNPSTRLOG(e.Message);
				index.ndxObraId = IDINDEFINIDO;
			}
		}
		__finally
		{
			// Colocado dentro do finally para garantir que será deletado, mesmo que
			// haja um return no meio do código

			// destrói objetos locais
			if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
			{
				delete ptrQryDS;
			}
		}
		// índice da base (fim)
	}

	return (index.ndxObraId);
}

// ---------------------------------------------------------------------------
int __fastcall TBDeqptoAccess::FNSNP_rdManobraIdProx(bool bolLocaIndex, bool bolBeginNext)
{
	TADOQuery* ptrQryDS = NULL;
	AnsiString strSQL;
	AnsiString strlog;

	if (bolLocaIndex)
	{
		// índice local (ini)
		index.ndxManobraId++;
		// índice local (fim)
	}
	else
	{
		// índice da base (ini)
		if (!bd_conn)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}
		if (!bd_conn->getConnectedOK())
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}

		__try
		{
			try
			{
				// Monta a string de execução
				strSQL = "EXEC FNSNP_rdManobraIdProx";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				// Comando da consulta
				if (!bd_conn->Execute(true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				// Recupera o resultado
				if (ptrQryDS->RecordCount != 1)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				index.ndxManobraId = ptrQryDS->FieldByName("IdProx")->AsInteger;
				// Começa a contagem somente a partir da próxima chamada
				if (bolBeginNext)
				{
					index.ndxManobraId--;
				}
				ptrQryDS->Close();
			}
			catch (Exception& e)
			{
				SNPSTRLOGLINE(strlog);
				SNPSTRLOG(e.Message);
				index.ndxManobraId = IDINDEFINIDO;
			}
		}
		__finally
		{
			// Colocado dentro do finally para garantir que será deletado, mesmo que
			// haja um return no meio do código

			// destrói objetos locais
			if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
			{
				delete ptrQryDS;
			}
		}
		// índice da base (fim)
	}

	return (index.ndxManobraId);
}

// ---------------------------------------------------------------------------
int __fastcall TBDeqptoAccess::FNSNP_rdRedeDominioCountDominio(VTEstudo* estudo, VTDominio* dominio)
{
	int intret = IDINDEFINIDO;
	TADOQuery* ptrQryDS = NULL;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (intret);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (intret);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!dominio)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_DOMINIOINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "EXEC FNSNP_rdRedeDominioCountDominio :p_EstudoId, :p_DominioId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_DominioId", PARAMIN_INT(dominio->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (intret);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (intret);
			}
			// Recupera o resultado
			if (ptrQryDS->RecordCount != 1)
			{
				SNPSTRLOGLINE(strlog);
				return (intret);
			}
			intret = ptrQryDS->FieldByName("Count")->AsInteger;
			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			intret = IDINDEFINIDO;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (intret);
}

// ---------------------------------------------------------------------------
int __fastcall TBDeqptoAccess::FNSNP_rdRedeDominioCountRede(VTDominio* dominio)
{
	int intret = IDINDEFINIDO;
	TADOQuery* ptrQryDS = NULL;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (intret);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (intret);
	}
	if (!dominio)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_DOMINIOINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "EXEC FNSNP_rdRedeDominioCountRede :p_DominioId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_DominioId", PARAMIN_INT(dominio->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (intret);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (intret);
			}
			// Recupera o resultado
			if (ptrQryDS->RecordCount != 1)
			{
				SNPSTRLOGLINE(strlog);
				return (intret);
			}
			intret = ptrQryDS->FieldByName("Count")->AsInteger;
			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			intret = IDINDEFINIDO;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (intret);
}

// ---------------------------------------------------------------------------
int __fastcall TBDeqptoAccess::FNSNP_rdSincronismoIdProx(bool bolLocaIndex, bool bolBeginNext)
{
	TADOQuery* ptrQryDS = NULL;
	AnsiString strSQL;
	AnsiString strlog;

	if (bolLocaIndex)
	{
		// índice local (ini)
		index.ndxSincronismoId++;
		// índice local (fim)
	}
	else
	{
		// índice da base (ini)
		if (!bd_conn)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}
		if (!bd_conn->getConnectedOK())
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_SESSIONCON return (IDINDEFINIDO);
		}

		__try
		{
			try
			{
				// Monta a string de execução
				strSQL = "EXEC FNSNP_rdSincronismoIdProx";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				// Comando da consulta
				if (!bd_conn->Execute(true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				// Recupera o resultado
				if (ptrQryDS->RecordCount != 1)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_EXECINV return (IDINDEFINIDO);
				}
				index.ndxSincronismoId = ptrQryDS->FieldByName("IdProx")->AsInteger;
				// Começa a contagem somente a partir da próxima chamada
				if (bolBeginNext)
				{
					index.ndxSincronismoId--;
				}
				ptrQryDS->Close();
			}
			catch (Exception& e)
			{
				SNPSTRLOGLINE(strlog);
				SNPSTRLOG(e.Message);
				index.ndxSincronismoId = IDINDEFINIDO;
			}
		}
		__finally
		{
			// Colocado dentro do finally para garantir que será deletado, mesmo que
			// haja um return no meio do código

			// destrói objetos locais
			if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
			{
				delete ptrQryDS;
			}
		}
		// índice da base (fim)
	}

	return (index.ndxSincronismoId);
}

// ---------------------------------------------------------------------------
int __fastcall TBDeqptoAccess::FNSNP_rdTipoEqligCount(VTEstudo* estudo, int intRedeId,
	int intTipoEquipId)
{
	int intret = IDINDEFINIDO;
	TADOQuery* ptrQryDS = NULL;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (intret);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (intret);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (intRedeId != IDINDEFINIDO)
			{
				strSQL = "EXEC FNSNP_rdTipoEqligCount_p01 :p_EstudoId, :p_TipoEquipId, :p_RedeId";
			}
			else
			{
				strSQL = "EXEC FNSNP_rdTipoEqligCount_p02 :p_EstudoId, :p_TipoEquipId";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_TipoEquipId", PARAMIN_INT(intTipoEquipId)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (intRedeId != IDINDEFINIDO)
			{
				if (!bd_conn->SetParam("p_RedeId", PARAMIN_INT(intRedeId)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
			}
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (intret);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (intret);
			}
			// Recupera o resultado
			if (ptrQryDS->RecordCount != 1)
			{
				SNPSTRLOGLINE(strlog);
				return (intret);
			}
			intret = ptrQryDS->FieldByName("TipoEquipCount")->AsInteger;
			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			intret = IDINDEFINIDO;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (intret);
}

// ---------------------------------------------------------------------------
int __fastcall TBDeqptoAccess::FNSNP_rdTipoEqbarCount(VTEstudo* estudo, int intRedeId,
	int intTipoEquipId)
{
	int intret = IDINDEFINIDO;
	TADOQuery* ptrQryDS = NULL;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (intret);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (intret);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (intRedeId != IDINDEFINIDO)
			{
				strSQL = "EXEC FNSNP_rdTipoEqbarCount_p01 :p_EstudoId, :p_TipoEquipId, :p_RedeId";
			}
			else
			{
				strSQL = "EXEC FNSNP_rdTipoEqbarCount_p02 :p_EstudoId, :p_TipoEquipId";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_TipoEquipId", PARAMIN_INT(intTipoEquipId)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (intRedeId != IDINDEFINIDO)
			{
				if (!bd_conn->SetParam("p_RedeId", PARAMIN_INT(intRedeId)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
			}
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (intret);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (intret);
			}
			// Recupera o resultado
			if (ptrQryDS->RecordCount != 1)
			{
				SNPSTRLOGLINE(strlog);
				return (intret);
			}
			intret = ptrQryDS->FieldByName("TipoEquipCount")->AsInteger;
			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			intret = IDINDEFINIDO;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (intret);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SalvaReducoes(VTEstudo *estudo)
{
	if (estudo->Id == 0)
		return (SalvaReducoesEstudoZero(estudo));
	else
		return (SalvaReducoesEstudoNaoZero(estudo));
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SalvaReducoesEstudoZero(VTEstudo *estudo)
{
	bool sucesso = true;
	AnsiString strlog;
	TList* lisMREDE = NULL;
	TList* lisREDU = NULL;
	VTMRede* mrede = NULL;
	VTReducao* reducao;
	TList* lisEQP = NULL;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}

	__try
	{
		lisMREDE = new TList();
		// Retirado em 20120102, porque salva os cortes de todas as metaredes
		// (inclusive redes que só tenham reduções).
		// obtém lista de MRedes carregadas
		// redes->LisMRedeCarregada(lisMREDE);
		// obtém lista das MRedes (não somente as carregadas)
		redes->LisMRede(lisMREDE);
		// verifica se o Estudo já existe na base de dados
		if (!estudo->Novo)
		{ // elimina Cortes e Eqptos das MRedes carregadas cujas Reducoes foram recalculadas
			if (cronometro)
				cronometro->Start("SPSNP_dlEstudoCorte");
			if (!SPSNP_dlEstudoCorte(estudo, lisMREDE))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBDDL_CORTES return (false);
			}
			if (cronometro)
				cronometro->Stop("SPSNP_dlEstudoCorte");
		}

		if (cronometro)
			cronometro->Start("SPSNP_wrEstudoCorte");
		if (!SPSNP_wrEstudoCorteZero(estudo, lisMREDE))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDWR_CORTES return (false);
		}
		if (cronometro)
			cronometro->Stop("SPSNP_wrEstudoCorte");

		// Reinicia o indexador de equipamentos
		FNSNP_rdEquipIdProx(estudo, false, true);
		// Reinicia o indexador de curvas
		bd_curva->FNSNP_rdConjCurvaIdProx(false, true);

		// Primeiro salva os equipamentos reduzidos em modo batch
		// Somente possível para cargas e suprimentos (não contém Blobs)
		if (cronometro)
			cronometro->Start("SPSNP_wrRRCarga");
		if (!SPSNP_wrRRCarga(estudo, lisMREDE))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDWR_RRCARGA return (false);
		}
		if (cronometro)
			cronometro->Stop("SPSNP_wrRRCarga");

		if (cronometro)
			cronometro->Start("SPSNP_wrRRSuprimento");
		if (!SPSNP_wrRRSuprimento(estudo, lisMREDE))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDWR_RRSUPRIMENTO return (false);
		}
		if (cronometro)
			cronometro->Stop("SPSNP_wrRRSuprimento");

		// Em seguida, salva o restante das reduções
		lisEQP = new TList();
		for (int i = 0; i < lisMREDE->Count; i++)
		{
			if ((mrede = (VTMRede*)lisMREDE->Items[i]) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_MREDEINV sucesso = false;
				break;
			}
			// obtém lista de Reduções da MREde
			if ((lisREDU = mrede->LisReducao()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_LISREDUINV sucesso = false;
				break;
			}

			for (int i = 0; i < lisREDU->Count; i++)
			{
				if ((reducao = (VTReducao*)lisREDU->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_REDUCAOINV return (false);
				}
				if (!reducao->MRede)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MREDEINV return (false);
				}

				lisEQP->Clear();
				reducao->LisEqpto(lisEQP, eqptoYREF);
				if (lisEQP->Count > 0)
				{
					if (cronometro)
						cronometro->Start("SPSNP_wrRRYRef");
					if (!SPSNP_wrRRYRef(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBDWR_RRYREF return (false);
					}
					if (cronometro)
						cronometro->Stop("SPSNP_wrRRYRef");
				}

				lisEQP->Clear();
				reducao->LisEqpto(lisEQP, eqptoTRAFO);
				if (lisEQP->Count > 0)
				{
					if (cronometro)
						cronometro->Start("SPSNP_wrRRTrafo");
					if (!SPSNP_wrRRTrafo(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBDWR_RRTRAFO return (false);
					}
					if (cronometro)
						cronometro->Stop("SPSNP_wrRRTrafo");
				}
			}
		}
	}
	__finally
	{
		// destrói lista sem destruir seus objetos
		if (lisEQP)
		{
			delete lisEQP;
			lisEQP = NULL;
		}

		if (lisMREDE)
		{
			delete lisMREDE;
			lisMREDE = NULL;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SalvaReducoesEstudoNaoZero(VTEstudo *estudo)
{
	bool sucesso = true;
	AnsiString strlog;
	TList* lisMREDE = NULL;
	TList* lisREDU = NULL;
	VTMRede* mrede = NULL;
	VTReducao* reducao;
	TList* lisEQP = NULL;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}

	__try
	{
		lisEQP = new TList();
		lisMREDE = new TList();
		// grava Cortes das MRedes
		redes->LisMRede(lisMREDE);
		if (!SPSNP_wrEstudoCorteNaoZero(estudo, lisMREDE))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDWR_CORTES return (false);
		}
		// obtém lista de MRedes reduzidas
		redes->LisMRedeReduzida(lisMREDE);
		// Reinicia o indexador de equipamentos
		FNSNP_rdEquipIdProx(estudo, false, true);
		// Reinicia o indexador de curvas
		bd_curva->FNSNP_rdConjCurvaIdProx(false, true);

		// Primeiro salva os equipamentos reduzidos em modo batch
		// Somente possível para cargas e suprimentos (não contém Blobs)
		if (!SPSNP_wrRRCarga(estudo, lisMREDE))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDWR_RRCARGA return (false);
		}

		if (!SPSNP_wrRRSuprimento(estudo, lisMREDE))
		{
			SNPSTRLOGLINE(strlog);
			MSGERRBDWR_RRSUPRIMENTO return (false);
		}

		// Em seguida, salva o restante das reduções
		for (int i = 0; i < lisMREDE->Count; i++)
		{
			if ((mrede = (VTMRede*)lisMREDE->Items[i]) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_MREDEINV sucesso = false;
				break;
			}

			// obtém lista de Reduções da MREde (que é única)
			if ((lisREDU = mrede->LisReducao()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_LISREDUINV sucesso = false;
				break;
			}

			for (int i = 0; i < lisREDU->Count; i++)
			{
				if ((reducao = (VTReducao*)lisREDU->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_REDUCAOINV return (false);
				}

				if (!reducao->MRede)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MREDEINV return (false);
				}

				lisEQP->Clear();
				reducao->LisEqpto(lisEQP, eqptoYREF);
				if (lisEQP->Count > 0)
				{
					if (!SPSNP_wrRRYRef(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBDWR_RRYREF return (false);
					}
				}

				lisEQP->Clear();
				reducao->LisEqpto(lisEQP, eqptoTRAFO);
				if (lisEQP->Count > 0)
				{
					if (!SPSNP_wrRRTrafo(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBDWR_RRTRAFO return (false);
					}
				}

			}
		}
	}
	__finally
	{
		// destrói lista sem destruir seus objetos
		if (lisEQP)
		{
			delete lisEQP;
			lisEQP = NULL;
		}
		if (lisMREDE)
		{
			delete lisMREDE;
			lisMREDE = NULL;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlDominio(VTEstudo *estudo, VTDominio* dominio)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	TList* lisEqpto = NULL;
	VTRede* rede = NULL;
	VTBarra* barra = NULL;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!dominio)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_DOMINIOINV return (false);
	}

	if (FNSNP_rdRedeDominioCountDominio(estudo, dominio) > 1)
	{
		Aviso("Uma ou mais redes fazem parte de domínios distintos.");
		return (false);
	}

	__try
	{
		lisEqpto = new TList();

		try
		{
			// Remove os registros da tabela RedeDominio
			// Monta a string de execução
			strSQL = "EXEC SPSNP_dlDominio_p01 :p_EstudoId, :p_DominioId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_DominioId", PARAMIN_INT(dominio->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Remove os registros das tabelas de rede do estudo
			// Determina a lista de barras do estudo
			lisEqpto->Clear();
			estudo->LisEqptoEstudo(lisEqpto, NULL, eqptoREDE);
			for (int i = 0; i < lisEqpto->Count; i++)
			{
				if ((rede = (VTRede*)lisEqpto->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!SPSNP_dlEstudoRede(estudo, rede))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
			}

			// Remove os registros das tabelas de barras do estudo
			// Determina a lista de barras do estudo
			lisEqpto->Clear();
			estudo->LisEqptoEstudo(lisEqpto, NULL, eqptoBARRA);
			for (int i = 0; i < lisEqpto->Count; i++)
			{
				if ((barra = (VTBarra*)lisEqpto->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				for (int j = 3; j <= 4; j++)
				{
					// Monta a string de execução
					strSQL = Format("EXEC SPSNP_dlDominio_p%.2d :p_EstudoId, :p_BarraId",
						ARRAYOFCONST((i)));
					// Decodifica comando
					bd_conn->ParseCmd(strSQL);
					// Determina os valores dos parâmetros
					if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_BarraId", PARAMIN_INT(barra->Id)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					// Comando da consulta
					if (!bd_conn->Execute(false))
					{
						SNPSTRLOGLINE(strlog);
						return (false);
					}
				}
			}

			// Remove o domínio se não houver mais ninguém associado
			if (FNSNP_rdRedeDominioCountRede(dominio) <= 1)
			{
				// Monta a string de execução
				strSQL = "EXEC SPSNP_dlDominio_p05 :p_DominioId";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_DominioId", PARAMIN_INT(dominio->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
		if (lisEqpto)
		{
			delete lisEqpto;
			lisEqpto = NULL;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlEquipamentoBarra(VTEstudo* estudo, VTEqpto* eqpto)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTMRede* mrede;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!eqpto)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}

	__try
	{
		try
		{
			// Passo 01
			// Monta a string de execução
			strSQL = "EXEC SPSNP_dlEquipamentoBarra_p01 :p_EstudoId, :p_EquipId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (NULL);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlEstudo(VTEstudo* estudo, bool bolRedeInclusive)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}

	__try
	{

		try
		{
			// Executa os 7 passos da deleção
			// mml@20130217 - Alterado de 6 para 7 passos, pois agora inclui a tabela obraAtribExt
			// As comparações com os índices abaixo também foram corrigidas
			for (int i = 1; i <= 7; i++)
			{
				// Somente executa os passos 6 e 7 se bolRedeInclusive for true
				if ((!bolRedeInclusive) && (i >= 6))
					break;
				// A execução do passo 4 é diferente das demais, pois corresponde à chamada
				// de uma função.
				if (i == 5)
				{
					if (!SPSNP_dlEstudoEquip(estudo, bolRedeInclusive))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
				}
				else
				{
					// Monta a string de execução
					strSQL = Format("EXEC SPSNP_dlEstudo_p%.2d :p_EstudoId", ARRAYOFCONST((i)));
					// Decodifica comando
					bd_conn->ParseCmd(strSQL);
					// Determina os valores dos parâmetros
					if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					// Comando da consulta
					if (!bd_conn->Execute(false))
					{
						SNPSTRLOGLINE(strlog);
						return (false);
					}
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlEquipTmpEquipCurva(VTEstudo* estudo)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTMRede* mrede;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	// mml@20141212 - Antes o estudo não era passado
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}

	__try
	{
		try
		{
			// Passo 01
			// Monta a string de execução
			// mml@20141212 - Antes o estudo não era passado
			strSQL = "EXEC SPSNP_dlEquipTmpEquipCurva :p_EstudoId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (NULL);
			}
			// Monta a string de execução
			strSQL = "EXEC SPSNP_dlConjCurvaSegTmpEquipCurva";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (NULL);
			}
			// Monta a string de execução
			strSQL = "EXEC SPSNP_dlConjCurvaTmpEquipCurva";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (NULL);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlEstudoCorte(VTEstudo* estudo, TList* lisMRede)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTMRede* mrede;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisMRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISMREDEINV return (false);
	}

	__try
	{
		try
		{
			// Passo 00
			// Precisa salvar os equipamentos e as curvas a serem apagadas no final
			if (!SPSNP_dlTmpEquipCurva())
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_LISMREDEINV return (false);
			}
			// O armazenamento dos equipamentos é feito no loop das mredes

			// Passo 01
			// Preenche a tabela temporária
			if (!SPSNP_dlTmpEquip())
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_LISMREDEINV return (false);
			}
			for (int i = 0; i < lisMRede->Count; i++)
			{
				if ((mrede = (VTMRede*)lisMRede->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MREDEINV return (false);
				}
				// verifica se as Reducoes e os Cortes da MRede devem ser excluídos
				// if ((! mrede->Carregada)||(! mrede->Status[sttENABLED])) continue;
				// if (mrede->StatusNovo() && (mrede->Id != IDINDEFINIDO))
				if (mrede->Carregada)
				{ // os Cortes da MRede devem ser excluídos
					if (!SPSNP_wrTmpEquip(mrede->Id))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_LISMREDEINV return (false);
					}
					// Completa passo 00
					if (!SPSNP_wrMRedeRREquipTmpEquipCurva(estudo, mrede->Id))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_LISMREDEINV return (false);
					}
				}
			}
			// Passo 02
			// Monta a string de execução
			for (int i = 1; i <= 6; i++)
			{
				// Loop procedures (ini)
				strSQL = Format("EXEC SPSNP_dlEstudoCorte_p%.2d :p_EstudoId", ARRAYOFCONST((i)));
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (NULL);
				}
				// Loop procedures (fim)
			}

			// Passo 03
			// Limpa os equipamentos e curvas salvos nos passos 00 e 01. Esses itens estão
			// armazenados na tabelaTmpEquipCurva
			if (!SPSNP_dlEquipTmpEquipCurva(estudo))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_LISMREDEINV return (false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlEstudoCorte(VTEstudo* estudo, int intMRedeId)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTMRede* mrede;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}

	__try
	{
		try
		{
			// Passo 01
			// Monta a string de execução
			for (int i = 7; i <= 12; i++)
			{
				// Loop procedures (ini)
				strSQL = Format("EXEC SPSNP_dlEstudoCorte_p%.2d :p_EstudoId :p_MRedeId",
					ARRAYOFCONST((i)));
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_MRedeId", PARAMIN_INT(intMRedeId)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (NULL);
				}
				// Loop procedures (fim)
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlEstudoEquip(VTEstudo* estudo, bool bolRedeInclusive)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTEqpto* eqpto;
	bool bolCurvaPropria;
	VTCurva* curva;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}

	__try
	{
		try
		{
			// [04 - 10] Remove as reduções (RREquipamentoBarra, RRYRef, RRTrafo, RRSuprimento, RRCarga), EquipamentoImagem e EquipamentoBarra
			for (int i = 1; i <= 35; i++)
			{
				// Monta a string de execução
				strSQL = Format("EXEC SPSNP_dlEstudoEquip_p%.2d :p_EstudoId, :p_RedeInclusive",
					ARRAYOFCONST((i)));
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_RedeInclusive", PARAMIN_INT(bolRedeInclusive ? 1 : 0)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}
			}

			// As curvas são apagadas de acordo com a lista de equipamentos do estudo
			// Porém, essa lista é montada na base de dados, a partir do estudo (todos
			// equipamentos do estudo são carregados na tabela temporária)
			if (!SPSNP_dlTmpEquip())
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!SPSNP_wrTmpEquipEstudo(estudo->Id))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!SPSNP_dlTmpCurva())
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!SPSNP_wrTmpCurva(estudo->Id))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Depois de salvar quais as curvas devem ser salvas, apaga os equipamento
			// Monta a string de execução
			strSQL = "EXEC SPSNP_dlEstudoEquip_p37 :p_EstudoId, :p_RedeInclusive";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_RedeInclusive", PARAMIN_INT(bolRedeInclusive ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			strSQL = "EXEC SPSNP_dlEstudoEquip_p38";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			// Executa o comando
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			strSQL = "EXEC SPSNP_dlEstudoEquip_p39";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			// Executa o comando
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlEstudoRede(VTEstudo *estudo, VTRede* rede)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	TList* lisEqpto = NULL;
	VTEqpto* eqpto;
	bool bolCurvaPropria;
	VTCurva* curva;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!rede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDEINV return (false);
	}

	__try
	{
		lisEqpto = new TList();

		try
		{
			rede->LisEqpto(lisEqpto);

			// BarraRede
			strSQL = "EXEC SPSNP_dlEstudoRede_p02 :p_EstudoId, :p_RedeId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Seta os parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_RedeId", PARAMIN_INT(rede->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Executa o comando
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Remoções por equipamento
			for (int i = 0; i < lisEqpto->Count; i++)
			{
				// Loop equipamentos rede (ini)
				if ((eqpto = (VTEqpto*)lisEqpto->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// [04 - 10] Remove as reduções (RREquipamentoBarra, RRYRef, RRTrafo, RRSuprimento, RRCarga), EquipamentoImagem e EquipamentoBarra
				for (int j = 4; j <= 10; j++)
				{
					// Monta a string de execução
					strSQL = Format("EXEC SPSNP_dlEstudoRede_p%.2d :p_EstudoId, :p_EqptoId",
						ARRAYOFCONST((j)));
					// Decodifica comando
					bd_conn->ParseCmd(strSQL);
					// Determina os valores dos parâmetros
					if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_EqptoId", PARAMIN_INT(eqpto->Id)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					// Comando da consulta
					if (!bd_conn->Execute(false))
					{
						SNPSTRLOGLINE(strlog);
						return (false);
					}
				}

				switch (eqpto->Tipo())
				{
				case eqptoINDEF:
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				case eqptoARRANJO:
					continue;
				case eqptoBARRA:
					strSQL = "EXEC SPSNP_dlEstudoRede_p11 :p_EstudoId, :p_EqptoId";
					break;
				case eqptoEQBAR:
					continue;
				case eqptoCABO:
					continue;
				case eqptoCAPACITOR:
					strSQL = "EXEC SPSNP_dlEstudoRede_p12 :p_EstudoId, :p_EqptoId";
					break;
				case eqptoCAPSERIE:
					strSQL = "EXEC SPSNP_dlEstudoRede_p13 :p_EstudoId, :p_EqptoId";
					break;
				case eqptoCARGA
						: // Cargas são um caso especial. Tem que retirar da tabela Carga e da tabela CargaIP.
					strSQL = "EXEC SPSNP_dlEstudoRede_p14 :p_EstudoId, :p_EqptoId";
					// Decodifica comando
					bd_conn->ParseCmd(strSQL);
					// Seta os parâmetros
					if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_EqptoId", PARAMIN_INT(eqpto->Id)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					// Comando da consulta
					if (!bd_conn->Execute(false))
					{
						SNPSTRLOGLINE(strlog);
						return (false);
					}
					// Prepara para remover a carga
					strSQL = "EXEC SPSNP_dlEstudoRede_p15 :p_EstudoId, :p_EqptoId";
					break;
				case eqptoCORTE:
					continue;
				case eqptoCHAVE:
					strSQL = "EXEC SPSNP_dlEstudoRede_p31 :p_EstudoId, :p_EqptoId";
					break;
				case eqptoGERADOR:
					strSQL = "EXEC SPSNP_dlEstudoRede_p16 :p_EstudoId, :p_EqptoId";
					break;
				case eqptoFILTRO:
					strSQL = "EXEC SPSNP_dlEstudoRede_p17 :p_EstudoId, :p_EqptoId";
					break;
				case eqptoLIGACAO:
					continue;
				case eqptoMEDIDOR:
					strSQL = "EXEC SPSNP_dlEstudoRede_p18 :p_EstudoId, :p_EqptoId";
					break;
				case eqptoMREDE:
					continue;
				case eqptoMUTUA:
					strSQL = "EXEC SPSNP_dlEstudoRede_p19 :p_EstudoId, :p_EqptoId";
					break;
				case eqptoNO:
					continue;
				case eqptoNET:
					continue;
				case eqptoREATOR:
					strSQL = "EXEC SPSNP_dlEstudoRede_p20 :p_EstudoId, :p_EqptoId";
					break;
				case eqptoREDE:
					continue;
					// strSQL = "EXEC SPSNP_dlEstudoRede_p21 :p_EstudoId, :p_EqptoId"; break;
				case eqptoREDUCAO:
					continue;
				case eqptoREGULADOR:
					strSQL = "EXEC SPSNP_dlEstudoRede_p22 :p_EstudoId, :p_EqptoId";
					break;
				case eqptoSUPORTE:
					continue;
				case eqptoSUPRIMENTO:
					strSQL = "EXEC SPSNP_dlEstudoRede_p23 :p_EstudoId, :p_EqptoId";
					break;
				case eqptoTRAFO
						: // Trafos são um caso especial. Tem que retirar os enrolamentos antes.
					strSQL = "EXEC SPSNP_dlEstudoRede_p24 :p_EstudoId, :p_EqptoId";
					// Decodifica comando
					bd_conn->ParseCmd(strSQL);
					// Seta os parâmetros
					if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_EqptoId", PARAMIN_INT(eqpto->Id)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					// Executa o comando
					if (!bd_conn->Execute(false))
					{
						SNPSTRLOGLINE(strlog);
						return (false);
					}
					// Prepara para remover o Trafo
					strSQL = "EXEC SPSNP_dlEstudoRede_p25 :p_EstudoId, :p_EqptoId";
					break;
				case eqptoTRAFO3E
						: // Trafos são um caso especial. Tem que retirar os enrolamentos antes.
					strSQL = "EXEC SPSNP_dlEstudoRede_p24 :p_EstudoId, :p_EqptoId";
					// Decodifica comando
					bd_conn->ParseCmd(strSQL);
					// Seta os parâmetros
					if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_EqptoId", PARAMIN_INT(eqpto->Id)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					// Executa o comando
					if (!bd_conn->Execute(false))
					{
						SNPSTRLOGLINE(strlog);
						return (false);
					}
					// Prepara para remover o Trafo3E
					strSQL = "EXEC SPSNP_dlEstudoRede_p27 :p_EstudoId, :p_EqptoId";
					break;
				case eqptoTRAFOZZ:
					strSQL = "EXEC SPSNP_dlEstudoRede_p28 :p_EstudoId, :p_EqptoId";
				case eqptoTRAMO:
					continue;
				case eqptoTRECHO: // Aqui tem que remover os arranjos antes??
					if (((VTTrecho*)eqpto)->arranjo == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					// Se o arranjo não é típico detona o arranjo
					if (!((VTTrecho*)eqpto)->ArranjoTipico)
					{
						for (int k = 50; k <= 53; k++)
						{
							// Monta a string de execução
							strSQL = Format("EXEC SPSNP_dlEstudoRede_p%.2d :p_ArranjoId",
								ARRAYOFCONST((k)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_ArranjoId",
								PARAMIN_INT(((VTTrecho*)eqpto)->arranjo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Comando da consulta
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}
					}
					strSQL = "EXEC SPSNP_dlEstudoRede_p30 :p_EstudoId, :p_EqptoId";
				case eqptoYREF:
					continue;
				case eqptoARRANJOS:
					continue;
				case eqptoCLUSTER:
					continue; // Não remove
				case eqptoCANAL:
					strSQL = "EXEC SPSNP_dlEstudoRede_p32 :p_EstudoId, :p_EqptoId";
				case eqptoDOMINIO:
					continue;
				case eqptoCARGANL:
					strSQL = "EXEC SPSNP_dlEstudoRede_p33 :p_EstudoId, :p_EqptoId";
				default:
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Seta os parâmetros
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EqptoId", PARAMIN_INT(eqpto->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// Executa o comando
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}
				// Loop equipamentos rede (fim)
			}

			// [35 - 39] Elimina o que tem a MRede como sendo a rede passada como parâmetro
			// [   40  ] Elimina os cortes da rede
			// [   41  ] Elimina da tabela rede
			// [   42  ] Elimina da tabela equipamento
			for (int i = 35; i <= 42; i++)
			{
				strSQL = Format("EXEC SPSNP_dlEstudoRede_p%.2d :p_EstudoId, :p_RedeId",
					ARRAYOFCONST((i)));
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Seta os parâmetros
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_RedeId", PARAMIN_INT(rede->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// Executa o comando
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}
			}

			// ATENÇÃO: não foi implementada a procedure 43.
			// Mesmo que a rede não exista mais, o id meta rede permanecerá (funciona com um "cluster").
			// Atualiza as demais redes que possuíam a rede anterior como MRede
			// SELECT COUNT(*) INTO conta_mredes FROM Rede WHERE intEstudoId = [p_EstudoId] AND intMRedeId = [p_RedeId];
			// IF (conta_mredes > 0) THEN
			// SELECT IIF(ISNULL(intRedeId, 0) INTO mrede_id FROM Rede WHERE intEstudoId = [p_EstudoId] AND intMRedeId = [p_RedeId] AND ROWNUM = 1;
			// UPDATE Rede SET intMRedeId = mrede_id WHERE intEstudoId = [p_EstudoId] AND intMRedeId = [p_RedeId];
			// END IF;

			// Finalizando, elimina todas as curvas próprias dos equipamentos que possuírem
			for (int i = 0; i < lisEqpto->Count; i++)
			{
				// Loop equipamentos rede (ini)
				if ((eqpto = (VTEqpto*)lisEqpto->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				bolCurvaPropria = false;
				if ((curva = bd_curva->CurvaEqpto(eqpto)) != NULL)
				{
					if (!curva->Tipica)
					{
						bolCurvaPropria = true;
					};
				}
				// Elimina a curva, se for própria
				if ((bolCurvaPropria) && (curva))
				{
					// Elimina CurvaSegmento e ConjuntoCurva
					for (int i = 45; i <= 46; i++)
					{
						strSQL = Format("EXEC SPSNP_dlEstudoRede_p%.2d :p_CurvaId",
							ARRAYOFCONST((i)));
						// Decodifica comando
						bd_conn->ParseCmd(strSQL);
						// Seta os parâmetros
						if (!bd_conn->SetParam("p_CurvaId", PARAMIN_INT(curva->Id)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						// Executa o comando
						if (!bd_conn->Execute(false))
						{
							SNPSTRLOGLINE(strlog);
							return (false);
						}
					}
				}
				// Loop equipamentos rede (fim)
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
		if (lisEqpto)
		{
			delete lisEqpto;
			lisEqpto = NULL;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlManobra(VTManobra* manobra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!manobra)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_MANOBRAINV return (false);
	}

	// Apaga as chaves das manobras antes
	if (!SPSNP_dlManobraChave(manobra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_MANOBRAINV return (false);
	}

	__try
	{
		try
		{
			// Passo 01
			// Monta a string de execução
			strSQL = "EXEC SPSNP_dlManobra :p_ManobraId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_ManobraId", PARAMIN_INT(manobra->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (NULL);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlManobraChave(VTManobra* manobra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!manobra)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_MANOBRAINV return (false);
	}

	__try
	{
		try
		{
			// Passo 01
			// Monta a string de execução
			strSQL = "EXEC SPSNP_dlManobraChave :p_ManobraId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_ManobraId", PARAMIN_INT(manobra->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (NULL);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlMedidor(VTEstudo* estudo, VTEqpto* eqpto)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTMRede* mrede;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!eqpto)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}

	__try
	{
		try
		{
			// Devem ser executadas 5 procedures para completar a deleção
			for (int i = 1; i <= 5; i++)
			{
				// Loop procedures (ini)
				// Monta a string de execução
				strSQL = Format("EXEC SPSNP_dlMedidor_p%.2d :p_EstudoId, :p_EquipId",
					ARRAYOFCONST((i)));
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (NULL);
				}
				// Loop procedures (fim)
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlRedeDominio(VTEstudo *estudo, VTRede* rede)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!rede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDEINV return (false);
	}

	__try
	{
		try
		{
			// Passo 01
			// Monta a string de execução
			strSQL = "EXEC SPSNP_dlRedeDominio :p_EstudoId, :p_RedeId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_RedeId", PARAMIN_INT(rede->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (NULL);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlRREquipamentoBarra(VTEstudo* estudo, VTEqpto* eqpto,
	int intMRedeId, int intCorteId)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTMRede* mrede;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!eqpto)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}

	__try
	{
		try
		{
			// Passo 01
			// Monta a string de execução
			strSQL =
				"EXEC SPSNP_dlRREquipamentoBarra_p01 :p_EstudoId, :p_MRedeId, :p_EquipId, :p_CorteId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_MRedeId", PARAMIN_INT(intMRedeId)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_CorteId", PARAMIN_INT(intCorteId)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (NULL);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
// A nova versão utiliza o modo batch
bool __fastcall TBDeqptoAccess::SPSNP_dlRREquipamentoBarra(VTEstudo* estudo, int intTipoEquipId,
	TList* lisMRede)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisMRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISMREDEINV return (false);
	}

	__try
	{
		try
		{
			// Passo 01 - Preenche a tabela temporária
			if (!SPSNP_dlTmpEquipCorte())
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_MREDEINV return (false);
			}
			if (!SPSNP_wrTmpEquipCorte(estudo, intTipoEquipId, lisMRede))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_MREDEINV return (false);
			}
			// Passo 02 - Chama a procedure que utiliza a tabela temporária
			// Monta a string de execução
			strSQL = "EXEC SPSNP_dlRREquipamentoBarra_p02 :p_EstudoId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlSincronismo(VTLock* lock)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!lock)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SINCRONISMOINV return (false);
	}

	// Apaga as chaves das manobras antes
	if (!SPSNP_dlSincronismoChave(lock))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SINCRONISMOINV return (false);
	}

	__try
	{
		try
		{
			// Passo 01
			// Monta a string de execução
			strSQL = "EXEC SPSNP_dlSincronismo :p_SincronismoId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_SincronismoId", PARAMIN_INT(lock->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (NULL);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlSincronismoChave(VTLock* lock)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!lock)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SINCRONISMOINV return (false);
	}

	__try
	{
		try
		{
			// Passo 01
			// Monta a string de execução
			strSQL = "EXEC SPSNP_dlSincronismoChave :p_SincronismoId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_SincronismoId", PARAMIN_INT(lock->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (NULL);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlTmpBarraRede(VTEstudo* estudo, TList* lstRedes)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTRede* rede;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lstRedes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISOBJINV return (false);
	}

	__try
	{
		try
		{
			// Passo 01
			// Preenche a tabela temporária
			if (!SPSNP_dlTmpEquip())
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_LISOBJINV return (false);
			}
			if (!SPSNP_wrTmpEquip(lstRedes))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_LISOBJINV return (false);
			}

			// Passo 02
			// Monta a string de execução
			strSQL = "EXEC SPSNP_dlBarraRede :p_EstudoId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (NULL);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlTmpCurva(void)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			strSQL = "EXEC SPSNP_dlTmpCurva";

			// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// Determina os valores dos parâmetros

			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlTmpEquip(void)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			strSQL = "EXEC SPSNP_dlTmpEquip";

			// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// Determina os valores dos parâmetros

			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlTmpEquipCurva(void)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			strSQL = "EXEC SPSNP_dlTmpEquipCurva";

			// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// Determina os valores dos parâmetros

			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrMRedeRREquipTmpEquipCurva(VTEstudo* estudo, int intMRedeId)
{
	bool sucesso = true;
	const int intNumTabRR = 4;
	int intCount;
	AnsiString strSQL[intNumTabRR];
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			intCount = 0;
			strSQL[intCount] = "EXEC SPSNP_wrMRedeRRCargaTmpEquipCurva :p_EstudoId, :p_MRedeId";
			intCount++;
			if (intCount >= intNumTabRR)
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			strSQL[intCount] =
				"EXEC SPSNP_wrMRedeRRSuprimentoTmpEquipCurva :p_EstudoId, :p_MRedeId";
			intCount++;
			if (intCount >= intNumTabRR)
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			strSQL[intCount] = "EXEC SPSNP_wrMRedeRRTrafoTmpEquipCurva :p_EstudoId, :p_MRedeId";
			intCount++;
			if (intCount >= intNumTabRR)
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			strSQL[intCount] = "EXEC SPSNP_wrMRedeRRYRefTmpEquipCurva :p_EstudoId, :p_MRedeId";

			for (int i = 0; i < intNumTabRR; i++)
			{
				// Decodifica comando
				bd_conn->ParseCmd(strSQL[i]);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_MRedeId", PARAMIN_INT(intMRedeId)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlTmpEquipCorte(void)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			strSQL = "EXEC SPSNP_dlTmpEquipCorte";

			// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// Determina os valores dos parâmetros

			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlTmpEstudoRedeCorte(void)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			strSQL = "EXEC SPSNP_dlTmpEstudoRedeCorte";

			// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// Determina os valores dos parâmetros

			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_dlTrafoEnrol(VTEstudo* estudo, VTEqpto* eqpto)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!eqpto)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "EXEC SPSNP_dlTrafo_p01 :p_EstudoId, :p_EquipId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (NULL);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdBarra(VTEstudo* estudo, bool bolUseRedeTmp, bool bolObra)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	strBARRA str_barra;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdBarra_p01 :p_EstudoId, :p_AlteradoObra";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdBarra_p02 :p_EstudoId, :p_AlteradoObra";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			if (!SPSNP_rdBarraCursor(estudo, ptrQryDS))
			{
				SNPSTRLOGLINE(strlog);
				sucesso = false;
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdBarraCursor(VTEstudo* estudo, TADOQuery* ptrQryDS)
{
	bool sucesso = true;
	AnsiString strlog;
	strBARRA str_barra;
	VTBarra* barra;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}
	if (!monta)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_MONTAINV return (false);
	}
	if (!ptrQryDS)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{ // JCG 2016.01.27
			// inicia controle de inserção de Barras de VTMOnta
			monta->InsereBarraStart();
			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				// preenche estrutura strBARRA
				str_barra.bar_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
				str_barra.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
				str_barra.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
				str_barra.vnom = ptrQryDS->FieldByName("VNom")->AsFloat;
				str_barra.utm.x = ptrQryDS->FieldByName("UtmX")->AsInteger;
				str_barra.utm.y = ptrQryDS->FieldByName("UtmY")->AsInteger;
				str_barra.esq.x1 = ptrQryDS->FieldByName("EsqX1")->AsInteger;
				str_barra.esq.y1 = ptrQryDS->FieldByName("EsqY1")->AsInteger;
				str_barra.esq.x2 = ptrQryDS->FieldByName("EsqX2")->AsInteger;
				str_barra.esq.y2 = ptrQryDS->FieldByName("EsqY2")->AsInteger;
				str_barra.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;
				str_barra.show = ptrQryDS->FieldByName("Show")->AsInteger;

				// Comentário do equipamento
				str_barra.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsInteger;
				str_barra.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsInteger;
				str_barra.comentario.comentario = ptrQryDS->FieldByName("Comentario")->AsString;

				// insere em Monta
				if ((barra = (VTBarra*)monta->InsereBarraEqpto(str_barra)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BARRAINV sucesso = false;
					break;
				}
				else
				{
					bd_func->DefineStatusEqpto(barra, estudo->Novo);
				}

				ptrQryDS->Next();
				// loop registros (end)
			}
			// finaliza controle de inserção de Barras de VTMOnta
			// JCG 2016.01.27
			monta->InsereBarraStop();

			// Esta função aqui não pode fechar o cursor
			// ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		// Esta função aqui não pode destruir
		// if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
	}

	return (sucesso);
}
/*
 //---------------------------------------------------------------------------
 bool __fastcall TBDeqptoAccess::SPSNP_rdCanal(VTEstudo* estudo, bool bolUseRedeTmp, bool bolObra)
 {
 Alerta("TBDeqptoAccess::SPSNP_rdCanal() - bloqueada");
 return(true);
 }
 */

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdCanal(VTEstudo* estudo, bool bolUseRedeTmp, bool bolObra)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTCanal* canal = NULL;
	strCANAL str_canal;
	TStream* ptrStream;
	strCURHEADER curhdr;
	bool bolTipica;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdCanal_p01 :p_EstudoId, :p_AlteradoObra";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdCanal_p02 :p_EstudoId, :p_AlteradoObra";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				str_canal.medidor_id = ptrQryDS->FieldByName("MedidorId")->AsInteger;
				str_canal.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
				str_canal.tipo_canal = ptrQryDS->FieldByName("TipoCanalId")->AsInteger;
				str_canal.tolerancia_perc = ptrQryDS->FieldByName("Sigma")->AsFloat;
				str_canal.eqpto_medido.tipo = ptrQryDS->FieldByName("TipoEquipId")->AsInteger;
				str_canal.eqpto_medido.id = ptrQryDS->FieldByName("EquipId")->AsInteger;
				str_canal.eqpto_medido.fases = ptrQryDS->FieldByName("TipoFaseId")->AsInteger;
				str_canal.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;
				bolTipica = (bool)ptrQryDS->FieldByName("Tipica")->AsInteger;
				if (!bolTipica)
				{
					ptrStream = ptrQryDS->CreateBlobStream(ptrQryDS->FieldByName("Pontos"), bmRead);
					if (!ptrStream)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CURVAINV sucesso = false;
						break;
					}
					if (!bd_func->DesmontaStreamCurvaEquipamento(ptrStream, &(str_canal.curva),
						curhdr))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CURVAINV sucesso = false;
						break;
					}
					// O LOBField limpa a stream
					// delete ptrStream;
				}
				// Ao desmontar o BLOB, o header da curva é zerado. Assim, os campos devem
				// ser preenchidos posteriormente
				str_canal.curva.tipica = bolTipica;
				str_canal.curva.status = (unsigned)ptrQryDS->FieldByName("Status")->AsInteger;
				str_canal.curva.id = ptrQryDS->FieldByName("ConjCurvaId")->AsInteger;

				// insere em Monta
				if ((canal = (VTCanal*)monta->InsereMedidorCanal(str_canal)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CANALINV sucesso = false;
					break;
				}
				else
				{
					bd_func->DefineStatusEqpto(canal, estudo->Novo);
					if (canal->Curva)
					{
						canal->Curva->Status[sttNOVO] = estudo->Novo;
					}
				}

				ptrQryDS->Next();
				// loop registros (fim)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdCapacitor(VTEstudo* estudo, bool bolUseRedeTmp,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTCapacitor* capacitor;
	strCAPACITOR str_capacitor;
	TStream* ptrStream;
	strCURHEADER curhdr;
	bool bolTipica;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdCapacitor_p01 :p_EstudoId, :p_AlteradoObra";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdCapacitor_p02 :p_EstudoId, :p_AlteradoObra";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				str_capacitor.cap_id = ptrQryDS->FieldByName("CapacitorId")->AsInteger;
				str_capacitor.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
				str_capacitor.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
				str_capacitor.ligacao = ptrQryDS->FieldByName("TipoLigacaoId")->AsInteger;
				str_capacitor.q_mvar = ptrQryDS->FieldByName("Qnom")->AsFloat;
				str_capacitor.vnom = ptrQryDS->FieldByName("Vnom")->AsFloat;
				str_capacitor.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;
				str_capacitor.bar_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
				str_capacitor.esq.posbar = ptrQryDS->FieldByName("PosBar")->AsFloat;
				str_capacitor.esq.dxbar = ptrQryDS->FieldByName("DX")->AsInteger;
				str_capacitor.esq.dybar = ptrQryDS->FieldByName("DY")->AsInteger;
				str_capacitor.show = ptrQryDS->FieldByName("Show")->AsInteger;
				bolTipica = (bool)ptrQryDS->FieldByName("Tipica")->AsInteger;
				if (!bolTipica)
				{
					ptrStream = ptrQryDS->CreateBlobStream(ptrQryDS->FieldByName("Pontos"), bmRead);
					if (!ptrStream)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CURVAINV sucesso = false;
						break;
					}
					if (!bd_func->DesmontaStreamCurvaEquipamento(ptrStream, &(str_capacitor.curva),
						curhdr))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CURVAINV sucesso = false;
						break;
					}
					// O LOBField limpa a stream
					// delete ptrStream;
				}
				// Ao desmontar o BLOB, o header da curva é zerado. Assim, os campos devem
				// ser preenchidos posteriormente
				str_capacitor.curva.tipica = bolTipica;
				str_capacitor.curva.status = (unsigned)ptrQryDS->FieldByName("Status")->AsInteger;
				str_capacitor.curva.id = ptrQryDS->FieldByName("ConjCurvaId")->AsInteger;

				// Comentário do equipamento
				str_capacitor.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
				str_capacitor.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
				str_capacitor.comentario.comentario = ptrQryDS->FieldByName("Comentario")->AsString;

				// insere em Monta
				if ((capacitor = (VTCapacitor*)monta->InsereCapacitor(str_capacitor)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CAPACITORINV sucesso = false;
					break;
				}
				else
				{
					bd_func->DefineStatusEqpto(capacitor, estudo->Novo);
					if (capacitor->curva)
					{
						capacitor->curva->Status[sttNOVO] = estudo->Novo;
					}
				}

				ptrQryDS->Next();
				// loop registros (fim)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdCapacitorSerie(VTEstudo* estudo, bool bolUseRedeTmp,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTCapserie* capserie = NULL;
	strCAPSERIE str_capserie;
	int ndxBarra;
	int intEquipIdPrev;
	int intEquipId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdCapacitorSerie_p01 :p_EstudoId, :p_AlteradoObra";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdCapacitorSerie_p02 :p_EstudoId, :p_AlteradoObra";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			intEquipIdPrev = IDINDEFINIDO;
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				intEquipId = ptrQryDS->FieldByName("CapacitorSerieId")->AsInteger;
				// Protege contra equipamentos com número de barras inválido
				if (intEquipIdPrev != intEquipId)
				{
					intEquipIdPrev = intEquipId;
					ndxBarra = 0;
				}

				// preenche estrutura strCAPSERIE
				if (ndxBarra == 0)
				{
					str_capserie.id = intEquipId;
					str_capserie.rede_id = ptrQryDS->FieldByName("RedeId")->AsInteger;
					str_capserie.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
					str_capserie.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;

					str_capserie.vnom = ptrQryDS->FieldByName("Vnom")->AsFloat;
					str_capserie.snom = ptrQryDS->FieldByName("Snom")->AsFloat;
					str_capserie.z.r = ptrQryDS->FieldByName("ZR")->AsFloat;
					str_capserie.z.x = ptrQryDS->FieldByName("ZX")->AsFloat;
					str_capserie.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;

					str_capserie.bar1_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
					str_capserie.esq.posbar1 = ptrQryDS->FieldByName("PosBar")->AsFloat;
					str_capserie.show = ptrQryDS->FieldByName("Show")->AsInteger;

					// Comentário do equipamento
					str_capserie.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
					str_capserie.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
					str_capserie.comentario.comentario =
						ptrQryDS->FieldByName("Comentario")->AsString;

				}
				else
				{
					str_capserie.bar2_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
					str_capserie.esq.posbar2 = ptrQryDS->FieldByName("PosBar")->AsFloat;

					if ((capserie = (VTCapserie*)monta->InsereCapserie(str_capserie)) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CAPSERIEINV sucesso = false;
						break;
					}
					else
					{
						bd_func->DefineStatusEqpto(capserie, estudo->Novo);
					}
				}
				ndxBarra++;
				if (ndxBarra > 1)
				{
					ndxBarra = 0;
				}

				ptrQryDS->Next();
				// loop registros (end)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdCarga(VTEstudo* estudo, bool bolUseRedeTmp, bool bolObra)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTCarga* carga = NULL;
	strCARGA str_carga;
	TStream* ptrStream;
	strCURHEADER curhdr;
	bool bolTipica;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdCarga_p01 :p_EstudoId, :p_AlteradoObra";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdCarga_p02 :p_EstudoId, :p_AlteradoObra";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				str_carga.car_id = ptrQryDS->FieldByName("CargaId")->AsInteger;
				str_carga.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
				str_carga.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
				str_carga.fases = ptrQryDS->FieldByName("TipoFaseId")->AsInteger;
				str_carga.energia_mwhmes = ptrQryDS->FieldByName("Energia")->AsFloat;
				str_carga.classe_cons = ptrQryDS->FieldByName("TipoConsTag")->AsInteger;
				str_carga.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;
				str_carga.modelo.icte_pu = ptrQryDS->FieldByName("ICtePu")->AsFloat;
				str_carga.modelo.zcte_pu = ptrQryDS->FieldByName("ZCtePu")->AsFloat;
				str_carga.modelo.scte_pu = ptrQryDS->FieldByName("SCtePu")->AsFloat;
				str_carga.num_cons.residencial = ptrQryDS->FieldByName("NumConsRes")->AsFloat;
				str_carga.num_cons.comercial = ptrQryDS->FieldByName("NumConsCom")->AsFloat;
				str_carga.num_cons.industrial = ptrQryDS->FieldByName("NumConsInd")->AsFloat;
				str_carga.num_cons.rural = ptrQryDS->FieldByName("NumConsRur")->AsFloat;
				str_carga.num_cons.outros = ptrQryDS->FieldByName("NumConsOut")->AsFloat;
				str_carga.num_cons.a4 = ptrQryDS->FieldByName("NumConsA4")->AsFloat;
				// código temporário: a informação de VIP e GrupoA está em Extern_id
				str_carga.vip = false;
				str_carga.grupoA = false;
				if (str_carga.extern_id.Pos("vip") > 0)
					str_carga.vip = true;
				if (str_carga.extern_id.Pos("grupoA") > 0)
					str_carga.grupoA = true;
				str_carga.extern_id = "";
				// fim código temporário
				// zera dados de IP
				str_carga.ip.pot_kw = 0;
				str_carga.ip.fat_pot = 0;
				str_carga.ip.num_lamp = 0;
				//
				str_carga.bar_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
				str_carga.esq.posbar = ptrQryDS->FieldByName("PosBar")->AsFloat;
				str_carga.esq.dxbar = ptrQryDS->FieldByName("DX")->AsInteger;
				str_carga.esq.dybar = ptrQryDS->FieldByName("DY")->AsInteger;
				str_carga.show = ptrQryDS->FieldByName("Show")->AsInteger;
				bolTipica = (bool)ptrQryDS->FieldByName("Tipica")->AsInteger;
				if (!bolTipica)
				{
					ptrStream = ptrQryDS->CreateBlobStream(ptrQryDS->FieldByName("Pontos"), bmRead);
					if (!ptrStream)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CURVAINV sucesso = false;
						break;
					}
					if (!bd_func->DesmontaStreamCurvaEquipamento(ptrStream, &(str_carga.curva),
						curhdr))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CURVAINV sucesso = false;
						break;
					}
					// O LOBField limpa a stream
					// delete ptrStream;
					// IMPORTANTE: curva própria da Carga possui classe_id = IDINDEFINIDO
					// str_carga.classe_cons = (int)curhdr.uchClasseId;
				}
				// Ao desmontar o BLOB, o header da curva é zerado. Assim, os campos devem
				// ser preenchidos posteriormente
				str_carga.curva.tipica = bolTipica;
				str_carga.curva.status = (unsigned)ptrQryDS->FieldByName("Status")->AsInteger;
				str_carga.curva.id = ptrQryDS->FieldByName("ConjCurvaId")->AsInteger;

				// Comentário do equipamento
				str_carga.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
				str_carga.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
				str_carga.comentario.comentario = ptrQryDS->FieldByName("Comentario")->AsString;

				// insere em Monta
				if (cronometro)
					cronometro->Start("Carga Monta");
				if ((carga = (VTCarga*)monta->InsereCarga(str_carga)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CARGAINV sucesso = false;
					break;
				}
				else
				{
					bd_func->DefineStatusEqpto(carga, estudo->Novo);
					if (carga->curva)
					{
						carga->curva->Status[sttNOVO] = estudo->Novo;
					}
				}
				if (cronometro)
					cronometro->Stop("Carga Monta");

				ptrQryDS->Next();
				// loop registros (fim)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdCargaIP(VTEstudo* estudo, bool bolUseRedeTmp, bool bolObra)
{
	bool sucesso = true;
	unsigned modelo_num_lamp;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTCarga* carga = NULL;
	strCARGA str_carga;
	TStream* ptrStream;
	strCURHEADER curhdr;
	bool bolTipica;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdCargaIP_p01 :p_EstudoId, :p_AlteradoObra";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdCargaIP_p02 :p_EstudoId, :p_AlteradoObra";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				str_carga.car_id = ptrQryDS->FieldByName("CargaIPId")->AsInteger;
				str_carga.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
				str_carga.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
				str_carga.fases = ptrQryDS->FieldByName("TipoFaseId")->AsInteger;
				str_carga.bar_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
				str_carga.esq.posbar = ptrQryDS->FieldByName("PosBar")->AsFloat;
				str_carga.esq.dxbar = ptrQryDS->FieldByName("DX")->AsFloat;
				str_carga.esq.dybar = ptrQryDS->FieldByName("DY")->AsFloat;
				str_carga.show = ptrQryDS->FieldByName("Show")->AsInteger;
				str_carga.ip.pot_kw = ptrQryDS->FieldByName("Pnom")->AsFloat;
				str_carga.ip.fat_pot = ptrQryDS->FieldByName("FatPot")->AsFloat;
				// str_carga.ip.num_lamp          = ptrQryDS->FieldByName("NumLamp")->AsInteger;
				str_carga.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;
				// lê campo com informação composta: modelo + número de lâmpadas
				modelo_num_lamp = ptrQryDS->FieldByName("NumLamp")->AsInteger;
				str_carga.ip.num_lamp = modelo_num_lamp & 0xffff;
				// define modelo de carga
				str_carga.modelo.icte_pu = ptrQryDS->FieldByName("ICtePu")->AsFloat;
				str_carga.modelo.zcte_pu = ptrQryDS->FieldByName("ZCtePu")->AsFloat;
				str_carga.modelo.scte_pu = ptrQryDS->FieldByName("SCtePu")->AsFloat;
				switch ((modelo_num_lamp >> 16) & 0xf)
				{
				case cargaICTE:
					str_carga.modelo.icte_pu = 1;
					break;
				case cargaSCTE:
					str_carga.modelo.scte_pu = 1;
					break;
				case cargaZCTE:
					str_carga.modelo.zcte_pu = 1;
					break;
				default:
					str_carga.modelo.scte_pu = 1;
					break;
				}
				str_carga.num_cons.comercial = 0;
				str_carga.num_cons.residencial = 0;
				str_carga.num_cons.industrial = 0;
				str_carga.num_cons.rural = 0;
				str_carga.num_cons.outros = 0;
				str_carga.num_cons.a4 = 0;
				str_carga.energia_mwhmes = 0;
				str_carga.classe_cons = consIP;
				bolTipica = (bool)ptrQryDS->FieldByName("Tipica")->AsInteger;
				if (!bolTipica)
				{
					ptrStream = ptrQryDS->CreateBlobStream(ptrQryDS->FieldByName("Pontos"), bmRead);
					if (!ptrStream)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CURVAINV sucesso = false;
						break;
					}
					if (!bd_func->DesmontaStreamCurvaEquipamento(ptrStream, &(str_carga.curva),
						curhdr))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CURVAINV sucesso = false;
						break;
					}
					// O LOBField limpa a stream
					// delete ptrStream;

					str_carga.classe_cons = (int)curhdr.uchClasseId;
				}
				// Ao desmontar o BLOB, o header da curva é zerado. Assim, os campos devem
				// ser preenchidos posteriormente
				str_carga.curva.tipica = bolTipica;
				str_carga.curva.status = (unsigned)ptrQryDS->FieldByName("Status")->AsInteger;
				str_carga.curva.id = ptrQryDS->FieldByName("ConjCurvaId")->AsInteger;

				// Comentário do equipamento
				str_carga.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
				str_carga.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
				str_carga.comentario.comentario = ptrQryDS->FieldByName("Comentario")->AsString;

				// insere em Monta
				if (cronometro)
					cronometro->Start("CargaIP Monta");
				if ((carga = (VTCarga*)monta->InsereCarga(str_carga)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CARGAINV sucesso = false;
					break;
				}
				else
				{
					bd_func->DefineStatusEqpto(carga, estudo->Novo);
					if (carga->curva)
					{
						carga->curva->Status[sttNOVO] = estudo->Novo;
					}
				}
				if (cronometro)
					cronometro->Stop("CargaIP Monta");

				ptrQryDS->Next();
				// loop registros (fim)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdCargaNL(VTEstudo* estudo, int intRedeId, bool bolObra)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTCargaNL* cargaNL = NULL;
	strCARGANL str_cargaNL;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (intRedeId != IDINDEFINIDO)
			{
				strSQL = "EXEC SPSNP_rdCargaNL_p01 :p_EstudoId, :p_AlteradoObra, :p_RedeId";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_RedeId", PARAMIN_INT(intRedeId)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
			}
			else
			{
				strSQL = "EXEC SPSNP_rdCargaNL_p02 :p_EstudoId, :p_AlteradoObra";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				str_cargaNL.car_id = ptrQryDS->FieldByName("cargaNLId")->AsInteger;
				str_cargaNL.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
				str_cargaNL.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
				str_cargaNL.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;
				str_cargaNL.bar_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
				str_cargaNL.esq.posbar = ptrQryDS->FieldByName("PosBar")->AsFloat;
				str_cargaNL.esq.dxbar = ptrQryDS->FieldByName("DX")->AsInteger;
				str_cargaNL.esq.dybar = ptrQryDS->FieldByName("DY")->AsInteger;
				str_cargaNL.show = ptrQryDS->FieldByName("Show")->AsInteger;
				str_cargaNL.ordem = ptrQryDS->FieldByName("OrdemId")->AsInteger;
				str_cargaNL.fat_pot = ptrQryDS->FieldByName("FatorPot")->AsFloat;
				str_cargaNL.flag_indutivo = (bool)ptrQryDS->FieldByName("Indutivo")->AsInteger;
				str_cargaNL.flag_kVA = (bool)ptrQryDS->FieldByName("kVA")->AsFloat;
				str_cargaNL.snomA = ptrQryDS->FieldByName("SNomA")->AsFloat;
				str_cargaNL.snomB = ptrQryDS->FieldByName("SNomB")->AsFloat;
				str_cargaNL.snomC = ptrQryDS->FieldByName("SNomC")->AsFloat;
				str_cargaNL.angA = ptrQryDS->FieldByName("AngA")->AsFloat;
				str_cargaNL.angB = ptrQryDS->FieldByName("AngB")->AsFloat;
				str_cargaNL.angC = ptrQryDS->FieldByName("AngC")->AsFloat;

				// Comentário do equipamento
				str_cargaNL.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
				str_cargaNL.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
				str_cargaNL.comentario.comentario = ptrQryDS->FieldByName("Comentario")->AsString;

				// insere em Monta
				// O monta deve saber quem é a fundamental e inserir uma nova carga
				// somente nesse caso. Para as harmônicas, retorna a mesma carga pre-
				// viamente inserida.
				if ((cargaNL = (VTCargaNL*)monta->InsereCargaNL(str_cargaNL)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CARGAINV sucesso = false;
					break;
				}
				else
				{
					bd_func->DefineStatusEqpto(cargaNL, estudo->Novo);
				}

				ptrQryDS->Next();
				// loop registros (fim)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdChave(VTEstudo* estudo, bool bolUseRedeTmp, bool bolObra)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTChave* chave = NULL;
	strCHAVE str_chave;
	int ndxBarra;
	int intEquipIdPrev;
	int intEquipId;
	int offset, length;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdChave_p01 :p_EstudoId, :p_AlteradoObra";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdChave_p02 :p_EstudoId, :p_AlteradoObra";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			intEquipIdPrev = IDINDEFINIDO;
			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				intEquipId = ptrQryDS->FieldByName("ChaveId")->AsInteger;
				// Protege contra equipamentos com número de barras inválido
				if (intEquipIdPrev != intEquipId)
				{
					intEquipIdPrev = intEquipId;
					ndxBarra = 0;
				}
				// preenche estrutura strCHAVE
				if (ndxBarra == 0)
				{
					str_chave.chv_id = intEquipId;
					str_chave.rede_id = ptrQryDS->FieldByName("RedeId")->AsInteger;
					str_chave.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
					str_chave.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
					str_chave.tipochave.id = ptrQryDS->FieldByName("TipoChaveId")->AsInteger;
					str_chave.estado = ptrQryDS->FieldByName("Estado")->AsInteger;
					str_chave.corn = ptrQryDS->FieldByName("Inom")->AsFloat;
					str_chave.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;

					str_chave.bar1_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
					str_chave.esq.posbar1 = ptrQryDS->FieldByName("PosBar")->AsFloat;
					str_chave.show = ptrQryDS->FieldByName("Show")->AsInteger;
					str_chave.telecomandada = (bool)ptrQryDS->FieldByName("Telecomando")->AsInteger;
					str_chave.protecao_ativa = (bool)ptrQryDS->FieldByName("Protecao")->AsInteger;
					str_chave.tecno_comunicacao = ptrQryDS->FieldByName("TecnoCom")->AsInteger;

					// Comentário do equipamento
					str_chave.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
					str_chave.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
					str_chave.comentario.comentario = ptrQryDS->FieldByName("Comentario")->AsString;
					// código temporário: a informação de EloFusivel está em Extern_id
					if ((offset = str_chave.extern_id.Pos("_elo") - 1) > 1)
					{
						str_chave.elo_fusivel = str_chave.extern_id.SubString(1, offset);
						length = str_chave.extern_id.Length();
						str_chave.extern_id =
							str_chave.extern_id.SubString(offset + 5, length + 1); // "";
					}
					// código temporário: a informação de TipoGIS está em Extern_id
					if ((offset = str_chave.extern_id.Pos("_gis") - 1) > 1)
					{
						str_chave.tipo_gis = str_chave.extern_id.SubString(1, offset);
						str_chave.extern_id = "";
					}
				}
				else
				{
					str_chave.bar2_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
					str_chave.esq.posbar2 = ptrQryDS->FieldByName("PosBar")->AsFloat;
					if ((chave = (VTChave*)monta->InsereChave(str_chave)) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CHAVEINV sucesso = false;
						break;
					}
					else
					{
						bd_func->DefineStatusEqpto(chave, estudo->Novo);
						//Define o estado da chave na rede original como o status dela
						//para usos que compare com o estado na rede original
                        chave->Status[sttCHAVE_ABERTO_ORIGINAL] = (chave->Aberta);
					}
				}
				ndxBarra++;
				if (ndxBarra > 1)
				{
					ndxBarra = 0;
				}

				ptrQryDS->Next();
				// loop registros (end)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdCorteMRede(VTEstudo* estudo, TList *lisMRede,
	TList *lisCORTE)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	int intCorteId;
	VTMRede* mrede;
	VTCorte* corte;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!lisMRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISMREDEINV return (false);
	}
	if (!monta)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_MONTAINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdCorteMRede :p_EstudoId, :p_MRedeId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Executa para todas as mredes da lista
			for (int i = 0; i < lisMRede->Count; i++)
			{
				if ((mrede = (VTMRede*)lisMRede->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MREDEINV return (false);
				}
				if (!bd_conn->SetParam("p_MRedeId", PARAMIN_INT(mrede->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// Comando da consulta
				if (!bd_conn->Execute(true))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}
				if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}
				// Recupera o resultado
				for (int i = 0; i < ptrQryDS->RecordCount; i++)
				{
					// loop registros (ini)
					intCorteId = ptrQryDS->FieldByName("CorteId")->AsInteger;
					// insere em Monta
					if ((corte = (VTCorte*)monta->InsereCorte(intCorteId, mrede->Id)) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CORTEINV return (false);
					}
					else
					{ // redefine status do Corte
						corte->DefineStatus(sttNOVO, false);
						// salva Corte em lisCORTE, se a lista foi passada
						if (lisCORTE)
						{
							if (lisCORTE->IndexOf(corte) < 0)
								lisCORTE->Add(corte);
						}
					}
					ptrQryDS->Next();
					// loop registros (end)
				}
			}

			// Nesse caso, pode ser que o cursor nem seja aberto, caso a lista esteja vazia
			if (ptrQryDS)
			{
				ptrQryDS->Close();
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdDominio(void)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	strDOMINIO str_dominio;
	VTDominio* dominio = NULL;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}
	if (!monta)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_MONTAINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdDominio";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				// !OOPS! Há realmente necessida de haver a indicação "AlteradoObra"?
				str_dominio.obra = false;
				str_dominio.dominio_id = ptrQryDS->FieldByName("DominioId")->AsInteger;
				str_dominio.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
				str_dominio.data_criacao = ptrQryDS->FieldByName("DataCriacao")->AsDateTime;
				if ((dominio = (VTDominio*)monta->InsereDominio(str_dominio)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDRD_DOMINIO sucesso = false;
					break;
				}
				dominio->DefineStatus(sttNOVO, false);
				dominio->DefineStatus(sttALTERADO, false);

				ptrQryDS->Next();
				// loop registros (end)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdTipoEqbarRedes(VTEstudo* estudo, int intTipoEquipId,
	TList* lstRedes)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	int intRedeId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lstRedes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Essa lista ão contém ponteiros válidos, apenas inteiros
			lstRedes->Clear();

			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdTipoEqbarRedes :p_EstudoId, :p_TipoEquipId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_TipoEquipId", PARAMIN_INT(intTipoEquipId)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				intRedeId = ptrQryDS->FieldByName("RedeId")->AsInteger;
				lstRedes->Add((void*)intRedeId);

				ptrQryDS->Next();
				// loop registros (end)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdTipoEqbarCargaRedes(VTEstudo* estudo, int intTipoEquipId,
	TList* lstRedes)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	int intRedeId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lstRedes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Essa lista ão contém ponteiros válidos, apenas inteiros
			lstRedes->Clear();

			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdTipoEqbarCargaRedes :p_EstudoId, :p_TipoEquipId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_TipoEquipId", PARAMIN_INT(intTipoEquipId)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				intRedeId = ptrQryDS->FieldByName("RedeId")->AsInteger;
				lstRedes->Add((void*)intRedeId);

				ptrQryDS->Next();
				// loop registros (end)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdTipoEqbarCargaIPRedes(VTEstudo* estudo, int intTipoEquipId,
	TList* lstRedes)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	int intRedeId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lstRedes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Essa lista ão contém ponteiros válidos, apenas inteiros
			lstRedes->Clear();

			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdTipoEqbarCargaIPRedes :p_EstudoId, :p_TipoEquipId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_TipoEquipId", PARAMIN_INT(intTipoEquipId)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				intRedeId = ptrQryDS->FieldByName("RedeId")->AsInteger;
				lstRedes->Add((void*)intRedeId);

				ptrQryDS->Next();
				// loop registros (end)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdTipoEqligRedes(VTEstudo* estudo, int intTipoEquipId,
	TList* lstRedes)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	int intRedeId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lstRedes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Essa lista ão contém ponteiros válidos, apenas inteiros
			lstRedes->Clear();

			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdTipoEqligRedes :p_EstudoId, :p_TipoEquipId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_TipoEquipId", PARAMIN_INT(intTipoEquipId)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				intRedeId = ptrQryDS->FieldByName("RedeId")->AsInteger;
				lstRedes->Add((void*)intRedeId);

				ptrQryDS->Next();
				// loop registros (end)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

////---------------------------------------------------------------------------
// bool __fastcall TBDeqptoAccess::SPSNP_rdEstudoOld(void)
// {
// bool           sucesso = true;
// AnsiString     strlog;
// AnsiString     strSQL;
// TADOQuery*     ptrQryDS = NULL;
// strPLAN_ESTUDO str_estudo;
// VTEstudo*      estudo;
//
// if (!bd_conn)                   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
// if (!bd_conn->getConnectedOK()) { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
// if (!monta)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_MONTAINV   return(false); }
//
// __try
// {
// try
// {
// // Monta a string de execução
// strSQL = "EXEC SPSNP_rdEstudo :p_EstudoId";
// // Decodifica comando
// bd_conn->ParseCmd(strSQL);
// // Determina os valores dos parâmetros
// if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(IDINDEFINIDO))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
// // Comando da consulta
// if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(false); }
// if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(false); }
//
// // Recupera o resultado
// for (int i = 0; i < ptrQryDS->RecordCount; i++)
// {
// // loop registros (ini)
// str_estudo.estudo_id = ptrQryDS->FieldByName("EstudoId")->AsInteger;
// str_estudo.tipo      = ptrQryDS->FieldByName("TipoEstudoId")->AsInteger;
// str_estudo.codigo    = ptrQryDS->FieldByName("EstudoDescricao")->AsString;
// str_estudo.autor     = ptrQryDS->FieldByName("EstudoProprietario")->AsString;
// str_estudo.data      = ptrQryDS->FieldByName("EstudoModificacao")->AsDateTime;
//
// if ((estudo = monta->InsereEstudo(str_estudo)) == NULL)
// {
// SNPSTRLOGLINE(strlog);
// MSGERRSYS_ESTUDOINV
// sucesso = false;
// break;
// }
//
// // Define o estado como false
// estudo->Novo = false;
//
// ptrQryDS->Next();
// // loop registros (end)
// }
//
// ptrQryDS->Close();
// }
// catch (Exception& e)
// {
// SNPSTRLOGLINE(strlog);
// SNPSTRLOG(e.Message);
// sucesso = false;
// }
// }
// __finally
// {
// // Colocado dentro do finally para garantir que será deletado, mesmo que
// // haja um return no meio do código
//
// //destrói objetos locais
// if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
// }
//
// return(sucesso);
// }

////---------------------------------------------------------------------------
// bool __fastcall TBDeqptoAccess::SPSNP_rdEstudoNew(void)
// {
// bool           sucesso = true;
// AnsiString     strlog;
// AnsiString     strSQL;
// TADOQuery*     ptrQryDS = NULL;
// strPLAN_ESTUDO str_estudo;
// VTEstudo*      estudo;
//
// if (!bd_conn)                   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
// if (!bd_conn->getConnectedOK()) { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
// if (!monta)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_MONTAINV   return(false); }
//
// __try
// {
// try
// {
// // Monta a string de execução
// strSQL = "EXEC SPSNP_rdEstudo :p_EstudoId";
// // Decodifica comando
// bd_conn->ParseCmd(strSQL);
// // Determina os valores dos parâmetros
// if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(IDINDEFINIDO))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
// // Comando da consulta
// if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(false); }
// if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(false); }
//
// // Recupera o resultado
// for (int i = 0; i < ptrQryDS->RecordCount; i++)
// {
// // loop registros (ini)
// str_estudo.estudo_id = ptrQryDS->FieldByName("EstudoId")->AsInteger;
// str_estudo.tipo      = ptrQryDS->FieldByName("TipoEstudoId")->AsInteger;
// str_estudo.codigo    = ptrQryDS->FieldByName("EstudoDescricao")->AsString;
// str_estudo.autor     = ptrQryDS->FieldByName("EstudoProprietario")->AsString;
// str_estudo.data      = ptrQryDS->FieldByName("EstudoModificacao")->AsDateTime;
// str_estudo.justificativa = ptrQryDS->FieldByName("Justificativa")->AsString;
//
// if ((estudo = monta->InsereEstudo(str_estudo)) == NULL)
// {
// SNPSTRLOGLINE(strlog);
// MSGERRSYS_ESTUDOINV
// sucesso = false;
// break;
// }
//
// // Define o estado como false
// estudo->Novo = false;
//
// ptrQryDS->Next();
// // loop registros (end)
// }
//
// ptrQryDS->Close();
// }
// catch (Exception& e)
// {
// SNPSTRLOGLINE(strlog);
// SNPSTRLOG(e.Message);
// sucesso = false;
// }
// }
// __finally
// {
// // Colocado dentro do finally para garantir que será deletado, mesmo que
// // haja um return no meio do código
//
// //destrói objetos locais
// if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
// }
//
// return(sucesso);
// }

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdFiltro(VTEstudo* estudo, bool bolUseRedeTmp, bool bolObra)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTFiltro* filtro = NULL;
	strFILTRO str_filtro;
	TStream* ptrStream;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdFiltro_p01 :p_EstudoId, :p_AlteradoObra";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdFiltro_p02 :p_EstudoId, :p_AlteradoObra";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				str_filtro.id = ptrQryDS->FieldByName("FiltroId")->AsInteger;
				str_filtro.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
				str_filtro.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
				str_filtro.vnom = ptrQryDS->FieldByName("Vnom")->AsFloat;
				str_filtro.snom = ptrQryDS->FieldByName("Snom")->AsFloat;
				str_filtro.z0.r = ptrQryDS->FieldByName("Z0R")->AsFloat;
				str_filtro.z0.x = ptrQryDS->FieldByName("Z0X")->AsFloat;
				str_filtro.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;

				str_filtro.bar_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
				str_filtro.esq.posbar = ptrQryDS->FieldByName("PosBar")->AsFloat;
				str_filtro.esq.dxbar = ptrQryDS->FieldByName("DX")->AsInteger;
				str_filtro.esq.dybar = ptrQryDS->FieldByName("DY")->AsInteger;
				str_filtro.show = ptrQryDS->FieldByName("Show")->AsInteger;
				// Comentário do equipamento
				str_filtro.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
				str_filtro.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
				str_filtro.comentario.comentario = ptrQryDS->FieldByName("Comentario")->AsString;

				// insere em Monta
				if ((filtro = (VTFiltro*)monta->InsereFiltro(str_filtro)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_FILTROINV sucesso = false;
					break;
				}
				else
				{
					bd_func->DefineStatusEqpto(filtro, estudo->Novo);
				}

				ptrQryDS->Next();
				// loop registros (fim)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdManobra(void)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	int intManobraId;
	int intManobraIdPrev;
	int intRedeId;
	int intChaveId;
	bool bolFechada;
	VTChave* chave;
	VTManobra* manobra;
	VTManobras* manobras = (VTManobras*)apl->GetObject(__classid(VTManobras));

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}
	if (!manobras)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_MANOBRASINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdManobra";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			intManobraIdPrev = IDINDEFINIDO;
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				intManobraId = ptrQryDS->FieldByName("ManobraId")->AsInteger;
				intRedeId = ptrQryDS->FieldByName("RedeId")->AsInteger;
				intChaveId = ptrQryDS->FieldByName("ChaveId")->AsInteger;
				bolFechada = (bool)ptrQryDS->FieldByName("Fechada")->AsInteger;
				// verifica se é uma Manobra nova
				if (intManobraId != intManobraIdPrev)
				{ // salva ult_manobra_id lido
					intManobraIdPrev = intManobraId;
					// cria um novo objeto Manobra
					manobra = manobras->InsereManobra();
					manobra->Id = intManobraId;
					manobra->Codigo = ptrQryDS->FieldByName("Codigo")->AsString;
					manobra->Status[sttNOVO] = false;
					manobra->Status[sttALTERADO] = false;
				}
				// verifica se o objeto Manobra não foi destruído devido à ausência de Chave
				if (manobra != NULL)
				{
					// determina Chave do objeto Locked
					if ((chave = bd_func->ExisteChave(redes, intRedeId, intChaveId)) == NULL)
					{
						// Chave da Manobra não disponível: descarta Manobra
						manobras->RetiraManobra(manobra);
						manobra = NULL;
					}
					else
					{
						// insere um objeto Locked na Manobra
						manobra->InsereLocked(chave, bolFechada);
					}
				}

				ptrQryDS->Next();
				// loop registros (fim)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdGerador(VTEstudo* estudo, bool bolUseRedeTmp, bool bolObra)
{
	bool sucesso = true;
	AnsiString txt;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTGerador* gerador = NULL;
	strGERADOR str_gerador;
	TStream* ptrStream;
	strCURHEADER curhdr;
	bool bolTipica;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdGerador_p01 :p_EstudoId, :p_AlteradoObra";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdGerador_p02 :p_EstudoId, :p_AlteradoObra";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				str_gerador.ger_id = ptrQryDS->FieldByName("GeradorId")->AsInteger;
				str_gerador.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
				str_gerador.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
				str_gerador.ligacao = ptrQryDS->FieldByName("TipoLigacaoId")->AsInteger;
				str_gerador.tiposup = ptrQryDS->FieldByName("TipoSuprId")->AsInteger;
				str_gerador.tipo_geracao = ptrQryDS->FieldByName("TipoGerId")->AsInteger;
				str_gerador.vnom = ptrQryDS->FieldByName("Vnom")->AsFloat;
				str_gerador.snom_mva = ptrQryDS->FieldByName("Snom")->AsFloat;
				str_gerador.smax_mva = ptrQryDS->FieldByName("Smax")->AsFloat;
				str_gerador.fplim = ptrQryDS->FieldByName("FatorPotMin")->AsFloat;
				str_gerador.z0.r = ptrQryDS->FieldByName("Z0R")->AsFloat;
				str_gerador.z0.x = ptrQryDS->FieldByName("Z0X")->AsFloat;
				str_gerador.z1.r = ptrQryDS->FieldByName("Z1R")->AsFloat;
				str_gerador.z1.x = ptrQryDS->FieldByName("Z1X")->AsFloat;
				str_gerador.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;
				str_gerador.bar_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
				str_gerador.esq.posbar = ptrQryDS->FieldByName("PosBar")->AsFloat;
				str_gerador.esq.dxbar = ptrQryDS->FieldByName("DX")->AsInteger;
				str_gerador.esq.dybar = ptrQryDS->FieldByName("DY")->AsInteger;
				str_gerador.show = ptrQryDS->FieldByName("Show")->AsInteger;
				bolTipica = (bool)ptrQryDS->FieldByName("Tipica")->AsInteger;
				// código temporário: fases do Gerador está em Extern_id
				if (str_gerador.extern_id.AnsiPos("Fases=") == 1)
				{
					txt = str_gerador.extern_id.SubString(7, str_gerador.extern_id.Length() - 6);
					str_gerador.fases = fases->ExisteFase(txt);
				}
				else
				{
					str_gerador.fases = faseABCN;
				}
				// fim do código temporário
				if (!bolTipica)
				{
					ptrStream = ptrQryDS->CreateBlobStream(ptrQryDS->FieldByName("Pontos"), bmRead);
					if (!ptrStream)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CURVAINV sucesso = false;
						break;
					}
					if (!bd_func->DesmontaStreamCurvaEquipamento(ptrStream, &(str_gerador.curva),
						curhdr))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CURVAINV sucesso = false;
						break;
					}
					// O LOBField limpa a stream
					// delete ptrStream;
				}
				// Ao desmontar o BLOB, o header da curva é zerado. Assim, os campos devem
				// ser preenchidos posteriormente
				str_gerador.curva.tipica = bolTipica;
				str_gerador.curva.status = (unsigned)ptrQryDS->FieldByName("Status")->AsInteger;
				str_gerador.curva.id = ptrQryDS->FieldByName("ConjCurvaId")->AsInteger;

				// Comentário do equipamento
				str_gerador.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
				str_gerador.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
				str_gerador.comentario.comentario = ptrQryDS->FieldByName("Comentario")->AsString;

				// insere em Monta
				if ((gerador = (VTGerador*)monta->InsereGerador(str_gerador)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_GERADORINV sucesso = false;
					break;
				}
				else
				{
					bd_func->DefineStatusEqpto(gerador, estudo->Novo);
					if (gerador->curva)
					{
						gerador->curva->Status[sttNOVO] = estudo->Novo;
					}
				}

				ptrQryDS->Next();
				// loop registros (fim)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdMedidor(VTEstudo* estudo, bool bolUseRedeTmp, bool bolObra)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTMedidor* medidor = NULL;
	strMEDIDOR str_medidor;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdMedidor_p01 :p_EstudoId, :p_AlteradoObra";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdMedidor_p02 :p_EstudoId, :p_AlteradoObra";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				str_medidor.medidor_id = ptrQryDS->FieldByName("MedidorId")->AsInteger;
				str_medidor.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
				str_medidor.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
				str_medidor.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;

				str_medidor.bar_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
				str_medidor.esq.posbar = ptrQryDS->FieldByName("PosBar")->AsFloat;
				str_medidor.esq.dxbar = ptrQryDS->FieldByName("DX")->AsInteger;
				str_medidor.esq.dybar = ptrQryDS->FieldByName("DY")->AsInteger;
				str_medidor.show = ptrQryDS->FieldByName("Show")->AsInteger;

				// Comentário do equipamento
				str_medidor.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
				str_medidor.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
				str_medidor.comentario.comentario = ptrQryDS->FieldByName("Comentario")->AsString;

				// insere em Monta
				if ((medidor = (VTMedidor*)monta->InsereMedidor(str_medidor)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MEDIDORINV sucesso = false;
					break;
				}
				else
				{
					bd_func->DefineStatusEqpto(medidor, estudo->Novo);
				}

				ptrQryDS->Next();
				// loop registros (fim)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdMRedeCorte(VTEstudo* estudo, TList *lisCORTE)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	int intMRedeId;
	VTCorte* corte;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!monta)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_MONTAINV return (false);
	}
	if (!lisCORTE)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISCORTEINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdMRedeCorte :p_EstudoId, :p_CorteId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_ESTUDOINV return (false);
			}
			// Loop de todos os cortes
			for (int i = 0; i < lisCORTE->Count; i++)
			{
				if ((corte = (VTCorte*)lisCORTE->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CORTEINV return (false);
				}
				if (!bd_conn->SetParam("p_CorteId", PARAMIN_INT(corte->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_ESTUDOINV return (false);
				}
				// Comando da consulta
				if (!bd_conn->Execute(true))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}
				if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}
				// Recupera o resultado
				for (int i = 0; i < ptrQryDS->RecordCount; i++)
				{
					// loop registros (ini)
					intMRedeId = ptrQryDS->FieldByName("MRedeId")->AsInteger;

					// insere em Monta
					if ((corte = (VTCorte*)monta->InsereCorte(corte->Id, intMRedeId)) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CORTEINV return (false);
					}
					else
					{
						corte->DefineStatus(sttNOVO, false);
					}

					ptrQryDS->Next();
					// loop registros (end)
				}
				ptrQryDS->Close();
				// destrói objetos locais
				if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
				{
					delete ptrQryDS;
					ptrQryDS = NULL;
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdMutua(VTEstudo* estudo, bool bolUseRedeTmp, bool bolObra)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTMutua* mutua = NULL;
	strMUTUA str_mutua;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdMutua_p01 :p_EstudoId, :p_AlteradoObra";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdMutua_p02 :p_EstudoId, :p_AlteradoObra";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				str_mutua.id = ptrQryDS->FieldByName("MutuaId")->AsInteger;
				str_mutua.rede_id = ptrQryDS->FieldByName("RedeId")->AsInteger;
				str_mutua.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
				str_mutua.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
				str_mutua.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;

				// preenche estrutura strMUTUA
				str_mutua.tre1_id = ptrQryDS->FieldByName("Trecho1Id")->AsInteger;
				str_mutua.tre1_bar_id = ptrQryDS->FieldByName("Trecho1BarraSntId")->AsInteger;
				str_mutua.postre1 = ptrQryDS->FieldByName("Trecho1Pos")->AsFloat;
				str_mutua.tre2_id = ptrQryDS->FieldByName("Trecho2Id")->AsInteger;
				str_mutua.tre2_bar_id = ptrQryDS->FieldByName("Trecho2BarraSntId")->AsInteger;
				str_mutua.postre2 = ptrQryDS->FieldByName("Trecho2Pos")->AsFloat;
				str_mutua.z0.r = ptrQryDS->FieldByName("Z0R")->AsFloat;
				str_mutua.z0.x = ptrQryDS->FieldByName("Z0X")->AsFloat;
				str_mutua.z1.r = ptrQryDS->FieldByName("Z1R")->AsFloat;
				str_mutua.z1.x = ptrQryDS->FieldByName("Z1X")->AsFloat;
				str_mutua.show = ptrQryDS->FieldByName("Show")->AsInteger;

				// Comentário do equipamento
				str_mutua.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
				str_mutua.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
				str_mutua.comentario.comentario = ptrQryDS->FieldByName("Comentario")->AsString;

				if ((mutua = (VTMutua*)monta->InsereMutua(str_mutua)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MUTUAINV sucesso = false;
					break;
				}
				else
				{
					bd_func->DefineStatusEqpto(mutua, estudo->Novo);
				}

				ptrQryDS->Next();
				// loop registros (end)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdReator(VTEstudo* estudo, bool bolUseRedeTmp, bool bolObra)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTReator* reator = NULL;
	strREATOR str_reator;
	TStream* ptrStream;
	strCURHEADER curhdr;
	bool bolTipica;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdReator_p01 :p_EstudoId, :p_AlteradoObra";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdReator_p02 :p_EstudoId, :p_AlteradoObra";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				str_reator.id = ptrQryDS->FieldByName("ReatorId")->AsInteger;
				str_reator.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
				str_reator.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
				str_reator.ligacao = ptrQryDS->FieldByName("TipoLigacaoId")->AsInteger;
				str_reator.vnom = ptrQryDS->FieldByName("Vnom")->AsFloat;
				str_reator.q_mvar = ptrQryDS->FieldByName("Qnom")->AsFloat;
				str_reator.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;

				str_reator.bar_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
				str_reator.esq.posbar = ptrQryDS->FieldByName("PosBar")->AsFloat;
				str_reator.esq.dxbar = ptrQryDS->FieldByName("DX")->AsInteger;
				str_reator.esq.dybar = ptrQryDS->FieldByName("DY")->AsInteger;
				str_reator.show = ptrQryDS->FieldByName("Show")->AsInteger;
				// mml@20130816 - colocada a curva
				bolTipica = (bool)ptrQryDS->FieldByName("Tipica")->AsInteger;
				if (!bolTipica)
				{
					ptrStream = ptrQryDS->CreateBlobStream(ptrQryDS->FieldByName("Pontos"), bmRead);
					if (!ptrStream)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CURVAINV sucesso = false;
						break;
					}
					if (!bd_func->DesmontaStreamCurvaEquipamento(ptrStream, &(str_reator.curva),
						curhdr))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CURVAINV sucesso = false;
						break;
					}
					// O LOBField limpa a stream
					// delete ptrStream;
				}
				// Ao desmontar o BLOB, o header da curva é zerado. Assim, os campos devem
				// ser preenchidos posteriormente
				str_reator.curva.tipica = bolTipica;
				str_reator.curva.status = (unsigned)ptrQryDS->FieldByName("Status")->AsInteger;
				str_reator.curva.id = ptrQryDS->FieldByName("ConjCurvaId")->AsInteger;

				// Comentário do equipamento
				str_reator.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
				str_reator.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
				str_reator.comentario.comentario = ptrQryDS->FieldByName("Comentario")->AsString;

				// insere em Monta
				if ((reator = (VTReator*)monta->InsereReator(str_reator)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_REATORINV sucesso = false;
					break;
				}
				else
				{
					bd_func->DefineStatusEqpto(reator, estudo->Novo);
					if ((reator->curva) && (!reator->curva->Tipica))
					{
						reator->curva->Status[sttNOVO] = estudo->Novo;
					}
				}

				ptrQryDS->Next();
				// loop registros (fim)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdRede(VTEstudo* estudo, int intTipoRedeId, bool bolObra)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	strREDE str_rede;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			// JCG 20120619
			strSQL = "EXEC SPSNP_rdRede :p_EstudoId, :p_AlteradoObra, :p_TipoRedeId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_TipoRedeId", PARAMIN_INT(intTipoRedeId)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			if (!SPSNP_rdRedeCursor(estudo, ptrQryDS))
			{
				SNPSTRLOGLINE(strlog);
				sucesso = false;
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16666485
bool __fastcall TBDeqptoAccess::SPSNP_rdRedeCursor(VTEstudo* estudo, TADOQuery* ptrQryDS,
	TList *lisRedeAux)
{
	bool sucesso = true;
	VTRede* rede = NULL;
	AnsiString strlog;
	strREDE str_rede;
	strCLUSTER str_cluster;
	VTCluster* cluster;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}
	if (!monta)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_MONTAINV return (false);
	}
	if (!ptrQryDS)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				// preenche estrutura strREDE
				str_rede.rede_id = ptrQryDS->FieldByName("RedeId")->AsInteger;
				str_rede.mrede_id = ptrQryDS->FieldByName("MRedeId")->AsInteger;
				str_rede.tiporede.id = ptrQryDS->FieldByName("TipoRedeId")->AsInteger;
				str_rede.tiporede.segmento = str_rede.tiporede.id;
				str_rede.tiporede.codigo = ptrQryDS->FieldByName("TipoRedeDescricao")->AsString;
				str_rede.mrede_redutivel = ptrQryDS->FieldByName("Redutivel")->AsInteger;
				str_rede.estatica = ptrQryDS->FieldByName("Estatica")->AsInteger;
				str_rede.radial = ptrQryDS->FieldByName("Radial")->AsInteger;
				str_rede.barini_id = ptrQryDS->FieldByName("BarraIniId")->AsInteger;
				str_rede.equivalente = ptrQryDS->FieldByName("Equivalente")->AsInteger;

				str_rede.codigo = ptrQryDS->FieldByName("RedeCodigo")->AsString;
				str_rede.extern_id = ptrQryDS->FieldByName("RedeExtId")->AsString;
				str_rede.barini_id = ptrQryDS->FieldByName("BarraIniId")->AsInteger;
				str_rede.cluster_id = ptrQryDS->FieldByName("AgrupamentoId")->AsInteger;
				str_rede.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;
				str_rede.area_esq.x1 = ptrQryDS->FieldByName("EsqX1")->AsFloat;
				str_rede.area_esq.y1 = ptrQryDS->FieldByName("EsqY1")->AsFloat;
				str_rede.area_esq.x2 = ptrQryDS->FieldByName("EsqX2")->AsFloat;
				str_rede.area_esq.y2 = ptrQryDS->FieldByName("EsqY2")->AsFloat;
				str_rede.color = (TColor)((int)ptrQryDS->FieldByName("ColorId")->AsInteger);

                //trata extern_id da rede, que pode ter informações de id_alim_pai
                TrataExternIdRedeLe(str_rede);

				// Essa rede aparecerá quantas vezes forem os domínios de que ela participa
				// -1 indica que não participa de nenhum domínio. O sistema pode forçar o domínio
				// padrão (que na base está cadastrado como zero?
				str_rede.dominio_id = ptrQryDS->FieldByName("DominioId")->AsInteger;

				// Antes de inserir a rede, verifica se deve inserir o Cluster
				str_cluster.cluster_id = str_rede.cluster_id;
				str_cluster.obra = str_rede.obra;
				if (str_rede.cluster_id != IDINDEFINIDO)
				{
					str_cluster.codigo = ptrQryDS->FieldByName("AgrupamentoCodigo")->AsString;
					if ((cluster = (VTCluster*)monta->InsereCluster(str_cluster)) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						SNPSTRLOG(str_cluster.codigo);
						MSGERRSYS_CLUSTERINV sucesso = false;
						break;
					}
					else
					{
						cluster->DefineStatus(sttNOVO | sttALTERADO, false);
					}
				}

				// Comentário do equipamento
				str_rede.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
				str_rede.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
				str_rede.comentario.comentario = ptrQryDS->FieldByName("Comentario")->AsString;

				// insere em Monta
				if ((rede = (VTRede*)monta->InsereRede(str_rede)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					SNPSTRLOG(str_rede.codigo);
					MSGERRSYS_REDEINV sucesso = false;
					break;
				}
				else
				{
					bd_func->DefineStatusEqpto(rede, estudo->Novo);
					if (rede->MRede)
						rede->MRede->DefineStatus(sttNOVO, estudo->Novo);
					if (lisRedeAux)
						lisRedeAux->Add(rede);
				}

				ptrQryDS->Next();
				// loop registros (end)
			}

			// Esta função aqui não pode fechar o cursor
			// ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		// Esta função aqui não pode destruir
		// if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdRedeDominio(VTEstudo* estudo, VTDominio *dominio)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!dominio)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_DOMINIOINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdRedeDominio :p_EstudoId, :p_DominioId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_DominioId", PARAMIN_INT(dominio->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			if (!SPSNP_rdRedeCursor(estudo, ptrQryDS))
			{
				SNPSTRLOGLINE(strlog);
				sucesso = false;
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdRedeMRede(VTEstudo* estudo, int intMRedeId,
	TList *lisRedeAux)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	// Não pode testar abaixo, pois o ponteiro pode ser nulo, de propósito
	// if (!lisRedeAux)                { SNPSTRLOGLINE(strlog); MSGERRSYS_LISOBJINV  return(false); }

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdRedeMRede :p_EstudoId, :p_MRedeId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_MRedeId", PARAMIN_INT(intMRedeId)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			if (!SPSNP_rdRedeCursor(estudo, ptrQryDS, lisRedeAux))
			{
				SNPSTRLOGLINE(strlog);
				sucesso = false;
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdRedeVizinha(VTEstudo* estudo, int intRedeId, TList *lisVIZ)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisVIZ)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISOBJINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdRedeVizinha :p_EstudoId, :p_RedeId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_RedeId", PARAMIN_INT(intRedeId)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			if (!SPSNP_rdRedeCursor(estudo, ptrQryDS, lisVIZ))
			{
				SNPSTRLOGLINE(strlog);
				sucesso = false;
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdRRBarra(VTEstudo* estudo, bool bolSemFiltro)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	strBARRA str_barra;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdRRBarra :p_EstudoId, :p_SemFiltro";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_SemFiltro", PARAMIN_INT(bolSemFiltro ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			if (!SPSNP_rdBarraCursor(estudo, ptrQryDS))
			{
				SNPSTRLOGLINE(strlog);
				sucesso = false;
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdRRCarga(VTEstudo* estudo, bool bolSemFiltro)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTCarga* carga;
	strCARGA str_carga;
	TStream* ptrStream;
	strCURHEADER curhdr;
	bool bolTipica;
	int intMRedeId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdRRCarga :p_EstudoId, :p_SemFiltro";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_SemFiltro", PARAMIN_INT(bolSemFiltro ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				// preenche estrutura strBARRA
				str_carga.car_id = ptrQryDS->FieldByName("RRCargaId")->AsInteger;
				str_carga.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
				str_carga.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
				str_carga.fases = faseABCN;
				str_carga.energia_mwhmes = 0;
				/*
				 !OOPS! E esses campos?
				 EqAux.fltIcte_pu                AS Icte_pu,
				 EqAux.fltScte_pu                AS Scte_pu,
				 EqAux.fltZcte_pu                AS Zcte_pu,
				 */
				str_carga.classe_cons = consEQUIVALENTE;
				str_carga.bar_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
				str_carga.esq.posbar = ptrQryDS->FieldByName("PosBar")->AsFloat;
				str_carga.esq.dxbar = ptrQryDS->FieldByName("DX")->AsInteger;
				str_carga.esq.dybar = ptrQryDS->FieldByName("DY")->AsInteger;
				str_carga.show = ptrQryDS->FieldByName("Show")->AsInteger;
				str_carga.obra = false;
				bolTipica = (bool)ptrQryDS->FieldByName("Tipica")->AsInteger;
				if (!bolTipica)
				{
					ptrStream = ptrQryDS->CreateBlobStream(ptrQryDS->FieldByName("Pontos"), bmRead);
					if (!ptrStream)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CURVAINV break;
					}
					if (!bd_func->DesmontaStreamCurvaEquipamento(ptrStream, &(str_carga.curva),
						curhdr))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CURVAINV sucesso = false;
						break;
					}

					str_carga.classe_cons = (int)curhdr.uchClasseId;
				}
				// Ao desmontar o BLOB, o header da curva é zerado. Assim, os campos devem
				// ser preenchidos posteriormente
				str_carga.curva.tipica = bolTipica;
				str_carga.curva.status = (unsigned)ptrQryDS->FieldByName("Status")->AsInteger;
				str_carga.curva.id = ptrQryDS->FieldByName("ConjCurvaId")->AsInteger;

				// Atualiza o valor de mrede pois pode ter sido passado como -1
				intMRedeId = ptrQryDS->FieldByName("MRedeId")->AsInteger;

				// insere em Monta
				if ((carga = (VTCarga*)monta->InsereReducaoCarga(intMRedeId, str_carga)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CARGAINV sucesso = false;
					break;
				}
				else
				{
					bd_func->DefineStatusEqpto(carga, estudo->Novo);
					if (carga->curva)
					{
						carga->curva->Status[sttNOVO] = estudo->Novo;
					}
				}

				ptrQryDS->Next();
				// loop registros (end)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdRRSuprimento(VTEstudo* estudo, bool bolSemFiltro)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTSuprimento* suprimento;
	strSUPRIMENTO str_suprimento;
	TStream* ptrStream;
	strCURHEADER curhdr;
	bool bolTipica;
	int intMRedeId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdRRSuprimento :p_EstudoId, :p_SemFiltro";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_SemFiltro", PARAMIN_INT(bolSemFiltro ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				str_suprimento.sup_id = ptrQryDS->FieldByName("RRSuprimentoId")->AsInteger;
				str_suprimento.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
				str_suprimento.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
				str_suprimento.tiposup = ptrQryDS->FieldByName("TipoSuprId")->AsInteger;
				str_suprimento.smax = ptrQryDS->FieldByName("Smax")->AsFloat;
				str_suprimento.pcc_3f.p = ptrQryDS->FieldByName("Pcc3F")->AsFloat;
				str_suprimento.pcc_3f.q = ptrQryDS->FieldByName("Qcc3F")->AsFloat;
				str_suprimento.pcc_ft.p = ptrQryDS->FieldByName("PccFT")->AsFloat;
				str_suprimento.pcc_ft.q = ptrQryDS->FieldByName("QccFT")->AsFloat;
				str_suprimento.zeq0.r = ptrQryDS->FieldByName("Z0REqv")->AsFloat;
				str_suprimento.zeq0.x = ptrQryDS->FieldByName("Z0XEqv")->AsFloat;
				str_suprimento.zeq1.r = ptrQryDS->FieldByName("Z1REqv")->AsFloat;
				str_suprimento.zeq1.x = ptrQryDS->FieldByName("Z1XEqv")->AsFloat;
				str_suprimento.bar_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
				str_suprimento.esq.posbar = ptrQryDS->FieldByName("PosBar")->AsFloat;
				str_suprimento.esq.dxbar = ptrQryDS->FieldByName("DX")->AsInteger;
				str_suprimento.esq.dybar = ptrQryDS->FieldByName("DY")->AsInteger;
				str_suprimento.show = ptrQryDS->FieldByName("Show")->AsInteger;
				str_suprimento.vnom_kv = ptrQryDS->FieldByName("Vnom")->AsFloat;
				str_suprimento.obra = false;
				bolTipica = (bool)ptrQryDS->FieldByName("Tipica")->AsInteger;
				if (!bolTipica)
				{
					ptrStream = ptrQryDS->CreateBlobStream(ptrQryDS->FieldByName("Pontos"), bmRead);
					if (!ptrStream)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CURVAINV break;
					}
					if (!bd_func->DesmontaStreamCurvaEquipamento(ptrStream, &(str_suprimento.curva),
						curhdr))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CURVAINV sucesso = false;
						break;
					}
				}
				// Ao desmontar o BLOB, o header da curva é zerado. Assim, os campos devem
				// ser preenchidos posteriormente
				str_suprimento.curva.tipica = bolTipica;
				str_suprimento.curva.status = (unsigned)ptrQryDS->FieldByName("Status")->AsInteger;
				str_suprimento.curva.id = ptrQryDS->FieldByName("ConjCurvaId")->AsInteger;

				// Atualiza o valor de mrede pois pode ter sido passado como -1
				intMRedeId = ptrQryDS->FieldByName("MRedeId")->AsInteger;

				// insere em Monta
				if ((suprimento = (VTSuprimento*)monta->InsereReducaoSuprimento(intMRedeId,
					str_suprimento)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_SUPRIMENTOINV sucesso = false;
					break;
				}
				else
				{
					bd_func->DefineStatusEqpto(suprimento, estudo->Novo);
					if (suprimento->curva)
					{
						suprimento->curva->Status[sttNOVO] = estudo->Novo;
					}
				}

				ptrQryDS->Next();
				// loop registros (end)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdRRTrafo(VTEstudo* estudo, bool bolSemFiltro)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTTrafo* trafo;
	strTRAFO str_trafo;
	TStream* ptrStream;
	int ndxBarra = 0;
	int intBarraId;
	// complex<double> matComplex[4][4];
	double dblaux;
	int intEquipIdPrev;
	int intEquipId;
	int intMRedeId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdRRTrafo :p_EstudoId, :p_SemFiltro";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_SemFiltro", PARAMIN_INT(bolSemFiltro ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			intEquipIdPrev = IDINDEFINIDO;
			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				intEquipId = ptrQryDS->FieldByName("RRTrafoId")->AsInteger;
				// Protege contra equipamentos com número de barras inválido
				if (intEquipIdPrev != intEquipId)
				{
					intEquipIdPrev = intEquipId;
					ndxBarra = 0;
				}
				// preenche estrutura strTRAFO
				// Atenção: os enrolamentos devem vir ordenados
				if (ndxBarra == 0)
				{
					str_trafo.trf_id = intEquipId;
					str_trafo.rede_id = ptrQryDS->FieldByName("MRedeId")->AsInteger;
					str_trafo.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
					str_trafo.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
					str_trafo.z0.r = ptrQryDS->FieldByName("Z0R")->AsFloat;
					str_trafo.z0.x = ptrQryDS->FieldByName("Z0X")->AsFloat;
					str_trafo.z1.r = ptrQryDS->FieldByName("Z1R")->AsFloat;
					str_trafo.z1.x = ptrQryDS->FieldByName("Z1X")->AsFloat;
					str_trafo.modelo_eqv = ptrQryDS->FieldByName("intModeloEqv")->AsInteger;
					str_trafo.obra = false;
					str_trafo.show = ptrQryDS->FieldByName("Show")->AsInteger;

					str_trafo.bar1_id = ptrQryDS->FieldByName("PriBarraId")->AsInteger;
					str_trafo.pri.ligacao = ptrQryDS->FieldByName("PriTipoLigacaoId")->AsInteger;
					str_trafo.pri.defasagem = 0;
					str_trafo.pri.fases = ptrQryDS->FieldByName("PriTipoFaseId")->AsInteger;
					str_trafo.pri.vnom = ptrQryDS->FieldByName("PriVnom")->AsFloat;
					str_trafo.pri.tap = str_trafo.pri.vnom;
					str_trafo.snom_mva = ptrQryDS->FieldByName("PriSnom")->AsFloat;
					str_trafo.pri.zterra.r = ptrQryDS->FieldByName("PriZterraR")->AsFloat;
					str_trafo.pri.zterra.x = ptrQryDS->FieldByName("PriZterraX")->AsFloat;

					str_trafo.bar2_id = ptrQryDS->FieldByName("SecBarraId")->AsInteger;
					str_trafo.sec.ligacao = ptrQryDS->FieldByName("SecTipoLigacaoId")->AsInteger;
					str_trafo.sec.defasagem = ptrQryDS->FieldByName("RotFase")->AsFloat;
					str_trafo.sec.fases = ptrQryDS->FieldByName("SecTipoFaseId")->AsInteger;
					str_trafo.sec.vnom = ptrQryDS->FieldByName("SecVnom")->AsFloat;
					str_trafo.sec.tap = str_trafo.sec.vnom;
					str_trafo.snom_mva = ptrQryDS->FieldByName("SecSnom")->AsFloat;
					str_trafo.sec.zterra.r = ptrQryDS->FieldByName("SecZterraR")->AsFloat;
					str_trafo.sec.zterra.x = ptrQryDS->FieldByName("SecZterraX")->AsFloat;

					// Monta a matriz
					ptrStream = ptrQryDS->CreateBlobStream(ptrQryDS->FieldByName("MatY"), bmRead);
					if (!ptrStream)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_MATYINV break;
					}
					// if (bd_func->DesmontaStreamMatComplex(ptrStream, matComplex))
					if (!bd_func->DesmontaStreamTrafoEqv(ptrStream, &str_trafo.eqv))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_MATYINV sucesso = false;
						break;
					}

					// Descobre dados da barra
					intBarraId = ptrQryDS->FieldByName("BarraId")->AsInteger;
					if (intBarraId == str_trafo.bar1_id)
					{
						str_trafo.esq.posbar1 = ptrQryDS->FieldByName("PosBar")->AsFloat;
					}
					else
					{
						str_trafo.esq.posbar2 = ptrQryDS->FieldByName("PosBar")->AsFloat;
					}
				}
				else
				{
					// Descobre dados da barra
					intBarraId = ptrQryDS->FieldByName("BarraId")->AsInteger;
					if (intBarraId == str_trafo.bar1_id)
					{
						str_trafo.esq.posbar1 = ptrQryDS->FieldByName("PosBar")->AsFloat;
					}
					else
					{
						str_trafo.esq.posbar2 = ptrQryDS->FieldByName("PosBar")->AsFloat;
					}

					// Atualiza o valor de mrede pois pode ter sido passado como -1
					intMRedeId = ptrQryDS->FieldByName("MRedeId")->AsInteger;

					if ((trafo = (VTTrafo*)monta->InsereReducaoTrafo(intMRedeId, str_trafo))
						== NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_TRAFOINV sucesso = false;
						break;
					}
					else
					{
						bd_func->DefineStatusEqpto(trafo, estudo->Novo);
					}
				}
				ndxBarra++;
				if (ndxBarra > 1)
				{
					ndxBarra = 0;
				}

				ptrQryDS->Next();
				// loop registros (end)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdRRYRef(VTEstudo* estudo, bool bolSemFiltro)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTYref* yref = NULL;
	strYREF str_yref;
	TStream* ptrStream;
	complex<double>matComplex[4][4];
	double dblaux;
	int intMRedeId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdRRYRef :p_EstudoId, :p_SemFiltro";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_SemFiltro", PARAMIN_INT(bolSemFiltro ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				// preenche estrutura strYREF
				str_yref.yref_id = ptrQryDS->FieldByName("RRYRefId")->AsInteger;
				str_yref.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
				str_yref.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
				str_yref.barra_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
				str_yref.esq.posbar = ptrQryDS->FieldByName("PosBar")->AsFloat;
				str_yref.esq.dxbar = ptrQryDS->FieldByName("DX")->AsInteger;
				str_yref.esq.dybar = ptrQryDS->FieldByName("DY")->AsInteger;
				str_yref.show = ptrQryDS->FieldByName("Show")->AsInteger;
				str_yref.y0_pu.g = ptrQryDS->FieldByName("Y0G")->AsFloat;
				str_yref.y0_pu.b = ptrQryDS->FieldByName("Y0B")->AsFloat;
				str_yref.y1_pu.g = ptrQryDS->FieldByName("Y1G")->AsFloat;
				str_yref.y1_pu.b = ptrQryDS->FieldByName("Y1B")->AsFloat;
				str_yref.obra = false;

				// Monta a matriz
				ptrStream = ptrQryDS->CreateBlobStream(ptrQryDS->FieldByName("MatY"), bmRead);
				if (!ptrStream)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MATYINV break;
				}
				if (bd_func->DesmontaStreamMatComplex(ptrStream, matComplex))
				{
					for (int i = 0; i < 4; i++)
					{
						for (int j = 0; j < 4; j++)
						{
							str_yref.maty_pu[i][j].g = matComplex[i][j].real();
							str_yref.maty_pu[i][j].b = matComplex[i][j].imag();
						}
					}
				}
				else
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CURVAINV sucesso = false;
					break;
				}

				// Atualiza o valor de mrede pois pode ter sido passado como -1
				intMRedeId = ptrQryDS->FieldByName("MRedeId")->AsInteger;

				if ((yref = (VTYref*)monta->InsereReducaoYref(intMRedeId, str_yref)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_YREFINV sucesso = false;
					break;
				}
				else
				{
					bd_func->DefineStatusEqpto(yref, estudo->Novo);
				}

				ptrQryDS->Next();
				// loop registros (end)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdSincronismo(void)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	int intSincronismoId;
	int intSincronismoIdPrev;
	int intRedeId;
	int intChaveId;
	bool bolFechada;
	bool bolMaster;
	VTChave* chave;
	VTLock* lock;
	VTLocked* locked;
	VTLocks* locks = (VTLocks*)apl->GetObject(__classid(VTLocks));

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}
	if (!locks)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SINCRONISMOSINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdSincronismo";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			intSincronismoIdPrev = IDINDEFINIDO;
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				intSincronismoId = ptrQryDS->FieldByName("SincronismoId")->AsInteger;
				intRedeId = ptrQryDS->FieldByName("RedeId")->AsInteger;
				intChaveId = ptrQryDS->FieldByName("ChaveId")->AsInteger;
				bolFechada = (bool)ptrQryDS->FieldByName("Fechada")->AsInteger;
				bolMaster = (bool)ptrQryDS->FieldByName("Master")->AsInteger;
				// verifica se é uma Manobra nova
				if (intSincronismoId != intSincronismoIdPrev)
				{ // salva ult_manobra_id lido
					intSincronismoIdPrev = intSincronismoId;
					// cria um novo objeto Manobra
					lock = locks->InsereLock();
					lock->Id = intSincronismoId;
					lock->Codigo = ptrQryDS->FieldByName("Codigo")->AsString;
					lock->Status[sttENABLED] = bool(ptrQryDS->FieldByName("Enabled")->AsInteger);
					lock->Status[sttNOVO] = false;
					lock->Status[sttALTERADO] = false;
				}
				// verifica se o objeto Manobra não foi destruído devido à ausência de Chave
				if (lock != NULL)
				{
					// determina Chave do objeto Locked
					if ((chave = bd_func->ExisteChave(redes, intRedeId, intChaveId)) == NULL)
					{
						// Chave da Manobra não disponível: descarta Manobra
						locks->RetiraLock(lock);
						lock = NULL;
					}
					else
					{
						// insere um objeto Locked no Lock
						if (bolMaster)
						{
							lock->InsereMaster(chave, bolFechada);
						}
						else
						{
							lock->InsereSlave(chave, bolFechada);
						}
					}
				}

				ptrQryDS->Next();
				// loop registros (fim)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdSuprimento(VTEstudo* estudo, bool bolUseRedeTmp,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTSuprimento* suprimento = NULL;
	strSUPRIMENTO str_suprimento;
	TStream* ptrStream;
	strCURHEADER curhdr;
	bool bolTipica;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdSuprimento_p01 :p_EstudoId, :p_AlteradoObra";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdSuprimento_p02 :p_EstudoId, :p_AlteradoObra";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				str_suprimento.sup_id = ptrQryDS->FieldByName("suprimentoId")->AsInteger;
				str_suprimento.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
				str_suprimento.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
				str_suprimento.tiposup = ptrQryDS->FieldByName("TipoSuprId")->AsInteger;
				str_suprimento.smax = ptrQryDS->FieldByName("Smax")->AsFloat;
				str_suprimento.pcc_3f.p = ptrQryDS->FieldByName("Pcc3F")->AsFloat;
				str_suprimento.pcc_3f.q = ptrQryDS->FieldByName("Qcc3F")->AsFloat;
				str_suprimento.pcc_ft.p = ptrQryDS->FieldByName("PccFT")->AsFloat;
				str_suprimento.pcc_ft.q = ptrQryDS->FieldByName("QccFT")->AsFloat;
				str_suprimento.zeq0.r = ptrQryDS->FieldByName("Z0R")->AsFloat;
				str_suprimento.zeq0.x = ptrQryDS->FieldByName("Z0X")->AsFloat;
				str_suprimento.zeq1.r = ptrQryDS->FieldByName("Z1R")->AsFloat;
				str_suprimento.zeq1.x = ptrQryDS->FieldByName("Z1X")->AsFloat;
				str_suprimento.bar_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
				str_suprimento.esq.posbar = ptrQryDS->FieldByName("PosBar")->AsFloat;
				str_suprimento.esq.dxbar = ptrQryDS->FieldByName("DX")->AsInteger;
				str_suprimento.esq.dybar = ptrQryDS->FieldByName("DY")->AsInteger;
				str_suprimento.show = ptrQryDS->FieldByName("Show")->AsInteger;
				str_suprimento.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;
				str_suprimento.vnom_kv = ptrQryDS->FieldByName("Vnom")->AsFloat;
				bolTipica = (bool)ptrQryDS->FieldByName("Tipica")->AsInteger;

				if (!bolTipica)
				{
					ptrStream = ptrQryDS->CreateBlobStream(ptrQryDS->FieldByName("Pontos"), bmRead);
					if (!ptrStream)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CURVAINV sucesso = false;
						break;
					}
					if (!bd_func->DesmontaStreamCurvaEquipamento(ptrStream, &(str_suprimento.curva),
						curhdr))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_CURVAINV sucesso = false;
						break;
					}
					// O LOBField limpa a stream
					// delete ptrStream;
				}
				// Ao desmontar o BLOB, o header da curva é zerado. Assim, os campos devem
				// ser preenchidos posteriormente
				str_suprimento.curva.tipica = bolTipica;
				str_suprimento.curva.status = (unsigned)ptrQryDS->FieldByName("Status")->AsInteger;
				str_suprimento.curva.id = ptrQryDS->FieldByName("ConjCurvaId")->AsInteger;
				// Comentário do equipamento
				str_suprimento.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
				str_suprimento.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
				str_suprimento.comentario.comentario =
					ptrQryDS->FieldByName("Comentario")->AsString;
				// insere em Monta
				if ((suprimento = (VTSuprimento*)monta->InsereSuprimento(str_suprimento)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_SUPRIMENTOINV sucesso = false;
					break;
				}
				else
				{
					bd_func->DefineStatusEqpto(suprimento, estudo->Novo);
					if (suprimento->curva)
					{
						suprimento->curva->Status[sttNOVO] = estudo->Novo;
					}
				}

				ptrQryDS->Next();
				// loop registros (fim)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdTrafo(VTEstudo* estudo, bool bolUseRedeTmp, bool bolObra)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTTrafo* trafo = NULL;
	strTRAFO str_trafo;
	int ndxEnrol;
	int intEquipIdPrev;
	int intEquipId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdTrafo_p01 :p_EstudoId, :p_AlteradoObra";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdTrafo_p02 :p_EstudoId, :p_AlteradoObra";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			intEquipIdPrev = IDINDEFINIDO;
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				intEquipId = ptrQryDS->FieldByName("TrafoId")->AsInteger;
				// Protege contra equipamentos com número de enrolamentos inválido
				if (intEquipIdPrev != intEquipId)
				{
					intEquipIdPrev = intEquipId;
					ndxEnrol = 0;
				}
				// preenche estrutura strTRAFO
				// Atenção: os enrolamentos devem vir ordenados
				if (ndxEnrol == 0)
				{
					str_trafo.trf_id = intEquipId;
					str_trafo.rede_id = ptrQryDS->FieldByName("RedeId")->AsInteger;
					str_trafo.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
					str_trafo.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
					str_trafo.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;

					str_trafo.auto_trafo = (bool)ptrQryDS->FieldByName("AutoTrafo")->AsInteger;
					str_trafo.perda_ferro = ptrQryDS->FieldByName("PerdaFerro")->AsFloat;
					str_trafo.ltc.tipo = ptrQryDS->FieldByName("LtcTipoUtl")->AsFloat;
					str_trafo.ltc.bar_id = ptrQryDS->FieldByName("LtcBarRefId")->AsInteger;
					str_trafo.ltc.v_pu = ptrQryDS->FieldByName("LtcVpu")->AsFloat;
					str_trafo.z0.r = ptrQryDS->FieldByName("Z0R")->AsFloat;
					str_trafo.z0.x = ptrQryDS->FieldByName("Z0X")->AsFloat;
					str_trafo.z1.r = ptrQryDS->FieldByName("Z1R")->AsFloat;
					str_trafo.z1.x = ptrQryDS->FieldByName("Z1X")->AsFloat;

					str_trafo.pri.ligacao = ptrQryDS->FieldByName("Enrol_TipoLigacao")->AsInteger;
					str_trafo.pri.fases = ptrQryDS->FieldByName("Enrol_TipoFase")->AsInteger;
					str_trafo.pri.vnom = ptrQryDS->FieldByName("Enrol_Vnom")->AsFloat;
					str_trafo.snom_mva = ptrQryDS->FieldByName("Enrol_Snom")->AsFloat;
					str_trafo.pri.defasagem = ptrQryDS->FieldByName("Enrol_RotFase")->AsFloat;
					str_trafo.pri.tap = ptrQryDS->FieldByName("Enrol_Vtap")->AsFloat;
					str_trafo.pri.zterra.r = ptrQryDS->FieldByName("Enrol_ZterraR")->AsFloat;
					str_trafo.pri.zterra.x = ptrQryDS->FieldByName("Enrol_ZterraX")->AsFloat;

					str_trafo.bar1_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
					str_trafo.esq.posbar1 = ptrQryDS->FieldByName("PosBar")->AsFloat;
					str_trafo.show = ptrQryDS->FieldByName("Show")->AsInteger;

					// Comentário do equipamento
					str_trafo.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
					str_trafo.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
					str_trafo.comentario.comentario = ptrQryDS->FieldByName("Comentario")->AsString;
				}
				else
				{
					str_trafo.sec.ligacao = ptrQryDS->FieldByName("Enrol_TipoLigacao")->AsInteger;
					str_trafo.sec.fases = ptrQryDS->FieldByName("Enrol_TipoFase")->AsInteger;
					str_trafo.sec.vnom = ptrQryDS->FieldByName("Enrol_Vnom")->AsFloat;
					// str_trafo.snom_mva           = ptrQryDS->FieldByName("Enrol_Snom");
					str_trafo.sec.defasagem = ptrQryDS->FieldByName("Enrol_RotFase")->AsFloat;
					str_trafo.sec.tap = ptrQryDS->FieldByName("Enrol_Vtap")->AsFloat;
					str_trafo.sec.zterra.r = ptrQryDS->FieldByName("Enrol_ZterraR")->AsFloat;
					str_trafo.sec.zterra.x = ptrQryDS->FieldByName("Enrol_ZterraX")->AsFloat;

					str_trafo.bar2_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
					str_trafo.esq.posbar2 = ptrQryDS->FieldByName("PosBar")->AsFloat;

					if (cronometro)
						cronometro->Start("Trafo Monta");
					if ((trafo = (VTTrafo*)monta->InsereTrafo(str_trafo)) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_TRAFOINV sucesso = false;
						break;
					}
					else
					{
						bd_func->DefineStatusEqpto(trafo, estudo->Novo);
					}
					if (cronometro)
						cronometro->Stop("Trafo Monta");

				}
				ndxEnrol++;
				if (ndxEnrol > 1)
				{
					ndxEnrol = 0;
				}

				ptrQryDS->Next();
				// loop registros (end)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdTrafo3E(VTEstudo* estudo, bool bolUseRedeTmp, bool bolObra)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTTrafo3E* trafo3e = NULL;
	strTRAFO3E str_trafo3e;
	int ndxEnrol;
	int intEquipIdPrev;
	int intEquipId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdTrafo3E_p01 :p_EstudoId, :p_AlteradoObra";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdTrafo3E_p02 :p_EstudoId, :p_AlteradoObra";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			intEquipIdPrev = IDINDEFINIDO;
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				intEquipId = ptrQryDS->FieldByName("Trafo3eId")->AsInteger;
				// Protege contra equipamentos com número de barras inválido
				if (intEquipIdPrev != intEquipId)
				{
					intEquipIdPrev = intEquipId;
					ndxEnrol = 0;
				}
				// preenche estrutura strTRAFO3E
				// Atenção: os enrolamentos devem vir ordenados
				if (ndxEnrol == 0)
				{
					str_trafo3e.trf_id = intEquipId;
					str_trafo3e.rede_id = ptrQryDS->FieldByName("RedeId")->AsInteger;
					str_trafo3e.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
					str_trafo3e.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
					str_trafo3e.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;

					str_trafo3e.perda_ferro = ptrQryDS->FieldByName("PerdaFerro")->AsFloat;
					str_trafo3e.zps0.r = ptrQryDS->FieldByName("ZPS0R")->AsFloat;
					str_trafo3e.zps0.x = ptrQryDS->FieldByName("ZPS0X")->AsFloat;
					str_trafo3e.zpt0.r = ptrQryDS->FieldByName("ZPT0R")->AsFloat;
					str_trafo3e.zpt0.x = ptrQryDS->FieldByName("ZPT0X")->AsFloat;
					str_trafo3e.zst0.r = ptrQryDS->FieldByName("ZST0R")->AsFloat;
					str_trafo3e.zst0.x = ptrQryDS->FieldByName("ZST0X")->AsFloat;
					str_trafo3e.zps1.r = ptrQryDS->FieldByName("ZPS1R")->AsFloat;
					str_trafo3e.zps1.x = ptrQryDS->FieldByName("ZPS1X")->AsFloat;
					str_trafo3e.zpt1.r = ptrQryDS->FieldByName("ZPT1R")->AsFloat;
					str_trafo3e.zpt1.x = ptrQryDS->FieldByName("ZPT1X")->AsFloat;
					str_trafo3e.zst1.r = ptrQryDS->FieldByName("ZST1R")->AsFloat;
					str_trafo3e.zst1.x = ptrQryDS->FieldByName("ZST1X")->AsFloat;

					str_trafo3e.pri.ligacao = ptrQryDS->FieldByName("Enrol_TipoLigacao")->AsInteger;
					str_trafo3e.pri.fases = ptrQryDS->FieldByName("Enrol_TipoFase")->AsInteger;
					str_trafo3e.pri.vnom = ptrQryDS->FieldByName("Enrol_Vnom")->AsFloat;
					str_trafo3e.pri.snom = ptrQryDS->FieldByName("Enrol_Snom")->AsFloat;
					str_trafo3e.pri.defasagem = ptrQryDS->FieldByName("Enrol_RotFase")->AsFloat;
					str_trafo3e.pri.tap = ptrQryDS->FieldByName("Enrol_Vtap")->AsFloat;
					str_trafo3e.pri.zterra.r = ptrQryDS->FieldByName("Enrol_ZterraR")->AsFloat;
					str_trafo3e.pri.zterra.x = ptrQryDS->FieldByName("Enrol_ZterraX")->AsFloat;

					str_trafo3e.bar1_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
					str_trafo3e.esq.posbar1 = ptrQryDS->FieldByName("PosBar")->AsFloat;
					str_trafo3e.show = ptrQryDS->FieldByName("Show")->AsInteger;

					// Comentário do equipamento
					str_trafo3e.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
					str_trafo3e.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
					str_trafo3e.comentario.comentario =
						ptrQryDS->FieldByName("Comentario")->AsString;
				}
				else

					if (ndxEnrol == 1)
				{
					str_trafo3e.sec.ligacao = ptrQryDS->FieldByName("Enrol_TipoLigacao")->AsInteger;
					str_trafo3e.sec.fases = ptrQryDS->FieldByName("Enrol_TipoFase")->AsInteger;
					str_trafo3e.sec.vnom = ptrQryDS->FieldByName("Enrol_Vnom")->AsFloat;
					str_trafo3e.sec.snom = ptrQryDS->FieldByName("Enrol_Snom")->AsFloat;
					str_trafo3e.sec.defasagem = ptrQryDS->FieldByName("Enrol_RotFase")->AsFloat;
					str_trafo3e.sec.tap = ptrQryDS->FieldByName("Enrol_Vtap")->AsFloat;
					str_trafo3e.sec.zterra.r = ptrQryDS->FieldByName("Enrol_ZterraR")->AsFloat;
					str_trafo3e.sec.zterra.x = ptrQryDS->FieldByName("Enrol_ZterraX")->AsFloat;

					str_trafo3e.bar2_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
					str_trafo3e.esq.posbar2 = ptrQryDS->FieldByName("PosBar")->AsFloat;
				}
				else
				{
					str_trafo3e.ter.ligacao = ptrQryDS->FieldByName("Enrol_TipoLigacao")->AsInteger;
					str_trafo3e.ter.fases = ptrQryDS->FieldByName("Enrol_TipoFase")->AsInteger;
					str_trafo3e.ter.vnom = ptrQryDS->FieldByName("Enrol_Vnom")->AsFloat;
					str_trafo3e.ter.snom = ptrQryDS->FieldByName("Enrol_Snom")->AsFloat;
					str_trafo3e.ter.defasagem = ptrQryDS->FieldByName("Enrol_RotFase")->AsFloat;
					str_trafo3e.ter.tap = ptrQryDS->FieldByName("Enrol_Vtap")->AsFloat;
					str_trafo3e.ter.zterra.r = ptrQryDS->FieldByName("Enrol_ZterraR")->AsFloat;
					str_trafo3e.ter.zterra.x = ptrQryDS->FieldByName("Enrol_ZterraX")->AsFloat;

					str_trafo3e.bar3_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
					str_trafo3e.esq.posbar3 = ptrQryDS->FieldByName("PosBar")->AsFloat;

					if ((trafo3e = (VTTrafo3E*)monta->InsereTrafo3E(str_trafo3e)) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_TRAFO3EINV sucesso = false;
						break;
					}
					else
					{
						bd_func->DefineStatusEqpto(trafo3e, estudo->Novo);
					}
				}
				ndxEnrol++;
				if (ndxEnrol > 2)
				{
					ndxEnrol = 0;
				}

				ptrQryDS->Next();
				// loop registros (end)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdTrafoZZ(VTEstudo* estudo, bool bolUseRedeTmp, bool bolObra)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTTrafoZZ* trafoZZ = NULL;
	strTRAFOZZ str_trafoZZ;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdTrafoZZ_p01 :p_EstudoId, :p_AlteradoObra";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdTrafoZZ_p02 :p_EstudoId, :p_AlteradoObra";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				str_trafoZZ.id = ptrQryDS->FieldByName("TrafoZZId")->AsInteger;
				str_trafoZZ.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
				str_trafoZZ.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
				str_trafoZZ.vnom = ptrQryDS->FieldByName("Vnom")->AsFloat;
				str_trafoZZ.snom = ptrQryDS->FieldByName("Snom")->AsFloat;
				str_trafoZZ.z0.r = ptrQryDS->FieldByName("Z0R")->AsFloat;
				str_trafoZZ.z0.x = ptrQryDS->FieldByName("Z0X")->AsFloat;
				str_trafoZZ.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;

				str_trafoZZ.bar_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
				str_trafoZZ.esq.posbar = ptrQryDS->FieldByName("PosBar")->AsFloat;
				str_trafoZZ.esq.dxbar = ptrQryDS->FieldByName("DX")->AsInteger;
				str_trafoZZ.esq.dybar = ptrQryDS->FieldByName("DY")->AsInteger;
				str_trafoZZ.show = ptrQryDS->FieldByName("Show")->AsInteger;

				// Comentário do equipamento
				str_trafoZZ.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
				str_trafoZZ.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
				str_trafoZZ.comentario.comentario = ptrQryDS->FieldByName("Comentario")->AsString;

				// insere em Monta
				if ((trafoZZ = (VTTrafoZZ*)monta->InsereTrafoZZ(str_trafoZZ)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_TRAFOZZINV sucesso = false;
					break;
				}
				else
				{
					bd_func->DefineStatusEqpto(trafoZZ, estudo->Novo);
				}

				ptrQryDS->Next();
				// loop registros (fim)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_rdTrecho(VTEstudo* estudo, bool bolUseRedeTmp, bool bolObra)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTBDgeral* bd_geral;
	VTTrecho* trecho = NULL;
	strTRECHO str_trecho;
	VTArranjo* arranjo;
	int intArranjoId;
	int intTipoArranjoId;
	bool bolTipico;
	int ndxBarra;
	int intEquipIdPrev;
	int intEquipId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}
	// Pega o ponteiro para o BDGeral
	bd_geral = (VTBDgeral*)apl->GetObject(__classid(VTBDgeral));
	if (bd_geral == NULL)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDGERALINV return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdTrecho_p01 :p_EstudoId, :p_AlteradoObra";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdTrecho_p02 :p_EstudoId, :p_AlteradoObra";
			}
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			intEquipIdPrev = IDINDEFINIDO;
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				intEquipId = ptrQryDS->FieldByName("TrechoId")->AsInteger;
				// Protege contra equipamentos com número de barras inválido
				if (intEquipIdPrev != intEquipId)
				{
					intEquipIdPrev = intEquipId;
					ndxBarra = 0;
				}

				// preenche estrutura strTRECHO
				if (ndxBarra == 0)
				{
					str_trecho.tre_id = intEquipId;
					str_trecho.rede_id = ptrQryDS->FieldByName("RedeId")->AsInteger;
					str_trecho.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
					str_trecho.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
					str_trecho.comp_km = ptrQryDS->FieldByName("Comprimento")->AsFloat;
					str_trecho.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;

					str_trecho.bar1_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
					str_trecho.esq.posbar1 = ptrQryDS->FieldByName("PosBar")->AsFloat;
					str_trecho.show = ptrQryDS->FieldByName("Show")->AsInteger;

					// Comentário do equipamento
					str_trecho.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
					str_trecho.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
					str_trecho.comentario.comentario =
						ptrQryDS->FieldByName("Comentario")->AsString;
				}
				else
				{
					str_trecho.bar2_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
					str_trecho.esq.posbar2 = ptrQryDS->FieldByName("PosBar")->AsFloat;

					intArranjoId = ptrQryDS->FieldByName("ArranjoId")->AsInteger;
					intTipoArranjoId = ptrQryDS->FieldByName("TipoArranjoId")->AsInteger;
					bolTipico = (bool)ptrQryDS->FieldByName("Tipico")->AsInteger;
					// insere em Monta
					if (cronometro)
						cronometro->Start("TrechoArranjo");
					if ((arranjo = bd_geral->LeArranjo(intArranjoId, intTipoArranjoId, bolTipico))
						== NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_ARRANJOINV sucesso = false;
						break;
					}
					if (cronometro)
						cronometro->Stop("TrechoArranjo");
					str_trecho.fases = arranjo->Fases;
					if (cronometro)
						cronometro->Start("TrechoInsere");
					if ((trecho = (VTTrecho*)monta->InsereTrecho(str_trecho, arranjo)) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_TRECHOINV sucesso = false;
						break;
					}
					else
					{
						bd_func->DefineStatusEqpto(trecho, estudo->Novo);
					}
					if (cronometro)
						cronometro->Stop("TrechoInsere");
				}
				ndxBarra++;
				if (ndxBarra > 1)
				{
					ndxBarra = 0;
				}

				ptrQryDS->Next();
				// loop registros (end)
			}

			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói objetos locais
		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
		{
			delete ptrQryDS;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrAgrupamento(VTEstudo *estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTCluster* cluster;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISOBJINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			// Reinicia o indexador de agrupamentos
			FNSNP_rdAgrupamentoIdProx(false, true);

			for (int nc = 0; nc < lisObjRede->Count; nc++)
			{
				cluster = (VTCluster*)lisObjRede->Items[nc];
				if (!cluster)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CLUSTERINV return (false);
				}

				if ((!cluster->StatusNovo()) && (!cluster->StatusAlterado()))
				{
					continue;
				}

				// Monta a string de execução
				// Verifica se o objeto é novo ou alterado
				if (cluster->StatusNovo() || (cluster->Id < 0))
				{
					// Trata-se de uma inserção
					// if (cluster->Id < 0)
					// {
					// cluster->Id = FNSNP_rdAgrupamentoIdProx();
					// }
					cluster->Id = FNSNP_rdAgrupamentoIdProx();
					strSQL =
						"EXEC SPSNP_wrAgrupamento_p01 :p_Codigo, :p_AlteradoObra, :p_AgrupamentoId, :p_EstudoId";
				}
				else
				{
					// Trata-se de uma atualização
					strSQL =
						"EXEC SPSNP_wrAgrupamento_p02 :p_Codigo, :p_AlteradoObra, :p_AgrupamentoId, :p_EstudoId";
				}
				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);

				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_Codigo", PARAMIN_STR(cluster->Codigo)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT((int)bolObra)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_AgrupamentoId", PARAMIN_INT(cluster->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Atualiza o status do cluster
				cluster->DefineStatus(sttNOVO | sttALTERADO, false);
			}

		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrAlternativaObra(VTEstudo *estudo)
{
	bool sucesso = true;
	double tipo_obra;
	AnsiString strSQL;
	AnsiString strlog;
	TList* lisAlternativa;
	VTAlternativa* alternativa;
	VTObras* obras;
	TList* lisObras;
	VTObra* obra;
	VTIpeAtributo *ipe_atributo;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if ((lisAlternativa = estudo->LisAlternativa()) == NULL)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISOBJINV return (false);
	}

	// Se não tem alternativas, já retorna
	if (lisAlternativa->Count == 0)
		return (true);

	// Escrita propriamente dita
	__try
	{
		try
		{
			// Determina os elementos para execução dos comandos
			for (int i = 0; i < lisAlternativa->Count; i++)
			{
				// loop Alternativas (ini)
				if ((alternativa = (VTAlternativa*)lisAlternativa->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_ALTERNATIVA return (false);
				}
				// OOPS. O teste abaixo não existe no Oracle, mas acho que poderia existir
				if (alternativa == estudo->AlternativaPadrao)
					continue;

				if ((obras = alternativa->Obras) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_OBRAS return (false);
				}

				if ((lisObras = obras->LisObra()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_LISOBJINV return (false);
				}

				// Reinicia o indexador de obras
				FNSNP_rdObraIdProx(estudo, alternativa, false, true);

				// Monta a string de execução
				strSQL =
					"EXEC SPSNP_wrAlternativaObra :p_EstudoId, :p_AlternativaId, :p_ObraId, :p_Descricao, :p_DataProjeto, :p_DataExecucao, :p_UsuarioNome, :p_Custo, :p_ObrigEspecial, :p_DeltaCustoOM, :p_AlteraTaxaFalha, :p_TaxaFalhaIns, :p_TaxaFalhaSub, :p_BolFinalizada, :p_BolBloqueada ";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);

				for (int j = 0; j < lisObras->Count; j++)
				{
					// loop obras (ini)
					if ((obra = (VTObra*)lisObras->Items[j]) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_OBRAS return (false);
					}
					/*
					 // Ajusta o identificador da obra
					 if (obra->Id < 0)
					 {
					 obra->Id = FNSNP_rdObraIdProx(estudo, alternativa);
					 if (obra->Id < 0)
					 {
					 SNPSTRLOGLINE(strlog);
					 MSGERRSYS_OBRAS
					 return(false);
					 }
					 }
					 */
					// Nota: o estudo é apagado antes de salvar, então deve renumerar as obras
					obra->Id = FNSNP_rdObraIdProx(estudo, alternativa);
					if (obra->Id < 0)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_OBRAS return (false);
					}
					/*
					 Agora o FNSNP_rdObraIdProx é lido em memória, após a primeira vez, não detonando a string
					 nas chamadas seguintes.

					 // Monta a string de execução
					 strSQL = "EXEC SPSNP_wrAlternativaObra :p_EstudoId, :p_AlternativaId, :p_ObraId, :p_Descricao, :p_DataProjeto, :p_DataExecucao, :p_UsuarioNome, :p_Custo, :p_ObrigEspecial, :p_DeltaCustoOM, :p_AlteraTaxaFalha, :p_TaxaFalhaIns, :p_TaxaFalhaSub";
					 // Decodifica comando
					 bd_conn->ParseCmd(strSQL);
					 */
					// Determina os valores dos parâmetros
					if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_AlternativaId", PARAMIN_INT(alternativa->Id)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_ObraId", PARAMIN_INT(obra->Id)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_Descricao", PARAMIN_STR(obra->Nome)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					// if (!bd_conn->SetParam("p_DataProjeto"  , PARAMIN_DTT(obra->Data)))        { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
					if (!bd_conn->SetParam("p_DataProjeto", PARAMIN_DTT(obra->DataInicio)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_DataExecucao", PARAMIN_DTT(obra->DataAtivacao)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_UsuarioNome", PARAMIN_STR(obra->Responsavel)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_ObrigEspecial", PARAMIN_FLT(obra->ObEspecial)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_Custo", PARAMIN_FLT(obra->Custo)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					// armazena tipo da Obra em em p_DeltaCustoOM
					// if (!bd_conn->SetParam("p_DeltaCustoOM" , PARAMIN_FLT(custo_zero)))        { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
					tipo_obra = obra->Tipo * 1000;
					if (!bd_conn->SetParam("p_DeltaCustoOM", PARAMIN_FLT(tipo_obra)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					/*
					 //JCG 2014.02.26 =============================
					 //alterar esse código: gravar apenas tipo da Obra, sem ID do IpeAtributo

					 //monta valor único para tipo+atributo_id e armazena em p_DeltaCustoOM
					 if (obra->LisObject->Count == 0)
					 {ipe_atributo = NULL;}
					 else
					 {ipe_atributo = (VTIpeAtributo*)obra->LisObject->First();}
					 //verifica se há um IpeAtributo
					 if (ipe_atributo == NULL)
					 {tipo_padrao = (obra->Tipo * 1000);}
					 else
					 {tipo_padrao  = (obra->Tipo * 1000) + ipe_atributo->Id;}
					 //atualiza parâmetro p_DeltaCustoOM
					 if (!bd_conn->SetParam("p_DeltaCustoOM" , PARAMIN_FLT(tipo_padrao)))        { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
					 // fim =============================
					 */
					// JCG 2016.07.29 - dados adicionais
					if (!bd_conn->SetParam("p_AlteraTaxaFalha",
						PARAMIN_INT(int(obra->AlteraTaxaFalha))))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_TaxaFalhaIns", PARAMIN_FLT(obra->TaxaFalhaInsercao)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_TaxaFalhaSub",
						PARAMIN_FLT(obra->TaxaFalhaSubstituicao)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_BolFinalizada",
						PARAMIN_INT(int(obra->Finalizada))))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_BolBloqueada",
						PARAMIN_INT(int(obra->Bloqueada))))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					// fim =============================
					// Comando da consulta
					if (!bd_conn->Execute(false))
					{
						SNPSTRLOGLINE(strlog);
						return (false);
					}
					// loop obras (fim)
				}
				// loop Alternativas (fim)
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
// OOPS: tem algum contador pronto indicando se há elementos a serem atualizados / inseridos para
// colocar um IF antes de chamar as funções
bool __fastcall TBDeqptoAccess::SPSNP_wrBarra(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	AnsiString strlog;

	// Utiliza o modo batch para salvar os equipamentos novos
	if (!SPSNP_wrBarraIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BARRAINV return (false);
	}

	// Atualiza os equipamentos por meio de procedures
	if (!SPSNP_wrBarraUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BARRAINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrBarraIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTBarra* barra;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Utiliza o modo batch para salvar os equipamentos novos
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoBARRA, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_BARRAINV return (false);
			}

			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((barra = (VTBarra*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BARRAINV return (false);
				}
				// Somente salva equipamentos novos
				if (!barra->StatusNovo())
					continue;

				// Formata a string para escrever no arquivo
				// strSQL = Format("%d;%d;%f;%d;%d;%d;%d;%d;%d",
				strSQL = Format("%d;%d;%g;%d;%d;%d;%d;%d;%d",
					ARRAYOFCONST((estudo->Id, barra->Id, barra->vnom, (int)barra->esq.x1,
					(int)barra->esq.y1, (int)barra->esq.x2, (int)barra->esq.y2, (int)barra->utm.x,
					(int)barra->utm.y)));
				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "BarraOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL +=
					"Barra (intEstudoId, intBarraId, fltVNom, intEsqX1, intEsqY1, intEsqX2, intEsqY2 ,intUtmX, intUtmY) ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrBarraUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTBarra* barra;
	int intBarraId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				barra = (VTBarra*)lisObjRede->Items[n];
				if (!barra)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BARRAINV return (false);
				}
				// Somente salva equipamentos
				if ((!barra->StatusAlterado()) && (!barra->StatusDeslocado()))
					continue;

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, barra, barra->Tipo(), IDINDEFINIDO, bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
				// Monta a string para execução
				strSQL =
					"EXEC SPSNP_wrBarra_p02 :p_VNom, :p_EsqX1, :p_EsqY1, :p_EsqX2, :p_EsqY2, :p_UtmX, :p_UtmY, :p_EstudoId, :p_BarraId";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_VNom", PARAMIN_FLT(barra->vnom)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EsqX1", PARAMIN_INT(barra->esq.x1)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EsqY1", PARAMIN_INT(barra->esq.y1)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EsqX2", PARAMIN_INT(barra->esq.x2)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EsqY2", PARAMIN_INT(barra->esq.y2)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_UtmX", PARAMIN_INT(barra->utm.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_UtmY", PARAMIN_INT(barra->utm.y)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_BarraId", PARAMIN_INT(barra->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}
			}

		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
// Em BarraRede não há atualização.
//
// Objeto previamente existente. O identificador em memória é válido.
// Conforme comentário no ORacle: "não há sentido em atualizar BarraRede"
// A chave primária é Rede, Estudo e Barra
// strSQL = "EXEC SPSNP_wrBarraRede_p02 :p_RedeId, :p_EstudoId, :p_BarraId";
bool __fastcall TBDeqptoAccess::SPSNP_wrBarraRedeIns(VTEstudo* estudo)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTBarra* barra;
	VTRede* rede;
	TList* lisRedeCarregada = NULL;
	TList* lisObjRede = NULL;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}

	__try
	{
		// Lista auxiliar para carga de equipamentos
		lisObjRede = new TList();
		lisRedeCarregada = new TList();

		try
		{
			// Determina as redes que estão carregadas, somente
			redes->LisRedeCarregada(lisRedeCarregada);
			// Apaga todas
			if (!SPSNP_dlTmpBarraRede(estudo, lisRedeCarregada))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_BARRAINV return (false);
			}
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int ndxRede = 0; ndxRede < lisRedeCarregada->Count; ndxRede++)
			{
				// Loops das redes (ini)
				if ((rede = (VTRede*)lisRedeCarregada->Items[ndxRede]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BARRAINV return (false);
				}
				// Conforme comentário no ORacle: "não há sentido em atualizar BarraRede"
				// POde ser que a rede não tenha sido alterada, mas a barra associada à rede sim
				// mml@20130105
				// if (!rede->StatusNovo()) continue;
				lisObjRede->Clear();
				rede->LisBarra(lisObjRede, true); // Carrega também as barras das chaves abertas
				for (int n = 0; n < lisObjRede->Count; n++)
				{
					// Loop das barras por rede (ini)
					if ((barra = (VTBarra*)lisObjRede->Items[n]) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_BARRAINV return (false);
					}
					// Somente insere se um dos dois for novo (barra ou rede)
					// mml@20130107: Agora salva todo mundo, pois todo mundo foi eliminado
					// if ((!rede->StatusNovo()) && (!barra->StatusNovo())) continue;

					// Formata a string para escrever no arquivo
					strSQL = Format("%d;%d;%d", ARRAYOFCONST((estudo->Id, rede->Id, barra->Id)));
					lstStrData->Add(strSQL);
					// Loop das barras por rede (fim)
				}

				// Loops das redes (fim)
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "BarraRedeOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "BarraRede (intEstudoId, intRedeId, intBarraId) ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói lista sem destruir seus objetos
		if (lisObjRede)
		{
			delete lisObjRede;
			lisObjRede = NULL;
		}
		if (lisRedeCarregada)
		{
			delete lisRedeCarregada;
			lisRedeCarregada = NULL;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/13952424
bool __fastcall TBDeqptoAccess::SPSNP_wrBateria(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	AnsiString strlog;

	// Insere as baterias em modo batch
	if (!SPSNP_wrBateriaIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BATERIAINV return (false);
	}

	// Atualiza as baterias por meio de procedures
	if (!SPSNP_wrBateriaUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BATERIAINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/13952424
bool __fastcall TBDeqptoAccess::SPSNP_wrBateriaIns(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTBateria *bateria;
	VTCurva* curva;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Inicialmente, cria todas as curvas que não são típicas e atribui o identificador ao equipamento.
			// Assim, não há necessidade de fazer a atualização no final
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((bateria = (VTBateria*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BATERIAINV return (false);
				}
				// Somente salva equipamentos novos
				if (!bateria->StatusNovo())
					continue;

				if ((curva = bateria->curvaCargaDescarga) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BDCURVAINV return (false);
				}
				if (curva->NumeroPontos() > 0)
				{
					// Se a curva for típica e o status não for novo, não precisa salvar a curva
					if ((curva->Status[sttNOVO] == true) && (!curva->Tipica))
					{
						// Inicialmente, escreve a curva, cuja função já existe na base de dados geral
						// se a curva não for típica e a descrição estiver vazia, associa o código do equipamento à curva
						if ((!curva->Tipica) && (curva->Codigo.IsEmpty()))
						{
							curva->Codigo = bateria->Codigo;
						}
						if (!bd_curva->SPSNP_wrConjCurva(bateria->Tipo(), curva))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRSYS_CURVAINV return (false);
						}
					}
				}
			}

			// Nesse ponto, todas as curvas próprias já foram salvas na base e o id gerado associado
			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoBATERIA, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}

			// -------------------------
			// Inserção das baterias (ini)
			// -------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((bateria = (VTBateria*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BARRAINV return (false);
				}
				// Somente salva equipamentos novos
				if (!bateria->StatusNovo())
					continue;
				// Formata a string para escrever no arquivo
				// strSQL = Format("%d;%d;%d;%d;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f",
				strSQL = Format("%d;%d;%d;%d;%g;%g;%g;%d;%d;%g;%g;%g;%g;%g;%g;%g;%g;%g",
					ARRAYOFCONST((estudo->Id, bateria->Id, bateria->Fases, bateria->Tipo_operacao,
					bateria->Icte_pu, bateria->Zcte_pu, bateria->Scte_pu, bateria->ModeloCarga,
					bateria->ModeloDesc, bateria->Pnom_kw, bateria->Enom_kwh, bateria->Cap_inv_kva,
					bateria->P_ociosa_pu, bateria->Q_ociosa_pu, bateria->Rend_carga_pu,
					bateria->Rend_desc_pu, bateria->Gatilho_carga_pu, bateria->Gatilho_desc_pu)));
				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "BateriaOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "Bateria (intEstudoId,intBateriaId,intTipoFaseId,intTipoOperacaoId,";
				strSQL +=
					"fltIctepu,fltZctepu,fltSctepu,intModeloCarga,";
				strSQL +=
					"intModeloDescarga,fltPnomkW,fltEnomkWh,fltCapinvkVA,";
				strSQL +=
					"fltPociosapu,fltQociosapu,fltRendCargapu,";
				strSQL +=
					"fltRendDescpu,fltGatilhoCargapu,fltGatilhoDescpu";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			// -------------------------
			// Inserção das baterias (fim)
			// -------------------------

			// Associa as barras aos equipamentos em modo batch
			if (!SPSNP_wrEquipamentoBarraIns(estudo, lisObjRede))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_BATERIAINV return (false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/13952424
bool __fastcall TBDeqptoAccess::SPSNP_wrBateriaUpd(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTBateria* bateria;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}
	if (!bd_curva)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDCURVAINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				bateria = (VTBateria*)lisObjRede->Items[n];
				if (!bateria)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BATERIAINV return (false);
				}
				// Somente salva equipamentos que não são novos
				if ((!bateria->StatusAlterado()) && (!bateria->StatusDeslocado()))
					continue;

				if (!bateria->curvaCargaDescarga)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CURVAINV return (false);
				}

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, bateria, bateria->Tipo(), IDINDEFINIDO,
					bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
				// Monta a string de execução
				strSQL = "EXEC SPSNP_wrBateria_p02 :p_TipoFaseId, :p_TipoOperacaoId, ";
				strSQL +=
					":p_IctePu, :p_ZctePu, :p_SctePu, :p_ModeloCarga, :p_ModeloDescarga, :p_PnomkW, :p_EnomkWh, ";
				strSQL +=
					":p_CapinvkVA, :p_Pociosapu, :p_Qociosapu, :p_RendCargapu, :p_RendDescpu, ";
			    strSQL
					+= ":p_GatilhoCargapu, :p_GatilhoDescpu, :p_EstudoId, :p_BateriaId";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_TipoFaseId", PARAMIN_INT(bateria->Fases)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_TipoOperacaoId", PARAMIN_INT(bateria->Tipo_operacao)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_BateriaId", PARAMIN_INT(bateria->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_IctePu", PARAMIN_FLT(bateria->Icte_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ZctePu", PARAMIN_FLT(bateria->Zcte_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_SctePu", PARAMIN_FLT(bateria->Scte_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				if (!bd_conn->SetParam("p_ModeloCarga", PARAMIN_INT(bateria->ModeloCarga)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ModeloDescarga", PARAMIN_INT(bateria->ModeloDesc)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_PnomkW", PARAMIN_FLT(bateria->Pnom_kw)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EnomkWh", PARAMIN_FLT(bateria->Enom_kwh)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_CapinvkVA", PARAMIN_FLT(bateria->Cap_inv_kva)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Pociosapu", PARAMIN_FLT(bateria->P_ociosa_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Qociosapu", PARAMIN_FLT(bateria->Q_ociosa_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_RendCargapu", PARAMIN_FLT(bateria->Rend_carga_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_RendDescpu", PARAMIN_FLT(bateria->Rend_desc_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_GatilhoCargapu", PARAMIN_FLT(bateria->Gatilho_carga_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_GatilhoDescpu", PARAMIN_FLT(bateria->Gatilho_desc_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Escreve a curva da carga
				if (bateria->curvaCargaDescarga->NumeroPontos() > 0)
				{
					if (!bd_curva->SPSNP_wrConjCurvaEquipamento(estudo, bateria, bateria->curvaCargaDescarga))
					{
						sucesso = false;
						SNPSTRLOGLINE(strlog);
					}
				}
				// Atualiza posicionamento na barra
				if (!SPSNP_wrEquipamentoBarraUpd(estudo, bateria))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TBDeqptoAccess::SPSNP_wrCanal(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
 {
 Alerta("TBDeqptoAccess::SPSNP_wrCanal() - bloqueada");
 return(true);
 }
 */

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrCanal(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	AnsiString strlog;

	// redefine todos os Canais com status sstNOVO porque eles serão apagados e regravados da base
	RenovaStatus(lisObjRede);
	// Insere os canais em modo batch
	if (!SPSNP_wrCanalIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_CANALINV return (false);
	}
	// Atualiza os canais por meio de procedures
	if (!SPSNP_wrCanalUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_CANALINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrCanalIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTCanal* canal;
	VTCurva* curva;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Inicialmente, cria todas as curvas que não são típicas e atribui o identificador ao equipamento.
			// Assim, não há necessidade de fazer a atualização no final
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((canal = (VTCanal*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CANALINV return (false);
				}
				// Somente salva equipamentos novos
				if (!canal->StatusNovo())
					continue;

				if ((curva = canal->Curva) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BDCURVAINV return (false);
				}
				if (!canal->Medidor)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MEDIDORINV return (false);
				}
				if (curva->NumeroPontos() > 0)
				{
					// Se a curva for típica e o status não for novo, não precisa salvar a curva
					if ((curva->Status[sttNOVO] == true) && (!curva->Tipica))
					{
						// Inicialmente, escreve a curva, cuja função já existe na base de dados geral
						// se a curva não for típica e a descrição estiver vazia, associa o código do equipamento à curva
						if ((!curva->Tipica) && (curva->Codigo.IsEmpty()))
						{
							curva->Codigo = canal->Codigo;
						}
						if (!bd_curva->SPSNP_wrConjCurva(canal->Tipo(), curva))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRSYS_CURVAINV return (false);
						}
					}
				}
			}

			// Nesse ponto, todas as curvas próprias já foram salvas na base e o id gerado associado
			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoCANAL, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}

			// -------------------------------
			// Inserção dos equipamentos (ini)
			// -------------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((canal = (VTCanal*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CANALINV return (false);
				}
				// Somente salva equipamentos novos
				if (!canal->StatusNovo())
					continue;
				// Verifica o equipamento associado ao canal
				if (!canal->Eqpto)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_EQUIPINV return (false);
				}

				// Formata a string para escrever no arquivo
				// strSQL = Format("%d;%d;%d;%d;%f",
				strSQL = Format("%d;%d;%d;%d;%g;%d;%d",
					ARRAYOFCONST((estudo->Id, canal->Medidor->Id, canal->Id, canal->TipoCanal,
					canal->Tolerancia_perc, canal->Eqpto->Id, canal->Fases)));
				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "CanalOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "Canal (";
				strSQL +=
					"intEstudoId, intMedidorId, intCanalId, intTipoCanalId, fltSigma, intEquipId, intTipoFaseId";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			// -------------------------------
			// Inserção dos equipamentos (fim)
			// -------------------------------
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrCanalUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTCanal* canal;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}
	if (!bd_curva)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDCURVAINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				canal = (VTCanal*)lisObjRede->Items[n];
				if (!canal)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CARGAINV return (false);
				}
				if ((!canal->StatusAlterado()) && (!canal->StatusDeslocado()))
					continue;

				if (!canal->Curva)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CURVAINV return (false);
				}
				if (!canal->Medidor)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MEDIDORINV return (false);
				}
				if (!canal->Eqpto)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_EQUIPINV return (false);
				}

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, canal, canal->Tipo(), IDINDEFINIDO, bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
				// Monta a string de execução
				strSQL =
					"EXEC SPSNP_wrCanal_p02 :p_Sigma, :p_TipoCanalId, :p_EstudoId, :p_MedidorId, :p_CanalId, :p_EquipId, :p_TipoFaseId ";

				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_Sigma", PARAMIN_FLT(canal->Tolerancia_perc)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_TipoCanalId", PARAMIN_INT(canal->TipoCanal)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_MedidorId", PARAMIN_INT(canal->Medidor->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_CanalId", PARAMIN_INT(canal->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(canal->Eqpto->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_TipoFaseId", PARAMIN_INT(canal->Fases)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Escreve a curva da carga
				if (canal->Curva->NumeroPontos() > 0)
				{
					if (!bd_curva->SPSNP_wrConjCurvaEquipamento(estudo, canal, canal->Curva))
					{
						sucesso = false;
						SNPSTRLOGLINE(strlog);
					}
				}
			}

		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrCarga(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	AnsiString strlog;
	VTCarga *carga;

	// elimina da lista as Cargas que são de IP que são escritas por um método específico
	for (int n = lisObjRede->Count - 1; n >= 0; n--)
	{
		if ((carga = (VTCarga*)lisObjRede->Items[n]) == NULL)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_CARGAINV return (false);
		}
		if (carga->classe->Tag == consIP)
		{
			lisObjRede->Remove(carga);
		}
		// salva informações de VIP e GrupoA em Extern_id
		carga->Extern_id = "";
		if (carga->VeryImportant)
			carga->Extern_id = "vip ";
		if (carga->GrupoA)
			carga->Extern_id = carga->Extern_id + "grupoA";
	}
	// Insere as cargas em modo batch
	if (!SPSNP_wrCargaIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_CARGAINV return (false);
	}

	// Atualiza as cargas por meio de procedures
	if (!SPSNP_wrCargaUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_CARGAINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrCargaIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	int NumConsRes, NumConsCom, NumConsInd, NumConsRur, NumConsOut, NumConsA4;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTCarga* carga;
	VTCurva* curva;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Inicialmente, cria todas as curvas que não são típicas e atribui o identificador ao equipamento.
			// Assim, não há necessidade de fazer a atualização no final
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((carga = (VTCarga*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CARGAINV return (false);
				}
				// Somente salva equipamentos novos
				if (!carga->StatusNovo())
					continue;

				if ((curva = carga->curva) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BDCURVAINV return (false);
				}
				if (curva->NumeroPontos() > 0)
				{
					// Se a curva for típica e o status não for novo, não precisa salvar a curva
					if ((curva->Status[sttNOVO] == true) && (!curva->Tipica))
					{
						// Inicialmente, escreve a curva, cuja função já existe na base de dados geral
						// se a curva não for típica e a descrição estiver vazia, associa o código do equipamento à curva
						if ((!curva->Tipica) && (curva->Codigo.IsEmpty()))
						{
							curva->Codigo = carga->Codigo;
						}
						if (!bd_curva->SPSNP_wrConjCurva(carga->Tipo(), curva))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRSYS_CURVAINV return (false);
						}
					}
				}
			}

			// Nesse ponto, todas as curvas próprias já foram salvas na base e o id gerado associado
			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoCARGA, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}

			// -------------------------
			// Inserção das cargas (ini)
			// -------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((carga = (VTCarga*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BARRAINV return (false);
				}
				// Somente salva equipamentos novos
				if (!carga->StatusNovo())
					continue;
				// determina número de consumidores
				NumConsRes = carga->NumConsResidencial;
				NumConsCom = carga->NumConsComercial;
				NumConsInd = carga->NumConsIndustrial;
				NumConsRur = carga->NumConsRural;
				NumConsOut = carga->NumConsOutros;
				NumConsA4 = carga->NumConsA4;
				// Formata a string para escrever no arquivo
				// strSQL = Format("%d;%d;%d;%d;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f",
				strSQL = Format("%d;%d;%d;%d;%g;%g;%g;%g;%d;%d;%d;%d;%d;%d",
					ARRAYOFCONST((estudo->Id, carga->Id, carga->Fases, carga->classe->Id,
					carga->Energia_mwhmes, carga->Icte_pu, carga->Zcte_pu, carga->Scte_pu,
					NumConsRes, NumConsCom, NumConsInd, NumConsRur, NumConsOut, NumConsA4)));
				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "CargaOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "Carga (";
				strSQL +=
					"intEstudoId, intCargaId, intTipoFaseId, intTipoConsId, fltEnergia, fltIctepu, fltZctepu, fltSctepu,";
				strSQL +=
					"intNumConsRes, intNumConsCom, intNumConsInd, intNumConsRur, intNumConsOut, intNumConsA4";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			// -------------------------
			// Inserção das cargas (fim)
			// -------------------------

			// Associa as barras aos equipamentos em modo batch
			if (!SPSNP_wrEquipamentoBarraIns(estudo, lisObjRede))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_CARGAINV return (false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrCargaUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	int NumConsRes, NumConsCom, NumConsInd, NumConsRur, NumConsOut, NumConsA4;
	AnsiString strSQL;
	AnsiString strlog;
	VTCarga* carga;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}
	if (!bd_curva)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDCURVAINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				carga = (VTCarga*)lisObjRede->Items[n];
				if (!carga)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CARGAINV return (false);
				}
				// Somente salva equipamentos que não são novos
				if ((!carga->StatusAlterado()) && (!carga->StatusDeslocado()))
					continue;

				if (!carga->curva)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CURVAINV return (false);
				}
				// Cargas de iluminação pública são escritas por um método específico
				if (carga->curva->Tipo == curvaLD)
				{
					continue;
				}

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, carga, carga->Tipo(), IDINDEFINIDO, bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
				// determina número de consumidores
				NumConsRes = carga->NumConsResidencial;
				NumConsCom = carga->NumConsComercial;
				NumConsInd = carga->NumConsIndustrial;
				NumConsRur = carga->NumConsRural;
				NumConsOut = carga->NumConsOutros;
				NumConsA4 = carga->NumConsA4;
				// Monta a string de execução
				strSQL = "EXEC SPSNP_wrCarga_p02 :p_TipoFaseId, :p_TipoConsId, :p_Energia, ";
				strSQL +=
					":p_ICtePu, :p_ZCtePu, :p_SCtePu, :p_NumConsRes, :p_NumConsCom, :p_NumConsInd,";
				strSQL += ":p_NumConsRur, :p_NumConsOut, :p_NumConsA4, :p_EstudoId, :p_CargaId";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_TipoFaseId", PARAMIN_INT(carga->Fases)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_TipoConsId", PARAMIN_INT(carga->classe->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Energia", PARAMIN_FLT(carga->Energia_mwhmes)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_CargaId", PARAMIN_INT(carga->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ICtePu", PARAMIN_FLT(carga->Icte_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ZCtePu", PARAMIN_FLT(carga->Zcte_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_SCtePu", PARAMIN_FLT(carga->Scte_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// número de consumidores
				if (!bd_conn->SetParam("p_NumConsRes", PARAMIN_INT(NumConsRes)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_NumConsCom", PARAMIN_INT(NumConsCom)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_NumConsInd", PARAMIN_INT(NumConsInd)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_NumConsRur", PARAMIN_INT(NumConsRur)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_NumConsOut", PARAMIN_INT(NumConsOut)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_NumConsA4", PARAMIN_INT(NumConsA4)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Escreve a curva da carga
				if (carga->curva->NumeroPontos() > 0)
				{
					if (!bd_curva->SPSNP_wrConjCurvaEquipamento(estudo, carga, carga->curva))
					{
						sucesso = false;
						SNPSTRLOGLINE(strlog);
					}
				}
				// Atualiza posicionamento na barra
				if (!SPSNP_wrEquipamentoBarraUpd(estudo, carga))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrCargaIP(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	AnsiString strlog;
	VTCarga *carga;

	// retira da lista as cargas que não de Ip, que são gravadas por método específico
	for (int n = lisObjRede->Count - 1; n >= 0; n--)
	{
		if ((carga = (VTCarga*)lisObjRede->Items[n]) == NULL)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_CARGAINV return (false);
		}
		if (carga->classe->Tag != consIP)
		{
			lisObjRede->Remove(carga);
		}
	}
	// Insere as cargas em modo batch
	if (!SPSNP_wrCargaIPIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_CARGAINV return (false);
	}

	// Atualiza as cargas por meio de procedures
	if (!SPSNP_wrCargaIPUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_CARGAINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrCargaIPIns(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTCarga* carga;
	VTCurva* curva;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Inicialmente, cria todas as curvas que não são típicas e atribui o identificador ao equipamento.
			// Assim, não há necessidade de fazer a atualização no final
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((carga = (VTCarga*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CARGAINV return (false);
				}
				// Somente salva equipamentos novos
				if (!carga->StatusNovo())
					continue;

				if ((curva = carga->curva) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BDCURVAINV return (false);
				}
				if (curva->NumeroPontos() > 0)
				{
					// Se a curva for típica e o status não for novo, não precisa salvar a curva
					if ((curva->Status[sttNOVO] == true) && (!curva->Tipica))
					{
						// Inicialmente, escreve a curva, cuja função já existe na base de dados geral
						// se a curva não for típica e a descrição estiver vazia, associa o código do equipamento à curva
						if ((!curva->Tipica) && (curva->Codigo.IsEmpty()))
						{
							curva->Codigo = carga->Codigo;
						}
						if (!bd_curva->SPSNP_wrConjCurva(carga->Tipo(), curva))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRSYS_CURVAINV return (false);
						}
					}
				}
			}

			// Nesse ponto, todas as curvas próprias já foram salvas na base e o id gerado associado
			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoCARGA, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}

			// -------------------------
			// Inserção das cargas (ini)
			// -------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((carga = (VTCarga*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BARRAINV return (false);
				}
				// Somente salva equipamentos novos
				if (!carga->StatusNovo())
					continue;

				// Formata a string para escrever no arquivo
				// strSQL = Format("%d;%d;%d;%f;%f;%d",
				strSQL = Format("%d;%d;%d;%g;%g;%d;%f;%f;%f", ARRAYOFCONST((
					// estudo->Id, carga->Id, carga->Fases, carga->IP_Pot_kw, carga->IP_Fatpot, carga->IP_NumLamp
					estudo->Id, carga->Id, carga->Fases, carga->IP_Pot_kw, carga->IP_Fatpot,
					(carga->ModeloCarga << 16) + carga->IP_NumLamp, carga->Icte_pu, carga->Zcte_pu,
					carga->Scte_pu)));

				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "CargaIPOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "CargaIP (";
				strSQL +=
					"intEstudoId, intCargaIPId, intTipoFaseId, fltPnom, fltFatorPot, intNumLamp, fltICtepu, fltZCtepu, fltSCtepu";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			// -------------------------
			// Inserção das cargas (fim)
			// -------------------------

			// Associa as barras aos equipamentos em modo batch
			if (!SPSNP_wrEquipamentoBarraIns(estudo, lisObjRede))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_CARGAINV return (false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrCargaIPUpd(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	int modelo_num_lamp;
	AnsiString strSQL;
	AnsiString strlog;
	VTCarga* carga;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}
	if (!bd_curva)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDCURVAINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				carga = (VTCarga*)lisObjRede->Items[n];
				if (!carga)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CARGAINV return (false);
				}
				// Somente salva equipamentos que não são novos
				if (carga->StatusNovo())
					continue;

				if (!carga->curva)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CURVAINV return (false);
				}
				// Cargas de iluminação pública são escritas por um método específico
				if (carga->curva->Tipo != curvaLD)
				{
					continue;
				}

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, carga, carga->Tipo(), IDINDEFINIDO, bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
				// Monta a string de execução
				strSQL =
					"EXEC SPSNP_wrCargaIP_p02 :p_TipoFaseId, :p_Pnom, :p_FatorPot, :p_NumLamp, :p_ICte, :p_ZCte, :p_SCte, :p_EstudoId, :p_CargaId";

				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				modelo_num_lamp = (carga->ModeloCarga << 16) + carga->IP_NumLamp;
				if (!bd_conn->SetParam("p_TipoFaseId", PARAMIN_INT(carga->Fases)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Pnom", PARAMIN_FLT(carga->IP_Pot_kw)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_FatorPot", PARAMIN_FLT(carga->IP_Fatpot)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// if (!bd_conn->SetParam("p_NumLamp"       , PARAMIN_INT(carga->IP_NumLamp)))  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
				if (!bd_conn->SetParam("p_NumLamp", PARAMIN_INT(modelo_num_lamp)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_CargaId", PARAMIN_INT(carga->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ICte", PARAMIN_FLT(carga->Icte_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ZCte", PARAMIN_FLT(carga->Zcte_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_SCte", PARAMIN_FLT(carga->Scte_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Escreve a curva da carga
				if (carga->curva->NumeroPontos() > 0)
				{
					if (!bd_curva->SPSNP_wrConjCurvaEquipamento(estudo, carga, carga->curva))
					{
						sucesso = false;
						SNPSTRLOGLINE(strlog);
					}
				}
				// Atualiza posicionamento na barra
				if (!SPSNP_wrEquipamentoBarraUpd(estudo, carga))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrCargaNL(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTCargaNL* cargaNL;
	strFUN* ptrFUN;
	strHRM* ptrHRM;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			// Reinicia o indexador de equipamentos
			FNSNP_rdEquipIdProx(estudo, false, true);

			for (int n = 0; n < lisObjRede->Count; n++)
			{
				cargaNL = (VTCargaNL*)lisObjRede->Items[n];
				if (!cargaNL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CARGAINV return (false);
				}
				// Verifica se o objeto é novo ou alterado
				if (cargaNL->StatusNovo())
				{ // Salva o equipamento (a eventual atribuição de identficador é feita aqui)
					if (!SPSNP_wrEquipamentoIns(estudo, cargaNL, cargaNL->Tipo(), IDINDEFINIDO,
						bolObra))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBDWR_EQPTO return (false);
					}
					// Objeto novo
					strSQL =
						"EXEC SPSNP_wrCargaNL_p01 :p_FatorPot, :p_Indutivo, :p_kVA, :p_SNomA, :p_SNomB, :p_SNomC, :p_AngA, :p_AngB, :p_AngC, :p_EstudoId, :p_CargaNLId, :p_OrdemId";
				}
				else if (cargaNL->StatusAlterado() || (cargaNL->StatusDeslocado()))
				{ // atualiza o equipamento (a eventual atribuição de identficador é feita aqui)
					if (!SPSNP_wrEquipamentoUpd(estudo, cargaNL, cargaNL->Tipo(), IDINDEFINIDO,
						bolObra))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBDWR_EQPTO return (false);
					}
					// Objeto previamente existente. O identificador em memória é válido.
					strSQL =
						"EXEC SPSNP_wrCargaNL_p02 :p_FatorPot, :p_Indutivo, :p_kVA, :p_SNomA, :p_SNomB, :p_SNomC, :p_AngA, :p_AngB, :p_AngC, :p_EstudoId, :p_CargaNLId, :p_OrdemId";
				}
				else
				{
					// Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
					continue;
				}
				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);

				// Determina os valores dos parâmetros

				// Inicialmente salva a fundamental
				if ((ptrFUN = cargaNL->LeFundamental()) == NULL)
					return (false);
				if (!bd_conn->SetParam("p_FatorPot", PARAMIN_FLT(cargaNL->FatPot_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Indutivo", PARAMIN_INT(cargaNL->Indutiva)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_kVA", PARAMIN_INT((int)true)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_SNomA", PARAMIN_FLT(ptrFUN->sa_mod_kva)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_SNomB", PARAMIN_FLT(ptrFUN->sb_mod_kva)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_SNomC", PARAMIN_FLT(ptrFUN->sc_mod_kva)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_AngA", PARAMIN_FLT(ptrFUN->sa_ang_grau)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_AngB", PARAMIN_FLT(ptrFUN->sb_ang_grau)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_AngC", PARAMIN_FLT(ptrFUN->sc_ang_grau)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_CargaNLId", PARAMIN_INT(cargaNL->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_OrdemId", PARAMIN_INT(1)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Repete a execução para as harmônicas   strFUN*       ptrFUN;
				if (!bd_conn->SetParam("p_kVA", PARAMIN_INT((int)false)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				for (int j = 0; j < cargaNL->NumHarmonicas; j++)
				{
					if ((ptrHRM = cargaNL->LeHarmonica(j)) == NULL)
						return (false);

					if (!bd_conn->SetParam("p_SNomA", PARAMIN_FLT(ptrHRM->sa_mod_pu)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_SNomB", PARAMIN_FLT(ptrHRM->sb_mod_pu)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_SNomC", PARAMIN_FLT(ptrHRM->sc_mod_pu)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_AngA", PARAMIN_FLT(ptrHRM->sa_ang_grau)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_AngB", PARAMIN_FLT(ptrHRM->sb_ang_grau)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_AngC", PARAMIN_FLT(ptrHRM->sc_ang_grau)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}
					if (!bd_conn->SetParam("p_OrdemId", PARAMIN_INT(ptrHRM->ordem)))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBD_PARAMINV return (false);
					}

					// Executa query para a harmonica
					if (!bd_conn->Execute(false))
					{
						SNPSTRLOGLINE(strlog);
						return (false);
					}
				}
				// Atualiza posicionamento na barra
				if (!SPSNP_wrEquipamentoBarraUpd(estudo, cargaNL))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrCapacitor(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	AnsiString strlog;

	// Insere as cargas em modo batch
	if (!SPSNP_wrCapacitorIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_CARGAINV return (false);
	}

	// Atualiza as cargas por meio de procedures
	if (!SPSNP_wrCapacitorUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_CARGAINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrCapacitorIns(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTCapacitor* capacitor;
	VTCurva* curva;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Inicialmente, cria todas as curvas que não são típicas e atribui o identificador ao equipamento.
			// Assim, não há necessidade de fazer a atualização no final
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((capacitor = (VTCapacitor*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CAPACITORINV return (false);
				}
				// Somente salva equipamentos novos
				if (!capacitor->StatusNovo())
					continue;

				if ((curva = capacitor->curva) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BDCURVAINV return (false);
				}
				if (curva->NumeroPontos() > 0)
				{
					// Se a curva for típica e o status não for novo, não precisa salvar a curva
					if ((curva->Status[sttNOVO] == true) && (!curva->Tipica))
					{
						// Inicialmente, escreve a curva, cuja função já existe na base de dados geral
						// se a curva não for típica e a descrição estiver vazia, associa o código do equipamento à curva
						if ((!curva->Tipica) && (curva->Codigo.IsEmpty()))
						{
							curva->Codigo = capacitor->Codigo;
						}
						if (!bd_curva->SPSNP_wrConjCurva(capacitor->Tipo(), curva))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRSYS_CAPACITORINV return (false);
						}
					}
				}
			}

			// Nesse ponto, todas as curvas próprias já foram salvas na base e o id gerado associado
			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoCAPACITOR, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}

			// -------------------------------
			// Inserção dos equipamentos (ini)
			// -------------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((capacitor = (VTCapacitor*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CAPACITORINV return (false);
				}
				// Somente salva equipamentos novos
				if (!capacitor->StatusNovo())
					continue;

				// Formata a string para escrever no arquivo
				// strSQL = Format("%d;%d;%d;%f;%f",
				strSQL = Format("%d;%d;%d;%g;%g",
					ARRAYOFCONST((estudo->Id, capacitor->Id, capacitor->ligacao, capacitor->vnom,
					capacitor->q)));

				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "CapacitorOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "Capacitor (";
				strSQL += "intEstudoId, intCapacitorId, intTipoLigacaoId, fltVnom, fltQnom";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			// -------------------------------
			// Inserção dos equipamentos (fim)
			// -------------------------------

			// Associa as barras aos equipamentos em modo batch
			if (!SPSNP_wrEquipamentoBarraIns(estudo, lisObjRede))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_CAPACITORINV return (false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrCapacitorUpd(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTCapacitor* capacitor;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}
	if (!bd_curva)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDCURVAINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				capacitor = (VTCapacitor*)lisObjRede->Items[n];
				if (!capacitor)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CAPACITORINV return (false);
				}
				// Somente equipamentos alterados são permitidos aqui
				if ((!capacitor->StatusAlterado()) && (!capacitor->StatusDeslocado()))
					continue;

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, capacitor, capacitor->Tipo(), IDINDEFINIDO,
					bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
				// Monta a string de execução
				strSQL =
					"EXEC SPSNP_wrCapacitor_p02 :p_TipoLigacaoId, :p_Vnom, :p_Qnom, :p_EstudoId, :p_CapacitorId";

				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_TipoLigacaoId", PARAMIN_INT(capacitor->ligacao)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Vnom", PARAMIN_FLT(capacitor->vnom)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Qnom", PARAMIN_FLT(capacitor->q)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_CapacitorId", PARAMIN_INT(capacitor->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Escreve a curva da carga
				if (capacitor->curva->NumeroPontos() > 0)
				{
					if (!bd_curva->SPSNP_wrConjCurvaEquipamento(estudo, capacitor,
					capacitor->curva))
					{
						sucesso = false;
						SNPSTRLOGLINE(strlog);
					}
				}
				// Atualiza posicionamento na barra
				if (!SPSNP_wrEquipamentoBarraUpd(estudo, capacitor))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrCapacitorSerie(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	AnsiString strlog;

	// Insere as cargas em modo batch
	if (!SPSNP_wrCapacitorSerieIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_CAPSERIEINV return (false);
	}

	// Atualiza as cargas por meio de procedures
	if (!SPSNP_wrCapacitorSerieUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_CAPSERIEINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrCapacitorSerieIns(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTCapserie* capserie;
	int intBarRefId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoCAPSERIE, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}

			// -------------------------------
			// Inserção dos equipamentos (ini)
			// -------------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((capserie = (VTCapserie*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CAPSERIEINV return (false);
				}
				// Somente salva equipamentos novos
				if (!capserie->StatusNovo())
					continue;

				// Formata a string para escrever no arquivo
				// strSQL = Format("%d;%d;%f;%f;%f;%f",
				strSQL = Format("%d;%d;%g;%g;%g;%g",
					ARRAYOFCONST((estudo->Id, capserie->Id, capserie->vnom, capserie->snom,
					capserie->z.r, capserie->z.x)));
				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "CapSerieOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "CapacitorSerie (";
				strSQL += "intEstudoId, intCapacitorSerieId, fltVnom, fltSnom, fltZR, fltZX";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			// -------------------------------
			// Inserção dos equipamentos (fim)
			// -------------------------------

			// Atualiza posicionamento na barra
			if (!SPSNP_wrEquipamentoBarraIns(estudo, lisObjRede))
			{
				sucesso = false;
				SNPSTRLOGLINE(strlog);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrCapacitorSerieUpd(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTCapserie* capserie;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				capserie = (VTCapserie*)lisObjRede->Items[n];
				if (!capserie)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CAPSERIEINV return (false);
				}
				if ((!capserie->StatusAlterado()) && (!capserie->StatusDeslocado()))
					continue;

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, capserie, capserie->Tipo(), capserie->rede->Id,
					bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
				// Monta a string de execução
				strSQL =
					"EXEC SPSNP_wrCapacitorSerie_p02 :p_Vnom, :p_Snom, :p_ZR, :p_ZX, :p_EstudoId, :p_CapacitorSerieId";

				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_Vnom", PARAMIN_FLT(capserie->vnom)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Snom", PARAMIN_FLT(capserie->snom)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ZR", PARAMIN_FLT(capserie->z.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ZX", PARAMIN_FLT(capserie->z.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_CapacitorSerieId", PARAMIN_INT(capserie->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Atualiza posicionamento na barra
				if (!SPSNP_wrEquipamentoBarraUpd(estudo, capserie))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrChave(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	// variáveis locais
	AnsiString strlog;
	VTChave *chave;

	// CÓDIGO TEMPORÁRIO: atualiza Extern_id com informação de Elo fusível
	for (int n = 0; n < lisObjRede->Count; n++)
	{
		if ((chave = (VTChave*)lisObjRede->Items[n]) == NULL)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_CHAVEINV return (false);
		}
		// FKM 2017.12.12 - infelizmente essa gambiarra ta com um problema e o extern id
		// precisa ser usado exclusivamente para essas gambiarras, essa proxima linha
		// ignora o alguem quiser gravar no extern id... isso ja acontecia no caso da base
		// fusivel
		chave->Extern_id = "";
		if (chave->TipoBaseFusivel)
		{ // salva informação do Elo em Extern_id
			if (!chave->EloFusivel.IsEmpty())
				chave->Extern_id = chave->EloFusivel + "_elo";
		}
		// salva informação do TipoGIS em Extern_id
		if (!chave->TipoGIS.IsEmpty())
			chave->Extern_id = chave->Extern_id + chave->TipoGIS + "_gis";

		// int a = chave->Extern_id.Length();
	}
	// fim código temporário

	// Insere as chaves em modo batch
	if (!SPSNP_wrChaveIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_CHAVEINV return (false);
	}

	// Atualiza as chaves por meio de procedures
	if (!SPSNP_wrChaveUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_CHAVEINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrChaveIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTChave* chave;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoCHAVE, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}

			// -------------------------------
			// Inserção dos equipamentos (ini)
			// -------------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((chave = (VTChave*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CHAVEINV return (false);
				}
				// Somente salva equipamentos novos
				if (!chave->StatusNovo())
					continue;

				// Formata a string para escrever no arquivo
				// strSQL = Format("%d;%d;%d;%d;%f",
				strSQL = Format("%d;%d;%d;%d;%g;%d;%d;%d",
					ARRAYOFCONST((estudo->Id, chave->Id, chave->TipoChave->Id, chave->Estado,
					chave->Inom_amp, (chave->Telecomandada) ? 1 : 0, (chave->ProtecaoAtiva) ? 1 : 0,
					chave->TecnoComunicacao)));
				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "ChaveOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "Chave (";
				strSQL +=
					"intEstudoId, intChaveId, intTipoChaveId, intEstado, fltInom, bolTelecomando, bolProtecao, intTecnoCom";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}

			// Associa as barras aos equipamentos em modo batch
			if (!SPSNP_wrEquipamentoBarraIns(estudo, lisObjRede))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_TRAFOZZINV return (false);
			}
			// -------------------------------
			// Inserção dos equipamentos (fim)
			// -------------------------------
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrChaveUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTChave* chave;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				chave = (VTChave*)lisObjRede->Items[n];
				if (!chave)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CHAVEINV return (false);
				}
				if ((!chave->StatusAlterado()) && (!chave->StatusDeslocado()))
					continue;

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, chave, chave->Tipo(), chave->rede->Id, bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
				// Monta a string de execução
				// strSQL = "EXEC SPSNP_wrChave_p02 :p_TipoChaveId, :p_Estado, :p_Inom, :p_EstudoId, :p_Telecomando, :p_Protecao, :p_TecnoCom, :p_ChaveId";
				strSQL =
					"EXEC SPSNP_wrChave_p02 :p_TipoChaveId, :p_Estado, :p_Inom, :p_Telecomando, :p_Protecao, :p_TecnoCom, :p_EstudoId, :p_ChaveId";

				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_TipoChaveId", PARAMIN_INT(chave->TipoChave->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Estado", PARAMIN_INT(chave->Estado)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Inom", PARAMIN_FLT(chave->Inom_amp)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Telecomando", PARAMIN_INT((chave->Telecomandada) ?
					1 : 0)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Protecao", PARAMIN_INT((chave->ProtecaoAtiva) ? 1 : 0)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_TecnoCom", PARAMIN_INT(chave->TecnoComunicacao)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ChaveId", PARAMIN_INT(chave->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Atualiza posicionamento na barra
				if (!SPSNP_wrEquipamentoBarraUpd(estudo, chave))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrDominio(TList* lisDominio, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTDominio* dominio;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!lisDominio)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			// Reinicia o indexador de domínios
			FNSNP_rdDominioIdProx(false, true);

			for (int nc = 0; nc < lisDominio->Count; nc++)
			{
				dominio = (VTDominio*)lisDominio->Items[nc];
				if (!dominio)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_DOMINIOINV return (false);
				}

				if ((!dominio->StatusNovo()) && (!dominio->StatusAlterado()))
				{
					continue;
				}

				// Monta a string de execução
				// Verifica se o objeto é novo ou alterado
				if ((dominio->StatusNovo()) || (dominio->Id < 0))
				{
					// Trata-se de uma inserção
					// if (dominio->Id < 0)
					// mml@20130409
					// Acredito que o teste abaixo não seja necessário. Em uma ReclamacaoBT está
					// causando problema: o domínio é novo, porém o identificador vem > 0
					// {
					dominio->Id = FNSNP_rdDominioIdProx();
					// }
					strSQL = "EXEC SPSNP_wrDominio_p01 :p_Codigo, :p_DataCriacao, :p_DominioId";
				}
				else
				{
					// Trata-se de uma atualização
					strSQL = "EXEC SPSNP_wrDominio_p02 :p_Codigo, :p_DataCriacao, :p_DominioId";
				}

				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);

				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_Codigo", PARAMIN_STR(dominio->Codigo)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_DataCriacao", PARAMIN_DTT(dominio->DataCriacao)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_DominioId", PARAMIN_INT(dominio->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}
				// Define o status
				dominio->DefineStatus(sttNOVO, false);
				dominio->DefineStatus(sttALTERADO, false);
			}

		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrEstudo(VTEstudo* estudo)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		// Verifica se o objeto é novo ou alterado
		// if (estudo->Id < 0)
		// monta a string de execução: o estudo zero é pre-existente na base de dados
		if (estudo->Tipo == estudoREDE_ORIGINAL)
		{ // trata-se sempre de uma atualização
			estudo->Id = estudoID_ZERO;
			strSQL =
				"EXEC SPSNP_wrEstudo_p02 :p_Descricao, :p_UsuarioNome, :p_TipoEstudoId, :p_Justificativa, :p_EstudoId";
		}
		else if ((estudo->Novo) || (estudo->Id < 0))
		{ // trata-se de uma inserção de um novo Estudo
			estudo->Id = FNSNP_rdEstudoIdProx();
			strSQL =
				"EXEC SPSNP_wrEstudo_p01 :p_Descricao, :p_UsuarioNome, :p_TipoEstudoId, :p_EstudoId, :p_Justificativa";
		}
		else
		{ // trata-se de uma atualização de um estudo existente (diferente do estudo zero)
			strSQL =
				"EXEC SPSNP_wrEstudo_p02 :p_Descricao, :p_UsuarioNome, :p_TipoEstudoId, :p_Justificativa, :p_EstudoId";
		}

		try
		{
			// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_Descricao", PARAMIN_STR(estudo->Codigo)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_UsuarioNome", PARAMIN_STR(estudo->Autor)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_TipoEstudoId", PARAMIN_INT(estudo->Tipo)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_Justificativa", PARAMIN_STR(estudo->Justificativa)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// OOPS JCG - precisa incluir gravação de estudo->Tipo

			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16666485
//https://sinapsisenergia.teamwork.com/#/tasks/18565717
bool __fastcall TBDeqptoAccess::SPSNP_wrEquipamentoUpd(VTEstudo* estudo, VTEqpto* eqpto,
	int intTipoId, int intRedeId, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	int intDxComentario;
	int intDyComentario;
	AnsiString strComentario;
    AnsiString extern_id_orig;
	VTComentario* comentario;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!eqpto)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		// Somente atualizações. Inserções de equipamentos são executadas por outra função
		if ((!eqpto->StatusAlterado()) && (!eqpto->StatusDeslocado()))
		{
			return (true);
		}

		// Monta a string de execução
		strSQL =
			"EXEC SPSNP_wrEquipamento_p02 :p_RedeId, :p_TipoEquipId, :p_AlteradoObra, :p_Show, :p_Codigo, :p_ExtId, :p_Comentario, :p_DxComentario, :p_DyComentario, :p_EstudoId, :p_EquipId";

		try
		{
			// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			// Definições dos comentários
			intDxComentario = IDINDEFINIDO;
			intDyComentario = IDINDEFINIDO;
			strComentario = "";
			if ((comentario = redes->ExisteComentario(eqpto)) != NULL)
			{
				comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
				strComentario = comentario->Texto;
			}
            //guarda extern_id para nao ser apagado
            extern_id_orig = eqpto->Extern_id;
			// Ajusta o valor da rede. Ao salvar uma rede, o intRedeId pode vir < 0 e <> -1
			if ((intTipoId == eqptoREDE) && (intRedeId <= 0))
			{
				intRedeId = eqpto->Id;
                extern_id_orig = eqpto->Extern_id;
                TrataExternIdRedeSalva(eqpto);
			}
			if (!bd_conn->SetParam("p_RedeId", PARAMIN_INT(intRedeId)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_TipoEquipId", PARAMIN_INT(intTipoId)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_Show", PARAMIN_INT(eqpto->ShowAsUnsigned)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_Codigo", PARAMIN_STR(eqpto->Codigo)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_ExtId", PARAMIN_STR(eqpto->Extern_id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_DxComentario", PARAMIN_INT(intDxComentario)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_DyComentario", PARAMIN_INT(intDyComentario)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_Comentario", PARAMIN_STR(strComentario)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
            //retorna extern_id_orig se for rede
            if (intTipoId == eqptoREDE)
			{
                eqpto->Extern_id = extern_id_orig;
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16666485
bool __fastcall TBDeqptoAccess::SPSNP_wrEquipamentoIns(VTEstudo* estudo, VTEqpto* eqpto,
	int intTipoId, int intRedeId, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	int intDxComentario;
	int intDyComentario;
	AnsiString strComentario;
    AnsiString extern_id_orig;
	VTComentario *comentario;
	VTRede *rede;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!eqpto)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			if (!eqpto->StatusNovo())
			{
				return (true);
			}
			// Monta a string de execução: trata-se de uma inserção
			// if ((eqpto->Id < 0) || (bolObra))
			if ((eqpto->StatusNovo()) || (eqpto->Id < 0) || (bolObra))
			{
				eqpto->Id = FNSNP_rdEquipIdProx(estudo);
			}
			strSQL =
				"EXEC SPSNP_wrEquipamento_p01 :p_RedeId, :p_TipoEquipId, :p_AlteradoObra, :p_Show, :p_Codigo, :p_ExtId, :p_Comentario, :p_DxComentario, :p_DyComentario, :p_EstudoId, :p_EquipId";

			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			// Definições dos comentários
			intDxComentario = IDINDEFINIDO;
			intDyComentario = IDINDEFINIDO;
			strComentario = "";
			if ((comentario = redes->ExisteComentario(eqpto)) != NULL)
			{
				comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
				strComentario = comentario->Texto;
			}
			// Ajusta o valor da rede. Ao salvar uma rede, o intRedeId pode vir < 0 e <> -1
			if ((intTipoId == eqptoREDE) && (intRedeId <= 0))
			{
				intRedeId = eqpto->Id;
                extern_id_orig = eqpto->Extern_id;
                TrataExternIdRedeSalva(eqpto);
			}
			else
			{
				if (eqpto->TipoLigacao())
				{
					rede = ((VTLigacao*)eqpto)->rede;
					rede ? intRedeId = rede->Id : intRedeId = IDINDEFINIDO;
				}
				else if (eqpto->Tipo() == eqptoMUTUA)
				{
					rede = ((VTMutua*)eqpto)->Rede();
					rede ? intRedeId = rede->Id : intRedeId = IDINDEFINIDO;
				}
			}
			if (!bd_conn->SetParam("p_RedeId", PARAMIN_INT(intRedeId)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_TipoEquipId", PARAMIN_INT(intTipoId)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_AlteradoObra", PARAMIN_INT(bolObra ? 1 : 0)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_Show", PARAMIN_INT(eqpto->ShowAsUnsigned)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_Codigo", PARAMIN_STR(eqpto->Codigo)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_ExtId", PARAMIN_STR(eqpto->Extern_id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_DxComentario", PARAMIN_INT(intDxComentario)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_DyComentario", PARAMIN_INT(intDyComentario)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_Comentario", PARAMIN_STR(strComentario)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
            //retorna extern_id_orig se for rede
            if (intTipoId == eqptoREDE)
			{
                eqpto->Extern_id = extern_id_orig;
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16666485
// Processamento em modo batch
// somente funciona para inserções (equipamentos novos)
bool __fastcall TBDeqptoAccess::SPSNP_wrEquipamentoIns(VTEstudo* estudo, TList* lisObjRede,
	int intTipoId, int intRedeId, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTEqpto* eqpto;
	VTCurva* curva;
	int intDxComentario;
	int intDyComentario;
	AnsiString strComentario;
	VTComentario *comentario;
	VTRede *rede;
	int intRedeIdAux;
    std::map <VTEqpto*,AnsiString> map_eqpto;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((eqpto = (VTEqpto*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BARRAINV return (false);
				}
				// Somente salva equipamentos novos
				if (!eqpto->StatusNovo())
					continue;

				// Determina os valores dos parâmetros
				// eqpto->Id = FNSNP_rdEquipIdProx(estudo);
				// if ((eqpto->Id < 0) || (bolObra))
				if ((eqpto->StatusNovo()) || (eqpto->Id < 0) || (bolObra))
				{
					eqpto->Id = FNSNP_rdEquipIdProx(estudo);
				}
				// Definições dos comentários
				intDxComentario = IDINDEFINIDO;
				intDyComentario = IDINDEFINIDO;
				strComentario = "";
				if ((comentario = redes->ExisteComentario(eqpto)) != NULL)
				{
					comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
					strComentario = comentario->Texto;
				}
				// Ajusta o valor da rede. Ao salvar uma rede, o intRedeId pode vir < 0 e <> -1
				// Casos especiais: se for uma rede ou uma ligação.
				intRedeIdAux = intRedeId;
				if (intRedeId <= 0)
				{
					if (intTipoId == eqptoREDE)
					{
						intRedeIdAux = eqpto->Id;
                        //guarda extern_id original no map
                        map_eqpto[eqpto] = eqpto->Extern_id;
                        TrataExternIdRedeSalva(eqpto);
					}
					else
					{
						if (eqpto->TipoLigacao())
						{
							rede = ((VTLigacao*)eqpto)->rede;
							if (rede)
							{
								intRedeIdAux = rede->Id;
							}
						}
						else if (eqpto->Tipo() == eqptoMUTUA)
						{
							rede = ((VTMutua*)eqpto)->Rede();
							intRedeIdAux = rede->Id;
						}
					}
				}

				curva = bd_curva->CurvaEqpto(eqpto);

				// Formata a string para escrever no arquivo
				strSQL = Format("%d;%d;%d;%d;%d",
					ARRAYOFCONST((estudo->Id, eqpto->Id, intTipoId, (bolObra ? 1 : 0),
					eqpto->ShowAsUnsigned)));
				// Se não tiver rede, coloca valor nulo
				(intRedeIdAux == IDINDEFINIDO) ? straux = ";" : straux =
					Format(";%d", ARRAYOFCONST((intRedeIdAux)));
				strSQL += straux;
				// Concatena o código
				straux = Format(";%s", ARRAYOFCONST((eqpto->Codigo.c_str())));
				strSQL += straux;
				// Se não tiver identificador externo, coloca valor nulo
				(eqpto->Extern_id.IsEmpty()) ? straux = ";" : straux =
					Format(";%s", ARRAYOFCONST((eqpto->Extern_id.c_str())));
				strSQL += straux;
				// Se não tiver curva, coloca valor nulo
				(!curva) ? straux = ";" : straux = Format(";%d", ARRAYOFCONST((curva->Id)));
				strSQL += straux;
				// Se não tiver comentário, coloca valores nulos
				(strComentario.IsEmpty()) ? straux = ";;;" : straux =
					Format(";%d;%d;%s", ARRAYOFCONST((intDxComentario, intDyComentario,
					strComentario.c_str())));
				strSQL += straux;

				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "EquipOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "Equipamento (";
				strSQL +=
					"intEstudoId, intEquipId, intTipoEquipId, bolAlteradoObra, intShow, intRedeId, strCodigo, ";
				strSQL +=
					"strExtId, intConjCurvaId, intDxComentario, intDyComentario, strComentario ";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}

            //recupera extern_id original
            std::map<VTEqpto*,AnsiString>::iterator it = map_eqpto.begin();
            while(it != map_eqpto.end())
            {
                eqpto = it->first;
                eqpto->Extern_id = it->second;
                it++;
            }
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrEquipamentoBarraUpd(VTEstudo* estudo, VTEqpto* eqpto)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTBarra* barra;
	int intNumBarrasMax;
	float posbar;
	int dx, dy;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!eqpto)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			// Remove as barras antigas
			// if (!eqpto->StatusNovo())
			// OOPS: No planejamento, o status não vem !novo ?!?!
			{
				if (!SPSNP_dlEquipamentoBarra(estudo, eqpto))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}
			}

			// Monta a string de execução
			strSQL =
				"EXEC SPSNP_wrEquipamentoBarra_p01 :p_BarraId1, :p_PosBar1, :p_Dx, :p_Dy, :p_EstudoId, :p_EquipId";
			// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// O comando deve ser executado para o número de barras do equipamento
			(eqpto->TipoEqbar()) ? intNumBarrasMax = 1 : intNumBarrasMax = NBAR_LIG;
			// loop das barras, de acordo com o tipo de equipamento (EqBar ou Ligação)
			for (int i = 0; i < intNumBarrasMax; i++)
			{
				// Loop barras (ini)
				if (intNumBarrasMax == 1)
				{
					barra = ((VTEqbar*)eqpto)->pbarra;
					posbar = ((VTEqbar*)eqpto)->esq.posbar;
					dx = ((VTEqbar*)eqpto)->esq.dxbar;
					dy = ((VTEqbar*)eqpto)->esq.dybar;
				}
				else
				{
					barra = ((VTLigacao*)eqpto)->Barra(i);
					if (!barra)
						continue;
					switch (i)
					{
					case 0:
						posbar = ((VTLigacao*)eqpto)->esq.posbar1;
						break;
					case 1:
						posbar = ((VTLigacao*)eqpto)->esq.posbar2;
						break;
					case 2:
						posbar = ((VTLigacao*)eqpto)->esq.posbar3;
						break;
					}
					dx = IDINDEFINIDO;
					dy = IDINDEFINIDO;
				}

				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_BarraId1", PARAMIN_INT(barra->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_PosBar1", PARAMIN_FLT(posbar)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Dx", PARAMIN_INT(dx)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Dy", PARAMIN_INT(dy)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}
				// Loop barras (fim)
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrEquipamentoBarraIns(VTEstudo* estudo, TList* lisObjRede)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTEqpto* eqpto;
	int intNumBarrasMax;
	VTBarra* barra;
	float posbar;
	int dx, dy;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((eqpto = (VTEqpto*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_EQUIPINV return (false);
				}
				// Somente salva as barras de equipamentos novos
				if (!eqpto->StatusNovo())
					continue;
				// O comando deve ser executado para o número de barras do equipamento
				(eqpto->TipoEqbar()) ? intNumBarrasMax = 1 : intNumBarrasMax = NBAR_LIG;
				// loop das barras, de acordo com o tipo de equipamento (EqBar ou Ligação)
				for (int i = 0; i < intNumBarrasMax; i++)
				{
					// Loop barras (ini)
					if (intNumBarrasMax == 1)
					{
						barra = ((VTEqbar*)eqpto)->pbarra;
						posbar = ((VTEqbar*)eqpto)->esq.posbar;
						dx = ((VTEqbar*)eqpto)->esq.dxbar;
						dy = ((VTEqbar*)eqpto)->esq.dybar;
					}
					else
					{
						barra = ((VTLigacao*)eqpto)->Barra(i);
						if (!barra)
							continue;
						switch (i)
						{
						case 0:
							posbar = ((VTLigacao*)eqpto)->esq.posbar1;
							break;
						case 1:
							posbar = ((VTLigacao*)eqpto)->esq.posbar2;
							break;
						case 2:
							posbar = ((VTLigacao*)eqpto)->esq.posbar3;
							break;
						}
						dx = IDINDEFINIDO;
						dy = IDINDEFINIDO;
					}

					// Formata a string para escrever no arquivo
					// strSQL = Format("%d;%d;%d;%f", ARRAYOFCONST((estudo->Id, eqpto->Id, barra->Id, posbar)));
					strSQL =
						Format("%d;%d;%d;%g", ARRAYOFCONST((estudo->Id, eqpto->Id, barra->Id,
						posbar)));
					(dx == IDINDEFINIDO) ? straux = ";" : straux =
						Format(";%d", ARRAYOFCONST((dx)));
					strSQL += straux;
					(dy == IDINDEFINIDO) ? straux = ";" : straux =
						Format(";%d", ARRAYOFCONST((dy)));
					strSQL += straux;

					lstStrData->Add(strSQL);
					// Loop barras (fim)
				}
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "EquipamentoBarraOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL +=
					"EquipamentoBarra ( intEstudoId, intEquipId, intBarraId, fltPosBar, intDx, intDy )";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrFiltro(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	AnsiString strlog;

	// Insere as cargas em modo batch
	if (!SPSNP_wrFiltroIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_FILTROINV return (false);
	}

	// Atualiza as cargas por meio de procedures
	if (!SPSNP_wrFiltroUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_FILTROINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrFiltroIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTFiltro* filtro;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoFILTRO, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}

			// -------------------------------
			// Inserção dos equipamentos (ini)
			// -------------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((filtro = (VTFiltro*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_FILTROINV return (false);
				}
				// Somente salva equipamentos novos
				if (!filtro->StatusNovo())
					continue;

				// Formata a string para escrever no arquivo
				// strSQL = Format("%d;%d;%f;%f;%f;%f",
				strSQL = Format("%d;%d;%g;%g;%g;%g",
					ARRAYOFCONST((estudo->Id, filtro->Id, filtro->vnom, filtro->snom, filtro->z0.r,
					filtro->z0.x)));
				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "FiltroOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "Filtro (";
				strSQL += "intEstudoId, intFiltroId, fltVnom, fltSnom, fltZ0R, fltZ0X";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			// -------------------------------
			// Inserção dos equipamentos (fim)
			// -------------------------------
			// Associa as barras aos equipamentos em modo batch
			if (!SPSNP_wrEquipamentoBarraIns(estudo, lisObjRede))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_FILTROINV return (false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrFiltroUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTFiltro* filtro;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}
	if (!bd_curva)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDCURVAINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				filtro = (VTFiltro*)lisObjRede->Items[n];
				if (!filtro)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_FILTROINV return (false);
				}
				if ((!filtro->StatusAlterado()) && (!filtro->StatusDeslocado()))
					continue;

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, filtro, filtro->Tipo(), IDINDEFINIDO, bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
				// Monta a string de execução
				strSQL =
					"EXEC SPSNP_wrFiltro_p02 :p_Vnom, :p_Snom, :p_Z0R, :p_Z0X, :p_EstudoId, :p_FiltroId ";

				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_Vnom", PARAMIN_FLT(filtro->vnom)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Snom", PARAMIN_FLT(filtro->snom)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z0R", PARAMIN_FLT(filtro->z0.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z0X", PARAMIN_FLT(filtro->z0.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_FiltroId", PARAMIN_INT(filtro->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Atualiza posicionamento na barra
				if (!SPSNP_wrEquipamentoBarraUpd(estudo, filtro))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrEstudoEqpto(VTEstudo* estudo)
{
	// variáveis locais
	AnsiString strlog;
	TList* lisEQP = NULL;

	__try
	{
		try
		{
			// Reinicia o indexador de equipamentos
			FNSNP_rdEquipIdProx(estudo, false, true);

			// Lista auxiliar para carga de equipamentos
			lisEQP = new TList();

			if (cronometro)
				cronometro->Reinicia();
			if (cronometro)
				cronometro->Start("SalvaRedeEstudo");

			if (!estudo)
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_ESTUDOINV return (false);
			}

			// Salva dados das barras
			// Uma mesma barra pode estar em duas redes e será salva para todas as redes
//			if (progresso)
//				progresso->Add("Salvando barras");

			// Salva dados de Barras
			if (progresso)
				progresso->Add("Salvando barras");
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoBARRA);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrBarra(estudo, lisEQP, true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "barras") return (false);
				}
			}
			// Salva dados de Redes
			if (progresso)
				progresso->Add("Salvando redes");
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoREDE);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrRede(estudo, lisEQP, (estudo->Id > 0)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_REDES return (false);
				}
			}
			// Salva dados das cargas
			if (progresso)
				progresso->Add("Salvando cargas");
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoCARGA);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrCarga(estudo, lisEQP, true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "cargas") return (false);
				}
			}
			// Salva dados das cargas  IP
			if (progresso)
				progresso->Add("Salvando cargas (iluminação pública)");
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoCARGA);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrCargaIP(estudo, lisEQP, true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "cargas iluminação pública") return (false);
				}
			}

			// Salva dados dos capacitores
			if (progresso)
				progresso->Add("Salvando capacitores");
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoCAPACITOR);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrCapacitor(estudo, lisEQP, true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "capacitores") return (false);
				}
			}

			// Salva dados dos geradores
			if (progresso)
				progresso->Add("Salvando geradores");
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoGERADOR);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrGerador(estudo, lisEQP, true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "geradores") return (false);
				}
			}

			// Salva dados dos suprimentos
			if (progresso)
				progresso->Add("Salvando suprimentos");
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoSUPRIMENTO);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrSuprimento(estudo, lisEQP, true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "suprimentos") return (false);
				}
			}

			// Salva dados dos filtros
			if (progresso)
				progresso->Add("Salvando filtros");
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoFILTRO);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrFiltro(estudo, lisEQP, true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "filtros") return (false);
				}
			}

			// Salva dados dos reatores
			if (progresso)
				progresso->Add("Salvando reatores");
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoREATOR);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrReator(estudo, lisEQP, true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "reatores") return (false);
				}
			}

			// Salva dados dos trafos ZZ
			if (progresso)
				progresso->Add("Salvando transformadores Zig-Zag");
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoTRAFOZZ);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrTrafoZZ(estudo, lisEQP, true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "trafos Zig Zag") return (false);
				}
			}
			// Salva dados dos trechos
			if (progresso)
				progresso->Add("Salvando trechos");
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoTRECHO);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrTrecho(estudo, lisEQP, true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "trechos") return (false);
				}
			}

			// Salva dados dos capacitores série
			if (progresso)
				progresso->Add("Salvando capacitores série");
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoCAPSERIE);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrCapacitorSerie(estudo, lisEQP, true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "capacitores série") return (false);
				}
			}

			// Salva dados das chaves
			if (progresso)
				progresso->Add("Salvando chaves");
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoCHAVE);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrChave(estudo, lisEQP, true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "chaves") return (false);
				}
			}

			// Salva dados dos reguladores
			if (progresso)
				progresso->Add("Salvando reguladores");
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoREGULADOR);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrRegulador(estudo, lisEQP, true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "reguladores") return (false);
				}
			}

			// Salva dados dos transformadores
			if (progresso)
				progresso->Add("Salvando transformadores");
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoTRAFO);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrTrafo(estudo, lisEQP, true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "trafos") return (false);
				}
			}
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoTRAFO3E);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrTrafo3E(estudo, lisEQP, true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "trafos 3E") return (false);
				}
			}
			// Salva dados das mútuas
			if (progresso)
				progresso->Add("Salvando impedâncias mútuas");
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoMUTUA);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrMutua(estudo, lisEQP, true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "mútuas") return (false);
				}
			}

			// Salva dados dos medidores
			if (progresso)
				progresso->Add("Salvando medidores");
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoMEDIDOR);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrMedidor(estudo, lisEQP, true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "medidores") return (false);
				}
			}

			// Salva dados dos canais
			if (progresso)
				progresso->Add("Salvando canais");
			lisEQP->Clear();
			estudo->LisEqptoEstudo(lisEQP, NULL, eqptoCANAL);
			if (lisEQP->Count > 0)
			{
				if (!SPSNP_wrCanal(estudo, lisEQP, true))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_WREQUIPREDE(strlog, "canais") return (false);
				}
			}

			if (cronometro)
				cronometro->Stop("SalvaRedeEstudo");
			if (cronometro)
				cronometro->Show();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			return (false);
		}
	}
	__finally
	{
		// if (progresso) progresso->Stop();

		// destrói lista sem destruir seus objetos
		if (lisEQP)
		{
			delete lisEQP;
			lisEQP = NULL;
		}
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrGerador(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	// variáveis locais
	AnsiString strlog;
	VTGerador *gerador;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

	// CÓDIGO TEMPORÁRIO: atualiza Extern_id com fases do Gerador
	for (int n = 0; n < lisObjRede->Count; n++)
	{
		if ((gerador = (VTGerador*)lisObjRede->Items[n]) == NULL)
		{
			SNPSTRLOGLINE(strlog);
			MSGERRSYS_GERADORINV return (false);
		}
		gerador->Extern_id = "Fases=" + fases->AsStrABCN[gerador->Fases];
	}
	// fim código temporário

	// Insere as cargas em modo batch
	if (!SPSNP_wrGeradorIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_GERADORINV return (false);
	}

	// Atualiza as cargas por meio de procedures
	if (!SPSNP_wrGeradorUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_GERADORINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrGeradorIns(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTGerador* gerador;
	VTCurva* curva;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Inicialmente, cria todas as curvas que não são típicas e atribui o identificador ao equipamento.
			// Assim, não há necessidade de fazer a atualização no final
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((gerador = (VTGerador*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_GERADORINV return (false);
				}
				// Somente salva equipamentos novos
				if (!gerador->StatusNovo())
					continue;

				if ((curva = gerador->curva) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BDCURVAINV return (false);
				}
				if (curva->NumeroPontos() > 0)
				{
					// Se a curva for típica e o status não for novo, não precisa salvar a curva
					if ((curva->Status[sttNOVO] == true) && (!curva->Tipica))
					{
						// Inicialmente, escreve a curva, cuja função já existe na base de dados geral
						// se o código estiver vazio, associa o código do equipamento à curva
						if (curva->Codigo.IsEmpty())
						{
							curva->Codigo = gerador->Codigo;
						}
						// grava Curva somente se for própria
						if (!bd_curva->SPSNP_wrConjCurva(gerador->Tipo(), curva))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRSYS_CURVAINV return (false);
						}
					}
				}
			}

			// Nesse ponto, todas as curvas próprias já foram salvas na base e o id gerado associado
			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoGERADOR, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}

			// -------------------------------
			// Inserção dos equipamentos (ini)
			// -------------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((gerador = (VTGerador*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_GERADORINV return (false);
				}
				// Somente salva equipamentos novos
				if (!gerador->StatusNovo())
					continue;

				// Formata a string para escrever no arquivo
				// strSQL = Format("%d;%d;%d;%d;%d;%f;%f;%f;%f;%f;%f;%f;%f",
				strSQL = Format("%d;%d;%d;%d;%d;%g;%g;%g;%g;%g;%g;%g;%g",
					ARRAYOFCONST((estudo->Id, gerador->Id, gerador->ligacao, gerador->tiposup,
					gerador->FonteEnergia, gerador->vnom, gerador->snom, gerador->smax,
					gerador->fplim, gerador->z0.r, gerador->z0.x, gerador->z1.r, gerador->z1.x)));

				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "GeradorOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "Gerador (";
				strSQL +=
					"intEstudoId, intGeradorId, intTipoLigacaoId, intTipoSuprId, intTipoGerId, fltVnom, fltSnom, fltSmax, fltFatorPotMin, fltZ0R, fltZ0X, fltZ1R, fltZ1X";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			// -------------------------------
			// Inserção dos equipamentos (fim)
			// -------------------------------

			// Associa as barras aos equipamentos em modo batch
			if (!SPSNP_wrEquipamentoBarraIns(estudo, lisObjRede))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_GERADORINV return (false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrGeradorUpd(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTGerador* gerador;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}
	if (!bd_curva)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDCURVAINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				gerador = (VTGerador*)lisObjRede->Items[n];
				if (!gerador)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_GERADORINV return (false);
				}
				if ((!gerador->StatusAlterado()) && (!gerador->StatusDeslocado()))
					continue;

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, gerador, gerador->Tipo(), IDINDEFINIDO,
					bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
				// Monta a string de execução
				// Objeto previamente existente. O identificador em memória é válido.
				strSQL =
					"EXEC SPSNP_wrGerador_p02 :p_TipoLigacaoId, :p_TipoSuprId, :p_TipoGerId, :p_Vnom, :p_Snom, :p_sMax, ";
				strSQL +=
					":p_FatorPotMin, :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X, :p_EstudoId, :p_GeradorId";

				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_TipoLigacaoId", PARAMIN_INT(gerador->ligacao)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_TipoSuprId", PARAMIN_INT(gerador->tiposup)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_TipoGerId", PARAMIN_INT(gerador->FonteEnergia)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Vnom", PARAMIN_FLT(gerador->vnom)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Snom", PARAMIN_FLT(gerador->snom)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_sMax", PARAMIN_FLT(gerador->smax)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_FatorPotMin", PARAMIN_FLT(gerador->fplim)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z0R", PARAMIN_FLT(gerador->z0.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z0X", PARAMIN_FLT(gerador->z0.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z1R", PARAMIN_FLT(gerador->z1.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z1X", PARAMIN_FLT(gerador->z1.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_GeradorId", PARAMIN_INT(gerador->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Escreve a curva do gerador
				if (gerador->curva->NumeroPontos() > 0)
				{
					if (!bd_curva->SPSNP_wrConjCurvaEquipamento(estudo, gerador, gerador->curva))
					{
						sucesso = false;
						SNPSTRLOGLINE(strlog);
					}
				}
				// Atualiza posicionamento na barra
				if (!SPSNP_wrEquipamentoBarraUpd(estudo, gerador))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrManobra(VTEstudo* estudo)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTManobra* manobra;
	VTManobras* manobras = (VTManobras*)apl->GetObject(__classid(VTManobras));
	TList* lisMANOBRAS;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!manobras)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_MANOBRASINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			// Atualiza o indexador das manobras
			FNSNP_rdManobraIdProx(false, true);

			if ((lisMANOBRAS = manobras->LisManobra()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_MANOBRASINV return (false);
			}
			for (int n = 0; n < lisMANOBRAS->Count; n++)
			{
				manobra = (VTManobra*)lisMANOBRAS->Items[n];
				if (!manobra)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MANOBRAINV return (false);
				}

				// Monta a string de execução
				// Verifica se o objeto é novo ou alterado
				if (manobra->Status[sttNOVO])
				{
					// Objeto novo.
					manobra->Id = FNSNP_rdManobraIdProx();
					strSQL = "EXEC SPSNP_wrManobra_p01 :p_Codigo, :p_ManobraId";
				}
				else if (manobra->Status[sttALTERADO])
				{
					// Objeto previamente existente. O identificador em memória é válido.
					if (!SPSNP_dlManobraChave(manobra))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_MANOBRAINV return (false);
					}
					strSQL = "EXEC SPSNP_wrManobra_p02 :p_Codigo, :p_ManobraId";
				}
				else
				{
					// Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
					continue;
				}

				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_Codigo", PARAMIN_STR(manobra->Codigo)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ManobraId", PARAMIN_INT(manobra->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Insere as chaves da manobra
				if (!SPSNP_wrManobraChave(estudo, manobra))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}

				// Atualiza o estado
				manobra->Status[sttNOVO] = false;
				manobra->Status[sttALTERADO] = false;
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrManobraChave(VTEstudo* estudo, VTManobra* manobra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	TList* lisLOCKED;
	VTLocked* locked;
	VTChave* chave;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!manobra)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_MANOBRAINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			// Monta a string de execução
			strSQL =
				"EXEC SPSNP_wrManobraChave_p01 :p_Fechada, :p_EstudoId, :p_ChaveId, :p_ManobraId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// Loop das chaves por manobra
			if ((lisLOCKED = manobra->LisLocked()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_MANOBRAINV return (false);
			}

			for (int i = 0; i < lisLOCKED->Count; i++)
			{
				// Determina os valores dos parâmetros
				if ((locked = (VTLocked*)lisLOCKED->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_SINCRONISMOINV return (false);
				}
				if ((chave = locked->chave) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MANOBRAINV return (false);
				}

				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ManobraId", PARAMIN_INT(manobra->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ChaveId", PARAMIN_INT(chave->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Fechada", PARAMIN_INT(locked->estado)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrMedidor(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	AnsiString strlog;

	// DVK 2016.10.07 linha comentada
	// redefine todos os Medidores com status sstNOVO porque eles serão apagados e regravados da base
	// RenovaStatus(lisObjRede);
	// Insere os medidores em modo batch
	if (!SPSNP_wrMedidorIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_MEDIDORINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrMedidorIns(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTMedidor* medidor;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Inicialmente, cria todas as curvas que não são típicas e atribui o identificador ao equipamento.
			// Assim, não há necessidade de fazer a atualização no final
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((medidor = (VTMedidor*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MEDIDORINV return (false);
				}

				// OOPS: Aqui poderia ser utilizada uma tabela temporária, ao
				// Deleta o medidor, antes de inserir novamente
				if (!SPSNP_dlMedidor(estudo, medidor))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
			}
			// DVK 2016.10.07 redefine todos os Medidores com status sstNOVO
			// porque eles serão apagados e regravados da base
			RenovaStatus(lisObjRede);

			// Nesse ponto, todas as curvas próprias já foram salvas na base e o id gerado associado
			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoMEDIDOR, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}

			// -------------------------------
			// Inserção dos equipamentos (ini)
			// -------------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((medidor = (VTMedidor*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BARRAINV return (false);
				}

				// Somente salva equipamentos novos
				if (!medidor->StatusNovo())
					continue;

				// Formata a string para escrever no arquivo
				strSQL = Format("%d;%d;", ARRAYOFCONST((estudo->Id, medidor->Id)));
				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "MedidorOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "Medidor (";
				strSQL += "intEstudoId, intMedidorId";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			// -------------------------------
			// Inserção dos equipamentos (fim)
			// -------------------------------

			// Associa as barras aos equipamentos em modo batch
			if (!SPSNP_wrEquipamentoBarraIns(estudo, lisObjRede))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_MEDIDORINV return (false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrMunicipio(void)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	AnsiString nome, regiao;
	TStringList *slNomes;
	int codigo;
	VTMunicipio *municipio = (VTMunicipio*)apl->GetObject(__classid(VTMunicipio));

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// apaga todos municipios da base
			strSQL = "DELETE * FROM Municipios";
			bd_conn->ParseCmd(strSQL);
			bd_conn->Execute(false);

			//insere municipios do apl
			slNomes = municipio->SLisNome();

			// -------------------------------
			// Inserção dos equipamentos (ini)
			// -------------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			for (int n = 0; n < slNomes->Count; n++)
			{
				nome = slNomes->Strings[n];
				codigo = municipio->Codigo(nome);
				regiao = municipio->Regiao(codigo);
				// Formata a string para escrever no arquivo
				strSQL = Format("%d,%d,'%s','%s'", ARRAYOFCONST((n+1, codigo, nome, regiao)));
				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			for (int n = 0; n < lstStrData->Count; n++)
			{
				strSQL = "INSERT INTO ";
				strSQL += "Municipios (";
				strSQL += "intMunicipioId, intExternId, strCodigo, strRegiao";
				strSQL += ") ";
				strSQL += "VALUES (" + lstStrData->Strings[n] + ")";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			// -------------------------------
			// Inserção dos equipamentos (fim)
			// -------------------------------
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrMutua(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	AnsiString strlog;

	// Insere as cargas em modo batch
	if (!SPSNP_wrMutuaIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_MUTUAINV return (false);
	}

	// Atualiza as cargas por meio de procedures
	if (!SPSNP_wrMutuaUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_MUTUAINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrMutuaIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTMutua* mutua;
	int intBarRefId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoMUTUA, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}

			// -------------------------------
			// Inserção dos equipamentos (ini)
			// -------------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((mutua = (VTMutua*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MUTUAINV return (false);
				}
				// Somente salva equipamentos novos
				if (!mutua->StatusNovo())
					continue;

				// Verificações adicionais
				if ((!mutua->ptre1) || (!mutua->ptre2))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MUTUATREINV return (false);
				}
				if ((!mutua->pbar_tre1) || (!mutua->pbar_tre2))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MUTUABARINV return (false);
				}

				// Formata a string para escrever no arquivo
				// strSQL = Format("%d;%d;%d;%d;%f;%d;%d;%f;%f;%f;%f;%f;",
				strSQL = Format("%d;%d;%d;%d;%g;%d;%d;%g;%g;%g;%g;%g;",
					ARRAYOFCONST((estudo->Id, mutua->Id, mutua->ptre1->Id, mutua->pbar_tre1->Id,
					mutua->postre1, mutua->ptre2->Id, mutua->pbar_tre2->Id, mutua->postre2,
					mutua->z0.r, mutua->z0.x, mutua->z1.r, mutua->z1.x)));
				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "MutuaOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "Mutua (";
				strSQL += "intEstudoId, intMutuaId, ";
				strSQL +=
					"intTrecho1Id, intTrecho1BarraSntId, fltTrecho1Pos, intTrecho2Id, intTrecho2BarraSntId, fltTrecho2Pos, ";
				strSQL += "fltZ0R, fltZ0X, fltZ1R, fltZ1X";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			/*
			 // -------------------------------
			 // Inserção dos equipamentos (fim)
			 // -------------------------------

			 // Atualiza posicionamento na barra
			 if (!SPSNP_wrEquipamentoBarraIns(estudo, lisObjRede))
			 {
			 sucesso = false;
			 SNPSTRLOGLINE(strlog);
			 }
			 */
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrMutuaUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTMutua* mutua;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				mutua = (VTMutua*)lisObjRede->Items[n];
				if (!mutua)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MUTUAINV return (false);
				}
				if ((!mutua->StatusAlterado()) && (!mutua->StatusDeslocado()))
					continue;

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, mutua, mutua->Tipo(), mutua->Rede()->Id,
					bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
				// Monta a string de execução
				strSQL =
					"EXEC SPSNP_wrMutua_p02 :p_Trecho1Id, :p_Trecho1BarraSntId, :p_Trecho1Pos, :p_Trecho2Id, :p_Trecho2BarraSntId, :p_Trecho2Pos, :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X, :p_EstudoId, :p_MutuaId";

				// Verificações adicionais
				if ((!mutua->ptre1) || (!mutua->ptre2))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MUTUATREINV return (false);
				}
				if ((!mutua->pbar_tre1) || (!mutua->pbar_tre2))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MUTUABARINV return (false);
				}

				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_Trecho1Id", PARAMIN_INT(mutua->ptre1->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Trecho1BarraSntId", PARAMIN_INT(mutua->pbar_tre1->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Trecho1Pos", PARAMIN_INT(mutua->postre1)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Trecho2Id", PARAMIN_INT(mutua->ptre2->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Trecho2BarraSntId", PARAMIN_INT(mutua->pbar_tre2->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Trecho2Pos", PARAMIN_INT(mutua->postre2)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z0R", PARAMIN_FLT(mutua->z0.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z0X", PARAMIN_FLT(mutua->z0.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z1R", PARAMIN_FLT(mutua->z1.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z1X", PARAMIN_FLT(mutua->z1.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_MutuaId", PARAMIN_INT(mutua->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Atualiza posicionamento na barra
				if (!SPSNP_wrEquipamentoBarraUpd(estudo, mutua))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrEstudoAlternativa(VTEstudo *estudo)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	TList* lisAlternativa;
	VTAlternativa* alternativa;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if ((lisAlternativa = estudo->LisAlternativa()) == NULL)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISOBJINV return (false);
	}

	// Se não tem alternativas, já retorna
	if (lisAlternativa->Count == 0)
		return (true);

	// Escrita propriamente dita
	__try
	{
		try
		{
			// Reinicia o indexador de alternativas
			FNSNP_rdAlternativaIdProx(estudo, false, true);

			// Monta a string de execução
			strSQL =
				"EXEC SPSNP_wrEstudoAlternativa :p_EstudoId, :p_AlternativaId, :p_Descricao, :p_Justificativa";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// Determina os elementos para execução dos comandos
			for (int i = 0; i < lisAlternativa->Count; i++)
			{
				// loop Alternativas (ini)
				if ((alternativa = (VTAlternativa*)lisAlternativa->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_ALTERNATIVA return (false);
				}
				if (alternativa == estudo->AlternativaPadrao)
					continue;

				// Nota: o estudo é apagado antes de salvar, então pode renumerar as alternativas
				alternativa->Id = FNSNP_rdAlternativaIdProx(estudo);

				/*
				 Agora a alternativa é lida da memória, após a primeira vez.
				 Não há necessidade de reconstruir a string toda vez que executa

				 // Monta a string de execução
				 strSQL = "EXEC SPSNP_wrEstudoAlternativa :p_EstudoId, :p_AlternativaId, :p_Descricao";
				 // Decodifica comando
				 bd_conn->ParseCmd(strSQL);
				 */
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_AlternativaId", PARAMIN_INT(alternativa->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Descricao", PARAMIN_STR(alternativa->Codigo)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Justificativa", PARAMIN_STR(alternativa->Justificativa)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}
				// loop Alternativas (fim)
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TBDeqptoAccess::SPSNP_wrEstudoCorteZero(VTEstudo* estudo, TList* lisMRede)
 {
 bool       sucesso = true;
 AnsiString strSQL;
 AnsiString strlog;
 VTCorte*   corte;
 VTMRede*   mrede    = NULL;
 TList*     lisCorte = NULL;
 TList*     lisCORTE_EXT;


 if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON  return(false); }
 if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON  return(false); }
 if (!estudo)                      { SNPSTRLOGLINE(strlog); MSGERRSYS_ESTUDOINV   return(false); }
 if (!lisMRede)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_LISMREDEINV return(false); }
 if (!lstStrData)                  { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV     return(false); }

 // Escrita propriamente dita
 __try
 {
 // Lista auxiliar para Cortes
 lisCorte = new TList();
 try
 {
 // Reinicia o indexador de equipamentos
 FNSNP_rdCorteIdProx(false, true);

 // Inicia o loop de cortes
 // Limpa a lista de dados a serem salvos em arquivo
 lstStrData->Clear();
 for (int n = 0; n < lisMRede->Count; n++)
 {
 // loop cortes (ini)
 if ((mrede = (VTMRede*)lisMRede->Items[n]) == NULL)
 {
 SNPSTRLOGLINE(strlog);
 MSGERRSYS_MREDEINV
 return(false);
 }
 //não salva Cortes das MRedes não carregadas
 if ((! mrede->Carregada)||(! mrede->Status[sttENABLED])) continue;
 //insere em lisCorte os Cortes da MRede que conectam apenas MRedes carregadas
 lisCORTE_EXT = mrede->LisCorte();
 for (int j = 0; j < lisCORTE_EXT->Count; j++)
 {
 corte = (VTCorte*)lisCORTE_EXT->Items[j];
 //verifica se o Corte conecta alguma rede não carregada
 if (corte->NumeroMRedes() == corte->NumeroMRedesCarregadas())
 {
 if (lisCorte->IndexOf(corte) < 0) lisCorte->Add(corte);
 }
 }
 }
 //redefine ID dos Cortes em lisCorte
 for (int j = 0; j < lisCorte->Count; j++)
 {
 if ((corte = (VTCorte*)lisCorte->Items[j]) == NULL)
 {
 SNPSTRLOGLINE(strlog);
 MSGERRSYS_CORTEINV
 return(false);
 }
 //if (corte->Id < 0)
 //   {
 //   corte->Id = FNSNP_rdCorteIdProx();
 //   }
 corte->Id = FNSNP_rdCorteIdProx();
 }
 //loop p/ todas MRedes em lisMRede
 for (int n = 0; n < lisMRede->Count; n++)
 {
 mrede = (VTMRede*)lisMRede->Items[n];
 if (! mrede->Carregada) continue;
 //obtém lista de Cortes de MRede
 lisCorte->Clear();
 mrede->LisCorte(lisCorte);
 //loop p/ todos Cortes da MRede
 for (int j = 0; j < lisCorte->Count; j++)
 {
 corte = (VTCorte*)lisCorte->Items[j];

 // Formata a string para escrever no arquivo
 strSQL = Format("%d;%d;%d",
 ARRAYOFCONST((
 estudo->Id, mrede->Id, corte->Id
 ))
 );
 lstStrData->Add(strSQL);
 }
 }
 // loop cortes (fim)

 // Executa a lista de comandos de equipamentos
 if (lstStrData->Count > 0)
 {
 AnsiString strFileName = "CorteOut.txt";
 AnsiString strFilePath = path->DirTmp() + "\\";
 lstStrData->SaveToFile(strFilePath + strFileName);

 strSQL  = "INSERT INTO ";
 strSQL += "Corte (";
 strSQL +=        "intEstudoId, intMRedeId, intCorteId";
 strSQL +=        ") ";
 strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath + "' [Text;HDR=NO;]";
 bd_conn->ParseCmd(strSQL);
 bd_conn->Execute(false);
 }
 }
 catch (Exception& e)
 {
 SNPSTRLOGLINE(strlog);
 SNPSTRLOG(e.Message);
 sucesso = false;
 }
 }
 __finally
 {
 // Colocado dentro do finally para garantir que será deletado, mesmo que
 // haja um return no meio do código
 if (lisCorte) {delete lisCorte;}
 }

 return(sucesso);
 }
 */
// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/15692594
bool __fastcall TBDeqptoAccess::SPSNP_wrEstudoCorteZero(VTEstudo* estudo, TList* lisMRede)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTCorte* corte;
	VTMRede* mrede = NULL;
	TList* lisCorte = NULL;
	TList* lisCORTE_EXT = NULL;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisMRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISMREDEINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	// Escrita propriamente dita
	__try
	{ // Lista auxiliar para Cortes
		lisCorte = new TList();
		try
		{
			// Reinicia o indexador de equipamentos
			FNSNP_rdCorteIdProx(false, true);

			// Inicia o loop de cortes
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			for (int n = 0; n < lisMRede->Count; n++)
			{
				// loop cortes (ini)
				if ((mrede = (VTMRede*)lisMRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MREDEINV return (false);
				}
				// grava os Cortes apenas das MRede que são novas
				if (!mrede->Status[sttNOVO])
					continue;
				// identifica os Cortes que devem ser gravados
				lisCORTE_EXT = mrede->LisCorte();
				for (int j = 0; j < lisCORTE_EXT->Count; j++)
				{
					corte = (VTCorte*)lisCORTE_EXT->Items[j];
					// verifica se o Corte já existe em lisCorte
					if (lisCorte->IndexOf(corte) < 0)
					{ // insere Corte em lisCorte
						lisCorte->Add(corte);
						// verifica se o Corte é novo ou ID negativo
						if (corte->Status[sttNOVO] || (corte->Id < 0))
						{ // redfine ID do Corte
							corte->Id = FNSNP_rdCorteIdProx();
						}
					}
					// Formata a string para escrever no arquivo
					strSQL = Format("%d;%d;%d", ARRAYOFCONST((estudo->Id, mrede->Id, corte->Id)));
					lstStrData->Add(strSQL);
				}
			}
			// loop cortes (fim)

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "CorteOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "Corte (";
				strSQL += "intEstudoId, intMRedeId, intCorteId";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
		if (lisCorte)
		{
			delete lisCorte;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrEstudoCorteNaoZero(VTEstudo* estudo, TList* lisMRede)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTCorte* corte;
	VTMRede* mrede = NULL;
	TList* lisCorte = NULL;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisMRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISMREDEINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		// Lista auxiliar para Cortes
		lisCorte = new TList();
		try
		{
			// Reinicia o indexador de equipamentos
			FNSNP_rdCorteIdProx(false, true);

			// Inicia o loop de cortes
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			for (int n = 0; n < lisMRede->Count; n++)
			{
				// loop cortes (ini)
				if ((mrede = (VTMRede*)lisMRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MREDEINV return (false);
				}
				// insere Cortes da MRede em lisCorte
				mrede->LisCorte(lisCorte);
			}
			// redefine ID dos Cortes em lisCorte
			for (int j = 0; j < lisCorte->Count; j++)
			{
				corte = (VTCorte*)lisCorte->Items[j];
				corte->Id = FNSNP_rdCorteIdProx();
			}
			// loop p/ todas MRedes em lisMRede
			for (int n = 0; n < lisMRede->Count; n++)
			{
				mrede = (VTMRede*)lisMRede->Items[n];
				// obtém lista de Cortes de MRede
				lisCorte->Clear();
				mrede->LisCorte(lisCorte);
				// loop p/ todos Cortes da MRede
				for (int j = 0; j < lisCorte->Count; j++)
				{
					corte = (VTCorte*)lisCorte->Items[j];

					// Formata a string para escrever no arquivo
					strSQL = Format("%d;%d;%d", ARRAYOFCONST((estudo->Id, mrede->Id, corte->Id)));
					lstStrData->Add(strSQL);
				}
			}
			// loop cortes (fim)

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "CorteOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "Corte (";
				strSQL += "intEstudoId, intMRedeId, intCorteId";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
		if (lisCorte)
		{
			delete lisCorte;
		}
	}

	return (sucesso);
}

//// ---------------------------------------------------------------------------
//bool __fastcall TBDeqptoAccess::SPSNP_wrObraAtribExt(VTEstudo *estudo)
//{ // DVK 2018.02.20 alterado para incluir a descricao
//	bool sucesso = true;
//	AnsiString strSQL;
//	AnsiString strlog;
//	TList* lisAlternativa = NULL;
//	VTAlternativa* alternativa = NULL;
//	VTObras* obras = NULL;
//	TList* lisObras = NULL;
//	VTObra* obra = NULL;
//	VTIpeAtributo* ipe_atributo;
//	TList* lisATRIB;
//	TMemoryStream* ptrStream = NULL;
//	TParameter* ptrParam;
//	TStringList* slFields;
//	TADOQuery* ptrQryDS = NULL;
//
//	if (!bd_conn)
//	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON return (false);
//	}
//	if (!bd_conn->getConnectedOK())
//	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON return (false);
//	}
//	if (!estudo)
//	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_ESTUDOINV return (false);
//	}
//	if ((lisAlternativa = estudo->LisAlternativa()) == NULL)
//	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_LISOBJINV return (false);
//	}
//
//	// Se não tem alternativas, já retorna
//	if (lisAlternativa->Count == 0)
//		return (true);
//
//	// Escrita propriamente dita
//	__try
//	{
//		try
//		{ /*
//			 // Monta a string temporaria de execução para validação
//			 strSQL = "EXEC SPSNP_rdEstudoObraAtribExt :p_EstudoId";
//			 // Decodifica comando
//			 bd_conn->ParseCmd(strSQL);
//			 // Determina os valores dos parâmetros
//			 if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
//			 // Comando da consulta
//			 if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(false); }
//			 if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(false); }
//
//			 // Cria stringList
//			 slFields = new TStringList();
//			 //valida campos
//			 ptrQryDS->GetFieldNames(slFields);
//			 if(slFields->IndexOf("Descr") < 0)
//			 {
//			 InsereCampo("ObraAtribExt", "Descr", "TEXT(255)");
//			 }
//			 delete slFields;
//			 */
//
//			// Determina os elementos para execução dos comandos
//			for (int i = 0; i < lisAlternativa->Count; i++)
//			{
//				// loop Alternativas (ini)
//				if ((alternativa = (VTAlternativa*)lisAlternativa->Items[i]) == NULL)
//				{
//					SNPSTRLOGLINE(strlog);
//					MSGERRSYS_ALTERNATIVA return (false);
//				}
//				if (alternativa == estudo->AlternativaPadrao)
//					continue;
//
//				if ((obras = alternativa->Obras) == NULL)
//				{
//					SNPSTRLOGLINE(strlog);
//					MSGERRSYS_OBRAS return (false);
//				}
//
//				if ((lisObras = obras->LisObra()) == NULL)
//				{
//					SNPSTRLOGLINE(strlog);
//					MSGERRSYS_LISOBJINV return (false);
//				}
//
//				for (int j = 0; j < lisObras->Count; j++)
//				{
//					// loop obras (ini)
//					if ((obra = (VTObra*)lisObras->Items[j]) == NULL)
//					{
//						SNPSTRLOGLINE(strlog);
//						MSGERRSYS_OBRAS return (false);
//					}
//					// obtém lista de IpeAtributos da Obra
//					lisATRIB = obra->LisObject;
//					// Monta a string de execução
//					strSQL =
//						"EXEC SPSNP_wrObraAtribExt :p_EstudoId, :p_AlternativaId, :p_ObraId, :p_ExtId, :p_AtribId, :p_Descricao, :p_ItemId";
//					// Decodifica comando
//					bd_conn->ParseCmd(strSQL);
//					// loop para todos IpeAtributos em lisATRIB
//					for (int n = 0; n < lisATRIB->Count; n++)
//					{
//						ipe_atributo = (VTIpeAtributo*)lisATRIB->Items[n];
//						// proteção
//						if (ipe_atributo == NULL)
//							continue;
//						// Seta os valores dos parâmetros
//						if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
//						{
//							SNPSTRLOGLINE(strlog);
//							MSGERRBD_PARAMINV return (false);
//						}
//						if (!bd_conn->SetParam("p_AlternativaId", PARAMIN_INT(alternativa->Id)))
//						{
//							SNPSTRLOGLINE(strlog);
//							MSGERRBD_PARAMINV return (false);
//						}
//						if (!bd_conn->SetParam("p_ObraId", PARAMIN_INT(obra->Id)))
//						{
//							SNPSTRLOGLINE(strlog);
//							MSGERRBD_PARAMINV return (false);
//						}
//						if (!bd_conn->SetParam("p_ExtId", PARAMIN_INT(ipe_atributo->Tipo)))
//						{
//							SNPSTRLOGLINE(strlog);
//							MSGERRBD_PARAMINV return (false);
//						}
//						if (!bd_conn->SetParam("p_AtribId", PARAMIN_INT(ipe_atributo->Id)))
//						{
//							SNPSTRLOGLINE(strlog);
//							MSGERRBD_PARAMINV return (false);
//						}
//						if (!bd_conn->SetParam("p_Descricao", PARAMIN_STR(ipe_atributo->Descricao)))
//						{
//							SNPSTRLOGLINE(strlog);
//							MSGERRBD_PARAMINV return (false);
//						}
//						if (!bd_conn->SetParam("p_ItemId", PARAMIN_INT(obra->LisItemObra->Count + 1)))   //alterar
//						{
//							SNPSTRLOGLINE(strlog);
//							MSGERRBD_PARAMINV return (false);
//						}
//
//						/*
//						 // Cria descricao
//						 ptrStream = new TMemoryStream();
//						 // Monta a curva em memória
//						 if (!bd_func->MontaStreamDescricao(ptrStream, ipe_atributo->Descricao))
//						 {
//						 SNPSTRLOGLINE(strlog);
//						 return(false);
//						 }
//						 // Acerta os valores dos blobs
//						 ptrParam = bd_conn->SetParam("p_Descricao", PARAMIN_BLB(NULL));
//						 ptrParam->LoadFromStream(ptrStream, ptrParam->DataType);
//
//						 //              if (!bd_conn->SetParam("p_Descricao"    , PARAMIN_STR(ipe_atributo->Descricao)))    { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
//						 // loop acoes (fim)
//						 */
//						// Comando da consulta
//						if (!bd_conn->Execute(false))
//						{
//							SNPSTRLOGLINE(strlog);
//							return (false);
//						}
//					}
//
//					// loop obras (fim)
//				}
//				// loop Alternativas (fim)
//			}
//		}
//		catch (Exception& e)
//		{
//			SNPSTRLOGLINE(strlog);
//			SNPSTRLOG(e.Message);
//			sucesso = false;
//		}
//	}
//	__finally
//	{
//		// Colocado dentro do finally para garantir que será deletado, mesmo que
//		// haja um return no meio do código
//		if (ptrStream)
//		{
//			delete ptrStream;
//			ptrStream = NULL;
//		}
//	}
//
//	return (sucesso);
//}// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrObraAtribExt(VTEstudo *estudo)
{ // DVK 2018.02.20 alterado para incluir a descricao
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	TList* lisAlternativa = NULL;
	VTAlternativa* alternativa = NULL;
	VTObras* obras = NULL;
	TList* lisObras = NULL;
	VTObra* obra = NULL;
	TList* lisItemObra = NULL;
	VTItemObra  *itemObra = NULL;
	VTIpeAtributo* ipe_atributo;
	TList* lisATRIB;
	TMemoryStream* ptrStream = NULL;
	TParameter* ptrParam;
	TStringList* slFields;
	TADOQuery* ptrQryDS = NULL;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if ((lisAlternativa = estudo->LisAlternativa()) == NULL)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISOBJINV return (false);
	}

	// Se não tem alternativas, já retorna
	if (lisAlternativa->Count == 0)
		return (true);

	// Escrita propriamente dita
	__try
	{
		try
		{
			// Determina os elementos para execução dos comandos
			for (int i = 0; i < lisAlternativa->Count; i++)
			{
				// loop Alternativas (ini)
				if ((alternativa = (VTAlternativa*)lisAlternativa->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_ALTERNATIVA return (false);
				}
				if (alternativa == estudo->AlternativaPadrao)
					continue;

				if ((obras = alternativa->Obras) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_OBRAS return (false);
				}

				if ((lisObras = obras->LisObra()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_LISOBJINV return (false);
				}

				for (int j = 0; j < lisObras->Count; j++)
				{
					// loop obras (ini)
					if ((obra = (VTObra*)lisObras->Items[j]) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_OBRAS return (false);
					}
					// obtém lista de IpeAtributos da Obra
					lisATRIB = obra->LisObject;
					// Monta a string de execução
					strSQL =
						"EXEC SPSNP_wrObraAtribExt :p_EstudoId, :p_AlternativaId, :p_ObraId, :p_ExtId, :p_AtribId, :p_Descricao, :p_ItemId";
					// Decodifica comando
					bd_conn->ParseCmd(strSQL);
					// loop para todos IpeAtributos em lisATRIB
					for (int n = 0; n < lisATRIB->Count; n++)
					{
						ipe_atributo = (VTIpeAtributo*)lisATRIB->Items[n];
						// proteção
						if (ipe_atributo == NULL)
							continue;
						// Seta os valores dos parâmetros
						if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_AlternativaId", PARAMIN_INT(alternativa->Id)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_ObraId", PARAMIN_INT(obra->Id)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_ExtId", PARAMIN_INT(ipe_atributo->Tipo)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_AtribId", PARAMIN_INT(ipe_atributo->Id)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_Descricao", PARAMIN_STR(ipe_atributo->Descricao)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_ItemId", PARAMIN_INT(0)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						// Comando da consulta
						if (!bd_conn->Execute(false))
						{
							SNPSTRLOGLINE(strlog);
							return (false);
						}
					}
					// loop obras (fim)
					//faz o mesmo para os itemObra
                    if ((lisItemObra = obra->LisItemObra) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_ITEMSOBRA return (false);
					}
					//loop items obra
					for (int io = 0; io < lisItemObra->Count; io++)
					{
						if ((itemObra = (VTItemObra*)lisItemObra->Items[io]) == NULL)
						{
							SNPSTRLOGLINE(strlog);
							MSGERRSYS_ITEMOBRA return (false);
						}
						// obtém lista de IpeAtributos da Obra
						lisATRIB = itemObra->LisIpeAtributo;
						// Monta a string de execução
						strSQL =
							"EXEC SPSNP_wrObraAtribExt :p_EstudoId, :p_AlternativaId, :p_ObraId, :p_ExtId, :p_AtribId, :p_Descricao, :p_ItemId";
						// Decodifica comando
						bd_conn->ParseCmd(strSQL);
						// loop para todos IpeAtributos em lisATRIB
						for (int n = 0; n < lisATRIB->Count; n++)
						{
							ipe_atributo = (VTIpeAtributo*)lisATRIB->Items[n];
							// proteção
							if (ipe_atributo == NULL)
								continue;
							// Seta os valores dos parâmetros
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_AlternativaId", PARAMIN_INT(alternativa->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_ObraId", PARAMIN_INT(obra->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_ExtId", PARAMIN_INT(ipe_atributo->Tipo)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_AtribId", PARAMIN_INT(ipe_atributo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_Descricao", PARAMIN_STR(ipe_atributo->Descricao)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_ItemId", PARAMIN_INT(itemObra->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Comando da consulta
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}
					}
				}
				// loop Alternativas (fim)
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
		if (ptrStream)
		{
			delete ptrStream;
			ptrStream = NULL;
		}
	}

	return (sucesso);
}

//// ---------------------------------------------------------------------------
//bool __fastcall TBDeqptoAccess::SPSNP_wrObraAcao(VTEstudo *estudo)
//{
//	bool sucesso = true;
//	AnsiString strSQL;
//	AnsiString strlog;
//	TList* lisAlternativa = NULL;
//	VTAlternativa* alternativa = NULL;
//	VTObras* obras = NULL;
//	TList* lisObras = NULL;
//	VTObra* obra = NULL;
//	TList* lisAcao = NULL;
//	VTAcao* acao = NULL;
//	VTEqpto* eqpto = NULL;
//	VTEqpto* eqpto_estadopre = NULL;
//	VTEqpto* eqpto_estadopos = NULL;
//	int intEquipId;
//	int intEquipEstadoPreId;
//	int intEquipEstadoPosId;
//
//	if (!bd_conn)
//	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON return (false);
//	}
//	if (!bd_conn->getConnectedOK())
//	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON return (false);
//	}
//	if (!estudo)
//	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_ESTUDOINV return (false);
//	}
//	if ((lisAlternativa = estudo->LisAlternativa()) == NULL)
//	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_LISOBJINV return (false);
//	}
//	if (!redes)
//	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_REDESINV return (false);
//	}
//	if (redes->LisRede()->Count < 1)
//	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_REDESINV return (false);
//	}
//
//	// Se não tem alternativas, já retorna
//	if (lisAlternativa->Count == 0)
//		return (true);
//
//	// Escrita propriamente dita
//	__try
//	{
//		try
//		{
//			// Determina os elementos para execução dos comandos
//			for (int i = 0; i < lisAlternativa->Count; i++)
//			{
//				// loop Alternativas (ini)
//				if ((alternativa = (VTAlternativa*)lisAlternativa->Items[i]) == NULL)
//				{
//					SNPSTRLOGLINE(strlog);
//					MSGERRSYS_ALTERNATIVA return (false);
//				}
//				if (alternativa == estudo->AlternativaPadrao)
//					continue;
//
//				if ((obras = alternativa->Obras) == NULL)
//				{
//					SNPSTRLOGLINE(strlog);
//					MSGERRSYS_OBRAS return (false);
//				}
//
//				if ((lisObras = obras->LisObra()) == NULL)
//				{
//					SNPSTRLOGLINE(strlog);
//					MSGERRSYS_LISOBJINV return (false);
//				}
//
//				for (int j = 0; j < lisObras->Count; j++)
//				{
//					// loop obras (ini)
//					if ((obra = (VTObra*)lisObras->Items[j]) == NULL)
//					{
//						SNPSTRLOGLINE(strlog);
//						MSGERRSYS_OBRAS return (false);
//					}
//
//					if ((lisAcao = obra->LisAcao) == NULL)
//					{
//						SNPSTRLOGLINE(strlog);
//						MSGERRSYS_LISOBJINV return (false);
//					}
//
//					// Reinicia o indexador de ações
//					FNSNP_rdAcaoIdProx(estudo, alternativa, obra, false, true);
//
//					// Reinicia o indexador de itens
//					FNSNP_rdItemIdProx(estudo, alternativa, obra, false, true);
//
//					// Monta a string de execução
//					strSQL =
//						"EXEC SPSNP_wrObraAcao :p_EstudoId, :p_AlternativaId, :p_ObraId, :p_AcaoId, :p_ItemId, :p_TipoAcaoId, :p_EquipOriginalId, :p_EquipEstadoPreId, :p_EquipEstadoPosId";
//					// Decodifica comando
//					bd_conn->ParseCmd(strSQL);
//
//					// As ações são armazenadas na ordem inversa
//					for (int k = lisAcao->Count - 1; k >= 0; k--)
//					{
//						// loop acoes (ini)
//						if ((acao = (VTAcao*)lisAcao->Items[k]) == NULL)
//						{
//							SNPSTRLOGLINE(strlog);
//							MSGERRSYS_ACAO return (false);
//						}
//						// define Id da Acao
//						acao->Id = FNSNP_rdAcaoIdProx(estudo, alternativa, obra);
//						// define Id do Item
//						//acao->Id = FNSNP_rdItemIdProx(estudo, alternativa, obra);  //alterar para item
//						// verifica o tipo de Acao
//						switch (acao->Tipo())
//						{
//						case acaoALTERA:
//							eqpto = acao->Eqpto();
//							eqpto_estadopre = acao->CopiaDoEqptoOriginal();
//							eqpto_estadopos = acao->CopiaDoEqptoAlterado();
//							break;
//						case acaoINSERE:
//							eqpto = acao->Eqpto();
//							eqpto_estadopre = NULL;
//							eqpto_estadopos = NULL;
//							break;
//						case acaoRETIRA:
//							eqpto = acao->Eqpto();
//							eqpto_estadopre = NULL;
//							eqpto_estadopos = NULL;
//							break;
//						default:
//							continue;
//						}
//						// Compatibiliza os valores
//						eqpto ? intEquipId = eqpto->Id : intEquipId = IDINDEFINIDO;
//						eqpto_estadopre ? intEquipEstadoPreId = eqpto_estadopre->Id :
//							intEquipEstadoPreId = IDINDEFINIDO;
//						eqpto_estadopos ? intEquipEstadoPosId = eqpto_estadopos->Id :
//							intEquipEstadoPosId = IDINDEFINIDO;
//						/*
//						 Pode ser feito fora do loop, pois a iniciação com acesso à base de dados é feita
//						 somente uma vez.
//
//						 // Monta a string de execução
//						 strSQL = "EXEC SPSNP_wrObraAcao :p_EstudoId, :p_AlternativaId, :p_ObraId, :p_AcaoId, :p_TipoAcaoId, :p_EquipOriginalIdAux, :p_EquipAlteradoIdAux";
//						 // Decodifica comando
//						 bd_conn->ParseCmd(strSQL);
//						 */
//						// Seta os valores dos parâmetros
//						if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
//						{
//							SNPSTRLOGLINE(strlog);
//							MSGERRBD_PARAMINV return (false);
//						}
//						if (!bd_conn->SetParam("p_AlternativaId", PARAMIN_INT(alternativa->Id)))
//						{
//							SNPSTRLOGLINE(strlog);
//							MSGERRBD_PARAMINV return (false);
//						}
//						if (!bd_conn->SetParam("p_ObraId", PARAMIN_INT(obra->Id)))
//						{
//							SNPSTRLOGLINE(strlog);
//							MSGERRBD_PARAMINV return (false);
//						}
//						if (!bd_conn->SetParam("p_AcaoId", PARAMIN_INT(acao->Id)))
//						{
//							SNPSTRLOGLINE(strlog);
//							MSGERRBD_PARAMINV return (false);
//						}
//						if (!bd_conn->SetParam("p_ItemId", PARAMIN_INT(obra->LisItemObra->Count + 1)))   //alterar
//						{
//							SNPSTRLOGLINE(strlog);
//							MSGERRBD_PARAMINV return (false);
//						}
//						if (!bd_conn->SetParam("p_TipoAcaoId", PARAMIN_INT(acao->Tipo())))
//						{
//							SNPSTRLOGLINE(strlog);
//							MSGERRBD_PARAMINV return (false);
//						}
//						if (!bd_conn->SetParam("p_EquipOriginalId", PARAMIN_INT(intEquipId)))
//						{
//							SNPSTRLOGLINE(strlog);
//							MSGERRBD_PARAMINV return (false);
//						}
//						if (!bd_conn->SetParam("p_EquipEstadoPreId",
//							PARAMIN_INT(intEquipEstadoPreId)))
//						{
//							SNPSTRLOGLINE(strlog);
//							MSGERRBD_PARAMINV return (false);
//						}
//						if (!bd_conn->SetParam("p_EquipEstadoPosId",
//							PARAMIN_INT(intEquipEstadoPosId)))
//						{
//							SNPSTRLOGLINE(strlog);
//							MSGERRBD_PARAMINV return (false);
//						}
//						// loop acoes (fim)
//
//						// Comando da consulta
//						if (!bd_conn->Execute(false))
//						{
//							SNPSTRLOGLINE(strlog);
//							return (false);
//						}
//					}
//
//					// loop obras (fim)
//				}
//				// loop Alternativas (fim)
//			}
//		}
//		catch (Exception& e)
//		{
//			SNPSTRLOGLINE(strlog);
//			SNPSTRLOG(e.Message);
//			sucesso = false;
//		}
//	}
//	__finally
//	{
//		// Colocado dentro do finally para garantir que será deletado, mesmo que
//		// haja um return no meio do código
//	}
//
//	return (sucesso);
//}// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrObraAcao(VTEstudo *estudo)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	TList* lisAlternativa = NULL;
	VTAlternativa* alternativa = NULL;
	VTObras* obras = NULL;
	TList* lisObras = NULL;
	VTObra* obra = NULL;
	TList* lisAcao = NULL;
	TList* lisItemObra = NULL;
	VTItemObra  *itemObra = NULL;
	VTAcao* acao = NULL;
	VTEqpto* eqpto = NULL;
	VTEqpto* eqpto_estadopre = NULL;
	VTEqpto* eqpto_estadopos = NULL;
	int intEquipId;
	int intEquipEstadoPreId;
	int intEquipEstadoPosId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if ((lisAlternativa = estudo->LisAlternativa()) == NULL)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISOBJINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}
	if (redes->LisRede()->Count < 1)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}

	// Se não tem alternativas, já retorna
	if (lisAlternativa->Count == 0)
		return (true);

	// Escrita propriamente dita
	__try
	{
		try
		{
			// Determina os elementos para execução dos comandos
			for (int i = 0; i < lisAlternativa->Count; i++)
			{
				// loop Alternativas (ini)
				if ((alternativa = (VTAlternativa*)lisAlternativa->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_ALTERNATIVA return (false);
				}
				if (alternativa == estudo->AlternativaPadrao)
					continue;

				if ((obras = alternativa->Obras) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_OBRAS return (false);
				}

				if ((lisObras = obras->LisObra()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_LISOBJINV return (false);
				}

				for (int j = 0; j < lisObras->Count; j++)
				{
					// loop obras (ini)
					if ((obra = (VTObra*)lisObras->Items[j]) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_OBRAS return (false);
					}

					if ((lisItemObra = obra->LisItemObra) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_ITEMSOBRA return (false);
					}
					if ((lisAcao = obra->LisAcao) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_LISOBJINV return (false);
					}
					// Reinicia o indexador de itens
					FNSNP_rdItemIdProx(estudo, alternativa, obra, false, true);

					//loop items obra somente para criar os ids
					for (int io = 0; io < lisItemObra->Count; io++)
					{
						if ((itemObra = (VTItemObra*)lisItemObra->Items[io]) == NULL)
						{
							SNPSTRLOGLINE(strlog);
							MSGERRSYS_ITEMOBRA return (false);
						}
						// define Id do ItemObra
						itemObra->Id = FNSNP_rdItemIdProx(estudo, alternativa, obra);
					}
					// loop itemObra (fim)
					// Reinicia o indexador de ações
					FNSNP_rdAcaoIdProx(estudo, alternativa, obra, false, true);

					// Monta a string de execução
					strSQL =
						"EXEC SPSNP_wrObraAcao :p_EstudoId, :p_AlternativaId, :p_ObraId, :p_AcaoId, :p_ItemId, :p_TipoAcaoId, :p_EquipOriginalId, :p_EquipEstadoPreId, :p_EquipEstadoPosId";
					// Decodifica comando
					bd_conn->ParseCmd(strSQL);

					// As ações são armazenadas na ordem inversa
					for (int k = lisAcao->Count - 1; k >= 0; k--)
					{
						// loop acoes (ini)
						if ((acao = (VTAcao*)lisAcao->Items[k]) == NULL)
						{
							SNPSTRLOGLINE(strlog);
							MSGERRSYS_ACAO return (false);
						}
						// define Id da Acao
						acao->Id = FNSNP_rdAcaoIdProx(estudo, alternativa, obra);
						// define Id do Item
						//acao->Id = FNSNP_rdItemIdProx(estudo, alternativa, obra);  //alterar para item
						// verifica o tipo de Acao
						switch (acao->Tipo())
						{
						case acaoALTERA:
							eqpto = acao->Eqpto();
							eqpto_estadopre = acao->CopiaDoEqptoOriginal();
							eqpto_estadopos = acao->CopiaDoEqptoAlterado();
							break;
						case acaoINSERE:
							eqpto = acao->Eqpto();
							eqpto_estadopre = NULL;
							eqpto_estadopos = NULL;
							break;
						case acaoRETIRA:
							eqpto = acao->Eqpto();
							eqpto_estadopre = NULL;
							eqpto_estadopos = NULL;
							break;
						default:
							continue;
						}
						// Compatibiliza os valores
						eqpto ? intEquipId = eqpto->Id : intEquipId = IDINDEFINIDO;
						eqpto_estadopre ? intEquipEstadoPreId = eqpto_estadopre->Id :
							intEquipEstadoPreId = IDINDEFINIDO;
						eqpto_estadopos ? intEquipEstadoPosId = eqpto_estadopos->Id :
							intEquipEstadoPosId = IDINDEFINIDO;
						/*
						 Pode ser feito fora do loop, pois a iniciação com acesso à base de dados é feita
						 somente uma vez.

						 // Monta a string de execução
						 strSQL = "EXEC SPSNP_wrObraAcao :p_EstudoId, :p_AlternativaId, :p_ObraId, :p_AcaoId, :p_TipoAcaoId, :p_EquipOriginalIdAux, :p_EquipAlteradoIdAux";
						 // Decodifica comando
						 bd_conn->ParseCmd(strSQL);
						 */
						// Seta os valores dos parâmetros
						if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_AlternativaId", PARAMIN_INT(alternativa->Id)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_ObraId", PARAMIN_INT(obra->Id)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_AcaoId", PARAMIN_INT(acao->Id)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						//verifica se existe um itemObra associado
						if (acao->ItemObra == NULL)
						{
                            if (!bd_conn->SetParam("p_ItemId", PARAMIN_INT(0)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
						}
						else
						{
							if (!bd_conn->SetParam("p_ItemId", PARAMIN_INT(acao->ItemObra->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
						}
						if (!bd_conn->SetParam("p_TipoAcaoId", PARAMIN_INT(acao->Tipo())))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_EquipOriginalId", PARAMIN_INT(intEquipId)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_EquipEstadoPreId",
							PARAMIN_INT(intEquipEstadoPreId)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_EquipEstadoPosId",
							PARAMIN_INT(intEquipEstadoPosId)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						// loop acoes (fim)

						// Comando da consulta
						if (!bd_conn->Execute(false))
						{
							SNPSTRLOGLINE(strlog);
							return (false);
						}
					}
					// loop obras (fim)
				}
				// loop Alternativas (fim)
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrReator(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	AnsiString strlog;

	// Insere as cargas em modo batch
	if (!SPSNP_wrReatorIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REATORINV return (false);
	}

	// Atualiza as cargas por meio de procedures
	if (!SPSNP_wrReatorUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REATORINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrReatorIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTReator* reator;
	VTCurva* curva;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Inicialmente, cria todas as curvas que não são típicas e atribui o identificador ao equipamento.
			// Assim, não há necessidade de fazer a atualização no final
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((reator = (VTReator*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_REATORINV return (false);
				}
				// Somente salva equipamentos novos
				if (!reator->StatusNovo())
					continue;

				if ((curva = reator->curva) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BDCURVAINV return (false);
				}
				if (curva->NumeroPontos() > 0)
				{
					// Se a curva for típica e o status não for novo, não precisa salvar a curva
					if ((curva->Status[sttNOVO] == true) && (!curva->Tipica))
					{
						// Inicialmente, escreve a curva, cuja função já existe na base de dados geral
						// se a curva não for típica e a descrição estiver vazia, associa o código do equipamento à curva
						if ((!curva->Tipica) && (curva->Codigo.IsEmpty()))
						{
							curva->Codigo = reator->Codigo;
						}
						if (!bd_curva->SPSNP_wrConjCurva(reator->Tipo(), curva))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRSYS_REATORINV return (false);
						}
					}
				}
			}

			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoREATOR, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}

			// -------------------------------
			// Inserção dos equipamentos (ini)
			// -------------------------------
			// Salva todos os equipamentos em um arquivo texto
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((reator = (VTReator*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_REATORINV return (false);
				}
				// Somente salva equipamentos novos
				if (!reator->StatusNovo())
					continue;

				// Formata a string para escrever no arquivo
				// strSQL = Format("%d;%d;%d;%f;%f",
				strSQL = Format("%d;%d;%d;%g;%g",
					ARRAYOFCONST((estudo->Id, reator->Id, reator->ligacao, reator->vnom,
					reator->Q_mvar)));
				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "ReatorOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "Reator (";
				strSQL += "intEstudoId, intReatorId, intTipoLigacaoId, fltVnom, fltQnom";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			// -------------------------------
			// Inserção dos equipamentos (fim)
			// -------------------------------

			// Associa as barras aos equipamentos em modo batch
			if (!SPSNP_wrEquipamentoBarraIns(estudo, lisObjRede))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_REATORINV return (false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrReatorUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTReator* reator;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}
	if (!bd_curva)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDCURVAINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				reator = (VTReator*)lisObjRede->Items[n];
				if (!reator)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_REATORINV return (false);
				}
				if ((!reator->StatusAlterado()) && (!reator->StatusDeslocado()))
					continue;

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, reator, reator->Tipo(), IDINDEFINIDO, bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
				// Monta a string de execução
				strSQL =
					"EXEC SPSNP_wrReator_p02 :p_TipoLigacaoId, :p_Vnom, :p_Qnom, :p_EstudoId, :p_ReatorId ";

				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_TipoLigacaoId", PARAMIN_INT(reator->ligacao)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Vnom", PARAMIN_FLT(reator->vnom)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Qnom", PARAMIN_FLT(reator->Q_mvar)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ReatorId", PARAMIN_INT(reator->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Escreve a curva do reator
				if (reator->curva->NumeroPontos() > 0)
				{
					if (!bd_curva->SPSNP_wrConjCurvaEquipamento(estudo, reator, reator->curva))
					{
						sucesso = false;
						SNPSTRLOGLINE(strlog);
					}
				}

				// Atualiza posicionamento na barra
				if (!SPSNP_wrEquipamentoBarraUpd(estudo, reator))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrRede(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	AnsiString strlog;

	// Utiliza o modo batch para salvar os equipamentos novos
	if (!SPSNP_wrRedeIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDEINV return (false);
	}

	// Atualiza os equipamentos por meio de procedures
	if (!SPSNP_wrRedeUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDEINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrRedeIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString straux;
	AnsiString strlog;
	VTRede* rede;
	bool bolRedutivel;
	TDateTime dttRevisao;
	wchar_t bufUserName[255];
	DWORD dwdBufSize;
	int intBarraIniId;
	int intMRedeId;
	VTArea* area_esq;
	VTArea* area_utm;
	int intAgrupamentoId;
	AnsiString strDtt;
	VTBarra* barraini;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Utiliza o modo batch para salvar os equipamentos novos
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoREDE, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_REDEINV return (false);
			}

			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((rede = (VTRede*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_REDEINV return (false);
				}
				// Somente salva equipamentos novos
				if (!rede->StatusNovo())
					continue;
				/*
				 // Determina os valores dos parâmetros
				 if (rede->MRede)
				 {
				 // Se o identificador da meta rede é menor que zero, então a meta rede é a própria rede
				 if (rede->MRede->Id < 0)
				 {
				 rede->MRede->Id = rede->Id;
				 }
				 bolRedutivel = rede->MRede->Redutivel;
				 }
				 else
				 {
				 rede->MRede->Id = IDINDEFINIDO;
				 bolRedutivel    = false;
				 }
				 */
				// determina MRede (IMPORTANTE: Rede de um estudo não tem MRede válido)
				if ((rede->MRede) && (!bolObra))
				{ // Se o identificador da meta rede é menor que zero, então a meta rede é a própria rede
					if ((intMRedeId = rede->MRede->Id) < 0)
					{
						intMRedeId = rede->Id;
					}
					bolRedutivel = rede->MRede->Redutivel;
				}
				else
				{
					intMRedeId = rede->Id;
					bolRedutivel = false;
				}

				dttRevisao = TDateTime::CurrentDateTime();
				// Não declarar nenhum formato de data no Schema.Ini. Só é possivel colocar data ou hora.
				// Data e hora, não funciona. No esquema abaixo,a string é formatada de acordo com o control
				// panel (se ninguém mexeu nas variáveis do Borland). O access aceita as entradas corretamente
				// strDtt = dttRevisao.DateTimeString();
				// strDtt.sprintf("%02d/%02d/%4d", DayOf(dttRevisao), MonthOf(dttRevisao), YearOf(dttRevisao));
				strDtt = dttRevisao.DateString();
				dwdBufSize = sizeof(bufUserName);
				GetUserName(bufUserName, &dwdBufSize);
				intBarraIniId = IDINDEFINIDO;
				if ((barraini = rede->BarraInicial()) != NULL)
				{
					barraini->Id <= 0 ? intBarraIniId = IDINDEFINIDO : intBarraIniId = barraini->Id;
				}
				area_esq = rede->AreaEsquematico();
				area_utm = rede->AreaUtm_cm();
				rede->Cluster ? intAgrupamentoId = rede->Cluster->Id : intAgrupamentoId =
					IDINDEFINIDO;

				// Na procedure original está sendo carregado NULL
				// OOPS. É isso mesmo?
				if (rede->Cluster)
				{
					if (rede->Cluster->Id == IDINDEFINIDO)
					{
						rede->Cluster->Id = 0;
					}
				}

				// Formata a string para escrever no arquivo
				// Assim, não compila. Se colocar 3 parenteses compila, mas não roda.
				// Foi feita a montagem na raça do vetor, para evitar esse problema
				/*
				 strSQL = Format("%d;%d;%d;%d;%d;%d;%d;%d;%d;%s;%d;%s;%s;%d;%d;%d;%d;%d;%d;%d;%d;%d",
				 ARRAYOFCONST((
				 estudo->Id, rede->Id, rede->TipoRede->Id, rede->MRede->Id, intAgrupamentoId, bolRedutivel ? 1 : 0, rede->Estatica ? 1 : 0, rede->Radial ? 1 : 0,
				 rede->Valida ? 0 : 1, strDtt.c_str(), (int)rede->Color, strDtt.c_str(), bufUserName,
				 intBarraIniId, (int)area_esq->x1, (int)area_esq->y1, (int)area_esq->x2, (int)area_esq->y2, (int)area_utm->x1, (int)area_utm->y1, (int)area_utm->x2, (int)area_utm->y2
				 ))
				 );
				 lstStrData->Add(strSQL);
				 */
				TVarRec args[] =
				{estudo->Id, rede->Id, rede->TipoRede->Id, intMRedeId, intAgrupamentoId,
					bolRedutivel ? 1 : 0, rede->Estatica ? 1 : 0, rede->Radial ? 1 : 0,
					rede->Valida ? 0 : 1, strDtt.c_str(), (int)rede->Color, strDtt.c_str(),
					bufUserName, intBarraIniId, (int)area_esq->x1, (int)area_esq->y1,
					(int)area_esq->x2, (int)area_esq->y2, (int)area_utm->x1, (int)area_utm->y1,
					(int)area_utm->x2, (int)area_utm->y2};
				strSQL = Format("%d;%d;%d;%d;%d;%d;%d;%d;%d;%s;%d;%s;%s;%d;%d;%d;%d;%d;%d;%d;%d;%d",
					args, (sizeof(args) / sizeof(TVarRec)) - 1);
				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "RedeOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL +=
					"Rede (intEstudoId, intRedeId, intTipoRedeId, intMRedeId, intAgrupamentoId, bolRedutivel, bolEstatica, bolRadial, ";
				strSQL +=
					"bolEquivalente, dttRevisao, intColorId, tmsModificacao, strModificador, ";
				strSQL +=
					"intBarraIniId, intEsqX1, intEsqY1, intEsqX2, intEsqY2, intUtmX1, intUtmY1, intUtmX2, intUtmY2 ";
				strSQL += ")";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18565717
bool __fastcall TBDeqptoAccess::SPSNP_wrRedeUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString username;
	VTRede* rede;
	bool bolRedutivel;
	TDateTime dttRevisao;
	wchar_t bufUserName[255];
	DWORD dwdBufSize;
	int intBarraIniId;
	int intMRedeId;
	VTArea* area_esq;
	VTArea* area_utm;
	int intAgrupamentoId;
	VTBarra* barraini;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISOBJINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				// loop Redes (ini)
				rede = (VTRede*)lisObjRede->Items[n];
				if (!rede)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_REDEINV return (false);
				}

				// Somente salva equipamentos novos alterados
				if ((!rede->StatusAlterado()) && (!rede->StatusDeslocado()))
					continue;

                // Não passa aqui se for novo
                if (rede->StatusNovo())
                    continue;

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, rede, rede->Tipo(), rede->Id, bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
				// Monta a string de execução
				strSQL =
					"EXEC SPSNP_wrRede_p02 :p_TipoRedeId, :p_MRedeId, :p_Redutivel, :p_Estatica, :p_Radial,";
				strSQL += ":p_Equivalente, :p_Revisao, :p_ColorId, :p_TmsModif, :p_Modificador,";
				strSQL +=
					":p_BarraIniId, :p_EsqX1, :p_EsqY1, :p_EsqX2, :p_EsqY2, :p_UtmX1, :p_UtmY1, :p_UtmX2, :p_UtmY2,";
				strSQL += ":p_AgrupamentoId, :p_EstudoId, :p_RedeId";
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				/*
				 // Determina os valores dos parâmetros
				 if (rede->MRede)
				 {
				 // Se o identificador da meta rede é menor que zero, então a meta rede é a própria rede
				 if (rede->MRede->Id < 0)
				 {
				 rede->MRede->Id = rede->Id;
				 }
				 bolRedutivel = rede->MRede->Redutivel;
				 }
				 else
				 {
				 rede->MRede->Id = IDINDEFINIDO;
				 bolRedutivel    = false;
				 }
				 */
				// Determina os valores dos parâmetros
				if (rede->MRede)
				{ // Se o identificador da meta rede é menor que zero, então a meta rede é a própria rede
					if ((intMRedeId = rede->MRede->Id) < 0)
					{
						intMRedeId = rede->Id;
					}
					bolRedutivel = rede->MRede->Redutivel;
				}
				else
				{
					intMRedeId = rede->Id;
					bolRedutivel = false;
				}

				dttRevisao = TDateTime::CurrentDateTime();
				dwdBufSize = sizeof(bufUserName);
				GetUserName(bufUserName, &dwdBufSize);
				username = bufUserName;
				intBarraIniId = IDINDEFINIDO;
				if ((barraini = rede->BarraInicial()) != NULL)
				{
					barraini->Id <= 0 ? intBarraIniId = IDINDEFINIDO : intBarraIniId = barraini->Id;
				}
				area_esq = rede->AreaEsquematico();
				area_utm = rede->AreaUtm_cm();
				rede->Cluster ? intAgrupamentoId = rede->Cluster->Id : intAgrupamentoId =
					IDINDEFINIDO;

				if (!bd_conn->SetParam("p_TipoRedeId", PARAMIN_INT(rede->TipoRede->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_MRedeId", PARAMIN_INT(intMRedeId)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Redutivel", PARAMIN_INT(bolRedutivel ? 1 : 0)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Estatica", PARAMIN_INT(rede->Estatica ? 1 : 0)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Radial", PARAMIN_INT(rede->Radial ? 1 : 0)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Equivalente", PARAMIN_INT(rede->Valida ? 0 : 1)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Revisao", PARAMIN_DTT(dttRevisao)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ColorId", PARAMIN_INT(rede->Color)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_TmsModif", PARAMIN_DTT(Now())))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Modificador", PARAMIN_STR(username)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_BarraIniId", PARAMIN_INT(intBarraIniId)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EsqX1", PARAMIN_INT(area_esq->x1)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EsqY1", PARAMIN_INT(area_esq->y1)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EsqX2", PARAMIN_INT(area_esq->x2)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EsqY2", PARAMIN_INT(area_esq->y2)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_UtmX1", PARAMIN_INT(area_utm->x1)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_UtmY1", PARAMIN_INT(area_utm->y1)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_UtmX2", PARAMIN_INT(area_utm->x2)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_UtmY2", PARAMIN_INT(area_utm->y2)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_AgrupamentoId", PARAMIN_INT(intAgrupamentoId)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_RedeId", PARAMIN_INT(rede->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Na procedure original está sendo carregado NULL
				// OOPS. É isso mesmo?
				if (rede->Cluster)
				{
					if (rede->Cluster->Id == IDINDEFINIDO)
					{
						rede->Cluster->Id = 0;
					}
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrRedeDominioIns(VTEstudo *estudo, TList* lisRede)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTRede* rede;
	VTDominio* dominio;
	TList* lisDom = NULL;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDEINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		// Lista auxiliar para carga de equipamentos
		lisDom = new TList();

		try
		{
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();

			// Salva todos os equipamentos em um arquivo texto
			for (int ndxRede = 0; ndxRede < lisRede->Count; ndxRede++)
			{
				// Loops das redes (ini)
				if ((rede = (VTRede*)lisRede->Items[ndxRede]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BARRAINV return (false);
				}
				// "Limpa" os domínios associados
				if (!SPSNP_dlRedeDominio(estudo, rede))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDDL_DOMREDE return (false);
				}
				// Carrega os novos domínios
				lisDom->Clear();
				redes->LisDominio(lisDom, rede);
				for (int n = 0; n < lisDom->Count; n++)
				{
					// Loop das barras por rede (ini)
					if ((dominio = (VTDominio*)lisDom->Items[n]) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_DOMINIOINV return (false);
					}

					// Formata a string para escrever no arquivo
					strSQL = Format("%d;%d;%d", ARRAYOFCONST((estudo->Id, rede->Id, dominio->Id)));
					lstStrData->Add(strSQL);
					// Loop das barras por rede (fim)
				}

				// Loops das redes (fim)
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "RedeDominioOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "RedeDominio (intEstudoId, intRedeId, intDominioId) ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código

		// destrói lista sem destruir seus objetos
		if (lisDom)
		{
			delete lisDom;
			lisDom = NULL;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrRegulador(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	AnsiString strlog;

	// Insere reguladores em modo batch
	if (!SPSNP_wrReguladorIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REGULADORINV return (false);
	}

	// Atualiza reguladores por meio de procedures
	if (!SPSNP_wrReguladorUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REGULADORINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrReguladorIns(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTRegulador* regulador;
	int intBarRef01;
	int intBarRef02, intBarRef02inv;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoREGULADOR, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}

			// -------------------------------
			// Inserção dos equipamentos (ini)
			// -------------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			int sentido_reversivel = 1; // default temporario
			int automatico = 1; // default temporario
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((regulador = (VTRegulador*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_REGULADORINV return (false);
				}
				// Somente salva equipamentos novos
				if (!regulador->StatusNovo())
					continue;
				// Formata a string para escrever no arquivo
				regulador->sentido.bar_ref ? intBarRef01 = regulador->sentido.bar_ref->Id :
					intBarRef01 = regulador->pbarra1->Id;
				regulador->cfg_auto.bar_ref ? intBarRef02 = regulador->cfg_auto.bar_ref->Id :
					intBarRef02 = regulador->pbarra2->Id;
				regulador->cfg_auto.bar_ref_inv ? intBarRef02inv =
					regulador->cfg_auto.bar_ref_inv->Id : intBarRef02inv = regulador->pbarra1->Id;

				TVarRec args[] =
				{estudo->Id, regulador->Id, regulador->ligacao, regulador->snom,
					(regulador->by_pass) ? 1 : 0, regulador->var_tensao, regulador->num_passo,
					sentido_reversivel, intBarRef01, automatico, regulador->cfg_fixo.passo,
					intBarRef02, regulador->cfg_auto.v_pu, regulador->z0.r, regulador->z0.x,
					regulador->z1.r, regulador->z1.x, regulador->modelo, regulador->fases,
					intBarRef02inv, regulador->cfg_auto.vinv_pu,
					regulador->cfg_auto.larg_bandamorta_pu,
					regulador->cfg_auto.larg_bandamorta_inv_pu, regulador->rel_tap};
				strSQL =
					Format("%d;%d;%d;%g;%d;%g;%d;%d;%d;%d;%d;%d;%g;%g;%g;%g;%g;%d;%d;%d;%g;%g;%g;%d"
					, args, (sizeof(args) / sizeof(TVarRec)) - 1);
				lstStrData->Add(strSQL);
			}
			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "ReguladorOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "Regulador (";
				strSQL +=
					"intEstudoId, intReguladorId, intTipoLigacaoId, fltSnom, bolBypass, fltFaixaVar, intNumPasso, ";
				strSQL +=
					"bolSentidoReversivel, intSentidoBarRefId, bolAjusteAuto, intCfgFixPassoSel, intCfgAutBarRefId, fltCfgAutVpu, ";
				strSQL +=
					"fltZ0R, fltZ0X, fltZ1R, fltZ1X, intModeloId, intTipoFaseId, intCfgAutBarRefInvId, fltCfgAutVinvpu, ";
				strSQL += "fltCfgAutLbmpu, fltCfgAutLbmInvpu, intRelTap";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			// -------------------------------
			// Inserção dos equipamentos (fim)
			// -------------------------------

			// Atualiza posicionamento na barra
			if (!SPSNP_wrEquipamentoBarraIns(estudo, lisObjRede))
			{
				sucesso = false;
				SNPSTRLOGLINE(strlog);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrReguladorUpd(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTRegulador* regulador;
	int intSentBarRef;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				regulador = (VTRegulador*)lisObjRede->Items[n];
				if (!regulador)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_REGULADORINV return (false);
				}
				if ((!regulador->StatusAlterado()) && (!regulador->StatusDeslocado()))
					continue;

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, regulador, regulador->Tipo(),
					regulador->rede->Id, bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
				// Monta a string de execução
				strSQL =
					"EXEC SPSNP_wrRegulador_p02 :p_TipoLigacaoId, :p_Snom, :p_Bypass, :p_FaixaVar, :p_NumPasso, :p_SentidoReversivel, :p_SentidoBarRefId, ";
				strSQL +=
					":p_AjusteAuto, :p_CfgFixPassoSel, :p_CfgAutBarRefId, :p_CfgAutVpu, :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X, " ":p_ModeloId, :p_TipoFaseId, ";
				strSQL +=
					":p_CfgAutBarRefInvId, :p_CfgAutVinvpu, :p_CfgAutLbmpu, :p_CfgAutLbmInvpu, :p_RelTap, " ":p_EstudoId, :p_ReguladorId";

				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_TipoLigacaoId", PARAMIN_INT(regulador->ligacao)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Snom", PARAMIN_FLT(regulador->snom)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Bypass", PARAMIN_INT(regulador->by_pass)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_FaixaVar", PARAMIN_FLT(regulador->var_tensao)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_NumPasso", PARAMIN_INT(regulador->num_passo)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// if (!bd_conn->SetParam("p_SentidoReversivel", PARAMIN_INT(regulador->sentido.reversivel)))  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
				if (!bd_conn->SetParam("p_SentidoReversivel", PARAMIN_INT(1)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				} // default temporario

				regulador->sentido.bar_ref ? intSentBarRef = regulador->sentido.bar_ref->Id :
					intSentBarRef = regulador->pbarra1->Id;
				if (!bd_conn->SetParam("p_SentidoBarRefId", PARAMIN_INT(intSentBarRef)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// if (!bd_conn->SetParam("p_AjusteAuto"       , PARAMIN_INT(regulador->ajuste_auto)))         { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
				if (!bd_conn->SetParam("p_AjusteAuto", PARAMIN_INT(1)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				} // default temporario
				if (!bd_conn->SetParam("p_CfgFixPassoSel", PARAMIN_INT(regulador->cfg_fixo.passo)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				regulador->cfg_auto.bar_ref ? intSentBarRef = regulador->cfg_auto.bar_ref->Id :
					intSentBarRef = regulador->pbarra2->Id;
				if (!bd_conn->SetParam("p_CfgAutBarRefId", PARAMIN_INT(intSentBarRef)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_CfgAutVpu", PARAMIN_FLT(regulador->cfg_auto.v_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				if (!bd_conn->SetParam("p_Z0R", PARAMIN_FLT(regulador->z0.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z0X", PARAMIN_FLT(regulador->z0.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z1R", PARAMIN_FLT(regulador->z1.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z1X", PARAMIN_FLT(regulador->z1.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ModeloId", PARAMIN_INT(regulador->modelo)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_TipoFaseId", PARAMIN_INT(regulador->fases)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				regulador->cfg_auto.bar_ref_inv ? intSentBarRef =
					regulador->cfg_auto.bar_ref_inv->Id : intSentBarRef = regulador->pbarra1->Id;
				if (!bd_conn->SetParam("p_CfgAutBarRefInvId", PARAMIN_INT(intSentBarRef)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_CfgAutVinvpu", PARAMIN_FLT(regulador->cfg_auto.vinv_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				if (!bd_conn->SetParam("p_CfgAutLbmpu",
					PARAMIN_FLT(regulador->cfg_auto.larg_bandamorta_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_CfgAutLbmInvpu",
					PARAMIN_FLT(regulador->cfg_auto.larg_bandamorta_inv_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				if (!bd_conn->SetParam("p_RelTap", PARAMIN_INT(regulador->rel_tap)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ReguladorId", PARAMIN_INT(regulador->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Atualiza posicionamento na barra
				if (!SPSNP_wrEquipamentoBarraUpd(estudo, regulador))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrRRCarga(VTEstudo* estudo, TList* lisMRede, bool bolObra)
{
	AnsiString strlog;

	if (cronometro)
		cronometro->Start("SPSNP_wrRRCarga - SPSNP_wrRRCargaIns");
	// Insere as cargas em modo batch
	if (!SPSNP_wrRRCargaIns(estudo, lisMRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_CARGAINV return (false);
	}
	if (cronometro)
		cronometro->Stop("SPSNP_wrRRCarga - SPSNP_wrRRCargaIns");
	/*
	 //JCG 2013.07.19 - Os equipamentos de Reducao NUNCA SÃO ATUALIZADOS
	 if (cronometro) cronometro->Start("SPSNP_wrRRCarga - SPSNP_wrRRCargaUpd");
	 // Atualiza as cargas por meio de procedures
	 if (!SPSNP_wrRRCargaUpd(estudo, lisMRede, bolObra))      { SNPSTRLOGLINE(strlog); MSGERRSYS_CARGAINV return(false); }
	 if (cronometro) cronometro->Stop("SPSNP_wrRRCarga - SPSNP_wrRRCargaUpd");
	 */
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrRRCargaIns(VTEstudo* estudo, TList* lisMRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTCarga* carga;
	VTCurva* curva;
	int intMRedeId;
	int intCorteId;
	VTMRede* mrede;
	VTReducao* reducao;
	TList* lisObjRede = NULL;
	TList* lisREDU = NULL;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisMRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISMREDEINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISOBJINV return (false);
	}

	__try
	{
		try
		{
			lisObjRede = new TList();
			for (int i = 0; i < lisMRede->Count; i++)
			{
				// Loop MRede (ini)
				if ((mrede = (VTMRede*)lisMRede->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MREDEINV return (false);
				}
				// obtém lista de Reduções da MREde
				if ((lisREDU = mrede->LisReducao()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_LISREDUINV return (false);
				}
				for (int j = 0; j < lisREDU->Count; j++)
				{
					// Loop Redu (ini)
					if ((reducao = (VTReducao*)lisREDU->Items[j]) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_REDUCAOINV return (false);
					}
					if (!reducao->MRede)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_MREDEINV return (false);
					}
					reducao->LisEqpto(lisObjRede, eqptoCARGA);
					// Loop Redu (fim)
				}
				// Loop MRede (fim)
			}

			// Inicialmente, cria todas as curvas que não são típicas e atribui o identificador ao equipamento.
			// Assim, não há necessidade de fazer a atualização no final
			if (cronometro)
				cronometro->Start("SPSNP_wrRRCargaIns - Curva");
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((carga = (VTCarga*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_CARGAINV return (false);
				}
				if ((curva = carga->curva) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BDCURVAINV return (false);
				}
				// Somente salva equipamentos novos
				if (!carga->StatusNovo())
					continue;
				if (curva->NumeroPontos() > 0)
				{
					// Se a curva for típica e o status não for novo, não precisa salvar a curva
					if ((curva->Status[sttNOVO] == true) && (!curva->Tipica))
					{
						// Inicialmente, escreve a curva, cuja função já existe na base de dados geral
						// se a curva não for típica e a descrição estiver vazia, associa o código do equipamento à curva
						if ((!curva->Tipica) && (curva->Codigo.IsEmpty()))
						{
							curva->Codigo = carga->Codigo;
						}
						if (!bd_curva->SPSNP_wrConjCurva(carga->Tipo(), curva))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRSYS_CURVAINV return (false);
						}
					}
				}
			}
			if (cronometro)
				cronometro->Stop("SPSNP_wrRRCargaIns - Curva");

			// Nesse ponto, todas as curvas próprias já foram salvas na base e o id gerado associado
			// Escreve os equipamentos no modo batch
			if (cronometro)
				cronometro->Start("SPSNP_wrRRCargaIns - SPSNP_wrEquipamentoIns");
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoCARGA, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}
			if (cronometro)
				cronometro->Stop("SPSNP_wrRRCargaIns - SPSNP_wrEquipamentoIns");

			// Gera as strings de saída
			lstStrData->Clear();
			for (int i = 0; i < lisMRede->Count; i++)
			{
				// Loop MRede (ini)
				if ((mrede = (VTMRede*)lisMRede->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MREDEINV return (false);
				}
				// obtém lista de Reduções da MREde
				if ((lisREDU = mrede->LisReducao()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_LISREDUINV return (false);
				}
				for (int j = 0; j < lisREDU->Count; j++)
				{
					// Loop Redu (ini)
					if ((reducao = (VTReducao*)lisREDU->Items[j]) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_REDUCAOINV return (false);
					}
					if (!reducao->MRede)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_MREDEINV return (false);
					}
					lisObjRede->Clear();
					reducao->LisEqpto(lisObjRede, eqptoCARGA);
					for (int n = 0; n < lisObjRede->Count; n++)
					{
						// Loop Equip (ini)
						intMRedeId = reducao->MRede->Id;
						intCorteId = reducao->Corte->Id;
						// Salva todos os equipamentos em um arquivo texto
						for (int n = 0; n < lisObjRede->Count; n++)
						{
							if ((carga = (VTCarga*)lisObjRede->Items[n]) == NULL)
							{
								SNPSTRLOGLINE(strlog);
								MSGERRSYS_BARRAINV return (false);
							}
							// Somente salva equipamentos novos
							if (!carga->StatusNovo())
								continue;
							// Formata a string para escrever no arquivo
							// strSQL = Format("%d;%d;%d;%d;%f;%f;%f",
							strSQL =
								Format("%d;%d;%d;%d;%g;%g;%g",
								ARRAYOFCONST((estudo->Id, intMRedeId, carga->Id, intCorteId,
								carga->Icte_pu, carga->Scte_pu, carga->Zcte_pu)));
							lstStrData->Add(strSQL);
						}
						// Loop Equip (fim)
					}
					// Loop Redu (fim)
				}
				// Loop MRede (fim)
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "RRCargaOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				if (cronometro)
					cronometro->Start("SPSNP_wrRRCargaIns - INSERT INTO");
				strSQL = "INSERT INTO ";
				strSQL += "RRCarga (";
				strSQL +=
					"intEstudoId, intMRedeId, intRRCargaId, intCorteId, fltIcte_pu, fltScte_pu, fltZcte_pu";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
				if (cronometro)
					cronometro->Stop("SPSNP_wrRRCargaIns - INSERT INTO");
			}

			if (cronometro)
				cronometro->Start("SPSNP_wrRRCargaIns - SPSNP_wrRREquipamentoBarra");
			// Associa as barras aos equipamentos em modo batch
			if (!SPSNP_wrRREquipamentoBarra(estudo, eqptoCARGA, lisMRede))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_CARGAINV return (false);
			}
			if (cronometro)
				cronometro->Stop("SPSNP_wrRRCargaIns - SPSNP_wrRREquipamentoBarra");
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
		if (lisObjRede)
		{
			delete lisObjRede;
			lisObjRede = NULL;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrRRCargaUpd(VTEstudo* estudo, TList* lisMRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTCarga* carga;
	TList* lisObjRede = NULL;
	int intMRedeId;
	int intCorteId;
	TList* lisREDU;
	VTMRede* mrede;
	VTReducao* reducao;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_curva)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDCURVAINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			lisObjRede = new TList();
			lisObjRede->Clear();
			for (int i = 0; i < lisMRede->Count; i++)
			{
				// Loop MRede (ini)
				if ((mrede = (VTMRede*)lisMRede->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MREDEINV return (false);
				}
				// obtém lista de Reduções da MREde
				if ((lisREDU = mrede->LisReducao()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_LISREDUINV return (false);
				}
				for (int j = 0; j < lisREDU->Count; j++)
				{
					// Loop Redu (ini)
					if ((reducao = (VTReducao*)lisREDU->Items[j]) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_REDUCAOINV return (false);
					}
					if (!reducao->MRede)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_MREDEINV return (false);
					}
					lisObjRede->Clear();
					reducao->LisEqpto(lisObjRede, eqptoCARGA);
					for (int n = 0; n < lisObjRede->Count; n++)
					{
						// Loop Equip (ini)
						intMRedeId = reducao->MRede->Id;
						intCorteId = reducao->Corte->Id;
						carga = (VTCarga*)lisObjRede->Items[n];
						if (!carga)
						{
							SNPSTRLOGLINE(strlog);
							MSGERRSYS_CARGAINV return (false);
						}
						if (!carga->curva)
						{
							SNPSTRLOGLINE(strlog);
							MSGERRSYS_CURVAINV return (false);
						}
						// Cargas de iluminação pública são escritas por um método específico
						if (carga->curva->Tipo == curvaLD)
						{
							continue;
						}
						if ((!carga->StatusAlterado()) && (!carga->StatusDeslocado()))
							continue;
						// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
						if (!SPSNP_wrEquipamentoUpd(estudo, carga, carga->Tipo(), intMRedeId,
							bolObra))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBDWR_EQPTO return (false);
						}
						// Monta a string de execução
						strSQL =
							"EXEC SPSNP_wrRRCarga_p02 :p_Icte_pu, :p_Scte_pu, :p_Zcte_pu, :p_EstudoId, :p_MRedeId, :p_RRCargaId, :p_CorteId";
						// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
						// Decodifica comando
						bd_conn->ParseCmd(strSQL);
						// Determina os valores dos parâmetros
						if (!bd_conn->SetParam("p_Icte_pu", PARAMIN_FLT(carga->Icte_pu)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_Scte_pu", PARAMIN_FLT(carga->Scte_pu)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_Zcte_pu", PARAMIN_FLT(carga->Zcte_pu)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_MRedeId", PARAMIN_INT(intMRedeId)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_RRCargaId", PARAMIN_INT(carga->Id)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_CorteId", PARAMIN_INT(intCorteId)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						// Comando da consulta
						if (!bd_conn->Execute(false))
						{
							SNPSTRLOGLINE(strlog);
							return (false);
						}
						// Escreve a curva da carga
						if (carga->curva->NumeroPontos() > 0)
						{
							if (!bd_curva->SPSNP_wrConjCurvaEquipamento(estudo, carga,
							carga->curva))
							{
								sucesso = false;
								SNPSTRLOGLINE(strlog);
							}
						}
						// Atualiza posicionamento na barra
						if (!SPSNP_wrRREquipamentoBarra(estudo, carga, intMRedeId, intCorteId))
						{
							sucesso = false;
							SNPSTRLOGLINE(strlog);
						}
						// Loop Equip (fim)
					}
					// Loop Redu (fim)
				}
				// Loop MRede (fim)
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
		if (lisObjRede)
		{
			delete lisObjRede;
			lisObjRede = NULL;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrRREquipamentoBarra(VTEstudo* estudo, int intTipoEquipId,
	TList* lisMRede)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	int intNumBarrasMax;
	VTBarra* barra;
	float posbar;
	int dx, dy;
	VTEqpto* eqpto;
	TList* lisObjRede;
	int intMRedeId;
	int intCorteId;
	VTMRede* mrede;
	TList* lisREDU;
	VTReducao* reducao;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisMRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISMREDEINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			// OOPS: Verificar os loops de salvamento no ORACLE. Há dois loops aninhados utilizando
			// variável i (definida localmente, mas seria melhor modificar o identificador de uma delas)
			// Remove as barras antigas
			if (cronometro)
				cronometro->Start("SPSNP_wrRREquipamentoBarra - SPSNP_dlRREquipamentoBarra");
			if (!SPSNP_dlRREquipamentoBarra(estudo, intTipoEquipId, lisMRede))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if (cronometro)
				cronometro->Stop("SPSNP_wrRREquipamentoBarra - SPSNP_dlRREquipamentoBarra");

			if (cronometro)
				cronometro->Start("SPSNP_wrRREquipamentoBarra - Loop");
			lstStrData->Clear();
			lisObjRede = new TList();
			for (int i = 0; i < lisMRede->Count; i++)
			{
				// Loop MRede (ini)
				if ((mrede = (VTMRede*)lisMRede->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MREDEINV return (false);
				}
				// obtém lista de Reduções da MREde
				if ((lisREDU = mrede->LisReducao()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_LISREDUINV return (false);
				}
				for (int j = 0; j < lisREDU->Count; j++)
				{
					// Loop Redu (ini)
					if ((reducao = (VTReducao*)lisREDU->Items[j]) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_REDUCAOINV return (false);
					}
					if (!reducao->MRede)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_MREDEINV return (false);
					}
					lisObjRede->Clear();
					reducao->LisEqpto(lisObjRede, intTipoEquipId);

					// Coloca os arquivos no arquivo, para serem escritos todos de uma vez
					for (int n = 0; n < lisObjRede->Count; n++)
					{
						// Loop Equip (ini)
						intMRedeId = reducao->MRede->Id;
						intCorteId = reducao->Corte->Id;
						eqpto = (VTEqpto*)lisObjRede->Items[n];
						// O comando deve ser executado para o número de barras do equipamento
						(eqpto->TipoEqbar()) ? intNumBarrasMax = 1 : intNumBarrasMax = NBAR_LIG;
						// loop das barras, de acordo com o tipo de equipamento (EqBar ou Ligação)
						for (int i = 0; i < intNumBarrasMax; i++)
						{
							// Loop barras (ini)
							if (intNumBarrasMax == 1)
							{
								barra = ((VTEqbar*)eqpto)->pbarra;
								posbar = ((VTEqbar*)eqpto)->esq.posbar;
								dx = ((VTEqbar*)eqpto)->esq.dxbar;
								dy = ((VTEqbar*)eqpto)->esq.dybar;
							}
							else
							{
								barra = ((VTLigacao*)eqpto)->Barra(i);
								if (!barra)
									continue;
								switch (i)
								{
								case 0:
									posbar = ((VTLigacao*)eqpto)->esq.posbar1;
									break;
								case 1:
									posbar = ((VTLigacao*)eqpto)->esq.posbar2;
									break;
								case 2:
									posbar = ((VTLigacao*)eqpto)->esq.posbar3;
									break;
								}
								dx = IDINDEFINIDO;
								dy = IDINDEFINIDO;
							}
						}
						// Formata a string para escrever no arquivo
						strSQL = Format("%d;%d;%d;%d;%d;%f;%d;%d",
							ARRAYOFCONST((estudo->Id, intMRedeId, eqpto->Id, intCorteId, barra->Id,
							posbar, dx, dy)));
						lstStrData->Add(strSQL);
						// Loop Equip (fim)
					}
					// Loop Redu (fim)
				}
				// Loop MRede (fim)
			}
			if (cronometro)
				cronometro->Stop("SPSNP_wrRREquipamentoBarra - Loop");

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				if (cronometro)
					cronometro->Start("SPSNP_wrRREquipamentoBarra - Salva Arquivo");
				AnsiString strFileName = "RREquipamentoBarraOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);
				if (cronometro)
					cronometro->Stop("SPSNP_wrRREquipamentoBarra - Salva Arquivo");

				if (cronometro)
					cronometro->Start("SPSNP_wrRREquipamentoBarra - INSERT INTO");
				strSQL = "INSERT INTO ";
				strSQL +=
					"RREquipamentoBarra ( intEstudoId, intMRedeId, intEquipId, intCorteId, intBarraId, fltPosBar, intDx, intDy )";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
				if (cronometro)
					cronometro->Stop("SPSNP_wrRREquipamentoBarra - INSERT INTO");
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
		delete lisObjRede;
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrRREquipamentoBarra(VTEstudo* estudo, VTEqpto* eqpto,
	int intMRedeId, int intCorteId)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	int intNumBarrasMax;
	VTBarra* barra;
	float posbar;
	int dx, dy;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!eqpto)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			// Remove as barras antigas
			if (!SPSNP_dlRREquipamentoBarra(estudo, eqpto, intMRedeId, intCorteId))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Monta a string de execução
			strSQL =
				"EXEC SPSNP_wrRREquipamentoBarra_p01 :p_EstudoId, :p_MRedeId, :p_EquipId, :p_CorteId, :p_BarraId1, :p_PosBar1, :p_Dx, :p_Dy";
			// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// O comando deve ser executado para o número de barras do equipamento
			(eqpto->TipoEqbar()) ? intNumBarrasMax = 1 : intNumBarrasMax = NBAR_LIG;
			// loop das barras, de acordo com o tipo de equipamento (EqBar ou Ligação)
			for (int i = 0; i < intNumBarrasMax; i++)
			{
				// Loop barras (ini)
				if (intNumBarrasMax == 1)
				{
					barra = ((VTEqbar*)eqpto)->pbarra;
					posbar = ((VTEqbar*)eqpto)->esq.posbar;
					dx = ((VTEqbar*)eqpto)->esq.dxbar;
					dy = ((VTEqbar*)eqpto)->esq.dybar;
				}
				else
				{
					barra = ((VTLigacao*)eqpto)->Barra(i);
					if (!barra)
						continue;
					switch (i)
					{
					case 0:
						posbar = ((VTLigacao*)eqpto)->esq.posbar1;
						break;
					case 1:
						posbar = ((VTLigacao*)eqpto)->esq.posbar2;
						break;
					case 2:
						posbar = ((VTLigacao*)eqpto)->esq.posbar3;
						break;
					}
					dx = IDINDEFINIDO;
					dy = IDINDEFINIDO;
				}

				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_MRedeId", PARAMIN_INT(intMRedeId)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_CorteId", PARAMIN_INT(intCorteId)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_BarraId1", PARAMIN_INT(barra->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_PosBar1", PARAMIN_FLT(posbar)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Dx", PARAMIN_INT(dx)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Dy", PARAMIN_INT(dy)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Loop barras (fim)
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrRRSuprimento(VTEstudo* estudo, TList* lisMRede,
	bool bolObra)
{
	AnsiString strlog;

	if (cronometro)
		cronometro->Start("SPSNP_wrRRSuprimento - SPSNP_wrRRSuprimentoIns");
	// Insere as cargas em modo batch
	if (!SPSNP_wrRRSuprimentoIns(estudo, lisMRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SUPRIMENTOINV return (false);
	}
	if (cronometro)
		cronometro->Stop("SPSNP_wrRRSuprimento - SPSNP_wrRRSuprimentoIns");
	/*
	 //JCG 2013.07.19 - Os equipamentos de Reducao NUNCA SÃO ATUALIZADOS
	 if (cronometro) cronometro->Start("SPSNP_wrRRSuprimento - SPSNP_wrRRSuprimentoUpd");
	 // Atualiza as cargas por meio de procedures
	 if (!SPSNP_wrRRSuprimentoUpd(estudo, lisMRede, bolObra))      { SNPSTRLOGLINE(strlog); MSGERRSYS_SUPRIMENTOINV return(false); }
	 if (cronometro) cronometro->Stop("SPSNP_wrRRSuprimento - SPSNP_wrRRSuprimentoUpd");
	 */
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrRRSuprimentoIns(VTEstudo* estudo, TList* lisMRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTSuprimento* suprimento;
	VTCurva* curva;
	TList* lisObjRede;
	int intMRedeId;
	int intCorteId;
	VTMRede* mrede;
	TList* lisREDU;
	VTReducao* reducao;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisMRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISMREDEINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISOBJINV return (false);
	}

	__try
	{
		try
		{
			if (cronometro)
				cronometro->Start("SPSNP_wrRRSuprimentoIns - monta lista suprimento");
			// Utiliza uma lista local, pois há chamadas intermediárias
			lisObjRede = new TList();
			for (int i = 0; i < lisMRede->Count; i++)
			{
				// Loop MRede (ini)
				if ((mrede = (VTMRede*)lisMRede->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MREDEINV return (false);
				}
				// obtém lista de Reduções da MREde
				if ((lisREDU = mrede->LisReducao()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_LISREDUINV return (false);
				}
				for (int j = 0; j < lisREDU->Count; j++)
				{
					// Loop Redu (ini)
					if ((reducao = (VTReducao*)lisREDU->Items[j]) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_REDUCAOINV return (false);
					}
					if (!reducao->MRede)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_MREDEINV return (false);
					}
					reducao->LisEqpto(lisObjRede, eqptoSUPRIMENTO);
					// Loop Redu (fim)
				}
				// Loop MRede (fim)
			}
			if (cronometro)
				cronometro->Stop("SPSNP_wrRRSuprimentoIns - monta lista suprimento");

			if (cronometro)
				cronometro->Start("SPSNP_wrRRSuprimentoIns - SPSNP_wrConjCurva");
			// Inicialmente, cria todas as curvas que não são típicas e atribui o identificador ao equipamento.
			// Assim, não há necessidade de fazer a atualização no final
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((suprimento = (VTSuprimento*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_SUPRIMENTOINV return (false);
				}
				if ((curva = suprimento->curva) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BDCURVAINV return (false);
				}
				// Somente salva equipamentos novos
				if (!suprimento->StatusNovo())
					continue;
				if (curva->NumeroPontos() > 0)
				{
					// Se a curva for típica e o status não for novo, não precisa salvar a curva
					if ((curva->Status[sttNOVO] == true) && (!curva->Tipica))
					{
						// Inicialmente, escreve a curva, cuja função já existe na base de dados geral
						// se a curva não for típica e a descrição estiver vazia, associa o código do equipamento à curva
						if ((!curva->Tipica) && (curva->Codigo.IsEmpty()))
						{
							curva->Codigo = suprimento->Codigo;
						}
						if (!bd_curva->SPSNP_wrConjCurva(suprimento->Tipo(), curva))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRSYS_CURVAINV return (false);
						}
					}
				}
			}
			if (cronometro)
				cronometro->Stop("SPSNP_wrRRSuprimentoIns - SPSNP_wrConjCurva");

			if (cronometro)
				cronometro->Start("SPSNP_wrRRSuprimentoIns - SPSNP_wrEquipamentoIns");
			// Nesse ponto, todas as curvas próprias já foram salvas na base e o id gerado associado
			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoSUPRIMENTO, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}
			if (cronometro)
				cronometro->Stop("SPSNP_wrRRSuprimentoIns - SPSNP_wrEquipamentoIns");

			if (cronometro)
				cronometro->Start("SPSNP_wrRRSuprimentoIns - monta lstStrData");
			lstStrData->Clear();
			for (int i = 0; i < lisMRede->Count; i++)
			{
				// Loop MRede (ini)
				if ((mrede = (VTMRede*)lisMRede->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MREDEINV return (false);
				}
				// obtém lista de Reduções da MREde
				if ((lisREDU = mrede->LisReducao()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_LISREDUINV return (false);
				}
				for (int j = 0; j < lisREDU->Count; j++)
				{
					// Loop Redu (ini)
					if ((reducao = (VTReducao*)lisREDU->Items[j]) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_REDUCAOINV return (false);
					}
					if (!reducao->MRede)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_MREDEINV return (false);
					}
					lisObjRede->Clear();
					reducao->LisEqpto(lisObjRede, eqptoSUPRIMENTO);
					for (int n = 0; n < lisObjRede->Count; n++)
					{
						// Loop Equip (ini)
						intMRedeId = reducao->MRede->Id;
						intCorteId = reducao->Corte->Id;
						// Salva todos os equipamentos em um arquivo texto
						for (int n = 0; n < lisObjRede->Count; n++)
						{
							if ((suprimento = (VTSuprimento*)lisObjRede->Items[n]) == NULL)
							{
								SNPSTRLOGLINE(strlog);
								MSGERRSYS_BARRAINV return (false);
							}
							// Somente salva equipamentos novos
							if (!suprimento->StatusNovo())
								continue;
							// Formata a string para escrever no arquivo
							// strSQL = Format("%d;%d;%d;%d;%d;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f",
							strSQL = Format("%d;%d;%d;%d;%d;%g;%g;%g;%g;%g;%g;%g;%g;%g;%g",
								ARRAYOFCONST((estudo->Id, intMRedeId, suprimento->Id, intCorteId,
								suprimento->tiposup, suprimento->vnom, suprimento->smax,
								suprimento->pcc_3f.p, suprimento->pcc_3f.q, suprimento->pcc_ft.p,
								suprimento->pcc_ft.q, suprimento->zeq0.r, suprimento->zeq0.x,
								suprimento->zeq1.r, suprimento->zeq1.x)));
							lstStrData->Add(strSQL);
						}
						// Loop Equip (fim)
					}
					// Loop Redu (fim)
				}
				// Loop MRede (fim)
			}
			if (cronometro)
				cronometro->Stop("SPSNP_wrRRSuprimentoIns - monta lstStrData");

			if (lstStrData->Count > 0)
			{
				if (cronometro)
					cronometro->Start("SPSNP_wrRRSuprimentoIns - INSERT INTO");
				// Executa a lista de comandos de equipamentos
				AnsiString strFileName = "RRSuprimentoOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);
				strSQL = "INSERT INTO ";
				strSQL += "RRSuprimento (";
				strSQL +=
					"intEstudoId, intMRedeId, intRRSuprimentoId, intCorteId, intTipoSuprId, fltVnom, fltSmax, ";
				strSQL += "fltPcc3F, fltQcc3F, fltPccFT, fltQccFT, ";
				strSQL += "fltZ0REqv, fltZ0XEqv, fltZ1REqv, fltZ1XEqv";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
				if (cronometro)
					cronometro->Stop("SPSNP_wrRRSuprimentoIns - INSERT INTO");
			}

			// Associa as barras aos equipamentos em modo batch
			if (cronometro)
				cronometro->Start("SPSNP_wrRRSuprimentoIns - SPSNP_wrRREquipamentoBarra");
			if (!SPSNP_wrRREquipamentoBarra(estudo, eqptoSUPRIMENTO, lisMRede))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_CARGAINV return (false);
			}
			if (cronometro)
				cronometro->Stop("SPSNP_wrRRSuprimentoIns - SPSNP_wrRREquipamentoBarra");
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
		if (lisObjRede)
		{
			delete lisObjRede;
			lisObjRede = NULL;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrRRSuprimentoUpd(VTEstudo* estudo, TList* lisMRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTSuprimento* suprimento;
	TList* lisObjRede;
	VTMRede* mrede;
	TList* lisREDU;
	VTReducao* reducao;
	int intMRedeId;
	int intCorteId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_curva)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDCURVAINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			lisObjRede = new TList();
			for (int i = 0; i < lisMRede->Count; i++)
			{
				// Loop MRede (ini)
				if ((mrede = (VTMRede*)lisMRede->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MREDEINV return (false);
				}
				// obtém lista de Reduções da MREde
				if ((lisREDU = mrede->LisReducao()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_LISREDUINV return (false);
				}
				for (int j = 0; j < lisREDU->Count; j++)
				{
					// Loop Redu (ini)
					if ((reducao = (VTReducao*)lisREDU->Items[j]) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_REDUCAOINV return (false);
					}
					if (!reducao->MRede)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_MREDEINV return (false);
					}
					reducao->LisEqpto(lisObjRede, eqptoSUPRIMENTO);
					for (int n = 0; n < lisObjRede->Count; n++)
					{
						// Loop Equip (ini)
						intMRedeId = reducao->MRede->Id;
						intCorteId = reducao->Corte->Id;
						// Salva todos os equipamentos em um arquivo texto
						for (int n = 0; n < lisObjRede->Count; n++)
						{
							suprimento = (VTSuprimento*)lisObjRede->Items[n];
							if (!suprimento)
							{
								SNPSTRLOGLINE(strlog);
								MSGERRSYS_SUPRIMENTOINV return (false);
							}
							// Verifica se o objeto é novo ou alterado
							if (!(suprimento->StatusAlterado()) && (!suprimento->StatusDeslocado()))
								continue;
							// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
							if (!SPSNP_wrEquipamentoUpd(estudo, suprimento, suprimento->Tipo(),
								intMRedeId, bolObra))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBDWR_EQPTO return (false);
							}
							// Monta a string de execução
							strSQL =
								"EXEC SPSNP_wrRRSuprimento_p02 :p_TipoSuprId, :p_Vnom, :p_Smax, :p_Pcc3F, :p_Qcc3F, :p_PccFT, :p_QccFT, ";
							strSQL +=
								":p_Z0REqv, :p_Z0XEqv, :p_Z1REqv, :p_Z1XEqv, :p_EstudoId, :p_MRedeId, :p_RRSuprimentoId, :p_CorteId";
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_TipoSuprId",
								PARAMIN_INT(suprimento->tiposup)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_Vnom", PARAMIN_FLT(suprimento->vnom)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_Smax", PARAMIN_FLT(suprimento->smax)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_Pcc3F", PARAMIN_FLT(suprimento->pcc_3f.p)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_Qcc3F", PARAMIN_FLT(suprimento->pcc_3f.q)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_PccFT", PARAMIN_FLT(suprimento->pcc_ft.p)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_QccFT", PARAMIN_FLT(suprimento->pcc_ft.q)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_Z0REqv", PARAMIN_FLT(suprimento->zeq0.r)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_Z0XEqv", PARAMIN_FLT(suprimento->zeq0.x)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_Z1REqv", PARAMIN_FLT(suprimento->zeq1.r)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_Z1XEqv", PARAMIN_FLT(suprimento->zeq1.x)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_MRedeId", PARAMIN_INT(intMRedeId)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_RRSuprimentoId", PARAMIN_INT(suprimento->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_CorteId", PARAMIN_INT(intCorteId)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Comando da consulta
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
							// Escreve a curva do suprimento
							if (suprimento->curva->NumeroPontos() > 0)
							{
								if (!bd_curva->SPSNP_wrConjCurvaEquipamento(estudo, suprimento,
									suprimento->curva))
								{
									sucesso = false;
									SNPSTRLOGLINE(strlog);
								}
							}
							// Atualiza posicionamento na barra
							if (!SPSNP_wrRREquipamentoBarra(estudo, suprimento, intMRedeId,
								intCorteId))
							{
								sucesso = false;
								SNPSTRLOGLINE(strlog);
							}
						}
					}
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrRRTrafo(VTEstudo* estudo, int intRedeId, int intCorteId,
	TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTTrafo* trafo;
	TStream* ptrStreamMatY = NULL;
	TParameter* ptrParam;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				trafo = (VTTrafo*)lisObjRede->Items[n];
				if (!trafo)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_SUPRIMENTOINV return (false);
				}
				// Monta a string de execução
				// Verifica se o objeto é novo ou alterado
				if (trafo->StatusNovo())
				{
					// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
					if (!SPSNP_wrEquipamentoIns(estudo, trafo, trafo->Tipo(), intRedeId, bolObra))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBDWR_EQPTO return (false);
					}
					// Objeto novo.
					strSQL = "EXEC SPSNP_wrRRTrafo_p01 :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X, :p_MatY, ";
					strSQL +=
						":p_Barra1Id, :p_Enrol1_TipoLigacaoId, :p_Enrol1_TipoFaseId, :p_Enrol1_Vnom, :p_Enrol1_Snom, :p_Enrol1_ZterraR, :p_Enrol1_ZterraX, ";
					strSQL +=
						":p_Barra2Id, :p_Enrol2_TipoLigacaoId, :p_Enrol2_TipoFaseId, :p_Enrol2_Vnom, :p_Enrol2_Snom, :p_Enrol2_ZterraR, :p_Enrol2_ZterraX, ";
					strSQL +=
						":p_RotFase, :p_ModeloEqv, :p_EstudoId, :p_MRedeId, :p_RRTrafoId, :p_CorteId";
				}
				else if (trafo->StatusAlterado() || trafo->StatusDeslocado())
				{
					// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
					if (!SPSNP_wrEquipamentoUpd(estudo, trafo, trafo->Tipo(), intRedeId, bolObra))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBDWR_EQPTO return (false);
					}
					// Objeto previamente existente. O identificador em memória é válido.
					strSQL = "EXEC SPSNP_wrRRTrafo_p02 :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X, :p_MatY, ";
					strSQL +=
						":p_Barra1Id, :p_Enrol1_TipoLigacaoId, :p_Enrol1_TipoFaseId, :p_Enrol1_Vnom, :p_Enrol1_Snom, :p_Enrol1_ZterraR, :p_Enrol1_ZterraX, ";
					strSQL +=
						":p_Barra2Id, :p_Enrol2_TipoLigacaoId, :p_Enrol2_TipoFaseId, :p_Enrol2_Vnom, :p_Enrol2_Snom, :p_Enrol2_ZterraR, :p_Enrol2_ZterraX, ";
					strSQL +=
						":p_RotFase, :p_ModeloEqv, p_EstudoId, :p_MRedeId, :p_RRTrafoId, :p_CorteId";
				}
				else
				{
					// Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
					continue;
				}
				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_Z0R", PARAMIN_FLT(trafo->z0.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z0X", PARAMIN_FLT(trafo->z0.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z1R", PARAMIN_FLT(trafo->z1.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z1X", PARAMIN_FLT(trafo->z1.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Barra1Id", PARAMIN_INT(trafo->BarraPrimario()->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Enrol1_TipoLigacaoId", PARAMIN_INT(trafo->pri.ligacao)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Enrol1_TipoFaseId", PARAMIN_INT(trafo->pri.fase)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Enrol1_Vnom", PARAMIN_FLT(trafo->pri.vnom)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Enrol1_Snom", PARAMIN_FLT(trafo->snom)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Enrol1_ZterraR", PARAMIN_FLT(trafo->pri.zterra.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Enrol1_ZterraX", PARAMIN_FLT(trafo->pri.zterra.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Barra2Id", PARAMIN_INT(trafo->BarraSecundario()->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Enrol2_TipoLigacaoId", PARAMIN_INT(trafo->sec.ligacao)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Enrol2_TipoFaseId", PARAMIN_INT(trafo->sec.fase)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Enrol2_Vnom", PARAMIN_FLT(trafo->sec.vnom)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Enrol2_Snom", PARAMIN_FLT(trafo->snom)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Enrol2_ZterraR", PARAMIN_FLT(trafo->sec.zterra.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Enrol2_ZterraX", PARAMIN_FLT(trafo->sec.zterra.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				if (!bd_conn->SetParam("p_RotFase", PARAMIN_FLT(trafo->sec.defasagem)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// if (!bd_conn->SetParam("p_ModeloEqv"           , PARAMIN_INT(1)))                            { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
				if (!bd_conn->SetParam("p_ModeloEqv", PARAMIN_INT(trafo->ModeloEqv)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_MRedeId", PARAMIN_INT(intRedeId)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_RRTrafoId", PARAMIN_INT(trafo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_CorteId", PARAMIN_INT(intCorteId)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// monta Stream com dados do equivalente
				if ((ptrStreamMatY = bd_func->MontaStreamTrafoEqv(trafo)) != NULL)
				{
					// A stream tem que existir durante a execução do comando
					// delete ptrStream;
					// ptrStream = NULL;
				}
				else
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_YREFINV return (false);
				}
				ptrParam = bd_conn->SetParam("p_MatY", PARAMIN_BLB(NULL));
				ptrParam->LoadFromStream(ptrStreamMatY, ptrParam->DataType);

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Atualiza posicionamento na barra
				if (!SPSNP_wrRREquipamentoBarra(estudo, trafo, intRedeId, intCorteId))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
// OOPS Não foi possível modificar SPSNP_wrRRYRef, por causa do Blob
bool __fastcall TBDeqptoAccess::SPSNP_wrRRYRef(VTEstudo* estudo, int intRedeId, int intCorteId,
	TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTYref* yref;
	complex<double>y_pu;
	int intYRefId;
	complex<double>matComplex[4][4];
	TStream* ptrStreamMatY = NULL;
	TParameter* ptrParam;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				yref = (VTYref*)lisObjRede->Items[n];
				if (!yref)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_YREFINV return (false);
				}
				// Verifica se o objeto é novo ou alterado
				if (yref->StatusNovo())
				{ // Salva o equipamento (a eventual atribuição de identficador é feita aqui)
					if (!SPSNP_wrEquipamentoIns(estudo, yref, yref->Tipo(), intRedeId, bolObra))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBDWR_EQPTO return (false);
					}
					// Objeto novo.
					strSQL =
						"EXEC SPSNP_wrRRYRef_p01 :p_Y0G, :p_Y0B, :p_Y1G, :p_Y1B, :p_MatY, :p_EstudoId, :p_MRedeId, :p_RRYRefId, :p_CorteId";
				}
				else if (yref->StatusAlterado() || yref->StatusDeslocado())
				{ // Salva o equipamento (a eventual atribuição de identficador é feita aqui)
					if (!SPSNP_wrEquipamentoUpd(estudo, yref, yref->Tipo(), intRedeId, bolObra))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBDWR_EQPTO return (false);
					}
					// Objeto previamente existente. O identificador em memória é válido.
					strSQL =
						"EXEC SPSNP_wrRRYRef_p02 :p_Y0G, :p_Y0B, :p_Y1G, :p_Y1B, :p_MatY, :p_EstudoId, :p_MRedeId, :p_RRYRefId, :p_CorteId";
				}
				else
				{ // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
					continue;
				}
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				y_pu = yref->Y0_pu;
				if (!bd_conn->SetParam("p_Y0G", PARAMIN_FLT(y_pu.real())))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Y0B", PARAMIN_FLT(y_pu.imag())))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				y_pu = yref->Y1_pu;
				if (!bd_conn->SetParam("p_Y1G", PARAMIN_FLT(y_pu.real())))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Y1B", PARAMIN_FLT(y_pu.imag())))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_MRedeId", PARAMIN_INT(intRedeId)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_RRYRefId", PARAMIN_INT(yref->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_CorteId", PARAMIN_INT(intCorteId)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				// Carrega a matriz
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						matComplex[i][j] = yref->Y_pu[i][j];
					}
				}
				if ((ptrStreamMatY = bd_func->MontaStreamMatComplex(matComplex)) != NULL)
				{
					// A stream tem que existir durante a execução do comando
					// delete ptrStream;
					// ptrStream = NULL;
				}
				else
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_YREFINV return (false);
				}
				ptrParam = bd_conn->SetParam("p_MatY", PARAMIN_BLB(NULL));
				ptrParam->LoadFromStream(ptrStreamMatY, ptrParam->DataType);

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Atualiza posicionamento na barra
				if (!SPSNP_wrRREquipamentoBarra(estudo, yref, intRedeId, intCorteId))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
		if (ptrStreamMatY)
		{
			delete ptrStreamMatY;
			ptrStreamMatY = NULL;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrSincronismo(VTEstudo* estudo)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTLock* lock;
	VTLocks* locks = (VTLocks*)apl->GetObject(__classid(VTLocks));
	TList* lisLOCKS;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!locks)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SINCRONISMOSINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			// Atualiza o indexador dos sincronismos
			FNSNP_rdSincronismoIdProx(false, true);

			if ((lisLOCKS = locks->LisLock()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_SINCRONISMOSINV return (false);
			}
			for (int n = 0; n < lisLOCKS->Count; n++)
			{
				lock = (VTLock*)lisLOCKS->Items[n];
				if (!lock)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_SINCRONISMOINV return (false);
				}

				// Monta a string de execução
				// Verifica se o objeto é novo ou alterado
				if (lock->Status[sttNOVO])
				{
					// Objeto novo.
					lock->Id = FNSNP_rdSincronismoIdProx();
					strSQL = "EXEC SPSNP_wrSincronismo_p01 :p_Codigo, :p_Enabled, :p_SincronismoId";
				}
				else if (lock->Status[sttALTERADO])
				{
					// Objeto previamente existente. O identificador em memória é válido.
					if (!SPSNP_dlSincronismoChave(lock))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_MANOBRAINV return (false);
					}
					strSQL = "EXEC SPSNP_wrSincronismo_p02 :p_Codigo, :p_Enabled, :p_SincronismoId";
				}
				else
				{
					// Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
					continue;
				}

				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_Codigo", PARAMIN_STR(lock->Codigo)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Enabled", PARAMIN_INT((lock->Status[sttENABLED]) ?
					1 : 0)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_SincronismoId", PARAMIN_INT(lock->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Insere as chaves da manobra
				if (!SPSNP_wrSincronismoChave(estudo, lock))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}

				// Atualiza o estado
				lock->Status[sttNOVO] = false;
				lock->Status[sttALTERADO] = false;
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrSincronismoChave(VTEstudo* estudo, VTLock* lock)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	TList* lisLOCKED;
	VTLocked* locked;
	VTChave* chave;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lock)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SINCRONISMOINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			// Monta a string de execução
			strSQL =
				"EXEC SPSNP_wrSincronismoChave_p01 :p_Fechada, :p_Master, :p_EstudoId, :p_ChaveId, :p_SincronismoId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// Loop dos sincronismos Master e Slave
			if ((lisLOCKED = lock->LisMaster()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_MANOBRAINV return (false);
			}
			for (int i = 0; i < lisLOCKED->Count; i++)
			{
				// Determina os valores dos parâmetros
				if ((locked = (VTLocked*)lisLOCKED->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_SINCRONISMOINV return (false);
				}
				if ((chave = locked->chave) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MANOBRAINV return (false);
				}

				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Master", PARAMIN_INT(1)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_SincronismoId", PARAMIN_INT(lock->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ChaveId", PARAMIN_INT(chave->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Fechada", PARAMIN_INT(locked->estado)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}
			}

			if ((lisLOCKED = lock->LisSlave()) == NULL)
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_MANOBRAINV return (false);
			}
			for (int i = 0; i < lisLOCKED->Count; i++)
			{
				// Determina os valores dos parâmetros
				if ((locked = (VTLocked*)lisLOCKED->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_SINCRONISMOINV return (false);
				}
				if ((chave = locked->chave) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MANOBRAINV return (false);
				}

				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Master", PARAMIN_INT(0)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_SincronismoId", PARAMIN_INT(lock->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ChaveId", PARAMIN_INT(chave->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Fechada", PARAMIN_INT(locked->estado)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrSuprimento(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	AnsiString strlog;

	// Insere as cargas em modo batch
	if (!SPSNP_wrSuprimentoIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SUPRIMENTOINV return (false);
	}

	// Atualiza as cargas por meio de procedures
	if (!SPSNP_wrSuprimentoUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SUPRIMENTOINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrSuprimentoIns(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTSuprimento* suprimento;
	VTCurva* curva;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Inicialmente, cria todas as curvas que não são típicas e atribui o identificador ao equipamento.
			// Assim, não há necessidade de fazer a atualização no final
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((suprimento = (VTSuprimento*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_SUPRIMENTOINV return (false);
				}
				// Somente salva equipamentos novos
				if (!suprimento->StatusNovo())
					continue;

				if ((curva = suprimento->curva) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BDCURVAINV return (false);
				}
				if (curva->NumeroPontos() > 0)
				{
					// Se a curva for típica e o status não for novo, não precisa salvar a curva
					if ((curva->Status[sttNOVO] == true) && (!curva->Tipica))
					{
						// Inicialmente, escreve a curva, cuja função já existe na base de dados geral
						// se a curva não for típica e a descrição estiver vazia, associa o código do equipamento à curva
						if ((!curva->Tipica) && (curva->Codigo.IsEmpty()))
						{
							curva->Codigo = suprimento->Codigo;
						}
						if (!bd_curva->SPSNP_wrConjCurva(suprimento->Tipo(), curva))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRSYS_CAPACITORINV return (false);
						}
					}
				}
			}

			// Nesse ponto, todas as curvas próprias já foram salvas na base e o id gerado associado
			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoSUPRIMENTO, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}

			// -------------------------------
			// Inserção dos equipamentos (ini)
			// -------------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((suprimento = (VTSuprimento*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_SUPRIMENTOINV return (false);
				}
				// Somente salva equipamentos novos
				if (!suprimento->StatusNovo())
					continue;

				// Formata a string para escrever no arquivo
				// strSQL = Format("%d;%d;%d;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f",
				strSQL = Format("%d;%d;%d;%g;%g;%g;%g;%g;%g;%g;%g;%g;%g",
					ARRAYOFCONST((estudo->Id, suprimento->Id, suprimento->tiposup, suprimento->vnom,
					suprimento->smax, suprimento->pcc_3f.p, suprimento->pcc_3f.q,
					suprimento->pcc_ft.p, suprimento->pcc_ft.q, suprimento->zeq0.r,
					suprimento->zeq0.x, suprimento->zeq1.r, suprimento->zeq1.x)));

				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "SuprimentoOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "Suprimento (";
				strSQL +=
					"intEstudoId, intSuprimentoId, intTipoSuprId, fltVnom, fltSmax, fltPcc3F, fltQcc3F, fltPccFT, fltQccFT, fltZ0R, fltZ0X, fltZ1R, fltZ1X";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			// -------------------------------
			// Inserção dos equipamentos (fim)
			// -------------------------------

			// Associa as barras aos equipamentos em modo batch
			if (!SPSNP_wrEquipamentoBarraIns(estudo, lisObjRede))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_CAPACITORINV return (false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrSuprimentoUpd(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTSuprimento* suprimento;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}
	if (!bd_curva)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDCURVAINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				suprimento = (VTSuprimento*)lisObjRede->Items[n];
				if (!suprimento)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_SUPRIMENTOINV return (false);
				}
				if ((!suprimento->StatusAlterado()) && (!suprimento->StatusDeslocado()))
					continue;

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, suprimento, suprimento->Tipo(), IDINDEFINIDO,
					bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
				// Monta a string de execução
				strSQL =
					"EXEC SPSNP_wrSuprimento_p02 :p_TipoSuprId, :p_Vnom, :p_Smax, :p_Pcc3F, :p_Qcc3F, :p_PccFT, :p_QccFT, ";
				strSQL += ":p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X, :p_EstudoId, :p_SuprimentoId";
				// Verifica se o objeto é novo ou alterado

				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_TipoSuprId", PARAMIN_INT(suprimento->tiposup)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Vnom", PARAMIN_FLT(suprimento->vnom)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Smax", PARAMIN_FLT(suprimento->smax)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Pcc3F", PARAMIN_FLT(suprimento->pcc_3f.p)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Qcc3F", PARAMIN_FLT(suprimento->pcc_3f.q)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_PccFT", PARAMIN_FLT(suprimento->pcc_ft.p)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_QccFT", PARAMIN_FLT(suprimento->pcc_ft.q)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z0R", PARAMIN_FLT(suprimento->zeq0.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z0X", PARAMIN_FLT(suprimento->zeq0.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z1R", PARAMIN_FLT(suprimento->zeq1.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z1X", PARAMIN_FLT(suprimento->zeq1.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_SuprimentoId", PARAMIN_INT(suprimento->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Escreve a curva do suprimento
				if (suprimento->curva->NumeroPontos() > 0)
				{
					if (!bd_curva->SPSNP_wrConjCurvaEquipamento(estudo, suprimento,
						suprimento->curva))
					{
						sucesso = false;
						SNPSTRLOGLINE(strlog);
					}
				}
				// Atualiza posicionamento na barra
				if (!SPSNP_wrEquipamentoBarraUpd(estudo, suprimento))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrTmpCurva(int intEstudoId)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			strSQL = "EXEC SPSNP_wrTmpCurva :p_EstudoId";

			// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(intEstudoId)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrTmpEquip(int intEquipId)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			strSQL = "EXEC SPSNP_wrTmpEquip :p_EquipId";

			// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(intEquipId)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}

			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}

		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
// Quando recebe uma lista, já apaga a tabela temporária e escreve em modo batch
bool __fastcall TBDeqptoAccess::SPSNP_wrTmpEquip(TList* lisObjRede)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTEqpto* eqpto;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISOBJINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISOBJINV return (false);
	}

	__try
	{
		try
		{
			// Limpa a tabela temporária
			if (!SPSNP_dlTmpEquip())
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_LISOBJINV return (false);
			}

			lstStrData->Clear();
			for (int i = 0; i < lisObjRede->Count; i++)
			{
				if ((eqpto = (VTEqpto*)lisObjRede->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_EQUIPINV return (false);
				}
				strSQL = Format("%d", ARRAYOFCONST((eqpto->Id)));
				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "TmpEquipOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "TmpEquip(";
				strSQL += "intEquipId";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrTmpEstudoMRedeCorte(VTEstudo* estudo, TList* lisMREDE)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	TList* lisCORTE;
	VTMRede* mrede;
	VTCorte* corte;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!lisMREDE)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISOBJINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISOBJINV return (false);
	}

	__try
	{
		try
		{
			// Limpa a tabela temporária
			if (!SPSNP_dlTmpEstudoRedeCorte())
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_LISOBJINV return (false);
			}
			// Limpa a lista de strings de comandos
			lstStrData->Clear();
			// Loop de estudo, rede, etc.
			for (int n = 0; n < lisMREDE->Count; n++)
			{
				if ((mrede = (VTMRede*)lisMREDE->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_LISOBJINV return (false);
				}
				if ((lisCORTE = mrede->LisCorte()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_LISOBJINV return (false);
				}
				// OOPS: Não traz os demais cortes???
				if (lisCORTE->Count != 1)
					continue;
				if ((corte = (VTCorte*)lisCORTE->First()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_PONTINV return (false);
				}
				// Armazena o dados
				strSQL = Format("%d;%d;%d", ARRAYOFCONST((estudo->Id, mrede->Id, corte->Id)));
				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "TmpEstudoRedeCorteOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "TmpEstudoRedeCorte(";
				strSQL += "intEstudoId, intMRedeId, intCorteId";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrTmpEquipEstudo(int intEstudoId)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "EXEC SPSNP_wrTmpEquipEstudo :p_EstudoId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(intEstudoId)))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRBD_PARAMINV return (false);
			}
			// Comando da consulta
			if (!bd_conn->Execute(false))
			{
				SNPSTRLOGLINE(strlog);
				return (false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrTmpEquipCorte(VTEstudo* estudo, int intTipoEquipId,
	TList* lisMRede)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTEqpto* eqpto;
	TList* lisObjRede;
	int intMRedeId;
	int intCorteId;
	VTMRede* mrede;
	TList* lisREDU;
	VTReducao* reducao;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisMRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISMREDEINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISOBJINV return (false);
	}

	__try
	{
		try
		{
			// Cria uma lista local
			lisObjRede = new TList();
			lstStrData->Clear();
			for (int i = 0; i < lisMRede->Count; i++)
			{
				// Loop MRede (ini)
				if ((mrede = (VTMRede*)lisMRede->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_MREDEINV return (false);
				}
				// obtém lista de Reduções da MREde
				if ((lisREDU = mrede->LisReducao()) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_LISREDUINV return (false);
				}
				for (int j = 0; j < lisREDU->Count; j++)
				{
					// Loop Redu (ini)
					if ((reducao = (VTReducao*)lisREDU->Items[j]) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_REDUCAOINV return (false);
					}
					if (!reducao->MRede)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_MREDEINV return (false);
					}
					lisObjRede->Clear();
					reducao->LisEqpto(lisObjRede, intTipoEquipId);
					for (int n = 0; n < lisObjRede->Count; n++)
					{
						// Loop Equip (ini)
						intMRedeId = reducao->MRede->Id;
						intCorteId = reducao->Corte->Id;
						// Salva todos os equipamentos em um arquivo texto
						for (int n = 0; n < lisObjRede->Count; n++)
						{
							if ((eqpto = (VTEqpto*)lisObjRede->Items[n]) == NULL)
							{
								SNPSTRLOGLINE(strlog);
								MSGERRSYS_EQUIPINV return (false);
							}
							// Formata a string para escrever no arquivo
							strSQL =
								Format("%d;%d;%d",
								ARRAYOFCONST((intMRedeId, eqpto->Id, intCorteId)));
							lstStrData->Add(strSQL);
						}
						// Loop Equip (fim)
					}
					// Loop Redu (fim)
				}
				// Loop MRede (fim)
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "TmpEquipCorteOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "TmpEquipCorte (";
				strSQL += "intMRedeId, intEquipId, intCorteId";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
		if (lisObjRede)
		{
			delete lisObjRede;
			lisObjRede = NULL;
		}
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrTrafo(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	AnsiString strlog;

	// Insere as cargas em modo batch
	if (!SPSNP_wrTrafoIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_TRAFOINV return (false);
	}

	// Atualiza as cargas por meio de procedures
	if (!SPSNP_wrTrafoUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_TRAFOINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrTrafoIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTTrafo* trafo;
	int intBarRefId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoTRAFO, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}

			// -------------------------------
			// Inserção dos equipamentos (ini)
			// -------------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((trafo = (VTTrafo*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_TRAFOINV return (false);
				}
				// Somente salva equipamentos novos
				if (!trafo->StatusNovo())
					continue;

				// Formata a string para escrever no arquivo
				trafo->ltc.ajuste_auto.bar_ref ? intBarRefId = trafo->ltc.ajuste_auto.bar_ref->Id :
					intBarRefId = IDINDEFINIDO;
				// strSQL = Format("%d;%d;%d;%f;%d;%d;%f;%f;%f;%f;%f",
				strSQL = Format("%d;%d;%d;%g;%d;%d;%g;%g;%g;%g;%g",
					ARRAYOFCONST((estudo->Id, trafo->Id, (int)trafo->auto_trafo, trafo->perda_ferro,
					trafo->ltc.tipo, intBarRefId, trafo->ltc.ajuste_auto.v_pu, trafo->z0.r,
					trafo->z0.x, trafo->z1.r, trafo->z1.x)));
				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "TrafoOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "Trafo (";
				strSQL += "intEstudoId, intTrafoId, bolAutoTrafo, fltPerdaFerro, intLtcTipoUtl, ";
				strSQL += "intLtcBarRefId, fltLtcVpu, fltZ0R, fltZ0X, fltZ1R, fltZ1X";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			// -------------------------------
			// Inserção dos equipamentos (fim)
			// -------------------------------

			// Atualiza posicionamento na barra
			if (!SPSNP_wrEquipamentoBarraIns(estudo, lisObjRede))
			{
				sucesso = false;
				SNPSTRLOGLINE(strlog);
			}

			// Escreve os enrolamentos do trafo
			if (!SPSNP_wrTrafoEnrolIns(estudo, lisObjRede))
			{
				sucesso = false;
				SNPSTRLOGLINE(strlog);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrTrafoUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	int intBarRefId;
	double v_pu;
	AnsiString strSQL;
	AnsiString strlog;
	VTTrafo* trafo;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				trafo = (VTTrafo*)lisObjRede->Items[n];
				if (!trafo)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_TRAFOINV return (false);
				}
				if ((!trafo->StatusAlterado()) && (!trafo->StatusDeslocado()))
					continue;

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, trafo, trafo->Tipo(), trafo->rede->Id, bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
				// Monta a string de execução
				strSQL = "EXEC SPSNP_wrTrafo_p02 :p_AutoTrafo, :p_PerdaFerro, :p_LtcTipoUtl, ";
				strSQL +=
					":p_LtcBarRefId, :p_LtcVpu, :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X, :p_EstudoId, :p_TrafoId";

				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				trafo->ltc.ajuste_auto.bar_ref ? intBarRefId = trafo->ltc.ajuste_auto.bar_ref->Id :
					intBarRefId = IDINDEFINIDO;

				if (!bd_conn->SetParam("p_AutoTrafo", PARAMIN_INT(trafo->auto_trafo)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_PerdaFerro", PARAMIN_FLT(trafo->perda_ferro)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_LtcTipoUtl", PARAMIN_INT(trafo->ltc.tipo)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_LtcBarRefId", PARAMIN_INT(intBarRefId)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_LtcVpu", PARAMIN_FLT(trafo->ltc.ajuste_auto.v_pu)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z0R", PARAMIN_FLT(trafo->z0.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z0X", PARAMIN_FLT(trafo->z0.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z1R", PARAMIN_FLT(trafo->z1.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z1X", PARAMIN_FLT(trafo->z1.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_TrafoId", PARAMIN_INT(trafo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Atualiza posicionamento na barra
				if (!SPSNP_wrEquipamentoBarraUpd(estudo, trafo))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
			}

			// Escreve os enrolamentos do trafo
			if (!SPSNP_wrTrafoEnrolIns(estudo, lisObjRede))
			{
				sucesso = false;
				SNPSTRLOGLINE(strlog);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrTrafoEnrolIns(VTEstudo* estudo, TList* lisObjRede)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTEqpto* eqpto = NULL;
	VTTrafo* trafo = NULL;
	VTTrafo3E* trafo3E = NULL;
	VTBarra* barra;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			// Primeiro apaga os enrolamentos existentes
			// OOPS: vale a pena fazer uma inserção em uma tabela temporária para apagar a lista de uma vez?
			for (int i = 0; i < lisObjRede->Count; i++)
			{
				if ((eqpto = (VTEqpto*)lisObjRede->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_EQUIPINV return (false);
				}
				if (!SPSNP_dlTrafoEnrol(estudo, eqpto))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDDL_TRAFOENROL return (false);
				}
			}

			// -------------------------------
			// Inserção dos equipamentos (ini)
			// -------------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			for (int i = 0; i < lisObjRede->Count; i++)
			{
				if ((eqpto = (VTEqpto*)lisObjRede->Items[i]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_EQUIPINV return (false);
				}

				switch (eqpto->Tipo())
				{
				case eqptoTRAFO:
					trafo = (VTTrafo*)eqpto;
					// Formata a string para escrever no arquivo
					// Enrolamento primário
					if ((barra = trafo->BarraPrimario()) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_PONTINV return (false);
					}
					// strSQL = Format("%d;%d;%d;%d;%d;%d;%f;%f;%f;%f;%f;%f",
					strSQL = Format("%d;%d;%d;%d;%d;%d;%g;%g;%g;%g;%g;%g",
						ARRAYOFCONST((estudo->Id, ((VTTrafo*)eqpto)->Id, 1, barra->Id,
						trafo->pri.ligacao, trafo->pri.fase, trafo->pri.vnom, trafo->snom,
						trafo->pri.defasagem, trafo->pri.tap, trafo->pri.zterra.r,
						trafo->pri.zterra.x)));
					lstStrData->Add(strSQL);
					// Enrolamento secundário
					if ((barra = trafo->BarraSecundario()) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_PONTINV return (false);
					}
					// strSQL = Format("%d;%d;%d;%d;%d;%d;%f;%f;%f;%f;%f;%f",
					strSQL = Format("%d;%d;%d;%d;%d;%d;%g;%g;%g;%g;%g;%g",
						ARRAYOFCONST((estudo->Id, ((VTTrafo*)eqpto)->Id, 2, barra->Id,
						trafo->sec.ligacao, trafo->sec.fase, trafo->sec.vnom, trafo->snom,
						trafo->sec.defasagem, trafo->sec.tap, trafo->sec.zterra.r,
						trafo->sec.zterra.x)));
					lstStrData->Add(strSQL);
					break;

				case eqptoTRAFO3E:
					trafo3E = (VTTrafo3E*)eqpto;
					// Formata a string para escrever no arquivo
					// Enrolamento primário
					if ((barra = trafo3E->pbarra1) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_PONTINV return (false);
					}
					// strSQL = Format("%d;%d;%d;%d;%d;%d;%f;%f;%f;%f;%f;%f",
					strSQL = Format("%d;%d;%d;%d;%d;%d;%g;%g;%g;%g;%g;%g",
						ARRAYOFCONST((estudo->Id, ((VTTrafo3E*)eqpto)->Id, 1, barra->Id,
						trafo3E->pri.ligacao, trafo3E->Fases(barra), trafo3E->pri.vnom,
						trafo3E->pri.snom, trafo3E->pri.defasagem, trafo3E->pri.tap,
						trafo3E->pri.zterra.r, trafo3E->pri.zterra.x)));
					lstStrData->Add(strSQL);
					// Enrolamento secundário
					if ((barra = trafo3E->pbarra2) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_PONTINV return (false);
					}
					// strSQL = Format("%d;%d;%d;%d;%d;%d;%f;%f;%f;%f;%f;%f",
					strSQL = Format("%d;%d;%d;%d;%d;%d;%g;%g;%g;%g;%g;%g",
						ARRAYOFCONST((estudo->Id, ((VTTrafo3E*)eqpto)->Id, 2, barra->Id,
						trafo3E->sec.ligacao, trafo3E->Fases(barra), trafo3E->sec.vnom,
						trafo3E->sec.snom, trafo3E->sec.defasagem, trafo3E->sec.tap,
						trafo3E->sec.zterra.r, trafo3E->sec.zterra.x)));
					lstStrData->Add(strSQL);
					// Enrolamento terciário
					if ((barra = trafo3E->pbarra3) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_PONTINV return (false);
					}
					// strSQL = Format("%d;%d;%d;%d;%d;%d;%f;%f;%f;%f;%f;%f",
					strSQL = Format("%d;%d;%d;%d;%d;%d;%g;%g;%g;%g;%g;%g",
						ARRAYOFCONST((estudo->Id, ((VTTrafo3E*)eqpto)->Id, 3, barra->Id,
						trafo3E->ter.ligacao, trafo3E->Fases(barra), trafo3E->ter.vnom,
						trafo3E->ter.snom, trafo3E->ter.defasagem, trafo3E->ter.tap,
						trafo3E->ter.zterra.r, trafo3E->ter.zterra.x)));
					lstStrData->Add(strSQL);
					break;
				}
			}
			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "TrafoEnrolOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "TrafoEnrol (";
				strSQL +=
					"intEstudoId, intTrafoId, intTrafoEnrolId, intBarraId, intTipoLigacaoId, intTipoFaseId, ";
				strSQL += "fltVnom, fltSnom, fltRotFase, fltVtap, fltZterraR, fltZterraX";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			// -------------------------------
			// Inserção dos equipamentos (fim)
			// -------------------------------
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrTrafoZZ(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	AnsiString strlog;

	// Insere as cargas em modo batch
	if (!SPSNP_wrTrafoZZIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_TRAFOZZINV return (false);
	}

	// Atualiza as cargas por meio de procedures
	if (!SPSNP_wrTrafoZZUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_TRAFOZZINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrTrafoZZIns(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTTrafoZZ* trafozz;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoTRAFOZZ, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}

			// -------------------------------
			// Inserção dos equipamentos (ini)
			// -------------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((trafozz = (VTTrafoZZ*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_TRAFOZZINV return (false);
				}
				// Somente salva equipamentos novos
				if (!trafozz->StatusNovo())
					continue;

				// Formata a string para escrever no arquivo
				// strSQL = Format("%d;%d;%f;%f;%f;%f",
				strSQL = Format("%d;%d;%g;%g;%g;%g",
					ARRAYOFCONST((estudo->Id, trafozz->Id, trafozz->vnom, trafozz->snom,
					trafozz->z0.r, trafozz->z0.x)));
				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "TrafoZZOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "TrafoZZ (";
				strSQL += "intEstudoId, intTrafoZZId, fltVnom, fltSnom, fltZ0R, fltZ0X";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			// -------------------------------
			// Inserção dos equipamentos (fim)
			// -------------------------------

			// Associa as barras aos equipamentos em modo batch
			if (!SPSNP_wrEquipamentoBarraIns(estudo, lisObjRede))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_TRAFOZZINV return (false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrTrafoZZUpd(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTTrafoZZ* trafoZZ;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}
	if (!bd_curva)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDCURVAINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				trafoZZ = (VTTrafoZZ*)lisObjRede->Items[n];
				if (!trafoZZ)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_TRAFOZZINV return (false);
				}
				if ((!trafoZZ->StatusAlterado()) && (!trafoZZ->StatusDeslocado()))
					continue;

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, trafoZZ, trafoZZ->Tipo(), IDINDEFINIDO,
					bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
				// Monta a string de execução
				strSQL =
					"EXEC SPSNP_wrTrafoZZ_p02 :p_Vnom, :p_Snom, :p_Z0R, :p_Z0X, :p_EstudoId, :p_TrafoZZId ";

				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_Vnom", PARAMIN_FLT(trafoZZ->vnom)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Snom", PARAMIN_FLT(trafoZZ->snom)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z0R", PARAMIN_FLT(trafoZZ->z0.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Z0X", PARAMIN_FLT(trafoZZ->z0.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_TrafoZZId", PARAMIN_INT(trafoZZ->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Atualiza posicionamento na barra
				if (!SPSNP_wrEquipamentoBarraUpd(estudo, trafoZZ))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrTrafo3E(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	AnsiString strlog;

	// Insere as cargas em modo batch
	if (!SPSNP_wrTrafo3EIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_TRAFO3EINV return (false);
	}

	// Atualiza as cargas por meio de procedures
	if (!SPSNP_wrTrafo3EUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_TRAFO3EINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrTrafo3EIns(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTTrafo3E* trafo3E;
	int intBarRefId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoTRAFO3E, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}

			// -------------------------------
			// Inserção dos equipamentos (ini)
			// -------------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((trafo3E = (VTTrafo3E*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_TRAFO3EINV return (false);
				}
				// Somente salva equipamentos novos
				if (!trafo3E->StatusNovo())
					continue;

				// Formata a string para escrever no arquivo
				// strSQL = Format("%d;%d;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;",
				strSQL = Format("%d;%d;%g;%g;%g;%g;%g;%g;%g;%g;%g;%g;%g;%g;%g;%g;",
					ARRAYOFCONST((estudo->Id, trafo3E->Id, trafo3E->perda_ferro, trafo3E->Sbase,
					trafo3E->zps0.r, trafo3E->zps0.x, trafo3E->zpt0.r, trafo3E->zpt0.x,
					trafo3E->zst0.r, trafo3E->zst0.x, trafo3E->zps1.r, trafo3E->zps1.x,
					trafo3E->zpt1.r, trafo3E->zpt1.x, trafo3E->zst1.r, trafo3E->zst1.x)));
				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "Trafo3EOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "Trafo3E (";
				strSQL += "intEstudoId, intTrafo3EId, fltPerdaFerro, fltSbase, ";
				strSQL += "fltZPS0R, fltZPS0X, fltZPT0R, fltZPT0X, fltZST0R, fltZST0X, ";
				strSQL += "fltZPS1R, fltZPS1X, fltZPT1R, fltZPT1X, fltZST1R, fltZST1X ";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			// -------------------------------
			// Inserção dos equipamentos (fim)
			// -------------------------------

			// Atualiza posicionamento na barra
			if (!SPSNP_wrEquipamentoBarraIns(estudo, lisObjRede))
			{
				sucesso = false;
				SNPSTRLOGLINE(strlog);
			}

			// Escreve os enrolamentos do trafo
			if (!SPSNP_wrTrafoEnrolIns(estudo, lisObjRede))
			{
				sucesso = false;
				SNPSTRLOGLINE(strlog);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrTrafo3EUpd(VTEstudo* estudo, TList* lisObjRede,
	bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTTrafo3E* trafo3E;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				trafo3E = (VTTrafo3E*)lisObjRede->Items[n];
				if (!trafo3E)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_TRAFO3EINV return (false);
				}
				if ((!trafo3E->StatusAlterado()) && (!trafo3E->StatusDeslocado()))
					continue;

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, trafo3E, trafo3E->Tipo(), trafo3E->rede->Id,
					bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}
				// Monta a string de execução
				strSQL =
					"EXEC SPSNP_wrTrafo3E_p02 :p_PerdaFerro, :p_Sbase, :p_ZPS0R, :p_ZPS0X, :p_ZPT0R, :p_ZPT0X, :p_ZST0R, :p_ZST0X, ";
				strSQL +=
					":p_ZPS1R, :p_ZPS1X, :p_ZPT1R, :p_ZPT1X, :p_ZST1R, :p_ZST1X, :p_EstudoId, :p_Trafo3EId";

				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_PerdaFerro", PARAMIN_FLT(trafo3E->perda_ferro)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Sbase", PARAMIN_FLT(trafo3E->Sbase)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ZPS0R", PARAMIN_FLT(trafo3E->zps0.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ZPS0X", PARAMIN_FLT(trafo3E->zps0.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ZPT0R", PARAMIN_FLT(trafo3E->zpt0.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ZPT0X", PARAMIN_FLT(trafo3E->zpt0.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ZST0R", PARAMIN_FLT(trafo3E->zst0.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ZST0X", PARAMIN_FLT(trafo3E->zst0.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ZPS1R", PARAMIN_FLT(trafo3E->zps1.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ZPS1X", PARAMIN_FLT(trafo3E->zps1.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ZPT1R", PARAMIN_FLT(trafo3E->zpt1.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ZPT1X", PARAMIN_FLT(trafo3E->zpt1.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ZST1R", PARAMIN_FLT(trafo3E->zst1.r)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ZST1X", PARAMIN_FLT(trafo3E->zst1.x)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_Trafo3EId", PARAMIN_INT(trafo3E->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Atualiza posicionamento na barra
				if (!SPSNP_wrEquipamentoBarraUpd(estudo, trafo3E))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
				// Escreve os enrolamentos do trafo
			}

			if (!SPSNP_wrTrafoEnrolIns(estudo, lisObjRede))
			{
				sucesso = false;
				SNPSTRLOGLINE(strlog);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrTrecho(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	AnsiString strlog;

	// Insere as cargas em modo batch
	if (!SPSNP_wrTrechoIns(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_TRECHOINV return (false);
	}

	// Atualiza as cargas por meio de procedures
	if (!SPSNP_wrTrechoUpd(estudo, lisObjRede, bolObra))
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_TRECHOINV return (false);
	}

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrTrechoIns(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	AnsiString straux;
	VTTrecho* trecho;
	VTBDgeral* bd_geral = (VTBDgeral*)apl->GetObject(__classid(VTBDgeral));
	bool bolReloadLastId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_EQUIPINV return (false);
	}
	if (!path)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PATHINV return (false);
	}
	if (!lstStrData)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}
	if (!bd_geral)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDGERALINV return (false);
	}

	__try
	{
		try
		{
			// Escreve os equipamentos no modo batch
			if (!SPSNP_wrEquipamentoIns(estudo, lisObjRede, eqptoTRECHO, IDINDEFINIDO, bolObra))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_EQUIPINV return (false);
			}

			// --------------------------------------------------------------------------------
			// OOPS: verificar se é necessário quebrar o salvamento dos arranjos em modo batch
			// --------------------------------------------------------------------------------

			// Atenção
			// A escrita dos cabos e suportes deve ser feita antes da escrita dos arranjos
			// Arranjos típicos já devem estar na base de dados, então salva somente arranjos próprios
			bolReloadLastId = true;
			if (cronometro)
				cronometro->Start("TrechoArranjoIns");
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((trecho = (VTTrecho*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_TRECHOINV return (false);
				}
				// Somente salva equipamentos novos
				if (!trecho->StatusNovo())
					continue;

				if (!trecho->ArranjoTipico)
				{
					if (!bd_geral->SalvaArranjo(trecho->arranjo, trecho->ArranjoTipico,
						bolReloadLastId))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBDWR_ARRANJO return (false);
					}
					// Precisa fazer isso somente uma vez
					bolReloadLastId = false;
				}
			}
			if (cronometro)
				cronometro->Stop("TrechoArranjoIns");

			// -------------------------------
			// Inserção dos equipamentos (ini)
			// -------------------------------
			// Limpa a lista de dados a serem salvos em arquivo
			lstStrData->Clear();
			// Salva todos os equipamentos em um arquivo texto
			for (int n = 0; n < lisObjRede->Count; n++)
			{
				if ((trecho = (VTTrecho*)lisObjRede->Items[n]) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_TRECHOINV return (false);
				}
				// Somente salva equipamentos novos
				if (!trecho->StatusNovo())
					continue;
				// Formata a string para escrever no arquivo
				// strSQL = Format("%d;%d;%f;%d",
				strSQL =
					Format("%d;%d;%g;%d",
					ARRAYOFCONST((estudo->Id, trecho->Id, trecho->Comprimento_km,
					trecho->arranjo->Id)));
				lstStrData->Add(strSQL);
			}

			// Executa a lista de comandos de equipamentos
			if (lstStrData->Count > 0)
			{
				AnsiString strFileName = "TrechoOut.txt";
				AnsiString strFilePath = path->DirTmp() + "\\";
				lstStrData->SaveToFile(strFilePath + strFileName);

				strSQL = "INSERT INTO ";
				strSQL += "Trecho (";
				strSQL += "intEstudoId, intTrechoId, fltComprimento, intArranjoId";
				strSQL += ") ";
				strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath +
					"' [Text;HDR=NO;]";
				bd_conn->ParseCmd(strSQL);
				bd_conn->Execute(false);
			}
			// -------------------------------
			// Inserção dos equipamentos (fim)
			// -------------------------------

			// Associa as barras aos equipamentos em modo batch
			if (!SPSNP_wrEquipamentoBarraIns(estudo, lisObjRede))
			{
				SNPSTRLOGLINE(strlog);
				MSGERRSYS_TRAFOZZINV return (false);
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::SPSNP_wrTrechoUpd(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
{
	bool sucesso = true;
	AnsiString strSQL;
	AnsiString strlog;
	VTTrecho* trecho;
	VTBDgeral* bd_geral;
	bool bolReloadLastId;

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!lisObjRede)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_LISDOMINIOINV return (false);
	}
	if (!redes)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_REDESINV return (false);
	}
	// Pega o ponteiro para o BDGeral
	bd_geral = (VTBDgeral*)apl->GetObject(__classid(VTBDgeral));
	if (bd_geral == NULL)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDGERALINV return (false);
	}

	// Escrita propriamente dita
	__try
	{
		try
		{
			// Indica para recarregar os valores dos ids da base de dados assim que possível
			bolReloadLastId = true;

			for (int n = 0; n < lisObjRede->Count; n++)
			{
				trecho = (VTTrecho*)lisObjRede->Items[n];
				if (!trecho)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_TRECHOINV return (false);
				}
				if ((!trecho->StatusAlterado()) && (!trecho->StatusDeslocado()))
					continue;

				// Salva o equipamento (a eventual atribuição de identficador é feita aqui)
				if (!SPSNP_wrEquipamentoUpd(estudo, trecho, trecho->Tipo(), trecho->rede->Id,
					bolObra))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBDWR_EQPTO return (false);
				}

				// Atenção
				// A escrita dos cabos e suportes deve ser feita antes da escrita dos arranjos
				// Arranjos típicos já devem estar na base de dados, então salva somente arranjos próprios
				// OOPS: verificar se é necessário quebrar o salvamento dos arranjos em modo batch
				if (!trecho->ArranjoTipico)
				{
					if (!bd_geral->SalvaArranjo(trecho->arranjo, trecho->ArranjoTipico,
						bolReloadLastId))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBDWR_ARRANJO return (false);
					}
					// Precisa fazer isso somente uma vez
					bolReloadLastId = false;
				}

				// Monta a string de execução
				strSQL =
					"EXEC SPSNP_wrTrecho_p02 :p_Comprimento, :p_ArranjoId, :p_EstudoId, :p_TrechoId";

				// Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
				// Decodifica comando
				bd_conn->ParseCmd(strSQL);
				// Determina os valores dos parâmetros
				if (!bd_conn->SetParam("p_Comprimento", PARAMIN_FLT(trecho->Comprimento_km)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_ArranjoId", PARAMIN_INT(trecho->arranjo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}
				if (!bd_conn->SetParam("p_TrechoId", PARAMIN_INT(trecho->Id)))
				{
					SNPSTRLOGLINE(strlog);
					MSGERRBD_PARAMINV return (false);
				}

				// Comando da consulta
				if (!bd_conn->Execute(false))
				{
					SNPSTRLOGLINE(strlog);
					return (false);
				}

				// Atualiza posicionamento na barra
				if (!SPSNP_wrEquipamentoBarraUpd(estudo, trecho))
				{
					sucesso = false;
					SNPSTRLOGLINE(strlog);
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// Colocado dentro do finally para garantir que será deletado, mesmo que
		// haja um return no meio do código
	}

	return (sucesso);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16666485
//https://sinapsisenergia.teamwork.com/#/tasks/18335568
//https://sinapsisenergia.teamwork.com/#/tasks/18761472
void __fastcall TBDeqptoAccess::TrataExternIdRedeLe(strREDE &str_rede)
{
	// variáveis locais
    TStringList *campos = new TStringList();

    try
    {//reinicia estrutura
        str_rede.dados_ext.id_alim_pai = 0;
		str_rede.dados_ext.cod_chave = "";
		str_rede.dados_ext.mae_filha_conectada = true;
        //verifica se há campos separados por _
		if(ExtraiStrings(str_rede.extern_id, "_", campos) > 3)
        {//extern_id ; id_alim_pai ; cod_chave
            str_rede.extern_id = campos->Strings[0];
			str_rede.dados_ext.id_alim_pai = StrToInt(campos->Strings[1]);
			str_rede.dados_ext.cod_chave = campos->Strings[2];
			str_rede.dados_ext.mae_filha_conectada = StrToInt(campos->Strings[3]);
		}
    }
    catch(Exception &e)
    {
        //
    }
    catch(...)
    {
        //
    }
    delete campos;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16666485
//https://sinapsisenergia.teamwork.com/#/tasks/18761472
void __fastcall TBDeqptoAccess::TrataExternIdRedeSalva(VTEqpto* eqpto)
{
	// variáveis locais
    AnsiString extern_id;
    VTRede *rede;

    //proteção
    if(eqpto == NULL) return;
    if(eqpto->Tipo() != eqptoREDE) return;
    extern_id = eqpto->Extern_id;
    try
    {//verifica se há id_alim_pai e cod_chave
        rede = (VTRede*)eqpto;
        //extern_id ; id_alim_pai ; cod_chave
		if(rede->dados_ext->IdAlimPai != 0)
        {
            extern_id += "_" + IntToStr(rede->dados_ext->IdAlimPai);
        }
        if(! rede->dados_ext->CodChave.IsEmpty())
        {
            extern_id += "_" + (rede->dados_ext->CodChave);
		}
		if(rede->dados_ext->IdAlimPai != 0)
        {
			extern_id += "_" + IntToStr(rede->dados_ext->MaeFilhaConectada);
		}
        eqpto->Extern_id = extern_id;
    }
    catch(Exception &e)
    {
        //
    }
    catch(...)
    {
        //
    }
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::TrataLixeira(VTEstudo* estudo)
{
	// variáveis locais
	bool sucesso = true;
	int intTipoEquip;
	AnsiString strSQL;
	AnsiString strlog;
	TList* lisXeira = NULL;
	VTEqpto* eqpto;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTCurva* curva;
	VTArranjo* arranjo;

	// A ordem com que os equipamentos são removidos é importante
	int vecEquipOrder[] =
	{eqptoCAPACITOR, eqptoSUPRIMENTO, eqptoTRAFOZZ, eqptoCARGA, eqptoGERADOR, eqptoFILTRO,
		eqptoMEDIDOR, eqptoCANAL, eqptoREATOR, eqptoCHAVE, eqptoCAPSERIE, eqptoREGULADOR,
		eqptoTRAFO, eqptoTRAFO3E, eqptoMUTUA,
		// eqptoTRECHO,     eqptoREDE,       eqptoBARRA,
		eqptoTRECHO, eqptoBARRA, eqptoREDE, eqptoARRANJO, eqptoSUPORTE, eqptoCABO, eqptoCLUSTER};

	if (!bd_conn)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_SESSIONCON return (false);
	}
	if (!bd_func)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_BDFUNCINV return (false);
	}
	if (!estudo)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_ESTUDOINV return (false);
	}
	if (!edita)
	{
		SNPSTRLOGLINE(strlog);
		MSGERRSYS_PONTINV return (false);
	}

	__try
	{
		try
		{
			// verifica se é um estudo novo
			if (estudo->Novo)
				return (true);

			lisXeira = new TList();

			CopiaTList(edita->Obra->Lixeira->LisEqpto(), lisXeira);

			// destrói Cortes e Eqptos de redções de todas redes excluídas
			for (int i = 0; i < lisXeira->Count; i++)
			{
				if ((eqpto = (VTEqpto*)lisXeira->Items[i]) == NULL)
					continue;
				// verifica se é um Eqpto novo
				if ((eqpto->Status[sttNOVO]) || (eqpto->Id < 0))
					continue;
				if (eqpto->Tipo() == eqptoREDE)
				{ // Limpa os cortes e reduções
					if (!SPSNP_dlEstudoCorte(estudo, eqpto->Id))
					{
						SNPSTRLOGLINE(strlog);
						return (false);
					}
				}
			}
			// loop p/ todos tipos de Eqptos
			for (int i = 0; i < (int)(sizeof(vecEquipOrder) / sizeof(int)); i++)
			{
				for (int j = lisXeira->Count - 1; j >= 0; j--)
				{
					if ((eqpto = (VTEqpto*)lisXeira->Items[j]) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRBDWR_LIXEIRA return (false);
					}
					intTipoEquip = eqpto->Tipo();
					if (intTipoEquip != vecEquipOrder[i])
						continue;
					// verifica se é um Eqpto novo
					if ((eqpto->Status[sttNOVO]) || (eqpto->Id < 0))
						continue;
					// Equipamento será removido
					lisXeira->Remove(eqpto);
					// !OOPS! Falta incluir na lista abaixo cabos, arranjos, suportes e clusters
					switch (intTipoEquip)
					{
					case eqptoCAPACITOR:
						for (int i = 1; i <= 4; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlCapacitor_p%.2d :p_EstudoId, :p_EquipId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}

						if ((curva = ((VTCapacitor*)eqpto)->curva) == NULL)
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						// proteção p/ Curva típica
						if (!curva->Tipica)
						{ // elimina Curva própria
							for (int i = 5; i <= 6; i++)
							{
								// Cria a estrutura do comando
								strSQL = Format("EXEC SPSNP_dlCapacitor_p%.2d :p_ConjCurvaId",
									ARRAYOFCONST((i)));
								// Decodifica comando
								bd_conn->ParseCmd(strSQL);
								// Determina os valores dos parâmetros
								if (!bd_conn->SetParam("p_ConjCurvaId", PARAMIN_INT(curva->Id)))
								{
									SNPSTRLOGLINE(strlog);
									MSGERRBD_PARAMINV return (false);
								}
								// Executa o comando
								if (!bd_conn->Execute(false))
								{
									SNPSTRLOGLINE(strlog);
									return (false);
								}
							}
						}
						break;

					case eqptoSUPRIMENTO:
						for (int i = 1; i <= 4; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlSuprimento_p%.2d :p_EstudoId, :p_EquipId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}

						if ((curva = ((VTSuprimento*)eqpto)->curva) == NULL)
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						for (int i = 5; i <= 6; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlSuprimento_p%.2d :p_ConjCurvaId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_ConjCurvaId", PARAMIN_INT(curva->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}
						break;

					case eqptoTRAFOZZ:
						for (int i = 1; i <= 4; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlTrafoZZ_p%.2d :p_EstudoId, :p_EquipId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}
						break;

					case eqptoCARGA:
						for (int i = 1; i <= 5; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlCarga_p%.2d :p_EstudoId, :p_EquipId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}

						if ((curva = ((VTCarga*)eqpto)->curva) == NULL)
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						for (int i = 6; i <= 7; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlCarga_p%.2d :p_ConjCurvaId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_ConjCurvaId", PARAMIN_INT(curva->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}
						break;

					case eqptoGERADOR:
						for (int i = 1; i <= 4; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlGerador_p%.2d :p_EstudoId, :p_EquipId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}

						if ((curva = ((VTGerador*)eqpto)->curva) == NULL)
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						for (int i = 5; i <= 6; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlGerador_p%.2d :p_ConjCurvaId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_ConjCurvaId", PARAMIN_INT(curva->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}
						break;

					case eqptoFILTRO:
						for (int i = 1; i <= 4; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlFiltro_p%.2d :p_EstudoId, :p_EquipId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}
						break;

					case eqptoMEDIDOR:
						for (int i = 1; i <= 5; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlMedidor_p%.2d :p_EstudoId, :p_EquipId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}
						break;

					case eqptoCANAL:
						continue; // Não faz nada

					case eqptoREATOR: // mml@20130816 - passado de 4 para 6 passos
						for (int i = 1; i <= 6; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlReator_p%.2d :p_EstudoId, :p_EquipId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}
						break;

					case eqptoCHAVE:
						for (int i = 1; i <= 4; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlChave_p%.2d :p_EstudoId, :p_EquipId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}
						break;

					case eqptoCAPSERIE:
						for (int i = 1; i <= 4; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlCapserie_p%.2d :p_EstudoId, :p_EquipId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}
						break;

					case eqptoREGULADOR:
						for (int i = 1; i <= 4; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlRegulador_p%.2d :p_EstudoId, :p_EquipId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}
						break;

					case eqptoTRAFO:
						for (int i = 1; i <= 5; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlTrafo_p%.2d :p_EstudoId, :p_EquipId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}
						break;

					case eqptoTRAFO3E:
						for (int i = 1; i <= 5; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlTrafo3E_p%.2d :p_EstudoId, :p_EquipId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}
						break;

					case eqptoMUTUA:
						for (int i = 1; i <= 4; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlMutua_p%.2d :p_EstudoId, :p_EquipId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}
						break;

					case eqptoTRECHO:
						for (int i = 1; i <= 4; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlTrecho_p%.2d :p_EstudoId, :p_EquipId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}

						if ((arranjo = ((VTTrecho*)eqpto)->arranjo) == NULL)
						{
							SNPSTRLOGLINE(strlog);
							return (false);
						}
						for (int i = 5; i <= 8; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlTrecho_p%.2d :p_ArranjoId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_ArranjoId", PARAMIN_INT(arranjo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}
						break;

					case eqptoREDE:
						// mml@20130415 - Limpa os cortes antes
						// if (!SPSNP_dlEstudoCorte(estudo, eqpto->Id))                       { SNPSTRLOGLINE(strlog);                   return(false); }

						for (int i = 1; i <= 5; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlRede_p%.2d :p_EstudoId, :p_EquipId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}
						break;

					case eqptoBARRA:
						for (int i = 1; i <= 3; i++)
						{
							// Cria a estrutura do comando
							strSQL = Format("EXEC SPSNP_dlBarra_p%.2d :p_EstudoId, :p_EquipId",
								ARRAYOFCONST((i)));
							// Decodifica comando
							bd_conn->ParseCmd(strSQL);
							// Determina os valores dos parâmetros
							if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
							{
								SNPSTRLOGLINE(strlog);
								MSGERRBD_PARAMINV return (false);
							}
							// Executa o comando
							if (!bd_conn->Execute(false))
							{
								SNPSTRLOGLINE(strlog);
								return (false);
							}
						}
						break;

					case eqptoARRANJO:
						continue; // Não faz nada

					case eqptoSUPORTE:
						continue; // Não faz nada

					case eqptoCABO:
						continue; // Não faz nada

					case eqptoCLUSTER:
						// Cria a estrutura do comando
						strSQL = "EXEC SPSNP_dlAgrupamento_p01 :p_EstudoId, :p_EquipId";
						// Decodifica comando
						bd_conn->ParseCmd(strSQL);
						// Determina os valores dos parâmetros
						if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						// Executa o comando
						if (!bd_conn->Execute(false))
						{
							SNPSTRLOGLINE(strlog);
							return (false);
						}

						// Cria a estrutura do comando
						strSQL = "EXEC SPSNP_dlAgrupamento_p02 :p_EquipId";
						// Decodifica comando
						bd_conn->ParseCmd(strSQL);
						// Determina os valores dos parâmetros
						if (!bd_conn->SetParam("p_EquipId", PARAMIN_INT(eqpto->Id)))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRBD_PARAMINV return (false);
						}
						// Executa o comando
						if (!bd_conn->Execute(false))
						{
							SNPSTRLOGLINE(strlog);
							return (false);
						}
						break;

					default:
						return (false);
					}
				}
			}
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
			sucesso = false;
		}
	}
	__finally
	{
		// destrói objetos locais
		if (lisXeira)
		{
			delete lisXeira;
			lisXeira = NULL;
		}
	}

	return (sucesso);
}

/*
 // ---------------------------------------------------------------------------
 bool __fastcall TBDeqptoAccess::ValidaVersaoBaseIgualSuperior(AnsiString versao)
 {
 // variáveis locais
 AnsiString rev_base;
 bool base_ok = false;
 TADOQuery *Query;

 // monta consulta para obter revisão da base
 bd_conn->ParseCmd("SELECT * FROM SINAP");
 if (bd_conn->Execute(true))
 {
 if ((Query = bd_conn->getDataSet()) != NULL)
 {
 if (Query->RecordCount == 1)
 {
 rev_base = Query->FieldByName("strRevisao")->AsString;
 // FKM 2018.04.05 gambiarra para eliminar o problema do X
 rev_base = UpperCase(rev_base);
 rev_base = StringReplace(rev_base, "X", "0", TReplaceFlags() << rfReplaceAll);
 base_ok = (rev_base.AnsiCompareIC(versao) >= 0);
 }
 Query->Close();
 }
 }
 return (base_ok);
 }
 */
/*
 // ---------------------------------------------------------------------------
 bool __fastcall TBDeqptoAccess::ValidaVersaoBaseParaGravacao(void)
 {
 // variáveis locais
 AnsiString rev_base;
 bool base_ok = false;
 TADOQuery *Query;

 // monta consulta para obter revisão da base
 bd_conn->ParseCmd("SELECT * FROM SINAP");
 if (bd_conn->Execute(true))
 {
 if ((Query = bd_conn->getDataSet()) != NULL)
 {
 if (Query->RecordCount == 1)
 {
 rev_base = Query->FieldByName("strRevisao")->AsString;
 // FKM 2018.04.05 gambiarra para eliminar o problema do X
 rev_base = UpperCase(rev_base);
 rev_base = StringReplace(rev_base, "X", "0", TReplaceFlags() << rfReplaceAll);
 base_ok = (rev_base.AnsiCompareIC(BASE_REV_ATUAL) >= 0);
 // if (! base_ok) base_ok  = (rev_base.AnsiCompareIC(BASE_REV_42x) == 0);
 // if (! base_ok) base_ok  = (rev_base.AnsiCompareIC(BASE_REV_41x) == 0);
 // if (! base_ok) base_ok  = (rev_base.AnsiCompareIC(BASE_REV_4x) == 0);
 ////            if (! base_ok) base_ok  = (rev_base.AnsiCompareIC(REV_BASE_OLD1) == 0);
 ////            if (! base_ok) base_ok  = (rev_base == REV_BASE_OLD2);
 }
 Query->Close();
 }
 }
 return (base_ok);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::ValidaVersaoBaseParaGravacao(void)
{
	return (VersaoBase().AnsiCompareIC(BASE_REV_ATUAL) >= 0);
}

/*
 // ---------------------------------------------------------------------------
 bool __fastcall TBDeqptoAccess::ValidaVersaoBaseParaLeitura(void)
 {
 // variáveis locais
 AnsiString rev_base;
 bool base_ok = false;
 TADOQuery *Query;

 // monta consulta para obter revisão da base
 bd_conn->ParseCmd("SELECT * FROM SINAP");
 if (bd_conn->Execute(true))
 {
 if ((Query = bd_conn->getDataSet()) != NULL)
 {
 if (Query->RecordCount == 1)
 {
 rev_base = Query->FieldByName("strRevisao")->AsString;
 // FKM 2018.04.05 gambiarra para eliminar o problema do X
 rev_base = UpperCase(rev_base);
 rev_base = StringReplace(rev_base, "X", "0", TReplaceFlags() << rfReplaceAll);
 base_ok = (rev_base.AnsiCompareIC(BASE_REV310) >= 0);
 // base_ok  = (rev_base.AnsiCompareIC(BASE_REV_ATUAL) == 0);
 // if (! base_ok) base_ok  = (rev_base.AnsiCompareIC(BASE_REV_42x) == 0);
 // if (! base_ok) base_ok  = (rev_base.AnsiCompareIC(BASE_REV_41x) == 0);
 // if (! base_ok) base_ok  = (rev_base.AnsiCompareIC(BASE_REV_4x) == 0);
 // if (! base_ok) base_ok  = (rev_base.AnsiCompareIC(BASE_REV310) == 0);
 // if (! base_ok) base_ok  = (rev_base == BASE_REV2x);
 }
 Query->Close();
 }
 }
 return (base_ok);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess::ValidaVersaoBaseParaLeitura(void)
{
	return (VersaoBase().AnsiCompareIC(BASE_REV310) >= 0);
}

// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422298
// https://sinapsisenergia.teamwork.com/#tasks/14422299
// https://sinapsisenergia.teamwork.com/#tasks/14422301
AnsiString __fastcall TBDeqptoAccess::VersaoBase(void)
{
	// variáveis locais
	AnsiString rev_base;
	TADOQuery *Query;
	AnsiString versao = "";

	// monta consulta para obter revisão da base
	bd_conn->ParseCmd("SELECT * FROM SINAP");
	if (bd_conn->Execute(true))
	{
		if ((Query = bd_conn->getDataSet()) != NULL)
		{
			if (Query->RecordCount == 1)
			{
				rev_base = Query->FieldByName("strRevisao")->AsString;
				// FKM 2018.04.05 gambiarra para eliminar o problema do X
				rev_base = UpperCase(rev_base);
				rev_base = StringReplace(rev_base, "X", "0", TReplaceFlags() << rfReplaceAll);
				if (rev_base.AnsiCompareIC(BASE_REV310) == 0)
					versao = BASE_REV310;
				else if (rev_base.AnsiCompareIC(BASE_REV_4x) == 0)
					versao = BASE_REV_4x;
				else if (rev_base.AnsiCompareIC(BASE_REV_41x) == 0)
					versao = BASE_REV_41x;
				else if (rev_base.AnsiCompareIC(BASE_REV_42x) == 0)
					versao = BASE_REV_42x;
				else if (rev_base.AnsiCompareIC(BASE_REV_421) == 0)
					versao = BASE_REV_421;
				else if (rev_base.AnsiCompareIC(BASE_REV_422) == 0)
					versao = BASE_REV_422;
				else if (rev_base.AnsiCompareIC(BASE_REV_423) == 0)
					versao = BASE_REV_423;
				else if (rev_base.AnsiCompareIC(BASE_REV_424) == 0)
					versao = BASE_REV_424;
				else if (rev_base.AnsiCompareIC(BASE_REV_ATUAL) == 0)
					versao = BASE_REV_ATUAL;
				else
					versao = "invalida";
			}
			else if (Query->RecordCount == 0)
				versao = BASE_REV310;
			Query->Close();
		}
	}
	return (versao);
}

// ---------------------------------------------------------------------------
// eof
