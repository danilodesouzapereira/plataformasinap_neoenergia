// ---------------------------------------------------------------------------
#ifndef TUserPluginH
#define TUserPluginH

// arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include <System.Zip.hpp>

// ---------------------------------------------------------------------------
class VTApl;

// ---------------------------------------------------------------------------
class TUserPlugin : public TObject
{
public:
	__fastcall TUserPlugin(VTApl *apl);
	__fastcall ~TUserPlugin(void);
	bool __fastcall Executa(UnicodeString &diagnostico);

private: //métodos
//	void __fastcall DeletaPastas(void);
	void __fastcall DeletaPastas(UnicodeString packEmpresa);
	bool __fastcall DescompactaPack(UnicodeString packEmpresa);
	int  __fastcall DeterminaConjuntoEmpresa(int empresa_id);
	UnicodeString __fastcall NumEmpresaStr(int num_empresa);

private:	// objetos externos
	VTApl *apl;

private: // dados locais
	TStringList *pastas;
};

// ---------------------------------------------------------------------------
#endif
// eof
