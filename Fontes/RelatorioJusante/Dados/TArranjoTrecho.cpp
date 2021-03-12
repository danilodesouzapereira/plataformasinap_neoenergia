//---------------------------------------------------------------------------

#pragma hdrstop

#include "TArranjoTrecho.h"
#include <Fontes\Apl\VTApl.h>
#include "..\Arranjo\VTArranjo.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTRede.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TArranjoTrecho::TArranjoTrecho(VTApl *apl, VTTrecho *trecho)
{
	this->apl = apl;
	this->comprimentoKm = 0;

	//armazena o arranjo do trecho
	arranjo = trecho->arranjo;
	// armazena o segmento que o trecho pertence
	segmento = trecho->rede->TipoRede->Segmento;
	//adciona o nível de tensão ao arranjo
	CodigoSegmento(trecho);


}

//---------------------------------------------------------------------------
__fastcall TArranjoTrecho::~TArranjoTrecho(void)
{

}

//---------------------------------------------------------------------------
void __fastcall TArranjoTrecho::CodigoSegmento(VTTrecho *trecho)
{
	AnsiString nivelTensao;

	//verifica qual o segmento e armazena qual o seu nível de tensão
	switch (segmento)
	{
	case redeETT:
	case redeSUB:
	case redeTRANS:
		nivelTensao = "AT ";
		break;

	case redeETD:
	case redePRI:
		nivelTensao = "MT ";
		break;

	case redeSEC:
		nivelTensao = "BT ";
		break;

	default:
		nivelTensao = "MT ";
		break;
		;
	}
	//armazena em codigoArranjo o nível de tensão e o arranjo
	codigoArranjo = nivelTensao + trecho->arranjo->Codigo;
}

//---------------------------------------------------------------------------
AnsiString __fastcall TArranjoTrecho::GetCodigoArranjo()
{
    return codigoArranjo;
}

//---------------------------------------------------------------------------
double __fastcall TArranjoTrecho::GetComprimentoKm()
{
	return comprimentoKm;
}

//---------------------------------------------------------------------------
void __fastcall TArranjoTrecho::SetCodigoArranjo(AnsiString codigoArranjo)
{
	this->codigoArranjo = codigoArranjo;
}

//---------------------------------------------------------------------------
void __fastcall TArranjoTrecho::SetComprimentoKm(double comprimentoKm)
{
	this->comprimentoKm = comprimentoKm;
}

//---------------------------------------------------------------------------
//eof
