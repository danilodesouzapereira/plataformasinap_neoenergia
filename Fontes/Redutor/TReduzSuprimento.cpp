//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.hpp>
#include "TReduzSuprimento.h"
#include "VTRedutor.h"
#include "..\Apl\VTApl.h"
#include "..\Complexo\Complexo.h"
#include "..\Curvas\VTCurva.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTFornecimento.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCorte.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTResflowBar.h"
#include "..\Rede\VTResflowLig.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Zona\VTZona.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTReduz
//---------------------------------------------------------------------------
VTReduz* __fastcall NewObjReduzSuprimento(VTApl *apl)
   {
   return(new TReduzSuprimento(apl));
   }

//---------------------------------------------------------------------------
__fastcall TReduzSuprimento::TReduzSuprimento(VTApl *apl)
   {
   this->apl = apl;
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //cria objetos
   lisBAR   = new TList();
   lisLIG   = new TList();
   lisMRV   = new TList();
   lisTMP   = new TList();
   //inicia dados
   ModeloEqv = eqvDESEQUILIBRADO;
   }

//---------------------------------------------------------------------------
__fastcall TReduzSuprimento::~TReduzSuprimento(void)
   {
   //destrói listas sem destruir seus objetos
   if (lisBAR)   {delete lisBAR;   lisBAR   = NULL;}
   if (lisLIG)   {delete lisLIG;   lisLIG   = NULL;}
   if (lisMRV)   {delete lisMRV;   lisMRV   = NULL;}
   if (lisTMP)   {delete lisTMP;   lisTMP   = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzSuprimento::CorteIsolado(VTCorte *corte)
   {
   //variáveis locais
   int     ind_pat = 0;
   VTBarra *barra;
   TList   *lisBAR_COR = corte->LisBarra();

   //loop p/ todas Barras do Corte
   for (int nb = 0; nb < lisBAR_COR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR_COR->Items[nb];
      if (barra->resflow->Diagnostico[ind_pat] == diagISOLADO) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzSuprimento::CriaCurva(VTSuprimento *psup)
   {
   //variáveis locais
   VTCurva *curva;

   //cria uma nova Curva
   if ((curva = DLL_NewObjCurva()) == NULL) return(false);
   //associa Curva c/ a Carga
   psup->DefineCurva(curva);
   //define dados da Curva
   curva->Codigo        = psup->Codigo;
   curva->Id            = psup->Id;      //MUITO IMPORTANTE
   curva->Tipo          = curvaPQVT;
   curva->Unidade       = unidPU;
   curva->Escala        = patamares->NumPatamar();
   curva->Tipica        = false;
   curva->NumeroValores = nvCURVA_RRSUP;  //{p,q,v,t} por fase
   return(true);
   }

//---------------------------------------------------------------------------
VTReducao* __fastcall TReduzSuprimento::Executa(VTMNet *mnet, VTMRede *mrede,
                                                VTCorte *corte, bool aviso_enabled)
   {
   //variáveis locais
   VTReducao *reducao;

   //proteção
   if (corte->NumeroBarras() != 1) return(NULL);
   //proteção contra Corte entre redes que estão isoladas (sem geração)
   if (CorteIsolado(corte))
      {//MRede não pode ser reduzida p/ o Corte
      reducao = MontaReducaoInvalida();
      return(reducao);
      }
   //cria Reducao de carga p/ a MRede no Corte
   reducao = MontaReducaoTipoSuprimento(mrede, corte);
   return(reducao);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzSuprimento::Executa(VTRede *rede, TList *lisBAR, bool aviso_enabled)
   {
   //método não implementado nesta classe
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduzSuprimento::Executa(VTRedes *redes, TList *lisBAR, bool aviso_enabled)
   {
   //método não implementado nesta classe
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TReduzSuprimento::ExisteBarraInterface(VTMRede *mrede, VTCorte *corte, TList *lisEQP)
   {
   //variáveis locais
   VTBarra *pbarra;
   TList   *lisBC;   //lista de Barras do Corte

   //reinicia lista recebida c/ as Barras da MRede
   lisEQP->Clear();
   //obtém lista de Barras do Corte
   lisBC = corte->LisBarra();
   //obtém lista de Barras da MRede
   lisTMP->Clear();
   mrede->LisBarra(lisTMP);
   //loop p/ todas Barras do Corte
   for (int nb = 0; nb < lisBC->Count; nb++)
      {
      pbarra = (VTBarra*)lisBC->Items[nb];
      //verifica se a Barra do Corte existe na lista de Barras da MRede
      if (lisTMP->IndexOf(pbarra) >= 0)
         {//insere Barra na lista
         if (lisEQP->IndexOf(pbarra) < 0) lisEQP->Add(pbarra);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduzSuprimento::ExisteLigacao(VTMRede *mrede, VTBarra *pbarra, TList *lisEQP)
   {
   //variáveis locais
   VTLigacao *pliga;

   //reinicia lista recebida
   lisEQP->Clear();
   //obtém lista de Ligacoes da MRede
   lisTMP->Clear();
   mrede->LisLigacao(lisTMP);
   //loop p/ todas Ligacoes da Rede
   for (int nl = 0; nl < lisTMP->Count; nl++)
      {
      pliga = (VTLigacao*)lisTMP->Items[nl];
      if (pliga->ConectaBarra(pbarra))
         {//insere Ligacao na lista
         if (lisEQP->IndexOf(pliga) < 0) lisEQP->Add(pliga);
         }
      }
   }

//---------------------------------------------------------------------------
VTReducao* __fastcall TReduzSuprimento::MontaReducaoInvalida(void)
   {
   //variáveis locais
   VTReducao *reducao;

   //cria objeto Reducao
   if ((reducao = DLL_NewObjReducao(patamares->NumPatamar())) == NULL) return(NULL);
   //define Reducao como inválida
   reducao->Valida = false;
   return(reducao);
   }

//---------------------------------------------------------------------------
VTReducao* __fastcall TReduzSuprimento::MontaReducaoTipoSuprimento(VTMRede *mrede, VTCorte *corte)
   {
   //variáveis locais
   VTEqbar   *eqbar;
   VTBarra   *pbarra;
   VTReducao *reducao;

   //define MRede como NÃO redutivel
   mrede->Redutivel = false;
   //cria objeto Reducao
   if ((reducao = DLL_NewObjReducao(patamares->NumPatamar())) == NULL) return(NULL);
   //loop p/ todas Barras da Rede que pertencem ao Corte
   ExisteBarraInterface(mrede, corte, lisBAR);
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      pbarra = (VTBarra*)lisBAR->Items[nb];
      //cria Suprimento equivalente p/ a Rede na Barra
      if ((eqbar = MontaSuprimentoEquivalente(mrede, pbarra)) == NULL)
         {//destrói objeto
         DLL_DeleteEqpto(reducao);
         return(NULL);
         }
      //insere Suprimento na Reducao
      reducao->InsereEqbar(eqbar);
      }
   //define Reducao como válida
   reducao->Valida = true;
   //define MRede como redutivel
   mrede->Redutivel = true;
   return(reducao);
   }

//---------------------------------------------------------------------------
VTEqbar* __fastcall TReduzSuprimento::MontaSuprimentoEquivalente(VTMRede *mrede, VTBarra *pbarra)
   {
   //variáveis locais
   int            max_pat, ind_bar;
   double         p_mw, q_mvar, v_pu, t_rad;
   strSUP         *str_sup, vet_sup[MAX_FASE];
   VTSuprimento   *psup;
   VTLigacao      *pliga;
   VTFornecimento *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));

   //determina lista de Ligacoes da Rede que conectam a Barra
   ExisteLigacao(mrede, pbarra, lisLIG);
   if (lisLIG->Count == 0) return(NULL);
   //cria um novo Suprimento
   if ((psup = DLL_NewObjSuprimento(patamares->NumPatamar())) == NULL) return(NULL);
   //cria Curva PQVT p/ o Suprimento
   if (! CriaCurva(psup)) {delete psup; psup = NULL;}
   //associa Suprimento c/ sua Barra
   psup->DefineObjBarra(pbarra);
   //inicia dados do Suprimento
   psup->Id         = FalsoId();
   psup->Codigo     = "";
   psup->Reducao    = true;
   psup->tiposup    = supVF;
   //pot.curto em MVA ou impedância equivalente em pu
   psup->pcc_3f.p = 0.;
   psup->pcc_3f.q = 0.;
   psup->pcc_ft.p = 0.;
   psup->pcc_ft.q = 0.;
   psup->zeq0.r   = 0.01;
   psup->zeq0.x   = 0.5;
   psup->zeq1.r   = 0.01;
   psup->zeq1.x   = 0.5;
   //inicia dados p/ diag.esquemático
   psup->esq.posbar = 50;
   (pbarra->Vertical()) ? psup->esq.dxbar = -4000 : psup->esq.dybar = 4000 ;
   //loop p/ todos patamares
   max_pat = patamares->NumPatamar();
   for (int np = 0; np < max_pat; np++)
      {//atualiza patamar da curva PQVT por fase do Suprimento
      //loop p/ todas fases
      for (int nf = 0; nf < MAX_FASE; nf++)
         {//inicia tensões do Suprimento c/ as tensões da Barra
         v_pu  = Abs(pbarra->resflow->Vfn_pu[np][nf]);
         t_rad = Arg(pbarra->resflow->Vfn_pu[np][nf]);
         //inicia potência do Suprimento no patamar c/ fluxo nas Ligacoes da Barra
         p_mw = q_mvar = 0.;
         for (int nl = 0; nl < lisLIG->Count; nl++)
            {
            pliga = (VTLigacao*)lisLIG->Items[nl];
            //verifica se a Ligacao conecta a Barra inicial
            if ((ind_bar = pliga->IndiceBarra(pbarra)) < 0) continue;
            //inverte o sinal do fluxo
            p_mw   -= pliga->resflow->Pf_mw[ind_bar][np][nf];
            q_mvar -= pliga->resflow->Qf_mvar[ind_bar][np][nf];
            }
         str_sup    = vet_sup + nf;  // Obtém endereço no vetor
         str_sup->p = p_mw;
         str_sup->q = q_mvar;
         str_sup->v = v_pu;
         str_sup->t = t_rad;
         } // for(nf)
      fornecimento->Set_PQVT_porfase(psup, np, vet_sup);
      }
   return(psup);
   }

//---------------------------------------------------------------------------
//eof

