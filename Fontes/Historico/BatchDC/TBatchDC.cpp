// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TBatchDC.h"
#include <Fontes\AjusteDemanda\VTAjusta.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjos.h>
#include <Fontes\BDihm\VTAbre.h>
#include <Fontes\BDihm\VTSalva.h>
#include <Fontes\Diretorio\VTDir.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Edita\VTEdita.h>
#include <Fontes\Flow\VTFlow.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\ImportaMedicao\VTImportaMedicao.h>
#include <Fontes\ImportaMedicao\VTMedicoes.h>
#include <Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>
#include <Fontes\Obra\VTObra.h>
#include <Fontes\Radial\VTArvore.h>
#include <Fontes\Radial\VTRadial.h>
#include <Fontes\Radial\VTSubestacao.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTGerador.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <Fontes\Redutor\VTRedutor.h>
#include <Fontes\Topologia\VTTopologia.h>
#include "Arquivos\TArqMedicao.h"
#include "Arquivos\TArqRede.h"
#include "Arquivos\TArquivos.h"
#include "Densidade\VTDensidade.h"
#include <DLL_Inc\BDihm.h>
#include <DLL_Inc\AjusteDemanda.h>
#include <DLL_Inc\Edita.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\ImportaMedicao.h>
#include <DLL_Inc\ImportaRedeGis.h>
#include <DLL_Inc\Radial.h>
#include <DLL_Inc\Redutor.h>

// ---------------------------------------------------------------------------
VTBatchDC* __fastcall NewObjBatchDC(VTApl *apl_owner)
{
	return (new TBatchDC(apl_owner));
}

// ---------------------------------------------------------------------------
__fastcall TBatchDC::TBatchDC(VTApl *apl_owner)
{
	// cria objeto Apl próprio
	// apl               = NewObjApl(NULL, apl_owner);
	apl = apl_owner; // não cria apl próprio, mas usa o do TFormBatchDC
	apl->Add(DLL_NewObjRadial(apl));
	apl->Add(DLL_NewObjMedicoes(apl));
	apl->Add(DLL_NewObjSalva(NULL, apl));
	// cria demais objetos
	files = new TStringList();
	lisEQP = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TBatchDC::~TBatchDC(void)
{
	// destrói objetos
	// if (apl)   {delete apl;   apl   = NULL;}
	if (files)
	{
		delete files;
		files = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchDC::Executa(TListBox *LBoxLog)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString filename;
	TDateTime data;
	TArqRede *arqRede;
	TArquivos *arquivos = (TArquivos*)apl->GetObject(__classid(TArquivos));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	try
	{ // salva TStrings usado para apresentar mensagens indicando o progresso
		this->LBoxLog = LBoxLog;
		// calcula perda técnica para todos meses do período
		for (data = DataIni; data <= DataFim; data = IncMonth(data, 1))
		{ // reinicia Redes
			redes->Clear();
			// atualiza progresso
			Progresso(data);
			// determina ArqRede da data indicada
			if ((arqRede = arquivos->ExisteArqRede(data)) == NULL)
				return (false);
			// loop para todas as redes definidas em ArqRede
			for (int n = 0; n < arqRede->files->Count; n++)
			{ // lê as rede do arquivo
				filename = arqRede->files->Strings[n];
				Progresso(AnsiString(ExtractFileName(filename)));
				switch (arquivos->TipoArqRede)
				{
				case arqSINAP:
					sucesso = LeRedesBaseSinap(filename);
					break;
				case arqGIS:
					// verifica se é egrid
					if (arqRede->lisSub->Count > 0)
						sucesso = ImportaRedesSubestacaoGIS(data, arqRede);
					else
						sucesso = ImportaRedesArqGIS(data, filename);
					break;
				case arqLIS_GIS:
					sucesso = ImportaRedesListaArqGIS(data, filename);
					break;
				}
				if (!sucesso)
					continue;
				// verifica se é egrid (tudo ja foi feito)
				if (arqRede->lisSub->Count > 0)
					continue;
				// inicia objeto Radial
				// radial->Inicia(redes);
				// se necessário, executa ajuste de demanda DEPOIS de preparar a rede
				if (sucesso)
					sucesso = ExecutaAjusteDemanda(data);
				// executa cálculo de densidade, fazendo o ajuste de demandas em função
				// das opções do cálculo
				if (sucesso)
					if (!ExecutaCalculoDensidadeCarga(data))
						continue;
			}
		}
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}
/*
 //---------------------------------------------------------------------------
 bool __fastcall TBatchDC::ExecutaAjusteDemanda(TDateTime data)
 {
 //variáveis locais
 bool     sucesso;
 bool     med_neg_enabled = false;
 VTAjusta *ajusta_demanda;
 VTAjusta *ajusta = (VTAjusta*)apl->GetObject(__classid(VTAjusta));
 VTEdita  *edita  = (VTEdita*)apl->GetObject(__classid(VTEdita));
 VTObra   *obra, *obra_original;

 try{//verifica se o Ajuste de Demanda está habilitado
 if (! AjusteDemandaEnabled) return(true);
 //salva Obra originalmente associado a Edita
 obra_original = edita->Obra;
 //cria objeto Obra e associa com objeto Edita
 obra          = DLL_NewObjObra(apl);
 edita->Obra   = obra;
 //inicializa
 medicoes->Clear();
 //insere disjuntores ficticios (se necessário)
 medicoes->InsereDisjuntoresFicticios();
 //lê arquivo de medição
 if (! ImportaMedicao(data)) return(false);
 //calcula fluxo inicial
 sucesso = ajusta->CalculaFluxo();
 //atualiza valores estimados dos medidores
 medicoes->AtualizaMedidores();
 //inicia processo, validando medições e dados
 if(! medicoes->Valida(ajusta->LimitaAjuste, ajusta->AjusteMaximo)) return(false);
 //atualiza progresso
 Progresso(AnsiString("Executando ajuste de demanda"));
 //NÂO verifica se há Trafos nas redes: sempre haverá
 //executa ajuste da demanda
 if(sucesso) sucesso = ajusta->Executa(med_neg_enabled);
 //restaura cargas de redes que não foram ajustadas
 ajusta->RestauraCargasRedesNaoAjustadas();
 //desfaz todas ações do edita
 edita->UndoCompleto();
 edita->Clear();
 //restaura obra original do edita
 edita->Obra = obra_original;
 //destrói objeto
 delete obra;
 }catch(Exception &e)
 {
 sucesso = false;
 }
 return(sucesso);
 }
 */

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16436590
bool __fastcall TBatchDC::ExecutaAjusteDemanda(TDateTime data)
{
	// variáveis locais
	bool sucesso = true;
	bool med_neg_enabled = false;
	VTAjusta *ajusta_demanda = (VTAjusta*)apl->GetObject(__classid(VTAjusta));
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));

	try
	{ // verifica se o Ajuste de Demanda está habilitado
		if (!AjusteDemandaEnabled)
			return (true);
		// atualiza progresso
		Progresso(AnsiString("> Executando ajuste de demanda"));
		// verifica se há geradores
		med_neg_enabled = ExisteGeracao();
		// desabilita janela de progresso do Ajuste
		ajusta_demanda->ProgressoEnabled = false;
		// limpa edita para fazer undo completo no final sem interferir na rede
		edita->Clear();
		//primeiro fluxo
		ajusta_demanda->CalculaFluxo(false);
		// inicia ajuste
		if (!ajusta_demanda->IniciaDados(NULL))
			return (true);
		// lê arquivo de medição
		if (ImportaMedicao(data))
		{ // verifica se foram criados medidores para rede
			if (medicoes->LisMedidor()->Count > 0)
			{ // atualiza valores estimados dos medidores
				medicoes->AtualizaMedidores();
				// inicia processo, validando medições e dados
				if (medicoes->Valida(ajusta_demanda->LimitaAjuste, ajusta_demanda->AjusteMaximo))
				{ // executa ajuste da demanda
					sucesso = ajusta_demanda->Executa(med_neg_enabled);
					//verifica se foi ajustado mesmo
                    if (ajusta_demanda->Diagnostico.IsEmpty())
						ajusta_demanda->Diagnostico = "OK";
					Progresso(AnsiString("> Diagnóstico do ajuste: " + ajusta_demanda->Diagnostico));
					// finaliza ajuste
					ajusta_demanda->Finaliza();
					// remove os medidores inseridos
					edita->UndoCompleto();
				}
				else
				{ // atualiza progresso
					Progresso(AnsiString("> Não há medições válidas para as redes selecionadas"));
				}
			}
			else
			{ // atualiza progresso
				Progresso(AnsiString("> Não há medições para as redes selecionadas"));
			}
		}
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchDC::ExisteGeracao(void)
{
	// variáveis locais
	VTGerador *gerador;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	bool existe_gerador = false;
	TList *lisGER = new TList();

	// verifica se há geradores PV ou PQ na rede
	redes->LisEqpto(lisGER, eqptoGERADOR);
	for (int ng = 0; ng < lisGER->Count; ng++)
	{
		gerador = (VTGerador*)lisGER->Items[ng];
		if (gerador->tiposup == supPV)
			existe_gerador = true;
		if (gerador->tiposup == supPQ)
			existe_gerador = true;
	}
	// destroi lista
	delete lisGER;

	return (existe_gerador);
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchDC::ExecutaCalculoDensidadeCarga(TDateTime data)
{
	// variáveis locais
	bool sucesso = true; // false; //DVK 2016.04.06
	VTArvore *arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
	VTDensidade *densidade;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTSalva *salva = (VTSalva*)apl->GetObject(__classid(VTSalva));

	try
	{ // atualiza progresso
		Progresso(AnsiString("> Calculando densidade de carga"));
		// cria objeto Arvore, caso ele ainda não exista
		if (!arvore)
			apl->Add(arvore = DLL_NewObjArvore(apl));
		// cria objetos p/ densidade de carga
		densidade = NewObjDensidade(apl);
		// executa cálculo de densidade de carga
		if (sucesso)
			sucesso = densidade->Executa(data);
		// grava perdas na base de dados
		if (sucesso)
		{
			// arvore             = (VTArvore*)apl->GetObject(__classid(VTArvore));
			arvore->Data = data;
			arvore->TipoArvore = arvoreDENSIDADE;
			// atualiza progresso
			Progresso(AnsiString("> Salvando densidade de carga na base de histórico"));
			sucesso = salva->SalvaHistoricoDensidadeCarga(arvore);
		}
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchDC::ExisteMaeSelecionada(VTSub* sub, TList* lisSUB)
{
	// variáveis locais
	VTSub *sub_mae;

	for(int n = 0; n < lisSUB->Count; n++)
	{
		sub_mae = (VTSub*)lisSUB->Items[n];
		if(sub_mae->SubFilha->IndexOf(sub) >= 0)
			return true;
    }
	return false;
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchDC::ExisteTrafoNaRede(void)
{
	// variáveis locais
	VTTrafo *trafo;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// obtém lista de Trafos da Rede
	lisEQP->Clear();
	redes->LisLigacao(lisEQP, eqptoTRAFO);
	for (int n = 0; n < lisEQP->Count; n++)
	{
		trafo = (VTTrafo*)lisEQP->Items[n];
		if (trafo->rede->TipoRede->Segmento != redeETD)
			return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchDC::ImportaMedicao(TDateTime data)
{
	// variáveis locais
	bool sucesso;
	VTImportaMedicao *importa_medicao =
		(VTImportaMedicao*)apl->GetObject(__classid(VTImportaMedicao));
	TArqMedicao *arq_medicao;
	TArquivos *arquivos = (TArquivos*)apl->GetObject(__classid(TArquivos));

	// verifica se o Ajuste de Demanda está habilitado
	if (!AjusteDemandaEnabled)
		return (true);
	// determina ArqMedicao da data indicada
	if ((arq_medicao = arquivos->ExisteArqMedicao(data)) == NULL)
		return (false);
	try
	{ // desabilita Log da importação da medição
		importa_medicao->LogEnabled = false;
		// importa lista de medições no arquivo
		sucesso = importa_medicao->LeArquivoCurvaPadrao(arq_medicao->files);
		// if(sucesso) medicoes->AtualizaMedidores();
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

/*
 // ---------------------------------------------------------------------------
 bool __fastcall TBatchDC::ImportaRedesArqGIS(TDateTime data, TArqRede *arqRede)
 {
 // variáveis locais
 AnsiString filename;
 bool sucesso = false;
 strOPImporta opcao;
 VTImportaRedeGIS *ImportaRedeGIS;
 int destino = 0;

 // proteção
 if (arqRede == NULL)
 return false;
 if (arqRede->files->Count == 0)
 return false;
 filename = arqRede->files->Strings[0];
 // verifica se existe o arquivo
 if (!FileExists(filename))
 return (false);
 // define opções p/ importação
 opcao = PD.opcao;
 // cria objeto ImportaRedeGIS
 if ((ImportaRedeGIS = DLL_NewObjImportaRedeGIS(NULL, apl)) == NULL)
 return (false);
 // verifica se é egrid
 if (ImportaRedeGIS->ArquivoEgrid())
 {
 ImportaRedeGIS->ImportaDadosEmpresa(filename);
 ImportaRedeGIS->InsereSubestacao(arqRede->lisSub);
 // importa na memoria (destino = 0)
 sucesso = ImportaRedeGIS->ImportaEgrid(&opcao, destino);
 }
 if (sucesso)
 sucesso = ExecutaAjusteDemanda(data);
 // executa densidade de carga
 if (sucesso)
 {
 sucesso = ExecutaCalculoDensidadeCarga(data);
 }
 // destrói objeto
 delete ImportaRedeGIS;
 return (sucesso);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TBatchDC::ImportaRedesArqGIS(TDateTime data, AnsiString filename)
{
	// variáveis locais
	bool sucesso = false;
	strOPImporta opcao;
	VTImportaRedeGIS *ImportaRedeGIS;

	// verifica se existe o arquivo
	if (!FileExists(filename))
		return (false);
	// define opções p/ importação
	// opcao.date               = data;
	// opcao.sub.topologia      = opSUB_GIS;
	// opcao.pri.consA4         = opENERGIA;
	// opcao.fator_carga        = 0.6;
	// opcao.sec.topologia      = opSEC_GIS;//opSEC_CARGA;
	// opcao.valida_fase        = true;
	// opcao.corrige_fase       = true;
	// opcao.corrige_perda_fe   = true;
	// opcao.valida_eqpto       = true;
	// opcao.gerarTXT           = false;
	// opcao.ihm.log            = false;
	// opcao.ihm.progresso      = true;
	opcao = PD.opcao;
	// cria objeto ImportaRedeGIS
	if ((ImportaRedeGIS = DLL_NewObjImportaRedeGIS(NULL, apl)) == NULL)
		return (false);
	// inicia files com o arquivo com lista de arquivos
	files->Clear();
	files->Add(filename);
	// executa importação da rede
	sucesso = ImportaRedeGIS->ImportaLisArqPrimario(files, &opcao);
	// destrói objeto Conversor
	delete ImportaRedeGIS;
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchDC::ImportaRedesListaArqGIS(TDateTime data, AnsiString filename)
{
	// variáveis locais
	bool sucesso;
	strOPImporta opcao;
	VTImportaRedeGIS *ImportaRedeGIS;

	// verifica se existe o arquivo
	if (!FileExists(filename))
		return (false);
	// define opções p/ importação
	// opcao.date               = data;
	// opcao.sub.topologia      = opSUB_GIS;
	// opcao.pri.consA4         = opENERGIA;
	// opcao.fator_carga        = 0.6;
	// opcao.sec.topologia      = opSEC_GIS;//opSEC_CARGA;
	// opcao.valida_fase        = true;
	// opcao.corrige_fase       = true;
	// opcao.corrige_perda_fe   = true;
	// opcao.valida_eqpto       = true;
	// opcao.gerarTXT           = false;
	// opcao.ihm.log            = false;
	// opcao.ihm.progresso      = true;
	opcao = PD.opcao;
	// cria objeto ImportaRedeGIS
	if ((ImportaRedeGIS = DLL_NewObjImportaRedeGIS(NULL, apl)) == NULL)
		return (false);
	// inicia files com o arquivo com lista de arquivos
	files->Clear();
	files->Add(filename);
	// executa importação da rede
	sucesso = ImportaRedeGIS->ImportaLisArqSubestacao(files, &opcao);
	// destrói objeto Conversor
	delete ImportaRedeGIS;
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchDC::ImportaRedesSubestacaoGIS(TDateTime data, TArqRede *arqRede)
{
	// variáveis locais
	AnsiString filename;
	bool sucesso = false;
	strOPImporta opcao;
	VTImportaRedeGIS *ImportaRedeGIS;
	VTSub *sub;
	int destino = 0;
	TList *lisTMP;
	VTArranjos *arranjos;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

	// proteção
	if (arqRede == NULL)
		return false;
	if (arqRede->files->Count == 0)
		return false;
	filename = arqRede->files->Strings[0];
	// verifica se existe o arquivo
	if (!FileExists(filename))
		return (false);
	// define opções p/ importação
	opcao = PD.opcao;
	// cria lista
	lisTMP = new TList();
    // filtra redes a serem importadas
//	for (int n = arqRede->lisSub->Count - 1; n >= 0; n--)
//	{
//		sub = (VTSub*)arqRede->lisSub->Items[n];
//		if (sub->SubFilha->Count > 0)
//		{ // retira filhas da lista
//			RetiraLis1DeLis2(sub->SubFilha, arqRede->lisSub);
//		}
//	}
    lisTMP->Clear();
	for (int n = arqRede->lisSub->Count - 1; n >= 0; n--)
	{
		sub = (VTSub*)arqRede->lisSub->Items[n];
		if (sub->SubFilha->Count > 0)
		{ // insere sub mãe na lista
			lisTMP->Add(sub);
		}
		else
		{//verifica se a sub mãe foi selecionada (caso tenha sido selecionada, esta filha já vai para a lista)
			if(! ExisteMaeSelecionada(sub, arqRede->lisSub))
			{// insere sub mãe na lista
				lisTMP->Add(sub);
			}
		}
	}
	for (int n = 0; n < arqRede->lisSub->Count; n++)
	{
		lisTMP->Clear();
		sub = (VTSub*)arqRede->lisSub->Items[n];
		// insere sub e suas filhas, se houverem
		lisTMP->Add(sub);
		if (sub->SubFilha->Count > 0)
		{
			CopiaTList(sub->SubFilha, lisTMP);
		}
		// atualiza LBoxLog
		LBoxLog->Items->Add(sub->CodSub);
		// cria objeto ImportaRedeGIS
		if ((ImportaRedeGIS = DLL_NewObjImportaRedeGIS(NULL, apl)) == NULL)
		{
			sucesso = false;
			break;
		}
		// verifica se é egrid
		if (!ImportaRedeGIS->ArquivoEgrid())
		{
			sucesso = false;
			break;
		}
		// limpa arranjos
		arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
		arranjos->Clear();
		// le dados da empresa e insere subestações c/filhas selecionadas
		ImportaRedeGIS->ImportaDadosEmpresa(filename);
		ImportaRedeGIS->InsereSubestacao(lisTMP);
		// importa na memoria (destino = 0)
		sucesso = ImportaRedeGIS->ImportaEgrid(&opcao, destino);
        if(topologia) topologia->Reconfigura(redes);
		// se necessário, executa ajuste de demanda DEPOIS de preparar a rede
		if (sucesso)
			sucesso = ExecutaAjusteDemanda(data);
		// salva rede na base de dados
		if (sucesso)
		{ // executa cálculo de densidade, fazendo o ajuste de demandas em função das opções do cálculo
			if (!ExecutaCalculoDensidadeCarga(data))
				continue;
		}
		// destroi objeto
		delete ImportaRedeGIS;
	}
	// destroi lista
	delete lisTMP;

	return sucesso;
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchDC::LeRedesBaseSinap(AnsiString filename)
{
	// variáveis locais
	bool sucesso;
	VTAbre *abre;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	try
	{ // atualiza progresso
		Progresso(AnsiString("Lendo rede da base"));
		// atualiza aruivo da rede em VTPath
		path->ArqRede = filename;
		// cria objeto VTAbre
		abre = DLL_NewObjAbre(NULL, apl);
		// desabilita progresso
		abre->Progresso = true;
		abre->AvisoNumRede = 0;
		// lê a rede completa
		sucesso = abre->CarregaRedeCompleta();
		// destrói objeto VTAbre
		delete abre;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
void __fastcall TBatchDC::Progresso(AnsiString txt)
{
	if (LBoxLog)
	{
		LBoxLog->Items->Add(txt);
		LBoxLog->Refresh();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TBatchDC::Progresso(TDateTime data)
{
	// variáveis locais
	AnsiString txt;

	if (LBoxLog)
	{
		LBoxLog->Clear();
		LBoxLog->Items->Add(txt.sprintf("%02d/%4d", MonthOf(data), YearOf(data)));
		LBoxLog->Refresh();
	}
}

// -----------------------------------------------------------------------------
// eof
