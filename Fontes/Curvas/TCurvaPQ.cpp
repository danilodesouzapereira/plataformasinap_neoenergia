//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCurvaPQ.h"
#include "..\Constante\Const.h"
//#include "..\Rede\VTFases.h"
#include "..\Rede\VTEqpto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCurva* __fastcall NewObjCurvaPQ(void)
   {
   try{
      return(new TCurvaPQ());
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TCurvaPQ::TCurvaPQ(void)
   {
   //inicia atributos
   Codigo    = "";
   Extern_id = "";
   //assume curvaPQ
   Tipo     = curvaPQ;
   //inicia Status c/ valores false
	this->StatusAsUnsigned = 0;
	this->Status[sttNOVO]  = true;
	DiaMedicao             = SEMANA_COMPLETA;
   FatorCorrecaoEnergia   = 1.0;
   //inicia Escala como de 1Hora
   Escala = pat01HORA;
   //inicia valores dos pontos
   for (int n = 0; n < 24; n++)
      {
      ponto[n].hm_ini.hora   = n;
      ponto[n].hm_ini.minuto = 0;
		ponto[n].hm_fim.hora   = n+1;
      ponto[n].hm_ini.minuto = 0;
      ponto[n].valor[0] = 0;
      ponto[n].valor[1] = 0;
      }
   }

//---------------------------------------------------------------------------
__fastcall TCurvaPQ::~TCurvaPQ(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTCurva*  __fastcall TCurvaPQ::Clone(void)
	{
	//variáveis locais
	TCurvaPQ *curva;

	//cria novo objeto CurvaPQ
	curva = new TCurvaPQ();
	//copia atributos deste objeto para nova Curva
	curva->CopiaAtributosDe(*this);

	return(curva);
	}

//---------------------------------------------------------------------------
void __fastcall TCurvaPQ::CopiaAtributosDe(VTCurva &ref)
	{
	//variáveis locais
	TCurvaPQ &curva = (TCurvaPQ&)ref;

	//copia dados
	PD = curva.PD;
	//copia Pontos da Curva
	for (int n = 0; n < 24; n++)
		{
		ponto[n] = curva.ponto[n];
		}
	}

//---------------------------------------------------------------------------
void __fastcall TCurvaPQ::ConvertePadraoAntigoHoraMinutoFinal(strHM &hm_fim)
   {
   //variáveis locais
   bool formato_antigo = (hm_fim.minuto == 59);

   //verifica se hora/minuto no formato antigo
   if (formato_antigo)
      {//corrige minuto
      if ((hm_fim.minuto += 1) == 60)
         {
         hm_fim.minuto = 0;
         hm_fim.hora++;
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TCurvaPQ::DefineHoraMinutoFinal(strHM &hm_ini, strHM &hm_fim)
   {
   //variáveis locais
   int     incr_hora   = 1;
   int     incr_minuto = 0;

   //define minuto final
   if ((hm_fim.minuto = hm_ini.minuto + incr_minuto) >= 60)
      {
      hm_fim.minuto = 0;
      incr_hora++;
      }
   //define hora final
   if ((hm_fim.hora = hm_ini.hora + incr_hora) > 24) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TCurvaPQ::DefineStatus(int bit_status, bool enabled)
   {
   if (enabled)
      {
      PD.status |= bit_status;
      }
   else
      {
      PD.status &= ~bit_status;
      }
   }

//---------------------------------------------------------------------------
double __fastcall TCurvaPQ::DesvioPadrao(int ind_val)
   {
   return(Sqrt(Variancia(ind_val)));
   }

//---------------------------------------------------------------------------
TCurvaPQ::strPONTO*  __fastcall TCurvaPQ::ExistePonto(strHM &hm)
   {
   //procura ponto que contém hora/minuto indicado
   for (int n = 0; n < 24; n++)
      {
      if ((hm.hora >= ponto[n].hm_ini.hora)&&
          (hm.hora <  ponto[n].hm_fim.hora)) return(ponto + n);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TCurvaPQ::strPONTO* __fastcall TCurvaPQ::ExistePonto(strHM &hm_ini, strHM &hm_fim)
   {
   //verifica se existe Ponto
   for (int n = 0; n < 24; n++)
      {
      if ((hm_ini.hora   == ponto[n].hm_ini.hora)&&
          (hm_ini.minuto == ponto[n].hm_ini.minuto)&&
          (hm_fim.hora   == ponto[n].hm_fim.hora)&&
          (hm_fim.minuto == ponto[n].hm_fim.minuto)) return(ponto + n);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurvaPQ::GetPeriodo(int index, strHM &hm_ini, strHM &hm_fim)
   {
   //proteção:
   if ((index < 0)||(index >= 24)) return(false);
   //determina Ponto indicado
   hm_ini = ponto[index].hm_ini;
   hm_fim = ponto[index].hm_fim;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurvaPQ::GetPonto(int index, strHM &hm_ini, strHM &hm_fim, double valor[], int dim_valor)
   {
   //proteção:
   if ((index < 0)||(index >= 24)) return(false);
   if (dim_valor < NumeroValores) return(false);
   //copia valores
   hm_ini = ponto[index].hm_ini;
   hm_fim = ponto[index].hm_fim;
   for (int nv = 0; (nv < dim_valor)&&(nv < NumeroValores); nv++)
      {valor[nv] = ponto[index].valor[nv];}
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurvaPQ::GetValor(strHM &hm, double valor[], int dim_valor)
   {
   //variáveis locais
   strPONTO *ponto;

   //proteção:
   if (dim_valor > NumeroValores) return(false);
   //determina Ponto que contém hora/minuto indicado
   if ((ponto = ExistePonto(hm)) == NULL) return(false);
   //encontrou Ponto:copia valores
	for (int nv = 0; nv < dim_valor; nv++)
      {valor[nv] = ponto->valor[nv];}
   return(true);
   }
   
//---------------------------------------------------------------------------
bool __fastcall TCurvaPQ::InserePonto(strHM &hm_ini, strHM &hm_fim, double valor[], int dim_valor)
	{
   //variáveis locais
   strPONTO *ponto;

	//proteção:
	if (dim_valor > NumeroValores) return(false);
   //determina strPONTO
	if ((ponto = ExistePonto(hm_ini)) == NULL) return(false);
   //define valores do Ponto
   for (int n = 0; n < dim_valor; n++)
      {ponto->valor[n] = valor[n];}
	return(true);
	}

//---------------------------------------------------------------------------
double __fastcall TCurvaPQ::Maximo(int ind_val)
	{
	//variáveis locais
	double  valor;
	double  maximo   = 0;
	int     intCount = sizeof(ponto) / sizeof(struct strPONTO);

	for (int n = 0; n < intCount; n++)
		{
		valor =  ponto[n].valor[ind_val];
		if      (n == 0)         maximo = valor;
		else if (valor > maximo) maximo = valor;
		}
	return(maximo);
	}

//---------------------------------------------------------------------------
double __fastcall TCurvaPQ::Integral(int ind_val)
   {
   //variáveis locais
   double  total = 0;
	int     intCount = sizeof(ponto) / sizeof(struct strPONTO);

   //soma o valor indicado de todos os pontos da curva
	for (int n = 0; n < intCount; n++)
		{
		total += ponto[n].valor[ind_val];
      }
   return(total);
   }

//---------------------------------------------------------------------------
double __fastcall TCurvaPQ::Media(int ind_val)
   {
   //variáveis locais
   int    intCount;
   double dblMedia = 0;

   //loop de pontos
   intCount = sizeof(ponto) / sizeof(struct strPONTO);
   if (intCount > 0)
      {
      for (int n = 0; n < intCount; n++)
         {
         dblMedia += ponto[n].valor[ind_val];
         }
      dblMedia /= intCount;
      }
   else
      {
      dblMedia  = 0;
      }

   return(dblMedia);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurvaPQ::MediaVarianciaDesvioPadrao(int index, double &media, double &variancia, double &desvio)
   {
   //variáveis locais
   int     intCount = 0;
   double  dblDelta = 0;

   // Calcula a média
   media     = Media(index);
   // Calcula a variância
   variancia = 0;
   intCount = sizeof(ponto) / sizeof(struct strPONTO);
   if (intCount > 0)
      {
      for (int n = 0; n < intCount; n++)
         {
         dblDelta = ponto[n].valor[index] - media;
			variancia += (dblDelta * dblDelta);
         }
      // OOPS: Variância amostral (estimador não tendencioso -> n - 1)
      variancia /= (intCount - 1);
      }
   else
      {
      variancia  = 0;
      }
   // Calcula o desvio
   desvio = Sqrt(variancia);

   return(true);
   }

//---------------------------------------------------------------------------
double __fastcall TCurvaPQ::Minimo(int ind_val)
	{
	//variáveis locais
	double  valor;
	double  minimo = 0;
	int     intCount = sizeof(ponto) / sizeof(struct strPONTO);

	for (int n = 0; n < intCount; n++)
		{
		valor =  ponto[n].valor[ind_val];
		if      (n == 0)         minimo = valor;
		else if (valor < minimo) minimo = valor;
		}
	return(minimo);
	}

//---------------------------------------------------------------------------
int __fastcall TCurvaPQ::NumeroPontos(void)
	{
   return(24);
   }

//---------------------------------------------------------------------------
void __fastcall TCurvaPQ::OrdenaPontos(void)
   {
   //nada a fazer;
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCurvaPQ::PM_GetCodigoBD(void)
   {
   return(Codigo + "@" + IntToStr(DiaMedicao));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCurvaPQ::PM_GetDiaMedicaoAsStr(void)
   {
   switch(DiaMedicao)
      {
      case DOMINGO:         return("Domingo");
      case SABADO:          return("Sábado");
      case DIA_UTIL:        return("Dia útil");
      case SEMANA_COMPLETA: return("Média");
      default:              return("indefinido");
      }
   }

//---------------------------------------------------------------------------
double __fastcall TCurvaPQ::PM_GetFatorCorrecaoEnergia(void)
   {
   return(PD.fatcor_energia);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurvaPQ::PM_GetStatus(unsigned bit)
   {
   return((PD.status & bit) == bit);
   }

//---------------------------------------------------------------------------
unsigned __fastcall TCurvaPQ::PM_GetStatusAsUnsigned(void)
   {
   return(PD.status);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCurvaPQ::PM_GetTipoAsStr(void)
   {
   return("PQ");
   }

//---------------------------------------------------------------------------
void  __fastcall TCurvaPQ::PM_SetCodigoBD(AnsiString codigo_bd)
   {
   //variáveis locais
   int index;

   if ((index = codigo_bd.AnsiPos("@")) <= 0)
      {//base antiga, onde o código não incluia o dia da medição
      Codigo     = codigo_bd;
      DiaMedicao = SEMANA_COMPLETA;
      }
   else
      {//base nova, onde o código inclui o dia da mediçãocomo o string "@<dia>"
      Codigo     = codigo_bd.SubString(1, index-1);
      DiaMedicao = codigo_bd[index+1];
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCurvaPQ::PM_SetFatorCorrecaoEnergia(double fatcor)
   {
   PD.fatcor_energia = fatcor;
   }

//---------------------------------------------------------------------------
void __fastcall TCurvaPQ::PM_SetStatus(unsigned bit, bool enabled)
   {
   if (enabled)
      {PD.status |= bit;}
   else
      {PD.status &= ~bit;}
   }

//---------------------------------------------------------------------------
void __fastcall TCurvaPQ::PM_SetStatusAsUnsigned(unsigned status)
   {
   PD.status = status;
   }

//---------------------------------------------------------------------------
void __fastcall TCurvaPQ::Reconfigura(int escala)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TCurvaPQ::Reinicia(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TCurvaPQ::RetiraPonto(int index)
	{
   //nada a fazer
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TCurvaPQ::SetPonto(strHM &hm_ini, double valor[], int dim_valor)
   {
   //variáveis locais
   strHM hm_fim;

   //proteção
   if (dim_valor > NumeroValores)return(false);
   //verifica se Hora/Minuto inicial é válido de acordo c/ o tipo de curva
   if (! ValidaHoraMinutoInicial(hm_ini)) return(false);
   //determina Hora/Minuto do final do intervalo
   if (! DefineHoraMinutoFinal(hm_ini, hm_fim)) return(false);
   //insere um novo Ponto na Curva
   return(InserePonto(hm_ini, hm_fim, valor, dim_valor));
   }

//---------------------------------------------------------------------------
bool __fastcall TCurvaPQ::SetPonto(strHM &hm_ini, strHM &hm_fim, double valor[], int dim_valor)
   {
   //proteção
   if (dim_valor > NumeroValores)return(false);
   //verifica se Hora/Minuto inicial é válido de acordo c/ o tipo de curva
   if (! ValidaHoraMinutoInicial(hm_ini)) return(false);
   //converte valores de hora/minuto final definido no padrão antigo
   ConvertePadraoAntigoHoraMinutoFinal(hm_fim);
   //verifica se Hora/Minuto final é válido de acordo c/ o tipo de curva
   if (! ValidaHoraMinutoFinal(hm_fim))
      {//CÓDIGO TEMPORÁRIO: corrige pontos definidos c/ data/hora final no padrão antigo
      if (! DefineHoraMinutoFinal(hm_ini, hm_fim)) return(false);
      }
   //insere um novo Ponto na Curva
   return(InserePonto(hm_ini, hm_fim, valor, dim_valor));
   }

//---------------------------------------------------------------------------
bool __fastcall TCurvaPQ::ValidaHoraMinutoInicial(strHM &hm_ini)
   {
   //valida limites da hora inicial: 0 <= hora <= 23
   if ((hm_ini.hora < 0)||(hm_ini.hora > 23)) return(false);
   //valida limites do minuto inicial: 0 <= minuto = 59
   if ((hm_ini.minuto < 0)||(hm_ini.minuto > 59)) return(false);
   //valida minuto inicial de acordo c/ tipo da curva
   return(hm_ini.minuto == 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurvaPQ::ValidaHoraMinutoFinal(strHM &hm_fim)
   {
   //valida limites da hora final: 0 <= hora <= 24
   if ((hm_fim.hora < 0)||(hm_fim.hora > 24)) return(false);
   //valida limites do minuto final: 0 <= minuto = 59
   if ((hm_fim.minuto < 0)||(hm_fim.minuto > 59)) return(false);
   //valida minuto inicial de acordo c/ tipo da curva
   return(hm_fim.minuto == 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurvaPQ::ValidaPeriodo24Horas(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
double __fastcall TCurvaPQ::Variancia(int ind_val)
   {
   //variáveis locais
   int    intCount;
   double dblMedia     = 0;
   double dblDesvio    = 0;
   double dblVariancia = 0;

   //calcula a média
   dblMedia = Media(ind_val);
   //loop de pontos
   intCount = sizeof(ponto) / sizeof(struct strPONTO);
   if (intCount > 0)
      {
      for (int n = 0; n < intCount; n++)
         {
         dblDesvio = ponto[n].valor[ind_val] - dblMedia;
         dblVariancia += (dblDesvio * dblDesvio);
         }
      // OOPS: Variância amostral (estimador não tendencioso -> n - 1)
      dblVariancia /= (intCount - 1);
      }
   else
      {
      dblVariancia  = 0;
      }

   return(dblVariancia);
   }

//---------------------------------------------------------------------------
//eof
