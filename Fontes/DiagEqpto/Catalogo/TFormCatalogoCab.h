//---------------------------------------------------------------------------

#ifndef TFormCatalogoCabH
#define TFormCatalogoCabH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "TFormCatalogo.h"
#include "TDiagCabo.h"
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ValEdit.hpp>
//---------------------------------------------------------------------------
//todos strings
const AnsiString cabNOVOCAB            = "Novo Cabo ";
const AnsiString cabATRIBUTO_COD       = "Código";
const AnsiString cabATRIBUTO_IADM      = "I adm [A]";
const AnsiString cabATRIBUTO_VMAX      = "V max [kV]";
const AnsiString cabATRIBUTO_VMIN      = "V min [kV]";
const AnsiString cabATRIBUTO_DEFRMG    = "Def. por R e RMG";
const AnsiString cabATRIBUTO_RESIST    = "R [ohm/km]";
const AnsiString cabATRIBUTO_RAIO      = "Raio [m]";
const AnsiString cabATRIBUTO_RMG       = "RMG [m]";
const AnsiString cabATRIBUTO_DEFZSEQ   = "Def. por Zseq";
const AnsiString cabATRIBUTO_PREUN     = "Pré-reunido";
const AnsiString cabATRIBUTO_Z0R       = "Z0 R [ohm/km]";
const AnsiString cabATRIBUTO_Z0X       = "Z0 X [ohm/km]";
const AnsiString cabATRIBUTO_Z0C       = "Z0 C [nF/km]";
const AnsiString cabATRIBUTO_Z1R       = "Z1 R [ohm/km]";
const AnsiString cabATRIBUTO_Z1X       = "Z1 X [ohm/km]";
const AnsiString cabATRIBUTO_Z1C       = "Z1 C [nF/km]";

//---------------------------------------------------------------------------
class TFormCatalogoCab : public TFormCatalogo
{
__published:	// IDE-managed Components

private:	// User declarations
   TItemCatalogo*  __fastcall CriaItem(void);
   TItemCatalogo*  __fastcall CriaItem(DIAG_CABO *diagCabo);
   void            __fastcall IniciaListValue(void);
   void            __fastcall SalvaItemsCatalogo(void);

public:		// User declarations
   __fastcall TFormCatalogoCab(TComponent* Owner, VTApl *apl);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCatalogoCab *FormCatalogoCab;
//---------------------------------------------------------------------------
#endif
