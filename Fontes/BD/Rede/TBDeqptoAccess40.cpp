// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TBDeqptoAccess40.h"
#include "VTSNPDBConn.h"
#include "TBDfunc.h"
#include "..\Apl\VTApl.h"
#include "..\Log\VTLog.h"
#include "..\Importa\Monta\VTMonta.h"
#include "..\Obra\VTAcao.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Rede\VTRegulador.h"
#include "..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

#include <locale.h>

#include <cstdarg>

// ---------------------------------------------------------------------------
VTBDeqpto* NewObjBDeqptoAccess40(VTApl *apl_owner, VTSNPDBConn *bd_conn)
{
	return (new TBDeqptoAccess40(apl_owner, bd_conn));
}

// ---------------------------------------------------------------------------
__fastcall TBDeqptoAccess40::TBDeqptoAccess40(VTApl *apl_owner, VTSNPDBConn *bd_conn)
	: TBDeqptoAccess(apl_owner, bd_conn)
{
	// a classe básica já faz tudo
}

// ---------------------------------------------------------------------------
__fastcall TBDeqptoAccess40::~TBDeqptoAccess40(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess40::SPSNP_rdBateria(VTEstudo* estudo, bool bolUseRedeTmp,
	bool bolObra)
{
	// a tabela nao existe
	return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess40::SPSNP_rdEstudo(void)
{//não possui justificativa
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
bool __fastcall TBDeqptoAccess40::SPSNP_rdEstudoAlternativa(VTEstudo* estudo)
{//leitura da taxa de falha
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
				if (intObraId > 0)
				{
					if (intObraId != intObraIdAnterior)
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
                        str_obra.finalizada = false;
						str_obra.bloqueada = false;
						// fim             =============================
						// insere a Obra
						if (!monta->InsereEstudo(str_obra))
						{
							SNPSTRLOGLINE(strlog);
							MSGERRSYS_OBRAINV sucesso = false;
							break;
						}
						intObraIdAnterior = intObraId;
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
bool __fastcall TBDeqptoAccess40::SPSNP_rdEstudoObraAtribExt(VTEstudo* estudo)
{//não possui campo Descrição
	bool sucesso = true;
	double tipo_padrao;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	strPLAN_IPE str_ipe;

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

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				str_ipe.estudo_id = estudo->Id;
				str_ipe.alternativa_id = ptrQryDS->FieldByName("AlternativaId")->AsInteger;
				str_ipe.obra_id = ptrQryDS->FieldByName("ObraId")->AsInteger;
				str_ipe.atributo_tipo = ptrQryDS->FieldByName("ExtId")->AsInteger;
				str_ipe.atributo_id = ptrQryDS->FieldByName("AtribId")->AsInteger;
				// Aqui falta ler o Blob e salvar, se existir. Na versão inicial, o Blob é sempre nulo.

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
bool __fastcall TBDeqptoAccess40::SPSNP_rdMunicipio(void)
{
	// a tabela nao existe
	return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TBDeqptoAccess40::SPSNP_rdRegulador(VTEstudo* estudo, bool bolUseRedeTmp,
	bool bolObra)
{//tensao inversa igual direta, nao possui campo largura de banda
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
			}
			else
			{
				strSQL = "EXEC SPSNP_rdRegulador_p02 :p_EstudoId, :p_AlteradoObra";
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
					// adota igual a barra sentido direto
					str_regulador.cfg_auto.bar_ref_inv_id =
						ptrQryDS->FieldByName("CfgAutBarRefId")->AsInteger;
					str_regulador.cfg_auto.vinv_pu = ptrQryDS->FieldByName("CfgAutVpu")->AsFloat;

					// adota zero caso já que não é uma versão nova
					str_regulador.cfg_auto.larg_bandamorta_pu = 0;
					str_regulador.cfg_auto.larg_bandamorta_inv_pu = 0;

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
