// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TBatchPR.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjos.h>
#include <Fontes\BDihm\VTAbre.h>
#include <Fontes\BDihm\VTSalva.h>
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Curvas\VTCurvas.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Edita\VTEdita.h>
#include <Fontes\Flow\VTFlow.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\CfgPerdaTec\VTCfgPerdaTec.h>
#include <Fontes\Grafico\VTGrafico.h>
#include <Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>
#include <Fontes\Obra\VTObra.h>
#include <Fontes\Perdatec\VTPerdareg.h>
#include <Fontes\Radial\VTArvore.h>
#include <Fontes\Radial\VTRadial.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <Fontes\Topologia\VTTopologia.h>
#include "Arquivos\TArqMedicao.h"
#include "Arquivos\TArqRede.h"
#include "Arquivos\TArquivos.h"
#include <DLL_Inc\BDihm.h>
#include <DLL_Inc\Edita.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\ImportaRedeGis.h>
#include <DLL_Inc\Perdatec.h>
#include <DLL_Inc\Radial.h>

// ---------------------------------------------------------------------------
VTBatchPR* __fastcall NewObjBatchPR(VTApl *apl_owner)
{
	return (new TBatchPR(apl_owner));
}

// ---------------------------------------------------------------------------
__fastcall TBatchPR::TBatchPR(VTApl *apl_owner)
{
	// cria objeto Apl próprio
	// apl = NewObjApl(NULL, apl_owner);
	apl = apl_owner; // não cria apl próprio, mas usa o do TFormBatchPR
	apl->Add(radial = DLL_NewObjRadial(apl));
	apl->Add(salva = DLL_NewObjSalva(NULL, apl));
	// inicia ponteiros para ListBox de log
	LBoxLogRede = NULL;
	LBoxLogPasso = NULL;
	// cria demais objetos
	files = new TStringList();
	lisEQP = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TBatchPR::~TBatchPR(void)
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
void __fastcall TBatchPR::DefineOpcoes(strOPImporta &opcao, TDateTime data)
{
	opcao.date = data;
	opcao.sub.topologia = opSUB_GIS;
	opcao.pri.consA4 = opENERGIA;
	opcao.fator_carga = 0.6;
	opcao.sec.topologia = opSEC_GIS; // opSEC_CARGA;
	opcao.valida_fase = true;
	opcao.corrige_fase = true;
	opcao.corrige_perda_fe = true;
	opcao.valida_eqpto = true;
	opcao.gerarTXT = false;
	opcao.ihm.log = false;
	opcao.ihm.progresso = false;
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchPR::Executa(void)
{
	// variáveis locais
	bool sucesso = true;
	AnsiString filename;
	TDateTime data;
	TArqRede *arqRede;
	TArquivos *arquivos = (TArquivos*)apl->GetObject(__classid(TArquivos));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

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
				// lê rede de acordo com o tipo de arquivo
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
					radial->Inicia(redes);
					// executa cálculo de perda
					sucesso = ExecutaCalculoPerdaRegulatoria(data);
				}
				// indica diagnóstico do cálculo
				ProgressoRedeResultado(sucesso);
			}
		}
		// reinicia a rede e atualiza o gráfico
		ReiniciaRede();
		ExibeRede(redes);
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchPR::ExecutaCalculoPerdaRegulatoria(TDateTime data)
{
	// variáveis locais
	bool sucesso = false;
	TArqMedicao *arq_medicao;
	VTApl *apl_perdareg;
	VTArvore *arvore;
	VTPerdareg *perdareg;
	VTSalva *salva;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	try
	{ // cria objetos p/ cálculo de perdas
		apl_perdareg = DLL_NewObjAplPerdareg(NULL, apl);
		perdareg = (VTPerdareg*)apl_perdareg->GetObject(__classid(VTPerdareg));
		salva = (VTSalva*)apl_perdareg->GetObject(__classid(VTSalva));
		// proteção
		if (!salva)
			apl_perdareg->Add(salva = DLL_NewObjSalva(NULL, apl_perdareg));
		// determina arquivos com dados de medição
		if ((arq_medicao = ExisteArquivoMedicao(data)) == NULL)
		{
			ProgressoPasso(AnsiString("Não existe arquivo de medição"));
			return (false);
		}
		// atualiza progresso
		ProgressoPasso(AnsiString("Lendo dados de medição"));
		// importa dados de Medicao
		if (!perdareg->ImportaMedicao(data, arq_medicao->files))
		{
			ProgressoPasso(AnsiString("Erro na leitura de dados de medição"));
			return (false);
		}
		// atualiza progresso
		// ProgressoPasso(AnsiString("Configurando a rede"));
		// prepara a rede conforme parâmetros de cálculo de perdas
		geral->CfgPerdaTec->Alterada = true;
		geral->CfgPerdaTec->Progresso = false;
		// atualiza progresso
		ProgressoPasso(AnsiString("Calculando perdas regulatórias"));
		// executa cálculo de perdas
		sucesso = perdareg->Executa(data, true);
		// grava perdas na base de dados
		if (sucesso)
		{ // atualiza progresso
			ProgressoPasso(AnsiString("Salvando perdas técnicas na base de histórico"));
			arvore = (VTArvore*)apl_perdareg->GetObject(__classid(VTArvore));
			// arvore->Data       = data;
			// arvore->TipoArvore = arvorePERDATEC;
			sucesso = salva->SalvaHistoricoPerdaTecnica(arvore);
		}
		// limpa a rede antes de destruir objeto apl_perdareg
		ReiniciaRede();
		// destrói objetos
		delete apl_perdareg;
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
void __fastcall TBatchPR::ExibeRede(VTRedes *redes)
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
bool __fastcall TBatchPR::ExisteTrafoNaRede(void)
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
TArqMedicao* __fastcall TBatchPR::ExisteArquivoMedicao(TDateTime data)
{
	// variáveis locais
	TArquivos *arquivos = (TArquivos*)apl->GetObject(__classid(TArquivos));

	// determina ArqMedicao da data indicada
	return (arquivos->ExisteArqMedicao(data));
}

// ---------------------------------------------------------------------------
void __fastcall TBatchPR::GraficoExibeRede(void)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	grafico->AreaRedeAltera();
	grafico->Show(grafTOPO, 0, 0);
}
/*
// ---------------------------------------------------------------------------
bool __fastcall TBatchPR::ImportaRedesArqGIS(TDateTime data, TArqRede *arqRede)
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
	}

	// destrói objeto
	delete ImportaRedeGIS;
	return (sucesso);
}
*/
// ---------------------------------------------------------------------------
bool __fastcall TBatchPR::ImportaRedesArqGIS(TDateTime data, AnsiString filename)
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
bool __fastcall TBatchPR::ImportaRedesListaArqGIS(TDateTime data, AnsiString filename)
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
bool __fastcall TBatchPR::ImportaRedesSubestacaoGIS(TDateTime data, TArqRede *arqRede)
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
		if (sucesso)
		{ // exibe a rede carregada
			ExibeRede(redes);
			// inicia objeto Radial
			radial->Inicia(redes);
			// executa cálculo de perda
			sucesso = ExecutaCalculoPerdaRegulatoria(data);
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
bool __fastcall TBatchPR::LeRedesBaseSinap(AnsiString filename)
{
	// variáveis locais
	bool sucesso;
	VTAbre *abre;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTCurvas *curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

	try
	{ // atualiza progresso
		ProgressoPasso(AnsiString("Lendo rede da base"));
		// altera tipo de curva para semana média //DVK 2016.04.15
		curvas->DiaMedicao = SEMANA_COMPLETA;
		// atualiza aruivo da rede em VTPath
		path->ArqRede = filename;
		// cria objeto VTAbre
		abre = DLL_NewObjAbre(NULL, apl);
		// desabilita progresso
		abre->Progresso = false;
		abre->AvisoNumRede = 0;
		// lê a rede completa
		sucesso = abre->CarregaRedeCompleta();
		// destrói objeto VTAbre
		delete abre;
		// atualiza o grafico
		GraficoExibeRede();
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
void __fastcall TBatchPR::ProgressoPasso(AnsiString txt, bool clear)
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
void __fastcall TBatchPR::ProgressoRede(TDateTime data, AnsiString filename, int fileindex,
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
void __fastcall TBatchPR::ProgressoRedeResultado(bool sucesso)
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
void __fastcall TBatchPR::Refresh(void)
{
	// atualiza tela
	if (apl->Form)
	{
		apl->Form->Refresh();
		Application->ProcessMessages();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TBatchPR::ReiniciaRede(void)
{
	// variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	redes->Clear();
}

// -----------------------------------------------------------------------------
// eof
