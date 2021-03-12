//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Oracle.hpp>
#include <Math.hpp>
#include "TBDfunc.h"
#include "TBDhistOracle.h"
#include "..\Apl\VTApl.h"
#include "..\Arsesp\VTArsesp.h"
#include "..\BD\VTSNPDBConn.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurva.h"
#include "..\Log\VTLog.h"
#include "..\Patamar\VTPatamares.h"
#include "..\PerdaTec\VTBalanco.h"
#include "..\PerdaTec\VTBalancos.h"
#include "..\PerdaTec\VTPerda.h"
#include "..\PerdaTec\VTPerdas.h"
#include "..\PerdaTec\VTResumo.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTRadial.h"
#include "..\Radial\VTSecundario.h"
#include "..\Radial\VTSubestacao.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
VTBDhist* NewObjBDhistOracle(VTApl *apl_owner, TOracleSession* ptrOracleSession)
   {
   return(new TBDhistOracle(apl_owner, ptrOracleSession));
   }

//---------------------------------------------------------------------------
__fastcall TBDhistOracle::TBDhistOracle(VTApl *apl_owner, TOracleSession* ptrOracleSession)
   {
   AnsiString strlog;

   //salva ponteiros
   this->apl        = apl_owner;
   this->bd_session = ptrOracleSession;

   if ((plog = (VTLog*)apl->GetObject(__classid(VTLog))) == NULL)
      {
      SNPSTRLOGLINE(strlog);
      }
   }

//---------------------------------------------------------------------------
__fastcall TBDhistOracle::~TBDhistOracle(void)
   {
   //destrói objetos
   }

//---------------------------------------------------------------------------
bool __fastcall TBDhistOracle::ArvoreContemNo(VTArvore* arvore, AnsiString codigo)
	{
	return(false);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDhistOracle::InsereArvore(VTArvore* arvore)
	{
	return(false);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDhistOracle::InsereDensidadeCarga(VTArvore* arvore)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDhistOracle::InserePerdaTecnica(VTArvore* arvore)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDhistOracle::InsereRede(TList* lisPRI)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDhistOracle::LeArvoreDensidadeCarga(VTArvore* arvore, int intLevel)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDhistOracle::LeArvorePerdaTecnica(VTArvore* arvore, int intLevel)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool  __fastcall TBDhistOracle::LeListaArvore(TList* lstArvore)
   {
   return(false);
   }


