//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TRede3.h"
#include ".\TBarra.h"
#include ".\TCapSerie.h"
#include ".\TFase.h"
#include ".\TGrupoLTC12.h"
#include ".\TGrupoLTC32.h"
#include ".\TGrupoMutua.h"
#include ".\TLigacaoC.h"
#include ".\TReg12.h"
#include ".\TReg32.h"
#include ".\TSup.h"
#include ".\TTrafo12.h"
#include ".\TTrafo32.h"
#include ".\TTrafo33.h"
#include ".\TTrafoEquiv.h"
#include ".\TTrafoZZ.h"
#include ".\TTrecho.h"

#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\TMedidor.h"
#include "..\Comum13\VTGeralC.h"

#include "..\..\Complexo\Complexo.h"
#include "..\..\Constante\Fases.h"

#include "..\..\..\DLL_Inc\SisLin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTRede3 * __fastcall NewObjRede3(VTGeralC *geralC)
   {
   return(new TRede3(geralC));
   }

//---------------------------------------------------------------------------
__fastcall TRede3::TRede3(VTGeralC *geralC)
   {
   // Salva objetos externos
   this->geralC = geralC;

   // Aloca memória
   lis12          = new TList;
   lis32          = new TList;
   // lisATER        = new TList;
   lisBANCO_DF    = new TList;
   lisBAR         = new TList;
   lisBATERIA     = new TList;
   lisCAPS        = new TList;
   lisCARGA       = new TList;
   lisCNL         = new TList;
   lisGRUPO_LTC12 = new TList;
   lisGRUPO_LTC32 = new TList;
   lisGRUPO_MUTUA = new TList;
   lisMED         = new TList;
   lisMED_LIGADOS = new TList;
   lisREG12       = new TList;
   lisREG32       = new TList;
   lisSUP         = new TList;
   lisTRAFO12     = new TList;
   lisTRAFO32     = new TList;
   lisTRAFO33     = new TList;
   lisTRAFOEQUIV  = new TList;
   lisTRAFOZZ     = new TList;
   lisTRECHO      = new TList;

   // Inicializa ID de barras e trechos
   Maior_ID_Barra  = 0;
   Maior_ID_Trecho = 0;

   // Define valor de constantes
   raiz3         = sqrt(3.);
   raiz3_inverso = 1. / raiz3;
   cum           = complex<double>(1., 0.);
   czero         = complex<double>(0., 0.);
   }

//---------------------------------------------------------------------------
__fastcall TRede3::~TRede3(void)
   {
   // Libera memória
   delete lis12; // Os elementos desta lista são os mesmos de 'lisTRAFO12' e 'lisREG12'
   delete lis32; // Os elementos desta lista são os mesmos de 'lisTRAFO32' e 'lisREG32'
   // DeletaLista(&lisATER);
   DeletaLista(&lisBANCO_DF);
   DeletaLista(&lisBAR);
   DeletaLista(&lisBATERIA);
   DeletaLista(&lisCAPS);
   DeletaLista(&lisCARGA);
   DeletaLista(&lisCNL);
   DeletaLista(&lisGRUPO_LTC12);
   DeletaLista(&lisGRUPO_LTC32);
   DeletaLista(&lisGRUPO_MUTUA);
   DeletaLista(&lisMED);
   delete lisMED_LIGADOS; // Os elementos desta lista são os mesmos de 'lisMED'
   DeletaLista(&lisREG12);
   DeletaLista(&lisREG32);
   DeletaLista(&lisSUP);
   DeletaLista(&lisTRAFO12);
   DeletaLista(&lisTRAFO32);
   DeletaLista(&lisTRAFO33);
   DeletaLista(&lisTRAFOEQUIV);
   DeletaLista(&lisTRAFOZZ);
   DeletaLista(&lisTRECHO);
   }

//---------------------------------------------------------------------------
int __fastcall TRede3::AnalisaIDBarra(int id_sinap)
{
// Atualiza 'Maior_ID_Barra' SEMPRE
if(id_sinap > Maior_ID_Barra) Maior_ID_Barra = id_sinap;

// Retorna ID fornecido
return(id_sinap);
}

//---------------------------------------------------------------------------
int __fastcall TRede3::AnalisaIDTrecho(int id_sinap)
{
// Atualiza 'Maior_ID_Trecho' SEMPRE
if(id_sinap > Maior_ID_Trecho) Maior_ID_Trecho = id_sinap;

// Retorna ID fornecido
return(id_sinap);
}

//---------------------------------------------------------------------------
AnsiString __fastcall TRede3::AtribuiCodigoLigacao(AnsiString codigo,
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
else if(bar1 != NULL)  // Trafo ZZ
   {
   codigo2 = "ZZ-" + bar1->codigo;
   }

return(codigo2.SubString(0, 15));
}

//---------------------------------------------------------------------------
void __fastcall TRede3::AtribuiIDs(void)
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
int __fastcall TRede3::ConverteFasesCarga(int    fases_carga,
                                          TBarra *bar1)
   {
   // Variáveis locais
   int   fases_barra, int1, int2;
   TFase *fases;

   // Converte fases da barra em inteiro único
   fases       = bar1->fases;
   fases_barra = fases->FasesAsInt();

   // Elimina fase da carga que não existe na barra, preservando faseT se houver
   int1        = fases_carga & faseT;
   int2        = fases_carga & fases_barra;
   fases_carga = int1 | int2;

   // Determina código local da carga
   switch(fases_carga)
      {
      case faseAN:
         return(cgAN);

      case faseBN:
         return(cgBN);

      case faseCN:
         return(cgCN);

      case faseA:
         if(fases->ExisteN()) return(cgAN);
         else                 return(cgAT);

      case faseAT:
         return(cgAT);

      case faseB:
         if(fases->ExisteN()) return(cgBN);
         else                 return(cgBT);

      case faseBT:
         return(cgBT);

      case faseC:
         if(fases->ExisteN()) return(cgCN);
         else                 return(cgCT);

      case faseCT:
         return(cgCT);

      case faseAB:
         return(cgAB);

      case faseBC:
         return(cgBC);

      case faseCA:
         return(cgCA);

      case faseABN:
         return(cgABN);

      case faseBCN:
         return(cgBCN);

      case faseCAN:
         return(cgCAN);

      case faseABCNT:
         return(cgYaterr);

      case faseABCN:
         return(cgYiso);

      case faseABC:
         return(cgTriang);

      default:
         return(-1);  // Erro
      }
   }

//---------------------------------------------------------------------------
int __fastcall TRede3::ConverteLigacaoCargaParaEstatico(int lig_carga)
{
if     (lig_carga == cgYaterr) return(esYaterr);
else if(lig_carga == cgYiso)   return(esYiso);
else if(lig_carga == cgTriang) return(esTriang);
else                           return(-1);
}

//---------------------------------------------------------------------------
void __fastcall TRede3::DeletaLista(TList **lista)
   {
   TObject *ptr;

   if((*lista) == NULL) return;

   for(int n=0; n < (*lista)->Count; n++)
      {
      ptr = (TObject *) (*lista)->Items[n];
      if(ptr != NULL) delete ptr;
      }
   delete (*lista);
   (*lista) = NULL;
   }

//---------------------------------------------------------------------------
bool __fastcall TRede3::ExisteLigacaoComNeutro(TBarra *bar1,
                                               TBarra *bar2)
{
// Variáveis locais
TBarra      *bar_teste;
TCapSerie   *caps1;
TLigacaoC   *ligacao;
// TReg12      *reg12;
// TReg32      *reg32;
TTrecho     *trec1;
// TTrafo12    *trafo12;
// TTrafo32    *trafo32;
// TTrafo33    *trafo33;
// TTrafoEquiv *trafoequiv;
TTrafoZZ    *trafozz;

// IMPORTANTE: A verificação de conectividade do neutro ficou limitada
//             a CapSerie3, TrafoZZ3 e Trecho3 (todos os trafos exceto o
//             TrafoZZ3 foram desabilitados). Esta alteração foi motivada
//             por problemas numa rede com 2 trafos Trafo12 com barra
//             primária em comum e barras secundárias distintas.
//             HPS, 2014.11.19.

// Loop de ligações da barra 'bar1'
for(int nl=0; nl < bar1->lisLIG->Count; nl++)
   {
   ligacao = (TLigacaoC *) bar1->lisLIG->Items[nl];
   switch(ligacao->tipo)
      {
      case CapSerie3:
         caps1 = (TCapSerie *) ligacao->plig;
         if(ligacao->sentido == 0) bar_teste = caps1->bar2;
         else                      bar_teste = caps1->bar1;
         if(bar_teste == bar2) continue;  // Descarta ligação em paralelo
         if(caps1->fases->ExisteN()) return(true);
         break;

      case Reg12:
         /*
         reg12 = (TReg12 *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            if(reg12->bar2 == bar2) continue;  // Descarta ligação em paralelo
            if(reg12->fases_pri->ExisteN()) return(true);
            }
         else
            {
            if(reg12->bar1 == bar2) continue;  // Descarta ligação em paralelo
            if(reg12->fases_sec->ExisteN()) return(true);
            }
         */
         break;

      case Reg32:
         /*
         reg32 = (TReg32 *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            if(reg32->bar2 == bar2) continue;  // Descarta ligação em paralelo
            if(reg32->fases_pri->ExisteN()) return(true);
            }
         else
            {
            if(reg32->bar1 == bar2) continue;  // Descarta ligação em paralelo
            if(reg32->fases_sec->ExisteN()) return(true);
            }
         */
         break;

      case Trafo12:
         /*
         trafo12 = (TTrafo12 *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            if(trafo12->bar2 == bar2) continue;  // Descarta ligação em paralelo
            if(trafo12->fases_pri->ExisteN()) return(true);
            }
         else
            {
            if(trafo12->bar1 == bar2) continue;  // Descarta ligação em paralelo
            if(trafo12->fases_sec->ExisteN()) return(true);
            }
         */
         break;

      case Trafo32:
         /*
         trafo32 = (TTrafo32 *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            if(trafo32->bar2 == bar2) continue;  // Descarta ligação em paralelo
            if(trafo32->fases_pri->ExisteN()) return(true);
            }
         else
            {
            if(trafo32->bar1 == bar2) continue;  // Descarta ligação em paralelo
            if(trafo32->fases_sec->ExisteN()) return(true);
            }
         */
         break;

      case Trafo33:
         /*
         trafo33 = (TTrafo33 *) ligacao->plig;
         if(ligacao->sentido == 0)
            {
            if((trafo33->bar2 == bar2) || (trafo33->bar3 == bar2)) continue; // Descarta ligação em paralelo
            if(trafo33->fases_pri->ExisteN()) return(true);
            }
         else if(ligacao->sentido == 1)
            {
            if((trafo33->bar1 == bar2) || (trafo33->bar3 == bar2)) continue; // Descarta ligação em paralelo
            if(trafo33->fases_sec->ExisteN()) return(true);
            }
         else
            {
            if((trafo33->bar1 == bar2) || (trafo33->bar2 == bar2)) continue; // Descarta ligação em paralelo
            if(trafo33->fases_ter->ExisteN()) return(true);
            }
         */
         break;

      case TrafoEquiv3:
         /*
         trafoequiv = (TTrafoEquiv *) ligacao->plig;
         if(ligacao->sentido == 0) bar_teste = trafoequiv->bar2;
         else                      bar_teste = trafoequiv->bar1;
         if(bar_teste == bar2) continue;  // Descarta ligação em paralelo
         if(trafoequiv->fases->ExisteN()) return(true);
         */
         break;

      case TrafoZZ3:
         trafozz = (TTrafoZZ *) ligacao->plig;
         if(trafozz->fases->ExisteN()) return(true);
         break;

      case Trecho3:
         trec1 = (TTrecho *) ligacao->plig;
         if(ligacao->sentido == 0) bar_teste = trec1->bar2;
         else                      bar_teste = trec1->bar1;
         if(bar_teste == bar2) continue;  // Descarta ligação em paralelo
         if(trec1->fases->ExisteN()) return(true);
         break;

      default:
         break;  // Erro - nada a fazer
      } // switch
   } // for(nl)

return(false);  // Não existe ligação através do neutro
}

//---------------------------------------------------------------------------
bool __fastcall TRede3::ExisteMedidorReal(TBarra *bar1,
                                          int    tipo_eqpto,
                                          int    tipo_medicao,
                                          int    id_calc,
                                          int    fase_int)
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
   if(med1->id_calc      != id_calc)      continue;
   if(med1->fase_int     == fase_int)     return(true);
   }

return(false);
}

//---------------------------------------------------------------------------
bool __fastcall TRede3::IsBarraExternaSup(TBarra *bar1)
   {
   TSup *sup1;

   if(lisSUP == NULL) return(false);
   for(int n=0; n < lisSUP->Count; n++)
      {
      sup1 = (TSup *) lisSUP->Items[n];
      if(sup1->bar_ext == bar1) return(true);
      }

   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TRede3::LimpaLista(TList *lista)
   {
   TObject *ptr;

   if(lista == NULL) return;

   for(int n=0; n < lista->Count; n++)
      {
      ptr = (TObject *) lista->Items[n];
      if(ptr != NULL) delete ptr;
      }
   lista->Clear();
   }

//---------------------------------------------------------------------------
TBarra * __fastcall TRede3::LocalizaBarraCodigo(AnsiString codigo,
                                                TList      *lisBAR_AUX)
   {
   TBarra *bar1;

   if(lisBAR_AUX == NULL) lisBAR_AUX = lisBAR;

   for(int n=0; n < lisBAR_AUX->Count; n++)
      {
      bar1 = (TBarra *) lisBAR_AUX->Items[n];
      if(bar1->codigo.AnsiCompareIC(codigo) == 0) return(bar1);
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
TBarra * __fastcall TRede3::LocalizaBarraId(int   barra_id,
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
TBarra * __fastcall TRede3::LocalizaBarraOffset(int   offset,
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
TGrupoLTC12 * __fastcall TRede3::LocalizaGrupoLTC12(TBarra *bar_ref)
   {
   TGrupoLTC12 *grupo1;

   for(int n=0; n < lisGRUPO_LTC12->Count; n++)
      {
      grupo1 = (TGrupoLTC12 *) lisGRUPO_LTC12->Items[n];
      if(bar_ref == grupo1->bar_ref) return(grupo1);
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
TGrupoLTC32 * __fastcall TRede3::LocalizaGrupoLTC32(TBarra *bar_ref)
   {
   TGrupoLTC32 *grupo1;

   for(int n=0; n < lisGRUPO_LTC32->Count; n++)
      {
      grupo1 = (TGrupoLTC32 *) lisGRUPO_LTC32->Items[n];
      if(bar_ref == grupo1->bar_ref) return(grupo1);
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
TMedidor * __fastcall TRede3::LocalizaMedidor(void *pvoid)
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
TSup * __fastcall TRede3::LocalizaSuprimentoBarraExt(TBarra *barra)
   {
   TSup *sup1;

   sup1 = barra->sup;
   if(sup1 == NULL)           return(NULL);  // Barra não é de suprimento
   if(sup1->bar_ext == barra) return(sup1);  // Barra fornecida é a EXTERNA
   else                       return(NULL);  // Barra fornecida é a INTERNA
   }

//---------------------------------------------------------------------------
TSup * __fastcall TRede3::LocalizaSuprimentoBarraInt(TBarra *barra)
   {
   TSup *sup1;

   sup1 = barra->sup;
   if(sup1 == NULL)           return(NULL);  // Barra não é de suprimento
   if(sup1->bar_int == barra) return(sup1);  // Barra fornecida é a INTERNA
   else                       return(NULL);  // Barra fornecida é a EXTERNA
   }

//---------------------------------------------------------------------------
TTrafo12 * __fastcall TRede3::LocalizaTrafo12Id(int trafo12_id)
   {
   TTrafo12 *trafo12;

   if(lisTRAFO12 == NULL) return(NULL);

   for(int n=0; n < lisTRAFO12->Count; n++)
      {
      trafo12 = (TTrafo12 *) lisTRAFO12->Items[n];
      if(trafo12_id == trafo12->id) return(trafo12);
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
TTrafo32 * __fastcall TRede3::LocalizaTrafo32Id(int trafo32_id)
   {
   TTrafo32 *trafo32;

   if(lisTRAFO32 == NULL) return(NULL);

   for(int n=0; n < lisTRAFO32->Count; n++)
      {
      trafo32 = (TTrafo32 *) lisTRAFO32->Items[n];
      if(trafo32_id == trafo32->id) return(trafo32);
      }

   return(NULL);
   }

//---------------------------------------------------------------------------
TTrecho * __fastcall TRede3::LocalizaTrechoId(int   trecho_id,
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
bool __fastcall TRede3::TodosMedidoresReaisPotenciaBarra(void)
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
bool __fastcall TRede3::VerificaFasesCarga(TBarra *bar1,
                                           int    fases_carga)
   {
   int endA, endB, endC, endN;

   // Obtém endereço das fases da barra
   endA = bar1->fases->IndiceA();
   endB = bar1->fases->IndiceB();
   endC = bar1->fases->IndiceC();
   endN = bar1->fases->IndiceN();

   // Verifica fases da carga com as fases da barra
   switch(fases_carga)
      {
      case cgAN:
         if((endA < 0) || (endN < 0)) return(false);
         break;

      case cgBN:
         if((endB < 0) || (endN < 0)) return(false);
         break;

      case cgCN:
         if((endC < 0) || (endN < 0)) return(false);
         break;

      case cgAT:
         if(endA < 0) return(false);
         break;

      case cgBT:
         if(endB < 0) return(false);
         break;

      case cgCT:
         if(endC < 0) return(false);
         break;

      case cgAB:
         if((endA < 0) || (endB < 0)) return(false);
         break;

      case cgBC:
         if((endB < 0) || (endC < 0)) return(false);
         break;

      case cgCA:
         if((endC < 0) || (endA < 0)) return(false);
         break;

      case cgABN:
         if((endA < 0) || (endB < 0) || (endN < 0)) return(false);
         break;

      case cgBCN:
         if((endB < 0) || (endC < 0) || (endN < 0)) return(false);
         break;

      case cgCAN:
         if((endC < 0) || (endA < 0) || (endN < 0)) return(false);
         break;

      case cgYaterr:
      case cgYiso:
         if((endA < 0) || (endB < 0) || (endC < 0) || (endN < 0)) return(false);
         break;

      case cgTriang:
         if((endA < 0) || (endB < 0) || (endC < 0)) return(false);
         break;

      default:
         return(false);  // Erro - fase da carga inválida
      }

   return(true);
   }

//---------------------------------------------------------------------------

