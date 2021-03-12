//---------------------------------------------------------------------------

#pragma hdrstop

#include "ESQLite.h"
#include "SQLite.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
////////////////////////////////////////////////////////////////////////////////
ESQLite::ESQLite(AnsiString msg)  : Exception(msg)
{

}

__fastcall ESQLite::~ESQLite(void)
{

}

//---------------------------------------------------------------------------
ESQLite::ESQLite(const int nErrCode, AnsiString szErrMess, bool bDeleteMsg) : Exception(szErrMess,nErrCode)
{
	PD.errorCode = nErrCode;
}

//
const char* ESQLite::errorCodeAsString(int nErrCode)
{
	switch (nErrCode)
	{
	case SQLITE_OK:
		return "SQLITE_OK";
	case SQLITE_ERROR:
		return "SQLITE_ERROR";
	case SQLITE_INTERNAL:
		return "SQLITE_INTERNAL";
	case SQLITE_PERM:
		return "SQLITE_PERM";
	case SQLITE_ABORT:
		return "SQLITE_ABORT";
	case SQLITE_BUSY:
		return "SQLITE_BUSY";
	case SQLITE_LOCKED:
		return "SQLITE_LOCKED";
	case SQLITE_NOMEM:
		return "SQLITE_NOMEM";
	case SQLITE_READONLY:
		return "SQLITE_READONLY";
	case SQLITE_INTERRUPT:
		return "SQLITE_INTERRUPT";
	case SQLITE_IOERR:
		return "SQLITE_IOERR";
	case SQLITE_CORRUPT:
		return "SQLITE_CORRUPT";
	case SQLITE_NOTFOUND:
		return "SQLITE_NOTFOUND";
	case SQLITE_FULL:
		return "SQLITE_FULL";
	case SQLITE_CANTOPEN:
		return "SQLITE_CANTOPEN";
	case SQLITE_PROTOCOL:
		return "SQLITE_PROTOCOL";
	case SQLITE_EMPTY:
		return "SQLITE_EMPTY";
	case SQLITE_SCHEMA:
		return "SQLITE_SCHEMA";
	case SQLITE_TOOBIG:
		return "SQLITE_TOOBIG";
	case SQLITE_CONSTRAINT:
		return "SQLITE_CONSTRAINT";
	case SQLITE_MISMATCH:
		return "SQLITE_MISMATCH";
	case SQLITE_MISUSE:
		return "SQLITE_MISUSE";
	case SQLITE_NOLFS:
		return "SQLITE_NOLFS";
	case SQLITE_AUTH:
		return "SQLITE_AUTH";
	case SQLITE_FORMAT:
		return "SQLITE_FORMAT";
	case SQLITE_RANGE:
		return "SQLITE_RANGE";
	case SQLITE_ROW:
		return "SQLITE_ROW";
	case SQLITE_DONE:
		return "SQLITE_DONE";
		// case SQLITE_ERROR:
		// return "SQLITE_ERROR";
	default:
		return "UNKNOWN_ERROR";
	}
}
//
//TSQLiteException::~TSQLiteException()
//{
//
//	if (mpszErrMess)
//	{
//		sqlite3_free(mpszErrMess);
//		mpszErrMess = 0;
//	}
//}

////////////////////////////////////////////////////////////////////////////////
