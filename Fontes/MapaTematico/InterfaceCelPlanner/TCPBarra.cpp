// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Rede\VTBarra.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "TCPBarra.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
// função global p/ criar objeto da classe VTRede
// ---------------------------------------------------------------------------
// VTCPEqpto* __fastcall NewObjCPEqpto(VTApl *apl, VTEqpto *eqpto)
// {
// return(new TCPCarga(apl,eqpto));
// }
// ---------------------------------------------------------------------------
__fastcall TCPBarra::TCPBarra(VTApl *apl, VTBarra *barra) : TCPEqpto(apl)
{ // variaveis locais


	// inicia dados
	// strEQPTO_CP.subscriber = "Sub" + IntToStr(carga->Id);
	strEQPTO_CP.subscriber = barra->Codigo; // FKM 2018.07.19 mudança devida ao gerenciador
	strEQPTO_CP.numero = barra->Id;
	strEQPTO_CP.userData1 = barra->Codigo;
	strEQPTO_CP.userData2 = barra->TipoAsString();
	strEQPTO_CP.userData3 = "BARRA";
	strEQPTO_CP.userData4 = "";
	strEQPTO_CP.userData5 = "";
	strEQPTO_CP.userData6 = "";
	strEQPTO_CP.userData7 = "";
	strEQPTO_CP.userData8 = "";
	strEQPTO_CP.act = 1;
	strEQPTO_CP.fase = 1;
	strEQPTO_CP.area = 1;
	strEQPTO_CP.flag1 = 1; // consumidor?
	strEQPTO_CP.flag2 = 0; // chave?
	strEQPTO_CP.flag3 = 0; // gerador?
	strEQPTO_CP.flag4 = 0; // regulador
	strEQPTO_CP.flag5 = 0; // capacitor
	strEQPTO_CP.flag6 = 0; // subestacao
	strEQPTO_CP.flag7 = 0; // trafo
	strEQPTO_CP.flag8 = 0; //
	strEQPTO_CP.col = 8;
	strEQPTO_CP.coordUTM_x = barra->utm.x / 100;
	strEQPTO_CP.coordUTM_y = barra->utm.y / 100;
}

// ---------------------------------------------------------------------------
__fastcall TCPBarra::~TCPBarra(void)
{
	// nada a fazer
}

// ---------------------------------------------------------------------------
// eof