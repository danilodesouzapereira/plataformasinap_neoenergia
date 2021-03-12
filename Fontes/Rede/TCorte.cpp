//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCorte.h"
#include "VTBarra.h"
#include "VTMRede.h"
#include "VTRede.h"
#include "VTRedes.h"
#include "VTReducao.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCorte
//---------------------------------------------------------------------------
VTCorte* __fastcall NewObjCorte(void)
   {
   return(new TCorte);
   }

//---------------------------------------------------------------------------
__fastcall TCorte::TCorte(void)
   {
   //cria objetos
   lisBAR  = new TList();
   //lisMR   = new TList();
   lisREDE = new TList();
   //inicia dados
   pto_medio.x = pto_medio.y = 0;
   }

//---------------------------------------------------------------------------
__fastcall TCorte::~TCorte(void)
   {
   //destrói listas sem destruir seus objetos
   if (lisBAR)  {delete lisBAR;  lisBAR = NULL;}
   //if (lisMR)   {delete lisMR;   lisMR  = NULL;}
   if (lisREDE) {delete lisREDE; lisREDE  = NULL;}
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TCorte::ConectaMRede(VTMRede *mrede)
   {
   return(lisMR->IndexOf(mrede) >= 0);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TCorte::ExisteBarra(VTBarra *pbarra)
   {
   return(lisBAR->IndexOf(pbarra) >= 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TCorte::ExisteBarra(TList *lisEQP)
   {
   //variáveis locais
   VTBarra *pbarra;

   //verifica se alguma Barra do Corte está presente na lista lisEQP
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      pbarra = (VTBarra*)lisBAR->Items[nb];
      if (lisEQP->IndexOf(pbarra) >= 0) return(true);
      }
   return(false);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TCorte::ExisteMRede(VTMRede *mrede)
   {
   return(lisMR->IndexOf(mrede) >= 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TCorte::ExisteRedeSimples(void)
   {
   //variáveis locais
   VTRede    *rede;

   //verifica se pelo menos uma Rede do Corte é simples
   for (int n = 0; n < lisMR->Count; n++)
      {
      rede = (VTRede*)lisMR->Items[n];
      if (rede->Simples) return(true);
      }
   return(false);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TCorte::ExisteReducao(void)
	{
	//variáveis locais
	VTRede    *rede;

	//loop p/ todas as Redes do Corte
	for (int n = 0; n < lisREDE->Count; n++)
		{
		rede  = (VTRede*)lisREDE->Items[n];
		if (rede->MRede->ExisteReducao(this) != NULL) return(true);
		}
	return(false);
	}

//---------------------------------------------------------------------------
void __fastcall TCorte::InsereBarra(VTBarra *pbarra)
   {
   //proteção
   if (pbarra == NULL) return;
   //verifica se a Barra já existe na lista
   if (lisBAR->IndexOf(pbarra) < 0) lisBAR->Add(pbarra);
   }

//---------------------------------------------------------------------------
void __fastcall TCorte::InsereLisBarra(TList *lisEQP)
   {
   //variáveis locais
   VTBarra *pbarra;

   //insere Barras de lisEQP em lisBAR
   for (int nb = 0; nb < lisEQP->Count; nb++)
      {
      pbarra = (VTBarra*)lisEQP->Items[nb];
      InsereBarra(pbarra);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCorte::InsereLisRede(TList *lisEQP)
   {
   //insere todas Redes da lista
   for (int n = 0; n < lisEQP->Count; n++)
      {
      InsereRede((VTRede*)lisEQP->Items[n]);
      }
   }

/*
//---------------------------------------------------------------------------
void __fastcall TCorte::InsereMRede(VTMRede *mrede)
   {
   //proteção
   if (mrede == NULL) return;
   //verifica se a Rede já existe na lista
   if (lisMR->IndexOf(mrede) < 0)
      {//insere MRede no Corte
      lisMR->Add(mrede);
      //insere este Corte na MRede
      mrede->InsereCorte(this);
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TCorte::InsereRede(VTRede *rede)
   {
   //proteção
   if (rede == NULL) return;
   //verifica se a Rede já existe na lista
   if (lisREDE->IndexOf(rede) < 0)
      {//insere Rede em lisREDE
      lisREDE->Add(rede);
      //insere este Corte em Rede
      rede->InsereCorte(this);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TCorte::LisBarra(void)
   {
   return(lisBAR);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TCorte::LisBarra(TList *lisEQP)
   {
   CopiaTList(lisBAR, lisEQP);
   }
*/
//---------------------------------------------------------------------------
void __fastcall TCorte::LisBarra(TList *lisEQP)
   {
   //variáveis locais
   VTBarra *barra;

   //determina objetos VTEqbar dos objetos VTBarra
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      if (lisEQP->IndexOf(barra) < 0) lisEQP->Add(barra);
      }
	}

//---------------------------------------------------------------------------
void __fastcall TCorte::LisEqbar(TList *lisEQP, int tipo)
   {
   //variáveis locais
   VTBarra *barra;

   //determina objetos VTEqbar dos objetos VTBarra
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      barra->LisEqbar(lisEQP, tipo);
      }
   }
/*
//---------------------------------------------------------------------------
TList* __fastcall TCorte::LisMRede(void)
   {
   return(lisMR);
   }
*/
//---------------------------------------------------------------------------
void __fastcall TCorte::LisMRede(TList *lisEQP)
   {
   //variáveis locais
   VTMRede *mrede;
   VTRede  *rede;

   //loop p/ todas as Redes do Corte
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede  = (VTRede*)lisREDE->Items[n];
      mrede = rede->MRede;
      //insere MRede em lisEQP
      if (lisEQP->IndexOf(mrede) < 0) lisEQP->Add(mrede);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCorte::LisMRedeCarregada(TList *lisEQP)
   {
   //variáveis locais
   VTMRede *mrede;
   VTRede  *rede;

   //loop p/ todas as Redes do Corte
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede  = (VTRede*)lisREDE->Items[n];
      mrede = rede->MRede;
      if (! mrede->Carregada) continue;
      //insere MRede em lisEQP
      if (lisEQP->IndexOf(mrede) < 0) lisEQP->Add(mrede);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCorte::LisMRedeVizinha(VTMRede *mrede, TList *lisEQP)
   {
   //insere em lisEQP todas as MRedes do Corte
   LisMRede(lisEQP);
   //retira de lisEQP a MRede indicada
   lisEQP->Remove(mrede);
   }

//---------------------------------------------------------------------------
void __fastcall TCorte::LisMRedeVizinhaCarregada(VTMRede *mrede, TList *lisEQP)
   {
   //variáveis locais
   VTMRede *mrede_viz;
   VTRede  *rede;

   //loop p/ todas as Redes do Corte
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede  = (VTRede*)lisREDE->Items[n];
      mrede_viz = rede->MRede;
      if (mrede_viz == mrede) continue;
      if (! mrede_viz->Carregada) continue;
      //insere MRede em lisEQP
      if (lisEQP->IndexOf(mrede_viz) < 0) lisEQP->Add(mrede_viz);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TCorte::LisRede(void)
   {
   return(lisREDE);
   }

//---------------------------------------------------------------------------
void __fastcall TCorte::LisRede(TList *lisEQP)
   {
   //variáveis locais
   VTRede *rede;

   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (lisEQP->IndexOf(rede) < 0) lisEQP->Add(rede);
      }
   }
   
/*
//---------------------------------------------------------------------------
void __fastcall TCorte::LisRedeVizinha(VTRede *rede, TList *lisEQP)
   {
   //obtém lista de todas Redes do Corte
   LisRede(lisEQP);
   //retira Rede indicada de lisEQP
   lisEQP->Remove(rede);
   }
*/
//---------------------------------------------------------------------------
void __fastcall TCorte::LisRedeVizinha(VTRede *rede, TList *lisEQP)
   {
   //variáveis locais
   VTRede *rede_corte;

   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede_corte = (VTRede*)lisREDE->Items[n];
      //não inclui a rede indicada
      if (rede_corte == rede) continue;
      //insere a rede do corte em lisEQP
      if (lisEQP->IndexOf(rede_corte) < 0) lisEQP->Add(rede_corte);
      }
   }

/*
//---------------------------------------------------------------------------
void __fastcall TCorte::LisRedeVizinhaAtiva(VTRede *rede, TList *lisEQP)
   {
   //variáveis locais
   VTRede    *rede_viz;
   VTReducao *reducao_viz;

   for (int n = 0; n < lisMR->Count; n++)
      {
      rede_viz = (VTRede*)lisMR->Items[n];
      if (rede_viz == rede) continue;
      //obtém Reducao da Rede vizinha no Corte
      if ((reducao_viz = rede_viz->ExisteReducao(this)) == NULL) continue;
      if (reducao_viz->Nula()) continue;
      if (lisEQP->IndexOf(rede_viz) < 0) lisEQP->Add(rede_viz);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCorte::LisRedeVizinhaCarregada(VTRede *rede, TList *lisEQP)
   {
   //variáveis locais
   VTRede    *rede_viz;

   for (int n = 0; n < lisMR->Count; n++)
      {
      rede_viz = (VTRede*)lisMR->Items[n];
      if (rede_viz == rede) continue;
      //verifica se a Rede vizinha está carregada
      if (rede_viz->Carregada()) lisEQP->Add(rede_viz);
      }
   }
*/

//---------------------------------------------------------------------------
void __fastcall TCorte::LisReducao(TList *lisEQP)
	{
	//variáveis locais
	VTMRede   *mrede;
	VTRede    *rede;
	VTReducao *reducao;

	//loop p/ todas as Redes do Corte
	for (int n = 0; n < lisREDE->Count; n++)
		{
		rede  = (VTRede*)lisREDE->Items[n];
		mrede = rede->MRede;
		if ((reducao = mrede->ExisteReducao(this)) == NULL) continue;
		//insere Reducao em lisEQP
		if (lisEQP->IndexOf(reducao) < 0) lisEQP->Add(reducao);
		}
	}

//---------------------------------------------------------------------------
void __fastcall TCorte::LisReducaoVizinha(VTMRede *mrede, TList *lisEQP)
   {
   //variáveis locais
   VTReducao *reducao;

   //monta lista de c/ todas Reducoes do Corte
   LisReducao(lisEQP);
   //retira Reducao da MRede indicada
   lisEQP->Remove(mrede->ExisteReducao(this));
   }
   
/*
//---------------------------------------------------------------------------
void __fastcall TCorte::LisReducaoVizinha(VTRede *rede, TList *lisEQP)
   {
   //variáveis locais
   VTRede    *rede_viz;
   VTReducao *reducao_viz;

   for (int n = 0; n < lisMR->Count; n++)
      {
      rede_viz = (VTRede*)lisMR->Items[n];
      if (rede_viz == rede) continue;
      //verifica se a Rede vizinha está carregada
      //if (rede_viz->Carregada()) continue;
      //determina Reducao da Rede vizinha p/ este corte
      if ((reducao_viz = rede_viz->ExisteReducao(this)) == NULL) continue;
      if (lisEQP->IndexOf(reducao_viz) < 0) lisEQP->Add(reducao_viz);
      }
   }
*/
//---------------------------------------------------------------------------
int __fastcall TCorte::NumeroBarras(void)
   {
   return(lisBAR->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TCorte::NumeroMRedes(void)
   {
   //variáveis locais
   int    count = 0;
   TList *lisTMP;

   try{//cria lisTMP
      lisTMP = new TList();
      LisMRede(lisTMP);
      count = lisTMP->Count;
      //destrói lisTMP
      delete lisTMP;
      }catch(Exception &e)
         {}
   return(count);
   }

//---------------------------------------------------------------------------
int __fastcall TCorte::NumeroMRedesCarregadas(void)
   {
   //variáveis locais
   int    count = 0;
   TList *lisTMP;

   try{//cria lisTMP
      lisTMP = new TList();
      LisMRedeCarregada(lisTMP);
      count = lisTMP->Count;
      //destrói lisTMP
      delete lisTMP;
      }catch(Exception &e)
         {}
   return(count);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCorte::PM_GetCodigo(void)
   {
   //variáveis locais
   VTBarra *pbarra;

   //verifica se o código está definido
   if (! PD.codigo.IsEmpty()) return(PD.codigo);
   //define código usando código da primeira Barra
   if (lisBAR->Count > 0)
      {
      pbarra    = (VTBarra*)lisBAR->First();
      PD.codigo = "Corte[" + pbarra->Codigo + "]";
      return(PD.codigo);
      }
   else if (! PD.extern_id.IsEmpty()) return(PD.extern_id);
   return(IntToStr(Id));
   }

//---------------------------------------------------------------------------
int __fastcall TCorte::PM_NumeroBarra(void)
	{
	return(lisBAR->Count);
   }

//---------------------------------------------------------------------------
strCOORD __fastcall TCorte::PM_GetPtoMedio(void)
   {
   //variáveis locais
   VTBarra  *pbarra;

   //proteção
   if (lisBAR->Count == 0) return(pto_medio);
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      pbarra   = (VTBarra*)lisBAR->Items[nb];
      pto_medio.x += pbarra->esq.x1 + pbarra->esq.x2;
      pto_medio.y += pbarra->esq.y1 + pbarra->esq.y2;
      }
   pto_medio.x /= (lisBAR->Count + lisBAR->Count);
   pto_medio.y /= (lisBAR->Count + lisBAR->Count);
   return(pto_medio);
   }

//---------------------------------------------------------------------------
void __fastcall TCorte::PM_SetPtoMedio(strCOORD coord)
   {
   pto_medio = coord;
   }

/*
//---------------------------------------------------------------------------
void __fastcall TCorte::ReiniciaLisRede(VTRedes *redes)
   {
   ////variáveis locais
   VTRede *rede;
   TList  *lisEQP;

   //reinicia lista lisMR
   lisMR->Clear();
   //determina objetos VTRede que se ligam a este Corte
   lisEQP = redes->LisRede();
   for (int n = 0; n < lisEQP->Count; n++)
      {
      rede = (VTRede*)lisEQP->Items[n];
      if (ConectaRede(rede))
         {//Corte conecta a Rede: insere Rede em lisEQP
         if (lisMR->IndexOf(rede) < 0) lisMR->Add(rede);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCorte::ReiniciaLisBarra(void);
   {
   //limpa lista
   lisBAR->Clear();
   }


//---------------------------------------------------------------------------
void __fastcall TCorte::ReiniciaLisMRede(void)
   {
   //limpa listas
   lisMR->Clear();
   }
*/
//---------------------------------------------------------------------------
void __fastcall TCorte::ReiniciaLisRede(void)
   {
   //limpa listas
   lisREDE->Clear();
   }
/*
//---------------------------------------------------------------------------
void __fastcall TCorte::RemoveMRede(VTMRede *mrede)
   {
   //retira MRede da lista lisMR
   if (lisMR->Remove(mrede) >= 0)
      {//retira este Corte da MRede
      mrede->RemoveCorte(this);
      }
   }
*/

//---------------------------------------------------------------------------
void __fastcall TCorte::RemoveRede(TList *lisEQP)
   {
   //variáveis locais
   VTRede *rede;

   //remove Redes de lisREDE
   for (int n = 0; n < lisEQP->Count; n++)
      {
      rede = (VTRede*)lisEQP->Items[n];
      lisREDE->Remove(rede);
      }
   }

//---------------------------------------------------------------------------
int __fastcall TCorte::Tipo(void)
   {
   return(eqptoCORTE);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCorte::TipoAsString(void)
   {
   return("Corte");
   }

//---------------------------------------------------------------------------
//eof

