//---------------------------------------------------------------------------

#pragma hdrstop
//---------------------------------------------------------------------------
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Figura\VTFiguras.h>
#include <PlataformaSinap\Fontes\Figura\VTFiguraComentario.h>
#include <PlataformaSinap\Fontes\Figura\VTRetangulo.h>
#include <PlataformaSinap\Fontes\Figura\Estilos\VTEstilo.h>
#include <PlataformaSinap\Fontes\Figura\Estilos\VTEstiloComentario.h>
#include <PlataformaSinap\Fontes\Figura\Estilos\VTEstilos.h>
#include <PlataformaSinap\Fontes\Obra\VTAcao.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTEqbar.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
//---------------------------------------------------------------------------
#include "TTagFormatter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTTagFormatter* __fastcall NewObjTagFormatter(VTApl *apl)
{
	try
	{
		return (new TTagFormatter(apl));
	}catch (Exception &e)
	{
		return(NULL);
	}
}
//---------------------------------------------------------------------------
__fastcall TTagFormatter::TTagFormatter(VTApl *apl)
{
	this->apl_ = apl;
}
//---------------------------------------------------------------------------
__fastcall TTagFormatter::~TTagFormatter(void)
{

}
//---------------------------------------------------------------------------
bool __fastcall TTagFormatter::DefineCoordenadas(VTFiguraComentario *comentario, VTEqpto *eqpto_associado)
{
	bool sucesso = true;
	strUTM utm;
	strCOORD esq;

	try
	{
		if (eqpto_associado == NULL)
			{return false;}
		if (comentario == NULL)
			{return false;}
		if ((sucesso = GetCentro(eqpto_associado,utm,esq))== true)
		{
			comentario->Retangulo->SetCentro(utm,esq);
        }
	}
	catch (...)
	{
		sucesso = false;
    }

	return sucesso;
}
//---------------------------------------------------------------------------
void __fastcall TTagFormatter::FormataTagAberturaChave(VTFiguraComentario *comentario, VTEqpto *eqpto_associado)
{

}
//---------------------------------------------------------------------------
void __fastcall TTagFormatter::FormataTagFechamentoChave(VTFiguraComentario *comentario, VTEqpto *eqpto_associado)
{

}
//---------------------------------------------------------------------------
bool __fastcall TTagFormatter::GetCentro(VTEqpto *eqpto, strUTM &utm, strCOORD &esq)
{
	VTBarra *barra1, *barra2, *barra3;
	VTEqbar *eqbar = NULL;
	VTLigacao *ligacao = NULL;
	bool sucesso = true;
	strUTM utmAux;
	strCOORD esqAux;
	int nBar = 0;

	//inicializa
	barra1 = barra2 = barra3 = NULL;
	utmAux.x = utmAux.y = 0;
	esqAux.x = esqAux.y = 0;
	try
	{
		if (eqpto == NULL)
			{return false;}
		//verifica o tipo de eqpto
		if (eqpto->TipoBarra())
		{
			barra1 = (VTBarra*)eqpto;
		}
		else if (eqpto->TipoEqbar())
		{
			eqbar = (VTEqbar*)eqpto;
			barra1 = eqbar->pbarra;
		}
		else if (eqpto->TipoLigacao())
		{
			ligacao = (VTLigacao*)eqpto;
			barra1 = ligacao->pbarra1;
			barra2 = ligacao->pbarra2;
			barra3 = ligacao->pbarra3;
		}
		else
		{
            return false;
		}
		//verifica quantas barras foram setadas
		if (barra1 != NULL)
		{
			utmAux.x += barra1->utm.x;
			utmAux.y += barra1->utm.y;
			esqAux.x += (barra1->esq.x1 + barra1->esq.x2)*0.5;
			esqAux.y += (barra1->esq.y1 + barra1->esq.y2)*0.5;
			nBar++;
		}
		if (barra2 != NULL)
		{
			utmAux.x += barra2->utm.x;
			utmAux.y += barra2->utm.y;
			esqAux.x += (barra2->esq.x1 + barra2->esq.x2)*0.5;
			esqAux.y += (barra2->esq.y1 + barra2->esq.y2)*0.5;
			nBar++;
		}
        if (barra3 != NULL)
		{
			utmAux.x += barra3->utm.x;
			utmAux.y += barra3->utm.y;
			esqAux.x += (barra3->esq.x1 + barra3->esq.x2)*0.5;
			esqAux.y += (barra3->esq.y1 + barra3->esq.y2)*0.5;
			nBar++;
		}
		//tira a media
		if (nBar != 0)
		{
			utm.x = utmAux.x / nBar;
			utm.y = utmAux.y / nBar;
			esq.x = esqAux.x / nBar;
			esq.y = esqAux.y / nBar;
		}
	}
	catch (...)
	{
		sucesso = false;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TTagFormatter::PossuiTextoAutomatico(VTFiguraComentario *comentario)
{
    	TStrings *texto = NULL;
	AnsiString linha = "";

	if (comentario == NULL)
		{return false;}
	//verifica todos os texto automaticos criadas nessa classe
	texto = comentario->Texto();
	for (int ns = 0; ns < texto->Count; ns++)
	{
		linha = texto->Strings[ns];
		if (linha.Pos(tf_AUTO_TAG_TEXT)  > 0)
		{
			return true;
		}
		else
		{
			continue;
		}
	}

	return false;
}

//---------------------------------------------------------------------------
AnsiString __fastcall TTagFormatter::TextoTagAberturaChave(VTEqpto *eqpto_associado)
{
	return (tf_AUTO_TAG_TEXT);
}
//---------------------------------------------------------------------------
AnsiString __fastcall TTagFormatter::TextoTagFechamentoChave(VTEqpto *eqpto_associado)
{
    return (tf_AUTO_TAG_TEXT);
}
//---------------------------------------------------------------------------
