#include <vcl.h>
#include <windows.h>
#pragma hdrstop
//Fontes
#include <Fontes\Rede\VTArea.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTBateria.h>
#include <Fontes\Rede\VTCapacitor.h>
#include <Fontes\Rede\VTCapserie.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Rede\VTCargaNL.h>
#include <Fontes\Rede\VTChave.h>
#include <Fontes\Rede\VTCluster.h>
#include <Fontes\Rede\VTComentario.h>
#include <Fontes\Rede\VTCorte.h>
#include <Fontes\Rede\VTDominio.h>
#include <Fontes\Rede\VTFases.h>
#include <Fontes\Rede\VTFiltro.h>
#include <Fontes\Rede\VTGerador.h>
#include <Fontes\Rede\VTGrupo.h>
#include <Fontes\Rede\VTListeqp.h>
#include <Fontes\Rede\VTMatC.h>
#include <Fontes\Rede\VTMatY.h>
#include <Fontes\Rede\VTMatZ.h>
#include <Fontes\Rede\VTMedidor.h>
#include <Fontes\Rede\VTMNet.h>
#include <Fontes\Rede\VTMotor.h>
#include <Fontes\Rede\VTMRede.h>
#include <Fontes\Rede\VTMunicipio.h>
#include <Fontes\Rede\VTMutua.h>
#include <Fontes\Rede\VTNet.h>
#include <Fontes\Rede\VTNo.h>
#include <Fontes\Rede\VTReator.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTReducao.h>
#include <Fontes\Rede\VTRegulador.h>
#include <Fontes\Rede\VTResFlowBar.h>
#include <Fontes\Rede\VTResFlowLig.h>
#include <Fontes\Rede\VTResFlowSup.h>
#include <Fontes\Rede\VTSuprimento.h>
#include <Fontes\Rede\VTTipoChave.h>
#include <Fontes\Rede\VTTipoRede.h>
#include <Fontes\Rede\VTTipos.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Rede\VTTrafo3E.h>
#include <Fontes\Rede\VTTrafoZZ.h>
#include <Fontes\Rede\VTTrafo.h>
#include <Fontes\Rede\VTTramo.h>
#include <Fontes\Rede\VTTrecho.h>
#include <Fontes\Rede\VTYref.h>
//DLL_Inc
#include <Rede.h>
#pragma argsused

#pragma comment(lib, "Funcao.a")

//-----------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
	{
	return 1;
	}

//---------------------------------------------------------------------------
EXPORT void __fastcall DLL_DeleteEqpto(VTEqpto *eqpto)
   {
   try{
      if (eqpto) delete eqpto;
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
EXPORT VTArea* __fastcall DLL_NewObjArea(void)
   {
	return(NewObjArea());
   }

//---------------------------------------------------------------------------
EXPORT VTBarra* __fastcall DLL_NewObjBarra(int num_pat)
   {
   return(NewObjBarra(num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTBateria* __fastcall DLL_NewObjBateria(int num_pat)
   {
   return(NewObjBateria(num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTCapacitor* __fastcall DLL_NewObjCapacitor(int num_pat)
   {
   return(NewObjCapacitor(num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTCapserie* __fastcall DLL_NewObjCapserie(int num_pat)
   {
   return(NewObjCapserie(num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTCarga* __fastcall DLL_NewObjCarga(int num_pat)
   {
   return(NewObjCarga(num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTCargaNL* __fastcall DLL_NewObjCargaNL(void)
   {
   return(NewObjCargaNL());
   }

//---------------------------------------------------------------------------
EXPORT VTChave* __fastcall DLL_NewObjChave(int num_pat)
   {
   return(NewObjChave(num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTCluster* __fastcall DLL_NewObjCluster(void)
   {
   return(NewObjCluster());
   }

//---------------------------------------------------------------------------
EXPORT VTComentario* __fastcall DLL_NewObjComentario(VTEqpto *eqpto)
   {
   return(NewObjComentario(eqpto));
   }

//---------------------------------------------------------------------------
EXPORT VTCorte* __fastcall DLL_NewObjCorte(void)
   {
   return(NewObjCorte());
   }

//---------------------------------------------------------------------------
EXPORT VTDominio* __fastcall DLL_NewObjDominio(void)
   {
   return(NewObjDominio());
   }

//---------------------------------------------------------------------------
EXPORT VTGerador* __fastcall DLL_NewObjGerador(int num_pat)
   {
   return(NewObjGerador(num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTGrupo* __fastcall DLL_NewObjGrupo(void)
   {
   return(NewObjGrupo());
   }

//---------------------------------------------------------------------------
EXPORT VTListeqp* __fastcall DLL_NewObjListeqp(void)
   {
   return(NewObjListeqp());
   }

//---------------------------------------------------------------------------
EXPORT VTMunicipio* __fastcall DLL_NewObjMunicipio(void)
   {
   return(NewObjMunicipio());
   }

//---------------------------------------------------------------------------
EXPORT VTFases* __fastcall DLL_NewObjFases(void)
   {
   return(NewObjFases());
   }

//---------------------------------------------------------------------------
EXPORT VTFiltro* __fastcall DLL_NewObjFiltro(void)
   {
   return(NewObjFiltro());
   }

//---------------------------------------------------------------------------
EXPORT VTMatC* __fastcall DLL_NewObjMatC(void)
   {
   return(NewObjMatC());
   }

//---------------------------------------------------------------------------
EXPORT VTMatY* __fastcall DLL_NewObjMatY(void)
   {
   return(NewObjMatY());
   }

//---------------------------------------------------------------------------
EXPORT VTMatZ* __fastcall DLL_NewObjMatZ(void)
   {
   return(NewObjMatZ());
   }

//---------------------------------------------------------------------------
EXPORT VTMedidor* __fastcall DLL_NewObjMedidor(void)
   {
   return(NewObjMedidor());
   }

//---------------------------------------------------------------------------
EXPORT VTMNet* __fastcall DLL_NewObjMNet(void)
   {
   return(NewObjMNet());
   }

//---------------------------------------------------------------------------
EXPORT VTMotor* __fastcall DLL_NewObjMotor(void)
   {
   return(NewObjMotor());
   }

//---------------------------------------------------------------------------
EXPORT VTMRede* __fastcall DLL_NewObjMRede(void)
   {
   return(NewObjMRede());
   }

//---------------------------------------------------------------------------
EXPORT VTMutua* __fastcall DLL_NewObjMutua(void)
   {
   return(NewObjMutua());
   }

//---------------------------------------------------------------------------
EXPORT VTNet* __fastcall DLL_NewObjNet(void)
   {
   return(NewObjNet());
   }

//---------------------------------------------------------------------------
EXPORT VTNo* __fastcall DLL_NewObjNo(VTBarra *pbarra, VTNet *net)
   {
   return(NewObjNo(pbarra, net));
   }

//---------------------------------------------------------------------------
EXPORT VTReator* __fastcall DLL_NewObjReator(int num_pat)
   {
   return(NewObjReator(num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTRede* __fastcall DLL_NewObjRede(int num_pat)
   {
   return(NewObjRede(num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTRedes* __fastcall DLL_NewObjRedes(VTApl *apl)
   {
   return(NewObjRedes(apl));
   }

//---------------------------------------------------------------------------
EXPORT VTReducao* __fastcall DLL_NewObjReducao(int num_pat)
   {
   return(NewObjReducao(num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTRegulador* __fastcall DLL_NewObjRegulador(int num_pat)
   {
   return(NewObjRegulador(num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTResFlowBar* __fastcall DLL_NewObjResFlowBar(VTBarra *barra, int num_pat)
   {
   return(NewObjResFlowBar(barra, num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTResFlowLig* __fastcall DLL_NewObjResFlowLig(VTLigacao *ligacao, int num_bar, int num_pat)
   {
   return(NewObjResFlowLig(ligacao, num_bar, num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTResFlowSup* __fastcall DLL_NewObjResFlowSup(VTEqbar *eqbar, int num_pat)
   {
   return(NewObjResFlowSup(eqbar, num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTSuprimento* __fastcall DLL_NewObjSuprimento(int num_pat)
   {
   return(NewObjSuprimento(num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTTipoChave* __fastcall DLL_NewObjTipoChave(void)
   {
   return(NewObjTipoChave());
   }

//---------------------------------------------------------------------------
EXPORT VTTipoRede* __fastcall DLL_NewObjTipoRede(void)
   {
   return(NewObjTipoRede());
   }

//---------------------------------------------------------------------------
EXPORT VTTipos* __fastcall DLL_NewObjTipos(void)
   {
   return(NewObjTipos());
   }

//---------------------------------------------------------------------------
EXPORT VTTrafo* __fastcall DLL_NewObjTrafo(int num_pat)
   {
   return(NewObjTrafo(num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTTrafo* __fastcall DLL_NewObjTrafoMono(int num_pat)
   {
   return(NewObjTrafoMono(num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTTrafo3E* __fastcall DLL_NewObjTrafo3E(int num_pat)
   {
   return(NewObjTrafo3E(num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTTrafoZZ* __fastcall DLL_NewObjTrafoZZ(void)
   {
   return(NewObjTrafoZZ());
   }

//---------------------------------------------------------------------------
EXPORT VTTramo* __fastcall DLL_NewObjTramo(int num_pat)
   {
   return(NewObjTramo(num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTTrecho* __fastcall DLL_NewObjTrecho(int num_pat)
   {
   return(NewObjTrecho(num_pat));
   }

//---------------------------------------------------------------------------
EXPORT VTYref* __fastcall DLL_NewObjYref(void)
   {
   return(NewObjYref());
   }

//---------------------------------------------------------------------------
//eof

