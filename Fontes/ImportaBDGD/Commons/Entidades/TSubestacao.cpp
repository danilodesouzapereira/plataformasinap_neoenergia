// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <Fontes\Rede\VTRede.h>
#include <TRedeMT.h>
#include <TSubestacao.h>

#include <DLL_Inc\Rede.h>

// -------------------------------------------------------------------------------------------------
TSubestacao* __fastcall NewObjSubestacao(void)
{
	return (new TSubestacao());
}

// -------------------------------------------------------------------------------------------------
__fastcall TSubestacao::TSubestacao(void)
{
    // cria lista
	lisPRIMARIO = new TList();
	// Inicialização de variáveis
	PD.rede = NULL;
	PD.id_subestacao = 0;
	PD.codigo = "";
	PD.nome = "";
	PD.descricao = "";
}

// -------------------------------------------------------------------------------------------------
__fastcall TSubestacao::~TSubestacao(void)
{
	//
	if(lisPRIMARIO)
	{
        delete lisPRIMARIO;
    }
}
// ------------------------------------------------------------------------------------------------
TRedeMT* __fastcall TSubestacao::ExistePrimarioCodigo(AnsiString codigo)
{
	TRedeMT *aux, *redeMT;

	redeMT = NULL;
	//TODO : otimizar a busca usando map
	for (int i = 0; i < lisPRIMARIO->Count; i++)
	{
		aux = (TRedeMT*)lisPRIMARIO->Items[i];
		if (!aux->Codigo.AnsiCompare(codigo))
		{
			redeMT = aux;
			break;
		}
	}

	return redeMT;
}
// ------------------------------------------------------------------------------------------------
TRedeMT* __fastcall TSubestacao::ExistePrimarioNome(AnsiString nome)
{
	TRedeMT *aux, *redeMT;

	redeMT = NULL;
	//TODO : otimizar a busca usando map
	for (int i = 0; i < lisPRIMARIO->Count; i++)
	{
		aux = (TRedeMT*)lisPRIMARIO->Items[i];
		if (!aux->Nome.AnsiCompare(nome))
		{
			redeMT = aux;
			break;
		}
	}

	return redeMT;
}
// -------------------------------------------------------------------------------------------------
bool __fastcall TSubestacao::InserePrimario(TRedeMT *primario)
{
	// proteção
	if (primario == NULL)
		return (false);
	// insere na lista
	if (lisPRIMARIO->IndexOf(primario) < 0)
	{
		lisPRIMARIO->Add(primario);
		// associa com essa Subestacao
		primario->Subestacao = this;
	}
	return (true);
}

// -------------------------------------------------------------------------------------------------
TList* __fastcall TSubestacao::LisPrimario(void)
{
	return (lisPRIMARIO);
}
// -------------------------------------------------------------------------------------------------
void   __fastcall TSubestacao::SelecionaPrimarioCodigo(AnsiString codigo)
{
    TRedeMT *aux;

	aux = ExistePrimarioCodigo(codigo);

	if (aux != NULL)
	{
        aux->Selecionado = true;
	}
}
// -------------------------------------------------------------------------------------------------
void   __fastcall TSubestacao::SelecionaPrimarioNome(AnsiString nome)
{
    TRedeMT *aux;

	aux = ExistePrimarioNome(nome);

	if (aux != NULL)
	{
        aux->Selecionado = true;
	}
}
// -------------------------------------------------------------------------------------------------
void   __fastcall TSubestacao::TodosPrimariosSelecionado(bool selecionado)
{
	TRedeMT *aux;

	//TODO : otimizar a busca usando map
	for (int i = 0; i < lisPRIMARIO->Count; i++)
	{
		aux = (TRedeMT*)lisPRIMARIO->Items[i];
		aux->Selecionado = selecionado;
	}
}
// --------------------------------------------------------------------------------------------- eof
