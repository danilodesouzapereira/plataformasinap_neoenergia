//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TTopologia.h"
#include "VTClassifica.h"
#include "VTMontaMNet.h"
#include "VTRebuild.h"
#include "VTReconfigura.h"
#include "VTRecorta.h"
#include "..\Apl\VTapl.h"
#include "..\Alerta\VTAlerta.h"
//#include "..\Cronometro\VTCronometro.h"
#include "..\Mercado\VTMercado.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTRedes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTTopologia* __fastcall NewObjTopologia(VTApl *apl)
   {
   return(new TTopologia(apl));
   }

//---------------------------------------------------------------------------
__fastcall TTopologia::TTopologia(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl  = apl;
   //cria objetos
   rebuild     = NewObjRebuild(apl);
   reconfigura = NewObjReconfigura(apl);
	classifica  = NewObjClassifica(apl);
   monta_mnet  = NewObjMontaMNet(apl);
   lisNR  = new TList();
   lisRR  = new TList();
   lisVIZ = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TTopologia::~TTopologia(void)
   {
   //destrói objetos criados
   if (rebuild)     {delete rebuild;     rebuild     = NULL;}
   if (reconfigura) {delete reconfigura; reconfigura = NULL;}
   if (classifica)  {delete classifica;  classifica  = NULL;}
   if (monta_mnet)  {delete monta_mnet;  monta_mnet  = NULL;}
   //destrói listas sem destruir seus objetos
   if (lisNR)  {delete lisNR;  lisNR  = NULL;}
   if (lisRR)  {delete lisRR;  lisRR  = NULL;}
   if (lisVIZ) {delete lisVIZ; lisVIZ = NULL;}
	}

//---------------------------------------------------------------------------
void __fastcall TTopologia::ClassificaRedes(VTRedes *redes)
	{
	//verifica se o objeto Redes foi recebido
	if (redes == NULL)
		{//utiliza objeto Redes do Apl
		redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
		}
	//salva ponteiro para objeto
	this->redes = redes;
	//executa classificação das Redes: cria MRedes
   classifica->Executa(redes);
   //monta MNet p/ cálculo parcial de fluxo de potência
   monta_mnet->Executa(redes);
   }

//---------------------------------------------------------------------------
void __fastcall TTopologia::LimpaListas(void)
   {
   lisNR->Clear();
   lisRR->Clear();
   lisVIZ->Clear();
   }

//---------------------------------------------------------------------------
bool __fastcall TTopologia::Rebuild(VTRedes *redes, TList *lisREDE, TList *lisBARRA, TList *lisLIGACAO)
	{
   //variáveis locais
   VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

	//salva ponteiro para objeto
	this->redes = redes;
	//reconstrói as redes estáticas e reconfiguráveis
	rebuild->Executa(redes, lisREDE, lisBARRA, lisLIGACAO);
   //insere eventuais Barras isoladas (sem senhuma Ligacao) na Rede isolada
   VerificaBarraIsolada();
   //calcula áreas das Redes
   redes->CalculaArea();
   //reinicia Cortes e MRedes
   ReiniciaCorteMRede();
   //reinicia Crescimento das Cargas
   if (mercado) mercado->IniciaRedes(redes);
   //limpa listas
   LimpaListas();
   return(true);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422540
bool __fastcall TTopologia::Reconfigura(VTRedes *redes)
   {
   //variáveis locais
   TList     *lisREDE;
	VTRede    *rede;
   VTMercado *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));
   VTAlerta *alerta = (VTAlerta*)apl->GetObject(__classid(VTAlerta));

	//verifica se o objeto Redes foi recebido
	if (redes == NULL)
		{//utiliza objeto Redes do Apl
		redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
		}
	//salva ponteiro para objeto
	this->redes = redes;
	//obtém lista de Redes
	lisREDE = redes->LisRede();
	//cronometro = NULL;
   //cronometro = (VTCronometro*)apl->GetObject(__classid(VTCronometro));
   //if (cronometro) cronometro->Reinicia();
   //monta lisRR c/ todas Redes reconfiguráveis e lisNR c/ todas Redes NÃO reconfiguráveis
   lisNR->Clear();
   lisRR->Clear();
   if (alerta)
	{
	alerta->Clear(true);
	}
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //if ((rede->Carregada)&&(! rede->Estatica))
      if ((rede->Carregada)&&(! rede->Estatica)&&(rede->BarraInicial() != NULL))
         {//Rede radial não estática: insere em lisRR
         lisRR->Add(rede);
         }
      else
         {//Rede não reconfigurável
         lisNR->Add(rede);
         }
      }
   //reinicia lista de Barras das Redes em lisNR
   //if (cronometro) cronometro->Start("Reinicia Barras", "");
   ReiniciaLisBarraRede(lisNR);
   //if (cronometro) cronometro->Stop("Reinicia Barras");
   //reconfigura Redes em lisRR
   //if (cronometro) cronometro->Start("Reconfigura Redes", "");
	reconfigura->Executa(redes, lisRR);
   //if (cronometro) cronometro->Stop("Reconfigura Redes");
	//insere eventuais Barras isoladas (sem senhuma Ligacao) na Rede isolada
   VerificaBarraIsolada();
   //calcula áreas das Redes
   redes->CalculaArea();
   //reinicia Cortes e MRedes
   ReiniciaCorteMRede();
   //reinicia Crescimento das Cargas
   if (mercado) mercado->IniciaRedes(redes);
   //limpa listas
   LimpaListas();
   //if (cronometro) cronometro->Show();
   //if (cronometro) cronometro->Reinicia();
   return(true);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422540
bool __fastcall TTopologia::ReconfiguraRede(VTRedes *redes, TList *lisREDE)
	{
   VTAlerta *alerta = (VTAlerta*)apl->GetObject(__classid(VTAlerta));
   //variáveis locais
   VTRede *rede;

	//salva ponteiro para objeto
	this->redes = redes;
	//monta lisRR c/ todas Redes reconfiguráveis e lisNR c/ todas Redes NÃO reconfiguráveis
   lisNR->Clear();
   lisRR->Clear();
   if (alerta)
	{
	alerta->Clear(true);
	}
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if ((rede->Carregada)&&(! rede->Estatica))
         {//Rede reconfigurável: insere em lisRR
         if (lisRR->IndexOf(rede) < 0) lisRR->Add(rede);
         //determina lista de Redes vizinhas
         lisVIZ->Clear();
         rede->LisRedeVizinha(lisVIZ);
         //verifica se há Redes vizinhas não estáticas
         for (int n = 0; n < lisVIZ->Count; n++)
            {
            rede = (VTRede*)lisVIZ->Items[n];
            if ((rede->Carregada)&&(! rede->Estatica))
               {//Rede reconfigurável: insere em lisRR
               if (lisRR->IndexOf(rede) < 0) lisRR->Add(rede);
               }
            else
               {//Rede não reconfigurável
               if (lisNR->IndexOf(rede) < 0) lisNR->Add(rede);
               }
            }
         }
      else
         {//Rede não reconfigurável
         if (lisNR->IndexOf(rede) < 0) lisNR->Add(rede);
         }
      }
   //reinicia lista de Barras das Redes em lisNR
   ReiniciaLisBarraRede(lisNR);
   //reconfigura Redes em lisRR
	reconfigura->Executa(redes, lisRR);
   //reinicia fases de todas as Barras
   //ReiniciaFasesDasBarras();
   //insere eventuais Barras isoladas (sem senhuma Ligacao) na Rede isolada
   VerificaBarraIsolada();
   //reinicia Cortes e MRedes
   ReiniciaCorteMRede();
   //limpa listas
   LimpaListas();
   return(true);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422540
bool __fastcall TTopologia::ReconfiguraRede(VTRedes *redes, VTRede *rede)
   {
	VTAlerta *alerta = (VTAlerta*)apl->GetObject(__classid(VTAlerta));

	//salva ponteiro para objeto
	this->redes = redes;
	//monta lisRR c/ todas Redes reconfiguráveis e lisNR c/ todas Redes NÃO reconfiguráveis
   lisNR->Clear();
   lisRR->Clear();
   if (alerta)
	{
	alerta->Clear(true);
	}
   //verifica se a Rede reconfigurável
   if ((rede->Carregada)&&(! rede->Estatica))
      {//insere Rede em lisRR
      lisRR->Add(rede);
      }
   else
      {//Rede não reconfigurável
      if (lisNR->IndexOf(rede) < 0) lisNR->Add(rede);
      }
   //determina lista de Redes vizinhas
   lisVIZ->Clear();
   rede->LisRedeVizinha(lisVIZ);
   //verifica se há Redes vizinhas não estáticas
   for (int n = 0; n < lisVIZ->Count; n++)
      {
      rede = (VTRede*)lisVIZ->Items[n];
      if ((rede->Carregada)&&(! rede->Estatica))
         {//Rede não estática: insere em lisRR
         lisRR->Add(rede);
         }
      else
         {//Rede não reconfigurável
         if (lisNR->IndexOf(rede) < 0) lisNR->Add(rede);
         }
      }
   //reinicia lista de Barras das Redes em lisNR
   ReiniciaLisBarraRede(lisNR);
   //reconfigura Redes em lisRR
   reconfigura->Executa(redes, lisRR);
   //insere eventuais Barras isoladas (sem senhuma Ligacao) na Rede isolada
   VerificaBarraIsolada();
   //reinicia Cortes e MRedes
   ReiniciaCorteMRede();
   //limpa listas
   LimpaListas();
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TTopologia::ReiniciaCorteMRede(void)
   {
	//executa classificação das Redes: cria MRedes
   classifica->Executa(redes);
   //if (cronometro) cronometro->Stop("Cria MRedes");
   //a junção de Redes em Malha pode permitir a eliminação de Cortes
   //recorta->VerificaFusaoDeCortes(redes);
   //monta MNet p/ cálculo parcial de fluxo de potência
   //if (cronometro) cronometro->Start("Cria MNets", "");
   monta_mnet->Executa(redes);
   //if (cronometro) cronometro->Stop("Cria MNets");
   return(true);
	}

//---------------------------------------------------------------------------
void __fastcall TTopologia::ReiniciaLisBarraRede(TList *lisREDE)
	{
	//variáveis locais
	VTRede *rede;

	//reinicia lista de Barras das Redes NÃO reconfiguráveis
	for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      rede->IniciaLisBarra();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TTopologia::VerificaBarraIsolada(void)
   {
   //variáveis locais
   VTBarra   *pbarra;
   VTLigacao *pliga;
   VTMRede   *mrede;
   VTRede    *rede;
   VTReducao *reducao;
   TList     *lisLIG;
   TList     *lisBAR;
   TList     *lisREDE;
   TList     *lisREDU;
   TList     *lisMR;

   //define todas Barras como isoladas
   lisBAR = redes->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      pbarra = (VTBarra*)lisBAR->Items[nb];
      pbarra->DefineStatus(sttISOLADO, true);
      }
   //loop p/ todas MRedes
   lisREDE = redes->LisRede();
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (rede->Carregada)
         {//verifica se a Rede possui Barra inicial
         if ((pbarra = rede->BarraInicial()) != NULL)
            {//marca Barra inicial como NÃO isoladas
            pbarra->DefineStatus(sttISOLADO, false);
            }
         //loop p/ todas Ligacoes da Rede
         lisLIG = rede->LisLigacao();
         for (int nl = 0; nl < lisLIG->Count; nl++)
            {
            pliga = (VTLigacao*)lisLIG->Items[nl];
            /*
            //verifica se a Ligacao é uma Chave aberta
            if (pliga->Tipo() == eqptoCHAVE)
               {
               if (((VTChave*)pliga)->Aberta) continue;
               }
            */
            //marca as Barras da Ligacao como NÃO isoladas
            for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
               {
               if ((pbarra = pliga->Barra(ind_bar)) == NULL) continue;
               pbarra->DefineStatus(sttISOLADO, false);
               }
            }
         }
      }
   //loop p/ todas as Reducoes
   lisMR = redes->LisMRede();
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->Carregada) continue;
      //loop p/ todas Reducoes da MRede
      lisREDU = mrede->LisReducao();
      for (int nr = 0; nr < lisREDU->Count; nr++)
         {
         reducao = (VTReducao*)lisREDU->Items[nr];
         //loop p/ todas Barras da Reducao
         lisBAR = reducao->LisBarra();
         for (int nb = 0; nb < lisBAR->Count; nb++)
            {
            pbarra = (VTBarra*)lisBAR->Items[nb];
            //verifica se a Barra não pertence ao Corte
            if (! reducao->Corte->ExisteBarra(pbarra))
               {
               pbarra->DefineStatus(sttISOLADO, false);
               }
            }
         }
      }
   //reinicia lista de barras isoladas
   redes->ReiniciaLisBarraIsolada();
   }

//---------------------------------------------------------------------------
//eof

