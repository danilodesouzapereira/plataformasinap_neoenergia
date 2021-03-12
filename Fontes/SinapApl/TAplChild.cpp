// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <System.SysUtils.hpp>
#include "TAplChild.h"
#include "..\Alerta\VTAlerta.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\BD\VTBD.h"
#include "..\BD\Rede\VTBDgeral.h"
#include "..\BD\VTSNPDBConn.h"
#include "..\BDihm\VTInsere.h"
#include "..\Busca\VTBusca.h"
#include "..\Classes\VTClasses.h"
#include "..\Cronometro\VTCronometro.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Diretorio\VTPath.h"
#include "..\Edita\VTEdita.h"
#include "..\Figura\VTFiguras.h"
#include "..\Flow\VTFlow.h"
#include "..\Geral\VTGeral.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Manobra\VTManobras.h"
#include "..\Manobra\VTLocks.h"
#include "..\Mapa\VTMapas.h"
#include "..\Mapa\VTRegioes.h"
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
#include "..\..\DLL_Inc\BDihm.h"
#include "..\..\DLL_Inc\Busca.h"
#include "..\..\DLL_Inc\Classes.h"
#include "..\..\DLL_Inc\Cronometro.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Diretorio.h"
#include "..\..\DLL_Inc\Edita.h"
#include "..\..\DLL_Inc\Figura.h"
#include "..\..\DLL_Inc\Flow.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Geral.h"
#include "..\..\DLL_Inc\Grafico.h"
#include "..\..\DLL_Inc\Manobra.h"
#include "..\..\DLL_Inc\Mapa.h"
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

// ---------------------------------------------------------------------------
#pragma package(smart_init)

//https://sinapsisenergia.teamwork.com/#/tasks/16384467
// ---------------------------------------------------------------------------
__fastcall TAplChild::TAplChild(TForm *form_owner, VTApl *apl_owner, TAction *ActionInsRede)
	: TApl(form_owner, apl_owner)
{
	// variáveis locais
	VTEstudo *estudo;
	VTPlanejamento *planejamento;
	VTPath *path = (VTPath*)apl_owner->GetObject(__classid(VTPath));
	AnsiString dir_base = path->DirBase;
	AnsiString arq_rede = path->ArqRede;
	AnsiString arq_rede_original = path->DirRedes() + "\\Rede.mdb";

    if(!DirectoryExists(path->DirRedes()))
	{
		// Cria o diretório
		ForceDirectories(path->DirRedes());
	}
	if(!FileExists(arq_rede_original))
	{
		CriarArqRede(arq_rede_original);
    }
	// verifica se existe o arquivo da Rede
	if (!FileExists(arq_rede))
	{
		//arq_rede = path->DirRedes() + "\\DBNAME_REDE";
		arq_rede = arq_rede_original;
		// Deve verificar primeiro se o diretório Redes existe
	}

	// cria e inicia objeto VTPath
	Add(path = DLL_NewObjPath());
	path->DirBase = dir_base;
	path->ArqRede = arq_rede;
	// cria objetos básicos
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
	Add(DLL_NewObjMapas(this));
	Add(DLL_NewObjFiguras(this));
	// ===================================
	// eliminar Regioes depois que alterar base de dados para trabalhar com Mapas
	Add(DLL_NewObjRegioes(this));
	// ===================================
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
	// cria objeto Insere p/ expansão de redes reduzidas
	Add(DLL_NewObjInsere(form_owner, this, ActionInsRede));
	// IMPORTANTE: insere Estudo da rede original
	planejamento->InsereEstudoRedeOriginal();
	// inicia objetos a partir da base de dados
	IniciaObjetosBasicos();
}

// ---------------------------------------------------------------------------
__fastcall TAplChild::~TAplChild(void)
{
	// nada a fazer: a classe base TApl destrói todos os obejtos criados
}

// ---------------------------------------------------------------------------
void __fastcall TAplChild::IniciaObjetosBasicos(void)
{
	// variáveis locais
	VTBD *bd;
	VTGeral *geral = (VTGeral*)GetObject(__classid(VTGeral));

	try
	{ // cria objeto BD
		bd = DLL_NewObjBD(this);
		bd->Conecta();
		// reinicia objetos a partir da nova base de dados
		bd->Geral->LeGeral(geral);
		bd->Geral->LeTipos();
		// reinicia configuração de VTGeral que depende de dados lidos da base
		// geral->CfgPerdaTec->IniciaValoresDefault();
		delete bd;
	}
	catch (Exception &e)
	{
	}
}

// ---------------------------------------------------------------------------
void __fastcall TAplChild::CriarArqRede(AnsiString arq_rede)
{
	VTPath *path = (VTPath*)apl_owner->GetObject(__classid(VTPath));
	AnsiString arq_rede_empresa = path->ArqRedeEmpresa();
	AnsiString txt = "";

	CopyFile(((WideString)arq_rede_empresa).c_bstr(), ((WideString)arq_rede).c_bstr(), false);
    // Se não foi possível criar o arq rede.mdb vai dar um aviso para o usuário
//	if (!FileExists(arq_rede))
//	{
//		txt = "O arquivo " + path->DirRedes() + " não pode ser encontrado.\n";
//		txt += "Verifique que a instalação do programa e alocação de licenças foram realizadas corretamente\n" ;
//		Aviso(txt);
//	}
}
// ---------------------------------------------------------------------------
// eof

