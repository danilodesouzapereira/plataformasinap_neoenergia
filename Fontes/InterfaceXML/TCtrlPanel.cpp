//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TCtrlPanel.h"
#include "..\Registro\VTRegistro.h"
#include "..\..\DLL_Inc\Registro.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
class VTSuporte;

//---------------------------------------------------------------------------
VTCtrlPanel* NewObjCtrlPanel(void)
   {
   return(new TCtrlPanel());
   }

//---------------------------------------------------------------------------
__fastcall TCtrlPanel::TCtrlPanel(void)
   {
   //cria objetos
   registro = DLL_NewObjRegistro("Control Panel");
   //salva separador decimal configurado no Windows
   registro->ExisteInfo("International", "sDecimal", ctrlpanel.dec_separator);
   }

//---------------------------------------------------------------------------
__fastcall TCtrlPanel::~TCtrlPanel(void)
   {
   //restaura separador decimal originalmente configurado no Windows
   RestauraSeperadorDecimal();
   //destrói objetos
   delete registro;
   }

//---------------------------------------------------------------------------
bool __fastcall TCtrlPanel::DefineSeperadorDecimal(AnsiString dec_separator)
   {
   //verifica se o separador decimal configurado no Windows já está correto
   if (ValidaSeparadorDecimal(dec_separator)) return(true);
   //redefine  separador decimal configurado no Windows
   if (! registro->GravaInfo("International", "sDecimal", dec_separator)) return(false);
   //verifica se a alteração foi realmente efetuada
   return(ValidaSeparadorDecimal(dec_separator));
   }


//---------------------------------------------------------------------------
bool __fastcall TCtrlPanel::RestauraSeperadorDecimal(void)
   {
   //verifica se o separador decimal atual é diferente da configuração original
   if (ValidaSeparadorDecimal(ctrlpanel.dec_separator)) return(true);
   //restaura  separador decimal original do Windows
   return(registro->GravaInfo("International", "sDecimal", ctrlpanel.dec_separator));
   }

//---------------------------------------------------------------------------
bool __fastcall TCtrlPanel::ValidaSeparadorDecimal(AnsiString dec_separator)
   {
   //variáveis locais
   AnsiString curr_separator;

   //lê separador decimal configurado no Windows
   if (! registro->ExisteInfo("International", "sDecimal", curr_separator)) return(false);
   //verifica se o separador atual é diferente do indicado
   return (dec_separator == curr_separator);
   }

//-----------------------------------------------------------------------------
// eof


