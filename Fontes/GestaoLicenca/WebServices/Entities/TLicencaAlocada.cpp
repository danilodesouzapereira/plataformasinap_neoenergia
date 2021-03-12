// --------------------------------------------------------------------------------------------- bof
#include <vcl.h>
#pragma hdrstop
#pragma package(smart_init)

// ---------------------------------------------------------------------------------------- includes
#include "TLicencaAlocada.h"

// ------------------------------------------------------------------------------------------- class

// ------------------------------------------------------------------------------------- constructor
__fastcall TLicencaAlocada::TLicencaAlocada(void)
{
	//nada
}

// ------------------------------------------------------------------------------------------------
__fastcall TLicencaAlocada::~TLicencaAlocada(void)
{
	//nada
}

// ------------------------------------------------------------------------------- getters & setters
TDate TLicencaAlocada::GetDataAlocada()
{
	return dataAlocada;
}

// ------------------------------------------------------------------------------------------------
void TLicencaAlocada::SetDataAlocada(TDate dataAlocada)
{
	this->dataAlocada = dataAlocada;
}

// ------------------------------------------------------------------------------------------------
TDate TLicencaAlocada::GetDataTermino()
{
	return dataTermino;
}

// ------------------------------------------------------------------------------------------------
void TLicencaAlocada::SetDataTermino(TDate dataTermino)
{
	this->dataTermino = dataTermino;
}

// ------------------------------------------------------------------------------------------------
int TLicencaAlocada::GetValidade()
{
	return validade;
}

// ------------------------------------------------------------------------------------------------
void TLicencaAlocada::SetValidade(int validade)
{
	this->validade = validade;
}

// ------------------------------------------------------------------------------------------------
int TLicencaAlocada::GetTipo()
{
	return tipo;
}

// ------------------------------------------------------------------------------------------------
void TLicencaAlocada::SetTipo(int tipo)

{
	this->tipo = tipo;
}

// ------------------------------------------------------------------------------------------------
DynamicArray<int> TLicencaAlocada::GetOperacoes()
{
	return operacoes;
}

// ------------------------------------------------------------------------------------------------
void TLicencaAlocada::SetOperacoes(DynamicArray<int> operacoes)
{
	this->operacoes = operacoes;
}

// --------------------------------------------------------------------------------------------- eof
