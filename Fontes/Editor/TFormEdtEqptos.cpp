// ---------------------------------------------------------------------------
#include <vcl.h>
#include <algorithm>
#pragma hdrstop
#include "TFormEdtEqptos.h"
#include "TFormEdtCar.h"
#include "TFormEditaCurvaCarga.h"
#include "Grupo\TFormGrupo.h"
#include "SelecaoMultipla\VTFamiliaArranjos.h"
// #include "TFormSelRede.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Classes\VTClasse.h"
#include "..\Curvas\VTCurva.h"
#include "..\Diretorio\VTDir.h"
#include "..\Diretorio\VTPath.h"
#include "..\Edita\VTEdita.h"
#include "..\Forms\TFormEdtTxt.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\InterfaceTXT\Exporta\VTExportaTXT.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTRegulador.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\InterfaceTXT.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
int __fastcall CompareCodigoArranjo(void *Item1, void *Item2)
{
	int compare = -1;
	VTArranjo *arranjo1, *arranjo2;
	AnsiString cod1, cod2;

	arranjo1 = (VTArranjo*) Item1;
	arranjo2 = (VTArranjo*) Item2;
	cod1 = arranjo1->Codigo;
	cod2 = arranjo2->Codigo;
	// remove as fases do codigo
	// cod1 = TFormEdtEqptos::RemoveFaseString(cod1);
	// cod2 = TFormEdtEqptos::RemoveFaseString(cod2);
	// remove as fases do codigo
	//
	// remove as fases do codigo
	if (cod1.Pos("ABCN") > 0)
	{
		cod1 = StringReplace(cod1, "ABCN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("ABC") > 0)
	{
		cod1 = StringReplace(cod1, "ABC", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("ABN") > 0)
	{
		cod1 = StringReplace(cod1, "ABN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("BCN") > 0)
	{
		cod1 = StringReplace(cod1, "BCN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("CAN") > 0)
	{
		cod1 = StringReplace(cod1, "CAN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("ACN") > 0)
	{
		cod1 = StringReplace(cod1, "ACN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("*AB") > 0)
	{
		cod1 = StringReplace(cod1, "*AB", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("*BC") > 0)
	{
		cod1 = StringReplace(cod1, "*BC", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("*CA") > 0)
	{
		cod1 = StringReplace(cod1, "*CA", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("*AC") > 0)
	{
		cod1 = StringReplace(cod1, "*CA", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("*AN") > 0)
	{
		cod1 = StringReplace(cod1, "*AN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("*BN") > 0)
	{
		cod1 = StringReplace(cod1, "*BN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("*CN") > 0)
	{
		cod1 = StringReplace(cod1, "*CN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("*A") > 0)
	{
		cod1 = StringReplace(cod1, "*A", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("*BT") > 0)
	{
		cod1 = StringReplace(cod1, "*BT", "BT", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("*B") > 0)
	{
		cod1 = StringReplace(cod1, "*B", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("*C") > 0)
	{
		cod1 = StringReplace(cod1, "*C", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("_AB") > 0)
	{
		cod1 = StringReplace(cod1, "_AB", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("_BC") > 0)
	{
		cod1 = StringReplace(cod1, "_BC", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("_CA") > 0)
	{
		cod1 = StringReplace(cod1, "_CA", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("_AC") > 0)
	{
		cod1 = StringReplace(cod1, "_AC", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("_AN") > 0)
	{
		cod1 = StringReplace(cod1, "_AN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("_BN") > 0)
	{
		cod1 = StringReplace(cod1, "_BN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("_CN") > 0)
	{
		cod1 = StringReplace(cod1, "_CN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("_A") > 0)
	{
		cod1 = StringReplace(cod1, "_A", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("_BT") > 0)
	{
		cod1 = StringReplace(cod1, "_BT", "BT", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("_B") > 0)
	{
		cod1 = StringReplace(cod1, "_B", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("_C") > 0)
	{
		cod1 = StringReplace(cod1, "_C", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod1.Pos("N") > 0)
	{
		if (cod1.Pos("N") == cod1.Length())
		{
			cod1 = StringReplace(cod1, "N", "", TReplaceFlags() << rfReplaceAll);
		}
	}

	// if (cod1.Pos("ABCN") > 0)
	// {
	// cod1 = StringReplace(cod1, "ABCN", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod1.Pos("ABC") > 0)
	// {
	// cod1 = StringReplace(cod1, "ABC", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod1.Pos("ABN") > 0)
	// {
	// cod1 = StringReplace(cod1, "ABN", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod1.Pos("BCN") > 0)
	// {
	// cod1 = StringReplace(cod1, "BCN", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod1.Pos("CAN") > 0)
	// {
	// cod1 = StringReplace(cod1, "CAN", "", TReplaceFlags() << rfReplaceAll);
	// }
	// if (cod1.Pos("*AB") > 0)
	// {
	// cod1 = StringReplace(cod1, "*AB", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod1.Pos("*BC") > 0)
	// {
	// cod1 = StringReplace(cod1, "*BC", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod1.Pos("*CA") > 0)
	// {
	// cod1 = StringReplace(cod1, "*CA", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod1.Pos("*AN") > 0)
	// {
	// cod1 = StringReplace(cod1, "*AN", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod1.Pos("*BN") > 0)
	// {
	// cod1 = StringReplace(cod1, "*BN", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod1.Pos("*CN") > 0)
	// {
	// cod1 = StringReplace(cod1, "*CN", "", TReplaceFlags() << rfReplaceAll);
	// }
	//
	// else if (cod1.Pos("*A") > 0)
	// {
	// cod1 = StringReplace(cod1, "*A", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod1.Pos("*B") > 0)
	// {
	// cod1 = StringReplace(cod1, "*B", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod1.Pos("*C") > 0)
	// {
	// cod1 = StringReplace(cod1, "*", "", TReplaceFlags() << rfReplaceAll);
	// }
	//
	// // remove as fases do codigo
	if (cod2.Pos("ABCN") > 0)
	{
		cod2 = StringReplace(cod2, "ABCN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("ABC") > 0)
	{
		cod2 = StringReplace(cod2, "ABC", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("ABN") > 0)
	{
		cod2 = StringReplace(cod2, "ABN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("BCN") > 0)
	{
		cod2 = StringReplace(cod2, "BCN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("CAN") > 0)
	{
		cod2 = StringReplace(cod2, "CAN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("ACN") > 0)
	{
		cod2 = StringReplace(cod2, "ACN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("*AB") > 0)
	{
		cod2 = StringReplace(cod2, "*AB", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("*BC") > 0)
	{
		cod2 = StringReplace(cod2, "*BC", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("*CA") > 0)
	{
		cod2 = StringReplace(cod2, "*CA", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("*AC") > 0)
	{
		cod2 = StringReplace(cod2, "*CA", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("*AN") > 0)
	{
		cod2 = StringReplace(cod2, "*AN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("*BN") > 0)
	{
		cod2 = StringReplace(cod2, "*BN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("*CN") > 0)
	{
		cod2 = StringReplace(cod2, "*CN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("*A") > 0)
	{
		cod2 = StringReplace(cod2, "*A", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("*BT") > 0)
	{
		cod2 = StringReplace(cod2, "*BT", "BT", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("*B") > 0)
	{
		cod2 = StringReplace(cod2, "*B", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("*C") > 0)
	{
		cod2 = StringReplace(cod2, "*C", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("_AB") > 0)
	{
		cod2 = StringReplace(cod2, "_AB", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("_BC") > 0)
	{
		cod2 = StringReplace(cod2, "_BC", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("_CA") > 0)
	{
		cod2 = StringReplace(cod2, "_CA", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("_AC") > 0)
	{
		cod2 = StringReplace(cod2, "_AC", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("_AN") > 0)
	{
		cod2 = StringReplace(cod2, "_AN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("_BN") > 0)
	{
		cod2 = StringReplace(cod2, "_BN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("_CN") > 0)
	{
		cod2 = StringReplace(cod2, "_CN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("_A") > 0)
	{
		cod2 = StringReplace(cod2, "_A", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("_BT") > 0)
	{
		cod2 = StringReplace(cod2, "_BT", "BT", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("_B") > 0)
	{
		cod2 = StringReplace(cod2, "_B", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("_C") > 0)
	{
		cod2 = StringReplace(cod2, "_C", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (cod2.Pos("N") > 0)
	{
		if (cod2.Pos("N") == cod2.Length())
		{
			cod2 = StringReplace(cod2, "N", "", TReplaceFlags() << rfReplaceAll);
		}
	}

	// if (cod2.Pos("ABCN") > 0)
	// {
	// cod2 = StringReplace(cod2, "ABCN", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod2.Pos("ABC") > 0)
	// {
	// cod2 = StringReplace(cod2, "ABC", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod2.Pos("ABN") > 0)
	// {
	// cod2 = StringReplace(cod2, "ABN", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod2.Pos("BCN") > 0)
	// {
	// cod2 = StringReplace(cod2, "BCN", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod2.Pos("CAN") > 0)
	// {
	// cod2 = StringReplace(cod2, "CAN", "", TReplaceFlags() << rfReplaceAll);
	// }
	// if (cod2.Pos("*AB") > 0)
	// {
	// cod2 = StringReplace(cod2, "*AB", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod2.Pos("*BC") > 0)
	// {
	// cod2 = StringReplace(cod2, "*BC", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod2.Pos("*CA") > 0)
	// {
	// cod2 = StringReplace(cod2, "*CA", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod2.Pos("*AN") > 0)
	// {
	// cod2 = StringReplace(cod2, "*AN", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod2.Pos("*BN") > 0)
	// {
	// cod2 = StringReplace(cod2, "*BN", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod2.Pos("*CN") > 0)
	// {
	// cod2 = StringReplace(cod2, "*CN", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod2.Pos("*A") > 0)
	// {
	// cod2 = StringReplace(cod2, "*A", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod2.Pos("*B") > 0)
	// {
	// cod2 = StringReplace(cod2, "*B", "", TReplaceFlags() << rfReplaceAll);
	// }
	// else if (cod2.Pos("*C") > 0)
	// {
	// cod2 = StringReplace(cod2, "*", "", TReplaceFlags() << rfReplaceAll);
	// }
	//
	// // limpa os strings
	cod1 = StringReplace(cod1, "*", "", TReplaceFlags() << rfReplaceAll);
	cod1 = StringReplace(cod1, " ", "", TReplaceFlags() << rfReplaceAll);
	cod1 = StringReplace(cod1, "_", "", TReplaceFlags() << rfReplaceAll);

	//
	cod2 = StringReplace(cod2, "*", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, " ", "", TReplaceFlags() << rfReplaceAll);
	cod2 = StringReplace(cod2, "_", "", TReplaceFlags() << rfReplaceAll);

	compare = CompareText(cod1, cod2);

	return compare;

}

// ---------------------------------------------------------------------------
__fastcall TFormEdtEqptos::TFormEdtEqptos(TComponent* Owner, VTApl *apl, TList *lisEXT)
	: TForm(Owner)
{
	// salva ponteiro p/ objetos
	this->apl = apl;
	this->lisEXT = lisEXT;
	// cria lista
	lisEQP = new TList();
	lisTMP = new TList();
	lisFamiliaArranjo = new TList();
	// exibe dados de Barras e Ligacoes
	IniciaDadosBarra();
	IniciaDadosLigacao();
	IniciaDadosRegulador();
	IniciaDadosTrecho();
	IniciaDadosTrafo();
	IniciaDadosChave();
	IniciaDadosCarga();
	IniciaDadosModulo();
	// pré-seleciona TabSheet
	PageControl->ActivePage = TabSheetBarra;
	// posiciona o Form
	FormIniciaPosicao();
}

// ---------------------------------------------------------------------------
__fastcall TFormEdtEqptos::~TFormEdtEqptos(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	// apaga moldura
	Moldura();
	// salva parâmetros do Form
	if (geral)
		geral->Infoset->SaveToFile(this);
	// destrói listas sem destruir seus objetos
	if (lisEQP)
	{
		delete lisEQP;
		lisEQP = NULL;
	}
	if (lisTMP)
	{
		delete lisTMP;
		lisTMP = NULL;
	}
	if (lisFamiliaArranjo)
	{
		delete lisFamiliaArranjo;
		lisFamiliaArranjo = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::ActionAbreChaveExecute(TObject *Sender)
{
	// variáveis locais
	VTChave *chave;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// monta lista c/ as Chaves que estão fechadas
	lisEQP->Clear();
	for (int n = 0; n < LViewChave->Items->Count; n++)
	{
		chave = (VTChave*)(LViewChave->Items->Item[n]->Data);
		// verifica se a Chave está fechada
		if (chave->Fechada)
		{
			lisEQP->Add(chave);
		}
	}
	// abre todas Chaves através de uma única operação
	if (lisEQP->Count > 0)
		edita->Manobra(lisEQP);
	IniciaDadosChave();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::ActionByPassFalseExecute(TObject *Sender)
{
	// variáveis locais
	VTRegulador *regulador, *clone;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// monta lista c/ Reguladores com ByPass
	lisEQP->Clear();
	for (int n = 0; n < LViewReg->Items->Count; n++)
	{
		regulador = (VTRegulador*)(LViewReg->Items->Item[n]->Data);
		// verifica se Regulador não está com ByPass
		if (! regulador->by_pass)
			continue;
		// cria clone do regulador
		clone = regulador->Clone();
		clone->by_pass = false;
		// associa regulador c/ seu clone e insere em lisEQP
		regulador->Obj = clone;
		lisEQP->Add(regulador);
	}
	// tira ByPass em todos Reguladores através de uma única operação
	if (lisEQP->Count > 0)
		edita->AlteraLisEqpto(lisEQP);
	IniciaDadosRegulador();
}

//---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::ActionByPassTrueExecute(TObject *Sender)
{
	// variáveis locais
	VTRegulador *regulador, *clone;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// monta lista c/ Reguladores sem ByPass
	lisEQP->Clear();
	for (int n = 0; n < LViewReg->Items->Count; n++)
	{
		regulador = (VTRegulador*)(LViewReg->Items->Item[n]->Data);
		// verifica se Regulador está com ByPass
		if (regulador->by_pass)
			continue;
		// cria clone do regulador
		clone = regulador->Clone();
		clone->by_pass = true;
		// associa regulador c/ seu clone e insere em lisEQP
		regulador->Obj = clone;
		lisEQP->Add(regulador);
	}
	// aplica ByPass em todos Reguladores através de uma única operação
	if (lisEQP->Count > 0)
		edita->AlteraLisEqpto(lisEQP);
	IniciaDadosRegulador();
}

//---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::ActionCurvaExecute(TObject *Sender)
{ // DVK 2014.01.18
	// variaveis locais
	VTCarga *carga, *clone;
	VTCurva *curva;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));
	TFormEditaCurvaCarga *FormEdtCurvaCarga;

	// monta lista c/ as cargas originais e c/ as cargas alteradas
	lisTMP->Clear();
	// cria FormEdtCarga
	carga = (VTCarga*)lisEQP->First();
	FormEdtCurvaCarga = new TFormEditaCurvaCarga(this, apl, carga->curva);
	// exibe
	if (FormEdtCurvaCarga->ShowModal() == mrOk)
	{ // recebe curva atualizada
		curva = FormEdtCurvaCarga->Curva();
		// monta lista de Cargas e pega a primeira
		IniciaLisEqpto(lisEQP, eqptoCARGA);
		for (int n = 0; n < lisEQP->Count; n++)
		{ // todas as cargas
			carga = (VTCarga*)lisEQP->Items[n];
			// cria clone da carga
			clone = carga->Clone();
			clone->curva = curva->Clone();
			// associa carga c/ seu clone e insere em lisEQP
			carga->Obj = clone;
			lisTMP->Add(carga);
		}
		// efetiva a alteração de todas Cargas através de uma única operação
		edita->AlteraLisEqpto(lisTMP);
		IniciaDadosCarga();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::ActionDefArranjoTrechoExecute(TObject *Sender)
{
	// variáveis locais
	VTArranjo *arranjo = NULL;

	// verifica se há um Arranjo selecionado
	if (CBoxArranjo->ItemIndex >= 0)
	{ // determina Arranjo
		if (CBoxFases->ItemIndex >= 0)
		{ // determina Arranjo
			arranjo = (VTArranjo*)(CBoxFases->Items->Objects[CBoxFases->ItemIndex]);
		}
	}
	if (arranjo == NULL)
	{
		Aviso("Selecione um arranjo para recondutorar os trechos");
		return;
	}
	// redefine Arranjo de todos Trechos
	RedefineArranjoDosTrechos(arranjo);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::ActionDefSnomTrafoExecute(TObject *Sender)
{
	// variáveis locais
	double snom_kva;

	try
	{ // lê o valor da Vnom
		snom_kva = StrToDouble(EditTrafoSnom->Text);
	}
	catch (Exception &e)
	{
		Aviso("Defina um valor válido de potência nominal");
		return;
	}
	if (IsDoubleZero(snom_kva))
	{
		Aviso("Defina um valor válido de potência nominal");
		return;
	}
	// redefine Snom de todos Trafos
	RedefineSnomDosTrafos(snom_kva);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::ActionDefRedeLigacaoExecute(TObject *Sender)
{
	// variáveis locais
	VTRede *rede;

	// verifica se há uma Rede selecionada
	if (CBoxRede->ItemIndex < 0)
	{
		Aviso("Selecione a rede para associar as ligações");
		return;
	}
	// determina Rede
	if ((rede = (VTRede*)CBoxRede->Items->Objects[CBoxRede->ItemIndex]) == NULL)
	{
		Aviso("Selecione a rede para associar as ligações");
		return;
	}
	// redefine a Rede de todas as Ligacoes
	RedefineRedeDasLigacoes(rede);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::ActionDefVnomBarraExecute(TObject *Sender)
{
	// variáveis locais
	double vnom_kv;

	try
	{ // lê o valor da Vnom
		vnom_kv = StrToDouble(EditBarraVnom->Text);
	}
	catch (Exception &e)
	{
		Aviso("Defina um valor válido de tensão nominal");
		return;
	}
	if (IsDoubleZero(vnom_kv))
	{
		Aviso("Defina um valor válido de tensão nominal");
		return;
	}
	// redefine Vnom de todas Barras
	RedefineVnomDasBarras(vnom_kv);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::ActionEdtCargaExecute(TObject *Sender)
{
	// variaveis locais
	VTCarga *carga, *carga_alterada;

	// monta lista de Cargas e pega a primeira
	IniciaLisEqpto(lisEQP, eqptoCARGA);
	carga_alterada = (VTCarga*)lisEQP->Items[0];
	// verifica se já existe FormEdtCar
	if (FormEdtCar == NULL)
	{ // cria FormEdtCarga
		FormEdtCar = new TFormEdtCar(this, apl, carga_alterada, false);
		// exibe
		if (FormEdtCar->ShowModal() == mrOk)
		{
			for (int n = 0; n < lisEQP->Count; n++)
			{ // atualiza todas as cargas
				carga = (VTCarga*)lisEQP->Items[n];
				if (carga == carga_alterada)
					continue;
				RedefineCarga2IgualCarga1(carga_alterada, carga);
			}
		}
		// destroi FormEdtCarga
		delete FormEdtCar;
		FormEdtCar = NULL;
	}
	IniciaDadosCarga();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::ActionFechaExecute(TObject *Sender)
{
	// fecha o form
	ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::ActionFechaChaveExecute(TObject *Sender)
{
	// variáveis locais
	VTChave *chave;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// monta lista c/ as Chaves que estão fechadas
	lisEQP->Clear();
	for (int n = 0; n < LViewChave->Items->Count; n++)
	{
		chave = (VTChave*)(LViewChave->Items->Item[n]->Data);
		// verifica se a Chave está aberta
		if (chave->Aberta)
		{
			lisEQP->Add(chave);
		}
	}
	// fecha todas Chaves através de uma única operação
	if (lisEQP->Count > 0)
		edita->Manobra(lisEQP);
	IniciaDadosChave();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::ActionModuloInsExecute(TObject *Sender)
{
	// variáveis locais
	TFormEdtTxt *Form;
	TTreeNode *node;
	TFormGrupo *FormGrupo = (TFormGrupo*)apl->GetObject(__classid(TFormGrupo));

	// determina TreeNode de Level 0 que define o grupo
	if ((node = TViewGrupo->Selected) == NULL)
		return;
	while (node->Level > 0)
		node = node->Parent;
	// define nome do novo módulo
	Form = new TFormEdtTxt(this, "Novo módulo");
	if (Form->ShowModal() == mrOk)
	{ // cria novo módulo
		if (InsereModuloConstrutivo(node->Text, Form->Texto))
		{ // reinicia o TView
			IniciaDadosModulo();
			// atualiza FormGrupo existente no Apl
			FormGrupo->Atualiza();
		}
	}
	delete Form;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::ActionModuloRetExecute(TObject *Sender)
{
	// variáveis locais
	int tipo_arq;
	AnsiString arquivo;
	TTreeNode *node;
	TFormGrupo *FormGrupo = (TFormGrupo*)apl->GetObject(__classid(TFormGrupo));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	try
	{ // verifica se está selecionado um TreeNode de Level 1 que define o módulo
		node = TViewGrupo->Selected;
		if ((node == NULL) || (node->Level != 1))
		{
			Aviso("Selecione o módulo para exclusão");
			return;
		}
		// solicita confirmação do usuário
		if (Confirma("O módulo " + AnsiQuotedStr(node->Text, '\'') + " será excluído.",
			"Confirma a operação ?") != IDYES)
			return;
		// define path do arquivo
		arquivo = path->DirModuloConstrutivo() + "\\" + node->Parent->Text + "\\" + node->Text;
		// inclui extensão do arquivo
		tipo_arq = (int)(node->Data);
		if (tipo_arq == 0)
			arquivo += ".xml";
		else if (tipo_arq == 1)
			arquivo += ".txt";
		// elimina arquivo
		DeleteFile(arquivo);
		// reinicia o TView
		IniciaDadosModulo();
		// atualiza FormGrupo existente no Apl
		FormGrupo->Atualiza();
	}
	catch (Exception &e)
	{
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17852439
void __fastcall TFormEdtEqptos::ActionMudaFasesPrimarioExecute(TObject *Sender)
{
    // variáveis locais
    int fase_tag = 0;

    try
    { // determina fase
        switch(CBoxFasePri->ItemIndex)
        {
            case 0: fase_tag = faseAN; break;
            case 1: fase_tag = faseBN; break;
            case 2: fase_tag = faseCN; break;
            case 3: fase_tag = faseAB; break;
            case 4: fase_tag = faseBC; break;
            case 5: fase_tag = faseCA; break;
            case 6: fase_tag = faseABC; break;
        }
        if (fase_tag == 0)
        {
            Aviso("Defina uma fase para o primário dos trafos selecionados");
            return;
        }
        // redefine fase do primário de todos Trafos não trifásicos
        RedefineFasePriDosTrafos(fase_tag);
    }
    catch (...)
    {
        Aviso("Erro ao redefinir fase do primário dos trafos");
        return;
    }
}

//---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::ActionPastaInsExecute(TObject *Sender)
{
	// variáveis locais
	TFormEdtTxt *Form;
	TFormGrupo *FormGrupo = (TFormGrupo*)apl->GetObject(__classid(TFormGrupo));

	// define nome do novo módulo
	Form = new TFormEdtTxt(this, "Nova Pasta");
	if (Form->ShowModal() == mrOk)
	{ // cria novo diretório para módulos
		if (InsereDiretorio(Form->Texto))
		{ // reinicia o TView
			IniciaDadosModulo();
			// atualiza FormGrupo existente no Apl
			FormGrupo->Atualiza();
		}
	}
	delete Form;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::ActionPastaRetExecute(TObject *Sender)
{
	// variáveis locais
	AnsiString dir_path;
	TTreeNode *node, *node_filho;
	TFormGrupo *FormGrupo = (TFormGrupo*)apl->GetObject(__classid(TFormGrupo));
	VTDir *dir = (VTDir*)apl->GetObject(__classid(VTDir));
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	try
	{ // determina TreeNode de Level 0 que define o módulo
		node = TViewGrupo->Selected;
		if ((node == NULL) || (node->Level != 0))
		{
			Aviso("Selecione a pasta para exclusão");
			return;
		}
		// solicita confirmação do usuário
		if (Confirma("A pasta " + AnsiQuotedStr(node->Text, '\'') + " será excluída.",
			"Confirma a operação ?") != IDYES)
			return;
		// elimina o diretório
		dir_path = path->DirModuloConstrutivo() + "\\" + node->Text;
		dir->DeleteDirectory(dir_path);
		// reinicia o TView
		IniciaDadosModulo();
		// atualiza FormGrupo existente no Apl
		FormGrupo->Atualiza();
	}
	catch (Exception &e)
	{
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::ActionZerarTapsExecute(TObject *Sender)
{
	// variáveis locais
	VTTrafo *trafo, *clone;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// monta lista c/ os Trafos originais associados c/ seus clones alterados
	lisEQP->Clear();
	for (int n = 0; n < LViewTrafo->Items->Count; n++)
	{
		trafo = (VTTrafo*)(LViewTrafo->Items->Item[n]->Data);
		// cria clone do trafo
		clone = trafo->Clone();
		clone->pri.vnom = max(trafo->pbarra1->vnom, trafo->pbarra2->vnom);
		clone->pri.tap = clone->pri.vnom;
		clone->sec.vnom = min(trafo->pbarra1->vnom, trafo->pbarra2->vnom);
		clone->sec.tap = clone->sec.vnom;
		// associa Trafo c/ seu clone e insere em lisEQP
		trafo->Obj = clone;
		lisEQP->Add(trafo);
	}
	// efetiva a alteração de todos os Trafos através de uma única operação
	edita->AlteraLisEqpto(lisEQP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::ButComprimentoClick(TObject *Sender)
{
	// variáveis locais
	double compr;

	try
	{ // lê o valor do comprimento
		compr = StrToDouble(EditCompr->Text);
	}
	catch (Exception &e)
	{
		Aviso("Defina um valor válido de comprimento");
		return;
	}
	if (IsDoubleZero(compr))
	{
		Aviso("Defina um valor válido de comprimento");
		return;
	}
	// redefine Comprimento de todos os trechos
	RedefineComprTrechos(compr);
}

////---------------------------------------------------------------------------
// void __fastcall TFormEdtEqptos::CBoxArranjoInicia(void)
// {
// //variáveis locais
// VTArranjo  *arranjo;
// VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
// TList      *lisARRANJO   = arranjos->LisArranjo();
//
// //reinicia CBox c/ os Arranjos
// CBoxArranjo->Clear();
// if (lisARRANJO->Count == 0) return;
// for (int n = 0; n < lisARRANJO->Count; n++)
// {
// arranjo = (VTArranjo*)lisARRANJO->Items[n];
// //verifica se o Arranjo é obsoleto
// if (arranjo->StatusExcluido()) continue;
// CBoxArranjo->Items->AddObject(arranjo->Codigo, arranjo);
// }
// //define ordem alfabética
// CBoxArranjo->Sorted = true;
// //pré-seleciona primeiro Arranjo
// if (CBoxArranjo->Items->Count > 0) CBoxArranjo->ItemIndex = 0;
// }
// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::CBoxArranjoChange(TObject *Sender)
{
	CBoxFasesInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::CBoxArranjoInicia(void)
{
	// variáveis locais
	VTFamiliaArranjos *familia;

	// reinicia CBox c/ os Arranjos
	CBoxArranjo->Clear();
	// separa os arranjos em familias
	PreencheLisFamiliaArranjo();
	//
	for (int na = 0; na < lisFamiliaArranjo->Count; na++)
	{
		familia = (VTFamiliaArranjos*) lisFamiliaArranjo->Items[na];
		CBoxArranjo->Items->AddObject(familia->Codigo, familia);
	}
	// define ordem alfabética
	CBoxArranjo->Sorted = true;
	// pré-seleciona primeiro Arranjo
	if (CBoxArranjo->Items->Count > 0)
		CBoxArranjo->ItemIndex = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::CBoxFasesInicia(void)
{
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));
	VTFamiliaArranjos *familia = NULL;
	VTArranjo *arranjo;
	int faseTag = -1;

	CBoxFases->Clear();
	// verifica o index do cboxarranjo
	if (CBoxArranjo->ItemIndex < 0)
	{
		return;
	}
	familia = (VTFamiliaArranjos*)(CBoxArranjo->Items->Objects[CBoxArranjo->ItemIndex]);
	if (familia != NULL)
	{
		for (int nf = (fa_MAX_FASE - 1); nf > 0; nf--) // soh maior que zero pra nao add a invalida
		{
			arranjo = familia->Arranjo[nf];
			if (arranjo != NULL)
			{
				CBoxFases->Items->AddObject(fases->AsStrABCN[nf], arranjo);
			}
		}
	}
	// pré-seleciona primeira fase
	if (CBoxFases->Items->Count > 0)
		CBoxFases->ItemIndex = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::CBoxRedeInicia(void)
{
	// variáveis locais
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TList *lisREDE = redes->LisRede();

	// reinicia CBoxRede
	CBoxRede->Clear();
	// monta listas de objetos Rede: originais e cópias
	for (int nr = 0; nr < lisREDE->Count; nr++)
	{
		rede = (VTRede*)lisREDE->Items[nr];
		// verifica se a Rede está carregada
		if (!rede->Carregada)
			continue;
		CBoxRede->Items->AddObject(rede->Codigo, rede);
	}
	if (CBoxRede->Items->Count > 0)
	{ // ordena CBoxRede
		CBoxRede->Sorted = true;
		// pré-seleciona primeira Rede
		CBoxRede->ItemIndex = 0;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::FormClose(TObject *Sender, TCloseAction &Action)
{
	// esconde o Form
	Action = caHide;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::FormIniciaPosicao(void)
{
	// variáveis locais
	VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

	if (geral)
	{ // lê parâmetros do Form
		if (geral->Infoset->LoadFromFile(this))
			return;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::FormShow(TObject *Sender)
{
	// simula seleção de TabSheet
	PageControlChange(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::IniciaDadosBarra(void)
{
	// variáveis locais
	TListItem *item;
	VTBarra *barra = NULL;

	// monta lista de Barras
	IniciaLisEqpto(lisEQP, eqptoBARRA);
	// reinicia LView
	LViewBarra->Clear();
	// preenche LView c/ todas Barras
	for (int n = 0; n < lisEQP->Count; n++)
	{
		barra = (VTBarra*)lisEQP->Items[n];
		item = LViewBarra->Items->Add();
		item->Data = barra;
		item->Caption = barra->Codigo;
		// vnom da Barra
		item->SubItems->Add(DoubleToStr("%4.3f", barra->vnom));
	}
	// exibe/esconde TabSheetBarra
	TabSheetBarra->TabVisible = (LViewBarra->Items->Count > 0);
	// preenche EditBarraVnom
	if (barra)
		EditBarraVnom->Text = DoubleToStr("%4.3f", barra->vnom);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::IniciaDadosCarga(void)
{
	// variáveis locais
	double e_mes_kwh = 0.0;
	TListItem *item;
	VTCarga *carga;
	VTDemanda *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

	// esconde toolbar antigo das cargas
	ToolBarCargaOld->Visible = false; // DVK 2014.01.22
	// monta lista de Cargas
	IniciaLisEqpto(lisEQP, eqptoCARGA);
	// reinicia LView
	LViewCarga->Clear();
	// preenche LView c/ todas Ligacoes
	for (int n = 0; n < lisEQP->Count; n++)
	{
		carga = (VTCarga*)lisEQP->Items[n];
		item = LViewCarga->Items->Add();
		item->Data = carga;
		item->Caption = carga->Codigo;
		// exibe fases
		item->SubItems->Add(fases->AsStrUpper[carga->Fases]);
		// classe
		item->SubItems->Add(carga->classe->Codigo);
		// energia       //DVK 2014.01.22
		e_mes_kwh = demanda->Emes_mwh[carga] * 1000; // kWh
		item->SubItems->Add(DoubleToStr("%2.2f", e_mes_kwh, ','));
	}
	// exibe/esconde TabSheetCarga
	TabSheetCarga->TabVisible = (LViewCarga->Items->Count > 0);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::IniciaDadosChave(void)
{
	// variáveis locais
	TListItem *item;
	VTChave *chave;

	// monta lista de Barras
	IniciaLisEqpto(lisEQP, eqptoCHAVE);
	// reinicia LView
	LViewChave->Clear();
	// preenche LView c/ todas Ligacoes
	for (int n = 0; n < lisEQP->Count; n++)
	{
		chave = (VTChave*)lisEQP->Items[n];
		item = LViewChave->Items->Add();
		item->Data = chave;
		item->Caption = chave->Codigo;
		// exibe Rede da ligacao
		item->SubItems->Add(chave->rede->Codigo);
		// estado
		item->SubItems->Add((chave->Aberta) ? "Aberta" : "Fechada");
	}
	// exibe/esconde TabSheetChave
	TabSheetChave->TabVisible = (LViewChave->Items->Count > 0);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::IniciaDadosLigacao(void)
{
	// variáveis locais
	TListItem *item;
	VTLigacao *ligacao;

	// monta lista de Ligacoes
	IniciaLisEqpto(lisEQP, eqptoLIGACAO);
	// reinicia LView
	LViewLigacao->Clear();
	// preenche LView c/ todas Ligacoes
	for (int n = 0; n < lisEQP->Count; n++)
	{
		ligacao = (VTLigacao*)lisEQP->Items[n];
		item = LViewLigacao->Items->Add();
		item->Data = ligacao;
		item->Caption = ligacao->Codigo;
		item->SubItems->Add(ligacao->TipoAsString());
		// exibe Rede da ligacao
		item->SubItems->Add(ligacao->rede->Codigo);
	}
	// exibe/esconde TabSheetLigacao
	TabSheetLigacao->TabVisible = (LViewLigacao->Items->Count > 0);
	// inicia CBoxRede
	CBoxRedeInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::IniciaDadosModulo(void)
{
	// variáveis locais
	TFormGrupo *FormGrupo = (TFormGrupo*)apl->GetObject(__classid(TFormGrupo));

	// atenção: é possível criar editores que não utilizam TFormGrupo
	if (FormGrupo != NULL)
	{ // monta o TreeView de grupos/módulos
		FormGrupo->TViewGrupoInicia(TViewGrupo);
		// pré-seleciona um Grupo
		if (TViewGrupo->Items->Count > 0)
		{
			TViewGrupo->Selected = TViewGrupo->Items->Item[0];
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::IniciaDadosRegulador(void)
{
	// variáveis locais
	TListItem   *item;
	VTRegulador *regulador = NULL;
	AnsiString  bypass;

	// monta lista de Trafos
	IniciaLisEqpto(lisEQP, eqptoREGULADOR);
	// reinicia LView
	LViewReg->Clear();
	// preenche LView c/ todas Ligacoes
	for (int n = 0; n < lisEQP->Count; n++)
	{
		regulador = (VTRegulador*)lisEQP->Items[n];
		item = LViewReg->Items->Add();
		item->Data = regulador;
		item->Caption = regulador->Codigo;
		// exibe Rede da ligacao
		item->SubItems->Add(regulador->rede->Codigo);
		// estado
		bypass = (regulador->by_pass) ? "Sim" : "Não";
		item->SubItems->Add(bypass);
	}
	// exibe/esconde TabSheetRegulador
	TabSheetRegulador->TabVisible = (LViewReg->Items->Count > 0);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17852439
void __fastcall TFormEdtEqptos::IniciaDadosTrafo(void)
{
    // variáveis locais
    TListItem *item;
    VTTrafo *trafo = NULL;
    VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

    // monta lista de Trafos
    IniciaLisEqpto(lisEQP, eqptoTRAFO);
    // reinicia LView
    LViewTrafo->Clear();
    // preenche LView c/ todas Ligacoes
    for (int n = 0; n < lisEQP->Count; n++)
    {
        trafo = (VTTrafo*)lisEQP->Items[n];
        item = LViewTrafo->Items->Add();
        item->Data = trafo;
        item->Caption = trafo->Codigo;
        // exibe código
        item->SubItems->Add(trafo->rede->Codigo);
        // potencia nominal
        item->SubItems->Add(DoubleToStr("%2.1f", trafo->snom * 1000.));
        // fases do primario
        item->SubItems->Add(fases->AsStrUpper[trafo->pri.fase]);
    }
    // exibe/esconde TabSheetTrafo
    TabSheetTrafo->TabVisible = (LViewTrafo->Items->Count > 0);
    // preenche EditTrafoSnom
    if (trafo)
        EditTrafoSnom->Text = DoubleToStr("%2.1f", trafo->snom * 1000.);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::IniciaDadosTrecho(void)
{
	// variáveis locais
	TListItem *item;
	VTTrecho *trecho;
	VTFases *fases = (VTFases*)apl->GetObject(__classid(VTFases));

	// monta lista de Trafos
	IniciaLisEqpto(lisEQP, eqptoTRECHO);
	// reinicia LView
	LViewTrecho->Clear();
	// preenche LView c/ todas Ligacoes
	for (int n = 0; n < lisEQP->Count; n++)
	{
		trecho = (VTTrecho*)lisEQP->Items[n];
		item = LViewTrecho->Items->Add();
		item->Data = trecho;
		item->Caption = trecho->Codigo;
		// exibe Rede da ligacao
		item->SubItems->Add(trecho->rede->Codigo);
		// arranjo
		if (trecho->ArranjoTipico)
		{
			item->SubItems->Add(trecho->arranjo->Codigo);
		}
		else
		{
			item->SubItems->Add("próprio");
		}
		// fases
		item->SubItems->Add(fases->AsStrUpper[trecho->arranjo->Fases]);
	}
	// exibe/esconde TabSheetTrecho
	TabSheetTrecho->TabVisible = (LViewTrecho->Items->Count > 0);
	// inicia CBoxArranjo
	CBoxArranjoInicia();
	CBoxFasesInicia();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::IniciaLisEqpto(TList *lisEQP, int eqpto_tipo)
{
	// variáveis locais
	VTEqpto *eqpto;

	// monta lisEQP c/ os eqptos do tipo indicado presentes em lisEXT
	lisEQP->Clear();
	// verifica se eqpto_tipo é eqptoLIGACAO
	if (eqpto_tipo == eqptoLIGACAO)
	{
		for (int n = 0; n < lisEXT->Count; n++)
		{
			eqpto = (VTEqpto*)lisEXT->Items[n];
			if (eqpto->TipoLigacao())
			{
				if (lisEQP->IndexOf(eqpto) < 0)
					lisEQP->Add(eqpto);
			}
		}
	}
	else if (eqpto_tipo == eqptoEQBAR)
	{
		for (int n = 0; n < lisEXT->Count; n++)
		{
			eqpto = (VTEqpto*)lisEXT->Items[n];
			if (eqpto->TipoEqbar())
			{
				if (lisEQP->IndexOf(eqpto) < 0)
					lisEQP->Add(eqpto);
			}
		}
	}
	else
	{
		for (int n = 0; n < lisEXT->Count; n++)
		{
			eqpto = (VTEqpto*)lisEXT->Items[n];
			if (eqpto->Tipo() == eqpto_tipo)
			{
				if (lisEQP->IndexOf(eqpto) < 0)
					lisEQP->Add(eqpto);
			}
		}
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtEqptos::InsereDiretorio(AnsiString dir_name)
{
	// variáveis locais
	AnsiString dir_path;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	try
	{ // define path do diretorio
		dir_path = path->DirModuloConstrutivo() + "\\" + dir_name;
		// verifica se o diretório já existe
		if (DirectoryExists(dir_path))
		{
			Aviso("Já existe a pasta " + AnsiQuotedStr(dir_name, '\''));
			return (false);
		}
		// cria diretório
		CreateDir(dir_path);
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFormEdtEqptos::InsereModuloConstrutivo(AnsiString dir_grupo, AnsiString arq_txt)
{
	// variáveis locais
	VTExportaTXT *exporta;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

	try
	{ // define nome do arquivo TXT
		arq_txt = path->DirModuloConstrutivo() + "\\" + dir_grupo + "\\" + arq_txt;
		// define extensão p/ o arquivo como .txt
		arq_txt = ChangeFileExt(arq_txt, ".txt");
		// verifica se o arquivo já existe
		if (FileExists(arq_txt))
		{
			if (Confirma("Existe um módulo com esse nome", "Deseja substitui-lo ?") != IDYES)
				return (false);
		}
		// cria objeto p/ exportart arquivo TXT
		exporta = DLL_NewObjExportaTXT(apl);
		exporta->ExportaEqptos(lisEXT, arq_txt);
		// destrói objteo
		delete exporta;
	}
	catch (Exception &e)
	{
		return (false);
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::LViewDblClick(TObject *Sender)
{
	// variáveis locais
	VTEqpto *eqpto;
	TListView *LView = (TListView*)Sender;

	// determina Eqpto selecionado
	eqpto = (VTEqpto*)LView->Selected->Data;
	Moldura(eqpto);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::Moldura(VTEqpto *eqpto)
{
	// variáveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

	grafico->Moldura(eqpto);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::PageControlChange(TObject *Sender)
{
	// variáveis locais
	int width = 0;
	TListView *LView;

	// determina LView em exibição
	if (PageControl->ActivePage == TabSheetBarra)
		LView = LViewBarra;
	else if (PageControl->ActivePage == TabSheetLigacao)
		LView = LViewLigacao;
	else if (PageControl->ActivePage == TabSheetTrecho)
		LView = LViewTrecho;
	else if (PageControl->ActivePage == TabSheetTrafo)
		LView = LViewTrafo;
	else if (PageControl->ActivePage == TabSheetChave)
		LView = LViewChave;
	else if (PageControl->ActivePage == TabSheetCarga)
		LView = LViewCarga;
	else
		return;
//	// calcula Width
//	for (int n = 0; n < LView->Columns->Count; n++)
//	{
//		if (LView->Column[n]->AutoSize)
//			width += 120;
//		else
//			width += LView->Column[n]->Width;
//	}
//	// redefine Width do Form
//	if (width < Constraints->MinWidth)
//		width = Constraints->MinWidth;
//	Width = width;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::PreencheLisFamiliaArranjo(void)
{ // variáveis locais
	VTArranjo *arranjo;
	VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
	TList *lisARRANJO = arranjos->LisArranjo();
	VTFamiliaArranjos *familia;
	TList *lisARRANJOTMP = new TList;

	__try
	{
		// destroi todos objetos dentro da lista
		LimpaTList(lisFamiliaArranjo);
		// protecao
		if (lisARRANJO->Count == 0)
			return;
		for (int n = 0; n < lisARRANJO->Count; n++)
		{
			arranjo = (VTArranjo*)lisARRANJO->Items[n];
			// verifica se o Arranjo é obsoleto
			if (arranjo->StatusExcluido())
				continue;
			// adiciona os nao obsoletos (??)
			lisARRANJOTMP->Add(arranjo);
		}
		// ordena a lisata
		lisARRANJOTMP->Sort(CompareCodigoArranjo);
		// depois de ordenado, separa as familias
		SeparaArranjosEmFamilias(lisARRANJOTMP, lisFamiliaArranjo);
	}
	__finally
	{
		if (lisARRANJOTMP)
		{
			delete lisARRANJOTMP;
			lisARRANJOTMP = NULL;
		}
	}

}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::RedefineArranjoDosTrechos(VTArranjo *arranjo)
{
	// variáveis locais
	VTTrecho *trecho, *clone;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// monta lista c/ os Trechos originais e c/ os Trechos alterados
	lisEQP->Clear();
	for (int n = 0; n < LViewTrecho->Items->Count; n++)
	{
		trecho = (VTTrecho*)(LViewTrecho->Items->Item[n]->Data);
		if (trecho == NULL)
			continue;
		// cria uma cópia alterada
		clone = trecho->Clone();
		clone->DefineObjArranjo(arranjo);
		// associa Trecho c/ seu clone e insere ligacao em lisEQP
		trecho->Obj = clone;
		lisEQP->Add(trecho);
	}
	// efetiva a alteração de todas Ligacoes através de uma única operação
	edita->AlteraLisEqpto(lisEQP);
	IniciaDadosTrecho();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::RedefineCarga2IgualCarga1(VTCarga *carga1, VTCarga *carga2)
{
	// variáveis locais
	VTCarga *clone;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// monta lista c/ as cargas originais e c/ as cargas alteradas
	lisTMP->Clear();
	// clona e copia atributos
	clone = carga2->Clone();
	clone->CopiaAtributosDe(*carga1);
	// recupera esq, pbarra e código (se marcado)
	clone->DefineObjBarra(carga2->pbarra);
	clone->esq = carga2->esq;
	if (!cbCodCarga->Checked)
		clone->Codigo = carga2->Codigo;
	// associa Carga c/ seu clone e insere em  lisTMP
	carga2->Obj = clone;
	lisTMP->Add(carga2);
	// efetiva a alteração de todas Cargas através de uma única operação
	edita->AlteraLisEqpto(lisTMP);
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::RedefineComprTrechos(double compr)
{
	// variáveis locais
	VTTrecho *trecho, *clone;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// monta lista c/ as Ligacoes originais e c/ as Ligacoes alteradas
	lisEQP->Clear();
	for (int n = 0; n < LViewTrecho->Items->Count; n++)
	{
		trecho = (VTTrecho*)(LViewTrecho->Items->Item[n]->Data);
		if (!IsDoubleZero(trecho->Comprimento_km - compr))
		{ // cria clone da Barra
			clone = trecho->Clone();
			clone->Comprimento_km = compr;
			// associa trecho c/ seu clone e insere em lisEQP
			trecho->Obj = clone;
			lisEQP->Add(trecho);
		}
	}
	// efetiva a alteração de todos os trechos através de uma única operação
	edita->AlteraLisEqpto(lisEQP);
	IniciaDadosTrecho();
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/17852439
void __fastcall TFormEdtEqptos::RedefineFasePriDosTrafos(int fase_tag)
{
    // variáveis locais
    int count_trifasico = 0;
    bool trifasico = false;
    VTTrafo *trafo, *clone;
    VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

    //verifica se a fase nova é ABC
    trifasico = (fase_tag == faseABC);
    // monta lista c/ os Trafos originais associados c/ seus clones alterados
    lisEQP->Clear();
    for (int n = 0; n < LViewTrafo->Items->Count; n++)
    {
        trafo = (VTTrafo*)(LViewTrafo->Items->Item[n]->Data);
        if(trafo->pri.fase == faseABC)
        {
            count_trifasico++;
        }
        if (trafo->pri.fase != fase_tag)
        { // cria clone do Trafo
            clone = trafo->Clone();
            clone->pri.fase = fase_tag;
            //altera ligação e fase do secundario, se estiver passando trafo mono para tri
            if(trifasico)
            {
                clone->pri.ligacao = lgTRIANG;
                clone->pri.vnom = clone->BarraPrimario()->vnom;
                clone->pri.tap = clone->pri.vnom * 0.9566;
                clone->pri.defasagem = 0;
                clone->sec.fase = (fase_tag | faseNT);
                clone->sec.ligacao = lgEST_AT;
                clone->sec.defasagem = 30;
            }
            // associa Trafo c/ seu clone e insere em lisEQP
            trafo->Obj = clone;
            lisEQP->Add(trafo);
        }
    }
    //sinaliza trafos trifasicos
    if(count_trifasico > 0)
    {
        Aviso("Os trafos trifásicos selecionados não serão alterados");
    }
    // efetiva a alteração de todos Trafos através de uma única operação
    edita->AlteraLisEqpto(lisEQP);
    IniciaDadosTrafo();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::RedefineRedeDasLigacoes(VTRede *rede)
{
	// variáveis locais
	VTLigacao *ligacao, *clone;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// monta lista c/ as Ligacoes originais e c/ as Ligacoes alteradas
	lisEQP->Clear();
	for (int n = 0; n < LViewLigacao->Items->Count; n++)
	{
		ligacao = (VTLigacao*)(LViewLigacao->Items->Item[n]->Data);
		if (ligacao == NULL)
			continue;
		// cria uma cópia alterada e armazena em lisALT
		clone = ligacao->Clone();
		clone->DefineObjRede(rede);
		// associa Ligacao c/ seu clone e insere ligacao em lisEQP
		ligacao->Obj = clone;
		lisEQP->Add(ligacao);
	}
	// efetiva a alteração de todas Ligacoes através de uma única operação
	edita->AlteraLisEqpto(lisEQP);
	IniciaDadosLigacao();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::RedefineSnomDosTrafos(double snom_kva)
{
	// variáveis locais
	VTTrafo *trafo, *clone;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// monta lista c/ os Trafos originais associados c/ seus clones alterados
	lisEQP->Clear();
	for (int n = 0; n < LViewTrafo->Items->Count; n++)
	{ // trafo->snom em MVA
		trafo = (VTTrafo*)(LViewTrafo->Items->Item[n]->Data);
		if (!IsDoubleZero((trafo->snom * 1000) - snom_kva)) // snom em kVA
		{ // cria clone do Trafo
			clone = trafo->Clone();
			clone->snom = snom_kva / 1000; // volta p/MVA
			// associa Trafo c/ seu clone e insere em lisEQP
			trafo->Obj = clone;
			lisEQP->Add(trafo);
		}
	}
	// efetiva a alteração de todos Trafos através de uma única operação
	edita->AlteraLisEqpto(lisEQP);
	IniciaDadosTrafo();
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::RedefineVnomDasBarras(double vnom_kv)
{
	// variáveis locais
	VTBarra *barra, *clone;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// monta lista c/ as Ligacoes originais e c/ as Ligacoes alteradas
	lisEQP->Clear();
	for (int n = 0; n < LViewBarra->Items->Count; n++)
	{
		barra = (VTBarra*)(LViewBarra->Items->Item[n]->Data);
		if (!IsDoubleZero(barra->vnom - vnom_kv))
		{ // cria clone da Barra
			clone = barra->Clone();
			clone->vnom = vnom_kv;
			// associa Barra c/ seu clone e insere em lisEQP
			barra->Obj = clone;
			lisEQP->Add(barra);
		}
	}
	// efetiva a alteração de todas Barras através de uma única operação
	edita->AlteraLisEqpto(lisEQP);
	IniciaDadosBarra();
}

// ---------------------------------------------------------------------------
AnsiString __fastcall TFormEdtEqptos::RemoveFaseString(AnsiString string)
{

	// remove as fases do codigo
	if (string.Pos("ABCN") > 0)
	{
		string = StringReplace(string, "ABCN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("ABC") > 0)
	{
		string = StringReplace(string, "ABC", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("ABN") > 0)
	{
		string = StringReplace(string, "ABN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("BCN") > 0)
	{
		string = StringReplace(string, "BCN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("CAN") > 0)
	{
		string = StringReplace(string, "CAN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("ACN") > 0)
	{
		string = StringReplace(string, "ACN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("*AB") > 0)
	{
		string = StringReplace(string, "*AB", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("*BC") > 0)
	{
		string = StringReplace(string, "*BC", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("*CA") > 0)
	{
		string = StringReplace(string, "*CA", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("*AC") > 0)
	{
		string = StringReplace(string, "*CA", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("*AN") > 0)
	{
		string = StringReplace(string, "*AN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("*BN") > 0)
	{
		string = StringReplace(string, "*BN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("*CN") > 0)
	{
		string = StringReplace(string, "*CN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("*A") > 0)
	{
		string = StringReplace(string, "*A", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("*BT") > 0)
	{
		string = StringReplace(string, "*BT", "BT", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("*B") > 0)
	{
		string = StringReplace(string, "*B", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("*C") > 0)
	{
		string = StringReplace(string, "*C", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("_AB") > 0)
	{
		string = StringReplace(string, "_AB", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("_BC") > 0)
	{
		string = StringReplace(string, "_BC", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("_CA") > 0)
	{
		string = StringReplace(string, "_CA", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("_AC") > 0)
	{
		string = StringReplace(string, "_AC", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("_AN") > 0)
	{
		string = StringReplace(string, "_AN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("_BN") > 0)
	{
		string = StringReplace(string, "_BN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("_CN") > 0)
	{
		string = StringReplace(string, "_CN", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("_A") > 0)
	{
		string = StringReplace(string, "_A", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("_BT") > 0)
	{
		string = StringReplace(string, "_BT", "BT", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("_B") > 0)
	{
		string = StringReplace(string, "_B", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("_C") > 0)
	{
		string = StringReplace(string, "_C", "", TReplaceFlags() << rfReplaceAll);
	}
	else if (string.Pos("_N") > 0)
	{
		if (string.Pos("_N") == (string.Length() - 1))
		{
			string = StringReplace(string, "_N", "", TReplaceFlags() << rfReplaceAll);
		}
	}
	else if (string.Pos("N") > 0)
	{
		if (string.Pos("N") == string.Length())
		{
			string = StringReplace(string, "N", "", TReplaceFlags() << rfReplaceAll);
		}
	}

	string = StringReplace(string, "*", "", TReplaceFlags() << rfReplaceAll);
	string = StringReplace(string, "_", "", TReplaceFlags() << rfReplaceAll);
	return string;
}

// ---------------------------------------------------------------------------
void __fastcall TFormEdtEqptos::SeparaArranjosEmFamilias(TList *lisArranjos, TList *lisFamilias)
{
	VTArranjo *arranjo;
	AnsiString nomeAnterior, nomeAtual;
	VTFamiliaArranjos *familia;
	AnsiString nomeAnterior0, nomeAtual0;

	nomeAnterior = "";
	// percorre os arranjos e remove a indicao de fases dos nomes
	for (int na = 0; na < lisArranjos->Count; na++)
	{
		arranjo = (VTArranjo*)lisArranjos->Items[na];
		nomeAtual = RemoveFaseString(arranjo->Codigo);
		nomeAtual0 = StringReplace(nomeAtual, "*", "", TReplaceFlags() << rfReplaceAll);
		nomeAtual0 = StringReplace(nomeAtual0, " ", "", TReplaceFlags() << rfReplaceAll);
		nomeAtual0 = StringReplace(nomeAtual0, "_", "", TReplaceFlags() << rfReplaceAll);
		// Garante que nos casos em que as na importação existem arranjos com *fase_fase
		// ou _fase*fase essas duas "fases" vão ser tiradas do código
		nomeAtual0 = StringReplace(nomeAtual0, "ABC", "", TReplaceFlags() << rfReplaceAll);
		// como a lista esta ordenada...
		if (na == 0)
		{
			familia = NewObjFamiliaArranjos(apl);
			lisFamilias->Add(familia);
			familia->Codigo = nomeAtual;
			nomeAnterior = nomeAtual;
			nomeAnterior0 = StringReplace(nomeAnterior, "*", "", TReplaceFlags() << rfReplaceAll);
			nomeAnterior0 = StringReplace(nomeAnterior0, " ", "", TReplaceFlags() << rfReplaceAll);
			nomeAnterior0 = StringReplace(nomeAnterior0, "_", "", TReplaceFlags() << rfReplaceAll);
		}
		if (nomeAnterior0.AnsiCompare(nomeAtual0) != 0)
		{
			familia = NewObjFamiliaArranjos(apl);
			lisFamilias->Add(familia);
			familia->Codigo = nomeAtual;
			nomeAnterior = nomeAtual;
			nomeAnterior0 = StringReplace(nomeAnterior, "*", "", TReplaceFlags() << rfReplaceAll);
			nomeAnterior0 = StringReplace(nomeAnterior0, " ", "", TReplaceFlags() << rfReplaceAll);
			nomeAnterior0 = StringReplace(nomeAnterior0, "_", "", TReplaceFlags() << rfReplaceAll);
		}
		familia->InsereArranjo(arranjo);
	}
}
// ---------------------------------------------------------------------------
// eof



