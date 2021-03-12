//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TFormEditaObra.h"
#include "TFormCustos.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Obra\VTObra.h"
#include "..\Obra\VTObras.h"
#include "..\..\Dll_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormEditaObra::TFormEditaObra(TComponent* Owner, VTApl *apl_owner, VTObra *obra)
   : TForm(Owner)
   {
   //salva objetos
   this->apl         = apl_owner;
   this->obra_ref     = obra;
   //preenche dados da Obra
   if(obra_ref->Nome.IsEmpty())     EditNome->Text = NomeProximaObra();
   else                             EditNome->Text = obra_ref->Nome;
   DateTimePickerInicio->Date   = obra_ref->DataInicio;
   DateTimePickerAtivacao->Date = obra_ref->DataAtivacao;
   EditCusto->Text              = DoubleToStr("%3.2f", obra_ref->Custo);
   //indica na Obra que o usuário cancelou a edição (default)
   obra_ref->ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
__fastcall TFormEditaObra::~TFormEditaObra(void)
   {
   //destrói objetos
   //if (apl != NULL) {delete apl; apl = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormEditaObra::ActionCancelaExecute(TObject *Sender)
   {
   //fecha o Form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEditaObra::ActionConfirmaExecute(TObject *Sender)
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
   //lê valor de custo
   if (! GetCusto(custo)) return;
   //atualiza Obra
   obra_ref->Nome         = GetNome();
   obra_ref->DataInicio   = GetDataInicio();
   obra_ref->DataAtivacao = GetDataAtivacao();
   obra_ref->Custo        = custo;
   obra_ref->Responsavel  = "";
   obra_ref->ObEspecial   = 0.;
   //indica na Obra que o usuário confirmou a edição
   obra_ref->ModalResult = mrOk;
   //fecha o Form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEditaObra::ActionSelObraTipicaExecute(TObject *Sender)
   {
   //variáveis locais
   TFormCustos *FormCustos;

   //esconde este Form
   Hide();
   //seleciona arquivo de obras típicas
   if (SelecionaArquivoObraTipica())
      {//exibe FormCustos p/ usuário selecionar uma obra/custo
      FormCustos = new TFormCustos(this, apl);
      FormCustos->LeArquivoObrasCustos(OpenDialog->FileName);
      if (FormCustos->ShowModal() == mrOk)
         {//lê dados da ObraTipica selecionada
         if (EditNome->Text.IsEmpty()) EditNome->Text = FormCustos->ObraCodigo();
         EditCusto->Text = DoubleToStr("%3.2f", FormCustos->ObraCusto());
         }
      //destrói FormCustos
      delete FormCustos;
      }
   //exibe este Form
   Show();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEditaObra::ExisteObra(TList *lisOBRA, AnsiString nome)
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
void __fastcall TFormEditaObra::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormEditaObra::FormShow(TObject *Sender)
   {
   //define campo c/ o focus
   EditNome->SetFocus();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEditaObra::GetCusto(double &custo)
   {
   try{
      custo = StrToDouble(EditCusto->Text);
      }catch(Exception &e)
         {
         Erro("Custo com valor inválido");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
TDateTime __fastcall TFormEditaObra::GetDataAtivacao(void)
   {
   return(DateOf(DateTimePickerAtivacao->Date));
   }

//---------------------------------------------------------------------------
TDateTime __fastcall TFormEditaObra::GetDataInicio(void)
   {
   return(DateOf(DateTimePickerInicio->Date));
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormEditaObra::GetNome(void)
   {
   return(EditNome->Text.Trim());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormEditaObra::NomeProximaObra(void)
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
bool __fastcall TFormEditaObra::SelecionaArquivoObraTipica(void)
   {
   //variáveis locais
   VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //verifica se o arquivo de já foi definido
   if (! OpenDialog->FileName.IsEmpty()) return(true);
   //configura diretório inicial
   if (OpenDialog->InitialDir.IsEmpty()) OpenDialog->InitialDir = path->DirImporta();
   //exibe OpenDialog p/ usuário selecionar arquivo
   return(OpenDialog->Execute());
   }

//---------------------------------------------------------------------------
bool __fastcall TFormEditaObra::ValidaDataInicio(void)
   {
   //variáveis locais
   int       ind_obra;
   VTObra    *obra_ant, *obra_pos;
   TList     *lisOBRA     = obra_ref->Obras->LisObra();
   TDateTime  data        = DateTimePickerInicio->Date;
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
