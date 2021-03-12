#ifndef VTLicencas_H
#define VTLicencas_H

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;
class VTLicenca;

//-----------------------------------------------------------------------------
class VTLicencas  : public TObject
   {
   public: //property
		__property bool LoginLocal       = {read=PM_GetLoginLocal, write=PM_SetLoginLocal};
		__property bool ValidadeLimitada = {read=PM_GetValidadeLimitada};

   public:
						 __fastcall VTLicencas(void)  {};
	  virtual            __fastcall ~VTLicencas(void) {};
	  virtual bool       __fastcall AlocaLicencaLocal(AnsiString login, AnsiString password,
													  bool demo, AnsiString &diagnostico)  = 0;
	  virtual bool       __fastcall AlocaLicencaRemota(AnsiString login, AnsiString password,
													   bool demo, AnsiString &diagnostico)  = 0;
	  virtual VTLicenca* __fastcall ExisteLicenca(void) = 0;
	  virtual void       __fastcall GravaLicencas(void) = 0;
	  virtual bool       __fastcall InsereLicenca(VTLicenca *licenca) = 0;
	  virtual bool       __fastcall LiberaLicenca(AnsiString &diagnostico) = 0;
	  virtual bool       __fastcall LicencaTipoDemo(void) = 0;
	  virtual bool       __fastcall LicencaTipoDidatica(void) = 0;
	  virtual bool       __fastcall RemoveLicenca(VTLicenca *licenca) = 0;
	  virtual bool       __fastcall ValidaLicenca(AnsiString &diagnostico) = 0;
	  virtual bool       __fastcall ValidaOperacoes(AnsiString &diagnostico) = 0;

   //protected:  //métodos acessados via property
	  virtual bool __fastcall PM_GetLoginLocal(void) = 0;
	  virtual bool __fastcall PM_GetValidadeLimitada(void) = 0;
	  virtual void __fastcall PM_SetLoginLocal(bool enabled) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTLicencas
//-----------------------------------------------------------------------------
VTLicencas* __fastcall NewObjLicencas(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
