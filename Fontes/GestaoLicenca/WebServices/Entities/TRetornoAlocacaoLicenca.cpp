// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include "TRetornoAlocacaoLicenca.h"
#include "TLicencaAlocada.h"
#include <DLL_Inc\Funcao.h>

// ------------------------------------------------------------------------------------------- class

// ------------------------------------------------------------------------------------- constructor
__fastcall TRetornoAlocacaoLicenca::TRetornoAlocacaoLicenca(void)
{
	//cria objeto
	licencaAlocada = new TLicencaAlocada();
}

// ------------------------------------------------------------------------------------------------
__fastcall TRetornoAlocacaoLicenca::~TRetornoAlocacaoLicenca(void)
{
	//destroi objeto
	DestroiTObject(licencaAlocada);
}

// ------------------------------------------------------------------------------- getters & setters
bool TRetornoAlocacaoLicenca::GetSucesso()
{
	return sucesso;
}

// ------------------------------------------------------------------------------------------------
void TRetornoAlocacaoLicenca::SetSucesso(bool sucesso)
{
	this->sucesso = sucesso;
}

// ------------------------------------------------------------------------------------------------
AnsiString TRetornoAlocacaoLicenca::GetMensagem()
{
	return mensagem;
}

// ------------------------------------------------------------------------------------------------
void TRetornoAlocacaoLicenca::SetMensagem(AnsiString mensagem)
{
	this->mensagem = mensagem;
}

// ------------------------------------------------------------------------------------------------
TLicencaAlocada* TRetornoAlocacaoLicenca::GetLicencaAlocada()
{
	return licencaAlocada;
}

// ------------------------------------------------------------------------------------------------
void TRetornoAlocacaoLicenca::SetLicencaAlocada(TLicencaAlocada* licencaAlocada)
{
	this->licencaAlocada = licencaAlocada;
}

// --------------------------------------------------------------------------------------------- eof
