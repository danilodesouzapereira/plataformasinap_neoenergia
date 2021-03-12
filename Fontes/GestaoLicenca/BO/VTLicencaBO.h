#ifndef VTLicencaBOH
#define VTLicencaBOH

// arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include <Fontes\GestaoLicenca\Exceptions\ELicencaConnection.h>
// -----------------------------------------------------------------------------
class VTApl;
class TLicencaRequest;

// -----------------------------------------------------------------------------
class VTLicencaBO : public TObject
{

public:
	        __fastcall VTLicencaBO(void){};
	virtual __fastcall ~VTLicencaBO(void){};
	virtual bool  Alocar(TLicencaRequest* request, UnicodeString &diagnostico)throw(ELicencaConnection)= 0 ;
	virtual void  ConfiguraProxy(AnsiString server="", int port = -1,AnsiString user="",AnsiString password="") = 0;
	virtual bool  Liberar(UnicodeString &diagnostico)throw(ELicencaConnection) = 0;
	virtual bool  LicencaSinapViewer(UnicodeString login, UnicodeString senha, UnicodeString dominio, UnicodeString &diagnostico) = 0;
	virtual TLicencaRequest* MontaRequest(UnicodeString login, UnicodeString senha, UnicodeString dominio) = 0;

};

// -----------------------------------------------------------------------------
// função global para criar objeto VTLicencaBO
// -----------------------------------------------------------------------------
VTLicencaBO* __fastcall NewObjLicencaBO(VTApl *apl);

#endif
// ---------------------------------------------------------------------------
// eof
