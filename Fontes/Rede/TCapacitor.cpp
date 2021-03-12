//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCapacitor.h"
#include "..\Curvas\VTCurva.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCapacitor* __fastcall NewObjCapacitor(int num_pat)
   {
   return(new TCapacitor(num_pat));
   }

//---------------------------------------------------------------------------
__fastcall TCapacitor::TCapacitor(int num_pat)
   {
   //inicia dados
   ligacao     = lgEST_AT;
   Show[cap_Q] = true;
   }

//---------------------------------------------------------------------------
__fastcall TCapacitor::~TCapacitor(void)
   {
   //destrói Curva somente se não for típica
   if ((curva != NULL)&&(! curva->Tipica)) delete curva;
   curva = NULL;
   }

//---------------------------------------------------------------------------
VTCapacitor* __fastcall TCapacitor::Clone(void)
   {
   //variáveis locais
	TCapacitor *capacitor;

   //cria um novo Capacitor
	if ((capacitor = new TCapacitor(curva->Escala)) != NULL)
      {//copia dados deste Capacitor p/ o novo Capacitor
		capacitor->CopiaAtributosDe(*this);
      }
	return(capacitor);
   }

//---------------------------------------------------------------------------
void __fastcall TCapacitor::CopiaAtributosDe(VTEqpto &ref)
	{
	//variáveis locais
	VTCapacitor &capacitor = (VTCapacitor&)ref;

	//proteção
	if (ref.Tipo() != eqptoCAPACITOR) return;
	//copia atributos de TEqbar
	TEqbar::CopiaAtributosDe(capacitor);
	//copia atributos de VTCapacitor
	ligacao = capacitor.ligacao;
	vnom    = capacitor.vnom;
	q       = capacitor.q;
	//destrói Curva própria
	if ((curva)&&(! curva->Tipica)) delete curva;
	//verifica se a Curva do capacitor de referência é típica
	if (capacitor.curva->Tipica)
		{//assume a mesma Curva típica
		curva = capacitor.curva;
		}
	else
		{//cria cópia de Curva própria
		curva = capacitor.curva->Clone();
		curva->Eqpto = this;
      }
	}

//---------------------------------------------------------------------------
void __fastcall TCapacitor::DefineCurva(VTCurva *curva)
   {
   //salva ponteiro p/ objeto
   this->curva = curva;
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TCapacitor::ExisteCurva(void)
   {
   return(curva);
   }

//---------------------------------------------------------------------------
AnsiString  __fastcall TCapacitor::PM_GetPadrao(void)
	{
	return(padrao);
   }

//---------------------------------------------------------------------------
double __fastcall TCapacitor::PM_GetQ_mvar(void)
   {
   return(q);
   }

//---------------------------------------------------------------------------
void __fastcall TCapacitor::PM_SetPadrao(AnsiString padrao)
   {
   this->padrao = padrao;
   }

//---------------------------------------------------------------------------
int __fastcall TCapacitor::Tipo(void)
   {
   return(eqptoCAPACITOR);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCapacitor::TipoAsString(void)
   {
   return("Capacitor");
   }

//---------------------------------------------------------------------------
//eof
