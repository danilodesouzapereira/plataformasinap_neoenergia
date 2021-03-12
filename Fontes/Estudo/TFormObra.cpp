//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TFormObra.h"
//#include "VTCustos.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include "..\..\Dll_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormObra::TFormObra(TComponent* Owner, VTApl *apl_owner, VTObra *obra)
   : TForm(Owner)
   {
   //salva objetos
   this->apl         = apl_owner;
   this->obra_ref     = obra;
   //preenche dados da Obra
   if(obra_ref->Nome.IsEmpty()) EditNome->Text = NomeProximaObra();
   else                         EditNome->Text = obra_ref->Nome;
   DateTimePickerInicio->Date   = obra_ref->DataInicio;
   DateTimePickerAtivacao->Date = obra_ref->DataAtivacao;
   if (obra_ref->AlteraTaxaFalha)
      {
      CheckBoxQualidade->Checked = true;
      EditTaxaInsercao->Text     = DoubleToStr("%2.1f", obra_ref->TaxaFalhaInsercao);
      EditTaxaSubstituicao->Text = DoubleToStr("%2.1f", obra_ref->TaxaFalhaSubstituicao);
      }
   else
      {
      obra_ref->AlteraTaxaFalha  = false;
      EditTaxaInsercao->Text     = "";
      EditTaxaSubstituicao->Text = "";
      }
   }

//---------------------------------------------------------------------------
__fastcall TFormObra::~TFormObra(void)
   {
   //destrói objetos
   //if (apl != NULL) {delete apl; apl = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormObra::ActionCancelaExecute(TObject *Sender)
   {
   //fecha o Form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormObra::ActionConfirmaExecute(TObject *Sender)
   {
   //variáveis locais
   double custo, ob_esp;
   
   //valida nome da obra
   if (EditNome->Text.IsEmpty())
      {
      Aviso("Defina o nome da obra");
      return;
      }
   //valida data selecionada
   if (! ValidaDataInicio()) return;
   //atualiza Obra
   obra_ref->Nome         = GetNome();
   obra_ref->DataInicio   = GetDataInicio();
   obra_ref->DataAtivacao = GetDataAtivacao();
   obra_ref->Responsavel  = "";
   obra_ref->ObEspecial   = 0.;
   //taxas de falha
   if (CheckBoxQualidade->Checked)
      {
      obra_ref->AlteraTaxaFalha       = true;
      obra_ref->TaxaFalhaInsercao     = GetValorDouble(EditTaxaInsercao,     "Taxa falha de inserção de eqpto");
      obra_ref->TaxaFalhaSubstituicao = GetValorDouble(EditTaxaSubstituicao, "Taxa falha de substituição de eqpto");
      }
   else
      {
      obra_ref->AlteraTaxaFalha       = false;
      obra_ref->TaxaFalhaInsercao     = 0.;
      obra_ref->TaxaFalhaSubstituicao = 0.;
      }
   //fecha o Form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormObra::CheckBoxQualidadeClick(TObject *Sender)
   {
   //exibe/esconde GBoxTaxaFalha
   GBoxTaxaFalha->Visible = CheckBoxQualidade->Checked;
   }

//---------------------------------------------------------------------------
bool __fastcall TFormObra::ExisteObra(TList *lisOBRA, AnsiString nome)
   {
   //variáveis locais
   VTObra *obra;

   for (int n = 0; n < lisOBRA->Count; n++)
      {
      obra = (VTObra*)lisOBRA->Items[n];
      if (nome.AnsiCompareIC(obra->Nome) == 0) return(true);
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFormObra::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormObra::FormShow(TObject *Sender)
   {
   //define campo c/ o focus
   EditNome->SetFocus();
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormObra::NomeProximaObra(void)
   {
   //variáveis locais
   int        count = 1;
   AnsiString nome;

   do {//define nome
      nome.sprintf("Obra %02d", count++);
      }while (ExisteObra(obra_ref->Obras->LisObra(), nome));

   return(nome);
   }

//---------------------------------------------------------------------------
TDateTime __fastcall TFormObra::GetDataAtivacao(void)
   {
   return(DateOf(DateTimePickerAtivacao->Date));
   }

//---------------------------------------------------------------------------
TDateTime __fastcall TFormObra::GetDataInicio(void)
   {
   return(DateOf(DateTimePickerInicio->Date));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormObra::GetNome(void)
   {
   return(EditNome->Text.Trim());
   }

//---------------------------------------------------------------------------
double __fastcall TFormObra::GetValorDouble(TEdit *Edit, AnsiString descricao)
   {
   try{
      return(StrToDouble(Edit->Text));
      }catch(Exception &e)
         {
         Erro(descricao + " com valor inválido");
         return(0.);
         }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormObra::ValidaDataInicio(void)
   {
   //variáveis locais
   int       ind_obra;
   VTObra    *obra_ant, *obra_pos;
   TList     *lisOBRA    = obra_ref->Obras->LisObra();
   TDateTime  data       = DateTimePickerInicio->Date;
   TDateTime  data_atual = DateOf(Now());

   //verifica se data atual é posterior à data inicial da Alternativa
   if (data_atual > obra_ref->Obras->DataInicial)
      {//assume data_atual igual à data inicial da Alternativa
      data_atual = obra_ref->Obras->DataInicial;
      }
   //compara data com data_atual
   if (data < data_atual)
      {
      Aviso("Data de início da obra não pode ser anterior a " + DateToStr(data_atual));
      return(false);
      }
   //verifica se a Obra é nova
   if (obra_ref == NULL)
      {//Obra nova: compara com data final da Alternativa
      if (data < obra_ref->Obras->DataFinal)
         {
         Aviso("Data de início da obra não pode ser anterior a " + DateToStr(obra_ref->Obras->DataFinal));
         return(false);
         }
      return(true);
      }
   //Obra existente: determina índice da Obra na Alternativa
   if ((ind_obra = lisOBRA->IndexOf(obra_ref)) < 0) return(false);
   //verifica datas das Obras anteriores
   for (int n = 0; n < ind_obra; n++)
      {
      obra_ant = (VTObra*)lisOBRA->Items[n];
      if (data < obra_ant->DataInicio)
         {
         Aviso("Data de início anterior à obra " + AnsiQuotedStr(obra_ant->Nome, '\''));
         return(false);
         }
      }
   //verifica datas das Obras posteriore
   for (int n = ind_obra+1; n < lisOBRA->Count; n++)
      {
      obra_pos = (VTObra*)lisOBRA->Items[n];
      if (data > obra_pos->DataInicio)
         {
         Aviso("Data de início posterior à obra " + AnsiQuotedStr(obra_pos->Nome, '\''));
         return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

