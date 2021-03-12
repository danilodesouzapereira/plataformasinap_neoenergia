//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.hpp>
#include "TReduz.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Complexo\Complexo.h"
#include "..\..\Patamar\VTPatamar.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Potencia\VTDemanda.h"
#include "..\..\Potencia\VTFornecimento.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTCarga.h"
#include "..\..\Rede\VTChave.h"
#include "..\..\Rede\VTFases.h"
#include "..\..\Rede\VTLigacao.h"
#include "..\..\Rede\VTNet.h"
#include "..\..\Rede\VTNo.h"
#include "..\..\Rede\VTResFlowBar.h"
#include "..\..\Rede\VTResFlowLig.h"
#include "..\..\Rede\VTSuprimento.h"
#include "..\..\Rede\VTMNet.h"
#include "..\..\Rede\VTReducao.h"
#include "..\..\Zona\VTZona.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTReduz* __fastcall NewObjReduz(VTApl *apl)
   {
   return(new TReduz(apl));
   }

//---------------------------------------------------------------------------
__fastcall TReduz::TReduz(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //inicia dados
   ModeloCarga = cargaAUTO;
   //cria objetos
   lisCAR   = new TList();
   lisEQP   = new TList();
   lisLIG   = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TReduz::~TReduz(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisCAR)   {delete lisCAR;   lisCAR   = NULL;}
   if (lisEQP)   {delete lisEQP;   lisEQP   = NULL;}
   if (lisLIG)   {delete lisLIG;   lisLIG   = NULL;}
   }

//---------------------------------------------------------------------------
int __fastcall TReduz::LigacaoConectaNo(VTLigacao* ligacao, VTNo *no)
   {
   //variáveis locais
   VTBarra *barra;

   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {
      if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
      if (barra->No[net] == no) return(ind_bar);
      }
   return(-1);
   }

//---------------------------------------------------------------------------
void __fastcall TReduz::LisCargaDeReducaoMNet(VTMNet *mnet, VTBarra *barra, TList *lisCAR)
   {
   //variáveis locais
   VTCarga *carga;
   VTNet   *net = mnet->Net;
   VTNo    *no  = barra->No[net];

   //limpa lisCAR
   lisCAR->Clear();
   //obtém lista de todas Cargas da Net
   //JCG 2016.08.31 - mnet->LisEqbar() obtém somente as Cargas próprias das MRedes
   //mnet->LisEqbar(lisCAR, eqptoCARGA);
   mnet->Net->LisEqbar(lisCAR, eqptoCARGA);
   //mantém em lisCAR somente as Cargas de Reducao que conectam o No da Barra indicada
   for (int nc = lisCAR->Count-1; nc >= 0; nc--)
      {
      carga = (VTCarga*)lisCAR->Items[nc];
      if ((! carga->Reducao) || (carga->pbarra->No[net] != no))
         {//retira Carga de lisCAR
         lisCAR->Delete(nc);
         }
      }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TReduz::LisLigacaoMNet(VTMNet *mnet, VTBarra *barra, TList *lisLIG)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //limpa lisLIG
   lisLIG->Clear();
   //obtém lista de todas Ligacoes da MNet
   mnet->LisLigacao(lisLIG);
   //mantém em lisLIG somente a Ligacoes que não são Chave e que conectam o No da Barra indicada
   for (int nl = lisLIG->Count-1; nl >= 0; nl--)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      //descarta Ligacao tipo Chave
      if (ligacao->Tipo() == eqptoCHAVE)
         {//retira Ligacao de lisLIG
         lisLIG->Delete(nl);
         }
      //verifica se a Ligacao conecta o No da Barra indicada
      else if (LigacaoConectaNo(ligacao, barra->No[net]) < 0)
         {//retira Ligacao de lisLIG
         lisLIG->Delete(nl);
         }
      }
   }
*/
/*
//---------------------------------------------------------------------------
void __fastcall TReduz::LisLigacaoMNet(VTMNet *mnet, VTBarra *barra, TList *lisLIG)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //limpa lisLIG
   lisLIG->Clear();
   //obtém lista de todas Ligacoes da MNet
   mnet->LisLigacao(lisLIG);
   //mantém em lisLIG somente as Ligacoes que conectam o No da Barra indicada
   for (int nl = lisLIG->Count-1; nl >= 0; nl--)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      //verifica se a Ligacao conecta o No da Barra indicada
      if (LigacaoConectaNo(ligacao, barra->No[net]) < 0)
         {//retira Ligacao de lisLIG
         lisLIG->Delete(nl);
         }
      }
   }
*/

//---------------------------------------------------------------------------
void __fastcall TReduz::LisLigacaoMNet(VTMNet *mnet, VTBarra *barra, TList *lisLIG)
   {
   //variáveis locais
   VTLigacao *ligacao;

   //limpa lisLIG
   lisLIG->Clear();
   //obtém lista de todas Ligacoes da MNet
   mnet->LisLigacao(lisLIG);
   //proteção
   if (lisLIG->Count == 1)
      {
      ligacao = (VTLigacao*)lisLIG->First();
      //NÃO descarta Ligacao tipo Chave
      //if (ligacao->Tipo() == eqptoCHAVE)
      //   {//retira Ligacao de lisLIG
      //   lisLIG->Delete(nl);
      //   }
      //verifica se a Ligacao conecta o No da Barra indicada
      if (LigacaoConectaNo(ligacao, barra->No[net]) < 0)
         {//retira Ligacao de lisLIG
         lisLIG->Clear();
         }
      return;
      }
   //mantém em lisLIG somente a Ligacoes que não são Chave e que conectam o No da Barra indicada
   for (int nl = lisLIG->Count-1; nl >= 0; nl--)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      //descarta Ligacao tipo Chave
      if (ligacao->Tipo() == eqptoCHAVE)
         {//retira Ligacao de lisLIG
         lisLIG->Delete(nl);
         }
      //verifica se a Ligacao conecta o No da Barra indicada
      else if (LigacaoConectaNo(ligacao, barra->No[net]) < 0)
         {//retira Ligacao de lisLIG
         lisLIG->Delete(nl);
         }
      }
   }


//---------------------------------------------------------------------------
bool __fastcall TReduz::ReiniciaReducaoCarga(VTMNet *mnet)
   {
	//variáveis locais
	int             fases_car;
   double          p[MAX_PAT], q[MAX_PAT];
   complex<double> s_mva[MAX_FASE];
	VTCarga        *carga;
	VTNo           *no;
   VTReducao      *reducao;
   TList          *lisREDU;
   VTDemanda      *demanda   = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   VTPatamares    *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int             max_pat   = patamares->NumPatamar();

   //salva ponteiro para objeto Net
   net = mnet->Net;
   //obtém Reducoes da MNet
   lisREDU = mnet->LisReducao();
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      reducao = (VTReducao*)lisREDU->Items[nr];
      //obtém lista de Cargas da Reducao
      lisEQP->Clear();
      reducao->LisEqbar(lisEQP, eqptoCARGA);
      for (int nc = 0; nc < lisEQP->Count; nc++)
         {
			carga     = (VTCarga*)lisEQP->Items[nc];
			no        = carga->pbarra->No[net];
			//em Redes do tipo Fixo, pode haver uma parte isolada da rede que faz fronteira com outra,
			//isto é, a Barra da Carga não estará associada a um No
			if (no != NULL) carga->Fases = no->Fases;
			//reinicia demanda da Carga
         demanda->ReiniciaCurva(carga);
         //monta lisLIG c/ Ligacoes da MNet que conectam a Barra
         LisLigacaoMNet(mnet, carga->pbarra, lisLIG);
         //monta lisCAR c/ Cargas de reducao da MNet que conectam o No da Barra
         LisCargaDeReducaoMNet(mnet, carga->pbarra, lisCAR);
         //atualiza Curva da Carga da Reducao p/ todos os patamares
         for (int np = 0; np < max_pat; np++)
            {//verifica se patamar habilitado
            if (! patamares->Patamar[np]->Enabled) continue;
            //reinicia p,q em p/ todas fases
            for (int nf = 0; nf < MAX_FASE; nf++) {p[nf] = q[nf] = 0.;}
            //soma fluxo por fase nas Ligacoes
            SomaFluxoLigacao(np, lisLIG, p, q, carga->pbarra->No[net]);
            //soma demanda das Cargas das Reducoes
            SomaDemandaCarga(np, lisCAR, p, q);
            //monta vetor de demanda por fase
            for (int ind_fase = 0; ind_fase < MAX_FASE; ind_fase++)
               {
               s_mva[ind_fase] = complex<double>(p[ind_fase], q[ind_fase]);
               }
            //atualiza demanda da Carga no patamar
            demanda->Set_S3f_mva(carga, np, s_mva);
            }
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduz::ReiniciaReducaoSuprimento(VTMNet *mnet)
   {
   //variáveis locais
   int            fases_sup, fase_tag;
   double         v, t;
	strSUP         vet_sup[MAX_FASE];
	VTNo           *no;
   VTSuprimento   *suprimento;
   VTReducao      *reducao;
   TList          *lisREDU;
   VTFases        *fases        = (VTFases*)apl->GetObject(__classid(VTFases));
   VTFornecimento *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));
	VTPatamares    *patamares    = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
	int             max_pat      = patamares->NumPatamar();

   //salva ponteiro para objeto Net
   net = mnet->Net;
   //loop p/ todas Reducoes
   lisREDU = mnet->LisReducao();
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      reducao = (VTReducao*)lisREDU->Items[nr];
      //inicia lisEQP c/ os Suprimentos da Reducao
      lisEQP->Clear();
      reducao->LisEqbar(lisEQP, eqptoSUPRIMENTO);
      for (int n = 0; n < lisEQP->Count; n++)
         {
			suprimento = (VTSuprimento*)lisEQP->Items[n];
			no         = suprimento->pbarra->No[net];
			//em Redes do tipo Fixo, pode haver uma parte isolada da rede que faz fronteira com outra,
			//isto é, a Barra do Suprimento não estará associada a um No
			if (no != NULL) fases_sup  = no->Fases;
			else            fases_sup  = suprimento->Fases;
			suprimento->Fases = fases_sup;
			//loop p/ todos os patamares
         for (int np = 0; np < max_pat; np++)
            {//atualiza patamar da curva PQVT por fase do Suprimento
            for (int nf = 0; nf < MAX_FASE; nf++)
               {//zera valores
               vet_sup[nf].p = vet_sup[nf].q = vet_sup[nf].v = vet_sup[nf].t = 0.;
               //verifica se existe a fase
               fase_tag = fases->IndexToTag[nf];
               if ((fases_sup & fase_tag) == fase_tag)
                  {//inicia valores de v e t
                  v = Abs(suprimento->pbarra->resflow->Vfn_pu[np][nf]);
                  //JCG 2012.10.26 - início
                  /*
                  //t = Arg(suprimento->pbarra->resflow->Vfn_pu[np][nf]);
                  t = Arg(suprimento->pbarra->resflow->Vfn_pu[np][nf]) - suprimento->pbarra->zona->T_rad[nf];
                  */
                  t = Arg(suprimento->pbarra->resflow->Vfn_pu[np][nf]) - suprimento->pbarra->zona->Defasagem_rad;
                  //JCG 2012.10.26 - fim
                  vet_sup[nf].v = v;
                  vet_sup[nf].t = t;
                  }
               }
            fornecimento->Set_PQVT_porfase(suprimento, np, vet_sup);
            }
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TReduz::ReiniciaReducaoSuprimento1pu(VTMNet *mnet)
   {
	//variáveis locais
	double         defasagem = 0.;
	VTNo           *no;
   VTSuprimento   *suprimento;
   VTReducao      *reducao;
   TList          *lisREDU;
	VTFornecimento *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));

   //salva ponteiro para objeto Net
   net = mnet->Net;
   //loop p/ todas Reducoes
   lisREDU = mnet->LisReducao();
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      reducao = (VTReducao*)lisREDU->Items[nr];
      //inicia lisEQP c/ os Suprimentos da Reducao
      lisEQP->Clear();
      reducao->LisEqbar(lisEQP, eqptoSUPRIMENTO);
      for (int n = 0; n < lisEQP->Count; n++)
         {
			suprimento = (VTSuprimento*)lisEQP->Items[n];
			//redefine fases do Suprimento
			no = suprimento->pbarra->No[mnet->Net];
			//em Redes do tipo Fixo, pode haver uma parte isolada da rede que faz fronteira com outra,
			//isto é, a Barra do Suprimento não estará associada a um No
			if (no != NULL) suprimento->Fases = no->Fases;
			//redefine curva de tensão
         fornecimento->ReiniciaCurva(suprimento, defasagem);
         }
      }
   return(true);
   }

/*
//---------------------------------------------------------------------------
bool __fastcall TReduz::ReiniciaReducaoSuprimento1pu(VTMNet *mnet)
   {
   //variáveis locais
   int            fases_sup, fase_tag;
   double         p, q, v, t;
   VTSuprimento   *suprimento;
   VTReducao      *reducao;
   TList          *lisREDU;
   VTFases        *fases        = (VTFases*)apl->GetObject(__classid(VTFases));
   VTFornecimento *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));
   VTListaSupFase *listaSF      = fornecimento->ListaSupFase();
   VTPatamares    *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   int             max_pat   = patamares->NumPatamar();
   strSUP str_sup[MAX_FASE] = {{0., 0., 1.,  0.},
                               {0., 0., 1., -(2.*M_PI)/3.},
                               {0., 0., 1.,  (2.*M_PI)/3.},
                               {0., 0., 0.,  0.}};

   //loop p/ todas Reducoes
   lisREDU = mnet->LisReducao();
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      reducao = (VTReducao*)lisREDU->Items[nr];
      //inicia lisEQP c/ os Suprimentos da Reducao
      lisEQP->Clear();
      reducao->LisEqbar(lisEQP, eqptoSUPRIMENTO);
      for (int n = 0; n < lisEQP->Count; n++)
         {
         suprimento = (VTSuprimento*)lisEQP->Items[n];
         fases_sup  = suprimento->pbarra->resflow->Fases;
         //loop p/ todos os patamares
         for (int np = 0; np < max_pat; np++)
            {//atualiza patamar da curva PQVT por fase do Suprimento
            listaSF->Clear();
            for (int nf = 0; nf < MAX_FASE; nf++)
               {//verifica se existe a fase
               fase_tag = fases->IndexToTag[nf];
               if ((fases_sup & fase_tag) == fase_tag)
                  {
                  p = str_sup[nf].p;
                  q = str_sup[nf].q;
                  v = str_sup[nf].v;
                  t = str_sup[nf].t;
                  listaSF->Add(fase_tag, suprimento->tiposup, p, q, v, t);
                  }
               }
            fornecimento->PQVT[suprimento][np] = listaSF;
            }
         }
      }
   return(true);
   }
*/
//---------------------------------------------------------------------------
void __fastcall TReduz::SomaDemandaCarga(int np, TList *lisCAR, double *p, double *q)
   {
   //variáveis locais
   int             ind_fase;
   double          p_mw, q_mvar, v_pu;
   double          fat_icte, fat_scte, fat_zcte;
   complex<double> s_mva[MAX_FASE];
   VTCarga         *carga;
   VTDemanda       *demanda  = (VTDemanda*)apl->GetObject(__classid(VTDemanda));

   //loop p/ todas as Cargas em lisCAR
   for (int nc = 0; nc < lisCAR->Count; nc++)
      {
      carga = (VTCarga*)lisCAR->Items[nc];
      //obtém demanda por fase da Carga no patamar
      if(! demanda->Get_S3f_mva(carga, np, s_mva)) return; // Não conseguiu distribuir
      //loop de fases
      for(int ind_fase=indFASE_A; ind_fase <= indNEUTRO; ind_fase++)
         {
         p_mw   = s_mva[ind_fase].real();
         q_mvar = s_mva[ind_fase].imag();
         if (IsDoubleZero(p_mw + q_mvar)) continue;
         v_pu = Abs(carga->pbarra->resflow->Vfn_pu[np][ind_fase]);
         if (IsDoubleZero(v_pu)) continue;
         //determina parcelas de Icte, Scte e Zcte
         switch(ModeloCarga)
            {
            case cargaICTE: {fat_icte = 1.0; fat_scte = 0.0; fat_zcte = 0.0; break;}
            case cargaSCTE: {fat_icte = 0.0; fat_scte = 1.0; fat_zcte = 0.0; break;}
            case cargaZCTE: {fat_icte = 0.0; fat_scte = 0.0; fat_zcte = 1.0; break;}
            case cargaAUTO:
            default:
               fat_icte = carga->Icte_pu;
               fat_scte = carga->Scte_pu;
               fat_zcte = carga->Zcte_pu;
               break;
            }
         //trata parcela Icte
         if (fat_icte > 0)
            {
            p[ind_fase] += (p_mw   * fat_icte) / v_pu;
            q[ind_fase] += (q_mvar * fat_icte) / v_pu;
            }
         //trata parcela Scte
         if (fat_scte > 0)
            {
            p[ind_fase] += p_mw   * fat_scte;
            q[ind_fase] += q_mvar * fat_scte;
            }
         //trata parcela Zcte
         if (fat_zcte > 0)
            {
            p[ind_fase] += (p_mw   * fat_zcte) / (v_pu * v_pu);
            q[ind_fase] += (q_mvar * fat_zcte) / (v_pu * v_pu);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReduz::SomaFluxoLigacao(int np, TList *lisLIG, double *p, double *q, VTNo *no)
   {
   //variáveis locais
   int             ind_bar;
   complex<double> s_mva;
   VTBarra         *barra;
   VTLigacao       *ligacao;

   //loop p/ todas Ligacoes
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      if ((ind_bar = LigacaoConectaNo(ligacao, no)) < 0) continue;
      //loop/ todas as fases da Ligacao
      for (int nf = 0; nf < MAX_FASE; nf++)
         {//determina fluxo da Ligacao na fase
         s_mva = ligacao->resflow->Sf_mva[ind_bar][np][nf];
         p[nf] += s_mva.real();
         q[nf] += s_mva.imag();
         }
      }
   }

//---------------------------------------------------------------------------
//eof

