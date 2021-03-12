// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TLogTObject.h"
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
// ---------------------------------------------------------------------------------------- includes
#include <iostream>
#include <fstream>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
__fastcall TLogTObject::TLogTObject(AnsiString fileDir)
{
	AnsiString filename;
	char *file;
	bool sucesso = false;
	AnsiString last;

	if (fileDir.IsEmpty())
	{
		filename = "C:\\tmp\\";
	}
	else
	{
		filename = fileDir +"\\";
	}
	// caso nao exista o diretorio
	if (!DirectoryExists(filename))
	{
		if (ForceDirectories(filename))
		{
			sucesso = true;
		}
	}
	else
	{
		sucesso = true;
	}
	// caso nao tenha  criado a pasta

	// abre arquivo
	filename += FormatDateTime("yyyy.mm.dd", Now());
	filename = filename + "_Log_DEBUG";
	filename += ".log";
	file = filename.c_str();
	// pFile = fopen(filename, "w");
//	nome_arq = file;
	// myfile.open (file, std::ofstream::out | std::ofstream::app);
	myfile = NULL;
	myfile = fopen(file, "a");
}

// ---------------------------------------------------------------------------
__fastcall TLogTObject::~TLogTObject(void)
{
	if (myfile != NULL)
	{
		fflush(myfile);
		fclose(myfile);
		myfile = NULL;
	}
}
// -----------------------------------------------------------------------------
void __fastcall TLogTObject::AddTexto(TObject *obj, AnsiString msg)
{
	AnsiString ponteiro;
	AnsiString classname;
	//adiciona endereço do objeto (valor do ponteiro)
	ponteiro.printf("%p",obj);
	TList *list;
	VTRede *rede;

	msg += "endereço:" + ponteiro +";";
	msg += "classe:" + obj->ClassName() +";";
	//adiciona items da lista
	classname = obj->ClassName();
    if (classname.AnsiCompare("TRede") == 0)
	{
		rede = (VTRede*)obj;
        AddTextoRede(rede,msg);
    }
	msg +="\n";

	if (obj->InheritsFrom(__classid(TList)))
	{
		list = (TList*)obj;
		AddTextoLista(list, msg);
	}
}
// -----------------------------------------------------------------------------
void __fastcall TLogTObject::AddTextoLista(TList *list, AnsiString msg)
{
	TObject *aux;

	if (list != NULL)
	{
		msg +="items na lista: " + IntToStr(list->Count) +";";
		for (int no = 0; no < list->Count; no++)
		{
			aux = (TObject*)list->Items[no];
			AddTexto(aux,msg);
		}
	}
}
// -----------------------------------------------------------------------------
void __fastcall TLogTObject::AddTextoRede(VTRede *rede, AnsiString msg)
{
	if (rede != NULL)
	{
		msg += "codigo:" + rede->Codigo +";";
		msg += "\nLisBARRA;";
		AddTextoLista(rede->LisBarra(),msg);
	}
}
// -----------------------------------------------------------------------------
void __fastcall TLogTObject::LogObject(TObject *obj, AnsiString msg)
{
	char *txt;
	char *filename;

	AnsiString data;
	AnsiString Separador = ";";
	AnsiString texto;

	// o formato ficará
	// data;msg;string;
	data = FormatDateTime("yyyy.mm.dd-hh.mm.ss.zzz", Now()) + Separador;
	texto = data;
	texto += msg + Separador;
	AddTexto(obj,texto);

	txt = texto.c_str();

	if (myfile != NULL)
	{ // inclui 'new line' e grava mensagem
		texto += "\n";
		fputs(texto.c_str(), myfile);
		fflush(myfile);
	}
}
// -----------------------------------------------------------------------------
// eof
