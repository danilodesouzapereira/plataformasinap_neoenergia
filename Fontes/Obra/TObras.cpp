// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TObras.h"
#include "TAcaoComposta.h"
#include "VTAcao.h"
#include "VTObra.h"
#include "TDuplicacao.h"
#include "..\Apl\VTApl.h"
#include "..\Edita\VTEdita.h"
#include "..\MultiObra\VTMultiObraBO.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTRegulador.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTObras* __fastcall NewObjObras(VTApl *apl)
{
	return (new TObras(apl));
}

// ---------------------------------------------------------------------------
__fastcall TObras::TObras(VTApl *apl)
{
	// salva ponteiro p/ objeto externo
	this->apl = apl;
	// cria listas
	lisOBRA = new TList();
	lisACAO = new TList();
	lisEQP_1 = new TList();
	lisEQP_2 = new TList();
	lisBLOCK = new TList();
	lisLIXO = new TList();
	lisTMP = new TList();
	// inicia atributos
	Modificada = false;
	Analisada = false;
}

// ---------------------------------------------------------------------------
__fastcall TObras::~TObras(void)
{
	// vari�veis locais
	VTObra *obra;

	// destr�i listas sem destrrui seus objetos
	if (lisACAO)
	{
		delete lisACAO;
		lisACAO = NULL;
	}
	if (lisEQP_1)
	{
		delete lisEQP_1;
		lisEQP_1 = NULL;
	}
	if (lisEQP_2)
	{
		delete lisEQP_2;
		lisEQP_2 = NULL;
	}
	if (lisBLOCK)
	{
		delete lisBLOCK;
		lisBLOCK = NULL;
	}
	if (lisTMP)
	{
		delete lisTMP;
		lisTMP = NULL;
	}
	// destr�i Obras
	for (int n = lisOBRA->Count - 1; n >= 0; n--)
	{
		obra = (VTObra*)lisOBRA->Items[n];
		delete obra;
	}
	// destr�i lista
	if (lisOBRA)
	{
		delete lisOBRA;
		lisOBRA = NULL;
	}
	// destr�i listas e seus objetos
	if (lisLIXO)
	{
		LimpaTList(lisLIXO);
		delete lisLIXO;
		lisLIXO = NULL;
	}
}

// ---------------------------------------------------------------------------
VTObras* __fastcall TObras::Clone(void)
{
	// vari�veis locais
	TObras *clone;

	try
	{ // cria novo objeto Obras
		clone = new TObras(apl);
		// copia dados para o clone
		clone->CopiaAtributosDe(*this);
	}
	catch (Exception &e)
	{
		clone = NULL;
	}
	return (clone);
}

// ---------------------------------------------------------------------------
void __fastcall TObras::CopiaAtributosDe(VTObras &obras)
{
	// vari�veis locais
	VTObra *obra_orig, *clone;
	TList *lisOBRA_ORIG = obras.LisObra();

	try
	{ // cria objeto para c�pia das Acoes
		TDuplicacao duplicacao(apl);
		// copia dados de Obras original
		PD = ((TObras&)obras).PD;
		// copia cada um dos objetos VTObra
		for (int n = 0; n < lisOBRA_ORIG->Count; n++)
		{
			obra_orig = (VTObra*)lisOBRA_ORIG->Items[n];
			// cria c�pia da Obra
			if ((clone = obra_orig->Clone(duplicacao)) == NULL)
			{ // destr�i todas VTObra
				Reinicia();
				break;
			}
			// atualiza dados da Obra
			clone->Id = -1;
			clone->Fixa = false;
			// salva clone da Obra
			InsereObra(clone);
		}
	}
	catch (Exception &e)
	{ // destr�i todas VTObra
		Reinicia();
	}
}

//---------------------------------------------------------------------------
void __fastcall TObras::DefineObraAtiva(VTObra *obra)
   {
   //vari�veis locais
   TColor  color;
   VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

   //define clLime para obras anteriores e clRed para obra posteriores
   color = clLime;
   //associa Edita c/ objeto Obra indicado
   edita->Obra = obra;
   //redefine cores das Obras
   for (int n = 0; n < lisOBRA->Count; n++)
      {
	  obra = (VTObra*)lisOBRA->Items[n];
      if (obra == edita->Obra)
         {
         obra->Color = clAqua;
         //redefine a cor para obras posteriores
         color = clRed;
         }
      else
         {
         obra->Color = color;
         }
      }
   }

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
void __fastcall TObras::DefineRedesVisiveis(VTObra *obra)
{
	// vari�veis locais
	TList *lisREDE;
	VTRede *rede;
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// verifica se foi definida uma Obra
	if (obra != NULL)
	{ // exibe somente Redes da Obra
		lisREDE = redes->LisRede();
		//Deixa "N�O VISIVEL" todas as redes primeiro
		for (int n = 0; n < lisREDE->Count; n++)
		{
			rede = (VTRede*)lisREDE->Items[n];
			rede->Show[eqp_VISIBLE] = false;
		}
		lisREDE = obra->LisRede();
        //Coloca como vis�vel as redes ligadas � obra
		for (int n = 0; n < lisREDE->Count; n++)
		{
			rede = (VTRede*)lisREDE->Items[n];
			rede->Show[eqp_VISIBLE] = true;
		}
	}
	else
	{ // exibe todas as Redes
		lisREDE = redes->LisRede();
		for (int n = 0; n < lisREDE->Count; n++)
		{
			rede = (VTRede*)lisREDE->Items[n];
			rede->Show[eqp_VISIBLE] = true;
		}
	}
	// define a Obra indicada como ativa
	DefineObraAtiva(obra);
}
/*
// ---------------------------------------------------------------------------
bool __fastcall TObras::EfetivaObra(VTObra *obra)
{
	// vari�veis locais
	int index;
	VTAcao *acao;
	TList *lisACAO;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// verifica se a Obra n�o depende de outras
	if (!VerificaPossibilidadeExclusao(obra))
		return (false);
	// reconfigura Rede sem nenhuma Obra
	ReconfiguraRedeSemObra();
	// retira Obra de lisOBRA
	lisOBRA->Remove(obra);
	// associa Edita c/ Obra default do Editor
	edita->Obra = NULL;
	// transfere Acoes da Obra p/ Obra associada ao objeto Edita
	edita->InsereObra(obra);
	// destr�i Obra
	delete obra;
	// reconfigura Rede com toda Obra
	ReconfiguraRedeTodaObra();
	return (true);
}
*/
// ---------------------------------------------------------------------------
VTObra* __fastcall TObras::ExisteObra(int obra_id)
{
	// vari�veis locais
	VTObra *obra;

	// loop p/ todas Obras
	for (int n = 0; n < lisOBRA->Count; n++)
	{
		obra = (VTObra*)lisOBRA->Items[n];
		if (obra->Id == obra_id)
			return (obra);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTObra* __fastcall TObras::ExisteObraDataMenorIgual(TDateTime data)
{
	// vari�veis locais
	VTObra *obra;

	// as obras est�o organizadas em ordem crescente de data
	for (int n = lisOBRA->Count - 1; n >= 0; n--)
	{
		obra = (VTObra*)lisOBRA->Items[n];
		if (obra->DataInicio <= data)
			return (obra);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TObras::FundeObras(void)
{
	// vari�veis locais
	VTObra *obra;

	// garante que todas as Obras estejam incorporadas
	// JCG - 2011.01.05
	// ReconfiguraRedeTodaObra();
	// loop p/ todas Obras
	for (int n = 0; n < lisOBRA->Count; n++)
	{
		obra = (VTObra*)lisOBRA->Items[n];
		obra->FundeAcoes();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TObras::IniciaListaRedesPorObra(void)
{
	// vari�veis locais
	VTObra *obra = NULL;

	// loop para todas as Obras
	for (int n = 0; n < lisOBRA->Count; n++)
	{
		obra = (VTObra*)lisOBRA->Items[n];
		obra->IniciaLisRede();
	}
}

// ---------------------------------------------------------------------------
VTObra* __fastcall TObras::InsereObra(void)
{
	// vari�veis locais
	VTObra *obra;

	// cria um novo Obra
	if ((obra = NewObjObra(apl)) != NULL)
	{ // insere Obra em lisOBRA
		lisOBRA->Add(obra);
		// associa Obra c/ este objeto Obras
		obra->Obras = this;
		// inicia Obra como Visible
		obra->Visible = true;
		// marca que h� modifica��o
		Modificada = true;
	}
	return (obra);
}

// ---------------------------------------------------------------------------
VTObra* __fastcall TObras::InsereObra(TDateTime data)
{
	// vari�veis locais
	int index;
	VTObra *obra, *obra_anterior;

	// cria um novo Obra
	if ((obra = NewObjObra(apl)) != NULL)
	{ // define data de in�cio da Obra
		obra->DataInicio = data;
		// define data de ativa��o da Obra (=DataInicio)  //DVK 2013.12.05
		obra->DataAtivacao = data;
		// determina Obra com data menor ou igual � data indicada
		if ((obra_anterior = ExisteObraDataMenorIgual(data)) == NULL)
		{ // insere nova Obra no in�cio da lista
			lisOBRA->Insert(0, obra);
		}
		else
		{ // insere Obra nova ap�s Obra existente
			index = lisOBRA->IndexOf(obra_anterior);
			lisOBRA->Insert(index + 1, obra);
		}
		// associa Obra c/ este objeto Obras
		obra->Obras = this;
		// inicia Obra como Visible
		obra->Visible = true;
		// marca que h� modifica��o
		Modificada = true;
	}
	return (obra);
}

// ---------------------------------------------------------------------------
bool __fastcall TObras::InsereObra(VTObra *obra)
{
	// prote��o
	if (obra == NULL)
		return (false);
	if (lisOBRA->IndexOf(obra) < 0)
	{
		lisOBRA->Add(obra);
		// associa Obra c/ este objeto
		obra->Obras = this;
		// marca que h� modifica��o
		Modificada = true;
	}
	return (true);
}

// ---------------------------------------------------------------------------
VTAcao* __fastcall TObras::LastAcao(int tipo, VTEqpto *eqpto)
{
	// vari�veis locais
	VTObra *obra;
	VTAcao *acao;

	// loop p/ todas Obras
	for (int n = lisOBRA->Count - 1; n >= 0; n--)
	{
		obra = (VTObra*)lisOBRA->Items[n];
		// verifica se existe a Acao indicada
		if ((acao = obra->LastAcao(tipo, eqpto)) != NULL)
			return (acao);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
TList* __fastcall TObras::LisAcao(void)
{
	// vari�veis locais
	VTObra *obra;

	// monta lisACAO c/ Acoes de todas as Obras
	lisACAO->Clear();
	// loop p/ todas Obras
	for (int n = 0; n < lisOBRA->Count; n++)
	{
		obra = (VTObra*)lisOBRA->Items[n];
		// copia lista de Acoes da Obra em lisACAO
		CopiaTList(obra->LisAcao, lisACAO);
	}
	return (lisACAO);
}

// ---------------------------------------------------------------------------
TList* __fastcall TObras::LisObra(void)
{
	return (lisOBRA);
}

// ---------------------------------------------------------------------------
void __fastcall TObras::LisObra(TList *lisEXT, int ano)
{
	// vari�veis locais
	VTObra *obra;

	for (int n = 0; n < lisOBRA->Count; n++)
	{
		obra = (VTObra*)lisOBRA->Items[n];
		if (obra->AnoInicio == ano)
		{ // insere Obra em lisEXT
			if (lisEXT->IndexOf(obra) < 0)
				lisEXT->Add(obra);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TObras::LisObraContinuidade(TList *lisEXT, int ano)
{
	// vari�veis locais
	VTObra *obra;

	for (int n = 0; n < lisOBRA->Count; n++)
	{
		obra = (VTObra*)lisOBRA->Items[n];
		if (obra->AnoAtivacao <= ano)
		{ // insere Obra em lisEXT
			if (lisEXT->IndexOf(obra) < 0)
				lisEXT->Add(obra);
		}
	}
}

// ---------------------------------------------------------------------------
TList* __fastcall TObras::LisObraExcluida(void)
{
	return (lisLIXO);
}

// ---------------------------------------------------------------------------
void __fastcall TObras::MontaLisEqptoBloqueado(VTObra *obra_ativa, TList *lisEQP)
{
	// vari�veis locais
	VTObra *obra_2;

	// determina todos os Eqptos que s�o usados nas Obras posteriores
	lisEQP->Clear();
	for (int n = lisOBRA->Count - 1; n >= 0; n--)
	{
		obra_2 = (VTObra*)lisOBRA->Items[n];
		if (obra_2 == obra_ativa)
			break;
		MontaLisEqptoBloqueado(obra_2->LisAcao, lisEQP);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TObras::MontaLisEqptoBloqueado(TList *lisACAO, TList *lisEQP)
{
	// vari�veis locais
	bool incluir_eqpto;
	VTAcao *acao;

	// loop p/ toda Acao
	for (int n = 0; n < lisACAO->Count; n++)
	{
		acao = (VTAcao*)lisACAO->Items[n];
		// verifica o tipo da Acao
		switch (acao->Tipo())
		{
		case acaoALTERA:
			// insere Eqpto da Acao (e demais Eqptos relacionados)
			MontaLisEqptoBloqueado(acao->Eqpto(), lisEQP, incluir_eqpto = true);
			// insere Eqptos relacionados ao Eqpto alterado
			MontaLisEqptoBloqueado(acao->CopiaDoEqptoAlterado(), lisEQP, incluir_eqpto = false);
			break;
		case acaoCOMPOSTA: // chamada recursiva
			MontaLisEqptoBloqueado(((TAcaoComposta*)acao)->LisAcao(), lisEQP);
			break;
		case acaoINSERE:
			// insere Eqptos relacionados ao Eqpto inserido
			MontaLisEqptoBloqueado(acao->Eqpto(), lisEQP, incluir_eqpto = true);
			break;
		case acaoRETIRA:
			// insere Eqpto da Acao (e demais Eqptos relacionados)
			MontaLisEqptoBloqueado(acao->Eqpto(), lisEQP, incluir_eqpto = true);
			break;
		default:
			break;
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TObras::MontaLisEqptoBloqueado(VTEqpto *eqpto, TList *lisEQP, bool incluir_eqpto)
{
	// vari�veis locais
	VTBarra *barra;
	VTLigacao *ligacao;
	VTTrafo *trafo;
	VTRegulador *regulador;
	VTRede *rede;

	// prote��o
	if (eqpto == NULL)
		return;
	// verifica se deve incluir o Eqpto
	if ((incluir_eqpto) && (lisEQP->IndexOf(eqpto) < 0))
	{
		lisEQP->Add(eqpto);
	}
	// verifica o tipo do Eqpto
	if (eqpto->TipoEqbar())
	{
		barra = ((VTEqbar*)eqpto)->pbarra;
		if (lisEQP->IndexOf(barra) < 0)
		{
			lisEQP->Add(barra);
		}
	}
	else if (eqpto->TipoLigacao())
	{
		ligacao = (VTLigacao*)eqpto;
		// inclui Rede da Ligacao
		if (lisEQP->IndexOf(ligacao->rede) < 0)
		{
			lisEQP->Add(ligacao->rede);
		}
		// inclui Barras da Ligacao
		for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
		{
			if ((barra = ligacao->Barra(ind_bar)) == NULL)
				continue;
			if (lisEQP->IndexOf(barra) < 0)
			{
				lisEQP->Add(barra);
			}
		}
		// verifica casos espec�ficos de Ligacao
		if (ligacao->Tipo() == eqptoTRAFO)
		{ // verifica se o Trafo tem barra definida p/ ajuste autom�tico
			trafo = (VTTrafo*)ligacao;
			if ((barra = trafo->ltc.ajuste_auto.bar_ref) != NULL)
			{
				if (lisEQP->IndexOf(barra) < 0)
				{
					lisEQP->Add(barra);
				}
			}
		}
		else if (ligacao->Tipo() == eqptoREGULADOR)
		{
			regulador = (VTRegulador*)ligacao;
			if ((barra = regulador->cfg_auto.bar_ref) != NULL)
			{
				if (lisEQP->IndexOf(barra) < 0)
				{
					lisEQP->Add(barra);
				}
			}
		}
	}
}

// ---------------------------------------------------------------------------
VTObra* __fastcall TObras::ObraFinal(void)
{
	// prote��o
	if (lisOBRA->Count == 0)
		return (NULL);
	// retorna primeria Obra
	return ((VTObra*)lisOBRA->Last());
}

// ---------------------------------------------------------------------------
VTObra* __fastcall TObras::ObraFinalIncorporada(void)
{
	// vari�veis locais
	VTObra *obra;

	for (int n = lisOBRA->Count - 1; n >= 0; n--)
	{
		obra = (VTObra*)lisOBRA->Items[n];
		if (obra->Incorporada)
			return (obra);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
VTObra* __fastcall TObras::ObraInicial(void)
{
	// prote��o
	if (lisOBRA->Count == 0)
		return (NULL);
	// retorna primeria Obra
	return ((VTObra*)lisOBRA->First());
}

// ---------------------------------------------------------------------------
bool __fastcall TObras::PM_GetAnalisada(void)
{
	return (PD.analisada);
}

// ---------------------------------------------------------------------------
TDateTime __fastcall TObras::PM_GetDataFinal(void)
{
	// vari�veis locais
	VTObra *obra;

	// prote��o: verifica se h� Obras
	if (lisOBRA->Count == 0)
		return (DateOf(Now()));
	// retorna data da �ltima Obra
	obra = (VTObra*)lisOBRA->Last();
	return (obra->DataInicio);
}

// ---------------------------------------------------------------------------
TDateTime __fastcall TObras::PM_GetDataInicial(void)
{
	// vari�veis locais
	VTObra *obra;

	// prote��o: verifica se h� Obras
	if (lisOBRA->Count == 0)
		return (DateOf(Now()));
	// retorna data da primeira Obra
	obra = (VTObra*)lisOBRA->First();
	return (obra->DataInicio);
}

// ---------------------------------------------------------------------------
bool __fastcall TObras::PM_GetModificada(void)
{
	return (PD.modificada);
}

// ---------------------------------------------------------------------------
void __fastcall TObras::PM_SetAnalisada(bool analisada)
{
	PD.analisada = analisada;
}

// ---------------------------------------------------------------------------
void __fastcall TObras::PM_SetColor(TColor color)
{
	// vari�veis locais
	VTObra *obra;

	// localiza �ltima Obra c/ o ano indicado
	for (int n = 0; n < lisOBRA->Count; n++)
	{
		obra = (VTObra*)lisOBRA->Items[n];
		obra->Color = color;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TObras::PM_SetColorEnabled(bool enabled)
{
	// vari�veis locais
	VTObra *obra;

	// localiza �ltima Obra c/ o ano indicado
	for (int n = 0; n < lisOBRA->Count; n++)
	{
		obra = (VTObra*)lisOBRA->Items[n];
		obra->ColorEnabled = enabled;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TObras::PM_SetModificada(bool modificada)
{
	PD.modificada = modificada;
	if (modificada)
	{ // IMPORTANTE: cancela indica��o de analisada
		PD.analisada = false;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TObras::ReconfiguraRede(int ano)
{
	// vari�veis locais
	VTObra *obra;

	// localiza �ltima Obra c/ o ano indicado
	for (int n = lisOBRA->Count - 1; n >= 0; n--)
	{
		obra = (VTObra*)lisOBRA->Items[n];
		// if (obra->AnoInicio == ano)
		if (obra->AnoAtivacao <= ano)
		{ // reconfigura a Rede at� esta Obra
			ReconfiguraRedeAteObra(obra);
			return;
		}
	}
	// todas as obras entram depois do ano indicado
	ReconfiguraRedeSemObra();
}

// ---------------------------------------------------------------------------
void __fastcall TObras::ReconfiguraRede(VTObra *obra_ant, VTObra *obra_ref)
{
	// vari�veis locais
	int index_1, index_2;
	VTObra *obra;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// reinicia lisBLOCK
	lisBLOCK->Clear();
	// prote��o
	if (lisOBRA->Count == 0)
		return;
	if ((obra_ant == NULL) && (obra_ref == NULL))
		return;
	// verifica sequencia cronol�gica das Obras
	// if ((index_1 = lisOBRA->IndexOf(obra_ant)) < 0) index_1 = 0;
	// if ((index_2 = lisOBRA->IndexOf(obra_ref)) < 0) index_2 = 0;
	index_1 = (obra_ant == NULL) ? -1 : lisOBRA->IndexOf(obra_ant);
	index_2 = (obra_ref == NULL) ? -1 : lisOBRA->IndexOf(obra_ref);
	if (index_1 <= index_2)
	{ // prote��o
		// if (index_1 < 0) index_1 = 0;
		// monta lista com Obras para Redo
		lisTMP->Clear();
		for (int n = index_1 + 1; n <= index_2; n++)
		{
			obra = (VTObra*)lisOBRA->Items[n];
			// verifica se a Obra j� est� incorporada
			if (obra->Incorporada)
				continue;
			lisTMP->Add(lisOBRA->Items[n]);
			// marca que a Obra est� incorporada � Rede
			obra->Incorporada = true;
		}
		// executa Redo de todas Obra em lisTMP
		edita->RedoLisObra(lisTMP);
	}
	else
	{ // monta lista com Obras para Redo
		lisTMP->Clear();
		// executa Undo de todas Acoes das Obras anteriores � Obra indicada
		for (int n = index_2 + 1; n <= index_1; n++)
		{
			obra = (VTObra*)lisOBRA->Items[n];
			// verifica se a Obra j� n�o est� incorporada
			if (!obra->Incorporada)
				continue;
			lisTMP->Add(lisOBRA->Items[n]);
			// marca que a Obra n�o est� incorporada � Rede
			obra->Incorporada = false;
		}
		// executa Undo de todas Acoes das Obra
		edita->UndoLisObra(lisTMP);
	}
	// monta conjunto de Eqptos bloqueados p/ edi��o
	MontaLisEqptoBloqueado(obra_ref, lisBLOCK);
	// associa Edita c/ objeto Obra indicado
	edita->Obra = obra_ref;
}

// ---------------------------------------------------------------------------
void __fastcall TObras::ReconfiguraRedeAteObra(VTObra *obra_ref)
{
	// vari�veis locais
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	// monta a rede com as altera��es da obra indicada
	ReconfiguraRede(ObraFinalIncorporada(), obra_ref);
	// define como vis�veis as redes das obra
	DefineRedesVisiveis(obra_ref);
}

// ---------------------------------------------------------------------------
void __fastcall TObras::ReconfiguraRedeSemObra(void)
{
	// vari�veis locais
	VTObra *obra_ref = NULL;

	// monta a rede sem nenhuma Obra
	ReconfiguraRede(ObraFinalIncorporada(), NULL);
	// define como vis�veis as redes das obra
	DefineRedesVisiveis(obra_ref);
}

//https://sinapsisenergia.teamwork.com/#/tasks/16218705
// ---------------------------------------------------------------------------
void __fastcall TObras::ReconfiguraRedeTodaObra(VTObra *obra_ref)
{
	VTObra *obra_final = ObraFinal();

	// monta a rede com as altera��es da obra final
	ReconfiguraRede(ObraFinalIncorporada(), obra_final);
	// reinicia lisBLOCK
	if((obra_ref!=NULL) && (obra_ref != obra_final))
	{
		lisBLOCK->Clear();
		// monta conjunto de Eqptos bloqueados p/ edi��o
		MontaLisEqptoBloqueado(obra_ref, lisBLOCK);
	}
	// define como vis�veis as redes das obra indicada
	DefineRedesVisiveis(obra_ref);
}

// ---------------------------------------------------------------------------
void __fastcall TObras::Reinicia(void)
{
	// destr�i todos objetos TObra
	LimpaTList(lisOBRA);
}

/*
 //---------------------------------------------------------------------------
 void __fastcall TObras::RetiraObra(VTObra *obra)
 {
 //vari�veis locais
 VTEdita *edita   = (VTEdita*)apl->GetObject(__classid(VTEdita));

 //prote��o
 if (obra == NULL) return;
 //associa Edita c/ Obra
 edita->Obra = obra;
 //desfaz todas Acoes da Obra
 //while (edita->ActionUndo->Enabled) edita->ActionUndo->Execute();
 edita->UndoCompleto();
 //marca que a Obra n�o est� incorporada � Rede
 obra->Incorporada = false;
 //cancela associa��o de Edita c/ Obra
 if (edita->Obra == obra) edita->Obra = NULL;
 //verifica se � uma Obra j� gravada na base de dados ou uma Obra sem nenhuma Acao
 if ((obra->Id < 1)||(obra->LisAcao->Count == 0))
 {//obra nova: retira de lisOBRA e destr�i objeto
 lisOBRA->Remove(obra);
 delete(obra);
 }
 else
 {//obra gravada na base de dados: transfere p/ lisLIXO
 lisOBRA->Remove(obra);
 lisLIXO->Add(obra);
 //marca que h� modifica��o
 Modificada = true;
 }
 }
 */
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15938942
void __fastcall TObras::RetiraObra(VTObra *obra)
{
	// vari�veis locais
	VTMultiObraBO *multiObraBO = (VTMultiObraBO*)apl->GetObject(__classid(VTMultiObraBO));
	int index;
	VTObra *obra_ant;

	// prote��o
	if (obra == NULL)
		return;
	// determina Obra anterior � Obra indicada
	index = lisOBRA->IndexOf(obra);
	if (index <= 0)
		obra_ant = NULL;
	else
		obra_ant = (VTObra*)lisOBRA->Items[index - 1];
	// reconfigura Rede at� Obra anterior
	ReconfiguraRedeAteObra(obra_ant);
	// verifica se � uma Obra j� gravada na base de dados ou uma Obra sem nenhuma Acao
	if ((obra->Id < 1) || (obra->LisAcao->Count == 0))
	{ // obra nova: retira de lisOBRA e destr�i objeto
		lisOBRA->Remove(obra);
		if (multiObraBO)
		{
			multiObraBO->ReiniciaMultiObra(obra);
		}
		delete(obra);
	}
	else
	{ // obra gravada na base de dados: transfere p/ lisLIXO
		lisOBRA->Remove(obra);
        if (multiObraBO)
		{
			multiObraBO->ReiniciaMultiObra(obra);
		}
		lisLIXO->Add(obra);
		// marca que h� modifica��o
		Modificada = true;
	}
}

// ---------------------------------------------------------------------------
bool __fastcall TObras::VerificaEqptoBloqueado(VTEqpto *eqpto)
{
	return (lisBLOCK->IndexOf(eqpto) >= 0);
}

// ---------------------------------------------------------------------------
bool __fastcall TObras::VerificaPossibilidadeExclusao(VTObra *obra)
{
	// vari�veis locais
	VTBarra *barra;
	VTEqpto *eqpto;
	VTObra *obra_2;

	// verifica se a Obra � �nica
	if (lisOBRA->Count == 1)
		return (true);
	// C�DIGO TEMPORARIAMENTE BLOQUEADO - precisa ser revisto
	/*
	 //monta lista de Eqptos alterados ou inseridos pela obra_1
	 lisEQP_1->Clear();
	 MontaLisEqptoDaObra(obra->LisAcao, acaoALTERA, lisEQP_1);
	 MontaLisEqptoDaObra(obra->LisAcao, acaoINSERE, lisEQP_1);
	 //MontaLisEqptoDaObra(obra, acaoRETIRA, lisEQP_1);
	 //verifica se existe alguma Obra posterior que depende desta Obra
	 for (int n = lisOBRA->Count-1; n >= 0; n--)
	 {
	 obra_2 = (VTObra*)lisOBRA->Items[n];
	 //verifica se obra_2 � igual � Obra indicada
	 if (obra_2 == obra) break;
	 //monta lista de Eqptos alterados, inseridos e retirados pela obra_2
	 lisEQP_2->Clear();
	 MontaLisEqptoDaObra(obra_2->LisAcao, acaoALTERA, lisEQP_2);
	 MontaLisEqptoDaObra(obra_2->LisAcao, acaoINSERE, lisEQP_2);
	 MontaLisEqptoDaObra(obra_2->LisAcao, acaoRETIRA, lisEQP_2);
	 //monta lista de Barras usadas pela obra_2
	 MontaLisBarraUsadaNaObra(obra_2->LisAcao, acaoALTERA, lisEQP_2);
	 MontaLisBarraUsadaNaObra(obra_2->LisAcao, acaoINSERE, lisEQP_2);
	 MontaLisBarraUsadaNaObra(obra_2->LisAcao, acaoRETIRA, lisEQP_2);
	 //verifica se a obra_2 depende da obra indicada
	 if (Lista1IntersecaoLista2(lisEQP_1, lisEQP_2) > 0) return(false);
	 }
	 */
	return (false);
}

// ---------------------------------------------------------------------------
// eof
