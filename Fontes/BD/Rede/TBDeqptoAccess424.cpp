// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TBDeqptoAccess424.h"
#include "VTSNPDBConn.h"
#include "TBDfunc.h"
#include "..\Apl\VTApl.h"
#include "..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

#include <locale.h>

#include <cstdarg>

// ---------------------------------------------------------------------------
// nada muda em relação a 423
VTBDeqpto* NewObjBDeqptoAccess424(VTApl *apl_owner, VTSNPDBConn *bd_conn)
{
	return (new TBDeqptoAccess424(apl_owner, bd_conn));
}

// ---------------------------------------------------------------------------
__fastcall TBDeqptoAccess424::TBDeqptoAccess424(VTApl *apl_owner, VTSNPDBConn *bd_conn)
	: TBDeqptoAccess423(apl_owner, bd_conn)
{
	// a classe básica já faz tudo
}

// ---------------------------------------------------------------------------
__fastcall TBDeqptoAccess424::~TBDeqptoAccess424(void)
{
	// nada a fazer
}
// ---------------------------------------------------------------------------
