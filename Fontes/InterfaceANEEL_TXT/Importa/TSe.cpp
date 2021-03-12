//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TSe.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TSe* __fastcall NewObjSe(AnsiString codigo)
   {
   try{
		return(new TSe(codigo));
      }catch(Exception &e)
         {
         }
   //erro na cria��o do objeto
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TSe::TSe(AnsiString codigo)
	{
	//salvca c�digo
	this->codigo = codigo;
	//cria listas
	lisEQP     = new TList();
	lisBAR_MT  = new TList();
	lisREDE_MT = new TList();
	}

//---------------------------------------------------------------------------
__fastcall TSe::~TSe(void)
   {
	//destr�i lista sem destruir seus objetos
	if (lisEQP)     {delete lisEQP;     lisEQP     = NULL;}
	if (lisBAR_MT)  {delete lisBAR_MT;  lisBAR_MT  = NULL;}
	if (lisREDE_MT) {delete lisREDE_MT; lisREDE_MT = NULL;}
	}

//---------------------------------------------------------------------------
VTBarra* __fastcall TSe::ExisteBarraMT(double vnom_kv)
	{
	//vari�veis locais
	VTBarra *barra;

	for (int n = 0; n < lisBAR_MT->Count; n++)
		{
		barra = (VTBarra*)lisBAR_MT->Items[n];
		if (IsDoubleZero(barra->vnom - vnom_kv)) return(barra);
		}
	return(NULL);
	}

//---------------------------------------------------------------------------
bool __fastcall TSe::InsereBarraAT(VTBarra *barra)
	{
	this->barraAT = barra;

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TSe::InsereBarraMT(VTBarra *barra)
	{
	//prote��o
	if (barra == NULL) return(false);
	if (lisBAR_MT->IndexOf(barra) < 0)
		{
		lisBAR_MT->Add(barra);
		}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TSe::InsereRedeSE(VTRede *redeSE)
	{
	//salva ponteiro para rede
	this->redeSE = redeSE;

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TSe::InsereRedeMT(VTRede *redeMT)
	{
	//prote��o
	if (redeMT == NULL) return(false);
	//insere rede em lisREDE_MT
	if (lisREDE_MT->IndexOf(redeMT) < 0)
		{
		lisREDE_MT->Add(redeMT);
		}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TSe::IntegraComRedesPRI(void)
	{
	//vari�veis locais
	//VTBarra   *barra, *barra_ini, *barraMT;
	VTBarra   *barra_ini, *barraMT;
	VTEqbar   *eqbar;
	VTLigacao *ligacao;
	VTRede    *redeMT;

	//inicia lisEQP com todos os Suprimentos definidos para as redes prim�rias
	lisEQP->Clear();
	for (int n = 0; n < lisREDE_MT->Count; n++)
		{
		redeMT = (VTRede*)lisREDE_MT->Items[n];
		if ((barra_ini = redeMT->BarraInicial()) != NULL)
			{
			barra_ini->LisEqbar(lisEQP, eqptoSUPRIMENTO);
			}
		}
	//transfere primeiro Suprimento de lisEQP da sua Barra atual para a Barra AT da Rede da Se
	if (lisEQP->Count > 0)
		{//retira primeiro Suprimento de lisEQP
		eqbar = (VTEqbar*)lisEQP->First();
		lisEQP->Delete(0);
		//transfere Suprimento da sua Barra atual para a Barra AT da Rede da Se
		eqbar->pbarra->RemoveEqbar(eqbar);
		barraAT->InsereEqbar(eqbar);
		}
	//destr�i todos os Suprimento restantes em lisEQP
	for (int n = 0; n < lisEQP->Count; n++)
		{
		eqbar = (VTEqbar*)lisEQP->Items[n];
		eqbar->pbarra->RemoveEqbar(eqbar);
		}
	if (lisEQP->Count > 0) LimpaTList(lisEQP);
	//redefine Barra inicial de todas as Redes prim�rias com uma Barra da Se
	for (int n = 0; n < lisREDE_MT->Count; n++)
		{
		redeMT = (VTRede*)lisREDE_MT->Items[n];
		//determina Barra inicial
		if ((barra_ini = redeMT->BarraInicial()) == NULL) continue;
		//determina Barra da Se que ser� a nova Barra inicial da Rede prim�ria
		if ((barraMT = ExisteBarraMT(barra_ini->vnom)) == NULL)continue;
		//redefine coordenadas da BarraMT
		barraMT->utm = barra_ini->utm;
		barraMT->esq = barra_ini->esq;
		//redefine coordenadas da BarraAT iguais �s coordenadas da BarraMT
		barraAT->utm = barraMT->utm;
		barraAT->esq = barraMT->esq;
		//acerta Ligacoes que conectavam a Barra inicial da Rede prim�ria
		lisEQP->Clear();
		redeMT->LisLigacao(lisEQP, barra_ini);
		for (int n= 0; n < lisEQP->Count; n++)
			{
			ligacao = (VTLigacao*)lisEQP->Items[n];
			if (ligacao->pbarra1 == barra_ini) {ligacao->pbarra1 = barraMT; continue;}
			if (ligacao->pbarra2 == barra_ini) {ligacao->pbarra2 = barraMT; continue;}
			if (ligacao->pbarra3 == barra_ini) {ligacao->pbarra3 = barraMT; continue;}
			}
		//redefine Barra inicial da RedeMT
		redeMT->DefineBarraInicial(barraMT);
		/*
		//elimina barra_ini
		redes->RemoveBarra(barra_ini);
		lisEQP->Clear();
		lisEQP->Add(barra_ini);
		LimpaTList(lisEQP);
		*/
		}
	return(true);
	}

//---------------------------------------------------------------------------
//eof