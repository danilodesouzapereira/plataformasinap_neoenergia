#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <Rede.h>
#pragma argsused

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_DeleteEqpto(VTEqpto *eqpto)
   {
   }

//---------------------------------------------------------------------------
EXPORT VTArea* __fastcall DLL_NewObjArea(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTBarra* __fastcall DLL_NewObjBarra(int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTBateria* __fastcall DLL_NewObjBateria(int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTCapacitor* __fastcall DLL_NewObjCapacitor(int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTCapserie* __fastcall DLL_NewObjCapserie(int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTCarga* __fastcall DLL_NewObjCarga(int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTCargaNL* __fastcall DLL_NewObjCargaNL(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTChave* __fastcall DLL_NewObjChave(int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTCluster* __fastcall DLL_NewObjCluster(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTComentario* __fastcall DLL_NewObjComentario(VTEqpto *eqpto)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTCorte* __fastcall DLL_NewObjCorte(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTDominio*    __fastcall DLL_NewObjDominio(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTFases* __fastcall DLL_NewObjFases(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTFiltro* __fastcall DLL_NewObjFiltro(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTGerador* __fastcall DLL_NewObjGerador(int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTGrupo* __fastcall DLL_NewObjGrupo(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTListeqp* __fastcall DLL_NewObjListeqp(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTMatC* __fastcall DLL_NewObjMatC(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTMatY* __fastcall DLL_NewObjMatY(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTMatZ* __fastcall DLL_NewObjMatZ(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTMedidor* __fastcall DLL_NewObjMedidor(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTMNet* __fastcall DLL_NewObjMNet(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTMotor* __fastcall DLL_NewObjMotor(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTMRede* __fastcall DLL_NewObjMRede(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTMunicipio* __fastcall DLL_NewObjMunicipio(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTMutua* __fastcall DLL_NewObjMutua(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTNet* __fastcall DLL_NewObjNet(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTNo* __fastcall DLL_NewObjNo(VTBarra *pbarra, VTNet *net)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTReator* __fastcall DLL_NewObjReator(int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTRede* __fastcall DLL_NewObjRede(int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTRedes* __fastcall DLL_NewObjRedes(VTApl *apl)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTReducao* __fastcall DLL_NewObjReducao(int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTRegulador* __fastcall DLL_NewObjRegulador(int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTResFlowBar* __fastcall DLL_NewObjResFlowBar(VTBarra *barra, int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTResFlowLig* __fastcall DLL_NewObjResFlowLig(VTLigacao *ligacao, int num_bar, int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTResFlowSup* __fastcall DLL_NewObjResFlowSup(VTEqbar *eqbar, int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTSuprimento* __fastcall DLL_NewObjSuprimento(int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTTipoChave* __fastcall DLL_NewObjTipoChave(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTTipoRede* __fastcall DLL_NewObjTipoRede(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTTipos* __fastcall DLL_NewObjTipos(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTTrafo* __fastcall DLL_NewObjTrafo(int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTTrafo* __fastcall DLL_NewObjTrafoMono(int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTTrafo3E* __fastcall DLL_NewObjTrafo3E(int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTTrafoZZ* __fastcall DLL_NewObjTrafoZZ(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTTramo* __fastcall DLL_NewObjTramo(int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTTrecho* __fastcall DLL_NewObjTrecho(int num_pat)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
EXPORT VTYref* __fastcall DLL_NewObjYref(void)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof

