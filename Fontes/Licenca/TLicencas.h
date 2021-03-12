//---------------------------------------------------------------------------
#ifndef TLicencasH
#define TLicencasH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTLicencas.h"

//---------------------------------------------------------------------------
class VTServidor;
class VTBcd;
class VTCript;
class VTLicenca;
class VTMicro;
class VTPacote;

//---------------------------------------------------------------------------
class TLicencas : public VTLicencas
   {
   //funcões
   public :
				 __fastcall TLicencas(VTApl *apl);
				 __fastcall ~TLicencas(void);
	  bool       __fastcall AlocaLicencaLocal(AnsiString login, AnsiString password,
											  bool demo, AnsiString &diagnostico);
	  bool       __fastcall AlocaLicencaRemota(AnsiString login, AnsiString password,
											   bool demo, AnsiString &diagnostico);
	  VTLicenca* __fastcall ExisteLicenca(void);
	  void       __fastcall GravaLicencas(void);
	  bool       __fastcall InsereLicenca(VTLicenca *licenca);
	  bool       __fastcall LiberaLicenca(AnsiString &diagnostico);
	  bool       __fastcall LicencaTipoDemo(void);
	  bool       __fastcall LicencaTipoDidatica(void);
	  bool       __fastcall RemoveLicenca(VTLicenca *licenca);
	  bool       __fastcall RenovaLicenca(AnsiString &diagnostico);
	  bool       __fastcall ValidaLicenca(AnsiString &diagnostico);
	  bool       __fastcall ValidaOperacoes(AnsiString &diagnostico);

   protected:  //métodos acessados via property
	  bool __fastcall PM_GetLoginLocal(void);
	  bool __fastcall PM_GetValidadeLimitada(void);
	  void __fastcall PM_SetLoginLocal(bool enabled);

   private :
	  bool       __fastcall AlocaLicenca(VTServidor *servidor, AnsiString login,
										 AnsiString password, bool demo, AnsiString &diagnostico);
	  bool       __fastcall Criptografa(AnsiString &txt);
	  bool       __fastcall Descriptografa(AnsiString &txt);
	  VTLicenca* __fastcall ExisteLicenca(AnsiString produto);
	  void       __fastcall GravaLicencasArquivo(void);
	  void       __fastcall GravaLicencasRegWin(void);
	  void       __fastcall LeLicencas(void);
	  void       __fastcall LeLicencasArquivo(void);
	  void       __fastcall LeLicencasRegWin(void);
	  void       __fastcall SalvaLicenca(VTLicenca *licenca);

   public:  //objetos externos
	  VTApl *apl;

   private: //dados locais
	  VTBcd       *bcd;
	  VTCript     *cript;
	  VTMicro     *micro;
	  VTPacote    *pacote;
	  //TStringList *StrListArq;
	  TList       *lisLICENCA;
   };
//---------------------------------------------------------------------------
#endif
//eof
