//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TAplChild.h"
#include "..\Alerta\VTAlerta.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\BD\VTBD.h"
#include "..\BD\Rede\VTBDgeral.h"
#include "..\BD\VTSNPDBConn.h"
#include "..\Busca\VTBusca.h"
#include "..\Classes\VTClasses.h"
#include "..\Cronometro\VTCronometro.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Diretorio\VTPath.h"
#include "..\Edita\VTEdita.h"
#include "..\Flow\VTFlow.h"
#include "..\Geral\VTGeral.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Manobra\VTManobras.h"
#include "..\Manobra\VTLocks.h"
#include "..\Mercado\VTMercado.h"
#include "..\MontaZ\VTMontaZ.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Potencia\VTFornecimento.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTTipos.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\Redegraf\VTCelulas.h"
#include "..\Topologia\VTTopologia.h"
#include "..\Zona\VTZonas.h"
#include "..\Wizard\VTWizard.h"
#include "..\..\DLL_Inc\Alerta.h"
#include "..\..\DLL_Inc\Arranjo.h"
#include "..\..\DLL_Inc\BD.h"
#include "..\..\DLL_Inc\Busca.h"
#include "..\..\DLL_Inc\Classes.h"
#include "..\..\DLL_Inc\Cronometro.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Diretorio.h"
#include "..\..\DLL_Inc\Edita.h"
#include "..\..\DLL_Inc\Flow.h"
#include "..\..\DLL_Inc\Geral.h"
#include "..\..\DLL_Inc\Grafico.h"
#include "..\..\DLL_Inc\Manobra.h"
#include "..\..\DLL_Inc\Mercado.h"
#include "..\..\DLL_Inc\MontaZ.h"
#include "..\..\DLL_Inc\Patamar.h"
#include "..\..\DLL_Inc\Planejamento.h"
#include "..\..\DLL_Inc\Potencia.h"
#include "..\..\DLL_Inc\Rede.h"
#include "..\..\DLL_Inc\Redegraf.h"
#include "..\..\DLL_Inc\SinapApl.h"
#include "..\..\DLL_Inc\Topologia.h"
#include "..\..\DLL_Inc\Zona.h"
#include "..\..\DLL_Inc\Wizard.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TAplChild::TAplChild(TForm *form_owner, VTApl *apl_owner) //, TAction *ActionInsRede)
                     :TApl(form_owner, apl_owner)
   {
   //variáveis locais
   VTEstudo       *estudo;
	VTPlanejamento *planejamento;
   VTPath         *path   = (VTPath*)apl_owner->GetObject(__classid(VTPath));
   AnsiString     dirBase = path->DirBase();
   AnsiString     dirRede = path->DirRedeBase();

   //verifica se existe o diretório da rede
   if (! DirectoryExists(dirRede)) dirRede = dirBase;
   //cria e inicia objeto VTPath
	Add(path = DLL_NewObjPath());
   path->DefineDirBase(dirBase);
   path->DefineDirRede(dirRede);
	//cria objetos básicos
	Add(DLL_NewObjAlerta(this));
	Add(DLL_NewObjClasses());
   Add(DLL_NewObjGeral(this));
	Add(DLL_NewObjMontaZ(this));
	Add(DLL_NewObjCurvas(this));
	Add(DLL_NewObjArranjos());
	Add(DLL_NewObjTipos());
	Add(DLL_NewObjMercado(this));
	//
   Add(DLL_NewObjPatamares(this));
	Add(DLL_NewObjRedes(this));
   Add(DLL_NewObjFases());
	Add(DLL_NewObjRedegraf(this));
   Add(DLL_NewObjCelulas(this));
	Add(DLL_NewObjGrafico(this));
   Add(DLL_NewObjBusca(this));
	Add(DLL_NewObjLocks(this));
   Add(DLL_NewObjManobras(this));
	Add(DLL_NewObjDemanda(this));
   Add(DLL_NewObjFornecimento(this));
	Add(DLL_NewObjEdita(this));
   Add(DLL_NewObjTopologia(this));
   Add(DLL_NewObjZonas(this));
	Add(DLL_NewObjFlow(this));
   Add(DLL_NewObjWizard(this));
	Add(DLL_NewObjCronometro(this));
	Add(planejamento = DLL_NewObjPlanejamento(this));
   //cria objeto Insere p/ expansão de redes reduzidas
int OOPS_Retirado_DLL_NewObjInsere=1;
   /*
   // OOPS: Modificado em 20120611
   MessageDlg("Retirado DLL_NewObjInsere(this, ActionInsRede) de TAplChild::TAplChild", mtInformation, TMsgDlgButtons() << mbOK, 0);
   // Add(DLL_NewObjInsere(this, ActionInsRede));
   */
   //IMPORTANTE: insere Estudo da rede original
   planejamento->InsereEstudoRedeOriginal();
   //inicia objetos a partir da base de dados
   IniciaObjetosBasicos();
   }

//---------------------------------------------------------------------------
__fastcall TAplChild::~TAplChild(void)
   {
   //nada a fazer: a classe base TApl destrói todos os obejtos criados
   }

//---------------------------------------------------------------------------
void __fastcall TAplChild::IniciaObjetosBasicos(void)
   {
   //variáveis locais
	VTBD       *bd;
   VTGeral    *geral = (VTGeral*)GetObject(__classid(VTGeral));

   try{//cria objeto BD
		bd = DLL_NewObjBD(this);
      bd->Conecta();
      //reinicia objetos a partir da nova base de dados
      bd->Geral->LeGeral(geral);
      bd->Geral->LeTipos();
      //reinicia configuração de VTGeral que depende de dados lidos da base
      //geral->CfgPerdaTec->IniciaValoresDefault();
      delete bd;
      }
   catch(Exception &e)
      {
      }
   }
//---------------------------------------------------------------------------
//eof
