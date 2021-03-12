// ---------------------------------------------------------------------------
#ifndef TEmpresaH
#define TEmpresaH

// arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTEmpresa.h"

// ---------------------------------------------------------------------------
class TEmpresa : public VTEmpresa
{
public:
	__fastcall TEmpresa(void);
	__fastcall ~TEmpresa(void);
	UnicodeString __fastcall EmpresaAsStr(int empresa_id);
	int __fastcall EmpresaAsInt(UnicodeString empresa_nome);

private:
	void __fastcall MontaMap(void);

private: // objetos locais
	std::map<int, UnicodeString>empresa;

};

// ---------------------------------------------------------------------------
#endif
// eof
