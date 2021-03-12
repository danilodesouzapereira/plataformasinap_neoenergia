//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TMonta3.h"
#include ".\TGrupoTripla.h"
#include ".\TNo3.h"
#include ".\TTripla.h"

#include "..\Comum13\Constantes_Calculo.h"
#include "..\Comum13\Estruturas.h"
#include "..\Comum13\TBateriaC.h"
#include "..\Comum13\TMedidor.h"
#include "..\Comum13\TTempoCPU.h"
#include "..\Comum13\VTGeralC.h"
#include "..\Impressao3\TImpressao3.h"
#include "..\Rede3\TBancoDF.h"
#include "..\Rede3\TBarra.h"
#include "..\Rede3\TBarraDesc.h"
#include "..\Rede3\TBarraVnomZero.h"
#include "..\Rede3\TCapSerie.h"
#include "..\Rede3\TCarga.h"
#include "..\Rede3\TCNL.h"
#include "..\Rede3\TFase.h"
#include "..\Rede3\TGrupoLTC12.h"
#include "..\Rede3\TGrupoLTC32.h"
#include "..\Rede3\TGrupoMutua.h"
#include "..\Rede3\TLigacaoC.h"
#include "..\Rede3\TMutua.h"
#include "..\Rede3\TReg12.h"
#include "..\Rede3\TReg32.h"
#include "..\Rede3\TSup.h"
#include "..\Rede3\TTrafo12.h"
#include "..\Rede3\TTrafo33.h"
#include "..\Rede3\TTrafoEquiv.h"
#include "..\Rede3\TTrafoZZ.h"
#include "..\Rede3\TTrecho.h"
#include "..\Rede3\VTRede3.h"
#include "..\VTData.h"

#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Curto\TBarraDefeito.h"
#include "..\..\Curto\TCurtoAux3.h"
#include "..\..\Eqv\VTEqv.h"
#include "..\..\Estimador\VTEstimador.h"
#include "..\..\Matriz\VTMatriz.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\SisLin\VTSisLin.h"

#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\SisLin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta3 * __fastcall NewObjMonta3(TImpressao3 *impr3,
                                   VTRede3     *rede3,
                                   VTGeralC    *geralC,
                                   TCurtoAux3  *curto_aux3)
   {
   return(new TMonta3(impr3, rede3, geralC, curto_aux3));
   }

//---------------------------------------------------------------------------
// Fun��o global para ordena��o de barras
static int __fastcall EscolheBarraTipoCodigo(void *obj1,
                                             void *obj2)
   {
   // Vari�veis locais
   TBarra *bar1, *bar2;

   // Primeiro crit�rio: ordem crescente do tipo da barra
   bar1 = (TBarra *) obj1;
   bar2 = (TBarra *) obj2;
   if     (bar1->tipo < bar2->tipo) return(-1);
   else if(bar1->tipo > bar2->tipo) return( 1);

   // Segundo crit�rio: ordem crescente do c�digo da barra
   return(bar1->codigo.AnsiCompareIC(bar2->codigo));
   }

//---------------------------------------------------------------------------
// Fun��o global para ordena��o de medidores
static int __fastcall EscolheMedidor(void *obj1,
                                     void *obj2)
   {
   // Vari�veis locais
   int      num1;
   TBarra   *bar_ref1, *bar_ref2, *bar21, *bar22;
   TMedidor *med1, *med2;
   TTrafo32 *trafo1, *trafo2;
   TTrecho  *trec1, *trec2;

   // Inicializa��o
   med1     = (TMedidor *) obj1;
   med2     = (TMedidor *) obj2;
   bar_ref1 = med1->bar_ref;
   bar_ref2 = med2->bar_ref;

   // Primeiro crit�rio: ordem crescente do c�digo da barra de refer�ncia
   num1 = bar_ref1->codigo.AnsiCompareIC(bar_ref2->codigo);
   if     (num1 < 0) return(-1);
   else if(num1 > 0) return( 1);

   // Segundo crit�rio: ordem crescente do tipo de equipamento
   // (teBARRA, teTRECHO, teTRAFO)
   if     (med1->tipo_eqpto < med2->tipo_eqpto) return(-1);
   else if(med1->tipo_eqpto > med2->tipo_eqpto) return( 1);

   // Terceiro crit�rio: ordem crescente do c�digo da barra final
   // (teTRECHO, teTRAFO)
   if(med1->tipo_eqpto == teBARRA)
      {
      bar21 = bar_ref1;  // Mesma barra para 'bar21' e 'bar22'
      bar22 = bar_ref1;
      }
   else if(med1->tipo_eqpto == teTRECHO)
      {
      trec1 = (TTrecho *) med1->eqpto;
      trec2 = (TTrecho *) med2->eqpto;
      bar21 = trec1->bar2;
      bar22 = trec2->bar2;
      if(bar21 == bar_ref1) bar21 = trec1->bar1; // Pega a outra barra
      if(bar22 == bar_ref2) bar22 = trec2->bar1; // Pega a outra barra
      }
   else  //teTRAFO
      {
      trafo1 = (TTrafo32 *) med1->eqpto;
      trafo2 = (TTrafo32 *) med2->eqpto;
      bar21  = trafo1->bar2;
      bar22  = trafo2->bar2;
      if(bar21 == bar_ref1) bar21 = trafo1->bar1; // Pega a outra barra
      if(bar22 == bar_ref2) bar22 = trafo2->bar1; // Pega a outra barra
      }
   num1 = bar21->codigo.AnsiCompareIC(bar22->codigo);
   if     (num1 < 0) return(-1);
   else if(num1 > 0) return( 1);

   // Quarto crit�rio: ordem crescente do ID do equipamento
   if     (med1->id_calc < med2->id_calc) return(-1);
   else if(med1->id_calc > med2->id_calc) return( 1);

   // Quinto crit�rio: ordem crescente do tipo de medi��o
   // (medMOD_V, medMOD_I, medP, medQ, medS)
   if     (med1->tipo_medicao < med2->tipo_medicao) return(-1);
   else if(med1->tipo_medicao > med2->tipo_medicao) return( 1);

   // Sexto crit�rio: ordem crescente da fase
   if     (med1->fase_int < med2->fase_int) return(-1);
   else if(med1->fase_int > med2->fase_int) return( 1);

   // Empate
   return(0);
   }

//---------------------------------------------------------------------------
__fastcall TMonta3::TMonta3(TImpressao3 *impr3,
                            VTRede3     *rede3,
                            VTGeralC    *geralC,
                            TCurtoAux3  *curto_aux3)
   {
   // Salva ponteiros para objetos externos
   this->apl        = geralC->apl;
   this->data       = geralC->data;
   this->impr3      = impr3;
   this->rede3      = rede3;
   this->geralC     = geralC;
   this->curto_aux3 = curto_aux3;

   // Inicializa ponteiros para objetos locais
   Ynod = NULL;

   // Inicializa dados
   flag_monta_lig_barra = false;
   ordemh               = 1;  // Frequ�ncia fundamental por default

   // Define valor de constantes
   graurad       = M_PI / 180.;
   raiz3         = sqrt(3.);
   raiz3_inverso = 1. / raiz3;
   cum           = complex<double>(1., 0.);
   czero         = complex<double>(0., 0.);
   }

//---------------------------------------------------------------------------
__fastcall TMonta3::~TMonta3(void)
   {
   // Libera mem�ria
   delete Ynod;
   }

//---------------------------------------------------------------------------
void __fastcall TMonta3::AcertaAngRefBarras(void)
{
// Vari�veis locais
int    indice_ABCN, num_fases;
double vet_ang_temp[MAX_FASES_G3];
TBarra *bar1;
TFase  *fases;

// Loop de barras
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];

   // Salva conte�do atual de 'ang_ref_rad'
   for(int nfa=0; nfa < MAX_FASES_G3; nfa++)
      {
      vet_ang_temp[nfa] = bar1->ang_ref_rad[nfa];
      }

   // Transfere �ngulos de acordo com as fases existentes na barra
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      indice_ABCN            = fases->IndiceABCN(nfa);
      bar1->ang_ref_rad[nfa] = vet_ang_temp[indice_ABCN];
      }
   }
}

/*
// Desabilitada em 2018.03.08 - HPS
//---------------------------------------------------------------------------
bool __fastcall TMonta3::AcertaImpedanciaReguladoresNR3(void)
{
// Vari�veis locais
double          zbase;
complex<double> z_ohm;
TReg12          *reg12;
TReg32          *reg32;
TTrafo32        *trafo32;

// Loop de reguladores TReg12
// reat_pu = geralC->reat_regul_DEMAIS_NR_pu;
// for(int nr=0; nr < rede3->lisREG12->Count; nr++)
//    {
//    reg12 = (TReg12 *) rede3->lisREG12->Items[nr];
//    if((reg12->tipo == AUTOTRAFO4) || (reg12->tipo == AUTOTRAFO6_1T) ||
//       (reg12->tipo == AUTOTRAFO6_2T))
//       {
//       zbase             = reg12->vnom2 * reg12->vnom2 / reg12->snom_mva; // Ref. secund�rio
//       reg12->ztrafo_ohm = complex<double>(0., (reat_pu * zbase));
//       }
//    else  // TRAFO4 ou TRAFO5
//       {
//       zbase             = reg12->vnom1 * reg12->vnom1 / reg12->snom_mva; // Ref. prim�rio
//       reg12->ztrafo_ohm = complex<double>(0., (reat_pu * zbase));
// 		}
//    }

// Loop de reguladores TReg32
for(int nr=0; nr < rede3->lisREG32->Count; nr++)
   {
   reg32 = (TReg32 *) rede3->lisREG32->Items[nr];
   zbase = reg32->vnom2 * reg32->vnom2 / reg32->snom_mva;  // Ref. secund�rio
   if(reg32->tipo == AUTOTRAFO_DF)
      {
      z_ohm             = geralC->zreg_AUTODF_NR_pu * zbase;
      reg32->ztrafo_ohm = z_ohm;
      }
   // else
   //    {
   //    reat_pu           = geralC->reat_regul_DEMAIS_NR_pu;
   //    reg32->ztrafo_ohm = complex<double>(0., (reat_pu * zbase));
   //    }
   }

// Loop de trafos TTrafo32 (somente AUTOTRAFO_DF)
for(int nt=0; nt < rede3->lisTRAFO32->Count; nt++)
   {
   trafo32 = (TTrafo32 *) rede3->lisTRAFO32->Items[nt];
   if(trafo32->tipo == AUTOTRAFO_DF)
      {
      zbase               = trafo32->vnom2 * trafo32->vnom2 / trafo32->snom_mva; // Ref. secund�rio
      z_ohm               = geralC->zreg_AUTODF_NR_pu * zbase;
      trafo32->ztrafo_ohm = z_ohm;
      }
   }

return(true);
}
*/

//---------------------------------------------------------------------------
void __fastcall TMonta3::AcertaMatrizesTrechos(void)
{
// Vari�veis locais
int             fase_int, num_fases;
double          fator_ybase, ybase;
complex<double> y1, z1;
TBarra          *bar1;
TFase           *fases;
TTrecho         *trec1;

// Atualiza matrizes de imped�ncias s�rie e de suscept�ncias paralelo
// do trecho (passa as duas matrizes para [pu])
// IMPORTANTE: esta fun��o pressup�e uma chamada anterior � fun��o
//             'VerificaTensaoNominalBarras()'

// Loop de trechos
for(int nt=0; nt < rede3->lisTRECHO->Count; nt++)
   {
   trec1     = (TTrecho *) rede3->lisTRECHO->Items[nt];
   bar1      = trec1->bar1;
   fases     = trec1->fases;
   num_fases = fases->NumFases();
   for(int i=0; i < num_fases; i++)
      {
      fase_int    = fases->Fase(i);
      fator_ybase = geralC->Sbase_fase / bar1->VnomFase_kv(fase_int);
      for(int j=0; j < num_fases; j++)
         {
         fase_int = fases->Fase(j);
         ybase    = fator_ybase / bar1->VnomFase_kv(fase_int);
         z1                           = trec1->MatZserie_pu[i][j];
         y1                           = trec1->MatYparalelo_pu[i][j];
         trec1->MatZserie_pu[i][j]    = z1 * ybase;
         trec1->MatYparalelo_pu[i][j] = y1 / ybase;
         }
      }
   }
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddAterramento(void)
   {
   // Local variables
   int            count, tipo_ater;
   // TAterramento   *ater1;
   TBarra         *bar1;
   smcATERRAMENTO aterramento;

   // Loop para todos Aterramentos
   count = data->NumeroAterramento;
   for(int n=0; n < count; n++)
      {
      if(! data->EqptoAterramento(n, &aterramento)) return(false);
      bar1 = (TBarra *) aterramento.barraCALC;
      // ater1 = new TAterramento(bar1, aterramento.z_ohm);
      // rede3->lisATER->Add(ater1);
      // Redefine aterramento da barra
      if(IsComplexZero(aterramento.z_ohm)) tipo_ater = atSolido;
      else                                 tipo_ater = atZ;
      bar1->DefineAterramento(tipo_ater, geralC, aterramento.z_ohm);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddBarra(void)
   {
   // Vari�veis locais
   int        count;
   AnsiString codigo;
   TBarra     *bar1;
   smcBARRA   barra;

   // Loop para todas as Barras
   count = data->NumeroBarra;
   for(int n=0; n < count; n++)
      {
      bar1 = new TBarra;
      if(! data->EqptoBarra(n, &barra, bar1))
         {
         delete bar1;
         return(false);
         }

      // Insere TBarra em lisBAR
      rede3->lisBAR->Add(bar1);
      bar1->eqpto           = barra.eqpto;
      bar1->id              = rede3->AnalisaIDBarra(barra.id);
      bar1->mantida         = barra.mantida;
      bar1->sup_equivalente = barra.sup_equivalente;
      codigo                = barra.codigo;
      if(codigo.IsEmpty()) codigo = "B" + AnsiString(barra.id);
      bar1->codigo        = codigo.SubString(0, 15);  // Garante tamanho m�ximo
      bar1->vnom_linha_kv = barra.vff_kv;
      bar1->vnom_faseA_kv = barra.van_kv;
      bar1->vnom_faseB_kv = barra.vbn_kv;
      bar1->vnom_faseC_kv = barra.vcn_kv;
      bar1->tipo_zona     = barra.tipo_zona;
      // Atribui��o tempor�ria que ser� modificada em 'AcertaAngRefBarras()'.
      // Aqui � usada a indexa��o: A = 0, B = 1, C = 2, N = 3.
      // Na fun��o 'AcertaAngRefBarras()' os �ndices ser� convertidos de
      // acordo com as fases definitivamente existentes em cada barra.
      for(int nfa=0; nfa < MAX_FASE; nfa++)
         {
         bar1->ang_ref_rad[nfa] = barra.fase_zona_rad[nfa];
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddBarraInternaSuprimento(void)
   {
   // Vari�veis locais
   int    tipo_ant;
   TBarra *bar_ext, *bar_int;
   TSup   *sup1;

   // Loop para todos os suprimentos
   for(int ns=0; ns < rede3->lisSUP->Count; ns++)
      {
      sup1                   = (TSup *) rede3->lisSUP->Items[ns];
      bar_ext                = sup1->bar_ext;
      tipo_ant               = bar_ext->tipo;  // Salva tipo anterior
      bar_ext->tipo          = BAR_PQ;         // Redefine tipo da barra externa
      bar_ext->tipo_original = BAR_PQ;

      // Redefine aterramento da barra externa, se a defini��o
      // original (atSolido) n�o foi alterada
      if(bar_ext->tipo_ater == atSolido) bar_ext->tipo_ater = atIsolado;

      // Cria barra interna
      bar_int = new TBarra;
      rede3->lisBAR->Add(bar_int);
      sup1->bar_int          = bar_int;
      bar_int->sup           = sup1;
      bar_int->eqpto         = bar_ext->eqpto;
      bar_int->codigo        = ("z_" + bar_ext->codigo).SubString(0, 15); // Garante tamanho m�ximo
      bar_int->tipo          = tipo_ant;
      bar_int->tipo_original = tipo_ant;
      bar_int->tipo_ater     = atSolido;
      bar_int->mantida       = bar_ext->mantida;
      bar_ext->fases->Copia(bar_int->fases);  // Copia fases da barra externa
      bar_ext->CopiaAngRef(bar_int);          // Copia �ngulos de refer�ncia
      bar_int->vnom_faseA_kv = bar_ext->vnom_faseA_kv;
      bar_int->vnom_faseB_kv = bar_ext->vnom_faseB_kv;
      bar_int->vnom_faseC_kv = bar_ext->vnom_faseC_kv;
      bar_int->vnom_linha_kv = bar_ext->vnom_linha_kv;
      bar_int->ZeraCarga();
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddBarraTrechoFicticios(void)
   {
   // Adiciona barra e trecho fict�cios se n�o houver nenhuma barra de carga

   // Vari�veis locais
   int             fases_int, num_fases;
   complex<double> z1_ohm(0., 0.01);
   TBarra          *bar_ficticia, *bar1, *bar_ger;
   TFase           *fases;
   TTrecho         *trec1;

   // Verifica necessidade de adicionar barra fict�cia
   for(int nb=0; nb < rede3->lisBAR->Count; nb++)
      {
      bar1 = (TBarra *) rede3->lisBAR->Items[nb];
      if(bar1->TipoCarga()) return(true); // N�o � preciso
      }

   // Localiza primeiro gerador
   bar_ger = NULL;
   for(int nb=0; nb < rede3->lisBAR->Count; nb++)
      {
      bar1 = (TBarra *) rede3->lisBAR->Items[nb];
      if(bar1->TipoGeracao())
         {
         bar_ger = bar1;
         break;
         }
      }
   if(bar_ger == NULL) return(false);  // Erro

   // Adiciona barra fict�cia
   bar_ficticia = new TBarra;
   rede3->lisBAR->Add(bar_ficticia);
   bar_ficticia->mantida = true;
   bar_ficticia->codigo  = geralC->CodigoBarraFicticia;

   // Acerta dados faltantes da barra fict�cia
   bar_ficticia->vnom_linha_kv = bar_ger->vnom_linha_kv;
   bar_ficticia->vnom_faseA_kv = bar_ger->vnom_faseA_kv;
   bar_ficticia->vnom_faseB_kv = bar_ger->vnom_faseB_kv;
   bar_ficticia->vnom_faseC_kv = bar_ger->vnom_faseC_kv;
   bar_ficticia->tipo_zona     = bar_ger->tipo_zona;

   // Obt�m fases da barra de gera��o
   fases_int = bar_ger->fases->FasesAsInt();

   // Adiciona trecho fict�cio com as mesmas fases de 'bar_ger'
   trec1 = new TTrecho(bar_ficticia, bar_ger, fases_int);
   rede3->lisTRECHO->Add(trec1);
   trec1->codigo   = geralC->CodigoTrechoFicticio;
   trec1->compr_km = 0.001;

   // Copia �ngulos de refer�ncia para a barra fict�cia (por constru��o,
   // as fases de 'trec1' e de 'bar_ficticia' s�o as mesmas de 'bar_ger')
   bar_ger->CopiaAngRef(bar_ficticia);

   // Monta matriz de imped�ncias s�rie do trecho
   fases     = trec1->fases;
   num_fases = fases->NumFases();
   for(int i=0; i < num_fases; i++)
      {
      trec1->vet_iadm_a[i] = 10000.;  // Valor arbitr�rio
      for(int j=0; j < num_fases; j++)
         {
         if(i == j) trec1->MatZserie_pu[i][j] = z1_ohm; // Ser� convertido a [pu] posteriormente
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddBateria(void)
{
// Vari�veis locais
int        count;
TBarra     *bar1;
TBateriaC  *batc1;
smcBATERIA bateria;

// Loop para todas as Baterias
count = data->NumeroBateria;
for(int n=0; n < count; n++)
   {
   if (! data->EqptoBateria(n, &bateria)) return(false);

   // Obt�m barra da Bateria
   bar1 = (TBarra *) bateria.barraCALC;
   if(bar1 == NULL) continue;  // Prote��o

   // Insere Bateria nas listas de baterias (lista global e lista da barra)
   // NB: a inser��o na lista da barra n�o pode estar em TBateriaC porque
   //     depende de TBarra ser de NR1 ou NR3
   batc1 = NewObjBateriaC(bar1);
   rede3->lisBATERIA->Add(batc1);   // Lista global
   bar1->AddBateria(batc1);         // Lista da barra
   batc1->AtualizaDados(&bateria);  // Copia dados
   // Acerta dados espec�ficos do modelo desequilibrado (3)
   batc1->fases   = bateria.fases;
   batc1->ligacao = rede3->ConverteFasesCarga(bateria.fases, bar1);
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddCapSerie(void)
   {
   // Vari�veis locais
   int             count;
   complex<double> z1;
   TBarra          *bar1, *bar2;
   TCapSerie       *caps1;
   smcCAPSERIE     caps;

   // Loop para todos os Cap. S�rie
   count = data->NumeroCapserie;
   for (int n = 0; n < count; n++)
      {
      if (! data->EqptoCapserie(n, &caps)) return(false);

      // Obt�m barras do capacitor
      bar1 = (TBarra *) caps.barraCALC_1;
      bar2 = (TBarra *) caps.barraCALC_2;
      if(bar1 == bar2) continue;  // Prote��o

      // Insere Cap. S�rie em lisCAPS
      caps1 = new TCapSerie(bar1, bar2);
      rede3->lisCAPS->Add(caps1);
      caps1->id     = caps.id;
      caps1->eqpto  = caps.eqpto;
      caps1->codigo = rede3->AtribuiCodigoLigacao(caps.codigo, bar1, bar2);
      z1            = complex<double>(caps.r_ohm, caps.x_ohm);
      if(Abs(z1) > 0.) caps1->yser_s = cum / z1;
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddCarga(void)
   {
   // Vari�veis locais
   int        count, fases;
   TBarra     *bar1;
   TCarga     *cargaG3;
   smcCARGA   carga;
   smcDEMANDA demanda;

   // Define dimens�o
   demanda.capacity = MAX_FASE_DEM;

   // Loop para todas as Cargas
   count = data->NumeroCarga;
   for(int nc=0; nc < count; nc++)
      {
      if (! data->EqptoCarga(nc, &carga)) return(false);
      bar1    = (TBarra *) carga.barraCALC;
	  cargaG3 = new TCarga(carga.id, bar1);
      rede3->lisCARGA->Add(cargaG3);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddChave(void)
   {
   // Vari�veis locais
   int      count, fases_int, num_fases;
   TBarra   *bar1, *bar2;
   TTrecho  *trec1;
   smcCHAVE chave;

   // Loop para todas as chaves
   count              = data->NumeroChave;
   geralC->num_chaves = count;  // Salva contador
   for(int n=0; n < count; n++)
      {
      if(! data->EqptoChave(n, &chave)) return(false);

      // Descarta chave se ela estiver aberta
      if(chave.estado == 0) continue;

      // Obt�m barras da chave
      bar1 = (TBarra *) chave.barraCALC_1;
      bar2 = (TBarra *) chave.barraCALC_2;
      if(bar1 == bar2) continue;  // Prote��o

      // Modifica��o motivada por Estimador3: chave com fases ABC em trecho BCN
      // 2017.07.30
      // Determina fases da chave
      // if(chave.fases == faseINV) return(false);
      // fases_int = chave.fases & faseABC;
      fases_int = bar1->fases->FasesAsInt() | bar2->fases->FasesAsInt();

      // Cria trecho e o insere em lisTRECHO
      trec1 = new TTrecho(bar1, bar2, fases_int);
      rede3->lisTRECHO->Add(trec1);
      trec1->eqpto    = chave.eqpto;
      trec1->codigo   = rede3->AtribuiCodigoLigacao(chave.codigo, bar1, bar2);
      trec1->id       = rede3->AnalisaIDTrecho(chave.id);
	  trec1->compr_km = 0.001 * geralC->comp_chave_m;

      // Salva corrente admiss�vel e monta matriz de imped�ncias s�rie do trecho
      num_fases = trec1->fases->NumFases();
      for(int i=0; i < num_fases; i++)
         {
         trec1->vet_iadm_a[i] = chave.corn_amp;
         // IMPORTANTE: as matriz ser� convertida para [pu] mais adiante,
         //             na fun��o 'AcertaMatrizesTrechos()', porque neste
         //             momento as tens�es nominais das barras terminais do
         //             trecho podem n�o estar TODAS definidas. HPS, 2018.03.29.
         trec1->MatZserie_pu[i][i] = geralC->zchave_ohm;  // S� na diagonal
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddCNL(void)
   {
   // Vari�veis locais
   int    count;
   double fconvA, fconvB, fconvC;
   TBarra *bar1;
   TCNL   *cnl;
   smcCNL smc_cnl;

   // Loop para todas as CNLs
   count = data->NumeroCNL;
   for(int nc=0; nc < count; nc++)
      {
      if (! data->EqptoCNL(nc, &smc_cnl)) return(false);

      // Adiciona CNL na lista de CNLs
      bar1 = (TBarra *) smc_cnl.barraCALC;
      cnl  = new TCNL(bar1, smc_cnl.id, smc_cnl.num_harmonicas);
      rede3->lisCNL->Add(cnl);
      fconvA = bar1->VnomFase_kv(faseA);
      fconvB = bar1->VnomFase_kv(faseB);
      fconvC = bar1->VnomFase_kv(faseC);
      if(fconvA != 0.) fconvA = 1. / fconvA;
      if(fconvB != 0.) fconvB = 1. / fconvB;
      if(fconvC != 0.) fconvC = 1. / fconvC;

      // Copia vetores - frequ�ncia fundamental
      cnl->vet_ordem[0]            = 1;
      cnl->mat_corrente->D2[0][0] = smc_cnl.pot_kva[0][0] * fconvA;
      cnl->mat_ang_rad->D2[0][0]  = smc_cnl.ang_grau[0][0] * graurad;
      cnl->mat_corrente->D2[0][1] = smc_cnl.pot_kva[0][1] * fconvB;
      cnl->mat_ang_rad->D2[0][1]  = smc_cnl.ang_grau[0][1] * graurad;
      cnl->mat_corrente->D2[0][2] = smc_cnl.pot_kva[0][2] * fconvC;
      cnl->mat_ang_rad->D2[0][2]  = smc_cnl.ang_grau[0][2] * graurad;
      cnl->mat_corrente->D2[0][3] = 0.;
      cnl->mat_ang_rad->D2[0][3]  = 0.;

      // Copia vetores - demais frequ�ncias ('cnl->num_harmonicas' inclui a fundamental)
      for(int nh=1; nh < cnl->num_harmonicas; nh++)
         {
         cnl->vet_ordem[nh]            = smc_cnl.ordem[nh];
         cnl->mat_corrente->D2[nh][0] = smc_cnl.pot_kva[nh][0];
         cnl->mat_ang_rad->D2[nh][0]  = smc_cnl.ang_grau[nh][0] * graurad;
         cnl->mat_corrente->D2[nh][1] = smc_cnl.pot_kva[nh][1];
         cnl->mat_ang_rad->D2[nh][1]  = smc_cnl.ang_grau[nh][1] * graurad;
         cnl->mat_corrente->D2[nh][2] = smc_cnl.pot_kva[nh][2];
         cnl->mat_ang_rad->D2[nh][2]  = smc_cnl.ang_grau[nh][2] * graurad;
         cnl->mat_corrente->D2[nh][3] = 0.;
         cnl->mat_ang_rad->D2[nh][3]  = 0.;
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddGrupoLTC12(void)
{
// Esta fun��o s� � usada por NR3

// Vari�veis locais
bool        *vet_flag_inserido;
int         f1, f2, f3, f4, f5, f6, num_REG12;
TBarra      *bar_ref;
TGrupoLTC12 *grupo1;
TReg12      *reg12, *reg12_aux;

// Inicializa��o
num_REG12 = rede3->lisREG12->Count;
if(num_REG12 == 0) return(true);

// Aloca e inicializa mem�ria para vetor tempor�rio de controle
vet_flag_inserido = new bool[num_REG12];
for(int nr=0; nr < num_REG12; nr++)
   {
   vet_flag_inserido[nr] = false; // Valor 'true' indica LTC j� inserido
   }

// Percorre lista de reguladores, identificando LTCs e a correspondente
// barra de tens�o controlada
// IMPORTANTE: Somente os reguladores do tipo PADR�O entram no Jacobiano
//             (esta fun��o s� serve ao NR3)
for(int nr=0; nr < num_REG12; nr++)
   {
   reg12 = (TReg12 *) rede3->lisREG12->Items[nr];
   if(vet_flag_inserido[nr])       continue;  // Descarta LTC j� inserido anteriormente
   if(! reg12->IsPADRAO())         continue;  // Descarta regulador n�o-PADR�O
   bar_ref = reg12->bar_ref_12;
   if(bar_ref == NULL)             continue;  // Prote��o
   reg12->FasesOrdenadas(2, &f1, &f2, &f3);

   // Cria grupo de LTCs e o insere na lista de grupos. Insere primeiro LTC no grupo.
   grupo1 = new TGrupoLTC12(bar_ref, f1, f2, f3, reg12->tipo);
   rede3->lisGRUPO_LTC12->Add(grupo1);
   grupo1->lisLTC->Add(reg12);
   vet_flag_inserido[nr] = true;              // Liga flag de LTC inserido

   // Inicializa 'vet_tap_prev'
   for(int nt=0; nt < reg12->NumTaps; nt++)
      {
      reg12->vet_tap_prev[nt] = reg12->vet_tap[nt];
      }

   // Verifica nos reguladores � frente se h� outros reguladores com a
   // mesma barra controlada, inserindo-os em caso afirmativo
   for(int nr2=(nr+1); nr2 < num_REG12; nr2++)
      {
      reg12_aux = (TReg12 *) rede3->lisREG12->Items[nr2];
      if(reg12_aux->bar_ref_12 != bar_ref) continue; // Descarta regulador irrelevante
      if(vet_flag_inserido[nr2])           continue; // Descarta regulador j� inserido anteriormente
      if(! reg12_aux->IsPADRAO())          continue; // Descarta regulador n�o-PADR�O
      if(reg12_aux->tipo != reg12->tipo)   continue; // Descarta tipo diferente do tipo de 'reg12'
      reg12_aux->FasesOrdenadas(2, &f4, &f5, &f6);
      if((f1 != f4) || (f2 != f5) ||
         (f3 != f6))                     continue; // Descarta regulador com fases distintas
      grupo1->lisLTC->Add(reg12_aux);
      vet_flag_inserido[nr2] = true;               // Liga flag de LTC inserido
   // Inicializa 'vet_tap_prev'
      for(int nt=0; nt < reg12->NumTaps; nt++)
         {
         reg12_aux->vet_tap_prev[nt] = reg12_aux->vet_tap[nt];
         }
      }
   }

// Libera mem�ria
delete[] vet_flag_inserido;

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddGrupoLTC32(void)
{
// Esta fun��o s� � usada por NR3

// Vari�veis locais
bool        *vet_flag_inserido;
int         num_REG32;
TBarra      *bar_ref;
TGrupoLTC32 *grupo1;
TReg32      *reg32, *reg32_aux;

// Inicializa��o
num_REG32 = rede3->lisREG32->Count;
if(num_REG32 == 0) return(true);

// Aloca e inicializa mem�ria para vetor tempor�rio de controle
vet_flag_inserido = new bool[num_REG32];
for(int nr=0; nr < num_REG32; nr++)
   {
   vet_flag_inserido[nr] = false;  // Valor 'true' indica LTC j� inserido
   }

// Percorre lista de reguladores, identificando LTCs e a correspondente
// barra de tens�o controlada
// IMPORTANTE: Somente os reguladores do tipo PADR�O entram no Jacobiano
//             (esta fun��o s� serve ao NR3)
for(int nr=0; nr < num_REG32; nr++)
   {
   reg32 = (TReg32 *) rede3->lisREG32->Items[nr];
   if(vet_flag_inserido[nr])        continue; // Descarta LTC j� inserido anteriormente
   if(! reg32->IsPADRAO())          continue; // Descarta regulador n�o-PADR�O
   bar_ref = reg32->bar_ref_12;               // S� verifica sentido 1 -> 2
   if(bar_ref == NULL)              continue; // Prote��o

   // Cria grupo de LTCs e o insere na lista de grupos. Insere primeiro LTC no grupo.
   grupo1 = new TGrupoLTC32(bar_ref, reg32->tipo);
   rede3->lisGRUPO_LTC32->Add(grupo1);
   grupo1->lisLTC->Add(reg32);
   vet_flag_inserido[nr] = true;       // Liga flag de LTC inserido

   // Inicializa 'vet_tap_prev'
   for(int nt=0; nt < reg32->NumTaps(); nt++)
      {
      reg32->vet_tap_prev[nt] = reg32->vet_tap[nt];
      }

   // Verifica nos reguladores � frente se h� outros reguladores com a
   // mesma barra controlada, inserindo-os em caso afirmativo
   for(int nr2=(nr+1); nr2 < num_REG32; nr2++)
      {
      reg32_aux = (TReg32 *) rede3->lisREG32->Items[nr2];
      if(reg32_aux->bar_ref_12 != bar_ref) continue; // Descarta regulador irrelevante
      if(vet_flag_inserido[nr2])           continue; // Descarta LTC j� inserido anteriormente
      if(! reg32_aux->IsPADRAO())          continue; // Descarta regulador n�o-PADR�O
      if(reg32_aux->tipo != reg32->tipo)   continue; // Descarta tipo diferente do tipo de 'reg32'
      grupo1->lisLTC->Add(reg32_aux);
      vet_flag_inserido[nr2] = true;                 // Liga flag de LTC inserido
      // Inicializa 'vet_tap_prev'
      for(int nt=0; nt < reg32_aux->NumTaps(); nt++)
         {
         reg32_aux->vet_tap_prev[nt] = reg32_aux->vet_tap[nt];
         }
      }
   }

// Libera mem�ria
delete[] vet_flag_inserido;

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddMedidorReal(void)
{
// Vari�veis locais
bool       sucesso;
int        fase_int, id_calc, num_medidor, tipo_eqpto, tipo_medicao;
smcMEDIDOR smc_medidor;
void       *pvoid;
TBarra     *bar1, *bar_ref;
TFase      *fases;
TMedidor   *med1;
TTrafo32   *trafo32;
TTrecho    *trec1;

// Inicializa��o
sucesso = true;

// Determina n�mero de Medidores
num_medidor = data->NumeroMedidor;

// Loop para todos Medidores
for(int nm=0; nm < num_medidor; nm++)
   {//obt�m dados do Medidor
   if(! data->EqptoMedidor(nm, &smc_medidor)) {sucesso = false; break;}
   // Localiza a barra onde est� instalado o medidor
   bar_ref = (TBarra *) smc_medidor.barraCALC;

   // Loop p/ todos Canais do Medidor
   for(int nc=0; nc < smc_medidor.numero_canal; nc++)
      {
      tipo_eqpto   = smc_medidor.smc_canal[nc].tipo_eqpto;
      tipo_medicao = smc_medidor.smc_canal[nc].tipo_medicao;
      fase_int     = smc_medidor.smc_canal[nc].fase_int;

      // Analisa tipo do equipamento medido
      switch(tipo_eqpto)
         {
         case teBARRA:
            bar1 = rede3->LocalizaBarraId(smc_medidor.smc_canal[nc].eqpto_id);
            if(bar1 == NULL)                  {sucesso = false; break;}
            fases = bar1->fases;


// TEMPOR�RIO, DEVIDO A PROBLEMAS COM FASES
            // if(! fases->ExisteFase(fase_int)) {sucesso = false; break;}
            if(! fases->ExisteFase(fase_int)) continue;


            pvoid   = bar1;
            id_calc = bar1->id;
            break;

         case teTRECHO:
            trec1 = rede3->LocalizaTrechoId(smc_medidor.smc_canal[nc].eqpto_id);
            if(trec1 == NULL)                 {sucesso = false; break;}
            fases = trec1->fases;
            if(! fases->ExisteFase(fase_int)) {sucesso = false; break;}
            pvoid   = trec1;
            id_calc = trec1->id;
            break;

         case teTRAFO:
            trafo32 = rede3->LocalizaTrafo32Id(smc_medidor.smc_canal[nc].eqpto_id);
            if(trafo32 == NULL)               {sucesso = false; break;}
            if(bar_ref == trafo32->bar1) fases = trafo32->fases_pri;
            else                         fases = trafo32->fases_sec;
            if(! fases->ExisteFase(fase_int)) {sucesso = false; break;}
            pvoid   = trafo32;
            id_calc = trafo32->id;
            break;

         default:  // Erro
            sucesso = false;
            break;
         } // switch()
      if(! sucesso)
         {
         break;
         }

      // Descarta medidor repetido
      if(rede3->ExisteMedidorReal(bar_ref, tipo_eqpto, tipo_medicao, id_calc,
                                                            fase_int)) continue;

      // Cria medidor
      med1 = new TMedidor;
      rede3->lisMED->Add(med1);
      med1->snp_canal    = smc_medidor.smc_canal[nc].canal;
      med1->codigo       = smc_medidor.smc_canal[nc].codigo;
      med1->enabled      = smc_medidor.smc_canal[nc].enabled;
      med1->eqpto_id     = smc_medidor.smc_canal[nc].eqpto_id;
      med1->eqpto        = pvoid;
      med1->tipo_eqpto   = tipo_eqpto;
      med1->tipo_medicao = tipo_medicao;
      med1->id_calc      = id_calc;
      med1->bar_ref      = bar_ref;
      med1->fase_int     = fase_int;
      med1->toler_pu     = 0.01 * smc_medidor.smc_canal[nc].tolerancia_perc;

      // Calcula e salva valor de fundo de escala em pu
      med1->CalculaFundoDeEscala3(bar_ref->VnomFase_kv(fase_int), geralC->Sbase_fase,
                                  geralC->semi_intervalo_normal,
                                  smc_medidor.smc_canal[nc].valor_fe);
      } // for(nc)
   if(! sucesso) break;
   } // for(nm)

// Define 'geralC->num_med_reais'
geralC->num_med_reais = rede3->lisMED->Count;

return(sucesso);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddMedidorVirtual(void)
{
// Vari�veis locais
int        fase_int, num_fases, num_tipos, tipo_medicao;
AnsiString str1;
TBarra     *bar1;
TFase      *fases;
TMedidor   *med1;

// Loop de barras
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1      = (TBarra *) rede3->lisBAR->Items[nb];
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   if(bar1->TipoCarga()) num_tipos = 2;  // Barra de carga: medP e medQ
   else                  num_tipos = 1;  // Suprimento:     medMOD_V

   // Loop dos tipos de medi��o
   for(int nt=0; nt < num_tipos; nt++)
      {
      if(nt == 0)
         {
         if(bar1->TipoCarga()) tipo_medicao = medP;
         else                  tipo_medicao = medMOD_V;
         }
      else
         {
         tipo_medicao = medQ;
         }

      // Verifica se deve incluir medidor de m�dulo de tens�o
      if(tipo_medicao == medMOD_V)
         {
         if(! IncluirMedidorModV(bar1)) continue;
         }

      // Loop de fases
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         fase_int = fases->Fase(nfa);

         // N�o insere medidor virtual em neutro (2016.01.07, HPS)
         if(fase_int == faseN) continue;

         // N�o insere medidor virtual se j� existir medidor real
         if(rede3->ExisteMedidorReal(bar1, teBARRA, tipo_medicao, bar1->id,
                                                            fase_int)) continue;

         // Insere medidor virtual
         med1 = new TMedidor;
         rede3->lisMED->Add(med1);
         str1.sprintf("Virtual_%d", (rede3->lisMED->Count - geralC->num_med_reais));
         med1->codigo       = str1;
         med1->eqpto        = (void *) bar1;
         med1->tipo_eqpto   = teBARRA;
         med1->tipo_medidor = medVIRTUAL;
         med1->tipo_medicao = tipo_medicao;
         med1->id_calc      = bar1->id;
         med1->bar_ref      = bar1;
         med1->fase_int     = fase_int;
         } // for(nfa)
      } // for(nt)
   } // for(nb)

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddMutua(void)
   {
   // Vari�veis locais
   bool        sucesso;
   int         count;
   TGrupoMutua *grupo;
   TList       *lisMUTUA_TEMP;
   TMutua      *mutua;
   smcMUTUA    smc_mutua;

   // Inicializa��o
   count = data->NumeroMutua;
   if(count == 0) return(true);
   sucesso = true;

   // Cria e preenche 'lisMUTUA_TEMP'
   lisMUTUA_TEMP = new TList;
   for(int n=0; n < count; n++)
      {
      if(! data->EqptoMutua(n, &smc_mutua))
         {
         sucesso = false;
         break;
         }
      // Inclui m�tua na lista tempor�ria 'lisMUTUA_TEMP'
      if(! IncluiMutua(&smc_mutua, lisMUTUA_TEMP))
         {
         sucesso = false;
         break;
         }
      }

   // Ordena trechos envolvidos em m�tua ('lisMUTUA_TEMP') e monta
   // grupos de m�tuas
   if(sucesso)
      {
      sucesso = OrdenaMutuas(lisMUTUA_TEMP);
      }

   // Destroi 'lisMUTUA_TEMP'
   rede3->DeletaLista(&lisMUTUA_TEMP);

   // Retorna se houve erro
   if(! sucesso) return(false);

   // Monta matriz de imped�ncias de todas as m�tuas e de
   // de todos os grupos de m�tuas
   for(int ng=0; ng < rede3->lisGRUPO_MUTUA->Count; ng++)
      {
      grupo = (TGrupoMutua *) rede3->lisGRUPO_MUTUA->Items[ng];
      for(int nm=0; nm < grupo->lisMUTUA->Count; nm++)
         {
         mutua = (TMutua *) grupo->lisMUTUA->Items[nm];
         if(! mutua->MontaMatrizImpedancias()) return(false);
         }
      if(! grupo->MontaMatZserie(geralC)) return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddReg(void)
   {
   // Vari�veis locais
   bool       flag_regulador;
   int        count;
   smcTRAFO2E trafo;

   // Loop para todos os reguladores, monof�sicos e trif�sicos
   count = data->NumeroRegulador;
   for(int n = 0; n < count; n++)
      {
      if(! data->EqptoRegulador(n, &trafo)) return(false);

      // Separa trafo de regulador
      if(trafo.ltc.pri_sec == ltcNAO_USADOX) flag_regulador = false; // Trafo
      else                                   flag_regulador = true;  // Regulador

      // Verifica se equipamento 1F ou 3F
      if(trafo.monofasico)
         {
         if(! AddTrafo12(&trafo, flag_regulador)) return(false);
         }
      else  // 3F
         {
         if(! AddTrafo32(&trafo, flag_regulador)) return(false);
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddSuprimento(void)
   {
   // Vari�veis locais
   bool            retorno;
   int             count, tipo, tipo_orig;
   double          pcto1, pcto3, qcto1, qcto3, rp, sbinv, xp;
   complex<double> s3f, scurto_default, sft, z0, z1;
   TBarra          *bar1;
   TSup            *sup1;
   smcGERADOR      ger;
   smcSUPRIMENTO   sup;

   // Inicializa��o
   scurto_default = complex<double>(0., 5.e6);  // Default [MVA]

   // Loop para todos os Suprimentos
   count = data->NumeroSuprimento;
   for(int n=0; n < count; n++)
      {
      if(! data->EqptoSuprimento(n, &sup)) return(false);
      // Prote��o
      if(sup.fases == faseINV) return(false);
      // Insere Suprimento em lisSUP
      bar1 = (TBarra *) sup.barraCALC;
      if(rede3->LocalizaSuprimentoBarraExt(bar1) != NULL) continue; // Somente um suprimento por barra
      tipo_orig = sup.tipo_sup;
      if     (tipo_orig == supPV) tipo = BAR_PV;
      else if(tipo_orig == supPQ) tipo = BAR_PQ;
      else                        tipo = BAR_SW;
      sup1 = new TSup(bar1, tipo, sup.fases);
      rede3->lisSUP->Add(sup1);
      sup1->id       = sup.id;
      sup1->snom_mva = 100.;  // Valor fixo
      // Verifica valor das pot�ncias de curto-circuito,
      // aplicando valor de default se necess�rio
      sft = sup.scc.fase_terra_mva;
      s3f = sup.scc.trifasico_mva;
      if(IsComplexZero(sft)) sft = scurto_default;
      if(IsComplexZero(s3f)) s3f = scurto_default;
      // Salva pot�ncias de curto-circuito, por enquanto em [MVA] (ser�o
      // convertidas a [pu] logo abaixo, em fun��o do tipo de suprimento)
      sup1->pot_curto1_pu = sft;
      sup1->pot_curto3_pu = s3f;
      } // Loop de suprimentos

   // Loop para todos os Geradores
   count = data->NumeroGerador;
   for(int n=0; n < count; n++)
      {
      if(! data->EqptoGerador(n, &ger)) return(false);
      // Prote��o
      if(ger.fases == faseINV) return(false);
      // Insere Gerador em lisSUP
      bar1 = (TBarra *) ger.barraCALC;
      if(rede3->LocalizaSuprimentoBarraExt(bar1) != NULL) continue; // Somente um suprimento por barra
      tipo_orig = ger.tipo_sup;
      if     (tipo_orig == supPV) tipo = BAR_PV;
      else if(tipo_orig == supPQ) tipo = BAR_PQ;
      else                        tipo = BAR_SW;


// DEBUG GERADORES BCN, CAN, BN E CN - IN�CIO
// O Editor n�o permite definir as configura��es abaixo
// ger.fases = faseBCN;  // Caso GeradorBCN+PV
// ger.fases = faseCAN;  // Caso GeradorCAN+PV
// ger.fases = faseBN;   // Caso GeradorBN+PV_TrafoBN+AB
// ger.fases = faseCN;   // Caso GeradorCN+PV_TrafoCN+AB
// DEBUG GERADORES BCN, CAN, BN E CN - FIM


      sup1 = new TSup(bar1, tipo, ger.fases);
      rede3->lisSUP->Add(sup1);
      sup1->id       = ger.id;
      sup1->snom_mva = ger.snom_mva;
      sup1->fp_min   = ger.fp_min;
      // Verifica valor das pot�ncias de curto-circuito,
      // aplicando valor de default se necess�rio
      sft = ger.scc.fase_terra_mva;
      s3f = ger.scc.trifasico_mva;
      if(IsComplexZero(sft)) sft = scurto_default;
      if(IsComplexZero(s3f)) s3f = scurto_default;
      // Salva pot�ncias de curto-circuito, por enquanto em [MVA] (ser�o
      // convertidas a [pu] logo abaixo, em fun��o do tipo de gerador)
      sup1->pot_curto1_pu = sft;
      sup1->pot_curto3_pu = s3f;
      } // Loop de geradores

   // Calcula par�metros adicionais dos suprimentos
   for(int ns=0; ns < rede3->lisSUP->Count; ns++)
      {
      sup1 = (TSup *) rede3->lisSUP->Items[ns];

      // Tratamento espec�fico para cada tipo de suprimento
      switch(sup1->num_fases_sem_NT)
         {
         case 1:  // Suprimento monof�sico
            // 'pot_curto1_pu' cont�m a pot�ncia   de curto-circuito
            // 'zp_pu'         cont�m a imped�ncia de curto-circuito, a qual
            //                 inclui a resist�ncia de aterramento
            // Converte pot�ncia de curto-circuito de [MVA] a [pu]
            sup1->pot_curto1_pu /= geralC->Sbase_fase;  // NB: Sbase_fase !!
            sup1->zp_pu          = cum / conj(sup1->pot_curto1_pu);
            break;

         case 2:  // Suprimento bif�sico
            // Converte pot�ncias de curto-circuito de [MVA] a [pu]
            sbinv                = 1. / geralC->Sbase_fase;  // NB: Sbase_fase !!
            sup1->pot_curto1_pu *= sbinv;  // Pot. curto fase-terra
            sup1->pot_curto3_pu *= sbinv;  // Pot. curto BIF�SICO, na verdade
            // As imped�ncias abaixo j� est�o em [pu] na base {Sbf, Vbf}
            z1          = 2. / conj(sup1->pot_curto3_pu);
            z0          = (4. / conj(sup1->pot_curto1_pu)) - z1;
            rp          = z1.real();
            xp          = (z0.imag() + z1.imag()) / 2.;
            sup1->zp_pu = complex<double>(rp, xp);
            sup1->rt_pu = (z0.real() - z1.real()) / 2.;
            sup1->xm_pu = (z0.imag() - z1.imag()) / 2.;
            break;

         case 3:  // Suprimento trif�sico
            // IMPORTANTE: no c�lculo dos par�metros 'rt_pu', 'xm_pu' e 'zp_pu'
            //             foi assumido que as 3 tens�es nominais de fase (A, B, C)
            //             s�o iguais � tens�o nominal de linha dividida por raiz(3)
            //             [cf. TMonta3::AddTrechoInternoSuprimento()]
            // Converte pot�ncias de curto-circuito de [MVA] a [pu]
            sbinv                = 1. / geralC->Sbase;  // NB: Sbase (3F) !!
            sup1->pot_curto1_pu *= sbinv;
            sup1->pot_curto3_pu *= sbinv;
            z1                   = cum / conj(sup1->pot_curto3_pu);
            z0                   = (3. / conj(sup1->pot_curto1_pu)) - (2. * z1);
            rp                   = z1.real();
            xp                   = (z0.imag() + (2. * z1.imag())) / 3.;
            sup1->zp_pu          = complex<double>(rp, xp);
            sup1->rt_pu          = (z0.real() - z1.real()) / 3.;
            sup1->xm_pu          = (z0.imag() - z1.imag()) / 3.;
            break;

         default:  // Erro
            return(false);
         }
      } // for(ns)

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddTrafo(void)
   {
   // Vari�veis locais
   bool       flag_regulador;
   int        count;
   smcTRAFO2E trafo;

   // Loop para todos os trafos 2E (equivalentes, reais 1F e reais 3F)
   count = data->NumeroTrafo2E;
   for(int n = 0; n < count; n++)
      {
      if(! data->EqptoTrafo2E(n, &trafo)) return(false);

      // Verifica se � trafo equivalente (gerado por redu��o de rede)
      if(trafo.reducao)  // Trafo equivalente
         {
         if(! AddTrafoEquiv(&trafo)) return(false);
         }
      else  // Equipamento real - separa trafo de regulador
         {
         if(trafo.ltc.pri_sec == ltcNAO_USADOX) flag_regulador = false; // Trafo
         else                                   flag_regulador = true;  // Regulador
         // Verifica se equipamento 1F ou 3F
         if(trafo.monofasico)
            {
            if(! AddTrafo12(&trafo, flag_regulador)) return(false);
            }
         else  // 3F
            {
            if(! AddTrafo32(&trafo, flag_regulador)) return(false);
            }
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddTrafo12(smcTRAFO2E *trafo,
                                    bool       flag_regulador)
   {
   // Vari�veis locais
   bool            flagT_pri, flagT_sec;
   int             fase_comum, num_fases_pri, num_fases_sec, tipo;
   double          fmult, x_pu, zbase;
   complex<double> y1_pu;
   TBarra          *bar1, *bar2;
   TReg12          *reg12;
	TTrafo12        *trafo12;

   // Prote��o
   if(trafo->pri.fase == faseINV) return(false);
   if(trafo->sec.fase == faseINV) return(false);

   // Inicializa��o
   fase_comum = faseINV;

   // Obt�m barras terminais do trafo
   bar1 = (TBarra *) trafo->barraCALC_1;
   bar2 = (TBarra *) trafo->barraCALC_2;
   if(bar1 == bar2) return(true);  // Prote��o

   // Cria objeto TReg12 ou TTrafo12 e o insere na lista correspondente
   if(flag_regulador)
      {
      reg12 = new TReg12(geralC, bar1, bar2, trafo->pri.fase, trafo->sec.fase);
      rede3->lisREG12->Add(reg12);
      trafo12 = reg12;
      }
   else
      {
      trafo12 = new TTrafo12(geralC, bar1, bar2, trafo->pri.fase, trafo->sec.fase);
      rede3->lisTRAFO12->Add(trafo12);
      reg12 = NULL;
      }

   // Obt�m n�mero de fases dos 2 enrolamentos
   num_fases_pri = trafo12->fases_pri->NumFases();
	num_fases_sec = trafo12->fases_sec->NumFases();

   // Define tipo do trafo/regulador
	if((num_fases_pri == 2) && (num_fases_sec == 2))      // 4 terminais
      {
		if(trafo->auto_trafo) tipo = AUTOTRAFO4;           // AUTOTRAFO4
		else                  tipo = TRAFO4;               // TRAFO4
      }
	else if((num_fases_pri == 2) && (num_fases_sec == 3)) // 5 terminais
      {
		tipo = TRAFO5;                                     // TRAFO5
      }
	else if((num_fases_pri == 3) && (num_fases_sec == 3)) // 6 terminais
      {
		// Escolher abaixo a implementa��o ativa (1 tap ou 2 taps)
      // tipo = AUTOTRAFO6_1T;
      tipo = AUTOTRAFO6_2T;
      if(trafo->fase_comum <= 0) fase_comum = faseN;
      else                       fase_comum = trafo->fase_comum;
      }
   else  // Erro
      {
      return(false);
      }

   // Define aterramento (trafo e barras terminais)
   flagT_pri = trafo->pri.fase & faseT;
   flagT_sec = trafo->sec.fase & faseT;
   trafo12->DefineAterramento(flagT_pri, flagT_sec, trafo->pri.zater_ohm,
                              trafo->sec.zater_ohm, geralC);

   // Preenche demais dados
   trafo12->vnom1_kv   = trafo->pri.vnom_kv;
	trafo12->vnom2_kv   = trafo->sec.vnom_kv;
   trafo12->vet_tap[0] = trafo->sec.tap_kv / trafo->pri.tap_kv; // Modelo 1:alfa
	trafo12->vet_tap[1] = trafo12->vet_tap[0];
	trafo12->id         = trafo->id;
   trafo12->eqpto      = trafo->eqpto;
   trafo12->tipo       = tipo;
   trafo12->fase_comum = fase_comum;
   trafo12->codigo     = rede3->AtribuiCodigoLigacao(trafo->codigo, bar1, bar2);
   trafo12->snom_mva   = trafo->snom_mva;

   // Calcula e salva imped�ncia de curto e admit�ncia do ramo paralelo
   x_pu = 0.01 * trafo12->snom_mva;  // [1 * snom_mva / 100]; valor de default
   if(IsComplexZero(trafo->z1_pu)) trafo->z1_pu = complex<double>(0., x_pu);
   if((trafo12->tipo == AUTOTRAFO4) || (trafo12->tipo == AUTOTRAFO6_1T) ||
      (trafo12->tipo == AUTOTRAFO6_2T))
      {
      zbase = trafo12->vnom2_kv * trafo12->vnom2_kv / trafo12->snom_mva; // Ref. secund�rio
      }
   else  // TRAFO4 ou TRAFO5
      {
      zbase = trafo12->vnom1_kv * trafo12->vnom1_kv / trafo12->snom_mva; // Ref. prim�rio
		}
   trafo12->ztrafo_ohm = trafo->z1_pu * zbase;
   // ATEN��O: 1. Perda no ferro est� em % de Snom
   //          2. Reat�ncia de magnetiza��o ignorada na linha abaixo
   trafo12->ypar_S = complex<double>((0.01 * trafo->perda_ferro / zbase), 0.);

   // Acerta n�mero de taps e corrige admit�ncia do ramo paralelo para AUTOTRAFO6
   // (duas pernas no modelo + possibilidade de ser alimentado por tens�es de
   // fase ou de linha)
   if((trafo12->tipo == AUTOTRAFO6_1T) || (trafo12->tipo == AUTOTRAFO6_2T))
      {
		if(trafo12->tipo == AUTOTRAFO6_2T) trafo12->NumTaps = 2; // Acerta n�mero de taps
      trafo12->ypar_S *= 0.5;   // COM ou SEM neutro (duas pernas)
      if(trafo12->NeutroSec())  // COM neutro: acerto pela tens�o de fase
         {
         fmult            = trafo12->vnom2_kv / bar2->VnomFase_kv(trafo12->fases_sec->Fase(0));
         trafo12->ypar_S *= (fmult * fmult);
         }
      }

   // Insere dados espec�ficos do controle de tens�o (TReg12)
   if(reg12 != NULL)
      {
      reg12->modelo             = trafo->ltc.modelo;
      reg12->sub_modelo         = trafo->ltc.sub_modelo;
      reg12->banda_morta_pu     = trafo->ltc.banda_morta_pu;     // "Dead band" - sentido direto
      reg12->banda_morta_inv_pu = trafo->ltc.banda_morta_inv_pu; // "Dead band" - sentido reverso
      reg12->toler_tap_pu       = geralC->toler_tap_pu;
      // Inicializa��o de taps anteriores
      for(int nt=0; nt < reg12->NumTaps; nt++)
         {
         reg12->vet_tap_prev[nt] = reg12->vet_tap[nt];
			}
      reg12->bar_ref_12 = rede3->LocalizaBarraId(trafo->ltc.barra_id_12);
      if(reg12->bar_ref_12 == NULL) return(false);
      // Verifica condi��es para o regulador ser PADRAO - somente NR3
      if((geralC->Programa1 == progNR3) && reg12->IsPADRAO())
         {
         if(! reg12->TipoImplementadoNR3())
            {
            reg12->modelo     = regDOIS_SENTIDOS;
            reg12->sub_modelo = regAUTO_BI;
            }
         }
      // Analisa modelo do regulador
      if(reg12->IsDOIS_SENTIDOS()) // DOIS_SENTIDOS
         {
         reg12->bar_ref_21 = rede3->LocalizaBarraId(trafo->ltc.barra_id_21);
         if(reg12->bar_ref_21 == NULL) return(false);
         reg12->vref_12_pu    = trafo->ltc.v_pu_12;            // pu
         reg12->vref_21_pu    = trafo->ltc.v_pu_21;            // pu
         reg12->var_tensao_pu = 0.01 * trafo->ltc.var_tensao;  // [%] -> [pu]
         reg12->num_passo     = trafo->ltc.num_passo;
         }
      else if(reg12->IsENERQCT()) // ENERQCT - assume valores n�o fornecidos pela IHM
         {
         reg12->vref_12_pu    = 1.;   // pu
         reg12->var_tensao_pu = 0.2;  // pu
         reg12->num_passo     = 32;
         }
      else if(reg12->IsPADRAO()) // PADRAO
         {
         reg12->bar_ref_12->tipo = BAR_PQV;  // Redefine tipo da barra
         reg12->vref_12_pu       = trafo->ltc.v_pu_12;
         reg12->var_tensao_pu    = 0.01 * trafo->ltc.var_tensao;  // [%] -> [pu]
         reg12->num_passo        = trafo->ltc.num_passo;
         }
      else // Erro
         {
         return(false);
         }
      } // if(reg12)

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddTrafo32(smcTRAFO2E *trafo,
                                    bool       flag_regulador)
   {
   // Vari�veis locais
   int             id1, id2, lig1, lig2, num_fases_pri, num_fases_sec,
                   tipo, tipo_lig1, tipo_lig2;
   double          tap, x_pu, zbase;
   complex<double> y1_pu;
   TBarra          *bar1, *bar2;
   TReg32          *reg32;
   TTrafo32        *trafo32;

   // Obt�m barras terminais do trafo
   bar1 = (TBarra *) trafo->barraCALC_1;
   bar2 = (TBarra *) trafo->barraCALC_2;
   if(bar1 == bar2) return(true);  // Prote��o

   // Obt�m tipo das liga��es
   lig1 = trafo->pri.ligacao;
   lig2 = trafo->sec.ligacao;

   // Define tipo do equipamento (AUTOTRAFO_DF, AUTOTRAFO_Y, TRAFO_DD, TRAFO_DY1,
   // TRAFO_DY11, TRAFO_YD1, TRAFO_YD11 ou TRAFO_YY) e define n�mero de fases
   if(((lig1 == ligacaoTRIANGULO) || (lig1 == ligacaoDELTA_FECHADO)) &&
      ((lig2 == ligacaoTRIANGULO) || (lig2 == ligacaoDELTA_FECHADO))) // DD
      {
      if(trafo->auto_trafo)
         {
         tipo = AUTOTRAFO_DF;
         // Verifica se as tens�es nominais das barras terminais s�o iguais
         // (no AutoDF, � erro se n�o forem)
         if(! IsDoubleZero(bar1->VnomLinha_kv() - bar2->VnomLinha_kv())) return(false);
         }
      else
         {
         tipo = TRAFO_DD;
         }
      tipo_lig1     = enTRIANGULO;
      tipo_lig2     = enTRIANGULO;
      num_fases_pri = MAX_FASES_G3 - 1;
      num_fases_sec = MAX_FASES_G3 - 1;
      }
   else if(((lig1 == ligacaoTRIANGULO) || (lig1 == ligacaoDELTA_FECHADO)) &&
           ((lig2 == ligacaoESTRELA)   || (lig2 == ligacaoESTRELA_ATERRADO))) // DY
      {
      if(trafo->grupo_lig == glDy11) tipo = TRAFO_DY11;
      else                           tipo = TRAFO_DY1;  // Default
      tipo_lig1     = enTRIANGULO;
      num_fases_pri = MAX_FASES_G3 - 1;
      if(lig2 == ligacaoESTRELA)
         {
         num_fases_sec = MAX_FASES_G3 - 1;
         tipo_lig2     = enESTRELA_ISOLADA;
         }
      else
         {
         num_fases_sec = MAX_FASES_G3;
         tipo_lig2     = enESTRELA_ATERRADA;
         }
      }
   else if(((lig1 == ligacaoESTRELA)   || (lig1 == ligacaoESTRELA_ATERRADO)) &&
           ((lig2 == ligacaoTRIANGULO) || (lig2 == ligacaoDELTA_FECHADO))) // YD
      {
      if(trafo->grupo_lig == glYd1) tipo = TRAFO_YD1;
      else                          tipo = TRAFO_YD11;  // Default
      tipo_lig2     = enTRIANGULO;
      num_fases_sec = MAX_FASES_G3 - 1;
      if(lig1 == ligacaoESTRELA)
         {
         num_fases_pri = MAX_FASES_G3 - 1;
         tipo_lig1     = enESTRELA_ISOLADA;
         }
      else
         {
         num_fases_pri = MAX_FASES_G3;
         tipo_lig1     = enESTRELA_ATERRADA;
         }
      }
   else // YY
      {
      if(trafo->auto_trafo)  // AUTOTRAFO_Y
         {
         tipo = AUTOTRAFO_Y;

         // Linha abaixo HABILITADA   em 2011.08.18: problemas em rede da AES-SUL
         // Linha abaixo DESABILITADA em 2015......
         // Linha abaixo HABILITADA   em 2015.05.21: problemas em rede da CERTAJA
         trafo->pri.ligacao = ligacaoESTRELA_ATERRADO;

         // Garante mesma liga��o (ver chamada de 'DefineAterramento()' abaixo)
         lig1 = lig2 = trafo->sec.ligacao = trafo->pri.ligacao;

         if(lig1 == ligacaoESTRELA)
            {
            // ABC/ABC: o neutro comum ser� eliminado na montagem de Ynodal
            num_fases_pri = MAX_FASES_G3 - 1;
            num_fases_sec = MAX_FASES_G3 - 1;
            tipo_lig1     = enESTRELA_ISOLADA;
            tipo_lig2     = enESTRELA_ISOLADA;
            }
         else  // ABCN/ABCN: o neutro comum ser� mantido na montagem de Ynodal
            {
            num_fases_pri = MAX_FASES_G3;
            num_fases_sec = MAX_FASES_G3;
            tipo_lig1     = enESTRELA_ATERRADA;
            tipo_lig2     = enESTRELA_ATERRADA;
            }
         }
      else  // TRAFO_YY
         {
         tipo = TRAFO_YY;
         if(lig1 == ligacaoESTRELA)
            {
            num_fases_pri = MAX_FASES_G3 - 1;
            tipo_lig1     = enESTRELA_ISOLADA;
            }
         else
            {
            num_fases_pri = MAX_FASES_G3;
            tipo_lig1     = enESTRELA_ATERRADA;
            }
         if(lig2 == ligacaoESTRELA)
            {
            num_fases_sec = MAX_FASES_G3 - 1;
            tipo_lig2     = enESTRELA_ISOLADA;
            }
         else
            {
            num_fases_sec = MAX_FASES_G3;
            tipo_lig2     = enESTRELA_ATERRADA;
            }
         }
      }

   // Cria objeto TReg32 ou TTrafo32 e o insere na lista correspondente
   if(flag_regulador)
      {
      reg32 = new TReg32(tipo, num_fases_pri, num_fases_sec, bar1, bar2);
      rede3->lisREG32->Add(reg32);
      trafo32 = reg32;
      }
   else
      {
      trafo32 = new TTrafo32(tipo, num_fases_pri, num_fases_sec, bar1, bar2);
      rede3->lisTRAFO32->Add(trafo32);
      reg32 = NULL;
      }

   // Define aterramento (trafo e barras terminais)
   trafo32->DefineAterramento(trafo, geralC);

   // Preenche demais dados
   trafo32->eqpto      = trafo->eqpto;
   trafo32->codigo     = rede3->AtribuiCodigoLigacao(trafo->codigo, bar1, bar2);
   trafo32->auto_trafo = trafo->auto_trafo;
   trafo32->id         = trafo->id;
   trafo32->tipo_lig1  = tipo_lig1;
   trafo32->tipo_lig2  = tipo_lig2;
   trafo32->vnom1_kv   = trafo->pri.vnom_kv;
   trafo32->vnom2_kv   = trafo->sec.vnom_kv;
   trafo32->snom_mva   = trafo->snom_mva;
   tap                 = trafo->sec.tap_kv / trafo->pri.tap_kv;
   trafo32->vet_tap[0] = tap;
   trafo32->vet_tap[1] = tap;
   trafo32->vet_tap[2] = tap;

   // Calcula e salva imped�ncia de curto e admit�ncia do ramo paralelo
   x_pu = 0.01 * trafo32->snom_mva;  // [1 * snom_mva / 100]; valor de default
   if(IsComplexZero(trafo->z1_pu)) trafo->z1_pu = complex<double>(0., x_pu);
   zbase               = trafo32->vnom2_kv * trafo32->vnom2_kv / trafo32->snom_mva; // Ref. secund�rio
   trafo32->ztrafo_ohm = trafo->z1_pu * zbase;
   // ATEN��O: 1. Perda no ferro est� em % de Snom
   //          2. 'ypar_s': valor por fase na liga��o Y
   //          3. Reat�ncia de magnetiza��o ignorada na linha abaixo
   trafo32->ypar_S = complex<double>((0.01 * trafo->perda_ferro / zbase), 0.);

   // Insere dados espec�ficos do controle de tens�o (TReg32)
   if(reg32 != NULL)
      {
      reg32->modelo             = trafo->ltc.modelo;
      reg32->sub_modelo         = trafo->ltc.sub_modelo;
      reg32->banda_morta_pu     = trafo->ltc.banda_morta_pu;     // "Dead band" - sentido direto
      reg32->banda_morta_inv_pu = trafo->ltc.banda_morta_inv_pu; // "Dead band" - sentido reverso
      reg32->toler_tap_pu       = geralC->toler_tap_pu;
      reg32->vet_tap_prev[0]    = tap;                           // Inicializa��o
      reg32->vet_tap_prev[1]    = tap;
      reg32->vet_tap_prev[2]    = tap;
      reg32->bar_ref_12         = rede3->LocalizaBarraId(trafo->ltc.barra_id_12);
      if(reg32->bar_ref_12 == NULL) return(false);
      // Verifica condi��es para o regulador ser PADRAO - somente NR3
      if((geralC->Programa1 == progNR3) && reg32->IsPADRAO())
         {
         if(! reg32->TipoImplementadoNR3())
            {
            reg32->modelo     = regDOIS_SENTIDOS;
            reg32->sub_modelo = regAUTO_BI;
            }
         }
      // Analisa modelo do regulador
      if(reg32->IsDOIS_SENTIDOS()) // DOIS_SENTIDOS
         {
         reg32->bar_ref_21 = rede3->LocalizaBarraId(trafo->ltc.barra_id_21);
         if(reg32->bar_ref_21 == NULL) return(false);
         reg32->vref_12_pu    = trafo->ltc.v_pu_12;            // pu
         reg32->vref_21_pu    = trafo->ltc.v_pu_21;            // pu
         reg32->var_tensao_pu = 0.01 * trafo->ltc.var_tensao;  // [%] -> [pu]
         reg32->num_passo     = trafo->ltc.num_passo;
         }
      else if(reg32->IsENERQCT()) // ENERQCT - assume valores n�o fornecidos pela IHM
         {
         reg32->vref_12_pu    = 1.;   // pu
         reg32->var_tensao_pu = 0.2;  // pu
         reg32->num_passo     = 32;
         }
      else if(reg32->IsPADRAO()) // PADRAO
         {
         reg32->bar_ref_12->tipo = BAR_PQV;  // Redefine tipo da barra
         reg32->vref_12_pu       = trafo->ltc.v_pu_12;
         reg32->var_tensao_pu    = 0.01 * trafo->ltc.var_tensao;  // [%] -> [pu]
         reg32->num_passo        = trafo->ltc.num_passo;
         }
      else // Erro
         {
         return(false);
         }
      } // if(reg32)

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddTrafo33(void)
   {
   // Vari�veis locais
   int        count, lig1, lig2, lig3, num_fases_pri,
              num_fases_sec, num_fases_ter, tipo, tipo_lig1, tipo_lig2,
              tipo_lig3;
   double     fmult, zbase;
   TBarra     *bar1, *bar2, *bar3;
   TTrafo33   *trafo33;
   smcTRAFO3E trafo;

   // Loop para todos os trafos 3E
   count = data->NumeroTrafo3E;
   for(int n=0; n < count; n++)
      {
      if(! data->EqptoTrafo3E(n, &trafo)) return(false);

      // Obt�m barras terminais do trafo
      bar1 = (TBarra *) trafo.barraCALC_1;
      bar2 = (TBarra *) trafo.barraCALC_2;
      bar3 = (TBarra *) trafo.barraCALC_3;
      if(bar1 == bar2) continue;  // Prote��o
      if(bar2 == bar3) continue;  // Prote��o
      if(bar3 == bar1) continue;  // Prote��o

      // Obt�m c�digo das liga��es
      lig1 = trafo.pri.ligacao;
      lig2 = trafo.sec.ligacao;
      lig3 = trafo.ter.ligacao;

      // Traduz tipo de liga��o e define n�mero de fases
      if(lig1 == ligacaoESTRELA_ATERRADO)
         {
         tipo_lig1 = enESTRELA_ATERRADA; num_fases_pri = MAX_FASES_G3;
         }
      else if((lig1 == ligacaoTRIANGULO) || (lig1 == ligacaoDELTA_FECHADO))
         {
         tipo_lig1 = enTRIANGULO; num_fases_pri = MAX_FASES_G3 - 1;
         }
      else  // Estrela isolada
         {
         tipo_lig1 = enESTRELA_ISOLADA; num_fases_pri = MAX_FASES_G3 - 1;
         }

      if(lig2 == ligacaoESTRELA_ATERRADO)
         {
         tipo_lig2 = enESTRELA_ATERRADA; num_fases_sec = MAX_FASES_G3;
         }
      else if((lig2 == ligacaoTRIANGULO) || (lig2 == ligacaoDELTA_FECHADO))
         {
         tipo_lig2 = enTRIANGULO; num_fases_sec = MAX_FASES_G3 - 1;
         }
      else  // Estrela isolada
         {
         tipo_lig2 = enESTRELA_ISOLADA; num_fases_sec = MAX_FASES_G3 - 1;
         }

      if(lig3 == ligacaoESTRELA_ATERRADO)
         {
         tipo_lig3 = enESTRELA_ATERRADA; num_fases_ter = MAX_FASES_G3;
         }
      else if((lig3 == ligacaoTRIANGULO) || (lig3 == ligacaoDELTA_FECHADO))
         {
         tipo_lig3 = enTRIANGULO; num_fases_ter = MAX_FASES_G3 - 1;
         }
      else  // Estrela isolada
         {
         tipo_lig3 = enESTRELA_ISOLADA; num_fases_ter = MAX_FASES_G3 - 1;
         }

      // Define tipo do trafo
      if((tipo_lig1 == enTRIANGULO) && (tipo_lig2 == enTRIANGULO) &&
         (tipo_lig3 != enTRIANGULO))
         {
         if(trafo.grupo_lig == glDD0Y11) tipo = TRAFO_DDY11;
         else                            tipo = TRAFO_DDY1;  // Default
         }
      else if((tipo_lig1 == enTRIANGULO) && (tipo_lig2 != enTRIANGULO) &&
              (tipo_lig3 != enTRIANGULO))
         {
         if     (trafo.grupo_lig == glDY11Y11) tipo = TRAFO_DY11Y11;
         else if(trafo.grupo_lig == glDY11Y1)  tipo = TRAFO_DY11Y1;
         else if(trafo.grupo_lig == glDY1Y11)  tipo = TRAFO_DY1Y11;
         else                                  tipo = TRAFO_DY1Y1;  // Default
         }
      else if((tipo_lig1 != enTRIANGULO) && (tipo_lig2 != enTRIANGULO) &&
              (tipo_lig3 != enTRIANGULO))
         {
         tipo = TRAFO_YYY;
         }
      else  // Tipo inv�lido
         {
         return(false);
         }

      // Cria objeto TTrafo33 e insere em lisTRAFO33
      trafo33 = new TTrafo33(tipo, num_fases_pri, num_fases_sec, num_fases_ter,
                             bar1, bar2, bar3);
      rede3->lisTRAFO33->Add(trafo33);
      trafo33->eqpto     = trafo.eqpto;
      trafo33->codigo    = rede3->AtribuiCodigoLigacao(trafo.codigo, bar1, bar2, bar3);
      trafo33->id        = trafo.id;
      trafo33->tipo_lig1 = tipo_lig1;
      trafo33->tipo_lig2 = tipo_lig2;
      trafo33->tipo_lig3 = tipo_lig3;
      trafo33->vnom1_kv  = trafo.pri.vnom_kv;
      trafo33->vnom2_kv  = trafo.sec.vnom_kv;
      trafo33->vnom3_kv  = trafo.ter.vnom_kv;
      trafo33->vtap1_kv  = trafo.pri.tap_kv;
      trafo33->vtap2_kv  = trafo.sec.tap_kv;
      trafo33->vtap3_kv  = trafo.ter.tap_kv;
      trafo33->snom1_mva = trafo.pri.snom_mva;
      trafo33->snom2_mva = trafo.sec.snom_mva;
      trafo33->snom3_mva = trafo.ter.snom_mva;

      // Determina os enrolamentos de refer�ncia
      trafo33->DeterminaEnrolamentosRef();
      fmult            = geralC->Sbase / trafo33->Sref_mva;
      trafo33->zps1_pu = trafo.zps1_pu * fmult;  // pu na base Sbase
      trafo33->zpt1_pu = trafo.zpt1_pu * fmult;
      trafo33->zst1_pu = trafo.zst1_pu * fmult;
      // ATEN��O: 1. Perda no ferro est� em % de Sref_mva
      //          2. 'ypar_pu': valor por fase na liga��o Y
      //          3. Reat�ncia de magnetiza��o ignorada na linha abaixo
      trafo33->ypar_pu = complex<double>((0.01 * trafo.perda_ferro / fmult), 0.); // pu na base Sbase

      // Define aterramento (trafo e barras terminais)
      trafo33->DefineAterramento(&trafo, geralC);

      // Salva identifica��o original dos enrolamentos (antes de
      // eventual rota��o em TData)
      trafo33->vet_enrol_orig[0] = trafo.pri.enrol_orig;
      trafo33->vet_enrol_orig[1] = trafo.sec.enrol_orig;
      trafo33->vet_enrol_orig[2] = trafo.ter.enrol_orig;
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddTrafoEquiv(smcTRAFO2E *trafo)
   {
   // Vari�veis locais
   bool            sucesso_pri, sucesso_sec;
   int             fi, fj, ind_i, ind_j, num_fases_pri, num_fases_sec;
   double          fator_zbase, Sbase_fase_inv, zbase;
   complex<double> cvalor_ij, cvalor_ji;
   TBarra          *bar1, *bar2;
   TFase           *fases_pri, *fases_sec;
   TTrafoEquiv     *trafoequiv;

   // Prote��o
   if(trafo->pri.fase == faseINV) return(false);
   if(trafo->sec.fase == faseINV) return(false);

   // Inicializa��o
   Sbase_fase_inv = 1. / geralC->Sbase_fase;

   // Obt�m barras terminais do trafo
   bar1 = (TBarra *) trafo->barraCALC_1;
   bar2 = (TBarra *) trafo->barraCALC_2;
   if(bar1 == bar2) return(true);  // Prote��o

   // Define valor de 'geralC->modelo_eqv' (s� no primeiro trafo)
   if(rede3->lisTRAFOEQUIV->Count == 0)
      {
      geralC->modelo_eqv = trafo->modelo_eqv;
      if((geralC->modelo_eqv != modeloEQV1) && (geralC->modelo_eqv != modeloEQV3))
                     geralC->modelo_eqv = modeloEQV1; // Default em caso de erro
      }

   // Cria objeto TTrafoEquiv e insere em lisTRAFOEQUIV
   trafoequiv = new TTrafoEquiv(bar1, bar2, trafo->pri.fase, trafo->sec.fase);
   rede3->lisTRAFOEQUIV->Add(trafoequiv);
   trafoequiv->eqpto      = trafo->eqpto;
   trafoequiv->codigo     = rede3->AtribuiCodigoLigacao(trafo->codigo, bar1, bar2);
   trafoequiv->modelo_eqv = geralC->modelo_eqv;

   // Copia submatrizes de admit�ncias NODAIS do trafo (Yij e Yji)
   // IMPORTANTE: os valores em [pu] s�o calculados usando a imped�ncia
   //             de base "h�brida" (com 'vnom1' e 'vnom2'), pois as
   //             submatrizes armazenadas s�o as que conectam 'bar1' e
   //             'bar2' (submatrizes fora do bloco diagonal)
   fases_pri     = trafoequiv->fases_pri;
   fases_sec     = trafoequiv->fases_sec;
   num_fases_pri = fases_pri->NumFases();
	num_fases_sec = fases_sec->NumFases();
   for(int i=0; i < num_fases_pri; i++)
      {
      ind_i       = fases_pri->IndiceABCN(i);
      fi          = fases_pri->Fase(i);
      fator_zbase = bar1->VnomFase_kv(fi) * Sbase_fase_inv;
      for(int j=0; j < num_fases_sec; j++)
         {
         ind_j     = fases_sec->IndiceABCN(j);
         fj        = fases_sec->Fase(j);
         zbase     = fator_zbase * bar2->VnomFase_kv(fj);
         cvalor_ij = trafo->mat_ynodal_ij_S[ind_i][ind_j] * zbase;  // [pu]
         cvalor_ji = trafo->mat_ynodal_ji_S[ind_j][ind_i] * zbase;  // [pu]
         trafoequiv->MatYnodal_ij_pu[i][j] = cvalor_ij;
         trafoequiv->MatYnodal_ji_pu[j][i] = cvalor_ji;
         }
      }

   // Copia admit�ncias nodais
   trafoequiv->Ynodal0_ij_pu = trafo->Ynodal0_ij_pu;
   trafoequiv->Ynodal0_ji_pu = trafo->Ynodal0_ji_pu;
   trafoequiv->Ynodal1_ij_pu = trafo->Ynodal1_ij_pu;
   trafoequiv->Ynodal1_ji_pu = trafo->Ynodal1_ji_pu;

   // Aterra cada neutro solidamente, quando o trafo equivalente contiver
   // um trafo real e cada neutro existir
   if(fabs(bar1->VnomLinha_kv() - bar2->VnomLinha_kv()) > 0.001) // Havia um trafo real aqui...
      {
      if(fases_pri->ExisteN()) bar1->DefineAterramento(atSolido);
      if(fases_sec->ExisteN()) bar2->DefineAterramento(atSolido);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddTrafoZZ(void)
   {
   // Vari�veis locais
   int             count;
   complex<double> z0_ohm, z0_pu;
   smcTRAFOZZ      trafo;
   TBarra          *bar1;
   TTrafoZZ        *trafozz;

   // Loop para todos os trafos ZZ
   count = data->NumeroTrafoZZ;
   for(int n=0; n < count; n++)
      {
      if(! data->EqptoTrafoZZ(n, &trafo)) return(false);

      // Obt�m barra do trafo
      bar1 = (TBarra *) trafo.barraCALC;

      // Calcula imped�ncia por fase do trafo em ohm
      z0_pu = trafo.z0_pu;
      if(IsComplexZero(z0_pu)) z0_pu = complex<double>(0., 0.01);  // Prote��o
      z0_ohm = z0_pu * trafo.vnom_kv * trafo.vnom_kv / trafo.snom_mva;

      // Cria objeto TTrafoZZ e insere em lisTRAFOZZ
      trafozz = new TTrafoZZ(bar1);
      rede3->lisTRAFOZZ->Add(trafozz);
      trafozz->eqpto        = trafo.eqpto;
      trafozz->codigo       = rede3->AtribuiCodigoLigacao(trafo.codigo, bar1);
      trafozz->id           = trafo.id;
      trafozz->vnom_kv      = trafo.vnom_kv;
      trafozz->snom_mva     = trafo.snom_mva;
      trafozz->z0_ohm       = z0_ohm;
      trafozz->admit_fase_S = cum / (3. * z0_ohm);  // Frequ�ncia fundamental

      // Define aterramento atSolido para a barra
      bar1->DefineAterramento(atSolido);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddTramo(void)
   {
   // Vari�veis locais
   int             count, ind_i, num_fases;
   complex<double> z1;
   TBarra          *bar1, *bar2;
   TFase           *fases;
   TTrecho         *trec1;
   smcTRAMO        tramo;

   // Loop para todos os tramos
   count = data->NumeroTramo;
   for(int n=0; n < count; n++)
      {
      if(! data->EqptoTramo(n, &tramo)) return(false);

      // Prote��o
      if(tramo.fases == faseINV) return(false);

      // Obt�m barras do tramo
      bar1 = (TBarra *) tramo.barraCALC_1;
      bar2 = (TBarra *) tramo.barraCALC_2;
      if(bar1 == bar2) continue;  // Prote��o

      // Cria e insere Tramo em lisTRECHO
      trec1 = new TTrecho(bar1, bar2, tramo.fases);
      rede3->lisTRECHO->Add(trec1);
      trec1->eqpto    = tramo.eqpto;
      trec1->codigo   = rede3->AtribuiCodigoLigacao(tramo.codigo, bar1, bar2);
      trec1->id       = rede3->AnalisaIDTrecho(tramo.id);
      trec1->compr_km = tramo.comprimento_km;

      // Monta matrizes de imped�ncias s�rie e de admit�ncias paralelo do trecho
      fases     = trec1->fases;
      num_fases = fases->NumFases();
      for(int i=0; i < num_fases; i++)
         {
         ind_i                = fases->IndiceABCN(i);
         trec1->vet_iadm_a[i] = tramo.vet_iadm[ind_i]; // tramo.vet_iadm usa �ndices {0, 1, 2, 3} sempre
         for(int j=0; j < num_fases; j++)
            {
            z1 = tramo.matZ[i][j];
            // Inclui imped�ncias m�tuas condicionado ao flag
            if(i != j)
               {
               if(! geralC->flag_inclui_mutuas) z1 = czero;
               }
            // IMPORTANTE: as matrizes ser�o convertidas para [pu] mais adiante,
            //             na fun��o 'AcertaMatrizesTrechos()', porque neste
            //             momento as tens�es nominais das barras terminais do
            //             trecho podem n�o estar TODAS definidas. HPS, 2018.03.29.
            trec1->MatZserie_pu[i][j]    = z1;  // Fases ABN adjacentes em smcTRECHO
            trec1->MatYparalelo_pu[i][j] = complex<double>(0., tramo.matC[i][j]); // Fases ABN adjacentes em smcTRECHO
            }
         }

      // Calcula modelo pi-equivalente, quando indicado pelo flag
      if(geralC->usa_pi_equiv) trec1->CalculaPiEquivalente();
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddTrecho(void)
   {
   // Vari�veis locais
   int             count, ind_i, num_fases;
   complex<double> z1;
   TBarra          *bar1, *bar2;
   TFase           *fases;
   TTrecho         *trec1;
   smcTRECHO       trecho;

   // Loop para todos os trechos
   count = data->NumeroTrecho;
   for(int n=0; n < count; n++)
      {
      if (! data->EqptoTrecho(n, &trecho)) return(false);

      // Prote��o
      if(trecho.fases == faseINV)
         {
         return(false);
         }

      // Obt�m barras do trecho
      bar1 = (TBarra *) trecho.barraCALC_1;
      bar2 = (TBarra *) trecho.barraCALC_2;
      if(bar1 == bar2) continue;  // Prote��o

      // Cria e insere Trecho em lisTRECHO
      trec1 = new TTrecho(bar1, bar2, trecho.fases);
      rede3->lisTRECHO->Add(trec1);
      trec1->eqpto    = trecho.eqpto;
      trec1->codigo   = rede3->AtribuiCodigoLigacao(trecho.codigo, bar1, bar2);
      trec1->id       = rede3->AnalisaIDTrecho(trecho.id);
      trec1->compr_km = trecho.comprimento_km;

      // Monta matrizes de imped�ncias s�rie e de admit�ncias paralelo do trecho
      fases     = trec1->fases;
      num_fases = fases->NumFases();
      for(int i=0; i < num_fases; i++)
         {
         ind_i                = fases->IndiceABCN(i);
         trec1->vet_iadm_a[i] = trecho.vet_iadm[ind_i]; // trecho.vet_iadm usa �ndices {0, 1, 2, 3} sempre
         for(int j=0; j < num_fases; j++)
            {
            z1 = trecho.matZ[i][j];  // matZ em ohm; matC em Siemens
            if(i != j)  // Inclui imped�ncias m�tuas condicionado ao flag
               {
               if(! geralC->flag_inclui_mutuas) z1 = czero;
               }
            // IMPORTANTE: as matrizes ser�o convertidas para [pu] mais adiante,
            //             na fun��o 'AcertaMatrizesTrechos()', porque neste
            //             momento as tens�es nominais das barras terminais do
            //             trecho podem n�o estar TODAS definidas. HPS, 2018.03.29.
            trec1->MatZserie_pu[i][j]    = z1;  // Fases ABN adjacentes em smcTRECHO
            trec1->MatYparalelo_pu[i][j] = complex<double>(0., trecho.matC[i][j]); // Fases ABN adjacentes em smcTRECHO
            }
         }

      // Calcula modelo pi-equivalente, quando indicado pelo flag
      if(geralC->usa_pi_equiv) trec1->CalculaPiEquivalente();
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddTrechoInternoSuprimento(void)
   {
   // Vari�veis locais
   bool            retorno;
   int             fase_int;
   double          sbase_fase_inv, vnom_fase_kv, vnom_linha_kv, zbase_ohm;
   complex<double> valor, valor_pequeno;
   TBarra          *bar_ext, *bar_int;
   TFase           *fases_sup;
   TSup            *sup1;
   TTrecho         *trec1;

   // Inicializa��o
   valor_pequeno  = complex<double>(1.e-8, 0.);
   sbase_fase_inv = 1. / geralC->Sbase_fase;

   // Loop para todos os suprimentos
   for(int ns=0; ns < rede3->lisSUP->Count; ns++)
      {
      sup1      = (TSup *) rede3->lisSUP->Items[ns];
      bar_int   = sup1->bar_int;
      bar_ext   = sup1->bar_ext;
      fases_sup = sup1->fases;
      fase_int  = fases_sup->FasesAsInt();
      trec1     = new TTrecho(bar_int, bar_ext, fase_int, ttrSUPRIM);
      rede3->lisTRECHO->Add(trec1);
      trec1->codigo   = "Lg.int.sup.";
      trec1->compr_km = 0.001;
      for(int nfa=0; nfa < trec1->fases->NumFases(); nfa++)
         {
         trec1->vet_iadm_a[nfa] = 10000.;  // Valor arbitr�rio
         }

      // Monta matriz de imped�ncias s�rie do suprimento - tratamento
      // espec�fico para cada tipo de suprimento
      // IMPORTANTE: a matriz ser� convertida para [pu] mais adiante,
      //             na fun��o 'AcertaMatrizesTrechos()'. HPS, 2018.03.29.
      switch(fases_sup->NumFasesSemNT())
         {
         case 1:  // Suprimento monof�sico (� sempre AN, BN ou CN: matriz 2x2)
            fase_int                  = sup1->fases->Fase(0);
            vnom_fase_kv              = sup1->bar_ext->VnomFase_kv(fase_int);
            zbase_ohm                 = vnom_fase_kv * vnom_fase_kv * sbase_fase_inv;
            trec1->MatZserie_pu[0][0] = sup1->zp_pu   * zbase_ohm; // Valor em [ohm]
            trec1->MatZserie_pu[1][1] = valor_pequeno * zbase_ohm; // Valor em [ohm]
            break;

         case 2:  // Suprimento bif�sico (� sempre ABN, BCN ou CAN: matriz 3x3)
            fase_int     = sup1->fases->Fase(0);
            vnom_fase_kv = sup1->bar_ext->VnomFase_kv(fase_int); // NB: assume mesmo valor para as 2 fases
            zbase_ohm    = vnom_fase_kv * vnom_fase_kv * sbase_fase_inv;
            // 2 pr�prias de fase (zp)
            valor  = complex<double>(sup1->rt_pu, 0.) + sup1->zp_pu;
            valor *= zbase_ohm;
            trec1->MatZserie_pu[0][0] = valor;  // Valor em [ohm]
            trec1->MatZserie_pu[1][1] = valor;
            // 2 m�tuas entre fases (xm)
            valor  = complex<double>(sup1->rt_pu, sup1->xm_pu);
            valor *= zbase_ohm;
            trec1->MatZserie_pu[0][1] = valor;  // Valor em [ohm]
            trec1->MatZserie_pu[1][0] = valor;
            // 3a. linha/coluna (pr�pria e m�tuas do neutro)
            valor  = complex<double>(sup1->rt_pu, 0.);
            valor *= zbase_ohm;
            trec1->MatZserie_pu[0][2] = valor;  // Valor em [ohm]
            trec1->MatZserie_pu[1][2] = valor;
            trec1->MatZserie_pu[2][0] = valor;
            trec1->MatZserie_pu[2][1] = valor;
            // Se necess�rio, coloca imped�ncia pequena para permitir a invers�o da matriz
            if(fabs(sup1->rt_pu) < ZEROV)
               {
               valor                     = valor_pequeno * zbase_ohm;
               trec1->MatZserie_pu[2][2] = valor;  // Valor em [ohm]
               }
            break;

         case 3:  // Suprimento trif�sico (� sempre ABCN: matriz 4x4)
            // IMPORTANTE: no c�lculo dos par�metros 'rt_pu', 'xm_pu' e 'zp_pu'
            //             foi assumido que as 3 tens�es nominais de fase (A, B, C)
            //             s�o iguais � tens�o nominal de linha dividida por raiz(3)
            //             [cf. TMonta3::AddSuprimento()]
            vnom_linha_kv = sup1->bar_ext->VnomLinha_kv();
            zbase_ohm     = vnom_linha_kv * vnom_linha_kv / geralC->Sbase;
            // 3 pr�prias de fase (zp)
            valor  = complex<double>(sup1->rt_pu, 0.) + sup1->zp_pu;
            valor *= zbase_ohm;
            trec1->MatZserie_pu[0][0] = valor;  // Valor em [ohm]
            trec1->MatZserie_pu[1][1] = valor;
            trec1->MatZserie_pu[2][2] = valor;
            // 6 m�tuas entre fases (xm)
            valor  = complex<double>(sup1->rt_pu, sup1->xm_pu);
            valor *= zbase_ohm;
            trec1->MatZserie_pu[0][1] = valor;  // Valor em [ohm]
            trec1->MatZserie_pu[0][2] = valor;
            trec1->MatZserie_pu[1][0] = valor;
            trec1->MatZserie_pu[1][2] = valor;
            trec1->MatZserie_pu[2][0] = valor;
            trec1->MatZserie_pu[2][1] = valor;
            // 4a. linha/coluna (pr�pria e m�tuas do neutro)
            valor  = complex<double>(sup1->rt_pu, 0.);
            valor *= zbase_ohm;
            trec1->MatZserie_pu[0][3] = valor;  // Valor em [ohm]
            trec1->MatZserie_pu[1][3] = valor;
            trec1->MatZserie_pu[2][3] = valor;
            trec1->MatZserie_pu[3][0] = valor;
            trec1->MatZserie_pu[3][1] = valor;
            trec1->MatZserie_pu[3][2] = valor;
            trec1->MatZserie_pu[3][3] = valor;
            // Se necess�rio, coloca imped�ncia pequena para permitir a invers�o da matriz
            if(fabs(sup1->rt_pu) < ZEROV)
               {
               valor                     = valor_pequeno * zbase_ohm;
               trec1->MatZserie_pu[3][3] = valor;  // Valor em [ohm]
               }
            break;

         default:  // Erro
            return(false);
         }
      } // for(ns)

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddYrefReduc(void)
   {
   // Local variables
   int             count, fi, fj, ind_i, ind_j, num_fases;
   double          fator_zbase, sbase_fase_inv, zbase_ij;
   complex<double> cvalor;
   TBarra          *bar1;
   TFase           *fases;
   smcYREF         yref;

   // Inicializa��o
   sbase_fase_inv = 1. / geralC->Sbase_fase;

   // Loop para todos os Yref
   count = data->NumeroYref;
   for(int n=0; n < count; n++)
      {
      if(! data->EqptoYref(n, &yref)) return(false);

      // Obt�m a barra e suas fases
      bar1      = (TBarra *) yref.barraCALC;
      fases     = bar1->fases;
      num_fases = fases->NumFases();

      // Copia admit�ncias de sequ�ncia zero e direta
      bar1->Ynodal0_pu += yref.y0_pu;
      bar1->Ynodal1_pu += yref.y1_pu;

      // Copia matriz de admitancias de barra
      for(int i=0; i < num_fases; i++)
         {
         ind_i       = fases->IndiceABCN(i);
         fi          = fases->Fase(i);
         fator_zbase = bar1->VnomFase_kv(fi) * sbase_fase_inv;
         for(int j=0; j < num_fases; j++)
            {
            ind_j    = fases->IndiceABCN(j);
            fj       = fases->Fase(j);
            zbase_ij = fator_zbase * bar1->VnomFase_kv(fj);
            cvalor   = yref.maty_S[ind_i][ind_j] * zbase_ij;  // [pu]
            bar1->mat_ynodal_reduc_pu->AddC2[i][j] = cvalor;
            }
         }
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AnalisaAterramentoTrafosReguladoresMono(void)
{
// Esta fun��o analisa aterramento de reguladores e trafos monof�sicos,
// impondo aterramento atSolido se necess�rio (para evitar rede desconexa
// pelo neutro)

// Vari�veis locais
TTrafo12 *trafo12;

// Analisa objetos TTrafo12 + TReg12
for(int nt=0; nt < rede3->lis12->Count; nt++)
   {
   trafo12 = (TTrafo12 *) rede3->lis12->Items[nt];
   trafo12->AnalisaAterramento(geralC->flag_ater_comuns, rede3);
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AnalisaSuprimentosPatamar(int np)
{
// Vari�veis locais
bool   flag_mudou_tipo, flag_PV_VF;
int    tipo_ant;
TBarra *bar1;
TSup   *sup1;

// IMPORTANTE: a an�lise N�O pode ser abandonada na primeira mudan�a de
//             tipo pois � preciso determinar o tipo de todos os suprimentos
//             (original ou PQ).

// Inicializa��o
flag_mudou_tipo = false;  // Em princ�pio, nenhuma barra mudou de tipo

// Loop de suprimentos
for(int ns=0; ns < rede3->lisSUP->Count; ns++)
   {
   sup1       = (TSup *) rede3->lisSUP->Items[ns];
   bar1       = sup1->bar_int;  // Barra interna
   tipo_ant   = bar1->tipo;     // Salva tipo atual
   flag_PV_VF = data->ExisteGeracaoPV_VF(bar1->eqpto, np);
   if(flag_PV_VF)  // Neste patamar, este suprimento possui gera��o PV ou VF
      {
      bar1->tipo = bar1->tipo_original;  // Restaura tipo original
      }
   else  // Suprimento PQ ou suprimento PV/VF inativo neste patamar
      {
      bar1->tipo = BAR_PQ;  // Redefine tipo
      }
   if(bar1->tipo != tipo_ant) flag_mudou_tipo = true;
   }

return(flag_mudou_tipo);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AterraBarrasPorCargaOuEstatico(void)
{
// Esta fun��o aterra as barras que possu�rem carga, capacitor ou reator aterrado

// Vari�veis locais
int          count;
smcCAPACITOR capacitor;
smcCARGA     carga;
smcREATOR    reator;
TBarra       *bar1;

// Loop para todas as Cargas
count = data->NumeroCarga;
for(int nc=0; nc < count; nc++)
   {
   if (! data->EqptoCarga(nc, &carga)) return(false);
   if((carga.fases & faseT) != faseT) continue; // Liga��o irrelevante
   bar1 = (TBarra *) carga.barraCALC;
   bar1->fases->AddFase(faseN);
   bar1->DefineAterramento(atSolido);
   }

// Loop para todos os Capacitores
count = data->NumeroCapacitor;
for(int n=0; n < count; n++)
   {
   if(! data->EqptoCapacitor(n, &capacitor)) return(false);
   if(capacitor.ligacao != ligacaoESTRELA_ATERRADO) continue; // Liga��o irrelevante
   bar1 = (TBarra *) capacitor.barraCALC;
   bar1->fases->AddFase(faseN);
   bar1->DefineAterramento(atSolido);
   }

// Loop para todos os Reatores
count = data->NumeroReator;
for(int n=0; n < count; n++)
   {
   if (! data->EqptoReator(n, &reator)) return(false);
   if(reator.ligacao != ligacaoESTRELA_ATERRADO) continue; // Liga��o irrelevante
   bar1 = (TBarra *) reator.barraCALC;
   bar1->fases->AddFase(faseN);
   bar1->DefineAterramento(atSolido);
   }

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::AtribuiLinhasY(void)
{
// Vari�veis locais
bool     sucesso, teste1, teste2, teste3;
int      fase_comum, ind_fase1, ind_fase2, ind_fase3, linha1,
         linha2, num_enrol_ater;
clock_t  inicio;
TList    *lisTRIPLA_GLOBAL;
TBarra   *bar1, *bar2, *bar3;
TFase    *fases1, *fases2, *fases3;
TTrafo12 *trafo12;
TTrafo32 *trafo32;
TTrafo33 *trafo33;
TTripla  *tripla;

// IMPORTANTE: Esta fun��o define as linhas de cada barra em Ynodal nos
//             seguintes casos:
//             - fase ou neutro comum em TTrafo12 (AUTOTRAFO4 ou AUTOTRAFO6)
//             - Trafos com neutros comuns (flag_ater_comuns == true)
//             - Trafos sem neutros comuns (flag_ater_comuns == false)

// IMPORTANTE: Esta fun��o exige que as barras estejam ordenadas

// IMPORTANTE: Os n�s de barras de carga s�o TODOS n�s de carga; as barras de
//             gera��o possuem SEMPRE n�s de gera��o e podem possuir tamb�m
//             n�s de carga. Exemplo: gerador AN em barra ABCN
//             (n� de gera��o: A; n�s de carga: BCN). HPS, 2019.07.04.

// Inicializa��o
inicio           = clock();
sucesso          = true;
lisTRIPLA_GLOBAL = new TList;

// Inicializa contador de linhas e linhas no objeto TFase de cada barra
linha1 = 0;
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1   = (TBarra *) rede3->lisBAR->Items[nb];
   fases1 = bar1->fases;
   fases1->ReiniciaLinhas();  // Coloca valor "-1"
   }

// Loop de duas passagens: n�s de carga (barras de carga e gera��o) e
// n�s de gera��o (s� barras de gera��o)
for(int nv=0; nv < 2; nv++)
   {
   DefineLinhasYnodal(nv, &linha1);
   }

// Define n�s comuns em TRAFO12 e REG12 se:
//    1. AUTOTRAFO4 ou AUTOTRAFO6: sempre (fase ou neutro)
//    2. TRAFO4 ou TRAFO5: se houver aterramentos comuns e ambos
//                         aterramentos forem atZ ou atSolido (somente neutro)
for(int nt=0; nt < rede3->lis12->Count; nt++)
   {
   trafo12 = (TTrafo12 *) rede3->lis12->Items[nt];
   bar1    = trafo12->bar1;
   bar2    = trafo12->bar2;
   fases1  = bar1->fases;
   fases2  = bar2->fases;
   switch(trafo12->tipo)
      {
      case AUTOTRAFO4:
      case AUTOTRAFO6_1T:
      case AUTOTRAFO6_2T:
         // Cria tripla de n�s comuns e a insere na lista
         tripla = new TTripla(trafo12, trafo12->fase_comum);
         lisTRIPLA_GLOBAL->Add(tripla);

         // AUTOTRAFO6 sem neutro: liga neutro do secund�rio ao neutro do
         // prim�rio, se ambas barras possuirem neutro n�o-ATSolido (para
         // evitar rede desconexa pelo neutro)
         teste1 = ((trafo12->tipo == AUTOTRAFO6_1T)  ||
                   (trafo12->tipo == AUTOTRAFO6_2T)) &&
                  (trafo12->fases_pri->IndiceN() < 0);
         if(teste1)  // AUTOTRAFO6 sem neutro
            {
            ind_fase1 = fases1->IndiceN();                // Neutro da barra
            ind_fase2 = fases2->IndiceN();                // Neutro da barra
            if((ind_fase1 < 0) || (ind_fase2 < 0)) break; // Descarta caso irrelevante (n�o existe neutro na barra)
            linha1 = fases1->LinhaIndice(ind_fase1);      // Linha do neutro
            linha2 = fases2->LinhaIndice(ind_fase2);      // Linha do neutro
            if((linha1 < 0) || (linha2 < 0)) break;       // Descarta caso irrelevante (neutro ATSolido)
            // Cria tripla de n�s comuns e a insere na lista
            tripla = new TTripla(trafo12);
            lisTRIPLA_GLOBAL->Add(tripla);
				}
         break;

      case TRAFO4:
      case TRAFO5:
         if(! geralC->flag_ater_comuns) break;
         ind_fase1 = trafo12->fases_pri->IndiceN();    // Neutro do TRAFO
         ind_fase2 = trafo12->fases_sec->IndiceN();    // Neutro do TRAFO
         if((ind_fase1 < 0) || (ind_fase2 < 0)) break; // Caso irrelevante: n�o existem 2 neutros
         if((trafo12->tipo_ater1 == atIsolado) || (trafo12->tipo_ater2 == atIsolado)) break; // Caso irrelevante
         // Cria tripla de n�s comuns e a insere na lista
         tripla = new TTripla(trafo12);
         lisTRIPLA_GLOBAL->Add(tripla);
         break;
      } // switch()
   } // for(nt)

// Define n�s comuns em TRAFO32 e REG32 se:
//    1. AUTOTRAFOY (sempre), OU
//    2. ambas BARRAS forem atIsolado (o neutro passa pelo trafo), OU
//    3. houver aterramentos comuns e ambos aterramentos forem atZ ou atSolido
for(int nt=0; nt < rede3->lis32->Count; nt++)
   {
   // IMPORTANTE: por causa do Teste 2, � necesss�rio verificar o neutro DAS BARRAS
   trafo32   = (TTrafo32 *) rede3->lis32->Items[nt];
   bar1      = trafo32->bar1;
   bar2      = trafo32->bar2;
   fases1    = bar1->fases;
   fases2    = bar2->fases;
   ind_fase1 = fases1->IndiceN();
   ind_fase2 = fases2->IndiceN();
   if((ind_fase1 < 0) || (ind_fase2 < 0)) continue; // Descarta caso irrelevante (pelo menos uma das barras n�o tem neutro)
   teste1 = (trafo32->tipo == AUTOTRAFO_Y);
   teste2 = (bar1->tipo_ater == atIsolado) && (bar2->tipo_ater == atIsolado);
   teste3 = geralC->flag_ater_comuns &&
               ((trafo32->tipo_ater1 != atIsolado) && (trafo32->tipo_ater2 != atIsolado));
   if(teste1 || teste2 || teste3)  // Existe n� comum
      {
      // Cria tripla de n�s comuns e a insere na lista
      tripla = new TTripla(trafo32);
      lisTRIPLA_GLOBAL->Add(tripla);
      }
   }

// Define n�s comuns em TRAFO33 se houver:
//    1. aterramentos comuns, E
//    2. pelo menos 2 enrolamentos aterrados em atZ ou atSolido
if(geralC->flag_ater_comuns)
   {
   for(int nt=0; nt < rede3->lisTRAFO33->Count; nt++)
      {
      trafo33        = (TTrafo33 *) rede3->lisTRAFO33->Items[nt];
      num_enrol_ater = 0;
      if(trafo33->tipo_ater1 != atIsolado) num_enrol_ater++; // Aterramento atZ ou atSolido no prim�rio
      if(trafo33->tipo_ater2 != atIsolado) num_enrol_ater++; // Aterramento atZ ou atSolido no secund�rio
      if(trafo33->tipo_ater3 != atIsolado) num_enrol_ater++; // Aterramento atZ ou atSolido no terci�rio
      if(num_enrol_ater < 2) continue;                       // Descarta trafo irrelevante

      // Trafo relevante (2 ou 3 aterramentos atZ ou atSolido); analisa enrolamentos aterrados
      ind_fase1 = -1;  // Inicializa��o
      ind_fase2 = -1;  //      "
      ind_fase3 = -1;  //      "
      if(trafo33->tipo_ater1 != atIsolado)
         {
         bar1      = trafo33->bar1;
         fases1    = bar1->fases;
         ind_fase1 = fases1->IndiceN();
         if(ind_fase1 < 0) {sucesso = false; break;}  // Erro
         }
      if(trafo33->tipo_ater2 != atIsolado)
         {
         bar2      = trafo33->bar2;
         fases2    = bar2->fases;
         ind_fase2 = fases2->IndiceN();
         if(ind_fase2 < 0) {sucesso = false; break;}  // Erro
         }
      if(trafo33->tipo_ater3 != atIsolado)
         {
         bar3      = trafo33->bar3;
         fases3    = bar3->fases;
         ind_fase3 = fases3->IndiceN();
         if(ind_fase3 < 0) {sucesso = false; break;}  // Erro
         }
      // Cria tripla de n�s comuns e a insere na lista
      if((ind_fase1 >= 0) && (ind_fase2 >= 0) && (ind_fase3 >= 0))
         {
         tripla = new TTripla(trafo33, faseN, bar1, bar2, bar3);
         }
      else if((ind_fase1 >= 0) && (ind_fase2 >= 0))
         {
         tripla = new TTripla(trafo33, faseN, bar1, bar2);
         }
      else if((ind_fase2 >= 0) && (ind_fase3 >= 0))
         {
         tripla = new TTripla(trafo33, faseN, bar2, bar3);
         }
      else
         {
         tripla = new TTripla(trafo33, faseN, bar3, bar1);
         }
      lisTRIPLA_GLOBAL->Add(tripla);
      } // for(nt)
   } // if()

// Numera��o definitiva de n�s: renumera n�s da rede considerando as
// triplas de n�s comuns identificadas logo acima
if(sucesso && (lisTRIPLA_GLOBAL->Count > 0)) RenumeraNos(lisTRIPLA_GLOBAL);

// Redefine aterramento de trafos para compatibiliz�-los com o aterramento
// das barras (por causa de eventuais altera��es no aterramento das barras
// dentro de cada grupo de triplas, ou simplesmente porque j� havia diferen�as
// nos dados. Exemplo: trafo DY definido como atZ no secund�rio e posterior
// inser��o de trafo ZigZag na barra do secund�rio)
if(sucesso) RedefineAterramentoTrafos();

// Verifica compatibilidade entre neutro e tipo de aterramento nas barras
// (prote��o adicional motivada pelo bug de barras sem neutro com aterramento
// atSolido, em trafos equivalentes - 2014.03.27)
if(sucesso)
   {
   for(int nb=0; nb < rede3->lisBAR->Count; nb++)
      {
      bar1   = (TBarra *) rede3->lisBAR->Items[nb];
      fases1 = bar1->fases;
      if(fases1->ExisteN()) continue;   // Descarta barra com neutro
      if(bar1->tipo_ater != atIsolado)  // Erro
         {
         sucesso = false;
         break;
         }
      }
   }

// Libera mem�ria
rede3->DeletaLista(&lisTRIPLA_GLOBAL);

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttATRIB_LIN_Y, inicio);

return(sucesso);
}

//---------------------------------------------------------------------------
void __fastcall TMonta3::CalculaYpYm(int             num_fases,
                                     complex<double> Ynodal0,
                                     complex<double> Ynodal1,
                                     complex<double> *yp,
                                     complex<double> *ym)
{
// Vari�veis locais
complex<double> z0, z1, zm, zp;
VTMatriz        *yelem, *zelem;

// Inicializa��o
*yp = czero;
*ym = czero;

// Calcula as admit�ncias pr�pria e m�tua (yp e ym) da liga��o trif�sica
// entre as duas barras, de duas formas distintas: quando a admit�ncia de
// sequ�ncia zero � nula (no caso de elimina��o de trafo DY ou YD) e
// caso contr�rio
if(IsComplexZero(Ynodal0))  // Y0 == 0
   {
   // Com as express�es abaixo, resultam y1 = yp - ym = Ynodal1 e
   // y0 = (yp + 2ym) = 0 na liga��o trif�sica. Esta forma equivale a
   // colocar um valor bem pequeno para 'Ynodal0' e proceder com
   // a montagem e invers�o de 'zelem'.
   *yp = (2./3.) * Ynodal1;
   *ym = - Ynodal1 / 3.;
   }
else  // Y0 != 0
   {
    // Monta e inverte matriz de imped�ncias da liga��o
   z0    = cum / Ynodal0;  // Imped�ncia da liga��o a menos do sinal
   z1    = cum / Ynodal1;  // Imped�ncia da liga��o a menos do sinal
   zp    = (z0 + (2. * z1)) / 3.;
   zm    = (z0 -       z1)  / 3.;
   zelem = NewObjMatrizC(num_fases, num_fases);
   zelem->C2[0][0] = zp; zelem->C2[1][1] = zp; zelem->C2[2][2] = zp;
   zelem->C2[0][1] = zm; zelem->C2[1][0] = zm; zelem->C2[0][2] = zm;
   zelem->C2[2][0] = zm; zelem->C2[1][2] = zm; zelem->C2[2][1] = zm;
   yelem = zelem->Inverte();
   if(yelem != NULL)
      {
      // Sem trocar o sinal de 'yp' e 'ym' porque n�o trocou no come�o
      *yp = (yelem->C2[0][0] + yelem->C2[1][1] + yelem->C2[2][2]) / 3.;
      *ym = (yelem->C2[0][1] + yelem->C2[1][0] + yelem->C2[0][2] +
             yelem->C2[2][0] + yelem->C2[1][2] + yelem->C2[2][1]) / 6.;
      }
   delete yelem;
   delete zelem;
   }
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::CalculaZentrada_1Vez3nx3n(bool so_trechos)
{
// Vari�veis locais
int             coluna, coluna2, linha, linha2, num_elems, num_fases,
                num_lin_submatriz, num_lin_total, ordem;
double          vnom_linha, zbase_ohm;
complex<double> zent0, zent1, zm, zp;
TBarra          *bar_def;
TBarraDefeito   *bar_defeito;
TFase           *fases_bi, *fases_bj;
TSup            *sup1;
VTMatriz        *MY, *MZ;

// Prote��o
if(curto_aux3 == NULL) return(false);  // Erro

// Inicializa��o
num_elems = 0;  // Tamanho da matriz final, ap�s redu��o
MY        = NULL;
MZ        = NULL;

// Determina tamanho da matriz Ynodal reduzida
for(int nb=0; nb < curto_aux3->lisBAR_DEF->Count; nb++)
   {
   bar_defeito = (TBarraDefeito *) curto_aux3->lisBAR_DEF->Items[nb];
   bar_def     = bar_defeito->barra;
   // Prote��es
   if(bar_def == NULL)        return(false); // Barra para defeito indefinida
   if(! bar_def->flag_conexa) return(false); // Barra para defeito desconexa
   // Obt�m demais dados
   fases_bi  = bar_def->fases;
   num_fases = fases_bi->NumFasesSemNT();    // Exclui neutro e terra
   if(num_fases != 3) continue;              // Imposs�vel calcular Zentrada
   bar_defeito->calculavel = true;           // Usado logo abaixo
   num_elems += 3;                           // Atualiza 'num_elems'
   } // for(nb)

// Prote��o: deve haver pelo menos 3 n�s na matriz Ynodal reduzida (MY)
if(num_elems < 3) return(true);

// Remonta Ynodal
// IMPORTANTE: a montagem j� inclui a imped�ncia de defeito, sendo
//             necess�rio remov�-la
//             Argumentos:    1:    'ordemh'
//                            true: 'flag_inclui_mutuas'
if(! MontaY(1, true, so_trechos)) return(false);
if(! InsereZdef(true))            return(false);  // 'true': remove Zdef

// Remove parti��o de Ynodal, j� que ser� executada uma fatora��o parcial
// da matriz completa (= n�o particionada)
num_lin_submatriz = Ynod->NumLinhasSubmatriz();  // Salva valor
num_lin_total     = Ynod->NumTotalLinhas();
Ynod->DefineTamanho(num_lin_total, num_lin_total);

// Aterra fases da barra INTERNA dos suprimentos (compatibilidade com CurtoCS)
for(int ns=0; ns < rede3->lisSUP->Count; ns++)
   {
   sup1      = (TSup *) rede3->lisSUP->Items[ns];
   fases_bi  = sup1->bar_int->fases;  // Barra INTERNA
   num_fases = fases_bi->NumFases();
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      if(fases_bi->Fase(nfa) == faseN) continue;  // Descarta o neutro
      linha = fases_bi->LinhaIndice(nfa);
      Ynod->InsereDiagonal(linha, complex<double>(0., -1.e8));
      }
   }

// Define linhas que ser�o mantidas na fatora��o parcial (fases da
// barra de defeito)
for(int nb=0; nb < curto_aux3->lisBAR_DEF->Count; nb++)
   {
   bar_defeito = (TBarraDefeito *) curto_aux3->lisBAR_DEF->Items[nb];
   if(! bar_defeito->calculavel) continue;  // Imposs�vel calcular Zentrada
   bar_def   = bar_defeito->barra;
   fases_bi  = bar_def->fases;
   num_fases = fases_bi->NumFasesSemNT();   // Exclui neutro e terra
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      linha = fases_bi->LinhaIndice(nfa);
      Ynod->MantemLinha(linha);
      }
   } // for(nb)
if(Ynod->Erro != slSemErro) return(false);  // Erro

// Imprime Ynodal antes da fatora��o parcial
// AnsiString filename1 = geralC->DirTmp() + "\\Ynod_ANTES.txt";
// Ynod->Imprime(filename1);

// Fatora��o parcial de Ynodal
Ynod->Triang();
if(Ynod->Erro != slSemErro) return(false);

// Imprime Ynodal depois da fatora��o parcial
// AnsiString filename2 = geralC->DirTmp() + "\\Ynod_DEPOIS.txt";
// Ynod->Imprime(filename2);

// Monta e inverte Ynodal reduzida
MY     = NewObjMatrizC(num_elems, num_elems);
linha2 = 0;
for(int nbi=0; nbi < curto_aux3->lisBAR_DEF->Count; nbi++)
   {
   bar_defeito = (TBarraDefeito *) curto_aux3->lisBAR_DEF->Items[nbi];
   if(! bar_defeito->calculavel) continue; // Zentrada n�o pode ser calculada
   bar_def  = bar_defeito->barra;
   fases_bi = bar_def->fases;
   for(int ix=0; ix < 3; ix++)
      {
      linha   = fases_bi->LinhaIndice(ix);
      coluna2 = 0;
      for(int nbj=0; nbj < curto_aux3->lisBAR_DEF->Count; nbj++)
         {
         bar_defeito = (TBarraDefeito *) curto_aux3->lisBAR_DEF->Items[nbj];
         if(! bar_defeito->calculavel) continue; // Zentrada n�o pode ser calculada
         bar_def  = bar_defeito->barra;
         fases_bj = bar_def->fases;
         for(int jx=0; jx < 3; jx++)
            {
            coluna = fases_bj->LinhaIndice(jx);
            ordem  = Ynod->ElementoLC(linha, coluna, &zp);
            if(ordem >= 0) MY->C2[linha2][coluna2] = zp;
            coluna2++;
            } // for(jx)
         } // for(nbj)
      linha2++;
      } // for(ix)
   } // for(nbi)
// MY->Imprime("D:\\Transfer\\1Vez3nx3n_MY.txt");
MZ = MY->Inverte();
if(MZ != NULL)
   {
   // MZ->Imprime("D:\\Transfer\\1Vez3nx3n_MZ.txt");
   // Calcula Zp, Zm, Zent0 e Zent1
   linha2 = 0;
   for(int nb=0; nb < curto_aux3->lisBAR_DEF->Count; nb++)
      {
      bar_defeito = (TBarraDefeito *) curto_aux3->lisBAR_DEF->Items[nb];
      if(! bar_defeito->calculavel) continue; // Zentrada n�o pode ser calculada
      bar_def = bar_defeito->barra;
      if(bar_def->fases->NumFasesSemNT() != 3) continue; // N�o foi calculada Zentrada para esta barra
      zp    = (MZ->C2[linha2][linha2] + MZ->C2[linha2+1][linha2+1] +
               MZ->C2[linha2+2][linha2+2]) / 3.;
      zm    = (MZ->C2[linha2][linha2+1] + MZ->C2[linha2][linha2+2] +
               MZ->C2[linha2+1][linha2+2]) / 3.;
      zent0 = zp + (2.*zm);
      zent1 = zp -     zm;

      // Calcula e salva imped�ncias de entrada (valores em ohm)
      vnom_linha  = bar_def->VnomLinha_kv();
      zbase_ohm   = vnom_linha * vnom_linha / geralC->Sbase;
      zent0      *= zbase_ohm;
      zent1      *= zbase_ohm;
      if(so_trechos)
         {
         curto_aux3->zent0_so_trechos_ohm[nb] = zent0;
         curto_aux3->zent1_so_trechos_ohm[nb] = zent1;
         }
      else
         {
         curto_aux3->zent0_ohm[nb] = zent0;
         curto_aux3->zent1_ohm[nb] = zent1;
         }

      // Atualiza 'linha2'
      linha2 += 3;
      } // for(nb)
   }

// Libera mem�ria
delete MY;
delete MZ;

// Limpa a matriz e restaura a parti��o
Ynod->ZeraMatriz();
Ynod->DefineTamanho(num_lin_submatriz, num_lin_total);

return(true);
}

/*
//---------------------------------------------------------------------------
bool __fastcall TMonta3::CalculaZentrada_nVezes3x3(void)
{
// Vari�veis locais
int             coluna, linha, num_fases_b, num_fases_sup,
                num_lin_submatriz, num_lin_total, ordem;
double          vnom_linha, zbase_ohm;
complex<double> zent0, zent1, zm, zp;
TBarra          *bar_def;
TBarraDefeito   *bar_defeito;
TFase           *fases_b, *fases_sup;
TSup            *sup1;
VTMatriz        *MY, *MZ;

// Prote��o
if(curto_aux3 == NULL) return(false);  // Erro

// Remove parti��o de Ynodal, j� que ser� executada uma fatora��o parcial
// da matriz completa (= n�o particionada)
num_lin_submatriz = Ynod->NumLinhasSubmatriz();  // Salva valor
num_lin_total     = Ynod->NumTotalLinhas();
Ynod->DefineTamanho(num_lin_total, num_lin_total);

// Loop de barras para defeito (mais de uma barra no caso de curto simult�neo)
for(int nb=0; nb < curto_aux3->lisBAR_DEF->Count; nb++)
   {
   bar_defeito = (TBarraDefeito *) curto_aux3->lisBAR_DEF->Items[nb];
   bar_def     = bar_defeito->barra;

   // Prote��es
   if(bar_def == NULL)        return(false);  // Barra para defeito indefinida
   if(! bar_def->flag_conexa) return(false);  // Barra para defeito desconexa

   // Obt�m demais dados
   fases_b     = bar_def->fases;
   num_fases_b = fases_b->NumFasesSemNT();  // Exclui neutro e terra
   if(num_fases_b != 3) return(true);       // Imposs�vel calcular Zentrada

   // Inicializa��o
   zent0 = czero;
   zent1 = czero;
   MY    = NULL;
   MZ    = NULL;

   // Remonta Ynodal
   // IMPORTANTE: a montagem j� inclui a imped�ncia de defeito, sendo
   //             necess�rio remov�-la
   if(! MontaY())         return(false);
   if(! InsereZdef(true)) return(false);  // 'true': remove Zdef

   // Aterra fases da barra INTERNA dos suprimentos (compatibilidade com CurtoCS)
   for(int ns=0; ns < rede3->lisSUP->Count; ns++)
      {
      sup1          = (TSup *) rede3->lisSUP->Items[ns];
      fases_sup     = sup1->bar_int->fases;  // Barra INTERNA
      num_fases_sup = fases_sup->NumFases();
      for(int nfa=0; nfa < num_fases_sup; nfa++)
         {
         if(fases_sup->Fase(nfa) == faseN) continue;  // Descarta o neutro
         linha = fases_sup->LinhaIndice(nfa);
         Ynod->InsereDiagonal(linha, complex<double>(0., -1.e8));
         }
      }

   // Imprime Ynodal antes da fatora��o parcial
   // AnsiString filename1 = geralC->DirTmp() + "\\Ynod_ANTES.txt";
   // Ynod->Imprime(filename1);

   // Especifica linhas a serem mantidas na fatora��o parcial (fases da
   // barra de defeito)
   for(int nfa=0; nfa < num_fases_b; nfa++)
      {
      linha = fases_b->LinhaIndice(nfa);
      Ynod->MantemLinha(linha);
      }
   if(Ynod->Erro != slSemErro) return(false);

   // Fatora��o parcial de Ynodal
   Ynod->Triang();
   if(Ynod->Erro != slSemErro) return(false);

   // Imprime Ynodal depois da fatora��o parcial
   // AnsiString filename2 = geralC->DirTmp() + "\\Ynod_DEPOIS.txt";
   // Ynod->Imprime(filename2);

   // Monta e inverte Ynodal reduzida
   MY = NewObjMatrizC(num_fases_b, num_fases_b);
   for(int i=0; i < num_fases_b; i++)
      {
      linha = fases_b->LinhaIndice(i);
      for(int j=0; j < num_fases_b; j++)
         {
         coluna = fases_b->LinhaIndice(j);
         ordem  = Ynod->ElementoLC(linha, coluna, &zp);
         if(ordem >= 0) MY->C2[i][j] = zp;
         }
      }
   // MY->Imprime("D:\\Transfer\\MY.txt");
   MZ = MY->Inverte();
   if(MZ != NULL)
      {
      // MZ->Imprime("D:\\Transfer\\MZ.txt");
      // Calcula Zp, Zm, Zent0 e Zent1
      zp    = (MZ->C2[0][0] + MZ->C2[1][1] + MZ->C2[2][2]) / 3.;
      zm    = (MZ->C2[0][1] + MZ->C2[0][2] + MZ->C2[1][2]) / 3.;
      zent0 = zp + (2.*zm);
      zent1 = zp -     zm;
      }

   // Salva valores
   vnom_linha                = bar_def->VnomLinha();
   zbase_ohm                 = vnom_linha * vnom_linha / geralC->Sbase;
   curto_aux3->zent0_ohm[nb] = zent0 * zbase_ohm;
   curto_aux3->zent1_ohm[nb] = zent1 * zbase_ohm;

   // Libera mem�ria
   delete MY;
   delete MZ;
   } // for(nb)

// Limpa a matriz e restaura a parti��o
Ynod->ZeraMatriz();
Ynod->DefineTamanho(num_lin_submatriz, num_lin_total);

return(true);
}
*/

//---------------------------------------------------------------------------
bool __fastcall TMonta3::DefineAterramentoBarras(int             tipo_ater,
                                                 complex<double> zater_ohm)
{
// Vari�veis locais
TBarra *bar1;

// Loop de barras
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   bar1->DefineAterramento(tipo_ater, geralC, zater_ohm);
   }

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TMonta3::DefineLinhasYnodal(int nv,
                                            int *linha)
{
// Vari�veis locais
bool   teste1, teste2;
int    fase_int, nb_inic, num_fases;
TBarra *bar1;
TFase  *fases;

// Inicializa��o
if(nv == 0) nb_inic = 0;                        // Todas as barras (carga e gera��o)
else        nb_inic = geralC->num_barras_carga; // S� barras de gera��o

// Loop de barras
for(int nb=nb_inic; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   if(! bar1->flag_conexa) continue;  // Barra desconexa: nada mais a fazer
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      // A prote��o abaixo � necess�ria na renumera��o de n�s
      if(fases->LinhaIndice(nfa) >= 0) continue; // N� j� numerado anteriormente
      fase_int = fases->Fase(nfa);
      // Pula a cria��o de n� nas seguintes condi��es:
      //    1. neutro de barra com aterramento atSolido (n�o existe o n�), OU
      //    2. n� de tipo diferente do tipo sendo tratado
      teste1 = (fase_int == faseN) && (bar1->tipo_ater == atSolido);
      if(nv == 0) teste2 =   bar1->IsNoGeracao(fase_int); // Exclui n� de gera��o
      else        teste2 = ! bar1->IsNoGeracao(fase_int); // Exclui n� de carga
      if(teste1 || teste2) continue;                      // N�o cria n�
      // Cria o n�
      fases->SetLinhaIndice(nfa, *linha);  // Atribui linha atual ao n�
      (*linha)++;                          // Atualiza contador de linhas
      } // for(nfa)
   } // for(nb)

// Define contadores da matriz
if(nv == 0) geralC->num_lin_submatriz = *linha; // Parti��o da matriz [Ycc]
else        geralC->num_tot_linhas    = *linha; // Matriz completa
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::DeterminaFaseComumAUTOTRAFO4(void)
{
bool     flag_padrao_a, flag_padrao_b, teste, sucesso;
int      f1, f2, f3, f4, f5, f6, fase_comum, soma1, soma2;
TBarra   *bar1, *bar2, *bar3, *bar4;
TTrafo12 *trafo12a, *trafo12b, *trafo12c;
TList    *lisEQP, *lisTEMP;

// Esta fun��o determina a fase comum para trafos e reguladores do tipo
// 'AUTOTRAFO4', uma �nica unidade ou 2-3 unidades ligadas em paralelo.
// Os trafos e reguladores do tipo 'AUTOTRAFO6' n�o podem ser ligados
// em paralelo.

// Verifica exist�ncia de trafos e reguladores
if((rede3->lisTRAFO12->Count == 0) && (rede3->lisREG12->Count == 0)) return(true);

// Monta 'lisEQP': objetos TTrafo12 e TReg12 juntos
sucesso = true;
lisEQP  = new TList;
for(int nt=0; nt < rede3->lisTRAFO12->Count; nt++)
   {
   trafo12a = (TTrafo12 *) rede3->lisTRAFO12->Items[nt];
   lisEQP->Add(trafo12a);
   }
for(int nr=0; nr < rede3->lisREG12->Count; nr++)
   {
   trafo12a = (TTrafo12 *) rede3->lisREG12->Items[nr];
   lisEQP->Add(trafo12a);
   }

// Verifica se h� reguladores 'AUTOTRAFO6' em paralelo (n�o permitido)
for(int ne=0; ne < lisEQP->Count; ne++)
   {
   trafo12a = (TTrafo12 *) lisEQP->Items[ne];
   if((trafo12a->tipo != AUTOTRAFO6_1T) && (trafo12a->tipo != AUTOTRAFO6_2T)) continue;
   bar1 = trafo12a->bar1;
   bar2 = trafo12a->bar2;
   for(int ne2=(ne+1); ne2 < lisEQP->Count; ne2++)
      {
      trafo12b = (TTrafo12 *) lisEQP->Items[ne2];
      if((trafo12b->tipo != AUTOTRAFO6_1T) && (trafo12b->tipo != AUTOTRAFO6_2T)) continue;
      bar3  = trafo12b->bar1;
      bar4  = trafo12b->bar2;
      teste = ((bar1 == bar3) && (bar2 == bar4)) ||
              ((bar1 == bar4) && (bar2 == bar3));
      if(teste) {sucesso = false; break;}  // Erro
      }
   if(! sucesso) break;
   }
if(! sucesso)
   {
   delete lisEQP;
   return(false);
   }

// Loop de equipamentos
lisTEMP = new TList;
for(int ne=0; ne < lisEQP->Count; ne++)
   {
   trafo12a = (TTrafo12 *) lisEQP->Items[ne];
	if(trafo12a->tipo != AUTOTRAFO4)    continue;  // Tipo irrelevante
	if(trafo12a->fase_comum != faseINV) continue;  // Equipamento j� tratado
   lisTEMP->Clear();
   lisTEMP->Add(trafo12a);
   bar1 = trafo12a->bar1;
   bar2 = trafo12a->bar2;

   // Procura outros equipamentos 'AUTOTRAFO4' com mesmas barras terminais
   for(int ne2=(ne+1); ne2 < lisEQP->Count; ne2++)
      {
      trafo12b = (TTrafo12 *) lisEQP->Items[ne2];
		if(trafo12b->tipo != AUTOTRAFO4)    continue;  // Tipo irrelevante
		if(trafo12b->fase_comum != faseINV) continue;  // Equipamento j� tratado
      bar3  = trafo12b->bar1;
      bar4  = trafo12b->bar2;
      teste = ((bar1 == bar3) && (bar2 == bar4)) ||
              ((bar1 == bar4) && (bar2 == bar3));
      if(teste) lisTEMP->Add(trafo12b);  // 'trafo12a' e 'trafo12b' em paralelo
      }

   // Analisa conte�do de 'lisTEMP' e determina valor de 'fase_comum'
   f1    = trafo12a->fases_sec->Fase(0);  // 'trafo12a' � o primeiro em 'lisTEMP'
   f2    = trafo12a->fases_sec->Fase(1);
   soma1 = f1 + f2;
   switch(lisTEMP->Count)
      {
      case 1:  // Somente um equipamento ('trafo12a')
         if     ((f1 == faseN) || (f2 == faseN)) fase_comum = faseN;
         else if(soma1 == faseAB)                fase_comum = faseB;
         else if(soma1 == faseBC)                fase_comum = faseC;
         else                                    fase_comum = faseA;  // CA
         trafo12a->fase_comum = fase_comum;
         break;

      case 2:  // 2 equipamentos em paralelo
         trafo12b = (TTrafo12 *) lisTEMP->Items[1];
         f3       = trafo12b->fases_sec->Fase(0);
         f4       = trafo12b->fases_sec->Fase(1);
         soma2    = f3 + f4;
         if     (((f1 == faseN) || (f2 == faseN)) &&
                 ((f3 == faseN) || (f4 == faseN)))         fase_comum = faseN;
         else if(((soma1 == faseAB) && (soma2 == faseBC)) ||
                 ((soma1 == faseBC) && (soma2 == faseAB))) fase_comum = faseB;
         else if(((soma1 == faseBC) && (soma2 == faseCA)) ||
                 ((soma1 == faseCA) && (soma2 == faseBC))) fase_comum = faseC;
         else if(((soma1 == faseCA) && (soma2 == faseAB)) ||
                 ((soma1 == faseAB) && (soma2 == faseCA))) fase_comum = faseA;
         else {sucesso = false; break;}  // Erro
         trafo12a->fase_comum = fase_comum;
         trafo12b->fase_comum = fase_comum;
         break;

      case 3:  // 3 reguladores em paralelo - somente Estrela
         trafo12b = (TTrafo12 *) lisTEMP->Items[1];
         trafo12c = (TTrafo12 *) lisTEMP->Items[2];
         f3       = trafo12b->fases_sec->Fase(0);
         f4       = trafo12b->fases_sec->Fase(1);
         f5       = trafo12c->fases_sec->Fase(0);
         f6       = trafo12c->fases_sec->Fase(1);
         if(((f1 == faseN) || (f2 == faseN)) &&
            ((f3 == faseN) || (f4 == faseN)) &&
            ((f5 == faseN) || (f6 == faseN)))  // Estrela
            {
            trafo12a->fase_comum = faseN;
            trafo12b->fase_comum = faseN;
            trafo12c->fase_comum = faseN;
            }
         else  // Erro
            {
            sucesso = false;
            }
         break;

      default:  // Erro
         sucesso = false;
         break;
      } // switch
   if(! sucesso) break;
   } // for(ne)

// Libera mem�ria
delete lisEQP;
delete lisTEMP;

return(sucesso);
}

//---------------------------------------------------------------------------
void __fastcall TMonta3::ImprimeNumeracaoGlobal(int chave)
{
// Vari�veis locais
int        num_fases;
AnsiString filename1;
FILE       *arq;
TBarra     *bar1;
TFase      *fases;

// Abre arquivo
filename1 = geralC->DirTmp() + "\\NumeracaoGlobal.txt";
if(chave == 0) arq = fopen(filename1.c_str(), "w");
else           arq = fopen(filename1.c_str(), "a");
if(arq == NULL) return;

// Imprime numera��o global
if(chave != 0) fprintf(arq, "\n");
fprintf(arq, "\n Chave: %d", chave);
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   fprintf(arq, "\n    Barra: %s", bar1->codigo.c_str());
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      fprintf(arq, "\n       Fase: %1d   Linha global: %4d",
              fases->Fase(nfa), fases->LinhaFase(fases->Fase(nfa)));
      }
   }

// Fecha arquivo
fclose(arq);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::IncluiMutua(smcMUTUA *smc_mutua,
                                     TList    *lisMUTUA_TEMP)
   {
   int     iap, trec1_id, trec2_id;
   TBarra  *bar1, *bar2;
   TMutua  *mutua;
   TTrecho *trec1, *trec2;

   // Verifica dados da m�tua
   trec1_id = smc_mutua->trecho1_id;
   trec2_id = smc_mutua->trecho2_id;
   trec1    = rede3->LocalizaTrechoId(trec1_id);
   trec2    = rede3->LocalizaTrechoId(trec2_id);
   if((trec1 == NULL) || (trec2 == NULL)) return(false);
   if(! trec1->fases->FasesIguais(trec2->fases)) return(false); // Erro: trechos com fases distintas
   bar1 = rede3->LocalizaBarraId(smc_mutua->sentido.trecho1_bar_id);
   bar2 = rede3->LocalizaBarraId(smc_mutua->sentido.trecho2_bar_id);
   if((bar1 == NULL) || (bar2 == NULL)) return(false);

   // Inclui m�tua em 'lisMUTUA_TEMP'
   mutua = new TMutua(trec1, trec2);
   lisMUTUA_TEMP->Add(mutua);
   mutua->id     = smc_mutua->id;
   mutua->eqpto  = smc_mutua->eqpto;
   mutua->bar1   = bar1;
   mutua->bar2   = bar2;
   mutua->z0_ohm = smc_mutua->z0_ohm;
   mutua->z1_ohm = smc_mutua->z1_ohm;
   // S� vai montar 'mutua->mat_imped_ohm' ap�s a ordena��o das m�tuas
   // (a ordena��o pode alterar a ordem 'trec1'-'trec2' em cada m�tua)

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::IncluirMedidorModV(TBarra *bar1)
{
// Analisa tipo de refer�ncia
switch(geralC->TipoBarraRef)
   {
   case brFIXOS_1THETA_0V:
   case brFIXOS_nTHETA_0V:
      // Insere medidor medMOD_V (a tens�o ser� calculada em todas as barras)
      return(true);

   case brFIXOS_1THETA_1V:
      // N�o insere se a barra atual for a de refer�ncia (tens�o fixa);
      // insere caso contr�rio (a tens�o ser� calculada)
      if(bar1->offset == geralC->offset_bar_ref) return(false);
      else                                       return(true);

   case brFIXOS_nTHETA_nV:
   default:
      // N�o insere medidor medMOD_V (tens�o fixa em todas as barras)
      return(false);
   }
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::InicializaTensaoSuprimentos(void)
{
// IMPORTANTE: esta fun��o pressup�e que os �ngulos de refer�ncia das
//             das barras j� foram colocados em suas posi��es definitivas
//             atrav�s de uma chamada anterior a 'AcertaAngRefBarras()'

// Vari�veis locais
int    fase_int, ind_fase_bar, num_fases;
double ang1;
TFase  *fases;
TSup   *sup1;

// Loop de suprimentos
for(int ns=0; ns < rede3->lisSUP->Count; ns++)
   {
   sup1      = (TSup *) rede3->lisSUP->Items[ns];
   fases     = sup1->fases;
   num_fases = fases->NumFases();
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      // As fases do suprimento n�o s�o obrigatoriamente as mesmas da barra
      fase_int = fases->Fase(nfa);
      if(fase_int != faseN)  // Fases A, B ou C
         {
         ind_fase_bar = sup1->bar_int->fases->Indice(fase_int);
         if(ind_fase_bar < 0) return(false);
         ang1                     = sup1->bar_int->ang_ref_rad[ind_fase_bar];
         sup1->vet_tensao_pu[nfa] = polar(1., ang1);
         }
      else  // Neutro
         {
         sup1->vet_tensao_pu[nfa] = czero;
         }
      }
   }

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::InicializaYnodal(void)
   {
   // Vari�veis locais
   bool    sucesso;
   clock_t inicio;

   // Inicializa��o
   inicio  = clock();
   sucesso = true;

   // Deleta objeto SisLin, se tiver sido criado anteriormente
   delete Ynod; Ynod = NULL;

   // Determina linhas de cada barra em Ynodal e calcula tamanho da matriz
   if(! AtribuiLinhasY()) sucesso = false;

   // Cria objeto VTSisLin com tamanho calculado acima
   if(sucesso)
      {
      Ynod = DLL_NewObjSisLinC(geralC->num_lin_submatriz, geralC->num_tot_linhas,
                               geralC->tCPU, "Ynod");
      if(Ynod == NULL) sucesso = false;
      }

   // Acumula tempo de processamento
   geralC->tCPU->Acumula(ttINIC_Y, inicio);

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::InsereAterramentos(void)
{
bool            *vet_repet;
int             linha, num_linhas;
double          dordemh, imag1, real1;
complex<double> yater;
TBarra          *bar1;

// Inicializa��o
num_linhas = Ynod->NumTotalLinhas();
if(num_linhas <= 0) return(false);
dordemh = double(ordemh);

// Aloca e inicializa vetor de controle de repeti��o (para n�o inserir
// mais de uma vez aterramentos comuns entre barras distintas)
vet_repet = new bool [num_linhas];
for(int nl=0; nl < num_linhas; nl++)
   {
   vet_repet[nl] = false;
   }

// Loop de barras
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   if(! bar1->flag_conexa)    continue;   // Barra desconexa
   if(bar1->tipo_ater != atZ) continue;   // Aterramento diferente de atZ
   linha = bar1->fases->LinhaFase(faseN); // Linha do neutro atZ
   if(linha < 0)              continue;   // Prote��o
   if(vet_repet[linha])       continue;   // J� inseriu aterramento nesta linha
   yater = bar1->yater_pu;
   if(ordemh > 1)
      {
      yater = cum / yater;  // Obt�m imped�ncia
      real1 = yater.real();
      imag1 = yater.imag() * dordemh;
      yater = cum / complex<double>(real1, imag1);  // Admit�ncia atualizada
      }
   Ynod->InsereDiagonal(linha, yater);
   vet_repet[linha] = true;  // Desabilita novas inser��es
   }

// Libera mem�ria
delete[] vet_repet;

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::InsereCLsH(void)
{
// Vari�veis locais
TBarra *bar1;

// Loop de barras
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   if(! bar1->InsereCL_H(ordemh, geralC, Ynod)) return(false);
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::InsereEstaticoBarraH(void)
{
// Vari�veis locais
TBarra *bar1;

// Loop de barras
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   if(! bar1->InsereEstaticoH(ordemh, geralC, Ynod)) return(false);
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::InsereSuprimentosH(void)
{
// Local variables
TSup *sup1;

// Loop de suprimentos
for(int ns=0; ns < rede3->lisSUP->Count; ns++)
   {
   sup1 = (TSup *) rede3->lisSUP->Items[ns];
   if(! sup1->InsereYnodalH(ordemh, Ynod)) return(false);
   }

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::InsereZdef(bool flag_inverte)
{
// Local variables
int             fases_curto, lin1, lin2, tipo_curto;
double          sbase_fase_inv, vnom1_kv, vnom2_kv, zbase11, zbase12, zbase22;
complex<double> y11, y12, y22, ydef_S, ydef_fase_S, ydef_terra_S;
TBarra          *bar1;
TBarraDefeito   *bar_def;
TFase           *fases;

// Prote��o
if(curto_aux3 == NULL) return(true); // N�o foi especificado c�lculo de curto-circuito

// Inicializa��o
sbase_fase_inv = 1. / geralC->Sbase_fase;

// Loop de barras para defeito (mais de uma barra no caso de curto simult�neo)
for(int nb=0; nb < curto_aux3->lisBAR_DEF->Count; nb++)
   {
   bar_def = (TBarraDefeito *) curto_aux3->lisBAR_DEF->Items[nb];
   bar1    = bar_def->barra;

   // Prote��es
   if(bar1 == NULL)        return(true);  // Barra para defeito indefinida
   if(! bar1->flag_conexa) return(true);  // Barra para defeito desconexa

   // Obt�m demais dados
   tipo_curto  = bar_def->tipo_curto;
   fases_curto = bar_def->fases_curto;
   fases       = bar1->fases;

   // Obt�m imped�ncias de defeito no objeto TCurtoAux3
   if(! curto_aux3->ReiniciaImpedanciasDefeito(tipo_curto)) return(false);
   ydef_fase_S  = curto_aux3->ydef_fase_S;
   ydef_terra_S = curto_aux3->ydef_terra_S;
   if(flag_inverte)  // Remove admit�ncia de Ynodal (adiciona com sinal trocado)
      {
      ydef_fase_S  = - ydef_fase_S;
      ydef_terra_S = - ydef_terra_S;
      }

   // Insere admit�ncia de defeito em Ynodal, de acordo com o tipo de
   // curto especificado
   switch(fases_curto)
      {
      case ctAN:
      case ctBN:
      case ctCN:
      case ctAB:
      case ctBC:
      case ctCA:
         if(fases_curto == ctAN)
            {
            lin1     = fases->LinhaFase(faseA);
            lin2     = fases->LinhaFase(faseN);
            vnom1_kv = bar1->VnomFase_kv(faseA);
            vnom2_kv = bar1->VnomFase_kv(faseN);
            ydef_S   = ydef_terra_S;
            }
         else if(fases_curto == ctBN)
            {
            lin1     = fases->LinhaFase(faseB);
            lin2     = fases->LinhaFase(faseN);
            vnom1_kv = bar1->VnomFase_kv(faseB);
            vnom2_kv = bar1->VnomFase_kv(faseN);
            ydef_S   = ydef_terra_S;
            }
         else if(fases_curto == ctCN)
            {
            lin1     = fases->LinhaFase(faseC);
            lin2     = fases->LinhaFase(faseN);
            vnom1_kv = bar1->VnomFase_kv(faseC);
            vnom2_kv = bar1->VnomFase_kv(faseN);
            ydef_S   = ydef_terra_S;
            }
         else if(fases_curto == ctAB)
            {
            lin1     = fases->LinhaFase(faseA);
            lin2     = fases->LinhaFase(faseB);
            vnom1_kv = bar1->VnomFase_kv(faseA);
            vnom2_kv = bar1->VnomFase_kv(faseB);
            ydef_S   = ydef_fase_S;
            }
         else if(fases_curto == ctBC)
            {
            lin1     = fases->LinhaFase(faseB);
            lin2     = fases->LinhaFase(faseC);
            vnom1_kv = bar1->VnomFase_kv(faseB);
            vnom2_kv = bar1->VnomFase_kv(faseC);
            ydef_S   = ydef_fase_S;
            }
         else  // ctCA
            {
            lin1     = fases->LinhaFase(faseC);
            lin2     = fases->LinhaFase(faseA);
            vnom1_kv = bar1->VnomFase_kv(faseC);
            vnom2_kv = bar1->VnomFase_kv(faseA);
            ydef_S   = ydef_fase_S;
            }
         if((lin1 < 0) || (lin2 < 0)) return(false);  // Erro
         zbase11 = vnom1_kv * vnom1_kv * sbase_fase_inv;
         zbase12 = vnom1_kv * vnom2_kv * sbase_fase_inv;
         zbase22 = vnom2_kv * vnom2_kv * sbase_fase_inv;
         y11     = ydef_S * zbase11;
         y12     = ydef_S * zbase12;
         y22     = ydef_S * zbase22;
         Ynod->InsereDiagonal(lin1, y11);
         Ynod->InsereDiagonal(lin2, y22);
         Ynod->InsereSimetrico(lin1, lin2, -(y12));
         break;

      case ctAT:
      case ctBT:
      case ctCT:
         if(fases_curto == ctAT)
            {
            lin1     = fases->LinhaFase(faseA);
            vnom1_kv = bar1->VnomFase_kv(faseA);
            }
         else if(fases_curto == ctBT)
            {
            lin1     = fases->LinhaFase(faseB);
            vnom1_kv = bar1->VnomFase_kv(faseB);
            }
         else  // ctCT
            {
            lin1     = fases->LinhaFase(faseC);
            vnom1_kv = bar1->VnomFase_kv(faseC);
            }
         if(lin1 < 0) return(false);  // Erro
         zbase11 = vnom1_kv * vnom1_kv * sbase_fase_inv;
         y11     = ydef_terra_S * zbase11;
         Ynod->InsereDiagonal(lin1, y11);
         break;

      case ctABN:
      case ctBCN:
      case ctCAN:
         lin2 = fases->LinhaFase(faseN);
         if(lin2 < 0) return(false);  // Erro
         vnom2_kv = bar1->VnomFase_kv(faseN);
         zbase22  = vnom2_kv * vnom2_kv * sbase_fase_inv;
         y22      = ydef_terra_S * zbase22;
         for(int nv=0; nv < 2; nv++)
            {
            if(((nv == 0) && (fases_curto == ctABN)) ||
               ((nv == 1) && (fases_curto == ctCAN)))
               {
               lin1     = fases->LinhaFase(faseA);
               vnom1_kv = bar1->VnomFase_kv(faseA);
               }
            else if(((nv == 0) && (fases_curto == ctBCN)) ||
                    ((nv == 1) && (fases_curto == ctABN)))
               {
               lin1     = fases->LinhaFase(faseB);
               vnom1_kv = bar1->VnomFase_kv(faseB);
               }
            else  // ctCAN (nv == 0) ou ctBCN (nv == 1)
               {
               lin1     = fases->LinhaFase(faseC);
               vnom1_kv = bar1->VnomFase_kv(faseC);
               }
            if(lin1 < 0) return(false);  // Erro
            zbase11 = vnom1_kv * vnom1_kv * sbase_fase_inv;
            zbase12 = vnom1_kv * vnom2_kv * sbase_fase_inv;
            y11     = ydef_terra_S * zbase11;
            y12     = ydef_terra_S * zbase12;
            Ynod->InsereDiagonal(lin1, y11);
            Ynod->InsereDiagonal(lin2, y22);
            Ynod->InsereSimetrico(lin1, lin2, -(y12));
            } // for(nv)
         break;

      case ctABT:
      case ctBCT:
      case ctCAT:
         // IMPORTANTE: as admit�ncias usadas neste caso s�o
         //             'curto_aux3->yfase_fase_S' e 'curto_aux3->yfase_terra_S'.
         // Considera-se que existem 3 admit�ncias ligadas em estrela entre
         // B, C e T e a partir da� constroi-se o tri�ngulo equivalente (tudo
         // isto porque o n� correspondente ao centro-estrela N�O est� definido
         // na rede). Vide 'TCurtoAux::ReiniciaImpedanciasDefeito()'.
         if(fases_curto == ctABT)
            {
            lin1     = fases->LinhaFase(faseA);
            lin2     = fases->LinhaFase(faseB);
            vnom1_kv = bar1->VnomFase_kv(faseA);
            vnom2_kv = bar1->VnomFase_kv(faseB);
            }
         else if(fases_curto == ctBCT)
            {
            lin1     = fases->LinhaFase(faseB);
            lin2     = fases->LinhaFase(faseC);
            vnom1_kv = bar1->VnomFase_kv(faseB);
            vnom2_kv = bar1->VnomFase_kv(faseC);
            }
         else  // ctCAT
            {
            lin1     = fases->LinhaFase(faseC);
            lin2     = fases->LinhaFase(faseA);
            vnom1_kv = bar1->VnomFase_kv(faseC);
            vnom2_kv = bar1->VnomFase_kv(faseA);
            }
         if((lin1 < 0) || (lin2 < 0)) return(false);  // Erro
         zbase11 = vnom1_kv * vnom1_kv * sbase_fase_inv;
         zbase12 = vnom1_kv * vnom2_kv * sbase_fase_inv;
         zbase22 = vnom2_kv * vnom2_kv * sbase_fase_inv;
         y11     = curto_aux3->yfase_fase_S * zbase11;
         y12     = curto_aux3->yfase_fase_S * zbase12;
         y22     = curto_aux3->yfase_fase_S * zbase22;
         Ynod->InsereDiagonal(lin1, y11);
         Ynod->InsereDiagonal(lin2, y22);
         Ynod->InsereSimetrico(lin1, lin2, -(y12));
         y11 = curto_aux3->yfase_terra_S * zbase11;
         y22 = curto_aux3->yfase_terra_S * zbase22;
         Ynod->InsereDiagonal(lin1, y11);
         Ynod->InsereDiagonal(lin2, y22);
         break;

      case ctABC:
         for(int nv=0; nv < 3; nv++)
            {
            if(nv == 0)
               {
               lin1     = fases->LinhaFase(faseA);
               lin2     = fases->LinhaFase(faseB);
               vnom1_kv = bar1->VnomFase_kv(faseA);
               vnom2_kv = bar1->VnomFase_kv(faseB);
               }
            else if(nv == 1)
               {
               lin1     = fases->LinhaFase(faseB);
               lin2     = fases->LinhaFase(faseC);
               vnom1_kv = bar1->VnomFase_kv(faseB);
               vnom2_kv = bar1->VnomFase_kv(faseC);
               }
            else  // nv == 2
               {
               lin1     = fases->LinhaFase(faseC);
               lin2     = fases->LinhaFase(faseA);
               vnom1_kv = bar1->VnomFase_kv(faseC);
               vnom2_kv = bar1->VnomFase_kv(faseA);
               }
            if((lin1 < 0) || (lin2 < 0)) return(false);  // Erro
            zbase11 = vnom1_kv * vnom1_kv * sbase_fase_inv;
            zbase12 = vnom1_kv * vnom2_kv * sbase_fase_inv;
            zbase22 = vnom2_kv * vnom2_kv * sbase_fase_inv;
            y11     = ydef_fase_S * zbase11;
            y12     = ydef_fase_S * zbase12;
            y22     = ydef_fase_S * zbase22;
            Ynod->InsereDiagonal(lin1, y11);
            Ynod->InsereDiagonal(lin2, y22);
            Ynod->InsereSimetrico(lin1, lin2, -(y12));
            } // for(nv)
         break;

      case ctABCN:
         lin2 = fases->LinhaFase(faseN);
         if(lin2 < 0) return(false);  // Erro
         vnom2_kv = bar1->VnomFase_kv(faseN);
         zbase22  = vnom2_kv * vnom2_kv * sbase_fase_inv;
         y22      = ydef_fase_S * zbase22;
         for(int nv=0; nv < 3; nv++)
            {
            if(nv == 0)
               {
               lin1     = fases->LinhaFase(faseA);
               vnom1_kv = bar1->VnomFase_kv(faseA);
               }
            else if(nv == 1)
               {
               lin1     = fases->LinhaFase(faseB);
               vnom1_kv = bar1->VnomFase_kv(faseB);
               }
            else  // nv == 2
               {
               lin1     = fases->LinhaFase(faseC);
               vnom1_kv = bar1->VnomFase_kv(faseC);
               }
            if(lin1 < 0) return(false);  // Erro
            zbase11 = vnom1_kv * vnom1_kv * sbase_fase_inv;
            zbase12 = vnom1_kv * vnom2_kv * sbase_fase_inv;
            y11     = ydef_fase_S * zbase11;
            y12     = ydef_fase_S * zbase12;
            Ynod->InsereDiagonal(lin1, y11);
            Ynod->InsereDiagonal(lin2, y22);
            Ynod->InsereSimetrico(lin1, lin2, -(y12));
            } // for(nv)
         break;

      case ctABCT:
         for(int nv=0; nv < 3; nv++)
            {
            if(nv == 0)
               {
               lin1     = fases->LinhaFase(faseA);
               vnom1_kv = bar1->VnomFase_kv(faseA);
               }
            else if(nv == 1)
               {
               lin1     = fases->LinhaFase(faseB);
               vnom1_kv = bar1->VnomFase_kv(faseB);
               }
            else  // nv == 2
               {
               lin1     = fases->LinhaFase(faseC);
               vnom1_kv = bar1->VnomFase_kv(faseC);
               }
            if(lin1 < 0) return(false);  // Erro
            zbase11 = vnom1_kv * vnom1_kv * sbase_fase_inv;
            y11     = ydef_fase_S * zbase11;
            Ynod->InsereDiagonal(lin1, y11);
            } // for(nv)
         break;

      default:  // Nada a fazer
         break;
      } // switch()
   } // for(nb)

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::MontaLigacoesBarra(void)
{
char        string1[30];
int         tipo;
AnsiString  cod_bar2, codigo;
TBancoDF    *bancoDF;
TBarra      *bar1;
TCapSerie   *caps1;
TLigacaoC   *ligacao;
TTrafo12    *trafo12;
TTrafo32    *trafo32;
TTrafo33    *trafo33;
TTrafoEquiv *trafoequiv;
TTrafoZZ    *trafozz;
TTrecho     *trec1;
void        *plig;

// Retorna se esta montagem j� foi feita anteriormente
if(flag_monta_lig_barra) return(true);

// Cria ou limpa listas de liga��es (todas as barras)
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   if(bar1->lisLIG == NULL) bar1->lisLIG = new TList;
   else                     rede3->LimpaLista(bar1->lisLIG);
   }

// Loop de bancos em Delta Fechado
for(int nb=0; nb < rede3->lisBANCO_DF->Count; nb++)
   {
   bancoDF = (TBancoDF *) rede3->lisBANCO_DF->Items[nb];
   plig    = (void *) bancoDF;
   codigo.sprintf("DFechado%d", (nb+1));
   ligacao = new TLigacaoC(plig, BancoDF, 0, bancoDF->bar_BT->codigo, codigo);
   bancoDF->bar_AT->lisLIG->Add(ligacao);
   ligacao = new TLigacaoC(plig, BancoDF, 1, bancoDF->bar_AT->codigo, codigo);
   bancoDF->bar_BT->lisLIG->Add(ligacao);
   }

// Loop de Capacitores S�rie
for(int nl=0; nl < rede3->lisCAPS->Count; nl++)
   {
   caps1   = (TCapSerie *) rede3->lisCAPS->Items[nl];
   plig    = (void *) caps1;
   ligacao = new TLigacaoC(plig, CapSerie3, 0, caps1->bar2->codigo, caps1->codigo);
   caps1->bar1->lisLIG->Add(ligacao);
   ligacao = new TLigacaoC(plig, CapSerie3, 1, caps1->bar1->codigo, caps1->codigo);
   caps1->bar2->lisLIG->Add(ligacao);
   }

// Loop de Transformadores e Reguladores de Tens�o Monof�sicos
for(int nl=0; nl < rede3->lis12->Count; nl++)
   {
   trafo12 = (TTrafo12 *) rede3->lis12->Items[nl];
   if(trafo12->bancoDF != NULL) continue; // Descarta trafo que faz parte de banco DF
   plig    = (void *) trafo12;
   if(trafo12->flag_reg12) tipo = Reg12;
   else                    tipo = Trafo12;
   ligacao = new TLigacaoC(plig, tipo, 0, trafo12->bar2->codigo, trafo12->codigo);
   trafo12->bar1->lisLIG->Add(ligacao);
   ligacao = new TLigacaoC(plig, tipo, 1, trafo12->bar1->codigo, trafo12->codigo);
   trafo12->bar2->lisLIG->Add(ligacao);
   }

// Loop de Transformadores e Reguladores de Tens�o Trif�sicos
for(int nl=0; nl < rede3->lis32->Count; nl++)
   {
   trafo32 = (TTrafo32 *) rede3->lis32->Items[nl];
   plig    = (void *) trafo32;
   if(trafo32->flag_reg32) tipo = Reg32;
   else                    tipo = Trafo32;
   ligacao = new TLigacaoC(plig, tipo, 0, trafo32->bar2->codigo, trafo32->codigo);
   trafo32->bar1->lisLIG->Add(ligacao);
   ligacao = new TLigacaoC(plig, tipo, 1, trafo32->bar1->codigo, trafo32->codigo);
   trafo32->bar2->lisLIG->Add(ligacao);
   }

// Loop de Transformadores Trif�sicos de 3 enrolamentos
for(int nl=0; nl < rede3->lisTRAFO33->Count; nl++)
   {
   trafo33 = (TTrafo33 *) rede3->lisTRAFO33->Items[nl];
   plig    = (void *) trafo33;
   sprintf(string1, "Trafo3 %d", (nl+1));
   cod_bar2 = AnsiString(string1) + " Pri.";
   ligacao  = new TLigacaoC(plig, Trafo33, 0, cod_bar2, trafo33->codigo);
   trafo33->bar1->lisLIG->Add(ligacao);
   cod_bar2 = AnsiString(string1) + " Sec.";
   ligacao  = new TLigacaoC(plig, Trafo33, 1, cod_bar2, trafo33->codigo);
   trafo33->bar2->lisLIG->Add(ligacao);
   cod_bar2 = AnsiString(string1) + " Ter.";
   ligacao  = new TLigacaoC(plig, Trafo33, 2, cod_bar2, trafo33->codigo);
   trafo33->bar3->lisLIG->Add(ligacao);
   }

// Loop de Transformadores Equivalentes
for(int nl=0; nl < rede3->lisTRAFOEQUIV->Count; nl++)
   {
   trafoequiv = (TTrafoEquiv *) rede3->lisTRAFOEQUIV->Items[nl];
   plig       = (void *) trafoequiv;
   ligacao    = new TLigacaoC(plig, TrafoEquiv3, 0, trafoequiv->bar2->codigo, trafoequiv->codigo);
   trafoequiv->bar1->lisLIG->Add(ligacao);
   ligacao    = new TLigacaoC(plig, TrafoEquiv3, 1, trafoequiv->bar1->codigo, trafoequiv->codigo);
   trafoequiv->bar2->lisLIG->Add(ligacao);
   }

// Loop de Transformadores ZZ
for(int nl=0; nl < rede3->lisTRAFOZZ->Count; nl++)
   {
   trafozz = (TTrafoZZ *) rede3->lisTRAFOZZ->Items[nl];
   plig    = (void *) trafozz;
   sprintf(string1, "ZigZag%d", (nl+1));
   cod_bar2 = AnsiString(string1);
   ligacao  = new TLigacaoC(plig, TrafoZZ3, 0, cod_bar2, trafozz->codigo);
   trafozz->barra->lisLIG->Add(ligacao);
   }

// Loop de Trechos
for(int nl=0; nl < rede3->lisTRECHO->Count; nl++)
   {
   trec1   = (TTrecho *) rede3->lisTRECHO->Items[nl];
   plig    = (void *) trec1;
   ligacao = new TLigacaoC(plig, Trecho3, 0, trec1->bar2->codigo, trec1->codigo);
   trec1->bar1->lisLIG->Add(ligacao);
   ligacao = new TLigacaoC(plig, Trecho3, 1, trec1->bar1->codigo, trec1->codigo);
   trec1->bar2->lisLIG->Add(ligacao);
   }

// Ordena lista de liga��es em ordem crescente das barras KC
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   OrdenaLigacoesBarra(bar1->lisLIG);
   }

// Liga flag de controle
flag_monta_lig_barra = true;

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::MontaListaBancosDF(void)
{
// Vari�veis locais
TBancoDF *bancoDF;
TTrafo12 *pdf, *trafo1, *trafo2;

// Limpa lista
rede3->lisBANCO_DF->Clear();

// Retorna se o flag de montagem dos bancos DF estiver desligado
if(! geralC->flag_utiliza_bancoDF) return(true);

// Loop externo na lista de TTrafo12
for(int nt1=0; nt1 < rede3->lisTRAFO12->Count; nt1++)
   {
   trafo1 = (TTrafo12 *) rede3->lisTRAFO12->Items[nt1];
   pdf    = (TTrafo12 *) trafo1->bancoDF;
   // Aten��o � condi��o abaixo, cf. inicializa��o em 'TTrafo12::TTrafo12()':
   //    (a) trafo1->bancoDF == NULL: trafo n�o pertence a banco
   //    (b) trafo1->bancoDF == trafo1: trafo pertence a banco mas n�o foi agrupado ainda
   //    (c) (trafo1->bancoDF != NULL) && (trafo1->bancoDF != trafo1): trafo j� agrupado
   if(pdf == NULL)   continue;  // Descarta trafo na situa��o (a)
   if(pdf != trafo1) continue;  // Descarta trafo na situa��o (c)

   // 'trafo1' na situa��o (b): cria banco com o primeiro trafo e adiciona banco � lista
   bancoDF = new TBancoDF(trafo1);
   rede3->lisBANCO_DF->Add(bancoDF);

   // Loop interno na lista de TTrafo12, a partir de (nt1+1)
   for(int nt2=(nt1+1); nt2 < rede3->lisTRAFO12->Count; nt2++)
      {
      trafo2 = (TTrafo12 *) rede3->lisTRAFO12->Items[nt2];
      pdf    = (TTrafo12 *) trafo2->bancoDF;
      if(pdf == NULL)   continue;  // Descarta trafo na situa��o (a)
      if(pdf != trafo2) continue;  // Descarta trafo na situa��o (c)

      // 'trafo2' na situa��o (b): verifica igualdade de barras terminais com 'trafo1'
      if((trafo2->bar1 != trafo1->bar1) || (trafo2->bar2 != trafo1->bar2)) continue; // Descarta trafo irrelevante

      // Adiciona 'trafo2' ao banco atual
      bancoDF->AddTrafo(trafo2);
      }
   }

// Verifica (1) se cada banco tem 3 trafos, e (2) se cada banco tem 2 TRAFO4 e 1 TRAFO5
for(int nb=0; nb < rede3->lisBANCO_DF->Count; nb++)
   {
   bancoDF = (TBancoDF *) rede3->lisBANCO_DF->Items[nb];
   if(! bancoDF->OK()) return(false);
   }

// Retorna OK
return(true);
}

//---------------------------------------------------------------------------
void __fastcall TMonta3::MontaListasTrafosReguladores(void)
{
// Vari�veis locais
TReg12   *reg12;
TReg32   *reg32;
TTrafo12 *trafo12;
TTrafo32 *trafo32;

// Limpa listas
rede3->lis12->Clear();
rede3->lis32->Clear();

// Monta lista TRAFO12 + REG12
for(int nt=0; nt < rede3->lisTRAFO12->Count; nt++)
   {
   trafo12 = (TTrafo12 *) rede3->lisTRAFO12->Items[nt];
   rede3->lis12->Add(trafo12);
   }
for(int nr=0; nr < rede3->lisREG12->Count; nr++)
   {
   reg12   = (TReg12 *) rede3->lisREG12->Items[nr];
   trafo12 = (TTrafo12 *) reg12;
   rede3->lis12->Add(trafo12);
   }

// Monta lista TRAFO32 + REG32
for(int nt=0; nt < rede3->lisTRAFO32->Count; nt++)
   {
   trafo32 = (TTrafo32 *) rede3->lisTRAFO32->Items[nt];
   rede3->lis32->Add(trafo32);
   }
for(int nr=0; nr < rede3->lisREG32->Count; nr++)
   {
   reg32   = (TReg32 *) rede3->lisREG32->Items[nr];
   trafo32 = (TTrafo32 *) reg32;
   rede3->lis32->Add(trafo32);
   }
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::MontaRedeEstimador3(void)
{
// Vari�veis locais
clock_t inicio;

inicio = clock();
if(! AddBarra())      return(false);
if(! AddSuprimento()) return(false);
if(! AddTrecho())     return(false);
if(! AddTramo())      return(false);
if(! AddCapSerie())   return(false);
if(! AddTrafo())      return(false);
if(! AddTrafo33())    return(false);
if(! AddTrafoZZ())    return(false);
if(! AddReg())        return(false);
if(! AddChave())      return(false);

// Verifica exist�ncia de AUTOTRAFO6 em paralelo (n�o permitido) e
// determina fase comum de trafos e reguladores AUTOTRAFO4
if(! DeterminaFaseComumAUTOTRAFO4()) return(false);

if(! AddYrefReduc())   return(false);
if(! AddAterramento()) return(false);
if(! AddCarga())       return(false);
if(! AddBateria())     return(false);

// Adiciona Cargas N�o-Lineares, se indicado pelo flag
if(geralC->flag_inclui_CNLs)
   {
   if(! AddCNL()) return(false);
   }

// Verifica tens�o nominal das barras
if(! VerificaTensaoNominalBarras()) return(false);

// Acerta matrizes de trechos (passa para [pu]). N�o � poss�vel fazer isso
// na leitura de trechos porque naquele momento as tens�es nominais das
// barras podem n�o estar todas definidas, j� que a leitura de trechos,
// trafos, etc. adiciona fases �s barras (HPS, 2018.03.29).
AcertaMatrizesTrechos();

// Acerta �ngulo de refer�ncia das barras (passa da indexa��o {A=0, B=1,
// C=2, N=3} para a indexa��o das fases efetivamente existentes em cada barra)
AcertaAngRefBarras();

// Inicializa tens�o nos suprimentos (importante pois a atualiza��o de
// suprimentos em cada patamar pega o �ngulo anterior da tens�o)
if(! InicializaTensaoSuprimentos()) return(false);

// A ordena��o de barras deve ser feita AP�S a leitura de reguladores de
// tens�o (a barra de tens�o controlada � redefinida para tipo PQV)
// if(! VerificaFasesSuprimentos()) return(false);
if(! OrdenaBarrasSup(rede3->lisBAR, geralC, rede3->lisSUP)) return(false);

// Adiciona medidores reais e virtuais
// IMPORTANTE: os medidores virtuais devem ser adicionados DEPOIS da
//             ordena��o de barras e suprimentos
if(! AddMedidorReal())    return(false);
rede3->lisMED->Sort(EscolheMedidor);
if(! AddMedidorVirtual()) return(false);

// L� imped�ncias m�tuas
if(! AddMutua()) return(false);

// Monta lista de bancos de trafos monof�sicos ligados em Delta-Fechado
// (cada banco � tratado como entidade �nica na montagem de Ynodal, pois
//  no prim�rio eles s�o ligados em estrela isolada e assim o
//  centro-estrela � eliminado)
if(! MontaListaBancosDF()) return(false);

// Monta listas de trafos e reguladores
// IMPORTANTE: tem que ser antes de 'MontaLigacoesBarra()'
MontaListasTrafosReguladores();

// Monta listas de ligacoes por barra
if(! MontaLigacoesBarra()) return(false);

// Aterra trafos e reguladores monof�sicos que possuem neutro atIsolado
// sem nenhuma conex�o com a rede
if(! AnalisaAterramentoTrafosReguladoresMono()) return(false);

// Aterra barras que possuem carga, capacitor ou reator aterrado
if(! AterraBarrasPorCargaOuEstatico()) return(false);

// Atribui IDs pendentes
rede3->AtribuiIDs();

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttMONTA_REDE, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::MontaRedeFluxoH3(void)
{
if(! AddBarra())      return(false);
if(! AddSuprimento()) return(false);
if(! AddTrecho())     return(false);
if(! AddTramo())      return(false);
if(! AddCapSerie())   return(false);
if(! AddTrafo())      return(false);
if(! AddTrafo33())    return(false);
if(! AddTrafoZZ())    return(false);
if(! AddReg())        return(false);
if(! AddChave())      return(false);

// Verifica exist�ncia de AUTOTRAFO6 em paralelo (n�o permitido) e
// determina fase comum de trafos e reguladores AUTOTRAFO4
if(! DeterminaFaseComumAUTOTRAFO4()) return(false);

if(! AddYrefReduc())   return(false);
if(! AddAterramento()) return(false);
if(! AddCarga())       return(false);
if(! AddBateria())     return(false);

// Adiciona Cargas N�o-Lineares, se indicado pelo flag
if(geralC->flag_inclui_CNLs)
   {
   if(! AddCNL()) return(false);
   }

// Verifica tens�o nominal das barras
if(! VerificaTensaoNominalBarras()) return(false);

// Acerta matrizes de trechos (passa para [pu]). N�o � poss�vel fazer isso
// na leitura de trechos porque naquele momento as tens�es nominais das
// barras podem n�o estar todas definidas, j� que a leitura de trechos,
// trafos, etc. adiciona fases �s barras (HPS, 2018.03.29).
AcertaMatrizesTrechos();

// Acerta �ngulo de refer�ncia das barras (passa da indexa��o {A=0, B=1,
// C=2, N=3} para a indexa��o das fases efetivamente existentes em cada barra)
AcertaAngRefBarras();

// Inicializa tens�o nos suprimentos (importante pois a atualiza��o de
// suprimentos em cada patamar pega o �ngulo anterior da tens�o)
if(! InicializaTensaoSuprimentos()) return(false);

// A ordena��o de barras deve ser feita AP�S a leitura de reguladores de
// tens�o (a barra de tens�o controlada � redefinida para tipo PQV).
// Embora a ordena��o de barras seja feita dentro do loop de patamares,
// ela deve ser feita aqui tamb�m por causa da chamada de
// TMonta3::MontaLinhasY(), que permite determinar o tamanho de Ynodal.
// if(! VerificaFasesSuprimentos()) return(false);
if(! OrdenaBarrasSup(rede3->lisBAR, geralC, rede3->lisSUP)) return(false);

// L� imped�ncias m�tuas
if(! AddMutua()) return(false);

// Monta lista de bancos de trafos monof�sicos ligados em Delta-Fechado
// (cada banco � tratado como entidade �nica na montagem de Ynodal, pois
//  no prim�rio eles s�o ligados em estrela isolada e assim o
//  centro-estrela � eliminado)
if(! MontaListaBancosDF()) return(false);

// Monta listas de trafos e reguladores
// IMPORTANTE: tem que ser antes de 'MontaLigacoesBarra()'
MontaListasTrafosReguladores();

// Monta listas de ligacoes por barra
if(! MontaLigacoesBarra()) return(false);

// Aterra trafos e reguladores monof�sicos que possuem neutro atIsolado
// sem nenhuma conex�o com a rede
if(! AnalisaAterramentoTrafosReguladoresMono()) return(false);

// Aterra barras que possuem carga, capacitor ou reator aterrado
if(! AterraBarrasPorCargaOuEstatico()) return(false);

// Atribui IDs pendentes
rede3->AtribuiIDs();

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::MontaRedeGauss3(void)
{
// Vari�veis locais
clock_t inicio;

inicio = clock();
if(! AddBarra())                return(false);
if(! AddSuprimento())           return(false);
if(! AddTrecho())               return(false);
if(! AddBarraTrechoFicticios()) return(false);
if(! AddTramo())                return(false);

// Adiciona barra interna dos suprimentos e os correspondentes
// trechos internos, se indicado pelo flag
if(geralC->todos_sup_barra_interna)
   {
   if(! AddBarraInternaSuprimento())  return(false);
   if(! AddTrechoInternoSuprimento()) return(false);
   }

if(! AddCapSerie()) return(false);
if(! AddTrafo())    return(false);
if(! AddTrafo33())  return(false);
if(! AddTrafoZZ())  return(false);
if(! AddReg())      return(false);

// Verifica exist�ncia de AUTOTRAFO6 em paralelo (n�o permitido) e
// determina fase comum de trafos e reguladores AUTOTRAFO4
if(! DeterminaFaseComumAUTOTRAFO4()) return(false);

if(! AddYrefReduc())   return(false);
if(! AddAterramento()) return(false);
if(! AddCarga())       return(false);
if(! AddBateria())     return(false);

// Adiciona Cargas N�o-Lineares, se indicado pelo flag
if(geralC->flag_inclui_CNLs)
   {
   if(! AddCNL()) return(false);
   }

// Verifica tens�o nominal das barras
if(! VerificaTensaoNominalBarras()) return(false);

// Acerta matrizes de trechos (passa para [pu]). N�o � poss�vel fazer isso
// na leitura de trechos porque naquele momento as tens�es nominais das
// barras podem n�o estar todas definidas, j� que a leitura de trechos,
// trafos, etc. adiciona fases �s barras (HPS, 2018.03.29).
AcertaMatrizesTrechos();

// Acerta �ngulo de refer�ncia das barras (passa da indexa��o {A=0, B=1,
// C=2, N=3} para a indexa��o das fases efetivamente existentes em cada barra)
AcertaAngRefBarras();

// Inicializa tens�o nos suprimentos (importante pois a atualiza��o de
// suprimentos em cada patamar pega o �ngulo anterior da tens�o)
if(! InicializaTensaoSuprimentos()) return(false);

// A ordena��o de barras deve ser feita AP�S a leitura de reguladores de
// tens�o (a barra de tens�o controlada � redefinida para tipo PQV).
// if(! VerificaFasesSuprimentos()) return(false);
// if(! OrdenaBarrasSup(rede3->lisBAR, geralC, rede3->lisSUP)) return(false);

// L� imped�ncias m�tuas
if(! AddMutua()) return(false);

// Monta lista de bancos de trafos monof�sicos ligados em Delta-Fechado
// (cada banco � tratado como entidade �nica na montagem de Ynodal, pois
//  no prim�rio eles s�o ligados em estrela isolada e assim o
//  centro-estrela � eliminado)
if(! MontaListaBancosDF()) return(false);

// Monta listas de trafos e reguladores
// IMPORTANTE: tem que ser antes de 'MontaLigacoesBarra()'
MontaListasTrafosReguladores();

// Monta listas de ligacoes por barra
if(! MontaLigacoesBarra()) return(false);

// Aterra trafos e reguladores monof�sicos que possuem neutro atIsolado
// sem nenhuma conex�o com a rede
if(! AnalisaAterramentoTrafosReguladoresMono()) return(false);

// Aterra barras que possuem carga, capacitor ou reator aterrado
if(! AterraBarrasPorCargaOuEstatico()) return(false);

// Atribui IDs pendentes
rede3->AtribuiIDs();

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttMONTA_REDE, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::MontaRedeNR3(void)
{
// Vari�veis locais
clock_t inicio;

inicio = clock();
if(! AddBarra())                return(false);
if(! AddSuprimento())           return(false);
if(! AddTrecho())               return(false);
if(! AddBarraTrechoFicticios()) return(false);
if(! AddTramo())                return(false);

// Adiciona barra interna dos suprimentos e os correspondentes
// trechos internos, se indicado pelo flag
if(geralC->todos_sup_barra_interna)
   {
   if(! AddBarraInternaSuprimento())  return(false);
   if(! AddTrechoInternoSuprimento()) return(false);
   }

if(! AddCapSerie())    return(false);
if(! AddTrafo())       return(false);
if(! AddTrafo33())     return(false);
if(! AddTrafoZZ())     return(false);
if(! AddReg())         return(false);


// DEBUG ESTIMADOR3
// if(! AddChave())       return(false);


if(! AddYrefReduc())   return(false);
if(! AddAterramento()) return(false);
if(! AddCarga())       return(false);
if(! AddBateria())     return(false);

// Acerta imped�ncia dos reguladores de tens�o (somente NR3)
// if(! AcertaImpedanciaReguladoresNR3()) return(false);

// Verifica exist�ncia de AUTOTRAFO6 em paralelo (n�o permitido) e
// determina fase comum de trafos e reguladores AUTOTRAFO4
if(! DeterminaFaseComumAUTOTRAFO4()) return(false);

// Adiciona Cargas N�o-Lineares, se indicado pelo flag
if(geralC->flag_inclui_CNLs)
   {
   if(! AddCNL()) return(false);
   }

// Verifica tens�o nominal das barras
if(! VerificaTensaoNominalBarras()) return(false);

// Acerta matrizes de trechos (passa para [pu]). N�o � poss�vel fazer isso
// na leitura de trechos porque naquele momento as tens�es nominais das
// barras podem n�o estar todas definidas, j� que a leitura de trechos,
// trafos, etc. adiciona fases �s barras (HPS, 2018.03.29).
AcertaMatrizesTrechos();

// Acerta �ngulo de refer�ncia das barras (passa da indexa��o {A=0, B=1,
// C=2, N=3} para a indexa��o das fases efetivamente existentes em cada barra)
AcertaAngRefBarras();

// Inicializa tens�o nos suprimentos (importante pois a atualiza��o de
// suprimentos em cada patamar pega o �ngulo anterior da tens�o)
if(! InicializaTensaoSuprimentos()) return(false);

// Monta grupos de LTCs tipo PADR�O, monof�sicos e trif�sicos, com
// ajuste autom�tico de tap (para inclus�o de equa��es e vari�veis
// adicionais em NR3)
if(! AddGrupoLTC12()) return(false);
if(! AddGrupoLTC32()) return(false);

// A ordena��o de barras deve ser feita AP�S a leitura de reguladores de
// tens�o e montagem dos grupos de reguladores (a barra de tens�o controlada
// � redefinida para tipo PQV).
// if(! VerificaFasesSuprimentos()) return(false);
// if(! OrdenaBarrasSup(rede3->lisBAR, geralC, rede3->lisSUP)) return(false);

// L� imped�ncias m�tuas
if(! AddMutua()) return(false);

// Monta lista de bancos de trafos monof�sicos ligados em Delta-Fechado
// (cada banco � tratado como entidade �nica na montagem de Ynodal, pois
//  no prim�rio eles s�o ligados em estrela isolada e assim o
//  centro-estrela � eliminado)
if(! MontaListaBancosDF()) return(false);

// Monta listas de trafos e reguladores
// IMPORTANTE: tem que ser antes de 'MontaLigacoesBarra()'
MontaListasTrafosReguladores();

// Monta listas de ligacoes por barra
if(! MontaLigacoesBarra()) return(false);

// Aterra trafos e reguladores monof�sicos que possuem neutro atIsolado
// sem nenhuma conex�o com a rede
if(! AnalisaAterramentoTrafosReguladoresMono()) return(false);

// Aterra barras que possuem carga, capacitor ou reator aterrado
if(! AterraBarrasPorCargaOuEstatico()) return(false);

// Atribui IDs pendentes
rede3->AtribuiIDs();

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttMONTA_REDE, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::MontaRedeRedutor3(void)
{
if(! AddBarra())      return(false);
if(! AddSuprimento()) return(false);
if(! AddTrecho())     return(false);
if(! AddTramo())      return(false);
if(! AddCapSerie())   return(false);
if(! AddTrafo())      return(false);
if(! AddTrafo33())    return(false);
if(! AddTrafoZZ())    return(false);
if(! AddReg())        return(false);

// Verifica exist�ncia de AUTOTRAFO6 em paralelo (n�o permitido) e
// determina fase comum de trafos e reguladores AUTOTRAFO4
if(! DeterminaFaseComumAUTOTRAFO4()) return(false);

if(! AddYrefReduc())   return(false);
if(! AddAterramento()) return(false);

// Verifica tens�o nominal das barras
if(! VerificaTensaoNominalBarras()) return(false);

// Acerta matrizes de trechos (passa para [pu]). N�o � poss�vel fazer isso
// na leitura de trechos porque naquele momento as tens�es nominais das
// barras podem n�o estar todas definidas, j� que a leitura de trechos,
// trafos, etc. adiciona fases �s barras (HPS, 2018.03.29).
AcertaMatrizesTrechos();

// Acerta �ngulo de refer�ncia das barras (passa da indexa��o {A=0, B=1,
// C=2, N=3} para a indexa��o das fases efetivamente existentes em cada barra)
AcertaAngRefBarras();

// Inicializa tens�o nos suprimentos (importante pois a atualiza��o de
// suprimentos em cada patamar pega o �ngulo anterior da tens�o)
if(! InicializaTensaoSuprimentos()) return(false);

// A ordena��o de barras deve ser feita AP�S a leitura de reguladores de
// tens�o (a barra de tens�o controlada � redefinida para tipo PQV)
// NB: o retorno 'false' de 'OrdenaBarrasSup()' N�O � TRATADO AQUI porque
//     existe a possibilidade de que a rede n�o possua suprimento
// if(! VerificaFasesSuprimentos()) return(false);
OrdenaBarrasSup(rede3->lisBAR, geralC);

// L� imped�ncias m�tuas
if(! AddMutua()) return(false);

// Monta lista de bancos de trafos monof�sicos ligados em Delta-Fechado
// (cada banco � tratado como entidade �nica na montagem de Ynodal, pois
//  no prim�rio eles s�o ligados em estrela isolada e assim o
//  centro-estrela � eliminado)
if(! MontaListaBancosDF()) return(false);

// Monta listas de trafos e reguladores
// IMPORTANTE: tem que ser antes de 'MontaLigacoesBarra()'
MontaListasTrafosReguladores();

// Monta listas de ligacoes por barra
if(! MontaLigacoesBarra()) return(false);

// Aterra trafos e reguladores monof�sicos que possuem neutro atIsolado
// sem nenhuma conex�o com a rede
if(! AnalisaAterramentoTrafosReguladoresMono()) return(false);

// Aterra barras que possuem carga, capacitor ou reator aterrado
if(! AterraBarrasPorCargaOuEstatico()) return(false);

// Atribui IDs pendentes
rede3->AtribuiIDs();

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::MontaY(int  ordemh,
                                bool flag_inclui_mutuas,
                                bool so_trechos)
{
// Local variables
bool        flag_zpequena;
clock_t     inicio;
TBancoDF    *bancoDF;
TBarra      *bar1;
TCapSerie   *caps1;
TGrupoMutua *grupo;
TTrecho     *trec1;
TSup        *sup1;
TTrafo12    *trafo12;
TTrafo32    *trafo32;
TTrafo33    *trafo33;
TTrafoEquiv *trafoequiv;
TTrafoZZ    *trafozz;

// Prote��o
if(Ynod == NULL) return(false);

// Inicializa��o
inicio = clock();

// Salva 'ordemh'
this->ordemh = ordemh;

// Se o flag 'so_trechos' estiver ligado, coloca imped�ncia
// pequena em TTrecho (suprimentos), TTrafo12, TBancoDF, TTrafo32 e TTrafo33
// NB: no CurtoMA, os suprimentos s�o representados por trecho de rede (TTrecho)
flag_zpequena = so_trechos;

// Limpa a matriz
Ynod->ZeraMatriz();

// Insere aterramentos de neutro
if(! InsereAterramentos()) return(false);

// Insere admit�ncias para a refer�ncia resultantes de redu��o anterior
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   if(! bar1->InsereYrefReduc(Ynod)) return(false);
   }

// Insere cargas em Ynodal, quando for o caso (s� na fundamental)
if(geralC->CargaEmYnodal && (ordemh == 1))
   {
   for(int nb=0; nb < rede3->lisBAR->Count; nb++)
      {
      bar1 = (TBarra *) rede3->lisBAR->Items[nb];
      if(! bar1->InsereCarga(geralC, Ynod)) return(false);
      }
   }

// Insere contribui��o de trechos sem m�tuas
// NB: 'flag_zpequena' serve apenas para os trechos que representam suprimentos
// int num_trechos_nao_OK = 0;
for(int n=0; n < rede3->lisTRECHO->Count; n++)
   {
   trec1 = (TTrecho *) rede3->lisTRECHO->Items[n];
   if(trec1->grupo_mutua >= 0) continue;  // Descarta trecho com m�tua
   if(! trec1->InsereYnodal(ordemh, geralC, Ynod, flag_inclui_mutuas, flag_zpequena))
      {
      return(false);
      // num_trechos_nao_OK++;
      }
   }

// Insere contribui��o de trechos com m�tuas
for(int ng=0; ng < rede3->lisGRUPO_MUTUA->Count; ng++)
   {
   grupo = (TGrupoMutua *) rede3->lisGRUPO_MUTUA->Items[ng];
   if(! grupo->InsereYnodal(ordemh, geralC, Ynod, flag_inclui_mutuas)) return(false);
   }

// Insere capacitores s�rie
for(int ncs=0; ncs < rede3->lisCAPS->Count; ncs++)
   {
   caps1 = (TCapSerie *) rede3->lisCAPS->Items[ncs];
   if(! caps1->InsereYnodal(ordemh, geralC, Ynod)) return(false);
   }

// Insere transformadores e reguladores monof�sicos de 2 enrolamentos,
// que n�o perten�am a banco Delta Fechado
for(int nt=0; nt < rede3->lis12->Count; nt++)
   {
   trafo12 = (TTrafo12 *) rede3->lis12->Items[nt];
   if(! trafo12->InsereYnodal(ordemh, geralC, Ynod, flag_zpequena)) return(false);
   }

// Insere bancos de transformadores monof�sicos ligados em Delta Fechado
for(int nb=0; nb < rede3->lisBANCO_DF->Count; nb++)
   {
   bancoDF = (TBancoDF *) rede3->lisBANCO_DF->Items[nb];
   if(! bancoDF->InsereYnodal(ordemh, geralC, Ynod, flag_zpequena)) return(false);
   }

// Insere transformadores e reguladores trif�sicos de 2 enrolamentos
for(int nt=0; nt < rede3->lis32->Count; nt++)
   {
   trafo32 = (TTrafo32 *) rede3->lis32->Items[nt];
   if(! trafo32->InsereYnodal(ordemh, geralC, Ynod, flag_zpequena)) return(false);
   }

// Insere transformadores trif�sicos de 3 enrolamentos
for(int nt=0; nt < rede3->lisTRAFO33->Count; nt++)
   {
   trafo33 = (TTrafo33 *) rede3->lisTRAFO33->Items[nt];
   if(! trafo33->InsereYnodal(ordemh, geralC, Ynod, flag_zpequena)) return(false);
   }

// Insere trafos equivalentes
for(int nt=0; nt < rede3->lisTRAFOEQUIV->Count; nt++)
   {
   trafoequiv = (TTrafoEquiv *) rede3->lisTRAFOEQUIV->Items[nt];
   if(! trafoequiv->InsereYnodal(Ynod)) return(false);
   }

// Insere trafos ZZ
for(int nt=0; nt < rede3->lisTRAFOZZ->Count; nt++)
   {
   trafozz = (TTrafoZZ *) rede3->lisTRAFOZZ->Items[nt];
   if(! trafozz->InsereYnodal(ordemh, geralC, Ynod)) return(false);
   }

// Insere imped�ncia de defeito
if(! InsereZdef()) return(false);

// Insere elementos espec�ficos do fluxo harm�nico (FluxoH3), para ordemh > 1:
//    - admit�ncias de CLs
//    - admit�ncias de capacitores
//    - admit�ncias de suprimentos
if(ordemh > 1)
   {
   if(! InsereCLsH())           return(false);
   if(! InsereEstaticoBarraH()) return(false);
   if(! InsereSuprimentosH())   return(false);
	}

// Imprime matriz Ynodal
// AnsiString filename1 = geralC->DirTmp() + "\\Ynodal3.txt";
// Ynod->Imprime(filename1);

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttMONTA_Y, inicio);

// Retorna
return(Ynod->Erro == slSemErro);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::MontaY_Conexa(void)
   {
   // Vari�veis locais
   bool   rede_conexa;
   TBarra *bar1;

   // Ordena barras
   if(! OrdenaBarrasSup(rede3->lisBAR, geralC, rede3->lisSUP)) return(false);

   // Reseta 'flag_conexa' em todas as barras
   for(int nb=0; nb < rede3->lisBAR->Count; nb++)
      {
      bar1              = (TBarra *) rede3->lisBAR->Items[nb];
      bar1->flag_conexa = true;
      }

   // Monta matriz 'Ynod'
   if(! InicializaYnodal()) return(false);
   if(! MontaY())           return(false);

   // Verifica se a rede � conexa, acertando 'flag_conexa' de cada barra
   rede_conexa = VerificaRedeConexa(false);

   if(! rede_conexa) // Rede desconexa; remonta 'Ynod' sem as barras desconexas
      {
      if(! InicializaYnodal()) return(false);
      if(! MontaY())           return(false);
      }

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMonta3::OrdenaBarrasSup(TList    *lisBAR,
                                         VTGeralC *geralC_aux,
                                         TList    *lisSUP)
{
// Ordena��o das barras usando a fun��o 'Sort()' do TList

// Vari�veis locais
bool    sucesso;
int     ind1, ind2, jinic, jm1;
clock_t inicio;
TBarra  *bar1;
TSup    *sup1, *sup2;

// Inicializa��o
inicio                         = clock();
sucesso                        = true;
geralC_aux->num_barras_PQ      = 0;
geralC_aux->num_barras_EXT     = 0;
geralC_aux->num_barras_PQV     = 0;
geralC_aux->num_barras_PV      = 0;
geralC_aux->num_barras_SW      = 0;
geralC_aux->num_barras_INT     = 0;
geralC_aux->num_barras_carga   = 0;
geralC_aux->num_barras_geracao = 0;

// Ordena lista
lisBAR->Sort(EscolheBarraTipoCodigo);

// Define 'offset', contadores e barra de refer�ncia (a barra de refer�ncia
// � a �LTIMA barra swing encontrada (Estimador3))
geralC_aux->offset_bar_ref = -1;
for(int i=0; i < lisBAR->Count; i++)
   {
   bar1         = (TBarra *) lisBAR->Items[i];
   bar1->offset = i;
   switch(bar1->tipo)
      {
      case BAR_PQ:  geralC_aux->num_barras_PQ++;  break;
      case BAR_EXT: geralC_aux->num_barras_EXT++; break;
      case BAR_PQV: geralC_aux->num_barras_PQV++; break;
      case BAR_PV:  geralC_aux->num_barras_PV++;  break;
      case BAR_SW:
         geralC_aux->num_barras_SW++;
         geralC_aux->offset_bar_ref = i;
         break;
      case BAR_INT: geralC_aux->num_barras_INT++; break;
      default:      sucesso = false;              break;
      }
   }
if(geralC_aux->offset_bar_ref < 0) sucesso = false;  // N�o achou barra Swing

// Atualiza contadores
geralC_aux->num_barras_carga   = geralC_aux->num_barras_PQ  +
                                 geralC_aux->num_barras_EXT +
                                 geralC_aux->num_barras_PQV;
geralC_aux->num_barras_geracao = geralC_aux->num_barras_PV +
                                 geralC_aux->num_barras_SW +
                                 geralC_aux->num_barras_INT;

// Ordena suprimentos em 'lisSUP' para que estejam na mesma
// ordem que as barras EXTERNAS (j� ordenadas)
if(lisSUP != NULL)
   {
   for(int i=1; i < lisSUP->Count; i++)
      {
      sup1  = (TSup *) lisSUP->Items[i];
      ind1  = sup1->bar_ext->offset;
      jinic = i - 1;
      jm1   = i;
      for(int j=jinic; j >= 0; j--)
         {
         sup2 = (TSup *) lisSUP->Items[j];
         ind2 = sup2->bar_ext->offset;
         if(ind1 >= ind2) break;
         lisSUP->Items[jm1] = sup2;
         jm1                = j;
         }
      lisSUP->Items[jm1] = sup1;
      }
   }

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttORDENA_BARRAS, inicio);

return(sucesso);
}

//---------------------------------------------------------------------------
void __fastcall TMonta3::OrdenaLigacoesBarra(TList *lisLIG)
{
// Local variables
int        comp1, comp2, jinic, jm1;
AnsiString codb_i, codb_j, codl_i, codl_j;
TLigacaoC  *lig_i, *lig_j;

// Loop de liga��es
for(int i=1; i < lisLIG->Count; i++)
   {
   lig_i  = (TLigacaoC *) lisLIG->Items[i];
   codb_i = lig_i->cod_bar2;
   codl_i = lig_i->cod_lig;
   jinic  = i - 1;
   jm1    = i;
   for(int j=jinic; j >= 0; j--)
      {
      lig_j  = (TLigacaoC *) lisLIG->Items[j];
      codb_j = lig_j->cod_bar2;
      codl_j = lig_j->cod_lig;
      comp1  = codb_i.AnsiCompareIC(codb_j);
      comp2  = codl_i.AnsiCompareIC(codl_j);
      if((comp1 > 0) || ((comp1 == 0) && (comp2 >= 0))) break;
      lisLIG->Items[jm1] = lig_j;
      jm1                = j;
      }
   lisLIG->Items[jm1] = lig_i;
   }
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::OrdenaMutuas(TList *lisMUTUA_TEMP)
{
bool        achou, teste;
int         ind_grupo, jinic, jm1, offset1, offset2, offset3, offset4;
TBarra      *bar1;
TGrupoMutua *grupo;
TMutua      *mutua_i, *mutua_j;
TTrecho     *trec1, *trec2, *trec3, *trec4;

// Ordena o par de trechos em cada m�tua, de acordo com
// a lista de trechos da rede
for(int i=0; i < lisMUTUA_TEMP->Count; i++)
   {
   mutua_i = (TMutua *) lisMUTUA_TEMP->Items[i];
   trec1   = mutua_i->trec1;
   trec2   = mutua_i->trec2;
   offset1 = rede3->lisTRECHO->IndexOf(trec1);
   offset2 = rede3->lisTRECHO->IndexOf(trec2);
   if(offset1 > offset2)
      {
      mutua_i->trec1 = trec2;
      mutua_i->trec2 = trec1;
      bar1           = mutua_i->bar1;
      mutua_i->bar1  = mutua_i->bar2;
      mutua_i->bar2  = bar1;
      }
   }

// Ordena m�tuas de acordo com: (1) trechos da m�tua, e
//                              (2) lista de trechos da rede
for(int i=1; i < lisMUTUA_TEMP->Count; i++)
   {
   mutua_i = (TMutua *) lisMUTUA_TEMP->Items[i];
   offset1 = rede3->lisTRECHO->IndexOf(mutua_i->trec1);
   offset2 = rede3->lisTRECHO->IndexOf(mutua_i->trec2);

   // Ordena 'mutua_i' entre as m�tuas ordenadas anteriormente
   jinic = i - 1;
   jm1   = i;
   for(int j=jinic; j >= 0; j--)
      {
      mutua_j = (TMutua *) lisMUTUA_TEMP->Items[j];
      offset3 = rede3->lisTRECHO->IndexOf(mutua_j->trec1);
      offset4 = rede3->lisTRECHO->IndexOf(mutua_j->trec2);
      teste   =  (offset1 > offset3) ||
                ((offset1 == offset3) && (offset2 >= offset4));
      if(teste) break;
      lisMUTUA_TEMP->Items[jm1] = mutua_j;
      jm1                       = j;
      }
   lisMUTUA_TEMP->Items[jm1] = mutua_i;
   }

// Monta grupos de m�tuas
for(int i=0; i < lisMUTUA_TEMP->Count; i++)
   {
   mutua_i = (TMutua *) lisMUTUA_TEMP->Items[i];
   trec1   = mutua_i->trec1;
   trec2   = mutua_i->trec2;
   achou   = false;
   // Loop de grupos de m�tuas
   for(int ng=0; ng < rede3->lisGRUPO_MUTUA->Count; ng++)
      {
      grupo = (TGrupoMutua *) rede3->lisGRUPO_MUTUA->Items[ng];
      for(int nm=0; nm < grupo->lisMUTUA->Count; nm++)
         {
         mutua_j = (TMutua *) grupo->lisMUTUA->Items[nm];
         trec3   = mutua_j->trec1;
         trec4   = mutua_j->trec2;
         teste   = (trec1 == trec3) || (trec1 == trec4) ||
                   (trec2 == trec3) || (trec2 == trec4);
         if(teste)  // A nova m�tua deve ser inserida no grupo atual
            {
            ind_grupo = ng;
            achou     = true;
            break;
            }
         }
      if(achou) break;
      } // for(ng)

   // Insere nova m�tua
   if(! achou) // A nova m�tua n�o pertence a nenhum grupo existente; cria grupo
      {
      grupo     = new TGrupoMutua;
      ind_grupo = rede3->lisGRUPO_MUTUA->Add(grupo);
      }
   if(grupo->lisTRECHO->IndexOf(trec1) < 0) grupo->lisTRECHO->Add(trec1);
   if(grupo->lisTRECHO->IndexOf(trec2) < 0) grupo->lisTRECHO->Add(trec2);
   grupo->lisMUTUA->Add(mutua_i);
   lisMUTUA_TEMP->Items[i] = NULL;  // Elimina 'mutua_i' de 'lisMUTUA_TEMP'
   trec1->grupo_mutua      = ind_grupo;
   trec2->grupo_mutua      = ind_grupo;
   } // for(i)

return(true);
}

//---------------------------------------------------------------------------
void __fastcall TMonta3::RedefineAterramentoTrafos(void)
{
// Vari�veis locais
TTrafo12 *trafo12;
TTrafo32 *trafo32;
TTrafo33 *trafo33;

// Loop de TRAFO12 e REG12
for(int nt=0; nt < rede3->lis12->Count; nt++)
   {
   trafo12 = (TTrafo12 *) rede3->lis12->Items[nt];
   trafo12->CompatibilizaAterramentos();
   }

// Loop de TRAFO32 e REG32
for(int nt=0; nt < rede3->lis32->Count; nt++)
   {
   trafo32 = (TTrafo32 *) rede3->lis32->Items[nt];
   trafo32->CompatibilizaAterramentos();
   }

// Loop de TRAFO33
for(int nt=0; nt < rede3->lisTRAFO33->Count; nt++)
   {
   trafo33 = (TTrafo33 *) rede3->lisTRAFO33->Items[nt];
   trafo33->CompatibilizaAterramentos();
   }
}

//---------------------------------------------------------------------------
void __fastcall TMonta3::RenumeraNos(TList *lisTRIPLA_GLOBAL)
{
// Vari�veis locais
bool         tipo_carga;
int          ind_fase, linha;
TBarra       *bar1;
TFase        *fases;
TGrupoTripla *grupo;
TNo3         *no;
TTripla      *tr1, *tr2;
TList        *lisGRUPO;

// Inicializa��o
lisGRUPO = new TList;

// Monta grupos de triplas e os coloca em 'lisGRUPO'
// Loop externo de triplas
for(int nt=0; nt < lisTRIPLA_GLOBAL->Count; nt++)
   {
   tr1 = (TTripla *) lisTRIPLA_GLOBAL->Items[nt];
   if(tr1->grupo != NULL) continue;  // Tripla j� possui grupo
   // Cria grupo de triplas com a tripla atual e insere o grupo em 'lisGRUPO'
   grupo = new TGrupoTripla(tr1);
   lisGRUPO->Add(grupo);
   // Loop interno de triplas
   for(int i=(nt+1); i < lisTRIPLA_GLOBAL->Count; i++)
      {
      tr2 = (TTripla *) lisTRIPLA_GLOBAL->Items[i];
      if(tr2->grupo != NULL)  continue; // Tripla j� possui grupo
      if(! tr2->NoComum(tr1)) continue; // Esta tripla n�o tem n� comum com 'tr1'
      grupo->AddTripla(tr2);            // Adiciona tripla ao grupo
      } // for(i)
   } // for(nt)

// Em cada grupo de triplas de neutro, imp�e a todas as barras o aterramento
// mais significativo (ordem considerada: atIsolado -> atZ -> atSolido)
for(int ng=0; ng < lisGRUPO->Count; ng++)
   {
   grupo = (TGrupoTripla *) lisGRUPO->Items[ng];
   if(! grupo->neutro) continue;  // Descarta grupo de triplas de fase
   grupo->ImpoeAterramentoMaisSignificativo();
   }

// Inicializa contador de linhas e linhas no objeto TFase de cada barra
linha = 0;
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1  = (TBarra *) rede3->lisBAR->Items[nb];
   fases = bar1->fases;
   fases->ReiniciaLinhas();  // Coloca valor "-1"
   }

// Renumera��o definitiva de n�s
// Loop de duas passagens: n�s de carga e n�s de gera��o
for(int nv=0; nv < 2; nv++)
   {
   tipo_carga = (nv == 0);
   // Loop de grupos de triplas
   for(int ng=0; ng < lisGRUPO->Count; ng++)
      {
      grupo = (TGrupoTripla *) lisGRUPO->Items[ng];
      if(grupo->tipo_carga != tipo_carga) continue; // Grupo irrelevante
      if(grupo->aterr_atSolido)           continue; // Aterramento do grupo � atSolido (n�o s�o numerados)
      // Loop das triplas do grupo
      for(int nt=0; nt < grupo->lisTRIPLA->Count; nt++)
         {
         tr1 = (TTripla *) grupo->lisTRIPLA->Items[nt];
         // Loop dos n�s da tripla
         for(int nn=0; nn < tr1->lisNO->Count; nn++)
            {
            no       = (TNo3 *) tr1->lisNO->Items[nn];
            bar1     = no->barra;
            ind_fase = no->ind_fase;
            fases    = bar1->fases;
            // IMPORTANTE: resultado "true" no teste abaixo n�o � erro
            //             necessariamente (uma barra pode aparecer em
            //             triplas distintas no mesmo grupo)
            if(fases->LinhaIndice(ind_fase) >= 0) continue; // N� j� numerado
            fases->SetLinhaIndice(ind_fase, linha);         // Atribui linha atual ao n�
            } // for(nn)
         } // for(nt)
      // Atualiza contador de linhas
      linha++;
      } // for(ng)
   // Define linhas de Ynodal para os demais n�s (n�o pertencentes a triplas)
   DefineLinhasYnodal(nv, &linha);
   } // for(nv)

// Libera mem�ria
rede3->DeletaLista(&lisGRUPO);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::VerificaAterramento(int    tipo_ater,
                                             TBarra *bar1)
{
if(tipo_ater == atSolido)
   {
   if(bar1->tipo_ater != atSolido) return(false);
   }
else if(tipo_ater == atZ)
   {
   if((bar1->tipo_ater != atSolido) && (bar1->tipo_ater != atZ)) return(false);
   }

return(true);
}

/*
//---------------------------------------------------------------------------
bool __fastcall TMonta3::VerificaFasesSuprimentos(void)
{
int   fase_int, num_fases_b;
TFase *fases_b;
TSup  *sup1;

for(int ns=0; ns < rede3->lisSUP->Count; ns++)
   {
   sup1        = (TSup *) rede3->lisSUP->Items[ns];
   fases_b     = sup1->bar_int->fases;
   num_fases_b = fases_b->NumFases();
   for(int nfa=0; nfa < num_fases_b; nfa++)
      {
      fase_int = fases_b->Fase(nfa);
      if(! sup1->fases->ExisteFase(fase_int)) return(false); // Suprimento N�O possui a fase
      }
   }

return(true);
}
*/

/*
//---------------------------------------------------------------------------
bool __fastcall TMonta3::VerificaNeutroConexo(void)
   {

   bool       flag_conexa, neutro_conexo, *vet_flag_neutro, *vet_linhas_conexas;
   int        coluna, iap1, iap2, indN, linha, num_elem_fd, num_linhas,
              *vet_linhas;
   TList      *lisBAR_DESC, *lisBAR_CONEXAS, *lisELEMS;
   TBarra     *bar1, *bar2;
   TBarraDesc *bar_desc;
   TElemC     *elem;
   TFase      *fases;
   TTrafoZZ   *trafozz;
   TTrecho    *trec1;

   // Monta matriz Ynodal
   if(! InicializaYnodal()) return(false);
   if(! MontaY())           return(false);

   // Inicializa��o
   num_linhas = Ynod->NumTotalLinhas();
   if(num_linhas <= 0) return(false);
   neutro_conexo = true;  // Em princ�pio, n�o h� neutro(s) desconexo(s)

   // Aloca e inicializa mem�ria
   vet_flag_neutro    = new bool [num_linhas];
   vet_linhas_conexas = new bool [num_linhas];
   vet_linhas         = new int  [num_linhas];
   lisBAR_DESC        = new TList;
   lisBAR_CONEXAS     = new TList;
   for(int nl=0; nl < num_linhas; nl++)
      {
      vet_flag_neutro[nl]    = false;
      vet_linhas_conexas[nl] = false;
      }

   // Inicializa��o 1: seta 'vet_flag_neutro' nas linhas correspondentes aos neutros
   for(int nb=0; nb < rede3->lisBAR->Count; nb++)
      {
      bar1  = (TBarra *) rede3->lisBAR->Items[nb];
      fases = bar1->fases;
      linha = fases->LinhaFase(faseN);
      if(linha >= 0) vet_flag_neutro[linha] = true;
      }

   // Inicializa��o 2: neutro de gerador � conexo por defini��o
   iap2 = 0;
   for(int nb=geralC->num_barras_carga; nb < rede3->lisBAR->Count; nb++)
      {
      bar1  = (TBarra *) rede3->lisBAR->Items[nb];
      fases = bar1->fases;
      linha = fases->LinhaFase(faseN);
      if(linha < 0) continue;
      vet_linhas_conexas[linha] = true;
      vet_linhas[iap2]          = linha;
      iap2++;
      }

   // Inicializa��o 3: neutro de barra de carga atZ � conexo por defini��o
   for(int nb=0; nb < geralC->num_barras_carga; nb++)
      {
      bar1 = (TBarra *) rede3->lisBAR->Items[nb];
      if(bar1->tipo_ater != atZ) continue;  // Descarta aterramento n�o-atZ
      fases = bar1->fases;
      linha = fases->LinhaFase(faseN);
      if(linha < 0) continue;
      if(vet_linhas_conexas[linha]) continue;  // Linha j� inclu�da
      vet_linhas_conexas[linha] = true;
      vet_linhas[iap2]          = linha;
      iap2++;
      }

   // Inicializa��o 4: neutro de barra com trafo ZZ � conexo por defini��o
   for(int nt=0; nt < rede3->lisTRAFOZZ->Count; nt++)
      {
      trafozz = (TTrafoZZ *) rede3->lisTRAFOZZ->Items[nt];
      bar1    = trafozz->barra;
      fases   = bar1->fases;
      linha   = fases->LinhaFase(faseN);
      if(linha < 0) continue;
      if(vet_linhas_conexas[linha]) continue;  // Linha j� inclu�da
      vet_linhas_conexas[linha] = true;
      vet_linhas[iap2]          = linha;
      iap2++;
      }

   // Inicializa��o 5: neutro com admit�ncia de redu��o diferente de zero �
   //                  conexo por defini��o
   for(int nb=0; nb < geralC->num_barras_carga; nb++)
      {
      bar1  = (TBarra *) rede3->lisBAR->Items[nb];
      fases = bar1->fases;
      linha = fases->LinhaFase(faseN);
      if(linha < 0) continue;                                         // N�o existe o neutro
      if(vet_linhas_conexas[linha]) continue;                         // Linha j� inclu�da
      indN = fases->IndiceN();
      if(IsComplexZero(bar1->MatYnodalReduc_S[indN][indN])) continue; // N�o existe a admit�ncia
      vet_linhas_conexas[linha] = true;
      vet_linhas[iap2]          = linha;
      iap2++;
      }

   // Inicializa��o 6: Neutro de barra vizinha a barra solidamente aterrada
   // � conexo se o trecho entre ambas tiver neutro (este caso n�o � tratado
   // nas inicializa��es 2, 3 e 4)
   for(int nb=0; nb < rede3->lisBAR->Count; nb++)
      {
      bar1 = (TBarra *) rede3->lisBAR->Items[nb];
      if(bar1->tipo_ater != atSolido) continue; // Descarta aterramento n�o-atSolido
      for(int nt=0; nt < rede3->lisTRECHO->Count; nt++)
         {
         trec1 = (TTrecho *) rede3->lisTRECHO->Items[nt];
         if((trec1->bar1 != bar1) && (trec1->bar2 != bar1)) continue;
         if(trec1->bar1 == bar1) bar2 = trec1->bar2;  // 'bar2': barra vizinha
         else                    bar2 = trec1->bar1;
         linha = bar2->fases->LinhaFase(faseN);
         if((linha >= 0) && trec1->fases->ExisteN())
            {
            // Existe: - neutro n�o-atSolido em 'bar2', e
            //         - neutro em 'trec1'
            if(vet_linhas_conexas[linha]) continue;  // Linha j� inclu�da
            vet_linhas_conexas[linha] = true;
            vet_linhas[iap2]          = linha;
            iap2++;
            }
         }
      }

   // Loop de inclus�o de linhas de neutro conexas
   iap1 = 0;
   while(iap1 < iap2)
      {
      linha    = vet_linhas[iap1];
      lisELEMS = Ynod->ElementosLinha(linha);
      for(int nel=0; nel < lisELEMS->Count; nel++)
         {
         elem   = (TElemC *) lisELEMS->Items[nel];
         coluna = elem->coluna;
         if(! vet_flag_neutro[coluna])  continue; // Coluna n�o � de neutro
         if(vet_linhas_conexas[coluna]) continue; // Coluna conexa (j� inserida)
         vet_linhas_conexas[coluna] = true;
         vet_linhas[iap2]           = coluna;
         iap2++;
         }
      iap1++;
      }

   // Monta lista de barras conexas e desconexas
   for(int nb=0; nb < rede3->lisBAR->Count; nb++)
      {
      flag_conexa = true;
      bar1        = (TBarra *) rede3->lisBAR->Items[nb];
      fases       = bar1->fases;
      linha       = fases->LinhaFase(faseN);
      if(linha >= 0)
         {
         if(! vet_linhas_conexas[linha]) flag_conexa = false;
         }
      if(flag_conexa)
         {
         lisBAR_CONEXAS->Add(bar1);
         }
      else
         {
         neutro_conexo     = false;  // Desliga flag de neutro conexo
         bar1->flag_conexa = false;  // Desliga flag de barra  conexa
         bar_desc          = new TBarraDesc(bar1);
         lisBAR_DESC->Add(bar_desc);
         bar_desc->AddFase(faseN);
         }
      }

   // Imprime lista de barras conexas e desconexas
   if((lisBAR_DESC->Count > 0) && (impr3 != NULL))
              impr3->ImprimeBarrasDesconexas(true, lisBAR_DESC, lisBAR_CONEXAS);

   // Libera mem�ria
   delete[] vet_flag_neutro;
   delete[] vet_linhas_conexas;
   delete[] vet_linhas;
   rede3->DeletaLista(&lisBAR_DESC);
   delete lisBAR_CONEXAS;  // N�o deve deletar objetos apontados pela lista

   return(neutro_conexo);
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TMonta3::VerificaRedeConexa(bool flag_ordena_barras)
{
// Vari�veis locais
bool       rede_conexa, *vet_linha_conexa;
int        coluna, iap1, indN, linha, num_fases, num_linha_OK, num_linhasY;
int        *vet_linha_OK;
clock_t    inicio;
TList      *lisBAR_CONEXAS, *lisBAR_DESC, *lisELEMS;
TBarra     *bar1, *bar2;
TBarraDesc *bar_desc;
TElemC     *elem;
TFase      *fases, *fasest;
TLigacaoC  *ligacao;
TSup       *sup1;
TTrafoZZ   *trafozz;
TTrecho    *trec1;

// In�cio
inicio = clock();

// Ordena barras, se indicado pelo argumento 'flag_ordena_barras'
if(flag_ordena_barras) OrdenaBarrasSup(rede3->lisBAR, geralC, rede3->lisSUP);

// Monta matriz Ynodal sem imped�ncias m�tuas, para analisar apenas os
// caminhos fisicamente existentes entre os n�s
if(! InicializaYnodal()) return(false);
if(! MontaY(1, false))   return(false); // 2o. argumento: sem imped�ncias m�tuas
num_linhasY = Ynod->NumTotalLinhas();
// Imprime matriz Ynodal
// AnsiString filename1 = geralC->DirTmp() + "\\Ynodal_VerifRedeConexa.txt";
// Ynod->Imprime(filename1);

// Inicializa��o
rede_conexa      = false;                  // Rede desconexa em princ�pio
num_linha_OK     = 0;
vet_linha_conexa = new bool [num_linhasY]; // Indica se cada linha � conexa
vet_linha_OK     = new int  [num_linhasY]; // Conjunto de linhas OK (conexas)
lisBAR_CONEXAS   = NULL;                   // Lista de barras conexas
lisBAR_DESC      = NULL;                   // Lista de barras desconexas
for(int nl=0; nl < num_linhasY; nl++)
   {
   vet_linha_conexa[nl] = false;
   }

// Inicializa��o 1: fases e neutro de suprimentos s�o conexos por defini��o
for(int ns=0; ns < rede3->lisSUP->Count; ns++)
   {
   sup1      = (TSup *) rede3->lisSUP->Items[ns];
   bar1      = sup1->bar_int;
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      linha = fases->LinhaIndice(nfa);
      if(linha < 0) continue;  // Neutro solidamente aterrado
      vet_linha_conexa[linha]    = true;
      vet_linha_OK[num_linha_OK] = linha;
      num_linha_OK++;
      }
   }

// Inicializa��o 2: neutro de barra com trafo ZZ � conexo por defini��o
for(int nt=0; nt < rede3->lisTRAFOZZ->Count; nt++)
   {
   trafozz = (TTrafoZZ *) rede3->lisTRAFOZZ->Items[nt];
   bar1    = trafozz->barra;
   fases   = bar1->fases;
   linha   = fases->LinhaFase(faseN);
   if(linha < 0) continue;
   if(vet_linha_conexa[linha]) continue;  // Linha j� inclu�da
   vet_linha_conexa[linha]    = true;
   vet_linha_OK[num_linha_OK] = linha;
   num_linha_OK++;
   }

// Inicializa��o 3: neutro de barra com aterramento atZ � conexo por defini��o
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   if(bar1->tipo_ater != atZ) continue;   // Descarta aterramento n�o-atZ
   fases = bar1->fases;
   linha = fases->LinhaFase(faseN);
   if(linha < 0) continue;
   if(vet_linha_conexa[linha]) continue;  // Linha j� inclu�da
   vet_linha_conexa[linha]    = true;
   vet_linha_OK[num_linha_OK] = linha;
   num_linha_OK++;
   }

// Inicializa��o 4: neutro com admit�ncia de redu��o diferente de zero �
//                  conexo por defini��o
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1  = (TBarra *) rede3->lisBAR->Items[nb];
   fases = bar1->fases;
   linha = fases->LinhaFase(faseN);
   if(linha < 0) continue;                // N�o existe o neutro
   if(vet_linha_conexa[linha]) continue;  // Linha j� inclu�da
   indN = fases->IndiceN();
   if(IsComplexZero(bar1->mat_ynodal_reduc_pu->C2[indN][indN])) continue; // N�o existe a admit�ncia
   vet_linha_conexa[linha]    = true;
   vet_linha_OK[num_linha_OK] = linha;
   num_linha_OK++;
   }

// Inicializa��o 5: neutro de barra vizinha a barra solidamente aterrada
// � conexo se o trecho entre ambas tiver neutro
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar1 = (TBarra *) rede3->lisBAR->Items[nb];
   if(bar1->tipo_ater != atSolido) continue; // Descarta aterramento n�o-atSolido
   for(int nl=0; nl < bar1->lisLIG->Count; nl++)
      {
      ligacao = (TLigacaoC *) bar1->lisLIG->Items[nl];
      if(ligacao->tipo != Trecho3) continue;  // Descarta n�o-Trecho
      trec1  = (TTrecho *) ligacao->plig;
      fasest = trec1->fases;
      if(fasest->ExisteN())
         {
         if(trec1->bar1 == bar1) bar2 = trec1->bar2;  // 'bar2' � a outra barra
         else                    bar2 = trec1->bar1;
         fases = bar2->fases;
         linha = fases->LinhaFase(faseN);
         if(linha >= 0)  // Neutro da outra barra existe e � n�o-atSolido
            {
            if(! vet_linha_conexa[linha])  // Linha n�o inclu�da ainda
               {
               vet_linha_conexa[linha]    = true;
               vet_linha_OK[num_linha_OK] = linha;
               num_linha_OK++;
               }
            }
         }
      }
   }

// Loop de inclus�o das linhas que se conectam �s linhas conexas inclu�das
// nas situa��es anteriores
iap1 = 0;
while(iap1 < num_linha_OK)
   {
   linha    = vet_linha_OK[iap1];
   lisELEMS = Ynod->ElementosLinha(linha);
   for(int nel=0; nel < lisELEMS->Count; nel++)
      {
      elem   = (TElemC *) lisELEMS->Items[nel];
      coluna = elem->coluna;
      if(vet_linha_conexa[coluna]) continue;  // Coluna j� inserida
      vet_linha_conexa[coluna]   = true;
      vet_linha_OK[num_linha_OK] = coluna;
      num_linha_OK++;
      }
   iap1++;
   }

// Analisa resultado
if(num_linha_OK == num_linhasY)  // Rede conexa
   {
   rede_conexa = true;
   }
else  // Rede desconexa
   {
   // Desliga 'flag_conexa' nas barras desconexas e monta listas de
   // barras conexas e desconexas
   lisBAR_CONEXAS = new TList;
   lisBAR_DESC    = new TList;
   for(int nb=0; nb < rede3->lisBAR->Count; nb++)
      {
      bar1      = (TBarra *) rede3->lisBAR->Items[nb];
      fases     = bar1->fases;
      num_fases = fases->NumFases();
      bar_desc  = NULL;
      for(int nfa=0; nfa < num_fases; nfa++)
         {
         linha = fases->LinhaIndice(nfa);
         if(linha < 0)               continue;
         if(vet_linha_conexa[linha]) continue; // Fase conexa
         if(bar_desc == NULL)                  // Primeira fase desconexa
            {
            bar_desc = new TBarraDesc(bar1);
            lisBAR_DESC->Add(bar_desc);
            bar1->flag_conexa = false;         // Desliga 'flag_conexa'
            }
         bar_desc->AddFase(fases->Fase(nfa));
         }
      if(bar1->flag_conexa) lisBAR_CONEXAS->Add(bar1);
      } // for(nb)
   } // if(num_linha_OK)

// Imprime relat�rio
if(geralC->flag_imprime_bar_desc && (impr3 != NULL))
                    impr3->ImprimeBarrasDesconexas(lisBAR_DESC, lisBAR_CONEXAS);

// Libera mem�ria
delete[] vet_linha_conexa;
delete[] vet_linha_OK;
delete   lisBAR_CONEXAS;  // N�o deve deletar objetos apontados pela lista
rede3->DeletaLista(&lisBAR_DESC);
Ynod->ZeraMatriz();       // Seguran�a

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttVERIF_CONECT, inicio);

// Retorna
return(rede_conexa);
}

//---------------------------------------------------------------------------
bool __fastcall TMonta3::VerificaTensaoNominalBarras(void)
{
// Vari�veis locais
bool           sucesso;
char           char1;
int            fase_int, num_fases;
TList          *lisBAR_VNZ;
TBarra         *bar1;
TBarraVnomZero *bar_vnz;
TFase          *fases;

// Inicializa��o
lisBAR_VNZ = new TList;  // Lista de barras com tens�o nominal igual a zero

// Loop de barras
for(int nb=0; nb < rede3->lisBAR->Count; nb++)
   {
   bar_vnz = NULL;
   bar1    = (TBarra *) rede3->lisBAR->Items[nb];
   if(IsDoubleZero(bar1->VnomLinha_kv()))
      {
      bar_vnz = new TBarraVnomZero(bar1, 'L');
      lisBAR_VNZ->Add(bar_vnz);
      }
   fases     = bar1->fases;
   num_fases = fases->NumFases();
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      fase_int = fases->Fase(nfa);
      if(IsDoubleZero(bar1->VnomFase_kv(fase_int)))
         {
         char1 = fases->FaseAsChar(fase_int);
         if(bar_vnz == NULL)
            {
            bar_vnz = new TBarraVnomZero(bar1, char1);
            lisBAR_VNZ->Add(bar_vnz);
            }
         else
            {
            bar_vnz->AddCodigo(char1);
            }
         }
      }
   }

// Imprime relat�rio
if(geralC->flag_imprime_bar_desc && (impr3 != NULL))
                                       impr3->ImprimeBarrasVnomZero(lisBAR_VNZ);

// Determina flag de retorno
if(lisBAR_VNZ->Count == 0) sucesso = true;
else                       sucesso = false;

// Libera mem�ria
rede3->DeletaLista(&lisBAR_VNZ);

// Retorna
return(sucesso);
}

//---------------------------------------------------------------------------

// FUN��ES ABAIXO TEMPOR�RIAS, APENAS PARA IMPLEMENTA��O E DEBUG DO ESTIMADOR3

/*
//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddMedidor_BAD_MEASUREMENT(void)
{
// ESTA FUN��O INSERE OS MEDIDORES DA REDE DE 6 BARRAS DO LIVRO
// WOOD & WOLLENBERG (P�GINA 477), COM MEDI��O ERRADA NOS MEDIDORES
// P12 E Q12 (P�GINA 489)

// Vari�veis locais
int        num_fases, vet_fase_int[3];
AnsiString vet_codigo_fase[3];
TBarra     *bar1, *bar2, *bar3, *bar4, *bar5, *bar6, *bar_ref;
TMedidor   *med1;
TTrecho    *trec12, *trec14, *trec15, *trec23, *trec24, *trec25, *trec26,
           *trec35, *trec36, *trec45, *trec56;

// Inicializa��o
num_fases          = 3;
vet_codigo_fase[0] = AnsiString("_A");
vet_codigo_fase[1] = AnsiString("_B");
vet_codigo_fase[2] = AnsiString("_C");
vet_fase_int[0]    = faseA;
vet_fase_int[1]    = faseB;
vet_fase_int[2]    = faseC;

// Localiza as barras da rede
bar1 = rede3->LocalizaBarraCodigo("B1");
bar2 = rede3->LocalizaBarraCodigo("B2");
bar3 = rede3->LocalizaBarraCodigo("B3");
bar4 = rede3->LocalizaBarraCodigo("B4");
bar5 = rede3->LocalizaBarraCodigo("B5");
bar6 = rede3->LocalizaBarraCodigo("B6");

// Localiza os trechos da rede
trec12 = rede3->LocalizaTrechoId(14);
trec14 = rede3->LocalizaTrechoId(15);
trec15 = rede3->LocalizaTrechoId(16);
trec23 = rede3->LocalizaTrechoId(17);
trec24 = rede3->LocalizaTrechoId(18);
trec25 = rede3->LocalizaTrechoId(19);
trec26 = rede3->LocalizaTrechoId(20);
trec35 = rede3->LocalizaTrechoId(21);
trec36 = rede3->LocalizaTrechoId(22);
trec45 = rede3->LocalizaTrechoId(23);
trec56 = rede3->LocalizaTrechoId(24);

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV1") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 238.4  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG1_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = 113.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG1_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  20.2 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M12_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec12->id;
   med1->eqpto        = trec12;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   // med1->val_med      =  31.5 * 1.e6 / double(num_fases);
   med1->val_med      =  -31.5 * 1.e6 / double(num_fases);  // BAD MEASUREMENT
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M12_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec12->id;
   med1->eqpto        = trec12;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   // med1->val_med      = -13.2 * 1.e6 / double(num_fases);
   med1->val_med      =  13.2 * 1.e6 / double(num_fases);  // BAD MEASUREMENT
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M14_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec14->id;
   med1->eqpto        = trec14;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  38.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M14_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec14->id;
   med1->eqpto        = trec14;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  21.2 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M15_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec15->id;
   med1->eqpto        = trec15;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  35.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M15_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec15->id;
   med1->eqpto        = trec15;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =   9.4 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV2") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 237.8  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG2_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  48.4 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG2_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M21_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec12->id;
   med1->eqpto        = trec12;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -34.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M21_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec12->id;
   med1->eqpto        = trec12;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =   9.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M23_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec23->id;
   med1->eqpto        = trec23;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =   8.6 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M23_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec23->id;
   med1->eqpto        = trec23;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -11.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M24_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec24->id;
   med1->eqpto        = trec24;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  32.8 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M24_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec24->id;
   med1->eqpto        = trec24;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  38.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M25_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec25->id;
   med1->eqpto        = trec25;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  17.4 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M25_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec25->id;
   med1->eqpto        = trec25;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  22.  * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M26_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec26->id;
   med1->eqpto        = trec26;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  22.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M26_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec26->id;
   med1->eqpto        = trec26;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  15.  * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV3") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 250.7  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG3_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  55.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG3_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  90.6 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M32_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec23->id;
   med1->eqpto        = trec23;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  -2.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M32_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec23->id;
   med1->eqpto        = trec23;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  10.2 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M35_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec35->id;
   med1->eqpto        = trec35;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  17.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M35_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec35->id;
   med1->eqpto        = trec35;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  23.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M36_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec36->id;
   med1->eqpto        = trec36;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  43.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M36_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec36->id;
   med1->eqpto        = trec36;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  58.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV4") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 225.7  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML4_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.8 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML4_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M41_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec14->id;
   med1->eqpto        = trec14;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -40.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M41_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec14->id;
   med1->eqpto        = trec14;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -14.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M42_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec24->id;
   med1->eqpto        = trec24;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -29.8 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M42_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec24->id;
   med1->eqpto        = trec24;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -44.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M45_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec45->id;
   med1->eqpto        = trec45;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =   0.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M45_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec45->id;
   med1->eqpto        = trec45;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -17.4 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV5") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 225.2  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML5_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 * 1.e6 / double(num_fases);
   med1->val_med      =  72.0 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML5_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  67.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M51_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec15->id;
   med1->eqpto        = trec15;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -36.6 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M51_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec15->id;
   med1->eqpto        = trec15;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -17.5 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M52_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec25->id;
   med1->eqpto        = trec25;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -11.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M52_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec25->id;
   med1->eqpto        = trec25;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -22.2 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M53_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec35->id;
   med1->eqpto        = trec35;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -25.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M53_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec35->id;
   med1->eqpto        = trec35;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -29.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M54_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec45->id;
   med1->eqpto        = trec45;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  -2.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M54_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec45->id;
   med1->eqpto        = trec45;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  -1.5 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M56_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec56->id;
   med1->eqpto        = trec56;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  -2.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M56_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec56->id;
   med1->eqpto        = trec56;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  -0.8 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV6") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 228.9  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML6_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  72.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML6_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  60.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M62_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec26->id;
   med1->eqpto        = trec26;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -19.6 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M62_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec26->id;
   med1->eqpto        = trec26;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -22.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M63_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec36->id;
   med1->eqpto        = trec36;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -46.8 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M63_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec36->id;
   med1->eqpto        = trec36;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -51.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M65_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec56->id;
   med1->eqpto        = trec56;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =   1.  * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M65_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec56->id;
   med1->eqpto        = trec56;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =   2.9 * 1.e6 / double(num_fases);
   }

// Define 'geralC->num_med_reais'
geralC->num_med_reais = rede3->lisMED->Count;

return(true);
}
*/

/*
// DEBUG VTCDs
//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddMedidor_Demo_VTCDs(void)
{
// ESTA FUN��O INSERE MEDIDORES NA REDE DEMO: APLICA��O DO ESTIMADOR
// NO ESTUDO DE VTCDs

// Vari�veis locais
int        num_fases, vet_fase_int[3];
double     sigma_pu;
AnsiString vet_codigo_fase[3];
TBarra     *bar1, *barB1, *barB2, *barB15, *barB56;
TMedidor   *med1;

// Inicializa��o
num_fases          = 3;
vet_codigo_fase[0] = AnsiString("_A");
vet_codigo_fase[1] = AnsiString("_B");
vet_codigo_fase[2] = AnsiString("_C");
vet_fase_int[0]    = faseA;
vet_fase_int[1]    = faseB;
vet_fase_int[2]    = faseC;

// Localiza as barras not�veis da rede
barB1  = rede3->LocalizaBarraCodigo("B1");   // Suprimento
barB2  = rede3->LocalizaBarraCodigo("B2");   // Suprimento
barB15 = rede3->LocalizaBarraCodigo("B15");  // Suprimento
barB56 = rede3->LocalizaBarraCodigo("B56");  // Barra com tens�o baixa

// Insere medidores de tens�o nas 4 barras evidenciadas
sigma_pu = 0.01;  // [pu]
for(int nb=0; nb < 4; nb++)
   {
   if     (nb == 0) bar1 = barB1;
   else if(nb == 1) bar1 = barB2;
   else if(nb == 2) bar1 = barB15;
   else             bar1 = barB56;
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      med1 = new TMedidor;
      rede3->lisMED->Add(med1);
      med1->codigo       = AnsiString("MV") + vet_codigo_fase[nfa];
      med1->tipo_eqpto   = teBARRA;
      med1->eqpto_id     = bar1->id;
      med1->eqpto        = bar1;
      med1->id_calc      = bar1->id;
      med1->bar_ref      = bar1;
      med1->fase_int     = vet_fase_int[nfa];
      med1->tipo_medicao = medMOD_V;
      if(bar1 == barB56) med1->val_med_pu = 1.;
      else               med1->val_med_pu = 1.;
      med1->toler_pu     = sigma_pu * geralC->semi_intervalo_normal / med1->val_med_pu;
      }
   }

// Insere medidores de pot�ncia ativa e reativa (sigma grande e valores
// quaisquer de pot�ncia)
sigma_pu = 1.;  // [pu]
for(int nb=0; nb < 4; nb++)
   {
   if     (nb == 0) bar1 = barB1;
   else if(nb == 1) bar1 = barB2;
   else if(nb == 2) bar1 = barB15;
   else             bar1 = barB56;

   // Medidores de pot�ncia ativa
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      med1 = new TMedidor;
      rede3->lisMED->Add(med1);
      med1->codigo       = AnsiString("MP") + vet_codigo_fase[nfa];
      med1->tipo_eqpto   = teBARRA;
      med1->eqpto_id     = bar1->id;
      med1->eqpto        = bar1;
      med1->id_calc      = bar1->id;
      med1->bar_ref      = bar1;
      med1->fase_int     = vet_fase_int[nfa];
      med1->tipo_medicao = medP;
      med1->val_med_pu   = 1.;
      med1->toler_pu     = sigma_pu * geralC->semi_intervalo_normal / med1->val_med_pu;
      }

   // Medidores de pot�ncia reativa
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      med1 = new TMedidor;
      rede3->lisMED->Add(med1);
      med1->codigo       = AnsiString("MQ") + vet_codigo_fase[nfa];
      med1->tipo_eqpto   = teBARRA;
      med1->eqpto_id     = bar1->id;
      med1->eqpto        = bar1;
      med1->id_calc      = bar1->id;
      med1->bar_ref      = bar1;
      med1->fase_int     = vet_fase_int[nfa];
      med1->tipo_medicao = medQ;
      med1->val_med_pu   = 1.;
      med1->toler_pu     = sigma_pu * geralC->semi_intervalo_normal / med1->val_med_pu;
      }
   }

// Define 'geralC->num_med_reais'
geralC->num_med_reais = rede3->lisMED->Count;

return(true);
}
*/

/*
//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddMedidor_LIGmedMOD_I(void)
{
// ESTA FUN��O INSERE OS MEDIDORES DA REDE DE 6 BARRAS DO LIVRO
// WOOD & WOLLENBERG, P�GINA 477
// MEDIDORES DE P/Q EM LIGA��ES SUBSTITU�DOS POR MEDIDORES DE I (S� 1 SENTIDO)

// Vari�veis locais
int        num_fases, vet_fase_int[3];
AnsiString vet_codigo_fase[3];
TBarra     *bar1, *bar2, *bar3, *bar4, *bar5, *bar6, *bar_ref;
TMedidor   *med1;
TTrecho    *trec12, *trec14, *trec15, *trec23, *trec24, *trec25, *trec26,
           *trec35, *trec36, *trec45, *trec56;

// Inicializa��o
num_fases          = 3;
vet_codigo_fase[0] = AnsiString("_A");
vet_codigo_fase[1] = AnsiString("_B");
vet_codigo_fase[2] = AnsiString("_C");
vet_fase_int[0]    = faseA;
vet_fase_int[1]    = faseB;
vet_fase_int[2]    = faseC;

// Localiza as barras da rede
bar1 = rede3->LocalizaBarraCodigo("B1");
bar2 = rede3->LocalizaBarraCodigo("B2");
bar3 = rede3->LocalizaBarraCodigo("B3");
bar4 = rede3->LocalizaBarraCodigo("B4");
bar5 = rede3->LocalizaBarraCodigo("B5");
bar6 = rede3->LocalizaBarraCodigo("B6");

// Localiza os trechos da rede
trec12 = rede3->LocalizaTrechoId(14);
trec14 = rede3->LocalizaTrechoId(15);
trec15 = rede3->LocalizaTrechoId(16);
trec23 = rede3->LocalizaTrechoId(17);
trec24 = rede3->LocalizaTrechoId(18);
trec25 = rede3->LocalizaTrechoId(19);
trec26 = rede3->LocalizaTrechoId(20);
trec35 = rede3->LocalizaTrechoId(21);
trec36 = rede3->LocalizaTrechoId(22);
trec45 = rede3->LocalizaTrechoId(23);
trec56 = rede3->LocalizaTrechoId(24);

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV1") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   0.1  * 3.83 * 1000. / raiz3;  // V
   med1->val_med      = 238.4  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG1_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = 113.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG1_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  20.2 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M12_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec12->id;
   med1->eqpto        = trec12;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      =  85.72;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M14_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec14->id;
   med1->eqpto        = trec14;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      = 111.20;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M15_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec15->id;
   med1->eqpto        = trec15;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      =  92.68;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV2") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   0.1  * 3.83 * 1000. / raiz3;  // V
   med1->val_med      = 237.8  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG2_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  48.4 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG2_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M23_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec23->id;
   med1->eqpto        = trec23;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      =  36.85;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M24_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec24->id;
   med1->eqpto        = trec24;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      = 126.59;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M25_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec25->id;
   med1->eqpto        = trec25;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      =  70.41;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M26_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec26->id;
   med1->eqpto        = trec26;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      =  67.47;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV3") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   0.1  * 3.83 * 1000. / raiz3;  // V
   med1->val_med      = 250.7  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG3_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  55.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG3_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  90.6 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M35_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec35->id;
   med1->eqpto        = trec35;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      =  74.65;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M36_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec36->id;
   med1->eqpto        = trec36;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      = 182.29;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV4") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   0.1  * 3.83 * 1000. / raiz3;  // V
   med1->val_med      = 225.7  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML4_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.8 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML4_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M45_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec45->id;
   med1->eqpto        = trec45;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      =  43.7;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV5") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   0.1  * 3.83 * 1000. / raiz3;  // V
   med1->val_med      = 225.2  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML5_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 * 1.e6 / double(num_fases);
   med1->val_med      =  72.0 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML5_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  67.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M56_I") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec56->id;
   med1->eqpto        = trec56;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_I;
   med1->sigma        =  12.55;
   med1->val_med      =   5.65;
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV6") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   0.1  *  3.83 * 1000. / raiz3;  // V
   med1->val_med      = 228.9  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML6_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  72.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML6_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  60.9 * 1.e6 / double(num_fases);
   }

// Define 'geralC->num_med_reais'
geralC->num_med_reais = rede3->lisMED->Count;

return(true);
}
*/

/*
//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddMedidor_LIGmedS_ComSinal(void)
{
// ESTA FUN��O INSERE OS MEDIDORES DA REDE DE 6 BARRAS DO LIVRO
// WOOD & WOLLENBERG, P�GINA 477
// MEDIDORES DE P/Q EM LIGA��ES SUBSTITU�DOS POR MEDIDORES DE S (COM SINAL)

// Vari�veis locais
int        num_fases, vet_fase_int[3];
AnsiString vet_codigo_fase[3];
TBarra     *bar1, *bar2, *bar3, *bar4, *bar5, *bar6, *bar_ref;
TMedidor   *med1;
TTrecho    *trec12, *trec14, *trec15, *trec23, *trec24, *trec25, *trec26,
           *trec35, *trec36, *trec45, *trec56;

// Inicializa��o
num_fases          = 3;
vet_codigo_fase[0] = AnsiString("_A");
vet_codigo_fase[1] = AnsiString("_B");
vet_codigo_fase[2] = AnsiString("_C");
vet_fase_int[0]    = faseA;
vet_fase_int[1]    = faseB;
vet_fase_int[2]    = faseC;

// Localiza as barras da rede
bar1 = rede3->LocalizaBarraCodigo("B1");
bar2 = rede3->LocalizaBarraCodigo("B2");
bar3 = rede3->LocalizaBarraCodigo("B3");
bar4 = rede3->LocalizaBarraCodigo("B4");
bar5 = rede3->LocalizaBarraCodigo("B5");
bar6 = rede3->LocalizaBarraCodigo("B6");

// Localiza os trechos da rede
trec12 = rede3->LocalizaTrechoId(14);
trec14 = rede3->LocalizaTrechoId(15);
trec15 = rede3->LocalizaTrechoId(16);
trec23 = rede3->LocalizaTrechoId(17);
trec24 = rede3->LocalizaTrechoId(18);
trec25 = rede3->LocalizaTrechoId(19);
trec26 = rede3->LocalizaTrechoId(20);
trec35 = rede3->LocalizaTrechoId(21);
trec36 = rede3->LocalizaTrechoId(22);
trec45 = rede3->LocalizaTrechoId(23);
trec56 = rede3->LocalizaTrechoId(24);

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV1") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 238.4  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG1_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = 113.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG1_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  20.2 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M12_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec12->id;
   med1->eqpto        = trec12;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  34.15 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M14_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec14->id;
   med1->eqpto        = trec14;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  44.30 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M15_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec15->id;
   med1->eqpto        = trec15;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  36.92 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV2") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 237.8  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG2_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  48.4 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG2_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M23_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec23->id;
   med1->eqpto        = trec23;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      = -14.68 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M24_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec24->id;
   med1->eqpto        = trec24;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  50.43 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M25_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec25->id;
   med1->eqpto        = trec25;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  28.05 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M26_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec26->id;
   med1->eqpto        = trec26;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  26.88 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV3") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 250.7  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG3_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  55.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG3_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  90.6 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M35_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec35->id;
   med1->eqpto        = trec35;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  29.74 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M36_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec36->id;
   med1->eqpto        = trec36;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  72.62 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV4") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 225.7  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML4_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.8 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML4_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M45_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec45->id;
   med1->eqpto        = trec45;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      = -17.41 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV5") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 225.2  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML5_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 * 1.e6 / double(num_fases);
   med1->val_med      =  72.0 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML5_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  67.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M56_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec56->id;
   med1->eqpto        = trec56;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  -2.25 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV6") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 228.9  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML6_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  72.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML6_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  60.9 * 1.e6 / double(num_fases);
   }

// Define 'geralC->num_med_reais'
geralC->num_med_reais = rede3->lisMED->Count;

return(true);
}
*/

/*
//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddMedidor_LIGmedS_SemSinal(void)
{
// ESTA FUN��O INSERE OS MEDIDORES DA REDE DE 6 BARRAS DO LIVRO
// WOOD & WOLLENBERG, P�GINA 477
// MEDIDORES DE P/Q EM LIGA��ES SUBSTITU�DOS POR MEDIDORES DE S (SEM SINAL)

// Vari�veis locais
int        num_fases, vet_fase_int[3];
AnsiString vet_codigo_fase[3];
TBarra     *bar1, *bar2, *bar3, *bar4, *bar5, *bar6, *bar_ref;
TMedidor   *med1;
TTrecho    *trec12, *trec14, *trec15, *trec23, *trec24, *trec25, *trec26,
           *trec35, *trec36, *trec45, *trec56;

// Inicializa��o
num_fases          = 3;
vet_codigo_fase[0] = AnsiString("_A");
vet_codigo_fase[1] = AnsiString("_B");
vet_codigo_fase[2] = AnsiString("_C");
vet_fase_int[0]    = faseA;
vet_fase_int[1]    = faseB;
vet_fase_int[2]    = faseC;

// Localiza as barras da rede
bar1 = rede3->LocalizaBarraCodigo("B1");
bar2 = rede3->LocalizaBarraCodigo("B2");
bar3 = rede3->LocalizaBarraCodigo("B3");
bar4 = rede3->LocalizaBarraCodigo("B4");
bar5 = rede3->LocalizaBarraCodigo("B5");
bar6 = rede3->LocalizaBarraCodigo("B6");

// Localiza os trechos da rede
trec12 = rede3->LocalizaTrechoId(14);
trec14 = rede3->LocalizaTrechoId(15);
trec15 = rede3->LocalizaTrechoId(16);
trec23 = rede3->LocalizaTrechoId(17);
trec24 = rede3->LocalizaTrechoId(18);
trec25 = rede3->LocalizaTrechoId(19);
trec26 = rede3->LocalizaTrechoId(20);
trec35 = rede3->LocalizaTrechoId(21);
trec36 = rede3->LocalizaTrechoId(22);
trec45 = rede3->LocalizaTrechoId(23);
trec56 = rede3->LocalizaTrechoId(24);

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV1") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 238.4  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG1_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = 113.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG1_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  20.2 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M12_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec12->id;
   med1->eqpto        = trec12;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  34.15 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M14_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec14->id;
   med1->eqpto        = trec14;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  44.30 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M15_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec15->id;
   med1->eqpto        = trec15;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  36.92 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV2") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 237.8  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG2_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  48.4 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG2_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M23_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec23->id;
   med1->eqpto        = trec23;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  14.68 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M24_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec24->id;
   med1->eqpto        = trec24;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  50.43 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M25_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec25->id;
   med1->eqpto        = trec25;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  28.05 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M26_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec26->id;
   med1->eqpto        = trec26;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  26.88 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV3") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 250.7  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG3_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  55.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG3_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  90.6 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M35_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec35->id;
   med1->eqpto        = trec35;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  29.74 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M36_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec36->id;
   med1->eqpto        = trec36;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  72.62 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV4") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 225.7  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML4_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.8 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML4_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M45_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec45->id;
   med1->eqpto        = trec45;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =  17.41 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV5") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 225.2  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML5_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 * 1.e6 / double(num_fases);
   med1->val_med      =  72.0 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML5_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  67.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M56_S") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec56->id;
   med1->eqpto        = trec56;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medS;
   med1->sigma        =   5.   * 1.e6 / double(num_fases);
   med1->val_med      =   2.25 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV6") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 228.9  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML6_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  72.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML6_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  60.9 * 1.e6 / double(num_fases);
   }

// Define 'geralC->num_med_reais'
geralC->num_med_reais = rede3->lisMED->Count;

return(true);
}
*/

/*
//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddMedidor_ORIGINAL(void)
{
// ESTA FUN��O INSERE OS MEDIDORES DA REDE DE 6 BARRAS DO LIVRO
// WOOD & WOLLENBERG, P�GINA 477

// Vari�veis locais
int        num_fases, vet_fase_int[3];
AnsiString vet_codigo_fase[3];
TBarra     *bar1, *bar2, *bar3, *bar4, *bar5, *bar6, *bar_ref;
TMedidor   *med1;
TTrecho    *trec12, *trec14, *trec15, *trec23, *trec24, *trec25, *trec26,
           *trec35, *trec36, *trec45, *trec56;

// Inicializa��o
num_fases          = 3;
vet_codigo_fase[0] = AnsiString("_A");
vet_codigo_fase[1] = AnsiString("_B");
vet_codigo_fase[2] = AnsiString("_C");
vet_fase_int[0]    = faseA;
vet_fase_int[1]    = faseB;
vet_fase_int[2]    = faseC;

// Localiza as barras da rede
bar1 = rede3->LocalizaBarraCodigo("B1");
bar2 = rede3->LocalizaBarraCodigo("B2");
bar3 = rede3->LocalizaBarraCodigo("B3");
bar4 = rede3->LocalizaBarraCodigo("B4");
bar5 = rede3->LocalizaBarraCodigo("B5");
bar6 = rede3->LocalizaBarraCodigo("B6");

// Localiza os trechos da rede
trec12 = rede3->LocalizaTrechoId(14);
trec14 = rede3->LocalizaTrechoId(15);
trec15 = rede3->LocalizaTrechoId(16);
trec23 = rede3->LocalizaTrechoId(17);
trec24 = rede3->LocalizaTrechoId(18);
trec25 = rede3->LocalizaTrechoId(19);
trec26 = rede3->LocalizaTrechoId(20);
trec35 = rede3->LocalizaTrechoId(21);
trec36 = rede3->LocalizaTrechoId(22);
trec45 = rede3->LocalizaTrechoId(23);
trec56 = rede3->LocalizaTrechoId(24);

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV1") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 238.4  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG1_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = 113.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG1_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  20.2 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M12_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec12->id;
   med1->eqpto        = trec12;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  31.5 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M12_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec12->id;
   med1->eqpto        = trec12;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -13.2 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M14_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec14->id;
   med1->eqpto        = trec14;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  38.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M14_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec14->id;
   med1->eqpto        = trec14;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  21.2 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M15_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec15->id;
   med1->eqpto        = trec15;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  35.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M15_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec15->id;
   med1->eqpto        = trec15;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =   9.4 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV2") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 237.8  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG2_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  48.4 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG2_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M21_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec12->id;
   med1->eqpto        = trec12;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -34.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M21_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec12->id;
   med1->eqpto        = trec12;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =   9.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M23_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec23->id;
   med1->eqpto        = trec23;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =   8.6 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M23_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec23->id;
   med1->eqpto        = trec23;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -11.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M24_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec24->id;
   med1->eqpto        = trec24;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  32.8 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M24_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec24->id;
   med1->eqpto        = trec24;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  38.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M25_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec25->id;
   med1->eqpto        = trec25;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  17.4 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M25_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec25->id;
   med1->eqpto        = trec25;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  22.  * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M26_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec26->id;
   med1->eqpto        = trec26;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  22.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M26_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec26->id;
   med1->eqpto        = trec26;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  15.  * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV3") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 250.7  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG3_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  55.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG3_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar3->id;
   med1->eqpto        = bar3;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  90.6 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M32_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec23->id;
   med1->eqpto        = trec23;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  -2.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M32_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec23->id;
   med1->eqpto        = trec23;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  10.2 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M35_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec35->id;
   med1->eqpto        = trec35;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  17.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M35_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec35->id;
   med1->eqpto        = trec35;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  23.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M36_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec36->id;
   med1->eqpto        = trec36;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  43.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M36_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec36->id;
   med1->eqpto        = trec36;
   med1->bar_ref      = bar3;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  58.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV4") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 225.7  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML4_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.8 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML4_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar4->id;
   med1->eqpto        = bar4;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M41_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec14->id;
   med1->eqpto        = trec14;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -40.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M41_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec14->id;
   med1->eqpto        = trec14;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -14.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M42_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec24->id;
   med1->eqpto        = trec24;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -29.8 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M42_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec24->id;
   med1->eqpto        = trec24;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -44.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M45_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec45->id;
   med1->eqpto        = trec45;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =   0.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M45_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec45->id;
   med1->eqpto        = trec45;
   med1->bar_ref      = bar4;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -17.4 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV5") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 225.2  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML5_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  72.0 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML5_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar5->id;
   med1->eqpto        = bar5;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  67.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M51_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec15->id;
   med1->eqpto        = trec15;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -36.6 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M51_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec15->id;
   med1->eqpto        = trec15;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -17.5 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M52_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec25->id;
   med1->eqpto        = trec25;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -11.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M52_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec25->id;
   med1->eqpto        = trec25;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -22.2 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M53_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec35->id;
   med1->eqpto        = trec35;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -25.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M53_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec35->id;
   med1->eqpto        = trec35;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -29.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M54_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec45->id;
   med1->eqpto        = trec45;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  -2.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M54_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec45->id;
   med1->eqpto        = trec45;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  -1.5 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M56_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec56->id;
   med1->eqpto        = trec56;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  -2.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M56_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec56->id;
   med1->eqpto        = trec56;
   med1->bar_ref      = bar5;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  -0.8 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV6") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 228.9  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML6_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  72.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("ML6_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar6->id;
   med1->eqpto        = bar6;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  60.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M62_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec26->id;
   med1->eqpto        = trec26;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -19.6 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M62_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec26->id;
   med1->eqpto        = trec26;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -22.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M63_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec36->id;
   med1->eqpto        = trec36;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -46.8 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M63_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec36->id;
   med1->eqpto        = trec36;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -51.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M65_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec56->id;
   med1->eqpto        = trec56;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =   1.  * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M65_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec56->id;
   med1->eqpto        = trec56;
   med1->bar_ref      = bar6;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =   2.9 * 1.e6 / double(num_fases);
   }

// Define 'geralC->num_med_reais'
geralC->num_med_reais = rede3->lisMED->Count;

return(true);
}
*/

/*
//---------------------------------------------------------------------------
bool __fastcall TMonta3::AddMedidor_SEM_BARRAS_3456(void)
{
// ESTA FUN��O INSERE OS MEDIDORES DA REDE DE 6 BARRAS DO LIVRO
// WOOD & WOLLENBERG (P�GINA 477), SEM A MEDI��O NAS BARRAS
// 3, 4, 5 E 6 (P�GINA 493)

// Vari�veis locais
int        num_fases, vet_fase_int[3];
AnsiString vet_codigo_fase[3];
TBarra     *bar1, *bar2, *bar_ref;
TMedidor   *med1;
TTrecho    *trec12, *trec14, *trec15, *trec23, *trec24, *trec25, *trec26;

// Inicializa��o
num_fases          = 3;
vet_codigo_fase[0] = AnsiString("_A");
vet_codigo_fase[1] = AnsiString("_B");
vet_codigo_fase[2] = AnsiString("_C");
vet_fase_int[0]    = faseA;
vet_fase_int[1]    = faseB;
vet_fase_int[2]    = faseC;

// Localiza as barras da rede
bar1 = rede3->LocalizaBarraCodigo("B1");
bar2 = rede3->LocalizaBarraCodigo("B2");

// Localiza os trechos da rede
trec12 = rede3->LocalizaTrechoId(14);
trec14 = rede3->LocalizaTrechoId(15);
trec15 = rede3->LocalizaTrechoId(16);
trec23 = rede3->LocalizaTrechoId(17);
trec24 = rede3->LocalizaTrechoId(18);
trec25 = rede3->LocalizaTrechoId(19);
trec26 = rede3->LocalizaTrechoId(20);

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV1") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 238.4  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG1_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = 113.1 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG1_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar1->id;
   med1->eqpto        = bar1;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  20.2 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M12_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec12->id;
   med1->eqpto        = trec12;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  31.5 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M12_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec12->id;
   med1->eqpto        = trec12;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -13.2 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M14_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec14->id;
   med1->eqpto        = trec14;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  38.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M14_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec14->id;
   med1->eqpto        = trec14;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  21.2 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M15_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec15->id;
   med1->eqpto        = trec15;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  35.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M15_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec15->id;
   med1->eqpto        = trec15;
   med1->bar_ref      = bar1;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =   9.4 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MV2") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medMOD_V;
   med1->sigma        =   3.83 * 1000. / raiz3;  // V
   med1->val_med      = 237.8  * 1000. / raiz3;  // Tens�o de fase (V)
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG2_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  48.4 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("MG2_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teBARRA;
   med1->eqpto_id     = bar2->id;
   med1->eqpto        = bar2;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  71.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M21_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec12->id;
   med1->eqpto        = trec12;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -34.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M21_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec12->id;
   med1->eqpto        = trec12;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =   9.7 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M23_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec23->id;
   med1->eqpto        = trec23;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =   8.6 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M23_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec23->id;
   med1->eqpto        = trec23;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      = -11.9 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M24_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec24->id;
   med1->eqpto        = trec24;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  32.8 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M24_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec24->id;
   med1->eqpto        = trec24;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  38.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M25_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec25->id;
   med1->eqpto        = trec25;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  17.4 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M25_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec25->id;
   med1->eqpto        = trec25;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  22.  * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M26_P") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec26->id;
   med1->eqpto        = trec26;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medP;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  22.3 * 1.e6 / double(num_fases);
   }

for(int nfa=0; nfa < num_fases; nfa++)
   {
   med1 = new TMedidor;
   rede3->lisMED->Add(med1);
   med1->codigo       = AnsiString("M26_Q") + vet_codigo_fase[nfa];
   med1->tipo_eqpto   = teTRECHO;
   med1->eqpto_id     = trec26->id;
   med1->eqpto        = trec26;
   med1->bar_ref      = bar2;
   med1->fase_int     = vet_fase_int[nfa];
   med1->tipo_medicao = medQ;
   med1->sigma        =   5.  * 1.e6 / double(num_fases);
   med1->val_med      =  15.  * 1.e6 / double(num_fases);
   }

// Define 'geralC->num_med_reais'
geralC->num_med_reais = rede3->lisMED->Count;

return(true);
}
*/

//---------------------------------------------------------------------------

