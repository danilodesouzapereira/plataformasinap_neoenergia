//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.hpp>
#pragma resource "*.dfm"
#include "TFormEdtCarMS.h"
#include "..\Apl\VTApl.h"
#include "..\Classes\VTClasse.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurva.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TFormEdtCarMS::TFormEdtCarMS(TComponent* Owner, VTApl *apl, VTCarga *carga, bool op_retira)
   : TForm(Owner)
   {
   //salva ponteiro p/ objeto
   this->apl    = apl;
   this->carga = carga;
   demanda     = (VTDemanda*)apl->GetObject(__classid(VTDemanda));
   //configura opção de retirar
   ActionRetira->Visible = op_retira;
   //cria objetos
   lisCARGA = new TList();
   //inicia lista com todas Cargas da rede
   IniciaLisCarga();
   //se necessário, define valores default p/ Carga
   DefValoresDefaultCarga();
   //exibe dados da Carga
   ExibeDadosCarga();
   //posiciona o Form
   FormIniciaPosicao();
   }

//---------------------------------------------------------------------------
__fastcall TFormEdtCarMS::~TFormEdtCarMS(void)
   {
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   //destrói lista sem destruir seus objetos
   if (lisCARGA) {delete lisCARGA; lisCARGA = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarMS::ActionCancelaExecute(TObject *Sender)
   {
   //cancela inserção da Carga
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarMS::ActionConfirmaExecute(TObject *Sender)
   {
   //valida dados da Carga
   if (! LeDadosCarga()) return;
   //confirma inserção da Carga
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarMS::ActionRetiraExecute(TObject *Sender)
   {
   //fecha o form indicando exclusão do Eqpto
   ModalResult = mrNo;
   }

//---------------------------------------------------------------------------
VTCurva* __fastcall TFormEdtCarMS::CriaCurva(void)
   {
   //variáveis locais
   VTCurva *curva;

   //cria Curva
   curva                = DLL_NewObjCurva();
   curva->Tipica        = false;
   curva->Tipo          = curvaPQ;
   curva->Unidade       = unidMVA;
   curva->NumeroValores = nvCURVA_PQ;
   curva->Escala        = pat01HORA;
   curva->Reconfigura(pat01HORA);
   return(curva);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarMS::DefValoresDefaultCarga(void)
   {
   //variáveis locais
   int        count = 1;
   AnsiString codigo;
   VTClasses  *classes = (VTClasses*)apl->GetObject(__classid(VTClasses));


   //verifica se a Carga ainda não definida
   if (carga->Codigo.IsEmpty())
      {//define código da Carga
      while(ExisteCarga(codigo.sprintf("Carga %d", count++)) != NULL);
      carga->Codigo = codigo;
      }
   //verifica se a Carga possui Classe
   if (carga->classe == NULL)
      {//define Classe
      carga->classe = classes->ExisteClasseTag(consEQUIVALENTE);
      }
   //verifica se a Carga ainda não tem uma Curva associada
   if (carga->curva == NULL)
      {//cria Curva própria p/ a Carga
      carga->DefineCurva(CriaCurva());
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarMS::ExibeDadosCarga(void)
   {
   //variáveis locais
   int    ind_pat = 0;
   double p_mw, q_mvar;

   //determina fator de demanda da Carga
   p_mw   = demanda->P_mw[carga][ind_pat];
   q_mvar = demanda->Q_mvar[carga][ind_pat];
   //atualiza tela: dados da Carga
   EditCodigo->Text     = carga->Codigo;
   EditPotAtiva->Text   = DoubleToStr("%4.3f", p_mw   * 1000.);
   EditPotReativa->Text = DoubleToStr("%4.3f", q_mvar * 1000.);
   EditNumCons->Text    = IntToStr(carga->NumConsTotal);
   CheckBoxVip->Checked = carga->VeryImportant;
   }

//---------------------------------------------------------------------------
VTCarga* __fastcall TFormEdtCarMS::ExisteCarga(AnsiString codigo)
   {
   //variáveis locais
   VTCarga *carga;

   for (int n = 0; n < lisCARGA->Count; n++)
      {
      carga = (VTCarga*)lisCARGA->Items[n];
      if (carga->Codigo.AnsiCompareIC(codigo) == 0) return(carga);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarMS::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //libera a memoria ocupada pelo form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarMS::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarMS::FormShow(TObject *Sender)
   {
   //posiciona o Cursor em EditCodigo
   EditCodigo->SetFocus();
   }

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarMS::IniciaLisCarga(void)
   {
   //variáveis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));

   lisCARGA->Clear();
   redes->LisEqbar(lisCARGA, eqptoCARGA);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEdtCarMS::LeDadosCarga(void)
   {
   //variáveis locais
   AnsiString codigo;
   int        num_cons;
   double     fatpot;
   double     s_mva, p_kw, q_kvar;
   VTPatamares *patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));

   try{//lê códigoda Carga
      codigo = EditCodigo->Text;
      if (codigo.IsEmpty())
         {
         Aviso("Defina o código da carga");
         return(false);
         }
      //lê potência ativa
      if (! StrToDouble(EditPotAtiva->Text, p_kw))
         {
         Aviso("Potência ativa com valor inválido");
         return(false);
         }
      //lê potência reativa
      if (! StrToDouble(EditPotReativa->Text, q_kvar))
         {
         Aviso("Potência reativa com valor inválido");
         return(false);
         }
      //lê número de consumidores
      if (! StrToInteger(EditNumCons->Text, num_cons))
         {
         Aviso("Número de consumidores com valor inválido");
         return(false);
         }
      if (num_cons <= 0)
         {
         Aviso("Número de consumidores com valor incorreto");
         return(false);
         }
      //atualiza Carga
      carga->Codigo             = codigo;
      carga->NumConsA4          = 0;
      carga->NumConsResidencial = 0;
      carga->NumConsComercial   = 0;
      carga->NumConsIndustrial  = 0;
      carga->NumConsRural       = 0;
      carga->NumConsOutros      = num_cons;
      carga->VeryImportant      = CheckBoxVip->Checked;
      //preenche curva de demanda
      carga->curva->Escala = patamares->NumPatamar();
      for (int np = 0; np < patamares->NumPatamar(); np++)
         {
         demanda->P_mw[carga][np]   = p_kw   * 0.001;
         demanda->Q_mvar[carga][np] = q_kvar * 0.001;;
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


