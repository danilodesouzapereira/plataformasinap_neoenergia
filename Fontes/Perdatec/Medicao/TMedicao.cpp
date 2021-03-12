//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMedicao.h"
#include "..\Rede\VTRede.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TMedicao::TMedicao(VTRede *rede)
   {
   //inicia dados
   this->rede                = rede;
   if(rede != NULL) cod_rede = rede->Codigo;
   else             cod_rede = "";
   valida					 = true;
   level                     = -1;
   energia.medida_mwhmes     = 0;
   energia.carga_ini_mwhmes  = 0;
   energia.carga_fim_mwhmes  = 0;
   perda.tecnica_mwhmes      = 0;
   alocacao_perda_comercial.sdbt_pu = 0.5;
   alocacao_perda_comercial.sdmt_pu = 0.5;
   alocacao_perda_comercial.valida = true;
   //cria listas
   lisREDE_AJUS = new TList();
   lisREDE_FIXA = new TList();
   lisCAR_BT_AJUS  = new TList();
   lisCAR_BT_FIXA  = new TList();
   lisCAR_MT_AJUS  = new TList();
   lisCAR_MT_FIXA  = new TList();
   //cria listas auxiliares
   A3a.lisCAR = new TList();
   A4.lisCAR = new TList();
   outros.lisCAR = new TList();
   B.lisCAR = new TList();
   //inicializa com zero
   A3a.energia.carga_ini_mwhmes = 0.;
   A3a.energia.carga_fim_mwhmes = 0.;
   A4.energia.carga_ini_mwhmes = 0.;
   A4.energia.carga_fim_mwhmes = 0.;
   outros.energia.carga_ini_mwhmes = 0.;
   outros.energia.carga_fim_mwhmes = 0.;
   B.energia.carga_ini_mwhmes = 0.;
   B.energia.carga_fim_mwhmes = 0.;
   }

//---------------------------------------------------------------------------
__fastcall TMedicao::~TMedicao(void)
   {
   //destrói listas sem destruir seus objetos
   if (lisREDE_AJUS)   {delete lisREDE_AJUS;   lisREDE_AJUS   = NULL;}
   if (lisREDE_FIXA)   {delete lisREDE_FIXA;   lisREDE_FIXA   = NULL;}
   if (lisCAR_BT_AJUS) {delete lisCAR_BT_AJUS; lisCAR_BT_AJUS = NULL;}
   if (lisCAR_BT_FIXA) {delete lisCAR_BT_FIXA; lisCAR_BT_FIXA = NULL;}
   if (lisCAR_MT_AJUS) {delete lisCAR_MT_AJUS; lisCAR_MT_AJUS = NULL;}
   if (lisCAR_MT_FIXA) {delete lisCAR_MT_FIXA; lisCAR_MT_FIXA = NULL;}
   if (A3a.lisCAR)	   {delete A3a.lisCAR;     A3a.lisCAR     = NULL;}
   if (A4.lisCAR) 	   {delete A4.lisCAR;      A4.lisCAR      = NULL;}
   if (outros.lisCAR)  {delete outros.lisCAR;  outros.lisCAR = NULL;}
   if (B.lisCAR) 	   {delete B.lisCAR;  	   B.lisCAR 	 = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TMedicao::Clear(void)
   {
   //limpa listas
   lisREDE_AJUS->Clear();
   lisREDE_FIXA->Clear();
   lisCAR_BT_AJUS->Clear();
   lisCAR_BT_FIXA->Clear();
   lisCAR_MT_AJUS->Clear();
   lisCAR_MT_FIXA->Clear();
   A3a.lisCAR->Clear();
   A4.lisCAR->Clear();
   outros.lisCAR->Clear();
   B.lisCAR->Clear();
   //limpa ponteiro da rede
   rede = NULL;
   }
//---------------------------------------------------------------------------
double   __fastcall  TMedicao::FatorCargaMT_perc(double energia_cargas)
{
    double perdantBt_perc = 0;
	double perdantMt_perc = 0;
	double cargaMT_ini_mwhmes = 0;
	double k = 0;

	perdantBt_perc = PerdaComercialB_perc();
	perdantMt_perc = PerdaComercial_perc() - perdantBt_perc;
	//soma as cargas mt
	cargaMT_ini_mwhmes =  A3a.energia.carga_ini_mwhmes
						+ A4.energia.carga_ini_mwhmes
						+ outros.energia.carga_ini_mwhmes;
	//divide a parte MT proporcional a energia das cargas mt
	if (!IsDoubleZero(cargaMT_ini_mwhmes))
	{
		k = energia_cargas / cargaMT_ini_mwhmes;
	}

	return(k * perdantMt_perc);
}

//---------------------------------------------------------------------------
double __fastcall TMedicao::PerdaComercial_perc(void)
   {
   return(PerdaTotal_perc() - PerdaTecnica_perc());
   }
//---------------------------------------------------------------------------
double __fastcall  TMedicao::PerdaComercialA3a_perc(void)
{
	double valor = 0;

	//divide a parte MT proporcional a energia das cargas mt
//	valor = PerdaComercial_perc() * FatorCargaMT_perc(A3a.energia.carga_ini_mwhmes);
	valor = PerdaNT_perc(A3a.energia.carga_fim_mwhmes, A3a.energia.carga_ini_mwhmes);
	return(valor);
}
//---------------------------------------------------------------------------
double __fastcall  TMedicao::PerdaComercialA4_perc(void)
{
    double valor = 0;

//	valor = PerdaComercial_perc() * FatorCargaMT_perc(A4.energia.carga_ini_mwhmes);
	valor = PerdaNT_perc(A4.energia.carga_fim_mwhmes, A4.energia.carga_ini_mwhmes);
	return(valor);
}
//---------------------------------------------------------------------------
double __fastcall  TMedicao::PerdaComercialB_perc(void)
{
	double valor = 0;

//	valor = PerdaComercial_perc()*alocacao_perda_comercial.sdbt_pu;
	valor = PerdaNT_perc(B.energia.carga_fim_mwhmes, B.energia.carga_ini_mwhmes);
	return(valor);
}
//---------------------------------------------------------------------------
double __fastcall  TMedicao::PerdaComercialOutros_perc(void)
{
	double valor = 0;

//	valor = PerdaComercial_perc() * FatorCargaMT_perc(outros.energia.carga_ini_mwhmes);
	valor = PerdaNT_perc(outros.energia.carga_fim_mwhmes, outros.energia.carga_ini_mwhmes);
	return(valor);
}
//---------------------------------------------------------------------------
double __fastcall TMedicao::PerdaNT_perc(double energia_fim, double  energia_ini)
{
	double perdaNT_tot_mwhmes = 0;
	double perdaNT_parc_mwhmes = 0;
	double fator = 0;

	perdaNT_tot_mwhmes = (A3a.energia.carga_fim_mwhmes - A3a.energia.carga_ini_mwhmes)
						+ (A4.energia.carga_fim_mwhmes - A4.energia.carga_ini_mwhmes)
						+ (outros.energia.carga_fim_mwhmes - outros.energia.carga_ini_mwhmes)
						+ (B.energia.carga_fim_mwhmes - B.energia.carga_ini_mwhmes);
	perdaNT_parc_mwhmes = energia_fim - energia_ini;

	if (!IsDoubleZero(perdaNT_tot_mwhmes))
	{
		fator = perdaNT_parc_mwhmes / perdaNT_tot_mwhmes;
	}

	return fator*PerdaComercial_perc();
}
//---------------------------------------------------------------------------
double __fastcall TMedicao::PerdaTecnica_perc(void)
	{
   //variáveis locais
   double  perda_perc;

   if (IsDoubleZero(energia.medida_mwhmes))
      {
      perda_perc = 0.;
      }
   else
      {
      perda_perc = (perda.tecnica_mwhmes / energia.medida_mwhmes) * 100.;
      }
   return(perda_perc);
   }

//---------------------------------------------------------------------------
double __fastcall TMedicao::PerdaTotal_mwhmes(void)
   {
   return(energia.medida_mwhmes - energia.carga_ini_mwhmes);
   }

//---------------------------------------------------------------------------
double __fastcall TMedicao::PerdaTotal_perc(void)
   {
   //variáveis locais
   double  perda_perc;

   if (IsDoubleZero(energia.medida_mwhmes))
      {
      perda_perc = 0.;
      }
   else
      {
      perda_perc = (PerdaTotal_mwhmes() / energia.medida_mwhmes) * 100.;
      }
   return(perda_perc);
   }

//---------------------------------------------------------------------------
//eof

