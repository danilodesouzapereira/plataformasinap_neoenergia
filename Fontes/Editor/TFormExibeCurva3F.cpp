//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "TFormExibeCurva3F.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Const.h"
#include "..\Curvas\VTCurva.h"
#include "..\Legenda\VTLegenda.h"
#include "..\Legenda\VTVetval.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTCarga.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Legenda.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;

//---------------------------------------------------------------------------
__fastcall TFormExibeCurva::TFormExibeCurva(TForm* form_owner, VTApl *apl_owner, TWinControl *parent)
   : TForm(Owner)
   {
   //salva ponteiro p/ objetos
   this->apl = apl_owner;
   //insere em seu Parent
   if (parent)
      {
      Parent = parent;
      Align  = alClient;
      }
   //cria objetos
   legenda   = DLL_NewObjLegenda(apl);
   lisVETVAL = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TFormExibeCurva::~TFormExibeCurva(void)
   {
   //destrói objetos criados
   if (legenda)   {delete legenda; legenda = NULL;}
   //destrói listas e seus obejtos
   if (lisVETVAL) {LimpaTList(lisVETVAL); delete lisVETVAL; lisVETVAL = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormExibeCurva::ChartClick(TObject *Sender)
   {
   //variáveis locais
   double       val_min, val_max, delta;
   TChartSeries *Serie;

   //esconde todas as Series
   for (int n = 0; n < Chart->SeriesCount(); n++)
      {
      Chart->Series[n]->Visible = false;
      }
   //reinicia dimensões do eixo Y
   if (Chart->LeftAxis->Minimum > 0)
      {
      Chart->LeftAxis->Minimum = 0;
      Chart->LeftAxis->Maximum = 0;
      }
   else
      {
      Chart->LeftAxis->Maximum = 0;
      Chart->LeftAxis->Minimum = 0;
      }
   //proteção
   if (RGroupSerie->ItemIndex < 0) return;
   //dtermina Serie selecionada
   Serie = (TChartSeries*)RGroupSerie->Items->Objects[RGroupSerie->ItemIndex];
   //redimensiona o eixo Y
   //Chart->LeftAxis->Maximum = 1.2 * Serie->MaxYValue();
   //Chart->LeftAxis->Minimum = 0.8 * Serie->MinYValue();
   //determina valores máximo e mínimo do eixo y
   val_max = Serie->MaxYValue();
   val_min = Serie->MinYValue();
   //inclui uma margem adicional nos valores máximo e mínimo
   delta = fabs(val_max) * 0.05;
   val_max += delta;
   val_min -= delta;
   //redimensiona o eixo Y
	if (val_max > 0)
      {
		Chart->LeftAxis->Maximum   = val_max;
      Chart->LeftAxis->Minimum   = val_min;
		}
   else
      {
      Chart->LeftAxis->Minimum   = val_min;
      Chart->LeftAxis->Maximum   = val_max;
		}
   //exibe a Serie
   Serie->Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormExibeCurva::DefineCurva(VTEqpto *eqpto, VTCurva *curva)
   {
   //salva ponteiro p/ objeto
   this->eqpto  = eqpto;
   this->curva  = curva;
   //reinicia CLBoxSerie
   RGroupSerie->Items->Clear();
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormExibeCurva::HabilitaCurva(int atributo_id, AnsiString caption)
   {
   //variáveis locais
   double       valor[nvCURVA_PQVT];
   strHM        hm;
   TChartSeries *Serie;
   VTVetval     *vetval;
   VTPatamar    *patamar;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //verifica se é possível incluir uma nova Curva
   if (RGroupSerie->Items->Count >= Chart->SeriesCount()) return;
   //determina Serie p/ a Curva
   Serie = Chart->Series[RGroupSerie->Items->Count];
   Serie->Title = caption;
   Serie->Clear();
   //obtém lista de valores do atributo indicado
   legenda->LisVetValFluxo(eqpto, lisVETVAL, atributo_id);
   if (lisVETVAL->Count == 0) return;
   //insere Serie no TChart p/ o Vetval
   vetval = (VTVetval*)lisVETVAL->First();
   ChartExibeCurva(vetval);
   //preenche a Série c/ os valores em todos patamares
   for (int np = 0; np < patamares->NumPatamar(); np++)
      {//determina Patamar
      patamar = patamares->Patamar[np];
      //determina valores da Curva  no patamar
      hm.hora   = patamar->Hora;
      hm.minuto = patamar->Minuto;
      if (! curva->GetValor(hm, valor, curva->NumeroValores)) continue;
      //insere ponto na Serie
      Serie->AddXY(np, valor[ind_valor], "", Serie->Color);
      //inclui ponto adicional p/ representar o fim do patamar(histograma)
      //serie->AddXY(np+1, val, IntToStr(np+1), cor);
      Serie->AddXY(np+patamar->Duracao, valor[ind_valor], "", Serie->Color);
      }
   //inclui Serie em RGroupSerie
   RGroupSerie->ItemIndex = RGroupSerie->Items->AddObject(caption, Serie);
   //força atualização do Chart
   ChartClick(NULL);
   }
*/

//---------------------------------------------------------------------------
void __fastcall TFormExibeCurva::HabilitaCurva(int atributo_id)
   {
   //variáveis locais
   int          index;
   double       valor;
   TChartSeries *Serie;
   VTVetval     *vetval;
   VTPatamar    *patamar;
   VTPatamares  *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //reinicia RGroupSerie
   //RGroupSerie->Items->Clear();
   //obtém lista de valores do atributo indicado
   legenda->LisVetValEqpto(eqpto, lisVETVAL, atributo_id);
   if (lisVETVAL->Count == 0) return;
   for (int n = 0; n < lisVETVAL->Count; n++)
      {
      vetval = (VTVetval*)lisVETVAL->Items[n];
      //verifica se existe ChartSeries disponível
      if (RGroupSerie->Items->Count >= Chart->SeriesCount()) return;
      //determina Serie p/ a Curva
      Serie = Chart->Series[RGroupSerie->Items->Count];
      Serie->Title = vetval->DescUnid();
      Serie->Clear();
      //insere Serie no TChart p/ o Vetval
      //preenche a Série c/ os valores em todos patamares
      for (int np = 0; np < patamares->NumPatamar(); np++)
         {//determina valor no patamar
         valor = vetval->Valor[np];
         //insere ponto na Serie
			Serie->AddXY(np,   valor, IntToStr(np+1), Serie->Color);
			Serie->AddXY(np+1, valor, IntToStr(np+1), Serie->Color);
         }
      //inclui Serie em RGroupSerie
      index = RGroupSerie->Items->AddObject(Serie->Title, Serie);
      if (index == 0)
         {
         RGroupSerie->ItemIndex = 0;
         //força atualização do Chart
         ChartClick(NULL);
         }
      }
   }

//---------------------------------------------------------------------------
//eof

