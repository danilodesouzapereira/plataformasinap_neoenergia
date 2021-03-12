//---------------------------------------------------------------------------
#include <vcl.h>
#include <ADODB.hpp>
#include <DB.hpp>
#pragma hdrstop
#include "TBDfunc.h"
#include "TBDgeralOracle.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Arranjo\VTCabo.h"
#include "..\Arranjo\VTPtoFixacao.h"
#include "..\Arranjo\VTSuporte.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
//#include "..\Cronometro\VTCronometro.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTPadraoRede.h"
#include "..\Importa\Monta\VTMonta.h"
#include "..\Log\VTLog.h"
#include "..\Mapa\VTRegiao.h"
#include "..\Mapa\VTRegioes.h"
#include "..\Mercado\VTCenario.h"
#include "..\Mercado\VTCrescimento.h"
#include "..\Mercado\VTMercado.h"
#include "..\Mercado\VTSegmento.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTDominio.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Geral.h"
#include "..\..\DLL_Inc\Rede.h"
#include "..\..\DLL_Inc\Classes.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Mapa.h"
#include "..\..\DLL_Inc\Mercado.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTBDgeral* NewObjBDgeralOracle(VTApl *apl_owner, TOracleSession* ptrOracleSession)
   {
   return(new TBDgeralOracle(apl_owner, ptrOracleSession));
   }
/*
//---------------------------------------------------------------------------
__fastcall TBDgeralOracle::TBDgeralOracle(VTApl *apl_owner, TOracleSession* ptrOracleSession)
   {
   AnsiString strlog;

   //salva ponteiro
   apl     = apl_owner;
   bd_session = ptrOracleSession;

   plog    = (VTLog*)apl->GetObject(__classid(VTLog));

   if ((progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso))) == NULL)
      {
      SNPSTRLOGLINE(strlog);
      }

   if ((tipos = (VTTipos*)apl->GetObject(__classid(VTTipos))) == NULL)
      {
      MSGERRSYS_TIPOSINV
      SNPSTRLOGLINE(strlog);
      }

   if ((monta = (VTMonta*)apl->GetObject(__classid(VTMonta))) == NULL)
      {
      MSGERRSYS_MONTAINV
      SNPSTRLOGLINE(strlog);
      }

   if ((bd_func = (TBDfunc*)apl->GetObject(__classid(TBDfunc))) == NULL)
      {
      MSGERRSYS_BDFUNCINV
      SNPSTRLOGLINE(strlog);
      }

   if ((arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos))) == NULL)
      {
      MSGERRSYS_ARRANJOSINV
      SNPSTRLOGLINE(strlog);
      }

   if ((classes = (VTClasses*)apl->GetObject(__classid(VTClasses))) == NULL)
      {
      MSGERRSYS_CLASSESINV
      SNPSTRLOGLINE(strlog);
      }

   //o objeto VTCronometo pode não existir
   //cronometro = (VTCronometro*)apl->GetObject(__classid(VTCronometro));
   }
*/
//---------------------------------------------------------------------------
__fastcall TBDgeralOracle::TBDgeralOracle(VTApl *apl_owner, TOracleSession* ptrOracleSession)
   {
   AnsiString strlog;

   //salva ponteiro
   apl     = apl_owner;
   bd_session = ptrOracleSession;

   plog    = (VTLog*)apl->GetObject(__classid(VTLog));
   progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
   tipos = (VTTipos*)apl->GetObject(__classid(VTTipos));
   monta = (VTMonta*)apl->GetObject(__classid(VTMonta));
   bd_func = (TBDfunc*)apl->GetObject(__classid(TBDfunc));
   arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   classes = (VTClasses*)apl->GetObject(__classid(VTClasses));
   //o objeto VTCronometo pode não existir
   //cronometro = (VTCronometro*)apl->GetObject(__classid(VTCronometro));
   }


//---------------------------------------------------------------------------
__fastcall TBDgeralOracle::~TBDgeralOracle(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::LeArranjo(void)
   {
   //lê Suportes
   SPSNP_rdSuporte();
   //lê cabos
   SPSNP_rdCabo();
   //lê arranjos
   SPSNP_rdArranjo_00_01();
   SPSNP_rdArranjo_02_04();
   SPSNP_rdArranjo_03();

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::LeCfgFluxo(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::LeCurvaTipica(void)
   {
   return(SPSNP_rdConjCurvaTipica());
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::LeMercado(void)
   {
   AnsiString strlog;
   VTMercado* mercado;

   if ((mercado = (VTMercado*)apl->GetObject(__classid(VTMercado))) == NULL)
      {
      MSGERRSYS_MERCADOINV
      SNPSTRLOGLINE(strlog);
      }
   mercado->Reinicia();
   return(SPSNP_rdMercado(mercado));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::LeTipoChave(void)
   {
   //variáveis locais
   AnsiString strlog;

   if ((bd_session == NULL)||(!bd_session->Connected))
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }
   if (!SPSNP_rdTipoChave())
      {
      SNPSTRLOGLINE(strlog);
      MSGERRBDRD_TIPOCHAVE
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::LeTipoConsumidor(void)
   {
   //variáveis locais
   AnsiString strlog;

   if ((bd_session == NULL)||(!bd_session->Connected))
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }
   if (!SPSNP_rdTipoConsumidor())
      {
      SNPSTRLOGLINE(strlog);
      MSGERRBDRD_TIPOCONSUMIDOR
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::LeTipoRede(void)
   {
   //variáveis locais
   AnsiString strlog;

   if ((bd_session == NULL)||(!bd_session->Connected))
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }
   if (!SPSNP_rdTipoRede())
      {
      SNPSTRLOGLINE(strlog);
      MSGERRBDRD_TIPOREDE
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::LeTipos(void)
   {
   //variáveis locais
   AnsiString strlog;

   if ((bd_session == NULL)||(!bd_session->Connected))
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!SPSNP_rdTipoConsumidor()) { SNPSTRLOGLINE(strlog); MSGERRBDRD_TIPOCONSUMIDOR return(false); }
   if (!SPSNP_rdTipoChave())      { SNPSTRLOGLINE(strlog); MSGERRBDRD_TIPOCHAVE      return(false); }
   if (!SPSNP_rdTipoRede())       { SNPSTRLOGLINE(strlog); MSGERRBDRD_TIPOREDE       return(false); }
   if (!LeArranjo())              { SNPSTRLOGLINE(strlog); MSGERRBDRD_ARRANJO        return(false); }
   if (!LeCurvaTipica())          { SNPSTRLOGLINE(strlog); MSGERRBDRD_CURTIPICAS     return(false); }
   if (!LeMercado())              { SNPSTRLOGLINE(strlog); MSGERRBDRD_MERCADO        return(false); }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::LimpezaAuxiliares(void)
   {
   return(SPSNP_dlLimpezaAuxiliares());
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::LimpezaEstudos(void)
   {
   return(SPSNP_dlLimpezaEstudos());
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::RetiraCaboSuporteArranjo(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::RetiraSegmentoMercado(VTSegmento* segmento)
	{
   return(SPSNP_dlSegmentoMercado(segmento));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::RetiraTipoChave(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::RetiraTipoRede(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SalvaTipoChave(bool bolResetaStatus)
   {
   //o parâmetro bolResetaStatus é utilizado apenas no Access, por enquanto.
   return(SPSNP_wrTipoChave(tipos->LisTipoChave()));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SalvaTipoConsumidor(bool bolResetaStatus)
   {
   return(SPSNP_wrTipoConsumidor());
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SalvaTipoRede(bool bolResetaStatus)
   {
   //o parâmetro bolResetaStatus é utilizado apenas no Access, por enquanto.
   return(SPSNP_wrTipoRede(tipos->LisTipoRede()));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SalvaTipos(bool bolResetaStatus)
   {
   AnsiString strlog;

   if (!tipos)                             { SNPSTRLOGLINE(strlog); MSGERRBDWR_ARRANJOS return(false); }
   if (!SalvaArranjo(bolResetaStatus))     { SNPSTRLOGLINE(strlog); MSGERRBDWR_ARRANJOS return(false); }
   if (!SalvaCurvaTipica(bolResetaStatus)) { SNPSTRLOGLINE(strlog); MSGERRBDWR_CURVAS   return(false); }
   if (!SalvaTipoChave(bolResetaStatus))   { SNPSTRLOGLINE(strlog); MSGERRBDWR_ARRANJOS return(false); }
   if (!SalvaTipoRede(bolResetaStatus))    { SNPSTRLOGLINE(strlog); MSGERRBDWR_ARRANJOS return(false); }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_rdTipoChave(void)
   {
   //variáveis locais
   bool          sucesso = true;
   int           tipochave_id;
   AnsiString    strlog;
   VTTipoChave*  tipochave;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdTipoChave(:p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor"  , otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            tipochave_id = ptrCursor->Field("intTipoChaveId");
            if (! tipos->ExisteTipoChave(tipochave_id))
               {//cria objeto VTTipoChave
               if ((tipochave = DLL_NewObjTipoChave()) == NULL) return(false);
               //lê dados do TipoChave
               tipochave->Id               = tipochave_id;
               tipochave->Codigo           = ptrCursor->Field("strDescricao");
               tipochave->Tipo             = ptrCursor->Field("intTipoChaveId");
               tipochave->StatusAsUnsigned = ptrCursor->Field("intStatus");
               // OOPS@20130625: Colocar os campos do tipo de chave aqui
               // tipochave->cor          = ptrCursor->Field("intCorId");
               //insere TipoChave Redes
               tipos->InsereTipoChave(tipochave);
               }
            ptrCursor->Next();
            }
         ptrCursor->Close();
         ptrQry->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrCursor) { delete ptrCursor; ptrCursor = NULL; }
      if (ptrQry)    { delete ptrQry;    ptrQry    = NULL; }
      }

   return(sucesso);
   } // SPSNP_rdTipoChave

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_rdTipoRede(int intEstudoId)
   {
   //variáveis locais
   bool          sucesso = true;
   int           tipo_rede_id;
   strTIPO_REDE  str_tipo_rede;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   if (!tipos)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_TIPOSINV
      return(false);
      }

   if (!monta)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_MONTAINV
      return(false);
      }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdTipoRede(:p_EstudoId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor"  , otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);
         ptrQry->DeclareVariable("p_EstudoId", otInteger);

         // Seta valores dos parâmetros
         ptrQry->SetVariable("p_EstudoId", intEstudoId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            str_tipo_rede.id        = ptrCursor->Field("intTipoRedeId");
            str_tipo_rede.segmento  = str_tipo_rede.id;
            str_tipo_rede.codigo    = ptrCursor->Field("strCodigo");
            str_tipo_rede.descricao = ptrCursor->Field("strDescricao");
            str_tipo_rede.qtde_rede = ptrCursor->Field("intQuantidade");
            str_tipo_rede.vmin_kv   = ptrCursor->Field("fltVmin");
            str_tipo_rede.vmax_kv   = ptrCursor->Field("fltVmax");
            str_tipo_rede.color     = TColor(int(ptrCursor->Field("intColorId")));
            str_tipo_rede.status    = ptrCursor->Field("intStatus");
            if (! monta->InsereTipoRede(str_tipo_rede))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDRD_TIPOREDE2
               sucesso = false;
               break;
               }
            ptrCursor->Next();
            }
         ptrCursor->Close();
         ptrQry->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrCursor) { delete ptrCursor; ptrCursor = NULL; }
      if (ptrQry)    { delete ptrQry;    ptrQry    = NULL; }
      }

   return(sucesso);
   } // SPSNP_rdTipoRede

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_rdTipoConsumidor(void)
   {
   //variáveis locais
   bool          sucesso = true;
   VTClasse*     classe;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   if (!classes)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_CLASSESINV
      return(false);
      }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdTipoConsumidor(:p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor"  , otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         // Seta valores dos parâmetros

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            if ((classe = DLL_NewObjClasse()) == NULL) continue;
            // lê dados da Classe
            classe->Id     = ptrCursor->Field("TipoConsId");
            classe->Tag    = ptrCursor->Field("Tag");
            classe->Codigo = ptrCursor->Field("Descricao");
            classe->Tipico = bool(int(ptrCursor->Field("Tipico")));
            classes->InsereClasse(classe);

            ptrCursor->Next();
            }
         ptrCursor->Close();
         ptrQry->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrCursor) { delete ptrCursor; ptrCursor = NULL; }
      if (ptrQry)    { delete ptrQry;    ptrQry    = NULL; }
      }

   return(sucesso);
   } // SPSNP_rdTipoConsumidor

//---------------------------------------------------------------------------
VTCabo* __fastcall TBDgeralOracle::SPSNP_rdCabo(int intCaboId)
   {
   //variáveis locais
   VTCabo*       cabo = NULL;
   AnsiString    strlog;
   strCABO       str_cabo;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(NULL);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdCabo(:p_CaboId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor"  , otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);
         ptrQry->DeclareVariable("p_CaboId", otInteger);

         // Seta valores dos parâmetros
         ptrQry->SetVariable("p_CaboId", intCaboId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();

         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_cabo.id = ptrCursor->Field("CaboId");
            str_cabo.tipo_cfg  = ptrCursor->Field("TipoCaboId");
            str_cabo.codigo    = ptrCursor->Field("Descricao");
//mml@20130730
            str_cabo.extern_id = ptrCursor->Field("ExtId");
            str_cabo.iadm      = ptrCursor->Field("Inom");
            str_cabo.vmin      = ptrCursor->Field("Vmin");
            str_cabo.vmax      = ptrCursor->Field("Vmax");
            //
            // OOPS. Falta um campo para status
            //
            if (str_cabo.tipo_cfg == caboR_GMR)
               {
               // Cabo RGMR (ini)
               str_cabo.r = ptrCursor->Field("RGMR_R");
               str_cabo.gmr = ptrCursor->Field("RGMR_GMR");

               str_cabo.z0.r = ptrCursor->Field("Z0Z1_Z0R"); // Base retorna zero
               str_cabo.z0.x = ptrCursor->Field("Z0Z1_Z0X"); // Base retorna zero
               str_cabo.z1.r = ptrCursor->Field("Z0Z1_Z1R"); // Base retorna zero
               str_cabo.z1.x = ptrCursor->Field("Z0Z1_Z1X"); // Base retorna zero
               str_cabo.c0   = ptrCursor->Field("Z0Z1_C0");  // Base retorna zero
               str_cabo.c1   = ptrCursor->Field("Z0Z1_C1");  // Base retorna zero
               // Cabo RGMR (fim)
               }
            else
               {
               // Cabo Z0Z1 (ini)
               str_cabo.r = ptrCursor->Field("RGMR_R");      // Base retorna zero
               str_cabo.gmr = ptrCursor->Field("RGMR_GMR");  // Base retorna zero

               str_cabo.z0.r = ptrCursor->Field("Z0Z1_Z0R");
               str_cabo.z0.x = ptrCursor->Field("Z0Z1_Z0X");
               str_cabo.z1.r = ptrCursor->Field("Z0Z1_Z1R");
               str_cabo.z1.x = ptrCursor->Field("Z0Z1_Z1X");
               str_cabo.c0   = ptrCursor->Field("Z0Z1_C0");
               str_cabo.c1   = ptrCursor->Field("Z0Z1_C1");
               // Cabo Z0Z1 (fim)
               }

            //insere em Monta
            if ((cabo = (VTCabo*)monta->InsereCabo(str_cabo)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               SNPSTRLOG(str_cabo.codigo);
               MSGERRBDRD_CABO
               break;
               }
            else
               {
               cabo->DefineStatus(sttNOVO, false);
               cabo->DefineStatus(sttALTERADO, false);
               }

            ptrCursor->Next();
            }
         ptrCursor->Close();
         ptrQry->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrCursor) { delete ptrCursor; ptrCursor = NULL; }
      if (ptrQry)    { delete ptrQry;    ptrQry    = NULL; }
      }

   return(cabo);
   } // SPSNP_rdCabo

//---------------------------------------------------------------------------
VTSuporte* __fastcall TBDgeralOracle::SPSNP_rdSuporte(int intSuporteId)
   {
   //variáveis locais
   VTSuporte*    suporte = NULL;
   AnsiString    strlog;
   strSUPORTE    str_suporte;
   int           intTipoSuporteId;
   int           intPontosTotal;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(NULL);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdSuporte(:p_SuporteId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor"  , otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);
         ptrQry->DeclareVariable("p_SuporteId", otInteger);

         // Seta valores dos parâmetros
         ptrQry->SetVariable("p_SuporteId", intSuporteId);

         // Inicia a estrutura
         str_suporte.num_pto = 0;

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {

            // loop registros (begin)
            if (str_suporte.num_pto == 0)
               {
               intPontosTotal = ptrCursor->Field("NumPontosFix");
               if (intPontosTotal == 0)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDRD_NUMPONTOSFIX
                  break;
                  }
               if (intPontosTotal >= SIZEOF_VALOR)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDRD_NUMPONTOSFIX
                  break;
                  }
               }

            str_suporte.pto_fixacao[str_suporte.num_pto].id      = ptrCursor->Field("FixacaoItem");
            str_suporte.pto_fixacao[str_suporte.num_pto].coord_x = ptrCursor->Field("FixacaoX");
            str_suporte.pto_fixacao[str_suporte.num_pto].coord_y = ptrCursor->Field("FixacaoY");
            str_suporte.pto_fixacao[str_suporte.num_pto].codigo  = ptrCursor->Field("FixacaoCodigo");
            str_suporte.num_pto++;

            if (str_suporte.num_pto == intPontosTotal)
               {
               str_suporte.id        = ptrCursor->Field("SuporteId");
               str_suporte.codigo    = ptrCursor->Field("Descricao");
//mml@20130730
               str_suporte.extern_id = ptrCursor->Field("ExtId");
               str_suporte.vmin      = ptrCursor->Field("Vmin");
               str_suporte.vmax      = ptrCursor->Field("Vmax");
               str_suporte.tipo      = ptrCursor->Field("TipoSuporteId");
               //
               // OOPS. Falta um campo para status
               //

               //insere em Monta
               if ((suporte = (VTSuporte*)monta->InsereSuporte(str_suporte)) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDRD_SUPORTE
                  break;
                  }
               else
                  {
                  suporte->DefineStatus(sttNOVO, false);
                  suporte->DefineStatus(sttALTERADO, false);
                  }
               str_suporte.num_pto = 0;
               }

            ptrCursor->Next();
            // loop registros (end)
            }

         ptrCursor->Close();
         ptrQry->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrCursor) { delete ptrCursor; ptrCursor = NULL; }
      if (ptrQry)    { delete ptrQry;    ptrQry    = NULL; }
      }

   return(suporte);
   } // SPSNP_rdSuporte(int intCaboId)

//---------------------------------------------------------------------------
int __fastcall TBDgeralOracle::SPSNP_rdSuporteUtilizado(int intSuporteId)
   {
   //variáveis locais
   AnsiString    strlog;
   int           intNumVezes;
   TOracleQuery* ptrQry = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdSuporteUtilizado(:p_SuporteId, :p_NumVezes);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NumVezes" , otInteger);
         ptrQry->DeclareVariable("p_SuporteId", otInteger);

         // Seta valores dos parâmetros
         ptrQry->SetVariable("p_SuporteId", intSuporteId);

         // Executa query
         ptrQry->Execute();

         // Número de vezes que o suporte foi utilizado
         intNumVezes = ptrQry->GetVariable("p_NumVezes");

         // Fecha o cursor
         ptrQry->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(intNumVezes);
   } // SPSNP_rdSuporteUtilizado(int intSuporteId)

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_rdSuporteUtilizado(int intSuporteId, TStringList* arrlist)
   {
   //variáveis locais
   bool          bolret = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;
   int           intArranjoId;
   AnsiString    strArranjoDescr;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   if (!arrlist) return(false);
   arrlist->Clear();

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdSuporteUtilizado(:p_SuporteId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_SuporteId", otInteger);
         ptrQry->DeclareVariable("p_cursor"  , otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         // Seta valores dos parâmetros
         ptrQry->SetVariable("p_SuporteId", intSuporteId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            intArranjoId = ptrCursor->Field("ArranjoId");
            strArranjoDescr = ptrCursor->Field("ArranjoDescricao");
            arrlist->AddObject(strArranjoDescr, (TObject*)intArranjoId);

            ptrCursor->Next();
            }
         ptrCursor->Close();
         ptrQry->Close();

         // Indica se o suporte está sendo utilizado, ou não
         arrlist->Count > 0 ? bolret = true : bolret = false;
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         bolret = true; // A condição segura é indicar que o suporte está sendo utilizado
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrCursor) { delete ptrCursor; ptrCursor = NULL; }
      if (ptrQry)    { delete ptrQry;    ptrQry    = NULL; }
      }

   return(bolret);
   } // SPSNP_rdSuporteUtilizado(int intSuporteId, TStringList* arrlist)

//---------------------------------------------------------------------------
VTArranjo* __fastcall TBDgeralOracle::SPSNP_rdArranjo_00_01(int intArranjoId)
   {
   //variáveis locais
   VTArranjo*    arranjo = NULL;
   AnsiString    strlog;
   strARRANJO2   str_arranjo2;
   int           intFaseId;
   int           intCabosTotal;
   int           intCabosConta;
   int           ndxFase;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(NULL);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdArranjo_00_01(:p_ArranjoId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor"  , otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);
         ptrQry->DeclareVariable("p_ArranjoId", otInteger);

         // Seta valores dos parâmetros
         ptrQry->SetVariable("p_ArranjoId", intArranjoId);

         intCabosConta = 0;

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            str_arranjo2.tipo_cfg = ptrCursor->Field("TipoArranjoId");

            // loop registros (begin)
            if (intCabosConta == 0)
               {
               intCabosTotal = ptrCursor->Field("NumCabos");

               // Carrega estruturas de dados com valores inválidos
               memset(&(str_arranjo2.cfg_suporte_cabo), IDINDEFINIDO, sizeof(str_arranjo2.cfg_suporte_cabo));
               memset(&(str_arranjo2.cfg_caboZOZ1), IDINDEFINIDO, sizeof(str_arranjo2.cfg_caboZOZ1));
               }

            if (str_arranjo2.tipo_cfg == arranjoCABOxFASE)
               {
               // Arranjo de cabos monopolares (ini)
               intFaseId = ptrCursor->Field("TipoFaseId");
               switch (intFaseId)
                  {
                  case faseA    : ndxFase = 0;
                                  break;
                  case faseB    : ndxFase = 1;
                                  break;
                  case faseC    : ndxFase = 2;
                                  break;
                  case faseN    : ndxFase = 3;
                                  break;
                  default       : ndxFase = IDINDEFINIDO;
                  }

               if (ndxFase != IDINDEFINIDO)
                  {
                  str_arranjo2.cfg_suporte_cabo.suporte_id = ptrCursor->Field("SuporteId");
                  str_arranjo2.cfg_suporte_cabo.fase[ndxFase].cabo_id        = ptrCursor->Field("CaboId");
                  str_arranjo2.cfg_suporte_cabo.fase[ndxFase].pto_fixacao_id = ptrCursor->Field("FixacaoItem");
                  str_arranjo2.cfg_suporte_cabo.fase[ndxFase].multiplicidade = 1; // !OOPS! Sempre 1 mesmo?
                  }
               else
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDRD_ARRANJO
                  break;
                  }
               // Arranjo de cabos monopolares (fim)
               }
            else
               {
               // Arranjo de cabo multiplexado (ini)
               str_arranjo2.cfg_caboZOZ1.cabo_id    = ptrCursor->Field("CaboId");
               str_arranjo2.cfg_caboZOZ1.suporte_id = ptrCursor->Field("SuporteId");
               // Arranjo de cabo multiplexado (fim)
               }
            intCabosConta++;

            if (intCabosConta == intCabosTotal)
               {
               str_arranjo2.arranjo_id = ptrCursor->Field("ArranjoId");
               str_arranjo2.tipo_cfg   = ptrCursor->Field("TipoArranjoId");
               str_arranjo2.codigo     = ptrCursor->Field("Descricao");
//mml@20130730
               str_arranjo2.extern_id  = ptrCursor->Field("ExtId");
               str_arranjo2.tipico     = bool(int(ptrCursor->Field("Tipico")));
               str_arranjo2.fases      = ptrCursor->Field("ArranjoTipoFaseId");
               str_arranjo2.res_terra  = ptrCursor->Field("ResTerra");
               str_arranjo2.vmin_kv    = ptrCursor->Field("Vmin");
               str_arranjo2.vmax_kv    = ptrCursor->Field("Vmax");
               str_arranjo2.color      = (TColor)(int)ptrCursor->Field("ColorId");
               str_arranjo2.status     = ptrCursor->Field("Status");
					//JCG@2015.01.05: extrai informação de ramal do Satus
					str_arranjo2.ramal      = ((str_arranjo2.status & sttRAMAL) == sttRAMAL);
					//str_arranjo2.data     = (TDateTime&)ptrCursor->Field("DataVersao");
               str_arranjo2.data       = TDateTime(double(ptrCursor->Field("DataVersao")));

               //insere em Monta
               if ((arranjo = (VTArranjo*)monta->InsereArranjo(str_arranjo2)) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  SNPSTRLOG(str_arranjo2.codigo);
                  MSGERRBDRD_ARRANJO
                  break;
                  }
               else
                  {
                  arranjo->DefineStatus(sttNOVO, false);
                  arranjo->DefineStatus(sttALTERADO, false);
                  }

               intCabosConta = 0;
               }

            ptrCursor->Next();
            // loop registros (end)
            }
         ptrCursor->Close();
         ptrQry->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrCursor) { delete ptrCursor; ptrCursor = NULL; }
      if (ptrQry)    { delete ptrQry;    ptrQry    = NULL; }
      }

   return(arranjo);
   } // SPSNP_rdArranjo_00_01

//---------------------------------------------------------------------------
VTArranjo* __fastcall TBDgeralOracle::SPSNP_rdArranjo_02_04(int intArranjoId)
   {
   //variáveis locais
   VTArranjo*    arranjo = NULL;
   AnsiString    strlog;
   strARRANJO2   str_arranjo2;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(NULL);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdArranjo_02_04(:p_ArranjoId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor"  , otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);
         ptrQry->DeclareVariable("p_ArranjoId", otInteger);

         // Seta valores dos parâmetros
         ptrQry->SetVariable("p_ArranjoId", intArranjoId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)

            // Carrega estruturas de dados com valores inválidos
            memset(&(str_arranjo2.cfg_arranjoZ0Z1), IDINDEFINIDO, sizeof(str_arranjo2.cfg_arranjoZ0Z1));
            memset(&(str_arranjo2.cfg_puSB100), IDINDEFINIDO, sizeof(str_arranjo2.cfg_puSB100));

            str_arranjo2.arranjo_id = ptrCursor->Field("ArranjoId");
            str_arranjo2.tipo_cfg   = ptrCursor->Field("TipoArranjoId");
            str_arranjo2.codigo     = ptrCursor->Field("Descricao");
//mml@20130730
            str_arranjo2.extern_id  = ptrCursor->Field("ExtId");
            str_arranjo2.tipico     = bool(int(ptrCursor->Field("Tipico")));
            str_arranjo2.fases      = ptrCursor->Field("ArranjoTipoFaseId");
            str_arranjo2.res_terra  = ptrCursor->Field("ResTerra");
            str_arranjo2.vmin_kv    = ptrCursor->Field("Vmin");
            str_arranjo2.vmax_kv    = ptrCursor->Field("Vmax");
            str_arranjo2.color      = (TColor)(int)ptrCursor->Field("ColorId");
            str_arranjo2.status     = (TColor)(int)ptrCursor->Field("Status");
				//JCG@2015.01.05: extrai informação de ramal do Satus
				str_arranjo2.ramal      = ((str_arranjo2.status & sttRAMAL) == sttRAMAL);
				//str_arranjo2.data     = (TDateTime&)ptrCursor->Field("DataVersao");
			   str_arranjo2.data       = TDateTime(double(ptrCursor->Field("DataVersao")));

            if (str_arranjo2.tipo_cfg == arranjoZ0Z1)
               {
               // Arranjo Z0Z1 (ini)
               str_arranjo2.cfg_arranjoZ0Z1.suporte_id = ptrCursor->Field("SuporteId");
               str_arranjo2.cfg_arranjoZ0Z1.z0.r       = ptrCursor->Field("Z0R");
               str_arranjo2.cfg_arranjoZ0Z1.z0.x       = ptrCursor->Field("Z0X");
               str_arranjo2.cfg_arranjoZ0Z1.z1.r       = ptrCursor->Field("Z1R");
               str_arranjo2.cfg_arranjoZ0Z1.z1.x       = ptrCursor->Field("Z1X");
               str_arranjo2.cfg_arranjoZ0Z1.c0         = ptrCursor->Field("C0");
               str_arranjo2.cfg_arranjoZ0Z1.c1         = ptrCursor->Field("C1");
               str_arranjo2.cfg_arranjoZ0Z1.iadm_amp   = ptrCursor->Field("Iadm");
               // Arranjo Z0Z1 (fim)
               }
            else
               {
               // Arranjo PUSB100 (ini)
               str_arranjo2.cfg_puSB100.suporte_id = ptrCursor->Field("SuporteId");
               str_arranjo2.cfg_puSB100.z0.r       = ptrCursor->Field("Z0R");
               str_arranjo2.cfg_puSB100.z0.x       = ptrCursor->Field("Z0X");
               str_arranjo2.cfg_puSB100.z1.r       = ptrCursor->Field("Z1R");
               str_arranjo2.cfg_puSB100.z1.x       = ptrCursor->Field("Z1X");
               str_arranjo2.cfg_puSB100.c0         = ptrCursor->Field("C0");
               str_arranjo2.cfg_puSB100.c1         = ptrCursor->Field("C1");
               // Arranjo PUSB100 (fim)
               }

            // insere em Monta o úlimo registro (ou o único, caso somente tenha um regsitro de arranjo)
            if ((arranjo = monta->InsereArranjo(str_arranjo2)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               }
            else
               {
               arranjo->DefineStatus(sttNOVO, false);
               arranjo->DefineStatus(sttALTERADO, false);
               }

            ptrCursor->Next();
            }
         ptrCursor->Close();
         ptrQry->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrCursor) { delete ptrCursor; ptrCursor = NULL; }
      if (ptrQry)    { delete ptrQry;    ptrQry    = NULL; }
      }

   return(arranjo);
   } // SPSNP_rdArranjo_02_04

//---------------------------------------------------------------------------
VTArranjo* __fastcall TBDgeralOracle::SPSNP_rdArranjo_03(int intArranjoId)
   {
   //variáveis locais
   VTArranjo*    arranjo = NULL;
   AnsiString    strlog;
   strARRANJO2   str_arranjo2;
   TStream*      ptrStream = NULL;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(NULL);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(NULL);
      }

   if (!bd_func)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_BDFUNCINV
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdArranjo_03(:p_ArranjoId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor"  , otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);
         ptrQry->DeclareVariable("p_ArranjoId", otInteger);

         // Seta valores dos parâmetros
         ptrQry->SetVariable("p_ArranjoId", intArranjoId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)

            // Carrega estruturas de dados com valores inválidos
            memset(&(str_arranjo2.cfg_matZ), IDINDEFINIDO, sizeof(str_arranjo2.cfg_matZ));

            str_arranjo2.arranjo_id          = ptrCursor->Field("ArranjoId");
            str_arranjo2.tipo_cfg            = ptrCursor->Field("TipoArranjoId");
            str_arranjo2.codigo              = ptrCursor->Field("Descricao");
//mml@20130730
            str_arranjo2.extern_id           = ptrCursor->Field("ExtId");
            str_arranjo2.tipico              = bool(int(ptrCursor->Field("Tipico")));
            str_arranjo2.fases               = ptrCursor->Field("ArranjoTipoFaseId");
            str_arranjo2.res_terra           = ptrCursor->Field("ResTerra");
            str_arranjo2.vmin_kv             = ptrCursor->Field("Vmin");
            str_arranjo2.vmax_kv             = ptrCursor->Field("Vmax");
            str_arranjo2.color               = (TColor)(int)ptrCursor->Field("ColorId");
            str_arranjo2.status              = (TColor)(int)ptrCursor->Field("Status");
				//JCG@2015.01.05: extrai informação de ramal do Satus
				str_arranjo2.ramal               = ((str_arranjo2.status & sttRAMAL) == sttRAMAL);
            str_arranjo2.cfg_matZ.iadm_amp   = ptrCursor->Field("Iadm");
            //str_arranjo2.data              = (TDateTime&)ptrCursor->Field("DataVersao");
            str_arranjo2.data                = TDateTime(double(ptrCursor->Field("DataVersao")));

            ptrStream = ptrCursor->LOBField("MatZ");
            if (!ptrStream)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDRD_MATZARRANJO
               break;
               }
            if (!bd_func->DesmontaStreamVTMatZ(ptrStream, str_arranjo2.cfg_matZ.mat_z))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDRD_MATZARRANJO
               break;
               }

            ptrStream = ptrCursor->LOBField("MatC");
            if (!ptrStream)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDRD_MATCARRANJO
               break;
               }
            if (!bd_func->DesmontaStreamVTMatC(ptrStream, str_arranjo2.cfg_matZ.mat_c))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDRD_MATCARRANJO
               break;
               }

            // insere em Monta o último registro (ou o único, caso somente tenha um regsitro de arranjo)
            if ((arranjo = monta->InsereArranjo(str_arranjo2)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDRD_ARRANJO
               break;
               }
            else
               {
               arranjo->DefineStatus(sttNOVO, false);
               arranjo->DefineStatus(sttALTERADO, false);
               }

            ptrCursor->Next();
            // loop registros (end)
            }
         ptrCursor->Close();
         ptrQry->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrCursor) { delete ptrCursor; ptrCursor = NULL; }
      if (ptrQry)    { delete ptrQry;    ptrQry    = NULL; }
      }

   return(arranjo);
   } // SPSNP_rdArranjo_03

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_rdConjCurvaTipica(void)
   {
   //variáveis locais
   AnsiString    strlog;
   TLOBLocator*      ptrStream = NULL;
   int           intConjCurvaId;
   AnsiString    strDescricao;
   int           intTipoCurvaId;
   int           intStatus;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   if (!bd_func)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_BDFUNCINV
      return(false);
      }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdConjCurvaTipica(:p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor"  , otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         // Seta valores dos parâmetros

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            intConjCurvaId = ptrCursor->Field("ConjCurvaId");
            intTipoCurvaId = ptrCursor->Field("TipoCurvaId");
            intStatus      = ptrCursor->Field("Status");
            strDescricao   = ptrCursor->Field("Descricao");
            ptrStream      = ptrCursor->LOBField("Pontos");
            if (!ptrStream)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_CURVAINV
               break;
               }
            if (!bd_func->DesmontaStreamCurvaTipica(ptrStream, intConjCurvaId, strDescricao, intTipoCurvaId, intStatus))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_CURVAINV
               }
            // O LOBField limpa a stream
            //delete ptrStream;

            ptrCursor->Next();
            // loop registros (end)
            }
         ptrCursor->Close();
         ptrQry->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrCursor) { delete ptrCursor; ptrCursor = NULL; }
      if (ptrQry)    { delete ptrQry;    ptrQry    = NULL; }
      }

   return(true);
   } // SPSNP_rdConjCurvaTipica

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_wrCabo(VTCabo *cabo)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   int           intCaboId;
   TOracleQuery* ptrQry = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   if (!cabo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_CABOINV
      return(false);
      }
   // Verifica se o objeto é novo ou alterado
   if (cabo->StatusNovo())
      {
      // Objeto novo. Tem que atualizar em memória o valor do identificador
      // retornado pela base de dados.
      intCaboId = IDINDEFINIDO;
      }
   else if (cabo->StatusAlterado())
      {
      // Objeto previamente existente. O identificador em memória é válido.
      intCaboId = cabo->Id;
      }
      else
         {
         // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
         return(true);
         }

   // Escrita propriamente dita
   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Monta a string de execução
         if (cabo->TipoCfg == caboR_GMR)
            {
            // Cria estrutura do comando
            ptrQry->Clear();
            ptrQry->SQL->Add("BEGIN");
            ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrCaboRGMR(:p_CaboId, :p_Descricao, :p_ExtId, :p_Inom, :p_Vmin, :p_Vmax, :p_R, :p_GMR, :p_Status);");
            ptrQry->SQL->Add("END;");
            ptrQry->DeclareVariable("p_CaboId"   , otInteger);
            ptrQry->DeclareVariable("p_Descricao", otString );
            ptrQry->DeclareVariable("p_ExtId"    , otString );
            ptrQry->DeclareVariable("p_Inom"     , otFloat  );
            ptrQry->DeclareVariable("p_Vmin"     , otFloat  );
            ptrQry->DeclareVariable("p_Vmax"     , otFloat  );
            ptrQry->DeclareVariable("p_R"        , otFloat  );
            ptrQry->DeclareVariable("p_GMR"      , otFloat  );
            ptrQry->DeclareVariable("p_Status"   , otInteger);

            // Seta valores dos parâmetros
            ptrQry->SetVariable("p_CaboId"   , intCaboId             );
            ptrQry->SetVariable("p_Descricao", cabo->Codigo          );
            ptrQry->SetVariable("p_ExtId"    , cabo->Extern_id       );
            ptrQry->SetVariable("p_Inom"     , cabo->Iadm_A          );
            ptrQry->SetVariable("p_Vmin"     , cabo->Vmin_KV         );
            ptrQry->SetVariable("p_Vmax"     , cabo->Vmax_KV         );
            ptrQry->SetVariable("p_R"        , cabo->r               );
            ptrQry->SetVariable("p_GMR"      , cabo->gmr             );
            ptrQry->SetVariable("p_Status"   , cabo->StatusAsUnsigned);
            }
         else
            {
            ptrQry->Clear();
            ptrQry->SQL->Add("BEGIN");
            ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrCaboZ0Z1(:p_CaboId, :p_Descricao, :p_ExtId, :p_Inom, :p_Vmin, :p_Vmax, ");
            ptrQry->SQL->Add("                            :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X, :p_C0, :p_C1, :p_Status ");
            ptrQry->SQL->Add("                           );"                                                   );
            ptrQry->SQL->Add("END;");
            ptrQry->DeclareVariable("p_CaboId"   , otInteger);
            ptrQry->DeclareVariable("p_Descricao", otString );
            ptrQry->DeclareVariable("p_ExtId"    , otString );
            ptrQry->DeclareVariable("p_Inom"     , otFloat  );
            ptrQry->DeclareVariable("p_Vmin"     , otFloat  );
            ptrQry->DeclareVariable("p_Vmax"     , otFloat  );
            ptrQry->DeclareVariable("p_Z0R"      , otFloat  );
            ptrQry->DeclareVariable("p_Z0X"      , otFloat  );
            ptrQry->DeclareVariable("p_Z1R"      , otFloat  );
            ptrQry->DeclareVariable("p_Z1X"      , otFloat  );
            ptrQry->DeclareVariable("p_C0"       , otFloat  );
            ptrQry->DeclareVariable("p_C1"       , otFloat  );
            ptrQry->DeclareVariable("p_Status"   , otInteger);

            // Seta valores dos parâmetros
            ptrQry->SetVariable("p_CaboId"   , intCaboId             );
            ptrQry->SetVariable("p_Descricao", cabo->Codigo          );
            ptrQry->SetVariable("p_ExtId"    , cabo->Extern_id       );
            ptrQry->SetVariable("p_Inom"     , cabo->Iadm_A          );
            ptrQry->SetVariable("p_Vmin"     , cabo->Vmin_KV         );
            ptrQry->SetVariable("p_Vmax"     , cabo->Vmax_KV         );
            ptrQry->SetVariable("p_Z0R"      , cabo->z0.r            );
            ptrQry->SetVariable("p_Z0X"      , cabo->z0.x            );
            ptrQry->SetVariable("p_Z1R"      , cabo->z1.r            );
            ptrQry->SetVariable("p_Z1X"      , cabo->z1.x            );
            ptrQry->SetVariable("p_C0"       , cabo->c0              );
            ptrQry->SetVariable("p_C1"       , cabo->c1              );
            ptrQry->SetVariable("p_Status"   , cabo->StatusAsUnsigned);
            }

         // Executa query
         ptrQry->Execute();

         // Verifica o valor retornado a ser atualizado no objeto em memória
         cabo->Id = ptrQry->GetVariable("p_CaboId");
         if (cabo->Id == IDINDEFINIDO)
            {
            // Condição de erro
            sucesso = false;
            SNPSTRLOGLINE(strlog);
            }
         // Define status
         cabo->DefineStatus(sttNOVO,     false);
         cabo->DefineStatus(sttALTERADO, false);

         ptrQry->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrCabo

//---------------------------------------------------------------------------
// O parâmetro bolIgnoraTipico indica que a escrita não deve ser efetuada
// se o arranjo for típico.
// Como essa mesma rotina deve ser utilizada para escrita de arranjos típicos,
// o flag é necessário para não escrever dados que já estão na base.
//
bool __fastcall TBDgeralOracle::SPSNP_wrArranjo(VTArranjo *arranjo, bool bolTipico)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   int           intArranjoId;
   AnsiString    straux;
   int           intTipoFase;
   int           intNumCabos;
   TOracleQuery* ptrQry = NULL;
   TLOBLocator*  lobLocMatC = NULL;
   TLOBLocator*  lobLocMatZ = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   if (!bd_func)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_BDFUNCINV
      return(false);
      }

   // Atualiza Arranjo
   if (!arranjo)
       {
       SNPSTRLOGLINE(strlog);
       MSGERRSYS_ARRANJOINV
       return(false);
       }

   if (arranjo->TipoCfg == arranjoMUTUA)
      return(true);

   // Verifica se o objeto é novo ou alterado
   if (arranjo->StatusNovo())
      {
      // Objeto novo. Tem que atualizar em memória o valor do identificador
      // retornado pela base de dados.
      intArranjoId = IDINDEFINIDO;
      }
   else if (arranjo->StatusAlterado())
      {
      // Objeto previamente existente. O identificador em memória é válido.
      intArranjoId = arranjo->Id;
      }
      else
         {
         // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
         return(true);
         }

   // Escrita propriamente dita
   __try
      {
      try
         {
			ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
			//JCG@2015.01.05: inclui no Status a informação de ramal
			arranjo->Status[sttRAMAL] = arranjo->ramal;

         // O arranjo não deve ser ignorado, então é salvo
         switch (arranjo->TipoCfg)
            {
            case arranjoCABOxFASE :
               // Monta a string de execução
               ptrQry->Clear();
               ptrQry->SQL->Add("BEGIN");
               ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrArranjo00(:p_ArranjoId, :p_TipoArranjoId, :p_Descricao, :p_ExtId, :p_Tipico, :p_SuporteId, "  );
               ptrQry->SQL->Add("                             :p_ResTerra, :p_Vmin, :p_Vmax, :p_ColorId, :p_Status, :p_NumCabos, :p_TipoFase, ");
               ptrQry->SQL->Add("                             :p_Cabo1Id, :p_TipoFase1Id, :p_FixacaoItem1, "                                   );
               ptrQry->SQL->Add("                             :p_Cabo2Id, :p_TipoFase2Id, :p_FixacaoItem2, "                                   );
               ptrQry->SQL->Add("                             :p_Cabo3Id, :p_TipoFase3Id, :p_FixacaoItem3, "                                   );
               ptrQry->SQL->Add("                             :p_Cabo4Id, :p_TipoFase4Id, :p_FixacaoItem4, "                                   );
               ptrQry->SQL->Add("                             :p_DataVersao "                                                                  );
               ptrQry->SQL->Add("                            );"                                                                               );
               ptrQry->SQL->Add("END;");
               ptrQry->DeclareVariable("p_ArranjoId"    , otInteger);
               ptrQry->DeclareVariable("p_TipoArranjoId", otInteger);
               ptrQry->DeclareVariable("p_Descricao"    , otString );
               ptrQry->DeclareVariable("p_ExtId"        , otString );
               ptrQry->DeclareVariable("p_Tipico"       , otInteger);
               ptrQry->DeclareVariable("p_SuporteId"    , otInteger);
               ptrQry->DeclareVariable("p_ResTerra"     , otFloat  );
               ptrQry->DeclareVariable("p_Vmin"         , otFloat  );
               ptrQry->DeclareVariable("p_Vmax"         , otFloat  );
               ptrQry->DeclareVariable("p_ColorId"      , otInteger);
               ptrQry->DeclareVariable("p_Status"       , otInteger);
               ptrQry->DeclareVariable("p_NumCabos"     , otInteger);
               ptrQry->DeclareVariable("p_TipoFase"     , otInteger);
               ptrQry->DeclareVariable("p_Cabo1Id"      , otInteger);
               ptrQry->DeclareVariable("p_TipoFase1Id"  , otInteger);
               ptrQry->DeclareVariable("p_FixacaoItem1" , otInteger);
               ptrQry->DeclareVariable("p_Cabo2Id"      , otInteger);
               ptrQry->DeclareVariable("p_TipoFase2Id"  , otInteger);
               ptrQry->DeclareVariable("p_FixacaoItem2" , otInteger);
               ptrQry->DeclareVariable("p_Cabo3Id"      , otInteger);
               ptrQry->DeclareVariable("p_TipoFase3Id"  , otInteger);
               ptrQry->DeclareVariable("p_FixacaoItem3" , otInteger);
               ptrQry->DeclareVariable("p_Cabo4Id"      , otInteger);
               ptrQry->DeclareVariable("p_TipoFase4Id"  , otInteger);
               ptrQry->DeclareVariable("p_FixacaoItem4" , otInteger);
               ptrQry->DeclareVariable("p_DataVersao"   , otDate   );

               ptrQry->SetVariable("p_ArranjoId"    , intArranjoId             );
               ptrQry->SetVariable("p_TipoArranjoId", arranjo->TipoCfg         );
               ptrQry->SetVariable("p_Descricao"    , arranjo->Codigo          );
               ptrQry->SetVariable("p_ExtId"        , arranjo->Extern_id       );
               ptrQry->SetVariable("p_Tipico"       , (bolTipico ? 1 : 0)      );
               ptrQry->SetVariable("p_SuporteId"    , arranjo->suporte->Id     );
               ptrQry->SetVariable("p_ResTerra"     , arranjo->ResTerra        );
               ptrQry->SetVariable("p_Vmin"         , arranjo->Vmin_KV         );
               ptrQry->SetVariable("p_Vmax"         , arranjo->Vmax_KV         );
               ptrQry->SetVariable("p_ColorId"      , arranjo->Color           );
               ptrQry->SetVariable("p_Status"       , arranjo->StatusAsUnsigned);
               ptrQry->SetVariable("p_NumCabos"     , 0                        );
               ptrQry->SetVariable("p_TipoFase"     , arranjo->Fases           );
               ptrQry->SetVariable("p_Cabo1Id"      , IDINDEFINIDO             );
               ptrQry->SetVariable("p_TipoFase1Id"  , IDINDEFINIDO             );
               ptrQry->SetVariable("p_FixacaoItem1" , IDINDEFINIDO             );
               ptrQry->SetVariable("p_Cabo2Id"      , IDINDEFINIDO             );
               ptrQry->SetVariable("p_TipoFase2Id"  , IDINDEFINIDO             );
               ptrQry->SetVariable("p_FixacaoItem2" , IDINDEFINIDO             );
               ptrQry->SetVariable("p_Cabo3Id"      , IDINDEFINIDO             );
               ptrQry->SetVariable("p_TipoFase3Id"  , IDINDEFINIDO             );
               ptrQry->SetVariable("p_FixacaoItem3" , IDINDEFINIDO             );
               ptrQry->SetVariable("p_Cabo4Id"      , IDINDEFINIDO             );
               ptrQry->SetVariable("p_TipoFase4Id"  , IDINDEFINIDO             );
               ptrQry->SetVariable("p_FixacaoItem4" , IDINDEFINIDO             );
               ptrQry->SetVariable("p_DataVersao"   , arranjo->Data            );

               intNumCabos = 0;
               for (int i = 0; i < MAX_FASE; i++)
                  {
                  if (arranjo->fase[i].cabo == NULL)
                    continue;

                  straux = Format("p_Cabo%dId", ARRAYOFCONST((i + 1)));
                  ptrQry->SetVariable(straux, arranjo->fase[i].cabo->Id);

                  switch (i)
                     {
                     case 0 : intTipoFase = faseA;
                              break;
                     case 1 : intTipoFase = faseB;
                              break;
                     case 2 : intTipoFase = faseC;
                              break;
                     case 3 : intTipoFase = faseN;
                              break;
                     }
                  straux = Format("p_TipoFase%dId", ARRAYOFCONST((i + 1)));
                  ptrQry->SetVariable(straux, intTipoFase);

                  straux = Format("p_FixacaoItem%d", ARRAYOFCONST((i + 1)));
                  ptrQry->SetVariable(straux, arranjo->fase[i].pto_fixacao->id);

                  intNumCabos++;
                  }

                  // Ajusta o número de cabos
                  ptrQry->SetVariable("p_NumCabos", intNumCabos);
               break;

            case arranjoCABO_Z0Z1 :
               // Monta a string de execução
               ptrQry->Clear();
               ptrQry->SQL->Add("BEGIN");
               ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrArranjo01(:p_ArranjoId, :p_TipoArranjoId, :p_Descricao, :p_ExtId, :p_Tipico, "                       );
               ptrQry->SQL->Add("                             :p_ResTerra, :p_Vmin, :p_Vmax, :p_ColorId, :p_Status, :p_NumCabos, :p_TipoFase, ");
               ptrQry->SQL->Add("                             :p_CaboId, :p_SuporteId, :p_FixacaoItem, "                                       );
               ptrQry->SQL->Add("                             :p_DataVersao "                                                                  );
               ptrQry->SQL->Add("                            );"                                                                               );
               ptrQry->SQL->Add("END;");
               ptrQry->DeclareVariable("p_ArranjoId"    , otInteger);
               ptrQry->DeclareVariable("p_TipoArranjoId", otInteger);
               ptrQry->DeclareVariable("p_Descricao"    , otString );
               ptrQry->DeclareVariable("p_ExtId"        , otString );
               ptrQry->DeclareVariable("p_Tipico"       , otInteger);
               ptrQry->DeclareVariable("p_ResTerra"     , otFloat  );
               ptrQry->DeclareVariable("p_Vmin"         , otFloat  );
               ptrQry->DeclareVariable("p_Vmax"         , otFloat  );
               ptrQry->DeclareVariable("p_ColorId"      , otInteger);
               ptrQry->DeclareVariable("p_Status"       , otInteger);
               ptrQry->DeclareVariable("p_NumCabos"     , otInteger);
               ptrQry->DeclareVariable("p_TipoFase"     , otInteger);
               ptrQry->DeclareVariable("p_CaboId"       , otInteger);
               ptrQry->DeclareVariable("p_SuporteId"    , otInteger);
               ptrQry->DeclareVariable("p_FixacaoItem"  , otInteger);
               ptrQry->DeclareVariable("p_DataVersao"   , otDate   );

               ptrQry->SetVariable("p_ArranjoId"    , intArranjoId             );
               ptrQry->SetVariable("p_TipoArranjoId", arranjo->TipoCfg         );
               ptrQry->SetVariable("p_Descricao"    , arranjo->Codigo          );
               ptrQry->SetVariable("p_ExtId"        , arranjo->Extern_id       );
               ptrQry->SetVariable("p_Tipico"       , (bolTipico ? 1 : 0)      );
               ptrQry->SetVariable("p_ResTerra"     , arranjo->ResTerra        );
               ptrQry->SetVariable("p_Vmin"         , arranjo->Vmin_KV         );
               ptrQry->SetVariable("p_Vmax"         , arranjo->Vmax_KV         );
               ptrQry->SetVariable("p_ColorId"      , arranjo->Color           );
               ptrQry->SetVariable("p_Status"       , arranjo->StatusAsUnsigned);
               ptrQry->SetVariable("p_NumCabos"     , 1                        );
               ptrQry->SetVariable("p_TipoFase"     , arranjo->Fases           );
               ptrQry->SetVariable("p_CaboId"       , arranjo->caboZ0Z1->Id    );

               if (arranjo->suporte)
                  {
                  ptrQry->SetVariable("p_SuporteId", arranjo->suporte->Id);
                  }
               else
                  {
                  ptrQry->SetVariable("p_SuporteId", IDINDEFINIDO);
                  }
               ptrQry->SetVariable("p_FixacaoItem", IDINDEFINIDO);
               ptrQry->SetVariable("p_DataVersao"   , arranjo->Data            );
               break;

            case arranjoZ0Z1      :
               // Monta a string de execução
               ptrQry->Clear();
               ptrQry->SQL->Add("BEGIN");
               ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrArranjo02(:p_ArranjoId, :p_TipoArranjoId, :p_Descricao, :p_ExtId, :p_Tipico, :p_SuporteId, "                   );
               ptrQry->SQL->Add("                             :p_ResTerra, :p_Iadm, :p_Vmin, :p_Vmax, :p_ColorId, :p_Status, :p_NumCabos, :p_TipoFase, " );
               ptrQry->SQL->Add("                             :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X, :p_C0, :p_C1, "                                            );
               ptrQry->SQL->Add("                             :p_DataVersao "                                                                            );
               ptrQry->SQL->Add("                            );");
               ptrQry->SQL->Add("END;");
               ptrQry->DeclareVariable("p_ArranjoId"    , otInteger);
               ptrQry->DeclareVariable("p_TipoArranjoId", otInteger);
               ptrQry->DeclareVariable("p_Descricao"    , otString );
               ptrQry->DeclareVariable("p_ExtId"        , otString );
               ptrQry->DeclareVariable("p_Tipico"       , otInteger);
               ptrQry->DeclareVariable("p_SuporteId"    , otInteger);
               ptrQry->DeclareVariable("p_ResTerra"     , otFloat  );
               ptrQry->DeclareVariable("p_Iadm"         , otFloat  );
               ptrQry->DeclareVariable("p_Vmin"         , otFloat  );
               ptrQry->DeclareVariable("p_Vmax"         , otFloat  );
               ptrQry->DeclareVariable("p_ColorId"      , otInteger);
               ptrQry->DeclareVariable("p_Status"       , otInteger);
               ptrQry->DeclareVariable("p_NumCabos"     , otInteger);
               ptrQry->DeclareVariable("p_TipoFase"     , otInteger);
               ptrQry->DeclareVariable("p_Z0R"          , otFloat  );
               ptrQry->DeclareVariable("p_Z0X"          , otFloat  );
               ptrQry->DeclareVariable("p_Z1R"          , otFloat  );
               ptrQry->DeclareVariable("p_Z1X"          , otFloat  );
               ptrQry->DeclareVariable("p_C0"           , otFloat  );
               ptrQry->DeclareVariable("p_C1"           , otFloat  );
               ptrQry->DeclareVariable("p_DataVersao"   , otDate   );

               ptrQry->SetVariable("p_ArranjoId"    , intArranjoId                                            );
               ptrQry->SetVariable("p_TipoArranjoId", arranjo->TipoCfg                                        );
               ptrQry->SetVariable("p_Descricao"    , arranjo->Codigo                                         );
               ptrQry->SetVariable("p_ExtId"        , arranjo->Extern_id                                      );
               ptrQry->SetVariable("p_Tipico"       , (bolTipico ? 1 : 0)                                     );
               ptrQry->SetVariable("p_SuporteId"    , (arranjo->suporte ? arranjo->suporte->Id : IDINDEFINIDO));
               ptrQry->SetVariable("p_ResTerra"     , arranjo->ResTerra                                       );
               ptrQry->SetVariable("p_Iadm"         , arranjo->Iadm_amp                                       );
               ptrQry->SetVariable("p_Vmin"         , arranjo->Vmin_KV                                        );
               ptrQry->SetVariable("p_Vmax"         , arranjo->Vmax_KV                                        );
               ptrQry->SetVariable("p_ColorId"      , arranjo->Color                                          );
               ptrQry->SetVariable("p_Status"       , arranjo->StatusAsUnsigned                               );
               ptrQry->SetVariable("p_NumCabos"     , 0                                                       );
               ptrQry->SetVariable("p_TipoFase"     , arranjo->Fases                                          );
               ptrQry->SetVariable("p_Z0R"          , arranjo->z0.r                                           );
               ptrQry->SetVariable("p_Z0X"          , arranjo->z0.x                                           );
               ptrQry->SetVariable("p_Z1R"          , arranjo->z1.r                                           );
               ptrQry->SetVariable("p_Z1X"          , arranjo->z1.x                                           );
               ptrQry->SetVariable("p_C0"           , arranjo->c0                                             );
               ptrQry->SetVariable("p_C1"           , arranjo->c1                                             );
               ptrQry->SetVariable("p_DataVersao"   , arranjo->Data                                           );
               break;

            case arranjoMAT_Z     :
               // Monta a string de execução
               ptrQry->Clear();
               ptrQry->SQL->Add("BEGIN");
               ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrArranjo03(:p_ArranjoId, :p_TipoArranjoId, :p_Descricao, :p_ExtId, :p_Tipico, "                                 );
               ptrQry->SQL->Add("                             :p_ResTerra, :p_Iadm, :p_Vmin, :p_Vmax, :p_ColorId, :p_Status, :p_NumCabos, :p_TipoFase, " );
               ptrQry->SQL->Add("                             :p_MatZ, :p_MatC, "                                                                         );
               ptrQry->SQL->Add("                             :p_DataVersao "                                                                            );
               ptrQry->SQL->Add("                            );"                                                                                         );
               ptrQry->SQL->Add("END;");
               ptrQry->DeclareVariable("p_ArranjoId"    , otInteger);
               ptrQry->DeclareVariable("p_TipoArranjoId", otInteger);
               ptrQry->DeclareVariable("p_Descricao"    , otString );
               ptrQry->DeclareVariable("p_ExtId"        , otString );
               ptrQry->DeclareVariable("p_Tipico"       , otInteger);
               ptrQry->DeclareVariable("p_ResTerra"     , otFloat  );
               ptrQry->DeclareVariable("p_Iadm"         , otFloat  );
               ptrQry->DeclareVariable("p_Vmin"         , otFloat  );
               ptrQry->DeclareVariable("p_Vmax"         , otFloat  );
               ptrQry->DeclareVariable("p_ColorId"      , otInteger);
               ptrQry->DeclareVariable("p_Status"       , otInteger);
               ptrQry->DeclareVariable("p_NumCabos"     , otInteger);
               ptrQry->DeclareVariable("p_TipoFase"     , otInteger);
               ptrQry->DeclareVariable("p_MatZ"         , otBLOB   );
               ptrQry->DeclareVariable("p_MatC"         , otBLOB   );
               ptrQry->DeclareVariable("p_DataVersao"   , otDate   );

               ptrQry->SetVariable("p_ArranjoId"    , intArranjoId             );
               ptrQry->SetVariable("p_TipoArranjoId", arranjo->TipoCfg         );
               ptrQry->SetVariable("p_Descricao"    , arranjo->Codigo          );
               ptrQry->SetVariable("p_ExtId"        , arranjo->Extern_id       );
               ptrQry->SetVariable("p_Tipico"       , (bolTipico ? 1 : 0)      );
               ptrQry->SetVariable("p_ResTerra"     , arranjo->ResTerra        );
               ptrQry->SetVariable("p_Iadm"         , arranjo->Iadm_amp        );
               ptrQry->SetVariable("p_Vmin"         , arranjo->Vmin_KV         );
               ptrQry->SetVariable("p_Vmax"         , arranjo->Vmax_KV         );
               ptrQry->SetVariable("p_ColorId"      , arranjo->Color           );
               ptrQry->SetVariable("p_Status"       , arranjo->StatusAsUnsigned);
               ptrQry->SetVariable("p_NumCabos"     , 0                        );
               ptrQry->SetVariable("p_TipoFase"     , arranjo->Fases           );
               ptrQry->SetVariable("p_DataVersao"   , arranjo->Data            );

               lobLocMatC = new TLOBLocator(bd_session, otBLOB, true);
               ptrQry->SetComplexVariable("p_MatC", lobLocMatC);
               if (bd_func->MontaStreamVTMatC(arranjo->MatC, lobLocMatC))
                  {
                  // A stream tem que existir durante a execução do comando.  Deletar somente no final.
                  }
               else
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_MATCSTREAM
                  return(false);
                  }

               lobLocMatZ = new TLOBLocator(bd_session, otBLOB, true);
               ptrQry->SetComplexVariable("p_MatZ", lobLocMatZ);
               if (bd_func->MontaStreamVTMatZ(arranjo->MatZ, lobLocMatZ))
                  {
                  // A stream tem que existir durante a execução do comando.  Deletar somente no final.
                  }
               else
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_MATZSTREAM
                  return(false);
                  }
               break;

            case arranjoPU_SB100  :
               // Monta a string de execução
               ptrQry->Clear();
               ptrQry->SQL->Add("BEGIN");
               ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrArranjo04(:p_ArranjoId, :p_TipoArranjoId, :p_Descricao, :p_ExtId, :p_Tipico, :p_SuporteId, "                   );
               ptrQry->SQL->Add("                             :p_ResTerra, :p_Iadm, :p_Vmin, :p_Vmax, :p_ColorId, :p_Status, :p_NumCabos, :p_TipoFase, " );
               ptrQry->SQL->Add("                             :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X, "                                                          );
               ptrQry->SQL->Add("                             :p_C0, :p_C1, "                                                                            );
               ptrQry->SQL->Add("                             :p_DataVersao "                                                                            );
               ptrQry->SQL->Add("                            );"                                                                                         );
               ptrQry->SQL->Add("END;");
               ptrQry->DeclareVariable("p_ArranjoId"    , otInteger);
               ptrQry->DeclareVariable("p_TipoArranjoId", otInteger);
               ptrQry->DeclareVariable("p_Descricao"    , otString );
               ptrQry->DeclareVariable("p_ExtId"        , otString );
               ptrQry->DeclareVariable("p_Tipico"       , otInteger);
               ptrQry->DeclareVariable("p_SuporteId"    , otInteger);
               ptrQry->DeclareVariable("p_ResTerra"     , otFloat  );
               ptrQry->DeclareVariable("p_Iadm"         , otFloat  );
               ptrQry->DeclareVariable("p_Vmin"         , otFloat  );
               ptrQry->DeclareVariable("p_Vmax"         , otFloat  );
               ptrQry->DeclareVariable("p_ColorId"      , otInteger);
               ptrQry->DeclareVariable("p_Status"       , otInteger);
               ptrQry->DeclareVariable("p_NumCabos"     , otInteger);
               ptrQry->DeclareVariable("p_TipoFase"     , otInteger);
               ptrQry->DeclareVariable("p_Z0R"          , otFloat  );
               ptrQry->DeclareVariable("p_Z0X"          , otFloat  );
               ptrQry->DeclareVariable("p_Z1R"          , otFloat  );
               ptrQry->DeclareVariable("p_Z1X"          , otFloat  );
               ptrQry->DeclareVariable("p_C0"           , otFloat  );
               ptrQry->DeclareVariable("p_C1"           , otFloat  );
               ptrQry->DeclareVariable("p_DataVersao"   , otDate   );

               ptrQry->SetVariable("p_ArranjoId"    , intArranjoId                                            );
               ptrQry->SetVariable("p_TipoArranjoId", arranjo->TipoCfg                                        );
               ptrQry->SetVariable("p_Descricao"    , arranjo->Codigo                                         );
               ptrQry->SetVariable("p_ExtId"        , arranjo->Extern_id                                      );
               ptrQry->SetVariable("p_Tipico"       , (bolTipico ? 1 : 0)                                     );
               ptrQry->SetVariable("p_SuporteId"    , (arranjo->suporte ? arranjo->suporte->Id : IDINDEFINIDO));
               ptrQry->SetVariable("p_ResTerra"     , arranjo->ResTerra                                       );
               ptrQry->SetVariable("p_Iadm"         , arranjo->Iadm_amp                                       );
               ptrQry->SetVariable("p_Vmin"         , arranjo->Vmin_KV                                        );
               ptrQry->SetVariable("p_Vmax"         , arranjo->Vmax_KV                                        );
               ptrQry->SetVariable("p_ColorId"      , arranjo->Color                                          );
               ptrQry->SetVariable("p_Status"       , arranjo->StatusAsUnsigned                               );
               ptrQry->SetVariable("p_NumCabos"     , 0                                                       );
               ptrQry->SetVariable("p_TipoFase"     , arranjo->Fases                                          );
               ptrQry->SetVariable("p_Z0R"          , arranjo->z0.r                                           );
               ptrQry->SetVariable("p_Z0X"          , arranjo->z0.x                                           );
               ptrQry->SetVariable("p_Z1R"          , arranjo->z1.r                                           );
               ptrQry->SetVariable("p_Z1X"          , arranjo->z1.x                                           );
               ptrQry->SetVariable("p_C0"           , arranjo->c0                                             );
               ptrQry->SetVariable("p_C1"           , arranjo->c1                                             );
               ptrQry->SetVariable("p_DataVersao"   , arranjo->Data                                           );
               break;

            default :
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_ARRANJOINV
               return(false);
            }

         // Executa query
         ptrQry->Execute();

         // Verifica o valor retornado a ser atualizado no objeto em memória
         arranjo->Id = ptrQry->GetVariable("p_ArranjoId");
         if (arranjo->Id == IDINDEFINIDO)
            {
            // Condição de erro
            sucesso = false;
            SNPSTRLOGLINE(strlog);
            }
         // Define status
         arranjo->DefineStatus(sttNOVO,     false);
         arranjo->DefineStatus(sttALTERADO, false);

         ptrQry->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (lobLocMatC) { delete lobLocMatC; lobLocMatC = NULL; }
      if (lobLocMatZ) { delete lobLocMatZ; lobLocMatZ = NULL; }
      if (ptrQry)     { delete ptrQry;     ptrQry = NULL;     }
      }
   return(sucesso);
   } // SPSNP_wrArranjo

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_dlArranjoCaboTipico(void)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_dlArranjoCaboTipico(); ");
         ptrQry->SQL->Add("END;");

         // Executa query
         ptrQry->Execute();

         ptrQry->Close();
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_dlArranjoCaboTipico


//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_dlLimpezaAuxiliares(void)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_dlLimpezaAuxiliares(); ");
         ptrQry->SQL->Add("END;");

         // Executa query
         ptrQry->Execute();

         ptrQry->Close();
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_dlAuxiliares

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_dlLimpezaEstudos(void)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_dlLimpezaEstudos(); ");
         ptrQry->SQL->Add("END;");

         // Executa query
         ptrQry->Execute();

         ptrQry->Close();
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_dlLimpezaEstudos

//---------------------------------------------------------------------------
// O parâmetro bolIgnoraTipico indica que a escrita não deve ser efetuada
// se o arranjo for típico.
// Como essa mesma rotina deve ser utilizada para escrita de arranjos típicos,
// o flag é necessário para não escrever dados que já estão na base.
//
bool __fastcall TBDgeralOracle::SPSNP_wrSuporte(VTSuporte *suporte)
   {
   //variáveis locais
   bool          sucesso = true;
   VTArranjo*    arranjo;
   AnsiString    strlog;
   int           intSuporteId;
   TOracleQuery* ptrQry = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   // Atualiza Suporte
   if (!suporte)
       {
       // Como o suporte não é obrigatório, não salva o suporte, mas não gera erro
       return(true);
       }

   // Verifica se o objeto é novo ou alterado
   if (suporte->StatusNovo())
      {
      // Objeto novo. Tem que atualizar em memória o valor do identificador
      // retornado pela base de dados.
      intSuporteId = IDINDEFINIDO;
      }
   else if (suporte->StatusAlterado())
      {
      // Objeto previamente existente. O identificador em memória é válido.
      intSuporteId = suporte->Id;
      }
      else
         {
         // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
         return(true);
         }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // Escrita propriamente dita
         // Monta a string de execução
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrSuporte(:p_SuporteId, :p_TipoSuporteId, :p_Descricao, :p_ExtId, :p_Vmin, :p_Vmax, :p_NumPontosFix, :p_Suporte); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_SuporteId"    , otInteger);
         ptrQry->DeclareVariable("p_TipoSuporteId", otInteger);
         ptrQry->DeclareVariable("p_Descricao"    , otString );
         ptrQry->DeclareVariable("p_ExtId"        , otString );
         ptrQry->DeclareVariable("p_Vmin"         , otFloat  );
         ptrQry->DeclareVariable("p_Vmax"         , otFloat  );
         ptrQry->DeclareVariable("p_NumPontosFix" , otInteger);
         ptrQry->DeclareVariable("p_Suporte"      , otInteger);

         ptrQry->SetVariable("p_SuporteId"    , intSuporteId                   );
         ptrQry->SetVariable("p_TipoSuporteId", suporte->TipoDeSuporte         );
         ptrQry->SetVariable("p_Descricao"    , suporte->Codigo                );
         ptrQry->SetVariable("p_ExtId"        , suporte->Extern_id             );
         ptrQry->SetVariable("p_Vmin"         , suporte->Vmin_KV               );
         ptrQry->SetVariable("p_Vmax"         , suporte->Vmax_KV               );
         ptrQry->SetVariable("p_NumPontosFix" , suporte->LisPtoFixacao()->Count);
         ptrQry->SetVariable("p_Suporte"      , suporte->StatusAsUnsigned      );

         // Executa query
         ptrQry->Execute();

         // Verifica o valor retornado a ser atualizado no objeto em memória
         suporte->Id = ptrQry->GetVariable("p_SuporteId");
         if (suporte->Id == IDINDEFINIDO)
            {
            // Condição de erro
            sucesso = false;
            SNPSTRLOGLINE(strlog);
            }
         // Define status
         suporte->DefineStatus(sttNOVO,     false);
         suporte->DefineStatus(sttALTERADO, false);

         ptrQry->Close();

         // Agora insere a segunda parte do suporte, que são os pontos de fixação.
         // No caso de uma exclusão, como será somente alterado o status do suporte
         // não é necessário (nem podem) ser alterados os pontos de fixação.
         if ((sucesso) && (!suporte->StatusExcluido()))
            {
            // Se o arranjo foi inserido ou atualizado, os pontos de fixação devem ser reescritos
            if (!SPSNP_dlSuporteFixacao(suporte->Id))
               {
               sucesso = false;
               SNPSTRLOGLINE(strlog);
               }
            else
               {
               // Após apagar, com sucesso, os registros de suporte, re-insere
               if (!SPSNP_wrSuporteFixacao(suporte))
                  {
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            }
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrSuporte

//---------------------------------------------------------------------------
// Essa rotina insere todos os pontos de fixação armazenados em um suporte.
// Para que a inserção seja feita corretamente, todos os pontos de fixação
// existentes para o suporte devem ser previamente removidos
bool __fastcall TBDgeralOracle::SPSNP_wrSuporteFixacao(VTSuporte* suporte)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TList*        lisPtoFixacao;
   VTPtoFixacao* ptrPtoFixacao;
   TOracleQuery* ptrQry = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   // Determina os valores dos parâmetros
   if ((lisPtoFixacao = suporte->LisPtoFixacao()) == NULL)
      {
      // Não é obrigatório ter suporte...
      return(true);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrSuporteFixacao(:p_SuporteId, :p_FixacaoItem, :p_X, :p_Y, :p_Codigo); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_SuporteId"  , otInteger);
         ptrQry->DeclareVariable("p_FixacaoItem", otInteger);
         ptrQry->DeclareVariable("p_X"          , otFloat  );
         ptrQry->DeclareVariable("p_Y"          , otFloat  );
         ptrQry->DeclareVariable("p_Codigo"     , otString );

         // Sempre insere um novo ponto e deixa que a base de dados retorne o novo identificador.
         // Esse identificador é posteriormente atualizado nos objetos em memória.
         ptrQry->SetVariable("p_SuporteId", suporte->Id);
         for (int i = 0; i < lisPtoFixacao->Count; i++)
            {
            if ((ptrPtoFixacao = (VTPtoFixacao*)lisPtoFixacao->Items[i]) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_PTOFIX
               return(false);
               }

            ptrQry->SetVariable("p_FixacaoItem", IDINDEFINIDO          );
            ptrQry->SetVariable("p_X"          , ptrPtoFixacao->coord.x);
            ptrQry->SetVariable("p_Y"          , ptrPtoFixacao->coord.y);
            ptrQry->SetVariable("p_Codigo"     , ptrPtoFixacao->codigo );

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            ptrPtoFixacao->id = ptrQry->GetVariable("p_FixacaoItem");
            if (ptrPtoFixacao->id == IDINDEFINIDO)
               {
               // Condição de erro
               sucesso = false;
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_PTOFIX
               break;
               }
            }
         ptrQry->Close();
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrSuporteFixacao

//---------------------------------------------------------------------------
// Trabalha em conjunto com o método SPSNP_wrSuporteFixacao
bool __fastcall TBDgeralOracle::SPSNP_dlSuporteFixacao(int intSuporteId)
   {
   //variáveis locais
   bool       sucesso = true;
   AnsiString strlog;
   TOracleQuery* ptrQry = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_dlSuporteFixacao(:p_SuporteId); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_SuporteId"  , otInteger);
         ptrQry->SetVariable("p_SuporteId", intSuporteId);

         // Executa query
         ptrQry->Execute();

         ptrQry->Close();
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_dlSuporteFixacao

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SalvaCurvaTipica(int intTipoEquipId, VTCurva *curva)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TLOBLocator*  lobLocator = NULL;
   TOracleQuery* ptrQry = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   if (!curva)
       {
       SNPSTRLOGLINE(strlog);
       MSGERRSYS_CURVAINV
       return(false);
       }

   // Somente salva as curvas que tiverem sido alteradas
   if ((!curva->Status[sttNOVO]) && (!curva->Status[sttALTERADO]))
      {
      return(true);
      }

// OOPS TEste
// Retirar quando os ids forem zerados corretamente
if (curva->Status[sttNOVO])
   {
   curva->Id = -1;
   }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrConjCurvaTipica(:p_ConjCurvaId, :p_Descricao, :p_Status, :p_TipoCurvaId, :p_Pontos); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_ConjCurvaId", otInteger          );
         ptrQry->DeclareVariable("p_Descricao"  , otString           );
         ptrQry->DeclareVariable("p_TipoCurvaId", otInteger          );
         ptrQry->DeclareVariable("p_Status"     , otInteger          );
         ptrQry->DeclareVariable("p_Pontos"     , otBLOB             );

         ptrQry->SetVariable("p_ConjCurvaId", curva->Id              );
         ptrQry->SetVariable("p_Descricao"  , curva->Codigo          );
         ptrQry->SetVariable("p_TipoCurvaId", curva->Tipo            );
         ptrQry->SetVariable("p_Status"     , curva->StatusAsUnsigned);

         lobLocator = new TLOBLocator(bd_session, otBLOB, true);
         ptrQry->SetComplexVariable("p_Pontos", lobLocator);
         if (bd_func->MontaStreamCurvaEquipamento(intTipoEquipId, curva, lobLocator))
            {
            // A stream tem que existir durante a execução do comando
            // delete ptrStream;
            // ptrStream = NULL;
            }
         else
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_CURVAINV
            return(false);
            }

         // Executa query
         ptrQry->Execute();

         // Verifica o valor retornado a ser atualizado no objeto em memória
         curva->Id = ptrQry->GetVariable("p_ConjCurvaId");
         if (curva->Id == IDINDEFINIDO)
            {
            sucesso = false;
            SNPSTRLOGLINE(strlog);
            }

         ptrQry->Close();
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (lobLocator) { delete lobLocator; lobLocator = NULL; }
      if (ptrQry)     { delete ptrQry;     ptrQry = NULL;     }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
// Esse parâmetro somente é utilizado no Access, por enquanto.
bool __fastcall TBDgeralOracle::SalvaCurvaTipica(bool bolResetaStatus)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   VTCurvas*     curvas;
   VTCurva *     curva;
   int           intTipoEquipId;
   TOracleQuery* ptrQry = NULL;
   TLOBLocator*  lobLocator = NULL;

   // Estrutura auxiliar para simplificação do loop
   struct sttaux
      {
      int intTipoEquipId;
      TList* lisCur;
      };
   struct sttaux veccur[5];

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   if (!bd_func)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_BDFUNCINV
      return(false);
      }

   if ((curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas))) == NULL)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_CURVAINV
      return(false);
      }
   veccur[0].intTipoEquipId = eqptoCAPACITOR; veccur[0].lisCur = curvas->ListaCurvaTipicaCapacitor();
   veccur[1].intTipoEquipId = eqptoCARGA;     veccur[1].lisCur = curvas->ListaCurvaTipicaCarga();
   veccur[2].intTipoEquipId = eqptoCARGA;     veccur[2].lisCur = curvas->ListaCurvaTipicaIP();
   veccur[3].intTipoEquipId = eqptoREATOR;    veccur[3].lisCur = curvas->ListaCurvaTipicaReator();
   veccur[4].intTipoEquipId = eqptoGERADOR;   veccur[4].lisCur = curvas->ListaCurvaTipicaGeradorFotovoltaico();

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrConjCurvaTipica(:p_ConjCurvaId, :p_Descricao, :p_Status, :p_TipoCurvaId, :p_Pontos); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_ConjCurvaId", otInteger);
         ptrQry->DeclareVariable("p_Descricao"  , otString );
         ptrQry->DeclareVariable("p_TipoCurvaId", otInteger);
         ptrQry->DeclareVariable("p_Status"     , otInteger);
         ptrQry->DeclareVariable("p_Pontos"     , otBLOB   );

         for (int i = 0; i < (int)(sizeof(veccur) / sizeof(struct sttaux)); i++)
            {
            // Loop de tipos de equipamentos (ini)
            if (!veccur[i].lisCur) continue;

            for (int j = 0; j < veccur[i].lisCur->Count; j++)
               {
               // Loop de curvas por tipo de equipamento (ini)
               if ((curva = (VTCurva*)veccur[i].lisCur->Items[j]) == NULL)
                   {
                   SNPSTRLOGLINE(strlog);
                   MSGERRSYS_CURVAINV
                   return(false);
                   }

               // Somente salva as curvas que tiverem sido alteradas
               if ((!curva->Status[sttNOVO]) && (!curva->Status[sttALTERADO])) continue;
               // mml@20130312 - Força o identificador (na leitura do XML o identificador vem > 0)
               if (curva->Status[sttNOVO]) { curva->Id = IDINDEFINIDO; }

               // Escrita propriamente dita
               // Padroniza o valor passado para a procedure
               if (curva->Id <= 0) { curva->Id = IDINDEFINIDO; };

               ptrQry->SetVariable("p_ConjCurvaId", curva->Id               );
//JCG 2016.07.13 - código temporário até alteração da base
               //ptrQry->SetVariable("p_Descricao"  , curva->Codigo         );
               ptrQry->SetVariable("p_Descricao"  , curva->CodigoBD         );
               ptrQry->SetVariable("p_TipoCurvaId", curva->Tipo             );
               ptrQry->SetVariable("p_Status"     , curva->StatusAsUnsigned );

               lobLocator = new TLOBLocator(bd_session, otBLOB, true);
               ptrQry->SetComplexVariable("p_Pontos", lobLocator);
               if (bd_func->MontaStreamCurvaEquipamento(veccur[i].intTipoEquipId, curva, lobLocator))
                  {
                  // A stream tem que existir durante a execução do comando
                  // delete ptrStream;
                  // ptrStream = NULL;
                  }
               else
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CURVAINV
                  return(false);
                  }

               // Executa comando
               ptrQry->Execute();

               // Verifica o valor retornado a ser atualizado no objeto em memória
               curva->Id = ptrQry->GetVariable("p_ConjCurvaId");
               if (curva->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               // Loop de curvas por tipo de equipamento (fim)
               }
            // Loop de tipos de equipamentos (fim)
            }
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (lobLocator) { delete lobLocator; lobLocator = NULL; }
      if (ptrQry)     { delete ptrQry;     ptrQry = NULL;     }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
// Esse parâmetro somente é utilizado no Access, por enquanto
bool __fastcall TBDgeralOracle::SalvaArranjo(bool bolResetaStatus)
   {
   VTCabo*      cabo;
   TList*       lisCABO;
   VTSuporte*   suporte;
   TList*       lisSUPORTE;
   VTArranjo*   arranjo;
   TList*       lisARRANJOS;
   AnsiString   strlog;

   // Atualiza o ponteiro, pois pode ter havido uma edição de arranjos entre
   // dois salvamentos.
   if ((arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos))) == NULL)
      {
      return(NULL);
      }

   // Salva dados dos cabos
   if (progresso) progresso->Add("Salvando cabos");
   // Atualiza Cabos
   if ((lisCABO = arranjos->LisCabo()) == NULL)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISCABO
      return(false);
      }
   for (int i = 0; i < lisCABO->Count; i++)
      {
      // loop cabos (ini)
      cabo = (VTCabo*)lisCABO->Items[i];
      if(!SPSNP_wrCabo(cabo))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRSYS_CABOINV
         return(false);
         }
      // loop cabos (fim)
      }

   // Salva dados dos suporte
   if (progresso) progresso->Add("Salvando suportes");
   if ((lisSUPORTE = arranjos->LisSuporte()) == NULL)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISSUPORTE
      return(false);
      }
   for (int i = 0; i < lisSUPORTE->Count; i++)
      {
      // loop suportes (ini)
      suporte = (VTSuporte*)lisSUPORTE->Items[i];
      if(!SPSNP_wrSuporte(suporte))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDWR_SUPORTE
         return(false);
         }
      // loop suportes (fim)
      }

   // Salva dados dos arranjos típicos
   if (progresso) progresso->Add("Salvando arranjos típicos");
   if ((lisARRANJOS = arranjos->LisArranjo()) == NULL)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISARRANJOS
      return(false);
      }
   for (int i = 0; i < lisARRANJOS->Count; i++)
      {
      // loop arranjos (ini)
      arranjo = (VTArranjo*)lisARRANJOS->Items[i];
      // Todos arranjos dessa lista são típicos
      if(!SPSNP_wrArranjo(arranjo, true))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDWR_ARRANJO2
         return(false);
         }
      // loop arranjos (fim)
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SalvaArranjo(VTArranjo *arranjo, bool bolTipico, bool bolReloadLastId)
   {
   return(SPSNP_wrArranjo(arranjo, bolTipico));
   }

//---------------------------------------------------------------------------
// Combinação de todas as fontes possíveis de arranjos
VTArranjo* __fastcall TBDgeralOracle::LeArranjo(int intArranjoId, int intTipoArranjoId, bool bolTipico)
   {
   AnsiString  strlog;
   VTArranjo*  arranjo = NULL;

   if (!arranjos)
      {
      SNPSTRLOGLINE(strlog);
      return(NULL);
      }

   if (!bolTipico)
      {
      switch (intTipoArranjoId)
         {
         case  arranjoCABOxFASE :
         case  arranjoCABO_Z0Z1 :  arranjo = SPSNP_rdArranjo_00_01(intArranjoId);
                                   break;

         case  arranjoZ0Z1      :
         case  arranjoPU_SB100  :  arranjo = SPSNP_rdArranjo_02_04(intArranjoId);
                                   break;

         case  arranjoMAT_Z     :  arranjo = SPSNP_rdArranjo_03(intArranjoId);
                                   break;
         }
      }
   else
     {
     arranjo = arranjos->ExisteArranjo(intArranjoId);
     }

   return(arranjo);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_rdMercado(VTMercado* mercado)
   {
   //variáveis locais
   bool           sucesso = true;
   AnsiString     strlog;
   TOracleQuery*  ptrQry = NULL;
   TOracleQuery*  ptrCursor = NULL;

	int            intTipoSegmentoId;
	int            intSegmentoId;
	int            intSegmentoIdPrev;
   VTSegmento*    segmento;
   VTCrescimento* crescimento;
   VTClasse*      classe;
   TStream*       ptrStream = NULL;
   int            regiaoId;
   VTRegioes*     regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes));

   if (!classes)
      {
      MSGERRSYS_CLASSESINV
      SNPSTRLOGLINE(strlog);
      }

   if (!mercado)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_MERCADOINV
      return(false);
      }

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   if (!bd_func)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_BDFUNCINV
      return(false);
      }

   // OOPS: esse seria mesmo o melhor lugar pra colocar essa chamada?
   // Efetua a leitura de todas as regiões geográficas antes de ler o mercado
   // if (!SPSNP_rdAreasGeograficas())  { SNPSTRLOGLINE(strlog); MSGERRSYS_REGIAOINV  return(NULL); }
   // Se não conseguir ler, não interrompe a execução. Simplesmente ignora o problema, para
   // ser compatível com as versões anteriores, que não têm a procedure específica para leitura das áreas
   // geográficas.
   SPSNP_rdAreasGeograficas();
   
   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
			ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdRegiao(:p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor"  , otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         // Os registros vêm ordenados por região
			intSegmentoIdPrev = IDINDEFINIDO;

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
				intSegmentoId = ptrCursor->Field("RegiaoId");
				if (intSegmentoId != intSegmentoIdPrev)
               {
               // Nova região
					intSegmentoIdPrev = intSegmentoId;
					segmento = DLL_NewObjSegmento();
					segmento->Id = intSegmentoId;
					segmento->Tipo = ptrCursor->Field("TipoRegiaoId");
					switch (segmento->Tipo)
                  {
						case smINDEF    :
						case smDEFAULT  : segmento->Codigo = "Default";
												break;

                  case smREGIAO   : regiaoId = ptrCursor->Field("AreaGeoId");
                                    segmento->Regiao = regioes->ExisteRegiao(regiaoId);
                                    break;

						case smTIPOREDE :
						case smREDE     : segmento->Codigo = ptrCursor->Field("Codigo");
												break;
						case smAREA     : segmento->Area->x1 = ptrCursor->Field("Utm1X");
												segmento->Area->y1 = ptrCursor->Field("Utm1Y");
												segmento->Area->x2 = ptrCursor->Field("Utm2X");
												segmento->Area->y2 = ptrCursor->Field("Utm2Y");
												break;
						case smCARGA    : segmento->Codigo = ptrCursor->Field("Codigo");
												break;
                  }
					mercado->InsereSegmento(segmento);
               }

            crescimento = DLL_NewObjCrescimento();
            crescimento->Id = IDINDEFINIDO;
				crescimento->Segmento = segmento;
            if ((classe = classes->ExisteClasseTag(ptrCursor->Field("TipoConsTag"))) == NULL)
               {
               MSGERRBDRD_CLASSE
               SNPSTRLOGLINE(strlog);
               sucesso = false;
               break;
               }
            crescimento->Classe = classe;
				segmento->InsereCrescimento(crescimento);

				ptrStream = ptrCursor->LOBField("Pontos");
            if (!ptrStream)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDRD_CURCRESCIMENTO
               sucesso = false;
               break;
               }
            if (!bd_func->DesmontaStreamCurvaCrescimento(crescimento, ptrStream))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDRD_CURCRESCIMENTO
               sucesso = false;
               break;
               }

            ptrCursor->Next();
            }
         ptrCursor->Close();
         ptrQry->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrCursor) { delete ptrCursor; ptrCursor = NULL; }
      if (ptrQry)    { delete ptrQry;    ptrQry    = NULL; }
      }

   return(sucesso);
   } // SPSNP_rdMercado(VTMercado* mercado)

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_rdAreasGeograficas(void)
   {
   //variáveis locais
   bool          bolret = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;
   VTRegioes*    regioes = NULL;
   VTRegiao*     regiao = NULL;
   int           intAreaGeoId;
   TStream*      ptrStream;

   if (!bd_session)                                                          { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)                                               { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if ((regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes))) == NULL) { SNPSTRLOGLINE(strlog); MSGERRSYS_REGIAOINV return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdAreaGeografica(:p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor"  , otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         // Seta valores dos parâmetros

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            intAreaGeoId = ptrCursor->Field("AreaGeoId");
            if ((regiao = regioes->ExisteRegiao(intAreaGeoId)) == NULL)
               {
               regiao = DLL_NewObjRegiao(apl);
               regiao->Id = intAreaGeoId;
               }

            regiao->Codigo = ptrCursor->Field("Codigo");

            ptrStream = ptrCursor->LOBField("Pontos");
            if (!ptrStream)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDRD_POLIGONO
               bolret = false;
               break;
               }
            if (!bd_func->DesmontaStreamPoligono(ptrStream, regiao->Poligono))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDRD_POLIGONO
               bolret = false;
               break;
               }

            ptrCursor->Next();
            }
         ptrCursor->Close();
         ptrQry->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         bolret = true; // A condição segura é indicar que o suporte está sendo utilizado
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrCursor) { delete ptrCursor; ptrCursor = NULL; }
      if (ptrQry)    { delete ptrQry;    ptrQry    = NULL; }
      }

   return(bolret);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_wrMercado(VTMercado* mercado)
   {
   bool           sucesso = true;
   AnsiString     strlog;
	TList*         lisSegmento;
   TList*         lisCrescimento;
	TList*         lisRegiao;
   VTSegmento*    segmento;
   VTCrescimento* crescimento;
   VTRegioes*     regioes = NULL;
   VTRegiao*      regiao = NULL;

   if (!mercado)
      {
      MSGERRSYS_MERCADOINV
      SNPSTRLOGLINE(strlog);
      return(false);
      }

   // mercado->LisCenario();
	lisSegmento = mercado->LisSegmento();
	if (!lisSegmento)
      {
      MSGERRSYS_MERCADOINV
      SNPSTRLOGLINE(strlog);
      return(false);
      }
	for (int i = 0; i < lisSegmento->Count; i++)
      {
		if ((segmento = (VTSegmento*)lisSegmento->Items[i]) == NULL)
         {
         MSGERRSYS_SEGMENTOINV
         SNPSTRLOGLINE(strlog);
         return(false);
         }

		if ((lisCrescimento = segmento->LisCrescimento()) == NULL)
         {
			MSGERRSYS_SEGMENTOINV
         SNPSTRLOGLINE(strlog);
         return(false);
         }

		if (!SPSNP_dlSegmentoMercado(segmento))
			{
			MSGERRBDDL_SEGMENTO
         SNPSTRLOGLINE(strlog);
         return(false);
         }
      }

   // Apaga as Áreas geográficas
   if (!SPSNP_dlAreaGeografica())                      { SNPSTRLOGLINE(strlog); MSGERRBDDL_REGIAO     return(false); }

   // Insere as Áreas Geográficas
   if ((regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes))) == NULL) { SNPSTRLOGLINE(strlog); MSGERRSYS_REGIAOINV return(false); }
   if ((lisRegiao = regioes->LisRegiao()) == NULL)                           { SNPSTRLOGLINE(strlog); MSGERRSYS_REGIAOINV return(false); }
   for (int i = 0; i < lisRegiao->Count; i++)
      {
      if ((regiao = (VTRegiao*)lisRegiao->Items[i]) == NULL)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_REGIAOINV return(false); }

      if (!SPSNP_wrAreaGeografica(regiao))                                   { SNPSTRLOGLINE(strlog); MSGERRSYS_REGIAOINV return(false); }
      }

	for (int i = 0; i < lisSegmento->Count; i++)
      {
		if ((segmento = (VTSegmento*)lisSegmento->Items[i]) == NULL)
         {
         MSGERRSYS_SEGMENTOINV
         SNPSTRLOGLINE(strlog);
         return(false);
         }
		//verifica se Regiao possui crescimento definido
		if (! segmento->Enabled) continue;
      
      // Como foi apagada a região, força a criação de uma região nova
		segmento->Id = IDINDEFINIDO;

		if (!SPSNP_wrSegmentoMercado(segmento))
			{
			MSGERRBDWR_SEGMENTO
			SNPSTRLOGLINE(strlog);
         return(false);
         }

      for (int j = 0; j < lisCrescimento->Count; j++)
         {
         if ((crescimento = (VTCrescimento*)lisCrescimento->Items[j]) == NULL)
            {
            MSGERRSYS_CRESCIMENTOINV
            SNPSTRLOGLINE(strlog);
            }

			if (!SPSNP_wrSegmentoCrescimento(segmento, crescimento))
				{
				MSGERRBDWR_SEGMENTO_CRESCIMENTO
				SNPSTRLOGLINE(strlog);
            return(false);
            }
         }

      // Atualiza o status da região
		segmento->Alterada = false;
      //regiao->Excluida = false;
      segmento->Nova     = false;
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_wrAreaGeografica(VTRegiao* regiao)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   TLOBLocator*  lobLocator = NULL;

	if (!regiao)
      {
      SNPSTRLOGLINE(strlog);
		MSGERRSYS_REGIAOINV
      return(false);
      }

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
			ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrAreaGeografica(:p_AreaGeoId, :p_Codigo, :p_Pontos); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_AreaGeoId"    , otInteger     );
         ptrQry->DeclareVariable("p_Codigo"      , otString      );

			ptrQry->SetVariable("p_AreaGeoId"        , regiao->Id    );
			ptrQry->SetVariable("p_Codigo"          , regiao->Codigo);

         lobLocator = new TLOBLocator(bd_session, otBLOB, true);
         ptrQry->SetComplexVariable("p_Pontos", lobLocator);
         if (bd_func->MontaStreamPoligono(regiao->Poligono, lobLocator))
            {
            // A stream tem que existir durante a execução do comando
            // delete ptrStream;
            // ptrStream = NULL;
            }
         else
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBDWR_POLIGONO
            return(false);
            }

         // Executa query
         ptrQry->Execute();

         // Carrega o valor da região criada
			regiao->Id = ptrQry->GetVariable("p_AreaGeoId");
         if (regiao->Id == IDINDEFINIDO)
            {
            // Condição de erro
            sucesso = false;
            SNPSTRLOGLINE(strlog);
            }

         ptrQry->Close();
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
	} // SPSNP_wrAreaGeografica

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_wrSegmentoMercado(VTSegmento* segmento)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

	if (!segmento)
      {
      SNPSTRLOGLINE(strlog);
		MSGERRSYS_SEGMENTOINV
      return(false);
      }

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
			ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrRegiao(:p_RegiaoId, :p_TipoRegiaoId, :p_Codigo, :p_Utm1X, :p_Utm1Y, :p_Utm2X, :p_Utm2Y, :p_AreaGeoId); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_RegiaoId"    , otInteger     );
         ptrQry->DeclareVariable("p_TipoRegiaoId", otInteger     );
         ptrQry->DeclareVariable("p_Codigo"      , otString      );
         ptrQry->DeclareVariable("p_Utm1X"       , otInteger     );
         ptrQry->DeclareVariable("p_Utm1Y"       , otInteger     );
         ptrQry->DeclareVariable("p_Utm2X"       , otInteger     );
         ptrQry->DeclareVariable("p_Utm2Y"       , otInteger     );
         ptrQry->DeclareVariable("p_AreaGeoId"   , otInteger     );

			ptrQry->SetVariable("p_RegiaoId"        , segmento->Id    );
			ptrQry->SetVariable("p_TipoRegiaoId"    , segmento->Tipo  );
			ptrQry->SetVariable("p_Codigo"          , segmento->Codigo);
         ptrQry->SetVariable("p_Utm1X"           , IDINDEFINIDO  );
         ptrQry->SetVariable("p_Utm1Y"           , IDINDEFINIDO  );
         ptrQry->SetVariable("p_Utm2X"           , IDINDEFINIDO  );
         ptrQry->SetVariable("p_Utm2Y"           , IDINDEFINIDO  );
         ptrQry->SetVariable("p_AreaGeoId"       , segmento->Regiao != NULL? segmento->Regiao->Id : IDINDEFINIDO );

         // !OOPS! modificar procedure para colocar NULL se receber IDINDEFINIDO
			switch (segmento->Tipo)
            {
				case smINDEF    :
				case smDEFAULT  : ptrQry->SetVariable("p_Codigo", "");
										break;
				case smTIPOREDE :
				case smREDE     : ptrQry->SetVariable("p_Codigo", segmento->Codigo);
										break;
				case smAREA     : ptrQry->SetVariable("p_Utm1X", (int)segmento->Area->x1);
										ptrQry->SetVariable("p_Utm1Y", (int)segmento->Area->y1);
										ptrQry->SetVariable("p_Utm2X", (int)segmento->Area->x2);
										ptrQry->SetVariable("p_Utm2Y", (int)segmento->Area->y2);
										break;
				case smCARGA    : ptrQry->SetVariable("p_Codigo", segmento->Codigo);
										break;
            }

         // Executa query
         ptrQry->Execute();

         // Carrega o valor da região criada
			segmento->Id = ptrQry->GetVariable("p_RegiaoId");
         if (segmento->Id == IDINDEFINIDO)
            {
            // Condição de erro
            sucesso = false;
            SNPSTRLOGLINE(strlog);
            }

         ptrQry->Close();
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_wrSegmentoCrescimento(VTSegmento* segmento, VTCrescimento* crescimento)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   int           intConjCurvaId;
   int           intCurvaId;
   int           intSegmentoId;
   TLOBLocator*  lobLocator = NULL;

	if (!segmento)
      {
      SNPSTRLOGLINE(strlog);
		MSGERRSYS_SEGMENTOINV
      return(false);
      }

   if (!crescimento)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_CRESCIMENTOINV
      return(false);
      }

   if (!crescimento->Classe)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_CRESCIMENTOINV
      return(false);
      }

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   if (!bd_func)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_BDFUNCINV
      return(false);
      }

   __try
      {
      try
         {
         lobLocator = new TLOBLocator(bd_session, otBLOB, true);
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         if (!bd_func->MontaStreamCurvaCrescimento(lobLocator, crescimento))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_CURVASTREAM2
            return(false);
            }

         intConjCurvaId = intCurvaId = intSegmentoId = IDINDEFINIDO;
         if (!SPSNP_wrConjCurvaSegmento(intConjCurvaId, intCurvaId, intSegmentoId,
                                        crescimento->Codigo, false, 0 /* status */, 8 /* Crescimento*/, // !OOPS! Fazer macro aqui?
                                        lobLocator
                                       )
            )
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBDWR_CURCRESCIMENTO
            return(false);
            }

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
			ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrRegiaoCrescimento(:p_RegiaoId, :p_TipoConsId, :p_ConjCurvaId); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_RegiaoId"  , otInteger);
			ptrQry->SetVariable("p_RegiaoId", segmento->Id);
         ptrQry->DeclareVariable("p_TipoConsId", otInteger);
         ptrQry->SetVariable("p_TipoConsId", crescimento->Classe->Id);
         ptrQry->DeclareVariable("p_ConjCurvaId", otInteger);
         ptrQry->SetVariable("p_ConjCurvaId", intConjCurvaId);

         // Executa query
         ptrQry->Execute();

         ptrQry->Close();
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)     { delete ptrQry;     ptrQry     = NULL; }
      if (lobLocator) { delete lobLocator; lobLocator = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrRegiaoCrescimento

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_dlSegmentoMercado(VTSegmento* segmento)
	{
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

	if (!segmento)
      {
      SNPSTRLOGLINE(strlog);
		MSGERRSYS_SEGMENTOINV
      return(false);
      }

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
			ptrQry->SQL->Add(" PCK_SINAP.SPSNP_dlRegiao(:p_RegiaoId); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_RegiaoId"  , otInteger);
         ptrQry->SetVariable("p_RegiaoId", segmento->Id);

         // Executa query
         ptrQry->Execute();

         ptrQry->Close();
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_dlRegiao

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_wrCenario(VTCenario* cenario)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

   if (!cenario)
      {
      SNPSTRLOGLINE(strlog);
		MSGERRSYS_SEGMENTOINV
      return(false);
      }

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrCenario(:p_CenarioId, :p_Descricao, :p_Taxa); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_CenarioId"  , otInteger);
         ptrQry->SetVariable("p_CenarioId", cenario->Id);
         ptrQry->DeclareVariable("p_Descricao"  , otString);
         ptrQry->SetVariable("p_Descricao", cenario->Codigo);
         ptrQry->DeclareVariable("p_Taxa", otFloat);
         ptrQry->SetVariable("p_Taxa", cenario->Taxa);

         // Executa query
         ptrQry->Execute();

         // Carrega o valor do cenário criado
         cenario->Id = ptrQry->GetVariable("p_CenarioId");
         if (cenario->Id == IDINDEFINIDO)
            {
            // Condição de erro
            sucesso = false;
            SNPSTRLOGLINE(strlog);
            }

         ptrQry->Close();
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrCenario

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_dlCenario(void)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_dlCenario(); ");
         ptrQry->SQL->Add("END;");

         // Executa query
         ptrQry->Execute();

         ptrQry->Close();
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_dlCenario

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_dlAreaGeografica(void)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_dlAreaGeografica(); ");
         ptrQry->SQL->Add("END;");

         // Executa query
         ptrQry->Execute();

         ptrQry->Close();
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_dlAreaGeografica

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SalvaMercado(void)
   {
   AnsiString strlog;
   VTMercado* mercado;

   if ((mercado = (VTMercado*)apl->GetObject(__classid(VTMercado))) == NULL)
      {
      MSGERRSYS_MERCADOINV
      SNPSTRLOGLINE(strlog);
      }

   return(SPSNP_wrMercado(mercado));
   } // SalvaMercado(void)

//---------------------------------------------------------------------------
// Função de usuo geral para escrita de uma nova curva na base de dados
// Essa função deve receber a stream previamente preparada (ou receber NULL, caso seja uma curva por equação).
// Os identificadores do Conjunto de Curvas, Curva e Segmento são gerados automaticamente.
// Se forem passados valores positivos npara os identificadores na chamada da função, a curva é atualizada.
// Um conjunto de curvas pode ser composto por uma ou mais curvas, cada uma possuindo um ou mais segmentos.
bool __fastcall TBDgeralOracle::SPSNP_wrConjCurvaSegmento(int& intConjCurvaId, int& intCurvaId, int& intSegmentoId,
                                                          AnsiString strDescricao, bool bolTipica, int intStatus, int intTipoCurvaId,
                                                          TLOBLocator* lobLocator
                                                         )
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         if (lobLocator)
            {
            ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrConjCurvaSegmento(:p_ConjCurvaId, :p_CurvaId, :p_SegmentoId, :p_Descricao, :p_Status, :p_Tipica, :p_TipoCurvaId, :p_Pontos); ");
            }
         else
            {
            ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrConjCurvaSegmento(:p_ConjCurvaId, :p_CurvaId, :p_SegmentoId, :p_Descricao, :p_Status, :p_Tipica, :p_TipoCurvaId); ");
            }
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_ConjCurvaId", otInteger);
         ptrQry->DeclareVariable("p_CurvaId"    , otInteger);
         ptrQry->DeclareVariable("p_SegmentoId" , otInteger);
         ptrQry->DeclareVariable("p_Descricao"  , otString );
         ptrQry->DeclareVariable("p_Tipica"     , otInteger);
         ptrQry->DeclareVariable("p_Status"     , otInteger);
         ptrQry->DeclareVariable("p_TipoCurvaId", otInteger);
         ptrQry->DeclareVariable("p_Pontos"     , otBLOB   );

         ptrQry->SetVariable("p_ConjCurvaId", intConjCurvaId);
         ptrQry->SetVariable("p_CurvaId"    , intCurvaId    );
         ptrQry->SetVariable("p_SegmentoId" , intSegmentoId );
         ptrQry->SetVariable("p_Descricao"  , strDescricao  );
         ptrQry->SetVariable("p_Tipica"     , bolTipica     );
         ptrQry->SetVariable("p_Status"     , intStatus     );
         ptrQry->SetVariable("p_TipoCurvaId", intTipoCurvaId);

         if (lobLocator)
            {
            ptrQry->SetComplexVariable("p_Pontos", lobLocator);
            }

         // Executa query
         ptrQry->Execute();

         // Verifica o valor retornado a ser atualizado no objeto em memória
         intConjCurvaId = ptrQry->GetVariable("p_ConjCurvaId");
         if (intConjCurvaId == IDINDEFINIDO)
            {
            sucesso = false;
            SNPSTRLOGLINE(strlog);
            }
         intCurvaId = ptrQry->GetVariable("p_CurvaId");
         if (intCurvaId == IDINDEFINIDO)
            {
            sucesso = false;
            SNPSTRLOGLINE(strlog);
            }
         intSegmentoId = ptrQry->GetVariable("p_SegmentoId");
         if (intSegmentoId == IDINDEFINIDO)
            {
            sucesso = false;
            SNPSTRLOGLINE(strlog);
            }

         ptrQry->Close();
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrConjCurvaSegmento

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::LeGeral(VTGeral* geral)
   {
   if (SPSNP_rdGeralCorCirc(geral))
      {
      return(SPSNP_rdGeralPadraoRede(geral));
      }

   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SalvaGeral(VTGeral* geral)
   {
   if (SPSNP_wrGeralCorCirc(geral))
	  {
	  return(SPSNP_wrGeralPadraoRede(geral));
	  }

   return(false);
   }

//---------------------------------------------------------------------------
int __fastcall TBDgeralOracle::SuporteUtilizado(int intSuporteId)
   {
   return(SPSNP_rdSuporteUtilizado(intSuporteId));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SuporteUtilizado(int intSuporteId, TStringList* suplist)
   {
   return(SPSNP_rdSuporteUtilizado(intSuporteId, suplist));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_rdGeralCorCirc(VTGeral* geral)
   {
   //variáveis locais
   bool           sucesso = true;
   AnsiString     strlog;
   TOracleQuery*  ptrQry = NULL;
   TOracleQuery*  ptrCursor = NULL;
   int            contacor;

   if (!bd_session)
	  {
	  SNPSTRLOGLINE(strlog);
	  MSGERRSYS_SESSION
	  return(false);
	  }

   if (!bd_session->Connected)
	  {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   if (!geral)
      {
      SNPSTRLOGLINE(strlog);
	  MSGERRSYS_GERALINV
      return(false);
      }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
		 ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdGeralCorCirc(:p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor"  , otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();

         contacor = 0;
         while (!ptrCursor->Eof)
            {
            // Protege contra bobagens
			if (contacor > MAX_COR - 1) return(false);

            geral->COR.Circ[contacor] = (TColor)(int)ptrCursor->Field("CorCircVal");
            contacor++;

            ptrCursor->Next();
            }
         ptrCursor->Close();
         ptrQry->Close();
		 }
	  catch (Exception& e)
		 {
         SNPSTRLOGLINE(strlog);
		 SNPSTRLOG(e.Message);
		 sucesso = false;
		 }
	  }
   __finally
	  {
	  //destrói objetos locais
	  if (ptrCursor) { delete ptrCursor; ptrCursor = NULL; }
	  if (ptrQry)    { delete ptrQry;    ptrQry    = NULL; }
	  }

   return(sucesso);
   } // SPSNP_rdGeralCorCirc(void)
//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_rdGeralPadraoRede(VTGeral* geral)
   {
   //variáveis locais
   bool           sucesso = true;
   AnsiString     strlog;
   TOracleQuery*  ptrQry = NULL;
   TOracleQuery*  ptrCursor = NULL;
   VTPadraoRede*  padraorede;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   if (!geral)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_GERALINV
      return(false);
      }

   // Elimina os objetos em memória
   LimpaTList(geral->LisPadraoRede());

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdGeralPadraoRede(:p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor"  , otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();

         while (!ptrCursor->Eof)
            {
            padraorede               = DLL_NewObjPadraoRede();
            padraorede->PadraoRedeId = ptrCursor->Field("PadraoRedeId");
            padraorede->Descricao    = ptrCursor->Field("Descricao");
            padraorede->NumFases     = ptrCursor->Field("NumFases");
            padraorede->VFF          = ptrCursor->Field("VFF");
            padraorede->VFN          = ptrCursor->Field("VFN");
            padraorede->V1           = ptrCursor->Field("V1");
            padraorede->V2           = ptrCursor->Field("V2");
            padraorede->V3           = ptrCursor->Field("V3");
            padraorede->V4           = ptrCursor->Field("V4");
            padraorede->FPI1         = ptrCursor->Field("FPI1");
            padraorede->FPI2         = ptrCursor->Field("FPI2");
            padraorede->FPS1         = ptrCursor->Field("FPS1");
            padraorede->FPS2         = ptrCursor->Field("FPS2");

            // !OOPS! Deveria gerar um erro aqui (retornar false)??? 
            if (!geral->InserePadraoRede(padraorede))
               {
               SNPSTRLOGLINE(strlog);
               }

            ptrCursor->Next();
            }
         ptrCursor->Close();
         ptrQry->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrCursor) { delete ptrCursor; ptrCursor = NULL; }
      if (ptrQry)    { delete ptrQry;    ptrQry    = NULL; }
	  }

   return(sucesso);
   } // SPSNP_rdGeralPadraoRede(void)

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_wrGeralCorCirc(VTGeral* geral)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   if (!geral)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_GERALINV
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrGeralCorCirc( :p_CorCircId, :p_CorCircVal );");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_CorCircId" , otInteger);
         ptrQry->DeclareVariable("p_CorCircVal", otInteger);

         for (int i = 0; i < MAX_COR; i++)
            {
            ptrQry->SetVariable("p_CorCircId" , i);
            ptrQry->SetVariable("p_CorCircVal", (int)geral->COR.Circ[i]);

            // Executa query
            ptrQry->Execute();
            }

         ptrQry->Close();
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrGeralCorCirc(void)
//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_wrGeralPadraoRede(VTGeral* geral)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   TList*        lisPADRAOREDE;
   VTPadraoRede* padraorede;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   if (!geral)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_GERALINV
      return(false);
      }

   if (!geral->LisPadraoRede())
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_GERALINV
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrGeralPadraoRede(:p_PadraoRedeId, :p_Descricao, :p_NumFases, :p_VFF, :p_VFN, ");
         ptrQry->SQL->Add("                                   :p_V1, :p_V2, :p_V3, :p_V4, :p_FPI1, :p_FPI2, :p_FPS1, :p_FPS2 ");
         ptrQry->SQL->Add("                                  ); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_PadraoRedeId" , otInteger);
         ptrQry->DeclareVariable("p_Descricao"    , otString);
         ptrQry->DeclareVariable("p_NumFases"     , otInteger);
         ptrQry->DeclareVariable("p_VFF"          , otFloat  );
         ptrQry->DeclareVariable("p_VFN"          , otFloat  );
         ptrQry->DeclareVariable("p_V1"           , otFloat  );
         ptrQry->DeclareVariable("p_V2"           , otFloat  );
         ptrQry->DeclareVariable("p_V3"           , otFloat  );
         ptrQry->DeclareVariable("p_V4"           , otFloat  );
         ptrQry->DeclareVariable("p_FPI1"         , otFloat  );
         ptrQry->DeclareVariable("p_FPI2"         , otFloat  );
         ptrQry->DeclareVariable("p_FPS1"         , otFloat  );
         ptrQry->DeclareVariable("p_FPS2"         , otFloat  );

         lisPADRAOREDE = geral->LisPadraoRede();
         for (int i = 0; i < lisPADRAOREDE->Count; i++)
            {
            if ((padraorede = (VTPadraoRede*)lisPADRAOREDE->Items[i]) == NULL) return(false);

            ptrQry->SetVariable("p_PadraoRedeId" , padraorede->PadraoRedeId);
            ptrQry->SetVariable("p_Descricao"    , padraorede->Descricao   );
            ptrQry->SetVariable("p_NumFases"     , padraorede->NumFases    );
            ptrQry->SetVariable("p_VFF"          , padraorede->VFF         );
            ptrQry->SetVariable("p_VFN"          , padraorede->VFN         );
            ptrQry->SetVariable("p_V1"           , padraorede->V1          );
            ptrQry->SetVariable("p_V2"           , padraorede->V2          );
            ptrQry->SetVariable("p_V3"           , padraorede->V3          );
            ptrQry->SetVariable("p_V4"           , padraorede->V4          );
            ptrQry->SetVariable("p_FPI1"         , padraorede->FPI1        );
            ptrQry->SetVariable("p_FPI2"         , padraorede->FPI2        );
            ptrQry->SetVariable("p_FPS1"         , padraorede->FPS1        );
            ptrQry->SetVariable("p_FPS2"         , padraorede->FPS2        );
            // Executa query
            ptrQry->Execute();
            // Carrega o identificador gerado pela base de dados.
            padraorede->PadraoRedeId = ptrQry->GetVariable("p_PadraoRedeId");
            if (padraorede->PadraoRedeId == IDINDEFINIDO)
               {
               // Condição de erro
               sucesso = false;
               SNPSTRLOGLINE(strlog);
               }
            }
         ptrQry->Close();
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrGeralPadraoRede(void)

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_wrTipoChave(TList* lstTipoChave)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   VTTipoChave*  tipo_chave;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   if (!lstTipoChave)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrTipoChave(:p_TipoChaveId, :p_Descricao, :p_Status, :p_CorId);");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_TipoChaveId", otInteger);
         ptrQry->DeclareVariable("p_Descricao"  , otString);
         ptrQry->DeclareVariable("p_Status"     , otInteger);
         ptrQry->DeclareVariable("p_CorId"      , otInteger);

         for (int i = 0; i < lstTipoChave->Count; i++)
            {
            if ((tipo_chave = (VTTipoChave*)lstTipoChave->Items[i]) == NULL) return(false);
            //atualiza status
            tipo_chave->Status[sttNOVO]     = false;
            tipo_chave->Status[sttALTERADO] = false;

            ptrQry->SetVariable("p_TipoChaveId" , tipo_chave->Id     );
            ptrQry->SetVariable("p_Descricao"   , tipo_chave->Codigo );
            ptrQry->SetVariable("p_Status"      , tipo_chave->StatusAsUnsigned );
            // OOPS@20130625: Colocar os campos da chave aqui
            ptrQry->SetVariable("p_CorId"       , 0 );
            // Executa query
            ptrQry->Execute();
            // Carrega o identificador gerado pela base de dados.
            tipo_chave->Id = ptrQry->GetVariable("p_TipoChaveId");
            if (tipo_chave->Id == IDINDEFINIDO)
               {
               // Condição de erro
               sucesso = false;
               SNPSTRLOGLINE(strlog);
               }
            }
         ptrQry->Close();
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_wrTipoConsumidor(void)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   VTClasse*     classe;
   TList*        lisCLASSE;

   if (!bd_session)                                                { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)                                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!classes)                                                   { SNPSTRLOGLINE(strlog); MSGERRSYS_CLASSESINV return(false); }

   // Escrita propriamente dita
   __try
      {
      try
         {
         lisCLASSE = classes->LisClasse();
         if (!lisCLASSE)                                           { SNPSTRLOGLINE(strlog); MSGERRSYS_CLASSESINV return(false); }

         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrTipoConsumidor(:p_TipoConsId, :p_Tag, :p_Descricao, :p_Tipico, :p_CorId);");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_TipoConsId", otInteger);
         ptrQry->DeclareVariable("p_Tag"       , otInteger);
         ptrQry->DeclareVariable("p_Descricao" , otString);
         ptrQry->DeclareVariable("p_Tipico"    , otInteger);
         ptrQry->DeclareVariable("p_CorId"     , otInteger);

         for (int i = 0; i < lisCLASSE->Count; i++)
            {
            if ((classe = (VTClasse*)lisCLASSE->Items[i]) == NULL) { SNPSTRLOGLINE(strlog); MSGERRSYS_CLASSESINV return(false); }

            // Determina os valores dos parâmetros
            ptrQry->SetVariable("p_TipoConsId", classe->Id     );
            ptrQry->SetVariable("p_Tag"       , classe->Tag    );
            ptrQry->SetVariable("p_Descricao" , classe->Codigo );
            ptrQry->SetVariable("p_Tipico"    , classe->Tipico );
            // OOPS@20130625: Colocar os campos da chave aqui
            ptrQry->SetVariable("p_CorId"     , 0              );

            // Executa query
            ptrQry->Execute();
            // Carrega o identificador gerado pela base de dados.
            classe->Id = ptrQry->GetVariable("p_TipoConsId");
            if (classe->Id == IDINDEFINIDO)
               {
               // Condição de erro
               sucesso = false;
               SNPSTRLOGLINE(strlog);
               }
            }
            ptrQry->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      // Colocado dentro do finally para garantir que será deletado, mesmo que
      // haja um return no meio do código
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralOracle::SPSNP_wrTipoRede(TList* lstTipoRede)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   VTTipoRede*   tipo_rede;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
      return(false);
      }

   if (!bd_session->Connected)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSIONCON
      return(false);
      }

   if (!lstTipoRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrTipoRede(:p_TipoRedeId, :p_Codigo, :p_Descricao, :p_Vmin, :p_Vmax, :p_ColorId, :p_Status);");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_TipoRedeId", otInteger);
         ptrQry->DeclareVariable("p_Codigo"    , otString);
         ptrQry->DeclareVariable("p_Descricao" , otString);
         ptrQry->DeclareVariable("p_Vmin"      , otFloat);
         ptrQry->DeclareVariable("p_Vmax"      , otFloat);
         ptrQry->DeclareVariable("p_ColorId"   , otInteger);
         ptrQry->DeclareVariable("p_Status"    , otInteger);

         for (int i = 0; i < lstTipoRede->Count; i++)
            {
            if ((tipo_rede = (VTTipoRede*)lstTipoRede->Items[i]) == NULL) return(false);
            //atualiza status
            tipo_rede->Status[sttNOVO]     = false;
            tipo_rede->Status[sttALTERADO] = false;

            ptrQry->SetVariable("p_TipoRedeId"  , tipo_rede->Id       );
            ptrQry->SetVariable("p_Codigo"      , tipo_rede->Codigo   );
            ptrQry->SetVariable("p_Descricao"   , tipo_rede->Descricao);
            ptrQry->SetVariable("p_Vmin"        , tipo_rede->Vmin_kv  );
            ptrQry->SetVariable("p_Vmax"        , tipo_rede->Vmax_kv  );
            ptrQry->SetVariable("p_ColorId"     , tipo_rede->Color    );
            ptrQry->SetVariable("p_Status"      , tipo_rede->StatusAsUnsigned );
            // Executa query
            ptrQry->Execute();
            // Carrega o identificador gerado pela base de dados.
            tipo_rede->Id = ptrQry->GetVariable("p_TipoRedeId");
            if (tipo_rede->Id == IDINDEFINIDO)
               {
               // Condição de erro
               sucesso = false;
               SNPSTRLOGLINE(strlog);
               }
            }
         ptrQry->Close();
         }
      catch(Exception &e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
//eof

