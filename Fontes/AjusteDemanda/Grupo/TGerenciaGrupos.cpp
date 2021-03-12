// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
// #include <Math.hpp>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\Fontes\AjusteDemanda\Grupo\VTGrupos.h>
#include <PlataformaSinap\Fontes\AjusteDemanda\Grupo\VTGrupo.h>
#include <PlataformaSinap\Fontes\ImportaMedicao\VTMedicao.h>
#include <PlataformaSinap\Fontes\ImportaMedicao\VTMedicoes.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include "..\Constante\Const.h"
#include "TGerenciaGrupos.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTGerenciaGrupos* __fastcall NewObjGerenciaGrupos(void)
{
	try
	{
		return (new TGerenciaGrupos());
	}
	catch (Exception &e)
	{
		return (NULL);
	}
}

// ---------------------------------------------------------------------------
__fastcall TGerenciaGrupos::TGerenciaGrupos(void)
{
	lisGRUPOS = new TList;
}

// ---------------------------------------------------------------------------
__fastcall TGerenciaGrupos::~TGerenciaGrupos(void)
{
	// destroi a lista sem destruir os objetos
	if (lisGRUPOS)
	{
		LimpaTList(lisGRUPOS);
		delete lisGRUPOS;
		lisGRUPOS = NULL;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TGerenciaGrupos::Clear(void)
{
	LimpaTList(lisGRUPOS);
    mapGRUPOS.clear();
}
// ---------------------------------------------------------------------------
void __fastcall TGerenciaGrupos::CriaGrupos(TList *lisMEDICAO_VALIDAS)
{
	VTMedicao *medicao;
	VTGrupos *grupos;

	this->Clear();
	if (lisMEDICAO_VALIDAS != NULL)
	{
		//percorre todas medicoes criando os grupos
		for (int i = 0; i < lisMEDICAO_VALIDAS->Count; i++)
		{
			medicao = (VTMedicao*)lisMEDICAO_VALIDAS->Items[i];
			grupos = InsereGrupos(medicao);
            grupos->ClassificaMedicao();
		}
	}
}
// ---------------------------------------------------------------------------
VTGrupos* __fastcall TGerenciaGrupos::ExisteGrupos(VTMedicao *medicao)
{
	VTGrupos *grupos = NULL;
	std::map<VTMedicao*, VTGrupos*>::iterator it;

	if (medicao == NULL)
		{return NULL;}
	it = mapGRUPOS.find(medicao);
	if (it != mapGRUPOS.end())
	{
		grupos = it->second;
	}

	return grupos;
}
  // ---------------------------------------------------------------------------
void __fastcall TGerenciaGrupos::Inicia(void)
{
	VTGrupos *grupos = NULL;

	//percorre todos grupos iniciando a lista de cargas ajustaveis
	for (int ng = 0; ng < lisGRUPOS->Count; ng++)
	{
		grupos = (VTGrupos*) lisGRUPOS->Items[ng];
        grupos->Inicia();
	}
}
// ---------------------------------------------------------------------------
VTGrupos* __fastcall TGerenciaGrupos::InsereGrupos(VTMedicao *medicao)
{
	VTGrupos *grupos = NULL;

	if (medicao != NULL)
	{
		if ((grupos = ExisteGrupos(medicao)) == NULL)
		{   //cria um grupo
			grupos = NewObjGrupos(medicao);
			InsereGrupos(grupos);
		}
	}
	return grupos;
}
// ---------------------------------------------------------------------------
void __fastcall TGerenciaGrupos::InsereGrupos(VTGrupos *grupos)
{
	if (grupos != NULL)
	{
		if (lisGRUPOS->IndexOf(grupos) < 0)
		{
			lisGRUPOS->Add(grupos);
			mapGRUPOS[grupos->Medicao] = grupos;
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TGerenciaGrupos::PreencheLisGrupos(TList *lisEXT)
{
	if (lisEXT != NULL)
	{
		lisEXT->Clear();
		lisEXT->Assign(lisGRUPOS, laCopy);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TGerenciaGrupos::SetCargasAjustavelGrupo(bool ajustavel, int categoria)
{
	VTGrupos *grupos = NULL;
	VTGrupo *grupo = NULL;
	VTCarga *carga = NULL;
	TList *lisGRUPO = new TList;

	for (int ngs = 0; ngs < lisGRUPOS->Count; ngs++)
	{
		grupos = (VTGrupos*)lisGRUPOS->Items[ngs];
		grupos->PreencheLisGrupo(lisGRUPO);
		for (int ng = 0; ng < lisGRUPO->Count; ng++)
		{
			grupo = (VTGrupo*)lisGRUPO->Items[ng];
			if (grupo->categoria == categoria)
			{
				if (!ajustavel)
				{
					grupo->peso = 0;
				}
				else
				{
					//?? comofas?
				}
				for (int nc = 0; nc < grupo->lisCargas->Count; nc++)
				{
					carga = (VTCarga*)grupo->lisCargas->Items[nc];
					carga->Ajustavel = ajustavel;
				}
			}
		}
	}
	if (lisGRUPO)
	{
		delete lisGRUPO;
		lisGRUPO = NULL;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TGerenciaGrupos::SetCargasAjustavelCategoria(bool ajustavel, TList *lisCARGA, int categoria)
{
	VTCarga *carga = NULL;

	if (lisCARGA != NULL)
	{
		for (int i = 0; i < lisCARGA->Count; i++)
		{
			carga = (VTCarga*) lisCARGA->Items[i];
			if (carga->Categoria == categoria)
			{
                carga->Ajustavel = ajustavel;
			}
		}
	}
}
// ---------------------------------------------------------------------------
void __fastcall TGerenciaGrupos::SetPesoGrupo(double peso, int categoria)
{
    VTGrupos *grupos = NULL;
	VTGrupo *grupo = NULL;
	VTCarga *carga = NULL;
	TList *lisGRUPO = new TList;

	for (int ngs = 0; ngs < lisGRUPOS->Count; ngs++)
	{
		grupos = (VTGrupos*)lisGRUPOS->Items[ngs];
		grupos->PreencheLisGrupo(lisGRUPO);
		for (int ng = 0; ng < lisGRUPO->Count; ng++)
		{
			grupo = (VTGrupo*)lisGRUPO->Items[ng];
			if (grupo->categoria == categoria)
			{
				grupo->peso = peso;
			}
		}
	}
	if (lisGRUPO)
	{
		delete lisGRUPO;
		lisGRUPO = NULL;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TGerenciaGrupos::DEBUG_CategoriaAleatoria(TList *lisCARGAS)
{
	VTCarga *carga = NULL;

	if (lisCARGAS != NULL)
	{
		for (int i = 0; i < lisCARGAS->Count; i++)
		{
			carga = (VTCarga*) lisCARGAS->Items[i];
			carga->Categoria = Random(catAD_NAO_INSPEC) + 1; //+1 pq 0 nao eh valido
		}
	}
}
// ---------------------------------------------------------------------------
// eof
