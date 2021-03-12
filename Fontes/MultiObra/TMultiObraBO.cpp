//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMultiObraBO.h"
#include "VTMultiObra.h"
//---------------------------------------------------------------------------
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Licenca\VTEmpresa.h>
#include <PlataformaSinap\Fontes\Licenca\VTLicenca.h>
#include <PlataformaSinap\Fontes\Licenca\VTLicencas.h>
#include <PlataformaSinap\Fontes\Obra\VTObra.h>
#include <PlataformaSinap\Fontes\Obra\ItemObra\VTItemObra.h>
#include <PlataformaSinap\Fontes\Planejamento\VTIpe.h>
#include <PlataformaSinap\Fontes\Planejamento\VTPlanejamento.h>
//---------------------------------------------------------------------------
#include <ModulosOpcionais\Fontes\BaseGRP\VTIpeGRP.h>
//---------------------------------------------------------------------------
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
VTMultiObraBO* __fastcall NewObjMultiObraBO(VTApl *apl_owner)
{
    try
	{
		return (new TMultiObraBO(apl_owner));
	}
	catch (Exception &e)
	{
		return (NULL);
	}
}
////---------------------------------------------------------------------------
//VTMultiObraBO* __fastcall NewObjMultiObraBO(VTApl *apl_owner)
//   {
//   try{
//	  return (new TMultiObraBO(apl_owner));
//      }catch (Exception &e)
//         {
//         return(NULL);
//         }
//   }

//---------------------------------------------------------------------------
__fastcall TMultiObraBO::TMultiObraBO(VTApl *apl_owner)
   {
   //salva ponteir
   this->apl = apl_owner;
   //cria listas
	lisMultiObra = new TList;
   }

//---------------------------------------------------------------------------
__fastcall TMultiObraBO::~TMultiObraBO(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisMultiObra) {delete lisMultiObra; lisMultiObra = NULL;}
   }
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/15938942
VTMultiObra* __fastcall TMultiObraBO::AnalisaAcoesObra(VTObra *obra)
{
	VTMultiObra *multiObra = NULL;
	bool novo = false;
	bool sucesso = false;

	//se nao existe cria novo
	if ((multiObra = ExisteMultiObra(obra)) == NULL)
	{
		try
		{
			//cria um novo
			novo = true;
			multiObra = NewObjMultiObra(apl);
		}
		catch(...)
		{
			DestroiTObject(multiObra);
			multiObra = NULL;
		}
	}
	//verifica se precisa analisar novamente a obra
	if ((novo) || (multiObra->UpToDate == false))
	{
		try
		{
			if(multiObra->UpToDate == false)
			{
                multiObra->Reinicia();
            }
			sucesso = multiObra->AnalisaObra(obra);
			//se falhou
			if (!sucesso)
			{
				DestroiTObject(multiObra);
				multiObra = NULL;
			}
		}
		catch(...)
		{
			DestroiTObject(multiObra);
			multiObra = NULL;
		}
	}
	//verifica se é novo
	if ((novo) &&(multiObra !=NULL))
	{
		lisMultiObra->Add(multiObra);
		//insere relacao
		mapMultiObra.insert(std::pair<VTObra*, VTMultiObra*>(obra, multiObra));
	}

	return multiObra;
}
//---------------------------------------------------------------------------
void __fastcall TMultiObraBO::DestroiMultiObra(VTObra *obra)
{
	VTMultiObra *multiObra = NULL;

	if (obra == NULL)
		{return;}
	multiObra =  ExisteMultiObra(obra);
	if (multiObra != NULL)
	{
		delete multiObra;
        //remove do map
		mapMultiObra.erase(obra);
	}
	//limpa os items da obra
	LimpaTList(obra->LisItemObra);
}
//---------------------------------------------------------------------------
VTMultiObra* __fastcall TMultiObraBO::ExisteMultiObra(VTObra *obra)
{
	std::map<VTObra*, VTMultiObra*>::iterator it;
	VTMultiObra *multiObra = NULL;

	it = mapMultiObra.find(obra);
	//se encontrou algo
	if (it != mapMultiObra.end())
	{
		multiObra = it->second;
	}

	return multiObra;
}
//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17073894
bool __fastcall TMultiObraBO::IsObraFinalizada(VTObra *obra)
{
    // variaveis locais
	VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
	VTLicenca *licenca = licencas->ExisteLicenca();
	VTItemObra *item;
	bool finalizada = true;

	if (obra == NULL)
	{
        return false;
	}
	//1o percorre os items da obra verificando se eles foram finalizados
	for(int ni = 0; ni < obra->LisItemObra->Count; ni++)
	{
		item = (VTItemObra*) obra->LisItemObra->Items[ni];

		if (item->Contabilizar)
		{
			if (!item->Configurado)
			{
				finalizada = false;
			}
		}
	}
	//se for Copel
	if (licenca != NULL)
	{
		if (licenca->Empresa == COPEL)
		{   //verifica se a obra tem regiao e obraCopel
			finalizada &= ((PossuiIpeAtributo(obra, ipeREGIAO))
										 && (PossuiIpeAtributo(obra, ipeOBRA)));
        }
	}
	//a obra esta finalizada se cumprir os requisitos E o usuario "setou"
    //finalizada &= obra->Finalizada;

	return finalizada;
}
//---------------------------------------------------------------------------
bool __fastcall TMultiObraBO::PossuiIpeAtributo(VTObra *obra, int tipoAtributo)
{	// variaveis locais
	VTIpeAtributo *atributo;
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTIpeGRP *ipe_aries = (VTIpeGRP*)planejamento->Ipe;

	if (obra == NULL)
	{
        return false;
	}
	for (int n = 0; n < obra->LisObject->Count; n++)
	{ // localiza atributo texto
		atributo = (VTIpeAtributo*)obra->LisObject->Items[n];
		if (atributo->Tipo != tipoAtributo)
			continue;
		// retorna descricao do atributo
		return (true);
	}
	return (false);
}
//---------------------------------------------------------------------------
void __fastcall TMultiObraBO::ReiniciaMultiObra(VTObra *obra)
{
    if (obra == NULL)
		{return;}
	obra->Bloqueada = false;
	obra->Finalizada = false;
	DestroiMultiObra(obra);
}
//---------------------------------------------------------------------------
//eof

