//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TNo.h"
#include "VTBarra.h"
#include "VTCapacitor.h"
#include "VTCarga.h"
#include "VTFiltro.h"
#include "VTGerador.h"
#include "VTLigacao.h"
#include "VTReator.h"
#include "VTSuprimento.h"
#include "VTTrafoZZ.h"
#include "VTYref.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Fases.h"
#include "..\Constante\Trifasico.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTNo* __fastcall NewObjNo(VTBarra *pbarra, VTNet *net)
   {
   return(new TNo(pbarra, net));
   }

//---------------------------------------------------------------------------
__fastcall TNo::TNo(VTBarra *pbarra, VTNet *net)
   {
   //salva ponteiro para objeto Net
   this->net = net;
   //cria listas
   lisBAR  = new TList();
   lisCAP  = new TList();
   lisCAR  = new TList();
	lisGER  = new TList();
   lisLIG  = new TList();
	lisMED  = new TList();
   lisREA  = new TList();
   lisSUP  = new TList();
	lisZT   = new TList();
	lisBAT = new TList();
	//inicia dados
   redutivel    = true;
	PD.fases_lig = faseINV;
	PD.fases_trf = faseINV;
   //insere a Barra
   InsereBarra(pbarra);
   }

//---------------------------------------------------------------------------
__fastcall TNo::~TNo(void)
   {
   //variáveis locais
   VTBarra *barra;
   
   //cancela associação do No com suas Barras
   for (int n = 0; n < lisBAR->Count; n++)
      {
      barra = (VTBarra*)lisBAR->Items[n];
      barra->RemoveNo(this);
      }
   //destrói lista SEM destruir seus objetos
   if (lisBAR)  {delete lisBAR;  lisBAR  = NULL;}
   if (lisCAP)  {delete lisCAP;  lisCAP  = NULL;}
   if (lisCAR)  {delete lisCAR;  lisCAR  = NULL;}
   if (lisGER)  {delete lisGER;  lisGER  = NULL;}
   if (lisLIG)  {delete lisLIG;  lisLIG  = NULL;}
	if (lisMED)  {delete lisMED;  lisMED  = NULL;}
	if (lisREA)  {delete lisREA;  lisREA  = NULL;}
	if (lisSUP)  {delete lisSUP;  lisSUP  = NULL;}
   if (lisZT)   {delete lisZT;   lisZT   = NULL;}
   if (lisBAT)  {delete lisBAT; lisBAT = NULL;}


   }

//---------------------------------------------------------------------------
void __fastcall TNo::ConfiguraFases(VTEqpto *eqpto, int fases)
   {
   //proteção
   if (eqpto == NULL) return;
   //verifica se Ligacao é um Trafo ou Trafo3E
   if ((eqpto->Tipo() == eqptoTRAFO) || (eqpto->Tipo() == eqptoTRAFO3E))
      {//atualiza fases definidas por trafo
      if (PD.fases_trf == faseINV) {PD.fases_trf  = fases;}
      else                         {PD.fases_trf |= fases;}
      }
   else
      {//atualiza fases definidas pelas demais Ligacoes
      if (PD.fases_lig == faseINV) {PD.fases_lig  = fases;}
      else                         {PD.fases_lig |= fases;}
      }
   }

//---------------------------------------------------------------------------
void __fastcall TNo::ConfiguraFasesNT(void)
   {
   if (PD.fases_lig != faseINV) {PD.fases_lig |= faseNT;}
   if (PD.fases_trf != faseINV) {PD.fases_trf |= faseNT;}
   }

//---------------------------------------------------------------------------
bool __fastcall TNo::ExisteGeracao(void)
   {
   //verifica se há Gerador ou Suprimento no No
   return((lisSUP->Count + lisGER->Count) > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TNo::ExisteGeracao(int tipo_sup)
   {
   //variáveis locais
   VTGerador    *pger;
   VTSuprimento *psup;

   //procura Geradores do tipo PV
   for (int n = 0; n < lisGER->Count; n++)
      {
      pger = (VTGerador*)lisGER->Items[n];
      if (pger->tiposup == tipo_sup) return(true);
      }
	//procura Suprimentos do tipo PV
   for (int n = 0; n < lisSUP->Count; n++)
      {
      psup = (VTSuprimento*)lisSUP->Items[n];
      if (psup->tiposup == tipo_sup) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TNo::ExisteGeracaoPV(void)
   {
   return(ExisteGeracao(supPV));
   }

//---------------------------------------------------------------------------
bool __fastcall TNo::ExisteGeracaoVF(void)
   {
   return(ExisteGeracao(supVF));
   }

//---------------------------------------------------------------------------
void __fastcall TNo::InsereBarra(VTBarra *pbarra)
   {
   //proteção
   if (pbarra == NULL) return;
   //verifica se a Barra já existe no Nó
   if (lisBAR->IndexOf(pbarra) >= 0) return;
   //insere Barra no Nó
   lisBAR->Add(pbarra);
   //CopiaTList(pbarra->lisCAP, lisCAP);
   //CopiaTList(pbarra->lisCAR, lisCAR);
   //CopiaTList(pbarra->lisGER, lisGER);
   //CopiaTList(pbarra->lisSUP, lisSUP);
   //associa Barra c/ este Nó
   pbarra->InsereNo(this);
   //se necessário, define Barra pai do Nó
   if (pai == NULL) pai = pbarra;
   }

//---------------------------------------------------------------------------
void __fastcall TNo::InsereLigacao(VTLigacao *ligacao)
   {
   //proteção
   if (ligacao == NULL) return;
   //insere Chave na lista
   if (lisLIG->IndexOf(ligacao) < 0) lisLIG->Add(ligacao);
   }

//---------------------------------------------------------------------------
void __fastcall TNo::InsereEqbar(VTEqbar *eqbar)
   {
   //proteção
   if (eqbar == NULL) return;
   switch(eqbar->Tipo())
      {
      case eqptoCAPACITOR:  lisCAP->Add(eqbar);  break;
	  case eqptoCARGA:      lisCAR->Add(eqbar);  break;
		case eqptoGERADOR:    lisGER->Add(eqbar);  break;
		case eqptoMEDIDOR:    lisMED->Add(eqbar);  break;
		case eqptoREATOR:     lisREA->Add(eqbar);  break;
      case eqptoSUPRIMENTO: lisSUP->Add(eqbar);  break;
		case eqptoFILTRO:     lisZT->Add(eqbar);   break;
		case eqptoYREF:       lisZT->Add(eqbar);   break;
		case eqptoTRAFOZZ:    lisZT->Add(eqbar);   break;
	  case eqptoBATERIA:    lisBAT->Add(eqbar);  break;
		}
   }

//---------------------------------------------------------------------------
void __fastcall TNo::LisEqbar(VTBarra *pbarra, TList *lisEQP)
   {
   //variáveis locais
   VTEqbar *eqbar;
	TList   *list[] = {lisCAP,lisCAR, lisGER, lisMED, lisREA, lisSUP, lisZT};

   //loop p/ todas listas
	for (int nl = 0; nl < sizeof(list)/sizeof(TList*); nl++)
      {
		for (int n = 0; n < list[nl]->Count; n++)
         {
         eqbar = (VTEqbar*)list[nl]->Items[n];
         if (eqbar->pbarra == pbarra)
            {
            if (lisEQP->IndexOf(eqbar) < 0) lisEQP->Add(eqbar);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TNo::LisGerador(TList *lisEQP)
   {
   CopiaTList(lisGER, lisEQP);
   }

//---------------------------------------------------------------------------
void __fastcall TNo::LisSuprimento(TList *lisEQP)
   {
   CopiaTList(lisSUP, lisEQP);
   }

//---------------------------------------------------------------------------
int  __fastcall TNo::NumEqbar(void)
   {
	return(lisCAP->Count + lisCAR->Count + lisGER->Count + lisMED->Count +
          lisREA->Count + lisSUP->Count + lisZT->Count);
   }

//---------------------------------------------------------------------------
int  __fastcall TNo::NumEqbar(VTBarra *pbarra)
   {
   //variáveis locais
   int     count = 0;
   VTEqbar *eqbar;
	TList   *list[] = {lisCAP,lisCAR, lisGER, lisMED, lisREA, lisSUP, lisZT};

   //loop p/ todas listas
	for (int nl = 0; nl < sizeof(list)/sizeof(TList*); nl++)
      {
		for (int n = 0; n < list[nl]->Count; n++)
         {
         eqbar = (VTEqbar*)list[nl]->Items[n];
         if (eqbar->pbarra == pbarra) count++;
         }
      }
   return(count);
   }

//---------------------------------------------------------------------------
int  __fastcall TNo::NumSupGerPQ(void)
   {
   //variáveis locais
   VTGerador    *pger;
   VTSuprimento *psup;
   int           numPQ = 0;

   //procura Geradores do tipo PV
   for (int n = 0; n < lisGER->Count; n++)
      {
      pger = (VTGerador*)lisGER->Items[n];
      if (pger->tiposup == supPQ) numPQ++;
      }
   //procura Suprimentos do tipo PV
   for (int n = 0; n < lisSUP->Count; n++)
      {
      psup = (VTSuprimento*)lisSUP->Items[n];
      if (psup->tiposup == supPQ) numPQ++;
      }
   return(numPQ);
   }

//---------------------------------------------------------------------------
int  __fastcall TNo::NumSupGerPV(void)
   {
   //variáveis locais
   VTGerador    *pger;
   VTSuprimento *psup;
   int           numPV = 0;

   //procura Geradores do tipo PV
   for (int n = 0; n < lisGER->Count; n++)
      {
      pger = (VTGerador*)lisGER->Items[n];
      if (pger->tiposup == supPV) numPV++;
      }
   //procura Suprimentos do tipo PV
   for (int n = 0; n < lisSUP->Count; n++)
      {
      psup = (VTSuprimento*)lisSUP->Items[n];
      if (psup->tiposup == supPV) numPV++;
      }
   return(numPV);
   }

//---------------------------------------------------------------------------
int  __fastcall TNo::NumSupGerPV_VF(void)
   {
   return(NumSupGerPV() + NumSupGerVF());
   }

//---------------------------------------------------------------------------
int  __fastcall TNo::NumSupGerVF(void)
   {
   //variáveis locais
   VTGerador    *pger;
   VTSuprimento *psup;
   int           numVF = 0;

   //procura Geradores do tipo VF
   for (int n = 0; n < lisGER->Count; n++)
      {
      pger = (VTGerador*)lisGER->Items[n];
      if (pger->tiposup == supVF) numVF++;
      }
   //procura Suprimentos do tipo PV
   for (int n = 0; n < lisSUP->Count; n++)
      {
      psup = (VTSuprimento*)lisSUP->Items[n];
      if (psup->tiposup == supVF) numVF++;
      }
   return(numVF);
   }
/*
//---------------------------------------------------------------------------
int& __fastcall TNo::operator|(int &fases)
	{
	//proteção
	if (fases == faseINV)return(PD.fases);
	//verifica se ainda não há fase definida
	if (PD.fases == faseINV) PD.fases = fases;
	//atualiza fases do No
	PD.fases |= fases;
	//retorna fases
	return(PD.fases);
   }
*/

//---------------------------------------------------------------------------
int __fastcall TNo::PM_GetFases(void)
	{
   //variáveis locais
   int fases = PD.fases_lig;

   if      (PD.fases_lig == faseINV) {fases  = PD.fases_trf;}
   else if (PD.fases_trf != faseINV) {fases |= PD.fases_trf;}

	return(fases);
	}

//---------------------------------------------------------------------------
int __fastcall TNo::PM_GetModeloCarga(void)
	{
   //variáveis locais
   VTCarga *carga;

   //verifica se há Cargas
   if (lisCAR->Count == 0) return(cargaSCTE);
   //assume modelo da primeira Carga
   carga = (VTCarga*)lisCAR->First();
   return(carga->ModeloCarga);
	}

/*
//---------------------------------------------------------------------------
void __fastcall TNo::PM_SetFases(int fases)
	{
	PD.fases = fases;
	}
*/

//---------------------------------------------------------------------------
void __fastcall TNo::RemoveEqbar(VTEqbar *eqbar)
   {
   //proteção
   if (eqbar == NULL) return;
   switch(eqbar->Tipo())
      {
      case eqptoCAPACITOR:  lisCAP->Remove(eqbar);   break;
		case eqptoCARGA:      lisCAR->Remove(eqbar);   break;
      case eqptoGERADOR:    lisGER->Remove(eqbar);   break;
		case eqptoMEDIDOR:    lisMED->Remove(eqbar);   break;
		case eqptoREATOR:     lisREA->Remove(eqbar);   break;
		case eqptoSUPRIMENTO: lisSUP->Remove(eqbar);   break;
		case eqptoFILTRO:     lisZT->Remove(eqbar);  break;
		case eqptoTRAFOZZ:    lisZT->Remove(eqbar);  break;
		case eqptoYREF:       lisZT->Remove(eqbar);  break;
		case eqptoBATERIA:    lisBAT->Remove(eqbar); break;
		default: break;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TNo::Swing(void)
   {
   return(flow.tipo_barra == barraSWING);
   /*
   //variáveis locais
   VTBarra *pbarra;

   //verifica se há alguma Barra Swing no Nó
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      pbarra = (VTBarra*)lisBAR->Items[nb];
      if (pbarra->Swing()) return(true);
      }
   //não há Barras Swing
   return(false);
   */
   }

//---------------------------------------------------------------------------
int __fastcall TNo::Tipo(void)
   {
   return(eqptoNO);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TNo::TipoAsString(void)
   {
   return("No");
   }

//---------------------------------------------------------------------------
int __fastcall TNo::TipoDeBarra(void)
   {
   return(flow.tipo_barra);
	}
/*
//---------------------------------------------------------------------------
bool __fastcall TNo::ValidaFasesTrafo(void)
	{
	//variáveis locais
   int fases_lig, fases_trf;

   //OBS: esta função não funciona quando o trafo alimenta um trecho com menos fases do que ele,
   //     por exemplo, um trafo D/Y com rede secundária ABN

   //verifica se há fases definidas a partir de trafos
   if (PD.fases_trf == faseINV) return(true);
   //verifica se há fases definidas por outreos Eqptos
   if (PD.fases_lig == faseINV) return(true);
   //verifica se as fases definidas por Trafos são compatíveis com as fases definidas pelos demais Eqptos
   fases_lig = PD.fases_lig & faseABC;
   fases_trf = PD.fases_trf & faseABC;
   return((fases_lig & fases_trf) == fases_trf);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TNo::ValidaFasesTrafo(void)
	{
	return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TNo::Y0ref_pu(double Sbase, double Vbase, complex<double> &y0)
	{
	//variáveis locais
	int        num_eqpto = 0;
	strIMP     z0;
	VTEqpto   *eqpto;
	VTFiltro  *filtro;
	VTTrafoZZ *trafoZZ;
	VTYref    *yref;

	//somatória das admitâncias de Filtro, TrafoZZ e Yref

	//inicia y0 c/ valor nulo
	y0 = CZero();
	//loop p/ Eqptos em lisZT
	for (int n = 0; n < lisZT->Count; n++)
		{
		eqpto = (VTEqpto*)lisZT->Items[n];
		switch(eqpto->Tipo())
			{
			case eqptoFILTRO:	//obtém z0 de Filtro em pu
				filtro =(VTFiltro*)eqpto;
				filtro->Z0_pu(Sbase, z0);
				break;
			case eqptoTRAFOZZ:	//obtém z0 de TrafoZZ em pu
				trafoZZ =(VTTrafoZZ*)eqpto;
				trafoZZ->Z0_pu(Sbase, z0);
				break;
			case eqptoYREF:	//obtém z0 de Zref em pu
				yref  = (VTYref*)eqpto;
				yref->Z0_pu(z0);
				break;
			default: continue;
			}
		//acumula admitância
		if ((fabs(z0.r) > 0.)||(fabs(z0.x) > 0.))
			{
			y0 +=  1./complex<double>(z0.r, z0.x);
			//incrementa contador de equipamentos
			num_eqpto++;
			}
		}
	return(num_eqpto > 0);
	}

//---------------------------------------------------------------------------
bool __fastcall TNo::Y1ref_pu(double Sbase, double Vbase, complex<double> &y1)
	{
	//variáveis locais
	int      num_eqpto = 0;
	VTEqpto *eqpto;
	VTYref  *yref;

	//somatória das admitâncias de Yref (pois Filtro e TrafoZZ não tem z1)

	//inicia y1 c/ valor nulo
	y1 = CZero();
	//loop p/ Eqptos em lisZT
	for (int n = 0; n < lisZT->Count; n++)
		{
		eqpto = (VTEqpto*)lisZT->Items[n];
		if (eqpto->Tipo() != eqptoYREF) continue;
		yref = (VTYref*)eqpto;
		//acumula admitância
		if (IsComplexZero(yref->Y1_pu)) continue;
		//acumula admitância
		y1 +=  yref->Y1_pu;
		//incrementa contador de equipamentos
		num_eqpto++;
		}
	return(num_eqpto > 0);
	}

//---------------------------------------------------------------------------
bool __fastcall TNo::Z0ref_pu(double Sbase, double Vbase, complex<double> &z0)
	{
	//variáveis locais
	complex<double> y0;

   //reinicia z1
   z0 = complex<double>(1.e12, 1.e12);
   //calcula admitância p/ terra
   if (! Y0ref_pu(Sbase, Vbase, y0)) return(false);
   //calcula impdância p/ terra
   if (Abs(y0) > 0.)
      {
      z0 = 1./y0;
      return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TNo::Z1ref_pu(double Sbase, double Vbase, complex<double> &z1)
   {
   //variáveis locais
   complex<double> y1;

   //reinicia z1
   z1 = complex<double>(1.e12, 1.e12);
   //calcula admitância p/ terra
   if (! Y1ref_pu(Sbase, Vbase, y1)) return(false);
   //calcula impedância p/ terra
   if (Abs(y1) > 0.) 
      {
      z1 = 1./y1;
      return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TNo::Zeq(double Sbase, complex<double> &zeq0, complex<double> &zeq1)
   {
   //variáveis locais
   VTGerador       *pger;
   VTSuprimento    *psup;
   complex<double> pcc_3f, pcc_ft;
   complex<double> z0, z1;
   complex<double> yeq0(0., 0.), yeq1(0., 0.);

   //proteção: verifica se a Barra possui Geradores ou Suprimentos
   if ((lisGER->Count == 0)&&(lisSUP->Count == 0))
      {//retorna valores nulos p/ zeq0 e zeq1
		zeq1 = zeq0 = CZero();
      return(true);
      }
   //loop p/ todos os Geradores
   for (int n = 0; n < lisGER->Count; n++)
      {
      pger  = (VTGerador*)lisGER->Items[n];
      z1    = complex<double>(pger->z1.r, pger->z1.x)*(Sbase/pger->snom);
      z0    = complex<double>(pger->z0.r, pger->z0.x)*(Sbase/pger->snom);
      yeq1 += 1./z1;
      yeq0 += 1./z0;
      }
   //loop p/ todos os Suprimentos
   for (int n = 0; n < lisSUP->Count; n++)
      {
      psup = (VTSuprimento*)lisSUP->Items[n];
      //verifica qual o dado definido: potência de curto ou zeq
      if (((psup->pcc_3f.p == 0.)&&(psup->pcc_3f.q == 0.))||
          ((psup->pcc_ft.p == 0.)&&(psup->pcc_ft.q == 0.)))
         {//assume as zeq do suprimento
         z1 = complex<double>(psup->zeq1.r, psup->zeq1.x);
         z0 = complex<double>(psup->zeq0.r, psup->zeq0.x);
         }
      else
         {//calcula z1 e z0 a partir das potência de curto
         pcc_3f = complex<double>(psup->pcc_3f.p, psup->pcc_3f.q);
         pcc_ft = complex<double>(psup->pcc_ft.p, psup->pcc_ft.q);
         z1     = Sbase/conj(pcc_3f);
         z0     = ((3*Sbase)/conj(pcc_ft)) - (z1+z1);
         }
      //proteção: verifica se impedâncias de seqüência são nulas
      if (abs(z0+z1) == 0.)
         {
         //Erro(psup->codigo + ": Suprimento sem informação de potência de curto.");
         return(false);
         }
      yeq1  += 1./z1;
      yeq0  += 1./z0;
      }
   //calcula valores a serem retornados: z0 e z1
   zeq0 = 1./yeq0;
   zeq1 = 1./yeq1;
   return(true);
   }

//---------------------------------------------------------------------------
//eof
