//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TUnifCurto.h"
#include "VTAnimacao.h"
#include "VTDraw.h"
#include "TPadrao.h"
#include "TFormGraf.h"
#include "VTGrafico.h"
#include "VTGrid.h"
#include "..\Apl\VTApl.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTReducao.h"
#include "..\Rede\VTTrafo3E.h"
#include "..\Rede\VTTrecho.h"
#include "..\RedeCC\VTLigCC.h"
#include "..\RedeCC\VTRedeCC.h"
#include "..\Redegraf\VTCoordenada.h"
#include "..\Redegraf\VTRedegraf.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
// função global
//-----------------------------------------------------------------------------
VTUnif* NewObjUnifCurto(VTApl *apl, int tipo_graf, AnsiString nome)
   {
   return(new TUnifCurto(apl, tipo_graf, nome));
   }

//---------------------------------------------------------------------------
// TUnifCurto::TUnifCurto() - construtor
//---------------------------------------------------------------------------
__fastcall TUnifCurto::TUnifCurto(VTApl *apl, int tipo_graf, AnsiString nome)
                      :TUnifEditor(apl, tipo_graf, nome)
   {
   //substitui o objeto Draw por DrawCurto
   if (draw) delete draw;
   draw = NewObjDrawCurto(apl);
   }

//---------------------------------------------------------------------------
// TUnifCurto::~TUnifCurto() - destrutor
//---------------------------------------------------------------------------
__fastcall TUnifCurto::~TUnifCurto(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TUnifCurto::Redraw(VTEdt *editor)
   {
   //obtém objeto RedeCC
   redeCC = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
   //desabilita animação
   animacao->Stop();
   //indica que os círculos da animação não precisam ser apagados
   redraw_animacao.apagar = false;
   //utiliza método da classe base
   TUnifEditor::Redraw(editor);
   //desenha marca no ponto de defeito
   RedrawPontoDefeito();
   //inicia animação
   if (animacao->Enabled) RedrawAnimacao();
   //atualiza gráfico
   form->RedrawTelaReal();
   //desenha borda
   //RedrawBordas();
   //reabilita animação
   animacao->Start(this);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifCurto::Redraw(int op, VTEdt *editor)
   {
   //obtém objeto RedeCC
   redeCC = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
   //utiliza método da classe base
   TUnifEditor::Redraw(op, editor);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifCurto::RedrawAnimacao(void)
   {
   //variáveis locais
   VTLigCC   *ligCC;
   VTLigacao *ligacao;
   VTReducao *reducao;
   //VTTrafo3E *ptrf3e;
   TList     *lisLIG;
   TList     *lisLIG_CC;
   TList     *lisREDU;

   //obtém objeto RedeCC
   redeCC = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
   if (redeCC == NULL) return;
   //determina lista de Ligacoes da RedeCC
   lisLIG_CC = redeCC->LisLigacaoCC();
   if (lisLIG_CC->Count == 0) return;
   //loop p/ todas Ligacoes
   for (int nl = 0; nl < lisLIG_CC->Count; nl++)
      {
      ligCC = (VTLigCC*)lisLIG_CC->Items[nl];
      //verifica se é uma Ligacao de Reducao (verifica se não pertence a uma Rede)
      if (ligCC->Ligacao->rede == NULL) continue;
      if (! ligCC->Ligacao->rede->Visible()) continue;
      RedrawAnimacao(ligCC);
      }
   //loop p/ todas Reducoes
   lisREDU = consulta->LisReducaoCarregada(redes);
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      reducao = (VTReducao*)lisREDU->Items[nr];
      //verifica se a Redução está visível
      if (! reducao->Visible()) continue;
      //verifica se está exibindo o esquemático das reducoes
      if (reducao->Show[rdu_EQPTOS])
         {//determina lista de Ligações da Reducao
         lisLIG = reducao->LisLigacao();
         for (int nl = 0; nl < lisLIG->Count; nl++)
            {
            ligacao = (VTLigacao*)lisLIG->Items[nl];
            ligCC   = (VTLigCC*)(ligacao->Obj);
            if (ligCC) RedrawAnimacao(ligCC);
            }
         }
      else
         {//determina lista de Ligacoes do esquemático da Reducao
         lisLIG = reducao->LisLigacaoEsquematico();
         for (int nl = 0; nl < lisLIG->Count; nl++)
            {
            ligacao = (VTLigacao*)lisLIG->Items[nl];
            ligCC   = (VTLigCC*)(ligacao->Obj);
            if (ligCC) RedrawAnimacao(ligCC);
            }
         }
      }
   redraw_animacao.apagar = true;
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifCurto::RedrawAnimacao(VTLigCC *ligCC)
   {
   //proteção
   if (ligCC == NULL) return;
   //verifica tipo da Ligacao
   switch(ligCC->Ligacao->Tipo())
      {
      case eqptoTRECHO:
         RedrawAnimacaoTrecho(ligCC);
         break;
      case eqptoTRAFO3E: //Trafo3E: Ligação c/ 3 Barras
         RedrawAnimacaoB1B2B3(ligCC);
         break;
      default: //demais Ligações: 2 Barras
         RedrawAnimacaoB1B2(ligCC);
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifCurto::RedrawAnimacaoB1B2(VTLigCC *ligCC)
   {
   //variáveis locais
   int        ind_def;
   int        ind_bar = 0;
   int        sentido;
   VTLigacao *pliga = ligCC->Ligacao;
   VTCoordenada *coordenada;

   //determina defeito a ser usado
   ind_def = redeCC->ShowDefeito;
   //determina sentido da corrente de curto trifásico na Barra 1 da Ligação
   sentido = ligCC->Sentido[ind_def][ind_bar];
   if ((sentido == fluxoINDEF)||(sentido == fluxoNENHUM)) return;
   //determina coordenadas da Ligacao
   coordenada = redegraf->CoordLigacao[pliga];
   if (coordenada->Line == NULL) return;
   //apresenta caracteres de animação
   RedrawAnimacaoX1Y1X2Y2(coordenada->Line->x1, coordenada->Line->y1,
                          coordenada->Line->x2, coordenada->Line->y2,
                          sentido);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifCurto::RedrawAnimacaoB1B2B3(VTLigCC *ligCC)
   {
   //variáveis locais
   int          ind_def;
   VTLigacao    *pliga = ligCC->Ligacao;
   VTCoordenada *coordenada;

   //determina defeito a ser usado
   ind_def = redeCC->ShowDefeito;
   //obtém coordenadas da Ligação
   coordenada = redegraf->CoordTrafo3E[(VTTrafo3E*)pliga];
   if (coordenada->Line == NULL) return;
   //apresenta animação p/ os 3 segmentos
   RedrawAnimacaoX1Y1X2Y2(coordenada->Line->x1, coordenada->Line->y1,
                          coordenada->Line->xm, coordenada->Line->ym,
                          ligCC->Sentido[ind_def][0]);
   RedrawAnimacaoX1Y1X2Y2(coordenada->Line->x2, coordenada->Line->y2,
                          coordenada->Line->xm, coordenada->Line->ym,
                          ligCC->Sentido[ind_def][1]);
   RedrawAnimacaoX1Y1X2Y2(coordenada->Line->x3, coordenada->Line->y3,
                          coordenada->Line->xm, coordenada->Line->ym,
                          ligCC->Sentido[ind_def][2]);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifCurto::RedrawAnimacaoX1Y1X2Y2(int x1, int y1, int x2, int y2, int sentido)
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
void __fastcall TUnifCurto::RedrawAnimacaoTrecho(VTLigCC *ligCC)
   {
   //variáveis locais
   int          ind_def;
   int          ind_bar = 0;
   int          sentido;

   //determina defeito a ser usado
   ind_def = redeCC->ShowDefeito;
   //determina sentido da corrente de Curto trifásico na Barra 1 da Ligação
   sentido = ligCC->Sentido[ind_def][ind_bar];
   if ((sentido == fluxoINDEF)||(sentido == fluxoNENHUM)) return;
   //desenha animação entre as Barras do Trecho
   RedrawAnimacaoB1B2(ligCC);
   }

//-----------------------------------------------------------------------------
void __fastcall TUnifCurto::RedrawPontoDefeito(void)
   {
   //variáveis locais
   int          x, y;
   VTBarra      *barra;
   VTCoordenada *coordenada;

   //protecao
   if (redeCC == NULL) return;
   //identifica a barra de defeito c/ uma cruz
   if ((barra = redeCC->BarraDefeito()) == NULL) return;
   //determina coordenadas da Barra
   coordenada = redegraf->CoordBarra[barra];
   if (coordenada->Line == NULL) return;
   form->DrawCruz((coordenada->Line->x1+coordenada->Line->x2)/2,
                  (coordenada->Line->y1+coordenada->Line->y2)/2, 10, clRed);
   }

//---------------------------------------------------------------------------
//eof

