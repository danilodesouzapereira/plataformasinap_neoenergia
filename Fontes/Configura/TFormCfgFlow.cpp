//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCfgFlow.h"
#include "..\Sisplan\Funcao.h"
#include "..\DLL_Stub\Basico.h"
#include "..\BDado\VTBDado.h"
#include "..\Sisplan\DirArq.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
//função global
//---------------------------------------------------------------------------
__fastcall TformCfgFlow::TformCfgFlow(TComponent* Owner, AnsiString dirBase, AnsiString dirRede)
   : TForm(Owner)
   {

   //cria objeto BDado
   bdado = DLL_NewObjBDado();
   bdado->DefDiretorio(dirBase, dirRede);
   //cria objeto Geral
   geral = bdado->LeGeral();
   //recebe valor do geral
   FLOW=geral->FLOW;

   }

//---------------------------------------------------------------------------
__fastcall TformCfgFlow::~TformCfgFlow(void)
   {

   //destrói objetos criados
   if (geral) {delete geral; geral = NULL;}
   if (bdado) {delete bdado; bdado = NULL;}

   }

//---------------------------------------------------------------------------
void __fastcall TformCfgFlow::ActionCancelaExecute(TObject *Sender)
   {

   ModalResult = mrCancel;

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgFlow::ActionConfirmaExecute(TObject *Sender)
   {

   //lê e valida dados editados
   if (! LeDados()) return;
   //atualiza base de dados
   bdado->GravaGeral(geral);
   //fecha o form
   ModalResult = mrOk;

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgFlow::ActionDefaultExecute(TObject *Sender)
   {

   //precisão
   FLOW.ITERMAX = 20;
   FLOW.TOLER   = 0.00001;
   //carga
   FLOW.POTCON = 0.;
   FLOW.CORCON = 1.;
   FLOW.IMPCON = 0.;
   //intervalos de Tensão
   FLOW.V[0] = 0.96;
   FLOW.V[1] = 0.98;
   FLOW.V[2] = 1.02;
   FLOW.V[3] = 1.04;
   //intervalos de Corrente
   FLOW.I[0] = 0.7;
   FLOW.I[1] = 1.0;
   //atualiza tela
   ExibeDados();

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgFlow::ExibeDados(void)
   {

   //precisão
   edtIterMax->Text = IntToStr(FLOW.ITERMAX);
   edtToler->Text   = FloatToStr(FLOW.TOLER);
   //carga: soma .0001 para evitar problema de truncamento
   edtPotCon->Text = IntToStr((int)((FLOW.POTCON + .0001) * 100));
   edtCorCon->Text = IntToStr((int)((FLOW.CORCON + .0001) * 100));
   edtImpCon->Text = IntToStr((int)((FLOW.IMPCON + .0001) * 100));
   //faixa A de Tensão
   edtV1->Text     = DoubleToStr("%5.3f", FLOW.V[1]);
   edtV2->Text     = DoubleToStr("%5.3f", FLOW.V[2]);
   //faixa B de Tensão
   edtV3->Text     = DoubleToStr("%5.3f", FLOW.V[0]);
   edtV4->Text     = DoubleToStr("%5.3f", FLOW.V[3]);
   //faixa A de Corrente
   edtI1->Text     = IntToStr((int)((FLOW.I[0] + .0001) * 100));
   edtI2->Text     = IntToStr((int)((FLOW.I[1] + .0001) * 100));
   //cores para tensões
   panCorV1->Color = geral->COR.Faixa[diagRUIM];
   panCorV2->Color = geral->COR.Faixa[diagREGULAR];
   panCorV3->Color = geral->COR.Faixa[diagBOM];
   panCorV4->Color = geral->COR.Faixa[diagREGULAR];
   panCorV5->Color = geral->COR.Faixa[diagRUIM];
   //cores para correntes
   panCorI1->Color = geral->COR.Faixa[diagBOM];
   panCorI2->Color = geral->COR.Faixa[diagREGULAR];
   panCorI3->Color = geral->COR.Faixa[diagRUIM];
   //labels de valores intermediários
   ActionExibeLblExecute(NULL);

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgFlow::ActionExibeLblExecute(TObject *Sender)
   {
   //atualiza labels de corrente
   lblI1->Caption = "abaixo de " + edtI1->Text;
   lblI2->Caption = "entre "     + edtI1->Text + " e " + edtI2->Text;
   lblI3->Caption = "acima de " + edtI2->Text;
   //atualiza labels de tensão
   lblV1->Caption = "abaixo de " + edtV3->Text;
   lblV2->Caption = "entre "     + edtV3->Text + " e " + edtV1->Text;
   lblV3->Caption = "entre "     + edtV1->Text + " e " + edtV2->Text;
   lblV4->Caption = "entre "     + edtV2->Text + " e " + edtV4->Text;
   lblV5->Caption = "acima de "  + edtV4->Text;
   }
//---------------------------------------------------------------------------
void __fastcall TformCfgFlow::FormClose(TObject *Sender, TCloseAction &Action)
   {

   //destrói o form
   Action = caFree;

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgFlow::FormShow(TObject *Sender)
   {
   //apresenta dados na tela
   ExibeDados();
   }

//---------------------------------------------------------------------------
bool __fastcall TformCfgFlow::LeDados(void)
   {
   //variáveis locais
   int soma;

   try
   {
   //precisão
   FLOW.ITERMAX = edtIterMax->Text.ToInt();
   FLOW.TOLER   = StrToDouble(edtToler->Text);
   //cargas
   FLOW.POTCON  = (int)(StrToDouble(edtPotCon->Text) + .001);
   FLOW.CORCON  = (int)(StrToDouble(edtCorCon->Text) + .001);
   FLOW.IMPCON  = (int)(StrToDouble(edtImpCon->Text) + .001);
   soma = (int)(FLOW.POTCON + FLOW.CORCON + FLOW.IMPCON + .001);
   if (soma != 100)
      {
      edtPotCon->Text = IntToStr((int)(FLOW.POTCON));
      edtCorCon->Text = IntToStr((int)(FLOW.CORCON));
      edtImpCon->Text = IntToStr((int)(FLOW.IMPCON));
      Erro("Fluxo de Potência: composição das cargas não soma 100%");
      return(false);
      }
   //converte valores para porcentagem
   FLOW.POTCON  /= 100;
   FLOW.CORCON  /= 100;
   FLOW.IMPCON  /= 100;
   //faixa A de Tensão
   FLOW.V[1] = StrToDouble(edtV1->Text);
   FLOW.V[2] = StrToDouble(edtV2->Text);
   //faixa B de Tensão
   FLOW.V[0] = StrToDouble(edtV3->Text);
   FLOW.V[3] = StrToDouble(edtV4->Text);
   //intervalos de Corrente
   FLOW.I[0] = StrToDouble(edtI1->Text)/100;
   FLOW.I[1] = StrToDouble(edtI2->Text)/100;
   if ((FLOW.V[0] < 0)||(FLOW.V[0] > FLOW.V[1])||
       (FLOW.V[1] > FLOW.V[2])||(FLOW.V[2] > FLOW.V[3]))
      {
      Erro("Fluxo de Potência: Intervalo de tensão inválido");
      return(false);
      }
   if ((FLOW.I[0] < 0)||(FLOW.I[0] > FLOW.I[1]))
      {
      Erro("Fluxo de Potência: Intervalo de corrente inválido");
      return(false);
      }
   } catch (EConvertError &exception)
      {
      Erro("Fluxo de Potência: valor não numérico");
      return(false);
      }
   //leu os dados corretamente: atualiza objeto geral
   geral->FLOW = FLOW;

    return(true);
   }
//---------------------------------------------------------------------------
