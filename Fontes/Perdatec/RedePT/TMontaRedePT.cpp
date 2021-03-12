//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMontaRedePT.h"
#include "VTRamais.h"
#include "VTRedePT.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Arranjo\VTArranjo.h"
#include "..\..\Classes\VTClasse.h"
#include "..\..\Classes\VTClasses.h"
#include "..\..\Curvas\VTCurva.h"
#include "..\..\Geral\VTGeral.h"
#include "..\..\Geral\CfgPerdaTec\VTCfgPerdaTec.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTCapacitor.h"
#include "..\..\Rede\VTCarga.h"
#include "..\..\Rede\VTCorte.h"
#include "..\..\Rede\VTFases.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTTrecho.h"
#include "..\..\Rede\VTTipoRede.h"
#include "..\..\Rede\VTTrafo.h"
#include "..\..\Zona\VTZona.h"
#include "..\..\Zona\VTZonas.h"
#include "..\..\..\DLL_Inc\Curvas.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMontaRedePT* __fastcall NewObjMontaRedePT(VTApl *apl)
   {
   try{
      return(new TMontaRedePT(apl));
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaRedePT::TMontaRedePT(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria listas
   lisEQP    = new TList();
   lisRAMAL  = new TList();
   lisREDEPT = new TList();
   //cria objetos
   ramais = NewObjRamais(apl);
   }

//---------------------------------------------------------------------------
__fastcall TMontaRedePT::~TMontaRedePT(void)
   {
   //destr�i objetos
   if (ramais) {delete ramais; ramais = NULL;}
   //destr�i lista sem destruir seus objetos
   if (lisEQP)   {delete lisEQP;   lisEQP   = NULL;}
   if (lisRAMAL) {delete lisRAMAL; lisRAMAL = NULL;}
   //destr�i lista e seus objetos
   if (lisREDEPT) {LimpaTList(lisREDEPT); delete lisREDEPT; lisREDEPT = NULL;}
   }
/*
//---------------------------------------------------------------------------
void __fastcall TMontaRedePT::AlteraNomeRede(void)
   {
   //vari�veis locais
   VTRede *rede, *sub;
   TList *lisREDE = redes->LisRede();

   for(int n = 0; n < lisREDE->Count; n++)
	  {//somente redes prim�rias
	  rede = (VTRede*)lisREDE->Items[n];
	  if(rede->TipoRede->Segmento != redePRI) continue;
	  //determina subesta��o
	  sub = ExisteSubestacao(rede);
	  if(sub != NULL)
		 {
		 rede->Codigo = sub->Codigo + "_" + rede->Codigo;
		 }
	  }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TMontaRedePT::AlteraNomeRede(void)
   {
   //vari�veis locais
   VTRede *rede, *clone;
   TList *lisREDE = redes->LisRede();
   VTLicencas *licencas = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTLicenca *licenca = licencas->ExisteLicenca();

   //verifica se � da Copel (altera��o exclusiva da Copel)
   if ((licenca != NULL) && (licenca->Empresa != COPEL))
	  return;

   for(int n = 0; n < lisREDE->Count; n++)
	  {//somente redes prim�rias
	  rede = (VTRede*)lisREDE->Items[n];
	  if(rede->TipoRede->Segmento != redePRI) continue;
	  //verifica se existe extern_id
	  if(rede->Extern_id.IsEmpty()) continue;
	  //verifica se tem um clone da rede
	  clone = (VTRede*)rede->Obj;
	  if(clone == NULL)
		 {
		 clone = rede->Clone();
		 rede->Obj = clone;
         }
	  //concatena extern_id com codigo
	  rede->Codigo = rede->Extern_id + "_" + clone->Codigo;
	  }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaRedePT::Clear(void)
   {
   //destroi objetos rede_pt
   LimpaTList(lisREDEPT);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaRedePT::DeterminaPerdaMedidor(VTCarga *carga, double &p_w)
   {
   //vari�veis locais
   int           num_elementos;
   PERDA_MEDIDOR *perda_med;

   //zera vari�vel de retorno
   p_w = 0;
   //identifica perdas p/ medidor
   perda_med = geral->CfgPerdaTec->ExistePerdaMedidor();
   //verifica se a Carga est� em Zona trif�sica
   if (carga->pbarra->zona->Trifasica)
      {//Carga ligada a uma rede trif�sica
      switch(carga->Fases & faseABCN)
         {
         case faseA:
         case faseB:
         case faseC:
         case faseAN:
         case faseBN:
         case faseCN: //medidor monof�sico
            num_elementos = 1;
            break;
         case faseAB:
         case faseBC:
         case faseCA:   //medidor monof�sico
            num_elementos = 1;
            break;
         case faseABN:
         case faseBCN:
         case faseCAN:  //medidor bif�sico
            num_elementos = 2;
            break;
         case faseABC:  //medidor bif�sico
            num_elementos = 2;
            break;
          case faseABCN: //medidor trif�sico
            num_elementos = 3;
            break;
          default:
            num_elementos = 2;
            break;
         }
      }
   else
      {//Carga ligada a uma rede monof�sica ou delta aberto/fechado
      switch(carga->Fases & faseABCN)
         {
         case faseA:
         case faseB:
         case faseC:
         case faseAN:
         case faseBN:
         case faseCN: //medidor monof�sico
            num_elementos = 1;
            break;
         case faseAB:
         case faseBC:
         case faseCA:   //medidor monof�sico
            num_elementos = 2;
            break;
         case faseABN:
         case faseBCN:
         case faseCAN:  //medidor monof�sico
            num_elementos = 1;
            break;
         case faseABC:  //medidor bif�sico
            num_elementos = 2;
            break;
          case faseABCN: //medidor bif�sico de 3 elementos (OBS: assumindo trif�sico)
            num_elementos = 3;
            break;
          default:
            num_elementos = 2;
            break;
         }
      }
   //prote��o
   if (perda_med == NULL) return(false);
   p_w = perda_med->perda_w * num_elementos;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaRedePT::Executa(void)
   {
   //obt�m objetos
   fases     = (VTFases*)apl->GetObject(__classid(VTFases));
   classes   = (VTClasses*)apl->GetObject(__classid(VTClasses));
   geral     = (VTGeral*)apl->GetObject(__classid(VTGeral));
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   zonas     = (VTZonas*)apl->GetObject(__classid(VTZonas));

   //inicia Zonas da rede
   zonas->Executa(redes);
   //insere Eqptos na rede
   if (! InsereEqptosRedePT()) return(false);
   //configura perda dos transformadores SED
   if (geral->CfgPerdaTec->OpcaoPerdaTrafoSED != opTRAFO_GIS)
      {
      if (! RedefineTrafoSED_PerdaFeCu()) return(false);
      }
   //configura perda dos transformadores SDBT
   if (geral->CfgPerdaTec->OpcaoPerdaTrafo != opTRAFO_GIS)
      {
      if (! RedefineTrafoSDBT_PerdaFeCu()) return(false);
      }
   //configura Zat das redes MRT
   if (geral->CfgPerdaTec->Zat_mrt)
      {
      if (! RedefineTrafoSDBT_Zat()) return(false);
      }
   //configura op��o de rede sem neutro
   if (! geral->CfgPerdaTec->RedeComNeutro)
      {
      if (! RedefineCarga_SemNeutro()) return(false);
      }
   //configura op��o de capacitores ligados/desligados
   if (! RedefineRede_Capacitor(geral->CfgPerdaTec->RedeComCapacitor))  return(false);
   //altera nome das redes MT (concatenando com o numero que est� no extern_id)
   AlteraNomeRede();

   return(true);
   }

//---------------------------------------------------------------------------
PERDA_MEDIDOR* __fastcall TMontaRedePT::ExistePerdaMedidor(VTCarga *carga)
   {
   //determina PERDA_MEDIDOR
   return(geral->CfgPerdaTec->ExistePerdaMedidor());
   }

//---------------------------------------------------------------------------
PERDA_RAMAL* __fastcall TMontaRedePT::ExistePerdaRamal(VTCarga *carga)
   {
   //vari�veis locais
   int    num_fases;
   double v_kv;

   //determina n�mero de fases
   num_fases = fases->NumeroDeFases(carga->Fases);
   //determina tens�o da Carga
   if (num_fases == 1)
      {//tens�o fase/neutro
      switch(carga->Fases & faseABC)
         {
         case faseA: v_kv = carga->pbarra->zona->Vfn_kv[indFASE_A]; break;
         case faseB: v_kv = carga->pbarra->zona->Vfn_kv[indFASE_B]; break;
         case faseC: v_kv = carga->pbarra->zona->Vfn_kv[indFASE_C]; break;
         default:    v_kv = carga->pbarra->zona->Vfn_kv[indFASE_A]; break;
         }
      }
   else
      {//tens�o fase/neutro
      v_kv = carga->pbarra->zona->Vff_kv;
      }
   //determina PERDA_RAMAL
   return(geral->CfgPerdaTec->ExistePerdaRamal(num_fases, v_kv));
   }
/*
//---------------------------------------------------------------------------
PERDA_TRAFO*   __fastcall TMontaRedePT::ExistePerdaTrafo(VTTrafo *trafo)
   {
   //vari�veis locais
   int    tipo_trf;

   //verifica se Trafo monof�sico ou trif�sico
   if (trafo->Monofasico) tipo_trf = trfMONOFASICO;
   else                   tipo_trf = trfTRIFASICO;
   //determina PERDA_TRAFO
   return(geral->CfgPerdaTec->ExistePerdaTrafo(tipo_trf, trafo->snom, trafo->sec.vnom));
   }
*/
//---------------------------------------------------------------------------
PERDA_TRAFO* __fastcall TMontaRedePT::ExistePerdaTrafo(VTTrafo *trafo)
   {
   //vari�veis locais
   int tipo;

   //DVK 2016.07.19 converte unidade da pot�ncia nominal
   tipo = (trafo->Monofasico) ? trfMONOFASICO : trfTRIFASICO;
   return(geral->CfgPerdaTec->ExistePerdaTrafo(tipo, trafo->snom * 1000., trafo->pri.vnom));
   }

//---------------------------------------------------------------------------
PERDA_TRAFO_SED*   __fastcall TMontaRedePT::ExistePerdaTrafoSED(VTTrafo *trafo)
   {
   //determina PERDA_TRAFO
   return(geral->CfgPerdaTec->ExistePerdaTrafoSED(trafo->snom));
   }

//---------------------------------------------------------------------------
VTRedePT* __fastcall TMontaRedePT::ExisteRedePT(VTRede *rede)
   {
   //vari�veis locais
   VTRedePT *redePT;

   for (int n = 0; n < lisREDEPT->Count; n++)
      {
      redePT = (VTRedePT*)lisREDEPT->Items[n];
      if (redePT->Rede == rede) return(redePT);
      }
   return(NULL);
   }
/*
//---------------------------------------------------------------------------
VTTrecho* __fastcall TMontaRedePT::ExisteRamalLigacao(VTCarga *carga, TList *lisRAMAL)
   {
   //vari�veis locais
   VTTrecho *trecho;

   for (int n = 0; n < lisRAMAL->Count; n++)
      {
      trecho =(VTTrecho*)lisRAMAL->Items[n];
      if (trecho->ConectaBarra(carga->pbarra)) return(trecho);
      }
   return(NULL);
   }
*/
//---------------------------------------------------------------------------
VTTrecho* __fastcall TMontaRedePT::ExisteRamalLigacao(VTCarga *carga, TList *lisRAMAL)
   {//DVK 2017.06.02
   //vari�veis locais
   VTTrecho *trecho;

   for (int n = 0; n < lisRAMAL->Count; n++)
      {
      trecho =(VTTrecho*)lisRAMAL->Items[n];
      if (trecho->ConectaBarra(carga->pbarra))
         {//verifica se o trecho possui as mesmas fases da carga
         if(carga->Fases == trecho->arranjo->Fases) return(trecho);
         }
      }
   return(NULL);
   }
/*
//---------------------------------------------------------------------------
VTRede* __fastcall TMontaRedePT::ExisteSubestacao(VTRede *rede_pri)
   {
   //vari�veis locais
   VTCorte *corte;
   VTRede *rede, *sub;
   TList *lisREDE = new TList();
   TList *lisBARRA;

   //inicia ponteiro
   sub = NULL;
   //determina lista de subesta��es
   redes->LisRede(lisREDE, redeETD);
   for(int n = 0; n < lisREDE->Count; n++)
	  {
	  rede = (VTRede*)lisREDE->Items[n];
	  lisBARRA = rede->LisBarra();
	  //verifica se a barra inicial da rede_pri faz parte das barras da rede
	  if(lisBARRA->IndexOf(rede_pri->BarraInicial()) >= 0)
		 {
		 sub = rede;
		 break;
		 }
	  //verifica se h� corte entre rede e rede_pri
	  corte = rede->ExisteCorte(rede_pri);
	  if(corte != NULL)
		 {
		 sub = rede;
		 break;
		 }
      }

   delete lisREDE;
   return sub;
   }
*/
//---------------------------------------------------------------------------
int __fastcall TMontaRedePT::IniciaLisRamal(VTRedePT *redePT, TList *lisRAMAL)
   {
   //vari�veis locais
   int       count = 0;
   VTLigacao *ligacao;
   VTTrecho  *trecho;
   TList     *lisLIGACAO = redePT->Rede->LisLigacao();

   for (int n = 0; n < lisLIGACAO->Count; n++)
      {
      ligacao = (VTLigacao*)lisLIGACAO->Items[n];
      if (ligacao->Tipo() == eqptoTRECHO)
         {//verifica se o Trecho utiliza Arranjo de ramal
         trecho = (VTTrecho*)ligacao;
         if (trecho->arranjo->ramal)
            {//insere Trecho em lisRAMAL
            lisRAMAL->Add(trecho);
            count++;
            }
         }
      }
   return(count);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaRedePT::InsereEqptosRedePT(void)
   {
   //vari�veis locais
   VTRede       *rede;
   VTRedePT     *redePT;
   TList        *lisREDE;

   try{//destr�i eventuais RedePT criados anteriormente
      LimpaTList(lisREDEPT);
      //loop p/ todas Redes
      lisREDE   = redes->LisRede();
      for (int n = 0; n < lisREDE->Count; n++)
         {//verifica se � uma Rede secund�ria e est� carregada
         rede = (VTRede*)lisREDE->Items[n];
         if (rede->TipoRede->Segmento != redeSEC) continue;
         if (! rede->Carregada) continue;
         //cria objeto RedePT
         lisREDEPT->Add(redePT = NewObjRedePT());
         //associa RedePT com sua Rede
         redePT->Rede = rede;

         //IMPORTANTE: cria Trecho do ramal ANTES de criar a Carga do medidor para que a Carga
         //            j� fique associada � Barra correta (barra final do ramal)

         //verifica se o c�lculo de perda em ramal est� utilizando os ramais configurados no Gis
         if (geral->CfgPerdaTec->OpcaoPerdaRamal == opRAMAL_NAO_USAR)
            {//elimina os Trechos que s�o ramal de liga��o
            RetiraEqpto_RamalLigacao(redePT);
            }
         else if (geral->CfgPerdaTec->OpcaoPerdaRamal == opRAMAL_GIS)
            {//N�O cria trecho do ramal
            }
         else
            {//cria Trecho do ramal se o c�lculo for por dentro
            //DVK 2017.06.02: remove os trechos e insere para todas as cargas
            if (! geral->CfgPerdaTec->RamalPorFora)
               {//elimina os Trechos que s�o ramal de liga��o
               RetiraEqpto_RamalLigacao(redePT);
               //cria novos ramais para cada Carga
               InsereEqpto_TrechoRamal(redePT);
               }
            }
         //cria Carga do medidor
         if (! geral->CfgPerdaTec->MedidorPorFora) InsereEqpto_CargaMedidor(redePT);
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaRedePT::InsereEqpto_CargaMedidor(VTRedePT *redePT)
   {
   //vari�veis locais
   VTCarga  *carga;
   VTCarga  *carga_med;

   //obt�m lista de Cargas da Rede
   lisEQP->Clear();
   redePT->Rede->LisEqbar(lisEQP, eqptoCARGA);
   //cria uma Barra e um ramal de liga��o para cada Carga
   for (int n = 0; n < lisEQP->Count; n++)
      {
      if ((carga = (VTCarga*)lisEQP->Items[n]) == NULL) continue;;
      //prote��es
      if (carga->IP) continue;
      //verifica se a carga � do grupo A ou classe A4 //DVK 2016.12.02
      if (carga->GrupoA) continue;
	  if (carga->classe->Tag == consA4) continue;
	  //verifica se a carga est� numa barra MT
	  if(carga->pbarra->vnom >= 1.0) continue;
      //prote��o: verifica se a Carga est� associada a uma Barra sem Zona (isolada)
      if ((carga->pbarra->zona == NULL)||
          (carga->pbarra->zona->Tipo == zonaINDEFINIDA)) continue;
      //cria uma nova Carga para respresentar o medidor
      if ((carga_med = MontaCargaMedidor(carga)) == NULL) continue;
      //insere Carga do medidor na mesma Barra da Carga
      carga->pbarra->InsereEqbar(carga_med);
      //salva Carga do medidor em redePT
      redePT->InsereCargaMedidor(carga_med);
      }
   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMontaRedePT::InsereEqpto_TrechoRamal(VTRedePT *redePT)
   {
   //vari�veis locais
   PERDA_RAMAL *perda_ramal;
   VTArranjo   *arranjo;
   VTBarra     *barra;
   VTCarga     *carga;
   VTLigacao   *ligacao;
   VTTrecho    *trecho;

   //�nicia lisRAMAL com os Ramais de liga��o existentes na Rede
   lisRAMAL->Clear();
   IniciaLisRamal(redePT, lisRAMAL);
   //obt�m lista de Cargas da Rede
   lisEQP->Clear();
   redePT->Rede->LisEqbar(lisEQP, eqptoCARGA);
   //loop para todas Carga
   for (int n = 0; n < lisEQP->Count; n++)
      {
      if ((carga = (VTCarga*)lisEQP->Items[n]) == NULL) continue;;
      //prote��es
      if (carga->IP) continue;
      //verifica se a carga � do grupo A ou classe A4 //DVK 2016.12.02
      if (carga->GrupoA) continue;
      if (carga->classe->Tag == consA4) continue;
      //prote��o: verifica se a Carga est� associada a uma Barra sem Zona (isolada)
//      if ((carga->pbarra->zona == NULL)||
//          (carga->pbarra->zona->Tipo == zonaINDEFINIDA)) continue;
      if (carga->pbarra->zona == NULL) continue;
      //determina PERDA_RAMAL para a Carga
      if ((perda_ramal = ExistePerdaRamal(carga)) == NULL) continue;
      //determina Arranjo p/ o Trecho do ramal de liga��o da Carga
      if ((arranjo = ramais->ArranjoRamal(perda_ramal->r_ohm_km, carga->Fases)) == NULL) continue;
      //verifica se j� existe o Trecho que � o ramal da Carga
      if ((trecho = ExisteRamalLigacao(carga, lisRAMAL)) != NULL)
         {//redefine comprimento e arranjo do Trecho que � ramal
         trecho->DefineObjArranjo(arranjo);
         trecho->Comprimento_km = perda_ramal->comp_m * 0.001;
         }
      else
         {//cria uma nova Barra e insere em RedePT
         if ((barra = carga->pbarra->Clone()) == NULL) return(false);
         barra->Id   = FalsoId();
         barra->zona = carga->pbarra->zona;
         redePT->InsereBarra(barra);
         //IMPORTANTE: salva ponteiro da Barra original na Carga
         carga->Obj = carga->pbarra;
         //cria Trecho do ramal de liga��o e insere em RedePT
         if ((ligacao = MontaTrechoRamal(perda_ramal->comp_m, carga->pbarra, barra, arranjo)) == NULL) continue;
         redePT->InsereLigacaoRamal(ligacao);
         //insere Barra em Redes
         redes->InsereBarra(barra);
         //insere Trecho do ramal na Rede
         redePT->Rede->InsereLigacao(ligacao);
         //transfere Carga da sua Barra original para a nova Barra
         carga->pbarra->RemoveEqbar(carga);
         barra->InsereEqbar(carga);
         }
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TMontaRedePT::InsereEqpto_TrechoRamal(VTRedePT *redePT)
   {//DVK 2017.06.02: sempre insere um trecho novo como ramal
   //vari�veis locais
   PERDA_RAMAL *perda_ramal;
   VTArranjo   *arranjo;
   VTBarra     *barra;
   VTCarga     *carga;
   VTLigacao   *ligacao;
   VTTrecho    *trecho;

   //�nicia lisRAMAL com os Ramais de liga��o existentes na Rede
   lisRAMAL->Clear();
   IniciaLisRamal(redePT, lisRAMAL);
   //obt�m lista de Cargas da Rede
   lisEQP->Clear();
   redePT->Rede->LisEqbar(lisEQP, eqptoCARGA);
   //loop para todas Carga
   for (int n = 0; n < lisEQP->Count; n++)
      {
      if ((carga = (VTCarga*)lisEQP->Items[n]) == NULL) continue;;
      //prote��es
      if (carga->IP) continue;
      //verifica se a carga � do grupo A ou classe A4 //DVK 2016.12.02
      if (carga->GrupoA) continue;
      if (carga->classe->Tag == consA4) continue;
      //prote��o: verifica se a Carga est� associada a uma Barra sem Zona (isolada)
      if (carga->pbarra->zona == NULL) continue;
      //determina PERDA_RAMAL para a Carga
      if ((perda_ramal = ExistePerdaRamal(carga)) == NULL) continue;
      //determina Arranjo p/ o Trecho do ramal de liga��o da Carga
      if ((arranjo = ramais->ArranjoRamal(perda_ramal->r_ohm_km, carga->Fases)) == NULL) continue;
      //cria uma nova Barra e insere em RedePT
      if ((barra = carga->pbarra->Clone()) == NULL) return(false);
      barra->Id   = FalsoId();
      barra->zona = carga->pbarra->zona;
      redePT->InsereBarra(barra);
      //IMPORTANTE: salva ponteiro da Barra original na Carga
      carga->Obj = carga->pbarra;
      //cria Trecho do ramal de liga��o e insere em RedePT
      if ((ligacao = MontaTrechoRamal(perda_ramal->comp_m, carga->pbarra, barra, arranjo)) == NULL) continue;
      redePT->InsereLigacaoRamal(ligacao);
      //insere Barra em Redes
      redes->InsereBarra(barra);
      //insere Trecho do ramal na Rede
      redePT->Rede->InsereLigacao(ligacao);
      //transfere Carga da sua Barra original para a nova Barra
      carga->pbarra->RemoveEqbar(carga);
      barra->InsereEqbar(carga);
      }
   return(true);
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TMontaRedePT::MontaCargaMedidor(VTCarga *carga)
   {
   //vari�veis locais
   double   p_w;
   double   valor[nvCURVA_PQ];
   strHM    hm_ini, hm_fim;
   VTCurva  *curva;

   VTCarga  *carga_med;

   try{//determina perda do medidor, em fun��o das caracter�sticas da Carga
      if (! DeterminaPerdaMedidor(carga, p_w)) return(NULL);
      //cria uma nova Carga, com um �nico patamar
      carga_med                 = DLL_NewObjCarga(patamares->NumPatamar());
      carga_med->Id             = FalsoId();
      carga_med->Codigo         = "Medidor";
      carga_med->Extern_id      = "";
      carga_med->Energia_mwhmes = 0;
      carga_med->DefineClasse(classes->ExisteClasseTag(consEQUIVALENTE));
      carga_med->Fases      = carga->Fases;
      carga_med->IP_NumLamp = 0;
      carga_med->IP_Pot_kw  = 0;
      carga_med->IP_Fatpot  = 1;
      carga_med->esq        = carga->esq;
      //define modelo de Carga: Scte
      carga_med->Icte_pu    = 0;
      carga_med->Scte_pu    = 1;
      carga_med->Zcte_pu    = 0;
      //carga definida por Curva pr�pria
      curva                = DLL_NewObjCurva();
      curva->Id            = FalsoId();
      curva->Codigo        = "";
      curva->Tipo          = curvaPQ;
      curva->Unidade       = unidMVA;
      curva->Escala        = pat24HORA;
      curva->NumeroValores = nvCURVA_PQ;
      curva->Tipica        = false;
      curva->Eqpto         = NULL;
      curva->Classe        = NULL;
      //define ponto �nico da Curva
      hm_ini.hora   =  0;
      hm_ini.minuto =  0;
      hm_fim.hora   = 24;
      hm_fim.minuto =  0;
      valor[indP]   = p_w * 1e-6;
      valor[indQ]   = 0;
      curva->SetPonto(hm_ini, hm_fim, valor, nvCURVA_PQ);
      //insere Cruva na Carga
      carga_med->DefineCurva(curva);
      }catch(Exception &e)
         {
         carga_med = NULL;
         }
   return(carga_med);
   }

//---------------------------------------------------------------------------
VTLigacao* __fastcall TMontaRedePT::MontaTrechoRamal(double comp_m, VTBarra *bar1, VTBarra *bar2, VTArranjo *arranjo)
   {
   //vari�veis locais
   VTTrecho *trecho;

   try{//cria um Arranjo clone para ser definido como pr�prio do Trecho
      //arranjo = arranjo->Clone();
      //arranjo->Owner = NULL;
      //cria um novo Trecho
      trecho          = DLL_NewObjTrecho(patamares->NumPatamar());
      trecho->Id      = FalsoId();
      trecho->Codigo  = "Ramal";
      trecho->pbarra1 = bar1;
      trecho->pbarra2 = bar2;
      trecho->Comprimento_km = comp_m * 0.001;
      trecho->DefineObjArranjo(arranjo);
      }catch(Exception &e)
         {
         trecho = NULL;
         }
   return(trecho);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMontaRedePT::RedeAlterada(void)
   {
   return(rede_alterada);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TMontaRedePT::RedefineCarga_SemNeutro(void)
   {
   //vari�veis locais
   VTCarga *carga;
   VTRede  *rede;
   TList   *lisREDE;

   try{//loop p/ todas Redes
      lisREDE  = redes->LisRede();
      for (int n = 0; n < lisREDE->Count; n++)
         {//verifica se � uma Rede secund�ria e est� carregada
         rede = (VTRede*)lisREDE->Items[n];
         if (rede->TipoRede->Segmento != redeSEC) continue;
         if (! rede->Carregada) continue;
         //obt�m lista de Cargas da Rede
         lisEQP->Clear();
         rede->LisEqbar(lisEQP, eqptoCARGA);
         //loop p/ todas as Cargas
         for (int n = 0; n < lisEQP->Count; n++)
            {
            carga = (VTCarga*)lisEQP->Items[n];
            //prote��es
            //if (carga->IP) continue;
            //if (carga->pbarra->zona == NULL) continue;
            //if (carga->pbarra->zona->Tipo == zonaINDEFINIDA) continue;
            //redefine fases da Carga sem neutro
            if (fases->NumeroDeFases(carga->Fases) == 1)
               {//redefine a Carga como fase/terra
               //BLOQUEADO por suspeita de bug em Gauss3
               //carga->Fases = (carga->Fases & faseABC) | faseT;
               }
            else
               {//redefine a Carga sem o neutro
               carga->Fases = carga->Fases & faseABC;
               }
            }
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaRedePT::RedefineRede_Capacitor(bool enabled)
   {
   //vari�veis locais
   VTCapacitor *capacitor;

   try{//inicia lista com todos capacitores
      lisEQP->Clear();
      redes->LisEqpto(lisEQP, eqptoCAPACITOR);
      //loop p/ todos os Eqptos
      for (int n = 0; n < lisEQP->Count; n++)
         {
         capacitor = (VTCapacitor*)lisEQP->Items[n];
         capacitor->Status[sttENABLED] = enabled;
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaRedePT::RedefineTrafoSDBT_PerdaFeCu(void)
   {
   //vari�veis locais
   double      r_pu;
   PERDA_TRAFO *perda_trafo;
   VTTrafo     *trafo;
   VTRede      *rede;
   TList       *lisREDE;

   try{//loop p/ todas Redes
      lisREDE  = redes->LisRede();
      for (int n = 0; n < lisREDE->Count; n++)
         {//verifica se � uma Rede secund�ria e est� carregada
         rede = (VTRede*)lisREDE->Items[n];
         if (rede->TipoRede->Segmento != redeSEC) continue;
         if (! rede->Carregada) continue;
         //obt�m lista de Cargas da Rede
         lisEQP->Clear();
         rede->LisLigacao(lisEQP, eqptoTRAFO);
         //loop p/ todos os Trafos
         for (int n = 0; n < lisEQP->Count; n++)
            {
            trafo = (VTTrafo*)lisEQP->Items[n];
            //determina PERDA_TRAFO para o trafo
            if ((perda_trafo = ExistePerdaTrafo(trafo)) == NULL) continue;
            //redefine perda no ferro
            trafo->perda_ferro = 100. * ((perda_trafo->perda_vazio_w * 1e-6) / trafo->snom);
            //redefine resist�ncia em pu
            r_pu = ((perda_trafo->perda_total_w  - perda_trafo->perda_vazio_w) * 1e-6) / trafo->snom;
            trafo->z0.r = r_pu;
            trafo->z1.r = r_pu;
            }
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaRedePT::RedefineTrafoSDBT_Zat(void)
   {
   //vari�veis locais
   VTTrafo     *trafo;
   VTRede      *rede;
   TList       *lisREDE;

   try{//loop p/ todas Redes
      lisREDE  = redes->LisRede();
      for (int n = 0; n < lisREDE->Count; n++)
         {//verifica se � uma Rede secund�ria e est� carregada
         rede = (VTRede*)lisREDE->Items[n];
         if (rede->TipoRede->Segmento != redeSEC) continue;
         if (! rede->Carregada) continue;
         //obt�m lista de Cargas da Rede
         lisEQP->Clear();
         rede->LisLigacao(lisEQP, eqptoTRAFO);
         //loop p/ todos os Trafos
         for (int n = 0; n < lisEQP->Count; n++)
            {
            trafo = (VTTrafo*)lisEQP->Items[n];
            //verifica se o prim�rio est� aterrado
            if (trafo->pri.fase & faseT)
               {//redefine Zat do prim�rio
               trafo->pri.zterra.r = geral->CfgPerdaTec->Zat_ohm;
               trafo->pri.zterra.x = 0.;
               }
            }
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaRedePT::RedefineTrafoSED_PerdaFeCu(void)
   {
   //vari�veis locais
   double           r_pu;
   PERDA_TRAFO_SED *perda_trafo_sed;
   VTTrafo         *trafo;
   VTRede          *rede;
   TList           *lisREDE;

   try{//loop p/ todas Redes
      lisREDE  = redes->LisRede();
      for (int n = 0; n < lisREDE->Count; n++)
         {//verifica se � uma Rede secund�ria e est� carregada
         rede = (VTRede*)lisREDE->Items[n];
         if (rede->TipoRede->Segmento != redeETD) continue;
         if (! rede->Carregada) continue;
         //obt�m lista de Cargas da Rede
         lisEQP->Clear();
         rede->LisLigacao(lisEQP, eqptoTRAFO);
         //loop p/ todos os Trafos
         for (int n = 0; n < lisEQP->Count; n++)
            {
            trafo = (VTTrafo*)lisEQP->Items[n];
            //determina PERDA_TRAFO_SED para o trafo
            if ((perda_trafo_sed = ExistePerdaTrafoSED(trafo)) == NULL) continue;
            //redefine perda no ferro
            trafo->perda_ferro = perda_trafo_sed->perda_vazio_perc;
            //redefine resist�ncia em pu
            r_pu        = (perda_trafo_sed->perda_total_perc  - perda_trafo_sed->perda_vazio_perc) * 0.01;
            trafo->z0.r = r_pu;
            trafo->z1.r = r_pu;
            }
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaRedePT::RetiraEqpto_RamalLigacao(VTRedePT  *redePT)
   {
   //vari�veis locais
   VTBarra     *bar_orig, *bar_dest;
   VTCarga     *carga;
   VTTrecho    *trecho;

   //inicia lisRAMAL com os Ramais de liga��o existentes na Rede
   lisRAMAL->Clear();
   IniciaLisRamal(redePT, lisRAMAL);
   //obt�m lista de Cargas da Rede
   lisEQP->Clear();
   redePT->Rede->LisEqbar(lisEQP, eqptoCARGA);
   //loop para todas Carga
   for (int n = 0; n < lisEQP->Count; n++)
      {
      if ((carga = (VTCarga*)lisEQP->Items[n]) == NULL) continue;;
      //prote��es
      if (carga->IP) continue;
      //prote��o: verifica se a Carga est� associada a uma Barra sem Zona (isolada)
      if ((carga->pbarra->zona == NULL)||
          (carga->pbarra->zona->Tipo == zonaINDEFINIDA)) continue;
      //verifica se existe o Trecho que � o ramal da Carga
      if ((trecho = ExisteRamalLigacao(carga, lisRAMAL)) != NULL)
         {//determina Barra original Carga
         bar_orig = carga->pbarra;
         //determina barra destino da Carga
         if (trecho->pbarra2 == carga->pbarra) bar_dest = trecho->pbarra1;
         else                                  bar_dest = trecho->pbarra2;
         //transfere Carga de sua Barra original para sua Barra destino
         bar_orig->RemoveEqbar(carga);
         bar_dest->InsereEqbar(carga);
         //retira Trecho da Rede e destr�i objeto
         //redePT->Rede->RemoveLigacao(trecho);
         //DLL_DeleteEqpto(trecho);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof



