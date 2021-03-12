// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormNIX.h"
#include "VTInterfaceNIX.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Edita\VTEdita.h"
#include "..\Flow\VTFlow.h"
#include "..\Radial\VTMalha.h"
#include "..\Radial\VTMalhas.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTrafo.h"
#include "..\..\DLL_Inc\Radial.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Progresso.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormNIX::TFormNIX(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
	: TForm(Owner)
{
	// salva ponteiro dos objetos
	this->apl = apl_owner;
	if (parent)
		Parent = parent;
	// cria objetos locais
	nix = NewObjInterfaceNIX(apl_owner);
	// gera dados para o NIX
	// ActionGeraDados->Execute(); //FKM 2016.04.27
	index_patamar = -1;
	index_pat_usado = -1;
	fdA4 = 1.;
	fdTrafo = 1.;

}

// ---------------------------------------------------------------------------
__fastcall TFormNIX::~TFormNIX(void)
{
	// destrói objetos
	if (nix)
	{
		delete nix;
		nix = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormNIX::ActionGeraDadosExecute(TObject *Sender)
{
	// variaveis locais
	VTFlow *flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTProgresso *progresso = DLL_NewObjProgresso(this, NULL);

	// salva qual patamar foi usado
	index_pat_usado = index_patamar;

	if (ContemMalha())
	{
		Aviso("Rede contém malha!");
	}
	// else if (PrimarioSemTrafoAberto())
	// {
	// Aviso("A rede aberta no Sinap está com a carga concentrada no primário."
	// "\nAssim, as potências dos transformadores exportadas para o NIX não serão as "
	// "nominais."
	// "\nSerá exportado um valor comercial próximo da demanda das cargas."
	// "\n"
	// "\n"
	// "\nCaso deseje o valor de potência nominal, será necessário utilizar a rede "
	// "com secundário completo,"
	// "\nou com a rede secundária concentrada no secundário "
	// "dos transformadores de MT/BT.");
	// }
	else if (ExisteRedeBTEquivalente())
	{
		Aviso("Existem redes SDBT não carregadas, não é possível exportá-las corretamente para o NIX."
			);
	}
	else
	{
		if (PrimarioSemTrafoAberto())
		{
			Aviso("A rede aberta no Sinap está com a carga concentrada no primário."
				"\nAssim, as potências dos transformadores exportadas para o NIX não serão as " "nominais."
				"\nSerá exportado um valor comercial próximo da demanda das cargas." "\n" "\n"
				"\nCaso deseje o valor de potência nominal, será necessário utilizar a rede " "com secundário completo,"
				"\nou com a rede secundária concentrada no secundário " "dos transformadores de MT/BT."
				);
		}
		if (ValidaInputForm())
		{ // configura o interface nix
			ConfiguraInterfaceNix();
			// se necessário, calcula fluxo
			if (!(flow->UpToDate))
				flow->CalculaPorRede(redes);
			// prepara o richedit pra update
			SendMessage(RichEdit->Handle, WM_SETREDRAW, false, 0);
			// gera dados para NIX
			if (progresso)
				progresso->Start(progDEFAULT);
			nix->ExportaArqNix(RichEdit->Lines);
			// redesenha o richedit
			SendMessage(RichEdit->Handle, WM_SETREDRAW, true, 0);
			RichEdit->Refresh();
			Application->ProcessMessages();
			if (progresso)
				progresso->Stop();
		}
	}
	if (progresso)
		delete progresso;
}

// ---------------------------------------------------------------------------
void __fastcall TFormNIX::ActionHelpExecute(TObject *Sender)
{
	// variaveis locais
	TForm *FormHelp;

	FormHelp = DLL_NewFormHelp(this, apl, "Interface_NIX");
	if (FormHelp)
		FormHelp->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormNIX::ActionImportaDadosExecute(TObject *Sender)
{

	// Variáveis locais
	VTEdita* edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	VTRedes* redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede* rede;
	TList* lisChaves;
	TList* lisTNixPR;
	TList* lisChavesAlterar;
	TStringList* linhasArquivoNix;
	TStringList* linhaNix;
	AnsiString arquivo = "";
	AnsiString nixALstr;

	IniciaMapaParametro();

	//
	// 1 - Ler arquivo;
	// reinicia
	OpenDialog->FileName = arquivo;
	// mostra a janela de salvar acima das outras
	Application->ModalPopupMode = pmAuto;
	// exibe OpenTextFileDialog
	OpenDialog->Execute();
	// valida seleção
	if (OpenDialog->Files->Count == 0)
	{
		Aviso("Selecione um arquivo");
		return;
	}
	// identifica caminho do arquivo
	arquivo = OpenDialog->FileName;
	linhasArquivoNix = new TStringList();
	linhasArquivoNix->LoadFromFile(arquivo);

	// verifica se o arquivo está vazio
	if (linhasArquivoNix->Count == 0)
	{
		Aviso("O arquivo selecionado está vazio");
		delete linhasArquivoNix;
		return;
	}

	//
	// 2 - Pegar o código do Alimentador;
	linhaNix = new TStringList();
	ExtraiStrings(linhasArquivoNix->Strings[3], ";", linhaNix);
	nixALstr = linhaNix->Strings[1];

	// 3 - Pegar o código Alimentador e ver se existe uma VTRede com esse código. Se não houver, retornar erro;
	for (int i = 0; (i < redes->LisRede()->Count); i++)
	{
		rede = (VTRede*)redes->LisRede()->Items[i];
		if (nixALstr.AnsiCompare(rede->Codigo) == 0)
		{
			break;
		}
	}

	if (rede == NULL || rede->Codigo.AnsiCompare(nixALstr) != 0)
	{
		Aviso("Erro: Não foi encontrada nenhum Alimentador para o código: " + nixALstr);
		delete linhaNix;
		delete linhasArquivoNix;
		return;
	}

	//
	// 4 - Criar uma lista de chaves a partir do objeto VTRede;
	lisChaves = new TList();
	rede->LisEqpto(lisChaves, eqptoCHAVE);

	//
	// 5 - Loop por todos os registros do ArquivoNIX;
	VTChave* chave;
	VTChave* clone;
	VTChave* chavePU;

	// cria listas
	// lisTNixPR = new TList();
	lisChavesAlterar = new TList();

	for (int i = 4; (i < linhasArquivoNix->Count); i++)
	{
		ExtraiStrings(linhasArquivoNix->Strings[i], ";", linhaNix);
		AnsiString codigoSetor = linhaNix->Strings[0];
		if (codigoSetor.AnsiCompare("PR") == 0)
		{
			AnsiString codigoChaveNix = linhaNix->Strings[1];
			codigoChaveNix = codigoChaveNix.SubString(4, codigoChaveNix.Pos("(") - 4);
			for (int j = 0; j < lisChaves->Count; j++)
			{

				chave = (VTChave*)lisChaves->Items[j];

				//
				// 6 - Comparar cada variável CODIGO, para ver se existe dentro de alguma Chave da lista criada;
				if (codigoChaveNix.AnsiCompare(chave->Codigo) == 0 && chave->Codigo.AnsiCompare
					("FU") != 0)
				{
					//
					// 7 - Caso exista, então setar a variável EloFusivel de VTChave, com o valor de ELO de TNixPR;
					clone = chave->Clone();
					chave->Obj = clone;
					clone->EloFusivel = linhaNix->Strings[6];
					lisChavesAlterar->Add(chave);
					break;
				}
			}
		}
		else if (codigoSetor.AnsiCompare("PU") == 0)
		{
			AnsiString codigoChaveNix = linhaNix->Strings[1];
			AnsiString comecoCodigo = codigoChaveNix.SubString(0, 2);

			if (comecoCodigo.AnsiCompare("BF") == 0)
			{
				codigoChaveNix = codigoChaveNix.SubString(4, codigoChaveNix.Pos("(") - 4);
				AnsiString codigoEloPU = linhaNix->Strings[10];
				for (int j = 0; j < lisChavesAlterar->Count; j++)
				{
					chavePU = (VTChave*)lisChavesAlterar->Items[j];
					if (chavePU->Codigo.AnsiCompare(codigoChaveNix) == 0)
					{

						std::map<int, AnsiString>::iterator i;

						// 1- O parâmetro do find nem sempre vai ser um inteiro.
						try
						{
							i = mapa.find(StrToInt(codigoEloPU));
						}
						catch (Exception& e)
						{
							continue;
						}

						// 2- Checar se o iterator é null ANTES de botar ele na String
						std::map<int, AnsiString> test;
						if (i == test.end())
						{
							continue;
						}

						// 3- Checar se o valor é null
						AnsiString eloOficialPU = (AnsiString)i->second;
						if (eloOficialPU == NULL)
						{
							continue;
						}
						clone = (VTChave*)chavePU->Obj;
						clone->EloFusivel = eloOficialPU;

					}
				}
			}
		}
	}

	//
	// 8 - Salvar estado da rede;
	try
	{
		edita->AlteraLisEqpto(lisChavesAlterar);
		Aviso("Arquivo NIX importado com sucesso!");
	}
	catch (Exception &e)
	{
		Aviso("Erro ao alterar lista de equipamentos da Rede");
	}

	//
	// Limpa objetos
	if (lisChaves)
	{
		delete lisChaves;
		lisChaves = NULL;
	}
	// if (lisTNixPR)
	// {
	// LimpaTList(lisTNixPR);
	// delete lisTNixPR;
	// lisTNixPR = NULL;
	// }
	if (lisChavesAlterar)
	{
		delete lisChavesAlterar;
		lisChavesAlterar = NULL;
	}
	if (linhasArquivoNix)
	{
		delete linhasArquivoNix;
		linhasArquivoNix = NULL;
	}
	if (linhaNix)
	{
		delete linhaNix;
		linhaNix = NULL;
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormNIX::ActionSalvaArquivoExecute(TObject *Sender)
{
	// varia´veis locais
	AnsiString filename;
	int count = 1;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	AnsiString filepath;
	AnsiString aux = "";
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;
	AnsiString str_patamar;

	// verifica se o saveDialog tem um caminho
	if (SaveDialog->InitialDir.IsEmpty())
	{
		SaveDialog->InitialDir = path->DirExporta();
	}
	// cria um nome de arquivo
	while (FileExists(filepath = path->DirExporta() + "\\" + ExtractFileName(path->ArqRede) + "_" +
		IntToStr(count++) + ".nix"));
	// caso tenha sido selecionado um patamar
	if (index_pat_usado >= 0)
	{
		patamar = patamares->Patamar[index_pat_usado];
		if (patamar != NULL)
		{
			str_patamar = StringReplace(patamar->Nome, ":", ".", TReplaceFlags() << rfReplaceAll);
			aux = "[" + str_patamar + "]";
		}

		filename = aux + ExtractFileName(filepath);
	}
	else
	{
		filename = "[Máx]" + ExtractFileName(filepath);
	}
	// seta um nome
	SaveDialog->FileName = filename;
	if (SaveDialog->Execute())
	{ // salva dados do RichEdit no arquivo
		RichEdit->Lines->SaveToFile(SaveDialog->FileName);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormNIX::cboxIAlClick(TObject *Sender)
{
	if (cboxIAl->Checked)
	{
		clboxPatamar->Enabled = true;
		clboxPatamarClick(Sender);
	}
	else
	{
		clboxPatamar->Enabled = false;
		clboxPatamarClick(Sender);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormNIX::cboxFDa4Click(TObject *Sender)
{
	if (cboxFDa4->Checked)
	{
		editFDa4->Enabled = true;
		editFDa4->Visible = true;
		editFDa4->SetFocus();
	}
	else
	{
		editFDa4->Enabled = false;
		editFDa4->Visible = false;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFormNIX::cboxFDbtClick(TObject *Sender)
{
	if (cboxFDbt->Checked)
	{
		editFDbt->Enabled = true;
		editFDbt->Visible = true;
		editFDbt->SetFocus();
	}
	else
	{
		editFDbt->Enabled = false;
		editFDbt->Visible = false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormNIX::clboxPatamarClick(TObject *Sender)
{
	if (clboxPatamar->Checked)
	{
		if (!IniciaCBoxPatamar())
		{
			DesabilitaCombo();
		}
	}
	else
	{
		DesabilitaCombo();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormNIX::comboPatamarChange(TObject *Sender)
{ // variaveis locais
	VTPatamar *patamar;
	int indice;

	indice = comboPatamar->ItemIndex;
	// seleciona o objeto
	patamar = (VTPatamar*) comboPatamar->Items->Objects[indice];
	if (patamar != NULL)
	{
		index_patamar = patamar->Index;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormNIX::ContemMalha(void)
{
	VTMalhas *malhas = DLL_NewObjMalhas(apl);
	bool contemmalha = false;

	// Verifica se tem malhas, preenchendo as listas de malha abaixo
	malhas->Inicia();
	int malhaExterna = malhas->LisMalhaExterna()->Count;
	int malhaGeracao = malhas->LisMalhaGeracao()->Count;
	int malhaInterna = malhas->LisMalhaInterna()->Count;
	// Se as listas forem preenchidas significa que existem malhas
	if (((malhaExterna > 0) || (malhaGeracao > 0)) || (malhaInterna > 0))
	{
		// Verifica se existem somente malhas MT de trafos nas malhas internas
		if ((malhaExterna == 0) && (malhaGeracao == 0))
		{
			// Verifica se possui pelo menos uma malha em rede MT
			if (ContemMalhaMT(malhas))
			{
				// Se houver malha em rede MT, verifica se é o caso particular
				// de só haver malhas em trafos MT/BT
				if (SomenteMalhaDeTrafo(malhas))
				{
					// se realmente só existirem malhas de trafos, o programa pode prosseguir
					// ou seja, é como se não houvessem malhas, portanto contemmalha recebe falso
					contemmalha = false;
					return contemmalha;
				}
				// Se possui malha em rede MT e não é o caso particular,
				// então existe malhas e o programa não deve continuar
				contemmalha = true;
				return contemmalha;
			}
			// se ele não possui malhas MT então não há problemas
			// malhas em transmissão, BT, etc não causam problemas pro NIX
			// Por isso, contemmalha recebe falso
			contemmalha = false;
			return contemmalha;
		}
		// se existem malhas e a condição "SomenteMalhaDeTrafo" não for atendida
		// então existem malhas e o programa deve parar
		contemmalha = true;
	}
	// se malhaExterna, malhaGeracao e malhaInterna forem igual a zero, não há malhas
	else
	{
		contemmalha = false;
	}

	delete malhas;
	return contemmalha;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormNIX::ContemMalhaMT(VTMalhas *malhas)
{
	TList *lisLigacao;
	TList *lisMalhaInterna = malhas->LisMalhaInterna();
	VTLigacao *ligacao;
	VTMalha *malha;
	VTTipoRede *tipoRede;

	for (int i = 0; i < lisMalhaInterna->Count; i++)
	{
		// pega a i-ésima malha da lista de malhas internas
		malha = (VTMalha*) lisMalhaInterna->Items[i];
		// pega a lista de ligações da i-ésima malha
		lisLigacao = malha->LisLigacao();
		// percorre a lista de ligações
		for (int j = 0; j < lisLigacao->Count; j++)
		{
			// pega a j-ésima ligação da lista de ligações analisada
			ligacao = (VTLigacao*) lisLigacao->Items[j];
			// pega o tipo de rede da ligação analisada
			tipoRede = ligacao->rede->TipoRede;
			// verifica se não é nulo
			if (tipoRede != NULL)
			{
				// verifica se o tipo de rede é primário (MT), se for, retorna true
				if (tipoRede->Segmento == redePRI)
				{
					// basta que encontre uma rede MT para parar de procurar e
					// terminar o código
					return true;
				}
			}
		}
	}
	// se o código chegar até aqui é porque não encontrou nenhuma rede MT
	// ou seja, ele não contém malhas em redes MT
	return false;
}

// ---------------------------------------------------------------------------
void __fastcall TFormNIX::ConfiguraInterfaceNix(void)
{
	if (nix != NULL)
	{
		// configura se deve inserir valor de corrente nos elementos de protecao
		nix->is_PR_ComCorrente = cboxIChave->Checked;
		// configura se deve inserir valor de corrente nos disjuntores do alimentador
		nix->is_AL_ComCorrente = cboxIAl->Checked;
		// se a opcao de escolher o patamar estiver marcada
		if (clboxPatamar->Checked)
		{ // configura qual o indice/patamar
			nix->Patamar = index_patamar;
		}
		// se a opcao de forçar FD estiver marcada
		nix->is_FD_UnicoTrafo = cboxFDbt->Checked;
		if (nix->is_FD_UnicoTrafo)
		{ // configura qual valor
			nix->FD_Trafo = fdTrafo;
		}
		// se a opcao de forçar FD estiver marcada
		nix->is_FD_UnicoA4 = cboxFDa4->Checked;
		if (nix->is_FD_UnicoA4)
		{ // configura qual valor
			nix->FD_A4 = fdA4;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormNIX::DesabilitaCombo(void)
{
	comboPatamar->Clear();
	comboPatamar->Text = "Patamar de demanda máxima";
	comboPatamar->Enabled = false;
	index_patamar = -1;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormNIX::ExisteRedeBTEquivalente(void)
{ // variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTRede *rede;
	VTTipoRede *tipoRede;

	// varre todas as redes à procura de redes bt reduzidas
	for (int nr = 0; nr < redes->LisRede()->Count; nr++)
	{
		rede = (VTRede*)redes->LisRede()->Items[nr];
		tipoRede = rede->TipoRede;
		if (tipoRede != NULL)
		{ // se a rede for redeSEC
			if (tipoRede->Segmento == redeSEC)
			{ // caso seja rede equivalente
				if (!rede->Carregada)
				{
					return (true);
				}
			}
		}
	}
	// caso tenha varrido todas e não encontrado
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormNIX::IniciaCBoxPatamar(void)
{ // variaveis locais
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTPatamar *patamar;
	TList *lisPAT = new TList;
	bool sucesso = false;

	;
	// preenche o combo com todos patamares disponivels
	patamares->LisPatamarHabilitado(lisPAT);
	// caso tenha pelo menos 1 patamar
	if (lisPAT->Count > 0)
	{
		// limpa  combo
		comboPatamar->Clear();
		for (int np = 0; np < lisPAT->Count; np++)
		{
			patamar = (VTPatamar*) lisPAT->Items[np];
			// adiciona o nome e ponteiro
			comboPatamar->AddItem(patamar->Nome, patamar);
		}
		// seleciona o primeior
		if (comboPatamar->Items->Count > 0)
		{
			comboPatamar->ItemIndex = 0;
			// habilita o combo
			comboPatamar->Enabled = true;
			// salva o index do patamar
			index_patamar = ((VTPatamar*) comboPatamar->Items->Objects[0])->Index;
			sucesso = true;
		}
	}
	return sucesso;
}

// ---------------------------------------------------------------------------
void __fastcall TFormNIX::IniciaMapaParametro(void)
{
	mapa[1] = "6T";
	mapa[2] = "6K";
	mapa[3] = "10K";
	mapa[10] = "10K";
	mapa[12] = "10T";
	mapa[13] = "15K";
	mapa[14] = "15T";
	mapa[17] = "25K";
	mapa[18] = "25T";
	mapa[19] = "40T";
	mapa[20] = "40K";
	mapa[21] = "65K";
	mapa[22] = "65T";
	mapa[23] = "8T";
	mapa[24] = "8K";
	mapa[52] = "100K";
	mapa[53] = "100T";
	mapa[72] = "12T";
	mapa[86] = "140T";
	mapa[88] = "140K";
	mapa[112] = "20T";
	mapa[113] = "20K";
	mapa[114] = "200K";
	mapa[115] = "200T";
	mapa[130] = "30T";
	mapa[131] = "30K";
	mapa[151] = "50T";
	mapa[152] = "50K";
	mapa[183] = "80T";
	mapa[184] = "80K";
	mapa[559] = "1H";
	mapa[561] = "2H";
	mapa[562] = "3H";
	mapa[563] = "5H";
	mapa[577] = "12K";
	mapa[618] = "0,5H";
	mapa[1076] = "0,25H";
	mapa[1077] = "0,75H";
	mapa[1078] = "1,25H";
	mapa[1079] = "1,75H";
	mapa[1137] = "6K";
	mapa[1138] = "8K";
	mapa[1139] = "10K";
	mapa[1140] = "12K";
	mapa[1141] = "15K";
	mapa[1142] = "20K";
	mapa[1143] = "25K";
	mapa[1144] = "30K";
	mapa[1145] = "40K";
	mapa[1146] = "50K";
	mapa[1147] = "65K";
	mapa[1148] = "80K";
	mapa[1149] = "100K";
	mapa[1150] = "140K";
	mapa[1151] = "200K";
	mapa[1152] = "6T";
	mapa[1153] = "8T";
	mapa[1154] = "10T";
	mapa[1155] = "12T";
	mapa[1156] = "15T";
	mapa[1157] = "20T";
	mapa[1158] = "25T";
	mapa[1159] = "30T";
	mapa[1160] = "40T";
	mapa[1161] = "50T";
	mapa[1162] = "65T";
	mapa[1163] = "80T";
	mapa[1164] = "100T";
	mapa[1165] = "140T";
	mapa[1166] = "200T";
}

// ---------------------------------------------------------------------------
/*
 void  __fastcall TFormNIX::IniciaForm(void)
 {
 if (nix != NULL)
 {
 //configura se deve inserir valor de corrente nos elementos de protecao
 cboxIChave->Checked = nix->is_PR_ComCorrente;
 //configura se deve inserir valor de corrente nos disjuntores do alimentador
 cboxIAl->Checked = nix->is_AL_ComCorrente;
 //se a opcao de escolher o patamar estiver marcada
 if(nix->Patamar == -1)
 {
 clboxPatamar->Checked = false;
 }
 else
 {
 clboxPatamar->Checked = true;
 clboxPatamarClick(NULL);
 }
 if(clboxPatamar->Checked)
 {//configura qual o indice/patamar
 nix->Patamar = index_patamar;
 }
 //se a opcao de forçar FD estiver marcada
 cboxFDbt->Checked = nix->is_FD_UnicoTrafo;
 editFDbt->Text = DoubleToStr("%3.2f",nix->FD_Trafo);
 //se a opcao de forçar FD estiver marcada
 cboxFDa4->Checked = nix->is_FD_UnicoA4;
 editFDa4->Text = DoubleToStr("%3.2f",nix->FD_A4);
 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TFormNIX::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destrói o Form
	Action = caFree;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormNIX::PrimarioSemTrafoAberto(void)
{
	// variáveis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTTipoRede *tipoRede;
	VTTrafo *trafo;
	TList *lisTrafo = new TList();

	// preenche a lista lisTrafo com todos os trafos da rede
	redes->LisLigacao(lisTrafo, eqptoTRAFO);

	// percorre a lista de trafos
	for (int ntr = 0; ntr < lisTrafo->Count; ntr++)
	{
		trafo = (VTTrafo*)lisTrafo->Items[ntr];
		// para cada trafo, verifica qual a rede que ele pertence
		tipoRede = trafo->rede->TipoRede;
		// proteção caso o tipo de rede seja NULL
		if (tipoRede == NULL)
		{
			continue;
		}
		// caso exista um trafo aberto na rede secundária, a rede aberta estará completa
		if (tipoRede->Segmento == redeSEC)
		{
			delete lisTrafo;
			return (false);
		}
		// caso exista um trafo aberto na rede primária, a rede aberta estará com trafos MT/BT
		if (tipoRede->Segmento == redePRI)
		{
			delete lisTrafo;
			return (false);
		}
	}
	delete lisTrafo;
	return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormNIX::SomenteMalhaDeTrafo(VTMalhas *malhas)
{
	// esse método irá verificar se está ocorrendo um problema em específico, que é
	// quando existe um banco de trafo monofásicos MT/BT e o Sinap indica como malha.
	// Ou seja, ele irá verificar se a malha consiste apenas de duas barras e n trafos //DSF
	VTMalha *malha;
	TList *lisMalhaInterna = malhas->LisMalhaInterna();
	TList *lisbarra, *lisligacao;
	VTLigacao *ligacao;

	// Procura nas malhas se existem malhas de trafos
	for (int i = 0; i < lisMalhaInterna->Count; i++)
	{
		// pega a i-ésima malha na lista de malhas internas
		malha = (VTMalha*) lisMalhaInterna->Items[i];
		// verifica se existem apenas duas barras nessa malha
		lisbarra = malha->LisBarra();
		if (lisbarra->Count == 2)
		{
			// irá verificar cada uma das ligações da malha
			lisligacao = malha->LisLigacao();
			for (int j = 0; j < lisligacao->Count; j++)
			{
				ligacao = (VTLigacao*)lisligacao->Items[j];
				// verifica se todas as ligações são trafos
				if (ligacao->Tipo() == eqptoTRAFO)
				{
					continue;
				}
				else
				{
					// se pelo menos uma delas não for um trafo, não há somente
					// malhas de trafos e portanto retorna falso
					return false;
				}
			}
			continue;
		}
		// se houver uma malha com mais de duas barras, então não há somente
		// malhas de trafos e portanto retorna falso
		return false;
	}
	// se ele chegou até aqui é porque ele somente encontrou malhas
	// de duas barras e entre essas duas barras só existem trafos
	// da pra concluir portanto que somente existem malhas de trafos
	// Assim o Sinap pode continuar sem problemas
	return true;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormNIX::ValidaFatorDemandaA4(void)
{ // variaveis locais
	bool sucesso = true;
	AnsiString string;

	string = editFDa4->Text;
	if (!StrToDouble(string, fdA4))
	{
		Aviso("Valor de Fator de Demanda inválido.");
		sucesso = false;
	}
	return sucesso;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormNIX::ValidaFatorDemandaTrafo(void)
{ // variaveis locais
	bool sucesso = true;
	AnsiString string;

	string = editFDbt->Text;
	if (!StrToDouble(string, fdTrafo))
	{
		Aviso("Valor de Fator de Demanda inválido.");
		sucesso = false;
	}
	return sucesso;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormNIX::ValidaInputForm(void)
{ // variaveis locais
	bool sucesso = true;

	try
	{
		// se for forçar FD
		if (cboxFDbt->Checked)
		{ // valida os valores de demanda
			if (!ValidaFatorDemandaTrafo())
			{
				sucesso = false;
			}
		}
		// se for forçar FD
		if (cboxFDa4->Checked)
		{ // valida os valores de demanda
			if (!ValidaFatorDemandaA4())
			{
				sucesso = false;
			}
		}
	}
	catch (Exception &e)
	{
		sucesso = false;
	}
	return sucesso;
}

// ---------------------------------------------------------------------------
// eof
void __fastcall TFormNIX::editFDbtMouseActivate(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y, int HitTest, TMouseActivate &MouseActivate)

{
	editFDbt->SetFocus();
}
// ---------------------------------------------------------------------------

void __fastcall TFormNIX::editFDa4MouseActivate(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y, int HitTest, TMouseActivate &MouseActivate)

{
	editFDa4->SetFocus();
}
// ---------------------------------------------------------------------------
