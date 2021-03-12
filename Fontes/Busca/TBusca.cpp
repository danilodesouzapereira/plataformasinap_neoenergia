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
//função para criar objeto VTBusca
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
   //destrói objetos
   if (consulta) {delete consulta; consulta = NULL;}
   //destrói lista sem destruir seus objetos
   if (lis) {delete lis; lis = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TBusca::AchaBarra(TList *lisBAR)
   {
   //variáveis locais
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
   //variáveis locais
   VTEqbar *eqbar;

   //loop p/ todas Ligações
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
   //variáveis locais
   VTFiguraComentario *comentario;
   TList              *lisEXT;

   //proteção
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
   //variáveis locais
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
   //variáveis locais
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
   //variáveis locais
   VTLigacao *pliga;

   //loop p/ todas Ligações
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
   //variáveis locais
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
   //variáveis locais
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
   //variáveis locais
   VTReducao *reducao;

   //loop p/ todas Redes
   for (int n = 0; n < lisREDU->Count; n++)
      {
      reducao = (VTReducao*)lisREDU->Items[n];
      //descarta Reducao que não está visível
      if (! reducao->Visible()) continue;
      //descarta Reducao que está exibindo os Eqptos (e não o esquemático)
      if (reducao->Show[rdu_EQPTOS]) continue;
      if (AreaContemReducao(reducao)) lis->Add(reducao);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AchaNaDiagonal(int x1, int y1, int x2, int y2)
   {
   //variáveis locais
   int    X1, Y1, X2, Y2;
   double m;
   int    ya, yb, ymin, ymax;

   //converte coordenadas de acordo c/ alinhamento no grid
   //grid->SnapToGrid(x1, y1, x2, y2);
   //OBS: a área na verdade indica um único ponto, isto é,
   //     area->x1 =area->x2 e  area->y1 = area->y2
   //ordena as coordenadas
   X1 = min(x1, x2);
   X2 = max(x1, x2);
   Y1 = min(y1, y2);
   Y2 = max(y1, y2);
   //verifica se a área está fora das coordenadas
   if ((area->x1 < X1-delta)||(area->x1 > X2+delta)) return(false);
   if ((area->x2 < X1-delta)||(area->x2 > X2+delta)) return(false);
   if ((area->y1 < Y1-delta)||(area->y1 > Y2+delta)) return(false);
   if ((area->y2 < Y1-delta)||(area->y2 > Y2+delta)) return(false);
   //calcula declividade da reta
   m  = double(y2-y1)/double(x2-x1);
   //verifica se o ponto (area->x1, area->y1) pertence à reta
   //OBS: com existe uma imprecisão 'delta', determina dois pontos
   //     (area->x1-delta-1, ymin) e (area->x1+delta+1, ymax)
   ya = y1 + m*((area->x1-delta-1)-x1);
   yb = y1 + m*((area->x1+delta+1)-x1);
   //determina ymin e ymax
   ymin = min(ya, yb);
   ymax = max(ya, yb);
   //verifica se a ordenada area->y1 está entre ymin-delta e ymax+delta
   return((area->y1 >= (ymin-delta-1))&&(area->y1 <= (ymax+delta+1)));
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemBarra(VTBarra *pbar)
   {
   //variáveis locais
   VTCoordenada *coordenada;

   //proteção
   if (pbar == NULL) return(false);
	//determina Coordenada de Barra
   coordenada = redegraf->CoordBarra[pbar];
   if (coordenada->Line == NULL) return(false);
   //verifica se a Area contém as coordenadas da Barra
   return(AreaContemReta(coordenada->Line->x1, coordenada->Line->y1,
                         coordenada->Line->x2, coordenada->Line->y2));
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemEqbar(VTEqbar *eqbar)
   {
   //variáveis locais
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
   //variáveis locais
   int x1, x2, y1, y2, dx, dy;

   //determina coordenadas
   if (! redegraf->CoordFigComentario(comentario, graf->Escala(), x1, y1, x2, y2)) return(false);
   //verifica instersecção de áreas
   return(AreaContemRetangulo(x1, y1, x2, y2));
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemFigLink(VTLink *link)
   {
   //variáveis locais
   int x1, x2, y1, y2;

   //determina coordenadas
   if (! redegraf->CoordFigLink(link, graf->Escala(), x1, y1, x2, y2)) return(false);
   //verifica instersecção de áreas
   return(AreaContemReta(x1, y1, x2, y2));
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemFigRetangulo(VTRetangulo *retangulo)
   {
   //variáveis locais
   int x1, x2, y1, y2, dx, dy;

   //determina coordenadas
   if (! redegraf->CoordFigRetangulo(retangulo, graf->Escala(), x1, y1, x2, y2)) return(false);
   //verifica instersecção de áreas
   return(AreaContemRetangulo(x1, y1, x2, y2));
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemLigacao(VTLigacao *pliga)
   {
   //variáveis locais
   VTCoordenada *coordenada;

   //verifica se a Ligação conecta apenas 2 Barras
   if (pliga->pbarra3 == NULL)
      {//Ligação conecta 2 Barras: determina coordenadas
      coordenada = redegraf->CoordLigacao[pliga];
      if (coordenada->Line == NULL) return(false);
      return(AreaContemReta(coordenada->Line->x1, coordenada->Line->y1,
                            coordenada->Line->x2, coordenada->Line->y2));
      }
   //Ligação conecta 3 Barras: determina coordenadas
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
   //variáveis locais
   VTCoordenada *coordenada;

   //define coordenadas de busca da Marca
   coordenada = redegraf->CoordMarca[marca];
   if (coordenada->Line == NULL) return(false);
   //IMPORTANTE: Marca pode não ter representação no gráfico geo-referenciado
   if (coordenada->Line == NULL) return(false);
   return(AreaContemReta(coordenada->Line->x1, coordenada->Line->y1,
                            coordenada->Line->x2, coordenada->Line->y2));
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemMutua(VTMutua *mutua)
   {
   //variáveis locais
   VTCoordenada *coordenada;

   //determina coordenadas
   coordenada = redegraf->CoordMutua[mutua];
   if (coordenada->Line == NULL) return(false);
   return(AreaContemReta(coordenada->Line->x1, coordenada->Line->y1, coordenada->Line->x2, coordenada->Line->y2));
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemRedeEsquematico(VTRede *rede)
   {
   //variáveis locais
   VTArea        *area;
   int           x, y;
   int           x1, y1, x2, y2;
   int           width, height;
   double        escala;

   //determina coordenadas da Rede
   area = redegraf->Area(rede);
   x = (area->x1 + area->x2) / 2;
   y = (area->y1 + area->y2) / 2;
   //obtém dimensão do String no gráfico (em pixel)
   graf->SizeOfString(rede->Codigo, width, height);
   //converte dimensões do String p/ unidade da rede
   if ((escala = graf->Escala()) == 0.) escala = 1.;
   width  /= escala;
   height /= escala;
   //define coordenadas de busca da Rede
   x1 = x - (width/2);
   x2 = x + (width/2);
   y1 = y - (height/2);
   y2 = y + (height/2);
   //verifica se a Área selecionada contém a Rede
   return(AreaContemRetangulo(x1, y1, x2, y2));
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemReducao(VTReducao *reducao)
   {
   //variáveis locais
   int          x1, y1, x2, y2;
   int          width, height;
   double       escala;
   VTBarra      *pbarra  = reducao->BarraFicticia();
   VTCoordenada *coordenada;

   //proteção
   if(pbarra == NULL) return(false);
   //obtém dimensão do String no gráfico (em pixel)
   graf->SizeOfString(reducao->Codigo, width, height);
   //converte dimensões do String p/ unidade da rede
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
   //verifica se a Área selecionada contém a Rede
   return(AreaContemRetangulo(x1, y1, x2, y2));
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemReta(int x1, int y1, int x2, int y2)
   {
   //variáveis locais
   int X1, Y1, X2, Y2;

   //verifica se a Area foi definida
   if (area == NULL) return(true);
   //ordena as coordenadas
   X1 = min(x1, x2);
   X2 = max(x1, x2);
   Y1 = min(y1, y2);
   Y2 = max(y1, y2);
   //verifica se a Área define apenas um ponto
   if ((area->x1 == area->x2)&&(area->y1 == area->y2))
      {//verifica se as coordenadas definem uma reta horizontal ou vertical
      if ((X1==X2)||(Y1==Y2))
         {//verifica se a Área está contida nas coordenadas
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
      {//Área não é apenas um ponto: verifica se Área contém as coord.
      if ((X1 >= area->x1-delta)&&(X1 <= area->x2+delta)&&
          (X2 >= area->x1-delta)&&(X2 <= area->x2+delta)&&
          (Y1 >= area->y1-delta)&&(Y1 <= area->y2+delta)&&
          (Y2 >= area->y1-delta)&&(Y2 <= area->y2+delta)) return(true);
      }
   //Área não contém as coordenadas x1,y1,x2,y2 e não está contidas por elas
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TBusca::AreaContemRetangulo(int x1, int y1, int x2, int y2)
   {
   //variáveis locais
   int X1, Y1, X2, Y2;

   //verifica se a Area foi definida
   if (area == NULL) return(true);
   //ordena as coordenadas
   X1 = min(x1, x2);
   X2 = max(x1, x2);
   Y1 = min(y1, y2);
   Y2 = max(y1, y2);
   //verifica se a Área está contida nas coordenadas
   if ((area->x1 >= X1-delta)&&(area->x1 <= X2+delta)&&
       (area->x2 >= X1-delta)&&(area->x2 <= X2+delta)&&
       (area->y1 >= Y1-delta)&&(area->y1 <= Y2+delta)&&
       (area->y2 >= Y1-delta)&&(area->y2 <= Y2+delta)) return(true);
   //verifica se Área contém as coord.
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
   //acha Barras das Redes na área
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
   //acha Capacitores da rede na área
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
   //acha Cargas da rede na área
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
   //determina Chaves na área
   AchaLigacao(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoCHAVE));
   //acha Chaves isoladas
   //AchaLigacao(consulta->LisEqptoIsolado(redes, eqptoCHAVE));
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Chave(VTBarra *pbar)
   {
   //variáveis locais
   VTLigacao  *pliga;
   TList     *lisCHV;

   //limpa lista
   lis->Clear();
   //determina Chaves ligadas à Barra
   lisCHV = consulta->LisEqptoRedeReducaoVisible(redes, NULL, eqptoCHAVE);
   for(int n = 0; n < lisCHV->Count; n++)
      {
      pliga = (VTLigacao*)lisCHV->Items[n];
      if ((pliga->pbarra1 == pbar)||(pliga->pbarra2 == pbar)) lis->Add(pliga);
      }
   /*
   //determina Chaves isoladas ligadas à Barra
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
   //variáveis locais
   double escala;

   //assume valor default
   delta = 1;
   //verifica se o objeto VTGraf está definido
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
   //variáveis locais
   double  width;
   VTArea *area_visivel;

   //assume valor default
   delta = 1;
   //verifica se o objeto VTGraf está definido
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
   //acha Eqbar da rede na área
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
   //proteção
//   if ((redes->LisRede())->Count == 0) return(lis);
   //determina delta
   DefineDelta();
   //acha Barra, Eqpbar, Ligação, Mutua, Reducao na área
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
   //variáveis locais
   bool visible = true;

   //reinicia lis
   lis->Clear();
   //inicia lista c/ Ligações conectadas à Barra
   Ligacao(pbar, visible);
   //inclui todos os Eqptos de barra ligados à barra
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
   //determina Comentarios na área
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
   //determina Links na área
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
   //determina Retangulos na área
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
   //determina Retangulos na área
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
   //acha Geradores da rede na área
   AchaEqbar(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoGERADOR));
   //acha Geradores isolados na área
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
   //determina Ligações das Redes na área
   AchaLigacao(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoLIGACAO));
   //determina Ligações isoladas na área
   //AchaLigacao(consulta->LisEqptoIsolado(redes, eqptoLIGACAO));
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Ligacao(VTBarra *pbar, bool visible)
   {
   //variáveis locais
   TList     *lisLIG;
   VTLigacao *ligacao;

   //detemina as Ligações conectadas à Barra
   if (visible)
      {//somente Ligações visíveis
      lisLIG = consulta->LisEqptoRedeReducaoVisible(redes, eqptoLIGACAO);
      }
   else
      {//todas Ligações, visíveis ou não
      lisLIG = consulta->LisEqptoRedeReducao(redes, eqptoLIGACAO);
      }
   //descarta Ligacoes que não conectam a Barra
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
   //variáveis locais
   VTBarra   *pbar;
   VTEqpto   *eqpto;
   VTLigacao *pliga;
   TList     *lisLIG;

   //reinicia lis
   lis->Clear();
   //determina área definida pelas Barras
   if (! redegraf->Area(lisBAR, area)) return(lis);
   //obtém lista de Ligações das Redes na área
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
   //variáveis locais
   VTLigacao *ligacao;
   VTReducao *reducao;
   TList     *lisREDUCAO;
   TList     *lisLIG_FIC;

   //limpa lista
   lis->Clear();
   //acha Reducoes que conectam a Barra
   lisREDUCAO = consulta->LisReducaoCarregada(redes);
   //determina Ligacoes fictícias que conectam a Barra
   for (int nr = 0; nr < lisREDUCAO->Count; nr++)
      {
      reducao = (VTReducao*)lisREDUCAO->Items[nr];
      //obtém lista de Ligacoes do esquemático da Reducao
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
   //variáveis locais
   VTLigacao *pliga;

   //detemina Ligações da área, incluindo Chaves
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
//função específica p/ editor: TMarca não é um eqpto da RedeSUb
//---------------------------------------------------------------------------
VTMarca* __fastcall TBusca::Marca(TList *lisMAR, VTArea *area)
   {
   //variáveis locais
   VTMarca *marca;

   //salva ponteiro para Area
   this->area = area;
   //determina delta
   DefineDelta();
   delta = 3*delta;
   //procura Marca na área
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
   //determina Ligações das Redes na área
   AchaMutua(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoMUTUA));
   return(lis);
   }

//---------------------------------------------------------------------------
TList* __fastcall TBusca::Mutua(VTBarra *pbar)
   {
   //variáveis locais
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
   //acha Reducoes na área
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
   //acha Reducoes na área
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
   //acha Suprimentos da rede na área
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
   //determina Trafos na área
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
   //determina Trafo3E na área
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
   //determina Trechos na área
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
   //acha Cargas da rede na área
	AchaEqbar(consulta->LisEqptoRedeReducaoVisible(redes, area, eqptoYREF));
   return(lis);
   }

//---------------------------------------------------------------------------
//eof
