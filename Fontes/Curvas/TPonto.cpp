//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TPonto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTPonto* __fastcall NewObjPonto(int num_val)
   {
   switch(num_val)
      {
      case  1: return NewObjPonto1();
      case  2: return NewObjPonto2();
      case  4: return NewObjPonto4();
      case  8: return NewObjPonto8();
      case 16: return NewObjPonto16();
      default: return NewObjPontoG(num_val);
      }
   }
/*
//---------------------------------------------------------------------------
VTPonto* __fastcall NewVetObjPonto(int num_pto, int num_val)
   {
   switch(num_val)
      {
      case  1: return NewVetObjPonto1(num_pto);
      case  2: return NewVetObjPonto2(num_pto);
      case  4: return NewVetObjPonto4(num_pto);
      case  8: return NewVetObjPonto8(num_pto);
      case 16: return NewVetObjPonto16(num_pto);
      default: return NULL;
      }
   }
*/
//---------------------------------------------------------------------------
__fastcall TPonto::TPonto(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TPonto::~TPonto(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TPonto::ContemHM(strHM &hm)
   {
   //variáveis locais
   int min     = (60 * hm.hora) + hm.minuto;
   int min_ini = (60 * HM_ini.hora) + HM_ini.minuto;
   int min_fim = (60 * HM_fim.hora) + HM_fim.minuto;

   //verifica se intervalo inclui a virada do dia
	if (min_ini < min_fim)
      {//intervalo NÃO INCLUI virada do dia
      return((min >= min_ini)&&(min < min_fim));
      }
   else
      {//intervalo INCLUI virada do dia
		return((min < min_fim)||(min >= min_ini));
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TPonto::ContemPeriodo(VTPonto *ponto)
   {
   //variáveis locais
   strHM hm_ini = ponto->HM_ini;
   strHM hm_fim = ponto->HM_fim;
   return(ContemHM(hm_ini) && ContemHM(hm_fim));
   }

//---------------------------------------------------------------------------
double __fastcall TPonto::PM_GetNumHoras(void)
	{
	//variáveis locais
	int    min_ini, min_fim;
	double num_horas;

	min_ini = (60 * PD.hm_ini.hora) + PD.hm_ini.minuto;
	min_fim = (60 * PD.hm_fim.hora) + PD.hm_fim.minuto;
	//verifica se hm_fim é posterior a hm_ini
	if (min_fim >= min_ini)
		{
		num_horas = double(min_fim - min_ini) / 60.;
		}
	else
		{
		num_horas = double(((24 * 60) - min_ini) + min_fim) / 60.;
		}
	return(num_horas);
	}

//---------------------------------------------------------------------------
double __fastcall TPonto::PM_GetValor(int index)
	{
   //retorna valor
   return(PD.valor[index]);
   }

//---------------------------------------------------------------------------
bool __fastcall TPonto::PM_SetValor(int index, double val)
   {
   //atualiza valor
   PD.valor[index] = val;
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TPonto::Reinicia(int len)
   {
   for (int n = 0; n < len; n++) Valor[n] = 0.;
   }

//---------------------------------------------------------------------------
//eof
