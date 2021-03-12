//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <Math.hpp>
#include <algorithm>
#include "TFormProtecao.h"
#include "TAplProtecao.h"
#include "TAngulos.h"
#include "VTFasor.h"
#include "TFasorAjuste.h"
#include "VTFasores.h"
#include "VTGrupo.h"
#include "..\Complexo\Complexo.h"
#include "..\Diretorio\VTPath.h"
//#include "..\Edita\VTEdita.h"
#include "..\Editor\VTEdt.h"
#include "..\Flow\VTFlow.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Report\VTReport.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTChave.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTrecho.h"
#include "..\RedeCC\VTRedeCC.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Report.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"
using namespace std;

//---------------------------------------------------------------------------
__fastcall TFormProtecao::TFormProtecao(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
                          : TFormEdt(Owner, parent)
   {
   //variáveis locais
   //VTEdita   *edita   = (VTEdita*)apl_owner->GetObject(__classid(VTEdita));

   try{//cria objeto TAplSpy
      apl      = new TAplProtecao(this, apl_owner);
      fasores  = (VTFasores*)apl->GetObject(__classid(VTFasores));
      lisEQP   = new TList();
      //cria objeto Angulos
      ang = new TAngulos();
      //inicia valores default para ângulos de ajuste
      IniciaAngulos();
      //inicia CBoxPatamar
      IniciaCBoxPatamar();
      //inicia propriedades do Form p/ desenhar os fasores
      FormIniciaPropriedades();
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
__fastcall TFormProtecao::~TFormProtecao(void)
   {
   //variáveis locais
   //VTEdita   *edita   = (VTEdita*)apl->GetObject(__classid(VTEdita));
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTGeral   *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //apaga moldura
   if (grafico) grafico->MolduraSolida();
   //if (grafico) grafico->DestacaEqpto(NULL, clBlack);
   //retira ActionRedeChanged de VTEdita
   //if (edita) edita->ActionOnChangeRedeRetira(ActionRedeChanged);
   //destrói objetos
   if (edt)   {delete edt;   edt   = NULL;}
   if (apl)   {delete apl;   apl   = NULL;}
   if (ang)   {delete ang;   ang   = NULL;}
//   if (formTabFasores) {delete formTabFasores; formTabFasores = NULL;}
   //libera memória alocada p/ bitmap de TImage
   if (graph_bmp) {delete graph_bmp; graph_bmp = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::ActionAnimacaoExecute(TObject *Sender)
   {
   //habilita/desabilita Timer
   Timer->Enabled = butAnimacao->Down;
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::ActionEdtExecute(TObject *Sender)
   {
   try{//se necessário, cria um objeto VTEdt
      if (edt != NULL) delete edt;
      //cria VTEdt p/ controle do mouse
      edt = DLL_NewObjEdtBusca(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
      //atualiza indicação de mouse ativo e força botão Down
      AtualizaIndicacaoEditorOn(butEdt);
      } catch (Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::ActionExportaExecute(TObject *Sender)
   {
   //variaveis locais
   AnsiString	nome_arq;
   VTReport 	*report;
   VTPath 		*path = (VTPath*)apl->GetObject(__classid(VTPath));

   //esconde form, para a janela Salvar Como não ficar por baixo
   Hide();
   try{//cria objeto report
      if ((report = DLL_NewObjReport(apl)) == NULL) return;
      //define pasta inicial
      SaveDialog->InitialDir = path->DirExporta();
      SaveDialog->FileName   = "Fasores.txt";
      if(SaveDialog->Execute())
         {//recebe nome do arquivo a ser exportado e exporta listView
         nome_arq = SaveDialog->FileName;
         if (report->ArquivoTexto(nome_arq, LViewFasor, "", false))
            Aviso("A tabela foi exportada corretamente.");
         else
            Erro("Erro ao exportar tabela.");
         }
      //destroi report
      DLL_DeleteObjReport(report);
      }catch(Exception &e)
         {//nada a fazer
         }
   //reapresenta form
   Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::ActionRedeChangedExecute(TObject *Sender)
   {
   //
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::ActionFasorSelAllExecute(TObject *Sender)
   {
   //variáveis locais
   VTFasor *fasor;

   //seleciona todos os items
   for (int n = 0; n < LViewFasor->Items->Count; n++)
      {
      LViewFasor->Items->Item[n]->Checked = true;
      fasor = (VTFasor*)(LViewFasor->Items->Item[n]->Data);
      if (fasor != NULL) fasor->visible = true;
      }
   //DVK 2014.10.31 insere fasores visiveis no LViewFasorGrafico
   LViewFasorVisivel();
   //atualiza gráfico
   DrawFasor();
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::ActionFasorSelNoneExecute(TObject *Sender)
   {
   //variáveis locais
   VTFasor *fasor;

   //seleciona todos os items
   for (int n = 0; n < LViewFasor->Items->Count; n++)
      {
      LViewFasor->Items->Item[n]->Checked = false;
      fasor = (VTFasor*)(LViewFasor->Items->Item[n]->Data);
      if (fasor != NULL) fasor->visible = false;
      }
   //DVK 2014.10.31 insere fasores visiveis no LViewFasorGrafico
   LViewFasorVisivel();
   //atualiza gráfico
   DrawFasor();
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::ActionGrupoSelAllExecute(TObject *Sender)
   {
   //variáveis locais
   VTGrupo *grupo;

   //cancela seleção de todos os items
   for (int n = 0; n < CLBoxGrupo->Items->Count; n++)
      {
      grupo = (VTGrupo*)(CLBoxGrupo->Items->Objects[n]);
      grupo->Visible         = true;
      CLBoxGrupo->Checked[n] = true;
      }
   //atrulaiza LViewFasor
   LViewFasorAtualiza();
   //atualiza gráfico
   DrawFasor();
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::ActionGrupoSelNoneExecute(TObject *Sender)
   {
   //variáveis locais
   VTGrupo *grupo;

   //cancela seleção de todos os items
   for (int n = 0; n < CLBoxGrupo->Items->Count; n++)
      {
      grupo = (VTGrupo*)(CLBoxGrupo->Items->Objects[n]);
      grupo->Visible         = false;
      CLBoxGrupo->Checked[n] = false;
      }
   //atrulaiza LViewFasor
   LViewFasorAtualiza();
   //atualiza gráfico
   DrawFasor();
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::ActionSelEqptoExecute(TObject *Sender)
   {
   //variáveis locais
   VTBarra   *barra;
   VTEqpto   *eqpto;
   TList     *lisTMP     = (TList*)Sender;
   VTFlow    *flow   = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTRedeCC  *redeCC = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));

   try{//verifica se existe a RedeCC
      if ((redeCC == NULL) && (! flow->UpToDate))
         {
         Aviso("Execute o cálculo de Fluxo de Potência e de Curto-Circuito.");
         return;
         }
      //determina Eqpto selecionado
      if (Sender == NULL) return;
      //loop p/ todos Eqptos recebidos em lisTMP (Sender)
      for (int n = 0; n < lisTMP->Count; n++)
         {
         if ((eqpto = (VTEqpto*)lisTMP->Items[n]) == NULL) continue;
         //permite selecionar Chave, Trafo ou Trafo3E
         if (eqpto->TipoLigacao()) break;
         eqpto = NULL;
         }
      //verifica se o eqpto é uma Chave ou trafo
      if (eqpto == NULL)
         {
         Aviso("Selecione chave, transformador (ou outro eqpto. de ligação entre barras) para análise.");
         return;
         }
      ligacao = (VTLigacao*)eqpto;
      GBoxEqpto->Caption = ligacao->TipoAsString();
      EditCodigo->Text   = ligacao->Codigo;
      //reinicia CBoxBarra com as Barras da Ligacao
      CBoxBarraInicia(ligacao);
      //determina Barra de referência para aa correntes da Chave
      barra = (VTBarra*)(CBoxBarra->Items->Objects[CBoxBarra->ItemIndex]);
      //cria os Fasores
      fasores->Reinicia();
      fasores->InsereFasores(barra, ligacao);
      //atualiza patamar e LView
      Atualiza();
      //Form está ativo: atualiza moldura
      Ativo();
      }catch(Exception &e)
         {
         //nada
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::Ativo(void)
   {
   //variáveis locais
   VTBarra   *barra;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //determina Barra de referência
   if (CBoxBarra->ItemIndex >= 0)
      {
      barra = (VTBarra*)(CBoxBarra->Items->Objects[CBoxBarra->ItemIndex]);
      }
   //exibe moldura na Chave e na Barra de referência
   lisEQP->Clear();
//   if (chave != NULL)   //DVK 2014.11.14
   if (ligacao != NULL)
      {
//      lisEQP->Add(chave); //DVK 2014.11.14
      lisEQP->Add(ligacao);
      if (barra) lisEQP->Add(barra);
      }
   if (grafico) grafico->MolduraSolida(lisEQP);
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::Atualiza(void)
   {
   //alê valores editados de ângulos de ajuste
   LeAngulosAjuste();
   //atualiza ângulos dos fasores
   fasores->DefineAnguloAjuste_rad(ang);
   //atualiza Fasores de acordo c/ patamar
   fasores->DefinePatamar(CBoxPatamar->ItemIndex);
   //reinicia CLBoxGrupo
   CLBoxGrupoInicia();
   //reinicia LViewFasor p/ exibir novos valores
   LViewFasorAtualiza();
   //atualilza o gráfico
   DrawFasor();
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::CBoxBarraChange(TObject *Sender)
   {
   //variáveis locais
   VTBarra   *barra;

   //proteção
   if (CBoxBarra->ItemIndex < 0) return;
   //determina Barra de referência
   barra = (VTBarra*)(CBoxBarra->Items->Objects[CBoxBarra->ItemIndex]);
   if (barra)
      {//redefine Barra de referência dos  Fasores
      fasores->RedefineBarraReferencia(barra);
      //atualiza LView
      LViewFasorAtualiza();
      //atualiza o gráfico
      DrawFasor();
      //atualiza Eqptos destacados no gráfico
      Ativo();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::CBoxBarraInicia(VTLigacao *ligacao)
   {
   //variáveis locais
   VTBarra *barra;

   //reinicia CBoxBarra
   CBoxBarra->Clear();
   for (int ind_bar = 0; ind_bar < NBAR_LIG; ind_bar++)
      {
      if ((barra = (VTBarra*)ligacao->Barra(ind_bar)) == NULL) continue;
      CBoxBarra->Items->AddObject(barra->Codigo, barra);
      }
   //pré-seleciona primeira Barra
   if (CBoxBarra->Items->Count > 0) CBoxBarra->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::CBoxPatamarChange(TObject *Sender)
   {
   //proteção
   if (CBoxPatamar->ItemIndex < 0) return;
   //recalcula os Fasores e atualiza a tela
   Atualiza();
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::CBoxUnidadeChange(TObject *Sender)
   {
   //atualiza LViewFasor
   LViewFasorAtualiza();
   //DVK 2014.11.06 atualiza LViewFasorGrafico
   LViewFasorVisivel();
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::CLBoxGrupoClickCheck(TObject *Sender)
   {
   //var locais
   VTGrupo *grupo;

   for(int n = 0; n < CLBoxGrupo->Items->Count; n++)
      {
      if ((grupo = (VTGrupo*)CLBoxGrupo->Items->Objects[n]) == NULL) continue;
      grupo->Visible = CLBoxGrupo->Checked[n];
      }
   //atualiza LViewFasor
   LViewFasorAtualiza();
   //atualiza gráfico
   DrawFasor();
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::CLBoxGrupoInicia(void)
   {
   //var locais
   int     index;
   VTGrupo *grupo;

   //limpa CLBox
   CLBoxGrupo->Clear();
   for(int ng = 0; ng < fasores->LisGrupo()->Count; ng++)
      {
      grupo = (VTGrupo*)fasores->LisGrupo()->Items[ng];
      //verifica se o Grupo possui fasor habilitado
      if (grupo->ExisteFasorEnabled())
         {
         index = CLBoxGrupo->Items->AddObject(grupo->Codigo, grupo);
         CLBoxGrupo->Checked[index] = grupo->Visible;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::LeAngulosAjuste(void)
   {
   if(! ang) return;
   try{
       ang->ajuste_A_seq0   = StrToDouble(EditAjusteA0->Text);
       ang->ajuste_B_seq0   = StrToDouble(EditAjusteB0->Text);
       ang->ajuste_A_outros = StrToDouble(EditAjusteA->Text);
       ang->ajuste_B_outros = StrToDouble(EditAjusteB->Text);
      }catch(Exception &e)
         {return;}   
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::DrawArea(int x0, int y0, TFasorAjuste *fasor)
   {
   //variáveis locais
   int               x,y,raio;
   int               x1,y1,x2,y2,x3,y3,x4,y4;
   double            ang_ini, ang_fim, escala;
   complex <double>  valor_ini, valor_fim;

   //define escala
   escala = (2. * fasor->dim) / (0.8 * min(ImageFasor->Width, ImageFasor->Height));
   raio   = RoundTo(fasor->raio/escala, 0) + 1;
   //limite da circunferência do Pie
   x1 = x0 - raio;
   x2 = x0 + raio;
   y1 = y0 - raio;
   y2 = y0 + raio;
   //determina ângulos da área de ajuste
   if(! fasor->ValorAnguloIni_rad(ang_ini)) return;
   if(! fasor->ValorAnguloFim_rad(ang_fim)) return;
   //determina valores, garantindo que valor_ini tem o menor ângulo
   valor_ini = polar(double(raio), ang_ini);
   valor_fim = polar(double(raio), ang_fim);
   //determina menor ângulodefine pontos (x3,y3) e (x4,y4)
   x3 = x0 + valor_ini.real();
   y3 = y0 - valor_ini.imag();
   x4 = x0 + valor_fim.real();
   y4 = y0 - valor_fim.imag();
   //configura cores
   ImageFasor->Canvas->Brush->Style = bsSolid;
   ImageFasor->Canvas->Brush->Color = clMoneyGreen;
   ImageFasor->Canvas->Pen->Color   = clWhite;
   ImageFasor->Canvas->Pie(x1, y1, x2, y2, x3, y3, x4, y4);
   //desenha eixos
   DrawEixos();
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::DrawCircunferencia(int x, int y, int raio, TColor cor)
   {
   //variáveis locais
   struct   {
            int    width;
            TColor color;
            } pen;
   struct   {
            TBrushStyle style;
            } brush;
   //salva configuração de TPen e TBrush
   pen.width   = ImageFasor->Canvas->Pen->Width;
   pen.color   = ImageFasor->Canvas->Pen->Color;
   brush.style = ImageFasor->Canvas->Brush->Style;
   //configura TPen e TBrush
   ImageFasor->Canvas->Pen->Width   = 1;
   ImageFasor->Canvas->Pen->Color   = cor;
   ImageFasor->Canvas->Brush->Style = bsClear;
   //desenha a circunferência
   ImageFasor->Canvas->Ellipse(x-raio, y-raio, x+raio, y+raio);
   //restaura configuração de TPen e TBrush
   ImageFasor->Canvas->Pen->Width   = pen.width;
   ImageFasor->Canvas->Pen->Color   = pen.color;
   ImageFasor->Canvas->Brush->Style = brush.style;
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::DrawEixos(void)
   {
   //variáveis locais
   int     x0, y0;

   //define coordenadas da origem dos eixos
   x0 = ImageFasor->Width / 2;
   y0 = ImageFasor->Height / 2;
   //desenha eixos
   ImageFasor->Canvas->Pen->Color = clWhite; //clLtGray clGray;
   ImageFasor->Canvas->MoveTo(x0, 0);
   ImageFasor->Canvas->LineTo(x0, ImageFasor->Height);
   ImageFasor->Canvas->MoveTo(0, y0);
   ImageFasor->Canvas->LineTo(ImageFasor->Width, y0);
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::DrawFasor(void)
   {
   //variáveis locais
   int     x0, y0;
   struct {    
          int        width;
          TColor     color;
          } pen;
   VTFasor *fasor;

   //salva atributos do Canvas
   pen.color = ImageFasor->Canvas->Pen->Color;
   pen.width = ImageFasor->Canvas->Pen->Width;
   //define cor de fundo
   draw.cor_fundo = clBtnFace;
   //limpa o Canvas
   LimpaImageFasor(draw.cor_fundo);
   //define coordenadas da origem dos eixos
   x0 = ImageFasor->Width / 2;
   y0 = ImageFasor->Height / 2;
   //desenha eixos
   DrawEixos();
   //desenha primeiro fasores que preenchem uma área com cor
   for (int n = 0; n < fasores->LisFasorGruposSelecionados()->Count; n++)
      {//verifica se o Fasor está habilitado
      fasor = (VTFasor*)fasores->LisFasorGruposSelecionados()->Items[n];
      if (fasor->dim == 0)  continue;
      if (! fasor->enabled) continue;
      if (! fasor->visible) continue;
      if (fasor->Tipo() == fasorAJUSTE) DrawFasor(x0, y0, fasor);
      }
   //desenha fasores que NÃO preenchem uma área com cor
   for (int n = 0; n < fasores->LisFasorGruposSelecionados()->Count; n++)
      {//verifica se o Fasor está habilitado
      fasor = (VTFasor*)fasores->LisFasorGruposSelecionados()->Items[n];
      if (fasor->dim == 0)  continue;
      if (! fasor->enabled) continue;
      if (! fasor->visible) continue;
      if (fasor->Tipo() != fasorAJUSTE) DrawFasor(x0, y0, fasor);
      }    
   //restaura configuração de TPen
   ImageFasor->Canvas->Pen->Color = pen.color;
   ImageFasor->Canvas->Pen->Width = pen.width;
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::DrawFasor(int x0, int y0, VTFasor *fasor)
   {
   //variáveis locais
   double  raio, escala;
   int     x, y, xt, yt;

   //verifica se é fasorAJUSTE: desenha região de atuação primeiro
   if (fasor->Tipo() == fasorAJUSTE) DrawArea(x0, y0, (TFasorAjuste*)fasor);
   //define escala
   escala = (2. * fasor->dim) / (0.8 * min(ImageFasor->Width, ImageFasor->Height));
   //converte coordenadas do fasor p/ pixel (invertendo y)
   x = x0 + (fasor->coord.x / escala);
   y = y0 - (fasor->coord.y /escala);
   //desenha circunferência
   raio = RoundTo(fasor->raio/escala, 0) + 1;
   DrawCircunferencia(x0, y0, raio, clWhite);//clLtGray clGray
   //configura Canvas(Font, Pen e Brush)
   ImageFasor->Canvas->Pen->Color   = fasor->color;
   ImageFasor->Canvas->Pen->Width   = 1;
   ImageFasor->Canvas->Font->Color  = fasor->color;
   ImageFasor->Canvas->Brush->Color = draw.cor_fundo;
   ImageFasor->Canvas->Brush->Style = bsClear;
   //desenha reta do fasor
   ImageFasor->Canvas->MoveTo(x0, y0);
   ImageFasor->Canvas->LineTo(x, y);
   //desenha seta do fasor
   if (fasor->draw_seta)
      {DrawSeta(x0, y0, x, y, fasor->color);}
   //exibe nome do fasor
   xt = x; if (x < x0) xt -= ImageFasor->Canvas->TextWidth(fasor->codigo);
   yt = y; if (y < y0) yt -= ImageFasor->Canvas->TextHeight(fasor->codigo);
   ImageFasor->Canvas->TextOut(xt, yt, fasor->codigo);
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::DrawSeta(int x1, int y1, int x2, int y2, TColor cor)
   {
   //variáveis locais
   double hipot, sin_alfa, cos_alfa;
   double dx = x2 - x1;
   double dy = y2 - y1;
   double comp = 12;
   double larg = 4;
   int    xa, ya, xb, yb;

   //proteção
   if ((x2 == x1)&&(y2 == y1)) return;
   //determina seno e cosseno do ângulo da reta
   hipot = sqrt((dx*dx)+(dy*dy));
   if (hipot < comp) return;
   cos_alfa = dx / hipot;
   sin_alfa = dy / hipot;
   //determina os pontos A e B
   xa = x2 - RoundTo((comp * cos_alfa) + (larg * sin_alfa), 0);
   ya = y2 - RoundTo((comp * sin_alfa) - (larg * cos_alfa), 0);
   xb = x2 - RoundTo((comp * cos_alfa) - (larg * sin_alfa), 0);
   yb = y2 - RoundTo((comp * sin_alfa) + (larg * cos_alfa), 0);
   //desenha as linha (xa,ya)/(x2,y2) e (x2,y2)/(xb,yb)
   ImageFasor->Canvas->MoveTo(xa, ya);
   ImageFasor->Canvas->LineTo(x2, y2);
   ImageFasor->Canvas->LineTo(xb, yb);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormProtecao::ExecutaCalculoFluxo(void)
   {
   //variáveis locais
   VTFlow  *flow  = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //verifica se o fluxo foi calculado
   if (! flow->UpToDate)
      {//executa o cálculo de fluxo desequilibrado
      if (Confirma("Dados de fluxo de potência não estão disponíveis.",
                   "Deseja executar o cálculo de fluxo de potência ?") != IDYES) return;
      flow->TipoFluxo          = fluxoAUTO;
      flow->MultiThreadEnabled = true;
      flow->TipoRelatorio      = relNENHUM;
      flow->HabilitaAviso(true);
      flow->HabilitaOtimizacao(true);
      flow->CalculaPorRede(redes);
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormProtecao::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form sem alinhamento
   ActionAlignNoneExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::FormIniciaPropriedades(void)
   {
   //seta propriedade DoubleBuffered p/ evitar flicker
   DoubleBuffered = true;
   //limita tamanho do form
   Constraints->MaxHeight = Screen->Height;
   Constraints->MaxWidth  = Screen->Width;
   //limita tamanho do gráfico
   ImageFasor->Constraints->MaxHeight = Screen->Height;// - 46;
   ImageFasor->Constraints->MaxWidth  = Screen->Width;// - 8;
   //aloca Bitmap p/ o gráfico
   //if (ImageFasor->Picture->Graphic) delete ImageFasor->Picture->Graphic;
   graph_bmp         = new Graphics::TBitmap();
   graph_bmp->Height = Screen->Height;
   graph_bmp->Width  = Screen->Width;
   ImageFasor->Picture->Graphic = graph_bmp;
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::FormResize(TObject *Sender)
   {
   //redesenha o fasor
   if (fasores) DrawFasor();
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   //inicia objeto EdtExplorer
   ActionEdtExecute(NULL);
   }

//---------------------------------------------------------------------
void __fastcall TFormProtecao::IniciaAngulos(void)
   {
   //insere valores default
   EditAjusteA->Text  = "-60";
   EditAjusteA0->Text = "110";
   EditAjusteB->Text  = "-150";
   EditAjusteB0->Text = "-60";
   }

//---------------------------------------------------------------------
void __fastcall TFormProtecao::IniciaCBoxPatamar(void)
   {
   //variáveis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   TList       *lisPAT    = patamares->LisPatamar();

   //reinicia CBoxPatamar c/ Patamares
   CBoxPatamar->Clear();
   for (int np = 0; np < lisPAT->Count; np++)
      {
      patamar = (VTPatamar*)lisPAT->Items[np];
      CBoxPatamar->Items->AddObject(patamar->Nome, patamar);
      }
   //pré-seleciona primeiro patamar
   if (CBoxPatamar->Items->Count > 0) CBoxPatamar->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::LimpaImageFasor(TColor color)
   {
   /*
   //variáveis locais
   struct   {
            TColor color;
            TBrushStyle = style;
            }brush;
   //salva configuração de Brush
   brush.color = ImageFasor->Canvas->Brush->Color;
   brush.style = ImageFasor->Canvas->Brush->Style;
   */
   //obtém área de TImagefasor
   TRect r = ImageFasor->ClientRect;
   //apaga a área com a cor de fundo
   ImageFasor->Canvas->Brush->Color = color;
   ImageFasor->Canvas->Brush->Style = bsSolid;
   //imag->Canvas->Brush->Color = tela.cor_fundo;
   ImageFasor->Canvas->FillRect(r);
   /*
   //salva configuração de Brush
   ImageFasor->Canvas->Brush->Color = brush.color;
   ImageFasor->Canvas->Brush->Style = brush.style;
   */
   }
//---------------------------------------------------------------------------
void __fastcall TFormProtecao::LViewFasorAtualiza(void)
   {
   //variaveis locais
   bool            unidade_pu;
   double          ang_grau;
   complex<double> valor;
   TListItem       *item, *item_visivel;
   VTFasor         *fasor;
   TList           *lisFASOR = fasores->LisFasorGruposSelecionados();

   //verifica se o valor de tensão deve ser exibido em pu
   unidade_pu = (CBoxUnidade->ItemIndex == 0);
   //limpa LView
   LViewFasor->Clear();
   for (int np = 0; np < lisFASOR->Count; np++)
      {//insere cod.fasor, módulo e ângulo
      fasor = (VTFasor*)lisFASOR->Items[np];
      if(! fasor->enabled) continue;
      if (! fasor->Valor(valor, unidade_pu)) continue;
      ang_grau = RadToDeg(Arg(valor));
      item = LViewFasor->Items->Add();
      item->Data    = fasor;
      item->Checked = fasor->visible;
      item->Caption = "";
      item->SubItems->Add("");
      item->SubItems->Add(fasor->codigo);
      //módulo: verifica se fasor de ângulo
      if (fasor->Tipo() == fasorANGULO)
         {item->SubItems->Add("-");}
      else
         {item->SubItems->Add(DoubleToStr("%4.3f", Abs(valor), ','));}
      //ângulo
      item->SubItems->Add(DoubleToStr("%3.2f", ang_grau,   ','));
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::LViewFasorCustomDrawSubItem(TCustomListView *Sender,
          TListItem *Item, int SubItem, TCustomDrawState State, bool &DefaultDraw)

   {
   //variáveis locais
   VTFasor *fasor = (VTFasor*)Item->Data;

   //proteção
   if (fasor == NULL) return;
   //insere a cor do fasor no LView
   if (SubItem == 1)
      {
      LViewFasor->Canvas->Brush->Color = fasor->color;
      LViewFasor->Canvas->Brush->Style = bsSolid;
      }
   else
      {
      LViewFasor->Canvas->Brush->Color = clWhite;//clWindow;
      LViewFasor->Canvas->Brush->Style = bsSolid;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::LViewFasorClick(TObject *Sender)
   {
   //variáveis locais
   TListItem *Item;
   VTFasor   *fasor;

   //DVK 2014.10.31
   LViewFasorGrafico->Clear();
   //loop p/ todos TListItem
   for (int n = 0; n < LViewFasor->Items->Count; n++)
      {
      Item = LViewFasor->Items->Item[n];
      //habilita/desabilita visualização do fasor
      if ((fasor = (VTFasor*)Item->Data) == NULL) continue;
      fasor->visible = Item->Checked;
      }
   //DVK 2014.10.31 insere fasores visiveis no LViewFasorGrafico
   LViewFasorVisivel();
   //reapresenta os Fasores
   DrawFasor();
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::LViewFasorDblClick(TObject *Sender)
   {
   //variáveis locais
   VTFasor *fasor;

   //proteção
   if(! LViewFasor->Selected) return;
   if ((fasor = (VTFasor*)LViewFasor->Selected->Data) == NULL) return;
   ColorDialog->Color = fasor->color;
   //altera cor do fasor selecionado
   Hide();
   if (ColorDialog->Execute())
      {//seleciona nova cor do fasor
      fasor->color = ColorDialog->Color;
      //reapresenta os Fasores
      DrawFasor();
      }
   Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::LViewFasorDrawItem(TCustomListView *Sender, TListItem *Item,
          TRect &Rect, TOwnerDrawState State)
   {
   //variáveis locais
   TCanvas      *Canvas = LViewFasor->Canvas;
   VTFasor      *fasor;
   TColor       color_ori;
   TBrushStyle  style_ori;

   //limpa a área do item
   Canvas->FillRect(Rect);
   //determina objeto Fasor
   if ((fasor = (VTFasor*)Item->Data) == NULL) return;
   //salva cor/style original
   color_ori = Canvas->Brush->Color;
   style_ori = Canvas->Brush->Style;
   //define área do retângulo p/ a cor do Fasor
   TRect r = Rect;
   r.Top +=2;  r.Bottom -= 2;
   r.Right = r.Left + (r.Bottom - r.Top) + 2;
   //escreve código do Fasor
   Canvas->TextOut(r.Right+1, Rect.Top, fasor->codigo);
   //desenha retângulo c/ a cor
   Canvas->Brush->Color = fasor->color;
   Canvas->Brush->Style = bsSolid;
   Canvas->FillRect(r);  //desenha o retângulo da cor
   //restaura cor/style original
   Canvas->Brush->Color = color_ori;
   Canvas->Brush->Style = style_ori;
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::LViewFasorVisivel(void)
   {//DVK 2014.10.31 insere fasores visiveis no LViewFasorGrafico
   //variáveis locais
   TListItem      *Item, *item_visivel;
   VTFasor        *fasor;
   double          ang_grau;
   bool            unidade_pu;
   complex<double> valor;

   //limpa
   LViewFasorGrafico->Clear();
   //DVK 2014.11.06 verifica se o valor de tensão deve ser exibido em pu
   unidade_pu = (CBoxUnidade->ItemIndex == 0);
   //loop p/ todos TListItem
   for (int n = 0; n < LViewFasor->Items->Count; n++)
      {
      Item = LViewFasor->Items->Item[n];
      if ((fasor = (VTFasor*)Item->Data) == NULL) continue;
      if(fasor->visible)
         {//mostra somente fasores de tensão e corrente visiveis
         if((fasor->Tipo() == fasorANGULO) || (fasor->Tipo() == fasorAJUSTE)) continue;
         if (! fasor->Valor(valor, unidade_pu)) continue;
         ang_grau = RadToDeg(Arg(valor));
         item_visivel = LViewFasorGrafico->Items->Add();
         item_visivel->Data = fasor;
         item_visivel->Caption = fasor->codigo;
         item_visivel->SubItems->Add(DoubleToStr("%4.3f", Abs(valor), ','));
         item_visivel->SubItems->Add(DoubleToStr("%3.2f", ang_grau,   ','));
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::LViewFasorItemChecked(TObject *Sender, TListItem *Item)
   {
   //variáveis locais
   VTFasor *fasor;

   //habilita/desabilita visualização do fasor
   if ((fasor = (VTFasor*)Item->Data) == NULL) return;
   fasor->visible = Item->Checked;
   //reapresenta os Fasores
   DrawFasor();
   }

//---------------------------------------------------------------------------
void __fastcall TFormProtecao::TimerTimer(TObject *Sender)
   {
   //variáveis locais
   int index;

   //protecao
   if (fasores == NULL) return;
   //seleciona próximo patamar
   if ((index  = CBoxPatamar->ItemIndex + 1) >= CBoxPatamar->Items->Count) index = 0;
   CBoxPatamar->ItemIndex = index;
   //força atualizaçao do Fasor
   CBoxPatamarChange(NULL);
   }

//---------------------------------------------------------------------------
//eof



