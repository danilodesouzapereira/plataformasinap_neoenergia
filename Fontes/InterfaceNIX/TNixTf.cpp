//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TNixTf.h"
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
VTNixTf* __fastcall NewObjNixTfDefault(VTApl *apl, VTCarga *carga, strCFGINIX cfg)
{
	return (new TNixTf(apl, carga, cfg));
}
// ---------------------------------------------------------------------------
VTNixTf* __fastcall NewObjNixTfDefault(VTApl *apl, VTSecundario *secundario, strCFGINIX cfg)
{
	return (new TNixTf(apl, secundario, cfg));
}
// ---------------------------------------------------------------------------
VTNixTf* __fastcall NewObjNixTfDefault(VTApl *apl, TList *lisTRFParalelo, strCFGINIX cfg)
{
	return (new TNixTf(apl, lisTRFParalelo, cfg));
}
//---------------------------------------------------------------------------
__fastcall TNixTf::TNixTf(void)
   {

   }
//---------------------------------------------------------------------------
__fastcall TNixTf::TNixTf(VTApl *apl, VTCarga *carga, strCFGINIX cfg)
   {
   //variávies locais
   int num_ip;

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
   strTF.tf           = "TF";
   strTF.numero_no    = carga->pbarra->Id;
   strTF.codigo       = carga->Codigo;
   strTF.potencia     = PotenciaInstalada_kva(carga);
   strTF.fp           = FatorPotencia(carga);
   //strTF.fd           = FatorDemanda(carga);
   strTF.tipo         = 2;
   strTF.proprietario = 1;
   strTF.fases        = 3;
//   strTF.residenciais = 0;
//   strTF.comerciais   = 0;
//   strTF.industriais  = 1;
//   strTF.rurais       = 0;
//   strTF.outros       = 0;
   //caso seja uma carga equivalente
   if (carga->classe->Tag == consEQUIVALENTE)
      {
      strTF.residenciais = carga->NumConsResidencial;
      strTF.comerciais   = carga->NumConsComercial;
      strTF.industriais  = carga->NumConsIndustrial;
      strTF.rurais       = carga->NumConsRural;
      strTF.outros       = carga->NumConsOutros;
      }
   else
      {
      strTF.residenciais = 0;
      strTF.comerciais   = 0;
      strTF.industriais  = 1;
      strTF.rurais       = 0;
      strTF.outros       = 0;
      }
   //FD de acordo com configuracao
   if (cfg.fd_unico_a4)
      {strTF.fd  = cfg.fdTrafo;}
   else
      {strTF.fd = FatorDemanda(carga);}
   }

//---------------------------------------------------------------------------
__fastcall TNixTf::TNixTf(VTApl *apl, VTSecundario *secundario, strCFGINIX cfg)
   {
   //variávies locais
   int num_ip;
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
   strTF.tf           = "TF";
   strTF.numero_no    = secundario->BarraInicial->Id;
   strTF.codigo       = secundario->Rede->Codigo;
   strTF.potencia     = PotenciaInstalada_kva();
   strTF.fp           = FatorPotencia();
   //strTF.fd           = FatorDemanda();
   strTF.tipo         = 0;
   strTF.proprietario = 0;
   strTF.fases        = FasesPrimario();
   strTF.residenciais = NumeroDeCarga(consRESIDENCIAL);
   strTF.comerciais   = NumeroDeCarga(consCOMERCIAL);
   strTF.industriais  = NumeroDeCarga(consINDUSTRIAL);
   strTF.rurais       = NumeroDeCarga(consRURAL);
   strTF.outros       = NumeroDeCarga(consOUTROS);
   //redefine tipo da rede em função dos consumidores
   num_ip = NumeroDeCarga(consIP);
   if      ((strTF.residenciais + strTF.comerciais + strTF.outros) > 0) strTF.tipo = 0;
   else if (strTF.industriais > 0)                                      strTF.tipo = 2;
   else if (strTF.rurais > 0)                                           strTF.tipo = 1;
   else if (num_ip > 0)                                                 strTF.tipo = 3;
   else                                                                 strTF.tipo = 0;
   //FD de acordo com configuracao
   if (cfg.fd_unico_trafo)
      {strTF.fd  = cfg.fdTrafo;}
   else
      {strTF.fd = FatorDemanda();}
   }
 //---------------------------------------------------------------------------
__fastcall TNixTf::TNixTf(VTApl *apl, TList *lisTRFParalelo, strCFGINIX cfg)
   {//variávies locais
   int num_ip;
   VTTrafo *trafo;
   //salva ponteiro para objeto
   demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));


   //inicializa o vetor de complexos
   smva.size = 0;
   //cria objetos
   lisCAR = new TList();
   lisTRF = new TList();
   CopiaTList(lisTRFParalelo,lisTRF);
   //protecao contra lista vazia
   if(lisTRFParalelo->Count <= 0){return;}
   //Obtem a lista de cargas
   LisCargasBancoTrafo(lisTRFParalelo,lisCAR);
   //pega um trafo da lista
   trafo = (VTTrafo*) lisTRFParalelo->First();
   //inicia dados
   strTF.tf           = "TF";
   strTF.numero_no    = trafo->BarraPrimario()->Id;
   strTF.codigo       = trafo->Codigo;
   strTF.potencia     = PotenciaInstalada_kva();
   strTF.fp           = FatorPotencia();
   //strTF.fd           = FatorDemanda();
   strTF.tipo         = 0;
   strTF.proprietario = 0;
   strTF.fases        = FasesPrimario();
   strTF.residenciais = NumeroDeCarga(consRESIDENCIAL);
   strTF.comerciais   = NumeroDeCarga(consCOMERCIAL);
   strTF.industriais  = NumeroDeCarga(consINDUSTRIAL);
   strTF.rurais       = NumeroDeCarga(consRURAL);
   strTF.outros       = NumeroDeCarga(consOUTROS);
   //redefine tipo da rede em função dos consumidores
   num_ip = NumeroDeCarga(consIP);
   if      ((strTF.residenciais + strTF.comerciais + strTF.outros) > 0) strTF.tipo = 0;
   else if (strTF.industriais > 0)                                      strTF.tipo = 2;
   else if (strTF.rurais > 0)                                           strTF.tipo = 1;
   else if (num_ip > 0)                                                 strTF.tipo = 3;
   else                                                                 strTF.tipo = 0;
   //FD de acordo com configuracao
   if (cfg.fd_unico_trafo)
      {strTF.fd  = cfg.fdTrafo;}
   else
      {strTF.fd = FatorDemanda();}
   }

//---------------------------------------------------------------------------
__fastcall TNixTf::~TNixTf(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisCAR) {delete lisCAR; lisCAR = NULL;}
   if (lisTRF) {delete lisTRF; lisTRF = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TNixTf::AlocaVectorSmva(int numpat)
   {
   //verifica se o vetor de complexos alocado tem tamanho suficiente
   if (smva.size < numpat)
      {//verifica se havia um vetor alocado
      if (smva.size > 0)
         {//libera vetor alocado
         delete[] smva.complex;
         delete[] smva.P;
         }
      //aloca vetor do tamanho indicado
      smva.size = numpat;
      smva.complex = new complex<double>[numpat];
      smva.P = new double[numpat];
      }
   return(smva.size >= numpat);
   }
//---------------------------------------------------------------------------
void __fastcall TNixTf::ClearVetorSmva(void)
{
   for (int i = 0; i < smva.size; i++)
      {
      smva.complex[i] = CZero();
      smva.P[i] = 0.0;
      }
}
//---------------------------------------------------------------------------
AnsiString __fastcall TNixTf::LinhaArqCsv(void)
   {
   //variáveis locais
   //char       separador = ';';
   AnsiString       separador = ";";
   AnsiString txt;

   txt  = strTF.tf                             + separador;
   txt += IntToStr(strTF.numero_no)            + separador;
   txt += strTF.codigo                         + separador;
   txt += DoubleToStr("%2.1f", strTF.potencia) + separador;
   //FKM 2016.05.10 - aumentei a precisao devido a alguns trafos
   txt += DoubleToStr("%4.3f", strTF.fp)       + separador;
   txt += DoubleToStr("%4.3f", strTF.fd)       + separador;
   txt += IntToStr(strTF.tipo        )         + separador;
   txt += IntToStr(strTF.proprietario)         + separador;
   txt += IntToStr(strTF.fases       )         + separador;
   txt += IntToStr(strTF.residenciais)         + separador;
   txt += IntToStr(strTF.comerciais  )         + separador;
   txt += IntToStr(strTF.industriais )         + separador;
   txt += IntToStr(strTF.rurais      )         + separador;
   txt += IntToStr(strTF.outros      )         + separador;
   return(txt);
   }
////---------------------------------------------------------------------------
//void __fastcall TNixTf::LisTrafoParalelo(VTTrafo *trafo, TList *lisEXT)
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
int __fastcall TNixTf::FasesPrimario(void)
   {
   //variáveis locais
   int      fases = 0;
   VTTrafo *trafo;

   //loop p/ todos Trafos
   for (int n = 0; n < lisTRF->Count; n++)
      {
      trafo = (VTTrafo*)lisTRF->Items[n];
      fases |= trafo->pri.fase;
      }
   //retorna fases no padrão NIX
   return(TraduzFases(fases));
   }

////---------------------------------------------------------------------------
//double __fastcall TNixTf::FatorDemanda(void)
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
double __fastcall TNixTf::FatorDemanda(void)
   {
   //variáveis locais
   double          fp = 0.92;
   double          fatdem;
   double          P_max_mw = 0;
   VTTrafo         *trafo;
   VTCarga         *carga;
   int             max_pat = patamares->NumPatamar();

   if (!IsDoubleZero(strTF.fp))   fp = strTF.fp;
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
   if (IsDoubleZero(strTF.potencia))
      {fatdem = 1.0;}
   else
      {fatdem = (P_max_mw*1000)/(strTF.potencia*fp);}
   //ultima verificação
   if (IsDoubleZero(fatdem))
      {fatdem = 1.0;}
   return(fatdem);
   }
//---------------------------------------------------------------------------
//double __fastcall TNixTf::FatorDemanda(VTCarga *carga)
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
double __fastcall TNixTf::FatorDemanda(VTCarga *carga)
   {
   //variáveis locais
   double fp     = 0.92;
   double fatdem = 1.0;
   double s_max_mva, p_max_mw, q_max_mvar;


   if (!IsDoubleZero(strTF.fp))   fp = strTF.fp;
   //determina demanda máxima
   if (demanda->GetSmax_mva(carga, s_max_mva, p_max_mw, q_max_mvar))
      {
      if (IsDoubleZero(strTF.potencia))
         {fatdem = 1.0;}
      else
         {fatdem = (p_max_mw*1000)/(strTF.potencia*fp);}
      }
   //ultima verificação
   if (IsDoubleZero(fatdem))
      {fatdem = 1.0;}
   return(fatdem);
   }
//---------------------------------------------------------------------------
double __fastcall TNixTf::FatorPotencia(void)
   {
   //variáveis locais
   double          p_mw, mod_s_mva, fatpot = 0.92;
   complex<double> s_mva = CZero();
   VTTrafo        *trafo;

   //loop p/ todoas Trafos
   for (int n = 0; n < lisTRF->Count; n++)
      {
      trafo = (VTTrafo*)lisTRF->Items[n];
      s_mva += trafo->resflow->S_mva_max_allpat();
      }
   //calcula fatpot
   p_mw      = s_mva.real();
   mod_s_mva = Abs(s_mva);
   if      (IsDoubleZero(p_mw))      fatpot = 0.92;
   else if (IsDoubleZero(mod_s_mva)) fatpot = 0.92;
   else                              fatpot = p_mw / mod_s_mva;

   return(fatpot);
   }

//---------------------------------------------------------------------------
double __fastcall TNixTf::FatorPotencia(VTCarga *carga)
   {
   //variáveis locais
   double fatpot = 0.92;
   double s_max_mva, p_max_mw, q_max_mvar;

   //determina demanda máxima
   if (demanda->GetSmax_mva(carga, s_max_mva, p_max_mw, q_max_mvar))
      {
      if      (IsDoubleZero(p_max_mw))  fatpot = 0.92;
      else if (IsDoubleZero(s_max_mva)) fatpot = 0.92;
      else                              fatpot = p_max_mw / s_max_mva;
      }
   return(fatpot);
   }
//---------------------------------------------------------------------------
void __fastcall TNixTf::LisCargasBancoTrafo(TList *lisTrafos, TList *lisEXT)
   {//variaveis locais
   VTCarga *carga;
   VTBarra *barra1, *barra2, *barra3;
   VTBarra *barraPri;
   VTTrafo *trafo;

   //percorre todos os trafos do banco
   for(int nt = 0; nt < lisTrafos->Count; nt++)
      {
      trafo = (VTTrafo*)lisTrafos->Items[nt];
      barraPri = trafo->BarraPrimario();

      barra1 = trafo->pbarra1;
      if((barra1 != NULL) && (barra1 != barraPri))
         {barra1->LisEqbar(lisEXT, eqptoCARGA);}
      barra2 = trafo->pbarra2;
      if((barra2 != NULL) && (barra2 != barraPri))
         {barra2->LisEqbar(lisEXT, eqptoCARGA);}
      barra3 = trafo->pbarra3;
      if((barra3 != NULL) && (barra3 != barraPri))
         {barra3->LisEqbar(lisEXT, eqptoCARGA);}
      }
   }
//---------------------------------------------------------------------------
//int __fastcall TNixTf::NumeroDeCarga(int classe_tag)
//   {
//   //variáveis locais
//   int     num_carga = 0;
//   VTCarga *carga;
//
//   for (int n = 0; n < lisCAR->Count; n++)
//      {
//      carga = (VTCarga*)lisCAR->Items[n];
//      if (carga->classe->Tag == classe_tag) num_carga++;
//      }
//   return(num_carga);
//   }
//---------------------------------------------------------------------------
//FKM 2016.06.06 - não estava contabilizando corretamente as cargas equivalentes
//---------------------------------------------------------------------------
int __fastcall TNixTf::NumeroDeCarga(int classe_tag)
   {
   //variáveis locais
   int     num_carga = 0;
   VTCarga *carga;

   for (int n = 0; n < lisCAR->Count; n++)
      {
      carga = (VTCarga*)lisCAR->Items[n];
      if (carga->classe->Tag == consEQUIVALENTE)
         {
         switch (classe_tag)
            {
            case consRESIDENCIAL:
               num_carga += carga->NumConsResidencial;
               break;
            case consCOMERCIAL:
               num_carga += carga->NumConsComercial;
               break;
            case consINDUSTRIAL:
               num_carga += carga->NumConsIndustrial;
               break;
            case consRURAL:
               num_carga += carga->NumConsRural;
               break;
            case consOUTROS:
               num_carga += carga->NumConsOutros;
               break;
            default:
               ;
            }
         }
      else if (carga->classe->Tag == classe_tag) num_carga++;
      }
   return(num_carga);
   }
//---------------------------------------------------------------------------
double __fastcall TNixTf::PotenciaInstalada_kva(void)
   {
   //variáveis locais
   double   s_mva = 0.;
   VTTrafo *trafo;

   //loop p/ todos Trafos
   for (int n = 0; n < lisTRF->Count; n++)
      {
      trafo = (VTTrafo*)lisTRF->Items[n];
      s_mva += trafo->snom;
      }
   return(s_mva * 1000.);
   }
//---------------------------------------------------------------------------
//soma picos não coincidentes
//---------------------------------------------------------------------------
//double __fastcall TNixTf::PotenciaInstalada_kva(void)
//   {
//   //variáveis locais
//   //double   s_mva = 0.;
//   double     mod_Smva = 0;
//   complex<double> s_mva = CZero();
//   VTTrafo *trafo;
//   VTCarga *carga;
//
//   //loop p/ todos Trafos
//   for (int n = 0; n < lisTRF->Count; n++)
//      {
//      trafo = (VTTrafo*)lisTRF->Items[n];
//      //s_mva += trafo->snom;
//      s_mva += trafo->resflow->S_mva_max_allpat();
//      }
//   mod_Smva = Abs(s_mva);
//   //verifica se o resultado foi nulo
//   //se for porque não convergiu o fluxo pras redes bt...
//   if (IsDoubleZero(mod_Smva))
//      {
//      for (int nc = 0; nc < lisCAR->Count; nc++)
//         {
//         carga = (VTCarga*) lisCAR->Items[nc];
//         mod_Smva += PotenciaInstalada_kva(carga)/1000;
//         }
//      }
//   //return(s_mva * 1000.);
//   return(mod_Smva*1000);
//   }
//---------------------------------------------------------------------------
double __fastcall TNixTf::PotenciaInstalada_kva(VTCarga *carga)
   {
   //variáveis locais
   double s_max_kva = 10.0;
   double p_max_mw, q_max_mvar;

   //determina demanda máxima
   if (demanda->GetSmax_mva(carga, s_max_kva, p_max_mw, q_max_mvar))
      {//vem em mva
      s_max_kva *= 1100;
      }
   //assume um valor padrão
   if      (s_max_kva <   5.) s_max_kva =   5.;
   else if (s_max_kva <  10.) s_max_kva =  10.;
   else if (s_max_kva <  15.) s_max_kva =  15.;
   else if (s_max_kva <  30.) s_max_kva =  30.;
   else if (s_max_kva <  45.) s_max_kva =  45.;
   else if (s_max_kva <  75.) s_max_kva =  75.;
   else if (s_max_kva <  112.5) s_max_kva =  112.5;
   else if (s_max_kva <  150.) s_max_kva =  150.;
   else if (s_max_kva <  225.) s_max_kva = 225.;
   else if (s_max_kva <  300.) s_max_kva = 300.;
   else if (s_max_kva <  500.) s_max_kva = 500.;
   else if (s_max_kva <  750.) s_max_kva = 750.;
   else if (s_max_kva <  1000.) s_max_kva = 1000.;
   else if (s_max_kva <  2000.) s_max_kva = 2000.;
   else if (s_max_kva <  2500.) s_max_kva = 2500.;
   else if (s_max_kva <  3000.) s_max_kva = 3000.;
   else                       s_max_kva = 5000.;

   return(s_max_kva);
   }
//---------------------------------------------------------------------------
//double __fastcall TNixTf::PotenciaInstalada_kva(VTCarga *carga)
//   {
//   //variáveis locais
//   double s_max_kva = 10.0;
//   double p_max_mw, q_max_mvar;
//
//   //determina demanda máxima
//   if (demanda->GetSmax_mva(carga, s_max_kva, p_max_mw, q_max_mvar))
//      {//vem em mva
//      s_max_kva *= 1000;
//      }
//   return(s_max_kva);
//   }
//---------------------------------------------------------------------------
int __fastcall TNixTf::TraduzFases(int fases)
   {
   fases |= faseNT;

   if      ((fases & faseABCNT) == faseABCNT)
	{return(3);}
   else if ((fases & faseABNT ) == faseABNT )
	{return(4);}
   else if ((fases & faseCANT ) == faseCANT )
	{return(5);}
   else if ((fases & faseBCNT ) == faseBCNT )
	{return(6);}
   else if ((fases & faseANT  ) == faseANT  )
	{return(7);}
   else if ((fases & faseBNT  ) == faseBNT  )
	{return(8);}
   else if ((fases & faseCNT  ) == faseCNT  )
	{return(9);}
   else
   	{return(3);}
   }

