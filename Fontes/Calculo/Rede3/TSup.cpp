//---------------------------------------------------------------------------
#pragma hdrstop
#include ".\TSup.h"
#include ".\TBarra.h"
#include ".\TFase.h"
#include "..\Complexo\Complexo.h"
#include "..\Comum13\VTGeralC.h"
#include "..\..\Constante\Fases.h"
#include "..\..\Matriz\VTMatriz.h"
#include "..\..\SisLin\VTSisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TSup::TSup(TBarra *bar_ext,
                      int    tipo,
                      int    fases_int)
   {
   // Variáveis locais
   int             f1, ind1, ind2, num_fases;
   complex<double> czero(0., 0.);

   // Inicialização
   flag_barra_interna = false;
   tipo_ater          = atSolido;
   yater_pu           = czero;

   // Aloca memória
   vet_tensao_pu = new complex<double> [MAX_FASES_G3];
   vet_sesp_mva  = new complex<double> [MAX_FASES_G3];
   // Argumento 'false': não ordena as fases (suprimentos CAN)
   fases = new TFase(fases_int, false);

   // Define variáveis
   this->bar_ext          = bar_ext;
   codigo                 = bar_ext->codigo; // Código do suprimento
   bar_ext->tipo          = tipo;            // Redefine tipo da barra externa
   bar_ext->tipo_original = tipo;
   bar_ext->sup           = this;
   bar_int                = bar_ext;         // Inicialização
   fp_min                 = 0.;
   zp_pu                  = czero;
   rt_pu                  = 0.;
   xm_pu                  = 0.;
   pot_curto1_pu          = czero;
   pot_curto3_pu          = czero;

   // 1. Adiciona neutro se ele não existir
   // 2. Redefine fases da barra externa
   // 3. Define aterramento atSolido para a barra externa
   // 4. Define 'num_fases_sem_NT'
   // 5. Calcula defasagem entre a primeira e a segunda fase
   fases->AddFase(faseN, false);  // 'false': não ordena as fases (sup. CAN)
   num_fases = fases->NumFases();
   for(int nfa=0; nfa < num_fases; nfa++)
      {
      f1 = fases->Fase(nfa);
      bar_ext->fases->AddFase(f1);
      }
   bar_ext->tipo_ater = atSolido;
   num_fases_sem_NT   = fases->NumFasesSemNT();
   switch(num_fases_sem_NT)
      {
      case 2:  // Suprimento bifásico
         // Neste ponto, os ângulos de referência ainda estão nas posições
         // A=0, B=1, C=2
         ind1          = fases->IndiceABCN(0);
         ind2          = fases->IndiceABCN(1);
         defasagem_rad = DefasagemRad(bar_ext->ang_ref_rad[ind1],
                                      bar_ext->ang_ref_rad[ind2]);
         break;

      case 3:  // Suprimento trifásico
         defasagem_rad = 2. * M_PI / 3.;  // 120 graus
         break;

      default:  // Suprimento monofásico ou erro
         defasagem_rad = 0.;
         break;
      }

   // Zera potência
   ZeraPotencia();
   }

//---------------------------------------------------------------------------
__fastcall TSup::~TSup(void)
   {
   // Deleta objetos locais
   delete[] vet_tensao_pu;
   delete[] vet_sesp_mva;
   delete   fases;
   }

//---------------------------------------------------------------------------
double __fastcall TSup::DefasagemRad(double ang1_rad,
                                     double ang2_rad)
{
// Variáveis locais
double defasagem_rad;

// Calcula defasagem e acerta para que seja menor que 180 graus (em módulo)
defasagem_rad = ang1_rad - ang2_rad;
if(defasagem_rad      < (-M_PI)) defasagem_rad += 2. * M_PI; // Soma    360 graus
else if(defasagem_rad >   M_PI)  defasagem_rad -= 2. * M_PI; // Subtrai 360 graus

// Retorna
return(defasagem_rad);
}

//---------------------------------------------------------------------------
bool __fastcall TSup::InsereYnodalH(int      ordemh,
                                    VTSisLin *Ynod)
{
// Local variables
bool            sucesso;
int             fase_int, linha1, linha2, linha3, linha4;
double          imag1, real1, valor_grande, yt;
complex<double> cum(1., 0.);
complex<double> cvalor, ym, yp;
TFase           *fasesb;
VTMatriz        *Yel, *Zel;

// Insere a matriz Ynodal do suprimento na matriz Ynodal global (Ynod),
// apenas nas frequências além da fundamental (esta função é usada
// somente pelo FluxoH3)
// IMPORTANTE: no FluxoH3 o neutro dos suprimentos é atSolido (não existe o
//             nó), ao contrário de CurtoMA/Gauss3/NR3 onde as impedâncias
//             dos suprimentos são representadas por um trecho interno com
//             neutro

// Proteção
if(ordemh <= 1) return(true);

// Inicialização
sucesso      = true;
fasesb       = bar_ext->fases;
Yel          = NULL;
Zel          = NULL;
valor_grande = 1.e8;

// Tratamento específico para cada tipo de suprimento
switch(fases->NumFasesSemNT())
   {
   case 1:  // Suprimento monofásico
      cvalor = zp_pu;
      real1  = cvalor.real();
      imag1  = cvalor.imag();
      if(ordemh > 1) imag1 *= double(ordemh);
      cvalor = complex<double>(real1, imag1);
      if(IsComplexZero(cvalor)) cvalor = complex<double>(valor_grande, 0.);
      else                      cvalor = cum / cvalor;
      fase_int = fases->Fase(0);  // A, B ou C
      linha1   = fasesb->LinhaFase(fase_int);
      Ynod->InsereDiagonal(linha1, cvalor);
      break;

   case 2:  // Suprimento bifásico
      // Aloca memória para matriz Zel (2 x 2)
      Zel = NewObjMatrizC(2, 2);
      // Monta matriz Zel com valores atualizados para a freqüência atual
      cvalor = zp_pu;
      real1  = cvalor.real();
      imag1  = cvalor.imag();
      if(ordemh > 1) imag1 *= double(ordemh);
      cvalor = complex<double>(real1, imag1);
      Zel->C2[0][0] = cvalor;
      Zel->C2[1][1] = cvalor;
      imag1 = xm_pu;
      if(ordemh > 1) imag1 *= double(ordemh);
      cvalor = complex<double>(0., imag1);
      Zel->C2[0][1] = cvalor;
      Zel->C2[1][0] = cvalor;
      // Inverte Zel, obtendo Yel
      Yel = Zel->Inverte();
      if(Yel == NULL)
         {
         sucesso = false;
         break;
         }
      // Inclui Yel em Ynod
      linha1 = fasesb->LinhaIndice(0);
      linha2 = fasesb->LinhaIndice(1);
      linha3 = fasesb->LinhaIndice(2);  // faseN, sempre
      yp     = Yel->C2[0][0];           // Elem.      da diagonal de Yel
      ym     = Yel->C2[0][1];           // Elem. fora da diagonal de Yel
      // Insere elementos da diagonal
      cvalor = yp;
      Ynod->InsereDiagonal(linha1, cvalor);  // (0,0)
      Ynod->InsereDiagonal(linha2, cvalor);  // (1,1)
      if(rt_pu == 0.) yt = valor_grande;     // Proteção
      else            yt = 1. / rt_pu;
      cvalor = complex<double>(yt, 0.) + (2. * (yp + ym));
      Ynod->InsereDiagonal(linha3, cvalor);  // (2,2)
      // Insere elementos fora da diagonal
      cvalor = ym;
      Ynod->InsereSimetrico(linha1, linha2, cvalor);  // (0,1) e (1,0)
      cvalor = -(yp + ym);
      Ynod->InsereSimetrico(linha1, linha3, cvalor);  // (0,2) e (2,0)
      Ynod->InsereSimetrico(linha2, linha3, cvalor);  // (1,2) e (2,1)
      break;

   case 3:  // Suprimento trifásico
      // IMPORTANTE: no cálculo dos parâmetros 'rt_pu', 'xm_pu' e 'zp_pu'
      //             dos suprimentos foi assumido que as 3 tensões nominais de
      //             fase (A, B, C) são iguais à tensão nominal de linha dividida
      //             por raiz(3) [cf. TMonta3::AddSuprimento()]
      // Aloca memória para matriz Zel (3 x 3)
      Zel = NewObjMatrizC(3, 3);
      // Monta matriz Zel com valores atualizados para a freqüência atual
      cvalor = zp_pu;
      real1  = cvalor.real();
      imag1  = cvalor.imag();
      if(ordemh > 1) imag1 *= double(ordemh);
      cvalor = complex<double>(real1, imag1);
      Zel->C2[0][0] = cvalor;
      Zel->C2[1][1] = cvalor;
      Zel->C2[2][2] = cvalor;
      imag1 = xm_pu;
      if(ordemh > 1) imag1 *= double(ordemh);
      cvalor = complex<double>(0., imag1);
      Zel->C2[0][1] = cvalor;
      Zel->C2[0][2] = cvalor;
      Zel->C2[1][0] = cvalor;
      Zel->C2[1][2] = cvalor;
      Zel->C2[2][0] = cvalor;
      Zel->C2[2][1] = cvalor;
      // Inverte Zel, obtendo Yel
      Yel = Zel->Inverte();
      if(Yel == NULL)
         {
         sucesso = false;
         break;
         }
      // Inclui Yel em Ynod
      linha1 = fasesb->LinhaFase(faseA);
      linha2 = fasesb->LinhaFase(faseB);
      linha3 = fasesb->LinhaFase(faseC);
      linha4 = fasesb->LinhaFase(faseN);
      yp     = Yel->C2[0][0];  // Elem. da diagonal de Yel
      ym     = Yel->C2[0][1];  // Elem. fora da diagonal de Yel
      // Insere elementos da diagonal
      cvalor = yp;
      Ynod->InsereDiagonal(linha1, cvalor);  // (0,0)
      Ynod->InsereDiagonal(linha2, cvalor);  // (1,1)
      Ynod->InsereDiagonal(linha3, cvalor);  // (2,2)
      if(rt_pu == 0.) yt = valor_grande;     // Proteção
      else            yt = 1. / rt_pu;
      cvalor = complex<double>(yt, 0.) + (3. * (yp + 2. * ym));
      Ynod->InsereDiagonal(linha4, cvalor);  // (3,3)
      // Insere elementos fora da diagonal
      cvalor = ym;
      Ynod->InsereSimetrico(linha1, linha2, cvalor);  // (0,1) e (1,0)
      Ynod->InsereSimetrico(linha1, linha3, cvalor);  // (0,2) e (2,0)
      Ynod->InsereSimetrico(linha2, linha3, cvalor);  // (1,2) e (2,1)
      cvalor   = -(yp + 2. * ym);
      Ynod->InsereSimetrico(linha1, linha4, cvalor);  // (0,3) e (3,0)
      Ynod->InsereSimetrico(linha2, linha4, cvalor);  // (1,3) e (3,1)
      Ynod->InsereSimetrico(linha3, linha4, cvalor);  // (2,3) e (3,2)
      break;

   default:  // Erro
      sucesso = false;
      break;
   } // switch()

// Libera memória
// IMPORTANTÍSSIMO: destruir 'Yel' (bug chatíssimo de achar, em 2012.06.08)
delete Yel;
delete Zel;

// Verifica condição de erro
if(sucesso && (Ynod->Erro == slSemErro)) return(true);
else                                     return(false);
}

//---------------------------------------------------------------------------
void __fastcall TSup::ZeraPotencia(void)
   {
   complex<double> czero(0., 0.);

   for(int nfa=0; nfa < MAX_FASES_G3; nfa++)
      {
      vet_sesp_mva[nfa] = czero;
      }
   }

//---------------------------------------------------------------------------






