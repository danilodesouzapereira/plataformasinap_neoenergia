//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "TFormCfgTensao.h"
#include "TMedicao.h"
#include "TRes505.h"
#include <DLL_INC\Funcao.h>
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFormCfgTensao::TFormCfgTensao(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //salva ponteiro
   this->apl = apl;
   //salva os ponteiros
   res     = (TRes505*)  apl->GetObject(__classid(TRes505));
   //medicao = (TMedicao*) apl->GetObject(__classid(TMedicao));
   //inicia valores
   IniciaValores();
   confirma    = true;
   }

//---------------------------------------------------------------------------
__fastcall TFormCfgTensao::~TFormCfgTensao(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTensao::ActionCancelarExecute(TObject *Sender)
   {
   //cancela e sair
   confirma    = false;
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTensao::ActionConfirmarExecute(TObject *Sender)
   {
   //le e valida os valores
   if(! SalvaRes()) return;
   confirma    = false;
   //salvar e sair
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTensao::editAdeq1Exit(TObject *Sender)
   {
   //variáveis locais
   double valor_pu;

   //copia valores
   editPrecSup1->Text = editAdeq1->Text;
   //converte para pu
   valor_pu = adeq1/res->Vnom_fn_kv;
   Label_prec_sup1->Caption = DoubleToStr("%3.2f", valor_pu, ',') + " pu";
   Label_adeq1->Caption = DoubleToStr("%3.2f", valor_pu, ',') + " pu";
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTensao::editAdeq2Exit(TObject *Sender)
   {
   //variáveis locais
   double valor_pu;

   //copia valores
   editPrecInf2->Text = editAdeq2->Text;
   //converte para pu
   valor_pu = adeq2/res->Vnom_fn_kv;
   Label_prec_inf2->Caption = DoubleToStr("%3.2f", valor_pu, ',') + " pu";
   Label_adeq2->Caption = DoubleToStr("%3.2f", valor_pu, ',') + " pu";
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTensao::editCrit1Exit(TObject *Sender)
   {
   //variáveis locais
   double valor_pu;

   //copia valores
   editPrecInf1->Text = editCrit1->Text;
   //converte para pu
   valor_pu = crit1/res->Vnom_fn_kv;
   Label_prec_inf1->Caption = DoubleToStr("%3.2f", valor_pu, ',') + " pu";
   Label_crit1->Caption = DoubleToStr("%3.2f", valor_pu, ',') + " pu";
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTensao::editCrit2Exit(TObject *Sender)
   {
   //variáveis locais
   double valor_pu;

   //copia valores
   editPrecSup2->Text = editCrit2->Text;
   //converte para pu
   valor_pu = crit2/res->Vnom_fn_kv;
   Label_prec_sup2->Caption = DoubleToStr("%3.2f", valor_pu, ',') + " pu";
   Label_crit2->Caption = DoubleToStr("%3.2f", valor_pu, ',') + " pu";
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTensao::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //nada
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTensao::FormCloseQuery(TObject *Sender, bool &CanClose)
   {
   //variável local
   int resp = IDNO;

   if(! confirma) {CanClose = true; return;}
   //verifica se deve salvar
   resp = Confirma("Deseja salvar os valores definidos ?", "", MB_YESNOCANCEL);
   switch(resp)
      {
      case IDYES:
         //atualiza valores no cfg
         if(! SalvaRes())  CanClose = false;
         else               CanClose = true;
         return;
      case IDNO:
         //volta valores originais sem salvar
         CanClose = true;
         return;
      case IDCANCEL: //não salva a rede
      default:
         CanClose = false;
         return;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTensao::FormShow(TObject *Sender)
   {
   //nada
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTensao::IniciaCboxMedicao(void)
   {
   AnsiString vfn[] = {"Van (fase-neutro)", "Vbn (fase-neutro)", "Vcn (fase-neutro)"};
   AnsiString vff[] = {"Vab (fase-fase)",   "Vbc (fase-fase)",   "Vca (fase-fase)"};

   cboxV1->Items->Clear();
   cboxV2->Items->Clear();
   cboxV3->Items->Clear();

   //fase neutro
   if(radiobutFN->Checked)
      {
      for(int n=0; n<3; n++)
         {
         cboxV1->Items->Add(vfn[n]);
         cboxV2->Items->Add(vfn[n]);
         cboxV3->Items->Add(vfn[n]);
         }
      }
   //fase-fase
   else
      {
      for(int n=0; n<3; n++)
         {
         cboxV1->Items->Add(vff[n]);
         cboxV2->Items->Add(vff[n]);
         cboxV3->Items->Add(vff[n]);
         }
      }
   //atualiza indice
   cboxV1->ItemIndex = 0;
   cboxV2->ItemIndex = 1;
   cboxV3->ItemIndex = 2;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTensao::IniciaValores(void)
{
   //ligação
   /*
   if(res->Tensao == eFN)
      {
      radiobutFN->Checked  = true;
      radiobutFF->Checked  = false;
      }
   else
      {
      radiobutFN->Checked  = false;
      radiobutFF->Checked  = true;
      }  */
   radiobutFN->Checked =  (res->Tensao == eFN);
   radiobutFF->Checked = !(res->Tensao == eFN);
   //tensão nominal
   editVff->Text = DoubleToStr("%f", res->Vnom_ff_kv*1000.,',');
   editVfn->Text = DoubleToStr("%f", res->Vnom_fn_kv*1000.,',');

   //faixa adequada
   editAdeq1->Text = DoubleToStr("%f", res->adequado.inf*1000.,',');
   editAdeq2->Text = DoubleToStr("%f", res->adequado.sup*1000.,',');
   //faixa precaria
   editPrecInf1->Text = DoubleToStr("%f", res->precario1.inf*1000.,',');
   editPrecSup1->Text = DoubleToStr("%f", res->precario1.sup*1000.,',');
   editPrecInf2->Text = DoubleToStr("%f", res->precario2.inf*1000.,',');
   editPrecSup2->Text = DoubleToStr("%f", res->precario2.sup*1000.,',');
   //faixa critica
   editCrit1->Text = DoubleToStr("%f", res->critico.inf*1000.,',');
   editCrit2->Text = DoubleToStr("%f", res->critico.sup*1000.,',');
   //valores locais (em V)
   adeq1 = res->adequado.inf;
   adeq2 = res->adequado.sup;
   crit1 = res->critico.inf;
   crit2 = res->critico.sup;
   //valores em pu
   editAdeq1Exit(NULL);
   editAdeq2Exit(NULL);
   editCrit1Exit(NULL);
   editCrit2Exit(NULL);

   IniciaCboxMedicao();
   }
/*
//---------------------------------------------------------------------------
bool __fastcall TFormCfgTensao::LeValores(void)
   {
   //variaveis locais
   double vnom, v1, v2;

   if(cboxV1->ItemIndex < 0 ||
      cboxV2->ItemIndex < 0 ||
      cboxV3->ItemIndex < 0)
      {
      Aviso("Defina pelo menos uma fase com medição de tensão !");
      return false;
      }
   if(cboxV1->ItemIndex == cboxV2->ItemIndex ||
      cboxV2->ItemIndex == cboxV3->ItemIndex ||
      cboxV3->ItemIndex == cboxV1->ItemIndex)
      {
      Aviso("Existe mais de uma medição de tensão para a mesma fase!");
      return false;
      }

   try{
       //medicao->v1 = cboxV1->ItemIndex;
       //medicao->v2 = cboxV2->ItemIndex;
       //medicao->v3 = cboxV3->ItemIndex;

      //tensão nominal
      res->Vnom_fn_kv = fabs(StrToDouble(editVfn->Text))/1000.0;
      res->Vnom_ff_kv = fabs(StrToDouble(editVff->Text))/1000.0;

      //ligação
      if(radiobutFN->Checked)
         {
         res->Tensao = eFN;
         vnom = res->Vnom_fn_kv;
         }
      else
         {
         res->Tensao = eFF;
         vnom = res->Vnom_ff_kv;
         }
      //faixa adequada
      v1 = fabs(StrToDouble(editAdeq1->Text))/1000.0;
      v2 = fabs(StrToDouble(editAdeq2->Text))/1000.0;
      if(vnom < v1 || vnom > v2)
         {
         Aviso("Faixa adequada inválida !");
         return false;
         }
      res->adequado.inf = v1;
      res->adequado.sup = v2;

      //faixa critica
      v1 = fabs(StrToDouble(editCrit1->Text))/1000.0;
      v2 = fabs(StrToDouble(editCrit2->Text))/1000.0;

      if(v1 >= res->adequado.inf || v2 <= res->adequado.sup)
         {
         Aviso("Faixa crítica inválida !");
         return false;
         }
      res->critico.inf = v1;
      res->critico.sup = v2;

      //faixa precaria
      res->precario1.inf = res->critico.inf;
      res->precario1.sup = res->adequado.inf;
      res->precario2.inf = res->adequado.sup;
      res->precario2.sup = res->critico.sup;

      //atualiza tensão nominal
      //res->vnom = vnom;
   }   catch(...)
      {
      return false;
      }
   return true;
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormCfgTensao::radiobutFNClick(TObject *Sender)
   {
   IniciaCboxMedicao();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgTensao::radiobutFFClick(TObject *Sender)
   {
   IniciaCboxMedicao();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgTensao::SalvaRes(void)
   {
   //salva no infoset
   if(! SalvaDados())  return(false);
   //adequada (em kV)
   res->adequado.inf = adeq1/1000.;
   res->adequado.sup = adeq2/1000.;
   //critica (em kV)
   res->critico.inf = crit1/1000.;
   res->critico.sup = crit2/1000.;
   //precaria (em kV)
   res->precario1.inf = res->critico.inf;
   res->precario1.sup = res->adequado.inf;
   res->precario2.inf = res->adequado.sup;
   res->precario2.sup = res->critico.sup;
   //tensões fase-fase e fase-neutro
   res->Vnom_ff_kv = vff_v/1000.;
   res->Vnom_fn_kv = vfn_v/1000.;
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgTensao::SalvaDados(void)
   {
   //variáveis locais
   VTGeral        *geral        = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //valida antes de salvar
   if(! ValidaDados()) return(false);
   //salva informações no infoset
   if(! geral) return(false);
   try{//guarda valores em kV
      geral->Infoset->Clear();
      geral->Infoset->AddInfo("Adeq1", adeq1/1000.);
      geral->Infoset->AddInfo("Adeq2", adeq2/1000.);
      geral->Infoset->AddInfo("Crit1", crit1/1000.);
      geral->Infoset->AddInfo("Crit2", crit2/1000.);
      geral->Infoset->SaveToFile(res->Descricao);
   }catch(Exception &e)
      {
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgTensao::ValidaDados(void)
   {
   try{//adequado
      if (! StrToDouble(editAdeq1->Text, adeq1))
         {
         Aviso("Valor inválido do limite inferior adequado");
         return false;
         }
      if (! StrToDouble(editAdeq2->Text, adeq2))
         {
         Aviso("Valor inválido do limite superior adequado");
         return false;
         }
      if (adeq1 > adeq2)
         {
         Aviso("Limite inferior adequado maior que limite superior adequado");
         return false;
         }
      //critico
      if (! StrToDouble(editCrit1->Text, crit1))
         {
         Aviso("Valor inválido do limite inferior crítico");
         return false;
         }
      if (! StrToDouble(editCrit2->Text, crit2))
         {
         Aviso("Valor inválido do limite superior crítico");
         return false;
         }
      if (crit1 > crit2)
         {
         Aviso("Limite inferior crítico maior que limite superior crítico");
         return false;
         }
      //valida valores
      if(crit1 > adeq1)
         {
         Aviso("Crítico inferior maior que adequado inferior");
         return false;
         }
      if(crit2 < adeq2)
         {
         Aviso("Crítico superior menor que adequado superior");
         return false;
         }
      //tensões fase-fase e fase-neutro
      if(! StrToDouble(editVff->Text, vff_v))
         {
         Aviso("Tensão fase-fase inválida");
         return false;
         }
      if(! StrToDouble(editVfn->Text, vfn_v))
         {
         Aviso("Tensão fase-neutro inválida");
         return false;
         }
   }catch(Exception &e)
      {
      Aviso("Valor inválido !");
      return false;
      }
   return true;
   }

//---------------------------------------------------------------------------

