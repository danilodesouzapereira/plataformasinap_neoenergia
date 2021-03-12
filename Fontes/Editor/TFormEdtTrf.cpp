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
   //salva op��o de criar c�pia
   this->criar_copia = criar_copia;
   //salva ponteiro p/ objeto
   this->apl = apl;
   geral     = (VTGeral*)apl->GetObject(__classid(VTGeral));
   //cria lista
   lisTRF = new TList();
   lisTMP = new TList();
   //salva Trafo em lisTRF
   lisTRF->Add(trafo);
   //cria um clone do Trafo que ser� usado durante edi��o
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
   //vari�veis locais
   VTTrafo *trafo;

   //salva op��o de criar c�pia
   this->criar_copia = criar_copia;
   //salva ponteiro p/ objeto
   this->apl = apl;
   geral     = (VTGeral*)apl->GetObject(__classid(VTGeral));
   //prote��o
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
   //cria um clone de um Trafo que ser� uado durante edi��o
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
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva par�metros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //apaga eventual moldura
   Moldura();
   //destr�i clone do Trafo usado na edi��o
   if (clone)  {delete clone; clone = NULL;}
   //destr�i lista sem destruir seus objetos
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
   //atualiza o clone: l� e valida dados editados
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
   //fecha o form indicando exclus�o do Eqpto
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ActionSelFotoExecute(TObject *Sender)
   {
   //vari�veis locais
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
   //vari�veis locais
   VTTrafo *trafo;

   //atualiza todos os Trafos em lisTRF: somente os campos habilitados
   for (int n = 0; n < lisTRF->Count; n++)
      {
      trafo = (VTTrafo*)lisTRF->Items[n];
      //verifica se deve criar uma c�pia
      if (criar_copia)
         {//cria um clone do Trafo e associa com trafo original
         trafo->Obj = trafo->Clone();
         //redefine trafo para apontar para a c�pia
         trafo = (VTTrafo*)(trafo->Obj);
         }
      //atualiza atributos (do trafo original ou de sua c�pia)
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
   //vari�veis locais
   VTBarra *barra;

   //determina Barra selecionada
//   if (CBoxLtcBar->ItemIndex <= 0)
//      {barra = NULL;}
//   else
//      {barra = (VTBarra*)(CBoxLtcBar->Items->Objects[CBoxLtcBar->ItemIndex]);}
   //exibe/apaga moldura na Barra selecionada
   //Moldura(barra);
   //FKM - 2015.09.03 - /* o itemindex == 0 tamb�m pegava o 1o item da lista de barras
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
   //vari�veis locais
   //TList   *lisBAR;
   VTBarra     *pbarra;
   VTRede      *rede;
   AnsiString  nomeBarra;

   //reinicia CBoxLtcBar
   CBoxLtcBar->Clear();
   //verifica se est� selecionada op��o de ajuste autom�tico
   if (CBoxLtcTipo->ItemIndex <= 0) return;
   //verifica se Rede do Trafo est� selecionada
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
   //habilita ordem alfab�tica
   CBoxLtcBar->Sorted = true;
   //n�o seleciona nenhuma Barra
   CBoxLtcBar->ItemIndex = -1;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxLtcTipoChange(TObject *Sender)
   {
   //l� tipo de Ltc
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
   //l� Barra de refer�ncia (pois sem ajuste autom�tico a barra de refer�ncia � cancelada)
   LeLtcBar();
   //atualiza campos
   ExibeDadosCondicionais();
   //exibe moldura na Barra de refer�nica
   MolduraBarraLtc();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxPriBarInicia(void)
   {
   //reinicia CBoxPriBar
   CBoxPriBar->Clear();
   CBoxPriBar->Items->AddObject(clone->pbarra1->Codigo, clone->pbarra1);
   CBoxPriBar->Items->AddObject(clone->pbarra2->Codigo, clone->pbarra2);
   //n�o seleciona nenhuma Barra
   CBoxPriBar->ItemIndex = -1;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxPriBarChange(TObject *Sender)
   {
   //vari�veis locais
   double     v_kv;
   VTBarra    *barra;

   //prote��o: verifica se houve altera��o da Barra
   if (CBoxPriBar->ItemIndex != CBoxSecBar->ItemIndex) return;
   //troca Barra do secund�rio
	CBoxSecBar->ItemIndex = 1 - CBoxPriBar->ItemIndex;
   //l� Barras do prim�rio e secund�rio
   LePriBarra();
   LeSecBarra();
   //l� tens�es do prim�rio e do secund�rio
   LePriTensao();
   LeSecTensao();
	//troca tens�es entre prim�rio/secund�rio
   v_kv = clone->pri.vnom; clone->pri.vnom = clone->sec.vnom; clone->sec.vnom = v_kv;
   v_kv = clone->pri.tap;  clone->pri.tap  = clone->sec.tap;  clone->sec.tap  = v_kv;
   //exibe tens�es do prim�rio e do secund�rio
   ExibePriTensao();
   ExibeSecTensao();
   //exibe Moldura na Barra do prim�rio
   barra = (VTBarra*)CBoxPriBar->Items->Objects[CBoxPriBar->ItemIndex];
   Moldura(barra);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxPriLigaChange(TObject *Sender)
   {
   //l� ligaca��o do prim�rio
   LePriLigacao();
   //exibe campos que podem ser afetados pela altera��o da liga��o
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
   //vari�veis locais
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
   //define ordem alfab�tica
   CBoxRede->Sorted = true;
   //n�o seleciona nenhuma rede
   CBoxRede->ItemIndex = -1;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxSecBarInicia(void)
   {
   //reinicia CBoxSecBar
   CBoxSecBar->Clear();
   CBoxSecBar->Items->AddObject(clone->pbarra1->Codigo, clone->pbarra1);
   CBoxSecBar->Items->AddObject(clone->pbarra2->Codigo, clone->pbarra2);
   //n�o seleciona nenhuma Barra
   CBoxSecBar->ItemIndex = -1;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxSecBarChange(TObject *Sender)
   {
   //vari�veis locais
   double     v_kv;
   VTBarra    *barra;

   //prote��o: verifica se houve altera��o da Barra
   if (CBoxSecBar->ItemIndex != CBoxPriBar->ItemIndex) return;
   //troca Barra do prim�rio
	CBoxPriBar->ItemIndex = 1 - CBoxSecBar->ItemIndex;
   //l� Barras do prim�rio e secund�rio
   LePriBarra();
   LeSecBarra();
   //l� tens�es do prim�rio e do secund�rio
   LePriTensao();
   LeSecTensao();
	//troca tens�es entre prim�rio/secund�rio
   v_kv = clone->pri.vnom; clone->pri.vnom = clone->sec.vnom; clone->sec.vnom = v_kv;
   v_kv = clone->pri.tap;  clone->pri.tap  = clone->sec.tap;  clone->sec.tap  = v_kv;
   //exibe tens�es do prim�rio e do secund�rio
   ExibePriTensao();
   ExibeSecTensao();
   //exibe Moldura na Barra do secund�rio
   MolduraBarraSec();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxSecLigaChange(TObject *Sender)
   {
   //l� ligacao do secund�rio
   LeSecLigacao();
   //exibe campos que podem ser afetados pela altera��o da liga��o
   ExibeSecZat();
   ExibeSecDefasagem();
   //exibe/esconde campos condiconais
   ExibeDadosCondicionais();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CBoxSbaseChange(TObject *Sender)
   {
   //vari�veis locais
   double sbase_old, sbase_new;
   strIMP z0, z1;

   try{//pergunta ao usu�rio se os valores das imped�ncias devem ser recalculados na nova base
      if (Confirma("O valor de base das imped�ncias foi alterado",
                   "Os valores de Z0 e Z1 devem ser recalculados na nova base selecionada ?") == IDYES)
         {//l� valores editados de Z0/Z1
         if (! LeZ0Z1(z0, z1)) return;
         //verifica se o valor editado estava em pu de Sb100
         if (CBoxSbase->ItemIndex == 0)
            {//l� pot�ncia nominal
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
         Aviso("Dados inv�lidos.\nOs valores de Z0 e Z1 n�o puderam ser calculados");
         }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::CLBoxAtributoClickCheck(TObject *Sender)
   {
   //vari�veis locais
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
   //vari�veis locais
   int    index;
   bool   trafo_unico = (lisTRF->Count == 1);
   struct strWIN_CTRL   {
                        TWinControl *winctrl;
                        bool         enabled;
                        char        *nome;
                        } vet_winctrl[] =
                                    {{PanelRede,         trafo_unico, "Rede"},
                                     {PanelCodigo,       trafo_unico, "C�digo"},
                                     {PanelSnom,         true,        "Pot�ncia nominal"},
                                     {PanelPerdaFerro,   true,        "Perda ferro"},
                                     {PanelLtc,          true,        "LTC: Tipo de ajuste"},
                                     {PanelLtcBarra,     trafo_unico, "LTC: Barra de refer�ncai"},
                                     {PanelLtcTensao,    true,        "LTC: Tens�o alvo"},
                                     {PanelZ0Z1,         true,        "Imped�ncias de sequ�ncia"},
                                     {PanelAutoTrafo,    true,        "Auto-transformador"},
                                     {PanelPriBarra,     trafo_unico, "Prim�rio: Barra"},
                                     {PanelPriLigacao,   true,        "Prim�rio: Tipo de liga��o"},
                                     {PanelPriTensao,    trafo_unico, "Prim�rio: Tens�es"},
                                     {gboxPriZterra,     true,        "Prim�rio: Imped�ncia de aterramento"},
                                     {PanelSecBarra,     trafo_unico, "Secund�rio: Barra"},
                                     {PanelSecLigacao,   true,        "Secund�rio: Tipo de liga��o"},
                                     {PanelSecTensao,    trafo_unico, "Secund�rio: Tens�es"},
                                     {gboxSecZterra,     true,        "Secund�rio: Imped�ncia de aterramento"},
                                     {gboxSecDefasagem,  true,        "Secund�rio: �ngulo de defasagem"}
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
   //vari�veis locais
   double defasagem;

   //verifica se est� marcada a op��o de auto-trafo
   if (checkAutoTrafo->Checked)
      {//auto-trafo: muda as liga��es do prim�rio e do secund�rio
      clone->auto_trafo    = true;
      clone->pri.ligacao   = lgEST_AT;
      clone->sec.ligacao   = lgEST_AT;
      clone->sec.defasagem = 0;
      ExibeSecDefasagem(); //FKM - atualiza o combobox
      }
   else
      {//assume do Trafo tri�ngulo/estrela c/ defasagem 30
      clone->auto_trafo    = false;
      clone->pri.ligacao   = lgTRIANG;
      clone->sec.ligacao   = lgEST_ISO;
      clone->sec.defasagem = 30.;
      ExibeSecDefasagem(); //FKM - atualiza o combobox
      }
   //exibe/esconde campos condicionais em rela��o ao auto_trafo
   ExibeDadosCondicionais();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::ConverteZ0Z1(strIMP &z0, strIMP &z1, double sbase_old, double sbase_new)
   {
	//variaveis locais
   double fat_mult;

   if (IsDoubleZero(sbase_old) || IsDoubleZero(sbase_new)) return(false);
   //define fator de multiplica��o p/ converter z0 e z1 na base selecionada
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
   //vari�veis locais
   double  vat, vmt;
   VTTrafo *ult_trf;
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

	//determina Vnom p/ prim�rio e secund�rio
	vat = max(clone->pbarra1->vnom, clone->pbarra2->vnom);
   vmt = min(clone->pbarra1->vnom, clone->pbarra2->vnom);
   //verifica se o Trafo n�o est� definido
   if ((clone->snom == 0.)||(clone->pri.vnom == 0.)||(clone->sec.vnom == 0.))
      {//inicia dados gerais c/ valores default
      clone->auto_trafo  = false;
      clone->snom        = 10;
      clone->perda_ferro = 0.1;
      clone->z0.r        = 0.;
      clone->z0.x        = 0.05;
      clone->z1.r        = 0.;
      clone->z1.x        = 0.05;
      //inicia dados do prim�rio
      clone->pri.vnom      = vat;
		clone->pri.tap       = vat;
      clone->pri.ligacao   = lgTRIANG;
      clone->pri.defasagem = 0.;
      clone->pri.zterra.r  = 0;
      clone->pri.zterra.x  = 0;
      //inicia dados do secund�rio: Dy1
      clone->sec.vnom      = vmt;
      clone->sec.tap       = vmt;
      clone->sec.ligacao   = lgEST_AT;
      clone->sec.defasagem = -30.;
      clone->sec.zterra.r  = 0;
      clone->sec.zterra.x  = 0;
      //inicia dados de LTC
		clone->ltc.tipo             = ltcFIXO;
		clone->ltc.ajuste_auto.v_pu = 1.0;
      //seleciona Trafo de refer�ncia p/ copiar alguns dados
      lisTMP->Clear();
      redes->LisLigacao(lisTMP, eqptoTRAFO);
//      if (lisTMP->Count == 0) return;
//      if ((ult_trf = (VTTrafo*)lisTMP->Last()) == NULL)  return;
      //determina �ltimo Trafo trif�sico //DVK 2016.11.22
      for (int n = lisTMP->Count-1; n >= 0; n--)
         {
         ult_trf = (VTTrafo*)lisTMP->Items[n];
         if (ult_trf->Monofasico) continue;
         //inicia dados gerais
         clone->snom   = ult_trf->snom;
         clone->z0     = ult_trf->z0;
         clone->z1     = ult_trf->z1;
         //inicia dados do prim�rio
         clone->pri.ligacao = ult_trf->pri.ligacao;
         clone->pri.zterra  = ult_trf->pri.zterra;
         //inicia dados do secund�rio
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
   //verifica se o atributo est� habilitado
   if (! PanelAutoTrafo->Enabled) return;
   checkAutoTrafo->Checked = clone->auto_trafo; //FKM 2015.10.05 - N�o atualizava o checkbox
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeCodigo(void)
   {
   //verifica se o atributo est� habilitado
   if (! PanelCodigo->Enabled) return;
   EditCodigo->Text   = clone->Codigo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeDados(void)
	{
   //a exibi��o dos dados depende de os campos estarem habilitados
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
   //exibe/esconde campos condicionais em rela��o a auto-trafo
   PanelPriLigacao->Visible = ! clone->auto_trafo;
   PanelSecLigacao->Visible = ! clone->auto_trafo;
   //exibe/esconde campos condicionais em rela��o ao tipo de liga��o
   gboxPriZterra->Visible = (clone->pri.ligacao == lgEST_AT);
   gboxSecZterra->Visible = (clone->sec.ligacao == lgEST_AT);
   //exibe campos condicionais em rela��o a LTC
	switch (clone->ltc.tipo)
		{
      case 1: //ajuste autom�tico no tap do prim�rio
         //habilita sele��o de barra e edi��o de tens�o controlada
         GBoxLTC->Visible = true;
         //desabilita edi��o de Vtap do prim�rio
         LabelPriTap->Visible  = false;
         EditPriVtap->Visible  = false;
         //desabilita edi��o de Vtap do secund�rio
         LabelSecTap->Visible  = false;
         EditSecVtap->Visible  = false;
         break;
      case 2:  //ajuste autom�tico no tap do secund�rio
         //habilita sele��o de barra e edi��o de tens�o controlada
         GBoxLTC->Visible = true;
         //desabilita edi��o de Vtap do prim�rio
         LabelPriTap->Visible  = false;
         EditPriVtap->Visible  = false;
         //desabilita edi��o de Vtap do secund�rio
         LabelSecTap->Visible  = false;
         EditSecVtap->Visible  = false;
         break;
      default:  //n�o utiliza LTC
         //desabilita sele��o de barra e edi��o de tens�o controlada
         GBoxLTC->Visible = false;
         //habilita edi��o de Vtap do prim�rio
         LabelPriTap->Visible  = true;
         EditPriVtap->Visible  = true;
         //habilita edi��o de Vtap do secund�rio
         LabelSecTap->Visible  = true;
         EditSecVtap->Visible  = true;
         break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeLtcBar(void)
   {
   //verifica se o atributo est� habilitado
   if (! PanelLtcBarra->Enabled)
      {
      CBoxLtcBar->ItemIndex = -1;
      return;
      }
  //verifica se h� Barras no CBox
  if (CBoxLtcBar->Items->Count > 0)
     {//verifica se h� uma Barra de refer�ncia definida
	  if (clone->ltc.ajuste_auto.bar_ref)
		  {//pr�-seleciona Barra controlada
		  CBoxLtcBar->ItemIndex = CBoxLtcBar->Items->IndexOfObject(clone->ltc.ajuste_auto.bar_ref);
        }
     else
        {//seleciona Barra do secund�rio
        CBoxLtcBar->ItemIndex = CBoxLtcBar->Items->IndexOfObject(clone->pbarra2);
        }
      //pr�-seleciona primeira Barra
      if (CBoxLtcBar->ItemIndex < 0) CBoxLtcBar->ItemIndex = 0;
      }
   }


//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeLtcTipo(void)
   {
   //verifica se o atributo est� habilitado
   if (! PanelLtc->Enabled)
      {
      CBoxLtcTipo->ItemIndex = -1;
      return;
      }
   //verifica op��o de LTC do Trafo
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
   //exibe tens�o controlada
	EditLtcVpu->Text = DoubleToStr("%5.4f", clone->ltc.ajuste_auto.v_pu);
	}

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibePerdaFerro(void)
   {
   //verifica se o atributo est� habilitado
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
   //verifica se o atributo est� habilitado
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
   //verifica se o atributo est� habilitado
   if (! PanelPriBarra->Enabled)
      {//n�o seleciona nenhuma Barra
      CBoxPriBar->ItemIndex = -1;
      return;
      }
   //seleciona Barra do prim�rio
   CBoxPriBar->ItemIndex = CBoxPriBar->Items->IndexOfObject(clone->pbarra1);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibePriLigacao(void)
   {
   //verifica se o atributo est� habilitado
   if (! PanelPriLigacao->Enabled)
      {
      CBoxPriLiga->ItemIndex = -1;
      return;
      }
   //exibe liga��o do prim�rio
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
   //verifica se o atributo est� habilitado
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
   //verifica se o atributo est� habilitado
   if (! gboxPriZterra->Enabled)
      {
      EditPriRat->Text = "";
      EditPriXat->Text = "";
      return;
      }
   //exibe Zat do prim�rio
   EditPriRat->Text = DoubleToStr("%2.1f", clone->pri.zterra.r);
   EditPriXat->Text = DoubleToStr("%2.1f", clone->pri.zterra.x);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeRede(void)
   {
   //vari�veis looais
   VTRede *rede;

   //verifica se o atributo est� habilitado
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
   //seleciona Rede que cont�m a barra do prim�rio
   else if ((rede = ExisteRede(clone->pbarra1)) != NULL)
      {
      CBoxRede->ItemIndex = CBoxRede->Items->IndexOfObject(rede);
      }
   //seleciona Rede que cont�m a barra do prim�rio
   else if ((rede = ExisteRede(clone->pbarra2)) != NULL)
      {
      CBoxRede->ItemIndex = CBoxRede->Items->IndexOfObject(rede);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeSecBarra(void)
   {
   //verifica se o atributo est� habilitado
   if (! PanelSecBarra->Enabled)
      {//n�o seleciona Barra nenhuma
      CBoxSecBar->ItemIndex = -1;
      return;
      }
   //seleciona Barra do prim�rio
   CBoxSecBar->ItemIndex = CBoxSecBar->Items->IndexOfObject(clone->pbarra2);
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeSecDefasagem(void)
   {
   //vari�veis locais
   double defasagem;

   //verifica se o atributo est� habilitado
   if (! gboxSecDefasagem->Enabled)
      {
      EditSecDefasagem->Text = "";
      return;
      }
   //exibe �ngulo de defasagem do secund�rio
   EditSecDefasagem->Text = DoubleToStr("%2.1f", clone->sec.defasagem);
   }
*/

//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/17071782
void __fastcall TFormEdtTrf::ExibeSecDefasagem(void)
   {
   //reinicia CBoxSecDefasagem
   CBoxSecDefasagem->Clear();
   //verifica se o atributo est� habilitado
   if (! gboxSecDefasagem->Enabled) return;
   //reinicia op��es do CBoxSecDefasagem
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
      CBoxSecDefasagem->Items->Add("n�o se aplica");
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
   //verifica se o atributo est� habilitado
   if (! gboxSecDefasagem->Enabled) return;
   //inicia op��es do CBoxSecDefasagem
   CBoxSecDefasagem->Items->Add("n�o se aplica");   //0
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
   //verifica se o atributo est� habilitado
   if (! PanelSecLigacao->Enabled)
      {
      CBoxSecLiga->ItemIndex = -1;
      return;
      }
   //exibe liga��o do secund�rio
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
   //verifica se o atributo est� habilitado
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
   //verifica se o atributo est� habilitado
   if (! gboxSecZterra->Enabled)
      {
      EditSecRat->Text = "";
      EditSecXat->Text = "";
      return;
      }
   //exibe Zat do prim�rio
   EditSecRat->Text = DoubleToStr("%2.1f", clone->sec.zterra.r);
   EditSecXat->Text = DoubleToStr("%2.1f", clone->sec.zterra.x);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::ExibeZ0Z1(void)
   {
   //vari�veis locais
   strIMP z0, z1;

   //verifica se o atributo est� habilitado
   if (! PanelZ0Z1->Enabled) return;
   //determina valores de Z0/Z1 do Trafo
   z0 = clone->z0;
   z1 = clone->z1;
   //verifica se o valor deve ser mostrado em pu de Sb100MVA
   if (CBoxSbase->ItemIndex == 1)
      {//l� pot�ncia nominal
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
   //vari�veis locais
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
   //vari�veis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//l� par�metros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form sem alinhamento
   Left = 0;
   Top  = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::FormShow(TObject *Sender)
   {
   //se necess�rio, define valores default p/ Trafo
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
   //verifica se o atributo est� habilitado
   if (! PanelAutoTrafo->Enabled) return(true);
   //l� op��o de auto-trafo
   clone->auto_trafo  = checkAutoTrafo->Checked;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeCodigo(void)
   {
   //verifica se o atributo est� habilitado
   if (! PanelCodigo->Enabled) return(true);
   //l� c�digo
   clone->Codigo  = EditCodigo->Text;
   //verifica c�digo nulo
   if (clone->Codigo.IsEmpty())
      {
      //if (Confirma("C�digo n�o preenchido.", "Deseja definir o c�digo?") != IDNO) return(false);
      //FKM - 2015.10.05 poe o focus onde o usu�rio precisa editar   \\
      if (Confirma("C�digo n�o preenchido.", "Deseja definir o c�digo?") != IDNO)
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
   //verifica se o atributo est� habilitado
   if (! PanelLtcBarra->Enabled) return(true);
   //verifica se op��o de ajuste autom�tico est� selecionada
   if (CBoxLtcTipo->ItemIndex <= 0)
      {
		clone->ltc.ajuste_auto.bar_ref  = NULL;
		return(true);
      }
	//verifica se h� Barra de refer�ncia selecionada
   if (CBoxLtcBar->ItemIndex < 0)
      {
//      if (aviso_enabled) Aviso("Selecione barra de refer�ncia para ajuste autom�tico de tens�o");
//      return(false);
         //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
        if (aviso_enabled)
           {
           Aviso("Selecione barra de refer�ncia para ajuste autom�tico de tens�o");
           pgCtrl->ActivePage = TabSheetTrafo;
           CBoxLtcBar->DroppedDown = true;
           }
        return(false);
        //FKM 2015.10.06   //
      }
   //salva Barra de refer�ncia
	clone->ltc.ajuste_auto.bar_ref = (VTBarra*)(CBoxLtcBar->Items->Objects[CBoxLtcBar->ItemIndex]);
	return( clone->ltc.ajuste_auto.bar_ref != NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeLtcTipo(void)
   {
   //verifica se o atributo est� habilitado
   if (! PanelLtc->Enabled) return(true);
   //salva op��o de LTC
   switch(CBoxLtcTipo->ItemIndex)
      {
		case 0:   clone->ltc.tipo = ltcFIXO;       break;
		case 1:   clone->ltc.tipo = ltcPRIMARIO;   break;
		case 2:   clone->ltc.tipo = ltcSECUNDARIO; break;
      default:  Aviso("Selecione op��o de ajuste autom�tico"); return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeLtcVpu(void)
   {
   //verifica se o atributo est� habilitado
   if (! PanelLtcTensao->Enabled) return(true);
   try{//verifica se op��o de ajuste autom�tico est� selecionada
   if (CBoxLtcTipo->ItemIndex == 0)
      {//n�o est� utilizando ajuste autom�tico
		clone->ltc.ajuste_auto.v_pu = 1.0;
      return(true);
      }
      //l� tens�o da barra controlada
		clone->ltc.ajuste_auto.v_pu = StrToDouble(EditLtcVpu->Text);
      //verifica valor
		if (clone->ltc.ajuste_auto.v_pu < 0.8)
         {
         //if (aviso_enabled) Aviso("Tens�o da barra refer�ncia com valor abaixo de 0.8 pu");
         //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
         if (aviso_enabled)
            {
            Aviso("Tens�o da barra refer�ncia com valor abaixo de 0.8 pu");
            pgCtrl->ActivePage = TabSheetTrafo;
            EditLtcVpu->SetFocus();
            }
         //FKM 2015.10.06     //
         return(false);
         }
		else if (clone->ltc.ajuste_auto.v_pu > 1.2)
         {
         //if (aviso_enabled) Aviso("Tens�o da barra refer�ncia com valor acima de 1.2 pu");
         //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
         if (aviso_enabled)
            {
            Aviso("Tens�o da barra refer�ncia com valor acima de 1.2 pu");
            pgCtrl->ActivePage = TabSheetTrafo;
            EditLtcVpu->SetFocus();
            }
         //FKM 2015.10.06     //
         return(false);
         }
      }catch(Exception &e)
         {
         //if (aviso_enabled) Erro("Tens�o da barra refer�ncia com valor inv�lido");
         //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
         if (aviso_enabled)
            {
            Erro("Tens�o da barra refer�ncia com valor inv�lido");
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
   //verifica se o atributo est� habilitado
   if (! PanelPerdaFerro->Enabled) return(true);
   try{//l� valor (%)
      clone->perda_ferro = StrToDouble(edtPerdaFe->Text);
		if (clone->perda_ferro < 0)
         {
         //if (aviso_enabled) Aviso("Perda no ferro com valor negativo");
         //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
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
         //if (aviso_enabled) Erro("Perda no ferro com valor inv�lido");
         //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
         if (aviso_enabled)
            {
            Erro("Perda no ferro com valor inv�lido");
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
   //verifica se o atributo est� habilitado
   if (! PanelSnom->Enabled) return(true);
   try{//l� valor
      clone->snom = StrToDouble(edtSnom->Text);
      if (IsDoubleZero(clone->snom))
         {
         //if (aviso_enabled) Aviso("Pot�ncia nominal com valor nulo");
         //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
         if (aviso_enabled)
            {
            Aviso("Pot�ncia nominal com valor nulo");
            pgCtrl->ActivePage = TabSheetTrafo;
            edtSnom->SetFocus();
            }
         //FKM 2015.10.06     //
         return(false);
         }
      }catch(Exception &e)
         {
         //if (aviso_enabled) Erro("Pot�ncia nominal com valor inv�lido");
         //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
         if (aviso_enabled)
            {
            Erro("Pot�ncia nominal com valor inv�lido");
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
   //verifica se o atributo est� habilitado
   if (! PanelPriBarra->Enabled) return(true);
   //verifica se h� uma Barra selecionada
   if (CBoxPriBar->ItemIndex < 0)
      {
      //if (aviso_enabled) Aviso("Selecione barra do prim�rio");
      //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
      if (aviso_enabled)
         {
         Aviso("Selecione barra do prim�rio");
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
   //defasagem do prim�rio sempre � nula
   clone->pri.defasagem = 0;
   return true; //FKM - estava sem return...
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LePriLigacao(void)
   {
   //verifica se o atributo est� habilitado
   if (! PanelPriLigacao->Enabled) return(true);
   //l� liga��o do prim�rio
   switch(CBoxPriLiga->ItemIndex)
      {
      case 0: clone->pri.ligacao = lgEST_AT;  break;
      case 1: clone->pri.ligacao = lgTRIANG;  break;
      case 2: clone->pri.ligacao = lgEST_ISO; break;
      default:
         //if (aviso_enabled) Aviso("Selecione liga��o do prim�rio");
         //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
         if (aviso_enabled)
            {
            Aviso("Selecione liga��o do prim�rio");
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
   //vari�veis locais
   AnsiString txt;

   //verifica se o atributo est� habilitado
   if (! PanelPriTensao->Enabled) return(true);
   try{//l� tens�o nominal
      clone->pri.vnom = StrToDouble(EditPriVnom->Text);
      }catch(Exception &e)
         {
         //if (aviso_enabled) Erro("Tens�o nominal do prim�rio com valor inv�lido");
         //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
         if (aviso_enabled)
            {
            Erro("Tens�o nominal do prim�rio com valor inv�lido");
            pgCtrl->ActivePage = TabSheetPrimario;
            EditPriVnom->SetFocus();
            }
         //FKM 2015.10.06     //
         return(false);
         }
   try{//l� tens�o de tap
      clone->pri.tap = StrToDouble(EditPriVtap->Text);
      }catch(Exception &e)
         {
         //if (aviso_enabled) Erro("Tens�o de tap do prim�rio com valor inv�lido");
         //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
         if (aviso_enabled)
            {
            Erro("Tens�o de tap do prim�rio com valor inv�lido");
            pgCtrl->ActivePage = TabSheetPrimario;
            EditPriVtap->SetFocus();
            }
         //FKM 2015.10.06     //
         return(false);
         }
   //compara tens�o do trafo com tesn�o da Barra
   if (! IsDoubleZero(clone->pri.vnom - clone->pbarra1->vnom))
      {
//      if (aviso_enabled)
//         {
//         Aviso(txt.sprintf("Vnom do prim�rio(%4.3f kV) diferente da tens�o da barra (%4.3f kV)",
//                           clone->pri.vnom, clone->pbarra1->vnom));
//         }
          //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
         if (aviso_enabled)
            {
            Aviso(txt.sprintf("Vnom do prim�rio(%4.3f kV) diferente da tens�o da barra (%4.3f kV)",
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
   //verifica se o atributo est� habilitado
   if (! gboxPriZterra->Enabled) return(true);
   //verifica se o prim�rio � estrela aterrado
   if (clone->pri.ligacao == lgEST_AT)
      {
      try{//l� resist�ncia de aterramento
         clone->pri.zterra.r = StrToDouble(EditPriRat->Text);
         }catch(Exception &e)
            {
            //if (aviso_enabled) Erro("Resist�ncia de aterramento com valor inv�lido");
            //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
            if (aviso_enabled)
               {
               Erro("Resist�ncia de aterramento com valor inv�lido");
               pgCtrl->ActivePage = TabSheetPrimario;
               EditPriRat->SetFocus();
               }
            //FKM 2015.10.06     //
            return(false);
            }
      try{//l� reat�ncia de aterramento
         clone->pri.zterra.x = StrToDouble(EditPriXat->Text);
         }catch(Exception &e)
            {
            //if (aviso_enabled) Erro("Reat�ncia de aterramento com valor inv�lido");
            //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
            if (aviso_enabled)
               {
               Erro("Reat�ncia de aterramento com valor inv�lido");
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
   //verifica se o atributo est� habilitado
   if (! PanelSecBarra->Enabled) return(true);
   //verifica se h� uma Barra selecionada
   if (CBoxSecBar->ItemIndex < 0)
      {
      //if (aviso_enabled) Aviso("Selecione barra do secund�rio");
      //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
      if (aviso_enabled)
         {
         Aviso("Selecione barra do secund�rio");
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
   //verifica se o atributo est� habilitado
   if (! gboxSecDefasagem->Enabled) return(true);
   try{//l� valor do �ngulo
      clone->sec.defasagem = StrToDouble(EditSecDefasagem->Text);
      }catch(Exception &e)
         {
         if (aviso_enabled) Erro("�ngulo de defasagem do secund�rio com valor inv�lido");
         return(false);
         }
   return(true);
   }
*/
/*
//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeSecDefasagem(void)
   {
   //vari�veis locais
   int defasagem;

   //verifica se o atributo est� habilitado
   if (! gboxSecDefasagem->Enabled) return(true);
   //verifica se op��o de liga��o est� selecionada
   if (CBoxSecDefasagem->ItemIndex < 0)
      {
      //if (aviso_enabled) Aviso("Selecione a defasagem entre prim�rio/secund�rio");
      //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
      if (aviso_enabled)
         {
         Aviso("Selecione a defasagem entre prim�rio/secund�rio");
         pgCtrl->ActivePage = TabSheetSecundario;
         CBoxSecDefasagem->DroppedDown = true;
         }
      //FKM 2015.10.06     //
      return(false);
      }
   //define valor do �ngulo em fun��o da liga��o selecionada
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
   //atualiza �ngulo de defasagem
   clone->sec.defasagem = defasagem;
   return(true);
   }
*/    //DVK 2016.11.16
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/17071782
bool __fastcall TFormEdtTrf::LeSecDefasagem(void)
   {
   //vari�veis locais
   int defasagem;

   //verifica se o atributo est� habilitado
   if (! gboxSecDefasagem->Enabled) return(true);
   //verifica se op��o de liga��o est� selecionada
   if (CBoxSecDefasagem->ItemIndex < 0)
      {
      //if (aviso_enabled) Aviso("Selecione a defasagem entre prim�rio/secund�rio");
      //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
      if (aviso_enabled)
         {
         Aviso("Selecione a defasagem entre prim�rio/secund�rio");
         pgCtrl->ActivePage = TabSheetSecundario;
         CBoxSecDefasagem->DroppedDown = true;
         }
      //FKM 2015.10.06     //
      return(false);
      }
   //define valor do �ngulo em fun��o da liga��o selecionada
   if ((clone->pri.ligacao == lgTRIANG)&&(clone->sec.ligacao != lgTRIANG))
      {
      switch(CBoxSecDefasagem->ItemIndex)
         {
//         case 0:  Aviso("Defasagem inv�lida");
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
//         case 0:  Aviso("Defasagem inv�lida");
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
         case 2:  Aviso("Defasagem inv�lida");
                  return(false);
         case 0:  defasagem = 0;
                  break;
         }
      }
   //atualiza �ngulo de defasagem
   clone->sec.defasagem = defasagem;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeSecLigacao(void)
   {
   //verifica se o atributo est� habilitado
   if (! PanelSecLigacao->Enabled) return(true);
   //l� liga��o do secund�rio
   switch(CBoxSecLiga->ItemIndex)
      {
      case 0: clone->sec.ligacao = lgEST_AT;  break;
      case 1: clone->sec.ligacao = lgTRIANG;  break;
      case 2: clone->sec.ligacao = lgEST_ISO; break;
      default:
         if (aviso_enabled) Aviso("Selecione liga��o do secund�rio");
         return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtTrf::LeSecTensao(void)
   {
   //vari�veis locais
   AnsiString txt;

   //verifica se o atributo est� habilitado
   if (! PanelSecTensao->Enabled) return(true);
   try{//l� tens�o nominal
      clone->sec.vnom = StrToDouble(EditSecVnom->Text);
      }catch(Exception &e)
         {
         //if (aviso_enabled) Erro("Tens�o nominal do secund�rio com valor inv�lido");
         //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
         if (aviso_enabled)
            {
            Erro("Tens�o nominal do secund�rio com valor inv�lido");
            pgCtrl->ActivePage = TabSheetSecundario;
            EditSecVnom->SetFocus();
            }
         //FKM 2015.10.06     //
         return(false);
         }
   try{//l� tens�o de tap
      clone->sec.tap = StrToDouble(EditSecVtap->Text);
      }catch(Exception &e)
         {
         //if (aviso_enabled) Erro("Tens�o de tap do secund�rio com valor inv�lido");
         //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
         if (aviso_enabled)
            {
            Erro("Tens�o de tap do secund�rio com valor inv�lido");
            pgCtrl->ActivePage = TabSheetSecundario;
            EditSecVtap->SetFocus();
            }
         //FKM 2015.10.06     //
         return(false);
         }
   //compara tens�o do trafo com tens�o da Barra
   if (! IsDoubleZero(clone->sec.vnom - clone->pbarra2->vnom))
      {
//      if (aviso_enabled)
//         {
//         Aviso(txt.sprintf("Vnom do secund�rio(%4.3f kV) diferente da tens�o da barra (%4.3f kV)",
//                           clone->sec.vnom, clone->pbarra2->vnom));
//         }
      //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
      if (aviso_enabled)
         {
         Aviso(txt.sprintf("Vnom do secund�rio(%4.3f kV) diferente da tens�o da barra (%4.3f kV)",
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
   //verifica se o atributo est� habilitado
   if (! gboxSecZterra->Enabled) return(true);
   //verifica se o secm�rio � estrela aterrado
   if (clone->sec.ligacao == lgEST_AT)
      {
      try{//l� resist�ncia de aterramento
         clone->sec.zterra.r = StrToDouble(EditSecRat->Text);
         }catch(Exception &e)
            {
            //if (aviso_enabled) Erro("Resist�ncia de aterramento do secund�rio com valor inv�lido");
            //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
            if (aviso_enabled)
               {
               Erro("Resist�ncia de aterramento do secund�rio com valor inv�lido");
               pgCtrl->ActivePage = TabSheetSecundario;
               EditSecRat->SetFocus();
               }
            //FKM 2015.10.06     //
            return(false);
            }
      try{//l� reat�ncia de aterramento
         clone->sec.zterra.x = StrToDouble(EditSecXat->Text);
         }catch(Exception &e)
            {
            //if (aviso_enabled) Erro("Reat�ncia de aterramento do secund�rio com valor inv�lido");
            //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
            if (aviso_enabled)
               {
               Erro("Reat�ncia de aterramento do secund�rio com valor inv�lido");
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
   //verifica se o atributo est� habilitado
   if (! PanelRede->Enabled) return(true);
   //verifica se foi selecionada uma Rede
   if (CBoxRede->ItemIndex < 0)
      {
      if (aviso_enabled) Aviso("Defina a Rede do Transformador");
      //FKM 2015.10.06 - Seleciona o lugar que o usu�rio deve editar \\
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
   //vari�veis locais
   strIMP z0, z1;

   //l� valores editados de Z0/Z1
   if (! LeZ0Z1(z0, z1)) return(false);
   //verifica se o valor editado estava em pu de Sb100
   if (CBoxSbase->ItemIndex == 1)
      {//l� pot�ncia nominal
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
   //verifica se o atributo est� habilitado
   if (! PanelZ0Z1->Enabled) return(true);
   try{
      //fkm 2015.09.02 verifica��o de valores nulos
      if((StrToDouble(EditR0->Text) == 0) && (StrToDouble(EditX0->Text) == 0))
      {
         throw Exception("valores nulos de imped�ncia");
      }
      if((StrToDouble(EditR1->Text) == 0) && (StrToDouble(EditX1->Text) == 0))
      {
         throw Exception("valores nulos de imped�ncia");
      }
//      if(StrToDouble(EditR0->Text) == 0){ throw Exception("valores nulos de imped�ncia");}
//      if(StrToDouble(EditX0->Text) == 0){ throw Exception("valores nulos de imped�ncia");}
//      if(StrToDouble(EditR1->Text) == 0){ throw Exception("valores nulos de imped�ncia");}
//      if(StrToDouble(EditX1->Text) == 0){ throw Exception("valores nulos de imped�ncia");}
      //l� dados de z0 e z1 em pu
      z0.r  = StrToDouble(EditR0->Text);
      z0.x  = StrToDouble(EditX0->Text);
      z1.r  = StrToDouble(EditR1->Text);
      z1.x  = StrToDouble(EditX1->Text);
      }catch(Exception &e)
         {
         if (aviso_enabled) Erro("Valores inv�lidos de imped�ncia de sequ�ncia");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtTrf::Moldura(VTEqpto *eqpto)
   {
   //vari�veis locais
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





