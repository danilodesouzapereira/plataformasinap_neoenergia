//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TSuprimento.h"
#include "VTResFlowSup.h"
#include "..\Constante\Fases.h"
#include "..\Curvas\VTCurva.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTSuprimento* __fastcall NewObjSuprimento(int num_pat)
   {
   return(new TSuprimento(num_pat));
   }
   
//---------------------------------------------------------------------------
__fastcall TSuprimento::TSuprimento(int num_pat)
   {
   //inicia dados
   tiposup = supVF;
   Fases   = faseABCNT;
   smax    = 100.;
   //inicia atributos p/ visualiza��o gr�fica
   Show[eqp_FIXO]   = true;
   Show[eqp_CODIGO] = false;
   Show[sup_TIPO]   = true;
   //cria objetos
   resflow = NewObjResFlowSup(this, num_pat);
   }

//---------------------------------------------------------------------------
__fastcall TSuprimento::~TSuprimento(void)
   {
   //destr�i objetos
   if (resflow) {delete resflow; resflow = NULL;}
   //destr�i Curva somente se n�o for t�pica
   if ((curva != NULL)&&(! curva->Tipica)) delete curva;
   curva = NULL;
   }

//---------------------------------------------------------------------------
VTSuprimento* __fastcall TSuprimento::Clone(void)
   {
   //vari�veis locais
   VTSuprimento *suprimento;

   //cria um novo Suprimento
   if ((suprimento = NewObjSuprimento(resflow->Max_pat)) != NULL)
      {//copia dados deste Suprimento p/ o novo Suprimento
      suprimento->CopiaAtributosDe(*this);
      }
   return(suprimento);
   }

//---------------------------------------------------------------------------
void __fastcall TSuprimento::CopiaAtributosDe(VTEqpto &ref)
   {
   //vari�veis locais
   TSuprimento &suprimento = (TSuprimento&)ref;

   //prote��o
   if (ref.Tipo() != eqptoSUPRIMENTO) return;
   //copia atributos de TEqbar
   TEqbar::CopiaAtributosDe(suprimento);
   //copia atributos de VTSuprimento
   PD_SUP  = suprimento.PD_SUP;
   tiposup = suprimento.tiposup;
   vnom    = suprimento.vnom;
   smax    = suprimento.smax;
   pcc_3f  = suprimento.pcc_3f;
   pcc_ft  = suprimento.pcc_ft;
   zeq0    = suprimento.zeq0;
   zeq1    = suprimento.zeq1;
   //copia atributos de VTResFlowSup
   resflow->CopiaAtributosDe(*(suprimento.resflow));
   //cria c�pia da Curva, destruindo eventual inst�ncia anterior
   if ((curva)&&(! curva->Tipica)) delete curva;
   curva = suprimento.curva->Clone();
   curva->Eqpto = this;
   }

//---------------------------------------------------------------------------
void __fastcall TSuprimento::DefineCurva(VTCurva *curva)
   {
   //salva ponteiro p/ objeto
   this->curva = curva;
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TSuprimento::ExisteCurva(void)
   {
   return(curva);
   }

//---------------------------------------------------------------------------
bool __fastcall TSuprimento::PM_GetDefinidoPorFase(void)
   {
   //verifica se a Curva � definida por fase
   if ((curva != NULL)&&(curva->NumeroValores == nvCURVA_RRSUP)) return(true);
   //verifica se � um Suprimento equivalente
   return(Reducao);
   }

//---------------------------------------------------------------------------
int __fastcall TSuprimento::Tipo(void)
   {
   return(eqptoSUPRIMENTO);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TSuprimento::TipoAsString(void)
   {
   return("Suprimento");
   }

//---------------------------------------------------------------------------
//eof
