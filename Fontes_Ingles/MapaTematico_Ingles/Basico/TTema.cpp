//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Math.h>
#include <Math.hpp>
#include "TTema.h"
#include "..\Apl\VTApl.h"
#include "..\Redegraf\VTCelula.h"
#include "..\Redegraf\VTCelulas.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
/*
//---------------------------------------------------------------------------
VTTema* __fastcall NewObjTema(VTApl *apl)
   {
   try{
      return (new TTema(apl));
      }catch (Exception &e)
         {
         return(NULL);
         }
   }
*/
//---------------------------------------------------------------------------
__fastcall TTema::TTema(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   //define cor default
   Color = clBlack;
   }

//---------------------------------------------------------------------------
__fastcall TTema::~TTema(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
/*
void __fastcall TTema::CalculaValorProprioCelula(void)
   {
   //método a ser implementado nas classes derivadas
   }
*/

//---------------------------------------------------------------------------
/*
void __fastcall TTema::CalculaValorTotalCelula(void)
   {
   //método a ser implementado nas classes derivadas
   }
*/
//---------------------------------------------------------------------------
void __fastcall TTema::Reinicia(int ind_pat)
   {
   //salva ind_pat
   this->ind_pat = ind_pat;
   //recalcula valor das células
   CalculaValorProprioCelula();
   //calcula valor das células que não tem valor próprio
   CalculaValorTotalCelula();
   //calcula valor porcentual das células
   NormalizaValorTotalCelula();
   //redefine cor das células
   DefineCorCelula();
   }

//---------------------------------------------------------------------------
/*
void __fastcall TTema::DefineCorCelula(void)
   {
   //método a ser implementado nas classes derivadas
   }
*/
//---------------------------------------------------------------------------
/*
TColor __fastcall  TTema::Degrade(double valor)
   {
   //variáveis locais
   unsigned r = 0xff;
   unsigned g = 0xff * valor;
   unsigned b = 0x00;

   //retorna tom de cinza
   return(TColor(r + (g << 8) + (b << 16)));
   }

//---------------------------------------------------------------------------
TColor __fastcall  TTema::DegradeCinza(double valor)
   {
   //variáveis locais
   unsigned valor_hexa = 0xff * valor;

   //retorna tom de cinza
   return(TColor(valor_hexa + (valor_hexa << 8) + (valor_hexa << 16)));
   }

//---------------------------------------------------------------------------
void __fastcall  TTema::NormalizaValorTotalCelula(void)
   {
   //variáveis locais
   double    valor_max = 0;
   VTCelula  *celula;
   VTCelulas *celulas = (VTCelulas*)apl->GetObject(__classid(VTCelulas));
   TList     *lisCEL  = celulas->LisCelula();

   //loop p/ todas celulas
   for (int n = 0; n < lisCEL->Count; n++)
      {
      celula = (VTCelula*)lisCEL->Items[n];
      valor_max = Max(valor_max, fabs(celula->ValorTotal));
      }
   //proteção
   if (IsDoubleZero(valor_max )) return;
   //calcula valor porcentual das celulas
   for (int n = 0; n < lisCEL->Count; n++)
      {
      celula = (VTCelula*)lisCEL->Items[n];
      celula->ValorTotal = celula->ValorTotal / valor_max;
      }
   }
*/
//---------------------------------------------------------------------------
/*
void __fastcall  TTema::NormalizaValorTotalCelula(void)
   {
   //método a ser implementado nas classes derivadas
   }
*/

//---------------------------------------------------------------------------
//eof
/*
sites c/ exemplos de degradê
http://www.site.com.br/wiki/index.php/Degrad%C3%AA_-_Exemplo_de_C%C3%B3digo_de_Cores
http://www.dicasblogger.com.br/2010/10/css3-efeito-gradiente-degrade.html
http://www.efeitosespeciais.net/tabela.htm
http://www.webcalc.com.br/frame.asp?pag=http://www.webcalc.com.br/utilitarios/rgb_hex.html
http://www.perlmonks.org/?node_id=204824
*/
