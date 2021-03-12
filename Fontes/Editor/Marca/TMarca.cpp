//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TMarca.h"
#include "..\Utils\TVerificaMalhaEdicao.h"
#include "..\..\Apl\VTApl.h"
#include "..\..\Busca\VTBusca.h"
#include "..\..\Edita\VTEdita.h"
#include "..\..\Rede\VTBarra.h"
#include "..\..\Rede\VTEqbar.h"
//#include "..\..\Rede\VTLigacao.h"
#include "..\..\Redegraf\VTRedegraf.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// métodos da classe TMarcaBar
//---------------------------------------------------------------------------
VTMarca* __fastcall NewObjMarcaBar(VTApl *apl, VTBarra *pbar, bool ref_x1y1)
   {
   try{
      return(new TMarcaBar(apl, pbar, ref_x1y1));
      }catch (Exception &)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TMarcaBar::TMarcaBar(VTApl *apl, VTBarra *pbar, bool ref_x1y1)
   {
   //salva ponteiro p/ objetos
   this->pbar = pbar;
   edita      = (VTEdita*)apl->GetObject(__classid(VTEdita));
   redegraf   = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   ref.x1y1   = ref_x1y1;
   }

//---------------------------------------------------------------------------
__fastcall TMarcaBar::~TMarcaBar(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TMarcaBar::BarraDaMarca(void)
   {
   return(pbar);
   }

//---------------------------------------------------------------------------
bool __fastcall TMarcaBar::DeBarra(void)
   {
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TMarcaBar::CoordenadasEsquematico(int &x1, int &y1, int &x2, int &y2)
   {
   //determina coordenadas esquemático da Barra
   pbar->CoordenadasEsquematico(x1, y1, x2, y2);
   //inicia coordenadas da marca
   if (ref.x1y1)
      {//marca associada a (x1,y1)
      x2 = x1;
      y2 = y1;
      }
   else
      {//marca associada a (x2,y2)
      x1 = x2;
      y1 = y2;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TMarcaBar::CoordenadasUtm_cm(int &x1, int &y1, int &x2, int &y2)
   {
   x1 = x2 = 0;
   y1 = y2 = 0;
   return(false);
   }

//---------------------------------------------------------------------------
int __fastcall TMarcaBar::Dimensao(double escala)
   {
   //variáveis locais
   int dx, dy;
   int dim = DIM_MARCA;  //dimensão default

   //verifica caso em que pode haver sobreposição de 2 marcas
   dx = abs(pbar->esq.x2 - pbar->esq.x1)*escala;
   dy = abs(pbar->esq.y2 - pbar->esq.y1)*escala;
   if ((dx < dim)&&(dy < dim))
      {
      dim += (ref.x1y1) ? 2 : -2;
      }
   return(dim);
   }

//---------------------------------------------------------------------------
void __fastcall TMarcaBar::Move(int coord_x, int coord_y, bool tecla_shift, VTBarra *pbar_nova, bool moving)
   {
   //variáveis locais
   struct {int x, y;} fixa;
   int dx, dy;

   //determina coordenadas fixas da Barra
   if (ref.x1y1)
      {//marca associada a (x1,y1): coordenadas fixas são (x2,y2)
      fixa.x = pbar->esq.x2; fixa.y = pbar->esq.y2;
      }
   else
      {//marca associada a (x2,y2);: coordenadas fixas são (x1,y1)
      fixa.x = pbar->esq.x1; fixa.y = pbar->esq.y1;
      }
   //OBS: sempre mantém como vertical ou horizontal
   /*
   //verifica se deve manter como linha horizontal ou vertical
   if (tecla_shift)
      {
      dx = abs(fixa.x - coord_x);
      dy = abs(fixa.y - coord_y);
      if (dx > dy)
         {coord_y = fixa.y;}
      else
         {coord_x = fixa.x;}
      }
   */
   dx = abs(fixa.x - coord_x);
   dy = abs(fixa.y - coord_y);
   if (dx > dy)
      {coord_y = fixa.y;}
   else
      {coord_x = fixa.x;}
   //redefine coordenadas da Barra
   if (ref.x1y1)
      //{pbar->esq.x1 = coord_x; pbar->esq.y1 = coord_y;}
      {edita->MoveBarra(pbar, coord_x, coord_y, pbar->esq.x2, pbar->esq.y2, moving);}
   else
      //{pbar->esq.x2 = coord_x; pbar->esq.y2 = coord_y;}
      {edita->MoveBarra(pbar, pbar->esq.x1, pbar->esq.y1, coord_x, coord_y, moving);}
   }

//---------------------------------------------------------------------------
// métodos da classe TMarcaEqbar
//---------------------------------------------------------------------------
VTMarca* __fastcall NewObjMarcaEqbar(VTApl *apl, VTEqbar *eqbar, VTBarra *pbar)
   {
   try{
      return(new TMarcaEqbar(apl, eqbar, pbar));
      }catch (Exception &)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TMarcaEqbar::TMarcaEqbar(VTApl *apl, VTEqbar *eqbar, VTBarra *pbar)
   {
   //salva ponteiro p/ objetos
   this->eqbar     = eqbar;
   this->bar_orig  = eqbar->pbarra;
   this->bar_dest  = NULL;
   edita           = (VTEdita*)apl->GetObject(__classid(VTEdita));
   redegraf        = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   //cria Barra temporária
   bar_tmp = bar_orig->Clone();
   bar_tmp->Show[eqp_VISIBLE] = false;
   //associa Eqbar com a Barra temporária
   eqbar->pbarra->RemoveEqbar(eqbar);
   bar_tmp->InsereEqbar(eqbar);
   }

//---------------------------------------------------------------------------
__fastcall TMarcaEqbar::~TMarcaEqbar(void)
   {
   //transfere Eqbar de bar_tmp p/ bar_orig
   eqbar->pbarra->RemoveEqbar(eqbar);
   bar_orig->InsereEqbar(eqbar);
   //verifica se foi selecionada uma Barra final p/ o Eqbar
   if (bar_dest != NULL)
      {//verifica se Barra destino é diferente da Barra original
      if (bar_dest == bar_orig)
         {//Eqbar pode ter sido deslocado no segmento da Barra original
         edita->MoveEqbar(eqbar, bar_tmp->esq.x1 + eqbar->esq.dxbar, bar_tmp->esq.y1 + eqbar->esq.dybar, false);
         }
      else
         {//verifica se é um Medidor
         if (eqbar->Tipo() == eqptoMEDIDOR)
            {//pede para usuário confirmar a alteração
            if (Confirma("As medições serão descartadas.", "Confirma a alteração ?") == IDYES)
               {
               edita->TrocaBarraEqbar(eqbar, bar_dest);
               }
            }
         //verifica se Barra dest possui mesma tensão da Barra original da Marca
         else if (IsDoubleZero(bar_dest->vnom - bar_orig->vnom))
            {//altera Barra do Eqbar
            edita->TrocaBarraEqbar(eqbar, bar_dest);
            }
         else
            {//pede para usuário confirmar a alteração
            if (Confirma("A barra selecionada possui tensão diferente da barra original",
                         "Confirma a alteração ?") == IDYES)
               {
               edita->TrocaBarraEqbar(eqbar, bar_dest);
               }
            }
         }
      }
   //destrói Barra temporária
   if (bar_tmp) {DLL_DeleteEqpto(bar_tmp); bar_tmp = NULL;}
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TMarcaEqbar::BarraDaMarca(void)
   {
   return(bar_orig);
   }

//---------------------------------------------------------------------------
void __fastcall TMarcaEqbar::CoordenadasEsquematico(int &x1, int &y1, int &x2, int &y2)
   {
   //determina coordenadas do Eqbar
   eqbar->CoordenadasEsquematico(x1, y1, x2, y2);
   //a Marca está associada a (x1,y1)
   x2 = x1;
   y2 = y1;
   }

//---------------------------------------------------------------------------
bool __fastcall TMarcaEqbar::CoordenadasUtm_cm(int &x1, int &y1, int &x2, int &y2)
   {
   //determina coordenadas do Eqbar
   eqbar->pbarra->CoordenadasUtm_cm(x1, y1, x2, y2);
   //a Marca está associada a (x1,y1)
   x2 = x1;
   y2 = y1;
   return(true);
   }

//---------------------------------------------------------------------------
int __fastcall TMarcaEqbar::Dimensao(double escala)
   {
   return(DIM_MARCA);
   }

//---------------------------------------------------------------------------
void __fastcall TMarcaEqbar::Move(int coord_x, int coord_y, bool shift, VTBarra *pbar_nova, bool moving)
   {
   //atualiza Barra alvo
   barra_alvo = pbar_nova;
   //verifica o modo gráfico em uso
   if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
      {//redefine coordenadas utm de bar_tmp
      bar_tmp->utm.x = coord_x;
      bar_tmp->utm.y = coord_y;
      }
   else
      {//redefine coordenadas esquemático de bar_tmp
      bar_tmp->esq.x1 = bar_tmp->esq.x2 = coord_x;
      bar_tmp->esq.y1 = bar_tmp->esq.y2 = coord_y;
      }
   //define Barra p/ onde Eqbar será removido
   bar_dest = pbar_nova;
   }

//---------------------------------------------------------------------------
// métodos da classe TMarcaLiga
//---------------------------------------------------------------------------
VTMarca* __fastcall NewObjMarcaLiga(VTApl *apl, VTLigacao *pliga, VTBarra *pbar)
   {
   try{
      return(new TMarcaLiga(apl, pliga, pbar));
      }catch (Exception &)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TMarcaLiga::TMarcaLiga(VTApl *apl, VTLigacao *pliga, VTBarra *pbar)
   {
   //salva ponteiro p/ objetos
   this->ligacao  = pliga;
   this->bar_orig = pbar;
   this->bar_dest = NULL;
   this->apl      = apl;
   edita       = (VTEdita*)apl->GetObject(__classid(VTEdita));
   redegraf    = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   //cria Barra temporária
   bar_tmp = bar_orig->Clone();
   bar_tmp->Codigo = "";
   bar_tmp->Show[eqp_VISIBLE] = false;
   //associa bar_tmp com a Barra indicada
   bar_tmp->Obj = pbar;
   //associa Ligacao com a Barra temporária
   //if      (ligacao->pbarra1 == bar_orig) ligacao->pbarra1 = bar_tmp;
   //else if (ligacao->pbarra2 == bar_orig) ligacao->pbarra2 = bar_tmp;
   //else if (ligacao->pbarra3 == bar_orig) ligacao->pbarra3 = bar_tmp;
   }

//---------------------------------------------------------------------------
__fastcall TMarcaLiga::~TMarcaLiga(void)
   {
   //variáveis locais
   AnsiString aviso    = "A barra selecionada possui tensão diferente.";
   AnsiString confirma = "Confirma a alteração ?";
   VTBarra    *bar_fixa;
   TVerificaMalhaEdicao *utilMalha = new TVerificaMalhaEdicao(apl);
   bool malhaIntAntes, malhaIntDepois, malhaExterna;

   malhaIntAntes = malhaIntDepois = malhaExterna = false;
   //restaura Barra da Ligacao
   if      (ligacao->pbarra1 == bar_tmp) ligacao->pbarra1 = bar_orig;
   else if (ligacao->pbarra2 == bar_tmp) ligacao->pbarra2 = bar_orig;
   else if (ligacao->pbarra3 == bar_tmp) ligacao->pbarra3 = bar_orig;
   //verifica se foi selecionada uma Barra para a Ligacao
   if (bar_dest != NULL)
      {//verifica se a Barra selecionada é igual à Barra original
      if (bar_dest == bar_orig)
         {//a Ligacao pode ter sido deslocada no segmento da barra original
         edita->MoveLigacao(ligacao, bar_orig, bar_tmp->esq.x1, bar_tmp->esq.y1, false);
         }
      else
		 {
		 //verifica se possui malha interna previa
		 malhaIntAntes = utilMalha->ExisteAlertaMalha(ligacao->rede);
		 //verifica se a Ligacao é um Trafo ou Trafo3E
         if ((ligacao->Tipo() == eqptoTRAFO) || (ligacao->Tipo() == eqptoTRAFO3E))
            {//verifica se Barra dest possui tensão igual a da Barra original
            if (IsDoubleZero(bar_dest->vnom - bar_orig->vnom))
               {//troca a Barra
               edita->TrocaBarraLigacao(ligacao, bar_orig, bar_dest);
               }
            else
               {//pede para usuário confirmar
               aviso += "\nAs tensões do transformador ficarão inconsistentes!";
               if (Confirma(aviso, confirma) == IDYES)
                  {//altera Barra da Ligacao
                  edita->TrocaBarraLigacao(ligacao, bar_orig, bar_dest);
                  }
               }
            }
         //ligação não é Trafo ou Trafo3E
         else
            {//determina Barra da Ligacao que não está sendo alterada
            if (bar_orig == ligacao->pbarra1) bar_fixa = ligacao->pbarra2;
            else                              bar_fixa = ligacao->pbarra1;
            //verifica se Barra dest possui a mesma tensão da Barra da Ligacao que não está sendo alterada
            if (IsDoubleZero(bar_dest->vnom - bar_fixa->vnom))
               {//troca a Barra
               edita->TrocaBarraLigacao(ligacao, bar_orig, bar_dest);
               }
            else
               {//pede para usuário confirmar
               aviso += "\nA rede ficará inconsistente!";
//               if (Confirma(aviso, confirma) == IDYES)
//                  {//altera Barra da Ligacao
//                  edita->TrocaBarraLigacao(ligacao, bar_orig, bar_dest);
//                  }
			   //pensando bem, é melhor não deixar o usuário fazer isso...
			   Erro(aviso);
			   }
			}
            //verifica se possui malha interna previa
			malhaIntDepois = utilMalha->ExisteAlertaMalha(ligacao->rede);
			//verifica se possui malha externa
			malhaExterna = utilMalha->VerificaSeFechaMalhaExterna(ligacao);
			//confirma acao, em caso negativo desfaz
			if ((!malhaIntAntes && malhaIntDepois) ||(malhaExterna))
			{
				if(Confirma("Esta operação fechou malha.", "Deseja confirmar ?") != IDYES)
				{
					edita->ActionUndo->Execute();
				}
			}
         }
      }
   //destrói Barra temporária
   if (bar_tmp) {DLL_DeleteEqpto(bar_tmp); bar_tmp = NULL;}

   //destro utilitario
   if (utilMalha){delete utilMalha; utilMalha = NULL;}
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TMarcaLiga::BarraDaMarca(void)
   {
   return(bar_orig);
   }

//---------------------------------------------------------------------------
void __fastcall TMarcaLiga::CoordenadasEsquematico(int &x1, int &y1, int &x2, int &y2)
   {
   //variáveis locais
   int xm, ym, x3, y3;

   //verifica se a Ligação conecta 2 ou 3 barras
   if (ligacao->pbarra3 == NULL)
      {//Ligação conecta 2 barras: inicia coordenadas da marca
      ligacao->CoordenadasEsquematico(x1, y1, x2, y2);
      }
   else
      {//Ligação conecta 3 barras: inicia coordenadas da marca
      ligacao->CoordenadasEsquematico(xm, ym, x1, y1, x2, y2, x3, y3);
      }
   if (bar_tmp->Obj == ligacao->pbarra1)
      {x2 = x1; y2 = y1;}
   else if (bar_tmp->Obj == ligacao->pbarra2)
      {x1 = x2; y1 = y2;}
   else //if (bar_tmp == ligacao->pbar3)
      {x1 = x2 = x3; y1 = y2 = y3;}
   }

//---------------------------------------------------------------------------
bool __fastcall TMarcaLiga::CoordenadasUtm_cm(int &x1, int &y1, int &x2, int &y2)
   {
   //variáveis locais
   int xm, ym, x3, y3;

   //verifica se a Ligação conecta 2 ou 3 barras
   if (ligacao->pbarra3 == NULL)
      {//Ligação conecta 2 barras: inicia coordenadas da marca
      ligacao->CoordenadasUtm_cm(x1, y1, x2, y2);
      }
   else
      {//Ligação conecta 3 barras: inicia coordenadas da marca
      ligacao->CoordenadasUtm_cm(xm, ym, x1, y1, x2, y2, x3, y3);
      }
   if (bar_tmp == ligacao->pbarra1)
      {x2 = x1; y2 = y1;}
   else if (bar_tmp == ligacao->pbarra2)
      {x1 = x2; y1 = y2;}
   else //if (bar_tmp == ligacao->pbar3)
      {x1 = x2 = x3; y1 = y2 = y3;}
   return(true);
   }

//---------------------------------------------------------------------------
int __fastcall TMarcaLiga::Dimensao(double escala)
   {
   return(DIM_MARCA);
   }

//---------------------------------------------------------------------------
void __fastcall TMarcaLiga::Move(int coord_x, int coord_y, bool shift, VTBarra *pbar_nova, bool moving)
   {
   //atualiza Barra alvo
   barra_alvo = pbar_nova;
   //verifica o modo gráfico em uso
   if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
      {//redefine coordenadas utm de bar_tmp
      bar_tmp->utm.x = coord_x;
      bar_tmp->utm.y = coord_y;
      }
   else
      {//redefine coordenadas esquemático de bar_tmp
      bar_tmp->esq.x1 = bar_tmp->esq.x2 = coord_x;
      bar_tmp->esq.y1 = bar_tmp->esq.y2 = coord_y;
      }
   //define Barra p/ onde Ligacao será reconectada
   bar_dest = pbar_nova;
   }

//---------------------------------------------------------------------------
void __fastcall TMarcaLiga::StartMove(void)
   {
   //variáveis locais
   VTBarra *bar_orig = (VTBarra*)bar_tmp->Obj;

   //associa Ligacao com a Barra temporária
   if      (ligacao->pbarra1 == bar_orig) ligacao->pbarra1 = bar_tmp;
   else if (ligacao->pbarra2 == bar_orig) ligacao->pbarra2 = bar_tmp;
   else if (ligacao->pbarra3 == bar_orig) ligacao->pbarra3 = bar_tmp;
   }

//---------------------------------------------------------------------------
void __fastcall TMarcaLiga::StopMove(void)
   {
   //variáveis locais
   VTBarra *bar_orig = (VTBarra*)bar_tmp->Obj;

   //associa Ligacao com a Barra original
   if      (ligacao->pbarra1 == bar_tmp) ligacao->pbarra1 = bar_orig;
   else if (ligacao->pbarra2 == bar_tmp) ligacao->pbarra2 = bar_orig;
   else if (ligacao->pbarra3 == bar_tmp) ligacao->pbarra3 = bar_orig;
   }

//---------------------------------------------------------------------------
//eof
