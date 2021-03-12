// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TBDeqptoAccess423.h"
#include "VTSNPDBConn.h"
#include "TBDfunc.h"
#include "..\Apl\VTApl.h"
#include "..\Log\VTLog.h"
#include "..\Importa\Monta\VTMonta.h"
#include "..\Obra\VTAcao.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Rede\VTBateria.h"
#include "..\Rede\VTMunicipio.h"
#include "..\Rede\VTRegulador.h"
#include "..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

#include <locale.h>

#include <cstdarg>
// ---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#tasks/14422298
// https://sinapsisenergia.teamwork.com/#tasks/14422299
// https://sinapsisenergia.teamwork.com/#tasks/14422301
// nada muda em relação a 422, só os campos que ficaram maiores na base
VTBDeqpto* NewObjBDeqptoAccess423(VTApl *apl_owner, VTSNPDBConn *bd_conn)
{
	return (new TBDeqptoAccess423(apl_owner, bd_conn));
}

// ---------------------------------------------------------------------------
__fastcall TBDeqptoAccess423::TBDeqptoAccess423(VTApl *apl_owner, VTSNPDBConn *bd_conn)
	: TBDeqptoAccess(apl_owner, bd_conn)
{
	// a classe básica já faz tudo
}

// ---------------------------------------------------------------------------
__fastcall TBDeqptoAccess423::~TBDeqptoAccess423(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/13952424
bool __fastcall TBDeqptoAccess423::SPSNP_rdBateria(VTEstudo* estudo, bool bolUseRedeTmp,
	bool bolObra)
{ // tabela nova
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTBateria* bateria = NULL;
	strBATERIA str_bateria;
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
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdBateria_p01 :p_EstudoId, :p_AlteradoObra";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdBateria_p02 :p_EstudoId, :p_AlteradoObra";
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
				intEquipId = ptrQryDS->FieldByName("BateriaId")->AsInteger;
				// Protege contra equipamentos com número de barras inválido
				if (intEquipIdPrev != intEquipId)
				{
					intEquipIdPrev = intEquipId;
				}
				// preenche estrutura strBateria
				str_bateria.bat_id = intEquipId;
				str_bateria.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
				str_bateria.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
				str_bateria.fases = ptrQryDS->FieldByName("TipoFaseId")->AsInteger;
				str_bateria.tipo_operacao = ptrQryDS->FieldByName("TipoOperacaoId")->AsInteger;

				str_bateria.modelo.scte_pu = ptrQryDS->FieldByName("SCtePu")->AsFloat;
				str_bateria.modelo.icte_pu = ptrQryDS->FieldByName("ICtePu")->AsFloat;
				str_bateria.modelo.zcte_pu = ptrQryDS->FieldByName("ZCtePu")->AsFloat;

				str_bateria.modelo_carga = ptrQryDS->FieldByName("ModeloCarga")->AsInteger;
				str_bateria.modelo_desc = ptrQryDS->FieldByName("ModeloDescarga")->AsInteger;

				str_bateria.p_nom_kw = ptrQryDS->FieldByName("PnomkW")->AsFloat;
				str_bateria.e_nom_kwh = ptrQryDS->FieldByName("EnomkWh")->AsFloat;
				str_bateria.cap_inv_kva = ptrQryDS->FieldByName("CapinvkVA")->AsFloat;

				str_bateria.p_ociosa_pu = ptrQryDS->FieldByName("Pociosapu")->AsFloat;
				str_bateria.q_ociosa_pu = ptrQryDS->FieldByName("Qociosapu")->AsFloat;

				str_bateria.rend_carga_pu = ptrQryDS->FieldByName("RendCargapu")->AsFloat;
				str_bateria.rend_desc_pu = ptrQryDS->FieldByName("RendDescpu")->AsFloat;

				str_bateria.gatilho_carga_pu = ptrQryDS->FieldByName("GatilhoCargapu")->AsFloat;
				str_bateria.gatilho_desc_pu = ptrQryDS->FieldByName("GatilhoDescpu")->AsFloat;

				str_bateria.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;

				str_bateria.bar_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
				str_bateria.esq.posbar = ptrQryDS->FieldByName("PosBar")->AsFloat;
				str_bateria.esq.dxbar = ptrQryDS->FieldByName("DX")->AsInteger;
				str_bateria.esq.dybar = ptrQryDS->FieldByName("DY")->AsInteger;

				str_bateria.show = ptrQryDS->FieldByName("Show")->AsInteger;

				// Comentário do equipamento
				str_bateria.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
				str_bateria.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
				str_bateria.comentario.comentario = ptrQryDS->FieldByName("Comentario")->AsString;

				if ((bateria = (VTBateria*)monta->InsereBateria(str_bateria)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_BATERIAINV sucesso = false;
					break;
				}
				else
				{
					bd_func->DefineStatusEqpto(bateria, estudo->Novo);
				}
			}

			ptrQryDS->Next();
			// loop registros (end)
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
bool __fastcall TBDeqptoAccess423::SPSNP_rdEstudo(void)
{ // igual versao 410
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	strPLAN_ESTUDO str_estudo;
	VTEstudo* estudo;

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

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdEstudo :p_EstudoId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(IDINDEFINIDO)))
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
				str_estudo.estudo_id = ptrQryDS->FieldByName("EstudoId")->AsInteger;
				str_estudo.tipo = ptrQryDS->FieldByName("TipoEstudoId")->AsInteger;
				str_estudo.codigo = ptrQryDS->FieldByName("EstudoDescricao")->AsString;
				str_estudo.autor = ptrQryDS->FieldByName("EstudoProprietario")->AsString;
				str_estudo.data = ptrQryDS->FieldByName("EstudoModificacao")->AsDateTime;
				str_estudo.justificativa = ptrQryDS->FieldByName("Justificativa")->AsString;

				if ((estudo = monta->InsereEstudo(str_estudo)) == NULL)
				{
					SNPSTRLOGLINE(strlog);
					MSGERRSYS_ESTUDOINV sucesso = false;
					break;
				}

				// Define o estado como false
				estudo->Novo = false;

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
bool __fastcall TBDeqptoAccess423::SPSNP_rdEstudoAlternativa(VTEstudo* estudo)
{ // inserção do campo justificativa, obra bloqueada e obra finalizada, item id
	bool sucesso = true;
	double tipo_padrao;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	strPLAN_ESTUDO str_estudo;
	strPLAN_ALTERNATIVA str_alternativa;
	strPLAN_OBRA str_obra;
	strPLAN_ACAO str_acao;
	int intAlternativaId;
	int intAlternativaIdAnterior;
	int intObraId;
	int intObraIdAnterior;
	int intItemObraId;
	int intItemObraIdAnterior;
	int intAcaoId;

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
		try
		{
			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdEstudoAlternativa :p_EstudoId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
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
			intAlternativaIdAnterior = IDINDEFINIDO;
			intObraIdAnterior = IDINDEFINIDO;
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				intAlternativaId = ptrQryDS->FieldByName("AlternativaId")->AsInteger;
				if (intAlternativaId != intAlternativaIdAnterior)
				{
					if (intAlternativaIdAnterior == IDINDEFINIDO)
					{
						// Primeira alternativa, então salva o estudo
						str_estudo.estudo_id = ptrQryDS->FieldByName("EstudoId")->AsInteger;
						// !OOPS! Desconsidera o estudo zero
						str_estudo.codigo = ptrQryDS->FieldByName("EstudoDescricao")->AsString;
						str_estudo.autor = ptrQryDS->FieldByName("EstudoProprietario")->AsString;
						str_estudo.data = ptrQryDS->FieldByName("EstudoModificacao")->AsDateTime;
						str_estudo.justificativa =
							ptrQryDS->FieldByName("EstudoJustificativa")->AsString;
						if (!monta->InsereEstudo(str_estudo))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRSYS_ESTUDOINV sucesso = false;
							break;
						}
					}

					// Nova Alternativa (ini)
					str_alternativa.estudo_id = str_estudo.estudo_id;
					str_alternativa.alternativa_id = intAlternativaId;
					str_alternativa.codigo = ptrQryDS->FieldByName("AlternativaDescricao")
						->AsString;
					str_alternativa.justificativa =
						ptrQryDS->FieldByName("AlternativaJustificativa")->AsString;
					// !OOPS! gambiarra para não inserir a rede zero duas vezes
					if (!monta->InsereEstudo(str_alternativa))
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_ALTERNATIVAINV sucesso = false;
						break;
					}

					intAlternativaIdAnterior = intAlternativaId;
					intObraIdAnterior = IDINDEFINIDO;
					// Nova Alternativa (fim)
				}

				intObraId = ptrQryDS->FieldByName("ObraId")->AsInteger;
				intItemObraId = ptrQryDS->FieldByName("ItemId")->AsInteger;
				if (intObraId > 0)
				{
					if ((intObraId != intObraIdAnterior)||(intItemObraId != intItemObraIdAnterior))
					{
						// Nova Obra (ini)
						str_obra.estudo_id = str_alternativa.estudo_id;
						str_obra.alternativa_id = str_alternativa.alternativa_id;
						str_obra.obra_id = intObraId;
						str_obra.codigo = ptrQryDS->FieldByName("ObraDescricao")->AsString;
						// str_obra.data           = ptrQryDS->FieldByName("ObraDataProjeto")->AsDateTime;
						str_obra.data_inicio = ptrQryDS->FieldByName("ObraDataProjeto")->AsDateTime;
						str_obra.data_ativacao =
							ptrQryDS->FieldByName("ObraDataExecucao")->AsDateTime;
						str_obra.responsavel = ptrQryDS->FieldByName("ObraResponsavel")->AsString;
						str_obra.ob_especial = ptrQryDS->FieldByName("ObraObrigEspecial")->AsFloat;
						str_obra.custo = ptrQryDS->FieldByName("ObraCusto")->AsFloat;
						// str_obra.custo_om     = ptrQryDS->FieldByName("DeltaCustoOM")->AsFloat;
						// lê valor único para tipo+padrao armazenado em p_DeltaCustoOM
						// JCG 2014.02.26  =============================
						// eliminar esse código
						tipo_padrao = ptrQryDS->FieldByName("DeltaCustoOM")->AsFloat;
						// determina tipo e id do ipe_atributo da Obra
						str_obra.tipo = int(tipo_padrao) / 1000;
						str_obra.ipe_atributo_id = int(tipo_padrao) % 1000;
						// fim             =============================
						// JCG 2016.07.29 - dados adicionais
						str_obra.altera_taxa_falha =
							bool(ptrQryDS->FieldByName("bolAlteraTaxaFalha")->AsInteger);
						str_obra.taxa_falha_ins = ptrQryDS->FieldByName("fltTaxaFalhaIns")->AsFloat;
						str_obra.taxa_falha_sub = ptrQryDS->FieldByName("fltTaxaFalhaSub")->AsFloat;
						str_obra.finalizada =
							bool(ptrQryDS->FieldByName("bolFinalizada")->AsInteger);
						str_obra.bloqueada = bool(ptrQryDS->FieldByName("bolBloqueada")->AsInteger);
						str_obra.item_id = ptrQryDS->FieldByName("ItemId")->AsInteger;
						// fim             =============================
						// insere a Obra
						if (!monta->InsereEstudo(str_obra))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRSYS_OBRAINV sucesso = false;
							break;
						}
						intObraIdAnterior = intObraId;
						intItemObraIdAnterior = intItemObraId;
						// Nova Obra (fim)
					}

					intAcaoId = ptrQryDS->FieldByName("AcaoId")->AsInteger;
					if (intAcaoId > 0)
					{
						// Insere a nova ação
						str_acao.estudo_id = str_obra.estudo_id;
						str_acao.alternativa_id = str_obra.alternativa_id;
						str_acao.obra_id = str_obra.obra_id;
						str_acao.acao_id = intAcaoId;
						str_acao.tipo_acao = ptrQryDS->FieldByName("TipoAcaoId")->AsInteger;
						str_acao.tipo_eqpto = ptrQryDS->FieldByName("TipoEquipId")->AsInteger;
						str_acao.item_id = ptrQryDS->FieldByName("ItemId")->AsInteger;
						str_acao.eqpto_id = ptrQryDS->FieldByName("EquipOriginalId")->AsInteger;
						str_acao.eqpto_antes_id =
							ptrQryDS->FieldByName("EquipEstadoPreId")->AsInteger;
						str_acao.eqpto_depos_id =
							ptrQryDS->FieldByName("EquipEstadoPosId")->AsInteger;
						/*
						 if ((str_acao.alternativa_id == 1) &&(str_acao.obra_id == 6) && (intAcaoId == 10))
						 {
						 Aviso("Acao Insere chave");
						 int a=0;
						 }
						 */
						if (!monta->InsereEstudo(str_acao))
						{
							// IMPORTANTE:  JCG 2013.07.16
							// Existem Acoes de alterar barras fictícias de redução. Estas Barras não
							// podem ser que localizadas pois não são salvas na Base.
							// Estas Ações não devem ser consideradas erros
							// SNPSTRLOGLINE(strlog);
							// MSGERRSYS_ACAOINV
							// sucesso = false;
							// break;
							if (str_acao.tipo_acao != acaoALTERA)
							{
								SNPSTRLOGLINE(strlog);
								MSGERRSYS_ACAOINV sucesso = false;
								break;
							}
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
bool __fastcall TBDeqptoAccess423::SPSNP_rdEstudoObraAtribExt(VTEstudo* estudo)
{ // item id
	bool sucesso = true;
	double tipo_padrao;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	strPLAN_IPE str_ipe;
	TStream* ptrStream;
	TStringList* slFields;

	// proteção
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
			strSQL = "EXEC SPSNP_rdEstudoObraAtribExt :p_EstudoId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo->Id)))
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
			/*
			 // Cria stringList
			 slFields = new TStringList();
			 //valida campos
			 ptrQryDS->GetFieldNames(slFields);
			 if(slFields->IndexOf("Descr") < 0)
			 {
			 InsereCampo("ObraAtribExt", "Descr", "TEXT(255)");
			 }
			 delete slFields;
			 */
			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				str_ipe.estudo_id = estudo->Id;
				str_ipe.alternativa_id = ptrQryDS->FieldByName("AlternativaId")->AsInteger;
				str_ipe.obra_id = ptrQryDS->FieldByName("ObraId")->AsInteger;
				str_ipe.atributo_tipo = ptrQryDS->FieldByName("ExtId")->AsInteger;
				str_ipe.atributo_id = ptrQryDS->FieldByName("AtribId")->AsInteger;
				str_ipe.descricao = ptrQryDS->FieldByName("Descr")->AsString;
				str_ipe.item_id = ptrQryDS->FieldByName("ItemId")->AsInteger;
				// Aqui falta ler o Blob e salvar, se existir. Na versão inicial, o Blob é sempre nulo.
				/*
				 ptrStream = ptrQryDS->CreateBlobStream(ptrQryDS->FieldByName("BlobExt"), bmRead);
				 if (!ptrStream)
				 {
				 SNPSTRLOGLINE(strlog);
				 sucesso = false;
				 break;
				 }
				 if (!bd_func->DesmontaStreamDescricao(ptrStream, str_ipe.descricao))
				 {
				 SNPSTRLOGLINE(strlog);
				 sucesso = false;
				 break;
				 }
				 */
				monta->InsereEstudo(str_ipe);
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
bool __fastcall TBDeqptoAccess423::SPSNP_rdMunicipio(void)
{//tabela nova
	VTMunicipio *municipio = (VTMunicipio*)apl->GetObject(__classid(VTMunicipio));
	int id, codigo;
	AnsiString nome, regiao;
	TADOQuery* ptrQryDS = NULL;
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

	__try
	{
		try
		{//limpa municipios existentes
			municipio->Limpa();
			strSQL = " \
			 SELECT \
			   intMunicipioId, intExternId, strCodigo, strRegiao \
			 FROM \
			   Municipios";
			bd_conn->ParseCmd(strSQL);
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
				id = ptrQryDS->FieldByName("intMunicipioId")->AsInteger;
				codigo = ptrQryDS->FieldByName("intExternId")->AsInteger;
				nome = ptrQryDS->FieldByName("strCodigo")->AsString;
				regiao = ptrQryDS->FieldByName("strRegiao")->AsString;
				// preenche Municipio
				municipio->Insere(codigo, nome, regiao);
				ptrQryDS->Next();
			}
			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			SNPSTRLOGLINE(strlog);
			SNPSTRLOG(e.Message);
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
	return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess423::SPSNP_rdRegulador(VTEstudo* estudo, bool bolUseRedeTmp,
	bool bolObra)
{ // igual versao 422 (alternando estudo com alterado obra, pois o script agora está certo)
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTRegulador* regulador = NULL;
	strREGULADOR str_regulador;
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
			if (bolUseRedeTmp)
			{
				strSQL = "EXEC SPSNP_rdRegulador_p01 :p_EstudoId, :p_AlteradoObra";
				// strSQL = "EXEC SPSNP_rdRegulador_p01 :p_AlteradoObra, :p_EstudoId";
			}
			else
			{
				strSQL = "EXEC SPSNP_rdRegulador_p02 :p_EstudoId, :p_AlteradoObra";
				// strSQL = "EXEC SPSNP_rdRegulador_p02 :p_AlteradoObra, :p_EstudoId";
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
				intEquipId = ptrQryDS->FieldByName("ReguladorId")->AsInteger;
				// Protege contra equipamentos com número de barras inválido
				if (intEquipIdPrev != intEquipId)
				{
					intEquipIdPrev = intEquipId;
					ndxBarra = 0;
				}
				// preenche estrutura strREGULADOR
				if (ndxBarra == 0)
				{
					str_regulador.id = intEquipId;
					str_regulador.rede_id = ptrQryDS->FieldByName("RedeId")->AsInteger;
					str_regulador.codigo = ptrQryDS->FieldByName("Codigo")->AsString;
					str_regulador.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
					str_regulador.ligacao = ptrQryDS->FieldByName("TipoLigacaoId")->AsInteger;
					// str_regulador.ajuste_auto         = (bool)ptrQryDS->FieldByName("AjusteAuto")->AsInteger;
					str_regulador.snom_mva = ptrQryDS->FieldByName("Snom")->AsFloat;
					str_regulador.by_pass = (bool)ptrQryDS->FieldByName("Bypass")->AsInteger;
					str_regulador.var_tensao = ptrQryDS->FieldByName("FaixaVar")->AsFloat;
					str_regulador.num_passo = ptrQryDS->FieldByName("NumPasso")->AsInteger;
					str_regulador.fases = ptrQryDS->FieldByName("TipoFaseId")->AsInteger;
					str_regulador.modelo = ptrQryDS->FieldByName("ModeloId")->AsInteger;

					str_regulador.cfg_fixo.passo =
						ptrQryDS->FieldByName("CfgFixPassoSel")->AsInteger;
					str_regulador.cfg_auto.bar_ref_id =
						ptrQryDS->FieldByName("CfgAutBarRefId")->AsInteger;
					str_regulador.cfg_auto.v_pu = ptrQryDS->FieldByName("CfgAutVpu")->AsFloat;
					str_regulador.obra = (bool)ptrQryDS->FieldByName("AlteradoObra")->AsInteger;

					// str_regulador.sentido.reversivel  = (bool)ptrQryDS->FieldByName("SentidoReversivel")->AsInteger;
					str_regulador.sentido.bar_ref_id =
						ptrQryDS->FieldByName("SentidoBarRefId")->AsInteger;
					str_regulador.z0.r = ptrQryDS->FieldByName("Z0R")->AsFloat;
					str_regulador.z0.x = ptrQryDS->FieldByName("Z0X")->AsFloat;
					str_regulador.z1.r = ptrQryDS->FieldByName("Z1R")->AsFloat;
					str_regulador.z1.x = ptrQryDS->FieldByName("Z1X")->AsFloat;

					str_regulador.bar1_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
					str_regulador.esq.posbar1 = ptrQryDS->FieldByName("PosBar")->AsFloat;
					str_regulador.show = ptrQryDS->FieldByName("Show")->AsInteger;
					str_regulador.cfg_auto.bar_ref_inv_id =
						ptrQryDS->FieldByName("CfgAutBarRefInvId")->AsInteger;
					str_regulador.cfg_auto.vinv_pu = ptrQryDS->FieldByName("CfgAutVinvpu")->AsFloat;
					// Como eh a versao nova - pega os valores de largura de banda
					str_regulador.cfg_auto.larg_bandamorta_pu =
						ptrQryDS->FieldByName("CfgAutLbmpu")->AsFloat;
					str_regulador.cfg_auto.larg_bandamorta_inv_pu =
						ptrQryDS->FieldByName("CfgAutLbmInvpu")->AsFloat;

					// relação de tap
					str_regulador.rel_tap = ptrQryDS->FieldByName("RelTap")->AsInteger;

					// Comentário do equipamento
					str_regulador.comentario.dx = ptrQryDS->FieldByName("DxComentario")->AsFloat;
					str_regulador.comentario.dy = ptrQryDS->FieldByName("DyComentario")->AsFloat;
					str_regulador.comentario.comentario =
						ptrQryDS->FieldByName("Comentario")->AsString;
				}
				else
				{
					str_regulador.bar2_id = ptrQryDS->FieldByName("BarraId")->AsInteger;
					str_regulador.esq.posbar2 = ptrQryDS->FieldByName("PosBar")->AsFloat;

					if ((regulador = (VTRegulador*)monta->InsereRegulador(str_regulador)) == NULL)
					{
						SNPSTRLOGLINE(strlog);
						MSGERRSYS_REGULADORINV sucesso = false;
						break;
					}
					else
					{
						bd_func->DefineStatusEqpto(regulador, estudo->Novo);
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
