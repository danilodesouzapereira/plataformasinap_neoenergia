//---------------------------------------------------------------------------

#ifndef TFormCatalogoTrfH
#define TFormCatalogoTrfH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "TFormCatalogo.h"
#include "TDiagTrafo.h"
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ValEdit.hpp>
//---------------------------------------------------------------------------
//todos strings
const AnsiString trfNOVOTRF               = "Novo Transformador ";
const AnsiString trfATRIBUTO_COD          = "C�digo";
const AnsiString trfATRIBUTO_SNOM_MVA     = "Snom [MVA]";
const AnsiString trfATRIBUTO_VNOMPRI_KV   = "Prim�rio: Vnom [kV]";
const AnsiString trfATRIBUTO_VNOMSEC_KV   = "Secund�rio: Vnom [kV]";
//const AnsiString capATRIBUTO_LIGPRI  = "Liga��o Prim�rio";
//const AnsiString capATRIBUTO_LIGSEC  = "Liga��o Secund�rio";
const AnsiString trfTRIBUTO_PERDAFE      = "Perda no Ferro [%]";

//---------------------------------------------------------------------------
class VTApl;
//---------------------------------------------------------------------------
class TFormCatalogoTrf : public TFormCatalogo
{
__published:	// IDE-managed Components
private:	// User declarations
   TItemCatalogo*  __fastcall CriaItem(void);
   TItemCatalogo*  __fastcall CriaItem(DIAG_TRAFO *cfgDiagTrafo);
   void            __fastcall IniciaListValue(void);
   void            __fastcall SalvaItemsCatalogo(void);

public:		// User declarations
   __fastcall TFormCatalogoTrf(TComponent* Owner, VTApl *apl);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCatalogoTrf *FormCatalogoTrf;
//---------------------------------------------------------------------------
#endif
