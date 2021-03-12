//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TDado.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTDado* __fastcall NewObjDado(AnsiString descricao)
   {
   try{
      return (new TDado(descricao));
      }catch (Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TDado::TDado(AnsiString descricao)
   {
   //salva descrição
   this->descricao = descricao;
   }

//---------------------------------------------------------------------------
__fastcall TDado::~TDado(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TDado::DescUnid(void)
	{
	if (descricao.IsEmpty())
		{
		if (unidade.IsEmpty()) return("");
		return("(" + unidade + "):");
		}
	if (unidade.IsEmpty()) return(descricao + ":");
	return(descricao + "(" + unidade + "):");
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TDado::DescUnidValor(void)
	{
	AnsiString txt;

	//if (unidade.IsEmpty()) return(descricao + ": " + valor);
	//return(descricao + "(" + unidade + "): " + valor);
	txt = DescUnid();
	if (txt.IsEmpty()) return(valor);
	return(txt  + ": " + valor);
	}

//---------------------------------------------------------------------------
AnsiString __fastcall TDado::DescValorUnid(void)
	{
	if (descricao.IsEmpty()) return(ValorUnid());
	return(descricao + ": " + ValorUnid());
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TDado::ValorUnid(void)
   {
   return(valor + unidade);
   }
   
//---------------------------------------------------------------------------
//eof
