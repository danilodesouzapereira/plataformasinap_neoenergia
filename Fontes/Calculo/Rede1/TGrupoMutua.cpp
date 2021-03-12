//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TGrupoMutua.h"
#include ".\TBarra.h"
#include ".\TMutua.h"
#include ".\TTrecho.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\SisLin\VTSisLin.h"
#include "..\..\..\DLL_Inc\SisLin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TGrupoMutua::TGrupoMutua(void)
   {
   lisTRECHO = new TList;
   lisMUTUA  = new TList;
   Yelem0_pu = NULL;
   Yelem1_pu = NULL;
   cmeio     = complex<double>(0.5, 0.);
   }

//---------------------------------------------------------------------------
__fastcall TGrupoMutua::~TGrupoMutua(void)
   {
   delete lisTRECHO;  // Não deve deletar os elementos da lista (trechos)
   DeletaLista(&lisMUTUA);
   delete Yelem0_pu;
   delete Yelem1_pu;
   }

//---------------------------------------------------------------------------
void __fastcall TGrupoMutua::Calcula(int      np,
                                     VTGeralC *geralC)
{
// Variáveis locais
int             num_trec, ordem;
double          ibase;
double          raiz3 = sqrt(3.);
complex<double> corrente1, corrente2, perda, perda1, perda2, perda_total,
                v1, v2, ypar, yser;
complex<double> czero(0., 0.);
TBarra          *bar1, *bar2;
TTrecho         *trec1;

// Inicialização
perda_total = czero;

// Calcula correntes, potências e perdas
num_trec = lisTRECHO->Count;
for(int linha=0; linha < num_trec; linha++)
   {
   corrente1 = czero;
   for(int coluna=0; coluna < num_trec; coluna++)
      {
      ordem = Yelem1_pu->ElementoLC(linha, coluna, &yser);
      if(ordem < 0) continue;
      trec1 = (TTrecho *) lisTRECHO->Items[coluna];
      if(! trec1->flag_chave_fechada) continue; // Descarta trecho com chave aberta
      bar1       = trec1->bar1;
      bar2       = trec1->bar2;
      v1         = bar1->tensao_pu;
      v2         = bar2->tensao_pu;
      corrente1 += yser * (v1 - v2);
      }
   corrente2            = -corrente1;
   trec1                = (TTrecho *) lisTRECHO->Items[linha];
   trec1->vet_corr_a[0] = czero;
   trec1->vet_corr_a[1] = czero;
   trec1->vet_s_kva[0]  = czero;
   trec1->vet_s_kva[1]  = czero;
   trec1->perda_kva     = czero;
   if(! trec1->flag_chave_fechada) continue; // Descarta trecho com chave aberta
   bar1 = trec1->bar1;
   bar2 = trec1->bar2;
   v1   = bar1->tensao_pu;
   v2   = bar2->tensao_pu;
   if(geralC->flag_capac_LT)  // Considera capacitância das linhas
      {
      ypar       = cmeio * trec1->Y1cap_pu;
      corrente1 += ypar * v1;
      corrente2 += ypar * v2;
      }
   ibase                            = 1000. * geralC->Sbase / (raiz3 * bar1->vnom_kv); // A
   trec1->vet_corr_a[0]             = corrente1 * ibase;
   trec1->vet_corr_a[1]             = corrente2 * ibase;
   perda1                           = 1000. * v1 * conj(corrente1) * geralC->Sbase; // kVA
   perda2                           = 1000. * v2 * conj(corrente2) * geralC->Sbase; // kVA
   trec1->vet_s_kva[0]              = perda1;                                       // Pot. que entra na barra inicial
   trec1->vet_s_kva[1]              = perda2;                                       // Pot. que entra na barra final
   perda                            = perda1 + perda2;                              // Perda no trecho (kVA)
   trec1->perda_kva                 = perda;
   trec1->perda_energia_diaria_kwh += perda.real() * geralC->DuracaoPatamar(np);
   perda_total                     += perda;
   } // for(linha)

// Acumula perda em potência
geralC->perda_total_2 += perda_total;
}

//---------------------------------------------------------------------------
void __fastcall TGrupoMutua::DeletaLista(TList **lista)
   {
   TObject *ptr;

   if((*lista) == NULL) return;

   for(int n=0; n < (*lista)->Count; n++)
      {
      ptr = (TObject *) (*lista)->Items[n];
      delete ptr;
      }
   delete (*lista);
   (*lista) = NULL;
   }

//---------------------------------------------------------------------------
bool __fastcall TGrupoMutua::InsereYnodal(int      seq,
                                          VTGeralC *geralC,
                                          VTSisLin *Ynod,
                                          bool     flag_inclui_mutuas)
{
int             indp, indq, indr, inds, num_trec, ordem;
complex<double> y1;
TBarra          *bar1, *bar2, *bar3, *bar4;
TTrecho         *trec1, *trec2;
VTSisLin        *Yelem_pu;

// Insere matriz elementar
num_trec = lisTRECHO->Count;
if(seq == 0) Yelem_pu = Yelem0_pu;
else         Yelem_pu = Yelem1_pu;
// Loop dos trechos-1
for(int nt1=0; nt1 < num_trec; nt1++)
   {
   trec1 = (TTrecho *) lisTRECHO->Items[nt1];
   if(! trec1->flag_chave_fechada) continue; // Descarta trecho com chave aberta
   bar1 = trec1->bar1;
   bar2 = trec1->bar2;
   indp = bar1->linhaY;
   indq = bar2->linhaY;
   if((indp < 0) || (indq < 0)) continue;  // Descarta barras(s) desconexa(s)
   // Loop dos trechos-2
   for(int nt2=0; nt2 < num_trec; nt2++)
      {
      if(! flag_inclui_mutuas)
         {
         if(nt2 != nt1) continue; // Pula impedância mútua (verificação de conexidade)
         }
      ordem = Yelem_pu->ElementoLC(nt1, nt2, &y1);
      if(ordem < 0) continue;  // Elemento (nt1, nt2) não existe
      trec2 = (TTrecho *) lisTRECHO->Items[nt2];
      if(! trec2->flag_chave_fechada) continue; // Descarta trecho com chave aberta
      bar3 = trec2->bar1;
      bar4 = trec2->bar2;
      indr = bar3->linhaY;
      inds = bar4->linhaY;
      if((indr < 0) || (inds < 0)) continue;  // Descarta barras(s) desconexa(s)

      // Algoritmo "+pr +qs -ps -qr"
      Ynod->Insere(indp, indr, y1);
      Ynod->Insere(indq, inds, y1);
      Ynod->Insere(indp, inds, (-y1));
      Ynod->Insere(indq, indr, (-y1));
      }  // for(nt2)

   // Insere yparalelo, se o correspondente flag permitir
   if(geralC->flag_capac_LT)
      {
      if(seq == 0) y1 = cmeio * trec1->Y0cap_pu; // Metade da admitância total
      else         y1 = cmeio * trec1->Y1cap_pu; // Metade da admitância total
      Ynod->InsereDiagonal(indp, y1);
      Ynod->InsereDiagonal(indq, y1);
      }
   } // for(nt1)

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TGrupoMutua::MontaMatrizYelem(int    seq,
                                              double Sbase)
   {
   bool            retorno;
   int             coluna, ind1, ind2, linha;
   double          ybase;
   complex<double> zm_pu;
   TMutua          *mutua;
   TTrecho         *trec1, *trec2;
   VTSisLin        *Zelem_pu;

   // Inicialização
   retorno = true;

   // Destroi instância anterior da matriz a ser montada
   if(seq == 0) {delete Yelem0_pu; Yelem0_pu = NULL;}
   else         {delete Yelem1_pu; Yelem1_pu = NULL;}

   // Aloca memória para matriz Zelem
   Zelem_pu = DLL_NewObjSisLinC(0, lisTRECHO->Count, NULL, "Zelem_pu");

   // Insere elementos na diagonal de Zelem
   for(int nt=0; nt < lisTRECHO->Count; nt++)
      {
      trec1 = (TTrecho *) lisTRECHO->Items[nt];
      if(seq == 0) Zelem_pu->InsereDiagonal(nt, trec1->Z0ser_pu);
      else         Zelem_pu->InsereDiagonal(nt, trec1->Z1ser_pu);
      }

   // Insere elementos fora da diagonal de Zelem
   for(int nm=0; nm < lisMUTUA->Count; nm++)
      {
      mutua  = (TMutua *) lisMUTUA->Items[nm];
      linha  = lisTRECHO->IndexOf(mutua->trec1);
      coluna = lisTRECHO->IndexOf(mutua->trec2);
      ybase  = Sbase / (mutua->trec1->bar1->vnom_kv * mutua->trec1->bar1->vnom_kv);
      if(seq == 0) zm_pu = mutua->z0_ohm * ybase;
      else         zm_pu = mutua->z1_ohm * ybase;
      // Acerta sentido da mútua
      if(mutua->trec1->bar1 == mutua->bar1) ind1 = 0;
      else                                  ind1 = 1;
      if(mutua->trec2->bar1 == mutua->bar2) ind2 = 0;
      else                                  ind2 = 1;
      if(ind1 != ind2) zm_pu = -zm_pu;  // Sentido dos trechos trocado
      Zelem_pu->InsereSimetrico(linha, coluna, zm_pu);
      }

   // Inverte Zelem, obtendo Yelem
   if(seq == 0) Yelem0_pu = Zelem_pu->Inverte("Yelem0_pu");
   else         Yelem1_pu = Zelem_pu->Inverte("Yelem1_pu");

   // Finaliza
   if(Zelem_pu->Erro != slSemErro)                                       retorno = false;
   if(seq == 0) {if(Yelem0_pu != NULL) {if(Yelem0_pu->Erro != slSemErro) retorno = false;}}
   else         {if(Yelem1_pu != NULL) {if(Yelem1_pu->Erro != slSemErro) retorno = false;}}
   delete Zelem_pu;

   return(retorno);
   }

//---------------------------------------------------------------------------

