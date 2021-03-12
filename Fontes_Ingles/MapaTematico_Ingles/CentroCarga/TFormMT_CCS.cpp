//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormMT_CCS.h"
#include "VTCcs.h"
#include "..\..\Apl\TApl.h"
#include "..\..\Geral\VTGeral.h"
#include "..\..\Geral\VTInfoset.h"
#include "..\..\Grafico\VTGrafico.h"
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
#include "..\..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\..\DLL_Inc\Consulta.h"
#include "..\..\..\DLL_Inc\Radial.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormMT_CCS::TFormMT_CCS(TComponent* Owner, VTApl *apl_owner, TWinControl *parent)
   : TForm(Owner)
   {
   //insere Form em seu Parent
   if (parent) Parent = parent;
   //cria objetos
   apl = new TApl(this, apl_owner);
   //cria objetos
   lisCCS = new TList();
   apl->Add(radial   = DLL_NewObjRadial(apl));
   //apl->Add(consulta = DLL_NewObjConsulta(apl));
   //inicia objetos
   radial->Inicia((VTRedes*)apl->GetObject(__classid(VTRedes)));
   //configura gráfico em modo geo-referenciado
   SelecionaModoGrafico(modoGEOREFERENCIADO);
   //inicia listas de Ccs
   IniciaCcs();
   //inicia CLBoxSe
   CLBoxCcsInicia();
   //inicia CBoxPatamar
   CBoxPatamarInicia();
   //monta centros de carga p/ o patamar selecionado
   RecalculaCentroCarga();
   //apresenta centros de cargasselecionadas
   ExibeCcsSelecionada();
   }

//---------------------------------------------------------------------------
__fastcall TFormMT_CCS::~TFormMT_CCS(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //desabilita Timer
   if (Timer->Enabled) ActionAnimacaoExecute(NULL);
   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //desabilita mapa temático
   HabilitaMapaTematico(false);
   //restaura modo gráfico
   SelecionaModoGrafico(modoESQUEMATICO);
   //destrói objetos
   if (apl) {delete apl; apl = NULL;}
   //destrói lista e destrói seus objetos
   if (lisCCS) {LimpaTList(lisCCS); delete lisCCS; lisCCS = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::ActionAnimacaoExecute(TObject *Sender)
   {
   //atualiza lista de patamares habilitados
//   CBoxPatamarInicia();     //DVK 2013.07.17
   //variáveis locais
   //VTStk *stk = (VTStk*)apl->GetObject(__classid(VTStk));

   //habilita/desabilita Editor
   //stk->ActionEditor->Enabled = Timer->Enabled;
   //habilita/desabilita Timer
   Timer->Enabled = ! Timer->Enabled;
   //atualiza Caption
	ActionAnimacao->Caption = (Timer->Enabled) ? "Parar" : "Iniciar";
	}

//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::ActionFechaExecute(TObject *Sender)
   {
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::ActionHelpExecute(TObject *Sender)
{  //variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Mapa_Temático_Centro_de_Carga");
	if(formHelp) formHelp->Show();
}
//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::ActionSelAllExecute(TObject *Sender)
   {
   //variáveis locais
   int    index;

   //seleciona todos items em CLBoxCCS
   for (int n = 0; n < lisCCS->Count; n++)
      {
      CLBoxCCS->Checked[n] = true;
      }
   //apresenta centros de carga selecionados
   ExibeCcsSelecionada();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::ActionSelNoneExecute(TObject *Sender)
   {
   //variáveis locais
   int    index;

   //seleciona todos items em CLBoxCCS
   for (int n = 0; n < lisCCS->Count; n++)
      {
      CLBoxCCS->Checked[n] = false;
      }
   //apresenta centros de carga selecionados
   ExibeCcsSelecionada();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::Atualiza(void)
   {
   //cancela cor das Celulas de todas as Ccs
   CancelaMapaTematicoAtual();
   //monta centros de carga p/ o patamar selecionado
   RecalculaCentroCarga();
   //apresenta centros de carga selecionados
   ExibeCcsSelecionada();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::CancelaMapaTematicoAtual(void)
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
void __fastcall TFormMT_CCS::CBoxPatamarChange(TObject *Sender)
   {
   Atualiza();
   }

//---------------------------------------------------------------------
void __fastcall TFormMT_CCS::CBoxPatamarInicia(void)
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
void __fastcall TFormMT_CCS::CLBoxCCSClickCheck(TObject *Sender)
   {
   //apresenta centros de carga selecionados
   ExibeCcsSelecionada();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::CLBoxCCSDblClick(TObject *Sender)
   {
   //variáveis locais
   VTCcs  *ccs;

   //determina Ccs selecionado
   if (CLBoxCCS->ItemIndex < 0) return;
   if ((ccs = (VTCcs*)CLBoxCCS->Items->Objects[CLBoxCCS->ItemIndex]) == NULL) return;
   //abre ColorDialog p/ selecionar cor da Ccs
   ColorDialog->Color = ccs->Color;
   if (ColorDialog->Execute())
      {//redefine cor da Ccs
      ccs->Color = ColorDialog->Color;
      //apresenta centros de carga selecionados
      ExibeCcsSelecionada();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::CLBoxCCSDrawItem(TWinControl *Control, int Index,
      TRect &Rect, TOwnerDrawState State)
   {
   //variáveis locais
   TColor      color_ori;
   TBrushStyle style_ori;
   TRect       rect_cor;
   TCanvas    *Canvas = CLBoxCCS->Canvas;
   VTCcs      *ccs    = (VTCcs*)(CLBoxCCS->Items->Objects[Index]);

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
   Canvas->Brush->Color = ccs->Color;
   Canvas->Brush->Style = bsSolid;
   Canvas->FillRect(rect_cor);
   //restaura cor/style original
   Canvas->Brush->Color = color_ori;
   Canvas->Brush->Style = style_ori;
   //escreve nome das Subestacao
   Canvas->TextOut(rect_cor.Right + 1, Rect.Top, ccs->Subestacao->Rede->Codigo);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::CLBoxCcsInicia(void)
   {
   //variáveis locais
   int    index;
   VTCcs  *ccs;

   //reinicia CLBoxCCS
   CLBoxCCS->Clear();
   for (int n = 0; n < lisCCS->Count; n++)
      {
      ccs   = (VTCcs*)lisCCS->Items[n];
      index = CLBoxCCS->Items->AddObject(ccs->Subestacao->Rede->Codigo, ccs);
      CLBoxCCS->Checked[index] = true;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::ExibeCcsSelecionada(void)
   {
   //variáveis locais
   VTCcs *ccs;

   //cancela cor das Celulas de todas as Ccs
   CancelaMapaTematicoAtual();
   //define Cor das celulas da Ccs selecionados
   for (int n = 0; n < CLBoxCCS->Items->Count; n++)
      {
      ccs = (VTCcs*)CLBoxCCS->Items->Objects[n];
      ccs->Visible = CLBoxCCS->Checked[n];
      if (ccs->Visible) ccs->PintaCelulas(ccs->Color);
      }
   //habilita mapa temático
   HabilitaMapaTematico(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona no canto superior esquerdo
   Top  = 0;
   Left = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::HabilitaMapaTematico(bool enabled)
   {
   //variáveis locais
   VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //habilita/desabilita apresentação do mapa temático
   grafico->MapaTematicoEnabled = enabled;
   }

//---------------------------------------------------------------------------
int __fastcall TFormMT_CCS::IndicePatamarSelecionado(void)
	{
   //variáveis locais
   VTPatamar *patamar;

   //verifica o patamar selecionado
   if (CBoxPatamar->ItemIndex < 0) return(-1);
   patamar = (VTPatamar*)(CBoxPatamar->Items->Objects[CBoxPatamar->ItemIndex]);
   return(patamar->Index);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::IniciaCcs(void)
   {
   //variáveis locais
   int          index = 0;
   VTCcs        *ccs;
   VTSubestacao *subestacao;
   TList        *lisSE = radial->LisSubestacao();
   VTGeral      *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //reinicia lisCCS
   LimpaTList(lisCCS);
   //cria um objeto Ccs p/ cada Subestacao
   for (int n = 0; n < lisSE->Count; n++)
      {
      subestacao = (VTSubestacao*)lisSE->Items[n];
      lisCCS->Add(ccs = NewObjCcs(apl, subestacao));
      //se necessário, troca cor da Ccs
      if (ccs->Color == subestacao->Rede->TipoRede->Color)
         {
         ccs->Color = geral->COR.Circ[index];
         index = ++index % MAX_COR;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::RecalculaCentroCarga(void)
   {
   //variáveis locais
   int   ind_pat;
   VTCcs *ccs;

   //proteção: determina patamar selecionado
	ind_pat = IndicePatamarSelecionado();
   //identifica os Ccs selecionados
   for (int n = 0; n < lisCCS->Count; n++)
      {
      ccs = (VTCcs*)lisCCS->Items[n];
      ccs->MontaCentroCarga(ind_pat);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::SelecionaModoGrafico(int modo)
   {
   //variáveis locais
   VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

   //seleciono modo gráfico indicado
   redegraf->ModoGrafico = modo;
   grafico->ModoGraficoAlterado();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_CCS::TimerTimer(TObject *Sender)
   {
   //seleciona novo patamar
   if (CBoxPatamar->ItemIndex < (CBoxPatamar->Items->Count - 1))
      {//seleciona próximo patamar
      CBoxPatamar->ItemIndex = CBoxPatamar->ItemIndex + 1;
      }
   else
      {//seleciona primeiro patamar
      CBoxPatamar->ItemIndex = 0;
      }
   //recalcula centros de carga e atualiza mapa temático
   Atualiza();
   }

//---------------------------------------------------------------------------
//eof



