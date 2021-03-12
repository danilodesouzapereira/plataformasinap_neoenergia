// ---------------------------------------------------------------------------
#ifndef TRBACManagerH
#define TRBACManagerH

// arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTRBACManager.h"

// ---------------------------------------------------------------------------
class VTApl;

// ---------------------------------------------------------------------------
class TModOp : public TObject
{
    public:
        __fastcall TModOp(void){};
        __fastcall ~TModOp(void){};
    public:
        int operacao_id;
        DynamicArray<int> modulo_id;
        UnicodeString operacao_codigo;
};

// ---------------------------------------------------------------------------
class TRBACManager : public VTRBACManager
{
public:
	__fastcall TRBACManager(VTApl *apl);
	__fastcall ~TRBACManager(void);
	bool __fastcall ExistePermissaoModulo(int modulo_id);
	UnicodeString __fastcall GetDescricaoOperacao(int operacao_id);
    bool __fastcall SinapViewer(void);

private: // métodos
    void __fastcall CriaOperacoes(void);
	void __fastcall MontaMap(void);

private: // objetos externos
	VTApl *apl;

private: // objetos locais
    TList *lisOP;
//	std::map<int, UnicodeString>operacao;
    std::map<int, TModOp*>operacao;

};

// ---------------------------------------------------------------------------
#endif
