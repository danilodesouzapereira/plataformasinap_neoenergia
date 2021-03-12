//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#pragma resource "*.dfm"
#include "TFormEdtSupMS.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTFornecimento.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TFormEdtSupMS::TFormEdtSupMS(TComponent* Owner, VTApl *apl, VTSuprimento *suprimento, bool op_retira)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl        = apl;
   this->suprimento = suprimento;
   //configura opção de retirar
   ActionRetira->Visible = op_retira;
   //cria objetos
   lisEQP = new TList();
   //inicia lista com todas Suprimentos da rede
   IniciaLisSuprimento();
   //se necessário, define valores default p/ Suprimento
   DefValoresDefaultSuprimento();
   //exibe dados da Suprimento
   ExibeDadosSuprimento();
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtSupMS::~TFormEdtSupMS(void)
   {
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destrói lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSupMS::ActionCancelaExecute(TObject *Sender)
   {
   //cancela inserção da Suprimento
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSupMS::ActionConfirmaExecute(TObject *Sender)
   {
   //valida dados da Suprimento
   if (! LeDadosSuprimento()) return;
   //confirma inserção da Suprimento
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSupMS::ActionRetiraExecute(TObject *Sender)
   {
   //fecha o form indicando exclusão do Eqpto
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TFormEdtSupMS::CriaCurva(void)
   {
   //variáveis locais
   strHM   hm_ini, hm_fim;
   double  valor[nvCURVA_PQVT];
   VTCurva *curva;

   //cria Curva
   curva                = DLL_NewObjCurva();
   curva->Tipica        = false;
   curva->Tipo          = curvaPQVT;
   curva->Unidade       = unidMVA;
   curva->NumeroValores = nvCURVA_PQVT;
   curva->Escala        = patEDITAVEL;
   //inicia tensão de suprimento c/ 1 pu
   curva->GetPonto(0, hm_ini, hm_fim, valor, nvCURVA_PQVT);
   valor[indP] = 0.;
   valor[indQ] = 0.;
   valor[indV] = 1.;
   valor[indT] = 0.;
   curva->SetPonto(hm_ini, hm_fim, valor, nvCURVA_PQVT);
   //redefine Curva com 24 pontos
   curva->Reconfigura(pat01HORA);
   return(curva);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSupMS::DefValoresDefaultSuprimento(void)
   {
   //variáveis locais
   int        count = 1;
   AnsiString codigo;


   //verifica se a Suprimento ainda não definida
   if (suprimento->Codigo.IsEmpty())
      {//define código da Suprimento
      while(ExisteSuprimento(codigo.sprintf("Suprimento %d", count++)) != NULL);
      suprimento->Codigo = codigo;
      //define demais atributos
      suprimento->smax = 100.;
      suprimento->vnom = suprimento->pbarra->vnom;
      //define Imp.Eqv
      suprimento->zeq1.x = suprimento->zeq0.x = 0.05;
      }
   //verifica se a Suprimento ainda não tem uma Curva associada
   if (suprimento->curva == NULL)
      {//cria Curva própria p/ a Suprimento
      suprimento->DefineCurva(CriaCurva());
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSupMS::ExibeDadosSuprimento(void)
   {
   //variáveis locais
   strHM   hm_ini, hm_fim;
   double  valor[nvCURVA_PQVT];

   //determina valores do Suprimento
   suprimento->curva->GetPonto(0, hm_ini, hm_fim, valor, nvCURVA_PQVT);
   //atualiza tela: dados da Suprimento
   EditCodigo->Text = suprimento->Codigo;
   EditVpu->Text    = DoubleToStr("%4.3f", valor[indV]);
   }

//---------------------------------------------------------------------------
VTSuprimento* __fastcall TFormEdtSupMS::ExisteSuprimento(AnsiString codigo)
   {
   //variáveis locais
   VTSuprimento *suprimento;

   for (int n = 0; n < lisEQP->Count; n++)
      {
      suprimento = (VTSuprimento*)lisEQP->Items[n];
      if (suprimento->Codigo.AnsiCompareIC(codigo) == 0) return(suprimento);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSupMS::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //libera a memoria ocupada pelo form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSupMS::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSupMS::FormShow(TObject *Sender)
   {
   //posiciona o Cursor em EditCodigo
   EditCodigo->SetFocus();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtSupMS::IniciaLisSuprimento(void)
   {
   //variáveis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoSUPRIMENTO);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtSupMS::LeDadosSuprimento(void)
   {
   //variáveis locais
   AnsiString codigo;
   double     v_pu;
   VTFornecimento *fornecimento = (VTFornecimento*)apl->GetObject(__classid(VTFornecimento));
   VTPatamares    *patamares    = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   try{//lê dados da Suprimento
      codigo = EditCodigo->Text;
      if (codigo.IsEmpty())
         {
         Aviso("Defina o código da suprimento");
         return(false);
         }
      /*
      if (ExisteSuprimento(codigo) != NULL)
         {
         Aviso("Existe outra suprimento com o mesmo código");
         return(false);
         }
      */
      if (! StrToDouble(EditVpu->Text, v_pu))
         {
         Aviso("Tensão de operação com valor inválido");
         return(false);
         }
      if (v_pu < 0.)
         {
         Aviso("Tensão de operação com valor incorreto");
         return(false);
         }
      //atualiza Suprimento
      suprimento->Codigo = codigo;
      suprimento->curva->Escala = patamares->NumPatamar();
      for (int np = 0; np < patamares->NumPatamar(); np++)
         {
         fornecimento->V_pu[suprimento][np]  = v_pu;
         }
      }catch(Exception &e)
         {
         Erro("Valor numérico c/ caracteres inválidos");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
//eof


