//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCrescimento.h"
#include "TPeriodo.h"
#include "..\Classes\VTClasse.h"
#include "..\Constante\Const.h"
#include "..\Rede\VTFases.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCrescimento* __fastcall NewObjCrescimento(void)
   {
   try{
      return(new TCrescimento());
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TCrescimento::TCrescimento(void)
   {
   //inicia dados
   Id = -1;
   try{//cria lista de Pontos
      lisPERIODO = new TList();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TCrescimento::~TCrescimento(void)
   {
   //variáveis locais
   TPeriodo *periodo;

   //libera memória do DynamicArray usado p/ taxa acumulada
   taxa_acum.Length = 0;
   //destrói lista e seus objetos
   if (lisPERIODO) {LimpaTList(lisPERIODO); delete lisPERIODO; lisPERIODO = NULL;}
   }

//---------------------------------------------------------------------------
VTCrescimento* __fastcall TCrescimento::Clone(void)
   {
   //variáveis locais
   TCrescimento *crescimento;
   VTPeriodo    *periodo;

   //cria um novo objeto  VTCrescimento
   if ((crescimento = (TCrescimento*)NewObjCrescimento()) != NULL)
      {//copia dados deste objeto
      crescimento->PD = PD;
      crescimento->taxa_acum.Length = taxa_acum.Length;
      //copia taxas de crescimento
      for (int n = 0; n < lisPERIODO->Count; n++)
         {
         periodo = (VTPeriodo*)lisPERIODO->Items[n];
         crescimento->InserePeriodo(periodo->AnoInicial, periodo->AnoFinal, periodo->Valor);
         }
      }
   return(crescimento);
   }

//---------------------------------------------------------------------------
TPeriodo* __fastcall TCrescimento::ExistePeriodo(int ano)
   {
   //variáveis locais
   TPeriodo *periodo;

   //procura Periodo que contém ano indicado
   for (int n = 0; n < lisPERIODO->Count; n++)
      {
      periodo = (TPeriodo*)lisPERIODO->Items[n];
      if (periodo->ContemAno(ano)) return(periodo);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TCrescimento::InserePeriodo(int ano_ini, int ano_fim, double taxa_pu)
   {
   //variáveis locais
   VTPeriodo *periodo, *periodo_ref;

   try{//verifica se existe um Período c/ o ano indicado
      if ((periodo = ExistePeriodo(ano_ini)) != NULL)
         {//atualiza valor do periodo
         periodo->Valor = taxa_pu;;
         return(true);
         }
      //cria um novo Periodo
      periodo = new TPeriodo();
      periodo->AnoInicial = ano_ini;
      periodo->AnoFinal   = ano_fim;
      periodo->Valor      = taxa_pu;
      //determina posição onde inserir em lisPERIODO
      if (ano_ini > AnoFinal)
         {//insere Periodo no fim de lisPERIODO
         lisPERIODO->Add(periodo);
         }
      else if (ano_fim < AnoInicial)
         {//insere Periodo no início de lisPERIODO
         lisPERIODO->Insert(0, periodo);
         }
      else
         {//determina posição onde inserir em lisPERIODO
         for (int index = 0; index < lisPERIODO->Count; index++)
            {
            periodo_ref = (VTPeriodo*)lisPERIODO->Items[index];
            if (ano_fim < periodo_ref->AnoInicial)
               {//insere novo Periodo na posição index
               lisPERIODO->Insert(index, periodo);
               return(true);
               }
            }
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCrescimento::LisPeriodo(void)
   {
   return(lisPERIODO);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TCrescimento::MontaTaxaAcumulada(void)
   {
   //variáveis locais
   int      ano_ini = AnoInicial;
   int      ano_fim = AnoFinal;
   double   taxa    = 1.;
   TPeriodo *periodo;

   //redefine vetor de taxa acumulada (DinamicArray)
   taxa_acum.Length = (AnoFinal - AnoInicial + 1);
   //calcula Taxa acumulada do período indicado
   for (int ano = ano_ini; ano <= ano_fim; ano++)
      {//determina Periodo que contém o ano indicado
      if ((periodo = ExistePeriodo(ano)) != NULL)
         {//calcula taxa acumulada
         taxa *= (1. + periodo->Valor);
         }
      taxa_acum[ano - ano_ini] = taxa - 1.;
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TCrescimento::MontaTaxaAcumulada(void)
   {
   //variáveis locais
   int      ano_ini = AnoInicial;
   int      ano_fim = AnoFinal;
   double   taxa    = 1.;
   TPeriodo *periodo;

   //redefine vetor de taxa acumulada (DinamicArray)
   taxa_acum.Length = (AnoFinal - AnoInicial + 1);
   //taxa acumulada do ano inicial é nula
   taxa_acum[0] = 0.;
   //calcula Taxa acumulada do período indicado
   for (int ano = ano_ini; ano < ano_fim; ano++)
      {//determina Periodo que contém o ano indicado
      if ((periodo = ExistePeriodo(ano)) != NULL)
         {//calcula taxa acumulada
         taxa *= (1. + periodo->Valor);
         }
      taxa_acum[ano - ano_ini + 1] = taxa - 1.;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TCrescimento::Nulo(void)
   {
   //verifica se a taxa é nula em todos os anos
   for (int ano = AnoInicial; ano <= AnoFinal; ano++)
      {
      if (Taxa_pu[ano] != 0.) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
int __fastcall TCrescimento::PM_GetAnoFinal(void)
   {
   //variáveis locais
   TPeriodo *periodo;

   //proteção
   if (lisPERIODO->Count == 0) return(0);
   //retorna ano final do último período
   periodo = (TPeriodo*)lisPERIODO->Last();
   return(periodo->AnoFinal);
   }

//---------------------------------------------------------------------------
int __fastcall TCrescimento::PM_GetAnoInicial(void)
   {
   //variáveis locais
   TPeriodo *periodo;

   //proteção
   if (lisPERIODO->Count == 0) return(0);
   //retorna ano inicial do primeiro período
   periodo = (TPeriodo*)lisPERIODO->First();
   return(periodo->AnoInicial);
   }

//---------------------------------------------------------------------------
VTClasse* __fastcall TCrescimento::PM_GetClasse(void)
   {
   return(PD.classe);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCrescimento::PM_GetCodigo(void)
   {
//   return(Classe->Codigo);
   return(codigo);
   }

//---------------------------------------------------------------------------
int __fastcall TCrescimento::PM_GetNumPeriodo(void)
   {
   return(lisPERIODO->Count);
   }

//---------------------------------------------------------------------------
double __fastcall TCrescimento::PM_GetTaxa_perc(int ano)
   {
   return(100. * Taxa_pu[ano]);
   }

//---------------------------------------------------------------------------
double __fastcall TCrescimento::PM_GetTaxa_pu(int ano)
   {
   //variáveis locais
   double   taxa = 0.;
   TPeriodo *periodo;

   //determina Periodo que contém o ano indicado
   if ((periodo = ExistePeriodo(ano)) != NULL) taxa = periodo->Valor;
   return(taxa);
   }

//---------------------------------------------------------------------------
double __fastcall TCrescimento::PM_GetTaxaAcumulada_pu(int index)
   {
   //proteção: verifica se as taxas acumuladas já foram calculadas
   if (taxa_acum.Length == 0)  MontaTaxaAcumulada();
	//proteção
   if (index < 0) return(0.);
   if (index >= taxa_acum.Length) index = taxa_acum.Length - 1;
   //retorna taxa acumulada
   return(taxa_acum[index]);
   }

//---------------------------------------------------------------------------
void __fastcall TCrescimento::PM_SetClasse(VTClasse *classe)
   {
   codigo = classe->Codigo;
   PD.classe = classe;
   }

//---------------------------------------------------------------------------
void __fastcall TCrescimento::PM_SetTaxa_pu(int ano, double taxa)
   {
   //variáveis locais
   TPeriodo *periodo;

   //determina Periodo que contém o ano indicado
	if ((periodo = ExistePeriodo(ano)) != NULL)
		{//salva taxa
		periodo->Valor = taxa;
		//cancela valores de taxa acumulada
		if (taxa_acum.Length > 0) {taxa_acum.Length = 0;}
		}
	}

//---------------------------------------------------------------------------
void __fastcall TCrescimento::PM_SetTaxa_perc(int ano, double taxa)
   {
   Taxa_pu[ano] = taxa / 100.;
   }

//---------------------------------------------------------------------------
void __fastcall TCrescimento::RedefinePeriodo(int ano_ini, int ano_fim)
   {
   //variáveis locais
   VTPeriodo *periodo;

   //elimina Periodos fora do novo intervalo
   for (int n = lisPERIODO->Count - 1; n >= 0; n--)
      {
      periodo = (VTPeriodo*)lisPERIODO->Items[n];
      if ((ano_fim < periodo->AnoInicial)||(ano_ini > periodo->AnoFinal))
         {//elimina o Periodo
         lisPERIODO->Delete(n);
         }
      }
   //loop p todos anos
   for (int ano = ano_ini; ano <= ano_fim; ano++)
      {//verifica se existe o Periodo p/ o ano
      if ((periodo = ExistePeriodo(ano)) != NULL) continue;
      //insere um novo Periodo com taxa nula
      InserePeriodo(ano, ano, 0.);
      }
	//cancela valores de taxa acumulada
   if (taxa_acum.Length > 0) {taxa_acum.Length = 0;}
   }

//---------------------------------------------------------------------------
void __fastcall TCrescimento::Reinicia(void)
   {
   //reinicia lisPERIODO
   LimpaTList(lisPERIODO);
	//cancela valores de taxa acumulada
   if (taxa_acum.Length > 0) {taxa_acum.Length = 0;}
   }


//---------------------------------------------------------------------------
//eof
