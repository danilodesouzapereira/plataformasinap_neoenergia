//---------------------------------------------------------------------------
#ifndef RedeH
#define RedeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "Export.h"

//---------------------------------------------------------------------------
class VTApl;
class VTArea;
class VTBarra;
class VTBateria;
class VTCapacitor;
class VTCapserie;
class VTCarga;
class VTCargaNL;
class VTChave;
class VTCluster;
class VTComentario;
class VTCorte;
class VTDominio;
class VTEqbar;
class VTEqpto;
class VTFases;
class VTFiltro;
class VTGerador;
class VTGrupo;
class VTLigacao;
class VTListeqp;
class VTMatC;
class VTMatY;
class VTMatZ;
class VTMedidor;
class VTMNet;
class VTMotor;
class VTMRede;
class VTMunicipio;
class VTMutua;
class VTNet;
class VTNo;
class VTReator;
class VTRede;
class VTRedes;
class VTReducao;
class VTRegulador;
class VTResFlowBar;
class VTResFlowLig;
class VTResFlowSup;
class VTSuprimento;
class VTTipoChave;
class VTTipoRede;
class VTTipos;
class VTTrafo;
class VTTrafo3E;
class VTTrafoZZ;
class VTTramo;
class VTTrecho;
class VTYref;

//-----------------------------------------------------------------------------
// funções da DLL
//-----------------------------------------------------------------------------
EXPORT void          __fastcall DLL_DeleteEqpto(VTEqpto *eqpto);
EXPORT VTArea*       __fastcall DLL_NewObjArea(void);
EXPORT VTBarra*      __fastcall DLL_NewObjBarra(int num_pat);
EXPORT VTBateria* 	 __fastcall DLL_NewObjBateria(int num_pat);
EXPORT VTCapacitor*  __fastcall DLL_NewObjCapacitor(int num_pat);
EXPORT VTCapserie*   __fastcall DLL_NewObjCapserie(int num_pat);
EXPORT VTCarga*      __fastcall DLL_NewObjCarga(int num_pat);
EXPORT VTCargaNL*    __fastcall DLL_NewObjCargaNL(void);
EXPORT VTChave*      __fastcall DLL_NewObjChave(int num_pat);
EXPORT VTCluster*    __fastcall DLL_NewObjCluster(void);
EXPORT VTComentario* __fastcall DLL_NewObjComentario(VTEqpto *eqpto);
EXPORT VTCorte*      __fastcall DLL_NewObjCorte(void);
EXPORT VTDominio*    __fastcall DLL_NewObjDominio(void);
EXPORT VTFases*      __fastcall DLL_NewObjFases(void);
EXPORT VTFiltro*     __fastcall DLL_NewObjFiltro(void);
EXPORT VTGerador*    __fastcall DLL_NewObjGerador(int num_pat);
EXPORT VTGrupo*      __fastcall DLL_NewObjGrupo(void);
EXPORT VTListeqp*    __fastcall DLL_NewObjListeqp(void);
EXPORT VTMatC*       __fastcall DLL_NewObjMatC(void);
EXPORT VTMatY*       __fastcall DLL_NewObjMatY(void);
EXPORT VTMatZ*       __fastcall DLL_NewObjMatZ(void);
EXPORT VTMedidor*    __fastcall DLL_NewObjMedidor(void);
EXPORT VTMNet*       __fastcall DLL_NewObjMNet(void);
EXPORT VTMotor*      __fastcall DLL_NewObjMotor(void);
EXPORT VTMRede*      __fastcall DLL_NewObjMRede(void);
EXPORT VTMunicipio*  __fastcall DLL_NewObjMunicipio(void);
EXPORT VTMutua*      __fastcall DLL_NewObjMutua(void);
EXPORT VTNet*        __fastcall DLL_NewObjNet(void);
EXPORT VTNo*         __fastcall DLL_NewObjNo(VTBarra *pbarra, VTNet *net);
EXPORT VTReator*     __fastcall DLL_NewObjReator(int num_pat);
EXPORT VTRede*       __fastcall DLL_NewObjRede(int num_pat);
EXPORT VTRedes*      __fastcall DLL_NewObjRedes(VTApl *apl);
EXPORT VTReducao*    __fastcall DLL_NewObjReducao(int num_pat);
EXPORT VTRegulador*  __fastcall DLL_NewObjRegulador(int num_pat);
EXPORT VTResFlowBar* __fastcall DLL_NewObjResFlowBar(VTBarra *barra, int num_pat);
EXPORT VTResFlowLig* __fastcall DLL_NewObjResFlowLig(VTLigacao *ligacao, int num_bar, int num_pat);
EXPORT VTResFlowSup* __fastcall DLL_NewObjResFlowSup(VTEqbar *eqbar, int num_pat);
EXPORT VTSuprimento* __fastcall DLL_NewObjSuprimento(int num_pat);
EXPORT VTTipoChave*  __fastcall DLL_NewObjTipoChave(void);
EXPORT VTTipoRede*   __fastcall DLL_NewObjTipoRede(void);
EXPORT VTTipos*      __fastcall DLL_NewObjTipos(void);
EXPORT VTTrafo*      __fastcall DLL_NewObjTrafo(int num_pat);
EXPORT VTTrafo*      __fastcall DLL_NewObjTrafoMono(int num_pat);
EXPORT VTTrafo3E*    __fastcall DLL_NewObjTrafo3E(int num_pat);
EXPORT VTTrafoZZ*    __fastcall DLL_NewObjTrafoZZ(void);
EXPORT VTTramo*      __fastcall DLL_NewObjTramo(int num_pat);
EXPORT VTTrecho*     __fastcall DLL_NewObjTrecho(int num_pat);
EXPORT VTYref*       __fastcall DLL_NewObjYref(void);

//---------------------------------------------------------------------------
#endif
//eof


 