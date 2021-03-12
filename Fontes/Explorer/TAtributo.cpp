//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TAtributo.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Geral\VTGeral.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTYref.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTTipoChave.h"
#include "..\Rede\VTTipoRede.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar obbjeto da classe
//---------------------------------------------------------------------------
VTAtributo* __fastcall NewObjAtributo(VTApl *apl)
   {
   try{
   return(new TAtributo(apl));
      }catch(Exception &e)
         {//nada a fazer
         }
   //erro na criação do objeto
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TAtributo::TAtributo(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria objetos
    slNome  = new TStringList();
    slValor = new TStringList();
   }

//---------------------------------------------------------------------------
__fastcall TAtributo::~TAtributo(void)
   {
   //destrói objetos criados
   if (slNome)  {delete slNome;  slNome  = NULL;}
   if (slValor) {delete slValor; slValor = NULL;}
   }

//---------------------------------------------------------------------------
int __fastcall TAtributo::Count(void)
   {
   //proteção
   if (slNome->Count != slValor->Count) return(0);
   return(slNome->Count);
   }

//---------------------------------------------------------------------------
void __fastcall TAtributo::Executa(VTEqpto *eqpto)
   {
   //obtém ponteiros p/ objetos
   consulta = (VTConsulta*)apl->GetObject(__classid(VTConsulta));
   geral    = (VTGeral*)apl->GetObject(__classid(VTGeral));
   //reinicia TStringList
   slNome->Clear();
   slValor->Clear();
   //trata cada um dos Eqptos
   switch(eqpto->Tipo())
      {
      case eqptoBARRA:      MontaBarra((VTBarra*)eqpto);           break;
      case eqptoCARGA:      MontaCarga((VTCarga*)eqpto);           break;
      case eqptoCAPACITOR:  MontaCapacitor((VTCapacitor*)eqpto);   break;
      case eqptoCHAVE:      MontaChave((VTChave*)eqpto);           break;
      case eqptoGERADOR:    MontaGerador((VTGerador*)eqpto);       break;
      case eqptoREDE:       MontaRede((VTRede*)eqpto);             break;
      case eqptoSUPRIMENTO: MontaSuprimento((VTSuprimento*)eqpto); break;
      case eqptoTRAFO:      MontaTrafo((VTTrafo*)eqpto);           break;
      case eqptoTRECHO:     MontaTrecho((VTTrecho*)eqpto);         break;
		case eqptoYREF:       MontaYref((VTYref*)eqpto);             break;
      default: return;
		}
   }

//---------------------------------------------------------------------------
double __fastcall TAtributo::ExtensaoTrecho(TList *lisTRE)
   {
   //variáveis locais
   VTTrecho *ptrecho;
   double   comp = 0;

   //loop p/ todos Trechos
   for (int n = 0; n < lisTRE->Count; n++)
      {
      ptrecho = (VTTrecho*)lisTRE->Items[n];
      comp   += ptrecho->Comprimento_km;
      }
   return(comp);
   }

//---------------------------------------------------------------------------
void __fastcall TAtributo::InsereLinhaSeparacao(void)
   {
   slNome->Add("");
   slValor->Add("");
   }
/*
//---------------------------------------------------------------------------
void __fastcall TAtributo::MontaBarra(VTBarra *pbarra)
   {
   //monta atributos da classe base VTEqpto
   MontaEqpto(pbarra);
   //monta atributos da barra
   //Vnom
   slNome->Add("Vnom(kV)");
   slValor->Add(DoubleToStr("%4.3f", pbarra->vnom));
   //V(pu) por patamar
   InsereLinhaSeparacao();
   slNome->Add("Patamar");
   slValor->Add("V(pu)");
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      slNome->Add(geral->PAT.NOME[np].LowerCase());
      slValor->Add(DoubleToStr("%4.3f", pbarra->flow.v[np]));
      }
   //V(kV) por patamar
   InsereLinhaSeparacao();
   slNome->Add("Patamar");
   slValor->Add("V(kV)");
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      slNome->Add(geral->PAT.NOME[np].LowerCase());
      slValor->Add(DoubleToStr("%4.3f", pbarra->flow.v[np] * pbarra->vnom));
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TAtributo::MontaBarra(VTBarra *pbarra)
   {
   //variáveis locais
   char  *vfn[] = {"Van", "Vbn", "Vcn"};
   char  *vff[] = {"Vab", "Vbc", "Vca"};

   //monta atributos da classe base VTEqpto
   MontaEqpto(pbarra);
   //monta atributos da barra
   //Vnom
   slNome->Add("Vnom(kV)");
   slValor->Add(DoubleToStr("%4.3f", pbarra->vnom));
   //Vfn(pu) por patamar
   InsereLinhaSeparacao();
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      slNome->Add("Patamar");
      slValor->Add(geral->PAT.NOME[np].LowerCase());
      for (int nf = 0; nf < sizeof(vfn)/sizeof(char*); nf++)
         {
         slNome->Add(AnsiString(vfn[nf]) + "(pu:)");
         slValor->Add(ComplexPolarToStr("%4.3f", Abs(pbarra->resflow->Vfn_pu[np][nf]),
                                                 Arg(pbarra->resflow->Vfn_pu[np][nf])));
         }
      }
   //Vfn(kV) por patamar
   InsereLinhaSeparacao();
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      slNome->Add("Patamar");
      slValor->Add(geral->PAT.NOME[np].LowerCase());
      for (int nf = 0; nf < sizeof(vfn)/sizeof(char*); nf++)
         {
         slNome->Add(AnsiString(vfn[nf]) + "(kV:)");
         slValor->Add(ComplexPolarToStr("%4.3f", Abs(pbarra->resflow->Vfn_pu[np][nf]) * pbarra->vnom / sqrt(3.),
                                                 Arg(pbarra->resflow->Vfn_pu[np][nf])));
         }
      }
   //Vff(pu) por patamar
   InsereLinhaSeparacao();
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      slNome->Add("Patamar");
      slValor->Add(geral->PAT.NOME[np].LowerCase());
      for (int nf = 0; nf < sizeof(vff)/sizeof(char*); nf++)
         {
         slNome->Add(AnsiString(vff[nf]) + "(pu:)");
         slValor->Add(ComplexPolarToStr("%4.3f", Abs(pbarra->resflow->Vff_pu[np][nf]),
                                                 Arg(pbarra->resflow->Vff_pu[np][nf])));
         }
      }
   //Vfn(kV) por patamar
   InsereLinhaSeparacao();
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      slNome->Add("Patamar");
      slValor->Add(geral->PAT.NOME[np].LowerCase());
      for (int nf = 0; nf < sizeof(vff)/sizeof(char*); nf++)
         {
         slNome->Add(AnsiString(vff[nf]) + "(kV:)");
         slValor->Add(ComplexPolarToStr("%4.3f", Abs(pbarra->resflow->Vff_pu[np][nf]) * pbarra->vnom,
                                                 Arg(pbarra->resflow->Vff_pu[np][nf])));
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAtributo::MontaCapacitor(VTCapacitor *pcap)
   {
   //monta atributos da classe base VTEqbar
   MontaEqbar(pcap);
   //monta atributos da Carga
   //Vnom
   slNome->Add("Vnom(kV)");
   slValor->Add(DoubleToStr("%4.3f", pcap->vnom));
   //Pot.reativa
   slNome->Add("Q(MVA)");
   slValor->Add("");
   slValor->Add(DoubleToStr("%4.3f", pcap->q));
   }

//---------------------------------------------------------------------------
void __fastcall TAtributo::MontaCarga(VTCarga *pcarga)
   {
   //monta atributos da classe base VTEqbar
   MontaEqbar(pcarga);
   //monta atributos da Carga
   //energia
   slNome->Add("Energia(MWhmês)");
   slValor->Add(DoubleToStr("%4.3f", pcarga->Energia_mwhmes));
   }

//---------------------------------------------------------------------------
void __fastcall TAtributo::MontaChave(VTChave *pchave)
   {
   //monta atributos da classe base VTLigacao
   MontaLigacao(pchave);
   //monta atributos da Chave
   //tipo
   slNome->Add("Tipo");
   slValor->Add(pchave->TipoChave->Codigo);
   //estado
   slNome->Add("Estado");
   (pchave->Aberta) ? slValor->Add("Aberta") : slValor->Add("Fechada");
   //iadm
   slNome->Add("Iadm(A)");
   slValor->Add(DoubleToStr("%2.1f", pchave->Inom_amp));
   /*
   //flow:corrente
   InsereLinhaSeparacao();
   slNome->Add("Patamar");
   slValor->Add("I(A)");
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      slNome->Add(geral->PAT.NOME[np].LowerCase());
      slValor->Add(DoubleToStr("%2.1f", pchave->flow.i[0][np]));
      }
   //flow:carregamento
   InsereLinhaSeparacao();
   slNome->Add("Patamar");
   slValor->Add("Cargto(%)");
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      slNome->Add(geral->PAT.NOME[np].LowerCase());
      slValor->Add(DoubleToStr("%2.1f", pchave->flow.cargto[np]*100));
      }
   */
   }

//---------------------------------------------------------------------------
void __fastcall TAtributo::MontaEqbar(VTEqbar *eqbar)
   {
   //monta atributos da classe base VTEqpto
   MontaEqpto(eqbar);
   //monta atributos de VTEqbar
   //barra
   slNome->Add("barra");
   slValor->Add(eqbar->pbarra->Codigo);
   }

//---------------------------------------------------------------------------
void __fastcall TAtributo::MontaEqpto(VTEqpto *eqpto)
   {
   //monta atributos de VTEqpto
   //id
   slNome->Add("id");
   slValor->Add(IntToStr(eqpto->Id));
   //código
   slNome->Add("codigo");
   slValor->Add(eqpto->Codigo);
   //extern_id
   slNome->Add("extern_id");
   slValor->Add(eqpto->Extern_id);
   }

//---------------------------------------------------------------------------
void __fastcall TAtributo::MontaGerador(VTGerador *pgera)
   {
   //monta atributos da classe base VTEqbar
   MontaEqbar(pgera);
   //monta atributos do Gerador
   //Smax
   slNome->Add("Snom(MVA)");
   slValor->Add(DoubleToStr("%4.3f", pgera->snom));
   slNome->Add("Vnom(kV)");
   slValor->Add(DoubleToStr("%4.3f", pgera->vnom));
   slNome->Add("Ligação");
   switch(pgera->ligacao)
      {
      case lgEST_AT:  slValor->Add("Yaterrado"); break;
      case lgEST_ISO: slValor->Add("Yisolado");  break;
      case lgTRIANG:  slValor->Add("Delta");     break;
      }
   slNome->Add("Fat.Pot.máximo)");
   slValor->Add(DoubleToStr("%4.3f", pgera->fplim));
   //z1
   slNome->Add("z1(pu)");
   slValor->Add(ComplexToStr("%4.3f", pgera->z1.r, pgera->z1.x));
   //z0
   slNome->Add("z0(pu)");
   slValor->Add(ComplexToStr("%4.3f", pgera->z0.r, pgera->z0.x));
   //verifica o tipo de Gerador
   if (pgera->tiposup == supPV)
      {//tipo
      slNome->Add("Tipo");
      slValor->Add("PV");
      /*
      //V
      InsereLinhaSeparacao();
      slNome->Add("Patamar");
      slValor->Add("V(pu)");
      for (int np = 0; np < geral->PAT.NUM_PAT; np++)
         {
         slNome->Add(geral->PAT.NOME[np].LowerCase());
         slValor->Add(DoubleToStr("%4.3f", pgera->v[np]));
         }
      //P
      InsereLinhaSeparacao();
      slNome->Add("Patamar");
      slValor->Add("P(MW)");
      for (int np = 0; np < geral->PAT.NUM_PAT; np++)
         {
         slNome->Add(geral->PAT.NOME[np].LowerCase());
         slValor->Add(DoubleToStr("%4.3f", pgera->p[np]));
         }
      */
      }
   else
      {//tipo
      slNome->Add("Tipo");
      slValor->Add("PQ");
      /*
      //P e Q
      InsereLinhaSeparacao();
      slNome->Add("Patamar");
      slValor->Add("S(MVA)");
      for (int np = 0; np < geral->PAT.NUM_PAT; np++)
         {
         slNome->Add(geral->PAT.NOME[np].LowerCase());
         slValor->Add(ComplexToStr("%4.3f", pgera->p[np], pgera->q[np]));
         }
      */
      }
   //resultados de flow
   /*
   //fluxo(MVA)
   InsereLinhaSeparacao();
   slNome->Add("Patamar");
   slValor->Add("Geração(MVA)");
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      slNome->Add(geral->PAT.NOME[np].LowerCase());
      slValor->Add(ComplexToStr("%4.3f", pgera->flow.p[np], pgera->flow.q[np]));
      }
   //carregamento(%)
   InsereLinhaSeparacao();
   slNome->Add("Patamar");
   slValor->Add("Cargto(%))");
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      slNome->Add(geral->PAT.NOME[np].LowerCase());
      slValor->Add(DoubleToStr("%2.1f", pgera->flow.cargto[np]));
      }
   */
   }

//---------------------------------------------------------------------------
void __fastcall TAtributo::MontaLigacao(VTLigacao *pliga)
   {
   //variáveis lcoais
   VTBarra *pbar[NBAR_LIG] = {pliga->pbarra1, pliga->pbarra2, pliga->pbarra3};

   //monta atributos da classe base VTEqpto
   MontaEqpto(pliga);
   //monta atributos de VTLigacao
   //rede
   slNome->Add("rede");
   (pliga->rede == NULL) ? slValor->Add("-") : slValor->Add(pliga->rede->Codigo);
   //barras da ligação
   for (int nb = 0; nb < NBAR_LIG; nb++)
      {
      if (pbar[nb] == NULL) continue;
      slNome->Add("barra" + IntToStr(nb+1));
      slValor->Add(pbar[nb]->Codigo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAtributo::MontaRede(VTRede *rede)
   {
   //variáveis locais
   int num_liga;

   //monta atributos da classe base VTEqto
   MontaEqpto(rede);
   //monta atributos de VTRede
   //tipo de rede
   slNome->Add("Classe");
   slValor->Add(rede->TipoRede->Codigo);
   //radial
   if (rede->Radial)
      {
      slNome->Add("Topologia");
      slValor->Add("Radial");
      }
   //número de Barras
   slNome->Add("N.Barras");
   slValor->Add((rede->LisBarra())->Count);
   //número de Trechos
   if ((num_liga = rede->NumeroDeLigacoes(eqptoTRECHO)) > 0)
      {
      slNome->Add("N.Trechos");
      slValor->Add(IntToStr(num_liga));
      slNome->Add("Extensão(km)");
      slValor->Add(DoubleToStr("%4.3f", ExtensaoTrecho(consulta->LisEqpto(rede, eqptoTRECHO))));
      }
   //número de Trafos
   if ((num_liga = rede->NumeroDeLigacoes(eqptoTRAFO)) > 0)
      {
      slNome->Add("N.Trafos");
      slValor->Add(IntToStr(num_liga));
      }
   //número de Chaves
   if ((num_liga = rede->NumeroDeLigacoes(eqptoCHAVE)) > 0)
      {
      slNome->Add("N.Chaves");
      slValor->Add(IntToStr(num_liga));
      }
   //número de Cargas
   if ((num_liga = rede->NumeroDeEqbar(eqptoCARGA)) > 0)
      {
      slNome->Add("N.Cargas");
      slValor->Add(IntToStr(num_liga));
      }
   //número de Geradores
   if ((num_liga = rede->NumeroDeEqbar(eqptoGERADOR)) > 0)
      {
      slNome->Add("N.Geradores");
      slValor->Add(IntToStr(num_liga));
      }
   //número de Suprimentos
   if ((num_liga = rede->NumeroDeEqbar(eqptoSUPRIMENTO)) > 0)
      {
      slNome->Add("N.Suprimentos");
      slValor->Add(IntToStr(num_liga));
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAtributo::MontaSuprimento(VTSuprimento *psup)
   {
   //monta atributos da classe base VTEqbar
   MontaEqbar(psup);
   //monta atributos do Suprimento
   //Smax
   slNome->Add("Smax(MVA)");
   slValor->Add(DoubleToStr("%4.3f", psup->smax));
   //verifica o tipo de suprimento
   if (psup->tiposup == supPV)
      {//tipo se suprimento
      slNome->Add("Tipo");
      slValor->Add("PV");
      /*
      //V
      InsereLinhaSeparacao();
      slNome->Add("Patamar");
      slValor->Add("V(pu)");
      for (int np = 0; np < geral->PAT.NUM_PAT; np++)
         {
         slNome->Add(geral->PAT.NOME[np].LowerCase());
         slValor->Add(DoubleToStr("%4.3f", psup->v[np]));
         }
      //P
      InsereLinhaSeparacao();
      slNome->Add("Patamar");
      slValor->Add("P(MW)");
      for (int np = 0; np < geral->PAT.NUM_PAT; np++)
         {
         slNome->Add(geral->PAT.NOME[np].LowerCase());
         slValor->Add(DoubleToStr("%4.3f", psup->p[np]));
         }
      */
      }
   else
      {//tipo
      slNome->Add("Tipo");
      slValor->Add("PQ");
      /*
      //P e Q
      InsereLinhaSeparacao();
      slNome->Add("Patamar");
      slValor->Add("S(MVA)");
      for (int np = 0; np < geral->PAT.NUM_PAT; np++)
         {
         slNome->Add(geral->PAT.NOME[np].LowerCase());
         slValor->Add(ComplexToStr("%4.3f", psup->p[np], psup->q[np]));
         }
      */
      }
   //resultados de flow
   /*
   //fluxo(MVA)
   InsereLinhaSeparacao();
   slNome->Add("Patamar");
   slValor->Add("Contribuição(MVA)");
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      slNome->Add(geral->PAT.NOME[np].LowerCase());
      slValor->Add(ComplexToStr("%4.3f", psup->flow.p[np], psup->flow.q[np]));
      }
   //carregamento(%)
   InsereLinhaSeparacao();
   slNome->Add("Patamar");
   slValor->Add("Cargto(%))");
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      slNome->Add(geral->PAT.NOME[np].LowerCase());
      slValor->Add(DoubleToStr("%2.1f", psup->flow.cargto[np]));
      }
   */
   }

//---------------------------------------------------------------------------
void __fastcall TAtributo::MontaTrafo(VTTrafo *ptrafo)
   {
   //variáveis locais
   bool       fluxo[MAX_PAT];
   strPOT     perda;
   AnsiString ligacao[NBAR_LIG] = {"pri", "sec", "ter"};
//   VTBarra    *pbar[NBAR_LIG] = {ptrafo->pbarra1, ptrafo->pbarra2, ptrafo->pbarra3};

   //monta atributos da classe base VTLigacao
   MontaLigacao(ptrafo);
   //monta atributos do Trecho
   //snom
   slNome->Add("Snom(MVA)");
   slValor->Add(DoubleToStr("%4.3f", ptrafo->snom));
   //pri.ligacao
   slNome->Add("pri:Ligação");
   switch(ptrafo->pri.ligacao)
      {
      case lgEST_AT:  slValor->Add("Yaterrado"); break;
      case lgEST_ISO: slValor->Add("Yisolado");  break;
      case lgTRIANG:  slValor->Add("Delta");     break;
      }
   //pri.vnom
   slNome->Add("pri: Vnom(kV)");
   slValor->Add(DoubleToStr("%4.3f", ptrafo->pri.vnom));
   //pri.tap
   slNome->Add("pri: Tap(kV)");
   slValor->Add(DoubleToStr("%4.3f", ptrafo->pri.tap));
   //pri.zterra
   if (ptrafo->pri.ligacao == lgEST_AT)
      {
      slNome->Add("pri: Zterra(ohm)");
      slValor->Add(ComplexToStr("%2.1f", ptrafo->pri.zterra.r, ptrafo->pri.zterra.x));
      }
   //sec.ligacao
   slNome->Add("sec:Ligação");
   switch(ptrafo->sec.ligacao)
      {
      case lgEST_AT:  slValor->Add("Yaterrado"); break;
      case lgEST_ISO: slValor->Add("Yisolado");  break;
      case lgTRIANG:  slValor->Add("Delta");     break;
      }
   //sec.vnom
   slNome->Add("sec: Vnom(kV)");
   slValor->Add(DoubleToStr("%4.3f", ptrafo->sec.vnom));
   //sec.tap
   slNome->Add("sec: Tap(kV)");
   slValor->Add(DoubleToStr("%4.3f", ptrafo->sec.tap));
   //sec.zterra
   if (ptrafo->sec.ligacao == lgEST_AT)
      {
      slNome->Add("sec: Zterra(ohm)");
      slValor->Add(ComplexToStr("%2.1f", ptrafo->sec.zterra.r, ptrafo->sec.zterra.x));
      }
   //z1
   slNome->Add("z1(pu)");
   slValor->Add(ComplexToStr("%4.3f", ptrafo->z1.r, ptrafo->z1.x));
   //z0
   slNome->Add("z0(pu)");
   slValor->Add(ComplexToStr("%4.3f", ptrafo->z0.r, ptrafo->z0.x));
   /*
   //flow: monta vetor de controle de existência de fluxo por patamar
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {//verifica se há fluxo
      fluxo[np] = ((ptrafo->flow.sentido[np] != fluxoNENHUM)||
                   (ptrafo->flow.sentido_reat[np] != fluxoNENHUM));
      }
   //flow: resultados por ligação
   for (int nb = 0; nb < NBAR_LIG; nb++)
      {//verifica se existe a barra
      if (pbar[nb] == NULL) continue;
      //flow:I(A) por patamar
      InsereLinhaSeparacao();
      slNome->Add("Patamar");
      slValor->Add(ligacao[nb] + ": I(A)");
      for (int np = 0; np < geral->PAT.NUM_PAT; np++)
         {
         if (! fluxo[np]) continue;
         slNome->Add(geral->PAT.NOME[np].LowerCase());
         slValor->Add(DoubleToStr("%2.1f", ptrafo->flow.i[nb][np]));
         }
      //flow: S(MVA) por patamar
      InsereLinhaSeparacao();
      slNome->Add("Patamar");
      slValor->Add(ligacao[nb] + ": S(MVA)");
      for (int np = 0; np < geral->PAT.NUM_PAT; np++)
         {
         if (! fluxo[np]) continue;
         //flow:S(MVA) no sentido orig->dest
         slNome->Add(geral->PAT.NOME[np].LowerCase());
         slValor->Add(ComplexToStr("%4.3f", ptrafo->flow.p[nb][np], ptrafo->flow.q[nb][np]));
         }
      }
   //flow:carregamento por patamar
   InsereLinhaSeparacao();
   slNome->Add("Patamar");
   slValor->Add("Cargto(%)");
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      slNome->Add( geral->PAT.NOME[np].LowerCase());
      slValor->Add(DoubleToStr("%2.1f", ptrafo->flow.cargto[np]*100));
      }
   //flow:perda(kW) por patamar
   InsereLinhaSeparacao();
   slNome->Add("Patamar");
   slValor->Add("Perda(kW)");
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      //determina perda em MVA
      perda.p = fabs(ptrafo->flow.p[0][np] + ptrafo->flow.p[1][np]);
      perda.q = fabs(ptrafo->flow.q[0][np] + ptrafo->flow.q[1][np]);
      //converte perdas de MVA p/ kVA
      perda.p *= 1000.;
      perda.q *= 1000.;
      slNome->Add(geral->PAT.NOME[np].LowerCase());
      slValor->Add(DoubleToStr("%4.3f", perda.p));
      }
   */
   }

//---------------------------------------------------------------------------
void __fastcall TAtributo::MontaTrecho(VTTrecho *ptrecho)
   {
   //variáveis locais
   bool   fluxo[MAX_PAT];
   int    orig, dest;
   strPOT perda;

   //monta atributos da classe base VTLigacao
   MontaLigacao(ptrecho);
   //monta atributos de Trecho
   //comprimento
   slNome->Add("Extensão(km)");
   slValor->Add(DoubleToStr("%4.3f", ptrecho->Comprimento_km));
   /*
   //flow: monta vetor de controle de existência de fluxo por patamar
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {//verifica se há fluxo
      fluxo[np] = ((ptrecho->flow.sentido[np] != fluxoNENHUM)||
                   (ptrecho->flow.sentido_reat[np] != fluxoNENHUM));
      }
   //flow:I(A) por patamar
   InsereLinhaSeparacao();
   slNome->Add("Patamar");
   slValor->Add("I(A)");
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      if (! fluxo[np]) continue;
      slNome->Add(geral->PAT.NOME[np].LowerCase());
      slValor->Add(DoubleToStr("%2.1f", ptrecho->flow.i[0][np]));
      }
   //flow: S(MVA) da barra1 p/ barra2 por patamar
   InsereLinhaSeparacao();
   slNome->Add("Patamar");
   slValor->Add("S(MVA)b1=>b2");
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      if (! fluxo[np]) continue;
      //flow:S(MVA) no sentido orig->dest
      slNome->Add(geral->PAT.NOME[np].LowerCase());
      slValor->Add(ComplexToStr("%4.3f", ptrecho->flow.p[0][np], ptrecho->flow.q[0][np]));
      }
   //flow: S(MVA) da barra2 p/ barra1 por patamar
   InsereLinhaSeparacao();
   slNome->Add("Patamar");
   slValor->Add("S(MVA)b2=>b1");
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      if (! fluxo[np]) continue;
      //flow:S(MVA) no sentido orig->dest
      slNome->Add(geral->PAT.NOME[np].LowerCase());
      slValor->Add(ComplexToStr("%4.3f", ptrecho->flow.p[1][np], ptrecho->flow.q[1][np]));
      }
   //flow:carregamento por patamar
   InsereLinhaSeparacao();
   slNome->Add("Patamar");
   slValor->Add("Cargto(%)");
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      slNome->Add( geral->PAT.NOME[np].LowerCase());
      slValor->Add(DoubleToStr("%2.1f", ptrecho->flow.cargto[np]*100));
      }
   //flow:perda(kW) por patamar
   InsereLinhaSeparacao();
   slNome->Add("Patamar");
   slValor->Add("Perda(kW)");
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      //determina perda em MVA
      perda.p = fabs(ptrecho->flow.p[0][np] + ptrecho->flow.p[1][np]);
      perda.q = fabs(ptrecho->flow.q[0][np] + ptrecho->flow.q[1][np]);
      //converte perdas de MVA p/ kVA
      perda.p *= 1000.;
		perda.q *= 1000.;
      slNome->Add(geral->PAT.NOME[np].LowerCase());
      slValor->Add(DoubleToStr("%4.3f", perda.p));
      }
   */
   }

//---------------------------------------------------------------------------
void __fastcall TAtributo::MontaYref(VTYref *yref)
   {
	//variáveis locais
	double          Sbase = 100;
	complex<double> y_pu, y_s;

   //monta atributos da classe base VTEqbar
	MontaEqbar(yref);
	//monta atributos de Yref
	//y_pu
	y_pu = yref->Y1_pu;
	slNome->Add("y(pu Sb100)");
	slValor->Add(ComplexToStr("%4.3f", y_pu.real(), y_pu.imag()));
	//y_siemens
	y_s = yref->Y1_s(Sbase, yref->pbarra->vnom);
	slNome->Add("y(s)");
	slValor->Add(ComplexToStr("%4.3f", y_s.real(), y_s.imag()));
   /*
   //S(MVA)
   slNome->Add("Patamar");
   slValor->Add("S(MVA)");
   for (int np = 0; np < geral->PAT.NUM_PAT; np++)
      {
      s_mva = yref->DemandaMVA(yref->pbarra->flow.v[np]);
      slNome->Add(geral->PAT.NOME[np].LowerCase());
      slValor->Add(ComplexToStr("%4.3f", s_mva.real(), s_mva.imag()));
      }
   */
   }

//---------------------------------------------------------------------------
void __fastcall TAtributo::NomeValor(int index, AnsiString &nome, AnsiString &valor)
   {
   //proteção
   if (index < slNome->Count)
      {
      nome  = slNome->Strings[index];
      valor = slValor->Strings[index];
      }
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TAtributo::Valor(AnsiString nome)
   {
   //procura 'nome' em slNome
   for (int n = 0; n < slNome->Count; n++)
      {
      if (slNome->Strings[n].CompareIC(nome) == 0) return(slNome->Strings[n]);
      }
   return("");
   }

//---------------------------------------------------------------------------
//eof
