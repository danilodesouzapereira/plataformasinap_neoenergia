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
// nada muda em rela��o a 424 (que nada muda em relacao � 423)
// a mudan�a de base s�o as procedures com UPDATE, devido � atualiza��o
// do Office : https://support.office.com/en-us/article/access-error-query-is-corrupt-fad205a5-9fd4-49f1-be83-f21636caedec
VTBDeqpto* NewObjBDeqptoAccess425(VTApl *apl_owner, VTSNPDBConn *bd_conn)
{
	return (new TBDeqptoAccess425(apl_owner, bd_conn));
}

// ---------------------------------------------------------------------------
__fastcall TBDeqptoAccess425::TBDeqptoAccess425(VTApl *apl_owner, VTSNPDBConn *bd_conn)
	: TBDeqptoAccess424(apl_owner, bd_conn)
{
	// a classe b�sica j� faz tudo
}

// ---------------------------------------------------------------------------
__fastcall TBDeqptoAccess425::~TBDeqptoAccess425(void)
{
	// nada a fazer
}
// ---------------------------------------------------------------------------
