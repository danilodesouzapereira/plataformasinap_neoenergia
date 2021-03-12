//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaTrafo.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTVetval.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaTrafo(VTApl *apl)
   {
   try{
      return (new TMontaTrafo(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaTrafo::TMontaTrafo(VTApl *apl)
                       :TMontaLigacao(apl)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TMontaTrafo::~TMontaTrafo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaTrafo::IniciaGrupo(VTEqpto *eqpto)
   {
   return(TMontaLigacao::IniciaGrupo(eqpto));
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaTrafo::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Trafo
   trafo = (VTTrafo*)eqpto;
   //insere atributos do Eqpto
   TMontaEqpto::LisAtributosBasico(eqpto, lisATR);
   LisAtributosEqpto(eqpto, lisATR);
   //insere atributos de fluxo
   LisAtributosFluxo(eqpto, lisATR);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaTrafo::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Trafo
   trafo = (VTTrafo*)eqpto;
   //atributos de Ligacao
   TMontaLigacao::LisAtributosEqpto(eqpto, lisATR);
   //atributos de Trafo
   lisATR->Add(new TAtributo(trafo, trf_SNOM,      true, false, "Pot.nominal", "(MVA)"));
   lisATR->Add(new TAtributo(trafo, trf_VNOM,      true, false, "Tensão nominal", "(kV)"));
   lisATR->Add(new TAtributo(trafo, trf_LIGACAO,   true, false, "Tipo de Ligação"));
   lisATR->Add(new TAtributo(trafo, trf_Z_SEQ,     true, false, "Imp.Seq.0/Seq.1", "(pu)"));
   lisATR->Add(new TAtributo(trafo, trf_ZAT,       true, false, "Imp.Aterramento", "(ohm)"));
   lisATR->Add(new TAtributo(trafo, trf_PERDA_FE , true, false, "Perda ferro", "(%)"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaTrafo::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Trafo
   trafo = (VTTrafo*)eqpto;
   //atributos de fluxo da Ligacao
   TMontaLigacao::LisAtributosFluxo(eqpto, lisATR);
   //atributos de fluxo de Capserie

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaTrafo::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //monta lista de valores do Eqpto
   LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores de Fluxo
   LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaTrafo::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->trafo   = (VTTrafo*)eqpto;
   try{//dados do Eqpto
      TMontaLigacao::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
      //dados do trafo
      if (trafo->Show[trf_SNOM])     ValorPotNominal();
      if (trafo->Show[trf_Z_SEQ])    ValorImpSequencia();
      if (trafo->Show[trf_PERDA_FE]) ValorPerdaFerro();
      //dados de primário e secundário
      for (int pri_sec = PRIMARIO; pri_sec <= SECUNDARIO; pri_sec++)
         {
         if (trafo->Show[trf_VNOM])     ValorTensaoNominal(pri_sec);
         if (trafo->Show[trf_LIGACAO])  ValorTipoLigacao(pri_sec);
         if (trafo->Show[trf_ZAT])      ValorImpAterramento(pri_sec);
         }
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMontaTrafo::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   //variáveis locais
   AnsiString valor;
   VTTrafo   *trafo = (VTTrafo*)eqpto;

   //determina valor de atributo
   switch(ind_atrib)
      {
      case eqp_ID:      valor = IntToStr(eqpto->Id);               break;
      case eqp_CODIGO:  valor = eqpto->Codigo;                     break;
      case lig_REDE:    valor = trafo->rede->Codigo;               break;
      case trf_SNOM:    valor = DoubleToStr("%4.3f", trafo->snom); break;
      }
   return(valor);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrafo::ValorImpAterramento(int pri_sec)
   {
   //variáveis locais
	int             ligacao;
   AnsiString      descricao;
   complex<double> zat;
   VTDado           *dado;

   //define descrição e valor do primário ou do secundário
   ligacao   = (pri_sec == 1) ? trafo->pri.ligacao : trafo->sec.ligacao;
   descricao = (pri_sec == 1) ? "pri:Zat" : "sec:Zat";
   zat       = (pri_sec == 1) ? complex<double>(trafo->pri.zterra.r, trafo->pri.zterra.x)
                              : complex<double>(trafo->sec.zterra.r, trafo->sec.zterra.x);
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado(descricao));
   //verifica se a ligação é Yaterrado
	if (ligacao == lgEST_AT)
		{aue.Impedancia_OHM(zat, dado->valor, dado->unidade);}
	//DVK 2015.09.04 verifica se é monofásico e do secundário
	else if((ligacao == lgMONOFASICO) && (pri_sec != 1))
		{aue.Impedancia_OHM(zat, dado->valor, dado->unidade);}
   else
      {dado->valor = "não aterrado";}
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrafo::ValorImpSequencia(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Zseq.0"));
   aue.Impedancia_PU(trafo->z0, dado->valor, dado->unidade);
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Zseq.1"));
   aue.Impedancia_PU(trafo->z1, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrafo::ValorPerdaFerro(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Perda Ferro"));
   aue.PerdaFerro_PERC(trafo->perda_ferro, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrafo::ValorPotNominal(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Snom"));
   //define unidade e valor
   aue.PotAparente_MVA(trafo->snom, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrafo::ValorTensaoNominal(int pri_sec)
   {
   //variáveis locais
   AnsiString descricao;
   double     vnom;
   VTDado      *dado;

   //define valores do primário ou secundário
   if (pri_sec == PRIMARIO)
      {
      descricao = "pri:Vnom";
      vnom      = trafo->pri.vnom;
      }
   else
      {
      descricao = "sec:Vnom";
      vnom      = trafo->sec.vnom;
      }
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado(descricao));
   //define unidade e valor
   aue.Tensao_KV(vnom, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaTrafo::ValorTipoLigacao(int pri_sec)
   {
   //variáveis locais
   AnsiString descricao;
   int        ligacao, fases_tag;
   VTDado     *dado;
   VTFases    *fases = (VTFases*)apl->GetObject(__classid(VTFases));

   //verifica se Trafo monofásico
   if (trafo->Monofasico)
      {//define fases
      if (pri_sec == PRIMARIO)
         {//define fases do primário
         descricao = "pri:Fases";
         fases_tag = trafo->pri.fase;
         }
      else
         {//define fases do secundário
         descricao = "sec:Fases";
         fases_tag = trafo->sec.fase;
         }
      //cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado(descricao));
      dado->valor    = fases->AsStrUpper[fases_tag];
      dado->unidade  = "";
      }
   else
      {//define tipo de ligação
      if (pri_sec == PRIMARIO)
         {
         descricao = "pri:Ligação";
         ligacao   = trafo->pri.ligacao;
         }
      else
         {
         descricao = "sec:Ligação";
         ligacao   = trafo->sec.ligacao;
         }
      //cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado(descricao));
      //define unidade e valor
      aue.TipoLigacao(ligacao, dado->valor, dado->unidade);
      }
   }

//---------------------------------------------------------------------------
//eof


