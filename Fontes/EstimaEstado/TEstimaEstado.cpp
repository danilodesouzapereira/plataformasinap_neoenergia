//---------------------------------------------------------------------------
#pragma hdrstop
#include "TEstimaEstado.h"
#include "..\Apl\VTApl.h"
#include "..\Calculo\VTData.h"
#include "..\Estimador\VTEstimador.h"
#include "..\Ordena\VTOrdena.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTRedes.h"
#include "..\SisLin\VTSisLin.h"
#include "..\..\DLL_Inc\Estimador1.h"
#include "..\..\DLL_Inc\Estimador3.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Ordena.h"
#include "..\..\DLL_Inc\Patamar.h"
#include "..\..\DLL_Inc\Rede.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTEstimaEstado * __fastcall NewObjEstimaEstado(VTApl *apl)
   {
   return(new TEstimaEstado(apl));
   }

//---------------------------------------------------------------------------
__fastcall TEstimaEstado::TEstimaEstado(VTApl *apl)
   {
   //salva ponteiro para objeto
   this->apl = apl;
   //cria objetos locais
   lisEQP = new TList();
   //define op��es default
   Patamar           = 1;  // Primeiro patamar = 1
   MetodoEE          = meeQR_GIVENS;
   Tipo              = estimaDESEQUILIBRADO;
   TipoGrupo         = grupoUNICO;
   PerdaFerroEnabled = true;
   TipoBarraRef      = brFIXOS_1THETA_0V;  // Estima��o de estado convencional
   RelFluxoPotencia  = true;
   OrdenacaoLinhasQR = qrlMENOR_NUM_ELEM_FD;
   UtilizaVDNM       = false;
   NumMaxMontH       = 2;
   // Prote��o para o caso de o m�todo de estima��o ser alterado logo acima:
   // a ordena��o de linhas foi implementada somente no m�todo QR_GIVENS
   if(MetodoEE != meeQR_GIVENS) OrdenacaoLinhasQR = qrlSEM_ORDENACAO;
   }

//---------------------------------------------------------------------------
__fastcall TEstimaEstado::~TEstimaEstado(void)
   {
   //destr�i lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TEstimaEstado::Executa(void)
   {
   //verifica se foram definidos Medidores v�lidos
   if (! ValidaMedidores()) return(false);
   return(ExecutaEstimaEstadoRedeUnica());
   }

//---------------------------------------------------------------------------
bool __fastcall TEstimaEstado::ExecutaEstimaEstadoRedeUnica(void)
   {
   //vari�veis locais
   int         pat;
   VTEstimador *estimador;
   VTData      *data;
   VTNet       *net;
   VTOrdena    *ordena;
   VTRedes     *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes)) ;
   bool        sucesso = false;

   try{//cria objeto Ordena
      if ((ordena = DLL_NewObjOrdena(apl)) != NULL)
         {//configura op��es de montagem de VTNet
         ordena->ChaveNA_Enabled = false;
         ordena->ChaveNF_Enabled = true;
         ordena->TramoEnabled    = false;
         //cria objeto Net
         if ((net = ordena->Executa(redes)) != NULL)
            {//cria objeto VTData
            if ((data = DLL_NewObjData(apl)) != NULL)
               {//configura op��es do objeto VTData
               data->RedeComCarga      = true;
               data->IncluirChave      = true;
               data->ModeloCarga       = cargaAUTO;
               data->PerdaFerroEnabled = PerdaFerroEnabled;
               data->Inicia(apl, net);
               // Habilita patamar �nico
               pat = Patamar - 1;
               HabilitaPatamarUnico(pat);
               //cria Estimador equilibrado/desequilibrado
               if (Tipo == estimaEQUILIBRADO) {estimador = DLL_NewObjEstimador1(apl);}
               else                           {estimador = DLL_NewObjEstimador3(apl);}
               if (estimador != NULL)
                  {//executa Estimador
                  estimador->TipoBarraRef = TipoBarraRef;
                  estimador->MetodoEE     = MetodoEE;
                  // TipoRelatorio: 0=nenhum  1=resumido  2=completo sem fluxo  3=completo com fluxo
                  estimador->TipoRelatorio = 2;
                  if(RelFluxoPotencia) estimador->TipoRelatorio = 3;
                  estimador->OrdenacaoLinhasQR = OrdenacaoLinhasQR;
                  estimador->UtilizaVDNM       = UtilizaVDNM;
                  estimador->NumMaxMontH       = NumMaxMontH;
                  sucesso                      = estimador->Executa(data);
                  }
               }
            }
         }
      //destr�i objetos
      if (estimador) {delete estimador;}
      if (data)      {delete data;}
      if (net)       {DLL_DeleteEqpto(net);}
      if (ordena)    {delete ordena;}
      }catch(Exception &e)
         {
         Alerta("Estima��o de Estado: erro na execu��o");
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
void __fastcall TEstimaEstado::HabilitaPatamarUnico(int ind_pat)
   {
   // Vari�veis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares *) apl->GetObject(__classid(VTPatamares));
   TList       *lisPAT    = patamares->LisPatamar();

   // Desabilita todos os patamares
   for(int np=0; np < lisPAT->Count; np++)
      {
      patamar = (VTPatamar *) lisPAT->Items[np];
      patamar->Enabled = false;
      }

   // Habilita o patamar fornecido. Se o �ndice fornecido for inv�lido,
   // habilita o patamar 0.
   if((ind_pat < 0) || (ind_pat >= lisPAT->Count)) ind_pat = 0;
   patamar = (VTPatamar *) lisPAT->Items[ind_pat];
   patamar->Enabled = true;
   }

//---------------------------------------------------------------------------
bool __fastcall TEstimaEstado::ValidaMedidores(void)
   {
   //vari�veis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obt�m lista de Medidores da Rede
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoMEDIDOR);
   if (lisEQP->Count == 0)
      {
      Alerta("Estima��o de Estado: n�o h� medidores na rede");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

