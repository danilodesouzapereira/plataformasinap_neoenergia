//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TBanco.h"
#include "..\Apl\VTApl.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTResFlowBar.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe
//---------------------------------------------------------------------------
VTBanco* __fastcall NewObjBanco(VTApl *apl)
   {
   return(new TBanco(apl));
   }

//---------------------------------------------------------------------------
__fastcall TBanco::TBanco(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria lista
   lisCAP  = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TBanco::~TBanco(void)
   {
   //destr�i listas sem destruir seus objetos
   if (lisCAP)  {delete lisCAP;  lisCAP  = NULL;}
	}

//---------------------------------------------------------------------------
bool  __fastcall TBanco::InsereCapacitor(VTCapacitor *capacitor)
   {
   //prote��o
   if (capacitor == NULL) return(NULL);
   //insere Capacitor na lista
   if (lisCAP->IndexOf(capacitor) < 0)
      {
      lisCAP->Add(capacitor);
      //associa Capacitor com sua Barra mas sem inserir
      capacitor->pbarra = Barra;
      }
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBanco::LisCapacitor(void)
   {
   return(lisCAP);
   }

//---------------------------------------------------------------------------
int __fastcall TBanco::NumCapacitor(void)
   {
   return(lisCAP->Count);
   }

//---------------------------------------------------------------------------
double __fastcall TBanco::Q_mvar(int ind_pat, bool nominal)
   {
   //vari�veis locais
   double       v_pu;
   double       q_mvar = 0.;
   VTCapacitor *capacitor;
   VTDemanda   *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //loop p/ todos Capacitores em lisCAP
   for (int n = 0; n < lisCAP->Count; n++)
      {
      capacitor = (VTCapacitor*)lisCAP->Items[n];
      q_mvar   += demanda->Q_mvar[capacitor][ind_pat];
      }
   //verifica se deve corrigri a pot.reativa
   if (! nominal)
      {//determina tens�o da Barra, em pu
      v_pu     = Barra->resflow->Vfn_pu_min[ind_pat];
      q_mvar  *= (v_pu * v_pu);
      }
   return(q_mvar);
   }

//---------------------------------------------------------------------------
double __fastcall TBanco::Qtotal_mvar(void)
   {
   //vari�veis locais
   double       q_mvar = 0.;
   VTCapacitor *capacitor;

   //loop p/ todos Capacitores em lisCAP
   for (int n = 0; n < lisCAP->Count; n++)
      {
      capacitor = (VTCapacitor*)lisCAP->Items[n];
      q_mvar   += capacitor->Q_mvar;
      }
   return(q_mvar);
   }

//---------------------------------------------------------------------------
//eof
