// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include "TRetornoLiberacaoLicenca.h"

// ------------------------------------------------------------------------------------------- class
//using namespace sinapsis::sinap::gestaoLicenca;

// ------------------------------------------------------------------------------------- constructor
__fastcall TRetornoLiberacaoLicenca::TRetornoLiberacaoLicenca()
{

}

// ------------------------------------------------------------------------------------------------
__fastcall TRetornoLiberacaoLicenca::~TRetornoLiberacaoLicenca(void)
{

}

// ------------------------------------------------------------------------------- getters & setters
bool TRetornoLiberacaoLicenca::GetSucesso()
{
	return sucesso;
}

// ------------------------------------------------------------------------------------------------
void TRetornoLiberacaoLicenca::SetSucesso(bool sucesso)
{
	this->sucesso = sucesso;
}

// ------------------------------------------------------------------------------------------------
AnsiString TRetornoLiberacaoLicenca::GetMensagem()
{
	return mensagem;
}

// ------------------------------------------------------------------------------------------------
void TRetornoLiberacaoLicenca::SetMensagem(AnsiString mensagem)
{
	this->mensagem = mensagem;
}

// --------------------------------------------------------------------------------------------- eof
