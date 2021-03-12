// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include <TLicencaRequest.h>

// ------------------------------------------------------------------------------------------- class

// ------------------------------------------------------------------------------------- constructor
__fastcall TLicencaRequest::TLicencaRequest(void)
{
	// nada
}

// ------------------------------------------------------------------------------------------------
__fastcall TLicencaRequest::~TLicencaRequest(void)
{
	// nada
}

// ----------------------------------------------------------------------------------------- methods
AnsiString TLicencaRequest::ToJSON(void)
{
	//variaveis locais
	AnsiString parametros;

	// insere parametros no string formato JSON
	parametros = ("{\"dominio\":" + AnsiString(dominio) + "," + "\"login\" : \"" + login + "\"," +
		"\"senha\" : \"" + senha + "\"," + "\"produto\" : \"" + produto + "\"," + "\"cpuId\" : \"" +
		cpuId + "\"}");
	return parametros;
}

// ------------------------------------------------------------------------------- getters & setters
int TLicencaRequest::GetDominio(void)
{
	return dominio;
}

// ------------------------------------------------------------------------------------------------
void TLicencaRequest::SetDominio(int dominio)
{
	this->dominio = dominio;
}

// ------------------------------------------------------------------------------------------------
AnsiString TLicencaRequest::GetLogin(void)
{
	return login;
}

// ------------------------------------------------------------------------------------------------
void TLicencaRequest::SetLogin(AnsiString login)
{
	this->login = login;
}

// ------------------------------------------------------------------------------------------------
AnsiString TLicencaRequest::GetSenha(void)
{
	return senha;
}

// ------------------------------------------------------------------------------------------------
void TLicencaRequest::SetSenha(AnsiString senha)
{
	this->senha = senha;
}

// ------------------------------------------------------------------------------------------------
AnsiString TLicencaRequest::GetProduto(void)
{
	return produto;
}

// ------------------------------------------------------------------------------------------------
void TLicencaRequest::SetProduto(AnsiString produto)
{
	this->produto = produto;
}

// ------------------------------------------------------------------------------------------------
AnsiString TLicencaRequest::GetCPUid(void)
{
	return cpuId;
}

// ------------------------------------------------------------------------------------------------
void TLicencaRequest::SetCPUid(AnsiString cpuId)
{
	this->cpuId = cpuId;
}

// --------------------------------------------------------------------------------------------- eof
