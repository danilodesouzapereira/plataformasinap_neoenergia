//---------------------------------------------------------------------------
#ifndef ProdutoH
#define ProdutoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//-----------------------------------------------------------------------------
class VTApl;
class VTProduto;

//-----------------------------------------------------------------------------
EXPORT TForm*      __fastcall DLL_NewFormLogo(TComponent *Owner, TWinControl *parent);
EXPORT TForm*      __fastcall DLL_NewFormSobre(TComponent *Owner, VTApl *apl_owner);
EXPORT VTProduto*  __fastcall DLL_NewObjProduto(void);

//---------------------------------------------------------------------------
#endif
//eof


 