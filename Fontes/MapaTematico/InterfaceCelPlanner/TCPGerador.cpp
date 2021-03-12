// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "..\Apl\VTApl.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTGerador.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "TCPGerador.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função global p/ criar objeto da classe VTRede
// ---------------------------------------------------------------------------
// VTCPEqpto* __fastcall NewObjCPEqpto(VTApl *apl, VTEqpto *eqpto)
// {
// return(new TCPGerador(apl,eqpto));
// }
// ---------------------------------------------------------------------------
__fastcall TCPGerador::TCPGerador(VTApl *apl, VTGerador *gerador) : TCPEqpto(apl)
{ // variaveis locais
	VTBarra *barra;

	barra = gerador->pbarra;
	// inicia dados
	// strEQPTO_CP.subscriber = "GD" + IntToStr(gerador->Id);
	strEQPTO_CP.subscriber = gerador->Codigo; // FKM 2018.07.19 mudança devida ao gerenciador
	strEQPTO_CP.numero = gerador->Id;
	strEQPTO_CP.userData1 = gerador->Codigo;
	strEQPTO_CP.userData2 = gerador->TipoAsString();
	strEQPTO_CP.userData3 = gerador->FonteStr;
	strEQPTO_CP.userData4 = DoubleToStr("%2.1f", gerador->snom) + " MVA";
	strEQPTO_CP.userData5 = "";
	strEQPTO_CP.userData6 = "";
	strEQPTO_CP.userData7 = "";
	strEQPTO_CP.userData8 = "";
	strEQPTO_CP.act = 1;
	strEQPTO_CP.fase = 1;
	strEQPTO_CP.area = 1;
	strEQPTO_CP.flag1 = 0; // consumidor?
	strEQPTO_CP.flag2 = 0; // chave?
	strEQPTO_CP.flag3 = 1; // gerador?
	strEQPTO_CP.flag4 = 0; // MT?
	strEQPTO_CP.flag5 = 0; // Comercial?
	strEQPTO_CP.flag6 = 0; // Industrial?
	strEQPTO_CP.flag7 = 0; //
	strEQPTO_CP.flag8 = 0; //
	strEQPTO_CP.col = 8;
	strEQPTO_CP.coordUTM_x = barra->utm.x / 100;
	strEQPTO_CP.coordUTM_y = barra->utm.y / 100;
}

// ---------------------------------------------------------------------------
__fastcall TCPGerador::~TCPGerador(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
// eof
