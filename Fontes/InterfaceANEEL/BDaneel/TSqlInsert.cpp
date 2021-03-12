//---------------------------------------------------------------------------
#pragma hdrstop
#include "TSqlInsert.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TSqlInsert::TSqlInsert(void)
	{
	//nada a fazer
	}
//---------------------------------------------------------------------------
__fastcall TSqlInsert::~TSqlInsert(void)
	{
	//nada a fazer
	}
//---------------------------------------------------------------------------
void __fastcall TSqlInsert::AddField(AnsiString fieldname, AnsiString value)
	{
	//atualiza string dos campos
	if (! fields.IsEmpty()) fields += ", ";
	fields += "[" + fieldname + "]";
	//atualiza string dos valores
	if (! values.IsEmpty()) values += ", ";
	values += AnsiQuotedStr(value, '\'');
	}

//---------------------------------------------------------------------------
void __fastcall TSqlInsert::AddField(AnsiString fieldname, int value)
	{
	//atualiza string dos campos
	if (! fields.IsEmpty()) fields += ", ";
	fields += "[" + fieldname + "]";
	//atualiza string dos valores
	if (! values.IsEmpty()) values += ", ";
	values += IntToStr(value);
	}

//---------------------------------------------------------------------------
void __fastcall TSqlInsert::AddField(AnsiString fieldname, double value)
	{
	//atualiza string dos campos
	if (! fields.IsEmpty()) fields += ", ";
	fields += "[" + fieldname + "]";
	//atualiza string dos valores
	if (! values.IsEmpty()) values += ", ";
	values += DoubleToStr("%6.5f", value);
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TSqlInsert::GetSqlCmd(void)
	{
	//variáveis locais
	AnsiString cmd_sql;

	cmd_sql  = "INSERT INTO " + tablename + " ( " + fields + " ) VALUES ( " + values + " )";

	return(cmd_sql);
	}

//---------------------------------------------------------------------------
void __fastcall TSqlInsert::SetTableName(AnsiString tablename)
	{
	//salva nome da tabela
	this->tablename = tablename;
	//reinicia strings
	fields = "";
	values = "";
   }

//---------------------------------------------------------------------------
//eof

