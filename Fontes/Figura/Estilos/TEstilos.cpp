// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <System.Hash.hpp>
// ---------------------------------------------------------------------------
// #include "..\Obra\VTObra.h"
// #include "..\Obra\VTObras.h"
#include "..\Apl\VTApl.h"
#include "..\BD\VTBD.h"
#include "..\BD\Figura\VTBDfigura.h"
#include "..\Diretorio\VTPath.h"
#include "..\Figura\VTFiguras.h"
#include "..\Obra\VTObra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTRede.h"
// ---------------------------------------------------------------------------
#include "TEstilos.h"
#include "VTEstilo.h"
#include "VTEstiloComentario.h"
#include "VTEstiloLink.h"
#include "VTEstiloRetangulo.h"
#include "VTFiguraComentario.h"
#include "VTLink.h"
#include "VTRetangulo.h"
#include "VTFigura.h"
// ---------------------------------------------------------------------------
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\BD.h>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
VTEstilos* __fastcall NewObjEstilos(VTApl *apl)
{
	return (new TEstilos(apl));
}

// ---------------------------------------------------------------------------
__fastcall TEstilos::TEstilos(VTApl *apl)
{
	// salva ponteiros para objetos
	this->apl = apl;

	//associa estilos à figuras existentes
//	AssociaEstilos();
	// cria listas
	PD.lisESTILO = new TList();
	PD.lisAtributoGrafico = new TList;
	PD.lisFormaAtributo = new TList();
//	PD.lisValorFormaAtributo = new TList();
	PD.lisForma  = new TList();
	PD.lisTipoValorAtributoGrafico  = new TList();
}

// ---------------------------------------------------------------------------
__fastcall TEstilos::~TEstilos(void)
{
	// destrói lista e seus objetos
	if (PD.lisESTILO)
	{
		LimpaTList(PD.lisESTILO);
		delete PD.lisESTILO;
		PD.lisESTILO = NULL;
	}
	//destroi os objetos auxiliares
    DestroiAuxiliares();
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TEstilos::AssociaEstilos(void)
{
	// variáveis locais
	VTFiguras *figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
	TList *lisCOM, *lisLINK;
	TStrings *texto;
	TList *lisRET;
	VTRetangulo *retangulo;
	VTFiguraComentario *comentario;
	VTLink *link;
	bool sucesso = true;

	try
	{ // guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;
		//garante que os auxiliares estão presentes
        PreencheAuxiliares();
		//cria um estilo para cada comentario, link, retangulo
		//(e texto(?) hoje o texto herda o estilo do comentario)
		// loop para todos Comentarios
		lisCOM = figuras->LisComentario();
		for (int i = 0; i < lisCOM->Count; i++)
		{
			comentario = (VTFiguraComentario*)lisCOM->Items[i];
			//cria/atualiza id do estilo na figura
			AssociaEstilo(comentario);
			// insere linhas com texto do Comentario
			// num 1o momento, os textos vão herdar o estilo do comentario
			// no futuro pode ser q cada um tenha seu estilo
			// insere linhas com Links do Comentario
			lisLINK = comentario->LisLink();
			 for (int j = 0; j < lisLINK->Count; j++)
			 {
				 link = (VTLink*)lisLINK->Items[j];
				 //cria/atualiza id do estilo na figura
				 AssociaEstilo(link);
			 }
		}
		// loop para todos retangulos
		lisRET = figuras->LisRetangulo();
		for (int i = 0; i < lisRET->Count; i++)
		{
			retangulo = (VTRetangulo*)lisRET->Items[i];
			//cria/atualiza id do estilo na figura
			AssociaEstilo(retangulo);
		}
	}
	catch(Exception &e)
	{
//		Loga(mErrorFunction + " " + e.Message);
	}
	catch(...)
	{
//		Loga(mErrorFunction);
	}
}
// ---------------------------------------------------------------------------
int  __fastcall TEstilos::AssociaEstilo(VTFigura *figura)
{
	AnsiString hash = "";
	int estiloId = 0;
	VTEstilo *estilo = NULL;
	VTEstilo *estiloNovo = NULL;

	try
	{
		// guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;
		//protecoes
		if (figura == NULL)
			{return 0;}
		//verifica se deve criar um novo
		if (figura->Estilo != NULL)
		{
			estilo = figura->Estilo;
		}
		else
		{   //
			hash = CriaHash(figura);
			//verifica se ja foi carregado ou se tem q criar
			estilo = MapaEstilo(hash);
		}
		//cria
		if (estilo == NULL)
			{//se criou um hash e o não encontrou o estilo no map, cria um novo
			if (!hash.IsEmpty())
			{   //cria
				estiloNovo = InsereEstilo(figura, "");
				estiloId = estiloNovo->Id;
				figura->Estilo = estiloNovo;
				estiloNovo->Descricao = hash;
				estilo = estiloNovo;
			}
		}
		//salva info
		if (estilo != NULL)
		{
            estilo->SalvaEstiloAtributos();
		}

	}
	catch(Exception &e)
	{
//		Loga(mErrorFunction + " " + e.Message);
		estiloId = 0;
	}
	catch(...)
	{
//		Loga(mErrorFunction);
		estiloId = 0;
	}
	return estiloId;
}
//// ---------------------------------------------------------------------------
//int  __fastcall TEstilos::AssociaEstilo(VTFigura *figura)
//{
//	AnsiString hash = "";
//	int estiloId = FalsoId();
//	VTEstilo *estilo = NULL;
//
//	try
//	{
//		// guarda o nome da funcao
//		mErrorFunction = __PRETTY_FUNCTION__;
//		if (figura == NULL)
//		{
//			return estiloId;
//		}
//		//verifica se a figura já tem estilo
//		if (figura->Estilo != NULL)
//		{
//			try
//			{
//				hash = figura->Estilo->Descricao;
//				estilo = MapaEstilo(hash);
//				if (estilo == figura->Estilo)
//				{
//					return estilo->Id;
//				}
//				else
//				{
//					figura->Estilo = estilo;
//                    return estilo->Id;
//				}
//			}
//			catch(...)
//			{
//                figura->Estilo = NULL;
//            }
//		}
//		else
//		{
//			hash = CriaHash(figura);
//			//verifica se ja foi carregado ou se tem q criar
//	//		estiloId = MapaEstiloId(hash);
//			estilo = MapaEstilo(hash);
//			//se criou um hash e o não encontrou o estilo no map, cria um novo
//	//		if ((!hash.IsEmpty()) && (estiloId < 0))
//			if ((!hash.IsEmpty()) && (estilo == NULL))
//			{
//				estilo = InsereEstilo(figura, "");
//				estiloId = estilo->Id;
//				figura->Estilo = estilo;
//				estilo->Descricao = hash;
//			}
//			else if (estilo != NULL)
//			{
//				figura->Estilo= estilo;
//				estiloId = estilo->Id;
//			}
//        }
//
//		if (estilo != NULL)
//		{
//            estilo->SalvaEstiloAtributos();
//		}
//	}
//	catch(Exception &e)
//	{
////		Loga(mErrorFunction + " " + e.Message);
//		estiloId = -1;
//		if (estilo)
//		{
//			delete estilo;
//			estilo = NULL;
//		}
//
//	}
//	catch(...)
//	{
////		Loga(mErrorFunction);
//		estiloId = -1;
//		if (estilo)
//		{
//			delete estilo;
//			estilo = NULL;
//		}
//    }
//
//	return estiloId;
//}
// ---------------------------------------------------------------------------
int __fastcall TEstilos::AtributoGraficoId(AnsiString descricao)
{
	strATRIBUTO_GRAFICO *str = NULL;
	map<AnsiString,strATRIBUTO_GRAFICO*>::iterator it;
	int id = FalsoId();

	it = mapaAtributoGrafico.find(descricao);
	if (it!= mapaAtributoGrafico.end())
	{
		str = it->second;
        id = str->id;
	}

	return id;
}
// ---------------------------------------------------------------------------
void __fastcall TEstilos::AtualizaEstilos(void)
{
    // variáveis locais
	VTFiguras *figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
	VTFiguraComentario *coment =  NULL;
	VTRetangulo *retangulo = NULL;


	for (int nf = 0; nf < figuras->LisComentario()->Count ; nf++)
	{
		coment = (VTFiguraComentario*)figuras->LisComentario()->Items[nf];
		coment->SalvaInfoNoEstilo();
	}
	for (int nr = 0; nr < figuras->LisRetangulo()->Count ; nr++)
	{
		retangulo = (VTRetangulo*)figuras->LisRetangulo()->Items[nr];
		retangulo->SalvaInfoNoEstilo();
	}
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
AnsiString  __fastcall TEstilos::CriaHash(VTFigura *figura)
{
	AnsiString hash = "";
	AnsiString original = "";
	int obra_id, rede_id, alternativa_id, tipoId;
	AnsiString separador = ";";

	if (figura == NULL)
	{
		return hash;
	}
	//inicia variaveis
	obra_id = rede_id = alternativa_id = tipoId = 0;
	//se possui obra
	if (figura->Obra != NULL)
		{obra_id = figura->Obra->Id;}
	//salva tipo
	tipoId = figura->TipoFigura;
	//se é comentario e possui rede
	if (figura->TipoFigura  == figCOMENTARIO)
	{
		if(((VTFiguraComentario*)figura)->Rede != NULL)
		{
			rede_id = ((VTFiguraComentario*)figura)->Rede->Id;
        }
	}
	//se possui alternativa    /* TODO : achar alternativa da figura */
//	if (alternativa != NULL)
//		{alternativa_id = alternativa->Id;}
	//cria a string original
	original = IntToStr(alternativa_id) + separador +
			   IntToStr(obra_id) + separador +
			   IntToStr(rede_id) +  separador +
			   IntToStr(figura->Id) + separador +
			   IntToStr(tipoId)+ separador;
	//adiciona info do estilo proprio
	switch (figura->TipoFigura)
	{
		case figCOMENTARIO:
			original += CriaString((VTFiguraComentario*)figura);
			break;
		case figLINK:
			original += CriaString((VTLink*)figura);
			break;
		case figRETANGULO:
			original += CriaString((VTRetangulo*)figura);
			break;
		default:
			;
	}
	hash = THashMD5::GetHashString(original);

	return hash;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
AnsiString  __fastcall TEstilos::CriaString(VTFiguraComentario *comentario)
{
	AnsiString frase = "";
	AnsiString separador = ";";
	//cria uma string com as configs de estilo do retangulo

    frase += CriaString(comentario->Font);
	frase += CriaString(comentario->Retangulo);

	return frase;
}
// ---------------------------------------------------------------------------
AnsiString  __fastcall TEstilos::CriaString(VTRetangulo *retangulo)
{
	AnsiString frase = "";
	AnsiString separador = ";";
	//cria uma string com as configs de estilo do retangulo

	frase += IntToStr(retangulo->BordaEstilo) + separador;
	frase += IntToStr(retangulo->BordaEspessura) + separador;
	frase += IntToStr(int(retangulo->BordaCor)) + separador;
	frase += IntToStr(int(retangulo->FundoCor)) + separador;
	frase += IntToStr(retangulo->Transparente ? 1 : 0) + separador;

	return frase;
}
// ---------------------------------------------------------------------------
AnsiString  __fastcall TEstilos::CriaString(VTLink *link)
{
   AnsiString frase = "";
	AnsiString separador = ";";
	//cria uma string com as configs de estilo do link

	frase += IntToStr(link->BordaTipo) + separador;
	frase += IntToStr(link->Espessura) + separador;
	frase += IntToStr(int(link->Color)) + separador;

	return frase;
}
// ---------------------------------------------------------------------------
AnsiString  __fastcall TEstilos::CriaString(TFont *font)
{
	AnsiString frase = "";
	AnsiString separador = ";";
	//cria uma string com as configs de estilo do link

	frase += font->Name + separador;
	frase += IntToStr(font->Size) + separador;
	frase += IntToStr(font->Style.Contains(fsBold) ? 1 : 0) + separador;
	frase += IntToStr(font->Style.Contains(fsItalic) ? 1 : 0) + separador;
	frase += IntToStr(font->Style.Contains(fsUnderline) ? 1 : 0) + separador;
	frase += IntToStr(font->Style.Contains(fsStrikeOut) ? 1 : 0) + separador;

	return frase;
}
// ---------------------------------------------------------------------------
// bool __fastcall TEstilos::ExisteAlteracao(void)
// {
//
// }
// ---------------------------------------------------------------------------
void __fastcall TEstilos::DestroiAuxiliares(void)
{
	if (PD.lisAtributoGrafico != NULL)
	{
		DestroiAuxiliares(PD.lisAtributoGrafico);
		delete PD.lisAtributoGrafico;
		PD.lisAtributoGrafico = NULL;
	}
	if (PD.lisFormaAtributo != NULL)
	{
		DestroiAuxiliares(PD.lisFormaAtributo);
		delete PD.lisFormaAtributo;
		PD.lisFormaAtributo = NULL;
	}
//	if (lisValorFormaAtributo != NULL)
//	{
//		DestroiAuxiliares(lisValorFormaAtributo);
//		delete lisValorFormaAtributo;
//		lisValorFormaAtributo = NULL;
//	}
	if (PD.lisForma != NULL)
	{
		DestroiAuxiliares(PD.lisForma);
		delete PD.lisForma;
		PD.lisForma = NULL;
	}
	if (PD.lisTipoValorAtributoGrafico != NULL)
	{
		DestroiAuxiliares(PD.lisTipoValorAtributoGrafico);
		delete PD.lisTipoValorAtributoGrafico;
		PD.lisTipoValorAtributoGrafico = NULL;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TEstilos::DestroiAuxiliares(TList *list)
{
	strAUXILIAR_FIGURA *str;

	for (int na = (list->Count - 1); na >= 0; na--)
	{
		str = (strAUXILIAR_FIGURA*) list->Items[na];
		list->Delete(na);
		delete str;
	}
}
// ---------------------------------------------------------------------------
strESTILO_ATRIBUTO* __fastcall TEstilos::EstiloAtributo(int estiloAtributoId, strESTILO *str)
{
	strESTILO_ATRIBUTO *aux, *encontrado;

	aux = encontrado = NULL;

	try
	{
		for (int ns = 0; ns < str->atributos.Length; ns++)
		{
			aux = &str->atributos[ns];
			if (aux->atributoGraficoId == estiloAtributoId)
			{
				encontrado = aux;
				break;
			}
		}
	}
	catch(...)
	{

	}

	return encontrado;
}
// ---------------------------------------------------------------------------
VTEstilo*   __fastcall TEstilos::ExisteEstilo(int estiloId)
{
	VTEstilo *encontrado, *aux;

	encontrado = NULL;
	for (int ne = 0; ne < PD.lisESTILO->Count; ne++)
	{
		aux = (VTEstilo*) PD.lisESTILO->Items[ne];
		if (aux->Id == estiloId)
		{
			encontrado = aux;
			break;
		}
	}

    return encontrado;
}
// ---------------------------------------------------------------------------
VTEstilo*   __fastcall TEstilos::ExisteEstilo(AnsiString descricao)
{
	VTEstilo *estilo = NULL;

	estilo = MapaEstilo(descricao);

    return estilo;
}
// ---------------------------------------------------------------------------
int __fastcall TEstilos::FormaAtributoId(AnsiString nome)
{
	strFORMA_ATRIBUTO *str = NULL;
	map<AnsiString,strFORMA_ATRIBUTO*>::iterator it;
	int id = FalsoId();

	it = mapaFormaAtributo.find(nome);
	if (it!= mapaFormaAtributo.end())
	{
		str = it->second;
        id = str->id;
	}

	return id;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
int __fastcall TEstilos::IdEstiloFiguraId(int figuraId)
{
	VTFiguras *figuras = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
	VTFiguraComentario *comentario;
	int idEstilo = -1;

	if (figuras != NULL)
	{
		comentario = figuras->ExisteComentario(figuraId);
		if ((comentario != NULL) && (comentario->Estilo != NULL))
		{
			idEstilo = comentario->Estilo->Id;
		}
	}

	return idEstilo;
}
// ---------------------------------------------------------------------------
strATRIBUTO_GRAFICO* __fastcall TEstilos::InsereAtributoGrafico(AnsiString descricao)
{
	strATRIBUTO_GRAFICO * str= NULL;
	std::map<AnsiString,strATRIBUTO_GRAFICO*>::iterator it;

	it = mapaAtributoGrafico.find(descricao);
	if (it == mapaAtributoGrafico.end())
	{
        // loop registros (ini)
		str = new strATRIBUTO_GRAFICO;
		str->salvoNaBase = false;
		str->descricao = descricao;
		//insere no map
		mapaAtributoGrafico.insert(pair<AnsiString,strATRIBUTO_GRAFICO*>(str->descricao,str));
		PD.lisAtributoGrafico->Add(str);
	}
	else
	{
        str = it->second;
    }


	return str;
}
// ---------------------------------------------------------------------------
VTEstilo* __fastcall TEstilos::InsereEstilo(strESTILO *str)
{
	VTEstilo *estilo = NULL;

	try
	{
		//cria o estilo de acordo com o numero de atributos (provisório)
		switch (str->atributos.Length)
		{
			case 12://figCOMENTARIO:
				estilo = NewObjEstiloComentario(apl);
				PreencheEstilo(estilo, str);
				break;
			case 3://figLINK:
				estilo = NewObjEstiloLink(apl);
				PreencheEstilo(estilo, str);
				break;
			case 5://figRETANGULO:
				estilo = NewObjEstiloRetangulo(apl);
				PreencheEstilo(estilo, str);
				break;
			default:
				//faz nada
			;
		}
		if (estilo != NULL)
		{
			if (!InsereEstilo(estilo))
			{
				if (estilo)
				{
					delete estilo;
					estilo = NULL;
				}
			}
        }
	}
	catch (...)
	{
        estilo = NULL;
    }

	return estilo;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
VTEstilo* __fastcall TEstilos::InsereEstilo(VTFigura *figura, AnsiString descricao)
{
	VTEstilo *estilo = NULL;
//	AnsiString descricao;
	try
	{
		if (descricao.IsEmpty())
		{
			descricao = CriaHash(figura);
		}
		//cria o estilo de acordo com o TipoFigura
		switch (figura->TipoFigura)
		{
			case figCOMENTARIO:
				estilo = NewObjEstiloComentario(apl);
				PreencheEstilo(estilo, (VTFiguraComentario*)figura);
				estilo->Descricao = descricao;
				break;
			case figLINK:
				estilo = NewObjEstiloLink(apl);
				PreencheEstilo(estilo, (VTLink*)figura);
				estilo->Descricao = descricao;
				break;
			case figRETANGULO:
				estilo = NewObjEstiloRetangulo(apl);
				PreencheEstilo(estilo, (VTRetangulo*)figura);
				estilo->Descricao = descricao;
				break;
			default:
			;
		}
		if (estilo != NULL)
		{
			if (!InsereEstilo(estilo))
			{
				if (estilo)
				{
					delete estilo;
					estilo = NULL;
				}
			}
			else
			{
                estilo->Id = FalsoId();
            }
		}
	}
	catch (...)
	{
        estilo = NULL;
    }

	return estilo;
}

// ---------------------------------------------------------------------------
bool __fastcall TEstilos::InsereEstilo(VTEstilo *estilo)
{
	// proteção
	if (estilo == NULL)
		return (false);
	// insere Comentario na lista
	if (PD.lisESTILO->IndexOf(estilo) < 0)
	{
		PD.lisESTILO->Add(estilo);
		//insere no map tambem
        mapaEstilo.insert(std::pair<AnsiString,VTEstilo*>(estilo->Descricao,estilo));
	}
	return (true);
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall  TEstilos::InsereEstiloAtributoComentario(VTEstilo *estilo, VTFiguraComentario *comentario)
{
	strESTILO_ATRIBUTO *referencia = NULL;
	bool sucesso = true;
	int auxBBool = 0;

	try
	{ 	// guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;
		//cria reverencia pra reuso
		referencia = new strESTILO_ATRIBUTO;
		referencia->salvoNaBase = false;
		//insere FontName
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_FONTNAME);
		referencia->valor = comentario->Font->Name;
		if(referencia->atributoGraficoId > 0)
			{estilo->InsereCopiaEstiloAtributo(referencia);}
		//insere FontSize
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_FONTSIZE);
		try
		{
			referencia->valor = IntToStr(comentario->Font->Size);
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
		//insere FontColor
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_FONTCOLOR);
		try
		{
			referencia->valor = IntToStr((int)comentario->Font->Color);
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
		//insere FontBold
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_FONTBOLD);
		try
		{
			if (comentario->Font->Style.Contains(fsBold))
				{auxBBool= 1;}
			else
				{auxBBool= 0;}
			referencia->valor = IntToStr(auxBBool);
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
		//insere FontItalic
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_FONTITALIC);
		try
		{
			if (comentario->Font->Style.Contains(fsItalic))
				{auxBBool= 1;}
			else
				{auxBBool= 0;}
			referencia->valor = IntToStr(auxBBool);
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
		//insere FontUnderline
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_FONTUNDERLINE);
		try
		{
			if (comentario->Font->Style.Contains(fsUnderline))
				{auxBBool= 1;}
			else
				{auxBBool= 0;}
			referencia->valor = IntToStr(auxBBool);
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
		//insere FontStrikeOut
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_FONTSTRIKEOUT);
		try
		{
			if (comentario->Font->Style.Contains(fsStrikeOut))
				{auxBBool= 1;}
			else
				{auxBBool= 0;}
			referencia->valor = IntToStr(auxBBool);
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
		//essa proxima parte podia ser so InsereEstiloAtributoRetangulo
		//insere BordaTipo
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_BORDATIPO);
		try
		{
			referencia->valor = IntToStr(comentario->Retangulo->BordaEstilo);
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
		//insere BordaEspessura
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_BORDAESPESSURA);
		try
		{
			referencia->valor = IntToStr(comentario->Retangulo->BordaEspessura);
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
		//insere BordaCor
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_BORDACOR);
		try
		{
			referencia->valor = IntToStr(int(comentario->Retangulo->BordaCor));
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
		//insere FundoCor
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_FUNDOCOR);
		try
		{
			referencia->valor = IntToStr(int(comentario->Retangulo->FundoCor));
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
		//insere transparente
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_TRANSPARENTE);
		try
		{
			if (comentario->Retangulo->Transparente)
				{auxBBool= 1;}
			else
				{auxBBool= 0;}
			referencia->valor = IntToStr(auxBBool);
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
	}
    catch(Exception &e)
	{
//		Loga(mErrorFunction + " " + e.Message);
		sucesso = false;
	}
	catch(...)
	{
//		Loga(mErrorFunction);
		sucesso = false;
	}

	if (referencia)
	{
		delete referencia;
		referencia = NULL;
	}
	return sucesso;
}
// ---------------------------------------------------------------------------
bool __fastcall  TEstilos::InsereEstiloAtributoLink(VTEstilo *estilo, VTLink *link)
{
	strESTILO_ATRIBUTO *referencia = NULL;
	bool sucesso = true;
	int auxBBool = 0;

	// estilo link
			// infoset->AddInfo("Estilo", link->Estilo);
			// infoset->AddInfo("Espessura", link->Espessura);
			// infoset->AddInfo("Color", link->Color);
	try
	{ 	// guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;
		//cria reverencia pra reuso
		referencia = new strESTILO_ATRIBUTO;
		referencia->salvoNaBase = false;
		//insere BordaTipo
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_BORDATIPO);
		try
		{
			referencia->valor = IntToStr(link->BordaTipo);
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
		//insere BordaEspessura
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_BORDAESPESSURA);
		try
		{
			referencia->valor = IntToStr(link->Espessura);
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
		//insere BordaCor
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_BORDACOR);
		try
		{
			referencia->valor = IntToStr(int(link->Color));
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
	}
	catch(Exception &e)
	{
//		Loga(mErrorFunction + " " + e.Message);
		sucesso = false;
	}
	catch(...)
	{
//		Loga(mErrorFunction);
		sucesso = false;
	}

	if (referencia)
	{
		delete referencia;
		referencia = NULL;
	}
	return sucesso;
}
// ---------------------------------------------------------------------------
bool __fastcall  TEstilos::InsereEstiloAtributoRetangulo(VTEstilo *estilo, VTRetangulo *retangulo)
{
	strESTILO_ATRIBUTO *referencia = NULL;
	bool sucesso = true;
	int auxBBool = 0;

	try
	{ 	// guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;
		//cria reverencia pra reuso
		referencia = new strESTILO_ATRIBUTO;
		referencia->salvoNaBase = false;
		//insere BordaTipo
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_BORDATIPO);
		try
		{
			referencia->valor = IntToStr(retangulo->BordaEstilo);
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
		//insere BordaEspessura
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_BORDAESPESSURA);
		try
		{
			referencia->valor = IntToStr(retangulo->BordaEspessura);
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
		//insere BordaCor
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_BORDACOR);
		try
		{
			referencia->valor = IntToStr(int(retangulo->BordaCor));
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
		//insere FundoCor
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_FUNDOCOR);
		try
		{
			referencia->valor = IntToStr(int(retangulo->FundoCor));
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
		//insere transparente
		referencia->atributoGraficoId = AtributoGraficoId(AUXF_AG_TRANSPARENTE);
		try
		{
			if (retangulo->Transparente)
				{auxBBool= 1;}
			else
				{auxBBool= 0;}
			referencia->valor = IntToStr(auxBBool);
			if(referencia->atributoGraficoId > 0)
				{estilo->InsereCopiaEstiloAtributo(referencia);}
		}
		catch(...)
		{
//			Loga("");
		}
	}
    catch(Exception &e)
	{
//		Loga(mErrorFunction + " " + e.Message);
		sucesso = false;
	}
	catch(...)
	{
//		Loga(mErrorFunction);
		sucesso = false;
	}

	if (referencia)
	{
		delete referencia;
		referencia = NULL;
	}
	return sucesso;
}
// ---------------------------------------------------------------------------
strFORMA_ATRIBUTO* __fastcall TEstilos::InsereFormaAtributo(AnsiString nome)
{
	strFORMA_ATRIBUTO * str= NULL;
	std::map<AnsiString,strFORMA_ATRIBUTO*>::iterator it;

	it = mapaFormaAtributo.find(nome);
	if (it == mapaFormaAtributo.end())
	{
		// loop registros (ini)
		// loop registros (ini)
		str = new strFORMA_ATRIBUTO;
		str->salvoNaBase = false;
		str->nome = nome;
		//insere no map
		mapaFormaAtributo.insert(pair<AnsiString,strFORMA_ATRIBUTO*>(nome,str));
		PD.lisFormaAtributo->Add(str);
	}
	else
	{
		str = it->second;
	}

	return str;
}
// ---------------------------------------------------------------------------
strAUXILIAR_FIGURA*	 __fastcall TEstilos::InsereForma(AnsiString descricao, int id)
{
	strAUXILIAR_FIGURA * str = NULL;
	std::map<AnsiString,strAUXILIAR_FIGURA*>::iterator it;

	it = mapaForma.find(descricao);
	if (it == mapaForma.end())
	{
		// loop registros (ini)
		str = new strAUXILIAR_FIGURA;
		str->salvoNaBase = false;
		str->descricao = descricao;
		str->id = id;
		//insere no map
		mapaForma.insert(pair<AnsiString,strAUXILIAR_FIGURA*>(descricao,str));
		PD.lisForma->Add(str);
	}
	else
	{
		str = it->second;
	}

	return str;
}
// ---------------------------------------------------------------------------
strAUXILIAR_FIGURA*	 __fastcall TEstilos::InsereTipoValorAtributoGrafico(AnsiString descricao, int id)
{
    strAUXILIAR_FIGURA * str;
    std::map<AnsiString,strAUXILIAR_FIGURA*>::iterator it;

	it = mapaTipoValorAtributoGrafico.find(descricao);
	if (it == mapaTipoValorAtributoGrafico.end())
	{
		// loop registros (ini)
		str = new strAUXILIAR_FIGURA;
		str->salvoNaBase = false;
		str->descricao = descricao;
		str->id = id;
		//insere no map
		mapaTipoValorAtributoGrafico.insert(pair<AnsiString,strAUXILIAR_FIGURA*>(descricao,str));
		PD.lisTipoValorAtributoGrafico->Add(str);
	}else
	{
		str = it->second;
	}

	return str;
}
// ---------------------------------------------------------------------------
bool __fastcall TEstilos::LeEstilosBase(void)
{
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTBD * bd = DLL_NewObjBD(apl);
	bool sucesso = false;

	try
	{

		bd->Conecta();
		sucesso = bd->Figura->LeAuxiliaresFiguras();
		bd->Desconecta();

		delete bd;
	}
	catch(Exception &e)
	{
        return false;
    }
	return sucesso;
}
// ---------------------------------------------------------------------------
//TList* __fastcall TEstilos::LisEstilo(void)
//{
//	return lisESTILO;
//}

// ---------------------------------------------------------------------------
VTEstilo* __fastcall TEstilos::MapaEstilo(AnsiString descricao)
{
	VTEstilo *str = NULL;
	map<AnsiString,VTEstilo*>::iterator it;

	it = mapaEstilo.find(descricao);
	if (it!= mapaEstilo.end())
	{
		str = it->second;
	}

	return str;
}
// ---------------------------------------------------------------------------
int  __fastcall TEstilos::MapaEstiloId(AnsiString descricao)
{
	VTEstilo *str = NULL;
	map<AnsiString,VTEstilo*>::iterator it;
	int id = FalsoId();

	it = mapaEstilo.find(descricao);
	if (it!= mapaEstilo.end())
	{
		str = it->second;
		id = str->Id;
	}

	return id;
}
// ---------------------------------------------------------------------------
bool __fastcall TEstilos::PreencheAuxiliares(void)
{
	bool sucesso = true;

	try
	{
		sucesso &= PreencheDefaultTipoValorAtributoGrafico();
		sucesso &= PreencheDefaultForma();
		sucesso &= PreencheDefaultAtributoGrafico();
		sucesso &= PreencheDefaultFormaAtributo();
	}
	catch(...)
	{
		sucesso = false;
	}

	return sucesso;
}
// ---------------------------------------------------------------------------
bool __fastcall TEstilos::PreencheDefaultAtributoGrafico(void)
{
	bool sucesso = true;
	bool sucessoTotal = true;
	char *valorInt[] =
		{AUXF_AG_BORDATIPO, AUXF_AG_BORDAESPESSURA, AUXF_AG_BORDACOR,
		AUXF_AG_FUNDOCOR, AUXF_AG_TRANSPARENTE, AUXF_AG_FONTSIZE,
		AUXF_AG_FONTCOLOR, AUXF_AG_FONTBOLD, AUXF_AG_FONTITALIC,
		AUXF_AG_FONTUNDERLINE, AUXF_AG_FONTSTRIKEOUT};

	char *valorString[] ={AUXF_AG_FONTNAME};
	int id =1;
	strATRIBUTO_GRAFICO *str;
	strAUXILIAR_FIGURA *aux;
	std::map<AnsiString,strAUXILIAR_FIGURA*>::iterator it;

	try
	{
		//insere os tipos de atributos  inteiros
		for (int nc = 0; nc < sizeof(valorInt) / sizeof(char*); nc++)
		{
			//preenche estrutura
			//1o procura o id do tipo inteiro
			it = mapaTipoValorAtributoGrafico.find(AUXF_TVAG_INT);
			if (it!= mapaTipoValorAtributoGrafico.end())
			{
				aux = it->second;
				//encontr o id da forma
				str = InsereAtributoGrafico(valorInt[nc]);
				str->tipoValorAG = aux->id;
				//preenche no map para consulta
				str->id =id;
				id++;
			}
		}
		//insere os tipos de atributos  string
		for (int ns = 0; ns < sizeof(valorString) / sizeof(char*); ns++)
		{
			//preenche estrutura
			//1o procura o id do tipo inteiro
			it = mapaTipoValorAtributoGrafico.find(AUXF_TVAG_STRING);
			if (it!= mapaTipoValorAtributoGrafico.end())
			{
				aux = it->second;
                //encontr o id da forma
				str = InsereAtributoGrafico(valorString[ns]);
				str->tipoValorAG = aux->id;
				str->descricao = valorString[ns];
				//preenche no map para consulta
				str->id =id;
				id++;
			}
		}
	}
	catch(Exception &e)
	{
//		Loga(e.Message);
		sucessoTotal = false;
	}
	catch(...)
	{
//		Loga(mErrorFunction);
		sucessoTotal = false;
	}

	return sucessoTotal;
}
// ---------------------------------------------------------------------------
//bool __fastcall TEstilos::PreencheDefaultEstilo(void)
//{
//	bool sucesso = true;
//	bool sucessoTotal = true;
//	char *valor[] ={AUXF_ESTILO_DEFAULT};
//	int id =0;
//	VTEstilo *estilo;
//
//    /* TODO :
//isso aqui está meio hardcoded pro id = 0 ser um default...
//tá bom não */
//    try
//	{ // guarda o nome da funcao
//		mErrorFunction = __PRETTY_FUNCTION__;
//		//insere os tipos de atributos
//		for (int nc = 0; nc < sizeof(valor) / sizeof(char*); nc++)
//		{
//			estilo = new strESTILO;
//			estilo->Id = id;
//			estilo->Descricao = valor[nc];
//			mapaEstilo.insert(pair<AnsiString,VTEstilo*>(valor[nc],estilo));
//			lisEstilo->Add(estilo);
//			id++;
//		}
//	}
//    catch(Exception &e)
//	{
//		Loga(mErrorFunction + " " + e.Message);
//		sucessoTotal = false;
//	}
//	catch(...)
//	{
//		Loga(mErrorFunction);
//		sucessoTotal = false;
//	}
//	return sucessoTotal;
//}
// ---------------------------------------------------------------------------
bool __fastcall TEstilos::PreencheDefaultFormaAtributo(void)
{
	bool sucesso = true;
	bool sucessoTotal = true;
	char *valorRetangulo[] ={AUXF_FORMAAT_DIMX, AUXF_FORMAAT_DIMY};
	int id =1;
	strFORMA_ATRIBUTO *str;
	strAUXILIAR_FIGURA *aux;
	map<AnsiString,strAUXILIAR_FIGURA*>::iterator it;

	try
	{ // guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;
		//insere os tipos de atributos
		for (int nc = 0; nc < sizeof(valorRetangulo) / sizeof(char*); nc++)
		{
			//preenche estrutura
			//1o procura o id do retangulo
			it = mapaForma.find(AUXF_FORMA_RETANGULO);
			if (it!= mapaForma.end())
			{
				aux = it->second;
				//encontr o id da forma
				str = InsereFormaAtributo(valorRetangulo[nc]);
				str->formaId = aux->id;
				//poe o valor
				str->nome = valorRetangulo[nc];
				str->tipoValor = AUXF_TVAG_INT;
				//preenche no map para consulta
				str->id = id;
				id++;
			}
		}
	}
	catch(Exception &e)
	{
//		Loga(mErrorFunction + " " + e.Message);
		sucessoTotal = false;
	}
	catch(...)
	{
//		Loga(mErrorFunction);
		sucessoTotal = false;
	}

	return sucessoTotal;
}
// ---------------------------------------------------------------------------
bool __fastcall TEstilos::PreencheDefaultForma(void)
{
	bool sucesso = true;
	bool sucessoTotal = true;
	char *valor[] ={AUXF_FORMA_RETANGULO};
	int id =1;
	strAUXILIAR_FIGURA * aux;

    try
	{ // guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;
		//insere os tipos de atributos
		for (int nc = 0; nc < sizeof(valor) / sizeof(char*); nc++)
		{
			aux = InsereForma(valor[nc], id);
			mapaForma.insert(pair<AnsiString,strAUXILIAR_FIGURA*>(valor[nc],aux));
			id++;
		}
	}
	catch(Exception &e)
	{
//		Loga(mErrorFunction + " " + e.Message);
		sucessoTotal = false;
	}
	catch(...)
	{
//		Loga(mErrorFunction);
		sucessoTotal = false;
	}
	return sucessoTotal;
}
// ---------------------------------------------------------------------------
bool __fastcall TEstilos::PreencheDefaultTipoValorAtributoGrafico(void)
{
	bool sucesso = true;
	bool sucessoTotal = true;
	char *valor[] ={AUXF_TVAG_INT, AUXF_TVAG_DOUBLE, AUXF_TVAG_STRING};
	int id =1;
	strAUXILIAR_FIGURA *aux;

    try
	{ // guarda o nome da funcao
		mErrorFunction = __PRETTY_FUNCTION__;
		//insere os tipos de atributos
		for (int nc = 0; nc < sizeof(valor) / sizeof(char*); nc++)
		{
//			sucesso = SPSNP_wrTipoValorAtributGraficoIns(valor[nc]);
			aux = InsereTipoValorAtributoGrafico(valor[nc], id);
			mapaTipoValorAtributoGrafico.insert(pair<AnsiString,strAUXILIAR_FIGURA*>(valor[nc],aux));
			id++;
		}
	}
	catch(Exception &e)
	{
//		Loga(mErrorFunction + " " + e.Message);
		sucessoTotal = false;
	}
	catch(...)
	{
//		Loga(mErrorFunction);
		sucessoTotal = false;
	}

	return sucessoTotal;
}
// ---------------------------------------------------------------------------
void __fastcall TEstilos::PreencheEstilo(VTEstilo *estilo, strESTILO *str)
{
	VTEstiloComentario *estC;
	VTEstiloLink *estL;
	VTEstiloRetangulo *estR;
	strESTILO_ATRIBUTO *atrib = NULL;
	AnsiString tahoma = "Tahoma";

	try
	{
		estilo->Id = str->id;
		switch (str->atributos.Length)
		{
			case 12://figCOMENTARIO:
                if (estilo->TipoEstilo == estiloCOMENTARIO)
				{
					estC= (VTEstiloComentario*) estilo;
					//font
					estC->FontColor = (TColor)ValorInt(EstiloAtributo(AtributoGraficoId(AUXF_AG_FONTCOLOR),str));
					atrib = EstiloAtributo(AtributoGraficoId(AUXF_AG_FONTNAME),str);
					estC->FontName = ((atrib == NULL)? tahoma : atrib->valor);
					estC->FontSize = ValorInt(EstiloAtributo(AtributoGraficoId(AUXF_AG_FONTSIZE),str));
					estC->FontBold = ValorBool(EstiloAtributo(AtributoGraficoId(AUXF_AG_FONTBOLD),str));
					estC->FontItalic = ValorBool(EstiloAtributo(AtributoGraficoId(AUXF_AG_FONTITALIC),str));
					estC->FontUnderline = ValorBool(EstiloAtributo(AtributoGraficoId(AUXF_AG_FONTUNDERLINE),str));
					estC->FontStrikeOut = ValorBool(EstiloAtributo(AtributoGraficoId(AUXF_AG_FONTSTRIKEOUT),str));
					//retangulo
					estC->FundoCor = (TColor)ValorInt(EstiloAtributo(AtributoGraficoId(AUXF_AG_FUNDOCOR),str));
					estC->FundoTransparente = ValorBool(EstiloAtributo(AtributoGraficoId(AUXF_AG_TRANSPARENTE),str));
					estC->Bordacor = (TColor)ValorInt(EstiloAtributo(AtributoGraficoId(AUXF_AG_BORDACOR),str));
					estC->BordaEspessura = ValorInt(EstiloAtributo(AtributoGraficoId(AUXF_AG_BORDAESPESSURA),str));
					estC->BordaTipo = ValorInt(EstiloAtributo(AtributoGraficoId(AUXF_AG_BORDATIPO),str));;
				}
				break;
			case 3://figLINK:
                if (estilo->TipoEstilo == estiloLINK)
				{
					estL= (VTEstiloLink*) estilo;
					estL->Bordacor = (TColor)ValorInt(EstiloAtributo(AtributoGraficoId(AUXF_AG_BORDACOR),str));
					estL->BordaEspessura = ValorInt(EstiloAtributo(AtributoGraficoId(AUXF_AG_BORDAESPESSURA),str));
					estL->BordaTipo = ValorInt(EstiloAtributo(AtributoGraficoId(AUXF_AG_BORDATIPO),str));;
				}
				break;
			case 5://figRETANGULO:
				if (estilo->TipoEstilo == estiloRETANGULO)
				{
					estR= (VTEstiloRetangulo*) estilo;
					//retangulo
					estR->FundoCor = (TColor)ValorInt(EstiloAtributo(AtributoGraficoId(AUXF_AG_FUNDOCOR),str));
					estR->FundoTransparente = ValorBool(EstiloAtributo(AtributoGraficoId(AUXF_AG_TRANSPARENTE),str));
					estR->Bordacor = (TColor)ValorInt(EstiloAtributo(AtributoGraficoId(AUXF_AG_BORDACOR),str));
					estR->BordaEspessura = ValorInt(EstiloAtributo(AtributoGraficoId(AUXF_AG_BORDAESPESSURA),str));
					estR->BordaTipo = ValorInt(EstiloAtributo(AtributoGraficoId(AUXF_AG_BORDATIPO),str));;
				}
				break;
			default:
				//faz nada
			;
		}
	}
	catch(...)
	{

    }
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TEstilos::PreencheEstilo(VTEstilo *estilo, VTFiguraComentario *comentario)
{
	VTEstiloComentario *est;

	try
	{
		if (estilo->TipoEstilo == estiloCOMENTARIO)
		{
			est= (VTEstiloComentario*) estilo;
			//font
			est->FontColor = comentario->Font->Color;
			est->FontName = comentario->Font->Name;
			est->FontSize = comentario->Font->Size;
			est->FontBold = comentario->Font->Style.Contains(fsBold);
            est->FontItalic = comentario->Font->Style.Contains(fsItalic);
			est->FontUnderline = comentario->Font->Style.Contains(fsUnderline);
			est->FontStrikeOut = comentario->Font->Style.Contains(fsStrikeOut);
			//retangulo
			est->FundoCor = comentario->Retangulo->FundoCor;
			est->FundoTransparente = comentario->Retangulo->Transparente;
			est->Bordacor = comentario->Retangulo->BordaCor;
			est->BordaEspessura = comentario->Retangulo->BordaEspessura;
			est->BordaTipo = comentario->Retangulo->BordaEstilo;
		}

	}
	catch(...)
	{

    }
}
// ---------------------------------------------------------------------------
void __fastcall TEstilos::PreencheEstilo(VTEstilo *estilo, VTLink *link)
{
	VTEstiloLink *est;

	try
	{
		if (estilo->TipoEstilo == estiloLINK)
		{
			est= (VTEstiloLink*) estilo;
			est->Bordacor = link->Color;
			est->BordaEspessura = link->Espessura;
			est->BordaTipo = link->BordaTipo;
		}
	}
	catch(...)
	{

	}
}
// ---------------------------------------------------------------------------
void __fastcall TEstilos::PreencheEstilo(VTEstilo *estilo, VTRetangulo *retangulo)
{
	VTEstiloRetangulo *est;

	try
	{
		if (estilo->TipoEstilo == estiloRETANGULO)
		{
			est= (VTEstiloRetangulo*) estilo;
			//retangulo
			est->FundoCor = retangulo->FundoCor;
			est->FundoTransparente = retangulo->Transparente;
			est->Bordacor = retangulo->BordaCor;
			est->BordaEspessura = retangulo->BordaEspessura;
			est->BordaTipo = retangulo->BordaEstilo;
		}
	}
	catch(...)
	{

	}
}
// ---------------------------------------------------------------------------
void __fastcall TEstilos::RedefineIdsEstilo(void)
{
	VTEstilo *estilo = NULL;

	for (int ne; ne < PD.lisESTILO->Count; ne++)
	{
		estilo = (VTEstilo*) PD.lisESTILO->Items[ne];
        estilo->Id = FalsoId();
	}
}
// ---------------------------------------------------------------------------
bool __fastcall TEstilos::RetiraEstilo(VTEstilo *estilo)
{
	bool sucesso = false;
	try
	{
		if (PD.lisESTILO->IndexOf(estilo) > 0)
		{
			PD.lisESTILO->Remove(estilo);
			delete estilo;
			estilo = NULL;
			sucesso = true;
		}
	}
	catch(...)
	{
        sucesso = false;
	}

	return sucesso;
}
// ---------------------------------------------------------------------------
void __fastcall TEstilos::ResetAuxiliaresLidos(void)
{
	strAUXILIAR_FIGURA *aux;

	for (int n = 0; n < PD.lisAtributoGrafico->Count; n++)
	{
		aux = (strAUXILIAR_FIGURA*)PD.lisAtributoGrafico->Items[n];
		aux->salvoNaBase = false;
	}
	for (int n = 0; n < PD.lisFormaAtributo->Count; n++)
	{
		aux = (strAUXILIAR_FIGURA*)PD.lisFormaAtributo->Items[n];
		aux->salvoNaBase = false;
	}
	for (int n = 0; n < PD.lisForma->Count; n++)
	{
		aux = (strAUXILIAR_FIGURA*)PD.lisForma->Items[n];
		aux->salvoNaBase = false;
	}
	for (int n = 0; n < PD.lisTipoValorAtributoGrafico->Count; n++)
	{
        aux = (strAUXILIAR_FIGURA*)PD.lisTipoValorAtributoGrafico->Items[n];
		aux->salvoNaBase = false;
	}
}
// ---------------------------------------------------------------------------
int  __fastcall TEstilos::TipoFiguraToFormaId(VTFigura *figura)
{
	AnsiString formaDescr = "";
	int formaId = -1;
	map<AnsiString,strAUXILIAR_FIGURA*>::iterator it;
    strAUXILIAR_FIGURA *aux;

	if (figura == NULL)
	{
		return formaId;
	}
	//procura no map de acordo
	switch (figura->TipoFigura)
	{
		default:
			formaDescr = AUXF_FORMA_RETANGULO;
        ;
	}
	it = mapaForma.find(formaDescr);
	if (it != mapaForma.end() )
	{
		aux = it->second;
		formaId = aux->id;
	}

	return  formaId;
}
// ---------------------------------------------------------------------------
bool __fastcall TEstilos::ValorBool(strESTILO_ATRIBUTO *ref)
{
	int valor = -1;

	try
	{
		valor = StrToInt(ref->valor);
	}
	catch(...)
	{
		valor = -1;
	}

	return (valor == 1);
}
// ---------------------------------------------------------------------------
int __fastcall TEstilos::ValorInt(strESTILO_ATRIBUTO *ref)
{
	int valor = -1;

	try
	{
		valor = StrToInt(ref->valor);
	}
	catch(...)
	{
		valor = -1;
	}

    return valor;
}
// ---------------------------------------------------------------------------
// eof
