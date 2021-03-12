//---------------------------------------------------------------------------
#ifndef TLicencaH
#define TLicencaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTLicenca.h"

//---------------------------------------------------------------------------
class TLicenca : public VTLicenca
   {
   public:
                 __fastcall TLicenca(void);
                 __fastcall ~TLicenca(void);
      AnsiString __fastcall Diagnostico(void);
      void       __fastcall ReadFromPacote(VTPacote *pacote);
      int        __fastcall VerificaCPU_ID(AnsiString cpu_id);
      int        __fastcall VerificaPeriodoValidade(void);
      void       __fastcall WriteToPacote(VTPacote *pacote);

   private: //métodos acessados via property
      AnsiString __fastcall PM_GetCPU_ID(void);
      bool       __fastcall PM_GetCtrlRemoto(void);
      TDate      __fastcall PM_GetDH_controle(void);
      TDate      __fastcall PM_GetDH_inicio(void);
      TDate      __fastcall PM_GetDH_alocada(void);
      TDate      __fastcall PM_GetDH_termino(void);
      int        __fastcall PM_GetEmpresa(void);
      AnsiString __fastcall PM_GetEmpresaStr(void);
      int        __fastcall PM_GetID(void);
      bool       __fastcall PM_GetGoogle(void);
      AnsiString __fastcall PM_GetLogin(void);
	  bool       __fastcall PM_GetLoginLocal(void);
	  AnsiString __fastcall PM_GetPassword(void);
	  DynamicArray<int>  __fastcall PM_GetOperacoes(void);
	  AnsiString __fastcall PM_GetProduto(void);
      int        __fastcall PM_GetRenovada(void);
      int        __fastcall PM_GetStatus(void);
	  int        __fastcall PM_GetTipo(void);
      int        __fastcall PM_GetValidade(void);
      void       __fastcall PM_SetCPU_ID(AnsiString cpu_id);
      void       __fastcall PM_SetCtrlRemoto(bool ctrl_remoto);
      void       __fastcall PM_SetDH_controle(TDate dh_controle);
      void       __fastcall PM_SetDH_inicio(TDate dh_inicio);
      void       __fastcall PM_SetDH_alocada(TDate dh_alocada);
      void       __fastcall PM_SetDH_termino(TDate dh_termino);
      void       __fastcall PM_SetEmpresa(int empresa);
      void       __fastcall PM_SetID(int id);
      void       __fastcall PM_SetGoogle(bool google);
      void       __fastcall PM_SetLogin(AnsiString login);
      void       __fastcall PM_SetLoginLocal(bool login_local);
	  void       __fastcall PM_SetPassword(AnsiString password);
	  void       __fastcall PM_SetOperacoes(DynamicArray<int> operacoes);
      void       __fastcall PM_SetRenovada(int renovada);
      void       __fastcall PM_SetProduto(AnsiString produto);
      void       __fastcall PM_SetStatus(int status);
      void       __fastcall PM_SetTipo(int tipo);
      void       __fastcall PM_SetValidade(int validade);
      
   private: //dados acessados via property
      struct   {
               int        id;
               int        empresa;
               AnsiString cpu_id;
               AnsiString produto;
               AnsiString login;
               AnsiString password;
               TDate      dh_alocada;
               TDate      dh_controle;
               TDate      dh_inicio;
               TDate      dh_termino;
               int        status;
               int        tipo;
               int        validade;
               int        renovada;
               bool       login_local;  //controle local de login
               bool       ctrl_remoto;  //licença alocada via servidor remoto ou local
			   bool       google;
			   DynamicArray<int> operacoes;
               }PD;
   };

#endif
//---------------------------------------------------------------------------
//eof

