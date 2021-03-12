// ---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <Math.hpp>
#include "TEstudo.h"
#include "VTAlternativa.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Obra\VTAcao.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrecho.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTEstudo* __fastcall NewObjEstudo(VTApl *apl)
   {
   try{
      return(new TEstudo(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
//---------------------------------------------------------------------------
__fastcall TEstudo::TEstudo(VTApl *apl)
{
	// salva ponteiro p/ objeto
	this->apl = apl;
	// define dados
	Id = -1;
	Tipo = estudoREDE_ORIGINAL;
	Codigo = "";
	Data = DateOf(Now());
	Novo = true;
	AnoInicial = 0;
	AnoFinal = 0;
	Justificativa = "";
	// cria objetos
	strlistREDE = new TStringList();
	lisALT = new TList();
	lisLIXO = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TEstudo::~TEstudo(void)
{
	// destr�i objetos
	if (strlistREDE)
	{
		delete strlistREDE;
		strlistREDE = NULL;
	}
	// destr�i lista e seus objetos
	if (lisALT)
	{
		LimpaTList(lisALT);
		delete lisALT;
		lisALT = NULL;
	}
	if (lisLIXO)
	{
		LimpaTList(lisLIXO);
		delete lisLIXO;
		lisLIXO = NULL;
	}
}

//https://sinapsisenergia.teamwork.com/#/tasks/16335731
// ---------------------------------------------------------------------------
VTAlternativa* __fastcall TEstudo::CopiaAlternativa(VTAlternativa *alternativa)
{
	// vari�veis locais
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
	VTAlternativa *nova_alternativa;

	// define Alternativa padr�o como ativa
	//PD.alternativa_ativa = AlternativaPadrao;
	DefineAlternativaAtiva(AlternativaPadrao, NULL);
	if(sinap_child != NULL)
	{
		sinap_child->EndEditorBlock(alternativa);
	}
	// cria uma nova Alternativa
	nova_alternativa = InsereAlternativa("Copiada", false);
	// copia Obras da Alternativa indicada
	nova_alternativa->CopiaObras(alternativa);
	// atualiza dados da Alternativa
	nova_alternativa->Id = -1;
	nova_alternativa->Fixa = false;

	return (nova_alternativa);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
bool __fastcall TEstudo::DefineAlternativaAtiva(VTAlternativa *alternativa, VTObra *obra)
{
	// verifica se n�o � a mesma Alternativa/Obra ativas
	if ((PD.alternativa_ativa == alternativa) && (PD.obra_ativa == obra))
		return (false);
	// prote��o p/ evitar c�lculo de fluxo
	// ExibeGraficoTopologia();
	// verifica se Alternativa � igual � Alternativa ativa
	if (PD.alternativa_ativa == alternativa)
	{ // exibe as redes da Obra indicada (se Obra for NULL s�o exibidas todas as redes)
		PD.alternativa_ativa->Obras->DefineRedesVisiveis(obra);
		// salva Obra ativa
		PD.obra_ativa = obra;
	}
	else
	{ // restarua a rede original: remonta a Rede sem nenhuma Obra da Alternativa ativa
		if (PD.alternativa_ativa != NULL)
			PD.alternativa_ativa->ReconfiguraRedeSemObra();
	}
	// redefine Alternativa Ativa
	if ((PD.alternativa_ativa = alternativa) != NULL)
	{ // reconfigura a Rede c/ todas as Obras da nova Alternativa ativa (e exibe apenas as Redes da Obra)
		PD.alternativa_ativa->ReconfiguraRedeTodaObra(obra);
		// salva Obra ativa
		PD.obra_ativa = obra;
	}
	return (true);
}

// ---------------------------------------------------------------------------
void __fastcall TEstudo::DefineObrasComoFixas(void)
{
	// vari�veis locais
	VTAlternativa *alternativa;

	for (int n = 0; n < lisALT->Count; n++)
	{
		alternativa = (VTAlternativa*)lisALT->Items[n];
		// marca Alternativa como fixa
		alternativa->Fixa = true;
	}
}

// ---------------------------------------------------------------------------
VTAlternativa* __fastcall TEstudo::ExisteAlternativa(AnsiString codigo)
{
	// vari�veis locais
	VTAlternativa *alternativa;

	for (int n = 0; n < lisALT->Count; n++)
	{
		alternativa = (VTAlternativa*)lisALT->Items[n];
		if (alternativa->Codigo.AnsiCompareIC(codigo) == 0)
			return (alternativa);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTAlternativa* __fastcall TEstudo::ExisteAlternativa(int id)
{
	// vari�veis locais
	VTAlternativa *alternativa;

	for (int n = 0; n < lisALT->Count; n++)
	{
		alternativa = (VTAlternativa*)lisALT->Items[n];
		if (alternativa->Id == id)
			return (alternativa);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TEstudo::ExibeGraficoTopologia(void)
{
	// vari�veis locais
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

	sinap_child->ActionTopo->Execute();
}

// ---------------------------------------------------------------------------
bool __fastcall TEstudo::FundeAcoesDasObras(void)
{
	// vari�veis locais
	VTAlternativa *alternativa;

	for (int n = 0; n < lisALT->Count; n++)
	{
		alternativa = (VTAlternativa*)lisALT->Items[n];
		alternativa->Obras->FundeObras();
	}
	return (true);
}

// ---------------------------------------------------------------------------
VTAlternativa* __fastcall TEstudo::InsereAlternativa(AnsiString codigo, bool padrao)
{
	// vari�veis locais
	VTAlternativa *alternativa;

	// cria uma nova Alternativa
	if ((alternativa = NewObjAlternativa(apl)) != NULL)
	{
		alternativa->Codigo = codigo;
		// verifica se Alternativa padr�o
		if (padrao)
		{ // insere Alternativa no in�cio de  lisALT
			lisALT->Insert(0, alternativa);
			// salva ponteiro p/ Alternativa padr�o
			PD.alternativa_padrao = alternativa;
		}
		else
		{ // insere Alternativa no fim de  lisALT
			lisALT->Add(alternativa);
		}
	}
	return (alternativa);
}

// ---------------------------------------------------------------------------
TList* __fastcall TEstudo::LisAlternativa(void)
{
	return (lisALT);
}

// ---------------------------------------------------------------------------
TList* __fastcall TEstudo::LisAlternativaExcluida(void)
{
	return (lisLIXO);
}

// ---------------------------------------------------------------------------
void __fastcall TEstudo::LisAcaoEstudo(TList *lisEXT)
{
	// vari�veis locais
	VTObra *obra;
	VTAlternativa *alternativa;
	TList *lisOBRA;
	TList *lisACAO;
	VTAcao* acao;

	for (int n = 0; n < lisALT->Count; n++)
	{
		alternativa = (VTAlternativa*)lisALT->Items[n];
		lisOBRA = alternativa->Obras->LisObra();
		for (int i = 0; i < lisOBRA->Count; i++)
		{
			obra = (VTObra*)lisOBRA->Items[i];
			lisACAO = obra->LisAcao;
			for (int j = lisACAO->Count - 1; j >= 0; j--)
			{
				acao = (VTAcao*)lisACAO->Items[j];
				lisEXT->Add(acao);
			}
		}
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TEstudo::LisEqptoEstudo(TList *lisEXT, VTRede *rede, int tipo)
 {
 //vari�veis locais
 VTAcao        *acao;
 VTObra        *obra;
 VTTrecho      *trecho;
 VTEqpto       *eqpto;
 VTLigacao     *ligacao;
 VTAlternativa *alternativa;
 TList         *lisOBRA;
 TList         *lisACAO;

 for (int n = 0; n < lisALT->Count; n++)
 {
 alternativa = (VTAlternativa*)lisALT->Items[n];
 lisOBRA = alternativa->Obras->LisObra();
 for (int i = 0; i < lisOBRA->Count; i++)
 {
 obra = (VTObra*)lisOBRA->Items[i];
 lisACAO = obra->LisAcao;
 // !OOPS!
 //         for (int j = 0; j < lisACAO->Count; j++)
 for (int j = lisACAO->Count - 1; j >= 0; j--)
 {
 acao = (VTAcao*)lisACAO->Items[j];
 switch(acao->Tipo())
 {
 case acaoALTERA:
 eqpto = acao->CopiaDoEqptoAlterado();
 break;
 case acaoINSERE:
 //eqpto = acao->CopiaDoEqptoOriginal();
 eqpto = acao->Eqpto();
 break;
 case acaoRETIRA:
 eqpto = NULL;
 break;
 default:
 eqpto = NULL;
 break;
 }
 //verifica se existe Eqpto da Acao
 if (eqpto == NULL) continue;
 //verifica se foi indicada uma Rede
 if (rede != NULL)
 {//verifica se o Eqpto � do tipo liga��o
 if (! eqpto->TipoLigacao()) continue;
 ligacao = (VTLigacao*)eqpto;
 //verifica se a Ligacao pertence � Rede indicada
 if (ligacao->rede != rede) continue;
 }
 //verifica se o Eqpto � do tipo indicado
 if ((tipo < 0)||(eqpto->Tipo() == tipo))
 {//insere Eqpto em lisEXT
 lisEXT->Add(eqpto);
 //verifica se o Eqpto � um Trecho c/ Arranjo pr�prio
 if (eqpto->Tipo() == eqptoTRECHO)
 {
 trecho = (VTTrecho*)eqpto;
 if (! trecho->ArranjoTipico)
 {//inclui Arranjo do Trecho em lisEXT
 lisEXT->Add(trecho->arranjo);
 }
 }
 }
 }
 }
 }
 }
 */
// ---------------------------------------------------------------------------
void __fastcall TEstudo::LisEqptoEstudo(TList *lisEXT, VTRede *rede, int tipo)
{
	// vari�veis locais
	VTAcao *acao;
	VTObra *obra;
	VTTrecho *trecho;
	VTEqpto *eqpto, *copia_original, *copia_alterado;
	VTLigacao *ligacao;
	VTAlternativa *alternativa;
	TList *lisOBRA;
	TList *lisACAO;

	// IMPORTANTE: os eqptos inseridos em lisEXT devem ser os mesmos usados
	// por BDrede na grava��o das Acoes
	for (int n = 0; n < lisALT->Count; n++)
	{
		alternativa = (VTAlternativa*)lisALT->Items[n];
		lisOBRA = alternativa->Obras->LisObra();
		for (int i = 0; i < lisOBRA->Count; i++)
		{
			obra = (VTObra*)lisOBRA->Items[i];
			lisACAO = obra->LisAcao;
			// for (int j = 0; j < lisACAO->Count; j++)
			for (int j = lisACAO->Count - 1; j >= 0; j--)
			{
				acao = (VTAcao*)lisACAO->Items[j];
				eqpto = acao->Eqpto();
				copia_original = (VTEqpto*)acao->CopiaDoEqptoOriginal();
				copia_alterado = (VTEqpto*)acao->CopiaDoEqptoAlterado();
				switch (acao->Tipo())
				{
				case acaoALTERA:
					lisEXT->Add(copia_original);
					lisEXT->Add(copia_alterado);
					break;
				case acaoINSERE:
					// 2014/04/05: garante Eqpto igual � sua c�pia original
					// (*eqpto) << (*copia_original);
					lisEXT->Add(eqpto);
					break;
				case acaoRETIRA:
					// nada a fazer
					break;
				default:
					// nada a fazer
					break;
				}
			}
		}
	}
	// aplica filtro de rede
	if (rede != NULL)
	{
		for (int n = lisEXT->Count - 1; n >= 0; n--)
		{
			eqpto = (VTEqpto*)lisEXT->Items[n];
			// verifica se o Eqpto � do tipo liga��o
			if (!eqpto->TipoLigacao())
				continue;
			ligacao = (VTLigacao*)eqpto;
			// verifica se a Ligacao pertence � Rede indicada
			if (ligacao->rede != rede)
				lisEXT->Delete(n);
		}
	}
	// aplica filtro de tipo de eqpto
	if (tipo >= 0)
	{
		for (int n = lisEXT->Count - 1; n >= 0; n--)
		{
			eqpto = (VTEqpto*)lisEXT->Items[n];
			// verifica se o Eqpto � do tipo indicado
			if (eqpto->Tipo() != tipo)
				lisEXT->Delete(n);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEstudo::LisObraEstudo(TList *lisEXT)
{
	// vari�veis locais
	VTObra *obra;
	VTAlternativa *alternativa;

	for (int n = 0; n < lisALT->Count; n++)
	{
		alternativa = (VTAlternativa*)lisALT->Items[n];
		CopiaTList(alternativa->Obras->LisObra(), lisEXT);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEstudo::MontaRedesObrasLidasDaBase(void)
{
	// vari�veis locais
	VTAlternativa *alternativa;

	// cria Alternativa padr�o caso ainda n�o exista
	if (AlternativaPadrao == NULL)
	{
		InsereAlternativa("[Rede Original]", true);
	}
	// loop p/ todas Alternativas
	for (int n = 0; n < lisALT->Count; n++)
	{
		alternativa = (VTAlternativa*)lisALT->Items[n];
		// salva lista de redes de cada Obra
		alternativa->Obras->IniciaListaRedesPorObra();
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TEstudo::PM_GetAlterado(void)
{
	// vari�veis locais
	VTAlternativa *alternativa;

	for (int n = 0; n < lisALT->Count; n++)
	{
		alternativa = (VTAlternativa*)lisALT->Items[n];
		// if (alternativa->Id < 1)            return(true);
		if (alternativa->Obras->Modificada)
			return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
int __fastcall TEstudo::PM_GetAnoInicial(void)
{
	// vari�veis locais
	int ano_atual = YearOf(Now());

	// verifica se ano inicial ainda n�o foi definido
	if (PD.ano_ini <= 0)
	{ // define ano inicial c/ ano atual
		PD.ano_ini = ano_atual;
	}
	return (PD.ano_ini);
}

// ---------------------------------------------------------------------------
int __fastcall TEstudo::PM_GetAnoFinal(void)
{
	// verifica se ano final ainda n�o foi definido
	if (PD.ano_fim <= 0)
	{ // define ano final igual ao ano da �ltima obra, mais 5 anos
		PD.ano_fim = Max(AnoInicial, AnoObraFinal) + 5;
	}
	return (PD.ano_fim);
}

// ---------------------------------------------------------------------------
int __fastcall TEstudo::PM_GetAnoObraInicial(void)
{
	// vari�veis locais
	int ano_inicial;
	VTAlternativa *alternativa;

	// define ano inicial como ano corrente
	ano_inicial = YearOf(Now());
	// determina menor ano inicial das Alternativas
	for (int n = 0; n < lisALT->Count; n++)
	{
		alternativa = (VTAlternativa*)lisALT->Items[n];
		ano_inicial = Min(ano_inicial, int(YearOf(alternativa->Obras->DataInicial)));
	}
	return (ano_inicial);
}

// ---------------------------------------------------------------------------
int __fastcall TEstudo::PM_GetAnoObraFinal(void)
{
	// vari�veis locais
	int ano_final;
	VTAlternativa *alternativa;

	// define ano inicial como ano corrente
	ano_final = YearOf(Now());
	// determina maior ano final das Alternativas
	for (int n = 0; n < lisALT->Count; n++)
	{
		alternativa = (VTAlternativa*)lisALT->Items[n];
		ano_final = Max(ano_final, int(YearOf(alternativa->Obras->DataFinal)));
	}
	return (ano_final);
}

// ---------------------------------------------------------------------------
bool __fastcall TEstudo::PM_GetNovo(void)
{
	return (PD.novo);
}

// ---------------------------------------------------------------------------
VTAlternativa* __fastcall TEstudo::PM_GetAlternativaAtiva(void)
{
	return (PD.alternativa_ativa);
}

// ---------------------------------------------------------------------------
VTAlternativa* __fastcall TEstudo::PM_GetAlternativaPadrao(void)
{
	return (PD.alternativa_padrao);
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422293
int __fastcall TEstudo::PM_GetModalResult(void)
    {
   return(PD.modal_result);
   }
//---------------------------------------------------------------------------
void __fastcall TEstudo::PM_SetAlterado(bool alterado)
{
	// vari�veis locais
	VTAlternativa *alternativa;

	// atualiza estado de todas Alternativas
	for (int n = 0; n < lisALT->Count; n++)
	{
		alternativa = (VTAlternativa*)lisALT->Items[n];
		alternativa->Obras->Modificada = alterado;
	}
}
/*
// ---------------------------------------------------------------------------
void __fastcall TEstudo::PM_SetAlternativaAtiva(VTAlternativa *alternativa)
{
	// verifica se Alternativa � a mesma que j� est� ativa
	if (PD.alternativa_ativa == alternativa)
		return;
	// prote��o p/ evitar c�lculo de fluxo
	ExibeGraficoTopologia();
	// remonta a Rede sem nenhuma Obra da atual Alternativa ativa
	if (PD.alternativa_ativa != NULL)
		PD.alternativa_ativa->ReconfiguraRedeSemObra();
	// redefine Alternativa Ativa
	PD.alternativa_ativa = alternativa;
	// reconfigura a Rede c/ todas as Obras da Alternativa ativa
	if (PD.alternativa_ativa != NULL)
		PD.alternativa_ativa->ReconfiguraRedeTodaObra();
}
*/
// ---------------------------------------------------------------------------
void __fastcall TEstudo::PM_SetAnoInicial(int ano_ini)
{
	PD.ano_ini = ano_ini;
}

// ---------------------------------------------------------------------------
void __fastcall TEstudo::PM_SetAnoFinal(int ano_fim)
{
	PD.ano_fim = ano_fim;
}

// ---------------------------------------------------------------------------
void __fastcall TEstudo::PM_SetNovo(bool novo)
{
	PD.novo = novo;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422293
void __fastcall TEstudo::PM_SetModalResult(int modal_result)
   {
   PD.modal_result = modal_result;
   }
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15987292
void __fastcall TEstudo::PreparaParaSalvarNaBase(void)
{
	// seleciona Alternativa padr�o como Alternativa ativa
	//PD.alternativa_ativa = AlternativaPadrao;
	DefineAlternativaAtiva(AlternativaPadrao, NULL);
	// funde as Acoes das Obras
	FundeAcoesDasObras();
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16335731
//https://sinapsisenergia.teamwork.com/#tasks/15987292
void __fastcall TEstudo::RetiraAlternativa(VTAlternativa *alternativa)
{
	VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));
	VTObra *obra;

	// verifica se Alternativa padr�o
	if (alternativa == PD.alternativa_padrao)
	{ // n�o permite excluir Alternativa padr�o
		Aviso("A alternativa " + AnsiQuotedStr(alternativa->Codigo, '\'') +
			" n�o deve ser exclu�da");
		return;
	}
	// verifica se alternativa sendo retirada � a Alternativa ativa
	if((alternativa != NULL) &&  (sinap_child != NULL))
	{
		sinap_child->EndEditorBlock(alternativa);
		for(int n=0; n <(alternativa->Obras->LisObra())->Count; n++)
		{
			obra = (VTObra*)(alternativa->Obras->LisObra())->Items[n];
			if(obra!=NULL)
			{
			   sinap_child->EndEditorBlock(obra);
            }
		}
	}
	if (alternativa == AlternativaAtiva)
	{ // seleciona a Alternativa padr�o
//		PD.alternativa_ativa = AlternativaPadrao;
		DefineAlternativaAtiva(AlternativaPadrao, NULL);
	}
	// retira a Alternativa de lisALT
	lisALT->Remove(alternativa);
	// verifica se � uma Alternativa nova
	if (alternativa->Id < 1)
	{ // destr�i objeto
		delete alternativa;
		// OBS: estudo N�O foi alterado
	}
	else
	{ // salva Alternativa em lisLIXO p/ ser exclu�da da base
		if (lisLIXO->IndexOf(alternativa) < 0)
			lisLIXO->Add(alternativa);
		// marca que o estudo foi alterado
		Alterado = true;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEstudo::RetiraAlternativaInvalida(void)
{
	// vari�veis locais
	VTAlternativa *alternativa;

	// monta a Rede sem nenhuma Alternativa
	// AlternativaAtiva = NULL;
	for (int n = lisALT->Count - 1; n >= 0; n--)
	{
		alternativa = (VTAlternativa*)lisALT->Items[n];
		if (!alternativa->Valida)
		{ // verifica se Alternativa padrao
			if (alternativa == PD.alternativa_padrao)
				PD.alternativa_padrao = NULL;
			if (alternativa == AlternativaAtiva)
				PD.alternativa_ativa = NULL;
			// retira a Alternativa de lisALT e destr�i objeto
			lisALT->Remove(alternativa);
			delete alternativa;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEstudo::RetiraObraSemAcao(void)
{
	// vari�veis locais
	VTAlternativa *alternativa;

	for (int n = 0; n < lisALT->Count; n++)
	{
		alternativa = (VTAlternativa*)lisALT->Items[n];
		alternativa->RetiraObraSemAcao();
	}
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TEstudo::SalvaNaBase(void)
 {
 //vari�veis locais
 VTBDrede      *bdrede  = (VTBDrede*)apl->GetObject(__classid(VTBDrede));

 //salva Estudo na base
 bdrede->SalvaEstudo(this);
 //atualiza estado
 Alterado = falae;
 }
 */
// ---------------------------------------------------------------------------
TStringList* __fastcall TEstudo::StrListRede(void)
{
	return (strlistREDE);
}

// ---------------------------------------------------------------------------
// eof
