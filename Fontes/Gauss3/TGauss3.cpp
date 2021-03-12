//---------------------------------------------------------------------------
#pragma hdrstop
#include <math.hpp>
#include ".\TGauss3.h"
#include "..\Apl\VTApl.h"
#include "..\Calculo\Atualiza3\VTAtualiza3.h"
#include "..\Calculo\Calculo3\VTCalculo3.h"
#include "..\Calculo\Comum13\Constantes_Calculo.h"
#include "..\Calculo\Comum13\Estruturas.h"
#include "..\Calculo\Comum13\VTGeralC.h"
#include "..\Calculo\Comum13\VTTempoCPU.h"
#include "..\Calculo\Impressao3\TImpressao3.h"
#include "..\Calculo\Monta3\VTMonta3.h"
#include "..\Calculo\Rede3\TBarra.h"
#include "..\Calculo\Rede3\TFase.h"
#include "..\Calculo\Rede3\TReg12.h"
#include "..\Calculo\Rede3\TReg32.h"
#include "..\Calculo\Rede3\TSup.h"
#include "..\Calculo\Rede3\TTrafo12.h"
#include "..\Calculo\Rede3\TTrafo32.h"
#include "..\Calculo\Rede3\TTrecho.h"
#include "..\Calculo\Rede3\VTRede3.h"
#include "..\Calculo\Resultados3\VTResultados3.h"
#include "..\Calculo\VTData.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Fases.h"
#include "..\Curto\TBarraDefeito.h"
#include "..\Curto\TCurtoAux3.h"
#include "..\Geral\VTGeral.h"
#include "..\Matriz\VTMatriz.h"
#include "..\Patamar\VTpatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTNo.h"
#include "..\RedeCC\ResCurto.h"
#include "..\SisLin\VTSisLin.h"
#include "..\..\DLL_Inc\SisLin.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTGauss3* __fastcall NewObjGauss3(VTApl *apl)
   {
   return(new TGauss3(apl));
   }

//---------------------------------------------------------------------------
__fastcall TGauss3::TGauss3(VTApl *apl)
   {
   // Salva ponteiro para 'apl'
   this->apl = apl;

   // Inicializa ponteiros
   data   = NULL;
   net    = NULL;
   redeCC = NULL;

   // Inicializa vari�veis
   ModelaBarraPV        = false;
   flag_alocou_externos = false;
   flag_ordenar_barras  = true;
   // flag_IEEE13          = false;
   flag_verifica_NAN    = true;
   vet_corr_ger         = NULL;
   vet_tensoes          = NULL;
   vet_TC               = NULL;
   rede3                = NULL;
   atual3               = NULL;
   calculo3             = NULL;
   geralC               = NULL;
   impr3                = NULL;
   monta3               = NULL;
   result3              = NULL;
   curto_aux3           = NULL;
   Ynod2                = NULL;

   // Define valor de constantes
   grau_rad      = M_PI / 180.;
   raiz3         = sqrt(3.);
   raiz3_inverso = 1. / raiz3;
   cum           = complex<double>(1., 0.);
   czero         = complex<double>(0., 0.);
   zerov         = complex<double>(0., -ZEROV);

   // Demais inicializa��es
   RelatorioBarDesc = false;
   TipoRelatorio    = trSEM_RELATORIO;
   lisBAR_SEM_CONV  = NULL;

   // Curto-circuito simult�neo
   lisBAR_DEF = NULL;  // S� o ponteiro, n�o � alocado nesta classe
   }

//---------------------------------------------------------------------------
__fastcall TGauss3::~TGauss3(void)
   {
   if(flag_alocou_externos)
      {
      // Uso normal de Gauss3 (objetos externos controlados por Gauss3)
      // NB: s�o destru�dos todos os objetos que comp�em a
      //     estrutura 'FluxoH3_G3'. Estes objetos N�O s�o
      //     destru�dos quando Gauss3 � chamado por FluxoH3,
      //     pois nesse caso eles s�o controlados por FluxoH3.
      delete geralC;
      delete rede3;
      delete atual3;
      delete impr3;
      delete monta3;
      delete calculo3;
      }

   // Demais objetos
   delete[] vet_corr_ger;
   delete[] vet_tensoes;
   delete[] vet_TC;
   delete   result3;
   delete   curto_aux3;
   delete   Ynod2;
   delete   lisBAR_SEM_CONV;
   }

/*
//---------------------------------------------------------------------------
bool __fastcall TGauss3::AcertaImpedanciasAdmitancias_IEEE13(void)
{
// Vari�veis locais
bool       sucesso, teste01, teste02, teste03, teste04, teste05, teste06,
           teste07, teste08, teste09, teste10, teste11;
int        j1, j2, num_fases;
TFase      *fases;
TTrecho    *trec1;
VTMatriz   *Zserie, *Ypar;
AnsiString cod1, cod2;

// Vers�o 1 - transpondo os valores do relat�rio do IEEE

// Inicializa��o
sucesso = true;
Zserie  = NewObjMatrizC(3, 3);
Ypar    = NewObjMatrizD(3, 3);

// Loop de trechos
for(int nt=0; nt < rede3->lisTRECHO->Count; nt++)
   {
   Zserie->Zera();  // IMPORTANTE!!!
   Ypar->Zera();    // IMPORTANTE!!!
   trec1   = (TTrecho *) rede3->lisTRECHO->Items[nt];
   fases   = trec1->fases;
   cod1    = trec1->bar1->codigo;
   cod2    = trec1->bar2->codigo;

   teste01 = ((cod1.AnsiCompareIC("RG60") == 0) && (cod2.AnsiCompareIC("632") == 0)) ||
             ((cod2.AnsiCompareIC("RG60") == 0) && (cod1.AnsiCompareIC("632") == 0));
   teste02 = ((cod1.AnsiCompareIC("632") == 0) && (cod2.AnsiCompareIC("671") == 0)) ||
             ((cod2.AnsiCompareIC("632") == 0) && (cod1.AnsiCompareIC("671") == 0));
   teste03 = ((cod1.AnsiCompareIC("671") == 0) && (cod2.AnsiCompareIC("680") == 0)) ||
             ((cod2.AnsiCompareIC("671") == 0) && (cod1.AnsiCompareIC("680") == 0));
   teste04 = ((cod1.AnsiCompareIC("632") == 0) && (cod2.AnsiCompareIC("633") == 0)) ||
             ((cod2.AnsiCompareIC("632") == 0) && (cod1.AnsiCompareIC("633") == 0));
   teste05 = ((cod1.AnsiCompareIC("632") == 0) && (cod2.AnsiCompareIC("645") == 0)) ||
             ((cod2.AnsiCompareIC("632") == 0) && (cod1.AnsiCompareIC("645") == 0));
   teste06 = ((cod1.AnsiCompareIC("645") == 0) && (cod2.AnsiCompareIC("646") == 0)) ||
             ((cod2.AnsiCompareIC("645") == 0) && (cod1.AnsiCompareIC("646") == 0));
   teste07 = ((cod1.AnsiCompareIC("684") == 0) && (cod2.AnsiCompareIC("671") == 0)) ||
             ((cod2.AnsiCompareIC("684") == 0) && (cod1.AnsiCompareIC("671") == 0));
   teste08 = ((cod1.AnsiCompareIC("611") == 0) && (cod2.AnsiCompareIC("684") == 0)) ||
             ((cod2.AnsiCompareIC("611") == 0) && (cod1.AnsiCompareIC("684") == 0));
   teste09 = ((cod1.AnsiCompareIC("692_X") == 0) && (cod2.AnsiCompareIC("675") == 0)) ||
             ((cod2.AnsiCompareIC("692_X") == 0) && (cod1.AnsiCompareIC("675") == 0));
   teste10 = ((cod1.AnsiCompareIC("684") == 0) && (cod2.AnsiCompareIC("652") == 0)) ||
             ((cod2.AnsiCompareIC("684") == 0) && (cod1.AnsiCompareIC("652") == 0));
   teste11 = ((cod1.AnsiCompareIC("692_X") == 0) && (cod2.AnsiCompareIC("671") == 0)) ||
             ((cod2.AnsiCompareIC("692_X") == 0) && (cod1.AnsiCompareIC("671") == 0));
   if(teste01 || teste02 || teste03)  // Config_601
      {
      fases->SetNumFases(3);                               // Define n�mero de fases
      fases->SetFase(0, faseA);
      fases->SetFase(1, faseB);
      fases->SetFase(2, faseC);
      Zserie->C2[0][0] = complex<double>(0.3465, 1.0179);  // AA
      Zserie->C2[0][1] = complex<double>(0.1560, 0.5017);  // AB
      Zserie->C2[0][2] = complex<double>(0.1580, 0.4236);  // AC
      Zserie->C2[1][1] = complex<double>(0.3375, 1.0478);  // BB, ohm/milha
      Zserie->C2[1][2] = complex<double>(0.1535, 0.3849);  // BC
      Zserie->C2[2][2] = complex<double>(0.3414, 1.0348);  // CC
      Ypar->D2[0][0]   =  6.2998;                          // AA
      Ypar->D2[0][1]   = -1.9958;                          // AB
      Ypar->D2[0][2]   = -1.2595;                          // AC
      Ypar->D2[1][1]   =  5.9597;                          // BB, 1e-6 siemens/milha
      Ypar->D2[1][2]   = -0.7417;                          // BC
      Ypar->D2[2][2]   =  5.6386;                          // CC
      }

   else if(teste04)  // Config_602
      {
      fases->SetNumFases(3);                               // Define n�mero de fases
      fases->SetFase(0, faseA);
      fases->SetFase(1, faseB);
      fases->SetFase(2, faseC);
      Zserie->C2[0][0] = complex<double>(0.7526, 1.1814);  // AA
      Zserie->C2[0][1] = complex<double>(0.1580, 0.4236);  // AB
      Zserie->C2[0][2] = complex<double>(0.1560, 0.5017);  // AC
      Zserie->C2[1][1] = complex<double>(0.7475, 1.1983);  // BB
      Zserie->C2[1][2] = complex<double>(0.1535, 0.3849);  // BC
      Zserie->C2[2][2] = complex<double>(0.7436, 1.2112);  // CC
      Ypar->D2[0][0]   =  5.6990;                          // AA
      Ypar->D2[0][1]   = -1.0817;                          // AB
      Ypar->D2[0][2]   = -1.6905;                          // AC
      Ypar->D2[1][1]   =  5.1795;                          // BB
      Ypar->D2[1][2]   = -0.6588;                          // BC
      Ypar->D2[2][2]   =  5.4246;                          // CC
      }

   else if(teste05 || teste06)  // Config_603
      {
      fases->SetNumFases(2);                               // Define n�mero de fases
      fases->SetFase(0, faseB);
      fases->SetFase(1, faseC);
      Zserie->C2[0][0] = complex<double>(1.3238, 1.3569);  // BB
      Zserie->C2[0][1] = complex<double>(0.2066, 0.4591);  // BC
      Zserie->C2[1][1] = complex<double>(1.3294, 1.3471);  // CC
      Ypar->D2[0][0]   =  4.6658;                          // BB
      Ypar->D2[0][1]   = -0.8999;                          // BC
      Ypar->D2[1][1]   =  4.7097;                          // CC
      }

   else if(teste07)  // Config_604
      {
      fases->SetNumFases(2);                               // Define n�mero de fases
      fases->SetFase(0, faseA);
      fases->SetFase(1, faseC);
      Zserie->C2[0][0] = complex<double>(1.3238, 1.3569);  // AA
      Zserie->C2[0][1] = complex<double>(0.2066, 0.4591);  // AC
      Zserie->C2[1][1] = complex<double>(1.3294, 1.3471);  // CC
      Ypar->D2[0][0]   =  4.6658;                          // AA
      Ypar->D2[0][1]   = -0.8999;                          // AC
      Ypar->D2[1][1]   =  4.7097;                          // CC
      }

   else if(teste08)  // Config_605
      {
      fases->SetNumFases(1);                               // Define n�mero de fases
      fases->SetFase(0, faseC);
      Zserie->C2[0][0] = complex<double>(1.3292, 1.3475);  // CC
      Ypar->D2[0][0]   =  4.5193;                          // CC
      }

   else if(teste09 || teste11)  // Config_606
      {
      fases->SetNumFases(3);                               // Define n�mero de fases
      fases->SetFase(0, faseA);
      fases->SetFase(1, faseB);
      fases->SetFase(2, faseC);
      Zserie->C2[0][0] = complex<double>(0.7982,  0.4463);  // AA
      Zserie->C2[0][1] = complex<double>(0.3192,  0.0328);  // AB
      Zserie->C2[0][2] = complex<double>(0.2849, -0.0143);  // AC
      Zserie->C2[1][1] = complex<double>(0.7891,  0.4041);  // BB
      Zserie->C2[1][2] = complex<double>(0.3192,  0.0328);  // BC
      Zserie->C2[2][2] = complex<double>(0.7982,  0.4463);  // CC
      Ypar->D2[0][0]   = 96.8897;                           // AA
      Ypar->D2[1][1]   = 96.8897;                           // BB
      Ypar->D2[2][2]   = 96.8897;                           // CC
      }

   else if(teste10)  // Config_607
      {
      fases->SetNumFases(1);                               // Define n�mero de fases
      fases->SetFase(0, faseA);
      Zserie->C2[0][0] = complex<double>(1.3425, 0.5124);  // AA
      Ypar->D2[0][0]   = 88.9912;                          // AA
      }

   else  // Erro
      {
      sucesso = false;
      break;
      }

   // Monta matrizes de imped�ncias [ohm] e admit�ncias [siemens]
   num_fases = fases->NumFases();
   for(int i=0; i < num_fases; i++)
      {
      for(int j=i; j < num_fases; j++)
         {
         trec1->MatZserie_ohm[i][j] = Zserie->C2[i][j] * trec1->compr_km;
         if(j > i) trec1->MatZserie_ohm[j][i] = trec1->MatZserie_ohm[i][j]; // Simetria

         Ypar->D2[i][j] = Ypar->D2[i][j] * trec1->compr_km;
         if(j > i) Ypar->D2[j][i] = Ypar->D2[i][j]; // Simetria
         }
      }

   // Monta matriz de admit�ncias nodais (exigido por TMonta3),
   // considerando que a matriz no relat�rio do IEEE j� � nodal
   for(int i=0; i < num_fases; i++)
      {
      for(int j=0; j < num_fases; j++)
         {
         trec1->MatYparalelo_S[i][j] = 1.e-6 * Ypar->D2[i][j]; // micro-siemens para siemens
         }
      }

   // Monta matriz de admit�ncias nodais (exigido por TMonta3),
   // considerando que a matriz do IEEE N�O � nodal
   // for(int i=0; i < num_fases; i++)
   //    {
   //    double soma = 0.;
   //    for(int j=0; j < num_fases; j++)
   //       {
   //       soma += Ypar->D2[i][j];
   //       if(j != i) trec1->MatYparalelo_S[i][j] = -1.e-6 * Ypar->D2[i][j];
   //       }
   //    trec1->MatYparalelo_S[i][i] = 1.e-6 * soma;
   //    }

   } // for(nt)

// Libera mem�ria
delete Zserie;
delete Ypar;

return(sucesso);
}
*/

/*
//---------------------------------------------------------------------------
bool __fastcall TGauss3::AcertaImpedanciasAdmitancias_IEEE13(void)
{
// Vari�veis locais
bool       sucesso, teste01, teste02, teste03, teste04, teste05, teste06,
           teste07, teste08, teste09, teste10, teste11;
int        num_fases;
double     fmult, soma;
TFase      *fases;
TTrecho    *trec1;
VTMatriz   *Zserie, *Ypar;
AnsiString cod1, cod2;

// Vers�o 2 - sem transpor os valores do relat�rio do IEEE

// Inicializa��o
sucesso = true;
Zserie  = NewObjMatrizC(3, 3);
Ypar    = NewObjMatrizD(3, 3);

// Loop de trechos
for(int nt=0; nt < rede3->lisTRECHO->Count; nt++)
   {
   trec1   = (TTrecho *) rede3->lisTRECHO->Items[nt];
   fases   = trec1->fases;
   cod1    = trec1->bar1->codigo;
   cod2    = trec1->bar2->codigo;

   teste01 = ((cod1.AnsiCompareIC("RG60") == 0) && (cod2.AnsiCompareIC("632") == 0)) ||
             ((cod2.AnsiCompareIC("RG60") == 0) && (cod1.AnsiCompareIC("632") == 0));
   teste02 = ((cod1.AnsiCompareIC("632") == 0) && (cod2.AnsiCompareIC("671") == 0)) ||
             ((cod2.AnsiCompareIC("632") == 0) && (cod1.AnsiCompareIC("671") == 0));
   teste03 = ((cod1.AnsiCompareIC("671") == 0) && (cod2.AnsiCompareIC("680") == 0)) ||
             ((cod2.AnsiCompareIC("671") == 0) && (cod1.AnsiCompareIC("680") == 0));
   teste04 = ((cod1.AnsiCompareIC("632") == 0) && (cod2.AnsiCompareIC("633") == 0)) ||
             ((cod2.AnsiCompareIC("632") == 0) && (cod1.AnsiCompareIC("633") == 0));
   teste05 = ((cod1.AnsiCompareIC("632") == 0) && (cod2.AnsiCompareIC("645") == 0)) ||
             ((cod2.AnsiCompareIC("632") == 0) && (cod1.AnsiCompareIC("645") == 0));
   teste06 = ((cod1.AnsiCompareIC("645") == 0) && (cod2.AnsiCompareIC("646") == 0)) ||
             ((cod2.AnsiCompareIC("645") == 0) && (cod1.AnsiCompareIC("646") == 0));
   teste07 = ((cod1.AnsiCompareIC("684") == 0) && (cod2.AnsiCompareIC("671") == 0)) ||
             ((cod2.AnsiCompareIC("684") == 0) && (cod1.AnsiCompareIC("671") == 0));
   teste08 = ((cod1.AnsiCompareIC("611") == 0) && (cod2.AnsiCompareIC("684") == 0)) ||
             ((cod2.AnsiCompareIC("611") == 0) && (cod1.AnsiCompareIC("684") == 0));
   teste09 = ((cod1.AnsiCompareIC("692_X") == 0) && (cod2.AnsiCompareIC("675") == 0)) ||
             ((cod2.AnsiCompareIC("692_X") == 0) && (cod1.AnsiCompareIC("675") == 0));
   teste10 = ((cod1.AnsiCompareIC("684") == 0) && (cod2.AnsiCompareIC("652") == 0)) ||
             ((cod2.AnsiCompareIC("684") == 0) && (cod1.AnsiCompareIC("652") == 0));
   teste11 = ((cod1.AnsiCompareIC("692_X") == 0) && (cod2.AnsiCompareIC("671") == 0)) ||
             ((cod2.AnsiCompareIC("692_X") == 0) && (cod1.AnsiCompareIC("671") == 0));
   if(teste01 || teste02 || teste03 || teste11)  // Config_601
      {
      fases->SetNumFases(3);                               // Define n�mero de fases
      fases->SetFase(0, faseA);
      fases->SetFase(1, faseB);
      fases->SetFase(2, faseC);
      Zserie->C2[0][0] = complex<double>(0.3465, 1.0179);  // ohm/milha
      Zserie->C2[0][1] = complex<double>(0.1560, 0.5017);
      Zserie->C2[0][2] = complex<double>(0.1580, 0.4236);
      Zserie->C2[1][1] = complex<double>(0.3375, 1.0478);
      Zserie->C2[1][2] = complex<double>(0.1535, 0.3849);
      Zserie->C2[2][2] = complex<double>(0.3414, 1.0348);
      Ypar->D2[0][0]   =  6.2998;                         // 1e-6 siemens/milha
      Ypar->D2[0][1]   = -1.9958;
      Ypar->D2[0][2]   = -1.2595;
      Ypar->D2[1][1]   =  5.9597;
      Ypar->D2[1][2]   = -0.7417;
      Ypar->D2[2][2]   =  5.6386;
      }

   else if(teste04)  // Config_602
      {
      fases->SetNumFases(3);                               // Define n�mero de fases
      fases->SetFase(0, faseA);
      fases->SetFase(1, faseB);
      fases->SetFase(2, faseC);
      Zserie->C2[0][0] = complex<double>(0.7526, 1.1814);
      Zserie->C2[0][1] = complex<double>(0.1580, 0.4236);
      Zserie->C2[0][2] = complex<double>(0.1560, 0.5017);
      Zserie->C2[1][1] = complex<double>(0.7475, 1.1983);
      Zserie->C2[1][2] = complex<double>(0.1535, 0.3849);
      Zserie->C2[2][2] = complex<double>(0.7436, 1.2112);
      Ypar->D2[0][0]   =  5.6990;
      Ypar->D2[0][1]   = -1.0817;
      Ypar->D2[0][2]   = -1.6905;
      Ypar->D2[1][1]   =  5.1795;
      Ypar->D2[1][2]   = -0.6588;
      Ypar->D2[2][2]   =  5.4246;
      }

   else if(teste05 || teste06)  // Config_603
      {
      fases->SetNumFases(2);                               // Define n�mero de fases
      fases->SetFase(0, faseB);
      fases->SetFase(1, faseC);
      Zserie->C2[0][0] = complex<double>(1.3294, 1.3471);
      Zserie->C2[0][1] = complex<double>(0.2066, 0.4591);
      Zserie->C2[1][1] = complex<double>(1.3238, 1.3569);
      Ypar->D2[0][0]   =  4.7097;
      Ypar->D2[0][1]   = -0.8999;
      Ypar->D2[1][1]   =  4.6658;
      }

   else if(teste07)  // Config_604
      {
      fases->SetNumFases(2);                               // Define n�mero de fases
      fases->SetFase(0, faseA);
      fases->SetFase(1, faseC);
      Zserie->C2[0][0] = complex<double>(1.3238, 1.3569);
      Zserie->C2[0][1] = complex<double>(0.2066, 0.4591);
      Zserie->C2[1][1] = complex<double>(1.3294, 1.3471);
      Ypar->D2[0][0]   =  4.6658;
      Ypar->D2[0][1]   = -0.8999;
      Ypar->D2[1][1]   =  4.7097;
      }

   else if(teste08)  // Config_605
      {
      fases->SetNumFases(1);                               // Define n�mero de fases
      fases->SetFase(0, faseC);
      Zserie->C2[0][0] = complex<double>(1.3292, 1.3475);
      Ypar->D2[0][0]   =  4.5193;
      }

   else if(teste09)  // Config_606
      {
      fases->SetNumFases(3);                               // Define n�mero de fases
      fases->SetFase(0, faseA);
      fases->SetFase(1, faseB);
      fases->SetFase(2, faseC);
      Zserie->C2[0][0] = complex<double>(0.7982,  0.4463);
      Zserie->C2[0][1] = complex<double>(0.3192,  0.0328);
      Zserie->C2[0][2] = complex<double>(0.2849, -0.0143);
      Zserie->C2[1][1] = complex<double>(0.7891,  0.4041);
      Zserie->C2[1][2] = complex<double>(0.3192,  0.0328);
      Zserie->C2[2][2] = complex<double>(0.7982,  0.4463);
      Ypar->D2[0][0]   = 96.8897;
      Ypar->D2[1][1]   = 96.8897;
      Ypar->D2[2][2]   = 96.8897;
      }

   else if(teste10)  // Config_607
      {
      fases->SetNumFases(1);                               // Define n�mero de fases
      fases->SetFase(0, faseA);
      Zserie->C2[0][0] = complex<double>(1.3425, 0.5124);  // AA
      Ypar->D2[0][0]   = 88.9912;                          // AA
      }

   else  // Erro
      {
      sucesso = false;
      break;
      }

   // Monta matrizes de imped�ncias [ohm] e admit�ncias [siemens]
   num_fases = fases->NumFases();
   fmult     = trec1->compr_km / 1.609;  // Converte de ohm/milha para ohm/km
   for(int i=0; i < num_fases; i++)
      {
      for(int j=i; j < num_fases; j++)
         {
         trec1->MatZserie_ohm[i][j] = Zserie->C2[i][j] * fmult;
         if(j > i) trec1->MatZserie_ohm[j][i] = trec1->MatZserie_ohm[i][j];   // Simetria

         Ypar->D2[i][j] = Ypar->D2[i][j] * fmult;
         if(j > i) Ypar->D2[j][i] = Ypar->D2[i][j]; // Simetria
         }
      }

   // Monta matriz de admit�ncias nodais (exigido por TMonta3)
   // A matriz no relat�rio do IEEE j� � nodal!
   for(int i=0; i < num_fases; i++)
      {
      for(int j=0; j < num_fases; j++)
         {
         trec1->MatYparalelo_S[i][j] = 1.e-6 * Ypar->D2[i][j]; // micro-siemens para siemens
         }
      }
   } // for(nt)

// Libera mem�ria
delete Zserie;
delete Ypar;

return(sucesso);
}
*/

/*
//---------------------------------------------------------------------------
void __fastcall TGauss3::AdicionaBarraSemConvergencia(TBarra *bar1)
{
if(lisBAR_SEM_CONV == NULL) return;       // Prote��o
int ind = lisBAR_SEM_CONV->IndexOf(bar1);
if(ind >= 0) return;                      // A barra fornecida j� est� na lista
else         lisBAR_SEM_CONV->Add(bar1);  // Adiciona a barra fornecida � lista
}
*/

//---------------------------------------------------------------------------
void __fastcall TGauss3::AjustaTapReguladores1(void)
{
// Vari�veis locais
TReg12 *reg12;

// Loop de reguladores de tens�o
for(int nr=0; nr < rede3->lisREG12->Count; nr++)
   {
   reg12 = (TReg12 *) rede3->lisREG12->Items[nr];
   reg12->AjustaTap(vet_TC);
   }
}

//---------------------------------------------------------------------------
void __fastcall TGauss3::AjustaTapReguladores3(void)
{
// Vari�veis locais
TReg32 *reg32;

// Loop de reguladores de tens�o
for(int nr=0; nr < rede3->lisREG32->Count; nr++)
   {
   reg32 = (TReg32 *) rede3->lisREG32->Items[nr];
   reg32->AjustaTap(vet_TC);
   }
}

//---------------------------------------------------------------------------
bool __fastcall TGauss3::AlocaMemoria(void)
   {
   // Retorna se a mem�ria j� foi alocada em chamada anterior
   if(vet_corr_ger != NULL) return(true);

   // Aloca mem�ria
   vet_corr_ger = new complex<double> [geralC->num_tot_linhas];
   vet_tensoes  = new complex<double> [geralC->num_tot_linhas];
   vet_TC       = new complex<double> [geralC->num_tot_linhas];

   return(true);
   }

/*
//---------------------------------------------------------------------------
void __fastcall TGauss3::AnalisaCurtoEPUSP(void)
{
// Vari�veis locais
int             num_total_linhas;
complex<double> cvalor;
VTMatriz        *Ynodal_red, *Znodal_red;
VTSisLin        *Ynodal,     *Znodal;

// Inicializa��o (prote��o para delete)
Ynodal_red = NULL;
Znodal_red = NULL;
Ynodal     = NULL;
Znodal     = NULL;

// Copia Ynodal em outra loca��o
Ynodal = monta3->Ynod->Copia("Ynodal");

// Elimina parti��o de Ynodal
num_total_linhas = Ynodal->NumTotalLinhas();
Ynodal->DefineTamanho(0, num_total_linhas);

// Imprime 'Ynodal'
Ynodal->Imprime("D:\\Transfer\\Ynodal.txt");

// Cria e imprime Znodal (inversa de Ynodal)
Znodal = Ynodal->Inverte("Znodal");
Znodal->Imprime("D:\\Transfer\\Znodal.txt");

// Fatora Ynodal parcialmente
Ynodal->MantemLinha(3);  // Linha 4
Ynodal->MantemLinha(4);  // Linha 5
Ynodal->MantemLinha(5);  // Linha 6
Ynodal->Triang();
Ynodal->Imprime("D:\\Transfer\\Ynodal_red.txt");

// Copia 'Ynodal_red' em outra loca��o
Ynodal_red = NewObjMatrizC(3, 3, 0, "Ynodal_red");
for(int i=0; i < 3; i++)
   {
   for(int j=0; j < 3; j++)
      {
      Ynodal->ElementoLC((i+3), (j+3), &cvalor);
      Ynodal_red->C2[i][j] = cvalor;
      }
   }

// Inverte e imprime 'Ynodal_red'
Znodal_red = Ynodal_red->Inverte("Znodal_red");
Znodal_red->Imprime("D:\\Transfer\\Znodal_red.txt");

// Libera mem�ria
delete Ynodal;
delete Znodal;
delete Ynodal_red;
delete Znodal_red;
}
*/

//---------------------------------------------------------------------------
void __fastcall TGauss3::AtualizaTensaoSupPQ(void)
{
int    linha;
TBarra *bar1;
TSup   *sup1;

for(int ns=0; ns < rede3->lisSUP->Count; ns++)
   {
   sup1 = (TSup *) rede3->lisSUP->Items[ns];
   bar1 = sup1->bar_int;               // Barra interna
   if(bar1->tipo != BAR_PQ) continue;  // Descarta tipo irrelevante
   for(int nf=0; nf < bar1->fases->NumFases(); nf++)
      {
      linha = bar1->fases->LinhaIndice(nf);
      if(linha >= 0) sup1->vet_tensao_pu[nf] = vet_tensoes[linha];
      }
   }
}

//---------------------------------------------------------------------------
bool __fastcall TGauss3::CalculaCorrenteGeradores(void)
{
// Vari�veis locais
int     linha, nfa, nfim, num_linhas_submatriz;
clock_t inicio;
TBarra  *bar1;

// Inicializa��o
inicio = clock();

// Calcula contribui��o dos geradores: [Ycg][Vg]
num_linhas_submatriz = monta3->Ynod->NumLinhasSubmatriz();
for(int nlin=0; nlin < num_linhas_submatriz; nlin++)
   {
   vet_corr_ger[nlin] = monta3->Ynod->ProdutoLinhaVetor(nlin, vet_tensoes, 2,
                                                        num_linhas_submatriz);
   }

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCORR_GERADORES, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TGauss3::CalculaRedeCurtoMA(int np)
{
// Vari�veis locais
bool    flag_conv_Gauss, flag_tap_alterado, sucesso;
int     erro_Y, iter_Gauss, num_monta_Y;
clock_t inicio;

// Inicializa vari�veis
inicio            = clock();
flag_tap_alterado = false;
flag_conv_Gauss   = false;  // Controle de converg�ncia (Gauss)
num_monta_Y       = 0;

// Monta Ynodal, incluindo imped�ncia de defeito (espec�fica
// para cada tipo de curto)
sucesso = monta3->MontaY();
num_monta_Y++;
if(! sucesso)
   {
   data->ErroFluxo = rfErroMontagemYnodal;
   return(false);
   }

// Copia Ynodal em outra loca��o antes de fatorar, para uso em
// TCalculo3::CalculaCorrentesPerdas()
delete Ynod2;
Ynod2 = monta3->Ynod->Copia();

// Fatora Ynodal
monta3->Ynod->Triang();
erro_Y = monta3->Ynod->Erro;
if(erro_Y != slSemErro)
   {
   if(erro_Y == slErroMatrizSingular)  // Prov�vel rede desconexa
      {
      if(! monta3->VerificaRedeConexa(false)) data->ErroFluxo = rfErroRedeDesconexa;
      else                                    data->ErroFluxo = rfErroYnodalSingular;
      }
   else
      {
      data->ErroFluxo = rfErroYnodalNaoEspecificado;
      }
   return(false);
   }
monta3->Ynod->DesabilitaOrdenacaoLinhas();

// Calcula contribui��o de corrente dos geradores ([Ycg][Vg])
CalculaCorrenteGeradores();

// Impress�o de dados da rede (s� para debug)
// impr3->ImprimeDados(np);

// Loop do processo iterativo (Gauss)
for(iter_Gauss=0; iter_Gauss <= geralC->MaxIteracao[0]; iter_Gauss++)
   {
   // Calcula termo conhecido = { [Ic] - [Ycg][Vg] }
   if(! CalculaTC())
      {
      data->ErroFluxo = rfErroTermoConhecido;
      return(false);
      }

   // Calcula tens�o nas barras de carga
   monta3->Ynod->Back(vet_TC);
   if(monta3->Ynod->Erro != slSemErro) return(false);

   // Verifica converg�ncia (Gauss)
   flag_conv_Gauss = VerificaConvergencia(iter_Gauss, &flag_tap_alterado);
   if(flag_conv_Gauss) break;
   } // for(iter_Gauss)

// Salva flag de converg�ncia e n�mero de itera��es executadas, e
// atualiza n�mero de patamares calculados
geralC->FlagConv[0]    = flag_conv_Gauss;
geralC->NumIteracao[0] = iter_Gauss;
geralC->NumIteracao[2] = num_monta_Y;
geralC->num_pat_calc++;
if(flag_conv_Gauss) geralC->num_pat_OK++;

// Atualiza tens�o de suprimentos do tipo PQ
AtualizaTensaoSupPQ();

// Calcula e salva valores de tens�es, correntes e perdas
if(! calculo3->CalculaCorrentesPerdas(np, vet_tensoes, Ynod2))
   {
   data->ErroFluxo = rfErroCalculoFinal;
   return(false);
   }

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCALC_REDE, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TGauss3::CalculaRedeFluxo(int  np,
                                          bool flag_imprime)
{
// Vari�veis locais
bool    flag_conv_Gauss, flag_novo_patamar, flag_tap_alterado, flag_Y,
        sucesso;
int     erro_Y, iter_Gauss, num_monta_Y;
clock_t inicio;

// Inicializa vari�veis
inicio            = clock();
flag_novo_patamar = true;
flag_tap_alterado = false;
flag_conv_Gauss   = false;  // Controle de converg�ncia (Gauss)
num_monta_Y       = 0;
LiberaTaps();


/*
// Rede IEEE 13 barras Distribui��o (se o flag estiver ligado)
if(flag_IEEE13)
   {
   for(int ns=0; ns < rede3->lisSUP->Count; ns++)
      {
      double graurad = M_PI / 180.;
      TSup   *sup1   = (TSup *) rede3->lisSUP->Items[ns];
      sup1->vet_tensao[0] = 1000. * sup1->bar_ext->VnomFase(faseA) * polar(1.06250, 0.);
      sup1->vet_tensao[1] = 1000. * sup1->bar_ext->VnomFase(faseA) * polar(1.05000, (-120.*graurad));
      sup1->vet_tensao[2] = 1000. * sup1->bar_ext->VnomFase(faseA) * polar(1.06875, ( 120.*graurad));
      if(ns == 0) break;
      }
   }
*/


// Inicializa Ynodal, se foi determinado que as barras devem ser
// ordenadas neste patamar.
if(flag_ordenar_barras)
   {
   monta3->OrdenaBarrasSup(rede3->lisBAR, geralC, rede3->lisSUP);
   if(! monta3->InicializaYnodal()) return(false);
   if(! AlocaMemoria())             return(false);
   calculo3->InicializaTensao_PQ_EXT_PQV(vet_tensoes);
   }

// Copia em 'vet_tensoes' a tens�o especificada para suprimentos
// neste patamar
calculo3->CopiaTensao_Sup(vet_tensoes);
// impr3->ImprimeVetorComplex("vet_tensoes", monta3->Ynod->NumTotalLinhas(), vet_tensoes);

// Impress�o de dados da rede (s� para debug)
// impr3->ImprimeDados(np);

// Loop do processo iterativo (Gauss)
for(iter_Gauss=0; iter_Gauss <= geralC->MaxIteracao[0]; iter_Gauss++)
   {
   // Determina se Ynodal deve ser montada ou n�o
   flag_Y = DeterminaMontagemYnodal(flag_novo_patamar, flag_tap_alterado);

   // Monta e fatora a matriz Ynodal, se indicado por 'flag_Y'
   if(flag_Y)
      {
      flag_ordenar_barras = false;  // Desliga flag
      sucesso             = monta3->MontaY();
      num_monta_Y++;
      if(! sucesso)
         {
         data->ErroFluxo = rfErroMontagemYnodal;
         return(false);
         }
      // Imprime numera��o global
      // monta3->ImprimeNumeracaoGlobal(0);
      // Imprime matriz Ynodal
      // AnsiString filename1 = geralC->DirTmp() + "\\Ynodal.txt";
      // monta3->Ynod->Imprime(filename1);


      // TEMPOR�RIO
      // Montagem de Znodal e fatora��o parcial de Ynodal (estudo de curto-circuito)
      // IMPORTANTE: ao rodar o fluxo de pot�ncia, escolher a op��o
      //             "Impor modelo de carga Zcte"
      // AnalisaCurtoEPUSP();

      // Copia Ynodal em outra loca��o antes de fatorar, para uso em
      // TCalculo3::CalculaCorrentesPerdas()
      delete Ynod2;
      Ynod2 = monta3->Ynod->Copia();

      // Fatora 'Ynod'
      monta3->Ynod->Triang();
      erro_Y = monta3->Ynod->Erro;
      if(erro_Y != slSemErro)
         {
         if(erro_Y == slErroMatrizSingular)  // Prov�vel rede desconexa
            {
            if(! monta3->VerificaRedeConexa(false)) data->ErroFluxo = rfErroRedeDesconexa;
            else                                    data->ErroFluxo = rfErroYnodalSingular;
            }
         else
            {
            data->ErroFluxo = rfErroYnodalNaoEspecificado;
            }
         return(false);
         }
	  // AnsiString nome_arq = geralC->DirTmp() + "\\Ynodal_Fatorada.txt";
      // monta3->Ynod->Imprime(nome_arq);
      monta3->Ynod->DesabilitaOrdenacaoLinhas();
      }  // if(flag_Y)

   // Calcula contribui��o de corrente dos geradores ([Ycg][Vg]), a qual
   // se alterou se:
   //    1. a matriz Ynodal foi montada (flag_Y), OU
   //    2. as tens�es nos geradores variaram por causa de
   //       um novo patamar (flag_novo_patamar).
   // IMPORTANTE: a fatora��o de Ynodal N�O destroi as submatrizes Ycg,
   //             Ygc e Ygg, ent�o n�o � necess�rio manter outra inst�ncia
   //             n�o-fatorada de Ynodal.
   if(flag_Y || flag_novo_patamar) CalculaCorrenteGeradores();

   // Calcula termo conhecido = { [Ic] - [Ycg][Vg] }
   if(! CalculaTC())
      {
      data->ErroFluxo = rfErroTermoConhecido;
      return(false);
      }

   // Calcula tens�o nas barras de carga
   monta3->Ynod->Back(vet_TC);
   if(monta3->Ynod->Erro != slSemErro) return(false);
   flag_novo_patamar = false;  // J� fez um c�lculo de tens�es

   // Verifica exist�ncia de valores NAN (rede desconexa, etc.)
   if(flag_verifica_NAN)
      {
      if(ExisteNAN()) return(false);  // Erro
      flag_verifica_NAN = false;      // Desabilita novas verifica��es
      }

   // Ajusta tap de reguladores de tens�o (monof�sicos e trif�sicos)
   if(rede3->lisREG12->Count > 0) AjustaTapReguladores1();
   if(rede3->lisREG32->Count > 0) AjustaTapReguladores3();

   // Verifica converg�ncia (Gauss)
   flag_conv_Gauss = VerificaConvergencia(iter_Gauss, &flag_tap_alterado);
   if(flag_conv_Gauss) break;
   } // for(iter_Gauss)

// Salva flag de converg�ncia e n�mero de itera��es executadas, e
// atualiza n�mero de patamares calculados
geralC->FlagConv[0]    = flag_conv_Gauss;
geralC->NumIteracao[0] = iter_Gauss;
geralC->NumIteracao[2] = num_monta_Y;
geralC->num_pat_calc++;
if(flag_conv_Gauss)  // Convergiu
   {
   // Atualiza n�mero de patamares OK
   geralC->num_pat_OK++;
   // Atualiza tens�o de suprimentos do tipo PQ
   AtualizaTensaoSupPQ();
   // Calcula e salva valores de tens�es, correntes e perdas
   if(! calculo3->CalculaCorrentesPerdas(np, vet_tensoes, Ynod2))
      {
      data->ErroFluxo = rfErroCalculoFinal;
      return(false);
      }
   }

// Imprime dados & resultados do patamar
if(flag_imprime)
   {
   impr3->ImprimeDados(np);
   impr3->ImprimeResultados(np, lisBAR_SEM_CONV);
   }

// Grava sistema linear de equa��es (avalia��o da implementa��o em GPU)
// HPS, 2012_12_06
// if(np == 0) GravaSistemaEquacoes();

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCALC_REDE, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TGauss3::CalculaRedeFluxoH3(int np)
   {
   // Esta vers�o calcula o fluxo de pot�ncia em um �nico patamar (fornecido),
   // utilizando objetos externos fornecidos na chamada da fun��o
   // TGauss3::InicializaGauss3_FluxoH3(), chamada por FluxoH3

   // Aloca mem�ria para vetores de c�lculo, se isso n�o foi feito antes
   if(! AlocaMemoria()) return(false);

   // Define valor de 'flag_ordenar_barras'
   // IMPORTANTE: a an�lise harm�nica realizada no patamar anterior destroi
   //             Ynodal, raz�o pela qual � necess�rio mont�-la novamente
   //             ao realizar a an�lise na frequ�ncia fundamental de um
   //             novo patamar
   flag_ordenar_barras = true;

   // Executa c�lculo no patamar fornecido
   if(! CalculaRedeFluxo(np, false)) return(false);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TGauss3::CalculaTC(void)
{
// Vari�veis locais
int             linha, num_fases, num_linhas_submatriz;
complex<double> vet_corr_pu[MAX_FASES_G3];
clock_t         inicio;
TBarra          *bar1;
TFase           *fases;
TSup            *sup1;

// Inicaliza contagem de tempo
inicio = clock();

// Inicializa termo conhecido com o negativo da contribui��o
// de corrente dos geradores (-[Ycg][Vg])
num_linhas_submatriz = monta3->Ynod->NumLinhasSubmatriz();
for(int nlin=0; nlin < num_linhas_submatriz; nlin++)
   {
   vet_TC[nlin] = -vet_corr_ger[nlin];
   }

// Complementa termo conhecido com corrente injetada pelas cargas ([Ic]),
// em fun��o do tipo de tratamento das mesmas
switch(geralC->TipoCarga)
   {
   case tcTODAS_YNODAL:  // Cargas em Ynodal: s� inclui suprimentos PQ (Scte)
      for(int ns=0; ns < rede3->lisSUP->Count; ns++)
         {
         sup1 = (TSup *) rede3->lisSUP->Items[ns];
         bar1 = sup1->bar_int;               // Carga alocada na barra interna, se houver
         if(bar1->tipo != BAR_PQ) continue;  // Suprimento irrelevante
         if(! calculo3->CalculaCorrentesBarra(bar1, vet_tensoes, vet_corr_pu)) return(false);
         fases     = bar1->fases;
         num_fases = fases->NumFases();
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            linha = fases->LinhaIndice(nfa);
            if(linha < 0) continue;            // Este n� � a refer�ncia (terra) da rede
            vet_TC[linha] -= vet_corr_pu[nfa]; // Iinjetada = -Icarga
            }
         }
      break;

   case tcINJECOES_EXT:  // Cargas como inje��es externas
      // IMPORTANTE: devido � implementa��o de n�s de carga e n�s de gera��o
      //             coexistindo na mesma barra de gera��o (2019.07), o loop
      //             abaixo tem que considerar TODAS as barras (n�o somente
      //             as de carga)
      for(int nb=0; nb < rede3->lisBAR->Count; nb++)
         {
         bar1 = (TBarra *) rede3->lisBAR->Items[nb];
         if(! calculo3->CalculaCorrentesBarra(bar1, vet_tensoes, vet_corr_pu)) return(false);
         fases     = bar1->fases;
         num_fases = fases->NumFases();
         for(int nfa=0; nfa < num_fases; nfa++)
            {
            linha = fases->LinhaIndice(nfa);
            if(linha < 0)          continue;   // Este n� � a refer�ncia (terra) da rede
            if(IsNoGeracao(linha)) continue;   // Descarta n� de gera��o
            vet_TC[linha] -= vet_corr_pu[nfa]; // Iinjetada = -Icarga
            }
         }
      break;

   default:  // tcSEM_CARGA ou erro (nada a fazer)
      break;
   }

// Acumula tempo de processamento
geralC->tCPU->Acumula(ttCALC_TC, inicio);

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TGauss3::DefineCurto(int tipo)
{
// Vari�veis locais
int           fase_int;
TBarra        *bar1;
TBarraDefeito *bar_def;

// Salva 'tipo'
bar_def             = (TBarraDefeito *) curto_aux3->lisBAR_DEF->First();
bar_def->tipo_curto = tipo;  // cuTRI, cu2F, ...

// Determina fases da barra de defeito
bar1     = bar_def->barra;
fase_int = bar1->fases->FasesAsInt();

// Define e salva 'fases_curto' (ctAN, ctBN, ...) em fun��o do tipo de
// defeito e das fases existentes na barra de defeito
switch(tipo)
   {
   case cuTRI:  // Curto trif�sico
      if((fase_int & faseABC) == faseABC) bar_def->fases_curto = ctABCT;
      else                                return(false);  // Erro
      break;

   case cu2F:  // Curto dupla fase (BC � preferencial)
      if     ((fase_int & faseBC) == faseBC) bar_def->fases_curto = ctBC;
      else if((fase_int & faseAB) == faseAB) bar_def->fases_curto = ctAB;
      else if((fase_int & faseCA) == faseCA) bar_def->fases_curto = ctCA;
      else                                   return(false);  // Erro
      break;

   case cuFT:   // Curto fase-terra FRANCO
   case cuFTZ:  // Curto fase-terra COM IMPED�NCIA
      if     ((fase_int & faseA) == faseA) bar_def->fases_curto = ctAT;
      else if((fase_int & faseB) == faseB) bar_def->fases_curto = ctBT;
      else if((fase_int & faseC) == faseC) bar_def->fases_curto = ctCT;
      else                                 return(false);  // Erro
      break;

   case cu2FT:  // Curto dupla fase-terra (BC � preferencial)
      if     ((fase_int & faseBC) == faseBC) bar_def->fases_curto = ctBCT;
      else if((fase_int & faseAB) == faseAB) bar_def->fases_curto = ctABT;
      else if((fase_int & faseCA) == faseCA) bar_def->fases_curto = ctCAT;
      else                                   return(false);  // Erro
      break;

   default:  // Erro
      return(false);
   }

// Reinicia imped�ncias de defeito no objeto TCurtoAux3
if(! curto_aux3->ReiniciaImpedanciasDefeito(tipo)) return(false);

// Retorna
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TGauss3::DeterminaMontagemYnodal(bool flag_novo_patamar,
                                                 bool flag_tap_alterado)
{
bool cond;

// Condi��o que INDEPENDE de 'flag_novo_patamar' para montar Ynodal:
//    1. flag_tap_alterado == true (pelo menos um tap de regulador alterado)

// Condi��es que DEPENDEM de 'flag_novo_patamar == true' para montar Ynodal:
//    2. (geralC->num_pat_calc == 0)       (primeiro c�lculo geral), OU
//    3. (flag_ordenar_barras == true)     (as barras foram/ser�o ordenadas), OU
//    4. (geralC->CargaEmYnodal() == true) (cargas de Z conste. em Ynodal).

// Condi��o 1
if(flag_tap_alterado) return(true);

// Condi��es 2, 3 e 4
cond = false;
if(flag_novo_patamar)
   {
   cond = (geralC->num_pat_calc == 0) ||
          flag_ordenar_barras         ||
          geralC->CargaEmYnodal;
   }

return(cond);
}

//---------------------------------------------------------------------------
bool __fastcall TGauss3::ExecutaCurtoMA(int      ind_pat,
                                        VTData   *data,
                                        VTBarra  *barra_sinap,
                                        VTNet    *net,
                                        VTRedeCC *redeCC,
                                        TList    *lisBAR_DEF_SINAP)
   {
   // Vari�veis locais
   bool            sucesso = false;
   strSinapCurtoMA *Curto;

   //salva ponteiro para objetos
   this->data       = data;
   this->net        = net;
   this->redeCC     = redeCC;
   this->lisBAR_DEF = lisBAR_DEF_SINAP;
   //verifica se foi definida uma �nica Barra de defeito p/ c�lculo de todos tipos de defeito
   if (barra_sinap != NULL)
      {//prote��o
      if(barra_sinap->No[net] == NULL) return(false);
      //cria uma lista local
      lisBAR_DEF = new TList();
      //define uma �nica estrutura strSinapCurtoMA
      strSinapCurtoMA          CurtoUnico;
      CurtoUnico.barra_sinap  = barra_sinap;
      CurtoUnico.fases_sinap  = faseAT; // Qualquer valor v�lido funciona neste ponto
      //insere strSinapCurtoMA em lisBAR_DEF
      lisBAR_DEF->Add(&CurtoUnico);
      //executa c�lculo em uma �nica barra de defeito (todos os tipos de defeito)
      sucesso = ExecutaCurtoMA_Gauss3(ind_pat);
      //destr�i lista criada
      delete lisBAR_DEF;
      }
   //verifica se foi definida uma lista de barras de defeito
   else if (lisBAR_DEF != NULL)
      {//executa c�lculo de defeitos simult�neos
      sucesso = ExecutaCurtoMA_Gauss3_Simultaneo(ind_pat);
      }
   lisBAR_DEF = NULL;
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TGauss3::ExecutaCurtoMA_Gauss3(int ind_pat)
{
// Vari�veis locais
bool            salva_zent, sucesso;
int             np;
clock_t         inicio;
strSinapCurtoMA *Curto;
TBarra          *bar1;
VTBarra         *barra_sinap;


// TEMPOR�RIO
if(ind_pat < 0) ind_pat = 0; // Precisa atualizar suprimentos com um patamar v�lido

// Inicializa��o
np     = ind_pat;
inicio = clock();

// Cria objetos externos
curto_aux3 = NewObjCurtoAux3(apl, lisBAR_DEF->Count);
geralC     = NewObjGeralC(progCURTO_MA, TipoRelatorio, apl, data);
rede3      = NewObjRede3(geralC);
atual3     = NewObjAtualiza3(rede3, geralC);
result3    = NewObjResultados3(rede3, geralC);
impr3      = NewObjImpressao3(rede3, geralC, curto_aux3);
monta3     = NewObjMonta3(impr3, rede3, geralC, curto_aux3);
calculo3   = NewObjCalculo3(rede3, monta3, geralC, curto_aux3);

// Liga flag para indicar que os objetos externos s�o controlados
// por Gauss3 (ao contr�rio do que ocorre quando o c�lculo via Gauss3 �
// solicitado por FluxoH3, cf. InicializaGauss3_FluxoH3() +
//                             CalculaRedeFluxoH3())
flag_alocou_externos = true;

// Monta a rede
// Obs.: a verifica��o de rede conexa j� est� em 'CalculaRedeCurtoMA()'
if(! monta3->MontaRedeGauss3()) return(false);

// Obt�m ponteiro da barra para defeito e o salva no objeto TCurtoAux3
Curto       = (strSinapCurtoMA *) lisBAR_DEF->First();
barra_sinap = Curto->barra_sinap;
bar1        = rede3->LocalizaBarraId(barra_sinap->No[net]->pai->Id);
if(bar1 == NULL) return(false);
sucesso = curto_aux3->AddBarraDefeito(bar1);
if(! sucesso) return(false);

// Ordena barras
monta3->OrdenaBarrasSup(rede3->lisBAR, geralC, rede3->lisSUP);

// Inicializa Ynodal
if(! monta3->InicializaYnodal()) return(false);

// Calcula Zentrada na barra de defeito, nas sequ�ncias direta e
// zero, para c�lculo posterior das correntes assim�tricas
// NB: chamada SEM argumento: 'so_trechos' = false
//     chamada COM argumento: 'so_trechos' = true
if(! monta3->CalculaZentrada_1Vez3nx3n())     return(false);
if(! monta3->CalculaZentrada_1Vez3nx3n(true)) return(false);

// Calcula pot�ncias de curto-circuito
curto_aux3->CalculaPotenciasCurtoCircuito(geralC);

// Atualiza componentes da rede para o patamar atual
if(! atual3->AtualizaGauss3(np)) return(false);

// Zera perda de energia
calculo3->ZeraPerdaEnergia();

// Aloca mem�ria para vetores usados no c�lculo
if(! AlocaMemoria()) return(false);

// Inicializa tens�o nas barras de carga
calculo3->InicializaTensao_PQ_EXT_PQV(vet_tensoes);

// Copia em 'vet_tensoes' a tens�o especificada para suprimentos
// neste patamar
calculo3->CopiaTensao_Sup(vet_tensoes);

// Loop dos tipos de curto-circuito (cuTRI, cu2F, cuFT, cuFTZ, cu2FT)
for(int tipo=0; tipo < MAX_CU; tipo++)
   {
   // Define tipo do curto e executa c�lculo no patamar especificado
   // IMPORTANTE: ocorrendo erro na defini��o do tipo de defeito ou no
   //             c�lculo, a execu��o prossegue no pr�ximo tipo de defeito
   //             (n�o � abandonada). Por exemplo, o c�lculo de defeito
   //             trif�sico no secund�rio de um trafo monof�sico retorna
   //             erro [return(false)], mas n�o o c�lculo de defeito fase-terra.
   salva_zent = (tipo == 0); // S� salva as imped�ncias de entrada na primeira passagem
   if(! DefineCurto(tipo))      continue;
   if(! CalculaRedeCurtoMA(np)) continue;
   impr3->ImprimeDados(np);
   if(! geralC->FlagConv[0])    continue; // N�o convergiu
   if(! result3->ResGauss3_CurtoMA(curto_aux3, tipo, salva_zent)) return(false);
   impr3->ImprimeResultados(np);
   } // for(classe)

// Finaliza��o
result3->ResPerdaEnergia();
geralC->tCPU->Acumula(ttGAUSS3, inicio);
Finaliza();
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TGauss3::ExecutaCurtoMA_Gauss3_Simultaneo(int ind_pat)
{
// Vari�veis locais
bool            salva_zent, sucesso;
int             fases_curto, np, tipo_curto;
clock_t         inicio;
strSinapCurtoMA *Curto;
TBarra          *bar1;
TBarraDefeito   *bar_def;
VTBarra         *barra_sinap;


// TEMPOR�RIO
if(ind_pat < 0) ind_pat = 0; // Precisa atualizar suprimentos com um patamar v�lido


// Inicializa��o
np     = ind_pat;
inicio = clock();

// Cria objetos externos
curto_aux3 = NewObjCurtoAux3(apl, lisBAR_DEF->Count);
geralC     = NewObjGeralC(progCURTO_MA, TipoRelatorio, apl, data);
rede3      = NewObjRede3(geralC);
atual3     = NewObjAtualiza3(rede3, geralC);
result3    = NewObjResultados3(rede3, geralC);
impr3      = NewObjImpressao3(rede3, geralC, curto_aux3);
monta3     = NewObjMonta3(impr3, rede3, geralC, curto_aux3);
calculo3   = NewObjCalculo3(rede3, monta3, geralC, curto_aux3);

// Liga flag para indicar que os objetos externos s�o controlados
// por Gauss3 (ao contr�rio do que ocorre quando o c�lculo via Gauss3 �
// solicitado por FluxoH3, cf. InicializaGauss3_FluxoH3() +
//                             CalculaRedeFluxoH3())
flag_alocou_externos = true;

// Monta a rede
// Obs.: a verifica��o de rede conexa j� est� em 'CalculaRedeCurtoMA()'
if(! monta3->MontaRedeGauss3()) return(false);

// Monta lista de barras para defeito
for(int nb=0; nb < lisBAR_DEF->Count; nb++)
   {
   Curto       = (strSinapCurtoMA *) lisBAR_DEF->Items[nb];
   barra_sinap = Curto->barra_sinap;
   bar1        = rede3->LocalizaBarraId(barra_sinap->No[net]->pai->Id);
   if(bar1 == NULL) return(false);
   tipo_curto = curto_aux3->TipoFasesCurto(Curto->fases_sinap, &fases_curto);
   if(tipo_curto < 0) return(false);
   sucesso = curto_aux3->AddBarraDefeito(bar1, tipo_curto, fases_curto);
   if(! sucesso) return(false);
   }

/*
// CODIGO TEPOR�RIO
curto_aux3->Reinicia(2);  // Duas barras para curto-circuito
bar1 = rede3->LocalizaBarraCodigo("B_1203");
if(bar1 == NULL) return(false);  // Erro
curto_aux3->AddBarraDefeito(bar1, cuTRI, ctABCT);
bar1 = rede3->LocalizaBarraCodigo("B_3903");
if(bar1 == NULL) return(false);  // Erro
curto_aux3->AddBarraDefeito(bar1, cuFT, ctAT);
//FIM DO C�DIGO TEMPOR�RIO
*/

// Ordena barras
monta3->OrdenaBarrasSup(rede3->lisBAR, geralC, rede3->lisSUP);

// Inicializa Ynodal
if(! monta3->InicializaYnodal()) return(false);

// Calcula Zentrada na barra de defeito, nas sequ�ncias direta e
// zero, para c�lculo posterior das correntes assim�tricas
if(! monta3->CalculaZentrada_1Vez3nx3n()) return(false);

// Atualiza componentes da rede para o patamar atual
if(! atual3->AtualizaGauss3(np)) return(false);

// Zera perda de energia
calculo3->ZeraPerdaEnergia();

// Aloca mem�ria para vetores usados no c�lculo
if(! AlocaMemoria()) return(false);

// Inicializa tens�o nas barras de carga
calculo3->InicializaTensao_PQ_EXT_PQV(vet_tensoes);

// Copia em 'vet_tensoes' a tens�o especificada para suprimentos
// neste patamar
calculo3->CopiaTensao_Sup(vet_tensoes);

// Executa c�lculo e retorna e imprime resultados
bar_def    = (TBarraDefeito *) curto_aux3->lisBAR_DEF->First();
tipo_curto = bar_def->tipo_curto;
salva_zent = true;
if(! CalculaRedeCurtoMA(np)) return(false);
impr3->ImprimeDados(np);
if(! geralC->FlagConv[0])    return(false);
if(! result3->ResGauss3_CurtoMA(curto_aux3, tipo_curto, salva_zent)) return(false);
impr3->ImprimeResultados(np);

// Finaliza��o
result3->ResPerdaEnergia();
geralC->tCPU->Acumula(ttGAUSS3, inicio);
Finaliza();
return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TGauss3::ExecutaFluxo(VTData *data)
   {
   //vari�veis locais
   bool sucesso;

   //salva ponteiro p/ objeto
   this->data = data;
   try{//executa c�lculos
      sucesso = ExecutaFluxo_Gauss3();
      }catch(Exception &e)
         {//erro
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TGauss3::ExecutaFluxo_Gauss3(void)
   {
   // Vari�veis locais
   clock_t inicio;

   // Inicializa��o
   inicio = clock();

   // Cria objetos externos
   geralC   = NewObjGeralC(progG3, TipoRelatorio, apl, data, RelatorioBarDesc);
   rede3    = NewObjRede3(geralC);
   atual3   = NewObjAtualiza3(rede3, geralC);
   result3  = NewObjResultados3(rede3, geralC);
   impr3    = NewObjImpressao3(rede3, geralC);
   monta3   = NewObjMonta3(impr3, rede3, geralC);
   calculo3 = NewObjCalculo3(rede3, monta3, geralC);


// DEBUG
// geralC->DefineTipoCarga(tcSEM_CARGA);
// geralC->flag_TRAFO5_Como_Trafo3E = false;
// geralC->flag_utiliza_bancoDF     = false;
// geralC->flag_ypar_trafos         = true;
// geralC->flag_capac_LT            = false;
// geralC->flag_imprime_bar_desc    = true;
// geralC->usa_pi_equiv             = true;
// flag_IEEE13                      = true;


   // Liga flag para indicar que os objetos externos s�o controlados
   // por Gauss3 (ao contr�rio do que ocorre quando o c�lculo via Gauss3 �
   // solicitado por FluxoH3, cf. InicializaGauss3_FluxoH3() +
   //                             CalculaRedeFluxoH3())
   flag_alocou_externos = true;

   // Prepara a rede - parte independente dos patamares
   // Obs.: a verifica��o de rede conexa j� est� em 'CalculaRedeFluxo()'
   if(! monta3->MontaRedeGauss3()) return(false);
   calculo3->ZeraPerdaEnergia();


   // Acerta imped�ncias e admit�ncias para a rede IEEE 13 barras
   // (se o flag estiver ligado)
   // if(flag_IEEE13)
   //    {
   //    if(! AcertaImpedanciasAdmitancias_IEEE13()) return(false);
   //    }


   // Loop de patamares
   // A impress�o dos dados do caso est� depois do c�lculo da rede porque a
   // inicializa��o de Ynodal (defini��o de linhas, que � impressa no
   // relat�rio de dados), � feita em CalculaRede().
   for(int np=0; np < geralC->num_tot_pat; np++)
      {
      if(! geralC->PatamarHabilitado(np, rede3->lisBATERIA)) continue; // Ignora patamar desabilitado
      flag_ordenar_barras |= monta3->AnalisaSuprimentosPatamar(np);
      if(! atual3->AtualizaGauss3(np))   return(false);
      if(! CalculaRedeFluxo(np, true))   return(false);
      if(! geralC->FlagConv[0])          continue; // N�o convergiu (n�o retorna resultados)
      if(! result3->ResGauss3_Fluxo(np)) return(false);
      } // for(np)

   // Finaliza��o
   result3->ResPerdaEnergia();
   geralC->tCPU->Acumula(ttGAUSS3, inicio);
   Finaliza();
   return(geralC->num_pat_OK == geralC->num_pat_calc);
   }

//---------------------------------------------------------------------------
bool __fastcall TGauss3::ExisteNAN(void)
   {
   // Vari�veis locais
   int num_linhas_submatriz;

   // Verifica 'vet_TC'
   num_linhas_submatriz = monta3->Ynod->NumLinhasSubmatriz();
   for(int nlin=0; nlin < num_linhas_submatriz; nlin++)
      {
      if(IsNan(vet_TC[nlin].real())) return(true);
      if(IsNan(vet_TC[nlin].imag())) return(true);
      }

   // Valores OK (n�o NAN)
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TGauss3::Finaliza(void)
   {
   // Prints processing summary
   impr3->ImprimeResumoProc();

   // Insere relat�rios gerados pelo programa
   if(geralC->tipo_relatorio != trSEM_RELATORIO)
      {
      data->InsereRelatorio(geralC->strFilename->dados);
      data->InsereRelatorio(geralC->strFilename->result);
      }

   return(true);
   }

/*
//---------------------------------------------------------------------------
bool __fastcall TGauss3::GravaSistemaEquacoes(void)
   {
   // Grava sistema linear de equa��es (avalia��o da implementa��o em GPU)
   // HPS, 2012_12_06

   // Vari�veis locais
   int        jinic, jm1, num_linhas, num_nao_nulos;
   AnsiString nome1, nome2, nome3;
   FILE       *file1;
   TList      *lisELEMS;
   TElemC     *elem_i, *elem_j;

   // Define nome de arquivos;
   nome1 = geralC->DirTmp() + "\\SL_Matriz.txt";
   nome2 = geralC->DirTmp() + "\\SL_Vetor_b.txt";
   nome3 = geralC->DirTmp() + "\\SL_Vetor_x.txt";

   // Monta e grava matriz Ynodal
   if(! monta3->MontaY()) return(false);
   // Conta elementos n�o-nulos
   num_linhas    = monta3->Ynod->NumLinhasSubmatriz();
   num_nao_nulos = 0;
   for(int nl=0; nl < num_linhas; nl++)
      {
      lisELEMS = monta3->Ynod->ElementosTriangSupLinha(nl);
      for(int ne=0; ne < lisELEMS->Count; ne++)
         {
         elem_i = (TElemC *) lisELEMS->Items[ne];
         if(elem_i->coluna < num_linhas) num_nao_nulos++; // Descarta elementos de [Ycg]
         }
      }
   file1 = fopen(nome1.c_str(), "w");
   fprintf(file1, "%d %d %d\n", num_linhas, num_linhas, num_nao_nulos);
   for(int nl=0; nl < num_linhas; nl++)
      {
      lisELEMS = monta3->Ynod->ElementosTriangSupLinha(nl);
      // Ordena elementos em 'lisELEMS' em ordem crescente da coluna
      for(int i=1; i < lisELEMS->Count; i++)
         {
         elem_i = (TElemC *) lisELEMS->Items[i];
         jinic  = i - 1;
         jm1    = i;
         for(int j=jinic; j >= 0; j--)
            {
            elem_j = (TElemC *) lisELEMS->Items[j];
            if(elem_j->coluna <= elem_i->coluna) break;
            lisELEMS->Items[jm1] = elem_j;
            jm1                  = j;
            }
         lisELEMS->Items[jm1] = elem_i;
         }
      for(int ne=0; ne < lisELEMS->Count; ne++)
         {
         elem_i = (TElemC *) lisELEMS->Items[ne];
         if(elem_i->coluna < num_linhas) // Descarta elementos de [Ycg]
            {
            fprintf(file1, "%d %d %20.12e %20.12e\n", nl, elem_i->coluna,
                    elem_i->valor.real(), elem_i->valor.imag());
            }
         }
      }
   fclose(file1);

   // Fatora matriz Ynodal
   monta3->Ynod->Triang();
   if(monta3->Ynod->Erro != slSemErro) return(false);

   // Monta e grava termo conhecido
   if(! CalculaTC()) return(false);
   file1 = fopen(nome2.c_str(), "w");
   for(int nl=0; nl < num_linhas; nl++)
      {
      fprintf(file1, "%20.12e %20.12e\n", vet_TC[nl].real(), vet_TC[nl].imag());
      }
   fclose(file1);

   // Resolve sistema de equa��es e grava vetor solu��o
   monta3->Ynod->Back(vet_TC);
   if(monta3->Ynod->Erro != slSemErro) return(false);
   file1 = fopen(nome3.c_str(), "w");
   for(int nl=0; nl < num_linhas; nl++)
      {
      fprintf(file1, "%20.12e %20.12e\n", vet_TC[nl].real(), vet_TC[nl].imag());
      }
   fclose(file1);

   return(true);
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TGauss3::InicializaGauss3_FluxoH3(FluxoH3_G3 *str)
{
// Redefine ponteiros para objetos externos
geralC   = str->geralC;
rede3    = str->rede3;
atual3   = str->atual3;
impr3    = str->impr3;
monta3   = str->monta3;
calculo3 = str->calculo3;
data     = geralC->data;  // Obrigat�rio!

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TGauss3::InsereZdef_CurtoSimultaneo(void)
{
// Local variables
int             lin1;
double          sbase_fase_inv, vnom1_kv, zbase11;
complex<double> y11, ydef_terra_S;
TBarra          *bar1;
TFase           *fases;

/*
// Prote��es
bar1 = bar_def2;                      // Segunda barra para defeito
if(bar1 == NULL)        return(true); // Barra para defeito indefinida
if(! bar1->flag_conexa) return(true); // Barra para defeito desconexa

// Inicializa��o
sbase_fase_inv = 1. / geralC->Sbase_fase;
fases          = bar1->fases;

// Obt�m admit�ncias de defeito do objeto VTCurtoAux (j� calculadas pela chamada
// a 'TCurtoAux::CalculaAdmitanciasDefeito()' em 'TGauss3::DefineCurto()')
ydef_terra_S = curto_aux3->ydef_terra_S * 2.; // Multiplica por 2 para ser diferente

// Insere admit�ncia de defeito em Ynodal
lin1     = fases->LinhaFase(faseB);  // Segunda barra: fase B
vnom1_kv = bar1->VnomFase(faseB);
if(lin1 < 0) return(false);  // Erro
zbase11 = vnom1_kv * vnom1_kv * sbase_fase_inv;
y11     = ydef_terra_S * zbase11;
monta3->Ynod->InsereDiagonal(lin1, y11);

// Retorna
return(monta3->Ynod->Erro == slSemErro);
*/

return(true);
}

//---------------------------------------------------------------------------
bool __fastcall TGauss3::IsNoGeracao(int linha)
{
if(linha < geralC->num_lin_submatriz) return(false);  // N� de carga
else                                  return(true);   // N� de gera��o
}

//---------------------------------------------------------------------------
void __fastcall TGauss3::LiberaTaps(void)
{
// Vari�veis locais
TReg32 *reg32;

for(int nr=0; nr < rede3->lisREG32->Count; nr++)
   {
   reg32 = (TReg32 *) rede3->lisREG32->Items[nr];
   reg32->LiberaTaps();
   }
}

//---------------------------------------------------------------------------
int __fastcall TGauss3::NumPatamaresOK(void)
{
return(geralC->num_pat_OK);
}

//---------------------------------------------------------------------------
bool __fastcall TGauss3::VerificaConvergencia(int  iter_Gauss,
                                              bool *flag_tap_alterado)
{
// Vari�veis locais
int             fase_int, linha;
bool            flag_conv, teste;
// double          fator_prec, Precisao_V;
complex<double> v_pu, vant_pu;
TBarra          *bar1;
TFase           *fases;
TReg12          *reg12;
TReg32          *reg32;

// Inicializa vari�veis de retorno
*flag_tap_alterado = false;  // Em princ�pio, nenhum tap foi alterado
flag_conv          = true;   // Em princ�pio, o caso convergiu
// fator_prec         = 1000. * geralC->Precisao[0];  // [kV] -> [V]

// Salva valores de tens�o para a pr�xima itera��o e verifica converg�ncia
for(int nb=0; nb < rede3->lisBAR->Count; nb++) // NB: todas as barras
   {
   bar1  = (TBarra *) rede3->lisBAR->Items[nb];
   fases = bar1->fases;
   for(int nfa=0; nfa < fases->NumFases(); nfa++)
      {
      fase_int = fases->Fase(nfa);
      if(bar1->IsNoGeracao(fase_int)) continue; // Ignora n� de gera��o em barra de gera��o
      linha = fases->LinhaIndice(nfa);
      if(linha < 0) continue;                   // Neutro solidamente aterrado
      vant_pu            = vet_tensoes[linha];
      v_pu               = vet_TC[linha];
      vet_tensoes[linha] = v_pu;   // Atualiza tens�o
      if(! flag_conv) continue;    // N�o precisa mais verificar converg�ncia
      teste = Abs((v_pu - vant_pu), geralC->Precisao[0]) > 0.;
      if(teste) flag_conv = false; // N�o pode abandonar o loop na primeira
                                   // ocorr�ncia de 'teste == true' pois � preciso
                                   // completar antes a transferencia de TODAS as
                                   // tens�es de 'vet_TC' para 'vet_tensoes'
      }
   }

// IMPORTANTE: mesmo com 'flag_conv' == false, � necess�rio verificar se houve
//             altera��o de tap, para eventual nova montagem de Ynodal e
//             eventual bloqueio de tap

// Reguladores monof�sicos
for(int nr=0; nr < rede3->lisREG12->Count; nr++)
   {
   reg12 = (TReg12 *) rede3->lisREG12->Items[nr];
   if(reg12->AlterouTap())
      {
      *flag_tap_alterado = true;  // Tap alterado, dever� montar Ynodal na pr�xima itera��o
      flag_conv          = false; // N�o convergiu
      }
   }

// Reguladores trif�sicos
for(int nr=0; nr < rede3->lisREG32->Count; nr++)
   {
   reg32 = (TReg32 *) rede3->lisREG32->Items[nr];
   if(reg32->AlterouTap())
      {
      *flag_tap_alterado = true;  // Tap alterado, dever� montar Ynodal na pr�xima itera��o
      flag_conv          = false; // N�o convergiu
      if(iter_Gauss >= geralC->num_iter_min)
         {
         for(int nt=0; nt < reg32->NumTaps(); nt++)
            {
            if(abs(reg32->DeltaPasso[nt]) <= 1) reg32->TapBloqueado[nt] = true;
            }
         }
      }
   }

return(flag_conv);
}

//---------------------------------------------------------------------------

