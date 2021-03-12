//-----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TGraficoAbre.h"
#include "..\BDihm\Esquematico\VTEsquematico.h"
#include "..\BDihm\Esquematico\VTLiga.h"
#include "..\Apl\VTApl.h"
#include "..\Editor\VTEdt.h"
#include "..\Grafico\TFormGraf.h"
#include "..\Grafico\TPadrao.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipoRede.h"
#include "..\Redegraf\VTCelulas.h"

//-----------------------------------------------------------------------------
VTGrafico* __fastcall NewObjGraficoAbre(VTApl *apl)
   {
   return(new TGraficoAbre(apl));
   }

//-----------------------------------------------------------------------------
// TGraficoAbre::TGraficoAbre()   -  construtor
//-----------------------------------------------------------------------------
__fastcall TGraficoAbre::TGraficoAbre(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria objeto TPadrao
   padrao = NewObjPadrao();
   //cria TFormGraf
   formGraf   = new TFormGraf(apl->Form, this, apl);
   //esconde botão p/ mostrar cartografia
   formGraf->ActionMapa->Visible = false;
   //define este objeto VTGraficoAbre para TFormGraf
   //formGraf->DefObjGraf(this);
   //cria lista
   lisEQP = new TList();
   //define cores p/ Redes incluídas e excluídas da seleção
   color.fundo    = clBlack;
   color.incluida = clGreen;
   color.excluida = clOlive; //clSkyBlue
   color.valida   = clGray;
   }

//-----------------------------------------------------------------------------
// TGraficoAbre::~TGraficoAbre()   -  destrutor
//-----------------------------------------------------------------------------
__fastcall TGraficoAbre::~TGraficoAbre(void)
   {
   //fecha e destrói FormGraf
   Close();
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   //não destrói o form TFormGraf criado pelo construtor
   //porque ele será destruído automaticamente pelo Owner
   //if (formCfg)  {delete formCfg;  formCfg  = NULL;}
   //if (formGraf) {delete formGraf; formGraf = NULL;}
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::AreaRedeAltera(void)
   {
   AreaRedeDefine();
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::AreaRedeDefine(void)
   {
   //variáveis locais
   VTCelulas     *celulas     = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   VTEsquematico *esquematico = (VTEsquematico*)apl->GetObject(__classid(VTEsquematico));

   //proteção
   if (esquematico == NULL) return;
   //reinicia conjunto de Celulas
   celulas->DefineAreaTotal();
   //define área da rede p/ FormGraf
   formGraf->AreaRedeDefine(esquematico->Area());
   }


//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::AreaZoomAlterada(void)
   {
   //nada a fazer
   }
   
//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::Close(void)
   {
   //variáveis locais
   TFormGraf *form_graf_tmp = formGraf;

   //cancela ponteiro antes  de destruir FormGraf
   formGraf = NULL;
   //destrói TformGraf criado pelo construtor
   if (form_graf_tmp) {form_graf_tmp->Parent = NULL; delete form_graf_tmp;}
   //destrói objetos
   if (padrao) {delete padrao; padrao = NULL;}
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::Cursor(TCursor cursor)
   {
   if (formGraf) formGraf->imag->Cursor = cursor;
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::DefActionOnZoomChange(TAction *ActionOnZoomChange)
   {
   //nada a fazer
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::DefMouseOp(int op)
   {
   formGraf->DefMouseOp(op);
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::DefineParent(TWinControl *parent)
   {
   //redefine Parent dos Forms
   formGraf->DefineParent(parent);
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::DefineParentToolBar(TWinControl *parent)
   {
   //proteção
   if (parent == NULL)
      {
      formGraf->ToolBar->Visible = false;
      return;
      }
   //redefine Parent do ToolBar do FormGraf
   formGraf->ToolBar->Parent = parent;
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::Disable(void)
   {
   }

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16082236
void __fastcall TGraficoAbre::DestacaBarras(TList *lisEQP, TColor color, int dim_linha)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TGraficoAbre::DestacaEqpto(TList *lisEQP, TColor color, int dim_linha)
   {
   }

//-----------------------------------------------------------------------------
double __fastcall TGraficoAbre::Escala(void)
   {
   return(formGraf->Escala());
   }

//-----------------------------------------------------------------------------
bool __fastcall TGraficoAbre::Exporta(TCanvas *canvas, TRect &rect, TColor cor_fundo, int opcao)
   {
   return(false);
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::Hide(void)
   {
   //esconde o form
   formGraf->Hide();
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::InsereObjEditor(VTEdt *edt)
   {
   //salva ponteiro p/ objeto
   edt_ext = edt;
   }
/*
//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::InsereObjUnif(VTUnif *unif)
   {
   //nada a fazer
   }
*/

//-----------------------------------------------------------------------------
VTUnif* __fastcall TGraficoAbre::InsereObjUnif(VTApl *apl, int graf_num)
   {
   return(NULL);
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::LimpaTela(void)
   {
   formGraf->LimpaTela(color.fundo, false);
   }

//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16082236
void __fastcall TGraficoAbre::LimpaDestaque(void)
   {
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::ModoGraficoAlterado(void)
   {
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::Moldura(VTEqpto *eqpto)
   {
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::Moldura(TList *lisEQP)
   {
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::MolduraSolida(TList *lisEQP, TColor color)
   {
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::Mouse(int Action, TMouseButton Button, TShiftState Shift, int x_utm, int y_utm, int x, int y)
   {
   //verifica se há um editor definido
   if (edt_ext) edt_ext->EvtMouse(Action, Button, Shift, x_utm, y_utm, x, y);
   }

//-----------------------------------------------------------------------------
VTApl* __fastcall TGraficoAbre::PM_GetApl(void)
   {
   return(apl);
   }

//-----------------------------------------------------------------------------
VTArea* __fastcall TGraficoAbre::PM_GetAreaTotal(void)
   {
   //nada a fazer
   return(NULL);
   }

//-----------------------------------------------------------------------------
VTArea* __fastcall TGraficoAbre::PM_GetAreaVisivel(void)
   {
   //nada a fazer
   return(NULL);
   }

//-----------------------------------------------------------------------------
VTArea* __fastcall TGraficoAbre::PM_GetAreaZoom(void)
   {
   //nada a fazer
   return(NULL);
   }
//-----------------------------------------------------------------------------
TCanvas* __fastcall TGraficoAbre::PM_GetCanvas(void)
	{
    //nada a fazer
    return(NULL);
	}
//-----------------------------------------------------------------------------
TPadrao* __fastcall TGraficoAbre::PM_GetPadrao(void)
   {
   return(padrao);
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::PM_SetAnimacaoEnabled(bool enabled)
   {
   //nada a fazer
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::PM_SetAreaZoom(VTArea *area)
   {
   //nada a fazer
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::PM_SetMapaEnabled(bool enabled)
   {
   //nada a fazer
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::PM_SetMapaTematicoEnabled(bool enabled)
   {
   //nada a fazer
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::Redraw(void)
   {
   //proteção necessária na iniciação
   if (! formGraf) return;
   //se o form não está visível, não faz nada
   if (! formGraf->Visible) return;
   //limpa a tela
   formGraf->LimpaTela(color.fundo, false);
   //apresenta esquematico das redes
   RedrawEsquematico();
   }

//-----------------------------------------------------------------------------
// TGraficoAbre::Redraw()   -  public
//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::Redraw(int op, VTEdt *editor)
   {
   //proteção necessária na iniciação
   if (! formGraf)return;
   //se o form não está visível, não faz nada
   if (! formGraf->Visible) return;
   //limpa a tela
   formGraf->LimpaTela(color.fundo, false);
   //apresenta layout das redes
   RedrawEsquematico();
   }
/*
//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::RedrawEsquematico(void)
   {
   //variáveis locais
   int           x1, y1, x2, y2;
   VTLiga        *liga;
   VTRede        *rede;
   int           seq_exibicao[] = {redeSEC, redePRI, redeETD, redeSUB, redeETT, redeTRANS};
   VTEsquematico *esquematico   = (VTEsquematico*)apl->GetObject(__classid(VTEsquematico));
   TList         *lisREDE;
   TList         *lisLIGA;

   //proteção
   if (esquematico == NULL) return;
   lisREDE       = esquematico->LisRede();
   lisLIGA       = esquematico->LisLiga();
   //loop p/ todos objetos LigacaoEsq
   for (int nl = 0; nl < lisLIGA->Count; nl++)
      {
      liga = (VTLiga*)lisLIGA->Items[nl];
      //obtém coordenadas de Liga
      esquematico->Coordenada(liga, x1, y1, x2, y2);
      //desenha Liga
      formGraf->DrawLinha(x1, y1, x2, y2, padrao->ligacao.dim, clWhite);
      }
   for (int ind_seq = 0; ind_seq < (sizeof(seq_exibicao)/sizeof(int)); ind_seq++)
      {//exibe nome das Redes que não podem ser selecionadas
      for (int n = 0; n < lisREDE->Count; n++)
         {
         rede = (VTRede*)lisREDE->Items[n];
         //valida a Rede
         if (rede->TipoRede->Segmento != seq_exibicao[ind_seq]) continue;
         if (rede->MRede->Carregada) continue;
         if (rede->MRede->Valida)    continue;
         //obtém coordenadas da Rede
         esquematico->Coordenada(rede, x1, y1);
         //desenha nome da Rede
         formGraf->DrawTextoRect(rede->Codigo, x1, y1, color.excluida);
         }
      //exibe nome das Redes que podem ser selecionadas mas ainda não foram
      for (int n = 0; n < lisREDE->Count; n++)
         {
         rede = (VTRede*)lisREDE->Items[n];
         //valida a Rede
         if (rede->TipoRede->Segmento != seq_exibicao[ind_seq]) continue;
         if (rede->MRede->Carregada) continue;
         if (rede->MRede->Valida)
            {//obtém coordenadas da Rede
            esquematico->Coordenada(rede, x1, y1);
            //desenha nome da Rede
            formGraf->DrawTextoRect(rede->Codigo, x1, y1, color.excluida);
            }
         }
      //exibe nome das Redes já selecionadas
      for (int n = 0; n < lisREDE->Count; n++)
         {
         rede = (VTRede*)lisREDE->Items[n];
         //valida a Rede
         if (rede->TipoRede->Segmento != seq_exibicao[ind_seq]) continue;
         if (rede->MRede->Carregada)
            {//obtém coordenadas da Rede
            esquematico->Coordenada(rede, x1, y1);
            //desenha nome da Rede
            formGraf->DrawTextoRect(rede->Codigo, x1, y1, color.excluida);
            }
         }
      }
   //atualiza gráfico
   formGraf->RedrawTelaReal();
   }
*/
//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::RedrawEsquematico(void)
   {
   //variáveis locais
   int           x1, y1, x2, y2;
   VTLiga        *liga;
   VTRede        *rede;
   VTEsquematico *esquematico   = (VTEsquematico*)apl->GetObject(__classid(VTEsquematico));
   TList         *lisREDE;
   TList         *lisLIGA;

   //proteção
   if (esquematico == NULL) return;
   lisREDE       = esquematico->LisRede();
   lisLIGA       = esquematico->LisLiga();
   //loop p/ todos objetos LigacaoEsq
   for (int nl = 0; nl < lisLIGA->Count; nl++)
      {
      liga = (VTLiga*)lisLIGA->Items[nl];
      //obtém coordenadas de Liga
      esquematico->Coordenada(liga, x1, y1, x2, y2);
      //desenha Liga
      formGraf->DrawLinha(x1, y1, x2, y2, padrao->ligacao.dim, clWhite);
      }
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      //obtém coordenadas da Rede
      esquematico->Coordenada(rede, x1, y1);
      //desenha nome da Rede
      formGraf->DrawTextoRect(rede->Codigo, x1, y1, color.excluida);
      }
   //atualiza gráfico
   formGraf->RedrawTelaReal();
   }


//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::Refresh(void)
   {
   //esconde o form
   if (formGraf->Visible) formGraf->imag->Refresh();
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::RetiraObjEditor(VTEdt *edt)
   {
   //salva ponteiro p/ objeto
   if (edt_ext == edt) edt_ext = NULL;
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::RetiraObjUnif(VTUnif *obj_unif)
   {
   //não implementado nesta classe
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::SelArea(int x1, int y1, int x2, int y2)
   {
   if (edt_ext) edt_ext->EvtSelArea(x1, y1, x2, y2);
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::SelecionaZoom(VTArea *area_utm, VTArea *area_esq)
   {
   //não implementada nesta classe
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::SelecionaZoom(VTEqpto *eqpto, bool ampliar)
   {
   //não implementada nesta fase
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::SizeOfString(AnsiString txt, int &width, int &height)
   {
   width = formGraf->Canvas->TextWidth(txt);
   height = formGraf->Canvas->TextHeight(txt);
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::Show(int graf_num, int ano_carga, int npat)
   {
   //verifica se deve deixar de exibir o gráfico
   if (graf_num == grafNONE)
      {//limpa gráfico atual
      LimpaTela();
      }
   //exibe formGraf
   if (! formGraf->Visible)
      {//exibe o Form e atualiza gráfico
      formGraf->Show();
      Redraw();
      }
   else
      {//atualiza gráfico
      Redraw();
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TGraficoAbre::Show(AnsiString unif_nome, int ano_carga, int npat)
   {
   //nada a fazer
   }
   
//---------------------------------------------------------------------------
//eof


