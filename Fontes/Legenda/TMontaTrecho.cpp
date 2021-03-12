//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaTrecho.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTGrupo.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Constante\Fases.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaTrecho(VTApl *apl)
   {
   try{
      return (new TMontaTrecho(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaTrecho::TMontaTrecho(VTApl *apl)
                        :TMontaLigacao(apl)
   {
   //salva ponteiro p/ objetos
   fases = (VTFases*)apl->GetObject(__classid(VTFases));
   //cria objetos
   monta_arranjo = NewObjMontaArranjo(apl);
   }

//---------------------------------------------------------------------------
__fastcall TMontaTrecho::~TMontaTrecho(void)
   {
   //destrói objetos
   if (monta_arranjo) {delete monta_arranjo; monta_arranjo = NULL;}
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaTrecho::IniciaGrupo(VTEqpto *eqpto)
   {
   //variáveis locais
   VTGrupo *grupo_tre, *grupo_cad, *grupo_arr;
   TList   *lisGRUPO;

   //salva parâmetros
   trecho = (VTTrecho*)eqpto;
   grupo_tre  = TMontaLigacao::IniciaGrupo(eqpto);
   //verifica se deve incluir informações do Arranjo
   if (eqpto->Show[tre_ARRANJO])
      {//cria grupo de Arranjo e insere no grupo de cadastro
      grupo_arr = monta_arranjo->IniciaGrupo(trecho->arranjo);
      lisGRUPO  = grupo_tre->LisGrupo;
      grupo_cad = (VTGrupo*)lisGRUPO->First();
      grupo_cad->Add(grupo_arr);
      }
   return(grupo_tre);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaTrecho::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Trecho
   trecho = (VTTrecho*)eqpto;
   //atributos de Eqpto
   TMontaEqpto::LisAtributosBasico(eqpto, lisATR);
   LisAtributosEqpto(eqpto, lisATR);
   //atributos de fluxo
   LisAtributosFluxo(eqpto, lisATR);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaTrecho::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Trecho
   trecho = (VTTrecho*)eqpto;
   //atributos de Eqpto
   TMontaLigacao::LisAtributosEqpto(eqpto, lisATR);
   //atributos de Trecho
   lisATR->Add(new TAtributo(trecho, tre_COMP,     true, false, "Comprimento", "(km)"));
   lisATR->Add(new TAtributo(trecho, tre_INOM,     true, false, "Inom", "A"));
   lisATR->Add(new TAtributo(trecho, tre_FASES,    true, false, "Fases"));
   lisATR->Add(new TAtributo(trecho, tre_ARRANJO,  true, false, "Arranjo"));
   lisATR->Add(new TAtributo(trecho, tre_ZSEQ_OHM, true, false, "Zseq", "(ohm)"));
   lisATR->Add(new TAtributo(trecho, tre_ZSEQ_PU,  true, false, "Zseq", "(pu)"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaTrecho::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Trecho
   trecho = (VTTrecho*)eqpto;
   //atributos de fluxo da Ligacao
   TMontaLigacao::LisAtributosFluxo(eqpto, lisATR);
   //atributos de fluxo de Capserie

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaTrecho::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //monta lista de valores do Eqpto
   LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores de Fluxo
   LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaTrecho::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->trecho     = (VTTrecho*)eqpto;
   try{//dados do Eqpto
      TMontaLigacao::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
      //dados gerais do trecho
      if (trecho->Show[tre_COMP])     ValorComprimento();
      if (trecho->Show[tre_INOM])     ValorIadm();
      if (trecho->Show[tre_FASES])    ValorFases();
      if (trecho->Show[tre_ARRANJO])  ValorArranjo();
      if (trecho->Show[tre_ZSEQ_OHM]) ValorZseq(ueZ_OHM);
      if (trecho->Show[tre_ZSEQ_PU])  ValorZseq(ueZ_PU);
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }
/*
//---------------------------------------------------------------------------
AnsiString __fastcall TMontaTrecho::TipoMonoBiTrifasico(int fases)
   {
   //variáveis locais
   AnsiString tipo_mbt;

   switch(fases)
      {
      case faseA:
      case faseB:
      case faseC:
      case faseAN:
      case faseBN :
		case faseCN:
			tipo_mbt = "Monofásico";
         break;
		case faseAB:
		case faseBC:
		case faseCA:
		case faseABN:
		case faseBCN:
		case faseCAN:
			tipo_mbt = "Bifásico";
         break;
      case faseABC:
      case faseABCN:
			tipo_mbt = "Trifásico";
         break;
      default:
         tipo_mbt = "Indefinido";
      }
   return(tipo_mbt);
   }
*/

//---------------------------------------------------------------------------
AnsiString __fastcall TMontaTrecho::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   //variáveis locais
   AnsiString valor;
   VTTrecho   *trecho = (VTTrecho*)eqpto;

   //determina valor de atributo
   switch(ind_atrib)
      {
      case eqp_ID:      valor = IntToStr(eqpto->Id);                             break;
      case eqp_CODIGO:  valor = eqpto->Codigo;                                   break;
      case lig_REDE:    valor = trecho->rede->Codigo;                            break;
      case tre_COMP:    valor = DoubleToStr("%6.5f", trecho->Comprimento_km);    break;
      case tre_ARRANJO: valor = trecho->arranjo->Codigo;                         break;
      case tre_INOM:    valor = DoubleToStr("%2.1f", trecho->arranjo->Iadm_amp); break;
      case tre_FASES:   valor = fases->AsStrUpper[trecho->arranjo->Fases];       break;
      }
   return(valor);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrecho::ValorArranjo(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Arranjo"));
   //define unidade e valor
   dado->valor   = trecho->arranjo->Codigo;
   dado->unidade = "";
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrecho::ValorZseq(int unidade)
   {
   ValorZseq0(unidade);
   ValorZseq1(unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrecho::ValorZseq0(int unidade)
   {
   //variáveis locais
   double  r0, x0, y0;
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Zseq0"));
   //verifica se o Trecho é trifásico
   if ((trecho->arranjo->TipoCfg == arranjoZ0Z1)||
       ((trecho->arranjo->Fases & faseABC) == faseABC))
      {//verifica a unidade
      if (unidade == ueZ_OHM)
         {//obtém impedância em ohm
         trecho->Z0_ohm(r0, x0, y0);
         //define unidade e valor
         aue.Impedancia_OHM(complex<double>(r0, x0), dado->valor, dado->unidade);
         }
      else
         {//obtém impedância em pu
         trecho->Z0_pu(Sbase100MVA, trecho->pbarra1->vnom, r0, x0, y0);
         //define unidade e valor
         aue.Impedancia_PU(complex<double>(r0, x0), dado->valor, dado->unidade);
         }
      }
   else
      {//Trecho
      dado->valor    = "não aplicável";
      dado->unidade  = "";
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrecho::ValorZseq1(int unidade)
   {
   //variáveis locais
   double  r1, x1, y1;
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Zseq1"));
   //verifica se o Trecho é trifásico
   if ((trecho->arranjo->TipoCfg == arranjoZ0Z1)||
       ((trecho->arranjo->Fases & faseABC) == faseABC))
      {//verifica a unidade
      if (unidade == ueZ_OHM)
         {//obtém impedância em ohm
         trecho->Z1_ohm(r1, x1, y1);
         //define unidade e valor
         aue.Impedancia_OHM(complex<double>(r1, x1), dado->valor, dado->unidade);
         }
      else
         {//obtém impedância em pu
         trecho->Z1_pu(Sbase100MVA, trecho->pbarra1->vnom, r1, x1, y1);
         //define unidade e valor
         aue.Impedancia_PU(complex<double>(r1, x1), dado->valor, dado->unidade);
         }
      }
   else
      {//Trecho
      dado->valor    = "não aplicável";
      dado->unidade  = "";
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrecho::ValorComprimento(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Comp."));
   //define unidade e valor
   aue.Comprimento_KM(trecho->Comprimento_km, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrecho::ValorIadm(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Iadm."));
   //define unidade e valor
   aue.Corrente_A(trecho->arranjo->Iadm_amp, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrecho::ValorFases(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Fases"));
   //define unidade e valor
   dado->unidade = "";
   dado->valor   = fases->AsStrUpper[trecho->arranjo->Fases];
   }
/*
//---------------------------------------------------------------------------
void __fastcall TMontaTrecho::ValorTipo(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Num.Fases"));
   //define unidade e valor
   dado->unidade = "";
   dado->valor   = TipoMonoBiTrifasico(trecho->arranjo->Fases);
   }
*/
//---------------------------------------------------------------------------
//eof



