//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TSubestacao.h"
#include "VTPrimario.h"
#include "VTReticulado.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTSubestacao* __fastcall NewObjSubestacao(VTApl *apl_owner)
   {
   try{
      return (new TSubestacao(apl_owner));
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TSubestacao::TSubestacao(VTApl *apl_owner)
   {
   //salva ponteiro para objeto
   this->apl = apl_owner;
   //cria objetos locais
   lisEQP        = new TList();
   lisPRIMARIO   = new TList();
   lisRETICULADO = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TSubestacao::~TSubestacao(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisEQP)        {delete lisEQP;        lisEQP        = NULL;}
   if (lisPRIMARIO)   {delete lisPRIMARIO;   lisPRIMARIO   = NULL;}
   if (lisRETICULADO) {delete lisRETICULADO; lisRETICULADO = NULL;}
   }

//---------------------------------------------------------------------------
VTPrimario* __fastcall TSubestacao::ExistePrimario(VTRede *rede)
   {
   //variáveis locais
   VTPrimario *primario;

   //procura Primario associado à Rede indicada
   for (int n = 0; n < lisPRIMARIO->Count; n++)
      {
      primario = (VTPrimario*)lisPRIMARIO->Items[n];
      if (primario->Rede == rede) return(primario);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TSubestacao::InserePrimario(VTPrimario *primario)
   {
   //proteção
   if (primario == NULL) return;
   ////insere Primario em lisPRIMARIO
   if (lisPRIMARIO->IndexOf(primario) < 0)
      {
      lisPRIMARIO->Add(primario);
      //associa Primario c/ esta Subestacao
      primario->Subestacao = this;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TSubestacao::InsereReticulado(VTReticulado *reticulado)
   {
   //proteção
   if (reticulado == NULL) return;
   ////insere Reticulado em lisRETICULADO
   if (lisRETICULADO->IndexOf(reticulado) < 0)
      {
      lisRETICULADO->Add(reticulado);
      //associa Primario c/ esta Subestacao
      reticulado->Subestacao = this;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TSubestacao::LisEqptoSubPriSec(TList *lisEXT, int tipo_eqpto, bool barra_isolada)
   {
   //variáveis locais
   VTPrimario *primario;

   //obtém lista de Eqptos da Subestacao
   Rede->LisEqpto(lisEXT, tipo_eqpto, barra_isolada);
   //obtém lista de Eqptos de todos Primários e Secundários
   for (int n = 0; n < lisPRIMARIO->Count; n++)
      {
      primario = (VTPrimario*)lisPRIMARIO->Items[n];
      primario->LisEqptoPriSec(lisEXT, tipo_eqpto, barra_isolada);
      }
	}

//---------------------------------------------------------------------------
TList* __fastcall TSubestacao::LisPrimario(void)
   {
   return(lisPRIMARIO);
   }

//---------------------------------------------------------------------------
int __fastcall TSubestacao::LisPrimario(VTTrafo *trafo, TList *lisEXT)
	{
	return(LisPrimarioSupridoPor(trafo, lisEXT));
	}

//---------------------------------------------------------------------------
int __fastcall TSubestacao::LisPrimario(VTTrafo3E *trafo3E, TList *lisEXT)
	{
	return(LisPrimarioSupridoPor(trafo3E, lisEXT));
	}

//---------------------------------------------------------------------------
int __fastcall TSubestacao::LisPrimarioSupridoPor(VTLigacao *lig_ref, TList *lisEXT)
	{
	//variáveis locais
	bool       achou;
	TList      *lisBAR;
	TList      *lisLIG;
	VTBarra    *barra;
	VTLigacao  *ligacao;
	VTPrimario *primario;

	//cria listas
	lisBAR = new TList();
	lisLIG = new TList();
	//loop p/ todos Primarios
	for (int np = 0; np < lisPRIMARIO->Count; np++)
		{
		primario = (VTPrimario*)lisPRIMARIO->Items[np];
		//proteção: verifica se o Primario tem Barra inicial definida
		if ((barra = primario->BarraInicial) == NULL) continue;
		//reinicia lisBAR com a Barra inicial do Primario
		lisBAR->Clear();
		lisBAR->Add(barra);
		//reinicia lisLIG com todas Ligacoes da Subestação
		lisLIG->Clear();
		Rede->LisLigacao(lisLIG);
		//loop p/ todas as Barras em lisBAR
		achou = false;
		for (int nb = 0; (! achou)&&(nb < lisBAR->Count); nb++)
			{
			barra = (VTBarra*)lisBAR->Items[nb];
			//loop p/ todas Ligacoes em lisLIG
			for (int nl = lisLIG->Count-1;  (! achou)&&(nl >= 0); nl--)
				{
				ligacao = (VTLigacao*)lisLIG->Items[nl];
				//verifica se a Ligacao conecta a Barra
				if (! ligacao->ConectaBarra(barra)) continue;
				//verifica se a Ligacao é o Trafo indicado
				if (ligacao == lig_ref)
					{//insere Primario em lisEXT
					lisEXT->Add(primario);
					//interrompe a busca
					achou = true;
					}
				else
					{//inclui Barras da Ligacao em lisBAR
					ligacao->LisBarra(lisBAR);
					}
				}
			}
		}
	//destrói listas sem destruir seus objetos
	delete lisBAR;
	delete lisLIG;
	//retorna número de Primarios alimentados pelo Trafo
	return(lisEXT->Count);
	}

//---------------------------------------------------------------------------
TList* __fastcall TSubestacao::LisReticulado(void)
	{
	return(lisRETICULADO);
	}

//---------------------------------------------------------------------------
void __fastcall TSubestacao::LisRede(TList *lisEXT, int segmento)
   {
   //variáveis locais
   VTPrimario *primario;

   switch(segmento)
      {
      case redeETD: //nada a fazer
         break;
      case redePRI:
         for (int n = 0; n < lisPRIMARIO->Count; n++)
            {
            primario = (VTPrimario*)lisPRIMARIO->Items[n];
            if (lisEXT->IndexOf(primario->Rede) < 0) lisEXT->Add(primario->Rede);
           }
         break;
      case redeSEC:
         for (int n = 0; n < lisPRIMARIO->Count; n++)
            {
            primario = (VTPrimario*)lisPRIMARIO->Items[n];
            primario->LisRede(lisEXT, segmento);
           }
         break;
      }
   }

//---------------------------------------------------------------------------
VTPatamar* __fastcall TSubestacao::PatamarDemandaMaxima(complex<double>&s_mva)
   {
   //variáveis locais
   bool            barra_isolada=false;
   //loop para todos patamares;
   complex<double> pq_mva;
   VTCarga         *carga;
   VTPatamar       *patamar_max=NULL;
   VTDemanda       *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //obtém lista de Cargas da Subestacao, e de todos Primario e Secundarios
   lisEQP->Clear();
   LisEqptoSubPriSec(lisEQP, eqptoCARGA, barra_isolada);
   //zera demanda máxima
   s_mva = CZero();
   //loop para todos patamares
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
VTPatamar* __fastcall TSubestacao::PatamarDemandaMinima(complex<double>&s_mva)
   {
   //variáveis locais
   bool            barra_isolada=false;
   //loop para todos patamares;
   complex<double> pq_mva;
   VTCarga         *carga;
   VTPatamar       *patamar_min=NULL;
   VTDemanda       *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares     *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //obtém lista de Cargas da Subestacao, e de todos Primario e Secundarios
   lisEQP->Clear();
   LisEqptoSubPriSec(lisEQP, eqptoCARGA, barra_isolada);
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
double __fastcall TSubestacao::PM_GetSfirme_mva(void)
   {
   //variáveis locais
   int        count      = 0;
   double     stotal_mva = 0;
   double     strafo_mva = 0;
   VTLigacao  *ligacao;
   VTTrafo    *trafo;
   VTTrafo3E  *trafo3e;
   TList      *lisLIG = Rede->LisLigacao();

   //determina número de trafos, Snom_mva total e Snom do maior trafo
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      if (ligacao->Tipo() == eqptoTRAFO)
         {
         trafo = (VTTrafo*)ligacao;
         //soma potência nominal do Trafo
         stotal_mva += trafo->snom;
         //determina potencia nominal do maior trafo
         if (trafo->snom > strafo_mva) strafo_mva = trafo->snom;
         count++;
         }
      else if (ligacao->Tipo() == eqptoTRAFO3E)
         {
         trafo3e= (VTTrafo3E*)ligacao;
         //soma potência nominal do Trafo
         stotal_mva += trafo3e->Sbase;
         //determina potencia nominal do maior trafo
         if (trafo3e->Sbase > strafo_mva) strafo_mva = trafo3e->Sbase;
         count++;
         }
      }
   //verifica se há mais de um trafo
   if (count > 1) stotal_mva -= strafo_mva;

   return(stotal_mva);
   }

//---------------------------------------------------------------------------
double __fastcall TSubestacao::PM_GetSnom_mva(void)
   {
   //variávei locais
   double     stotal_mva = 0.;
   VTLigacao  *ligacao;
   VTTrafo    *trafo;
   VTTrafo3E  *trafo3e;
   TList      *lisLIG = Rede->LisLigacao();

   //loop p/ todas Ligacoes
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      if (ligacao->Tipo() == eqptoTRAFO)
         {
         trafo = (VTTrafo*)ligacao;
         //soma potência nominal do Trafo
         stotal_mva += trafo->snom;
         }
      else if (ligacao->Tipo() == eqptoTRAFO3E)
         {
         trafo3e= (VTTrafo3E*)ligacao;
         //soma potência nominal do Trafo3E
         stotal_mva += trafo3e->Sbase;
         }
      }
   return(stotal_mva);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TSubestacao::PM_GetRede(void)
   {
   return(PD.rede);
   }

//---------------------------------------------------------------------------
void __fastcall TSubestacao::PM_SetRede(VTRede *rede)
   {
   PD.rede = rede;
   }

//---------------------------------------------------------------------------
//eof
