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
   //define opções default
   Patamar           = 1;  // Primeiro patamar = 1
   MetodoEE          = meeQR_GIVENS;
   Tipo              = estimaDESEQUILIBRADO;
   TipoGrupo         = grupoUNICO;
   PerdaFerroEnabled = true;
   TipoBarraRef      = brFIXOS_1THETA_0V;  // Estimação de estado convencional
   RelFluxoPotencia  = true;
   OrdenacaoLinhasQR = qrlMENOR_NUM_ELEM_FD;
   UtilizaVDNM       = false;
   NumMaxMontH       = 2;
   // Proteção para o caso de o método de estimação ser alterado logo acima:
   // a ordenação de linhas foi implementada somente no método QR_GIVENS
   if(MetodoEE != meeQR_GIVENS) OrdenacaoLinhasQR = qrlSEM_ORDENACAO;
   }

//---------------------------------------------------------------------------
__fastcall TEstimaEstado::~TEstimaEstado(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TEstimaEstado::Executa(void)
   {
   //verifica se foram definidos Medidores válidos
   if (! ValidaMedidores()) return(false);
   return(ExecutaEstimaEstadoRedeUnica());
   }

//---------------------------------------------------------------------------
bool __fastcall TEstimaEstado::ExecutaEstimaEstadoRedeUnica(void)
   {
   //variáveis locais
   int         pat;
   VTEstimador *estimador;
   VTData      *data;
   VTNet       *net;
   VTOrdena    *ordena;
   VTRedes     *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes)) ;
   bool        sucesso = false;

   try{//cria objeto Ordena
      if ((ordena = DLL_NewObjOrdena(apl)) != NULL)
         {//configura opções de montagem de VTNet
         ordena->ChaveNA_Enabled = false;
         ordena->ChaveNF_Enabled = true;
         ordena->TramoEnabled    = false;
         //cria objeto Net
         if ((net = ordena->Executa(redes)) != NULL)
            {//cria objeto VTData
            if ((data = DLL_NewObjData(apl)) != NULL)
               {//configura opções do objeto VTData
               data->RedeComCarga      = true;
               data->IncluirChave      = true;
               data->ModeloCarga       = cargaAUTO;
               data->PerdaFerroEnabled = PerdaFerroEnabled;
               data->Inicia(apl, net);
               // Habilita patamar único
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
      //destrói objetos
      if (estimador) {delete estimador;}
      if (data)      {delete data;}
      if (net)       {DLL_DeleteEqpto(net);}
      if (ordena)    {delete ordena;}
      }catch(Exception &e)
         {
         Alerta("Estimação de Estado: erro na execução");
         sucesso = false;
         }
   return(sucesso);
   }

//---------------------------------------------------------------------------
void __fastcall TEstimaEstado::HabilitaPatamarUnico(int ind_pat)
   {
   // Variáveis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares *) apl->GetObject(__classid(VTPatamares));
   TList       *lisPAT    = patamares->LisPatamar();

   // Desabilita todos os patamares
   for(int np=0; np < lisPAT->Count; np++)
      {
      patamar = (VTPatamar *) lisPAT->Items[np];
      patamar->Enabled = false;
      }

   // Habilita o patamar fornecido. Se o índice fornecido for inválido,
   // habilita o patamar 0.
   if((ind_pat < 0) || (ind_pat >= lisPAT->Count)) ind_pat = 0;
   patamar = (VTPatamar *) lisPAT->Items[ind_pat];
   patamar->Enabled = true;
   }

//---------------------------------------------------------------------------
bool __fastcall TEstimaEstado::ValidaMedidores(void)
   {
   //variáveis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //obtém lista de Medidores da Rede
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoMEDIDOR);
   if (lisEQP->Count == 0)
      {
      Alerta("Estimação de Estado: não há medidores na rede");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

