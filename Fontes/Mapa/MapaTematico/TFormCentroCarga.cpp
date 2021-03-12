//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCentroCarga.h"
#include "..\VTRegiao.h"
#include "..\VTRegioes.h"
#include "..\..\Apl\TApl.h"
#include "..\..\Geral\VTGeral.h"
#include "..\..\Grafico\VTGrafico.h"
#include "..\..\Mercado\VTMercado.h"
#include "..\..\Patamar\VTPatamar.h"
#include "..\..\Patamar\VTPatamares.h"
#include "..\..\Radial\VTRadial.h"
#include "..\..\Radial\VTSubestacao.h"
#include "..\..\Rede\VTRede.h"
#include "..\..\Rede\VTRedes.h"
#include "..\..\Rede\VTTipoRede.h"
#include "..\..\Redegraf\VTCelula.h"
#include "..\..\Redegraf\VTCelulas.h"
#include "..\..\Redegraf\VTRedegraf.h"
//#include "..\..\Stk\VTStk.h"
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\..\DLL_Inc\Consulta.h"
#include "..\..\..\DLL_Inc\Radial.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCentroCarga::TFormCentroCarga(TComponent* Owner, VTApl *apl_owner, TWinControl *parent)
   : TForm(Owner)
   {
   //salva ponteiro
   this->apl = apl_owner;
   //insere Form em seu Parent
   //if (parent) Parent = parent;
   //configura gráfico em modo geo-referenciado
   SelecionaModoGrafico(modoGEOREFERENCIADO);
   //inicia CLBoxSe
   CLBoxRegiaoInicia();
   //inicia CBoxPatamar e CBoxAno
   CBoxPatamarInicia();
   CBoxAnoInicia();
   //monta centros de carga p/ o ano/patamar selecionado
   Atualiza();
   }

//---------------------------------------------------------------------------
__fastcall TFormCentroCarga::~TFormCentroCarga(void)
   {
   //desabilita Timer
   if (Timer->Enabled) ActionAnimacaoExecute(NULL);
   //restaura modo gráfico
   //SelecionaModoGrafico(modoESQUEMATICO);
   //exibe Form Owner
   if (ActionOnClose) ActionOnClose->Execute();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::ActionAnimacaoExecute(TObject *Sender)
   {
   //habilita/desabilita Timer
   Timer->Enabled = ! Timer->Enabled;
   //atualiza Caption
	ActionAnimacao->Caption = (Timer->Enabled) ? "Parar" : "Iniciar";
	}

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::ActionCalculaCentroCargaExecute(TObject *Sender)
   {
   //recalcula densidade de carga e atualiza mapa temático
   Atualiza();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::ActionFechaExecute(TObject *Sender)
   {
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::ActionSelAllExecute(TObject *Sender)
   {
   //variáveis locais
   int    index;

   //seleciona todos items em CLBoxRegiao
   for (int n = 0; n < CLBoxRegiao->Items->Count; n++)
      {
      CLBoxRegiao->Checked[n] = true;
      }
   //apresenta centros de carga selecionados
   ExibeCentroCarga();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::ActionSelNoneExecute(TObject *Sender)
   {
   //variáveis locais
   int    index;

   //seleciona todos items em CLBoxRegiao
   for (int n = 0; n < CLBoxRegiao->Items->Count; n++)
      {
      CLBoxRegiao->Checked[n] = false;
      }
   //apresenta centros de carga selecionados
   ExibeCentroCarga();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::Atualiza(void)
   {
   //monta centros de carga p/ o patamar selecionado
   RecalculaCentroCarga();
   //apresenta centros de carga selecionados
   ExibeCentroCarga();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::CBoxAnoInicia(void)
   {
   //variáveis locais
   VTMercado  *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

   //reinicia CBoxPatamar c/ Patamares
   CBoxAno->Clear();
   for (int ano = mercado->AnoInicial; ano <= mercado->AnoFinal; ano++)
      {
      CBoxAno->Items->Add(IntToStr(ano));
      }
   //pré-seleciona primeiro ano
   if (CBoxAno->Items->Count > 0) CBoxAno->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::CBoxPatamarChange(TObject *Sender)
   {
   Atualiza();
   }

//---------------------------------------------------------------------
void __fastcall TFormCentroCarga::CBoxPatamarInicia(void)
   {
   //variáveis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //TList       *lisPAT    = patamares->LisPatamar();
   TList *lisPAT  = new TList();

   //guarda patamares habilitados
   patamares->LisPatamarHabilitado(lisPAT);

   //reinicia CBoxPatamar c/ Patamares
   CBoxPatamar->Clear();
   for (int np = 0; np < lisPAT->Count; np++)
      {
	   patamar = (VTPatamar*)lisPAT->Items[np];
      CBoxPatamar->Items->AddObject(patamar->Nome, patamar);
      }
   //pré-seleciona primeiro patamar
   if (CBoxPatamar->Items->Count > 0) CBoxPatamar->ItemIndex = 0;
   delete lisPAT;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::CLBoxRegiaoClickCheck(TObject *Sender)
   {
   //apresenta centros de carga selecionados
   ExibeCentroCarga();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::CLBoxRegiaoDblClick(TObject *Sender)
   {
   //variáveis locais
   VTRegiao *regiao;

   //determina Ccs selecionado
   if (CLBoxRegiao->ItemIndex < 0) return;
   if ((regiao = (VTRegiao*)CLBoxRegiao->Items->Objects[CLBoxRegiao->ItemIndex]) == NULL) return;
   //abre ColorDialog p/ selecionar cor da Ccs
   ColorDialog->Color = regiao->Color;
   if (ColorDialog->Execute())
      {//redefine cor da Ccs
      regiao->Color = ColorDialog->Color;
      //apresenta centro de carga das Regioes selecionadas
      ExibeCentroCarga();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::CLBoxRegiaoDrawItem(TWinControl *Control, int Index,
      TRect &Rect, TOwnerDrawState State)
   {
   //variáveis locais
   TColor      color_ori;
   TBrushStyle style_ori;
   TRect       rect_cor;
   TCanvas    *Canvas = CLBoxRegiao->Canvas;
   VTRegiao   *regiao = (VTRegiao*)(CLBoxRegiao->Items->Objects[Index]);

   //salva cor/style original
   color_ori = Canvas->Brush->Color;
   style_ori = Canvas->Brush->Style;
   //limpa a área do item
   Canvas->FillRect(Rect);
   //define área p/ desenhar o retângulo c/ a cor da Rede
   rect_cor.Top    = Rect.Top + 1;
   rect_cor.Bottom = Rect.Bottom - 1;
   rect_cor.Left   = Rect.Left;
   rect_cor.Right  = rect_cor.Left + (rect_cor.Bottom - rect_cor.Top);
   //desenha o retângulo c/ a Cor do Ccs
   Canvas->Brush->Color = regiao->Color;
   Canvas->Brush->Style = bsSolid;
   Canvas->FillRect(rect_cor);
   //restaura cor/style original
   Canvas->Brush->Color = color_ori;
   Canvas->Brush->Style = style_ori;
   //escreve nome das Subestacao
   Canvas->TextOut(rect_cor.Right + 1, Rect.Top, regiao->Codigo);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::CLBoxRegiaoInicia(void)
   {
   //variáveis locais
   int       index;
   VTRegiao  *regiao;
   VTRegioes *regioes   = (VTRegioes*)apl->GetObject(__classid(VTRegioes));
   TList     *lisREGIAO = regioes->LisRegiao();

   //reinicia CLBoxRegiao
   CLBoxRegiao->Clear();
   for (int n = 0; n < lisREGIAO->Count; n++)
      {
      regiao = (VTRegiao*)lisREGIAO->Items[n];
      index  = CLBoxRegiao->Items->AddObject(regiao->Codigo, regiao);
      CLBoxRegiao->Checked[index] = true;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::ExibeCentroCarga(void)
   {
   //cancela cor das Celulas
   MapaTematicoCancela();
   //remonta cor das Celulas
   MapaTematicoMonta();
   //habilita mapa temático
   MapaTematicoHabilita(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::FormShow(TObject *Sender)
   {
   //reposiciona o Form
   Left = 0;
   Top  = 0;
   }

//---------------------------------------------------------------------------
int __fastcall TFormCentroCarga::IndicePatamarSelecionado(void)
	{
   //variáveis locais
   VTPatamar *patamar;

   //verifica o patamar selecionado
   if (CBoxPatamar->ItemIndex < 0) return(-1);
   patamar = (VTPatamar*)(CBoxPatamar->Items->Objects[CBoxPatamar->ItemIndex]);
   return(patamar->Index);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::MapaTematicoCancela(void)
   {
   //variáveis locais
   VTCelula   *celula;
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   VTCelulas  *celulas  = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList      *lisCEL   = celulas->LisCelula();

   for (int i = 0; i < lisCEL->Count; i++)
      {
      celula = (VTCelula*)lisCEL->Items[i];
      celula->Color = redegraf->CorFundo;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::MapaTematicoHabilita(bool enabled)
   {
   //variáveis locais
   VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //habilita/desabilita apresentação do mapa temático
   grafico->MapaTematicoEnabled = enabled;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::MapaTematicoMonta(void)
   {
   //variáveis locais
   VTRegiao   *regiao;
   VTCelula   *celula;
   VTCelulas  *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));

   //loop p/ todas Regioes selecionadas
   for (int n = 0; n < CLBoxRegiao->Items->Count; n++)
      {
      if (! CLBoxRegiao->Checked[n]) continue;
      regiao = (VTRegiao*)CLBoxRegiao->Items->Objects[n];
      //determina Celula do centro de carga
      if ((celula = celulas->ExisteCelula(regiao->Centrocarga_cm.x, regiao->Centrocarga_cm.y)) == NULL) continue;
      //pinta Celula do centro de carga com cor preta
      celula->Color = regiao->Color;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::RecalculaCentroCarga(void)
   {
   //variáveis locais
   int          ind_pat;
   VTRegiao     *regiao;
   VTMercado    *mercado = (VTMercado*)apl->GetObject(__classid(VTMercado));

   //redefine ano do Mercado
   mercado->AnoAtual = mercado->AnoInicial + CBoxAno->ItemIndex;
   //determina ano e patamar selecionado
	ind_pat = IndicePatamarSelecionado();
   //recalcula centro de carga de todas Regioes
   for (int n = 0; n < CLBoxRegiao->Items->Count; n++)
      {
      regiao = (VTRegiao*)CLBoxRegiao->Items->Objects[n];
      regiao->CalculaCentroCarga(ind_pat);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::SelecionaModoGrafico(int modo)
   {
   //variáveis locais
   VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

   //seleciono modo gráfico indicado
   redegraf->ModoGrafico = modo;
   grafico->ModoGraficoAlterado();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCentroCarga::TimerTimer(TObject *Sender)
	{
	//verifica se está habilitada alteração do patamar
	if (CheckBoxPatamar->Checked)
		{//seleciona novo patamar
		if (CBoxPatamar->ItemIndex < (CBoxPatamar->Items->Count - 1))
			{//seleciona próximo patamar
			CBoxPatamar->ItemIndex = CBoxPatamar->ItemIndex + 1;
			}
      else
         {//seleciona primeiro patamar
         CBoxPatamar->ItemIndex = 0;
         //verifica se está habilitada alteração do ano
         if (CheckBoxAno->Checked)
				{//seleciona novo ano
            if (CBoxAno->ItemIndex < (CBoxAno->Items->Count - 1))
               {//seleciona próximo ano
               CBoxAno->ItemIndex = CBoxAno->ItemIndex + 1;
               }
            else
               {
               CBoxAno->ItemIndex = 0;
               }
            }
         }
      }
   //verifica se está habilitada a alteração do ano
   else if (CheckBoxAno->Checked)
      {//seleciona novo ano
      if (CBoxAno->ItemIndex < (CBoxAno->Items->Count - 1))
         {//seleciona próximo ano
         CBoxAno->ItemIndex = CBoxAno->ItemIndex + 1;
         }
      else
         {
         CBoxAno->ItemIndex = 0;
         }
      }
   //recalcula densidade de carga e atualiza mapa temático
   Atualiza();
   }

//---------------------------------------------------------------------------
//eof

