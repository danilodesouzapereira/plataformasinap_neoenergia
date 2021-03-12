// -------------------------------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TFormSelRedeEgrid.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\ImportaRedeGis\VTImportaRedeGIS.h>
#include <DLL_Inc\Funcao.h>
#include "TArqRede.h"

// -------------------------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// -------------------------------------------------------------------------------------------------
__fastcall TFormSelRedeEgrid::TFormSelRedeEgrid(TComponent* Owner, VTApl *apl_owner, TArqRede *arq)
	: TForm(Owner)
{
	// salva ponteiros
	this->apl = apl_owner;
	this->arq = arq;
	// cria lista
	lisTMP = new TList();
	// inicia dados
	Inicia();
}

// -------------------------------------------------------------------------------------------------
__fastcall TFormSelRedeEgrid::~TFormSelRedeEgrid(void)
{
	// destroi lista sem destruir objetos
	DestroiTObject(lisTMP);
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormSelRedeEgrid::ActionCancelarExecute(TObject *Sender)
{
	// fecha
	ModalResult = mrCancel;
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormSelRedeEgrid::ActionConfirmarExecute(TObject *Sender)
{
	// monta lista com as Subesta��es selecionadas
	lisTMP->Clear();
	if (LisObjectChecked(CLBoxSubestacao, lisTMP) == 0)
	{ // avisa que nenhuma subesta��o foi selecionada
		Aviso("Selecione uma subesta��o para importa��o.");
		return;
	}
	//verifica se algum prim�rio foi selecionado
	if(! ExistePriSel(lisTMP))
	{// avisa que nenhuma rede foi selecionada
		Aviso("Selecione uma rede prim�ria para importa��o.");
		return;
    }
	// guarda subesta��es e prim�rios selecionados em arq
	arq->lisSub->Clear();
	CopiaTList(lisTMP, arq->lisSub);
	// fecha
	ModalResult = mrOk;
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormSelRedeEgrid::ActionPrimarioSelAllExecute(TObject *Sender)
{
	// vari�veis locais
	VTSub *sub;
	TNotifyEvent event;

	// seleciona todos os Primarios
	event = CLBoxPrimarioClickCheck;
	CLBoxPrimario->OnClickCheck = NULL;
	CheckAll(CLBoxPrimario, true);
	CLBoxPrimario->OnClickCheck = event;
	// monta lista com as Subesta��es selecionadas
	lisTMP->Clear();
	if (LisObjectChecked(CLBoxSubestacao, lisTMP) == 0)
		return;
	// loop p/ todas Subestacoes selecionadas
	for (int i = 0; i < lisTMP->Count; i++)
	{ // insere todos primarios na lista de selecionados
		sub = (VTSub*)lisTMP->Items[i];
		sub->PrimariosSel->Clear();
		sub->PrimariosSel->AddStrings(sub->Primarios);
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormSelRedeEgrid::ActionPrimarioSelNoneExecute(TObject *Sender)
{
	// vari�veis locais
	VTSub *sub;
	TNotifyEvent event;

	// cancela sele��o de todos os Primarios
	event = CLBoxPrimarioClickCheck;
	CLBoxPrimario->OnClickCheck = NULL;
	CheckAll(CLBoxPrimario, false);
	CLBoxPrimario->OnClickCheck = event;
	// monta lista com as Subesta��es selecionadas
	lisTMP->Clear();
	if (LisObjectChecked(CLBoxSubestacao, lisTMP) == 0)
		return;
	// loop p/ todas Subestacoes selecionadas
	for (int i = 0; i < lisTMP->Count; i++)
	{ // limpa lista de primarios selecionados
		sub = (VTSub*)lisTMP->Items[i];
		sub->PrimariosSel->Clear();
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormSelRedeEgrid::ActionSubestacaoSelAllExecute(TObject *Sender)
{
	// seleciona todas as Subestacoes
	CheckAll(CLBoxSubestacao, true);
	// reinicia CLBoxPrimario
	CLBoxPrimarioInicia();
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormSelRedeEgrid::ActionSubestacaoSelNoneExecute(TObject *Sender)
{
	// cancela sele��o de todas as Subestacoes
	CheckAll(CLBoxSubestacao, false);
	// reinicia CLBoxPrimario
	CLBoxPrimarioInicia();
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormSelRedeEgrid::CheckAll(TCheckListBox *CLBox, bool checked)
{
	for (int n = 0; n < CLBox->Items->Count; n++)
	{
		CLBox->Checked[n] = checked;
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormSelRedeEgrid::CLBoxFilhasInicia(TList *lisFilhas)
{
	// Vari�veis locais
	VTSub *subfilha;
	AnsiString prifilha;
	// TList *lisPrimFilha;
	int indexf, index_sub;

	// insere todas as filhas no CLBox
	for (int kf = 0; kf < lisFilhas->Count; kf++)
	{
		// analisa a kf-�sima filha em lisFilhas
		subfilha = (VTSub*)lisFilhas->Items[kf];
		// prote��o inserida pois nem todas as SE's da lista foram extra�das para COELBA #DSF
		if (subfilha == NULL)
			continue;
		// Encontra a subesta��o filha analisada e d� um check no CLBoxSubestacao
		index_sub = CLBoxSubestacao->Items->IndexOfObject(subfilha);
		if (index_sub < 0)
			continue;
		CLBoxSubestacao->Checked[index_sub] = true;
		// analisa os prim�rios da filha analisada
		for (int jf = 0; jf < subfilha->Primarios->Count; jf++)
		{
			prifilha = subfilha->Primarios->Strings[jf];
			// verifica se esse prim�rio j� existe no CLBox - DVC
			if (CLBoxPrimario->Items->IndexOf(prifilha) >= 0)
				continue;
			// insere Primario em CLBoxPrimario
			indexf = CLBoxPrimario->Items->Add(prifilha);
			// marca checado
			if (indexf >= 0)
			{
				CLBoxPrimario->Checked[indexf] = (subfilha->PrimariosSel->IndexOf(prifilha) >= 0);
				// true;
				// if (subfilha->PrimariosSel->IndexOf(prifilha) < 0)
				// subfilha->PrimariosSel->Add(prifilha);
			}
		}
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormSelRedeEgrid::CLBoxPrimarioClickCheck(TObject *Sender)
{
	// vari�veis locais
	AnsiString pri;
	// bool checked = false;
	int index;
	TCheckListBox *CLBox = CLBoxPrimario;
	VTSub *sub;

	// prote��o
	if (CLBox->ItemIndex < 0)
		return;

	// determina pri
	pri = CLBox->Items->Strings[CLBox->ItemIndex];
	// monta lista com as Subesta��es selecionadas
	lisTMP->Clear();
	if (LisObjectChecked(CLBoxSubestacao, lisTMP) == 0)
		return;
	// loop p/ todas Subestacoes selecionadas
	for (int i = 0; i < lisTMP->Count; i++)
	{
		sub = (VTSub*)lisTMP->Items[i];
		if (sub->Primarios->IndexOf(pri) < 0)
			continue;
		// marca/desmarca sele��o
		if (CLBox->Checked[CLBox->ItemIndex] == true)
		{ // insere na lista de primarios selecionados
			// checked = true;
			if (sub->PrimariosSel->IndexOf(pri) < 0)
				sub->PrimariosSel->Add(pri);
		}
		else
		{ // retira da lista de primarios selecionados
			// checked = false;
			index = sub->PrimariosSel->IndexOf(pri);
			if (index >= 0)
				sub->PrimariosSel->Delete(index);
		}
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormSelRedeEgrid::CLBoxPrimarioInicia(void)
{
	// vari�veis locais
	AnsiString pri;
	VTSub *sub;
	int index;

	// reinicia LBoxPrimarioAereo
	CLBoxPrimario->Clear();
	// CLBoxPrimario->Sorted = false;
	CLBoxPrimario->Visible = false;
	// monta lista com as Subesta��es selecionadas
	lisTMP->Clear();
	if (LisObjectChecked(CLBoxSubestacao, lisTMP) == 0)
		return;
	// desativa sorted
	CLBoxPrimario->Sorted = false;
	// loop p/ todas Subestacoes selecionadas
	for (int i = 0; i < lisTMP->Count; i++)
	{
		sub = (VTSub*)lisTMP->Items[i];
		// insere em CLBoxPrimario os Primarios da Subestacao
		for (int j = 0; j < sub->Primarios->Count; j++)
		{
			pri = sub->Primarios->Strings[j];
			// verifica se esse prim�rio j� existe no CLBox - DVC
			if (CLBoxPrimario->Items->IndexOf(pri) >= 0)
				continue;
			// insere Primario em CLBoxPrimario
			index = CLBoxPrimario->Items->Add(pri);
			// determina se estava checado
			if (index >= 0)
				CLBoxPrimario->Checked[index] = (sub->PrimariosSel->IndexOf(pri) >= 0);
		}
		// verifica se h� sub filhas
		if (sub->SubFilha->Count > 0)
		{ // Analisa as filhas e seus prim�rios e insere-os em seus respectivos CheckListBox
			CLBoxFilhasInicia(sub->SubFilha);
		}
	}
	// reapresenta CLBoxPrimarioAereo
	CLBoxPrimario->Sorted = true;
	CLBoxPrimario->Visible = true;
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormSelRedeEgrid::CLBoxSubestacaoClickCheck(TObject *Sender)
{
	// vari�veis locais
	// TCheckListBox *CLBox = CLBoxSubestacao;

	// marca/desmarca sele��o
	// CLBox->Checked[CLBoxSubestacao->ItemIndex] = (! CLBox->Checked[CLBoxSubestacao->ItemIndex]);
	CLBoxPrimarioInicia();
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormSelRedeEgrid::CLBoxSubestacaoInicia(void)
{
	// vari�veis locais
	VTSub *sub;
	TCheckListBox *CLBox = CLBoxSubestacao;
	int index;

	// reinicia CLBoxSubestacao
	CLBox->Clear();
	CLBox->Sorted = false;
	CLBox->Visible = false;
	// insere todas Subesta�oes em CLBoxSubestacao
	for (int n = 0; n < lisAllSub->Count; n++)
	{
		sub = (VTSub*)lisAllSub->Items[n];
		index = CLBox->Items->AddObject(sub->CodSub, sub);
		// determina se estava checado
		if (index >= 0)
			CLBox->Checked[index] = (sub->PrimariosSel->Count > 0);
	}
	CLBox->Sorted = true;
	CLBox->Visible = true;
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TFormSelRedeEgrid::ExistePriSel(TList *lisSub)
{
	// vari�veis locais
	VTSub *sub;

	for (int n = 0; n < lisSub->Count; n++)
	{//dentre as subesta��es selecionadas, verifica se, pelo menos, h� primarios em 1 subesta��o
		sub = (VTSub*)lisSub->Items[n];
		if(sub->PrimariosSel->Count > 0) return true;
	}
    return false;
}

// -------------------------------------------------------------------------------------------------
VTSub* __fastcall TFormSelRedeEgrid::ExisteSub(TList *lisSub, AnsiString cod_sub)
{
	// vari�veis locais
	VTSub *sub;

	for (int n = 0; n < lisSub->Count; n++)
	{
		sub = (VTSub*)lisSub->Items[n];
		if (sub->CodSub.AnsiCompareIC(cod_sub) == 0)
			return sub;
	}
	return NULL;
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormSelRedeEgrid::FormClose(TObject *Sender, TCloseAction &Action)
{
	// fecha form
	Action = caFree;
}

// ---------------------------------------------------------------------------------------------------
void __fastcall TFormSelRedeEgrid::Inicia(void)
{
	// vari�veis locais
	AnsiString arq_egrid;
	VTImportaRedeGIS *importa = (VTImportaRedeGIS*)apl->GetObject(__classid(VTImportaRedeGIS));

	//prote��o
	if(importa == NULL)
        return;
	// carrega as relacoes mae->filhas e os cabos
	if (arq->files->Count > 0)
	{ // guarda lista com todas VTSub
		lisAllSub = importa->LisAllSub();
		if (lisAllSub->Count == 0)
		{ // l� arquivo egrid
			arq_egrid = arq->files->Strings[0];
			importa->ImportaDadosEmpresa(arq_egrid);
			// guarda lista com todas VTSub
			lisAllSub = importa->LisAllSub();
		}
		// determina subesta��es e primarios selecionados
		// LeSubPriSelecionados();
		// inicia CheckListBox de Subesta��o
		CLBoxSubestacaoInicia();
		// inicia CheckListBox de Primarios
		CLBoxPrimarioInicia();
	}
}

// -------------------------------------------------------------------------------------------------
void __fastcall TFormSelRedeEgrid::LeSubPriSelecionados(void)
{
	// vari�veis locais
	VTSub *sub, *sub_arq;

	for (int n = 0; n < lisAllSub->Count; n++)
	{
		sub = (VTSub*)lisAllSub->Items[n];
		if ((sub_arq = ExisteSub(arq->lisSub, sub->CodSub)) == NULL)
			continue;
		// copia lista de primarios selecionados de sub_arq para sub
		sub->PrimariosSel->AddStrings(sub_arq->PrimariosSel);
	}
}

// -------------------------------------------------------------------------------------------------
int __fastcall TFormSelRedeEgrid::LisObjectChecked(TCheckListBox *CLBox, TList *lisOBJ)
{
	// vari�veis locais
	TObject *object;

	// insere em lisOBJ os objetos associados aos Items checked
	for (int n = 0; n < CLBox->Items->Count; n++)
	{
		if (CLBox->Checked[n])
		{
			if ((object = CLBox->Items->Objects[n]) == NULL)
				continue;
			if (lisOBJ->IndexOf(object) < 0)
				lisOBJ->Add(object);
		}
	}

	return (lisOBJ->Count);
}

// -------------------------------------------------------------------------------------------------
