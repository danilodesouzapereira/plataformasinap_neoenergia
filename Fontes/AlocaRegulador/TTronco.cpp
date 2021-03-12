//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Arranjo\VTArranjo.h>
#include <Fontes\Arranjo\VTCabo.h>
#include <Fontes\Radial\VTPrimario.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\Rede\VTLigacao.h>
#include <Fontes\Rede\VTRede.h>
#include <Fontes\Rede\VTTrecho.h>
#include <Fontes\Rede\VTResFlowBar.h>
#include <Fontes\Rede\VTResFlowLig.h>
#include <DLL_Inc\Funcao.h>
#include "TTronco.h"
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
   //salva ponteiro p/ objetos
   this->apl = apl;
   //ordena    = (VTOrdena*)apl->GetObject(__classid(VTOrdena));
   //cria objetos
   lisBARRA  = new TList();
   lisRAMAL  = new TList();
   lisTRONCO = new TList();
   lisLIGACAO    = new TList();
   lisTMP    = new TList();
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
AnsiString __fastcall TTronco::CaboRamalBifasico(void)
   {
   return(ramal.bif.cabo);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTronco::CaboRamalMonofasico(void)
   {
   return(ramal.mon.cabo);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTronco::CaboRamalTrifasico(void)
   {
   return(ramal.tri.cabo);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTronco::CaboTroncoBifasico(void)
   {
   return(tronco.bif.cabo);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTronco::CaboTroncoMonofasico(void)
   {
   return(tronco.mon.cabo);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TTronco::CaboTroncoTrifasico(void)
   {
   return(tronco.tri.cabo);
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
void __fastcall TTronco::DefinePrimario(VTPrimario *primario, int ind_pat)
   {
   //salva dados
   this->primario = primario;
   this->ind_pat  = ind_pat;
   //reinicia dados c/ valores nulos
   ramal.mon.cabo  = ""; ramal.mon.comp  = 0;
   ramal.bif.cabo  = ""; ramal.bif.comp  = 0;
   ramal.tri.cabo  = ""; ramal.tri.comp  = 0;
   tronco.mon.cabo = ""; tronco.mon.comp = 0;
   tronco.bif.cabo = ""; tronco.bif.comp = 0;
   tronco.tri.cabo = ""; tronco.tri.comp = 0;
   //ordena o Primario
   //if (! ordena->Executa(primario->Rede)) return;
   //monta listas de Trechos do tronco e dos ramais
   IdentificaTroncoRamal();
   //determina dados de tronco
   IniciaDadosTroncoMonofasico();
   IniciaDadosTroncoBifasico();
   IniciaDadosTroncoTrifasico();
   IniciaDadosRamalMonofasico();
   IniciaDadosRamalBifasico();
   IniciaDadosRamalTrifasico();
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
void __fastcall TTronco::IdentificaTroncoRamal(void)
   {
   //variáveis locais
   VTBarra   *barra, *bar_lig;
   VTLigacao *ligacao;

   //cancela Ligacao ramal de maior corrente
   lig_ramal_imax = NULL;
   //reinicia listas
   lisBARRA->Clear();
   lisLIGACAO->Clear();
   lisRAMAL->Clear();
   lisTRONCO->Clear();
   //inicia lisLIGACAO c/ as Ligacoes do Primario
   CopiaTList(primario->Rede->LisLigacao(), lisLIGACAO);
   //inicia lisBARRA c/ a Barra inicial do Primarioa
   lisBARRA->Add(primario->BarraInicial);
   //proteção
   if (lisBARRA->Count == 0)   return;
   if (lisLIGACAO->Count == 0) return;
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
   }

//---------------------------------------------------------------------------
void __fastcall TTronco::IniciaDadosRamalBifasico(void)
   {
   //variáveis locais
   VTCabo *cabo;

   //monta lista somente c/ os Trechos bifásicos
   SelecionaTrecho(2, lisRAMAL, lisTMP);
   //determina comprimento dos Trechos
   ramal.bif.comp = SomaComprimento(lisTMP);
   //determina cabo ramal
   cabo = SelecionaCaboDeMaiorCorrente(lisTMP);
   if (cabo) ramal.bif.cabo = cabo->Codigo;
   }

//---------------------------------------------------------------------------
void __fastcall TTronco::IniciaDadosRamalMonofasico(void)
   {
   //variáveis locais
   VTCabo *cabo;

   //monta lista somente c/ os Trechos monofásicos
   SelecionaTrecho(1, lisRAMAL, lisTMP);
   //determina comprimento dos Trechos
   ramal.mon.comp = SomaComprimento(lisTMP);
   //determina cabo ramal
   cabo = SelecionaCaboDeMaiorCorrente(lisTMP);
   if (cabo) ramal.mon.cabo = cabo->Codigo;
   }

//---------------------------------------------------------------------------
void __fastcall TTronco::IniciaDadosRamalTrifasico(void)
   {
   //variáveis locais
   VTCabo *cabo;

   //monta lista somente c/ os Trechos trifásicos
   SelecionaTrecho(3, lisRAMAL, lisTMP);
   //determina comprimento dos Trechos
   ramal.tri.comp = SomaComprimento(lisTMP);
   //determina cabo ramal
   cabo = SelecionaCaboDeMaiorCorrente(lisTMP);
   if (cabo) ramal.tri.cabo = cabo->Codigo;
   }

//---------------------------------------------------------------------------
void __fastcall TTronco::IniciaDadosTroncoBifasico(void)
   {
   //variáveis locais
   VTCabo *cabo;

   //monta lista somente c/ os Trechos bifásicos
   SelecionaTrecho(2, lisTRONCO, lisTMP);
   //determina comprimento dos Trechos
   tronco.bif.comp = SomaComprimento(lisTMP);
   //determina cabo tronco
   cabo = SelecionaCaboDeMaiorCorrente(lisTMP);
   if (cabo) tronco.bif.cabo = cabo->Codigo;
   }

//---------------------------------------------------------------------------
void __fastcall TTronco::IniciaDadosTroncoMonofasico(void)
   {
   //variáveis locais
   VTCabo *cabo;

   //monta lista somente c/ os Trechos monofásicos
   SelecionaTrecho(1, lisTRONCO, lisTMP);
   //determina comprimento dos Trechos
   tronco.mon.comp = SomaComprimento(lisTMP);
   //determina cabo tronco
   cabo = SelecionaCaboDeMaiorCorrente(lisTMP);
   if (cabo) tronco.mon.cabo = cabo->Codigo;
   }

//---------------------------------------------------------------------------
void __fastcall TTronco::IniciaDadosTroncoTrifasico(void)
   {
   //variáveis locais
   VTCabo *cabo;

   //monta lista somente c/ os Trechos trifásicos
   SelecionaTrecho(3, lisTRONCO, lisTMP);
   //determina comprimento dos Trechos
   tronco.tri.comp = SomaComprimento(lisTMP);
   //determina cabo tronco
   cabo = SelecionaCaboDeMaiorCorrente(lisTMP);
   if (cabo) tronco.tri.cabo = cabo->Codigo;
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
   double     cor, cor_max_ramal;

   //determina máxima de corrente do Ramal
   cor_max_ramal = -1;
   if (lig_ramal_imax) cor_max_ramal = lig_ramal_imax->resflow->If_amp_max[ind_bar][ind_pat];
   //loop p/ todas Ligacoes
   for (int nl = 0; nl < lisLIGACAO->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIGACAO->Items[nl];
      //insere Ligacao em lisRAMAL
      lisRAMAL->Add(ligacao);
      //verifica se a Ligacao é o ramal de maior corrente
      if (lig_ramal_imax == NULL) lig_ramal_imax = ligacao;
      cor = ligacao->resflow->If_amp_max[ind_bar][ind_pat];
      if (cor > cor_max_ramal)
         {//salva ramal de corrente máxima
         lig_ramal_imax = ligacao;
         cor_max_ramal  = cor;
         }
      }
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
VTCabo* __fastcall TTronco::SelecionaCaboDeMaiorCorrente(TList *lisTRE)
   {
   //variáveis locais
   VTTrecho *trecho;
   VTCabo   *cabo, *cabo_sel = NULL;

   //seleciona Cabo de maior capacidade
   for (int n = 0; n < lisTRE->Count; n++)
      {
      trecho = (VTTrecho*)lisTRE->Items[n];
      if (trecho->arranjo->TipoCfg == arranjoCABOxFASE)
         {
         for (int ind_fase = indFASE_A; ind_fase <= indFASE_C; ind_fase++)
            {
            if ((cabo = trecho->arranjo->fase[ind_fase].cabo) == NULL) continue;
            if (cabo_sel == NULL) cabo_sel = cabo;
            if (cabo->Iadm_A > cabo_sel->Iadm_A) cabo_sel = cabo;
            }
         }
      else if (trecho->arranjo->TipoCfg == arranjoCABO_Z0Z1)
         {
         if ((cabo = trecho->arranjo->caboZ0Z1) != NULL)
            {
            if (cabo_sel == NULL) cabo_sel = cabo;
            if (cabo->Iadm_A > cabo_sel->Iadm_A) cabo_sel = cabo;
            }
         }
      }
   return(cabo_sel);
   }

//---------------------------------------------------------------------------
double __fastcall TTronco::SomaComprimento(TList *lisTRE)
   {
   //variáveis locais
   int     comp_total = 0;
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