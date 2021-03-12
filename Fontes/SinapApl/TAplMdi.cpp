//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAplMdi.h"
#include "VTBasico.h"
#include "..\BD\VTBD.h"
#include "..\BD\Rede\VTBDgeral.h"
#include "..\BD\VTSNPDBConn.h"
#include "..\Diretorio\VTDir.h"
#include "..\Diretorio\VTPath.h"
#include "..\Editor\VTDragdrop.h"
#include "..\Geral\VTGeral.h"
#include "..\Licenca\VTLicencas.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Produto\VTProduto.h"
#include "..\Progresso\VTAborta.h"
#include "..\Protesw\VTProtesw.h"
#include "..\RBAC\VTRBACManager.h"
#include "..\Rede\VTMunicipio.h"
#include "..\Rede\VTTipos.h"
#include "..\..\DLL_Inc\Arranjo.h"
#include "..\..\DLL_Inc\BD.h"
#include "..\..\DLL_Inc\Diretorio.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Geral.h"
#include "..\..\DLL_Inc\Licenca.h"
#include "..\..\DLL_Inc\Patamar.h"
#include "..\..\DLL_Inc\Produto.h"
#include "..\..\DLL_Inc\Protesw.h"
#include "..\..\DLL_Inc\RBAC.h"
#include "..\..\DLL_Inc\Rede.h"
#include "..\..\DLL_Inc\SinapToolKit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/18251640
__fastcall TAplMdi::TAplMdi(TForm *form_owner, VTApl *apl_owner)
                   :TApl(form_owner, apl_owner)
   {
   //cria objeto Basico
   Add(NewObjBasico());
   //cria objeto Produto
   Add(DLL_NewObjProduto());
   //cria objetos b�sicos
   Add(DLL_NewObjPath());
   Add(DLL_NewObjDir(this));
   //cria demais objetos
   Add(DLL_NewObjProtesw());
   Add(DLL_NewObjLicencas(this));
   Add(DLL_NewObjPatamares(this));
   Add(DLL_NewObjDragdrop());
   Add(DLL_NewObjGeral(this));
   Add(DLL_NewObjRBACManager(this));
   Add(DLL_NewObjMunicipio());
   //cria objeto aborta
   Add(new VTAborta());
   }

//---------------------------------------------------------------------------
__fastcall TAplMdi::~TAplMdi(void)
   {
   //nada a fazer: a classe base TApl destr�i todos os obejtos criados
   }

//---------------------------------------------------------------------------
bool __fastcall TAplMdi::DeleteObject(TMetaClass* meta_class)
   {
   //vari�veis locais
   TObject *object;

   //obt�m objeto
   if ((object = GetObject(meta_class)) == NULL) return(false);
   Remove(object);
   delete object;

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TAplMdi::IniciaObjetosBasicos(void)
   {
   //vari�veis locais
   VTBD      *bd;
   VTGeral   *geral = (VTGeral*)GetObject(__classid(VTGeral));

   //reinicia objetos VTGeral e VTMercado
   geral->ReiniciaValoresDefault();
   try
      {//verifica a  base de dados selecionada
      bd = DLL_NewObjBD(this);
      bd->Conecta();
      bd->Geral->LeGeral(geral);
      delete bd;
      }
   catch(Exception &e)
      {
      }
   }
//---------------------------------------------------------------------------
//eof
