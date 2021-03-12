//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include <algorithm>
#include "TEstoque.h"
#include "..\Rede\VTRegulador.h"
#include "..\..\DLL_Inc\Rede.h"
#pragma package(smart_init)
using namespace std;

//---------------------------------------------------------------------------
__fastcall TEstoque::TEstoque(void)
   { //DVK 2015.07.21
	//nada
   }

//---------------------------------------------------------------------------
__fastcall TEstoque::~TEstoque(void)
	{
	//destroi regulador
	if(PD.reg) {DLL_DeleteEqpto(PD.reg); PD.reg = NULL;}
	}

//---------------------------------------------------------------------------
//eof

