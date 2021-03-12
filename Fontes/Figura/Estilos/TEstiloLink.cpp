// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DLL_Inc\Funcao.h>
// #include "..\Obra\VTObra.h"
// #include "..\Obra\VTObras.h"
 #include "TEstiloLink.h"
// #include "VTFiguraComentario.h"
// #include "VTLink.h"
// #include "VTRetangulo.h"
// #include "VTFigura.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTEstilo* __fastcall NewObjEstiloLink(VTApl *apl)
{
	return (new TEstiloLink(apl));
}

// ---------------------------------------------------------------------------
__fastcall TEstiloLink::TEstiloLink(VTApl *apl)
{
	this->apl = apl;
	this->TipoEstilo = estiloLINK;
	bordaTipo = psSolid;
	bordaEspessura = 1;
	bordaCor = clWhite;
}
// ---------------------------------------------------------------------------
__fastcall TEstiloLink::~TEstiloLink(void)
{

}
// ---------------------------------------------------------------------------
bool __fastcall TEstiloLink::LeEstiloAtributos(strESTILO_ATRIBUTO *ref)
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
bool __fastcall TEstiloLink::SalvaEstiloAtributos(void)
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
