//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include ".\TFPcc.h"
#include ".\TBar.h"
#include ".\TLig.h"
#include ".\VTData.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Diretorio\VTPath.h>
#include <Fontes\SisLin\VTSisLin.h>
#include <DLL_Inc\SisLin.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe VTFPcc
//---------------------------------------------------------------------------
VTFPcc * __fastcall NewObjFPcc(VTApl *apl)
   {
   return(new TFPcc(apl));
   }

//---------------------------------------------------------------------------
// Fun��o global para ordena��o de barras
static int __fastcall EscolheBarraCodigo(void *obj1,
                                         void *obj2)
   {
   // Vari�veis locais
   TBar *bar1, *bar2;

   // Primeiro crit�rio: c�digo da barra em ordem crescente
   bar1 = (TBar *) obj1;
   bar2 = (TBar *) obj2;
   return(bar1->codigo.AnsiCompareIC(bar2->codigo));
   }

//---------------------------------------------------------------------------
// Fun��o global para ordena��o de liga��es
static int __fastcall EscolheLigacao(void *obj1,
                                     void *obj2)
   {
   // Vari�veis locais
   int        num1;
   AnsiString cod_bar1, cod_bar2;
   TBar       *bar1, *bar2;
   TLig       *lig1, *lig2;

   // Obt�m trechos
   lig1 = (TLig *) obj1;
   lig2 = (TLig *) obj2;

   // Primeiro crit�rio: c�digo da barra inicial em ordem crescente
   // (para cada liga��o, a barra inicial � a menor entre 'bar1' e 'bar2')
   bar1 = lig1->bar1;
   bar2 = lig2->bar1;
   if(lig1->bar2->codigo.AnsiCompareIC(bar1->codigo) < 0) bar1 = lig1->bar2;
   if(lig2->bar2->codigo.AnsiCompareIC(bar2->codigo) < 0) bar2 = lig2->bar2;
   num1 = bar1->codigo.AnsiCompareIC(bar2->codigo);
   if     (num1 < 0) return(-1);
   else if(num1 > 0) return( 1);

   // Segundo crit�rio: c�digo da barra final em ordem crescente
   if(bar1 == lig1->bar1) bar1 = lig1->bar2;
   else                   bar1 = lig1->bar1;
   if(bar2 == lig2->bar1) bar2 = lig2->bar2;
   else                   bar2 = lig2->bar1;
   return(bar1->codigo.AnsiCompareIC(bar2->codigo));
   }

//---------------------------------------------------------------------------
__fastcall TFPcc::TFPcc(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;

   // Inicializa��o
   RadGrau  = 180. / M_PI;
   NomeRede = AnsiString("");
   }

//---------------------------------------------------------------------------
__fastcall TFPcc::~TFPcc(void)
   {
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFPcc::DirTmp(void)
{
// Vari�veis locais
AnsiString string1;
VTPath     *path = (VTPath *) apl->GetObject(__classid(VTPath));

// Monta e retorna diret�rio Tmp completo
string1 = path->DirBase + "\\Tmp";
return(string1);
}

//---------------------------------------------------------------------------
void __fastcall TFPcc::EnableAviso(bool enabled)
   {
   aviso.enabled = enabled;
   }

//---------------------------------------------------------------------------
void __fastcall TFPcc::EnableRelatorio(bool enabled)
   {
   relatorio.enabled = enabled;
   }

//---------------------------------------------------------------------------
bool __fastcall TFPcc::Executa(VTData     *data,
                               AnsiString NomeRede,
                               int        ind_pat)
   {
   //vari�veis locais
   bool sucesso;
   int  num_bar;

   //salva ponteiro p/ objeto
   this->data     = data;
   this->NomeRede = NomeRede;

   //define uma �nica Bar swing no final da lista de Bar
   PreparaBarSwing();

   //define �ndices de Bar que ser�o usados na montagem da matriz bnodal
   PreparaIndiceBar();

   //determina n�mero de Barras da  rede
   num_bar = data->NumBar;

   try{//cria objetos usados no c�lculo
      vetorB = new double[num_bar];
      bnodal = DLL_NewObjSisLinD(num_bar-1, num_bar);
      if (bnodal == NULL) return(false);

      //executa c�lculo de fluxo
      sucesso = ExecutaCalculo(ind_pat);

      //destr�i objetos
      delete[] vetorB;
      delete bnodal;

      }catch(Exception &e)
         {
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TFPcc::ExecutaCalculo(int ind_pat)
   {
   //vari�veis locais
   double paux, pger_swing;
   TBar   *bar, *swing;
   TLig   *lig;
   TList  *lisBAR = data->LisBarra();
   TList  *lisLIG = data->LisLigacao();

   //insere Lig em bnodal
   for(int n = 0; n < lisLIG->Count; n++)
      {
      lig = (TLig*)lisLIG->Items[n];
      bnodal->InsereQuatro(lig->bar1->index, lig->bar2->index, lig->bser);
      }
   // VTPath     *path1    = (VTPath *) apl->GetObject(__classid(VTPath));
   // AnsiString nome_arq1 = path1->DirTmp() + AnsiString("\\") + NomeRede +
   //                        AnsiString("_Bnodal_AF.txt");
   // bnodal->Imprime(nome_arq1.c_str());
   if (bnodal->Erro != slSemErro) return(false);

   //executa triangulariza��o
   bnodal->Triang();
   if(bnodal->Erro != slSemErro)
      {
      // VTPath     *path2    = (VTPath *) apl->GetObject(__classid(VTPath));
      // AnsiString nome_arq2 = path2->DirTmp() + AnsiString("\\") + NomeRede +
      //                        AnsiString("_Bnodal_DF.txt");
      // bnodal->Imprime(nome_arq2.c_str());
      return(false);
      }

   //determina Bar swing
   swing = (TBar *) lisBAR->Last();

   //monta vetorB c/ as pot�ncias injetadas em Bar e valor nulo p/ a swing
   vetorB[swing->index] = 0;
   pger_swing           = 0.;
   for (int n = 0; n < lisBAR->Count-1; n++)
      {
      bar                 = (TBar*)lisBAR->Items[n];
      paux                = -(bar->p_ger - bar->p_car);
      vetorB[bar->index]  = paux;
      pger_swing         += paux;
      }
   swing->p_ger = pger_swing + swing->p_car;

   //resolve o sistema de equa��es(vetorB fica c/ os resultados: �ngulos em rad)
   bnodal->Back(vetorB);

   //salva os �ngulos do vetorB em TBar, e inclui o m�dulo da Bar swing
   for (int n = 0; n < lisBAR->Count; n++)
      {
      bar = (TBar*)lisBAR->Items[n];
      bar->res.v_pu       = swing->res.v_pu;
      bar->res.angulo_rad = vetorB[bar->index];
      }

   //calcula os fluxos de pot�ncia e salva em Lig
   for (int n = 0; n < lisLIG->Count; n++)
      {
      lig = (TLig*)lisLIG->Items[n];
      lig->res.fluxo12_pu = -lig->bser * (lig->bar1->res.angulo_rad -
                                          lig->bar2->res.angulo_rad);
      }

   // Imprime resultados
   ImprimeResultados(ind_pat);

   // Retorna
   return(true);
   }

//---------------------------------------------------------------------------
TBar* __fastcall TFPcc::ExisteBar(TList *lisBAR,
                                  int   tipo)
   {
   //vari�veis locais
   TBar *bar;

   for (int n = 0; n < lisBAR->Count; n++)
      {
      bar = (TBar *) lisBAR->Items[n];
      if(bar->tipo == tipo) return(bar);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFPcc::ImprimeResultados(int ind_pat)
{
// Vari�veis locais
int        tipo;
double     ang_grau, pc1, pg1, pi1, somac, somag;
AnsiString nome_arq, str1;
FILE       *arq;
TBar       *bar1, *bar2;
TLig       *lig;
TList      *lisBAR, *lisBAR_ORD;
TList      *lisLIG, *lisLIG_ORD;
// VTPath     *path;

// Abre arquivo de resultados
// path     = (VTPath *) apl->GetObject(__classid(VTPath));
// nome_arq = path->DirTmp() + "\\" + NomeRede + "_FluxoCC_Resultados.txt";

nome_arq = DirTmp() + "\\" + NomeRede + "_FluxoCC_Resultados.txt";
arq      = fopen(nome_arq.c_str(), "w");
if(arq == NULL) return;

// Inicializa��o
lisBAR     = data->LisBarra();
lisLIG     = data->LisLigacao();
lisBAR_ORD = new TList;
lisLIG_ORD = new TList;

// Copia e ordena listas de barras e liga��es
for(int n=0; n < lisBAR->Count; n++)
   {
   bar1 = (TBar *) lisBAR->Items[n];
   lisBAR_ORD->Add(bar1);
   }
for(int n=0; n < lisLIG->Count; n++)
   {
   lig = (TLig *) lisLIG->Items[n];
   lisLIG_ORD->Add(lig);
   }
lisBAR_ORD->Sort(EscolheBarraCodigo);
lisLIG_ORD->Sort(EscolheLigacao);

// Imprime cabe�alho
fprintf(arq, "\n *** Fluxo de Pot�ncia por Analogia em CC - "
             "rede '%s' - patamar %d ***", NomeRede.c_str(), (ind_pat+1));

// Imprime resultados de barras
somag = 0.;
somac = 0.;
bar2  = (TBar *) lisBAR->Last();  // Barra swing para o Fluxo CC (�nica)
fprintf(arq, "\n\n\n *** Resultados de barras ***"
             "\n\n     Barra                     Tipo          Pger (MW)"
             "     Pcarga (MW)       Pinj (MW)      �ngulo (�)\n");
for(int n=0; n < lisBAR_ORD->Count; n++)
   {
   bar1 = (TBar *) lisBAR_ORD->Items[n];
   tipo = bar1->tipo;
   if(bar1 == bar2)
      {
      str1 = "Swing";
      }
   else
      {
      str1 = "     ";
      tipo = barCARGA;  // Pode at� ser gerador, mas no Fluxo CC � carga
      }
   pg1       = bar1->p_ger * data->SbaseMVA;
   pc1       = bar1->p_car * data->SbaseMVA;
   pi1       = pg1 - pc1;
   somag    += pg1;
   somac    += pc1;
   ang_grau  = bar1->res.angulo_rad * RadGrau;
   fprintf(arq, "\n     %-20s   %5s   %1d     %12.6f    %12.6f    %12.6f    %12.6f",
           bar1->codigo.c_str(), str1.c_str(), tipo, pg1, pc1, pi1, ang_grau);
   }
fprintf(arq, "\n                                          "
             "------------    ------------"
             "\n                                          %12.6f    %12.6f",
        somag, somac);

// Imprime resultados de liga��es
fprintf(arq, "\n\n\n *** Resultados de liga��es ***"
             "\n\n     B.inic.                 B.final                     "
             "Suscep.s�rie (pu)         P (MW)\n");
for(int n=0; n < lisLIG_ORD->Count; n++)
   {
   lig  = (TLig *) lisLIG_ORD->Items[n];
   pg1  = lig->res.fluxo12_pu * data->SbaseMVA;
   bar1 = lig->bar1;
   bar2 = lig->bar2;
   if(bar1->codigo.AnsiCompareIC(bar2->codigo) > 0)
      {
      bar1 = bar2;
      bar2 = lig->bar1;
      pg1  = -pg1;
      }
   fprintf(arq, "\n     %-20s    %-20s       %14.6f        %12.6f",
           bar1->codigo.c_str(), bar2->codigo.c_str(), lig->bser, pg1);
   }

// Fecha o arquivo
fclose(arq);

// Libera mem�ria
delete lisBAR_ORD;  // Sem destruir os objetos da lista
delete lisLIG_ORD;  // Sem destruir os objetos da lista
}

//---------------------------------------------------------------------------
void __fastcall TFPcc::PreparaBarSwing(void)
   {
   //vari�veis locais
   TBar  *bar;
   TList *lisBAR = data->LisBarra();

   //verifica se existe uma Bar swing
   if ((bar = ExisteBar(lisBAR, barSWING)) != NULL)
      {//transfere Bar p/ o fim de lisBAR
      if (bar != lisBAR->Last())
         {
         lisBAR->Remove(bar);
         lisBAR->Add(bar);
         }
      }
   else
      {//assume �ltima Bar como swing
      bar = (TBar *) lisBAR->Last();
      bar->tipo = barSWING;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFPcc::PreparaIndiceBar(void)
   {
   //vari�veis locais
   TBar  *bar;
   TList *lisBAR = data->LisBarra();

   for (int n = 0; n < lisBAR->Count; n++)
      {
      bar = (TBar *) lisBAR->Items[n];
      bar->index = n;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFPcc::Relatorios(TStringList *StrList)
   {
   }

//---------------------------------------------------------------------------
//eof
