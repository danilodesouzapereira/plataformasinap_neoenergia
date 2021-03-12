// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormEdtMotorMS.h"
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Rede\VTMotor.h"
#include "..\..\DLL_Inc\Funcao.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFormEdtMotorMS::TFormEdtMotorMS(TComponent* Owner, VTApl *apl, VTMotor* motor)
                           : TForm(Owner)
   {
   // salva ponteiros p/ objetos
   this->apl = apl;
   this->motor = motor;
   // posiciona o Form
   FormIniciaPosicao();
   ExibeDadosMotor();
   }

// ---------------------------------------------------------------------------
__fastcall TFormEdtMotorMS::~TFormEdtMotorMS(void)
   {
   // variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   // salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   }

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotorMS::ActionCancelaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotorMS::ActionConfirmaExecute(TObject *Sender)
   {
   // lê dados do Eqpto
   if (! LeDadosMotor()) return;
   ModalResult = mrOk;
   }

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotorMS::ActionRetiraExecute(TObject *Sender)
   {
   // fecha o form indicando exclusão do Eqpto
   ModalResult = mrNo;
   }

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotorMS::ExibeDadosMotor(void)
   {
   // proteção: verifica se o Motor está incluído
   if (motor == NULL) return;
   // lê dados do Motor
   EditCodigo->Text     = motor->Codigo;
   EditCorNominal->Text = DoubleToStr("%2.1f", motor->inom_A);
   EditCorPartida->Text = DoubleToStr("%2.1f", motor->ipart_A);
   EditFatPot->Text     = DoubleToStr("%4.3f", motor->fpot100_pu);
   }

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotorMS::FormIniciaPosicao(void)
   {
   // variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)  geral->Infoset->LoadFromFile(this);
   }

// ---------------------------------------------------------------------------// ---------------------------------------------------------------------------
bool __fastcall TFormEdtMotorMS::LeDadosMotor(void)
   {
   //variáveis locais
   double cor_nominal, cor_partida, fat_pot;

   // proteção: verifica se o Motor está incluído
   if (motor == NULL) return (true);
   try{// lê dados do Motor
      if (EditCodigo->Text.IsEmpty())
         {
         Aviso("Defina o código do motor");
         return(false);
         }
      if (! StrToDouble(EditCorNominal->Text, cor_nominal))
         {
         Aviso("Corrente nominal com valor inválido");
         return(false);
         }
      if (! StrToDouble(EditCorPartida->Text, cor_partida))
         {
         Aviso("Corrente de partida com valor inválido");
         return(false);
         }
      if (! StrToDouble(EditFatPot->Text, fat_pot))
         {
         Aviso("Fator de potência com valor inválido");
         return(false);
         }
      //verifica corrente nominal nula
      if (cor_nominal <= 0)
         {
         Aviso("Corrente nominal não pode se nula ou negativa");
         return(false);
         }
      //verifica se corrente de partida é maior que corrente nominal
      if (cor_partida <= cor_nominal)
         {
         Aviso("Corrente de partida deve ser maior que corrente nominal");
         return(false);
         }
      //verifica intervalo do fator de potência
      if ((fat_pot < 0.) || (fat_pot > 1.))
         {
         Aviso("Fator de potência com valor inválido");
         return(false);
         }
      //atualiza motor
      motor->Codigo     = EditCodigo->Text;
      motor->inom_A     = cor_nominal;
      motor->ipart_A    = cor_partida;
      motor->fpot100_pu = fat_pot;
      } catch (Exception &e)
         {
         Erro("Dados Inválidos do Motor");
         return (false);
         }
   return (true);
   }

// ---------------------------------------------------------------------------
void __fastcall TFormEdtMotorMS::OpcaoRetira(bool enabled)
   {
   butRetira->Visible = enabled;
   }

// ---------------------------------------------------------------------------
//eof
