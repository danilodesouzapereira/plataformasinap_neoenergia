//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#include <algorithm>  //min(), max
#pragma hdrstop
#include "TFormEdtTrf.h"
#include "TFormEdtFoto.h"
//#include "TFormEdtPlan.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTTrafo.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Geral\VTGeral.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;

//---------------------------------------------------------------------------
__fastcall TFormEdtTrf::TFormEdtTrf(TComponent* Owner, VTApl *apl, VTTrafo *trafo, bool criar_copia)
                       :TForm(Owner)
   {
   //salva opção de criar cópia
   this->criar_copia = criar_copia;
   //salva ponteiro p/ objeto
   this->apl = apl;
   geral     = (VTGeral*)apl->GetObject(__classid(VTGeral));
   //cria lista
   lisTRF = new TList();
   lisTMP = new TList();
   //salva Trafo em lisTRF
   lisTRF->Add(trafo);
   //cria um clone do Trafo que será usado durante edição
   clone = trafo->Clone();
   //inicia dados
   aviso_enabled = false;
   sbase_100mva  = 100.0;
   //inicia CLBoxAtributo
   CLBoxAtributoInicia();
   //desabilita TabSheetAtributo
   TabSheetAtributo->TabVisible = false;
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtTrf::TFormEdtTrf(TComponent* Owner, VTApl *apl, TList *lisEXT, bool criar_copia)
                       : TForm(Owner)
   {
   //variáveis locais
   VTTrafo *trafo;

   //salva opção de criar cópia
   this->criar_copia = criar_copia;
   //salva ponteiro p/ objeto
   this->apl = apl;
   geral     = (VTGeral*)apl->GetObject(__classid(VTGeral));
   //proteção
   if (lisEXT->Count == 0)
      {
      Close();
      return;
      }
   //cria lista
   lisTRF = new TList();
   lisTMP = new TList();
   //salva Trafos de lisEXT em lisTRF
   CopiaTList(lisEXT, lisTRF);
   //cria um clone de um Trafo que será uado durante edição
   trafo = (VTTrafo*)lisEXT->First();
   clone = trafo->Clone();
   //inicia dados
   aviso_enabled = false;
   sbase_100mva  = 100.0;
   //inicia CLBoxAtributo
   CLBoxAtributoInicia();
   //habilita TabSheetAtributo
   TabSheetAtributo->TabVisible = (lisTRF->Count > 1);
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtTrf::~TFormEdtTrf(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //apaga eventual moldura
   Moldura();
   //destrói clone do Trafo usado na edição
   if (clone)  {delete clone; clone = NULL;}
   //destrói lista sem destruir seus objetos
   if (lisTRF)  {delete lisTRF; lisTRF = NULL;}
   if (lisTMP)  {delete lisTMP; lisTMP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ActionConfirmaExecute(TObject *Sender)
   {
   //habilita mensagens de aviso/erro
   aviso_enabled = true;
   //atualiza o clone: lê e valida dados editados
   if (! LeDadosTrafo())
      {//desabilita mensagens de aviso/erro
      aviso_enabled = false;
      return;
      }
   //atualiza dados do Trafo original a partir do clone
   AtualizaDados();
   //fecha o form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ActionRetiraExecute(TObject *Sender)
   {
   //fecha o form indicando exclusão do Eqpto
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ActionSelFotoExecute(TObject *Sender)
   {
   //variáveis locais
   TFormEdtFoto *form;

   try{//cria Form e exibe como janela modal
      form = new TFormEdtFoto(this, apl, clone);
      form->ShowModal();
      delete form;
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ActionSelPadraoExecute(TObject *Sender)
   {
   //
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaAutoTrafo(VTTrafo *trafo)
   {
   if (! PanelAutoTrafo->Enabled) return;
   //atualiza atributo
   trafo->auto_trafo = clone->auto_trafo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaCodigo(VTTrafo *trafo)
   {
   if (! PanelCodigo->Enabled) return;
   //atualiza atributo
   trafo->Codigo = clone->Codigo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaDados(void)
   {
   //variáveis locais
   VTTrafo *trafo;

   //atualiza todos os Trafos em lisTRF: somente os campos habilitados
   for (int n = 0; n < lisTRF->Count; n++)
      {
      trafo = (VTTrafo*)lisTRF->Items[n];
      //verifica se deve criar uma cópia
      if (criar_copia)
         {//cria um clone do Trafo e associa com trafo original
         trafo->Obj = trafo->Clone();
         //redefine trafo para apontar para a cópia
         trafo = (VTTrafo*)(trafo->Obj);
         }
      //atualiza atributos (do trafo original ou de sua cópia)
      AtualizaRede(trafo);
      AtualizaCodigo(trafo);
      AtualizaPotenciaNominal(trafo);
      AtualizaPerdaFerro(trafo);
      AtualizaLtcTipo(trafo);
      AtualizaLtcBar(trafo);
      AtualizaLtcVpu(trafo);
      AtualizaZ0Z1(trafo);
      AtualizaAutoTrafo(trafo);
      AtualizaPriBarra(trafo);
      AtualizaPriLigacao(trafo);
      AtualizaPriTensao(trafo);
      AtualizaPriZat(trafo);
      AtualizaSecBarra(trafo);
      AtualizaSecLigacao(trafo);
      AtualizaSecTensao(trafo);
      AtualizaSecZat(trafo);
      AtualizaSecDefasagem(trafo);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaLtcBar(VTTrafo *trafo)
   {
   if (! PanelLtcBarra->Enabled) return;
   //atualiza atributo
	trafo->ltc.ajuste_auto.bar_ref = clone->ltc.ajuste_auto.bar_ref;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaLtcVpu(VTTrafo *trafo)
   {
   if (! PanelLtcTensao->Enabled) return;
   //atualiza atributo
	trafo->ltc.ajuste_auto.v_pu = clone->ltc.ajuste_auto.v_pu;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaLtcTipo(VTTrafo *trafo)
   {
   if (! PanelLtc->Enabled) return;
	//atualiza atributo
	trafo->ltc.tipo = clone->ltc.tipo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaPotenciaNominal(VTTrafo *trafo)
   {
   if (! PanelSnom->Enabled) return;
   //atualiza atributo
   trafo->snom = clone->snom;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaPerdaFerro(VTTrafo *trafo)
   {
   if (! PanelPerdaFerro->Enabled) return;
   //atualiza atributo
   trafo->perda_ferro = clone->perda_ferro;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaPriBarra(VTTrafo *trafo)
   {
   if (! PanelPriBarra->Enabled) return;
   //atualiza atributo
   trafo->pbarra1 = clone->pbarra1;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaPriLigacao(VTTrafo *trafo)
   {
   if (! PanelPriLigacao->Enabled) return;
   //atualiza atributo
   trafo->pri.ligacao = clone->pri.ligacao;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaPriTensao(VTTrafo *trafo)
   {
   if (! PanelPriTensao->Enabled) return;
   //atualiza atributo
   trafo->pri.vnom = clone->pri.vnom;
   trafo->pri.tap  = clone->pri.tap;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaPriZat(VTTrafo *trafo)
   {
   if (! gboxPriZterra->Enabled) return;
   //atualiza atributo
   trafo->pri.zterra = clone->pri.zterra;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaRede(VTTrafo *trafo)
   {
   if (! PanelRede->Enabled) return;
   //atualiza atributo
   trafo->rede = clone->rede;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaSecBarra(VTTrafo *trafo)
   {
   if (! PanelSecBarra->Enabled) return;
   //atualiza atributo
   trafo->pbarra2 = clone->pbarra2;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaSecLigacao(VTTrafo *trafo)
   {
   if (! PanelSecLigacao->Enabled) return;
   //atualiza atributo
   trafo->sec.ligacao = clone->sec.ligacao;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaSecTensao(VTTrafo *trafo)
   {
   if (! PanelSecTensao->Enabled) return;
   //atualiza atributo
   trafo->sec.vnom = clone->sec.vnom;
   trafo->sec.tap  = clone->sec.tap;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaSecZat(VTTrafo *trafo)
   {
   if (! gboxSecZterra->Enabled) return;
   //atualiza atributo
   trafo->sec.zterra = clone->sec.zterra;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaSecDefasagem(VTTrafo *trafo)
   {
   if (! gboxSecDefasagem->Enabled) return;
   //atualiza atributo
   trafo->sec.defasagem = clone->sec.defasagem;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::AtualizaZ0Z1(VTTrafo *trafo)
   {
   if (! PanelZ0Z1->Enabled) return;
   //atualiza atributo
   trafo->z0 = clone->z0;
   trafo->z1 = clone->z1;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxSecDefasagemChange(TObject *Sender)
   {
   //atualiza valor de defagems
   LeSecDefasagem();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxLtcBarChange(TObject *Sender)
   {
   //variáveis locais
   VTBarra *barra;

   //determina Barra selecionada
//   if (CBoxLtcBar->ItemIndex <= 0)
//      {barra = NULL;}
//   else
//      {barra = (VTBarra*)(CBoxLtcBar->Items->Objects[CBoxLtcBar->ItemIndex]);}
   //exibe/apaga moldura na Barra selecionada
   //Moldura(barra);
   //FKM - 2015.09.03 - /* o itemindex == 0 também pegava o 1o item da lista de barras
   try{
      barra = (VTBarra*)(CBoxLtcBar->Items->Objects[CBoxLtcBar->ItemIndex]);
      if (!barra) {throw Exception("TFormEdtTrf::CBoxLtBar->ItemIndex aponta pra NULL");}
      else
         {//exibe/apaga moldura na Barra selecionada
         Moldura(barra);
         }
      }
   catch(Exception &e)
      {
      //
      }
   //FKM - 2015.09.03 - ************************************************************* */
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxLtcBarInicia(void)
   {
   //variáveis locais
   //TList   *lisBAR;
   VTBarra     *pbarra;
   VTRede      *rede;
   AnsiString  nomeBarra;

   //reinicia CBoxLtcBar
   CBoxLtcBar->Clear();
   //verifica se está selecionada opção de ajuste automático
   if (CBoxLtcTipo->ItemIndex <= 0) return;
   //verifica se Rede do Trafo está selecionada
   if (CBoxRede->ItemIndex < 0) return;
   rede = (VTRede*)(CBoxRede->Items->Objects[CBoxRede->ItemIndex]);
   if (rede == NULL) return;
//   lisBAR = rede->LisBarra();
//   //loop p/ todas Barras
//   for (int n = 0; n < lisBAR->Count; n++)
//      {
//      pbarra = (VTBarra*)lisBAR->Items[n];
//      CBoxLtcBar->Items->AddObject(pbarra->Codigo, pbarra);
//      }
   //FKM - 2015.09.03 /*Listar somente as duas barras do trafo
   if(clone->pbarra1->Codigo.IsEmpty()){ nomeBarra = "(Id = " + IntToStr(clone->pbarra1->Id) +")";}
   else{nomeBarra = clone->pbarra1->Codigo;}
   CBoxLtcBar->Items->AddObject(nomeBarra, clone->pbarra1);
   if(clone->pbarra2->Codigo.IsEmpty()){ nomeBarra = "(Id = " + IntToStr(clone->pbarra2->Id) +")";}
   else{nomeBarra = clone->pbarra2->Codigo;}
   CBoxLtcBar->Items->AddObject(nomeBarra, clone->pbarra2);
   //FKM - 2015.09.03 ******************************************/
   //habilita ordem alfabética
   CBoxLtcBar->Sorted = true;
   //não seleciona nenhuma Barra
   CBoxLtcBar->ItemIndex = -1;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxLtcTipoChange(TObject *Sender)
   {
   //lê tipo de Ltc
   LeLtcTipo();
   //verifica se deve iniciar CBoxLtcBar
   if (CBoxLtcTipo->ItemIndex <= 0)
      {//reinicia CBoxLtcBar
      CBoxLtcBarInicia();
      }
   else if (CBoxLtcBar->Items->Count == 0)
      {//reinicia CBoxLtcBar
      CBoxLtcBarInicia();
      }
   //lê Barra de referência (pois sem ajuste automático a barra de referência é cancelada)
   LeLtcBar();
   //atualiza campos
   ExibeDadosCondicionais();
   //exibe moldura na Barra de referênica
   MolduraBarraLtc();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxPriBarInicia(void)
   {
   //reinicia CBoxPriBar
   CBoxPriBar->Clear();
   CBoxPriBar->Items->AddObject(clone->pbarra1->Codigo, clone->pbarra1);
   CBoxPriBar->Items->AddObject(clone->pbarra2->Codigo, clone->pbarra2);
   //não seleciona nenhuma Barra
   CBoxPriBar->ItemIndex = -1;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxPriBarChange(TObject *Sender)
   {
   //variáveis locais
   double     v_kv;
   VTBarra    *barra;

   //proteção: verifica se houve alteração da Barra
   if (CBoxPriBar->ItemIndex != CBoxSecBar->ItemIndex) return;
   //troca Barra do secundário
	CBoxSecBar->ItemIndex = 1 - CBoxPriBar->ItemIndex;
   //lê Barras do primário e secundário
   LePriBarra();
   LeSecBarra();
   //lê tensões do primário e do secundário
   LePriTensao();
   LeSecTensao();
	//troca tensões entre primário/secundário
   v_kv = clone->pri.vnom; clone->pri.vnom = clone->sec.vnom; clone->sec.vnom = v_kv;
   v_kv = clone->pri.tap;  clone->pri.tap  = clone->sec.tap;  clone->sec.tap  = v_kv;
   //exibe tensões do primário e do secundário
   ExibePriTensao();
   ExibeSecTensao();
   //exibe Moldura na Barra do primário
   barra = (VTBarra*)CBoxPriBar->Items->Objects[CBoxPriBar->ItemIndex];
   Moldura(barra);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxPriLigaChange(TObject *Sender)
   {
   //lê ligacação do primário
   LePriLigacao();
   //exibe campos que podem ser afetados pela alteração da ligação
   ExibePriZat();
   ExibeSecDefasagem();
   //exibe/esconde campos condiconais
   ExibeDadosCondicionais();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxRedeChange(TObject *Sender)
   {
   //reinicia CBoxLtxBarra
   CBoxLtcBarInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxRedeInicia(void)
   {
   //variáveis locais
   VTRede  *rede;
   VTRedes *redes   = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //reinicia CBox c/ as Redes
   CBoxRede->Clear();
   if (lisREDE->Count == 0) return;
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (! rede->Carregada) continue;
      CBoxRede->Items->AddObject(rede->Codigo, rede);
      }
   //define ordem alfabética
   CBoxRede->Sorted = true;
   //não seleciona nenhuma rede
   CBoxRede->ItemIndex = -1;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxSecBarInicia(void)
   {
   //reinicia CBoxSecBar
   CBoxSecBar->Clear();
   CBoxSecBar->Items->AddObject(clone->pbarra1->Codigo, clone->pbarra1);
   CBoxSecBar->Items->AddObject(clone->pbarra2->Codigo, clone->pbarra2);
   //não seleciona nenhuma Barra
   CBoxSecBar->ItemIndex = -1;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxSecBarChange(TObject *Sender)
   {
   //variáveis locais
   double     v_kv;
   VTBarra    *barra;

   //proteção: verifica se houve alteração da Barra
   if (CBoxSecBar->ItemIndex != CBoxPriBar->ItemIndex) return;
   //troca Barra do primário
	CBoxPriBar->ItemIndex = 1 - CBoxSecBar->ItemIndex;
   //lê Barras do primário e secundário
   LePriBarra();
   LeSecBarra();
   //lê tensões do primário e do secundário
   LePriTensao();
   LeSecTensao();
	//troca tensões entre primário/secundário
   v_kv = clone->pri.vnom; clone->pri.vnom = clone->sec.vnom; clone->sec.vnom = v_kv;
   v_kv = clone->pri.tap;  clone->pri.tap  = clone->sec.tap;  clone->sec.tap  = v_kv;
   //exibe tensões do primário e do secundário
   ExibePriTensao();
   ExibeSecTensao();
   //exibe Moldura na Barra do secundário
   MolduraBarraSec();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxSecLigaChange(TObject *Sender)
   {
   //lê ligacao do secundário
   LeSecLigacao();
   //exibe campos que podem ser afetados pela alteração da ligação
   ExibeSecZat();
   ExibeSecDefasagem();
   //exibe/esconde campos condiconais
   ExibeDadosCondicionais();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxSbaseChange(TObject *Sender)
   {
   //variáveis locais
   double sbase_old, sbase_new;
   strIMP z0, z1;

   try{//pergunta ao usuário se os valores das impedâncias devem ser recalculados na nova base
      if (Confirma("O valor de base das impedâncias foi alterado",
                   "Os valores de Z0 e Z1 devem ser recalculados na nova base selecionada ?") == IDYES)
         {//lê valores editados de Z0/Z1
         if (! LeZ0Z1(z0, z1)) return;
         //verifica se o valor editado estava em pu de Sb100
         if (CBoxSbase->ItemIndex == 0)
            {//lê potência nominal
            if (! LePotenciaNominal()) return;
            //converte valores lidos de Z0/Z1 para pu de Snom
            sbase_old = sbase_100mva;
            sbase_new = clone->snom;
            ConverteZ0Z1(z0, z1, sbase_old, sbase_new);
            }
         //atualiza Z0/Z1 do trafo em pu de Snom
         clone->z0 = z0;
         clone->z1 = z1;
         //exibe os valores de Z0/Z1 na nova base
         ExibeZ0Z1();
         }
      }catch(Exception &)
         {
         Aviso("Dados inválidos.\nOs valores de Z0 e Z1 não puderam ser calculados");
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CLBoxAtributoClickCheck(TObject *Sender)
   {
   //variáveis locais
   bool        enabled;
   TWinControl *WinControl;

   //habilita/desabilita todos WinControl
   for (int i = 0; i < CLBoxAtributo->Count; i++)
      {
      WinControl = (TWinControl*)CLBoxAtributo->Items->Objects[i];
      //habilita/desabilita WinControl
      WinControl->Enabled = CLBoxAtributo->Checked[i];
      //habilita/desabilita WinControl filhos
      for (int j = 0; j < WinControl->ControlCount; j++)
         {
         WinControl->Controls[j]->Enabled = WinControl->Enabled;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CLBoxAtributoInicia(void)
   {
   //variáveis locais
   int    index;
   bool   trafo_unico = (lisTRF->Count == 1);
   struct strWIN_CTRL   {
                        TWinControl *winctrl;
                        bool         enabled;
                        char        *nome;
                        } vet_winctrl[] =
                                    {{PanelRede,         trafo_unico, "Rede"},
                                     {PanelCodigo,       trafo_unico, "Código"},
                                     {PanelSnom,         true,        "Potência nominal"},
                                     {PanelPerdaFerro,   true,        "Perda ferro"},
                                     {PanelLtc,          true,        "LTC: Tipo de ajuste"},
                                     {PanelLtcBarra,     trafo_unico, "LTC: Barra de referêncai"},
                                     {PanelLtcTensao,    true,        "LTC: Tensão alvo"},
                                     {PanelZ0Z1,         true,        "Impedâncias de sequência"},
                                     {PanelAutoTrafo,    true,        "Auto-transformador"},
                                     {PanelPriBarra,     trafo_unico, "Primário: Barra"},
                                     {PanelPriLigacao,   true,        "Primário: Tipo de ligação"},
                                     {PanelPriTensao,    trafo_unico, "Primário: Tensões"},
                                     {gboxPriZterra,     true,        "Primário: Impedância de aterramento"},
                                     {PanelSecBarra,     trafo_unico, "Secundário: Barra"},
                                     {PanelSecLigacao,   true,        "Secundário: Tipo de ligação"},
                                     {PanelSecTensao,    trafo_unico, "Secundário: Tensões"},
                                     {gboxSecZterra,     true,        "Secundário: Impedância de aterramento"},
                                     {gboxSecDefasagem,  true,        "Secundário: Ângulo de defasagem"}
                                    };

   //reinicia CLBoxAtributo
   CLBoxAtributo->Clear();
   //configura
   for (int n = 0; n < sizeof(vet_winctrl)/sizeof(strWIN_CTRL); n++)
      {
      index = CLBoxAtributo->Items->AddObject(vet_winctrl[n].nome, vet_winctrl[n].winctrl);
      CLBoxAtributo->Checked[index] = vet_winctrl[n].enabled;
      }
   //simula evento para configurar os componenetes
   CLBoxAtributoClickCheck(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::checkAutoTrafoClick(TObject *Sender)
   {
   //variáveis locais
   double defasagem;

   //verifica se está marcada a opção de auto-trafo
   if (checkAutoTrafo->Checked)
      {//auto-trafo: muda as ligações do primário e do secundário
      clone->auto_trafo    = true;
      clone->pri.ligacao   = lgEST_AT;
      clone->sec.ligacao   = lgEST_AT;
      clone->sec.defasagem = 0;
      ExibeSecDefasagem(); //FKM - atualiza o combobox
      }
   else
      {//assume do Trafo triângulo/estrela c/ defasagem 30
      clone->auto_trafo    = false;
      clone->pri.ligacao   = lgTRIANG;
      clone->sec.ligacao   = lgEST_ISO;
      clone->sec.defasagem = 30.;
      ExibeSecDefasagem(); //FKM - atualiza o combobox
      }
   //exibe/esconde campos condicionais em relação ao auto_trafo
   ExibeDadosCondicionais();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::ConverteZ0Z1(strIMP &z0, strIMP &z1, double sbase_old, double sbase_new)
   {
	//variaveis locais
   double fat_mult;

   if (IsDoubleZero(sbase_old) || IsDoubleZero(sbase_new)) return(false);
   //define fator de multiplicação p/ converter z0 e z1 na base selecionada
   fat_mult = sbase_new / sbase_old;
   //recalcula Z0 e Z1
   z0.r *= fat_mult;
   z0.x *= fat_mult;
   z1.r *= fat_mult;
   z1.x *= fat_mult;

   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::DefValoresDefaultTrafo(void)
   {
   //variáveis locais
   double  vat, vmt;
   VTTrafo *ult_trf;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//determina Vnom p/ primário e secundário
	vat = max(clone->pbarra1->vnom, clone->pbarra2->vnom);
   vmt = min(clone->pbarra1->vnom, clone->pbarra2->vnom);
   //verifica se o Trafo não está definido
   if ((clone->snom == 0.)||(clone->pri.vnom == 0.)||(clone->sec.vnom == 0.))
      {//inicia dados gerais c/ valores default
      clone->auto_trafo  = false;
      clone->snom        = 10;
      clone->perda_ferro = 0.1;
      clone->z0.r        = 0.;
      clone->z0.x        = 0.05;
      clone->z1.r        = 0.;
      clone->z1.x        = 0.05;
      //inicia dados do primário
      clone->pri.vnom      = vat;
		clone->pri.tap       = vat;
      clone->pri.ligacao   = lgTRIANG;
      clone->pri.defasagem = 0.;
      clone->pri.zterra.r  = 0;
      clone->pri.zterra.x  = 0;
      //inicia dados do secundário: Dy1
      clone->sec.vnom      = vmt;
      clone->sec.tap       = vmt;
      clone->sec.ligacao   = lgEST_AT;
      clone->sec.defasagem = -30.;
      clone->sec.zterra.r  = 0;
      clone->sec.zterra.x  = 0;
      //inicia dados de LTC
		clone->ltc.tipo             = ltcFIXO;
		clone->ltc.ajuste_auto.v_pu = 1.0;
      //seleciona Trafo de referência p/ copiar alguns dados
      lisTMP->Clear();
      redes->LisLigacao(lisTMP, eqptoTRAFO);
//      if (lisTMP->Count == 0) return;
//      if ((ult_trf = (VTTrafo*)lisTMP->Last()) == NULL)  return;
      //determina último Trafo trifásico //DVK 2016.11.22
      for (int n = lisTMP->Count-1; n >= 0; n--)
         {
         ult_trf = (VTTrafo*)lisTMP->Items[n];
         if (ult_trf->Monofasico) continue;
         //inicia dados gerais
         clone->snom   = ult_trf->snom;
         clone->z0     = ult_trf->z0;
         clone->z1     = ult_trf->z1;
         //inicia dados do primário
         clone->pri.ligacao = ult_trf->pri.ligacao;
         clone->pri.zterra  = ult_trf->pri.zterra;
         //inicia dados do secundário
         clone->sec.ligacao   = ult_trf->sec.ligacao;
         clone->sec.defasagem = ult_trf->sec.defasagem;
         clone->sec.zterra    = ult_trf->sec.zterra;
         break;
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeAutoTrafo(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelAutoTrafo->Enabled) return;
   checkAutoTrafo->Checked = clone->auto_trafo; //FKM 2015.10.05 - Não atualizava o checkbox
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeCodigo(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelCodigo->Enabled) return;
   EditCodigo->Text   = clone->Codigo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeDados(void)
	{
   //a exibição dos dados depende de os campos estarem habilitados
   ExibeRede();
   ExibeCodigo();
   ExibePotenciaNominal();
   ExibePerdaFerro();
   ExibeLtcTipo();
   //FKM- 2015.09.03 /* inicia o Combobox antes
   CBoxLtcBarInicia();
   //FKM- 2015.09.03                            */
   ExibeLtcBar();
   ExibeLtcVpu();
   ExibeZ0Z1();
   ExibeAutoTrafo();
   ExibePriBarra();
   ExibePriLigacao();
   ExibePriTensao();
   ExibePriZat();
   ExibeSecBarra();
   ExibeSecLigacao();
   ExibeSecTensao();
   ExibeSecZat();
   ExibeSecDefasagem();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeDadosCondicionais(void)
   {
   //exibe/esconde campos condicionais em relação a auto-trafo
   PanelPriLigacao->Visible = ! clone->auto_trafo;
   PanelSecLigacao->Visible = ! clone->auto_trafo;
   //exibe/esconde campos condicionais em relação ao tipo de ligação
   gboxPriZterra->Visible = (clone->pri.ligacao == lgEST_AT);
   gboxSecZterra->Visible = (clone->sec.ligacao == lgEST_AT);
   //exibe campos condicionais em relação a LTC
	switch (clone->ltc.tipo)
		{
      case 1: //ajuste automático no tap do primário
         //habilita seleção de barra e edição de tensão controlada
         GBoxLTC->Visible = true;
         //desabilita edição de Vtap do primário
         LabelPriTap->Visible  = false;
         EditPriVtap->Visible  = false;
         //desabilita edição de Vtap do secundário
         LabelSecTap->Visible  = false;
         EditSecVtap->Visible  = false;
         break;
      case 2:  //ajuste automático no tap do secundário
         //habilita seleção de barra e edição de tensão controlada
         GBoxLTC->Visible = true;
         //desabilita edição de Vtap do primário
         LabelPriTap->Visible  = false;
         EditPriVtap->Visible  = false;
         //desabilita edição de Vtap do secundário
         LabelSecTap->Visible  = false;
         EditSecVtap->Visible  = false;
         break;
      default:  //não utiliza LTC
         //desabilita seleção de barra e edição de tensão controlada
         GBoxLTC->Visible = false;
         //habilita edição de Vtap do primário
         LabelPriTap->Visible  = true;
         EditPriVtap->Visible  = true;
         //habilita edição de Vtap do secundário
         LabelSecTap->Visible  = true;
         EditSecVtap->Visible  = true;
         break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeLtcBar(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelLtcBarra->Enabled)
      {
      CBoxLtcBar->ItemIndex = -1;
      return;
      }
  //verifica se há Barras no CBox
  if (CBoxLtcBar->Items->Count > 0)
     {//verifica se há uma Barra de referência definida
	  if (clone->ltc.ajuste_auto.bar_ref)
		  {//pré-seleciona Barra controlada
		  CBoxLtcBar->ItemIndex = CBoxLtcBar->Items->IndexOfObject(clone->ltc.ajuste_auto.bar_ref);
        }
     else
        {//seleciona Barra do secundário
        CBoxLtcBar->ItemIndex = CBoxLtcBar->Items->IndexOfObject(clone->pbarra2);
        }
      //pré-seleciona primeira Barra
      if (CBoxLtcBar->ItemIndex < 0) CBoxLtcBar->ItemIndex = 0;
      }
   }


//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeLtcTipo(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelLtc->Enabled)
      {
      CBoxLtcTipo->ItemIndex = -1;
      return;
      }
   //verifica opção de LTC do Trafo
	switch(clone->ltc.tipo)
      {
		case ltcFIXO:        CBoxLtcTipo->ItemIndex = 0; break;
      case ltcPRIMARIO:    CBoxLtcTipo->ItemIndex = 1; break;
      case ltcSECUNDARIO:  CBoxLtcTipo->ItemIndex = 2; break;
      default:             CBoxLtcTipo->ItemIndex = 0; break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeLtcVpu(void)
   {
   if (! PanelLtcTensao->Enabled) return;
   //exibe tensão controlada
	EditLtcVpu->Text = DoubleToStr("%5.4f", clone->ltc.ajuste_auto.v_pu);
	}

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibePerdaFerro(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelPerdaFerro->Enabled)
      {
      edtPerdaFe->Text = "";
      return;
      }
   //exibe perda no ferro
   edtPerdaFe->Text = DoubleToStr("%4.3f", clone->perda_ferro);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibePotenciaNominal(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelSnom->Enabled)
      {
      edtSnom->Text = "";
      return;
      }
   //exibe Snom
   edtSnom->Text = DoubleToStr("%4.3f", clone->snom);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibePriBarra(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelPriBarra->Enabled)
      {//não seleciona nenhuma Barra
      CBoxPriBar->ItemIndex = -1;
      return;
      }
   //seleciona Barra do primário
   CBoxPriBar->ItemIndex = CBoxPriBar->Items->IndexOfObject(clone->pbarra1);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibePriLigacao(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelPriLigacao->Enabled)
      {
      CBoxPriLiga->ItemIndex = -1;
      return;
      }
   //exibe ligação do primário
   switch(clone->pri.ligacao)
      {
      case lgEST_AT:  CBoxPriLiga->ItemIndex = 0; break;
      case lgTRIANG:  CBoxPriLiga->ItemIndex = 1; break;
      case lgEST_ISO: CBoxPriLiga->ItemIndex = 2; break;
      default:        CBoxPriLiga->ItemIndex = 1; break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibePriTensao(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelPriTensao->Enabled)
      {
      EditPriVnom->Text = "";
      EditPriVtap->Text  = "";
      return;
      }
   EditPriVnom->Text = DoubleToStr("%4.3f", clone->pri.vnom);
   EditPriVtap->Text  = DoubleToStr("%4.3f", clone->pri.tap);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibePriZat(void)
   {
   //verifica se o atributo está habilitado
   if (! gboxPriZterra->Enabled)
      {
      EditPriRat->Text = "";
      EditPriXat->Text = "";
      return;
      }
   //exibe Zat do primário
   EditPriRat->Text = DoubleToStr("%2.1f", clone->pri.zterra.r);
   EditPriXat->Text = DoubleToStr("%2.1f", clone->pri.zterra.x);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeRede(void)
   {
   //variáveis looais
   VTRede *rede;

   //verifica se o atributo está habilitado
   if (! PanelRede->Enabled)
      {
      CBoxRede->ItemIndex = -1;
      return;
      }
   //exibe Rede do Trafo
   if (clone->rede != NULL)
      {
      CBoxRede->ItemIndex = CBoxRede->Items->IndexOfObject(clone->rede);
      }
   //seleciona Rede que contém a barra do primário
   else if ((rede = ExisteRede(clone->pbarra1)) != NULL)
      {
      CBoxRede->ItemIndex = CBoxRede->Items->IndexOfObject(rede);
      }
   //seleciona Rede que contém a barra do primário
   else if ((rede = ExisteRede(clone->pbarra2)) != NULL)
      {
      CBoxRede->ItemIndex = CBoxRede->Items->IndexOfObject(rede);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeSecBarra(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelSecBarra->Enabled)
      {//não seleciona Barra nenhuma
      CBoxSecBar->ItemIndex = -1;
      return;
      }
   //seleciona Barra do primário
   CBoxSecBar->ItemIndex = CBoxSecBar->Items->IndexOfObject(clone->pbarra2);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeSecDefasagem(void)
   {
   //variáveis locais
   double defasagem;

   //verifica se o atributo está habilitado
   if (! gboxSecDefasagem->Enabled)
      {
      EditSecDefasagem->Text = "";
      return;
      }
   //exibe ângulo de defasagem do secundário
   EditSecDefasagem->Text = DoubleToStr("%2.1f", clone->sec.defasagem);
   }
*/

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/17071782
void __fastcall TFormEdtTrf::ExibeSecDefasagem(void)
   {
   //reinicia CBoxSecDefasagem
   CBoxSecDefasagem->Clear();
   //verifica se o atributo está habilitado
   if (! gboxSecDefasagem->Enabled) return;
   //reinicia opções do CBoxSecDefasagem
   if ((clone->pri.ligacao == lgTRIANG)&&(clone->sec.ligacao != lgTRIANG))
      {
      CBoxSecDefasagem->Items->Add("Dy1");        //-30
      CBoxSecDefasagem->Items->Add("Dy11");       //30
      CBoxSecDefasagem->ItemIndex = (clone->sec.defasagem <= 0) ? 0 : 1;
      }
   else if ((clone->pri.ligacao != lgTRIANG)&&(clone->sec.ligacao == lgTRIANG))
      {
      CBoxSecDefasagem->Items->Add("Yd1");        //-30
      CBoxSecDefasagem->Items->Add("Yd11");       //30
      CBoxSecDefasagem->ItemIndex = (clone->sec.defasagem <= 0) ? 0 : 1;
      }
   else
      {
      CBoxSecDefasagem->Items->Add("não se aplica");
      CBoxSecDefasagem->ItemIndex = 0; //FKM - 2015.10.06 - define um valor
      }
   }

/*
 //comentado em DVK 2016.11.16
 //descomentado em DVK 2020.01.10
//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeSecDefasagem(void)
   {
   //reinicia CBoxSecDefasagem
   CBoxSecDefasagem->Clear();
   //verifica se o atributo está habilitado
   if (! gboxSecDefasagem->Enabled) return;
   //inicia opções do CBoxSecDefasagem
   CBoxSecDefasagem->Items->Add("não se aplica");   //0
   CBoxSecDefasagem->Items->Add("Dy1");             //1
   CBoxSecDefasagem->Items->Add("Dy11");            //2
   if(clone->sec.defasagem == 30)
      {
      CBoxSecDefasagem->ItemIndex = 2;
      }
   else if (clone->sec.defasagem == -30)
      {
      CBoxSecDefasagem->ItemIndex = 1;
      }
   else
      {
      CBoxSecDefasagem->ItemIndex = 0;
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeSecLigacao(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelSecLigacao->Enabled)
      {
      CBoxSecLiga->ItemIndex = -1;
      return;
      }
   //exibe ligação do secundário
   switch(clone->sec.ligacao)
      {
      case lgEST_AT:  CBoxSecLiga->ItemIndex = 0; break;
      case lgTRIANG:  CBoxSecLiga->ItemIndex = 1; break;
      case lgEST_ISO: CBoxSecLiga->ItemIndex = 2; break;
      default:        CBoxSecLiga->ItemIndex = 0; break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeSecTensao(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelSecTensao->Enabled)
      {
      EditSecVnom->Text = "";
      EditSecVtap->Text = "";
      return;
      }
   EditSecVnom->Text = DoubleToStr("%4.3f", clone->sec.vnom);
   EditSecVtap->Text  = DoubleToStr("%4.3f", clone->sec.tap);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeSecZat(void)
   {
   //verifica se o atributo está habilitado
   if (! gboxSecZterra->Enabled)
      {
      EditSecRat->Text = "";
      EditSecXat->Text = "";
      return;
      }
   //exibe Zat do primário
   EditSecRat->Text = DoubleToStr("%2.1f", clone->sec.zterra.r);
   EditSecXat->Text = DoubleToStr("%2.1f", clone->sec.zterra.x);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeZ0Z1(void)
   {
   //variáveis locais
   strIMP z0, z1;

   //verifica se o atributo está habilitado
   if (! PanelZ0Z1->Enabled) return;
   //determina valores de Z0/Z1 do Trafo
   z0 = clone->z0;
   z1 = clone->z1;
   //verifica se o valor deve ser mostrado em pu de Sb100MVA
   if (CBoxSbase->ItemIndex == 1)
      {//lê potência nominal
      if (! LePotenciaNominal()) return;
      //converte valores lidos de Z0/Z1 para pu de Sb100
      ConverteZ0Z1(z0, z1, clone->snom, sbase_100mva);
      }
   EditR0->Text = DoubleToStr("%6.5f", z0.r);
   EditX0->Text = DoubleToStr("%6.5f", z0.x);
   EditR1->Text = DoubleToStr("%6.5f", z1.r);
   EditX1->Text = DoubleToStr("%6.5f", z1.x);
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TFormEdtTrf::ExisteRede(VTBarra *barra)
   {
   //variáveis locais
   VTRede  *rede;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   TList   *lisREDE = redes->LisRede();

   //procura uma Rede que conecta a Barras do Trafo indicada
   for (int n = 0; n < lisREDE->Count; n++)
      {
      rede = (VTRede*)lisREDE->Items[n];
      if (rede->ExisteBarra(barra)) return(rede);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form sem alinhamento
   Left = 0;
   Top  = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::FormShow(TObject *Sender)
   {
   //se necessário, define valores default p/ Trafo
   DefValoresDefaultTrafo();
   //inicia CBoxPriBar
   CBoxPriBarInicia();
   //inicia CBoxPriBar
   CBoxSecBarInicia();
   //inicia CBox c/ as Redes
   CBoxRedeInicia();
   //inicia CBoxLtcBar
   CBoxLtcBarInicia();
   //exibe dados do Trafo
   ExibeDados();
   //exibe/esconde dados opcionais
   ExibeDadosCondicionais();
   //seleciona TableSheet
   if   (lisTRF->Count == 1) pgCtrl->ActivePage = TabSheetTrafo;
   else                      pgCtrl->ActivePage = TabSheetAtributo;
   //posiciona o Cursor em EditCodigo
   if (EditCodigo->Enabled) EditCodigo->SetFocus();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeAutoTrafo(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelAutoTrafo->Enabled) return(true);
   //lê opção de auto-trafo
   clone->auto_trafo  = checkAutoTrafo->Checked;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeCodigo(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelCodigo->Enabled) return(true);
   //lê código
   clone->Codigo  = EditCodigo->Text;
   //verifica código nulo
   if (clone->Codigo.IsEmpty())
      {
      //if (Confirma("Código não preenchido.", "Deseja definir o código?") != IDNO) return(false);
      //FKM - 2015.10.05 poe o focus onde o usuário precisa editar   \\
      if (Confirma("Código não preenchido.", "Deseja definir o código?") != IDNO)
         {
         pgCtrl->ActivePage = TabSheetTrafo;
         EditCodigo->SetFocus();
         return(false);
         }
      }
      //FKM - 2015.10.05    //
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeDadosTrafo(void)
	{
   //a leitura dos dados depende de os campos estarem habilitados
   if (! LeRede())            return(false);
   if (! LeCodigo())          return(false);
   if (! LePotenciaNominal()) return(false);
   if (! LePerdaFerro())      return(false);
   if (! LeLtcTipo())         return(false);
   if (! LeLtcBar())          return(false);
   if (! LeLtcVpu())          return(false);
   if (! LeZ0Z1())            return(false);
   if (! LeAutoTrafo())       return(false);
   if (! LePriBarra())        return(false);
   if (! LePriLigacao())      return(false);
   if (! LePriDefasagem())    return(false);
   if (! LePriTensao())       return(false);
   if (! LePriZat())          return(false);
   if (! LeSecBarra())        return(false);
   if (! LeSecLigacao())      return(false);
   if (! LeSecDefasagem())    return(false);
   if (! LeSecTensao())       return(false);
   if (! LeSecZat())          return(false);

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeLtcBar(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelLtcBarra->Enabled) return(true);
   //verifica se opção de ajuste automático está selecionada
   if (CBoxLtcTipo->ItemIndex <= 0)
      {
		clone->ltc.ajuste_auto.bar_ref  = NULL;
		return(true);
      }
	//verifica se há Barra de referência selecionada
   if (CBoxLtcBar->ItemIndex < 0)
      {
//      if (aviso_enabled) Aviso("Selecione barra de referência para ajuste automático de tensão");
//      return(false);
         //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
        if (aviso_enabled)
           {
           Aviso("Selecione barra de referência para ajuste automático de tensão");
           pgCtrl->ActivePage = TabSheetTrafo;
           CBoxLtcBar->DroppedDown = true;
           }
        return(false);
        //FKM 2015.10.06   //
      }
   //salva Barra de referência
	clone->ltc.ajuste_auto.bar_ref = (VTBarra*)(CBoxLtcBar->Items->Objects[CBoxLtcBar->ItemIndex]);
	return( clone->ltc.ajuste_auto.bar_ref != NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeLtcTipo(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelLtc->Enabled) return(true);
   //salva opção de LTC
   switch(CBoxLtcTipo->ItemIndex)
      {
		case 0:   clone->ltc.tipo = ltcFIXO;       break;
		case 1:   clone->ltc.tipo = ltcPRIMARIO;   break;
		case 2:   clone->ltc.tipo = ltcSECUNDARIO; break;
      default:  Aviso("Selecione opção de ajuste automático"); return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeLtcVpu(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelLtcTensao->Enabled) return(true);
   try{//verifica se opção de ajuste automático está selecionada
   if (CBoxLtcTipo->ItemIndex == 0)
      {//não está utilizando ajuste automático
		clone->ltc.ajuste_auto.v_pu = 1.0;
      return(true);
      }
      //lê tensão da barra controlada
		clone->ltc.ajuste_auto.v_pu = StrToDouble(EditLtcVpu->Text);
      //verifica valor
		if (clone->ltc.ajuste_auto.v_pu < 0.8)
         {
         //if (aviso_enabled) Aviso("Tensão da barra referência com valor abaixo de 0.8 pu");
         //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
         if (aviso_enabled)
            {
            Aviso("Tensão da barra referência com valor abaixo de 0.8 pu");
            pgCtrl->ActivePage = TabSheetTrafo;
            EditLtcVpu->SetFocus();
            }
         //FKM 2015.10.06     //
         return(false);
         }
		else if (clone->ltc.ajuste_auto.v_pu > 1.2)
         {
         //if (aviso_enabled) Aviso("Tensão da barra referência com valor acima de 1.2 pu");
         //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
         if (aviso_enabled)
            {
            Aviso("Tensão da barra referência com valor acima de 1.2 pu");
            pgCtrl->ActivePage = TabSheetTrafo;
            EditLtcVpu->SetFocus();
            }
         //FKM 2015.10.06     //
         return(false);
         }
      }catch(Exception &e)
         {
         //if (aviso_enabled) Erro("Tensão da barra referência com valor inválido");
         //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
         if (aviso_enabled)
            {
            Erro("Tensão da barra referência com valor inválido");
            pgCtrl->ActivePage = TabSheetTrafo;
            EditLtcVpu->SetFocus();
            }
         //FKM 2015.10.06     //
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LePerdaFerro(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelPerdaFerro->Enabled) return(true);
   try{//lê valor (%)
      clone->perda_ferro = StrToDouble(edtPerdaFe->Text);
		if (clone->perda_ferro < 0)
         {
         //if (aviso_enabled) Aviso("Perda no ferro com valor negativo");
         //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
         if (aviso_enabled)
            {
            Aviso("Perda no ferro com valor negativo");
            pgCtrl->ActivePage = TabSheetTrafo;
            edtPerdaFe->SetFocus();
            }
         //FKM 2015.10.06     //
         return(false);
         }
      }catch(Exception &e)
         {
         //if (aviso_enabled) Erro("Perda no ferro com valor inválido");
         //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
         if (aviso_enabled)
            {
            Erro("Perda no ferro com valor inválido");
            pgCtrl->ActivePage = TabSheetTrafo;
            edtPerdaFe->SetFocus();
            }
         //FKM 2015.10.06     //
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LePotenciaNominal(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelSnom->Enabled) return(true);
   try{//lê valor
      clone->snom = StrToDouble(edtSnom->Text);
      if (IsDoubleZero(clone->snom))
         {
         //if (aviso_enabled) Aviso("Potência nominal com valor nulo");
         //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
         if (aviso_enabled)
            {
            Aviso("Potência nominal com valor nulo");
            pgCtrl->ActivePage = TabSheetTrafo;
            edtSnom->SetFocus();
            }
         //FKM 2015.10.06     //
         return(false);
         }
      }catch(Exception &e)
         {
         //if (aviso_enabled) Erro("Potência nominal com valor inválido");
         //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
         if (aviso_enabled)
            {
            Erro("Potência nominal com valor inválido");
            pgCtrl->ActivePage = TabSheetTrafo;
            edtSnom->SetFocus();
            }
         //FKM 2015.10.06     //
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LePriBarra(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelPriBarra->Enabled) return(true);
   //verifica se há uma Barra selecionada
   if (CBoxPriBar->ItemIndex < 0)
      {
      //if (aviso_enabled) Aviso("Selecione barra do primário");
      //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
      if (aviso_enabled)
         {
         Aviso("Selecione barra do primário");
         pgCtrl->ActivePage = TabSheetPrimario;
         CBoxPriBar->DroppedDown = true;
         }
      //FKM 2015.10.06     //
      return(false);
      }
   //determina Barra selecionada
   clone->pbarra1 = (VTBarra*)(CBoxPriBar->Items->Objects[CBoxPriBar->ItemIndex]);

   return(clone->pbarra1 != NULL);
   }


//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LePriDefasagem(void)
   {
   //defasagem do primário sempre é nula
   clone->pri.defasagem = 0;
   return true; //FKM - estava sem return...
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LePriLigacao(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelPriLigacao->Enabled) return(true);
   //lê ligação do primário
   switch(CBoxPriLiga->ItemIndex)
      {
      case 0: clone->pri.ligacao = lgEST_AT;  break;
      case 1: clone->pri.ligacao = lgTRIANG;  break;
      case 2: clone->pri.ligacao = lgEST_ISO; break;
      default:
         //if (aviso_enabled) Aviso("Selecione ligação do primário");
         //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
         if (aviso_enabled)
            {
            Aviso("Selecione ligação do primário");
            pgCtrl->ActivePage = TabSheetPrimario;
            CBoxPriLiga->DroppedDown = true;
            }
         //FKM 2015.10.06     //
         return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LePriTensao(void)
   {
   //variáveis locais
   AnsiString txt;

   //verifica se o atributo está habilitado
   if (! PanelPriTensao->Enabled) return(true);
   try{//lê tensão nominal
      clone->pri.vnom = StrToDouble(EditPriVnom->Text);
      }catch(Exception &e)
         {
         //if (aviso_enabled) Erro("Tensão nominal do primário com valor inválido");
         //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
         if (aviso_enabled)
            {
            Erro("Tensão nominal do primário com valor inválido");
            pgCtrl->ActivePage = TabSheetPrimario;
            EditPriVnom->SetFocus();
            }
         //FKM 2015.10.06     //
         return(false);
         }
   try{//lê tensão de tap
      clone->pri.tap = StrToDouble(EditPriVtap->Text);
      }catch(Exception &e)
         {
         //if (aviso_enabled) Erro("Tensão de tap do primário com valor inválido");
         //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
         if (aviso_enabled)
            {
            Erro("Tensão de tap do primário com valor inválido");
            pgCtrl->ActivePage = TabSheetPrimario;
            EditPriVtap->SetFocus();
            }
         //FKM 2015.10.06     //
         return(false);
         }
   //compara tensão do trafo com tesnão da Barra
   if (! IsDoubleZero(clone->pri.vnom - clone->pbarra1->vnom))
      {
//      if (aviso_enabled)
//         {
//         Aviso(txt.sprintf("Vnom do primário(%4.3f kV) diferente da tensão da barra (%4.3f kV)",
//                           clone->pri.vnom, clone->pbarra1->vnom));
//         }
          //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
         if (aviso_enabled)
            {
            Aviso(txt.sprintf("Vnom do primário(%4.3f kV) diferente da tensão da barra (%4.3f kV)",
                           clone->pri.vnom, clone->pbarra1->vnom));
            pgCtrl->ActivePage = TabSheetPrimario;
            EditPriVnom->Hint = DoubleToStr("%4.3f",clone->pbarra1->vnom);
            EditPriVnom->SetFocus();
            }
         //FKM 2015.10.06     //
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LePriZat(void)
   {
   //verifica se o atributo está habilitado
   if (! gboxPriZterra->Enabled) return(true);
   //verifica se o primário é estrela aterrado
   if (clone->pri.ligacao == lgEST_AT)
      {
      try{//lê resistência de aterramento
         clone->pri.zterra.r = StrToDouble(EditPriRat->Text);
         }catch(Exception &e)
            {
            //if (aviso_enabled) Erro("Resistência de aterramento com valor inválido");
            //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
            if (aviso_enabled)
               {
               Erro("Resistência de aterramento com valor inválido");
               pgCtrl->ActivePage = TabSheetPrimario;
               EditPriRat->SetFocus();
               }
            //FKM 2015.10.06     //
            return(false);
            }
      try{//lê reatância de aterramento
         clone->pri.zterra.x = StrToDouble(EditPriXat->Text);
         }catch(Exception &e)
            {
            //if (aviso_enabled) Erro("Reatância de aterramento com valor inválido");
            //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
            if (aviso_enabled)
               {
               Erro("Reatância de aterramento com valor inválido");
               pgCtrl->ActivePage = TabSheetPrimario;
               EditPriXat->SetFocus();
               }
            //FKM 2015.10.06     //
            return(false);
            }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeSecBarra(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelSecBarra->Enabled) return(true);
   //verifica se há uma Barra selecionada
   if (CBoxSecBar->ItemIndex < 0)
      {
      //if (aviso_enabled) Aviso("Selecione barra do secundário");
      //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
      if (aviso_enabled)
         {
         Aviso("Selecione barra do secundário");
         pgCtrl->ActivePage = TabSheetPrimario;
         CBoxSecBar->DroppedDown = true;
         }
      //FKM 2015.10.06     //
      return(false);
      }
   //determina Barra selecionada
   clone->pbarra2 = (VTBarra*)(CBoxSecBar->Items->Objects[CBoxSecBar->ItemIndex]);

   return(clone->pbarra2 != NULL);
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeSecDefasagem(void)
   {
   //verifica se o atributo está habilitado
   if (! gboxSecDefasagem->Enabled) return(true);
   try{//lê valor do ângulo
      clone->sec.defasagem = StrToDouble(EditSecDefasagem->Text);
      }catch(Exception &e)
         {
         if (aviso_enabled) Erro("Ângulo de defasagem do secundário com valor inválido");
         return(false);
         }
   return(true);
   }
*/
/*
//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeSecDefasagem(void)
   {
   //variáveis locais
   int defasagem;

   //verifica se o atributo está habilitado
   if (! gboxSecDefasagem->Enabled) return(true);
   //verifica se opção de ligação está selecionada
   if (CBoxSecDefasagem->ItemIndex < 0)
      {
      //if (aviso_enabled) Aviso("Selecione a defasagem entre primário/secundário");
      //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
      if (aviso_enabled)
         {
         Aviso("Selecione a defasagem entre primário/secundário");
         pgCtrl->ActivePage = TabSheetSecundario;
         CBoxSecDefasagem->DroppedDown = true;
         }
      //FKM 2015.10.06     //
      return(false);
      }
   //define valor do ângulo em função da ligação selecionada
   if ((clone->pri.ligacao == lgTRIANG)&&(clone->sec.ligacao != lgTRIANG))
      {
      if (CBoxSecDefasagem->ItemIndex == 0) defasagem = -30;  //Dy1
      else                                  defasagem =  30;  //Dy11
      }
   else if ((clone->pri.ligacao != lgTRIANG)&&(clone->sec.ligacao == lgTRIANG))
      {
      if (CBoxSecDefasagem->ItemIndex == 0) defasagem = -30;   //Yd1
      else                                  defasagem =  30;   //Yd11
      }
   else
      {
      defasagem = 0;
      }
   //atualiza ângulo de defasagem
   clone->sec.defasagem = defasagem;
   return(true);
   }
*/    //DVK 2016.11.16
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/17071782
bool __fastcall TFormEdtTrf::LeSecDefasagem(void)
   {
   //variáveis locais
   int defasagem;

   //verifica se o atributo está habilitado
   if (! gboxSecDefasagem->Enabled) return(true);
   //verifica se opção de ligação está selecionada
   if (CBoxSecDefasagem->ItemIndex < 0)
      {
      //if (aviso_enabled) Aviso("Selecione a defasagem entre primário/secundário");
      //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
      if (aviso_enabled)
         {
         Aviso("Selecione a defasagem entre primário/secundário");
         pgCtrl->ActivePage = TabSheetSecundario;
         CBoxSecDefasagem->DroppedDown = true;
         }
      //FKM 2015.10.06     //
      return(false);
      }
   //define valor do ângulo em função da ligação selecionada
   if ((clone->pri.ligacao == lgTRIANG)&&(clone->sec.ligacao != lgTRIANG))
      {
      switch(CBoxSecDefasagem->ItemIndex)
         {
//         case 0:  Aviso("Defasagem inválida");
//                  return(false);
         case 0:  defasagem = -30;  //Dy1
                  break;
         case 1:  defasagem =  30;  //Dy11
                  break;
         }
      }
   else if ((clone->pri.ligacao != lgTRIANG)&&(clone->sec.ligacao == lgTRIANG))
      {
      switch(CBoxSecDefasagem->ItemIndex)
         {
//         case 0:  Aviso("Defasagem inválida");
//                  return(false);
         case 0:  defasagem = -30;  //Yd1
                  break;
         case 1:  defasagem =  30;  //Yd11
                  break;
         }
      }
   else
      {
      switch(CBoxSecDefasagem->ItemIndex)
         {
         case 1:
         case 2:  Aviso("Defasagem inválida");
                  return(false);
         case 0:  defasagem = 0;
                  break;
         }
      }
   //atualiza ângulo de defasagem
   clone->sec.defasagem = defasagem;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeSecLigacao(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelSecLigacao->Enabled) return(true);
   //lê ligação do secundário
   switch(CBoxSecLiga->ItemIndex)
      {
      case 0: clone->sec.ligacao = lgEST_AT;  break;
      case 1: clone->sec.ligacao = lgTRIANG;  break;
      case 2: clone->sec.ligacao = lgEST_ISO; break;
      default:
         if (aviso_enabled) Aviso("Selecione ligação do secundário");
         return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeSecTensao(void)
   {
   //variáveis locais
   AnsiString txt;

   //verifica se o atributo está habilitado
   if (! PanelSecTensao->Enabled) return(true);
   try{//lê tensão nominal
      clone->sec.vnom = StrToDouble(EditSecVnom->Text);
      }catch(Exception &e)
         {
         //if (aviso_enabled) Erro("Tensão nominal do secundário com valor inválido");
         //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
         if (aviso_enabled)
            {
            Erro("Tensão nominal do secundário com valor inválido");
            pgCtrl->ActivePage = TabSheetSecundario;
            EditSecVnom->SetFocus();
            }
         //FKM 2015.10.06     //
         return(false);
         }
   try{//lê tensão de tap
      clone->sec.tap = StrToDouble(EditSecVtap->Text);
      }catch(Exception &e)
         {
         //if (aviso_enabled) Erro("Tensão de tap do secundário com valor inválido");
         //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
         if (aviso_enabled)
            {
            Erro("Tensão de tap do secundário com valor inválido");
            pgCtrl->ActivePage = TabSheetSecundario;
            EditSecVtap->SetFocus();
            }
         //FKM 2015.10.06     //
         return(false);
         }
   //compara tensão do trafo com tensão da Barra
   if (! IsDoubleZero(clone->sec.vnom - clone->pbarra2->vnom))
      {
//      if (aviso_enabled)
//         {
//         Aviso(txt.sprintf("Vnom do secundário(%4.3f kV) diferente da tensão da barra (%4.3f kV)",
//                           clone->sec.vnom, clone->pbarra2->vnom));
//         }
      //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
      if (aviso_enabled)
         {
         Aviso(txt.sprintf("Vnom do secundário(%4.3f kV) diferente da tensão da barra (%4.3f kV)",
                           clone->sec.vnom, clone->pbarra2->vnom));
         pgCtrl->ActivePage = TabSheetSecundario;
         EditSecVnom->Hint = DoubleToStr("%4.3f",clone->pbarra2->vnom);
         EditSecVnom->SetFocus();
         }
      //FKM 2015.10.06     //
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeSecZat(void)
   {
   //verifica se o atributo está habilitado
   if (! gboxSecZterra->Enabled) return(true);
   //verifica se o secmário é estrela aterrado
   if (clone->sec.ligacao == lgEST_AT)
      {
      try{//lê resistência de aterramento
         clone->sec.zterra.r = StrToDouble(EditSecRat->Text);
         }catch(Exception &e)
            {
            //if (aviso_enabled) Erro("Resistência de aterramento do secundário com valor inválido");
            //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
            if (aviso_enabled)
               {
               Erro("Resistência de aterramento do secundário com valor inválido");
               pgCtrl->ActivePage = TabSheetSecundario;
               EditSecRat->SetFocus();
               }
            //FKM 2015.10.06     //
            return(false);
            }
      try{//lê reatância de aterramento
         clone->sec.zterra.x = StrToDouble(EditSecXat->Text);
         }catch(Exception &e)
            {
            //if (aviso_enabled) Erro("Reatância de aterramento do secundário com valor inválido");
            //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
            if (aviso_enabled)
               {
               Erro("Reatância de aterramento do secundário com valor inválido");
               pgCtrl->ActivePage = TabSheetSecundario;
               EditSecXat->SetFocus();
               }
            //FKM 2015.10.06     //
            return(false);
            }
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeRede(void)
   {
   //verifica se o atributo está habilitado
   if (! PanelRede->Enabled) return(true);
   //verifica se foi selecionada uma Rede
   if (CBoxRede->ItemIndex < 0)
      {
      if (aviso_enabled) Aviso("Defina a Rede do Transformador");
      //FKM 2015.10.06 - Seleciona o lugar que o usuário deve editar \\
      if (aviso_enabled)
         {
         Aviso("Defina a Rede do Transformador");
         pgCtrl->ActivePage = TabSheetTrafo;
         CBoxRede->DroppedDown = true;
         }
      //FKM 2015.10.06     //
      return(false);
      }
   //determina Rede do Trafo
   clone->rede  = (VTRede*)CBoxRede->Items->Objects[CBoxRede->ItemIndex];

   return(clone->rede != NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeZ0Z1(void)
   {
   //variáveis locais
   strIMP z0, z1;

   //lê valores editados de Z0/Z1
   if (! LeZ0Z1(z0, z1)) return(false);
   //verifica se o valor editado estava em pu de Sb100
   if (CBoxSbase->ItemIndex == 1)
      {//lê potência nominal
      if (! LePotenciaNominal()) return(false);
      //converte valores lidos de Z0/Z1 para pu de Snom
      ConverteZ0Z1(z0, z1, sbase_100mva, clone->snom);
      }
   //atualiza Z0/Z1 do trafo em pu de Snom
   clone->z0 = z0;
   clone->z1 = z1;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeZ0Z1(strIMP &z0, strIMP &z1)
   {
   //verifica se o atributo está habilitado
   if (! PanelZ0Z1->Enabled) return(true);
   try{
      //fkm 2015.09.02 verificação de valores nulos
      if((StrToDouble(EditR0->Text) == 0) && (StrToDouble(EditX0->Text) == 0))
      {
         throw Exception("valores nulos de impedância");
      }
      if((StrToDouble(EditR1->Text) == 0) && (StrToDouble(EditX1->Text) == 0))
      {
         throw Exception("valores nulos de impedância");
      }
//      if(StrToDouble(EditR0->Text) == 0){ throw Exception("valores nulos de impedância");}
//      if(StrToDouble(EditX0->Text) == 0){ throw Exception("valores nulos de impedância");}
//      if(StrToDouble(EditR1->Text) == 0){ throw Exception("valores nulos de impedância");}
//      if(StrToDouble(EditX1->Text) == 0){ throw Exception("valores nulos de impedância");}
      //lê dados de z0 e z1 em pu
      z0.r  = StrToDouble(EditR0->Text);
      z0.x  = StrToDouble(EditX0->Text);
      z1.r  = StrToDouble(EditR1->Text);
      z1.x  = StrToDouble(EditX1->Text);
      }catch(Exception &e)
         {
         if (aviso_enabled) Erro("Valores inválidos de impedância de sequência");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::Moldura(VTEqpto *eqpto)
   {
   //variáveis locais
   VTGrafico *grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));

   grafico->Moldura(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::MolduraBarraLtc(void)
   {
   Moldura(clone->ltc.ajuste_auto.bar_ref);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::MolduraBarraPri(void)
   {
   Moldura(clone->pbarra1);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::MolduraBarraSec(void)
   {
   Moldura(clone->pbarra2);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::OpcaoRetira(bool enabled)
   {
   ActionRetira->Visible = enabled;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::pgCtrlChange(TObject *Sender)
   {
   //atualiza moldura
   if      (pgCtrl->ActivePage == TabSheetTrafo)      MolduraBarraLtc();
   else if (pgCtrl->ActivePage == TabSheetPrimario)   MolduraBarraPri();
   else if (pgCtrl->ActivePage == TabSheetSecundario) MolduraBarraSec();
   else                                               Moldura();
   }

//---------------------------------------------------------------------------
//eof





