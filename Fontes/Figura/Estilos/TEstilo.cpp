// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DLL_Inc\Funcao.h>
#include "..\Apl\VTApl.h"
// #include "..\Obra\VTObra.h"
// #include "..\Obra\VTObras.h"
 #include "TEstilo.h"
 #include "VTEstilos.h"
// #include "VTFiguraComentario.h"
// #include "VTLink.h"
// #include "VTRetangulo.h"
// #include "VTFigura.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
__fastcall TEstilo::TEstilo(void)
{
	id = FalsoId();
	tag = -1;
	tipo = -1;
	AnsiString descricao = "";
	lisESTILO_ATRIBUTO = new TList;
}
// ---------------------------------------------------------------------------
__fastcall TEstilo::~TEstilo(void)
{
	strESTILO_ATRIBUTO *str;

	// destrói lista e seus objetos
	if (lisESTILO_ATRIBUTO)
	{
		for (int ns = (lisESTILO_ATRIBUTO->Count - 1); ns >= 0 ; ns--)
		{
			str = (strESTILO_ATRIBUTO*) lisESTILO_ATRIBUTO->Items[ns];
			lisESTILO_ATRIBUTO->Delete(ns);
			delete str;
		}
		delete lisESTILO_ATRIBUTO;
		lisESTILO_ATRIBUTO = NULL;
	}
}
// ---------------------------------------------------------------------------
strESTILO_ATRIBUTO* __fastcall TEstilo::CriaAtributo(int estiloAtributoId, AnsiString valor)
{
	strESTILO_ATRIBUTO *str;

	if ((str = ExisteAtributo(estiloAtributoId)) == NULL)
	{
		str = new strESTILO_ATRIBUTO;
		str->salvoNaBase = false;
		str->estiloId = this->Id;
		str->atributoGraficoId = estiloAtributoId;
		str->id = FalsoId();
		str->valor = valor;
	}
	else
	{
        str->valor = valor;
	}

	return str;
}
// ---------------------------------------------------------------------------
strESTILO_ATRIBUTO* __fastcall TEstilo::ExisteAtributo(int estiloAtributoId)
{
	strESTILO_ATRIBUTO *aux = NULL;
	strESTILO_ATRIBUTO *encontrado = NULL;

	for(int na = 0; na < lisESTILO_ATRIBUTO->Count; na++)
	{
		aux = (strESTILO_ATRIBUTO*)lisESTILO_ATRIBUTO->Items[na];
		if (aux->atributoGraficoId == estiloAtributoId)
		{
			encontrado = aux;
			break;
		}
	}

	return encontrado;
}
// ---------------------------------------------------------------------------
bool __fastcall  TEstilo::InsereEstiloAtributo(strESTILO_ATRIBUTO *atributo)

{
	bool sucesso = true;

	if (atributo == NULL)
	{
		return false;
	}
	if (lisESTILO_ATRIBUTO->IndexOf(atributo) >= 0)
	{
		return false;
	}
	else
	{
        lisESTILO_ATRIBUTO->Add(atributo);
    }

	return sucesso;
}
// ---------------------------------------------------------------------------
bool __fastcall TEstilo::InsereCopiaEstiloAtributo(strESTILO_ATRIBUTO *ref)
{
	strESTILO_ATRIBUTO *pStr;
	int indiceAtributo = 0;
	bool sucesso = true;

	try
	{
        // guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;
		//pprotecao
		if (ref ==  NULL)
		{
			return false;
		}
		//cria um novo
		pStr = new strESTILO_ATRIBUTO;
		//copia os atributos da referencia
		pStr->salvoNaBase = false;
		pStr->id = FalsoId();
		pStr->estiloId = this->id;
		pStr->atributoGraficoId = ref->atributoGraficoId;
		pStr->valor = ref->valor;
		//adiciona
		lisESTILO_ATRIBUTO->Add(pStr);

	}
    catch(Exception &e)
	{
		//Loga(mErrorFunction + " " + e.Message);
		sucesso = false;
	}
	catch(...)
	{
		//Loga(mErrorFunction);
		sucesso = false;
	}
	return sucesso;

}
// ---------------------------------------------------------------------------
bool __fastcall TEstilo::LeEstiloAtributos(void)
{
	strESTILO_ATRIBUTO *atributo = NULL;
	bool sucesso = true;

	try
	{
		//le a lista de ipe atributos e atualiza seus atributos
		for (int na = 0; na < lisESTILO_ATRIBUTO->Count; na++)
		{
			atributo = (strESTILO_ATRIBUTO*)lisESTILO_ATRIBUTO->Items[na];
			LeEstiloAtributos(atributo);
		}
	}
	catch(...)
	{
		sucesso = false;
	}
	return sucesso;
}
// ---------------------------------------------------------------------------
bool __fastcall TEstilo::LeEstiloAtributos(strESTILO_ATRIBUTO *ref)
{
	//classe derivada deve implementar
    return false;
}
// ---------------------------------------------------------------------------
void __fastcall TEstilo::LimpaLisAtributos(void)
{
    strESTILO_ATRIBUTO *atributo = NULL;

	//le a lista de ipe atributos e atualiza seus atributos
	for (int na = (lisESTILO_ATRIBUTO->Count - 1); na >= 0; na--)
	{
		atributo = (strESTILO_ATRIBUTO*)lisESTILO_ATRIBUTO->Items[na];
		delete atributo;
        lisESTILO_ATRIBUTO->Delete(na);
	}
}
// ---------------------------------------------------------------------------
bool __fastcall TEstilo::SalvaEstiloAtributos(void)
{
	//deve ser definido nas classes derivadas
	return false;
}
// ---------------------------------------------------------------------------
// eof
