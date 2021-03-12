//---------------------------------------------------------------------------
#ifndef VTAccessH
#define VTAccessH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <DB.hpp>

//---------------------------------------------------------------------------
class VTAccess : public TObject
   {
   public:
                        __fastcall VTAccess(void) {};
      virtual           __fastcall ~VTAccess(void) {};
      virtual bool      __fastcall Conecta(AnsiString database_name) = 0;
      virtual bool      __fastcall Conectado(void) = 0;
      virtual void      __fastcall Desconecta(void) = 0;
      virtual TDataSet* __fastcall ExecutaCmdSelect(AnsiString cmd_sql) = 0;
      virtual bool      __fastcall ExecutaCmdUpdate(AnsiString cmd_sql) = 0;
   };

//---------------------------------------------------------------------------
//função p/ criar/destruir objetos da classe
//---------------------------------------------------------------------------
VTAccess* __fastcall NewObjAccess(void);

//---------------------------------------------------------------------------
#endif
//eof
