// ---------------------------------------------------------------------------

#ifndef VTFamiliaArranjosH
#define VTFamiliaArranjosH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Constante\Fases.h"
// ---------------------------------------------------------------------------
#define fa_MAX_FASE  (faseABCNT + 1)
// ---------------------------------------------------------------------------
class VTApl;
class VTArranjo;
// ---------------------------------------------------------------------------
class VTFamiliaArranjos : public TObject
{
public:
	__fastcall VTFamiliaArranjos(void){};
	__fastcall ~VTFamiliaArranjos(void){};

public: // property
		__property AnsiString Codigo = {read = mCodigo, write = mCodigo};
		__property VTArranjo* Arranjo[int fase] = {read=PM_GetArranjo};
public:
	virtual bool __fastcall InsereArranjo(VTArranjo *arranjo)=0;

protected:
	AnsiString mCodigo;
	VTArranjo *arranjos[fa_MAX_FASE];//vetor de arranjos com indices relacioandos à fase

protected: //funções acessadas via property
	virtual VTArranjo* __fastcall PM_GetArranjo(int fase) = 0;
};
// ---------------------------------------------------------------------------
VTFamiliaArranjos* __fastcall NewObjFamiliaArranjos(VTApl *apl);
// ---------------------------------------------------------------------------
#endif
// eof
