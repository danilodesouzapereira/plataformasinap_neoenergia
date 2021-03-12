//---------------------------------------------------------------------------
#ifndef TAccessH
#define TAccessH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <DB.hpp>
#include <ADODB.hpp>
#include "VTAccess.h"

//---------------------------------------------------------------------------
class TAccess : public VTAccess
   {
   public:
                __fastcall TAccess(void);
                __fastcall ~TAccess(void);
      bool      __fastcall Conecta(AnsiString database_name);
      bool      __fastcall Conectado(void);
      void      __fastcall Desconecta(void);
      TDataSet* __fastcall ExecutaCmdSelect(AnsiString cmd_sql);
      bool      __fastcall ExecutaCmdUpdate(AnsiString cmd_sql);

   private: //métodos
      bool __fastcall Conectado(AnsiString arq_mdb);
      bool __fastcall IniciaQuery(AnsiString cmd_sql);

   private: //objetos locais
      AnsiString     arq_mdb;
      TADOConnection *connection;
      TADOQuery      *query;
   };

//---------------------------------------------------------------------------
#endif
//eof
 