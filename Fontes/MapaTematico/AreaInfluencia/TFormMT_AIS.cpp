//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormMT_AIS.h"
#include "VTAis.h"
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
//#include "..\..\..\DLL_Inc\Consulta.h"
#include "..\..\..\DLL_Inc\Help.h"
#include "..\..\..\DLL_Inc\Radial.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormMT_AIS::TFormMT_AIS(TComponent* Owner, VTApl *apl_owner, TWinControl *parent)
   : TForm(Owner)
   {
   //insere Form em seu Parent
   if (parent) Parent = parent;
   //cria objetos
   apl = new TApl(this, apl_owner);
   //cria objetos
   lisAIS = new TList();
   apl->Add(radial   = DLL_NewObjRadial(apl));
   //apl->Add(consulta = DLL_NewObjConsulta(apl));
   //inicia objetos
   radial->Inicia((VTRedes*)apl->GetObject(__classid(VTRedes)));
   //configura gr�fico em modo geo-referenciado
   SelecionaModoGrafico(modoGEOREFERENCIADO);
   //inicia listas de Ais
   IniciaAis();
   //inicia LViewAis
   LViewAisInicia();
   //inicia CBoxPatamar
   CBoxPatamarInicia();
/*
   //monta �reas de influ�ncia p/ o patamar selecionado
   RecalculaAreaInfluencia();
   //apresenta �rea de influ�ncia selecionadas
   ExibeAisSelecionada();
*/
   //recalcula e exibe �rea de influ�ncia
   ActionOptionChange->Execute();
   }

//---------------------------------------------------------------------------
__fastcall TFormMT_AIS::~TFormMT_AIS(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //desabilita Timer
   Timer->Enabled = false;
   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //desabilita mapa tem�tico
   HabilitaMapaTematico(false);
   //restaura modo gr�fico
   SelecionaModoGrafico(modoESQUEMATICO);
   //destr�i objetos
   if (apl) {delete apl; apl = NULL;}
   //destr�i lista e destr�i seus objetos
   if (lisAIS) {LimpaTList(lisAIS); delete lisAIS; lisAIS = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::ActionAnimacaoExecute(TObject *Sender)
   {
   //atualiza lista de patamares habilitados
//   CBoxPatamarInicia();    //DVK 2013.07.17
   //habilita/desabilita Timer
   Timer->Enabled = ! Timer->Enabled;
   //atualiza Caption
   ActionAnimacao->Caption = (Timer->Enabled) ? "Parar" : "Iniciar";
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::ActionFechaExecute(TObject *Sender)
   {
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::ActionHelpExecute(TObject *Sender)
{  //variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Mapa_Tem�tico_�rea_de_Influ�ncia");
	if(formHelp) formHelp->Show();

}
//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::ActionOptionChangeExecute(TObject *Sender)
   {
   //verifica se o Timer est� habilitado
   if (Timer->Enabled)
      {//deixa que a atualiza��o seja feita pelo Timer
      return;
      }
   //exibe/esconde componentes
   PanelOpcao->Visible = (RGroupCarga->ItemIndex == 0);
   //Timer desabilitado: atualiza �reas de influ�ncia
   Atualiza();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::ActionSelAllExecute(TObject *Sender)
   {
   //seleciona todos items em CLBoxAIS
   for (int n = 0; n < LViewAis->Items->Count; n++)
      {
      LViewAis->Items->Item[n]->Checked = true;
      }
   //apresenta �rea de influ�ncia selecionadas
   ExibeAisSelecionada();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::ActionSelNoneExecute(TObject *Sender)
   {
   //seleciona todos items em CLBoxAIS
   for (int n = 0; n < LViewAis->Items->Count; n++)
      {
      LViewAis->Items->Item[n]->Checked = false;
      }
   //apresenta �rea de influ�ncia selecionadas
   ExibeAisSelecionada();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::Atualiza(void)
   {
   //cancela cor das Celulas de todas as Ais
   CancelaMapaTematicoAtual();
   //monta �reas de influ�ncia p/ o patamar selecionado
   RecalculaAreaInfluencia();
   //apresenta �rea de influ�ncia selecionadas
   ExibeAisSelecionada();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::CancelaMapaTematicoAtual(void)
   {
   //vari�veis locais
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
void __fastcall TFormMT_AIS::CBoxPatamarInicia(void)
   {
   //vari�veis locais
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
   //pr�-seleciona primeiro patamar
   if (CBoxPatamar->Items->Count > 0) CBoxPatamar->ItemIndex = 0;
   delete lisPAT;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::ExibeAisSelecionada(void)
   {
   //vari�veis locais
   VTAis *ais;

   //cancela cor das Celulas de todas as Ais
   CancelaMapaTematicoAtual();
   //define Cor das celulas da Ais selecionadas
   for (int n = 0; n < LViewAis->Items->Count; n++)
      {
      ais = (VTAis*)LViewAis->Items->Item[n]->Data;
      ais->Visible = LViewAis->Items->Item[n]->Checked;
      if (ais->Visible) ais->PintaCelulas(ais->Color);
      }
   //habilita mapa tem�tico
   HabilitaMapaTematico(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destr�i o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona no canto superior esquerdo
   Top  = 0;
   Left = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
	}

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::HabilitaMapaTematico(bool enabled)
   {
   //vari�veis locais
   VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //habilita/desabilita apresenta��o do mapa tem�tico
   grafico->MapaTematicoEnabled = enabled;
	}

//---------------------------------------------------------------------------
int __fastcall TFormMT_AIS::IndicePatamarSelecionado(void)
	{
   //vari�veis locais
   VTPatamar *patamar;

   //verifica o patamar selecionado
   if (CBoxPatamar->ItemIndex < 0) return(-1);
	patamar = (VTPatamar*)(CBoxPatamar->Items->Objects[CBoxPatamar->ItemIndex]);
   return(patamar->Index);
	}
//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::IniciaAis(void)
   {
   //vari�veis locais
   int          index = 0;
   VTAis        *ais;
   VTSubestacao *subestacao;
   TList        *lisSE = radial->LisSubestacao();
   VTGeral      *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //reinicia lisAIS
   LimpaTList(lisAIS);
   //cria um objeto Ais p/ cada Subestacao
   for (int n = 0; n < lisSE->Count; n++)
      {
      subestacao = (VTSubestacao*)lisSE->Items[n];
      lisAIS->Add(ais = NewObjAis(apl, subestacao));
      //se necess�rio, troca cor da Ais
      if (ais->Color == subestacao->Rede->TipoRede->Color)
         {
         ais->Color = geral->COR.Circ[index];
         index = ++index % MAX_COR;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::LViewAisClick(TObject *Sender)
   {
   //vari�veis locais
   VTAis *ais;

   //verifica se mudou a op��o de exibir/esconder alguma Ais
   for (int n = 0; n < LViewAis->Items->Count; n++)
      {
      ais = (VTAis*)(LViewAis->Items->Item[n]->Data);
      if (ais->Visible != LViewAis->Items->Item[n]->Checked)
         {//h� mudan�a de op��o de exibir uma Ais: apresenta �rea de influ�ncia selecionadas
         ExibeAisSelecionada();
         break;;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::LViewAisCustomDrawSubItem(TCustomListView *Sender,
      TListItem *Item, int SubItem, TCustomDrawState State, bool &DefaultDraw)
   {
   //vari�veis locais
   VTAis *ais = (VTAis*)Item->Data;

   if (SubItem == Item->SubItems->Count)
      {
      LViewAis->Canvas->Brush->Color = ais->Color;
      LViewAis->Canvas->Brush->Style = bsSolid;
      }
   else
      {
      LViewAis->Canvas->Brush->Color = clWindow;
      LViewAis->Canvas->Brush->Style = bsSolid;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::LViewAisDblClick(TObject *Sender)
   {
   //vari�veis locais
   VTAis  *ais;
   //determina Ais selecionada
   if (LViewAis->Selected == NULL) return;
   if ((ais = (VTAis*)LViewAis->Selected->Data) == NULL) return;
   //abre ColorDialog p/ selecionar cor da Ais
   ColorDialog->Color = ais->Color;
   if (ColorDialog->Execute())
      {//redefine cor da Ais
      ais->Color = ColorDialog->Color;
      //apresenta �rea de influ�ncia selecionadas
      ExibeAisSelecionada();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::LViewAisInicia(void)
   {
   //vari�veis locais
   int       num_trafo;
   TListItem *Item;
   VTAis      *ais;

   //reinicia CLBoxAIS
   LViewAis->Items->Clear();
   for (int n = 0; n < lisAIS->Count; n++)
      {
      ais   = (VTAis*)lisAIS->Items[n];
      //determina n�mero de trafos e trafo3e
      num_trafo = ais->Subestacao->Rede->NumeroDeLigacoes(eqptoTRAFO) +
                  ais->Subestacao->Rede->NumeroDeLigacoes(eqptoTRAFO3E);
      Item  = LViewAis->Items->Add();
      Item->Data    = ais;
      Item->Caption = ais->Subestacao->Rede->Codigo;
      Item->SubItems->Add(IntToStr(num_trafo));
      Item->SubItems->Add(DoubleToStr("%4.3f", ais->Subestacao->Snom_mva));
      Item->SubItems->Add(DoubleToStr("%4.3f", ais->Subestacao->Sfirme_mva));
      Item->SubItems->Add("");
      Item->Checked = true;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::RecalculaAreaInfluencia(void)
   {
   //vari�veis locais
   strOPCAO_AIS opcao;
   VTAis       *ais;

   //determina op��es de c�lculo da �rea de influ�ncia selecionadas pelo usu�rioa
   opcao.sfirme        = (RGroupCapacidade->ItemIndex == 1);
   opcao.carga_propria = (RGroupCarga->ItemIndex == 1);
   opcao.ind_pat       = IndicePatamarSelecionado();
   opcao.cargto_max_pu = StrToDouble(CBoxCargto->Text) / 100.;
   //identifica as Ais selecionadas
   for (int n = 0; n < lisAIS->Count; n++)
      {
      ais = (VTAis*)lisAIS->Items[n];
      ais->MontaAreaInfluencia(opcao);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::SelecionaModoGrafico(int modo)
   {
   //vari�veis locais
   VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

   //seleciono modo rg�fico indicado
   redegraf->ModoGrafico = modo;
   grafico->ModoGraficoAlterado();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMT_AIS::TimerTimer(TObject *Sender)
   {
   //seleciona novo patamar
   if (CBoxPatamar->ItemIndex < (CBoxPatamar->Items->Count - 1))
      {//seleciona pr�ximo patamar
      CBoxPatamar->ItemIndex = CBoxPatamar->ItemIndex + 1;
      }
   else
      {//seleciona primeiro patamar
      CBoxPatamar->ItemIndex = 0;
      }
   //recalcula centros de carga e atualiza mapa tem�tico
   Atualiza();
   }

//---------------------------------------------------------------------------
//eof




