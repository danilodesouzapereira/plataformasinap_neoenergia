//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TSegmento.h"
#include "VTCrescimento.h"
#include "..\Classes\VTClasse.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTSegmento* __fastcall NewObjSegmento(void)
   {
   try{
		return(new TSegmento());
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TSegmento::TSegmento(void)
   {
   try{//cria objetos
      PD.area = DLL_NewObjArea();
		lisCRES = new TList();
		//inicia dados
		Codigo  = "";
		Tipo    = smINDEF;
      Nova    = true;
      Enabled = false;
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TSegmento::~TSegmento(void)
   {
   //destrói objetos
   if (PD.area) {delete PD.area; PD.area = NULL;}
   //destrói lista e seus objetos
   if (lisCRES) {LimpaTList(lisCRES); delete lisCRES; lisCRES = NULL;}
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TSegmento::CrescimentoNulo(void)
   {
   //variáveis locais
   VTCrescimento *crescimento;

   //verifica se todo Crescimento tem taxas nulas
   for (int n = 0; n < lisCRES->Count; n++)
      {
      crescimento = (VTCrescimento*)lisCRES->Items[n];
      if (! crescimento->Nulo()) return(false);
      }
   return(true);
   }
*/

//---------------------------------------------------------------------------
VTCrescimento* __fastcall TSegmento::ExisteCrescimento(VTClasse *classe)
   {
   //variáveis locais
   VTCrescimento *crescimento;

   //verifica se existe um Crescimento p/ a Classe
   for (int n = 0; n < lisCRES->Count; n++)
      {
      crescimento = (VTCrescimento*)lisCRES->Items[n];
      if (crescimento->Classe == classe) return(crescimento);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TSegmento::IniciaCrescimentoNulo(void)
   {
   //variáveis locais
   VTCrescimento *crescimento;

   //verifica se existe um Crescimento p/ a Classe
   for (int n = 0; n < lisCRES->Count; n++)
      {
      crescimento = (VTCrescimento*)lisCRES->Items[n];
      //loop p/ todos os anos
      for (int ano = crescimento->AnoInicial; ano <= crescimento->AnoFinal; ano++)
         {
         crescimento->Taxa_pu[ano] = 0.;
         }
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TSegmento::InsereCrescimento(VTCrescimento *crescimento)
   {
   //proteção
   if (crescimento == NULL) return(false);
/*
OBS: Esta proteção foi comentada porque, na importação de redes externas,
     haverá Cargas equivalentes que precisam assumir uma taxa de crescimento.

   //proteção: não insereCrescimento p/ Classe consEQUIVALENTE
   if (crescimento->Classe->Tag == consEQUIVALENTE)
      {
      delete crescimento;
      return(true);
      }
*/
   //verifica se já existe Crecscimento para a Classe
   if (ExisteCrescimento(crescimento->Classe))
      {//destrói objeto
      delete crescimento;
      return(true);
      }
   //insere Crescimento na lista
   if (lisCRES->IndexOf(crescimento) < 0) lisCRES->Add(crescimento);
   //define Segmento do Crescimento
	crescimento->Segmento = this;
   return(true);
	}

//---------------------------------------------------------------------------
TList* __fastcall TSegmento::LisCrescimento(void)
   {
   return(lisCRES);
   }

//---------------------------------------------------------------------------
VTArea* __fastcall TSegmento::PM_GetArea(void)
   {
   return(PD.area);
   }

//---------------------------------------------------------------------------
bool __fastcall TSegmento::PM_GetAlterada(void)
	{
	return((PD.status & sttALTERADO) == sttALTERADO);
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TSegmento::PM_GetCodigo(void)
   {
   return(PD.codigo);
   }

//---------------------------------------------------------------------------
bool __fastcall TSegmento::PM_GetEnabled(void)
	{
	return((PD.status & sttENABLED) == sttENABLED);
	}

//---------------------------------------------------------------------------
int  __fastcall TSegmento::PM_GetId(void)
   {
   return(PD.id);
   }

//---------------------------------------------------------------------------
bool __fastcall TSegmento::PM_GetNova(void)
   {
   return((PD.status & sttNOVO) == sttNOVO);
   }

//---------------------------------------------------------------------------
VTRegiao* __fastcall TSegmento::PM_GetRegiao(void)
	{
	return(PD.regiao);
	}

//---------------------------------------------------------------------------
int  __fastcall TSegmento::PM_GetTipo(void)
	{
	return(PD.tipo);
	}

//---------------------------------------------------------------------------
void __fastcall TSegmento::PM_SetAlterada(bool alterada)
	{
	PM_SetStatus(sttALTERADO, alterada);
	}

//---------------------------------------------------------------------------
void __fastcall TSegmento::PM_SetArea(VTArea *area)
	{
	PD.area->CopiaAtributosDe(*area);
	}

//---------------------------------------------------------------------------
void __fastcall TSegmento::PM_SetEnabled(bool enabled)
	{
	PM_SetStatus(sttENABLED, enabled);
	}

//---------------------------------------------------------------------------
void __fastcall TSegmento::PM_SetId(int Segmento_id)
	{
	PD.id = Segmento_id;
	}

//---------------------------------------------------------------------------
void __fastcall TSegmento::PM_SetCodigo(AnsiString codigo)
	{
	PD.codigo = codigo;
	}

//---------------------------------------------------------------------------
void __fastcall TSegmento::PM_SetNova(bool nova)
	{
	PM_SetStatus(sttNOVO, nova);
	}

//---------------------------------------------------------------------------
void __fastcall TSegmento::PM_SetRegiao(VTRegiao *regiao)
	{
	PD.regiao = regiao;
	}

//---------------------------------------------------------------------------
void __fastcall TSegmento::PM_SetStatus(unsigned bit, bool enabled)
	{
	if (enabled)
		{PD.status |= bit;}
	else
		{PD.status &= ~bit;}
	}

//---------------------------------------------------------------------------
void __fastcall TSegmento::PM_SetTipo(int tipo)
	{
	PD.tipo = tipo;
	}

//---------------------------------------------------------------------------
void __fastcall TSegmento::RedefinePeriodo(int ano_ini, int ano_fim)
   {
   //variáveis locais
   VTCrescimento *crescimento;

   //verifica se existe um Crescimento p/ a Classe
   for (int n = 0; n < lisCRES->Count; n++)
      {
      crescimento = (VTCrescimento*)lisCRES->Items[n];
      crescimento->RedefinePeriodo(ano_ini, ano_fim);
      }
   }

//---------------------------------------------------------------------------
//eof
