//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TEdtPolyline.h"
#include "TFormEdtPolyline.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Busca\VTBusca.h"
#include "..\Constante\Cursor.h"
#include "..\Edita\VTEdita.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTRede.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Rede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//função global p/ criar objetos da classe TEdtPolyline
//---------------------------------------------------------------------------
VTEdt* NewObjEdtPolyline(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone, TWinControl *parent)
   {
   return(new TEdtPolyline(apl, ResetFocus, SetFocus, OnDone, parent));
   }

//---------------------------------------------------------------------------
__fastcall TEdtPolyline::TEdtPolyline(VTApl *apl, TAction *ResetFocus, TAction *SetFocus, TAction *OnDone, TWinControl *parent)
                        :TEdt(apl, crDefault, ResetFocus, SetFocus, OnDone)
   {
   //obtém objeto VTRedegraf
   redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   //cria lista de Barras
   lisBARRA = new TList();
   //cria Barra e Trecho fictícios p/ exibir rastro do mouse
   rastro.barra   = DLL_NewObjBarra(patamares->NumPatamar());
   rastro.trecho  = DLL_NewObjTrecho(patamares->NumPatamar());
   //cria FormEdtPolyline como janela normal
   FormEdtPolyline = new TFormEdtPolyline(NULL, apl, this, parent);
   FormEdtPolyline->Show();
   //prepara p/ iniciar entrada de Trechos
   IniciaNovaSequenciaTrechos();
   //habilita tratamento dos eventos demouse
   evt_mouse_enabled = true;
   //mais nada a fazer: o contrutor de VTEdt faz todas iniciações
   }

//---------------------------------------------------------------------------
__fastcall TEdtPolyline::~TEdtPolyline(void)
   {
   //IMPORTANTE: destrói FormEdtPolyline SOMENTE SE VISÍVEL
   if ((FormEdtPolyline)&&(FormEdtPolyline->Visible)&&(FormEdtPolyline->Enabled))
      {//destrói SEM FECHAR FormEdtPolyline
      delete FormEdtPolyline;
      FormEdtPolyline = NULL;
      }
   //destrói lista sem destruir seus objetos
   if (lisBARRA) {delete lisBARRA; lisBARRA = NULL;}
   //destrói objetos locais
   if (rastro.barra)  {DLL_DeleteEqpto(rastro.barra);  rastro.barra  = NULL;}
   if (rastro.trecho) {DLL_DeleteEqpto(rastro.trecho); rastro.trecho = NULL;}
   //executa Action externa
   if (extAction.ResetFocus) extAction.ResetFocus->Execute();
   //mais nada a fazer: o destrutor de VTEdt faz todas finalizações
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TEdtPolyline::BuscaBarra(void)
   {
   //variáveis locais
   VTBarra *barra;
   TList   *lisBAR;

   //define a area como o ponto do evento para fazer a busca
   area->x1 = area->x2 = evt.x;
   area->y1 = area->y2 = evt.y;
   //faz a busca de barra
   lisBAR = busca->Barra(area);
   //verifica se encontrou uma Barra
   if (lisBAR->Count > 0)
      {//altera formato do mouse
      graf->Cursor(TCursor(curBarra));
      //seleciona primeira Barra
      barra = (VTBarra*)lisBAR->First();
      }
   else
      {//altera formato do mouse
      graf->Cursor(crDefault);
      //nenhuma barra selecionada
      barra = NULL;
      }
   return(barra);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtPolyline::EstimaCoordEsquematico(strUTM &utm, strESQ_BARRA &esq)
   {
   //variágveis locais
   double  fat_mult;
   VTArea *area_utm = redes->AreaUtm_cm();
   VTArea *area_esq = redes->AreaEsquematico();

   //verifica se área utm á válida
   if ((area_utm->x1 < 0)||(area_utm->y1 < 0))
      {//ainda não há nenhuma Barra definida: define coordenadas p/ Barra ficar no centro do esquemático
      esq.x1 = esq.x2 = area_esq->x1 + (area_esq->Width()  / 2);
      esq.y1 = esq.y2 = area_esq->y1 + (area_esq->Height() / 2);
      }
   else
      {//calcula coordenada x
      fat_mult = double(utm.x - area_utm->x1) / area_utm->Width();
      esq.x1   = esq.x2 = area_esq->x1 + (fat_mult * area_esq->Width());
      //calcula coordenada y
      fat_mult = double(utm.y - area_utm->y1) / area_utm->Height();
      esq.y1   = esq.y2 = area_esq->y1 + (fat_mult * area_esq->Height());
      }
   }
   
//---------------------------------------------------------------------------
void __fastcall TEdtPolyline::EstimaCoordUtm(strUTM &utm, strESQ_BARRA &esq)
   {
   //variágveis locais
   double  fat_mult;
   VTArea *area_utm = redes->AreaUtm_cm();
   VTArea *area_esq = redes->AreaEsquematico();

   //verifica se área utm á válida
   if ((area_utm->x1 < 0)||(area_utm->y1 < 0))
      {//ainda não há nenhuma Barra definida: não dá p/ fazer nada
      return;
      }
   else
      {//calcula coordenada x
      fat_mult = double(esq.x1 - area_esq->x1) / area_esq->Width();
      utm.x    = area_utm->x1 + (fat_mult * area_utm->Width());
      //calcula coordenada y
      fat_mult = double(esq.y1 - area_esq->y1) / area_esq->Height();
      utm.y   = area_utm->y1  + (fat_mult * area_utm->Height());
      }
   }
   
//---------------------------------------------------------------------------
void __fastcall TEdtPolyline::EvtEnter(void)
   {
   //inicia uma nova sequencia de Trechos
   IniciaNovaSequenciaTrechos();
   }

//---------------------------------------------------------------------------
void __fastcall TEdtPolyline::EvtMouseDown(void)
   {
   //variáveis locais
   VTBarra  *barra;
   VTTrecho *trecho;

   //verifica se o tratamento dos eventos de mouse está habilitado
   if (! evt_mouse_enabled) return;
   //verifica se usuário definiu dados p/ os Trechos
   if (! ValidaDadosTrecho()) return;
   //cancela exibição da lista de Eqpto novo
   LisEqptoNovoApaga();
   /*
   //verifica se a tecla Shift foi usada
   if (evt.Shift.Contains(ssShift))
      {//reinicia nova sequência de Trechos
      IniciaNovaSequenciaTrechos();
      }
   */
   //verifica se existe uma Barra selecionada
   if (barra_sel != NULL)
      {//insere Barra existente
      InsereBarraExistente(barra_sel);
      }
   else
      {//cria uma nova Barra e insere na Rede
      if ((barra_sel = InsereBarraNova()) == NULL) return;
      }
   //redefine Barra do Trecho usado p/ rastro do mouse
   rastro.trecho->pbarra1 = barra_sel;
   //atualiza gráfico: apaga eqptos selecionados
   if (lisEQS->Count > 0) graf->Redraw(redrawEQP_SEL_COR_ORIG, this);
   //insere Barra em lisEQS
   if (lisEQS->IndexOf(barra_sel) < 0) lisEQS->Add(barra_sel);
   //cria um novo Trecho e insere na Rede
   if ((trecho = InsereTrechoNovo()) != NULL)
      {//insere Barra e Trecho em lisEQS
      lisEQS->Add(trecho);
      }
   //atualiza gráfico: exibe eqptos selecionados
   graf->Redraw(redrawEQP_SEL_COR_UNICA, this);
   //exibe lista de Eqptos novos
   LisEqptoNovoExibe();
   }

//---------------------------------------------------------------------------
void __fastcall TEdtPolyline::EvtMouseDownMove(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TEdtPolyline::EvtMouseMove(void)
   {
   //verifica se o tratamento dos eventos de mouse está habilitado
   if (! evt_mouse_enabled) return;
   //verifica se o mouse foi movimentado
   if ((evt.x == evt.x_ant)&&(evt.y == evt.y_ant)) return;
   //cancela exibição da lista de Eqpto novo
   LisEqptoNovoApaga();
   //atualiza coordenadas da Barra usada p/ rastro do mouse
   if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
      {//define coordenadas utm da Barra
      rastro.barra->utm.x = evt.x;
      rastro.barra->utm.y = evt.y;
      //define coordenadas esquemático p/ que Barra fique no canto superior esquerdo
      rastro.barra->esq.x1 = rastro.barra->esq.x2 = redes->AreaEsquematico()->x1;
      rastro.barra->esq.y1 = rastro.barra->esq.y2 = redes->AreaEsquematico()->y1;
      }
   else
      {
      rastro.barra->utm.x = 0;
      rastro.barra->utm.y = 0;
      rastro.barra->esq.x1 = rastro.barra->esq.x2 = evt.x;
      rastro.barra->esq.y1 = rastro.barra->esq.y2 = evt.y;
      }
   //verifica se existe barra apontada pelo mouse
   barra_sel = BuscaBarra();
   //exibe da lista de Eqpto novo
   LisEqptoNovoExibe();
   }

//---------------------------------------------------------------------------
void __fastcall TEdtPolyline::EvtMouseUp(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TEdtPolyline::IniciaNovaSequenciaTrechos(void)
   {
   //cancela exibição da lista de Eqpto novo
   LisEqptoNovoApaga();
   //reinicia Barras do Trecho usado p/ rastro do mouse
   rastro.trecho->pbarra1 = rastro.barra;
   rastro.trecho->pbarra2 = rastro.barra;
   //reinicia lista de Barras
   lisBARRA->Clear();
   //reinicia Barra selecionada
   barra_sel = NULL;
   }

//---------------------------------------------------------------------------
void __fastcall TEdtPolyline::InsereBarraExistente(VTBarra *barra)
   {
   //insere Barra em lisBARRA (mesmo que a Barra já exista em lisBARRA)
   lisBARRA->Add(barra);
   }

//---------------------------------------------------------------------------
VTBarra* __fastcall TEdtPolyline::InsereBarraNova(void)
   {
   //variáveis locais
   VTBarra *barra;

   if ((barra = DLL_NewObjBarra(patamares->NumPatamar())) != NULL)
      {//define atributos da Barra
      barra->Id     = FalsoId();
      barra->Codigo = FormEdtPolyline->CodigoBarra;
      barra->vnom   = FormEdtPolyline->Vnom_kv;
      //define coordenadas iguais à da barra usada p/ rastro do mouse
      barra->utm = rastro.barra->utm;
      barra->esq = rastro.barra->esq;
      //verifica o modo gráfico
      if (redegraf->ModoGrafico == modoGEOREFERENCIADO)
         {//estima coordenadas esquemático
         EstimaCoordEsquematico(barra->utm, barra->esq);
         }
      else
         {//estima coordenadas utm
         EstimaCoordUtm(barra->utm, barra->esq);
         }
      //insere Barra em lisBARRA
      lisBARRA->Add(barra);
      //insere Barra em FormEdtPolyline
      FormEdtPolyline->InsereBarra(barra);
      //insere Barra
      edita->InsereBarra(barra);
      }
   return(barra);
   }

//---------------------------------------------------------------------------
VTTrecho* __fastcall TEdtPolyline::InsereTrechoNovo(void)
   {
   //variáveis locais
   int       x1, y1, x2, y2;
   double    comp_km;
   VTBarra   *barra_ini;
   VTBarra   *barra_fim;
   VTTrecho  *trecho = NULL;

   //verifica se há duas Barras em lisBARRA
   if (lisBARRA->Count < 2) return(NULL);
   //determina Barras do Trecho
   barra_ini = (VTBarra*)lisBARRA->Items[lisBARRA->Count-2];
   barra_fim = (VTBarra*)lisBARRA->Items[lisBARRA->Count-1];
   //verifica se as Barras tem a mesma tensão nominal
   if (! IsDoubleZero(barra_ini->vnom - barra_fim->vnom))
      {//desabilita tratamento de eventos de mouse
      evt_mouse_enabled = false;
      Aviso("As barras do trecho devem ter a mesma tensão nominal");
      //reabilita tratamento de eventos de mouse
      evt_mouse_enabled = true;
      return(NULL);
      }
   //verifica se o comprimento está definido
   if (barra_ini->CoordenadasUtm_cm(x1, y1) && barra_fim->CoordenadasUtm_cm(x2, y2))
      {//calcula comprimento real do Trecho
      comp_km = Distancia_cm(x1, y1, x2, y2) * 1e-5;
      if (comp_km <= 0.)  return(NULL);
      }
   else
      {//assume comprimento definido em FormEdtPolyline
      comp_km = FormEdtPolyline->Comprimento_km;
      if (comp_km <= 0.)
         {
         evt_mouse_enabled = false;
         Aviso("Barras sem coordenadas geo-referenciadas.\nDefina o comprimento dos trechos");
         if (trecho != NULL) DLL_DeleteEqpto(trecho);
         //reabilita tratamento de eventos de mouse
         evt_mouse_enabled = true;
         return(NULL);
         }
      }
   //cria o novo Trecho
   if ((trecho = DLL_NewObjTrecho(patamares->NumPatamar())) != NULL)
      {
      trecho->Id             = FalsoId();
      trecho->Codigo         = FormEdtPolyline->CodigoTrecho;
      trecho->Comprimento_km = comp_km;
      //define Barras e Arranjo do Trecho
      trecho->DefineObjBarra(barra_ini, barra_fim);
      trecho->DefineObjArranjo(FormEdtPolyline->Arranjo);
      //trecho->rede = FormEdtPolyline->Rede;
      trecho->DefineObjRede(FormEdtPolyline->Rede);
      //insere Trecho na Rede
      edita->InsereLigacao(trecho, trecho->rede);
      //insere Trecho em FormEdtPolyline
      FormEdtPolyline->InsereTrecho(trecho);
      }
   return(trecho);
   }

//---------------------------------------------------------------------------
void __fastcall TEdtPolyline::LisEqptoNovoApaga(void)
   {
   //verifica se há Eqpto em lisEQN
   if (lisEQN->Count > 0)
      {//atualiza o grafico e limpa lisEQN
      graf->Redraw(redrawEQP_NOVO, this);
      lisEQN->Clear();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TEdtPolyline::LisEqptoNovoExibe(void)
   {
   //insere Trecho usado p/ rastro do mouse
   if ((FormEdtPolyline->ExibirRastro)&&(rastro.trecho->pbarra1 != rastro.trecho->pbarra2)) lisEQN->Add(rastro.trecho);
   //insere última Barra de lisBARRA
   if (lisBARRA->Count > 0) lisEQN->Add(lisBARRA->Last());
   //insere Barra apontada pelo mouse
   if ((barra_sel != NULL)&&(lisEQN->IndexOf(barra_sel) < 0)) lisEQN->Add(barra_sel);
   //verifica se há Eqpto em lisEQN
   if (lisEQN->Count > 0)
      {//atualiza o grafico
      graf->Redraw(redrawEQP_NOVO, this);
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TEdtPolyline::ValidaDadosTrecho(void)
// https://sinapsisenergia.teamwork.com/#/tasks/18687628
{
    // variáveis locais
    TList       *lisEQP;
    VTTrecho    *trecho = NULL;
    double      vnom_kv;

    // verifica se algumas informações podem ser recuperadas da barra inicialmente clicada
    if(barra_sel)
    {
        // verifica se o nível de tensão pode ser recuperado
        vnom_kv = StrToDouble(FormEdtPolyline->EditVnom->Text);
        if(IsDoubleZero(vnom_kv))
            FormEdtPolyline->EditVnom->Text = barra_sel->vnom;

        // verfica se o último trecho da barra pode ser recuperado
        lisEQP = new TList();
        lisEQP->Clear();
        barra_sel->LisLigacao(lisEQP, eqptoTRECHO);

        //determina último Trecho
        if(lisEQP->Count > 0)
            trecho = (VTTrecho*)lisEQP->Last();
        if(lisEQP)
        {
            delete lisEQP;
            lisEQP = NULL;
        }
    }

    // se o último trecho da barra clicada pôde ser recuperado
    if(trecho)
    {
        // verifica se o último trecho possui arranjo típico e qual é o seu código
        if((FormEdtPolyline->CBoxArranjo->ItemIndex < 0) &&
            trecho->ArranjoTipico)
            FormEdtPolyline->CBoxArranjo->ItemIndex =
                FormEdtPolyline->CBoxArranjo->Items->IndexOf(trecho->arranjo->Codigo);

        // verifica se o último trecho possui rede e qual é o seu código
        if((FormEdtPolyline->CBoxRede->ItemIndex < 0) &&
           trecho->rede)
            FormEdtPolyline->CBoxRede->ItemIndex =
                FormEdtPolyline->CBoxRede->Items->IndexOf(trecho->rede->Codigo);
    }

    if (FormEdtPolyline->Rede == NULL)
        return(false);
    if (FormEdtPolyline->Arranjo == NULL)
        return(false);
    if (IsDoubleZero(FormEdtPolyline->Vnom_kv))
        return(false);
    //valida comprimento somente se edição em modo esquemático
    if (redegraf->ModoGrafico == modoESQUEMATICO)
      {
      if (IsDoubleZero(FormEdtPolyline->Comprimento_km)) return(false);
      }
    return(true);
}

//---------------------------------------------------------------------------
//eof
