// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TBDeqptoAccess425.h"
#include "VTSNPDBConn.h"
#include "TBDfunc.h"
#include "..\Apl\VTApl.h"
#include "..\..\DLL_Inc\Funcao.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

#include <locale.h>

#include <cstdarg>

// ---------------------------------------------------------------------------
// nada muda em relação a 424 (que nada muda em relacao à 423)
// a mudança de base são as procedures com UPDATE, devido à atualização
// do Office : https://support.office.com/en-us/article/access-error-query-is-corrupt-fad205a5-9fd4-49f1-be83-f21636caedec
VTBDeqpto* NewObjBDeqptoAccess425(VTApl *apl_owner, VTSNPDBConn *bd_conn)
{
	return (new TBDeqptoAccess425(apl_owner, bd_conn));
}

// ---------------------------------------------------------------------------
__fastcall TBDeqptoAccess425::TBDeqptoAccess425(VTApl *apl_owner, VTSNPDBConn *bd_conn)
	: TBDeqptoAccess424(apl_owner, bd_conn)
{
	// a classe básica já faz tudo
}

// ---------------------------------------------------------------------------
__fastcall TBDeqptoAccess425::~TBDeqptoAccess425(void)
{
	// nada a fazer
}
// ---------------------------------------------------------------------------
