//---------------------------------------------------------------------------


#include <vcl.h>
#pragma hdrstop

#include "SNPGlobal.h"
#include "TBDcurvaAccess.h"
#include "TBDfunc.h"
#include "VTSNPDBConn.h"
#include "..\Apl\VTApl.h"
#include "..\Log\VTLog.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Rede\VTBateria.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTDominio.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


//---------------------------------------------------------------------------
__fastcall TBDcurvaAccess::TBDcurvaAccess(VTApl *apl_owner, VTSNPDBConn *bd_conn)
   {
   AnsiString strlog;

   //salva ponteiro
   this->apl     = apl_owner;
   this->bd_conn = bd_conn;
   //inicia ponteiros de uso geral, buscando no Apl
   plog = (VTLog*)apl->GetObject(__classid(VTLog));
   // Carrega ponteiros previamente criados
   if ((bd_func    = (TBDfunc*)apl->GetObject(__classid(TBDfunc)))         == NULL)   { MSGERRSYS_BDFUNCINV    SNPSTRLOGLINE(strlog); }

   // Inicia indexadores
   index.ndxConjCurvaId = 0;
   //index.ndxCurvaId     = 0;
   index.ndxSegmentoId  = 0;
   }

//---------------------------------------------------------------------------
__fastcall TBDcurvaAccess::~TBDcurvaAccess(void)
   {
   //destrói objetos
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/13952424
VTCurva* __fastcall TBDcurvaAccess::CurvaEqpto(VTEqpto* eqpto)
   {
   VTCurva* curva = NULL;

   if (!eqpto) return(NULL);

   switch (eqpto->Tipo())
	  {
	  case eqptoBATERIA    : curva = ((VTBateria*)eqpto)->curvaCargaDescarga;    break;
      case eqptoCAPACITOR  : curva = ((VTCapacitor*)eqpto)->curva;  break;
      case eqptoCARGA      : curva = ((VTCarga*)eqpto)->curva;      break;
      case eqptoGERADOR    : curva = ((VTGerador*)eqpto)->curva;    break;
      case eqptoREATOR     : curva = ((VTReator*)eqpto)->curva;     break; // mml@20130816
      case eqptoSUPRIMENTO : curva = ((VTSuprimento*)eqpto)->curva; break;
      case eqptoCANAL      : curva = ((VTCanal*)eqpto)->Curva;      break;
      default              : curva = NULL;                          break;
      }

   return(curva);
   }

//---------------------------------------------------------------------------
int __fastcall TBDcurvaAccess::FNSNP_rdConjCurvaIdProx(bool bolLocaIndex, bool bolBeginNext)
   {
   TADOQuery* ptrQryDS = NULL;
   AnsiString strSQL;
   AnsiString strlog;

   if (bolLocaIndex)
      {
      // índice local (ini)
      index.ndxConjCurvaId++;
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
            strSQL = "EXEC FNSNP_rdConjCurvaIdProx";
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            // Comando da consulta
            if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            // Recupera o resultado
            if (ptrQryDS->RecordCount != 1)                 { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            index.ndxConjCurvaId = ptrQryDS->FieldByName("IdProx")->AsInteger;
            // Começa a contagem somente a partir da próxima chamada
            if (bolBeginNext) { index.ndxConjCurvaId--; }
            ptrQryDS->Close();
            }
         catch (Exception& e)
            {
            SNPSTRLOGLINE(strlog);
            SNPSTRLOG(e.Message);
            index.ndxConjCurvaId = IDINDEFINIDO;
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

   return(index.ndxConjCurvaId);
   }
/*
//---------------------------------------------------------------------------
int __fastcall TBDcurvaAccess::FNSNP_rdConjCurva_CurvaIdProx(int intConjCurvaId, bool bolLocaIndex, bool bolBeginNext)
   {
   TADOQuery* ptrQryDS = NULL;
   AnsiString strSQL;
   AnsiString strlog;

   if (bolLocaIndex)
      {
      // índice local (ini)
      index.ndxCurvaId++;
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
            strSQL = "EXEC FNSNP_rdConjCurva_CurvaIdProx :p_ConjCurvaId";
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            if (!bd_conn->SetParam("p_ConjCurvaId", PARAMIN_INT(intConjCurvaId)))
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
            index.ndxCurvaId = ptrQryDS->FieldByName("IdProx")->AsInteger;
            // Começa a contagem somente a partir da próxima chamada
            if (bolBeginNext) { index.ndxCurvaId--; }
            ptrQryDS->Close();
            }
         catch (Exception& e)
            {
            SNPSTRLOGLINE(strlog);
            SNPSTRLOG(e.Message);
            index.ndxCurvaId = IDINDEFINIDO;
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

   return(index.ndxCurvaId);
   }
*/

//---------------------------------------------------------------------------
int __fastcall TBDcurvaAccess::FNSNP_rdConjCurva_SegIdProx(int intConjCurvaId, int intCurvaId, bool bolLocaIndex, bool bolBeginNext)
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
            strSQL = "EXEC FNSNP_rdConjCurva_SegIdProx :p_ConjCurvaId :p_CurvaId";
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            if (!bd_conn->SetParam("p_ConjCurvaId", PARAMIN_INT(intConjCurvaId)))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBD_PARAMINV
               return(NULL);
               }
            if (!bd_conn->SetParam("p_CurvaId", PARAMIN_INT(intCurvaId)))
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
bool __fastcall TBDcurvaAccess::SPSNP_wrConjCurvaEquipamento(VTEstudo* estudo, VTEqpto* eqpto, VTCurva* curva)
   {
   bool       sucesso = true;
   AnsiString strSQL;
   AnsiString strlog;
   VTDominio* dominio;

   if (!bd_conn)                                      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON    return(false); }
   if (!bd_conn->getConnectedOK())                    { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON    return(false); }
   if (!estudo)                                       { SNPSTRLOGLINE(strlog); MSGERRSYS_ESTUDOINV     return(false); }
   if (!eqpto)                                        { SNPSTRLOGLINE(strlog); MSGERRSYS_EQUIPINV      return(false); }
   if (!curva)                                        { SNPSTRLOGLINE(strlog); MSGERRSYS_CURVAINV      return(false); }

   // Se a curva não for típica, salva/atualiza a curva
   if (!curva->Tipica)
      {
      // Inicialmente, escreve a curva, cuja função já existe na base de dados geral
      // se a curva não for típica e a descrição estiver vazia, associa o código do equipamento à curva
      if ((!curva->Tipica) && (curva->Codigo.IsEmpty())) { curva->Codigo = eqpto->Codigo;                                }
      if (!SPSNP_wrConjCurva(eqpto->Tipo(), curva))      { SNPSTRLOGLINE(strlog); MSGERRSYS_CURVAINV      return(false); }
      }

   // Em seguida, completa a função, associando o equipamento à curva
   __try
      {
      try
         {
         strSQL = "EXEC SPSNP_wrConjCurvaEquipamento_p01 :p_ConjCurvaId, :p_EstudoId, :p_EquipId";

         // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);

         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_ConjCurvaId", PARAMIN_INT(curva->Id)))   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_EstudoId"   , PARAMIN_INT(estudo->Id)))  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_EquipId"    , PARAMIN_INT(eqpto->Id)))   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }

         // Comando da consulta
         if (!bd_conn->Execute(false))                                      { SNPSTRLOGLINE(strlog);                   return(false); }
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
bool __fastcall TBDcurvaAccess::SPSNP_wrConjCurva(int& intConjCurvaId, AnsiString strDescricao, bool bolTipica, int intStatus)
   {
   bool       sucesso = true;
   AnsiString strSQL;
   AnsiString strlog;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }

   // Escrita propriamente dita
   __try
      {
      // Verifica se o objeto é indefinido
      if (intConjCurvaId == IDINDEFINIDO)
         {
         // Trata-se de uma inserção
         intConjCurvaId = FNSNP_rdConjCurvaIdProx();
         strSQL = "EXEC SPSNP_wrConjCurva_p01 :p_Descricao, :p_Tipica, :p_Status, :p_ConjCurvaId";
         }
      else
         {
         // Trata-se de uma atualização
         strSQL = "EXEC SPSNP_wrConjCurva_p02 :p_Descricao, :p_Tipica, :p_Status, :p_ConjCurvaId";
         }

      try
         {
         // Monta a string de execução
         // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_Descricao"  , PARAMIN_STR(strDescricao)))             { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Tipica"     , PARAMIN_INT((int)(bolTipica ? 1 : 0)))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Status"     , PARAMIN_INT(intStatus)))                { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_ConjCurvaId", PARAMIN_INT(intConjCurvaId)))           { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
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
bool __fastcall TBDcurvaAccess::SPSNP_wrConjCurva(int intTipoEquipId, VTCurva *curva)
   {
   bool       sucesso = true;
   AnsiString strSQL;
   AnsiString strlog;
   int        intConjCurvaId;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!curva)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_CURVAINV   return(false); }

   // Escrita propriamente dita
   __try
      {
      // Trata-se de uma inserção
      if (curva->Status[sttNOVO])
         {
         intConjCurvaId = IDINDEFINIDO;
         }
      else
         {
         intConjCurvaId = curva->Id;
         }

      try
         {
         // Executa a escrita da curva
//JCG 2014.01.15 - código temporário até alteração da base
         //if ((sucesso = SPSNP_wrConjCurva(intConjCurvaId, curva->Codigo, curva->Tipica, curva->StatusAsUnsigned)) == TRUE)
         if ((sucesso = SPSNP_wrConjCurva(intConjCurvaId, curva->CodigoBD, curva->Tipica, curva->StatusAsUnsigned)) == TRUE)
            {
            // Atualiza o identificador da curva
            curva->Id = intConjCurvaId;
            // Caso o identificador do tipo de equipamento seja válido, prossegue com o resto
            if (intTipoEquipId != IDINDEFINIDO)
               {
               // Curvas de equipamentos possuem somente uma curva no conjunto e um segmento por curva
               sucesso = SPSNP_wrConjCurvaSegmento(1, 1, intTipoEquipId, curva);
               // Associa a curva ao equipamento
               // Não associa curva ao equipamento, uma vez que essa é a base geral
               // A associação deve ser feita quando for ser salvo um equipamento (na base de dados de rede)
               }

            if (sucesso)
               {
               // Define status
               curva->Status[sttNOVO]     = false;
               curva->Status[sttALTERADO] = false;
               }
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
bool __fastcall TBDcurvaAccess::SPSNP_wrConjCurvaSegmento(int intCurvaId, int intSegmentoId, int intTipoEquipId, VTCurva *curva)
   {
   bool        sucesso = true;
   AnsiString  strSQL;
   AnsiString  strlog;
   TStream*    ptrStream = NULL;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!curva)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_CURVAINV   return(false); }

   // Escrita propriamente dita
   __try
      {
      /*
      OOPS: Pepino: ao modificar o valor de um patamar de uma carga (curva própria) o status ocntinua em
      0, não entrando na rotina
      if ((!curva->Status[sttNOVO]) && (!curva->Status[sttALTERADO]))
         {
         return(true);
         }
      */

      // Acerta os valores dos blobs
      if ((ptrStream = bd_func->MontaStreamCurvaEquipamento(intTipoEquipId, curva)) == NULL)
         {
         SNPSTRLOGLINE(strlog);
         MSGERRSYS_STREAM
         return(false);
         }

      // Verifica se o objeto é novo ou alterado
      if (curva->Status[sttNOVO])
         {
         // Trata-se de uma inserção
         sucesso = SPSNP_wrConjCurvaSegmento(curva->Id, curva->Tipo, IDINDEFINIDO, IDINDEFINIDO, ptrStream);
         }
      else
         {
         // Trata-se de uma atualização
         sucesso = SPSNP_wrConjCurvaSegmento(curva->Id, curva->Tipo, intCurvaId, intSegmentoId, ptrStream);
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
bool __fastcall TBDcurvaAccess::SPSNP_wrConjCurvaSegmento(int intConjCurvaId, int intTipoCurvaId, int intCurvaId, int intSegmentoId, TStream* ptrStream)
   {
   bool        sucesso = true;
   AnsiString  strSQL;
   AnsiString  strlog;
   TParameter* ptrParam;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }

   // Escrita propriamente dita
   __try
      {
      // Verifica se o objeto é novo ou alterado
      if (intCurvaId == IDINDEFINIDO)
         {
         // Trata-se de uma inserção
         intCurvaId = 1;
         if (intSegmentoId == IDINDEFINIDO) { intSegmentoId = 1; };
         
         strSQL = "EXEC SPSNP_wrConjCurvaSegmento_p01 :p_TipoCurvaId, :p_Pontos, :p_ConjCurvaId, :p_CurvaId, :p_SegmentoId";
         }
      else
         {
         // Trata-se de uma atualização
         strSQL = "EXEC SPSNP_wrConjCurvaSegmento_p02 :p_TipoCurvaId, :p_Pontos, :p_ConjCurvaId, :p_CurvaId, :p_SegmentoId";
         }

      try
         {
         // Monta a string de execução
         // Montada anteriormente, dependendo do tipo de execução (inserção ou atualização)
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_TipoCurvaId", PARAMIN_INT(intTipoCurvaId)))  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_ConjCurvaId", PARAMIN_INT(intConjCurvaId)))  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_CurvaId"    , PARAMIN_INT(intCurvaId)))      { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_SegmentoId" , PARAMIN_INT(intSegmentoId)))   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         // Acerta os valores dos blobs
         ptrParam = bd_conn->SetParam("p_Pontos", PARAMIN_BLB(NULL));
         ptrParam->LoadFromStream(ptrStream, ptrParam->DataType);

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
// Para essa função não existe uma consulta específica. Ela reproduz uma
// procedure do Oracle, que chama outras procedures
bool __fastcall TBDcurvaAccess::SPSNP_wrConjCurvaTipica(int intTipoEquipId, VTCurva *curva)
   {
   /*
   int intCurvaIdAux;
   int intSegmentoIdAux;

   // Reproduz o teste existente no Oracle, evitando que as procedures abaixo sejam sempre
   // chamadas
   if (curva->Id < 0)
      {
      intCurvaIdAux    = FNSNP_rdConjCurva_CurvaIdProx(curva->Id);
      intSegmentoIdAux = FNSNP_rdConjCurva_SegIdProx(curva->Id, intCurvaIdAux);
      }
   else
      {
      intCurvaIdAux    = 1; // Curvas de potóncia possuem somente uma curva no conjunto
      intSegmentoIdAux = 1; // Curvas de potóncia possuem somente um segmento por curva
      }
   */

   return(SPSNP_wrConjCurva(intTipoEquipId, curva));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDcurvaAccess::RenovaStatusCurva(TList *lisCurva)
   {
   bool     sucesso = true;
   VTCurva* curva;

   // Reinicia os identificadores / status NOVO do equipamentos
   for (int n = 0; n < lisCurva->Count; n++)
      {
      curva = (VTCurva*)lisCurva->Items[n];
      if ((curva->Status[sttNOVO]) || (curva->Id > index.ndxConjCurvaId))
         {
         curva->Id              = IDINDEFINIDO;
         curva->Status[sttNOVO] = true;
         }
      }
   return(sucesso);
   }


