//---------------------------------------------------------------------------
#ifndef TSqlInsertH
#define TSqlInsertH

//---------------------------------------------------------------------------
#include <Classes.hpp>

class TSqlInsert
	{
	public:
				  __fastcall  TSqlInsert(void);
				  __fastcall ~TSqlInsert(void);
	void       __fastcall  AddField(AnsiString fieldname, AnsiString value);
	void       __fastcall  AddField(AnsiString fieldname, int        value);
	void       __fastcall  AddField(AnsiString fieldname, double     value);
	AnsiString __fastcall  GetSqlCmd(void);
	void       __fastcall  SetTableName(AnsiString tablename);

private:
	AnsiString tablename;
	AnsiString fields;
	AnsiString values;
	};

//---------------------------------------------------------------------------
#endif
//eof

