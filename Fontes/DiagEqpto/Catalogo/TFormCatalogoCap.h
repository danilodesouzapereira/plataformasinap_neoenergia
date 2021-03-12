//---------------------------------------------------------------------------

#ifndef TFormCatalogoCapH
#define TFormCatalogoCapH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "TFormCatalogo.h"
#include "TDiagCapacitor.h"
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ValEdit.hpp>
//---------------------------------------------------------------------------
//todos strings
const AnsiString capNOVOCAP         = "Novo Capacitor ";
const AnsiString capATRIBUTO_COD    = "Código";
const AnsiString capATRIBUTO_QMVAR  = "Q [MVAr]";

//---------------------------------------------------------------------------
class TFormCatalogoCap : public TFormCatalogo
{
__published:	// IDE-managed Components

private:	// User declarations
   TItemCatalogo*  __fastcall CriaItem(void);
   TItemCatalogo*  __fastcall CriaItem(DIAG_CAPACITOR *diagCapacitor);
   void            __fastcall IniciaListValue(void);
   void            __fastcall SalvaItemsCatalogo(void);

public:		// User declarations
   __fastcall TFormCatalogoCap(TComponent* Owner, VTApl *apl);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCatalogoCap *FormCatalogoCap;
//---------------------------------------------------------------------------
#endif
