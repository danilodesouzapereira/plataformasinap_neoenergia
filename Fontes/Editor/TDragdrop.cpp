//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDragdrop.h"
#include "TEdtCola.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTDragdrop* __fastcall NewObjDragdrop(void)
   {
   try{
      return(new TDragdrop());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TDragdrop::TDragdrop(void)
   {
	//cria lista
	lisEDT_COLA = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TDragdrop::~TDragdrop(void)
   {
	//destrói lista sem destrui seus objetos
	if (lisEDT_COLA) {delete lisEDT_COLA; lisEDT_COLA = NULL;}
	}

//---------------------------------------------------------------------------
void __fastcall TDragdrop::InsEdtCopia(VTEdt *edt)
   {
   //variáveis locais
   TEdtCola *edt_cola;
   TList    *lisEQP;

   //redefine EdtCopia
	PD.edt_copia = edt;
	//define EdtCopia p/ todos EdtCola
	for (int n = 0; n < lisEDT_COLA->Count; n++)
      {
		edt_cola = (TEdtCola*)lisEDT_COLA->Items[n];
		edt_cola->EdtCopia = PD.edt_copia;
		}
   }

//---------------------------------------------------------------------------
void __fastcall TDragdrop::InsEdtCola(VTEdt *edt)
   {
   //variáveis locais
   TEdtCola *edt_cola = (TEdtCola*)edt;

   //insere EdtCola de lisEDT_COLA
	if (lisEDT_COLA->IndexOf(edt_cola) < 0) lisEDT_COLA->Add(edt_cola);
   //define EdtCopia p/ todos EdtCola
	edt_cola->EdtCopia = PD.edt_copia;
	}

//---------------------------------------------------------------------------
void __fastcall TDragdrop::RetEdtCola(VTEdt *edt)
   {
   //retira EdtCola de lisEDT_COLA
   lisEDT_COLA->Remove(edt);
   }

//---------------------------------------------------------------------------
void __fastcall TDragdrop::RetEdtCopia(VTEdt *edt)
   {
   //cancela EdtCopia
	InsEdtCopia(NULL);
   }

//---------------------------------------------------------------------------
//eof
