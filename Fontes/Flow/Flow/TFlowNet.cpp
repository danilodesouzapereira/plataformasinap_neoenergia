//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFlowNet.h"
#include "..\Diag\VTDiag.h"
#include "..\VTFlow.h"
#include "..\FluxoNo\VTFluxoNo.h"
#include "..\FluxoTramo\VTFluxoTramo.h"
#include "..\Redutor\VTReduz.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Diretorio\VTPath.h"
#include "..\..\Fluxo\VTFluxo.h"
#include "..\..\Log\VTLog.h"
#include "..\..\Patamar\VTPatamar.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Potencia\VTFornecimento.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTCarga.h"
#include "..\..\Rede\VTGerador.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTMNet.h"
#include "..\..\Rede\VTNet.h"
#include "..\..\Rede\VTNo.h"
#include "..\..\Rede\VTMRede.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTResFlowBar.h"
#include "..\..\Rede\VTResFlowLig.h"
#include "..\..\Rede\VTResFlowSup.h"
#include "..\..\Rede\VTSuprimento.h"
#include "..\..\Rede\VTTramo.h"
#include "..\..\Zona\VTZona.h"
#include "..\..\Zona\VTZonas.h"
#include "..\..\..\DLL_Inc\FluxoCC.h"
#include "..\..\..\DLL_Inc\FluxoG1.h"
#include "..\..\..\DLL_Inc\FluxoG3.h"
#include "..\..\..\DLL_Inc\FluxoH3.h"
#include "..\..\..\DLL_Inc\FluxoNR1.h"
#include "..\..\..\DLL_Inc\FluxoNR3.h"
#include "..\..\..\DLL_Inc\FluxoRadial1.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTFlowNet* NewObjFlowNet(VTApl* apl)
   {
   return(new TFlowNet(apl));
   }

//---------------------------------------------------------------------------
__fastcall TFlowNet::TFlowNet(VTApl* apl)
   {
   //vari�veis locais
   VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //salva ponteiro p/ objetos externos
   this->apl  = apl;
   this->flow = (VTFlow*)apl->GetObject(__classid(VTFlow));
   this->plog = (VTLog*)apl->GetObject(__classid(VTLog));
   //cria objetos locais
   diag      = NewObjDiag(apl);
   reduz     = NewObjReduz(apl);
   lisEQP    = new TList();
   lisGERSUP = new TList();
   //inicia dados
   aviso_enabled     = false;
   crono_enabled     = false;
   tipo_relatorio    = relNENHUM;
   Sbase             = 100.; //MVA
   //define nome do arquivo de relat�rio
   arqRelFlow    = path->ArqRelFlow();
   arqRelRede    = path->ArqRelFlowDebug();
   //elimina relat�rios gerados anteriormente
   EliminaRelatorios();
   }

//---------------------------------------------------------------------------
__fastcall TFlowNet::~TFlowNet(void)
   {
   //destr�i objetos criados
   if (diag)   {delete diag;   diag   = NULL;}
   if (reduz)  {delete reduz;  reduz  = NULL;}
   //destr�i lista sem destruir seus objetos
   if (lisEQP)    {delete lisEQP;    lisEQP    = NULL;}
   if (lisGERSUP) {delete lisGERSUP; lisGERSUP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFlowNet::EliminaRelatorios(void)
   {
   //elimina arquivos de relat�rios
   if (FileExists(arqRelFlow))   DeleteFile(arqRelFlow);
   if (FileExists(arqRelRede))   DeleteFile(arqRelRede);
   }

//---------------------------------------------------------------------------
bool __fastcall TFlowNet::Executa(VTMNet *mnet)
   {
   //vari�veis locais
   bool       sucesso;
   TList      *lisMR;
   TList      *lisREDE;
   VTMRede    *mrede;
   VTRede     *rede;

   //prote��o
   if (mnet->Net == NULL) return(false);
   //calcula Fluxo de Pot�ncia p/ VTNet
   sucesso = Executa(mnet->Net);
   //atualiza informa��o de fluxo correto ou n�o das Redes
   lisMR = mnet->LisMRede();
   for (int i = 0; i < lisMR->Count; i++)
      {
      mrede = (VTMRede*)lisMR->Items[i];
      lisREDE = mrede->LisRede();
      for (int j = 0; j < lisREDE->Count; j++)
         {
         rede = (VTRede*)lisREDE->Items[j];
         rede->FluxoOK = sucesso;
         }
      }
   //reinicia Suprimentos equivalentes das Reducoes da MNet
   reduz->ReiniciaReducaoSuprimento(mnet);
   reduz->ReiniciaReducaoCarga(mnet);
   //N�O destr�i objeto VTNet
   //delete net;
   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TFlowNet::Executa(VTNet *net)
   {
   //vari�veis locais
   int  tipo_fluxo, tipo_fluxo_alt;

   //determina algoritmo de fluxo a ser usado
   tipo_fluxo = SelecionaAlgoritmoFluxo(net, flow->TipoFluxo);
   if (tipo_fluxo == fluxoDISABLED) return(false);
   //executa c�lculo
   if (ExecutaCalculoNet(net, tipo_fluxo)) return(true);
   //determina algoritmo de fluxo alternativo a ser usado
   tipo_fluxo_alt = SelecionaAlgoritmoFluxo(net, flow->TipoFluxoAlt);
   if ((tipo_fluxo_alt == fluxoDISABLED)||(tipo_fluxo_alt == tipo_fluxo)) return(false);
   //executa c�lculo
   return(ExecutaCalculoNet(net, tipo_fluxo_alt));
   }

//---------------------------------------------------------------------------
bool __fastcall TFlowNet::ExecutaCalculoNet(VTNet *net, int tipo_fluxo)
   {
   //vari�veis locais
   VTFluxo      *fluxo;
   VTFluxoNo    *fluxoNo;
   VTFluxoTramo *fluxoTramo;
   bool         resp = false;

   //reinicia diagn�stico do fluxo dos eqptos da Net
   ReiniciaResFluxoEqptoNet(net);
   //prote��o
   if (tipo_fluxo == fluxoDISABLED) return(false);
   //prote��o: verifica se h� algum Suprimento ou Gerador c/ tens�o v�lida
   if (! ExisteSuprimentoTensaoValida(net)) return(false);
   //cria objeto VTFluxo p/ c�lculo do fluxo de pot�ncia de Net
   switch(tipo_fluxo)
      {
      case fluxoBARRA:   fluxo = NewObjFluxoBarra(apl);         break;
      case fluxoGAUSS1:  fluxo = DLL_NewObjFluxoG1(apl);        break;
      case fluxoGAUSS3:  fluxo = DLL_NewObjFluxoG3(apl);        break;
      case fluxoHARMO3:  fluxo = DLL_NewObjFluxoH3(apl);        break;
      case fluxoNR1:     fluxo = DLL_NewObjFluxoNR1(apl);       break;
      case fluxoNR3:     fluxo = DLL_NewObjFluxoNR3(apl);       break;
      case fluxoRADIAL1: fluxo = DLL_NewObjFluxoRadial1(apl);   break;
      case fluxoCC1:     fluxo = DLL_NewObjFluxoCC(apl);        break;
      default:           fluxo = DLL_NewObjFluxoG1(apl);        break;
      }
   //verifica se a Rede � aderente ao modelo de fluxo selecionado
   if (! fluxo->ValidaAderencia(net, plog))
      {
      delete fluxo;
      return(false);
      }
   //verifica se fluxo equilibrado
   if (fluxo->Equilibrado)
      {//redefine Zona para para Fluxo equilibrado
      ReconfiguraZonaFluxoEquilibrado(net);
      }
   else
      {//prote��o: verifica se h� Zona inv�lida
      if (ExisteZonaInvalida(net)) return(false);
      //prote��o: verifica se h� Suprimento c/ fase inv�lida
      if (ExisteSuprimentoFasesInvalidas(net)) return(false);
      //prote��o: verifica se h� Carga c/ fase inv�lida
      if (ExisteCargaFasesInvalidas(net)) return(false);
      }
   //cria objeto VTFluxoTramo p/ c�lculo do fluxo nos Trechos de um Tramo
   fluxoTramo = NewObjFluxoTramo(apl);
   //cria objeto VTFluxoNo p/ c�lculo do fluxo nas Chaves de um N�
   fluxoNo = NewObjFluxoNo(apl);
   //prote��o
   if ((fluxo != NULL)&&(fluxoTramo != NULL)&&(fluxoNo != NULL))
      {//configura op��es do fluxo
      fluxo->TipoRelatorio     = flow->TipoRelatorio;
      fluxo->RelatorioBarDesc  = flow->RelatorioBarDesc;
      fluxo->ModeloCarga       = flow->ModeloCarga;
      fluxo->RedeComCarga      = flow->RedeComCarga;
      fluxo->PerdaFerroEnabled = flow->PerdaFerroEnabled;
      //calcula fluxo de pot�ncia
      if ((resp = fluxo->Executa(net, Sbase, arqRelFlow, arqRelRede)) == true)
         {//salva n�mero de patamres calculados
         net->StatusFluxoNumPatOK = fluxo->NumPatamaresOK;
         //reinicia resultados de fluxo p/ parte isolada da Net
         diag->ReiniciaResFluxoBarra(net->LisBarraIsolada());
         diag->ReiniciaResFluxoLigacao(net->LisLigacaoIsolada());
         //verifica se fluxo calculado n�o � harm�nico
         if (tipo_fluxo != fluxoHARMO3)
            {//calcula fluxo de pot�ncia nas Chaves dos Nos
            fluxoNo->ModeloCarga = flow->ModeloCarga;
            fluxoNo->Executa(net, Sbase);
            //calcula fluxo de pot�ncia nos Trechos de um Tramo
            //fluxoTramo->ModeloCarga = ModeloCarga;
            fluxoTramo->Executa(net, Sbase);
            }
         //inicia dados de diagn�stico
         diag->Executa(net);
         }
      else
         {//salva n�mero de patamres calculados
         net->StatusFluxoNumPatOK = fluxo->NumPatamaresOK;
         //reinicia diagn�stico do fluxo dos eqptos da Net
         ReiniciaResFluxoEqptoNet(net);
         }
      }
   //destr�i objetos
   if (fluxo)      delete (TObject*)fluxo;
   if (fluxoNo)    delete (TObject*)fluxoNo;
   if (fluxoTramo) delete (TObject*)fluxoTramo;
   return(resp);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TFlowNet::ExisteCargaFasesInvalidas(VTNet *net)
   {
   //vari�veis locais
   int      fases_car, fases_no;
   VTCarga *carga;
   VTEqpto *eqpto;
   VTNo    *no;
   TList   *lisEQP;

   //loop p/ todas Cargas de Net
   lisEQP = net->LisEqbar();
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      if (eqpto->Tipo() == eqptoCARGA)
         {
         carga = (VTCarga*)eqpto;
         no    = carga->pbarra->No[net];
         //verifica se o No n�o possui alguma das fases da Carga (sem considerar faseT)
         //fases_car = carga->Fases & faseABCN;
         //fases_no  = no->Fases    & faseABCN;
         //verifica se o No n�o possui alguma das fases da Carga (sem considerar faseNT)
         fases_car = carga->Fases & faseABC;
         fases_no  = no->Fases    & faseABC;
         //if ((fases_car == faseINV) || ((fases_car & fases_no) != fases_car))
         if (fases_car == faseINV) {continue;}

         //if ((fases_car & fases_no) != fases_car)
         //   {//N�O registra Alerta pois ocorre erro se estiver executando como thread
         //   //Alerta("Fluxo n�o pode ser executado: carga " + carga->Codigo + " com fases inv�lidas");
         //   return(true);
         //   }

         if ((fases_car & fases_no) != fases_car)
            {//verifica se � uma Carga de reducao
            if (carga->Reducao)
               {//redefine suas fases para fasesINV para n�o ser considerada no fluxo
               carga->Fases = faseINV;
               }
            else
               {//retorna indica��o de que h� Carga com fases inv�lidas
               return(true);
               }
            }
         }
      }
   return(false);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TFlowNet::ExisteCargaFasesInvalidas(VTNet *net)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TFlowNet::ExisteSuprimentoFasesInvalidas(VTNet *net)
   {
   //vari�veis locais
   int            fases_sup, fases_no;
   VTEqpto        *eqpto;
   VTNo           *no;
   VTSuprimento   *psup;
   TList          *lisEQP;

   //loop p/ todos os Suprimentos de Net
   lisEQP = net->LisEqbar();
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      if (eqpto->Tipo() == eqptoSUPRIMENTO)
         {
         psup = (VTSuprimento*)eqpto;
         //verifica se o Suprimento possui fases v�lidas
         if ((psup->tiposup == supPV) || (psup->tiposup == supVF))
            {
            no = psup->pbarra->No[net];
            //verifica se o Suprimento n�o possui alguma fase do No (sem considerar faseT)
            //fases_sup = psup->Fases & faseABCN;
            //fases_no  = no->Fases   & faseABCN;
            //verifica se o Suprimento n�o possui alguma fase do No (sem considerar faseNT)
            fases_sup = psup->Fases & faseABC;
            fases_no  = no->Fases   & faseABC;
            if ((fases_sup & fases_no) != fases_no)
               {return(true);}
            }
         }
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TFlowNet::ExisteSuprimentoTensaoValida(VTNet *net)
   {
   //vari�veis locais
   int            max_pat;
   VTEqpto        *eqpto;
   VTNo           *no;
   VTGerador      *pger;
   VTSuprimento   *psup;
   TList          *lisEQP;
   VTPatamar      *patamar;
   VTPatamares    *patamares    = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   VTFornecimento *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));

   //determina n�mero de patamares
   max_pat = patamares->NumPatamar();
   //loop p/ todos os Suprimentos de Net
   lisEQP = net->LisEqbar();
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      if (eqpto->Tipo() == eqptoSUPRIMENTO)
         {
         psup = (VTSuprimento*)eqpto;
         //verifica se o Suprimento possui tens�o v�lidas
         for (int np = 0; np < max_pat; np++)
            {//verifica se o Patamar est� habilitado
            if (! patamares->Patamar[np]->Enabled) continue;
            if (fornecimento->V_pu[psup][np] > 0.) return(true);
            }
         }
      else if (eqpto->Tipo() == eqptoGERADOR)
         {
         pger = (VTGerador*)eqpto;
         //verifica se o Gerador possui tens�es v�lidas
         for (int np = 0; np < max_pat; np++)
            {//verifica se o Patamar est� habilitado
            if (! patamares->Patamar[np]->Enabled) continue;
            if (fornecimento->V_pu[pger][np] > 0.) return(true);
            }
         }
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TFlowNet::ExisteZonaInvalida(VTNet *net)
   {
   //vari�veis locais
   VTNo  *no;
   TList *lisNO = net->LisNo();

   for (int n = 0; n < lisNO->Count; n++)
      {
      no = (VTNo*)lisNO->Items[n];
      if (no->pai->zona->Tipo == zonaINDEFINIDA)
         {return(true);}
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFlowNet::ReconfiguraZonaFluxoEquilibrado(VTNet *net)
   {
   //vari�veis locais
   VTZona *zona;

   //obt�m lista de Zonas de Net
   lisEQP->Clear();
   net->LisZona(lisEQP);
   for (int n = 0; n < lisEQP->Count; n++)
      {
      zona = (VTZona*)lisEQP->Items[n];
      zona->UsadaEmFluxoEquilibrado();
      }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFlowNet::ReiniciaResFluxoEqptoNet(VTNet *net)
   {
   //vari�veis locais
   VTBarra *barra;
   VTNo    *no;

   //reinicia resultados do fluxo das Barras, exceto as que tem gera��o
   lisEQP->Clear();
   CopiaTList(net->LisBarra(), lisEQP);
   for (int n = lisEQP->Count-1; n >= 0; n--)
      {
      barra = (VTBarra*)lisEQP->Items[n];
      if ((no = barra->No[net]) == NULL) continue;
      if (no->ExisteGeracao()) lisEQP->Delete(n);
      }
   diag->ReiniciaResFluxoBarra(lisEQP);
   //reinicia resultados do fluxo das Liga��es
   diag->ReiniciaResFluxoLigacao(net->LisLigacao());
   //reinicia resultados do fluxo das Liga��es internas aos Nos
   diag->ReiniciaResFluxoLigacaoNo(net->LisNo());
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFlowNet::ReiniciaResFluxoEqptoNet(VTNet *net)
   {
   //vari�veis locais
   VTBarra *barra;
   VTEqbar *eqbar;
   VTNo    *no;

   //reinicia resultados do fluxo das Barras, exceto as que tem gera��o
   lisEQP->Clear();
   CopiaTList(net->LisBarra(), lisEQP);
   lisGERSUP->Clear();
   net->LisEqbar(lisGERSUP, eqptoGERADOR);
   net->LisEqbar(lisGERSUP, eqptoSUPRIMENTO);
   for (int n = 0; n <lisGERSUP->Count; n++)
      {
      eqbar = (VTEqbar*)lisGERSUP->Items[n];
      lisEQP->Remove(eqbar->pbarra);
      }
   diag->ReiniciaResFluxoBarra(lisEQP);
   //reinicia resultados do fluxo das Liga��es
   diag->ReiniciaResFluxoLigacao(net->LisLigacao());
   //reinicia resultados do fluxo das Liga��es internas aos Nos
   diag->ReiniciaResFluxoLigacaoNo(net->LisNo());
   }
/*
//---------------------------------------------------------------------------
int __fastcall TFlowNet::SelecionaAlgoritmoFluxo(VTNet *net, int tipo_fluxo)
   {
   //prote��o
   if (tipo_fluxo == fluxoDISABLED) return(fluxoDISABLED);
   //verifica se Net possui um �nico No
   if (net->NumeroNo == 1) return(fluxoBARRA);
   //verifica op��o de fluxo selecionada
   switch(tipo_fluxo)
      {
      case fluxoEQUILIBRADO:
         if (net->Radial)
            {//Radial1
//Bloqueio tempor�rioa at� acertar fluxo Radial
            //tipo_fluxo = fluxoRADIAL1;
            tipo_fluxo = fluxoGAUSS1;
            }
         else if ((net->NumGerSupPV() >= 1)&&(net->NumGerSup() > 1))
            {//NR1
            tipo_fluxo = fluxoNR1;
            }
         else
            {//Gauss1
            tipo_fluxo = fluxoGAUSS1;
            }
         break;
      case fluxoDESEQUILIBRADO:
         if (net->Radial)
            {//Radial3
//Bloqueio tempor�rioa at� acertar fluxo Radial
            //tipo_fluxo = fluxoRADIAL3;
            tipo_fluxo = fluxoGAUSS3;
            }
         else if ((net->NumGerSupPV() >= 1)&&(net->NumGerSup() > 1))
            {//NR3
            tipo_fluxo = fluxoNR3;
            }
         else
            {//Gauss3
            tipo_fluxo = fluxoGAUSS3;
            }
         break;
      case fluxoAUTO:   //determina algoritmo mais adequado
         if (net->Radial)
            {//Radial: verifica se h� trecho/carga desequilibrada
            if (net->ExisteDesequilibrio())
               {//Radial3
//Bloqueio tempor�rioa at� acertar fluxo Radial
               //tipo_fluxo = fluxoRADIAL3;
               tipo_fluxo = fluxoGAUSS3;
               }
            else
               {//Radial1
//Bloqueio tempor�rioa at� acertar fluxo Radial
               //tipo_fluxo = fluxoRADIAL1;
               tipo_fluxo = fluxoGAUSS1;
               }
            }
         else if ((net->NumGerSupPV() >= 1)&&(net->NumGerSup() > 1))
            {//Newton-Raphson: verifica se h� trecho/carga desequilibrada
            if (net->ExisteDesequilibrio())
               {//NR3
               tipo_fluxo = fluxoNR3;
               }
            else
               {//NR1
               tipo_fluxo = fluxoNR1;
               }
            }
         else
            {//Gauss: verifica se h� trecho/carga desequilibrada
            if (net->ExisteDesequilibrio())
               {//Gauss3
               tipo_fluxo = fluxoGAUSS3;
               }
            else
               {//Gauss1
               tipo_fluxo = fluxoGAUSS1;
               }
            }
         break;
      default:   //retorna o pr�prio tipo_fluxo recebido
         break;
      }
   return(tipo_fluxo);
   }
*/
//---------------------------------------------------------------------------
int __fastcall TFlowNet::SelecionaAlgoritmoFluxo(VTNet *net, int tipo_fluxo)
   {
   //prote��o
   if (tipo_fluxo == fluxoDISABLED) return(fluxoDISABLED);
   //verifica se Net possui um �nico No
   if (net->NumeroNo == 1) return(fluxoBARRA);
   //verifica op��o de fluxo selecionada
   switch(tipo_fluxo)
      {
      case fluxoEQUILIBRADO:
         tipo_fluxo = fluxoNR1;
         break;
      case fluxoDESEQUILIBRADO:
         tipo_fluxo = fluxoNR3;
         break;
      case fluxoAUTO:   //determina algoritmo mais adequado
         if (net->ExisteDesequilibrio()) tipo_fluxo = fluxoNR3;
         else                            tipo_fluxo = fluxoNR1;
         break;
      default:   //retorna o pr�prio tipo_fluxo recebido
         break;
      }
   return(tipo_fluxo);
   }

//---------------------------------------------------------------------------
//eof

