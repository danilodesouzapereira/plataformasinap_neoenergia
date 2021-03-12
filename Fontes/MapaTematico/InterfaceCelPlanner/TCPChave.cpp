// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "..\Apl\VTApl.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTTipoChave.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "TCPChave.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// função global p/ criar objeto da classe VTRede
// ---------------------------------------------------------------------------
// VTCPEqpto* __fastcall NewObjCPEqpto(VTApl *apl, VTEqpto *eqpto)
// {
// return(new TCPChave(apl, eqpto));
// }

// ---------------------------------------------------------------------------
__fastcall TCPChave::TCPChave(VTApl *apl, VTChave *chave) : TCPEqpto(apl)
{ // variaveis locais
	VTBarra *barra1;
	VTBarra *barra2;
	double x = 0.0;
	double y = 0.0;

	this->apl = apl;
	barra1 = chave->pbarra1;
	barra2 = chave->pbarra2;

	// inicia dados
	// strEQPTO_CP.subscriber = "Chv" + IntToStr(chave->Id);
	strEQPTO_CP.subscriber = chave->Codigo; //FKM 2018.07.19 mudança devida ao gerenciador
	strEQPTO_CP.numero = chave->Id;
	strEQPTO_CP.userData1 = chave->Codigo;
	strEQPTO_CP.userData2 = chave->TipoAsString();
	strEQPTO_CP.userData3 = chave->TipoChave->Codigo;
	strEQPTO_CP.userData4 = "";
	strEQPTO_CP.userData5 = "";
	strEQPTO_CP.userData6 = "";
	strEQPTO_CP.userData7 = "";
	strEQPTO_CP.userData8 = "";
	strEQPTO_CP.act = 1;
	strEQPTO_CP.fase = 1;
	strEQPTO_CP.area = 1;
	strEQPTO_CP.flag1 = 0; // consumidor?
	strEQPTO_CP.flag2 = 1; // chave?
	strEQPTO_CP.flag3 = 0; // gerador?
	strEQPTO_CP.flag4 = 0; // MT?
	strEQPTO_CP.flag5 = 0; // Comercial?
	strEQPTO_CP.flag6 = 0; // Industrial?
	strEQPTO_CP.flag7 = 0; //
	strEQPTO_CP.flag8 = 0; //
	strEQPTO_CP.col = 8;

	// calcula uma coordenada media
	if ((barra1 != NULL) && (barra2 != NULL))
	{
		x = (double)(barra1->utm.x + barra2->utm.x) / 200;
		y = (double)(barra1->utm.y + barra2->utm.y) / 200;
	}
	strEQPTO_CP.coordUTM_x = x;
	strEQPTO_CP.coordUTM_y = y;
}

// ---------------------------------------------------------------------------
__fastcall TCPChave::~TCPChave(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
// eof
