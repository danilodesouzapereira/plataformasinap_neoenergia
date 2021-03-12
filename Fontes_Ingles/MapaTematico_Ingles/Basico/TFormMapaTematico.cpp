//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TFormMapaTematico.h"
#include "VTTema.h"
#include "VTCfgCor.h"
#include "TTema1.h"
#include "Energia\TFormCorEnergia.h"
#include "Energia\TTemaEnergia.h"
#include "Tensao\TFormCorTensao.h"
#include "Tensao\TTemaTensao.h"
#include "..\Apl\VTApl.h"
#include "..\Edita\VTEdita.h"
#include "..\Flow\VTFlow.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Mercado\VTMercado.h" //FKM
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Progresso\VTProgresso.h" //FKM
#include "..\Rede\VTRedes.h"
#include "..\Redegraf\VTCelula.h"
#include "..\Redegraf\VTCelulas.h"
#include "..\..\Redegraf\VTRedegraf.h" //FKM
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Progresso.h"  //FKM

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormMapaTematico::TFormMapaTematico(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
   : TFormEdt(Owner, parent)
   {//variáveis locais
   VTMercado  *mercado  = (VTMercado*)apl_owner->GetObject(__classid(VTMercado));
   VTRedegraf *redegraf = (VTRedegraf*)apl_owner->GetObject(__classid(VTRedegraf));


   //salva ponteiro p/ objeto
   apl = apl_owner;
   //cria progresso
   progresso = DLL_NewObjProgresso(this, NULL); //FKM
   //cria objetos
   apl->Add(NewObjCfgCor());
   lisTEMA  = new TList();
   //inicia CBoxPatamar
   CBoxPatamarInicia();
   //cria temas básicos
   CBoxTemaInicia();
   //redefine número de celulas
   SelecionaResolucao();
   //FKM 2015.09.17 \\
   panelMouse->Visible = false;
   //FKM 2015.09.17 //
   //FKM 2015.10.01 \\
   //salva modo gráfico
   original.modo_grafico = redegraf->ModoGrafico;
   //configura gráfico em modo geo-referenciado
   SelecionaModoGrafico(modoGEOREFERENCIADO);
   CBoxResolucaoInicia();
   //atualiza
   CBoxResolucaoChange(NULL);
   //FKM 2015.10.01 //
   progresso = DLL_NewObjProgresso(this, NULL); //FKM
   }

//---------------------------------------------------------------------------
__fastcall TFormMapaTematico::~TFormMapaTematico(void)
   {
   //variáveis locais
   VTGeral   *geral   = (VTGeral*)apl->GetObject(__classid(VTGeral));
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //desabilita o Timer
   Timer->Enabled = false;
   //retira ActionAtualiza de Edita
   //CadastraActionAtualiza(false);
   //atualiza o gráfico: desabilita mapa temático
   grafico->MapaTematicoEnabled = false;
   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destrói lista sem destruir seus objetos
   if (lisTEMA) {delete lisTEMA; lisTEMA = NULL;}
   //FKM 2015.10.01 \\
   //restaura modo gráfico
   SelecionaModoGrafico(original.modo_grafico);
   //FKM 2015.10.01 //
   if(progresso) {delete progresso; progresso = NULL;}

   }

//---------------------------------------------------------------------------
void __fastcall TFormMapaTematico::ActionAtualizaExecute(TObject *Sender)
   {
   ExibeTemaSelecionado();
   }

//---------------------------------------------------------------------------
//FKM - 2015.10.01 //atualiza somente a cor \\
void __fastcall TFormMapaTematico::ActionAtualizaCorExecute(TObject *Sender)
   {
   VTTema    *tema;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //determina Mapa
   tema = (VTTema*)(CBoxTema->Items->Objects[CBoxTema->ItemIndex]);
   //atualiza cores das celulas
   tema->DefineCorCelula();
   //atualiza o gráfico: exibe mapa temático
   grafico->MapaTematicoEnabled = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMapaTematico::ActionHelpExecute(TObject *Sender)
{  //variaveis locais
	TForm *formHelp;

	formHelp = DLL_NewFormHelp(this, apl, "Mapa_Temático_Energia_Tensão");
	if(formHelp) formHelp->Show();
}
//---------------------------------------------------------------------------
void __fastcall TFormMapaTematico::ActionPlayExecute(TObject *Sender)
   {
   //habilita/desabilita Timer
   Timer->Enabled = ! Timer->Enabled;
   //atualiza Caption do GBoxPatamar
   GBoxPatamar->Caption = (Timer->Enabled) ? "Patamar: automático" : "Patamar";
   BitBtn1->Caption = (Timer->Enabled) ? "Parar" : "Iniciar";
	}

//---------------------------------------------------------------------------
void __fastcall TFormMapaTematico::Atualiza(void)
   {
   ExibeTemaSelecionado();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMapaTematico::CBoxPatamarChange(TObject *Sender)
   {
   //atualiza o mapa temático
   ExibeTemaSelecionado();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMapaTematico::CBoxPatamarInicia(void)
   {
   //variáveis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //reinicia CBoxPatamar
   CBoxPatamar->Clear();
   //insere opção "todos patamares"
   CBoxPatamar->Items->AddObject("todos patamares", NULL);
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      patamar = patamares->Patamar[ind_pat];
      if (patamar->Enabled)
         {
         CBoxPatamar->Items->AddObject(patamar->Nome, patamar);
         }
      }
   if (CBoxPatamar->Items->Count >= 2) CBoxPatamar->ItemIndex = 1;
   }

//---------------------------------------------------------------------------
//void __fastcall TFormMapaTematico::CBoxResolucaoChange(TObject *Sender)
//   {
//   //redefine número de celulas
//   SelecionaResolucao();
//   //atualiza o mapa temático
//   ExibeTemaSelecionado();
//   }
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TFormMapaTematico::CBoxResolucaoInicia(void)
   {
   //variáveis locais
   int        dim_max, dim_min, exp, delta;
   int        dim_ref[] = {100, 200, 250, 300, 400, 500, 1000, 2000, 5000};
   VTArea     *area;
   VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));
   VTRedes    *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //determina área da rede
   area = redegraf->Area(redes);
   //determina dimensão da celula p/ que resulte um grid com 50x50 celulas
   dim_max = (max(area->Width(), area->Height())) / 50;
   //determina dimensão mínima da célula p/ que resulte em um grid de 80x80
   dim_min = (max(area->Width(), area->Height())) / 80;
   //converte dimensões p/ metros
   if ((dim_max *= 0.01) <= 0) dim_max = 1;
   if ((dim_min *= 0.01) <= 0) dim_min = 1;
   //arredonda dim_max p/ valor múltiplo de 10
	exp = Log10(double(dim_max));
   dim_max = RoundTo(double(dim_max), exp);
   //arredonda dim_min p/ valor múltiplo de 10
	exp = Log10(double(dim_min));
   dim_min = RoundTo(double(dim_min), exp);
   if (dim_max <= dim_min) dim_max++;
   //determina incremento
   delta = dim_max - dim_min;
   if (delta < 10)
      {delta  = 1;}
   else
      {
      exp   = Log10(double(delta));
      delta = RoundTo(double(delta), exp) / 5;
      }
   //preenche CBoxResolucao
   CBoxResolucao->Clear();
   //preenche dimensões obrigatórias no início
   for (int n = 0; n < sizeof(dim_ref)/sizeof(int); n++)
      {
      if (dim_min > dim_ref[n])
         {//insere opção obrigatória de quadrícula
         CBoxResolucao->Items->Add(DoubleToStr("%5.1f",  dim_ref[n]));
         }
      }
   //preenche com as opções entre dim_min e dim_max
   for (int dim = dim_min; dim <= (dim_max + delta); dim += delta)
      {
      CBoxResolucao->Items->Add(DoubleToStr("%5.1f", dim));
      }
   //preenche dimensões obrigatórias no final
   for (int n = 0; n < sizeof(dim_ref)/sizeof(int); n++)
      {
      if (dim_max < dim_ref[n])
         {//insere opção obrigatória de quadrícula
         CBoxResolucao->Items->Add(DoubleToStr("%5.1f",  dim_ref[n]));
         }
      }
   //pré seleciona valor intermediário
   CBoxResolucao->ItemIndex = (CBoxResolucao->Items->Count-1) / 2;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMapaTematico::CBoxResolucaoChange(TObject *Sender)
   {
   //variáveis locais
   int        dim_celula_m;
   VTCelulas  *celulas  = (VTCelulas*)apl->GetObject(__classid(VTCelulas));


   //determina resolução (dimensão da célula)
   dim_celula_m = StrToDouble(CBoxResolucao->Text);
   //redefine Grid de células
   celulas->DefineGridQuadrado(dim_celula_m);
   //recalcula densidade de carga e atualiza o gráfico
   Atualiza();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMapaTematico::CBoxTemaChange(TObject *Sender)
   {
   if (formCOR){formCOR->Close(); delete formCOR; formCOR = NULL;}
   if (CBoxTema->ItemIndex == 0)
      {//esconde GBoxPatamar
      GBoxPatamar->Visible = false;
      //cria FormCorEnergia
      //formCOR = new TFormCorEnergia(this, apl, ActionAtualiza);
      formCOR = new TFormCorEnergia(this, apl, ActionAtualizaCor);//FKM 2015.10.01
      }
   else
      {//exibe GBoxPatamar
      GBoxPatamar->Visible = true;
      //cria FormCorTensao
      //formCOR = new TFormCorTensao(this, apl, ActionAtualiza);
      formCOR = new TFormCorTensao(this, apl, ActionAtualizaCor); //FKM 2015.10.01
      //se necessário, executa cálculo de fluxo
      ExecutaCalculoFluxo();
      }
   GBoxCor->ClientHeight = formCOR->Height + 25;
   GBoxCor->ClientWidth  = formCOR->Width;
   Refresh();
   formCOR->Parent = GBoxCor;
   formCOR->Align = alClient;
   formCOR->Show();
   formCOR->Refresh();
   ExibeTemaSelecionado();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMapaTematico::CBoxTemaInicia(void)
   {
   //reinicia CBoxTema
   CBoxTema->Clear();
   //insere temas básicos
   //InsereTema(new TTema1(apl));
   InsereTema(new TTemaEnergia(apl));
   InsereTema(new TTemaTensao(apl));
   //pré-seleciona primeiro Tema
   if (CBoxTema->Items->Count > 0)
      {
      CBoxTema->ItemIndex = 0;
      }
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TFormMapaTematico::ExecutaCalculoFluxo(void)
   {
   //variáveis locais
   VTStk   *stk      = (VTStk*)apl->GetObject(__classid(VTStk));

   //executa Flow
   stk->ActionFlow->Execute();

   return(true);
   }
*/

//---------------------------------------------------------------------------
bool __fastcall TFormMapaTematico::ExecutaCalculoFluxo(void)
   {
   //variáveis locais
   bool     sucesso;
   VTFlow  *flow     = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTRedes *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //verifica se o fluxo está atualizado
   if (flow->UpToDate) return(true);
   //executa Flow
   sucesso = flow->CalculaPorRede(redes);

   return(sucesso);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMapaTematico::ExibeTemaSelecionado(void)
   {
   //variáveis locais
   int       ind_pat;
   VTTema    *tema;
   VTPatamar *patamar;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   //VTProgresso *progresso = DLL_NewObjProgresso(this, NULL); //FKM

   //verifica se há um Tema selecionado
   if (CBoxTema->ItemIndex < 0)
      {//atualiza o gráfico: desabilita mapa temático
      grafico->MapaTematicoEnabled = false;
      }
   //determina Patamar selecionado
   if (CBoxPatamar->ItemIndex <= 0)
      {//define opção "todos patamares"
      ind_pat = 0;
      }
   else
      {//determina Patamar
      patamar = (VTPatamar*)(CBoxPatamar->Items->Objects[CBoxPatamar->ItemIndex]);
      ind_pat = patamar->Index;
      }
   //determina Mapa
   tema = (VTTema*)(CBoxTema->Items->Objects[CBoxTema->ItemIndex]);
   //inicia aguarde
   progresso->Start(progDEFAULT);
   //atualiza cores das celulas
   tema->Reinicia(ind_pat);
   //atualiza o gráfico: exibe mapa temático
   grafico->MapaTematicoEnabled = true;
   //para aguarde
   progresso->Stop();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMapaTematico::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //detrói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMapaTematico::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form sem alinhamento
   Top  = 0;
   Left = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMapaTematico::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   //CBoxResolucaoChange(NULL);
   CBoxTemaChange(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMapaTematico::InsereTema(VTTema *tema, bool show)
   {
   //insere o Tema em CBoxTema
   CBoxTema->ItemIndex = CBoxTema->Items->AddObject(tema->Codigo, tema);
   //exibe o novo Tema
   if (show) ExibeTemaSelecionado();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMapaTematico::SelecionaModoGrafico(int modo)
	{
	//variáveis locais
	VTGrafico  *grafico  = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	VTRedegraf *redegraf = (VTRedegraf*)apl->GetObject(__classid(VTRedegraf));

	//verifica se está sendo alterado o modo gráfico
	if (redegraf->ModoGrafico != modo)
		{//seleciono modo gráfico indicado
		redegraf->ModoGrafico = modo;
		grafico->ModoGraficoAlterado();
		}
	}

//---------------------------------------------------------------------------
//void __fastcall TFormMapaTematico::SelecionaResolucao(void)
//   {
//   //variáveis locais
//   VTCelula  *celula;
//   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
//
//   //redefine número de celulas
//   switch (CBoxResolucao->ItemIndex)
//      {
//      case 0: //50
//         celulas->NumCelula = 50;
//         break;
//      case 1: //60
//         celulas->NumCelula = 60;
//         break;
//      case 2: //70
//         celulas->NumCelula = 70;
//         break;
//      case 3: //80
//         celulas->NumCelula = 80;
//         break;
//      case 4: //90
//         celulas->NumCelula = 90;
//         break;
//      case 5: //100x100
//         celulas->NumCelula = 100;
//         break;
//      default:
//         celulas->NumCelula = 50;
//      }
//   }
//---------------------------------------------------------------------------
void __fastcall TFormMapaTematico::SelecionaResolucao(void)
   {
   //variáveis locais
   VTCelula  *celula;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));

   //redefine número de celulas
   switch (CBoxResolucao->ItemIndex)
      {
      case 0: //50
         celulas->NumCelula = 50;
         break;
      case 1: //60
         celulas->NumCelula = 60;
         break;
      case 2: //70
         celulas->NumCelula = 70;
         break;
      case 3: //80
         celulas->NumCelula = 80;
         break;
      case 4: //90
         celulas->NumCelula = 90;
         break;
      case 5: //100x100
         celulas->NumCelula = 100;
         break;
      default:
         celulas->NumCelula = 50;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMapaTematico::TimerTimer(TObject *Sender)
   {
   //seleciona próximo Patamar
   if (CBoxPatamar->ItemIndex < (CBoxPatamar->Items->Count - 1))
      {
      CBoxPatamar->ItemIndex += 1;
      }
   else
      {//não utiliza primeiro Item ("todos patamares")
      CBoxPatamar->ItemIndex = 1;
      }
   //atualiza mapa
   ExibeTemaSelecionado();
   }

//---------------------------------------------------------------------------
//eof






