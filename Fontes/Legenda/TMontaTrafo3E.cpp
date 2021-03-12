//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaTrafo3E.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTVetval.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaTrafo3E(VTApl *apl)
   {
   try{
      return (new TMontaTrafo3E(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaTrafo3E::TMontaTrafo3E(VTApl *apl)
                       :TMontaLigacao(apl)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TMontaTrafo3E::~TMontaTrafo3E(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaTrafo3E::IniciaGrupo(VTEqpto *eqpto)
   {
   return(TMontaLigacao::IniciaGrupo(eqpto));
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaTrafo3E::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Trafo3E
   trafo3E   = (VTTrafo3E*)eqpto;
   //insere atributos do Eqpto
   TMontaEqpto::LisAtributosBasico(eqpto, lisATR);
   LisAtributosEqpto(eqpto, lisATR);
   //insere atributos de fluxo
   LisAtributosFluxo(eqpto, lisATR);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaTrafo3E::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Trafo3E
   trafo3E   = (VTTrafo3E*)eqpto;
   //atributos de Eqpto
   TMontaLigacao::LisAtributosEqpto(eqpto, lisATR);
   //atributos de Trafo3E
   lisATR->Add(new TAtributo(trafo3E, trf_SNOM,     true,  false, "Pot.nominal", "(MVA)"));
   lisATR->Add(new TAtributo(trafo3E, trf_VNOM,     true,  false, "Tensão nominal", "(kV)"));
   lisATR->Add(new TAtributo(trafo3E, trf_LIGACAO,  true,  false, "Tipo de Ligação"));
   lisATR->Add(new TAtributo(trafo3E, trf_Z_SEQ,    true,  false, "Imp.Seq.0/Seq.1", "(pu)"));
   lisATR->Add(new TAtributo(trafo3E, trf_ZAT,      true,  false, "Imp.Aterramento", "(ohm)"));
   lisATR->Add(new TAtributo(trafo3E, trf_PERDA_FE, true,  false, "Perda ferro", "(%)"));
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaTrafo3E::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Trafo3E
   trafo3E   = (VTTrafo3E*)eqpto;
   //atributos de fluxo da Ligacao
   TMontaLigacao::LisAtributosFluxo(eqpto, lisATR);
   //atributos de fluxo de Trafo3E
   
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaTrafo3E::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //monta lista de valores do Eqpto
   LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores de Fluxo
   LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaTrafo3E::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->trafo3E   = (VTTrafo3E*)eqpto;
   try{//dados do Eqpto
      TMontaLigacao::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
      //dados do trafo3E
      if (trafo3E->Show[trf_Z_SEQ])    ValorImpSequencia();
      if (trafo3E->Show[trf_PERDA_FE]) ValorPerdaFerro();
      //dados de primário, secundário e terciário
      for (int pst = PRIMARIO; pst <= TERCIARIO; pst++)
         {
         if (trafo3E->Show[trf_SNOM])   ValorPotNominal(pst);
         if (trafo3E->Show[trf_VNOM])   ValorTensaoNominal(pst);
         if (trafo3E->Show[trf_LIGACAO])ValorTipoLigacao(pst);
         if (trafo3E->Show[trf_ZAT])    ValorImpAterramento(pst);
         }
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMontaTrafo3E::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   //variáveis locais
   AnsiString valor;
   VTTrafo3E *trafo3e = (VTTrafo3E*)eqpto;

   //determina valor de atributo
   switch(ind_atrib)
      {
      case eqp_ID:      valor = IntToStr(eqpto->Id);                  break;
      case eqp_CODIGO:  valor = eqpto->Codigo;                        break;
      case lig_REDE:    valor = trafo3e->rede->Codigo;                break;
      case trf3_SNOM:   valor = DoubleToStr("%4.3f", trafo3e->Sbase); break;
      }
   return(valor);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrafo3E::ValorImpAterramento(int pst)
   {
   //variáveis locais
   int             ligacao;
   AnsiString      descricao;
   complex<double> zat;
   VTDado          *dado;

   //define descrição e valor do primário ou do secundário
   switch(pst)
      {
      case PRIMARIO:
         descricao = "pri:Zat";
         ligacao   = trafo3E->pri.ligacao;
         zat       = complex<double>(trafo3E->pri.zterra.r, trafo3E->pri.zterra.x);
         break;
      case SECUNDARIO:
         descricao = "sec:Zat";
         ligacao   = trafo3E->sec.ligacao;
         zat       = complex<double>(trafo3E->sec.zterra.r, trafo3E->sec.zterra.x);
         break;
      case TERCIARIO:
         descricao = "ter:Zat";
         ligacao   = trafo3E->ter.ligacao;
         zat       = complex<double>(trafo3E->ter.zterra.r, trafo3E->ter.zterra.x);
         break;
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado(descricao));
   //verifica se a ligação é Yaterrado
   if (ligacao == lgEST_AT)
      {aue.Impedancia_OHM(zat, dado->valor, dado->unidade);}
   else
      {dado->valor = "não aterrado";}
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrafo3E::ValorImpSequencia(void)
   {
   //variáveis locais
   VTDado *dado;
   complex<double> zps0(trafo3E->zps0.r, trafo3E->zps0.x);
   complex<double> zps1(trafo3E->zps1.r, trafo3E->zps1.x);
   complex<double> zpt0(trafo3E->zpt0.r, trafo3E->zpt0.x);
   complex<double> zpt1(trafo3E->zpt1.r, trafo3E->zpt1.x);
   complex<double> zst0(trafo3E->zst0.r, trafo3E->zst0.x);
   complex<double> zst1(trafo3E->zst1.r, trafo3E->zst1.x);

   //cria um novo Dado p/ cada Zseq.0 e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Zps_seq.0"));
   aue.Impedancia_PU(zps0, dado->valor, dado->unidade);
   lisEXT->Add(dado = NewObjDado("Zpt_seq.0"));
   aue.Impedancia_PU(zpt0, dado->valor, dado->unidade);
   lisEXT->Add(dado = NewObjDado("Zst_seq.0"));
   aue.Impedancia_PU(zst0, dado->valor, dado->unidade);
   //cria um novo Dado p/ cada Zseq.1 e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Zps_seq.1"));
   aue.Impedancia_PU(zps1, dado->valor, dado->unidade);
   lisEXT->Add(dado = NewObjDado("Zpt_seq.1"));
   aue.Impedancia_PU(zpt1, dado->valor, dado->unidade);
   lisEXT->Add(dado = NewObjDado("Zst_seq.1"));
   aue.Impedancia_PU(zst1, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrafo3E::ValorPerdaFerro(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Perda Ferro"));
   aue.PerdaFerro_PERC(trafo3E->perda_ferro, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrafo3E::ValorPotNominal(int pst)
   {
   //variáveis locais
   AnsiString descricao;
   double     snom;
   VTDado     *dado;

   //define valores do primário ou secundário
   switch(pst)
      {
      case PRIMARIO:
         descricao = "pri:Snom";
         snom      = trafo3E->pri.snom;
         break;
      case SECUNDARIO:
         descricao = "sec:Snom";
         snom      = trafo3E->sec.snom;
         break;
      case TERCIARIO:
         descricao = "ter:Snom";
         snom      = trafo3E->ter.snom;
         break;
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado(descricao));
   //define unidade e valor
   aue.PotAparente_MVA(snom, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrafo3E::ValorTensaoNominal(int pst)
   {
   //variáveis locais
   AnsiString descricao;
   double     vnom;
   VTDado      *dado;

   //define valores do primário ou secundário
   switch(pst)
      {
      case PRIMARIO:
         descricao = "pri:Vnom";
         vnom      = trafo3E->pri.vnom;
         break;
      case SECUNDARIO:
         descricao = "sec:Vnom";
         vnom      = trafo3E->sec.vnom;
         break;
      case TERCIARIO:
         descricao = "ter:Vnom";
         vnom      = trafo3E->ter.vnom;
         break;
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado(descricao));
   //define unidade e valor
   aue.Tensao_KV(vnom, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrafo3E::ValorTipoLigacao(int pst)
   {
   //variáveis locais
   AnsiString descricao;
   int        ligacao;
   VTDado     *dado;

   //define valores do primário ou secundário
   switch(pst)
      {
      case PRIMARIO:
         descricao = "pri:Ligação";
         ligacao   = trafo3E->pri.ligacao;
         break;
      case SECUNDARIO:
         descricao = "sec:Ligação";
         ligacao   = trafo3E->sec.ligacao;
         break;
      case TERCIARIO:
         descricao = "ter:Ligação";
         ligacao   = trafo3E->ter.ligacao;
         break;
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado(descricao));
   //define unidade e valor
   aue.TipoLigacao(ligacao, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
//eof


