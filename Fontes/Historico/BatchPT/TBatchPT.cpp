// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <SysUtils.hpp>
#include <DateUtils.hpp>
#include "TBatchPT.h"
#include "RelRegulador\TRelRegulador.h"

#include <Fontes\AjusteDemanda\VTAjusta.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjos.h>
#include <Fontes\BDihm\VTAbre.h>
#include <Fontes\BDihm\VTSalva.h>
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Edita\VTEdita.h>
#include <Fontes\Flow\VTFlow.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\CfgPerdaTec\VTCfgPerdaTec.h>
#include <Fontes\Grafico\VTGrafico.h>
#include <Fontes\ImportaMedicao\VTImportaMedicao.h>
#include <Fontes\ImportaMedicao\VTMedicoes.h>
#include <Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>
#include <Fontes\Obra\VTObra.h>
#include <Fontes\Potencia\VTDemanda.h>
#include <Fontes\Perdatec\VTPerdatec.h>
#include <Fontes\Perdatec\VTResumo.h>
#include <Fontes\Perdatec\VTResumos.h>
#include <Fontes\Radial\VTArvore.h>
#include <Fontes\Radial\VTRadial.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTGerador.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <Fontes\Topologia\VTTopologia.h>
#include "Arquivos\TArqMedicao.h"
#include "Arquivos\TArqRede.h"
#include "Arquivos\TArquivos.h"
#include <DLL_Inc\BDihm.h>
#include <DLL_Inc\AjusteDemanda.h>
#include <DLL_Inc\Edita.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\ImportaMedicao.h>
#include <DLL_Inc\ImportaRedeGis.h>
#include <DLL_Inc\Perdatec.h>
#include <DLL_Inc\Radial.h>

// ---------------------------------------------------------------------------
VTBatchPT* __fastcall NewObjBatchPT(VTApl *apl_owner)
{
	return (new TBatchPT(apl_owner));
}

// ---------------------------------------------------------------------------
__fastcall TBatchPT::TBatchPT(VTApl *apl_owner)
{
	// cria objeto Apl próprio
	// apl = NewObjApl(NULL, apl_owner);
	apl = apl_owner; // não cria apl próprio, mas usa o do TFormBatchPT
	apl->Add(DLL_NewObjRadial(apl));
	apl->Add(DLL_NewObjMedicoes(apl));
	apl->Add(DLL_NewObjSalva(NULL, apl));
	// inicia ponteiros para ListBox de log
	LBoxLogRede = NULL;
	LBoxLogPasso = NULL;
	// cria demais objetos
	files = new TStringList();
	lisEQP = new TList();
	//relatorio regulador
    relRegulador = NULL;
}

// ---------------------------------------------------------------------------
__fastcall TBatchPT::~TBatchPT(void)
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
void __fastcall TBatchPT::AtualizaResumosComEnergiaOriginalDasRedes(VTResumos *resumos)
{
	// variáveis locais
	VTRede *rede;
	VTResumo *resumo;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();

	// proteção
	if (resumos == NULL)
		return;
	// verifica se o Ajuste de Demanda está habilitado
	if (!AjusteDemandaEnabled)
		return;
	// loop para todas Redes
	for (int nr = 0; nr < lisREDE->Count; nr++)
	{
		rede = (VTRede*)lisREDE->Items[nr];
		if ((resumo = resumos->ExisteResumo(rede)) != NULL)
		{ // atualiza valor da energia da rede
			resumo->rede.EnergiaMercado = double(rede->Tag) * 1.0E-6;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TBatchPT::DefineOpcoes(strOPImporta &opcao, TDateTime data)
{
	opcao.date = data;
	opcao.sub.topologia = opSUB_GIS;
	opcao.pri.consA4 = opENERGIA;
	opcao.fator_carga = 0.6;
	opcao.sec.topologia = opSEC_GIS;
	opcao.valida_fase = true;
	opcao.corrige_fase = true;
	opcao.corrige_perda_fe = true;
	opcao.valida_eqpto = true;
	opcao.gerarTXT = false;
	opcao.ihm.log = false;
	opcao.ihm.progresso = false;
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchPT::Executa(void)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString filename;
	TDateTime data;
	TArqRede *arqRede;
	TArquivos *arquivos = (TArquivos*)apl->GetObject(__classid(TArquivos));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));


	if (relRegulador != NULL)
	{
		delete relRegulador;
	}
	relRegulador = new TRelRegulador(apl);
	try
	{ // calcula perda técnica para todos meses do período
		for (data = DataIni; data <= DataFim; data = IncMonth(data, 1))
		{ // determina ArqRede da data indicada
			if ((arqRede = arquivos->ExisteArqRede(data)) == NULL)
				return (false);
			// loop para todas as redes definidas em ArqRede
			for (int n = 0; n < arqRede->files->Count; n++)
			{ // reinicia Redes
				ReiniciaRede();
				// lê as rede do arquivo
				filename = arqRede->files->Strings[n];
				// atualiza progresso
				ProgressoRede(data, AnsiString(ExtractFileName(filename)), n + 1,
					arqRede->files->Count);
				ProgressoPasso(AnsiString("Iniciando..."), true);
				switch (arquivos->TipoArqRede)
				{
				case arqSINAP:
					sucesso = LeRedesBaseSinap(filename);
					break;
				case arqGIS: // verifica se é egrid
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
				if (sucesso)
				{ // exibe a rede carregada
					ExibeRede(redes);
					// inicia objeto Radial
					// radial->Inicia(redes);
					// se necessário, salva energia das Redes antes de fazer o ajuste de demanda
					// SalvaEnergiaOriginalDasRedes();
					// se necessário, executa ajuste de demanda
					if (sucesso)
						sucesso = ExecutaAjusteDemanda(data);
					// executa cálculo de perdas, fazendo o ajuste de demandas em função das opções do cálculo
					if (sucesso)
						sucesso = ExecutaCalculoPerdaTecnica(data);
				}
				// indica diagnóstico do cálculo
				ProgressoRedeResultado(sucesso);
			}
		}
		// reinicia a rede e atualiza o gráfico
		ReiniciaRede();
		ExibeRede(redes);
		//salva o relatorio de reguladores
		SalvaRelReguladores(DataIni);
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16436590
bool __fastcall TBatchPT::ExecutaAjusteDemanda(TDateTime data)
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
		ProgressoPasso(AnsiString("> Executando ajuste de demanda"));
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
					ProgressoPasso(AnsiString("> Diagnóstico do ajuste: " + ajusta_demanda->Diagnostico));
					// finaliza ajuste
					ajusta_demanda->Finaliza();
					// remove os medidores inseridos
					edita->UndoCompleto();
				}
                else
				{ // atualiza progresso
					ProgressoPasso(AnsiString("> Não há medições válidas para as redes selecionadas"));
				}
			}
			else
			{ // atualiza progresso
				ProgressoPasso(AnsiString("> Não há medições para as redes selecionadas"));
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
bool __fastcall TBatchPT::ExecutaCalculoPerdaTecnica(TDateTime data)
{
	// variáveis locais
	bool sucesso = false;
	VTApl *apl_pertec;
	VTArvore *arvore;
	VTPerdatec *perdatec;
	VTSalva *salva;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	try
	{ // cria objetos p/ cálculo de perdas
		apl_pertec = DLL_NewObjAplPerdatec(NULL, apl);
		perdatec = (VTPerdatec*)apl_pertec->GetObject(__classid(VTPerdatec));
		salva = (VTSalva*)apl_pertec->GetObject(__classid(VTSalva));
		// proteção
		if (!salva)
			apl_pertec->Add(salva = DLL_NewObjSalva(NULL, apl_pertec));
		// atualiza progresso
		// ProgressoPasso(AnsiString("Configurando a rede"));
		// prepara a rede conforme parâmetros de cálculo de perdas
		geral->CfgPerdaTec->Alterada = true;
		geral->CfgPerdaTec->Progresso = false;
		// sucesso = perdatec->PreparaRede(data);
		// atualiza progresso
		ProgressoPasso(AnsiString("> Calculando perdas técnicas"));
		// executa cálculo de perdas
		sucesso = perdatec->Executa(data, SEMANA_COMPLETA);
		// grava perdas na base de dados
		if (sucesso)
		{
			arvore = (VTArvore*)apl_pertec->GetObject(__classid(VTArvore));
			// arvore->Data       = data;
			// arvore->TipoArvore = arvorePERDATEC;
			// atualiza Resumos com energia inicial salvo antes de fazer ajuste de demanda
			AtualizaResumosComEnergiaOriginalDasRedes((VTResumos*)apl->GetObject
				(__classid(VTResumos)));
			// atualiza progresso
			ProgressoPasso(AnsiString("> Salvando perdas técnicas na base de histórico"));
			sucesso = salva->SalvaHistoricoPerdaTecnica(arvore);
		}
		// preenche o relatorio de energias passante antes de destruirem a rede
		ExecutaRelReguladores(data);
		// limpa a rede antes de destruir objeto apl_pertec
		ReiniciaRede();
		// destrói objetos
		delete apl_pertec;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}
// ---------------------------------------------------------------------------
void __fastcall TBatchPT::ExecutaRelReguladores(TDateTime data)
{
//	int nDias;
//
//	nDias = DaysInMonth(data);
	if (relRegulador != NULL)
	{
		relRegulador->Executa(data);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TBatchPT::ExibeRede(VTRedes *redes)
{
	// variáveis locais
	TList *lisREDE;
	VTRede *rede;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// não exibe as redes secundárias
	lisREDE = redes->LisRede();
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		rede->Show[eqp_VISIBLE] = (rede->TipoRede->Segmento != redeSEC);
	}
	// atualiza o gráfico
	// grafico->Hide();
	grafico->AreaRedeDefine();
	// grafico->Show(grafTOPO);
	if (apl->Owner->Form)
		apl->Owner->Form->Refresh();
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchPT::ExisteGeracao(void)
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
bool __fastcall TBatchPT::ExisteMaeSelecionada(VTSub* sub, TList* lisSUB)
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
bool __fastcall TBatchPT::ExisteTrafoNaRede(void)
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
bool __fastcall TBatchPT::ImportaMedicao(TDateTime data)
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
 bool __fastcall TBatchPT::ImportaRedesArqGIS(TDateTime data, TArqRede *arqRede)
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
 DefineOpcoes(opcao, data);
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
 // destroi objeto
 delete ImportaRedeGIS;
 return sucesso;
 }
 // destrói objeto
 delete ImportaRedeGIS;
 return (sucesso);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TBatchPT::ImportaRedesArqGIS(TDateTime data, AnsiString filename)
{
	// variáveis locais
	bool sucesso = false;
	strOPImporta opcao;
	VTImportaRedeGIS *ImportaRedeGIS;

	// verifica se existe o arquivo
	if (!FileExists(filename))
		return (false);
	// define opções p/ importação
	DefineOpcoes(opcao, data);
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
bool __fastcall TBatchPT::ImportaRedesListaArqGIS(TDateTime data, AnsiString filename)
{
	// variáveis locais
	bool sucesso;
	strOPImporta opcao;
	VTImportaRedeGIS *ImportaRedeGIS;

	// verifica se existe o arquivo
	if (!FileExists(filename))
		return (false);
	// define opções p/ importação
	DefineOpcoes(opcao, data);
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
bool __fastcall TBatchPT::ImportaRedesSubestacaoGIS(TDateTime data, TArqRede *arqRede)
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
	DefineOpcoes(opcao, data);
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
		LBoxLogRede->Items->Add(sub->CodSub);
		LBoxLogRede->Refresh();
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
		// // se necessário, executa ajuste de demanda DEPOIS de preparar a rede
		// if (sucesso)
		// sucesso = ExecutaAjusteDemanda(data);
		if (sucesso)
		{ // exibe a rede carregada
			ExibeRede(redes);
			// se necessário, executa ajuste de demanda
			if (sucesso)
				sucesso = ExecutaAjusteDemanda(data);
			// executa cálculo de perdas, fazendo o ajuste de demandas em função das opções do cálculo
			if (sucesso)
				sucesso = ExecutaCalculoPerdaTecnica(data);
		}
		// indica diagnóstico do cálculo
		ProgressoRedeResultado(sucesso);
		// reinicia a rede e atualiza o gráfico
		ReiniciaRede();
		ExibeRede(redes);
		// destroi objeto
		delete ImportaRedeGIS;
	}
	// destroi lista
	delete lisTMP;

	return sucesso;
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchPT::LeRedesBaseSinap(AnsiString filename)
{
	// variáveis locais
	bool sucesso;
	VTAbre *abre;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	try
	{ // atualiza progresso
		ProgressoPasso(AnsiString("Lendo rede da base"));
		// atualiza aruivo da rede em VTPath
		path->ArqRede = filename;
		// teste
		abre = NULL;
		// cria objeto VTAbre
		abre = DLL_NewObjAbre(NULL, apl);
		// desabilita progresso
		abre->Progresso = false;
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
void __fastcall TBatchPT::ProgressoPasso(AnsiString txt, bool clear)
{
	if (LBoxLogPasso)
	{ // verifica se deve reiniciar o LBox
		if (clear)
			LBoxLogPasso->Clear();
		LBoxLogPasso->Items->Add(txt);
		// atualiza tela
		Refresh();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TBatchPT::ProgressoRede(TDateTime data, AnsiString filename, int fileindex,
	int filecount)
{
	// variáveis locais
	AnsiString txt;

	if (LBoxLogRede)
	{
		txt.sprintf("%02d/%4d: %-10s  [%03d/%03d]", MonthOf(data), YearOf(data), filename.c_str(),
			fileindex, filecount);
		LBoxLogRede->Items->Insert(0, txt);
		// atualiza tela
		Refresh();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TBatchPT::ProgressoRedeResultado(bool sucesso)
{
	// variáveis locais
	AnsiString txt;

	// obtém texto do primeiro string do ListBox
	if (LBoxLogRede->Items->Count < 0)
		return;
	// atualiza primeiro string do ListBox
	if (sucesso)
		txt = LBoxLogRede->Items->Strings[0] + " - Sucesso";
	else
		txt = LBoxLogRede->Items->Strings[0] + " - Erro";
	LBoxLogRede->Items->Strings[0] = txt;
	// atualiza tela
	Refresh();
}

// ---------------------------------------------------------------------------
void __fastcall TBatchPT::Refresh(void)
{
	// atualiza tela
	if (apl->Form)
	{
		apl->Form->Refresh();
		Application->ProcessMessages();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TBatchPT::ReiniciaRede(void)
{
	// variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	redes->Clear();
}

// ---------------------------------------------------------------------------
void __fastcall TBatchPT::SalvaEnergiaOriginalDasRedes(void)
{
	/*
	 //variáveis locais
	 double     energia_mwh;
	 VTCarga   *carga;
	 VTRede    *rede;
	 VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	 VTRedes   *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
	 TList     *lisREDE = redes->LisRede();

	 //verifica se o Ajuste de Demanda está habilitado
	 if (! AjusteDemandaEnabled) return;
	 //loop para todas Redes
	 for (int nr = 0; nr < lisREDE->Count; nr++)
	 {
	 rede = (VTRede*)lisREDE->Items[nr];
	 //obtém lista de todas Cargas da Rede
	 lisEQP->Clear();
	 rede->LisEqbar(lisEQP, eqptoCARGA);
	 //acumula energia de todas as Crgas
	 energia_mwh = 0;
	 for (int nc = 0; nc < lisEQP->Count; nc++)
	 {
	 carga        = (VTCarga*)lisEQP->Items[nc];
	 energia_mwh += demanda->Edia_mwh[carga];
	 }
	 //salva valor da energia na variável Tag da Rede (valor inteiro em wh)
	 rede->Tag = int(energia_mwh * 1.0E6);
	 }
	 */
}
// -----------------------------------------------------------------------------
void __fastcall TBatchPT::SalvaRelReguladores(TDateTime data)
{
	AnsiString nomeArquivo = "RelatorioRegulador";
	AnsiString mesStr = "_";

	if (relRegulador != NULL)
	{
		mesStr += relRegulador->MesStr(data);
		nomeArquivo += mesStr;
		relRegulador->SalvaArquivo(nomeArquivo);
	}
}
// -----------------------------------------------------------------------------
// eof
