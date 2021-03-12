//---------------------------------------------------------------------------
#include <vcl.h>
#include <ADODB.hpp>
#include <DB.hpp>
#include <Oracle.hpp>
#pragma hdrstop
#include "TBDfunc.h"
#include "TBDeqptoOracle.h"
#include "VTBDeqpto.h"
#include "VTBDgeral.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Cronometro\VTCronometro.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Edita\VTEdita.h"
#include "..\Importa\Monta\VTMonta.h"
#include "..\Manobra\VTLock.h"
#include "..\Manobra\VTLocked.h"
#include "..\Manobra\VTLocks.h"
#include "..\Manobra\VTManobra.h"
#include "..\Manobra\VTManobras.h"
#include "..\MontaZ\VTMontaZ.h"
#include "..\Log\VTLog.h"
#include "..\Obra\VTLixeira.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTIpe.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCanal.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTCargaNL.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCapserie.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTCluster.h"
#include "..\Rede\VTComentario.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTDominio.h"
#include "..\Rede\VTFiltro.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTMatZ.h"
#include "..\Rede\VTMatC.h"
#include "..\Rede\VTMedidor.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafoZZ.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTYref.h"
#include "..\Planejamento\VTAlternativa.h"
#include "..\Obra\VTObras.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTAcao.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTBDeqpto* NewObjBDeqptoOracle(VTApl *apl_owner, TOracleSession* ptrOracleSession)
   {
   return(new TBDeqptoOracle(apl_owner, ptrOracleSession));
   }

//---------------------------------------------------------------------------
__fastcall TBDeqptoOracle::TBDeqptoOracle(VTApl *apl_owner, TOracleSession* ptrOracleSession)
   {
   AnsiString strlog;

   //salva ponteiro
   apl        = apl_owner;
   bd_session = ptrOracleSession;
   plog       = (VTLog*)apl->GetObject(__classid(VTLog));
   bd_geral   = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      }

   if ((progresso = (VTProgresso*)apl->GetObject(__classid(VTProgresso))) == NULL)
      {
      SNPSTRLOGLINE(strlog);
      }

   if ((monta = (VTMonta*)apl->GetObject(__classid(VTMonta))) == NULL)
      {
      SNPSTRLOGLINE(strlog);
      }

   if ((bd_func = (TBDfunc*)apl->GetObject(__classid(TBDfunc))) == NULL)
      {
      SNPSTRLOGLINE(strlog);
      }
   //o objeto VTCronometro pode não existir
   cronometro = (VTCronometro*)apl->GetObject(__classid(VTCronometro));

   //o objeto VTRedes pode não existir
   redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   }

//---------------------------------------------------------------------------
__fastcall TBDeqptoOracle::~TBDeqptoOracle(void)
   {
   //destrói objetos
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::CarregaEqptosRede(VTEstudo *estudo, VTRede *rede, bool bolObra)
   {
   //variáveis locais
   bool sucesso;
   AnsiString strlog;
   AnsiString straux;
   int intRedeId = IDINDEFINIDO;

   // Se não for passado o ponteiro, assume todas as redes
   if (rede != NULL)
      {
      intRedeId = rede->Id;
      }
   //trecho
   if (progresso) progresso->Add("Leitura dos trechos");
   if (cronometro) cronometro->Start("Trecho");
   if ((sucesso = SPSNP_rdTrecho(estudo, intRedeId, bolObra)) == false) {SNPSTRLOGLINE(strlog);}
   if (cronometro) cronometro->Stop("Trecho");
   if (sucesso)
      {//trafo
      if (progresso) progresso->Add("Leitura dos trafos");
      if (cronometro) cronometro->Start("Trafo");
      if ((sucesso = SPSNP_rdTrafo(estudo, intRedeId, bolObra)) == false) {SNPSTRLOGLINE(strlog);}
      if (cronometro) cronometro->Stop("Trafo");
      }
   if (sucesso)
      {//trafo3e
      if (progresso) progresso->Add("Leitura dos trafos 3 enrolamentos");
      if (cronometro) cronometro->Start("Trafo3E");
      if ((sucesso = SPSNP_rdTrafo3E(estudo, intRedeId, bolObra)) == false) {SNPSTRLOGLINE(strlog);}
      if (cronometro) cronometro->Stop("Trafo3E");
      }
   if (sucesso)
      {//carga
      if (progresso) progresso->Add("Leitura das cargas");
      if (cronometro) cronometro->Start("Carga");
      if ((sucesso = SPSNP_rdCarga(estudo, intRedeId, bolObra)) == false) {SNPSTRLOGLINE(strlog);}
      if (cronometro) cronometro->Stop("Carga");
      }
   if (sucesso)
      {//carga IP
      if (progresso) progresso->Add("Leitura das cargas de iluminação pública");
      if (cronometro) cronometro->Start("CargaIP");
      if ((sucesso = SPSNP_rdCargaIP(estudo, intRedeId, bolObra)) == false) {SNPSTRLOGLINE(strlog);}
      if (cronometro) cronometro->Stop("CargaIP");
      }
   if (sucesso)
      {//capacitor
      if (progresso) progresso->Add("Leitura dos capacitores");
      if (cronometro) cronometro->Start("Capacitor");
      if ((sucesso = SPSNP_rdCapacitor(estudo, intRedeId, bolObra)) == false) {SNPSTRLOGLINE(strlog);}
      if (cronometro) cronometro->Stop("Capacitor");
      }
   if (sucesso)
      {//gerador
      if (progresso) progresso->Add("Leitura dos geradores");
      if (cronometro) cronometro->Start("Gerador");
      if ((sucesso = SPSNP_rdGerador(estudo, intRedeId, bolObra)) == false) {SNPSTRLOGLINE(strlog);}
      if (cronometro) cronometro->Stop("Gerador");
      }
   if (sucesso)
      {//medidor
      if (progresso) progresso->Add("Leitura dos medidores");
      if (cronometro) cronometro->Start("Medidor");
      if ((sucesso = SPSNP_rdMedidor(estudo, intRedeId, bolObra)) == false) {SNPSTRLOGLINE(strlog);}
      if (cronometro) cronometro->Stop("Medidor");
      }
   if (sucesso)
      {//canal
      if (progresso) progresso->Add("Leitura dos canais");
      if (cronometro) cronometro->Start("Canal");
      if ((sucesso = SPSNP_rdCanal(estudo, intRedeId, bolObra)) == false) {SNPSTRLOGLINE(strlog);}
      if (cronometro) cronometro->Stop("Canal");
      }
   if (sucesso)
      {//suprimento
      if (progresso) progresso->Add("Leitura dos suprimentos");
      if (cronometro) cronometro->Start("Suprimento");
      if ((sucesso = SPSNP_rdSuprimento(estudo, intRedeId, bolObra)) == false) {SNPSTRLOGLINE(strlog);}
      if (cronometro) cronometro->Stop("Suprimento");
      }
   if (sucesso)
      {//filtro
      if (progresso) progresso->Add("Leitura dos filtros");
      if (cronometro) cronometro->Start("Filtro");
      if ((sucesso = SPSNP_rdFiltro(estudo, intRedeId, bolObra)) == false) {SNPSTRLOGLINE(strlog);}
      if (cronometro) cronometro->Stop("Filtro");
      }
   if (sucesso)
      {//reator
      if (progresso) progresso->Add("Leitura dos reatores");
      if (cronometro) cronometro->Start("Reator");
      if ((sucesso = SPSNP_rdReator(estudo, intRedeId, bolObra)) == false) {SNPSTRLOGLINE(strlog);}
      if (cronometro) cronometro->Stop("Reator");
      }
    if (sucesso)
      {//trafoZZ
      if (progresso) progresso->Add("Leitura dos trafos zig zag");
      if (cronometro) cronometro->Start("TrafoZZ");
      if ((sucesso = SPSNP_rdTrafoZZ(estudo, intRedeId, bolObra)) == false) {SNPSTRLOGLINE(strlog);}
      if (cronometro) cronometro->Stop("TrafoZZ");
      }
   if (sucesso)
      {//capacitor serir
      if (progresso) progresso->Add("Leitura dos capacitores série");
      if (cronometro) cronometro->Start("CapacitorSerie");
      if ((sucesso = SPSNP_rdCapacitorSerie(estudo, intRedeId, bolObra)) == false) {SNPSTRLOGLINE(strlog);}
      if (cronometro) cronometro->Stop("CapacitorSerie");
      }
   if (sucesso)
      {//mutua
      if (progresso) progresso->Add("Leitura das mútuas");
      if (cronometro) cronometro->Start("Mutua");
      if ((sucesso = SPSNP_rdMutua(estudo, intRedeId, bolObra)) == false) {SNPSTRLOGLINE(strlog);}
      if (cronometro) cronometro->Stop("Mutua");
      }
    if (sucesso)
      {//chave
      if (progresso) progresso->Add("Leitura das chaves");
      if (cronometro) cronometro->Start("Chave");
      if ((sucesso = SPSNP_rdChave(estudo, intRedeId, bolObra)) == false) {SNPSTRLOGLINE(strlog);}
      if (cronometro) cronometro->Stop("Chave");
      }
   if (sucesso)
      {//regulador
      if (progresso) progresso->Add("Leitura dos reguladores");
      if (cronometro) cronometro->Start("Regulador");
      if ((sucesso = SPSNP_rdRegulador(estudo, intRedeId, bolObra)) == false) {SNPSTRLOGLINE(strlog);}
      if (cronometro) cronometro->Stop("Regulador");
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::CarregaEstudo(VTEstudo *estudo)
   {
   AnsiString strlog;

   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
      return(false);
      }

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
      return(false);
      }

   if (!monta)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_MONTAINV
      return(false);
      }
   if (!CarregaRedeEstudo(estudo))
      {
      SNPSTRLOGLINE(strlog);
      MSGERRBDRD_ESTUDO
      return(false);
      }

   if (!LeListaCorte(estudo, redes->LisMRede()))
      {
      SNPSTRLOGLINE(strlog);
      MSGERRBDRD_ESTUDO
      return(false);
      }

   if (!CarregaReducaoEstudo(estudo))
      {
      SNPSTRLOGLINE(strlog);
      MSGERRBDRD_RRESTUDO
      return(false);
      }

   if (estudo->Id > 0)
      {
      if (!CarregaAlternativaEstudo(estudo))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDRD_ESTUDOALTERNATIVA
         return(false);
         }
      }

   //finaliza montagem da rede
   if (! monta->TrataFimDeDados(true))
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_TRATAFIMDADOS
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::CarregaMRedeEstudo(VTEstudo *estudo, TList *lisMREDE, TList *lisMREDE_VIZ)
   {
   //variáveis locais
   AnsiString strlog;
   int        rede_id;
   TList*     lisREDE;
   VTMRede*   mrede;
   VTRede*    rede;

   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
      return(false);
      }

   if (!lisMREDE)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISMREDEINV
      return(false);
      }
   //reinicia cronometros
   if (cronometro) cronometro->Reinicia();
   if (cronometro) cronometro->Start("RedeTotal");
   //exibe progresso
   if (progresso) progresso->Start(progTEXTO);
   if (progresso) progresso->Add("Leitura das redes selecionadas");
   //cria lista temporária
   lisREDE = new TList();
   //lê as Redes de todas MRede (apenas as Redes, sem seus Eqptos)
   for (int n = 0; n < lisMREDE->Count; n++)
      {
      mrede = (VTMRede*)lisMREDE->Items[n];
      if (!SPSNP_rdRedeMRede(estudo, mrede->Id, lisREDE))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDRD_REDEMREDE
         return(false);
         }
      }
   //IMPORTANTE: as chamadas abaixo estão em BDihm e não podem sair de lá
   //lê todos os Cortes da MRede selecionadas
   //if (! LeListaCorte(estudo, lisMREDE)) return(false);
   //lê todos os Cortes da MRede vizinhas
   //if (! LeListaCorte(estudo, lisMREDE_VIZ)) return(false);
   //carrega Eqptos das Redes lidas
   CarregaRedeEstudo(estudo, lisREDE);
   //carrega Reducao das MRedes vizinhas
   CarregaReducaoEstudo(estudo, lisMREDE_VIZ);
   //destrói lista sem destruir seus objetos
   delete lisREDE;
   //finaliza montagem da rede
   if (progresso)  progresso->Add("Montagem das redes", true);
   if (cronometro) cronometro->Start("Montagem");
   monta->TrataFimDeDados(true);
   if (cronometro) cronometro->Stop("Montagem");
   //fecha janela de  progresso
   if (progresso) progresso->Stop();
   //finaliza cornometro total
   if (cronometro) cronometro->Stop("RedeTotal");
   //exibe cronometro
   if (cronometro) cronometro->Show();
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::CarregaRedeEstudo(VTEstudo *estudo)
   {
   bool       sucesso = true;
   VTRede*    rede;
   TList*     lisRede;
   AnsiString strlog;
   AnsiString straux;
   int        intEtapa = 0;
   int        intEtapaTotal = 20;

   //proteções
   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
      return(false);
      }
   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
      return(false);
      }

   if (sucesso)
      {//rede
      if (cronometro) cronometro->Start("Redes");
      sucesso = SPSNP_rdRede(estudo);
      if (cronometro) cronometro->Stop("Redes");
      if (! sucesso)
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDRD_REDE
         return(false);
         }
      }

   if (sucesso)
      {//barras
      if (cronometro) cronometro->Start("Barra");
      if (progresso)  progresso->Add("Leitura das barras");
      sucesso = SPSNP_rdBarra(estudo);
      if (cronometro) cronometro->Stop("Barra");
      if (! sucesso) {SNPSTRLOGLINE(strlog);}
      }

   if (sucesso)
      {// leitura de Barras e Redes OK: loop de leitura de Eqptos de cada rede
      lisRede = redes->LisRede();
      for (int i = 0; (sucesso)&&(i < lisRede->Count); i++)
         {
         rede = (VTRede*)lisRede->Items[i];
         //atualiza progresso, apagando o conteúdo anterior
         straux.sprintf("Leitura da rede %03d/%03d: %s", i+1, lisRede->Count,  rede->Codigo.c_str());
         if (progresso) progresso->Add(straux, true);

         //efetua leitura de Eqbar e Ligacao da Rede
         sucesso = CarregaEqptosRede(estudo, rede, false);
         //atualiza indicação de que a rede real foi carregada
         //rede->Valida = true;
         rede->MRede->Carregada = true;
         }
      }

   // Leitura dos sincronismos e manobras
   if (sucesso)
      {//Sincronismos (Locks)
      if (cronometro) cronometro->Start("Sincronismo");
      if (progresso)  progresso->Add("Leitura dos Sincronismos entre chaves", true);
      sucesso = SPSNP_rdSincronismo();
      if (cronometro) cronometro->Stop("Sincronismo");
      if (! sucesso) {SNPSTRLOGLINE(strlog);}
      }
   if (sucesso)
      {//Manobras
      if (cronometro) cronometro->Start("Manobra");
      if (progresso)  progresso->Add("Leitura das manobras");
      sucesso = SPSNP_rdManobra();
      if (cronometro) cronometro->Stop("Manobra");
      if (! sucesso) {SNPSTRLOGLINE(strlog);}
      }
   //retorna resultado
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::CarregaReducaoEstudo(VTEstudo *estudo)
   {
   //variáveis locais
   AnsiString strlog;

   //proteções
   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
      return(false);
      }

   if (!SPSNP_rdRRBarra(estudo, IDINDEFINIDO, IDINDEFINIDO))
      {
      SNPSTRLOGLINE(strlog);
      MSGERRBDRD_RRBARRA
      return(false);
      }

   if (!SPSNP_rdRRCarga(estudo, IDINDEFINIDO, IDINDEFINIDO))
      {
      SNPSTRLOGLINE(strlog);
      MSGERRBDRD_RRCARGA
      return(false);
      }

   if (!SPSNP_rdRRSuprimento(estudo, IDINDEFINIDO, IDINDEFINIDO))
      {
      SNPSTRLOGLINE(strlog);
      MSGERRBDRD_RRSUPRIMENTO
      return(false);
      }

   if (!SPSNP_rdRRTrafo(estudo, IDINDEFINIDO, IDINDEFINIDO))
      {
      SNPSTRLOGLINE(strlog);
      MSGERRBDRD_RRTRAFO
      return(false);
      }

   if (!SPSNP_rdRRYRef(estudo, IDINDEFINIDO, IDINDEFINIDO))
      {
      SNPSTRLOGLINE(strlog);
      MSGERRBDRD_RRYREF
      return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::CarregaReducaoEstudo(VTEstudo *estudo, TList *lisMREDE)
   {
   //variáveis locais
   AnsiString strlog;
   TList   *lisCORTE;
   VTMRede *mrede;
   VTCorte *corte;

   //proteções
   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
      return(false);
      }
   //loop p/ todas MRede em lisMREDE
   for (int n = 0; n < lisMREDE->Count; n++)
      {
      mrede = (VTMRede*)lisMREDE->Items[n];
      lisCORTE = mrede->LisCorte();
      if (lisCORTE->Count != 1) continue;
      corte = (VTCorte*)lisCORTE->First();
      //lê as Redes da MRedes (apenas as Redes, sem seus Eqptos)
      if (!SPSNP_rdRedeMRede(estudo, mrede->Id, NULL))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDRD_REDEMREDE
         return(false);
         }

      if (!SPSNP_rdRRBarra(estudo, mrede->Id, corte->Id))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDRD_RRBARRA
         return(false);
         }

      if (!SPSNP_rdRRCarga(estudo, mrede->Id, corte->Id))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDRD_RRCARGA
         return(false);
         }

      if (!SPSNP_rdRRSuprimento(estudo, mrede->Id, corte->Id))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDRD_RRSUPRIMENTO
         return(false);
         }

      if (!SPSNP_rdRRTrafo(estudo, mrede->Id, corte->Id))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDRD_RRTRAFO
         return(false);
         }

      if (!SPSNP_rdRRYRef(estudo, mrede->Id, corte->Id))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDRD_RRYREF
         return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::CarregaRedeEstudo(VTEstudo *estudo, TList *lisREDE)
   {
   //Variáveis
   AnsiString straux;
   AnsiString strlog;
   bool       sucesso = true;
   VTRede     *rede;

   //loop p/ todas redes
   for (int i = 0; (sucesso)&&(i < lisREDE->Count); i++)
      {
      rede = (VTRede*)lisREDE->Items[i];
      //atualiza progresso
      straux.sprintf("Leitura da rede %03d/%03d: %s", i+1, lisREDE->Count,  rede->Codigo.c_str());
      if (progresso) progresso->Add(straux, true);
      //barra
      if (cronometro) cronometro->Start("Barra");
      if (progresso)  progresso->Add("Leitura das barras");
      if ((sucesso = SPSNP_rdBarra(estudo, rede->Id)) == false) {SNPSTRLOGLINE(strlog);}
      if (cronometro) cronometro->Stop("Barra");
      //lê todos Eqptos da Rede
      if (sucesso) sucesso = CarregaEqptosRede(estudo, rede, false);
      //atualiza indicação de que a rede real foi carregada
      //rede->Valida = true;
      rede->MRede->Carregada = true;
      }

   // Leitura dos sincronismos e manobras
   if (sucesso)
      {//Sincronismos (Locks)
      if (cronometro) cronometro->Start("Sincronismo");
      if (progresso)  progresso->Add("Leitura dos Sincronismos entre chaves");
      sucesso = SPSNP_rdSincronismo();
      if (cronometro) cronometro->Stop("Sincronismo");
      if (! sucesso) {SNPSTRLOGLINE(strlog);}
      }
   if (sucesso)
      {//Manobras
      if (cronometro) cronometro->Start("Manobra");
      if (progresso)  progresso->Add("Leitura das manobras");
      sucesso = SPSNP_rdManobra();
      if (cronometro) cronometro->Stop("Manobra");
      if (! sucesso) {SNPSTRLOGLINE(strlog);}
      }
   //retorna resultado
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::CarregaAlternativaEstudo(VTEstudo *estudo)
   {
   bool       sucesso = true;
   VTRede*    rede;
   TList*     lisRede;
   AnsiString strlog;
   AnsiString straux;

   //proteções
   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
      return(false);
      }
   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
      return(false);
      }
   //reinicia cronometros
   if (cronometro) cronometro->Reinicia();
   if (cronometro) cronometro->Start("RedeTotal");
   //exibe barra de progresso
   if (progresso) progresso->Add("Leitura da base de dados");
   //barra
   if (sucesso)
      {//rede
      if (cronometro) cronometro->Start("Rede");
      if (progresso)  progresso->Add("Leitura das redes");
      sucesso = SPSNP_rdRede(estudo, IDINDEFINIDO, true);
      if (cronometro) cronometro->Stop("Rede");
      if (! sucesso)
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDRD_REDE
         return(false);
         }
      }

   if (sucesso)
      {//barras
      if (cronometro) cronometro->Start("Barra");
      if (progresso)  progresso->Add("Leitura das barras");
      sucesso = SPSNP_rdBarra(estudo, IDINDEFINIDO, true);
      if (cronometro) cronometro->Stop("Barra");
      if (! sucesso) {SNPSTRLOGLINE(strlog);}
      }

   if (sucesso)
      {
      // Carga de todos os equipamentos do estudo, independentemente da rede
      sucesso = CarregaEqptosRede(estudo, NULL, true);
      }

   if (sucesso)
      {//executa montagem final das Redes
      if (cronometro) cronometro->Start("Alternativas");
      if (progresso)  progresso->Add("Leitura das alternativas");
      if (!SPSNP_rdEstudoAlternativa(estudo))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDRD_ESTUDOALTERNATIVA
         sucesso = false;
         }
      if (cronometro) cronometro->Stop("Alternativas");
      }
   //finaliza cornometro total
   if (cronometro) cronometro->Stop("RedeTotal");
   //exibe cronometro
   if (cronometro) cronometro->Show();
   //retorna resultado
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::CriaEstudoNovo(VTEstudo *estudo, TList *lisREDE)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::LeListaCargaNL(VTEstudo *estudo, TList *lisREDE, TList* lisCargaNL, bool bolObra)
   {
   // Variáveis locais
   VTRede* rede;

   if (!estudo)     { MSGERRSYS_ESTUDOINV  return(false); }
   if (!lisREDE)    { MSGERRSYS_LISOBJINV  return(false); }
   if (!lisCargaNL) { MSGERRSYS_LISOBJINV  return(false); }

   for (int i = 0; i < lisREDE->Count; i++)
      {
      if ((rede = (VTRede*)lisREDE->Items[i]) == NULL)
         {
         MSGERRSYS_REDEINV
         return(false);
         }

      if (!SPSNP_rdCargaNL(estudo, rede->Id, bolObra))
         {
         MSGERRSYS_REDEINV
         return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::LeListaCorte(VTEstudo *estudo, TList *lisMREDE, TList *lisCORTE)
   {
   if (!estudo)
      {
      MSGERRSYS_ESTUDOINV
      return(false);
      }
   if (!lisMREDE)
      {
      MSGERRSYS_LISMREDEINV
      return(false);
      }

   return(SPSNP_rdCorteMRede(estudo, lisMREDE, lisCORTE));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::LeListaDominio(void)
   {
   return(SPSNP_rdDominio());
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::LeListaEstudo(void)
   {
   return(SPSNP_rdEstudo());
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::LeListaMRede(VTEstudo *estudo, TList *lisCORTE)
   {
   //variáveis locais
   if (!estudo)
      {
      MSGERRSYS_ESTUDOINV
      return(false);
      }
   if (!lisCORTE)
      {
      MSGERRSYS_LISCORTEINV
      return(false);
      }

   return(SPSNP_rdMRedeCorte(estudo, lisCORTE));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::LeListaRede(VTEstudo *estudo, VTDominio *dominio)
   {
   return(SPSNP_rdRedeDominio(estudo, dominio));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::LeListaRede(VTEstudo *estudo, VTTipoRede *tiporede)
   {
   AnsiString strlog;

   //variáveis locais
   int tipo_rede_id = -1;

   if (!estudo)
      {
      MSGERRSYS_ESTUDOINV
      return(false);
      }

   if (!SPSNP_rdDominio())
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISDOMINIOINV
      return(false);
      }


   //verifica se foi definido um determinado TipoRede
   if (tiporede) tipo_rede_id = tiporede->Id;

   return(SPSNP_rdRede(estudo, tipo_rede_id, false));
   }

//---------------------------------------------------------------------------
bool __fastcall  TBDeqptoOracle::LeListaRedeVizinha(VTEstudo *estudo, VTRede *rede, TList *lisVIZ)
   {
   AnsiString strlog;

   if (!rede->MRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_MREDEINV
      return(false);
      }

   return(SPSNP_rdRedeVizinha(estudo, rede->MRede->Id, lisVIZ));
   }

//---------------------------------------------------------------------------
int __fastcall TBDeqptoOracle::LeQtdeCargaRede(int rede_id)
   {
   //variáveis locais
   int count = 0;

   return(count);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::RemoveDominio(VTEstudo *estudo, VTDominio *dominio)
   {
   return(SPSNP_dlDominio(estudo, dominio));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::RemoveEstudo(VTEstudo *estudo)
   {
   return(SPSNP_dlEstudo(estudo, true));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::RemoveEstudoRede(VTEstudo *estudo, VTRede *rede)
   {
   //return(SPSNP_dlEstudoRede(estudo, rede->Codigo));
   return(SPSNP_dlEstudoRede(estudo, rede->Id));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::RemoveManobra(VTManobra* manobra)
   {
   return(SPSNP_dlManobra(manobra));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::RemoveSincronismo(VTLock* lock)
   {
   return(SPSNP_dlSincronismo(lock));
   }

//---------------------------------------------------------------------------
bool __fastcall  TBDeqptoOracle::SalvaAlternativaEstudo(VTEstudo *estudo)
   {
   //variáveis locais
   AnsiString strlog;
   TList*     lisEQP;
   VTEqpto*   eqpto;
   VTCurva*   curva;

   if (!SPSNP_dlEstudo(estudo, false))
      {
      SNPSTRLOGLINE(strlog);
      MSGERRBDDL_ESTUDOALT
      return(false);
      }

   // Atualiza status dis equipamentos das obras
   lisEQP = new TList();
   estudo->LisEqptoEstudo(lisEQP, NULL, IDINDEFINIDO);
   for (int i = 0; i < lisEQP->Count; i++)
      {
      if ((eqpto = (VTEqpto*)lisEQP->Items[i]) == NULL)
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDDL_ESTUDOALT
         return(false);
         }
      eqpto->DefineStatus(sttNOVO, true);
      //define status da Curva do Eqpto, se existir
      if ((curva = eqpto->ExisteCurva()) != NULL)
         {
         curva->DefineStatus(sttNOVO, true);
         }
      }
   delete lisEQP;

   if (!SPSNP_wrEstudoEqpto(estudo))
      {
      SNPSTRLOGLINE(strlog);
      MSGERRBDWR_ESTUDOEQUIP
      return(false);
      }

   if (!SPSNP_wrEstudoAlternativa(estudo))
      {
      SNPSTRLOGLINE(strlog);
      MSGERRBDWR_ESTUDOALT
      return(false);
      }

   if (!SPSNP_wrAlternativaObra(estudo))
      {
      SNPSTRLOGLINE(strlog);
      MSGERRBDWR_ESTUDOOBRA
      return(false);
      }

   if (!SPSNP_wrObraAcao(estudo))
      {
      SNPSTRLOGLINE(strlog);
      MSGERRBDWR_ESTUDOACAO
      return(false);
      }

   // OOPS: No access há verificação de base mínima. No Oracle será feita essa verificação?
   //grava atributos da obra somente para base Rev3.1 ou superior
   // if (VerificaBaseIgualSuperior(REV_BASE_MINIMA))
      {
      if (!SPSNP_wrObraAtribExt(estudo))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDWR_ESTUDOOBRA
         return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SalvaListaCargaNL(VTEstudo *estudo, TList* lisCargaNL, bool bolObra)
   {
   return(SPSNP_wrCargaNL(estudo, lisCargaNL, bolObra));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SalvaRedeEstudo(VTEstudo *estudo)
   {
   //variáveis locais
   AnsiString   strlog;
   VTMRede*     mrede;
   VTRede*      rede;
   VTEqpto*     eqpto;
   VTReducao*   reducao;
   VTDominio*   dominio;
   TList*       lisREDE;
   TList*       lisCLUSTER;
   TList*       lisDOMINIO;
   TList*       lisREDEDOMS;
   TList*       lisEQP = NULL;

   __try
      {
      try
         {
         // Lista auxiliar para carga de equipamentos
         lisEQP = new TList();
         // Lista auxiliar para carga domínios por rede
         lisREDEDOMS = new TList();

         if (cronometro) cronometro->Reinicia();
         if (cronometro) cronometro->Start("SalvaRedeEstudo");
         if (progresso)  progresso->Start(progTEXTO);

         if(!redes)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_REDESINV
            return(false);
            }

         if (!bd_geral)
            {
            if ((bd_geral = (VTBDgeral*)apl->GetObject(__classid(VTBDgeral))) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_BDGERALINV
               return(false);
               }
            }

         if (!bd_func)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_BDFUNCINV
            return(false);
            }

         if(!estudo)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_ESTUDOINV
            return(false);
            }

         // Se o estudo é novo, registra na base de dados antes de começar
         // a salvar o equipamentos.
         if (estudo->Novo)
            {
            redes->DefineStatus(sttNOVO, true);

            if (!SPSNP_wrEstudo(estudo))
               {
               MSGERRBDWR_ESTUDO
               return(false);
               }
            }

         // Salva dados dos Agrupamentos
         if (progresso) progresso->Add("Salvando modificações na base de dados");
         if (progresso) progresso->Add("Salvando agrupamentos");
         lisCLUSTER = redes->LisCluster();
         if (lisCLUSTER->Count > 0)
            {
            if(!SPSNP_wrAgrupamento(estudo, lisCLUSTER))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_CLUSTERS
               return(false);
               }
            }

         // !OOPS!
         // 1) Aqui precisa apagar os clusters inúteis

         // Salva dados de Redes
         lisREDE = redes->LisRede();
         // Atualiza dados de cada Rede (em memória)
         for (int i = 0; i < lisREDE->Count; i++)
            {
            rede = (VTRede*)lisREDE->Items[i];
            rede->Valida = rede->Carregada;

            if (rede->Carregada) rede->Revisao = Now();
            }
         //salva dados dos Domínios
         lisDOMINIO = redes->LisDominio();
         if (lisDOMINIO->Count > 0)
            {
            if (progresso) progresso->Add("Salvando domínios");
            if (! SPSNP_wrDominio(lisDOMINIO, false))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_DOMINIOS
               return(false);
               }
            }

         // Salva dados de Redes
         if (progresso) progresso->Add("Salvando redes");
         if (lisREDE->Count > 0)
            {
            if (!SPSNP_wrRede(estudo, lisREDE))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_REDES
               return(false);
               }
            }

         // Salva dados das barras
         // Uma mesma barra pode estar em duas redes e será salva para todas as redes
         if (progresso) progresso->Add("Salvando barras");

         lisEQP->Clear();
         // OOPS. Precisa fazer a alteração no redes para trazer as barras de chaves abertas
         // rede->LisBarra(lisEQP, true); // Carrega também as barras das chaves abertas
         redes->LisEqptoNaoIsolado(lisEQP, eqptoBARRA);
         if(!SPSNP_wrBarra(estudo, lisEQP))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBDWR_WREQUIPREDE(strlog, "barras")
            return(false);
            }

         // Associa as barras e domínios às redes
         for (int i = 0; i < lisREDE->Count; i++)
            {
            rede = (VTRede*)lisREDE->Items[i];

            lisEQP->Clear();
            rede->LisBarra(lisEQP, true); // Carrega também as barras das chaves abertas
            if(!SPSNP_wrBarraRede(estudo, rede->Id, lisEQP))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "barras (redes)")
               return(false);
               }

            // Associa os domínios à rede
            lisREDEDOMS->Clear();
            redes->LisDominio(lisREDEDOMS, rede);
            if (!SPSNP_dlRedeDominio(estudo, rede))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDDL_DOMREDE
               return(false);
               }

            for (int i = 0; i < lisREDEDOMS->Count; i++)
               {
               if ((dominio = (VTDominio*)lisREDEDOMS->Items[i]) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_DOMINIOINV
                  return(false);
                  }

               if (!SPSNP_wrRedeDominio(estudo, rede, dominio))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_DOMREDE
                  return(false);
                  }
               }
            }

         // Atualiza as barras iniciais das redes
         if (progresso) progresso->Add("Salvando barras iniciais das redes");
         for (int i = 0; i < lisREDE->Count; i++)
            {
            rede = (VTRede*)lisREDE->Items[i];

            if (rede->BarraInicial())
               {
               if(!SPSNP_wrRedeBarraIni(estudo, rede->Id, rede->BarraInicial()->Id))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_WREQUIPREDE(strlog, "barras iniciais");
                  return(false);
                  }
               }
            }

         // ---------------------
         // Salva dados dos eqbar
         // ---------------------

         // Salva dados das cargas
         if (progresso) progresso->Add("Salvando cargas");
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoCARGA);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrCarga(estudo, lisEQP))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "cargas")
               return(false);
               }
            }

         if (progresso) progresso->Add("Salvando cargas (iluminação pública)");
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoCARGA);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrCargaIP(estudo, lisEQP))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "cargas iluminação pública")
               return(false);
               }
            }

         // Salva dados dos capacitores
         if (progresso) progresso->Add("Salvando capacitores");
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoCAPACITOR);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrCapacitor(estudo, lisEQP))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "capacitores")
               return(false);
               }
            }

         // Salva dados dos geradores
         if (progresso) progresso->Add("Salvando geradores");
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoGERADOR);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrGerador(estudo, lisEQP))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "geradores")
               return(false);
               }
            }

         // Salva dados dos medidores
         if (progresso) progresso->Add("Salvando medidores");
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoMEDIDOR);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrMedidor(estudo, lisEQP))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "medidores")
               return(false);
               }
            }

         // Salva dados dos canais
         if (progresso) progresso->Add("Salvando canais");
         lisEQP->Clear();
         redes->LisEqpto(lisEQP, eqptoCANAL);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrCanal(estudo, lisEQP))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "canais")
               return(false);
               }
            }

         // Salva dados dos suprimentos
         if (progresso) progresso->Add("Salvando suprimentos");
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoSUPRIMENTO);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrSuprimento(estudo, lisEQP))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "suprimentos")
               return(false);
               }
            }

         // Salva dados dos filtros
         if (progresso) progresso->Add("Salvando filtros");
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoFILTRO);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrFiltro(estudo, lisEQP))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "filtros")
               return(false);
               }
            }

         // Salva dados dos reatores
         if (progresso) progresso->Add("Salvando reatores");
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoREATOR);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrReator(estudo, lisEQP))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "reatores")
               return(false);
               }
            }

         // Salva dados dos trafos ZZ
         if (progresso) progresso->Add("Salvando transformadores Zig-Zag");
         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoTRAFOZZ);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrTrafoZZ(estudo, lisEQP))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "trafos Zig Zag")
               return(false);
               }
            }

         // ------------------------
         // Salva dados das ligações
         // ------------------------

         //salva dados de cada Rede
         for (int n = 0; n < lisREDE->Count; n++)
            {
            // loop redes (ini)
            rede = (VTRede*)lisREDE->Items[n];
            //verifica se a Rede está carregada
            if (!rede->Carregada) continue;

            // Salva dados dos trechos
            if (progresso) progresso->Add("Salvando trechos");
            lisEQP->Clear();
            rede->LisLigacao(lisEQP, eqptoTRECHO);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrTrecho(estudo, rede->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_WREQUIPREDE(strlog, "trechos")
                  return(false);
                  }
               }

            // Salva dados dos capacitores série
            if (progresso) progresso->Add("Salvando capacitores série");
            lisEQP->Clear();
            rede->LisLigacao(lisEQP, eqptoCAPSERIE);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrCapacitorSerie(estudo, rede->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_WREQUIPREDE(strlog, "capacitores série")
                  return(false);
                  }
               }

            // Salva dados das mútuas
            if (progresso) progresso->Add("Salvando impedâncias mútuas");
            lisEQP->Clear();
            rede->LisMutua(lisEQP);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrMutua(estudo, rede->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_WREQUIPREDE(strlog, "mútuas")
                  return(false);
                  }
               }

            // Salva dados das chaves
            if (progresso) progresso->Add("Salvando chaves");
            lisEQP->Clear();
            rede->LisLigacao(lisEQP, eqptoCHAVE);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrChave(estudo, rede->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_WREQUIPREDE(strlog, "chaves")
                  return(false);
                  }
               }

            // Salva dados dos reguladores
            if (progresso) progresso->Add("Salvando reguladores");
            lisEQP->Clear();
            rede->LisLigacao(lisEQP, eqptoREGULADOR);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrRegulador(estudo, rede->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_WREQUIPREDE(strlog, "reguladores")
                  return(false);
                  }
               }

            // Salva dados dos transformadores
            if (progresso) progresso->Add("Salvando transformadores");
            lisEQP->Clear();
            rede->LisLigacao(lisEQP, eqptoTRAFO);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrTrafo(estudo, rede->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_WREQUIPREDE(strlog, "trafos")
                  return(false);
                  }
               }

            lisEQP->Clear();
            rede->LisLigacao(lisEQP, eqptoTRAFO3E);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrTrafo3E(estudo, rede->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_WREQUIPREDE(strlog, "trafos 3E")
                  return(false);
                  }
               }

            // loop redes (fim)
            }

         // IMPORTANTE: atualiza Status de todos os equipamentos
         // Como as barras e EqBars podem pertencer a mais de uma rede, a atualização
         // não pode ser feita dentro de cada loop.
         // Assim, para uniformizar o código, todas as atualizações são feitas ao final
         bd_func->DefineStatusEqpto(lisREDE, false);

         lisEQP->Clear();
         redes->LisEqptoNaoIsolado(lisEQP, eqptoBARRA);
         redes->LisEqptoNaoIsolado(lisEQP, eqptoEQBAR);
         redes->LisEqptoNaoIsolado(lisEQP, eqptoLIGACAO);
         redes->LisEqptoNaoIsolado(lisEQP, eqptoMUTUA);
         bd_func->DefineStatusEqpto(lisEQP, false);

         // Redefine os passos finais
         // Aumentar o número final, se aumentarem os passos finais
         if (progresso) progresso->Add("Fase final da transferência");

         // Salva as reduções da rede
         if (progresso) progresso->Add("Salvando reduções");
         if (cronometro) cronometro->Start("SalvaReducoes");
         if (!SalvaReducoes(estudo))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBDWR_WREQUIPREDE(strlog, "reduções")
            return(false);
            }
         if (cronometro) cronometro->Stop("SalvaReducoes");

         // Salvando manobras
         if (progresso) progresso->Add("Salvando manobras");
         if (cronometro) cronometro->Start("SalvaManobras");
         if (!SPSNP_wrSincronismo(estudo))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBDWR_WREQUIPREDE(strlog, "Sincronismo")
            return(false);
            }
         if (!SPSNP_wrManobra(estudo))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBDWR_WREQUIPREDE(strlog, "Manobras")
            return(false);
            }
         if (cronometro) cronometro->Stop("SalvaManobras");

         if (progresso) progresso->Add("Removendo itens eliminados na edição");
         if (cronometro) cronometro->Start("Removendo itens eliminados na edição");
         if (!TrataLixeira(estudo))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBDWR_LIXEIRA
            return(false);
            }
         if (cronometro) cronometro->Stop("Removendo itens eliminados na edição");

         if (cronometro) cronometro->Stop("SalvaRedeEstudo");
         if (cronometro) cronometro->Show();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         }
      }
   __finally
      {
      if (progresso) progresso->Stop();

      //destrói lista sem destruir seus objetos
      if (lisEQP)      { delete lisEQP;      lisEQP = NULL;      }
      if (lisREDEDOMS) { delete lisREDEDOMS; lisREDEDOMS = NULL; }
      }

   Aviso("Dados salvos com sucesso - Modificar essa mensagem para o local correto.");
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SalvaRedeEstudoComo(VTEstudo *estudo)
   {
   Aviso("TBDeqptoOracle::SalvaRedeEstudoComo() - falta implementar");
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SalvaListaDominio(TList *lisDOM)
   {
   return(SPSNP_wrDominio(lisDOM));
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_rdDominio(void)
   {
   //variáveis locais
   bool          sucesso = true;
   int           tipo_rede_id;
   strDOMINIO    str_dominio;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;
   VTDominio*    dominio = NULL;

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

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdDominio(:p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor"  , otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // !OOPS! Há realmente necessida de haver a indicação "AlteradoObra"?
            str_dominio.obra         = false;
            str_dominio.dominio_id   = ptrCursor->Field("DominioId");
            str_dominio.codigo       = ptrCursor->Field("Codigo");
            str_dominio.data_criacao = ptrCursor->FieldAsDate(System::String("DataCriacao"));
            if ((dominio = (VTDominio*)monta->InsereDominio(str_dominio)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDRD_DOMINIO
               sucesso = false;
               break;
               }
            dominio->DefineStatus(sttNOVO,     false);
            dominio->DefineStatus(sttALTERADO, false);

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
   } // SPSNP_rdDominio

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_rdRede(VTEstudo* estudo, int intTipoRedeId, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   strREDE       str_rede;
   strCLUSTER    str_cluster;
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdRede(:p_EstudoId, :p_TipoRedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_TipoRedeId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_TipoRedeId", intTipoRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();

         if (!SPSNP_rdRedeCursor(estudo, ptrCursor))
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
bool __fastcall TBDeqptoOracle::SPSNP_rdRedeDominio(VTEstudo* estudo, VTDominio* dominio)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   strREDE       str_rede;
   strCLUSTER    str_cluster;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   if (!bd_session)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SESSION
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

   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
      return(false);
      }

   if (!dominio)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_DOMINIOINV
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdRedeDominio(:p_EstudoId, :p_DominioId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_DominioId", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_DominioId", dominio->Id);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();

         if (!SPSNP_rdRedeCursor(estudo, ptrCursor))
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
   } // SPSNP_rdRedeDominio

//---------------------------------------------------------------------------
// Função que decodifica o cursor retornado pelas diversas funções de leitura
// de rede.
bool __fastcall TBDeqptoOracle::SPSNP_rdRedeCursor(VTEstudo* estudo, TOracleQuery* ptrCursor, TList *lisRedeAux)
   {
   //variáveis locais
   bool          sucesso = true;
   VTRede*       rede = NULL;
   AnsiString    strlog;
   strREDE       str_rede;
   strCLUSTER    str_cluster;
   VTCluster*    cluster;

   if (!ptrCursor)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Cursor inválido.");
      return(false);
      }

   if (!bd_func)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_BDFUNCINV
      return(false);
      }

   try
      {
      while (!ptrCursor->Eof)
         {
         // loop registros (begin)
         // preenche estrutura strREDE
         str_rede.rede_id            = ptrCursor->Field("RedeId");
         str_rede.tiporede.id        = ptrCursor->Field("TipoRedeId");
         str_rede.mrede_id           = ptrCursor->Field("MRedeId");
         str_rede.tiporede.segmento  = str_rede.tiporede.id;
         str_rede.tiporede.codigo    = ptrCursor->Field("TipoRedeDescricao");
         str_rede.mrede_redutivel    = bool(int(ptrCursor->Field("Redutivel")));
         str_rede.estatica           = bool(int(ptrCursor->Field("Estatica")));
         str_rede.radial             = bool(int(ptrCursor->Field("Radial")));
         str_rede.equivalente        = bool(int(ptrCursor->Field("Equivalente")));
         str_rede.codigo             = ptrCursor->Field("RedeCodigo");
         str_rede.extern_id          = ptrCursor->Field("RedeExtId");
         str_rede.barini_id          = ptrCursor->Field("BarraIniId");
         str_rede.cluster_id         = ptrCursor->Field("AgrupamentoId");
         str_rede.obra               = bool(int(ptrCursor->Field("AlteradoObra")));
         str_rede.area_esq.x1        = ptrCursor->Field("EsqX1");
         str_rede.area_esq.y1        = ptrCursor->Field("EsqY1");
         str_rede.area_esq.x2        = ptrCursor->Field("EsqX2");
         str_rede.area_esq.y2        = ptrCursor->Field("EsqY2");
         str_rede.color              = (TColor)((int)ptrCursor->Field("ColorId"));

         // Essa rede aparecerá quantas vezes forem os domínios de que ela participa
         // -1 indica que não participa de nenhum domínio. O sistema pode forçar o domínio
         // padrão (que na base está cadastrado como zero?
         str_rede.dominio_id         = ptrCursor->Field("DominioId");

         // Antes de inserir a rede, verifica se deve inserir o Cluster
         str_cluster.cluster_id = str_rede.cluster_id;
         str_cluster.obra       = str_rede.obra;
         if (str_rede.cluster_id != IDINDEFINIDO)
            {
            str_cluster.codigo = ptrCursor->Field("AgrupamentoCodigo");
            if ((cluster = (VTCluster*)monta->InsereCluster(str_cluster)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               SNPSTRLOG(str_cluster.codigo);
               MSGERRSYS_CLUSTERINV
               sucesso = false;
               break;
               }
            else
               {
               cluster->DefineStatus(sttNOVO | sttALTERADO, false);
               }
            }

         // Comentário do equipamento
         str_rede.comentario.dx         = ptrCursor->Field("DxComentario");
         str_rede.comentario.dy         = ptrCursor->Field("DyComentario");
         str_rede.comentario.comentario = ptrCursor->Field("Comentario");

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
            bd_func->DefineStatusEqpto(rede, estudo->Novo);
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
bool __fastcall TBDeqptoOracle::SPSNP_rdRedeVizinha(VTEstudo* estudo, int intRedeId, TList *lisVIZ)
   {
   //variáveis locais
   bool          sucesso = true;
   VTRede*       rede;
   AnsiString    strlog;
   strREDE       str_rede;
   strCLUSTER    str_cluster;
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

   if (!lisVIZ)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Lista de redes vizinhas inválida.");
      return(false);
      }

   __try
      {
      try
         {
         ptrQry    = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         ptrCursor = new TOracleQuery(NULL); ptrCursor->Session = bd_session;

         lisVIZ->Clear();

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdRedeVizinha(:p_EstudoId, :p_RedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();

         if (!SPSNP_rdRedeCursor(estudo, ptrCursor, lisVIZ))
            {
            SNPSTRLOGLINE(strlog);
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
   } // SPSNP_rdRedeVizinha

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_rdBarra(VTEstudo* estudo, int intRedeId, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTBarra*      barra;
   AnsiString    strlog;
   strBARRA      str_barra;
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdBarra(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId"  , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId"  , intRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();

         if (!SPSNP_rdBarraCursor(estudo, ptrCursor))
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
bool __fastcall TBDeqptoOracle::SPSNP_rdBarraCursor(VTEstudo* estudo, TOracleQuery* ptrCursor)
   {
   //variáveis locais
   bool          sucesso = true;
   VTBarra*      barra;
   AnsiString    strlog;
   strBARRA      str_barra;

   if (!ptrCursor)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Cursor inválido.");
      return(false);
      }

   if (!bd_func)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_BDFUNCINV
      return(false);
      }

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
         str_barra.esq.x1    = ptrCursor->Field("EsqX1");
         str_barra.esq.y1    = ptrCursor->Field("EsqY1");
         str_barra.esq.x2    = ptrCursor->Field("EsqX2");
         str_barra.esq.y2    = ptrCursor->Field("EsqY2");
         str_barra.obra      = bool(int(ptrCursor->Field("AlteradoObra")));

         // Comentário do equipamento
         str_barra.comentario.dx         = ptrCursor->Field("DxComentario");
         str_barra.comentario.dy         = ptrCursor->Field("DyComentario");
         str_barra.comentario.comentario = ptrCursor->Field("Comentario");

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
            bd_func->DefineStatusEqpto(barra, estudo->Novo);
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
bool __fastcall TBDeqptoOracle::SPSNP_rdTrecho(VTEstudo* estudo, int intRedeId, bool bolObra)
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
   int           ndxBarra;
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

   if (!bd_geral)
      {
      if ((bd_geral = (VTBDgeral*)apl->GetObject(__classid(VTBDgeral))) == NULL)
         {
         SNPSTRLOGLINE(strlog);
         }
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

         ptrQry->ReadBuffer    = ORACLE_DRIVER_BUFFER_SIZE;
         ptrCursor->ReadBuffer = ORACLE_DRIVER_BUFFER_SIZE;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdTrecho(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         ndxBarra = 0;
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            // preenche estrutura strTRECHO
            if (ndxBarra == 0)
               {
               str_trecho.tre_id      = ptrCursor->Field("TrechoId");
               str_trecho.rede_id     = ptrCursor->Field("RedeId");
               str_trecho.codigo      = ptrCursor->Field("Codigo");
               str_trecho.extern_id   = ptrCursor->Field("ExtId");
               str_trecho.comp_km     = ptrCursor->Field("Comprimento");
               str_trecho.obra        = bool(int(ptrCursor->Field("AlteradoObra")));

               str_trecho.bar1_id     = ptrCursor->Field("BarraId");
               str_trecho.esq.posbar1 = ptrCursor->Field("PosBar");

               // Comentário do equipamento
               str_trecho.comentario.dx         = ptrCursor->Field("DxComentario");
               str_trecho.comentario.dy         = ptrCursor->Field("DyComentario");
               str_trecho.comentario.comentario = ptrCursor->Field("Comentario");
               }
            else
               {
               str_trecho.bar2_id     = ptrCursor->Field("BarraId");
               str_trecho.esq.posbar2 = ptrCursor->Field("PosBar");

               intArranjoId           = ptrCursor->Field("ArranjoId");
               intTipoArranjoId       = ptrCursor->Field("TipoArranjoId");
               bolTipico              = bool(int(ptrCursor->Field("Tipico")));
               //insere em Monta
               if (cronometro) cronometro->Start("TrechoArranjo");
               if ((arranjo = bd_geral->LeArranjo(intArranjoId, intTipoArranjoId, bolTipico)) == NULL)
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
                  bd_func->DefineStatusEqpto(trecho, estudo->Novo);
                  }
               if (cronometro) cronometro->Stop("TrechoInsere");
               }
            ndxBarra++;
            if (ndxBarra > 1)
               {
               ndxBarra = 0;
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
   } // SPSNP_rdTrecho

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_rdCarga(VTEstudo* estudo, int intRedeId, bool bolObra)
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

         ptrQry->ReadBuffer    = ORACLE_DRIVER_BUFFER_SIZE;
         ptrCursor->ReadBuffer = ORACLE_DRIVER_BUFFER_SIZE;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdCarga(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

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
            str_carga.obra                 = bool(int(ptrCursor->Field("AlteradoObra")));
            str_carga.modelo.icte_pu       = ptrCursor->Field("ICtePu");
            str_carga.modelo.zcte_pu       = ptrCursor->Field("ZCtePu");
            str_carga.modelo.scte_pu       = ptrCursor->Field("SCtePu");
            str_carga.num_cons.comercial   = ptrCursor->Field("NumConsCom");
            str_carga.num_cons.residencial = ptrCursor->Field("NumConsRes");
            str_carga.num_cons.industrial  = ptrCursor->Field("NumConsInd");
            str_carga.num_cons.rural       = ptrCursor->Field("NumConsRur");
            str_carga.num_cons.outros      = ptrCursor->Field("NumConsOut");
            str_carga.num_cons.a4          = ptrCursor->Field("NumConsA4");
            //código temporário: a informação de VIP e GrupoA está em Extern_id
            str_carga.vip       = false;
            str_carga.grupoA    = false;
            if (str_carga.extern_id.Pos("vip")    > 0) str_carga.vip    = true;
            if (str_carga.extern_id.Pos("grupoA") > 0) str_carga.grupoA = true;
            str_carga.extern_id = "";
            //fim código temporário
            //zera dados de IP
            str_carga.ip.pot_kw            = 0;
            str_carga.ip.fat_pot           = 0;
            str_carga.ip.num_lamp          = 0;
            //
            str_carga.bar_id               = ptrCursor->Field("BarraId");
            str_carga.esq.posbar           = ptrCursor->Field("PosBar");
            str_carga.esq.dxbar            = ptrCursor->Field("DX");
            str_carga.esq.dybar            = ptrCursor->Field("DY");

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
            str_carga.curva.status = (unsigned)ptrCursor->Field("Status");
            str_carga.curva.id     = ptrCursor->Field("ConjCurvaId");

            // Comentário do equipamento
            str_carga.comentario.dx         = ptrCursor->Field("DxComentario");
            str_carga.comentario.dy         = ptrCursor->Field("DyComentario");
            str_carga.comentario.comentario = ptrCursor->Field("Comentario");

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
               bd_func->DefineStatusEqpto(carga, estudo->Novo);
               if (carga->curva) { carga->curva->Status[sttNOVO] = estudo->Novo; }
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
bool __fastcall TBDeqptoOracle::SPSNP_rdCargaIP(VTEstudo* estudo, int intRedeId, bool bolObra)
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdCargaIP(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

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
            str_carga.bar_id               = ptrCursor->Field("BarraId");
            str_carga.esq.posbar           = ptrCursor->Field("PosBar");
            str_carga.esq.dxbar            = ptrCursor->Field("DX");
            str_carga.esq.dybar            = ptrCursor->Field("DY");
            str_carga.ip.pot_kw            = ptrCursor->Field("Pnom");
            str_carga.ip.fat_pot           = ptrCursor->Field("FatPot");
            //str_carga.ip.num_lamp          = ptrCursor->Field("NumLamp");
            str_carga.obra                 = bool(int(ptrCursor->Field("AlteradoObra")));
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
            str_carga.curva.status = (unsigned)ptrCursor->Field("Status");
            str_carga.curva.id     = ptrCursor->Field("ConjCurvaId");

            // Comentário do equipamento
            str_carga.comentario.dx         = ptrCursor->Field("DxComentario");
            str_carga.comentario.dy         = ptrCursor->Field("DyComentario");
            str_carga.comentario.comentario = ptrCursor->Field("Comentario");

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
               bd_func->DefineStatusEqpto(carga, estudo->Novo);
               if (carga->curva) { carga->curva->Status[sttNOVO] = estudo->Novo; }
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
bool __fastcall TBDeqptoOracle::SPSNP_rdCargaNL(VTEstudo* estudo, int intRedeId, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTCargaNL*    cargaNL = NULL;
   AnsiString    strlog;
   strCARGANL    str_cargaNL;
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdCargaNL(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_cargaNL.car_id                = ptrCursor->Field("cargaNLId");
            str_cargaNL.codigo                = ptrCursor->Field("Codigo");
            str_cargaNL.extern_id             = ptrCursor->Field("ExtId");
            str_cargaNL.obra                  = bool(int(ptrCursor->Field("AlteradoObra")));
            str_cargaNL.bar_id                = ptrCursor->Field("BarraId");
            str_cargaNL.esq.posbar            = ptrCursor->Field("PosBar");
            str_cargaNL.esq.dxbar             = ptrCursor->Field("DX");
            str_cargaNL.esq.dybar             = ptrCursor->Field("DY");
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
            str_cargaNL.comentario.dx         = ptrCursor->Field("DxComentario");
            str_cargaNL.comentario.dy         = ptrCursor->Field("DyComentario");
            str_cargaNL.comentario.comentario = ptrCursor->Field("Comentario");

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
               bd_func->DefineStatusEqpto(cargaNL, estudo->Novo);
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
bool __fastcall TBDeqptoOracle::SPSNP_rdCapacitor(VTEstudo* estudo, int intRedeId, bool bolObra)
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdCapacitor(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

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
            str_capacitor.obra         = bool(int(ptrCursor->Field("AlteradoObra")));

            str_capacitor.bar_id       = ptrCursor->Field("BarraId");
            str_capacitor.esq.posbar   = ptrCursor->Field("PosBar");
            str_capacitor.esq.dxbar    = ptrCursor->Field("DX");
            str_capacitor.esq.dybar    = ptrCursor->Field("DY");

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
            str_capacitor.curva.status = (unsigned)ptrCursor->Field("Status");
            str_capacitor.curva.id     = ptrCursor->Field("ConjCurvaId");

            // Comentário do equipamento
            str_capacitor.comentario.dx         = ptrCursor->Field("DxComentario");
            str_capacitor.comentario.dy         = ptrCursor->Field("DyComentario");
            str_capacitor.comentario.comentario = ptrCursor->Field("Comentario");

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
               bd_func->DefineStatusEqpto(capacitor, estudo->Novo);
               if (capacitor->curva) { capacitor->curva->Status[sttNOVO] = estudo->Novo; }
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
bool __fastcall TBDeqptoOracle::SPSNP_rdGerador(VTEstudo* estudo, int intRedeId, bool bolObra)
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdGerador(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

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
            str_gerador.obra         = bool(int(ptrCursor->Field("AlteradoObra")));

            str_gerador.bar_id       = ptrCursor->Field("BarraId");
            str_gerador.esq.posbar   = ptrCursor->Field("PosBar");
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
            str_gerador.curva.status = (unsigned)ptrCursor->Field("Status");
            str_gerador.curva.id     = ptrCursor->Field("ConjCurvaId");

            // Comentário do equipamento
            str_gerador.comentario.dx         = ptrCursor->Field("DxComentario");
            str_gerador.comentario.dy         = ptrCursor->Field("DyComentario");
            str_gerador.comentario.comentario = ptrCursor->Field("Comentario");

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
               bd_func->DefineStatusEqpto(gerador, estudo->Novo);
               if (gerador->curva) { gerador->curva->Status[sttNOVO] = estudo->Novo; }
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
bool __fastcall TBDeqptoOracle::SPSNP_rdMedidor(VTEstudo* estudo, int intRedeId, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTMedidor*    medidor = NULL;
   AnsiString    strlog;
   strMEDIDOR    str_medidor;
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdMedidor(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_medidor.medidor_id = ptrCursor->Field("MedidorId");
            str_medidor.codigo     = ptrCursor->Field("Codigo");
            str_medidor.extern_id  = ptrCursor->Field("ExtId");
            str_medidor.obra       = bool(int(ptrCursor->Field("AlteradoObra")));

            str_medidor.bar_id     = ptrCursor->Field("BarraId");
            str_medidor.esq.posbar = ptrCursor->Field("PosBar");
            str_medidor.esq.dxbar  = ptrCursor->Field("DX");
            str_medidor.esq.dybar  = ptrCursor->Field("DY");

            // Comentário do equipamento
            str_medidor.comentario.dx         = ptrCursor->Field("DxComentario");
            str_medidor.comentario.dy         = ptrCursor->Field("DyComentario");
            str_medidor.comentario.comentario = ptrCursor->Field("Comentario");

            //insere em Monta
            if ((medidor = (VTMedidor*)monta->InsereMedidor(str_medidor)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_MEDIDORINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(medidor, estudo->Novo);
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
   } // SPSNP_rdMedidor

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_rdCanal(VTEstudo* estudo, int intRedeId, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTCanal*      canal = NULL;
   AnsiString    strlog;
   strCANAL      str_canal;
   TStream*      ptrStream;
   strCURHEADER  curhdr;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;
   bool          bolTipica;

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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdCanal(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_canal.medidor_id      = ptrCursor->Field("MedidorId");
            str_canal.codigo          = ptrCursor->Field("Codigo");
            str_canal.tipo_canal      = ptrCursor->Field("TipoCanalId");
            str_canal.tolerancia_perc = ptrCursor->Field("Sigma");
            str_canal.obra            = bool(int(ptrCursor->Field("AlteradoObra")));

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
               if (!bd_func->DesmontaStreamCurvaEquipamento(ptrStream, &(str_canal.curva), curhdr))
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
            str_canal.curva.tipica = bolTipica;
            str_canal.curva.status = (unsigned)ptrCursor->Field("Status");
            str_canal.curva.id     = ptrCursor->Field("ConjCurvaId");

            //insere em Monta
            if ((canal = (VTCanal*)monta->InsereMedidorCanal(str_canal)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_CANALINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(canal, estudo->Novo);
               if (canal->Curva) { canal->Curva->Status[sttNOVO] = estudo->Novo; }
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
bool __fastcall TBDeqptoOracle::SPSNP_rdSuprimento(VTEstudo* estudo, int intRedeId, bool bolObra)
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdSuprimento(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

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
            str_suprimento.bar_id     = ptrCursor->Field("BarraId");
            str_suprimento.esq.posbar = ptrCursor->Field("PosBar");
            str_suprimento.esq.dxbar  = ptrCursor->Field("DX");
            str_suprimento.esq.dybar  = ptrCursor->Field("DY");
            str_suprimento.obra       = bool(int(ptrCursor->Field("AlteradoObra")));
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
            str_suprimento.curva.status = (unsigned)ptrCursor->Field("Status");
            str_suprimento.curva.id     = ptrCursor->Field("ConjCurvaId");

            // Comentário do equipamento
            str_suprimento.comentario.dx         = ptrCursor->Field("DxComentario");
            str_suprimento.comentario.dy         = ptrCursor->Field("DyComentario");
            str_suprimento.comentario.comentario = ptrCursor->Field("Comentario");

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
               bd_func->DefineStatusEqpto(suprimento, estudo->Novo);
               if (suprimento->curva) { suprimento->curva->Status[sttNOVO] = estudo->Novo; }
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
bool __fastcall TBDeqptoOracle::SPSNP_rdFiltro(VTEstudo* estudo, int intRedeId, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTFiltro*     filtro = NULL;
   AnsiString    strlog;
   strFILTRO     str_filtro;
   TStream*      ptrStream;
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdFiltro(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

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
            str_filtro.obra       = bool(int(ptrCursor->Field("AlteradoObra")));

            str_filtro.bar_id     = ptrCursor->Field("BarraId");
            str_filtro.esq.posbar = ptrCursor->Field("PosBar");
            str_filtro.esq.dxbar  = ptrCursor->Field("DX");
            str_filtro.esq.dybar  = ptrCursor->Field("DY");

            // Comentário do equipamento
            str_filtro.comentario.dx         = ptrCursor->Field("DxComentario");
            str_filtro.comentario.dy         = ptrCursor->Field("DyComentario");
            str_filtro.comentario.comentario = ptrCursor->Field("Comentario");

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
               bd_func->DefineStatusEqpto(filtro, estudo->Novo);
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
bool __fastcall TBDeqptoOracle::SPSNP_rdReator(VTEstudo* estudo, int intRedeId, bool bolObra)
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdReator(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

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
            str_reator.obra         = bool(int(ptrCursor->Field("AlteradoObra")));

            str_reator.bar_id       = ptrCursor->Field("BarraId");
            str_reator.esq.posbar   = ptrCursor->Field("PosBar");
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
            str_reator.curva.status = (unsigned)ptrCursor->Field("Status");
            str_reator.curva.id     = ptrCursor->Field("ConjCurvaId");

            // Comentário do equipamento
            str_reator.comentario.dx         = ptrCursor->Field("DxComentario");
            str_reator.comentario.dy         = ptrCursor->Field("DyComentario");
            str_reator.comentario.comentario = ptrCursor->Field("Comentario");

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
               bd_func->DefineStatusEqpto(reator, estudo->Novo);
               if (reator->curva) { reator->curva->Status[sttNOVO] = estudo->Novo; }
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
bool __fastcall TBDeqptoOracle::SPSNP_rdTrafoZZ(VTEstudo* estudo, int intRedeId, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTTrafoZZ*    trafoZZ = NULL;
   AnsiString    strlog;
   strTRAFOZZ    str_trafoZZ;
   TStream*      ptrStream;
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdTrafoZZ(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

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
            str_trafoZZ.obra       = bool(int(ptrCursor->Field("AlteradoObra")));

            str_trafoZZ.bar_id     = ptrCursor->Field("BarraId");
            str_trafoZZ.esq.posbar = ptrCursor->Field("PosBar");
            str_trafoZZ.esq.dxbar  = ptrCursor->Field("DX");
            str_trafoZZ.esq.dybar  = ptrCursor->Field("DY");

            // Comentário do equipamento
            str_trafoZZ.comentario.dx         = ptrCursor->Field("DxComentario");
            str_trafoZZ.comentario.dy         = ptrCursor->Field("DyComentario");
            str_trafoZZ.comentario.comentario = ptrCursor->Field("Comentario");

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
               bd_func->DefineStatusEqpto(trafoZZ, estudo->Novo);
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
bool __fastcall TBDeqptoOracle::SPSNP_rdCapacitorSerie(VTEstudo* estudo, int intRedeId, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTCapserie*   capserie = NULL;
   AnsiString    strlog;
   strCAPSERIE   str_capserie;
   int           ndxBarra;
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdCapacitorSerie(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

         ndxBarra = 0;
         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            // preenche estrutura strCAPSERIE
            if (ndxBarra == 0)
               {
               str_capserie.id          = ptrCursor->Field("CapacitorSerieId");
               str_capserie.rede_id     = ptrCursor->Field("RedeId");
               str_capserie.codigo      = ptrCursor->Field("Codigo");
               str_capserie.extern_id   = ptrCursor->Field("ExtId");

               str_capserie.vnom        = ptrCursor->Field("Vnom");
               str_capserie.snom        = ptrCursor->Field("Snom");
               str_capserie.z.r         = ptrCursor->Field("ZR");
               str_capserie.z.x         = ptrCursor->Field("ZX");
               str_capserie.obra        = bool(int(ptrCursor->Field("AlteradoObra")));

               str_capserie.bar1_id     = ptrCursor->Field("BarraId");
               str_capserie.esq.posbar1 = ptrCursor->Field("PosBar");

               // Comentário do equipamento
               str_capserie.comentario.dx         = ptrCursor->Field("DxComentario");
               str_capserie.comentario.dy         = ptrCursor->Field("DyComentario");
               str_capserie.comentario.comentario = ptrCursor->Field("Comentario");

               }
            else
               {
               str_capserie.bar2_id     = ptrCursor->Field("BarraId");
               str_capserie.esq.posbar2 = ptrCursor->Field("PosBar");

               if ((capserie = (VTCapserie*)monta->InsereCapserie(str_capserie)) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CAPSERIEINV
                  sucesso = false;
                  break;
                  }
               else
                  {
                  bd_func->DefineStatusEqpto(capserie, estudo->Novo);
                  }
               }
            ndxBarra++;
            if (ndxBarra > 1)
               {
               ndxBarra = 0;
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
bool __fastcall TBDeqptoOracle::SPSNP_rdMutua(VTEstudo* estudo, int intRedeId, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTMutua*      mutua = NULL;
   AnsiString    strlog;
   strMUTUA      str_mutua;
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdMutua(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

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
            str_mutua.obra         = bool(int(ptrCursor->Field("AlteradoObra")));

            // preenche estrutura strMUTUA
            str_mutua.tre1_id      = ptrCursor->Field("Trecho1Id");
            str_mutua.tre1_bar_id  = ptrCursor->Field("Trecho1BarraSntId");
            str_mutua.postre1      = ptrCursor->Field("Trecho1Pos");
            str_mutua.tre2_id      = ptrCursor->Field("Trecho2Id");
            str_mutua.tre2_bar_id = ptrCursor->Field("Trecho2BarraSntId");
            str_mutua.postre2      = ptrCursor->Field("Trecho2Pos");
            str_mutua.z0.r         = ptrCursor->Field("Z0R");
            str_mutua.z0.x         = ptrCursor->Field("Z0X");
            str_mutua.z1.r         = ptrCursor->Field("Z1R");
            str_mutua.z1.x         = ptrCursor->Field("Z1X");

            // Comentário do equipamento
            str_mutua.comentario.dx         = ptrCursor->Field("DxComentario");
            str_mutua.comentario.dy         = ptrCursor->Field("DyComentario");
            str_mutua.comentario.comentario = ptrCursor->Field("Comentario");

            if ((mutua = (VTMutua*)monta->InsereMutua(str_mutua)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_MUTUAINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(mutua, estudo->Novo);
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
bool __fastcall TBDeqptoOracle::SPSNP_rdChave(VTEstudo* estudo, int intRedeId, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTChave*      chave = NULL;
   AnsiString    strlog;
   strCHAVE      str_chave;
   int           ndxBarra;
   int           offset;
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdChave(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

         ndxBarra = 0;
         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)

            // preenche estrutura strCHAVE
            if (ndxBarra == 0)
               {
               str_chave.chv_id       = ptrCursor->Field("ChaveId");
               str_chave.rede_id      = ptrCursor->Field("RedeId");
               str_chave.codigo       = ptrCursor->Field("Codigo");
               str_chave.extern_id    = ptrCursor->Field("ExtId");
               str_chave.tipochave.id = ptrCursor->Field("TipoChaveId");
               str_chave.estado       = ptrCursor->Field("Estado");
               str_chave.corn         = ptrCursor->Field("Inom");
               str_chave.obra         = bool(int(ptrCursor->Field("AlteradoObra")));
               //código temporário: a informação de EloFusivel está em Extern_id
               if ((offset = str_chave.extern_id.Pos("_elo") - 1) > 1)
                  {
                  str_chave.elo_fusivel = str_chave.extern_id.SubString(1, offset);
                  str_chave.extern_id   = "";
                  }

               str_chave.bar1_id      = ptrCursor->Field("BarraId");
               str_chave.esq.posbar1  = ptrCursor->Field("PosBar");

               // Comentário do equipamento
               str_chave.comentario.dx         = ptrCursor->Field("DxComentario");
               str_chave.comentario.dy         = ptrCursor->Field("DyComentario");
               str_chave.comentario.comentario = ptrCursor->Field("Comentario");
               // OOPS estruturas VTMonta
               // str_chave.Telecomando           = (bool)ptrCursor->Field("Telecomando");
               // str_chave.Protecao              = (bool)ptrCursor->Field("Protecao");
               // str_chave.TecnoCom              = ptrCursor->Field("TecnoCom");
               }
            else
               {
               str_chave.bar2_id     = ptrCursor->Field("BarraId");
               str_chave.esq.posbar2 = ptrCursor->Field("PosBar");

               if ((chave = (VTChave*)monta->InsereChave(str_chave)) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CHAVEINV
                  sucesso = false;
                  break;
                  }
               else
                  {
                  bd_func->DefineStatusEqpto(chave, estudo->Novo);
                  }
               }
            ndxBarra++;
            if (ndxBarra > 1)
               {
               ndxBarra = 0;
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
bool __fastcall TBDeqptoOracle::SPSNP_rdRegulador(VTEstudo* estudo, int intRedeId, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTRegulador*  regulador = NULL;
   AnsiString    strlog;
   strREGULADOR  str_regulador;
   int           ndxBarra;
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdRegulador(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

         ndxBarra = 0;
         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            // preenche estrutura strREGULADOR
            if (ndxBarra == 0)
               {
               str_regulador.id                  = ptrCursor->Field("ReguladorId");
               str_regulador.rede_id             = ptrCursor->Field("RedeId");
               str_regulador.codigo              = ptrCursor->Field("Codigo");
               str_regulador.extern_id           = ptrCursor->Field("ExtId");
               str_regulador.ligacao             = ptrCursor->Field("TipoLigacaoId");
//               str_regulador.ajuste_auto         = bool(int(ptrCursor->Field("AjusteAuto")));
               str_regulador.snom_mva            = ptrCursor->Field("Snom");
               str_regulador.by_pass             = bool(int(ptrCursor->Field("Bypass")));
               str_regulador.var_tensao          = ptrCursor->Field("FaixaVar");
               str_regulador.num_passo           = ptrCursor->Field("NumPasso");
               str_regulador.cfg_fixo.passo      = ptrCursor->Field("CfgFixPassoSel");
               str_regulador.cfg_auto.bar_ref_id = ptrCursor->Field("CfgAutBarRefId");
               str_regulador.cfg_auto.v_pu       = ptrCursor->Field("CfgAutVpu");
               str_regulador.obra                = bool(int(ptrCursor->Field("AlteradoObra")));

//               str_regulador.sentido.reversivel  = bool(int(ptrCursor->Field("SentidoReversivel")));
               str_regulador.sentido.bar_ref_id  = ptrCursor->Field("SentidoBarRefId");
               str_regulador.z0.r                = ptrCursor->Field("Z0R");
               str_regulador.z0.x                = ptrCursor->Field("Z0X");
               str_regulador.z1.r                = ptrCursor->Field("Z1R");
               str_regulador.z1.x                = ptrCursor->Field("Z1X");

               str_regulador.bar1_id             = ptrCursor->Field("BarraId");
               str_regulador.esq.posbar1         = ptrCursor->Field("PosBar");

               // Comentário do equipamento
               str_regulador.comentario.dx         = ptrCursor->Field("DxComentario");
               str_regulador.comentario.dy         = ptrCursor->Field("DyComentario");
               str_regulador.comentario.comentario = ptrCursor->Field("Comentario");
               }
            else
               {
               str_regulador.bar2_id             = ptrCursor->Field("BarraId");
               str_regulador.esq.posbar2         = ptrCursor->Field("PosBar");

               if ((regulador = (VTRegulador*)monta->InsereRegulador(str_regulador)) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_REGULADORINV
                  sucesso = false;
                  break;
                  }
               else
                  {
                  bd_func->DefineStatusEqpto(regulador, estudo->Novo);
                  }
               }
            ndxBarra++;
            if (ndxBarra > 1)
               {
               ndxBarra = 0;
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
bool __fastcall TBDeqptoOracle::SPSNP_rdTrafo(VTEstudo* estudo, int intRedeId, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTTrafo*      trafo = NULL;
   AnsiString    strlog;
   strTRAFO      str_trafo;
   int           ndxEnrol;
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdTrafo(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

         ndxEnrol = 0;
         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)

            // preenche estrutura strTRAFO
            // Atenção: os enrolamentos devem vir ordenados
            if (ndxEnrol == 0)
               {
               str_trafo.trf_id             = ptrCursor->Field("TrafoId");
               str_trafo.rede_id            = ptrCursor->Field("RedeId");
               str_trafo.codigo             = ptrCursor->Field("Codigo");
               str_trafo.extern_id          = ptrCursor->Field("ExtId");
               str_trafo.obra               = bool(int(ptrCursor->Field("AlteradoObra")));

               str_trafo.auto_trafo         = bool(int(ptrCursor->Field("AutoTrafo")));
               str_trafo.perda_ferro        = ptrCursor->Field("PerdaFerro");
               str_trafo.ltc.tipo           = ptrCursor->Field("LtcTipoUtl");
               str_trafo.ltc.bar_id         = ptrCursor->Field("LtcBarRefId");
               str_trafo.ltc.v_pu           = ptrCursor->Field("LtcVpu");
               str_trafo.z0.r               = ptrCursor->Field("Z0R");
               str_trafo.z0.x               = ptrCursor->Field("Z0X");
               str_trafo.z1.r               = ptrCursor->Field("Z1R");
               str_trafo.z1.x               = ptrCursor->Field("Z1X");

               str_trafo.pri.ligacao        = ptrCursor->Field("Enrol_TipoLigacao");
               str_trafo.pri.fases          = ptrCursor->Field("Enrol_TipoFase");
               str_trafo.pri.vnom           = ptrCursor->Field("Enrol_Vnom");
               str_trafo.snom_mva           = ptrCursor->Field("Enrol_Snom");
               str_trafo.pri.defasagem      = ptrCursor->Field("Enrol_RotFase");
               str_trafo.pri.tap            = ptrCursor->Field("Enrol_Vtap");
               str_trafo.pri.zterra.r       = ptrCursor->Field("Enrol_ZterraR");
               str_trafo.pri.zterra.x       = ptrCursor->Field("Enrol_ZterraX");

               str_trafo.bar1_id            = ptrCursor->Field("BarraId");
               str_trafo.esq.posbar1        = ptrCursor->Field("PosBar");

               // Comentário do equipamento
               str_trafo.comentario.dx         = ptrCursor->Field("DxComentario");
               str_trafo.comentario.dy         = ptrCursor->Field("DyComentario");
               str_trafo.comentario.comentario = ptrCursor->Field("Comentario");
               }
            else
               {
               str_trafo.sec.ligacao        = ptrCursor->Field("Enrol_TipoLigacao");
               str_trafo.sec.fases          = ptrCursor->Field("Enrol_TipoFase");
               str_trafo.sec.vnom           = ptrCursor->Field("Enrol_Vnom");
               //str_trafo.snom_mva           = ptrCursor->Field("Enrol_Snom");
               str_trafo.sec.defasagem      = ptrCursor->Field("Enrol_RotFase");
               str_trafo.sec.tap            = ptrCursor->Field("Enrol_Vtap");
               str_trafo.sec.zterra.r       = ptrCursor->Field("Enrol_ZterraR");
               str_trafo.sec.zterra.x       = ptrCursor->Field("Enrol_ZterraX");

               str_trafo.bar2_id            = ptrCursor->Field("BarraId");
               str_trafo.esq.posbar2        = ptrCursor->Field("PosBar");

               if ((trafo = (VTTrafo*)monta->InsereTrafo(str_trafo)) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_TRAFOINV
                  sucesso = false;
                  break;
                  }
               else
                  {
                  bd_func->DefineStatusEqpto(trafo, estudo->Novo);
                  }
               }
            ndxEnrol++;
            if (ndxEnrol > 1)
               {
               ndxEnrol = 0;
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
bool __fastcall TBDeqptoOracle::SPSNP_rdTrafo3E(VTEstudo* estudo, int intRedeId, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTTrafo3E*    trafo3e = NULL;
   AnsiString    strlog;
   strTRAFO3E    str_trafo3e;
   int           ndxEnrol;
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdTrafo3e(:p_EstudoId, :p_RedeId, :p_AlteradoObra, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_RedeId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_RedeId", intRedeId);
         ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);

         ndxEnrol = 0;
         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            // preenche estrutura strTRAFO
            // Atenção: os enrolamentos devem vir ordenados
            if (ndxEnrol == 0)
               {
               str_trafo3e.trf_id             = ptrCursor->Field("Trafo3eId");
               str_trafo3e.rede_id            = ptrCursor->Field("RedeId");
               str_trafo3e.codigo             = ptrCursor->Field("Codigo");
               str_trafo3e.extern_id          = ptrCursor->Field("ExtId");
               str_trafo3e.obra               = bool(int(ptrCursor->Field("AlteradoObra")));

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

               str_trafo3e.pri.ligacao        = ptrCursor->Field("Enrol_TipoLigacao");
               str_trafo3e.pri.fases          = ptrCursor->Field("Enrol_TipoFase");
               str_trafo3e.pri.vnom           = ptrCursor->Field("Enrol_Vnom");
               str_trafo3e.pri.snom           = ptrCursor->Field("Enrol_Snom");
               str_trafo3e.pri.defasagem      = ptrCursor->Field("Enrol_RotFase");
               str_trafo3e.pri.tap            = ptrCursor->Field("Enrol_Vtap");
               str_trafo3e.pri.zterra.r       = ptrCursor->Field("Enrol_ZterraR");
               str_trafo3e.pri.zterra.x       = ptrCursor->Field("Enrol_ZterraX");

               str_trafo3e.bar1_id            = ptrCursor->Field("BarraId");
               str_trafo3e.esq.posbar1        = ptrCursor->Field("PosBar");

               // Comentário do equipamento
               str_trafo3e.comentario.dx         = ptrCursor->Field("DxComentario");
               str_trafo3e.comentario.dy         = ptrCursor->Field("DyComentario");
               str_trafo3e.comentario.comentario = ptrCursor->Field("Comentario");
               }
            else

            if (ndxEnrol == 1)
               {
               str_trafo3e.sec.ligacao        = ptrCursor->Field("Enrol_TipoLigacao");
               str_trafo3e.sec.fases          = ptrCursor->Field("Enrol_TipoFase");
               str_trafo3e.sec.vnom           = ptrCursor->Field("Enrol_Vnom");
               str_trafo3e.sec.snom           = ptrCursor->Field("Enrol_Snom");
               str_trafo3e.sec.defasagem      = ptrCursor->Field("Enrol_RotFase");
               str_trafo3e.sec.tap            = ptrCursor->Field("Enrol_Vtap");
               str_trafo3e.sec.zterra.r       = ptrCursor->Field("Enrol_ZterraR");
               str_trafo3e.sec.zterra.x       = ptrCursor->Field("Enrol_ZterraX");

               str_trafo3e.bar2_id            = ptrCursor->Field("BarraId");
               str_trafo3e.esq.posbar2        = ptrCursor->Field("PosBar");
               }
            else
               {
               str_trafo3e.ter.ligacao        = ptrCursor->Field("Enrol_TipoLigacao");
               str_trafo3e.ter.fases          = ptrCursor->Field("Enrol_TipoFase");
               str_trafo3e.ter.vnom           = ptrCursor->Field("Enrol_Vnom");
               str_trafo3e.ter.snom           = ptrCursor->Field("Enrol_Snom");
               str_trafo3e.ter.defasagem      = ptrCursor->Field("Enrol_RotFase");
               str_trafo3e.ter.tap            = ptrCursor->Field("Enrol_Vtap");
               str_trafo3e.ter.zterra.r       = ptrCursor->Field("Enrol_ZterraR");
               str_trafo3e.ter.zterra.x       = ptrCursor->Field("Enrol_ZterraX");

               str_trafo3e.bar3_id            = ptrCursor->Field("BarraId");
               str_trafo3e.esq.posbar3        = ptrCursor->Field("PosBar");

               if ((trafo3e = (VTTrafo3E*)monta->InsereTrafo3E(str_trafo3e)) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_TRAFO3EINV
                  sucesso = false;
                  break;
                  }
               else
                  {
                  bd_func->DefineStatusEqpto(trafo3e, estudo->Novo);
                  }
               }
            ndxEnrol++;
            if (ndxEnrol > 2)
               {
               ndxEnrol = 0;
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
bool __fastcall TBDeqptoOracle::SPSNP_wrAgrupamento(VTEstudo *estudo, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTCluster*    cluster;
   AnsiString    strlog;
   int           intClusterId;
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

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
      return(false);
      }

   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrAgrupamento(:p_NovoId, :p_EstudoId, :p_AgrupamentoId, :p_Codigo, :p_AlteradoObra);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoId"       , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"     , otInteger);
         ptrQry->DeclareVariable("p_AgrupamentoId", otInteger);
         ptrQry->DeclareVariable("p_Codigo"       , otString );
         ptrQry->DeclareVariable("p_AlteradoObra" , otInteger);

         for (int nc = 0; nc < lisObjRede->Count; nc++)
            {
            // loop Clusters (ini)
            cluster = (VTCluster*)lisObjRede->Items[nc];
            if (!cluster)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_CLUSTERINV
               return(false);
               }

            // Verifica se o objeto é novo ou alterado
            if ((cluster->Id < 0) || (bolObra))
               {
               intClusterId = IDINDEFINIDO;
               }
            else
               {
               intClusterId = cluster->Id;
               }

            if ((!cluster->StatusNovo()) && (!cluster->StatusAlterado()))
               {
               continue;
               }

            // Seta valores dos parâmetros
            ptrQry->SetVariable("p_NovoId"       , (cluster->StatusNovo() ? 1 : 0) );
            ptrQry->SetVariable("p_EstudoId"     , estudo->Id                      );
            ptrQry->SetVariable("p_AgrupamentoId", intClusterId                    );
            ptrQry->SetVariable("p_Codigo"       , cluster->Codigo                 );
            ptrQry->SetVariable("p_AlteradoObra" , (int)bolObra                    );

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            cluster->Id = ptrQry->GetVariable("p_AgrupamentoId");
            if (cluster->Id == IDINDEFINIDO)
               {
               // Condição de erro
               sucesso = false;
               SNPSTRLOGLINE(strlog);
               }
            // Atualiza o status do cluster
            cluster->DefineStatus(sttNOVO | sttALTERADO, false);
            // loop Clusters (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrAgrupamento

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrRede(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTRede*       rede;
   AnsiString    strlog;
   int           intRedeId;
   VTArea*       area_esq;
   VTArea*       area_utm;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrRede(:p_NovoReg, :p_EstudoId, :p_RedeId, :p_TipoRedeId, :p_AlteradoObra, :p_Show, ");
         ptrQry->SQL->Add("                        :p_Codigo, :p_ExtId, :p_MRedeId, :p_Redutivel, :p_Estatica, :p_Radial, :p_Equivalente, ");
         ptrQry->SQL->Add("                        :p_Revisao, :p_ColorId, :p_Modificador, :p_BarraIniId, :p_EsqX1, :p_EsqY1, :p_EsqX2, :p_EsqY2, ");
         ptrQry->SQL->Add("                        :p_UtmX1, :p_UtmY1, :p_UtmX2, :p_UtmY2, :p_AgrupamentoId, ");
         ptrQry->SQL->Add("                        :p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                       );"                                                   );
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"           , otInteger );
         ptrQry->DeclareVariable("p_EstudoId"          , otInteger );
         ptrQry->DeclareVariable("p_RedeId"            , otInteger );
         ptrQry->DeclareVariable("p_TipoRedeId"        , otInteger );
         ptrQry->DeclareVariable("p_AlteradoObra"      , otInteger );
         ptrQry->DeclareVariable("p_Show"              , otInteger );
         ptrQry->DeclareVariable("p_Codigo"            , otString  );
         ptrQry->DeclareVariable("p_ExtId"             , otString  );
         ptrQry->DeclareVariable("p_MRedeId"           , otInteger );
         ptrQry->DeclareVariable("p_Redutivel"         , otInteger );
         ptrQry->DeclareVariable("p_Estatica"          , otInteger );
         ptrQry->DeclareVariable("p_Radial"            , otInteger );
         ptrQry->DeclareVariable("p_Equivalente"       , otInteger );
         ptrQry->DeclareVariable("p_Revisao"           , otDate    );
         ptrQry->DeclareVariable("p_ColorId"           , otInteger );
         ptrQry->DeclareVariable("p_Modificador"       , otString  );
         ptrQry->DeclareVariable("p_BarraIniId"        , otInteger );
         ptrQry->DeclareVariable("p_EsqX1"             , otInteger );
         ptrQry->DeclareVariable("p_EsqY1"             , otInteger );
         ptrQry->DeclareVariable("p_EsqX2"             , otInteger );
         ptrQry->DeclareVariable("p_EsqY2"             , otInteger );
         ptrQry->DeclareVariable("p_UtmX1"             , otInteger );
         ptrQry->DeclareVariable("p_UtmY1"             , otInteger );
         ptrQry->DeclareVariable("p_UtmX2"             , otInteger );
         ptrQry->DeclareVariable("p_UtmY2"             , otInteger );
         ptrQry->DeclareVariable("p_AgrupamentoId"     , otInteger );
         ptrQry->DeclareVariable("p_DxComentario"      , otInteger);
         ptrQry->DeclareVariable("p_DyComentario"      , otInteger);
         ptrQry->DeclareVariable("p_Comentario"        , otString );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop redes (ini)
            rede = (VTRede*)lisObjRede->Items[n];
            if (!rede)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_REDEINV
               return(false);
               }
            // Verifica se o objeto é novo ou alterado
            if ((rede->Id < 0) || (bolObra))
               {
               intRedeId = IDINDEFINIDO;
               }
            else
               {
               intRedeId = rede->Id;
               }

            bolNovoReg = false;
            if (rede->StatusNovo())
               {
               bolNovoReg = true;
               }
            else
               {
               if (!rede->StatusAlterado())
                  {
                  continue;
                  }
               }

            // Escrita propriamente dita
            try
               {
               // Seta valores dos parâmetros
               ptrQry->SetVariable("p_NovoReg"          , (bolNovoReg ? 1 : 0));
               ptrQry->SetVariable("p_EstudoId"         , estudo->Id);
               ptrQry->SetVariable("p_RedeId"           , intRedeId);
               ptrQry->SetVariable("p_TipoRedeId"       , rede->TipoRede->Id);
               ptrQry->SetVariable("p_AlteradoObra"     , (int)bolObra);
               ptrQry->SetVariable("p_Show"             , rede->ShowAsUnsigned);
               ptrQry->SetVariable("p_Codigo"           , rede->Codigo);
               ptrQry->SetVariable("p_ExtId"            , rede->Extern_id);
               ptrQry->SetVariable("p_MRedeId"          , (rede->MRede ? rede->MRede->Id : IDINDEFINIDO));
               ptrQry->SetVariable("p_Redutivel"        , (int)(rede->MRede ? rede->MRede->Redutivel : false));
               ptrQry->SetVariable("p_Estatica"         , (int)rede->Estatica);
               ptrQry->SetVariable("p_Radial"           , (int)rede->Radial);
               ptrQry->SetVariable("p_Equivalente"      , (rede->Valida ? 0 : 1));
               // !OOPS!
               //               ptrQry->SetVariable("p_Revisao"          , rede->Revisao);
               // !OOPS! Modificar aqui
               ptrQry->SetVariable("p_Revisao"          , TDateTime::CurrentDateTime());
               ptrQry->SetVariable("p_ColorId"          , rede->Color);
               ptrQry->SetVariable("p_Modificador"      , bd_session->LogonUsername);
               ptrQry->SetVariable("p_BarraIniId"       , (rede->BarraInicialId <= 0 ? IDINDEFINIDO : rede->BarraInicialId));

               //obtém áreas da rede
               area_esq = rede->AreaEsquematico();
               area_utm = rede->AreaUtm_cm();
               ptrQry->SetVariable("p_EsqX1"            , area_esq->x1);
               ptrQry->SetVariable("p_EsqY1"            , area_esq->y1);
               ptrQry->SetVariable("p_EsqX2"            , area_esq->x2);
               ptrQry->SetVariable("p_EsqY2"            , area_esq->y2);
               ptrQry->SetVariable("p_UtmX1"            , area_utm->x1);
               ptrQry->SetVariable("p_UtmY1"            , area_utm->y1);
               ptrQry->SetVariable("p_UtmX2"            , area_utm->x2);
               ptrQry->SetVariable("p_UtmY2"            , area_utm->y2);

               ptrQry->SetVariable("p_AgrupamentoId"    , (rede->Cluster ? rede->Cluster->Id : IDINDEFINIDO));
               // !OOPS!
               // Uma rede pode fazer parte de mais de um domínio. Como será aqui?
               // Deverá ter uma função que salva uma lista de domínios por rede.
               // Definições dos comentários
               intDxComentario = IDINDEFINIDO;
               intDyComentario = IDINDEFINIDO;
               strComentario = "";
               if ((comentario = redes->ExisteComentario(rede)) != NULL)
                  {
                  comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
                  strComentario = comentario->Texto;
                  }
               ptrQry->SetVariable("p_DxComentario", intDxComentario);
               ptrQry->SetVariable("p_DyComentario", intDyComentario);
               ptrQry->SetVariable("p_Comentario"  , strComentario);

               // Executa query
               ptrQry->Execute();

               // Verifica o valor retornado a ser atualizado no objeto em memória
               rede->Id = ptrQry->GetVariable("p_RedeId");
               // No caso do cluster, somente é criado um novo cluster caso o valor
               // passado seja menor que zero. 
               if (rede->Cluster)
                  {
                  if (rede->Cluster->Id < 0)
                     {
                     rede->Cluster->Id = ptrQry->GetVariable("p_AgrupamentoId");
                     }
                  }

               if (rede->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }

               // Atualiza o identificador da MRede
               if (rede->MRede)
                  {
                  rede->MRede->Id = ptrQry->GetVariable("p_MRedeId");
                  if (rede->MRede->Id == IDINDEFINIDO)
                     {
                     // Condição de erro
                     sucesso = false;
                     SNPSTRLOGLINE(strlog);
                     }
                  }
               }
            catch (Exception& e)
               {
               SNPSTRLOGLINE(strlog);
               SNPSTRLOG(e.Message);
               SNPSTRLOG("Rede=" + IntToStr(rede->Id) + ", Cluster=" + IntToStr((rede->Cluster ? rede->Cluster->Id : -1) ));
               sucesso = false;
               }
            // loop redes (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrRede

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrRedeBarraIni(VTEstudo* estudo, int intRedeId, int intBarraIniId)
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

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrRedeBarraIni(:p_EstudoId, :p_RedeId, :p_BarraIniId);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_EstudoId"  , otInteger );
         ptrQry->DeclareVariable("p_RedeId"    , otInteger );
         ptrQry->DeclareVariable("p_BarraIniId", otInteger );

         // Escrita propriamente dita
         try
            {
            // Seta valores dos parâmetros
            ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
            ptrQry->SetVariable("p_RedeId"    , intRedeId);
            ptrQry->SetVariable("p_BarraIniId", intBarraIniId);

            // Executa query
            ptrQry->Execute();

            ptrQry->Close();
            }
         catch (Exception& e)
            {
            SNPSTRLOGLINE(strlog);
            SNPSTRLOG(e.Message);
            sucesso = false;
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrRedeBarraIni

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrBarra(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTBarra*      barra;
   AnsiString    strlog;
   int           intBarraId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrBarra(:p_NovoReg, :p_EstudoId, :p_BarraId, :p_VNom, ");
         ptrQry->SQL->Add("                         :p_EsqX1, :p_EsqY1, :p_EsqX2, :p_EsqY2, :p_UtmX, :p_UtmY, ");
         ptrQry->SQL->Add("                         :p_AlteradoObra, :p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                         :p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                        );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"        , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"       , otInteger);
         ptrQry->DeclareVariable("p_BarraId"        , otInteger);
         ptrQry->DeclareVariable("p_VNom"           , otFloat  );
         ptrQry->DeclareVariable("p_EsqX1"          , otInteger);
         ptrQry->DeclareVariable("p_EsqY1"          , otInteger);
         ptrQry->DeclareVariable("p_EsqX2"          , otInteger);
         ptrQry->DeclareVariable("p_EsqY2"          , otInteger);
         ptrQry->DeclareVariable("p_UtmX"           , otInteger);
         ptrQry->DeclareVariable("p_UtmY"           , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra"   , otInteger);
         ptrQry->DeclareVariable("p_Show"           , otInteger);
         ptrQry->DeclareVariable("p_Codigo"         , otString );
         ptrQry->DeclareVariable("p_ExtId"          , otString );
         ptrQry->DeclareVariable("p_DxComentario"   , otInteger);
         ptrQry->DeclareVariable("p_DyComentario"   , otInteger);
         ptrQry->DeclareVariable("p_Comentario"     , otString );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop Barras (ini)
            barra = (VTBarra*)lisObjRede->Items[n];
            if (!barra)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_BARRAINV
               return(false);
               }

            // Verifica se Id definido
            if ((barra->Id < 0) || (bolObra))
               {
               intBarraId = IDINDEFINIDO;
               }
            else
               {
               intBarraId = barra->Id;
               }
            // Verifica se o objeto é novo ou alterado
            if (barra->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;
               }
            else if (barra->StatusAlterado() || barra->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            // Seta valores dos parâmetros
            ptrQry->SetVariable("p_NovoReg"     , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"    , estudo->Id);
            ptrQry->SetVariable("p_BarraId"     , intBarraId);
            ptrQry->SetVariable("p_VNom"        , barra->vnom);
            ptrQry->SetVariable("p_EsqX1"       , barra->esq.x1);
            ptrQry->SetVariable("p_EsqY1"       , barra->esq.y1);
            ptrQry->SetVariable("p_EsqX2"       , barra->esq.x2);
            ptrQry->SetVariable("p_EsqY2"       , barra->esq.y2);
            ptrQry->SetVariable("p_UtmX"        , barra->utm.x);
            ptrQry->SetVariable("p_UtmY"        , barra->utm.y);
            ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);
            ptrQry->SetVariable("p_Show"        , barra->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"      , barra->Codigo);
            ptrQry->SetVariable("p_ExtId"       , barra->Extern_id);
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(barra)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario", intDxComentario);
            ptrQry->SetVariable("p_DyComentario", intDyComentario);
            ptrQry->SetVariable("p_Comentario"  , strComentario);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               barra->Id = ptrQry->GetVariable("p_BarraId");
               if (barra->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            // loop Barras (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrBarra

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrBarraRede(VTEstudo* estudo, int intRedeId, TList* lisObjRede)
   {
   //variáveis locais
   bool          sucesso = true;
   VTBarra*      barra;
   AnsiString    strlog;
   bool          bolNovoReg;
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

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrBarraRede(:p_NovoReg, :p_EstudoId, :p_RedeId, :p_BarraId);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"     , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"    , otInteger);
         ptrQry->DeclareVariable("p_RedeId"      , otInteger);
         ptrQry->DeclareVariable("p_BarraId"     , otInteger);

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop Barras (ini)
            barra = (VTBarra*)lisObjRede->Items[n];
            if (!barra)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_BARRAINV
               return(false);
               }
            // Verifica se o Id é válido
            if (barra->Id < 0)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_BARRAINV
               return(false);
               }
            // Verifica se o objeto é novo ou alterado
            if (barra->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;
               }
            else if (barra->StatusAlterado() || barra->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            // Seta valores dos parâmetros
            ptrQry->SetVariable("p_NovoReg"     , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"    , estudo->Id);
            ptrQry->SetVariable("p_RedeId"      , intRedeId);
            ptrQry->SetVariable("p_BarraId"     , barra->Id);

            // Executa query
            ptrQry->Execute();

            // loop Barras (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrBarraRede

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrTrecho(VTEstudo* estudo, int intRedeId, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTTrecho*     trecho;
   AnsiString    strlog;
   int           intTrechoId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

   if (!bd_geral)
      {
      if ((bd_geral = (VTBDgeral*)apl->GetObject(__classid(VTBDgeral))) == NULL)
         {
         Erro("Banco de dados inválido.");
         SNPSTRLOGLINE(strlog);
         }
      }

   if (!lisObjRede)
      {
      SNPSTRLOGLINE(strlog);
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrTrecho(:p_NovoReg, :p_EstudoId, :p_RedeId, :p_TrechoId, :p_Comprimento, ");
         ptrQry->SQL->Add("                          :p_ArranjoId, :p_AlteradoObra, :p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                          :p_Barra1Id, :p_PosBar1, :p_Barra2Id, :p_PosBar2, ");
         ptrQry->SQL->Add("                          :p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                         );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"     , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"    , otInteger);
         ptrQry->DeclareVariable("p_RedeId"      , otInteger);
         ptrQry->DeclareVariable("p_TrechoId"    , otInteger);
         ptrQry->DeclareVariable("p_Comprimento" , otFloat  );
         ptrQry->DeclareVariable("p_ArranjoId"   , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);
         ptrQry->DeclareVariable("p_Show"        , otInteger);
         ptrQry->DeclareVariable("p_Codigo"      , otString );
         ptrQry->DeclareVariable("p_ExtId"       , otString );
         ptrQry->DeclareVariable("p_Barra1Id"    , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"     , otFloat  );
         ptrQry->DeclareVariable("p_Barra2Id"    , otInteger);
         ptrQry->DeclareVariable("p_PosBar2"     , otFloat  );
         ptrQry->DeclareVariable("p_DxComentario", otInteger);
         ptrQry->DeclareVariable("p_DyComentario", otInteger);
         ptrQry->DeclareVariable("p_Comentario"  , otString );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop trechos (ini)
            trecho = (VTTrecho*)lisObjRede->Items[n];
            if (!trecho)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_TRECHOINV
                return(false);
                }
            // Verifica se o objeto é novo ou alterado
            if ((trecho->Id < 0) || (bolObra))
               {
               intTrechoId = IDINDEFINIDO;
               }
            else
               {
               intTrechoId = trecho->Id;
               }

            if (trecho->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;
               }
            else if (trecho->StatusAlterado() || trecho->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            // Atenção
            // A escrita dos cabos e suportes deve ser feita antes da escrita dos arranjos
            // Arranjos típicos já devem estar na base de dados, então salva somente arranjos próprios
            if (!trecho->ArranjoTipico)
               {
               if ( !bd_geral->SalvaArranjo(trecho->arranjo, trecho->ArranjoTipico) )
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_ARRANJO
                  return(false);
                  }
               }

            // Determina os valores dos parâmetros
            ptrQry->SetVariable("p_NovoReg"     , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"    , estudo->Id);
            ptrQry->SetVariable("p_RedeId"      , intRedeId);
            ptrQry->SetVariable("p_TrechoId"    , intTrechoId);
            ptrQry->SetVariable("p_Comprimento" , trecho->Comprimento_km);
            ptrQry->SetVariable("p_ArranjoId"   , trecho->arranjo->Id);
            ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);
            ptrQry->SetVariable("p_Show"        , trecho->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"      , trecho->Codigo);
            ptrQry->SetVariable("p_ExtId"       , (trecho->Extern_id.IsEmpty() ? trecho->Codigo : trecho->Extern_id));
            ptrQry->SetVariable("p_Barra1Id"    , trecho->pbarra1->Id);
            ptrQry->SetVariable("p_PosBar1"     , trecho->esq.posbar1);
            ptrQry->SetVariable("p_Barra2Id"    , trecho->pbarra2->Id);
            ptrQry->SetVariable("p_PosBar2"     , trecho->esq.posbar2);
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(trecho)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario", intDxComentario);
            ptrQry->SetVariable("p_DyComentario", intDyComentario);
            ptrQry->SetVariable("p_Comentario"  , strComentario);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               trecho->Id = ptrQry->GetVariable("p_TrechoId");
               if (trecho->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            // loop trechos (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrTrecho

//---------------------------------------------------------------------------
/*
Mudanças para salvar comentário:
0)
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

1)
   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

2) colocar , após o último parâmetro da linha anterior
         ptrQry->SQL->Add("                         :p_DxComentario, :p_DyComentario, :p_Comentario ");

3)
         ptrQry->DeclareVariable("p_DxComentario"   , otInteger);
         ptrQry->DeclareVariable("p_DyComentario"   , otInteger);
         ptrQry->DeclareVariable("p_Comentario"     , otString );

4)
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(carga)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario", intDxComentario);
            ptrQry->SetVariable("p_DyComentario", intDyComentario);
            ptrQry->SetVariable("p_Comentario"  , strComentario);


*/

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrCarga(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTCarga*      carga;
   AnsiString    strlog;
   int           intCargaId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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
   if (!lisObjRede)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Banco de dados inválido.");
      return(NULL);
      }
   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrCarga(:p_NovoReg, :p_EstudoId, :p_CargaId, :p_AlteradoObra, ");
         ptrQry->SQL->Add("                         :p_Show, :p_Codigo, :p_ExtId, :p_TipoFaseId, ");
         ptrQry->SQL->Add("                         :p_TipoConsId, :p_Enegia, :p_Barra1Id, :p_PosBar1, :p_Dx, :p_Dy, ");
         ptrQry->SQL->Add("                         :p_DxComentario, :p_DyComentario, :p_Comentario, ");
         ptrQry->SQL->Add("                         :p_ICtePu, :p_ZCtePu, :p_SCtePu, ");
         ptrQry->SQL->Add("                         :p_NumConsRes, :p_NumConsCom, :p_NumConsInd, :p_NumConsRur, :p_NumConsOut, :p_NumConsA4");
         ptrQry->SQL->Add("                        );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"        , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"       , otInteger);
         ptrQry->DeclareVariable("p_CargaId"        , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra"   , otInteger);
         ptrQry->DeclareVariable("p_Show"           , otInteger);
         ptrQry->DeclareVariable("p_Codigo"         , otString );
         ptrQry->DeclareVariable("p_ExtId"          , otString );
         ptrQry->DeclareVariable("p_TipoFaseId"     , otInteger);
         ptrQry->DeclareVariable("p_TipoConsId"     , otInteger);
         ptrQry->DeclareVariable("p_Enegia"         , otFloat  );
         ptrQry->DeclareVariable("p_Barra1Id"       , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"        , otFloat  );
         ptrQry->DeclareVariable("p_Dx"             , otInteger);
         ptrQry->DeclareVariable("p_Dy"             , otInteger);
         ptrQry->DeclareVariable("p_DxComentario"   , otInteger);
         ptrQry->DeclareVariable("p_DyComentario"   , otInteger);
         ptrQry->DeclareVariable("p_Comentario"     , otString );
         ptrQry->DeclareVariable("p_ICtePu"         , otFloat  );
         ptrQry->DeclareVariable("p_ZCtePu"         , otFloat  );
         ptrQry->DeclareVariable("p_SCtePu"         , otFloat  );
         ptrQry->DeclareVariable("p_NumConsRes"     , otInteger);
         ptrQry->DeclareVariable("p_NumConsCom"     , otInteger);
         ptrQry->DeclareVariable("p_NumConsInd"     , otInteger);
         ptrQry->DeclareVariable("p_NumConsRur"     , otInteger);
         ptrQry->DeclareVariable("p_NumConsOut"     , otInteger);
         ptrQry->DeclareVariable("p_NumConsA4"      , otInteger);

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop cargas (ini)
            carga = (VTCarga*)lisObjRede->Items[n];
            if (!carga)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_CARGAINV
                return(false);
                }
            if (!carga->curva)
               {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_CURVAINV
                return(false);
               }
            // Cargas de iluminação pública são escritas por um método específico
            if (carga->curva->Tipo == curvaLD)
               {
               continue;
               }
            //salva informação de VIP e GrupoA em Extern_id
            carga->Extern_id = "";
            if (carga->VeryImportant) carga->Extern_id  = "vip ";
            if (carga->GrupoA)        carga->Extern_id += "grupoA";

            // Verifica se o objeto é novo ou alterado
            if ((carga->Id < 0) || (bolObra))
               {
               intCargaId = IDINDEFINIDO;
               }
            else
               {
               intCargaId = carga->Id;
               }

            if (carga->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;

               // Na manipulação de obras, os IDs de curvas não são reiniciados para
               // equipamentos novos
               if (!carga->curva->Tipica)
                  {
                  carga->curva->Id = IDINDEFINIDO;
                  }
               }
            else if (carga->StatusAlterado() || carga->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            ptrQry->SetVariable("p_NovoReg"        , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"       , estudo->Id);
            ptrQry->SetVariable("p_CargaId"        , intCargaId);
            ptrQry->SetVariable("p_AlteradoObra"   , (int)bolObra);
            ptrQry->SetVariable("p_Show"           , carga->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"         , carga->Codigo);
            ptrQry->SetVariable("p_ExtId"          , carga->Extern_id);
            ptrQry->SetVariable("p_TipoFaseId"     , carga->Fases);
            ptrQry->SetVariable("p_TipoConsId"     , carga->classe->Id);
            ptrQry->SetVariable("p_Enegia"         , carga->Energia_mwhmes);
            ptrQry->SetVariable("p_Barra1Id"       , carga->pbarra->Id);
            ptrQry->SetVariable("p_PosBar1"        , carga->esq.posbar);
            ptrQry->SetVariable("p_Dx"             , carga->esq.dxbar);
            ptrQry->SetVariable("p_Dy"             , carga->esq.dybar);
            ptrQry->SetVariable("p_ICtePu"         , carga->Icte_pu);
            ptrQry->SetVariable("p_ZCtePu"         , carga->Zcte_pu);
            ptrQry->SetVariable("p_SCtePu"         , carga->Scte_pu);
            ptrQry->SetVariable("p_NumConsRes"     , carga->NumConsResidencial);
            ptrQry->SetVariable("p_NumConsCom"     , carga->NumConsComercial);
            ptrQry->SetVariable("p_NumConsInd"     , carga->NumConsIndustrial);
            ptrQry->SetVariable("p_NumConsRur"     , carga->NumConsRural);
            ptrQry->SetVariable("p_NumConsOut"     , carga->NumConsOutros);
            ptrQry->SetVariable("p_NumConsA4"      , carga->NumConsA4);

            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(carga)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario", intDxComentario);
            ptrQry->SetVariable("p_DyComentario", intDyComentario);
            ptrQry->SetVariable("p_Comentario"  , strComentario);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               carga->Id = ptrQry->GetVariable("p_CargaId");
               if (carga->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }

            // Escreve a curva da carga
            if (carga->curva->NumeroPontos() > 0)
               {
               if (!SPSNP_wrConjCurvaEquipamento(estudo, carga, carga->curva))
                  {
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            // loop cargas (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrCarga

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrCargaIP(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTCarga*      carga;
   AnsiString    strlog;
   int           intCargaId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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

   if (!lisObjRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrCargaIP(:p_NovoReg, :p_EstudoId, :p_CargaId, :p_AlteradoObra, ");
         ptrQry->SQL->Add("                           :p_Show, :p_Codigo, :p_ExtId, :p_Pnom, :p_FatorPot, ");
         ptrQry->SQL->Add("                           :p_ICte, :p_ZCte, :p_SCte, ");
         ptrQry->SQL->Add("                           :p_NumLamp, :p_TipoFaseId, :p_Barra1Id, :p_PosBar1, :p_Dx, :p_Dy, ");
         ptrQry->SQL->Add("                           :p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                          );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"     , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"    , otInteger);
         ptrQry->DeclareVariable("p_CargaId"     , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);
         ptrQry->DeclareVariable("p_Show"        , otInteger);
         ptrQry->DeclareVariable("p_Codigo"      , otString );
         ptrQry->DeclareVariable("p_ExtId"       , otString );
         ptrQry->DeclareVariable("p_Pnom"        , otFloat  );
         ptrQry->DeclareVariable("p_FatorPot"    , otFloat  );
         ptrQry->DeclareVariable("p_NumLamp"     , otInteger);
         ptrQry->DeclareVariable("p_ICte"        , otFloat  );
         ptrQry->DeclareVariable("p_ZCte"        , otFloat  );
         ptrQry->DeclareVariable("p_SCte"        , otFloat  );
         ptrQry->DeclareVariable("p_TipoFaseId"  , otInteger);
         ptrQry->DeclareVariable("p_Barra1Id"    , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"     , otFloat  );
         ptrQry->DeclareVariable("p_Dx"          , otInteger);
         ptrQry->DeclareVariable("p_Dy"          , otInteger);
         ptrQry->DeclareVariable("p_DxComentario", otInteger);
         ptrQry->DeclareVariable("p_DyComentario", otInteger);
         ptrQry->DeclareVariable("p_Comentario"  , otString );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop cargas IP (ini)
            carga = (VTCarga*)lisObjRede->Items[n];
            if (!carga)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_CARGAINV
                return(false);
                }
            if (!carga->curva)
               {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_CURVAINV
                return(false);
               }
            // Cargas que não são do tipo iluminação pública são escritas por um método específico
            if (carga->curva->Tipo != curvaLD)
               {
               continue;
               }

            // Verifica se o objeto é novo ou alterado
            if ((carga->Id < 0) || (bolObra))
               {
               intCargaId = IDINDEFINIDO;
               }
            else
               {
               intCargaId = carga->Id;
               }

            if (carga->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;

               // Na manipulação de obras, os IDs de curvas não são reiniciados para
               // equipamentos novos
               if (!carga->curva->Tipica)
                  {
                  carga->curva->Id = IDINDEFINIDO;
                  }
               }
            else if (carga->StatusAlterado() || carga->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            ptrQry->SetVariable("p_NovoReg"     , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"    , estudo->Id);
            ptrQry->SetVariable("p_CargaId"     , intCargaId);
            ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);
            ptrQry->SetVariable("p_Show"        , carga->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"      , carga->Codigo);
            ptrQry->SetVariable("p_ExtId"       , (carga->Extern_id.IsEmpty() ? carga->Codigo : carga->Extern_id));
            ptrQry->SetVariable("p_Pnom"        , carga->IP_Pot_kw);
            ptrQry->SetVariable("p_FatorPot"    , carga->IP_Fatpot);
            ptrQry->SetVariable("p_NumLamp"     , carga->IP_NumLamp);
            ptrQry->SetVariable("p_ICte"        , carga->Icte_pu);
            ptrQry->SetVariable("p_ZCte"        , carga->Zcte_pu);
            ptrQry->SetVariable("p_SCte"        , carga->Scte_pu);
            ptrQry->SetVariable("p_TipoFaseId"  , carga->Fases);
            ptrQry->SetVariable("p_Barra1Id"    , carga->pbarra->Id);
            ptrQry->SetVariable("p_PosBar1"     , carga->esq.posbar);
            ptrQry->SetVariable("p_Dx"          , carga->esq.dxbar);
            ptrQry->SetVariable("p_Dy"          , carga->esq.dybar);
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(carga)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario", intDxComentario);
            ptrQry->SetVariable("p_DyComentario", intDyComentario);
            ptrQry->SetVariable("p_Comentario"  , strComentario);
            
            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               carga->Id = ptrQry->GetVariable("p_CargaId");
               if (carga->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }

            // Escreve a curva da carga
            if (carga->curva->NumeroPontos() > 0)
               {
               if (!SPSNP_wrConjCurvaEquipamento(estudo, carga, carga->curva))
                  {
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }

            // loop cargas IP (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrCargaIP

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrCargaNL(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTCargaNL*    cargaNL;
   AnsiString    strlog;
   int           intCargaNLId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;
   strFUN*       ptrFUN;
   strHRM*       ptrHRM;
   TList*        lisEQP;

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

   if (!lisObjRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrCargaNL(:p_NovoReg, :p_EstudoId, :p_CargaNLId, :p_AlteradoObra, ");
         ptrQry->SQL->Add(                           ":p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add(                           ":p_OrdemId, :p_FatorPot, :p_Indutivo, :p_kVA, ");
         ptrQry->SQL->Add(                           ":p_SNomA, :p_SNomB, :p_SNomC, :p_AngA, :p_AngB, :p_AngC, ");
         ptrQry->SQL->Add(                           ":p_Barra1Id, :p_PosBar1, :p_Dx, :p_Dy, ");
         ptrQry->SQL->Add(                           ":p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                          );");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_NovoReg"     , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"    , otInteger);
         ptrQry->DeclareVariable("p_CargaNLId"   , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);
         ptrQry->DeclareVariable("p_Show"        , otInteger);
         ptrQry->DeclareVariable("p_Codigo"      , otString );
         ptrQry->DeclareVariable("p_ExtId"       , otString );
         ptrQry->DeclareVariable("p_OrdemId"     , otInteger);
         ptrQry->DeclareVariable("p_FatorPot"    , otFloat  );
         ptrQry->DeclareVariable("p_Indutivo"    , otInteger);
         ptrQry->DeclareVariable("p_kVA"         , otInteger);
         ptrQry->DeclareVariable("p_SNomA"       , otFloat  );
         ptrQry->DeclareVariable("p_SNomB"       , otFloat  );
         ptrQry->DeclareVariable("p_SNomC"       , otFloat  );
         ptrQry->DeclareVariable("p_AngA"        , otFloat  );
         ptrQry->DeclareVariable("p_AngB"        , otFloat  );
         ptrQry->DeclareVariable("p_AngC"        , otFloat  );
         ptrQry->DeclareVariable("p_Barra1Id"    , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"     , otFloat  );
         ptrQry->DeclareVariable("p_Dx"          , otInteger);
         ptrQry->DeclareVariable("p_Dy"          , otInteger);
         ptrQry->DeclareVariable("p_DxComentario", otInteger);
         ptrQry->DeclareVariable("p_DyComentario", otInteger);
         ptrQry->DeclareVariable("p_Comentario"  , otString );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop cargas NL (ini)
            cargaNL = (VTCargaNL*)lisObjRede->Items[n];
            if (!cargaNL)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_CARGAINV
                return(false);
                }

            // Verifica se o objeto é novo ou alterado
            if ((cargaNL->Id < 0) || (bolObra))
               {
               intCargaNLId = IDINDEFINIDO;
               }
            else
               {
               intCargaNLId = cargaNL->Id;
               }

            if (cargaNL->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;
               }
            else if (cargaNL->StatusAlterado() || cargaNL->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            ptrQry->SetVariable("p_NovoReg"     , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"    , estudo->Id);
            ptrQry->SetVariable("p_CargaNLId"   , intCargaNLId);
            ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);
            ptrQry->SetVariable("p_Show"        , cargaNL->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"      , cargaNL->Codigo);
            ptrQry->SetVariable("p_ExtId"       , (cargaNL->Extern_id.IsEmpty() ? cargaNL->Codigo : cargaNL->Extern_id));

            // Inicialmente salva a fundamental
            if ((ptrFUN = cargaNL->LeFundamental()) == NULL) return(false);
            ptrQry->SetVariable("p_OrdemId"     , 1                     );
            ptrQry->SetVariable("p_FatorPot"    , cargaNL->FatPot_pu    );
            ptrQry->SetVariable("p_Indutivo"    , (int)cargaNL->Indutiva);
            ptrQry->SetVariable("p_kVA"         , (int)true             );
            ptrQry->SetVariable("p_SNomA"       , ptrFUN->sa_mod_kva    );
            ptrQry->SetVariable("p_SNomB"       , ptrFUN->sb_mod_kva    );
            ptrQry->SetVariable("p_SNomC"       , ptrFUN->sc_mod_kva    );
            ptrQry->SetVariable("p_AngA"        , ptrFUN->sa_ang_grau   );
            ptrQry->SetVariable("p_AngB"        , ptrFUN->sb_ang_grau   );
            ptrQry->SetVariable("p_AngC"        , ptrFUN->sc_ang_grau   );
            ptrQry->SetVariable("p_Barra1Id"    , cargaNL->pbarra->Id   );
            ptrQry->SetVariable("p_PosBar1"     , cargaNL->esq.posbar   );
            ptrQry->SetVariable("p_Dx"          , cargaNL->esq.dxbar    );
            ptrQry->SetVariable("p_Dy"          , cargaNL->esq.dybar    );
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(cargaNL)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario", intDxComentario);
            ptrQry->SetVariable("p_DyComentario", intDyComentario);
            ptrQry->SetVariable("p_Comentario"  , strComentario);

            // Executa query para a fundamental
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               cargaNL->Id = ptrQry->GetVariable("p_CargaNLId");
               if (cargaNL->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }

            // Repete a execução para as harmônicas   strFUN*       ptrFUN;
            ptrQry->SetVariable("p_kVA", (int)false);
            for (int j = 0; j < cargaNL->NumHarmonicas; j++)
               {
               if ((ptrHRM = cargaNL->LeHarmonica(j)) == NULL) return(false);

               ptrQry->SetVariable("p_OrdemId", ptrHRM->ordem      );
               ptrQry->SetVariable("p_SNomA"  , ptrHRM->sa_mod_pu  );
               ptrQry->SetVariable("p_SNomB"  , ptrHRM->sb_mod_pu  );
               ptrQry->SetVariable("p_SNomC"  , ptrHRM->sc_mod_pu  );
               ptrQry->SetVariable("p_AngA"   , ptrHRM->sa_ang_grau);
               ptrQry->SetVariable("p_AngB"   , ptrHRM->sb_ang_grau);
               ptrQry->SetVariable("p_AngC"   , ptrHRM->sc_ang_grau);

               // Executa query para a harmonica
               ptrQry->Execute();
               }

            // loop cargas NL (fim)
            }

         ptrQry->Close();

         // Nos equipamentos e ligações, a atualização abaixo é executada logo após
         // a escrita. Para o caso particular das cargasNL é feito aqui.
         lisEQP = new TList();
         redes->LisEqbar(lisEQP, eqptoCARGANL);
         bd_func->DefineStatusEqpto(lisEQP, false);
         delete lisEQP;

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
   } // SPSNP_wrCargaNL

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrCapacitor(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTCapacitor*  capacitor;
   AnsiString    strlog;
   int           intCapacitorId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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

   if (!lisObjRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrCapacitor(:p_NovoReg, :p_EstudoId, :p_CapacitorId, :p_AlteradoObra, ");
         ptrQry->SQL->Add("                             :p_Show, :p_Codigo, :p_ExtId, :p_TipoLigacaoId, :p_Qnom, :p_Vnom, ");
         ptrQry->SQL->Add("                             :p_Barra1Id, :p_PosBar1, :p_Dx, :p_Dy, ");
         ptrQry->SQL->Add("                             :p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                            );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"        , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"       , otInteger);
         ptrQry->DeclareVariable("p_CapacitorId"    , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra"   , otInteger);
         ptrQry->DeclareVariable("p_Show"           , otInteger);
         ptrQry->DeclareVariable("p_Codigo"         , otString );
         ptrQry->DeclareVariable("p_ExtId"          , otString );
         ptrQry->DeclareVariable("p_TipoLigacaoId"  , otInteger);
         ptrQry->DeclareVariable("p_Qnom"           , otFloat  );
         ptrQry->DeclareVariable("p_Vnom"           , otFloat  );
         ptrQry->DeclareVariable("p_Barra1Id"       , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"        , otFloat  );
         ptrQry->DeclareVariable("p_Dx"             , otInteger);
         ptrQry->DeclareVariable("p_Dy"             , otInteger);
         ptrQry->DeclareVariable("p_DxComentario"   , otInteger);
         ptrQry->DeclareVariable("p_DyComentario"   , otInteger);
         ptrQry->DeclareVariable("p_Comentario"     , otString );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop capacitores (ini)
            capacitor = (VTCapacitor*)lisObjRede->Items[n];
            if (!capacitor)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_CAPACITORINV
                return(false);
                }
            // Verifica se o objeto é novo ou alterado
            if ((capacitor->Id < 0) || (bolObra))
               {
               intCapacitorId = IDINDEFINIDO;
               }
            else
               {
               intCapacitorId = capacitor->Id;
               }

            if (capacitor->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;

               // Na manipulação de obras, os IDs de curvas não são reiniciados para
               // equipamentos novos
               if (!capacitor->curva->Tipica)
                  {
                  capacitor->curva->Id = IDINDEFINIDO;
                  }
               }
            else if (capacitor->StatusAlterado() || capacitor->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            ptrQry->SetVariable("p_NovoReg"      , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"     , estudo->Id);
            ptrQry->SetVariable("p_CapacitorId"  , intCapacitorId);
            ptrQry->SetVariable("p_AlteradoObra" , (int)bolObra);
            ptrQry->SetVariable("p_Show"         , capacitor->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"       , capacitor->Codigo);
            ptrQry->SetVariable("p_ExtId"        , (capacitor->Extern_id.IsEmpty() ? capacitor->Codigo : capacitor->Extern_id));
            ptrQry->SetVariable("p_TipoLigacaoId", capacitor->ligacao);
            ptrQry->SetVariable("p_Qnom"         , capacitor->q);
            ptrQry->SetVariable("p_Vnom"         , capacitor->vnom);
            ptrQry->SetVariable("p_Barra1Id"     , capacitor->pbarra->Id);
            ptrQry->SetVariable("p_PosBar1"      , capacitor->esq.posbar);
            ptrQry->SetVariable("p_Dx"           , capacitor->esq.dxbar);
            ptrQry->SetVariable("p_Dy"           , capacitor->esq.dybar);
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(capacitor)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario", intDxComentario);
            ptrQry->SetVariable("p_DyComentario", intDyComentario);
            ptrQry->SetVariable("p_Comentario"  , strComentario);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               capacitor->Id = ptrQry->GetVariable("p_CapacitorId");
               if (capacitor->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }

            // Escreve a curva do capacitor
            if (capacitor->curva->NumeroPontos() > 0)
               {
               if (!SPSNP_wrConjCurvaEquipamento(estudo, capacitor, capacitor->curva))
                  {
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            // loop trechos (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrCapacitor

//---------------------------------------------------------------------------
// !OOPS! Para que o bolObra?
bool __fastcall TBDeqptoOracle::SPSNP_wrDominio(TList* lisDominio, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTDominio*    dominio;
   AnsiString    strlog;
   int           intDominioId;
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

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
      return(false);
      }

   if (!lisDominio)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISDOMINIOINV
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrDominio(:p_DominioId, :p_Codigo, :p_DataCriacao);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_DominioId"  , otInteger);
         ptrQry->DeclareVariable("p_Codigo"     , otString );
         ptrQry->DeclareVariable("p_DataCriacao", otDate   );

         for (int nc = 0; nc < lisDominio->Count; nc++)
            {
            // loop Dominios (ini)
            dominio = (VTDominio*)lisDominio->Items[nc];
            if (!dominio)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_DOMINIOINV
               return(false);
               }
            // Verifica se o objeto é novo ou alterado
            // Se o domínio já existe, não atualiza
            if ((!dominio->StatusNovo()) && (!dominio->StatusAlterado()))
               {
               continue;
               }
            // Verifica se o objeto é novo ou alterado
            if (dominio->Id < 0)
               {
               intDominioId = IDINDEFINIDO;
               }
            else
               {
               intDominioId = dominio->Id;
               }

            // Seta valores dos parâmetros
            ptrQry->SetVariable("p_DominioId"  , intDominioId         );
            ptrQry->SetVariable("p_Codigo"     , dominio->Codigo      );
            ptrQry->SetVariable("p_DataCriacao", dominio->DataCriacao );

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            dominio->Id = ptrQry->GetVariable("p_DominioId");
            if (dominio->Id == IDINDEFINIDO)
               {
               // Condição de erro
               sucesso = false;
               SNPSTRLOGLINE(strlog);
               }
            dominio->DefineStatus(sttNOVO,     false);
            dominio->DefineStatus(sttALTERADO, false);
            // loop Dominios (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrDominio

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrGerador(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTGerador*    gerador;
   AnsiString    strlog;
   int           intGeradorId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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
      
   if (!lisObjRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrGerador(:p_NovoReg, :p_EstudoId, :p_GeradorId, :p_AlteradoObra, ");
         ptrQry->SQL->Add("                           :p_Show, :p_Codigo, :p_ExtId, :p_TipoLigacaoId, :p_TipoSuprId, :p_TipoGerId, ");
         ptrQry->SQL->Add("                           :p_Vnom, :p_Snom, :p_Smax, :p_FatorPotMin, :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X, ");
         ptrQry->SQL->Add("                           :p_Barra1Id, :p_PosBar1, :p_Dx, :p_Dy, ");
         ptrQry->SQL->Add("                           :p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                          );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"      , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"     , otInteger);
         ptrQry->DeclareVariable("p_GeradorId"    , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra" , otInteger);
         ptrQry->DeclareVariable("p_Show"         , otInteger);
         ptrQry->DeclareVariable("p_Codigo"       , otString );
         ptrQry->DeclareVariable("p_ExtId"        , otString );
         ptrQry->DeclareVariable("p_TipoLigacaoId", otInteger);
         ptrQry->DeclareVariable("p_TipoSuprId"   , otInteger);
         ptrQry->DeclareVariable("p_TipoGerId"    , otInteger);
         ptrQry->DeclareVariable("p_Vnom"         , otFloat  );
         ptrQry->DeclareVariable("p_Snom"         , otFloat  );
         ptrQry->DeclareVariable("p_Smax"         , otFloat  );
         ptrQry->DeclareVariable("p_FatorPotMin"  , otFloat  );
         ptrQry->DeclareVariable("p_Z0R"          , otFloat  );
         ptrQry->DeclareVariable("p_Z0X"          , otFloat  );
         ptrQry->DeclareVariable("p_Z1R"          , otFloat  );
         ptrQry->DeclareVariable("p_Z1X"          , otFloat  );
         ptrQry->DeclareVariable("p_Barra1Id"     , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"      , otFloat  );
         ptrQry->DeclareVariable("p_Dx"           , otInteger);
         ptrQry->DeclareVariable("p_Dy"           , otInteger);
         ptrQry->DeclareVariable("p_DxComentario" , otInteger);
         ptrQry->DeclareVariable("p_DyComentario" , otInteger);
         ptrQry->DeclareVariable("p_Comentario"   , otString );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop Geradores (ini)
            gerador = (VTGerador*)lisObjRede->Items[n];
            if (!gerador)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_GERADORINV
                return(false);
                }
            // Verifica se o objeto é novo ou alterado
            if ((gerador->Id < 0) || (bolObra))
               {
               intGeradorId = IDINDEFINIDO;
               }
            else
               {
               intGeradorId = gerador->Id;
               }

            if (gerador->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;

               // Na manipulação de obras, os IDs de curvas não são reiniciados para
               // equipamentos novos
               if (!gerador->curva->Tipica)
                  {
                  gerador->curva->Id = IDINDEFINIDO;
                  }
               }
            else if (gerador->StatusAlterado() || gerador->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            ptrQry->SetVariable("p_NovoReg"      , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"     , estudo->Id);
            ptrQry->SetVariable("p_GeradorId"    , intGeradorId);
            ptrQry->SetVariable("p_AlteradoObra" , (int)bolObra);
            ptrQry->SetVariable("p_Show"         , gerador->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"       , gerador->Codigo);
            ptrQry->SetVariable("p_ExtId"        , (gerador->Extern_id.IsEmpty() ? gerador->Codigo : gerador->Extern_id));
            ptrQry->SetVariable("p_TipoLigacaoId", gerador->ligacao);
            ptrQry->SetVariable("p_TipoSuprId"   , gerador->tiposup);
            ptrQry->SetVariable("p_TipoGerId"    , gerador->FonteEnergia);
            ptrQry->SetVariable("p_Vnom"         , gerador->vnom);
            ptrQry->SetVariable("p_Snom"         , gerador->snom);
            ptrQry->SetVariable("p_Smax"         , gerador->smax);
            ptrQry->SetVariable("p_FatorPotMin"  , gerador->fplim);
            ptrQry->SetVariable("p_Z0R"          , gerador->z0.r);
            ptrQry->SetVariable("p_Z0X"          , gerador->z0.x);
            ptrQry->SetVariable("p_Z1R"          , gerador->z1.r);
            ptrQry->SetVariable("p_Z1X"          , gerador->z1.x);
            ptrQry->SetVariable("p_Barra1Id"     , gerador->pbarra->Id);
            ptrQry->SetVariable("p_PosBar1"      , gerador->esq.posbar);
            ptrQry->SetVariable("p_Dx"           , gerador->esq.dxbar);
            ptrQry->SetVariable("p_Dy"           , gerador->esq.dybar);
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(gerador)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario", intDxComentario);
            ptrQry->SetVariable("p_DyComentario", intDyComentario);
            ptrQry->SetVariable("p_Comentario"  , strComentario);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               gerador->Id = ptrQry->GetVariable("p_GeradorId");
               if (gerador->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }

            // Escreve a curva do gerador
            if (gerador->curva->NumeroPontos() > 0)
               {
               if (!SPSNP_wrConjCurvaEquipamento(estudo, gerador, gerador->curva))
                  {
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            // loop Geradores (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrGerador

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrMedidor(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTMedidor*    medidor;
   AnsiString    strlog;
   int           intMedidorId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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

   if (!lisObjRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrMedidor(:p_NovoReg, :p_EstudoId, :p_MedidorId, :p_AlteradoObra, ");
         ptrQry->SQL->Add("                           :p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                           :p_Barra1Id, :p_PosBar1, :p_Dx, :p_Dy, ");
         ptrQry->SQL->Add("                           :p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                          );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"     , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"    , otInteger);
         ptrQry->DeclareVariable("p_MedidorId"   , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);
         ptrQry->DeclareVariable("p_Show"        , otInteger);
         ptrQry->DeclareVariable("p_Codigo"      , otString );
         ptrQry->DeclareVariable("p_ExtId"       , otString );
         ptrQry->DeclareVariable("p_Barra1Id"    , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"     , otFloat  );
         ptrQry->DeclareVariable("p_Dx"          , otInteger);
         ptrQry->DeclareVariable("p_Dy"          , otInteger);
         ptrQry->DeclareVariable("p_DxComentario", otInteger);
         ptrQry->DeclareVariable("p_DyComentario", otInteger);
         ptrQry->DeclareVariable("p_Comentario"  , otString );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop Medidores (ini)
            medidor = (VTMedidor*)lisObjRede->Items[n];
            if (!medidor)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_MEDIDORINV
                return(false);
                }
            // Verifica se o objeto é novo ou alterado
            if ((medidor->Id < 0) || (bolObra))
               {
               intMedidorId = IDINDEFINIDO;
               }
            else
               {
               intMedidorId = medidor->Id;
               }

            if (medidor->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;
               }
            else if (medidor->StatusAlterado() || medidor->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            ptrQry->SetVariable("p_NovoReg"     , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"    , estudo->Id);
            ptrQry->SetVariable("p_MedidorId"   , intMedidorId);
            ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);
            ptrQry->SetVariable("p_Show"        , medidor->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"      , medidor->Codigo);
            ptrQry->SetVariable("p_ExtId"       , (medidor->Extern_id.IsEmpty() ? medidor->Codigo : medidor->Extern_id));
            ptrQry->SetVariable("p_Barra1Id"    , medidor->pbarra->Id);
            ptrQry->SetVariable("p_PosBar1"     , medidor->esq.posbar);
            ptrQry->SetVariable("p_Dx"          , medidor->esq.dxbar);
            ptrQry->SetVariable("p_Dy"          , medidor->esq.dybar);
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(medidor)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario", intDxComentario);
            ptrQry->SetVariable("p_DyComentario", intDyComentario);
            ptrQry->SetVariable("p_Comentario"  , strComentario);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               medidor->Id = ptrQry->GetVariable("p_MedidorId");
               if (medidor->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }

            // loop Medidores (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrMedidor

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrCanal(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTCanal*      canal;
   AnsiString    strlog;
   int           intCanalId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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

   if (!lisObjRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrCanal(:p_NovoReg, :p_EstudoId, :p_CanalId, :p_AlteradoObra, ");
         ptrQry->SQL->Add("                         :p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                         :p_MedidorId, :p_Sigma, :p_TipoCanalId, ");
         ptrQry->SQL->Add("                         :p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                        );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"     , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"    , otInteger);
         ptrQry->DeclareVariable("p_CanalId"     , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);
         ptrQry->DeclareVariable("p_Show"        , otInteger);
         ptrQry->DeclareVariable("p_Codigo"      , otString );
         ptrQry->DeclareVariable("p_ExtId"       , otString );
         ptrQry->DeclareVariable("p_MedidorId"   , otInteger);
         ptrQry->DeclareVariable("p_Sigma"       , otFloat  );
         ptrQry->DeclareVariable("p_TipoCanalId" , otInteger);
         ptrQry->DeclareVariable("p_DxComentario", otInteger);
         ptrQry->DeclareVariable("p_DyComentario", otInteger);
         ptrQry->DeclareVariable("p_Comentario"  , otString );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop Canais (ini)
            canal = (VTCanal*)lisObjRede->Items[n];
            if (!canal)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_CANALINV
                return(false);
                }
            // Verifica se o objeto é novo ou alterado
            if ((canal->Id < 0) || (bolObra))
               {
               intCanalId = IDINDEFINIDO;
               }
            else
               {
               intCanalId = canal->Id;
               }

            if (canal->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;

               // Na manipulação de obras, os IDs de curvas não são reiniciados para
               // equipamentos novos
               if (!canal->Curva->Tipica)
                  {
                  canal->Curva->Id = IDINDEFINIDO;
                  }
               }
            else if (canal->StatusAlterado() || canal->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            // Seta valores dos parâmetros
            ptrQry->SetVariable("p_NovoReg"     , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"    , estudo->Id);
            ptrQry->SetVariable("p_CanalId"     , intCanalId);
            ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);
            ptrQry->SetVariable("p_Show"        , canal->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"      , canal->Codigo);
            ptrQry->SetVariable("p_ExtId"       , (canal->Extern_id.IsEmpty() ? canal->Codigo : canal->Extern_id));
            ptrQry->SetVariable("p_MedidorId"   , (canal->Medidor ? canal->Medidor->Id : IDINDEFINIDO));
            ptrQry->SetVariable("p_Sigma"       , canal->Tolerancia_perc);
            ptrQry->SetVariable("p_TipoCanalId" , canal->TipoCanal);
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(canal)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario", intDxComentario);
            ptrQry->SetVariable("p_DyComentario", intDyComentario);
            ptrQry->SetVariable("p_Comentario"  , strComentario);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               canal->Id = ptrQry->GetVariable("p_CanalId");
               if (canal->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }

            // Escreve a curva do canal
            if (canal->Curva->NumeroPontos() > 0)
               {
               if (!SPSNP_wrConjCurvaEquipamento(estudo, canal, canal->Curva))
                  {
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            // loop Canais (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrCanal

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrSuprimento(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTSuprimento* suprimento;
   AnsiString    strlog;
   int           intSuprimentoId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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

   if (!lisObjRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrSuprimento(:p_NovoReg, :p_EstudoId, :p_SuprimentoId, :p_AlteradoObra, ");
         ptrQry->SQL->Add("                              :p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                              :p_TipoSuprId, :p_Vnom, :p_Smax, :p_Pcc3F, :p_Qcc3F, :p_PccFT, :p_QccFT, ");
         ptrQry->SQL->Add("                              :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X, ");
         ptrQry->SQL->Add("                              :p_Barra1Id, :p_PosBar1, :p_Dx, :p_Dy, ");
         ptrQry->SQL->Add("                              :p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                             );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"      , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"     , otInteger);
         ptrQry->DeclareVariable("p_SuprimentoId" , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra" , otInteger);
         ptrQry->DeclareVariable("p_Show"         , otInteger);
         ptrQry->DeclareVariable("p_Codigo"       , otString );
         ptrQry->DeclareVariable("p_ExtId"        , otString );
         ptrQry->DeclareVariable("p_TipoSuprId"   , otInteger);
         ptrQry->DeclareVariable("p_Vnom"         , otFloat  );
         ptrQry->DeclareVariable("p_Smax"         , otFloat  );
         ptrQry->DeclareVariable("p_Pcc3F"        , otFloat  );
         ptrQry->DeclareVariable("p_Qcc3F"        , otFloat  );
         ptrQry->DeclareVariable("p_PccFT"        , otFloat  );
         ptrQry->DeclareVariable("p_QccFT"        , otFloat  );
         ptrQry->DeclareVariable("p_Z0R"          , otFloat  );
         ptrQry->DeclareVariable("p_Z0X"          , otFloat  );
         ptrQry->DeclareVariable("p_Z1R"          , otFloat  );
         ptrQry->DeclareVariable("p_Z1X"          , otFloat  );
         ptrQry->DeclareVariable("p_Barra1Id"     , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"      , otFloat  );
         ptrQry->DeclareVariable("p_Dx"           , otInteger);
         ptrQry->DeclareVariable("p_Dy"           , otInteger);
         ptrQry->DeclareVariable("p_DxComentario" , otInteger);
         ptrQry->DeclareVariable("p_DyComentario" , otInteger);
         ptrQry->DeclareVariable("p_Comentario"   , otString );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop Suprimentos (ini)
            suprimento = (VTSuprimento*)lisObjRede->Items[n];
            if (!suprimento)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_SUPRIMENTOINV
                return(false);
                }
            // Verifica se o objeto é novo ou alterado
            if ((suprimento->Id < 0) || (bolObra))
               {
               intSuprimentoId = IDINDEFINIDO;
               }
            else
               {
               intSuprimentoId = suprimento->Id;
               }

            if (suprimento->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;

               // Na manipulação de obras, os IDs de curvas não são reiniciados para
               // equipamentos novos
               if (!suprimento->curva->Tipica)
                  {
                  suprimento->curva->Id = IDINDEFINIDO;
                  }
               }
            else if (suprimento->StatusAlterado() || suprimento->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            // Seta valores dos parâmetros
            ptrQry->SetVariable("p_NovoReg"      , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"     , estudo->Id);
            ptrQry->SetVariable("p_SuprimentoId" , intSuprimentoId);
            ptrQry->SetVariable("p_AlteradoObra" , (int)bolObra);
            ptrQry->SetVariable("p_Show"         , suprimento->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"       , suprimento->Codigo);
            ptrQry->SetVariable("p_ExtId"        , (suprimento->Extern_id.IsEmpty() ? suprimento->Codigo : suprimento->Extern_id));
            ptrQry->SetVariable("p_TipoSuprId"   , suprimento->tiposup);
            ptrQry->SetVariable("p_Vnom"         , suprimento->vnom);
            ptrQry->SetVariable("p_Smax"         , suprimento->smax);
            ptrQry->SetVariable("p_Pcc3F"        , suprimento->pcc_3f.p);
            ptrQry->SetVariable("p_Qcc3F"        , suprimento->pcc_3f.q);
            ptrQry->SetVariable("p_PccFT"        , suprimento->pcc_ft.p);
            ptrQry->SetVariable("p_QccFT"        , suprimento->pcc_ft.q);
            ptrQry->SetVariable("p_Z0R"          , suprimento->zeq0.r);
            ptrQry->SetVariable("p_Z0X"          , suprimento->zeq0.x);
            ptrQry->SetVariable("p_Z1R"          , suprimento->zeq1.r);
            ptrQry->SetVariable("p_Z1X"          , suprimento->zeq1.x);
            ptrQry->SetVariable("p_Barra1Id"     , suprimento->pbarra->Id);
            ptrQry->SetVariable("p_PosBar1"      , suprimento->esq.posbar);
            ptrQry->SetVariable("p_Dx"           , suprimento->esq.dxbar);
            ptrQry->SetVariable("p_Dy"           , suprimento->esq.dybar);
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(suprimento)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario", intDxComentario);
            ptrQry->SetVariable("p_DyComentario", intDyComentario);
            ptrQry->SetVariable("p_Comentario"  , strComentario);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               suprimento->Id = ptrQry->GetVariable("p_SuprimentoId");
               if (suprimento->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }

            // Escreve a curva do gerador
            if (suprimento->curva->NumeroPontos() > 0)
               {
               if (!SPSNP_wrConjCurvaEquipamento(estudo, suprimento, suprimento->curva))
                  {
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            // loop Suprimentos (fim)
            }

         ptrQry->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         if (suprimento)
            {
            strlog = "Suprimento " + IntToStr(suprimento->Id) + " - " + suprimento->Codigo;
            SNPSTRLOG(strlog);
            }
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrSuprimento

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrFiltro(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTFiltro*     filtro;
   AnsiString    strlog;
   int           intFiltroId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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

   if (!lisObjRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrFiltro(:p_NovoReg, :p_EstudoId, :p_FiltroId, :p_AlteradoObra, ");
         ptrQry->SQL->Add("                          :p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                          :p_Vnom, :p_Snom, :p_Z0R, :p_Z0X, ");
         ptrQry->SQL->Add("                          :p_Barra1Id, :p_PosBar1, :p_Dx, :p_Dy, ");
         ptrQry->SQL->Add("                          :p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                         );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"      , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"     , otInteger);
         ptrQry->DeclareVariable("p_FiltroId"     , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra" , otInteger);
         ptrQry->DeclareVariable("p_Show"         , otInteger);
         ptrQry->DeclareVariable("p_Codigo"       , otString );
         ptrQry->DeclareVariable("p_ExtId"        , otString );
         ptrQry->DeclareVariable("p_Vnom"         , otFloat  );
         ptrQry->DeclareVariable("p_Snom"         , otFloat  );
         ptrQry->DeclareVariable("p_Z0R"          , otFloat  );
         ptrQry->DeclareVariable("p_Z0X"          , otFloat  );
         ptrQry->DeclareVariable("p_Barra1Id"     , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"      , otFloat  );
         ptrQry->DeclareVariable("p_Dx"           , otInteger);
         ptrQry->DeclareVariable("p_Dy"           , otInteger);
         ptrQry->DeclareVariable("p_DxComentario" , otInteger);
         ptrQry->DeclareVariable("p_DyComentario" , otInteger);
         ptrQry->DeclareVariable("p_Comentario"   , otString );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop Filtros (ini)
            filtro = (VTFiltro*)lisObjRede->Items[n];
            if (!filtro)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_FILTROINV
                return(false);
                }
            // Verifica se o objeto é novo ou alterado
            if ((filtro->Id < 0) || (bolObra))
               {
               intFiltroId = IDINDEFINIDO;
               }
            else
               {
               intFiltroId = filtro->Id;
               }

            if (filtro->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;
               }
            else if (filtro->StatusAlterado() || filtro->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            // Seta valores dos parâmetros
            ptrQry->SetVariable("p_NovoReg"      , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"     , estudo->Id);
            ptrQry->SetVariable("p_FiltroId"     , intFiltroId);
            ptrQry->SetVariable("p_AlteradoObra" , (int)bolObra);
            ptrQry->SetVariable("p_Show"         , filtro->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"       , filtro->Codigo);
            ptrQry->SetVariable("p_ExtId"        , (filtro->Extern_id.IsEmpty() ? filtro->Codigo : filtro->Extern_id));
            ptrQry->SetVariable("p_Vnom"         , filtro->vnom);
            ptrQry->SetVariable("p_Snom"         , filtro->snom);
            ptrQry->SetVariable("p_Z0R"          , filtro->z0.r);
            ptrQry->SetVariable("p_Z0X"          , filtro->z0.x);
            ptrQry->SetVariable("p_Barra1Id"     , filtro->pbarra->Id);
            ptrQry->SetVariable("p_PosBar1"      , filtro->esq.posbar);
            ptrQry->SetVariable("p_Dx"           , filtro->esq.dxbar);
            ptrQry->SetVariable("p_Dy"           , filtro->esq.dybar);
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(filtro)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario", intDxComentario);
            ptrQry->SetVariable("p_DyComentario", intDyComentario);
            ptrQry->SetVariable("p_Comentario"  , strComentario);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               filtro->Id = ptrQry->GetVariable("p_FiltroId");
               if (filtro->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            // loop Filtros (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrFiltro

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrReator(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTReator*     reator;
   AnsiString    strlog;
   int           intReatorId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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

   if (!lisObjRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrReator(:p_NovoReg, :p_EstudoId, :p_ReatorId, :p_AlteradoObra, ");
         ptrQry->SQL->Add("                           :p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                           :p_TipoLigacao, :p_Vnom, :p_Qnom, ");
         ptrQry->SQL->Add("                           :p_Barra1Id, :p_PosBar1, :p_Dx, :p_Dy, ");
         ptrQry->SQL->Add("                           :p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                          );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"      , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"     , otInteger);
         ptrQry->DeclareVariable("p_ReatorId"     , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra" , otInteger);
         ptrQry->DeclareVariable("p_Show"         , otInteger);
         ptrQry->DeclareVariable("p_Codigo"       , otString );
         ptrQry->DeclareVariable("p_ExtId"        , otString );
         ptrQry->DeclareVariable("p_TipoLigacao"  , otInteger);
         ptrQry->DeclareVariable("p_Vnom"         , otFloat  );
         ptrQry->DeclareVariable("p_Qnom"         , otFloat  );
         ptrQry->DeclareVariable("p_Barra1Id"     , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"      , otFloat  );
         ptrQry->DeclareVariable("p_Dx"           , otInteger);
         ptrQry->DeclareVariable("p_Dy"           , otInteger);
         ptrQry->DeclareVariable("p_DxComentario" , otInteger);
         ptrQry->DeclareVariable("p_DyComentario" , otInteger);
         ptrQry->DeclareVariable("p_Comentario"   , otString );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop Reatores (ini)
            reator = (VTReator*)lisObjRede->Items[n];
            if (!reator)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_REATORINV
                return(false);
                }
            // Verifica se o objeto é novo ou alterado
            if ((reator->Id < 0) || (bolObra))
               {
               intReatorId = IDINDEFINIDO;

               // Na manipulação de obras, os IDs de curvas não são reiniciados para
               // equipamentos novos
               if (!reator->curva->Tipica)
                  {
                  reator->curva->Id = IDINDEFINIDO;
                  }
               }
            else
               {
               intReatorId = reator->Id;
               }

            if (reator->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;
               }
            else if (reator->StatusAlterado() || reator->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            // Seta valores dos parâmetros
            ptrQry->SetVariable("p_NovoReg"      , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"     , estudo->Id);
            ptrQry->SetVariable("p_ReatorId"     , intReatorId);
            ptrQry->SetVariable("p_AlteradoObra" , (int)bolObra);
            ptrQry->SetVariable("p_Show"         , reator->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"       , reator->Codigo);
            ptrQry->SetVariable("p_ExtId"        , (reator->Extern_id.IsEmpty() ? reator->Codigo : reator->Extern_id));
            ptrQry->SetVariable("p_TipoLigacao"  , reator->ligacao);
            ptrQry->SetVariable("p_Vnom"         , reator->vnom);
            ptrQry->SetVariable("p_Qnom"         , reator->Q_mvar);
            ptrQry->SetVariable("p_Barra1Id"     , reator->pbarra->Id);
            ptrQry->SetVariable("p_PosBar1"      , reator->esq.posbar);
            ptrQry->SetVariable("p_Dx"           , reator->esq.dxbar);
            ptrQry->SetVariable("p_Dy"           , reator->esq.dybar);
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(reator)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario", intDxComentario);
            ptrQry->SetVariable("p_DyComentario", intDyComentario);
            ptrQry->SetVariable("p_Comentario"  , strComentario);

            // Executa query
            ptrQry->Execute();

            // Escreve a curva do reator
            if (reator->curva->NumeroPontos() > 0)
               {
               if (!SPSNP_wrConjCurvaEquipamento(estudo, reator, reator->curva))
                  {
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               reator->Id = ptrQry->GetVariable("p_ReatorId");
               if (reator->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            // loop Reatores (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrReator


//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrTrafoZZ(VTEstudo* estudo, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTTrafoZZ*    trafoZZ;
   AnsiString    strlog;
   int           intTrafoZZId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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

   if (!lisObjRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrTrafoZZ(:p_NovoReg, :p_EstudoId, :p_TrafoZZId, :p_AlteradoObra, ");
         ptrQry->SQL->Add("                           :p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                           :p_Vnom, :p_Snom, :p_Z0R, :p_Z0X, ");
         ptrQry->SQL->Add("                           :p_Barra1Id, :p_PosBar1, :p_Dx, :p_Dy, ");
         ptrQry->SQL->Add("                           :p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                          );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"      , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"     , otInteger);
         ptrQry->DeclareVariable("p_TrafoZZId"    , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra" , otInteger);
         ptrQry->DeclareVariable("p_Show"         , otInteger);
         ptrQry->DeclareVariable("p_Codigo"       , otString );
         ptrQry->DeclareVariable("p_ExtId"        , otString );
         ptrQry->DeclareVariable("p_Vnom"         , otFloat  );
         ptrQry->DeclareVariable("p_Snom"         , otFloat  );
         ptrQry->DeclareVariable("p_Z0R"          , otFloat  );
         ptrQry->DeclareVariable("p_Z0X"          , otFloat  );
         ptrQry->DeclareVariable("p_Barra1Id"     , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"      , otFloat  );
         ptrQry->DeclareVariable("p_Dx"           , otInteger);
         ptrQry->DeclareVariable("p_Dy"           , otInteger);
         ptrQry->DeclareVariable("p_DxComentario" , otInteger);
         ptrQry->DeclareVariable("p_DyComentario" , otInteger);
         ptrQry->DeclareVariable("p_Comentario"   , otString );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop Trafos ZZ (ini)
            trafoZZ = (VTTrafoZZ*)lisObjRede->Items[n];
            if (!trafoZZ)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_TRAFOZZINV
                return(false);
                }
            // Verifica se o objeto é novo ou alterado
            if ((trafoZZ->Id < 0) || (bolObra))
               {
               intTrafoZZId = IDINDEFINIDO;
               }
            else
               {
               intTrafoZZId = trafoZZ->Id;
               }

            if (trafoZZ->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;
               }
            else if (trafoZZ->StatusAlterado() || trafoZZ->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

             // Seta valores dos parâmetros
            ptrQry->SetVariable("p_NovoReg"      , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"     , estudo->Id);
            ptrQry->SetVariable("p_TrafoZZId"    , intTrafoZZId);
            ptrQry->SetVariable("p_AlteradoObra" , (int)bolObra);
            ptrQry->SetVariable("p_Show"         , trafoZZ->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"       , trafoZZ->Codigo);
            ptrQry->SetVariable("p_ExtId"        , (trafoZZ->Extern_id.IsEmpty() ? trafoZZ->Codigo : trafoZZ->Extern_id));
            ptrQry->SetVariable("p_Vnom"         , trafoZZ->vnom);
            ptrQry->SetVariable("p_Snom"         , trafoZZ->snom);
            ptrQry->SetVariable("p_Z0R"          , trafoZZ->z0.r);
            ptrQry->SetVariable("p_Z0X"          , trafoZZ->z0.x);
            ptrQry->SetVariable("p_Barra1Id"     , trafoZZ->pbarra->Id);
            ptrQry->SetVariable("p_PosBar1"      , trafoZZ->esq.posbar);
            ptrQry->SetVariable("p_Dx"           , trafoZZ->esq.dxbar);
            ptrQry->SetVariable("p_Dy"           , trafoZZ->esq.dybar);
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(trafoZZ)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario", intDxComentario);
            ptrQry->SetVariable("p_DyComentario", intDyComentario);
            ptrQry->SetVariable("p_Comentario"  , strComentario);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               trafoZZ->Id = ptrQry->GetVariable("p_TrafoZZId");
               if (trafoZZ->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }

            // loop Trafos ZZ (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrTrafoZZ

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrCapacitorSerie(VTEstudo* estudo, int intRedeId, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTCapserie*   capserie;
   AnsiString    strlog;
   int           intCapSerieId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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

   if (!lisObjRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrCapacitorSerie(:p_NovoReg, :p_EstudoId, :p_RedeId, :p_CapacitorSerieId, :p_AlteradoObra, ");
         ptrQry->SQL->Add("                                  :p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                                  :p_Vnom, :p_Snom, :p_ZR, :p_ZX,");
         ptrQry->SQL->Add("                                  :p_Barra1Id, :p_PosBar1, :p_Barra2Id, :p_PosBar2, ");
         ptrQry->SQL->Add("                                  :p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                                 );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"         , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"        , otInteger);
         ptrQry->DeclareVariable("p_RedeId"          , otInteger);
         ptrQry->DeclareVariable("p_CapacitorSerieId", otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra"    , otInteger);
         ptrQry->DeclareVariable("p_Show"            , otInteger);
         ptrQry->DeclareVariable("p_Codigo"          , otString );
         ptrQry->DeclareVariable("p_ExtId"           , otString );
         ptrQry->DeclareVariable("p_Vnom"            , otFloat  );
         ptrQry->DeclareVariable("p_Snom"            , otFloat  );
         ptrQry->DeclareVariable("p_ZR"              , otFloat  );
         ptrQry->DeclareVariable("p_ZX"              , otFloat  );
         ptrQry->DeclareVariable("p_Barra1Id"        , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"         , otFloat  );
         ptrQry->DeclareVariable("p_Barra2Id"        , otInteger);
         ptrQry->DeclareVariable("p_PosBar2"         , otFloat  );
         ptrQry->DeclareVariable("p_DxComentario"    , otInteger);
         ptrQry->DeclareVariable("p_DyComentario"    , otInteger);
         ptrQry->DeclareVariable("p_Comentario"      , otString );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop capseries (ini)
            capserie = (VTCapserie*)lisObjRede->Items[n];
            if (!capserie)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_CAPSERIEINV
                return(false);
                }
            // Verifica se o objeto é novo ou alterado
            if ((capserie->Id < 0) || (bolObra))
               {
               intCapSerieId = IDINDEFINIDO;
               }
            else
               {
               intCapSerieId = capserie->Id;
               }

            if (capserie->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;
               }
            else if (capserie->StatusAlterado() || capserie->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            // Seta valores dos parâmetros
            ptrQry->SetVariable("p_NovoReg"         , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"        , estudo->Id);
            ptrQry->SetVariable("p_RedeId"          , intRedeId);
            ptrQry->SetVariable("p_CapacitorSerieId", intCapSerieId);
            ptrQry->SetVariable("p_AlteradoObra"    , (int)bolObra);
            ptrQry->SetVariable("p_Show"            , capserie->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"          , capserie->Codigo);
            ptrQry->SetVariable("p_ExtId"           , (capserie->Extern_id.IsEmpty() ? capserie->Codigo : capserie->Extern_id));
            ptrQry->SetVariable("p_Vnom"            , capserie->vnom);
            ptrQry->SetVariable("p_Snom"            , capserie->snom);
            ptrQry->SetVariable("p_ZR"              , capserie->z.x);
            ptrQry->SetVariable("p_ZX"              , capserie->z.r);
            ptrQry->SetVariable("p_Barra1Id"        , capserie->pbarra1->Id);
            ptrQry->SetVariable("p_PosBar1"         , capserie->esq.posbar1);
            ptrQry->SetVariable("p_Barra2Id"        , capserie->pbarra2->Id);
            ptrQry->SetVariable("p_PosBar2"         , capserie->esq.posbar2);
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(capserie)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario"    , intDxComentario);
            ptrQry->SetVariable("p_DyComentario"    , intDyComentario);
            ptrQry->SetVariable("p_Comentario"      , strComentario);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               capserie->Id = ptrQry->GetVariable("p_CapacitorSerieId");
               if (capserie->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            // loop capseries (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrCapacitorSerie

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrMutua(VTEstudo* estudo, int intRedeId, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTMutua*      mutua;
   AnsiString    strlog;
   int           intMutuaId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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

   if (!lisObjRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrMutua(:p_NovoReg, :p_EstudoId, :p_RedeId, :p_MutuaId, :p_AlteradoObra, ");
         ptrQry->SQL->Add("                         :p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                         :p_Trecho1Id, :p_Trecho1BarraSntId, :p_Trecho1Pos, :p_Trecho2Id, :p_Trecho2BarraSntId, :p_Trecho2Pos, ");
         ptrQry->SQL->Add("                         :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X, ");
         ptrQry->SQL->Add("                         :p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                        );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"           , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"          , otInteger);
         ptrQry->DeclareVariable("p_RedeId"            , otInteger);
         ptrQry->DeclareVariable("p_MutuaId"           , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra"      , otInteger);
         ptrQry->DeclareVariable("p_Show"              , otInteger);
         ptrQry->DeclareVariable("p_Codigo"            , otString );
         ptrQry->DeclareVariable("p_ExtId"             , otString );
         ptrQry->DeclareVariable("p_Trecho1Id"         , otInteger);
         ptrQry->DeclareVariable("p_Trecho1BarraSntId" , otInteger);
         ptrQry->DeclareVariable("p_Trecho1Pos"        , otFloat  );
         ptrQry->DeclareVariable("p_Trecho2Id"         , otInteger);
         ptrQry->DeclareVariable("p_Trecho2BarraSntId" , otInteger);
         ptrQry->DeclareVariable("p_Trecho2Pos"        , otFloat  );
         ptrQry->DeclareVariable("p_Z0R"               , otFloat  );
         ptrQry->DeclareVariable("p_Z0X"               , otFloat  );
         ptrQry->DeclareVariable("p_Z1R"               , otFloat  );
         ptrQry->DeclareVariable("p_Z1X"               , otFloat  );
         ptrQry->DeclareVariable("p_DxComentario"      , otInteger);
         ptrQry->DeclareVariable("p_DyComentario"      , otInteger);
         ptrQry->DeclareVariable("p_Comentario"        , otString );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop mutuas (ini)
            mutua = (VTMutua*)lisObjRede->Items[n];
            if (!mutua)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_MUTUAINV
                return(false);
                }
            // Verifica se o objeto é novo ou alterado
            if ((mutua->Id < 0) || (bolObra))
               {
               intMutuaId = IDINDEFINIDO;
               }
            else
               {
               intMutuaId = mutua->Id;
               }

            if (mutua->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;
               }
            else if (mutua->StatusAlterado() || mutua->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }
               // Verificações adicionais
               if ((!mutua->ptre1) || (!mutua->ptre2))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_MUTUATREINV
                  return(false);
                  }
               if ((!mutua->pbar_tre1) || (!mutua->pbar_tre2))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_MUTUABARINV
                  return(false);
                  }

            // Seta valores dos parâmetros
            ptrQry->SetVariable("p_NovoReg"           , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"          , estudo->Id);
            ptrQry->SetVariable("p_RedeId"            , intRedeId);
            ptrQry->SetVariable("p_MutuaId"           , intMutuaId);
            ptrQry->SetVariable("p_AlteradoObra"      , (int)bolObra);
            ptrQry->SetVariable("p_Show"              , mutua->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"            , mutua->Codigo);
            ptrQry->SetVariable("p_ExtId"             , (mutua->Extern_id.IsEmpty() ? mutua->Codigo : mutua->Extern_id));
            ptrQry->SetVariable("p_Trecho1Id"         , mutua->ptre1->Id);
            ptrQry->SetVariable("p_Trecho1BarraSntId" , mutua->pbar_tre1->Id);
            ptrQry->SetVariable("p_Trecho1Pos"        , mutua->postre1);
            ptrQry->SetVariable("p_Trecho2Id"         , mutua->ptre2->Id);
            ptrQry->SetVariable("p_Trecho2BarraSntId" , mutua->pbar_tre2->Id);
            ptrQry->SetVariable("p_Trecho2Pos"        , mutua->postre2);
            ptrQry->SetVariable("p_Z0R"               , mutua->z0.x);
            ptrQry->SetVariable("p_Z0X"               , mutua->z0.r);
            ptrQry->SetVariable("p_Z1R"               , mutua->z1.x);
            ptrQry->SetVariable("p_Z1X"               , mutua->z1.r);
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(mutua)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario"      , intDxComentario);
            ptrQry->SetVariable("p_DyComentario"      , intDyComentario);
            ptrQry->SetVariable("p_Comentario"        , strComentario);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               mutua->Id = ptrQry->GetVariable("p_MutuaId");
               if (mutua->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            // loop mutuas (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrMutua

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrChave(VTEstudo* estudo, int intRedeId, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTChave*      chave;
   AnsiString    strlog;
   int           intChaveId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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

   if (!lisObjRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrChave(:p_NovoReg, :p_EstudoId, :p_RedeId, :p_ChaveId, :p_AlteradoObra, ");
         ptrQry->SQL->Add("                         :p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                         :p_TipoChaveId, :p_Estado, :p_Inom, ");
         ptrQry->SQL->Add("                         :p_Telecomando, :p_Protecao, :p_TecnoCom, ");
         ptrQry->SQL->Add("                         :p_Barra1Id, :p_PosBar1, :p_Barra2Id, :p_PosBar2, ");
         ptrQry->SQL->Add("                         :p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                        );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"     , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"    , otInteger);
         ptrQry->DeclareVariable("p_RedeId"      , otInteger);
         ptrQry->DeclareVariable("p_ChaveId"     , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra", otInteger);
         ptrQry->DeclareVariable("p_Show"        , otInteger);
         ptrQry->DeclareVariable("p_Codigo"      , otString );
         ptrQry->DeclareVariable("p_ExtId"       , otString );
         ptrQry->DeclareVariable("p_TipoChaveId" , otInteger);
         ptrQry->DeclareVariable("p_Estado"      , otInteger);
         ptrQry->DeclareVariable("p_Inom"        , otFloat  );
         ptrQry->DeclareVariable("p_Telecomando" , otInteger);
         ptrQry->DeclareVariable("p_Protecao"    , otInteger);
         ptrQry->DeclareVariable("p_TecnoCom"    , otInteger);
         ptrQry->DeclareVariable("p_Barra1Id"    , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"     , otFloat  );
         ptrQry->DeclareVariable("p_Barra2Id"    , otInteger);
         ptrQry->DeclareVariable("p_PosBar2"     , otFloat  );
         ptrQry->DeclareVariable("p_DxComentario", otInteger);
         ptrQry->DeclareVariable("p_DyComentario", otInteger);
         ptrQry->DeclareVariable("p_Comentario"  , otString );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop chaves (ini)
            chave = (VTChave*)lisObjRede->Items[n];
            if (!chave)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_CHAVEINV
                return(false);
                }
            // Verifica se o objeto é novo ou alterado
            if ((chave->Id < 0) || (bolObra))
               {
               intChaveId = IDINDEFINIDO;
               }
            else
               {
               intChaveId = chave->Id;
               }

            if (chave->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;
               }
            else if (chave->StatusAlterado() || chave->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }
            //CÓDIGO TEMPORÁRIO: atualiza Extern_id com informação de Elo fusível
            if (chave->TipoBaseFusivel)
               {//salva informação do Elo em Extern_id
               if (! chave->EloFusivel.IsEmpty()) chave->Extern_id = chave->EloFusivel + "_elo";
               }

            ptrQry->SetVariable("p_NovoReg"     , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"    , estudo->Id);
            ptrQry->SetVariable("p_RedeId"      , intRedeId);
            ptrQry->SetVariable("p_ChaveId"     , intChaveId);
            ptrQry->SetVariable("p_AlteradoObra", (int)bolObra);
            ptrQry->SetVariable("p_Show"        , chave->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"      , chave->Codigo);
            ptrQry->SetVariable("p_ExtId"       , (chave->Extern_id.IsEmpty() ? chave->Codigo : chave->Extern_id));
            // OOPS@20130625: Colocar os campos da chave aqui
            ptrQry->SetVariable("p_Telecomando" , 0);
            ptrQry->SetVariable("p_Protecao"    , 0);
            ptrQry->SetVariable("p_TecnoCom"    , 0);

            if (!chave->TipoChave)
               {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_CHAVETIPOINV
                return(false);
               }
            ptrQry->SetVariable("p_TipoChaveId" , chave->TipoChave->Id);
            ptrQry->SetVariable("p_Estado"      , chave->Estado);
            ptrQry->SetVariable("p_Inom"        , chave->Inom_amp);

            ptrQry->SetVariable("p_Barra1Id"    , chave->pbarra1->Id);
            ptrQry->SetVariable("p_PosBar1"     , chave->esq.posbar1);
            ptrQry->SetVariable("p_Barra2Id"    , chave->pbarra2->Id);
            ptrQry->SetVariable("p_PosBar2"     , chave->esq.posbar2);
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(chave)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario", intDxComentario);
            ptrQry->SetVariable("p_DyComentario", intDyComentario);
            ptrQry->SetVariable("p_Comentario"  , strComentario);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               chave->Id = ptrQry->GetVariable("p_ChaveId");
               if (chave->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            // loop chaves (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrChave

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrRegulador(VTEstudo* estudo, int intRedeId, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTRegulador*  regulador;
   AnsiString    strlog;
   int           intReguladorId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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

   if (!lisObjRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrRegulador(:p_NovoReg, :p_EstudoId, :p_RedeId, :p_ReguladorId, :p_AlteradoObra, ");
         ptrQry->SQL->Add("                             :p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                             :p_TipoLigacaoId, :p_Snom, :p_Bypass, :p_FaixaVar, :p_NumPasso, ");
         ptrQry->SQL->Add("                             :p_SentidoReversivel, :p_SentidoBarRefId, :p_AjusteAuto, :p_CfgFixPassoSel, :p_CfgAutBarRefId, ");
         ptrQry->SQL->Add("                             :p_CfgAutVpu, :p_Z0R, :p_Z0X, :p_Z1R, :p_Z1X, ");
         ptrQry->SQL->Add("                             :p_ModeloId, :p_TipoFaseId, ");
         ptrQry->SQL->Add("                             :p_Barra1Id, :p_PosBar1, :p_Barra2Id, :p_PosBar2, ");
         ptrQry->SQL->Add("                             :p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                             );");
         ptrQry->SQL->Add("END;");

         // Seta valores dos parâmetros
         ptrQry->DeclareVariable("p_NovoReg"          , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"         , otInteger);
         ptrQry->DeclareVariable("p_RedeId"           , otInteger);
         ptrQry->DeclareVariable("p_ReguladorId"      , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra"     , otInteger);
         ptrQry->DeclareVariable("p_Show"             , otInteger);
         ptrQry->DeclareVariable("p_Codigo"           , otString );
         ptrQry->DeclareVariable("p_ExtId"            , otString );
         ptrQry->DeclareVariable("p_TipoLigacaoId"    , otInteger);
         ptrQry->DeclareVariable("p_Snom"             , otFloat  );
         ptrQry->DeclareVariable("p_Bypass"           , otInteger);
         ptrQry->DeclareVariable("p_FaixaVar"         , otFloat  );
         ptrQry->DeclareVariable("p_NumPasso"         , otInteger);
         ptrQry->DeclareVariable("p_SentidoReversivel", otInteger);
         ptrQry->DeclareVariable("p_SentidoBarRefId"  , otInteger);
         ptrQry->DeclareVariable("p_AjusteAuto"       , otInteger);
         ptrQry->DeclareVariable("p_CfgFixPassoSel"   , otInteger);
         ptrQry->DeclareVariable("p_CfgAutBarRefId"   , otInteger);
         ptrQry->DeclareVariable("p_CfgAutVpu"        , otFloat  );
         ptrQry->DeclareVariable("p_Z0R"              , otFloat  );
         ptrQry->DeclareVariable("p_Z0X"              , otFloat  );
         ptrQry->DeclareVariable("p_Z1R"              , otFloat  );
         ptrQry->DeclareVariable("p_Z1X"              , otFloat  );
         ptrQry->DeclareVariable("p_ModeloId"         , otInteger);
         ptrQry->DeclareVariable("p_TipoFaseId"       , otInteger);
         ptrQry->DeclareVariable("p_Barra1Id"         , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"          , otFloat  );
         ptrQry->DeclareVariable("p_Barra2Id"         , otInteger);
         ptrQry->DeclareVariable("p_PosBar2"          , otFloat  );
         ptrQry->DeclareVariable("p_DxComentario"     , otInteger);
         ptrQry->DeclareVariable("p_DyComentario"     , otInteger);
         ptrQry->DeclareVariable("p_Comentario"       , otString );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop reguladores (ini)
            regulador = (VTRegulador*)lisObjRede->Items[n];
            if (!regulador)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_REGULADORINV
                return(false);
                }
            // Verifica se o objeto é novo ou alterado
            if ((regulador->Id < 0) || (bolObra))
               {
               intReguladorId = IDINDEFINIDO;
               }
            else
               {
               intReguladorId = regulador->Id;
               }

            if (regulador->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;
               }
            else if (regulador->StatusAlterado() || regulador->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            ptrQry->SetVariable("p_NovoReg"          , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"         , estudo->Id);
            ptrQry->SetVariable("p_RedeId"           , intRedeId);
            ptrQry->SetVariable("p_ReguladorId"      , intReguladorId);
            ptrQry->SetVariable("p_AlteradoObra"     , (int)bolObra);
            ptrQry->SetVariable("p_Show"             , regulador->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"           , regulador->Codigo);
            ptrQry->SetVariable("p_ExtId"            , (regulador->Extern_id.IsEmpty() ? regulador->Codigo : regulador->Extern_id));
            ptrQry->SetVariable("p_TipoLigacaoId"    , regulador->ligacao);
            ptrQry->SetVariable("p_Snom"             , regulador->snom);
            ptrQry->SetVariable("p_Bypass"           , regulador->by_pass);
            ptrQry->SetVariable("p_FaixaVar"         , regulador->var_tensao);
            ptrQry->SetVariable("p_NumPasso"         , regulador->num_passo);
//			ptrQry->SetVariable("p_SentidoReversivel", regulador->sentido.reversivel);
			ptrQry->SetVariable("p_SentidoReversivel", 1); //default temporario
			ptrQry->SetVariable("p_SentidoBarRefId"  , (regulador->sentido.bar_ref ? regulador->sentido.bar_ref->Id : IDINDEFINIDO));
//			ptrQry->SetVariable("p_AjusteAuto"       , regulador->ajuste_auto);
			ptrQry->SetVariable("p_AjusteAuto"       , 1); //default temporario
			ptrQry->SetVariable("p_CfgFixPassoSel"   , regulador->cfg_fixo.passo);
            ptrQry->SetVariable("p_CfgAutBarRefId"   , (regulador->cfg_auto.bar_ref ? regulador->cfg_auto.bar_ref->Id : IDINDEFINIDO));
            ptrQry->SetVariable("p_CfgAutVpu"        , regulador->cfg_auto.v_pu);
            ptrQry->SetVariable("p_Z0R"              , regulador->z0.r);
            ptrQry->SetVariable("p_Z0X"              , regulador->z0.x);
            ptrQry->SetVariable("p_Z1R"              , regulador->z1.r);
            ptrQry->SetVariable("p_Z1X"              , regulador->z1.x);
            ptrQry->SetVariable("p_ModeloId"         , regulador->modelo);
            ptrQry->SetVariable("p_TipoFaseId"       , regulador->fases);
            ptrQry->SetVariable("p_Barra1Id"         , regulador->pbarra1->Id);
            ptrQry->SetVariable("p_PosBar1"          , regulador->esq.posbar1);
            ptrQry->SetVariable("p_Barra2Id"         , regulador->pbarra2->Id);
            ptrQry->SetVariable("p_PosBar2"          , regulador->esq.posbar2);
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(regulador)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario"     , intDxComentario);
            ptrQry->SetVariable("p_DyComentario"     , intDyComentario);
            ptrQry->SetVariable("p_Comentario"       , strComentario);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               regulador->Id = ptrQry->GetVariable("p_ReguladorId");
               if (regulador->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            // loop reguladores (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrRegulador

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrTrafo(VTEstudo* estudo, int intRedeId, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTTrafo*      trafo;
   AnsiString    strlog;
   int           intTrafoId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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

   if (!lisObjRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrTrafo(:p_NovoReg, :p_EstudoId, :p_RedeId, :p_TrafoId, :p_AlteradoObra, ");
         ptrQry->SQL->Add("                         :p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                         :p_AutoTrafo, :p_PerdaFerro, :p_LtcTipoUtl, :p_LtcBarRefId,");
         ptrQry->SQL->Add("                         :p_LtcVpu, :pZ0R, :pZ0X, :pZ1R, :pZ1X,");
         ptrQry->SQL->Add("                         :p_Enrol1_TipoLigacaoId, :p_Enrol1_TipoFaseId, :p_Enrol1_Vnom, :p_Enrol1_Snom, :p_Enrol1_RotFase, :p_Enrol1_Vtap, :p_Enrol1_ZterraR, :p_Enrol1_ZterraX, ");
         ptrQry->SQL->Add("                         :p_Enrol2_TipoLigacaoId, :p_Enrol2_TipoFaseId, :p_Enrol2_Vnom, :p_Enrol2_Snom, :p_Enrol2_RotFase, :p_Enrol2_Vtap, :p_Enrol2_ZterraR, :p_Enrol2_ZterraX, ");
         ptrQry->SQL->Add("                         :p_Barra1Id, :p_PosBar1, :p_Barra2Id, :p_PosBar2, ");
         ptrQry->SQL->Add("                         :p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                        );");
         ptrQry->SQL->Add("END;");

         // Seta valores dos parâmetros
         ptrQry->DeclareVariable("p_NovoReg"             , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"            , otInteger);
         ptrQry->DeclareVariable("p_RedeId"              , otInteger);
         ptrQry->DeclareVariable("p_TrafoId"             , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra"        , otInteger);
         ptrQry->DeclareVariable("p_Show"                , otInteger);
         ptrQry->DeclareVariable("p_Codigo"              , otString );
         ptrQry->DeclareVariable("p_ExtId"               , otString );
         ptrQry->DeclareVariable("p_AutoTrafo"           , otInteger);
         ptrQry->DeclareVariable("p_PerdaFerro"          , otFloat  );
         ptrQry->DeclareVariable("p_LtcTipoUtl"          , otInteger);
         ptrQry->DeclareVariable("p_LtcBarRefId"         , otInteger);
         ptrQry->DeclareVariable("p_LtcVpu"              , otFloat  );
         ptrQry->DeclareVariable("pZ0R"                  , otFloat  );
         ptrQry->DeclareVariable("pZ0X"                  , otFloat  );
         ptrQry->DeclareVariable("pZ1R"                  , otFloat  );
         ptrQry->DeclareVariable("pZ1X"                  , otFloat  );
         ptrQry->DeclareVariable("p_Enrol1_TipoLigacaoId", otInteger);
         ptrQry->DeclareVariable("p_Enrol1_TipoFaseId"   , otInteger);
         ptrQry->DeclareVariable("p_Enrol1_Vnom"         , otFloat  );
         ptrQry->DeclareVariable("p_Enrol1_Snom"         , otFloat  );
         ptrQry->DeclareVariable("p_Enrol1_RotFase"      , otFloat  );
         ptrQry->DeclareVariable("p_Enrol1_Vtap"         , otFloat  );
         ptrQry->DeclareVariable("p_Enrol1_ZterraR"      , otFloat  );
         ptrQry->DeclareVariable("p_Enrol1_ZterraX"      , otFloat  );
         ptrQry->DeclareVariable("p_Enrol2_TipoLigacaoId", otInteger);
         ptrQry->DeclareVariable("p_Enrol2_TipoFaseId"   , otInteger);
         ptrQry->DeclareVariable("p_Enrol2_Vnom"         , otFloat  );
         ptrQry->DeclareVariable("p_Enrol2_Snom"         , otFloat  );
         ptrQry->DeclareVariable("p_Enrol2_RotFase"      , otFloat  );
         ptrQry->DeclareVariable("p_Enrol2_Vtap"         , otFloat  );
         ptrQry->DeclareVariable("p_Enrol2_ZterraR"      , otFloat  );
         ptrQry->DeclareVariable("p_Enrol2_ZterraX"      , otFloat  );
         ptrQry->DeclareVariable("p_Barra1Id"            , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"             , otFloat  );
         ptrQry->DeclareVariable("p_Barra2Id"            , otInteger);
         ptrQry->DeclareVariable("p_PosBar2"             , otFloat  );
         ptrQry->DeclareVariable("p_DxComentario"        , otInteger);
         ptrQry->DeclareVariable("p_DyComentario"        , otInteger);
         ptrQry->DeclareVariable("p_Comentario"          , otString );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop trafos (ini)
            trafo = (VTTrafo*)lisObjRede->Items[n];
            if (!trafo)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_TRAFOINV
                return(false);
                }
            // Verifica se o objeto é novo ou alterado
            if ((trafo->Id < 0) || (bolObra))
               {
               intTrafoId = IDINDEFINIDO;
               }
            else
               {
               intTrafoId = trafo->Id;
               }

            if (trafo->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;
               }
            else if (trafo->StatusAlterado() || trafo->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            ptrQry->SetVariable("p_NovoReg"             , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"            , estudo->Id);
            ptrQry->SetVariable("p_RedeId"              , intRedeId);
            ptrQry->SetVariable("p_TrafoId"             , intTrafoId);
            ptrQry->SetVariable("p_AlteradoObra"        , (int)bolObra);
            ptrQry->SetVariable("p_Show"                , trafo->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"              , trafo->Codigo);
            ptrQry->SetVariable("p_ExtId"               , (trafo->Extern_id.IsEmpty() ? trafo->Codigo : trafo->Extern_id));
            ptrQry->SetVariable("p_AutoTrafo"           , trafo->auto_trafo);
            ptrQry->SetVariable("p_PerdaFerro"          , trafo->perda_ferro);
				ptrQry->SetVariable("p_LtcTipoUtl"          , trafo->ltc.tipo);
				ptrQry->SetVariable("p_LtcBarRefId"         , (trafo->ltc.ajuste_auto.bar_ref ? trafo->ltc.ajuste_auto.bar_ref->Id : IDINDEFINIDO));
				ptrQry->SetVariable("p_LtcVpu"              , (trafo->ltc.ajuste_auto.v_pu));
            ptrQry->SetVariable("pZ0R"                  , trafo->z0.r);
            ptrQry->SetVariable("pZ0X"                  , trafo->z0.x);
            ptrQry->SetVariable("pZ1R"                  , trafo->z1.r);
            ptrQry->SetVariable("pZ1X"                  , trafo->z1.x);
            ptrQry->SetVariable("p_Enrol1_TipoLigacaoId", trafo->pri.ligacao);
            ptrQry->SetVariable("p_Enrol1_TipoFaseId"   , trafo->pri.fase);
            ptrQry->SetVariable("p_Enrol1_Vnom"         , trafo->pri.vnom);
            ptrQry->SetVariable("p_Enrol1_Snom"         , trafo->snom);
            ptrQry->SetVariable("p_Enrol1_RotFase"      , trafo->pri.defasagem);
            ptrQry->SetVariable("p_Enrol1_Vtap"         , trafo->pri.tap);
            ptrQry->SetVariable("p_Enrol1_ZterraR"      , trafo->pri.zterra.r);
            ptrQry->SetVariable("p_Enrol1_ZterraX"      , trafo->pri.zterra.x);
            ptrQry->SetVariable("p_Enrol2_TipoLigacaoId", trafo->sec.ligacao);
            ptrQry->SetVariable("p_Enrol2_TipoFaseId"   , trafo->sec.fase);
            ptrQry->SetVariable("p_Enrol2_Vnom"         , trafo->sec.vnom);
            ptrQry->SetVariable("p_Enrol2_Snom"         , trafo->snom);
            ptrQry->SetVariable("p_Enrol2_RotFase"      , trafo->sec.defasagem);
            ptrQry->SetVariable("p_Enrol2_Vtap"         , trafo->sec.tap);
            ptrQry->SetVariable("p_Enrol2_ZterraR"      , trafo->sec.zterra.r);
            ptrQry->SetVariable("p_Enrol2_ZterraX"      , trafo->sec.zterra.x);
            ptrQry->SetVariable("p_Barra1Id"            , trafo->BarraPrimario()->Id);
            ptrQry->SetVariable("p_PosBar1"             , trafo->esq.posbar1);
            ptrQry->SetVariable("p_Barra2Id"            , trafo->BarraSecundario()->Id);
            ptrQry->SetVariable("p_PosBar2"             , trafo->esq.posbar2);
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(trafo)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario"       , intDxComentario);
            ptrQry->SetVariable("p_DyComentario"       , intDyComentario);
            ptrQry->SetVariable("p_Comentario"         , strComentario);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               trafo->Id = ptrQry->GetVariable("p_TrafoId");
               if (trafo->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            // loop trafos (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrTrafo

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrTrafo3E(VTEstudo* estudo, int intRedeId, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTTrafo3E*    trafo3e;
   AnsiString    strlog;
   int           intTrafo3eId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   VTComentario* comentario;
   int           intDxComentario;
   int           intDyComentario;
   AnsiString    strComentario;

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

   if (!lisObjRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      Erro("Objeto redes inválido.");
      return(NULL);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;
         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrTrafo3e(:p_NovoReg, :p_EstudoId, :p_RedeId, :p_Trafo3eId, :p_AlteradoObra, ");
         ptrQry->SQL->Add("                           :p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                           :p_PerdaFerro, :p_Sbase, ");
         ptrQry->SQL->Add("                           :p_ZPS0R, :p_ZPS0X, :p_ZPT0R, :p_ZPT0X, :p_ZST0R, :p_ZST0X, :p_ZPS1R, :p_ZPS1X, :p_ZPT1R, :p_ZPT1X, :p_ZST1R, :p_ZST1X, ");
         ptrQry->SQL->Add("                           :p_Enrol1_TipoLigacaoId, :p_Enrol1_TipoFaseId, :p_Enrol1_Vnom, :p_Enrol1_Snom, :p_Enrol1_RotFase, :p_Enrol1_Vtap, :p_Enrol1_ZterraR, :p_Enrol1_ZterraX, ");
         ptrQry->SQL->Add("                           :p_Enrol2_TipoLigacaoId, :p_Enrol2_TipoFaseId, :p_Enrol2_Vnom, :p_Enrol2_Snom, :p_Enrol2_RotFase, :p_Enrol2_Vtap, :p_Enrol2_ZterraR, :p_Enrol2_ZterraX, ");
         ptrQry->SQL->Add("                           :p_Enrol3_TipoLigacaoId, :p_Enrol3_TipoFaseId, :p_Enrol3_Vnom, :p_Enrol3_Snom, :p_Enrol3_RotFase, :p_Enrol3_Vtap, :p_Enrol3_ZterraR, :p_Enrol3_ZterraX, ");
         ptrQry->SQL->Add("                           :p_Barra1Id, :p_PosBar1, :p_Barra2Id, :p_PosBar2, :p_Barra3Id, :p_PosBar3, ");
         ptrQry->SQL->Add("                           :p_DxComentario, :p_DyComentario, :p_Comentario ");
         ptrQry->SQL->Add("                          );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"             , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"            , otInteger);
         ptrQry->DeclareVariable("p_RedeId"              , otInteger);
         ptrQry->DeclareVariable("p_Trafo3eId"           , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra"        , otInteger);
         ptrQry->DeclareVariable("p_Show"                , otInteger);
         ptrQry->DeclareVariable("p_Codigo"              , otString );
         ptrQry->DeclareVariable("p_ExtId"               , otString );
         ptrQry->DeclareVariable("p_PerdaFerro"          , otFloat  );
         ptrQry->DeclareVariable("p_Sbase"               , otFloat  );
         ptrQry->DeclareVariable("p_ZPS0R"               , otFloat  );
         ptrQry->DeclareVariable("p_ZPS0X"               , otFloat  );
         ptrQry->DeclareVariable("p_ZPT0R"               , otFloat  );
         ptrQry->DeclareVariable("p_ZPT0X"               , otFloat  );
         ptrQry->DeclareVariable("p_ZST0R"               , otFloat  );
         ptrQry->DeclareVariable("p_ZST0X"               , otFloat  );
         ptrQry->DeclareVariable("p_ZPS1R"               , otFloat  );
         ptrQry->DeclareVariable("p_ZPS1X"               , otFloat  );
         ptrQry->DeclareVariable("p_ZPT1R"               , otFloat  );
         ptrQry->DeclareVariable("p_ZPT1X"               , otFloat  );
         ptrQry->DeclareVariable("p_ZST1R"               , otFloat  );
         ptrQry->DeclareVariable("p_ZST1X"               , otFloat  );
         ptrQry->DeclareVariable("p_Enrol1_TipoLigacaoId", otInteger);
         ptrQry->DeclareVariable("p_Enrol1_TipoFaseId"   , otInteger);
         ptrQry->DeclareVariable("p_Enrol1_Vnom"         , otFloat  );
         ptrQry->DeclareVariable("p_Enrol1_Snom"         , otFloat  );
         ptrQry->DeclareVariable("p_Enrol1_RotFase"      , otFloat  );
         ptrQry->DeclareVariable("p_Enrol1_Vtap"         , otFloat  );
         ptrQry->DeclareVariable("p_Enrol1_ZterraR"      , otFloat  );
         ptrQry->DeclareVariable("p_Enrol1_ZterraX"      , otFloat  );
         ptrQry->DeclareVariable("p_Enrol2_TipoLigacaoId", otInteger);
         ptrQry->DeclareVariable("p_Enrol2_TipoFaseId"   , otInteger);
         ptrQry->DeclareVariable("p_Enrol2_Vnom"         , otFloat  );
         ptrQry->DeclareVariable("p_Enrol2_Snom"         , otFloat  );
         ptrQry->DeclareVariable("p_Enrol2_RotFase"      , otFloat  );
         ptrQry->DeclareVariable("p_Enrol2_Vtap"         , otFloat  );
         ptrQry->DeclareVariable("p_Enrol2_ZterraR"      , otFloat  );
         ptrQry->DeclareVariable("p_Enrol2_ZterraX"      , otFloat  );
         ptrQry->DeclareVariable("p_Enrol3_TipoLigacaoId", otInteger);
         ptrQry->DeclareVariable("p_Enrol3_TipoFaseId"   , otInteger);
         ptrQry->DeclareVariable("p_Enrol3_Vnom"         , otFloat  );
         ptrQry->DeclareVariable("p_Enrol3_Snom"         , otFloat  );
         ptrQry->DeclareVariable("p_Enrol3_RotFase"      , otFloat  );
         ptrQry->DeclareVariable("p_Enrol3_Vtap"         , otFloat  );
         ptrQry->DeclareVariable("p_Enrol3_ZterraR"      , otFloat  );
         ptrQry->DeclareVariable("p_Enrol3_ZterraX"      , otFloat  );
         ptrQry->DeclareVariable("p_Barra1Id"            , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"             , otFloat  );
         ptrQry->DeclareVariable("p_Barra2Id"            , otInteger);
         ptrQry->DeclareVariable("p_PosBar2"             , otFloat  );
         ptrQry->DeclareVariable("p_Barra3Id"            , otInteger);
         ptrQry->DeclareVariable("p_PosBar3"             , otFloat  );
         ptrQry->DeclareVariable("p_DxComentario"        , otInteger);
         ptrQry->DeclareVariable("p_DyComentario"        , otInteger);
         ptrQry->DeclareVariable("p_Comentario"          , otString );

      for (int n = 0; n < lisObjRede->Count; n++)
         {
         // loop trafos 3e (ini)
         trafo3e = (VTTrafo3E*)lisObjRede->Items[n];
         if (!trafo3e)
             {
             SNPSTRLOGLINE(strlog);
             MSGERRSYS_TRAFO3EINV
             return(false);
             }
         // Verifica se o objeto é novo ou alterado
         if (trafo3e->Id < 0)
            {
            intTrafo3eId = IDINDEFINIDO;
            }
         else
            {
            intTrafo3eId = trafo3e->Id;
            }

         if (trafo3e->StatusNovo())
            {
            // Objeto novo. Tem que atualizar em memória o valor do identificador
            // retornado pela base de dados.
            bolNovoReg = true;
            }
         else if (trafo3e->StatusAlterado() || trafo3e->StatusDeslocado())
            {
            // Objeto previamente existente. O identificador em memória é válido.
            bolNovoReg = false;
            }
            else
               {
               // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
               continue;
               }

         // Seta valores dos parâmetros
         ptrQry->SetVariable("p_NovoReg"             , (bolNovoReg ? 1 : 0));
         ptrQry->SetVariable("p_EstudoId"            , estudo->Id);
         ptrQry->SetVariable("p_RedeId"              , intRedeId);
         ptrQry->SetVariable("p_Trafo3eId"           , intTrafo3eId);
         ptrQry->SetVariable("p_AlteradoObra"        , (int)bolObra);
         ptrQry->SetVariable("p_Show"                , trafo3e->ShowAsUnsigned);
         ptrQry->SetVariable("p_Codigo"              , trafo3e->Codigo);
         ptrQry->SetVariable("p_ExtId"               , (trafo3e->Extern_id.IsEmpty() ? trafo3e->Codigo : trafo3e->Extern_id));
         ptrQry->SetVariable("p_PerdaFerro"          , trafo3e->perda_ferro);
         ptrQry->SetVariable("p_Sbase"               , trafo3e->Sbase);
         ptrQry->SetVariable("p_ZPS0R"               , trafo3e->zps0.r);
         ptrQry->SetVariable("p_ZPS0X"               , trafo3e->zps0.x);
         ptrQry->SetVariable("p_ZPT0R"               , trafo3e->zpt0.r);
         ptrQry->SetVariable("p_ZPT0X"               , trafo3e->zpt0.x);
         ptrQry->SetVariable("p_ZST0R"               , trafo3e->zst0.r);
         ptrQry->SetVariable("p_ZST0X"               , trafo3e->zst0.x);
         ptrQry->SetVariable("p_ZPS1R"               , trafo3e->zps1.r);
         ptrQry->SetVariable("p_ZPS1X"               , trafo3e->zps1.x);
         ptrQry->SetVariable("p_ZPT1R"               , trafo3e->zpt1.r);
         ptrQry->SetVariable("p_ZPT1X"               , trafo3e->zpt1.x);
         ptrQry->SetVariable("p_ZST1R"               , trafo3e->zst1.r);
         ptrQry->SetVariable("p_ZST1X"               , trafo3e->zst1.x);
         ptrQry->SetVariable("p_Enrol1_TipoLigacaoId", trafo3e->pri.ligacao);
         ptrQry->SetVariable("p_Enrol1_TipoFaseId"   , trafo3e->Fases(trafo3e->pbarra1));
         ptrQry->SetVariable("p_Enrol1_Vnom"         , trafo3e->pri.vnom);
         ptrQry->SetVariable("p_Enrol1_Snom"         , trafo3e->pri.snom);
         ptrQry->SetVariable("p_Enrol1_RotFase"      , trafo3e->pri.defasagem);
         ptrQry->SetVariable("p_Enrol1_Vtap"         , trafo3e->pri.tap);
         ptrQry->SetVariable("p_Enrol1_ZterraR"      , trafo3e->pri.zterra.r);
         ptrQry->SetVariable("p_Enrol1_ZterraX"      , trafo3e->pri.zterra.x);
         ptrQry->SetVariable("p_Enrol2_TipoLigacaoId", trafo3e->sec.ligacao);
         ptrQry->SetVariable("p_Enrol2_TipoFaseId"   , trafo3e->Fases(trafo3e->pbarra2));
         ptrQry->SetVariable("p_Enrol2_Vnom"         , trafo3e->sec.vnom);
         ptrQry->SetVariable("p_Enrol2_Snom"         , trafo3e->sec.snom);
         ptrQry->SetVariable("p_Enrol2_RotFase"      , trafo3e->sec.defasagem);
         ptrQry->SetVariable("p_Enrol2_Vtap"         , trafo3e->sec.tap);
         ptrQry->SetVariable("p_Enrol2_ZterraR"      , trafo3e->sec.zterra.r);
         ptrQry->SetVariable("p_Enrol2_ZterraX"      , trafo3e->sec.zterra.x);
         ptrQry->SetVariable("p_Enrol3_TipoLigacaoId", trafo3e->ter.ligacao);
         ptrQry->SetVariable("p_Enrol3_TipoFaseId"   , trafo3e->Fases(trafo3e->pbarra3));
         ptrQry->SetVariable("p_Enrol3_Vnom"         , trafo3e->ter.vnom);
         ptrQry->SetVariable("p_Enrol3_Snom"         , trafo3e->ter.snom);
         ptrQry->SetVariable("p_Enrol3_RotFase"      , trafo3e->ter.defasagem);
         ptrQry->SetVariable("p_Enrol3_Vtap"         , trafo3e->ter.tap);
         ptrQry->SetVariable("p_Enrol3_ZterraR"      , trafo3e->ter.zterra.r);
         ptrQry->SetVariable("p_Enrol3_ZterraX"      , trafo3e->ter.zterra.x);
         ptrQry->SetVariable("p_Barra1Id"            , trafo3e->pbarra1->Id);
         ptrQry->SetVariable("p_PosBar1"             , trafo3e->esq.posbar1);
         ptrQry->SetVariable("p_Barra2Id"            , trafo3e->pbarra2->Id);
         ptrQry->SetVariable("p_PosBar2"             , trafo3e->esq.posbar2);
         ptrQry->SetVariable("p_Barra3Id"            , trafo3e->pbarra3->Id);
         ptrQry->SetVariable("p_PosBar3"             , trafo3e->esq.posbar3);
            // Definições dos comentários
            intDxComentario = IDINDEFINIDO;
            intDyComentario = IDINDEFINIDO;
            strComentario = "";
            if ((comentario = redes->ExisteComentario(trafo3e)) != NULL)
               {
               comentario->CoordenadasEsquematico(intDxComentario, intDyComentario);
               strComentario = comentario->Texto;
               }
            ptrQry->SetVariable("p_DxComentario"     , intDxComentario);
            ptrQry->SetVariable("p_DyComentario"     , intDyComentario);
            ptrQry->SetVariable("p_Comentario"       , strComentario);

         // Executa query
         ptrQry->Execute();

         // Verifica o valor retornado a ser atualizado no objeto em memória
         if (bolNovoReg)
            {
            trafo3e->Id = ptrQry->GetVariable("p_Trafo3eId");
            if (trafo3e->Id == IDINDEFINIDO)
               {
               // Condição de erro
               sucesso = false;
               SNPSTRLOGLINE(strlog);
               }
            }
         // loop trafos 3e (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrTrafo3e

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrConjCurvaEquipamento(VTEstudo* estudo, VTEqpto* eqpto, VTCurva* curva)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry     = NULL;
   TLOBLocator*  lobLocator = NULL;

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

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrConjCurvaEquipamento(:p_EstudoId, :p_EquipId, :p_ConjCurvaId, :p_Descricao, :p_Tipica, :p_Status, :p_TipoCurvaId, :p_Pontos); ");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_EstudoId"   , otInteger);
         ptrQry->DeclareVariable("p_EquipId"    , otInteger);
         ptrQry->DeclareVariable("p_ConjCurvaId", otInteger);
         ptrQry->DeclareVariable("p_Descricao"  , otString );
         ptrQry->DeclareVariable("p_Tipica"     , otInteger);
         ptrQry->DeclareVariable("p_Status"     , otInteger);
         ptrQry->DeclareVariable("p_TipoCurvaId", otInteger);
         ptrQry->DeclareVariable("p_Pontos"     , otBLOB   );

         // Seta valores dos parâmetros
         ptrQry->SetVariable("p_EstudoId"   , estudo->Id);
         ptrQry->SetVariable("p_EquipId"    , eqpto->Id);
         ptrQry->SetVariable("p_ConjCurvaId", (curva->Id <= 0 ? IDINDEFINIDO : curva->Id));
         ptrQry->SetVariable("p_Descricao"  , (curva->Codigo.IsEmpty() ? eqpto->Codigo : curva->Codigo));
         ptrQry->SetVariable("p_Tipica"     , (int)curva->Tipica);
         ptrQry->SetVariable("p_Status"     , (int)curva->StatusAsUnsigned);
         ptrQry->SetVariable("p_TipoCurvaId", curva->Tipo);

         lobLocator = new TLOBLocator(bd_session, otBLOB, true);
         ptrQry->SetComplexVariable("p_Pontos", lobLocator);
         if (bd_func->MontaStreamCurvaEquipamento(eqpto->Tipo(), curva, lobLocator))
            {
            // A stream tem que existir durante a execução do comando
            // delete ptrStream;
            // ptrStream = NULL;
            }
         else
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_CURVASTREAM
            return(false);
            }

         // Executa query
         ptrQry->Execute();

         // Verifica o valor retornado a ser atualizado no objeto em memória
         curva->Id = ptrQry->GetVariable("p_ConjCurvaId");
         if (curva->Id == IDINDEFINIDO)
            {
            // Condição de erro
            sucesso = false;
            SNPSTRLOGLINE(strlog);
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
      //destrói objetos locais
      if (lobLocator) { delete lobLocator; lobLocator = NULL; }
      if (ptrQry)     { delete ptrQry;     ptrQry = NULL;     }
      }

   return(sucesso);
   } // SPSNP_wrConjCurvaEquipamento

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_rdEstudo(void)
   {
   //variáveis locais
   bool           sucesso = true;
   AnsiString     strlog;
   strPLAN_ESTUDO str_estudo;
   TOracleQuery*  ptrQry    = NULL;
   TOracleQuery*  ptrCursor = NULL;
   VTEstudo*      estudo;

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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdEstudo(:p_EstudoId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId", otInteger);

         ptrQry->SetVariable("p_EstudoId", IDINDEFINIDO);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_estudo.estudo_id = ptrCursor->Field("EstudoId");
            str_estudo.tipo      = ptrCursor->Field("EstudoTipoId");
            str_estudo.codigo    = ptrCursor->Field("EstudoDescricao");
            str_estudo.autor     = ptrCursor->Field("EstudoProprietario");
            str_estudo.data      = ptrCursor->FieldAsDate(System::String("EstudoModificacao"));

            if ((estudo = monta->InsereEstudo(str_estudo)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_ESTUDOINV
               sucesso = false;
               break;
               }

            // Define o estado como false
            estudo->Novo = false;
            
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
   } // SPSNP_rdEstudo

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_rdEstudoAlternativa(VTEstudo* estudo)
   {
   //variáveis locais
   bool                sucesso = true;
   AnsiString          strlog;

   // Ações, Obras e Alternativas
   strPLAN_ESTUDO      str_estudo;
   strPLAN_ALTERNATIVA str_alternativa;
   strPLAN_OBRA        str_obra;
   strPLAN_ACAO        str_acao;
   int                 intAlternativaId;
   int                 intAlternativaIdAnterior;
   int                 intObraId;
   int                 intObraIdAnterior;
   int                 intAcaoId;
   TOracleQuery*       ptrQry = NULL;
   TOracleQuery*       ptrCursor = NULL;

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

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdEstudoAlternativa(:p_EstudoId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId", otInteger);

         ptrQry->SetVariable("p_EstudoId", estudo->Id);

         intAlternativaIdAnterior = IDINDEFINIDO;
         intObraIdAnterior        = IDINDEFINIDO;
         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            intAlternativaId = ptrCursor->Field("AlternativaId");
            if (intAlternativaId != intAlternativaIdAnterior)
               {
               if (intAlternativaIdAnterior == IDINDEFINIDO)
                  {
                  // Primeira alternativa, então salva o estudo
                  str_estudo.estudo_id = ptrCursor->Field("EstudoId");
                  // !OOPS! Desconsidera o estudo zero
                  str_estudo.codigo    = ptrCursor->Field("EstudoDescricao");
                  str_estudo.autor     = ptrCursor->Field("EstudoProprietario");
                  str_estudo.data      = ptrCursor->FieldAsDate(System::String("EstudoModificacao"));
                  if (!monta->InsereEstudo(str_estudo))
                     {
                     SNPSTRLOGLINE(strlog);
                     MSGERRSYS_ESTUDOINV
                     sucesso = false;
                     break;
                     }
                  }

               // Nova Alternativa (ini)
               str_alternativa.estudo_id      = str_estudo.estudo_id;
               str_alternativa.alternativa_id = intAlternativaId;
               str_alternativa.codigo         = ptrCursor->Field("AlternativaDescricao");
               // !OOPS! gambiarra para não inserir a rede zero duas vezes
               if (!monta->InsereEstudo(str_alternativa))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_ALTERNATIVAINV
                  sucesso = false;
                  break;
                  }

               intAlternativaIdAnterior = intAlternativaId;
               intObraIdAnterior = IDINDEFINIDO;
               // Nova Alternativa (fim)
               }

            intObraId = ptrCursor->Field("ObraId");
            if (intObraId > 0)
               {
               if (intObraId != intObraIdAnterior)
                  {
                  // Nova Obra (ini)
                  str_obra.estudo_id      = str_alternativa.estudo_id;
                  str_obra.alternativa_id = str_alternativa.alternativa_id;
                  str_obra.obra_id        = intObraId;
                  str_obra.codigo         = ptrCursor->Field("ObraDescricao");
                  //str_obra.data           = ptrCursor->FieldAsDate(System::String("ObraDataProjeto"));
                  str_obra.data_inicio    = ptrCursor->FieldAsDate(System::String("ObraDataProjeto"));
                  str_obra.data_ativacao  = ptrCursor->FieldAsDate(System::String("ObraDataExecucao"));
                  str_obra.responsavel    = ptrCursor->Field("ObraResponsavel");
                  str_obra.ob_especial    = ptrCursor->Field("ObraObrigEspecial");
                  str_obra.custo          = ptrCursor->Field("ObraCusto");
                  //str_obra.custo_om       = ptrCursor->Field("DeltaCustoOM");
//fim             =============================
//JCG 2016.07.29 - dados adicionais
                  str_obra.altera_taxa_falha = ptrCursor->Field("bolAlteraTaxaFalha").operator bool();
                  str_obra.taxa_falha_ins    = ptrCursor->Field("fltTaxaFalhaIns");
                  str_obra.taxa_falha_sub    = ptrCursor->Field("fltTaxaFalhaSub");
//fim             =============================

                  if (!monta->InsereEstudo(str_obra))
                     {
                     SNPSTRLOGLINE(strlog);
                     MSGERRSYS_OBRAINV
                     sucesso = false;
                     break;
                     }
                  intObraIdAnterior = intObraId;
                  // Nova Obra (fim)
                  }

               intAcaoId = ptrCursor->Field("AcaoId");
               if (intAcaoId > 0)
                  {
                  // Insere a nova ação
                  str_acao.estudo_id          = str_obra.estudo_id;
                  str_acao.alternativa_id     = str_obra.alternativa_id;
                  str_acao.obra_id            = str_obra.obra_id;
                  str_acao.tipo_acao          = ptrCursor->Field("TipoAcaoId");
                  str_acao.tipo_eqpto         = ptrCursor->Field("TipoEquipId");
                  str_acao.eqpto_id           = ptrCursor->Field("EquipOriginalId");
                  str_acao.eqpto_id           = ptrCursor->Field("EquipOriginalId");
                  str_acao.eqpto_antes_id     = ptrCursor->Field("EquipEstadoPreId");
                  str_acao.eqpto_depos_id     = ptrCursor->Field("EquipEstadoPosId");

                  if (!monta->InsereEstudo(str_acao))
                     {
                     //IMPORTANTE:  JCG 2013.07.16
                     //Existem Acoes de alterar barras fictícias de redução. Estas Barras não
                     //podem ser que localizadas pois não são salvas na Base.
                     //Estas Ações não devem ser consideradas erros
                     //SNPSTRLOGLINE(strlog);
                     //MSGERRSYS_ACAOINV
                     //sucesso = false;
                     //break;
                     if (str_acao.tipo_acao != acaoALTERA)
                        {
                        SNPSTRLOGLINE(strlog);
                        MSGERRSYS_ACAOINV
                        sucesso = false;
                        break;
                        }
                     }
                  }
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
   } // SPSNP_rdEstudoAlternativa

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_dlEstudo(VTEstudo* estudo, bool bolRedeInclusive)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   VTMRede*      mrede;

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

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_dlEstudo(:p_EstudoId, :p_RedeInclusive);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_EstudoId", otInteger);
         ptrQry->SetVariable("p_EstudoId", estudo->Id);
         ptrQry->DeclareVariable("p_RedeInclusive", otInteger);
         ptrQry->SetVariable("p_RedeInclusive", bolRedeInclusive ? 1 : 0);

         // Executa query
         ptrQry->Execute();

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
   } // SPSNP_dlEstudo

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_dlEstudoCorte(VTEstudo* estudo, TList* lisMRede)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   VTMRede*      mrede;

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

   if (!lisMRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISMREDEINV
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_dlEstudoCorte(:p_EstudoId, :p_MRedeId);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_EstudoId", otInteger);
         ptrQry->SetVariable("p_EstudoId", estudo->Id);
         ptrQry->DeclareVariable("p_MRedeId", otInteger);
         
         for (int i = 0; i < lisMRede->Count; i++)
            {
            if ((mrede = (VTMRede*)lisMRede->Items[i]) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_MREDEINV
               return(false);
               }
            //verifica se foram criadas as Reducoes da MRede
            if ((! mrede->Carregada)||(! mrede->Status[sttENABLED])) continue;

            ptrQry->SetVariable("p_MRedeId", mrede->Id);

            // Executa query
            ptrQry->Execute();
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_dlEstudoCorte

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrEstudoCorteNaoZero(VTEstudo* estudo, TList* lisMRede)
   {
   //variáveis locais
   bool          sucesso = true;
   VTCorte*      corte;
   AnsiString    strlog;
   bool          bolNovoReg;
   TOracleQuery* ptrQry       = NULL;
   VTMRede*      mrede        = NULL;
   TList*        lisCorte     = NULL;

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

   if (!lisMRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISMREDEINV
      return(NULL);
      }

   __try
      {
      // Lista auxiliar para Cortes
      lisCorte = new TList();
      ptrQry   = new TOracleQuery(NULL); ptrQry->Session = bd_session;

      try
         {
         // Preenche a lista de cortes
         for (int n = 0; n < lisMRede->Count; n++)
            {
            // loop cortes (ini)
            if ((mrede = (VTMRede*)lisMRede->Items[n]) == NULL)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_MREDEINV
                return(false);
                }
            //insere Cortes da MRede em lisCorte
            mrede->LisCorte(lisCorte);
            }
         //redefine ID dos Cortes em lisCorte
         for (int j = 0; j < lisCorte->Count; j++)
            {
            if ((corte = (VTCorte*)lisCorte->Items[j]) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_CORTEINV
               return(false);
               }
            corte->Id = IDINDEFINIDO;
            }

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrEstudoCorte(:p_EstudoId, :p_MRedeId, :p_CorteId);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_EstudoId", otInteger);
         ptrQry->DeclareVariable("p_MRedeId" , otInteger);
         ptrQry->DeclareVariable("p_CorteId" , otInteger);

         // loop cortes (fim)
         for (int n = 0; n < lisMRede->Count; n++)
            {
            mrede = (VTMRede*)lisMRede->Items[n];
            //obtém lista de Cortes de MRede
            lisCorte->Clear();
            mrede->LisCorte(lisCorte);
            //loop p/ todos Cortes da MRede
            for (int j = 0; j < lisCorte->Count; j++)
               {
               if ((corte = (VTCorte*)lisCorte->Items[j]) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CORTEINV
                  return(false);
                  }

               ptrQry->SetVariable("p_EstudoId", estudo->Id);
               ptrQry->SetVariable("p_MRedeId" , mrede->Id);
               ptrQry->SetVariable("p_CorteId" , corte->Id);

               // Executa query
               ptrQry->Execute();

               corte->Id = ptrQry->GetVariable("p_CorteId");
               if (corte->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CORTEINV
                  sucesso = false;
                  break;
                  }
               }
            }
         // loop cortes (fim)

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
      if (ptrQry)   { delete ptrQry;   ptrQry   = NULL; }
      if (lisCorte) { delete lisCorte; lisCorte = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrEstudoCorteNaoZero

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrEstudoCorteZero(VTEstudo* estudo, TList* lisMRede)
   {
   //variáveis locais
   bool          sucesso = true;
   VTCorte*      corte;
   AnsiString    strlog;
   bool          bolNovoReg;
   TOracleQuery* ptrQry       = NULL;
   VTMRede*      mrede        = NULL;
   TList*        lisCorte     = NULL;
   TList*        lisCORTE_EXT = NULL;

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

   if (!lisMRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISMREDEINV
      return(NULL);
      }

   __try
      {
      // Lista auxiliar para Cortes
      lisCorte = new TList();
      ptrQry   = new TOracleQuery(NULL); ptrQry->Session = bd_session;

      try
         {
         // Preenche a lista de cortes
         for (int n = 0; n < lisMRede->Count; n++)
            {
            // loop cortes (ini)
            if ((mrede = (VTMRede*)lisMRede->Items[n]) == NULL)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_MREDEINV
                return(false);
                }
            //não salva Cortes das MRedes não carregadas
            if ((! mrede->Carregada)||(! mrede->Status[sttENABLED])) continue;
            //insere em lisCorte os Cortes da MRede que conectam apenas MRedes carregadas
            lisCORTE_EXT = mrede->LisCorte();
            for (int j = 0; j < lisCORTE_EXT->Count; j++)
               {
               corte = (VTCorte*)lisCORTE_EXT->Items[j];
               //verifica se o Corte conecta alguma rede não carregada
               if (corte->NumeroMRedes() == corte->NumeroMRedesCarregadas())
                  {
                  if (lisCorte->IndexOf(corte) < 0) lisCorte->Add(corte);
                  }
               }
            }
         //redefine ID dos Cortes em lisCorte
         for (int j = 0; j < lisCorte->Count; j++)
            {
            if ((corte = (VTCorte*)lisCorte->Items[j]) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_CORTEINV
               return(false);
               }
            corte->Id = IDINDEFINIDO;
            }

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrEstudoCorte(:p_EstudoId, :p_MRedeId, :p_CorteId);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_EstudoId", otInteger);
         ptrQry->DeclareVariable("p_MRedeId" , otInteger);
         ptrQry->DeclareVariable("p_CorteId" , otInteger);

         // loop cortes (fim)
         for (int n = 0; n < lisMRede->Count; n++)
            {
            mrede = (VTMRede*)lisMRede->Items[n];
            if (! mrede->Carregada) continue;
            //obtém lista de Cortes de MRede
            lisCorte->Clear();
            mrede->LisCorte(lisCorte);
            //loop p/ todos Cortes da MRede
            for (int j = 0; j < lisCorte->Count; j++)
               {
               if ((corte = (VTCorte*)lisCorte->Items[j]) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CORTEINV
                  return(false);
                  }

               ptrQry->SetVariable("p_EstudoId", estudo->Id);
               ptrQry->SetVariable("p_MRedeId" , mrede->Id);
               ptrQry->SetVariable("p_CorteId" , corte->Id);

               // Executa query
               ptrQry->Execute();

               corte->Id = ptrQry->GetVariable("p_CorteId");
               if (corte->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CORTEINV
                  sucesso = false;
                  break;
                  }
               }
            }
         // loop cortes (fim)

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
      if (ptrQry)   { delete ptrQry;   ptrQry   = NULL; }
      if (lisCorte) { delete lisCorte; lisCorte = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrEstudoCorteZero

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrRRCarga(VTEstudo* estudo, int intRedeId, int intCorteId, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTCarga*      carga;
   AnsiString    strlog;
   int           intCargaId;
   bool          bolNovoReg;
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

   if (!lisObjRede)
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

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrRRCarga(:p_NovoReg, :p_EstudoId, :p_MRedeId, :p_RRCargaId, :p_CorteId, :p_AlteradoObra, :p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                           :p_Icte_pu, :p_Scte_pu, :p_Zcte_pu, ");
         ptrQry->SQL->Add("                           :p_Barra1Id, :p_PosBar1, :p_Dx, :p_Dy ");
         ptrQry->SQL->Add("                          );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"        , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"       , otInteger);
         ptrQry->DeclareVariable("p_MRedeId"        , otInteger);
         ptrQry->DeclareVariable("p_RRCargaId"      , otInteger);
         ptrQry->DeclareVariable("p_CorteId"        , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra"   , otInteger);
         ptrQry->DeclareVariable("p_Show"           , otInteger);
         ptrQry->DeclareVariable("p_Codigo"         , otString );
         ptrQry->DeclareVariable("p_ExtId"          , otString );
         ptrQry->DeclareVariable("p_Icte_pu"        , otFloat  );
         ptrQry->DeclareVariable("p_Scte_pu"        , otFloat  );
         ptrQry->DeclareVariable("p_Zcte_pu"        , otFloat  );
         ptrQry->DeclareVariable("p_Barra1Id"       , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"        , otFloat  );
         ptrQry->DeclareVariable("p_Dx"             , otInteger);
         ptrQry->DeclareVariable("p_Dy"             , otInteger);

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop cargas (ini)
            carga = (VTCarga*)lisObjRede->Items[n];
            if (!carga)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_CARGAINV
                return(false);
                }
            if (!carga->curva)
               {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_CURVAINV
                return(false);
               }
            // Cargas de iluminação pública são escritas por um método específico
            if (carga->curva->Tipo == curvaLD)
               {
               continue;
               }

            // Verifica se o objeto é novo ou alterado
            if ((carga->Id < 0) || (bolObra))
               {
               intCargaId = IDINDEFINIDO;
               }
            else
               {
               intCargaId = carga->Id;
               }

            if (carga->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;
               }
            else if (carga->StatusAlterado() || carga->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            ptrQry->SetVariable("p_NovoReg"        , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"       , estudo->Id);
            ptrQry->SetVariable("p_MRedeId"        , intRedeId);
            ptrQry->SetVariable("p_RRCargaId"      , intCargaId);
            ptrQry->SetVariable("p_CorteId"        , intCorteId);
            ptrQry->SetVariable("p_AlteradoObra"   , (int)bolObra);
            ptrQry->SetVariable("p_Show"           , carga->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"         , carga->Codigo);
            ptrQry->SetVariable("p_ExtId"          , (carga->Extern_id.IsEmpty() ? carga->Codigo : carga->Extern_id));
            ptrQry->SetVariable("p_Icte_pu"        , carga->Icte_pu);
            ptrQry->SetVariable("p_Scte_pu"        , carga->Scte_pu);
            ptrQry->SetVariable("p_Zcte_pu"        , carga->Zcte_pu);
            ptrQry->SetVariable("p_Barra1Id"       , carga->pbarra->Id);
            ptrQry->SetVariable("p_PosBar1"        , carga->esq.posbar);
            ptrQry->SetVariable("p_Dx"             , carga->esq.dxbar);
            ptrQry->SetVariable("p_Dy"             , carga->esq.dybar);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               carga->Id = ptrQry->GetVariable("p_RRCargaId");
               if (carga->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }

            // Escreve a curva da carga
            if (carga->curva->NumeroPontos() > 0)
               {
               if (!SPSNP_wrConjCurvaEquipamento(estudo, carga, carga->curva))
                  {
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            // loop cargas (fim)
            }

         ptrQry->Close();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);

         strlog  = "EstudoId=" + IntToStr(estudo->Id) + ", MRedeId= " + IntToStr(intRedeId);
         strlog += ", RRCargaId=" + IntToStr(intCargaId) + ", CorteId= " + IntToStr(intCorteId);
         SNPSTRLOG(strlog);
         sucesso = false;
         }
      }
   __finally
      {
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrRRCarga

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrRRSuprimento(VTEstudo* estudo, int intRedeId, int intCorteId, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTSuprimento* suprimento;
   AnsiString    strlog;
   int           intSuprimentoId;
   bool          bolNovoReg;
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

   if (!lisObjRede)
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

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrRRSuprimento(:p_NovoReg, :p_EstudoId, :p_MRedeId, :p_RRSuprimentoId, :p_CorteId, :p_AlteradoObra, :p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                                :p_TipoSuprId, :p_Vnom, :p_Smax, :p_Pcc3F, :p_Qcc3F, :p_PccFT, :p_QccFT, ");
         ptrQry->SQL->Add("                                :p_Z0REqv, :p_Z0XEqv, :p_Z1REqv, :p_Z1XEqv, ");
         ptrQry->SQL->Add("                                :p_Barra1Id, :p_PosBar1, :p_Dx, :p_Dy");
         ptrQry->SQL->Add("                               );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_NovoReg"        , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"       , otInteger);
         ptrQry->DeclareVariable("p_MRedeId"        , otInteger);
         ptrQry->DeclareVariable("p_RRSuprimentoId" , otInteger);
         ptrQry->DeclareVariable("p_CorteId"        , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra"   , otInteger);
         ptrQry->DeclareVariable("p_Show"           , otInteger);
         ptrQry->DeclareVariable("p_Codigo"         , otString );
         ptrQry->DeclareVariable("p_ExtId"          , otString );
         ptrQry->DeclareVariable("p_TipoSuprId"     , otInteger);
         ptrQry->DeclareVariable("p_Vnom"           , otFloat  );
         ptrQry->DeclareVariable("p_Smax"           , otFloat  );
         ptrQry->DeclareVariable("p_Pcc3F"          , otFloat  );
         ptrQry->DeclareVariable("p_Qcc3F"          , otFloat  );
         ptrQry->DeclareVariable("p_PccFT"          , otFloat  );
         ptrQry->DeclareVariable("p_QccFT"          , otFloat  );
         ptrQry->DeclareVariable("p_Z0REqv"         , otFloat  );
         ptrQry->DeclareVariable("p_Z0XEqv"         , otFloat  );
         ptrQry->DeclareVariable("p_Z1REqv"         , otFloat  );
         ptrQry->DeclareVariable("p_Z1XEqv"         , otFloat  );
         ptrQry->DeclareVariable("p_Barra1Id"       , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"        , otFloat  );
         ptrQry->DeclareVariable("p_Dx"             , otInteger);
         ptrQry->DeclareVariable("p_Dy"             , otInteger);

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop Suprimentos (ini)
            suprimento = (VTSuprimento*)lisObjRede->Items[n];
            if (!suprimento)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_SUPRIMENTOINV
                return(false);
                }
            // Verifica se o objeto é novo ou alterado
            if ((suprimento->Id < 0) || (bolObra))
               {
               intSuprimentoId = IDINDEFINIDO;
               }
            else
               {
               intSuprimentoId = suprimento->Id;
               }

            if (suprimento->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;
               }
            else if (suprimento->StatusAlterado() || suprimento->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            // Seta valores dos parâmetros
            ptrQry->SetVariable("p_NovoReg"        , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"       , estudo->Id);
            ptrQry->SetVariable("p_MRedeId"        , intRedeId);
            ptrQry->SetVariable("p_RRSuprimentoId" , intSuprimentoId);
            ptrQry->SetVariable("p_CorteId"        , intCorteId);
            ptrQry->SetVariable("p_AlteradoObra"   , (int)bolObra);
            ptrQry->SetVariable("p_Show"           , suprimento->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"         , suprimento->Codigo);
            ptrQry->SetVariable("p_ExtId"          , (suprimento->Extern_id.IsEmpty() ? suprimento->Codigo : suprimento->Extern_id));
            ptrQry->SetVariable("p_TipoSuprId"     , suprimento->tiposup);
            ptrQry->SetVariable("p_Vnom"           , suprimento->vnom);
            ptrQry->SetVariable("p_Smax"           , suprimento->smax);
            ptrQry->SetVariable("p_Pcc3F"          , suprimento->pcc_3f.p);
            ptrQry->SetVariable("p_Qcc3F"          , suprimento->pcc_3f.q);
            ptrQry->SetVariable("p_PccFT"          , suprimento->pcc_ft.p);
            ptrQry->SetVariable("p_QccFT"          , suprimento->pcc_ft.q);
            ptrQry->SetVariable("p_Z0REqv"         , suprimento->zeq0.r);
            ptrQry->SetVariable("p_Z0XEqv"         , suprimento->zeq0.x);
            ptrQry->SetVariable("p_Z1REqv"         , suprimento->zeq1.r);
            ptrQry->SetVariable("p_Z1XEqv"         , suprimento->zeq1.x);
            ptrQry->SetVariable("p_Barra1Id"       , suprimento->pbarra->Id);
            ptrQry->SetVariable("p_PosBar1"        , suprimento->esq.posbar);
            ptrQry->SetVariable("p_Dx"             , suprimento->esq.dxbar);
            ptrQry->SetVariable("p_Dy"             , suprimento->esq.dybar);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               suprimento->Id = ptrQry->GetVariable("p_RRSuprimentoId");
               if (suprimento->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }

            // Escreve a curva do gerador
            if (suprimento->curva->NumeroPontos() > 0)
               {
               if (!SPSNP_wrConjCurvaEquipamento(estudo, suprimento, suprimento->curva))
                  {
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            // loop Suprimentos (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrRRSuprimento

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrRRTrafo(VTEstudo* estudo, int intRedeId, int intCorteId, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool          sucesso = true;
   VTTrafo*      trafo;
   AnsiString    strlog;
   int           intTrafoId;
   bool          bolNovoReg;
   TOracleQuery* ptrQry = NULL;
   TLOBLocator*  lobLocator = NULL;

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

   if (!lisObjRede)
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

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrRRTrafo(:p_NovoReg, :p_EstudoId, :p_MRedeId, :p_RRTrafoId, :p_CorteId, :p_AlteradoObra, :p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                           :pZ0R, :pZ0X, :pZ1R, :pZ1X, :p_MatY, ");
         ptrQry->SQL->Add("                           :p_Enrol1_TipoLigacaoId, :p_Enrol1_TipoFaseId, :p_Enrol1_Vnom, :p_Enrol1_Snom, :p_Enrol1_ZterraR, :p_Enrol1_ZterraX, ");
         ptrQry->SQL->Add("                           :p_Enrol2_TipoLigacaoId, :p_Enrol2_TipoFaseId, :p_Enrol2_Vnom, :p_Enrol2_Snom, :p_Enrol2_ZterraR, :p_Enrol2_ZterraX, ");
         ptrQry->SQL->Add("                           :p_RotFase, :p_ModeloEqv, :p_Barra1Id, :p_PosBar1, :p_Barra2Id, :p_PosBar2");
         ptrQry->SQL->Add("                          );");
         ptrQry->SQL->Add("END;");

         // Seta valores dos parâmetros
         ptrQry->DeclareVariable("p_NovoReg"               , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"              , otInteger);
         ptrQry->DeclareVariable("p_MRedeId"               , otInteger);
         ptrQry->DeclareVariable("p_RRTrafoId"             , otInteger);
         ptrQry->DeclareVariable("p_CorteId"               , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra"          , otInteger);
         ptrQry->DeclareVariable("p_Show"                  , otInteger);
         ptrQry->DeclareVariable("p_Codigo"                , otString );
         ptrQry->DeclareVariable("p_ExtId"                 , otString );
         ptrQry->DeclareVariable("pZ0R"                    , otFloat  );
         ptrQry->DeclareVariable("pZ0X"                    , otFloat  );
         ptrQry->DeclareVariable("pZ1R"                    , otFloat  );
         ptrQry->DeclareVariable("pZ1X"                    , otFloat  );
         ptrQry->DeclareVariable("p_MatY"                  , otBLOB   );
         ptrQry->DeclareVariable("p_Enrol1_TipoLigacaoId"  , otInteger);
         ptrQry->DeclareVariable("p_Enrol1_TipoFaseId"     , otInteger);
         ptrQry->DeclareVariable("p_Enrol1_Vnom"           , otFloat  );
         ptrQry->DeclareVariable("p_Enrol1_Snom"           , otFloat  );
         ptrQry->DeclareVariable("p_Enrol1_ZterraR"        , otFloat  );
         ptrQry->DeclareVariable("p_Enrol1_ZterraX"        , otFloat  );
         ptrQry->DeclareVariable("p_Enrol2_TipoLigacaoId"  , otInteger);
         ptrQry->DeclareVariable("p_Enrol2_TipoFaseId"     , otInteger);
         ptrQry->DeclareVariable("p_Enrol2_Vnom"           , otFloat  );
         ptrQry->DeclareVariable("p_Enrol2_Snom"           , otFloat  );
         ptrQry->DeclareVariable("p_Enrol2_ZterraR"        , otFloat  );
         ptrQry->DeclareVariable("p_Enrol2_ZterraX"        , otFloat  );
         ptrQry->DeclareVariable("p_RotFase"               , otFloat  );
         ptrQry->DeclareVariable("p_ModeloEqv"             , otInteger);
         ptrQry->DeclareVariable("p_Barra1Id"              , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"               , otFloat  );
         ptrQry->DeclareVariable("p_Barra2Id"              , otInteger);
         ptrQry->DeclareVariable("p_PosBar2"               , otFloat  );

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop trafos (ini)
            trafo = (VTTrafo*)lisObjRede->Items[n];
            if (!trafo)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_TRAFOINV
                return(false);
                }
            // Verifica se o objeto é novo ou alterado
            if ((trafo->Id < 0) || (bolObra))
               {
               intTrafoId = IDINDEFINIDO;
               }
            else
               {
               intTrafoId = trafo->Id;
               }

            if (trafo->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;
               }
            else if (trafo->StatusAlterado() || trafo->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }

            ptrQry->SetVariable("p_NovoReg"              , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"             , estudo->Id);
            ptrQry->SetVariable("p_MRedeId"              , intRedeId);
            ptrQry->SetVariable("p_RRTrafoId"            , intTrafoId);
            ptrQry->SetVariable("p_CorteId"              , intCorteId);
            ptrQry->SetVariable("p_AlteradoObra"         , (int)bolObra);
            ptrQry->SetVariable("p_Show"                 , trafo->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"               , trafo->Codigo);
            ptrQry->SetVariable("p_ExtId"                , (trafo->Extern_id.IsEmpty() ? trafo->Codigo : trafo->Extern_id));
            ptrQry->SetVariable("pZ0R"                   , trafo->z0.r);
            ptrQry->SetVariable("pZ0X"                   , trafo->z0.x);
            ptrQry->SetVariable("pZ1R"                   , trafo->z1.r);
            ptrQry->SetVariable("pZ1X"                   , trafo->z1.x);
            ptrQry->SetVariable("p_Enrol1_TipoLigacaoId" , trafo->pri.ligacao);
            ptrQry->SetVariable("p_Enrol1_TipoFaseId"    , faseABCN);
            ptrQry->SetVariable("p_Enrol1_Vnom"          , trafo->pri.vnom);
            ptrQry->SetVariable("p_Enrol1_Snom"          , trafo->snom);
            ptrQry->SetVariable("p_Enrol1_ZterraR"       , trafo->pri.zterra.r);
            ptrQry->SetVariable("p_Enrol1_ZterraX"       , trafo->pri.zterra.x);
            ptrQry->SetVariable("p_Enrol2_TipoLigacaoId" , trafo->sec.ligacao);
            ptrQry->SetVariable("p_Enrol2_TipoFaseId"    , faseABCN);
            ptrQry->SetVariable("p_Enrol2_Vnom"          , trafo->sec.vnom);
            ptrQry->SetVariable("p_Enrol2_Snom"          , trafo->snom);
            ptrQry->SetVariable("p_Enrol2_ZterraR"       , trafo->sec.zterra.r);
            ptrQry->SetVariable("p_Enrol2_ZterraX"       , trafo->sec.zterra.x);
            ptrQry->SetVariable("p_RotFase"              , trafo->sec.defasagem);
            //ptrQry->SetVariable("p_ModeloEqv"            , trafo->sec.zterra.x);
            ptrQry->SetVariable("p_ModeloEqv"            , 1);
            ptrQry->SetVariable("p_Barra1Id"             , trafo->BarraPrimario()->Id);
            ptrQry->SetVariable("p_PosBar1"              , trafo->esq.posbar1);
            ptrQry->SetVariable("p_Barra2Id"             , trafo->BarraSecundario()->Id);
            ptrQry->SetVariable("p_PosBar2"              , trafo->esq.posbar2);

            lobLocator = new TLOBLocator(bd_session, otBLOB, true);
            ptrQry->SetComplexVariable("p_MatY", lobLocator);
            if (bd_func->MontaStreamTrafoEqv(trafo, lobLocator))
               {
               // A stream tem que existir durante a execução do comando
               // delete ptrStream;
               // ptrStream = NULL;
               }
            else
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_TRAFOMATY
               return(false);
               }

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               trafo->Id = ptrQry->GetVariable("p_RRTrafoId");
               if (trafo->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            // loop trafos (fim)
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
      //destrói objetos locais
      if (lobLocator) { delete lobLocator; lobLocator = NULL; }
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrRRTrafo

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrRRYRef(VTEstudo* estudo, int intRedeId, int intCorteId, TList* lisObjRede, bool bolObra)
   {
   //variáveis locais
   bool            sucesso = true;
   complex<double> y_pu;
   VTYref*         yref;
   AnsiString      strlog;
   int             intYRefId;
   bool            bolNovoReg;
   TOracleQuery*   ptrQry = NULL;
   TLOBLocator*    lobLocator = NULL;
   complex<double> matComplex[4][4];

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

   if (!lisObjRede)
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

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrRRYRef(:p_NovoReg, :p_EstudoId, :p_MRedeId, :p_RRYRefId, :p_CorteId, :p_AlteradoObra, :p_Show, :p_Codigo, :p_ExtId, ");
         ptrQry->SQL->Add("                          :p_Y0G, :p_Y0B, :p_Y1G, :p_Y1B, :p_MatY, ");
         ptrQry->SQL->Add("                          :p_Barra1Id, :p_PosBar1, :p_Dx, :p_Dy");
         ptrQry->SQL->Add("                         );");
         ptrQry->SQL->Add("END;");

         // Seta valores dos parâmetros
         ptrQry->DeclareVariable("p_NovoReg"       , otInteger);
         ptrQry->DeclareVariable("p_EstudoId"      , otInteger);
         ptrQry->DeclareVariable("p_MRedeId"       , otInteger);
         ptrQry->DeclareVariable("p_RRYRefId"      , otInteger);
         ptrQry->DeclareVariable("p_CorteId"       , otInteger);
         ptrQry->DeclareVariable("p_AlteradoObra"  , otInteger);
         ptrQry->DeclareVariable("p_Show"          , otInteger);
         ptrQry->DeclareVariable("p_Codigo"        , otString );
         ptrQry->DeclareVariable("p_ExtId"         , otString );
         ptrQry->DeclareVariable("p_Y0G"           , otFloat  );
         ptrQry->DeclareVariable("p_Y0B"           , otFloat  );
         ptrQry->DeclareVariable("p_Y1G"           , otFloat  );
         ptrQry->DeclareVariable("p_Y1B"           , otFloat  );
         ptrQry->DeclareVariable("p_MatY"          , otBLOB   );
         ptrQry->DeclareVariable("p_Barra1Id"      , otInteger);
         ptrQry->DeclareVariable("p_PosBar1"       , otFloat  );
         ptrQry->DeclareVariable("p_Dx"            , otInteger);
         ptrQry->DeclareVariable("p_Dy"            , otInteger);

         for (int n = 0; n < lisObjRede->Count; n++)
            {
            // loop trafos (ini)
            yref = (VTYref*)lisObjRede->Items[n];
            if (!yref)
                {
                SNPSTRLOGLINE(strlog);
                MSGERRSYS_YREFINV
                return(false);
                }
            // Verifica se o objeto é novo ou alterado
            if ((yref->Id < 0) || (bolObra))
               {
               intYRefId = IDINDEFINIDO;
               }
            else
               {
               intYRefId = yref->Id;
               }

            if (yref->StatusNovo())
               {
               // Objeto novo. Tem que atualizar em memória o valor do identificador
               // retornado pela base de dados.
               bolNovoReg = true;
               }
            else if (yref->StatusAlterado() || yref->StatusDeslocado())
               {
               // Objeto previamente existente. O identificador em memória é válido.
               bolNovoReg = false;
               }
               else
                  {
                  // Objeto possui outro status que não deve ser atualizado na base de dados, nesse momento.
                  continue;
                  }
            ptrQry->SetVariable("p_NovoReg"      , (bolNovoReg ? 1 : 0));
            ptrQry->SetVariable("p_EstudoId"     , estudo->Id);
            ptrQry->SetVariable("p_MRedeId"      , intRedeId);
            ptrQry->SetVariable("p_RRYRefId"     , intYRefId);
            ptrQry->SetVariable("p_CorteId"      , intCorteId);
            ptrQry->SetVariable("p_AlteradoObra" , (int)bolObra);
            ptrQry->SetVariable("p_Show"         , yref->ShowAsUnsigned);
            ptrQry->SetVariable("p_Codigo"       , yref->Codigo);
            ptrQry->SetVariable("p_ExtId"        , (yref->Extern_id.IsEmpty() ? yref->Codigo : yref->Extern_id));

            y_pu = yref->Y0_pu;
            ptrQry->SetVariable("p_Y0G"          , y_pu.real());
            ptrQry->SetVariable("p_Y0B"          , y_pu.imag());

            y_pu = yref->Y1_pu;
            ptrQry->SetVariable("p_Y1G"          , y_pu.real());
            ptrQry->SetVariable("p_Y1B"          , y_pu.imag());

            ptrQry->SetVariable("p_Barra1Id"     , yref->pbarra->Id);
            ptrQry->SetVariable("p_PosBar1"      , yref->esq.posbar);
            ptrQry->SetVariable("p_Dx"           , yref->esq.dxbar);
            ptrQry->SetVariable("p_Dy"           , yref->esq.dybar);

            lobLocator = new TLOBLocator(bd_session, otBLOB, true);
            ptrQry->SetComplexVariable("p_MatY", lobLocator);

            for (int i = 0; i < 4; i++)
               {
               for (int j = 0; j < 4; j++)
                  {
                  matComplex[i][j] = yref->Y_pu[i][j];
                  }
               }
            if (bd_func->MontaStreamMatComplex(matComplex, lobLocator))
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

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            if (bolNovoReg)
               {
               yref->Id = ptrQry->GetVariable("p_RRYRefId");
               if (yref->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               }
            // loop trafos (fim)
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
      //destrói objetos locais
      if (lobLocator) { delete lobLocator; lobLocator = NULL; }
      if (ptrQry)     { delete ptrQry;     ptrQry = NULL;     }
      }

   return(sucesso);
   } // SPSNP_wrRRYRef
/*
//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SalvaReducoes(VTEstudo *estudo)
   {
   bool       sucesso = true;
   AnsiString strlog;
   TList*     lisMRedeCarregada = NULL;
   TList*     lisREDU = NULL;
   VTMRede*   mrede = NULL;
   VTReducao* reducao;
   TList*     lisEQP = NULL;


   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
      return(false);
      }

   __try
      {
      lisMRedeCarregada = new TList();
      redes->LisMRedeCarregada(lisMRedeCarregada);

      if (!SPSNP_dlEstudoCorte(estudo, lisMRedeCarregada))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDDL_CORTES
         return(false);
         }

      if (!SPSNP_wrEstudoCorte(estudo, lisMRedeCarregada))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDWR_CORTES
         return(false);
         }

      lisEQP = new TList();

      for (int i = 0; i < lisMRedeCarregada->Count; i++)
         {
         if ((mrede = (VTMRede*)lisMRedeCarregada->Items[i]) == NULL)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_MREDEINV
            sucesso = false;
            break;
            }
         //verifica se foram criadas as Reducoes da MRede
         if (! mrede->Status[sttENABLED]) continue;

         if ((lisREDU = mrede->LisReducao()) == NULL)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_LISREDUINV
            sucesso = false;
            break;
            }

         for (int i = 0; i < lisREDU->Count; i++)
            {
            if ((reducao = (VTReducao*)lisREDU->Items[i]) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_REDUCAOINV
               return(false);
               }

            if (!reducao->MRede)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_MREDEINV
               return(false);
               }

            lisEQP->Clear();
            reducao->LisEqpto(lisEQP, eqptoCARGA);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrRRCarga(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_RRCARGA
                  return(false);
                  }
               }

            lisEQP->Clear();
            reducao->LisEqpto(lisEQP, eqptoSUPRIMENTO);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrRRSuprimento(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_RRSUPRIMENTO
                  return(false);
                  }
               }

            lisEQP->Clear();
            reducao->LisEqpto(lisEQP, eqptoYREF);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrRRYRef(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_RRYREF
                  return(false);
                  }
               }

            lisEQP->Clear();
            reducao->LisEqpto(lisEQP, eqptoTRAFO);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrRRTrafo(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_RRTRAFO
                  return(false);
                  }
               }

            }
         }
      }
   __finally
      {
      //destrói lista sem destruir seus objetos
      if (lisEQP)
         {
         delete lisEQP;
         lisEQP = NULL;
         }

      if (lisMRedeCarregada)
         {
         delete lisMRedeCarregada;
         lisMRedeCarregada = NULL;
         }
      }

   return(sucesso);
   } // SalvaReducoes(VTEstudo *estudo)
*/
/*
//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SalvaReducoes(VTEstudo *estudo)
   {
   bool       sucesso = true;
   AnsiString strlog;
   TList*     lisMREDE = NULL;
   TList*     lisREDU = NULL;
   VTMRede*   mrede = NULL;
   VTReducao* reducao;
   TList*     lisEQP = NULL;


   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
      return(false);
      }

   __try
      {
      lisMREDE = new TList();
      //verifica se é o Estudo
      if (estudo->Id == 0)
         {//obtém lista de MRedes carregadas
         redes->LisMRedeCarregada(lisMREDE);
         }
      else
         {
//OOPS
         //obtém lista de MRedes carregadas
         redes->LisMRedeCarregada(lisMREDE);
         if (!SPSNP_wrEstudoCorte(estudo, lisMREDE))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBDWR_CORTES
            return(false);
            }
//OOPS fim


         lisMREDE->Clear();
         //obtém lista de MRedes não carregadas
         redes->LisMRedeReduzida(lisMREDE);
         }

      if (!SPSNP_dlEstudoCorte(estudo, lisMREDE))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDDL_CORTES
         return(false);
         }

      if (!SPSNP_wrEstudoCorte(estudo, lisMREDE))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDWR_CORTES
         return(false);
         }

      lisEQP = new TList();

      for (int i = 0; i < lisMREDE->Count; i++)
         {
         if ((mrede = (VTMRede*)lisMREDE->Items[i]) == NULL)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_MREDEINV
            sucesso = false;
            break;
            }
         //verifica se foram criadas as Reducoes da MRede carregada
         if ((mrede->Carregada)&&(! mrede->Status[sttENABLED])) continue;
         //obtém lista de Reduções da MREde
         if ((lisREDU = mrede->LisReducao()) == NULL)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_LISREDUINV
            sucesso = false;
            break;
            }

         for (int i = 0; i < lisREDU->Count; i++)
            {
            if ((reducao = (VTReducao*)lisREDU->Items[i]) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_REDUCAOINV
               return(false);
               }

            if (!reducao->MRede)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_MREDEINV
               return(false);
               }

            lisEQP->Clear();
            reducao->LisEqpto(lisEQP, eqptoCARGA);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrRRCarga(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_RRCARGA
                  return(false);
                  }
               }

            lisEQP->Clear();
            reducao->LisEqpto(lisEQP, eqptoSUPRIMENTO);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrRRSuprimento(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_RRSUPRIMENTO
                  return(false);
                  }
               }

            lisEQP->Clear();
            reducao->LisEqpto(lisEQP, eqptoYREF);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrRRYRef(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_RRYREF
                  return(false);
                  }
               }

            lisEQP->Clear();
            reducao->LisEqpto(lisEQP, eqptoTRAFO);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrRRTrafo(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_RRTRAFO
                  return(false);
                  }
               }

            }
         }
      }
   __finally
      {
      //destrói lista sem destruir seus objetos
      if (lisEQP)
         {
         delete lisEQP;
         lisEQP = NULL;
         }

      if (lisMREDE)
         {
         delete lisMREDE;
         lisMREDE = NULL;
         }
      }

   return(sucesso);
   } // SalvaReducoes(VTEstudo *estudo)
*/

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SalvaReducoes(VTEstudo *estudo)
   {
   if (estudo->Id == 0) return(SalvaReducoesEstudoZero(estudo));
   else                 return(SalvaReducoesEstudoNaoZero(estudo));
   }
   
//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SalvaReducoesEstudoZero(VTEstudo *estudo)
   {
   bool       sucesso = true;
   AnsiString strlog;
   TList*     lisMREDE = NULL;
   TList*     lisREDU = NULL;
   VTMRede*   mrede = NULL;
   VTReducao* reducao;
   TList*     lisEQP = NULL;


   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
      return(false);
      }

   __try
      {
      lisMREDE = new TList();
      //obtém lista de MRedes
      redes->LisMRede(lisMREDE);
      //elimina Cortes das MRedes carregadas cujas Reducoes foram recalculadas
      if (!SPSNP_dlEstudoCorte(estudo, lisMREDE))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDDL_CORTES
         return(false);
         }

      if (!SPSNP_wrEstudoCorteZero(estudo, lisMREDE))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDWR_CORTES
         return(false);
         }

      lisEQP = new TList();


      for (int i = 0; i < lisMREDE->Count; i++)
         {
         if ((mrede = (VTMRede*)lisMREDE->Items[i]) == NULL)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_MREDEINV
            sucesso = false;
            break;
            }
         //obtém lista de Reduções da MREde
         if ((lisREDU = mrede->LisReducao()) == NULL)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_LISREDUINV
            sucesso = false;
            break;
            }

         for (int i = 0; i < lisREDU->Count; i++)
            {
            if ((reducao = (VTReducao*)lisREDU->Items[i]) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_REDUCAOINV
               return(false);
               }

            if (!reducao->MRede)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_MREDEINV
               return(false);
               }

            lisEQP->Clear();
            reducao->LisEqpto(lisEQP, eqptoCARGA);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrRRCarga(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_RRCARGA
                  return(false);
                  }
               }

            lisEQP->Clear();
            reducao->LisEqpto(lisEQP, eqptoSUPRIMENTO);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrRRSuprimento(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_RRSUPRIMENTO
                  return(false);
                  }
               }

            lisEQP->Clear();
            reducao->LisEqpto(lisEQP, eqptoYREF);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrRRYRef(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_RRYREF
                  return(false);
                  }
               }

            lisEQP->Clear();
            reducao->LisEqpto(lisEQP, eqptoTRAFO);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrRRTrafo(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_RRTRAFO
                  return(false);
                  }
               }

            }
         }
      }
   __finally
      {
      //destrói lista sem destruir seus objetos
      if (lisEQP)
         {
         delete lisEQP;
         lisEQP = NULL;
         }

      if (lisMREDE)
         {
         delete lisMREDE;
         lisMREDE = NULL;
         }
      }

   return(sucesso);
   } // SalvaReducoes(VTEstudo *estudo)


//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SalvaReducoesEstudoNaoZero(VTEstudo *estudo)
   {
   bool       sucesso = true;
   AnsiString strlog;
   TList*     lisMREDE = NULL;
   TList*     lisREDU = NULL;
   VTMRede*   mrede = NULL;
   VTReducao* reducao;
   TList*     lisEQP = NULL;

   if (!estudo)                        { SNPSTRLOGLINE(strlog); MSGERRSYS_ESTUDOINV  return(false); }
   if (!redes)                         { SNPSTRLOGLINE(strlog); MSGERRSYS_REDESINV   return(false); }

   __try
      {
      lisEQP   = new TList();
      lisMREDE = new TList();
      //grava Cortes das MRedes
      redes->LisMRede(lisMREDE);
      if (!SPSNP_wrEstudoCorteNaoZero(estudo, lisMREDE))
         {
         SNPSTRLOGLINE(strlog);
         MSGERRBDWR_CORTES
         return(false);
         }
      //obtém lista de MRedes reduzidas
      redes->LisMRedeReduzida(lisMREDE);

      // Em seguida, salva o restante das reduções
      for (int i = 0; i < lisMREDE->Count; i++)
         {
         if ((mrede = (VTMRede*)lisMREDE->Items[i]) == NULL)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_MREDEINV
            sucesso = false;
            break;
            }

         //obtém lista de Reduções da MREde (que é única)
         if ((lisREDU = mrede->LisReducao()) == NULL)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_LISREDUINV
            sucesso = false;
            break;
            }

         for (int i = 0; i < lisREDU->Count; i++)
            {
            if ((reducao = (VTReducao*)lisREDU->Items[i]) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_REDUCAOINV
               return(false);
               }

            if (!reducao->MRede)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_MREDEINV
               return(false);
               }

            lisEQP->Clear();
            reducao->LisEqpto(lisEQP, eqptoCARGA);
            if(!SPSNP_wrRRCarga(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_RRCARGA
               return(false);
               }

            lisEQP->Clear();
            reducao->LisEqpto(lisEQP, eqptoSUPRIMENTO);
            if(!SPSNP_wrRRSuprimento(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_RRSUPRIMENTO
               return(false);
               }

            lisEQP->Clear();
            reducao->LisEqpto(lisEQP, eqptoYREF);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrRRYRef(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_RRYREF
                  return(false);
                  }
               }

            lisEQP->Clear();
            reducao->LisEqpto(lisEQP, eqptoTRAFO);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrRRTrafo(estudo, reducao->MRede->Id, reducao->Corte->Id, lisEQP))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_RRTRAFO
                  return(false);
                  }
               }
            }
         }
      }
   __finally
      {
      //destrói lista sem destruir seus objetos
      if (lisEQP)        { delete lisEQP;      lisEQP = NULL;      }
      if (lisMREDE)      { delete lisMREDE;    lisMREDE = NULL;    }
      }

   return(sucesso);
   } // SalvaReducoes(VTEstudo *estudo)

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_rdCorteMRede(VTEstudo* estudo, TList *lisMRede, TList *lisCORTE)
   {
   //variáveis locais
   bool          sucesso = true;
   int           intCorteId;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;
   VTMRede*      mrede;
   VTCorte*      corte;

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

   if (!lisMRede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISMREDEINV
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

         ptrQry->ReadBuffer    = ORACLE_DRIVER_BUFFER_SIZE;
         ptrCursor->ReadBuffer = ORACLE_DRIVER_BUFFER_SIZE;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdCorteMRede(:p_EstudoId, :p_MRedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId", otInteger);
         ptrQry->SetVariable("p_EstudoId", estudo->Id);
         ptrQry->DeclareVariable("p_MRedeId", otInteger);

         for (int i = 0; i < lisMRede->Count; i++)
            {
            if ((mrede = (VTMRede*)lisMRede->Items[i]) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_MREDEINV
               return(false);
               }

            ptrQry->SetVariable("p_MRedeId", mrede->Id);

            // Executa query e depois abre o cursor com os resultados
            ptrQry->Execute();
            ptrCursor->Execute();
            while (!ptrCursor->Eof)
               {
               // loop registros (begin)
               intCorteId = ptrCursor->Field("CorteId");

               //insere em Monta
               if ((corte = (VTCorte*)monta->InsereCorte(intCorteId, mrede->Id)) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CORTEINV
                  return(false);
                  }
               else
                  {//salva Corte em lisCORTE, se a lista foi passada
                  if (lisCORTE) lisCORTE->Add(corte);
                  corte->DefineStatus(sttNOVO, false);
                  }

               ptrCursor->Next();
               // loop registros (end)
               }
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
   } // SPSNP_rdCorteMRede

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_rdMRedeCorte(VTEstudo* estudo, TList *lisCORTE)
   {
   //variáveis locais
   bool          sucesso = true;
   int           intMRedeId;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;
   VTCorte*      corte;

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

   if (!lisCORTE)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISCORTEINV
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

         ptrQry->ReadBuffer    = ORACLE_DRIVER_BUFFER_SIZE;
         ptrCursor->ReadBuffer = ORACLE_DRIVER_BUFFER_SIZE;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdMRedeCorte(:p_EstudoId, :p_CorteId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId", otInteger);
         ptrQry->SetVariable("p_EstudoId", estudo->Id);
         ptrQry->DeclareVariable("p_CorteId", otInteger);

         for (int i = 0; i < lisCORTE->Count; i++)
            {
            if ((corte = (VTCorte*)lisCORTE->Items[i]) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_CORTEINV
               return(false);
               }

            ptrQry->SetVariable("p_CorteId", corte->Id);

            // Executa query e depois abre o cursor com os resultados
            ptrQry->Execute();
            ptrCursor->Execute();
            while (!ptrCursor->Eof)
               {
               // loop registros (begin)
               intMRedeId = ptrCursor->Field("MRedeId");

               //insere em Monta
               if ((corte = (VTCorte*)monta->InsereCorte(corte->Id, intMRedeId)) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_CORTEINV
                  return(false);
                  }
               else
                  {
                  corte->DefineStatus(sttNOVO, false);
                  }

               ptrCursor->Next();
               // loop registros (end)
               }
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
   } // SPSNP_rdMRedeCorte

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_rdRedeMRede(VTEstudo* estudo, int intMRedeId, TList *lisRedeAux)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   TOracleQuery* ptrCursor = NULL;

   // Atenção: a lista de redes auxiliares pode ser nula
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

         ptrQry->ReadBuffer    = ORACLE_DRIVER_BUFFER_SIZE;
         ptrCursor->ReadBuffer = ORACLE_DRIVER_BUFFER_SIZE;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdRedeMRede(:p_EstudoId, :p_MRedeId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId", otInteger);
         ptrQry->SetVariable("p_EstudoId", estudo->Id);
         ptrQry->DeclareVariable("p_MRedeId", otInteger);
         ptrQry->SetVariable("p_MRedeId", intMRedeId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();

         if (!SPSNP_rdRedeCursor(estudo, ptrCursor, lisRedeAux))
            {
            SNPSTRLOGLINE(strlog);
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
   } // SPSNP_rdRedeMRede

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_rdRRCarga(VTEstudo* estudo, int intMRedeId, int intCorteId)
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

         ptrQry->ReadBuffer    = ORACLE_DRIVER_BUFFER_SIZE;
         ptrCursor->ReadBuffer = ORACLE_DRIVER_BUFFER_SIZE;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdRRCarga(:p_EstudoId, :p_MRedeId, :p_CorteId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_MRedeId", otInteger);
         ptrQry->DeclareVariable("p_CorteId", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_MRedeId", intMRedeId);
         ptrQry->SetVariable("p_CorteId", intCorteId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_carga.car_id         = ptrCursor->Field("RRCargaId");
            str_carga.codigo         = ptrCursor->Field("Codigo");
            str_carga.extern_id      = ptrCursor->Field("ExtId");
            str_carga.fases          = faseABCN;
            str_carga.energia_mwhmes = 0;

            /*
            !OOPS! E esses campos?
            EqAux.fltIcte_pu                AS Icte_pu,
            EqAux.fltScte_pu                AS Scte_pu,
            EqAux.fltZcte_pu                AS Zcte_pu,
            */
            str_carga.classe_cons  = consEQUIVALENTE;

            str_carga.bar_id       = ptrCursor->Field("BarraId");
            str_carga.esq.posbar   = ptrCursor->Field("PosBar");
            str_carga.esq.dxbar    = ptrCursor->Field("DX");
            str_carga.esq.dybar    = ptrCursor->Field("DY");
            str_carga.obra         = false;

            bolTipica              = bool(int(ptrCursor->Field("Tipica")));
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

               str_carga.classe_cons = (int)curhdr.uchClasseId;
               }
            // Ao desmontar o BLOB, o header da curva é zerado. Assim, os campos devem
            // ser preenchidos posteriormente
            str_carga.curva.tipica = bolTipica;
            str_carga.curva.status = (unsigned)ptrCursor->Field("Status");
            str_carga.curva.id     = ptrCursor->Field("ConjCurvaId");

            // Atualiza o valor de mrede pois pode ter sido passado como -1
            intMRedeId = ptrCursor->Field("MRedeId");

            //insere em Monta
            if ((carga = (VTCarga*)monta->InsereReducaoCarga(intMRedeId, str_carga)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_CARGAINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(carga, estudo->Novo);
               if (carga->curva) { carga->curva->Status[sttNOVO] = estudo->Novo; }
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
   } // SPSNP_rdRRCarga

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_rdRRSuprimento(VTEstudo* estudo, int intMRedeId, int intCorteId)
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
         //ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdSuprimento(:p_EstudoId, :p_MRedeId, :p_cursor);");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdRRSuprimento(:p_EstudoId, :p_MRedeId, :p_CorteId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_MRedeId", otInteger);
         ptrQry->DeclareVariable("p_CorteId", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_MRedeId", intMRedeId);
         ptrQry->SetVariable("p_CorteId", intCorteId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)
            str_suprimento.sup_id     = ptrCursor->Field("RRSuprimentoId");
            str_suprimento.codigo     = ptrCursor->Field("Codigo");
            str_suprimento.extern_id  = ptrCursor->Field("ExtId");
            str_suprimento.tiposup    = ptrCursor->Field("TipoSuprId");
            str_suprimento.smax       = ptrCursor->Field("Smax");
            str_suprimento.pcc_3f.p   = ptrCursor->Field("Pcc3F");
            str_suprimento.pcc_3f.q   = ptrCursor->Field("Qcc3F");
            str_suprimento.pcc_ft.p   = ptrCursor->Field("PccFT");
            str_suprimento.pcc_ft.q   = ptrCursor->Field("QccFT");
            str_suprimento.zeq0.r     = ptrCursor->Field("Z0REqv");
            str_suprimento.zeq0.x     = ptrCursor->Field("Z0XEqv");
            str_suprimento.zeq1.r     = ptrCursor->Field("Z1REqv");
            str_suprimento.zeq1.x     = ptrCursor->Field("Z1XEqv");
            str_suprimento.bar_id     = ptrCursor->Field("BarraId");
            str_suprimento.esq.posbar = ptrCursor->Field("PosBar");
            str_suprimento.esq.dxbar  = ptrCursor->Field("DX");
            str_suprimento.esq.dybar  = ptrCursor->Field("DY");
            str_suprimento.vnom_kv    = ptrCursor->Field("Vnom");
            str_suprimento.obra       = false;

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
            str_suprimento.curva.status = (unsigned)ptrCursor->Field("Status");
            str_suprimento.curva.id     = ptrCursor->Field("ConjCurvaId");

            // Atualiza o valor de mrede pois pode ter sido passado como -1
            intMRedeId = ptrCursor->Field("MRedeId");

            //insere em Monta
            if ((suprimento = (VTSuprimento*)monta->InsereReducaoSuprimento(intMRedeId, str_suprimento)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_SUPRIMENTOINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(suprimento, estudo->Novo);
               if (suprimento->curva) { suprimento->curva->Status[sttNOVO] = estudo->Novo; }
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
   } // SPSNP_rdRRSuprimento

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_rdRRTrafo(VTEstudo* estudo, int intMRedeId, int intCorteId)
   {
   //variáveis locais
   bool            sucesso = true;
   VTTrafo*        trafo = NULL;
   AnsiString      strlog;
   strTRAFO        str_trafo;
   TStream*        ptrStream;
   TOracleQuery*   ptrQry = NULL;
   TOracleQuery*   ptrCursor = NULL;
   int             ndxBarra = 0;
   int             intBarraId;
   double          dblaux;

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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdRRTrafo(:p_EstudoId, :p_MRedeId, :p_CorteId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_MRedeId", otInteger);
         ptrQry->DeclareVariable("p_CorteId", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_MRedeId", intMRedeId);
         ptrQry->SetVariable("p_CorteId", intCorteId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)

            // preenche estrutura strTRAFO
            // Atenção: os enrolamentos devem vir ordenados
            if (ndxBarra == 0)
               {
               str_trafo.trf_id             = ptrCursor->Field("RRTrafoId");
               str_trafo.rede_id            = ptrCursor->Field("MRedeId");
               str_trafo.codigo             = ptrCursor->Field("Codigo");
               str_trafo.extern_id          = ptrCursor->Field("ExtId");
               str_trafo.z0.r               = ptrCursor->Field("Z0R");
               str_trafo.z0.x               = ptrCursor->Field("Z0X");
               str_trafo.z1.r               = ptrCursor->Field("Z1R");
               str_trafo.z1.x               = ptrCursor->Field("Z1X");
               str_trafo.modelo_eqv         = ptrCursor->Field("ModeloEqv");
               str_trafo.obra               = false;

               str_trafo.bar1_id            = ptrCursor->Field("PriBarraId");
               str_trafo.pri.ligacao        = ptrCursor->Field("PriTipoLigacaoId");
               str_trafo.pri.defasagem      = 0;
               str_trafo.pri.fases          = ptrCursor->Field("PriTipoFaseId");
               str_trafo.pri.vnom           = ptrCursor->Field("PriVnom");
               str_trafo.pri.tap            = str_trafo.pri.vnom ;
               str_trafo.snom_mva           = ptrCursor->Field("PriSnom");
               str_trafo.pri.zterra.r       = ptrCursor->Field("PriZterraR");
               str_trafo.pri.zterra.x       = ptrCursor->Field("PriZterraX");

               str_trafo.bar2_id            = ptrCursor->Field("SecBarraId");
               str_trafo.sec.ligacao        = ptrCursor->Field("SecTipoLigacaoId");
               str_trafo.sec.defasagem      = ptrCursor->Field("RotFase");
               str_trafo.sec.fases          = ptrCursor->Field("SecTipoFaseId");
               str_trafo.sec.vnom           = ptrCursor->Field("SecVnom");
               str_trafo.sec.tap            = str_trafo.sec.vnom;
               str_trafo.snom_mva           = ptrCursor->Field("SecSnom");
               str_trafo.sec.zterra.r       = ptrCursor->Field("SecZterraR");
               str_trafo.sec.zterra.x       = ptrCursor->Field("SecZterraX");

               // Monta a metriz
               ptrStream = ptrCursor->LOBField("MatY");
               if (!ptrStream)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_MATYINV
                  sucesso = false;
                  break;
                  }
               // O LOBField limpa a stream
               // delete ptrStream;
               if (! bd_func->DesmontaStreamTrafoEqv(ptrStream, &str_trafo.eqv))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_MATYINV
                  sucesso = false;
                  break;
                  }

               // Descobre dados da barra
               intBarraId = ptrCursor->Field("BarraId");
               if (intBarraId == str_trafo.bar1_id)
                  {
                  str_trafo.esq.posbar1 = ptrCursor->Field("PosBar");
                  }
               else
                  {
                  str_trafo.esq.posbar2 = ptrCursor->Field("PosBar");
                  }
               }
            else
               {
               // Descobre dados da barra
               intBarraId = ptrCursor->Field("BarraId");
               if (intBarraId == str_trafo.bar1_id)
                  {
                  str_trafo.esq.posbar1 = ptrCursor->Field("PosBar");
                  }
               else
                  {
                  str_trafo.esq.posbar2 = ptrCursor->Field("PosBar");
                  }

               // Atualiza o valor de mrede pois pode ter sido passado como -1
               intMRedeId = ptrCursor->Field("MRedeId");
               
               if ((trafo = (VTTrafo*)monta->InsereReducaoTrafo(intMRedeId, str_trafo)) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_TRAFOINV
                  sucesso = false;
                  break;
                  }
               else
                  {
                  bd_func->DefineStatusEqpto(trafo, estudo->Novo);
                  }
               }
            ndxBarra++;
            if (ndxBarra > 1)
               {
               ndxBarra = 0;
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
   } // SPSNP_rdRRTrafo


//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_rdRRYRef(VTEstudo* estudo, int intMRedeId, int intCorteId)
   {
   //variáveis locais
   bool            sucesso = true;
   VTYref*         yref = NULL;
   AnsiString      strlog;
   strYREF         str_yref;
   TStream*        ptrStream;
   TOracleQuery*   ptrQry = NULL;
   TOracleQuery*   ptrCursor = NULL;
   complex<double> matComplex[4][4];

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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdRRYRef(:p_EstudoId, :p_MRedeId, :p_CorteId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_MRedeId", otInteger);
         ptrQry->DeclareVariable("p_CorteId", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_MRedeId", intMRedeId);
         ptrQry->SetVariable("p_CorteId", intCorteId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         while (!ptrCursor->Eof)
            {
            // loop registros (begin)

            // preenche estrutura strYREF
            str_yref.yref_id            = ptrCursor->Field("RRYRefId");
            str_yref.codigo             = ptrCursor->Field("Codigo");
            str_yref.extern_id          = ptrCursor->Field("ExtId");
            str_yref.barra_id           = ptrCursor->Field("BarraId");
            str_yref.esq.posbar         = ptrCursor->Field("PosBar");
            str_yref.esq.dxbar          = ptrCursor->Field("DX");
            str_yref.esq.dybar          = ptrCursor->Field("DY");
            str_yref.y0_pu.g            = ptrCursor->Field("Y0G");
            str_yref.y0_pu.b            = ptrCursor->Field("Y0B");
            str_yref.y1_pu.g            = ptrCursor->Field("Y1G");
            str_yref.y1_pu.b            = ptrCursor->Field("Y1B");
            str_yref.obra               = false;

            // Monta a matriz
            ptrStream = ptrCursor->LOBField("MatY");
            if (!ptrStream)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_MATYINV
               sucesso = false;
               break;
               }
            // O LOBField limpa a stream
            // delete ptrStream;
            if (bd_func->DesmontaStreamMatComplex(ptrStream, matComplex))
               {
               for (int i = 0; i < 4; i++)
                  {
                  for (int j = 0; j < 4; j++)
                     {
                     str_yref.maty_pu[i][j].g = matComplex[i][j].real();
                     str_yref.maty_pu[i][j].b = matComplex[i][j].imag();
                     }
                  }
               }
            else
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_CURVAINV
               sucesso = false;
               break;
               }

            // Atualiza o valor de mrede pois pode ter sido passado como -1
            intMRedeId = ptrCursor->Field("MRedeId");

            if ((yref = (VTYref*)monta->InsereReducaoYref(intMRedeId, str_yref)) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_YREFINV
               sucesso = false;
               break;
               }
            else
               {
               bd_func->DefineStatusEqpto(yref, estudo->Novo);
               }
            // Atenção: os enrolamentos devem vir ordenados

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
   } // SPSNP_rdRRYRef
//---------------------------------------------------------------------------

bool __fastcall TBDeqptoOracle::SPSNP_rdRRBarra(VTEstudo* estudo, int intMRedeId, int intCorteId)
   {
   //variáveis locais
   bool          sucesso = true;
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdRRBarra(:p_EstudoId, :p_MRedeId, :p_CorteId, :p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor", otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->DeclareVariable("p_MRedeId", otInteger);
         ptrQry->DeclareVariable("p_CorteId", otInteger);

         ptrQry->SetVariable("p_EstudoId"  , estudo->Id);
         ptrQry->SetVariable("p_MRedeId", intMRedeId);
         ptrQry->SetVariable("p_CorteId", intCorteId);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();

         if (!SPSNP_rdBarraCursor(estudo, ptrCursor))
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
   } // SPSNP_rdRRBarra

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrEstudo(VTEstudo* estudo)
   {
   //variáveis locais
   bool          sucesso = true;
   VTCluster*    cluster;
   AnsiString    strlog;
   int           intClusterId;
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

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrEstudo(:p_EstudoId, :p_Descricao, :p_Autor, :p_TipoEstudoId);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_EstudoId"    , otInteger);
         ptrQry->DeclareVariable("p_Descricao"   , otString );
         ptrQry->DeclareVariable("p_Autor"       , otString );
         ptrQry->DeclareVariable("p_TipoEstudoId", otInteger);

         // Seta valores dos parâmetros
         ptrQry->SetVariable("p_EstudoId"     , (estudo->Id < 0 ? IDINDEFINIDO : estudo->Id) );
         ptrQry->SetVariable("p_Descricao"    , estudo->Codigo );
         ptrQry->SetVariable("p_Autor"        , estudo->Autor );
         ptrQry->SetVariable("p_TipoEstudoId" , estudo->Tipo);

         // Executa query
         ptrQry->Execute();

         // Verifica o valor retornado a ser atualizado no objeto em memória
         estudo->Id = ptrQry->GetVariable("p_EstudoId");
         if (estudo->Id == IDINDEFINIDO)
            {
            // Condição de erro
            sucesso = false;
            SNPSTRLOGLINE(strlog);
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrEstudo

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrEstudoAlternativa(VTEstudo *estudo)
   {
   //variáveis locais
   bool           sucesso = true;
   AnsiString     strlog;
   TOracleQuery*  ptrQry = NULL;
   TList*         lisAlternativa;
   VTAlternativa* alternativa;

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

   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
      return(false);
      }

   lisAlternativa = estudo->LisAlternativa();
   if (!lisAlternativa)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }
   // Se não tem alternativas, já retorna
   if (lisAlternativa->Count == 0) return(true);

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrEstudoAlternativa(:p_EstudoId, :p_AlternativaId, :p_Descricao);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_EstudoId"     , otInteger);
         ptrQry->DeclareVariable("p_AlternativaId", otInteger);
         ptrQry->DeclareVariable("p_Descricao"    , otString );

         for (int i = 0; i < lisAlternativa->Count; i++)
            {
            // loop Alternativas (ini)
            alternativa = (VTAlternativa*)lisAlternativa->Items[i];
            if (!alternativa)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_ALTERNATIVA
               return(false);
               }
            if (alternativa == estudo->AlternativaPadrao) continue;

            // Seta valores dos parâmetros
            ptrQry->SetVariable("p_EstudoId"     , estudo->Id                                              );
            ptrQry->SetVariable("p_AlternativaId", (alternativa->Id <= 0 ? IDINDEFINIDO : alternativa->Id) );
            ptrQry->SetVariable("p_Descricao"    , alternativa->Codigo                                     );

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            alternativa->Id = ptrQry->GetVariable("p_AlternativaId");
            if (alternativa->Id == IDINDEFINIDO)
               {
               // Condição de erro
               sucesso = false;
               SNPSTRLOGLINE(strlog);
               }
            // loop Alternativas (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrEstudoAlternativa

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrAlternativaObra(VTEstudo *estudo)
   {
   //variáveis locais
   bool           sucesso    = true;
   double         custo_zero = 0.;
   AnsiString     strlog;
   TOracleQuery*  ptrQry = NULL;
   TList*         lisAlternativa;
   VTAlternativa* alternativa;
   VTObras*       obras;
   TList*         lisObras;
   VTObra*        obra;

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

   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
      return(false);
      }

   lisAlternativa = estudo->LisAlternativa();
   if (!lisAlternativa)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }
   // Se não tem alternativas, já retorna
   if (lisAlternativa->Count == 0) return(true);

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrAlternativaObra(:p_EstudoId, :p_AlternativaId, :p_ObraId, :p_Descricao, :p_DataProjeto, ");
         ptrQry->SQL->Add("                                   :p_DataExecucao, :p_Responsavel, :p_Custo, :p_ObrigEspecial, :p_DeltaCustoOM );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_EstudoId"        , otInteger);
         ptrQry->DeclareVariable("p_AlternativaId"   , otInteger);
         ptrQry->DeclareVariable("p_ObraId"          , otInteger);
         ptrQry->DeclareVariable("p_Descricao"       , otString );
         ptrQry->DeclareVariable("p_DataProjeto"     , otDate   );
         ptrQry->DeclareVariable("p_DataExecucao"    , otDate   );
         ptrQry->DeclareVariable("p_Responsavel"     , otString );
         ptrQry->DeclareVariable("p_ObrigEspecial"   , otFloat  );
         ptrQry->DeclareVariable("p_Custo"           , otFloat  );
         ptrQry->DeclareVariable("p_DeltaCustoOM"    , otFloat  );
         ptrQry->DeclareVariable("p_AlteraTaxaFalha" , otInteger);
         ptrQry->DeclareVariable("p_TaxaFalhaIns"    , otFloat  );
         ptrQry->DeclareVariable("p_TaxaFalhaSub"    , otFloat  );

         for (int i = 0; i < lisAlternativa->Count; i++)
            {
            // loop Alternativas (ini)
            if ((alternativa = (VTAlternativa*)lisAlternativa->Items[i]) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_ALTERNATIVA
               return(false);
               }

            if ((obras = alternativa->Obras) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_OBRAS
               return(false);
               }

            if ((lisObras = obras->LisObra()) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_LISOBJINV
               return(false);
               }

            for (int j = 0; j < lisObras->Count; j++)
               {
               // loop obras (ini)
               if ((obra = (VTObra*)lisObras->Items[j]) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_OBRAS
                  return(false);
                  }

               // Seta valores dos parâmetros
               ptrQry->SetVariable("p_EstudoId"        , estudo->Id                                );
               ptrQry->SetVariable("p_AlternativaId"   , alternativa->Id                           );
               ptrQry->SetVariable("p_ObraId"          , (obra->Id <= 0 ? IDINDEFINIDO : obra->Id) );
               ptrQry->SetVariable("p_Descricao"       , obra->Nome                                );
               //ptrQry->SetVariable("p_DataProjeto"   , obra->Data                                );
               //ptrQry->SetVariable("p_DataExecucao"  , obra->Data                                );
               ptrQry->SetVariable("p_DataProjeto"     , obra->DataInicio                          );
               ptrQry->SetVariable("p_DataExecucao"    , obra->DataAtivacao                        );
               ptrQry->SetVariable("p_Responsavel"     , obra->Responsavel                         );
               ptrQry->SetVariable("p_ObrigEspecial"   , obra->ObEspecial                          );
               ptrQry->SetVariable("p_Custo"           , obra->Custo                               );
               //ptrQry->SetVariable("p_DeltaCustoOM" , obra->CustoOM                              );
               ptrQry->SetVariable("p_DeltaCustoOM"    , custo_zero                                );
               ptrQry->SetVariable("p_AlteraTaxaFalha" , int(obra->AlteraTaxaFalha)                );
               ptrQry->SetVariable("p_TaxaFalhaIns"    , obra->TaxaFalhaInsercao                   );
               ptrQry->SetVariable("p_TaxaFalhaSub"    , obra->TaxaFalhaSubstituicao               );

               // Executa query
               ptrQry->Execute();

               // !OOPS! Se os identificadores forem válidos, isso não é necessário
               // Verifica o valor retornado a ser atualizado no objeto em memória
               obra->Id = ptrQry->GetVariable("p_ObraId");
               if (obra->Id == IDINDEFINIDO)
                  {
                  // Condição de erro
                  sucesso = false;
                  SNPSTRLOGLINE(strlog);
                  }
               // loop obras (fim)
               }

            // loop Alternativas (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrAlternativaObra

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrObraAcao(VTEstudo *estudo)
   {
   //variáveis locais
   bool           sucesso = true;
   AnsiString     strlog;
   TOracleQuery*  ptrQry          = NULL;
   TList*         lisAlternativa  = NULL;
   VTAlternativa* alternativa     = NULL;
   VTObras*       obras           = NULL;
   TList*         lisObras        = NULL;
   VTObra*        obra            = NULL;
   TList*         lisAcao         = NULL;
   VTAcao*        acao            = NULL;
   VTEqpto*       eqpto           = NULL;
   VTEqpto*       eqpto_estadopre = NULL;
   VTEqpto*       eqpto_estadopos = NULL;
   int            intEquipId;
   int            intEquipEstadoPreId;
   int            intEquipEstadoPosId;

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

   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
      return(false);
      }

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
      return(false);
      }

   if (redes->LisRede()->Count < 1)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
      return(false);
      }

   lisAlternativa = estudo->LisAlternativa();
   if (!lisAlternativa)
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

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrObraAcao(:p_EstudoId, :p_AlternativaId, :p_ObraId, :p_AcaoId, ");
         ptrQry->SQL->Add("                            :p_TipoAcaoId, :p_EquipOriginalId, :p_EquipEstadoPreId, :p_EquipEstadoPosId );");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_EstudoId"        , otInteger );
         ptrQry->DeclareVariable("p_AlternativaId"   , otInteger );
         ptrQry->DeclareVariable("p_ObraId"          , otInteger );
         ptrQry->DeclareVariable("p_AcaoId"          , otString  );
         ptrQry->DeclareVariable("p_TipoAcaoId"      , otInteger );
         ptrQry->DeclareVariable("p_EquipOriginalId" , otInteger );
         ptrQry->DeclareVariable("p_EquipEstadoPreId", otInteger );
         ptrQry->DeclareVariable("p_EquipEstadoPosId", otInteger );

         for (int i = 0; i < lisAlternativa->Count; i++)
            {
            // loop Alternativas (ini)
            if ((alternativa = (VTAlternativa*)lisAlternativa->Items[i]) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_ALTERNATIVA
               return(false);
               }

            if ((obras = alternativa->Obras) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_OBRAS
               return(false);
               }

            if ((lisObras = obras->LisObra()) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_LISOBJINV
               return(false);
               }

            for (int j = 0; j < lisObras->Count; j++)
               {
               // loop obras (ini)
               if ((obra = (VTObra*)lisObras->Items[j]) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_OBRAS
                  return(false);
                  }

               if ((lisAcao = obra->LisAcao) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_LISOBJINV
                  return(false);
                  }

               // As ações são armazenadas na ordem inversa
               for (int k = lisAcao->Count - 1; k >= 0 ; k--)
                  {
                  // loop acoes (ini)
                  if ((acao = (VTAcao*)lisAcao->Items[k]) == NULL)
                     {
                     SNPSTRLOGLINE(strlog);
                     MSGERRSYS_ACAO
                     return(false);
                     }
                  //define Id da Acao
                  acao->Id = IDINDEFINIDO;
                  //verifica o tipo de Acao
                  switch(acao->Tipo())
                     {
                     case acaoALTERA:
                        eqpto           = acao->Eqpto();
                        eqpto_estadopre = acao->CopiaDoEqptoOriginal();
                        eqpto_estadopos = acao->CopiaDoEqptoAlterado();
                        break;
                     case acaoINSERE:
                        eqpto           = acao->Eqpto();
                        eqpto_estadopre = NULL;
                        eqpto_estadopos = NULL;
                        break;
                     case acaoRETIRA:
                        eqpto           = acao->Eqpto();
                        eqpto_estadopre = NULL;
                        eqpto_estadopos = NULL;
                        break;
                     default:
                        continue;
                     }
                  // Compatibiliza os valores
                  eqpto           ? intEquipId          = eqpto->Id           : intEquipId          = IDINDEFINIDO;
                  eqpto_estadopre ? intEquipEstadoPreId = eqpto_estadopre->Id : intEquipEstadoPreId = IDINDEFINIDO;
                  eqpto_estadopos ? intEquipEstadoPosId = eqpto_estadopos->Id : intEquipEstadoPosId = IDINDEFINIDO;

                  // Seta valores dos parâmetros
                  ptrQry->SetVariable("p_EstudoId"        , estudo->Id                                   );
                  ptrQry->SetVariable("p_AlternativaId"   , alternativa->Id                              );
                  ptrQry->SetVariable("p_ObraId"          , obra->Id                                     );
                  ptrQry->SetVariable("p_AcaoId"          , (acao->Id <= 0 ? IDINDEFINIDO : acao->Id)    );
                  ptrQry->SetVariable("p_TipoAcaoId"      , acao->Tipo()                                 );
                  ptrQry->SetVariable("p_EquipOriginalId" , intEquipId                                   );
                  ptrQry->SetVariable("p_EquipEstadoPreId", intEquipEstadoPreId                          );
                  ptrQry->SetVariable("p_EquipEstadoPosId", intEquipEstadoPosId                          );

                  // Executa query
                  ptrQry->Execute();

                  // !OOPS! Se os identificadores forem válidos, isso não é necessário
                  // Verifica o valor retornado a ser atualizado no objeto em memória
                  obra->Id = ptrQry->GetVariable("p_ObraId");
                  if (obra->Id == IDINDEFINIDO)
                     {
                     // Condição de erro
                     sucesso = false;
                     SNPSTRLOGLINE(strlog);
                     }
                  //!OOPS!
                  //eqpto->Id = eqpto_alterado->Id;

                  // loop acoes (fim)
                  }

               // loop Alternativas (fim)
               }

               // loop obras (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } // SPSNP_wrObraAcao

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrObraAtribExt(VTEstudo *estudo)
   {
   //variáveis locais
   bool           sucesso = true;
   AnsiString     strlog;
   TOracleQuery*  ptrQry          = NULL;
   TList*         lisAlternativa  = NULL;
   VTAlternativa* alternativa     = NULL;
   VTObras*       obras           = NULL;
   TList*         lisObras        = NULL;
   VTObra*        obra            = NULL;
   TList*         lisATRIB        = NULL;
   VTIpeAtributo* ipe_atributo    = NULL;

   if (!bd_session)                  { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSION    return(false); }
   if (!bd_session->Connected)       { SNPSTRLOGLINE(strlog); MSGERRSYS_SESSIONCON return(false); }
   if (!estudo)                      { SNPSTRLOGLINE(strlog); MSGERRSYS_ESTUDOINV  return(false); }
   if (!redes)                       { SNPSTRLOGLINE(strlog); MSGERRSYS_REDESINV   return(false); }
   if (redes->LisRede()->Count < 1)  { SNPSTRLOGLINE(strlog); MSGERRSYS_REDESINV   return(false); }

   lisAlternativa = estudo->LisAlternativa();
   if (!lisAlternativa)              { SNPSTRLOGLINE(strlog); MSGERRSYS_LISOBJINV  return(false); }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         // OOPS: essa versão ainda não considera o Blob
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrObraAtribExt(:p_EstudoId, :p_AlternativaId, :p_ObraId, :p_ExtId, :p_AtribId);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_EstudoId"        , otInteger );
         ptrQry->DeclareVariable("p_AlternativaId"   , otInteger );
         ptrQry->DeclareVariable("p_ObraId"          , otInteger );
         ptrQry->DeclareVariable("p_ExtId"           , otInteger );
         ptrQry->DeclareVariable("p_AtribId"         , otInteger );

         for (int i = 0; i < lisAlternativa->Count; i++)
            {
            // loop Alternativas (ini)
            if ((alternativa = (VTAlternativa*)lisAlternativa->Items[i]) == NULL)  { SNPSTRLOGLINE(strlog); MSGERRSYS_ALTERNATIVA return(false); }
            if ((obras = alternativa->Obras) == NULL)                              { SNPSTRLOGLINE(strlog); MSGERRSYS_OBRAS       return(false); }
            if ((lisObras = obras->LisObra()) == NULL)                             { SNPSTRLOGLINE(strlog); MSGERRSYS_LISOBJINV   return(false); }

            for (int j = 0; j < lisObras->Count; j++)
               {
               // loop obras (ini)
               if ((obra = (VTObra*)lisObras->Items[j]) == NULL)                   { SNPSTRLOGLINE(strlog); MSGERRSYS_OBRAS       return(false); }
               if ((lisATRIB = obra->LisObject) == NULL)                           { SNPSTRLOGLINE(strlog); MSGERRSYS_LISOBJINV   return(false); }

               // As ações são armazenadas na ordem inversa
               for (int k = lisATRIB->Count - 1; k >= 0 ; k--)
                  {
                  // loop acoes (ini)
                  if ((ipe_atributo = (VTIpeAtributo*)lisATRIB->Items[k]) == NULL) { SNPSTRLOGLINE(strlog); MSGERRSYS_ACAO        return(false); }

                  // Seta valores dos parâmetros
                  ptrQry->SetVariable("p_EstudoId"     , estudo->Id         );
                  ptrQry->SetVariable("p_AlternativaId", alternativa->Id    );
                  ptrQry->SetVariable("p_ObraId"       , obra->Id           );
                  ptrQry->SetVariable("p_ExtId"        , ipe_atributo->Tipo );
                  ptrQry->SetVariable("p_AtribId"      , ipe_atributo->Id   );

                  // Executa query
                  ptrQry->Execute();
                  // loop acoes (fim)
                  }

               // loop Alternativas (fim)
               }

               // loop obras (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   } //

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrEstudoEqpto(VTEstudo* estudo)
   {
   //variáveis locais
   AnsiString   strlog;
   TList*       lisEQP = NULL;
   TList*       lisREDE = NULL;
   VTRede*      rede = NULL;

   __try
      {
      try
         {
         // Lista auxiliar para carga de equipamentos
         lisEQP = new TList();
         lisREDE = new TList();

         if (cronometro) cronometro->Reinicia();
         if (cronometro) cronometro->Start("SalvaRedeEstudo");

         if(!estudo)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_ESTUDOINV
            return(false);
            }

         // Salva dados das barras
         // Uma mesma barra pode estar em duas redes e será salva para todas as redes
         if (progresso) progresso->Add("Salvando barras");

         lisEQP->Clear();
         // OOPS. Precisa fazer a alteração no redes para trazer as barras de chaves abertas
         // rede->LisBarra(lisEQP, true); // Carrega também as barras das chaves abertas
         estudo->LisEqptoEstudo(lisEQP, NULL, eqptoBARRA);
         if(!SPSNP_wrBarra(estudo, lisEQP, true))
            {
            SNPSTRLOGLINE(strlog);
            MSGERRBDWR_WREQUIPREDE(strlog, "barras")
            return(false);
            }

         // Associa as barras às redes
         // Aproveita a lista de equipamentos (barras) carregada anteriormente
         // Atenção: lisREDE conterá somente as redes criadas no estudo (obra).
         estudo->LisEqptoEstudo(lisREDE, NULL, eqptoREDE);

         // Salva dados de Redes
         if (progresso) progresso->Add("Salvando redes");
         if (lisREDE->Count > 0)
            {
            if (!SPSNP_wrRede(estudo, lisREDE, (estudo->Id > 0)))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_REDES
               return(false);
               }
            }

         // Adiciona as redes da rede original
         redes->LisRede(lisREDE);

         // Salva dados das cargas
         if (progresso) progresso->Add("Salvando cargas");
         lisEQP->Clear();
         estudo->LisEqptoEstudo(lisEQP, NULL, eqptoCARGA);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrCarga(estudo, lisEQP, true))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "cargas")
               return(false);
               }
            }

         if (progresso) progresso->Add("Salvando cargas (iluminação pública)");
         lisEQP->Clear();
         estudo->LisEqptoEstudo(lisEQP, NULL, eqptoCARGA);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrCargaIP(estudo, lisEQP, true))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "cargas iluminação pública")
               return(false);
               }
            }

         // Salva dados dos capacitores
         if (progresso) progresso->Add("Salvando capacitores");
         lisEQP->Clear();
         estudo->LisEqptoEstudo(lisEQP, NULL, eqptoCAPACITOR);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrCapacitor(estudo, lisEQP, true))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "capacitores")
               return(false);
               }
            }

         // Salva dados dos geradores
         if (progresso) progresso->Add("Salvando geradores");
         lisEQP->Clear();
         estudo->LisEqptoEstudo(lisEQP, NULL, eqptoGERADOR);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrGerador(estudo, lisEQP, true))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "geradores")
               return(false);
               }
            }

         // Salva dados dos medidores
         if (progresso) progresso->Add("Salvando medidores");
         lisEQP->Clear();
         estudo->LisEqptoEstudo(lisEQP, NULL, eqptoMEDIDOR);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrMedidor(estudo, lisEQP, true))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "medidores")
               return(false);
               }
            }

         // Salva dados dos canais
         if (progresso) progresso->Add("Salvando canais");
         lisEQP->Clear();
         estudo->LisEqptoEstudo(lisEQP, NULL, eqptoCANAL);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrCanal(estudo, lisEQP, true))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "canais")
               return(false);
               }
            }

         // Salva dados dos suprimentos
         if (progresso) progresso->Add("Salvando suprimentos");
         lisEQP->Clear();
         estudo->LisEqptoEstudo(lisEQP, NULL, eqptoSUPRIMENTO);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrSuprimento(estudo, lisEQP, true))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "suprimentos")
               return(false);
               }
            }

         // Salva dados dos filtros
         if (progresso) progresso->Add("Salvando filtros");
         lisEQP->Clear();
         estudo->LisEqptoEstudo(lisEQP, NULL, eqptoFILTRO);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrFiltro(estudo, lisEQP, true))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "filtros")
               return(false);
               }
            }

         // Salva dados dos reatores
         if (progresso) progresso->Add("Salvando reatores");
         lisEQP->Clear();
         estudo->LisEqptoEstudo(lisEQP, NULL, eqptoREATOR);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrReator(estudo, lisEQP, true))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "reatores")
               return(false);
               }
            }

         // Salva dados dos trafos ZZ
         if (progresso) progresso->Add("Salvando transformadores Zig-Zag");
         lisEQP->Clear();
         estudo->LisEqptoEstudo(lisEQP, NULL, eqptoTRAFOZZ);
         if (lisEQP->Count > 0)
            {
            if(!SPSNP_wrTrafoZZ(estudo, lisEQP, true))
               {
               SNPSTRLOGLINE(strlog);
               MSGERRBDWR_WREQUIPREDE(strlog, "trafos Zig Zag")
               return(false);
               }
            }

         for (int i = 0; i < lisREDE->Count; i++)
            {
            if ((rede = (VTRede*)lisREDE->Items[i]) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_REDEINV
               return(false);
               }

            // Salva dados dos trechos
            if (progresso) progresso->Add("Salvando trechos");
            lisEQP->Clear();
            estudo->LisEqptoEstudo(lisEQP, rede, eqptoTRECHO);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrTrecho(estudo, rede->Id, lisEQP, true))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_WREQUIPREDE(strlog, "trechos")
                  return(false);
                  }
               }

            // Salva dados dos capacitores série
            if (progresso) progresso->Add("Salvando capacitores série");
            lisEQP->Clear();
            estudo->LisEqptoEstudo(lisEQP, rede, eqptoCAPSERIE);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrCapacitorSerie(estudo, rede->Id, lisEQP, true))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_WREQUIPREDE(strlog, "capacitores série")
                  return(false);
                  }
               }

            // Salva dados das mútuas
            if (progresso) progresso->Add("Salvando impedâncias mútuas");
            lisEQP->Clear();
            rede->LisMutua(lisEQP);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrMutua(estudo, rede->Id, lisEQP, true))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_WREQUIPREDE(strlog, "mútuas")
                  return(false);
                  }
               }

            // Salva dados das chaves
            if (progresso) progresso->Add("Salvando chaves");
            lisEQP->Clear();
            estudo->LisEqptoEstudo(lisEQP, rede, eqptoCHAVE);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrChave(estudo, rede->Id, lisEQP, true))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_WREQUIPREDE(strlog, "chaves")
                  return(false);
                  }
               }

            // Salva dados dos reguladores
            if (progresso) progresso->Add("Salvando reguladores");
            lisEQP->Clear();
            estudo->LisEqptoEstudo(lisEQP, rede, eqptoREGULADOR);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrRegulador(estudo, rede->Id, lisEQP, true))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_WREQUIPREDE(strlog, "reguladores")
                  return(false);
                  }
               }

            // Salva dados dos transformadores
            if (progresso) progresso->Add("Salvando transformadores");
            lisEQP->Clear();
            estudo->LisEqptoEstudo(lisEQP, rede, eqptoTRAFO);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrTrafo(estudo, rede->Id, lisEQP, true))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_WREQUIPREDE(strlog, "trafos")
                  return(false);
                  }
               }
            lisEQP->Clear();
            estudo->LisEqptoEstudo(lisEQP, rede, eqptoTRAFO3E);
            if (lisEQP->Count > 0)
               {
               if(!SPSNP_wrTrafo3E(estudo, rede->Id, lisEQP, true))
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_WREQUIPREDE(strlog, "trafos 3E")
                  return(false);
                  }
               }

            }

         if (cronometro) cronometro->Stop("SalvaRedeEstudo");
         if (cronometro) cronometro->Show();
         }
      catch (Exception& e)
         {
         SNPSTRLOGLINE(strlog);
         SNPSTRLOG(e.Message);
         return(false);
         }
      }
   __finally
      {
      if (progresso) progresso->Stop();

      //destrói lista sem destruir seus objetos
      if (lisEQP)  { delete lisEQP;  lisEQP = NULL;  }
      if (lisREDE) { delete lisREDE; lisREDE = NULL; }
      }

   return(true);
   } // SPSNP_wrEstudoEqpto

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::RedefineIDAlternativa(VTEstudo* estudo)
   {
   //variáveis locais
   AnsiString     strlog;
   TList*         lisAlternativa;
   VTAlternativa* alternativa;
   VTObras*       obras;
   TList*         lisObras;
   VTObra*        obra;
   TList*         lisAcao;
   VTAcao*        acao;

   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
      return(false);
      }

   lisAlternativa = estudo->LisAlternativa();
   if (!lisAlternativa)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_LISOBJINV
      return(false);
      }

   for (int i = 0; i < lisAlternativa->Count; i++)
      {
      // loop Alternativas (ini)
      if ((alternativa = (VTAlternativa*)lisAlternativa->Items[i]) == NULL)
         {
         SNPSTRLOGLINE(strlog);
         MSGERRSYS_ALTERNATIVA
         return(false);
         }
      alternativa->Id = 0;

      if ((obras = alternativa->Obras) == NULL)
         {
         SNPSTRLOGLINE(strlog);
         MSGERRSYS_OBRAS
         return(false);
         }

      if ((lisObras = obras->LisObra()) == NULL)
         {
         SNPSTRLOGLINE(strlog);
         MSGERRSYS_LISOBJINV
         return(false);
         }

      for (int j = 0; j < lisObras->Count; j++)
         {
         // loop obras (ini)
         if ((obra = (VTObra*)lisObras->Items[j]) == NULL)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_OBRAS
            return(false);
            }
         obra->Id = 0;

         if ((lisAcao = obra->LisAcao) == NULL)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_LISOBJINV
            return(false);
            }

         for (int k = 0; k < lisAcao->Count; k++)
            {
            // loop acoes (ini)
            if ((acao = (VTAcao*)lisAcao->Items[k]) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_ACAO
               return(false);
               }

            acao->Id = 0;
            // loop acoes (fim)
            }

         // loop Alternativas (fim)
         }

      // loop obras (fim)
      }

   return(true);
   } // RedefineIDAlternativa

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::TrataLixeira(VTEstudo* estudo)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   TList*        lisXeira = NULL;
   VTEqpto*      eqpto;
   AnsiString    strQueryName;
   int           intTipoEquip;
   VTEdita       *edita   = (VTEdita*)apl->GetObject(__classid(VTEdita));

   // A ordem com que os equipamentos são removidos é importante
   int           vecEquipOrder[] = { eqptoCAPACITOR,  eqptoSUPRIMENTO, eqptoTRAFOZZ,
                                     eqptoCARGA,      eqptoGERADOR,    eqptoFILTRO,
                                     eqptoMEDIDOR,    eqptoCANAL,      eqptoREATOR,
                                     eqptoCHAVE,      eqptoCAPSERIE,   eqptoREGULADOR,
                                     eqptoTRAFO,      eqptoTRAFO3E,    eqptoMUTUA,
                                     eqptoTRECHO,     eqptoREDE,       eqptoBARRA,
                                     eqptoARRANJO,    eqptoSUPORTE,    eqptoCABO,
                                     eqptoCLUSTER
                                   };

   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
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
   /*
   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
      return(false);
      }
   */
   if (!edita)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
      return(false);
      }

   __try
      {
      try
         {
         lisXeira = new TList();

         //CopiaTList(redes->Lixeira->LisEqpto(), lisXeira);
         CopiaTList(edita->Obra->Lixeira->LisEqpto(), lisXeira);

         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         for (int i = 0; i < (int)(sizeof(vecEquipOrder) / sizeof(int)); i++)
            {
            for (int j = lisXeira->Count - 1; j >= 0; j--)
               {
               if ((eqpto = (VTEqpto*)lisXeira->Items[j]) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRBDWR_LIXEIRA
                  return(false);
                  }

               intTipoEquip = eqpto->Tipo();
               if ( intTipoEquip != vecEquipOrder[i] ) continue;

               // Equipamento será removido
               lisXeira->Remove(eqpto);

               // !OOPS! Falta incluir na lista abaixo cabos, arranjos, suportes e clusters
               switch(intTipoEquip)
                  {
                  case eqptoCAPACITOR  : strQueryName = " PCK_SINAP.SPSNP_dlCapacitor";   break;
                  case eqptoSUPRIMENTO : strQueryName = " PCK_SINAP.SPSNP_dlSuprimento";  break;
                  case eqptoTRAFOZZ    : strQueryName = " PCK_SINAP.SPSNP_dlTrafoZZ";     break;
                  case eqptoCARGA      : strQueryName = " PCK_SINAP.SPSNP_dlCarga";       break;
                  case eqptoGERADOR    : strQueryName = " PCK_SINAP.SPSNP_dlGerador";     break;
                  case eqptoFILTRO     : strQueryName = " PCK_SINAP.SPSNP_dlFiltro";      break;
                  case eqptoMEDIDOR    : strQueryName = " PCK_SINAP.SPSNP_dlMedidor";     break;
                  case eqptoCANAL      : continue;   // Não faz nada
                  case eqptoREATOR     : strQueryName = " PCK_SINAP.SPSNP_dlReator";      break;
                  case eqptoCHAVE      : strQueryName = " PCK_SINAP.SPSNP_dlChave";       break;
                  case eqptoCAPSERIE   : strQueryName = " PCK_SINAP.SPSNP_dlCapserie";    break;
                  case eqptoREGULADOR  : strQueryName = " PCK_SINAP.SPSNP_dlRegulador";   break;
                  case eqptoTRAFO      : strQueryName = " PCK_SINAP.SPSNP_dlTrafo";       break;
                  case eqptoTRAFO3E    : strQueryName = " PCK_SINAP.SPSNP_dlTrafo3E";     break;
                  case eqptoMUTUA      : strQueryName = " PCK_SINAP.SPSNP_dlMutua";       break;
                  case eqptoTRECHO     : strQueryName = " PCK_SINAP.SPSNP_dlTrecho";      break;
                  case eqptoREDE       : strQueryName = " PCK_SINAP.SPSNP_dlRede";        break;
                  case eqptoBARRA      : strQueryName = " PCK_SINAP.SPSNP_dlBarra";       break;
                  case eqptoARRANJO    : continue;   // Não faz nada
                  case eqptoSUPORTE    : continue;   // Não faz nada
                  case eqptoCABO       : continue;   // Não faz nada
                  case eqptoCLUSTER    : strQueryName = " PCK_SINAP.SPSNP_dlAgrupamento"; break;
                  default              : return(false);
                  }

              // Cria estrutura do comando
               ptrQry->Clear();
               ptrQry->SQL->Add("BEGIN");
               ptrQry->SQL->Add(strQueryName + "(:p_EstudoId, :p_EquipId);");
               ptrQry->SQL->Add("END;");
               ptrQry->DeclareVariable("p_EstudoId", otInteger);
               ptrQry->SetVariable("p_EstudoId", estudo->Id);
               ptrQry->DeclareVariable("p_EquipId", otInteger);
               ptrQry->SetVariable("p_EquipId", eqpto->Id);

               // Executa query
               ptrQry->Execute();

               ptrQry->Close();
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
      if (lisXeira) { delete lisXeira; lisXeira = NULL; }
      if (ptrQry)   { delete ptrQry;   ptrQry = NULL;   }
      }

   return(sucesso);
   } // TrataLixeira

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_dlDominio(VTEstudo *estudo, VTDominio* dominio)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

   if (!dominio)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_DOMINIOINV
      return(false);
      }

   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_dlDominio(:p_EstudoId, :p_DominioId); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->SetVariable("p_EstudoId", estudo->Id);
         ptrQry->DeclareVariable("p_DominioId"  , otInteger);
         ptrQry->SetVariable("p_DominioId", dominio->Id);

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
   } //

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_dlRedeDominio(VTEstudo *estudo, VTRede* rede)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
      return(false);
      }

   if (!rede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDEINV
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_dlRedeDominio(:p_EstudoId, :p_RedeId); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->SetVariable("p_EstudoId", estudo->Id);
         ptrQry->DeclareVariable("p_RedeId"  , otInteger);
         ptrQry->SetVariable("p_RedeId", rede->Id);

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
   } // SPSNP_dlRedeDominio

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrRedeDominio(VTEstudo* estudo, VTRede* rede, VTDominio* dominio)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
      return(false);
      }

   if (!rede)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDEINV
      return(false);
      }

   if (!dominio)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_DOMINIOINV
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrRedeDominio(:p_EstudoId, :p_RedeId, :p_DominioId); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->SetVariable("p_EstudoId", estudo->Id);
         ptrQry->DeclareVariable("p_RedeId"  , otInteger);
         ptrQry->SetVariable("p_RedeId", rede->Id);
         ptrQry->DeclareVariable("p_DominioId"  , otInteger);
         ptrQry->SetVariable("p_DominioId", dominio->Id);

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
   } // SPSNP_wrRedeDominio

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_dlEstudoRede(VTEstudo *estudo, AnsiString strRedeNome)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_dlEstudoRede(:p_EstudoId, :p_RedeNome); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->SetVariable("p_EstudoId", estudo->Id);
         ptrQry->DeclareVariable("p_RedeNome"  , otString);
         ptrQry->SetVariable("p_RedeNome", strRedeNome.Trim());

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
   } // SPSNP_dlEstudoRede

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_dlEstudoRede(VTEstudo *estudo, int rede_id)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_dlEstudoRede(:p_EstudoId, :p_RedeId); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_EstudoId"  , otInteger);
         ptrQry->SetVariable("p_EstudoId", estudo->Id);
         ptrQry->DeclareVariable("p_RedeId"  , otInteger);
         ptrQry->SetVariable("p_RedeId", rede_id);

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
   } // SPSNP_dlEstudoRede

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_dlManobra(VTManobra* manobra)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

   if (!manobra)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_MANOBRAINV
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_dlManobra(:p_ManobraId); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_ManobraId"  , otInteger);
         ptrQry->SetVariable("p_ManobraId", manobra->Id);

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
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_dlManobraChave(VTManobra* manobra)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

   if (!manobra)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_MANOBRAINV
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_dlManobraChave(:p_ManobraId); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_ManobraId"  , otInteger);
         ptrQry->SetVariable("p_ManobraId", manobra->Id);

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
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_dlSincronismo(VTLock* lock)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

   if (!lock)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SINCRONISMOINV
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_dlSincronismo(:p_SincronismoId); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_SincronismoId"  , otInteger);
         ptrQry->SetVariable("p_SincronismoId", lock->Id);

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
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_dlSincronismoChave(VTLock* lock)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;

   if (!lock)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SINCRONISMOINV
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_dlSincronismoChave(:p_SincronismoId); ");
         ptrQry->SQL->Add("END;");

         ptrQry->DeclareVariable("p_SincronismoId"  , otInteger);
         ptrQry->SetVariable("p_SincronismoId", lock->Id);

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
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_rdManobra(void)
   {
   //variáveis locais
   bool          sucesso = true;
   int           intManobraId;
   int           intManobraIdPrev;
   int           intRedeId;
   int           intChaveId;
   bool          bolFechada;
   VTChave*      chave;
   VTManobra*    manobra;
   VTManobras*   manobras = (VTManobras*)apl->GetObject(__classid(VTManobras));
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

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
      return(false);
      }

   if (!manobras)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_MANOBRASINV
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdManobra(:p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor"  , otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         // Recupera o resultado
         intManobraIdPrev = IDINDEFINIDO;
         while (!ptrCursor->Eof)
            {
            intManobraId = ptrCursor->Field("ManobraId");
            intRedeId    = ptrCursor->Field("RedeId");
            intChaveId   = ptrCursor->Field("ChaveId");
            bolFechada   = bool(int(ptrCursor->Field("Fechada")));
            //verifica se é uma Manobra nova
            if (intManobraId != intManobraIdPrev)
               {//salva ult_manobra_id lido
               intManobraIdPrev = intManobraId;
               //cria um novo objeto Manobra
               manobra                      = manobras->InsereManobra();
               manobra->Id                  = intManobraId;
               manobra->Codigo              = ptrCursor->Field("Codigo");
               manobra->Status[sttNOVO]     = false;
               manobra->Status[sttALTERADO] = false;
               }
            //verifica se o objeto Manobra não foi destruído devido à ausência de Chave
            if (manobra != NULL)
               {
               //determina Chave do objeto Locked
               if ((chave = bd_func->ExisteChave(redes, intRedeId, intChaveId)) == NULL)
                  {
                  //Chave da Manobra não disponível: descarta Manobra
                  manobras->RetiraManobra(manobra);
                  manobra = NULL;
                  }
                  else
                  {
                  //insere um objeto Locked na Manobra
                  manobra->InsereLocked(chave, bolFechada);
                  }
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
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_rdSincronismo(void)
   {
   //variáveis locais
   bool          sucesso = true;
   int           intSincronismoId;
   int           intSincronismoIdPrev;
   int           intRedeId;
   int           intChaveId;
   bool          bolFechada;
   bool          bolMaster;
   VTChave*      chave;
   VTLock*       lock;
   VTLocked*     locked;
   VTLocks*      locks = (VTLocks*)apl->GetObject(__classid(VTLocks));
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

   if (!redes)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_REDESINV
      return(false);
      }

   if (!locks)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_MANOBRASINV
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_rdSincronismo(:p_cursor);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_cursor"  , otCursor);
         ptrQry->SetComplexVariable("p_cursor", ptrCursor);

         // Executa query e depois abre o cursor com os resultados
         ptrQry->Execute();
         ptrCursor->Execute();
         // Recupera o resultado
         intSincronismoIdPrev = IDINDEFINIDO;
         while (!ptrCursor->Eof)
            {
            intSincronismoId = ptrCursor->Field("SincronismoId");
            intRedeId        = ptrCursor->Field("RedeId");
            intChaveId       = ptrCursor->Field("ChaveId");
            bolFechada       = bool(int(ptrCursor->Field("Fechada")));
            bolMaster        = bool(int(ptrCursor->Field("Master")));
            //verifica se é uma Manobra nova
            if (intSincronismoId != intSincronismoIdPrev)
               {//salva ult_manobra_id lido
               intSincronismoIdPrev = intSincronismoId;
               //cria um novo objeto Manobra
               lock                      = locks->InsereLock();
               lock->Id                  = intSincronismoId;
               lock->Codigo              = ptrCursor->Field("Codigo");
               lock->Status[sttENABLED]  = bool(int(ptrCursor->Field("Enabled")));
               lock->Status[sttNOVO]     = false;
               lock->Status[sttALTERADO] = false;
               }
            //verifica se o objeto Manobra não foi destruído devido à ausência de Chave
            if (lock != NULL)
               {
               //determina Chave do objeto Locked
               if ((chave = bd_func->ExisteChave(redes, intRedeId, intChaveId)) == NULL)
                  {
                  //Chave da Manobra não disponível: descarta Manobra
                  locks->RetiraLock(lock);
                  lock = NULL;
                  }
                  else
                  {
                  //insere um objeto Locked no Lock
                  if (bolMaster) { lock->InsereMaster(chave, bolFechada); }
                  else           { lock->InsereSlave(chave, bolFechada);  }
                  }
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
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrManobra(VTEstudo* estudo)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   VTManobra*    manobra;
   VTManobras*   manobras = (VTManobras*)apl->GetObject(__classid(VTManobras));
   TList*        lisMANOBRAS;
   int           intManobraId;

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

   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
      return(false);
      }

   if (!manobras)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_MANOBRASINV
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrManobra(:p_ManobraId, :p_Codigo);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_Codigo"     , otString );
         ptrQry->DeclareVariable("p_ManobraId"  , otInteger);

         if ((lisMANOBRAS = manobras->LisManobra()) == NULL)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_MANOBRASINV
            return(false);
            }
            
         for (int n = 0; n < lisMANOBRAS->Count; n++)
            {
            // loop manobras (ini)
            manobra = (VTManobra*)lisMANOBRAS->Items[n];
            if (!manobra)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_MANOBRAINV
               return(false);
               }

            // Verifica se o objeto é novo ou alterado
            if ((!manobra->Status[sttNOVO]) && (!manobra->Status[sttALTERADO]))
               {
               continue;
               }
            // Verifica se o objeto é novo ou alterado
            if (manobra->Status[sttNOVO])
               {
               intManobraId = IDINDEFINIDO;
               }
            else
               {
               // Apaga as chaves associadas ao sincronismo. Não pode chamar dentro da procedure, pois a procedure pode ser
               // chamada mais de uma vez para uma mesma mesma manobra
               if (!SPSNP_dlManobraChave(manobra))
                  {
                  SNPSTRLOGLINE(strlog);
                  return(false);
                  }

               intManobraId = manobra->Id;
               }

            ptrQry->SetVariable("p_ManobraId", intManobraId   );
            ptrQry->SetVariable("p_Codigo"   , manobra->Codigo);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            manobra->Id = ptrQry->GetVariable("p_ManobraId");
            if (manobra->Id == IDINDEFINIDO)
               {
               // Condição de erro
               sucesso = false;
               SNPSTRLOGLINE(strlog);
               }

            // Insere as chaves da manobra
            if (!SPSNP_wrManobraChave(estudo, manobra))
               {
               sucesso = false;
               SNPSTRLOGLINE(strlog);
               }

            // Atualiza o estados das manobras
            manobra->Status[sttNOVO]     = false;
            manobra->Status[sttALTERADO] = false;
            // loop manobras (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrManobraChave(VTEstudo* estudo, VTManobra* manobra)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   TList*        lisLOCKED;
   VTLocked*     locked;
   VTChave*      chave;

   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
      return(false);
      }

   if (!manobra)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_MANOBRAINV
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrManobraChave(:p_ManobraId, :p_EstudoId, :p_ChaveId, :p_Fechada); ");
         ptrQry->SQL->Add("END;");

         // Loop das chaves por manobra
         if ((lisLOCKED = manobra->LisLocked()) == NULL)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_MANOBRAINV
            return(false);
            }

         for (int i = 0; i < lisLOCKED->Count; i++)
            {
            // Determina os valores dos parâmetros
            if ((locked = (VTLocked*)lisLOCKED->Items[i]) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_SINCRONISMOINV
               return(false);
               }

            if ((chave = locked->chave) == NULL)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_MANOBRAINV
               return(false);
               }
            // Seta os valores dos parâmetros
            ptrQry->DeclareVariable("p_ManobraId", otInteger); ptrQry->SetVariable("p_ManobraId", manobra->Id);
            ptrQry->DeclareVariable("p_EstudoId" , otInteger); ptrQry->SetVariable("p_EstudoId" , estudo->Id);
            ptrQry->DeclareVariable("p_ChaveId"  , otInteger); ptrQry->SetVariable("p_ChaveId"  , chave->Id);
            ptrQry->DeclareVariable("p_Fechada"  , otInteger); ptrQry->SetVariable("p_Fechada"  , locked->estado);

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
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrSincronismo(VTEstudo* estudo)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   VTLock*       lock;
   VTLocks*      locks = (VTLocks*)apl->GetObject(__classid(VTLocks));
   TList*        lisLOCKS;
   int           intSincronismoId;

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

   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
      return(false);
      }

   if (!locks)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SINCRONISMOSINV
      return(false);
      }

   __try
      {
      try
         {
         ptrQry = new TOracleQuery(NULL); ptrQry->Session = bd_session;

         // Cria estrutura do comando
         ptrQry->Clear();
         ptrQry->SQL->Add("BEGIN");
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrSincronismo (:p_SincronismoId, :p_Codigo, :p_Enabled);");
         ptrQry->SQL->Add("END;");
         ptrQry->DeclareVariable("p_Codigo"     , otString );
         ptrQry->DeclareVariable("p_Enabled"  , otInteger);
         ptrQry->DeclareVariable("p_SincronismoId"  , otInteger);

         if ((lisLOCKS = locks->LisLock()) == NULL)
            {
            SNPSTRLOGLINE(strlog);
            MSGERRSYS_SINCRONISMOSINV
            return(false);
            }

         for (int n = 0; n < lisLOCKS->Count; n++)
            {
            // loop manobras (ini)
            lock = (VTLock*)lisLOCKS->Items[n];
            if (!lock)
               {
               SNPSTRLOGLINE(strlog);
               MSGERRSYS_SINCRONISMOINV
               return(false);
               }

            // Verifica se o objeto é novo ou alterado
            if ((!lock->Status[sttNOVO]) && (!lock->Status[sttALTERADO]))
               {
               continue;
               }
            // Verifica se o objeto é novo ou alterado
            if (lock->Status[sttNOVO])
               {
               intSincronismoId = IDINDEFINIDO;
               }
            else
               {
               // Apaga as chaves associadas ao sincronismo. Não pode chamar dentro da procedure, pois a procedure é chamada
               // mais de uma vez para um mesmo sincronismo
               if (!SPSNP_dlSincronismoChave(lock))
                  {
                  SNPSTRLOGLINE(strlog);
                  return(false);
                  }

               intSincronismoId = lock->Id;
               }

            ptrQry->SetVariable("p_SincronismoId", intSincronismoId   );
            ptrQry->SetVariable("p_Codigo"       , lock->Codigo);
            ptrQry->SetVariable("p_Enabled"      , (lock->Status[sttENABLED]) ? 1 : 0);

            // Executa query
            ptrQry->Execute();

            // Verifica o valor retornado a ser atualizado no objeto em memória
            lock->Id = ptrQry->GetVariable("p_SincronismoId");
            if (lock->Id == IDINDEFINIDO)
               {
               // Condição de erro
               sucesso = false;
               SNPSTRLOGLINE(strlog);
               }

            // Insere as chaves da manobra
            if (!SPSNP_wrSincronismoChave(estudo, lock))
               {
               sucesso = false;
               SNPSTRLOGLINE(strlog);
               }

            // Atualiza status dos sincronismos
            lock->Status[sttNOVO]     = false;
            lock->Status[sttALTERADO] = false;
            // loop manobras (fim)
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
      //destrói objetos locais
      if (ptrQry) { delete ptrQry; ptrQry = NULL; }
      }

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::SPSNP_wrSincronismoChave(VTEstudo* estudo, VTLock* lock)
   {
   //variáveis locais
   bool          sucesso = true;
   AnsiString    strlog;
   TOracleQuery* ptrQry = NULL;
   TList*        lisLOCKED;
   VTLocked*     locked;
   VTChave*      chave;

   if (!estudo)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_ESTUDOINV
      return(false);
      }

   if (!lock)
      {
      SNPSTRLOGLINE(strlog);
      MSGERRSYS_SINCRONISMOINV
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
         ptrQry->SQL->Add(" PCK_SINAP.SPSNP_wrSincronismoChave(:p_SincronismoId, :p_EstudoId, :p_ChaveId, :p_Master, :p_Fechada); ");
         ptrQry->SQL->Add("END;");

         // Loop das chaves por sincronismo master / slave
         for (int ndxMaster = 1; ndxMaster >= 0; ndxMaster--)
            {
            if (ndxMaster == 1)
               {
               if ((lisLOCKED = lock->LisMaster()) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_MANOBRAINV
                  return(false);
                  }
               }
            else
               {
               if ((lisLOCKED = lock->LisSlave()) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_MANOBRAINV
                  return(false);
                  }
               }

            for (int i = 0; i < lisLOCKED->Count; i++)
               {
               // Determina os valores dos parâmetros
               if ((locked = (VTLocked*)lisLOCKED->Items[i]) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_SINCRONISMOINV
                  return(false);
                  }

               if ((chave = locked->chave) == NULL)
                  {
                  SNPSTRLOGLINE(strlog);
                  MSGERRSYS_MANOBRAINV
                  return(false);
                  }

               // Seta os valores dos parâmetros
               ptrQry->DeclareVariable("p_SincronismoId", otInteger); ptrQry->SetVariable("p_SincronismoId", lock->Id);
               ptrQry->DeclareVariable("p_EstudoId"     , otInteger); ptrQry->SetVariable("p_EstudoId"     , estudo->Id);
               ptrQry->DeclareVariable("p_ChaveId"      , otInteger); ptrQry->SetVariable("p_ChaveId"      , chave->Id);
               ptrQry->DeclareVariable("p_Master"       , otInteger); ptrQry->SetVariable("p_Master"       , ndxMaster);
               ptrQry->DeclareVariable("p_Fechada"      , otInteger); ptrQry->SetVariable("p_Fechada"      , locked->estado);

               // Executa query
               ptrQry->Execute();
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
bool __fastcall TBDeqptoOracle::ValidaVersaoBaseParaGravacao(void)
	{
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TBDeqptoOracle::ValidaVersaoBaseParaLeitura(void)
	{
	return(true);
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TBDeqptoOracle::VersaoBase(void)
	{
	return("");
	}

//---------------------------------------------------------------------------
//eof

