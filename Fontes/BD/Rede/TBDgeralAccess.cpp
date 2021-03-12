//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <ADODB.hpp>
//#include <DB.hpp>
#include "BD_inc.h"
#include "TBDfunc.h"
#include "TBDgeralAccess.h"
#include "TBDcurvaAccess.h"
#include "SNPGlobal.h"
#include "VTSNPDBConn.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Arranjo\VTCabo.h"
#include "..\Arranjo\VTPtoFixacao.h"
#include "..\Arranjo\VTSuporte.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTPadraoRede.h"
#include "..\Diretorio\VTPath.h"
#include "..\Importa\Monta\VTMonta.h"
#include "..\Log\VTLog.h"
#include "..\Mapa\VTRegiao.h"
#include "..\Mapa\VTRegioes.h"
#include "..\Mercado\VTCenario.h"
#include "..\Mercado\VTCrescimento.h"
#include "..\Mercado\VTMercado.h"
#include "..\Mercado\VTSegmento.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTipos.h"
#include "..\..\DLL_Inc\Classes.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Geral.h"
#include "..\..\DLL_Inc\Mapa.h"
#include "..\..\DLL_Inc\Mercado.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTBDgeral* NewObjBDgeralAccess(VTApl *apl_owner, VTSNPDBConn *bd_conn)
   {
   return(new TBDgeralAccess(apl_owner, bd_conn));
   }
/*
//---------------------------------------------------------------------------
__fastcall TBDgeralAccess::TBDgeralAccess(VTApl *apl_owner, VTSNPDBConn *bd_conn)
   {
   AnsiString strlog;
   
   //salva ponteiro
   this->apl      = apl_owner;
   this->bd_conn  = bd_conn;
   bd_curva       = new TBDcurvaAccess(apl_owner, bd_conn);
   //inicia ponteiros de uso geral, buscando no Apl
   plog           = (VTLog*)apl->GetObject(__classid(VTLog));
   if ((path      = (VTPath*)apl->GetObject(__classid(VTPath)))           == NULL) { MSGERRSYS_PATHINV                            }
   if ((arranjos  = (VTArranjos*)apl->GetObject(__classid(VTArranjos)))   == NULL) { MSGERRSYS_ARRANJOSINV SNPSTRLOGLINE(strlog); }
   if ((classes   = (VTClasses*)apl->GetObject(__classid(VTClasses)))     == NULL) { MSGERRSYS_CLASSESINV  SNPSTRLOGLINE(strlog); }
   if ((monta     = (VTMonta*)apl->GetObject(__classid(VTMonta)))         == NULL) { MSGERRSYS_MONTAINV    SNPSTRLOGLINE(strlog); }
   if ((progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso))) == NULL) { MSGERRSYS_PATHINV     SNPSTRLOGLINE(strlog); }
   if ((tipos     = (VTTipos*)apl->GetObject(__classid(VTTipos)))         == NULL) { MSGERRSYS_TIPOSINV    SNPSTRLOGLINE(strlog); }
   if ((bd_func   = (TBDfunc*)apl->GetObject(__classid(TBDfunc)))         == NULL) { MSGERRSYS_BDFUNCINV   SNPSTRLOGLINE(strlog); }
   }
*/
//---------------------------------------------------------------------------
__fastcall TBDgeralAccess::TBDgeralAccess(VTApl *apl_owner, VTSNPDBConn *bd_conn)
   {
   AnsiString strlog;
   
   //salva ponteiro
   this->apl      = apl_owner;
   this->bd_conn  = bd_conn;
   bd_curva       = new TBDcurvaAccess(apl_owner, bd_conn);
   //inicia ponteiros de uso geral, buscando no Apl
   plog      = (VTLog*)apl->GetObject(__classid(VTLog));
   path      = (VTPath*)apl->GetObject(__classid(VTPath));
   arranjos  = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   classes   = (VTClasses*)apl->GetObject(__classid(VTClasses));
   monta     = (VTMonta*)apl->GetObject(__classid(VTMonta));
   progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));
   tipos     = (VTTipos*)apl->GetObject(__classid(VTTipos));
   bd_func   = (TBDfunc*)apl->GetObject(__classid(TBDfunc));
   }

//---------------------------------------------------------------------------
__fastcall TBDgeralAccess::~TBDgeralAccess(void)
   {
   // Elimina os objetos criados localmente
   if (bd_curva) { delete bd_curva; bd_curva = NULL; }
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::Conecta(void)
   {
   //verifica se a base de dados já está aberta
   if (!bd_conn)                  return(false);
   // OOPZ: removi o path de conexão
   // O comando abaixo tenta se conectar se não estiver conectado
   // Verificar o path em que se conecta
   if (bd_conn->getConnectedOK()) return(true);

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TBDgeralAccess::Desconecta(void)
   {
   // OOPZ: Não está fechando a conexão. Isso é feito ao destruir o objeto de conexão
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::LeArranjo(void)
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
VTArranjo* __fastcall TBDgeralAccess::LeArranjo(int intArranjoId, int intTipoArranjoId, bool bolTipico)
   {
   AnsiString strlog;
   VTArranjo* arranjo = NULL;

   if (!arranjos)   { SNPSTRLOGLINE(strlog); return(NULL); }

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
bool __fastcall TBDgeralAccess::LeCfgFluxo(void)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::LeCurvaTipica(void)
   {
   return(SPSNP_rdConjCurvaTipica());
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::LeGeral(VTGeral* geral)
   {
   if (SPSNP_rdGeralCorCirc(geral))
      {
      return(SPSNP_rdGeralPadraoRede(geral));
	  }

   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::LeMercado(void)
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
bool __fastcall TBDgeralAccess::LeTipoChave(void)
   {
   AnsiString strlog;

   if (!SPSNP_rdTipoChave())  { SNPSTRLOGLINE(strlog); MSGERRBDRD_TIPOCHAVE  return(false); }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::LeTipoConsumidor(void)
   {
   AnsiString strlog;

   if (!SPSNP_rdTipoConsumidor()) { SNPSTRLOGLINE(strlog); MSGERRBDRD_TIPOCONSUMIDOR return(false); }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::LeTipoRede(void)
   {
   AnsiString strlog;

   if (!SPSNP_rdTipoRede()) { SNPSTRLOGLINE(strlog); MSGERRBDRD_TIPOREDE  return(false); }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::LeTipos(void)
   {
   AnsiString strlog;

   if (!SPSNP_rdTipoConsumidor()) { SNPSTRLOGLINE(strlog); MSGERRBDRD_TIPOCONSUMIDOR return(false); }
   if (!SPSNP_rdTipoChave())      { SNPSTRLOGLINE(strlog); MSGERRBDRD_TIPOCHAVE      return(false); }
   if (!SPSNP_rdTipoRede())       { SNPSTRLOGLINE(strlog); MSGERRBDRD_TIPOREDE       return(false); }
   if (!LeArranjo())              { SNPSTRLOGLINE(strlog); MSGERRBDRD_ARRANJO        return(false); }
   if (!LeCurvaTipica())          { SNPSTRLOGLINE(strlog); MSGERRBDRD_CURTIPICAS     return(false); }
   if (!LeMercado())              { SNPSTRLOGLINE(strlog); MSGERRBDRD_MERCADO        return(false); }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::RenovaStatus(TList *lisEQP, int ult_id_base)
   {
   bool     sucesso = true;
   VTEqpto* eqpto;
   VTCurva* curva;

   // Reinicia os identificadores / status NOVO do equipamentos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      if ((eqpto->Status[sttNOVO]) || (eqpto->Id <= IDINDEFINIDO) || (eqpto->Id > ult_id_base))
         {
         eqpto->Id = IDINDEFINIDO;
         eqpto->Status[sttNOVO] = true;
         }
      }

   return(sucesso);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::RenovaStatusTipoChave(TList *lisTIPO)
   {
   //variáveis locais
   VTTipoChave *tipochave;

   for (int n = 0; n < lisTIPO->Count; n++)
      {
      tipochave = (VTTipoChave*)lisTIPO->Items[n];

      if ((tipochave->Status[sttNOVO])  || (tipochave->Id <= IDINDEFINIDO) || (tipochave->Tipo > index.ndxTipoChaveId))
         {
         tipochave->Id              = IDINDEFINIDO;
         tipochave->Status[sttNOVO] = true;
         }
      }
   return(true);
   }
*/
/*alteração não adotada para não alterar o conjunto de redes da Eletrobrás
//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::RenovaStatusTipoChave(TList *lisTIPO, bool bolResetaStatus)
   {
   //variáveis locais
   int         aux_id;
   VTTipoChave *tipochave, *tipochave_indef;

   //JCG 2016.08.02: código incluído para corrigir bases de dados em que
   //                TipoChave "indefinida" está com ID diferente de chaveINDEFINIDA
   if (bolResetaStatus)
      {//determina TipoChave indefinida
      if ((tipochave_indef = tipos->ExisteTipoChave("indefinida")) != NULL)
         {//verifica se o TipoChave indefinida está com ID correto
         if (tipochave_indef->Id != chaveINDEFINIDA)
            {//salva ID atual do TipoChave indefinida
            aux_id = tipochave_indef->Id;
            //redefine ID de todos TipoChave entre os valores aux_id e chaveINDEFINIDA
            for (int n = 0; n < lisTIPO->Count; n++)
               {
               tipochave = (VTTipoChave*)lisTIPO->Items[n];
               if ((tipochave->Id > aux_id) && (tipochave->Id < chaveINDEFINIDA))
                  {
                  tipochave->Id              = IDINDEFINIDO;
                  tipochave->Status[sttNOVO] = true;
                  }
               }
            }
         }
      //proteção
      if (index.ndxTipoChaveId <= chaveINDEFINIDA) index.ndxTipoChaveId = chaveINDEFINIDA + 1;
      }
   //JCG 2016.08.02: fim da alteração

   for (int n = 0; n < lisTIPO->Count; n++)
      {
      tipochave = (VTTipoChave*)lisTIPO->Items[n];

      if ((tipochave->Status[sttNOVO])  || (tipochave->Id <= IDINDEFINIDO) || (tipochave->Tipo > index.ndxTipoChaveId))
         {
         tipochave->Id              = IDINDEFINIDO;
         tipochave->Status[sttNOVO] = true;
         }
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::RenovaStatusTipoChave(TList *lisTIPO, bool bolResetaStatus)
   {
   //variáveis locais
   int         indef_id = chaveINDEFINIDA;
   VTTipoChave *tipochave, *tipochave_indef;

   //JCG 2016.08.02: código incluído para corrigir bases de dados em que
   //                TipoChave "indefinida" está com ID diferente de chaveINDEFINIDA
   if (bolResetaStatus)
      {//determina TipoChave indefinida
      if ((tipochave_indef = tipos->ExisteTipoChave("indefinida")) != NULL)
         {//verifica se o TipoChave indefinida está com ID correto
         if (tipochave_indef->Id != chaveINDEFINIDA)
            {//salva ID atual do TipoChave indefinida
            indef_id = tipochave_indef->Id;
            //redefine ID de todos TipoChave com ID maior ao ID da chave indefinida
            for (int n = 0; n < lisTIPO->Count; n++)
               {
               tipochave = (VTTipoChave*)lisTIPO->Items[n];
               if (tipochave->Id > indef_id)
                  {
                  tipochave->Id              = IDINDEFINIDO;
                  tipochave->Status[sttNOVO] = true;
                  }
               }
            }
         }
      //proteção
      if (index.ndxTipoChaveId < indef_id) index.ndxTipoChaveId = indef_id;
      }
   //JCG 2016.08.02: fim da alteração

   for (int n = 0; n < lisTIPO->Count; n++)
      {
      tipochave = (VTTipoChave*)lisTIPO->Items[n];

      if ((tipochave->Status[sttNOVO])  || (tipochave->Id <= IDINDEFINIDO) || (tipochave->Tipo > index.ndxTipoChaveId))
         {
         tipochave->Id              = IDINDEFINIDO;
         tipochave->Status[sttNOVO] = true;
         }
      }
   return(true);
   }

/*
//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::RenovaStatusTipoConsumidor(TList *lisCLASSE)
   {
   //variáveis locais
   VTClasse *classe;

   for (int n = 0; n < lisCLASSE->Count; n++)
      {
      classe = (VTClasse*)lisCLASSE->Items[n];
      if (classe->Id > index.ndxTipoConsId)
         {
         classe->Id              = IDINDEFINIDO;
         //classe->Status[sttNOVO] = true;
         }
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::RenovaStatusTipoConsumidor(TList *lisCLASSE)
   {
   //variáveis locais
   VTClasse *classe;

   for (int n = 0; n < lisCLASSE->Count; n++)
      {
      classe = (VTClasse*)lisCLASSE->Items[n];
      //if ((classe->Status[sttNOVO]) || (classe->Id <= IDINDEFINIDO) || (classe->Id > index.ndxTipoConsId)))
      if ((classe->Id <= IDINDEFINIDO) || (classe->Id > index.ndxTipoConsId))
         {
         classe->Id              = IDINDEFINIDO;
         //classe->Status[sttNOVO] = true;
         }
     }
   return(true);
   }


//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::RenovaStatusTipoRede(TList *lisTIPO)
   {
   //variáveis locais
   VTTipoRede*  tiporede;

   for (int n = 0; n < lisTIPO->Count; n++)
      {
      tiporede = (VTTipoRede*)lisTIPO->Items[n];
      if ((tiporede->Status[sttNOVO]) || (tiporede->Id <= IDINDEFINIDO) || (tiporede->Segmento > index.ndxTipoRedeId))
         {
         tiporede->Id              = IDINDEFINIDO;
         tiporede->Status[sttNOVO] = true;
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::LimpezaAuxiliares(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::LimpezaEstudos(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::RetiraCaboSuporteArranjo(void)
   {
   //variáveis locais
   AnsiString strSQL;

   try{//limpa tabelas ArranjoCaboTipico
      for (int num_passo = 1; num_passo <= 7; num_passo++)
         {
         strSQL.sprintf("Execute SPSNP_dlArranjoCaboTipico_p%02d", num_passo);
         bd_conn->ParseCmd(strSQL);
         if (! bd_conn->Execute()) return(false);
         }
      //limpa tabelas Arranjo
      for (int num_passo = 1; num_passo <= 4; num_passo++)
         {
         strSQL.sprintf("Execute SPSNP_dlArranjo_p%02d :p_ArranjoId", num_passo);
         //decodifica comando
         bd_conn->ParseCmd(strSQL);
         //define valor do parâmetro
         if (! bd_conn->SetParam("p_ArranjoId", PARAMIN_INT(-1))) return(false);
         //executa o comando
         if (! bd_conn->Execute()) return(false);
         }
      //limpa tabelas de Cabos
      for (int num_passo = 1; num_passo <= 3; num_passo++)
         {
         strSQL.sprintf("Execute SPSNP_dlCabo_p%02d", num_passo);
         bd_conn->ParseCmd(strSQL);
         if (! bd_conn->Execute()) return(false);
         }
      //limpa tabelas de Cabos  SuporteFixacao
      strSQL = "Execute SPSNP_dlSuporteFixacao";
      bd_conn->ParseCmd(strSQL);
      if (! bd_conn->Execute()) return(false);
      //limpa tabelas Suporte
      for (int num_passo = 1; num_passo <= 2; num_passo++)
         {
         strSQL.sprintf("Execute SPSNP_dlSuporte_p%02d", num_passo);
         bd_conn->ParseCmd(strSQL);
         if (! bd_conn->Execute()) return(false);
         }
      }catch (Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::RetiraSegmentoMercado(VTSegmento* segmento)
   {
   return(SPSNP_dlSegmentoMercado(segmento));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::RetiraTipoChave(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::RetiraTipoRede(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SalvaTipoChave(bool bolResetaStatus)
   {
   // Reinicia o indexador de tipos de chave
   FNSNP_rdTipoChaveIdProx(false, true);
   // reinicia o status dos TipoChave
   RenovaStatusTipoChave(tipos->LisTipoChave(), bolResetaStatus);
   //grava dados
   return(SPSNP_wrTipoChave(tipos->LisTipoChave()));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SalvaTipoConsumidor(bool bolResetaStatus)
   {
   // Reinicia o indexador de tipos de consumidor
   FNSNP_rdTipoConsumidorIdProx(false, true);
   // reinicia o status dos TipoChave
   RenovaStatusTipoConsumidor(classes->LisClasse());
   //grava dados
   return(SPSNP_wrTipoConsumidor());
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SalvaTipoRede(bool bolResetaStatus)
   {
   // Reinicia o indexador de tipos de rede
   FNSNP_rdTipoRedeIdProx(false, true);
   // reinicia o status dos TipoChave
   RenovaStatusTipoRede(tipos->LisTipoRede());
   //grava dados
   return(SPSNP_wrTipoRede(tipos->LisTipoRede()));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SalvaTipos(bool bolResetaStatus)
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
bool __fastcall TBDgeralAccess::SalvaArranjo(bool bolResetaStatus)
   {
   VTCabo*      cabo;
   TList*       lisCABO;
   VTSuporte*   suporte;
   TList*       lisSUPORTE;
   VTArranjo*   arranjo;
   TList*       lisARRANJOS;
   AnsiString   strlog;
   VTArranjos*  arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));

   // Reinicia indexadores pertinentes
   FNSNP_rdCaboIdProx(false, true);
   FNSNP_rdSuporteIdProx(false, true);
   FNSNP_rdArranjoIdProx(false, true);
   // Reseta o status dos cabos, suportes e arranjos
   RenovaStatus(arranjos->LisCabo(),    index.ndxCaboId);
   RenovaStatus(arranjos->LisSuporte(), index.ndxSuporteId);
   RenovaStatus(arranjos->LisArranjo(), index.ndxArranjoId);
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
// OOPS: Se houver uma probabilidade grande de arranjos típicos, é melhor modificar
// a escrita, fazendo a escrita em modo batch também
bool __fastcall TBDgeralAccess::SalvaArranjo(VTArranjo *arranjo, bool bolTipico, bool bolReloadLastId)
   {
   if (bolReloadLastId)
      {
      // Reinicia indexadores pertinentes
      FNSNP_rdCaboIdProx(false, true);
      FNSNP_rdSuporteIdProx(false, true);
      FNSNP_rdArranjoIdProx(false, true);
      }

   return(SPSNP_wrArranjo(arranjo, bolTipico));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SalvaCurvaTipica(bool bolResetaStatus)
   {
   //variáveis locais
   bool          sucesso = true;
   int           intTipoEquipId;
   AnsiString    strlog;
   VTCurva *     curva;
   VTCurvas*     curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas));

   // Estrutura auxiliar para simplificação do loop
   struct sttaux
      {
      int intTipoEquipId;
      TList* lisCur;
      };
   struct sttaux veccur[5];

   if (!curvas)                      { SNPSTRLOGLINE(strlog);  MSGERRSYS_CURVAINV  return(false); }
   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_curva)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDCURVAINV return(false); }
   // Reinicia o indexador pertinente
   bd_curva->FNSNP_rdConjCurvaIdProx(false, true);
   // Reseta o status das curvas típicas
   bd_curva->RenovaStatusCurva(curvas->ListaCurvaTipicaCapacitor());
   bd_curva->RenovaStatusCurva(curvas->ListaCurvaTipicaCarga());
   bd_curva->RenovaStatusCurva(curvas->ListaCurvaTipicaGeradorFotovoltaico());
   bd_curva->RenovaStatusCurva(curvas->ListaCurvaTipicaIP());
   bd_curva->RenovaStatusCurva(curvas->ListaCurvaTipicaReator());

   // Iniciação da estrutura auxiliar
   veccur[0].intTipoEquipId = eqptoCAPACITOR; veccur[0].lisCur = curvas->ListaCurvaTipicaCapacitor();
   veccur[1].intTipoEquipId = eqptoCARGA;     veccur[1].lisCur = curvas->ListaCurvaTipicaCarga();
   veccur[2].intTipoEquipId = eqptoCARGA;     veccur[2].lisCur = curvas->ListaCurvaTipicaIP();
   veccur[3].intTipoEquipId = eqptoREATOR;    veccur[3].lisCur = curvas->ListaCurvaTipicaReator();
   veccur[4].intTipoEquipId = eqptoGERADOR;   veccur[4].lisCur = curvas->ListaCurvaTipicaGeradorFotovoltaico();

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

         // Padroniza o valor passado para a procedure
         if (curva->Id <= 0) { curva->Id = IDINDEFINIDO; };

         // Escrita propriamente dita
         //if (!SalvaCurvaTipica(veccur[i].intTipoEquipId, curva))
         if (! bd_curva->SPSNP_wrConjCurvaTipica(veccur[i].intTipoEquipId, curva))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_CURVAINV
            return(false);
            }
         // Loop de curvas por tipo de equipamento (fim)
         }
      // Loop de tipos de equipamentos (fim)
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SalvaCurvaTipica(int intTipoEquipId, VTCurva *curva)
   {
   AnsiString strlog;

   // Evita explosões
   if (!bd_curva) { SNPSTRLOGLINE(strlog); MSGERRSYS_BDCURVAINV return(false); }
   // Reinicia o indexador pertinente
   bd_curva->FNSNP_rdConjCurvaIdProx(false, true);
   // Escrita propriamente dita
   return(bd_curva->SPSNP_wrConjCurvaTipica(intTipoEquipId, curva));
   }

//---------------------------------------------------------------------------
int __fastcall TBDgeralAccess::FNSNP_rdArranjoIdProx(bool bolLocaIndex, bool bolBeginNext)
   {
   TADOQuery* ptrQryDS = NULL;
   AnsiString strSQL;
   AnsiString strlog;

   if (bolLocaIndex)
      {
      // índice local (ini)
      index.ndxArranjoId++;
      // índice local (fim)
      }
   else
      {
      // índice da base (ini)
      if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }
      if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }

      __try
         {
         try
            {
            // Monta a string de execução
            strSQL = "EXEC FNSNP_rdArranjoIdProx";
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            // Comando da consulta
            if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            // Recupera o resultado
            if (ptrQryDS->RecordCount != 1)                 { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            index.ndxArranjoId = ptrQryDS->FieldByName("IdProx")->AsInteger;
            // Começa a contagem somente a partir da próxima chamada
            if (bolBeginNext) { index.ndxArranjoId--; }
            ptrQryDS->Close();
            }
         catch (Exception& e)
            {
            SNPSTRLOGLINE(strlog);
            SNPSTRLOG(e.Message);
            index.ndxArranjoId = IDINDEFINIDO;
            }
         }
      __finally
         {
         // Colocado dentro do finally para garantir que será deletado, mesmo que
         // haja um return no meio do código

         //destrói objetos locais
         if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
         }
      // índice da base (fim)
      }

   return(index.ndxArranjoId);
   }

//---------------------------------------------------------------------------
int __fastcall TBDgeralAccess::FNSNP_rdCenarioIdProx(bool bolLocaIndex, bool bolBeginNext)
   {
   TADOQuery* ptrQryDS = NULL;
   AnsiString strSQL;
   AnsiString strlog;

   if (bolLocaIndex)
      {
      // índice local (ini)
      index.ndxCenarioId++;
      // índice local (fim)
      }
   else
      {
      // índice da base (ini)
      if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }
      if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }

      __try
         {
         try
            {
            // Monta a string de execução
            strSQL = "EXEC FNSNP_rdCenarioIdProx";
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            // Comando da consulta
            if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            // Recupera o resultado
            if (ptrQryDS->RecordCount != 1)                 { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            index.ndxCenarioId = ptrQryDS->FieldByName("IdProx")->AsInteger;
            // Começa a contagem somente a partir da próxima chamada
            if (bolBeginNext) { index.ndxCenarioId--; }
            ptrQryDS->Close();
            }
         catch (Exception& e)
            {
            SNPSTRLOGLINE(strlog);
            SNPSTRLOG(e.Message);
            index.ndxCenarioId = IDINDEFINIDO;
            }
         }
      __finally
         {
         // Colocado dentro do finally para garantir que será deletado, mesmo que
         // haja um return no meio do código

         //destrói objetos locais
         if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
         }
      // índice da base (fim)
      }

   return(index.ndxCenarioId);
   }

//---------------------------------------------------------------------------
int __fastcall TBDgeralAccess::FNSNP_rdCaboIdProx(bool bolLocaIndex, bool bolBeginNext)
   {
   TADOQuery* ptrQryDS = NULL;
   AnsiString strSQL;
   AnsiString strlog;

   if (bolLocaIndex)
      {
      // índice local (ini)
      index.ndxCaboId++;
      // índice local (fim)
      }
   else
      {
      // índice da base (ini)
      if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }
      if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }

      __try
         {
         try
            {
            // Monta a string de execução
            strSQL = "EXEC FNSNP_rdCaboIdProx";
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            // Comando da consulta
            if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            // Recupera o resultado
            if (ptrQryDS->RecordCount != 1)                 { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            index.ndxCaboId = ptrQryDS->FieldByName("IdProx")->AsInteger;
            // Começa a contagem somente a partir da próxima chamada
            if (bolBeginNext) { index.ndxCaboId--; }
            ptrQryDS->Close();
            }
         catch (Exception& e)
            {
            SNPSTRLOGLINE(strlog);
            SNPSTRLOG(e.Message);
            index.ndxCaboId = IDINDEFINIDO;
            }
         }
      __finally
         {
         // Colocado dentro do finally para garantir que será deletado, mesmo que
         // haja um return no meio do código

         //destrói objetos locais
         if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
         }
      // índice da base (fim)
      }

   return(index.ndxCaboId);
   }

//---------------------------------------------------------------------------
int __fastcall TBDgeralAccess::FNSNP_rdSegmentoIdProx(bool bolLocaIndex, bool bolBeginNext)
	{
   TADOQuery* ptrQryDS = NULL;
   AnsiString strSQL;
   AnsiString strlog;

   if (bolLocaIndex)
      {
      // índice local (ini)
      index.ndxSegmentoId++;
      // índice local (fim)
      }
   else
      {
      // índice da base (ini)
      if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }
      if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }

      __try
         {
         try
            {
            // Monta a string de execução
				strSQL = "EXEC FNSNP_rdRegiaoIdProx";
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            // Comando da consulta
            if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            // Recupera o resultado
            if (ptrQryDS->RecordCount != 1)                 { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
				index.ndxSegmentoId = ptrQryDS->FieldByName("IdProx")->AsInteger;
            // Começa a contagem somente a partir da próxima chamada
            if (bolBeginNext) { index.ndxSegmentoId--; }
            ptrQryDS->Close();
            }
         catch (Exception& e)
            {
            SNPSTRLOGLINE(strlog);
            SNPSTRLOG(e.Message);
				index.ndxSegmentoId = IDINDEFINIDO;
				}
         }
      __finally
         {
         // Colocado dentro do finally para garantir que será deletado, mesmo que
         // haja um return no meio do código

         //destrói objetos locais
         if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
         }
      // índice da base (fim)
      }

	return(index.ndxSegmentoId);
	}

//---------------------------------------------------------------------------
int __fastcall TBDgeralAccess::FNSNP_rdSuporteIdProx(bool bolLocaIndex, bool bolBeginNext)
   {
   TADOQuery* ptrQryDS = NULL;
   AnsiString strSQL;
   AnsiString strlog;

   if (bolLocaIndex)
      {
      // índice local (ini)
      index.ndxSuporteId++;
      // índice local (fim)
      }
   else
      {
      // índice da base (ini)
      if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }
      if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }

      __try
         {
         try
            {
            // Monta a string de execução
            strSQL = "EXEC FNSNP_rdSuporteIdProx";
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            // Comando da consulta
            if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            // Recupera o resultado
            if (ptrQryDS->RecordCount != 1)                 { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            index.ndxSuporteId = ptrQryDS->FieldByName("IdProx")->AsInteger;
            // Começa a contagem somente a partir da próxima chamada
            if (bolBeginNext) { index.ndxSuporteId--; }
            ptrQryDS->Close();
            }
         catch (Exception& e)
            {
            SNPSTRLOGLINE(strlog);
            SNPSTRLOG(e.Message);
            index.ndxSuporteId = IDINDEFINIDO;
            }
         }
      __finally
         {
         // Colocado dentro do finally para garantir que será deletado, mesmo que
         // haja um return no meio do código

         //destrói objetos locais
         if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
         }
      // índice da base (fim)
      }

   return(index.ndxSuporteId);
   }
/*
//---------------------------------------------------------------------------
int __fastcall TBDgeralAccess::FNSNP_rdSuporteFixacaoItemProx(VTSuporte* suporte, bool bolLocaIndex, bool bolBeginNext)
   {
   TADOQuery* ptrQryDS = NULL;
   AnsiString strSQL;
   AnsiString strlog;

   if (bolLocaIndex)
      {
      // índice local (ini)
      index.ndxSuporteFixacaoId++;
      // índice local (fim)
      }
   else
      {
      // índice da base (ini)
      if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }
      if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }
      if (!suporte)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SUPORTEINV return(IDINDEFINIDO); }

      __try
         {
         try
            {
            // Monta a string de execução
            strSQL = "EXEC FNSNP_rdSuporteFixacaoItemProx :p_SuporteId";
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            if (!bd_conn->SetParam("p_SuporteId", PARAMIN_INT(suporte->Id)))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBD_PARAMINV
               return(NULL);
               }
            // Comando da consulta
            if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            // Recupera o resultado
            if (ptrQryDS->RecordCount != 1)                 { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            index.ndxSuporteFixacaoId = ptrQryDS->FieldByName("IdProx")->AsInteger;
            // Começa a contagem somente a partir da próxima chamada
            if (bolBeginNext) { index.ndxSuporteFixacaoId--; }
            ptrQryDS->Close();
            }
         catch (Exception& e)
            {
            SNPSTRLOGLINE(strlog);
            SNPSTRLOG(e.Message);
            index.ndxSuporteFixacaoId = IDINDEFINIDO;
            }
         }
      __finally
         {
         // Colocado dentro do finally para garantir que será deletado, mesmo que
         // haja um return no meio do código

         //destrói objetos locais
         if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
         }
      // índice da base (fim)
      }

   return(index.ndxSuporteFixacaoId);
   }
*/
//---------------------------------------------------------------------------
int __fastcall TBDgeralAccess::FNSNP_rdTipoChaveIdProx(bool bolLocaIndex, bool bolBeginNext)
   {
   TADOQuery* ptrQryDS = NULL;
   AnsiString strSQL;
   AnsiString strlog;

   if (bolLocaIndex)
      {
      // índice local (ini)
      index.ndxTipoChaveId++;
      // índice local (fim)
      }
   else
      {
      // índice da base (ini)
      if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }
      if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }

      __try
         {
         try
            {
            // Monta a string de execução
            strSQL = "EXEC FNSNP_rdTipoChaveIdProx";
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            // Comando da consulta
            if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            // Recupera o resultado
            if (ptrQryDS->RecordCount != 1)                 { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            index.ndxTipoChaveId = ptrQryDS->FieldByName("IdProx")->AsInteger;
            // Começa a contagem somente a partir da próxima chamada
            if (bolBeginNext) { index.ndxTipoChaveId--; }
            ptrQryDS->Close();
            }
         catch (Exception& e)
            {
            SNPSTRLOGLINE(strlog);
            SNPSTRLOG(e.Message);
            index.ndxTipoChaveId = IDINDEFINIDO;
            }
         }
      __finally
         {
         // Colocado dentro do finally para garantir que será deletado, mesmo que
         // haja um return no meio do código

         //destrói objetos locais
         if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
         }
      // índice da base (fim)
      }

   return(index.ndxTipoChaveId);
   }

//---------------------------------------------------------------------------
int __fastcall TBDgeralAccess::FNSNP_rdTipoConsumidorIdProx(bool bolLocaIndex, bool bolBeginNext)
   {
   TADOQuery* ptrQryDS = NULL;
   AnsiString strSQL;
   AnsiString strlog;

   if (bolLocaIndex)
      {
      // índice local (ini)
      index.ndxTipoConsId++;
      // índice local (fim)
      }
   else
      {
      // índice da base (ini)
      if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }
      if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }

      __try
         {
         try
            {
            // Monta a string de execução
            strSQL = "EXEC FNSNP_rdTipoConsIdProx";
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            // Comando da consulta
            if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            // Recupera o resultado
            if (ptrQryDS->RecordCount != 1)                 { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            index.ndxTipoConsId = ptrQryDS->FieldByName("IdProx")->AsInteger;
            // Começa a contagem somente a partir da próxima chamada
            if (bolBeginNext) { index.ndxTipoConsId--; }
            ptrQryDS->Close();
            }
         catch (Exception& e)
            {
            SNPSTRLOGLINE(strlog);
            SNPSTRLOG(e.Message);
            index.ndxTipoConsId = IDINDEFINIDO;
            }
         }
      __finally
         {
         // Colocado dentro do finally para garantir que será deletado, mesmo que
         // haja um return no meio do código

         //destrói objetos locais
         if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
         }
      // índice da base (fim)
      }

   return(index.ndxTipoConsId);
   }

//---------------------------------------------------------------------------
int __fastcall TBDgeralAccess::FNSNP_rdTipoRedeIdProx(bool bolLocaIndex, bool bolBeginNext)
   {
   TADOQuery* ptrQryDS = NULL;
   AnsiString strSQL;
   AnsiString strlog;

   if (bolLocaIndex)
      {
      // índice local (ini)
      index.ndxTipoRedeId++;
      // índice local (fim)
      }
   else
      {
      // índice da base (ini)
      if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }
      if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }

      __try
         {
         try
            {
            // Monta a string de execução
            strSQL = "EXEC FNSNP_rdTipoRedeIdProx";
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            // Comando da consulta
            if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            // Recupera o resultado
            if (ptrQryDS->RecordCount != 1)                 { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            index.ndxTipoRedeId = ptrQryDS->FieldByName("IdProx")->AsInteger;
            // Começa a contagem somente a partir da próxima chamada
            if (bolBeginNext) { index.ndxTipoRedeId--; }
            ptrQryDS->Close();
            }
         catch (Exception& e)
            {
            SNPSTRLOGLINE(strlog);
            SNPSTRLOG(e.Message);
            index.ndxTipoRedeId = IDINDEFINIDO;
            }
         }
      __finally
         {
         // Colocado dentro do finally para garantir que será deletado, mesmo que
         // haja um return no meio do código

         //destrói objetos locais
         if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
         }
      // índice da base (fim)
      }

   return(index.ndxTipoRedeId);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TBDgeralAccess::SPSNP_rdArranjo_00_01(int intArranjoId)
   {
   VTArranjo*    arranjo  = NULL;
   TADOQuery*    ptrQryDS = NULL;
   AnsiString    strSQL;
   AnsiString    strlog;
   strARRANJO2   str_arranjo2;
   int           intFaseId;
   int           intCabosTotal;
   int           intCabosConta;
   int           ndxFase;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!monta)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_MONTAINV   return(NULL); }

   __try
      {
      try
         {
         // Monta a string de execução
         strSQL = "EXEC SPSNP_rdArranjo_00_01 :p_ArranjoId";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_ArranjoId", PARAMIN_INT(intArranjoId)))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBD_PARAMINV
            return(NULL);
            }
         // Comando da consulta
         if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(NULL); }
         if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(NULL); }
         // Recupera o resultado
         intCabosConta = 0;
         for (int i = 0; i < ptrQryDS->RecordCount; i++)
            {
            // loop registros (ini)
            str_arranjo2.tipo_cfg = ptrQryDS->FieldByName("TipoArranjoId")->AsInteger;

            // loop registros (begin)
            if (intCabosConta == 0)
               {
               intCabosTotal = ptrQryDS->FieldByName("NumCabos")->AsInteger;

               // Carrega estruturas de dados com valores inválidos
               memset(&(str_arranjo2.cfg_suporte_cabo), IDINDEFINIDO, sizeof(str_arranjo2.cfg_suporte_cabo));
               memset(&(str_arranjo2.cfg_caboZOZ1)    , IDINDEFINIDO, sizeof(str_arranjo2.cfg_caboZOZ1));
               }

            if (str_arranjo2.tipo_cfg == arranjoCABOxFASE)
               {
               // Arranjo de cabos monopolares (ini)
               intFaseId = ptrQryDS->FieldByName("TipoFaseId")->AsInteger;
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
                  str_arranjo2.cfg_suporte_cabo.suporte_id                   = ptrQryDS->FieldByName("SuporteId")->AsInteger;
                  str_arranjo2.cfg_suporte_cabo.fase[ndxFase].cabo_id        = ptrQryDS->FieldByName("CaboId")->AsInteger;
                  str_arranjo2.cfg_suporte_cabo.fase[ndxFase].pto_fixacao_id = ptrQryDS->FieldByName("FixacaoItem")->AsInteger;
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
               str_arranjo2.cfg_caboZOZ1.cabo_id    = ptrQryDS->FieldByName("CaboId")->AsInteger;
               str_arranjo2.cfg_caboZOZ1.suporte_id = ptrQryDS->FieldByName("SuporteId")->AsInteger;
               // Arranjo de cabo multiplexado (fim)
               }
            intCabosConta++;

            if (intCabosConta == intCabosTotal)
               {
               str_arranjo2.arranjo_id = ptrQryDS->FieldByName("ArranjoId")->AsInteger;
               str_arranjo2.tipo_cfg   = ptrQryDS->FieldByName("TipoArranjoId")->AsInteger;
               str_arranjo2.codigo     = ptrQryDS->FieldByName("Descricao")->AsString;
//jcg@2013.08.08
               str_arranjo2.extern_id  = ptrQryDS->FieldByName("ExtId")->AsString;
               str_arranjo2.tipico     = ptrQryDS->FieldByName("Tipico")->AsInteger;
               str_arranjo2.fases      = ptrQryDS->FieldByName("ArranjoTipoFaseId")->AsInteger;
               str_arranjo2.res_terra  = ptrQryDS->FieldByName("ResTerra")->AsFloat;
               str_arranjo2.vmin_kv    = ptrQryDS->FieldByName("Vmin")->AsFloat;
               str_arranjo2.vmax_kv    = ptrQryDS->FieldByName("Vmax")->AsFloat;
               str_arranjo2.color      = (TColor)ptrQryDS->FieldByName("ColorId")->AsInteger;
               str_arranjo2.status     = ptrQryDS->FieldByName("Status")->AsInteger;
					//JCG@2015.01.05: extrai informação de ramal do Satus
					str_arranjo2.ramal      = ((str_arranjo2.status & sttRAMAL) == sttRAMAL);
					str_arranjo2.data       = TDateTime(ptrQryDS->FieldByName("DataVersao")->AsDateTime);

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

            ptrQryDS->Next();
            // loop registros (end)
            }

         ptrQryDS->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         arranjo = NULL;
         }
      }
   __finally
      {
      // Colocado dentro do finally para garantir que será deletado, mesmo que
      // haja um return no meio do código

      //destrói objetos locais
      if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
      }

   return(arranjo);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TBDgeralAccess::SPSNP_rdArranjo_02_04(int intArranjoId)
   {
   VTArranjo*    arranjo  = NULL;
   TADOQuery*    ptrQryDS = NULL;
   AnsiString    strSQL;
   AnsiString    strlog;
   strARRANJO2   str_arranjo2;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!monta)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_MONTAINV   return(NULL); }

   __try
      {
      try
         {
         // Monta a string de execução
         strSQL = "EXEC SPSNP_rdArranjo_02_04 :p_ArranjoId";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_ArranjoId", PARAMIN_INT(intArranjoId)))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBD_PARAMINV
            return(NULL);
            }
         // Comando da consulta
         if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(NULL); }
         if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(NULL); }
         // Recupera o resultado
         for (int i = 0; i < ptrQryDS->RecordCount; i++)
            {
            // loop registros (ini)
            // Carrega estruturas de dados com valores inválidos
            memset(&(str_arranjo2.cfg_arranjoZ0Z1), IDINDEFINIDO, sizeof(str_arranjo2.cfg_arranjoZ0Z1));
            memset(&(str_arranjo2.cfg_puSB100)    , IDINDEFINIDO, sizeof(str_arranjo2.cfg_puSB100));

            str_arranjo2.arranjo_id = ptrQryDS->FieldByName("ArranjoId")->AsInteger;
            str_arranjo2.tipo_cfg   = ptrQryDS->FieldByName("TipoArranjoId")->AsInteger;
            str_arranjo2.codigo     = ptrQryDS->FieldByName("Descricao")->AsString;
//jcg@2013.08.08
            str_arranjo2.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
            str_arranjo2.tipico     = ptrQryDS->FieldByName("Tipico")->AsInteger;
            str_arranjo2.fases      = ptrQryDS->FieldByName("ArranjoTipoFaseId")->AsInteger;
            str_arranjo2.res_terra  = ptrQryDS->FieldByName("ResTerra")->AsFloat;
            str_arranjo2.vmin_kv    = ptrQryDS->FieldByName("Vmin")->AsFloat;
            str_arranjo2.vmax_kv    = ptrQryDS->FieldByName("Vmax")->AsFloat;
            str_arranjo2.color      = (TColor)(int)ptrQryDS->FieldByName("ColorId")->AsInteger;
            str_arranjo2.status     = (TColor)(int)ptrQryDS->FieldByName("Status")->AsInteger;
				//JCG@2015.01.05: extrai informação de ramal do Satus
				str_arranjo2.ramal      = ((str_arranjo2.status & sttRAMAL) == sttRAMAL);
				str_arranjo2.data       = TDateTime(ptrQryDS->FieldByName("DataVersao")->AsDateTime);

            if (str_arranjo2.tipo_cfg == arranjoZ0Z1)
               {
               // Arranjo Z0Z1 (ini)
               str_arranjo2.cfg_arranjoZ0Z1.suporte_id = ptrQryDS->FieldByName("SuporteId")->AsInteger;
               str_arranjo2.cfg_arranjoZ0Z1.z0.r       = ptrQryDS->FieldByName("Z0R")->AsFloat;
               str_arranjo2.cfg_arranjoZ0Z1.z0.x       = ptrQryDS->FieldByName("Z0X")->AsFloat;
               str_arranjo2.cfg_arranjoZ0Z1.z1.r       = ptrQryDS->FieldByName("Z1R")->AsFloat;
               str_arranjo2.cfg_arranjoZ0Z1.z1.x       = ptrQryDS->FieldByName("Z1X")->AsFloat;
               str_arranjo2.cfg_arranjoZ0Z1.c0         = ptrQryDS->FieldByName("C0")->AsFloat;
               str_arranjo2.cfg_arranjoZ0Z1.c1         = ptrQryDS->FieldByName("C1")->AsFloat;
               str_arranjo2.cfg_arranjoZ0Z1.iadm_amp   = ptrQryDS->FieldByName("Iadm")->AsFloat;
               // Arranjo Z0Z1 (fim)
               }
            else
               {
               // Arranjo PUSB100 (ini)
               str_arranjo2.cfg_puSB100.suporte_id = ptrQryDS->FieldByName("SuporteId")->AsInteger;
               str_arranjo2.cfg_puSB100.z0.r       = ptrQryDS->FieldByName("Z0R")->AsFloat;
               str_arranjo2.cfg_puSB100.z0.x       = ptrQryDS->FieldByName("Z0X")->AsFloat;
               str_arranjo2.cfg_puSB100.z1.r       = ptrQryDS->FieldByName("Z1R")->AsFloat;
               str_arranjo2.cfg_puSB100.z1.x       = ptrQryDS->FieldByName("Z1X")->AsFloat;
               str_arranjo2.cfg_puSB100.c0         = ptrQryDS->FieldByName("C0")->AsFloat;
               str_arranjo2.cfg_puSB100.c1         = ptrQryDS->FieldByName("C1")->AsFloat;
               str_arranjo2.cfg_puSB100.iadm_amp   = ptrQryDS->FieldByName("Iadm")->AsFloat;
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

            ptrQryDS->Next();
            // loop registros (end)
            }

         ptrQryDS->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         arranjo = NULL;
         }
      }
   __finally
      {
      // Colocado dentro do finally para garantir que será deletado, mesmo que
      // haja um return no meio do código

      //destrói objetos locais
      if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
      }

   return(arranjo);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TBDgeralAccess::SPSNP_rdArranjo_03(int intArranjoId)
   {
   VTArranjo*    arranjo  = NULL;
   TADOQuery*    ptrQryDS = NULL;
   AnsiString    strSQL;
   AnsiString    strlog;
   strARRANJO2   str_arranjo2;
   TStream*      ptrStream = NULL;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!monta)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_MONTAINV   return(NULL); }
   if (!bd_func)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(NULL); }

   __try
      {
      try
         {
         // Monta a string de execução
         strSQL = "EXEC SPSNP_rdArranjo_03 :p_ArranjoId";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_ArranjoId", PARAMIN_INT(intArranjoId)))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBD_PARAMINV
            return(NULL);
            }
         // Comando da consulta
         if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(NULL); }
         if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(NULL); }
         // Recupera o resultado
         for (int i = 0; i < ptrQryDS->RecordCount; i++)
            {
            // loop registros (ini)
            // Carrega estruturas de dados com valores inválidos
            memset(&(str_arranjo2.cfg_matZ), IDINDEFINIDO, sizeof(str_arranjo2.cfg_matZ));

            str_arranjo2.arranjo_id          = ptrQryDS->FieldByName("ArranjoId")->AsInteger;
            str_arranjo2.tipo_cfg            = ptrQryDS->FieldByName("TipoArranjoId")->AsInteger;
            str_arranjo2.codigo              = ptrQryDS->FieldByName("Descricao")->AsString;
//jcg@2013.08.08
            str_arranjo2.extern_id           = ptrQryDS->FieldByName("ExtId")->AsString;
            str_arranjo2.tipico              = ptrQryDS->FieldByName("Tipico")->AsInteger;
            str_arranjo2.fases               = ptrQryDS->FieldByName("ArranjoTipoFaseId")->AsInteger;
            str_arranjo2.res_terra           = ptrQryDS->FieldByName("ResTerra")->AsFloat;
            str_arranjo2.vmin_kv             = ptrQryDS->FieldByName("Vmin")->AsFloat;
            str_arranjo2.vmax_kv             = ptrQryDS->FieldByName("Vmax")->AsFloat;
            str_arranjo2.color               = (TColor)(int)ptrQryDS->FieldByName("ColorId")->AsInteger;
            str_arranjo2.status              = ptrQryDS->FieldByName("Status")->AsInteger;
				//JCG@2015.01.05: extrai informação de ramal do Satus
				str_arranjo2.ramal               = ((str_arranjo2.status & sttRAMAL) == sttRAMAL);
				str_arranjo2.cfg_matZ.iadm_amp   = ptrQryDS->FieldByName("Iadm")->AsFloat;
            str_arranjo2.data                = TDateTime(ptrQryDS->FieldByName("DataVersao")->AsDateTime);

            ptrStream = ptrQryDS->CreateBlobStream(ptrQryDS->FieldByName("MatZ"), bmRead);
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

            ptrStream = ptrQryDS->CreateBlobStream(ptrQryDS->FieldByName("MatC"), bmRead);
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

            ptrQryDS->Next();
            // loop registros (end)
            }

         ptrQryDS->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         arranjo = NULL;
         }
      }
   __finally
      {
      // Colocado dentro do finally para garantir que será deletado, mesmo que
      // haja um return no meio do código

      //destrói objetos locais
      if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
      }

   return(arranjo);
   }

//---------------------------------------------------------------------------
VTCabo* __fastcall TBDgeralAccess::SPSNP_rdCabo(int intCaboId)
   {
   VTCabo*    cabo = NULL;
   TADOQuery* ptrQryDS = NULL;
   AnsiString strSQL;
   AnsiString strlog;
   strCABO    str_cabo;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!monta)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_MONTAINV   return(NULL); }

   __try
      {
      try
         {
         // Monta a string de execução
         strSQL = "EXEC SPSNP_rdCabo :p_CaboId";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_CaboId", PARAMIN_INT(intCaboId)))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBD_PARAMINV
            return(NULL);
            }
         // Comando da consulta
         if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(NULL); }
         if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(NULL); }
         // Recupera o resultado
         for (int i = 0; i < ptrQryDS->RecordCount; i++)
            {
            // loop registros (ini)
            str_cabo.id       = ptrQryDS->FieldByName("CaboId")->AsInteger;
            str_cabo.tipo_cfg = ptrQryDS->FieldByName("TipoCaboId")->AsInteger;
            str_cabo.codigo   = ptrQryDS->FieldByName("Descricao")->AsString;
//jcg@2013.08.08
            str_cabo.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
            str_cabo.iadm     = ptrQryDS->FieldByName("Inom")->AsFloat;
            str_cabo.vmin     = ptrQryDS->FieldByName("Vmin")->AsFloat;
            str_cabo.vmax     = ptrQryDS->FieldByName("Vmax")->AsFloat;
            str_cabo.status   = ptrQryDS->FieldByName("Status")->AsInteger;
            //
            // OOPS. Falta um campo para status
            //
            if (str_cabo.tipo_cfg == caboR_GMR)
               {
               // Cabo RGMR (ini)
               str_cabo.r = ptrQryDS->FieldByName("RGMR_R")->AsFloat;
               str_cabo.gmr = ptrQryDS->FieldByName("RGMR_GMR")->AsFloat;

               str_cabo.z0.r = ptrQryDS->FieldByName("Z0Z1_Z0R")->AsFloat; // Base retorna zero
               str_cabo.z0.x = ptrQryDS->FieldByName("Z0Z1_Z0X")->AsFloat; // Base retorna zero
               str_cabo.z1.r = ptrQryDS->FieldByName("Z0Z1_Z1R")->AsFloat; // Base retorna zero
               str_cabo.z1.x = ptrQryDS->FieldByName("Z0Z1_Z1X")->AsFloat; // Base retorna zero
               str_cabo.c0   = ptrQryDS->FieldByName("Z0Z1_C0")->AsFloat;  // Base retorna zero
               str_cabo.c1   = ptrQryDS->FieldByName("Z0Z1_C1")->AsFloat;  // Base retorna zero
               // Cabo RGMR (fim)
               }
            else
               {
               // Cabo Z0Z1 (ini)
               str_cabo.r = ptrQryDS->FieldByName("RGMR_R")->AsFloat;      // Base retorna zero
               str_cabo.gmr = ptrQryDS->FieldByName("RGMR_GMR")->AsFloat;  // Base retorna zero

               str_cabo.z0.r = ptrQryDS->FieldByName("Z0Z1_Z0R")->AsFloat;
               str_cabo.z0.x = ptrQryDS->FieldByName("Z0Z1_Z0X")->AsFloat;
               str_cabo.z1.r = ptrQryDS->FieldByName("Z0Z1_Z1R")->AsFloat;
               str_cabo.z1.x = ptrQryDS->FieldByName("Z0Z1_Z1X")->AsFloat;
               str_cabo.c0   = ptrQryDS->FieldByName("Z0Z1_C0")->AsFloat;
               str_cabo.c1   = ptrQryDS->FieldByName("Z0Z1_C1")->AsFloat;
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

            ptrQryDS->Next();
            // loop registros (end)
            }

         ptrQryDS->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         cabo = NULL;
         }
      }
   __finally
      {
      // Colocado dentro do finally para garantir que será deletado, mesmo que
      // haja um return no meio do código

      //destrói objetos locais
      if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
      }

   return(cabo);
   }  // SPSNP_rdCabo

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_rdConjCurvaTipica(void)
   {
   bool          sucesso  = true;
   TADOQuery*    ptrQryDS = NULL;
   AnsiString    strSQL;
   AnsiString    strlog;
   TStream*      ptrStream = NULL;
   int           intConjCurvaId;
   int           intTipoCurvaId;
   int           intStatus;
   AnsiString    strDescricao;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!tipos)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_TIPOSINV   return(false); }
   if (!bd_func)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   __try
      {
      try
         {
         // Monta a string de execução
         strSQL = "EXEC SPSNP_rdConjCurvaTipica";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         // Comando da consulta
         if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(false); }
         if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(false); }
         // Recupera o resultado
         for (int i = 0; i < ptrQryDS->RecordCount; i++)
            {
            // loop registros (ini)
            intConjCurvaId = ptrQryDS->FieldByName("ConjCurvaId")->AsInteger;
            intTipoCurvaId = ptrQryDS->FieldByName("TipoCurvaId")->AsInteger;
            intStatus      = ptrQryDS->FieldByName("Status")->AsInteger;
            strDescricao   = ptrQryDS->FieldByName("Descricao")->AsString;
            ptrStream      = ptrQryDS->CreateBlobStream(ptrQryDS->FieldByName("Pontos"), bmRead);
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

            ptrQryDS->Next();
            // loop registros (end)
            }

         ptrQryDS->Close();
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

	  //destrói objetos locais
	  if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
	  }

   return(sucesso);
   }

////---------------------------------------------------------------------------
//bool __fastcall TBDgeralAccess::SPSNP_rdGeralCorCirc(VTGeral* geral)
//   {
//   bool          sucesso  = true;
//   TADOQuery*    ptrQryDS = NULL;
//   AnsiString    strSQL;
//   AnsiString    strlog;
//   int           contacor;
//
//   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
//   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
//   if (!geral)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
//
//   __try
//      {
//      try
//         {
//		 // Monta a string de execução
//         strSQL = "EXEC SPSNP_rdGeralCorCirc";
//         // Decodifica comando
//         bd_conn->ParseCmd(strSQL);
//         // Determina os valores dos parâmetros
//         // Comando da consulta
//         if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(false); }
//		 if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(false); }
//         // Recupera o resultado
//         contacor = 0;
//         for (int i = 0; i < ptrQryDS->RecordCount; i++)
//            {
//            // Protege contra bobagens
//            if (contacor > MAX_COR - 1)                  { SNPSTRLOGLINE(strlog); return(false); }
//
//            geral->COR.Circ[contacor] = (TColor)(ptrQryDS->FieldByName("CorCircVal")->AsInteger);
//            contacor++;
//
//            ptrQryDS->Next();
//            // loop registros (end)
//            }
//
//         ptrQryDS->Close();
//         }
//      catch (Exception& e)
//         {
//         SNPSTRLOGLINE(strlog);
//         SNPSTRLOG(e.Message);
//		 sucesso = false;
//         }
//      }
//   __finally
//      {
//      // Colocado dentro do finally para garantir que será deletado, mesmo que
//      // haja um return no meio do código
//
//	  //destrói objetos locais
//	  if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
//	  }
//
//   return(sucesso);
//   }
//---------------------------------------------------------------------------
// FKM 2019.02.22 - não estava sendo usado,
//                       roubei pra salvar as cores do diagnostico de fluxo
//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_rdGeralCorCirc(VTGeral* geral)
   {
   bool          sucesso  = true;
   TADOQuery*    ptrQryDS = NULL;
   AnsiString    strSQL;
   AnsiString    strlog;
   int           contacor;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!geral)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }

   __try
      {
      try
         {
		 // Monta a string de execução
         strSQL = "EXEC SPSNP_rdGeralCorCirc";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         // Comando da consulta
         if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(false); }
		 if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(false); }
         // Recupera o resultado
         contacor = 0;
         for (int i = 0; i < ptrQryDS->RecordCount; i++)
            {
            // Protege contra bobagens
			if (contacor > MAX_FAIXA)                  { SNPSTRLOGLINE(strlog); return(false); }

            geral->COR.Faixa[contacor] = (TColor)(ptrQryDS->FieldByName("CorCircVal")->AsInteger);
            contacor++;

            ptrQryDS->Next();
            // loop registros (end)
            }

         ptrQryDS->Close();
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

	  //destrói objetos locais
	  if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
	  }

   return(sucesso);
   }
//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_rdGeralPadraoRede(VTGeral* geral)
   {
   bool          sucesso  = true;
   TADOQuery*    ptrQryDS = NULL;
   AnsiString    strSQL;
   AnsiString    strlog;
   VTPadraoRede* padraorede;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!geral)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }

   __try
      {
      try
         {
         // Monta a string de execução
         strSQL = "EXEC SPSNP_rdGeralPadraoRede";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         // Comando da consulta
         if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(false); }
         if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(false); }
         // Recupera o resultado
         for (int i = 0; i < ptrQryDS->RecordCount; i++)
            {
            padraorede               = DLL_NewObjPadraoRede();
            padraorede->PadraoRedeId = ptrQryDS->FieldByName("PadraoRedeId")->AsInteger;
            padraorede->Descricao    = ptrQryDS->FieldByName("Descricao")->AsString;
            padraorede->NumFases     = ptrQryDS->FieldByName("NumFases")->AsInteger;
            padraorede->VFF          = ptrQryDS->FieldByName("VFF")->AsFloat;
            padraorede->VFN          = ptrQryDS->FieldByName("VFN")->AsFloat;
            padraorede->V1           = ptrQryDS->FieldByName("V1")->AsFloat;
            padraorede->V2           = ptrQryDS->FieldByName("V2")->AsFloat;
            padraorede->V3           = ptrQryDS->FieldByName("V3")->AsFloat;
            padraorede->V4           = ptrQryDS->FieldByName("V4")->AsFloat;
            padraorede->FPI1         = ptrQryDS->FieldByName("FPI1")->AsFloat;
            padraorede->FPI2         = ptrQryDS->FieldByName("FPI2")->AsFloat;
            padraorede->FPS1         = ptrQryDS->FieldByName("FPS1")->AsFloat;
            padraorede->FPS2         = ptrQryDS->FieldByName("FPS2")->AsFloat;

            // !OOPS! Deveria gerar um erro aqui (retornar false)???
            if (!geral->InserePadraoRede(padraorede))
               {
               SNPSTRLOGLINE(strlog);
               }

            ptrQryDS->Next();
            // loop registros (end)
            }

         ptrQryDS->Close();
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

      //destrói objetos locais
      if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_rdMercado(VTMercado* mercado)
   {
   bool           sucesso  = true;
   TADOQuery*     ptrQryDS = NULL;
   AnsiString     strSQL;
   AnsiString     strlog;
	int            intTipoRegiaoId;
	int            intSegmentoId;
	int            intSegmentoIdPrev;
   VTSegmento*    segmento;
   VTCrescimento* crescimento;
   VTClasse*      classe;
   TStream*       ptrStream = NULL;
   int            regiaoId;
   VTRegioes*     regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes));

   if (!bd_conn)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!bd_conn->getConnectedOK())     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!classes)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_CLASSESINV return(NULL); }
   if (!mercado)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_MERCADOINV return(NULL); }
   if (!regioes)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_REGIAOINV  return(NULL); }

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
         // Monta a string de execução
         strSQL = "EXEC SPSNP_rdRegiao";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         // Comando da consulta
         if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(NULL); }
         if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(NULL); }

         // Os registros vêm ordenados por região
			intSegmentoIdPrev = IDINDEFINIDO;
			// Recupera o resultado
         for (int i = 0; i < ptrQryDS->RecordCount; i++)
            {
            // loop registros (ini)
				intSegmentoId = ptrQryDS->FieldByName("RegiaoId")->AsInteger;
				if (intSegmentoId != intSegmentoIdPrev)
					{
               // Nova região
					intSegmentoIdPrev = intSegmentoId;
					segmento = DLL_NewObjSegmento();
					segmento->Id = intSegmentoId;
					segmento->Tipo = ptrQryDS->FieldByName("TipoRegiaoId")->AsInteger;
					switch(segmento->Tipo)
                  {
						case smINDEF    :
						case smDEFAULT  : segmento->Codigo = "Default";
												break;

                  case smREGIAO   : segmento->Codigo   = ptrQryDS->FieldByName("Codigo")->AsString;
                                    regiaoId = ptrQryDS->FieldByName("AreaGeoId")->AsInteger;
                                    segmento->Regiao = regioes->ExisteRegiao(regiaoId);
                                    break;

						case smTIPOREDE :
						case smREDE     : segmento->Codigo   = ptrQryDS->FieldByName("Codigo")->AsString;
												break;
						case smAREA     : segmento->Area->x1 = ptrQryDS->FieldByName("Utm1X")->AsFloat;
												segmento->Area->y1 = ptrQryDS->FieldByName("Utm1Y")->AsFloat;
												segmento->Area->x2 = ptrQryDS->FieldByName("Utm2X")->AsFloat;
												segmento->Area->y2 = ptrQryDS->FieldByName("Utm2Y")->AsFloat;
												break;
						case smCARGA    : segmento->Codigo   = ptrQryDS->FieldByName("Codigo")->AsString;
												break;
                  }
					mercado->InsereSegmento(segmento);
               }

            crescimento = DLL_NewObjCrescimento();
            crescimento->Id = IDINDEFINIDO;
				crescimento->Segmento = segmento;
				if ((classe = classes->ExisteClasseTag(ptrQryDS->FieldByName("TipoConsTag")->AsInteger)) == NULL)
               {
               MSGERRBDRD_CLASSE
               SNPSTRLOGLINE(strlog);
               sucesso = false;
               break;
               }
            crescimento->Classe = classe;

            ptrStream = ptrQryDS->CreateBlobStream(ptrQryDS->FieldByName("Pontos"), bmRead);
            if (!ptrStream)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDRD_CURCRESCIMENTO
               break;
               }
            if (!bd_func->DesmontaStreamCurvaCrescimento(crescimento, ptrStream))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDRD_CURCRESCIMENTO
               sucesso = false;
               break;
               }
            //insere Crescimento na Regiao
				mercado->InsereCrescimento(segmento, crescimento);

            ptrQryDS->Next();
            // loop registros (end)
            }

         ptrQryDS->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         }
      }
   __finally
      {
      // Colocado dentro do finally para garantir que será deletado, mesmo que
      // haja um return no meio do código

      //destrói objetos locais
      if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_rdAreasGeograficas(void)
   {
   bool        sucesso  = true;
   TADOQuery*  ptrQryDS = NULL;
   AnsiString  strSQL;
   AnsiString  strlog;
   VTRegioes*  regioes = NULL;
   VTRegiao*   regiao = NULL;
   int         intAreaGeoId;
   TStream*    ptrStream;

   // Recarrega a lista de regiões geográficas da base de dados
   if (!bd_conn)                                                             { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!bd_conn->getConnectedOK())                                           { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if ((regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes))) == NULL) { SNPSTRLOGLINE(strlog); MSGERRSYS_REGIAOINV return(false); }

   //proteção para bases antigas sem tabela AreaGeografica
   if (! bd_conn->FindTable("AreaGeografica"))
      {
      Alerta("Base de dados desatualizada: não existe tabela regiões");
      return(true);
      }

   __try
      {
      try
         {
         // Monta a string de execução
         strSQL = "EXEC SPSNP_rdAreaGeografica";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         // Comando da consulta
         if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(NULL); }
         if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(NULL); }
			// Recupera o resultado
         for (int i = 0; i < ptrQryDS->RecordCount; i++)
            {
            // loop registros (ini)
				intAreaGeoId = ptrQryDS->FieldByName("AreaGeoId")->AsInteger;
            if ((regiao = regioes->ExisteRegiao(intAreaGeoId)) == NULL)
               {//cria uma nova Região e salva em Regioes
               regiao = DLL_NewObjRegiao(apl);
               regiao->Id = intAreaGeoId;
               regioes->InsereRegiao(regiao);
               }
            regiao->Codigo = ptrQryDS->FieldByName("Codigo")->AsString;
            ptrStream = ptrQryDS->CreateBlobStream(ptrQryDS->FieldByName("Pontos"), bmRead);
            if (ptrStream)
               {
               if (!bd_func->DesmontaStreamPoligono(ptrStream, regiao->Poligono))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDRD_POLIGONO
                  break;
                  }
               }

            ptrQryDS->Next();
            // loop registros (end)
            }

         ptrQryDS->Close();
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

      //destrói objetos locais
      if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
      }

   return(sucesso);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_rdAreasGeograficas(void)
   {
   Alerta("TBDgeralAccess::SPSNP_rdAreasGeograficas() - bloqueada");
   return(true);
   }
*/
//---------------------------------------------------------------------------
VTSuporte* __fastcall TBDgeralAccess::SPSNP_rdSuporte(int intSuporteId)
   {
   VTSuporte* suporte  = NULL;
   TADOQuery* ptrQryDS = NULL;
   AnsiString strSQL;
   AnsiString strlog;
   strSUPORTE str_suporte;
   int        intTipoSuporteId;
   int        intPontosTotal;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!monta)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_MONTAINV   return(NULL); }

   __try
      {
      try
         {
         // Monta a string de execução
         strSQL = "EXEC SPSNP_rdSuporte :p_SuporteId";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_SuporteId", PARAMIN_INT(intSuporteId)))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBD_PARAMINV
            return(NULL);
            }
         // Comando da consulta
         if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(NULL); }
         if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(NULL); }

         // Inicia a estrutura
         str_suporte.num_pto = 0;

         // Recupera o resultado
         for (int i = 0; i < ptrQryDS->RecordCount; i++)
            {
            // loop registros (ini)
            if (str_suporte.num_pto == 0)
               {
               intPontosTotal = ptrQryDS->FieldByName("NumPontosFix")->AsInteger;
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

            str_suporte.pto_fixacao[str_suporte.num_pto].id      = ptrQryDS->FieldByName("FixacaoItem")->AsInteger;
            str_suporte.pto_fixacao[str_suporte.num_pto].coord_x = ptrQryDS->FieldByName("FixacaoX")->AsFloat;
            str_suporte.pto_fixacao[str_suporte.num_pto].coord_y = ptrQryDS->FieldByName("FixacaoY")->AsFloat;
            str_suporte.pto_fixacao[str_suporte.num_pto].codigo  = ptrQryDS->FieldByName("FixacaoCodigo")->AsString;
            str_suporte.num_pto++;

            if (str_suporte.num_pto == intPontosTotal)
               {
               str_suporte.id        = ptrQryDS->FieldByName("SuporteId")->AsInteger;
               str_suporte.codigo    = ptrQryDS->FieldByName("Descricao")->AsString;
//jcg@2013.08.08
               str_suporte.extern_id = ptrQryDS->FieldByName("ExtId")->AsString;
               str_suporte.vmin      = ptrQryDS->FieldByName("Vmin")->AsFloat;
               str_suporte.vmax      = ptrQryDS->FieldByName("Vmax")->AsFloat;
               str_suporte.tipo      = ptrQryDS->FieldByName("TipoSuporteId")->AsInteger;
               str_suporte.status    = ptrQryDS->FieldByName("Status")->AsInteger;
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

            ptrQryDS->Next();
            // loop registros (end)
            }

         ptrQryDS->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         suporte = NULL;
         }
      }
   __finally
      {
      // Colocado dentro do finally para garantir que será deletado, mesmo que
      // haja um return no meio do código

      //destrói objetos locais
      if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
      }

   return(suporte);
   }  // SPSNP_rdSuporte

//---------------------------------------------------------------------------
int __fastcall TBDgeralAccess::SPSNP_rdSuporteUtilizado(int intSuporteId)
   {
   int        intret   = IDINDEFINIDO;
   TADOQuery* ptrQryDS = NULL;
   AnsiString strSQL;
   AnsiString strlog;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }

   __try
      {
      try
         {
         // Passo 01
         // Monta a string de execução
         strSQL = "EXEC SPSNP_rdSuporteUtilizado_p01 :p_SuporteId";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_SuporteId", PARAMIN_INT(intSuporteId)))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBD_PARAMINV
            return(IDINDEFINIDO);
            }
         // Comando da consulta
         if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(IDINDEFINIDO); }
         if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(IDINDEFINIDO); }
         if (ptrQryDS->RecordCount == 0)                 { SNPSTRLOGLINE(strlog); return(IDINDEFINIDO); }
         // Recupera o resultado
         intret =  ptrQryDS->FieldByName("SuporteId")->AsInteger;
         // Fecha o recordset
         ptrQryDS->Close();
         // Delete o objeto criado (SOMENTE PARA OS INTERMEDIÁRIOS)
         if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}

         // Passo 02
         // Monta a string de execução
         strSQL = "EXEC SPSNP_rdSuporteUtilizado_p02 :p_SuporteId";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_SuporteId", PARAMIN_INT(intSuporteId)))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBD_PARAMINV
            return(IDINDEFINIDO);
            }
         // Comando da consulta
         if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(IDINDEFINIDO); }
         if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(IDINDEFINIDO); }
         if (ptrQryDS->RecordCount == 0)                 { SNPSTRLOGLINE(strlog); return(IDINDEFINIDO); }
         // Recupera o resultado e soma ao anterior
         intret +=  ptrQryDS->FieldByName("SuporteId")->AsInteger;
         // Fecha o recordset
         ptrQryDS->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         intret = IDINDEFINIDO;
         }
      }
   __finally
      {
      // Colocado dentro do finally para garantir que será deletado, mesmo que
      // haja um return no meio do código

      //destrói objetos locais
      if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
      }

   return(intret);
   } // SPSNP_rdSuporteUtilizado

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_rdSuporteUtilizado(int intSuporteId, TStringList* arrlist)
   {
   int        intret   = IDINDEFINIDO;
   TADOQuery* ptrQryDS = NULL;
   AnsiString strSQL;
   AnsiString strlog;
   int        intArranjoId;
   AnsiString strArranjoDescr;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(IDINDEFINIDO); }

   __try
      {
      try
         {
         // Monta a string de execução
         strSQL = "EXEC SPSNP_rdSuporteUtilizado_p03 :p_SuporteId";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_SuporteId", PARAMIN_INT(intSuporteId)))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBD_PARAMINV
            return(IDINDEFINIDO);
            }
         // Comando da consulta
         if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(IDINDEFINIDO); }
         if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(IDINDEFINIDO); }

         if (ptrQryDS->RecordCount == 0)                 { SNPSTRLOGLINE(strlog); return(IDINDEFINIDO); }
         // Recupera o resultado e soma ao anterior
         intret +=  ptrQryDS->FieldByName("SuporteId")->AsInteger;

         for (int i = 0; i < ptrQryDS->RecordCount; i++)
            {
            intArranjoId = ptrQryDS->FieldByName("ArranjoId")->AsInteger;
            strArranjoDescr = ptrQryDS->FieldByName("ArranjoDescricao")->AsString;
            arrlist->AddObject(strArranjoDescr, (TObject*)intArranjoId);
            ptrQryDS->Next();
            }

         // Fecha o recordset
         ptrQryDS->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         intret = IDINDEFINIDO;
         }
      }
   __finally
      {
      // Colocado dentro do finally para garantir que será deletado, mesmo que
      // haja um return no meio do código

      //destrói objetos locais
      if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
      }

   return(intret);
   } // SPSNP_rdSuporteUtilizado

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_rdTipoChave(void)
   {
   bool          sucesso  = true;
   TADOQuery*    ptrQryDS = NULL;
   AnsiString    strSQL;
   AnsiString    strlog;
   int           tipochave_id;
   VTTipoChave*  tipochave;
   unsigned      intStatus;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!tipos)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_TIPOSINV   return(false); }

   __try
      {
      try
         {
         // Monta a string de execução
         strSQL = "EXEC SPSNP_rdTipoChave";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         // Comando da consulta
         if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(false); }
         if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(false); }
         // Recupera o resultado
         for (int i = 0; i < ptrQryDS->RecordCount; i++)
            {
            // loop registros (ini)
            tipochave_id = ptrQryDS->FieldByName("intTipoChaveId")->AsInteger;
            if (! tipos->ExisteTipoChave(tipochave_id))
               {//cria objeto VTTipoChave
               if ((tipochave = DLL_NewObjTipoChave()) == NULL) return(false);
               //lê dados do TipoChave
               tipochave->Id               = tipochave_id;
               tipochave->Codigo           = ptrQryDS->FieldByName("strDescricao")->AsString;
               tipochave->Tipo             = tipochave_id;
               tipochave->StatusAsUnsigned = ptrQryDS->FieldByName("intStatus")->AsInteger;
//jcg@2013.08.08
               tipochave->Color            = TColor(ptrQryDS->FieldByName("intCorId")->AsInteger);
               //insere TipoChave Redes
               tipos->InsereTipoChave(tipochave);
               }
            ptrQryDS->Next();
            // loop registros (end)
            }

         ptrQryDS->Close();
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

      //destrói objetos locais
      if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
      }

   return(sucesso);
   } // DBRED_TipoChave

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_rdTipoConsumidor(void)
   {
   bool          sucesso  = true;
   TADOQuery*    ptrQryDS = NULL;
   AnsiString    strSQL;
   AnsiString    strlog;
   VTClasse*     classe;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!classes)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_CLASSESINV return(false); }

   __try
      {
      try
         {
         // Monta a string de execução
         strSQL = "EXEC SPSNP_rdTipoConsumidor";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         // Comando da consulta
         if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(false); }
         if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(false); }
         // Recupera o resultado
         for (int i = 0; i < ptrQryDS->RecordCount; i++)
            {
            // loop registros (ini)
            // OOPZ: Não deveria testar se a classe já existe? No Oracle também não está testando...
            if ((classe = DLL_NewObjClasse()) == NULL) continue;
            // lê dados da Classe
            classe->Id     = ptrQryDS->FieldByName("TipoConsId")->AsInteger;
            classe->Tag    = ptrQryDS->FieldByName("Tag")->AsInteger;
            classe->Codigo = ptrQryDS->FieldByName("Descricao")->AsString;
            classe->Tipico = ptrQryDS->FieldByName("Tipico")->AsInteger;
            //insere Classe em Classes
            classes->InsereClasse(classe);

            ptrQryDS->Next();
            // loop registros (end)
            }

         ptrQryDS->Close();
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

      //destrói objetos locais
      if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
      }

   return(sucesso);
   } // SPSNP_rdTipoConsumidor

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_rdTipoRede(int intEstudoId)
   {
   bool          sucesso  = true;
   TADOQuery*    ptrQryDS = NULL;
   AnsiString    strSQL;
   AnsiString    strlog;
   strTIPO_REDE  str_tipo_rede;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!monta)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_MONTAINV   return(false); }

   __try
      {
      try
         {
         // Monta a string de execução
         strSQL = "EXEC SPSNP_rdTipoRede :p_EstudoId";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_EstudoId", PARAMIN_INT(intEstudoId)))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBD_PARAMINV
            return(false);
            }
         // Comando da consulta
         if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); return(false); }
         if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(false); }
         // Recupera o resultado
         for (int i = 0; i < ptrQryDS->RecordCount; i++)
            {
            // loop registros (ini)
            str_tipo_rede.id        = ptrQryDS->FieldByName("intTipoRedeId")->AsInteger;
            str_tipo_rede.segmento  = str_tipo_rede.id;
            str_tipo_rede.codigo    = ptrQryDS->FieldByName("strCodigo")->AsString;
            str_tipo_rede.descricao = ptrQryDS->FieldByName("strDescricao")->AsString;
            str_tipo_rede.qtde_rede = ptrQryDS->FieldByName("intQuantidade")->AsInteger;
            str_tipo_rede.vmin_kv   = ptrQryDS->FieldByName("fltVmin")->AsFloat;
            str_tipo_rede.vmax_kv   = ptrQryDS->FieldByName("fltVmax")->AsFloat;
//jcg@2013.08.08
            str_tipo_rede.color     = TColor(ptrQryDS->FieldByName("intColorId")->AsInteger);
            str_tipo_rede.status    = TColor(ptrQryDS->FieldByName("intStatus")->AsInteger);
            if (monta->InsereTipoRede(str_tipo_rede) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDRD_TIPOREDE2
               sucesso = false;
               break;
               }
            ptrQryDS->Next();
            // loop registros (end)
            }

         ptrQryDS->Close();
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

      //destrói objetos locais
      if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
      }

   return(sucesso);
   } // SPSNP_rdTipoRede

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SalvaMercado(void)
   {
   //variáveis locais
   AnsiString strlog;
   VTMercado  *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

   if (!bd_conn)                                   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_curva)                                  { SNPSTRLOGLINE(strlog); MSGERRSYS_BDCURVAINV return(false); }
   if (!mercado)                                   { SNPSTRLOGLINE(strlog); MSGERRSYS_MERCADOINV return(false); }
   // Reinicia os indexadores pertinentes
   FNSNP_rdSegmentoIdProx(false, true);
   bd_curva->FNSNP_rdConjCurvaIdProx(false, true);
   //executa gravação
   return(SPSNP_wrMercado(mercado));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SalvaGeral(VTGeral* geral)
   {

   if (SPSNP_wrGeralCorCirc(geral))
	  {
	  return(SPSNP_wrGeralPadraoRede(geral));
	  }

   return(false);
   }

//---------------------------------------------------------------------------
int __fastcall TBDgeralAccess::SuporteUtilizado(int intSuporteId)
   {
   return(-1);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SuporteUtilizado(int intSuporteId, TStringList* arrlist)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_dlAreaGeografica(void)
   {
   bool       sucesso  = true;
   AnsiString strSQL;
   AnsiString strlog;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!monta)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_MONTAINV   return(NULL); }

   //proteção para bases antigas sem tabela AreaGeografica
   if (! bd_conn->FindTable("AreaGeografica"))
      {
      Alerta("Base de dados desatualizada: não existe tabela regiões");
      return(true);
      }

   __try
      {
      try
         {

         // Monta a string de execução
         strSQL = "EXEC SPSNP_dlAreaGeografica";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         // Comando da consulta
         if (!bd_conn->Execute(false))                 { SNPSTRLOGLINE(strlog); return(NULL); }
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
 /*
 //---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_dlAreaGeografica(void)
   {
   Alerta("TBDgeralAccess::SPSNP_dlAreaGeografica() - bloqueada");
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_dlArranjoCaboTipico(void)
   {
   bool       sucesso  = true;
   AnsiString strSQL;
   AnsiString strlog;
   int        numStep;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!monta)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_MONTAINV   return(NULL); }

   __try
      {
      try
         {
         for (numStep = 0; numStep < 7; numStep++)
            {
            // Loop de passos a serem executados (ini)

            // Monta a string de execução
            strSQL = Format("SPSNP_dlArranjoCaboTipico_p%.2d", ARRAYOFCONST((numStep)));
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            // Comando da consulta
            if (!bd_conn->Execute(false))                 { SNPSTRLOGLINE(strlog); return(NULL); }
            // Loop de passos a serem executados (fim)
            }
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         strlog = Format("SPSNP_dlArranjoCaboTipico: erro no passo %.2d", ARRAYOFCONST((numStep)));
         SNPSTRLOG(strlog);
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
//***************************************************************************
//***************************************************************************
// OOPS Isso aqui não está sendo chamado em lugar nenhum (NEM NO ORACLE)
//***************************************************************************
//***************************************************************************
//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_dlCenario(void)
   {
   bool       sucesso  = true;
   AnsiString strSQL;
   AnsiString strlog;

   if (!bd_conn)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!bd_conn->getConnectedOK())     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }

   __try
      {
      try
         {
         // Monta a string de execução
         strSQL = "EXEC SPSNP_dlCenario";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(NULL); }

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
bool __fastcall TBDgeralAccess::SPSNP_dlGeralCorCirc(void)
   {
   bool       sucesso  = true;
   AnsiString strSQL;
   AnsiString strlog;

   if (!bd_conn)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!bd_conn->getConnectedOK())     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }

   __try
      {
      try
         {
         // Monta a string de execução
         strSQL = "EXEC SPSNP_dlGeralCorCirc";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(NULL); }

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
bool __fastcall TBDgeralAccess::SPSNP_dlGeralPadraoRede(void)
   {
   bool       sucesso  = true;
   AnsiString strSQL;
   AnsiString strlog;

   if (!bd_conn)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!bd_conn->getConnectedOK())     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }

   __try
      {
      try
         {
         // Monta a string de execução
         strSQL = "EXEC SPSNP_dlGeralPadraoRede";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(NULL); }

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
bool __fastcall TBDgeralAccess::SPSNP_dlSegmentoMercado(VTSegmento* segmento)
	{
   bool       sucesso  = true;
   AnsiString strSQL;
   AnsiString strlog;

   if (!bd_conn)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }

   __try
      {
      try
         {
         // Passo 01
         // Monta a string de execução
         strSQL = "EXEC SPSNP_dlRegiao_p03 :p_RegiaoId";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_RegiaoId", PARAMIN_INT(segmento->Id)))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBD_PARAMINV
            return(false);
            }
         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(NULL); }

         // Passo 02
         // Monta a string de execução
         strSQL = "EXEC SPSNP_dlRegiao_p01 :p_RegiaoId";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_RegiaoId", PARAMIN_INT(segmento->Id)))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBD_PARAMINV
            return(false);
            }
         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(NULL); }

         // Passo 03
         // Monta a string de execução
			strSQL = "EXEC SPSNP_dlRegiao_p02 :p_RegiaoId";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_RegiaoId", PARAMIN_INT(segmento->Id)))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBD_PARAMINV
            return(false);
            }
         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(NULL); }

         // Passo 04
         // Monta a string de execução
			strSQL = "EXEC SPSNP_dlRegiao_p04 :p_RegiaoId";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_RegiaoId", PARAMIN_INT(segmento->Id)))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBD_PARAMINV
            return(false);
            }
         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(NULL); }
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
bool __fastcall TBDgeralAccess::SPSNP_dlSuporteFixacao(int intSuporteId)
   {
   bool       sucesso  = true;
   AnsiString strSQL;
   AnsiString strlog;

   if (!bd_conn)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!bd_conn->getConnectedOK())     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }

   __try
      {
      try
         {
         // Monta a string de execução
         strSQL = "EXEC SPSNP_dlSuporteFixacao :p_SuporteId";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_SuporteId", PARAMIN_INT(intSuporteId)))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBD_PARAMINV
            return(NULL);
            }
         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(NULL); }

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
bool __fastcall TBDgeralAccess::SPSNP_wrArranjo(VTArranjo *arranjo, bool bolTipico)
   {
   bool       sucesso  = true;
   AnsiString strSQL;
   AnsiString strlog;
   int        intArranjoId;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!arranjo)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_ARRANJOINV return(false); }

   // Inicialmente escreve o arranjo, determinando o número do próximo arranjo a ser gerado (ou não)
   if (arranjo->TipoCfg == arranjoMUTUA)
      return(true);

   // Escrita propriamente dita
   __try
      {
      // Verifica se o objeto é novo ou alterado
      if (arranjo->StatusNovo())
         {
         // Objeto novo. Tem que atualizar em memória o valor do identificador
         // retornado pela base de dados.
         intArranjoId = FNSNP_rdArranjoIdProx();
         // Trata-se de uma inserção
         strSQL  = "EXEC SPSNP_wrArranjo_p01 :p_TipoArranjoId, :p_Descricao, :p_ExtId, :p_Tipico, :p_ResTerra, ";
         strSQL += ":p_Vmin, :p_Vmax, :p_ColorId, :p_Status, :p_NumCabos, :p_TipoFase, :p_DataVersao, :p_ArranjoId";
         }
      else if (arranjo->StatusAlterado())
         {
         // Objeto previamente existente. O identificador em memória é válido.
         intArranjoId = arranjo->Id;
         // Trata-se de uma atualização
         strSQL  = "EXEC SPSNP_wrArranjo_p02 :p_TipoArranjoId, :p_Descricao, :p_ExtId, :p_Tipico, :p_ResTerra, ";
         strSQL += ":p_Vmin, :p_Vmax, :p_ColorId, :p_Status, :p_NumCabos, :p_TipoFase, :p_DataVersao, :p_ArranjoId";
         }
         else
            {
            // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
            return(true);
            }
      try
         {
			//JCG@2015.01.05: inclui no Status a informação de ramal
			arranjo->Status[sttRAMAL] = arranjo->ramal;
			// Monta a string de execução
         // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if(!bd_conn->SetParam("p_TipoArranjoId", PARAMIN_INT(arranjo->TipoCfg)))             { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if(!bd_conn->SetParam("p_Descricao"    , PARAMIN_STR(arranjo->Codigo)))              { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if(!bd_conn->SetParam("p_ExtId"        , PARAMIN_STR(arranjo->Extern_id)))           { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if(!bd_conn->SetParam("p_Tipico"       , PARAMIN_INT((bolTipico ? 1 : 0))))          { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if(!bd_conn->SetParam("p_ResTerra"     , PARAMIN_FLT(arranjo->ResTerra)))            { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if(!bd_conn->SetParam("p_Vmin"         , PARAMIN_FLT(arranjo->Vmin_KV)))             { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if(!bd_conn->SetParam("p_Vmax"         , PARAMIN_FLT(arranjo->Vmax_KV)))             { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if(!bd_conn->SetParam("p_ColorId"      , PARAMIN_INT(arranjo->Color)))               { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if(!bd_conn->SetParam("p_Status"       , PARAMIN_INT(arranjo->StatusAsUnsigned)))    { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if(!bd_conn->SetParam("p_NumCabos"     , PARAMIN_INT(arranjo->NumeroCabos)))         { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if(!bd_conn->SetParam("p_TipoFase"     , PARAMIN_INT(arranjo->Fases)))               { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if(!bd_conn->SetParam("p_DataVersao"   , PARAMIN_DTT(TDateTime::CurrentDateTime()))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if(!bd_conn->SetParam("p_ArranjoId"    , PARAMIN_INT(intArranjoId)))                 { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
         // Atualiza o identificador do arranjo
         arranjo->Id = intArranjoId;
         // Dependendo do tipo de arranjo, chama a procedure correspondente
         switch (arranjo->TipoCfg)
            {
            case arranjoCABOxFASE : sucesso = SPSNP_wrArranjo00(arranjo); break;
            case arranjoCABO_Z0Z1 : sucesso = SPSNP_wrArranjo01(arranjo); break;
            case arranjoZ0Z1      : sucesso = SPSNP_wrArranjo02(arranjo); break;
            case arranjoMAT_Z     : sucesso = SPSNP_wrArranjo03(arranjo); break;
            case arranjoPU_SB100  : sucesso = SPSNP_wrArranjo04(arranjo); break;
            default               : { SNPSTRLOGLINE(strlog); return(false); }
            }
         // Atualiza o estado do arranjo, caso a execução tenha ocorrido sem problemas
         if (sucesso)
            {
            // Define status
            arranjo->DefineStatus(sttNOVO,     false);
            arranjo->DefineStatus(sttALTERADO, false);
            }
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
bool __fastcall TBDgeralAccess::SPSNP_wrArranjo00(VTArranjo *arranjo)
   {
   bool         sucesso = true;
   AnsiString   strSQL;
   TStringList* lstSQL  = NULL;
   AnsiString   strlog;
   bool         bolInsert;
   int          numStep;
   int          intTipoFase; 

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!arranjo)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_ARRANJOINV return(false); }

   // Escrita propriamente dita
   __try
      {
      lstSQL = new TStringList();

      // Verifica se o objeto é novo ou alterado
      if (arranjo->StatusNovo())
         {
         // Trata-se de uma inserção
         bolInsert = true;

         strSQL = "EXEC SPSNP_wrArranjo00_p02 :p_SuporteId, :p_FixacaoItem, :p_ArranjoId, :p_CaboId, :p_TipoFaseId ";
         lstSQL->Add(strSQL);
         strSQL = "EXEC SPSNP_wrArranjo00_p03 :p_SuporteId, :p_FixacaoItem, :p_ArranjoId, :p_CaboId, :p_TipoFaseId ";
         lstSQL->Add(strSQL);
         strSQL = "EXEC SPSNP_wrArranjo00_p04 :p_SuporteId, :p_FixacaoItem, :p_ArranjoId, :p_CaboId, :p_TipoFaseId ";
         lstSQL->Add(strSQL);
         strSQL = "EXEC SPSNP_wrArranjo00_p05 :p_SuporteId, :p_FixacaoItem, :p_ArranjoId, :p_CaboId, :p_TipoFaseId ";
         lstSQL->Add(strSQL);
         }
      else if (arranjo->StatusAlterado())
         {
         // Trata-se de uma atualização
         bolInsert = false;

         strSQL = "EXEC SPSNP_wrArranjo00_p06 :p_SuporteId, :p_FixacaoItem, :p_ArranjoId, :p_CaboId, :p_TipoFaseId, :p_TipoArranjoId ";
         lstSQL->Add(strSQL);
         strSQL = "EXEC SPSNP_wrArranjo00_p07 :p_SuporteId, :p_FixacaoItem, :p_ArranjoId, :p_CaboId, :p_TipoFaseId, :p_TipoArranjoId ";
         lstSQL->Add(strSQL);
         strSQL = "EXEC SPSNP_wrArranjo00_p08 :p_SuporteId, :p_FixacaoItem, :p_ArranjoId, :p_CaboId, :p_TipoFaseId, :p_TipoArranjoId ";
         lstSQL->Add(strSQL);
         strSQL = "EXEC SPSNP_wrArranjo00_p09 :p_SuporteId, :p_FixacaoItem, :p_ArranjoId, :p_CaboId, :p_TipoFaseId, :p_TipoArranjoId ";
         lstSQL->Add(strSQL);
         }
         else
            {
            // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
            return(true);
            }

      try
         {
         for (numStep = 0; numStep < lstSQL->Count; numStep++)
            {
            // Loop de passos a serem executados (ini)

            // Nesse caso, particularmente, os passos correspondem às fases.
            // O teste abaixo é somente para garantia
            if (lstSQL->Count > MAX_FASE) { SNPSTRLOGLINE(strlog); MSGERRSYS_ARRANJOINV return(false); }

            // O número de passos corresponde ao número de fases no cabo. Se a fase é nula, ignora a execução
            if (arranjo->fase[numStep].cabo == NULL)
               continue;
            // Determina o tipo de fase
            switch (numStep)
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

            // Monta a string de execução
            // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
            // Seleciona o comando a ser executado
            strSQL = lstSQL->Strings[numStep];
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            if (!bd_conn->SetParam("p_SuporteId"    , PARAMIN_INT(arranjo->suporte->Id)))                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_FixacaoItem"  , PARAMIN_INT(arranjo->fase[numStep].pto_fixacao->id))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_ArranjoId"    , PARAMIN_INT(arranjo->Id)))                            { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_CaboId"       , PARAMIN_INT(arranjo->fase[numStep].cabo->Id)))        { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_TipoFaseId"   , PARAMIN_INT(intTipoFase)))                            { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bolInsert)
               {
               if (!bd_conn->SetParam("p_TipoArranjoId", PARAMIN_INT(arranjo->TipoCfg)))                       { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
               }
            // Comando da consulta
            if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }

            // Loop de passos a serem executados (fim)
            }
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
      if (lstSQL) { delete lstSQL; lstSQL = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_wrArranjo01(VTArranjo *arranjo)
   {
   bool            sucesso = true;
   AnsiString      strSQL;
   AnsiString      strlog;
   bool            bolInsert;
   VTPtoFixacao*   ptrPtoFixacao;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!arranjo)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_ARRANJOINV return(false); }

   // Escrita propriamente dita
   __try
      {
      // Verifica se o objeto é novo ou alterado
      if (arranjo->StatusNovo())
         {
         // Trata-se de uma inserção
         bolInsert = true;

         strSQL = "EXEC SPSNP_wrArranjo01_p01 :p_TipoFaseId, :p_SuporteId, :p_FixacaoItem, :p_ArranjoId, :p_CaboId ";
         }
      else if (arranjo->StatusAlterado())
         {
         // Trata-se de uma atualização
         bolInsert = false;

         strSQL = "EXEC SPSNP_wrArranjo01_p02 :p_SuporteId, :p_FixacaoItem, :p_ArranjoId, :p_CaboId ";
         }
         else
            {
            // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
            return(true);
            }

      try
         {
         // Monta a string de execução
         // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if ((arranjo->suporte)&&(arranjo->suporte->LisPtoFixacao()->Count > 0))
            {//assume primeiro PtoFixacao disponível
            if ((ptrPtoFixacao = (VTPtoFixacao*)arranjo->suporte->LisPtoFixacao()->Items[0]) == NULL)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_SuporteId"  , PARAMIN_INT(arranjo->suporte->Id))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_FixacaoItem", PARAMIN_INT(ptrPtoFixacao->id)))    { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            }
         else
            {
            if (!bd_conn->SetParam("p_SuporteId"  , PARAMIN_INT(IDINDEFINIDO)))         { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_FixacaoItem", PARAMIN_INT(IDINDEFINIDO)))         { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            }
         if (!bd_conn->SetParam("p_ArranjoId"    , PARAMIN_INT(arranjo->Id)))           { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_CaboId"       , PARAMIN_INT(arranjo->caboZ0Z1->Id))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (bolInsert)
            {
            if (!bd_conn->SetParam("p_TipoFaseId", PARAMIN_INT(arranjo->Fases)))        { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            }

         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
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
bool __fastcall TBDgeralAccess::SPSNP_wrArranjo02(VTArranjo *arranjo)
   {
   bool         sucesso = true;
   AnsiString   strSQL;
   AnsiString   strlog;
   int          intSuporteId;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!arranjo)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_ARRANJOINV return(false); }

   // Escrita propriamente dita
   __try
      {
      // Verifica se o objeto é novo ou alterado
      if (arranjo->StatusNovo())
         {
         // Trata-se de uma inserção
         strSQL = "EXEC SPSNP_wrArranjo02_p01 :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X, :p_C0, :p_C1, :p_Iadm, :p_SuporteId, :p_ArranjoId";
         }
      else if (arranjo->StatusAlterado())
         {
         // Trata-se de uma atualização
         strSQL = "EXEC SPSNP_wrArranjo02_p02 :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X, :p_C0, :p_C1, :p_Iadm, :p_SuporteId, :p_ArranjoId";
         }
         else
            {
            // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
            return(true);
            }

      try
         {
         // Monta a string de execução
         // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         intSuporteId = (arranjo->suporte ? arranjo->suporte->Id : IDINDEFINIDO);

         if (!bd_conn->SetParam("p_Z0R"          , PARAMIN_FLT(arranjo->z0.r)))     { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Z0X"          , PARAMIN_FLT(arranjo->z0.x)))     { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Z1R"          , PARAMIN_FLT(arranjo->z1.r)))     { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Z1X"          , PARAMIN_FLT(arranjo->z1.x)))     { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_C0"           , PARAMIN_FLT(arranjo->c0)))       { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_C1"           , PARAMIN_FLT(arranjo->c1)))       { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Iadm"         , PARAMIN_FLT(arranjo->Iadm_amp))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_SuporteId"    , PARAMIN_INT(intSuporteId)))      { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_ArranjoId"    , PARAMIN_INT(arranjo->Id)))       { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }

         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
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
bool __fastcall TBDgeralAccess::SPSNP_wrArranjo03(VTArranjo *arranjo)
   {
   bool         sucesso = true;
   AnsiString   strSQL;
   AnsiString   strlog;
   TStream*     ptrStreamMatC = NULL;
   TStream*     ptrStreamMatZ = NULL;
   TParameter*  ptrParam; 

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!arranjo)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_ARRANJOINV return(false); }
   if (!bd_func)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   // Escrita propriamente dita
   __try
      {
      // Verifica se o objeto é novo ou alterado
      if (arranjo->StatusNovo())
         {
         // Trata-se de uma inserção
         strSQL = "EXEC SPSNP_wrArranjo03_p01 :p_MatZ, :p_MatC, :p_Iadm, :p_ArranjoId";
         }
      else if (arranjo->StatusAlterado())
         {
         // Trata-se de uma atualização
         strSQL = "EXEC SPSNP_wrArranjo03_p02 :p_MatZ, :p_MatC, :p_Iadm, :p_ArranjoId";
         }
         else
            {
            // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
            return(true);
            }

      try
         {
         // Monta a string de execução
         // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         bd_conn->SetParam("p_Iadm"     , PARAMIN_FLT(arranjo->Iadm_amp));
         bd_conn->SetParam("p_ArranjoId", PARAMIN_INT(arranjo->Id));
         // Acerta os valores dos blobs
         if ((ptrStreamMatC = bd_func->MontaStreamVTMatC(arranjo->MatC)) == NULL)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_MATCSTREAM
            return(false);
            }
         ptrParam = bd_conn->SetParam("p_MatC", PARAMIN_BLB(NULL));
         ptrParam->LoadFromStream(ptrStreamMatC, ptrParam->DataType);

         if ((ptrStreamMatZ = bd_func->MontaStreamVTMatZ(arranjo->MatZ)) == NULL)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_MATZSTREAM
            return(false);
            }
         ptrParam = bd_conn->SetParam("p_MatZ", PARAMIN_BLB(NULL));
         ptrParam->LoadFromStream(ptrStreamMatZ, ptrParam->DataType);

         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
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
      if (ptrStreamMatC) { delete ptrStreamMatC; ptrStreamMatC = NULL; }
      if (ptrStreamMatZ) { delete ptrStreamMatZ; ptrStreamMatZ = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_wrArranjo04(VTArranjo *arranjo)
   {
   return(SPSNP_wrArranjo02(arranjo));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_wrCabo(VTCabo *cabo)
   {
   bool         sucesso = true;
   AnsiString   strSQL;
   AnsiString   strlog;
   int          intCaboId;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!cabo)                        { SNPSTRLOGLINE(strlog); MSGERRSYS_CABOINV    return(false); }

   // Escrita propriamente dita
   __try
      {
      // Verifica se o objeto é novo ou alterado
      if (cabo->StatusNovo())
         {
         // Trata-se de uma inserção
         intCaboId = FNSNP_rdCaboIdProx();
         strSQL    = "EXEC SPSNP_wrCabo_p01 :p_Descricao, :p_ExtId, :p_Inom, :p_Vmin, :p_Vmax, :p_Status, :p_CaboId, :p_TipoCaboId";
         }
      else if (cabo->StatusAlterado())
         {
         // Trata-se de uma atualização
         intCaboId = cabo->Id;
         strSQL    = "EXEC SPSNP_wrCabo_p02 :p_Descricao, :p_ExtId, :p_Inom, :p_Vmin, :p_Vmax, :p_Status, :p_CaboId, :p_TipoCaboId";
         }
         else
            {
            // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
            return(true);
            }

      try
         {
         // Monta a string de execução
         // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_Descricao" , PARAMIN_STR(cabo->Codigo)))           { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_ExtId"     , PARAMIN_STR(cabo->Extern_id)))        { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Inom"      , PARAMIN_FLT(cabo->Iadm_A)))           { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Vmin"      , PARAMIN_FLT(cabo->Vmin_KV)))          { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Vmax"      , PARAMIN_FLT(cabo->Vmax_KV)))          { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Status"    , PARAMIN_INT(cabo->StatusAsUnsigned))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_CaboId"    , PARAMIN_INT(intCaboId)))              { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_TipoCaboId", PARAMIN_INT(cabo->TipoCfg)))          { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
         // Atualiza o identificador do cabo
         cabo->Id = intCaboId;
         if (cabo->TipoCfg == caboR_GMR)
            {
            sucesso = SPSNP_wrCaboRGMR(cabo);
            }
         else
            {
            sucesso = SPSNP_wrCaboZ0Z1(cabo);
            }
         if (sucesso)
            {
            // Define status
            cabo->DefineStatus(sttNOVO,     false);
            cabo->DefineStatus(sttALTERADO, false);
            }
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
bool __fastcall TBDgeralAccess::SPSNP_wrCaboZ0Z1(VTCabo *cabo)
   {
   bool         sucesso = true;
   AnsiString   strSQL;
   AnsiString   strlog;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!cabo)                        { SNPSTRLOGLINE(strlog); MSGERRSYS_CABOINV    return(false); }

   // Escrita propriamente dita
   __try
      {
      // Verifica se o objeto é novo ou alterado
      if (cabo->StatusNovo())
         {
         // Trata-se de uma inserção
         strSQL = "EXEC SPSNP_wrCaboZ0Z1_p01 :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X, :p_C0, :p_C1, :p_CaboId";
         }
      else if (cabo->StatusAlterado())
         {
         // Trata-se de uma atualização
         strSQL = "EXEC SPSNP_wrCaboZ0Z1_p02 :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X, :p_C0, :p_C1, :p_CaboId";
         }
         else
            {
            // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
            return(true);
            }

      try
         {
         // Monta a string de execução
         // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_Z0R"   , PARAMIN_FLT(cabo->z0.r)))  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Z0X"   , PARAMIN_FLT(cabo->z0.x)))  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Z1R"   , PARAMIN_FLT(cabo->z1.r)))  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Z1X"   , PARAMIN_FLT(cabo->z1.x)))  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_C0"    , PARAMIN_FLT(cabo->c0)))    { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_C1"    , PARAMIN_FLT(cabo->c1)))    { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_CaboId", PARAMIN_INT(cabo->Id)))    { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
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
bool __fastcall TBDgeralAccess::SPSNP_wrCaboRGMR(VTCabo *cabo)
   {
   bool         sucesso = true;
   AnsiString   strSQL;
   AnsiString   strlog;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!cabo)                        { SNPSTRLOGLINE(strlog); MSGERRSYS_SUPORTEINV return(false); }

   // Escrita propriamente dita
   __try
      {
      // Verifica se o objeto é novo ou alterado
      if (cabo->StatusNovo())
         {
         // Trata-se de uma inserção
         strSQL = "EXEC SPSNP_wrCaboRGMR_p01 :p_R, :p_GMR, :p_CaboId";
         }
      else if (cabo->StatusAlterado())
         {
         // Trata-se de uma atualização
         strSQL = "EXEC SPSNP_wrCaboRGMR_p02 :p_R, :p_GMR, :p_CaboId";
         }
         else
            {
            // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
            return(true);
            }

      try
         {
         // Monta a string de execução
         // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_R"     , PARAMIN_FLT(cabo->r)))   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_GMR"   , PARAMIN_FLT(cabo->gmr))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_CaboId", PARAMIN_INT(cabo->Id)))  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
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
//***************************************************************************
//***************************************************************************
// OOPS Isso aqui não está sendo chamado em lugar nenhum (NEM NO ORACLE)
//***************************************************************************
//***************************************************************************
//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_wrCenario(VTCenario* cenario)
   {
   bool       sucesso = true;
   AnsiString strSQL;
   AnsiString strlog;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!cenario)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_CENARIOINV return(false); }

   // Escrita propriamente dita
   __try
      {
      // Verifica se o objeto é novo ou alterado
      if (cenario->Id < 0)
         {
         // Trata-se de uma inserção
         cenario->Id = FNSNP_rdCenarioIdProx();
         strSQL = "EXEC SPSNP_wrCenario_p01 :p_Descricao, :p_Taxa, :p_CenarioId";
         }
      else
         {
         // Trata-se de uma atualização
         strSQL = "EXEC SPSNP_wrCenario_p02 :p_Descricao, :p_Taxa, :p_CenarioId";
         }

      try
         {
         // Monta a string de execução
         // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_Descricao", PARAMIN_STR(cenario->Codigo)))  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Taxa"     , PARAMIN_INT(cenario->Taxa)))    { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_CenarioId", PARAMIN_INT(cenario->Id)))      { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
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

////---------------------------------------------------------------------------
//bool __fastcall TBDgeralAccess::SPSNP_wrGeralCorCirc(VTGeral* geral)
//   {
//   bool         sucesso = true;
//   AnsiString   strSQL;
//   AnsiString   strlog;
//   bool         bolInsert;
//
//   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
//   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
//   if (!geral)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_SUPORTEINV return(false); }
//
//   // Escrita propriamente dita
//   __try
//      {
//      // Apaga os registros existentes e escreve todos novamente
//      if (!SPSNP_dlGeralCorCirc())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SUPORTEINV return(false); }
//
//      try
//		 {
//         // Monta a string de execução
//         strSQL = "EXEC SPSNP_wrGeralCorCirc :p_CorCircVal, :p_CorCircId";
//         // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
//         // Decodifica comando
//         bd_conn->ParseCmd(strSQL);
//         // Loop de valores
//		 for (int i = 0; i < MAX_COR; i++)
//			{
//			// Determina os valores dos parâmetros
//			if (!bd_conn->SetParam("p_CorCircId" , PARAMIN_INT(i)))                       { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
//			if (!bd_conn->SetParam("p_CorCircVal", PARAMIN_INT((int)geral->COR.Circ[i]))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
//			// Comando da consulta
//			if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
//			}
//		 }
//	  catch (Exception& e)
//         {
//		 SNPSTRLOGLINE(strlog);
//		 SNPSTRLOG(e.Message);
//		 sucesso = false;
//		 }
//	  }
//   __finally
//	  {
//	  // Colocado dentro do finally para garantir que será deletado, mesmo que
//	  // haja um return no meio do código
//	  }
//
//   return(sucesso);
//   }
//---------------------------------------------------------------------------
// FKM 2019.02.22 - não estava sendo usado,
//                       roubei pra salvar as cores do diagnostico de fluxo
//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_wrGeralCorCirc(VTGeral* geral)
   {
   bool         sucesso = true;
   AnsiString   strSQL;
   AnsiString   strlog;
   bool         bolInsert;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!geral)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_SUPORTEINV return(false); }

   // Escrita propriamente dita
   __try
      {
      // Apaga os registros existentes e escreve todos novamente
      if (!SPSNP_dlGeralCorCirc())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SUPORTEINV return(false); }

      try
		 {
         // Monta a string de execução
         strSQL = "EXEC SPSNP_wrGeralCorCirc :p_CorCircVal, :p_CorCircId";
         // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Loop de valores
		 for (int i = 0; i < MAX_FAIXA+1; i++)
			{
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_CorCircId" , PARAMIN_INT(i)))                       { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
			if (!bd_conn->SetParam("p_CorCircVal", PARAMIN_INT((int)geral->COR.Faixa[i]))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
			// Comando da consulta
			if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
			}
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
bool __fastcall TBDgeralAccess::SPSNP_wrGeralPadraoRede(VTGeral* geral)
   {
   bool          sucesso = true;
   AnsiString    strSQL;
   AnsiString    strlog;
   bool          bolInsert;
   TList*        lisPADRAOREDE;
   VTPadraoRede* padraorede;

   if (!bd_conn)                                         { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())                       { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!geral)                                           { SNPSTRLOGLINE(strlog); MSGERRSYS_SUPORTEINV return(false); }
   if ((lisPADRAOREDE = geral->LisPadraoRede()) == NULL) { SNPSTRLOGLINE(strlog); MSGERRSYS_SUPORTEINV return(false); }

   // Escrita propriamente dita
   __try
      {
      // Apaga os registros existentes e escreve todos novamente
      if (!SPSNP_dlGeralPadraoRede()) { SNPSTRLOGLINE(strlog); MSGERRSYS_SUPORTEINV return(false); }

      try
         {
         // Monta a string de execução
         strSQL = "EXEC SPSNP_wrGeralPadraoRede :p_PadraoRedeId, :p_Descricao, :p_NumFases, :p_VFF, :p_VFN, :p_V1, :p_V2, :p_V3, :p_V4, :p_FPI1, :p_FPI2, :p_FPS1, :p_FPS2";
         // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Loop de valores
         for (int i = 0; i < lisPADRAOREDE->Count; i++)
            {
            if ((padraorede = (VTPadraoRede*)lisPADRAOREDE->Items[i]) == NULL) return(false);
            // Como apagou todos os parâmetros, cria novos identificadores e salva na base de dados
            //padraorede->PadraoRedeId = i + 1;   //FKM o padrao rede é o id do tipo rede, essa atribuicao gera confusao
            // Determina os valores dos parâmetros
            if (!bd_conn->SetParam("p_PadraoRedeId", PARAMIN_INT(padraorede->PadraoRedeId))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_Descricao"   , PARAMIN_STR(padraorede->Descricao)))    { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_NumFases"    , PARAMIN_INT(padraorede->NumFases)))     { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_VFF"         , PARAMIN_FLT(padraorede->VFF)))          { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_VFN"         , PARAMIN_FLT(padraorede->VFN)))          { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_V1"          , PARAMIN_FLT(padraorede->V1)))           { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_V2"          , PARAMIN_FLT(padraorede->V2)))           { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_V3"          , PARAMIN_FLT(padraorede->V3)))           { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_V4"          , PARAMIN_FLT(padraorede->V4)))           { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_FPI1"        , PARAMIN_FLT(padraorede->FPI1)))         { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_FPI2"        , PARAMIN_FLT(padraorede->FPI2)))         { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_FPS1"        , PARAMIN_FLT(padraorede->FPS1)))         { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_FPS2"        , PARAMIN_FLT(padraorede->FPS2)))         { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            // Comando da consulta
            if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
            }

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
/*
//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_wrSuporte(VTSuporte *suporte)
   {
   bool         sucesso = true;
   AnsiString   strSQL;
   AnsiString   strlog;
   bool         bolInsert;
   int          intSuporteId;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!suporte)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SUPORTEINV return(false); }

   // Escrita propriamente dita
   __try
      {
      // Verifica se o objeto é novo ou alterado
      if (suporte->StatusNovo())
         {
         // Trata-se de uma inserção
         bolInsert = true;

         intSuporteId = FNSNP_rdSuporteIdProx();
         strSQL = "EXEC SPSNP_wrSuporte_p01 :p_Descricao, :p_ExtId, :p_Vmin, :p_Vmax, :p_NumPontosFix, :p_Status, :p_SuporteId, :p_TipoSuporteId";
         }
      else if (suporte->StatusAlterado())
         {
         // Trata-se de uma atualização
         bolInsert = false;

         intSuporteId = suporte->Id;
         strSQL = "EXEC SPSNP_wrSuporte_p02 :p_Descricao, :p_ExtId, :p_Vmin, :p_Vmax, :p_NumPontosFix, :p_Status, :p_SuporteId";
         }
         else
            {
            // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
            return(true);
            }

      try
         {
         // Monta a string de execução
         // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_Descricao"   , PARAMIN_STR(suporte->Codigo)))                 { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_ExtId"       , PARAMIN_STR(suporte->Extern_id)))              { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Vmin"        , PARAMIN_FLT(suporte->Vmin_KV)))                { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Vmax"        , PARAMIN_FLT(suporte->Vmax_KV)))                { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_NumPontosFix", PARAMIN_INT(suporte->LisPtoFixacao()->Count))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Status"      , PARAMIN_INT(suporte->StatusAsUnsigned)))       { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_SuporteId"   , PARAMIN_INT(intSuporteId)))                    { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (bolInsert)
            {
            if (!bd_conn->SetParam("p_TipoSuporteId", PARAMIN_INT(suporte->TipoDeSuporte)))         { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            }
         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
         // Atualiza o identificador do cabo
         suporte->Id = intSuporteId;
         // Reinicia o indexador dos pontos de fixação
         //FNSNP_rdSuporteFixacaoItemProx(suporte, false, true);
         // Salva os pontos de fixação
         sucesso = SPSNP_wrSuporteFixacao(suporte);
         // Define status
         if (sucesso)
            {
            suporte->DefineStatus(sttNOVO,     false);
            suporte->DefineStatus(sttALTERADO, false);
            }
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
 */

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_wrSuporte(VTSuporte *suporte)
   {
   bool         sucesso = true;
   AnsiString   strSQL;
   AnsiString   strlog;
   bool         bolInsert;
   int          intSuporteId;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!suporte)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SUPORTEINV return(false); }

   // Escrita propriamente dita
   __try
      {
      // Verifica se o objeto é novo ou alterado
      if (suporte->StatusNovo())
         {
         // Trata-se de uma inserção
         bolInsert = true;

         intSuporteId = FNSNP_rdSuporteIdProx();
         strSQL = "EXEC SPSNP_wrSuporte_p01 :p_Descricao, :p_ExtId, :p_Vmin, :p_Vmax, :p_NumPontosFix, :p_Status, :p_SuporteId, :p_TipoSuporteId";
         }
      else if (suporte->StatusAlterado())
         {
         // Trata-se de uma atualização
         bolInsert = false;

         intSuporteId = suporte->Id;
         strSQL = "EXEC SPSNP_wrSuporte_p02 :p_Descricao, :p_ExtId, :p_Vmin, :p_Vmax, :p_NumPontosFix, :p_Status, :p_SuporteId";
         }
         else
            {
            // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
            return(true);
            }

      try
         {
         // Monta a string de execução
         // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_Descricao"   , PARAMIN_STR(suporte->Codigo)))                 { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_ExtId"       , PARAMIN_STR(suporte->Extern_id)))              { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Vmin"        , PARAMIN_FLT(suporte->Vmin_KV)))                { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Vmax"        , PARAMIN_FLT(suporte->Vmax_KV)))                { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_NumPontosFix", PARAMIN_INT(suporte->LisPtoFixacao()->Count))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Status"      , PARAMIN_INT(suporte->StatusAsUnsigned)))       { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_SuporteId"   , PARAMIN_INT(intSuporteId)))                    { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (bolInsert)
            {
            if (!bd_conn->SetParam("p_TipoSuporteId", PARAMIN_INT(suporte->TipoDeSuporte)))         { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            }
         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
         // Atualiza o identificador do cabo
         suporte->Id = intSuporteId;
         // Reinicia o indexador dos pontos de fixação
         //FNSNP_rdSuporteFixacaoItemProx(suporte, false, true);
         // Salva os pontos de fixação
         // mml@20140807 - Adicionado parâmetro bolInsert na chamada
         sucesso = SPSNP_wrSuporteFixacao(suporte, bolInsert);
         // Define status
         if (sucesso)
            {
            suporte->DefineStatus(sttNOVO,     false);
            suporte->DefineStatus(sttALTERADO, false);
            }
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

/*
//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_wrSuporteFixacao(VTSuporte* suporte)
   {
   bool          sucesso = true;
   AnsiString    strSQL;
   AnsiString    strlog;
   TList*        lisPtoFixacao;
   VTPtoFixacao* ptrPtoFixacao;
   // int           intSuporteIdProx;

   if (!bd_conn)                                           { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())                         { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!suporte)                                           { SNPSTRLOGLINE(strlog); MSGERRSYS_SUPORTEINV return(false); }
   if ((lisPtoFixacao = suporte->LisPtoFixacao()) == NULL) { SNPSTRLOGLINE(strlog); MSGERRSYS_SUPORTEINV return(false); }

   // Escrita propriamente dita
   __try
      {
      // Remove os itens existentes.
      if (!SPSNP_dlSuporteFixacao(suporte->Id)) { SNPSTRLOGLINE(strlog); MSGERRSYS_SUPORTEINV return(false); }

      // Executa a gravação de todos os itens
      try
         {
         strSQL = "EXEC SPSNP_wrSuporteFixacao_p01 :p_X, :p_Y, :p_Codigo, :p_SuporteId, :p_FixacaoItem";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);

         for (int i = 0; i < lisPtoFixacao->Count; i++)
            {
            // Loop de pontos (ini)
            if ((ptrPtoFixacao = (VTPtoFixacao*)lisPtoFixacao->Items[i]) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_PTOFIX
               return(false);
               }
            //
            //Está utilizando o próprio identificador do ponto de fixação
            //if ((intSuporteIdProx = FNSNP_rdSuporteFixacaoItemProx(suporte)) == IDINDEFINIDO)
            //   {
            //   SNPSTRLOGLINE(strlog);
            //   MSGERRSYS_PTOFIX
            //   return(false);
            //   }
            // Tem que montar aqui, pois a execução anterior "zera" a pilha de parâmetros (FNSNP_rdSuporteFixacaoItemProx)
            // Monta a string de execução
            //strSQL = "EXEC SPSNP_wrSuporteFixacao_p01 :p_X, :p_Y, :p_Codigo, :p_SuporteId, :p_FixacaoItem";
            // Decodifica comando
            //bd_conn->ParseCmd(strSQL);
            //

            // Determina os valores dos parâmetros
            if (!bd_conn->SetParam("p_X"           , PARAMIN_FLT(ptrPtoFixacao->coord.x))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_Y"           , PARAMIN_FLT(ptrPtoFixacao->coord.y))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_Codigo"      , PARAMIN_STR(ptrPtoFixacao->codigo)))  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_SuporteId"   , PARAMIN_INT(suporte->Id)))            { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_FixacaoItem" , PARAMIN_INT(ptrPtoFixacao->id)))      { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            // Comando da consulta
            if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
            // Loop de pontos (fim)
            }
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
*/

//---------------------------------------------------------------------------
// mml@20140807 - Adicionado parâmetro bolInsert na chamada
bool __fastcall TBDgeralAccess::SPSNP_wrSuporteFixacao(VTSuporte* suporte, bool bolInsert)
   {
   bool          sucesso = true;
   AnsiString    strSQL;
   AnsiString    strlog;
   TList*        lisPtoFixacao;
   VTPtoFixacao* ptrPtoFixacao;
   int           intSuporteIdProx;
   bool          bolNovoPonto;

   if (!bd_conn)                                           { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())                         { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!suporte)                                           { SNPSTRLOGLINE(strlog); MSGERRSYS_SUPORTEINV return(false); }
   if ((lisPtoFixacao = suporte->LisPtoFixacao()) == NULL) { SNPSTRLOGLINE(strlog); MSGERRSYS_SUPORTEINV return(false); }

   // Escrita propriamente dita
   __try
      {
      // mml@20140807 - Agora não remove mais. Ou insere, porque não existe, ou atualiza o que existe
      // Remove os itens existentes.
      // if (!SPSNP_dlSuporteFixacao(suporte->Id)) { SNPSTRLOGLINE(strlog); MSGERRSYS_SUPORTEINV return(false); }

      // Executa a gravação de todos os itens
      try
         {
         for (int i = 0; i < lisPtoFixacao->Count; i++)
            {
            // Loop de pontos (ini)
            if ((ptrPtoFixacao = (VTPtoFixacao*)lisPtoFixacao->Items[i]) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_PTOFIX
               return(false);
               }
            // mml@20140807 + jcg2014.08.08
            if (bolInsert)
               {//Suportte novo: define ID do PtoFixacao
               intSuporteIdProx  = i + 1;
               ptrPtoFixacao->id = intSuporteIdProx;
               }
            else
               {//Suporte existente: determina ID do PtoFixacao
               intSuporteIdProx = ptrPtoFixacao->id;
               }
            // mml@20140807 - Insere ou atualiza o suporte
            // Pode ser que uma atualização do suporte inclua um novo ponto. Logo, não dá pra confiar
            // somente na informação de inserção ou atualização do suporte.
            // Logo, essa atribuição tem que ser feita para cada ponto, e a verificação para todos os pontos
            // cujo id é menor que zero
            bolNovoPonto = bolInsert;
            // Pode ser que seja uma atualização do suporte, mas seja um ponto novo
            // pois um ponto pode ter sido removido antes dessa inserção
            if (intSuporteIdProx < 0)
               {
               // Sinaliza que tem novo ponto a ser inserido
               bolNovoPonto = true;

               int intSuporteIdMax = 0;
               for (int j = 0; j < lisPtoFixacao->Count; j++)
                  {
                  // Loop de pontos (ini)
                  if ((ptrPtoFixacao = (VTPtoFixacao*)lisPtoFixacao->Items[j]) == NULL)
                     {
                     SNPSTRLOGLINE(strlog);
                     MSGERRSYS_PTOFIX
                     return(false);
                     }
                  if (intSuporteIdMax < ptrPtoFixacao->id)
                     {
                     intSuporteIdMax = ptrPtoFixacao->id;
                     }
                  }
               intSuporteIdProx = intSuporteIdMax + 1;
               }

            // mml@20140807 - Insere ou atualiza o suporte
            // Agora isso é repetido pra todo ponto, pois pode ser:
            // 1) uma inserção do suporte e, consequentemente, uma inserção do ponto OU
            // 2) uma atualização do suporte e uma atualização do ponto OU
            // 3) uma atualização do suporte e uma inserção do ponto
            if (bolNovoPonto)
               {
               strSQL = "EXEC SPSNP_wrSuporteFixacao_p01 :p_X, :p_Y, :p_Codigo, :p_SuporteId, :p_FixacaoItem";
               }
            else
               {
               strSQL = "EXEC SPSNP_wrSuporteFixacao_p02 :p_X, :p_Y, :p_Codigo, :p_SuporteId, :p_FixacaoItem";
               }
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            /*
            Isso aqui era uma versão antiga.

            Está utilizando o próprio identificador do ponto de fixação
            if ((intSuporteIdProx = FNSNP_rdSuporteFixacaoItemProx(suporte)) == IDINDEFINIDO)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_PTOFIX
               return(false);
               }
            // Tem que montar aqui, pois a execução anterior "zera" a pilha de parâmetros (FNSNP_rdSuporteFixacaoItemProx)
            // Monta a string de execução
            strSQL = "EXEC SPSNP_wrSuporteFixacao_p01 :p_X, :p_Y, :p_Codigo, :p_SuporteId, :p_FixacaoItem";
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            */

            // Determina os valores dos parâmetros
            if (!bd_conn->SetParam("p_X"           , PARAMIN_FLT(ptrPtoFixacao->coord.x))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_Y"           , PARAMIN_FLT(ptrPtoFixacao->coord.y))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_Codigo"      , PARAMIN_STR(ptrPtoFixacao->codigo)))  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_SuporteId"   , PARAMIN_INT(suporte->Id)))            { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            // mml@20140807
            // if (!bd_conn->SetParam("p_FixacaoItem" , PARAMIN_INT(ptrPtoFixacao->id)))      { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_FixacaoItem" , PARAMIN_INT(intSuporteIdProx)))       { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            // Comando da consulta
            if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
            // Loop de pontos (fim)
            }
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
bool __fastcall TBDgeralAccess::SPSNP_wrMercado(VTMercado* mercado)
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

   // Como agora tem que salvar as áreas geográficas, a remoção / inserçaõ dos segmentos
   // foi quebrada em dois loops

   //obtém lista de Regiões
	if ((lisSegmento = mercado->LisSegmento()) == NULL) { SNPSTRLOGLINE(strlog); MSGERRSYS_MERCADOINV return(false); }
   // Inicia o loop
	for (int i = 0; i < lisSegmento->Count; i++)
      {
		segmento = (VTSegmento*)lisSegmento->Items[i];
		if (segmento == NULL)                            { SNPSTRLOGLINE(strlog); MSGERRSYS_SEGMENTOINV  return(false); }

		lisCrescimento = segmento->LisCrescimento();
		if (lisCrescimento == NULL)                      { SNPSTRLOGLINE(strlog); MSGERRSYS_SEGMENTOINV  return(false); }
                                                    
		if (!SPSNP_dlSegmentoMercado(segmento))          { SNPSTRLOGLINE(strlog); MSGERRBDDL_SEGMENTO   return(false); }
      }

   // Apaga as Áreas geográficas
   if (!SPSNP_dlAreaGeografica())                      { SNPSTRLOGLINE(strlog); MSGERRBDDL_REGIAO     return(false); }

   // Insere as Áreas Geográficas
   if ((regioes = (VTRegioes*)apl->GetObject(__classid(VTRegioes))) == NULL) { SNPSTRLOGLINE(strlog); MSGERRSYS_REGIAOINV return(false); }
   if ((lisRegiao = regioes->LisRegiao()) == NULL)                           { SNPSTRLOGLINE(strlog); MSGERRSYS_REGIAOINV return(false); }
   for (int i = 0; i < lisRegiao->Count; i++)
      {
      if ((regiao = (VTRegiao*)lisRegiao->Items[i]) == NULL)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_REGIAOINV return(false); }
      //define ID da Regiao
      regiao->Id = i+1;
      if (!SPSNP_wrAreaGeografica(regiao))                                   { SNPSTRLOGLINE(strlog); MSGERRSYS_REGIAOINV return(false); }
      }

	for (int i = 0; i < lisSegmento->Count; i++)
      {
		segmento = (VTSegmento*)lisSegmento->Items[i];
		if (segmento == NULL)                          { SNPSTRLOGLINE(strlog); MSGERRSYS_SEGMENTOINV  return(false); }
		lisCrescimento = segmento->LisCrescimento();
		if (lisCrescimento == NULL)                      { SNPSTRLOGLINE(strlog); MSGERRSYS_SEGMENTOINV  return(false); }

		//verifica se Segmento possui crescimento definido
		if (! segmento->Enabled) continue;

      // Como foi apagada a região, força a criação de uma região nova
		segmento->Id = IDINDEFINIDO;
		if (!SPSNP_wrSegmentoMercado(segmento))                 { SNPSTRLOGLINE(strlog); MSGERRBDDL_SEGMENTO    return(false); }

      for (int j = 0; j < lisCrescimento->Count; j++)
         {
         crescimento = (VTCrescimento*)lisCrescimento->Items[j];
         if (crescimento == NULL)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_CRESCIMENTOINV
            return(false);
            }

			if (!SPSNP_wrSegmentoCrescimento(segmento, crescimento))
            {
            SNPSTRLOGLINE(strlog);
				MSGERRBDWR_SEGMENTO_CRESCIMENTO
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
bool __fastcall TBDgeralAccess::SPSNP_wrAreaGeografica(VTRegiao* regiao)
   {
   bool            sucesso = true;
   AnsiString      strSQL;
   AnsiString      strlog;
   TStream*        ptrStreamPontos = NULL;
   TParameter*     ptrParam;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON    return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON    return(false); }
   if (!bd_func)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV     return(false); }
   if (!regiao)                      { SNPSTRLOGLINE(strlog); MSGERRSYS_ESTUDOINV     return(false); }

   //proteção para bases antigas sem tabela AreaGeografica
   if (! bd_conn->FindTable("AreaGeografica"))
      {
      Alerta("Base de dados desatualizada: não existe tabela regiões");
      return(true);
      }

   // Escrita propriamente dita
   __try
      {
      try
         {
         strSQL  = "EXEC SPSNP_wrAreaGeografica :p_Codigo, :p_Pontos, :p_AreaGeoId";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         
         if (!bd_conn->SetParam("p_Codigo"   , PARAMIN_STR(regiao->Codigo))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_AreaGeoId", PARAMIN_INT(regiao->Id)))     { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }

         if ((ptrStreamPontos = bd_func->MontaStreamPoligono(regiao->Poligono)) != NULL)
            {
            // A stream tem que existir durante a execução do comando
            // delete ptrStream;
            // ptrStream = NULL;
            }
         else
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_YREFINV
            return(false);
            }
         ptrParam = bd_conn->SetParam("p_Pontos", PARAMIN_BLB(NULL));
         ptrParam->LoadFromStream(ptrStreamPontos, ptrParam->DataType);

         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
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
      if (ptrStreamPontos) { delete ptrStreamPontos; ptrStreamPontos = NULL; }
      }

   return(sucesso);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_wrAreaGeografica(VTRegiao* regiao)
   {
   Alerta("TBDgeralAccess::SPSNP_wrAreaGeografica() - bloqueada");
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_wrSegmentoMercado(VTSegmento* segmento)
	{
   bool       sucesso = true;
   AnsiString strSQL;
   AnsiString strlog;

	if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON  return(false); }
	if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON  return(false); }
	if (!segmento)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_SEGMENTOINV return(false); }

   // Escrita propriamente dita
   __try
      {
      // A região sempre é apagada antes, então não está sendo considerado o Update
		segmento->Id = FNSNP_rdSegmentoIdProx();
		strSQL = "EXEC SPSNP_wrRegiao_p01 :p_TipoRegiaoId, :p_Codigo, :p_Utm1X, :p_Utm1Y, :p_Utm2X, :p_Utm2Y, :p_AreaGeoId, :p_RegiaoId";

      try
         {
         // Monta a string de execução
         // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_RegiaoId"    , PARAMIN_INT(segmento->Id)))                                                  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
			if (!bd_conn->SetParam("p_TipoRegiaoId", PARAMIN_INT(segmento->Tipo)))                                                { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
			if (!bd_conn->SetParam("p_Codigo"      , PARAMIN_STR(segmento->Codigo)))                                              { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Utm1X"       , PARAMIN_FLT(IDINDEFINIDO)))                                                  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Utm1Y"       , PARAMIN_FLT(IDINDEFINIDO)))                                                  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Utm2X"       , PARAMIN_FLT(IDINDEFINIDO)))                                                  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Utm2Y"       , PARAMIN_FLT(IDINDEFINIDO)))                                                  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
			if (!bd_conn->SetParam("p_AreaGeoId"   , PARAMIN_INT(segmento->Regiao != NULL? segmento->Regiao->Id : IDINDEFINIDO))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }

         // Reajusta demais parâmetros
			switch (segmento->Tipo)
            {
				case smINDEF    :
				case smDEFAULT  : if (!bd_conn->SetParam("p_Codigo", PARAMIN_STR("")))                     { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
										break;
				case smTIPOREDE :
				case smREDE     : // OOPS: Precisa fazer de novo? Está diferente do Oracle
										//if (!bd_conn->SetParam("p_Codigo", PARAMIN_STR(regiao->Codigo)))         { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
										break;

				case smAREA     : if (!bd_conn->SetParam("p_Utm1X", PARAMIN_FLT((int)segmento->Area->x1)))   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
										if (!bd_conn->SetParam("p_Utm1Y", PARAMIN_FLT((int)segmento->Area->y1)))   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
										if (!bd_conn->SetParam("p_Utm2X", PARAMIN_FLT((int)segmento->Area->x2)))   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
										if (!bd_conn->SetParam("p_Utm2Y", PARAMIN_FLT((int)segmento->Area->y2)))   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
										break;

				case smCARGA    : // OOPS: Precisa fazer de novo? Está diferente do Oracle
										//if (!bd_conn->SetParam("p_Codigo", PARAMIN_STR(regiao->Codigo)))         { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
										break;
            }


         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
         // OOPS: No ORacle não estou fazendo isso
         // Altera estado
			segmento->Nova     = false;
         segmento->Alterada = false;
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
bool __fastcall TBDgeralAccess::SPSNP_wrSegmentoCrescimento(VTSegmento* segmento, VTCrescimento* crescimento)
	{
   bool           sucesso = true;
   AnsiString     strSQL;
   AnsiString     strlog;
   int            intConjCurvaId;
   int            intCurvaId;
   int            intSegmentoId;
   TMemoryStream* ptrStream = NULL;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON     return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON     return(false); }
   if (!bd_func)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV      return(false); }
	if (!segmento)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_SEGMENTOINV    return(false); }
   if (!crescimento)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_CRESCIMENTOINV return(false); }
   if (!crescimento->Classe)         { SNPSTRLOGLINE(strlog); MSGERRSYS_CRESCIMENTOINV return(false); }
   if (!bd_curva)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDCURVAINV     return(false); }

   // Escrita propriamente dita
   __try
      {
      // Cria uma curva para o crescimento
      ptrStream = new TMemoryStream();
      // Monta a curva em memória
      if (!bd_func->MontaStreamCurvaCrescimento(ptrStream, crescimento))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRSYS_CURVASTREAM2
         return(false);
         }
      // Salva a curva em duas etapas. Etapa 01/02
      intConjCurvaId = intCurvaId = intSegmentoId = IDINDEFINIDO;
      if (!bd_curva->SPSNP_wrConjCurva(intConjCurvaId, crescimento->Codigo, false /*Tipica*/, 0 /* status*/))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDWR_CURCRESCIMENTO
         return(false);
         }
      // Salva a curva em duas etapas. Etapa 02/02
      // !OOPS! Fazer macro aqui?
      if (!bd_curva->SPSNP_wrConjCurvaSegmento(intConjCurvaId, 8 /* Crescimento*/, intCurvaId, intSegmentoId, ptrStream))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDWR_CURCRESCIMENTO
         return(false);
         }

      // A região sempre é apagada antes, então não está sendo considerado o Update
      // mml@02130304 Antes de chamar essa rotina, é chamada a SPSNP_wrAreaGeografica, que já acerta o identificador da região
      // regiao->Id = FNSNP_rdRegiaoIdProx();
      strSQL = "EXEC SPSNP_wrRegiaoCrescimento_p01 :p_RegiaoId, :p_TipoConsId, :p_ConjCurvaId";

      try
         {
         // Monta a string de execução
         // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_RegiaoId"   , PARAMIN_INT(segmento->Id)))              { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_TipoConsId" , PARAMIN_INT(crescimento->Classe->Id))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_ConjCurvaId", PARAMIN_INT(intConjCurvaId)))          { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }

         // Comando da consulta
         if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
         // OOPS: No ORacle não estou fazendo isso
         // Altera estado
			segmento->Nova     = false;
			segmento->Alterada = false;
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
      if (ptrStream) { delete ptrStream; ptrStream = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDgeralAccess::SPSNP_wrTipoChave(TList* lstTipoChave)
   {
   bool         sucesso = true;
   AnsiString   strSQL;
   AnsiString   strlog;
   VTTipoChave* tipo_chave;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstTipoChave)                { SNPSTRLOGLINE(strlog); MSGERRSYS_LISOBJINV  return(false); }

   // Escrita propriamente dita
   __try
      {
      try{
         for (int i = 0; i < lstTipoChave->Count; i++)
            {
            if ((tipo_chave = (VTTipoChave*)lstTipoChave->Items[i]) == NULL) { SNPSTRLOGLINE(strlog); MSGERRSYS_LISOBJINV  return(false); }
            // Monta a string de execução
            //if (tipo_chave->Id < 0)
            if ((tipo_chave->Status[sttNOVO]) ||(tipo_chave->Id < 0))
               {// Tipo novo de chave (insere)
               tipo_chave->Id = FNSNP_rdTipoChaveIdProx();
               strSQL = "EXEC SPSNP_wrTipoChave_p01 :p_Descricao, :p_Status, :p_CorId, :p_TipoChaveId ";
               }
            else if (tipo_chave->Status[sttALTERADO])
               {// Tipo já exstente de chave (atualiza)
               strSQL = "EXEC SPSNP_wrTipoChave_p02 :p_Descricao, :p_Status, :p_CorId, :p_TipoChaveId ";
               }
            else
               {continue;}
            //atualiza status antes de gravar
            tipo_chave->Status[sttNOVO]     = false;
            tipo_chave->Status[sttALTERADO] = false;

            // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            if (!bd_conn->SetParam("p_Descricao"  , PARAMIN_STR(tipo_chave->Codigo)))          { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_Status"     , PARAMIN_INT(tipo_chave->StatusAsUnsigned))){ SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_TipoChaveId", PARAMIN_INT(tipo_chave->Id)))              { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_CorId"      , PARAMIN_INT(tipo_chave->Color)))           { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }

            // Comando da consulta
            if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
            }
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
bool __fastcall TBDgeralAccess::SPSNP_wrTipoConsumidor(void)
   {
   bool         sucesso = true;
   AnsiString   strSQL;
   AnsiString   strlog;
   VTClasse*    classe;
   TList*       lisCLASSE;

   if (!bd_conn)                                                   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())                                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!classes)                                                   { SNPSTRLOGLINE(strlog); MSGERRSYS_CLASSESINV return(false); }

   // Escrita propriamente dita
   __try
      {
      try
         {
         lisCLASSE = classes->LisClasse();
         if (!lisCLASSE)                                           { SNPSTRLOGLINE(strlog); MSGERRSYS_CLASSESINV return(false); }
         for (int i = 0; i < lisCLASSE->Count; i++)
            {
            if ((classe = (VTClasse*)lisCLASSE->Items[i]) == NULL) { SNPSTRLOGLINE(strlog); MSGERRSYS_CLASSESINV return(false); }

            if (classe->Id < 0)
               {
               // Tipo novo de chave (insere)
               classe->Id = FNSNP_rdTipoConsumidorIdProx();
               strSQL = "EXEC SPSNP_wrTipoConsumidor_p01 :p_Tag, :p_Descricao, :p_Tipico, :p_CorId, :p_TipoConsId ";
               }
            else
               {
               // Tipo já exstente de chave (atualiza)
               strSQL = "EXEC SPSNP_wrTipoConsumidor_p02 :p_Tag, :p_Descricao, :p_Tipico, :p_CorId, :p_TipoConsId ";
               }

            // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            if (!bd_conn->SetParam("p_Tag"       , PARAMIN_INT(classe->Tag)))      { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_Descricao" , PARAMIN_STR(classe->Codigo)))   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_Tipico"    , PARAMIN_INT(classe->Tipico)))   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_CorId"     , PARAMIN_INT(0)))                { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_TipoConsId", PARAMIN_INT(classe->Id)))       { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }

            // Comando da consulta
            if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
            }
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
bool __fastcall TBDgeralAccess::SPSNP_wrTipoRede(TList* lstTipoRede)
   {
   bool         sucesso = true;
   AnsiString   strSQL;
   AnsiString   strlog;
   VTTipoRede*  tipo_rede;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstTipoRede )                { SNPSTRLOGLINE(strlog); MSGERRSYS_LISOBJINV  return(false); }

   // Escrita propriamente dita
   __try
      {
      try
         {
         for (int i = 0; i < lstTipoRede->Count; i++)
            {
            if ((tipo_rede = (VTTipoRede*)lstTipoRede->Items[i]) == NULL) { SNPSTRLOGLINE(strlog); MSGERRSYS_LISOBJINV  return(false); }
            // Monta a string de execução
            if ((tipo_rede->Status[sttNOVO]) ||(tipo_rede->Id < 0))
               {// Tipo novo de rede (insere)
               tipo_rede->Id       = FNSNP_rdTipoRedeIdProx();
               tipo_rede->Segmento = tipo_rede->Id;
               strSQL = "EXEC SPSNP_wrTipoRede_p01 :p_Codigo, :p_Descricao, :p_Vmin, :p_Vmax, :p_ColorId, :p_Status, :p_TipoRedeId";
               }
            else if ((tipo_rede->Status[sttALTERADO]))
               {// Tipo já exstente de chave (atualiza)
               strSQL = "EXEC SPSNP_wrTipoRede_p02 :p_Codigo, :p_Descricao, :p_Vmin, :p_Vmax, :p_ColorId, :p_Status, :p_TipoRedeId";
               }
            else
               {continue;}
            //atualiza status antes de gravar
            tipo_rede->Status[sttNOVO]     = false;
            tipo_rede->Status[sttALTERADO] = false;
            // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            if (!bd_conn->SetParam("p_Codigo"    , PARAMIN_STR(tipo_rede->Codigo)))          { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_Descricao" , PARAMIN_STR(tipo_rede->Descricao)))       { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_VMin"      , PARAMIN_FLT(tipo_rede->Vmin_kv)))         { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_VMax"      , PARAMIN_FLT(tipo_rede->Vmax_kv)))         { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_ColorId"   , PARAMIN_INT(tipo_rede->Color)))           { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_Status"    , PARAMIN_INT(tipo_rede->StatusAsUnsigned))){ SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
            if (!bd_conn->SetParam("p_TipoRedeId", PARAMIN_INT(tipo_rede->Id)))              { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }

            // Comando da consulta
            if (!bd_conn->Execute(false)) { SNPSTRLOGLINE(strlog); return(false); }
            }
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
//eof


