//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCaminho.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Rede\VTTrafo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCaminho
//---------------------------------------------------------------------------
VTCaminho* __fastcall NewObjCaminho(VTApl *apl)
   {
   return(new TCaminho(apl));
   }

//---------------------------------------------------------------------------
__fastcall TCaminho::TCaminho(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria listas
   lisBAR_INI = new TList();
   lisBAR_FIM = new TList();
   lisLIG     = new TList();
   lisRAMO    = new TList();
   lisSEL     = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TCaminho::~TCaminho(void)
   {
   //destrói listas sem destruir seus objetos
   if (lisBAR_INI)  {delete lisBAR_INI;  lisBAR_INI  = NULL;}
   if (lisBAR_FIM)  {delete lisBAR_FIM;  lisBAR_FIM  = NULL;}
   if (lisLIG)      {delete lisLIG;      lisLIG      = NULL;}
   //destrói lista e seus objetos
   if (lisRAMO) {LimpaTList(lisRAMO); delete lisRAMO; lisRAMO = NULL;}
   if (lisSEL)  {LimpaTList(lisSEL);  delete lisSEL;  lisSEL  = NULL;}
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TCaminho::BarraDoEqpto(VTEqpto *eqpto)
   {
   //proteção:
   if (eqpto == NULL) return(NULL);
   if (eqpto->TipoBarra())   return((VTBarra*)eqpto);
   if (eqpto->TipoEqbar())   return(((VTEqbar*)eqpto)->pbarra);
   if (eqpto->TipoLigacao()) return(((VTLigacao*)eqpto)->pbarra1);
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TCaminho::ChaveAberta(VTLigacao *ligacao)
   {
   if (ligacao->Tipo() == eqptoCHAVE)
      {
      if (((VTChave*)ligacao)->Aberta) return(true);
      }
   return(false);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TCaminho::Executa(VTEqpto *eqpto, TList *lisEXT)
   {
   //variáveis locais
   VTBarra   *barra, *bar_fim;
   VTLigacao *ligacao;
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //determina Barra p/ busca
   if((barra = BarraDoEqpto(eqpto)) == NULL) return;
   //insere o Eqpto em lisEXT, se for Ligacao
   if (eqpto->TipoLigacao()) InsereLigacao((VTLigacao*)eqpto, lisEXT);
   //obtém lista de Ligacoes de Redes
   lisLIG->Clear();
   redes->LisLigacao(lisLIG);
   //inicia lisBAR c/ a Barra indicada
   lisBAR->Clear();
   lisBAR->Add(barra);
   //loop p/ todas Barras
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      //monta conjunto de Ligacoes conectadas à Barra
      for (int nl = lisLIG->Count-1; nl >= 0; nl--)
         {
         ligacao = (VTLigacao*)lisLIG->Items[nl];
         //descarta Chaves aberta
         if (ChaveAberta(ligacao)) continue;
         //verifica se a Ligacao conecta a Barra
         if (! ligacao->ConectaBarra(barra)) continue;
         //transfere Ligacao de lisLIG p/ lisEXT
         lisLIG->Delete(nl);
         if (lisEXT->IndexOf(ligacao) < 0) lisEXT->Add(ligacao);
         //insere em lisBAR as demais Barras da Ligacao
         for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
            {//descarta Barra nula ou igual à Barra
            if ((bar_fim = ligacao->Barra(ind_bar)) == NULL)  continue;
            if (bar_fim == barra) continue;
            lisBAR->Add(bar_fim);
            }
         }
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TCaminho::Executa(VTEqpto *eqpto1, VTEqpto *eqpto2, TList *lisLIG_CAM)
   {
   //variáveis locais
   VTBarra *barra1, *barra2;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //reinicia lisRAMO e lisSEL
   LimpaTList(lisSEL);
   LimpaTList(lisRAMO);
   //determina Barras p/ busca
   if((barra1 = BarraDoEqpto(eqpto1)) == NULL) return;
   if((barra2 = BarraDoEqpto(eqpto2)) == NULL) return;
   //insere Eqpto1 em lisLIG_CAM, se for Ligacao
   if (eqpto1->TipoLigacao()) InsereLigacao((VTLigacao*)eqpto1, lisLIG_CAM);
   //if (eqpto2->TipoLigacao()) InsereLigacao((VTLigacao*)eqpto2, lisLIG_CAM);
   //obtém lista de Ligacoes
   lisLIG->Clear();
   redes->LisLigacao(lisLIG);
   //inicia lisBAR_INI
   lisBAR_INI->Clear();
   lisBAR_INI->Add(barra1);
   //inicia lisBAR_fim
   lisBAR_FIM->Clear();
   lisBAR_FIM->Add(barra2);
   //loop p/ todas as Barras em lisBAR_INI
   for (int nb = 0; nb < lisBAR_INI->Count; nb++)
      {
      barra1 = (VTBarra*)lisBAR_INI->Items[nb];
      do {//monta conjunto de Ramos a partir da Barra 1
         MontaRamos(barra1);
         } while (ExisteCaminhoBarraIniBarraFim());
      }
   //monta conjunto de Ligacoes que interligam as Barras 1 e 2
   MontaLigacoesInterligadas(lisLIG_CAM);
   //insere Eqpto2 em lisLIG_CAM, se for Ligacao
   if (eqpto2->TipoLigacao()) InsereLigacao((VTLigacao*)eqpto2, lisLIG_CAM);
   //descarta Ramos
   LimpaTList(lisRAMO);
   LimpaTList(lisSEL);
   }

//https://sinapsisenergia.teamwork.com/#/tasks/16586360
//---------------------------------------------------------------------------
void __fastcall TCaminho::Executa(TList *lisEQP_INI, TList *lisEQP_FIM, TList *lisLIG_ALL, TList *lisLIG_CAM)
   {
   //variáveis locais
   VTRedes   *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTEqpto *eqpto;
   VTBarra *barra;
   VTRede    *rede;
   TList *lisRedes = new TList();
   TList *lisLigacoesBar = new TList();
   AnsiString cod_eqpto, tipo;
   //reinicia lisRAMO e lisSEL
   LimpaTList(lisSEL);
   LimpaTList(lisRAMO);
   //inicia lisBAR_INI com as Barras dos Eqptos em lisEQP_INI
   lisBAR_INI->Clear();
   for (int n = 0; n < lisEQP_INI->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP_INI->Items[n];
      if ((barra = BarraDoEqpto(eqpto)) != NULL) lisBAR_INI->Add(barra);
      }
   //inicia lisBAR_FIM com as Barras dos Eqptos em lisEQP_FIM
   lisBAR_FIM->Clear();
   for (int n = 0; n < lisEQP_FIM->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP_FIM->Items[n];
	  if((barra = BarraDoEqpto(eqpto)) != NULL) lisBAR_FIM->Add(barra);
	  }
   if(lisBAR_FIM->Count == 0)
   {
		redes->LisRedeCarregada(lisRedes);
		for(int nr = 0 ; nr < lisRedes->Count; nr++)
		{
		   rede = (VTRede*)lisRedes->Items[nr];
		   if(rede->TipoRede->Segmento == redePRI)
		   {
			 if(((barra = rede->BarraInicial()) != NULL) && (lisBAR_FIM->IndexOf(barra) < 0))
			 {
				 lisBAR_FIM->Add(barra);
				 lisLigacoesBar->Clear();
				 barra->LisLigacao(lisLigacoesBar);
				 for(int nt = 0 ; nt < lisLigacoesBar->Count; nt++)
				 {
//					VTTrafo *trafo = (VTTrafo*)lisLigacoesBar->Items[nt];
					VTEqpto *eqpto_liga = (VTEqpto*)lisLigacoesBar->Items[nt];
					if((eqpto_liga->Tipo() == eqptoTRAFO3E) || (eqpto_liga->Tipo() == eqptoTRAFO))
					{
						//lisEQP_FIM->Add(eqpto_liga);
						VTTrafo *trafo = (VTTrafo*) eqpto_liga;
						if(((barra = trafo->BarraPrimario()) != NULL) && (lisBAR_FIM->IndexOf(barra) < 0))
						lisBAR_FIM->Add(barra);
					}
				 }
			 }
		   }
		}
   }
   //proteção
   if ((lisBAR_INI->Count == 0)||(lisBAR_FIM->Count == 0)) return;
   //insere as Ligacoes de lisEQP_INI em lisEXT
   for (int n = 0; n < lisEQP_INI->Count; n++)
	  {
	  eqpto = (VTEqpto*)lisEQP_INI->Items[n];
      if (eqpto->TipoLigacao()) InsereLigacao((VTLigacao*)eqpto, lisLIG_CAM);
      }
   //inicia lista com as Ligacoes
   lisLIG->Clear();
   CopiaTList(lisLIG_ALL, lisLIG);
   //loop p/ todas as Barras em lisBAR_INI
   for (int nb = 0; nb < lisBAR_INI->Count; nb++)
      {
	  barra = (VTBarra*)lisBAR_INI->Items[nb];
      do {//monta conjunto de Ramos a partir da Barra selecionada
         MontaRamos(barra);
		 } while (ExisteCaminhoBarraIniBarraFim());
	  }
   //monta conjunto de Ligacoes que interligam as Barras iniciais e finais
   MontaLigacoesInterligadas(lisLIG_CAM);
   //insere as Ligacoes de lisEQP_FIM em lisEXT
   for (int n = 0; n < lisEQP_FIM->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP_FIM->Items[n];
      if (eqpto->TipoLigacao()) InsereLigacao((VTLigacao*)eqpto, lisLIG_CAM);
      }
   //descarta Ramos
   LimpaTList(lisRAMO);
   LimpaTList(lisSEL);
   if (lisRedes) {delete lisRedes; lisRedes = NULL;}
   if (lisLigacoesBar) {delete lisLigacoesBar; lisLigacoesBar = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TCaminho::ExisteCaminhoBarraIniBarraFim(void)
   {
   //variáveis locais
   int     num_ramo = 0;
   TRamo   *ramo;

   //identifica Ramo que conecta uma Barra final
   while ((ramo = ExisteRamoBarraFinal(lisBAR_FIM)) != NULL)
      {//percore o Ramo selecionado e seus Ramos pai
      while (ramo != NULL)
         {//transfere Ramo de lisRAMO p/ lisSEL
         if (lisSEL->IndexOf(ramo) < 0)
            {//incrementa número de ramos
            num_ramo++;
            lisSEL->Add(ramo);
			lisRAMO->Remove(ramo);
            //retira Ligacao de lisLIG
            lisLIG->Remove(ramo->ligacao);
            //insere Barra inicial do Ramo em lisBAR_INI
            if (lisBAR_INI->IndexOf(ramo->bar_ini) < 0) lisBAR_INI->Add(ramo->bar_ini);
            //insere Barra inicial do Ramo em lisBAR_FIM
//            if (lisBAR_FIM->IndexOf(ramo->bar_ini) < 0) lisBAR_FIM->Add(ramo->bar_ini);
            }
         //determina Ramo pai
         ramo = ramo->pai;
         }
      }
   return(num_ramo > 0);
   }

//---------------------------------------------------------------------------
TRamo* __fastcall TCaminho::ExisteRamo(VTLigacao *ligacao, VTBarra *bar_ini, VTBarra *bar_fim)
   {
   //variáveis locais
   TRamo *ramo;

   //loop p/ todos Ramos em lisRAMO
   for (int nr = 0; nr < lisRAMO->Count; nr++)
      {
      ramo = (TRamo*)lisRAMO->Items[nr];
      if ((ramo->ligacao == ligacao)&&
          (ramo->bar_ini == bar_ini)&&
          (ramo->bar_fim == bar_fim)) return(ramo);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TRamo* __fastcall TCaminho::ExisteRamoBarraFinal(VTBarra *barra)
   {
   //variáveis locais
   TRamo *ramo;

   //loop p/ todos Ramos em lisRAMO
   for (int nr = lisRAMO->Count-1; nr >= 0; nr--)
      {
      ramo = (TRamo*)lisRAMO->Items[nr];
      if (ramo->bar_fim == barra) return(ramo);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TRamo* __fastcall TCaminho::ExisteRamoBarraFinal(TList *lisBAR_FIM)
   {
   //variáveis locais
   TRamo   *ramo;

   //loop p/ todos Ramos em lisRAMO
   for (int nr = lisRAMO->Count-1; nr >= 0; nr--)
      {
      ramo = (TRamo*)lisRAMO->Items[nr];
      //verifica se a Barra final do Ramo existe em lisBAR_FIM
      if (lisBAR_FIM->IndexOf(ramo->bar_fim) >= 0) return(ramo);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TRamo* __fastcall TCaminho::ExisteRamo(VTLigacao *ligacao_pai, VTLigacao *ligacao_filha)
   {
   //variáveis locais
   TRamo *ramo;
   VTLigacao *ligacao1, *ligacao2;

   //loop p/ todos Ramos em lisRAMO
   for (int nr = lisRAMO->Count-1; nr >= 0; nr--)
      {
      ramo = (TRamo*)lisRAMO->Items[nr];
      ligacao1 = ramo->ligacao;
      ligacao2 = (ramo->pai == NULL) ? NULL : ramo->pai->ligacao;
      if ((ligacao1 == ligacao_pai)&&(ligacao2 == ligacao_filha)) return(ramo);
      if ((ligacao2 == ligacao_pai)&&(ligacao1 == ligacao_filha)) return(ramo);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TCaminho::InsereLigacao(VTLigacao *ligacao, TList *lisEXT)
   {
   //proteção
   if (ligacao == NULL) return;
   //insere Ligacao na lista indicada
   if (lisEXT->IndexOf(ligacao) < 0) lisEXT->Add(ligacao);
   }

//---------------------------------------------------------------------------
void __fastcall TCaminho::InsereRamo(VTBarra *bar_ini, VTBarra *bar_fim, VTLigacao *ligacao, TRamo *pai)
   {
   //variáveis locais
   TRamo *ramo;

   try{//cria um novo Ramo e insere em lisEXT
      lisRAMO->Add(ramo = new TRamo());
      ramo->bar_ini = bar_ini;
      ramo->bar_fim = bar_fim;
      ramo->ligacao = ligacao;
      ramo->pai     = pai;
      }catch(Exception &e)
         {}
   }

//---------------------------------------------------------------------------
void __fastcall TCaminho::MontaLigacoesInterligadas(TList *lisEXT)
   {
   //variáveis locais
   TRamo *ramo;

   //loop p/ todos Ramos em lisSEL
   for (int nr = 0; nr < lisSEL->Count; nr++)
      {
      ramo = (TRamo*)lisSEL->Items[nr];
      //insere Ligacao do Ramo em lisEXT
      InsereLigacao(ramo->ligacao, lisEXT);
      }
  }

//---------------------------------------------------------------------------
void __fastcall TCaminho::MontaRamos(VTBarra *barra)
   {
   //variáveis locais
   TRamo   *ramo_pai;

   //reinicia listas lisRAMO
   LimpaTList(lisRAMO);
   //proteção: não monta os ramos para uma Barra final do caminho
   if (lisBAR_FIM->IndexOf(barra) >= 0) return;
   //cria Ramos p/ Barra inicial
   MontaRamosFilhos(NULL, barra);
   //loop p/ todos Ramos em lisRAMO
   for (int nr = 0; nr < lisRAMO->Count; nr++)
      {
      ramo_pai = (TRamo*)lisRAMO->Items[nr];
      //descarta Ramo pai cuja Barra final é igual a uma Barra final
      if (lisBAR_FIM->IndexOf(ramo_pai->bar_fim) >= 0) continue;
      //insere Ramos filhos que conectam a Barra final do Ramo pai
      MontaRamosFilhos(ramo_pai, ramo_pai->bar_fim);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCaminho::MontaRamosFilhos(TRamo *ramo_pai, VTBarra *bar_ini)
   {
   //variáveis locais
   VTBarra   *bar_fim;
   VTLigacao *ligacao;
   TRamo     *ramo;

   //determina Ligacoes que conectam Barra final do Ramo pai
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      //desconsidera Chaves abertas
      if (ChaveAberta(ligacao)) continue;
      //verifica se a Ligacao é a mesma do Ramo pai
      if ((ramo_pai != NULL)&&(ramo_pai->ligacao == ligacao)) continue;
      //verifica se a Ligacao conecta a Barra
      if (! ligacao->ConectaBarra(bar_ini)) continue;
      //cria novos Ramos p/ Barras conectadas pela Ligacao
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {//descarta Barra nula ou igual à Barra indicada
         if ((bar_fim = ligacao->Barra(ind_bar)) == NULL)  continue;
         if (bar_fim == bar_ini) continue;
         //proteção: verifica se já existe um Ramo igual ou complementar (pai/filho)
         if (ramo_pai == NULL) {if (ExisteRamo(NULL,              ligacao)) continue;}
         else                  {if (ExisteRamo(ramo_pai->ligacao, ligacao)) continue;}
         //cria novo Ramo c/ as Barras bar_ini  e bar_fim
         InsereRamo(bar_ini, bar_fim, ligacao, ramo_pai);
         }
      }
   }
//---------------------------------------------------------------------------
//eof

