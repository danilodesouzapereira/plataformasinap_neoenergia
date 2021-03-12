//---------------------------------------------------------------------------
#ifndef VTBDlicencaH
#define VTBDlicencaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTLicenca;
class VTPendrive;

//---------------------------------------------------------------------------
class VTBDlicenca : public TObject
   {
   public:
                         __fastcall VTBDlicenca(void) {};
      virtual            __fastcall ~VTBDlicenca(void) {};
      virtual bool       __fastcall AtualizaLicenca(VTLicenca *licenca) = 0;
      virtual bool       __fastcall AtualizaPendrive(VTPendrive *pendrive) = 0;
      virtual bool       __fastcall Conecta(AnsiString database_name) = 0;
      virtual void       __fastcall Desconecta(void) = 0;
      virtual bool       __fastcall ConsultaPendrive(VTPendrive *pendrive) = 0;
      virtual VTLicenca* __fastcall ExisteLicenca(AnsiString produto, AnsiString login, AnsiString password) = 0;
      virtual bool       __fastcall InsereLicenca(VTLicenca *licenca) = 0;
      virtual bool       __fastcall InsereLog(int licenca_id, int operacao_id, int resposta_id,
                                              int erro_id, TDateTime dh, AnsiString observacao) = 0;
   };

//---------------------------------------------------------------------------
//função p/ criar/destruir objetos da classe
//---------------------------------------------------------------------------
VTBDlicenca* __fastcall NewObjBDlicenca(void);

//---------------------------------------------------------------------------
#endif
//eof
