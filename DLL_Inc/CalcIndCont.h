//---------------------------------------------------------------------------
#ifndef CalcIndContH
#define CalcIndContH

//---------------------------------------------------------------------------
#include <vcl.h>
#include <Classes.hpp>
#include "..\DLL_Inc\Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTAvaliaVis;
class VTCalcIndCont;
class VTCalcEstratSoc;
class VTBuscaProt;
class VTDefeito;
class VTBloco;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT VTCalcIndCont*   __fastcall DLL_NewCalcIndCont(VTApl *apl_owner, TList *lisChavesFunde = NULL, bool flagVis = true);
//EXPORT VTCalcIndCont*   __fastcall DLL_NewObjCalcIndContCosim(VTApl *apl_owner, TList *lisChavesFunde, bool flagVis);//hk0818: versao inicial cosimulação
EXPORT VTBuscaProt*     __fastcall DLL_NewBuscaProt(VTApl *apl_owner, bool flagCalc = false);
EXPORT VTCalcEstratSoc* __fastcall DLL_NewObjCalcEstratSoc(void);
EXPORT VTAvaliaVis*     __fastcall DLL_NewAvaliaVis(VTApl *apl_owner);
EXPORT VTDefeito*       __fastcall DLL_NewObjDefeito(VTBloco *blocoDef, int numPat);
EXPORT TForm*           __fastcall DLL_NewFormCalcIndCont(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT TForm*           __fastcall DLL_NewFormAvaliaConf(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT TForm*           __fastcall DLL_NewFormMostraDefeito(TComponent *Owner, VTApl *apl_owner, VTDefeito *pdef, TList *lisChavesAuto, TWinControl *parent);
EXPORT TForm*           __fastcall DLL_NewFormEliminaFusivel(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);
EXPORT TForm*           __fastcall DLL_NewFormExportaInfoCircuito(TComponent *Owner, VTApl *apl_owner, TWinControl *parent);

//---------------------------------------------------------------------------
#endif
//eof

