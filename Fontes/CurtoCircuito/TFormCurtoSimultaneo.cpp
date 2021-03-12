//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#include "TFormCurtoSimultaneo.h"
#include "TFormCfgDefeito.h"
//#include "TFormCurtoArvore.h"
//#include "TFormCurtoTabela.h"
//#include "TFormPosDefTrecho.h"
#include "TFormRelChave.h"
#include "TFormRelTexto.h"
#include "TFormZdef.h"
#include "TDefeitos.h"
#include "VTCurtocircuito.h"
#include "..\Apl\VTApl.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Cronometro\VTCronometro.h"
#include "..\Curto\VTCurto.h"
#include "..\Diretorio\VTPath.h"
#include "..\Distancia\VTCaminho.h"
#include "..\Editor\TEdtCurto.h"
#include "..\Flow\VTFlow.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\MultiMonitor\VTMultiPanel.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTEqpto.h"
#include "..\Rede\VTFases.h"
#include "..\Rede\VTTrecho.h"
#include "..\Rede\VTMRede.h"
#include "..\Rede\VTNet.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTReducao.h"
#include "..\RedeCC\VTRedeCC.h"
#include "..\Sinap\VTSinapChild.h"
#include "..\Sinap\VTSinapMdi.h"
#include "..\..\DLL_Inc\Distancia.h"
#include "..\..\DLL_Inc\Editor.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Help.h"
#include "..\..\DLL_Inc\Protecao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFORMEDT"
#pragma link "TFormEdt"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCurtoSimultaneo::TFormCurtoSimultaneo(TComponent *Owner, VTApl *apl_owner, TWinControl *parent)
                      : TFormEdt(Owner, parent)
   {
   try{//seta indica��o de curto n�o calcualdo
      curto_calculado = false;
      //cria objetos
      lisBAR        = new TList();
      lisLIG        = new TList();
      defeitos      = NewObjDefeitos();
      curtocircuito = NewObjCurtocircuito(apl_owner);
      //obt�m objeto AplCurto
      apl    = curtocircuito->Apl;
      //obt�m objeto RedeCC
      redeCC = (VTRedeCC*)apl->GetObject(__classid(VTRedeCC));
      //obt�m Zdef definida em VTGeral
      IniciaZdef();
      //exibe imped�ncia de defeito
      ExibeZdef();
      //inicia CBoxPatamar
      CBoxPatamarInicia();
      //cria TFormRelTexto para apresentar relat�rio texto gerado pelo curto
      //FormCurtoArvore = new TFormCurtoArvore(this, PanelCurto, apl);
      //FormCurtoTabela = new TFormCurtoTabela(this, PanelCurto, apl);
      FormRelTexto = new TFormRelTexto(this, apl, Parent);
      }catch (Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TFormCurtoSimultaneo::~TFormCurtoSimultaneo(void)
   {
   //vari�veis locais
   VTGeral   *geral   = (VTGeral*)apl->GetObject(__classid(VTGeral));
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //apaga moldura
   GraficoMoldura();
   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //N�O destr�i FormCurtoArvore e FormCurtoTabela
   //if (FormCurtoArvore) {delete FormCurtoArvore; FormCurtoArvore = NULL;}
   //if (FormCurtoTabela) {delete FormCurtoTabela; FormCurtoTabela = NULL;}
   //destr�i lista sem destruir seus objetos
   //if (lisEQP_SEL) {delete lisEQP_SEL; lisEQP_SEL = NULL;}
   //destr�i lista e seus objetos
   //if (lisGRUPO) {LimpaTList(lisGRUPO); delete lisGRUPO; lisGRUPO = NULL;}
   //apaga moldura e eqptos em destaque
   grafico->Moldura();
   grafico->DestacaEqpto(NULL, clBlack, 1);
   //fecha eventual TFormProtecao
   DLL_CloseFormProtecao(this);
   //fecha TFormRelTexto
   if (FormRelTexto) {delete FormRelTexto; FormRelTexto = NULL;}
   //destr�i objetos
   if (edt_curto)     {delete edt_curto;     edt_curto     = NULL;}
   if (curtocircuito) {delete curtocircuito; curtocircuito = NULL;}
   if (defeitos)      {delete defeitos;      defeitos      = NULL;}
   //destr�i lista sem destrui seus objetos
   if (lisBAR) {delete lisBAR; lisBAR = NULL;}
   if (lisLIG) {delete lisLIG; lisLIG = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::ActionCalculaCurtoExecute(TObject *Sender)
   {
   ExecutaCalculo();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::ActionClearBarDefeitoExecute(TObject *Sender)
   {
   //cancela indica��o de curto calculado
   curto_calculado = false;
   //esconde FormRelTexto
   FormRelTexto->Hide();
   //elimina todos os defeitos
   defeitos->Clear();
   //atualiza LView
   LViewBarDefeitoInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::ActionEditaZdefExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormZdef *FormZdef;
   VTGeral   *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //exibe janela de edi��o da imped�ncia de defeito
   FormZdef = new TFormZdef(this);
   FormZdef->DefineZdef(Zdef);
   if (FormZdef->ShowModal() == mrOk)
      {//determina novo valor da imped�ncia de defeito
      FormZdef->LeZdef(Zdef);
      //salva arquivo
      SalvaArqCfgCurto();
      //exibe imped�ncia de dedefeito
      ExibeZdef();
      //recalcula o curto
      ExecutaCalculo();
      //atualiza FormCurtoArvore e FormCurtoTabela
      //FormCurtoArvore->EqptoDefeito(defeito.eqpto);
      //FormCurtoTabela->EqptoDefeito(defeito.eqpto);
      }
   //destr�i FormZdef
   delete FormZdef;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::ActionEdtExecute(TObject *Sender)
   {
   try{//se necess�rio, cria um objeto TEdtCurto
      if (edt_curto != NULL) delete edt_curto;
      //cria EdtSpy p/ controle do mouse
      edt_curto = DLL_NewObjEdtCurto(apl, ActionMouseOff, ActionMouseOn, ActionSelEqpto);
      ((TEdtCurto*)edt_curto)->DefineActionCurto(ActionSelEqptoDefeito);
      //atualiza indica��o de mouse ativo e for�a bot�o Down
      AtualizaIndicacaoEditorOn(butEdt);
      } catch (Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::ActionHelpExecute(TObject *Sender)
   {
   //variaveis locais
   TForm *formHelp;

   formHelp = DLL_NewFormHelp(this, apl, "Curto_Circuito");
   if(formHelp) formHelp->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::ActionRelatorioChaveExecute(TObject *Sender)
   {
   //vari�veis locais
   TFormRelChave *Form;

   try{//verifica se lista com as Ligacoes do caminho el�trico est� preenchida
      if (lisLIG->Count == 0)
         {//determina cainho el�trico
         MontaCaminhoCurto();
         if (lisLIG->Count == 0)
            {
            Aviso("N�o foi poss�vel identifica o caminho el�trico do defeito");
            return;
            }
         }
      //cria TFormRelChave como janela modal
      if ((Form = new TFormRelChave(this, apl)) != NULL)
         {
         Form->DefineCaminhoEletrico(lisLIG);
         Form->ShowModal();
         delete Form;
         }
      }catch(Exception &e)
         {
         //nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::ActionRelatorioCurtoExecute(TObject *Sender)
   {
   /*
   //altera Form de apresenta��o de resultados
   if (FormCurtoArvore->Visible)
      {
      FormCurtoArvore->Hide();
      FormCurtoTabela->Show();
      //redimensiona este form
      ClientWidth = FormCurtoTabela->WidthMinimo();
      //altera �cone
      ActionRelatorioCurto->ImageIndex = 10;
      }
   else
      {
      FormCurtoTabela->Hide();
      FormCurtoArvore->Show();
      //redimensiona este form
      ClientWidth = FormCurtoArvore->WidthMinimo();
      //altera �cone
      ActionRelatorioCurto->ImageIndex = 11;
      }
   */
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::ActionProtecaoExecute(TObject *Sender)
   {
   //vari�veis locais
   bool         pre_falta = true;
   VTMultiPanel *MultiPanel = (VTMultiPanel*)apl->GetObject(__classid(VTMultiPanel));
   VTRedes      *redes      = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //verifica se h� defeitos selecionados
   if (defeitos->NumeroDeDefeitos() == 0)
      {
      Aviso("N�o h� defeitos definidos");
      return;
      }
   //executa c�lculo de fluxo com pr�_falta
   ExecutaCalculoFluxo(pre_falta);
   //reinicia RedeCC
   redeCC->Inicia(redes);
   //torna a executar o c�lculo de curto porque os objetos Barra e Ligacao precisam
   //apontar para BarCC e LigCC
   ExecutaCalculo();
   //exibe TFormProtecao
   DLL_NewFormProtecao(this, apl, MultiPanel->PanelAtivo);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::ActionSelEqptoExecute(TObject *Sender)
   {
   //vari�veis locais
   VTEqpto *eqpto = (VTEqpto*)Sender;

   //prote��o: verifica se j� foi selecionado o eqpto
   if (eqpto == NULL) return;
   //prote��o: verifica se j� o c�lculo de curto j� foi executado
   if (! curto_calculado) return;
   //indica Eqpto selecionado p/ FormCurtoArvore e FormCurtoTabela
   //FormCurtoArvore->EqptoSelecionado(eqpto);
   //FormCurtoTabela->EqptoSelecionado(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::ActionSelEqptoDefeitoExecute(TObject *Sender)
   {
   //vari�veis locais
   TDefeito *defeito;
   VTBarra  *barra;
   VTEqpto  *eqpto = (VTEqpto*)Sender;

   //salva eqpto de defeito
   if (eqpto == NULL) return;
   //verifica se
   if (eqpto->Tipo() != eqptoBARRA)
      {
      Aviso("Selecione barra de defeito.");
      return;
      }
   //verifica se existe um defeito definido para a Barra
   barra = (VTBarra*)eqpto;
   if ((defeito = defeitos->ExisteDefeito(barra)) == NULL)
      {//cria um novo Defeito
      defeito = defeitos->InsereDefeito(barra, defeito3F, faseABC);
      }
   //exibe FormCfgDefeito para usu�rio selecionar defeito nessa Barra
   if (ConfiguraDefeito(defeito))
      {//reinicia LViewBarDefeito
      LViewBarDefeitoInicia();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::CBoxDefChange(TObject *Sender)
   {
   //vari�veis locais
   int       ind_def;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //determina tipo de defeito p/ mostrar anima��o no gr�fico
   switch(CBoxDef->ItemIndex)
      {
      case 0:  ind_def = defeito3F;  break;
      case 1:  ind_def = defeito2F;  break;
      case 2:  ind_def = defeitoFT;  break;
      case 3:  ind_def = defeitoFTZ; break;
      case 4:  ind_def = defeito2FT; break;
      default: return;
      }
   //seleciona defeito a ser exibido em RedeCC
   redeCC->ShowDefeito = ind_def;
   //atualiza gr�fico
   grafico->Redraw();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::CBoxPatamarChange(TObject *Sender)
   {
   //recalcula curto-circuito p/ a condi��o selecionada
   //ExecutaCalculo();
   //atualiza FormCurtoArvore e FormCurtoTabela
   //FormCurtoArvore->EqptoDefeito(defeito.eqpto);
   //FormCurtoTabela->EqptoDefeito(defeito.eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::CBoxPatamarInicia(void)
   {
   //vari�veis locais
   VTPatamar   *patamar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   //reinicia CBoxPatamar
   CBoxPatamar->Clear();
   //insere op��o "N�o utilizado"
   CBoxPatamar->Items->AddObject("N�o Utilizado", NULL);
   //insere Patamares
   for (int ind_pat = 0; ind_pat < patamares->NumPatamar(); ind_pat++)
      {
      patamar = patamares->Patamar[ind_pat];
      CBoxPatamar->Items->AddObject(patamar->Nome, patamar);
      }
   //pr�-seleciona primeiro patamar
   CBoxPatamar->ItemIndex = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::CheckBoxCaminhoClick(TObject *Sender)
   {
   //exibe o caminho do curto
   ExibeCaminhoCurto();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCurtoSimultaneo::ConfiguraDefeito(TDefeito *defeito)
   {
   //vari�veis locais
   TFormCfgDefeito *FormCfgDefeito;

   //escinde este Form
   Visible = false;
   //cria TFormCfgDefeito como janela modal
   FormCfgDefeito = new TFormCfgDefeito(this, defeitos, defeito);
   FormCfgDefeito->ShowModal();
   //destr�i TFormCfgDefeito
   delete FormCfgDefeito;
   //volta a exibir este Form
   Visible = true;

   return(true);
   }

//---------------------------------------------------------------------------
int __fastcall TFormCurtoSimultaneo::DeterminaPatamarSelecionado(void)
   {
   //vari�veis locais
   VTPatamar   *patamar;

   //prote��o:
   if (CBoxPatamar->ItemIndex <= 0) return(-1);
   //determina Patamar selecionado
   patamar  = (VTPatamar*)CBoxPatamar->Items->Objects[CBoxPatamar->ItemIndex];
   return(patamar->Index);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCurtoSimultaneo::ExecutaCalculo(void)
   {
   //vari�veis locais
   bool         pre_falta;
   int          ind_pat;
   VTPatamar    *patamar;
   VTTrecho     *trecho;
   VTCronometro *cronometro = (VTCronometro*)apl->GetObject(__classid(VTCronometro));
   VTRedes      *redes      = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTGrafico    *grafico    = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //verifica se foram selecionados os defitos
   if (defeitos->NumeroDeDefeitos() == 0)
      {
      Aviso("N�o h� defeitos definidos");
      return(false);
      }
   //seta indica�ao de curto n�o calculado
   curto_calculado = false;
   //esconde FormRelTexto
   FormRelTexto->Hide();
   //verifica se h� defeitos definidos
   if (defeitos->NumeroDeDefeitos() == 0)
      {
      Aviso("Defina os defeitos");
      return(false);
      }
   //desabilita anima��o do gr�fico
   grafico->AnimacaoEnabled = false;
   //desabilita medi��o de tempo
   cronometro = NULL;
   //determina algoritmo de c�lculo de curto
   curtocircuito->Algoritmo = curtoMA;
   //determina Patamar selecionado para a condi��o de pr�-falta
   if ((ind_pat = DeterminaPatamarSelecionado()) < 0)
      {//importante: define um valor v�lido para o patamar
      pre_falta = false;
      ind_pat   = 0;
      //desabilita visualiza��o do FormProtecao
      //ActionProtecao->Enabled = false;
      }
   else
      {//se necess�rio, executa c�lculo de fluxo
      pre_falta = true;
      if (! ExecutaCalculoFluxo(pre_falta)) return(false);
      //habilita visualiza��o do FormProtecao
      //ActionProtecao->Enabled = true;
      }
   //inicia cron�metro
   if (cronometro) cronometro->Reinicia();
   if (cronometro) cronometro->Start("Curto", "Curto circuito");
   //configura op��es do Curtocircuito
   curtocircuito->AvisoEnabled  = true;
   curtocircuito->TipoRelatorio = relCOMPLETO;
   //executa c�lculo para os defeitos configurados
//TESTES
//TDefeito *defeito = defeitos->FirstDefeito();
//curto_calculado = curtocircuito->Executa(redes, Zdef, true, defeito->barra, ind_pat, pre_falta);
   curto_calculado = curtocircuito->ExecutaCurtoSimultaneo(redes, Zdef, true, defeitos->LisDefeito(), ind_pat, pre_falta);

   if (cronometro) cronometro->Stop("Curto");
   if (! curto_calculado)
      {
      Aviso("O c�lculo de curto-circuito n�o pode ser executado.");
      return(false);
      }
   //atualiza gr�fico
   grafico->Redraw();
   grafico->AnimacaoEnabled = true;
   //FormCurtoArvore->EqptoDefeito(defeito.eqpto);
   //FormCurtoTabela->EqptoDefeito(defeito.eqpto);
   //atualiza FormRelTexto
   ExibeRelTexto();
   //atualiza FormProtecao
   DLL_AtualizaFormProtecao(this);
   //exibe tempo gasto no c�lculo
   if (cronometro) cronometro->Show();

   return(curto_calculado);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCurtoSimultaneo::ExecutaCalculoFluxo(bool pre_falta)
   {
   //vari�veis locais
   VTFlow    *flow     = (VTFlow*)apl->GetObject(__classid(VTFlow));
   VTRedes   *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));

   //2014.02.13 - CurtoCS sem pr�-falta n�o precisa do c�lculo de fluxo sem carga

   //verifica necessidade de calcular fluxo de pot�ncia
   if ((pre_falta)&&(! flow->UpToDate))
      {//executa c�lculo de fluxo desequilibrado
      flow->AvisoEnabled      = false;
      flow->CronometroEnabled = false;
      flow->TramoEnabled      = false;
      flow->TipoRelatorio     = relNENHUM;
      flow->TipoFluxo         = fluxoDESEQUILIBRADO;
      flow->UpToDate          = false;
      //calcula fluxo
      if (! flow->CalculaPorRede(redes))
      // if (! flow->CalculaRedeUnica(redes))
         {//gera aviso p/ usu�rio
         Aviso("Erro no c�lculo de fluxo de pot�ncia.\n"
               "N�o � poss�vel calcular a condi��o de pr�-carga");
         return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::ExibeCaminhoCurto(void)
   {
   //vari�veis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //destaca Ligacoes no gr�fico
   grafico->DestacaEqpto(lisLIG, PanelColor->Color, 3);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::ExibeRelTexto(void)
   {
   //vari�veis locais
   AnsiString pathname;
   AnsiString arq_texto;
   VTPath    *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //define nome do arquivo
   arq_texto = ChangeFileExt(ExtractFileName(path->ArqRede), "");
   pathname  = path->DirTmp() + "\\" + arq_texto + "_CMA_Resultados.txt";
   //carrega arquivo texto com relat�rio gerado pelo c�lculo
   FormRelTexto->Atualiza(pathname);
   //exibe FormRelTexto
   FormRelTexto->Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::ExibeZdef(void)
   {
   //vari�veis locais
   TListItem *Item;
   //exibe valores
   //EditRdef->Text = DoubleToStr("%2.1f", Zdef.Zdef.r);
   //EditXdef->Text = DoubleToStr("%2.1f", Zdef.Zdef.x);
   //reinicia ListViewZdef
   ListViewZdef->Clear();
   //insere dados de Zdef3F_ff
   Item          = ListViewZdef->Items->Add();
   Item->Caption = "3F";
   Item->SubItems->Add("fase/fase");
   Item->SubItems->Add(DoubleToStr("%2.1f", Zdef.Zdef3F_ff.r));
   Item->SubItems->Add(DoubleToStr("%2.1f", Zdef.Zdef3F_ff.x));
   //insere dados de Zdef2F_ff
   Item = ListViewZdef->Items->Add();
   Item->Caption = "2F";
   Item->SubItems->Add("fase/fase");
   Item->SubItems->Add(DoubleToStr("%2.1f", Zdef.Zdef2F_ff.r));
   Item->SubItems->Add(DoubleToStr("%2.1f", Zdef.Zdef2F_ff.x));
   //insere dados de Zdef2FT_ff
   Item = ListViewZdef->Items->Add();
   Item->Caption = "2FT";
   Item->SubItems->Add("fase/fase");
   Item->SubItems->Add(DoubleToStr("%2.1f", Zdef.Zdef2FT_ff.r));
   Item->SubItems->Add(DoubleToStr("%2.1f", Zdef.Zdef2FT_ff.x));
   //insere dados de Zdef2FT_ft
   Item = ListViewZdef->Items->Add();
   Item->Caption = "2FT";
   Item->SubItems->Add("fase/terra");
   Item->SubItems->Add(DoubleToStr("%2.1f", Zdef.Zdef2FT_ft.r));
   Item->SubItems->Add(DoubleToStr("%2.1f", Zdef.Zdef2FT_ft.x));
   //insere dados de Zdef1FT_ft
   Item = ListViewZdef->Items->Add();
   Item->Caption = "1FT";
   Item->SubItems->Add("fase/terra");
   Item->SubItems->Add(DoubleToStr("%2.1f", Zdef.Zdef1FT_ft.r));
   Item->SubItems->Add(DoubleToStr("%2.1f", Zdef.Zdef1FT_ft.x));
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //vari�veis locais
   VTSinapChild *sinap_child = (VTSinapChild*)apl->GetObject(__classid(VTSinapChild));

   //esconde e desabilita o Form
   Hide();
   Enabled = false;
   //seleciona gr�fico de topologia
   sinap_child->ActionTopo->Execute();
   //destr�i o form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::FormIniciaPosicao(void)
   {
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      if (! geral->Infoset->LoadFromFile(this))
         {//posiciona o Form sem alinhamento
         ActionAlignNoneExecute(NULL);
         }
      else
         {
         switch(Align)
            {
            case alLeft:   ActionAlignLeft->Execute();   break;
            case alRight:  ActionAlignRight->Execute();  break;
            case alTop:    ActionAlignUp->Execute();     break;
            case alBottom: ActionAlignDown->Execute();   break;
            case alNone:   //salva �ltima
               janela.top    = Top;
               janela.left   = Left;
               janela.width  = Width;
               janela.height = Height;
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   //inicia objeto EdtExplorer
   ActionEdtExecute(NULL);
   //exibe um dos Forms de apresenta��o de resultados
   ActionRelatorioCurto->Execute();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::GraficoMoldura(VTEqpto *eqpto)
   {
   //vari�veis locais
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   grafico->Moldura(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::GraficoZoom(VTEqpto *eqpto)
   {
   //vari�veis locais
   bool       ampliar = true;
   VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   //prote��o
   if (eqpto == NULL) return;
   //seleciona �rea de zoom
   grafico->SelecionaZoom(eqpto, ampliar);
   }
//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::IniciaZdef(void)
   {
   //zera imped�ncia de defeito
   Zdef.Zdef3F_ff.r  = Zdef.Zdef3F_ff.x  = 0;
   Zdef.Zdef2F_ff.r  = Zdef.Zdef2F_ff.x  = 0;
   Zdef.Zdef2FT_ff.r = Zdef.Zdef2FT_ff.x = 0;
   Zdef.Zdef2FT_ft.r = Zdef.Zdef2FT_ft.x = 0;
   Zdef.Zdef1FT_ft.r = Zdef.Zdef1FT_ft.x = 0;
   //l� Zdef do arquivo de arquivo de configura��o de curto
   LeArqCfgCurto();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::LeArqCfgCurto(void)
   {
   //vari�veis locais
   VTGeral    *geral        = (VTGeral*)apl->GetObject(__classid(VTGeral));
   AnsiString arq_cfg_curto = "CfgCurto";

   //l� valores de Zdef do arquivo
   if (geral->Infoset->LoadFromFile(arq_cfg_curto))
      {
      geral->Infoset->GetInfo("Zdef3F_ff.r",  Zdef.Zdef3F_ff.r );
      geral->Infoset->GetInfo("Zdef3F_ff.x",  Zdef.Zdef3F_ff.x );
      geral->Infoset->GetInfo("Zdef2F_ff.r",  Zdef.Zdef2F_ff.r );
      geral->Infoset->GetInfo("Zdef2F_ff.x",  Zdef.Zdef2F_ff.x );
      geral->Infoset->GetInfo("Zdef2FT_ff.r", Zdef.Zdef2FT_ff.r);
      geral->Infoset->GetInfo("Zdef2FT_ff.x", Zdef.Zdef2FT_ff.x);
      geral->Infoset->GetInfo("Zdef2FT_ft.r", Zdef.Zdef2FT_ft.r);
      geral->Infoset->GetInfo("Zdef2FT_ft.x", Zdef.Zdef2FT_ft.x);
      geral->Infoset->GetInfo("Zdef1FT_ft.r", Zdef.Zdef1FT_ft.r);
      geral->Infoset->GetInfo("Zdef1FT_ft.x", Zdef.Zdef1FT_ft.x);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::LViewBarDefeitoClick(TObject *Sender)
   {
   //vari�veis locais
   TDefeito *defeito;
   VTBarra  *barra = NULL;

   //verifica se h� um ItemList selecionado
   if (LViewBarDefeito->Selected != NULL)
      {//determina Defeito selecionado
      if ((defeito = (TDefeito*)(LViewBarDefeito->Selected->Data)) != NULL)
         {
         barra = defeito->barra;
         }
      }
   //exibe moldura na Barra do Defeito
   GraficoMoldura(barra);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::LViewBarDefeitoInicia(void)
   {
   //vari�veis locais
   TListItem *Item;
   TDefeito  *defeito;
   TList     *lisDEF = defeitos->LisDefeito();
   VTFases  *fases = (VTFases*)apl->GetObject(__classid(VTFases));

   //reinicia LView
   LViewBarDefeito->Clear();
   //insere todos Defeito em LView
   for (int n = 0; n < lisDEF->Count; n++)
      {
      defeito    = (TDefeito*)lisDEF->Items[n];
      Item       = LViewBarDefeito->Items->Add();
      Item->Data = defeito;
      //c�digo da barra
      if (defeito->barra->Codigo.IsEmpty()) Item->Caption = "ID= " + IntToStr(defeito->barra->Id);
      else                                  Item->Caption = defeito->barra->Codigo;
      //tipo de defeito
      switch(defeito->tipodefeito)
         {
         case defeito3F:  Item->SubItems->Add("3F"     ); break;
         case defeito2F:  Item->SubItems->Add("2F"     ); break;
         case defeitoFT:  Item->SubItems->Add("FT"     ); break;
         case defeitoFTZ: Item->SubItems->Add("FT/Zdef"); break;
         case defeito2FT: Item->SubItems->Add("2FT"    ); break;
         }
      //fase de defeito
      Item->SubItems->Add(fases->AsStrABCN[defeito->fases]);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::MontaCaminhoCurto(void)
   {
   //vari�veis locais
   VTCaminho *caminho;

   try{//reinicia listas de Barras e Ligacoes do cominho el�trico
      lisBAR->Clear();
      lisLIG->Clear();
      //verifica se a exibi��o do caminho est� habilitada
      if (! CheckBoxCaminho->Checked) return;
      //cria objeto VTCaminho
      caminho = DLL_NewObjCaminho(apl);
      //identifica Ligacoes entre a Barra de defeito e os Suprimentos/Geradores
      lisBAR->Add(redeCC->BarraDefeito());
      caminho->Executa(lisBAR, redeCC->LisGeradorSuprimento(), redeCC->LisLigacao(), lisLIG);
      //destr�i objeto caminho
      delete caminho;
      }catch(Exception &e)
         {//nada a fazer
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::PanelColorDblClick(TObject *Sender)
   {
   //exibe ColorDialog
   ColorDialog->Color = PanelColor->Color;
   if (ColorDialog->Execute())
      {//redefine ColorPanel com a cor selecionada
      PanelColor->Color = ColorDialog->Color;
      //atualiza o caminho do curto
      ExibeCaminhoCurto();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::RadioGroupAlgoritmoClick(TObject *Sender)
   {
   //verifica se h� um defeito selecionado
   if (defeitos->NumeroDeDefeitos() > 0)
      {//recalculo o curto
      ExecutaCalculo();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCurtoSimultaneo::SalvaArqCfgCurto(void)
   {
   //vari�veis locais
   VTGeral    *geral        = (VTGeral*)apl->GetObject(__classid(VTGeral));
   AnsiString arq_cfg_curto = "CfgCurto";

   //reinicia InfoSet
   geral->Infoset->Clear();
   //salva valores de Zdef
   geral->Infoset->AddInfo("Zdef3F_ff.r",  Zdef.Zdef3F_ff.r );
   geral->Infoset->AddInfo("Zdef3F_ff.x",  Zdef.Zdef3F_ff.x );
   geral->Infoset->AddInfo("Zdef2F_ff.r",  Zdef.Zdef2F_ff.r );
   geral->Infoset->AddInfo("Zdef2F_ff.x",  Zdef.Zdef2F_ff.x );
   geral->Infoset->AddInfo("Zdef2FT_ff.r", Zdef.Zdef2FT_ff.r);
   geral->Infoset->AddInfo("Zdef2FT_ff.x", Zdef.Zdef2FT_ff.x);
   geral->Infoset->AddInfo("Zdef2FT_ft.r", Zdef.Zdef2FT_ft.r);
   geral->Infoset->AddInfo("Zdef2FT_ft.x", Zdef.Zdef2FT_ft.x);
   geral->Infoset->AddInfo("Zdef1FT_ft.r", Zdef.Zdef1FT_ft.r);
   geral->Infoset->AddInfo("Zdef1FT_ft.x", Zdef.Zdef1FT_ft.x);
   //grava o arquivo
   geral->Infoset->SaveToFile(arq_cfg_curto);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCurtoSimultaneo::ValidaRedeParaCurto(void)
   {
   //vari�veis locais
   bool       rede_valida = true;
   AnsiString txt;
   VTReducao  *reducao;
   VTRedes    *redes    = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTConsulta *consulta = (VTConsulta*)apl->GetObject(__classid(VTConsulta));
   TList      *lisREDU   = consulta->LisReducaoCarregada(redes);
   TList      *lisEQP;

   try{//cria lista p/ uso local
      lisEQP = new TList();
      }catch(Exception &e)
         {
         return(false);
         }
   //loop p/ todas Reducoes
   for (int nr = 0; nr < lisREDU->Count; nr++)
      {
      reducao = (VTReducao*)lisREDU->Items[nr];
      //verifica se a Reducao possui Surpimento ou Trafo
      lisEQP->Clear();
      reducao->LisEqpto(lisEQP, eqptoSUPRIMENTO);
      reducao->LisEqpto(lisEQP, eqptoTRAFO);
      if (lisEQP->Count > 0)
         {
         rede_valida = false;
         txt += "\n" + reducao->MRede->Codigo ;
         }
      }
   //gera aviso p/ usu�rio
   if (! rede_valida) Aviso("As seguintes redes n�o podem ser utilizadas na sua forma equivalente:" + txt);
   //destr�i lista lisEQP
   delete lisEQP;

   return(rede_valida);
   }

//---------------------------------------------------------------------------
//eof


