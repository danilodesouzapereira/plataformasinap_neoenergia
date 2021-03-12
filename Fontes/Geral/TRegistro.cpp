//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TRegistro.h"
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTRegistro* __fastcall NewObjRegistro(void)
	{
   try{//cria objeto
      return(new TRegistro());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TRegistro::TRegistro(void)
   {
   //cria objetos
   lisINFO = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TRegistro::~TRegistro(void)
   {
   //destrói lisINFO e seus objetos
   if (lisINFO) {Clear(); delete lisINFO; lisINFO = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TRegistro::AddInfo(AnsiString nome, AnsiString valor)
   {
   //variáveis locais
   TInfo *info;

   try{//verifica se já existe objeto Info
      if ((info = ExisteInfo(nome)) == NULL)
         {//cria objeto TInfo
         info       = new TInfo();
         info->nome = nome;
         //insere Info em lisINFO
         lisINFO->Add(info);
         }
      info->valor = valor;
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TRegistro::AddInfo(AnsiString nome, double valor)
   {
   //variáveis locais
   AnsiString str_valor = DoubleToStr("%9.8f", valor);

   return(AddInfo(nome, str_valor));
   }

//---------------------------------------------------------------------------
bool __fastcall TRegistro::AddInfo(AnsiString nome, float valor)
   {
   //variáveis locais
   AnsiString str_valor = DoubleToStr("%9.8f", valor);

   return(AddInfo(nome, str_valor));
   }

//---------------------------------------------------------------------------
bool __fastcall TRegistro::AddInfo(AnsiString nome, int valor)
   {
   //variáveis locais
   AnsiString str_valor = IntToStr(valor);

   return(AddInfo(nome, str_valor));
   }

//---------------------------------------------------------------------------
bool __fastcall TRegistro::AddInfo(AnsiString nome, unsigned valor)
   {
   //variáveis locais
	AnsiString str_valor = IntToStr(int(valor));

   return(AddInfo(nome, str_valor));
   }
//---------------------------------------------------------------------------
void __fastcall TRegistro::Clear(void)
   {
   //destrói objsto TInfo em lisINFO
   for (int n = 0; n < lisINFO->Count; n++)
      {
      delete((TInfo*)(lisINFO->Items[n]));
      }
   lisINFO->Clear();
   }

//---------------------------------------------------------------------------
TRegistro::TInfo* __fastcall TRegistro::ExisteInfo(AnsiString nome)
   {
   //variáveis locais
   TInfo *info;

   //procura objeto TInfo pelo nome
   for (int n = 0; n < lisINFO->Count; n++)
      {
      info = (TInfo*)(lisINFO->Items[n]);
      if (info->nome.AnsiCompareIC(nome) == 0) return(info);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TRegistro::GetAllInfoAsString(AnsiString &valor)
   {
   //variáveis locais
   TInfo *info;

   try{//monta valor com todos os dados dos objtos Info
   valor = "";
      for (int n = 0; n < lisINFO->Count; n++)
         {
         info = (TInfo*)(lisINFO->Items[n]);
         valor += info->nome + "=" + info->valor + ";";
         }
      return(true);
      }catch(Exception &e)
         {//nada a fazer
         return(false);
         }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TRegistro::GetInfo(AnsiString nome, AnsiString &valor)
   {
   //variáveis locais
   TInfo *info;

   try{//procuro objeto TInfo pelo nome
      for (int n = 0; n < lisINFO->Count; n++)
         {
         info = (TInfo*)(lisINFO->Items[n]);
         if (info->nome.AnsiCompareIC(nome) == 0)
            {
            valor = info->valor;
            return(true);
            }
         }
      }catch(Exception &e)
         {//nada a fazer
         return(false);
         }
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TRegistro::GetInfo(AnsiString nome, double &valor)
   {
   //variáveis locais
   AnsiString str_val;

   try{//obtém valor como AnsiString
      if (! GetInfo(nome, str_val)) return(false);
      valor = StrToDouble(str_val);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TRegistro::GetInfo(AnsiString nome, float &valor)
   {
   //variáveis locais
   AnsiString str_val;

   try{//obtém valor como AnsiString
      if (! GetInfo(nome, str_val)) return(false);
      valor = StrToDouble(str_val);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TRegistro::GetInfo(AnsiString nome, int &valor)
   {
   //variáveis locais
   AnsiString str_val;

   try{//obtém valor como AnsiString
      if (! GetInfo(nome, str_val)) return(false);
      valor = StrToInteger(str_val);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TRegistro::GetInfo(AnsiString nome, unsigned &valor)
   {
   //variáveis locais
   AnsiString str_val;

   try{//obtém valor como AnsiString
      if (! GetInfo(nome, str_val)) return(false);
      valor = StrToInteger(str_val);
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
//eof
