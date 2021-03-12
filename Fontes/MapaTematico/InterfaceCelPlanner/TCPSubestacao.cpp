// ---------------------------------------------------------------------------

#pragma hdrstop
#include <vcl.h>
#include "TCPSubestacao.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRede.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "TCPCarga.h"
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
__fastcall TCPSubestacao::TCPSubestacao(VTApl *apl, VTRede *rede) : TCPEqpto(apl)
{ // variaveis locais
	VTBarra *barra;

	barra = rede->BarraInicial();
	if (barra == NULL)
		{
		return;
		}
	// inicia dados
	// strEQPTO_CP.subscriber = "SED" + IntToStr(barra->Id);
	strEQPTO_CP.subscriber = rede->Codigo; // FKM 2018.07.19 mudança devida ao gerenciador
	strEQPTO_CP.numero = rede->Id;
	strEQPTO_CP.userData1 = rede->Codigo;
	strEQPTO_CP.userData2 = "Subestação";
	strEQPTO_CP.userData3 = "";
	strEQPTO_CP.userData4 = "";
	strEQPTO_CP.userData5 = "";
	strEQPTO_CP.userData6 = "";
	strEQPTO_CP.userData7 = "";
	strEQPTO_CP.userData8 = "";
	strEQPTO_CP.act = 1;
	strEQPTO_CP.fase = 1;
	strEQPTO_CP.area = 1;
	strEQPTO_CP.flag1 = 0; // consumidor?
	strEQPTO_CP.flag2 = 0; // chave?
	strEQPTO_CP.flag3 = 0; // gerador?
	strEQPTO_CP.flag4 = 0; // regulador
	strEQPTO_CP.flag5 = 0; // capacitor
	strEQPTO_CP.flag6 = 1; // subestacao
	strEQPTO_CP.flag7 = 0; // trafo
	strEQPTO_CP.flag8 = 0; //
	strEQPTO_CP.col = 8;
	strEQPTO_CP.coordUTM_x = barra->utm.x / 100;
	strEQPTO_CP.coordUTM_y = barra->utm.y / 100;
}

// ---------------------------------------------------------------------------
__fastcall TCPSubestacao::~TCPSubestacao(void)
{
	// nada a fazer
}
// ---------------------------------------------------------------------------
// eof
