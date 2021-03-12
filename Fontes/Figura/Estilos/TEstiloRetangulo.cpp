// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DLL_Inc\Funcao.h>
// #include "..\Obra\VTObra.h"
// #include "..\Obra\VTObras.h"
 #include "VTEstilo.h"
 #include "TEstilo.h"
 #include "TEstiloRetangulo.h"
// #include "VTFiguraComentario.h"
// #include "VTLink.h"
// #include "VTRetangulo.h"
// #include "VTFigura.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTEstilo* __fastcall NewObjEstiloRetangulo(VTApl *apl)
{
	return (new TEstiloRetangulo(apl));
}

// ---------------------------------------------------------------------------
__fastcall TEstiloRetangulo::TEstiloRetangulo(VTApl *apl)
{
	this->apl = apl;
	this->TipoEstilo = estiloRETANGULO;
	bordaTipo = psSolid;
	bordaEspessura = 1;
	bordaCor = clWhite;
	fundoCor = clTeal;
	fundoTransparente = false;
}
// ---------------------------------------------------------------------------
__fastcall TEstiloRetangulo::~TEstiloRetangulo(void)
{

}
// ---------------------------------------------------------------------------
bool __fastcall TEstiloRetangulo::LeEstiloAtributos(strESTILO_ATRIBUTO *ref)
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
bool __fastcall TEstiloRetangulo::SalvaEstiloAtributos(void)
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
// ---------------------------------------------------------------------------
// eof
