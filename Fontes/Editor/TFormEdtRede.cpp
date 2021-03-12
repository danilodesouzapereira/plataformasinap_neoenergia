// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtRede.h"
#include "TFormSelPadrao.h"
#include "VTEditor.h"
#include "VTEdt.h"
#include "Tensao\VTTensao.h"
#include "..\Apl\VTApl.h"
#include "..\Edita\VTEdita.h"
#include "..\Forms\TFormEdtTxt.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormEdtRede::TFormEdtRede(TComponent* Owner, VTApl *apl, TAction *ActionOnDone,
	TWinControl *parent) : TForm(Owner)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	this->ActionOnDone = ActionOnDone;
	// insere em seu Parent
	if (parent)
		Parent = parent;
	// cria listas
	lisEQP = new TList();
	lisVIZ = new TList();
	local.lisCLUSTER = new TList();
	local.lisREDE = new TList();
	orig.lisCLUSTER = new TList();
	orig.lisREDE = new TList();
	// inicia listas de Cluster e Rede existentes e cria cópia temporárias
	IniciaLisCluster_Rede();
	// inicia CBoxBarra
	CBoxBarraInicia();
	// inicia TreeView
	TViewRedeInicia();
	// posiciona o Form
	FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
__fastcall TFormEdtRede::~TFormEdtRede(void)
{
	// variáveis locais
	int count = 0;
	VTRede *rede;
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();

	// sinaliza término
	if (ActionOnDone)
		ActionOnDone->OnExecute(this);
	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// destrói obejto EdtBusca
	if (edt_busca)
	{
		delete edt_busca;
		edt_busca = NULL;
	}
	// apaga moldura
	GraficoMoldura();
	// cancela destaque da rede
	GraficoDestacaRede();
	// habilita exibição de todas as redes
	for (int n = 0; n < lisREDE->Count; n++)
	{
		rede = (VTRede*)lisREDE->Items[n];
		if (rede->Carregada)
		{
			rede->Show[eqp_VISIBLE] = true;
			count++;
		}
	}
	// atualiza gráfico
	if (count > 0)
		grafico->Redraw();
	// destrói lista sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	if (lisVIZ)
	{
		delete lisVIZ;
		lisVIZ = NULL;
	}
	// destrói lista sem destruir seus objetos
	if (orig.lisCLUSTER)
	{
		delete orig.lisCLUSTER;
		orig.lisCLUSTER = NULL;
	}
	if (orig.lisREDE)
	{
		delete orig.lisREDE;
		orig.lisREDE = NULL;
	}
	// destrói lista e seus objetos
	if (local.lisCLUSTER)
	{
		LimpaTList(local.lisCLUSTER);
		delete local.lisCLUSTER;
		local.lisCLUSTER = NULL;
	}
	if (local.lisREDE)
	{
		LimpaTList(local.lisREDE);
		delete local.lisREDE;
		local.lisREDE = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::ActionAtributoAlteradoExecute(TObject *Sender)
{
	// variáveis locais
	VTRede *rede, *rede_alt;

	// verifica se há uma Rede selecionada
	if ((rede = HaRedeSelecionada()) == NULL)
		return;
	// atualiza atributos da Rede
	rede->Estatica = checkEstatica->Checked;
	rede->Radial = checkRadial->Checked;
	// marca que a rede foi alterada
	rede->Status[sttALTERADO] = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::ActionCancelaExecute(TObject *Sender)
{
	// fecha o form
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::ActionConfirmaExecute(TObject *Sender)
{
	// variáveis locais
	VTCluster *cluster, *cluster_orig;
	VTEstudo *estudo;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTRede *rede;
	TList *lisREDE;

	// verifica se está no Planejamento
	estudo = planejamento->ExisteEstudo();
	if (estudo->Tipo == estudoPLANEJAMENTO)
	{//verifica se alguma rede BT foi inserida
		if (ExisteRedeBTnova())
		{
			Aviso("Edição de Rede BT bloqueada no Planejamento");
			return;
		}
	}
	// finaliza eventual edição de TreeNode não terminada c/ Enter
	FinalizaEdicaoTreeNodeText();
	// verifica se as Redes radiais tem barra inicial
	if (!ValidaBarraInicialRedeRadial())
		return;
	// atualiza cópias das Redes p/ apontar p/ Cluster original
	for (int n = 0; n < local.lisCLUSTER->Count; n++)
	{
		cluster = (VTCluster*)local.lisCLUSTER->Items[n];
		cluster_orig = (VTCluster*)ExisteEqpto(orig.lisCLUSTER, cluster->Id);
		if (cluster_orig != NULL)
		{ // atualiza Redes do Cluster
			lisREDE = cluster->LisRede();
			for (int nr = 0; nr < lisREDE->Count; nr++)
			{
				rede = (VTRede*)lisREDE->Items[nr];
				rede->Cluster = cluster_orig;
			}
		}
		// IMPORTANTE: limpa listas do Cluster local
		cluster->Reinicia();
	}
	// retira Redes e Clusters excluídos
	EqptoExcluidoRetira(local.lisREDE, orig.lisREDE);
	EqptoExcluidoRetira(local.lisCLUSTER, orig.lisCLUSTER);
	// insere Clusters e Redes novos
	EqptoNovoInsere(local.lisCLUSTER, orig.lisCLUSTER);
	EqptoNovoInsere(local.lisREDE, orig.lisREDE);
	// atualiza Cluster e Redes alterados
	EqptoAlteradoAtualiza(local.lisCLUSTER, orig.lisCLUSTER);
	EqptoAlteradoAtualiza(local.lisREDE, orig.lisREDE);
	// fecha o form
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::ActionEdtBarraExecute(TObject *Sender)
{
	// variáveis locais
	VTEditor *editor = (VTEditor*)apl->GetObject(__classid(VTEditor));

	// habilita editor de barra
	editor->EditaBarra(NULL, NULL, ActionUsuarioEditouBarra);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::ActionEdtBuscaExecute(TObject *Sender)
{
	try
	{ // cria um novo objeto TEdtBusca
		if (edt_busca != NULL)
			delete edt_busca;
		edt_busca = NewObjEdtBusca(apl, NULL, NULL, ActionUsuarioSelecionouBarra);
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::ActionInsereExecute(TObject *Sender)
{
	// variáveis locais
	TTreeNode *node;

	// determina TreeNode p/ incluir um novo Cluster
	if ((node = IdentificaTreeNodeAgrupadas()) != NULL)
	{ // insere um novo Cluster
		InsereCluster(node);
		return;
	}
	// determina TreeNode de TipoRede
	if ((node = IdentificaTreeNodeTipoRede()) != NULL)
	{ // insere uma nova Rede
		InsereRede(node);
		return;
	}
	// inserção inválida
	Aviso("Selecione 'Agrupadas' para criar um novo agrupamento de redes ou \n"
		"Selecione um tipo de rede para criar uma nova rede");
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::ActionPadraoExecute(TObject *Sender)
{
	/*
	 //variáveis locais
	 int         padrao_id;
	 AnsiString  padrao_cod;
	 TFormSelPadrao *FormSelPadrao;
	 VTRede      *rede;

	 try{//determina Rede selecionada
	 if ((rede = HaRedeSelecionada()) == NULL)
	 {
	 Aviso("Selecione a rede para definir seu padrão");
	 return;
	 }
	 //esconde este Form
	 Visible = false;
	 //cria TFormSelPadrao p/ selecionar um padrão
	 FormSelPadrao = new TFormSelPadrao(this, apl);
	 FormSelPadrao->Inicia(eqptoREDE);
	 if (FormSelPadrao->ShowModal() == mrOk)
	 {//salva padrão selecionado
	 if (FormSelPadrao->PadraoSelecionado(padrao_id, padrao_cod))
	 {//salva padrão da rede
	 rede->Padrao = padrao_cod;
	 //atualiza Editpadrao
	 EditPadrao->Text = padrao_cod;
	 //marca que houve alteração na Redee
	 rede->Status[sttALTERADO] = true;
	 }
	 }
	 //destrói FormSelPadrao
	 delete FormSelPadrao;
	 //torna a exibir este Form
	 Visible = true;
	 }catch(Exception &e)
	 {//nada a fazer
	 }
	 */
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::ActionRetiraExecute(TObject *Sender)
{
	// variáveis locais
	int num_rede;
	TList *lisREDE;
	VTCluster *cluster;
	VTRede *rede;
	VTEqpto *rede_original;

	// verifica se há um Cluster selecionado
	if ((cluster = HaClusterSelecionado()) != NULL)
	{ // marca que todas Redes do Cluster foram alteradas
		lisREDE = cluster->LisRede();
		for (int n = 0; n < lisREDE->Count; n++)
		{
			rede = (VTRede*)lisREDE->Items[n];
			rede->Status[sttALTERADO] = true;
		}
		// determina número de redes do Cluster
		num_rede = cluster->NumeroRedes();
		// retira a Cluster de local.lisCLUSTER e destrói objeto
		local.lisCLUSTER->Remove(cluster);
		delete cluster;
		// verifica se o Cluster continha Redes
		if (num_rede == 0)
		{ // elimina o TreeNode do Cluster
			TViewRede->Selected->Delete();
		}
		else
		{ // reinicia o TreeView p/ apresentar as Redes nos TreeNodes de seus tipos
			TViewRedeInicia();
		}
	}
	// verifica se há uma Rede selecionada
	else if ((rede = HaRedeSelecionada()) != NULL)
	{ // verifica se a Rede pode ser excluída
		if (!ExclusaoPodeSerFeita(rede))
			return;
		// retira Rede de local.lisREDE
		local.lisREDE->Remove(rede);
		// desabilita apresentação da rede original
		if ((rede_original = ExisteEqpto(orig.lisREDE, rede->Id)) != NULL)
		{
			rede_original->Show[eqp_VISIBLE] = false;
		}
		// retira Rede do seu Cluster
		if (rede->Cluster)
			rede->Cluster->RemoveRede(rede);
		// destrói objeto
		delete rede;
		// elimina o TreeNode da Rede
		TViewRede->Selected->Delete();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::ActionSelColorDestaqueExecute(TObject *Sender)
{
	// variáveis locais
	VTRede *rede;

	// esconde este Form
	Hide();
	// apresenta form de seleção de cor
	ColorDialog->Color = PanelColorDestaque->Color;
	if (ColorDialog->Execute())
	{ // salva nova cor de destaque
		PanelColorDestaque->Color = ColorDialog->Color;
		// destaca rede selecionada
		if ((rede = HaRedeSelecionada()) != NULL)
		{
			GraficoDestacaRede(rede);
		}
	}
	// volta a exibir este Form
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::ActionSelColorRedeExecute(TObject *Sender)
{
	// variáveis locais
	VTRede *rede;

	// verifica se há uma Rede selecionada
	if ((rede = HaRedeSelecionada()) == NULL)
		return;
	// esconde este Form
	Hide();
	// apresenta form de seleção de cor
	ColorDialog->Color = rede->Color;
	if (ColorDialog->Execute())
	{ // salva nova cor do circuito
		rede->Color = ColorDialog->Color;
		// marca que a rede foi alterada
		rede->Status[sttALTERADO] = true;
		// atualiza PanelColor
		PanelColorRede->Color = rede->Color;
	}
	// volta a exibir este Form
	Show();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::ActionUsuarioEditouBarraExecute(TObject *Sender)
{
	// atualiza CBoxBarra
	CBoxBarraInicia();
	// exibe Barra inicial da rede selecionada
	ExibeBarraInicialRedeSelecionada();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::ActionUsuarioSelecionouBarraExecute(TObject *Sender)
{
	// variáveis locais
	VTBarra *barra;
	VTRede *rede;
	VTEqpto *eqpto;
	TList *lisEXT = (TList*)Sender;

	// proteção
	if (lisEXT == NULL)
		return;
	if (lisEXT->Count == 0)
		return;
	// determina primeiro Eqpto da lista
	if ((eqpto = (VTEqpto*)lisEXT->First()) == NULL)
		return;
	// verifica se é uma Barra
	if (!eqpto->TipoBarra())
	{
		Aviso("Seleciona barra inicial da rede");
		return;
	}
	// verifica se há uma Rede selecionada
	if ((rede = HaRedeSelecionada()) == NULL)
	{
		Aviso("Não há uma rede selecionada para definir sua barra inicial");
		return;
	}
	barra = (VTBarra*)eqpto;
	// redefine Barra inicial da Rede
	rede->DefineBarraInicial(barra);
	// marca que a rede foi alterada
	rede->Status[sttALTERADO] = true;
	// exibe Barra inicial selecionada
	ExibeBarraInicialRedeSelecionada();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::ActionVnomAlteraExecute(TObject *Sender)
{
	// variáveis locais
	double vnom_kv;
	VTBarra *barra;
	VTRede *rede;
	VTTensao *tensao;

	try
	{ // determina Rede selecionada
		if ((rede = HaRedeSelecionada()) == NULL)
		{
			Aviso("Selecione a rede para redefinir sua tensão");
			return;
		}
		// determina Barra inicial da rede selecionada
		if ((barra = rede->BarraInicial()) == NULL)
		{
			Aviso("Defina a barra inicial da rede");
			return;
		}
		// determina rede original
		if ((rede = (VTRede*)ExisteEqpto(orig.lisREDE, rede->Id)) == NULL)
			return;
		// determina a nova tensão nominal
		if (!LeTensaoNominal(vnom_kv))
		{
			Aviso("Tensão nominal incorreta");
			return;
		}
		// cria objeto para alterar tensão da rede
		tensao = NewObjTensao(apl);
		if (tensao->Executa(rede, barra, vnom_kv))
		{
			Aviso("Tensão nominal dos equipamentos da rede foi alterada corretamente");
		}
		else
		{
			Aviso("Erro na alteração da tensão nominal dos equipamentos da rede");
		}
		// destrói objeto
		delete tensao;
	}
	catch (Exception &e)
	{
		// nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::CBoxBarraChange(TObject *Sender)
{
	// variáveis locais
	VTBarra *barra = NULL;
	VTRede *rede;

	// verifica se há uma Rede selecionada
	if ((rede = HaRedeSelecionada()) == NULL)
		return;
	// determina Barra selecionada
	if (CBoxBarra->ItemIndex >= 0)
	{
		barra = (VTBarra*)(CBoxBarra->Items->Objects[CBoxBarra->ItemIndex]);
	}
	// redefine Barra inicial da Rede
	rede->DefineBarraInicial(barra);
	// marca que a rede foi alterada
	rede->Status[sttALTERADO] = true;
	// pré-seleciona Barra inicial da Rede
	ExibeBarraInicialRedeSelecionada();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::CBoxBarraInicia(void)
{
	// variáveis locais
	AnsiString codigo;
	VTBarra *barra;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisBARRA = redes->LisBarra();

	// reinicia  CBoxBarra
	CBoxBarra->Sorted = false;
	CBoxBarra->Clear();
	for (int n = 0; n < lisBARRA->Count; n++)
	{
		barra = (VTBarra*)lisBARRA->Items[n];
		if (barra->Codigo.IsEmpty())
			codigo.sprintf("id = %d", barra->Id);
		else
			codigo = barra->Codigo;
		CBoxBarra->Items->AddObject(codigo, barra);
	}
	// ordena  CBoxBarra
	CBoxBarra->Sorted = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::CheckBoxColorDestaqueClick(TObject *Sender)
{
	// variáveis locais
	VTRede *rede;

	// exibe/esconde PanelColorDestaque
	PanelColorDestaque->Visible = !CheckBoxColorDestaque->Checked;
	// destaca rede selecionada
	if ((rede = HaRedeSelecionada()) != NULL)
	{
		GraficoDestacaRede(rede);
	}
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormEdtRede::CodigoRede(VTRede *rede)
{
	if (rede->Status[sttNOVO])
		return (rede->Codigo + " (nova)");
	else
		return (rede->Codigo);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormEdtRede::DefineCodigoCluster(void)
{
	int index = 1;
	AnsiString codigo = "";
	TFormEdtTxt *FormEdtTxt;

	// predefine código do Cluster
	do
	{
		codigo = "Grupo_" + IntToStr(index++);
	}
	while (ExisteCluster(codigo));
	try
	{ // solicita que o usuário entre um string
		FormEdtTxt = new TFormEdtTxt(this, "Código do Grupo de Redes");
		FormEdtTxt->Texto = codigo;
		if (FormEdtTxt->ShowModal() == mrOk)
		{ // lê código digitado pelo usuário
			codigo = FormEdtTxt->Texto;
		}
		// destrói  FormEdtTxt
		delete FormEdtTxt;
	}
	catch (Exception &e)
	{
	}
	return (codigo);
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormEdtRede::DefineCodigoRede(void)
{
	int index = 1;
	AnsiString codigo = "";
	TFormEdtTxt *FormEdtTxt;

	// predefine código da Rede
	do
	{
		codigo = "Rede_" + IntToStr(index++);
	}
	while (ExisteRede(codigo));
	try
	{ // solicita que o usuário entre um string
		FormEdtTxt = new TFormEdtTxt(this, "Código da Rede");
		FormEdtTxt->Texto = codigo;
		if (FormEdtTxt->ShowModal() == mrOk)
		{ // lê código digitado pelo usuário
			codigo = FormEdtTxt->Texto;
		}
		// destrói  FormEdtTxt
		delete FormEdtTxt;
	}
	catch (Exception &e)
	{
	}
	return (codigo);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::EqptoAlteradoAtualiza(TList *lisLOCAL, TList *lisORIG)
{
	// variáveis locais
	VTEqpto *eqpto, *eqpto_orig;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// loop p/ todos Eqptos originais
	for (int n = 0; n < lisORIG->Count; n++)
	{
		eqpto_orig = (VTEqpto*)lisORIG->Items[n];
		// determina Eqpto local correspondente
		if ((eqpto = ExisteEqpto(lisLOCAL, eqpto_orig->Id)) == NULL)
			continue;
		// verifica se o Eqpto local foi alterado
		if (eqpto->Status[sttALTERADO])
		{ // retira Eqpto de lisLOCAL
			lisLOCAL->Remove(eqpto);
			// IMPORTANTE: assume mesmo status do Eqpto original
			eqpto->StatusAsUnsigned = eqpto_orig->StatusAsUnsigned;
			// altera Eqpto original
			edita->Altera(eqpto_orig, eqpto);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::EqptoNovoInsere(TList *lisLOCAL, TList *lisORIG)
{
	// variáveis locais
	VTEqpto *eqpto;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// reinicia lisEQP
	lisEQP->Clear();
	// loop p/ todos Eqptos locais
	for (int n = lisLOCAL->Count - 1; n >= 0; n--)
	{
		eqpto = (VTEqpto*)lisLOCAL->Items[n];
		// verifica se o Eqpto local é novo
		if (eqpto->Status[sttNOVO])
		{ // transfere Eqpto novo de lisLOCAL p/ lisEQP
			lisLOCAL->Delete(n);
			lisEQP->Add(eqpto);
		}
	}
	// insere Eqptos novos
	if (lisEQP->Count > 0)
		edita->InsereLisEqpto(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::EqptoExcluidoRetira(TList *lisLOCAL, TList *lisORIG)
{
	// variáveis locais
	VTEqpto *eqpto, *eqpto_orig;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// reinicia lisEQP c/ Eqptos originais que devem ser retirados
	lisEQP->Clear();
	// loop p/ todos Eqptos originais
	for (int n = 0; n < lisORIG->Count; n++)
	{
		eqpto_orig = (VTEqpto*)lisORIG->Items[n];
		// verifica se existe o Eqpto local
		if (ExisteEqpto(lisLOCAL, eqpto_orig->Id) == NULL)
		{ // não existe o Eqpto local: retira o Eqpto original
			lisEQP->Add(eqpto_orig);
		}
	}
	// retira Eqptos originais
	if (lisEQP->Count > 0)
		edita->RetiraLisEqpto(lisEQP);
}

/*
 //---------------------------------------------------------------------------
 bool __fastcall TFormEdtRede::ExclusaoPodeSerFeita(VTCluster *cluster)
 {
 //variáveis locais
 VTRede *rede;
 TList  *lisREDE = cluster->LisRede();

 //verifica se o Cluster está associado a um Cluster original
 //if ((cluster = (VTCluster*)(cluster->Obj)) == NULL) return(true);
 //verifica alguma Rede a ser excluída é equivalente
 for (int n = 0; n < lisREDE->Count; n++)
 {
 rede = (VTRede*)lisREDE->Items[n];
 if (! ExclusaoPodeSerFeita(rede)) return(false);
 }
 return(true);
 }
 */
// ---------------------------------------------------------------------------
bool __fastcall TFormEdtRede::ExclusaoPodeSerFeita(VTRede *rede)
{
	// variáveis locais
	VTRede *rede_viz;

	// verifica se a Rede está associada a uma Rede original
	if ((rede = (VTRede*)(rede->Obj)) == NULL)
		return (true);
	// verifica se Rede é equivalente
	if (!rede->Carregada)
	{
		Aviso("Redes equivalentes não podem ser excluídas");
		return (false);
	}
	// verifica alguma Rede vizinha é equivalente
	lisVIZ->Clear();
	rede->LisRedeVizinha(lisVIZ);
	for (int n = 0; n < lisVIZ->Count; n++)
	{
		rede_viz = (VTRede*)lisVIZ->Items[n];
		if (!rede_viz->Carregada)
		{
			Aviso("Redes que conectam redes equivalentes não podem ser excluídas");
			return (false);
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::ExibeBarraInicialRedeSelecionada(void)
{
	// variáveis locais dex;
	int index = -1;
	VTBarra *barra = NULL;
	VTRede *rede;

	// determina Rede selecionada
	rede = HaRedeSelecionada();
	// pré-seleciona Barra inicial da Rede
	if (rede != NULL)
		barra = rede->BarraInicial();
	if (barra != NULL)
		index = CBoxBarra->Items->IndexOfObject(barra);
	if (CBoxBarra->ItemIndex != index)
		CBoxBarra->ItemIndex = index;
	// preenche tensão nominal
	if (barra != NULL)
		RichEditVnom->Text = DoubleToStr("%4.3f", barra->vnom);
	else
		RichEditVnom->Text = "";
	// exibe Moldura na Barra
	GraficoMoldura(barra);
}

// ---------------------------------------------------------------------------
VTEqpto* __fastcall TFormEdtRede::ExisteEqpto(TList *lisEQP, int eqpto_id)
{
	// variáveis locais
	VTEqpto *eqpto;

	// procura Rede em lisREDE
	for (int n = 0; n < lisEQP->Count; n++)
	{
		eqpto = (VTEqpto*)lisEQP->Items[n];
		if (eqpto->Id == eqpto_id)
			return (eqpto);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtRede::ExisteCluster(AnsiString codigo)
{
	// loop p/ todos os TreeNodes de nível 2
	TTreeNode *node;
	VTEqpto *eqpto;

	for (int n = 0; n < TViewRede->Items->Count; n++)
	{
		node = (TTreeNode*)TViewRede->Items->Item[n];
		if (node->Level == 2)
		{
			eqpto = (VTEqpto*)node->Data;
			if (eqpto->Codigo.AnsiCompareIC(codigo) == 0)
				return (true);
		}
	}
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtRede::ExisteRede(AnsiString codigo)
{
	// loop p/ todos os TreeNodes de nível 2
	TTreeNode *node;
	VTRede *rede;

	for (int n = 0; n < TViewRede->Items->Count; n++)
	{
		node = (TTreeNode*)TViewRede->Items->Item[n];
		if (node->Level == 2)
		{
			rede = (VTRede*)node->Data;
			if (rede->Codigo.AnsiCompareIC(codigo) == 0)
				return (true);
		}
	}
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtRede::ExisteRedeBTnova(void)
{
	// loop p/ todos os TreeNodes de nível 2
	TTreeNode *node;
	VTRede *rede;

	for (int n = 0; n < TViewRede->Items->Count; n++)
	{
		node = (TTreeNode*)TViewRede->Items->Item[n];
		if (node->Level == 2)
		{//verifica se a rede é BT
			rede = (VTRede*)node->Data;
			if (rede->TipoRede->Segmento == redeSEC)
				return (true);
		}
	}
	return (false);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::FinalizaEdicaoTreeNodeText(void)
{
	// verifica se há um TreeNode selecionado
	if (TViewRede->Selected == NULL)
		return;
	// verifica se o TreeNode selecionado possui um objeto associado
	if (TViewRede->Selected->Data == NULL)
		return;
	// força atualização de Text do TreeNode
	TViewRede->Selected->EndEdit(false);
	// o código da Rede é atualizado pelo evento TViewRedeEdited
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::FormClose(TObject *Sender, TCloseAction &Action)
{
	// destrói este Form
	Action = caFree;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		geral->Infoset->LoadFromFile(this);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::FormShow(TObject *Sender)
{
	// posiciona o Form
	// FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::GraficoMoldura(VTEqpto *eqpto)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	grafico->Moldura(eqpto);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::GraficoDestacaRede(VTRede *rede)
{
	// variáveis locais
	TColor color;
	VTRede *rede_qq, *rede_original;
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// determina  rede original
	if (rede == NULL)
		rede_original = NULL;
	else
		rede_original = (VTRede*)ExisteEqpto(orig.lisREDE, rede->Id);
	// verifica a opção selecionada
	if (RadioGroupModo->ItemIndex == 0)
	{ // destaca as ligações da Rede
		if (rede_original == NULL)
		{ // cancela destaque
			grafico->DestacaEqpto(NULL, clBlack, 1);
		}
		else
		{ // determina Color a ser usada no destaque da rede
			if (CheckBoxColorDestaque->Checked)
				color = rede_original->Color;
			else
				color = PanelColorDestaque->Color;
			grafico->DestacaEqpto(rede_original->LisLigacao(), color, 3);
		}
	}
	else
	{ // exibe apenas a Rede indicada
		for (int n = 0; n < local.lisREDE->Count; n++)
		{
			rede_qq = (VTRede*)local.lisREDE->Items[n];
			// determina  rede original
			if ((rede_original = (VTRede*)ExisteEqpto(orig.lisREDE, rede_qq->Id)) != NULL)
			{
				rede_original->Show[eqp_VISIBLE] = (rede == NULL) || (rede_qq == rede);
			}
		}
		grafico->Redraw();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::GraficoZoom(VTEqpto *eqpto)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	// seleciona área de zoom
	grafico->SelecionaZoom(eqpto);
}

// ---------------------------------------------------------------------------
VTCluster* __fastcall TFormEdtRede::HaClusterSelecionado(void)
{
	// variáveis locais
	VTEqpto *eqpto;

	// verifica se há um TreeNode selecionado
	if (TViewRede->Selected == NULL)
		return (NULL);
	// descarta TreeNode c/ Level inferior a 2
	if (TViewRede->Selected->Level < 2)
		return (NULL);
	// verifica se o TreeNode selecionado possui um objeto associado
	if (TViewRede->Selected->Data == NULL)
		return (NULL);
	// verifica se o objeto associado ao TreeNode é uma Rede
	eqpto = (VTEqpto*)TViewRede->Selected->Data;
	if (eqpto->Tipo() != eqptoCLUSTER)
		return (NULL);
	// retorna Cluster associada ao TreeNode
	return ((VTCluster*)eqpto);
}

// ---------------------------------------------------------------------------
VTRede* __fastcall TFormEdtRede::HaRedeSelecionada(void)
{
	// variáveis locais
	VTEqpto *eqpto;

	// verifica se há um TreeNode selecionado
	if (TViewRede->Selected == NULL)
		return (NULL);
	// descarta TreeNode c/ Level inferior a 2
	if (TViewRede->Selected->Level < 2)
		return (NULL);
	// verifica se o TreeNode selecionado possui um objeto associado
	if (TViewRede->Selected->Data == NULL)
		return (NULL);
	// verifica se o objeto associado ao TreeNode é uma Rede
	eqpto = (VTEqpto*)TViewRede->Selected->Data;
	if (eqpto->Tipo() != eqptoREDE)
		return (NULL);
	// retorna Rede associada ao TreeNode
	return ((VTRede*)eqpto);
}

// ---------------------------------------------------------------------------
TTreeNode* __fastcall TFormEdtRede::IdentificaTreeNodeAgrupadas(void)
{
	// variáveis locais
	TTreeNode *node;

	// verifica se há um TreeNode selecionado
	if ((node = TViewRede->Selected) != NULL)
	{ // verifica se o TreeNode é o de "Agrupadas"
		if (node->Text.CompareIC("Agrupadas") != 0)
			node = NULL;
	}
	return (node);
}

// ---------------------------------------------------------------------------
TTreeNode* __fastcall TFormEdtRede::IdentificaTreeNodeTipoRede(void)
{
	// variáveis locais
	TTreeNode *node;

	// verifica se há um TreeNode selecionado
	if ((node = TViewRede->Selected) == NULL)
		return (NULL);
	// determina TreeNode de nível 1 ("Agrupadas" ou TipoRede)
	while (node->Level > 1)
		node = node->Parent;
	// verifica o TreeNode possui um objeto associado
	if (node->Data == NULL)
		return (NULL);
	// retorna TreeNode
	return (node);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::IniciaLisCluster_Rede(void)
{
	// variáveis locais
	VTCluster *cluster, *cluster_orig;
	VTRede *rede, *rede_orig;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisCLUSTER = redes->LisCluster();
	TList *lisREDE = redes->LisRede();

	// monta listas de objetos Cluster: originais e cópias
	for (int nc = 0; nc < lisCLUSTER->Count; nc++)
	{
		cluster_orig = (VTCluster*)lisCLUSTER->Items[nc];
		// verifica se o Cluster está carregado
		if (!cluster_orig->Carregado)
			continue;
		// insere Cluster em orig.lisCLUSTER
		orig.lisCLUSTER->Add(cluster_orig);
		// cria cópia e insere em local.lisCLUSTER
		cluster = cluster_orig->Clone();
		local.lisCLUSTER->Add(cluster);
		// associa cópia do cluster com cluster original
		cluster->Obj = cluster_orig;
		// IMPORTANTE: remove Redes originais do Cluster
		cluster->Reinicia();
		// atualiza Status da cópia
		cluster->Status[sttNOVO] = false;
		cluster->Status[sttALTERADO] = false;
		cluster->Status[sttEXCLUIDO] = false;
	}
	// monta listas de objetos Rede: originais e cópias
	for (int nr = 0; nr < lisREDE->Count; nr++)
	{
		rede_orig = (VTRede*)lisREDE->Items[nr];
		// verifica se a Rede está carregada
		if (!rede_orig->Carregada)
			continue;
		// insere Rede em orig.lisREDE
		orig.lisREDE->Add(rede_orig);
		// cria cópia e insere em local.lisREDE
		rede = rede_orig->Clone();
		local.lisREDE->Add(rede);
		// associa cópia dda Rede com Rede original
		rede->Obj = rede_orig;
		// atualiza Status da cópia
		rede->Status[sttNOVO] = false;
		rede->Status[sttALTERADO] = false;
		rede->Status[sttEXCLUIDO] = false;
		// verifica se a Rede original pertence a um Cluster
		if (rede_orig->Cluster != NULL)
		{ // insere cópia da Rede na cópia do Cluster
			cluster = (VTCluster*)ExisteEqpto(local.lisCLUSTER, rede_orig->Cluster->Id);
			if ((cluster) && (rede))
				cluster->InsereRede(rede);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::InsereCluster(TTreeNode *node)
{
	// variáveis locais
	AnsiString codigo;
	VTCluster *cluster;
	TTreeNode *node_cluster;

	// solicita código do Cluster
	codigo = DefineCodigoCluster();
	if (codigo.IsEmpty())
		return;
	try
	{ // cria um novo Cluster e insere em local.lisCLUSTER
		cluster = DLL_NewObjCluster();
		local.lisCLUSTER->Add(cluster);
		// define código do Cluster
		cluster->Id = FalsoId();
		cluster->Codigo = codigo;
		// marca que o Cluster é novo
		cluster->Status[sttNOVO] = true;
		// insere TreeNode do Cluster
		node_cluster = TViewRede->Items->AddChildObject(node, cluster->Codigo, cluster);
		// ordena alfabeticamente Nodes de Cluster dentro do seu Node pai
		node->AlphaSort();
		// seleciona Node do novo CLuster
		TViewRede->Selected = node_cluster;
	}
	catch (Exception &e)
	{ // nada a afazer
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15630027
void __fastcall TFormEdtRede::InsereRede(TTreeNode *node_tipo)
{
	// variáveis locais
	AnsiString codigo;
	VTRede *rede;
	VTTipoRede *tipo_rede;
	TTreeNode *node_rede;
	VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// solicita código da Rede
	codigo = DefineCodigoRede();
	if (codigo.IsEmpty())
		return;
	try
	{ // determina TipoRede
		tipo_rede = (VTTipoRede*)node_tipo->Data;
		// cria uma nova Rede e insere em local.lisREDE
		rede = DLL_NewObjRede(patamares->NumPatamar());
		local.lisREDE->Add(rede);
		// define tipo e código p/ a rede
		rede->Id = FalsoId();
        rede->Extern_id = IntToStr(rede->Id); //para Copel
		rede->Redes = redes;
		rede->TipoRede = tipo_rede;
		rede->Codigo = codigo;
		rede->Carregada = true;
		rede->Valida = true;
		rede->Radial = ((tipo_rede->Segmento == redePRI) || (tipo_rede->Segmento == redeSEC));
		rede->Estatica = !rede->Radial;
		// marca que a Rede é nova
		rede->Status[sttNOVO] = true;
		// insere TreeNode da Rede
		node_rede = TViewRede->Items->AddChildObject(node_tipo, CodigoRede(rede), rede);
		// ordena alfabeticamente Nodes de Redes dentro do seu Node pai
		node_tipo->AlphaSort();
		// seleciona Node da rede nova
		TViewRede->Selected = node_rede;
	}
	catch (Exception &e)
	{ // nada a afazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::InsereRedeNoCluster(VTRede *rede, VTCluster *cluster)
{
	// verifica se a Rede está associada a um Cluster
	if (rede->Cluster)
	{ // marca que o Cluster original da Rede foi alterado
		// rede->cluster->Status[sttALTERADO] = true;
		// retira Rede do seu Cluster atual
		rede->Cluster->RemoveRede(rede);
	}
	// insere Rede no Cluster
	cluster->InsereRede(rede);
	// marca que a Rede e o Cluster foram alterados
	rede->Status[sttALTERADO] = true;
	// cluster->Status[sttALTERADO] = true;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtRede::LeTensaoNominal(double &vnom_kv)
{
	try
	{
		vnom_kv = StrToDouble(RichEditVnom->Text);
		return (true);
	}
	catch (Exception &e)
	{
		return (false);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::RadioGroupModoClick(TObject *Sender)
{
	// variáveis locais
	int index;

	// exibe/esconde GBoxColorDestaque
	GBoxColorDestaque->Visible = (RadioGroupModo->ItemIndex == 0);
	// desabilita evento RadioGroupModo::OnClick
	RadioGroupModo->OnClick = NULL;
	// salva opção do modo de apresentação
	index = RadioGroupModo->ItemIndex;
	// restaura modo de apresentação anterior e cancela Rede destacada anteriormente
	RadioGroupModo->ItemIndex = 1 - index;
	GraficoDestacaRede(NULL);
	// restaura opção do modo de apresentação
	RadioGroupModo->ItemIndex = index;
	// executa mesmo tratamento de seleção de uma Rede
	TViewRedeChange(TViewRede, TViewRede->Selected);
	// reabilita evento RadioGroupModo::OnClick
	RadioGroupModo->OnClick = RadioGroupModoClick;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::RedefineTipoRede(VTRede *rede, VTTipoRede *tipo_rede)
{
	// variáveis locais
	VTCluster *cluster = rede->Cluster;

	// redefine TipoRede da Rede
	rede->TipoRede = tipo_rede;
	// verifica se a Rede está associada a um Cluster
	if (cluster)
	{ // remove Rede de seu Cluster
		cluster->RemoveRede(rede);
		// marca que o Cluster foi alterado
		// cluster->Status[sttALTERADO] = true;
	}
	// marca que a Rede foi alterada
	rede->Status[sttALTERADO] = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::TViewEqptoChange(TObject *Sender, TTreeNode *Node)
{
	// variáveis locais
	VTEqpto *eqpto = NULL;

	// verifica se foi selecionado um Eqpto
	if (Node->Level == 1)
	{
		eqpto = (VTEqpto*)Node->Data;
	}
	GraficoMoldura(eqpto);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::TViewEqptoDblClick(TObject *Sender)
{
	// variáveis locais
	VTEqpto *eqpto = NULL;

	// verifica se foi selecionada um Eqpto
	if ((TViewEqpto->Selected != NULL) && (TViewEqpto->Selected->Level == 1))
	{
		eqpto = (VTEqpto*)TViewEqpto->Selected->Data;
	}
	GraficoZoom(eqpto);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::TViewEqptoInicia(VTRede *rede)
{
	// variáveis locais
	AnsiString txt;
	VTEqpto *eqpto;
	TTreeNode *node_tipo;
	int vet_tipo[] =
	{eqptoBARRA, eqptoTRECHO, eqptoCHAVE, eqptoTRAFO, eqptoREGULADOR, eqptoCARGA, eqptoCAPACITOR};

	// reinicia TView
	TViewEqpto->Items->Clear();
	// proteção
	if (rede == NULL)
		return;
	// determina rede original
	if ((rede = (VTRede*)ExisteEqpto(orig.lisREDE, rede->Id)) == NULL)
		return; ;
	// desabilita evento TViewEqpto::OnChange
	TViewEqpto->OnChange = NULL;
	// loop p/ todos tipos de Eqptos
	for (int nt = 0; nt < sizeof(vet_tipo) / sizeof(int); nt++)
	{ // obtém lista de Eqptos
		lisEQP->Clear();
		rede->LisEqpto(lisEQP, vet_tipo[nt]);
		if (lisEQP->Count == 0)
			continue;
		// obtém primeiro Eqpto da lista p/ incluir TreeNode
		eqpto = (VTEqpto*)lisEQP->First();
		txt.sprintf("%s (%d)", eqpto->TipoAsString().c_str(), lisEQP->Count);
		node_tipo = TViewEqpto->Items->Add(NULL, txt);
		// loop p/ todos Eqptos da Lista
		for (int n = 0; n < lisEQP->Count; n++)
		{
			eqpto = (VTEqpto*)lisEQP->Items[n];
			if (eqpto->Codigo.IsEmpty())
				txt.sprintf("id = %d", eqpto->Id);
			else
				txt = eqpto->Codigo;
			// insere TreeNode p/ Eqpto no TreeNode de tipo de LigacaoEqpto
			TViewEqpto->Items->AddChildObject(node_tipo, txt, eqpto);
		}
	}
	// ordena alfabeticamente todo o TreeView
	TViewEqpto->AlphaSort(true);
	// expande todo o TreeView
	// TViewEqpto->FullExpand();
	// reabilita evento TViewEqpto::OnChange
	TViewEqpto->OnChange = TViewEqptoChange;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::TViewEqptoMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	// variáveis locais
	TPoint Point;
	TTreeNode *node;
	VTEqpto *eqpto;

	// verifica se foi apertado o botão direito
	if (Button != mbRight)
		return;
	// identifica o TreeNode na posição X,Y
	if ((node = TViewEqpto->GetNodeAt(X, Y)) == NULL)
		return;
	// verifica se o Node está associado a uma Barra
	if ((eqpto = (VTEqpto*)node->Data) == NULL)
		return;
	if (!eqpto->TipoBarra())
		return;
	// redefine X, Y como coordenadas globais do mouse
	Point = Mouse->CursorPos;
	// verifica se treeNode é diferente do selecionado
	if (node != TViewEqpto->Selected)
	{ // seleciona o TreeNode (que dispara o evento OnChange)
		TViewEqpto->Selected = node;
	}
	// exibe PopupMenu que permite definir Barra inicial
	PopupMenu->Popup(Point.x, Point.y);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::TViewRedeChange(TObject *Sender, TTreeNode *Node)
{
	// variáveis locais
	VTBarra *barra;
	VTRede *rede;

	// verifica se há uma Rede selecionada no TViewRede
	if ((rede = HaRedeSelecionada()) == NULL)
	{ // esconde o PanelRede
		PanelRede->Visible = false;
		// cancela destaque da rede  e moldura no Eqpto
		GraficoMoldura();
		GraficoDestacaRede();
		return;
	}
	// exibe o PanelRede
	PanelRede->Visible = true;
	// desabilita Action associado ao evento OnClick de CheckBox
	ActionAtributoAlterado->Enabled = false;
	// atualiza tela
	checkRadial->Checked = rede->Radial;
	checkEstatica->Checked = rede->Estatica;
	PanelColorRede->Color = rede->Color;
	// reabilita Action associado evento OnClick de CheckBox
	ActionAtributoAlterado->Enabled = true;
	// apaga moldura e destaca Ligacoes da Rede original
	GraficoMoldura();
	GraficoDestacaRede(rede);
	// atualiza TViewEqpto
	TViewEqptoInicia(rede);
	// pré-seleciona Barra inicial da Rede
	ExibeBarraInicialRedeSelecionada();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::TViewRedeDblClick(TObject *Sender)
{
	// redefine área de zoom p/ destacar a Rede selecionada
	GraficoZoom(HaRedeSelecionada());
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::TViewRedeDragDrop(TObject *Sender, TObject *Source, int X, int Y)
{
	// variáveis locais
	AnsiString class_name;
	TObject *object;
	VTCluster *cluster, *cluster_orig;
	VTEqpto *eqpto;
	VTRede *rede;
	TTreeNode *node = TViewRede->GetNodeAt(X, Y);

	// verifica se há uma Rede sendo transferida
	if ((rede = HaRedeSelecionada()) == NULL)
		return;
	// verifica se há um TreeNode destino válido na posição X,Y
	if ((node == NULL) || (node->Data == NULL))
		return;
	// trata o TreeNode de acordo c/ seu Level
	switch (node->Level)
	{
	case 0: // TreeNode raiz
		return;
	case 1: // TreeNode destino está associado a um TipoRede
		RedefineTipoRede(rede, (VTTipoRede*)node->Data);
		break;
	default:
		while (node->Level > 2)
			node = node->Parent;
		eqpto = (VTEqpto*)node->Data;
		if (eqpto->Tipo() == eqptoCLUSTER)
		{ // insere Rede em um Cluster
			InsereRedeNoCluster(rede, (VTCluster*)eqpto);
		}
		else if (eqpto->Tipo() == eqptoREDE)
		{
			node = node->Parent;
			RedefineTipoRede(rede, (VTTipoRede*)node->Data);
		}
	}
	// transfere TreeNode da Rede p/ o TreeNode destino
	TViewRede->Selected->MoveTo(node, naAddChild);
	// reordena TreeNodes das Redes no TreeNode do Cluster
	node->AlphaSort();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::TViewRedeDragOver(TObject *Sender, TObject *Source, int X, int Y,
	TDragState State, bool &Accept)
{
	// variáveis locais
	VTEqpto *eqpto;
	TTreeNode *node = TViewRede->GetNodeAt(X, Y);

	// por default, rejeita a operação de Drop
	Accept = false;
	// verifica se há um TreeNode destino válido na posição X,Y : Cluster ou TipoRede
	if (node == NULL)
		return;
	if (node->Level == 0)
		return;
	if ((node->Level == 1) && (node->Data == NULL))
		return;
	Accept = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::TViewRedeEdited(TObject *Sender, TTreeNode *Node, UnicodeString &S)
{
	// variáveis locais
	VTEqpto *eqpto;

	// verifica se há uma Rede selecionada
	if ((eqpto = (VTEqpto*)Node->Data) == NULL)
		return;
	// verifica se o Eqpto do TreeNode é um Cluster ou uma Rede
	if ((eqpto->Tipo() == eqptoCLUSTER) || (eqpto->Tipo() == eqptoREDE))
	{ // atualiza código do Eqpto
		eqpto->Codigo = S;
		// marca que o Eqpto foi alterado
		eqpto->Status[sttALTERADO] = true;
	}
}

// ---------------------------------------------------------------------------
TTreeNode* __fastcall TFormEdtRede::TViewRedeExisteNodeTipo(int tipo_rede_id)
{
	// variáveis locais
	TTreeNode *node;
	VTTipoRede *tipo_rede;

	// verifica se já existe um Node filho c/ texto indicado
	for (int n = 0; n < node_raiz->Count; n++)
	{
		node = node_raiz->Item[n];
		// verifica se TreeNode filho possui o texto indicado
		if (node->Level != 1)
			continue;
		if ((tipo_rede = (VTTipoRede*)node->Data) == NULL)
			continue;
		if (tipo_rede->Id == tipo_rede_id)
			return (node);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtRede::TViewRedeInicia(void)
{
	// variáveis locais
	VTCluster *cluster;
	VTRede *rede;
	VTTipoRede *tipo_rede;
	TTreeNode *node_tipo, *node_cluster;
	TList *lisREDE;
	VTTipos *tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
	TList *lisTIPOREDE = tipos->LisTipoRede();

	// desbilita evento TViewRede::OnChange
	TViewRede->OnChange = NULL;
	// reinicia TView
	TViewRede->Items->Clear();
	// insere TreeNode raiz
	node_raiz = TViewRede->Items->Add(NULL, "Redes");
	// insere TreeNode p/ "Agrupadas"
	node_tipo = TViewRede->Items->AddChild(node_raiz, "Agrupadas");
	// insere um TreeNode p/ cada Cluster
	for (int nc = 0; nc < local.lisCLUSTER->Count; nc++)
	{
		cluster = (VTCluster*)local.lisCLUSTER->Items[nc];
		// verifica se o Cluster está carregado
		if (!cluster->Carregado)
			continue;
		// verifica se o Cluster foi excluído
		if (cluster->Status[sttEXCLUIDO])
			continue;
		// cria um TreeNode c/ Cluster
		node_cluster = TViewRede->Items->AddChildObject(node_tipo, cluster->Codigo, cluster);
		// cria um TreeNode p/ cada Rede do Cluster
		lisREDE = cluster->LisRede();
		for (int nr = 0; nr < lisREDE->Count; nr++)
		{
			rede = (VTRede*)lisREDE->Items[nr];
			// verifica se a Rede foi excluída
			if (rede->Status[sttEXCLUIDO])
				continue;
			// insere TreeNode da Rede
			TViewRede->Items->AddChildObject(node_cluster, CodigoRede(rede), rede);
		}
		// ordena TreeNodes de Redes dentro do TreeNode de cluster
		node_cluster->AlphaSort();
	}
	// ordena TreeNodes de Clusters dentro do TreeNode "Agrupadas"
	node_tipo->AlphaSort();
	// insere TreeNodes p/ TipoRede e suas Redes
	for (int n = 0; n < lisTIPOREDE->Count; n++)
	{
		tipo_rede = (VTTipoRede*)lisTIPOREDE->Items[n];
		// cria um TreeNode c/ tipo da rede
		TViewRede->Items->AddChildObject(node_raiz, tipo_rede->Codigo, tipo_rede);
		// insere TreeNodes p/ as Redes
		for (int nr = 0; nr < local.lisREDE->Count; nr++)
		{
			rede = (VTRede*)local.lisREDE->Items[nr];
			// verifica se a Rede está carregada
			if (!rede->Carregada)
				continue;
			// verifica se a Rede pertence a um Cluster
			if (rede->Cluster != NULL)
				continue;
			// verifica se a Rede foi excluída
			if (rede->Status[sttEXCLUIDO])
				continue;
			// verifica se Rede é do TipoRede
			if (rede->TipoRede != tipo_rede)
				continue;
			// verifica se existe TreeNode p/ o tipo da Rede
			if ((node_tipo = TViewRedeExisteNodeTipo(rede->TipoRede->Id)) == NULL)
				continue;
			// insere TreeNode da Rede
			TViewRede->Items->AddChildObject(node_tipo, CodigoRede(rede), rede);
		}
		// ordena TreeNodes de Redes dentro do TreeNode de TipoRede
		node_tipo->AlphaSort();
	}
	// ordena alfabeticamente TreeNodes dentro do TreeNode raiz
	node_raiz->AlphaSort();
	// expande todo o TreeView
	TViewRede->FullExpand();
	// reabilita evento TViewRede::OnChange
	TViewRede->OnChange = TViewRedeChange;
	// seleciona Node raiz
	TViewRede->Selected = node_raiz;
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtRede::ValidaBarraInicialRedeRadial(void)
{
	// variáveis locais
	int count = 0;
	VTRede *rede;

	// loop p/ as Redes locais
	for (int nr = 0; nr < local.lisREDE->Count; nr++)
	{
		rede = (VTRede*)local.lisREDE->Items[nr];
		// verifica se a rede é estática
		if (rede->Estatica)
			continue;
		// verifica se a Rede é radial
		if (rede->Radial)
		{ // verifica se a Barra inicial está definida
			if (rede->BarraInicial() == NULL)
				count++;
		}
	}
	if (count > 0)
	{
		Aviso("Há " + IntToStr(count) + " redes radiais sem definição de barra inicial.");
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
// eof
