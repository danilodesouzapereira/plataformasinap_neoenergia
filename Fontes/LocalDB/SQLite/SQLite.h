////////////////////////////////////////////////////////////////////////////////
// SQLite - A C++ wrapper around the SQLite3 embedded database library.
//
// Copyright (c) 2004..2007 Rob Groves. All Rights Reserved. rob.groves@btinternet.com
//
// Permission to use, copy, modify, and distribute this software and its
// documentation for any purpose, without fee, and without a written
// agreement, is hereby granted, provided that the above copyright notice,
// this paragraph and the following two paragraphs appear in all copies,
// modifications, and distributions.
//
// IN NO EVENT SHALL THE AUTHOR BE LIABLE TO ANY PARTY FOR DIRECT,
// INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
// PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION,
// EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// THE AUTHOR SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF
// ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". THE AUTHOR HAS NO OBLIGATION
// TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
//
// V3.0		03/08/2004	-Initial Version for sqlite3
//
// V3.1		16/09/2004	-Implemented getXXXXField using sqlite3 functions
// -Added SQLITEDB3::tableExists()
//
// V3.2		01/07/2005	-Fixed execScalar to handle a NULL result
// 12/07/2007	-Added SQLITEDB::IsAutoCommitOn()
// -Added int64 functions to SQLiteQuery
// -Added Name based parameter binding to SQLiteStatement.
////////////////////////////////////////////////////////////////////////////////
#ifndef SQLiteH
#define SQLiteH

#include "sqlite3.h"
#include "VTSQLite.h"
#include "ESQLite.h"
#include <cstdio>
#include <cstring>
// #include <System.h>

// sclass UnicodeString;

// #define SQLITE_ERROR 1000

// --------------------------------------------------------------------------------------- namespace
/* namespace sinapsis
 {
 namespace sinap
 {
 namespace neoenergia
 {
 namespace sqlite
 {
 class SQLite;
 }
 }
 }
 } */

// using namespace std;

// using namespace sinapsis::sinap::neoenergia::sqlite::SQLite;

// ------------------------------------------------------------------------------------------- class
//class SQLiteException
//{
//public:
//
//	SQLiteException(const int nErrCode, char* szErrMess, bool bDeleteMsg = true);
//
//	SQLiteException(const SQLiteException& e);
//
//	virtual ~SQLiteException();
//
//	const int errorCode()
//	{
//		return mnErrCode;
//	}
//
//	const char* errorMessage()
//	{
//		return mpszErrMess;
//	}
//
//	static const char* errorCodeAsString(int nErrCode);
//
//private:
//
//	int mnErrCode;
//	char* mpszErrMess;
//};

class SQLiteBuffer
{
public:

	SQLiteBuffer();

	~SQLiteBuffer();

	const char* format(const char* szFormat, ...);

	operator const char*()
	{
		return mpBuf;
	}

	void clear();

private:

	char* mpBuf;
};
//---------------------------------------------------------------------------
class SQLiteBinary
{
public:

	SQLiteBinary();

	~SQLiteBinary();

	void setBinary(const unsigned char* pBuf, int nLen);
	void setEncoded(const unsigned char* pBuf);

	const unsigned char* getEncoded();
	const unsigned char* getBinary();

	int getBinaryLength();

	unsigned char* allocBuffer(int nLen);

	void clear();

private:

	unsigned char* mpBuf;
	int mnBinaryLen;
	int mnBufferLen;
	int mnEncodedLen;
	bool mbEncoded;
};
//---------------------------------------------------------------------------
class SQLiteQuery
{
public:

	SQLiteQuery();

	SQLiteQuery(const SQLiteQuery& rQuery);

	SQLiteQuery(sqlite3* pDB, sqlite3_stmt* pVM, bool bEof, bool bOwnVM = true);

	SQLiteQuery& operator = (const SQLiteQuery & rQuery);

	virtual ~SQLiteQuery();

	int numFields();

	int fieldIndex(const char* szField);
	const char* fieldName(int nCol);

	const char* fieldDeclType(int nCol);
	int fieldDataType(int nCol);

	const char* fieldValue(int nField);
	const char* fieldValue(const char* szField);

	int getIntField(int nField, int nNullValue = 0);
	int getIntField(const char* szField, int nNullValue = 0);

	sqlite_int64 getInt64Field(int nField, sqlite_int64 nNullValue = 0);
	sqlite_int64 getInt64Field(const char* szField, sqlite_int64 nNullValue = 0);

	double getFloatField(int nField, double fNullValue = 0.0);
	double getFloatField(const char* szField, double fNullValue = 0.0);

	const char* getStringField(int nField, const char* szNullValue = "");
	// UnicodeString getUnicodeStringField(int nField, const char* szNullValue  ="");

	const char* getStringField(const char* szField, const char* szNullValue = "");

	const unsigned char* getBlobField(int nField, int& nLen);
	const unsigned char* getBlobField(const char* szField, int& nLen);

	bool fieldIsNull(int nField);
	bool fieldIsNull(const char* szField);

	bool eof();

	void nextRow();

	void finalize();

private:

	void checkVM();

	sqlite3* mpDB;
	sqlite3_stmt* mpVM;
	bool mbEof;
	int mnCols;
	bool mbOwnVM;
};
//---------------------------------------------------------------------------
class SQLiteTable : public VTSQLiteTable
{
public:
	SQLiteTable(void);
	SQLiteTable(const SQLiteTable& rTable);
	SQLiteTable(char** paszResults, int nRows, int nCols);
	virtual ~SQLiteTable(void);


protected:
	AnsiString FieldValue(int nField);
	AnsiString FieldValue(AnsiString szField);
	double GetFloatField(AnsiString szField, double fNullValue = 0.0);
	int GetIntField(AnsiString szField, int nNullValue = 0);
	AnsiString  GetStringField(AnsiString szField, AnsiString szNullValue = "");
	int NumRows(void);
	void SetRow(int nRow);

private:
	SQLiteTable& operator = (const SQLiteTable & rTable);
	int numFields();
	const char* fieldName(int nCol);
	const char* fieldValue(int nField);
	const char* fieldValue(const char* szField);
	int getIntField(int nField, int nNullValue = 0);
	int getIntField(const char* szField, int nNullValue = 0);
	double getFloatField(int nField, double fNullValue = 0.0);
	double getFloatField(const char* szField, double fNullValue = 0.0);
	const char* getStringField(int nField, const char* szNullValue = "");
//	const char* getStringField(const char* szField, const char* szNullValue = "");
	AnsiString  getStringField(const char* szField, const char* szNullValue = "");
	bool fieldIsNull(int nField);
	bool fieldIsNull(const char* szField);
	void finalize();

//private:

	void checkResults();

	int mnCols;
	int mnRows;
	int mnCurrentRow;
	char** mpaszResults;
};
//---------------------------------------------------------------------------
class SQLiteStatement
{
public:

	SQLiteStatement();

	SQLiteStatement(const SQLiteStatement& rStatement);

	SQLiteStatement(sqlite3* pDB, sqlite3_stmt* pVM);

	virtual ~SQLiteStatement();

	SQLiteStatement& operator = (const SQLiteStatement & rStatement);

	int execDML();

	SQLiteQuery execQuery();

	void bind(int nParam, const char* szValue);
	void bind(int nParam, const int nValue);
	void bind(int nParam, const double dwValue);
	void bind(int nParam, const unsigned char* blobValue, int nLen);
	void bindNull(int nParam);

	int bindParameterIndex(const char* szParam);
	void bind(const char* szParam, const char* szValue);
	void bind(const char* szParam, const int nValue);
	void bind(const char* szParam, const double dwValue);
	void bind(const char* szParam, const unsigned char* blobValue, int nLen);
	void bindNull(const char* szParam);

	void reset();

	void finalize();

private:

	void checkDB();
	void checkVM();

	sqlite3* mpDB;
	sqlite3_stmt* mpVM;
};
//---------------------------------------------------------------------------
class SQLiteDB  : public VTSQLiteDB
{
public:
	SQLiteDB();
	virtual ~SQLiteDB();

protected:
	void Close();
	int ExecDML(AnsiString szSQL);
	VTSQLiteTable* GetTable(AnsiString szSQL);
	void Load(AnsiString filePath);
	void LoadAttach(AnsiString filePathLoad, AnsiString attach, AnsiString name);
	void Open(AnsiString szFile);

private:
	void open(const char* szFile);
	/**
	 *carrega um arquivo sqlite na memoria
	 */
	void load(const char* filePath);
	bool tableExists(const char* szTable);
	int execDML(const char* szSQL);
	SQLiteQuery execQuery(const char* szSQL);
	int execScalar(const char* szSQL, int nNullValue = 0);
	//SQLiteTable getTable(const char* szSQL);
	VTSQLiteTable* getTable(const char* szSQL);
	SQLiteStatement compileStatement(const char* szSQL);
	sqlite_int64 lastRowId();
	void interrupt()
	{
		sqlite3_interrupt(mpDB);
	}

	void setBusyTimeout(int nMillisecs);
	static const char* SQLiteVersion()
	{
		return SQLITE_VERSION;
	}

	static const char* SQLiteHeaderVersion()
	{
		return SQLITE_VERSION;
	}

	static const char* SQLiteLibraryVersion()
	{
		return sqlite3_libversion();
	}

	static int SQLiteLibraryVersionNumber()
	{
		return sqlite3_libversion_number();
	}

	bool IsAutoCommitOn();

//private:

	SQLiteDB(const SQLiteDB& db);
	SQLiteDB& operator = (const SQLiteDB & db);

	sqlite3_stmt* compile(const char* szSQL);

	void checkDB();
	int __fastcall LoadOrSaveDb(sqlite3 *pInMemory, const char *zFilename, int isSave);

	sqlite3* mpDB;
	int mnBusyTimeoutMs;
};

#endif
