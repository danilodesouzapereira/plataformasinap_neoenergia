//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TSecundario.h"
#include "VTPrimario.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTSecundario* __fastcall NewObjSecundario(VTApl *apl_owner)
   {
   try{
      return (new TSecundario(apl_owner));
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TSecundario::TSecundario(VTApl *apl_owner)
   {
   //salva ponteiro para objeto
   this->apl = apl_owner;
   //cria objetos
   lisEQP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TSecundario::~TSecundario(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisEQP)    {delete lisEQP;    lisEQP    = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TSecundario::Ordena(TList *lisEXT_BAR, TList *lisEXT_LIG)
   {
   //variáveis locais
   VTBarra   *barra;
	VTChave   *chave;
	VTEqpto   *eqpto;
   VTLigacao *ligacao_filha, *ligacao_pai;

   //determina Barra inicial
   if ((barra = BarraInicial) == NULL) return(false);
	//verifica se a Barra inicial está associada com sua Ligacao pai (definida na ordenação do Primario)
	if ((eqpto = (VTEqpto*)(barra->Obj)) != NULL)
		{//verifica se Eqpto é uma Ligacao
		if (! eqpto->TipoLigacao()) barra->Obj  = NULL;
		}
	//inicia lisBAR_EXT com a Barra inicial
	lisEXT_BAR->Clear();
	lisEXT_BAR->Add(barra);
	//inicia lisEQP com todas Ligacoes da rede
	lisEQP->Clear();
   Rede->LisLigacao(lisEQP);
   //inicia lisEXT_LIG com as Ligacoes que conectam a Barra inicial
   lisEXT_LIG->Clear();
   for (int nl = lisEQP->Count-1; nl >= 0; nl--)
      {
      ligacao_filha = (VTLigacao*)lisEQP->Items[nl];
      //verifica se é uma Chave aberta
      if (ligacao_filha->Tipo() == eqptoCHAVE)
         {
         chave = (VTChave*)ligacao_filha;
         if (chave->Aberta)
            {//retira Ligacao de lisEQP
            lisEQP->Delete(nl);
            continue;
            }
         }
      //verifica se a Ligacao conecta a Barra
      if (ligacao_filha->ConectaBarra(barra))
         {//transfere Ligacao filha de lisEQP p/ lisEXT_LIG
         lisEQP->Delete(nl);
         lisEXT_LIG->Add(ligacao_filha);
			//define Ligacao pai igual à Ligacao pai da barra inicial
			ligacao_filha->ligaPai = (VTLigacao*)barra->Obj;
         }
      }
   //loop p/ todas Ligacoes pai que estão em lisEXT_LIG
   for (int nlp = 0; nlp < lisEXT_LIG->Count; nlp++)
      {
      ligacao_pai = (VTLigacao*)lisEXT_LIG->Items[nlp];
      //loop p/ todas Barras da Ligacao pai
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if ((barra = ligacao_pai->Barra(ind_bar)) == NULL) continue;
         //descarta Barras já analisadas
         if (lisEXT_BAR->IndexOf(barra) >= 0) continue;
         //insere Barra em lisEXT_BAR
         lisEXT_BAR->Add(barra);
         //salva em Barra a Ligacao pai que a conectou
         barra->Obj = ligacao_pai;
         //determina Ligacoes em lisEQP que se conectam à Barra
         for (int nl = lisEQP->Count-1; nl >= 0; nl--)
            {//OBS: as chaves abertas foram excluídas de lisEQP no loop anterior
            ligacao_filha = (VTLigacao*)lisEQP->Items[nl];
            //verifica se a Ligacao conecta a Barra
            if (ligacao_filha->ConectaBarra(barra))
               {//transfere Ligacao filha de lisEQP p/ lisEXT_LIG
               lisEQP->Delete(nl);
               lisEXT_LIG->Add(ligacao_filha);
               //associa Ligacao filha c/ Ligacao Pai
               ligacao_filha->ligaPai = ligacao_pai;
               }
            }
         }
      }
   //limpa listas locais
   lisEQP->Clear();
   return(true);
   }

//---------------------------------------------------------------------------
VTPatamar* __fastcall TSecundario::PatamarDemandaMaxima(complex<double>&s_mva)
   {
   //variáveis locais
   bool            barra_isolada=false;
   //loop para todos patamares;
   complex<double> pq_mva;
   VTCarga         *carga;
   VTPatamar       *patamar_max=NULL;
   VTDemanda       *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //obtém lista de Cargas da rede
   lisEQP->Clear();
   Rede->LisEqbar(lisEQP, eqptoCARGA, barra_isolada);
   //zera demanda máxima
   s_mva = CZero();
   //loop p/ todos patamares
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      //totaliza demanda de todas Cargas no patamar
      pq_mva = CZero();
      for (int nc = 0; nc < lisEQP->Count; nc++)
         {
         carga   = (VTCarga*)lisEQP->Items[nc];
         pq_mva += demanda->S_mva[carga][ind_pat];
         }
      //verifica valor máximo
      if ((ind_pat == 0) || (Abs(pq_mva) > Abs(s_mva)))
         {
         s_mva       = pq_mva;
         patamar_max = patamares->Patamar[ind_pat];
         }
      }
   return(patamar_max);
   }

//---------------------------------------------------------------------------
VTPatamar* __fastcall TSecundario::PatamarDemandaMinima(complex<double>&s_mva)
   {
   //variáveis locais
   bool            barra_isolada=false;
   //loop para todos patamares;
   complex<double> pq_mva;
   VTCarga         *carga;
   VTPatamar       *patamar_min=NULL;
   VTDemanda       *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //obtém lista de Cargas da rede
   lisEQP->Clear();
   Rede->LisEqbar(lisEQP, eqptoCARGA, barra_isolada);
   //zera demanda mínima
   s_mva = CZero();
   //loop p/ todos patamares
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      //totaliza demanda de todas Cargas no patamar
      pq_mva = CZero();
      for (int nc = 0; nc < lisEQP->Count; nc++)
         {
         carga   = (VTCarga*)lisEQP->Items[nc];
         pq_mva += demanda->S_mva[carga][ind_pat];
         }
      //verifica valor mínimo
      if ((ind_pat == 0) || (Abs(pq_mva) < Abs(s_mva)))
         {
         s_mva       = pq_mva;
         patamar_min = patamares->Patamar[ind_pat];
         }
      }
   return(patamar_min);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TSecundario::PM_GetBarraInicial(void)
   {
   return(PD.barra_inicial);
   }

//---------------------------------------------------------------------------
VTPrimario* __fastcall TSecundario::PM_GetPrimario(void)
   {
   return(PD.primario);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TSecundario::PM_GetRede(void)
   {
   return(PD.rede);
   }

//---------------------------------------------------------------------------
void __fastcall TSecundario::PM_SetBarraInicial(VTBarra *barra)
   {
   PD.barra_inicial = barra;
   }

//---------------------------------------------------------------------------
void __fastcall TSecundario::PM_SetRede(VTRede* rede)
   {
   PD.rede = rede;
   }

//---------------------------------------------------------------------------
void __fastcall TSecundario::PM_SetPrimario(VTPrimario* primario)
   {
   PD.primario = primario;
   }

//---------------------------------------------------------------------------
//eof
