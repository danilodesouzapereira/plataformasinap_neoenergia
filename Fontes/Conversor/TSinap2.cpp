//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjos.h>
#include <Fontes\BD\VTBD.h>
#include <Fontes\BDado_Rev2\VTBD_arranjo.h>
#include <Fontes\BDado_Rev2\VTBD_classe.h>
#include <Fontes\BDado_Rev2\VTBD_curva.h>
#include <Fontes\BDado_Rev2\VTBD_geral.h>
#include <Fontes\BDado_Rev2\VTBD_mercado.h>
#include <Fontes\BDado_Rev2\VTBD_rede.h>
#include <Fontes\Diretorio\VTDir.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\Classes\VTClasses.h>
#include <Fontes\Curvas\VTCurvas.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Mercado\VTMercado.h>
#include <Fontes\Rede\VTRedes.h>
#include <Fontes\Rede\VTTipos.h>
#include <Fontes\Topologia\VTTopologia.h>
#include <Fontes\Zona\VTZonas.h>
#include <DLL_Inc\Abre_Rev2.h>
#include <DLL_Inc\BDado_Rev2.h>
#include <DLL_Inc\Diretorio.h>
#include <DLL_Inc\Geral.h>
#include "TSinap2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTSinap2* __fastcall NewObjSinap2(TComponent *Owner, VTApl *apl_owner)
   {
   return(new TSinap2(Owner, apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TSinap2::TSinap2(TComponent *Owner, VTApl *apl_owner)
   {
   apl = NewObjApl(NULL, apl_owner);
   //cria objetos básicos e insere em Apl
   apl->Add(DLL_NewObjPath());
   //apl->Add(DLL_NewObjDir(apl));
   //apl->Add(DLL_NewObjGeral(apl));
   //apl->Add(DLL_NewObjBD_rede(apl));
   }

//---------------------------------------------------------------------------
__fastcall TSinap2::~TSinap2(void)
   {
   //destrói listas sem destruir seus objetos
   if (apl)  {delete apl;  apl  = NULL;}
	}

//---------------------------------------------------------------------------
bool __fastcall TSinap2::AbreRedeCompleta(AnsiString dir_base, AnsiString dir_rede)
   {
   //variáveis locais
   VTPath  *path = (VTPath*)apl->GetObject(__classid(VTPath));
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //inicia diretórios de VTDir e VTPath
   //dir->DirRede = dir_rede;
   path->DefineDirBase(dir_base);
   path->DefineDirRede(dir_rede);
   //reinicia Redes
   redes->Clear();
   //cria objeto apl e demais objetos básicos
   IniciaObjetosBasicos();
   //carrega todas as Redes da base indicada
   CarregaRedeCompleta();

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TSinap2::CarregaRedeCompleta(void)
   {
   //variáveis locais
   bool sucesso;
   VTZonas     *zonas     = (VTZonas*)apl->GetObject(__classid(VTZonas));
   VTTopologia *topologia = (VTTopologia*)apl->GetObject(__classid(VTTopologia));

   try{//cria objeto VTAbre
      sucesso = DLL_BaseAccessCarregaRedeCompleta(NULL, apl);
      zonas->Executa();
      topologia->Reconfigura();
      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
void __fastcall TSinap2::IniciaObjetosBasicos(void)
   {
   //variáveis locais
   VTBD_arranjo *bd_arranjo;
   VTBD_classe  *bd_classe;
   VTBD_curva   *bd_curva;
   VTBD_geral   *bd_geral;
   VTBD_mercado *bd_mercado;
   VTBD_rede    *bd_rede;
   VTArranjos   *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   VTClasses    *classes  = (VTClasses*)apl->GetObject(__classid(VTClasses));
   VTCurvas     *curvas   = (VTCurvas*)apl->GetObject(__classid(VTCurvas));
   VTGeral      *geral    = (VTGeral*)apl->GetObject(__classid(VTGeral));
   VTMercado    *mercado  = (VTMercado*)apl->GetObject(__classid(VTMercado));

   try{//reinicia valores default do objeto Geral
      geral->ReiniciaValoresDefault();
      //define base de dados Access
      geral->BaseDado = SNPDBTYPE_ACCESS;
      //lê Geral
      bd_geral = DLL_NewObjBD_geral(apl);
      bd_geral->LeGeral(geral);
      delete bd_geral;
      //lê Classes
      //classes->Clear();
      bd_classe = DLL_NewObjBD_classe(apl);
      bd_classe->LeClasse();
      delete bd_classe;
      //lê mercado
      mercado->Reinicia();
      bd_mercado = DLL_NewObjBD_mercado(apl);
      bd_mercado->Le();
      delete bd_mercado;
      //lê Arranjos
      arranjos->Clear();
      bd_arranjo = DLL_NewObjBD_arranjo(apl);
      bd_arranjo->LeArranjo();
      delete bd_arranjo;
      //lê Curvas típicas
      //curvas->Clear();
      bd_curva = DLL_NewObjBD_curva(apl);
      bd_curva->LeCurvaTipicaCapacitor();
      bd_curva->LeCurvaTipicaCarga();
      bd_curva->LeCurvaTipicaGeradorFotovoltaico();
      bd_curva->LeCurvaTipicaIP();
      bd_curva->LeCurvaTipicaReator();
      delete bd_curva;
      //lê TipoChave e TipoRede
      bd_rede = DLL_NewObjBD_rede(apl);
      bd_rede->LeListaTipoEqpto();
      delete bd_rede;
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
//eof
