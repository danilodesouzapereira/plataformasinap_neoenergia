//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "TFormTUSD.h"
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Geral\VTGeral.h>
#include <Fontes\Geral\VTInfoset.h>
#include <DLL_Inc\Funcao.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormTUSD::TFormTUSD(TComponent* Owner, VTApl *apl_owner)
   : TForm(Owner)
   {
   //salva ponteiro
   this->apl = apl_owner;
   //inicia
   LimpaSGrid();
   PreencheSGrid();
   LeSGrid();
   confirma = true;
   }

//---------------------------------------------------------------------------
__fastcall TFormTUSD::~TFormTUSD(void)
   {
   //nada
   }

//---------------------------------------------------------------------------
void __fastcall TFormTUSD::ActionCancelaExecute(TObject *Sender)
   {
   //fecha sem salvar
   confirma = false;
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormTUSD::ActionConfirmaExecute(TObject *Sender)
   {
   //verifica se alguma opção do RadioGroup foi selecionada
   if(RadioGroupEUSD->ItemIndex < 0)
      {
      Aviso("Selecione uma opção para EUSD do consumidor reclamante.");
      return;
      }
   //salva e fecha
   if(! SalvaSGrid()) return;
   confirma = false;
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormTUSD::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //fecha
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormTUSD::FormCloseQuery(TObject *Sender, bool &CanClose)
   {
   //variáveis locais
   int resp = IDNO;

   //verifica se os valores devem ser salvos
   if(! confirma) {CanClose = true; return;}
   resp = Confirma("Deseja salvar os valores de TUSD ?", "", MB_YESNOCANCEL);
   switch(resp)
      {
      case IDYES: //salva
         CanClose = SalvaSGrid();
         return;
      case IDNO: //não salva
         CanClose = true;
         return;
      case IDCANCEL: //não faz nada
         CanClose = false;
         return;
      default:
         CanClose = false;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormTUSD::FormShow(TObject *Sender)
   {
   //nada
   }

//---------------------------------------------------------------------------
void __fastcall TFormTUSD::LeSGrid(void)
   {
   //variáveis locais
   VTGeral       *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
   AnsiString  valor;
   double      valor_d;

   //lê parâmetros do infoset
   if (! geral) return;
   if (geral->Infoset->LoadFromFile("ValoresTUSD"))
      {
      geral->Infoset->GetInfo("A4", valor);
      SGridTUSD->Cells[1][0] = valor;
      geral->Infoset->GetInfo("Residencial", valor);
      SGridTUSD->Cells[1][1] = valor;
      geral->Infoset->GetInfo("IP",          valor);
      SGridTUSD->Cells[1][2] = valor;
      geral->Infoset->GetInfo("Rural",       valor);
      SGridTUSD->Cells[1][3] = valor;
      geral->Infoset->GetInfo("Outros",      valor);
      SGridTUSD->Cells[1][4] = valor;
      //lê EUSD
      geral->Infoset->GetInfo("EUSD",        valor);
      valor_d = StrToDouble(valor);
      EditEUSD->Text = valor;
      if(IsDoubleZero(valor_d)) RadioGroupEUSD->ItemIndex = 0;
      else                      RadioGroupEUSD->ItemIndex = 1;
      }
   else
      {//insere valores default
      SGridTUSD->Cells[1][0] = 0.20002;
      SGridTUSD->Cells[1][1] = 0.17096;
      SGridTUSD->Cells[1][2] = 0.09634;
      SGridTUSD->Cells[1][3] = 0.11967;
      SGridTUSD->Cells[1][4] = 0.17266;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormTUSD::LimpaSGrid(void)
   {
   //apaga valores
   for(int nl = 0; nl < SGridTUSD->RowCount; nl++)
      {
      SGridTUSD->Cells[1][nl] = "";
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormTUSD::PreencheSGrid(void)
   {
   //insere classes de consumidores
   SGridTUSD->Cells[0][0] = "A4";
   SGridTUSD->Cells[0][1] = "Residencial";
   SGridTUSD->Cells[0][2] = "IP";
   SGridTUSD->Cells[0][3] = "Rural";
   SGridTUSD->Cells[0][4] = "Outros";
   }

//---------------------------------------------------------------------------
bool __fastcall TFormTUSD::SalvaSGrid(void)
   {
   //variáveis locais
   VTGeral       *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));
   AnsiString    classe, valor_str;
   double        valor;

   try{//salva valores no infoset
      if (! geral) return(false);
      for(int nl = 0; nl < SGridTUSD->RowCount; nl++)
         {
         classe    = SGridTUSD->Cells[0][nl];
         valor_str = SGridTUSD->Cells[1][nl];
         //testa se o valor é válido
         if(! StrToDouble(valor_str, valor))
            {
            Aviso("Valor '" + valor_str + "' inválido");
            return(false);
            }
         if(IsDoubleZero(valor))
            {
            Aviso("Classe '" + classe + "' com valor nulo");
            return(false);
            }
         //define valor com 5 casas decimais
         valor_str = DoubleToStr("%6.5f", valor);
         geral->Infoset->AddInfo(classe, valor_str);
         }
      //define opção de EUSD
      if(RadioGroupEUSD->ItemIndex == 1)
         {
         valor_str = EditEUSD->Text;
         if(! StrToDouble(valor_str, valor))
            {
            Aviso("Valor '" + valor_str + "' inválido");
            return(false);
            }
         if(IsDoubleZero(valor))
            {
            Aviso("Valor do EUSD nulo");
            return(false);
            }
         }
      else valor_str = "0";
      geral->Infoset->AddInfo("EUSD", valor_str);
      //salva no arquivo
      geral->Infoset->SaveToFile("ValoresTUSD");
      }catch(Exception &e)
         {
         Aviso("Valor '" + valor_str + "' inválido");
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------

