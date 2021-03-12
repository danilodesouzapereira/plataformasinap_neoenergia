//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMalhas.h"
#include "VTMalha.h"
#include "TMontaMalha.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMalhas* __fastcall NewObjMalhas(VTApl *apl)
   {
   try{
      return (new TMalhas(apl));
      }catch (Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TMalhas::TMalhas(VTApl *apl)
   {
	//salva ponteiro p/ objeto apl
	this->apl = apl;
	//cria objetos locais
	lisMALHA_EXT    = new TList();
	lisMALHA_GER    = new TList();
	lisMALHA_INT    = new TList();
	}

//---------------------------------------------------------------------------
__fastcall TMalhas::~TMalhas(void)
	{
	//destrói lista e seus objetos
	if (lisMALHA_EXT) {LimpaTList(lisMALHA_EXT); delete lisMALHA_EXT; lisMALHA_EXT = NULL;}
	if (lisMALHA_GER) {LimpaTList(lisMALHA_GER); delete lisMALHA_GER; lisMALHA_GER = NULL;}
	if (lisMALHA_INT) {LimpaTList(lisMALHA_INT); delete lisMALHA_INT; lisMALHA_INT = NULL;}
	}

//---------------------------------------------------------------------------
bool __fastcall TMalhas::Inicia(void)
	{
	//variáveis locais
	VTRedes     *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
	TMontaMalha monta  = TMontaMalha(apl);

	//reinicia lista de Malhas
	LimpaTList(lisMALHA_EXT);
	LimpaTList(lisMALHA_GER);
	LimpaTList(lisMALHA_INT);
	//monta conjunto de malhas internas e externas
	monta.Executa(this, redes);

	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TMalhas::InsereMalha(VTMalha *malha)
	{
	//proteção
	if (malha == NULL) return(false);
	//verifica número de Redes envolvidas na Malha
	if (malha->NumeroDeRedes() <= 1)
		{//malha interna
		if (lisMALHA_INT->IndexOf(malha) < 0)
			{
			lisMALHA_INT->Add(malha);
			}
		}
	else
		{//malha externa
		if (lisMALHA_EXT->IndexOf(malha) < 0)
			{
			lisMALHA_EXT->Add(malha);
			}
		}
	return(true);
	}

//---------------------------------------------------------------------------
bool __fastcall TMalhas::InsereMalhaGeracao(VTMalha *malha)
	{
	//proteção
	if (malha == NULL) return(false);
	//insere Mlaha em lisMALHA_GER
	lisMALHA_GER->Add(malha);
	return(true);
	}

//---------------------------------------------------------------------------
TList* __fastcall TMalhas::LisMalhaExterna(void)
	{
	return(lisMALHA_EXT);
	}

//---------------------------------------------------------------------------
TList* __fastcall TMalhas::LisMalhaGeracao(void)
	{
	return(lisMALHA_GER);
	}

//---------------------------------------------------------------------------
TList* __fastcall TMalhas::LisMalhaInterna(void)
	{
	return(lisMALHA_INT);
	}

//---------------------------------------------------------------------------
bool __fastcall TMalhas::RemoveMalha(VTMalha *malha)
	{
	//retira malha da Lista
	lisMALHA_EXT->Remove(malha);
	lisMALHA_GER->Remove(malha);
	lisMALHA_INT->Remove(malha);

	return(true);
	}

//---------------------------------------------------------------------------
//eof
