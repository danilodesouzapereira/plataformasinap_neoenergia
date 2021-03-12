//-----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TAlerta.h"
#include "TFormAlerta.h"
#include "..\Rede\VTRede.h"
#include "..\..\DLL_Inc\Funcao.h"
//-----------------------------------------------------------------------------
VTAlerta* NewObjAlerta(VTApl *apl_owner)
   {
   try{
      return(new TAlerta(apl_owner));
      }catch(Exception &e)
         {//nada a fazer
         }
   //erro na criação do objeto
   return(NULL);
   }

//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422540
__fastcall TAlerta::TAlerta(VTApl *apl_owner)
   {
   //salva ponteiro p/ objeto
   apl = apl_owner;
   //cria objetos
   lines = new TStringList();
   lines_aux = new TStringList();
   //define número máximo de mensagenas
   PD.length = 200;
   }

//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422540
__fastcall TAlerta::~TAlerta(void)
   {
   //destrói objetos
   if (lines) {delete lines;  lines  = NULL;}
   if (lines_aux) {delete lines_aux;  lines_aux  = NULL;}
   mapMsgMalha.clear();
   }

//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422540
int __fastcall TAlerta::Add(AnsiString msg)
   {
   //FKM 2015.10.08 - Timestamp
   TDateTime dateTime;
   AnsiString timeStamp;
   AnsiString msg_aux;

   timeStamp = "" +Date().DateString() +" " +Time().TimeString() +" ";
   msg_aux = timeStamp + msg;
   //FKM 2015.10.08 //
   //não insere msg duplicada
   if (lines->IndexOf(msg_aux) < 0)
      {//insere texto no fim da
	  lines->Add(msg_aux);
      //insere na lista auxiliar a msg sem o timestamp
	  lines_aux->Add(msg);
      //limita número de linhas
      while (lines->Count > PD.length) lines->Delete(0);
      }
   return(lines->Count);
   }
//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/16258668
int  __fastcall  TAlerta::AddMsgMalha(VTRede *rede, AnsiString msg)
	{
	if(rede != NULL)
		{mapMsgMalha.insert(std::pair<int, AnsiString>(rede->Id,msg));}
	return(Add(msg));
	}
//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422540
void __fastcall TAlerta::Clear(bool completo)
	{
	lines->Clear();
	lines_aux->Clear();
	if (completo)
		{
		mapMsgMalha.clear();
		}
	}
//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14422540
AnsiString __fastcall TAlerta::ExisteAvisoMalha(VTRede *rede)
{
	std::map<int,AnsiString>::iterator it;

    //protecao
	if (rede == NULL)
		{return "";}

	it = mapMsgMalha.find(rede->Id);
	if (it != mapMsgMalha.end())
	{
		return it->second;
	}

	return "";
}
//-----------------------------------------------------------------------------
int __fastcall TAlerta::NumeroDeMensagens(void)
   {
	return(lines->Count);
   }
//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/14569628
void __fastcall  TAlerta::ReexibeAlertas(bool somenteMalhas)
{
	TStrings *linesAux = new TStringList();


	if (somenteMalhas)
	{
		for (std::map<int,AnsiString>::iterator it=mapMsgMalha.begin(); it!=mapMsgMalha.end(); ++it)
		{
			Alerta(it->second);
        }
	}
	else
	{
		if(lines->Count == 0)
		{
            for (std::map<int,AnsiString>::iterator it=mapMsgMalha.begin(); it!=mapMsgMalha.end(); ++it)
			{
				lines->Add(it->second);
			}
		}
		linesAux->Assign(lines_aux);
		//limpa o atual e refaz
        this->Clear();
		//reexibe todos
		for (int ni = 0; ni < linesAux->Count; ni++)
		{
			Alerta(linesAux->Strings[ni]);
		}
	}

	delete linesAux;
}
//-----------------------------------------------------------------------------
void __fastcall TAlerta::SaveToFile(AnsiString filename)
   {
	lines->SaveToFile(filename);
	}

//-----------------------------------------------------------------------------
void __fastcall TAlerta::ShowModal(void)
   {
   //variáveis locais
   TFormAlerta *FormAlerta;

   //proteção:
   if (lines->Count == 0) return;
   //cria FormAlerta
   FormAlerta = new TFormAlerta(NULL, apl);
   //exibe texto do log
   FormAlerta->ExibeTexto(lines);
   //exibe arquivo como janela modal
	FormAlerta->ShowModal();
	if (FormAlerta->RichEdit->Lines->Count == 0)
		{//usuário descartou as mesnagens
		lines->Clear();
		}
   //destrói FormAlerta2
   delete FormAlerta;
   }

//---------------------------------------------------------------------------
//eof