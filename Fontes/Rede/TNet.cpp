//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TNet.h"
#include "VTBarra.h"
#include "VTCarga.h"
#include "VTEqbar.h"
#include "VTGrupo.h"
#include "VTLigacao.h"
#include "VTMutua.h"
#include "VTNo.h"
#include "VTRede.h"
#include "VTReducao.h"
#include "VTRegulador.h"
#include "VTSuprimento.h"
#include "VTTrafo.h"
#include "VTTramo.h"
#include "VTTrecho.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTNet
//---------------------------------------------------------------------------
VTNet* __fastcall NewObjNet(void)
   {
   return(new TNet());
   }

//---------------------------------------------------------------------------
__fastcall TNet::TNet(void)
   {
   //cria listas
   lisBAR  = new TList();
   lisEQB  = new TList();
   lisGRP  = new TList();
   lisLIG  = new TList();
   lisLSC  = new TList();
   lisLSCM = new TList();
   lisMUT  = new TList();
   lisNO   = new TList();
   lisTRM  = new TList();
   //listas da parte isolada
   isolada.lisBAR  = new TList();
   isolada.lisLIG  = new TList();
   //inicia dados
   Codigo      = "";
   Radial      = false;
   ModeloCarga = cargaICTE;
   StatusFluxo = fluxoNAO_CALCULADO;
   }

//---------------------------------------------------------------------------
__fastcall TNet::~TNet(void)
   {
   //IMPORTANTE: cancela associação das Barras c/ Nos
   //CancelaNoDaBarra();
   //destrói lista sem destruir seus objetos
   if (lisBAR)  {delete lisBAR;  lisBAR  = NULL;}
   if (lisEQB)  {delete lisEQB;  lisEQB  = NULL;}
   if (lisLIG)  {delete lisLIG;  lisLIG  = NULL;}
   if (lisLSC)  {delete lisLSC;  lisLSC  = NULL;}
   if (lisLSCM) {delete lisLSCM; lisLSCM = NULL;}
   if (lisMUT)  {delete lisMUT;  lisMUT  = NULL;}
   //destrói lista sem destruir seus objetos
   if (isolada.lisBAR)  {delete isolada.lisBAR;  isolada.lisBAR  = NULL;}
   if (isolada.lisLIG)  {delete isolada.lisLIG;  isolada.lisLIG  = NULL;}
   //destrói lista e seus objetos
   if (lisGRP) {LimpaTList(lisGRP); delete lisGRP; lisGRP = NULL;}
   if (lisNO)  {LimpaTList(lisNO);  delete lisNO;  lisNO  = NULL;}
   if (lisTRM) {LimpaTList(lisTRM); delete lisTRM; lisTRM = NULL;}
   }
/*
//---------------------------------------------------------------------------
void __fastcall TNet::CancelaNoDaBarra(void)
   {
   //variáveis locais
   VTBarra *pbarra;

   //proteção
   if (lisBAR == NULL) return;
   for (int n = 0; n < lisBAR->Count; n++)
      {
      pbarra = (VTBarra*)lisBAR->Items[n];
      pbarra->ReiniciaLisNo();
      }
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TNet::ExisteDesequilibrio(void)
   {
   //variáveis locais
   VTEqpto      *eqpto;
   VTCarga      *carga;
   VTRegulador *regulador;
   VTSuprimento *suprimento;
   VTTrafo      *trafo;
   VTTramo      *tramo;
	VTTrecho     *trecho;

	//verifica se há alguma Carga ou Suprimento desequilibrada
   for (int n = 0; n < lisEQB->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQB->Items[n];
      if (eqpto->Tipo() == eqptoCARGA)
         {
         carga = (VTCarga*)eqpto;
         if (! carga->Equilibrada) {return(true);}
         }
      else if (eqpto->Tipo() == eqptoSUPRIMENTO)
         {
         suprimento = (VTSuprimento*)eqpto;
         if (suprimento->DefinidoPorFase) {return(true);}
         }
      }
   //verifica se há algum Trecho, Tramo, Trafo ou Regulador desequilibrado
   for (int n = 0; n < lisLIG->Count; n++)
      {
      eqpto = (VTEqpto*)lisLIG->Items[n];
      if (eqpto->Tipo() == eqptoTRECHO)
         {
         trecho = (VTTrecho*)eqpto;
         if (! trecho->Equilibrado) {return(true);}
         }
      else if (eqpto->Tipo() == eqptoTRAMO)
         {
         tramo = (VTTramo*)eqpto;
         if (! tramo->Equilibrado) {return(true);}
         }
      else if (eqpto->Tipo() == eqptoTRAFO)
         {
         trafo = (VTTrafo*)eqpto;
         if (trafo->Monofasico) {return(true);}
         }
      else if (eqpto->Tipo() == eqptoREGULADOR)
         {
         regulador = (VTRegulador*)eqpto;
         if ((regulador->fases & faseABC) != faseABC) {return(true);}
         }
      }
   return(false);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TNet::ExisteGeracaoPV(void)
   {
   //variáveis locais
   VTNo *no;

   //verifica se algum No possui GeracaoPV
   for (int n = 0; n < lisNO->Count; n++)
      {
      no = (VTNo*)lisNO->Items[n];
      if (no->ExisteGeracaoPV()) return(true);
      }
   return(false);
   }
*/
//---------------------------------------------------------------------------
VTGrupo* __fastcall TNet::ExisteGrupoMutua(VTArranjo *arranjo, VTLigacao *ligacao)
   {
   //variáveis locais
   int       dist_max_cm;
   int       x1, y1, x2, y2;
   int       xa, ya, xb, yb;
   TList     *lisLIG;
   VTGrupo   *grupo;
   VTLigacao *ligacao_grupo;

   //obtém coordenadas do Trecho
   if (! ligacao->CoordenadasUtm_cm(x1, y1, x2, y2)) return(NULL);
   //verifica se o Trecho pode ser associado a um Grupo de Mutuas já existente
   for (int n = 0; n < lisGRP->Count; n++)
      {
      grupo = (VTGrupo*)lisGRP->Items[n];
      //verifica se o Grupo está associado ao Arranjdo indicado
      if (grupo->Arranjo != arranjo) continue;
      //verifica se o Grupo possui o Trecho indicado
      if (grupo->ExisteLigacao(ligacao)) return(grupo);
      //determinadas coordenadas do primeiro Trecho do Grupo
      lisLIG        = grupo->LisLigacao();
      ligacao_grupo = (VTLigacao*)(lisLIG->First());
      if (! ligacao_grupo->CoordenadasUtm_cm(xa, ya, xb, yb)) return(NULL);
      //define dist_max entre os pontos dos ligacaos
      if (ligacao->ComprimentoUtm_cm() < 10) dist_max_cm = 100;
      else                                   dist_max_cm = 500;
      //verifica se o Trecho possui as mesmas coordenadas dos demais Trechos do Grupo
      if ((Distancia_cm(xa, ya, x1, y1) < dist_max_cm)&&
          (Distancia_cm(xb, yb, x2, y2) < dist_max_cm)) return(grupo);
      if ((Distancia_cm(xb, yb, x1, y1) < dist_max_cm)&&
          (Distancia_cm(xa, ya, x2, y2) < dist_max_cm)) return(grupo);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TNet::ExisteGrupoMutua(VTMutua *mutua)
   {
   //variáveis locais
   VTGrupo *grupo;
   VTMutua *mutua_grp;
   TList   *lisMUT_GRP;

   //loop p/ todos Grupos de Mutuas
   for (int ng = 0; ng < lisGRP->Count; ng++)
      {
      grupo = (VTGrupo*)lisGRP->Items[ng];
      //loop p/ todas Mutuas do Grupo
      lisMUT_GRP = grupo->LisMutua();
      for (int nm = 0; nm < lisMUT_GRP->Count; nm++)
         {
         mutua_grp = (VTMutua*)lisMUT_GRP->Items[nm];
         //verifica se a Mutua possui Trecho em comum c/ Mutua  do Grupo
         if (mutua_grp->ExisteTrechoComum(mutua)) return(grupo);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTGrupo* __fastcall TNet::ExisteGrupoMutua(VTTrecho *trecho)
   {
   //variáveis locais
   VTGrupo *grupo;

   //verifica se o Trecho pertence a um Grupo de Mutuas
   for (int n = 0; n < lisGRP->Count; n++)
      {
      grupo = (VTGrupo*)lisGRP->Items[n];
      if (grupo->ExisteLigacao(trecho)) return(grupo);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTMutua* __fastcall TNet::ExisteMutua(VTTrecho *trecho)
   {
   //variáveis locais
   VTMutua *mutua;

   //loop p/ todas as Mutuas
   for (int n = 0; n < lisMUT->Count; n++)
      {
      mutua = (VTMutua*)lisMUT->Items[n];
      if (mutua->ExisteTrecho(trecho)) return(mutua);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TNet::InsereBarra(VTBarra *barra)
   {
   //proteção
   if (barra == NULL) return(false);
   //insere objeto VTBarra na lista lisBAR
   if (lisBAR->IndexOf(barra) < 0)
      {
      lisBAR->Add(barra);
      return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TNet::InsereEqbar(VTEqbar *eqbar)
   {
   //proteção
   if (eqbar == NULL) return;
   //insere objeto VTEqbar na lista lisEQB
   if (lisEQB->IndexOf(eqbar) < 0) lisEQB->Add(eqbar);
   }

//---------------------------------------------------------------------------
void __fastcall TNet::InsereLigacao(VTLigacao *ligacao)
   {
   //proteção
   if (ligacao == NULL) return;
   //insere objeto VTLigacao na lista lisLIG
   if (lisLIG->IndexOf(ligacao) < 0) lisLIG->Add(ligacao);
   //verifica se Ligacao não é Chave
   if (ligacao->Tipo() != eqptoCHAVE)
      {
      lisLSC->Add(ligacao);
      lisLSCM->Add(ligacao);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TNet::InsereLisBarra(TList *lisEQP)
   {
   //loop p/ todas Barras da lista
   for (int n = 0; n < lisEQP->Count; n++)
      {
      InsereBarra((VTBarra*)lisEQP->Items[n]);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TNet::InsereLisEqbar(TList *lisEQP)
   {
   //loop p/ todos Eqbar da lista
   for (int n = 0; n < lisEQP->Count; n++)
      {
      InsereEqbar((VTEqbar*)lisEQP->Items[n]);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TNet::InsereLisLigacao(TList *lisEQP)
   {
   //loop p/ todas Ligacoes da lista
   for (int n = 0; n < lisEQP->Count; n++)
      {
      InsereLigacao((VTLigacao*)lisEQP->Items[n]);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TNet::InsereLisLigacaoIsolada(TList *lisEQP)
   {
   //variáveis locais
   VTBarra   *barra;
   VTLigacao *ligacao;

   //salva Ligacoes em isolada.lisLIG e suas Barras em isolada.lisBAR
   for (int n = 0; n < lisEQP->Count; n++)
      {
      ligacao = (VTLigacao*)lisEQP->Items[n];
      //verifica se a Ligacao já existe na lista isolada.lisLIG
      if (isolada.lisLIG->IndexOf(ligacao) < 0)
         {//insere Ligacao na lista isolada.lisLIG
         isolada.lisLIG->Add(ligacao);
         //loop p/ Barras da Ligacao
         for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
            {
            if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
            if (isolada.lisBAR->IndexOf(barra) < 0)
               {//insere Barra na lista isolada.lisBAR
               isolada.lisBAR->Add(barra);
               }
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TNet::InsereLisMutua(TList *lisEQP)
   {
   //loop p/ todas Mutuas da lista
   for (int n = 0; n < lisEQP->Count; n++)
      {
      InsereMutua((VTMutua*)lisEQP->Items[n]);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TNet::InsereMutua(VTArranjo *arranjo, VTLigacao *ligacao)
   {
   //variáveis locais
   VTGrupo *grupo;

   //verifica se existe um Grupo de Mutuas p/ o Arranjo/Trecho indicado
   if ((grupo = ExisteGrupoMutua(arranjo, ligacao)) == NULL)
      {//cria um novo Grupo e insere em lisGRP
		lisGRP->Add(grupo = DLL_NewObjGrupo());
      //IMPORTANTE: define Arranjo do Grupo
      grupo->Arranjo = arranjo;
      }
   //insere Trecho no Grupo
   grupo->InsereLigacao(ligacao);
   //retira Ligacao de lisLSCM
   lisLSCM->Remove(ligacao);
   }

//---------------------------------------------------------------------------
void __fastcall TNet::InsereMutua(VTMutua *mutua)
   {
   //proteção
   if (mutua == NULL) return;
   //insere Mutua somente se não existir em lisMUT
   if (lisMUT->IndexOf(mutua) < 0)
      {//insere Mutua
      lisMUT->Add(mutua);
      //insere Mutua no Grupo da mutuas com trechos em comum
      MontaGrupoDeMutua(mutua);
      //retira Trechos da Mutua de lisLSCM
      lisLSCM->Remove(mutua->ptre1);
      lisLSCM->Remove(mutua->ptre2);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TNet::InsereNo(VTNo *pno)
   {
   //proteção
   if (pno == NULL) return;
   //insere objeto VTNo na lista lisNO
   if (lisNO->IndexOf(pno) < 0) lisNO->Add(pno);
   }

//---------------------------------------------------------------------------
void __fastcall TNet::InsereTramo(VTTramo *tramo)
   {
   //salva Tramo em lisTRM
   if (lisTRM->IndexOf(tramo) < 0)
      {//salva Tramo em lisTRM p/ ser destruído pelo destrutor
      lisTRM->Add(tramo);
      //insere Tramo como Ligacao
      InsereLigacao(tramo);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TNet::LisBarra(void)
   {
   return(lisBAR);
   }

//---------------------------------------------------------------------------
TList* __fastcall TNet::LisBarraIsolada(void)
   {
   return(isolada.lisBAR);
   }

//---------------------------------------------------------------------------
TList* __fastcall TNet::LisEqbar(void)
   {
   return(lisEQB);
   }

//---------------------------------------------------------------------------
void __fastcall TNet::LisEqbar(TList *lisEQP, int tipo)
   {
   //variáveis locais
   VTEqbar *eqbar;

   //insere em lisEQP os objetos VTEqbar do tipo indicado
   for (int n = 0; n < lisEQB->Count; n++)
      {
      eqbar = (VTEqbar*)lisEQB->Items[n];
      if (eqbar->Tipo() == tipo) lisEQP->Add(eqbar);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TNet::LisLigacao(void)
   {
   return(lisLIG);
   }

//---------------------------------------------------------------------------
void __fastcall TNet::LisLigacao(TList *lisEQP, int tipo)
   {
   //variáveis locais
   VTLigacao *pliga;

   //insere em lisEQP os objetos VTLigacao do tipo indicado
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
TList* __fastcall TNet::LisLigacaoIsolada(void)
   {
   return(isolada.lisLIG);
   }

//---------------------------------------------------------------------------
TList* __fastcall TNet::LisLigacaoSemChave(void)
   {
   return(lisLSC);
   }

//---------------------------------------------------------------------------
TList* __fastcall TNet::LisLigacaoSemChaveSemMutua(void)
   {
   return(lisLSCM);
   }

//---------------------------------------------------------------------------
TList* __fastcall TNet::LisGrupoMutua(void)
   {
   return(lisGRP);
   }

//---------------------------------------------------------------------------
TList* __fastcall TNet::LisMutua(void)
   {
   return(lisMUT);
	}

//---------------------------------------------------------------------------
TList* __fastcall TNet::LisNo(void)
   {
   return(lisNO);
   }

//---------------------------------------------------------------------------
void __fastcall TNet::LisZona(TList *lisEQP)
	{
	//variáveis locais
	VTBarra   *barra;
	VTLigacao *ligacao;

	//loop p/ todas Ligacoes
	for (int n = 0; n < lisLIG->Count; n++)
		{
		ligacao = (VTLigacao*)lisLIG->Items[n];
		if ((ligacao->Tipo() == eqptoTRAFO)||(ligacao->Tipo() == eqptoTRAFO3E))
      	{
			for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
				{
				if ((barra = ligacao->Barra(ind_bar)) == NULL) continue;
				//inclui Zona da Barra em lisEQP
				if (lisEQP->IndexOf(barra->zona) < 0) lisEQP->Add(barra->zona);
				}
         }
		}
	}

//---------------------------------------------------------------------------
void __fastcall TNet::MontaGrupoDeMutua(VTMutua *mutua)
   {
   //variáveis locais
   VTGrupo *grupo;

   //verifica se a Mutua deve ser incluida em um Grupo já existente
   if ((grupo = ExisteGrupoMutua(mutua)) == NULL)
      {//cria um novo Grupo e insere em lisGRP
		lisGRP->Add(grupo = DLL_NewObjGrupo());
      }
   //insere Mutua no Grupo
   grupo->InsereMutua(mutua);
   }

//---------------------------------------------------------------------------
int __fastcall TNet::NumGerSup(void)
   {
   //variáveis locais
   VTNo *no;
   int   count = 0;

   //verifica se algum No possui GeracaoPV
   for (int n = 0; n < lisNO->Count; n++)
      {
      no     = (VTNo*)lisNO->Items[n];
      count += no->NumSupGerPV_VF();
      }
   return(count);
   }

//---------------------------------------------------------------------------
int __fastcall TNet::NumGerSupPV(void)
   {
   //variáveis locais
   VTNo *no;
   int   count = 0;

   //verifica se algum No possui GeracaoPV
   for (int n = 0; n < lisNO->Count; n++)
      {
      no     = (VTNo*)lisNO->Items[n];
      count += no->NumSupGerPV();
      }
   return(count);
   }

//---------------------------------------------------------------------------
int __fastcall TNet::NumeroEqpto(TList *lisEQP, int tipo)
   {
   //variáveis locais
   VTEqpto *eqpto;
   int      count = 0;

   //totaliza número de Ligacoes que são Trafo
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      if (eqpto->Tipo() == tipo) count++;
      }
   return(count);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TNet::PM_GetCodigo(void)
   {
   return(PD.codigo);
   }

//---------------------------------------------------------------------------
int __fastcall TNet::PM_GetModeloCarga(void)
   {
   return(PD.modelo_carga);
   }

//---------------------------------------------------------------------------
int __fastcall TNet::PM_GetNumeroCapserie(void)
   {
   return(NumeroEqpto(lisLIG, eqptoCAPSERIE));
   }

//---------------------------------------------------------------------------
int __fastcall TNet::PM_GetNumeroNo(void)
   {
   return(lisNO->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TNet::PM_GetNumeroRegulador(void)
   {
   return(NumeroEqpto(lisLIG, eqptoREGULADOR));
   }

//---------------------------------------------------------------------------
int __fastcall TNet::PM_GetNumeroTrafo(void)
   {
   return(NumeroEqpto(lisLIG, eqptoTRAFO));
   }

//---------------------------------------------------------------------------
int __fastcall TNet::PM_GetNumeroTrafo3E(void)
   {
   return(NumeroEqpto(lisLIG, eqptoTRAFO3E));
   }

//---------------------------------------------------------------------------
int __fastcall TNet::PM_GetNumeroTramo(void)
   {
   return(lisTRM->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TNet::PM_GetNumeroTrechoComMutua(void)
   {
   return(lisLSC->Count - lisLSCM->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TNet::PM_GetNumeroTrechoSemMutua(void)
   {
   //variáveis locais
   //determina número de trechos e subtrais número de trechos c/ mútua
   return(NumeroEqpto(lisLIG, eqptoTRECHO) - NumeroTrechoComMutua);
   }

//---------------------------------------------------------------------------
bool __fastcall TNet::PM_GetRadial(void)
   {
   return(PD.radial);
   }

//---------------------------------------------------------------------------
bool __fastcall TNet::PM_GetRede4Fios(void)
   {
   //variáveis locais
   VTEqpto  *eqpto;
   VTTramo  *tramo;
   VTTrecho *trecho;

   //verifica se há algum Trafo ou Trecho desequilibrado
   for (int n = 0; n < lisLIG->Count; n++)
      {
      eqpto = (VTEqpto*)lisLIG->Items[n];
      if (eqpto->Tipo() == eqptoTRAFO) return(true);
      if (eqpto->Tipo() == eqptoTRECHO)
         {
         trecho = (VTTrecho*)eqpto;
         if (trecho->UtilizaNeutro) return(true);
         }
      else if (eqpto->Tipo() == eqptoTRAMO)
         {
         tramo = (VTTramo*)eqpto;
         if (tramo->UtilizaNeutro) return(true);
         }
		}
	return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TNet::PM_SetCodigo(AnsiString codigo)
   {
   PD.codigo = codigo;
   }

//---------------------------------------------------------------------------
int __fastcall TNet::PM_GetStatusFluxo(void)
   {
   return(PD.status_fluxo);
   }
//---------------------------------------------------------------------------
int __fastcall TNet::PM_GetStatusFluxoNumPatOK(void)
   {
   return(PD.status_fluxo_num_pat_ok);
   }

//---------------------------------------------------------------------------
void __fastcall TNet::PM_SetModeloCarga(int modelo_carga)
   {
   PD.modelo_carga = modelo_carga;
   }

//---------------------------------------------------------------------------
void __fastcall TNet::PM_SetRadial(bool radial)
   {
   PD.radial = radial;
   }

//---------------------------------------------------------------------------
void __fastcall TNet::PM_SetStatusFluxo(int status)
   {
   PD.status_fluxo = status;
   }

//---------------------------------------------------------------------------
void __fastcall TNet::PM_SetStatusFluxoNumPatOK(int num_pat)
   {
   PD.status_fluxo_num_pat_ok = num_pat;
   }

//---------------------------------------------------------------------------
void __fastcall TNet::Reinicia(void)
   {
   //limpa listas sem destruir seus objetos
   lisBAR->Clear();
   lisEQB->Clear();
   lisLIG->Clear();
   lisLSC->Clear();
   lisLSCM->Clear();
   lisMUT->Clear();
   //limpa listas sem destruir seus objetos
   isolada.lisBAR->Clear();
   isolada.lisLIG->Clear();
   //limpa listas destruindo seus objetos
   LimpaTList(lisNO);
   LimpaTList(lisGRP);
   //IMPORTANTE: NÃO limpa lista de Tramos
   //LimpaTList(lisTRM);
   }

//---------------------------------------------------------------------------
void __fastcall TNet::RemoveLigacao(VTLigacao *ligacao)
   {
   lisLIG->Remove(ligacao);
   lisLSC->Remove(ligacao);
   lisLSCM->Remove(ligacao);
   }

//---------------------------------------------------------------------------
void __fastcall TNet::RetiraNo(VTNo *no)
   {
   //variáveis locais
   VTBarra *barra;
   TList   *lisBAR = no->lisBAR;

   //cancela ponteiro das Barras p/ o No
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      barra->RemoveNo(no);
      }
   //retira No de lisNO
   lisNO->Remove(no);
   //destroi objeto
   delete no;
   }

//---------------------------------------------------------------------------
int __fastcall TNet::Tipo(void)
   {
   return(eqptoNET);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TNet::TipoAsString(void)
   {
   return("Net");
   }

//---------------------------------------------------------------------------
bool __fastcall TNet::ValidaFasesTrafo(void)
   {
   //variáveis locais
   VTNo *no;

   for (int n = 0; n < lisNO->Count; n++)
      {
      no = (VTNo*)lisNO->Items[n];
      if (! no->ValidaFasesTrafo()) return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

