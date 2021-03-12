//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DLL_Inc\Funcao.h>
#include <Fontes\Rede\VTBarra.h>
#include <Fontes\RedeCC\ResCurto.h>
#include "TFormCfgDefeito.h"
#include "TDefeitos.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCfgDefeito::TFormCfgDefeito(TComponent* Owner, TDefeitos *defeitos, TDefeito *defeito)
   : TForm(Owner)
   {
   //salva ponteiro para objetos
   this->defeitos = defeitos;
   this->defeito  = defeito;
   //exibe código ou ID da Barra
   ExibeCodigoBarra();
   //exibe tipo de defeito
   ExibeTipoDefeito();
   }

//---------------------------------------------------------------------------
__fastcall TFormCfgDefeito::~TFormCfgDefeito(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgDefeito::ActionCancelaExecute(TObject *Sender)
   {
   //elimina o Defeito
   defeitos->RetiraDefeito(defeito);
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgDefeito::ActionConfirmaExecute(TObject *Sender)
   {
   //verifica se foi selecionado um defeito
   if (RadioGroupTipoDefeito->ItemIndex < 0)
      {
      Aviso("Selecione o tipo de defeito");
      return;
      }
   //salva tipo de defeito selecionado
   switch(RadioGroupTipoDefeito->ItemIndex)
      {
      case  0: defeito->tipodefeito = defeito3F;  defeito->fases = faseABC; break;
      case  1: defeito->tipodefeito = defeitoFT;  defeito->fases = faseAT;  break;
      case  2: defeito->tipodefeito = defeitoFT;  defeito->fases = faseBT;  break;
      case  3: defeito->tipodefeito = defeitoFT;  defeito->fases = faseCT;  break;
      case  4: defeito->tipodefeito = defeitoFTZ; defeito->fases = faseAT;  break;
      case  5: defeito->tipodefeito = defeitoFTZ; defeito->fases = faseBT;  break;
      case  6: defeito->tipodefeito = defeitoFTZ; defeito->fases = faseCT;  break;
      case  7: defeito->tipodefeito = defeito2F;  defeito->fases = faseAB;  break;
      case  8: defeito->tipodefeito = defeito2F;  defeito->fases = faseBC;  break;
      case  9: defeito->tipodefeito = defeito2F;  defeito->fases = faseCA;  break;
      case 10: defeito->tipodefeito = defeito2FT; defeito->fases = faseABT; break;
      case 11: defeito->tipodefeito = defeito2FT; defeito->fases = faseBCT; break;
      case 12: defeito->tipodefeito = defeito2FT; defeito->fases = faseCAT; break;
      }
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgDefeito::ActionRetiraExecute(TObject *Sender)
   {
   //elimina o Defeito
   defeitos->RetiraDefeito(defeito);
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgDefeito::ExibeCodigoBarra(void)
   {
   if (defeito->barra->Codigo.IsEmpty())
      {
      EditBarra->Text = "ID= " + IntToStr(defeito->barra->Id);
      }
   else
      {
      EditBarra->Text = defeito->barra->Codigo;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgDefeito::ExibeTipoDefeito(void)
   {
   switch(defeito->tipodefeito)
      {
      case defeito3F:
         RadioGroupTipoDefeito->ItemIndex = 0;
         break;
      case defeitoFT:
         switch(defeito->fases)
            {
            case faseA: RadioGroupTipoDefeito->ItemIndex = 1; break;
            case faseB: RadioGroupTipoDefeito->ItemIndex = 2; break;
            case faseC: RadioGroupTipoDefeito->ItemIndex = 3; break;
            default:    RadioGroupTipoDefeito->ItemIndex = 1; break;
            }
         break;
      case defeitoFTZ:
         switch(defeito->fases)
            {
            case faseA: RadioGroupTipoDefeito->ItemIndex = 4; break;
            case faseB: RadioGroupTipoDefeito->ItemIndex = 5; break;
            case faseC: RadioGroupTipoDefeito->ItemIndex = 6; break;
            default:    RadioGroupTipoDefeito->ItemIndex = 4; break;
            }
      case defeito2F:
         switch(defeito->fases)
            {
            case faseAB: RadioGroupTipoDefeito->ItemIndex = 7; break;
            case faseBC: RadioGroupTipoDefeito->ItemIndex = 8; break;
            case faseCA: RadioGroupTipoDefeito->ItemIndex = 9; break;
            default:     RadioGroupTipoDefeito->ItemIndex = 7; break;
            }
         break;
     case defeito2FT:
         switch(defeito->fases)
            {
            case faseAB: RadioGroupTipoDefeito->ItemIndex = 10; break;
            case faseBC: RadioGroupTipoDefeito->ItemIndex = 11; break;
            case faseCA: RadioGroupTipoDefeito->ItemIndex = 12; break;
            default:     RadioGroupTipoDefeito->ItemIndex = 10; break;
            }
         break;
       default: //assume defeito trifásico
         RadioGroupTipoDefeito->ItemIndex = 0;
         break;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgDefeito::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o Form
   Action = caHide;
   }

//---------------------------------------------------------------------------
//eof

