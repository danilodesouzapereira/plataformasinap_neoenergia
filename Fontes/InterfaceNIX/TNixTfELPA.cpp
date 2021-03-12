//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TNixTfELPA.h"
#include "VTInterfaceNIX.h"   //FKM 2016.06.01
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Complexo\Complexo.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Radial\VTSecundario.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTTrafo.h"
#include "..\Patamar\VTPatamares.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
// função global p/ criar objeto da classe
// ---------------------------------------------------------------------------
VTNixTf* __fastcall NewObjNixTfElpa(VTApl *apl, VTCarga *carga, strCFGINIX cfg)
{
	return (new TNixTfELPA(apl, carga, cfg));
}
// ---------------------------------------------------------------------------
VTNixTf* __fastcall NewObjNixTfElpa(VTApl *apl, VTSecundario *secundario, strCFGINIX cfg)
{
	return (new TNixTfELPA(apl, secundario, cfg));
}
// ---------------------------------------------------------------------------
VTNixTf* __fastcall NewObjNixTfElpa(VTApl *apl, TList *lisTRFParalelo, strCFGINIX cfg)
{
	return (new TNixTfELPA(apl, lisTRFParalelo, cfg));
}

//---------------------------------------------------------------------------
__fastcall TNixTfELPA::TNixTfELPA(VTApl *apl, VTCarga *carga, strCFGINIX cfg): TNixTf()
   {
   //variávies locais
   int num_ip;

   infoET.numTrafos = 1;
   this->cfg = cfg;
   //salva ponteiro para objeto
   demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //this->apl = apl;
   //inicializa o vetor de complexos
   smva.size = 0;
   //não cria listas
   lisCAR = NULL;
   lisTRF = NULL;
   //inicia dados
   strTF[0].tf           = "TF";
   strTF[0].numero_no    = carga->pbarra->Id;
   strTF[0].codigo       = carga->Codigo;
   strTF[0].potencia     = TNixTf::PotenciaInstalada_kva(carga);
   strTF[0].fp           = TNixTf::FatorPotencia(carga);
   //strTF.fd           = FatorDemanda(carga);
   strTF[0].tipo         = 2;
   strTF[0].proprietario = 1;
   strTF[0].fases        = 3;
//   strTF.residenciais = 0;
//   strTF.comerciais   = 0;
//   strTF.industriais  = 1;
//   strTF.rurais       = 0;
//   strTF.outros       = 0;
   //caso seja uma carga equivalente
   if (carga->classe->Tag == consEQUIVALENTE)
      {
	  strTF[0].residenciais = carga->NumConsResidencial;
	  strTF[0].comerciais   = carga->NumConsComercial;
	  strTF[0].industriais  = carga->NumConsIndustrial;
	  strTF[0].rurais       = carga->NumConsRural;
	  strTF[0].outros       = carga->NumConsOutros;
      }
   else
      {
	  strTF[0].residenciais = 0;
	  strTF[0].comerciais   = 0;
	  strTF[0].industriais  = 1;
	  strTF[0].rurais       = 0;
	  strTF[0].outros       = 0;
      }
   //FD de acordo com configuracao
   if (cfg.fd_unico_a4)
	  {strTF[0].fd  = cfg.fdTrafo;}
   else
	  {strTF[0].fd = FatorDemanda(carga);}
   }

//---------------------------------------------------------------------------
__fastcall TNixTfELPA::TNixTfELPA(VTApl *apl, VTSecundario *secundario, strCFGINIX cfg):TNixTf()
   {
   //variávies locais
   int num_ip;
   infoET.numTrafos = 1;
   this->cfg = cfg;

   //salva ponteiro para objeto
   demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //inicializa o vetor de complexos
   smva.size = 0;
   //cria objetos
   lisCAR = new TList();
   lisTRF = new TList();
   //obtém lista de Trafos e de Cargas do Secundario
   secundario->Rede->LisEqbar(lisCAR, eqptoCARGA);
   secundario->Rede->LisLigacao(lisTRF, eqptoTRAFO);
   //inicia dados
   strTF[0].tf           = "TF";
   strTF[0].numero_no    = secundario->BarraInicial->Id;
   strTF[0].codigo       = secundario->Rede->Codigo;
   strTF[0].potencia     = TNixTf::PotenciaInstalada_kva();
   strTF[0].fp           = TNixTf::FatorPotencia();
   strTF[0].tipo         = 0;
   strTF[0].proprietario = 0;
   strTF[0].fases        = TNixTf::FasesPrimario();
   strTF[0].residenciais = NumeroDeCarga(consRESIDENCIAL);
   strTF[0].comerciais   = NumeroDeCarga(consCOMERCIAL);
   strTF[0].industriais  = NumeroDeCarga(consINDUSTRIAL);
   strTF[0].rurais       = NumeroDeCarga(consRURAL);
   strTF[0].outros       = NumeroDeCarga(consOUTROS);
   //redefine tipo da rede em função dos consumidores
   num_ip = NumeroDeCarga(consIP);
   if      ((strTF[0].residenciais + strTF[0].comerciais + strTF[0].outros) > 0) strTF[0].tipo = 0;
   else if (strTF[0].industriais > 0)                                   strTF[0].tipo = 2;
   else if (strTF[0].rurais > 0)                                        strTF[0].tipo = 1;
   else if (num_ip > 0)                                                 strTF[0].tipo = 3;
   else                                                                 strTF[0].tipo = 0;
   //FD de acordo com configuracao
   if (cfg.fd_unico_trafo)
	  {strTF[0].fd  = cfg.fdTrafo;}
   else
      {strTF[0].fd = FatorDemanda();}
   }
 //---------------------------------------------------------------------------
__fastcall TNixTfELPA::TNixTfELPA(VTApl *apl, TList *lisTRFParalelo, strCFGINIX cfg):TNixTf()
   {//variávies locais
   int num_ip;
   VTTrafo *trafo;
   //salva ponteiro para objeto
   demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int tipoLig =0;
   this->cfg = cfg;

   //inicializa o vetor de complexos
   smva.size = 0;
   //cria objetos
   lisCAR = new TList();
   lisTRF = new TList();
   CopiaTList(lisTRFParalelo,lisTRF);
   //protecao contra lista vazia
   if((infoET.numTrafos = lisTRFParalelo->Count) <= 0){return;}
   //Obtem a lista de cargas
   LisCargasBancoTrafo(lisTRFParalelo,lisCAR);

   //verifica o tipo de ligacao
   tipoLig = TipoET(lisTRFParalelo);
   //preenche a informacao da ET
   InfoET(lisTRFParalelo);

	//o nome é o nome da rede
	for (int n = 0; n < lisTRFParalelo->Count; n++)
	{
		//pega um trafo da lista
		trafo = (VTTrafo*) lisTRFParalelo->Items[n];
		//inicia dados
		strTF[n].tf           = "TF";
		strTF[n].numero_no    = trafo->BarraPrimario()->Id;
		strTF[n].codigo 	  = trafo->Codigo;
		//calcula a potencia do trafo de acordo com a regra da elpa
		strTF[n].potencia     = PotenciaInstalada_kva(trafo,tipoLig);
		strTF[n].fp           = FatorPotencia(trafo);
		//strTF.fd           = FatorDemanda();
		strTF[n].tipo         = 0;
		strTF[n].proprietario = 0;
		strTF[n].fases        = FasesPrimario(trafo);
		strTF[n].residenciais = 0;
		strTF[n].comerciais   = 0;
		strTF[n].industriais  = 0;
		strTF[n].rurais       = 0;
		strTF[n].outros       = 0;
		//FD de acordo com configuracao
		if (cfg.fd_unico_trafo)
		  {strTF[n].fd  = cfg.fdTrafo;}
		else
		  {strTF[n].fd = FatorDemanda(trafo,tipoLig);}
	}

   strTF[0].residenciais = NumeroDeCarga(consRESIDENCIAL);
   strTF[0].comerciais   = NumeroDeCarga(consCOMERCIAL);
   strTF[0].industriais  = NumeroDeCarga(consINDUSTRIAL);
   strTF[0].rurais       = NumeroDeCarga(consRURAL);
   strTF[0].outros       = NumeroDeCarga(consOUTROS);
   //redefine tipo da rede em função dos consumidores
   num_ip = NumeroDeCarga(consIP);
   if      ((strTF[0].residenciais + strTF[0].comerciais + strTF[0].outros) > 0) strTF[0].tipo = 0;
   else if (strTF[0].industriais > 0)                                      	strTF[0].tipo = 2;
   else if (strTF[0].rurais > 0)                                           	strTF[0].tipo = 1;
   else if (num_ip > 0)                                                 	strTF[0].tipo = 3;
   else                                                                		strTF[0].tipo = 0;

   }

//---------------------------------------------------------------------------
__fastcall TNixTfELPA::~TNixTfELPA(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisCAR) {delete lisCAR; lisCAR = NULL;}
   if (lisTRF) {delete lisTRF; lisTRF = NULL;}
   }

////---------------------------------------------------------------------------
//bool __fastcall TNixTfELPA::AlocaVectorSmva(int numpat)
//   {
//   //verifica se o vetor de complexos alocado tem tamanho suficiente
//   if (smva.size < numpat)
//      {//verifica se havia um vetor alocado
//      if (smva.size > 0)
//         {//libera vetor alocado
//         delete[] smva.complex;
//         delete[] smva.P;
//         }
//      //aloca vetor do tamanho indicado
//      smva.size = numpat;
//      smva.complex = new complex<double>[numpat];
//      smva.P = new double[numpat];
//      }
//   return(smva.size >= numpat);
//   }
////---------------------------------------------------------------------------
//void __fastcall TNixTfELPA::ClearVetorSmva(void)
//{
//   for (int i = 0; i < smva.size; i++)
//	  {
//	  smva.complex[i] = CZero();
//	  smva.P[i] = 0.0;
//	  }
//}
//---------------------------------------------------------------------------
AnsiString __fastcall TNixTfELPA::LinhaArqCsv(void)
   {
   //variáveis locais
   //char       separador = ';';
   AnsiString       separador = ";";
   AnsiString txt;

   txt  = strTF[0].tf                             + separador;
   txt += IntToStr(strTF[0].numero_no)            + separador;
   txt += strTF[0].codigo                         + separador;
   txt += DoubleToStr("%2.1f", strTF[0].potencia) + separador;
   //FKM 2016.05.10 - aumentei a precisao devido a alguns trafos
   txt += DoubleToStr("%4.3f", strTF[0].fp)       + separador;
   txt += DoubleToStr("%4.3f", strTF[0].fd)       + separador;
   txt += IntToStr(strTF[0].tipo        )         + separador;
   txt += IntToStr(strTF[0].proprietario)         + separador;
   txt += IntToStr(strTF[0].fases       )         + separador;
   txt += IntToStr(strTF[0].residenciais)         + separador;
   txt += IntToStr(strTF[0].comerciais  )         + separador;
   txt += IntToStr(strTF[0].industriais )         + separador;
   txt += IntToStr(strTF[0].rurais      )         + separador;
   txt += IntToStr(strTF[0].outros      )         + separador;

   //faz as outras linhas se necessario
   for (int n = 1; n < infoET.numTrafos; n++)
   {
		txt += "\n";
		txt += strTF[n].tf                             + separador;
	   txt += IntToStr(strTF[n].numero_no)            + separador;
	   txt += strTF[n].codigo                         + separador;
	   txt += DoubleToStr("%2.1f", strTF[n].potencia) + separador;
	   //FKM 2016.05.10 - aumentei a precisao devido a alguns trafos
	   txt += DoubleToStr("%4.3f", strTF[n].fp)       + separador;
	   txt += DoubleToStr("%4.3f", strTF[n].fd)       + separador;
	   txt += IntToStr(strTF[n].tipo        )         + separador;
	   txt += IntToStr(strTF[n].proprietario)         + separador;
	   txt += IntToStr(strTF[n].fases       )         + separador;
	   txt += IntToStr(strTF[n].residenciais)         + separador;
	   txt += IntToStr(strTF[n].comerciais  )         + separador;
	   txt += IntToStr(strTF[n].industriais )         + separador;
	   txt += IntToStr(strTF[n].rurais      )         + separador;
	   txt += IntToStr(strTF[n].outros      )         + separador;
   }
   return(txt);
   }
////---------------------------------------------------------------------------
//void __fastcall TNixTfELPA::LisTrafoParalelo(VTTrafo *trafo, TList *lisEXT)
//   {//variaveis locais
//   VTRede *rede;
//   VTBarra *barra1, *barra2, *barra3, *barraPri, *barraSec;
//   VTLigacao *ligacao;
//   TList *lisLIG = new TList;
//
//   rede = trafo->rede;
//   //lista todas ligacoes daquela rede naquela barra
//   barraPri = trafo->BarraPrimario();
//   barraSec = trafo->BarraSecundario();
//   rede->LisLigacao(lisLIG,barraPri);
//   //percorre a lista de ligacoes
//   for(int nl = 0; nl < lisLIG->Count; nl++)
//      {
//      ligacao = (VTLigacao*)lisLIG->Items[nl];
//      barra1 = ligacao->pbarra1;
//      barra2 = ligacao->pbarra2;
//      barra3 = ligacao->pbarra3;
//      //verifica se alguma barra é igual a barra do primario
//      if((barraPri == barra1) || (barraPri == barra2) || (barraPri == barra3))
//         {//verifica se alguma barra é igual a barra do secundario
//         if((barraSec == barra1) || (barraSec == barra2) || (barraSec == barra3))
//            {//se a ligacao for um trafo
//            if(ligacao->Tipo() == eqptoTRAFO)
//               {//adiciona o trafo À lista
//               if(lisEXT->IndexOf(ligacao) < 0)
//                  {lisEXT->Add(ligacao);}
//               }
//            }
//         }
//      }
//   if(lisLIG){delete lisLIG; lisLIG = NULL;}
//   }
//---------------------------------------------------------------------------
int __fastcall TNixTfELPA::FasesPrimario(VTTrafo *trafo)
   {
   //variáveis locais
   int      fases = 0;


   fases = trafo->pri.fase;

   //retorna fases no padrão NIX
   return(TraduzFases(fases));
   }

////---------------------------------------------------------------------------
//double __fastcall TNixTfELPA::FatorDemanda(void)
//   {
//   //variáveis locais
//   double          s_med, s_max, fatdem;
//   complex<double> s_med_mva = CZero();
//   complex<double> s_max_mva = CZero();
//   VTTrafo         *trafo;
//
//   //loop p/ todoas Trafos
//   for (int n = 0; n < lisTRF->Count; n++)
//      {
//      trafo = (VTTrafo*)lisTRF->Items[n];
//      //totaliza demanda média e máxima
//      s_med_mva += trafo->resflow->S_mva_med_allpat();;
//      s_max_mva += trafo->resflow->S_mva_max_allpat();;
//      }
//   s_med = Abs(s_med_mva);
//   s_max = Abs(s_max_mva);
//   //proteção
//   if (IsDoubleZero(s_max)) fatdem = 1.0;
//   else                     fatdem = s_med/s_max;
////   if(fatdem > 1)
////   {
////      int a =0;
////   }
//
//   return(fatdem);
//   }
//---------------------------------------------------------------------------
double __fastcall TNixTfELPA::FatorDemanda(void)
   {
   //variáveis locais
   double          fp = 0.92;
   double          fatdem;
   double          P_max_mw = 0;
   VTTrafo         *trafo;
   VTCarga         *carga;
   int             max_pat = patamares->NumPatamar();

   if (!IsDoubleZero(strTF[0].fp))   fp = strTF[0].fp;
   //se necessário, aloca vetor  para armazenas os valores da curva de S e P
   if (! AlocaVectorSmva(max_pat)) return -1.0;
   //inicaliza o vetor
   ClearVetorSmva();
   //loop p/ todos Trafos
   for (int n = 0; n < lisTRF->Count; n++)
      {
      trafo = (VTTrafo*)lisTRF->Items[n];
      //percorre todos patamares
      for(int np = 0; np < max_pat; np++)
         {
         smva.P[np] += fabs(trafo->resflow->P_mw[0][np]);
         }
      }
   //verifica qual e o máximo P
   for (int i = 0; i < smva.size; i++)
      {
      if(smva.P[i] > P_max_mw) P_max_mw = smva.P[i];
      }
   //caso não tenha rodado o fluxo...
   if(IsDoubleZero(P_max_mw))
      {//inicaliza o vetor
      ClearVetorSmva();
      //percorre todas cargas e monta uma curva da soma das cargas
      for(int nc = 0; nc < lisCAR->Count; nc++)
         {
         carga = (VTCarga*)lisCAR->Items[nc];
         //percorre todos patamares
         for(int np = 0; np < max_pat; np++)
            {
            smva.P[np] += demanda->P_mw[carga][np];
            }
         }
         //verifica qual e o máximo P
      for (int i = 0; i < smva.size; i++)
         {
         if(smva.P[i] > P_max_mw) P_max_mw = smva.P[i];
         }
      }
   //proteção
   if (IsDoubleZero(strTF[0].potencia))
      {fatdem = 1.0;}
   else
      {fatdem = (P_max_mw*1000)/(strTF[0].potencia*fp);}
   //ultima verificação
   if (IsDoubleZero(fatdem))
      {fatdem = 1.0;}
   return(fatdem);
   }
//---------------------------------------------------------------------------
//double __fastcall TNixTfELPA::FatorDemanda(VTCarga *carga)
//   {
//   //variáveis locais
//   double fatdem = 1.0;
//   double s_max_mva, p_max_mw, q_max_mvar;
//   double s_med_mva, p_med_mw, q_med_mvar;
//
//   //determina demanda máxima
//   if (demanda->GetSmax_mva(carga, s_max_mva, p_max_mw, q_max_mvar))
//      {//determina demanda média
//      if (demanda->GetSmedia_mva(carga, s_med_mva, p_med_mw, q_med_mvar))
//         {
//         if (IsDoubleZero(s_max_mva)) fatdem = 1.0;
//         else                         fatdem = s_med_mva/s_max_mva;
//         }
//      }
//
//   return(fatdem);
//   }
 //---------------------------------------------------------------------------
double __fastcall TNixTfELPA::FatorDemanda(VTCarga *carga)
   {
   //variáveis locais
   double fp     = 0.92;
   double fatdem = 1.0;
   double s_max_mva, p_max_mw, q_max_mvar;


   if (!IsDoubleZero(strTF[0].fp))   fp = strTF[0].fp;
   //determina demanda máxima
   if (demanda->GetSmax_mva(carga, s_max_mva, p_max_mw, q_max_mvar))
      {
      if (IsDoubleZero(strTF[0].potencia))
         {fatdem = 1.0;}
      else
         {fatdem = (p_max_mw*1000)/(strTF[0].potencia*fp);}
      }
   //ultima verificação
   if (IsDoubleZero(fatdem))
      {fatdem = 1.0;}
   return(fatdem);
   }
//---------------------------------------------------------------------------
double __fastcall TNixTfELPA::FatorDemanda(VTTrafo *trafo, int tipoLig)
{
	double valor = 0;

	//age de acordo com a configuracao
	if (cfg.carregDiurno)
	{
		valor = FatorDemandaDiurno(trafo,tipoLig);
	}
	else
	{
		valor = FatorDemandaNoturno(trafo,tipoLig);
	}

	return valor;
}
//---------------------------------------------------------------------------
double __fastcall TNixTfELPA::FatorDemandaDiurno(VTTrafo *trafo, int tipoLig)
{//variáveis locais
	double   fd = 0.;

	//1o verifica se é de força ou luz
	if (trafo->Codigo.Pos("luz") > 0)
	{
		switch (tipoLig)
		{
			case tetMONO:
				fd = 0.2;
				break;
			case tetTRIF:
				fd = 1.0;
				break;
			case tetDELTAABERTO:
			case tetDELTAFECHADO:
				fd = 1.0;
				break;
			default:
				fd = 1.0;
			;
		}
	}
	else if (trafo->Codigo.Pos("força") > 0)
	{
		fd = 1.0;
	}

	return fd;
}
//---------------------------------------------------------------------------
double __fastcall TNixTfELPA::FatorDemandaNoturno(VTTrafo *trafo, int tipoLig)
{	//variáveis locais
	double   fd = 0.;

	//1o verifica se é de força ou luz
	if (trafo->Codigo.Pos("luz") > 0)
	{
		fd = 1.0;
	}
	else if (trafo->Codigo.Pos("força") > 0)
	{
		switch (tipoLig)
		{
			case tetDELTAABERTO:
				fd = 0;
				break;
			case tetDELTAFECHADO:
				fd = 0.5;
				break;
			default:
				fd = 1.0;
			;
		}
	}
	else
	{
		fd = 1.0;
    }

	return fd;
}
//---------------------------------------------------------------------------
double __fastcall TNixTfELPA::FatorPotencia(VTTrafo *trafo)
   {
   //variáveis locais
   double          p_mw, mod_s_mva, fatpot = 0.92;
   complex<double> s_mva = CZero();

   //loop p/ todoas Trafos
   s_mva += trafo->resflow->S_mva_max_allpat();
   //calcula fatpot
   p_mw      = s_mva.real();
   mod_s_mva = Abs(s_mva);
   if      (IsDoubleZero(p_mw))      fatpot = 0.92;
   else if (IsDoubleZero(mod_s_mva)) fatpot = 0.92;
   else                              fatpot = p_mw / mod_s_mva;

   return(fatpot);
   }
//---------------------------------------------------------------------------
void __fastcall TNixTfELPA::InfoET(TList *lisTRAFOS)
{
	VTTrafo *trafo;
	int tipoET = 0;
	double   s_mva = 0.;

	//protecao contra lista vazia
	if (lisTRAFOS == NULL)
	{
		return;
	}
	if (lisTRAFOS->Count == 0)
	{
		return;
	}
	//percorre a lista de trafos
	infoET.numTrafos = lisTRAFOS->Count;
	tipoET = TipoET(lisTRAFOS);
	infoET.potLuz_kva = 0;
    infoET.potForca_kva = 0;
	if (tipoET != tetTRIF)
	{
    	for (int n = 0; n < lisTRAFOS->Count; n++)
		{
			trafo = (VTTrafo*) lisTRAFOS->Items[n];
			s_mva = trafo->snom;
			//1o verifica se é de força ou luz
			if (trafo->Codigo.Pos("luz") > 0)
			{
            	infoET.potLuz_kva = s_mva*1000;
			}
			else if (trafo->Codigo.Pos("força") > 0)
			{
				infoET.potForca_kva = s_mva*1000;
			}
		}
	}
}
//---------------------------------------------------------------------------
double __fastcall TNixTfELPA::PotenciaInstalada_kva(VTTrafo *trafo, int tipoLig)
{
	double valor = 0;

	//age de acordo com a configuracao
	if (cfg.carregDiurno)
	{
		valor = PotenciaInstaladaDiurno_kva(trafo,tipoLig);
	}
	else
	{
		valor = PotenciaInstaladaNoturno_kva(trafo,tipoLig);
	}

	return valor;
}
//---------------------------------------------------------------------------
double __fastcall TNixTfELPA::PotenciaInstaladaDiurno_kva(VTTrafo *trafo, int tipoLig)
{//variáveis locais
	double   s_kva = 0.;

	//1o verifica se é de força ou luz
	if (trafo->Codigo.Pos("luz") > 0)
	{
		switch (tipoLig)
		{
			case tetDELTAABERTO:
			case tetDELTAFECHADO:
				s_kva = infoET.potForca_kva;
				break;
			default:
				s_kva = trafo->snom*1000;
			;
		}
	}
	else if (trafo->Codigo.Pos("força") > 0)
	{
		s_kva = trafo->snom*1000;
	}

	return s_kva;
}
//---------------------------------------------------------------------------
double __fastcall TNixTfELPA::PotenciaInstaladaNoturno_kva(VTTrafo *trafo, int tipoLig)
{	//variáveis locais
	double   s_kva = 0.;

	//1o verifica se é de força ou luz
	if (trafo->Codigo.Pos("luz") > 0)
	{
		s_kva = trafo->snom*1000;
	}
	else if (trafo->Codigo.Pos("força") > 0)
	{
		switch (tipoLig)
		{
			case tetDELTAFECHADO:
				s_kva = infoET.potForca_kva;
				break;
			default:
				s_kva = trafo->snom*1000;
			;
		}
	}
	else
	{
		s_kva = trafo->snom*1000;
    }

	return s_kva;
}
//---------------------------------------------------------------------------
int __fastcall TNixTfELPA::TipoET(TList *lisTRAFOS)
{
	VTTrafo *trafo;
	int tipo = 0;

	//protecao contra lista vazia
	if (lisTRAFOS == NULL)
	{
		return tipo;
	}
	switch (lisTRAFOS->Count)
	{
		case 1:
			//caso tenha um é preciso verificar se é mono ou trifasico
			trafo = (VTTrafo*) lisTRAFOS->First();
			if (trafo->Monofasico)
			{
				tipo = tetMONO;
			}
			else
			{
				tipo = tetTRIF;
            }
			break;
		case 2:
			tipo = tetDELTAABERTO;
			break;
		case 3:
			tipo = tetDELTAFECHADO;
			break;
		default:
			tipo = 0;
			;
	}

	return tipo;
}
//---------------------------------------------------------------------------

