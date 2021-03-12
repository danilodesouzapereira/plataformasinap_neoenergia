//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaChave.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTTipoChave.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaChave(VTApl *apl)
   {
   try{
      return (new TMontaChave(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaChave::TMontaChave(VTApl *apl)
                       :TMontaLigacao(apl)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TMontaChave::~TMontaChave(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaChave::IniciaGrupo(VTEqpto *eqpto)
   {
   return(TMontaLigacao::IniciaGrupo(eqpto));
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaChave::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Chave
   this->chave = (VTChave*)eqpto;

   //insere atributos do Eqpto
   TMontaEqpto::LisAtributosBasico(eqpto, lisATR);
   LisAtributosEqpto(eqpto, lisATR);
   //insere atributos de fluxo
   LisAtributosFluxo(eqpto, lisATR);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaChave::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Chave
   this->chave = (VTChave*)eqpto;

   //atributos de Eqpto
   TMontaLigacao::LisAtributosEqpto(eqpto, lisATR);
   //atributos de Chave
   lisATR->Add(new TAtributo(chave, chv_TIPO,  true, false, "Tipo da chave"));
   lisATR->Add(new TAtributo(chave, chv_INOM,  true, false, "Corrente nominal", "(A)"));
   lisATR->Add(new TAtributo(chave, chv_ESTADO,true, false, "Estado"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaChave::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Chave
   this->chave = (VTChave*)eqpto;

   //atributos de fluxo da Ligacao
   TMontaLigacao::LisAtributosFluxo(eqpto, lisATR);
   //atributos de fluxo de Chave

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaChave::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //monta lista de valores do Eqpto
   LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores de Fluxo
   LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaChave::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->chave   = (VTChave*)eqpto;
   try{//dados do Eqpto
      TMontaLigacao::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
      //dados da Chaves
      if (chave->Show[chv_TIPO])   ValorTipoDaChave();
      if (chave->Show[chv_INOM])   ValorCorrenteNominal();
      if (chave->Show[chv_ESTADO]) ValorEstado();
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaChave::LisValoresFluxo(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //não há dados específicos da Chave
   TMontaLigacao::LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaChave::LisVetValFluxo(VTEqpto *eqpto, TList *lisEXT, int ind_atrib, int ano)
   {
   //não há dados específicos da Chave
   TMontaLigacao::LisVetValFluxo(eqpto, lisEXT, ind_atrib, ano);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall  TMontaChave::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   //variáveis locais
   AnsiString  valor, unidade;
   VTChave    *chave = (VTChave*)eqpto;

   //determina valor de atributo
   switch(ind_atrib)
      {
      case eqp_ID:     valor = IntToStr(eqpto->Id);                     break;
      case eqp_CODIGO: valor = eqpto->Codigo;                           break;
      case lig_REDE:   valor = chave->rede->Codigo;                     break;
      case chv_TIPO:   valor = chave->TipoChave->Codigo;                break;
      case chv_INOM:   valor = DoubleToStr("", chave->Inom_amp);        break;
      case chv_ESTADO: valor = (chave->Aberta) ? "Aberta" : "Fechada";  break;
      }
   return(valor);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaChave::ValorCorrenteNominal(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Inom"));
   //define unidade e valor
   aue.Corrente_A(chave->Inom_amp, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaChave::ValorEstado(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Estado"));
   //define unidade e valor
   dado->valor   = (chave->Aberta) ? "Aberta" : "Fechada";
   dado->unidade = "";
   }

//---------------------------------------------------------------------------
void __fastcall TMontaChave::ValorTipoDaChave(void)
   {
   //variáveis locais
   VTDado  *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Tipo"));
   dado->valor   = chave->TipoChave->Codigo;
   dado->unidade = "";
   //verifica se base fusível
   if (chave->TipoChave->Tipo == chaveBF)
      {//verifica se foi definido o elo
      if (! chave->EloFusivel.IsEmpty())
         {//inclui informação do elo
         dado->valor = dado->valor + " " + chave->EloFusivel;
         }
      }
   }

//---------------------------------------------------------------------------
//eof


