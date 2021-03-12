//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaYref.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTYref.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaYref(VTApl *apl)
   {
   try{
      return (new TMontaYref(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaYref::TMontaYref(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   }

//---------------------------------------------------------------------------
__fastcall TMontaYref::~TMontaYref(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaYref::IniciaGrupo(VTEqpto *eqpto)
   {
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaYref::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Yref
   yref = (VTYref*)eqpto;
   //atributos de Eqpto
   TMontaEqpto::LisAtributosBasico(eqpto, lisATR);
   LisAtributosEqpto(eqpto, lisATR);
   //atributos de fluxo
   LisAtributosFluxo(eqpto, lisATR);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaYref::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Yref
   yref = (VTYref*)eqpto;
   //atributos de Eqpto
   TMontaEqbar::LisAtributosEqpto(eqpto, lisATR);
   //atributos de Yref
	lisATR->Add(new TAtributo(yref, yref_Y_PU100,  true, false,  "Admitância",   "(pu)"));
	lisATR->Add(new TAtributo(yref, yref_Y_SIEMEN, true, false,  "Admitância",   "(s)"));
	lisATR->Add(new TAtributo(yref, yref_S_MVA,    true, false,  "Pot.Aparente", "(MVA)"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaYref::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Yref
   yref = (VTYref*)eqpto;
   //atributos de fluxo Eqbar
   TMontaEqbar::LisAtributosEqpto(eqpto, lisATR);
   //atributos de fluxo de Yref

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaYref::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //monta lista de valores do Eqpto
   LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores de Fluxo
   LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaYref::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->yref   = (VTYref*)eqpto;
   try{//dados do Eqpto
      TMontaEqbar::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
      //atributos de Yref
		if (yref->Show[yref_Y_PU100])  ValorAdmitancia_PU100();
		if (yref->Show[yref_Y_SIEMEN]) ValorAdmitancia_SIEMEN();
		}catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaYref::LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->yref   = (VTYref*)eqpto;
   try{//dados do Eqpto
      TMontaEqbar::LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
      //dados de fluxo
      if (yref->Show[yref_S_MVA]) ValorPotAparente();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaYref::LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano)
   {
   //salva parâmetros
   this->yref  = (VTYref*)eqpto;
   this->lisEXT = lisEXT;
   this->ano    = ano;

   return(true);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMontaYref::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   AnsiString valor;

   return(valor);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaYref::ValorAdmitancia_PU100(void)
	{
	//variáveis locais
	complex<double> y1_pu;
	VTDado  *dado;

	//determina Y em pu
	y1_pu = yref->Y1_pu;
	//cria um novo Dado e insere em lisEXT
	lisEXT->Add(dado = NewObjDado("Yseq.1"));
	aue.Admitancia_PU(y1_pu, dado->valor, dado->unidade);
	}

//---------------------------------------------------------------------------
void __fastcall TMontaYref::ValorAdmitancia_SIEMEN(void)
	{
   //variáveis locais
	complex<double> y1_s;
	double Sbase = 100.;
	strIMP z1;
	VTDado *dado;

   //determina Z em pu/Sb=100
	y1_s = yref->Y1_s(Sbase, yref->pbarra->vnom);
	//cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Zseq.1"));
	aue.Admitancia_SIEMEN(y1_s, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaYref::ValorPotAparente(void)
   {
   //variáveis locais
   VTDado *dado;
   complex<double> s_mva;

   //determina Pot.Aparente de Yref
   s_mva = yref->DemandaMVA(num_pat);
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("S"));
   //define unidade e valor
   aue.PotAparente_MVA(s_mva, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
//eof

