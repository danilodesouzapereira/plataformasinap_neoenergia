//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TUnifFlow.h"
#include "VTAnimacao.h"
#include "VTDraw.h"
#include "TPadrao.h"
#include "TFormGraf.h"
#include "VTGrafico.h"
#include "VTGrid.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Geral\VTGeral.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCapserie.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTGerador.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTResFlowSup.h"
#include "..\Rede\VTRegulador.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Redegraf\VTCoordenada.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\Rede\VTTrecho.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
// fun��o global
//-----------------------------------------------------------------------------
VTUnif* NewObjUnifFlow(VTApl *apl, int tipo_graf, AnsiString nome)
   {
   return(new TUnifFlow(apl, tipo_graf, nome));
   }

//---------------------------------------------------------------------------
// TUnifFlow::TUnifFlow() - construtor
//---------------------------------------------------------------------------
__fastcall TUnifFlow::TUnifFlow(VTApl *apl, int tipo_graf, AnsiString nome)
                     :TUnifEditor(apl, tipo_graf, nome)
   {
   //substitui o objeto Draw por DrawFlow
   if (draw) delete draw;
   draw = NewObjDrawFlow(apl);
   }

//---------------------------------------------------------------------------
// TUnifFlow::~TUnifFlow() - destrutor
//---------------------------------------------------------------------------
__fastcall TUnifFlow::~TUnifFlow(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TUnifFlow::Redraw(VTEdt *editor)
   {
   //desabilita anima��o
   animacao->Stop();
   //utiliza m�todo da classe base
   TUnifEditor::Redraw(editor);
   //atualiza anima��o
   if (animacao->Enabled) RedrawAnimacao();
   //atualiza gr�fico
   form->RedrawTelaReal();
   //reabilita anima��o
   animacao->Start(this);
   }

//---------------------------------------------------------------------------
void __fastcall TUnifFlow::Redraw(int op, VTEdt *edt)
   {
   //utiliza m�todo da classe base
   TUnifEditor::Redraw(op, edt);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFlow::RedrawAllBarra(void)
   {
   //vari�veis locais
   int       ind_cor;
   TColor    cor;
   VTBarra   *pbarra;
   TList     *lisBAR;

   //determina lista de Barras
   if ((lisBAR = LisEqptoRede(eqptoBARRA)) != NULL)
      {//loop p/ todas as Barras
      for (int nbar = 0; nbar < lisBAR->Count; nbar++)
         {
         pbarra  = (VTBarra*)lisBAR->Items[nbar];
         ind_cor = pbarra->resflow->Diagnostico[padrao->flow.num_pat];
         cor     = geral->COR.Faixa[ind_cor];
         RedrawBarra(pbarra, cor);
         //identifica Barra swing
         //if (pbarra->flow.swing) RedrawBarraSwing(pbarra);
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFlow::RedrawAllCapserie(void)
   {
   //vari�veis locais
   int        ind_cor;
   TColor     cor;
   VTCapserie *capserie;
   TList      *lisEQP;

   //determina lista de Capserie
   if ((lisEQP = LisEqptoRede(eqptoCAPSERIE)) != NULL)
      {//loop p/ todos Capserie
      for (int n = 0; n < lisEQP->Count; n++)
         {
         capserie = (VTCapserie*)lisEQP->Items[n];
         ind_cor = capserie->resflow->Diagnostico[padrao->flow.num_pat];
         cor     = geral->COR.Faixa[ind_cor];
         RedrawCapserie(capserie, cor);
         if ((capserie->Show[lig_PIE])&&(ind_cor != diagISOLADO)) RedrawPie(capserie, cor);
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFlow::RedrawAllChave(void)
   {
   //vari�veis locais
   int       ind_cor;
   TColor    cor;
   VTChave   *pchave;
   TList     *lisCHV;

   //determina lista de Chaves
   if ((lisCHV = LisEqptoRede(eqptoCHAVE)) != NULL)
      {//loop p/ todas as Chaves
      for (int nchv = 0; nchv < lisCHV->Count; nchv++)
         {
         pchave = (VTChave*)lisCHV->Items[nchv];
         if (pchave->Aberta)
            {//chave aberta
            RedrawChave(pchave, padrao->isolado.cor);
            }
         else
            {//chave fechada
            ind_cor = pchave->resflow->Diagnostico[padrao->flow.num_pat];
            cor     = geral->COR.Faixa[ind_cor];
            RedrawChave(pchave, cor);
            if (pchave->Show[lig_PIE]) RedrawPie(pchave, cor);
            }
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFlow::RedrawAllGerador(void)
   {
   //vari�veis locais
   int       ind_cor;
   double    cargto;
   TColor    cor;
   VTGerador *pger;
   TList     *lisGER;

   //assume cor padr�o do Gerador
   //cor = padrao->ger.cor;
   //determina lista de Geradores
   if ((lisGER = LisEqptoRede(eqptoGERADOR)) != NULL)
      {//loop p/ todos Geradores
      for (int n = 0; n < lisGER->Count; n++)
         {
         pger    = (VTGerador*)lisGER->Items[n];
         cargto  = pger->resflow->Cargto_pu[padrao->flow.num_pat];
         ind_cor = pger->resflow->Diagnostico[padrao->flow.num_pat];
         cor     = geral->COR.Faixa[ind_cor];
         RedrawGerador(pger, cor);
         if ((pger->Show[ger_PIE])&&(ind_cor != diagISOLADO)) RedrawPie(pger, cargto, cor);
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFlow::RedrawAllSuprimento(void)
   {
   //vari�veis locais
   int          ind_cor;
   double       cargto;
   TColor       cor;
   VTSuprimento *psup;
   TList        *lisSUP;

   //assume cor padr�o do Suprimento
   //cor = padrao->sup.cor;
   //determina lista de Suprimentos
   if ((lisSUP = LisEqptoRede(eqptoSUPRIMENTO)) != NULL)
      {//loop p/ todos Suprimentos
      for (int n = 0; n < lisSUP->Count; n++)
         {
         psup = (VTSuprimento*)lisSUP->Items[n];
         cargto  = psup->resflow->Cargto_pu[padrao->flow.num_pat];
         ind_cor = psup->resflow->Diagnostico[padrao->flow.num_pat];
         cor     = geral->COR.Faixa[ind_cor];
         RedrawSuprimento(psup, cor);
         if ((psup->Show[sup_PIE])&&(ind_cor != diagISOLADO)) RedrawPie(psup, cargto, cor);
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFlow::RedrawAllRegulador(void)
   {
   //vari�veis locais
   int          ind_cor;
   TColor       cor;
   VTRegulador *regulador;
   TList       *lisEQP;

   //determina lista de Regulador
   if ((lisEQP = LisEqptoRede(eqptoREGULADOR)) != NULL)
      {//loop p/ todos Regulador
      for (int n = 0; n < lisEQP->Count; n++)
         {
         regulador = (VTRegulador*)lisEQP->Items[n];
         ind_cor   = regulador->resflow->Diagnostico[padrao->flow.num_pat];
         cor       = geral->COR.Faixa[ind_cor];
         RedrawRegulador(regulador, Color(regulador, cor));
         if ((regulador->Show[lig_PIE])&&(ind_cor != diagISOLADO)) RedrawPie(regulador, cor);
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFlow::RedrawAllTrafo(void)
   {
   //vari�veis locais
   int    ind_cor;
   TColor cor;
   VTTrafo *ptrafo;
   TList  *lisTRF;

   //determina lista de Trafos
   if ((lisTRF = LisEqptoRede(eqptoTRAFO)) != NULL)
      {//loop p/ todos Trafos
      for (int ntrf = 0; ntrf < lisTRF->Count; ntrf++)
         {
         ptrafo  = (VTTrafo*)lisTRF->Items[ntrf];
         ind_cor = ptrafo->resflow->Diagnostico[padrao->flow.num_pat];
         cor     = geral->COR.Faixa[ind_cor];
         RedrawTrafo(ptrafo, cor);
         if ((ptrafo->Show[lig_PIE])&&(ind_cor != diagISOLADO)) RedrawPie(ptrafo, cor);
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFlow::RedrawAllTrafo3E(void)
   {
   //vari�veis locais
   int       ind_cor;
   TColor    cor;
   VTTrafo3E  *ptrf3e;
   TList     *lisT3E;

   //determina lista de Trafo3E
   if ((lisT3E = LisEqptoRede(eqptoTRAFO3E)) != NULL)
      {//loop p/ todas os Trafo3E
      for (int ntrf = 0; ntrf < lisT3E->Count; ntrf++)
         {
         ptrf3e = (VTTrafo3E*)lisT3E->Items[ntrf];
         ind_cor = ptrf3e->resflow->Diagnostico[padrao->flow.num_pat];
         cor     = geral->COR.Faixa[ind_cor];
         RedrawTrafo3E(ptrf3e, cor);
         if ((ptrf3e->Show[lig_PIE])&&(ind_cor != diagISOLADO)) RedrawPie(ptrf3e, cor);
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFlow::RedrawAllTrecho(void)
   {
   //vari�veis locais
   int       ind_cor;
   TColor    cor;
   VTTrecho   *ptrecho;
   TList     *lisTRE;

   //determina lista de Trechos
   if ((lisTRE = LisEqptoRede(eqptoTRECHO)) != NULL)
      {//loop p/ todos Trechos
      for (int ntre = 0; ntre < lisTRE->Count; ntre++)
         {
         ptrecho = (VTTrecho*)lisTRE->Items[ntre];
         ind_cor = ptrecho->resflow->Diagnostico[padrao->flow.num_pat];
         cor     = geral->COR.Faixa[ind_cor];
         RedrawTrecho(ptrecho, cor);
         if ((ptrecho->Show[lig_PIE])&&(ind_cor != diagISOLADO)) RedrawPie(ptrecho, cor);
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFlow::RedrawAnimacao(void)
   {
   //vari�veis locais
   VTLigacao *pliga;
   VTMRede   *mrede;
   VTReducao *reducao;
   VTTrafo3E *ptrf3e;
   TList     *lisLIG;
   TList     *lisREDU;
   TList     *lisMR = redes->LisMRede();

   //determina lista de Liga��es da Rede
   lisLIG = LisEqptoRede(eqptoLIGACAO);
   //loop p/ todas Liga��es
   for (int nlig = 0; nlig < lisLIG->Count; nlig++)
      {
      pliga = (VTLigacao*)lisLIG->Items[nlig];
      //verifica se deve ser desenhada a anima��o p/ o Trecho
      if (! ValidaEqptoPlaneja(pliga)) continue;
      //verifica tipo da Ligacao
      switch(pliga->Tipo())
         {
         case eqptoTRECHO:
            RedrawAnimacao((VTTrecho*)pliga);
            break;
         case eqptoTRAFO3E: //Trafo3E: Liga��o c/ 3 Barras
            RedrawAnimacaoB1B2B3(pliga);
            break;
         default: //demais Liga��es: 2 Barras
            RedrawAnimacaoB1B2(pliga);
         }
      }
   //loop p/ todas MRedes
   for (int n = 0; n < lisMR->Count; n++)
      {
      mrede = (VTMRede*)lisMR->Items[n];
      if (mrede->Carregada) continue;
      //OBS: uma MRede n�o carregada possui uma �nica Reducao
      lisREDU = mrede->LisReducao();
      if (lisREDU->Count == 0) continue;
      reducao = (VTReducao*)(lisREDU->First());
      //verifica se deve exibir a Reducao
      if (! reducao->Visible()) continue;
      lisLIG = LisEqptoReducao(reducao, eqptoLIGACAO);
      //loop p/ todas Liga��es
      for (int nlig = 0; nlig < lisLIG->Count; nlig++)
         {//verifica se � um Trafo3E
         pliga = (VTLigacao*)lisLIG->Items[nlig];
         //verifica se deve ser desenhada a anima��o p/ a Ligacao
         if (! ValidaEqptoPlaneja(pliga)) continue;
         if (pliga->Tipo() == eqptoTRAFO3E)
            {//Trafo3E: Liga��o c/ 3 Barras
            RedrawAnimacaoB1B2B3(pliga);
            }
         else
            {//demais Liga��es: 2 Barras
            RedrawAnimacaoB1B2(pliga);
            }
         }
      }
   redraw_animacao.apagar = true;
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFlow::RedrawAnimacao(VTTrecho *ptrecho)
   {
   //vari�veis locais
   int          nb = 0;
   int          sentido;

   //determina sentido do fluxo  na Barra 1 da Liga��o
   sentido = ptrecho->resflow->SentidoP[nb][padrao->flow.num_pat];
   if ((sentido == fluxoINDEF)||(sentido == fluxoNENHUM)) return;
   //desenha anima��o entre as Barras do Trecho
   RedrawAnimacaoB1B2(ptrecho);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFlow::RedrawAnimacaoB1B2(VTLigacao *pliga)
   {
   //vari�veis locais
   int          nb = 0;
   int          sentido;
   VTCoordenada *coordenada;

   //determina sentido do fluxo  na Barra 1 da Liga��o
   sentido = pliga->resflow->SentidoP[nb][padrao->flow.num_pat];
   if ((sentido == fluxoINDEF)||(sentido == fluxoNENHUM)) return;
   //determina coordenadas da Ligacao
   coordenada = redegraf->CoordLigacao[pliga];
   if (coordenada->Line == NULL) return;
   //apresenta caracteres de anima��o
   RedrawAnimacaoX1Y1X2Y2(coordenada->Line->x1, coordenada->Line->y1,
                          coordenada->Line->x2, coordenada->Line->y2,
                          sentido);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFlow::RedrawAnimacaoB1B2B3(VTLigacao *pliga)
   {
   //vari�veis locais
   VTCoordenada *coordenada;

   //obt�m coordenadas da Liga��o
   coordenada = redegraf->CoordTrafo3E[(VTTrafo3E*)pliga];
   if (coordenada->Line == NULL) return;
   //apresenta anima��o p/ os 3 segmentos
   RedrawAnimacaoX1Y1X2Y2(coordenada->Line->x1, coordenada->Line->y1,
                          coordenada->Line->xm, coordenada->Line->ym,
                          pliga->resflow->SentidoP[0][padrao->flow.num_pat]);
   RedrawAnimacaoX1Y1X2Y2(coordenada->Line->x2, coordenada->Line->y2,
                          coordenada->Line->xm, coordenada->Line->ym,
                          pliga->resflow->SentidoP[1][padrao->flow.num_pat]);
   RedrawAnimacaoX1Y1X2Y2(coordenada->Line->x3, coordenada->Line->y3,
                          coordenada->Line->xm, coordenada->Line->ym,
                          pliga->resflow->SentidoP[2][padrao->flow.num_pat]);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFlow::RedrawAnimacaoX1Y1X2Y2(int x1, int y1, int x2, int y2, int sentido)
   {
   //desenha caracteres p/ animacao de acordo c/ o sentido do fluxo
   switch(sentido)
      {
      case fluxoPOS:
         form->DrawCirculoXor(x1, y1, x2, y2, redraw_animacao.postre, 2);
         break;
      case fluxoNEG:
         form->DrawCirculoXor(x2, y2, x1, y1, redraw_animacao.postre, 2);
         break;
      default: //nada a fazer
         break;
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFlow::RedrawPie(VTEqbar *eqbar, double cargto, TColor cor)
   {
   //vari�veis locais
   double       alfa;
   int          x, y, dx, dy, desloc;
   int          raio = 15;
   VTCoordenada *coordenada;

   //determina coordenadas de Eqbar
   coordenada = redegraf->CoordEqbar[eqbar];
   if (coordenada->Line == NULL) return;
   //define coordenadas do centro da pizza
   x = coordenada->Line->x1;
   y = coordenada->Line->y1;
   //calcula valor do deslocamento a ser aplicado ao centro da pizza
   if (eqbar->pbarra->Vertical())
      {//barra vertical: determina coordenadas do centro da pizza
      dx = 0;
      dy = -(raio+raio);
      }
   else
      {//barra horizontal
      dx = -(raio+raio);
      dy = 0;
      }
   //define �ngulo da fatia a ser preenchida
   //if (cargto < 1.)
   if (cargto < 0.99) //ajuste fino no desenho da pizza
      alfa = (2.0 *  M_PI) * cargto;
   else
      alfa = 2.0 *  M_PI;
   //desenha pizza
   form->DrawPie(x, y, dx, dy, raio, alfa, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFlow::RedrawPie(VTLigacao *pliga, TColor cor)
   {
   //vari�veis locais
   double       alfa;
   int          x, y, dx, dy, desloc;
   int          x1, y1, x2, y2;
   int          raio = 15;
   VTCoordenada *coordenada;

   //determina coordenadas da Ligacao
   coordenada = redegraf->CoordEqpto[pliga];
   if (coordenada->Line == NULL) return;
   //inicia coordenadas (x1,y1) e (x2,y2) do segemnto de refer�ncia p/ a pizza
   if (coordenada->Line)
      {
      x1 = coordenada->Line->x1; y1 = coordenada->Line->y1;
      x2 = coordenada->Line->x2; y2 = coordenada->Line->y2;
      }
   else if (coordenada->Polyline)
      {
      x1 = coordenada->Polyline->point[0].x; y1 = coordenada->Polyline->point[0].y;
      x2 = coordenada->Polyline->point[1].x; y2 = coordenada->Polyline->point[1].y;
      }
   //define coordenadas do centro da pizza
   x = (x1 + x2) / 2;
   y = (y1 + y2) / 2;
   //calcula valor do deslocamento a ser aplicado ao centro da pizza p/ cada tipo de liga��o
   switch(pliga->Tipo())
      {
      case eqptoTRAFO:   desloc = raio+15; break;
      case eqptoTRAFO3E: desloc = raio+15; break;
      case eqptoTRECHO:  desloc = raio+10; break;
      case eqptoCHAVE:   desloc = raio+18; break;
      }
   //define deslocamento a ser considerado p/ o centro
   if (x1 == x2)
      {//Liga��o vertical
      dy = 0;
      dx = (y2 > y1) ? desloc : - desloc;
      }
   else if (y1 == y2)
      {//Liga��o horizontal
      dx = 0;
      dy = (x2 > x1) ? -desloc : desloc;
      }
   else if (x1 < x2)
      {//Liga��o inclinada (quadrante 1 ou 4)
      dx = (y1 < y2) ? desloc : -desloc;
      dy = -desloc;
      }
   else
      {//Liga��o inclinada (quadrante 2 ou 3)
      dx = (y1 < y2) ? desloc : -desloc;
      dy =  desloc;
      }
   //define �ngulo da fatia a ser preenchida
   //if (pliga->padrao->flow.cargto[padrao->flow.num_pat] < 1.)
   if (pliga->resflow->Cargto_pu[padrao->flow.num_pat] < 0.99) //ajuste fino no desenho da pizza
      alfa = (2.0 *  M_PI) * pliga->resflow->Cargto_pu[padrao->flow.num_pat];
   else
      alfa = 2.0 *  M_PI;
   //desenha pizza
   form->DrawPie(x, y, dx, dy, raio, alfa, cor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFlow::RedrawReducaoEqptos(VTReducao *reducao, TColor cor)
   {
   //vari�veis locais
   int          ind_cor;
   VTBarra      *pbarra;
   VTSuprimento *psup;
   VTTrafo      *ptrafo;
   VTTrecho     *ptrecho;
   VTEqpto      *eqpto;
   TList        *lisEQP;

   //obt�m lista de todos objetos VEqpto da Reducao
   lisEQP = LisEqptoReducao(reducao);
   //loop p/ todos objetos VEqpto
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      switch(eqpto->Tipo())
         {
         case eqptoCARGA:
            RedrawCarga((VTCarga*)eqpto, Color(eqpto, clWhite));
            break;
         case eqptoSUPRIMENTO:
            psup = (VTSuprimento*)eqpto;
            ind_cor = psup->resflow->Diagnostico[padrao->flow.num_pat];
            cor     = geral->COR.Faixa[ind_cor];
            RedrawSuprimento(psup, cor);
            break;
         case eqptoTRAFO:
            ptrafo  = (VTTrafo*)eqpto;
            ind_cor = ptrafo->resflow->Diagnostico[padrao->flow.num_pat];
            cor     = geral->COR.Faixa[ind_cor];
            RedrawTrafo(ptrafo, cor);
            break;
         case eqptoTRECHO:
            ptrecho = (VTTrecho*)eqpto;
            ind_cor = ptrecho->resflow->Diagnostico[padrao->flow.num_pat];
            cor     = geral->COR.Faixa[ind_cor];
            RedrawTrecho(ptrecho, cor);
            break;
         case eqptoYREF:
            RedrawYref((VTYref*)eqpto, Color(eqpto, clWhite));
            break;
         }
      }
   //determina lista de Barras da Reducao
   if ((lisEQP = LisEqptoReducao(reducao, eqptoBARRA)) != NULL)
      {//loop p/ todas as Barras
      for (int nbar = 0; nbar < lisEQP->Count; nbar++)
         {
         pbarra  = (VTBarra*)lisEQP->Items[nbar];
         ind_cor = pbarra->resflow->Diagnostico[padrao->flow.num_pat];
         cor     = geral->COR.Faixa[ind_cor];
         RedrawBarra(pbarra, cor);
         //identifica Barra swing
         //if (pbarra->flow.swing) RedrawBarraSwing(pbarra);
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifFlow::RedrawReducaoEsquematico(VTReducao *reducao, TColor cor)
   {
   //vari�veis locais
   TList     *lisEQP;
   VTEqpto   *eqpto;
   VTTrecho  *trecho;

   //obt�m lista de todas Ligacoes do esquem�tico do esquem�tico das Reducoes vis�veis
   //lisEQP = reducao->LisLigacaoEsquematico();
   lisEQP = LisEqptoReducao(reducao, eqptoTRECHO);
   if (lisEQP->Count == 0) return;
   //loop p/ todos objetos VEqpto
   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      if (eqpto->Tipo() != eqptoTRECHO) continue;
      trecho = (VTTrecho*)eqpto;
      RedrawTrecho(trecho, Color(trecho, cor));
      }
   //exibe nome da Rede reduzida
   draw->RedeNome(reducao->Codigo, reducao->BarraFicticia(), trecho, cor);
   }

//---------------------------------------------------------------------------
//eof

