//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TMontaRegulador.h"
#include "TAtributo.h"
#include "VTDado.h"
#include "VTVetval.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMonta* __fastcall NewObjMontaRegulador(VTApl *apl)
   {
   try{
      return (new TMontaRegulador(apl));
      }catch (Exception &e)
         {//nada a fazer
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TMontaRegulador::TMontaRegulador(VTApl *apl)
                           :TMontaLigacao(apl)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
__fastcall TMontaRegulador::~TMontaRegulador(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TMontaRegulador::IniciaGrupo(VTEqpto *eqpto)
   {
   return(TMontaLigacao::IniciaGrupo(eqpto));
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaRegulador::LisAtributos(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Regulador
   regulador = (VTRegulador*)eqpto;
   //insere atributos do Eqpto
   TMontaEqpto::LisAtributosBasico(eqpto, lisATR);
   LisAtributosEqpto(eqpto, lisATR);
   //insere atributos de fluxo
   LisAtributosFluxo(eqpto, lisATR);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaRegulador::LisAtributosEqpto(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Regulador
   regulador = (VTRegulador*)eqpto;
   //atributos de Eqpto
   TMontaLigacao::LisAtributosEqpto(eqpto, lisATR);
   //atributos de Regulador
   lisATR->Add(new TAtributo(regulador, reg_SNOM,     true, false, "Pot.nominal", "(MVA)"));
   lisATR->Add(new TAtributo(regulador, reg_LIGACAO,  true, false, "Tipo de Ligação"));
   lisATR->Add(new TAtributo(regulador, reg_Z_SEQ,    true, false, "Imp.Seq.0/Seq.1", "(pu)"));
   lisATR->Add(new TAtributo(regulador, reg_CFG,      true, false, "Configuração"));

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaRegulador::LisAtributosFluxo(VTEqpto *eqpto, TList *lisATR)
   {
   //salva ponteiro p/ Regulador
   regulador   = (VTRegulador*)eqpto;
   //atributos de fluxo da Ligacao
   TMontaLigacao::LisAtributosFluxo(eqpto, lisATR);
   //atributos de fluxo de Regulador

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaRegulador::LisValores(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //monta lista de valores do Eqpto
   LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
   //monta lista de valores de Fluxo
   LisValoresFluxo(eqpto, lisEXT, ano, num_pat);
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMontaRegulador::LisValoresEqpto(VTEqpto *eqpto, TList *lisEXT, int ano, int num_pat)
   {
   //salva parâmetros
   this->regulador   = (VTRegulador*)eqpto;
   try{//dados do Eqpto
      TMontaLigacao::LisValoresEqpto(eqpto, lisEXT, ano, num_pat);
      //dados do regulador
      if (regulador->Show[reg_SNOM])     ValorPotNominal();
      if (regulador->Show[reg_Z_SEQ])    ValorImpSequencia();
      if (regulador->Show[reg_LIGACAO])  ValorTipoLigacao();
      if (regulador->Show[reg_CFG])      ValorConfiguracao(num_pat);
      }catch(Exception &e)
         {
         }
   return(lisEXT->Count > 0);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMontaRegulador::ValorAtributo(VTEqpto *eqpto, int ind_atrib)
   {
   //variáveis locais
   AnsiString  valor, unidade;
   VTRegulador *regulador = (VTRegulador*)eqpto;

   //determina valor de atributo
   switch(ind_atrib)
      {
      case eqp_ID:     valor = IntToStr(eqpto->Id);                     break;
      case eqp_CODIGO: valor = eqpto->Codigo;                           break;
      case lig_REDE:   valor = regulador->rede->Codigo;                 break;
      case reg_SNOM:   valor = DoubleToStr("%2.1f", regulador->snom);   break;
      }
   return(valor);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TMontaRegulador::ValorConfiguracao(void)
   {
   //variáveis locais
   VTDado *dado;

   //verifica se ajuste automático ou fixo
   if (regulador->ajuste_auto)
	  {//cria um novo Dado e insere em lisEXT
      lisEXT->Add(dado = NewObjDado("Ajuste"));
      dado->valor = "Automático";
      lisEXT->Add(dado = NewObjDado("Vref"));
      aue.Tensao_PU(regulador->cfg_auto.v_pu, dado->valor, dado->unidade);
      }
   else
	  {
      lisEXT->Add(dado = NewObjDado("Ajuste"));
      dado->valor = "Tap fixo";
      lisEXT->Add(dado = NewObjDado("Tap"));
      dado->valor = IntToStr(regulador->cfg_fixo.passo);
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TMontaRegulador::ValorAutCfgTap(TList *lisEXTdados, int num_pat)
{	//variáveis locais
	VTDado *dado;
	VTFases *fases     = (VTFases*)apl->GetObject(__classid(VTFases));
	int tag = 0;
	int fase_id[]  = {faseA, faseB, faseC};
	AnsiString msg = "";

	for (int nf = 0; nf < sizeof(fase_id)/sizeof(int); nf++)
	{
		tag = fases->IndexToTag[nf];
		//verifica se a fase existe no regulador
		if ((regulador->fases & tag) == tag)
		{
        	msg = "Tap Fase " + fases->AsStrABCN[tag];
			lisEXT->Add(dado = NewObjDado(msg));
			dado->valor = IntToStr(regulador->passo_calc[num_pat][nf]);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMontaRegulador::ValorConfiguracao(int num_pat)
{
	//variáveis locais
	VTDado *dado;

	//cria um novo Dado e insere em lisEXT
	lisEXT->Add(dado = NewObjDado("Ajuste"));
   //verifica se ajuste automático ou fixo
	switch(regulador->modelo)
	{
		case reguENERQCT:
			dado->valor = "Regulador BT";
			lisEXT->Add(dado = NewObjDado("Tap"));
			dado->valor = IntToStr(regulador->cfg_fixo.passo);
			break;
		case reguFIXO:
			dado->valor = "Tap Fixo";
			lisEXT->Add(dado = NewObjDado("Tap"));
			dado->valor = IntToStr(regulador->cfg_fixo.passo);
			break;
		case reguAUTO_BI:
			dado->valor = "Automático Bidirecional";
			lisEXT->Add(dado = NewObjDado("Vref"));
			aue.Tensao_PU(regulador->cfg_auto.v_pu, dado->valor, dado->unidade);
			ValorAutCfgTap(lisEXT,num_pat);
			break;
		case reguAUTO_UNI:
			dado->valor = "Automático Unidirecional";
			lisEXT->Add(dado = NewObjDado("Vref"));
			aue.Tensao_PU(regulador->cfg_auto.v_pu, dado->valor, dado->unidade);
			ValorAutCfgTap(lisEXT,num_pat);
			break;
		case reguAUTO_FIXA:
			dado->valor = "Automático Barra Fixa";
			lisEXT->Add(dado = NewObjDado("Vref"));
			aue.Tensao_PU(regulador->cfg_auto.v_pu, dado->valor, dado->unidade);
			ValorAutCfgTap(lisEXT,num_pat);
			break;
		default:
			break;
	}
}

//---------------------------------------------------------------------------
void __fastcall TMontaRegulador::ValorImpSequencia(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Zseq.0"));
   aue.Impedancia_PU(regulador->z0, dado->valor, dado->unidade);
   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Zseq.1"));
   aue.Impedancia_PU(regulador->z1, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaRegulador::ValorPotNominal(void)
   {
   //variáveis locais
   VTDado *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Snom"));
   //define unidade e valor
   aue.PotAparente_MVA(regulador->snom, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
void __fastcall TMontaRegulador::ValorTipoLigacao(void)
   {
   //variáveis locais
   VTDado      *dado;

   //cria um novo Dado e insere em lisEXT
   lisEXT->Add(dado = NewObjDado("Ligação"));
   //define unidade e valor
   aue.TipoLigacao(regulador->ligacao, dado->valor, dado->unidade);
   }

//---------------------------------------------------------------------------
//eof


