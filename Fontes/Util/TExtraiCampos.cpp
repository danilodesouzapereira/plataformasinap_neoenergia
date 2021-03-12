// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TExtraiCampos.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TExtraiCampos::TExtraiCampos(void)
{
	try
	{ // cria objetos
		campos = new TStringList();
		// inicia dados
		SeparadorReinicia();
		// desabilita an�lise de coment�rio: texto iniciado por "//"
		SetOpcaoFiltraComentario(false);
		// filtra caracteres ' ' do in�cio e do fim de cada campo extra�do
		SetOpcaoTrimCampos(true);
	}
	catch (Exception &e)
	{
	}
}

// ---------------------------------------------------------------------------
__fastcall TExtraiCampos::~TExtraiCampos(void)
{
	// destr�i objetos
	if (campos)
	{
		delete campos;
		campos = NULL;
	}
}

// -----------------------------------------------------------------------------
TStrings* __fastcall TExtraiCampos::Executa(AnsiString txt)
{
	// vari�veis locais
	AnsiString campo;
	int ind_ini, ind_fim;
	int len = txt.Length();

	// reinicia StrinList campos
	campos->Clear();
	// prote��o
	if (len == 0)
		return (campos);
	// verifica se deve filtrar texto de comentario
	if (opcao.filtra_comentario)
	{ // verifica se � coment�rio
		if ((len >= 2) && (txt[1] == '/') && (txt[2] == '/'))
			return (campos);
	}
	// loop de identifica��o dos campos
	ind_ini = 1;
	while (ind_ini <= len)
	{ // determina fim de um campo
		ind_fim = ind_ini;
		while (ind_fim <= len)
		{ // procura por delimitador
			if (IsSeparador(txt[ind_fim]))
				break;
			// incrementa ind_fim
			ind_fim++;
		}
		// encontrou delimitador ou fim do string
		if (ind_fim == ind_ini)
		{ // campo vazio
			campos->Add("");
		}
		else if (ind_fim > ind_ini)
		{ // extrai o campo o campo
			campo = txt.SubString(ind_ini, ind_fim - ind_ini);
			// verifica se deve eliminar brancos
			if (opcao.trim_campo)
				campo = campo.Trim();
			// insere campo
			campos->Add(campo);
		}
		// atualiza ind_ini
		ind_ini = ind_fim + 1;
	}
	/*
	 for (int n = 0; n < campos->Count; n++)
	 {
	 txt = campos->Strings[n];
	 int a=0;
	 }
	 */
	return (campos);
}

// -----------------------------------------------------------------------------
bool __fastcall TExtraiCampos::IsSeparador(char car)
{
	// vari�ves locais
	int len = opcao.separadores.Length();

	// verifica se o caractere � igual a um dos scaracteeres spearadores definidos
	for (int n = 1; n <= len; n++)
	{
		if (opcao.separadores[n] == car)
			return (true);
	}
	return (false);
}

// -----------------------------------------------------------------------------
void __fastcall TExtraiCampos::SeparadorInsere(char car)
{
	opcao.separadores += car;
}

// -----------------------------------------------------------------------------
void __fastcall TExtraiCampos::SeparadorInsere(AnsiString separadores)
{
	opcao.separadores += separadores;
}

// -----------------------------------------------------------------------------
void __fastcall TExtraiCampos::SeparadorReinicia(void)
{
	opcao.separadores = "";
}

// -----------------------------------------------------------------------------
void __fastcall TExtraiCampos::SetOpcaoFiltraComentario(bool enabled)
{
	opcao.filtra_comentario = enabled;
}

// -----------------------------------------------------------------------------
void __fastcall TExtraiCampos::SetOpcaoTrimCampos(bool enabled)
{
	opcao.trim_campo = enabled;
}

// -----------------------------------------------------------------------------
// eof
