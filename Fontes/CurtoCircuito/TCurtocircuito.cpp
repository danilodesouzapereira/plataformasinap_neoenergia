//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.hpp>
#include "TCurtocircuito.h"
#include "TAplCurto.h"
#include "CurtoNo\VTCurtoNo.h"
#include "CurtoReducao\VTCurtoReducao.h"
#include "TDefeitos.h"
#include "..\Calculo\VTData.h"
#include "..\Curto\VTCurto.h"
#include "..\Curto\VTCurtoCS.h"
#include "..\Curto\VTCurtoMA.h"
#include "..\Diretorio\VTPath.h"
#include "..\Flow\VTFlow.h"
#include "..\Log\VTLog.h"
#include "..\Monta\VTMontaCC.h"
#include "..\Ordena\VTOrdena.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTTrecho.h"
#include "..\RedeCC\VTRedeCC.h"
#include "..\Zona\VTZonas.h"
#include "..\..\DLL_Inc\Curto.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Ordena.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCurtocircuito* __fastcall NewObjCurtocircuito(VTApl *apl_owner)
   {
   return(new TCurtocircuito(apl_owner));
   }

//---------------------------------------------------------------------------
__fastcall TCurtocircuito::TCurtocircuito(VTApl *apl_owner)
   {
   //cria objeto AplCurto
   apl = new TAplCurto(apl_owner->Form, apl_owner);

   //elimina relatórios gerados anteriormente
   EliminaRelatorios();
   //inicia dados
   Algoritmo        = curtoCS;
   PreFalta         = false;
   AvisoEnabled     = true;
   TipoRelatorio    = relNENHUM;
   CronoEnabled     = false;
   }

//---------------------------------------------------------------------------
__fastcall TCurtocircuito::~TCurtocircuito(void)
   {
   //destrói objetos
   if (apl) {delete apl; apl = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TCurtocircuito::EliminaRelatorios(void)
   {
   //variáveis locais
   VTPath    *path    = (VTPath*)apl->GetObject(__classid(VTPath));
   AnsiString arq_rel = path->ArqRelCurto();

   if (FileExists(arq_rel)) DeleteFile(arq_rel);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtocircuito::Executa(VTRedes *redes,
                                        STR_ZDef &Zdef,
                                        bool    rede_alterada,
                                        VTBarra *barra,
                                        int     ind_pat,
                                        bool    pre_falta)
   {
   //variáveis locais
   bool      sucesso;
   //bool      tramo_enabled = false;
   VTNet     *net;
   VTMontaCC *montaCC = (VTMontaCC*)apl->GetObject(__classid(VTMontaCC));
   VTOrdena  *ordena  = (VTOrdena*)apl->GetObject(__classid(VTOrdena));
   VTRedeCC  *redeCC  = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
   VTZonas   *zonas   = (VTZonas*)apl->GetObject(__classid(VTZonas));
   barras_isoladas = 0;
   //verifica se houve uma alteração na rede se é uma Redes nova
   if ((rede_alterada)||(this->redes != redes))
      {//salva ponteiro p/ objeto
      this->redes = redes;
      //inicia RedeCC
      redeCC->Inicia(redes);
      }
   else
      {//IMPORTANTE: refaz a associação Barra/BarCC desfeita por cálculo de fluxo (caso de pré-falta)
      redeCC->AssociaBarraBarCC();
      }
   //monta Zonas da Rede  ANTES de montar RedeCC
   if (! zonas->Executa()) return(false);
   //define Barra de defeito na RedeCC
   redeCC->DefineBarraDefeito(barra);
   //atualiza Zdef em RedeCC
   redeCC->SetZdef(Zdef);
   //monta Net da RedeCC
   if ((net = montaCC->Executa(redeCC)) == NULL) return(false);
   //ordena Net
   //ordena->ChaveEnabled    = false;
   //ordena->IsoladoEnabled  = false;
   //ordena->TramoEnabled    = false;
   //ordena->ReguladorBypass = false;
   //JCG 2014.07.15 - exclui Regulador do cálculo de curto
   ordena->ReguladorBypass = true;
   ordena->Executa(net);
   //associa RedeCC com a Net
   redeCC->Net = net;
   //IMPORTANTE: refaz a associação Barra/BarCC desfeita por VTOrdena
   redeCC->AssociaBarraBarCC();
   //atualiza indicação de que o defeito está isolado
   PD.defeito_isolado = (barra->No[net] == NULL);
   //gera aviso de defeito isolado para o usuário
   if (PD.defeito_isolado && AvisoEnabled)
	  {
      barras_isoladas++;
	  //Aviso("A barra de defeito está isolada");
	  return(false);
      }
   //executa cálculo de curto
   switch (Algoritmo)
      {
      case curtoCS:
         sucesso = ExecutaCalculoCS(ind_pat, pre_falta, net, redeCC);
         break;
      case curtoMA:
         sucesso = ExecutaCalculoMA(ind_pat, pre_falta, net, redeCC);
         break;
      }
   //destrói objeto Net
   delete net;
   redeCC->Net = NULL;

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtocircuito::Executa(VTRedes  *redes,
                                        STR_ZDef &Zdef,
                                        bool     rede_alterada,
                                        VTTrecho *trecho,
                                        double   dist_bar1_km,
                                        int      ind_pat,
                                        bool     pre_falta)
   {
   //variáveis locais
   bool      sucesso;
   //bool      tramo_enabled = false;
   VTNet     *net;
   VTMontaCC *montaCC = (VTMontaCC*)apl->GetObject(__classid(VTMontaCC));
   VTOrdena  *ordena  = (VTOrdena*)apl->GetObject(__classid(VTOrdena));
   VTRedeCC  *redeCC  = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
   //VTTrecho  *trecho_fic1, *trecho_fic2;
   VTZonas   *zonas   = (VTZonas*)apl->GetObject(__classid(VTZonas));

   //verifica se houve uma alteração na rede se é uma Redes nova
   if ((rede_alterada)||(this->redes != redes))
      {//salva ponteiro p/ objeto
      this->redes = redes;
      //inicia RedeCC
      redeCC->Inicia(redes);
      }
   //monta Zonas da Rede  ANTES de montar RedeCC
   zonas->Executa();
   //define Trecho de defeito na RedeCC
   redeCC->DefineTrechoDefeito(trecho, dist_bar1_km);
   //atualiza Zdef em RedeCC
   redeCC->SetZdef(Zdef);
   //monta Net da RedeCC
   if ((net = montaCC->Executa(redeCC)) == NULL) return(false);
   //ordena Net
   //ordena->ChaveEnabled   = false;
   //ordena->IsoladoEnabled = false;
   //ordena->TramoEnabled   = false;
   //JCG 2014.07.15 - exclui Regulador do cálculo de curto
   ordena->ReguladorBypass = true;
   ordena->Executa(net);
   //associa RedeCC com a Net
   redeCC->Net = net;
   //IMPORTANTE: refaz a associação Barra/BarCC desfeita por VTOrdena
   redeCC->AssociaBarraBarCC();
   //atualiza indicação de que o defeito está isolado
   PD.defeito_isolado = (trecho->pbarra1->No[net] == NULL);
   //gera aviso de defeito isolado para o usuário
   if (PD.defeito_isolado && AvisoEnabled)
      {
      Aviso("O trecho de defeito está isolado");
      return(false);
      }
   //executa cálculo de curto
   //trecho_fic1 = redeCC->TrechoFicticio(0);
   //trecho_fic2 = redeCC->TrechoFicticio(1);
   switch (Algoritmo)
      {
      case curtoCS:
		 sucesso = ExecutaCalculoCS(ind_pat, pre_falta, net, redeCC);//, trecho,
									//trecho_fic1, trecho_fic2);
         break;
      case curtoMA:
		 sucesso = ExecutaCalculoMA(ind_pat, pre_falta, net, redeCC);//, trecho,
									//trecho_fic1, trecho_fic2);
         break;
      }
   //destrói objeto Net
   delete net;

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtocircuito::ExecutaCalculoCS(int      ind_pat,
                                                 bool     pre_falta,
                                                 VTNet    *net,
												 VTRedeCC *redeCC)//,
												 //VTTrecho *trecho,
												 //VTTrecho *trecho_fic1,
												 //VTTrecho *trecho_fic2)
   {
   // Variáveis locais
   bool           sucesso;
   VTCurtoCS      *curtoCS;
   VTCurtoNo      *curto_no      = (VTCurtoNo*)      apl->GetObject(__classid(VTCurtoNo));
   VTCurtoReducao *curto_reducao = (VTCurtoReducao*) apl->GetObject(__classid(VTCurtoReducao));
   VTData         *data;

   // Cria objeto CurtoCS e configura opções
   if((curtoCS = DLL_NewObjCurtoCS(apl)) == NULL) return(false);
   curtoCS->AvisoEnabled  = AvisoEnabled;
   curtoCS->TipoRelatorio = TipoRelatorio;
   // Cria e inicia objeto Data
   data = DLL_NewObjData(apl);
   data->RedeComCarga = pre_falta;
   data->Inicia(apl, net);

// DEBUG
// int    id_barra_sinap;
// double idef_3F_ka;
// if(! ExecutaCurto3FMax(net, &id_barra_sinap, &idef_3F_ka)) return(false);

   // /*
   // Executa cálculo de curto - ALTERNATIVA 1: Forma original (via tela do Sinap)
   sucesso = curtoCS->Executa1(ind_pat, data, redeCC->BarraDefeito(), net, redeCC);
   if(sucesso)
      {
      curto_no->Executa(net);
      curto_reducao->Executa();
      }
   // */

   /*
   // Executa cálculo de curto - ALTERNATIVA 2: Por partes - Curto em barra
   double  iamp[MAX_FASE], assim_iamp[MAX_FASE], vfn[MAX_FASE];
   VTBarra *barra = NULL, *barra2 = NULL;
   strIMP  zdef_fase_ohm, zdef_terra_ohm;
   zdef_fase_ohm.r  = zdef_fase_ohm.x  = 0.;
   zdef_terra_ohm.r = 40.;
   zdef_terra_ohm.x =  0.;
   sucesso          = curtoCS->Inicia(data, net, ind_pat); // Apenas monta a rede
   if(sucesso)
      {
      // Cálculo de defeito em barra
      // Rede PreFalta_BarraNoMeio:
      // B1: ID=1   B2: ID=2   B3: ID=3   B4:ID=4   B_Aux: ID=5
      for(int n=0; n < net->LisBarra()->Count; n++)
         {
         barra = (VTBarra *) net->LisBarra()->Items[n];
         if(barra->Id == 5) break;
         }
      if(barra != NULL)
         {
         // Tipo de defeito: 0=cuTRI ; 2=cuFT ; 3=cuFTZ
         sucesso = curtoCS->Executa2(0, zdef_fase_ohm, zdef_terra_ohm, barra); // Apenas calcula defeito na barra
         if(sucesso)
            {
            for(int n=0; n < net->LisBarra()->Count; n++)
               {
               barra2 = (VTBarra *) net->LisBarra()->Items[n];
               if(barra2->Id == 2) break;
               }
            if(barra2 != NULL)
               {
               sucesso = curtoCS->IampBarDef(iamp, assim_iamp); // Apenas calcula corrente na barra de defeito
               sucesso = curtoCS->Vfn_kv(barra2, vfn);          // Apenas calcula tensão na barra
               int xxx = 1;  // Só para pegar valores calculados...
               }
            }
         }
      }
   */

   /*
   // Executa cálculo de curto - ALTERNATIVA 3: Por partes - Curto no meio do trecho
   VTLigacao *ligacao = NULL;
   VTTrecho  *trecho2 = NULL;
   double    iamp[MAX_FASE+MAX_FASE];
   strIMP    zdef_fase_ohm, zdef_terra_ohm;
   zdef_fase_ohm.r  = zdef_fase_ohm.x  = 0.;
   zdef_terra_ohm.r = 40.;
   zdef_terra_ohm.x =  0.;
   sucesso          = curtoCS->Inicia(data, net, ind_pat); // Apenas monta a rede
   if(sucesso)
      {
      // Cálculo de defeito no meio de trecho
      // Rede PreFalta_SemBarraNoMeio:
      // B1-B2: ID=1   B2-B3: ID=2   B3-B4: ID=3
      for(int n=0; n < net->LisLigacao()->Count; n++)
         {
         trecho2 = (VTTrecho *) net->LisLigacao()->Items[n];
         if(trecho2->Id == 3) break;
         }
      if(trecho2 != NULL)
         {
         // Apenas calcula defeito no meio do trecho
         // Tipo de defeito: 0 = cuTRI ; 2 = cuFT ; 3 = cuFTZ
         // Trecho de 1904.4 m, ponto de defeito a 1000 m da barra inicial
         sucesso = curtoCS->Executa3(3, zdef_fase_ohm, zdef_terra_ohm, trecho2,
                                     trecho2->pbarra1, 1000.);
         if(sucesso)
            {
            for(int n=0; n < net->LisLigacao()->Count; n++)
               {
               ligacao = (VTLigacao *) net->LisLigacao()->Items[n];
               if(ligacao->Id == 3) break;
               }
            if(ligacao != NULL)
               {
               sucesso = curtoCS->Iamp(ligacao, iamp); // Apenas calcula corrente na ligação
               int xxx = 1;  // Só para pegar 'iamp'...
               }
            }
         }
      }
   */

   /*
   // Executa cálculo de curto - ALTERNATIVA 4: Teste de todos os curtos com Zdef
   strIMP zdef_fase_ohm, zdef_terra_ohm;
   zdef_fase_ohm.r  = 0.;
   zdef_fase_ohm.x  = 0.;
   zdef_terra_ohm.r = 1.9044;
   zdef_terra_ohm.x = 0.;
   sucesso          = curtoCS->Inicia(data, net, ind_pat);  // Apenas monta a rede
   if(sucesso)
      {
      // Localiza barra para defeito
      VTBarra *barra = NULL;
      for(int n=0; n < net->LisBarra()->Count; n++)
         {
         barra = (VTBarra *) net->LisBarra()->Items[n];
         if(barra->Id == 2) break;  // ID=2: B2
         }
      // Localiza ligação para obtenção de correntes
      VTLigacao *ligacao = NULL;
      double    iamp[MAX_FASE+MAX_FASE];
      for(int n=0; n < net->LisLigacao()->Count; n++)
         {
         ligacao = (VTLigacao *) net->LisLigacao()->Items[n];
         if(ligacao->Id == 1) break; // ID=1: B1-B2
         }
      // Calcula todos os defeitos e obtém correntes na ligação
      if((barra != NULL) && (ligacao != NULL))
         {
         sucesso  = curtoCS->Executa2(0, zdef_fase_ohm, zdef_terra_ohm, barra); // 0: cuTRI
         sucesso  = curtoCS->Iamp(ligacao, iamp);
         sucesso  = curtoCS->Executa2(1, zdef_fase_ohm, zdef_terra_ohm, barra); // 1: cu2F
         sucesso  = curtoCS->Iamp(ligacao, iamp);
         sucesso  = curtoCS->Executa2(2, zdef_fase_ohm, zdef_terra_ohm, barra); // 2: cuFT
         sucesso  = curtoCS->Iamp(ligacao, iamp);
         sucesso  = curtoCS->Executa2(3, zdef_fase_ohm, zdef_terra_ohm, barra); // 3: cuFTZ
         sucesso  = curtoCS->Iamp(ligacao, iamp);
         sucesso  = curtoCS->Executa2(4, zdef_fase_ohm, zdef_terra_ohm, barra); // 4: cu2FT
         sucesso  = curtoCS->Iamp(ligacao, iamp);
         sucesso  = true;  // Só para pegar último 'iamp'...
         }
      }
   */

   // Destrói objetos
   delete data;
   delete curtoCS;

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtocircuito::ExecutaCalculoMA(int      ind_pat,
                                                 bool     pre_falta,
                                                 VTNet    *net,
												 VTRedeCC *redeCC)//,
												 //VTTrecho *trecho,
												 //VTTrecho *trecho_fic1,
												 //VTTrecho *trecho_fic2)
   {
   // Variáveis locais
   bool           sucesso;
   VTCurtoMA      *curtoMA;
   VTCurtoNo      *curto_no      = (VTCurtoNo*)      apl->GetObject(__classid(VTCurtoNo));
   VTCurtoReducao *curto_reducao = (VTCurtoReducao*) apl->GetObject(__classid(VTCurtoReducao));
   VTData         *data;

   // Cria objeto CurtoMA e configura opções
   if((curtoMA = DLL_NewObjCurtoMA(apl)) == NULL) return(false);
   curtoMA->AvisoEnabled  = AvisoEnabled;
   curtoMA->TipoRelatorio = TipoRelatorio;

   // Cria e inicia objeto Data
   data               = DLL_NewObjData(apl);
   data->RedeComCarga = pre_falta;
   data->Inicia(apl, net);

   // Executa cálculo de curto - ALTERNATIVA 1: Forma original (via tela do Sinap)
   sucesso = curtoMA->Executa(ind_pat, data, redeCC->BarraDefeito(), net, redeCC);//,
							  //trecho, trecho_fic1, trecho_fic2);
   if(sucesso)
      {
      curto_no->Executa(net);
      curto_reducao->Executa();
      }

   // Destrói objetos
   delete data;
   delete curtoMA;

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtocircuito::ExecutaCalculoSimultaneo(int      ind_pat,
                                                         bool     pre_falta,
                                                         VTNet    *net,
                                                         VTRedeCC *redeCC,
                                                         TList    *lisDEF)
   {
   // Variáveis locais
   bool           sucesso = false;
   VTCurtoMA      *curtoMA;
   VTCurtoNo      *curto_no      = (VTCurtoNo*)      apl->GetObject(__classid(VTCurtoNo));
   VTCurtoReducao *curto_reducao = (VTCurtoReducao*) apl->GetObject(__classid(VTCurtoReducao));
   VTData         *data;

   // Cria objeto CurtoMA e configura opções
   if((curtoMA = DLL_NewObjCurtoMA(apl)) == NULL) return(false);
   curtoMA->AvisoEnabled  = AvisoEnabled;
   curtoMA->TipoRelatorio = TipoRelatorio;

   // Cria e inicia objeto Data
   data               = DLL_NewObjData(apl);
   data->RedeComCarga = pre_falta;
   data->Inicia(apl, net);

   // Executa cálculo de curto
   sucesso = curtoMA->Executa(ind_pat, data, lisDEF, net, redeCC);
   if(sucesso)
      {
      curto_no->Executa(net);
      curto_reducao->Executa();
      }

   // Destrói objetos
   delete data;
   delete curtoMA;

   return(sucesso);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtocircuito::ExecutaCurto3FMax(VTNet *net,
                                                  int    *id_barra_sinap,
                                                  double *idef_3F_ka)
   {
   // Variáveis locais
   VTCurtoCS *curtoCS;
   VTData    *data;

   // Cria objeto CurtoCS e configura opções
   if((curtoCS = DLL_NewObjCurtoCS(apl)) == NULL) return(false);
   curtoCS->AvisoEnabled  = AvisoEnabled;
   curtoCS->TipoRelatorio = relNENHUM;

   // Cria e inicia objeto Data
   data               = DLL_NewObjData(apl);
   data->RedeComCarga = false;
   data->Inicia(apl, net);

   // Determina barra de curto-circuito 3F máximo, e a correspondente corrente
   if(! curtoCS->Curto_3F_Max(data, id_barra_sinap, idef_3F_ka)) return(false);

   // Destrói objetos
   delete data;
   delete curtoCS;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCurtocircuito::ExecutaCurtoSimultaneo(VTRedes  *redes,
                                                       STR_ZDef &Zdef,
                                                       bool     rede_alterada,
                                                       TList    *lisDEF,
                                                       int      ind_pat,
                                                       bool     pre_falta)
   {
   //variáveis locais
   bool      sucesso;
   VTNet     *net;
   TDefeito  *defeito = (TDefeito*)lisDEF->First();
   VTMontaCC *montaCC = (VTMontaCC*)apl->GetObject(__classid(VTMontaCC));
   VTOrdena  *ordena  = (VTOrdena*)apl->GetObject(__classid(VTOrdena));
   VTRedeCC  *redeCC  = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
   VTZonas   *zonas   = (VTZonas*)apl->GetObject(__classid(VTZonas));

   //verifica se houve uma alteração na rede se é uma Redes nova
   if ((rede_alterada)||(this->redes != redes))
      {//salva ponteiro p/ objeto
      this->redes = redes;
      //inicia RedeCC
      redeCC->Inicia(redes);
      }
   else
      {//IMPORTANTE: refaz a associação Barra/BarCC desfeita por cálculo de fluxo (caso de pré-falta)
      redeCC->AssociaBarraBarCC();
      }
   //monta Zonas da Rede  ANTES de montar RedeCC
   if (! zonas->Executa()) return(false);
   //define Barra de defeito na RedeCC
   redeCC->DefineBarraDefeito(defeito->barra);
   //atualiza Zdef em RedeCC
   redeCC->SetZdef(Zdef);
   //monta Net da RedeCC
   if ((net = montaCC->Executa(redeCC)) == NULL) return(false);
   //ordena Net
   //ordena->ChaveEnabled    = false;
   //ordena->IsoladoEnabled  = false;
   //ordena->TramoEnabled    = false;
   //ordena->ReguladorBypass = false;
   //JCG 2014.07.15 - exclui Regulador do cálculo de curto
   ordena->ReguladorBypass = true;
   ordena->Executa(net);
   //associa RedeCC com a Net
   redeCC->Net = net;
   //IMPORTANTE: refaz a associação Barra/BarCC desfeita por VTOrdena
   redeCC->AssociaBarraBarCC();
   //executa cálculo de curto simultaneo
   sucesso = ExecutaCalculoSimultaneo(ind_pat, pre_falta, net, redeCC, lisDEF);
   //destrói objeto Net
   delete net;
   redeCC->Net = NULL;

   return(sucesso);
   }

//---------------------------------------------------------------------------
VTApl* __fastcall TCurtocircuito::PM_GetApl(void)
   {
   return(apl);
   }

//---------------------------------------------------------------------------
VTRedeCC* __fastcall TCurtocircuito::PM_GetRedeCC(void)
   {
   return((VTRedeCC*)apl->GetObject(__classid(VTRedeCC)));
   }

//---------------------------------------------------------------------------
//eof

