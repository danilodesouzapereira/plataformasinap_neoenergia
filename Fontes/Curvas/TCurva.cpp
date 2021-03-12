//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCurva.h"
#include "VTPonto.h"
#include "..\Constante\Const.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTFases.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCurva* __fastcall NewObjCurva(void)
   {
   try{
		return(new TCurva());
		}catch(Exception &e)
			{
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TCurva::TCurva(void)
   {
   //inicia atributos
   Codigo    = "";
   Extern_id = "";
   //assume curvaPQ
	Tipo      = curvaPQ;
   //inicia Status c/ valores false
	this->StatusAsUnsigned = 0;
	this->Status[sttNOVO]  = true;
	DiaMedicao             = SEMANA_COMPLETA;
   FatorCorrecaoEnergia   = 1.0;
   //inicia Escala como de 1Hora
   Escala = pat01HORA;
	try{//cria lista de Pontos
      lisPONTO = new TList();
      }catch(Exception &e)
			{
         }
	}

//---------------------------------------------------------------------------
__fastcall TCurva::~TCurva(void)
	{
   //destrói curva siamesa, se houver
   if (CurvaSiamesaPUmedia) {delete CurvaSiamesaPUmedia; CurvaSiamesaPUmedia = NULL;}
   //destrói lista e seus objetos
   if (lisPONTO != NULL)
      {//destrói objetos Ponto e a lista
      Reinicia();
      delete lisPONTO; lisPONTO = NULL;
      }
	}

//---------------------------------------------------------------------------
VTCurva* __fastcall TCurva::Clone(void)
	{
	//variáveis locais
	TCurva *curva;

	//cria novo objeto Curva
	curva = new TCurva();
	//copia atributos deste objeto para nova Curva
	curva->CopiaAtributosDe(*this);

	return(curva);
	}

//---------------------------------------------------------------------------
void __fastcall TCurva::CopiaAtributosDe(VTCurva &ref)
	{
	//variáveis locais
	VTPonto *ponto, *ponto_orig;
	TCurva  &curva = (TCurva&)ref;

	//copia dados
	PD = curva.PD;
	//destrói Pontos existentes
	Reinicia();
	//copia Pontos da Curva
	for (int n = 0; n < curva.lisPONTO->Count; n++)
		{
		ponto_orig = (VTPonto*)curva.lisPONTO->Items[n];
		//cria uma cópia do Ponto
		ponto = ponto_orig->Clone(NumeroValores);
		//insere Ponto na lista
		lisPONTO->Add(ponto);
		}
   //verifica se existe Curva siamesa
   if (curva.CurvaSiamesaPUmedia != NULL)
      {//copia Curva siamesa
      CurvaSiamesaPUmedia = curva.CurvaSiamesaPUmedia->Clone();
      }
	}

//---------------------------------------------------------------------------
void __fastcall TCurva::ConvertePadraoAntigoHoraMinutoFinal(strHM &hm_fim)
   {
   //variáveis locais
   bool formato_antigo = false;

   //define hora/minuto final
   switch(Escala)
      {
      case pat24HORA:   formato_antigo = (hm_fim.minuto == 59);        break;
      case pat12HORA:   formato_antigo = (hm_fim.minuto == 59);        break;
      case pat08HORA:   formato_antigo = (hm_fim.minuto == 59);        break;
      case pat06HORA:   formato_antigo = (hm_fim.minuto == 59);        break;
      case pat04HORA:   formato_antigo = (hm_fim.minuto == 59);        break;
      case pat03HORA:   formato_antigo = (hm_fim.minuto == 59);        break;
      case pat02HORA:   formato_antigo = (hm_fim.minuto == 59);        break;
      case pat01HORA:   formato_antigo = (hm_fim.minuto == 59);        break;
      case pat15MIN:    formato_antigo = ((hm_fim.minuto % 15) == 14); break;
      case pat10MIN:    formato_antigo = ((hm_fim.minuto % 10) == 9);  break;
      case pat05MIN:    formato_antigo = ((hm_fim.minuto %  5) == 4);  break;
	  case patEDITAVEL: formato_antigo = (hm_fim.minuto == 59);        break;
	  case patMMTN:     formato_antigo = (hm_fim.minuto == 59);        break;
	  case patLMP:      formato_antigo = (hm_fim.minuto == 59);        break;
	  default:          return;
      }
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
bool __fastcall TCurva::CriaPonto(strHM &hm_ini, strHM &hm_fim, double valor[], int dim_valor)
	{
   //variáeis locais
   VTPonto *ponto;

   //proteção: verifica se hora inicial e final são iguais
   if ((hm_ini.hora == hm_fim.hora)&&(hm_ini.minuto == hm_fim.minuto))
      {//não cria o Ponto
      return(false);
      }
   //cria um novo Ponto e insere na lista
	lisPONTO->Add(ponto = NewObjPonto(NumeroValores));
	ponto->HM_ini   = hm_ini;
	ponto->HM_fim   = hm_fim;
   //define valores do Ponto
   for (int n = 0; n < dim_valor; n++) {ponto->Valor[n] = valor[n];}
   return(true);
	}

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/7056570
bool __fastcall TCurva::DefineHoraMinutoFinal(strHM &hm_ini, strHM &hm_fim)
   {
   //variáveis locais
   int     incr_hora   = 0;
   int     incr_minuto = 0;
   VTPonto *ponto;
   //define hora/minuto final
   switch(Escala)
      {
      case pat24HORA: incr_hora   = 24; break;
      case pat12HORA: incr_hora   = 12; break;
      case pat08HORA: incr_hora   =  8; break;
      case pat06HORA: incr_hora   =  6; break;
      case pat04HORA: incr_hora   =  4; break;
      case pat03HORA: incr_hora   =  3; break;
      case pat02HORA: incr_hora   =  2; break;
      case pat01HORA: incr_hora   =  1; break;
      case pat15MIN:  incr_minuto = 15; break;
      case pat10MIN:  incr_minuto = 10; break;
	  case pat05MIN:  incr_minuto =  5; break;
	  case patMMTN:
	  case patLMP:
      case patEDITAVEL:
         //verifica se existe um Ponto p/ hm_ini
         if ((ponto = ExistePonto(hm_ini)) != NULL)
            {
            hm_fim = ponto->HM_fim;
            return(true);
            }
         return(false);
      default:
         return(false);
      }
   //define minuto final
   if ((hm_fim.minuto = hm_ini.minuto + incr_minuto) >= 60)
      {
      hm_fim.minuto = 0;
      incr_hora++;
      }
   //define hora final
   if ((hm_fim.hora = hm_ini.hora + incr_hora) > 24)
	  {//passou de 24 horas: tira 24
	  hm_fim.hora -= 24;
	  //caso especial: 1 patamar de 24 horas
	  if (hm_fim.hora == hm_ini.hora)
		 {
		 hm_fim.hora -= 1;
         //caso bem particular: hora de referencia 0:00
		 if(hm_fim.hora < 0) hm_fim.hora = 24;
	     }
	  if (hm_fim.hora > 24) return(false);
	  }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TCurva::DefineStatus(int bit_status, bool enabled)
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
double __fastcall TCurva::DesvioPadrao(int ind_val)
   {
   return(Sqrt(Variancia(ind_val)));
   }

//---------------------------------------------------------------------------
VTPonto* __fastcall TCurva::ExistePonto(strHM &hm)
   {
   //variáveis locais
   VTPonto *ponto;

   //procura Ponto que contém hora/minuto indicado
   for (int n = 0; n < lisPONTO->Count; n++)
      {
      ponto = (VTPonto*)lisPONTO->Items[n];
      if (ponto->ContemHM(hm)) return(ponto);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTPonto* __fastcall TCurva::ExistePonto(strHM &hm_ini, strHM &hm_fim)
   {
   //variáveis locais
   VTPonto *ponto;

   //verifica se existe Ponto
   for (int n = 0; n < lisPONTO->Count; n++)
      {
      ponto = (VTPonto*)lisPONTO->Items[n];
      if ((hm_ini.hora   == ponto->HM_ini.hora)&&
          (hm_ini.minuto == ponto->HM_ini.minuto)&&
          (hm_fim.hora   == ponto->HM_fim.hora)&&
          (hm_fim.minuto == ponto->HM_fim.minuto)) return(ponto);
      }
   return(NULL);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TCurva::GetPeriodo(int index, strHM &hm_ini, strHM &hm_fim)
   {
   //variáveis locais
   int minutos = ((24 * 60) / Escala);

   //proteção
   if (index >= Escala) index = 0;
   hm_ini.hora   = (index * minutos)/60;
   hm_ini.minuto = (index * minutos)%60;
   hm_fim.hora   = (((index+1) * minutos)-1)/60;
   hm_fim.minuto = (((index+1) * minutos)-1)%60;
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TCurva::GetPeriodo(int index, strHM &hm_ini, strHM &hm_fim)
   {
   //variáveis locais
   VTPonto *ponto;

   //proteção:
   if ((index < 0)||(index >= lisPONTO->Count)) return(false);
   //determina Ponto indicado
   ponto = (VTPonto*)lisPONTO->Items[index];
   hm_ini = ponto->HM_ini;
   hm_fim = ponto->HM_fim;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurva::GetPonto(int index, strHM &hm_ini, strHM &hm_fim, double valor[], int dim_valor)
   {
   //variáveis locais
   VTPonto *ponto;

   //proteção:
   if ((index < 0)||(index >= lisPONTO->Count))
	  return(false);
   if (dim_valor < NumeroValores)
	  return(false);
   //determina Ponto indicado
   ponto = (VTPonto*)lisPONTO->Items[index];
   //copia valores
   hm_ini = ponto->HM_ini;
   hm_fim = ponto->HM_fim;
   for (int nv = 0; (nv < dim_valor)&&(nv < NumeroValores); nv++)
      {valor[nv] = ponto->Valor[nv];}
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurva::GetValor(strHM &hm, double valor[], int dim_valor)
   {
   //variáveis locais
   VTPonto *ponto;

   //proteção:
   if (dim_valor > NumeroValores)
	  return(false);
   //determina Ponto que contém hora/minuto indicado
   if ((ponto = ExistePonto(hm)) == NULL)
	  return(false);
   //encontrou Ponto:copia valores
   for (int nv = 0; nv < dim_valor; nv++)
      {valor[nv] = ponto->Valor[nv];}
   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TCurva::InserePonto(strHM &hm_ini, strHM &hm_fim, double valor[], int dim_valor)
	{
	//variáveis locais
	VTPonto *ponto;

	//proteção:
	if (dim_valor > NumeroValores) return(false);
	try{//verifica se existe o Ponto
		if ((ponto = ExistePonto(hm_ini, hm_fim)) == NULL)
			{//proteção p/ curva do tipo editável: verifica intersecção c/ Pontos existentes
			if (Escala == patEDITAVEL)
				{//verifica se existe Ponto que contém hora/minuto inicial
				if ((ponto = ExistePonto(hm_ini)) != NULL)
					{//redefine hora/minuto final do Ponto existente c/ hora/minuto inicial do novo Ponto
					ponto->HM_fim = hm_ini;
					}
				//verifica se existe Ponto que contém hora/minuto final
				if ((ponto = ExistePonto(hm_fim)) != NULL)
					{//redefine hora/minuto inicial do Ponto existente c/ hora/minuto final do novo Ponto
					ponto->HM_ini = hm_fim;
					}
				}
			//cria um novo Ponto e insere na lista
			//lisPONTO->Add(ponto = new TPonto(dim_valor));
			lisPONTO->Add(ponto = new TPonto(NumeroValores));
			ponto->HM_ini   = hm_ini;
			ponto->HM_fim   = hm_fim;
			}
      //define valores do Ponto
		for (int n = 0; n < dim_valor; n++)
         {ponto->Valor[n] = valor[n];}
		}catch(Exception &e)
         {
			return(false);
			}
	return(true);
	}
*/

//---------------------------------------------------------------------------
bool __fastcall TCurva::InserePonto(strHM &hm_ini, strHM &hm_fim, double valor[], int dim_valor)
	{
	//variáveis locais
   strHM   tmp_hm_ini, tmp_hm_fim;
	VTPonto *ponto, *ponto1, *ponto2;

	//proteção:
	if (dim_valor > NumeroValores) return(false);
	try{//verifica se Curva é do tipo editável
		if (Escala != patEDITAVEL)
			{//verifica se existe Ponto que contém hora/minuto inicial
			if ((ponto = ExistePonto(hm_ini)) == NULL)
				{//cria um novo Ponto e insere na lista
            CriaPonto(hm_ini, hm_fim, valor, dim_valor);
            }
         else
            {//atualiza valores do ponto existente
		      for (int n = 0; n < dim_valor; n++) {ponto->Valor[n] = valor[n];}
            }
         return(true);
			}
      //curva definida com patEDITAVEL: verifica se existe um ponto com o mesmo período
      if ((ponto = ExistePonto(hm_ini, hm_fim)) != NULL)
         {//apenas redefine valores do ponto1 existente
		   for (int n = 0; n < dim_valor; n++) {ponto->Valor[n] = valor[n];}
         return(true);
         }
      //determina pontos existentes que contém horário inicial e final do novo ponto
      ponto1 = ExistePonto(hm_ini);
      ponto2 = ExistePonto(hm_fim);
      //verifica se não existe nenhum dos pontos
      if ((ponto1 == NULL)&&(ponto2 == NULL))
         {//cria um novo ponto
         CriaPonto(hm_ini, hm_fim, valor, dim_valor);
         }
      //verifica se existe apenas o ponto1
      else if ((ponto1 != NULL)&&(ponto2 == NULL))
         {//redefine hora final do ponto1
         ponto1->HM_fim = hm_ini;
         //cria um novo ponto
         CriaPonto(hm_ini, hm_fim, valor, dim_valor);
         }
      else if ((ponto1 == NULL)&&(ponto2 != NULL))
         {//redefine hora inicial do ponto2
         ponto2->HM_ini = hm_fim;
         //cria um novo ponto
         CriaPonto(hm_ini, hm_fim, valor, dim_valor);
         }
      else if ((ponto1 != NULL)&&(ponto2 != NULL))
         {//verifica se ponto1 é igual a ponto2
         if (ponto1 == ponto2)
            {//verifica se a hora inicial é menor que final
            if (hm_ini.hora < hm_fim.hora)
               {//cria uma cópia do Ponto existente e insere em lisPONTO
               lisPONTO->Add(ponto2 = ponto1->Clone(NumeroValores));
               //redefine hora final do ponto1
               ponto1->HM_fim = hm_ini;
               //redefine hora inicial do ponto2
               ponto2->HM_ini = hm_fim;
               //cria um novo ponto e insere na Curva
               //CriaPonto(hm_ini, hm_fim, valor, dim_valor);
               }
            else
               {//redefine hora inicial e final do Ponto existente
               ponto1->HM_ini = hm_fim;
               ponto1->HM_fim = hm_ini;
               //retira todos Pontos, exceto o ponto1
               RetiraPontos(tmp_hm_ini = ponto1->HM_fim, tmp_hm_fim = ponto2->HM_ini);
               //cria um novo ponto e insere na Curva
               //CriaPonto(hm_ini, hm_fim, valor, dim_valor);
               }
            }
         else
            {//retira pontos entre ponto1 e ponto2, sem inclui-los
            RetiraPontos(tmp_hm_ini = ponto1->HM_fim, tmp_hm_fim = ponto2->HM_ini);
            //redefine hora final do ponto1
            ponto1->HM_fim = hm_ini;
            //redefine hora inicial do ponto2
            ponto2->HM_ini = hm_fim;
            //cria um novo ponto e insere na Curva
            //CriaPonto(hm_ini, hm_fim, valor, dim_valor);
            }
         //cria um novo ponto e insere na Curva
         CriaPonto(hm_ini, hm_fim, valor, dim_valor);
         }
      //exlui Pontos com hora inicial e final iguais
      RetiraPontosPeriodoNulo();
      //reordena Pontos
      OrdenaPontos();
		}catch(Exception &e)
         {
			return(false);
			}
	return(true);
	}

//---------------------------------------------------------------------------
double __fastcall TCurva::Integral(int ind_val)
   {
   //variáveis locais
   VTPonto *ponto;
	double  total = 0;
	double  valor;
	double  num_horas;

   //soma o valor indicado de todos os pontos da curva
   for (int n = 0; n < lisPONTO->Count; n++)
      {
		ponto  = (VTPonto*)lisPONTO->Items[n];
		//determina valor do ponto
		valor  = ponto->Valor[ind_val];
		//determina período do ponto
		num_horas = ponto->NumHoras;
		//soma integral do ponto
		total += (valor * num_horas);
		}
   return(total);
   }

//---------------------------------------------------------------------------
double __fastcall TCurva::Maximo(int ind_val)
	{
	//variáveis locais
	VTPonto *ponto;
	double  valor;
	double  maximo = 0;

	for (int n = 0; n < lisPONTO->Count; n++)
		{
		ponto = (VTPonto*)lisPONTO->Items[n];
		valor = ponto->Valor[ind_val];
		if      (n == 0)         maximo = valor;
		else if (valor > maximo) maximo = valor;
		}
	return(maximo);
	}

//---------------------------------------------------------------------------
double __fastcall TCurva::Media(int ind_val)
   {
   //variáveis locais
   VTPonto *ponto;
   int     intCount = 0;
   double  dblMedia = 0;

   //procura Ponto que contém hora/minuto indicado
	intCount = lisPONTO->Count;
   if (intCount > 0)
      {
      for (int n = 0; n < intCount; n++)
         {
         ponto = (VTPonto*)lisPONTO->Items[n];
         dblMedia += ponto->Valor[ind_val];
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
bool __fastcall TCurva::MediaVarianciaDesvioPadrao(int ind_val, double &media, double &variancia, double &desvio)
	{
	//variáveis locais
	VTPonto *ponto;
	int     intCount = 0;
	double  dblDelta = 0;

	// Calcula a média
	media     = Media(ind_val);
   // Calcula a variância
   variancia = 0;
   //procura Ponto que contém hora/minuto indicado
   intCount = lisPONTO->Count;
   if (intCount > 0)
      {
      for (int n = 0; n < intCount; n++)
         {
         ponto = (VTPonto*)lisPONTO->Items[n];
			dblDelta = ponto->Valor[ind_val] - media;
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
double __fastcall TCurva::Minimo(int ind_val)
	{
	//variáveis locais
	VTPonto *ponto;
	double  valor;
	double  minimo = 0;

	for (int n = 0; n < lisPONTO->Count; n++)
		{
		ponto = (VTPonto*)lisPONTO->Items[n];
		valor = ponto->Valor[ind_val];
		if      (n == 0)         minimo = valor;
		else if (valor < minimo) minimo = valor;
		}
	return(minimo);
	}

//---------------------------------------------------------------------------
int __fastcall TCurva::NumeroPontos(void)
	{
   return(lisPONTO->Count);
   }

//---------------------------------------------------------------------------
void __fastcall TCurva::OrdenaPontos(void)
   {
   //variáveis locais
   AnsiString  txt;
   TStringList *StrList;
   VTPonto      *ponto;

   try{//cria um ListBox para ordenar os Pontos
      StrList = new TStringList();
      //preenche LBox c/ os Pontos
      StrList->Sorted = false;
      for (int n = 0; n < lisPONTO->Count; n++)
         {
         ponto = (VTPonto*)lisPONTO->Items[n];
         txt.sprintf("%02d%02d", ponto->HM_ini.hora, ponto->HM_ini.minuto);
         StrList->AddObject(txt, (TObject*)ponto);
         }
      //ordena os Pontos em LBox
      StrList->Sorted = true;
      //reinicia lisPONTO
      lisPONTO->Clear();
      for (int  n = 0; n < StrList->Count; n++)
         {
         lisPONTO->Add(StrList->Objects[n]);
         }
      //destrói LBox
      delete StrList;
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCurva::PM_GetCodigoBD(void)
   {
   return(Codigo + "@" + IntToStr(DiaMedicao));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCurva::PM_GetDiaMedicaoAsStr(void)
   {
   switch(DiaMedicao)
      {
      case DOMINGO:         return("Domingo");
      case SABADO:          return("Sábado");
      case DIA_UTIL:        return("Dia útil");
      case SEMANA_COMPLETA: return("Semana (média)");
      default:              return("indefinido");
      }
   }

//---------------------------------------------------------------------------
double __fastcall TCurva::PM_GetFatorCorrecaoEnergia(void)
   {
   return(PD.fatcor_energia);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurva::PM_GetStatus(unsigned bit)
   {
   return((PD.status & bit) == bit);
   }

//---------------------------------------------------------------------------
unsigned __fastcall TCurva::PM_GetStatusAsUnsigned(void)
   {
   return(PD.status);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCurva::PM_GetTipoAsStr(void)
   {
   switch(Tipo)
      {
      case curvaPQVT:    return("PQVF");
      case curvaPQ:      return("PQ");
      case curvaPQ_MDP:  return("PQ_MDP");
      case curvaPFP:     return("PFP");
      case curvaPFP_MDP: return("PFP_MDP");
      case curvaLD:      return("LD");
      }
   return("INV");
   }

//---------------------------------------------------------------------------
void  __fastcall TCurva::PM_SetCodigoBD(AnsiString codigo_bd)
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
      DiaMedicao = StrToInteger(codigo_bd.SubString(index+1, 1));
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCurva::PM_SetFatorCorrecaoEnergia(double fatcor)
   {
   PD.fatcor_energia = fatcor;
   }

//---------------------------------------------------------------------------
void __fastcall TCurva::PM_SetStatus(unsigned bit, bool enabled)
   {
   if (enabled)
      {PD.status |= bit;}
   else
      {PD.status &= ~bit;}
   }

//---------------------------------------------------------------------------
void __fastcall TCurva::PM_SetStatusAsUnsigned(unsigned status)
   {
   PD.status = status;
   }

//---------------------------------------------------------------------------
void __fastcall TCurva::Reconfigura(int escala)

   {
   //variáveis locais
   int    minutos;
   int    num_pontos;
   double *valor;
   strHM  hm_ini, hm_fim;
   VTPonto *ponto;
   TList  *lisTMP;

   //proteção p/ curva c/ patEDITAVEL (escala é igual a zero)
	if ((num_pontos = escala) == 0) num_pontos = 1;
   //cria vetor temporário p/ ler um os valores de um ponto
   valor  = new double[NumeroValores];
   //cria lista temporária p/ salvar os novos Pontos
   lisTMP = new TList();
   //cria os novos Pontos da Curva e salva em lisTMP
   minutos = (24 * 60)/num_pontos;
   for (int np = 0; np < num_pontos; np++)
      {
      hm_ini.hora   = (np * minutos)/60;
      hm_ini.minuto = (np * minutos)%60;
      hm_fim.hora   = ((np+1) * minutos)/60;
      hm_fim.minuto = ((np+1) * minutos)%60;
      //cria um novo Ponto e insere em lisTMP
      lisTMP->Add(ponto = NewObjPonto(NumeroValores));
      //obtém valores da Curva original p/ hora/minuto inicial do novo ponto
      ponto->HM_ini = hm_ini;
      ponto->HM_fim = hm_fim;
      if (GetValor(hm_ini, valor, NumeroValores))
         {//copia valores do Ponto criado
         for (int nv = 0; nv < NumeroValores; nv++)
            {
            ponto->Valor[nv] = valor[nv];
            }
         }
      else
         {//inicia valores nulos
         for (int nv = 0; nv < NumeroValores; nv++)
            {
            ponto->Valor[nv] = 0.;
            }
         }
      }
   //reinicia lisPONTO da curva
   Reinicia();
   //redefine escala
   Escala = escala;
   CopiaTList(lisTMP, lisPONTO);
   //destrói dados temporários
   delete[] valor;
   delete lisTMP;
   }

//---------------------------------------------------------------------------
void __fastcall TCurva::Reinicia(void)
   {
	//variáveis locais
	VTPonto *ponto;

	//destrói Pontos armazenados em lisPONTO
   for (int n = 0; n < lisPONTO->Count; n++)
      {
      ponto = (VTPonto*)lisPONTO->Items[n];
      delete ponto;
      }
   lisPONTO->Clear();
   }

//---------------------------------------------------------------------------
bool __fastcall TCurva::RetiraPonto(int index)
	{
	//variáveis locais
	VTPonto *ponto;

	//proteção
	if ((index < 0)||(index > lisPONTO->Count)) return(false);
	//retira o Ponto indicado
	ponto = (VTPonto*)lisPONTO->Items[index];
	lisPONTO->Delete(index);
	//destrói o objeto Ponto
	delete ponto;

	return(true);
	}

//---------------------------------------------------------------------------
void __fastcall TCurva::RetiraPontos(strHM &hm_ini, strHM &hm_fim)
   {
	//variáveis locais
	VTPonto *ponto;

   //retira todos os pontos com período dentro do intervalo definido
   while ((ponto = ExistePonto(hm_ini, hm_fim)) != NULL)
      {//retira Ponto da lista
      lisPONTO->Remove(ponto);
	   //destrói o objeto Ponto
	   delete ponto;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCurva::RetiraPontosPeriodoNulo(void)
   {
   //variáveis locais
   VTPonto      *ponto;

   for (int n = lisPONTO->Count-1; n >= 0; n--)
      {
      ponto = (VTPonto*)lisPONTO->Items[n];
      if ((ponto->HM_ini.hora == ponto->HM_fim.hora)&&(ponto->HM_ini.minuto == ponto->HM_fim.minuto))
         {//retira Ponto
         lisPONTO->Delete(n);
         delete ponto;
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TCurva::SetPonto(strHM &hm_ini, double valor[], int dim_valor)
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
bool __fastcall TCurva::SetPonto(strHM &hm_ini, strHM &hm_fim, double valor[], int dim_valor)
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
//https://sinapsisenergia.teamwork.com/index.cfm#/tasks/7056570
bool __fastcall TCurva::ValidaHoraMinutoInicial(strHM &hm_ini)
   {
   //valida limites da hora inicial: 0 <= hora <= 23
   if ((hm_ini.hora < 0)||(hm_ini.hora > 23)) return(false);
   //valida limites do minuto inicial: 0 <= minuto = 59
   if ((hm_ini.minuto < 0)||(hm_ini.minuto > 59)) return(false);
   //valida minuto inicial de acordo c/ tipo da curva
   switch(Escala)
      {
      case pat24HORA:   return(hm_ini.minuto == 0);
      case pat12HORA:   return(hm_ini.minuto == 0);
      case pat08HORA:   return(hm_ini.minuto == 0);
      case pat06HORA:   return(hm_ini.minuto == 0);
      case pat04HORA:   return(hm_ini.minuto == 0);
      case pat03HORA:   return(hm_ini.minuto == 0);
      case pat02HORA:   return(hm_ini.minuto == 0);
      case pat01HORA:   return(hm_ini.minuto == 0);
      case pat15MIN:    return((hm_ini.minuto % 15) == 0);
      case pat10MIN:    return((hm_ini.minuto % 10) == 0);
      case pat05MIN:    return((hm_ini.minuto %  5) == 0);
	  case patEDITAVEL: return(true);
	  case patMMTN:     return(hm_ini.minuto == 0);
	  case patLMP:      return(hm_ini.minuto == 0);
      default:          break;
      }
   return(false);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/index.cfm#/tasks/7056570
bool __fastcall TCurva::ValidaHoraMinutoFinal(strHM &hm_fim)
   {
   //valida limites da hora final: 0 <= hora <= 24
   if ((hm_fim.hora < 0)||(hm_fim.hora > 24)) return(false);
   //valida limites do minuto final: 0 <= minuto = 59
   if ((hm_fim.minuto < 0)||(hm_fim.minuto > 59)) return(false);
   //valida minuto inicial de acordo c/ tipo da curva
   switch(Escala)
      {
      case pat24HORA:   return(hm_fim.minuto == 0);
      case pat12HORA:   return(hm_fim.minuto == 0);
      case pat08HORA:   return(hm_fim.minuto == 0);
      case pat06HORA:   return(hm_fim.minuto == 0);
      case pat04HORA:   return(hm_fim.minuto == 0);
      case pat03HORA:   return(hm_fim.minuto == 0);
      case pat02HORA:   return(hm_fim.minuto == 0);
      case pat01HORA:   return(hm_fim.minuto == 0);
      case pat15MIN:    return((hm_fim.minuto % 15) == 0);
      case pat10MIN:    return((hm_fim.minuto % 10) == 0);
      case pat05MIN:    return((hm_fim.minuto %  5) == 0);
	  case patEDITAVEL: return(true);
	  case patMMTN:     return(hm_fim.minuto == 0);
	  case patLMP:      return(hm_fim.minuto == 0);
      default:          break;
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurva::ValidaPeriodo24Horas(void)
   {
   switch(Escala)
      {
      case pat24HORA:   return(ValidaPeriodo24HorasPatNNHORA(pat24HORA, 24));
      case pat12HORA:   return(ValidaPeriodo24HorasPatNNHORA(pat12HORA, 12));
      case pat08HORA:   return(ValidaPeriodo24HorasPatNNHORA(pat08HORA,  8));
      case pat06HORA:   return(ValidaPeriodo24HorasPatNNHORA(pat06HORA,  6));
      case pat04HORA:   return(ValidaPeriodo24HorasPatNNHORA(pat04HORA,  4));
      case pat03HORA:   return(ValidaPeriodo24HorasPatNNHORA(pat03HORA,  3));
      case pat02HORA:   return(ValidaPeriodo24HorasPatNNHORA(pat02HORA,  2));
      case pat01HORA:   return(ValidaPeriodo24HorasPatNNHORA(pat01HORA,  1));
      case pat15MIN:    return(ValidaPeriodo24HorasPat15MIN());
      case pat10MIN:    return(ValidaPeriodo24HorasPat10MIN());
      case pat05MIN:    return(ValidaPeriodo24HorasPat05MIN());
      case patEDITAVEL: return(ValidaPeriodo24HorasPatEDITAVEL());
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurva::ValidaPeriodo24HorasPatNNHORA(int escala, int num_hora_pat)
   {
   //variáveis locais
   int     index;
   strHM   hm_ini, hm_fim;
   VTPonto *ponto;

   //verifica número de pontos em lisPONTO
   if (lisPONTO->Count != escala) return(false);
   //verifica se o período de cada Ponto está correto
   index = 0;
   for (hm_ini.hora = 0; hm_ini.hora < 24; hm_ini.hora+=num_hora_pat)
      {
      hm_ini.minuto =  0;
      hm_fim.hora   = hm_ini.hora + num_hora_pat;
      hm_fim.minuto = 0;
      //determina Ponto correspondente
      ponto = (VTPonto*)lisPONTO->Items[index++];
      if (ponto->HM_ini.hora   != hm_ini.hora)   return(false);
      if (ponto->HM_ini.minuto != hm_ini.minuto) return(false);
      if (ponto->HM_fim.hora   != hm_fim.hora)   return(false);
      if (ponto->HM_fim.minuto != hm_fim.minuto) return(false);
      }
   return(true);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TCurva::ValidaPeriodo24HorasPat01HORA(void)
   {
   //variáveis locais
   int     index;
   strHM   hm_ini, hm_fim;
   TPonto *ponto;

   //verifica número de pontos em lisPONTO
   if (lisPONTO->Count != pat01HORA) return(false);
   //verifica se o período de cada Ponto está correto
   index = 0;
   for (hm_ini.hora = 0; hm_ini.hora < 24; hm_ini.hora++)
      {
      hm_ini.minuto =  0;
      hm_fim.hora   = hm_ini.hora;
      hm_fim.minuto = 59;
      //determina Ponto correspondente
      ponto = (TPonto*)lisPONTO->Items[index++];
      if (ponto->HM_ini.hora   != hm_ini.hora)   return(false);
      if (ponto->HM_ini.minuto != hm_ini.minuto) return(false);
      if (ponto->HM_fim.hora   != hm_fim.hora)   return(false);
      if (ponto->HM_fim.minuto != hm_fim.minuto) return(false);
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TCurva::ValidaPeriodo24HorasPat15MIN(void)
   {
   //variáveis locais
   int     index;
   strHM   hm_ini, hm_fim;
   VTPonto *ponto;

   //verifica número de pontos em lisPONTO
   if (lisPONTO->Count != pat15MIN) return(false);
   //verifica se o período de cada Ponto está correto
   index = 0;
   for (hm_ini.hora = 0; hm_ini.hora < 24; hm_ini.hora++)
      {
      for (hm_ini.minuto = 0; hm_ini.minuto < 60; hm_ini.minuto += 15)
         {
         hm_fim.hora   = hm_ini.hora;
         if ((hm_fim.minuto = hm_ini.minuto + 15) >= 60)
            {//incrementa hora final e zera minuto final
            hm_fim.hora++;
            hm_fim.minuto -= 60;
            }
         //determina Ponto correspondente
         ponto = (VTPonto*)lisPONTO->Items[index++];
         if (ponto->HM_ini.hora   != hm_ini.hora)   return(false);
         if (ponto->HM_ini.minuto != hm_ini.minuto) return(false);
         if (ponto->HM_fim.hora   != hm_fim.hora)   return(false);
         if (ponto->HM_fim.minuto != hm_fim.minuto) return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurva::ValidaPeriodo24HorasPat10MIN(void)
   {
   //variáveis locais
   int     index;
   strHM   hm_ini, hm_fim;
   VTPonto *ponto;

   //verifica número de pontos em lisPONTO
   if (lisPONTO->Count != pat10MIN) return(false);
   //verifica se o período de cada Ponto está correto
   index = 0;
   for (hm_ini.hora = 0; hm_ini.hora < 24; hm_ini.hora++)
      {
      for (hm_ini.minuto = 0; hm_ini.minuto < 60; hm_ini.minuto += 10)
         {
         hm_fim.hora   = hm_ini.hora;
         if ((hm_fim.minuto = hm_ini.minuto + 10) >= 60)
            {//incrementa hora final e zera minuto final
            hm_fim.hora++;
            hm_fim.minuto -= 60;
            }
         //determina Ponto correspondente
         ponto = (VTPonto*)lisPONTO->Items[index++];
         if (ponto->HM_ini.hora   != hm_ini.hora)   return(false);
         if (ponto->HM_ini.minuto != hm_ini.minuto) return(false);
         if (ponto->HM_fim.hora   != hm_fim.hora)   return(false);
         if (ponto->HM_fim.minuto != hm_fim.minuto) return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurva::ValidaPeriodo24HorasPat05MIN(void)
   {
   //variáveis locais
   int     index;
   strHM   hm_ini, hm_fim;
   VTPonto *ponto;

   //verifica número de pontos em lisPONTO
   if (lisPONTO->Count != pat05MIN) return(false);
   //verifica se o período de cada Ponto está correto
   index = 0;
   for (hm_ini.hora = 0; hm_ini.hora < 24; hm_ini.hora++)
      {
      for (hm_ini.minuto = 0; hm_ini.minuto < 60; hm_ini.minuto += 5)
         {
         hm_fim.hora   = hm_ini.hora;
         if ((hm_fim.minuto = hm_ini.minuto + 5) >= 60)
            {//incrementa hora final e zera minuto final
            hm_fim.hora++;
            hm_fim.minuto -= 60;
            }
         //determina Ponto correspondente
         ponto = (VTPonto*)lisPONTO->Items[index++];
         if (ponto->HM_ini.hora   != hm_ini.hora)   return(false);
         if (ponto->HM_ini.minuto != hm_ini.minuto) return(false);
         if (ponto->HM_fim.hora   != hm_fim.hora)   return(false);
         if (ponto->HM_fim.minuto != hm_fim.minuto) return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurva::ValidaPeriodo24HorasPatEDITAVEL(void)
   {
   //variáveis locais
   int     dh;
   VTPonto *ponto1, *ponto2;

   //proteção
   if (lisPONTO->Count == 0) return(false);
   //verifica minuto inicial e final de cada ponto
   for (int n = 0; n < lisPONTO->Count; n++)
      {
      ponto1 = (VTPonto*)lisPONTO->Items[n];
      if (ponto1->HM_ini.minuto != 0) return(false);
      if (ponto1->HM_fim.minuto != 0) return(false);
      }
   //verifica se há apenas 1 Ponto lisPONTO
   if (lisPONTO->Count == 1)
      {//verifica validade do Ponto único
      ponto1 = (VTPonto*)lisPONTO->First();
      if (ponto1->HM_ini.hora !=  0) return(false);
      if (ponto1->HM_fim.hora != 24) return(false);
      return(true);
      }
   //verifica se a somatória dos períodos dos Pontos soma 24 horas
   dh = 0;
   for (int n = 0; n < lisPONTO->Count; n++)
      {
	   ponto2 = (VTPonto*)lisPONTO->Items[n];
      if (ponto2->HM_ini.hora < ponto2->HM_fim.hora) dh += (ponto2->HM_fim.hora - ponto2->HM_ini.hora);
      //else                                           dh += 24 - (ponto2->HM_fim.hora - ponto2->HM_ini.hora);
      else                                           dh += 24 + (ponto2->HM_fim.hora - ponto2->HM_ini.hora);
      }
   return(dh == 24);
   }

//---------------------------------------------------------------------------
double __fastcall TCurva::Variancia(int ind_val)
   {
   //variáveis locais
   VTPonto *ponto;
   int     intCount     = 0;
   double  dblMedia     = 0;
   double  dblDesvio    = 0;
   double  dblVariancia = 0;

   dblMedia = Media(ind_val);
   //procura Ponto que contém hora/minuto indicado
   intCount = lisPONTO->Count;
   if (intCount > 0)
      {
      for (int n = 0; n < intCount; n++)
         {
         ponto = (VTPonto*)lisPONTO->Items[n];
         dblDesvio = ponto->Valor[ind_val] - dblMedia;
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
