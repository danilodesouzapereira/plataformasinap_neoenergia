//---------------------------------------------------------------------------

#pragma hdrstop
#include <map>
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Figura\Estilos\VTEstilos.h>
#include <PlataformaSinap\Fontes\Figura\VTFiguras.h>
#include <PlataformaSinap\Fontes\Figura\VTFiguraComentario.h>
#include <PlataformaSinap\Fontes\Figura\VTLink.h>
#include <PlataformaSinap\Fontes\Obra\VTAcao.h>
#include <PlataformaSinap\Fontes\Rede\VTEqpto.h>
//---------------------------------------------------------------------------
#include "TTaggerDecorator.h"
#include "VTTagFormatter.h"
//#include "VTAutoTag.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TTaggerDecorator::TTaggerDecorator(VTApl *apl, VTTagger *tagger, VTTagFormatter *formatter)
{
	this->apl_ = apl;
	this->tagger_ = tagger;
	this->formatter_ = formatter;
}
//---------------------------------------------------------------------------
__fastcall TTaggerDecorator::~TTaggerDecorator(void)
{

	//destroi o parent
	if (tagger_)
	{
		delete tagger_;
		//teste
		#ifdef _DEBUG
//		Application->MessageBoxW(L" TTaggerDecorator destructor", L"Aviso", MB_OKCANCEL);
		#endif
	}
}
//---------------------------------------------------------------------------
bool __fastcall TTaggerDecorator::DeveDeletarComentario(VTFiguraComentario * comentario, AnsiString msgTag)
{
	bool deve = false;
	AnsiString aux = "";

	//verifica se somente possui uma linha com o texto automatico
	if (comentario != NULL)
	{
		if (comentario->Texto()->Count == 1)
		{
			if (msgTag.AnsiCompare(comentario->Texto()->Strings[0]) == 0)
			{
                return true;
			}
		}
	}

	return deve;
}
//---------------------------------------------------------------------------
VTFiguraComentario* __fastcall TTaggerDecorator::ExisteComentario(VTEqpto *eqpto)
{
	VTFiguraComentario *figuraComentario = NULL;
	std::map<VTEqpto*,VTFiguraComentario*>::iterator it;

	it = mapEQPTO_COMETARIO_.find(eqpto);
	if (it != mapEQPTO_COMETARIO_.end() )
	{
		figuraComentario = it->second;
	}

	return figuraComentario;
}
//---------------------------------------------------------------------------
void __fastcall  TTaggerDecorator::InicializaComentariosAutomaticos(void)
{
	VTFiguras *figuras = (VTFiguras*)apl_->GetObject(__classid(VTFiguras));
	VTFigura *figura;
	VTFiguraComentario *comentario;
	VTLink *link;
	VTEqpto *eqpto;
	TList *lisCOM = NULL;
	TList *lisLINK = NULL;

	lisCOM = figuras->LisComentario();
	//verifica todos comentarios
	for (int nf = 0; nf < lisCOM->Count; nf++)
	{
		figura = (VTFigura*)lisCOM->Items[nf];
        //verifica se é comentario
		if (figura->TipoFigura == figCOMENTARIO)
		{
			comentario = (VTFiguraComentario*)figura;
			//verifica se possui link
			lisLINK = comentario->LisLink();
			if (lisLINK->Count != 0)
			{
				//verifica se o texto é de comentario automatico
				if (IsComentarioAutomatico(comentario))
				{
					//sendo, insere todos pares eqpto comentario dos links
					for (int nl = 0; nl < lisLINK->Count; nl++)
					{
						link = (VTLink*)lisLINK->Items[nl];
						eqpto = link->Eqpto;
						if (eqpto != NULL)
						{
							mapEQPTO_COMETARIO_.insert(std::pair<VTEqpto*,VTFiguraComentario*>(eqpto, comentario));
						}
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
VTFiguraComentario* __fastcall TTaggerDecorator::InsereComentario(VTEqpto *eqpto)
{
	VTFiguras *figuras = (VTFiguras*)apl_->GetObject(__classid(VTFiguras));
	VTEstilos *estilos = (VTEstilos*)apl_->GetObject(__classid(VTEstilos));
	VTFiguraComentario *figuraComentario = NULL;

	bool criado = false;

	try
	{
		//verifica se existe comentario desse tipo associado ao eqpto

		if ((figuraComentario = ExisteComentario(eqpto)) == NULL)
		{
			figuraComentario = figuras->InsereComentario(false);
			if (figuraComentario->Id < 0)
			{
				figuraComentario->Status[sttEV_NOVO] = true;
			}
			criado = true;
		}

	}
	catch (...)
	{
		if ((figuraComentario) && criado)
		{
			delete figuraComentario;
			figuraComentario = NULL;
        }
    }

	return figuraComentario;
}
//---------------------------------------------------------------------------
bool __fastcall TTaggerDecorator::InsereTags(VTAcao *acao, bool undo)
{
	return(this->tagger_->InsereTags(acao,undo));
}
//---------------------------------------------------------------------------
bool __fastcall TTaggerDecorator::IsAcaoTipoEqpto(VTAcao *acao, int tipo_eqpto)
{
	VTEqpto *eqpto = NULL;

	if (acao == NULL)
		{return false;}
	if ((eqpto = acao->Eqpto()) == NULL)
		{return false;}
	if (eqpto->Tipo() != tipo_eqpto)
		{return false;}

    return true;
}
//---------------------------------------------------------------------------
bool    __fastcall TTaggerDecorator::IsComentarioAutomatico(VTFiguraComentario *comentario)
{
	if (comentario == NULL)
		{return false;}
	//verifica se possui texto automatico
	if (formatter_ != NULL)
	{
		return formatter_->PossuiTextoAutomatico(comentario);
	}

    return false;
}
//---------------------------------------------------------------------------
bool __fastcall TTaggerDecorator::RemoveTag(VTFiguraComentario *comentario, VTEqpto *eqpto)
{
	VTFiguras *figuras = (VTFiguras*)apl_->GetObject(__classid(VTFiguras));
	VTLink *link = NULL;
	bool sucesso = true;

	try
	{
		if (comentario == NULL)
			{return false;}
        if (eqpto == NULL)
			{return false;}
		//verifica se o comentario possui link pro eqpto
		if ((link = comentario->ExisteLink(eqpto)) == NULL)
            {return false;}
		//remove a figura
		figuras->RetiraComentario(comentario);
		//retira do map
		mapEQPTO_COMETARIO_.erase(eqpto);
	}
	catch (...)
	{
        sucesso = false;
    }

	return sucesso;
}
//---------------------------------------------------------------------------
