// arquivos incluídos ---------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)
//
#include <System.Classes.hpp>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Log\VTLog.h>
#include <Fontes\Rede\VTRedes.h>
#include <DLL_Inc\Funcao.h>
// Project include
#include <TImportaBDGD.h>
#include <Commons\VTCommons.h>
#include <Commons\Entidades\TSubestacao.h>
#include <Commons\Entidades\TRedeMT.h>
#include <Forms\TFormImportadorUgrid.h>


// ---------------------------------------------------------------------------
VTImportaRedeGIS* __fastcall NewObjImportaBDGD(TComponent *Owner, VTApl *apl_owner)
{
	try
	{
		return (new TImportaBDGD(Owner, apl_owner));
	}
	catch (Exception &e)
	{
	}
	// erro na criação do objeto
	return (NULL);
}

// ---------------------------------------------------------------------------
__fastcall TImportaBDGD::TImportaBDGD(TComponent *Owner, VTApl *apl_owner)
{


	// salva ponteiro p/ objetos
	this->Owner = Owner;
	this->apl_owner = apl_owner;
	importador_arq = NULL;
	// cria lista
	lisSubestacao = new TList();
	lisAllSub     = new TList();
	// cria conversor
	apl_owner->Add(conversor = NewObjConversor(apl_owner));
	//verifica a existencia do commons
	commons = (VTCommons*)apl_owner->GetObject(__classid(VTCommons));
	if (commons == NULL)
	{
		apl_owner->Add(commons = NewObjCommons(apl_owner, NULL));
	}
}

// ---------------------------------------------------------------------------
__fastcall TImportaBDGD::~TImportaBDGD(void)
{
	// destrói lista e seus objetos
	LimpaTList(lisAllSub);
	DestroiTObject(lisAllSub);
	// destrói lista sem destruir seus objetos
	DestroiTObject(lisSubestacao);
}

// ---------------------------------------------------------------------------
bool __fastcall TImportaBDGD::ArquivoEgrid(void)
{
	//variáveis locais
	return (false);
}


// -------------------------------------------------------------------------------------------------
bool __fastcall TImportaBDGD::ImportaLisArqPrimario(TStrings *files, strOPImporta *opcao)
{
	return (true);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TImportaBDGD::ImportaLisArqSubestacao(TStrings *files, strOPImporta *opcao)
{
	return (true);
}

//// -------------------------------------------------------------------------------------------------
//VTImportador* __fastcall TImportaBDGD::PM_GetImportador(void)
//{
//	 return(this->Importador);
//}
//
//// -------------------------------------------------------------------------------------------------
//void __fastcall TImportaBDGD::PM_SetImportador(VTImportador *importadorArquivo)
//{
//	 this->Importador = importadorArquivo;
//}

// -------------------------------------------------------------------------------------------------
void __fastcall TImportaBDGD::SelecionaArqTxt(TOpenDialog *OpenDialog, TStrings *files)
{
	//variáveis locais
}

// -------------------------------------------------------------------------------------------------
void __fastcall TImportaBDGD::SelecionaLisArqTxt(TOpenDialog *OpenDialog, TStrings *files)
{
	//variáveis locais
}

// ---------------------------------------------------------------------------
bool __fastcall TImportaBDGD::ShowModalFormImportaRedeGIS(void)
{
	// variáveis locais
	TForm *form_child;
	bool sucesso = false;
	TFormImportadorUgrid *Form;
	VTImportador *importador;

	try
	{
		if(Importador == NULL)
		{
			// Criação do Importador UGRID
			importador = NewObjImportadorUgrid(apl_owner, "");
			Importador = importador;
			importador->ArqEgrid = false;
		}
        // Criação do Form
		Form = new TFormImportadorUgrid(Owner, apl_owner, NULL, Importador);
		sucesso = Form->ShowModal();
		if (Form->ModalResult == mrCancel)
		{
			form_child = (TForm*)Owner->Owner;
			if(form_child->ClassName().CompareIC("TFormChild") != 0)
			{
                delete Form;
			}
			else
			{
				delete Form;
				form_child->Close();
			}
		}
		else
		{
			delete Form;
		}
	}
	catch (Exception &e)
	{
	}
	return (sucesso);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TImportaBDGD::ShowModalFormImportaRedeGIS_SDBT(void)
{
	return (false);
}

// -------------------------------------------------------------------------------------------------
bool __fastcall TImportaBDGD::ShowModalFormImportaRedeGIS_SDMT(void)
{
	return (false);
}


// -----------------------------------------------------------------------------
// eof
