//---------------------------------------------------------------------------
#include <vcl.h>
#include <algorithm>
#pragma hdrstop
#include "TBusca.h"
#include "..\Apl\VTApl.h"
#include "..\Editor\Marca\TMarca.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Figura\VTFiguras.h"
#include "..\Figura\VTFiguraComentario.h"
#include "..\Figura\VTLink.h"
#include "..\Figura\VTRetangulo.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqbar.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMutua.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrafo3e.h"
#include "..\Rede\VTTrecho.h"
#include "..\Redegraf\VTCoordenada.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\Consulta.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//fun��o para criar objeto VTBusca
VTBusca *NewObjBusca(VTApl *apl)
   {
   return(new TBusca(apl));
   }

//---------------------------------------------------------------------------
__fastcall TBusca::TBusca(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   graf      = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   redegraf  = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   figuras   = (VTFiguras*)apl->GetObject(__classid(VTFiguras));
   //define cte. p/ busca
   delta = 20;
   //cria objetos
   consulta = DLL_NewObjConsulta(apl);
   lis      = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TBusca::~TBusca(void)
   {
   //destr�i objetos
   if (consulta) {delete consulta; consulta = NULL;}
   //destr�i lista sem destruir seus objetos
   if (lis) {delete lis; lis = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TBusca::AchaBarra(TList *lisBAR)
   {
   //vari�veis locais
   VTBarra    *pbarra;

   //loop p/ todas as Barras
   for (int nbar = 0; nbar < lisBAR->Count; nbar++)
      {
      pbarra = (VTBarra*)lisBAR->Items[nbar];
      if (! pbarra->Visible()) continue;
      if (AreaContemBarra(pbarra)) lis->Add(pbarra);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TBusca::AchaEqbar(TList *lisEQB)
   {
   //vari�veis locais
   VTEqbar *eqbar;

   //loop p/ todas Liga��es
   for (int n = 0; n < lisEQB->Count; n++)
      {
      eqbar = (VTEqbar*)lisEQB->Items[n];
      if (! eqbar->Visible()) continue;
      if (AreaContemEqbar(eqbar)) lis->Add(eqbar);
      }
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TBusca::AchaFigComentario(void)
   {
   //vari�veis locais
   VTFiguraComentario *comentario;
   TList              *lisEXT;

   //prote��o
   if (figuras != NULL)
      {
      lisEXT = figuras->LisComentario();
      for (int n = 0; n < lisEXT->Count; n++)
         {
         comentario = (VTFiguraComentario*)lisEXT->Items[n];
         if (! comentario->Visible) continue;
         if (AreaContemFigComentario(comentario)) lis->Add(comentario);
         }
      }
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
void __fastcall TBusca::AchaFigLink(void)
   {
   //vari�veis locais
   VTFiguraComentario *comentario;
   VTLink       *link;
   TList        *lisLINK;
   TList        *lisEXT = figuras->LisComentario();

   for (int i = 0; i < lisEXT->Count; i++)
      {
      comentario = (VTFiguraComentario*)lisEXT->Items[i];
      if (! comentario->Visible) continue;
      //procura Links do Comentario
      lisLINK = comentario->LisLink();
      for (int j = 0; j < lisLINK->Count; j++)
         {
         link = (VTLink*)lisLINK->Items[j];
         if (AreaContemFigLink(link)) lis->Add(link);
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TBusca::AchaFigRetangulo(void)
   {
   //vari�veis locais
   VTRetangulo *retangulo;
   TList       *lisEXT = figuras->LisRetangulo();

   for (int n = 0; n < lisEXT->Count; n++)
      {
      retangulo = (VTRetangulo*)lisEXT->Items[n];
      if (! retangulo->Visible) continue;
      if (AreaContemFigRetangulo(retangulo)) lis->Add(retangulo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TBusca::AchaLigacao(TList *lisLIG)
   {
   //vari�veis locais
   VTLigacao *pliga;

   //loop p/ todas Liga��es
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      pliga = (VTLigacao*)lisLIG->Items[nl];
      if (! pliga->Visible()) continue;
      if (AreaContemLigacao(pliga)) lis->Add(pliga);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TBusca::AchaMutua(TList *lisMUTUA)
   {
   //vari�veis locais
   VTMutua *mutua;

   //loop p/ todas Mutuas
   for (int nl = 0; nl < lisMUTUA->Count; nl++)
      {
      mutua = (VTMutua*)lisMUTUA->Items[nl];
      if (! mutua->Visible()) continue;
      if (AreaContemMutua(mutua)) lis->Add(mutua);
      }
   }
//---------------------------------------------------------------------------
void __fastcall TBusca::AchaRedeEsquematico(TList *lisREDE)
   {
   //vari�veis locais
   VTRede *rede;

   //loop p/ todas Redes
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (AreaContemRedeEsquematico(rede)) lis->Add(rede);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TBusca::AchaReducao(TList *lisREDU)
   {
   //vari�veis locais
   VTReducao *reducao;

   //loop p/ todas Redes
   for (int n = 0; n < lisREDU->Count; n++)
      {
      reducao = (VTReducao*)lisREDU->Items[n];
      //descarta Reducao que n�o est� vis�vel
      if (! reducao->Visible()) continue;
      //descarta Reducao que est� exibindo os Eqptos (e n�o o esquem�tico)
      if (reducao->Show[rdu_EQPTOS]) continue;
      if (AreaContemReducao(reducao)) lis->Add(reducao);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AchaNaDiagonal(int x1, int y1, int x2, int y2)
   {
   //vari�veis locais
   int    X1, Y1, X2, Y2;
   double m;
   int    ya, yb, ymin, ymax;

   //converte coordenadas de acordo c/ alinhamento no grid
   //grid->SnapToGrid(x1, y1, x2, y2);
   //OBS: a �rea na verdade indica um �nico ponto, isto �,
   //     area->x1 =area->x2 e  area->y1 = area->y2
   //ordena as coordenadas
   X1 = min(x1, x2);
   X2 = max(x1, x2);
   Y1 = min(y1, y2);
   Y2 = max(y1, y2);
   //verifica se a �rea est� fora das coordenadas
   if ((area->x1 < X1-delta)||(area->x1 > X2+delta)) return(false);
   if ((area->x2 < X1-delta)||(area->x2 > X2+delta)) return(false);
   if ((area->y1 < Y1-delta)||(area->y1 > Y2+delta)) return(false);
   if ((area->y2 < Y1-delta)||(area->y2 > Y2+delta)) return(false);
   //calcula declividade da reta
   m  = double(y2-y1)/double(x2-x1);
   //verifica se o ponto (area->x1, area->y1) pertence � reta
   //OBS: com existe uma imprecis�o 'delta', determina dois pontos
   //     (area->x1-delta-1, ymin) e (area->x1+delta+1, ymax)
   ya = y1 + m*((area->x1-delta-1)-x1);
   yb = y1 + m*((area->x1+delta+1)-x1);
   //determina ymin e ymax
   ymin = min(ya, yb);
   ymax = max(ya, yb);
   //verifica se a ordenada area->y1 est� entre ymin-delta e ymax+delta
   return((area->y1 >= (ymin-delta-1))&&(area->y1 <= (ymax+delta+1)));
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemBarra(VTBarra *pbar)
   {
   //vari�veis locais
   VTCoordenada *coordenada;

   //prote��o
   if (pbar == NULL) return(false);
	//determina Coordenada de Barra
   coordenada = redegraf->CoordBarra[pbar];
   if (coordenada->Line == NULL) return(false);
   //verifica se a Area cont�m as coordenadas da Barra
   return(AreaContemReta(coordenada->Line->x1, coordenada->Line->y1,
                         coordenada->Line->x2, coordenada->Line->y2));
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemEqbar(VTEqbar *eqbar)
   {
   //vari�veis locais
   VTCoordenada *coordenada;

   //determina Coordenada de Barra
   coordenada = redegraf->CoordEqbar[eqbar];
   if (coordenada->Line == NULL) return(false);
   return(AreaContemReta(coordenada->Line->x1, coordenada->Line->y1,
                         coordenada->Line->x2, coordenada->Line->y2));
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TBusca::AreaContemFigComentario(VTFiguraComentario *comentario)
   {
   //vari�veis locais
   int x1, x2, y1, y2, dx, dy;

   //determina coordenadas
   if (! redegraf->CoordFigComentario(comentario, graf->Escala(), x1, y1, x2, y2)) return(false);
   //verifica instersec��o de �reas
   return(AreaContemRetangulo(x1, y1, x2, y2));
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemFigLink(VTLink *link)
   {
   //vari�veis locais
   int x1, x2, y1, y2;

   //determina coordenadas
   if (! redegraf->CoordFigLink(link, graf->Escala(), x1, y1, x2, y2)) return(false);
   //verifica instersec��o de �reas
   return(AreaContemReta(x1, y1, x2, y2));
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemFigRetangulo(VTRetangulo *retangulo)
   {
   //vari�veis locais
   int x1, x2, y1, y2, dx, dy;

   //determina coordenadas
   if (! redegraf->CoordFigRetangulo(retangulo, graf->Escala(), x1, y1, x2, y2)) return(false);
   //verifica instersec��o de �reas
   return(AreaContemRetangulo(x1, y1, x2, y2));
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemLigacao(VTLigacao *pliga)
   {
   //vari�veis locais
   VTCoordenada *coordenada;

   //verifica se a Liga��o conecta apenas 2 Barras
   if (pliga->pbarra3 == NULL)
      {//Liga��o conecta 2 Barras: determina coordenadas
      coordenada = redegraf->CoordLigacao[pliga];
      if (coordenada->Line == NULL) return(false);
      return(AreaContemReta(coordenada->Line->x1, coordenada->Line->y1,
                            coordenada->Line->x2, coordenada->Line->y2));
      }
   //Liga��o conecta 3 Barras: determina coordenadas
   coordenada = redegraf->CoordTrafo3E[(VTTrafo3E*)pliga];
   if (coordenada->Line == NULL) return(false);
   if (AreaContemReta(coordenada->Line->xm, coordenada->Line->ym, coordenada->Line->x1, coordenada->Line->y1)) return(true);
   if (AreaContemReta(coordenada->Line->xm, coordenada->Line->ym, coordenada->Line->x2, coordenada->Line->y2)) return(true);
   if (AreaContemReta(coordenada->Line->xm, coordenada->Line->ym, coordenada->Line->x3, coordenada->Line->y3)) return(true);
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemMarca(VTMarca *marca)
   {
   //vari�veis locais
   VTCoordenada *coordenada;

   //define coordenadas de busca da Marca
   coordenada = redegraf->CoordMarca[marca];
   if (coordenada->Line == NULL) return(false);
   //IMPORTANTE: Marca pode n�o ter representa��o no gr�fico geo-referenciado
   if (coordenada->Line == NULL) return(false);
   return(AreaContemReta(coordenada->Line->x1, coordenada->Line->y1,
                            coordenada->Line->x2, coordenada->Line->y2));
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemMutua(VTMutua *mutua)
   {
   //vari�veis locais
   VTCoordenada *coordenada;

   //determina coordenadas
   coordenada = redegraf->CoordMutua[mutua];
   if (coordenada->Line == NULL) return(false);
   return(AreaContemReta(coordenada->Line->x1, coordenada->Line->y1, coordenada->Line->x2, coordenada->Line->y2));
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemRedeEsquematico(VTRede *rede)
   {
   //vari�veis locais
   VTArea        *area;
   int           x, y;
   int           x1, y1, x2, y2;
   int           width, height;
   double        escala;

   //determina coordenadas da Rede
   area = redegraf->Area(rede);
   x = (area->x1 + area->x2) / 2;
   y = (area->y1 + area->y2) / 2;
   //obt�m dimens�o do String no gr�fico (em pixel)
   graf->SizeOfString(rede->Codigo, width, height);
   //converte dimens�es do String p/ unidade da rede
   if ((escala = graf->Escala()) == 0.) escala = 1.;
   width  /= escala;
   height /= escala;
   //define coordenadas de busca da Rede
   x1 = x - (width/2);
   x2 = x + (width/2);
   y1 = y - (height/2);
   y2 = y + (height/2);
   //verifica se a �rea selecionada cont�m a Rede
   return(AreaContemRetangulo(x1, y1, x2, y2));
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemReducao(VTReducao *reducao)
   {
   //vari�veis locais
   int          x1, y1, x2, y2;
   int          width, height;
   double       escala;
   VTBarra      *pbarra  = reducao->BarraFicticia();
   VTCoordenada *coordenada;

   //prote��o
   if(pbarra == NULL) return(false);
   //obt�m dimens�o do String no gr�fico (em pixel)
   graf->SizeOfString(reducao->Codigo, width, height);
   //converte dimens�es do String p/ unidade da rede
   if ((escala = graf->Escala()) == 0.) escala = 1.;
   width  /= escala;
   height /= escala;
   //define coordenadas de busca da Rede
   coordenada = redegraf->CoordBarra[pbarra];
   if (coordenada->Line == NULL) return(false);
   //x1 = coordenada->Line->x1 - (width/2);
   //x2 = coordenada->Line->x1 + (width/2);
   x1 = coordenada->Line->x1 - (height/2);
   x2 = coordenada->Line->x1 + (height/2);
   y1 = coordenada->Line->y1 - (height/2);
   y2 = coordenada->Line->y1 + (height/2);
   //verifica se a �rea selecionada cont�m a Rede
   return(AreaContemRetangulo(x1, y1, x2, y2));
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemReta(int x1, int y1, int x2, int y2)
   {
   //vari�veis locais
   int X1, Y1, X2, Y2;

   //verifica se a Area foi definida
   if (area == NULL) return(true);
   //ordena as coordenadas
   X1 = min(x1, x2);
   X2 = max(x1, x2);
   Y1 = min(y1, y2);
   Y2 = max(y1, y2);
   //verifica se a �rea define apenas um ponto
   if ((area->x1 == area->x2)&&(area->y1 == area->y2))
      {//verifica se as coordenadas definem uma reta horizontal ou vertical
      if ((X1==X2)||(Y1==Y2))
         {//verifica se a �rea est� contida nas coordenadas
         if ((area->x1 >= X1-delta)&&(area->x1 <= X2+delta)&&
             (area->x2 >= X1-delta)&&(area->x2 <= X2+delta)&&
             (area->y1 >= Y1-delta)&&(area->y1 <= Y2+delta)&&
             (area->y2 >= Y1-delta)&&(area->y2 <= Y2+delta)) return(true);
         }
      else
         {//coordenadas definem uma diagonal
         if(AchaNaDiagonal(x1,y1,x2,y2)) return(true);
         }
      }
   else
      {//�rea n�o � apenas um ponto: verifica se �rea cont�m as coord.
      if ((X1 >= area->x1-delta)&&(X1 <= area->x2+delta)&&
          (X2 >= area->x1-delta)&&(X2 <= area->x2+delta)&&
          (Y1 >= area->y1-delta)&&(Y1 <= area->y2+delta)&&
          (Y2 >= area->y1-delta)&&(Y2 <= area->y2+delta)) return(true);
      }
   //�rea n�o cont�m as coordenadas x1,y1,x2,y2 e n�o est� contidas por elas
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemRetangulo(int x1, int y1, int x2, int y2)
   {
   //vari�veis locais
   int X1, Y1, X2, Y2;

   //verifica se a Area foi definida
   if (area == NULL) return(true);
   //ordena as coordenadas
   X1 = min(x1, x2);
   X2 = max(x1, x2);
   Y1 = min(y1, y2);
   Y2 = max(y1, y2);
   //verifica se a �rea est� contida nas coordenadas
   if ((area->x1 >= X1-delta)&&(area->x1 <= X2+delta)&&
       (area->x2 >= X1-delta)&&(area->x2 <= X2+delta)&&
       (area->y1 >= Y1-delta)&&(area->y1 <= Y2+delta)&&
       (area->y2 >= Y1-delta)&&(area->y2 <= Y2+delta)) return(true);
   //verifica se �rea cont�m as coord.
   if ((X1 >= area->x1-delta)&&(X1 <= area->x2+delta)&&
       (X2 >= area->x1-delta)&&(X2 <= area->x2+delta)&&
       (Y1 >= area->y1-delta)&&(Y1 <= area->y2+delta)&&
       (Y2 >= area->y1-delta)&&(Y2 <= area->y2+delta)) return(true);
   return(false);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Barra(VTArea *area = NULL)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //acha Barras das Redes na �rea
	AchaBarra(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoBARRA));
   //acha Barras isoladas
   //AchaBarra(consulta->LisEqptoIsolado(redes, eqptoBARRA));
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Capacitor(VTArea *area = NULL)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //acha Capacitores da rede na �rea
   AchaEqbar(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoCAPACITOR));
   //acha Capacitores isolados
   //AchaEqbar(consulta->LisEqptoIsolado(redes, eqptoCAPACITOR));
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Carga(VTArea *area = NULL)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //acha Cargas da rede na �rea
   AchaEqbar(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoCARGA));
   //acha Cargas isoladas
   //AchaEqbar(consulta->LisEqptoIsolado(redes, eqptoCARGA));
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Chave(VTArea *area = NULL)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //determina Chaves na �rea
   AchaLigacao(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoCHAVE));
   //acha Chaves isoladas
   //AchaLigacao(consulta->LisEqptoIsolado(redes, eqptoCHAVE));
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Chave(VTBarra *pbar)
   {
   //vari�veis locais
   VTLigacao  *pliga;
   TList     *lisCHV;

   //limpa lista
   lis->Clear();
   //determina Chaves ligadas � Barra
   lisCHV = consulta->LisEqptoRedeReducaoVisible(redes, NULL, eqptoCHAVE);
   for(int n = 0; n < lisCHV->Count; n++)
      {
      pliga = (VTLigacao*)lisCHV->Items[n];
      if ((pliga->pbarra1 == pbar)||(pliga->pbarra2 == pbar)) lis->Add(pliga);
      }
   /*
   //determina Chaves isoladas ligadas � Barra
   lisCHV = consulta->LisEqptoIsolado(redes, eqptoCHAVE);
   for(int n = 0; n < lisCHV->Count; n++)
      {
      pliga = (VTLigacao*)lisCHV->Items[n];
      if ((pliga->pbarra1 == pbar)||(pliga->pbarra2 == pbar)) lis->Add(pliga);
      }
   */
   return(lis);
   }

//---------------------------------------------------------------------------
void __fastcall TBusca::DefineDelta(void)
   {
   //vari�veis locais
   double escala;

   //assume valor default
   delta = 1;
   //verifica se o objeto VTGraf est� definido
   if (graf)
      {
      if ((escala = graf->Escala()) == 0.) escala = 1.;
      delta = int(3./escala) + 1;
      }
   }

/*
//---------------------------------------------------------------------------
void __fastcall TBusca::DefineDelta(void)
   {
   //vari�veis locais
   double  width;
   VTArea *area_visivel;

   //assume valor default
   delta = 1;
   //verifica se o objeto VTGraf est� definido
   if (graf)
      {
      area_visivel = graf->AreaVisivel;
      delta        = 0.01 * area_visivel->Width();
      if (delta < 2) delta = 2;
      }
   }
*/
//---------------------------------------------------------------------------
TList* __fastcall TBusca::Eqbar(VTArea *area = NULL)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //acha Eqbar da rede na �rea
   AchaEqbar(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoEQBAR));
   //AchaEqbar(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoCAPACITOR));
   //AchaEqbar(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoCARGA));
   //AchaEqbar(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoGERADOR));
   //AchaEqbar(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoSUPRIMENTO));
   //AchaEqbar(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoZREF));
   //acha Eqbar isolados
   //AchaEqbar(consulta->LisEqptoIsolado(redes, eqptoEQBAR));
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Eqpto(VTArea *area = NULL)
   {
   //salva ponteiro para Area
   this->area = area;
   //limpa lista
   lis->Clear();
   //prote��o
//   if ((redes->LisRede())->Count == 0) return(lis);
   //determina delta
   DefineDelta();
   //acha Barra, Eqpbar, Liga��o, Mutua, Reducao na �rea
   AchaBarra(consulta->LisEqptoRedeReducaoVisible(redes, area,  eqptoBARRA));
   AchaEqbar(consulta->LisEqptoRedeReducaoVisible(redes, area,  eqptoEQBAR));
   AchaLigacao(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoLIGACAO));
   AchaMutua(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoMUTUA));
   AchaReducao(consulta->LisReducaoCarregadaArea(redes, area));
   //acha Redes do Esquematico
   //AchaRedeEsquematico(consulta->LisRedeEsquematicoArea(redes, area));
   AchaRedeEsquematico(consulta->LisRedeEsquematico(redes));

   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Eqpto(VTBarra *pbar)
   {
   //vari�veis locais
   bool visible = true;

   //reinicia lis
   lis->Clear();
   //inicia lista c/ Liga��es conectadas � Barra
   Ligacao(pbar, visible);
   //inclui todos os Eqptos de barra ligados � barra
   pbar->LisEqbar(lis);
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::FigComentario(VTArea *area)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //determina Comentarios na �rea
   AchaFigComentario();
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::FigLink(VTArea *area)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //determina Links na �rea
   AchaFigLink();
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::FigRetangulo(VTArea *area)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //determina Retangulos na �rea
   AchaFigRetangulo();
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Figura(VTArea *area)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //determina Retangulos na �rea
   AchaFigComentario();
   AchaFigLink();
   AchaFigRetangulo();
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Gerador(VTArea *area = NULL)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //acha Geradores da rede na �rea
   AchaEqbar(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoGERADOR));
   //acha Geradores isolados na �rea
   //AchaEqbar(consulta->LisEqptoIsolado(redes, eqptoGERADOR));
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Ligacao(VTArea *area = NULL)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //determina Liga��es das Redes na �rea
   AchaLigacao(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoLIGACAO));
   //determina Liga��es isoladas na �rea
   //AchaLigacao(consulta->LisEqptoIsolado(redes, eqptoLIGACAO));
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Ligacao(VTBarra *pbar, bool visible)
   {
   //vari�veis locais
   TList     *lisLIG;
   VTLigacao *ligacao;

   //detemina as Liga��es conectadas � Barra
   if (visible)
      {//somente Liga��es vis�veis
      lisLIG = consulta->LisEqptoRedeReducaoVisible(redes, eqptoLIGACAO);
      }
   else
      {//todas Liga��es, vis�veis ou n�o
      lisLIG = consulta->LisEqptoRedeReducao(redes, eqptoLIGACAO);
      }
   //descarta Ligacoes que n�o conectam a Barra
   lis->Clear();
   for (int nl = 0; nl < lisLIG->Count; nl++)
      {
      ligacao = (VTLigacao*)lisLIG->Items[nl];
      if (ligacao->ConectaBarra(pbar))
         {//insere Ligacao em lis
         lis->Add(ligacao);
         }
      }
   return(lis);
   }
//---------------------------------------------------------------------------
TList* __fastcall TBusca::Ligacao(TList *lisBAR)
   {
   //vari�veis locais
   VTBarra   *pbar;
   VTEqpto   *eqpto;
   VTLigacao *pliga;
   TList     *lisLIG;

   //reinicia lis
   lis->Clear();
   //determina �rea definida pelas Barras
   if (! redegraf->Area(lisBAR, area)) return(lis);
   //obt�m lista de Liga��es das Redes na �rea
   lisLIG = consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoLIGACAO);
   //determina Ligacoes que conectam as Barras
   for (int n = 0; n < lisLIG->Count; n++)
      {
      pliga = (VTLigacao*)lisLIG->Items[n];
      //verifica se a Ligacao conecta uma das Barras
      for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
         {
         if (lisBAR->IndexOf(pliga->Barra(ind_bar)) >= 0)
            {//insere Ligacao em lisEQN
            lis->Add(pliga);
            break;
            }
         }
      }
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::LigacaoEsquematicoReducao(VTBarra *pbar)
   {
   //vari�veis locais
   VTLigacao *ligacao;
   VTReducao *reducao;
   TList     *lisREDUCAO;
   TList     *lisLIG_FIC;

   //limpa lista
   lis->Clear();
   //acha Reducoes que conectam a Barra
   lisREDUCAO = consulta->LisReducaoCarregada(redes);
   //determina Ligacoes fict�cias que conectam a Barra
   for (int nr = 0; nr < lisREDUCAO->Count; nr++)
      {
      reducao = (VTReducao*)lisREDUCAO->Items[nr];
      //obt�m lista de Ligacoes do esquem�tico da Reducao
      lisLIG_FIC = reducao->LisLigacaoEsquematico();
      for (int nl= 0; nl < lisLIG_FIC->Count; nl++)
         {
         ligacao = (VTLigacao*)lisLIG_FIC->Items[nl];
         //verifica se a Ligacao conecta a Barra
         if (ligacao->ConectaBarra(pbar)) lis->Add(ligacao);
         }
      }
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::LigacaoNaoChave(VTArea *area = NULL)
   {
   //vari�veis locais
   VTLigacao *pliga;

   //detemina Liga��es da �rea, incluindo Chaves
   Ligacao(area);
   //descarta Chaves
   for (int nl = lis->Count-1; nl >= 0; nl--)
      {
      pliga = (VTLigacao*)lis->Items[nl];
      if (pliga->Tipo() == eqptoCHAVE) lis->Remove(pliga);
      }
   return(lis);
   }

//---------------------------------------------------------------------------
//fun��o espec�fica p/ editor: TMarca n�o � um eqpto da RedeSUb
//---------------------------------------------------------------------------
VTMarca* __fastcall TBusca::Marca(TList *lisMAR, VTArea *area)
   {
   //vari�veis locais
   VTMarca *marca;

   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   delta = 3*delta;
   //procura Marca na �rea
   for (int n = 0; n < lisMAR->Count; n++)
      {
      marca = (VTMarca*)lisMAR->Items[n];
      if (AreaContemMarca(marca)) return(marca);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Mutua(VTArea *area)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //determina Liga��es das Redes na �rea
   AchaMutua(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoMUTUA));
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Mutua(VTBarra *pbar)
   {
   //vari�veis locais
   VTMutua *mutua;
   TList   *lisMUTUA = consulta->LisEqptoRede(redes, eqptoMUTUA);

   //limpa lista
   lis->Clear();
   //verifica se os Trechos das Mutuas conectam a Barra indicada
   for (int n = 0; n < lisMUTUA->Count; n++)
      {
      mutua = (VTMutua*)lisMUTUA->Items[n];
      if ((mutua->ptre1->ConectaBarra(pbar))||(mutua->ptre2->ConectaBarra(pbar)))
         {//insere Mutua em lis
         lis->Add(mutua);
         }
      }
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Reducao(VTArea *area)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //acha Reducoes na �rea
   AchaReducao(consulta->LisReducaoCarregada(redes));
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::ReducaoAll(VTArea *area)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //acha Reducoes na �rea
   AchaReducao(consulta->LisReducao(redes));
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Suprimento(VTArea *area)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //acha Suprimentos da rede na �rea
   AchaEqbar(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoSUPRIMENTO));
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Trafo(VTArea *area)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //determina Trafos na �rea
   AchaLigacao(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoTRAFO));
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Trafo3E(VTArea *area)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //determina Trafo3E na �rea
   AchaLigacao(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoTRAFO3E));
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Trecho(VTArea *area)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //determina Trechos na �rea
   AchaLigacao(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoTRECHO));
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Yref(VTArea *area)
   {
   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   //limpa lista
   lis->Clear();
   //acha Cargas da rede na �rea
	AchaEqbar(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoYREF));
   return(lis);
   }

//---------------------------------------------------------------------------
//eof
