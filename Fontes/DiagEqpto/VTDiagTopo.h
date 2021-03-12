//---------------------------------------------------------------------------
#ifndef VTDiagTopoH
#define VTDiagTopoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class TDiagArranjo;
class TDiagBarra;
class TDiagBIso;
class TDiagCabo;
class TDiagCapacitor;
class TDiagCarga;
class TDiagChave;
class TDiagRede;
class TDiagReg;
class TDiagTrecho;
class TDiagTrafo;

//---------------------------------------------------------------------------
class VTDiagTopo : public TObject
{
public: //property
    __property TDiagArranjo*   DGArranjo     = {read = PM_GetDGArranjo};
    __property TDiagBarra*     DGBarra       = {read = PM_GetDGBarra};
    __property TDiagBIso*      DGBIso        = {read = PM_GetDGBIso};
    __property TDiagCabo*      DGCabo        = {read = PM_GetDGCabo};
    __property TDiagCapacitor* DGCapacitor   = {read = PM_GetDGCapacitor};
    __property TDiagCarga*     DGCarga       = {read = PM_GetDGCarga};
    __property TDiagChave*     DGChave       = {read = PM_GetDGChave};
    __property TDiagRede*      DGRede        = {read = PM_GetDGRede};
    __property TDiagReg*       DGReg         = {read = PM_GetDGReg};
    __property TDiagTrecho*    DGTrecho      = {read = PM_GetDGTrecho};
    __property TDiagTrafo*     DGTrafo       = {read = PM_GetDGTrafo};
    __property bool            RedePreparada = {read = PM_GetRedePreparada};

public: //User declarations
                     __fastcall  VTDiagTopo(void) {}
                     __fastcall ~VTDiagTopo(void) {};
    virtual void     __fastcall  Inicia(void) = 0;
    virtual void     __fastcall  LeCfgDiagEqpto(int tipo_DGEQPTO) = 0;
    virtual void     __fastcall  PreExecuta(void) = 0;
    virtual void     __fastcall  SalvaCfgDiagEqpto(int tipo_DGEQPTO) = 0;

protected:
    virtual bool            __fastcall PM_GetRedePreparada(void) = 0;
    virtual TDiagArranjo*   __fastcall PM_GetDGArranjo(void) = 0;
    virtual TDiagBarra*     __fastcall PM_GetDGBarra(void) = 0;
    virtual TDiagBIso*      __fastcall PM_GetDGBIso(void) = 0;
    virtual TDiagCabo*      __fastcall PM_GetDGCabo(void) = 0;
    virtual TDiagCapacitor* __fastcall PM_GetDGCapacitor(void) = 0;
    virtual TDiagCarga*     __fastcall PM_GetDGCarga(void) = 0;
    virtual TDiagChave*     __fastcall PM_GetDGChave(void) = 0;
    virtual TDiagRede*      __fastcall PM_GetDGRede(void) = 0;
    virtual TDiagReg*       __fastcall PM_GetDGReg(void) = 0;
    virtual TDiagTrecho*    __fastcall PM_GetDGTrecho(void) = 0;
    virtual TDiagTrafo*     __fastcall PM_GetDGTrafo(void) = 0;
};

//---------------------------------------------------------------------------
VTDiagTopo* __fastcall NewObjDiagTopo(VTApl *apl_owner, VTProgresso *progresso);

//---------------------------------------------------------------------------
#endif
//eof

