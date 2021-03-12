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
#include "..\Protesw\VTProtesw.h"
#include "..\Rede\VTTipos.h"
#include "..\..\DLL_Inc\Arranjo.h"
#include "..\..\DLL_Inc\BD.h"
#include "..\..\DLL_Inc\Diretorio.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Geral.h"
#include "..\..\DLL_Inc\Licenca.h"
#include "..\..\DLL_Inc\Patamar.h"
#include "..\..\DLL_Inc\Protesw.h"
#include "..\..\DLL_Inc\Rede.h"
#include "..\..\DLL_Inc\SinapToolKit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TAplMdi::TAplMdi(TForm *form_owner, VTApl *apl_owner)
                   :TApl(form_owner, apl_owner)
   {
   //cria objeto Basico
   Add(NewObjBasico());
   //cria objeto Produto
   Add(DLL_NewObjProduto(produtoSINAP_GRID));
   //cria objetos básicos
   Add(DLL_NewObjPath());
   Add(DLL_NewObjDir(this));
   //cria demais objetos
   Add(DLL_NewObjProtesw());
   Add(DLL_NewObjLicencas(this));
   Add(DLL_NewObjPatamares(this));
   Add(DLL_NewObjDragdrop());
   Add(DLL_NewObjGeral(this));
	}

//---------------------------------------------------------------------------
__fastcall TAplMdi::~TAplMdi(void)
   {
   //nada a fazer: a classe base TApl destrói todos os obejtos criados
   }

//---------------------------------------------------------------------------
bool __fastcall TAplMdi::DeleteObject(TMetaClass* meta_class)
   {
   //variáveis locais
   TObject *object;

   //obtém objeto
   if ((object = GetObject(meta_class)) == NULL) return(false);
   Remove(object);
   delete object;

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TAplMdi::IniciaObjetosBasicos(void)
   {
   //variáveis locais
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
