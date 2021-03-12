//---------------------------------------------------------------------------

#pragma hdrstop
//---------------------------------------------------------------------------
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Figura\VTFiguras.h>
#include <PlataformaSinap\Fontes\Figura\VTFiguraComentario.h>
#include <PlataformaSinap\Fontes\Figura\VTRetangulo.h>
#include <PlataformaSinap\Fontes\Figura\VTLink.h>
#include <PlataformaSinap\Fontes\Figura\Estilos\VTEstilo.h>
#include <PlataformaSinap\Fontes\Figura\Estilos\VTEstiloComentario.h>
#include <PlataformaSinap\Fontes\Figura\Estilos\VTEstilos.h>
#include <PlataformaSinap\Fontes\Obra\VTAcao.h>
#include <PlataformaSinap\Fontes\Rede\VTBarra.h>
#include <PlataformaSinap\Fontes\Rede\VTEqbar.h>
#include <PlataformaSinap\Fontes\Rede\VTLigacao.h>
//---------------------------------------------------------------------------
#include "TTagFormatterCOPEL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTTagFormatter* __fastcall NewObjTagFormatterCOPEL(VTApl *apl)
{
	try
	{
		return (new TTagFormatterCOPEL(apl));
	}catch (Exception &e)
	{
		return(NULL);
	}
}
//---------------------------------------------------------------------------
__fastcall TTagFormatterCOPEL::TTagFormatterCOPEL(VTApl *apl): TTagFormatter(apl)
{

}
//---------------------------------------------------------------------------
__fastcall TTagFormatterCOPEL::~TTagFormatterCOPEL(void)
{

}
//---------------------------------------------------------------------------
void __fastcall TTagFormatterCOPEL::FormataEstiloDefaultAbrir(VTEstiloComentario *estiloC)
{
    try
	{
		//define o estilo do comentario
		estiloC->FontColor = clGreen;
		estiloC->FontName = "Tahoma";
		estiloC->FontSize = 9;
		estiloC->FontBold = true;
		estiloC->FontItalic = false;
		estiloC->FontUnderline = false;
		estiloC->FontStrikeOut = false;
		//retangulo
		estiloC->Bordacor = clGreen;
		estiloC->FundoTransparente = false;
		estiloC->BordaEspessura = 1;
		estiloC->BordaTipo = psSolid;
		estiloC->FundoCor =  clBlack;
	}
	catch (...)
	{

	}
}
//---------------------------------------------------------------------------
void __fastcall TTagFormatterCOPEL::FormataEstiloDefaultFechar(VTEstiloComentario *estiloC)
{
	try
	{
		//define o estilo do comentario
		estiloC->FontColor = clRed;
		estiloC->FontName = "Tahoma";
		estiloC->FontSize = 9;
		estiloC->FontBold = true;
		estiloC->FontItalic = false;
		estiloC->FontUnderline = false;
		estiloC->FontStrikeOut = false;
		//retangulo
		estiloC->Bordacor = clRed;
		estiloC->FundoTransparente = false;
		estiloC->BordaEspessura = 1;
		estiloC->BordaTipo = psSolid;
		estiloC->FundoCor =  clBlack;
	}
	catch (...)
	{

	}
}
//---------------------------------------------------------------------------
void __fastcall TTagFormatterCOPEL::FormataTagAberturaChave(VTFiguraComentario *comentario, VTEqpto *eqpto_associado)
{
	VTEstilos *estilos = (VTEstilos*)apl_->GetObject(__classid(VTEstilos));
	VTEstiloComentario *estiloC = NULL;
	VTEstilo *estilo = NULL;
	VTLink *link = NULL;
	AnsiString descricao = "Estilo Comentario Manobra : ABRIR";
//	AnsiString descricao = "";

	try
	{
		//verifica se existe comentario
		estilo = estilos->ExisteEstilo(descricao);
		if (estilo == NULL)
		{ 	//cria estilo
			estiloC = (VTEstiloComentario*)  estilos->InsereEstilo(comentario, descricao);
			FormataEstiloDefaultAbrir(estiloC);
		}
		else
			{estiloC = (VTEstiloComentario*) estilo;}
		//define o texto
		comentario->RetiraTexto(TextoTagFechamentoChave(eqpto_associado));
		comentario->InsereTexto(TextoTagAberturaChave(eqpto_associado));
		comentario->Estilo = estiloC;
		//aplica estilo
		comentario->AplicaEstilo(estiloC);
		//insere link
		link = comentario->InsereLink(eqpto_associado);
		//define a posicao
		DefineCoordenadas(comentario,eqpto_associado);
		//autosize
		comentario->AutoSize();
	}
	catch (...)
	{

	}
}
//---------------------------------------------------------------------------
void __fastcall TTagFormatterCOPEL::FormataTagFechamentoChave(VTFiguraComentario *comentario, VTEqpto *eqpto_associado)
{
    VTEstilos *estilos = (VTEstilos*)apl_->GetObject(__classid(VTEstilos));
	VTEstiloComentario *estiloC = NULL;
	VTEstilo *estilo = NULL;
	VTLink *link = NULL;
	AnsiString descricao = "Estilo Comentario Manobra : FECHAR";
//    AnsiString descricao = "";

	try
	{
		//verifica se existe comentario
		estilo = estilos->ExisteEstilo(descricao);
		if (estilo == NULL)
		{ 	//cria estilo
			estiloC = (VTEstiloComentario*)  estilos->InsereEstilo(comentario, descricao);
			FormataEstiloDefaultFechar(estiloC);
		}
		else
			{estiloC = (VTEstiloComentario*) estilo;}
		//define o texto
		comentario->RetiraTexto(TextoTagAberturaChave(eqpto_associado));
		comentario->InsereTexto(TextoTagFechamentoChave(eqpto_associado));
		comentario->Estilo = estiloC;
		//aplica estilo
		comentario->AplicaEstilo(estiloC);
		//insere link
		link = comentario->InsereLink(eqpto_associado);
		//define a posicao
		DefineCoordenadas(comentario,eqpto_associado);
		//autosize
		comentario->AutoSize();
	}
	catch (...)
	{

	}
}
////---------------------------------------------------------------------------
//bool __fastcall TTagFormatterCOPEL::PossuiTextoAutomatico(VTFiguraComentario *comentario)
//{
//	TStrings *texto = NULL;
//	AnsiString linha = "";
//
//	if (comentario == NULL)
//		{return false;}
//	//verifica todos os texto automaticos criadas nessa classe
//	texto = comentario->Texto();
//	for (int ns = 0; ns < texto->Count; ns++)
//	{
//		linha = texto->Strings[ns];
//		if (linha.Pos(tf_AUTO_TAG_TEXT)  > 0)
//		{
//			return true;
//		}
////		else if (linha.Pos(tf_COPEL_ABRE_CHAVE) > 0)
////		{
////			return true;
////		}
////		else if (linha.Pos(tf_COPEL_FECHA_CHAVE) > 0)
////		{
////			return true;
//		}
//		else
//		{
//			continue;
//		}
//	}
//
//	return false;
//}
//---------------------------------------------------------------------------
AnsiString __fastcall TTagFormatterCOPEL::TextoTagAberturaChave(VTEqpto *eqpto_associado)
{
	AnsiString txt;

	txt = TTagFormatter::TextoTagAberturaChave(eqpto_associado);
	txt += tf_COPEL_ABRE_CHAVE + eqpto_associado->Codigo;
	return (txt);
}
//---------------------------------------------------------------------------
AnsiString __fastcall TTagFormatterCOPEL::TextoTagFechamentoChave(VTEqpto *eqpto_associado)
{
	AnsiString txt;

	txt = TTagFormatter::TextoTagFechamentoChave(eqpto_associado);
	txt += tf_COPEL_FECHA_CHAVE + eqpto_associado->Codigo;
	return (txt);
}
//---------------------------------------------------------------------------
