//---------------------------------------------------------------------------
#ifndef TBDlicencaH
#define BDlicenca

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTBDlicenca.h"

//---------------------------------------------------------------------------
class VTBcd;
class VTCmdSql;
class VTCript;
class VTAccess;

//---------------------------------------------------------------------------
class TBDlicenca : public VTBDlicenca
   {
   public:
                 __fastcall TBDlicenca(void);
                 __fastcall ~TBDlicenca(void);
      bool       __fastcall AtualizaLicenca(VTLicenca *licenca);
      bool       __fastcall AtualizaPendrive(VTPendrive *pendrive);
      bool       __fastcall Conecta(AnsiString database_name);
      bool       __fastcall ConsultaPendrive(VTPendrive *pendrive);
      void       __fastcall Desconecta(void);
      VTLicenca* __fastcall ExisteLicenca(AnsiString produto, AnsiString login, AnsiString password);
      bool       __fastcall InsereLicenca(VTLicenca *licenca);
      bool       __fastcall InsereLog(int licenca_id, int operacao_id, int resposta_id,
                                              int erro_id, TDateTime dh, AnsiString observacao);
   private: //métodos
      bool __fastcall Criptografa(AnsiString &txt);
      bool __fastcall Descriptografa(AnsiString &txt);
      int __fastcall TabGetId(AnsiString tab_name, AnsiString campo_id,
                              AnsiString campo_ref, AnsiString valor_ref);
      int __fastcall TabNextId(AnsiString tab_name, AnsiString campo);

   private: //objetos locais
      VTAccess  *access;
      VTCmdSql  *cmd_sql;
      VTBcd     *bcd;
      VTCript   *cript;
      VTLicenca *licenca;
   };

//---------------------------------------------------------------------------
#endif
//eof
 