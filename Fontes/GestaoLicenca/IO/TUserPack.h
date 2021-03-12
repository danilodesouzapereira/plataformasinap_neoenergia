// ---------------------------------------------------------------------------
#ifndef TUserPackH
#define TUserPackH

// arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include <System.Zip.hpp>

// ---------------------------------------------------------------------------
class VTApl;

// ---------------------------------------------------------------------------
class TUserPack : public TObject
{
public:
	__fastcall TUserPack(VTApl *apl);
	__fastcall ~TUserPack(void);
	bool __fastcall Executa(UnicodeString &diagnostico);

private: //métodos
	void __fastcall DeletaPastas(void);
	bool __fastcall DescompactaPack(UnicodeString packEmpresa);
	int  __fastcall DeterminaConjuntoEmpresa(int empresa_id);
	void __fastcall ListaPastas(void);
	UnicodeString __fastcall NumEmpresaStr(int num_empresa);

private:	// objetos externos
	VTApl *apl;

private: // dados locais
	TStringList *pastas;
};

// ---------------------------------------------------------------------------
#endif
// eof
