//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TTronco.h"
//#include ..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTCabo.h"
#include "..\Radial\VTPrimario.h"
#include "..\Radial\VTSecundario.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//método global p/ criar objetos VTTronco
//---------------------------------------------------------------------------
VTTronco* __fastcall NewObjTronco(VTApl *apl)
   {
   return(new TTronco(apl));
   }

//---------------------------------------------------------------------------
__fastcall TTronco::TTronco(VTApl *apl)
   {
   //salva pnteiros
   this->apl = apl;
   //cria objetos
   lisBARRA   = new TList();
   lisRAMAL   = new TList();
   lisTRONCO  = new TList();
   lisLIGACAO = new TList();
   lisTMP     = new TList();
   //inicia dados
   PD.i_ramal_limitada = false;
   PD.i_ramal_max_pu   = 1.;
   }

//---------------------------------------------------------------------------
__fastcall TTronco::~TTronco(void)
   {
   //destrói listas sem destruir objetos
   if (lisBARRA)   {delete lisBARRA;   lisBARRA   = NULL;}
   if (lisLIGACAO) {delete lisLIGACAO; lisLIGACAO = NULL;}
   if (lisRAMAL)   {delete lisRAMAL;   lisRAMAL   = NULL;}
   if (lisTRONCO)  {delete lisTRONCO;  lisTRONCO  = NULL;}
   if (lisTMP)     {delete lisTMP;     lisTMP     = NULL;}
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TTronco::ArranjoRamalBifasico(void)
   {
   return(ramal.bif.arranjo);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TTronco::ArranjoRamalMonofasico(void)
   {
   return(ramal.mon.arranjo);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TTronco::ArranjoRamalTrifasico(void)
   {
   return(ramal.tri.arranjo);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TTronco::ArranjoTroncoBifasico(void)
   {
   return(tronco.bif.arranjo);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TTronco::ArranjoTroncoMonofasico(void)
   {
   return(tronco.mon.arranjo);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TTronco::ArranjoTroncoTrifasico(void)
   {
   return(tronco.tri.arranjo);
   }

//---------------------------------------------------------------------------
VTCabo* __fastcall TTronco::CaboRamalBifasico(void)
   {
   return(ramal.bif.cabo);
   }

//---------------------------------------------------------------------------
VTCabo* __fastcall TTronco::CaboRamalMonofasico(void)
   {
   return(ramal.mon.cabo);
   }

//---------------------------------------------------------------------------
VTCabo* __fastcall TTronco::CaboRamalTrifasico(void)
   {
   return(ramal.tri.cabo);
   }

//---------------------------------------------------------------------------
VTCabo* __fastcall TTronco::CaboTroncoBifasico(void)
   {
   return(tronco.bif.cabo);
   }

//---------------------------------------------------------------------------
VTCabo* __fastcall TTronco::CaboTroncoMonofasico(void)
   {
   return(tronco.mon.cabo);
   }

//---------------------------------------------------------------------------
VTCabo* __fastcall TTronco::CaboTroncoTrifasico(void)
   {
   return(tronco.tri.cabo);
   }
   
//---------------------------------------------------------------------------
AnsiString __fastcall TTronco::CodigoCaboRamalBifasico(void)
   {
   if (ramal.bif.cabo    != NULL) return(ramal.bif.cabo->Codigo);
   if (ramal.bif.arranjo != NULL) return(ramal.bif.arranjo->Codigo);

   return(AnsiString(""));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTronco::CodigoCaboRamalMonofasico(void)
   {
   if (ramal.mon.cabo    != NULL) return(ramal.mon.cabo->Codigo);
   if (ramal.mon.arranjo != NULL) return(ramal.mon.arranjo->Codigo);

   return(AnsiString(""));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTronco::CodigoCaboRamalTrifasico(void)
   {
   if (ramal.tri.cabo    != NULL) return(ramal.tri.cabo->Codigo);
   if (ramal.tri.arranjo != NULL) return(ramal.tri.arranjo->Codigo);

   return(AnsiString(""));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTronco::CodigoCaboTroncoBifasico(void)
   {
   if(tronco.bif.cabo    != NULL) return(tronco.bif.cabo->Codigo);
   if(tronco.bif.arranjo != NULL) return(tronco.bif.arranjo->Codigo);

   return(AnsiString(""));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTronco::CodigoCaboTroncoMonofasico(void)
   {
   if  (tronco.mon.cabo    != NULL) return(tronco.mon.cabo->Codigo);
   if  (tronco.mon.arranjo != NULL) return(tronco.mon.arranjo->Codigo);

   return(AnsiString(""));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTronco::CodigoCaboTroncoTrifasico(void)
   {
   if (tronco.tri.cabo != NULL) return(tronco.tri.cabo->Codigo);
   if (tronco.tri.arranjo != NULL) return(tronco.tri.arranjo->Codigo);

   return(AnsiString(""));
   }

//---------------------------------------------------------------------------
int __fastcall TTronco::CompRamalBifasico(void)
   {
   return(ramal.bif.comp);
   }

//---------------------------------------------------------------------------
int __fastcall TTronco::CompRamalMonofasico(void)
   {
   return(ramal.mon.comp);
   }

//---------------------------------------------------------------------------
int __fastcall TTronco::CompRamalTrifasico(void)
   {
   return(ramal.tri.comp);
   }

//---------------------------------------------------------------------------
int __fastcall TTronco::CompTroncoBifasico(void)
   {
   return(tronco.bif.comp);
   }

//---------------------------------------------------------------------------
int __fastcall TTronco::CompTroncoMonofasico(void)
   {
   return(tronco.mon.comp);
   }

//---------------------------------------------------------------------------
int __fastcall TTronco::CompTroncoTrifasico(void)
   {
   return(tronco.tri.comp);
   }

//---------------------------------------------------------------------------
bool __fastcall TTronco::DefinePrimario(VTPrimario *primario, int ind_pat)
   {
   //salva patamar
   this->ind_pat  = ind_pat;
   //reinicia dados c/ valores nulos
   ramal.mon.cabo  = NULL; ramal.mon.comp  = 0;
   ramal.bif.cabo  = NULL; ramal.bif.comp  = 0;
   ramal.tri.cabo  = NULL; ramal.tri.comp  = 0;
   tronco.mon.cabo = NULL; tronco.mon.comp = 0;
   tronco.bif.cabo = NULL; tronco.bif.comp = 0;
   tronco.tri.cabo = NULL; tronco.tri.comp = 0;
   //proteção
   if ((primario == NULL)||(primario->BarraInicial == NULL)) return(false);
   //monta listas de Trechos do tronco e dos ramais
   if (! IdentificaTroncoRamal(primario)) return(false);
   //determina dados de tronco
   IniciaDadosTroncoMonofasico();
   IniciaDadosTroncoBifasico();
   IniciaDadosTroncoTrifasico();
   IniciaDadosRamalMonofasico();
   IniciaDadosRamalBifasico();
   IniciaDadosRamalTrifasico();
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TTronco::DefineSecundario(VTSecundario *secundario, int ind_pat)
   {
   //salva patamar
   this->ind_pat    = ind_pat;
   //reinicia dados c/ valores nulos
   ramal.mon.cabo  = NULL; ramal.mon.comp  = 0;
   ramal.bif.cabo  = NULL; ramal.bif.comp  = 0;
   ramal.tri.cabo  = NULL; ramal.tri.comp  = 0;
   tronco.mon.cabo = NULL; tronco.mon.comp = 0;
   tronco.bif.cabo = NULL; tronco.bif.comp = 0;
   tronco.tri.cabo = NULL; tronco.tri.comp = 0;
   //proteção
   if ((secundario == NULL)||(secundario->BarraInicial == NULL)) return(false);
   //monta listas de Trechos do tronco e dos ramais
   if (! IdentificaTroncoRamal(secundario)) return(false);
   //determina dados de tronco
   IniciaDadosTroncoMonofasico();
   IniciaDadosTroncoBifasico();
   IniciaDadosTroncoTrifasico();
   IniciaDadosRamalMonofasico();
   IniciaDadosRamalBifasico();
   IniciaDadosRamalTrifasico();
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TTronco::ExisteLigacaoQueConectaBarra(VTBarra *barra, TList *lisORIG, TList *lisDEST)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //reinicia lisDEST
	lisDEST->Clear();
   for (int nl = lisORIG->Count-1; nl >=0; nl--)
      {
		ligacao = (VTLigacao*)lisORIG->Items[nl];
		if(ligacao->Tipo() == eqptoCHAVE)
         {
			if(((VTChave*)ligacao)->Aberta) continue;
			}
      if (ligacao->ConectaBarra(barra))
         {//transfere Ligacao de lisORIG p/ lisDEST
         lisORIG->Delete(nl);
         lisDEST->Add(ligacao);
         }
      }
   return(lisDEST->Count > 0);
   }

//---------------------------------------------------------------------------
VTLigacao* __fastcall TTronco::ExisteLigacaoTronco(TList *lisLIG)
   {
   //variáveis locais
   int       ind_bar = 0;
   double    cor_tronco, cor_ligacao;
   VTLigacao *ligacao, *lig_tronco;

   //proteção
   if (lisLIG->Count == 0) return(NULL);
   //assume que a primeira Ligacao é o tronco
   lig_tronco = (VTLigacao*)lisLIG->Items[0];
   cor_tronco = lig_tronco->resflow->If_amp_max[ind_bar][ind_pat];
   //procura em lisLIG a Ligacao de maior corrente
   for (int nl = 1; nl < lisLIG->Count; nl++)
      {
		ligacao  = (VTLigacao*)lisLIG->Items[nl];
      cor_ligacao = ligacao->resflow->If_amp_max[ind_bar][ind_pat];
      if (cor_ligacao > cor_tronco)
         {//redefine Trecho tronco
         lig_tronco  = ligacao;
         cor_tronco  = cor_ligacao;
         }
      }
   //verifica se a Ligacao selecionada deve ser considerada como tronco ou não
   if (! ValidaLigacaoTronco(lig_tronco)) lig_tronco = NULL;
   return(lig_tronco);
   }

//---------------------------------------------------------------------------
bool __fastcall TTronco::IdentificaTroncoRamal(VTPrimario *primario)
   {
   //variáveis locais
   VTBarra   *barra, *bar_lig;
   VTLigacao *ligacao;

   //cancela Ligacao inicial de ramal de maior corrente
   lig_inicial    = NULL;
   lig_ramal_imax = NULL;
   //reinicia listas
   lisBARRA->Clear();
   lisLIGACAO->Clear();
   lisRAMAL->Clear();
   lisTRONCO->Clear();
   //determina Ligacao inicial
   if ((lig_inicial = primario->LigacaoInicial) == NULL)  return(false);
   //inicia lisLIGACAO c/ as Ligacoes do Primario
   CopiaTList(primario->Rede->LisLigacao(), lisLIGACAO);
   //inicia lisBARRA c/ a Barra inicial do Primarioa
   lisBARRA->Add(primario->BarraInicial);
   //proteção
   if (lisBARRA->Count == 0)   return(false);
   if (lisLIGACAO->Count == 0) return(false);
   //loop p/ todas Barras em lisBARRA
   for (int nb = 0; nb < lisBARRA->Count; nb++)
      {
      barra = (VTBarra*)lisBARRA->Items[nb];
      //determina conjunto de Ligacoes conectadas à Barra
      if (! ExisteLigacaoQueConectaBarra(barra, lisLIGACAO, lisTMP)) continue;
      //retira de lisTMP a Ligacao tronco
      if ((ligacao = ExisteLigacaoTronco(lisTMP)) != NULL)
         {//transfere Ligacao tronco de lisTMP p/ lisTRONCO
         lisTMP->Remove(ligacao);
         lisTRONCO->Add(ligacao);
         //insere as Barras conectadas pela Ligacao tronco em lisBARRA
         for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
            {
            if ((bar_lig = (VTBarra*)ligacao->Barra(ind_bar)) == NULL) continue;
            if (bar_lig == barra) continue;
            if (lisBARRA->IndexOf(bar_lig) < 0) lisBARRA->Add(bar_lig);
            }
         }
      //salva demais Ligacoes de lisTMP em lisRAMAL
      SalvaLigacaoRamal(lisTMP);
      }
   //as Ligacoes restantes em lisLIGACAO são ramais
   CopiaTList(lisLIGACAO, lisRAMAL);
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TTronco::IdentificaTroncoRamal(VTSecundario *secundario)
   {
   //variáveis locais
   VTBarra   *barra, *bar_lig;
   VTLigacao *ligacao;

   //cancela Ligacao inicial de ramal de maior corrente
   lig_inicial    = NULL;
   lig_ramal_imax = NULL;
   //reinicia listas
   lisBARRA->Clear();
   lisLIGACAO->Clear();
   lisRAMAL->Clear();
   lisTRONCO->Clear();
   //inicia lisLIGACAO c/ as Ligacoes do Secundario
   CopiaTList(secundario->Rede->LisLigacao(), lisLIGACAO);
   //inicia lisBARRA c/ a Barra inicial do Secundario
   lisBARRA->Add(secundario->BarraInicial);
   //proteção
   if (lisBARRA->Count == 0)   return(false);
   if (lisLIGACAO->Count == 0) return(false);
   //determina conjunto de Ligacoes conectadas à Barra inicial (trafos da Secundária)
   barra = secundario->BarraInicial;
   if (ExisteLigacaoQueConectaBarra(secundario->BarraInicial, lisLIGACAO, lisTMP))
      {//transfere todas as ligações p/ o Tronco
      for (int nl = 0; nl < lisTMP->Count; nl++)
         {
         ligacao = (VTLigacao*)lisTMP->Items[nl];
         lisTRONCO->Add(ligacao);
         //insere as Barras conectadas pela Ligacao tronco em lisBARRA
         for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
            {
            if ((bar_lig = (VTBarra*)ligacao->Barra(ind_bar)) == NULL) continue;
            if (bar_lig == barra) continue;
            if (lisBARRA->IndexOf(bar_lig) < 0) lisBARRA->Add(bar_lig);
            }
         }
      }
   //loop p/ todas Barras em lisBARRA (a partir de 1, pois a primeira é a barra inicial já analisada)
   for (int nb = 1; nb < lisBARRA->Count; nb++)
      {
      barra = (VTBarra*)lisBARRA->Items[nb];
      //determina conjunto de Ligacoes conectadas à Barra
      if (! ExisteLigacaoQueConectaBarra(barra, lisLIGACAO, lisTMP)) continue;
      //retira de lisTMP a Ligacao tronco
      if ((ligacao = ExisteLigacaoTronco(lisTMP)) != NULL)
         {//transfere Ligacao tronco de lisTMP p/ lisTRONCO
         lisTMP->Remove(ligacao);
         lisTRONCO->Add(ligacao);
         //insere as Barras conectadas pela Ligacao tronco em lisBARRA
         for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
            {
            if ((bar_lig = (VTBarra*)ligacao->Barra(ind_bar)) == NULL) continue;
            if (bar_lig == barra) continue;
            if (lisBARRA->IndexOf(bar_lig) < 0) lisBARRA->Add(bar_lig);
            }
         }
      //salva demais Ligacoes de lisTMP em lisRAMAL
      SalvaLigacaoRamal(lisTMP);
      }
   //as Ligacoes restantes em lisLIGACAO são ramais
   CopiaTList(lisLIGACAO, lisRAMAL);
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TTronco::IniciaDadosRamalBifasico(void)
   {
   //monta lista somente c/ os Trechos bifásicos
   SelecionaTrecho(2, lisRAMAL, lisTMP);
   //determina comprimento dos Trechos
   ramal.bif.comp = SomaComprimento(lisTMP);
   //determina arranjo ramal
   ramal.bif.arranjo = SelecionaArranjoDeMaiorCorrente(lisTMP);
   //determina cabo ramal
   ramal.bif.cabo = SelecionaCaboDeMaiorCorrente(ramal.bif.arranjo);
   }

//---------------------------------------------------------------------------
void __fastcall TTronco::IniciaDadosRamalMonofasico(void)
   {
   //monta lista somente c/ os Trechos monofásicos
   SelecionaTrecho(1, lisRAMAL, lisTMP);
   //determina comprimento dos Trechos
   ramal.mon.comp = SomaComprimento(lisTMP);
   //determina arranjo ramal
   ramal.mon.arranjo = SelecionaArranjoDeMaiorCorrente(lisTMP);
   //determina cabo ramal
   ramal.mon.cabo = SelecionaCaboDeMaiorCorrente(ramal.mon.arranjo);
   }

//---------------------------------------------------------------------------
void __fastcall TTronco::IniciaDadosRamalTrifasico(void)
   {
   //monta lista somente c/ os Trechos trifásicos
   SelecionaTrecho(3, lisRAMAL, lisTMP);
   //determina comprimento dos Trechos
   ramal.tri.comp = SomaComprimento(lisTMP);
   //determina arranjo ramal
   ramal.tri.arranjo = SelecionaArranjoDeMaiorCorrente(lisTMP);
   //determina cabo ramal
   ramal.tri.cabo = SelecionaCaboDeMaiorCorrente(ramal.tri.arranjo);
   }

//---------------------------------------------------------------------------
void __fastcall TTronco::IniciaDadosTroncoBifasico(void)
   {
   //monta lista somente c/ os Trechos bifásicos
   SelecionaTrecho(2, lisTRONCO, lisTMP);
   //determina comprimento dos Trechos
   tronco.bif.comp = SomaComprimento(lisTMP);
   //determina arranjo tronco
   tronco.bif.arranjo = SelecionaArranjoDeMaiorCorrente(lisTMP);
   //determina cabo tronco
   tronco.bif.cabo = SelecionaCaboDeMaiorCorrente(tronco.bif.arranjo);
   }

//---------------------------------------------------------------------------
void __fastcall TTronco::IniciaDadosTroncoMonofasico(void)
   {
    //monta lista somente c/ os Trechos monofásicos
   SelecionaTrecho(1, lisTRONCO, lisTMP);
   //determina comprimento dos Trechos
   tronco.mon.comp = SomaComprimento(lisTMP);
   //determina arranjo tronco
   tronco.mon.arranjo = SelecionaArranjoDeMaiorCorrente(lisTMP);
   //determina cabo tronco
   tronco.mon.cabo = SelecionaCaboDeMaiorCorrente(tronco.mon.arranjo);
   }

//---------------------------------------------------------------------------
void __fastcall TTronco::IniciaDadosTroncoTrifasico(void)
   {
   //monta lista somente c/ os Trechos trifásicos
   SelecionaTrecho(3, lisTRONCO, lisTMP);
   //determina comprimento dos Trechos
   tronco.tri.comp = SomaComprimento(lisTMP);
   //determina arranjo tronco
   tronco.tri.arranjo = SelecionaArranjoDeMaiorCorrente(lisTMP);
   //determina cabo tronco
   tronco.tri.cabo = SelecionaCaboDeMaiorCorrente(tronco.tri.arranjo);
   }
//---------------------------------------------------------------------------
TList* __fastcall TTronco::LisBarraTronco(void)
   {
   return(lisBARRA);
   }

//---------------------------------------------------------------------------
TList* __fastcall TTronco::LisLigacaoRamal(void)
    {
   return(lisRAMAL);
   }

//---------------------------------------------------------------------------
TList* __fastcall TTronco::LisLigacaoTronco(void)
   {
   return(lisTRONCO);
   }

//---------------------------------------------------------------------------
double __fastcall TTronco::MaiorTensaoTronco(int ind_pat)
   {
   //variáveis locais
   double  v_pu, v_pu_max;
   VTBarra *barra;

   //proteção
   if (lisBARRA->Count == 0) return(0.);
   //assume que a maior tensão é da primeira Barra em lisBAR
   barra    = (VTBarra*)lisBARRA->First();
   v_pu_max = barra->resflow->Vfn_pu_min[ind_pat];
   //loop p/ as demais Barras
   for (int nb = 0; nb < lisBARRA->Count; nb++)
      {
       barra = (VTBarra*)lisBARRA->Items[nb];
       v_pu  = barra->resflow->Vfn_pu_min[ind_pat];
       if (v_pu > v_pu_max) v_pu_max = v_pu;
      }
   return(v_pu_max);
   }

//---------------------------------------------------------------------------
double __fastcall TTronco::MenorTensaoTronco(int ind_pat)
   {
   //variáveis locais
   double  v_pu, v_pu_min;
   VTBarra *barra;

   //proteção
   if (lisBARRA->Count == 0) return(0.);
   //assume que a menor tensão é da primeira Barra em lisBAR
   barra    = (VTBarra*)lisBARRA->First();
   v_pu_min = barra->resflow->Vfn_pu_min[ind_pat];
   //loop p/ as demais Barras
   for (int nb = 0; nb < lisBARRA->Count; nb++)
      {
       barra = (VTBarra*)lisBARRA->Items[nb];
       v_pu  = barra->resflow->Vfn_pu_min[ind_pat];
       if (v_pu < v_pu_min) v_pu_min = v_pu;
      }
   return(v_pu_min);
   }

//---------------------------------------------------------------------------
int __fastcall TTronco::NumeroFases(VTArranjo *arranjo)
   {
   //variáveis locais
   int num_fase = 0;

   if ((arranjo->Fases & faseA) == faseA) num_fase++;
   if ((arranjo->Fases & faseB) == faseB) num_fase++;
   if ((arranjo->Fases & faseC) == faseC) num_fase++;

   return(num_fase);
   }

//---------------------------------------------------------------------------
void __fastcall TTronco::SalvaLigacaoRamal(TList *lisLIGACAO)
   {
   //variáveis locais
   int        ind_bar = 0;
   VTLigacao *ligacao;
   double     cor_ramal, cor_ramal_max;

   //determina máxima de corrente do Ramal
   cor_ramal_max = -1;
   if (lig_ramal_imax) cor_ramal_max = lig_ramal_imax->resflow->If_amp_max[ind_bar][ind_pat];
   //loop p/ todas Ligacoes
   for (int nl = 0; nl < lisLIGACAO->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIGACAO->Items[nl];
      //insere Ligacao em lisRAMAL
      lisRAMAL->Add(ligacao);
      //determina corrente da Ligacao
      cor_ramal = ligacao->resflow->If_amp_max[ind_bar][ind_pat];
      //verifica se foi definido limite máximo p/ corrente de ramal
      if ((CorrenteRamalLimitada)&&(lig_inicial != NULL))
         {//verifica se a corrente da Ligacao é maior que o limite definido
         if (cor_ramal > (CorrenteRamalMax_pu * lig_inicial->resflow->If_amp_max[ind_bar][ind_pat])) continue;
         }
      //verifica se a Ligacao é o ramal de maior corrente
      if (lig_ramal_imax == NULL) lig_ramal_imax = ligacao;
      if (cor_ramal > cor_ramal_max)
         {//salva ramal de corrente máxima
         lig_ramal_imax = ligacao;
         cor_ramal_max  = cor_ramal;
         }
      }
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TTronco::SelecionaArranjoDeMaiorCorrente(TList *lisTRE)
   {
   //variáveis locais
   VTTrecho  *trecho;
   VTArranjo *arranjo, *arranjo_sel = NULL;

   //seleciona Cabo de maior capacidade
   for (int n = 0; n < lisTRE->Count; n++)
      {
      trecho  = (VTTrecho*)lisTRE->Items[n];
      arranjo = trecho->arranjo;
      if (arranjo_sel == NULL) arranjo_sel = arranjo;
      //verifica qual arranjo possui maior corrente admissível
      if (arranjo->Iadm_amp > arranjo_sel->Iadm_amp)
         {//salva arranjo de maior corrente
         arranjo_sel = arranjo;
         }
      }
   return(arranjo_sel);
   }


//---------------------------------------------------------------------------
void __fastcall TTronco::SelecionaTrecho(int num_fase, TList *lisORIG, TList *lisDEST)
   {
   //variáveis locais
   VTLigacao *ligacao;
   VTTrecho  *trecho;

   //limpa lisDEST
   lisDEST->Clear();
   //copia Trechos c/ número de fases especificado de lisORIG p/ lisDEST
   for (int nl = 0; nl < lisORIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisORIG->Items[nl];
      if (ligacao->Tipo() != eqptoTRECHO) continue;
      trecho = (VTTrecho*)ligacao;
      if (NumeroFases(trecho->arranjo) == num_fase) lisDEST->Add(trecho);
      }
   }

//---------------------------------------------------------------------------
VTCabo* __fastcall TTronco::SelecionaCaboDeMaiorCorrente(VTArranjo *arranjo)
   {
   //variáveis locais
   VTCabo   *cabo, *cabo_sel = NULL;

   //proteção
   if (arranjo == NULL) return(NULL);
   //verifica o tipo de arranjo
   if (arranjo->TipoCfg == arranjoCABOxFASE)
      {
      for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
         {
         if ((cabo = arranjo->fase[ind_fase].cabo) == NULL) continue;
         if (cabo_sel == NULL) cabo_sel = cabo;
         if (cabo->Iadm_A > cabo_sel->Iadm_A) cabo_sel = cabo;
         }
      }
   else if (arranjo->TipoCfg == arranjoCABO_Z0Z1)
      {
      if ((cabo = arranjo->caboZ0Z1) != NULL)
         {
         if (cabo_sel == NULL) cabo_sel = cabo;
         if (cabo->Iadm_A > cabo_sel->Iadm_A) cabo_sel = cabo;
         }
      }
   return(cabo_sel);
   }

//---------------------------------------------------------------------------
double __fastcall TTronco::SomaComprimento(TList *lisTRE)
   {
   //variáveis locais
   double    comp_total = 0;
   VTTrecho *trecho;

   //soma comprimento dos Trechos
   for (int n = 0; n < lisTRE->Count; n++)
      {
      trecho      = (VTTrecho*)lisTRE->Items[n];
      comp_total += trecho->Comprimento_m;
      }
   return(comp_total);
   }

//---------------------------------------------------------------------------
bool __fastcall TTronco::ValidaLigacaoTronco(VTLigacao *ligacao)
   {
   //variáveis locais
   int    ind_bar = 0;
   double cor_ligacao, cor_max;

   //proteção
   if (ligacao == NULL) return(false);
   //;verifica se já existe a Ligacao ramal de maior corrente
   if (lig_ramal_imax == NULL) return(true);
   //verifica se a corrente da Ligacao é maior que a corrente do ramal de maior corrente
   cor_max      = lig_ramal_imax->resflow->If_amp_max[ind_bar][ind_pat];
   cor_ligacao  = ligacao->resflow->If_amp_max[ind_bar][ind_pat];
   return(cor_ligacao > cor_max);
   }

//---------------------------------------------------------------------------
//eof