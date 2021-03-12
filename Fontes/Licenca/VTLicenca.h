//---------------------------------------------------------------------------
#ifndef VTLicencaH
#define VTLicencaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEmpresa.h"

//---------------------------------------------------------------------------
enum enumStatusLicenca {sttLIVRE=0,   sttALOCADA,  sttEXPIRADA,
						sttBLOQUEADA, sttINEXISTENTE};

//---------------------------------------------------------------------------
enum enumTipoLicenca   {tipoINDEFINIDA  =     -1,
						tipoMASTER      =     0x0,
						tipoUSER        =     0x1,
						tipoDEMO        =     0x2,
						tipoELPA        =     0x3,
						tipoDIDATICA    =     0x4,
						//tipoGOOGLE      = 0x10000,
						tipoGOOGLE      =   0x400,
						//Eletropaulo
						tipoDIAGNOSTICO = 100};

//---------------------------------------------------------------------------
class VTPacote;

//---------------------------------------------------------------------------
class VTLicenca : public TObject
   {
   public:  //property
	  __property int           ID          = {read=PM_GetID,          write=PM_SetID};
	  __property AnsiString    CPU_ID      = {read=PM_GetCPU_ID,      write=PM_SetCPU_ID};
	  __property AnsiString    Produto     = {read=PM_GetProduto,     write=PM_SetProduto};
	  __property AnsiString    Login       = {read=PM_GetLogin,       write=PM_SetLogin};
	  __property AnsiString    Password    = {read=PM_GetPassword,    write=PM_SetPassword};
	  __property AnsiString    EmpresaStr  = {read=PM_GetEmpresaStr};
	  __property TDate         DH_alocada  = {read=PM_GetDH_alocada,  write=PM_SetDH_alocada};
	  __property TDate         DH_inicio   = {read=PM_GetDH_inicio,   write=PM_SetDH_inicio};
	  __property TDate         DH_controle = {read=PM_GetDH_controle, write=PM_SetDH_controle};
	  __property TDate         DH_termino  = {read=PM_GetDH_termino,  write=PM_SetDH_termino};
	  __property int           Status      = {read=PM_GetStatus,      write=PM_SetStatus};
	  __property int           Tipo        = {read=PM_GetTipo,        write=PM_SetTipo};
	  __property int           Empresa     = {read=PM_GetEmpresa,     write=PM_SetEmpresa};
	  __property int           Validade    = {read=PM_GetValidade,    write=PM_SetValidade};
	  __property int           Renovada    = {read=PM_GetRenovada,    write=PM_SetRenovada};
	  __property bool          LoginLocal  = {read=PM_GetLoginLocal,  write=PM_SetLoginLocal};
	  __property bool          CtrlRemoto  = {read=PM_GetCtrlRemoto,  write=PM_SetCtrlRemoto};
	  __property bool          Google      = {read=PM_GetGoogle,      write=PM_SetGoogle};
	  __property DynamicArray<int> Operacoes = {read=PM_GetOperacoes, write=PM_SetOperacoes};

   public:
						  __fastcall VTLicenca(void) {};
	  virtual             __fastcall ~VTLicenca(void) {};
	  virtual  AnsiString __fastcall Diagnostico(void) = 0;
	  virtual  void       __fastcall ReadFromPacote(VTPacote *pacote) = 0;
	  virtual  int        __fastcall VerificaCPU_ID(AnsiString cpu_id) = 0;
	  virtual  int        __fastcall VerificaPeriodoValidade(void) = 0;
	  virtual  void       __fastcall WriteToPacote(VTPacote *pacote) = 0;

   protected: //métodos acessados via property
	  virtual AnsiString    __fastcall PM_GetCPU_ID(void) = 0;
	  virtual bool          __fastcall PM_GetCtrlRemoto(void) = 0;
	  virtual TDate         __fastcall PM_GetDH_controle(void) = 0;
	  virtual TDate         __fastcall PM_GetDH_inicio(void) = 0;
	  virtual TDate         __fastcall PM_GetDH_alocada(void) = 0;
	  virtual TDate         __fastcall PM_GetDH_termino(void) = 0;
	  virtual int           __fastcall PM_GetEmpresa(void) = 0;
	  virtual AnsiString    __fastcall PM_GetEmpresaStr(void) = 0;
	  virtual int           __fastcall PM_GetID(void) = 0;
	  virtual bool          __fastcall PM_GetGoogle(void) = 0;
	  virtual AnsiString    __fastcall PM_GetLogin(void) = 0;
	  virtual bool          __fastcall PM_GetLoginLocal(void) = 0;
	  virtual AnsiString    __fastcall PM_GetPassword(void) = 0;
	  virtual DynamicArray<int> __fastcall PM_GetOperacoes(void) = 0;
	  virtual AnsiString    __fastcall PM_GetProduto(void) = 0;
	  virtual int           __fastcall PM_GetRenovada(void) = 0;
	  virtual int           __fastcall PM_GetStatus(void) = 0;
	  virtual int           __fastcall PM_GetTipo(void) = 0;
	  virtual int           __fastcall PM_GetValidade(void) = 0;
	  virtual void          __fastcall PM_SetCPU_ID(AnsiString cpu_id) = 0;
	  virtual void          __fastcall PM_SetCtrlRemoto(bool ctrl_remoto) = 0;
	  virtual void          __fastcall PM_SetDH_controle(TDate dh_controle) = 0;
	  virtual void          __fastcall PM_SetDH_inicio(TDate dh_inicio) = 0;
	  virtual void          __fastcall PM_SetDH_alocada(TDate dh_alocada) = 0;
	  virtual void          __fastcall PM_SetDH_termino(TDate dh_termino) = 0;
	  virtual void          __fastcall PM_SetEmpresa(int empresa) = 0;
	  virtual void          __fastcall PM_SetID(int id) = 0;
	  virtual void          __fastcall PM_SetGoogle(bool google) = 0;
	  virtual void          __fastcall PM_SetLogin(AnsiString login) = 0;
	  virtual void          __fastcall PM_SetLoginLocal(bool login_local) = 0;
	  virtual void          __fastcall PM_SetPassword(AnsiString password) = 0;
	  virtual void          __fastcall PM_SetOperacoes(DynamicArray<int> operacoes) = 0;
	  virtual void          __fastcall PM_SetProduto(AnsiString produto) = 0;
	  virtual void          __fastcall PM_SetRenovada(int renovada) = 0;
	  virtual void          __fastcall PM_SetStatus(int status) = 0;
	  virtual void          __fastcall PM_SetTipo(int tipo) = 0;
	  virtual void          __fastcall PM_SetValidade(int validade) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTLicenca* __fastcall NewObjLicenca(void);


#endif
//---------------------------------------------------------------------------//eof

