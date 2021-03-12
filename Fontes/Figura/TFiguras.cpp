// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DLL_Inc\Funcao.h>
#include "..\Apl\VTApl.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include "TFiguras.h"
#include "..\Edita\VTEdita.h"
#include "..\Estilos\VTEstilos.h"
#include "..\Estilos\VTEstilo.h"
#include "..\Estilos\VTEstiloComentario.h"
#include "..\Estilos\VTEstiloRetangulo.h"
#include "..\Rede\VTRedes.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "VTFiguraComentario.h"
#include "VTLink.h"
#include "VTRetangulo.h"
#include "VTFigura.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTFiguras* __fastcall NewObjFiguras(VTApl *apl_owner)
{
	return (new TFiguras(apl_owner));
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
__fastcall TFiguras::TFiguras(VTApl *apl_owner)
{

	// salva ponteiros para objetos
	this->apl = apl_owner;
	// salva o ponteiro para o estilos
	VTEstilos *estilos = (VTEstilos*)apl_owner->GetObject(__classid(VTEstilos));

	estudo = NULL;
	if (estilos == NULL)
	{
		apl->Add(estilos = NewObjEstilos(apl));
	}
	// cria listas
	lisCOMENTARIO = new TList();
	lisRETANGULO = new TList();

}

// ---------------------------------------------------------------------------
__fastcall TFiguras::~TFiguras(void)
{
	// destrói lista e seus objetos
	if (lisCOMENTARIO)
	{
		LimpaTList(lisCOMENTARIO);
		delete lisCOMENTARIO;
		lisCOMENTARIO = NULL;
	}
	if (lisRETANGULO)
	{
		LimpaTList(lisRETANGULO);
		delete lisRETANGULO;
		lisRETANGULO = NULL;
	}
}

// ---------------------------------------------------------------------------
VTAlternativa* __fastcall TFiguras::AlternativaAtiva(void)
{
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTAlternativa *alt = NULL;

	if (planejamento != NULL)
	{
		estudo = planejamento->ExisteEstudo();
	}
	else
	{
		estudo = NULL;
	}
	if(estudo != NULL)
	{
		alt = estudo->AlternativaAtiva;
	}

	return alt;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
VTFiguraComentario* __fastcall TFiguras::ComentarioPai(VTLink *link)
{
	VTLink *auxLink;
	VTFiguraComentario *auxComentario, *encontrado;
	TList *lisLink = NULL;

	encontrado = NULL;
	try
	{
		// percorre todos os comentarios à procura de quem possui o link
		for (int nc = 0; nc < lisCOMENTARIO->Count; nc++)
		{
			auxComentario = (VTFiguraComentario*)lisCOMENTARIO->Items[nc];
			lisLink = auxComentario->LisLink();
			// percorre todos os links
			for (int nl = 0; nl < lisLink->Count; nl++)
			{
				auxLink = (VTLink*)lisLink->Items[nl];
				if (auxLink == link)
				{
					encontrado = auxComentario;
					return encontrado;
				}
			}
		}
	}
	catch (...)
	{
		encontrado = NULL;
	}

	return encontrado;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
VTFiguraComentario* __fastcall TFiguras::ComentarioPai(VTRetangulo *retangulo)
{
	VTLink *auxLink;
	VTFiguraComentario *auxComentario, *encontrado;
	TList *lisLink = NULL;

	encontrado = NULL;
	try
	{
		// percorre todos os comentarios à procura de quem possui o retangulo
		for (int nc = 0; nc < lisCOMENTARIO->Count; nc++)
		{
			auxComentario = (VTFiguraComentario*)lisCOMENTARIO->Items[nc];
			if (auxComentario->Retangulo == retangulo)
			{
				encontrado = auxComentario;
				return encontrado;
			}
		}
	}
	catch (...)
	{
		encontrado = NULL;
	}

	return encontrado;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TFiguras::DestroiFigura(VTFigura *figura)
{
	bool sucesso = true;
	VTFiguraComentario *comentario;

	if (figura == NULL)
	{
		return false;
	}
	try
	{
		switch (figura->TipoFigura)
		{
		case figLINK:
			comentario = ComentarioPai((VTLink*) figura);
			if (comentario != NULL)
			{
				comentario->LisLink()->Remove(figura);
			}
			break;
		case figRETANGULO:
			comentario = ComentarioPai((VTRetangulo*) figura);
			if (comentario != NULL)
			{
				comentario->Retangulo = NULL;
			}
			// remove da lista retangulo
			lisRETANGULO->Remove(figura);
			break;
		case figCOMENTARIO:
			// remove da lista retangulo
			lisCOMENTARIO->Remove(figura);
			break;
		default:
			// faz nada diferente
			;
		}
		// destroi a figura
		delete figura;
		figura = NULL;
	}
	catch (...)
	{
		sucesso = false;
	}

	return sucesso;
}

// ---------------------------------------------------------------------------
bool __fastcall TFiguras::DeveInserir(strRETANGULO *ref)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	bool redesAllow = true;
	bool estudoAllow = true;
	bool deve = true;

	try
	{
		// verifica se a rede está presente
		redesAllow = ((redes->ExisteRede(ref->redeId) != NULL) || (ref->redeId == 0));
		// verifica se é do estudo certo
		if (PD.estudo != NULL)
		{
			estudoAllow = ((PD.estudo->Id == ref->estudoId) || (ref->estudoId == 0));
		}
	}
	catch (...)
	{
		deve = false;
	}

	return deve;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TFiguras::ExisteAlteracao(void)
{
	// variáveis locais
	VTFiguraComentario *comentario;
	VTLink *link;
	VTRetangulo *retangulo;
	TList *lisLINK;

	for (int n = 0; n < lisCOMENTARIO->Count; n++)
	{
		// Olha a o status dos comentários da lista
		comentario = (VTFiguraComentario*)lisCOMENTARIO->Items[n];
		if ((comentario->Status[sttEV_NOVO]) || (comentario->Status[sttEV_ALTERADO]) ||
			(comentario->Status[sttEV_EXCLUIDO]))
			return (true);
		lisLINK = comentario->LisLink();
		for (int n = 0; n < lisLINK->Count; n++)
		{
			// Olha a o status dos link's da lista no comentario
			link = (VTLink*)lisLINK->Items[n];
			if ((link->Status[sttEV_NOVO]) || (link->Status[sttEV_ALTERADO]) ||
				(link->Status[sttEV_EXCLUIDO]))
				return (true);
		}
	}
	for (int n = 0; n < lisRETANGULO->Count; n++)
	{
		// Olha a o status dos retángulos da lista
		retangulo = (VTRetangulo*)lisRETANGULO->Items[n];
		if ((retangulo->Status[sttEV_NOVO]) || (retangulo->Status[sttEV_ALTERADO]) ||
			(retangulo->Status[sttEV_EXCLUIDO]))
			return (true);
	}
	return (false);
}

// ---------------------------------------------------------------------------
VTObra* __fastcall TFiguras::ExisteObra(int obra_id)
{
	// Variáveis locais
	VTObra *obra = NULL;
	VTObra *aux = NULL;
	TList *lisOBRA = new TList;

	try
	{
		// Verifica se a obra é da Rede original
		if (obra_id == -1)
		{
			return (NULL);
		}
		if (PD.estudo == NULL)
		{
			return NULL;
		}
		PD.estudo->LisObraEstudo(lisOBRA);
		for (int no = 0; no < lisOBRA->Count; no++)
		{
			aux = (VTObra*)lisOBRA->Items[no];
			if (aux->Id == obra_id)
			{
				obra = aux;
				break;
			}
		}
		if (lisOBRA)
		{
			delete lisOBRA;
			lisOBRA = NULL;
		}
	}
	catch (...)
	{
		obra = NULL;
		if (lisOBRA)
		{
			delete lisOBRA;
			lisOBRA = NULL;
		}
	}
	return (obra);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
VTFiguraComentario* __fastcall TFiguras::ExisteComentario(int figura_id)
{
	// variáveis locais
	VTFiguraComentario *comentario;

	for (int n = 0; n < lisCOMENTARIO->Count; n++)
	{
		comentario = (VTFiguraComentario*)lisCOMENTARIO->Items[n];
		if (comentario->Id == figura_id)
			return (comentario);
	}
	return (NULL);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
VTFiguraComentario* __fastcall TFiguras::InsereComentario(bool novoEstilo)
{
	// variáveis locais
	VTEstilos *estilos = (VTEstilos*)apl->GetObject(__classid(VTEstilos));
	VTFiguraComentario *comentario;
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// cria novo objeto
	if ((comentario = NewObjFiguraComentario(apl)) != NULL)
	{
		// hoje em dia cada figura vai ter 1 estilo próprio, mas a ideia é que diversas figuras tenham o mesmo estilo
		if (novoEstilo)
		{
			if (estilos)
			{
				comentario->Estilo = estilos->InsereEstilo(comentario);
			}
		}

		comentario->Alternativa = AlternativaAtiva();
		comentario->Status[sttEV_NOVO] = false;
		lisCOMENTARIO->Add(comentario);
		if (edita != NULL)
		{
			comentario->Obra = edita->Obra;
		}
	}
	return (comentario);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
VTFiguraComentario* __fastcall TFiguras::InsereComentario(strRETANGULO *ref)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTEstilos *estilos = (VTEstilos*)apl->GetObject(__classid(VTEstilos));
	VTFiguraComentario *comentario = NULL;
	VTEstilo *estilo;
	VTEstiloComentario *estiloC;
	strUTM utm;
	strCOORD esq;

	// verifica se deve inserir
	if (DeveInserir(ref))
	{
		comentario = InsereComentario(false);
		// atribui valores
		// define formato fmtRETANGULO: importante na definição da dimensão
		comentario->Formato = fmtRETANGULO;
		// inicia atributos do Comentario
		comentario->Status[sttEV_NOVO] = false;
		comentario->Rede = redes->ExisteRede(ref->redeId);
		comentario->Obra = ExisteObra(ref->obraId);
		comentario->Id = ref->figuraId;
		comentario->Visible = ref->visible;
		comentario->EstudoId = ref->estudoId;
		// determina coordenadas
		esq.x = ref->esqX;
		esq.y = ref->esqY;
		utm.x = ref->utmX;
		utm.y = ref->utmY;
		comentario->Retangulo->SetCentro(utm, esq);
		// determina dimensao
		comentario->Retangulo->SetDimensao(ref->dx, ref->dy);
		// determina parametros de estilo
		if ((estilo = estilos->ExisteEstilo(ref->estiloId)) != NULL)
		{
			if (estilo->TipoEstilo == estiloCOMENTARIO)
			{
				estiloC = (VTEstiloComentario*) estilo;
				comentario->Estilo = estilo;
                comentario->AplicaEstilo(estilo);
			}

		}
	}

	return comentario;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TFiguras::InsereComentario(VTFiguraComentario *comentario)
{
	VTEdita *edita = (VTEdita*)apl->GetObject(__classid(VTEdita));

	// proteção
	if (comentario == NULL)
		return (false);
	// insere Comentario na lista
	if (lisCOMENTARIO->IndexOf(comentario) < 0)
	{
		if (comentario->Alternativa == NULL)
		{
			comentario->Alternativa = AlternativaAtiva();
		}
		lisCOMENTARIO->Add(comentario);
		comentario->Alternativa = AlternativaAtiva();
		if (edita != NULL)
		{
			comentario->Obra = edita->Obra;
		}
	}
	return (true);
}

// ---------------------------------------------------------------------------
VTRetangulo* __fastcall TFiguras::InsereRetangulo(bool novoEstilo)
{
	// variáveis locais
	VTEstilos *estilos = (VTEstilos*)apl->GetObject(__classid(VTEstilos));
	VTRetangulo *retangulo;

	// cria novo objeto
	if ((retangulo = NewObjRetangulo(apl)) != NULL)
	{
		if (novoEstilo)
		{
			// hoje em dia cada figura vai ter 1 estilo próprio, mas a ideia é que diversas figuras tenham o mesmo estilo
			if (estilos)
			{
				retangulo->Estilo = estilos->InsereEstilo(retangulo);
			}
		}
		if (retangulo->Alternativa == NULL)
		{
			retangulo->Alternativa = AlternativaAtiva();
		}
		retangulo->Status[sttEV_NOVO] = false;
		lisRETANGULO->Add(retangulo);
	}
	return (retangulo);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
VTRetangulo* __fastcall TFiguras::InsereRetangulo(strRETANGULO *ref)
{
	VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	VTEstilos *estilos = (VTEstilos*)apl->GetObject(__classid(VTEstilos));
	VTRetangulo *retangulo = NULL;
	VTEstilo *estilo;
	VTEstiloRetangulo *estiloR;
	strUTM utm;
	strCOORD esq;

	// verifica se deve inserir
	if (DeveInserir(ref))
	{
		retangulo = InsereRetangulo(false);
		// atribui valores
		// inicia atributos do Comentario
		retangulo->Status[sttEV_NOVO] = false;
		retangulo->Id = ref->figuraId;
		retangulo->Visible = ref->visible;
		retangulo->EstudoId = ref->estudoId;
		retangulo->Obra = ExisteObra(ref->obraId);
		// determina coordenadas
		esq.x = ref->esqX;
		esq.y = ref->esqY;
		utm.x = ref->utmX;
		utm.y = ref->utmY;
		retangulo->SetCentro(utm, esq);
		// determina dimensao
		retangulo->SetDimensao(ref->dx, ref->dy);
		// determina parametros de estilo
		if ((estilo = estilos->ExisteEstilo(ref->estiloId)) != NULL)
		{
			if (estilo->TipoEstilo == estiloRETANGULO)
			{
				estiloR = (VTEstiloRetangulo*) estilo;
				retangulo->Estilo = estilo;
				retangulo->AplicaEstilo(estilo);
			}
		}
	}

	return retangulo;
}

// ---------------------------------------------------------------------------
bool __fastcall TFiguras::InsereRetangulo(VTRetangulo *retangulo)
{
	// proteção
	if (retangulo == NULL)
		return (false);
	// insere Retangulo na lista
	if (lisRETANGULO->IndexOf(retangulo) < 0)
	{
		lisRETANGULO->Add(retangulo);
	}
    if (retangulo->Alternativa == NULL)
	{
		retangulo->Alternativa = AlternativaAtiva();
	}
	return (true);
}

// ---------------------------------------------------------------------------
TList* __fastcall TFiguras::LisComentario(void)
{
	return (lisCOMENTARIO);
}

// ---------------------------------------------------------------------------
TList* __fastcall TFiguras::LisRetangulo(void)
{
	return (lisRETANGULO);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TFiguras::ObraAtiva(VTObra *obra)
{
	// Variáveis locais
	VTFiguraComentario *comentario;
	VTRetangulo *retangulo;

	for (int nc = 0; nc < lisCOMENTARIO->Count; nc++)
	{
		comentario = (VTFiguraComentario*)lisCOMENTARIO->Items[nc];
		comentario->Visible = (comentario->Obra == obra);
		if (comentario->Visible)
		{
			if (obra != NULL)
			{
				obra->AddRede(comentario->Rede);
			}
		}
	}

	for (int nr = 0; nr < lisRETANGULO->Count; nr++)
	{
		retangulo = (VTRetangulo*)lisRETANGULO->Items[nr];
		retangulo->Visible = (retangulo->Obra == obra);
	}
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TFiguras::RedefineIds(void)
{
	bool sucesso = true;
	VTFiguraComentario *coment = NULL;

	try
	{
		// redefine retangulos
		sucesso &= RedefineIds(lisRETANGULO);
		// redefine retangulos dos comentarios
		for (int nc = 0; nc < lisCOMENTARIO->Count; nc++)
		{
			coment = (VTFiguraComentario*) lisCOMENTARIO->Items[nc];
			sucesso &= RedefineIds(coment->Retangulo);
			// redefine links
			sucesso &= RedefineIds(coment->LisLink());
		}
		// redefine comentarios
		sucesso &= RedefineIds(lisCOMENTARIO);
	}
	catch (...)
	{
		sucesso = false;
	}

	return sucesso;
}

// ---------------------------------------------------------------------------
bool __fastcall TFiguras::RedefineIds(TList *lisELEM_VISUAL)
{
	bool sucesso = true;
	VTElemVisual *elem = NULL;

	try
	{
		if (lisELEM_VISUAL == NULL)
		{
			return false;
		}
		for (int ne = 0; ne < lisELEM_VISUAL->Count; ne++)
		{
			elem = (VTElemVisual*)lisELEM_VISUAL->Items[ne];
			sucesso &= RedefineIds(elem);
		}
	}
	catch (...)
	{
		sucesso = false;
	}

	return sucesso;
}

// ---------------------------------------------------------------------------
bool __fastcall TFiguras::RedefineIds(VTElemVisual *elem)
{
	bool sucesso = true;

	try
	{
		if (elem == NULL)
		{
			return false;
		}
		elem->Id = FalsoId();
		elem->Status[sttEV_NOVO] = true;
	}
	catch (...)
	{
		sucesso = false;
	}

	return sucesso;
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TFiguras::RetiraComentario(VTFiguraComentario *comentario)
{
	// proteção
	if (comentario == NULL)
		return (false);
	if (comentario->Status[sttEV_NOVO] == true)
	{
		// retira Comentario na lista e destrói objeto
		lisCOMENTARIO->Remove(comentario);
		delete comentario;
	}
	else
	{
		// D.V.C. 13/08/2019
		// Troca o status da Fig para excluido
		comentario->Status[sttEV_EXCLUIDO] = true;
	}
	return (true);
}

// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TFiguras::RetiraFigura(VTFigura *figura)
{
	// proteção
	if (figura == NULL)
		return (false);
	// retira Figura
	figura->Status[sttEV_EXCLUIDO] = true;
	// switch(figura->Tipo)
	// {
	// case figCOMENTARIO: return(RetiraComentario((VTFiguraComentario*)figura));
	// case figLINK:       return(RetiraLink((VTLink*)figura));
	// case figRETANGULO:  return(RetiraRetangulo((VTRetangulo*)figura));
	// }
	return (false);
}

// ---------------------------------------------------------------------------
bool __fastcall TFiguras::RetiraLink(VTLink *link)
{
	// proteção
	if (link == NULL)
		return (false);
	// retira Link do seu Comentario e destrói objeto
	if (link->Status[sttEV_NOVO] == true)
	{
		link->Comentario->RetiraLink(link);
	}
	else
	{
		// D.V.C. 13/08/2019
		// Troca o status da Fig para excluido
		link->Status[sttEV_EXCLUIDO] = true;
	}
	return (true);
}

// ---------------------------------------------------------------------------
bool __fastcall TFiguras::RetiraRetangulo(VTRetangulo *retangulo)
{
	// proteção
	if (retangulo == NULL)
		return (false);
	// retira Comentario na lista  no caso de ser novo e destrói objeto
	if (retangulo->Status[sttEV_NOVO] == true)
	{
		lisRETANGULO->Remove(retangulo);
		delete retangulo;
	}
	else
	{
		// D.V.C. 13/08/2019
		// Troca o status da Fig para excluido
		retangulo->Status[sttEV_EXCLUIDO] = true;
	}
	return (true);
}

// ---------------------------------------------------------------------------
// eof
