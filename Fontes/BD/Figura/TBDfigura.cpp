// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Graphics.hpp>
//#include <System.Hash.hpp>
// #include "SNPGlobal.h"
#include "TBDfigura.h"
#include "..\VTSNPDBConn.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Figura\Estilos\VTEstilos.h"
#include "..\Figura\Estilos\VTEstilo.h"
#include "..\Figura\Estilos\VTEstiloLink.h"
#include "..\Figura\VTFiguraComentario.h"
#include "..\Figura\VTFiguras.h"
#include "..\Figura\VTLink.h"
#include "..\Figura\VTRetangulo.h"
#include "..\Figura\VTFigura.h"
#include "..\Log\VTLog.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include "..\Planejamento\VTAlternativa.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTBDfigura* NewObjBDfigura(VTApl *apl_owner, VTSNPDBConn *bd_conn)
{
	return (new TBDfigura(apl_owner, bd_conn));
}

// ---------------------------------------------------------------------------
__fastcall TBDfigura::TBDfigura(VTApl *apl_owner, VTSNPDBConn *bd_conn)
{
	// variáveis locais
	VTGeral *geral;

	// salva ponteiro
	this->apl = apl_owner;
	this->bd_conn = bd_conn;

	// obtém demais ponteiros
	figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
	estilos = (VTEstilos*)apl->GetObject(__classid(VTEstilos));
	redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	infoset = geral->Infoset;
	lisOBRA = new TList();
	// inicia ponteiros de uso geral, buscando no Apl
	plog = (VTLog*)apl->GetObject(__classid(VTLog));
//	AtualizaIndices();
}

// ---------------------------------------------------------------------------
__fastcall TBDfigura::~TBDfigura(void)
{
	// nada a fazer
	if (lisOBRA != NULL)
	{
		delete lisOBRA;
		lisOBRA = NULL;
	}

}

// ---------------------------------------------------------------------------
// bool __fastcall TBDfigura::Conecta(void)
// {
// /*
// //verifica se a base de dados já está aberta
// if (!bd_conn) return(false);
// // OOPZ: removi o path de conexão
// // O comando abaixo tenta se conectar se não estiver conectado
// // Verificar o path em que se conecta
// if (bd_conn->getConnectedOK()) return(true);
// */
// return (true);
// }

// ---------------------------------------------------------------------------
// void __fastcall TBDfigura::Desconecta(void)
// {
// // OOPZ: Não está fechando a conexão. Isso é feito ao destruir o objeto de conexão
// }

//// ---------------------------------------------------------------------------
//void __fastcall TBDfigura::AtualizaIndices(void)
//{
//	index.ndxAtributoGraficoId = FNSNP_rdAtributoGraficoIdProx();
//	index.ndxEstiloAtributoId = FNSNP_rdEstiloAtributoIdProx();
//	index.ndxEstiloId = FNSNP_rdEstiloIdProx();
//	index.ndxFiguraId = FNSNP_rdFiguraIdProx();
//	index.ndxFormaAtributoId = FNSNP_rdFormaAtributoIdProx();
//	index.ndxFormaId = FNSNP_rdFormaIdProx();
//	index.ndxLinkId   = FNSNP_rdLinkIdProx();
//	index.ndxTextoId = FNSNP_rdTextoIdProx();
//	index.ndxTipoValorAtributoGraficoId = FNSNP_rdTipoValorAtributoGraficoIdProx();
//	index.ndxValorFormaAtributoId = FNSNP_rdValorFormaAtributoIdProx();
//}
// ---------------------------------------------------------------------------
bool __fastcall TBDfigura::DeleteFigura(VTFigura *figura)
{
    bool sucesso = true;
	TList *lisLINK;
	VTLink *link;

	try
	{ // guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;
		if (figura == NULL)
		{
			return false;
		}

		//deve apagar os textos e links vinculados
		//deleta links
		if ((sucesso = SPSNP_dlAllLink(figura->Id)) == false)
		{
			Loga("");
		}
		//deleta texto
		if ((sucesso = SPSNP_dlAllTexto(figura->Id)) == false)
		{
			Loga("");
		}
		//deleta figura
		if ((sucesso = SPSNP_dlFigura(figura->Id)) == false)
		{
			Loga("");
		}
		//apaga da memoria
		if ((sucesso = figuras->DestroiFigura(figura)) == false)
		{
			Loga("");
		}
	}
	catch(Exception &e)
	{
		Loga(mErrorFunction + " " + e.Message);
		sucesso = false;
	}
	catch(...)
	{
		Loga(mErrorFunction);
		sucesso = false;
	}

	return sucesso;
}

//// ---------------------------------------------------------------------------
//VTObra* __fastcall TBDfigura::ExisteObra(int obra_id)
//{
//	// Variáveis locais
//	VTObra *obra = NULL;
//	VTObra *aux = NULL;
//	TList *lisOBRA = new TList;
//
//	try
//	{
//		// Verifica se a obra é da Rede original
//		if (obra_id == -1)
//		{
//			return (NULL);
//		}
//		if (PD.estudo == NULL)
//		{
//            return NULL;
//		}
//		PD.estudo->LisObraEstudo(lisOBRA);
//		for (int no = 0; no < lisOBRA->Count; no++)
//		{
//			aux = (VTObra*)lisOBRA->Items[no];
//			if (aux->Id == obra_id)
//			{
//				obra = aux;
//				break;
//			}
//		}
//		if (lisOBRA)
//		{
//			delete lisOBRA;
//			lisOBRA = NULL;
//		}
//	}
//	catch(...)
//	{
//		obra = NULL;
//		if (lisOBRA)
//		{
//			delete lisOBRA;
//			lisOBRA = NULL;
//		}
//    }
//	return (obra);
//}

// ---------------------------------------------------------------------------
bool __fastcall TBDfigura::FiguraFilename(AnsiString &filename)
{
	// proteção
	if (bd_conn == NULL)
		return (false);

	filename = ExtractFileName(bd_conn->getDBName());
	filename = "Figura_" + ChangeFileExt(filename, ".txt");

	return (true);
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
//https://sinapsisenergia.teamwork.com/#/tasks/16526463
AnsiString __fastcall TBDfigura::IdsComentariosSQL(void)
{
	AnsiString figuraIds ="";
	TList *lisCom;
	bool inicializado = false;
	VTFiguraComentario *comentario;

	try
	{
		lisCom = figuras->LisComentario();
		if (lisCom->Count == 0)
		{
            figuraIds ="0";
		}
		for (int nc = 0; nc < lisCom->Count; nc++)
		{
			comentario = (VTFiguraComentario*)lisCom->Items[nc];
			if (comentario->Id > 0)
			{
				if (inicializado == false)
				{
					figuraIds += IntToStr(comentario->Id);
				}
				else
				{
                    figuraIds += ", " + IntToStr(comentario->Id);
                }
				inicializado = true;
			}
		}
	}
	catch(...)
	{
		figuraIds = "0";
	}

    return figuraIds;
}
//// ---------------------------------------------------------------------------
//int  __fastcall TBDfigura::FNSNP_rdAtributoGraficoIdProx(void)
//{
//	index.ndxAtributoGraficoId = QueryNextId("AtributoGrafico", "intAtributoGraficoId");
//	return (index.ndxAtributoGraficoId);
//}
//// ---------------------------------------------------------------------------
//int  __fastcall TBDfigura::FNSNP_rdEstiloAtributoIdProx(void)
//{
//	index.ndxEstiloAtributoId = QueryNextId("EstiloAtributo", "intEstiloAtributoId");
//	return (index.ndxEstiloAtributoId);
//}
//// ---------------------------------------------------------------------------
//int __fastcall TBDfigura::FNSNP_rdEstiloIdProx(void)
//{
//	index.ndxEstiloId = QueryNextId("Estilo", "intEstiloId");
//	return (index.ndxEstiloId);
//}
//// ---------------------------------------------------------------------------
//int __fastcall TBDfigura::FNSNP_rdFiguraIdProx(void)
//{
//	index.ndxFiguraId = QueryNextId("Figura", "intFiguraId");
//	return (index.ndxFiguraId);
//}
//// ---------------------------------------------------------------------------
//int  __fastcall TBDfigura::FNSNP_rdFormaAtributoIdProx(void)
//{
//	index.ndxFormaAtributoId = QueryNextId("FormaAtributo", "intFormaAtributoId");
//	return (index.ndxFormaAtributoId);
//}
//// ---------------------------------------------------------------------------
//int  __fastcall TBDfigura::FNSNP_rdFormaIdProx(void)
//{
//	index.ndxFormaId = QueryNextId("Forma", "intFormaId");
//	return (index.ndxFormaId);
//}
//// ---------------------------------------------------------------------------
//int __fastcall TBDfigura::FNSNP_rdLinkIdProx(void)
//{
//	index.ndxLinkId = QueryNextId("Link", "intLinkId");
//	return (index.ndxLinkId);
//}
//// ---------------------------------------------------------------------------
//int  __fastcall TBDfigura::FNSNP_rdTextoIdProx(void)
//{
//	index.ndxTextoId = QueryNextId("Texto", "intTextoId");
//	return (index.ndxTextoId);
//}
//// ---------------------------------------------------------------------------
//int  __fastcall TBDfigura::FNSNP_rdTipoValorAtributoGraficoIdProx(void)
//{
//	index.ndxTipoValorAtributoGraficoId = QueryNextId("TipoValorAtributoGrafico", "intTipoValorAtributoGraficoId");
//	return (index.ndxTipoValorAtributoGraficoId);
//}
//// ---------------------------------------------------------------------------
//int  __fastcall TBDfigura::FNSNP_rdValorFormaAtributoIdProx(void)
//{
//	index.ndxValorFormaAtributoId = QueryNextId("ValorFormaAtributo", "intValorFormaAtributoId");
//	return (index.ndxValorFormaAtributoId);
//}
// ---------------------------------------------------------------------------
//bool __fastcall TBDfigura::LeAuxiliaresFiguras(void)
//{
//    return(LeTabelasDefault());
//}
// ---------------------------------------------------------------------------
bool __fastcall TBDfigura::LeEstilos(void)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	strESTILO *str;
	AnsiString descricao = "";
//	int estiloId = -1;
	int intEstiloIdPrev;
	int intEstiloId;
	int indiceAtributo;

	if (!bd_conn)
	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON
		return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "SELECT estilo.intEstiloId as EstiloId, estilo.strDescricao, "
			" ea.intAtributoGraficoId as AtributoGraficoId, ea.intEstiloAtributoId "
			" as EstiloAtributoId, ea.strValor as Valor "
			"from estilo  inner join estiloatributo ea on "
			"estilo.intEstiloId = ea.intEstiloId order by estilo.intEstiloId, intAtributoGraficoId;";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
//				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
//				SNPSTRLOGLINE(strlog);
				return (false);
			}
            // Recupera o resultado
			intEstiloIdPrev = -1;
			//cria estrutura de estilo
			str = new strESTILO;
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				intEstiloId = ptrQryDS->FieldByName("EstiloId")->AsInteger;

				// Protege contra equipamentos com número de enrolamentos inválido
				if (intEstiloIdPrev != intEstiloId)
				{
					intEstiloIdPrev = intEstiloId;
					if (str->atributos.Length != 0)
					{
						//insere estilo completo
						sucesso = estilos->InsereEstilo(str);
					}
					//limpa estrutura
					str->atributos.Length = 0;
				}
				//preenche a estrutura
				indiceAtributo = str->atributos.Length;
				str->id = intEstiloId;
				str->atributos.Length++;
				str->atributos[indiceAtributo].estiloId = intEstiloId;
				str->atributos[indiceAtributo].atributoGraficoId = ptrQryDS->FieldByName("AtributoGraficoId")->AsInteger;
				str->atributos[indiceAtributo].id = ptrQryDS->FieldByName("EstiloAtributoId")->AsInteger;
				str->atributos[indiceAtributo].valor = ptrQryDS->FieldByName("Valor")->AsString;
				//vixe, comofas?
				if (i == ptrQryDS->RecordCount - 1)
				{
                    if (str->atributos.Length != 0)
					{
						//insere estilo completo
						sucesso = estilos->InsereEstilo(str);
					}
				}
                ptrQryDS->Next();
			}
//			// Recupera o resultado
//			for (int i = 0; i < ptrQryDS->RecordCount; i++)
//			{
//				intEstiloIdPrev = ptrQryDS->FieldByName("EstiloId")->AsInteger;
//
//				// loop registros (ini)[
//				estilos->
//
//				str->id    	 	= ptrQryDS->FieldByName("intEstiloId")->AsInteger;
//				str->descricao 	= ptrQryDS->FieldByName("strDescricao")->AsString;
//				str->naBase = true;
//				//insere no map
//				mapaEstilo.insert(pair<AnsiString,strESTILO*>(descricao,str));
//				lisEstilo->Add(str);
//				ptrQryDS->Next();
//				// loop registros (fim)
//			}
//			//verifica se leu valores
//			if (mapaEstilo.empty())
//			{
//				sucesso = false;
//			}
			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
//			SNPSTRLOGLINE(strlog);
//			SNPSTRLOG(e.Message);
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
		delete str;
	}

	return (sucesso);
}
// ---------------------------------------------------------------------------
bool __fastcall TBDfigura::LeAuxiliaresFiguras(void)
{
    bool sucesso = true;

	if (estilos == NULL)
	{
        return false;
	}

//    if ((sucesso = PreencheDefaultEstilo())== false)
//	{
//		Loga("");
//		return false;
//	}
	//reseta a indicacao de salvo na base
	estilos->ResetAuxiliaresLidos();
    //le novamente
	if ((sucesso = LeDefaultTipoValorAtributoGrafico())== false)
	{
		sucesso = estilos->PreencheDefaultTipoValorAtributoGrafico();
	}
	if ((sucesso = LeDefaultAtributoGrafico())== false)
	{
		sucesso = estilos->PreencheDefaultAtributoGrafico();
	}
	if ((sucesso = LeDefaultForma())== false)
	{
		sucesso = estilos->PreencheDefaultForma();
	}
	if ((sucesso = LeDefaultFormaAtributo())== false)
	{
        sucesso = estilos->PreencheDefaultFormaAtributo();
	}

	return sucesso;
}
// ---------------------------------------------------------------------------
bool __fastcall TBDfigura::LeDefaultAtributoGrafico(void)
{
    bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	strATRIBUTO_GRAFICO *str;
	AnsiString descricao;

	if (!bd_conn)
	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON
		return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "SELECT * FROM AtributoGrafico ORDER BY intAtributoGraficoId;";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
//				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
//				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				descricao = ptrQryDS->FieldByName("strDescricao")->AsString;
				str = estilos->InsereAtributoGrafico(descricao);
				str->salvoNaBase = true;
				str->id			 = ptrQryDS->FieldByName("intAtributoGraficoId")->AsInteger;
				str->tipoValorAG = ptrQryDS->FieldByName("intTipoValorAtributoGraficoId")->AsInteger;
				str->descricao 	 = descricao;
				ptrQryDS->Next();
				// loop registros (fim)
			}
			//verifica se leu valores
			if (ptrQryDS->RecordCount == 0)
			{
                sucesso = false;
			}
			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
//			SNPSTRLOGLINE(strlog);
//			SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::LeDefaultFormaAtributo(void)
{
    bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	strFORMA_ATRIBUTO *str;
	AnsiString nome;

	if (!bd_conn)
	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON
		return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "SELECT * FROM FormaAtributo ORDER BY intFormaAtributoId;";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
//				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
//				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				nome = ptrQryDS->FieldByName("strNome")->AsString;
				str = estilos->InsereFormaAtributo(nome);
				str->salvoNaBase = true;
				str->id			 = ptrQryDS->FieldByName("intFormaAtributoId")->AsInteger;
				str->formaId     = ptrQryDS->FieldByName("intFormaId")->AsInteger;
				str->nome    	 = nome;
				str->tipoValor   = ptrQryDS->FieldByName("strTipoValor")->AsString;
				//insere no map
				ptrQryDS->Next();
				// loop registros (fim)
			}
			//verifica se leu valores
			if (ptrQryDS->RecordCount ==0)
			{
                sucesso = false;
			}
			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
//			SNPSTRLOGLINE(strlog);
//			SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::LeDefaultForma(void)
{
    bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	AnsiString descricao = "";
	int formaId = -1;
	strAUXILIAR_FIGURA *str;

	if (!bd_conn)
	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON
		return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "SELECT * FROM Forma ORDER BY intFormaId;";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
//				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
//				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				formaId    	 = ptrQryDS->FieldByName("intFormaId")->AsInteger;
				descricao  	 = ptrQryDS->FieldByName("strDescricao")->AsString;
				str = estilos->InsereForma(descricao,formaId);
				str->salvoNaBase = true;
				ptrQryDS->Next();
				// loop registros (fim)
			}
			//verifica se leu valores
			if (ptrQryDS->RecordCount == 0)
			{
                sucesso = false;
			}
			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
//			SNPSTRLOGLINE(strlog);
//			SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::LeDefaultTipoValorAtributoGrafico(void)
{
    bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
//	strFORMA_ATRIBUTO *str;
	AnsiString descricao = "";
	int tvagId = -1;
    strAUXILIAR_FIGURA *str;

	if (!bd_conn)
	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON
		return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "SELECT * FROM TipoValorAtributoGrafico ORDER BY intTipoValorAtributoGraficoId;";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
//				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
//				SNPSTRLOGLINE(strlog);
				return (false);
			}

			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				tvagId    	 = ptrQryDS->FieldByName("intTipoValorAtributoGraficoId")->AsInteger;
				descricao  	 = ptrQryDS->FieldByName("strDescricao")->AsString;
				str = estilos->InsereTipoValorAtributoGrafico(descricao, tvagId);
				str->salvoNaBase = true;
				ptrQryDS->Next();
				// loop registros (fim)
			}
			//verifica se leu valores
			if (ptrQryDS->RecordCount == 0)
			{
                sucesso = false;
			}
			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
//			SNPSTRLOGLINE(strlog);
//			SNPSTRLOG(e.Message);
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
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TBDfigura::LeFiguras(VTEstudo *estudo,bool estudo_novo, TList *lisREDE_SELE)
{
	// variáveis locais
	int valor_int;
	double valor_flo;
	AnsiString tipo;
	AnsiString filename;

	// Salva ponteiro
	this->estudo = estudo;
	if (figuras)
	{
        figuras->Estudo = estudo;
	}
	lisREDES = lisREDE_SELE;
	//Le informacao basica dos estilos
	if (!LeAuxiliaresFiguras())
		return (false);
	//Le estilos da base de dados
	if (!LeEstilos())
		return (false);
	//le os comentarios
	if (!ReadComentario(estudo_novo))
		{return false;}
	//le retangulos
	if (!ReadRetangulo(estudo_novo))
		{return false;}
	if (!ReadComentarioLink(estudo_novo))
		{return false;}
	//se for estudo novo
	if (estudo_novo)
	{
		figuras->RedefineIds();
	}

	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TBDfigura::Loga(AnsiString msg)
{
	// faz nada ainda
	if (plog)
	{
        plog->Add(msg);
	}
}
// ---------------------------------------------------------------------------
int __fastcall TBDfigura::NextComentarioId(void)
{
	return (++next_id.comentario);
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TBDfigura::ObtemCentroFigura(VTFigura *figura, strUTM &utm, strCOORD &esq)
{
	int formato, dim_x, dim_y;
	VTFiguraComentario *comentario;
	VTRetangulo *retangulo;

	try
	{ // guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;
		//verifica o tipo de figura
		switch (figura->TipoFigura)
		{
			case figCOMENTARIO:
				comentario = (VTFiguraComentario*) figura;
				// altera formato para fmtRETANGULO para obter dimensões
				formato = comentario->Formato;
				comentario->Formato = fmtRETANGULO;
				// determina centro e dimensão (no estilo retângulo)
				comentario->Retangulo->GetCentro(utm, esq);
				//	comentario->Retangulo->GetDimensao(dim_x, dim_y);
				// restaura estilo
				comentario->Formato = formato;
				break;
			case figRETANGULO:
				retangulo = (VTRetangulo*) figura;
				retangulo->GetCentro(utm, esq);
                break;
			default:
				;
		}
	}
	catch(Exception &e)
	{
		Loga(mErrorFunction + " " + e.Message);
//		sucesso = false;
	}
	catch(...)
	{
		Loga(mErrorFunction);
//		sucesso = false;
	}
}
///// ---------------------------------------------------------------------------
int __fastcall TBDfigura::QueryNextId(void)
{
	TADOQuery* ptrQryDS = NULL;
	AnsiString strSQL;
	int indice = 0;
	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "SELECT @@IDENTITY as NextID;";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
				Loga("Erro na execução da query: " + strSQL);
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_EXECINV return (IDINDEFINIDO);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
				Loga("ptrQryDS nulo " + strSQL);
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_EXECINV return (IDINDEFINIDO);
			}
			// Recupera o resultado
			if (ptrQryDS->RecordCount != 1)
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_EXECINV return (IDINDEFINIDO);
			}
			indice = ptrQryDS->FieldByName("NextID")->AsInteger;
			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
			// index.ndxAcaoId = IDINDEFINIDO;
		}
		catch (...)
		{

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

	return (indice);
}
//// ---------------------------------------------------------------------------
//int __fastcall TBDfigura::QueryNextId(AnsiString tabela, AnsiString coluna)
//{
//	TADOQuery* ptrQryDS = NULL;
//	AnsiString strSQL;
//	int indice = 0;
//	__try
//	{
//		try
//		{
//			// Monta a string de execução
//			strSQL = "SELECT IIf(IsNull(Max(" + coluna + ")),0,Max(" + coluna + "))+1 AS IdProx ";
//			strSQL += "FROM " + tabela + ";";
//			// Decodifica comando
//			bd_conn->ParseCmd(strSQL);
//			// Comando da consulta
//			if (!bd_conn->Execute(true))
//			{
//				Loga("Erro na execução da query: " + strSQL);
//				// SNPSTRLOGLINE(strlog);
//				// MSGERRBD_EXECINV return (IDINDEFINIDO);
//			}
//			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
//			{
//				Loga("ptrQryDS nulo " + strSQL);
//				// SNPSTRLOGLINE(strlog);
//				// MSGERRBD_EXECINV return (IDINDEFINIDO);
//			}
//			// Recupera o resultado
//			if (ptrQryDS->RecordCount != 1)
//			{
//				// SNPSTRLOGLINE(strlog);
//				// MSGERRBD_EXECINV return (IDINDEFINIDO);
//			}
//			indice = ptrQryDS->FieldByName("IdProx")->AsInteger;
//			ptrQryDS->Close();
//		}
//		catch (Exception& e)
//		{
//			// SNPSTRLOGLINE(strlog);
//			// SNPSTRLOG(e.Message);
//			// index.ndxAcaoId = IDINDEFINIDO;
//		}
//		catch (...)
//		{
//
//		}
//	}
//	__finally
//	{
//		// Colocado dentro do finally para garantir que será deletado, mesmo que
//		// haja um return no meio do código
//
//		// destrói objetos locais
//		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
//		{
//			delete ptrQryDS;
//		}
//	}
//
//	return (indice);
//}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
////https://sinapsisenergia.teamwork.com/#/tasks/16526463
bool __fastcall TBDfigura::ReadComentario(bool estudo_novo)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	int intFiguraIdPrev;
	int intFiguraId;
	strRETANGULO *strRet;
	VTFiguraComentario *comentario;

	if (!bd_conn)
	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON
		return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "SELECT figura.intEstudoId as EstudoId, figura.intAlternativaId as AlternativaId,"
				" figura.intObraId as ObraId, figura.intRedeId as RedeId, figura.intEstiloId as EstiloId,"
				" figura.intFormaId as FormaId, figura.intFiguraId as FiguraId,"
				" figura.bolVisible as Visible, figura.intEsqX as EsqX, figura.intEsqY as EsqY,"
				" figura.intUtmX as UtmX, figura.intUtmY as UtmY, texto.strTexto as Texto,"
				" dx.strValor as strDX, dy.strValor as strDY"
				" from ((figura inner join texto on texto.intFiguraId = figura.intFiguraId)"
				" left join valorFormaAtributo dx on (dx.intFiguraId =  figura.intFiguraId and dx.intFormaAtributoId =1))"
				" left join valorFormaAtributo dy on (dy.intFiguraId =  figura.intFiguraId and dy.intFormaAtributoId =2)";
			if (!estudo_novo)
			{
				strSQL +=" where figura.intEstudoId = " + IntToStr(estudo->Id);
			}
			else
			{
				strSQL +=" where figura.intEstudoId = 0";
			}
			strSQL +=" order by figura.intFiguraId, texto.intTextoId;";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
//				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
//				SNPSTRLOGLINE(strlog);
				return (false);
			}
			// Recupera o resultado
			intFiguraIdPrev = -1;
			//cria a estrutura
			strRet = new strRETANGULO;
			//cria estrutura de estilo
			int nreg = ptrQryDS->RecordCount;
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				intFiguraId = ptrQryDS->FieldByName("FiguraId")->AsInteger;
				// Protege contra equipamentos com número de enrolamentos inválido
				if (intFiguraIdPrev != intFiguraId)
				{
					intFiguraIdPrev = intFiguraId;
					//preenche com as infos do registro
					strRet->estudoId = ptrQryDS->FieldByName("EstudoId")->AsInteger;
					strRet->alternativaId = ptrQryDS->FieldByName("AlternativaId")->AsInteger;
					strRet->obraId = ptrQryDS->FieldByName("ObraId")->AsInteger;
					strRet->redeId = ptrQryDS->FieldByName("RedeId")->AsInteger;
					strRet->estiloId = ptrQryDS->FieldByName("EstiloId")->AsInteger;
					strRet->formaId = ptrQryDS->FieldByName("FormaId")->AsInteger;
					strRet->figuraId = intFiguraId;
					strRet->visible = (bool)ptrQryDS->FieldByName("Visible")->AsInteger;
					strRet->esqX = ptrQryDS->FieldByName("EsqX")->AsInteger;
					strRet->esqY = ptrQryDS->FieldByName("EsqY")->AsInteger;
					strRet->utmX = ptrQryDS->FieldByName("UtmX")->AsInteger;
					strRet->utmY = ptrQryDS->FieldByName("UtmY")->AsInteger;
					try
					{
						strRet->dx = ptrQryDS->FieldByName("strDX")->AsInteger;
					}
					catch(...)
					{
						strRet->dx = 5; //DIM_FLAG;
					}
					try
					{
						strRet->dy = ptrQryDS->FieldByName("strDY")->AsInteger;
					}
					catch(...)
					{
						strRet->dy = 5; //DIM_FLAG;
                    }
					//insere comentario
					comentario = figuras->InsereComentario(strRet);
					//acerta status
					if (comentario != NULL)
					{
						comentario->Status[sttEV_NOVO] = false;
						comentario->Retangulo->Status[sttEV_NOVO] = false;
						comentario->Status[sttEV_ALTERADO] = false;
						comentario->Retangulo->Status[sttEV_ALTERADO] = false;
                    }
				}
				if (comentario != NULL)
				{
					comentario->InsereTexto(ptrQryDS->FieldByName("Texto")->AsString);
                }
				ptrQryDS->Next();
			}
			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
//			SNPSTRLOGLINE(strlog);
//			SNPSTRLOG(e.Message);
			sucesso = false;
		}
		catch (...)
		{
//			SNPSTRLOGLINE(strlog);
//			SNPSTRLOG(e.Message);
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
		delete strRet;
	}

	return (sucesso);
}
////---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
//bool __fastcall TBDfigura::ReadComentario(void)
//   {
//   //variáveis locais
//   int           estudo_id, obra_id, rede_id, figura_id, formato, visible;
//   int           borda_cor, borda_espessura, borda_estilo;
//   int           fundo_cor, transparente;
//   int           font_size, font_color, font_bold, font_italic, font_underline, font_strikeout;
//   int           dim_x, dim_y;
//   AnsiString    font_name;
//   strUTM        utm;
//   strCOORD      esq;
//   VTFiguraComentario *comentario;
//
//   //lê atributos
//   if (! infoset->GetInfo("EstudoID",       estudo_id))       return(false);
//   if (! infoset->GetInfo("ObraID",         obra_id))         return(false);
//   if (! infoset->GetInfo("RedeID",         rede_id))         return(false);
//   if (! infoset->GetInfo("FiguraId",       figura_id))       return(false);
//   if (! infoset->GetInfo("Visible",        visible))         return(false);
//   if (! infoset->GetInfo("Formato",        formato))         return(false);
//   if (! infoset->GetInfo("FontName",       font_name))       return(false);
//   if (! infoset->GetInfo("FontSize",       font_size))       return(false);
//   if (! infoset->GetInfo("FontColor",      font_color))      return(false);
//   if (! infoset->GetInfo("FontBold",       font_bold))       font_bold      = 0;
//   if (! infoset->GetInfo("FontItalic",     font_italic))     font_italic    = 0;
//   if (! infoset->GetInfo("FontUnderline",  font_underline))  font_underline = 0;
//   if (! infoset->GetInfo("FontStrikeOut",  font_strikeout))  font_strikeout = 0;
//   if (! infoset->GetInfo("BordaEstilo",    borda_estilo))    return(false);
//   if (! infoset->GetInfo("BordaEspessura", borda_espessura)) return(false);
//   if (! infoset->GetInfo("BordaCor",       borda_cor))       return(false);
//   if (! infoset->GetInfo("FundoCor",       fundo_cor))       return(false);
//   if (! infoset->GetInfo("Transparente",   transparente))    return(false);
//   if (! infoset->GetInfo("EsqX",           esq.x))           return(false);
//   if (! infoset->GetInfo("EsqY",           esq.y))           return(false);
//   if (! infoset->GetInfo("UtmX",           utm.x))           return(false);
//   if (! infoset->GetInfo("UtmY",           utm.y))           return(false);
//   if (! infoset->GetInfo("DimX",           dim_x))           return(false);
//   if (! infoset->GetInfo("DimY",           dim_y))           return(false);
//   //Verifica se o comentário está no estudo atual
//   if((estudo->Id != estudo_id) && (estudo_id != 0))
//   {
//	   return(false);
//   }
//   //cria novo objeto Comentario
//   if ((comentario = figuras->InsereComentario()) == NULL) return(false);
//   //define formato fmtRETANGULO: importante na definição da dimensão
//   comentario->Formato                   = fmtRETANGULO;
//   //inicia atributos do Comentario
//   comentario->Status[sttFIGNOVA]        = false;
//   comentario->Rede                      = redes->ExisteRede(rede_id);
//   comentario->Obra                      = ExisteObra(obra_id);
//   comentario->Id                        = figura_id;
//   comentario->Visible                   = bool(visible);
//   if(lisREDES != NULL)
//   {
//	  if(lisREDES->IndexOf(comentario->Rede) >=0 )
//	  {
//		  comentario->Visible = true;
//	  }
//	  else
//	  {
//		  comentario->Visible = false;
//      }
//   }
//   //comentario->Formato                   = formato;
//   comentario->Font->Name                = font_name;
//   comentario->Font->Size                = font_size;
//   comentario->Font->Color               = TColor(font_color);
//   if (font_bold      == 1) comentario->Font->Style << fsBold;
//   if (font_italic    == 1) comentario->Font->Style << fsItalic;
//   if (font_underline == 1) comentario->Font->Style << fsUnderline;
//   if (font_strikeout == 1) comentario->Font->Style << fsStrikeOut;
//   comentario->Retangulo->Status[sttFIGNOVA] = false;
//   comentario->Retangulo->BordaEstilo    = borda_estilo;
//   comentario->Retangulo->BordaEspessura = borda_espessura;
//   comentario->Retangulo->BordaCor       = TColor(borda_cor);
//   comentario->Retangulo->FundoCor       = TColor(fundo_cor);
//   comentario->Retangulo->Transparente   = bool(transparente);
//   comentario->Retangulo->SetCentro(utm, esq);
//   comentario->Retangulo->SetDimensao(dim_x, dim_y);
//   //define o formato real
//   comentario->Formato                   = formato;
//   return(true);
//   }

//// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
//bool __fastcall TBDfigura::ReadComentarioLink(void)
//{
//	// variáveis locais
//	int figura_id, eqpto_tipo, eqpto_id;
//	int pos_com, pos_eqp;
//	int estilo, espessura, color;
//	VTFiguraComentario *comentario;
//	VTEqpto *eqpto;
//	VTLink *link;
//
//	// lê atributos
//	if (!infoset->GetInfo("FiguraId", figura_id))
//		return (false);
//	if (!infoset->GetInfo("EqptoTipo", eqpto_tipo))
//		return (false);
//	if (!infoset->GetInfo("EqptoId", eqpto_id))
//		return (false);
//	if (!infoset->GetInfo("PosCom", pos_com))
//		return (false);
//	if (!infoset->GetInfo("PosEqp", pos_eqp))
//		return (false);
//	// if (! infoset->GetInfo("Estilo",    estilo))       return(false);
//	// if (! infoset->GetInfo("Espessura", espessura))  return(false);
//	// if (! infoset->GetInfo("Color",     color))      return(false);
//	// determina Eqpto
//	if ((eqpto = redes->ExisteEqpto(eqpto_tipo, eqpto_id)) == NULL)
//		return (false);
//	// determina Comentario
//	if ((comentario = figuras->ExisteComentario(figura_id)) == NULL)
//		return (false);
//	// insere Link no Comentario
//	if ((link = comentario->InsereLink()) == NULL)
//		return (false);
//	link->Eqpto = eqpto;
//	link->PosEqp = pos_eqp;
//	link->PosCom = pos_com;
//	link->BordaTipo = psSolid; // estilo
//	link->Espessura = 1; // espessura
//	link->Color = clWhite; // TColor(color);
//	return (true);
//}// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TBDfigura::ReadComentarioLink(bool estudo_novo)
{
	// variáveis locais
	int figura_id, eqpto_tipo, eqpto_id, link_id;
	int pos_com, pos_eqp;
	int estiloId, tipo, espessura, color;
	AnsiString strlog;
	AnsiString strSQL;
	VTFiguraComentario *comentario;
	VTEqpto *eqpto;
	VTLink *link;
	VTEstilo *estilo = NULL;
	VTEstiloLink *estiloL = NULL;
	TADOQuery* ptrQryDS = NULL;
	bool sucesso = true;

    if (!bd_conn)
	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON
		return (false);
	}

	__try
	{
		try
		{
			//monta string com id dos comentarios
			;

			// Monta a string de execução
			strSQL = "select intFiguraId as FiguraId, intEstiloId as EstiloId, "
			" intLinkId as LinkId, intEqptoTipo as EqptoTipo, intEqptoId as EqptoId,"
			" fltPosCom as PosCom, fltPosEqp as PosEqp"
			" from link where intFiguraId in (" +IdsComentariosSQL() +
			") order by intFiguraId;";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
//				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
//				SNPSTRLOGLINE(strlog);
				return (false);
			}
			//cria estrutura de estilo
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
                // lê atributos
				figura_id = ptrQryDS->FieldByName("FiguraId")->AsInteger;
				eqpto_tipo = ptrQryDS->FieldByName("EqptoTipo")->AsInteger;
				eqpto_id = ptrQryDS->FieldByName("EqptoId")->AsInteger;
				pos_com = ptrQryDS->FieldByName("PosCom")->AsFloat;
				pos_eqp = ptrQryDS->FieldByName("PosEqp")->AsFloat;
				estiloId = ptrQryDS->FieldByName("EstiloId")->AsInteger;
				link_id = ptrQryDS->FieldByName("LinkId")->AsInteger;
				// determina Eqpto
				if ((eqpto = redes->ExisteEqpto(eqpto_tipo, eqpto_id)) == NULL)
					continue;
				// determina Comentario
				if ((comentario = figuras->ExisteComentario(figura_id)) == NULL)
					continue;
				// insere Link no Comentario
				if ((link = comentario->InsereLink(false)) == NULL)
					continue;
				link->Eqpto = eqpto;
				link->PosEqp = pos_eqp;
				link->PosCom = pos_com;
				link->Id = link_id;
				//encontra estilo
				if ((estilos != NULL) &&((estilo = estilos->ExisteEstilo(estiloId)) == NULL))
				{
					estilo = estilos->InsereEstilo(link,"");
				}
				if(estilo != NULL)
				{
					if (estilo->TipoEstilo == estiloLINK)
					{
						estiloL = (VTEstiloLink*)estilo;
						link->Estilo = estilo;
						link->BordaTipo = estiloL->BordaTipo; // estilo
						link->Espessura = estiloL->BordaEspessura; // espessura
						link->Color = estiloL->Bordacor; // TColor(color);
					}
				}
                // loop registros (ini)
				ptrQryDS->Next();
			}
			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
//			SNPSTRLOGLINE(strlog);
//			SNPSTRLOG(e.Message);
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

//// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
//bool __fastcall TBDfigura::ReadComentarioTexto(void)
//{
//	// variáveis locais
//	int figura_id;
//	AnsiString texto;
//	VTFiguraComentario *comentario;
//
//	// lê atributos
//	if (!infoset->GetInfo("FiguraId", figura_id))
//		return (false);
//	if (!infoset->GetInfo("Texto", texto))
//		return (false);
//	// determina Comentario
//	if ((comentario = figuras->ExisteComentario(figura_id)) == NULL)
//		return (false);
//	// insere texto no Comentario
//	comentario->InsereTexto(texto);
//
//	return (true);
//}
//// ---------------------------------------------------------------------------
//bool __fastcall TBDfigura::SalvaEstilos(void)
//{
//	bool sucessoTotal, sucesso;
//	strESTILO *estilo;
//
//	sucessoTotal = sucesso = true;
//	try
//	{ // guarda o nome da funcao
//		mErrorFunction = __PRETTY_FUNCTION__;
//		//percorre a lista de estilos e insere todos os novos
//		for(int ne = 0; ne < lisEstilo->Count; ne++)
//		{
//			estilo = (strESTILO*) lisEstilo->Items[ne];
//			sucesso = WriteEstilo(estilo);
//            sucessoTotal &= sucesso;
//		}
//	}
//    catch(Exception &e)
//	{
//		Loga(mErrorFunction + " " + e.Message);
//		sucessoTotal = false;
//	}
//	catch(...)
//	{
//		Loga(mErrorFunction);
//		sucessoTotal = false;
//	}
//
//	return sucessoTotal;
//}
// ---------------------------------------------------------------------------
//bool __fastcall TBDfigura::ReadRetangulo(void)
//{
//	// variáveis locais
//	int estudo_id, obra_id, figura_id, visible;
//	int borda_cor, borda_espessura, borda_estilo;
//	int fundo_cor, transparente;
//	int dim_x, dim_y;
//	strUTM utm;
//	strCOORD esq;
//	VTRetangulo *retangulo;
//
//	// lê atributos
//	if (!infoset->GetInfo("EstudoID", estudo_id))
//		return (false);
//	if (!infoset->GetInfo("ObraId", obra_id))
//		return (false);
//	if (!infoset->GetInfo("FiguraId", figura_id))
//		return (false);
//	if (!infoset->GetInfo("Visible", visible))
//		return (false);
//	if (!infoset->GetInfo("BordaEstilo", borda_estilo))
//		return (false);
//	if (!infoset->GetInfo("BordaEspessura", borda_espessura))
//		return (false);
//	if (!infoset->GetInfo("BordaCor", borda_cor))
//		return (false);
//	if (!infoset->GetInfo("FundoCor", fundo_cor))
//		return (false);
//	if (!infoset->GetInfo("Transparente", transparente))
//		return (false);
//	if (!infoset->GetInfo("EsqX", esq.x))
//		return (false);
//	if (!infoset->GetInfo("EsqY", esq.y))
//		return (false);
//	if (!infoset->GetInfo("UtmX", utm.x))
//		return (false);
//	if (!infoset->GetInfo("UtmY", utm.y))
//		return (false);
//	if (!infoset->GetInfo("DimX", dim_x))
//		return (false);
//	if (!infoset->GetInfo("DimY", dim_y))
//		return (false);
//	// Verifica se o comentário está no estudo atual
//	if ((estudo->Id != estudo_id) && (estudo_id != 0))
//	{
//		return (false);
//	}
//	// cria novo objeto Comentario
//	if ((retangulo = figuras->InsereRetangulo()) == NULL)
//		return (false);
//	// inicia atributos do Comentario
//	retangulo->Id = figura_id;
//	retangulo->Obra = ExisteObra(obra_id);
//	retangulo->Visible = bool(visible);
//	retangulo->BordaEstilo = borda_estilo;
//	retangulo->BordaEspessura = borda_espessura;
//	retangulo->BordaCor = TColor(borda_cor);
//	retangulo->FundoCor = TColor(fundo_cor);
//	retangulo->Transparente = bool(transparente);
//	retangulo->SetCentro(utm, esq);
//	retangulo->SetDimensao(dim_x, dim_y);
//	return (true);
//}// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TBDfigura::ReadRetangulo(bool estudo_novo)
{
	bool sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	int intFiguraIdPrev;
	int intFiguraId;
	strRETANGULO *strRet;
	VTRetangulo *retangulo;

	if (!bd_conn)
	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
//		SNPSTRLOGLINE(strlog);
//		MSGERRSYS_SESSIONCON
		return (false);
	}

	__try
	{
		try
		{
			// Monta a string de execução
			strSQL = "SELECT figura.intEstudoId as EstudoId, figura.intAlternativaId as AlternativaId,"
			" figura.intObraId as ObraId, figura.intRedeId as RedeId, figura.intEstiloId as EstiloId,"
			" figura.intFormaId as FormaId, figura.intFiguraId as FiguraId,"
			" figura.bolVisible as Visible, figura.intEsqX as EsqX, figura.intEsqY as EsqY,"
			" figura.intUtmX as UtmX, figura.intUtmY as UtmY, texto.strTexto as Texto,"
			" dx.strValor as strDX, dy.strValor as strDY"
			" from ((figura left join texto on texto.intFiguraId = figura.intFiguraId)"
			" left join valorFormaAtributo dx on (dx.intFiguraId =  figura.intFiguraId and dx.intFormaAtributoId =1))"
			" left join valorFormaAtributo dy on (dy.intFiguraId =  figura.intFiguraId and dy.intFormaAtributoId =2)"
			" where texto.intTextoId is null";
			if (!estudo_novo)
			{
				strSQL +=" And figura.intEstudoId = " + IntToStr(estudo->Id);
			}
			else
			{
				strSQL +=" And figura.intEstudoId = 0";
			}
			strSQL +=" order by figura.intFiguraId, texto.intTextoId;";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);

			// Comando da consulta
			if (!bd_conn->Execute(true))
			{
//				SNPSTRLOGLINE(strlog);
				return (false);
			}
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
			{
//				SNPSTRLOGLINE(strlog);
				return (false);
			}
            // Recupera o resultado
			intFiguraIdPrev = -1;
			//cria a estrutura
			strRet = new strRETANGULO;
			//cria estrutura de estilo
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				intFiguraId = ptrQryDS->FieldByName("FiguraId")->AsInteger;
				// Protege contra equipamentos com número de enrolamentos inválido
				if (intFiguraIdPrev != intFiguraId)
				{
					intFiguraIdPrev = intFiguraId;
					//preenche com as infos do registro
					strRet->estudoId = ptrQryDS->FieldByName("EstudoId")->AsInteger;
					strRet->alternativaId = ptrQryDS->FieldByName("AlternativaId")->AsInteger;
					strRet->obraId = ptrQryDS->FieldByName("ObraId")->AsInteger;
					strRet->redeId = ptrQryDS->FieldByName("RedeId")->AsInteger;
					strRet->estiloId = ptrQryDS->FieldByName("EstiloId")->AsInteger;
					strRet->formaId = ptrQryDS->FieldByName("FormaId")->AsInteger;
					strRet->figuraId = intFiguraId;
					strRet->visible = (bool)ptrQryDS->FieldByName("Visible")->AsInteger;
					strRet->esqX = ptrQryDS->FieldByName("EsqX")->AsInteger;
					strRet->esqY = ptrQryDS->FieldByName("EsqY")->AsInteger;
					strRet->utmX = ptrQryDS->FieldByName("UtmX")->AsInteger;
					strRet->utmY = ptrQryDS->FieldByName("UtmY")->AsInteger;
					strRet->dx = ptrQryDS->FieldByName("strDX")->AsInteger;
					strRet->dy = ptrQryDS->FieldByName("strDY")->AsInteger;
					//insere comentario
					retangulo = figuras->InsereRetangulo(strRet);
					if (retangulo != NULL)
					{
						retangulo->Status[sttEV_NOVO] = false;
						retangulo->Status[sttEV_NOVO] = false;
					}

				}
                ptrQryDS->Next();
			}
			ptrQryDS->Close();
		}
		catch (Exception& e)
		{
//			SNPSTRLOGLINE(strlog);
//			SNPSTRLOG(e.Message);
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
		delete strRet;
	}

	return (sucesso);
}
// ---------------------------------------------------------------------------
bool __fastcall	TBDfigura::RemoveEstudo(VTEstudo *estudo)
{
	bool sucesso = true;

	if (estudo == NULL)
		{return false;}
	if (estudo->Id < 0)
		{return false;}
	//1o deleta todos estilo atributos
	sucesso &= SPSNP_dlAllEstiloAtributoTextoEstudo(estudo->Id);
	sucesso &= SPSNP_dlAllEstiloAtributoLinkEstudo(estudo->Id);
	sucesso &= SPSNP_dlAllEstiloAtributoFiguraEstudo(estudo->Id);
	//2o deleta todos estilos
	sucesso &= SPSNP_dlAllEstiloEstudo(estudo->Id);
	//3o deleta todos textos
	sucesso &= SPSNP_dlAllTextoEstudo(estudo->Id);
	//4o deleta todos links
	sucesso &= SPSNP_dlAllLinkEstudo(estudo->Id);
	//5o deleta todos valor forma atributo
	sucesso &= SPSNP_dlAllValorFormaAtributoEstudo(estudo->Id);
	//6o deleta todas figuras
	sucesso &= SPSNP_dlAllFiguraEstudo(estudo->Id);

	return sucesso;
}
//// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
//bool __fastcall TBDfigura::ReadTexto(VTFigura *figura)
//{
//	bool sucesso = true;
//	AnsiString strSQL;
//	TADOQuery* ptrQryDS = NULL;
//	int intFiguraId = 0;
////	VTFiguraComentario *comentario;
//
//	if (!figura)
//	{
//		// SNPSTRLOGLINE(strlog);
//		// MSGERRSYS_SESSIONCON
//		return (false);
//	}
//	if (!bd_conn)
//	{
//		// SNPSTRLOGLINE(strlog);
//		// MSGERRSYS_SESSIONCON
//		return (false);
//	}
//	if (!bd_conn->getConnectedOK())
//	{
//		// SNPSTRLOGLINE(strlog);
//		// MSGERRSYS_SESSIONCON
//		return (false);
//	}
//	__try
//	{
//		try
//		{
//			intFiguraId = figura->Id;
//			// determina Comentario
//			// if ((comentario = figuras->ExisteComentario(intFiguraId)) == NULL)
//			// return (false);
//			// Monta a string de execução
//			strSQL = "SELECT * FROM TEXTO WHERE intFiguraId = p_FiguraId;";
//			// Decodifica comando
//			bd_conn->ParseCmd(strSQL);
//			// Determina os valores dos parâmetros
//			if (!bd_conn->SetParam("p_FiguraId", PARAMIN_INT(intFiguraId)))
//			{
//				// SNPSTRLOGLINE(strlog);
//				// MSGERRBD_PARAMINV
//				return (false);
//			}
//
//			// Comando da consulta
//			if (!bd_conn->Execute(true))
//			{
//				// SNPSTRLOGLINE(strlog);
//				return (false);
//			}
//			if ((ptrQryDS = bd_conn->getDataSet()) == NULL)
//			{
//				// SNPSTRLOGLINE(strlog);
//				return (false);
//			}
//
//			// Recupera o resultado
//			if (!SPSNP_rdTextoCursor(figura, ptrQryDS))
//			{
//				// SNPSTRLOGLINE(strlog);
//				sucesso = false;
//			}
//
//			ptrQryDS->Close();
//		}
//		catch (Exception& e)
//		{
//			// SNPSTRLOGLINE(strlog);
//			// SNPSTRLOG(e.Message);
//			sucesso = false;
//		}
//	}
//	__finally
//	{
//		// Colocado dentro do finally para garantir que será deletado, mesmo que
//		// haja um return no meio do código
//
//		// destrói objetos locais
//		if ((bd_conn->getMultiDataSet()) && (ptrQryDS))
//		{
//			delete ptrQryDS;
//		}
//	}
//
//	return (sucesso);
//}
// ---------------------------------------------------------------------------
bool __fastcall TBDfigura::SalvaAuxiliaresEstilo(void)
{
	bool sucessoTotal, sucesso;
	strAUXILIAR_FIGURA *aux;
	strFORMA_ATRIBUTO *formaAtributo;
	strATRIBUTO_GRAFICO *atributoGrafico;

	sucessoTotal = sucesso = true;
	try
	{ // guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;

		//lê novamente os valores, pois pode estar salvando em uma nova base com outros valores;
		sucesso = LeAuxiliaresFiguras();
		//percorre a lista de forma
		for(int nb = 0; nb < estilos->LisForma->Count; nb++)
		{
			aux = (strAUXILIAR_FIGURA*) estilos->LisForma->Items[nb];
			if (!aux->salvoNaBase)
			{
				sucesso = SPSNP_wrFormaIns(aux);
				if (sucesso)
					{aux->salvoNaBase = true;}
				sucessoTotal &= sucesso;
			}
		}
        //percorre a lista de formaAtributo
		for(int na = 0; na < estilos->LisFormaAtributo->Count; na++)
		{
			formaAtributo = (strFORMA_ATRIBUTO*) estilos->LisFormaAtributo->Items[na];
			if (!formaAtributo->salvoNaBase)
			{
				sucesso = SPSNP_wrFormaAtributoIns(formaAtributo);
				if (sucesso)
					{formaAtributo->salvoNaBase = true;}
				sucessoTotal &= sucesso;
			}
		}
		//percorre a lista de tipoValorAtributoGrafico
		for(int nc = 0; nc < estilos->LisTipoValorFormaAtributo->Count; nc++)
		{
			aux = (strAUXILIAR_FIGURA*) estilos->LisTipoValorFormaAtributo->Items[nc];
            if (!aux->salvoNaBase)
			{
				sucesso = SPSNP_wrTipoValorAtributGraficoIns(aux);
				if (sucesso)
					{aux->salvoNaBase = true;}
				sucessoTotal &= sucesso;
			}
		}
		//percorre a lista de atributoGrafico
		for(int nd = 0; nd < estilos->LisAtributoGrafico->Count; nd++)
		{
			atributoGrafico = (strATRIBUTO_GRAFICO*) estilos->LisAtributoGrafico->Items[nd];
            if (!atributoGrafico->salvoNaBase)
			{
				sucesso = SPSNP_wrAtributoGraficoIns(atributoGrafico);
                if (sucesso)
					{atributoGrafico->salvoNaBase = true;}
				sucessoTotal &= sucesso;
			}
		}

	}
    catch(Exception &e)
	{
		Loga(mErrorFunction + " " + e.Message);
		sucessoTotal = false;
	}
	catch(...)
	{
		Loga(mErrorFunction);
		sucessoTotal = false;
	}

	return sucessoTotal;
}
// ---------------------------------------------------------------------------
bool __fastcall TBDfigura::SalvaEstilos(void)
{
	bool sucessoTotal, sucesso;
	VTEstilo *estilo;

	sucessoTotal = sucesso = true;
	try
	{ // guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;
		//atualiza os estilos
        estilos->AtualizaEstilos();
		//percorre a lista de estilos
		for(int ne = 0; ne < estilos->LisEstilo->Count; ne++)
		{
			estilo = (VTEstilo*) estilos->LisEstilo->Items[ne];
			estilo->SalvaEstiloAtributos();
			sucesso = WriteEstilo(estilo);
			sucessoTotal &= sucesso;
		}
	}
    catch(Exception &e)
	{
		Loga(mErrorFunction + " " + e.Message);
		sucessoTotal = false;
	}
	catch(...)
	{
		Loga(mErrorFunction);
		sucessoTotal = false;
	}

	return sucessoTotal;
}
// ---------------------------------------------------------------------------
bool __fastcall TBDfigura::SalvaFiguras(VTEstudo *estudo)
{
	// variáveis locais
	AnsiString filename;
	this->estudo = estudo;
	bool sucesso = true;


	if (estudo->Novo)
	{	//redefine ids dos estilos
		estilos->RedefineIdsEstilo();
        figuras->RedefineIds();
	}
	//atualiza indices
//	AtualizaIndices();
	estilos->AssociaEstilos();
	if((sucesso = SalvaAuxiliaresEstilo()) != true)
	{
        return false;
    }
	if (SalvaEstilos() == true)
	{
		// grava Comentarios
		WriteComentario(estudo);
		// grava Retngulos
		WriteRetangulo(estudo);
	}
	else
	{
		sucesso = false;
	}



	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBDfigura::SPSNP_dlAllEstiloAtributo(int estilo_id)
{
    bool sucesso = true;
	AnsiString strSQL;

	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (estilo_id < 0)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	__try
	{
		try
		{
//			figura->Id = index.ndxFiguraId;
			strSQL = "DELETE FROM ";
			strSQL += "EstiloAtributo ";
			strSQL += "WHERE intEstiloId = p_EstiloId";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstiloId", PARAMIN_INT(estilo_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			bd_conn->Execute(false);
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_dlAllEstiloAtributoFiguraEstudo(int estudo_id)
{
    bool sucesso = true;
	AnsiString strSQL;

	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (estudo_id < 0)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	__try
	{
		try
		{
//			figura->Id = index.ndxFiguraId;
			strSQL = "Delete  DISTINCTROW EstiloAtributo.* from ";
			strSQL += "((EstiloAtributo ";
			strSQL += "inner join Estilo on estilo.intEstiloId = EstiloAtributo.intEstiloId) ";
			strSQL += "inner join Figura on Figura.intEstiloId = estilo.intEstiloId) ";
			strSQL += "WHERE Figura.intEstudoId = p_EstudoId";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			sucesso = bd_conn->Execute(false);
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_dlAllEstiloAtributoLinkEstudo(int estudo_id)
{
    bool sucesso = true;
	AnsiString strSQL;

	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (estudo_id < 0)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	__try
	{
		try
		{
//			figura->Id = index.ndxFiguraId;
			strSQL = "Delete  DISTINCTROW EstiloAtributo.* from ";
			strSQL += "(((EstiloAtributo ";
			strSQL += "inner join Estilo on estilo.intEstiloId = EstiloAtributo.intEstiloId) ";
			strSQL += "inner join Link on Link.intEstiloId = estilo.intEstiloId) ";
			strSQL += "inner join Figura on Figura.intEstiloId = Link.intEstiloId) ";
			strSQL += "WHERE Figura.intEstudoId = p_EstudoId";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			sucesso = bd_conn->Execute(false);
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_dlAllEstiloAtributoTextoEstudo(int estudo_id)
{
    bool sucesso = true;
	AnsiString strSQL;

	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (estudo_id < 0)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	__try
	{
		try
		{
//			figura->Id = index.ndxFiguraId;
			strSQL = "Delete  DISTINCTROW EstiloAtributo.* from ";
			strSQL += "(((EstiloAtributo ";
			strSQL += "inner join Estilo on estilo.intEstiloId = EstiloAtributo.intEstiloId) ";
			strSQL += "inner join Texto on Texto.intEstiloId = estilo.intEstiloId) ";
			strSQL += "inner join Figura on Figura.intEstiloId = Texto.intEstiloId) ";
			strSQL += "WHERE Figura.intEstudoId = p_EstudoId";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			sucesso = bd_conn->Execute(false);
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_dlAllEstiloEstudo(int estudo_id)
{
    bool sucesso = true;
	AnsiString strSQL;

	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (estudo_id < 0)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	__try
	{
		try
		{
//			figura->Id = index.ndxFiguraId;
			strSQL = "Delete  DISTINCTROW Estilo.* from ";
			strSQL += "(((((Estilo ";
			strSQL += "left join Texto on Texto.intEstiloId = estilo.intEstiloId) ";
			strSQL += "left join Link on Link.intEstiloId = estilo.intEstiloId) ";
			strSQL += "left join Figura fig1 on fig1.intEstiloId = estilo.intEstiloId) ";
			strSQL += "left join Figura fig2 on Texto.intFiguraId = fig2.intFiguraId) ";
			strSQL += "left join Figura fig3 on Link.intFiguraId = fig3.intFiguraId) ";
			strSQL += "where (fig1.intEstudoId = p_EstudoId or fig2.intEstudoId = p_EstudoId or fig3.intEstudoId = p_EstudoId)";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			sucesso = bd_conn->Execute(false);
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_dlAllFiguraEstudo(int estudo_id)
{
    bool sucesso = true;
	AnsiString strSQL;

	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (estudo_id < 0)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	__try
	{
		try
		{
//			figura->Id = index.ndxFiguraId;
			strSQL = "Delete  DISTINCTROW Figura.* from ";
			strSQL += "Figura ";
			strSQL += "where Figura.intEstudoId = p_EstudoId";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			sucesso = bd_conn->Execute(false);
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_dlAllLink(int figuraId)
{
	bool sucesso = true;
	AnsiString strSQL;

	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (figuraId < 0)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	__try
	{
		try
		{
			strSQL = "DELETE FROM ";
			strSQL += "Link ";
			strSQL += "WHERE intFiguraId = p_FiguraId;";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_FiguraId", PARAMIN_INT(figuraId)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			bd_conn->Execute(false);
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_dlAllLinkEstudo(int estudo_id)
{
	bool sucesso = true;
	AnsiString strSQL;

	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (estudo_id < 0)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	__try
	{
		try
		{
//			figura->Id = index.ndxFiguraId;
			strSQL = "Delete  DISTINCTROW Link.* from ";
			strSQL += "Link ";
			strSQL += "inner join Figura on Figura.intFiguraId = Link.intFiguraId ";
			strSQL += "where Figura.intEstudoId = p_EstudoId";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			sucesso = bd_conn->Execute(false);
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_dlAllTexto(int figuraId)
{
	bool sucesso = true;
	AnsiString strSQL;

	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (figuraId < 0)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	__try
	{
		try
		{
			strSQL = "DELETE FROM ";
			strSQL += "Texto ";
			strSQL += "WHERE intFiguraId = p_FiguraId;";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_FiguraId", PARAMIN_INT(figuraId)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			bd_conn->Execute(false);
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_dlAllTextoEstudo(int estudo_id)
{
    bool sucesso = true;
	AnsiString strSQL;

	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (estudo_id < 0)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	__try
	{
		try
		{
//			figura->Id = index.ndxFiguraId;
			strSQL = "Delete  DISTINCTROW Texto.* from ";
			strSQL += "Texto ";
			strSQL += "inner join Figura on Figura.intFiguraId = Texto.intFiguraId ";
			strSQL += "where Figura.intEstudoId = p_EstudoId";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			sucesso = bd_conn->Execute(false);
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_dlAllValorFormaAtributo(int figuraId)
{
    bool sucesso = true;
	AnsiString strSQL;

	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (figuraId < 0)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	__try
	{
		try
		{
			strSQL = "DELETE FROM ";
			strSQL += "ValorFormaAtributo ";
			strSQL += "WHERE intFiguraId = p_FiguraId";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_FiguraId", PARAMIN_INT(figuraId)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			bd_conn->Execute(false);
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_dlAllValorFormaAtributoEstudo(int estudo_id)
{
	bool sucesso = true;
	AnsiString strSQL;

	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (estudo_id < 0)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	__try
	{
		try
		{
//			figura->Id = index.ndxFiguraId;
			strSQL = "Delete  DISTINCTROW ValorFormaAtributo.* from ";
			strSQL += "ValorFormaAtributo ";
			strSQL += "inner join Figura on Figura.intFiguraId = ValorFormaAtributo.intFiguraId ";
			strSQL += "where Figura.intEstudoId = p_EstudoId";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			sucesso = bd_conn->Execute(false);
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_dlEstiloAtributo(int estiloAtributo_id)
{
    return false;
}
// ---------------------------------------------------------------------------
bool __fastcall TBDfigura::SPSNP_dlFigura(int figuraId)
{
	bool sucesso = true;
	AnsiString strSQL;

	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (figuraId < 0)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	__try
	{
		try
		{
			//deleta os texto vinculados à figura
			if ((sucesso &= SPSNP_dlAllTexto(figuraId))== false)
			{
				//Loga("");
				return (false);
			}
			//deleta os links vinculados à figura
			if ((sucesso &= SPSNP_dlAllLink(figuraId))== false)
			{
				//Loga("");
				return (false);
			}
			//deleta os valores de forma
			if ((sucesso &= SPSNP_dlAllValorFormaAtributo(figuraId))== false)
			{
				//Loga("");
				return (false);
			}
			//deleta a figura
			strSQL = "DELETE FROM ";
			strSQL += "Figura ";
			strSQL += "WHERE intFiguraId = p_FiguraId";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_FiguraId", PARAMIN_INT(figuraId)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			sucesso &= bd_conn->Execute(false);
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
//// ---------------------------------------------------------------------------
//bool __fastcall TBDfigura::SPSNP_dlLink(int linkId)
//{
//
//}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TBDfigura::SPSNP_rdTextoCursor(VTFigura *figura, TADOQuery* ptrQryDS)
{
	bool sucesso = true;
	AnsiString texto;
	VTFiguraComentario *comentario;

	if (!figura)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!ptrQryDS)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_PONTINV
		return (false);
	}

	__try
	{
		try
		{
			comentario = (VTFiguraComentario*) figura;
			/* TODO : impedir q entre nessa função uma figura q não seja um comentario */
			// Recupera o resultado
			for (int i = 0; i < ptrQryDS->RecordCount; i++)
			{
				// loop registros (ini)
				texto = ptrQryDS->FieldByName("strTexto")->AsString;
				// insere o texto
				comentario->InsereTexto(texto);
				ptrQryDS->Next();
				// loop registros (end)
			}
			// Esta função aqui não pode fechar o cursor
			// ptrQryDS->Close();
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_wrAtributoGraficoIns(strATRIBUTO_GRAFICO *str)
{
	bool sucesso = true;
	AnsiString strSQL;

    if (!str)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	__try
	{
		try
		{
			strSQL = "INSERT INTO ";
			strSQL += "AtributoGrafico (intTipoValorAtributoGraficoId, strDescricao) ";
			strSQL += "VALUES (p_TipoValorAtributoGraficoId, p_Descricao);";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_TipoValorAtributoGraficoId", PARAMIN_INT(str->tipoValorAG)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_Descricao", PARAMIN_STR(str->descricao)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			sucesso = bd_conn->Execute(false);
            if (sucesso)
			{
				str->id = QueryNextId();
//                index.ndxAtributoGraficoId++;
			}
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_wrEstiloAtributoIns(strESTILO_ATRIBUTO *str)
{
    bool sucesso = true;
	AnsiString strSQL;

	if (!str)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	__try
	{
		try
		{
			strSQL = "INSERT INTO ";
			strSQL += "EstiloAtributo (intAtributoGraficoId, intEstiloId, strValor) ";
			strSQL += "VALUES (p_AtributoGraficoId, p_EstiloId, p_Valor);";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_AtributoGraficoId", PARAMIN_INT(str->atributoGraficoId)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_EstiloId", PARAMIN_INT(str->estiloId)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_Valor", PARAMIN_STR(str->valor)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			sucesso = bd_conn->Execute(false);
			if (sucesso)
			{
				str->id = QueryNextId();
//                index.ndxEstiloAtributoId++;
			}
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_wrEstiloIns(VTEstilo *estilo, int force_id)
{
    bool sucesso = true;
	AnsiString strSQL;

	if (!estilo)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	__try
	{
		try
		{
			strSQL = "INSERT INTO ";
			strSQL += "Estilo (";
			if (force_id >= 0)
				{strSQL += "intEstiloId,";}
			strSQL += "strDescricao) ";
			strSQL += "VALUES (";
			if (force_id >= 0)
				{strSQL += "p_EstiloId,";}
			strSQL += "p_Descricao);";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (force_id >= 0)
			{
				if (!bd_conn->SetParam("p_EstiloId", PARAMIN_STR(force_id)))
				{
					// SNPSTRLOGLINE(strlog);
					// MSGERRBD_PARAMINV
					return (false);
				}
            }
			if (!bd_conn->SetParam("p_Descricao", PARAMIN_STR(estilo->Descricao)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			sucesso = bd_conn->Execute(false);
			if (sucesso)
			{
                estilo->Id = QueryNextId();
			}
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TBDfigura::SPSNP_wrFiguraIns(VTFigura *figura)
{
	bool sucesso = true;
	AnsiString strSQL;
	VTAlternativa *alternativa = NULL;
	int obra_id, rede_id, alternativa_id, estilo_id, forma_id, estudo_id;
	strUTM utm;
	strCOORD esq;

	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!figura)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	__try
	{
		try
		{
            // redefine ID
//			figura->Id = index.ndxFiguraId;
			//inicia variaveis
			obra_id = rede_id = alternativa_id = estilo_id = forma_id = 0;
			//se possui obra
			if (figura->Obra != NULL)
				{obra_id = figura->Obra->Id;}
			//se é comentario e possui rede
			if (figura->TipoFigura  == figCOMENTARIO)
			{
				if(((VTFiguraComentario*)figura)->Rede != NULL)
				{
					rede_id = ((VTFiguraComentario*)figura)->Rede->Id;
                }
			}
			//verifica o id do estudo
//			if (figura->EstudoId >=0)
//			{
//				estudo_id = figura->EstudoId;
//			}
//			else
			if(estudo != NULL)
			{
				estudo_id = estudo->Id;
			}
			else
			{
				estudo_id = 0;
			}
			//se possui alternativa
			alternativa = figura->Alternativa;
			if (alternativa != NULL)
				{alternativa_id = alternativa->Id;}
			//se possui estilo
			if(figura->Estilo != NULL)
				{estilo_id = figura->Estilo->Id;}
			//verifica o tipo
			forma_id = estilos->TipoFiguraToFormaId(figura);
			//obtem as coordenadas do centro da figura
			ObtemCentroFigura(figura,utm,esq);
			//acerta o id do link
//			figura->Id = index.ndxFiguraId;
			strSQL = "INSERT INTO ";
			strSQL += "Figura (intEstudoId, intAlternativaId, intObraId, intRedeId, ";
			strSQL += "intEstiloId, intFormaId, ";
//			strSQL += "intFiguraId, ";
			strSQL += "bolVisible, intEsqX, ";
			strSQL += "intEsqY, intUtmX, intUtmY) ";
			strSQL += "VALUES (p_EstudoId, p_AlternativaId, p_ObraId, p_RedeId, ";
			strSQL += "p_EstiloId, p_FormaId, ";
//			strSQL += "p_FiguraId, ";
			strSQL += "p_Visible, p_EsqX, ";
			strSQL += "p_EsqY, p_UtmX, p_UtmY);";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_AlternativaId", PARAMIN_INT(alternativa_id)))
					/* TODO : alterar o para o valor quando tiver o estilo */
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_ObraId", PARAMIN_INT(obra_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_RedeId", PARAMIN_INT(rede_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_EstiloId", PARAMIN_INT(estilo_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_FormaId", PARAMIN_INT(forma_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
//			if (!bd_conn->SetParam("p_FiguraId", PARAMIN_INT(figura->Id)))
//			{
//				// SNPSTRLOGLINE(strlog);
//				// MSGERRBD_PARAMINV
//				return (false);
//			}
			if (!bd_conn->SetParam("p_Visible", PARAMIN_INT((int)(figura->Visible ? 1 : 0))))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_EsqX", PARAMIN_INT(esq.x)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_EsqY", PARAMIN_INT(esq.y)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_UtmX", PARAMIN_INT(utm.x)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_UtmY", PARAMIN_INT(utm.y)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			sucesso = bd_conn->Execute(false);
			if (sucesso)
			{   
//				index.ndxFiguraId++;
                figura->Id = QueryNextId();
			}
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TBDfigura::SPSNP_wrFiguraUpd(VTFigura *figura)
{
	bool sucesso = true;
	AnsiString strSQL;
	VTAlternativa *alternativa = NULL;
	int obra_id, rede_id, alternativa_id, estilo_id, forma_id, estudo_id;
	strUTM utm;
	strCOORD esq;

	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!figura)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	__try
	{
		try
		{
            //inicia variaveis
			obra_id = rede_id = alternativa_id = estilo_id = forma_id = 0;
			//se possui obra
			if (figura->Obra != NULL)
				{obra_id = figura->Obra->Id;}
			//se é comentario e possui rede
			if (figura->TipoFigura  == figCOMENTARIO)
			{
				if(((VTFiguraComentario*)figura)->Rede != NULL)
				{
					rede_id = ((VTFiguraComentario*)figura)->Rede->Id;
				}
			}
			//se possui alternativa
			alternativa = figura->Alternativa;
			if (alternativa != NULL)
				{alternativa_id = alternativa->Id;}
			//se possui estilo
			if(figura->Estilo != NULL)
				{estilo_id = figura->Estilo->Id;}
			//verifica o id do estudo
//			if (figura->EstudoId >=0)
//			{
//				estudo_id = figura->EstudoId;
//			}
//			else
			if(estudo != NULL)
			{
				estudo_id = estudo->Id;
			}
			else
			{
				estudo_id = 0;
            }
			//verifica o tipo
			forma_id = estilos->TipoFiguraToFormaId(figura);
			//obtem as coordenadas do centro da figura
			ObtemCentroFigura(figura,utm,esq);
            //cria query
			strSQL = "UPDATE (SELECT * FROM Figura) sel SET";
			strSQL += " sel.intEstudoId = p_EstudoId";
			strSQL += ", sel.intAlternativaId = p_AlternativaId";
			strSQL += ", sel.intObraId = p_ObraId";
			strSQL += ", sel.intRedeId = p_RedeId";
			strSQL += ", sel.intEstiloId = p_EstiloId";
			strSQL += ", sel.intFormaId = p_FormaId";
			strSQL += ", sel.bolVisible = p_Visible";
			strSQL += ", sel.intEsqX = p_EsqX";
			strSQL += ", sel.intEsqY = p_EsqY";
			strSQL += ", sel.intUtmX = p_UtmX";
			strSQL += ", sel.intUtmY = p_UtmY";
			strSQL += " WHERE sel.intFiguraId = p_FiguraId;";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(estudo_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_AlternativaId", PARAMIN_INT(alternativa_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_ObraId", PARAMIN_INT(obra_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_RedeId", PARAMIN_INT(rede_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_EstiloId", PARAMIN_INT(estilo_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_FormaId", PARAMIN_INT(forma_id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_FiguraId", PARAMIN_INT(figura->Id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_Visible", PARAMIN_INT((int)(figura->Visible ? 1 : 0))))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_EsqX", PARAMIN_INT(esq.x)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_EsqY", PARAMIN_INT(esq.y)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_UtmX", PARAMIN_INT(utm.x)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_UtmY", PARAMIN_INT(utm.y)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			bd_conn->Execute(false);
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_wrFormaAtributoIns(strFORMA_ATRIBUTO *str)
{
    bool sucesso = true;
	AnsiString strSQL;

    if (!str)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	__try
	{
		try
		{
			strSQL = "INSERT INTO ";
			strSQL += "FormaAtributo (intFormaId, strNome, strTipoValor) ";
			strSQL += "VALUES (p_FormaId, p_Nome, p_TipoValor);";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_FormaId", PARAMIN_INT(str->formaId)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_Nome", PARAMIN_STR(str->nome)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_TipoValor", PARAMIN_STR(str->tipoValor)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			sucesso = bd_conn->Execute(false);
			if (sucesso)
			{   // redefine ID
				str->id = QueryNextId();
//				index.ndxFormaAtributoId++;
			}
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_wrFormaIns(strAUXILIAR_FIGURA *aux)
{
    bool sucesso = true;
	AnsiString strSQL;

	if (!aux)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	__try
	{
		try
		{
			strSQL = "INSERT INTO ";
			strSQL += "Forma (strDescricao) ";
			strSQL += "VALUES (p_Descricao);";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_Descricao", PARAMIN_STR(aux->descricao)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			sucesso = bd_conn->Execute(false);
			if (sucesso)
			{
				aux->id = QueryNextId();
//                index.ndxFormaId++;
			}

		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_wrLinkIns(VTFigura *figura, VTLink *link)
{
	bool sucesso = true;
	AnsiString strSQL;
	int estiloId = -1;

	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!figura)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	if (!link)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	__try
	{
		try
		{
			//acerta o id do link
//            link->Id = FNSNP_rdLinkIdProx();
			strSQL = "INSERT INTO ";
//			strSQL +=
//				"Link (intFiguraId, intEstiloId, intLinkId, intEqptoTipo, intEqptoId, fltPosCom, fltPosEqp) ";
			strSQL +=
				"Link (intFiguraId, intEstiloId,  intEqptoTipo, intEqptoId, fltPosCom, fltPosEqp) ";
//			strSQL +=
//				"VALUES (p_FiguraId, p_EstiloId, p_LinkId, p_EqptoTipo, p_EqptoId, p_PosCom, p_PosEqp);";
			strSQL +=
				"VALUES (p_FiguraId, p_EstiloId, p_EqptoTipo, p_EqptoId, p_PosCom, p_PosEqp);";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_FiguraId", PARAMIN_INT(figura->Id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (link->Estilo != NULL)
			{
				estiloId = link->Estilo->Id;
			}
			else
			{   //verifica se a figura tem um estilo
				if (figura->Estilo != NULL)
				{
					estiloId = figura->Estilo->Id;
				}
            }
			if (!bd_conn->SetParam("p_EstiloId", PARAMIN_INT(estiloId)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
//			if (!bd_conn->SetParam("p_LinkId", PARAMIN_INT(link->Id)))
//			{
//				// SNPSTRLOGLINE(strlog);
//				// MSGERRBD_PARAMINV
//				return (false);
//			}
			if (!bd_conn->SetParam("p_EqptoTipo", PARAMIN_INT(link->Eqpto->Tipo())))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_EqptoId", PARAMIN_INT(link->Eqpto->Id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_PosCom", PARAMIN_FLT(link->PosCom)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_PosEqp", PARAMIN_FLT(link->PosEqp)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			sucesso = bd_conn->Execute(false);
			if (sucesso)
			{   // redefine ID
				link->Id = QueryNextId();
//				index.ndxLinkId++;
			}
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_wrLinkUpd(VTFigura *figura, VTLink *link)
{
	bool sucesso = true;
	AnsiString strSQL;
	int estiloId = -1;

	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!figura)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	if (!link)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_ESTUDOINV
		return (false);
	}
	__try
	{
		try
		{
			strSQL = "UPDATE (SELECT * FROM Link) sel SET";
			strSQL += " sel.intFiguraId = p_FiguraId";
			strSQL += ", sel.intEstiloId = p_EstiloId";
			// strSQL += ", sel.intLinkId = p_LinkId";
			strSQL += ", sel.intEqptoTipo = p_EqptoTipo";
			strSQL += ", sel.intEqptoId = p_EqptoId";
			strSQL += ", sel.fltPosCom = p_PosCom";
			strSQL += ", sel.fltPosEqp = p_PosEqp";
			strSQL += " WHERE sel.intLinkId = p_LinkId;";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_FiguraId", PARAMIN_INT(figura->Id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			//guarda a info do estilo
			if (figura->Estilo != NULL)
			{
				estiloId = figura->Estilo->Id;
			}
			else
			{
                estiloId = -1;
            }
			if (!bd_conn->SetParam("p_EstiloId", PARAMIN_INT(estiloId)))
				/* TODO : alterar o para o valor quando tiver o estilo */
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_LinkId", PARAMIN_INT(link->Id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_EqptoTipo", PARAMIN_INT(link->Eqpto->Tipo())))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_EqptoId", PARAMIN_INT(link->Eqpto->Id)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_PosCom", PARAMIN_FLT(link->PosCom)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_PosEqp", PARAMIN_FLT(link->PosEqp)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			bd_conn->Execute(false);
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_wrTextoIns(strTEXTO *str)
{
    bool sucesso = true;
	AnsiString strSQL;

	if (!str)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	__try
	{
		try
		{
			strSQL = "INSERT INTO ";
			strSQL += "TEXTO (intFiguraId, intEstiloId, strTexto) ";
			strSQL += "VALUES (p_FiguraId, p_EstiloId, p_Texto);";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_FiguraId", PARAMIN_INT(str->figuraId)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_EstiloId", PARAMIN_INT(str->estiloId)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_Texto", PARAMIN_STR(str->texto)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			bd_conn->Execute(false);
			if (sucesso)
			{   // redefine ID
				str->id = QueryNextId();
//				index.ndxTextoId++;
			}
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_wrTipoValorAtributGraficoIns(strAUXILIAR_FIGURA *aux)
{
	bool sucesso = true;
	AnsiString strSQL;

	if (!aux)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	__try
	{
		try
		{
			strSQL = "INSERT INTO ";
			strSQL += "TipoValorAtributoGrafico (strDescricao) ";
			strSQL += "VALUES (p_Descricao);";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_Descricao", PARAMIN_STR(aux->descricao)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			sucesso = bd_conn->Execute(false);
			if (sucesso)
			{   // redefine ID
				aux->id = QueryNextId();
//				index.ndxTipoValorAtributoGraficoId++;
			}
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
bool __fastcall TBDfigura::SPSNP_wrValorFormaAtributoIns(strVALOR_FORMA_ATRIBUTO *str)
{
	bool sucesso = true;
	AnsiString strSQL;

	if (!str)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn)
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	if (!bd_conn->getConnectedOK())
	{
		// SNPSTRLOGLINE(strlog);
		// MSGERRSYS_SESSIONCON
		return (false);
	}
	__try
	{
		try
		{
			strSQL = "INSERT INTO ";
			strSQL += "ValorFormaAtributo (intFiguraId, intFormaAtributoId, strValor) ";
			strSQL += "VALUES (p_FiguraId,p_FormaAtributoId, p_Valor);";
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_FiguraId", PARAMIN_INT(str->figuraId)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_FormaAtributoId", PARAMIN_INT(str->formaAtributoId)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			if (!bd_conn->SetParam("p_Valor", PARAMIN_STR(str->valor)))
			{
				// SNPSTRLOGLINE(strlog);
				// MSGERRBD_PARAMINV
				return (false);
			}
			sucesso = bd_conn->Execute(false);
			if (sucesso)
			{   // redefine ID
				str->id = QueryNextId();
//				index.ndxValorFormaAtributoId++;
			}
		}
		catch (Exception& e)
		{
			// SNPSTRLOGLINE(strlog);
			// SNPSTRLOG(e.Message);
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
//// ---------------------------------------------------------------------------
//bool __fastcall TBDfigura::SPSNP_wrValorFormaAtributoUpd(strVALOR_FORMA_ATRIBUTO *str)
//{
//	bool sucesso = true;
//	AnsiString strSQL;
//
//	if (!str)
//	{
//		// SNPSTRLOGLINE(strlog);
//		// MSGERRSYS_SESSIONCON
//		return (false);
//	}
//	if (!bd_conn)
//	{
//		// SNPSTRLOGLINE(strlog);
//		// MSGERRSYS_SESSIONCON
//		return (false);
//	}
//	if (!bd_conn->getConnectedOK())
//	{
//		// SNPSTRLOGLINE(strlog);
//		// MSGERRSYS_SESSIONCON
//		return (false);
//	}
//	__try
//	{
//		try
//		{
//			strSQL = "UPDATE (SELECT * FROM ValorFormaAtributo) sel SET";
//			strSQL += ", sel.strValor = p_Valor";
//			strSQL += " WHERE sel.intFiguraId = p_FiguraId";
//			strSQL += " AND sel.intFormaAtributoId = p_FormaAtributoId";
//			strSQL += " AND sel.intValorFormaAtributo = p_ValorFormaAtributoId;";
//			bd_conn->ParseCmd(strSQL);
//			// Determina os valores dos parâmetros
//			if (!bd_conn->SetParam("p_FiguraId", PARAMIN_INT(str->figuraId)))
//			{
//				// SNPSTRLOGLINE(strlog);
//				// MSGERRBD_PARAMINV
//				return (false);
//			}
//			if (!bd_conn->SetParam("p_FormaAtributoId", PARAMIN_INT(str->formaAtributoId)))
//			{
//				// SNPSTRLOGLINE(strlog);
//				// MSGERRBD_PARAMINV
//				return (false);
//			}
//			if (!bd_conn->SetParam("p_ValorFormaAtributoId", PARAMIN_INT(str->id))) //como manter o id?
//			{
//				// SNPSTRLOGLINE(strlog);
//				// MSGERRBD_PARAMINV
//				return (false);
//			}
//			if (!bd_conn->SetParam("p_Valor", PARAMIN_STR(str->valor)))
//			{
//				// SNPSTRLOGLINE(strlog);
//				// MSGERRBD_PARAMINV
//				return (false);
//			}
//			sucesso = bd_conn->Execute(false);
//			if (sucesso)
//			{   // redefine ID
//				str->id = index.ndxValorFormaAtributoId;
//				index.ndxValorFormaAtributoId++;
//			}
//		}
//		catch (Exception& e)
//		{
//			// SNPSTRLOGLINE(strlog);
//			// SNPSTRLOG(e.Message);
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
//}
//// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
//bool __fastcall TBDfigura::WriteComentario(VTEstudo *estudo)
//{
//	// variáveis locais
//	int formato, dim_x, dim_y;
//	strUTM utm;
//	strCOORD esq;
//	TList *lisCOM, *lisLINK;
//	TStrings *texto;
//	VTFiguraComentario *comentario;
//	VTLink *link;
//	bool sucesso = true;
//
//	// loop para todos Comentarios
//	lisCOM = figuras->LisComentario();
//	for (int i = 0; i < lisCOM->Count; i++)
//	{
//		comentario = (VTFiguraComentario*)lisCOM->Items[i];
//		// redefine ID
//		comentario->Id = NextComentarioId();
//		// altera formato para fmtRETANGULO para obter dimensões
//		formato = comentario->Formato;
//		comentario->Formato = fmtRETANGULO;
//		comentario->Status[sttFIGNOVA] = false;
//		comentario->Status[sttFIGALTERADA] = false;
//		// determina centro e dimensão (no estilo retângulo)
//		comentario->Retangulo->GetCentro(utm, esq);
//		comentario->Retangulo->GetDimensao(dim_x, dim_y);
//		// restaura estilo
//		comentario->Formato = formato;
//		// insere linha com dados do Comentario
//		infoset->InsertNewLine();
//		infoset->AddInfo("Tipo", AnsiString("Comentario"));
//		infoset->AddInfo("EstudoID", estudo->Id);
//		if (comentario->Obra)
//			infoset->AddInfo("ObraID", comentario->Obra->Id);
//		else
//			infoset->AddInfo("ObraID", -1);
//		if (comentario->Rede != NULL)
//			infoset->AddInfo("RedeID", comentario->Rede->Id);
//		else
//			infoset->AddInfo("RedeID", -1);
//		infoset->AddInfo("FiguraId", comentario->Id);
//		infoset->AddInfo("Visible", int(comentario->Visible));
//		infoset->AddInfo("Formato", comentario->Formato);
//		infoset->AddInfo("FontName", comentario->Font->Name);
//		infoset->AddInfo("FontSize", comentario->Font->Size);
//		infoset->AddInfo("FontColor", comentario->Font->Color);
//		if (comentario->Font->Style.Contains(fsBold))
//			infoset->AddInfo("FontBold", 1);
//		if (comentario->Font->Style.Contains(fsItalic))
//			infoset->AddInfo("FontItalic", 1);
//		if (comentario->Font->Style.Contains(fsUnderline))
//			infoset->AddInfo("FontUnderline", 1);
//		if (comentario->Font->Style.Contains(fsStrikeOut))
//			infoset->AddInfo("FontStrikeOut", 1);
//		infoset->AddInfo("BordaEstilo", comentario->Retangulo->BordaEstilo);
//		infoset->AddInfo("BordaEspessura", comentario->Retangulo->BordaEspessura);
//		infoset->AddInfo("BordaCor", int(comentario->Retangulo->BordaCor));
//		infoset->AddInfo("FundoCor", int(comentario->Retangulo->FundoCor));
//		infoset->AddInfo("Transparente", int(comentario->Retangulo->Transparente));
//		infoset->AddInfo("EsqX", esq.x);
//		infoset->AddInfo("EsqY", esq.y);
//		infoset->AddInfo("UtmX", utm.x);
//		infoset->AddInfo("UtmY", utm.y);
//		infoset->AddInfo("DimX", dim_x);
//		infoset->AddInfo("DimY", dim_y);
//		// insere linhas com texto do Comentario
//		texto = comentario->Texto();
//		for (int j = 0; j < texto->Count; j++)
//		{
//			infoset->InsertNewLine();
//			infoset->AddInfo("Tipo", AnsiString("Texto"));
//			infoset->AddInfo("FiguraId", comentario->Id);
//			infoset->AddInfo("Texto", texto->Strings[j]);
//		}
//		// insere linhas com Links do Comentario
//		sucesso = WriteLink(comentario);
//		lisLINK = comentario->LisLink();
//		// for (int j = 0; j < lisLINK->Count; j++)
//		// {
//		// link = (VTLink*)lisLINK->Items[j];
//		// link->Status[sttFIGNOVA] = false;
//		// link->Status[sttFIGALTERADA] = false;
//		// infoset->InsertNewLine();
//		// infoset->AddInfo("Tipo", AnsiString("Link"));
//		// infoset->AddInfo("FiguraId", comentario->Id);
//		// infoset->AddInfo("EqptoTipo", link->Eqpto->Tipo());
//		// infoset->AddInfo("EqptoId", link->Eqpto->Id);
//		// infoset->AddInfo("PosCom", link->PosCom);
//		// infoset->AddInfo("PosEqp", link->PosEqp);
//		// infoset->AddInfo("Estilo", link->Estilo);
//		// infoset->AddInfo("Espessura", link->Espessura);
//		// infoset->AddInfo("Color", link->Color);
//		// }
//	}
//	return (true);
//}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TBDfigura::WriteComentario(VTEstudo *estudo)
{
	// variáveis locais
	int formato, dim_x, dim_y;
	strUTM utm;
	strCOORD esq;
	TList *lisCOM, *lisLINK;
//	TStrings *texto;
	VTFiguraComentario *comentario;
	VTLink *link;
	bool sucesso = true;

	// loop para todos Comentarios
	lisCOM = figuras->LisComentario();
	for (int i = 0; i < lisCOM->Count; i++)
	{
		comentario = (VTFiguraComentario*)lisCOM->Items[i];
		//verifica se foi excluido
		if (comentario->Status[sttEV_EXCLUIDO] == true)
		{
			//apaga figura das tabelas e memoria
			sucesso &= DeleteFigura(comentario);
		}
		else
		{
			//escreve na tabela figura
			sucesso &= WriteFigura(comentario);
			//insere os valores da forma
			sucesso &= WriteRetangulo(comentario);
			// insere linhas com texto do Comentario
			sucesso &= WriteTexto(comentario);
			// insere linhas com Links do Comentario
			sucesso &= WriteLink(comentario);
		}
	}
	return (sucesso);
}
//// ---------------------------------------------------------------------------
//bool __fastcall TBDfigura::WriteEstilo(strESTILO *str)
//{
//	bool sucesso;
//
//
//	sucesso = true;
//	try
//	{ 	// guarda o nome da funcao
//		mErrorFunction = __PRETTY_FUNCTION__;
//
//		if (!str->naBase) /* TODO : trocar isso pelo esquema do id negativo =( */
//		{
//			sucesso = SPSNP_wrEstiloIns(str->descricao,str->id);
//			//se consegiu inserir o estilo, insere os atributos
//			if (sucesso)
//			{
//				sucesso = WriteEstiloAtributo(str);
//			}
//		}
//		else
//		{
//			//limpa os atributos desse estilo e insere novaamente
//			sucesso = SPSNP_dlAllEstiloAtributo(str->id);
//			if (sucesso)
//			{
//				sucesso = WriteEstiloAtributo(str);
//			}
//		}
//	}
//    catch(Exception &e)
//	{
//		Loga(mErrorFunction + " " + e.Message);
//		sucesso = false;
//	}
//	catch(...)
//	{
//		Loga(mErrorFunction);
//		sucesso = false;
//	}
//
//	return sucesso;
//}// ---------------------------------------------------------------------------
bool __fastcall TBDfigura::WriteEstilo(VTEstilo *estilo)
{
	bool sucesso;


	sucesso = true;
	try
	{ 	// guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;

		if (estilo->Id < 0)
		{
			sucesso = SPSNP_wrEstiloIns(estilo, estilo->Id);
			if (sucesso)
			{
//				//atualiza o id
//				if (estilo->Id < 0)
//				{
//					estilo->Id = index.ndxEstiloId;
//					index.ndxEstiloId++;
//				}
				//insere atributos
				sucesso = WriteEstiloAtributo(estilo);
			}
		}
		else
		{
			//limpa os atributos desse estilo e insere novaamente
			sucesso = SPSNP_dlAllEstiloAtributo(estilo->Id);
			if (sucesso)
			{
				sucesso = WriteEstiloAtributo(estilo);
			}
		}
	}
    catch(Exception &e)
	{
		Loga(mErrorFunction + " " + e.Message);
		sucesso = false;
	}
	catch(...)
	{
		Loga(mErrorFunction);
		sucesso = false;
	}

	return sucesso;
}
//// ---------------------------------------------------------------------------
//bool __fastcall TBDfigura::WriteEstiloAtributo(strESTILO *str)
//{
//	bool sucessoTotal;
//	bool sucesso;
//	strESTILO_ATRIBUTO *atributo;
//
//	sucesso = sucessoTotal = true;
//	try
//	{ 	// guarda o nome da funcao
//		mErrorFunction = __PRETTY_FUNCTION__;
//		for (int na = 0; na < str->atributos.Length; na++)
//		{
//			atributo = &str->atributos[na];
//			sucesso = SPSNP_wrEstiloAtributoIns(atributo);
//			sucessoTotal &= sucesso;
//		}
//	}
//    catch(Exception &e)
//	{
//		Loga(mErrorFunction + " " + e.Message);
//		sucessoTotal = false;
//	}
//	catch(...)
//	{
//		Loga(mErrorFunction);
//		sucessoTotal = false;
//	}
//
//	return sucessoTotal;
//}// ---------------------------------------------------------------------------bool __fastcall TBDfigura::WriteEstiloAtributo(VTEstilo *estilo)
{
	bool sucessoTotal;
	bool sucesso;
	strESTILO_ATRIBUTO *atributo;

	sucesso = sucessoTotal = true;
	try
	{ 	// guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;

		for (int na = 0; na < estilo->LisEstiloAtributo->Count; na++)
		{
			atributo = (strESTILO_ATRIBUTO*)estilo->LisEstiloAtributo->Items[na];
            atributo->estiloId = estilo->Id;
			sucesso = SPSNP_wrEstiloAtributoIns(atributo);
			sucessoTotal &= sucesso;
		}
	}
    catch(Exception &e)
	{
		Loga(mErrorFunction + " " + e.Message);
		sucessoTotal = false;
	}
	catch(...)
	{
		Loga(mErrorFunction);
		sucessoTotal = false;
	}

	return sucessoTotal;
}
// ---------------------------------------------------------------------------
bool __fastcall TBDfigura::WriteFigura(VTFigura *figura)
{
	bool sucesso = true;
	TList *lisLINK;
	VTLink *link;

	try
	{ // guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;
		if (figura == NULL)
		{
			return false;
		}
		// se é nova insere
		if(figura->Status[sttEV_NOVO])
		{
			if ((sucesso = SPSNP_wrFiguraIns(figura)) == false)
			{
				Loga("");
			}
		}
		else
		{
			//se vai ser atualizada, deve apagar os textos e links vinculados
			//deleta links
			if ((sucesso = SPSNP_dlAllLink(figura->Id)) == false)
			{
				Loga("");
			}
			//deleta texto
			if ((sucesso = SPSNP_dlAllTexto(figura->Id)) == false)
			{
				Loga("");
			}
            //atualiza figura
			if ((sucesso = SPSNP_wrFiguraUpd(figura)) == false)
			{
				Loga("");
			}
		}
		// mantendo o q tinha no antigo (???)
		figura->Status[sttEV_NOVO] = false;
		figura->Status[sttEV_ALTERADO] = false;
	}
	catch(Exception &e)
	{
		Loga(mErrorFunction + " " + e.Message);
		sucesso = false;
	}
	catch(...)
	{
		Loga(mErrorFunction);
		sucesso = false;
	}

	return sucesso;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TBDfigura::WriteLink(VTFiguraComentario *comentario)
{
	bool sucesso = true;
	TList *lisLINK;
	VTLink *link;

	try
	{ // guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;
		if (comentario == NULL)
		{
			return false;
		}
		// guarda a lista de links
		lisLINK = comentario->LisLink();
		// insere linhas com Links do Comentario
		for (int j = 0; j < lisLINK->Count; j++)
		{
			link = (VTLink*)lisLINK->Items[j];
			//insere sempre pois quando a figura é atualizada se apaga todos links
            if ((sucesso = SPSNP_wrLinkIns(comentario, link)) == false)
			{
				Loga("");
			}
//			// se é nova insere
//			if (link->Status[sttEV_NOVO])
//			{
//				if ((sucesso = SPSNP_wrLinkIns(comentario, link)) == false)
//				{
//					Loga("");
//				}
//			}
//			else
//			{
//				if ((sucesso = SPSNP_wrLinkUpd(comentario, link)) == false)
//				{
//					Loga("");
//				}
//			}
			// mantendo o q tinha no antigo (???)
			link->Status[sttEV_NOVO] = false;
			link->Status[sttEV_ALTERADO] = false;
			// estilo link
			// infoset->AddInfo("Estilo", link->Estilo);
			// infoset->AddInfo("Espessura", link->Espessura);
			// infoset->AddInfo("Color", link->Color);
		}

	}
	catch (Exception &e)
	{
		mErrorMsg = e.Message;
	}
	catch (...)
	{

	}

	return sucesso;
}

//// ---------------------------------------------------------------------------
//bool __fastcall TBDfigura::WriteRetangulo(VTEstudo *estudo)
//{
//	// variáveis locais
//	int dim_x, dim_y;
//	strUTM utm;
//	strCOORD esq;
//	TList *lisRET;
//	VTRetangulo *retangulo;
//
//	// loop para todos Comentarios
//	lisRET = figuras->LisRetangulo();
//	for (int i = 0; i < lisRET->Count; i++)
//	{
//		retangulo = (VTRetangulo*)lisRET->Items[i];
//		// redefine ID
//		retangulo->Id = NextComentarioId();
//		retangulo->GetCentro(utm, esq);
//		retangulo->GetDimensao(dim_x, dim_y);
//		retangulo->Status[sttEV_NOVO] = false;
//		retangulo->Status[sttEV_ALTERADO] = false;
//		// insere linha com dados do Retangulo
//		infoset->InsertNewLine();
//		infoset->AddInfo("Tipo", AnsiString("Retangulo"));
//		infoset->AddInfo("EstudoID", estudo->Id);
//		if (retangulo->Obra)
//			infoset->AddInfo("ObraID", retangulo->Obra->Id);
//		else
//			infoset->AddInfo("ObraID", -1);
//		infoset->AddInfo("FiguraId", retangulo->Id);
//		infoset->AddInfo("Visible", int(retangulo->Visible));
//		infoset->AddInfo("BordaEstilo", retangulo->BordaEstilo);
//		infoset->AddInfo("BordaEspessura", retangulo->BordaEspessura);
//		infoset->AddInfo("BordaCor", int(retangulo->BordaCor));
//		infoset->AddInfo("FundoCor", int(retangulo->FundoCor));
//		infoset->AddInfo("Transparente", int(retangulo->Transparente));
//		infoset->AddInfo("EsqX", esq.x);
//		infoset->AddInfo("EsqY", esq.y);
//		infoset->AddInfo("UtmX", utm.x);
//		infoset->AddInfo("UtmY", utm.y);
//		infoset->AddInfo("DimX", dim_x);
//		infoset->AddInfo("DimY", dim_y);
//	}
//	return (true);
//}// ---------------------------------------------------------------------------bool __fastcall TBDfigura::WriteRetangulo(VTEstudo *estudo)
{
	// variáveis locais
	int dim_x, dim_y;
	strUTM utm;
	strCOORD esq;
	TList *lisRET;
	VTRetangulo *retangulo;
	bool sucesso = true;
	bool sucessoTotal = true;

	try
	{
        // guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;
		// loop para todos Comentarios
		lisRET = figuras->LisRetangulo();
		for (int i = 0; i < lisRET->Count; i++)
		{
			retangulo = (VTRetangulo*)lisRET->Items[i];
			sucesso =  WriteRetangulo(retangulo);
			sucessoTotal &= sucesso;
		}
	}
	catch (Exception &e)
	{
		mErrorMsg = e.Message;
		sucessoTotal= false;
	}
	catch (...)
	{
		sucessoTotal= false;
	}

	return (sucessoTotal);
}// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TBDfigura::WriteRetangulo(VTFiguraComentario *comentario)
{
	bool sucesso = true;
	VTRetangulo *retangulo = NULL;
	strVALOR_FORMA_ATRIBUTO * str = NULL;
	bool sucessoTotal = true;

	try
	{ // guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;
		if (comentario == NULL)
		{
			return false;
		}
		if (comentario->Retangulo == NULL)
		{
			return false;
		}
//		//verifica se deve atualizar ou inserir
//		if (comentario->Retangulo->Status[sttEV_NOVO] == false)
//		{
//		   sucesso = SPSNP_dlAllValorFormaAtributo(comentario->Id);
//		}
		// insere os valores do retangulo
        sucesso = SPSNP_dlAllValorFormaAtributo(comentario->Id);
		comentario->Retangulo->SalvaValorFormaAtributo(estilos);
		for (int ns = 0; ns < comentario->Retangulo->LisValorFormaAtributo->Count; ns++)
		{
			str = (strVALOR_FORMA_ATRIBUTO*) comentario->Retangulo->LisValorFormaAtributo->Items[ns];
			//atualiza o id da figura pra ser correspondente ao comentario e nao rao retangulo
            str->figuraId = comentario->Id;
			if ((sucesso = SPSNP_wrValorFormaAtributoIns(str)) == false)
			{
				Loga("");
			}
			sucessoTotal &= sucesso;
		}
		if (sucessoTotal)
		{
            comentario->Retangulo->Status[sttEV_NOVO] = false;
		}
	}
	catch (Exception &e)
	{
		mErrorMsg = e.Message;
		sucessoTotal= false;
	}
	catch (...)
	{
		sucessoTotal= false;
	}

	return sucessoTotal;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TBDfigura::WriteRetangulo(VTRetangulo *retangulo)
{
	bool sucesso = true;
	strVALOR_FORMA_ATRIBUTO * str = NULL;
	bool sucessoTotal = true;

	try
	{ // guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;
		if (retangulo == NULL)
		{
			return false;
		}
		//insere os valores da figura
		sucesso = WriteFigura(retangulo);
		if (!sucesso)
		{
            return false;
		}
		//verifica se deve atualizar ou inserir
//		if (retangulo->Status[sttEV_NOVO] == false)
//		{  //apaga os forma valor atributo
//		   sucesso = SPSNP_dlAllValorFormaAtributo(retangulo->Id);
//		}
		// insere os valores dos atributos do retangulo
		retangulo->SalvaValorFormaAtributo(estilos);
        sucesso = SPSNP_dlAllValorFormaAtributo(retangulo->Id);
		for (int ns = 0; ns < retangulo->LisValorFormaAtributo->Count; ns++)
		{
			str = (strVALOR_FORMA_ATRIBUTO*) retangulo->LisValorFormaAtributo->Items[ns];
			if ((sucesso = SPSNP_wrValorFormaAtributoIns(str)) == false)
			{
				Loga("");
			}
			sucessoTotal &= sucesso;
		}
		if (sucessoTotal)
		{
            retangulo->Status[sttEV_NOVO] = false;
		}
	}
	catch (Exception &e)
	{
		mErrorMsg = e.Message;
		sucessoTotal= false;
	}
	catch (...)
	{
		sucessoTotal= false;
	}

	return sucessoTotal;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TBDfigura::WriteTexto(VTFiguraComentario *comentario)
{
    bool sucesso = true;
	TStrings *texto;
	strTEXTO *str = NULL;

	try
	{ // guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;
		str = new strTEXTO;
		if (comentario == NULL)
		{
			return false;
		}

		// insere linhas com texto do Comentario
		texto = comentario->Texto();
		// insere linhas com Links do Comentario
		for (int j = 0; j < texto->Count; j++)
		{
			str->figuraId  = comentario->Id;
			if (comentario->Estilo != NULL)
			{
				str->estiloId = comentario->Estilo->Id;
			}
			else
			{
				str->estiloId = -1;
			}
			str->texto = texto->Strings[j];
			if ((sucesso = SPSNP_wrTextoIns(str)) == false)
			{
				Loga("");
			}
		}
	}
	catch (Exception &e)
	{
		mErrorMsg = e.Message;
	}
	catch (...)
	{

	}
	if (str)
	{
		delete str;
        str = NULL;
	}
	return sucesso;
}
// ---------------------------------------------------------------------------
// eof
