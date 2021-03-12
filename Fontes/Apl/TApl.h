// ---------------------------------------------------------------------------
#ifndef TAplH
#define TAplH

// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTApl.h"

// ---------------------------------------------------------------------------
class TApl : public VTApl
{
public:
	__fastcall TApl(TForm *form_owner, VTApl *apl_owner);
	virtual __fastcall ~TApl(void);
	virtual void __fastcall Add(TObject *obj);
	virtual TForm* __fastcall GetForm(void);
	virtual TObject* __fastcall GetObject(AnsiString class_name);
	virtual TObject* __fastcall GetObject(TMetaClass* meta_class);
	virtual void __fastcall Remove(TObject *obj);

protected: // métodos
	virtual bool __fastcall ObjetoProibido(AnsiString class_name);
	virtual bool __fastcall ObjetoProibido(TMetaClass* meta_class);

	// dados
	TList *lisOBJ;
};

#endif
// ---------------------------------------------------------------------------
// eof
