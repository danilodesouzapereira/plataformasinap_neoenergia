//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormSuporte.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Arranjo\VTPtoFixacao.h"
#include "..\Arranjo\VTSuporte.h"
#include "..\Rede\VTFases.h"
#include "..\..\DLL_Inc\Arranjo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormSuporte::TFormSuporte(TComponent* Owner, VTApl *apl,VTSuporte *suporte)
                        :TForm(Owner)
   {
   //salva ponteiro p/ objetos
   this->apl          = apl;
   this->suporte_orig = suporte;
   //cria cópia do objeto
   this->suporte = suporte_orig->Clone();     
   //DVK 2014.07.31 esconde níveis de tensão
   LabelVmin->Visible = false;
   LabelVmax->Visible = false;
   EditVmin->Visible  = false;
   EditVmax->Visible  = false;  //DVK 2014.10.27
   //DVK 2014.07.31 redefine localizações
   RGroupLocal->Items->Clear();
   RGroupLocal->Items->Add("Aéreo");
   RGroupLocal->Items->Add("Subterrâneo"); 
   }

//---------------------------------------------------------------------------
__fastcall TFormSuporte::~TFormSuporte(void)
   {
   //destrói objetos
   if (suporte) {delete suporte; suporte = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormSuporte::ActionAltNumPtoExecute(TObject *Sender)
   {
   //atualiza SGrid de PtoFixacao
   ExibePtoFixacao();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSuporte::ActionCancelaExecute(TObject *Sender)
   {
   //fecha o form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSuporte::ActionConfirmaExecute(TObject *Sender)
   {
   //lê os dados editados
   if (! LeDados()) return;
   //atualiza status indicando que o Suporte foi alterado
   suporte->Status[sttALTERADO] = true;
   //atualiza Suporte original
   suporte_orig->CopiaAtributosDe(*suporte);
   //fecha o form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSuporte::AjustaNumPtoFixacao(int num_pto)
   {
   VTPtoFixacao *pto_fixacao;
   TList        *lisPTO = suporte->LisPtoFixacao();

   //se necessário, cria novos PtoFixacao
   while (lisPTO->Count < num_pto)
      {//cria novos PtoFixacao
      lisPTO->Add(DLL_NewObjPtoFixacao());
      }
   //se necessário, destrói PtoFixacao excedente
   while (lisPTO->Count > num_pto)
      {//destrói PtoFixacao excedente
      pto_fixacao = (VTPtoFixacao*)lisPTO->Last();
      lisPTO->Delete(lisPTO->Count - 1);
      delete pto_fixacao;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormSuporte::ExibeDados(void)
   {
   //variáveis locais
   int   num_pto;
   TList *lisPTO = suporte->LisPtoFixacao();

   //código
   EditCodigo->Text = suporte->Codigo;
   //vmin e vmax
   EditVmin->Text   = DoubleToStr("%4.3f", suporte->Vmin_KV);
   EditVmax->Text   = DoubleToStr("%4.3f", suporte->Vmax_KV);
   //tipo de suporte
   switch(suporte->TipoDeSuporte)
      {
      case suporteAEREO:       RGroupLocal->ItemIndex = 0; break;
      case suporteSUBTERRANEO: RGroupLocal->ItemIndex = 1; break;
      case suporteSUBMERSO:    RGroupLocal->ItemIndex = 2; break;
      default:                 RGroupLocal->ItemIndex = 0; break;
      }
   //número de PtoFixacao
   num_pto = lisPTO->Count;
   CBoxNumPto->ItemIndex = CBoxNumPto->Items->IndexOf(IntToStr(num_pto));
   if (CBoxNumPto->ItemIndex < 0) CBoxNumPto->ItemIndex = 0;
   //atualiza SGrid de PtoFixacao
   ExibePtoFixacao();
   }

//---------------------------------------------------------------------------
void __fastcall TFormSuporte::ExibePtoFixacao(void)
   {
   //variáveis locais
   int          num_pto;
   VTPtoFixacao *pto_fixacao;
   TList        *lisPTO = suporte->LisPtoFixacao();

   //determina número de PtoFixacao selecionado
   num_pto = StrToInteger(CBoxNumPto->Text);
   if (num_pto == 0) num_pto = MAX_FASE;
   //reinicia SGrid
   SGridPto->RowCount = num_pto + 1;
   SGridPto->ColCount = 3;
   //redimensiona SGridPto
   if (num_pto <= 12)
      {SGridPto->Height = SGridPto->RowCount * (SGridPto->DefaultRowHeight + 2);}
   else
      {SGridPto->Height = 13 * (SGridPto->DefaultRowHeight + 2);}
   //limpa SGrid
   for (int nr = 0; nr < SGridPto->RowCount; nr++) SGridPto->Rows[nr]->Clear();
   //define títulos das colunas
   SGridPto->Cells[0][0] = "Código";
   SGridPto->Cells[1][0] = "x(m)";
   SGridPto->Cells[2][0] = "y(m)";
   //preenche SGrid c/ os PtoFixacao existentes
   for (int n = 0; (n < num_pto)&&(n < lisPTO->Count); n++)
      {
      pto_fixacao = (VTPtoFixacao*)lisPTO->Items[n];
      SGridPto->Cells[0][n+1] = pto_fixacao->codigo;
      SGridPto->Cells[1][n+1] = DoubleToStr("%4.3f", pto_fixacao->coord.x);
      SGridPto->Cells[2][n+1] = DoubleToStr("%4.3f", pto_fixacao->coord.y);
      }
   }

//---------------------------------------------------------------------------
VTSuporte* __fastcall TFormSuporte::ExisteSuporte(AnsiString codigo)
   {
   //variáveis locais
   VTSuporte  *um_suporte;
   VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   TList      *lisSUP   = arranjos->LisSuporte();

   //procura Suporte c/ o código indicado
   for (int n = 0; n < lisSUP->Count; n++)
      {
      um_suporte = (VTSuporte*)lisSUP->Items[n];
      //desconsidera o Suporte sendo editado
      if (um_suporte == suporte_orig) continue;
      if (um_suporte->Codigo.AnsiCompareIC(codigo) == 0) return(um_suporte);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormSuporte::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormSuporte::FormShow(TObject *Sender)
   {
   //exibe dados do Suporte
   ExibeDados();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormSuporte::LeDados(void)
   {
   //variáveis locais
   int num_pto;

   try{//Código
      suporte->Codigo = EditCodigo->Text;
      if (suporte->Codigo.IsEmpty())
         {
         Erro("Defina o código do suporte");
         return(false);
         }
      //verifica se existe outro Suporte c/ mesmo código
      if (ExisteSuporte(suporte->Codigo) != NULL)
         {
         Erro("Existe outro suporte com o código definido");
         return(false);
         }
      //tensões mínima e máxima
      suporte->Vmin_KV = StrToDouble(EditVmin->Text);
      suporte->Vmax_KV = StrToDouble(EditVmax->Text);
      if (suporte->Vmin_KV < 0) suporte->Vmin_KV = 0;
      if (suporte->Vmax_KV < 0) suporte->Vmax_KV = 0;
      if (suporte->Vmin_KV > suporte->Vmax_KV)
         {
         Erro("Tensão mínima do suporte é maior que a tensão máxima");
         return(false);
         }
      //tipo de local
      switch(RGroupLocal->ItemIndex)
         {
         case 0:  suporte->TipoDeSuporte = suporteAEREO;       break;
         case 1:  suporte->TipoDeSuporte = suporteSUBTERRANEO; break;
         case 2:  suporte->TipoDeSuporte = suporteSUBMERSO;    break;
         default: suporte->TipoDeSuporte = suporteAEREO;       break;
         }
      //número de PtoFixacao
      if (CBoxNumPto->ItemIndex < 0)
         {
         Erro("Defina o número de pontos de fixação do suporte");
         return(false);
         }
      //dados dos PtoFixacao
      num_pto = StrToInteger(CBoxNumPto->Text);
      //se necessário, cria ou destrói PtoFixacao
      AjustaNumPtoFixacao(num_pto);
      if (! LeDadosPtoFixacao(num_pto)) return(false);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormSuporte::LeDadosPtoFixacao(int num_pto)
   {
   VTPtoFixacao *pto_fixacao, *pto_fixacao_j;
   TList        *lisPTO = suporte->LisPtoFixacao();

   //verifica se é necessário criar ou destruir PtoFixacao
   if ((SGridPto->RowCount-1) != lisPTO->Count) return(false);
   //lê dados dos PtoFixacao do SGrid
   for (int n = 0; n < lisPTO->Count; n++)
      {
      pto_fixacao = (VTPtoFixacao*)lisPTO->Items[n];
      pto_fixacao->codigo  = SGridPto->Cells[0][n+1];
      pto_fixacao->coord.x = StrToDouble(SGridPto->Cells[1][n+1]);
      pto_fixacao->coord.y = StrToDouble(SGridPto->Cells[2][n+1]);
      //valida os dados
      if (pto_fixacao->codigo.IsEmpty())
         {
         Erro("Ponto de fixação com código não definido");
         return(false);
         }
      if ((pto_fixacao->coord.x == 0.)&&(pto_fixacao->coord.y == 0))
         {
         Erro("Ponto de fixação com coordenadas nulas");
         return(false);
         }
      }
   //verifica se há PtoFixacao c/ código repetido
   for (int i = 0; i < lisPTO->Count-1; i++)
      {
      pto_fixacao = (VTPtoFixacao*)lisPTO->Items[i];
      for (int j = i+1; j < lisPTO->Count; j++)
         {
         pto_fixacao_j = (VTPtoFixacao*)lisPTO->Items[j];
         if (pto_fixacao->codigo.AnsiCompareIC(pto_fixacao_j->codigo) == 0)
            {
            Erro("Ponto de fixação c/ código repetido: " + pto_fixacao->codigo);
            return(false);
            }
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

