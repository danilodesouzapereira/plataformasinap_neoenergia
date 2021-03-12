//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TCarga.h"
#include "VTFases.h"
#include "..\Classes\VTClasse.h"
#include "..\Curvas\VTCurva.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCarga* __fastcall NewObjCarga(int num_pat)
   {
   return(new TCarga(num_pat));
   }
   
//---------------------------------------------------------------------------
__fastcall TCarga::TCarga(int num_pat)
   {
   //valores default
   GrupoA        = false;
   VeryImportant = false;
   Energia_kwhmes = 0.;
   Fases         = faseABC;  //ligação em delta
   Reducao       = false;
   Icte_pu       = 1.0;
   Scte_pu       = 0.0;
   Zcte_pu       = 0.0;
   //número de consumidores
   NumConsResidencial = 0;
   NumConsIndustrial  = 0;
   NumConsComercial   = 0;
   NumConsRural       = 0;
   NumConsOutros      = 0;
   NumConsA4          = 0;
   //configuração gráfica
   Show[car_P] = true;
   Show[car_Q] = true;
   //info ajuste demanda
   Ajustavel = true;
   Categoria = -1;
   for (int n = indFASE_A; n < MAX_FASE; n++)
	{
	FatorAjusteP[n] = 1;
	FatorAjusteQ[n] = 1;
	}
   }

//---------------------------------------------------------------------------
__fastcall TCarga::~TCarga(void)
   {
   //destrói Curva somente se não for típica
   if ((curva != NULL)&&(! curva->Tipica)) delete curva;
   curva = NULL;
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TCarga::Clone(void)
   {
   //variáveis locais
   VTCarga *pcarga;

   //cria uma nova Carga
   if ((pcarga = NewObjCarga(curva->Escala)) != NULL)
      {//copia dados desta Carga p/ a nova Carga
		pcarga->CopiaAtributosDe(*this);
      }
   return(pcarga);
   }

//---------------------------------------------------------------------------
bool __fastcall TCarga::ConsumidorTipico(void)
   {
   return(curva->Tipica);
   }

//---------------------------------------------------------------------------
void __fastcall TCarga::CopiaAtributosDe(VTEqpto &ref)
	{
	//variáveis locais
	TCarga &carga = (TCarga&)ref;

	//proteção
	if (ref.Tipo() != eqptoCARGA) return;
	//copia atributos de TEqbar
	TEqbar::CopiaAtributosDe(carga);
	//copia atributos de VTCarga
	PD_CARGA = carga.PD_CARGA;
    AJUSTE_DEM = carga.AJUSTE_DEM;
	//define classe
	classe = carga.classe;
	//destrói Curva própria
	if ((curva)&&(! curva->Tipica)) delete curva;
   //verifica se a Carga de referência possui Curva tipica
	if (carga.ConsumidorTipico())
		{//define mesma Curva típica
		curva = carga.curva;
		}
	else
		{//cria cópia da Curva
		curva = carga.curva->Clone();
		curva->Eqpto   = this;
		//curva->Id     = FalsoId();
		//curva->Status = sttNOVO;
		}
	//JCG - 2009.09.01 - alteração p/ atender Carga associada à Obra lida da base de dado
	//copia ponteiro p/ curva de Crescimento
	//crescimento = carga.crescimento;
   if ((crescimento != NULL)&&(carga.crescimento == NULL))
		{//NÃO anula crescimento
      }
	else
      {//copia ponteiro p/ curva de Crescimento
      crescimento = carga.crescimento;
		}
	}

//---------------------------------------------------------------------------
void __fastcall TCarga::DefineClasse(VTClasse *classe)
   {
   //salva ponteiro p/ objeto
   this->classe = classe;
   switch(classe->Tag)
      {
      case consRESIDENCIAL: NumConsResidencial = 1; break;
      case consCOMERCIAL:   NumConsComercial   = 1; break;
      case consINDUSTRIAL:  NumConsIndustrial  = 1; break;
      case consRURAL:       NumConsRural       = 1; break;
      case consOUTROS:      NumConsOutros      = 1; break;
      case consA4:          NumConsA4          = 1; break;
      default: /*não altera valores*/               break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCarga::DefineCrescimento(VTCrescimento *crescimento)
   {
   //salva ponteiro p/ objeto
   this->crescimento = crescimento;
   }

//---------------------------------------------------------------------------
void __fastcall TCarga::DefineCurva(VTCurva *curva)
   {
   //proteção: verifica se a Curva indicada é a mesma que já está associada á Carga
   if (this->curva == curva) return;
   //destrói Curva própria
   if ((this->curva)&&(! this->curva->Tipica)) delete this->curva;
   //salva ponteiro p/ objeto
   this->curva = curva;
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TCarga::ExisteCurva(void)
   {
   return(curva);
   }

//---------------------------------------------------------------------------
bool __fastcall TCarga::PM_GetClasseIP(void)
	{
	//verifica se é consIP
   return (classe->Tag == consIP);
   }

//---------------------------------------------------------------------------
bool __fastcall TCarga::PM_GetDefinidaPorFase(void)
   {
   //verifica se é de uma Redução
   if (Reducao) return(true);
   //verifica se possui Curva própria, definida com nvCURVA_RRCAR
   if ((! curva->Tipica)&&(curva->NumeroValores == nvCURVA_RRCAR)) return(true);

   return(false);
   }

//---------------------------------------------------------------------------
double __fastcall TCarga::PM_GetEnergia_kwhmes(void)
   {
   return(PD_CARGA.energia_kwhmes);
   }

//---------------------------------------------------------------------------
double __fastcall TCarga::PM_GetEnergia_mwhmes(void)
   {
   return(PD_CARGA.energia_kwhmes * 0.001);
   }

//---------------------------------------------------------------------------
bool __fastcall TCarga::PM_GetEquilibrada(void)
   {
   //variáveis locais
   int       index = 0;
   int       ind_p, ind_q;
   strHM     hm_ini, hm_fim;
   double    valor[nvCURVA_RRCAR];

   //verifica se é uma Carga de redução
   if ((Reducao)||(curva->NumeroValores == nvCURVA_RRCAR))
      {//verifica se demanda nas fases são iguais
      curva->GetPonto(index, hm_ini, hm_fim, valor, sizeof(valor)/sizeof(double));
      for (int ind_fase = indFASE_A; ind_fase <= indFASE_B; ind_fase++)
         {
         ind_p = ind_fase + ind_fase;
         ind_q = ind_p+1;
         if (! IsDoubleZero(valor[ind_p] - valor[ind_p+2])) return(false);
         if (! IsDoubleZero(valor[ind_q] - valor[ind_q+2])) return(false);
         }
      return(true);
      }
   else
      {//verifica se é trifásica
      return(MBTfasica == cargaTRIFASICA);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TCarga::PM_GetGrupoA(void)
   {
   return(PD_CARGA.grupoA);
   }

//---------------------------------------------------------------------------
int __fastcall TCarga::PM_GetMBTfasica(void)
   {
   switch(Fases)
      {
      case faseA:
      case faseB:
      case faseC:    return(cargaMONOFASICA);
      case faseAN:
      case faseBN:
      case faseCN:   return(cargaMONOFASICA);
      case faseAT:
      case faseBT:
      case faseCT:   return(cargaMONOFASICA);
      case faseAB:
      case faseBC:
      case faseCA:   return(cargaBIFASICA);
      case faseABN:
      case faseBCN:
      case faseCAN:  return(cargaBIFASICA);
      case faseABC:
      case faseABCN: return(cargaTRIFASICA);
      default:       return(cargaTRIFASICA);
      }
   }

//---------------------------------------------------------------------------
int __fastcall TCarga::PM_GetModeloCarga(void)
   {
   //variáveis locais
   double mc_max;
   int    modelo;

   //assume Scte
   modelo = cargaSCTE;
   mc_max = Scte_pu;
   //verifica se modelo Icte
   if (Icte_pu > mc_max)
      {
      modelo = cargaICTE;
      mc_max = Icte_pu;
      }
   //verifica se modelo Zcte
   if (Zcte_pu > mc_max)
      {
	  modelo = cargaZCTE;
      //mc_max = Zcte_pu;
      }
   return(modelo);
   }
//---------------------------------------------------------------------------
int __fastcall TCarga::PM_GetNumConsTotal(void)
   {
   return(PD_CARGA.num_cons.res + PD_CARGA.num_cons.com + PD_CARGA.num_cons.ind +
          PD_CARGA.num_cons.rur + PD_CARGA.num_cons.out + PD_CARGA.num_cons.a4);
   }
//---------------------------------------------------------------------------
double __fastcall TCarga::PM_GetFatorAjusteP(int nf)
	{
	double valor = 1;

	if (nf < MAX_FASE)
		{
		valor = AJUSTE_DEM.fator_ajusteP[nf];
		}
	return (valor);
	}
//---------------------------------------------------------------------------
double __fastcall TCarga::PM_GetFatorAjusteQ(int nf)
	{
	double valor = 1;

	if (nf < MAX_FASE)
		{
		valor = AJUSTE_DEM.fator_ajusteQ[nf];
		}
	return (valor);
	}
//---------------------------------------------------------------------------
bool __fastcall TCarga::PM_GetVeryImportant(void)
   {
   return(PD_CARGA.vip);
   }

//---------------------------------------------------------------------------
void __fastcall TCarga::PM_SetEnergia_kwhmes(double energia_kwhmes)
   {
   PD_CARGA.energia_kwhmes = energia_kwhmes;
   }

//---------------------------------------------------------------------------
void __fastcall TCarga::PM_SetEnergia_mwhmes(double energia_mwhmes)
   {
   PD_CARGA.energia_kwhmes = energia_mwhmes * 1000.;
   }

//---------------------------------------------------------------------------
void __fastcall TCarga::PM_SetGrupoA(bool grupoA)
   {
   PD_CARGA.grupoA = grupoA;
   }
//---------------------------------------------------------------------------
void __fastcall TCarga::PM_SetFatorAjusteP(int nf, double valor)
{
	if (nf < MAX_FASE)
	{
		AJUSTE_DEM.fator_ajusteP[nf] = valor;
	}
}
//---------------------------------------------------------------------------
void __fastcall TCarga::PM_SetFatorAjusteQ(int nf, double valor)
{
	if (nf < MAX_FASE)
	{
		AJUSTE_DEM.fator_ajusteQ[nf] = valor;
	}
}
//---------------------------------------------------------------------------
void __fastcall TCarga::PM_SetVeryImportant(bool vip)
   {
   PD_CARGA.vip = vip;
   }

//---------------------------------------------------------------------------
int __fastcall TCarga::Tipo(void)
   {
   return(eqptoCARGA);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCarga::TipoAsString(void)
   {
   return("Carga");
   }

//---------------------------------------------------------------------------
//eof
