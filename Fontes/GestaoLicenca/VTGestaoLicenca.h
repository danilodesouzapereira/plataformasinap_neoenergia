#ifndef VTGestaoLicencaH
#define VTGestaoLicencaH

// arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

// -----------------------------------------------------------------------------
class VTApl;
class VTLicenca;
class TLicencaRequest;
class TLicencaAlocada;
/*
namespace sinapsis
{
    namespace sinap
    {
		namespace gestaoLicenca
        {
			class TGestaoLicenca;
        }
    }
}

using sinapsis::sinap::gestaoLicenca::TGestaoLicenca;
*/
// -----------------------------------------------------------------------------
class VTGestaoLicenca : public TObject
{

public:
	__fastcall VTGestaoLicenca(void){};
	virtual __fastcall ~VTGestaoLicenca(void){};
	virtual void __fastcall AlocaLicenca(TLicencaAlocada *retorno, UnicodeString &diagnostico) = 0;
	virtual bool __fastcall AlocaLicencaSinapViewer(UnicodeString &diagnostico) = 0;
	virtual AnsiString __fastcall CPUId(void) = 0;
	virtual int __fastcall EmpresaId(AnsiString empresa_nome) = 0;
	virtual void __fastcall LiberaLicencaLocal(UnicodeString &diagnostico) = 0;
	virtual void __fastcall LiberaLicencaRemota(UnicodeString &diagnostico) = 0;
	virtual bool __fastcall LiberaLicencaSinapViewer(UnicodeString &diagnostico) = 0;
	virtual bool __fastcall LicencaRemota(void) = 0;
    virtual bool __fastcall LicencaSinapViewer(void) = 0;
	virtual TLicencaRequest* __fastcall MontaRequest(UnicodeString login, UnicodeString senha, int dominio) = 0;
    virtual TLicencaRequest* __fastcall MontaRequest(void) = 0;
	virtual AnsiString __fastcall Produto(void) = 0;

};

// -----------------------------------------------------------------------------
// função global para criar objeto VTGestaoLicenca
// -----------------------------------------------------------------------------
VTGestaoLicenca* __fastcall NewObjGestaoLicenca(VTApl *apl);

#endif
// ---------------------------------------------------------------------------
// eof
