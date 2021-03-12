//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TRede1.h"
#include ".\TBarra.h"
#include ".\TCapSerie.h"
#include ".\TGrupoLTC.h"
#include ".\TGrupoMutua.h"
#include ".\TLigacaoC.h"
#include ".\TReg2.h"
#include ".\TSup.h"
#include ".\TTrafo2.h"
#include ".\TTrafo3.h"
#include ".\TTrafoEquiv.h"
#include ".\TTrafoZZ.h"
#include ".\TTrecho.h"
#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\TMedidor.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Complexo\Complexo.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTRede1 * __fastcall NewObjRede1(VTGeralC *geralC)
   {
   return(new TRede1(geralC));
   }

//---------------------------------------------------------------------------
__fastcall TRede1::TRede1(VTGeralC *geralC)
   {
   // Salva ponteiro para objetos externos
   this->geralC = geralC;

   // Aloca memória
   lis2           = new TList;
   lisBAR         = new TList;
   lisBATERIA     = new TList;
   lisCAPS        = new TList;
   lisCARGA       = new TList;
   lisCNL         = new TList;
   lisFILTRO      = new TList;
   lisGRUPO_LTC   = new TList;
   lisGRUPO_MUTUA = new TList;
   lisMED         = new TList;
   lisMED_LIGADOS = new TList;
   lisREG2        = new TList;
   lisSUP         = new TList;
   lisTRAFO2      = new TList;
   lisTRAFO3      = new TList;
   lisTRAFOEQUIV  = new TList;
   lisTRAFOZZ     = new TList;
   lisTRECHO      = new TList;

   // Inicializa ID de barras e trechos
   Maior_ID_Barra  = 0;
   Maior_ID_Trecho = 0;

   // Define valor de constantes
   cum   = complex<double>(1., 0.);
   czero = complex<double>(0., 0.);
   zerov = complex<double>(0., -ZEROV);
   }

//---------------------------------------------------------------------------
__fastcall TRede1::~TRede1(void)
   {
   // Libera memória
   delete lis2; // Os elementos desta lista são os mesmos de 'lisTRAFO2' e 'lisREG2'
   DeletaLista(&lisBAR);
   DeletaLista(&lisBATERIA);
   DeletaLista(&lisCAPS);
   DeletaLista(&lisCARGA);
   DeletaLista(&lisCNL);
   DeletaLista(&lisFILTRO);
   DeletaLista(&lisGRUPO_LTC);
   DeletaLista(&lisGRUPO_MUTUA);
   DeletaLista(&lisMED);
   delete lisMED_LIGADOS;  // Os elementos desta lista são os mesmos de 'lisMED'
   DeletaLista(&lisREG2);
   DeletaLista(&lisSUP);
   DeletaLista(&lisTRAFO2);
   DeletaLista(&lisTRAFO3);
   DeletaLista(&lisTRAFOEQUIV);
   DeletaLista(&lisTRAFOZZ);
   DeletaLista(&lisTRECHO);
   }

//---------------------------------------------------------------------------
int __fastcall TRede1::AnalisaIDBarra(int id_sinap)
{
// Atualiza 'Maior_ID_Barra' SEMPRE
if(id_sinap > Maior_ID_Barra) Maior_ID_Barra = id_sinap;

// Retorna ID fornecido
return(id_sinap);
}

//---------------------------------------------------------------------------
int __fastcall TRede1::AnalisaIDTrecho(int id_sinap)
{
// Atualiza 'Maior_ID_Trecho' SEMPRE
if(id_sinap > Maior_ID_Trecho) Maior_ID_Trecho = id_sinap;

// Retorna ID fornecido
return(id_sinap);
}

//---------------------------------------------------------------------------
AnsiString __fastcall TRede1::AtribuiCodigoLigacao(AnsiString codigo,
                                                   TBarra     *bar1,
                                                   TBarra     *bar2,
                                                   TBarra     *bar3)
{
AnsiString codigo2 = " ";

if(! codigo.IsEmpty()) return(codigo.SubString(0, 15)); // Código original não nulo
if((bar1 != NULL) && (bar2 != NULL) && (bar3 != NULL))  // Trafo de 3 enrolamentos
   {
   AnsiString cod_i, cod_j, vet_codigo[3];
   int        jinic, jm1;
   vet_codigo[0] = bar1->codigo;
   vet_codigo[1] = bar2->codigo;
   vet_codigo[2] = bar3->codigo;
   for(int i=1; i < 3; i++)
      {
      cod_i = vet_codigo[i];
      jinic = i - 1;
      jm1   = i;
      for(int j=jinic; j >= 0; j--)
         {
         cod_j = vet_codigo[j];
         if(cod_i.AnsiCompareIC(cod_j) >= 0) break;
         vet_codigo[jm1] = cod_j;
         jm1             = j;
         }
      vet_codigo[jm1] = cod_i;
      }
   codigo2 = vet_codigo[0] + "-" + vet_codigo[1] + "-" + vet_codigo[2];
   }
else if((bar1 != NULL) && (bar2 != NULL)) // Trecho ou trafo de 2 enrolamentos
   {
   if(bar1->codigo.AnsiCompareIC(bar2->codigo) < 0)
      {
      codigo2 = bar1->codigo + "-" + bar2->codigo;
      }
   else
      {
      codigo2 = bar2->codigo + "-" + bar1->codigo;
      }
   }

return(codigo2.SubString(0, 15));
}

//---------------------------------------------------------------------------
int __fastcall TRede1::AtribuiIDBarra(void)
   {
   // Função usada no Estabilidade
   Maior_ID_Barra++;
   return(Maior_ID_Barra);
   }

//---------------------------------------------------------------------------
void __fastcall TRede1::AtribuiIDs(void)
{
TBarra  *bar1;
TTrecho *trec1;

// Acerta barras com ID inválido
for(int nb=0; nb < lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) lisBAR->Items[nb];
   if(bar1->id == ID_INVALIDO)
      {
      Maior_ID_Barra++;
      bar1->id = Maior_ID_Barra;
      }
   }

// Acerta trechos com ID inválido
for(int nt=0; nt < lisTRECHO->Count; nt++)
   {
   trec1 = (TTrecho *) lisTRECHO->Items[nt];
   if(trec1->id == ID_INVALIDO)
      {
      Maior_ID_Trecho++;
      trec1->id = Maior_ID_Trecho;
      }
   }
}

//---------------------------------------------------------------------------
int __fastcall TRede1::AtribuiIDTrecho(void)
   {
   // Função usada no Estabilidade
   Maior_ID_Trecho++;
   return(Maior_ID_Trecho);
   }

//---------------------------------------------------------------------------
void __fastcall TRede1::DeletaLista(TList **lista)
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
bool __fastcall TRede1::ExisteMedidorReal(TBarra *bar1,
                                          int    tipo_eqpto,
                                          int    tipo_medicao,
                                          int    id_calc)
{
// Variáveis locais
TMedidor *med1;

// Loop de medidores
for(int nm=0; nm < lisMED->Count; nm++)
   {
   med1 = (TMedidor *) lisMED->Items[nm];
   if(med1->tipo_medidor != medREAL)      break; // Não há mais medidores reais
   if(med1->bar_ref      != bar1)         continue;
   if(med1->tipo_eqpto   != tipo_eqpto)   continue;
   if(med1->tipo_medicao != tipo_medicao) continue;
   if(med1->id_calc      == id_calc)      return(true);
   }

return(false);
}

//---------------------------------------------------------------------------
void __fastcall TRede1::LimpaLista(TList *lista)
   {
   TObject *ptr;

   if(lista == NULL) return;

   for(int n=0; n < lista->Count; n++)
      {
      ptr = (TObject *) lista->Items[n];
      delete ptr;
      }
   lista->Clear();
   }

//---------------------------------------------------------------------------
TBarra * __fastcall TRede1::LocalizaBarraCodigo(AnsiString codigo,
                                                TList      *lisBAR_AUX)
   {
   TBarra *bar1;

   if(lisBAR_AUX == NULL) lisBAR_AUX = lisBAR;

   for(int n=0; n < lisBAR_AUX->Count; n++)
      {
      bar1 = (TBarra *) lisBAR_AUX->Items[n];
      if(codigo.AnsiCompareIC(bar1->codigo) == 0) return(bar1);
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
TBarra * __fastcall TRede1::LocalizaBarraEqpto(VTEqpto *eqpto)
   {
   TBarra *bar1;

   if(lisBAR == NULL) return(NULL);

   for(int n=0; n < lisBAR->Count; n++)
      {
      bar1 = (TBarra *) lisBAR->Items[n];
      if(eqpto == bar1->eqpto) return(bar1);
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
TBarra * __fastcall TRede1::LocalizaBarraId(int   barra_id,
                                            TList *lisBAR_AUX)
   {
   TBarra *bar1;

   if(lisBAR_AUX == NULL) lisBAR_AUX = lisBAR;

   for(int n=0; n < lisBAR_AUX->Count; n++)
      {
      bar1 = (TBarra *) lisBAR_AUX->Items[n];
      if(barra_id == bar1->id) return(bar1);
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
TBarra * __fastcall TRede1::LocalizaBarraOffset(int   offset,
                                                TList *lisBAR_AUX)
   {
   TBarra *bar1;

   if(lisBAR_AUX == NULL) lisBAR_AUX = lisBAR;

   for(int n=0; n < lisBAR_AUX->Count; n++)
      {
      bar1 = (TBarra *) lisBAR_AUX->Items[n];
      if(offset == bar1->offset) return(bar1);
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
TGrupoLTC * __fastcall TRede1::LocalizaGrupoLTC(TBarra *bar_ref)
   {
   TGrupoLTC *grupo1;

   for(int n=0; n < lisGRUPO_LTC->Count; n++)
      {
      grupo1 = (TGrupoLTC *) lisGRUPO_LTC->Items[n];
      if(bar_ref == grupo1->bar_ref) return(grupo1);
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
TLigacaoC * __fastcall TRede1::LocalizaLigacaoC(TBarra *bar1,
                                                void   *plig)
   {
   TLigacaoC *ligC;

   for(int nl=0; nl < bar1->lisLIG->Count; nl++)
      {
      ligC = (TLigacaoC *) bar1->lisLIG->Items[nl];
      if(ligC->plig == plig) return(ligC);
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
TMedidor * __fastcall TRede1::LocalizaMedidor(void *pvoid)
   {
   TMedidor *med1;

   if(lisMED == NULL) return(NULL);

   for(int n=0; n < lisMED->Count; n++)
      {
      med1 = (TMedidor *) lisMED->Items[n];
      if(pvoid == med1->eqpto) return(med1);
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
TSup * __fastcall TRede1::LocalizaSuprimentoBarra(TBarra *barra)
   {
   return(barra->sup);
   }

//---------------------------------------------------------------------------
TSup * __fastcall TRede1::LocalizaSuprimentoEqpto(VTEqpto *eqpto)
   {
   TSup *sup1;

   if(lisSUP == NULL) return(NULL);
   for(int n=0; n < lisSUP->Count; n++)
      {
      sup1 = (TSup *) lisSUP->Items[n];
      if(eqpto == sup1->eqpto) return(sup1);
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
TTrafo2 * __fastcall TRede1::LocalizaTrafo2Eqpto(VTEqpto *eqpto)
   {
   TTrafo2 *trafo2;

   if(lisTRAFO2 == NULL) return(NULL);

   for(int n=0; n < lisTRAFO2->Count; n++)
      {
      trafo2 = (TTrafo2 *) lisTRAFO2->Items[n];
      if(eqpto == trafo2->eqpto) return(trafo2);
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
TTrafo2 * __fastcall TRede1::LocalizaTrafo2Id(int trafo2_id)
   {
   TTrafo2 *trafo2;

   if(lisTRAFO2 == NULL) return(NULL);

   for(int n=0; n < lisTRAFO2->Count; n++)
      {
      trafo2 = (TTrafo2 *) lisTRAFO2->Items[n];
      if(trafo2_id == trafo2->id) return(trafo2);
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
TTrecho * __fastcall TRede1::LocalizaTrechoCodigo(AnsiString codigo,
                                                  TList      *lisTRECHO_AUX)
   {
   TTrecho *trec1;

   if(lisTRECHO_AUX == NULL) lisTRECHO_AUX = lisTRECHO;

   for(int n=0; n < lisTRECHO_AUX->Count; n++)
      {
      trec1 = (TTrecho *) lisTRECHO_AUX->Items[n];
      if(codigo.AnsiCompareIC(trec1->codigo) == 0) return(trec1);
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
TTrecho * __fastcall TRede1::LocalizaTrechoEqpto(VTEqpto *eqpto)
   {
   TTrecho *trec1;

   if(lisTRECHO == NULL) return(NULL);

   for(int n=0; n < lisTRECHO->Count; n++)
      {
      trec1 = (TTrecho *) lisTRECHO->Items[n];
      if(eqpto == trec1->eqpto) return(trec1);
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
TTrecho * __fastcall TRede1::LocalizaTrechoId(int   trecho_id,
                                              TList *lisTRECHO_AUX)
   {
   TTrecho *trec1;

   if(lisTRECHO_AUX == NULL) lisTRECHO_AUX = lisTRECHO;

   for(int n=0; n < lisTRECHO_AUX->Count; n++)
      {
      trec1 = (TTrecho *) lisTRECHO_AUX->Items[n];
      if(trecho_id == trec1->id) return(trec1);
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TRede1::TodosMedidoresReaisPotenciaBarra(void)
{
// Variáveis locais
TMedidor *med1;

// Loop de medidores reais
for(int nm=0; nm < geralC->num_med_reais; nm++)
   {
   med1 = (TMedidor *) lisMED->Items[nm];
   if(med1->tipo_eqpto != teBARRA)   return(false); // Medidor real não de barra
   if(! med1->TipoMedicaoPotencia()) return(false); // Medidor real não de potência
   }

// Todos os medidores reais são de potência em barra
return(true);
}

//---------------------------------------------------------------------------

