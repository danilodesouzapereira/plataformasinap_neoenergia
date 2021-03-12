// ---------------------------------------------------------------------------
#ifndef TGestaoLicencaH
#define TGestaoLicencaH

// arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTGestaoLicenca.h"

// --------------------------------------------------------------------------------------- namespace


// ---------------------------------------------------------------------------

//class VTCript;
class VTLicenca;
class VTMicro;
class VTPacote;

// ---------------------------------------------------------------------------
class TGestaoLicenca : public VTGestaoLicenca
{
	// funcões
public:
	__fastcall TGestaoLicenca(VTApl *apl);
	__fastcall ~TGestaoLicenca(void);
    void             __fastcall AlocaLicenca(TLicencaAlocada *retorno, 
                                             UnicodeString   &diagnostico);
    bool __fastcall AlocaLicencaSinapViewer(UnicodeString &diagnostico);
	AnsiString __fastcall CPUId(void);
	int __fastcall EmpresaId(AnsiString empresa_nome);
	void __fastcall LiberaLicencaLocal(UnicodeString &diagnostico);
	void __fastcall LiberaLicencaRemota(UnicodeString &diagnostico);
    bool __fastcall LiberaLicencaSinapViewer(UnicodeString &diagnostico);
	bool __fastcall LicencaRemota(void);
    bool __fastcall LicencaSinapViewer(void);
	TLicencaRequest* __fastcall MontaRequest(UnicodeString login, UnicodeString senha, int dominio);
    TLicencaRequest* __fastcall MontaRequest(void);
	AnsiString __fastcall Produto(void);

private:
    bool             __fastcall AlteraEmpresaRegistro(int            empresa_id, 
                                                      UnicodeString &diagnostico, 
                                                      bool           aloca);
	bool __fastcall AlteraIPE(int empresa_id, UnicodeString &diagnostico);
    bool             __fastcall ArmazenaLicenca(VTLicenca     *licenca, 
                                                UnicodeString &diagnostico);
    VTLicenca*       __fastcall CriaLicenca(TLicencaAlocada *retorno, 
                                            UnicodeString   &diagnostico);
    TLicencaAlocada* __fastcall CriaRetornoSinapViewer(UnicodeString &diagnostico);
//	bool __fastcall DescompactaUsuario(UnicodeString &diagnostico);
	void __fastcall InsereEnderecoARIES(UnicodeString &diagnostico);
	void __fastcall InsereEnderecoVazio(UnicodeString &diagnostico);
	void __fastcall InsereEnderecoVIP(UnicodeString &diagnostico);
//    bool __fastcall LiberaDLL(UnicodeString &diagnostico);
	void __fastcall MontaRequest(VTLicenca *licenca, TLicencaRequest *request);
	// bool       __fastcall Criptografa(AnsiString &txt);
	// bool       __fastcall Descriptografa(AnsiString &txt);

public : // objetos externos
	VTApl *apl;

public : // dados locais
	VTMicro *micro;
	TLicencaRequest* request;
    AnsiString       empresa;
};

// ---------------------------------------------------------------------------
#endif
// eof
