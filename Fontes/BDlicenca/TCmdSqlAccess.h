//---------------------------------------------------------------------------
#ifndef TCmdSqlAccessH
#define TCmdSqlAccessH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCmdSql.h"

//---------------------------------------------------------------------------
class TCmdSqlAccess : public VTCmdSql
   {
   public:
                  __fastcall TCmdSqlAccess(void);
                  __fastcall ~TCmdSqlAccess(void);
       void       __fastcall AddComando(AnsiString cmd);
       void       __fastcall AddParametro(AnsiString valor);
       void       __fastcall AddParametro(TDate valor);
       void       __fastcall AddParametro(int valor);
       void       __fastcall AddParametro(double valor);

   protected: //métodos acessados via property
       AnsiString __fastcall PM_GetText(void);
       
   protected: //métodos
      AnsiString __fastcall DataHoraToSql(TDate data);
      
   protected: //dados locais
      struct   {
               AnsiString  comando;
               TStringList *parametro;
               } sql;
   };

//---------------------------------------------------------------------------
#endif
//eof
