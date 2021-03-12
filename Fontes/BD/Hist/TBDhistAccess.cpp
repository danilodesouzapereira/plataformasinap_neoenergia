//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include <Math.hpp>
#include "TBDhistAccess.h"
#include "TBDfunc.h"
#include "..\Apl\VTApl.h"
#include "..\Arsesp\VTArsesp.h"
#include "..\BD\VTSNPDBConn.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurva.h"
#include "..\Diretorio\VTPath.h"
#include "..\Historico\Densidade\VTResumoCarga.h"
#include "..\Log\VTLog.h"
#include "..\Mercado\VTMercado.h"
#include "..\Patamar\VTPatamares.h"
#include "..\PerdaTec\VTBalanco.h"
#include "..\PerdaTec\VTBalancos.h"
#include "..\PerdaTec\VTPerda.h"
#include "..\PerdaTec\VTPerdas.h"
#include "..\PerdaTec\VTResumo.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Radial\VTArvore.h"
#include "..\Radial\VTNode.h"
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
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Radial.h"
#include "..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
VTBDhist* NewObjBDhistAccess(VTApl *apl_owner, VTSNPDBConn *bd_conn)
   {
   return(new TBDhistAccess(apl_owner, bd_conn));
   }

//---------------------------------------------------------------------------
__fastcall TBDhistAccess::TBDhistAccess(VTApl *apl_owner, VTSNPDBConn *bd_conn)
   {
   AnsiString strlog;

   //salva ponteiros
   this->apl     = apl_owner;
   this->bd_conn = bd_conn;

   if (!this->bd_conn)
      {
      SNPSTRLOGLINE(strlog);
      }
   else
     {
     if (!this->bd_conn->getConnectedOK(true))
        {
        SNPSTRLOGLINE(strlog);
        }
     }
   // Variáveis externas
   if ((plog = (VTLog*)apl->GetObject(__classid(VTLog)))   == NULL)  {                     SNPSTRLOGLINE(strlog); }
   if ((path = (VTPath*)apl->GetObject(__classid(VTPath))) == NULL)  { MSGERRSYS_PATHINV   SNPSTRLOGLINE(strlog); }

   //define o estado inicial dos indexadores
   index.ndxArvoreId = 0;
   index.ndxNoId     = 0;
   index.ndxRedeId   = 0;
   // Reinicia indexadores
   FNSNP_rdArvoreIdProx(false, true);
   FNSNP_rdNoIdProx(false, true);
   FNSNP_rdRedeIdProx(false, true);
   // Lista de dados a serem salvos em modo batch
   lstStrData = new TStringList();
   // A configuração dessa lista não evita registros duplicados
   // Tive que forçar assim, pois os registros ordenados podem conflitar com a ordem
   // dos nós da árvore (o default é ordenar como string. Assim, 19, vem antes de 2
   lstStrData->Sorted = false;
   // OOPS: Seria bom colocar isso aqui no BD, pois é utilizado no Histórico e no Rede
   // Efetua a cópia do arquivo Schema.ini
   // CopiaSchemaIni();
   }

//---------------------------------------------------------------------------
__fastcall TBDhistAccess::~TBDhistAccess(void)
   {
   //destrói objetos criados localmente
   if (lstStrData) { lstStrData->Clear(); delete lstStrData; lstStrData = NULL; }
	}

//---------------------------------------------------------------------------
bool __fastcall TBDhistAccess::ArvoreContemNo(VTArvore* arvore, AnsiString codigo)
	{
	//variáveis locais
	bool       sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;

	if (!bd_conn)                   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
	if (!bd_conn->getConnectedOK()) { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }

	__try
		{
		try
			{// Monta a string de execução
			strSQL = "EXEC SPSNP_rdExisteArvoreNo :p_ArvoreId :p_Codigo ";
			// define os valores dos parâmetros
			if (!bd_conn->SetParam("p_ArvoreId", PARAMIN_INT(arvore->Id)))  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }
			if (!bd_conn->SetParam("p_Codigo",  PARAMIN_STR(codigo)))       { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			//executa o comando da consulta
			if (! bd_conn->Execute(true))                   { SNPSTRLOGLINE(strlog); return(false); }
			if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); return(false); }
			//verifica se foi retornado um registro
			sucesso =  (ptrQryDS->RecordCount > 0);
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
		{//destrói objetos locais
		if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
		}
	return(sucesso);
	}


//---------------------------------------------------------------------------
VTArvore* __fastcall TBDhistAccess::ExisteArvoreNaBase(int tipo_arvore, TDateTime data)
   {
   //variáveis locais
   VTArvore *arvore_bd;
   TList    *lisARV;

   try{//cria lista temporária
      lisARV = new TList();
      //lê lista de Arvores da Base
      LeListaArvore(lisARV);
      //verifica se existe uma Arvore com mesma data
      for (int n = 0; n < lisARV->Count; n++)
         {
         arvore_bd = (VTArvore*)lisARV->Items[n];
         //verifica tipo e data da árvore
         if ((arvore_bd->TipoArvore == tipo_arvore)&&(DateOf(arvore_bd->Data) == DateOf(data)))
            {//retira Arvore da lista
            lisARV->Delete(n);
            break;
            }
         else
            {//reinicia ponteiro p/ Arvore
            arvore_bd = NULL;
            }
         }
      //destrói lista e seus objetos
      LimpaTList(lisARV);
      delete lisARV;
      }catch(Exception &e)
         {//nada a fazer
         }
   return(arvore_bd);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TBDhistAccess::ExisteBarra(VTRede *rede, int utm_x, int utm_y)
   {
   //variáveis locais
   VTBarra     *barra;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //determina Barra
   if ((barra = redes->ExisteBarraCoordUtm(utm_x, utm_y)) == NULL)
      {//cria uma nova Barra
      barra = DLL_NewObjBarra(patamares->NumPatamar());
      barra->Id    = FalsoId();
      barra->utm.x = utm_x;
      barra->utm.y = utm_y;
      //IMPORTANTE: define Barra como não isolada
      barra->Status[sttISOLADO] = false;
      barra->Show[eqp_VISIBLE]  = false;
      //insere Barra em Redes
      redes->InsereBarra(barra);
      //insere Barra diretamente na lista de Barras da Rede
      (rede->LisBarra())->Add(barra);
      }
   return(barra);
   }
 
//---------------------------------------------------------------------------
VTRede* __fastcall TBDhistAccess::ExisteRede(int rede_id, int tiporede_id)
   {
   //variáveis locais
   VTRede     *rede;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   VTRedes     *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTTipos     *tipos     = (VTTipos*)apl->GetObject(__classid(VTTipos));

   //verifica se a Rede já existe
   if ((rede = redes->ExisteRede(rede_id)) == NULL)
      {//cria Rede
      rede = DLL_NewObjRede(patamares->NumPatamar());
      rede->Id        = rede_id;
      rede->Carregada = true;
      //define tipo da Rede
      rede->TipoRede = tipos->ExisteTipoRede(tiporede_id);
      //insere Rede em Redes
      redes->InsereRede(rede);
      }
   return(rede);
   }

//---------------------------------------------------------------------------
int __fastcall TBDhistAccess::FNSNP_rdArvoreIdProx(bool bolLocaIndex, bool bolBeginNext)
   {
   TADOQuery*    ptrQryDS = NULL;
   AnsiString    strSQL;
   AnsiString    strlog;

   if (bolLocaIndex)
      {
      // índice local (ini)
      index.ndxArvoreId++;
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
            strSQL = "EXEC FNSNP_rdArvoreIdProx";
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            // Comando da consulta
            if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            // Recupera o resultado
            if (ptrQryDS->RecordCount != 1)                 { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            index.ndxArvoreId = ptrQryDS->FieldByName("IdProx")->AsInteger;
            // Começa a contagem somente a partir da próxima chamada
            if (bolBeginNext) { index.ndxArvoreId--; }
            ptrQryDS->Close();
            }
         catch (Exception& e)
            {
            SNPSTRLOGLINE(strlog);
            SNPSTRLOG(e.Message);
            index.ndxArvoreId = IDINDEFINIDO;
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

   return(index.ndxArvoreId);
   }

//---------------------------------------------------------------------------
int __fastcall TBDhistAccess::FNSNP_rdNoIdProx(bool bolLocaIndex, bool bolBeginNext)
   {
   TADOQuery*    ptrQryDS = NULL;
   AnsiString    strSQL;
   AnsiString    strlog;

   if (bolLocaIndex)
      {
      // índice local (ini)
      index.ndxNoId++;
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
            strSQL = "EXEC FNSNP_rdNoIdProx";
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            // Comando da consulta
            if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            // Recupera o resultado
            if (ptrQryDS->RecordCount != 1)                 { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            index.ndxNoId = ptrQryDS->FieldByName("IdProx")->AsInteger;
            // Começa a contagem somente a partir da próxima chamada
            if (bolBeginNext) { index.ndxNoId--; }
            ptrQryDS->Close();
            }
         catch (Exception& e)
            {
            SNPSTRLOGLINE(strlog);
            SNPSTRLOG(e.Message);
            index.ndxNoId = IDINDEFINIDO;
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

   return(index.ndxNoId);
   }

// **************************************************************************
// mml@20130130 - Novas rotinas (abaixo)
// Antigas devem ser removidas posteriormente. Interface deve ser modificada.
// **************************************************************************

//---------------------------------------------------------------------------
int  __fastcall TBDhistAccess::FNSNP_rdRedeIdProx(bool bolLocaIndex, bool bolBeginNext)
   {
   TADOQuery*    ptrQryDS = NULL;
   AnsiString    strSQL;
   AnsiString    strlog;

   if (bolLocaIndex)
      {
      // índice local (ini)
      index.ndxRedeId++;
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
            strSQL = "EXEC FNSNP_rdRedeIdProx";
            // Decodifica comando
            bd_conn->ParseCmd(strSQL);
            // Determina os valores dos parâmetros
            // Comando da consulta
            if (!bd_conn->Execute(true))                    { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            if ((ptrQryDS = bd_conn->getDataSet()) == NULL) { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            // Recupera o resultado
            if (ptrQryDS->RecordCount != 1)                 { SNPSTRLOGLINE(strlog); MSGERRBD_EXECINV return(IDINDEFINIDO); }
            index.ndxRedeId = ptrQryDS->FieldByName("IdProx")->AsInteger;
            // Começa a contagem somente a partir da próxima chamada
            if (bolBeginNext) { index.ndxRedeId--; }
            ptrQryDS->Close();
            }
         catch (Exception& e)
            {
            SNPSTRLOGLINE(strlog);
            SNPSTRLOG(e.Message);
            index.ndxRedeId = IDINDEFINIDO;
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

   return(index.ndxRedeId);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDhistAccess::InsereArvore(VTArvore* arvore)
   {
   bool       sucesso = true;
   AnsiString strSQL;
   AnsiString strlog;
   AnsiString straux;
   VTNode*    node;
   TList*     lstNode = NULL;
   AnsiString strFileName;
   AnsiString strFilePath;

   if (!bd_conn)                                                { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())                              { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!arvore)                                                 { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV    return(false); }
   if (!lstStrData)                                             { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV    return(false); }
   // Reinicia indexadores
   FNSNP_rdArvoreIdProx(false, true);
   FNSNP_rdNoIdProx(false, true);
   FNSNP_rdRedeIdProx(false, true);

   __try
      {
      sucesso = true;
      lstNode = new TList();
      try
         {
         // Configuração do path dos arquivos de saída
         strFilePath = path->DirTmp() + "\\";
         // Escreve a árvore
         if (!SPSNP_wrArvore(arvore))                           { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV    return(false); }

         // Limpa a lista de dados a serem salvos em arquivo
         lstStrData->Clear();
         // Insere os nós da árvore
         arvore->LisAllNode(lstNode);
         for (int i = 0; i < lstNode->Count; i++)
            {
            if ((node = (VTNode*)lstNode->Items[i]) == NULL)    { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV    return(false); }
            node->Id = FNSNP_rdNoIdProx();
            // Formata a string para escrever no arquivo
            strSQL = Format("%d;%s;%d;%d;%d",
                            ARRAYOFCONST((
                            node->Id, node->Codigo, (node->Pai ? node->Pai->Id : node->Id), node->Level, arvore->Id
                            ))
                           );
            lstStrData->Add(strSQL);
            }
         // Executa a lista de comandos de nós
         if (lstStrData->Count > 0)
            {
            strFileName = "ArvoreNoOut.txt";
            lstStrData->SaveToFile(strFilePath + strFileName);

            strSQL  = "INSERT INTO ";
            strSQL += "ArvoreNo (";
            strSQL +=           "intNoId, strCodigo, intNoPaiId, intNivelId, intArvoreId";
            strSQL +=           ") ";
            strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath + "' [Text;HDR=NO;]";
            bd_conn->ParseCmd(strSQL);
            bd_conn->Execute(false);
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
      if (lstNode) { delete lstNode; lstNode = NULL; }
      }

   return(sucesso);
   }

//-----------------------------------------------------------------------------
// Esse código foi copiado do TMontaRede::InsereCurvaDemanda
// Isso foi feito para que as rotinas de montagem e desmontagem dos blobs fossem
// mantidas idênticas às demais utilizadas no BD
// OOPS: Isso não deveria ser incluído no bd_func???
VTCurva* __fastcall TBDhistAccess::InsereCurvaDemanda(VTCarga *carga, strCURVA &str)
   {
   //variáveis locais
   strHM     hm_ini, hm_fim;
   double    valor[32];
   VTCurva *curva;

   //JCG 2011.12.15
   //IMPORTANTE: strCURVA pode ser usado p/ definir curva c/ demanda total (nvCURVA_CAR valores) ou
   //            curva com demanda por fase (nvCURVA_RRCAR valores). Se strCURVA.num valor não estiver
   //            definido, assume curva de demanda total (default)

   //proteção: verifica se o número de valores da curva está definido
   if (str.num_valor == 0) str.num_valor = nvCURVA_PQ;
   //cria uma nova Curva
   curva                = DLL_NewObjCurva();
   curva->Id            = FalsoId();
   curva->Codigo        = "";
   curva->Tipo          = curvaPQ;
   curva->Unidade       = unidMVA;
   curva->Escala        = patEDITAVEL;
   curva->NumeroValores = str.num_valor;
   curva->Tipica        = false;
   curva->Eqpto         = NULL;
   curva->Classe        = NULL;
   //define pontos da Curva
   for (int np = 0; np < str.num_pat; np++)
       {
       hm_ini      = str.patamar[np].hm_ini;
       hm_fim      = str.patamar[np].hm_fim;
       for (int ind_val = 0; ind_val < str.num_valor; ind_val++)
         {
         valor[ind_val] = str.patamar[np].valor[ind_val];
         }
       curva->SetPonto(hm_ini, hm_fim, valor, str.num_valor);
       }
   //insere Cruva na Carga
   carga->DefineCurva(curva);
   return(curva);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDhistAccess::InsereDensidadeCarga(VTArvore* arvore)
   {
   bool       sucesso = true;
   AnsiString strlog;
   TList*     lstObj  = NULL;

   if (!bd_conn)                                                { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())                              { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!arvore)                                                 { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV    return(false); }
   if (arvore->TipoArvore != arvoreDENSIDADE)                   { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV    return(false); }
   // Reinicia indexador
   FNSNP_rdNoIdProx(false, true);

   __try
      {
      lstObj  = new TList();
      try
         {
         // obtém lista de objetos associados aos nós
         arvore->LisAllObject(lstObj);
         // Insere os objetos associados aos nós da árvore
         // O tipo de árvore determina em qual tabela os objetos serão escritos
         if (!SPSNP_wrRedeCarga(lstObj))                        { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV    return(false); }
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
      if (lstObj)  { delete lstObj;  lstObj  = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDhistAccess::InserePerdaTecnica(VTArvore* arvore)
   {
   bool       sucesso = true;
   AnsiString strSQL;
   AnsiString strlog;
   AnsiString straux;
   VTNode*    node;
   VTResumo*  resumo;
   TList*     lstObj  = NULL;
   AnsiString strFileName;
   AnsiString strFilePath;
   int        intNodeId;
   TVarRec*   args;

   if (!bd_conn)                                                { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())                              { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstStrData)                                             { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV    return(false); }
   if (!arvore)                                                 { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV    return(false); }
   if (!arvore->TipoArvore == arvorePERDATEC)                   { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV    return(false); }
   // Reinicia indexadores
   FNSNP_rdArvoreIdProx(false, true);
   FNSNP_rdNoIdProx(false, true);
   FNSNP_rdRedeIdProx(false, true);

   __try
      {
      lstObj  = new TList();
      try
         {
         // Configuração do path dos arquivos de saída
         strFilePath = path->DirTmp() + "\\";

         // obtém lista de objetos associados aos nós
         arvore->LisAllObject(lstObj);
         // Limpa a lista de dados a serem salvos em arquivo
         lstStrData->Clear();
         // Insere os objetos associados aos nós da árvore
         // O tipo de árvore determina em qual tabela os objetos serão escritos
         // Primeiro, aloca memória para os parâmetros
         for (int i = 0; i < lstObj->Count; i++)
            {
            if ((resumo = (VTResumo*)lstObj->Items[i]) == NULL) { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV    return(false); }
            if (!resumo->object)                                { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV return(false); }
            intNodeId = ((VTNode*)(resumo->object))->Id;

            TVarRec args[] =
               {
               intNodeId, resumo->eqpto->Id,
               resumo->balanco.EnergiaSuprimento, resumo->balanco.EnergiaGerador, resumo->balanco.EnergiaRecebidaExt,
               resumo->balanco.EnergiaEntregueExt, resumo->balanco.EnergiaCarga, resumo->balanco.EnergiaPerdaFluxo, resumo->balanco.EnergiaPerdaTotal,
               resumo->perda.PerdaCabo, resumo->perda.PerdaTrafoCobre, resumo->perda.PerdaTrafoFerro, resumo->perda.PerdaCapacitor,
               resumo->perda.PerdaReator, resumo->perda.PerdaRegulador, resumo->perda.PerdaMedidor, resumo->perda.PerdaRamal, resumo->perda.PerdaOutros,
               resumo->rede.QtdeTrafo, resumo->rede.QtdeCabo, resumo->rede.QtdeCapacitor, resumo->rede.QtdeReator, resumo->rede.QtdeRegulador,
               resumo->rede.QtdeConsA4, resumo->rede.QtdeConsRes, resumo->rede.QtdeConsCom, resumo->rede.QtdeConsInd, resumo->rede.QtdeConsRur,
               resumo->rede.QtdeConsOut, resumo->rede.QtdeConsNulo, resumo->rede.EnergiaMedida, resumo->rede.EnergiaMercado
               };
            //strSQL = Format("%d;%d;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d", args, (sizeof(args) / sizeof(TVarRec)) - 1);
            strSQL = Format("%d;%d;%g;%g;%g;%g;%g;%g;%g;%g;%g;%g;%g;%g;%g;%g;%g;%g;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%g;%g", args, (sizeof(args) / sizeof(TVarRec)) - 1);
            lstStrData->Add(strSQL);
            }

         // Salva os dados formatados no arquivo
         strFileName = "PerdaTecnicaOut.txt";
         lstStrData->SaveToFile(strFilePath + strFileName);
         // Executa o comando
         strSQL  = "INSERT INTO ";
         strSQL += "PerdaTecnica (";
         strSQL +=                "intNoId, intRedeId,";
         strSQL +=                "fltEnergiaSuprimento, fltEnergiaGerador, fltEnergiaRecebida, ";
         strSQL +=                "fltEnergiaEntregue, fltEnergiaCarga, fltEnergiaPerdaFluxo, fltEnergiaPerdaTotal, ";
         strSQL +=                "fltPerdaCabo, fltPerdaTrafoCu, fltPerdaTrafoFe, fltPerdaCapacitor, ";
         strSQL +=                "fltPerdaReator, fltPerdaRegulador, fltPerdaMedidor, fltPerdaRamal, fltPerdaOutros, ";
         strSQL +=                "intQtdeTrafo, intQtdeCabo, intQtdeCapacitor, intQtdeReator, intQtdeRegulador, ";
         strSQL +=                "intQtdeConsA4, intQtdeConsRes, intQtdeConsCom, intQtdeConsInd, intQtdeConsRur, ";
         strSQL +=                "intQtdeConsOut, intQtdeConsNulo, fltEnergiaMedida, fltEnergiaMercado";
         strSQL +=               ") ";
         strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath + "' [Text;HDR=NO;]";
         bd_conn->ParseCmd(strSQL);
         bd_conn->Execute(false);
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
      if (lstObj)  { delete lstObj;  lstObj  = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDhistAccess::InsereRede(TList* lisRede)
   {
   bool       sucesso = true;
   AnsiString strSQL;
   AnsiString strlog;
   AnsiString straux;
   VTRede*    rede;
   AnsiString strFileName;
   AnsiString strFilePath;

   if (!bd_conn)                                                { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())                              { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lisRede)                                                { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV    return(false); }
   if (!lstStrData)                                             { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV    return(false); }
   // Reinicia indexadores
   FNSNP_rdArvoreIdProx(false, true);
   FNSNP_rdNoIdProx(false, true);
   FNSNP_rdRedeIdProx(false, true);

   __try
      {
      sucesso = true;
      try
         {
         // Configuração do path dos arquivos de saída
         strFilePath = path->DirTmp() + "\\";

         // Limpa a lista de dados a serem salvos em arquivo
         lstStrData->Clear();
         for (int i = 0; i < lisRede->Count; i++)
            {
            if ((rede = (VTRede*)lisRede->Items[i]) == NULL)    { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV    return(false); }
            // OOPS: parto do princípio que a rede não foi salva
            rede->Id = FNSNP_rdRedeIdProx();
            if (!rede->TipoRede)                                { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV    return(false); }
            // Formata a string para escrever no arquivo
            strSQL = Format("%d;%s;%d",
                            ARRAYOFCONST((
                            rede->Id, rede->Codigo, rede->TipoRede->Id
                            ))
                           );
            lstStrData->Add(strSQL);
            }
         // Executa a lista de comandos de nós
         if (lstStrData->Count > 0)
            {
            strFileName = "RedeHistOut.txt";
            lstStrData->SaveToFile(strFilePath + strFileName);

            strSQL  = "INSERT INTO ";
            strSQL += "Rede (";
            strSQL +=       "intRedeId, strCodigo, intTipoRedeId";
            strSQL +=       ") ";
            strSQL += "SELECT * FROM [" + strFileName + "] IN '" + strFilePath + "' [Text;HDR=NO;]";
            bd_conn->ParseCmd(strSQL);
            bd_conn->Execute(false);
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
bool __fastcall TBDhistAccess::LeArvoreDensidadeCarga(VTArvore* arvore, int intLevel)
   {
   bool          sucesso = true;
   int           intNoId, intNoPaiId, intNoLevel, count;
   AnsiString    strlog;
   AnsiString    strSQL;
   TADOQuery     *ptrQryDS = NULL;
   VTNode        *node, *node_pai;
   VTNode        *node_raiz, *node_rede_conexa, *node_rede_isolada;
   VTResumoCarga *rescarga;
   VTRede        *rede;
   VTClasses     *classes;
   VTPatamares   *patamares;
   TBDfunc       *bd_func;
   int           intRedeId, intTipoRedeId, intClasseId;
   int           intUtmX, intUtmY;
   int           num_pontos, num_valores;
   VTBarra*      barra;
   VTCarga*      carga;
   VTMercado*    mercado;
   VTClasse*     classe;
   VTRedes*      redes;
   strCARGA      str_carga;
   TStream*      stream    = NULL;
   strCURHEADER  curhdr;
   VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

   //proteção
   if (!bd_conn)                                                                   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON   return(false); }
   if (!bd_conn->getConnectedOK())                                                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON   return(false); }
   if (!arvore)                                                                    { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV      return(false); }
   if ((classes   = (VTClasses*)apl->GetObject(__classid(VTClasses))) == NULL)     { SNPSTRLOGLINE(strlog); MSGERRSYS_CLASSESINV   return(false); }
   if ((mercado   = (VTMercado*)apl->GetObject(__classid(VTMercado))) == NULL)     { SNPSTRLOGLINE(strlog); MSGERRSYS_MERCADOINV   return(false); }
   if ((patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares))) == NULL) { SNPSTRLOGLINE(strlog); MSGERRSYS_PATAMARESINV return(false); }
   if ((redes     = (VTRedes*)apl->GetObject(__classid(VTRedes))) == NULL)         { SNPSTRLOGLINE(strlog); MSGERRSYS_REDESINV     return(false); }
   if ((bd_func   = (TBDfunc*)apl->GetObject(__classid(TBDfunc))) == NULL)         { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV      return(false); }

   __try
      {
      try{//inicia objeto Progresso
         if (progresso) progresso->Start(progTEXTO);
         if (progresso) progresso->Add("Leitura de Densidade de Cargas");
         // Limpa a árvore
         arvore->Clear();
         // Monta a string de execução
         strSQL = "EXEC SPSNP_rdArvore_DensidadeCarga :p_ArvoreId :p_NivelId";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_ArvoreId", PARAMIN_INT(arvore->Id)))  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }
         if (!bd_conn->SetParam("p_NivelId",  PARAMIN_INT(intLevel)))    { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }
         // Comando da consulta
         if (!bd_conn->Execute(true))                                    { SNPSTRLOGLINE(strlog);                      return(false); }
         if ((ptrQryDS = bd_conn->getDataSet()) == NULL)                 { SNPSTRLOGLINE(strlog);                      return(false); }
         //inicia objeto Progresso
         if ((count = (ptrQryDS->RecordCount / 100)) == 0) count = 1;
         //cria Node para identificar a Arvore
         node_raiz          = arvore->InsereNode();
         node_raiz->Codigo  = DateToStr(arvore->Data);
         //cria Node para redes conexas
         node_rede_conexa          = arvore->InsereNode(node_raiz);
         node_rede_conexa->Codigo  = "Redes";
         //cria Node para redes desconexas
         node_rede_isolada         = arvore->InsereNode(node_raiz);
         node_rede_isolada->Codigo = "Redes Isoladas";
         // loop p/ todos registros (ini)
         rede = NULL;
         node = node_pai = NULL;
         for (int i = 0; i < ptrQryDS->RecordCount; i++)
            {//atualiza progresso
            //determina Level
            intNoId    = ptrQryDS->FieldByName("NoId")->AsInteger;
            intNoPaiId = ptrQryDS->FieldByName("NoPaiId")->AsInteger;
            intNoLevel = ptrQryDS->FieldByName("NoNivelId")->AsInteger;
            //determina Node pai: verifica se Node de Level 0 ou sem indicação de Node pai
            if ((intNoLevel == 1)||(intNoPaiId < 0))
               {//insere Node em node_rede_conexa ou node_rede_isolada
//FALTA TESTE P/ IDENTIFICAR NODE PAI
               node_pai = node_rede_conexa;
               }
            else
               {//verifica se Node lido anteriormente é o Node pai
               if ((node != NULL)&&(node->Id == intNoPaiId)) node_pai = node;
               if ((node_pai == NULL)||(node_pai->Id != intNoPaiId))
                  {//procura Node pai na Arvore
                  node_pai = arvore->ExisteNode(intNoPaiId);
                  }
               //proteção: se não encontrar Node pai, insere em node_rede_isolada
               if (node_pai == NULL)
                  {
                  node_pai = node_rede_isolada;
                  }
               }
            //verifica se o ID é do mesmo No
            if ((node == NULL)||(node->Id != intNoId))
               {//verifica se o Node já existe na Arvore
               //if ((node = arvore->ExisteNode(intNoId)) == NULL)
                  {//cria um novo nó a ser inserido na árvore
                  node         = arvore->InsereNode(node_pai);
                  node->Id     = intNoId;
                  node->Codigo = ptrQryDS->FieldByName("NoCodigo")->AsString;
                  }
               }
            //proteção
            if (node == NULL) continue;
            //cria novo resumo e insere no Node
            node->InsereObject(rescarga = NewObjResumoCarga());
            //lê dados da Rede
            intRedeId     = ptrQryDS->FieldByName("RedeId")->AsInteger;
            intTipoRedeId = ptrQryDS->FieldByName("TipoRedeId")->AsInteger;
            // Verifica se a rede permanece a mesma, ou troca, se necessário
            if ((rede == NULL)||(rede->Id != intRedeId))
               {//se necessário, cria uma nova Rede
               if ((rede = ExisteRede(intRedeId, intTipoRedeId)) == NULL)   { SNPSTRLOGLINE(strlog);}
               rede->Codigo = ptrQryDS->FieldByName("RedeCodigo")->AsString;
               }
            rescarga->Rede = rede;

            //lê coordenadas da Barra da Carga
            intUtmX = ptrQryDS->FieldByName("UtmX")->AsInteger;
            intUtmY = ptrQryDS->FieldByName("UtmY")->AsInteger;
            //se necessário, cria uma nova Barra
            if ((barra = ExisteBarra(rede, intUtmX, intUtmY)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               continue;
               }
            //determina Classe da Carga
            intClasseId = ptrQryDS->FieldByName("ClasseId")->AsInteger;
            if ((classe = classes->ExisteClasse(intClasseId)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               continue;
               }
            //desmonta o blob e popula a curva criada
            // O número de pontos e valores está dentro do blob
            stream = ptrQryDS->CreateBlobStream(ptrQryDS->FieldByName("Pontos"), bmRead);
            if (!bd_func->DesmontaStreamCurvaEquipamento(stream, &(str_carga.curva), curhdr))
               {
               SNPSTRLOGLINE(strlog);
               return(false);
               }
            delete stream;

            //cria objeto VTCarga
            carga             = DLL_NewObjCarga(patamares->NumPatamar());
            carga->Id         = ptrQryDS->FieldByName("CargaId")->AsInteger;
            carga->Codigo     = ptrQryDS->FieldByName("Codigo")->AsString;
            carga->IP_NumLamp = ptrQryDS->FieldByName("NumConsumidor")->AsInteger;
            //define Classe e Curva da Carga
            carga->DefineClasse(classe);
            carga->Show[eqp_VISIBLE] = false;
            //insere carga na sua Barra
            barra->InsereEqbar(carga);
            //insere curva na carga
            InsereCurvaDemanda(carga, str_carga.curva);
            //define Crescimento de Mercado
            //mercado->DefineCrescimento(carga, redes);
            mercado->DefineCrescimento(carga, rede);
            // Insere a carga no objeto resumo
            rescarga->InsereCarga(carga);
            //próximo registro
            ptrQryDS->Next();
            // loop registros (end)
            }
         ptrQryDS->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         if (arvore) { delete arvore; arvore = NULL; }
         }
      }
   __finally
      {
      // Colocado dentro do finally para garantir que será deletado, mesmo que
      // haja um return no meio do código

      //esconde janela de progresso
      if (progresso) progresso->Stop();
      //destrói objetos locais
      if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDhistAccess::LeArvorePerdaTecnica(VTArvore* arvore, int intLevel)
   {
   bool        sucesso = true;
   int         intNoId, intNoPaiId, intNoLevel, count;
   AnsiString  strlog;
   AnsiString  strSQL;
   TADOQuery   *ptrQryDS = NULL;
   VTNode      *node, *node_pai;
   VTNode      *node_raiz, *node_rede_conexa, *node_rede_isolada;
   VTResumo    *resumo;
   VTProgresso *progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

   //proteção
   if (!bd_conn)                                                         { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())                                       { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!arvore)                                                          { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV    return(false); }

   __try
      {
      try
         {//exibe progresso
         if (progresso) progresso->Start(progTEXTO);
         if (progresso) progresso->Add("Leitura de Perdas Técnicas");
         // Limpa a árvore
         arvore->Clear();
         // Monta a string de execução
			strSQL = "EXEC SPSNP_rdArvore_PerdaTecnica :p_NivelId :p_ArvoreId";
			// Decodifica comando
			bd_conn->ParseCmd(strSQL);
			// Determina os valores dos parâmetros
			if (!bd_conn->SetParam("p_NivelId",  PARAMIN_INT(intLevel)))    { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }
			if (!bd_conn->SetParam("p_ArvoreId", PARAMIN_INT(arvore->Id)))  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }
			// Comando da consulta
         if (!bd_conn->Execute(true))                                    { SNPSTRLOGLINE(strlog);                      return(false); }
         if ((ptrQryDS = bd_conn->getDataSet()) == NULL)                 { SNPSTRLOGLINE(strlog);                      return(false); }
         //inicia objeto Progresso
         if ((count = (ptrQryDS->RecordCount / 100)) == 0) count = 1;
         //cria Node para identificar a Arvore
         node_raiz          = arvore->InsereNode();
         node_raiz->Codigo  = DateToStr(arvore->Data);
         //cria Node para redes conexas
         node_rede_conexa          = arvore->InsereNode(node_raiz);
         node_rede_conexa->Codigo  = "Redes";
         //cria Node para redes desconexas
         node_rede_isolada         = arvore->InsereNode(node_raiz);
         node_rede_isolada->Codigo = "Redes Isoladas";
         // loop p/ todos registros (ini)
         node = node_pai = NULL;
         for (int i = 0; i < ptrQryDS->RecordCount; i++)
            {//determina Level
            intNoId    = ptrQryDS->FieldByName("NoId")->AsInteger;
            intNoPaiId = ptrQryDS->FieldByName("NoPaiId")->AsInteger;
            intNoLevel = ptrQryDS->FieldByName("NoNivelId")->AsInteger;
            //determina Node pai: verifica se Node de Level 0 ou sem indicação de Node pai
            if ((intNoLevel == 1)||(intNoPaiId < 0))
               {//insere Node em node_rede_conexa ou node_rede_isolada
//FALTA TESTE P/ IDENTIFICAR NODE PAI
               node_pai = node_rede_conexa;
               }
            else
               {//verifica se Node lido anteriormente é o Node pai
               if ((node != NULL)&&(node->Id == intNoPaiId)) node_pai = node;
               if ((node_pai == NULL)||(node_pai->Id != intNoPaiId))
                  {//procura Node pai na Arvore
                  node_pai = arvore->ExisteNode(intNoPaiId);
                  }
               //proteção: se não encontrar Node pai, insere em node_rede_isolada
               if (node_pai == NULL)
                  {
                  node_pai = node_rede_isolada;
                  }
               }
            //verifica se o ID é do mesmo No
            if ((node == NULL)||(node->Id != intNoId))
               {//verifica se o Node já existe na Arvore
//               if ((node = arvore->ExisteNode(intNoId)) == NULL)
                  {//cria um novo nó a ser inserido na árvore
                  node         = arvore->InsereNode(node_pai);
                  node->Id     = intNoId;
                  node->Codigo = ptrQryDS->FieldByName("NoCodigo")->AsString;
                  }
               }
            //proteção
            if (node == NULL) continue;
            //cria novo resumo e insere no Node
            node->InsereObject(resumo         = NewObjResumo());
            resumo->tipo_resumo               = resumoREDE;
            //associa Resumo com seu Node
            resumo->object = node;
            //lê dados da Rede
            resumo->rede.Id                    = ptrQryDS->FieldByName("RedeId")->AsInteger;
            resumo->rede.Codigo                = ptrQryDS->FieldByName("RedeCodigo")->AsString;
            resumo->rede.TipoRedeId            = ptrQryDS->FieldByName("TipoRedeId")->AsInteger;
            resumo->rede.TipoRede              = ptrQryDS->FieldByName("TipoRedeCodigo")->AsString;
            resumo->rede.QtdeRede              = 1;
            //lê atributos de balança de energia
            resumo->balanco.EnergiaSuprimento  = ptrQryDS->FieldByName("EnergiaSuprimento")->AsFloat;
            resumo->balanco.EnergiaGerador     = ptrQryDS->FieldByName("EnergiaGerador")->AsFloat;
            resumo->balanco.EnergiaRecebidaExt = ptrQryDS->FieldByName("EnergiaRecebida")->AsFloat;
            resumo->balanco.EnergiaEntregueExt = ptrQryDS->FieldByName("EnergiaEntregue")->AsFloat;
            resumo->balanco.EnergiaCarga       = ptrQryDS->FieldByName("EnergiaCarga")->AsFloat;
            resumo->balanco.EnergiaPerdaFluxo  = ptrQryDS->FieldByName("EnergiaPerdaFluxo")->AsFloat;
            resumo->balanco.EnergiaPerdaTotal  = ptrQryDS->FieldByName("EnergiaPerdaTotal")->AsFloat;
            //calcula EnergiaEntrada
            resumo->balanco.EnergiaEntrada    = resumo->balanco.EnergiaSuprimento + resumo->balanco.EnergiaGerador + resumo->balanco.EnergiaRecebidaExt;
//            resumo->balanco.EnergiaEntrada     = resumo->balanco.EnergiaSuprimento + resumo->balanco.EnergiaGerador;
			//lê atributos de balança de perda
            resumo->perda.PerdaCabo            = ptrQryDS->FieldByName("PerdaCabo")->AsFloat;
            resumo->perda.PerdaTrafoCobre      = ptrQryDS->FieldByName("PerdaTrafoCu")->AsFloat;
            resumo->perda.PerdaTrafoFerro      = ptrQryDS->FieldByName("PerdaTrafoFe")->AsFloat;
            resumo->perda.PerdaCapacitor       = ptrQryDS->FieldByName("PerdaCapacitor")->AsFloat;
            resumo->perda.PerdaReator          = ptrQryDS->FieldByName("PerdaReator")->AsFloat;
            resumo->perda.PerdaRegulador       = ptrQryDS->FieldByName("PerdaRegulador")->AsFloat;
            resumo->perda.PerdaMedidor         = ptrQryDS->FieldByName("PerdaMedidor")->AsFloat;
            resumo->perda.PerdaRamal           = ptrQryDS->FieldByName("PerdaRamal")->AsFloat;
            resumo->perda.PerdaOutros          = ptrQryDS->FieldByName("PerdaOutros")->AsFloat;
            // Quantidade de equipamentos e consumidores
            resumo->rede.QtdeTrafo             = ptrQryDS->FieldByName("QtdeTrafo")->AsInteger;
            resumo->rede.QtdeCabo              = ptrQryDS->FieldByName("QtdeCabo")->AsInteger;
            resumo->rede.QtdeCapacitor         = ptrQryDS->FieldByName("QtdeCapacitor")->AsInteger;
            resumo->rede.QtdeReator            = ptrQryDS->FieldByName("QtdeReator")->AsInteger;
            resumo->rede.QtdeRegulador         = ptrQryDS->FieldByName("QtdeRegulador")->AsInteger;
            resumo->rede.QtdeConsA4            = ptrQryDS->FieldByName("QtdeConsA4")->AsInteger;
            resumo->rede.QtdeConsRes           = ptrQryDS->FieldByName("QtdeConsRes")->AsInteger;
            resumo->rede.QtdeConsCom           = ptrQryDS->FieldByName("QtdeConsCom")->AsInteger;
            resumo->rede.QtdeConsInd           = ptrQryDS->FieldByName("QtdeConsInd")->AsInteger;
            resumo->rede.QtdeConsRur           = ptrQryDS->FieldByName("QtdeConsRur")->AsInteger;
            resumo->rede.QtdeConsOut           = ptrQryDS->FieldByName("QtdeConsOut")->AsInteger;
            resumo->rede.QtdeConsNulo          = ptrQryDS->FieldByName("QtdeConsNulo")->AsInteger;
            //proteção para leitura da base antiga que não tem os campos EnergiaMedida e EnergiaMercado
            if (ptrQryDS->FindField("EnergiaMedida") != NULL)
               {
               resumo->rede.EnergiaMedida  = ptrQryDS->FindField("EnergiaMedida")->AsFloat;
               resumo->rede.EnergiaMercado = ptrQryDS->FindField("EnergiaMercado")->AsFloat;
               }
            else
               {
               resumo->rede.EnergiaMedida  = 0;
               resumo->rede.EnergiaMercado = 0;
               }
            ptrQryDS->Next();
            }
         ptrQryDS->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         if (arvore) { delete arvore; arvore = NULL; }
         }
      }
   __finally
      {
      // Colocado dentro do finally para garantir que será deletado, mesmo que
      // haja um return no meio do código

      //destrói objetos locais
      if ((bd_conn->getMultiDataSet())&&(ptrQryDS)) {delete ptrQryDS;}
      //esconde janela de progresso
      if (progresso) progresso->Stop();
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool  __fastcall TBDhistAccess::LeListaArvore(TList* lstArvore)
   {
	bool       sucesso = true;
	AnsiString strlog;
	AnsiString strSQL;
	TADOQuery* ptrQryDS = NULL;
	VTArvore*  arvore   = NULL;

	if (!bd_conn)                   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
	if (!bd_conn->getConnectedOK()) { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
	if (!lstArvore)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV    return(false); }

	__try
		{
		try
			{
			// Monta a string de execução
			strSQL = "EXEC SPSNP_rdListaArvore";
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
				arvore             = DLL_NewObjArvore(apl);
				arvore->Id         = ptrQryDS->FieldByName("ArvoreId")->AsInteger;
            // OOPS: Falta código na árvore ???
            //arvore->codigo     = ptrQryDS->FieldByName("Codigo")->AsString;
				arvore->Data       = ptrQryDS->FieldByName("DataAtualizacao")->AsDateTime;
            arvore->TipoArvore = ptrQryDS->FieldByName("TipoArvoreId")->AsInteger;

            lstArvore->Add(arvore);

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
bool __fastcall TBDhistAccess::SPSNP_wrArvore(VTArvore* arvore)
   {
   bool       sucesso = true;
   AnsiString strSQL;
   AnsiString strlog;
   VTArvore   *arvore_bd;

   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!arvore)                      { SNPSTRLOGLINE(strlog); MSGERRSYS_PONTINV    return(false); }

   // Escrita propriamente dita
   __try
      {
      try
         {//verifica se a Arvore já existe na base
         if ((arvore_bd = ExisteArvoreNaBase(arvore->TipoArvore, arvore->Data)) != NULL)
            {//assume mesmo ID da Arvore existente na base
            arvore->Id = arvore_bd->Id;
            //destrói Arvore
            delete arvore_bd;
            return(true);
            }
         //define Id
         arvore->Id = FNSNP_rdArvoreIdProx();
         // Monta a string de execução
         strSQL = "EXEC SPSNP_wrArvore :p_ArvoreId, :p_Codigo, :p_Data, :p_TipoArvoreId";
         // Decodifica comando
         bd_conn->ParseCmd(strSQL);
         // OOPS: árvore não tem código?
         // Determina os valores dos parâmetros
         if (!bd_conn->SetParam("p_ArvoreId",     PARAMIN_INT(arvore->Id)))         { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Codigo",       PARAMIN_STR("")))                 { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_Data",         PARAMIN_DTT(arvore->Data)))       { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
         if (!bd_conn->SetParam("p_TipoArvoreId", PARAMIN_INT(arvore->TipoArvore))) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV return(false); }
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
bool __fastcall TBDhistAccess::SPSNP_wrRedeCarga(TList* lisResumoCarga)
   {
   bool           bolret;
   AnsiString     strSQL;
   AnsiString     strlog;
   VTResumoCarga* rescarga;
   TList*         lisCarga;
   VTCarga*       carga;
   TStream*       ptrStream;
   TBDfunc*       bd_func = (TBDfunc*)apl->GetObject(__classid(TBDfunc));

   bolret = true;
   if (!bd_conn)                     { SNPSTRLOGLINE(strlog); return(false); }
   if (!bd_conn->getConnectedOK())   { SNPSTRLOGLINE(strlog); return(false); }
   if (!lisResumoCarga)              { SNPSTRLOGLINE(strlog); return(false); }
   if (!bd_func)                     { SNPSTRLOGLINE(strlog); return(false); }

   try
      {
      // Monta a string de execução
      strSQL  = "EXEC SPSNP_wrRedeCarga ";
      strSQL += ":p_NoId, :p_RedeId, :p_CargaId, :p_Codigo, :p_ClasseId, :p_UtmX, :p_UtmY, ";
      strSQL += ":p_NumConsumidor, :p_Pontos";

      // Decodifica comando
      bd_conn->ParseCmd(strSQL);

      for (int n = 0; n < lisResumoCarga->Count; n++)
         {
         // Loop de VTResumoCarga (ini)
         if ((rescarga = (VTResumoCarga*)lisResumoCarga->Items[n]) == NULL) { SNPSTRLOGLINE(strlog); return(false); }
         if (rescarga->Node == NULL)                                        { SNPSTRLOGLINE(strlog); return(false); }
         if (rescarga->Rede == NULL)                                        { SNPSTRLOGLINE(strlog); return(false); }
         if ((lisCarga = rescarga->LisCarga()) == NULL)                     { SNPSTRLOGLINE(strlog); return(false); }
         for (int i = 0; i < lisCarga->Count; i++)
            {
            // Loop cargas (ini)
            if ((carga = (VTCarga*)lisCarga->Items[i]) == NULL)             { SNPSTRLOGLINE(strlog); return(false); }
            if (!carga->classe)                                             { SNPSTRLOGLINE(strlog); return(false); }
            if (!carga->pbarra)                                             { SNPSTRLOGLINE(strlog); return(false); }
            if (!carga->curva)                                              { SNPSTRLOGLINE(strlog); return(false); }
            // Como a carga faz parte da chave, gera o identificador da carga (por rede)
            carga->Id = i + 1;

            // Determina os valores dos parâmetros
            //dados da rede
            bd_conn->SetParam("p_NoId"          , ftInteger, pdInput, sizeof(int), Variant(rescarga->Node->Id)  );
            bd_conn->SetParam("p_RedeId"        , ftInteger, pdInput, sizeof(int), Variant(rescarga->Rede->Id)  );
            bd_conn->SetParam("p_CargaId"       , ftInteger, pdInput, sizeof(int), Variant(carga->Id)           );
            bd_conn->SetParam("p_Codigo"        , ftString , pdInput, sizeof(int), AnsiString(carga->Codigo)    );
            bd_conn->SetParam("p_ClasseId"      , ftInteger, pdInput, sizeof(int), Variant(carga->classe->Id)   );
            bd_conn->SetParam("p_UtmX"          , ftInteger, pdInput, sizeof(int), Variant(carga->pbarra->utm.x));
            bd_conn->SetParam("p_UtmY"          , ftInteger, pdInput, sizeof(int), Variant(carga->pbarra->utm.y));
            bd_conn->SetParam("p_NumConsumidor" , ftInteger, pdInput, sizeof(int), Variant(carga->IP_NumLamp)   );

            // Monta a curva com as funções padrão
            ptrStream = bd_func->MontaStreamCurvaEquipamento(eqptoCARGA, carga->curva);
            if (!ptrStream)          { SNPSTRLOGLINE(strlog); return(false); }
            // Copia a curva no Blob
            TParameter* ptrParam = bd_conn->SetParam("p_Pontos", ftBlob, pdInput, sizeof(int), NULL);
            ptrParam->LoadFromStream(ptrStream, ptrParam->DataType);
            // Libera a memória alocada para o stream
            delete ptrStream;
            ptrStream = NULL;

            // Comando da consulta
            if (bd_conn->Execute(false))
               {
               bolret = true;
               }
               else
               {
               SNPSTRLOGLINE(strlog);
               bolret = false;
               }
            // Loop cargas (fim)
            }
         // Loop de VTResumoCarga (fim)
         }
      }
   catch (Exception& e)
      {
      SNPSTRLOGLINE(strlog);
      SNPSTRLOG(e.Message);
      bolret = false;
      }

   return(bolret);
   }

