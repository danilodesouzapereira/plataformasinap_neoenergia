// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
// #include <Math.hpp>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\Fontes\AjusteDemanda\Grupo\VTGrupo.h>
#include <PlataformaSinap\Fontes\ImportaMedicao\VTMedicao.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include "TGrupos.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTGrupos * __fastcall NewObjGrupos(VTMedicao *medicao)
{
	try
	{
		return (new TGrupos(medicao));
	}
	catch (Exception &e)
	{
		return (NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TGrupos::TGrupos(VTMedicao *medicao)
{
	if (medicao == NULL)
	{
		throw Exception("ponteiro nulo para medicao");
	}
	lisGRUPO = new TList;
	this->medicao = medicao;
}

// ---------------------------------------------------------------------------
__fastcall TGrupos::~TGrupos(void)
{
	// destroi a lista sem destruir os objetos
	if (lisGRUPO)
	{
		LimpaTList(lisGRUPO);
		delete lisGRUPO;
		lisGRUPO = NULL;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TGrupos::AtualizaProporcao(int fase)
{
	VTGrupo *grupo = NULL;
	double p_total_mw = 0;
	double q_total_mvar = 0;

	if (fase >= MAX_FASE)
	{
		throw Exception("fase inválida em TGrupos::AtualizaProporcao");
	}
	//percorre todos grupos totalizando as quantidades
	for (int ng = 0; ng < lisGRUPO->Count; ng++)
	{
		grupo = (VTGrupo*)lisGRUPO->Items[ng];
		p_total_mw += grupo->peso * grupo->p_car_ajus_mw[fase];
		q_total_mvar += grupo->peso * grupo->q_car_ajus_mvar[fase];
	}
	//calcula as proporcoes
	for (int ng = 0; ng < lisGRUPO->Count; ng++)
	{
		grupo = (VTGrupo*)lisGRUPO->Items[ng];
		if (!IsDoubleZero(p_total_mw))
		{
			grupo->proporcaoP[fase] = (grupo->peso * grupo->p_car_ajus_mw[fase]) / p_total_mw;
		}
		else
		{
			grupo->proporcaoP[fase] = 0;
		}
		if (!IsDoubleZero(q_total_mvar))
		{
			grupo->proporcaoQ[fase] = (grupo->peso * grupo->q_car_ajus_mvar[fase]) / q_total_mvar;;
		}
		else
		{
			grupo->proporcaoQ[fase] = 0;
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall  TGrupos::ClassificaMedicao(void)
{
	VTCarga *carga;
	VTGrupo *grupo;

	//percorre todas as cargas ajustaveis da medicao
	for (int i = 0; i < medicao->lisCAR_AJUS->Count; i++)
	{
		carga = (VTCarga*) medicao->lisCAR_AJUS->Items[i];
		//verifica a categoria
		grupo = InsereGrupo(carga->Categoria);
		grupo->lisCargas->Add(carga);
	}


}
// ---------------------------------------------------------------------------
VTGrupo* __fastcall TGrupos::ExisteGrupo(int categoria)
{
	VTGrupo *grupo = NULL;
	std::map<int, VTGrupo*>::iterator it;

	it = mapGRUPO.find(categoria);
	if (it != mapGRUPO.end())
	{
		grupo = it->second;
	}

	return grupo;
}
// ---------------------------------------------------------------------------
void     __fastcall TGrupos::Inicia(void)
{
	VTGrupo *grupo = NULL;


	for (int ng = 0; ng < lisGRUPO->Count; ng++)
	{
		grupo = (VTGrupo*)lisGRUPO->Items[ng];
        grupo->Inicia();
	}
}
// ---------------------------------------------------------------------------
VTGrupo* __fastcall TGrupos::InsereGrupo(int categoria)
{
	VTGrupo *grupo = NULL;

	if ((grupo = ExisteGrupo(categoria)) == NULL)
	{   //cria um grupo
		grupo = NewObjGrupo();
        grupo->categoria = categoria;
		InsereGrupo(grupo);
	}
	return grupo;
}
// ---------------------------------------------------------------------------
void __fastcall TGrupos::InsereGrupo(VTGrupo *grupo)
{
	if (grupo != NULL)
	{
		if (lisGRUPO->IndexOf(grupo) < 0)
		{
			lisGRUPO->Add(grupo);
			mapGRUPO[grupo->categoria] = grupo;
		}
	}
}
// ---------------------------------------------------------------------------
VTMedicao* __fastcall TGrupos::PM_GetMedicao(void)
{
	return (this->medicao);
}
// ---------------------------------------------------------------------------
void __fastcall TGrupos::PreencheLisGrupo(TList *lisEXT)
{
	if (lisEXT != NULL)
	{
		lisEXT->Clear();
		lisEXT->Assign(lisGRUPO, laCopy);
	}
}
// ---------------------------------------------------------------------------
// eof
