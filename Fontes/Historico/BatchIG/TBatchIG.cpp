// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TBatchIG.h"
#include <Fontes\AjusteDemanda\VTAjusta.h>
#include <Fontes\Arranjo\VTArranjos.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\BD\VTBD.h>
#include <Fontes\BD\Rede\VTBDgeral.h>
#include <Fontes\BDihm\VTAbre.h>
#include <Fontes\BDihm\VTSalva.h>
#include <Fontes\Curvas\VTCurva.h>
#include <Fontes\Classes\VTClasse.h>
#include <Fontes\Classes\VTClasses.h>
#include <Fontes\Diretorio\VTDir.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Edita\VTEdita.h>
#include <Fontes\Flow\VTFlow.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\CfgPerdaTec\VTCfgPerdaTec.h>
#include <Fontes\ImportaMedicao\VTImportaMedicao.h>
#include <Fontes\ImportaMedicao\VTMedicoes.h>
#include <Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>
#include <Fontes\Mercado\VTCrescimento.h>
#include <Fontes\Mercado\VTMercado.h>
#include <Fontes\Mercado\VTSegmento.h>
#include <Fontes\Obra\VTObra.h>
#include <Fontes\Radial\VTRadial.h>
#include <Fontes\Radial\VTSubestacao.h>
#include <Fontes\Rede\VTCapacitor.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTGerador.h>
#include <Fontes\Rede\VTListeqp.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTSuprimento.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <Fontes\Redutor\VTRedutor.h>
#include <Fontes\Topologia\VTTopologia.h>
#include "Arquivos\TArqMedicao.h"
#include "Arquivos\TArqRede.h"
#include "Arquivos\TArquivos.h"
#include <DLL_Inc\BD.h>
#include <DLL_Inc\BDihm.h>
#include <DLL_Inc\AjusteDemanda.h>
#include <DLL_Inc\Edita.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\ImportaMedicao.h>
#include <DLL_Inc\ImportaRedeGis.h>
#include <DLL_Inc\Mercado.h>
#include <DLL_Inc\Radial.h>
#include <DLL_Inc\Redutor.h>

// ---------------------------------------------------------------------------
VTBatchIG* __fastcall NewObjBatchIG(VTApl *apl_owner)
{
	return (new TBatchIG(apl_owner));
}

// ---------------------------------------------------------------------------
__fastcall TBatchIG::TBatchIG(VTApl *apl_owner)
{
	// não cria apl próprio, mas usa o do TFormBatchIG
	apl = apl_owner;
	// cria objetos adicionais
	apl->Add(DLL_NewObjRadial(apl));
	apl->Add(DLL_NewObjMedicoes(apl));
	apl->Add(DLL_NewObjSalva(NULL, apl));
	// cria demais objetos
	files = new TStringList();
	lisEQP = new TList();
	// inicia variavel
	nome_base = "";
}

// ---------------------------------------------------------------------------
__fastcall TBatchIG::~TBatchIG(void)
{
	// destrói objetos
	if (mercado_global)
	{
		delete mercado_global;
		mercado_global = NULL;
	}
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
AnsiString __fastcall TBatchIG::CodigoRedeSupridora(void)
{
	// variáveis locais
	TList *lisEXT;
	VTSubestacao *subestacao;
	VTRadial *radial = (VTRadial*)apl->GetObject(__classid(VTRadial));

	// verifica se há Subestacao
	lisEXT = radial->LisSubestacao();
	if (lisEXT->Count > 0)
	{
		subestacao = (VTSubestacao*)lisEXT->First();
		return (subestacao->Rede->Codigo);
	}
	return ("Indefinida");
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchIG::CriaBaseAccessDaRede(AnsiString arq_rede)
{
	// variáveis locais
	VTDir *dir = (VTDir*)apl->GetObject(__classid(VTDir));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// cria uma nova base de dados Access
	if (!dir->CriaCopiaRedeSinap(arq_rede, false))
		return (false);
	// atualiza objeto Path p/ apontar p/ nova base de dados (VTDir já aponta a nova base)
	path->ArqRede = arq_rede;
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchIG::DefineDiretorioRede(AnsiString &dir_rede)
{
	// variáveis locais
	int count = 1;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	// loop p/ definir o nome de um diretório ainda não existente
	do
	{
		dir_rede = path->DirBase + "\\Redes_" + IntToStr(count++);
	}
	while (DirectoryExists(dir_rede));
	// cria diretório
	CreateDir(dir_rede);

	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchIG::Executa(TListBox *LBoxLog)
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
		// inicia que usuário precisa confirmar a base unica
		usuario_confirmou_base_unica = false;
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
					// verifica se é base separada por subestação E egrid
					if ((!BaseUnica) && (arqRede->lisSub->Count > 0))
					{
						sucesso = ImportaRedesSubestacaoGIS(data, arqRede);
					}
					else if (arqRede->lisSub->Count > 0)
					{ // base unica E egrid
						sucesso = ImportaRedesArqGIS(data, arqRede);
					}
					else
					{ // outro tipo de arquivo
						sucesso = ImportaRedesArqGIS(data, filename);
					}
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
				// salva rede na base de dados
				if (sucesso)
					if (!SalvaRedeNaBase(data))
						continue;
			}
		}
		// salva Mercado global na base
		MercadoGlobalSalvaNaBase();
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return (sucesso);
}
/*
 //---------------------------------------------------------------------------
 bool __fastcall TBatchIG::ExecutaAjusteDemanda(TDateTime data)
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
 //verifica se há Trafos nas redes primárias ou secundárias
 if (ExisteTrafoNaRede())
 {//cria objeto VTAjusta para ajuste por fase
 ajusta_demanda = DLL_NewObjAjustaPorFase(apl);
 //configura opções de Ajusta
 ajusta_demanda->TipoFluxo     = ajusta->TipoFluxo;
 ajusta_demanda->IncluirConsA4 = ajusta->IncluirConsA4;
 ajusta_demanda->IncluirPerda  = ajusta->IncluirPerda;
 ajusta_demanda->LimitaAjuste  = ajusta->LimitaAjuste;
 ajusta_demanda->AjusteMaximo  = ajusta->AjusteMaximo;
 ajusta_demanda->ErroMaximo    = ajusta->ErroMaximo;
 //destroi ajusta antigo e mantém o criado
 apl->Add(ajusta_demanda);
 apl->Remove(ajusta);
 delete ajusta;
 }
 else
 {//utiliza objeto VTAjusta para ajuste trifásico
 ajusta_demanda = ajusta;
 }
 //calcula fluxo inicial
 sucesso = ajusta_demanda->CalculaFluxo();
 //atualiza valores estimados dos medidores
 medicoes->AtualizaMedidores();
 //inicia processo, validando medições e dados
 if(! medicoes->Valida(ajusta_demanda->LimitaAjuste, ajusta_demanda->AjusteMaximo)) return(false);
 //atualiza progresso
 Progresso(AnsiString("Executando ajuste de demanda"));
 //executa ajuste da demanda
 if(sucesso) sucesso = ajusta_demanda->Executa(med_neg_enabled);
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
bool __fastcall TBatchIG::ExecutaAjusteDemanda(TDateTime data)
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
					// verifica se foi ajustado mesmo
					if (ajusta_demanda->Diagnostico.IsEmpty())
						ajusta_demanda->Diagnostico = "OK";
					Progresso(AnsiString("> Diagnóstico do ajuste - " + ajusta_demanda->Diagnostico));
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
bool __fastcall TBatchIG::ExisteGeracao(void)
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
bool __fastcall TBatchIG::ExisteMaeSelecionada(VTSub* sub, TList* lisSUB)
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
bool __fastcall TBatchIG::ExisteTrafoNaRede(void)
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
bool __fastcall TBatchIG::ImportaMedicao(TDateTime data)
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
 //---------------------------------------------------------------------------
 bool __fastcall TBatchIG::ImportaRedesArqGIS(TDateTime data, AnsiString filename)
 {
 //variáveis locais
 bool              sucesso;
 strOPImporta      opcao;
 VTImportaRedeGIS *ImportaRedeGIS;

 //verifica se existe o arquivo
 if (! FileExists(filename)) return(false);
 //define opções p/ importação
 //   opcao.date               = data;
 //   opcao.sub.topologia      = opSUB_GIS;
 //   opcao.pri.consA4         = opENERGIA;
 //   opcao.fator_carga        = 0.6;
 //   opcao.sec.topologia      = opSEC_GIS;//opSEC_CARGA;
 //   opcao.valida_fase        = true;
 //   opcao.corrige_fase       = true;
 //   opcao.corrige_perda_fe   = true;
 //   opcao.valida_eqpto       = true;
 //   opcao.gerarTXT           = false;
 //   opcao.ihm.log            = false;
 //   opcao.ihm.progresso      = true;
 opcao = PD.opcao;
 //cria objeto ImportaRedeGIS
 if ((ImportaRedeGIS = DLL_NewObjImportaRedeGIS(NULL, apl)) == NULL) return(false);
 //inicia files com o arquivo com lista de arquivos
 files->Clear();
 files->Add(filename);
 //executa importação da rede
 sucesso = ImportaRedeGIS->ImportaLisArqPrimario(files, &opcao);
 //destrói objeto Conversor
 delete ImportaRedeGIS;
 return(sucesso);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TBatchIG::ImportaRedesArqGIS(TDateTime data, TArqRede *arqRede)
{
	// variáveis locais
	AnsiString filename;
	bool sucesso = false;
	strOPImporta opcao;
	VTImportaRedeGIS *ImportaRedeGIS;
	int destino = 0;
	VTArranjos *arranjos;
	VTRedes *redes;
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
	// cria objeto ImportaRedeGIS
	if ((ImportaRedeGIS = DLL_NewObjImportaRedeGIS(NULL, apl)) == NULL)
		return (false);

	// verifica se é egrid
	if (ImportaRedeGIS->ArquivoEgrid())
	{// limpa arranjos e redes
		arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
		arranjos->Clear();
		redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
		redes->Clear();
		// le dados da empresa e insere subestações c/filhas selecionadas
		ImportaRedeGIS->ImportaDadosEmpresa(filename);
		ImportaRedeGIS->InsereSubestacao(arqRede->lisSub);
		// monta nome da base
		MontaNomeBase(arqRede->lisSub);
		// atualiza progresso
		Progresso(AnsiString("> " + nome_base));
		// importa na memoria (destino = 0)
		sucesso = ImportaRedeGIS->ImportaEgrid(&opcao, destino);
        if(topologia) topologia->Reconfigura(redes);
	}
	if (sucesso)
		sucesso = ExecutaAjusteDemanda(data);
	// salva rede na base de dados
	if (sucesso)
	{
		if (SalvaRedeNaBase(data))
		{ // salva Mercado global na base
			MercadoGlobalSalvaNaBase();
		}
	}
	// destrói objeto
	delete ImportaRedeGIS;
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchIG::ImportaRedesArqGIS(TDateTime data, AnsiString filename)
{
	// variáveis locais
	bool sucesso = false;
	strOPImporta opcao;
	VTImportaRedeGIS *ImportaRedeGIS;
	int destino = 0;

	// verifica se existe o arquivo
	if (!FileExists(filename))
		return (false);
	// define opções p/ importação
	opcao = PD.opcao;
	// cria objeto ImportaRedeGIS
	if ((ImportaRedeGIS = DLL_NewObjImportaRedeGIS(NULL, apl)) == NULL)
		return (false);
	// inicia files com o arquivo com lista de arquivos
	files->Clear();
	files->Add(filename);
	// executa importação da rede
	sucesso = ImportaRedeGIS->ImportaLisArqPrimario(files, &opcao);
	// destrói objeto
	delete ImportaRedeGIS;
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchIG::ImportaRedesListaArqGIS(TDateTime data, AnsiString filename)
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

/*
 // ---------------------------------------------------------------------------
 bool __fastcall TBatchIG::ImportaRedesSubestacaoGIS(TDateTime data, TArqRede *arqRede)
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
 VTRedes *redes;

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
 MontaNomeBase(lisTMP);
 // atualiza LBoxLog
 LBoxLog->Items->Add(nome_base);
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
 // limpa arranjos e redes
 arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
 arranjos->Clear();
 redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
 redes->Clear();
 // le dados da empresa e insere subestações c/filhas selecionadas
 ImportaRedeGIS->ImportaDadosEmpresa(filename);
 ImportaRedeGIS->InsereSubestacao(lisTMP);
 // importa na memoria (destino = 0)
 sucesso = ImportaRedeGIS->ImportaEgrid(&opcao, destino);
 // se necessário, executa ajuste de demanda DEPOIS de preparar a rede
 if (sucesso)
 sucesso = ExecutaAjusteDemanda(data);
 // salva rede na base de dados
 if (sucesso)
 {
 if (!SalvaRedeNaBase(data))
 continue;
 // salva Mercado global na base
 MercadoGlobalSalvaNaBase();
 }
 // destroi objeto
 delete ImportaRedeGIS;
 }
 // destroi lista
 delete lisTMP;

 return sucesso;
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TBatchIG::ImportaRedesSubestacaoGIS(TDateTime data, TArqRede *arqRede)
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
	VTRedes *redes;
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
	//copia subestações filtradas
	arqRede->lisSub->Clear();
	CopiaTList(lisTMP, arqRede->lisSub);
	for (int n = 0; n < arqRede->lisSub->Count; n++)
	{
		lisTMP->Clear();
		sub = (VTSub*)arqRede->lisSub->Items[n];
		//verifica se ha primarios selecionados
		if(sub->PrimariosSel->Count == 0)
            continue;
		// insere sub e suas filhas, se houverem
		lisTMP->Add(sub);
		if (sub->SubFilha->Count > 0)
		{
			CopiaTList(sub->SubFilha, lisTMP);
		}
		// define nome para base de dados
		MontaNomeBase(lisTMP);
		// atualiza LBoxLog
		LBoxLog->Items->Add(nome_base);
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
		// limpa arranjos e redes
		arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
		arranjos->Clear();
		redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
		redes->Clear();
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
		{
			if (!SalvaRedeNaBase(data))
				continue;
			// salva Mercado global na base
			MercadoGlobalSalvaNaBase();
		}
		// destroi objeto
		delete ImportaRedeGIS;
	}
	// destroi lista
	delete lisTMP;

	return sucesso;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15653253
bool __fastcall TBatchIG::LeRedesBaseSinap(AnsiString filename)
{
	// variáveis locais
	bool sucesso;
	AnsiString arq_rede_atual;
	VTAbre *abre;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	try
	{ // atualiza progresso
		Progresso(AnsiString("Lendo rede da base"));
		// salva path da base de dados atual
		arq_rede_atual = path->ArqRede;
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
		// restaura path da base de dados atual
		path->ArqRede = arq_rede_atual;
		// salva dados do Mercado da base lida no objeto Mercado global
		MercadoGlobalAtualiza();
	}
	catch (Exception &e)
	{
		if(abre)
			delete abre;
		sucesso = false;
	}
	catch (...)
	{
		if(abre)
			delete abre;
		sucesso = false;
	}
	return (sucesso);
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchIG::MercadoGlobalAtualiza(void)
{
	// variáveis locais
	// VTClasses      *classes     = (VTClasses*)apl->GetObject(__classid(VTClasses));
	// VTCrescimento  *crescimento;
	VTSegmento *segmento;
	VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
	TList *lisSEGMENTO = mercado->LisSegmento();
	TList *lisCRESCIMENTO;

	// verifica se as redes estão sendo salvas em uma base única
	if (!BaseUnica)
		return (true);
	// verifica se o objeto Mercado global já foi criado
	if (mercado_global == NULL)
	{ // IMPORTANTE: cria objeto Mercado global sem inclui-lo no objeto Apl
		// para unir o Mercado de todas as bases de dados
		if ((mercado_global = DLL_NewObjMercado(apl)) == NULL)
			return (false);
	}
	// loop p/ todos Segmentos do Mercado lido da base atual
	for (int n = lisSEGMENTO->Count - 1; n >= 0; n--)
	{
		segmento = (VTSegmento*)lisSEGMENTO->Items[n];
		/*
		 lisCRESCIMENTO = segmento->LisCrescimento();
		 for(int nc = 0; nc < lisCRESCIMENTO->Count; nc++)
		 {
		 crescimento = (VTCrescimento*)lisCRESCIMENTO->Items[nc];
		 crescimento->Classe = classes->ExisteClasseTag(crescimento->Classe->Tag);
		 }
		 */
		// verifica se o Segmento é de uma Rede
		if (segmento->Tipo == smREDE)
		{ // transfere Segmento para o Mercado global
			lisSEGMENTO->Delete(n);
			mercado_global->InsereSegmento(segmento);
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TBatchIG::MercadoGlobalAtualizaClasse(void)
{
	// variáveis locais
	VTClasses *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
	VTCrescimento *crescimento;
	VTSegmento *segmento;
	TList *lisSEGMENTO = mercado_global->LisSegmento();
	TList *lisCRESCIMENTO;

	// loop p/ todos Segmentos do Mercado lido da base atual
	for (int n = lisSEGMENTO->Count - 1; n >= 0; n--)
	{
		segmento = (VTSegmento*)lisSEGMENTO->Items[n];
		lisCRESCIMENTO = segmento->LisCrescimento();
		for (int nc = 0; nc < lisCRESCIMENTO->Count; nc++)
		{
			crescimento = (VTCrescimento*)lisCRESCIMENTO->Items[nc];
			crescimento->Classe = classes->ExisteClasse(crescimento->Codigo);
		}
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16325016
bool __fastcall TBatchIG::MercadoGlobalSalvaNaBase(void)
{
	// variáveis locais
	bool sucesso = false;
	VTBD *bd;
	VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

	// verifica se foi criado o objeto Mercado global
	if (mercado_global == NULL)
		return (true);
	// atualiza classes dos crescimentos do mercado
	MercadoGlobalAtualizaClasse();
	// substitui o objeto Mercado do Apl pelo Mercado global
	//CUIDADO: o Apl aqui é local e o mercado original está no Owner do Apl
	apl->Owner->Remove(mercado);
	apl->Add(mercado_global);
	// cria objeto BD
	if ((bd = DLL_NewObjBD(apl)) != NULL)
	{ // inicia conexão com base de dados
		if (bd->Conecta())
		{
			sucesso = bd->Geral->SalvaMercado();
		}
		// destrói objeto BD
		delete bd;
	}
	// desfaz a troca dos objetos Mercado no objeto Apl
	apl->Remove(mercado_global);
	apl->Owner->Add(mercado);

	return (sucesso);
}

// ---------------------------------------------------------------------------
void __fastcall TBatchIG::MontaNomeBase(TList *lisSub)
{
	// variáveis locais
	VTSub *sub;

	// inicia nome_base
	nome_base = "Rede";
	for (int n = 0; n < lisSub->Count; n++)
	{
		sub = (VTSub*)lisSub->Items[n];
		if(sub->PrimariosSel->Count == 0)
            continue;
		nome_base += "_" + sub->CodSub;
	}
	// finaliza
	// nome_base += ".mdb";
}

// ---------------------------------------------------------------------------
void __fastcall TBatchIG::Progresso(AnsiString txt)
{
	if (LBoxLog)
	{
		LBoxLog->Items->Add(txt);
		LBoxLog->Refresh();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TBatchIG::Progresso(TDateTime data)
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

// ---------------------------------------------------------------------------
void __fastcall TBatchIG::RedefineId(void)
{ // DVK 2016.04.12
	// variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	VTRede *rede;
	TList *lisREDE = redes->LisRede();
	TList *lisTMP = new TList();

	// redefine IDs de todos Eqptos
	RedefineId(arranjos->LisSuporte());
	RedefineId(arranjos->LisCabo());
	RedefineId(arranjos->LisArranjo());
	RedefineId(redes->LisDominio());
	RedefineId(redes->LisRede());
	RedefineId(redes->LisCorte());
	RedefineId(redes->LisBarra());
	lisTMP->Clear();
	redes->LisEqbar(lisTMP);
	RedefineId(lisTMP);
	RedefineIdCurvaPropria(lisTMP);
	lisTMP->Clear();
	redes->LisLigacao(lisTMP);
	RedefineId(lisTMP);
	lisTMP->Clear();
	redes->LisReducao(lisTMP);
	RedefineId(lisTMP);
	lisTMP->Clear();
	redes->LisMutua(lisTMP);
	RedefineId(lisTMP);
	lisTMP->Clear();
	// reinicia id das Barras inicias das redes
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		rede->DefineBarraInicial(rede->BarraInicial());
	}
	// importante: reordena lista de Barras de VTRedes
	VTListeqp *lisBAR = (VTListeqp*)(redes->LisBarra());
	lisBAR->Ordena();
	// destroi lista
	delete lisTMP;
}

// ---------------------------------------------------------------------------
void __fastcall TBatchIG::RedefineId(TList *lisEQP)
{ // DVK 2016.04.12
	// variáveis locais
	VTEqpto *eqpto;
	VTCarga *carga;

	// loop p/ todos Eqptos
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		eqpto->Status[sttNOVO] = true;
		eqpto->Id = FalsoId();
		// if (eqpto->StatusNovo()) eqpto->Id = FalsoId();
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TBatchIG::RedefineIdCurvaPropria(TList *lisTMP)
 {//DVK 2016.04.13
 //variaveis locais
 VTCarga      *carga;
 VTEqpto      *eqpto;
 VTGerador    *gerador;
 VTSuprimento *suprimento;

 for(int n = 0; n < lisTMP->Count; n++)
 {
 eqpto = (VTEqpto*)lisTMP->Items[n];
 //verifica se é carga com curva própria
 if(eqpto->Tipo() == eqptoCARGA)
 {
 carga = (VTCarga*)eqpto;
 if(! carga->curva->Tipica)
 {//redefine Id da curva própria
 carga->curva->Status[sttNOVO] = true;
 carga->curva->Id = FalsoId();
 continue;
 }
 }
 //verifica se é suprimento
 if(eqpto->Tipo() == eqptoSUPRIMENTO)
 {
 suprimento = (VTSuprimento*)eqpto;
 //redefine Id da curva própria
 suprimento->curva->Status[sttNOVO] = true;
 suprimento->curva->Id = FalsoId();
 continue;
 }
 //verifica se é gerador
 if(eqpto->Tipo() == eqptoGERADOR)
 {
 gerador = (VTGerador*)eqpto;
 //redefine Id da curva própria
 gerador->curva->Status[sttNOVO] = true;
 gerador->curva->Id = FalsoId();
 continue;
 }
 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TBatchIG::RedefineIdCurvaPropria(TList *lisTMP)
{ // DVK 2016.04.13
	// variaveis locais
	VTCapacitor *capacitor;
	VTCarga *carga;
	VTCurva *curva;
	VTEqpto *eqpto;
	VTGerador *gerador;
	VTSuprimento *suprimento;

	for (int n = 0; n < lisTMP->Count; n++)
	{
		eqpto = (VTEqpto*)lisTMP->Items[n];
		switch (eqpto->Tipo())
		{
		case eqptoCARGA:
			// verifica se é carga com curva própria
			carga = (VTCarga*)eqpto;
			if (!carga->curva->Tipica)
				curva = carga->curva;
			else
				curva = NULL;
			break;
		case eqptoSUPRIMENTO:
			// verifica se é suprimento
			suprimento = (VTSuprimento*)eqpto;
			curva = suprimento->curva;
			break;
		case eqptoGERADOR:
			// verifica se é gerador com curva própria
			gerador = (VTGerador*)eqpto;
			if (!gerador->curva->Tipica)
				curva = gerador->curva;
			else
				curva = NULL;
			break;
		case eqptoCAPACITOR:
			// verifica se é capacitor com curva própria
			capacitor = (VTCapacitor*)eqpto;
			if (!capacitor->curva->Tipica)
				curva = capacitor->curva;
			else
				curva = NULL;
			break;
		}
		if (curva != NULL)
		{ // redefine status e Id da curva própria
			curva->Status[sttNOVO] = true;
			curva->Id = FalsoId();
		}
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TBatchIG::SalvaRedeNaBase(TDateTime data)
{
	// variáveis locais
	// AnsiString arq_rede;
	VTRedutor *redutor;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTSalva *salva = (VTSalva*)apl->GetObject(__classid(VTSalva));

	// verifica Abortar pedida pelo usuário
	// if (Abortar) return(false);
	// atualiza LBoxLog
	LBoxLog->Items->Add("> Preparando redes equivalentes");
	LBoxLog->Refresh();
	try
	{ // cria objetos
		if ((redutor = DLL_NewObjRedutor(apl)) == NULL)
			return (false);
		// proteção
		if (!salva)
			apl->Add(salva = DLL_NewObjSalva(NULL, apl));
		// configura mesnagens redutor
		redutor->ProgressoEnabled = false;
		redutor->AvisoEnabled = false;
		// monta redes reduzidas
		// if (! redutor->ExecutaReducaoMNet()) return(false);
		// DVK 2016.04.06 salva mesmo se não conseguir montar equivalentes
		redutor->ExecutaReducaoMNet();
		// destrói objeto VTRedutor
		delete redutor;
		// verifica se deve montar uma base única
		if (BaseUnica)
		{ // pergunta se o usuário deseja gravar as novas redes na base atualmente selecionada
			if (!usuario_confirmou_base_unica)
			{
				switch (Confirma("Base atual: " + path->ArqRede,
					"Deseja salvar as novas redes na base atual ?"))
				{
				case ID_YES:
					arq_rede = path->ArqRede;
					break;
				case ID_NO:
					// define um diretório onde as redes serão salvas //DVK 2016.11.23
					if (!DefineDiretorioRede(PD.dir_rede))
						return (false);
					// define nome da base da rede
					if (nome_base.IsEmpty())
						arq_rede.sprintf("RedeCompleta_%4d.%02d.mdb", YearOf(data), MonthOf(data));
					else
						arq_rede = nome_base + ".mdb";
					arq_rede = DirRede + "\\" + arq_rede;
					break;
				case ID_CANCEL:
					return (false);
				default:
					return (false);
				}
			}
			// atualiza informação de que usuário já confirmou a base única
			usuario_confirmou_base_unica = true;
			// atualiza log
			LBoxLog->Items->Add("> Gravando base da rede");
			LBoxLog->Refresh();
			// verifica se já existe a base de dados
			if (FileExists(arq_rede))
			{ // DVK 2016.04.12 redefine todos IDs
				RedefineId();
				path->ArqRede = arq_rede;
				// atualiza base de dados
				salva->SalvaRede();
			}
			else
			{ // cria a nova base de dados Access
				if (!CriaBaseAccessDaRede(arq_rede))
					return (false);
				// atualiza base de dados
				salva->SalvaRedeComo();
			}
		}
		else
		{ // atualiza log
			LBoxLog->Items->Add("> Gravando base da rede");
			LBoxLog->Refresh();
			// define nome da base da rede com o nome da Subestação
			if (nome_base.IsEmpty())
			{
				arq_rede = CodigoRedeSupridora();
				arq_rede.sprintf("Rede%s_%4d.%02d.mdb", arq_rede.c_str(), YearOf(data),
					MonthOf(data));
			}
			else
			{
				arq_rede = nome_base + ".mdb";
			}
			if (DirRede.IsEmpty())
				DefineDiretorioRede(PD.dir_rede);
			arq_rede = DirRede + "\\" + arq_rede;
			// cria uma nova base de dados Access
			if (!CriaBaseAccessDaRede(arq_rede))
				return (false);
			// atualiza base de dados
			salva->SalvaRedeComo();
		}
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// -----------------------------------------------------------------------------
// eof
