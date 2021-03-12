// ---------------------------------------------------------------------------
#pragma hdrstop
#include "TEstratSoc.h"
#include "VTVisRedeSoc.h"
#include "VTVisRede.h"
#include "VTBlocoRad.h"
#include "VTDefeito.h"
#include <PlataformaSinap\Fontes\Bloco\VTBloco.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTEstratSoc* __fastcall NewObjEstratSoc(int npat)
{
	return (new TEstratSoc(npat));
}

// ---------------------------------------------------------------------------
__fastcall TEstratSoc::TEstratSoc(int npat)
{
	this->npat = npat;
	pdef = NULL;
	lisVisRedeSoc = new TList();
	lisBlocosSoc = new TList();
	lisBlocosReparo = new TList(); // lista de blocos não socorridos, que permanecem em reparo
}

// ---------------------------------------------------------------------------
__fastcall TEstratSoc::~TEstratSoc(void)
{
	if (lisVisRedeSoc)
	{
		LimpaTList(lisVisRedeSoc);
		delete lisVisRedeSoc;
		lisVisRedeSoc = NULL;
	}
	if (lisBlocosSoc)
	{
		delete lisBlocosSoc;
		lisBlocosSoc = NULL;
	}
	if (lisBlocosReparo)
	{
		delete lisBlocosReparo;
		lisBlocosReparo = NULL;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TEstratSoc::Inicia(void)
{
	LimpaTList(lisVisRedeSoc);
	lisBlocosSoc->Clear();
	lisBlocosReparo->Clear(); // lista de blocos não socorridos, que permanecem em reparo
	cargaMw_Transf = 0.0; // carga transferida
	iAmp_Transf = 0.0; // Corrente transferida
	iAmp_Ligacao = 0.0; // Corrente existente no trecho crítico
	iAmp_AdmLiga = 0.0; // Corrente admissivel no trecho critico
	puTransf = 99.0; // valor mínimo caso não haja transferência
}

// ---------------------------------------------------------------------------
bool __fastcall TEstratSoc::AdicionaVisRedeSoc(VTVisRede *visRede, TList *lisBlocosSoc,
	TList *lisBlRadJus)
{
	VTVisRedeSoc *visRedeSoc;
	VTBloco *bloco;
	VTBlocoRad *blRad;

	// Adiciona visRedeSoc de 1 estágio

	// Remove manobra de chave vis
	for (int n = 0; n < lisVisRedeSoc->Count; n++)
	{
		visRedeSoc = (VTVisRedeSoc*)lisVisRedeSoc->Items[n];
		if (visRedeSoc->visRede == visRede)
		{
			delete visRedeSoc;
			lisVisRedeSoc->Delete(n);
			break;
		}
	}
	// Cria manobra de chave Vis
	lisVisRedeSoc->Add(visRedeSoc = NewObjVisRedeSoc(npat)); // new TVisRedeSoc(npat));

	visRedeSoc->lisBlocosSoc->Clear();
	visRedeSoc->lisBlRadIsola->Clear();
	visRedeSoc->lisChvIsola->Clear();
	visRedeSoc->visRede = visRede;
	visRedeSoc->lisBlocosSoc->Assign(lisBlocosSoc, laOr);

	visRedeSoc->lisBlRadIsola->Assign(lisBlRadJus, laOr);
	for (int nb = 0; nb < lisBlRadJus->Count; nb++)
	{
		blRad = (VTBlocoRad*)lisBlRadJus->Items[nb];
		visRedeSoc->lisChvIsola->Add(blRad->Chave);
	}

	// Define a rede de socorro para a transferencia dos blocos
	if (this->pdef->Rede == visRedeSoc->visRede->rede1)
		visRedeSoc->redeTransf = visRedeSoc->visRede->rede2;
	else
		visRedeSoc->redeTransf = visRedeSoc->visRede->rede1;

	// Adiciona os blocos socorridos da estrategia de npat
	lisBlocosSoc->Assign(lisBlocosSoc, laOr);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEstratSoc::AdicionaVisRedeSoc1(VTVisRede *visRede, VTBlocoRad *blRadRapida,
	VTBlocoRad *blRadLenta)
{
	VTVisRedeSoc *visRedeSoc;
	VTBloco *bloco;

	// Remove manobra de chave vis
	for (int n = 0; n < lisVisRedeSoc->Count; n++)
	{
		visRedeSoc = (VTVisRedeSoc*)lisVisRedeSoc->Items[n];
		if (visRedeSoc->visRede == visRede)
		{
			delete visRedeSoc;
			lisVisRedeSoc->Delete(n);
			break;
		}
	}

	// Verifica se existe isolamento a jusante da falha
	if ((blRadRapida == NULL) && (blRadLenta == NULL))
		return (false);

	// Cria manobra de chave Vis
	lisVisRedeSoc->Add(visRedeSoc = NewObjVisRedeSoc(npat)); // new TVisRedeSoc(npat));
	visRedeSoc->visRede = visRede;

	// Adiciona aos blocos socorridos blocos da 2a etapa lenta
	visRedeSoc->lisBlocosSoc->Clear();
	if (blRadLenta != NULL)
	{
		visRedeSoc->lisBlocosSoc->Add(blRadLenta->Bloco);
		visRedeSoc->lisBlocosSoc->Assign(blRadLenta->LisBlocoJus, laOr);
	}
	else
	{
		visRedeSoc->lisBlocosSoc->Add(blRadRapida->Bloco);
		visRedeSoc->lisBlocosSoc->Assign(blRadRapida->LisBlocoJus, laOr);
	}

	if (blRadRapida != NULL)
	{
		// Isolamento inicial etapa rápida
		visRedeSoc->blRadRapida = blRadRapida;
		visRedeSoc->lisBlocosSocRapida->Clear();
		visRedeSoc->lisBlocosSocRapida->Add(blRadRapida->Bloco);
		visRedeSoc->lisBlocosSocRapida->Assign(blRadRapida->LisBlocoJus, laOr);
	}
	if (blRadLenta != NULL)
	{
		// Isolamento final etapa lenta
		visRedeSoc->blRadLenta = blRadLenta;
		visRedeSoc->lisBlocosSocLenta->Clear();
		visRedeSoc->lisBlocosSocLenta->Add(blRadLenta->Bloco);
		visRedeSoc->lisBlocosSocLenta->Assign(blRadLenta->LisBlocoJus, laOr);
		if (blRadRapida != NULL)
		{
			visRedeSoc->lisBlocosSocLenta->Remove(blRadRapida->Bloco); // mmmuuuuddddaaa
			for (int n = 0; n < blRadRapida->LisBlocoJus->Count; n++)
				visRedeSoc->lisBlocosSocLenta->Remove(blRadRapida->LisBlocoJus->Items[n]);
		}
	}

	// Define a rede de socorro para a transferencia dos blocos
	if (this->pdef->Rede == visRedeSoc->visRede->rede1)
		visRedeSoc->redeTransf = visRedeSoc->visRede->rede2;
	else
		visRedeSoc->redeTransf = visRedeSoc->visRede->rede1;

	// Adiciona os blocos socorridos da estrategia de npat
	lisBlocosSoc->Assign(visRedeSoc->lisBlocosSoc, laOr);
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TEstratSoc::BlocoSoc(VTBloco* bloco)
{

	if (lisBlocosSoc->IndexOf(bloco) != -1)
		return (true);
	// for(int n = 0; n < lisVisRedeSoc->Count; n++)
	// if(((TVisRedeSoc*)lisVisRedeSoc->Items[n])->lisBlocosSoc->IndexOf(bloco) != -1) return(true);

	return (false);
}

// ---------------------------------------------------------------------------
void __fastcall TEstratSoc::ListaVisRede(TList *lisEXT)
{
	for (int n = 0; n < lisVisRedeSoc->Count; n++)
		lisEXT->Add(((VTVisRedeSoc*)lisVisRedeSoc->Items[n])->visRede);
}

// ---------------------------------------------------------------------------
void __fastcall TEstratSoc::ListaRedeTransf(TList *lisEXT)
{
	for (int n = 0; n < lisVisRedeSoc->Count; n++)
		lisEXT->Add(((VTVisRedeSoc*)lisVisRedeSoc->Items[n])->redeTransf);
}

// ---------------------------------------------------------------------------
void __fastcall TEstratSoc::ListaChvIsola(TList *lisEXT)
{
	for (int n = 0; n < lisVisRedeSoc->Count; n++)
		lisEXT->Assign(((VTVisRedeSoc*)lisVisRedeSoc->Items[n])->lisChvIsola, laOr);
}

// ---------------------------------------------------------------------------
int __fastcall TEstratSoc::PM_GetNBlocosSoc(void)
{
	int nblocos = 0;

	for (int n = 0; n < lisVisRedeSoc->Count; n++)
		nblocos += ((VTVisRedeSoc*)lisVisRedeSoc->Items[0])->lisBlocosSoc->Count;
	return (nblocos);
}
// ---------------------------------------------------------------------------
