//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TMRede.h"
#include "VTArea.h"
#include "VTCorte.h"
#include "VTCluster.h"
#include "VTRede.h"
#include "VTReducao.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMRede
//---------------------------------------------------------------------------
VTMRede* __fastcall NewObjMRede(void)
   {
   return(new TMRede());
   }

//---------------------------------------------------------------------------
__fastcall TMRede::TMRede(void)
   {
   //pbar_ini = NULL;
   //cria objetos
   lisREDE  = new TList();
   lisREDU  = new TList();
   lisCORTE = new TList();
   lisTMP   = new TList();
   //inicia dados
   Malha     = false;
   Pai       = NULL;
   //Carregada = false;
   }

//---------------------------------------------------------------------------
__fastcall TMRede::~TMRede(void)
   {
   //destrói lista e seus objetos
   if (lisREDU)  {LimpaTList(lisREDU); delete lisREDU;  lisREDU  = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisREDE)  {delete lisREDE;  lisREDE  = NULL;}
   if (lisCORTE) {delete lisCORTE; lisCORTE = NULL;}
   if (lisTMP)   {delete lisTMP;   lisTMP   = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TMRede::AreaEsquematico(int &x1, int &y1, int &x2, int &y2)
   {
   //variáveis locais
   int     count = 0;
   VTArea  *area;
   VTRede  *rede;

   //inicia x1, y1, x2, y2
   x1 = y1 = x2 = y2 = -1;
   //proteção
   if (lisREDE->Count == 0) return(false);
   //loop p/ as todas Redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      area = rede->AreaEsquematico();
      //proteção
      if ((area->x1 < 0)||(area->y1 < 0)) continue;
      if (count == 0)
         {
         x1 = area->x1;
         y1 = area->y1;
         x2 = area->x2;
         y2 = area->y2;
         }
      else
         {
         x1 = Min(x1, int(area->x1));
         y1 = Min(y1, int(area->y1));
         x2 = Max(x2, int(area->x2));
         y2 = Max(y2, int(area->y2));
         }
      count++;
      }
   return(count > 0);
   }

//---------------------------------------------------------------------------
bool __fastcall TMRede::AreaUtm_cm(int &x1, int &y1, int &x2, int &y2)
   {
   //variáveis locais
   int     count = 0;
   VTArea  *area;
   VTRede  *rede;

   //inicia x1, y1, x2, y2
   x1 = y1 = x2 = y2 = -1;
   //proteção
   if (lisREDE->Count == 0) return(false);
   //loop p/ todas Redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      area = rede->AreaUtm_cm();
      //proteção
      if ((area->x1 < 0)||(area->y1 < 0)) continue;
      if (count == 0)
         {
         x1 = area->x1;
         y1 = area->y1;
         x2 = area->x2;
         y2 = area->y2;
         }
      else
         {
         x1 = Min(x1, int(area->x1));
         y1 = Min(y1, int(area->y1));
         x2 = Max(x2, int(area->x2));
         y2 = Max(y2, int(area->y2));
         }
      count++;
      }
   return(count > 0);
   }

//---------------------------------------------------------------------------
VTMRede* __fastcall TMRede::Clone(void)
   {
   //variáveis locais
   TMRede *mrede;

   //cria objeto VTMRede
   if ((mrede = (TMRede*)NewObjMRede()) != NULL)
      {//copia dados deste MRede
      mrede->CopiaAtributosDe(*this);
      }
   return(mrede);
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::CopiaAtributosDe(VTEqpto &ref)
   {
   //variáveis locais
   TList     *lisEXT;
   VTReducao *reducao;
   TMRede    &mrede = (TMRede&)ref;

   //proteção
   if (ref.Tipo() != eqptoMREDE) return;
   //copia atributos de TEqpto
   VTEqpto::CopiaAtributosDe(mrede);
   //copia atributos de TMRede
   PD = mrede.PD;
   //copia lista lisCORTE e lisREDE
   lisREDE->Clear();  CopiaTList(mrede.LisRede(), lisREDE);
   lisCORTE->Clear(); CopiaTList(mrede.LisCorte(), lisCORTE);
   //destrói Reducoes pré-existentes
   LimpaTList(lisREDU);
   //copia Reducoes
   lisEXT = mrede.LisReducao();
   for (int nr = 0; nr < lisEXT->Count; nr++)
      {
      reducao = (VTReducao*)lisEXT->Items[nr];
      reducao = reducao->Clone();
      reducao->MRede = this;
      lisREDU->Add(reducao);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TMRede::ExisteCorte(VTCorte *corte)
   {
   return(lisCORTE->IndexOf(corte) >= 0);
   }

//---------------------------------------------------------------------------
VTCorte* __fastcall TMRede::ExisteCorte(VTMRede *mrede)
   {
   //variáveis locais
   VTCorte *corte;
   TList   *lisEXT;

   //obtém lista de Cortes da MRede indicada
   lisEXT = mrede->LisCorte();
   //verifica se existe algum Corte comum c/ a MRede indicada
   for (int n = 0; n < lisCORTE->Count; n++)
      {
      corte = (VTCorte*)lisCORTE->Items[n];
      if (lisEXT->IndexOf(corte) >= 0) return(corte);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TMRede::ExisteRede(VTRede *rede)
   {
   return(lisREDE->IndexOf(rede) >= 0);
   }

//---------------------------------------------------------------------------
VTReducao* __fastcall TMRede::ExisteReducao(VTCorte *corte)
   {
   //variáveis locais
   VTReducao *reducao;

   //analisa o Corte de cada Reducao
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      reducao = (VTReducao*)lisREDU->Items[nr];
      if (reducao->Corte == corte) return(reducao);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
VTReducao* __fastcall TMRede::ExisteReducaoMRedeNaoCarregada(void)
   {
   //variáveis locais
   VTReducao *reducao = NULL;

   //verifica se MRede Não carregada
   if (! Carregada)
      {
      if (lisREDU->Count == 1) reducao = (VTReducao*)lisREDU->First();
      }
   return(reducao);
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::IncorporaMRede(TList *lisMR)
   {
   //variáveis locais
   VTMRede *mrede;

   //loop p/ todos objetos MRede em lisMR
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      //incorpora MRede
      IncorporaMRede(mrede);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::IncorporaMRede(VTMRede *mrede)
   {
   //variáveis locais
   VTRede    *rede;
   VTReducao *reducao;
   struct   {
            TList *lisREDE;
            TList *lisREDU;
            } ext;

   //insere as Redes da MRede indicada
   ext.lisREDE = mrede->LisRede();
   for (int n = 0; n < ext.lisREDE->Count; n++)
      {
      rede = (VTRede*)ext.lisREDE->Items[n];
      InsereRede(rede);
      }
   //incorpora as Reducoes da MRede indicada
   ext.lisREDU = mrede->LisReducao();
   for (int nr = 0; nr < ext.lisREDU->Count; nr++)
      {
      reducao = (VTReducao*)ext.lisREDU->Items[nr];
      InsereReducao(reducao, reducao->Corte);
      }
   //limpa lista
   ext.lisREDU->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::IniciaLisCorte(void)
   {
   //variáveis locais
   VTCorte *corte;
   VTRede  *rede, *rede_viz;
   struct {TList *lisCOR, *lisREDE;} tmp;

   //reinicia lista lisCOR
   lisCORTE->Clear();
   //verifica se há apenas uma Rede em lisREDE
   if (lisREDE->Count == 1)
      {//os Cortes da MRede são os próprios Cortes da Rede
      rede = (VTRede*)lisREDE->First();
      tmp.lisCOR = rede->LisCorte();
      for (int nc = 0; nc < tmp.lisCOR->Count; nc++)
         {
         corte = (VTCorte*)tmp.lisCOR->Items[nc];
         InsereCorte(corte);
         }
      return;
      }
   //loop p/ todas as Redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //obtém lista de Cortes da Rede
      tmp.lisCOR = rede->LisCorte();
      //loop p/ todos Cortes da Rede
      for (int nc = 0; nc < tmp.lisCOR->Count; nc++)
         {
         corte = (VTCorte*)tmp.lisCOR->Items[nc];
         //verifica se o Corte já existe em lisCORTE
         if (lisCORTE->IndexOf(corte) >= 0) continue;
         //obtém lista de Redes do Corte
         tmp.lisREDE = corte->LisRede();
         //verifica se todas Rede do Corte pertencem a esta MRede
         if (! Lista1ContemLista2(lisREDE, tmp.lisREDE))
            {//Corte possui Rede que pertence à outra MTRede: insere Corte
            InsereCorte(corte);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::InsereCorte(VTCorte *corte)
   {
   //proteção
   if (corte == NULL) return;
   //verifica se o Corte já existe em lisCORTE
   if (lisCORTE->IndexOf(corte) < 0)
      {//insere Corte em lisCORTE
      lisCORTE->Add(corte);
      //insere este MRede no Corte
      //corte->InsereMRede(this);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::InsereRede(VTRede *rede)
   {
   //salva Rede na lista
   if (lisREDE->IndexOf(rede) < 0) lisREDE->Add(rede);
   //redefine MRede da Rede
   rede->MRede = this;
   //verifica se a Rede possui Cortes
   if ((rede->LisCorte())->Count > 0)
      {//reinicia lista de Cortes
      IniciaLisCorte();
      }
   //define Barra inicial
   //DefineBarraInicial(rede->BarraInicial());
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::InsereRede(TList *lisEQP)
   {
   //variáveis locais
   VTRede *rede;

   //loop p/ todas as Redes
   for (int n = 0; n < lisEQP->Count; n++)
      {
      rede = (VTRede*)lisEQP->Items[n];
      InsereRede(rede);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::InsereReducao(VTReducao *reducao, VTCorte *corte)
   {
   //variáveis locais
   VTRede *rede;

   //proteção
   if ((reducao == NULL)||(corte == NULL)) return;
   //insere Corte
   InsereCorte(corte);
   //insere reducao em lisREDU
   if (lisREDU->IndexOf(reducao) < 0) lisREDU->Add(reducao);
   //associa Reducao c/ esta MRede e c/ Corte
   reducao->MRede = this;
   //associa Reducao c/ Corte
   reducao->Corte = corte;
//JCG 2008.05.09 - dúvida: bloquear inserção das Redes no Corte
/*
   //insere Redes no Corte
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      corte->InsereRede(rede);
      }
*/
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::LisBarra(TList *lisEQP, bool barra_isolada)
   {
   //variáveis locais
   VTRede    *rede;
   VTReducao *reducao;

   //verifica se é uma MRede carregada
   if (Carregada)
      {//loop p/ todas as Redes Carregadas
      for (int n = 0; n < lisREDE->Count; n++)
         {
         rede = (VTRede*)lisREDE->Items[n];
         //monta lista de Barras, com a opção indicada de Barras isoladas
         rede->LisBarra(lisEQP, barra_isolada);
         //monta lista de Barras sem as Barras conectadas por chaves abertas
         //rede->LisBarra(lisEQP, false);
         }
      }
   else
      {//loop p/ todas as Reducoes de redes NÃO carregadas
      for (int n = 0; n < lisREDU->Count; n++)
         {
         reducao = (VTReducao*)lisREDU->Items[n];
         reducao->LisBarra(lisEQP);
         }
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TMRede::LisCorte(void)
   {
   return(lisCORTE);
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::LisCorte(TList *lisEQP)
   {
   //variáveis locais
   VTCorte *corte;

   //loop p/ todos Cortes
   for (int n = 0; n < lisCORTE->Count; n++)
      {
      corte = (VTCorte*)lisCORTE->Items[n];
      if (lisEQP->IndexOf(corte) < 0) lisEQP->Add(corte);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::LisEqbar(TList *lisEQP, int tipo, bool barra_isolada)
   {
   //variáveis locais
   VTRede    *rede;
   VTReducao *reducao;

   //verifica se é uma MRede carregada
   if (Carregada)
      {//loop p/ todas as Redes Carregadas
      for (int n = 0; n < lisREDE->Count; n++)
         {
         rede = (VTRede*)lisREDE->Items[n];
         rede->LisEqbar(lisEQP, tipo, barra_isolada);
         }
      }
   else
      {//loop p/ todas as Reducoes de redes NÃO carregadas
      for (int n = 0; n < lisREDU->Count; n++)
         {
         reducao = (VTReducao*)lisREDU->Items[n];
         reducao->LisEqbar(lisEQP, tipo);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::LisEqpto(TList *lisEQP, int tipo, bool barra_isolada)
   {
   //verifica tipo de eqpto solicitado
   if      (tipo == eqptoBARRA)   LisBarra(lisEQP, barra_isolada);
   else if (tipo == eqptoEQBAR)   LisEqbar(lisEQP, barra_isolada);
   else if (tipo == eqptoLIGACAO) LisLigacao(lisEQP);
   else if (tipo == eqptoMUTUA)   LisMutua(lisEQP);
   else if (tipo == -1)
      {//todos os equipamentos
      LisBarra(lisEQP, barra_isolada);
      LisEqbar(lisEQP, barra_isolada);
      LisLigacao(lisEQP);
      LisMutua(lisEQP);
      }
   else
      {//um tipo de equipamento específico (Eqbar ou Ligacao)
      LisEqbar(lisEQP, tipo, barra_isolada);
      LisLigacao(lisEQP, tipo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::LisLigacao(TList *lisEQP, int tipo)
   {
   //variáveis locais
   VTRede    *rede;
   VTReducao *reducao;

   //verifica se é uma MRede carregada
   if (Carregada)
      {//loop p/ todas as Redes Carregadas
      for (int n = 0; n < lisREDE->Count; n++)
         {
         rede = (VTRede*)lisREDE->Items[n];
         rede->LisLigacao(lisEQP, tipo);
         }
      }
   else
      {//loop p/ todas as Reducoes
      for (int n = 0; n < lisREDU->Count; n++)
         {
         reducao = (VTReducao*)lisREDU->Items[n];
         reducao->LisLigacao(lisEQP, tipo);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::LisMRedeVizinha(TList *lisMRV)
   {
   //variáveis locais
   VTCorte *corte;
   VTMRede *mrede;

   //loop p/ todos Cortes
   for (int nc = 0; nc < lisCORTE->Count; nc++)
      {
      corte = (VTCorte*)lisCORTE->Items[nc];
      //obtém lista de MRedes vizinhas neste Corte
      LisMRedeVizinha(corte, lisMRV);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::LisMRedeVizinha(VTCorte *corte, TList *lisMRV)
   {
   //variáveis locais
   VTMRede *mrede;
   VTRede  *rede;
   TList   *lisREDE;

   //obtém lista de Redes do Corte
   lisREDE = corte->LisRede();
   //insere em lisMRV as MRedes das Redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if ((mrede = rede->MRede) == this) continue;
      //insere MRede em lisMRV
      if (lisMRV->IndexOf(mrede) < 0) lisMRV->Add(mrede);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::LisMRedeVizinhaCarregada(VTCorte *corte, TList *lisMRV)
   {
   //variáveis locais
   VTMRede *mrede;
   VTRede  *rede;
   TList   *lisREDE;

   //obtém lista de Redes do Corte
   lisREDE = corte->LisRede();
   //insere em lisMRV as MRedes das Redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if ((mrede = rede->MRede) == this) continue;
      //verifica se MRede carregada
      if (! mrede->Carregada) continue;
      //insere MRede em lisMRV
      if (lisMRV->IndexOf(mrede) < 0) lisMRV->Add(mrede);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::LisMutua(TList *lisEQP)
   {
   //variáveis locais
   VTRede *rede;

   //verifica se é uma MRede carregada
   if (! Carregada) return;
   //loop p/ todas as Redes Carregadas
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      rede->LisMutua(lisEQP);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TMRede::LisRede(void)
   {
   return(lisREDE);
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::LisRede(TList *lisEQP)
   {
   //variáveis locais
   VTRede *rede;

   //loop p/ todas as Redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (lisEQP->IndexOf(rede) < 0) lisEQP->Add(rede);
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TMRede::LisReducao(void)
   {
   return(lisREDU);
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::LisReducao(TList *lisEQP)
   {
   //variaáveis locais
   VTReducao *reducao;

   for (int n = 0; n < lisREDU->Count; n++)
      {
      reducao = (VTReducao*)lisREDU->Items[n];
      if (lisEQP->IndexOf(reducao) < 0) lisEQP->Add(reducao);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::LisReducaoVizinha(TList *lisEQP)
   {
   //variáveis locais
   VTCorte   *corte;
   VTMRede   *mrede;
   VTReducao *reducao;

   //loop p/ todos Cortes de MRede
   for (int nc = 0; nc < lisCORTE->Count; nc++)
      {
      corte = (VTCorte*)lisCORTE->Items[nc];
      //obtém lista de MRedes vizinhas
      lisTMP->Clear();
      LisMRedeVizinha(lisTMP);
      for (int nr = 0; nr < lisTMP->Count; nr++)
         {
         mrede = (VTMRede*)lisTMP->Items[nr];
         if ((reducao = mrede->ExisteReducao(corte)) == NULL) continue;
         //insere Reducao em lisEQP
         if (lisEQP->IndexOf(reducao) < 0) lisEQP->Add(reducao);
         }
      }
   }

//---------------------------------------------------------------------------
int __fastcall TMRede::NumeroDeCortes(void)
   {
   return(lisCORTE->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TMRede::NumeroDeGerSup(void)
   {
   //variáveis locais
   int       count = 0;
   VTRede    *rede;
   VTReducao *reducao;

   //verifica se é uma MRede carregada
   if (Carregada)
      {//loop p/ todas as Redes Carregadas
      for (int n = 0; n < lisREDE->Count; n++)
         {
         rede = (VTRede*)lisREDE->Items[n];
         count += rede->NumeroDeEqbar(eqptoGERADOR);
         count += rede->NumeroDeEqbar(eqptoSUPRIMENTO);
         }
      }
   else
      {//loop p/ todas as Reducoes de redes NÃO carregadas
      for (int n = 0; n < lisREDU->Count; n++)
         {
         reducao = (VTReducao*)lisREDU->Items[n];
         count  += reducao->NumeroDeEqbar(eqptoGERADOR);
         count  += reducao->NumeroDeEqbar(eqptoSUPRIMENTO);
         }
      }
   return(count);
   }

//---------------------------------------------------------------------------
int __fastcall TMRede::NumeroDeRedes(void)
   {
   return(lisREDE->Count);
   }
   
/*
//---------------------------------------------------------------------------
void __fastcall TMRede::PM_SetSimples(bool simples)
   {
   //variáveis locais
   VTRede *rede;

   PD.simples = simples;
   //classifica todas Redes como simples
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      rede->Simples(simples);
      }
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TMRede::PM_GetCarregada(void)
   {
   return(PD.carregada);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMRede::PM_GetCodigo(void)
   {
   //variáveis locais
   VTRede     *rede;
   AnsiString codigo = "rede";

   if (lisREDE->Count > 0)
      {//determina primeira Rede
      rede = (VTRede*)lisREDE->First();
      codigo = rede->Codigo;
      //verifica se a Rede pertence a um CLuster
      if (rede->Cluster) codigo = rede->Cluster->Codigo;
      }
   return(codigo);
   }

//---------------------------------------------------------------------------
TColor __fastcall TMRede::PM_GetColor(void)
   {
   //variáveis locais
   VTRede *rede;

   //proteção
   if (lisREDE->Count == 0) return(clWhite);
   //assume a cor da primeira Rede
   rede = (VTRede*)lisREDE->First();
   return(rede->Color);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TMRede::PM_GetGeracao(void)
   {
   //variáveis locais
   VTRede    *rede;
   VTReducao *reducao;

   //verifica se alguma Rede possui geração
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (rede->ExisteGeracao()) return(true);
      }
   //verifica se alguma Reducao possui geracao
   for (int n = 0; n < lisREDU->Count; n++)
      {
      reducao = (VTReducao*)lisREDU->Items[n];
      if (reducao->ExisteGeracao()) return(true);
      }
   return(false);
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TMRede::PM_GetGeracao(void)
   {
   //variáveis locais
   VTRede    *rede;
   VTReducao *reducao;

   //verifica se é uma MRede carregada
   if (Carregada)
      {//verifica se alguma Rede possui geração
      for (int n = 0; n < lisREDE->Count; n++)
         {
         rede = (VTRede*)lisREDE->Items[n];
         if (rede->ExisteGeracao()) return(true);
         }
      }
   else
      {//verifica se alguma Reducao possui geracao
      for (int n = 0; n < lisREDU->Count; n++)
         {
         reducao = (VTReducao*)lisREDU->Items[n];
         if (reducao->ExisteGeracao()) return(true);
         }
      }
   return(false);
   }
/*
//---------------------------------------------------------------------------
int __fastcall TMRede::PM_GetId(void)
   {
   //variáveis locais
   VTRede *rede;

   //verifica se o Id é negativo
   if (VTEqpto::PD.id < 0)
      {//redefine Id igual ao menor ID positivo das Redes
      for (int n = 0; n < lisREDE->Count; n++)
         {
         rede = (VTRede*)lisREDE->Items[n];
         if (rede->Id > 0)
            {
            if      (VTEqpto::PD.id < 0)        VTEqpto::PD.id = rede->Id;
            else if (rede->Id < VTEqpto::PD.id) VTEqpto::PD.id = rede->Id;
            }
         }
      }
   //atualiza
   return(VTEqpto::PD.id);
   }
*/
//---------------------------------------------------------------------------
int __fastcall TMRede::PM_GetId(void)
   {
   //variáveis locais
   VTRede *rede;

   //retorna menor ID entre as redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //determina menor ID
      if      (n == 0)                    {VTEqpto::PD.id = rede->Id;}
      else if (rede->Id < VTEqpto::PD.id) {VTEqpto::PD.id = rede->Id;}
      }
   return(VTEqpto::PD.id);
   }

//---------------------------------------------------------------------------
bool __fastcall TMRede::PM_GetMalha(void)
   {
   return(PD.malha);
   }

//---------------------------------------------------------------------------
VTMNet* __fastcall TMRede::PM_GetMNet(void)
   {
   return(PD.mnet);
   }

//---------------------------------------------------------------------------
VTMRede* __fastcall TMRede::PM_GetPai(void)
   {
   return(PD.pai);
   }

//---------------------------------------------------------------------------
bool __fastcall TMRede::PM_GetRedutivel(void)
   {
   return(PD.redutivel);
   }

//---------------------------------------------------------------------------
TDate __fastcall TMRede::PM_GetRevisao(void)
   {
   return(PD.revisao);
   }

//---------------------------------------------------------------------------
bool __fastcall TMRede::PM_GetValida(void)
   {
   //variáveis locais
   VTRede *rede;

   //verifica se todas Redes são válidas 
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (! rede->Valida) return(false);
      }
   return(true);

   }

//---------------------------------------------------------------------------
void __fastcall TMRede::PM_SetCarregada(bool carregada)
   {
   //variáveis locais
   VTRede *rede;

   //atualiza estado
   PD.carregada = carregada;
   //atualiza estado de todas Redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      rede->Carregada = carregada;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::PM_SetMalha(bool malha)
   {
   PD.malha = malha;
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::PM_SetMNet(VTMNet *mnet)
   {
   PD.mnet = mnet;
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::PM_SetPai(VTMRede *pai)
   {
   PD.pai = pai;
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::PM_SetRedutivel(bool redutivel)
   {
   PD.redutivel = redutivel;
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::PM_SetRevisao(TDate revisao)
   {
   PD.revisao = revisao;
   }

//---------------------------------------------------------------------------
void __fastcall TMRede::ReiniciaReducao(void)
   {
   //variáveis locais
   VTReducao *reducao;

   //loop p/ todas Reducoes
   for (int n = 0; n < lisREDU->Count; n++)
      {
      reducao = (VTReducao*)lisREDU->Items[n];
      //destrói Reducao (OBS: ao ser destruída, a Reducao retira a MRede do Corte)
      delete reducao;
      }
   //limpa lista
   lisREDU->Clear();
   }


//---------------------------------------------------------------------------
void __fastcall TMRede::RemoveCorte(VTCorte *corte)
   {
   //proteção
   if (corte == NULL) return;
   //retira Corte de lisCORTE
   lisCORTE->Remove(corte);
   }
   
/*
//---------------------------------------------------------------------------
void __fastcall TMRede::RetiraReducao(VTReducao *reducao)
   {
   //proteção
   if (reducao == NULL) return;
   //ao ser destruída a Reducao retira a MRede do Corte
   //corte = reducao->Corte();
   //corte->RetiraRede(this);
   //retira Reducao da lista
   lisREDU->Remove(reducao);
   //destrói objeto
   delete reducao;
   }
*/

//---------------------------------------------------------------------------
int __fastcall TMRede::Tipo(void)
   {
   return(eqptoMREDE);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TMRede::TipoAsString(void)
   {
   return("MetaRede");
   }

//---------------------------------------------------------------------------
//eof

