// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DLL_Inc\Funcao.h>
// #include "..\Obra\VTObra.h"
// #include "..\Obra\VTObras.h"
 #include "TEstiloComentario.h"
// #include "VTFiguraComentario.h"
// #include "VTLink.h"
// #include "VTRetangulo.h"
// #include "VTFigura.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTEstilo* __fastcall NewObjEstiloComentario(VTApl *apl)
{
	return (new TEstiloComentario(apl));
}

// ---------------------------------------------------------------------------
__fastcall TEstiloComentario::TEstiloComentario(VTApl *apl)
{
	this->apl = apl;

    this->TipoEstilo = estiloCOMENTARIO;
	fontColor = clWhite;
	fontName = "Tahoma";
	fontSize = 8;
	fontBold = false;
	fontItalic = false;
	fontUnderline = false;
	fontStrikeOut = false;
	bordaTipo = psSolid;
	bordaEspessura = 1;
	bordaCor = clWhite;
	fundoCor = clTeal;
	fundoTransparente = false;
}
// ---------------------------------------------------------------------------
__fastcall TEstiloComentario::~TEstiloComentario(void)
{

}
// ---------------------------------------------------------------------------
bool __fastcall TEstiloComentario::LeEstiloAtributos(strESTILO_ATRIBUTO *ref)
{
	int valorInt = 0;
	double valorDouble = 0;
	bool sucesso = true;

	try
	{
		if (ref != NULL)
		{
			switch (ref->atributoGraficoId)
			{
				case eatBORDA_TIPO:
					this->BordaTipo = StrToInt(ref->valor);
					break;
				case eatBORDA_ESPESSURA:
					this->BordaEspessura = StrToInt(ref->valor);
					break;
				case eatBORDA_COR:
					this->Bordacor = (TColor)StrToInt(ref->valor);
					break;
				case eatFUNDO_COR:
					this->FundoCor = (TColor)StrToInt(ref->valor);
					break;
				case eatFUNDO_TRANSPARENTE:
					this->FundoTransparente = (StrToInt(ref->valor) == 1);
					break;
				case eatFONT_NAME:
					this->FontName = ref->valor;
					break;
				case eatFONT_SIZE:
					this->FontSize = StrToInt(ref->valor);
					break;
				case eatFONT_COLOR:
					this->FontColor = (TColor)StrToInt(ref->valor);
					break;
				case eatFONT_BOLD:
					this->FontBold = (StrToInt(ref->valor) == 1);
					break;
				case eatFONT_ITALIC:
					this->FontItalic = (StrToInt(ref->valor) == 1);
					break;
				case eatFONT_UNDERLINE:
					this->FontUnderline = (StrToInt(ref->valor) == 1);
					break;
				case eatFONT_STRIKEOUT:
					this->FontStrikeOut = (StrToInt(ref->valor) == 1);
					break;

			default:
				TEstilo::LeEstiloAtributos(ref);
				;
			}
		}
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		sucesso = false;
	}
	catch(...)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(funcao);
		sucesso = false;
	}

	return sucesso;
}
// ---------------------------------------------------------------------------
bool __fastcall TEstiloComentario::SalvaEstiloAtributos(void)
{
	// variaveis locais
	strESTILO_ATRIBUTO *str;
	bool sucesso = true;

	try
	{
		//apaga todos atributos
		LimpaLisAtributos();
		//salva em atributos
		//borda tipo
		str = CriaAtributo(eatBORDA_TIPO, this->BordaTipo);
		sucesso &= InsereEstiloAtributo(str);
		//borda espessura
		str = CriaAtributo(eatBORDA_ESPESSURA, this->BordaEspessura);
		sucesso &= InsereEstiloAtributo(str);
		//borda cor
		str = CriaAtributo(eatBORDA_COR, (int)this->Bordacor);
		sucesso &= InsereEstiloAtributo(str);
		//fundo cor
		str = CriaAtributo(eatFUNDO_COR, (int)this->FundoCor);
		sucesso &= InsereEstiloAtributo(str);
		//fundo transparete
		str = CriaAtributo(eatFUNDO_TRANSPARENTE, this->FundoTransparente ? 1 : 0);
		sucesso &= InsereEstiloAtributo(str);
		//font name
		str = CriaAtributo(eatFONT_NAME, this->FontName);
		sucesso &= InsereEstiloAtributo(str);
		//font size
		str = CriaAtributo(eatFONT_SIZE, this->FontSize);
		sucesso &= InsereEstiloAtributo(str);
		//font color
		str = CriaAtributo(eatFONT_COLOR, (int)this->FontColor);
		sucesso &= InsereEstiloAtributo(str);
		//font bold
		str = CriaAtributo(eatFONT_BOLD, this->FontBold ? 1 : 0);
		sucesso &= InsereEstiloAtributo(str);
		//font italic
		str = CriaAtributo(eatFONT_ITALIC, this->FontItalic ? 1 : 0);
		sucesso &= InsereEstiloAtributo(str);
		//font underline
		str = CriaAtributo(eatFONT_UNDERLINE, this->FontUnderline ? 1 : 0);
		sucesso &= InsereEstiloAtributo(str);
		//font strikeout
		//font underline
		str = CriaAtributo(eatFONT_STRIKEOUT, this->FontStrikeOut ? 1 : 0);
		sucesso &= InsereEstiloAtributo(str);
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		sucesso = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 sucesso = false;
	}

	return sucesso;
}
// ---------------------------------------------------------------------------
// eof
