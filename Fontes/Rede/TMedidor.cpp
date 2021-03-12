//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMedidor.h"
#include "VTBarra.h"
#include "VTCanal.h"
#include "VTRedes.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMedidor* __fastcall NewObjMedidor(void)
   {
   return(new TMedidor());
   }

//---------------------------------------------------------------------------
__fastcall TMedidor::TMedidor(void)
   {
   //cria objetos
   lisCANAL = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TMedidor::~TMedidor(void)
   {
   //destrói lista e seus objetos
   if (lisCANAL) {LimpaTList(lisCANAL); delete lisCANAL; lisCANAL = NULL;}
   }


//---------------------------------------------------------------------------
void __fastcall TMedidor::Clear(void)
   {
   //destrói todos os Canais
   LimpaTList(lisCANAL);
   }

//---------------------------------------------------------------------------
VTMedidor* __fastcall TMedidor::Clone(void)
   {
   //variáveis locais
   VTMedidor *medidor;

   //cria um novo Medidor
   if ((medidor = NewObjMedidor()) != NULL)
      {//copia dados deste Medidor p/ o novo Medidor
      medidor->CopiaAtributosDe(*this);
      }
   return(medidor);
   }

//---------------------------------------------------------------------------
void __fastcall TMedidor::CopiaAtributosDe(VTEqpto &ref)
   {
   //variáveis locais
   VTMedidor &medidor = (VTMedidor&)ref;
   VTCanal   *canal;
   TList     *lisEXT;

   //proteção
   if (ref.Tipo() != eqptoMEDIDOR) return;
   //copia atributos de TEqbar
   TEqbar::CopiaAtributosDe(medidor);
   //destrói objetos Canal
   LimpaTList(lisCANAL);
   //copia dados do Medidor
   Barra  = medidor.Barra;
   lisEXT = medidor.LisCanal();
   for (int n = 0; n < lisEXT->Count; n++)
      {
      canal          = (VTCanal*)lisEXT->Items[n];
      canal          = canal->Clone();
      canal->Medidor = this;
      lisCANAL->Add(canal);
      }
   }

//---------------------------------------------------------------------------
VTCanal* __fastcall TMedidor::ExisteCanal(int numero)
   {
   //variáveis locais
   VTCanal *canal;

   for (int n = 0; n < lisCANAL->Count; n++)
      {
      canal = (VTCanal*)lisCANAL->Items[n];
      if (canal->Numero == numero) return(canal);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTCanal* __fastcall TMedidor::ExisteCanal(AnsiString codigo)
   {
   //variáveis locais
   VTCanal *canal;

   for (int n = 0; n < lisCANAL->Count; n++)
      {
      canal = (VTCanal*)lisCANAL->Items[n];
      if (canal->Codigo.AnsiCompareIC(codigo) == 0) return(canal);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TMedidor::ExisteCanal(VTCanal *canal)
   {
   return(lisCANAL->IndexOf(canal) >= 0);
   }
   
//---------------------------------------------------------------------------
VTCanal* __fastcall TMedidor::ExisteCanal(VTEqpto *eqpto, int tipo_canal, int fases)
   {
	//variáveis locais
	VTCanal *canal;

	for (int n = 0; n < lisCANAL->Count; n++)
		{
		canal = (VTCanal*)lisCANAL->Items[n];
		if ((canal->Eqpto     == eqpto)      &&
          (canal->TipoCanal == tipo_canal) &&
          (canal->Fases     == fases))
         {
         return(canal);
         }
		}
	return(NULL);
	}

//---------------------------------------------------------------------------
bool __fastcall TMedidor::InsereCanal(VTCanal *canal)
	{
   //proteção
   if (canal == NULL) return(false);
   //insere Canal
   if (lisCANAL->IndexOf(canal) < 0) lisCANAL->Add(canal);
   //remove Eqpto da Lixeira
   //pbarra->redes->RemoveEqptoDaLixeira(canal);
   return(true);
   }

//---------------------------------------------------------------------------
VTCanal* __fastcall TMedidor::InsereCanal(VTEqpto *eqpto, int tipo_canal, int fases)
   {
   //variáveis locais
   int     num_canal;
   VTCanal *canal;

   //define número p/ o canal novo
   num_canal = 1;
   if (lisCANAL->Count > 0)
      {
      canal     = (VTCanal*)lisCANAL->Last();
      num_canal = canal->Numero + 1;
      }
   try{//cria um novo objeto Canal
      if ((canal = NewObjCanal()) != NULL)
         {//define dados do canal
         canal->Codigo    = "Canal " + IntToStr(num_canal);
         canal->Numero    = num_canal;
         canal->Medidor   = this;
         canal->Eqpto     = eqpto;
         canal->TipoCanal = tipo_canal;
         canal->Fases     = fases;
         lisCANAL->Add(canal);
         }
      }catch(Exception &e)
         {
         return(NULL);
         }
   return(canal);
   }

//---------------------------------------------------------------------------
TList* __fastcall TMedidor::LisCanal(void)
   {
   return(lisCANAL);
   }

//---------------------------------------------------------------------------
int __fastcall TMedidor::LisCanal(TList *lisEXT, VTEqpto *eqpto)
   {
	//variáveis locais
	VTCanal *canal;

	for (int n = 0; n < lisCANAL->Count; n++)
		{
		canal = (VTCanal*)lisCANAL->Items[n];
		if (canal->Eqpto == eqpto)
         {//insere Canal em lisEXT
         if (lisEXT->IndexOf(canal) < 0) lisEXT->Add(canal);
         }
      }
   return(lisEXT->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TMedidor::LisEqptoDosCanais(TList *lisEXT)
   {
	//variáveis locais
	VTCanal *canal;

	for (int n = 0; n < lisCANAL->Count; n++)
		{
		canal = (VTCanal*)lisCANAL->Items[n];
      //insere Eqpto do Canal em lisEXT
      if (lisEXT->IndexOf(canal->Eqpto) < 0) lisEXT->Add(canal->Eqpto);
      }
   return(lisEXT->Count);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TMedidor::PM_GetBarra(void)
   {
   return(pbarra);
   }

//---------------------------------------------------------------------------
bool __fastcall TMedidor::PM_GetCanaisEnabled(void)
   {
   //variáveis locais
   VTCanal *canal;

   //verifica se todos Canais estão habilitados
   for (int n = 0; n < lisCANAL->Count; n++)
      {
      canal = (VTCanal*)lisCANAL->Items[n];
      if (! canal->Enabled) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TMedidor::PM_SetBarra(VTBarra *barra)
   {
   pbarra = barra;
   }

//---------------------------------------------------------------------------
bool __fastcall TMedidor::RemoveCanal(VTCanal *canal)
   {
   //retira Canal da lista sem destruir objeto
   if (lisCANAL->Remove(canal) < 0) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
int __fastcall TMedidor::Tipo(void)
   {
   return(eqptoMEDIDOR);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMedidor::TipoAsString(void)
   {
   return("Medidor");
   }

//---------------------------------------------------------------------------
//eof
