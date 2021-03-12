//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TResFlowRede.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Constante\Const.h"
#include "..\Constante\Trifasico.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapacitor.h"
#include "..\Rede\VTCarga.h"
//#include "..\Rede\VTFases.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTReator.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTResFlowSup.h"
#include "..\Rede\VTSuprimento.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTResFlowRede* __fastcall NewObjResFlowRede(VTApl *apl)
   {
   try{
      return(new TResFlowRede(apl));
      }catch(Exception &e)
         {}
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TResFlowRede::TResFlowRede(VTApl *apl)
   {
   //salva ponteiro p/ Apl
   this->apl = apl;
   //cria objetos
   lisEQP = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TResFlowRede::~TResFlowRede(void)
   {
   //destr�i lista sem destruir objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::PM_GetPcar_mw(int np)
   {
   //obt�m lista de Cargas
   lisEQP->Clear();
   rede->LisEqbar(lisEQP, eqptoCARGA);
   //calcula pot�ncia aparente das Cragas
   return(Scarga_mva(lisEQP, np).real());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::PM_GetPerda_kw(int np)
   {
   //vari�veis locais
   double    perda_mw = 0;
   VTLigacao *ligacao;
   TList     *lisLIG = rede->LisLigacao();

   //totaliza perdas em todas as Ligacoes
   for (int n = 0; n < lisLIG->Count; n++)
      {
      ligacao   = (VTLigacao*)lisLIG->Items[n];
      perda_mw += ligacao->resflow->Perda_mw[np];
      }
   //retorna perda em kW
   return(perda_mw * 1000.);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::PM_GetPerda_kvar(int np)
   {
   return(0);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::PM_GetPger_mw(int np)
   {
   //obt�m lista de geradores
   lisEQP->Clear();
   rede->LisEqbar(lisEQP, eqptoGERADOR);
   //calcula pot�ncia aparente
   return(Sgerador_mva(lisEQP, np).real());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::PM_GetPsup_mw(int np)
   {
   //obt�m lista de Suprimentos
   lisEQP->Clear();
   rede->LisEqbar(lisEQP, eqptoSUPRIMENTO);
   //calcula pot�ncia aparente
   return(Ssuprimento_mva(lisEQP, np).real());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::PM_GetQcap_mvar(int np)
   {
   //obt�m lista de Capacitores
   lisEQP->Clear();
   rede->LisEqbar(lisEQP, eqptoCAPACITOR);
   //calcula pot�ncia reativa
   return(Qcapacitor_mvar(lisEQP, np));
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::PM_GetQcar_mvar(int np)
   {
   //obt�m lista de Cargas
   lisEQP->Clear();
   rede->LisEqbar(lisEQP, eqptoCARGA);
   //calcula pot�ncia aparente das Cargas
   return(Scarga_mva(lisEQP, np).imag());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::PM_GetQger_mvar(int np)
   {
   //obt�m lista de geradores
   lisEQP->Clear();
   rede->LisEqbar(lisEQP, eqptoGERADOR);
   //calcula pot�ncia aparente
   return(Sgerador_mva(lisEQP, np).imag());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::PM_GetQrea_mvar(int np)
   {
   //obt�m lista de reatores
   lisEQP->Clear();
   rede->LisEqbar(lisEQP, eqptoREATOR);
   //soma pot�ncias dos reatores
   return(Qreator_mvar(lisEQP, np));
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::PM_GetQsup_mvar(int np)
   {
   //obt�m lista de Suprimentos
   lisEQP->Clear();
   rede->LisEqbar(lisEQP, eqptoSUPRIMENTO);
   //calcula pot�ncia aparente
   return(Ssuprimento_mva(lisEQP, np).imag());
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TResFlowRede::PM_GetRede(void)
   {
   return(rede);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowRede::PM_GetScar_mva(int np)
   {
   //obt�m lista de Cargas
   lisEQP->Clear();
   rede->LisEqbar(lisEQP, eqptoCARGA);
   //calcula pot�ncia aparente das cargas
   return(Scarga_mva(lisEQP, np));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowRede::PM_GetSger_mva(int np)
   {
   //obt�m lista de geradores
   lisEQP->Clear();
   rede->LisEqbar(lisEQP, eqptoGERADOR);
   //calcula pot�ncia aparente dos geradores
   return(Sgerador_mva(lisEQP, np));
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowRede::PM_GetSsup_mva(int np)
   {
  //obt�m lista de suprimentos
   lisEQP->Clear();
   rede->LisEqbar(lisEQP, eqptoSUPRIMENTO);
   //calcula pot�ncias dos suprimentos
   return(Ssuprimento_mva(lisEQP, np));
   }

//---------------------------------------------------------------------------
void __fastcall TResFlowRede::PM_SetRede(VTRede *rede)
   {
   //salva ponteiro p/ rede
   this->rede = rede;
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::Pcarga_mw(VTBarra *barra, int np)
   {
   //obt�m lista de Cargas
   lisEQP->Clear();
   barra->LisEqbar(lisEQP, eqptoCARGA);
   //calcula pot�ncia aparente das cargas
   return(Scarga_mva(lisEQP, np).real());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::Pgerador_mw(VTBarra *barra, int np)
   {
   //obt�m lista de Geradores
   lisEQP->Clear();
   barra->LisEqbar(lisEQP, eqptoGERADOR);
   //calcula pot�ncia aparente dos geradores
   return(Sgerador_mva(lisEQP, np).real());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::Psuprimento_mw(VTBarra *barra, int np)
   {
   //obt�m lista de Suprimentos
   lisEQP->Clear();
   barra->LisEqbar(lisEQP, eqptoSUPRIMENTO);
   //calcula pot�ncia aparente dos suprimentos
   return(Ssuprimento_mva(lisEQP, np).real());
  }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::Qcapacitor_mvar(VTBarra *barra, int np)
   {
   //obt�m lista de Capacitores
   lisEQP->Clear();
   barra->LisEqbar(lisEQP, eqptoCAPACITOR);
   //calcula pot�ncia aparente das Capacitores
   return(Qcapacitor_mvar(lisEQP, np));
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::Qcapacitor_mvar(TList *lisEQP, int np)
   {
   //vari�veis locais
   double      v_pu;
   double      qcap_mvar, q_mvar = 0;
   VTCapacitor *capacitor;
   VTDemanda   *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));


   //soma pot�ncias dos Capacitores em lisEQP
   for (int n = 0; n < lisEQP->Count; n++)
      {
      capacitor  = (VTCapacitor*)lisEQP->Items[n];
      //obt�m demanda nominal do Capacitor
      qcap_mvar  = demanda->Q_mvar[capacitor][np];
      //determina tens�o em pu da Barra do Capacitor
      v_pu = capacitor->pbarra->resflow->Vff_pu_min[np];
      //determina deamnda real: Capacitor � tratado como Zcte
      q_mvar += qcap_mvar * (v_pu *v_pu);
      }
   //retorna pot�ncia reativa com valor negativo
   return(-q_mvar);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::Qcarga_mvar(VTBarra *barra, int np)
   {
   //obt�m lista de Cargas
   lisEQP->Clear();
   barra->LisEqbar(lisEQP, eqptoCARGA);
   //calcula pot�ncia aparente das cargas
   return(Scarga_mva(lisEQP, np).imag());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::Qgerador_mvar(VTBarra *barra, int np)
   {
   //obt�m lista de Geradores
   lisEQP->Clear();
   barra->LisEqbar(lisEQP, eqptoGERADOR);
   //calcula pot�ncia aparente dos geradores
   return(Sgerador_mva(lisEQP, np).imag());
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::Qreator_mvar(VTBarra *barra, int np)
   {
   //obt�m lista de Reatores
   lisEQP->Clear();
   barra->LisEqbar(lisEQP, eqptoREATOR);
   //calcula pot�ncia aparente das Reatores
   return(Qreator_mvar(lisEQP, np));
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::Qreator_mvar(TList *lisEQP, int np)
   {
   //vari�veis locais
   double      v_pu;
   double      qrea_mvar, q_mvar = 0;
   VTReator    *reator;
   VTDemanda   *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //soma pot�ncias dos reatores
   for (int n = 0; n < lisEQP->Count; n++)
      {
      reator  = (VTReator*)lisEQP->Items[n];
      //obt�m demanda nominal do reator
      qrea_mvar  = demanda->Q_mvar[reator][np];
      //determina tens�o em pu da Barra ddo Reator
      v_pu = reator->pbarra->resflow->Vff_pu_min[np];
      //determina deamnda real: Reator � tratado como Zcte
      q_mvar += qrea_mvar * (v_pu *v_pu);
      }
   return(q_mvar);
   }

//---------------------------------------------------------------------------
double __fastcall TResFlowRede::Qsuprimento_mvar(VTBarra *barra, int np)
   {
   //obt�m lista de Suprimentos
   lisEQP->Clear();
   barra->LisEqbar(lisEQP, eqptoSUPRIMENTO);
   //calcula pot�ncia aparente dos suprimentos
   return(Ssuprimento_mva(lisEQP, np).imag());
  }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowRede::Scarga_mva(TList *lisEQP, int np)
   {
   //vari�veis locais
   double          v_pu;
   complex<double> scar_mva;
   complex<double> s_mva = CZero();
   VTCarga         *carga;
   VTDemanda       *demanda = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //soma pot�ncias das Cargas
   for (int n = 0; n < lisEQP->Count; n++)
      {
      carga  = (VTCarga*)lisEQP->Items[n];
      //obt�m demanda nominal da Carga
      scar_mva  = demanda->S_mva[carga][np];
      //determina tens�o em pu da Barra da Carga
      v_pu = carga->pbarra->resflow->Vff_pu_min[np];
      //determina demanda real em fun��o do modelo de carga
      switch(carga->ModeloCarga)
         {
         case cargaICTE: scar_mva *= v_pu;         break;
         case cargaZCTE: scar_mva *= (v_pu *v_pu); break;
         case cargaSCTE: break;
         default:        break;
         }
      //totaliza demanda das Cargas
      s_mva  += scar_mva;
      }
   return(s_mva);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowRede::Sgerador_mva(TList *lisEQP, int np)
   {
   //vari�veis locais
   complex<double> s_mva = CZero();
   VTGerador       *gerador;

   //soma pot�ncias dos geradores
   for (int n = 0; n < lisEQP->Count; n++)
      {
      gerador = (VTGerador*)lisEQP->Items[n];
      s_mva  += gerador->resflow->S_mva[np];
      }
   return(s_mva);
   }

//---------------------------------------------------------------------------
complex<double> __fastcall TResFlowRede::Ssuprimento_mva(TList *lisEQP, int np)
   {
   //vari�veis locais
   complex<double> s_mva = CZero();
   VTSuprimento    *suprimento;

   //soma pot�ncias dos suprimentos
   for (int n = 0; n < lisEQP->Count; n++)
      {
      suprimento = (VTSuprimento*)lisEQP->Items[n];
      s_mva     += suprimento->resflow->S_mva[np];
      }
   return(s_mva);
   }

//---------------------------------------------------------------------------
//eof


