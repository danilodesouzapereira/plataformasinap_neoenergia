// ---------------------------------------------------------------------------
#include<stdexcept>

#include <vcl.h>

#pragma hdrstop
#include "TAplCfg.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Diretorio\VTPath.h"

#include "..\..\Geral\VTGeral.h"
#include "..\BD\VTBD.h"
#include "..\BD\Rede\VTBDgeral.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurvas.h"
#include "..\MontaZ\VTMontaZ.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTTipos.h"
#include "..\..\DLL_Inc\Arranjo.h"
#include "..\..\DLL_Inc\BD.h"
#include "..\..\DLL_Inc\Classes.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\MontaZ.h"
#include "..\..\DLL_Inc\Rede.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace std;

// ---------------------------------------------------------------------------
__fastcall TAplCfg::TAplCfg(TForm *form_owner, VTApl *apl_owner) : TApl(form_owner, apl_owner)
{
	// OBS: apl_owner já possui objeto Arranjos e Tipos iniciado
	try
	{ // cria objetos
		Add(DLL_NewObjFases());
		Add(DLL_NewObjClasses());
		Add(DLL_NewObjCurvas(this));
		// Add(DLL_NewObjTipos()); //DVK 2015.10.01
		Add(DLL_NewObjMontaZ(this));
		Add(DLL_NewObjBD(this));
	}
	catch (Exception &e)
	{
	}
}

// ---------------------------------------------------------------------------
__fastcall TAplCfg::~TAplCfg(void)
{
	// nada a fazer: a classe base TApl destrói todos os obejtos criados
}

// ---------------------------------------------------------------------------
bool __fastcall TAplCfg::ExisteCurva(TList *lisCURVA, VTCurva *curva)
{
	int a = 0;
	a = fabs((long double)lisCURVA->IndexOf(curva));
	return (a >= 0);
}

// ---------------------------------------------------------------------------
void __fastcall TAplCfg::LeBaseArranjo(void)
{
	// variáveis locais
	VTBD *bd = (VTBD*)GetObject(__classid(VTBD));

	try
	{ // lê dados da base
		if (!bd->Conectado)
			bd->Conecta();
		bd->Geral->LeArranjo();
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAplCfg::LeBaseCurvaTipica(void)
{
	// variáveis locais
	VTPath *path = (VTPath*)GetObject(__classid(VTPath));
	VTBD *bd = (VTBD*)GetObject(__classid(VTBD));
	AnsiString redetemp = path->ArqRede;

	bd->Desconecta();
//
//	path->ArqRede = path->ArqRedeEmpresa();

	try
	{ // lê dados da base
		// if (!bd->Conectado)
		bd->Conecta();
		bd->Geral->LeTipoConsumidor();
		bd->Geral->LeCurvaTipica();
	}
	catch (Exception &e)
	{ // nada a fazer
	}

//	bd->Desconecta();
//	path->ArqRede = redetemp;
}

// ---------------------------------------------------------------------------
void __fastcall TAplCfg::LeBaseTipoChave(void)
{
	// variáveis locais
	VTBD *bd = (VTBD*)GetObject(__classid(VTBD));

	try
	{ // lê dados da base
		if (!bd->Conectado)
			bd->Conecta();
		bd->Geral->LeTipoChave();
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAplCfg::LeBaseTipoRede(void)
{
	// variáveis locais
	VTBD *bd = (VTBD*)GetObject(__classid(VTBD));

	try
	{ // lê dados da base
		if (!bd->Conectado)
			bd->Conecta();
		bd->Geral->LeTipoRede();
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAplCfg::SalvaBaseArranjo(void)
{
	// variáveis locais
	VTBD *bd = (VTBD*)GetObject(__classid(VTBD));

	try
	{ // atualiza base de arranjos
		if (!bd->Conectado)
			bd->Conecta();
		bd->Geral->SalvaArranjo();
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAplCfg::SalvaBaseCurvaTipica(VTCurva *curva)
{
	// variáveis locais
	VTPath *path = (VTPath*)GetObject(__classid(VTPath));
	VTBD *bd = (VTBD*)GetObject(__classid(VTBD));
	VTCurvas *curvas = (VTCurvas*)GetObject(__classid(VTCurvas));
	AnsiString redetemp = path->ArqRede;

	bd->Desconecta();

	try
	{ // conecta com a base
//		if (!bd->Conectado)
		bd->Conecta();
		// TList* v = curvas->ListaCurvaTipicaCarga();
		// verifica se é uma curva de carga
		if (ExisteCurva(curvas->ListaCurvaTipicaCarga(), curva))
		{
			bd->Geral->SalvaCurvaTipica(eqptoCARGA, curva);
		}
		// verifica se é uma curva de IP
		else if (ExisteCurva(curvas->ListaCurvaTipicaIP(), curva))
		{
			bd->Geral->SalvaCurvaTipica(eqptoCARGA, curva);
		}
		// verifica se é uma curva de capacitor
		else if (ExisteCurva(curvas->ListaCurvaTipicaCapacitor(), curva))
		{
			bd->Geral->SalvaCurvaTipica(eqptoCAPACITOR, curva);
		}
		// verifica se é uma curva de reator
		else if (ExisteCurva(curvas->ListaCurvaTipicaReator(), curva))
		{
			bd->Geral->SalvaCurvaTipica(eqptoREATOR, curva);
		}
		// verifica se é uma curva de gerador
		else if (ExisteCurva(curvas->ListaCurvaTipicaGeradorFotovoltaico(), curva))
		{
			bd->Geral->SalvaCurvaTipica(eqptoGERADOR, curva);
		}
	}
	catch (Exception &e)
	{ // nada a fazer
	}

  //	bd->Desconecta();
	path->ArqRede = redetemp;
}


// ---------------------------------------------------------------------------
void __fastcall TAplCfg::SalvaBaseTipoChave(void)
{
	// variáveis locais
	VTBD *bd = (VTBD*)GetObject(__classid(VTBD));

	try
	{ // conecta com a base
		if (!bd->Conectado)
			bd->Conecta();
		// salva TipoRede na base de dados
		bd->Geral->SalvaTipoChave();
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAplCfg::SalvaBaseTipoRede(void)
{
	// variáveis locais
	VTBD *bd = (VTBD*)GetObject(__classid(VTBD));

	try
	{ // conecta com a base
		if (!bd->Conectado)
			bd->Conecta();
		// salva TipoRede na base de dados
		bd->Geral->SalvaTipoRede();
	}
	catch (Exception &e)
	{ // nada a fazer
	}
}
// ---------------------------------------------------------------------------
TList* __fastcall TAplCfg::GetCurvasTipicasEmpresa() throw(runtime_error)
{
	VTPath *path = (VTPath*)GetObject(__classid(VTPath));
	VTBD * bd = (VTBD*)GetObject(__classid(VTBD));
	VTGeral * geral = (VTGeral*)GetObject(__classid(VTGeral));
	VTCurvas *curvas = (VTCurvas*)GetObject(__classid(VTCurvas));

	// guarda o arquivo de rede(.mdb) corrente
	AnsiString redeUsuario = path->ArqRede;

	// define uso da base Access
	geral->BaseDado = SNPDBTYPE_ACCESS;

	//
	// 1. desconecta da base atual
	//
	bd->Desconecta();

	//
	// 2. limpa as curvas
	//
	curvas->Clear();

	//
	// 3. coloca o RedeEmpresa.mdb como o arquivo de rede atual
	//
	path->ArqRede = path->ArqRedeEmpresa();
	bd->Conecta();
	LeBaseCurvaTipica();

	//
	// 4. Monta a lista de curvas(demanda de consumidor + Iluminacao publica)
	//
	TList *retonoList = new TList();
	retonoList->Add(curvas->ListaCurvaTipicaCarga());
	retonoList->Add(curvas->ListaCurvaTipicaIP());

	//
	// 5. volta a base corrente para a base(rede) que o usuario estava utilizando
	//
	bd->Desconecta();
	path->ArqRede = redeUsuario;
	bd->Conecta();
	LeBaseCurvaTipica();

	return retonoList;

}

// ---------------------------------------------------------------------------
// eof
