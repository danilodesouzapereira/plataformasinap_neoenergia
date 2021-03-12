// ---------------------------------------------------------------------------
#include <vcl.h>
#include <map>
#pragma hdrstop
#include "VTAvaliaAcoesEqpto.h"
#include "TNodeDetalhe.h"
// ---------------------------------------------------------------------------
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Curvas\VTCurva.h>
#include <PlataformaSinap\Fontes\Obra\VTAcao.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
#include <PlataformaSinap\Fontes\Rede\VTEqbar.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall VTAvaliaAcoesEqpto::VTAvaliaAcoesEqpto(VTApl *apl)
{
	this->apl = apl;
}

// ---------------------------------------------------------------------------
__fastcall VTAvaliaAcoesEqpto::~VTAvaliaAcoesEqpto(void)
{

}
// ---------------------------------------------------------------------------
VTEqpto* __fastcall VTAvaliaAcoesEqpto::DeterminaEqpto(VTAcao *acao)
{
	VTEqpto *eqpto = NULL;

	if (acao == NULL)
	{
		return eqpto;
	}
	eqpto = acao->CopiaDoEqptoAlterado();
	if (eqpto == NULL)
	{
		eqpto = acao->Eqpto();
	}

	return eqpto;
}
// ---------------------------------------------------------------------------
bool __fastcall VTAvaliaAcoesEqpto::MesmaCurva(VTCurva *curva1, VTCurva *curva2)
{
	int nv =0;

	if (curva1 == NULL)
		{return false;}
    if (curva2 == NULL)
		{return false;}
	//verifica se tem tipos diferentes
	if (curva1->Tipo != curva2->Tipo)
		{return false;}
	//verifica se tem patamares diferentes
	if (curva1->NumeroPontos() != curva2->NumeroPontos())
		{return false;}
	//numero de valores diferentes
	if (curva1->NumeroValores!= curva2->NumeroValores)
		{return false;}
	//verifica se possui valores diferentes
	nv = curva1->NumeroValores;
	//meio acoxambrado, vou usar a integral
	for(int ni = 0; ni < nv ; ni++)
	{
		if(!IsDoubleZero(curva1->Integral(ni) - curva2->Integral(ni)))
			{return false;}
	}

	return true;
}
// ---------------------------------------------------------------------------
bool __fastcall VTAvaliaAcoesEqpto::MesmoLocal(VTEqbar *eqbar1, VTEqbar *eqbar2)
{
	VTBarra *barra;
	bool mesmo = true;

	if(eqbar1 == NULL)
		{return false;}
	if(eqbar2 == NULL)
		{return false;}

	//verifica se as barras são iguais
	if (eqbar1->pbarra != eqbar2->pbarra)
		{return false;}

	return mesmo;
}
// ---------------------------------------------------------------------------
bool __fastcall VTAvaliaAcoesEqpto::MesmoLocal(VTLigacao *lig1, VTLigacao *lig2)
{
	VTBarra *barra;
	TList *listBARLIG;
	bool mesmo = true;

	if(lig1 == NULL)
		{return false;}
	if(lig2 == NULL)
		{return false;}

	listBARLIG = new TList;
	//preenche a lista com as barras da ligacoe 2
	for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
	{
		barra = lig2->Barra(ind_bar);
		if (barra != NULL)
		{
			listBARLIG->Add(barra);
		}
	}
	//verifica se as barras da ligacao 1 estao sendo usadas na ligacao 2
	for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
	{
		barra = lig1->Barra(ind_bar);
		if (barra == NULL)
			{continue;}
		if (listBARLIG->IndexOf(barra) < 0)
		{
			mesmo = false;
			break;
		}
	}

	//verifica o contrario
	if (mesmo == true)
	{
        listBARLIG->Clear();
		//preenche a lista com as barras da ligacoe 2
		for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
		{
			barra = lig1->Barra(ind_bar);
			if (barra != NULL)
			{
				listBARLIG->Add(barra);
			}
		}
		//verifica se as barras da ligacao 1 estao sendo usadas na ligacao 2
		for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
		{
			barra = lig2->Barra(ind_bar);
			if (barra == NULL)
				{continue;}
			if (listBARLIG->IndexOf(barra) < 0)
			{
				mesmo = false;
				break;
			}
		}
	}

	delete listBARLIG;
	return mesmo;
}
// ---------------------------------------------------------------------------
bool __fastcall  VTAvaliaAcoesEqpto::ValidaTipoEqpto(VTAcao *acao, int tipoEQPTO)
{
	VTEqpto *eqpto;

	if (acao == NULL)
		{return false;}
	eqpto = acao->Eqpto();
    if (eqpto == NULL)
		{return false;}

	//verifica o tipo
	if (eqpto->Tipo() != tipoEQPTO)
		{return false;}
	//chegou ate aqui, é true haha

    return true;
}
// ---------------------------------------------------------------------------
// eof
