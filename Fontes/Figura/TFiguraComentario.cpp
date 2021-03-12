//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
#include <vcl.h>
#pragma hdrstop
#include <Vcl.Graphics.hpp>
#include <DLL_Inc\Funcao.h>
#include <Fontes\Rede\VTEqpto.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Grafico\VTGrafico.h>
#include <Fontes\Figura\Estilos\VTEstilos.h>
#include <Fontes\Figura\Estilos\VTEstiloComentario.h>
#include "TFiguraComentario.h"
#include "VTLink.h"
#include "VTRetangulo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTFiguraComentario* __fastcall NewObjFiguraComentario(VTApl *apl_owner)
   {
   return(new TFiguraComentario(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TFiguraComentario::TFiguraComentario(VTApl *apl_owner)
   {
   //salva ponteiro para objetos
   this->apl = apl_owner;
   //salva o ponteiro para o estilos
   styles = (VTEstilos*)apl_owner->GetObject(__classid(VTEstilos));
   //cria Retangulo e listas para Links
   Font      = new TFont();
   Retangulo = NewObjRetangulo(apl);
   lisLINK   = new TList();
   texto     = new TStringList();
   //define atributos
   Id                        	 = -1;
   Tag                       	 =  0;
   Visible                   	 = true;
   Retangulo->Status[sttEV_NOVO] = true;
   Retangulo->BordaEspessura 	 = 1;
   Retangulo->BordaEstilo  	     = psSolid;
   Retangulo->BordaCor       	 = clWhite;
   Retangulo->FundoCor       	 = clTeal;
   Font->Color               	 = clWhite;
   Font->Name                	 = "Tahoma";
   Font->Charset             	 = DEFAULT_CHARSET;
   Font->Size                	 = 8;
   Font->Style               	 = TFontStyles(0);
   }

//---------------------------------------------------------------------------
__fastcall TFiguraComentario::~TFiguraComentario(void)
   {
   //destrói objetos
   if (texto)   {delete texto;    texto   = NULL;}
   if (Font)    {delete PD.font;  Font    = NULL;}
   if (Retangulo) {delete Retangulo;  Retangulo = NULL;}
   //destrói lista e seus objetos
   if (lisLINK) {LimpaTList(lisLINK); delete lisLINK; lisLINK = NULL;}
   }
//---------------------------------------------------------------------------
bool __fastcall TFiguraComentario::AplicaEstilo(VTEstilo *estilo)
{
	VTEstiloComentario *estiloC = NULL;
	bool sucesso = true;

	try
	{
		if ((estilo != NULL) && (estilo->TipoEstilo == estiloCOMENTARIO))
		{
			estiloC = (VTEstiloComentario*) estilo;
		}
		else
		{   //verifica se a figura já possui um comentario
			if ((this->Estilo != NULL) && (this->Estilo->TipoEstilo == estiloCOMENTARIO))
			{
				estiloC = (VTEstiloComentario*) this->Estilo;
			}
		}
		if (estiloC != NULL)
		{
			Retangulo->BordaEspessura 	 = estiloC->BordaEspessura;
			Retangulo->BordaEstilo  	 = estiloC->BordaTipo;
			Retangulo->BordaCor       	 = estiloC->Bordacor;
			Retangulo->FundoCor       	 = estiloC->FundoCor;
			Retangulo->Transparente      = estiloC->FundoTransparente;
			Font->Color               	 = estiloC->FontColor;
			Font->Name                	 = estiloC->FontName;
			Font->Charset             	 = DEFAULT_CHARSET;
			Font->Size                	 = estiloC->FontSize;
			if (estiloC->FontItalic)
				{this->PD.font->Style = this->PD.font->Style << fsItalic;}
			if (estiloC->FontBold)
				{this->PD.font->Style = this->PD.font->Style << fsBold;}
            if (estiloC->FontUnderline)
				{this->PD.font->Style = this->PD.font->Style << fsUnderline;}
			if (estiloC->FontStrikeOut)
				{this->PD.font->Style = this->PD.font->Style << fsStrikeOut;}
		}
		else
		{
			sucesso = false;
        }
	}
	catch (...)
	{
		sucesso = true;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
void __fastcall  TFiguraComentario::AutoSize(void)
{
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TCanvas *canvas = NULL;
	AnsiString txt;
	int dx, dy, width,height;
	TFont *fontOriginal;

	if ((grafico != NULL) && (grafico->Canvas != NULL))
	{
		dx = dy = 0;
		canvas = grafico->Canvas;
		fontOriginal = new TFont;
		fontOriginal->Assign(canvas->Font);
		canvas->Font->Assign(this->Font);
		canvas->Font->Assign(this->Font);
		for (int n = 0; n < this->texto->Count; n++)
		{
			txt    = this->texto->Strings[n];
			width  = abs(canvas->TextWidth(txt))+1;
			height = abs(canvas->TextHeight(txt)) * this->texto->Count;
			//grafico->SizeOfString(txt, width, height);
			if (height > dy) dy = height;
			if (width  > dx) dx = width;
		}
		canvas->Font->Assign(fontOriginal);
		delete fontOriginal;
		this->Retangulo->SetDimensao(dx + 12, dy + 6);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFiguraComentario::ClearTexto(void)
   {
   texto->Clear();
   }

//---------------------------------------------------------------------------
VTLink* __fastcall TFiguraComentario::ExisteLink(VTEqpto *eqpto)
{
	VTLink *linkAux, *link;

	link = NULL;

	for (int nl = (this->lisLINK->Count - 1); nl >= 0; nl--)
	{
		linkAux = (VTLink*) lisLINK->Items[nl];
		if (linkAux->Eqpto == eqpto)
		{
			link = linkAux;
			break;
		}
	}

	return link;
}
//---------------------------------------------------------------------------
VTLink* __fastcall TFiguraComentario::InsereLink(bool create_estilo)
   {
   //variáveis locais
   VTLink *link;

   if ((link = NewObjLink(apl)) != NULL)
	  {
	  link->Status[sttEV_NOVO] = true;
	  link->Id = FalsoId();
      link->Color = Retangulo->BordaCor;
	  InsereLink(link);
	  //hoje em dia cada figura vai ter 1 estilo próprio, mas a ideia é que diversas figuras tenham o mesmo estilo

	  if ((styles) && (create_estilo))
		{

		link->Estilo = styles->InsereEstilo(link, "");
		}
      }
   return(link);
   }
//---------------------------------------------------------------------------
VTLink* __fastcall TFiguraComentario::InsereLink(VTEqpto *eqpto)
{
	//variáveis locais
	VTLink *link = NULL;

	if (eqpto != NULL)
	{
		link = InsereLink(true);
		link->Eqpto = eqpto;
	}

    return link;
}
//---------------------------------------------------------------------------
bool __fastcall TFiguraComentario::InsereLink(VTLink *link)
   {
   //proteção
   if (link == NULL) return(false);
   //insere Comentario na lista
   if (lisLINK->IndexOf(link) < 0)
      {
      lisLINK->Add(link);
	  }
   //associa o Link com este Comentario
   link->Comentario = this;
   //define ponto de conexão com Eqpto
   //if      (link->Eqpto->TipoBarra())   link->PosEqp = posEQP_XmYm;
   //else if (link->Eqpto->TipoEqbar())   link->PosEqp = posEQP_X1Y1;
   //else if (link->Eqpto->TipoLigacao()) link->PosEqp = posEQP_XmYm;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFiguraComentario::InsereTexto(AnsiString txt)
   {
   texto->Add(txt);
   this->Status[sttEV_ALTERADO] = true;
   this->AutoSize();
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFiguraComentario::InsereTexto(TStrings *lines)
   {
   texto->Clear();
   texto->AddStrings(lines);
   this->Status[sttEV_ALTERADO] = true;
   this->AutoSize();
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TFiguraComentario::LisLink(void)
   {
   return(lisLINK);
   }

//---------------------------------------------------------------------------
void __fastcall TFiguraComentario::Move_esq(int dx, int dy)
   {
   Retangulo->Move_esq(dx, dy);
   this->Status[sttEV_ALTERADO] = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFiguraComentario::Move_utm(int dx, int dy)
   {
   Retangulo->Move_utm(dx, dy);
   this->Status[sttEV_ALTERADO] = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFiguraComentario::PM_SetFormato(int formato)
   {
   //salva o novo estilo
   PD.formato = formato;
   //redefine dimensões do Retangulo
   if (Formato == fmtFLAG)
      {//salva dimensões do Retangulo
      Retangulo->GetDimensao(dim.dx, dim.dy);
      //redefine dimensões do Retangulo
      Retangulo->SetDimensao(DIM_FLAG, DIM_FLAG);
      }
   else if (( dim.dx > 0) || (dim.dy > 0))
      {//restaura dimensões do retângulo
      Retangulo->SetDimensao(dim.dx, dim.dy);
	  }
   Retangulo->Status[sttEV_ALTERADO] = true;
   }

//---------------------------------------------------------------------------
bool __fastcall TFiguraComentario::RetiraLink(VTLink *link)
   {
   //proteção
   if (link == NULL) return(false);
   //D.V.C. 13/08/2019
   // Troca o status da Fig para excluido
   link->Status[sttEV_EXCLUIDO] = true;
   //retira Link na lista e destrói objeto
//   lisLINK->Remove(link);
//   delete link;

   return(true);
   }
//---------------------------------------------------------------------------
bool  __fastcall  TFiguraComentario::RetiraTexto(AnsiString txt)
{
	AnsiString textoAux = "";
	bool encontrou = false;

	for (int nl = (texto->Count - 1); nl >= 0; nl--)
	{
		if (txt.AnsiCompare(texto->Strings[nl]) == 0)
		{
			encontrou = true;
			texto->Delete(nl);
			break;
		}
	}

	return encontrou;
}
//---------------------------------------------------------------------------
void __fastcall TFiguraComentario::SalvaInfoNoEstilo(void)
{
	VTEstiloComentario *est;
	VTLink *link;

	if (Estilo != NULL)
	{
		est = (VTEstiloComentario*)Estilo;
        int a = Estilo->TipoEstilo ;
		est->BordaTipo = this->Retangulo->BordaEstilo;
		est->BordaEspessura = this->Retangulo->BordaEspessura;
		est->Bordacor = this->Retangulo->BordaCor;
		est->FundoCor = this->Retangulo->FundoCor;
		est->FundoTransparente = this->Retangulo->Transparente;
		est->FontSize = this->Font->Size;
		est->FontName = this->Font->Name;
		est->FontColor = this->Font->Color;
		est->FontBold = this->Font->Style.Contains(fsBold);
		est->FontItalic = this->Font->Style.Contains(fsItalic);
		est->FontUnderline = this->Font->Style.Contains(fsUnderline);
		est->FontStrikeOut = this->Font->Style.Contains(fsStrikeOut);
		//percorre todos links
		for (int nl = 0; nl < lisLINK->Count; nl++)
		{
			link = (VTLink*)lisLINK->Items[nl];
            link->SalvaInfoNoEstilo();
		}
	}
}
//---------------------------------------------------------------------------
TStrings* __fastcall TFiguraComentario::Texto(void)
   {
   return(texto);
   }

//---------------------------------------------------------------------------
//eof
