//---------------------------------------------------------------------------

#ifndef ESQLiteH
#define ESQLiteH
// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>
// ------------------------------------------------------------------------------------------- class
class ESQLite  : public Sysutils::Exception
{
public:
	ESQLite(AnsiString msg);
	__fastcall ~ESQLite(void);
	ESQLite(const int nErrCode, AnsiString szErrMess, bool bDeleteMsg = true);
	const char* errorCodeAsString(int nErrCode);

protected:
	__property int ErrorCode = {read=PD.errorCode};

private:
	struct
	{
		int errorCode;
	}PD;
};
//---------------------------------------------------------------------------
#endif
