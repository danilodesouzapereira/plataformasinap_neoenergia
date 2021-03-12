//---------------------------------------------------------------------------
#include <vcl.h>
#include <Oracle.hpp>
#pragma hdrstop
#include <TypInfo.hpp>
#include "TBDGateway.h"
#include "VTMontaArvore.h"
#include "Fontes\Apl\VTApl.h"
#include "Fontes\Arranjo\VTArranjo.h"
#include "Fontes\Arranjo\VTArranjos.h"
#include "Fontes\Arranjo\VTCabo.h"
#include "Fontes\Arranjo\VTPtoFixacao.h"
#include "Fontes\Arranjo\VTSuporte.h"
#include "Fontes\BD\Rede\TBDfunc.h"
#include "Fontes\Classes\VTClasse.h"           // ($SINAP) no Project Options -> Include Path
#include "Fontes\Cronometro\VTCronometro.h"    // ($SINAP) no Project Options -> Include Path
#include "Fontes\Curvas\VTCurva.h"             // ($SINAP) no Project Options -> Include Path
#include "Fontes\Curvas\VTCurvas.h"            // ($SINAP) no Project Options -> Include Path
#include "Fontes\Diretorio\VTPath.h"           // ($SINAP) no Project Options -> Include Path
#include "Fontes\Log\VTLog.h"                  // ($SINAP) no Project Options -> Include Path
#include "Fontes\Classes\VTClasses.h"
#include "Fontes\Progresso\VTProgresso.h"
#include "Fontes\Radial\VTArvore.h"
#include "Fontes\Radial\VTNode.h"
#include "Fontes\Rede\VTBarra.h"
#include "Fontes\Rede\VTCapacitor.h"
#include "Fontes\Rede\VTCapserie.h"
#include "Fontes\Rede\VTCarga.h"
#include "Fontes\Rede\VTCargaNL.h"
#include "Fontes\Rede\VTChave.h"
#include "Fontes\Rede\VTCluster.h"
#include "Fontes\Rede\VTEqbar.h"
#include "Fontes\Rede\VTEqpto.h"
#include "Fontes\Rede\VTFiltro.h"
#include "Fontes\Rede\VTGerador.h"
#include "Fontes\Rede\VTLigacao.h"
#include "Fontes\Rede\VTMNet.h"
#include "Fontes\Rede\VTMRede.h"
#include "Fontes\Rede\VTMutua.h"
#include "Fontes\Rede\VTReator.h"
#include "Fontes\Rede\VTRede.h"
#include "Fontes\Rede\VTRedes.h"
#include "Fontes\Rede\VTRegulador.h"
#include "Fontes\Rede\VTSuprimento.h"
#include "Fontes\Rede\VTTrafo.h"
#include "Fontes\Rede\VTTrafo3E.h"
#include "Fontes\Rede\VTTrafoZZ.h"
#include "Fontes\Rede\VTTipoChave.h"
#include "Fontes\Rede\VTTipoRede.h"
#include "Fontes\Rede\VTTipos.h"
#include "Fontes\Rede\VTTrecho.h"
#include "Fontes\Importa\Monta\TStruct.h"
#include "Fontes\Importa\Monta\VTMonta.h"

#include "DLL_Inc\Classes.h"
#include "DLL_Inc\Cronometro.h"
#include "DLL_Inc\Funcao.h"
#include "DLL_Inc\Log.h"
#include "DLL_Inc\Progresso.h"
#include "DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Oracle"

//---------------------------------------------------------------------------
VTBDGateway* NewObjBDGateway(VTApl *apl_owner)
   {
   return(new TBDGateway(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TBDGateway::TBDGateway(VTApl *apl_owner)
   {
   AnsiString straux;
   AnsiString strlog;

   CoInitialize(NULL);

   //cria objetos
   apl = NewObjApl(NULL, apl_owner);
   plog = (VTLog*)apl->GetObject(__classid(VTLog));
   if (!plog)
      {
      apl->Add(plog = DLL_NewObjLog(apl));
      }
   apl->Add(DLL_NewObjCronometro(apl));
   // OOPS: o progresso está vindo nulo. Criar um aqui???
   // apl->Add(DLL_NewObjProgresso(NULL, NULL));
   // Leitura de objetos criados externamente
   if ((monta = (VTMonta*)apl->GetObject(__classid(VTMonta))) == NULL)                { SNPSTRLOGLINE(strlog); }
   if ((classes = (VTClasses*)apl->GetObject(__classid(VTClasses))) == NULL)          { SNPSTRLOGLINE(strlog); }
   if ((tipos = (VTTipos*)apl->GetObject(__classid(VTTipos))) == NULL)                { SNPSTRLOGLINE(strlog); }
   if (((bd_func = (TBDfunc*)apl->GetObject(__classid(TBDfunc)))) == NULL)            { SNPSTRLOGLINE(strlog); }
   if ((arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos))) == NULL)       { SNPSTRLOGLINE(strlog); }
   if ((classes = (VTClasses*)apl->GetObject(__classid(VTClasses))) == NULL)          { SNPSTRLOGLINE(strlog); }
   if ((cronometro = (VTCronometro*)apl->GetObject(__classid(VTCronometro))) == NULL) { SNPSTRLOGLINE(strlog); }
   if ((progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso))) == NULL)    { SNPSTRLOGLINE(strlog); }
   if ((redes = (VTRedes*)apl->GetObject(__classid(VTRedes))) == NULL)                { SNPSTRLOGLINE(strlog); }
   if ((curvas = (VTCurvas*)apl->GetObject(__classid(VTCurvas))) == NULL)             { SNPSTRLOGLINE(strlog); }

   //inicia dados
   revisao            = "Rev1.0";
   cronometro_enabled = true;
   progresso_enabled  = true;
   bd_session  = NULL;
   dbfConn            = NULL;
   strUltimoErro      = _T("");

   //configura Log
   plog->ExibirDataHora = true;
   plog->Open(SNPLOGFILENAME, true);
   if ((path = (VTPath*)apl->GetObject(__classid(VTPath))) != NULL)
      {
      straux  = path->DirTmp() + "\\";
      straux += SNPLOGFILENAME;
      plog->Open(straux, true /* Append */);
      }
   }

//---------------------------------------------------------------------------
__fastcall TBDGateway::~TBDGateway(void)
   {
   //cancela eventual conexão ativa
   Desconecta();
   // Destroi conexão com arquivos DBF
   if (dbfConn)         { delete dbfConn;    dbfConn = NULL;     }
   //fecha o arquivo de log
   if (plog)            { plog->Close();                         }
   // Destroi objetos locais
   if (bd_session)      { delete bd_session; bd_session = NULL;  }
   //destrói Apl
   if (apl)             { delete apl;        apl = NULL;         }

   CoUninitialize();
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::Clear(void)
   {
   //variáveis locais
   bool sucesso;

   try{//verifica se existe conexão com a base
   if (! Conectado) return(false);
      //apaga todos os dados existentes na base de dados
      sucesso = SPSNP_dlTudo();
      }
   catch(Exception &e)
      {
      sucesso = false;
      }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::Commit(void)
   {
   if (bd_session)
      {
      bd_session->Commit();
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::Conecta(void)
   {
   try
      {//cancela eventual Conexao pre-existente
      if (! Desconecta()) return(false);
      //destrói eventual conexão pré-existente
      if (bd_session) delete bd_session;
      //cria Conexao c/ bases Oracle
//      bd_session = IniciaConexaoOracle("SNPGATEW", "USRGATEWAY", "USRGATEWAYPWD");
      bd_session = IniciaConexaoOracle("SINAPSIS_GATEWAY", "USRSNPGATEWAY", "USRSNPGATEWAYPWD");
      if (bd_session == NULL) return(false);
      }
   catch(Exception &e)
      {
      return(false);
      }

   return(true);
   /*
   Retirado da COnectaServidor, caso se queira buscar no registry os dados

   //variáveis locais
   struct   {
            AnsiString tipo, TNS, user_name, password;
            }servidor;
   bool       sucesso;
   VTRegistro *registro;
   VTProduto  *produto = (VTProduto*)apl->GetObject(__classid(VTProduto));

   // Testa se o produto existe
   if (!produto) return(false);

   try{//cria objeto VTRegistro p/ ler registro do Windows
      if ((registro = DLL_NewObjRegistro(produto->Codigo)) == NULL) return(false);
      //verifica se o servidor está configurado
      if (registro->ExisteInfo("servidor\\Rede", "tipo", servidor.tipo     )&&
          registro->ExisteInfo("servidor\\Rede", "tns" , servidor.TNS      )&&
          registro->ExisteInfo("servidor\\Rede", "user", servidor.user_name)&&
          registro->ExisteInfo("servidor\\Rede", "pswd", servidor.password ))
         {//seleciona tipo de servidor
         if (servidor.tipo.AnsiCompareIC("oracle") == 0)
            {
            sucesso = ConectaBaseRedeOracle(servidor.TNS, servidor.user_name, servidor.password);
            }
         else if (servidor.tipo.AnsiCompareIC("SQLserver") == 0)
            {
         // Não implementado
            sucesso = false;
            }
         }
      else
         {
         sucesso = false;
         }
      //destrói objeto Registro
      delete registro;
      }catch(Exception &e)
         {
         sucesso = false;
         }
   */
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::Desconecta(void)
   {
   // Na versão original matava os ponteiros de BDGatewayrede e BDGatewayhist.

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TBDGateway::DecodificaUltimoErroDB(SNPSTR strErr)
   {
   // Espera uma string no formato: "ORA-20000: ErrNum:-2291;INSERÇÃO;ALIM;BAR"
   // Separados por ';' estão:
   // 1) Código de erro
   // 2) Operação
   // 3) Tabela em que foi efetuada a tentativa de operação
   // 4) Tabela em que causou o erro, no caso de errp 2291 (chave estrangeira).
   //    Nos demais casos, retorna a mesma string que chegou
   SNPSTR strErro;
   int    intErro;
   SNPSTR strOperacao;
   SNPSTR strTabelaDst;
   SNPSTR strTabelaOrg;

   TStringList* lstToken = new TStringList();
   lstToken->Delimiter = _T(';');
   lstToken->DelimitedText = strErr;

   strUltimoErro = _T("Erro indefinido");
   if (lstToken->Count > 0)
      {
      strErro = lstToken->Strings[1];
      intErro = StrToInt(strErro) * -1;
      switch (intErro)
         {
         case 2291 :
            strOperacao  = lstToken->Strings[2];
            strTabelaDst = lstToken->Strings[3];
            strTabelaOrg = lstToken->Strings[4];

            strUltimoErro  = "Erro de " + strOperacao + " na tabela " + strTabelaDst;
            strUltimoErro += ". Valor inexistente na tabela " + strTabelaOrg + ", referenciada pela tabela " + strTabelaDst + ".";
            SNPSTRLOG(UltimoErro());
         break;

         default:
           strUltimoErro = lstToken->Strings[1];
           SNPSTRLOG(UltimoErro());
         }
      }

   MessageDlg("Erro na execução: " + strUltimoErro, mtError, TMsgDlgButtons() << mbOK, 0);
   delete lstToken;
   }


//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SalvaArranjos(void)
   {
   VTCabo*      cabo;
   TList*       lisCABO;
   VTSuporte*   suporte;
   TList*       lisSUPORTE;
   VTArranjo*   arranjo;
   TList*       lisARRANJOS;
   AnsiString   strlog;

   if (!arranjos)   { SNPSTRLOGLINE(strlog); MSGERRBDWR_ARRANJOS return(false); }
   
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
bool __fastcall TBDGateway::SalvaRedes(void)
   {
   bool         sucesso;
   TStringList* lstStrExec;
   SNPSTR       strStatus;
   TList*       lisEQP = NULL;
   TList*       lisREDES = NULL;
   VTRede*      rede = NULL;
   AnsiString   strlog;

   // Evita explosão sumária
   if (!redes)                      { SNPSTRLOGLINE(strlog); MSGERRSYS_REDESINV   return(false); }

   __try
      {
      try
         {
         lisEQP = new TList();
         lstStrExec = new TStringList();
/*
         // Apaga os tipos (arranjos, cabos, etc.) existentes na base de dados
         sucesso = SPSNP_dlTudo();
         if (!sucesso)
            {
            strStatus = _T("Erro ao reiniciar a base");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }
*/
         // Carrega os tipos (arranjos, cabos, etc.) na base de dados
         sucesso = SalvaTipos();
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar Tipos");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Salva as barras
         // OOPS: SOmente os equipamentos não isolados?
         redes->LisEqptoNaoIsolado(lisEQP, eqptoBARRA);
         sucesso = SPSNP_wrBarra(lisEQP);
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar as barras");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Salva as redes
         // OOPS: MRede e Supridora
         lisEQP->Clear();
         redes->LisRede(lisEQP);
         sucesso = SPSNP_wrRede(lisEQP);
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar as redes");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Salva os trechos
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoTRECHO);
         sucesso = SPSNP_wrTrecho(lisEQP);
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar os trechos");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Salva as cargas
         // OOPS: SOmente os equipamentos não isolados?
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoCARGA);
         sucesso = SPSNP_wrCarga(lisEQP);
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar as cargas");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Salva as cargas
         // ATENÇÃO
         // Aproveita a lista de carga, montada na instrução anterior
         sucesso = SPSNP_wrCargaIP(lisEQP);
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar as cargas");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Salva as cargas NL
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoCARGANL);
         sucesso = SPSNP_wrCargaNL(lisEQP);
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar as cargas NL");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Salva as chaves
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoCHAVE);
         sucesso = SPSNP_wrChave(lisEQP);
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar as chaves");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Salva os filtros
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoFILTRO);
         sucesso = SPSNP_wrFiltro(lisEQP);
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar os filtros");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Salva os geradores
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoGERADOR);
         sucesso = SPSNP_wrGerador(lisEQP);
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar os geradores");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Salva os capacitores
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoCAPACITOR);
         sucesso = SPSNP_wrCapacitor(lisEQP);
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar os capacitores");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Salva os capacitores série
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoCAPSERIE);
         sucesso = SPSNP_wrCapacitorSerie(lisEQP);
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar os capacitores série");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Salva as mútuas
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoMUTUA);
         sucesso = SPSNP_wrMutua(lisEQP);
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar as mútuas");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Salva os reatores
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoREATOR);
         sucesso = SPSNP_wrReator(lisEQP);
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar os reatores");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Salva os reguladores
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoREGULADOR);
         sucesso = SPSNP_wrRegulador(lisEQP);
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar os reguladores");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Salva os suprimentos
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoSUPRIMENTO);
         sucesso = SPSNP_wrSuprimento(lisEQP);
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar os suprimentos");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Salva os trafos
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoTRAFO);
         sucesso = SPSNP_wrTrafo(lisEQP);
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar os transformadores");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Salva os trafos ZZ
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoTRAFOZZ);
         sucesso = SPSNP_wrTrafoZZ(lisEQP);
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar os transformadores Zig-Zag");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Salva os trafos 3E
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoTRAFO3E);
         sucesso = SPSNP_wrTrafo3E(lisEQP);
         if (!sucesso)
            {
            strStatus = _T("Erro ao salvar os transformadores de três enrolamentos");
            lstStrExec->Append(strStatus);
            goto LabelExecAbort;
            }

         // Vincula os equipamentos às respectivas redes
         if ((lisREDES = redes->LisRede()) != NULL)
            {
            for (int i = 0; i < lisREDES->Count; i++)
               {
               if ((rede = (VTRede*)lisREDES->Items[i]) != NULL)
                  {
                  lisEQP->Clear();
                  rede->LisEqpto(lisEQP); // O default é retornar os não isolados
                  sucesso = SPSNP_wrEquipamentoRede(rede, lisEQP);
                  if (!sucesso)
                     {
                     strStatus = _T("Erro ao associar rede a equipamentos.");
                     lstStrExec->Append(strStatus);
                     goto LabelExecAbort;
                     }
                  }
               }
            }

         // Se chegou aqui, sem problemas, vai para o final
         if (sucesso) goto LabelExecOK;

         // Final da execução (com erros)
         LabelExecAbort:
         strStatus = _T("Execução abortada.");
         lstStrExec->Append(strStatus);
         strStatus = UltimoErro();
         lstStrExec->Append(strStatus);

         goto LabelExecEnd;

         // Final da execução (com sucesso)
         LabelExecOK:
         strStatus = _T("Execução finalizada com sucesso.");
         lstStrExec->Append(strStatus);
         Aviso(strStatus);

         // Final da execução
         LabelExecEnd:;
         }
      catch(Exception &e)
         {
         sucesso = false;
         }
      }
   __finally
      {
      if (lstStrExec)  { delete lstStrExec; lstStrExec = NULL;  }
      if (lisEQP)      { delete lisEQP;     lisEQP = NULL;      }
      }

   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::LeTipos(void)
   {
   //variáveis locais
   AnsiString strlog;

   if (!bd_session)               { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION         return(false); }
   if (!bd_session->Connected)    { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON      return(false); }
   if (!arranjos)                 { SNPSTRLOGLINE(strlog); MSGERRBDWR_ARRANJOS       return(false); }
   if (!curvas)                   { SNPSTRLOGLINE(strlog); MSGERRSYS_CURVAINV        return(false); }
   if (!tipos)                    { SNPSTRLOGLINE(strlog); MSGERRBDWR_ARRANJOS       return(false); }

   // Limpa as estruturas existentes
   arranjos->Clear();
   curvas->Clear();
   tipos->Clear();
   
   if (!SPSNP_rdTipoConsumidor()) { SNPSTRLOGLINE(strlog); MSGERRBDRD_TIPOCONSUMIDOR return(false); }
   if (!SPSNP_rdTipoChave())      { SNPSTRLOGLINE(strlog); MSGERRBDRD_TIPOCHAVE      return(false); }
   if (!SPSNP_rdTipoRede())       { SNPSTRLOGLINE(strlog); MSGERRBDRD_TIPOREDE       return(false); }
   if (!LeArranjo())              { SNPSTRLOGLINE(strlog); MSGERRBDRD_ARRANJO        return(false); }
   if (!SPSNP_rdCurvaTipica())    { SNPSTRLOGLINE(strlog); MSGERRBDRD_CURTIPICAS     return(false); }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SalvaTipos(void)
   {
   AnsiString strlog;

   if (!tipos)                                     { SNPSTRLOGLINE(strlog); MSGERRBDWR_ARRANJOS return(false); }
   if (!SalvaArranjos())                           { SNPSTRLOGLINE(strlog); MSGERRBDWR_ARRANJOS return(false); }
   if (!SPSNP_wrCurvaTipica())                     { SNPSTRLOGLINE(strlog); MSGERRBDWR_CURVAS   return(false); }
   if (!SPSNP_wrCurvaPropria())                    { SNPSTRLOGLINE(strlog); MSGERRBDWR_CURVAS   return(false); }
   if (!SPSNP_wrTipoChave(tipos->LisTipoChave()))  { SNPSTRLOGLINE(strlog); MSGERRBDWR_ARRANJOS return(false); }
   if (!SPSNP_wrTipoRede(tipos->LisTipoRede()))    { SNPSTRLOGLINE(strlog); MSGERRBDWR_ARRANJOS return(false); }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::LeNodesFilho(int node_pai_id)
   {
   return(SPSNP_rdRedeNoPai(node_pai_id));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::LeGrupoRedes(DynamicArray<int> vet_rede_id)
   {
   bool       sucesso = true;
   int        intEtapaTotal = 0;
   AnsiString strlog;

   if (!redes)    { SNPSTRLOGLINE(strlog); MSGERRSYS_REDESINV return(false); }

   // Inicia o número de etapas
   intEtapaTotal = 17;
   
   //reinicia cronometros
   if (cronometro) cronometro->Reinicia();
   if (cronometro) cronometro->Start("RedeTotal");
   //exibe barra de progresso
   if (progresso) progresso->Start(progTEXTO);
   if (progresso) progresso->Add("Leitura da base de dados");

   if (sucesso)
      {
      if (cronometro) cronometro->Start("Tipos");
      if (progresso)  progresso->Add("Leitura dos tipos");
      sucesso = LeTipos();
      if (cronometro) cronometro->Stop("Tipos");
      if (! sucesso)
         {
         SNPSTRLOGLINE(strlog);
         return(false);
         }
      }

   if (sucesso)
      {
      redes->Clear();

      if (cronometro) cronometro->Start("Rede");
      if (progresso)  progresso->Add("Leitura das redes");
      if (cronometro) cronometro->Stop("Rede");
      for (int i = 0; i < vet_rede_id.Length; i++)
         {
         sucesso = SPSNP_rdRede(vet_rede_id[i]);
         if (!sucesso)
           break;
         }
      if (! sucesso)
         {
         SNPSTRLOGLINE(strlog);
         return(false);
         }
      }

   return(LeListaRedes(redes->LisRede()));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::LeTodasRedes(void)
   {
bool       sucesso = true;
   int        intEtapaTotal = 0;
   AnsiString strlog;

   if (!redes)    { SNPSTRLOGLINE(strlog); MSGERRSYS_REDESINV return(false); }

   // Inicia o número de etapas
   intEtapaTotal = 17;
   
   //reinicia cronometros
   if (cronometro) cronometro->Reinicia();
   if (cronometro) cronometro->Start("RedeTotal");
   //exibe barra de progresso
   if (progresso) progresso->Start(progTEXTO);
   if (progresso) progresso->Add("Leitura da base de dados");

   if (sucesso)
      {
      if (cronometro) cronometro->Start("Tipos");
      if (progresso)  progresso->Add("Leitura dos tipos");
      sucesso = LeTipos();
      if (cronometro) cronometro->Stop("Tipos");
      if (! sucesso)
         {
         SNPSTRLOGLINE(strlog);
         return(false);
         }
      }

   if (sucesso)
      {
      redes->Clear();

      if (cronometro) cronometro->Start("Rede");
      if (progresso)  progresso->Add("Leitura das redes");
      sucesso = SPSNP_rdRede(IDINDEFINIDO);
      if (cronometro) cronometro->Stop("Rede");
      if (! sucesso)
         {
         SNPSTRLOGLINE(strlog);
         return(false);
         }
      }
   return(LeListaRedes(redes->LisRede()));
   }

//---------------------------------------------------------------------------
// Faz a leitura das redes passadas na lista de redes
bool __fastcall TBDGateway::LeListaRedes(TList* lisRedes)
   {
   bool       sucesso = true;
   int        intEtapa = 0;
   AnsiString strlog;
   VTRede*    rede;

   if (!redes)    { SNPSTRLOGLINE(strlog); MSGERRSYS_REDESINV  return(false); }
   if (!monta)    { SNPSTRLOGLINE(strlog); MSGERRSYS_MONTAINV  return(false); }
   if (!lisRedes) { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV   return(false); }

   // Inicia a contagem de etapas
   intEtapa = 0;

   __try
      {
      try
         {
         // Leitura dos equipamentos de rede por rede
         for (int i = 0; i < lisRedes->Count; i++)
            {
            if ((rede = (VTRede*)lisRedes->Items[i]) == NULL) return(false);
            
            if (sucesso)
               {
               if (cronometro) cronometro->Start("Barra");
               if (progresso)  progresso->Add("Leitura das barras");
               sucesso = SPSNP_rdBarra(rede->Id);
               if (cronometro) cronometro->Stop("Barra");
               if (! sucesso) {SNPSTRLOGLINE(strlog);}
               }
               
            if (sucesso)
               {
               if (progresso) progresso->Add("Leitura dos trechos");
               if (cronometro) cronometro->Start("Trecho");
               if ((sucesso = SPSNP_rdTrecho(rede->Id)) == false) {SNPSTRLOGLINE(strlog);}
                if (cronometro) cronometro->Stop("Trecho");
               }

            if (sucesso)
               {
               if (progresso) progresso->Add("Leitura dos transformadores");
               if (cronometro) cronometro->Start("Trafos");
               if ((sucesso = SPSNP_rdTrafo(rede->Id)) == false) {SNPSTRLOGLINE(strlog);}
               if (cronometro) cronometro->Stop("Trafos");
               }

            if (sucesso)
               {
               if (progresso) progresso->Add("Leitura dos transformadores de três enrolamentos");
               if (cronometro) cronometro->Start("Trafos3E");
               if ((sucesso = SPSNP_rdTrafo3E(rede->Id)) == false) {SNPSTRLOGLINE(strlog);}
               if (cronometro) cronometro->Stop("Trafos3E");
               }

            if (sucesso)
               {
               if (progresso) progresso->Add("Leitura das cargas");
               if (cronometro) cronometro->Start("Cargas");
               if ((sucesso = SPSNP_rdCarga(rede->Id)) == false) {SNPSTRLOGLINE(strlog);}
               if (cronometro) cronometro->Stop("Cargas");
               }

            if (sucesso)
               {
               if (progresso) progresso->Add("Leitura das cargas de iluminação pública");
               if (cronometro) cronometro->Start("CargasIP");
               if ((sucesso = SPSNP_rdCargaIP(rede->Id)) == false) {SNPSTRLOGLINE(strlog);}
               if (cronometro) cronometro->Stop("CargasIP");
               }

            if (sucesso)
               {
               if (progresso) progresso->Add("Leitura das cargas não-lineares");
               if (cronometro) cronometro->Start("CargasNL");
               if ((sucesso = SPSNP_rdCargaNL(rede->Id)) == false) {SNPSTRLOGLINE(strlog);}
               if (cronometro) cronometro->Stop("CargasNL");
               }

            if (sucesso)
               {
               if (progresso) progresso->Add("Leitura dos capacitores");
               if (cronometro) cronometro->Start("Capacitores");
               if ((sucesso = SPSNP_rdCapacitor(rede->Id)) == false) {SNPSTRLOGLINE(strlog);}
               if (cronometro) cronometro->Stop("Capacitores");
               }

            if (sucesso)
               {
               if (progresso) progresso->Add("Leitura dos geradores");
               if (cronometro) cronometro->Start("Geradores");
               if ((sucesso = SPSNP_rdGerador(rede->Id)) == false) {SNPSTRLOGLINE(strlog);}
               if (cronometro) cronometro->Stop("Geradores");
               }

            if (sucesso)
               {
               if (progresso) progresso->Add("Leitura dos suprimentos");
               if (cronometro) cronometro->Start("Suprimentos");
               if ((sucesso = SPSNP_rdSuprimento(rede->Id)) == false) {SNPSTRLOGLINE(strlog);}
               if (cronometro) cronometro->Stop("Suprimentos");
               }

            if (sucesso)
               {
               if (progresso) progresso->Add("Leitura dos filtros");
               if (cronometro) cronometro->Start("Filtros");
               if ((sucesso = SPSNP_rdFiltro(rede->Id)) == false) {SNPSTRLOGLINE(strlog);}
               if (cronometro) cronometro->Stop("Filtros");
               }

            if (sucesso)
               {
               if (progresso) progresso->Add("Leitura dos reatores");
               if (cronometro) cronometro->Start("Reatores");
               if ((sucesso = SPSNP_rdReator(rede->Id)) == false) {SNPSTRLOGLINE(strlog);}
               if (cronometro) cronometro->Stop("Reatores");
               }

            if (sucesso)
               {
               if (progresso) progresso->Add("Leitura dos transformadores zig zag");
               if (cronometro) cronometro->Start("TrafosZZ");
               if ((sucesso = SPSNP_rdTrafoZZ(rede->Id)) == false) {SNPSTRLOGLINE(strlog);}
               if (cronometro) cronometro->Stop("TrafosZZ");
               }

            if (sucesso)
               {
               if (progresso) progresso->Add("Leitura dos capacitores série");
               if (cronometro) cronometro->Start("CapacitoresSerie");
               if ((sucesso = SPSNP_rdCapacitorSerie(rede->Id)) == false) {SNPSTRLOGLINE(strlog);}
               if (cronometro) cronometro->Stop("CapacitoresSerie");
               }

            if (sucesso)
               {
               if (progresso) progresso->Add("Leitura das impedâncias mútuas");
               if (cronometro) cronometro->Start("Mutuas");
               if ((sucesso = SPSNP_rdMutua(rede->Id)) == false) {SNPSTRLOGLINE(strlog);}
               if (cronometro) cronometro->Stop("Mutuas");
               }
      
            if (sucesso)
               {
               if (progresso) progresso->Add("Leitura das chaves");
               if (cronometro) cronometro->Start("Chaves");
               if ((sucesso = SPSNP_rdChave(rede->Id)) == false) {SNPSTRLOGLINE(strlog);}
               if (cronometro) cronometro->Stop("Chaves");
               }
      
            if (sucesso)
               {
               if (progresso) progresso->Add("Leitura dos reguladores");
               if (cronometro) cronometro->Start("Reguladores");
               if ((sucesso = SPSNP_rdRegulador(rede->Id)) == false) {SNPSTRLOGLINE(strlog);}
               if (cronometro) cronometro->Stop("Reguladores");
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
      //destrói objetos locais
      }

   //finaliza montagem da rede
   if (!monta->TrataFimDeDados(true))            { SNPSTRLOGLINE(strlog); MSGERRSYS_TRATAFIMDADOS      return(false); }

   Aviso("Leitura dos dados concluída.");
   return(true);
   }
   
//---------------------------------------------------------------------------
SNPSTR __fastcall TBDGateway::UltimoErro(void)
   {
   return(strUltimoErro);
   }

//---------------------------------------------------------------------------
void TBDGateway::TrataOnArrayError(TOracleQuery* Sender, int Index, int ErrorCode, const System::UnicodeString ErrorMessage, bool& Continue)
   {
   SNPSTR straux;

   // Mesmo após contato com a Allround automations, continua ocorrendo Access Violation após a execução.
   // O cabeçalho utilizado é o mesmo que foi suge
   // Está gerando Access Violation. O cabeçalho deve estar errado. SNPSTR ou AnsiString dá na mesma

   // straux = " Erro BDGateway. Índice = " + IntToStr(intIndex) + ", ErrorCode = " + IntToStr(intErrorCode) + "( " + strErrorMessage + " )";
   straux = " Erro BDGateway. Índice = " + IntToStr(Index) + ", ErrorCode = " + IntToStr(ErrorCode);
   MessageDlg(straux, mtError, mbOKCancel, 0);
   }

//---------------------------------------------------------------------------
TOracleSession* __fastcall TBDGateway::IniciaConexaoOracle(AnsiString strTNS, AnsiString strLoginName, AnsiString strLoginPwd)
   {
   //variáveis locais
   TOracleSession* ptrbd_session;

   try
      {
      // If you want to retrieve UTF8 data as WE8MSWIN1252 you can set the
      // global NoUnicodeSupport Boolean variable in the Oracle unit to True.
      // !OOPS! Isso aqui deveria se tornar um parâmetro do Sinap para futuros problemas
      // Precisa ver se funciona corretamente na Sinapsis
      NoUnicodeSupport = true;

      ptrbd_session = new TOracleSession(NULL);
      ptrbd_session->LogonUsername = strLoginName; // "SINAPADM";
      ptrbd_session->LogonPassword = strLoginPwd;  // "sinapadmpwd";
      ptrbd_session->LogonDatabase = strTNS;       // Sinapsis: SNPORA_SINAPSIS, Local: SNPORA
      ptrbd_session->LogOn();
      }
   catch(EOracleError &e)
      {
      if (e.ErrorCode == 0)
         {
         ShowMessage("Erro na conexão com o Oracle. Verifique a instalação do driver e sua configuração.");
         }
      return(NULL);
      }
   catch(Exception &e)
      {
      return(NULL);
      }

   return(ptrbd_session);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::PM_GetConectado(void)
   {
   //verifica se está conectado com Oracle
   if (bd_session)  { return(bd_session->Connected); }


   TOracleDirectPathLoader* dpl = new TOracleDirectPathLoader(NULL);
   dpl->Session = bd_session;
   dpl->TableName = "";

   delete dpl;

   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::Rollback(void)
   {
   if (bd_session)
      {
      bd_session->Rollback();
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::ArrayVarCria(TList* lstParam, int intMaxItems)
   {
   int vecBounds[2] = { 0, 0 };
   struct TBDGatewayProcParam* dbParam = NULL;

   if (lstParam == NULL)     return(false);
   if (lstParam->Count <= 0) return(false);
   if (intMaxItems <= 0)     return(false);

   // Inicialmente, limpa o que existe
   if (!ArrayVarClean(lstParam)) return(false);

   // Determina as fronteiras dos arrays de dados
   // Cria arrays "sem profundidade", pois os tamanhos dos arrays serão ajustados
   // a cada rodada. A carga tem de ser feita em blocos, para evitar mensagens "Out Of Memory"
   vecBounds[0] = 0;
   vecBounds[1] = intMaxItems;

   // Cria todos os vetores de variants e os armazena no vetor passado
   // como parâmetro
   for (int i = 0; i < lstParam->Count; i++)
      {
      dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[i]);
      dbParam->arrVar = VarArrayCreate(vecBounds, 1, dbParam->varType);
      }

   return(true);
   }

//---------------------------------------------------------------------------
// Deve ser passado o índice máximo.
// PAra 10 elementos, deve ser passado 9. Para um elemento, deve ser passado
// ZERO
bool __fastcall TBDGateway::ArrayVarRedim(TList* lstParam, int intNdxMax)
   {
   int intBoundHighCur;
   int intBoundLowCur;
   struct TBDGatewayProcParam* dbParam = NULL;

   if (lstParam == NULL)     return(false);
   if (lstParam->Count <= 0) return(false);
   if (intNdxMax < 0)        return(false);

   // Reajusta o tamanho do vetor de arrats, de acordo com o total de itens
   // passado como parâmetro

   // Somente redimensiona se tiver mudado o tamanho
   // Um teste basta, pois todos os arrays devem ter o mesmo tamanho.
   // Por isso, está tomando vecParam[0] como base
   dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
   intBoundHighCur = VarArrayHighBound(dbParam->arrVar, 1);
   intBoundLowCur  = VarArrayLowBound(dbParam->arrVar, 1);
   if (intBoundLowCur != 0) return(false);
   // Se o último índice é diferente, então troca
   if (intBoundHighCur != intNdxMax)
      {
      for (int i = 0; i < lstParam->Count; i++)
         {
         dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[i]);
         VarArrayRedim(dbParam->arrVar, intNdxMax);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::ArrayVarDOADeclVar(TList* lstParam, TOracleQuery* ptrQry)
   {
   struct TBDGatewayProcParam* dbParam = NULL;
   
   // Verificações iniciais
   if (lstParam == NULL)     return(false);
   if (lstParam->Count <= 0) return(false);
   if (!ptrQry)              return(false);

   for (int i = 0; i < lstParam->Count; i++)
      {
      dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[i]);

      ptrQry->DeclareVariable(dbParam->strORAname, dbParam->doaType);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::ArrayVarClean(TList* lstParam)
   {
   struct TBDGatewayProcParam* dbParam = NULL;
   
   // Verificações iniciais
   if (lstParam == NULL)     return(false);
   if (lstParam->Count <= 0) return(false);

   for (int i = 0; i < lstParam->Count; i++)
      {
      dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[i]);
      if (dbParam == NULL) return(false);
      
      VarClear(dbParam->arrVar);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdTipoRede(void)
   {
   //variáveis locais
   bool          sucesso = true;
   int           tipo_rede_id;
   strTIPO_REDE  str_tipo_rede;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!monta)                      { SNPSTRLOGLINE(strlog); MSGERRSYS_MONTAINV   return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdTipoRede(:p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor"  , otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         // Seta valores dos parâmetros
         // Não há parâmetros a serem configurados

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
bool __fastcall TBDGateway::SPSNP_rdTipoConsumidor(void)
   {
   //variáveis locais
   bool          sucesso = true;
   VTClasse*     classe;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!classes)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_CLASSESINV return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdTipoConsumidor(:p_cursor);");
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
bool __fastcall TBDGateway::SPSNP_rdTipoChave(void)
   {
   //variáveis locais
   bool          sucesso = true;
   int           tipochave_id;
   AnsiString    strlog;
   VTTipoChave*  tipochave;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdTipoChave(:p_cursor);");
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
bool __fastcall TBDGateway::SPSNP_rdRede(int intTipoRedeId)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   strREDE       str_rede;
   strCLUSTER    str_cluster;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdRede(:p_TipoRedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_TipoRedeId", otInteger);

         ptrQry->SetVariable("p_TipoRedeId", intTipoRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();

         if (!SPSNP_rdRedeCursor(ptrCursor))
            {
            SNPSTRLOGLINE(strlog);
            sucesso = false;
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
   } // SPSNP_rdRede

//---------------------------------------------------------------------------
// Função que decodifica o cursor retornado pelas diversas funções de leitura
// de rede.
bool __fastcall TBDGateway::SPSNP_rdRedeCursor(TOracleQuery* ptrCursor, TList *lisRedeAux)
   {
   //variáveis locais
   bool          sucesso = true;
   VTRede*       rede = NULL;
   AnsiString    strlog;
   strREDE       str_rede;
   strCLUSTER    str_cluster;
   VTCluster*    cluster;

   if (!ptrCursor)                  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   try
      {
      while (!ptrCursor->Eof)
         {
         // loop registros (begin)
         // preenche estrutura strREDE
         str_rede.rede_id            = ptrCursor->Field("RedeId");
         str_rede.codigo             = ptrCursor->Field("Codigo");
         str_rede.extern_id          = ptrCursor->Field("ExtId");
         str_rede.tiporede.id        = ptrCursor->Field("TipoRedeId");
         str_rede.tiporede.codigo    = ptrCursor->Field("TipoRedeCodigo");
         str_rede.mrede_id           = ptrCursor->Field("NoId");
         str_rede.radial             = bool(int(ptrCursor->Field("Radial")));
         str_rede.barini_id          = ptrCursor->Field("BarraIniId");
         str_rede.tiporede.segmento  = 0;
         str_rede.mrede_redutivel    = false;
         str_rede.estatica           = false;
         str_rede.equivalente        = false;
         str_rede.cluster_id         = 0;
         str_rede.obra               = false;
         str_rede.area_esq.x1        = 0;
         str_rede.area_esq.y1        = 0;
         str_rede.area_esq.x2        = 0;
         str_rede.area_esq.y2        = 0;
         // OOPS
         str_rede.color              = clMoneyGreen;
         str_rede.dominio_id         = 0;
         str_cluster.cluster_id      = 0;
         str_cluster.obra            = 0;
         // Comentário do equipamento
         str_rede.comentario.dx         = 0;
         str_rede.comentario.dy         = 0;
         str_rede.comentario.comentario = "";

         //insere em Monta
         if ((rede = (VTRede*)monta->InsereRede(str_rede)) == NULL)
            {
            SNPSTRLOGLINE(strlog);
            SNPSTRLOG(str_rede.codigo);
            MSGERRSYS_REDEINV
            sucesso = false;
            break;
            }
         else
            {
            // OOPS: Precisa disso?
            bd_func->DefineStatusEqpto(rede, true);
            if (lisRedeAux) lisRedeAux->Add(rede);
            }

         ptrCursor->Next();
         // loop registros (end)
         }
      }
   catch (Exception& e)
      {
      SNPSTRLOGLINE(strlog);
      SNPSTRLOG(e.Message);
      rede = NULL;
      sucesso = false;
      }

   return(sucesso);
   } // SPSNP_rdRedeCursor

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdRedeNoPai(int intNoPaiId)
   {
   //variáveis locais
   bool          sucesso = true;
   VTBarra*      barra;
   AnsiString    strlog;
   strBARRA      str_barra;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;
   strNODE       str;

   VTMontaArvore *monta = (VTMontaArvore*)apl->GetObject(__classid(VTMontaArvore));

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!monta)                      { SNPSTRLOGLINE(strlog); MSGERRSYS_MONTAINV   return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdRedeNoPai(:p_NoPai, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_NoPai", otInteger);

         ptrQry->SetVariable("p_NoPai", intNoPaiId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();

         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str.node_id     = ptrCursor->Field("NoId");
            str.node_pai_id = ptrCursor->Field("NoPaiId");
            str.obj_id      = ptrCursor->Field("RedeId");
            str.obj_codigo  = ptrCursor->Field("Codigo");

            monta->InsereNode(str);

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
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdBarra(int intRedeId)
   {
   //variáveis locais
   bool          sucesso = true;
   VTBarra*      barra;
   AnsiString    strlog;
   strBARRA      str_barra;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdBarra(:p_RedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_RedeId"  , otInteger);

         ptrQry->SetVariable("p_RedeId"  , intRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();

         if (!SPSNP_rdBarraCursor(ptrCursor))
            {
            SNPSTRLOGLINE(strlog);
            sucesso = false;
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
   } // SPSNP_rdBarra

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdBarraCursor(TOracleQuery* ptrCursor)
   {
   //variáveis locais
   bool          sucesso = true;
   VTBarra*      barra;
   AnsiString    strlog;
   strBARRA      str_barra;

   if (!ptrCursor)                  { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   try{//inicia controle de inserção de Barras de VTMOnta
      monta->InsereBarraStart();
      while (!ptrCursor->Eof)
         {
         // loop registros (begin)
         // preenche estrutura strBARRA
         str_barra.bar_id    = ptrCursor->Field("BarraId");
         str_barra.codigo    = ptrCursor->Field("Codigo");
         str_barra.extern_id = ptrCursor->Field("ExtId");
         str_barra.vnom      = ptrCursor->Field("VNom");
         str_barra.utm.x     = ptrCursor->Field("UtmX");
         str_barra.utm.y     = ptrCursor->Field("UtmY");
         str_barra.esq.x1    = 0;
         str_barra.esq.y1    = 0;
         str_barra.esq.x2    = 0;
         str_barra.esq.y2    = 0;
         str_barra.obra      = false;

         // Comentário do equipamento
         str_barra.comentario.dx         = 0;
         str_barra.comentario.dy         = 0;
         str_barra.comentario.comentario = "";

         //insere em Monta
         if ((barra = (VTBarra*)monta->InsereBarraEqpto(str_barra)) == NULL)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_BARRAINV
            sucesso = false;
            break;
            }
         else
            {
            bd_func->DefineStatusEqpto(barra, true);
            }

         ptrCursor->Next();
         // loop registros (end)
         }
      //finaliza controle de inserção de Barras de VTMOnta
      monta->InsereBarraStop();
      }
   catch (Exception& e)
      {
      SNPSTRLOGLINE(strlog);
      SNPSTRLOG(e.Message);
      sucesso = false;
      }

   return(sucesso);
   } // SPSNP_rdBarraCursor

//---------------------------------------------------------------------------
VTCabo* __fastcall TBDGateway::SPSNP_rdCabo(int intCaboId)
   {
   //variáveis locais
   VTCabo*       cabo = NULL;
   AnsiString    strlog;
   strCABO       str_cabo;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(NULL); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdCabo(:p_CaboId, :p_cursor);");
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
VTSuporte* __fastcall TBDGateway::SPSNP_rdSuporte(int intSuporteId)
   {
   //variáveis locais
   VTSuporte*    suporte = NULL;
   AnsiString    strlog;
   strSUPORTE    str_suporte;
   int           intTipoSuporteId;
   int           intPontosTotal;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(NULL); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdSuporte(:p_SuporteId, :p_cursor);");
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
   } // SPSNP_rdSuporte(int intSuporteId)

//---------------------------------------------------------------------------
int __fastcall TBDGateway::SPSNP_rdSuporteUtilizado(int intSuporteId)
   {
   //variáveis locais
   AnsiString    strlog;
   int           intNumVezes;
   TOracleQuery* ptrQry = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(0); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(0); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdSuporteUtilizado(:p_SuporteId, :p_NumVezes);");
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
bool __fastcall TBDGateway::SPSNP_rdSuporteUtilizado(int intSuporteId, TStringList* arrlist)
   {
   //variáveis locais
   bool          bolret = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;
   int           intArranjoId;
   AnsiString    strArranjoDescr;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }

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
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdSuporteUtilizado(:p_SuporteId, :p_cursor);");
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
bool __fastcall TBDGateway::LeArranjo(void)
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
VTArranjo* __fastcall TBDGateway::LeArranjo(int intArranjoId, int intTipoArranjoId, bool bolTipico)
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
VTArranjo* __fastcall TBDGateway::SPSNP_rdArranjo_00_01(int intArranjoId)
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

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(NULL); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdArranjo_00_01(:p_ArranjoId, :p_cursor);");
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
VTArranjo* __fastcall TBDGateway::SPSNP_rdArranjo_02_04(int intArranjoId)
   {
   //variáveis locais
   VTArranjo*    arranjo = NULL;
   AnsiString    strlog;
   strARRANJO2   str_arranjo2;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(NULL); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdArranjo_02_04(:p_ArranjoId, :p_cursor);");
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
VTArranjo* __fastcall TBDGateway::SPSNP_rdArranjo_03(int intArranjoId)
   {
   //variáveis locais
   VTArranjo*    arranjo = NULL;
   AnsiString    strlog;
   strARRANJO2   str_arranjo2;
   TStream*      ptrStream = NULL;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(NULL); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(NULL); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(NULL); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdArranjo_03(:p_ArranjoId, :p_cursor);");
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
bool __fastcall TBDGateway::SPSNP_rdCapacitorSerie(int intRedeId)
   {
   //variáveis locais
   bool          sucesso = true;
   VTCapserie*   capserie = NULL;
   AnsiString    strlog;
   strCAPSERIE   str_capserie;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdCapacitorSerie(:p_RedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_RedeId", otInteger);

         ptrQry->SetVariable("p_RedeId", intRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();

         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_capserie.id          = ptrCursor->Field("CapacitorSerieId");
            str_capserie.rede_id     = ptrCursor->Field("RedeId");
            str_capserie.codigo      = ptrCursor->Field("Codigo");
            str_capserie.extern_id   = ptrCursor->Field("ExtId");

            str_capserie.vnom        = ptrCursor->Field("Vnom");
            str_capserie.snom        = ptrCursor->Field("Snom");
            str_capserie.z.r         = ptrCursor->Field("ZR");
            str_capserie.z.x         = ptrCursor->Field("ZX");
            str_capserie.obra        = false;

            str_capserie.bar1_id     = ptrCursor->Field("Barra1");
            str_capserie.esq.posbar1 = 0;
            str_capserie.bar2_id     = ptrCursor->Field("Barra2");
            str_capserie.esq.posbar2 = 0;

            // Comentário do equipamento
            str_capserie.comentario.dx         = 0;
            str_capserie.comentario.dy         = 0;
            str_capserie.comentario.comentario = "";

            if ((capserie = (VTCapserie*)monta->InsereCapserie(str_capserie)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_CAPSERIEINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(capserie, true);
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
   } // SPSNP_rdCapacitorSerie


//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdTrecho(int intRedeId)
   {
   //variáveis locais
   bool          sucesso = true;
   VTTrecho*     trecho = NULL;
   AnsiString    strlog;
   strTRECHO     str_trecho;
   VTArranjo*    arranjo;
   int           intArranjoId;
   int           intTipoArranjoId;
   bool          bolTipico;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         ptrQry->ReadBuffer    = ORACLE_DRIVER_BUFFER_SIZE;
         ptrCursor->ReadBuffer = ORACLE_DRIVER_BUFFER_SIZE;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdTrecho(:p_RedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_RedeId", otInteger);

         ptrQry->SetVariable("p_RedeId", intRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_trecho.tre_id      = ptrCursor->Field("TrechoId");
            str_trecho.rede_id     = ptrCursor->Field("RedeId");
            str_trecho.codigo      = ptrCursor->Field("Codigo");
            str_trecho.extern_id   = ptrCursor->Field("ExtId");
            str_trecho.comp_km     = ptrCursor->Field("Comprimento");
            str_trecho.obra        = false;

            str_trecho.bar1_id     = ptrCursor->Field("Barra1");
            str_trecho.esq.posbar1 = 0;
            str_trecho.bar2_id     = ptrCursor->Field("Barra2");
            str_trecho.esq.posbar2 = 0;

            // Comentário do equipamento
            str_trecho.comentario.dx         = 0;
            str_trecho.comentario.dy         = 0;
            str_trecho.comentario.comentario = "";

            intArranjoId           = ptrCursor->Field("ArranjoId");
            intTipoArranjoId       = ptrCursor->Field("TipoArranjoId");
            bolTipico              = bool(int(ptrCursor->Field("Tipico")));
            //insere em Monta
            if (cronometro) cronometro->Start("TrechoArranjo");
            if ((arranjo = LeArranjo(intArranjoId, intTipoArranjoId, bolTipico)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_ARRANJOINV
               sucesso = false;
               break;
               }
            if (cronometro) cronometro->Stop("TrechoArranjo");
            str_trecho.fases = arranjo->Fases;
            if (cronometro) cronometro->Start("TrechoInsere");
            if ((trecho = (VTTrecho*)monta->InsereTrecho(str_trecho, arranjo)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_TRECHOINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(trecho, true);
               }
            if (cronometro) cronometro->Stop("TrechoInsere");

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
   } // SPSNP_rdTrecho

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdCarga(int intRedeId)
   {
   //variáveis locais
   bool          sucesso = true;
   VTCarga*      carga = NULL;
   AnsiString    strlog;
   strCARGA      str_carga;
   TStream*      ptrStream;
   strCURHEADER  curhdr;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;
   bool          bolTipica;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         ptrQry->ReadBuffer    = ORACLE_DRIVER_BUFFER_SIZE;
         ptrCursor->ReadBuffer = ORACLE_DRIVER_BUFFER_SIZE;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdCarga(:p_RedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_RedeId", otInteger);

         ptrQry->SetVariable("p_RedeId", intRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_carga.car_id               = ptrCursor->Field("CargaId");
            str_carga.codigo               = ptrCursor->Field("Codigo");
            str_carga.extern_id            = ptrCursor->Field("ExtId");
            str_carga.fases                = ptrCursor->Field("TipoFaseId");
            str_carga.energia_mwhmes       = ptrCursor->Field("Energia");
            str_carga.classe_cons          = ptrCursor->Field("TipoConsTag");
            str_carga.obra                 = false;
            str_carga.modelo.icte_pu       = ptrCursor->Field("ICtePu");
            str_carga.modelo.zcte_pu       = ptrCursor->Field("ZCtePu");
            str_carga.modelo.scte_pu       = ptrCursor->Field("SCtePu");
            str_carga.num_cons.comercial   = ptrCursor->Field("NumConsCom");
            str_carga.num_cons.residencial = ptrCursor->Field("NumConsRes");
            str_carga.num_cons.industrial  = ptrCursor->Field("NumConsInd");
            str_carga.num_cons.rural       = ptrCursor->Field("NumConsRur");
            str_carga.num_cons.outros      = ptrCursor->Field("NumConsOut");
            str_carga.num_cons.a4          = ptrCursor->Field("NumConsA4");
            //zera dados de IP
            str_carga.ip.pot_kw            = 0;
            str_carga.ip.fat_pot           = 0;
            str_carga.ip.num_lamp          = 0;
            //
            str_carga.bar_id               = ptrCursor->Field("Barra1");
            str_carga.esq.posbar           = 0;
            str_carga.esq.dxbar            = 0;
            str_carga.esq.dybar            = 0;

            bolTipica                      = bool(int(ptrCursor->Field("Tipica")));
            if (!bolTipica)
               {
               ptrStream = ptrCursor->LOBField("Pontos");
               if (!ptrStream)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CURVAINV
                  sucesso = false;
                  break;
                  }
               if (!bd_func->DesmontaStreamCurvaEquipamento(ptrStream, &(str_carga.curva), curhdr))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CURVAINV
                  sucesso = false;
                  break;
                  }
               // O LOBField limpa a stream
               // delete ptrStream;
               //IMPORTANTE: curva própria da Carga possui classe_id = IDINDEFINIDO
               //str_carga.classe_cons = (int)curhdr.uchClasseId;
               }
            // Ao desmontar o BLOB, o header da curva é zerado. Assim, os campos devem
            // ser preenchidos posteriormente
            str_carga.curva.tipica = bolTipica;
            str_carga.curva.status = 0;
            str_carga.curva.id     = ptrCursor->Field("CurvaId");

            // Comentário do equipamento
            str_carga.comentario.dx         = 0;
            str_carga.comentario.dy         = 0;
            str_carga.comentario.comentario = "";

            //insere em Monta
            if ((carga = (VTCarga*)monta->InsereCarga(str_carga)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_CARGAINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(carga, true);
               if (carga->curva) { carga->curva->Status[sttNOVO] = true; }
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
   } // SPSNP_rdCarga

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdCargaNL(int intRedeId)
   {
   //variáveis locais
   bool          sucesso = true;
   VTCargaNL*    cargaNL = NULL;
   AnsiString    strlog;
   strCARGANL    str_cargaNL;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdCargaNL(:p_RedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_RedeId", otInteger);

         ptrQry->SetVariable("p_RedeId", intRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_cargaNL.car_id                = ptrCursor->Field("cargaNLId");
            str_cargaNL.codigo                = ptrCursor->Field("Codigo");
            str_cargaNL.extern_id             = ptrCursor->Field("ExtId");
            str_cargaNL.obra                  = false;
            str_cargaNL.bar_id                = ptrCursor->Field("Barra1");
            str_cargaNL.esq.posbar            = 0;
            str_cargaNL.esq.dxbar             = 0;
            str_cargaNL.esq.dybar             = 0;
            str_cargaNL.ordem                 = ptrCursor->Field("OrdemId");
            str_cargaNL.fat_pot               = ptrCursor->Field("FatorPot");
            str_cargaNL.flag_indutivo         = bool(int(ptrCursor->Field("Indutivo")));
            str_cargaNL.flag_kVA              = bool(int(ptrCursor->Field("kVA")));
            str_cargaNL.snomA                 = ptrCursor->Field("SNomA");
            str_cargaNL.snomB                 = ptrCursor->Field("SNomB");
            str_cargaNL.snomC                 = ptrCursor->Field("SNomC");
            str_cargaNL.angA                  = ptrCursor->Field("AngA");
            str_cargaNL.angB                  = ptrCursor->Field("AngB");
            str_cargaNL.angC                  = ptrCursor->Field("AngC");

            // Comentário do equipamento
            str_cargaNL.comentario.dx         = 0;
            str_cargaNL.comentario.dy         = 0;
            str_cargaNL.comentario.comentario = "";

            //insere em Monta
            // O monta deve saber quem é a fundamental e inserir uma nova carga
            // somente nesse caso. Para as harmônicas, retorna a mesma carga pre-
            // viamente inserida.
            if ((cargaNL = (VTCargaNL*)monta->InsereCargaNL(str_cargaNL)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_CARGAINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(cargaNL, true);
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
   } // SPSNP_rdCargaNL

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdCargaIP(int intRedeId)
   {
   //variáveis locais
   bool          sucesso = true;
   unsigned      modelo_num_lamp;
   VTCarga*      carga = NULL;
   AnsiString    strlog;
   strCARGA      str_carga;
   TStream*      ptrStream;
   strCURHEADER  curhdr;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;
   bool          bolTipica;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdCargaIP(:p_RedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_RedeId", otInteger);

         ptrQry->SetVariable("p_RedeId", intRedeId);
 
         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_carga.car_id               = ptrCursor->Field("CargaIPId");
            str_carga.codigo               = ptrCursor->Field("Codigo");
            str_carga.extern_id            = ptrCursor->Field("ExtId");
            str_carga.fases                = ptrCursor->Field("TipoFaseId");
            str_carga.bar_id               = ptrCursor->Field("Barra1");
            str_carga.esq.posbar           = 0;
            str_carga.esq.dxbar            = 0;
            str_carga.esq.dybar            = 0;
            str_carga.ip.pot_kw            = ptrCursor->Field("Pnom");
            str_carga.ip.fat_pot           = ptrCursor->Field("FatPot");
            //str_carga.ip.num_lamp          = ptrCursor->Field("NumLamp");
            str_carga.obra                 = false;
            //lê campo com informação composta: modelo + número de lâmpadas
            modelo_num_lamp                = ptrCursor->Field("NumLamp");
            str_carga.ip.num_lamp          = modelo_num_lamp & 0xffff;
            //define modelo de carga
            str_carga.modelo.icte_pu       = ptrCursor->Field("ICtePu");
            str_carga.modelo.zcte_pu       = ptrCursor->Field("ZCtePu");
            str_carga.modelo.scte_pu       = ptrCursor->Field("SCtePu");
            switch((modelo_num_lamp >> 16)& 0xf)
               {
               case cargaICTE: str_carga.modelo.icte_pu = 1; break;
               case cargaSCTE: str_carga.modelo.scte_pu = 1; break;
               case cargaZCTE: str_carga.modelo.zcte_pu = 1; break;
               default:        str_carga.modelo.scte_pu = 1; break;
               }
            str_carga.num_cons.comercial   = 0;
            str_carga.num_cons.residencial = 0;
            str_carga.num_cons.industrial  = 0;
            str_carga.num_cons.rural       = 0;
            str_carga.num_cons.outros      = 0;
            str_carga.num_cons.a4          = 0;
            str_carga.energia_mwhmes       = 0;
            str_carga.classe_cons          = consIP;
            bolTipica                      = bool(int(ptrCursor->Field("Tipica")));
            if (!bolTipica)
               {
               ptrStream = ptrCursor->LOBField("Pontos");
               if (!ptrStream)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CURVAINV
                  sucesso = false;
                  break;
                  }
               if (!bd_func->DesmontaStreamCurvaEquipamento(ptrStream, &(str_carga.curva), curhdr))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CURVAINV
                  sucesso = false;
                  break;
                  }
               // O LOBField limpa a stream
               //delete ptrStream;

               str_carga.classe_cons = (int)curhdr.uchClasseId;
               }
            // Ao desmontar o BLOB, o header da curva é zerado. Assim, os campos devem
            // ser preenchidos posteriormente
            str_carga.curva.tipica = bolTipica;
            str_carga.curva.status = 0; // OOPS
            str_carga.curva.id     = ptrCursor->Field("CurvaId");

            // Comentário do equipamento
            str_carga.comentario.dx         = 0;
            str_carga.comentario.dy         = 0;
            str_carga.comentario.comentario = "";

            //insere em Monta
            if ((carga = (VTCarga*)monta->InsereCarga(str_carga)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_CARGAINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(carga, true);
               if (carga->curva) { carga->curva->Status[sttNOVO] = true; }
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
   } // SPSNP_rdCargaIP

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdCapacitor(int intRedeId)
   {
   //variáveis locais
   bool          sucesso = true;
   VTCapacitor*  capacitor = NULL;
   AnsiString    strlog;
   strCAPACITOR  str_capacitor;
   TStream*      ptrStream;
   strCURHEADER  curhdr;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;
   bool          bolTipica;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdCapacitor(:p_RedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_RedeId", otInteger);

         ptrQry->SetVariable("p_RedeId", intRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_capacitor.cap_id       = ptrCursor->Field("CapacitorId");
            str_capacitor.codigo       = ptrCursor->Field("Codigo");
            str_capacitor.extern_id    = ptrCursor->Field("ExtId");
            str_capacitor.ligacao      = ptrCursor->Field("TipoLigacaoId");
            str_capacitor.q_mvar       = ptrCursor->Field("Qnom");
            str_capacitor.vnom         = ptrCursor->Field("Vnom");
            str_capacitor.obra         = false;

            str_capacitor.bar_id       = ptrCursor->Field("Barra1");
            str_capacitor.esq.posbar   = 0;
            str_capacitor.esq.dxbar    = 0;
            str_capacitor.esq.dybar    = 0;

            bolTipica                  = bool(int(ptrCursor->Field("Tipica")));
            if (!bolTipica)
               {
               ptrStream = ptrCursor->LOBField("Pontos");
               if (!ptrStream)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CURVAINV
                  sucesso = false;
                  break;
                  }
               if (!bd_func->DesmontaStreamCurvaEquipamento(ptrStream, &(str_capacitor.curva), curhdr))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CURVAINV
                  sucesso = false;
                  break;
                  }
               // O LOBField limpa a stream
               //delete ptrStream;
               }
            // Ao desmontar o BLOB, o header da curva é zerado. Assim, os campos devem
            // ser preenchidos posteriormente
            str_capacitor.curva.tipica = bolTipica;
            str_capacitor.curva.status = 0; // OOPS
            str_capacitor.curva.id     = ptrCursor->Field("CurvaId");

            // Comentário do equipamento
            str_capacitor.comentario.dx         = 0;
            str_capacitor.comentario.dy         = 0;
            str_capacitor.comentario.comentario = "";

            //insere em Monta
            if ((capacitor = (VTCapacitor*)monta->InsereCapacitor(str_capacitor)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_CAPACITORINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(capacitor, true);
               if (capacitor->curva) { capacitor->curva->Status[sttNOVO] = true; }
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
   } // SPSNP_rdCapacitor


//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdChave(int intRedeId)
   {
   //variáveis locais
   bool          sucesso = true;
   VTChave*      chave = NULL;
   AnsiString    strlog;
   strCHAVE      str_chave;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdChave(:p_RedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->SetVariable("p_RedeId", intRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_chave.chv_id       = ptrCursor->Field("ChaveId");
            str_chave.rede_id      = ptrCursor->Field("RedeId");
            str_chave.codigo       = ptrCursor->Field("Codigo");
            str_chave.extern_id    = ptrCursor->Field("ExtId");
            str_chave.tipochave.id = ptrCursor->Field("TipoChaveId");
            str_chave.estado       = ptrCursor->Field("Estado");
            str_chave.corn         = ptrCursor->Field("Inom");
            str_chave.obra         = false;

            str_chave.bar1_id      = ptrCursor->Field("Barra1");
            str_chave.esq.posbar1  = 0;

            // Comentário do equipamento
            str_chave.comentario.dx         = 0;
            str_chave.comentario.dy         = 0;
            str_chave.comentario.comentario = "";
            // OOPS estruturas VTMonta
            // str_chave.Telecomando           = (bool)ptrCursor->Field("Telecomando");
            // str_chave.Protecao              = (bool)ptrCursor->Field("Protecao");
            // str_chave.TecnoCom              = ptrCursor->Field("TecnoCom");

            str_chave.bar2_id     = ptrCursor->Field("Barra2");
            str_chave.esq.posbar2 = 0;

            if ((chave = (VTChave*)monta->InsereChave(str_chave)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_CHAVEINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(chave, true);
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
   } // SPSNP_rdChave

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdFiltro(int intRedeId)
   {
   //variáveis locais
   bool          sucesso = true;
   VTFiltro*     filtro = NULL;
   AnsiString    strlog;
   strFILTRO     str_filtro;
   TStream*      ptrStream;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdFiltro(:p_RedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->SetVariable("p_RedeId", intRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_filtro.id         = ptrCursor->Field("FiltroId");
            str_filtro.codigo     = ptrCursor->Field("Codigo");
            str_filtro.extern_id  = ptrCursor->Field("ExtId");
            str_filtro.vnom       = ptrCursor->Field("Vnom");
            str_filtro.snom       = ptrCursor->Field("Snom");
            str_filtro.z0.r       = ptrCursor->Field("Z0R");
            str_filtro.z0.x       = ptrCursor->Field("Z0X");
            str_filtro.obra       = false;

            str_filtro.bar_id     = ptrCursor->Field("Barra1");
            str_filtro.esq.posbar = 0;
            str_filtro.esq.dxbar  = ptrCursor->Field("DX");
            str_filtro.esq.dybar  = ptrCursor->Field("DY");

            // Comentário do equipamento
            str_filtro.comentario.dx         = 0;
            str_filtro.comentario.dy         = 0;
            str_filtro.comentario.comentario = "";

            //insere em Monta
            if ((filtro = (VTFiltro*)monta->InsereFiltro(str_filtro)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_FILTROINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(filtro, true);
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
   } // SPSNP_rdFiltro

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdTrafoZZ(int intRedeId)
   {
   //variáveis locais
   bool          sucesso = true;
   VTTrafoZZ*    trafoZZ = NULL;
   AnsiString    strlog;
   strTRAFOZZ    str_trafoZZ;
   TStream*      ptrStream;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdTrafoZZ(:p_RedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->SetVariable("p_RedeId", intRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_trafoZZ.id         = ptrCursor->Field("TrafoZZId");
            str_trafoZZ.codigo     = ptrCursor->Field("Codigo");
            str_trafoZZ.extern_id  = ptrCursor->Field("ExtId");
            str_trafoZZ.vnom       = ptrCursor->Field("Vnom");
            str_trafoZZ.snom       = ptrCursor->Field("Snom");
            str_trafoZZ.z0.r       = ptrCursor->Field("Z0R");
            str_trafoZZ.z0.x       = ptrCursor->Field("Z0X");
            str_trafoZZ.obra       = false;

            str_trafoZZ.bar_id     = ptrCursor->Field("Barra1");
            str_trafoZZ.esq.posbar = 0;
            str_trafoZZ.esq.dxbar  = ptrCursor->Field("DX");
            str_trafoZZ.esq.dybar  = ptrCursor->Field("DY");

            // Comentário do equipamento
            str_trafoZZ.comentario.dx         = 0;
            str_trafoZZ.comentario.dy         = 0;
            str_trafoZZ.comentario.comentario = "";

            //insere em Monta
            if ((trafoZZ = (VTTrafoZZ*)monta->InsereTrafoZZ(str_trafoZZ)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_TRAFOZZINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(trafoZZ, true);
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
   } // SPSNP_rdTrafoZZ

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdSuprimento(int intRedeId)
   {
   //variáveis locais
   bool          sucesso = true;
   VTSuprimento* suprimento = NULL;
   AnsiString    strlog;
   strSUPRIMENTO str_suprimento;
   TStream*      ptrStream;
   strCURHEADER  curhdr;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;
   bool          bolTipica;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdSuprimento(:p_RedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->SetVariable("p_RedeId", intRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_suprimento.sup_id     = ptrCursor->Field("suprimentoId");
            str_suprimento.codigo     = ptrCursor->Field("Codigo");
            str_suprimento.extern_id  = ptrCursor->Field("ExtId");
            str_suprimento.tiposup    = ptrCursor->Field("TipoSuprId");
            str_suprimento.smax       = ptrCursor->Field("Smax");
            str_suprimento.pcc_3f.p   = ptrCursor->Field("Pcc3F");
            str_suprimento.pcc_3f.q   = ptrCursor->Field("Qcc3F");
            str_suprimento.pcc_ft.p   = ptrCursor->Field("PccFT");
            str_suprimento.pcc_ft.q   = ptrCursor->Field("QccFT");
            str_suprimento.zeq0.r     = ptrCursor->Field("Z0R");
            str_suprimento.zeq0.x     = ptrCursor->Field("Z0X");
            str_suprimento.zeq1.r     = ptrCursor->Field("Z1R");
            str_suprimento.zeq1.x     = ptrCursor->Field("Z1X");
            str_suprimento.bar_id     = ptrCursor->Field("Barra1");
            str_suprimento.esq.posbar = 0;
            str_suprimento.esq.dxbar  = 0;
            str_suprimento.esq.dybar  = 0;
            str_suprimento.obra       = false;
            str_suprimento.vnom_kv    = ptrCursor->Field("Vnom");;

            bolTipica                 = bool(int(ptrCursor->Field("Tipica")));
            if (!bolTipica)
               {
               ptrStream = ptrCursor->LOBField("Pontos");
               if (!ptrStream)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CURVAINV
                  sucesso = false;
                  break;
                  }
               if (!bd_func->DesmontaStreamCurvaEquipamento(ptrStream, &(str_suprimento.curva), curhdr))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CURVAINV
                  sucesso = false;
                  break;
                  }
               // O LOBField limpa a stream
               //delete ptrStream;
               }
            // Ao desmontar o BLOB, o header da curva é zerado. Assim, os campos devem
            // ser preenchidos posteriormente
            str_suprimento.curva.tipica = bolTipica;
            str_suprimento.curva.status = 0; // OOPS
            str_suprimento.curva.id     = ptrCursor->Field("CurvaId");

            // Comentário do equipamento
            str_suprimento.comentario.dx         = 0;
            str_suprimento.comentario.dy         = 0;
            str_suprimento.comentario.comentario = "";

            //insere em Monta
            if ((suprimento = (VTSuprimento*)monta->InsereSuprimento(str_suprimento)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_SUPRIMENTOINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(suprimento, true);
               if (suprimento->curva) { suprimento->curva->Status[sttNOVO] = true; }
               }

            ptrCursor->Next();
            // loop registros (end)
            }
         //ptrCursor->Close();
         //ptrQry->Close();
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
   } // SPSNP_rdSuprimento

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdGerador(int intRedeId)
   {
   //variáveis locais
   bool          sucesso = true;
   VTGerador*    gerador = NULL;
   AnsiString    strlog;
   strGERADOR    str_gerador;
   TStream*      ptrStream;
   strCURHEADER  curhdr;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;
   bool          bolTipica;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdGerador(:p_RedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->SetVariable("p_RedeId", intRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_gerador.ger_id       = ptrCursor->Field("GeradorId");
            str_gerador.codigo       = ptrCursor->Field("Codigo");
            str_gerador.extern_id    = ptrCursor->Field("ExtId");
            str_gerador.ligacao      = ptrCursor->Field("TipoLigacaoId");
            str_gerador.tiposup      = ptrCursor->Field("TipoSuprId");
            str_gerador.tipo_geracao = ptrCursor->Field("TipoGerId");
            str_gerador.vnom         = ptrCursor->Field("Vnom");
            str_gerador.snom_mva     = ptrCursor->Field("Snom");
            str_gerador.smax_mva     = ptrCursor->Field("Smax");
            str_gerador.fplim        = ptrCursor->Field("FatorPotMin");
            str_gerador.z0.r         = ptrCursor->Field("Z0R");
            str_gerador.z0.x         = ptrCursor->Field("Z0X");
            str_gerador.z1.r         = ptrCursor->Field("Z1R");
            str_gerador.z1.x         = ptrCursor->Field("Z1X");
            str_gerador.obra         = false;

            str_gerador.bar_id       = ptrCursor->Field("Barra1");
            str_gerador.esq.posbar   = 0;
            str_gerador.esq.dxbar    = ptrCursor->Field("DX");
            str_gerador.esq.dybar    = ptrCursor->Field("DY");

            bolTipica                = bool(int(ptrCursor->Field("Tipica")));
            if (!bolTipica)
               {
               ptrStream = ptrCursor->LOBField("Pontos");
               if (!ptrStream)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CURVAINV
                  sucesso = false;
                  break;
                  }
               if (!bd_func->DesmontaStreamCurvaEquipamento(ptrStream, &(str_gerador.curva), curhdr))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CURVAINV
                  sucesso = false;
                  break;
                  }
               // O LOBField limpa a stream
               //delete ptrStream;
               }
            // Ao desmontar o BLOB, o header da curva é zerado. Assim, os campos devem
            // ser preenchidos posteriormente
            str_gerador.curva.tipica = bolTipica;
            str_gerador.curva.status = 0; // OOPS
            str_gerador.curva.id     = ptrCursor->Field("CurvaId");

            // Comentário do equipamento
            str_gerador.comentario.dx         = 0;
            str_gerador.comentario.dy         = 0;
            str_gerador.comentario.comentario = "";

            //insere em Monta
            if ((gerador = (VTGerador*)monta->InsereGerador(str_gerador)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_GERADORINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(gerador, true);
               if (gerador->curva) { gerador->curva->Status[sttNOVO] = true; }
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
   } // SPSNP_rdGerador
   
//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdReator(int intRedeId)
   {
   //variáveis locais
   bool          sucesso = true;
   VTReator*     reator = NULL;
   AnsiString    strlog;
   strREATOR     str_reator;
   TStream*      ptrStream;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;
   strCURHEADER  curhdr;
   bool          bolTipica;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdReator(:p_RedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->SetVariable("p_RedeId", intRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_reator.id           = ptrCursor->Field("ReatorId");
            str_reator.codigo       = ptrCursor->Field("Codigo");
            str_reator.extern_id    = ptrCursor->Field("ExtId");
            str_reator.ligacao      = ptrCursor->Field("TipoLigacaoId");
            str_reator.vnom         = ptrCursor->Field("Vnom");
            str_reator.q_mvar       = ptrCursor->Field("Qnom");
            str_reator.obra         = false;

            str_reator.bar_id       = ptrCursor->Field("Barra1");
            str_reator.esq.posbar   = 0;
            str_reator.esq.dxbar    = ptrCursor->Field("DX");
            str_reator.esq.dybar    = ptrCursor->Field("DY");
            // mml@20130816 - colocada a curva
            bolTipica               = bool(int(ptrCursor->Field("Tipica")));
            if (!bolTipica)
               {
               ptrStream = ptrCursor->LOBField("Pontos");
               if (!ptrStream)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CURVAINV
                  sucesso = false;
                  break;
                  }
               if (!bd_func->DesmontaStreamCurvaEquipamento(ptrStream, &(str_reator.curva), curhdr))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CURVAINV
                  sucesso = false;
                  break;
                  }
               // O LOBField limpa a stream
               //delete ptrStream;
               }
            // Ao desmontar o BLOB, o header da curva é zerado. Assim, os campos devem
            // ser preenchidos posteriormente
            str_reator.curva.tipica = bolTipica;
            str_reator.curva.status = 0; // OOPS
            str_reator.curva.id     = ptrCursor->Field("CurvaId");

            // Comentário do equipamento
            str_reator.comentario.dx         = 0;
            str_reator.comentario.dy         = 0;
            str_reator.comentario.comentario = "";

            //insere em Monta
            if ((reator = (VTReator*)monta->InsereReator(str_reator)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_REATORINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(reator, true);
               if (reator->curva) { reator->curva->Status[sttNOVO] = true; }
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
   } // SPSNP_rdReator

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdRegulador(int intRedeId)
   {
   //variáveis locais
   bool          sucesso = true;
   VTRegulador*  regulador = NULL;
   AnsiString    strlog;
   strREGULADOR  str_regulador;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdRegulador(:p_RedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->SetVariable("p_RedeId", intRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            // preenche estrutura strREGULADOR
            str_regulador.id                  = ptrCursor->Field("ReguladorId");
            str_regulador.rede_id             = ptrCursor->Field("RedeId");
            str_regulador.codigo              = ptrCursor->Field("Codigo");
            str_regulador.extern_id           = ptrCursor->Field("ExtId");
            str_regulador.ligacao             = ptrCursor->Field("TipoLigacaoId");
            //str_regulador.ajuste_auto         = bool(int(ptrCursor->Field("AjusteAuto")));
            str_regulador.snom_mva            = ptrCursor->Field("Snom");
            str_regulador.by_pass             = bool(int(ptrCursor->Field("Bypass")));
            str_regulador.var_tensao          = ptrCursor->Field("FaixaVar");
            str_regulador.num_passo           = ptrCursor->Field("NumPasso");
            str_regulador.cfg_fixo.passo      = ptrCursor->Field("CfgFixPassoSel");
            str_regulador.cfg_auto.bar_ref_id = ptrCursor->Field("CfgAutBarRefId");
            str_regulador.cfg_auto.v_pu       = ptrCursor->Field("CfgAutVpu");
            str_regulador.obra                = false;

            //str_regulador.sentido.reversivel  = bool(int(ptrCursor->Field("SentidoReversivel")));
            str_regulador.sentido.bar_ref_id  = ptrCursor->Field("SentidoBarRefId");
            str_regulador.z0.r                = ptrCursor->Field("Z0R");
            str_regulador.z0.x                = ptrCursor->Field("Z0X");
            str_regulador.z1.r                = ptrCursor->Field("Z1R");
            str_regulador.z1.x                = ptrCursor->Field("Z1X");

            str_regulador.bar1_id             = ptrCursor->Field("Barra1");
            str_regulador.esq.posbar1         = 0;

            // Comentário do equipamento
            str_regulador.comentario.dx         = 0;
            str_regulador.comentario.dy         = 0;
            str_regulador.comentario.comentario = "";
            str_regulador.bar2_id             = ptrCursor->Field("Barra2");
            str_regulador.esq.posbar2         = 0;

            if ((regulador = (VTRegulador*)monta->InsereRegulador(str_regulador)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_REGULADORINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(regulador, true);
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
   } // SPSNP_rdRegulador

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdTrafo(int intRedeId)
   {
   //variáveis locais
   bool          sucesso = true;
   VTTrafo*      trafo = NULL;
   AnsiString    strlog;
   strTRAFO      str_trafo;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdTrafo(:p_RedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->SetVariable("p_RedeId", intRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)

            // preenche estrutura strTRAFO
            str_trafo.trf_id             = ptrCursor->Field("TrafoId");
            str_trafo.rede_id            = ptrCursor->Field("RedeId");
            str_trafo.codigo             = ptrCursor->Field("Codigo");
            str_trafo.extern_id          = ptrCursor->Field("ExtId");
            str_trafo.obra               = false;

            str_trafo.auto_trafo         = bool(int(ptrCursor->Field("AutoTrafo")));
            str_trafo.perda_ferro        = ptrCursor->Field("PerdaFerro");
            str_trafo.ltc.tipo           = ptrCursor->Field("LtcTipoUtl");
            str_trafo.ltc.bar_id         = ptrCursor->Field("LtcBarRefId");
            str_trafo.ltc.v_pu           = ptrCursor->Field("LtcVpu");
            str_trafo.z0.r               = ptrCursor->Field("Z0R");
            str_trafo.z0.x               = ptrCursor->Field("Z0X");
            str_trafo.z1.r               = ptrCursor->Field("Z1R");
            str_trafo.z1.x               = ptrCursor->Field("Z1X");

            str_trafo.pri.ligacao        = ptrCursor->Field("Enrol1_TipoLigacao");
            str_trafo.pri.fases          = ptrCursor->Field("Enrol1_TipoFase");
            str_trafo.pri.vnom           = ptrCursor->Field("Enrol1_Vnom");
            str_trafo.snom_mva           = ptrCursor->Field("Enrol1_Snom");
            str_trafo.pri.defasagem      = ptrCursor->Field("Enrol1_RotFase");
            str_trafo.pri.tap            = ptrCursor->Field("Enrol1_Vtap");
            str_trafo.pri.zterra.r       = ptrCursor->Field("Enrol1_ZterraR");
            str_trafo.pri.zterra.x       = ptrCursor->Field("Enrol1_ZterraX");

            str_trafo.bar1_id            = ptrCursor->Field("Barra1");
            str_trafo.esq.posbar1        = 0;

            str_trafo.sec.ligacao        = ptrCursor->Field("Enrol2_TipoLigacao");
            str_trafo.sec.fases          = ptrCursor->Field("Enrol2_TipoFase");
            str_trafo.sec.vnom           = ptrCursor->Field("Enrol2_Vnom");
            // OOPS: A linha abaixo está comentada no SINAP
            str_trafo.snom_mva           = ptrCursor->Field("Enrol2_Snom");
            str_trafo.sec.defasagem      = ptrCursor->Field("Enrol2_RotFase");
            str_trafo.sec.tap            = ptrCursor->Field("Enrol2_Vtap");
            str_trafo.sec.zterra.r       = ptrCursor->Field("Enrol2_ZterraR");
            str_trafo.sec.zterra.x       = ptrCursor->Field("Enrol2_ZterraX");

            str_trafo.bar2_id            = ptrCursor->Field("Barra2");
            str_trafo.esq.posbar2        = 0;

            // Comentário do equipamento
            str_trafo.comentario.dx         = 0;
            str_trafo.comentario.dy         = 0;
            str_trafo.comentario.comentario = "";

            if ((trafo = (VTTrafo*)monta->InsereTrafo(str_trafo)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_TRAFOINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(trafo, true);
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
   } // SPSNP_rdTrafo

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdTrafo3E(int intRedeId)
   {
   //variáveis locais
   bool          sucesso = true;
   VTTrafo3E*    trafo3e = NULL;
   AnsiString    strlog;
   strTRAFO3E    str_trafo3e;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdTrafo3e(:p_RedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->SetVariable("p_RedeId", intRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            // preenche estrutura strTRAFO
            str_trafo3e.trf_id             = ptrCursor->Field("Trafo3eId");
            str_trafo3e.rede_id            = ptrCursor->Field("RedeId");
            str_trafo3e.codigo             = ptrCursor->Field("Codigo");
            str_trafo3e.extern_id          = ptrCursor->Field("ExtId");
            str_trafo3e.obra               = false;

            str_trafo3e.perda_ferro        = ptrCursor->Field("PerdaFerro");
            str_trafo3e.zps0.r             = ptrCursor->Field("ZPS0R");
            str_trafo3e.zps0.x             = ptrCursor->Field("ZPS0X");
            str_trafo3e.zpt0.r             = ptrCursor->Field("ZPT0R");
            str_trafo3e.zpt0.x             = ptrCursor->Field("ZPT0X");
            str_trafo3e.zst0.r             = ptrCursor->Field("ZST0R");
            str_trafo3e.zst0.x             = ptrCursor->Field("ZST0X");
            str_trafo3e.zps1.r             = ptrCursor->Field("ZPS1R");
            str_trafo3e.zps1.x             = ptrCursor->Field("ZPS1X");
            str_trafo3e.zpt1.r             = ptrCursor->Field("ZPT1R");
            str_trafo3e.zpt1.x             = ptrCursor->Field("ZPT1X");
            str_trafo3e.zst1.r             = ptrCursor->Field("ZST1R");
            str_trafo3e.zst1.x             = ptrCursor->Field("ZST1X");

            str_trafo3e.pri.ligacao        = ptrCursor->Field("Enrol1_TipoLigacao");
            str_trafo3e.pri.fases          = ptrCursor->Field("Enrol1_TipoFase");
            str_trafo3e.pri.vnom           = ptrCursor->Field("Enrol1_Vnom");
            str_trafo3e.pri.snom           = ptrCursor->Field("Enrol1_Snom");
            str_trafo3e.pri.defasagem      = ptrCursor->Field("Enrol1_RotFase");
            str_trafo3e.pri.tap            = ptrCursor->Field("Enrol1_Vtap");
            str_trafo3e.pri.zterra.r       = ptrCursor->Field("Enrol1_ZterraR");
            str_trafo3e.pri.zterra.x       = ptrCursor->Field("Enrol1_ZterraX");

            str_trafo3e.bar1_id            = ptrCursor->Field("Barra1");
            str_trafo3e.esq.posbar1        = 0;

            str_trafo3e.sec.ligacao        = ptrCursor->Field("Enrol2_TipoLigacao");
            str_trafo3e.sec.fases          = ptrCursor->Field("Enrol2_TipoFase");
            str_trafo3e.sec.vnom           = ptrCursor->Field("Enrol2_Vnom");
            str_trafo3e.sec.snom           = ptrCursor->Field("Enrol2_Snom");
            str_trafo3e.sec.defasagem      = ptrCursor->Field("Enrol2_RotFase");
            str_trafo3e.sec.tap            = ptrCursor->Field("Enrol2_Vtap");
            str_trafo3e.sec.zterra.r       = ptrCursor->Field("Enrol2_ZterraR");
            str_trafo3e.sec.zterra.x       = ptrCursor->Field("Enrol2_ZterraX");

            str_trafo3e.bar2_id            = ptrCursor->Field("Barra2");
            str_trafo3e.esq.posbar2        = 0;

            str_trafo3e.ter.ligacao        = ptrCursor->Field("Enrol3_TipoLigacao");
            str_trafo3e.ter.fases          = ptrCursor->Field("Enrol3_TipoFase");
            str_trafo3e.ter.vnom           = ptrCursor->Field("Enrol3_Vnom");
            str_trafo3e.ter.snom           = ptrCursor->Field("Enrol3_Snom");
            str_trafo3e.ter.defasagem      = ptrCursor->Field("Enrol3_RotFase");
            str_trafo3e.ter.tap            = ptrCursor->Field("Enrol3_Vtap");
            str_trafo3e.ter.zterra.r       = ptrCursor->Field("Enrol3_ZterraR");
            str_trafo3e.ter.zterra.x       = ptrCursor->Field("Enrol3_ZterraX");

            str_trafo3e.bar3_id            = ptrCursor->Field("Barra3");
            str_trafo3e.esq.posbar3        = 0;

            // Comentário do equipamento
            str_trafo3e.comentario.dx         = 0;
            str_trafo3e.comentario.dy         = 0;
            str_trafo3e.comentario.comentario = "";

            if ((trafo3e = (VTTrafo3E*)monta->InsereTrafo3E(str_trafo3e)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_TRAFO3EINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(trafo3e, true);
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
   } // SPSNP_rdTrafo3e

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdMutua(int intRedeId)
   {
   //variáveis locais
   bool          sucesso = true;
   VTMutua*      mutua = NULL;
   AnsiString    strlog;
   strMUTUA      str_mutua;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdMutua(:p_RedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->SetVariable("p_RedeId", intRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_mutua.id           = ptrCursor->Field("MutuaId");
            str_mutua.rede_id      = ptrCursor->Field("RedeId");
            str_mutua.codigo       = ptrCursor->Field("Codigo");
            str_mutua.extern_id    = ptrCursor->Field("ExtId");
            str_mutua.obra         = false;

            // preenche estrutura strMUTUA
            str_mutua.tre1_id      = ptrCursor->Field("Trecho1Id");
            str_mutua.tre1_bar_id  = ptrCursor->Field("Trecho1BarraSntId");
            str_mutua.postre1      = ptrCursor->Field("Trecho1Pos");
            str_mutua.tre2_id      = ptrCursor->Field("Trecho2Id");
            str_mutua.tre2_bar_id  = ptrCursor->Field("Trecho2BarraSntId");
            str_mutua.postre2      = ptrCursor->Field("Trecho2Pos");
            str_mutua.z0.r         = ptrCursor->Field("Z0R");
            str_mutua.z0.x         = ptrCursor->Field("Z0X");
            str_mutua.z1.r         = ptrCursor->Field("Z1R");
            str_mutua.z1.x         = ptrCursor->Field("Z1X");

            // Comentário do equipamento
            str_mutua.comentario.dx         = 0;
            str_mutua.comentario.dy         = 0;
            str_mutua.comentario.comentario = "";

            if ((mutua = (VTMutua*)monta->InsereMutua(str_mutua)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_MUTUAINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(mutua, true);
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
   } // SPSNP_rdMutua

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrCurvaPropria(void)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   VTEqpto*      eqpto;
   VTCurva*      curva;
   TOracleQuery* ptrQry = NULL;
   TLOBLocator*  lobLocator = NULL;
   TList*        lstEqpto = NULL;
   TList*        lstCurvaCarga = NULL;
   TList*        lstCurvaCapacitor = NULL;
   TList*        lstCurvaGerador = NULL;
   TList*        lstCurvaSuprimento = NULL;

   // Estrutura auxiliar para simplificação do loop
   struct sttaux
      {
      int intTipoEquipId;
      TList* lisCur;
      };
   struct sttaux veccur[4];

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }
   if (!redes)                      { SNPSTRLOGLINE(strlog); MSGERRSYS_REDESINV   return(false); }
   if (!curvas)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_CURVAINV   return(false); }

   lstEqpto = new TList();
   lstCurvaCarga = new TList();
   lstCurvaCapacitor = new TList();
   lstCurvaGerador = new TList();
   lstCurvaSuprimento = new TList();

   // OOPS: Lista do Guaraldo da antiga
   // Os equipamentos que podem ter curva própria são Carga, Capacitor, Gerador e Suprimento.
   // veccur[2].intTipoEquipId = eqptoCARGA;     veccur[2].lisCur = lstCurvaIP;
   // veccur[3].intTipoEquipId = eqptoREATOR;    veccur[3].lisCur = lstCurvaReator;
   veccur[0].intTipoEquipId = eqptoCARGA;      veccur[0].lisCur = lstCurvaCarga;
   veccur[1].intTipoEquipId = eqptoCAPACITOR;  veccur[1].lisCur = lstCurvaCapacitor;
   veccur[2].intTipoEquipId = eqptoGERADOR;    veccur[2].lisCur = lstCurvaGerador;
   veccur[3].intTipoEquipId = eqptoSUPRIMENTO; veccur[3].lisCur = lstCurvaSuprimento;

   // Para cada tipo de equipamento, busca as curvas pertinentes
   // OOPS: Somente os não isolados?
   for (int i = 0; i < (int)(sizeof(veccur) / sizeof(struct sttaux)); i++)
      {
      lstEqpto->Clear();
      redes->LisEqptoNaoIsolado(lstEqpto, veccur[i].intTipoEquipId);
      for (int j = 0; j < lstEqpto->Count; j++)
         {
         if ((eqpto = (VTEqpto*)lstEqpto->Items[j]) != NULL)
            {
            if (eqpto->TipoEqbar())
               {
               curva = ((VTEqbar*)eqpto)->ExisteCurva();
               if (curva)
                  {
                  if (!curva->Tipica)
                     {
                     veccur[i].lisCur->Add(curva);
                     }
                  }
               }
            }
         }
      }

   // Após a pesquisa dos elementos, salva tudo na base
   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrCurva(:p_CurvaId, :p_Descricao, :p_Tipica, :p_TipoCurvaId, :p_Pontos); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_CurvaId",     otInteger);
         ptrQry->DeclareVariable("p_Descricao",   otString );
         ptrQry->DeclareVariable("p_Tipica",      otInteger);
         ptrQry->DeclareVariable("p_TipoCurvaId", otInteger);
         ptrQry->DeclareVariable("p_Pontos",      otBLOB   );

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

               // Considera que todas as curvas são novas, visto que a carga será
               // efetuada uma única vez
               curva->Id = IDINDEFINIDO;

               ptrQry->SetVariable("p_CurvaId",     curva->Id    );
               ptrQry->SetVariable("p_Descricao",   curva->Codigo);
               ptrQry->SetVariable("p_Tipica",      (curva->Tipica ? 1 : 0));
               ptrQry->SetVariable("p_TipoCurvaId", curva->Tipo  );

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
               curva->Id = ptrQry->GetVariable("p_CurvaId");
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
      if (lobLocator)           { delete lobLocator;         lobLocator = NULL;         }
      if (ptrQry)               { delete ptrQry;             ptrQry = NULL;             }
      if (lstEqpto)             { delete lstEqpto;           lstEqpto = NULL;           }
      if (lstCurvaCarga)        { delete lstCurvaCarga;      lstCurvaCarga = NULL;      }
      if (lstCurvaCapacitor)    { delete lstCurvaCapacitor;  lstCurvaCapacitor = NULL;  }
      if (lstCurvaGerador)      { delete lstCurvaGerador;    lstCurvaGerador = NULL;    }
      if (lstCurvaSuprimento)   { delete lstCurvaSuprimento; lstCurvaSuprimento = NULL; }
      }

   return(sucesso);
   }
   
//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrCurvaTipica(void)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   VTCurva*      curva;
   TOracleQuery* ptrQry = NULL;
   TLOBLocator*  lobLocator = NULL;

   // Estrutura auxiliar para simplificação do loop
   struct sttaux
      {
      int intTipoEquipId;
      TList* lisCur;
      };
   struct sttaux veccur[5];

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }
   if (!curvas)                     { SNPSTRLOGLINE(strlog); MSGERRSYS_CURVAINV   return(false); }

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
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrCurva(:p_CurvaId, :p_Descricao, :p_Tipica, :p_TipoCurvaId, :p_Pontos); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_CurvaId",     otInteger);
         ptrQry->DeclareVariable("p_Descricao",   otString );
         ptrQry->DeclareVariable("p_Tipica",      otInteger);
         ptrQry->DeclareVariable("p_TipoCurvaId", otInteger);
         ptrQry->DeclareVariable("p_Pontos",      otBLOB   );

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

               // Considera que todas as curvas são novas, visto que a carga será
               // efetuada uma única vez
               curva->Id = IDINDEFINIDO;

               ptrQry->SetVariable("p_CurvaId",     curva->Id    );
               ptrQry->SetVariable("p_Descricao",   curva->Codigo);
               ptrQry->SetVariable("p_Tipica",      (curva->Tipica ? 1 : 0));
               ptrQry->SetVariable("p_TipoCurvaId", curva->Tipo  );

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
               curva->Id = ptrQry->GetVariable("p_CurvaId");
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
bool __fastcall TBDGateway::SPSNP_wrEquipamento(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   VTEqbar*             eqbaraux;
   VTCurva*             curva;
   VTLigacao*           ligacaoaux;
   VTBarra*             barra1aux;
   VTBarra*             barra2aux;
   VTBarra*             barra3aux;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_EquipId"),     varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_TipoEquipId"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),      varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),       varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_CurvaId"),     varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra1Id"),    varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra2Id"),    varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra3Id"),    varInteger, otInteger);
         
         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrEquipamento ( :p_EquipId, :p_TipoEquipId, :p_Codigo, :p_ExtId, :p_CurvaId, :p_Barra1Id, :p_Barra2Id, :p_Barra3Id);");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

               // p_EquipId : índice 0
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 0, eqpto->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 1, eqpto->Tipo());
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 2, eqpto->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 3, eqpto->Extern_id);
               // Salva a curva, se existir
               curva = eqpto->ExisteCurva();
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 4, curva ? curva->Id : IDINDEFINIDO);
               // Salva as barras
               if (eqpto->TipoEqbar())
                  {
                  barra1aux = ((VTEqbar*)eqpto)->pbarra;
                  barra2aux = NULL;
                  barra3aux = NULL;
                  }
               else
                  {
                  if (eqpto->TipoLigacao())
                     {
                     barra1aux = ((VTLigacao*)eqpto)->pbarra1;
                     barra2aux = ((VTLigacao*)eqpto)->pbarra2;
                     barra3aux = ((VTLigacao*)eqpto)->pbarra3;
                     }
                  else
                     {
                     barra1aux = NULL;
                     barra2aux = NULL;
                     barra3aux = NULL;
                     }
                  }
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 5, barra1aux);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 6, barra2aux);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 7, barra3aux);
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();
            // Transfere os ids obtidos para os objetos em memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
               eqpto->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  eqpto->Id = varInt;
                  }
               }

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrRede(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros
   // Hierarquia das redes
   int                  node_id;
   int                  node_pai_id;
   VTNode*              node = NULL;
   VTNode*              node_pai = NULL;
   VTArvore*            arvore = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
   if (arvore == NULL)              { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_RedeId"),      varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),      varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),       varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_TipoRedeId"),  varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_NoId"),        varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_NoPaiId"),     varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Radial"),      varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_BarraIniId"),  varInteger, otInteger);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrRede( :p_RedeId, :p_Codigo, :p_ExtId, :p_TipoRedeId, :p_NoId, " );
         ptrQry->SQL->Add("                              :p_NoPaiId, :p_Radial, :p_BarraIniId");
         ptrQry->SQL->Add("                             );");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTRede* rede = (VTRede*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

               //determina Node e NodePai da Rede
               node = arvore->ExisteNode(rede);
               if (node != NULL)
                  {
                  node_pai = node->Pai;
                  if (node_pai != NULL)
                     {
                     if (node_pai->Pai == NULL)
                        {
                        node_pai = NULL;
                        }
                     }
                  }
               node_id     = (node     != NULL) ? node->Id     : -1;
               node_pai_id = (node_pai != NULL) ? node_pai->Id : -1;

               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 0, rede->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 1, rede->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 2, rede->Extern_id.IsEmpty() ? rede->Codigo : rede->Extern_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 3, rede->TipoRede->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 4, node_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 5, node_pai_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 6, (int)rede->Radial);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 7, rede->BarraInicial());
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();
            // Transfere os ids obtidos para os objetos em memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
               eqpto->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  eqpto->Id = varInt;
                  }
               }

            // Transfere os ids das MRedes obtidas para a memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[4]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTRede* rede = (VTRede*)lstEqpto->Items[0];
               if (!rede->MRede)
                  {
                  sucesso = false;
                  break;
                  }
               rede->MRede->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTRede* rede = (VTRede*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];
                  if (!rede->MRede)
                     {
                     sucesso = false;
                     break;
                     }
                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  rede->MRede->Id = varInt;
                  }
               }

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrEquipamentoRede(VTRede* rede, TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!rede)                       { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_EquipId"),  varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_RedeId"),   varInteger, otInteger);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrEquipamentoRede( :p_EquipId, :p_RedeId );"  );
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 0, eqpto->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 1, rede->Id);
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrBarra(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_BarraId"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),  varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),   varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_VNom"),    varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_UtmX"),    varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_UtmY"),    varInteger, otInteger);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrBarra( :p_BarraId, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                               :p_VNom,:p_UtmX, :p_UtmY ");
         ptrQry->SQL->Add("                              );");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;

            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTBarra* barra = (VTBarra*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 0, barra->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 1, barra->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 2, barra->Extern_id.IsEmpty() ? barra->Codigo : barra->Extern_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 3, barra->vnom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 4, barra->utm.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 5, barra->utm.y);
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();
            // Transfere os ids obtidos para os objetos em memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
               eqpto->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  eqpto->Id = varInt;
                  }
               }

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }
   
//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrCarga(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_CargaId"),    varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),     varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),      varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_CurvaId"),    varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra1Id"),   varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_TipoFaseId"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_TipoConsId"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Energia"),    varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ICtePu"),     varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ZCtePu"),     varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_SCtePu"),     varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_NumConsRes"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_NumConsCom"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_NumConsInd"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_NumConsRur"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_NumConsOut"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_NumConsA4"),  varInteger, otInteger);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrCarga( :p_CargaId, :p_Codigo, :p_ExtId, :p_CurvaId, :p_Barra1Id, "  );
         ptrQry->SQL->Add("                               :p_TipoFaseId, :p_TipoConsId, :p_Energia, :p_ICtePu, :p_ZCtePu, :p_SCtePu, ");
         ptrQry->SQL->Add("                               :p_NumConsRes, :p_NumConsCom, :p_NumConsInd, :p_NumConsRur, :p_NumConsOut, :p_NumConsA4 ");
         ptrQry->SQL->Add("                              );");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTCarga* carga = (VTCarga*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  0, carga->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  1, carga->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  2, carga->Extern_id.IsEmpty() ? carga->Codigo : carga->Extern_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  3, carga->curva ? carga->curva->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  4, carga->pbarra ? carga->pbarra->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  5, carga->Fases);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  6, carga->classe ? carga->classe->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  7, carga->Energia_mwhmes);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  8, carga->Icte_pu);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  9, carga->Zcte_pu);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 10, carga->Scte_pu);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 11, 0);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 12, 0);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 13, 0);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 14, 0);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 15, 0);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 16, 0);
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();
            // Transfere os ids obtidos para os objetos em memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
               eqpto->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  eqpto->Id = varInt;
                  }
               }

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrCargaNL(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_CargaNLId"),  varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),     varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),      varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra1Id"),   varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_OrdemId"),    varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_FatorPot"),   varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Indutivo"),   varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_kVA"),        varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_SNomA"),      varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_SNomB"),      varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_SNomC"),      varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_AngA"),       varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_AngB"),       varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_AngC"),       varDouble,  otFloat);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrCargaNL( :p_CargaNLId, :p_Codigo, :p_ExtId, :p_Barra1Id, "  );
         ptrQry->SQL->Add("                                 :p_OrdemId, :p_FatorPot, :p_Indutivo, :p_kVA, ");
         ptrQry->SQL->Add("                                 :p_SNomA, :p_SNomB, :p_SNomC, :p_AngA, :p_AngB, :p_AngC ");
         ptrQry->SQL->Add("                                );");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTCargaNL* cargaNL = (VTCargaNL*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];
               // Inicialmente salva a fundamental
               strFUN* ptrFUN = cargaNL->LeFundamental();
               if (ptrFUN == NULL) return(false);

               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  0, cargaNL->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  1, cargaNL->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  2, cargaNL->Extern_id.IsEmpty() ? cargaNL->Codigo : cargaNL->Extern_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  3, cargaNL->pbarra ? cargaNL->pbarra->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  4, 1);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  5, cargaNL->FatPot_pu);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  6, (int)cargaNL->Indutiva);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  7, (int)true);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  8, ptrFUN->sa_mod_kva);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  9, ptrFUN->sb_mod_kva);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 10, ptrFUN->sc_mod_kva);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 11, ptrFUN->sa_ang_grau);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 12, ptrFUN->sb_ang_grau);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 13, ptrFUN->sc_ang_grau);
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();
            // Transfere os ids obtidos para os objetos em memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
               eqpto->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  eqpto->Id = varInt;
                  }
               }

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrCargaIP(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros
   int                  intCargaIpCount;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_CargaIPId"),  varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),     varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),      varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_CurvaId"),    varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra1Id"),   varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Pnom"),       varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_FatorPot"),   varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_NumLamp"),    varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_TipoFaseId"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ICte"),       varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ZCte"),       varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_SCte"),       varDouble,  otFloat);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrCargaIP( :p_CargaIPId, :p_Codigo, :p_ExtId, :p_CurvaId, :p_Barra1Id, "  );
         ptrQry->SQL->Add("                                 :p_Pnom, :p_FatorPot, :p_NumLamp, :p_TipoFaseId, ");
         ptrQry->SQL->Add("                                 :p_ICte, :p_ZCte, :p_SCte  ");
         ptrQry->SQL->Add("                                );");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            // Indica quantas cargas são IP efetivamente
            intCargaIpCount = 0;
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTCarga* carga = (VTCarga*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];
               // Cargas que não são do tipo iluminação pública são escritas por um método específico
               if (!carga->curva)  { SNPSTRLOGLINE(strlog); MSGERRSYS_CURVAINV return(false); }
               if (carga->curva->Tipo != curvaLD)
                  {
                  continue;
                  }
               intCargaIpCount++;

               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  0, carga->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  1, carga->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  2, carga->Extern_id.IsEmpty() ? carga->Codigo : carga->Extern_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  3, carga->curva ? carga->curva->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  4, carga->pbarra ? carga->pbarra->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  5, carga->IP_Pot_kw);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  6, carga->IP_Fatpot);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  7, carga->IP_NumLamp);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  8, carga->Fases);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  9, carga->Icte_pu);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 10, carga->Zcte_pu);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 11, carga->Scte_pu);
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Nota: carga IP é um caso especial, porque somente grava alguns
            // itens da lista original. Por esse motivo é que foi feita essa verificação.
            if (intCargaIpCount > 0)
               {
               // Executa a query
               ptrQry->Execute();

               // Transfere os ids obtidos para os objetos em memória
               dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
               dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
               // Se tem somente um elemento, o Array não é considerado array e é
               // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
               if (VarType(dbParam->arrVar) == (varInteger))
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
                  eqpto->Id = dbParam->arrVar;
                  }
               else
                  {
                  // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
                  // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
                  // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
                  for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                     {
                     VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                     varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                     eqpto->Id = varInt;
                     }
                  }

               // Ajusta o ponteiro das linhas transferidas
               intTransferidos +=  intNumLinhas;
               }
            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrFiltro(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_FiltroId"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),   varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),    varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra1Id"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Vnom"),     varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Snom"),     varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z0R"),      varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z0X"),      varDouble,  otFloat);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrFiltro( :p_FiltroId, :p_Codigo, :p_ExtId, :p_Barra1Id, "  );
         ptrQry->SQL->Add("                                :p_Vnom, :p_Snom, :p_Z0R, :p_Z0X ");
         ptrQry->SQL->Add("                               );");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTFiltro* filtro = (VTFiltro*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  0, filtro->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  1, filtro->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  2, filtro->Extern_id.IsEmpty() ? filtro->Codigo : filtro->Extern_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  3, filtro->pbarra ? filtro->pbarra->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  4, filtro->vnom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  5, filtro->snom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  6, filtro->z0.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  7, filtro->z0.x);
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();
            // Transfere os ids obtidos para os objetos em memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
               eqpto->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  eqpto->Id = varInt;
                  }
               }

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrTrafoZZ(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_FiltroId"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),   varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),    varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra1Id"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Vnom"),     varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Snom"),     varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z0R"),      varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z0X"),      varDouble,  otFloat);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrTrafoZZ( :p_TrafoZZId, :p_Codigo, :p_ExtId, :p_Barra1Id, "  );
         ptrQry->SQL->Add("                                 :p_Vnom, :p_Snom, :p_Z0R, :p_Z0X ");
         ptrQry->SQL->Add("                                );");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTTrafoZZ* trafoZZ = (VTTrafoZZ*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  0, trafoZZ->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  1, trafoZZ->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  2, trafoZZ->Extern_id.IsEmpty() ? trafoZZ->Codigo : trafoZZ->Extern_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  3, trafoZZ->pbarra ? trafoZZ->pbarra->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  4, trafoZZ->vnom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  5, trafoZZ->snom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  6, trafoZZ->z0.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  7, trafoZZ->z0.x);
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();
            // Transfere os ids obtidos para os objetos em memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
               eqpto->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  eqpto->Id = varInt;
                  }
               }

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrSuprimento(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_SuprimentoId"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),       varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),        varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_CurvaId"),      varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra1Id"),     varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_TipoSuprId"),   varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Vnom"),         varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Smax"),         varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Pcc3F"),        varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Qcc3F"),        varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_PccFT"),        varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_QccFT"),        varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z0R"),          varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z0X"),          varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z1R"),          varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z1X"),          varDouble,  otFloat);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrSuprimento( :p_SuprimentoId, :p_Codigo, :p_ExtId, :p_CurvaId, :p_Barra1Id, "  );
         ptrQry->SQL->Add("                                    :p_TipoSuprId, :p_Vnom, :p_Smax, :p_Pcc3F, :p_Qcc3F, :p_PccFT, :p_QccFT, ");
         ptrQry->SQL->Add("                                    :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X  ");
         ptrQry->SQL->Add("                                  );");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTSuprimento* suprimento = (VTSuprimento*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  0, suprimento->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  1, suprimento->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  2, suprimento->Extern_id.IsEmpty() ? suprimento->Codigo : suprimento->Extern_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  3, suprimento->curva ? suprimento->curva->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  4, suprimento->pbarra ? suprimento->pbarra->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  5, suprimento->tiposup);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  6, suprimento->vnom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  7, suprimento->smax);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  8, suprimento->pcc_3f.p);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  9, suprimento->pcc_3f.q);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 10, suprimento->pcc_ft.p);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 11, suprimento->pcc_ft.q);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 12, suprimento->zeq0.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 13, suprimento->zeq0.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 14, suprimento->zeq1.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 15, suprimento->zeq1.x);
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();
            // Transfere os ids obtidos para os objetos em memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
               eqpto->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  eqpto->Id = varInt;
                  }
               }

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrGerador(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_GeradorId"),     varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),        varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),         varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_CurvaId"),       varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra1Id"),      varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_TipoLigacaoId"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_TipoSuprId"),    varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_TipoGerId"),     varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Vnom"),          varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Snom"),          varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Smax"),          varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_FatorPotMin"),   varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z0R"),           varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z0X"),           varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z1R"),           varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z1X"),           varDouble,  otFloat);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrGerador( :p_GeradorId, :p_Codigo, :p_ExtId, :p_CurvaId, :p_Barra1Id, "  );
         ptrQry->SQL->Add("                                 :p_TipoLigacaoId, :p_TipoSuprId, :p_TipoGerId, :p_Vnom, :p_Snom, :p_Smax, ");
         ptrQry->SQL->Add("                                 :p_FatorPotMin, :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X ");
         ptrQry->SQL->Add("                               );");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTGerador* gerador = (VTGerador*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  0, gerador->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  1, gerador->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  2, gerador->Extern_id.IsEmpty() ? gerador->Codigo : gerador->Extern_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  3, gerador->curva ? gerador->curva->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  4, gerador->pbarra ? gerador->pbarra->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  5, gerador->ligacao);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  6, gerador->tiposup);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  7, gerador->FonteEnergia);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  8, gerador->vnom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  9, gerador->snom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 10, gerador->smax);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 11, gerador->fplim);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 12, gerador->z0.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 13, gerador->z0.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 14, gerador->z1.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 15, gerador->z1.x);
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();
            // Transfere os ids obtidos para os objetos em memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
               eqpto->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  eqpto->Id = varInt;
                  }
               }

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrReator(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ReatorId"),      varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),        varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),         varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_CurvaId"),       varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra1Id"),      varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_TipoLigacaoId"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Vnom"),          varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Qnom"),          varDouble,  otFloat);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrReator( :p_ReatorId, :p_Codigo, :p_ExtId, :p_CurvaId, :p_Barra1Id, "  );
         ptrQry->SQL->Add("                                :p_TipoLigacaoId, :p_Vnom, :p_Qnom ");
         ptrQry->SQL->Add("                              );");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTReator* reator = (VTReator*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  0, reator->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  1, reator->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  2, reator->Extern_id.IsEmpty() ? reator->Codigo : reator->Extern_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  3, reator->curva ? reator->curva->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  4, reator->pbarra ? reator->pbarra->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  5, reator->ligacao);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  6, reator->vnom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  7, reator->Q_mvar);
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();
            // Transfere os ids obtidos para os objetos em memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
               eqpto->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  eqpto->Id = varInt;
                  }
               }

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrCapacitor(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_CapacitorId"),   varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),        varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),         varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_CurvaId"),       varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra1Id"),      varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_TipoLigacaoId"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Vnom"),          varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Qnom"),          varDouble,  otFloat);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrCapacitor( :p_CapacitorId, :p_Codigo, :p_ExtId, :p_CurvaId, :p_Barra1Id, "  );
         ptrQry->SQL->Add("                                   :p_TipoLigacaoId, :p_Qnom, :p_Vnom ");
         ptrQry->SQL->Add("                                 );");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTCapacitor* capacitor = (VTCapacitor*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  0, capacitor->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  1, capacitor->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  2, capacitor->Extern_id.IsEmpty() ? capacitor->Codigo : capacitor->Extern_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  3, capacitor->curva ? capacitor->curva->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  4, capacitor->pbarra ? capacitor->pbarra->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  5, capacitor->ligacao);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  6, capacitor->vnom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  7, capacitor->Q_mvar);
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();
            // Transfere os ids obtidos para os objetos em memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
               eqpto->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  eqpto->Id = varInt;
                  }
               }

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrCapacitorSerie(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_CapacitorSerieId"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),           varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),            varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra1Id"),         varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra2Id"),         varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Vnom"),             varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Snom"),             varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ZR"),               varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ZX"),               varDouble,  otFloat);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrCapacitorSerie( :p_CapacitorSerieId, :p_Codigo, :p_ExtId, :p_Barra1Id, :p_Barra2Id, "  );
         ptrQry->SQL->Add("                                        :p_Vnom, :p_Snom, :p_ZR, :p_ZX ");
         ptrQry->SQL->Add("                                      );");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTCapserie* capserie = (VTCapserie*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  0, capserie->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  1, capserie->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  2, capserie->Extern_id.IsEmpty() ? capserie->Codigo : capserie->Extern_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  3, capserie->pbarra1 ? capserie->pbarra1->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  4, capserie->pbarra2 ? capserie->pbarra2->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  5, capserie->vnom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  6, capserie->snom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  7, capserie->z.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  8, capserie->z.r);
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();
            // Transfere os ids obtidos para os objetos em memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
               eqpto->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  eqpto->Id = varInt;
                  }
               }

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrChave(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ChaveId"),     varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),      varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),       varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra1Id"),    varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra2Id"),    varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_TipoChaveId"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Estado"),      varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Inom"),        varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Telecomando"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Protecao"),    varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_TecnoCom"),    varInteger, otInteger);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrChave( :p_ChaveId, :p_Codigo, :p_ExtId, :p_Barra1Id, :p_Barra2Id, ");
         ptrQry->SQL->Add("                               :p_TipoChaveId, :p_Estado, :p_Inom,");
         ptrQry->SQL->Add("                               :p_Telecomando, :p_Protecao, :p_TecnoCom");
         ptrQry->SQL->Add("                             );");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTChave* chave = (VTChave*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  0, chave->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  1, chave->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  2, chave->Extern_id.IsEmpty() ? chave->Codigo : chave->Extern_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  3, chave->pbarra1 ? chave->pbarra1->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  4, chave->pbarra2 ? chave->pbarra2->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  5, chave->TipoChave ? chave->TipoChave->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  6, chave->Estado);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  7, chave->Inom_amp);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  8, 0);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  9, 0);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 10, 0);
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();
            // Transfere os ids obtidos para os objetos em memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
               eqpto->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  eqpto->Id = varInt;
                  }
               }

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }
   
//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrTrecho(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_TrechoId"),    varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),      varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),       varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra1Id"),    varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra2Id"),    varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Comprimento"), varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ArranjoId"),   varInteger, otInteger);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrTrecho( :p_TrechoId, :p_Codigo, :p_ExtId, :p_Barra1Id, :p_Barra2Id, ");
         ptrQry->SQL->Add("                                :p_Comprimento, :p_ArranjoId");
         ptrQry->SQL->Add("                              );");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTTrecho* trecho = (VTTrecho*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  0, trecho->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  1, trecho->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  2, trecho->Extern_id.IsEmpty() ? trecho->Codigo : trecho->Extern_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  3, trecho->pbarra1 ? trecho->pbarra1->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  4, trecho->pbarra2 ? trecho->pbarra2->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  5, trecho->Comprimento_km);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  6, trecho->arranjo ? trecho->arranjo->Id : IDINDEFINIDO);
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();
            // Transfere os ids obtidos para os objetos em memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
               eqpto->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  eqpto->Id = varInt;
                  }
               }

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrRegulador(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ReguladorId"),       varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),            varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),             varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra1Id"),          varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra2Id"),          varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_TipoLigacaoId"),     varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Snom"),              varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Bypass"),            varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_FaixaVar"),          varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_NumPasso"),          varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_SentidoReversivel"), varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_SentidoBarRefId"),   varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_AjusteAuto"),        varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_CfgFixPassoSel"),    varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_CfgAutBarRefId"),    varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_CfgAutVpu"),         varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z0R"),               varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z0X"),               varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z1R"),               varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z1X"),               varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ModeloId"),          varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_TipoFaseId"),        varInteger, otInteger);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrRegulador( :p_ReguladorId, :p_Codigo, :p_ExtId, :p_Barra1Id, :p_Barra2Id, ");
         ptrQry->SQL->Add("                                   :p_TipoLigacaoId, :p_Snom, :p_Bypass, :p_FaixaVar, :p_NumPasso, :p_SentidoReversivel, :p_SentidoBarRefId,");
         ptrQry->SQL->Add("                                   :p_AjusteAuto, :p_CfgFixPassoSel, :p_CfgAutBarRefId, :p_CfgAutVpu,");
         ptrQry->SQL->Add("                                   :p_ModeloId, :p_TipoFaseId");
         ptrQry->SQL->Add("                                 );");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTRegulador* regulador = (VTRegulador*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  0, regulador->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  1, regulador->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  2, regulador->Extern_id.IsEmpty() ? regulador->Codigo : regulador->Extern_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  3, regulador->pbarra1 ? regulador->pbarra1->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  4, regulador->pbarra2 ? regulador->pbarra2->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  5, regulador->ligacao);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  6, regulador->snom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  7, regulador->by_pass);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  8, regulador->var_tensao);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  9, regulador->num_passo);
			   GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 10, 1);//regulador->sentido.reversivel);  //default temporario
			   GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 11, regulador->sentido.bar_ref ? regulador->sentido.bar_ref->Id : IDINDEFINIDO);
			   GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 12, 1);//regulador->ajuste_auto);       //default temporario
			   GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 13, regulador->cfg_fixo.passo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 14, regulador->cfg_auto.bar_ref ? regulador->cfg_auto.bar_ref->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 15, regulador->cfg_auto.v_pu);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 16, regulador->z0.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 17, regulador->z0.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 18, regulador->z1.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 19, regulador->z1.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 20, regulador->modelo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 21, regulador->fases);
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();
            // Transfere os ids obtidos para os objetos em memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
               eqpto->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  eqpto->Id = varInt;
                  }
               }

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrTrafo(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_TrafoId"),               varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),                varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),                 varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra1Id"),              varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra2Id"),              varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_AutoTrafo"),             varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_PerdaFerro"),            varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_LtcTipoUtl"),            varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_LtcBarRefId"),           varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_LtcVpu"),                varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z0R"),                   varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z0X"),                   varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z1R"),                   varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z1X"),                   varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol1_TipoLigacaoId"),  varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol1_TipoFaseId"),     varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol1_Vnom"),           varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol1_Snom"),           varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol1_RotFase"),        varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol1_Vtap"),           varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol1_ZterraR"),        varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol1_ZterraX"),        varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol2_TipoLigacaoId"),  varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol2_TipoFaseId"),     varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol2_Vnom"),           varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol2_Snom"),           varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol2_RotFase"),        varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol2_Vtap"),           varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol2_ZterraR"),        varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol2_ZterraX"),        varDouble,  otFloat);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrTrafo( :p_TrafoId, :p_Codigo, :p_ExtId, :p_Barra1Id, :p_Barra2Id, ");
         ptrQry->SQL->Add("                               :p_AutoTrafo, :p_PerdaFerro, :p_LtcTipoUtl, :p_LtcBarRefId, :p_LtcVpu, :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X,");
         ptrQry->SQL->Add("                               :p_Enrol1_TipoLigacaoId, :p_Enrol1_TipoFaseId, :p_Enrol1_Vnom, :p_Enrol1_Snom, :p_Enrol1_RotFase, :p_Enrol1_Vtap, :p_Enrol1_ZterraR, :p_Enrol1_ZterraX,");
         ptrQry->SQL->Add("                               :p_Enrol2_TipoLigacaoId, :p_Enrol2_TipoFaseId, :p_Enrol2_Vnom, :p_Enrol2_Snom, :p_Enrol2_RotFase, :p_Enrol2_Vtap, :p_Enrol2_ZterraR, :p_Enrol2_ZterraX ");
         ptrQry->SQL->Add("                             );");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTTrafo* trafo = (VTTrafo*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  0, trafo->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  1, trafo->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  2, trafo->Extern_id.IsEmpty() ? trafo->Codigo : trafo->Extern_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  3, trafo->pbarra1 ? trafo->pbarra1->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  4, trafo->pbarra2 ? trafo->pbarra2->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  5, trafo->auto_trafo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  6, trafo->perda_ferro);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  7, trafo->ltc.tipo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  8, trafo->ltc.ajuste_auto.bar_ref ? trafo->ltc.ajuste_auto.bar_ref->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  9, trafo->ltc.ajuste_auto.v_pu);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 10, trafo->z0.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 11, trafo->z0.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 12, trafo->z1.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 13, trafo->z1.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 14, trafo->pri.ligacao);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 15, trafo->pri.fase);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 16, trafo->pri.vnom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 17, trafo->snom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 18, trafo->pri.defasagem);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 19, trafo->pri.tap);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 20, trafo->pri.zterra.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 21, trafo->pri.zterra.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 22, trafo->sec.ligacao);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 23, trafo->sec.fase);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 24, trafo->sec.vnom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 25, trafo->snom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 26, trafo->sec.defasagem);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 27, trafo->sec.tap);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 28, trafo->sec.zterra.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 29, trafo->sec.zterra.x);
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();
            // Transfere os ids obtidos para os objetos em memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
               eqpto->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  eqpto->Id = varInt;
                  }
               }

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrTrafo3E(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_TrafoId"),               varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),                varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),                 varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra1Id"),              varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra2Id"),              varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra3Id"),              varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_PerdaFerro"),            varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Sbase"),                 varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ZPS0R"),                 varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ZPS0X"),                 varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ZPT0R"),                 varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ZPT0X"),                 varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ZST0R"),                 varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ZST0X"),                 varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ZPS1R"),                 varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ZPS1X"),                 varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ZPT1R"),                 varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ZPT1X"),                 varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ZST1R"),                 varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ZST1X"),                 varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol1_TipoLigacaoId"),  varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol1_TipoFaseId"),     varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol1_Vnom"),           varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol1_Snom"),           varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol1_RotFase"),        varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol1_Vtap"),           varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol1_ZterraR"),        varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol1_ZterraX"),        varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol2_TipoLigacaoId"),  varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol2_TipoFaseId"),     varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol2_Vnom"),           varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol2_Snom"),           varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol2_RotFase"),        varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol2_Vtap"),           varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol2_ZterraR"),        varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol2_ZterraX"),        varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol3_TipoLigacaoId"),  varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol3_TipoFaseId"),     varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol3_Vnom"),           varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol3_Snom"),           varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol3_RotFase"),        varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol3_Vtap"),           varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol3_ZterraR"),        varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Enrol3_ZterraX"),        varDouble,  otFloat);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrTrafo3E( :p_Trafo3EId, :p_Codigo, :p_ExtId, :p_Barra1Id, :p_Barra2Id, :p_Barra3Id, ");
         ptrQry->SQL->Add("                                 :p_PerdaFerro, :p_Sbase, ");
         ptrQry->SQL->Add("                                 :p_ZPS0R, :p_ZPS0X, :p_ZPT0R, :p_ZPT0X, :p_ZST0R, :p_ZST0X, ");
         ptrQry->SQL->Add("                                 :p_ZPS1R, :p_ZPS1X, :p_ZPT1R, :p_ZPT1X, :p_ZST1R, :p_ZST1X, ");
         ptrQry->SQL->Add("                                 :p_Enrol1_TipoLigacaoId, :p_Enrol1_TipoFaseId, :p_Enrol1_Vnom, :p_Enrol1_Snom, :p_Enrol1_RotFase, :p_Enrol1_Vtap, :p_Enrol1_ZterraR, :p_Enrol1_ZterraX,");
         ptrQry->SQL->Add("                                 :p_Enrol2_TipoLigacaoId, :p_Enrol2_TipoFaseId, :p_Enrol2_Vnom, :p_Enrol2_Snom, :p_Enrol2_RotFase, :p_Enrol2_Vtap, :p_Enrol2_ZterraR, :p_Enrol2_ZterraX,");
         ptrQry->SQL->Add("                                 :p_Enrol3_TipoLigacaoId, :p_Enrol3_TipoFaseId, :p_Enrol3_Vnom, :p_Enrol3_Snom, :p_Enrol3_RotFase, :p_Enrol3_Vtap, :p_Enrol3_ZterraR, :p_Enrol3_ZterraX ");
         ptrQry->SQL->Add("                               );");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTTrafo3E* trafo3E = (VTTrafo3E*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  0, trafo3E->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  1, trafo3E->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  2, trafo3E->Extern_id.IsEmpty() ? trafo3E->Codigo : trafo3E->Extern_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  3, trafo3E->pbarra1 ? trafo3E->pbarra1->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  4, trafo3E->pbarra2 ? trafo3E->pbarra2->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  5, trafo3E->pbarra3 ? trafo3E->pbarra3->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  6, trafo3E->perda_ferro);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  7, trafo3E->Sbase);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  8, trafo3E->zps0.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  9, trafo3E->zps0.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 10, trafo3E->zpt0.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 11, trafo3E->zpt0.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 12, trafo3E->zst0.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 13, trafo3E->zst0.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 14, trafo3E->zps1.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 15, trafo3E->zps1.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 16, trafo3E->zpt1.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 17, trafo3E->zpt1.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 18, trafo3E->zst1.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 19, trafo3E->zst1.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 20, trafo3E->pri.ligacao);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 21, trafo3E->pri.defasagem);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 22, trafo3E->pri.vnom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 23, trafo3E->pri.snom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 24, trafo3E->pri.defasagem);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 25, trafo3E->pri.tap);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 26, trafo3E->pri.zterra.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 27, trafo3E->pri.zterra.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 28, trafo3E->sec.ligacao);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 29, trafo3E->sec.defasagem);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 30, trafo3E->sec.vnom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 31, trafo3E->sec.snom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 32, trafo3E->sec.defasagem);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 33, trafo3E->sec.tap);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 34, trafo3E->sec.zterra.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 35, trafo3E->sec.zterra.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 36, trafo3E->ter.ligacao);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 37, trafo3E->ter.defasagem);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 38, trafo3E->ter.vnom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 39, trafo3E->ter.snom);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 40, trafo3E->ter.defasagem);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr, 41, trafo3E->ter.tap);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 42, trafo3E->ter.zterra.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 43, trafo3E->ter.zterra.x);

               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();
            // Transfere os ids obtidos para os objetos em memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
               eqpto->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  eqpto->Id = varInt;
                  }
               }

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_wrMutua(TList* lstEqpto)
   {
   bool                 sucesso = true;
   AnsiString           strlog;
   TOracleQuery*        ptrQry = NULL;
   int                  intNumParams;
   int                  intMaxLinhas;
   struct TBDGatewayProcParam* dbParam = NULL;
   int                  intLinhasRestantes;
   int                  intNumLinhas;
   int                  intTransferidos;
   // Arrays de dados
   int                  ndx;                       // Contador auxiliar
   Variant              varInt, varStr;            // Conversão auxiliar
   int                  intBoundLow, intBoundHigh; // Estranho: se são alocados 10 elementos, o for segue de 0 a 10 (inclusive)
   TList*               lstParam = NULL;           // Lista de parâmetros

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstEqpto)                   { SNPSTRLOGLINE(strlog); MSGERRBD_PARAMINV    return(false); }

   __try
      {
      try
         {
         lstParam = new TList();

         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_MutuaId"),               varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Codigo"),                varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_ExtId"),                 varOleStr,  otString );
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Barra1Id"),              varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Trecho1Id"),             varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Trecho1BarraSntId"),     varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Trecho1Pos"),            varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Trecho2Id"),             varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Trecho2BarraSntId"),     varInteger, otInteger);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Trecho2Pos"),            varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z0R"),                   varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z0X"),                   varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z1R"),                   varDouble,  otFloat);
         GTWDB_PARAM_CRT(lstParam, dbParam, _T("p_Z1X"),                   varDouble,  otFloat);

         // Determina as variáveis de contagem
         intLinhasRestantes = lstEqpto->Count;
         intNumParams = lstParam->Count;
         // Reajusta o tamanho dos vetores de acordo com o número máximo de linhas
         // e executa em quantos blocos forem necessários para consumir todas as linhas
         intMaxLinhas = BDGATEWAYMAXARRAYDML / intNumParams;

         // Cria os arrays de dados
         if (!ArrayVarCria(lstParam, intMaxLinhas - 1)) { SNPSTRLOGLINE(strlog); }

         // Ajusta os tipos das veriáveis auxiliares
         varInt.ChangeType(varInteger);
         varStr.ChangeType(varOleStr);

         // Declara a query para execução
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // ptrQry->OnArrayError = (TArrayErrorEvent)(&TrataOnArrayError);
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrMutua( :p_MutuaId, :p_Codigo, :p_ExtId, :p_Barra1Id, ");
         ptrQry->SQL->Add("                               :p_Trecho1Id, :p_Trecho1BarraSntId, :p_Trecho1Pos, :p_Trecho2Id, :p_Trecho2BarraSntId, :p_Trecho2Pos, ");
         ptrQry->SQL->Add("                               :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X ");
         ptrQry->SQL->Add("                             );");
         ptrQry->SQL->Add("END;");

         // Declara as variáveis referentes à query de execução
         if (!ArrayVarDOADeclVar(lstParam, ptrQry)) { SNPSTRLOGLINE(strlog); }

         intTransferidos = 0;
         while (intLinhasRestantes > 0)
            { // Loop das linhas restantes (ini)
            // Ajusta o número de linhas a ser carregado
            intNumLinhas = (intLinhasRestantes > intMaxLinhas) ? intMaxLinhas : intLinhasRestantes;
            // Prepara a próxima rodada
            intLinhasRestantes -= intNumLinhas;
            // Reajusta o tamanho dos arrays
            // O vetor deve ser redimensionado passando o Upper Bound, não o número de itens
            if (!ArrayVarRedim(lstParam, intNumLinhas - 1)) { SNPSTRLOGLINE(strlog); }
            // Carrega os dados da lista de equipamentos para os vetores
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            intBoundLow  = VarArrayLowBound(dbParam->arrVar, 1);
            intBoundHigh = VarArrayHighBound(dbParam->arrVar, 1);
            for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
               {
               VTMutua* mutua = (VTMutua*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  0, mutua->Id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  1, mutua->Codigo);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varStr,  2, mutua->Extern_id.IsEmpty() ? mutua->Codigo : mutua->Extern_id);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  3, IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  4, mutua->ptre1 ? mutua->ptre1->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  5, mutua->pbar_tre1 ? mutua->pbar_tre1->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  6, mutua->postre1);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  7, mutua->ptre2 ? mutua->ptre2->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  8, mutua->pbar_tre2 ? mutua->pbar_tre2->Id : IDINDEFINIDO);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt,  9, mutua->postre2);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 10, mutua->z0.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 11, mutua->z0.r);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 12, mutua->z1.x);
               GTWDB_PARAM_SET(lstParam, dbParam, ndx, varInt, 13, mutua->z1.r);
               }

            // Declara e vincula as variáveis
            for (int i = 0; i < intNumParams; i++)
               {
               dbParam = (struct TBDGatewayProcParam*)lstParam->Items[i];
               ptrQry->SetVariable(dbParam->strORAname, dbParam->arrVar);
               }

            // Executa a query
            ptrQry->Execute();
            // Transfere os ids obtidos para os objetos em memória
            dbParam = (struct TBDGatewayProcParam*)(lstParam->Items[0]);
            dbParam->arrVar = ptrQry->GetVariable(dbParam->strORAname);
            // Se tem somente um elemento, o Array não é considerado array e é
            // gerada a exceção EVariantInvalidArgError. O testes abaixo previnem esse erro
            if (VarType(dbParam->arrVar) == (varInteger))
               {
               VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[0];
               eqpto->Id = dbParam->arrVar;
               }
            else
               {
               // Um array é do tipo 0x2000 (varArray, em Embarcadero\Studio\15.0\include\windows\rtl\System.hpp)
               // Um Array de inteiros é do tipo 0x2003 (varArray + varInteger).
               // Porém, pode retornar um array de outros tipos, enão o teste é feito pela pra ver se não é array, primeiro
               for (ndx = intBoundLow; ndx <= intBoundHigh; ndx++)
                  {
                  VTEqpto* eqpto = (VTEqpto*)lstEqpto->Items[intTransferidos + ndx - intBoundLow];

                  varInt = VarArrayGet(dbParam->arrVar, &ndx, 0);
                  eqpto->Id = varInt;
                  }
               }

            // Ajusta o ponteiro das linhas transferidas
            intTransferidos +=  intNumLinhas;

            } // Loop das linhas restantes (fim)

         // Fecha os recordsets
         ptrQry->Close();
         // Comita a transação
         Commit();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         plog->Add(e.Message);
         DecodificaUltimoErroDB(e.Message);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry)   { delete ptrQry;     ptrQry   = NULL; }
      // Limpa os arrays
      if (!ArrayVarClean(lstParam)) { SNPSTRLOGLINE(strlog); }
      if (lstParam) { LimpaTList(lstParam); delete lstParam; lstParam = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
// As rotinas abaixo foram mantidas idênticas ao SINAP
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// O parâmetro bolIgnoraTipico indica que a escrita não deve ser efetuada
// se o arranjo for típico.
// Como essa mesma rotina deve ser utilizada para escrita de arranjos típicos,
// o flag é necessário para não escrever dados que já estão na base.
//
bool __fastcall TBDGateway::SPSNP_wrArranjo(VTArranjo *arranjo, bool bolTipico)
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

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }
   if (!arranjo)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_ARRANJOINV return(false); }

   if (arranjo->TipoCfg == arranjoMUTUA)
      return(true);

   // Assume sempre que o arranjo é novo
   intArranjoId = IDINDEFINIDO;

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
               ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrArranjo00(:p_ArranjoId, :p_TipoArranjoId, :p_Descricao, :p_ExtId, :p_Tipico, :p_SuporteId, "  );
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
               ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrArranjo01(:p_ArranjoId, :p_TipoArranjoId, :p_Descricao, :p_ExtId, :p_Tipico, "                       );
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
               ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrArranjo02(:p_ArranjoId, :p_TipoArranjoId, :p_Descricao, :p_ExtId, :p_Tipico, :p_SuporteId, "                   );
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
               ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrArranjo03(:p_ArranjoId, :p_TipoArranjoId, :p_Descricao, :p_ExtId, :p_Tipico, "                                 );
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
               ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrArranjo04(:p_ArranjoId, :p_TipoArranjoId, :p_Descricao, :p_ExtId, :p_Tipico, :p_SuporteId, "                   );
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
bool __fastcall TBDGateway::SPSNP_wrCabo(VTCabo *cabo)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   int           intCaboId;
   TOracleQuery* ptrQry = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!cabo)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_CABOINV    return(false); }

   // Assume sempre que o cabo é novo
   intCaboId = IDINDEFINIDO;

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
            ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrCaboRGMR(:p_CaboId, :p_Descricao, :p_ExtId, :p_Inom, :p_Vmin, :p_Vmax, :p_R, :p_GMR, :p_Status);");
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
            ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrCaboZ0Z1(:p_CaboId, :p_Descricao, :p_ExtId, :p_Inom, :p_Vmin, :p_Vmax, ");
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
bool __fastcall TBDGateway::SPSNP_wrSuporte(VTSuporte *suporte)
   {
   //variáveis locais
   bool          sucesso = true;
   VTArranjo*    arranjo;
   AnsiString    strlog;
   int           intSuporteId;
   TOracleQuery* ptrQry = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }

   // Atualiza Suporte
   if (!suporte)
       {
       // Como o suporte não é obrigatório, não salva o suporte, mas não gera erro
       return(true);
       }

   // Assume sempre que o suporte é novo
   intSuporteId = IDINDEFINIDO;

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // Escrita propriamente dita
         // Monta a string de execução
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrSuporte(:p_SuporteId, :p_TipoSuporteId, :p_Descricao, :p_ExtId, :p_Vmin, :p_Vmax, :p_NumPontosFix, :p_Suporte); ");
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
bool __fastcall TBDGateway::SPSNP_wrSuporteFixacao(VTSuporte* suporte)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TList*        lisPtoFixacao;
   VTPtoFixacao* ptrPtoFixacao;
   TOracleQuery* ptrQry = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }

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
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrSuporteFixacao(:p_SuporteId, :p_FixacaoItem, :p_X, :p_Y, :p_Codigo); ");
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
bool __fastcall TBDGateway::SPSNP_dlSuporteFixacao(int intSuporteId)
   {
   //variáveis locais
   bool       sucesso = true;
   AnsiString strlog;
   TOracleQuery* ptrQry = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_dlSuporteFixacao(:p_SuporteId); ");
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
bool __fastcall TBDGateway::SPSNP_wrTipoChave(TList* lstTipoChave)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   VTTipoChave*  tipo_chave;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstTipoChave)               { SNPSTRLOGLINE(strlog); MSGERRSYS_LISOBJINV  return(false); }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrTipoChave(:p_TipoChaveId, :p_Descricao, :p_Status, :p_CorId);");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_TipoChaveId", otInteger);
         ptrQry->DeclareVariable("p_Descricao"  , otString);
         ptrQry->DeclareVariable("p_Status"     , otInteger);
         ptrQry->DeclareVariable("p_CorId"      , otInteger);

         for (int i = 0; i < lstTipoChave->Count; i++)
            {
            if ((tipo_chave = (VTTipoChave*)lstTipoChave->Items[i]) == NULL) return(false);
            ptrQry->SetVariable("p_TipoChaveId" , tipo_chave->Id );
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
bool __fastcall TBDGateway::SPSNP_wrTipoRede(TList* lstTipoRede)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   VTTipoRede*   tipo_rede;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!lstTipoRede)                { SNPSTRLOGLINE(strlog); MSGERRSYS_LISOBJINV  return(false); }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_wrTipoRede(:p_TipoRedeId, :p_Codigo, :p_Descricao, :p_Vmin, :p_Vmax, :p_ColorId, :p_Status);");
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
bool __fastcall TBDGateway::SPSNP_dlTudo(void)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   VTTipoChave*  tipo_chave;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_dlTudo();");
         ptrQry->SQL->Add("END;");

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
   }

//---------------------------------------------------------------------------
bool __fastcall TBDGateway::SPSNP_rdCurvaTipica(void)
   {
   //variáveis locais
   AnsiString    strlog;
   TLOBLocator*  ptrStream = NULL;
   int           intCurvaId;
   AnsiString    strDescricao;
   int           intTipoCurvaId;
   int           intStatus;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)                 { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)      { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!bd_func)                    { SNPSTRLOGLINE(strlog); MSGERRSYS_BDFUNCINV  return(false); }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SNPGATEWAY.SPSNP_rdCurvaTipica(:p_cursor);");
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
            intCurvaId     =  ptrCursor->Field("CurvaId");
            intTipoCurvaId = ptrCursor->Field("TipoCurvaId");
            intStatus      = 0;
            strDescricao   = ptrCursor->Field("Descricao");
            ptrStream      = ptrCursor->LOBField("Pontos");
            if (!ptrStream)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_CURVAINV
               break;
               }
            if (!bd_func->DesmontaStreamCurvaTipica(ptrStream, intCurvaId, strDescricao, intTipoCurvaId, intStatus))
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
   } // SPSNP_rdCurvaTipica

//---------------------------------------------------------------------------
//eof
