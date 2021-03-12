//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMalha.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTLIgacao.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMalha* __fastcall NewObjMalha(void)
   {
   try{
      return (new TMalha());
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TMalha::TMalha(void)
   {
   //cria objetos locais
   lisBAR = new TList();
	lisLIG = new TList();
	lisREDE = new TList();
	}

//---------------------------------------------------------------------------
__fastcall TMalha::~TMalha(void)
   {
   //destrói lista sem destruir seus objetos
	if (lisBAR)  {delete lisBAR;  lisBAR  = NULL;}
	if (lisLIG)  {delete lisLIG;  lisLIG  = NULL;}
	if (lisREDE) {delete lisREDE; lisREDE = NULL;}
	}

//---------------------------------------------------------------------------
bool __fastcall TMalha::ExisteLigacaoComum(VTMalha *malha)
	{
   //variáveis locais
   VTLigacao *ligacao;
   TList     *lisEXT =  malha->LisLigacao();

   //loop para todas Ligacoes em lisEXT
   for (int n = 0; n < lisEXT->Count; n++)
      {
      ligacao = (VTLigacao*)lisEXT->Items[n];
	  if (lisLIG->IndexOf(ligacao) >= 0)
	  return(true);
      }
   return(false);
	}

//---------------------------------------------------------------------------
VTRede* __fastcall TMalha::FirstRede(void)
	{
	//variáveis locais
	VTRede *rede = NULL;

	//determina primeira Rede
	if (lisREDE->Count > 0)
		{
		rede = (VTRede*)lisREDE->First();
		}
	return(rede);
	}

//---------------------------------------------------------------------------
void __fastcall TMalha::IncorporaMalha(VTMalha *malha)
	{
   //variáveis locais
   VTLigacao *ligacao;
   TList     *lisEXT =  malha->LisLigacao();

   //loop para todas Ligacoes em lisEXT
   for (int n = 0; n < lisEXT->Count; n++)
      {
      ligacao = (VTLigacao*)lisEXT->Items[n];
      if (lisLIG->IndexOf(ligacao) < 0)
         {//insere Ligacao em lisLIG
         lisLIG->Add(ligacao);
         }
      }
	//reinicia lisBARRA e lisREDE
	IniciaLisBarra();
	IniciaLisRede();
	}

//---------------------------------------------------------------------------
void __fastcall TMalha::IniciaLisBarra(void)
	{
	//variáveis locais
	VTLigacao *ligacao;

	//rfeinicia lisBAR
	lisBAR->Clear();
	//loop para todas Ligacoes em lisLIG
	for (int n = 0; n < lisLIG->Count; n++)
		{
		ligacao = (VTLigacao*)lisLIG->Items[n];
		ligacao->LisBarra(lisBAR);
		}
	}

//---------------------------------------------------------------------------
void __fastcall TMalha::IniciaLisRede(void)
	{
	//variáveis locais
	TList     *lisEXT;
	VTLigacao *ligacao;
	VTRede    *rede;

	//reinicia lisREDE
	lisREDE->Clear();
	//loop para todas Ligacoes em lisLIG
	for (int n = 0; n < lisLIG->Count; n++)
		{
		ligacao = (VTLigacao*)lisLIG->Items[n];
		//protecao
		if (ligacao->rede == NULL) continue;
		//verifica se a Rede não está carregada
		if (ligacao->rede->Carregada)
			{//insere Rede em lisREDE
			if (lisREDE->IndexOf(ligacao->rede) < 0) lisREDE->Add(ligacao->rede);
			}
		else
			{//obtém lista de Redes da MRede equivalente
			lisEXT = ligacao->rede->MRede->LisRede();
			for (int nr = 0; nr < lisEXT->Count; nr++)
				{
				rede = (VTRede*)lisEXT->Items[nr];
				//insere Rede em lisREDE
				if (lisREDE->IndexOf(rede) < 0) lisREDE->Add(rede);
				}
			}
		}
	}

//---------------------------------------------------------------------------
bool __fastcall TMalha::InsereLigacao(TList *lisEXT)
	{
	//variáveis locais
	VTLigacao *ligacao, *ligacao_esq;
	VTReducao *reducao;
	TList     *lisLIG_ESQ;

	//reinicia listas
	lisBAR->Clear();
	lisLIG->Clear();
	lisREDE->Clear();
	//copia Ligacoes da lista recebida: cuidado pode haver Ligacao de Reducao
	for (int n = 0; n < lisEXT->Count; n++)
		{
		ligacao = (VTLigacao*)lisEXT->Items[n];
		if (ligacao->rede->Carregada)
			{//insere Ligacao em lisLIG
			lisLIG->Add(ligacao);
			}
		else
			{//substitui Ligacao de Reducao pelas Ligacoes do esquemático da Reducao
			reducao    = (VTReducao*)ligacao->rede->MRede->LisReducao()->First();
			lisLIG_ESQ = reducao->LisLigacaoEsquematico();
			for (int i = 0; i < lisLIG_ESQ->Count; i++)
				{
				ligacao_esq = (VTLigacao*)lisLIG_ESQ->Items[i];
				if (lisLIG->IndexOf(ligacao_esq) < 0)
					{
					lisLIG->Add(ligacao_esq);
					}
				}
			}
		}
	//reinicia lista de barras e Redes
	IniciaLisBarra();
	IniciaLisRede();
	return(true);
	}

//---------------------------------------------------------------------------
TList* __fastcall TMalha::LisBarra(void)
	{
	return(lisBAR);
	}

//---------------------------------------------------------------------------
TList* __fastcall TMalha::LisLigacao(void)
	{
	return(lisLIG);
	}

//---------------------------------------------------------------------------
TList* __fastcall TMalha::LisRede(void)
	{
	return(lisREDE);
	}

//---------------------------------------------------------------------------
int __fastcall TMalha::NumeroDeLigacoes(VTRede *rede, int tipo)
	{
   //variáveis locais
	int        count = 0;
	VTLigacao *ligacao;

   //verifica se foi definido um tipo de ligacao
   if (tipo < 0) tipo = eqptoLIGACAO;
   //totaliza Ligacoes do tipo indicado
   for (int n = 0; n < lisLIG->Count; n++)
      {
		ligacao = (VTLigacao*)lisLIG->Items[n];
		if ((tipo == eqptoLIGACAO)||(ligacao->Tipo() == tipo))
			{//verifica se Eqpto pertence à rede indicada
			if ((rede == NULL)||(ligacao->rede == rede)) count++;
			}
      }
   return(count);
   }

//---------------------------------------------------------------------------
int __fastcall TMalha::NumeroDeRedes(void)
	{
	return(lisREDE->Count);
   }
//---------------------------------------------------------------------------
//eof
