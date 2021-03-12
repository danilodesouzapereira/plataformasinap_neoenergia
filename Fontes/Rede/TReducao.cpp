//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TReducao.h"
#include "VTBarra.h"
#include "VTCarga.h"
#include "VTCorte.h"
#include "VTEqbar.h"
#include "VTResFlowBar.h"
#include "VTTrecho.h"
#include "VTMRede.h"
#include "..\Mercado\VTCrescimento.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTReducao
//---------------------------------------------------------------------------
VTReducao* __fastcall NewObjReducao(int num_pat)
   {
   return(new TReducao(num_pat));
   }

//---------------------------------------------------------------------------
__fastcall TReducao::TReducao(int num_pat)
   {
   try{//cria objetos
      lisBAR = new TList();
      lisEQB = new TList();
      lisLIG = new TList();
      //objetos p/ esquemático
      esq.pbarra = NewObjBarra(num_pat);
      esq.lisBAR = new TList();
      esq.lisLIG = new TList();
      //inicia dados
      //nula             = false;
      Valida           = false;
      Show[eqp_FIXO]   = false;
      Show[rdu_EQPTOS] = false;
      Show[rdu_P]      = true;
      Show[rdu_Q]      = true;
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16233879
__fastcall TReducao::~TReducao(void)
   {
//JCG 2008.05.09 - dúvida: bloquear retirada das Redes no Corte
   //retira Rede do Corte
   //if ((corte)&&(mrede)) corte->RemoveRede(mrede->LisRede());
//
    //remove ligações da lista de ligação de suas barras
   RemoveLigDaBarra(lisLIG);
   //RemoveLigDaBarra(esq.lisLIG);
   //destrói listas sem destruir seus objetos
   if (lisBAR) {delete lisBAR; lisBAR = NULL;}
   //destrói listas e seus objetos
   if (lisEQB) {LimpaTList(lisEQB); delete lisEQB; lisEQB = NULL;}
   if (lisLIG) {LimpaTList(lisLIG); delete lisLIG; lisLIG = NULL;}
   //destrói objetos usados p/ esquemático
   if (esq.pbarra) {delete esq.pbarra; esq.pbarra = NULL;}
   if (esq.lisBAR) {LimpaTList(esq.lisBAR); delete esq.lisBAR; esq.lisBAR = NULL;}
   if (esq.lisLIG) {LimpaTList(esq.lisLIG); delete esq.lisLIG; esq.lisLIG = NULL;}
   //destrói objetos
    if (crescimento) {delete crescimento; crescimento = NULL;}
  }

//---------------------------------------------------------------------------
VTBarra* __fastcall  TReducao::BarraFicticia(VTLigacao *ligacao)
   {
   //variáveis locais
   VTBarra *barra;

   //verifica se foi indicada uma Ligacao do esquemático
   if (ligacao == NULL) {return(esq.pbarra);}
   //determina Barra fictícia da Ligacao
   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {
      if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
      if (esq.lisBAR->IndexOf(barra) >= 0) return(barra);
      }
   //por segurança, retorna Barra do esquemático
   return(esq.pbarra);
   }

//---------------------------------------------------------------------------
VTReducao* __fastcall TReducao::Clone(void)
   {
   //variáveis locais
   VTReducao *reducao;

   //cria uma nova Barra
   if ((reducao = NewObjReducao(esq.pbarra->resflow->Max_pat)) != NULL)
      {//copia dados desta Reducao p/ a nova Reducao
      reducao->CopiaAtributosDe(*this);
      }
   return(reducao);
   }

//---------------------------------------------------------------------------
void __fastcall TReducao::CopiaAtributosDe(VTEqpto &ref)
   {
   //variáveis locais
   TReducao  &reducao = (TReducao&)ref;
   VTEqbar   *eqbar;
   VTLigacao *ligacao;
   TList     *lisEXT;

   //proteção
   if (ref.Tipo() != eqptoREDUCAO) return;
   //destrói objetos usados p/ esquemático
   if (esq.pbarra) {delete esq.pbarra; esq.pbarra = NULL;}
   LimpaTList(esq.lisLIG);
   //limpa listas
   lisBAR->Clear();
   LimpaTList(lisEQB);
   LimpaTList(lisLIG);
   //copia atributos de VTEqpto
   VTEqpto::CopiaAtributosDe(reducao);
   //copia atributos de Reducao
   valida      = reducao.valida;
   mrede       = reducao.mrede;
   corte       = reducao.corte;
   //copia objetos usados p/ esquemático
   esq.height = reducao.esq.height;
   esq.width  = reducao.esq.width;
   esq.pbarra = reducao.esq.pbarra->Clone();
   lisEXT     = reducao.esq.lisLIG;
   for (int n = 0; n < lisEXT->Count; n++)
      {
      ligacao = (VTLigacao*)lisEXT->Items[n];
      esq.lisLIG->Add(ligacao->Clone());
      }
   //copia lista de Barras
   reducao.LisBarra(lisBAR);
   //duplica Eqbar
   lisEXT = reducao.LisEqbar();
   for (int n = 0; n < lisEXT->Count; n++)
      {
      eqbar = (VTEqbar*)lisEXT->Items[n];
      lisEQB->Add(eqbar->Clone());
      }
   //duplica Ligacoes
   lisEXT = reducao.LisLigacao();
   for (int n = 0; n < lisEXT->Count; n++)
      {
      ligacao = (VTLigacao*)lisEXT->Items[n];
      lisLIG->Add(ligacao->Clone());
      }
   //copia Crescimento
   crescimento = reducao.Crescimento->Clone();
   }

//---------------------------------------------------------------------------
bool __fastcall TReducao::ExisteEqbarLigacaoConectaBarra(VTBarra *barra)
   {
   //variáveis locais
   VTEqbar   *eqbar;
   VTLigacao *pliga;

   //loop p/ todos objetos VTEqbar em lisEQB
   for (int n = 0; n < lisEQB->Count; n++)
      {
      eqbar = (VTEqbar*)lisEQB->Items[n];
      if (eqbar->pbarra == barra) return(true);
      }
   //loop p/ todos objetos VTLigacao em lisLIG
   for (int n = 0; n < lisLIG->Count; n++)
      {
      pliga = (VTLigacao*)lisLIG->Items[n];
      if (pliga->ConectaBarra(barra)) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TReducao::ExisteGeracao(void)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //loop p/ todos objetos VTEqpto em lisEQB
   for (int n = 0; n < lisEQB->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQB->Items[n];
      if (eqpto->Tipo() == eqptoGERADOR)    return(true);
      if (eqpto->Tipo() == eqptoSUPRIMENTO) return(true);
      }
	return(false);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TReducao::FirstRede(void)
	{
	//variáveis locais
	TList  *lisREDE;
	VTRede *rede = NULL;

	//proteção
	if (MRede != NULL)
		{//determina primeira Rede da MRede
		lisREDE = MRede->LisRede();
		if (lisREDE->Count > 0)
			{
			rede = (VTRede*)lisREDE->First();
			}
		}
	return(rede);
	}

//---------------------------------------------------------------------------
void __fastcall TReducao::IniciaEsquematico(int num_pat)
   {
   //variáveis locais
   int       x1, y1, x2, y2;
   VTBarra   *pbarra, *pbar_fic;
   VTLigacao *pliga;
	TList     *lisBAR_COR;
	VTRede    *rede;

	//determina
   //reinicia listas esq.lisBAR e  esq.lisLIG
   LimpaTList(esq.lisBAR);
   LimpaTList(esq.lisLIG);
   try{//cria Barra p/ conectar Ligacoes do esquemático
      if (esq.pbarra == NULL) esq.pbarra = NewObjBarra(num_pat);
      //define coordenadas da Barra a partir da área da rede
      mrede->AreaEsquematico(x1, y1, x2, y2);
      esq.pbarra->esq.x1 = (x1 + x2)/2;
      esq.pbarra->esq.y1 = (y1 + y2)/2;
      esq.pbarra->esq.x2 = esq.pbarra->esq.x1;
      esq.pbarra->esq.y2 = esq.pbarra->esq.y1;
      //copia coordenadas
      esq.pbarra->esq_orig = esq.pbarra->esq;
      //define coordenadas da Barra a partir da área da rede
      mrede->AreaUtm_cm(x1, y1, x2, y2);
      esq.pbarra->utm.x  = (x1 + x2)/2;
      esq.pbarra->utm.y  = (y1 + y2)/2;
      //cria uma Barra e um Trecho fictício p/ ligar cada Barra do Corte
      lisBAR_COR = corte->LisBarra();
      for (int nb = 0; nb < lisBAR_COR->Count; nb++)
         {
         pbarra = (VTBarra*)lisBAR_COR->Items[nb];
         //verifica se existe uma Ligacao da Reducao conectando a Barra
         if (! ExisteEqbarLigacaoConectaBarra(pbarra)) continue;
         //verifica se a Barra do Corte existe em lisBAR
         if (lisBAR->IndexOf(pbarra) < 0)
            {//insere Barra do Corte em lisBAR
            lisBAR->Add(pbarra);
            }
         //cria uma nova Barra fictícia
         esq.lisBAR->Add(pbar_fic = NewObjBarra(num_pat));
         //define coordenadas da Barra fictícia
         pbar_fic->esq = esq.pbarra->esq;
         //copia coordenadas
         pbar_fic->esq_orig = pbar_fic->esq;
         //define coordenadas utm da Barra fictícia
         pbar_fic->utm = esq.pbarra->utm;
         //cria um novo Trecho e insere na lista
		 esq.lisLIG->Add(pliga = NewObjTrecho(num_pat));
         pliga->Codigo  = "Ligacao " + IntToStr(esq.lisLIG->Count + 1);
			pliga->DefineObjBarra(pbar_fic, pbarra);
			//associa Ligacao com uma Rede da MRede mas sem inserir Ligacao na Rede
			pliga->rede = FirstRede();
			//configura opção p/ não exibir nenhum atributo
         pliga->ShowAsUnsigned    = 0;
         pliga->Show[eqp_VISIBLE] = true;
         pliga->Show[eqp_FIXO]    = Show[eqp_FIXO];
         pliga->Show[eqp_ID]      = Show[eqp_ID];
         pliga->Show[eqp_CODIGO]  = Show[eqp_CODIGO];
         pliga->Show[lig_P]       = Show[rdu_P];
         pliga->Show[lig_Q]       = Show[rdu_Q];
         pliga->Show[lig_S]       = Show[rdu_S];
         }
      //define dimensões do retângulo da Reducao
      esq.height = 50;
      esq.width  = 150;
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TReducao::IniciaInterconexao(int num_pat)
   {
   //variáveis locais
   int       x1, y1, x2, y2;
   VTMRede   *mrede_viz;
   VTReducao *redu_viz;
   VTLigacao *pliga;
   TList     *lisREDU_VIZ;

   //reinicia listas esq.lisBAR e esq.lisLIG
   LimpaTList(esq.lisBAR);
   LimpaTList(esq.lisLIG);
   try{//cria TList
      lisREDU_VIZ = new TList();
      //cria Barra p/ conectar Ligacoes do esquemático
      if (esq.pbarra == NULL) esq.pbarra = NewObjBarra(num_pat);
      //define coordenadas esquemático
      mrede->AreaEsquematico(x1, y1, x2, y2);
      esq.pbarra->esq.x1 = (x1 + x2)/2;
      esq.pbarra->esq.y1 = (y1 + y2)/2;
      esq.pbarra->esq.x2 = esq.pbarra->esq.x1;
      esq.pbarra->esq.y2 = esq.pbarra->esq.y1;
      //define coordenadas utm
      mrede->AreaUtm_cm(x1, y1, x2, y2);
      esq.pbarra->utm.x  = (x1 + x2)/2;
      esq.pbarra->utm.y  = (y1 + y2)/2;
      //cria lista p/ Ligacoes do esquemático
      if (esq.lisLIG == NULL) esq.lisLIG = new TList();
      //CUIDADO: na montagem do esquemático pode haver Reducao sem Corte
      if (corte != NULL)
         {//obtém lista de Reducoes vizinhas
         lisREDU_VIZ->Clear();
         corte->LisReducaoVizinha(MRede, lisREDU_VIZ);
         //cria um Trecho fictício p/ ligar a Barra fictícia a cada Barra das demais Reducoes do Corte
         for (int nr = 0; nr < lisREDU_VIZ->Count; nr++)
            {
            redu_viz = (VTReducao*)lisREDU_VIZ->Items[nr];
            //verifica se a Reducao já possui Barra ficticia
            if (redu_viz->BarraFicticia() == NULL) continue;
            //cria um novo Trecho e insere na lista
            pliga         = NewObjTrecho(num_pat);
            pliga->Codigo = "Ligacao " + IntToStr(esq.lisLIG->Count + 1);
            pliga->DefineObjBarra(esq.pbarra, redu_viz->BarraFicticia());
            esq.lisLIG->Add(pliga);
            }
         }
      //define dimensões do retângulo da Reducao
      esq.height = 50;
      esq.width  = 150;
      //destrói TList
      delete lisREDU_VIZ;
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TReducao::InsereEqbar(VTEqbar *eqbar)
   {
   //proteção
   if (eqbar == NULL) return;
   //insere objeto VTEqbar na lista lisEQB
   if (lisEQB->IndexOf(eqbar) < 0) lisEQB->Add(eqbar);
   //insere Barra do Eqbar em lisBAR
   if (lisBAR->IndexOf(eqbar->pbarra) < 0) lisBAR->Add(eqbar->pbarra);
   //verifica se Eqbar é uma Carga
   if (eqbar->Tipo() == eqptoCARGA)
      {//associa Carga c/ Crescimento da Reducao
		((VTCarga*)eqbar)->crescimento = crescimento;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReducao::InsereLigacao(VTLigacao *ligacao)
	{
	//proteção
   if (ligacao == NULL) return;
   //insere objeto VTLigacao na lista lisLIG
   if (lisLIG->IndexOf(ligacao) < 0) lisLIG->Add(ligacao);
   //insere Barras da Ligacao em lisBAR
   if (lisBAR->IndexOf(ligacao->pbarra1) < 0) lisBAR->Add(ligacao->pbarra1);
	if (lisBAR->IndexOf(ligacao->pbarra2) < 0) lisBAR->Add(ligacao->pbarra2);
	//associa Ligacao com uma Rede da MRede mas sem inserir Ligacao na Rede
	ligacao->rede = FirstRede();
	}

//---------------------------------------------------------------------------
TList* __fastcall TReducao::LisBarra(void)
   {
   return(lisBAR);
   }

//---------------------------------------------------------------------------
void __fastcall TReducao::LisBarra(TList *lisEQP)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //loop p/ todos objetos VTBarra em lisBAR
   for (int n = 0; n < lisBAR->Count; n++)
      {
      eqpto = (VTEqpto*)lisBAR->Items[n];
      if (lisEQP->IndexOf(eqpto) < 0) lisEQP->Add(eqpto);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TReducao::LisBarraEsquematico(void)
   {
   return(esq.lisBAR);
   }

//---------------------------------------------------------------------------
TList* __fastcall TReducao::LisEqbar(void)
   {
   return(lisEQB);
   }

//---------------------------------------------------------------------------
void __fastcall TReducao::LisEqbar(TList *lisEQP)
   {
   //variáveis locais
   VTEqbar *eqbar;

   //loop p/ todos objetos VTEbar em lisEQB
   for (int n = 0; n < lisEQB->Count; n++)
      {
      eqbar = (VTEqbar*)lisEQB->Items[n];
      if (lisEQP->IndexOf(eqbar) < 0) lisEQP->Add(eqbar);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReducao::LisEqbar(TList *lisEQP, int tipo)
   {
   //variáveis locais
   VTEqbar *eqbar;

   //verifica se todos Eqbar
   if ((tipo < 0)||(tipo == eqptoEQBAR))
      {
      LisEqbar(lisEQP);
      return;
      }
   //loop p/ todos objetos VTEbar em lisEQB
   for (int n = 0; n < lisEQB->Count; n++)
      {
      eqbar = (VTEqbar*)lisEQB->Items[n];
      if (eqbar->Tipo() == tipo)
         {
         if (lisEQP->IndexOf(eqbar) < 0) lisEQP->Add(eqbar);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReducao::LisEqpto(TList *lisEQP)
   {
   //insere em lisEQP todos objetos VTEqbar e VTLigacao da Reducao
   LisBarra(lisEQP);
   LisEqbar(lisEQP);
   LisLigacao(lisEQP);
   }

//---------------------------------------------------------------------------
void __fastcall TReducao::LisEqpto(TList *lisEQP, int tipo)
   {
   //verifica o tipo de eqpto solicitado
   if      (tipo <  0)            LisEqpto(lisEQP);
   else if (tipo == eqptoBARRA)   LisBarra(lisEQP);
   else if (tipo == eqptoEQBAR)   LisEqbar(lisEQP);
   else if (tipo == eqptoLIGACAO) LisLigacao(lisEQP);
   else
      {
      LisEqbar(lisEQP, tipo);
      LisLigacao(lisEQP, tipo);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TReducao::LisLigacao(void)
   {
   return(lisLIG);
   }

//---------------------------------------------------------------------------
void __fastcall TReducao::LisLigacao(TList *lisEQP)
   {
   //variáveis locais
   VTLigacao *pliga;

   //loop p/ todos objetos VTLigacao em lisLIG
   for (int n = 0; n < lisLIG->Count; n++)
      {
      pliga = (VTLigacao*)lisLIG->Items[n];
      if (lisEQP->IndexOf(pliga) < 0) lisEQP->Add(pliga);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReducao::LisLigacao(TList *lisEQP, int tipo)
   {
   //variáveis locais
   VTLigacao *pliga;

   //verifica se todos Eqbar
   if ((tipo < 0)||(tipo == eqptoLIGACAO))
      {
      LisLigacao(lisEQP);
      return;
      }
   //loop p/ todos objetos VTLigacao em lisLIG
   for (int n = 0; n < lisLIG->Count; n++)
      {
      pliga = (VTLigacao*)lisLIG->Items[n];
      if (pliga->Tipo() == tipo)
         {
         if (lisEQP->IndexOf(pliga) < 0) lisEQP->Add(pliga);
         }
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TReducao::LisLigacaoEsquematico(void)
   {
   return(esq.lisLIG);
   }

//---------------------------------------------------------------------------
int __fastcall TReducao::NumeroDeEqbar(int tipo)
   {
   //variáveis locais
   VTEqpto *eqpto;
   int     count = 0;

   //verifica se não foi definido o tipo do Eqbar
   if ((tipo < 0)||(tipo == eqptoEQBAR)) return(lisEQB->Count);
   //loop p/ todos objetos VTEqpto em lisEQB
   for (int n = 0; n < lisEQB->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQB->Items[n];
      if (eqpto->Tipo() == tipo) count++;
      }
   return(count);
   }

//---------------------------------------------------------------------------
bool __fastcall TReducao::PM_GetCarregada(void)
   {
   return(! mrede->Carregada);
   }

//---------------------------------------------------------------------------
VTCorte* __fastcall TReducao::PM_GetCorte(void)
   {
   return(corte);
   }

//---------------------------------------------------------------------------
VTCrescimento* __fastcall TReducao::PM_GetCrescimento(void)
   {
   return(crescimento);
   }

//---------------------------------------------------------------------------
VTMRede* __fastcall TReducao::PM_GetMRede(void)
   {
   return(mrede);
   }

//---------------------------------------------------------------------------
int __fastcall TReducao::PM_GetNumeroEqpto(void)
   {
   return(lisBAR->Count + lisLIG->Count + lisEQB->Count);
   }

//---------------------------------------------------------------------------
bool __fastcall TReducao::PM_GetValida(void)
   {
   return(valida);
   }

//---------------------------------------------------------------------------
void __fastcall TReducao::PM_SetCorte(VTCorte *corte)
   {
   //salva ponteiro p/ objeto VTCorte
   this->corte = corte;
   }

//---------------------------------------------------------------------------
void __fastcall TReducao::PM_SetCrescimento(VTCrescimento *crescimento)
   {
   //variáveis locais
   VTEqbar *eqbar;

   //destrói eventual Crescimento pré-existente
   if ((this->crescimento)&&(this->crescimento != crescimento)) delete this->crescimento;
   //salva ponteiro p/ objeto
   this->crescimento = crescimento;
   //associa todas Cargas c/ Crescimento
   for (int n = 0; n < lisEQB->Count; n++)
      {
      eqbar = (VTEqbar*)lisEQB->Items[n];
      if (eqbar->Tipo() == eqptoCARGA)
         {//associa Carga c/ Crescimento da Reducao
         ((VTCarga*)eqbar)->crescimento = crescimento;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReducao::PM_SetMRede(VTMRede *mrede)
   {
   this->mrede = mrede;
   //define código igual ao da MRede
   Codigo = mrede->Codigo;
   }

//---------------------------------------------------------------------------
void __fastcall TReducao::PM_SetShow(unsigned bit, bool enabled)
   {
   //variáveis locais
   VTEqpto *eqpto;

   //utiliza método da classe básica
   VTEqpto::PM_SetShow(bit, enabled);
   //IMPORTANTE na iniciação
   if (esq.lisLIG == NULL) return;
   //atualiza atributos dos Trechos fictícios
   for (int n = 0; n < esq.lisLIG->Count; n++)
      {
      eqpto = (VTEqpto*)esq.lisLIG->Items[n];
      eqpto->Show[bit] = enabled;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TReducao::PM_SetValida(bool valida)
   {
   this->valida = valida;
   }

//---------------------------------------------------------------------------
void __fastcall TReducao::Reinicia(void)
   {
   //limpa lista lisBAR sem destruir seus objetos
   lisBAR->Clear();
   //destrói todos Eqptos da Reducao
   LimpaTList(lisEQB);
   LimpaTList(lisLIG);
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16233879
void __fastcall TReducao::RemoveLigDaBarra(TList *lisLIG)
   {
   //variáveis locais
   VTLigacao *ligacao;

   for(int n = 0; n < lisLIG->Count; n++)
	   {
	   ligacao = (VTLigacao*)lisLIG->Items[n];
       //garante que as barras da ligação não tenham mais a ligação em sua lista
	   if (ligacao->pbarra1 != NULL) ligacao->pbarra1->RemoveLigacao(ligacao);
	   if (ligacao->pbarra2 != NULL) ligacao->pbarra2->RemoveLigacao(ligacao);
	   if (ligacao->pbarra3 != NULL) ligacao->pbarra3->RemoveLigacao(ligacao);
	   }
   }

//---------------------------------------------------------------------------
int __fastcall TReducao::Tipo(void)
   {
   return(eqptoREDUCAO);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TReducao::TipoAsString(void)
   {
   return("Reducao");
   }

//---------------------------------------------------------------------------
bool __fastcall TReducao::Visible(void)
   {
   //if (! Show[eqp_VISIBLE]) return(false);
   //return(mrede->Visible));
   return(Show[eqp_VISIBLE]);
   }
   
//---------------------------------------------------------------------------
//eof

