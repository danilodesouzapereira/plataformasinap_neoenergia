//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TArranjos.h"
#include "VTArranjo.h"
#include "VTCabo.h"
#include "VTSuporte.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTArranjos
//---------------------------------------------------------------------------
VTArranjos* __fastcall NewObjArranjos(void)
   {
   return(new TArranjos());
   }

//---------------------------------------------------------------------------
__fastcall TArranjos::TArranjos(void)
   {
   try{//cria listas
      lisARR = new TList();
      lisCAB = new TList();
      lisSUP = new TList();
      //cria listas p/ componentes duplicados
      dup.lisARR = new TList();
      dup.lisCAB = new TList();
      dup.lisSUP = new TList();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TArranjos::~TArranjos(void)
   {
   //destrói lista e seus objetos
   if (lisARR) {LimpaTList(lisARR); delete lisARR; lisARR = NULL;}
   if (lisCAB) {LimpaTList(lisCAB); delete lisCAB; lisCAB = NULL;}
   if (lisSUP) {LimpaTList(lisSUP); delete lisSUP; lisSUP = NULL;}
   //destrói lista e seus objetos
   if (dup.lisARR) {LimpaTList(dup.lisARR); delete dup.lisARR; dup.lisARR = NULL;}
   if (dup.lisCAB) {LimpaTList(dup.lisCAB); delete dup.lisCAB; dup.lisCAB = NULL;}
   if (dup.lisSUP) {LimpaTList(dup.lisSUP); delete dup.lisSUP; dup.lisSUP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TArranjos::Clear(void)
   {
   //reinicia listas de objetos Arranjo, Cabo e Suporte
   LimpaTList(lisARR);
   LimpaTList(lisCAB);
   LimpaTList(lisSUP);
   //
   LimpaTList(dup.lisARR);
   LimpaTList(dup.lisCAB);
   LimpaTList(dup.lisSUP);
   }

//---------------------------------------------------------------------------
bool __fastcall TArranjos::ExisteAlteracao(void)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //verifica se algum Arranjo é novo ou foi alterado
   for (int n = 0; n < lisARR->Count; n++)
      {
      eqpto = (VTEqpto*)lisARR->Items[n];
      if (eqpto->StatusAlterado()) return(true);
      if (eqpto->StatusNovo())     return(true);
      }
   //verifica se algum Cabo é novo ou foi alterado
   for (int n = 0; n < lisCAB->Count; n++)
      {
      eqpto = (VTEqpto*)lisCAB->Items[n];
      if (eqpto->StatusAlterado()) return(true);
      if (eqpto->StatusNovo())     return(true);
      }
   //verifica se algum Suporte é novo ou foi alterado
   for (int n = 0; n < lisSUP->Count; n++)
      {
      eqpto = (VTEqpto*)lisSUP->Items[n];
      if (eqpto->StatusAlterado()) return(true);
      if (eqpto->StatusNovo())     return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TArranjos::ExisteArranjo(int arranjo_id)
   {
   //variáveis locais
   VTArranjo *arranjo;

   //loop p/ todos Arranjos em lisARR
   for (int n = 0; n < lisARR->Count; n++)
      {
      arranjo = (VTArranjo*)lisARR->Items[n];
      if (arranjo->Id == arranjo_id) return(arranjo);
      }
   //procura Arranjo na lista de Arranjos duplicado: dup.lisARR
   for (int n = 0; n < dup.lisARR->Count; n++)
      {
      arranjo = (VTArranjo*)dup.lisARR->Items[n];
      if (arranjo->Id == arranjo_id)
         {//existe Arranjo duplicado: determina Arranjo original pelo Codigo
         if ((arranjo = ExisteArranjo(arranjo->Codigo, arranjo->TipoCfg)) != NULL) return(arranjo);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TArranjos::ExisteArranjo(AnsiString codigo, int tipo_cfg)
   {
   //variáveis locais
   VTArranjo *arranjo;

   //proteção
   if (codigo.IsEmpty()) return(NULL);
   //protege código contra caracteres inválidos
   //codigo = ASCIIFilter(codigo);
   //loop p/ todo Arranjo em lisARR
   for (int n = 0; n < lisARR->Count; n++)
      {
      arranjo = (VTArranjo*)lisARR->Items[n];
      if (arranjo->Codigo.AnsiCompareIC(codigo) == 0)
         {//encontrou Arranjo c/ código indicado: verifica se foi indicado um tipo
         if ((tipo_cfg < arranjoCABOxFASE)||(tipo_cfg == arranjo->TipoCfg)) return(arranjo);
         }
      }
   return(NULL);
   }
/*
//---------------------------------------------------------------------------
VTArranjo* __fastcall TArranjos::ExisteArranjoCABOxFASE(VTSuporte *suporte, VTCabo *caboA,
                                                        VTCabo *caboB, VTCabo *caboC, VTCabo *caboN)
   {
   //variáveis locais
   VTArranjo *arranjo;

   //loop p/ todo Arranjo em lisARR
   for (int n = 0; n < lisARR->Count; n++)
      {
      arranjo = (VTArranjo*)lisARR->Items[n];
      //verifica se o Arranjo é do tipo arranjoCABOxFASE
      if (arranjo->TipoCfg != arranjoCABOxFASE) continue;
      //verifica Suporte e Cabos do Arranjo
      if (arranjo->suporte != suporte) continue;
      if (arranjo->fase[0].cabo != caboA) continue;
      if (arranjo->fase[1].cabo != caboB) continue;
      if (arranjo->fase[2].cabo != caboC) continue;
      if (arranjo->fase[3].cabo != caboN) continue;
      return(arranjo);
      }
   return(NULL);
   }
*/
//DVK 2017.03.22 alterado para retornar primeiro arranjo, se o suporte for NULL
//---------------------------------------------------------------------------
VTArranjo* __fastcall TArranjos::ExisteArranjoCABOxFASE(VTSuporte *suporte, VTCabo *caboA,
                                                        VTCabo *caboB, VTCabo *caboC, VTCabo *caboN)
   {
   //variáveis locais
   VTArranjo *arranjo;

   //loop p/ todo Arranjo em lisARR
   for (int n = 0; n < lisARR->Count; n++)
      {
      arranjo = (VTArranjo*)lisARR->Items[n];
      //verifica se o Arranjo é do tipo arranjoCABOxFASE
      if (arranjo->TipoCfg != arranjoCABOxFASE) continue;
      //verifica Cabos do Arranjo
      if (arranjo->fase[0].cabo != caboA) continue;
      if (arranjo->fase[1].cabo != caboB) continue;
      if (arranjo->fase[2].cabo != caboC) continue;
      if (arranjo->fase[3].cabo != caboN) continue;
      //verifica Suporte
      if (suporte == NULL) return(arranjo);
      else if (arranjo->suporte != suporte) continue;
      return(arranjo);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TArranjos::ExisteArranjoRamal(AnsiString codigo, int fases)
   {
   //variáveis locais
   VTArranjo *arranjo;

   //proteção
   if (codigo.IsEmpty()) return(NULL);
   //loop p/ todo Arranjo em lisARR
   for (int n = 0; n < lisARR->Count; n++)
      {
      arranjo = (VTArranjo*)lisARR->Items[n];
      //verifica se é ramal
      if (! arranjo->ramal) continue;
      if (arranjo->Codigo.AnsiCompareIC(codigo) == 0)
         {//encontrou Arranjo c/ código indicado: verifica fases
         if (arranjo->Fases == fases) return(arranjo);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TArranjos::ExisteArranjoCABO_Z0Z1(VTCabo *cabo, int fases)
   {
   //variáveis locais
   VTArranjo *arranjo;

   //proteção
   if (cabo == NULL) return(NULL);
   //loop p/ todo Arranjo em lisARR
   for (int n = 0; n < lisARR->Count; n++)
      {
      arranjo = (VTArranjo*)lisARR->Items[n];
      //verifica se o Arranjo é do tipo arranjoCABO_Z0Z1
      if (arranjo->TipoCfg != arranjoCABO_Z0Z1) continue;
      //verifica se o Arranjo utiliza o Cabo indicado
      if (arranjo->caboZ0Z1 != cabo) continue;
      //verifica se o arranjo atende às fases indicadas
      if ((fases == faseINV)||(arranjo->Fases == fases)) return(arranjo);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TArranjos::ExisteArranjoZ0Z1(double r0, double x0, double c0,
                                                   double r1, double x1, double c1, int fases)
   {
   //variáveis locais
   VTArranjo *arranjo;

   //loop p/ todo Arranjo em lisARR
   for (int n = 0; n < lisARR->Count; n++)
      {
      arranjo = (VTArranjo*)lisARR->Items[n];
      //verifica se o Arranjo é do tipo arranjoZ0Z1
      if (arranjo->TipoCfg != arranjoZ0Z1) continue;
      //verifica se a impedância do Arranjo é igual ao valores passados
      if ((arranjo->z0.r == r0)&&(arranjo->z0.x == x0)&&(arranjo->c0 == c0)&&
          (arranjo->z1.r == r1)&&(arranjo->z1.x == x1)&&(arranjo->c1 == c1))
         {
         if ((fases == faseINV)||(arranjo->Fases == fases)) return(arranjo);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TArranjos::ExisteArranjoExternId(AnsiString extern_id)
   {
   //variáveis locais
   VTArranjo *arranjo;

   //proteção
   if (extern_id.IsEmpty()) return(NULL);
   //loop p/ todo Arranjo em lisARR
   for (int n = 0; n < lisARR->Count; n++)
      {
      arranjo = (VTArranjo*)lisARR->Items[n];
      if (arranjo->Extern_id.AnsiCompareIC(extern_id) == 0) return(arranjo);
      }
   //procura Arranjo na lista de Arranjos duplicado: dup.lisARR
   for (int n = 0; n < dup.lisARR->Count; n++)
      {
      arranjo = (VTArranjo*)dup.lisARR->Items[n];
      if (arranjo->Extern_id.AnsiCompareIC(extern_id) == 0)
         {//existe Arranjo duplicado: determina Arranjo original pelo Codigo
         if ((arranjo = ExisteArranjo(arranjo->Codigo, arranjo->TipoCfg)) != NULL) return(arranjo);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTCabo* __fastcall TArranjos::ExisteCabo(int cabo_id)
   {
   //variáveis locais
   VTCabo *cabo;

   //loop p/ todos Cabos em lisCAB
   for (int n = 0; n < lisCAB->Count; n++)
      {
      cabo = (VTCabo*)lisCAB->Items[n];
      if (cabo->Id == cabo_id) return(cabo);
      }
   //procura Cabo na lista de Cabos duplicado: dup.lisCAB
   for (int n = 0; n < dup.lisCAB->Count; n++)
      {
      cabo = (VTCabo*)dup.lisCAB->Items[n];
      if (cabo->Id == cabo_id)
         {//existe Cabo duplicado: determina Cabo original pelo Codigo
         if ((cabo = ExisteCabo(cabo->Codigo, cabo->TipoCfg)) != NULL) return(cabo);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTCabo* __fastcall TArranjos::ExisteCabo(AnsiString codigo, int tipo_cfg)
   {
   //variáveis locais
   VTCabo *cabo;

   //proteção
   if (codigo.IsEmpty()) return(NULL);
   //protege código contra caracteres inválidos
   //codigo = ASCIIFilter(codigo);
   //loop p/ todos Cabos em lisCAB
   for (int n = 0; n < lisCAB->Count; n++)
      {
      cabo = (VTCabo*)lisCAB->Items[n];
      if (cabo->Codigo.AnsiCompareIC(codigo) == 0)
         {//encontrou Cabo c/ código indicado: verifica se foi indicado um tipo
         if ((tipo_cfg < caboR_GMR)||(tipo_cfg == cabo->TipoCfg)) return(cabo);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTCabo* __fastcall TArranjos::ExisteCaboExternId(AnsiString extern_id)
   {
   //variáveis locais
   VTCabo *cabo;

   //proteção
   if (extern_id.IsEmpty()) return(NULL);
   //loop p/ todos Cabos em lisCAB
   for (int n = 0; n < lisCAB->Count; n++)
      {
      cabo = (VTCabo*)lisCAB->Items[n];
      if (cabo->Extern_id.AnsiCompareIC(extern_id) == 0) return(cabo);
      }
   //procura Cabo na lista de Cabos duplicado: dup.lisCAB
   for (int n = 0; n < dup.lisCAB->Count; n++)
      {
      cabo = (VTCabo*)dup.lisCAB->Items[n];
      if (cabo->Extern_id.AnsiCompareIC(extern_id) == 0)
         {//existe Cabo duplicado: determina Cabo original pelo Codigo
         if ((cabo = ExisteCabo(cabo->Codigo, cabo->TipoCfg)) != NULL) return(cabo);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTSuporte* __fastcall TArranjos::ExisteSuporte(int suporte_id)
   {
   //variáveis locais
   VTSuporte *suporte;

   //loop p/ todos Suporte em lisSUP
   for (int n = 0; n < lisSUP->Count; n++)
      {
      suporte = (VTSuporte*)lisSUP->Items[n];
      if (suporte->Id == suporte_id) return(suporte);
      }
   //procura Suporte na lista de Suportes duplicado: dup.lisSUP
   for (int n = 0; n < dup.lisSUP->Count; n++)
      {
      suporte = (VTSuporte*)dup.lisSUP->Items[n];
      if (suporte->Id == suporte_id)
         {//existe Suporte duplicado: determina Suporte original pelo Codigo
         if ((suporte = ExisteSuporte(suporte->Codigo)) != NULL) return(suporte);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTSuporte* __fastcall TArranjos::ExisteSuporte(AnsiString codigo)
   {
   //variáveis locais
   VTSuporte *suporte;

   //proteção
   if (codigo.IsEmpty()) return(NULL);
   //loop p/ todos Suporte em lisSUP
   for (int n = 0; n < lisSUP->Count; n++)
      {
      suporte = (VTSuporte*)lisSUP->Items[n];
      if (suporte->Codigo.AnsiCompareIC(codigo) == 0) return(suporte);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTSuporte* __fastcall TArranjos::ExisteSuporteExternId(AnsiString extern_id)
   {
   //variáveis locais
   VTSuporte *suporte;

   //proteção
   if (extern_id.IsEmpty()) return(NULL);
   //loop p/ todos Suporte em lisSUP
   for (int n = 0; n < lisSUP->Count; n++)
      {
      suporte = (VTSuporte*)lisSUP->Items[n];
      if (suporte->Extern_id.AnsiCompareIC(extern_id) == 0) return(suporte);
      }
   //procura Suporte na lista de Suportes duplicado: dup.lisSUP
   for (int n = 0; n < dup.lisSUP->Count; n++)
      {
      suporte = (VTSuporte*)dup.lisSUP->Items[n];
      if (suporte->Extern_id.AnsiCompareIC(extern_id) == 0)
         {//existe Suporte duplicado: determina Suporte original pelo Codigo
         if ((suporte = ExisteSuporte(suporte->Codigo)) != NULL) return(suporte);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TArranjos::InsereArranjo(VTArranjo *arranjo)
   {
   //proteção: verifica se o Arranjo já existe
   if (lisARR->IndexOf(arranjo) < 0)
      {//define Owner
      arranjo->Owner = this;
      //verifica se existe Arranjo com mesmo código
      if (ExisteArranjo(arranjo->Codigo, arranjo->TipoCfg))
         {//insere Arranjo em dup.lisARR
         dup.lisARR->Add(arranjo);
         }
      else
         {//insere Arranjo em lisARR
         lisARR->Add(arranjo);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TArranjos::InsereCabo(VTCabo *cabo)
   {
   //proteção: verifica se o Cabo já existe
   if (lisCAB->IndexOf(cabo) < 0)
      {//verifica se existe Cabo com mesmo código
      if (ExisteCabo(cabo->Codigo, cabo->TipoCfg))
         {//insere Cabo em dup.lisCAB
         dup.lisCAB->Add(cabo);
         }
      else
         {//insere Cabo em lisCAB
         lisCAB->Add(cabo);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TArranjos::InsereSuporte(VTSuporte *suporte)
   {
   //proteção: verifica se o Suporte já existe
   if (lisSUP->IndexOf(suporte) < 0)
      {//verifica se existe Suporte com mesmo código
      if (ExisteSuporte(suporte->Codigo))
         {//insere Suporte em dup.lisSUP
         dup.lisSUP->Add(suporte);
         }
      else
         {//insere Suporte em lisSUP
         lisSUP->Add(suporte);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TArranjos::LisArranjo(void)
   {
   return(lisARR);
   }

//---------------------------------------------------------------------------
TList* __fastcall TArranjos::LisCabo(void)
   {
   return(lisCAB);
   }

//---------------------------------------------------------------------------
TList* __fastcall TArranjos::LisSuporte(void)
   {
   return(lisSUP);
   }

//---------------------------------------------------------------------------
void __fastcall TArranjos::ReiniciaControleExternId(void)
   {
   //destrói Arranjos, Cabos e Suportes  duplicados
   LimpaTList(dup.lisARR);
   LimpaTList(dup.lisCAB);
   LimpaTList(dup.lisSUP);
   }

//---------------------------------------------------------------------------
int __fastcall TArranjos::Tipo(void)
   {
   return(eqptoARRANJOS);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TArranjos::TipoAsString(void)
   {
   return("Arranjos");
   }

//---------------------------------------------------------------------------
//eof