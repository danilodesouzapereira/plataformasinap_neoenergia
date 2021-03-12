//---------------------------------------------------------------------------
#ifndef VTCmdSqlH
#define VTCmdSqlH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTCmdSql : public TObject
   {
   public:  //property
      __property AnsiString Text = {read=PM_GetText};

   public:
                          __fastcall VTCmdSql(void) {};
       virtual            __fastcall ~VTCmdSql(void) {};
       virtual void       __fastcall AddComando(AnsiString cmd) = 0;
       virtual void       __fastcall AddParametro(AnsiString valor) = 0;
       virtual void       __fastcall AddParametro(TDate valor) = 0;
       virtual void       __fastcall AddParametro(int valor) = 0;
       virtual void       __fastcall AddParametro(double valor) = 0;

   protected: //métodos acessados via property
       virtual AnsiString __fastcall PM_GetText(void) = 0;
   };

//---------------------------------------------------------------------------
//métodos globais p/ criar objeto CmdSql
//---------------------------------------------------------------------------
VTCmdSql* __fastcall NewObjCmdSqlAccess(void);
VTCmdSql* __fastcall NewObjCmdSqlOracle(void);

//---------------------------------------------------------------------------
#endif
//eof

