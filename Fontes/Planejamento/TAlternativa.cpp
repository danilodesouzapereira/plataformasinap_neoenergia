//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma hdrstop
#include "TAlternativa.h"
#include "..\Apl\VTApl.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include "..\..\DLL_Inc\Edita.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTAlternativa* __fastcall NewObjAlternativa(VTApl *apl)
   {
   try{
      return(new TAlternativa(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422298
__fastcall TAlternativa::TAlternativa(VTApl *apl)
   {
	//cria objetos
	PD.apl   = NewObjApl(NULL, apl);
   PD.obras = DLL_NewObjObras(apl);
   //inicia dados
   Valida = true;
   Fixa   = false;
   PD.justificativa = "";
   }

//---------------------------------------------------------------------------
__fastcall TAlternativa::~TAlternativa(void)
   {
   //destrói lista e seus objetos
   if (PD.apl)   {delete PD.apl;   PD.apl   = NULL;}
   if (PD.obras) {delete PD.obras; PD.obras = NULL;}
   }
/*
//---------------------------------------------------------------------------
void __fastcall  TAlternativa::CopiaObras(VTAlternativa *alternativa)
   {
   //variáveis locais
   VTObra *obra;
   TList *lisOBRA = alternativa->Obras->LisObra();

   //copia obras da Alterantiva indicada
   for (int n = 0; n < lisOBRA->Count; n++)
      {
      obra = (VTObra*)lisOBRA->Items[n];
      //cria uma cópia da Obra
      if ((obra = obra->Clone()) == NULL) return;
      //redefine dados da Obra
      obra->Id          = -1;
      obra->Fixa        = false;
      obra->Incorporada = false;
      //insere Obra em Obras
      Obras->InsereObra(obra);
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall  TAlternativa::CopiaObras(VTAlternativa *alternativa)
   {
   //copia Obras
   Obras->CopiaAtributosDe(*alternativa->Obras);
   }


// ---------------------------------------------------------------------------
bool __fastcall TAlternativa::PM_GetAlterado(void)
{

	// if (alternativa->Id < 1)            return(true);
	if (this->Obras->Modificada)
		return (true);

	return (false);
}
// ---------------------------------------------------------------------------
void __fastcall TAlternativa::PM_SetAlterado(bool alterado)
{

	this->Obras->Modificada = alterado;
}
//---------------------------------------------------------------------------
bool __fastcall TAlternativa::PM_GetFixa(void)
   {
   return(PD.fixa);
   }

//---------------------------------------------------------------------------
void __fastcall TAlternativa::PM_SetFixa(bool fixa)
   {
   //variáveis locais
   VTObra *obra;
   TList *lisOBRA = Obras->LisObra();

   PD.fixa = fixa;
   //atualiza todas Obras
   for (int n = lisOBRA->Count-1; n >= 0; n--)
      {
      obra = (VTObra*)lisOBRA->Items[n];
      obra->Fixa = Fixa;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAlternativa::RetiraObraSemAcao(void)
   {
   //variáveis locais
   VTObra *obra;
   TList *lisOBRA = Obras->LisObra();

   for (int n = lisOBRA->Count-1; n >= 0; n--)
      {
      obra = (VTObra*)lisOBRA->Items[n];
      if (obra->LisAcao->Count == 0) {Obras->RetiraObra(obra);}
      }
   }

//---------------------------------------------------------------------------
void __fastcall TAlternativa::ReconfiguraRede(int ano)
   {
   Obras->ReconfiguraRede(ano);
   }

//---------------------------------------------------------------------------
void __fastcall TAlternativa::ReconfiguraRedeAteObra(VTObra *obra)
   {
   Obras->ReconfiguraRedeAteObra(obra);
   }

//---------------------------------------------------------------------------
void __fastcall TAlternativa::ReconfiguraRedeSemObra(void)
   {
   Obras->ReconfiguraRedeSemObra();
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422280
void __fastcall TAlternativa::ReconfiguraRedeTodaObra(VTObra *obra)
   {
   Obras->ReconfiguraRedeTodaObra(obra);
   }


//---------------------------------------------------------------------------
//eof



