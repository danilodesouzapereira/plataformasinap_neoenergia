//---------------------------------------------------------------------------
#ifndef TDiagTopoH
#define TDiagTopoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTDiagTopo.h"
//---------------------------------------------------------------------------
class VTProgresso;
class VTArvore;
//---------------------------------------------------------------------------
class TDiagTopo : public VTDiagTopo
{
public:         //User declarations
             __fastcall  TDiagTopo(VTApl *apl_owner, VTProgresso *progresso);
             __fastcall ~TDiagTopo(void);
    void     __fastcall  Inicia(void);
    void     __fastcall  LeCfgDiagEqpto(int tipo_DGEQPTO);
    void     __fastcall  PreExecuta(void);
    void     __fastcall  SalvaCfgDiagEqpto(int tipo_DGEQPTO);

private:        //metodos
    void     __fastcall  AgregaLisBarras(TList *lisRede, TList *lisCarga);
    void     __fastcall  ListaRedesAlimentadas(VTRede *supridora, TList *lisEXT);
    void     __fastcall  PreparaRedeDiag(void);
    void     __fastcall  PreparaRedeUso(void);
    void     __fastcall  PreparaSecundarios(void);
    void     __fastcall  ZonasIndefParaNULL(void);


private: //objetos externos
    VTApl *apl;
private: //objetos internos
    VTProgresso *progresso;
    VTArvore    *arvore;

protected:  //métodos
    bool            __fastcall PM_GetRedePreparada(void);
    TDiagArranjo*   __fastcall PM_GetDGArranjo(void);
    TDiagBarra*     __fastcall PM_GetDGBarra(void);
    TDiagBIso*      __fastcall PM_GetDGBIso(void);
    TDiagCabo*      __fastcall PM_GetDGCabo(void);
    TDiagCapacitor* __fastcall PM_GetDGCapacitor(void);
    TDiagCarga*     __fastcall PM_GetDGCarga(void);
    TDiagChave*     __fastcall PM_GetDGChave(void);
    TDiagRede*      __fastcall PM_GetDGRede(void);
    TDiagReg*       __fastcall PM_GetDGReg(void);
    TDiagTrecho*    __fastcall PM_GetDGTrecho(void);
    TDiagTrafo*     __fastcall PM_GetDGTrafo(void);

protected:  //objetos locais
    bool            redePreparada;
    TDiagArranjo    *diagArranjo;
    TDiagBarra      *diagBarra;
    TDiagBIso       *diagBIso;
    TDiagCabo       *diagCabo;
    TDiagCapacitor  *diagCapacitor;
    TDiagCarga      *diagCarga;
    TDiagChave      *diagChave;
    TDiagRede       *diagRede;
    TDiagReg        *diagReg;
    TDiagTrecho     *diagTrecho;
    TDiagTrafo      *diagTrafo;
};

//---------------------------------------------------------------------------
#endif
//eof

