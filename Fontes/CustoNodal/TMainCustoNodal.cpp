//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMainCustoNodal.h"
#include "TMainCustoNodal.h"
#include "VTCustos.h"
#include "VTCustoEqpto.h"
#include "VTCustoNodal.h"
#include "..\Apl\VTApl.h"
#include "..\Flow\VTFlow.h"
#include "..\Fluxo\VTFluxo.h"
#include "..\Ordena\VTOrdena.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTRedes.h"
#include "..\Zona\VTZonas.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTMainCustoNodal* __fastcall NewObjMainCustoNodal(VTApl *apl)
   {
   return(new TMainCustoNodal(apl));
   }

//---------------------------------------------------------------------------
__fastcall TMainCustoNodal::TMainCustoNodal(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   try{//cria objetos
      lisEQP = new TList();
      lisLIG = new TList();
      }catch (Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TMainCustoNodal::~TMainCustoNodal(void)
   {
   //destr�i lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   if (lisLIG) {delete lisLIG; lisLIG = NULL;}
   }

//---------------------------------------------------------------------------
bool  __fastcall TMainCustoNodal::CalculaCoeficienteCustoNodal(void)
   {
   //vari�veis locais
   bool       sucesso;
   double     Sbase = 100.;
   AnsiString arqRelFlow, arqRelRede;
   VTNet      *net;
   VTFluxo    *coef_nodal = (VTFluxo*)apl->GetObject(__classid(VTFluxo));
   VTOrdena   *ordena     = (VTOrdena*)apl->GetObject(__classid(VTOrdena));
   VTRedes    *redes      = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTZonas    *zonas      = (VTZonas*)apl->GetObject(__classid(VTZonas));

   //monta Zonas da Rede
   zonas->Executa();
   //cria objeto VTNet p/ as Redes carregadas
   ordena->TramoEnabled = false;
   if ((net = ordena->Executa(redes)) == NULL) return(false);
   //executa c�lculo dos coeficientes do CustoNodal
   sucesso = coef_nodal->Executa(net, Sbase, arqRelFlow, arqRelRede);
   //destr�i objeto VTNet
   delete net;
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool  __fastcall TMainCustoNodal::CalculaCustoNodal(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
bool  __fastcall TMainCustoNodal::CalculaFluxoPotencia(void)
   {
   //vari�veis locais
   VTFlow    *flow  = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTRedes   *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //desabilita mensagens do flow
   flow->AvisoEnabled = false;
   //desabilita medida de tempo do flow
   flow->CronometroEnabled = false;
   //seleciona algoritmo de c�lculo de fluxo
   flow->TipoFluxo = fluxoCC1;
   //executa c�lculo de fluxo �nico das redes
   return(flow->CalculaRedeUnica(redes));
   }

//---------------------------------------------------------------------------
bool  __fastcall TMainCustoNodal::Executa(void)
   {
   //vari�veis locais
   VTCustos *custos = (VTCustos*)apl->GetObject(__classid(VTCustos));

   //reinicia objeto Custos
   custos->Reinicia();
   //inicia lista de CustoEqpto dos Eqptos a partir da da base de dados
   if (! IniciaLisCustoEqpto()) return(false);
   //inicia lista de CustoNodal da rede
   if (! IniciaLisCustoNodal()) return(false);
   //calcula coeficientes dos CustoNodal
   if (! CalculaCoeficienteCustoNodal()) return(false);
   //calcula fluxo de pot�ncia p/ determinar carregamento dos Eqptos
   if (! CalculaFluxoPotencia()) return(false);
   //calcula CustoNodal
   if (! CalculaCustoNodal()) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
bool  __fastcall TMainCustoNodal::IniciaLisCustoEqpto(void)
   {
   //vari�veis locais
   VTEqpto       *eqpto;
   VTCustoEqpto  *custo_eqpto;
   VTCustos      *custos = (VTCustos*)apl->GetObject(__classid(VTCustos));
   VTRedes       *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));

//C�DIGO TEMPOR�RIO
   //obt�m lista de Trechos da Rede
   lisLIG->Clear();
   redes->LisLigacao(lisLIG, eqptoTRECHO);
   //cria objetos CustoEqpto assumindo custo unit�rio igual p/ todos os Trechos
   for (int n = 0; n < lisLIG->Count; n++)
      {
      eqpto = (VTEqpto*)lisLIG->Items[n];
      if ((custo_eqpto = NewObjCustoEqpto()) == NULL) continue;
      custo_eqpto->eqpto = eqpto;
      custo_eqpto->custo = 100.;
      //insere CustoEqpto em Custos
      custos->InsereCustoEqpto(custo_eqpto);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool  __fastcall TMainCustoNodal::IniciaLisCustoNodal(void)
   {
   //vari�veis locais
   double        coef = 0.;
   VTEqpto       *eqpto;
   VTCustoEqpto  *custo_eqpto;
   VTCustoNodal  *custo_nodal;
   VTCustos      *custos = (VTCustos*)apl->GetObject(__classid(VTCustos));
   VTRedes       *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList         *lisCUSTO_EQP = custos->ListaCustoEqpto();

   //obt�m lista de Trechos da Rede
   lisLIG->Clear();
   redes->LisLigacao(lisLIG, eqptoTRECHO);
   //obt�m lista de Geradores e Suprimentos da Rede
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoGERADOR);
   redes->LisEqbar(lisEQP, eqptoSUPRIMENTO);
   //cria objetos CustoNodal para cada Gerador/Suprimento
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      if ((custo_nodal = NewObjCustoNodal()) == NULL) return(false);
      custo_nodal->eqpto = eqpto;
      //insere CustoNodal em Custos
      custos->InsereCustoNodal(custo_nodal);
      //cria Parcelas do CustoNodal
      for (int np = 0; np < lisCUSTO_EQP->Count; np++)
         {
         custo_eqpto = (VTCustoEqpto*)lisCUSTO_EQP->Items[np];
         if (! custo_nodal->InsereParcela(coef, custo_eqpto)) return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof



