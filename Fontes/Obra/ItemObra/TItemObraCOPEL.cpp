// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TItemObraCOPEL.h"
#include <DateUtils.hpp>
// ---------------------------------------------------------------------------
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Planejamento\VTIpe.h>
#include <PlataformaSinap\Fontes\Planejamento\VTPlanejamento.h>
// ---------------------------------------------------------------------------
#include <ModulosOpcionais\Fontes\BaseGRP\VTIpeGRP.h>
#include <ModulosOpcionais\Fontes\BaseGRP\VTPadrao.h>
// ---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// funções globais p/ criar objetos da classe TObra
// ---------------------------------------------------------------------------
VTItemObra* NewObjItemObraCOPEL(VTApl *apl)
{
	return (new TItemObraCOPEL(apl));
}

// ---------------------------------------------------------------------------
__fastcall TItemObraCOPEL::TItemObraCOPEL(VTApl *apl):VTItemObraCOPEL(apl)
{

//	copel.siglaObra = "";
//	copel.siglaRegiao= "";
//	copel.programa= "";
	copel.filepathCroqui= "";
//	copel.municipioPrincipal = "";
//	copel.lisMunicipiosExtras = new TStringList;
}

// ---------------------------------------------------------------------------
__fastcall TItemObraCOPEL::~TItemObraCOPEL(void)
{
//	if (copel.lisMunicipiosExtras)
//	{
//		delete copel.lisMunicipiosExtras;
//		copel.lisMunicipiosExtras = NULL;
//	}
}
// ---------------------------------------------------------------------------
VTItemObra* __fastcall TItemObraCOPEL::Clone(bool preserveId, bool isDeep)
{
	VTItemObra *clone;

	try
	{
		clone = NewObjItemObraCOPEL(apl);
		clone->CopiaAtributosDe(*this);
		if (!preserveId)
		{
			clone->Id = FalsoId();
		}
		if (isDeep)
		{
			//associa as acoes clones
			AssociaAcoesClone(clone);
			//limpa atributos
			clone->SalvaIpeAtributos();
		}
	}
	catch(...)
	{
        clone = NULL;
	}

	return clone;
}
// ---------------------------------------------------------------------------
void __fastcall TItemObraCOPEL::CopiaAtributosDe(VTItemObra &ref)
{
	// variáveis locais
	VTItemObraCOPEL &item = (VTItemObraCOPEL&)ref;

	// proteção
//	if (!ref.TipoEqbar())
//		return;
	// copia atributos de VTItemObraARIES
	TItemObraARIES::CopiaAtributosDe(ref);
	// copia atributos de VTItemObraCOPEL
	// copia atributos de VTItemObraARIES
//	this->SiglaObra     = item.SiglaObra;
//	this->SiglaRegiao = item.SiglaRegiao;
	this->Croqui  = item.Croqui;
//	this->MunicipioPrincipal  = item.MunicipioPrincipal;
//	//copia a lista
//	this->LisMunicipios->Assign(item.LisMunicipios);
}
// ---------------------------------------------------------------------------
void __fastcall  TItemObraCOPEL::CorrigeValores(int tipoEqpto)
{
	TItemObraARIES::CorrigeValores(tipoEqpto);
}
// ---------------------------------------------------------------------------
void __fastcall  TItemObraCOPEL::IniciaExtras(void)
{
	TItemObraARIES::IniciaExtras();
}
// ---------------------------------------------------------------------------
void __fastcall TItemObraCOPEL::LeIpeAtributo(VTIpeAtributo *atributo)
{
//	VTIpeAtributo *tipo_insere, *tipo_retira;
//	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
//	VTIpeGRP *ipe_grp = (VTIpeGRP*)planejamento->Ipe;

	int valorInt = 0;
	double valorDouble = 0;

	try
	{
		if (atributo != NULL)
		{
			switch (atributo->Tipo)
			{
				case ipeOBRA:
//					this->SiglaObra = atributo->Descricao;
					break;
				case ipeREGIAO:
//					this->SiglaRegiao = atributo->Descricao;
					break;
				case ipeCROQUI:
					this->Croqui = atributo->Descricao;
					break;
//				case ipeMUNICIPIO:
//					if (!atributo->Descricao.IsEmpty())
//					{
//						LisMunicipios->Add(atributo->Descricao);
//					}
//					break;
//				case ipeMUNICIPIO_VPO:
//					this->MunicipioPrincipal = atributo->Descricao;
//					break;
			default:
                TItemObraARIES::LeIpeAtributo(atributo);
				;
			}
		}
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TItemObraCOPEL::MantemAtributosComunsDe(VTItemObra &ref)
{
	// variáveis locais
	VTItemObraCOPEL &item = (VTItemObraCOPEL&)ref;
	TStringList *strings = new TStringList;

	// proteção
//	if (!ref.TipoEqbar())
//		return;
	// copia atributos de VTItemObraARIES
	TItemObraARIES::MantemAtributosComunsDe(ref);
	// copia atributos de VTItemObraCOPEL
//	if(copel.siglaRegiao.AnsiCompare(item.SiglaRegiao)  == 0)
//		{copel.siglaRegiao = "";}
	if(copel.filepathCroqui.AnsiCompare(item.Croqui)  == 0)
		{copel.filepathCroqui = "";}
//	if(copel.programa.AnsiCompare(item.Programa)  == 0)
//		{copel.programa = "";}
//	if(copel.siglaObra.AnsiCompare(item.SiglaObra)  == 0)
//		{copel.siglaObra = "";}
//	if(copel.municipioPrincipal.AnsiCompare(item.MunicipioPrincipal)  == 0)
//		{copel.municipioPrincipal = "";}
//	//fazer pros extras?
//	for (int ns = this->LisMunicipios->Count - 1; ns >= 0; ns--)
//	{
//		if (item.LisMunicipios->IndexOf(this->LisMunicipios->Strings[ns]) < 0)
//		{
//			this->LisMunicipios->Delete(ns);
//		}
//	}

	delete strings;
}

// ---------------------------------------------------------------------------
void __fastcall TItemObraCOPEL::SalvaIpeAtributosProprios(void)
{
    // variaveis locais
	AnsiString texto;
	VTIpeAtributo *atributo;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTIpeGRP *ipe_grp = (VTIpeGRP*)planejamento->Ipe;
	int valorInt = 0;
	int valorDouble = 0;
	VTIpeAtributo *tipo_insere, *tipo_retira;

	try
	{
//		case ipeOBRA:
//			this->SiglaObra = atributo->Descricao;
//			break;
//		case ipeREGIAO:
//			this->SiglaRegiao = atributo->Descricao;
//			break;
//		case ipeCROQUI:
//			this->Croqui = atributo->Descricao;
//			break;
//		case ipeMUNICIPIO:
//			if (!atributo->Descricao.IsEmpty())
//			{
//				LisMunicipios->Add(atributo->Descricao);
//			}
//			break;
//		case ipeMUNICIPIO_VPO:
//			this->MunicipioPrincipal = atributo->Descricao;
//			break;

		//ipeOBRA
//		texto = this->SiglaObra;
//		atributo = ipe_grp->InsereAtributoTexto(texto, ipeOBRA,this->Id);
//		if (this->LisIpeAtributo->IndexOf(atributo)< 0)
//			{this->LisIpeAtributo->Add(atributo);}
//		//ipeREGIAO
//		texto = this->SiglaRegiao;
//		atributo = ipe_grp->InsereAtributoTexto(texto, ipeREGIAO, this->Id);
//		if (this->LisIpeAtributo->IndexOf(atributo)< 0)
//			{this->LisIpeAtributo->Add(atributo);}
		//ipeCROQUI
		texto = this->Croqui;
		atributo = ipe_grp->InsereAtributoTexto(texto, ipeCROQUI, this->Id);
		if (this->LisIpeAtributo->IndexOf(atributo)< 0)
			{this->LisIpeAtributo->Add(atributo);}
//		//ipeMUNICIPIO_VPO
//		texto = this->MunicipioPrincipal;
//		atributo = ipe_grp->InsereAtributoTexto(texto, this->Id, ipeMUNICIPIO_VPO);
//		if (this->LisIpeAtributo->IndexOf(atributo)< 0)
//			{this->LisIpeAtributo->Add(atributo);}
//		//ipeMUNICIPIO
//		for (int nmu = 0; nmu < this->LisMunicipios->Count; nmu++)
//		{
//			texto = this->LisMunicipios->Strings[nmu];
//			atributo = ipe_grp->InsereAtributoTexto(texto, this->Id, ipeMUNICIPIO);
//			if (this->LisIpeAtributo->IndexOf(atributo)< 0)
//				{this->LisIpeAtributo->Add(atributo);}
//		}
		//salva as ifno basicas
		TItemObraARIES::SalvaIpeAtributosProprios();
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
// ---------------------------------------------------------------------------
// eof
