//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Algorithm>
#include "TCelulas.h"
#include "VTCelula.h"
#include "VTLayer.h"
#include "..\Apl\VTApl.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\Redegraf\VTCoordenada.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCelulas* NewObjCelulas(VTApl *apl)
   {
   try{
      return(new TCelulas(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }
   
//---------------------------------------------------------------------------
__fastcall TCelulas::TCelulas(VTApl *apl)
   {
   try{//salva ponteiro p/ objeto
      this->apl = apl;
      //cria objetos
      lisATIVA  = new TList();
      lisCELULA = new TList();
      lisEQP    = new TList();
      lisTMP    = new TList();
      //habilita mapeamento dos Eqptos nas Celulas
      EqptoEnabled = true;
      } catch (Exception &e)
         {//nada fazer
         }
   }

//---------------------------------------------------------------------------
// TCelulas::~TCelulas() - destrutor
//---------------------------------------------------------------------------
__fastcall TCelulas::~TCelulas(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisEQP)   {delete lisEQP;   lisEQP   = NULL;}
   if (lisTMP)   {delete lisTMP;   lisTMP   = NULL;}
   if (lisATIVA) {delete lisATIVA; lisATIVA = NULL;}
   //destrói lista e seus objetos
   if (lisCELULA) {LimpaTList(lisCELULA); delete lisCELULA; lisCELULA = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall  TCelulas::DefineAreaTotal(void)
   {
   //verifica se bloqueado
   if (! EqptoEnabled) return;
   //reinicia celulas
   ReiniciaCelulas();
   }

//---------------------------------------------------------------------------
void __fastcall  TCelulas::DefineAreaAtiva(VTArea *area)
   {
   //variáveis locais
   VTCelula *celula;

   //reinicia lista de Celulas ativas
   lisATIVA->Clear();
   for (int n = 0; n < lisCELULA->Count; n++)
      {
      celula = (VTCelula*)lisCELULA->Items[n];
      //verifica se a área da Celula está na área ativa
      if (celula->Area->x1 > area->x2) continue;
      if (celula->Area->x2 < area->x1) continue;
      if (celula->Area->y1 > area->y2) continue;
      if (celula->Area->y2 < area->y1) continue;
      //insere Celula em lisATIVA
      lisATIVA->Add(celula);
      }
   }

//---------------------------------------------------------------------------
void __fastcall  TCelulas::DefineGrid(int num_lin, int num_col)
   {
   //recria as Celulas
   ReiniciaCelulas(num_lin, num_col);
   }

//---------------------------------------------------------------------------
void __fastcall  TCelulas::DefineGridQuadrado(int dim_cel_m)
   {
   ReiniciaCelulas(-1, -1, dim_cel_m);
   }

//---------------------------------------------------------------------------
int __fastcall TCelulas::Distancia(VTCelula *celula1, VTCelula *celula2)
   {
   //variáveis locais
   int  dist = 0;
   int  lin1, col1, lin2, col2, dlin,  dcol;

   //determina posicionamento da célula1 no grid
   if (IndicesDaCelula(celula1, lin1, col1))
      {//determina posicionamento da célula2 no grid
      if (IndicesDaCelula(celula2, lin2, col2))
      //determina distância entre célula de referência e célula vizinha
      dlin = abs(lin1 - lin2);
      dcol = abs(col1 - col2);
      //dist = (dlin > dcol) ? dlin : dcol;
      dist = dlin + dcol;
      }
   return(dist);
   }

//---------------------------------------------------------------------------
VTCelula* __fastcall TCelulas::ExisteCelula(int x, int y)
   {
   //variáveis locais
   int i, j, index;

   //determina (i1,j1) da Celula p/ coordenadas (x1, y1)
   i = (y - grid.y1) / grid.dy;
   j = (x - grid.x1) / grid.dx;
   //proteção
   if ((i < 0)||(i >= grid.num_lin)) return(NULL);
   if ((j < 0)||(j >= grid.num_col)) return(NULL);
   //determina index da Celula na lista
   index = (grid.num_col * i) + j;
   //insere Celula em lisCEL
   return((VTCelula*)lisCELULA->Items[index]);
   }

//---------------------------------------------------------------------------
bool __fastcall TCelulas::IndicesDaCelula(VTCelula *celula, int &linha, int &coluna)
   {
   //veriáveis locais
   int index;

   //proteção
   if (celula == NULL) return(false);
   //determina indices da celula no grid
   index  = lisCELULA->IndexOf(celula);
   linha  = index / grid.num_col;
   coluna = index % grid.num_col;
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall  TCelulas::InsereBarrasRede(void)
   {
   //variáveis locais
   TList        *lisBARRA;
   VTCelula     *celula;
   VTBarra      *barra;
   VTRede       *rede;
   VTCoordenada *coordenada;
   VTRedegraf   *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   VTRedes      *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList        *lisREDE  = redes->LisRede();

   //loop p/ todas as Redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if (! rede->Carregada) continue;
      //loop p/ todas Barras da Rede, incluindo Barras de chaves abertas
      //lisBARRA = rede->LisBarra();
      lisEQP->Clear();
      rede->LisBarra(lisEQP, true);
      for (int nb = 0; nb < lisEQP->Count; nb++)
         {
         barra = (VTBarra*)lisEQP->Items[nb];
         //determina coordenadas
         coordenada = redegraf->CoordBarra[barra];
         if ((coordenada == NULL)||(coordenada->Line == NULL)) continue;
         //determina lista de Celulas que contém a Barra
         lisTMP->Clear();
         SelecionaCelula(coordenada->Line->x1,coordenada->Line->y1, coordenada->Line->x2, coordenada->Line->y2, lisTMP);
         //insere Barra nas Celulas selecionadas
         for (int nc = 0; nc < lisTMP->Count; nc++)
            {
            celula = (VTCelula*)lisTMP->Items[nc];
            celula->InsereEqptoRede(rede, layerREDE, barra);
            }
         }
      }
   //loop p/ todas Barras isoladas
   lisBARRA = redes->LisBarraIsolada();
   for (int nb = 0; nb < lisBARRA->Count; nb++)
      {
      barra = (VTBarra*)lisBARRA->Items[nb];
      //determina coordenadas
      coordenada = redegraf->CoordBarra[barra];
      if ((coordenada == NULL)||(coordenada->Line == NULL)) continue;
      //determina lista de Celulas que contém a Barra
      lisTMP->Clear();
      SelecionaCelula(coordenada->Line->x1,coordenada->Line->y1, coordenada->Line->x2, coordenada->Line->y2, lisTMP);
      //insere Barra nas Celulas selecionadas
      for (int nc = 0; nc < lisTMP->Count; nc++)
         {
         celula = (VTCelula*)lisTMP->Items[nc];
         celula->InsereEqptoRede(redes->RedeIsolada(), layerREDE, barra);
         }
      }
   }


//---------------------------------------------------------------------------
void __fastcall  TCelulas::InsereBarrasReducao(void)
   {
   //variáveis locais
   TList        *lisBARRA;
   TList        *lisREDUCAO;
   VTCelula     *celula;
   VTBarra      *barra;
   VTMRede      *mrede;
   VTReducao    *reducao;
   VTCoordenada *coordenada;
   VTRedegraf   *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   VTRedes      *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList        *lisMR    = redes->LisMRede();

   //loop p/ todas MRedes
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->Carregada) continue;
      //OBS: uma MRede não carregada possui uma única Reducao
      lisREDUCAO = mrede->LisReducao();
      if (lisREDUCAO->Count == 0) continue;
      reducao = (VTReducao*)(lisREDUCAO->First());
      //loop p/ todas Barras da Reducao
      lisBARRA = reducao->LisBarra();
      for (int nb = 0; nb < lisBARRA->Count; nb++)
         {
         barra = (VTBarra*)lisBARRA->Items[nb];
         //determina coordenadas
         coordenada = redegraf->CoordBarra[barra];
         if ((coordenada == NULL)||(coordenada->Line == NULL)) continue;
         //determina lista de Celulas que contém a Barra
         lisTMP->Clear();
         SelecionaCelula(coordenada->Line->x1,coordenada->Line->y1, coordenada->Line->x2, coordenada->Line->y2, lisTMP);
         //insere Barra nas Celulas selecionadas
         for (int nc = 0; nc < lisTMP->Count; nc++)
            {
            celula = (VTCelula*)lisTMP->Items[nc];
            celula->InsereEqptoReducao(reducao, layerREDU_EQPTO, barra);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall  TCelulas::InsereEqbarRede(void)
   {
   //variáveis locais
   TList        *lisBARRA;
   VTBarra      *barra;
   VTCelula     *celula;
   VTEqbar      *eqbar;
   VTRede       *rede;
   VTCoordenada *coordenada;
   VTRedegraf   *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   VTRedes      *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList        *lisREDE  = redes->LisRede();

   //loop p/ todas as Redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if (! rede->Carregada) continue;
      //loop p/ todos Eqbar da Rede
      lisEQP->Clear();
      rede->LisEqbar(lisEQP);
      //loop p/ todos Eqbar
      for (int nb = 0; nb < lisEQP->Count; nb++)
         {
         eqbar = (VTEqbar*)lisEQP->Items[nb];
         //determina coordenadas
         coordenada = redegraf->CoordEqbar[eqbar];
         if ((coordenada == NULL)||(coordenada->Line == NULL)) continue;
         //determina lista de Celulas que contém Eqbar
         lisTMP->Clear();
         SelecionaCelula(coordenada->Line->x1,coordenada->Line->y1, coordenada->Line->x2, coordenada->Line->y2, lisTMP);
         //insere Barra nas Celulas selecionadas
         for (int nc = 0; nc < lisTMP->Count; nc++)
            {
            celula = (VTCelula*)lisTMP->Items[nc];
            celula->InsereEqptoRede(rede, layerREDE, eqbar);
            }
         }
      }
   //determina Eqbar das  Barras isoladas
   lisBARRA = redes->LisBarraIsolada();
   lisEQP->Clear();
   for (int n = 0; n < lisBARRA->Count; n++)
      {
      barra = (VTBarra*)lisBARRA->Items[n];
      barra->LisEqbar(lisEQP);
      }
   //loop p/ todos Eqbar
   for (int nb = 0; nb < lisEQP->Count; nb++)
      {
      eqbar = (VTEqbar*)lisEQP->Items[nb];
      //determina coordenadas
      coordenada = redegraf->CoordEqbar[eqbar];
      if ((coordenada == NULL)||(coordenada->Line == NULL)) continue;
      //determina lista de Celulas que contém Eqbar
      lisTMP->Clear();
      SelecionaCelula(coordenada->Line->x1,coordenada->Line->y1, coordenada->Line->x2, coordenada->Line->y2, lisTMP);
      //insere Barra nas Celulas selecionadas
      for (int nc = 0; nc < lisTMP->Count; nc++)
         {
         celula = (VTCelula*)lisTMP->Items[nc];
         celula->InsereEqptoRede(redes->RedeIsolada(), layerREDE, eqbar);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall  TCelulas::InsereEqbarReducao(void)
   {
   //variáveis locais
   TList        *lisEQBAR;
   TList        *lisREDUCAO;
   VTCelula     *celula;
   VTEqbar      *eqbar;
   VTMRede      *mrede;
   VTReducao    *reducao;
   VTCoordenada *coordenada;
   VTRedegraf   *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   VTRedes      *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList        *lisMR    = redes->LisMRede();

   //loop p/ todas MRedes
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->Carregada) continue;
      //OBS: uma MRede não carregada possui uma única Reducao
      lisREDUCAO = mrede->LisReducao();
      if (lisREDUCAO->Count == 0) continue;
      reducao = (VTReducao*)(lisREDUCAO->First());
      //loop p/ todos Eqbar da Reducao
      lisEQBAR = reducao->LisEqbar();
      for (int n = 0; n < lisEQBAR->Count; n++)
         {
         eqbar = (VTEqbar*)lisEQBAR->Items[n];
         //determina coordenadas
         coordenada = redegraf->CoordEqbar[eqbar];
         if ((coordenada == NULL)||(coordenada->Line == NULL)) continue;
         //determina lista de Celulas que contém a Barra
         lisTMP->Clear();
         SelecionaCelula(coordenada->Line->x1,coordenada->Line->y1, coordenada->Line->x2, coordenada->Line->y2, lisTMP);
         //insere Eqbar nas Celulas selecionadas
         for (int nc = 0; nc < lisTMP->Count; nc++)
            {
            celula = (VTCelula*)lisTMP->Items[nc];
            celula->InsereEqptoReducao(reducao, layerREDU_EQPTO, eqbar);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall  TCelulas::InsereLigacoesRede(void)
   {
   //variáveis locais
   TList        *lisLIGACAO;
   VTCelula     *celula;
   VTLigacao    *ligacao;
   VTRede       *rede;
   VTCoordenada *coordenada;
   VTRedegraf   *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   VTRedes      *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList        *lisREDE  = redes->LisRede();

   //loop p/ todas as Redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if (! rede->Carregada) continue;
      //loop p/ todas Ligacoes da Rede
      lisLIGACAO = rede->LisLigacao();
      for (int nl = 0; nl < lisLIGACAO->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLIGACAO->Items[nl];
         //determina coordenadas
         coordenada = redegraf->CoordLigacao[ligacao];
         if ((coordenada == NULL)||(coordenada->Line == NULL)) continue;
         //verifica se Ligacao conecta 2 ou 3 barras
         if (ligacao->pbarra3 == NULL)
            {//determina lista de Celulas que contém a Ligacao
            lisTMP->Clear();
            SelecionaCelula(coordenada->Line->x1,coordenada->Line->y1, coordenada->Line->x2, coordenada->Line->y2, lisTMP);
            }
         else
            {//determina lista de Celulas que contém a Ligacao
            lisTMP->Clear();
            SelecionaCelula(coordenada->Line->x1,coordenada->Line->y1, coordenada->Line->x2, coordenada->Line->y2, lisTMP);
            SelecionaCelula(coordenada->Line->xm,coordenada->Line->ym, coordenada->Line->x3, coordenada->Line->y3, lisTMP);
            }
         //insere Ligacao nas Celulas selecionadas
         for (int nc = 0; nc < lisTMP->Count; nc++)
            {
            celula = (VTCelula*)lisTMP->Items[nc];
            celula->InsereEqptoRede(rede, layerREDE, ligacao);
            }
         }
      }
   //rede isolada
   rede = redes->RedeIsolada();
   //loop p/ todas Ligacoes da Rede
   lisLIGACAO = rede->LisLigacao();
   for (int nl = 0; nl < lisLIGACAO->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIGACAO->Items[nl];
      //determina coordenadas
      coordenada = redegraf->CoordLigacao[ligacao];
      if ((coordenada == NULL)||(coordenada->Line == NULL)) continue;
      //verifica se Ligacao conecta 2 ou 3 barras
      if (ligacao->pbarra3 == NULL)
         {//determina lista de Celulas que contém a Ligacao
         lisTMP->Clear();
         SelecionaCelula(coordenada->Line->x1,coordenada->Line->y1, coordenada->Line->x2, coordenada->Line->y2, lisTMP);
         }
      else
         {//determina lista de Celulas que contém a Ligacao
         lisTMP->Clear();
         SelecionaCelula(coordenada->Line->x1,coordenada->Line->y1, coordenada->Line->x2, coordenada->Line->y2, lisTMP);
         SelecionaCelula(coordenada->Line->xm,coordenada->Line->ym, coordenada->Line->x3, coordenada->Line->y3, lisTMP);
         }
      //insere Ligacao nas Celulas selecionadas
      for (int nc = 0; nc < lisTMP->Count; nc++)
         {
         celula = (VTCelula*)lisTMP->Items[nc];
         celula->InsereEqptoRede(rede, layerREDE, ligacao);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall  TCelulas::InsereLigacoesReducao(void)
   {
   //variáveis locais
   TList        *lisLIGACAO;
   TList        *lisREDUCAO;
   VTCelula     *celula;
   VTLigacao    *ligacao;
   VTMRede      *mrede;
   VTReducao    *reducao;
   VTCoordenada *coordenada;
   VTRedegraf   *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   VTRedes      *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList        *lisMR    = redes->LisMRede();

   //loop p/ todas MRedes
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->Carregada) continue;
      //OBS: uma MRede não carregada possui uma única Reducao
      lisREDUCAO = mrede->LisReducao();
      if (lisREDUCAO->Count == 0) continue;
      reducao = (VTReducao*)(lisREDUCAO->First());
      //loop p/ todas Ligacoes da Rede
      lisLIGACAO = reducao->LisLigacao();
      for (int nl = 0; nl < lisLIGACAO->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLIGACAO->Items[nl];
         //determina coordenadas
         coordenada = redegraf->CoordLigacao[ligacao];
         if ((coordenada == NULL)||(coordenada->Line == NULL)) continue;
         //OBS: uma Ligacao de Reducao conecta apenas 2 barras
         //determina lista de Celulas que contém a Ligacao
         lisTMP->Clear();
         SelecionaCelula(coordenada->Line->x1,coordenada->Line->y1, coordenada->Line->x2, coordenada->Line->y2, lisTMP);
         //insere Ligacao nas Celulas selecionadas
         for (int nc = 0; nc < lisTMP->Count; nc++)
            {
            celula = (VTCelula*)lisTMP->Items[nc];
            celula->InsereEqptoReducao(reducao, layerREDU_EQPTO, ligacao);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCelulas::InsereLigacaoFicticiaReducao(void)
   {
   //variáveis locais
   TList        *lisLIGACAO;
   TList        *lisREDUCAO;
   VTCelula     *celula;
   VTLigacao    *ligacao;
   VTMRede      *mrede;
   VTReducao    *reducao;
   VTCoordenada *coordenada;
   VTRedegraf   *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   VTRedes      *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList        *lisMR    = redes->LisMRede();

   //loop p/ todas MRedes
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->Carregada) continue;
      //OBS: uma MRede não carregada possui uma única Reducao
      lisREDUCAO = mrede->LisReducao();
      if (lisREDUCAO->Count == 0) continue;
      reducao = (VTReducao*)(lisREDUCAO->First());
      //loop p/ todas Ligacoes da Rede
      lisLIGACAO = reducao->LisLigacaoEsquematico();
      for (int nl = 0; nl < lisLIGACAO->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLIGACAO->Items[nl];
         //determina coordenadas
         coordenada = redegraf->CoordLigacao[ligacao];
         if ((coordenada == NULL)||(coordenada->Line == NULL)) continue;
         //OBS: uma Ligacao de Reducao conecta apenas 2 barras
         //determina lista de Celulas que contém a Ligacao
         lisTMP->Clear();
         SelecionaCelula(coordenada->Line->x1,coordenada->Line->y1, coordenada->Line->x2, coordenada->Line->y2, lisTMP);
         //insere Ligacao nas Celulas selecionadas
         for (int nc = 0; nc < lisTMP->Count; nc++)
            {
            celula = (VTCelula*)lisTMP->Items[nc];
            celula->InsereEqptoReducao(reducao, layerREDU_ESQ, ligacao);
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall  TCelulas::InsereMutuasRede(void)
   {
   //variáveis locais
   TList        *lisMUTUA;
   VTCelula     *celula;
   VTMutua      *mutua;
   VTRede       *rede;
   VTCoordenada *coordenada;
   VTRedegraf   *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   VTRedes      *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList        *lisREDE  = redes->LisRede();

   //loop p/ todas as Redes
   for (int nr = 0; nr < lisREDE->Count; nr++)
      {
      rede = (VTRede*)lisREDE->Items[nr];
      if (! rede->Carregada) continue;
      //loop p/ todas Mutuas da Rede
      lisMUTUA = rede->LisMutua();
      for (int nm = 0; nm < lisMUTUA->Count; nm++)
         {
         mutua = (VTMutua*)lisMUTUA->Items[nm];
         //determina coordenadas
         coordenada = redegraf->CoordLigacao[mutua];
         if ((coordenada == NULL)||(coordenada->Line == NULL)) continue;
         //determina lista de Celulas que contém a Mutua
         lisTMP->Clear();
         SelecionaCelula(coordenada->Line->x1,coordenada->Line->y1, coordenada->Line->x2, coordenada->Line->y2, lisTMP);
         //insere Mutua nas Celulas selecionadas
         for (int nc = 0; nc < lisTMP->Count; nc++)
            {
            celula = (VTCelula*)lisTMP->Items[nc];
            celula->InsereEqptoRede(rede, layerREDE, mutua);
            }
         }
      }
   //rede isolada
   rede = redes->RedeIsolada();
   //loop p/ todas Mutuas da Rede
   lisMUTUA = rede->LisMutua();
   for (int nm = 0; nm < lisMUTUA->Count; nm++)
      {
      mutua = (VTMutua*)lisMUTUA->Items[nm];
      //determina coordenadas
      coordenada = redegraf->CoordLigacao[mutua];
      if ((coordenada == NULL)||(coordenada->Line == NULL)) continue;
      //determina lista de Celulas que contém a Mutua
      lisTMP->Clear();
      SelecionaCelula(coordenada->Line->x1,coordenada->Line->y1, coordenada->Line->x2, coordenada->Line->y2, lisTMP);
      //insere Mutua nas Celulas selecionadas
      for (int nc = 0; nc < lisTMP->Count; nc++)
         {
         celula = (VTCelula*)lisTMP->Items[nc];
         celula->InsereEqptoRede(rede, layerREDE, mutua);
         }
      }
   }

//---------------------------------------------------------------------------
TList* __fastcall TCelulas::LisCelula(void)
   {
   return(lisCELULA);
   }

//---------------------------------------------------------------------------
TList* __fastcall TCelulas::LisCelulaVisivel(void)
   {
   return(lisATIVA);
   }

//---------------------------------------------------------------------------
void __fastcall TCelulas::LisCelulaVizinha(TList *lisEXT, VTCelula *celula, int delta)
   {
   //variáveis locais
   int       lin, col, i1, j1, i2, j2, index;
   VTCelula *celula_viz;

   //determina índices da Celula de referência
   IndicesDaCelula(celula, lin, col);
   //determina índices que delimitam a área vizinha
   if ((i1 = lin - delta) < 0) i1 = 0;
   if ((j1 = col - delta) < 0) j1 = 0;
   if ((i2 = lin + delta) >= grid.num_lin) i2 = grid.num_lin - 1;
   if ((j2 = col + delta) >= grid.num_col) j2 = grid.num_col - 1;
   for (int i = i1; i <= i2; i++)
      {
      for (int j = j1; j <= j2; j++)
         {//determina Celula vizinha
         index = (i * grid.num_col) + j;
         celula_viz = (VTCelula*)lisCELULA->Items[index];
         if (celula_viz == celula) continue;
         if (Distancia(celula, celula_viz) > delta) continue;
         if (lisEXT->IndexOf(celula_viz) < 0) lisEXT->Add(celula_viz);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCelulas::LisEqptoArea(TList *lisEXT, VTArea *area, int tipo)
   {
   //variáveis locais
   bool     vizinhas = true;
   VTCelula *celula;

   //inicia lisTMP c/ as Celula da área indicada
   lisTMP->Clear();
   if (area == NULL)
      {
      CopiaTList(lisCELULA, lisTMP);
      }
   else
      {
      SelecionaCelula(area->x1, area->y1, area->x2, area->y2, lisTMP, vizinhas) ;
      }
   //monta lisEQP c/ os Eqptos das Celulas selecionadas
   for (int n = 0; n < lisTMP->Count; n++)
      {
      celula = (VTCelula*)lisTMP->Items[n];
      celula->LisEqpto(lisEXT, tipo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCelulas::LisEqptoRedeAreaAtiva(TList *lisEXT, VTRede *rede, int tipo)
   {
   //variáveis locais
   VTCelula *celula;

   //verifica se todas as Celulas estão ativas (sem zoom)
   if (lisATIVA->Count == lisCELULA->Count)
      {
      rede->LisEqpto(lisEXT, tipo);
      }
   else
      {//loop p/ todas Celulas ativas
      for (int n = 0; n < lisATIVA->Count; n++)
         {
         celula = (VTCelula*)lisATIVA->Items[n];
         //obtém Eqptos da Celula
         celula->LisEqptoRede(lisEXT, layerREDE, rede, tipo);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCelulas::LisRedeArea(TList *lisEXT, VTArea *area)
   {
   //variáveis locais
   bool     vizinhas = true;
   VTCelula *celula;
   VTRedes  *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //reinicia lisTMP
   lisTMP->Clear();
   //verifica se foi definida uma Area
   if (area == NULL)
      {//retorna todas Redes
      redes->LisRedeCarregada(lisEXT);
      }
   else
      {//determina Celulas da Area
      SelecionaCelula(area->x1,area->y1, area->x2, area->y2, lisTMP, vizinhas);
      //determina Redes com Eqptos nas Celulas
      for (int n = 0; n < lisTMP->Count; n++)
         {
         celula = (VTCelula*)lisTMP->Items[n];
         celula->LisRede(lisEXT);
         }
      }
   }


//---------------------------------------------------------------------------
void __fastcall TCelulas::LisReducaoArea(TList *lisEXT, VTArea *area)
   {
   //variáveis locais
   bool     vizinhas = true;
   VTCelula *celula;
   VTRedes  *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //reinicia lisTMP
   lisTMP->Clear();
   //verifica se foi definida uma Area
   if (area == NULL)
      {//retorna todas Reduc oes
      redes->LisReducaoCarregada(lisEXT);
      }
   else
      {//determina Celulas da Area
      SelecionaCelula(area->x1,area->y1, area->x2, area->y2, lisTMP, vizinhas);
      //determina Reducao com Eqptos nas Celulas
      for (int n = 0; n < lisTMP->Count; n++)
         {
         celula = (VTCelula*)lisTMP->Items[n];
         celula->LisReducao(lisEXT);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCelulas::LisEqptoReducaoAreaAtiva(TList *lisEXT, VTReducao *reducao, int tipo)
   {
   //variáveis locais
   int      tipo_layer;
   VTCelula *celula;

   //loop p/ todas Celulas ativas
   for (int n = 0; n < lisATIVA->Count; n++)
      {
      celula = (VTCelula*)lisATIVA->Items[n];
      //obtém Eqptos da Celula
      if (reducao == NULL)
         {
         celula->LisEqptoReducao(lisEXT, layerREDU_EQPTO, reducao, tipo);
         celula->LisEqptoReducao(lisEXT, layerREDU_ESQ,   reducao, tipo);
         }
      else
         {
         tipo_layer = (reducao->Show[rdu_EQPTOS]) ? layerREDU_EQPTO : layerREDU_ESQ;
         celula->LisEqptoReducao(lisEXT, tipo_layer, reducao, tipo);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TCelulas::LisEqptoVisivelArea(TList *lisEXT, VTArea *area, int tipo)
   {
   //variáveis locais
   bool     vizinhas = true;
   VTCelula *celula;

   //inicia lisTMP c/ as Celula da área indicada
   lisTMP->Clear();
   if (area == NULL)
      {
      CopiaTList(lisCELULA, lisTMP);
      }
   else
      {
      SelecionaCelula(area->x1, area->y1, area->x2, area->y2, lisTMP, vizinhas) ;
      }
   //monta lisEQP c/ os Eqptos das Redes visíveis das Celulas selecionadas
   for (int n = 0; n < lisTMP->Count; n++)
      {
      celula = (VTCelula*)lisTMP->Items[n];
      celula->LisEqptoVisivel(lisEXT, tipo);
      }
   }

//---------------------------------------------------------------------------
int __fastcall TCelulas::PM_GetNumCelula(void)
   {
   return(lisCELULA->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TCelulas::PM_GetNumColuna(void)
   {
   return(grid.num_col);
   }

//---------------------------------------------------------------------------
int __fastcall TCelulas::PM_GetNumLinha(void)
   {
   return(grid.num_lin);
   }

//---------------------------------------------------------------------------
//Método obsoleto que deverá ser eliminado (mantido por compatibilidade)
//---------------------------------------------------------------------------
void __fastcall TCelulas::PM_SetNumCelula(int num_celula)
   {
   //proteção
   if ((num_celula <= 0)||(num_celula > 500)) return;
   //recria as Celulas
   DefineGrid(num_celula, num_celula);
   }

//---------------------------------------------------------------------------
int __fastcall  TCelulas::QtdeEqptoRedeAreaAtiva(VTRede *rede, int tipo)
   {
   //variáveis locais
   int      count = 0;
   VTCelula *celula;

   //IMPLEMENTAÇÃO parcial: retorna número de Barras e Licacoes
   //verifica se todas as Celulas estão ativas (sem zoom)
   if (lisATIVA->Count == lisCELULA->Count)
      {
      count  = (rede->LisBarra())->Count;
      count += (rede->LisLigacao())->Count;
      }
   else
      {//loop p/ todas Celulas ativas
      for (int n = 0; n < lisATIVA->Count; n++)
         {
         celula = (VTCelula*)lisATIVA->Items[n];
         //obtém número de Eqptos da Celula
         count += celula->QtdeEqptoRede(layerREDE, rede, tipo);
         }
      }
   return(count);
   }

//---------------------------------------------------------------------------
void __fastcall  TCelulas::ReiniciaCelulas(int num_lin, int num_col, int dim_cel_m)
   {
   //variáveis locais
   int        dim, dx, dy, x1, y1;
   VTCelula   *celula;
   VTRedes    *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   VTArea     *area     =  redegraf->Area(redes);

   //salva área total do grid
   grid.x1 = area->x1;
   grid.y1 = area->y1;
   grid.x2 = area->x2;
   grid.y2 = area->y2;
   //limpa conjunto de Celulas ativas
   lisATIVA->Clear();
   //destrói conjunto atual de Celulas
   LimpaTList(lisCELULA);
   //determina dimensões da área do grid (em pixel ou cm)
   dx  = (grid.x2 - grid.x1);
   dy  = (grid.y2 - grid.y1);
   dim = min(dx, dy);
   //verifica se foi definida a dimensão da Celula
   if (dim_cel_m > 0)
      {//determina dimensões da área do grid em metro
      dx *= 0.01;
      dy *= 0.01;
      //define número de linhas
      grid.num_lin = dy / dim_cel_m;
      if ((dy % dim_cel_m) > 0) grid.num_lin++;
      //define número de colunas
      grid.num_col = dx  / dim_cel_m;
      if ((dx  % dim_cel_m) > 0) grid.num_col++;
      //define dimensão de cada Celula
      grid.dx = grid.dy = dim_cel_m * 100;
      }
   else
      {//define número de linhas
      if (num_lin > 0)
         {grid.num_lin = num_lin;}
      else
         {grid.num_lin = (dim <= 2000) ? 1 : 50;}
      //define número de colunas
      if (num_col > 0)
         {grid.num_col = num_col;}
      else
         {grid.num_col = (dim <= 2000) ? 1 : 50;}
      //define dimensão de cada Celula
      grid.dx = dx / grid.num_col;
      grid.dy = dy / grid.num_lin;
      }
   //proteção:
   if (grid.dx == 0) grid.dx = 1;
   if (grid.dy == 0) grid.dy = 1;
   //recria Celulas
   for (int nl = 0; nl <  grid.num_lin; nl++)
      {//reinicia coordenada y
      y1 = grid.y1 + (nl * grid.dy);
      for (int nc = 0; nc < grid.num_col; nc++)
         {//reinicia coordenada y
         x1 = grid.x1 + (nc * grid.dx);
         //cria uma nova Celula p/ área (nl,nc)
         lisCELULA->Add(celula = NewObjCelula());
         //inicia coordenadas esq
         celula->Area->x1 = x1;
         celula->Area->y1 = y1;
         celula->Area->x2 = x1 + grid.dx;
         celula->Area->y2 = y1 + grid.dy;
         celula->Color    = redegraf->CorFundo;
         }
      }
   //verifica se o mapaemaneto dos eqptos nas celulas está habilitados
   if (EqptoEnabled)
      {//insere Barras e Ligacoes das Redes nas Celulas
      InsereBarrasRede();
      InsereEqbarRede();
      InsereLigacoesRede();
      InsereMutuasRede();
      //insere Barras, Eqbar e Ligacoes das Reducoes nas Celulas
      InsereBarrasReducao();
      InsereEqbarReducao();
      InsereLigacoesReducao();
      //insere Ligacoes fictícias do esquemático da Reducao
      InsereLigacaoFicticiaReducao();
      }
   //reinicia conjunto de Celulas ativas
   CopiaTList(lisCELULA, lisATIVA);
   }

//---------------------------------------------------------------------------
void __fastcall TCelulas::SelecionaCelula(int x1,int y1, int x2, int y2, TList *lisCEL, bool vizinhas)
   {
   //variáveis locais
   int       i1, i2, j1, j2, i_min, i_max, j_min, j_max, index;
   VTCelula  *celula;

   //proteção
   if ((grid.dx == 0)||(grid.dy == 0)) return;
   //determina (i1,j1) da Celula p/ coordenadas (x1, y1)
   i1 = (y1 - grid.y1) / grid.dy;
   j1 = (x1 - grid.x1) / grid.dx;
   //determina (i2,j2) da Celula p/ coordenadas (x2, y2)
   i2 = (y2 - grid.y1) / grid.dy;
   j2 = (x2 - grid.x1) / grid.dx;
   //proteção
   if      (i1 < 0)             i1 = 0;
   else if (i1 >= grid.num_lin) i1 = grid.num_lin - 1;
   if      (i2 < 0)             i2 = 0;
   else if (i2 >= grid.num_lin) i2 = grid.num_lin - 1;
   if      (j1 < 0)             j1 = 0;
   else if (j1 >= grid.num_col) j1 = grid.num_col - 1;
   if      (j2 < 0)             j2 = 0;
   else if (j2 >= grid.num_col) j2 = grid.num_col - 1;
   //determina valores mínimos e máximos
   i_min = min(i1, i2);
   i_max = max(i1, i2);
   j_min = min(j1, j2);
   j_max = max(j1, j2);
   //verifica se selecionou apenas uma célula
   if ((vizinhas)&&(i_min == i_max)&&(j_min == j_max))
      {//aumenta número de celulas
      if (i_min > 0) i_min--;
      if (j_min > 0) j_min--;
      if (i_max < (grid.num_lin - 1)) i_max++;
      if (j_max < (grid.num_col - 1)) j_max++;
      }
   for (int i = i_min; i <= i_max; i++)
      {
      for (int j = j_min; j <= j_max; j++)
         {
         index = (grid.num_col * i) + j;
         //insere Celula em lisCEL
         celula = (VTCelula*)lisCELULA->Items[index];
         if (lisCEL->IndexOf(celula) < 0) lisCEL->Add(celula);
         }
      }
   }

//---------------------------------------------------------------------------
//eof


