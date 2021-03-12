//---------------------------------------------------------------------------
#ifndef VTSQLiteH
#define VTSQLiteH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\LocalDB\SQLite\ESQLite.h>
//---------------------------------------------------------------------------
//class VTApl;

//---------------------------------------------------------------------------
class VTSQLiteTable //: public TObject
	{
	public:
							VTSQLiteTable(void) {};
		virtual            	~VTSQLiteTable(void) {};
		virtual AnsiString  FieldValue(int nField)=0;
		virtual AnsiString  FieldValue(AnsiString szField)=0;
		virtual double      GetFloatField(AnsiString szField, double nNullValue = 0)=0;
		virtual int         GetIntField(AnsiString szField, int nNullValue = 0)=0;
		virtual AnsiString  GetStringField(AnsiString szField, AnsiString szNullValue = "")=0;
		virtual int         NumRows(void)=0;
		virtual void        SetRow(int nRow)=0;;
	};

//---------------------------------------------------------------------------
class VTSQLiteDB //: public TObject
	{
	public:
							   VTSQLiteDB(void) {};
		virtual               ~VTSQLiteDB(void) {};
		virtual void           Close(void)=0;
		virtual int            ExecDML(AnsiString szSQL)=0;
		virtual VTSQLiteTable* GetTable(AnsiString szSQL)=0;
		virtual void           LoadAttach(AnsiString filePathLoad, AnsiString attach, AnsiString name)=0;
		virtual void           Load(AnsiString filePath)=0;  //carrega em memória
		virtual void           Open(AnsiString szFile)=0;    //carrega em disco
	};

//---------------------------------------------------------------------------
VTSQLiteDB* __fastcall NewObjSQLiteDB(void);
//VTSQLiteTable* __fastcall NewObjSQLiteTable(void);

//---------------------------------------------------------------------------
#endif
//eof


 