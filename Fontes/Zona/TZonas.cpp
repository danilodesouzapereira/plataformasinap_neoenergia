//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TZonas.h"
#include "..\Apl\VTApl.h"
//#include "..\Cronometro\VTCronometro.h"
//#include "..\Potencia\VTFornecimento.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMNet.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrafoZZ.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTZonas* NewObjZonas(VTApl *apl)
   {
   return(new TZonas(apl));
   }

//---------------------------------------------------------------------------
__fastcall TZonas::TZonas(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl  = apl;
   //cria listas
   lisZON   = new TList();
   lisINV   = new TList();
   lisGER   = new TList();
   lisSUP   = new TList();
   lisDELTA = new TList();
   lisBAR   = new TList();
   lisLIG   = new TList();
   lisTRF   = new TList();
   lisTMP   = new TList();
   //inicia opções
   AvisoEnabled       = true;
   ChaveAbertaEnabled = false;
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16571159
//https://sinapsisenergia.teamwork.com/#tasks/16749725
__fastcall TZonas::~TZonas(void)
   {
   //destrói listas sem destruir seus objetos
   if (lisGER)   {delete lisGER;   lisGER   = NULL;}
   if (lisSUP)   {delete lisSUP;   lisSUP   = NULL;}
   if (lisDELTA) {delete lisDELTA; lisDELTA = NULL;}
   if (lisBAR)   {delete lisBAR;   lisBAR   = NULL;}
   if (lisLIG)   {delete lisLIG;   lisLIG   = NULL;}
   if (lisTRF)   {delete lisTRF;   lisTRF   = NULL;}
   if (lisTMP)   {delete lisTMP;   lisTMP   = NULL;}
   if (lisINV)   {delete lisINV;   lisINV   = NULL;}
   //limpa tudo
   Clear(NULL);
   //destrói listas e seus objetos
   if (lisZON) {LimpaTList(lisZON); delete lisZON; lisZON = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TZonas::CancelaAssociacaoBarraZona(TList *lisBARRA)
   {
   //variáveis locais
   VTBarra *barra;

   for (int nb = 0; nb < lisBARRA->Count; nb++)
      {
      barra = (VTBarra*)lisBARRA->Items[nb];
      barra->zona = NULL;
      }
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16571159
//https://sinapsisenergia.teamwork.com/#tasks/16749725
void __fastcall TZonas::Clear(VTRedes *redes_local)
   {
   //teste
   this->redes;
   if (redes_local == NULL)
	  {
	  redes_local = (VTRedes*)apl->GetObject(__classid(VTRedes));
	  }
   //remove referências a zona de todas as barras e destroi todos obj. zona
   if(redes_local != NULL)
      {
      CancelaAssociacaoBarraZona(redes_local->LisBarra());
      }
   LimpaTList(lisZON);
   }

//---------------------------------------------------------------------------
bool __fastcall TZonas::DefineVnomAnguloFases(void)
   {
   //variáveis locais
   bool         sucesso = true;
   AnsiString   txt;
   VTBarra      *pbar;
   VTBarra      *vet_bar[NBAR_LIG];
   VTEqbar      *eqbar;
   VTEqpto      *eqpto;
   VTLigacao    *pliga;
   VTZona       *zona;

   //define fases das Zonas a partir de uma Zona de referência
   while ((zona = ExisteZonaReferencia()) != NULL)
      {//inicia lisTMP c/ a Zona de referência
      lisTMP->Clear();
      lisTMP->Add(zona);
      //loop p/ todas as zonas na lista lisTMP
      for (int nz = 0; nz < lisTMP->Count; nz++)
         {
         zona = (VTZona*)lisTMP->Items[nz];
         //loop p/ todos Trafos
         for (int nt = lisTRF->Count-1; nt >= 0; nt--)
            {//verifica se o Trafo conecta a Zona
            pliga = (VTLigacao*)lisTRF->Items[nt];
            if (! TrafoConectaZona(pliga, zona)) continue;
            //define ângulo das Zonas ligadas pelo Trafo
            if (pliga->Tipo() == eqptoTRAFO)
               {//Trafo
               if (! DefineVnomAnguloZonaTrafo(zona, (VTTrafo*)pliga))
                  {
                  txt = "Conflito de tensão ou ligação p/ o Trafo: " + pliga->Codigo +
                        ", Rede: " + pliga->rede->Codigo;
                  if (AvisoEnabled) Erro(txt);
                  else              Alerta(txt);
                  //return(false);
                  sucesso = false;
                  }
               }
            else
               {//Trafo3E
               if (! DefineVnomAnguloZonaTrafo3E(zona, (VTTrafo3E*)pliga))
                  {
                  txt = "Conflito de tensão ou ligação p/ o Trafo3E: " + pliga->Codigo +
                        ", Rede: " + pliga->rede->Codigo;
                  if (AvisoEnabled) Erro(txt);
                  else              Alerta(txt);
                  //return(false);
                  sucesso = false;
                  }
               }
            //retira trafo da lista
            lisTRF->Delete(nt);
            //inclui Zonas na lista lisTMP
            vet_bar[0] = pliga->pbarra1;
            vet_bar[1] = pliga->pbarra2;
            vet_bar[2] = pliga->pbarra3;
            for (int nb = 0; nb < NBAR_LIG; nb++)
               {
               if (vet_bar[nb] == NULL) continue;
               if (vet_bar[nb]->zona == NULL) continue;
               if (lisTMP->IndexOf(vet_bar[nb]->zona) >= 0) continue;
               lisTMP->Add(vet_bar[nb]->zona);
               }
            }
         }
      }
   //ajusta ângulo da fase alta das zonas do tipo zonaDELTA
   for (int n = 0; n < lisDELTA->Count; n++)
      {
      zona = (VTZona*)lisDELTA->Items[n];
      zona->CorrigeAnguloZonaDelta();
      }
   //complementa fases das zonas com equipamentos de aterramento
   IniciaLisTrafoZZ(lisTRF);
   for (int n = 0; n < lisTRF->Count; n++)
      {
      eqbar = (VTEqbar*)lisTRF->Items[n];
      eqbar->pbarra->zona->DefineNeutroAterrado();
      }
   //verifica se encontrou algum Gerador/Suprimento com fases incompatíveis com a Zona
   if (lisINV->Count > 0)
      {//gera Aviso ou Alerta
      txt.sprintf("Há %d geradores/suprimentos com fases incompatíveis com a rede", lisINV->Count);
      if (AvisoEnabled) Erro(txt);
      else              Alerta(txt);
      //gera Alertas
      for (int n = 0; n < lisINV->Count; n++)
         {
         eqpto = (VTEqpto*)lisINV->Items[n];
         if (eqpto->Codigo.IsEmpty())
            txt.sprintf("%s (ID=%d) com fases incompatíveis com a rede", eqpto->TipoAsString().c_str(), eqpto->Id);
         else
            txt.sprintf("%s %s com fases incompatíveis com a rede", eqpto->TipoAsString().c_str(), eqpto->Codigo.c_str());
         Alerta(txt);
         }
      //return(false);
      sucesso = false;
      }
   //return(true);
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TZonas::DefineVnomAnguloZonaTrafo(VTZona *zona, VTTrafo *trafo)
   {
   if (trafo->Reducao)
      {
      return(DefineVnomAnguloZonaTrafoReducao(zona, trafo));
      }
   else if (trafo->Monofasico)
      {
      return(DefineVnomAnguloZonaTrafoMonofasico(zona, trafo));
      }
   else
      {
      return(DefineVnomAnguloZonaTrafoTrifasico(zona, trafo));
      }
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TZonas::DefineVnomAnguloZonaTrafoMonofasico(VTZona *zona, VTTrafo *trafo)
   {
   //variáveis locais
   int    fase_pri, fase_sec;
   double ang_grau[MAX_FASE], ang_zona_grau[MAX_FASE];
   VTZona *zona_pri, *zona_sec;

   //proteção
   if (! trafo->Monofasico) return(false);
   //ordena Barras do Trafo: pbar1 aponta p/ primário e pbar2 aponta p/ secundário
   trafo->OrdenaBar12PriSec();
   //determina Zonas do primário e secundário
   if ((zona_pri = (trafo->pbarra1) ? trafo->pbarra1->zona : NULL) == NULL) return(false);
   if ((zona_sec = (trafo->pbarra2) ? trafo->pbarra2->zona : NULL) == NULL) return(false);
   //proteção para caso de 3 trafos monofásicos constituindo uma zona secundária trifásica
   if (zona_sec->Tipo == zonaTRIFASICA)
      {return(true);}
   //a zona de referência precisa ser a zona do primário do Trafo monofásico
   //if (zona != zona_pri) return(false);
   //determinas fases do primário e do secundário
   fase_pri = trafo->Fases(trafo->BarraPrimario());
   fase_sec = trafo->Fases(trafo->BarraSecundario());
   //define Zonas do primário e secundário:
   //CUIDADO: trafo mono tem tensão nominal de fase, usa a tensão da Barra
   if (! zona_pri->DefineTipoTrifasica(fase_pri,  trafo->pbarra1->vnom)) return(false);
   if (! zona_sec->DefineTipoMonofasica(fase_sec, trafo->sec.vnom, fase_pri, fase_sec)) return(false);
   //verifica se zona_sec é uma zonaDELTA
   if ((zona_sec->Tipo == zonaDELTA_ABERTO)||(zona_sec->Tipo == zonaDELTA_FECHADO))
      {//insere zona_sec em lisDELTA
      if (lisDELTA->IndexOf(zona_sec) < 0) lisDELTA->Add(zona_sec);
      }
   //determina ângulos das fases do secundário em função das ligações do primário/secundário
   if (! trafo->AngulosNoSecundario(ang_grau))
      {
      //Aviso("Trafo com fases inconsistentes: " + AnsiQuotedStr(trafo->Codigo, '\''));
      return(true);
      }
   //determina qual Barra do trafo se liga à Zona
   if (zona_pri == zona)
      {//Barra do primário ligada à zona indicada: calcula ãngulo da zona do secundário
      ang_zona_grau[indFASE_A] = zona_pri->T_grau[indFASE_A] + ang_grau[indFASE_A];
      ang_zona_grau[indFASE_B] = zona_pri->T_grau[indFASE_A] + ang_grau[indFASE_B];
      ang_zona_grau[indFASE_C] = zona_pri->T_grau[indFASE_A] + ang_grau[indFASE_C];
      ang_zona_grau[indNEUTRO] = 0.;
      //define fases da Zona do secundário
      zona_sec->DefineAngulo_grau(ang_zona_grau);
      }
   else if (zona_sec == zona)
      {//Barra do secundário ligada à zona indicada: calcula ãngulo da zona do secundário
      ang_zona_grau[indFASE_A] = zona_sec->T_grau[indFASE_A] - ang_grau[indFASE_A];
      ang_zona_grau[indFASE_B] = zona_sec->T_grau[indFASE_A] - ang_grau[indFASE_B];
      ang_zona_grau[indFASE_C] = zona_sec->T_grau[indFASE_A] - ang_grau[indFASE_C];
      ang_zona_grau[indNEUTRO] = 0.;
      //define fases da Zona do primário
      zona_pri->DefineAngulo_grau(ang_zona_grau);
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TZonas::DefineVnomAnguloZonaTrafoMonofasico(VTZona *zona, VTTrafo *trafo)
   {
   //variáveis locais
   int    fase_pri, fase_sec;
   double ang_grau[MAX_FASE], ang_zona_grau[MAX_FASE];
   VTZona *zona_pri, *zona_sec;

   //proteção
   if (! trafo->Monofasico) return(false);
   //ordena Barras do Trafo: pbar1 aponta p/ primário e pbar2 aponta p/ secundário
   trafo->OrdenaBar12PriSec();
   //determina Zonas do primário e secundário
   if ((zona_pri = (trafo->pbarra1) ? trafo->pbarra1->zona : NULL) == NULL) return(false);
   if ((zona_sec = (trafo->pbarra2) ? trafo->pbarra2->zona : NULL) == NULL) return(false);
   //determinas fases do primário e do secundário
   fase_pri = trafo->Fases(trafo->BarraPrimario());
   fase_sec = trafo->Fases(trafo->BarraSecundario());
   //verifica se Zona do primário não é trifásica
   if (zona_pri->Tipo != zonaTRIFASICA)
      {//define Zona do primário como monofásica
      if (! zona_pri->DefineTipoMonofasica(fase_pri, trafo->pbarra1->vnom, fase_pri, fase_sec)) return(false);
      }
   //verifica se a Zona do secundário não é trifásica
   if (zona_sec->Tipo != zonaTRIFASICA)
      {//define Zona do secundário como monofásica
      if (! zona_sec->DefineTipoMonofasica(fase_sec, trafo->sec.vnom, fase_pri, fase_sec)) return(false);
      }
   //verifica se Zona do primário é uma zonaDELTA
   if ((zona_pri->Tipo == zonaDELTA_ABERTO)||(zona_pri->Tipo == zonaDELTA_FECHADO))
      {//insere zona_pri em lisDELTA
      if (lisDELTA->IndexOf(zona_pri) < 0) lisDELTA->Add(zona_pri);
      }
   //verifica se Zona do secundário é uma zonaDELTA
   if ((zona_sec->Tipo == zonaDELTA_ABERTO)||(zona_sec->Tipo == zonaDELTA_FECHADO))
      {//insere zona_sec em lisDELTA
      if (lisDELTA->IndexOf(zona_sec) < 0) lisDELTA->Add(zona_sec);
      }
   //determina qual Barra do trafo se liga à Zona
   if (zona_pri == zona)
      {//verifica se zona do secundário é monofásica
      //if (zona_sec->Tipo == zonaMONOFASICA)
      //verifica se as fases do secundário incluem o neutro
      //if ((fase_sec & faseN) == faseN)
      //verifica se a fase da zona do secundário é inválida ou  inclui o neutro
      if ((zona_sec->Fases == faseINV) || ((zona_sec->Fases & faseN) == faseN))
         {//determina ângulos das fases do secundário em função das ligações do primário/secundário
         if (! trafo->AngulosNoSecundario(ang_grau))
            {
            //Aviso("Trafo com fases inconsistentes: " + AnsiQuotedStr(trafo->Codigo, '\''));
            return(true);
            }
         //Barra do primário ligada à zona indicada: calcula ângulo da zona do secundário
         ang_zona_grau[indFASE_A] = zona_pri->T_grau[indFASE_A] + ang_grau[indFASE_A];
         ang_zona_grau[indFASE_B] = zona_pri->T_grau[indFASE_A] + ang_grau[indFASE_B];
         ang_zona_grau[indFASE_C] = zona_pri->T_grau[indFASE_A] + ang_grau[indFASE_C];
         ang_zona_grau[indNEUTRO] = 0.;
         //define fases da Zona do secundário
         zona_sec->DefineAngulo_grau(ang_zona_grau);
         //define defasagem da Zona do secundario
         zona_sec->Defasagem_grau = zona_pri->Defasagem_grau + trafo->DefasagemPriSec();
         }
      }
   else if (zona_sec == zona)
      {//verifica se zona do primário é monofásica
      //if (zona_pri->Tipo == zonaMONOFASICA)
      //verifica se as fases do primário incluem o neutro
      //if ((fase_pri & faseN) == faseN)
      //verifica se a fase da zona do primário é inválida ou  inclui o neutro
      if ((zona_pri->Fases == faseINV) || ((zona_pri->Fases & faseN) == faseN))
         {//determina ângulos das fases do primário em função das ligações do primário/secundário
         if (! trafo->AngulosNoPrimario(ang_grau))
            {
            //Aviso("Trafo com fases inconsistentes: " + AnsiQuotedStr(trafo->Codigo, '\''));
            return(true);
            }
         //Barra do secundário ligada à zona indicada: calcula ângulo da zona do secundário
         ang_zona_grau[indFASE_A] = zona_sec->T_grau[indFASE_A] + ang_grau[indFASE_A];
         ang_zona_grau[indFASE_B] = zona_sec->T_grau[indFASE_A] + ang_grau[indFASE_B];
         ang_zona_grau[indFASE_C] = zona_sec->T_grau[indFASE_A] + ang_grau[indFASE_C];
         ang_zona_grau[indNEUTRO] = 0.;
         //define fases da Zona do primário
         zona_pri->DefineAngulo_grau(ang_zona_grau);
         //define defasagem da Zona do secundario
         zona_pri->Defasagem_grau = zona_sec->Defasagem_grau + trafo->DefasagemSecPri();
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TZonas::DefineVnomAnguloZonaTrafoReducao(VTZona *zona, VTTrafo *trafo)
   {
   //variáveis locais
   bool   sucesso;
   int    fase_pri, fase_sec;
   double defasagem_grau, ang_grau[MAX_FASE], ang_zona_grau[MAX_FASE];
   VTZona *zona_pri, *zona_sec;

   //proteção
   if (! trafo->Reducao) return(false);
   //ordena Barras do Trafo: pbar1 aponta p/ primário e pbar2 aponta p/ secundário
   trafo->OrdenaBar12PriSec();
   //determina Zonas do primário e secundário
   if ((zona_pri = (trafo->pbarra1) ? trafo->pbarra1->zona : NULL) == NULL) return(false);
   if ((zona_sec = (trafo->pbarra2) ? trafo->pbarra2->zona : NULL) == NULL) return(false);
   //determina defasagem em função das ligações do primário/secundário
   defasagem_grau = trafo->DefasagemPriSec();
   //determinas fases do primário e do secundário
   fase_pri = trafo->Fases(trafo->BarraPrimario());
   fase_sec = trafo->Fases(trafo->BarraSecundario());
   //define Zona do primário
   switch(trafo->pri.ligacao)
      {
      case lgEST_AT:
      case lgTRIANG:
      case lgEST_ISO:
         sucesso = zona_pri->DefineTipoEqvTrifasica(fase_pri, trafo->pri.vnom);
         break;
      case lgMONOFASICO:
         sucesso = zona_pri->DefineTipoEqvMonofasica(fase_pri, trafo->pri.vnom);
         break;
      case lgDELTA_ABERTO:
         sucesso = zona_pri->DefineTipoEqvDeltaAberto(fase_pri, trafo->pri.vnom);
         break;
      case lgDELTA_FECHADO:
         sucesso = zona_pri->DefineTipoEqvDeltaFechado(fase_pri,trafo->pri.vnom);
         break;
      default: return(false);
      }
   //define Zona do secundário
   switch(trafo->sec.ligacao)
      {
      case lgEST_AT:
      case lgTRIANG:
      case lgEST_ISO:
         sucesso = zona_sec->DefineTipoEqvTrifasica(fase_sec, trafo->sec.vnom);
         break;
      case lgMONOFASICO:
         sucesso = zona_sec->DefineTipoEqvMonofasica(fase_sec, trafo->sec.vnom);
         break;
      case lgDELTA_ABERTO:
         sucesso = zona_sec->DefineTipoEqvDeltaAberto(fase_sec, trafo->sec.vnom);
         break;
      case lgDELTA_FECHADO:
         sucesso = zona_sec->DefineTipoEqvDeltaFechado(fase_sec, trafo->sec.vnom);
         break;
      default: return(false);
      }
   //verifica se zona_sec é uma zonaDELTA
   if ((zona_sec->Tipo == zonaDELTA_ABERTO)||(zona_sec->Tipo == zonaDELTA_FECHADO))
      {//insere zona_sec em lisDELTA
      if (lisDELTA->IndexOf(zona_sec) < 0) lisDELTA->Add(zona_sec);
      }
   //determina ângulos das fases do secundário em função das ligações do primário/secundário
   if (! trafo->AngulosNoSecundario(ang_grau))
      {
      //Aviso("Trafo com fases inconsistentes: " + AnsiQuotedStr(trafo->Codigo, '\''));
      return(true);
      }
   //determina qual Barra do trafo se liga à Zona
   if (zona_pri == zona)
      {//Barra do primário ligada à zona indicada: calcula ãngulo da zona do secundário
      ang_zona_grau[indFASE_A] = zona_pri->T_grau[indFASE_A] + ang_grau[indFASE_A];
      ang_zona_grau[indFASE_B] = zona_pri->T_grau[indFASE_A] + ang_grau[indFASE_B];
      ang_zona_grau[indFASE_C] = zona_pri->T_grau[indFASE_A] + ang_grau[indFASE_C];
      ang_zona_grau[indNEUTRO] = 0.;
      //define fases da Zona do secundário
      zona_sec->DefineAngulo_grau(ang_zona_grau);
      //define defasagem da Zona do secundario
      zona_sec->Defasagem_grau = zona_pri->Defasagem_grau + defasagem_grau;
      }
   else if (zona_sec == zona)
      {//Barra do secundário ligada à zona indicada: calcula ãngulo da zona do secundário
      ang_zona_grau[indFASE_A] = zona_sec->T_grau[indFASE_A] - ang_grau[indFASE_A];
      ang_zona_grau[indFASE_B] = zona_sec->T_grau[indFASE_A] - ang_grau[indFASE_B];
      ang_zona_grau[indFASE_C] = zona_sec->T_grau[indFASE_A] - ang_grau[indFASE_C];
      ang_zona_grau[indNEUTRO] = 0.;
      //define fases da Zona do primário
      zona_pri->DefineAngulo_grau(ang_zona_grau);
      //define defasagem da Zona do primário
      zona_pri->Defasagem_grau = zona_sec->Defasagem_grau - defasagem_grau;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TZonas::DefineVnomAnguloZonaTrafoTrifasico(VTZona *zona, VTTrafo *trafo)
   {
   //variáveis locais
   int    fase_pri, fase_sec;
   double defasagem_grau, ang_zona_grau[MAX_FASE];
   VTZona *zona_pri, *zona_sec;

   //proteção
   if (trafo->Monofasico) return(false);
   //ordena Barras do Trafo: pbar1 aponta p/ primário e pbar2 aponta p/ secundário
   trafo->OrdenaBar12PriSec();
   //determina Zonas do primário e secundário
   if ((zona_pri = (trafo->pbarra1) ? trafo->pbarra1->zona : NULL) == NULL) return(false);
   if ((zona_sec = (trafo->pbarra2) ? trafo->pbarra2->zona : NULL) == NULL) return(false);
   //determinas fases do primário e do secundário
   fase_pri = trafo->Fases(trafo->BarraPrimario());
   fase_sec = trafo->Fases(trafo->BarraSecundario());
   //determina defasagem em função das ligações do primário/secundário
   defasagem_grau = trafo->DefasagemPriSec();
   //determina qual Barra do trafo se liga à Zona
   if (zona_pri == zona)
      {//define tipo da Zona do secundário
      if (! zona_sec->DefineTipoTrifasica(fase_sec, trafo->sec.vnom)) return(false);
      //Barra do primário ligada à zona indicada: calcula ãngulo da zona do secundário
      ang_zona_grau[indFASE_A] = zona_pri->T_grau[indFASE_A] + defasagem_grau;
      ang_zona_grau[indFASE_B] = zona_pri->T_grau[indFASE_B] + defasagem_grau;
      ang_zona_grau[indFASE_C] = zona_pri->T_grau[indFASE_C] + defasagem_grau;
      ang_zona_grau[indNEUTRO] = 0.;
      //define fases da Zona do secundário
      zona_sec->DefineAngulo_grau(ang_zona_grau);
      //define defasagem da Zona do secundario
      zona_sec->Defasagem_grau = zona_pri->Defasagem_grau + defasagem_grau;
      }
   else if (zona_sec == zona)
      {//define tipo da Zona do primário
      if (! zona_pri->DefineTipoTrifasica(fase_pri, trafo->pri.vnom)) return(false);
      //Barra do secundário ligada à zona indicada: calcula ãngulo da zona do secundário
      ang_zona_grau[indFASE_A] = zona_sec->T_grau[indFASE_A] - defasagem_grau;
      ang_zona_grau[indFASE_B] = zona_sec->T_grau[indFASE_B] - defasagem_grau;
      ang_zona_grau[indFASE_C] = zona_sec->T_grau[indFASE_C] - defasagem_grau;
      ang_zona_grau[indNEUTRO] = 0.;
      //define fases da Zona do primário
      zona_pri->DefineAngulo_grau(ang_zona_grau);
      //define defasagem da Zona do primário
      zona_pri->Defasagem_grau = zona_sec->Defasagem_grau - defasagem_grau;
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TZonas::DefineVnomAnguloZonaTrafo3E(VTZona *zona, VTTrafo3E* trafo)
   {
   //variáveis locais
   int    fase_pri, fase_sec, fase_ter;
   double def_prisec_grau, def_priter_grau, ang_zona_grau[MAX_FASE];
   //struct {double pri_sec, pri_ter;} defasagem;
   VTZona *zona_pri, *zona_sec, *zona_ter;

   //OBS: pbar1 aponta p/ primário, pbar2 p/ sencudário e pbar3 p/ terciário
   //determina Fases
   fase_pri = trafo->Fases(trafo->pbarra1);
   fase_sec = trafo->Fases(trafo->pbarra2);
   fase_ter = trafo->Fases(trafo->pbarra3);
   //determina Zonas do primário, secundário e terciário
   if ((zona_pri = (trafo->pbarra1) ? trafo->pbarra1->zona : NULL) == NULL) return(false);
   if ((zona_sec = (trafo->pbarra2) ? trafo->pbarra2->zona : NULL) == NULL) return(false);
   if ((zona_ter = (trafo->pbarra3) ? trafo->pbarra3->zona : NULL) == NULL) return(false);
   //determina defasagem em função das ligações do primário/secundário
   def_prisec_grau = trafo->DefasagemPriSec();
   //determina defasagem em função das ligações do primário/terciário
   def_priter_grau = trafo->DefasagemPriTer();
   //determina qual Zona do trafo é a Zona definida
   if (zona_pri == zona)
      {//Zona do primário definida: define tipo da Zona do secundário e terciário
      if (! zona_sec->DefineTipoTrifasica(fase_sec, trafo->sec.vnom)) return(false);
      if (! zona_ter->DefineTipoTrifasica(fase_ter, trafo->ter.vnom)) return(false);
      //Zona do primário definida: calcula ângulos da zona do secundário
      ang_zona_grau[indFASE_A] = zona_pri->T_grau[indFASE_A] + def_prisec_grau;
      ang_zona_grau[indFASE_B] = zona_pri->T_grau[indFASE_B] + def_prisec_grau;
      ang_zona_grau[indFASE_C] = zona_pri->T_grau[indFASE_C] + def_prisec_grau;
      ang_zona_grau[indNEUTRO] = 0.;
      //define fases da Zona do secundário
      zona_sec->DefineAngulo_grau(ang_zona_grau);;
      //define defasagem da Zona do secundario
      zona_sec->Defasagem_grau = zona_pri->Defasagem_grau + def_prisec_grau;
      //calcula ângulos da zona do terciário
      ang_zona_grau[indFASE_A] = zona_pri->T_grau[indFASE_A] + def_priter_grau;
      ang_zona_grau[indFASE_B] = zona_pri->T_grau[indFASE_B] + def_priter_grau;
      ang_zona_grau[indFASE_C] = zona_pri->T_grau[indFASE_C] + def_priter_grau;
      ang_zona_grau[indNEUTRO] = 0.;
      //define ângulos da Zona do terciário
      zona_ter->DefineAngulo_grau(ang_zona_grau);;
      //define defasagem da Zona do terciário
      zona_ter->Defasagem_grau = zona_pri->Defasagem_grau + def_priter_grau;
      }
   else if (zona_sec == zona)
      {//Zona do secundário definida: define tipo da Zona do primário e terciário
      if (! zona_pri->DefineTipoTrifasica(fase_pri, trafo->pri.vnom)) return(false);
      if (! zona_ter->DefineTipoTrifasica(fase_ter, trafo->ter.vnom)) return(false);
      //Zona do secundário definida: calcula ângulos da zona do primário
      ang_zona_grau[indFASE_A] = zona_sec->T_grau[indFASE_A] - def_prisec_grau;
      ang_zona_grau[indFASE_B] = zona_sec->T_grau[indFASE_B] - def_prisec_grau;
      ang_zona_grau[indFASE_C] = zona_sec->T_grau[indFASE_C] - def_prisec_grau;
      ang_zona_grau[indNEUTRO] = 0.;
      //define fases da Zona do primário
      zona_pri->DefineAngulo_grau(ang_zona_grau);
      //define defasagem da Zona do primário
      zona_pri->Defasagem_grau = zona_sec->Defasagem_grau - def_prisec_grau;
      //calcula ângulos da zona do terciário
      ang_zona_grau[indFASE_A] = zona_pri->T_grau[indFASE_A] + def_priter_grau;
      ang_zona_grau[indFASE_B] = zona_pri->T_grau[indFASE_B] + def_priter_grau;
      ang_zona_grau[indFASE_C] = zona_pri->T_grau[indFASE_C] + def_priter_grau;
      ang_zona_grau[indNEUTRO] = 0.;
      //define fases da Zona do terciário
      zona_ter->DefineAngulo_grau(ang_zona_grau);;
      //define defasagem da Zona do terciário
      zona_ter->Defasagem_grau = zona_pri->Defasagem_grau + def_priter_grau;
      }
   else if (zona_ter == zona)
      {//Zona do terciário definida: define tipo da Zona do primário e secundário
      if (! zona_pri->DefineTipoTrifasica(fase_pri, trafo->pri.vnom)) return(false);
      if (! zona_sec->DefineTipoTrifasica(fase_sec, trafo->sec.vnom)) return(false);
      //Zona do terciário definida: calcula ângulos da zona do primário
      ang_zona_grau[indFASE_A] = zona_ter->T_grau[indFASE_A] - def_priter_grau;
      ang_zona_grau[indFASE_B] = zona_ter->T_grau[indFASE_B] - def_priter_grau;
      ang_zona_grau[indFASE_C] = zona_ter->T_grau[indFASE_C] - def_priter_grau;
      ang_zona_grau[indNEUTRO] = 0.;
      //define fases da Zona do primário
      zona_pri->DefineAngulo_grau(ang_zona_grau);
      //define defasagem da Zona do primário
      zona_pri->Defasagem_grau = zona_ter->Defasagem_grau - def_priter_grau;
      //calcula ângulos da zona do secundário
      ang_zona_grau[indFASE_A] = zona_pri->T_grau[indFASE_A] + def_prisec_grau;
      ang_zona_grau[indFASE_B] = zona_pri->T_grau[indFASE_B] + def_prisec_grau;
      ang_zona_grau[indFASE_C] = zona_pri->T_grau[indFASE_C] + def_prisec_grau;
      ang_zona_grau[indNEUTRO] = 0.;
      //define fases da Zona do secundário
      zona_sec->DefineAngulo_grau(ang_zona_grau);
      //define defasagem da Zona do secundario
      zona_sec->Defasagem_grau = zona_pri->Defasagem_grau + def_prisec_grau;
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TZonas::EliminaDefasagemNegativa(void)
   {
   //variáveis locais
   double  ang_min_grau = 0;
   VTZona *zona;

   for (int n = 0; n < lisZON->Count; n++)
      {
      zona = (VTZona*)lisZON->Items[n];
      if (zona->Defasagem_grau < ang_min_grau) ang_min_grau = zona->Defasagem_grau;
      }
   if (IsDoubleZero(ang_min_grau)) return;
   //corrige defasagens das Zonas
   ang_min_grau = fabs(ang_min_grau);
   for (int n = 0; n < lisZON->Count; n++)
      {
      zona = (VTZona*)lisZON->Items[n];
      zona->RotacionaT_grau(ang_min_grau);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TZonas::Executa(VTRedes *redes)
   {
   //variáveis locais
   int          num_zona, num_trf;
   TList        *lisMNET;
   VTBarra      *pbarra;
   VTChave      *chave;
   VTEqbar      *eqbar;
   VTMNet       *mnet;
   //VTCronometro *cronometro = (VTCronometro*)apl->GetObject(__classid(VTCronometro));

   //if (cronometro) cronometro->Reinicia();
   //if (cronometro) cronometro->Start("Zonas 1", "Criação das zonas");
   //se necessário, obtém objeto VTRedes
   if (redes == NULL)
      {
      redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
      }
   //salva ponteiro para VTRedes
   this->redes = redes;
   //cancela Zona de todas as Barras
   CancelaAssociacaoBarraZona(redes->LisBarra());
   //reinicia listas de Zonas do tipo delta aberto ou fechado
   lisDELTA->Clear();
   //destrói Zonas criadas anteriormente
   LimpaTList(lisZON);
   //inicia lisSUP e lisGER com suprimentos e geradores
   IniciaLisSuprimentoGerador();
   //monta lista lisTRF c/ todos Trafo e Trafo3E
   IniciaLisTrafo(lisTRF);
   //obtém lista de MNets
   lisMNET = redes->LisMNetFluxo();
   //ATENÇÃO: em redes sem geração, não existe nenhuma MNet
   if (lisMNET->Count == 0)
      {//cria Zonas a partir de todas Barras/Ligacoes de Redes
      IniciaLisBarraLigacao(redes, lisBAR, lisLIG);
      while (lisBAR->Count > 0)
         {//determina primeira Barra da lista
         pbarra = (VTBarra*)lisBAR->First();
         //cria uma nova Zona à partir da Barra
         InsereZona(pbarra);
         }
      }
   else
      {//loop / todas as MNets
      for (int n = 0; n < lisMNET->Count; n++)
         {
         mnet = (VTMNet*)lisMNET->Items[n];
         //inicia lista de Ligações da Rede (exceto Trafos, Trafo3E) e lista de Barras ordenadas por Zona
         IniciaLisBarraLigacao(mnet, lisBAR, lisLIG, num_zona, num_trf);
         //analiza Barras da Rede e cria as Zonas necessárias
         while (lisBAR->Count > 0)
            {//determina primeira Barra da lista
            pbarra = (VTBarra*)lisBAR->First();
            //cria uma nova Zona à partir da Barra
            InsereZona(pbarra);
            }
         }
      }
   //proteção contra Suprimentos/Geradores conectados a barras isoladas
   for (int n = 0; n < lisSUP->Count; n++)
      {
      eqbar = (VTEqbar*)lisSUP->Items[n];
      if (eqbar->pbarra->zona == NULL)
         {//cria uma nova Zona à partir da Barra
         InsereZona(eqbar->pbarra);
         }
      }
   for (int n = 0; n < lisGER->Count; n++)
      {
      eqbar = (VTEqbar*)lisGER->Items[n];
      if (eqbar->pbarra->zona == NULL)
         {//cria uma nova Zona à partir da Barra
         InsereZona(eqbar->pbarra);
         }
      }
   //verifica se deve considerar chaves abertas na montagem das zonas
   if (ChaveAbertaEnabled)
      {//obtém lista de Chaves de todas MNets
      lisLIG->Clear();
      for (int n = 0; n < lisMNET->Count; n++)
         {
         mnet = (VTMNet*)lisMNET->Items[n];
         mnet->LisLigacao(lisLIG, eqptoCHAVE);
         }
      //loop p/ todas Chaves
      for (int n = 0; n < lisLIG->Count; n++)
         {
         chave = (VTChave*)lisLIG->Items[n];
         if (chave->Aberta)
            {
            if ((chave->pbarra1->zona != NULL)&&(chave->pbarra2->zona == NULL))
               {//inclui Barra2 na mesma Zona da Barra 1
               chave->pbarra1->zona->InsereBarra(chave->pbarra2);
               }
            else if ((chave->pbarra2->zona != NULL)&&(chave->pbarra1->zona == NULL))
               {//inclui Barra2 na mesma Zona da Barra 1
               chave->pbarra2->zona->InsereBarra(chave->pbarra1);
               }
            else if ((chave->pbarra1->zona != NULL)&&(chave->pbarra2->zona != NULL))
               {
               FundeZonas(chave->pbarra1->zona, chave->pbarra2->zona);
               }
            }
         }
      }
   //if (cronometro) cronometro->Stop("Zonas 1");
   //if (cronometro) cronometro->Show();
   //define Vnom, ângulo e fases das Zonas
   if (! DefineVnomAnguloFases()) return(false);
   //ajusta defasagens das Zonas p/ que não permaneça nenhuma Zona com defasagem negativa
//alteração OpenDSS
//   EliminaDefasagemNegativa();
   //verifica se há Zona sem aterramento
   VerificaZonaSemTerra();
   return(true);
   }

//---------------------------------------------------------------------------
VTZona* __fastcall TZonas::ExisteZona(VTBarra *barra)
   {
   //variáveis locais
   VTZona *zona;

   //procura Zona que contéma a Barra
   for (int n = 0; n < lisZON->Count; n++)
      {
      zona = (VTZona*)lisZON->Items[n];
      if (zona->ExisteBarra(barra)) return(zona);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTZona* __fastcall TZonas::ExisteZonaReferencia(void)
   {
   //variáveis locais
   int          fases_zona, fases_ger;
   VTGerador    *gerador;
   VTSuprimento *suprimento;
   VTZona       *zona;

   //descarta Suprimentos cujas zonas já foram definidas
   for (int n = lisSUP->Count-1; n >= 0; n--)
      {
      suprimento = (VTSuprimento*)lisSUP->Items[n];
      //verifica se a Zona já foi definida
      zona = suprimento->pbarra->zona;
      //if ((zona == NULL)||(zona->AnguloDefinido()))  //BUG
      if ((zona != NULL)&&(zona->AnguloDefinido()))
         {//complementa fases da zona
         zona->DefineNeutroAterrado();
         //retira Suprimento de lisSUP
         lisSUP->Delete(n);
         //verifica se a Zona não é trifásica
         if (! zona->Trifasica)
            {//redefine Zona como trifásica
            zona->DefineTipoTrifasica(faseABCNT, suprimento->pbarra->vnom);
            }
         }
      }
   //descarta Geradores cujas zonas já foram definidas
   for (int n = lisGER->Count-1; n >= 0; n--)
      {
      gerador = (VTGerador*)lisGER->Items[n];
      //verifica se a Zona já foi definida
      zona = gerador->pbarra->zona;
      if ((zona != NULL)&&(zona->AnguloDefinido()))
         {//complementa fases da zona
         zona->DefineNeutroAterrado();
         //retira Gerador de lisGER
         lisGER->Delete(n);
         //JCG 2019.04.18 - tratamento para gerador monofásico
         //verifica se a Zona não é trifásica
         if (! zona->Trifasica)
            {//verifica se o Gerador é trifásico
            if ((gerador->Fases & faseABC) == faseABC)
               {//redefine Zona como trifásica
               zona->DefineTipoTrifasica(faseABCNT, gerador->pbarra->vnom);
               }
            else
               {//inclui fases do gerador na Zona
               zona->RedefineFases(zona->Fases | gerador->Fases);
               }
            }
         }
      }
   //verifica se existe Suprimento de Reducao do tipos VF cuja zona ainda não foi definida
   for (int n = lisSUP->Count-1; n >= 0; n--)
      {
      suprimento = (VTSuprimento*)lisSUP->Items[n];
      //verifica se é um Suprimento de Reducao
      if ((suprimento->Reducao)&&(suprimento->tiposup == supVF))
         {//retira Suprimento de lisSUP
         lisSUP->Delete(n);
         //inicia Zona de referência
         IniciaZonaReferencia(suprimento->pbarra->zona, suprimento);
         //retorna Zona de referência
         return(suprimento->pbarra->zona);
         }
      }
   //verifica se existe Suprimento de tipo VF cuja zona ainda não definida
   for (int n = lisSUP->Count-1; n >= 0; n--)
      {
      suprimento = (VTSuprimento*)lisSUP->Items[n];
      //verifica se é um Suprimento de Reducao
      if (suprimento->tiposup == supVF)
         {//retira Suprimento de lisSUP
         lisSUP->Delete(n);
         //inicia Zona de referência
         IniciaZonaReferencia(suprimento->pbarra->zona, suprimento);
         //retorna Zona de referência
         return(suprimento->pbarra->zona);
         }
      }
   //verifica se existe Gerador de tipo VF cuja zona ainda não definida
   for (int n = lisGER->Count-1; n >= 0; n--)
      {
      gerador = (VTGerador*)lisGER->Items[n];
      if (gerador->tiposup == supVF)
         {//retira Gerador de lisGER
         lisGER->Delete(n);
         //inicia Zona de referência
         IniciaZonaReferencia(gerador->pbarra->zona, gerador);
         //retorna Zona de referência
         return(gerador->pbarra->zona);
         }
      }
   //verifica se existe Suprimento de tipo PV ou PQ cuja zona ainda não definida
   for (int n = lisSUP->Count-1; n >= 0; n--)
      {
      suprimento = (VTSuprimento*)lisSUP->Items[n];
      //verifica se é um Suprimento de Reducao
      if (suprimento->tiposup != supVF)
         {//retira Suprimento de lisSUP
         lisSUP->Delete(n);
         //inicia Zona de referência
         IniciaZonaReferencia(suprimento->pbarra->zona, suprimento);
         //retorna Zona de referência
         return(suprimento->pbarra->zona);
         }
      }
   //verifica se existe Gerador de tipo PV ou PQ cuja zona ainda não definida
   for (int n = lisGER->Count-1; n >= 0; n--)
      {
      gerador = (VTGerador*)lisGER->Items[n];
      if (gerador->tiposup != supVF)
         {//retira Gerador de lisGER
         lisGER->Delete(n);
         //inicia Zona de referência
         IniciaZonaReferencia(gerador->pbarra->zona, gerador);
         //retorna Zona de referência
         return(gerador->pbarra->zona);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TZonas::FundeZonas(VTZona *zona1, VTZona *zona2)
   {
   //variáveis locais
   TList *lisBAR;

   //proteção
   if (zona1 == zona2) return;
   //verifica se as duas Zonas tem o mesmo tipo
   if (zona1->Tipo != zona2->Tipo) return;
   //verifica se as duas Zonas tem as mesmas defasagens
   if (! IsDoubleZero(zona1->Defasagem_grau - zona2->Defasagem_grau)) return;
   //verifica se as duas Zonas tem as mesmas tensões
   if (! IsDoubleZero(zona1->Vff_kv - zona2->Vff_kv)) return;
   for (int ind_fase = 0; ind_fase < MAX_FASE-1; ind_fase++)
      {
      if (! IsDoubleZero(zona1->Vfn_kv[ind_fase] - zona2->Vfn_kv[ind_fase])) return;
      }
   //incorpora Barras da zona2 na zona1
   lisBAR = zona2->LisBarra();
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      zona1->InsereBarra((VTBarra*)lisBAR->Items[nb]);
      }
   //elimina a zona2
   lisZON->Remove(zona2);
   delete zona2;
   }

//---------------------------------------------------------------------------
void __fastcall TZonas::IniciaLisBarraLigacao(VTRedes *redes, TList *lisB, TList *lisL)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //inicia lista de Barras
   lisB->Clear();
   redes->LisBarra(lisB);
   //inicia lista de Ligacoes c/ todas Ligacoes, exceto Trafos, Trafo3E
   lisL->Clear();
   redes->LisLigacao(lisL);
   for (int n = lisL->Count-1; n >= 0; n--)
      {
      ligacao = (VTLigacao*)lisL->Items[n];
      if ((ligacao->Tipo() == eqptoTRAFO)||(ligacao->Tipo() == eqptoTRAFO3E))
         {//retira Ligacao da lista
         lisL->Delete(n);
         }
      }
   }
//---------------------------------------------------------------------------
void __fastcall TZonas::IniciaLisBarraLigacao(VTMNet *mnet, TList *lisB, TList *lisL, int &num_zona, int &num_trf)
   {
   //variáveis locais
   bool      barra_isolada = false;
   VTBarra   *barra;
   VTLigacao *ligacao;
   TList     *lisLIG;
   TList     *lisREDU;

   //reinicia parâmetros que serão retornados
   num_zona = num_trf = 0;
   //monta lisB com as Barras da MNet, SEM as basras isoladas, colocando no início as Barras já associadas com Zona
   lisB->Clear();
   lisTMP->Clear(); mnet->LisBarra(lisTMP, barra_isolada);
   for (int nb = 0; nb < lisTMP->Count; nb++)
      {
      barra = (VTBarra*)lisTMP->Items[nb];
      if (barra->zona == NULL) {lisB->Add(barra);}
      else                     {lisB->Insert(0, barra);num_zona++;}
      }
   //monta lista lisL c/ as Ligacoes da MNet, exceto  Trafo, Trafo3E e Chaves abertas
   lisL->Clear();
   lisTMP->Clear(); mnet->LisLigacao(lisTMP);
   for (int n = 0; n < lisTMP->Count; n++)
      {
      ligacao = (VTLigacao*)lisTMP->Items[n];
      if (ligacao->Tipo() == eqptoTRAFO)   {num_trf++; continue;}
      if (ligacao->Tipo() == eqptoTRAFO3E) {num_trf++; continue;}
      if (ligacao->Tipo() == eqptoCHAVE)
         {//2014/04/03 - NÃO INCLUI chave aberta na lista de Ligacoes pois isso pode causar erro na
         //montagem das Zonas a partir das MNets vizinhas
         if (((VTChave*)ligacao)->Aberta) continue;
         }
      //insere Ligacao em lisLIG
      lisL->Add(ligacao);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TZonas::IniciaLisTrafo(TList *lisDES)
   {
   //variáveis locais
   VTMNet *mnet;
   TList  *lisMNET = redes->LisMNetFluxo();

   //reincia lisDES
   lisDES->Clear();
   for (int n = 0; n < lisMNET->Count; n++)
      {
      mnet = (VTMNet*)lisMNET->Items[n];
      mnet->LisLigacao(lisDES, eqptoTRAFO);
      mnet->LisLigacao(lisDES, eqptoTRAFO3E);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TZonas::IniciaLisTrafoZZ(TList *lisDES)
   {
   //variáveis locais
   VTMNet *mnet;
   TList  *lisMNET = redes->LisMNetFluxo();

   //reincia lisDES
   lisDES->Clear();
   for (int n = 0; n < lisMNET->Count; n++)
      {
      mnet = (VTMNet*)lisMNET->Items[n];
      mnet->LisEqbar(lisDES, eqptoTRAFOZZ);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TZonas::IniciaLisSuprimentoGerador(void)
   {
   //variáveis locais
   VTMNet *mnet;
   TList  *lisMNET = redes->LisMNetFluxo();

   //reinicia lista lisINV
   lisINV->Clear();
   //reincia lisSUP e lisGER
   lisSUP->Clear();
   lisGER->Clear();
   for (int n = 0; n < lisMNET->Count; n++)
      {
      mnet = (VTMNet*)lisMNET->Items[n];
      mnet->LisEqbar(lisSUP, eqptoSUPRIMENTO);
      mnet->LisEqbar(lisGER, eqptoGERADOR);
      }
   }

/*
//---------------------------------------------------------------------------
void __fastcall TZonas::IniciaLisSuprimentoGerador(void)
   {
   //variáveis locais
   VTMNet       *mnet;
   VTGerador    *gerador;
   VTSuprimento *suprimento;
   TList        *lisMNET = redes->LisMNetFluxo();

   //reinicia lista lisINV
   lisINV->Clear();
   //reincia lisSUP e lisGER
   lisSUP->Clear();
   lisGER->Clear();
   for (int n = 0; n < lisMNET->Count; n++)
      {
      mnet = (VTMNet*)lisMNET->Items[n];
      mnet->LisEqbar(lisSUP, eqptoSUPRIMENTO);
      mnet->LisEqbar(lisGER, eqptoGERADOR);
      }
   //descarta Suprimentos que não são do tipo VF
   for (int n = lisSUP->Count - 1; n >= 0; n--)
      {
      suprimento = (VTSuprimento*)lisSUP->Items[n];
      if (suprimento->tiposup != supVF)
         {
         lisSUP->Delete(n);
         }
      }
   //descarta Geradores que não são do tipo VF
   for (int n = lisGER->Count - 1; n >= 0; n--)
      {
      gerador = (VTGerador*)lisGER->Items[n];
      if (gerador->tiposup != supVF)
         {
         lisGER->Delete(n);
         }
      }
   }
*/

//---------------------------------------------------------------------------
void __fastcall TZonas::IniciaZonaReferencia(VTZona *zona, VTGerador *gerador)
   {
   //variáveis locais
   double         vnom_kv;
   double         ang_zona_grau[MAX_FASE] = {0, -120, 120, 0};

   //define ângulo de rotação nulo (zona é de referência !!!)
   zona->Defasagem_grau = 0;
   //determina tensão nominal p/ zona
   if (IsDoubleZero(gerador->pbarra->vnom))
      {//redefine tensão nominal da Barra
      gerador->pbarra->vnom = gerador->vnom;
      }
   vnom_kv = gerador->pbarra->vnom;
   //verifica se gerador trifásico
   if ((gerador->Fases & faseABC) == faseABC)
      {//define Zona de referência como trifásica, c/ fases ABCNT
      zona->DefineTipoTrifasica(faseABCNT, vnom_kv);
      }
   else
      {//define Zona de referência como monofásica
      zona->DefineTipoMonofasica(gerador->Fases | faseN | faseT, vnom_kv, gerador->Fases, gerador->Fases);
      }
   //define ângulos das fases da Zona
   zona->DefineAngulo_grau(ang_zona_grau);
   }

//---------------------------------------------------------------------------
void __fastcall TZonas::IniciaZonaReferencia(VTZona *zona, VTSuprimento *suprimento)
   {
   //variáveis locais
   double         vnom_kv;
   double         ang_zona_grau[MAX_FASE] = {0, -120, 120, 0};

   //define ângulo de rotação nulo (zona é de referência !!!)
   zona->Defasagem_grau = 0;
   //determina tensão nominal p/ zona
   if (IsDoubleZero(suprimento->pbarra->vnom))
      {//redefine tensão nominal da Barra
      suprimento->pbarra->vnom = suprimento->vnom;
      }
   vnom_kv = suprimento->pbarra->vnom;
   //verifica se suprimento trifásico
   if ((suprimento->Fases & faseABC) == faseABC)
      {//define Zona de referência como trifásica, c/ fases ABCNT
      zona->DefineTipoTrifasica(faseABCNT, vnom_kv);
      }
   else
      {//define Zona de referência como monofásica
      zona->DefineTipoMonofasica(suprimento->Fases | faseN | faseT, vnom_kv, suprimento->Fases, suprimento->Fases);
      }
   //define ângulos das fases da Zona
   zona->DefineAngulo_grau(ang_zona_grau);
   }

//---------------------------------------------------------------------------
bool __fastcall TZonas::InsereZona(VTBarra *pbarra)
   {
   //variáveis locais
   TList     *lisBAR_ZONA;
   VTBarra   *bar_lig;
   VTLigacao *pliga;
   VTZona    *zona;

   //verifica se a Barra já possui uma Zona
   if ((zona = pbarra->zona) == NULL)
      {//cria uma nova Zona à partir da Barra
      if ((zona = NewObjZona()) == NULL) return(false);
      //insere Zona na lista
      lisZON->Add(zona);
      //insere Barra na Zona
      zona->InsereBarra(pbarra);
      //retira Barra da lista lisBAR
      lisBAR->Remove(pbarra);
      }
   else
      {//move Barra para o final da lista de Barras da Zona
      lisBAR_ZONA = zona->LisBarra();
      lisBAR_ZONA->Remove(pbarra);
      lisBAR_ZONA->Add(pbarra);
      //retira Barra da lista lisBAR
      lisBAR->Remove(pbarra);
      }
   //loop p/ todas as Barras em lisBAR_ZONA, a partir da Barra indicada
   lisBAR_ZONA = zona->LisBarra();
   for (int nbar = lisBAR_ZONA->Count - 1; nbar < lisBAR_ZONA->Count; nbar++)
      {
      pbarra = (VTBarra*)lisBAR_ZONA->Items[nbar];
      //determina todas as Ligações conectadas à Barra (NÃO há chaves abertas)
      for (int nlig = lisLIG->Count-1; nlig >= 0; nlig--)
         {
         pliga = (VTLigacao*)lisLIG->Items[nlig];
         //verifica se a Ligação conecta a Barra
         if (! pliga->ConectaBarra(pbarra)) continue;
         //retira Ligação da lista
         lisLIG->Delete(nlig);
         //Ligacao conecta a Barra: inclui Barras na Zona
         for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
            {
            if ((bar_lig = pliga->Barra(ind_bar)) == NULL) continue;
            if (bar_lig == pbarra)     continue;
            if (bar_lig->zona == NULL)
               {//insere nova Barra na Zona
               zona->InsereBarra(bar_lig);
               }
            //retira Barra da lista de Barras
            lisBAR->Remove(bar_lig);
            }
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TZonas::InsereZona(TList *lisBARRA)
   {
   //variáveis locais
   VTZona  *zona;

   //cria uma nova Zona
   if ((zona = NewObjZona()) == NULL) return(false);
   //insere Zona em lisZON
   lisZON->Add(zona);
   //insere Barras na Zona
   for (int nb = 0; nb < lisBARRA->Count; nb++)
      {
      zona->InsereBarra((VTBarra*)lisBARRA->Items[nb]);
      }
   return(true);
   }

//---------------------------------------------------------------------------
TList* __fastcall TZonas::LisZona(void)
   {
   return(lisZON);
   }
   
//---------------------------------------------------------------------------
bool __fastcall TZonas::TrafoConectaZona(VTLigacao *pliga, VTZona *zona)
   {
   //verifica se alguma das Barras da Ligação conecta a zona
   if ((pliga->pbarra1)&&(pliga->pbarra1->zona == zona)) return(true);
   if ((pliga->pbarra2)&&(pliga->pbarra2->zona == zona)) return(true);
   if ((pliga->pbarra3)&&(pliga->pbarra3->zona == zona)) return(true);
   return(false);
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/18599986
void __fastcall TZonas::VerificaZonaSemTerra(void)
   {
   //variáveis locais
   TList   *lisBAR;
   VTBarra *barra;
   VTZona  *zona;

   //verifica se há Zona sem aterramento
   for (int n = 0; n < lisZON->Count; n++)
      {
      zona = (VTZona*)lisZON->Items[n];
      if (zona->Tipo == zonaINDEFINIDA) continue;
      if ((zona->Fases & faseT) == faseINV)
         {//indica uma Barra da zona
         lisBAR = zona->LisBarra();
         if (lisBAR->Count > 0)
            {
            barra = (VTBarra*)lisBAR->First();
            Alerta("Há região de rede sem aterramento: barra " + AnsiQuotedStr(barra->Codigo, '\''));
            return;
            }
         }
      }
   }

//------------------------------------------------------------------------------
// eof



